# tls_macos.m worklog

## 2026-07-17 — sentinel surgery + dead partial-read check

Part of the VANA adjudication (quaestio 01KXQ9NF8J class 1):
tls_mittere/tls_recipere returned i32 (unsigned house type) with a
documented -1 sentinel — every `n < 0` check on them was dead.
Signatures now s32 (matching the documented design), internal
(i32)-1/(i32)processed casts became -1/(s32)processed, and the two
SecureTransport callbacks' `i32 n` locals became s32 (their n<0
guards on tcp_recipere/tcp_mittere were dead the same way).

SECOND BUG found during the same read (_tls_read_callback): the
partial-read check ran AFTER `*dataLength = (size_t)n;` — i.e. it
compared n against itself, always false, so partial reads never
returned errSSLWouldBlock (the SecureTransport contract). Fixed by
capturing `petitum = *dataLength` at entry and comparing n against
that. The check was INTENDED (it existed!) but the ordering killed
it — same disease as the whole arc: designed right, implemented
dead.

NOTE: .m files are invisible to examen/percursus (walks .c/.h only)
— the VANA census could never have found this file. Grep is the
only meter here; remember that when auditing platform layers.

## 2026-07-20 — read callback remapped onto the TCP_ITERUM contract

tcp_recipere's 0-return used to mean both EOF and would-block, and
the read callback mapped 0 → errSSLWouldBlock — so a peer that
closed mid-read looked like "no data yet" and SecureTransport
would spin/wait instead of reporting closure. With TCP_ITERUM (-2)
the mapping is now honest: ITERUM → errSSLWouldBlock, <0 →
errSSLClosedAbort, 0 → errSSLClosedGraceful. Order matters: the
ITERUM check must precede n < 0 because the sentinel itself is
negative.

CAVEAT: this path is validated only by the live -reticularis TLS
run — compile is green but a manual re-smoke is still owed before
trusting it.

## 2026-07-24 — peer certificate accessor (villa V1)

Added `tls_certificatum_obtinere` — notBefore/notAfter as unix epoch
plus the subject summary. Nothing vendored; SecureTransport already
held everything needed.

**Why it exists**: villa's external probe wants "days until this cert
expires **as the world sees it**." That is a different fact from what
the box has on disk — a misconfigured nginx can serve a different
cert than the one in its config, and reading the file over ssh would
report the wrong one while also re-coupling the probe to ssh (which
defeats its whole purpose of working when ssh is down).

### Two traps worth recording

**1. The validity values are not CFDate.** `SecCertificateCopyValues`
returns, per OID, a dict shaped `{kSecPropertyKeyValue: <value>}`,
and for validity dates that value is usually a **CFNumber holding a
CFAbsoluteTime double** — not a CFDate, despite the property type
being `kSecPropertyTypeDate`. `_tempus_ex_valore` handles both by
checking `CFGetTypeID` so an OS revision can't silently break it.
And CFAbsoluteTime counts from 2001-01-01, so the unix conversion
needs `+ kCFAbsoluteTimeIntervalSince1970` (978307200) — omit it and
every cert looks 31 years closer to expiry than it is.

**2. Lifetime.** The accessor must be called while the connection is
OPEN; the trust object lives in the SSL context and dies with
`tls_claudere`. The gate pins both sides — a real cert before close,
and a clean `FALSUM` (not a crash) after.

`SecTrustGetCertificateAtIndex` is deprecated on macOS 12+ in favor
of `SecTrustCopyCertificateChain`; kept with the deprecation pragma
the file already uses for SSLHandshake etc., so behavior is uniform
across OS versions rather than split by `#if`.

Gate: extended `probatio_tls.c` (already inside the `-reticularis`
exclusion, so no runner edit was needed — a live handshake was
already the file's contract). Verified live against httpbin.org:
subject `httpbin.org`, ~162 days remaining. Suite 103/103.

## 2026-09-26 — tls_recipere returned 0 ("closed") on WouldBlock: silent truncation

Reported from lapide (briar-feedback bugs/015, bugs/016): HTTPS bodies
over ~4 KB came back as `successus` with 0 / 1371 / 5484 of 7351 bytes;
GitHub's 5 KB redirect headers failed as "Capita non inventa" or
"Responsum vacuum". Timing-dependent — the same URL gave different
lengths run to run.

**Root cause (instrumented, not guessed):** the 07-17 fix above made
the read callback honour SecureTransport's contract (partial TCP read
→ `errSSLWouldBlock`). SSLRead then returns `errSSLWouldBlock` with
`processed == 0` whenever a TLS record straddles TCP reads — and
`tls_recipere` passed that 0 straight up. Its own contract says 0 =
"connexio clausa", so http's drain loop took it as EOF mid-body. A
probe printing every SSLRead showed `-9803/0` in every truncated run
and never in a whole one. The 07-20 caveat ("manual re-smoke still
owed") was exactly this path; the live gate only asserted
`totalis > 0`, so it could not see it.

**Fix:** `tls_recipere` loops while WouldBlock with 0 bytes. The socket
is blocking (SO_RCVTIMEO), so the callback waits in `tcp_recipere` —
no spin. The loop ends only on data, closure, error, or a real
timeout, which the callback now records (`tempus_excessum`, set on
TCP_ITERUM) and `tls_recipere` returns as `TCP_ITERUM` — tls.h's
contract gains that value, and http's existing timeout branch now
covers TLS too (it used to say TLS timeouts surface as IO errors).
Same flag ends the handshake loops, which spun forever on a silent
peer (each spin one full timeout).

Gate: `probatio_tls` mittere/recipere now pulls 64 KB and asserts
body == Content-Length and loop exit == clean EOF. Plant (old
single-SSLRead behaviour) red 3/3 (0 or 235 bytes received); fixed
65536/65536. Live probe: README 7351, png 338806, llama.cpp release
11755622 through two redirects — all exact, 10/10 runs.
