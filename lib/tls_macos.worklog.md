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
