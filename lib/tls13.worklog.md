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
