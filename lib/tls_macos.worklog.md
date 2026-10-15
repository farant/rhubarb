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

## 2026-10-07 - hardening: TLS 1.2 floor + cipher allowlist (vates-plan-1 T1b)

**Measured first** (`tools/rete_fumus.sh`, T1): SecureTransport with its
defaults negotiated TLS 1.2 with good servers, but ACCEPTED
tls-v1-0.badssl.com:1010 and tls-v1-1.badssl.com:1011, and OFFERED
`TLS_ECDHE_{ECDSA,RSA}_WITH_3DES_EDE_CBC_SHA` - howsmyssl rated us "Bad".

**Fix** (`_durare`, called after SNI, before the handshake):
`SSLSetProtocolVersionMin(kTLSProtocol12)`, then the supported suites
filtered through an ALLOWLIST (`_cifra_admissa`: ECDHE + AES-GCM or
ChaCha20-Poly1305 only) into `SSLSetEnabledCiphers`. Allowlist, not
denylist: a suite Apple adds later stays out until named here. Empty
filtered list or any setter failure = connection refused with a named
`TLS_ERROR_HANDSHAKE`.

**After**: howsmyssl "Improvable" (not "Probably Okay" - the remaining
gap is TLS 1.3, which SecureTransport cannot do; Network.framework or
BearSSL would be the route), insecure suites: none; badssl certificate
refusals 4/4 still; TLS 1.0/1.1 refused; TLS 1.2 accepted;
api.anthropic.com still fine.

**Plants - each line protects on its own, but not observably alone:**
- min-version removed (allowlist kept): probe stays GREEN - TLS 1.0/1.1
  cannot negotiate any AEAD suite, so the allowlist alone refuses them.
- allowlist removed (min-version kept): 3DES offered again, rating
  "Bad", but TLS 1.0/1.1 STILL refused - the floor is load-bearing
  whenever the ciphers are not restricted.
- The probe used to only PRINT howsmyssl's insecure suites, so the
  second plant first stayed green (a dead check). It now ASSERTS
  "nullae cifrae insecurae oblatae"; with the allowlist plant that line
  is FRACT. The floor's own plant has no observable red through public
  servers (TLS 1.0 + AEAD does not exist) - kept as defense in depth.

## 2026-10-14 - stepped (non-blocking) TLS + context leak (http-vocatio T4)

For the reactor-driven client (route B of $question): `tls_incipere`
takes an already-connected NON-BLOCKING `TcpConnexio` (from
`tcp_incipere`), prepares the context and sends nothing; `tls_gradus`
runs ONE `SSLHandshake` step and says what to wait for
(`TLS_GRADUS_LEGERE` / `_SCRIBERE` / `_PERFECTUS` / `_ERROR`). The
connection is marked `gradatim`; the blocking `tls_connectere` path is
unchanged and shares the setup through `_contextum_parare` (one
function for SNI, TLS 1.2 minimum, the cipher allow-list, verification).

- **"Would block" is not "timed out".** The read callback used to set
  `tempus_excessum` on `TCP_ITERUM`: on today's blocking sockets EAGAIN
  means `SO_RCVTIMEO` fired. On a non-blocking socket it only means
  "nothing yet" - in gradatim mode the flag is never set.
- **What to wait for** comes from the callbacks: the write callback
  sets `scribere_pendet` on EAGAIN (-> SCRIBERE), otherwise a
  WouldBlock handshake waits for readability.
  `errSSLServerAuthCompleted` (verification off) just steps again.
- **Reads drain until TLS_ITERUM.** SecureTransport can hold decrypted
  bytes the socket no longer signals; a caller that reads once per
  readiness can stall with data sitting in the TLS buffer. Contract in
  `tls.h`; http_vocatio (T5) drains.
- **Writes follow the libcurl SecureTransport quirk:** after
  `errSSLWouldBlock` with 0 processed, the data is buffered INSIDE;
  remember its length, flush with `SSLWrite(NULL, 0)` until noErr, then
  report the whole length. The caller re-calls with the same bytes.
- **Context leak (plan R5):** `tls_claudere` skipped `SSLClose` AND
  `CFRelease` when `clausa` was set - and `tls_recipere` sets it on a
  clean peer close - so every peer-closed TLS connection leaked its
  SSLContext. Now the context is always released (SSLClose only if we
  did not see the close).

Tests `probatio_tls_gradus.c` (no network; the local peer is not a TLS
server): first step sends a ClientHello (record 0x16 0x03) and returns
LEGERE at once, repeatedly, never "timeout"; garbage answer -> ERROR
within the bound; peer close -> ERROR and `tls_claudere` really closes
the fd. NOT verifiable offline (recorded, not planted): the context
release itself, the gradatim `TLS_ITERUM` read of application data, and
the `SSLWrite(NULL, 0)` flush - T5's rete file and the live check carry
the read path.

## 2026-10-15 - trust anchors + an offline TLS server for tests

`TlsOptiones.ancora_via` (DER CA): the ONLY trusted root for this
connection - for tests (our own CA) and later pinning. With an anchor
the handshake breaks at server auth (`kSSLSessionOptionBreakOnServerAuth`)
and `_fiduciam_iudicare` evaluates the peer trust against the anchor
ONLY (`SecTrustSetAnchorCertificatesOnly`), with an EXPLICIT SSL policy
carrying the host name (`SecPolicyCreateSSL(true, host)`) - chain,
validity and name are all checked; failure = `TLS_ERROR_CERTIFICATUM`
(blocking) / `TLS_GRADUS_ERROR` with the Security error text (stepped).
Both paths; anchors released in `tls_claudere`.
`HttpVocatioOptiones.tls_ancora_via` passes it through http_vocatio.

**Apple rejects long-lived server certs.** First run: "certificate is
not standards compliant" - macOS requires TLS server (leaf)
certificates valid <= 825 days. Committed fixtures would therefore rot
within two years, so certificates are GENERATED AT TEST TIME
(`probationes/fixa/tls/generare.sh <dir>`: test CA, localhost valid 365
days, alienus = wrong host, expiratus = 2020, ignotus = other CA; EC
P-256) - and no private key is ever committed.

Test `probationes/probatio_tls_servus.c` with `probationes/servus_tls.h`:
`/opt/homebrew/bin/openssl s_server -WWW` as a child-process ORACLE
(like bash for crusta), serving files from the suite's credo area
(deleted automatically after the suite). Sections: parvum (full
stepped handshake + read), magnum (256 KB: many records in one burst -
the drain-until-ITERUM rule), alienus / expiratus / ignotus (each a
named rejection), sine ancora (system store does not know our CA -
verification is really on), obstruens (blocking path, same anchor).
Before the anchor existed the four rejection sections passed for the
WRONG reason (everything was rejected); plants on the judgement and
the host-name policy prove each one now tests its own failure.

## 2026-10-15 - the context leak is finally observable: tools/tls_effluvia.sh

R5 (tls_claudere skipped SSLClose/CFRelease after a peer close) was
fixed in T4 but no test could SEE it. `tools/tls_effluvia.sh` runs the
TLS test binaries (probatio_tls_servus - a real handshake against
s_server, which closes after answering = exactly the R5 path - and
probatio_tls_gradus) under macOS `leaks --atExit`. Planting the old
guard back: 24 -> ~205 leaks, reported as `ROOT CYCLE: <SSLContext>`
(plus `malloc in SSLCreateContext`, `malloc in tls_handshake_create`).

Lessons from building it:
- our leak is a ROOT CYCLE, not a ROOT LEAK. The first runner counted
  only `ROOT LEAK` lines and stayed GREEN with the plant in place - the
  verdict now compares the summary's TOTAL against tolerated instances.
- Apple leaks inside Security.framework: `CryptKit::FEEKeyInfoProvider`,
  32 bytes per EC trust evaluation (~24 per run, count varies). Listed
  with its reason in `tools/tls_effluvia.tolerata`; nothing of ours may
  ever stand there.
- `leaks` sometimes leaves the inspected process suspended (state T):
  the runner waits for the summary (bound 180 s) then kills both; no
  summary = exit 2 (a mute check is never green).
- BSD sed has no `\|` in basic regexes (-E needed) - same trap as `\b`.
Manual tool (not a gate): `./tools/tls_effluvia.sh [-aedificare]`.

## 2026-10-15 - trust via fiducia (tls-bearssl T2)

The anchor path no longer owns SecTrust code: `_ancoram_legere` and the
SecTrust half of `_fiduciam_iudicare` moved to `lib/fiducia_macos.m`
(`fiducia_iudicare`), as did `_tempus_ex_valore`/`_proprietatem_capere`
(`fiducia_folium_describere`). This backend now copies the peer chain
out as DER (`_catenam_capere`: SSLCopyPeerTrust -> SecCertificateCopyData,
into the connection piscina) and hands it to fiducia - the same entry
the BearSSL backend will use, so both judge trust with ONE policy
(SecPolicyCreateSSL(true, host), anchors-only when ancora_via given).
`hospes`/`ancora_via` are piscina copies (option strings need not
outlive setup); no CF objects left in the struct (ancorae/hospes_cf
gone, so nothing to CFRelease in tls_claudere besides the context).

Behaviour kept: an unreadable anchor still fails at context setup
("ancora legi non potest", via filum_existit - examen code 96); a
readable but non-DER anchor now fails at server-auth with fiducia's
named reason instead of at setup (same TLS_ERROR category).

Mute by design (offline): SecTrustSetAnchorCertificatesOnly true->false
changes nothing - the system store knows neither the test CA nor the
rejected certs. Only the network -badssl run (T6) checks it.
