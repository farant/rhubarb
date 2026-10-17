# tls13.c worklog

## 2026-10-16 - T4: engine, 1-RTT path (RFC 8448 §3 and §7 byte for byte)

Green 10/10 on the first run of the full replay: §3 ClientHello record,
client Finished, application data, close_notify, and §7 CCS + Finished
all match the RFC byte for byte, with BearSSL really verifying the RFC
server's RSA-PSS CertificateVerify against the leaf key from
br_x509_decoder. The key schedule, record layer and codecs (T1-T3) were
each byte-exact already, so the engine was mostly wiring - the replay
is what proves the wiring order (transcript points, key switches).

Design notes worth keeping:

- **Write keys switch at ServerHello, not at our Finished.** `scribere`
  becomes the c hs protection as soon as the handshake secrets exist,
  so a fatal alert raised while reading the server flight (bad
  certificate, bad signature, bad Finished) goes out sealed under c hs
  at sequence 0 - what the server's read side expects. The tests open
  that alert with the RFC's c hs secret, so the choice is pinned.
- **The dummy CCS is lazy:** emitted right before our first sealed
  record when legacy_session_id is non-empty (P3). Normally that is the
  Finished flight; on an error it precedes the alert. Same rule, one
  place (`_recordum_mittere`).
- **Key-change boundary (RFC 8446 5.1):** after a message that changes
  keys (ServerHello, server Finished) nothing may remain in the
  reassembly buffer; leftovers = PARAMETRUM / unexpected_message.
- **One data buffer, ordered:** a data record is not opened while
  earlier plaintext is unread. `_procedere` stalls; reading
  (`tls13_data_introitus_ack` to empty) resumes it. Section
  data_coniuncta (NST + two data records in ONE feed) pins this - the
  plan's s3_data fed records one at a time and could not see it.
- **CertificateVerify:** trust callback FIRST, then the signature
  (64 x 0x20 || context || 0x00 || Transcript-Hash through Certificate,
  hashed with the scheme's hash). RSA-PSS salt length = hash length;
  ECDSA curve must match the scheme (P-256 for 0x0403, P-384 for
  0x0503).
- **Fixed ClientHello seam (P2):** the engine reads the session id out
  of the fixed CH (offset 38) so the echo check and the CCS rule work on
  the RFC's bytes too.

Not yet (T5): HelloRetryRequest (now PARAMETRUM), KeyUpdate (now
unexpected), alert names in the causa (now the number only).
CertificateRequest is "unexpected" - no client certificates in the spec;
a server asking for OPTIONAL client auth would fail the handshake.

## 2026-10-16 - T5: HelloRetryRequest, CertificateRequest, KeyUpdate, alerts

21/21 on the first run after implementing. Three RFC 8448 traces now
replay byte for byte through the engine: §3 (RSA-PSS), §5 (HRR -> P-256,
the `message_hash` substitution exercised for real) and §6 (server
ECDSA P-256 with a CertificateRequest).

- **§6 surprise:** the §6 SERVER certificate is ECDSA P-256 ("ecdsa256",
  CV 0x0403). RFC 8448 §2's RSA private key is the §6 CLIENT's key. So
  the §6 replay is our ECDSA verification oracle (T4 only had RSA-PSS),
  and a modified §6 flight cannot be re-signed (EC key unpublished).
- **CertificateRequest context must be empty in the handshake** (RFC
  8446 4.3.2; non-empty contexts are post-handshake auth, which we
  never offer). We reject a non-empty one with illegal_parameter rather
  than echo it - this replaced the "echo" design of R17, and the check
  sits before the signature, so its test only re-seals a modified flight.
- **Answer without identity:** empty Certificate `0b 00 00 04 00 00 00
  00` + Finished in ONE record under c hs. Order in `_finis`: derive c ap
  / s ap from the transcript through the SERVER Finished first, then add
  our Certificate, then compute our Finished. The test checks the
  consequence that proves the order: our close_notify equals §6's client
  alert record byte for byte even though our Finished differs from the
  RFC client's (which sent a real certificate).
- **HRR:** once only (a second = unexpected_message, RFC 8446 4.1.4);
  the only acceptable group is secp256r1 (offered, not sent). CH2 reuses
  CH1's random and session id; the dummy CCS goes before CH2 when the
  session id is non-empty (RFC 8446 D.4) - §5 has an empty one, so that
  branch waits for the s_server `-groups P-256` interop test (T6).
  Nothing may follow the HRR in the same record (treated like a key
  change).
- **P-256 shared secret** = x coordinate of `mul(peer, priv)` at
  `xoff()`; the plant shifting that offset by one goes red in §5.
- **KeyUpdate:** receive keys rotate at once; when requested we send
  KeyUpdate(update_not_requested) under the OLD write key and rotate
  after. Must end its record (same key-change rule).
- examen caught a writable `hic_manens` buffer in a test section (state
  that would survive into the next section) - made it a local.

## 2026-10-16 - T8: TLS 1.3 is the house default

- **Matrix III:** probatio_tls_servus (10 sections) and probatio_tls_gradus
  (3) run under tls13, bearssl and securetransport - all green, no
  behavioural difference. Under tls13 the default s_server negotiates
  1.3; `tls11` (s_server -tls1_1) is refused through the fallback
  (protocol_version -> BearSSL 1.2, which refuses 1.1).
- **Default flip** test-first (probatio_tls_vectura VECTURA_DOMUS red,
  then `TLS_VECTURA_DOMUS = TLS_VECTURA_TLS13`). Every http / tls suite
  green under the new default, including probatio_http_vocatio_rete.
- **Leaks** (`tools/tls_effluvia.sh -aedificare`, now also over
  probatio_tls13_servus and probatio_tls13_descensus): untolerated
  residue 0 everywhere; the tls13 suites leak 0 bytes (everything lives
  in the connection piscina); the matrix's 27 tolerated leaks are the
  known Apple-internal kinds.
- **Network** (`tools/rete_fumus.sh -tls -badssl`, by hand) under all
  three: identical verdicts - expired / wrong.host / self-signed /
  untrusted-root / tls-v1-0 / tls-v1-1 refused, tls-v1-2:1012 accepted
  (via the fallback under tls13). howsmyssl: **TLS 1.3** under tls13,
  1.2 under the others; "Improvable" under all three because no session
  tickets are offered (resumption is out of scope, spec II.5).
- **Live (Fran, 2026-10-16):** vicus `$question` (route B -> http_vocatio
  -> tls -> our TLS 1.3 client -> api.anthropic.com) works under the new
  default. Hand-run smoke tests owed since T6/T7 green:
  apps/forum/fumus.sh (FUMUS PLENUS), tools/icones_fumus.sh (sanum).

## 2026-10-16 - final review fixes (fresh Opus reviewer)

No Critical findings. Fixed with RED->GREEN tests: cookie-only HRR
(reuse the x25519 share - `s_server -stateless` turned out NOT to send an
HRR to us, so the test is engine-level with a hand-built HRR whose random
comes from RFC 8448 §5), rsa_pkcs1 signature algorithms for strict 1.2
servers, the engine's own DOWNGRD check on a 1.2 ServerHello (RFC MUST:
abort 47, no fallback), and tcp_redintegrare after an RST (peer address
remembered at connect). Deferred minors and rulings: plan, "Final
review".
