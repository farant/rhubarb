# fiducia_posix.worklog.md

## 2026-10-22 - the Linux verifier (fiducia-linux T3)

Replaces the placeholder that refused every chain. Order: arguments ->
house blocklist -> DER form of every certificate (fiducia_der) ->
BearSSL br_x509_minimal (anchors, times, DNS name) -> SAN-only (BearSSL
would fall back to the CN, bearssl_amalgama.h:8955) -> IP hosts against
iPAddress SANs (start_chain with NIHIL as name; BearSSL cannot extract
iPAddress).

- **Trust store**: first existing path of the spec list; Ubuntu 22.04
  loads 123 anchors, Alpine 3.20 121 (`/etc/ssl/certs/ca-certificates.crt`
  on both; Alpine's `/etc/ssl/cert.pem` is a symlink to it). No env
  override. `ancora_via` -> that DER alone.
- **CRLF**: BearSSL's PEM decoder does not tolerate '\r' - a CRLF bundle
  lost every certificate (fasciculus_mixtus red: 0 anchors, 3 skipped).
  The text is copied without CR before decoding.
- **Cost, measured**: a full bundle parse is ~26-29 ms per call (not
  "a few ms" as the spec guessed), so the system store is cached per
  process. filum_status has no mtime (adding one = a filum.h change
  owing ~29 gates), so the key is path + size + SHA-256 of the content:
  every call re-reads and hashes the file and rebuilds only on a real
  change. Warm judgment ~2 ms (musl 2.04, glibc 1.86).
- **Reasons**: BearSSL X.509 codes mapped to Latin with the chain
  position; unknown codes by number. Checked by `causae` /
  `algorithmi` / `hospes ip` on Linux (macOS refuses the same inputs
  with SecTrust's own words - those sections assert refusal only there).
- **First Linux run of the TLS suites**: probatio_tls_servus,
  tls_bearssl and tls13_servus had never compiled under gcc (512-byte
  path buffers, -Wformat-overflow) and servus_tls.h hard-coded
  Homebrew's openssl; now /usr/bin/openssl is the fallback. tls_servus's
  transport matrix runs SecureTransport only on macOS (on Linux it is
  a named stub). With that: tls_servus [tls13]+[bearssl] 10/10 each,
  tls_bearssl 80, tls13_servus 157 - real handshakes against
  openssl s_server judged by this file.
- Plants (by hand on Linux - silva.planta judges with a macOS gate):
  SHA-1 registered -> 3 red; SAN-only removed -> 3; IP check skipped ->
  6; blocked anchors kept -> 2; restored -> green. (My first plant
  script read section names from probatio.sh output, which only keeps
  the last 6 lines - it reported all four "mute"; judge by the Fracti
  count.)
- examen: `inet_pton` is not in the lexicon; an unknown FUNCTION is only
  suspect (ACCIPE), so the lexicon was left alone.

## 2026-10-22 - live from Linux (fiducia-linux T4)

`tools/fiducia_vivum.c` + `tools/linux/fiducia_vivum.sh` (by hand,
network): 16/16 expectations on glibc and musl, tls13 and bearssl -
see spec AUDIENDA "Answered live" for the table. Two lessons:
- An invented R2 account id gets `handshake_failure` from Cloudflare
  BEFORE any certificate - the server refusing, not us; the bare
  `r2.cloudflarestorage.com` serves a certificate expired 2026-09-09
  (we refuse it; `openssl s_client` only "connects" because it does not
  verify by default). cloudflare.com stands in for the R2 chain; real R2
  was proven by horreum.
- `tools/linux/horreum_vivum.sh TITULUS` builds bin/horreum in the
  container (whole posix closure: fiducia_posix, nomina_posix, tls13,
  bearssl) and HEADs `horreum-probatio/nusquam-linux` with the one
  profile mounted read-only. Fran ran it: 404 from DO Spaces and from
  R2 - TLS verification and SigV4 both work from Linux.
