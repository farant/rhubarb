# tls13_nuntii worklog

## 2026-10-16 - T3 (tls13-plan)

Readers take message BODIES (the engine has read the 4-byte header)
and every one ends with an exact-consumption check. Consequence found
while planting: a truncation sweep cannot see a broken bounds check -
the truncated input is still rejected (consumption mismatch) and the
over-read lands in the rest of the full message buffer, valid memory, no
crash. The sweep's crash pass now places each prefix flush against an
unmapped region (mmap 2 x 64 KiB, munmap the upper half; no mprotect -
it is not in examen's POSIX lexicon), so one byte past the end faults.
General lesson: "rejects truncation" and "never reads past the end" are
different properties; the second needs a guard region (or ASan).

RFC 8448 §5's HRR has a real cookie extension (0x002c, 116 octets incl.
the inner length; 114-octet cookie) - the test reuses it and checks our
CH2 echoes it exactly as the RFC's CH2 does.

A ServerHello cut right after the compression byte IS a valid TLS 1.2
ServerHello without extensions (versio_1_2) - the sweep names that one
prefix as licit.

ClientHello order: server_name, supported_groups, signature_algorithms,
supported_versions, cookie (CH2 only), key_share. CH2 keeps
supported_groups {x25519, secp256r1} (RFC 8446 4.1.2: only key_share,
cookie, early_data, psk, padding may change).

## 2026-10-31 - ALPN (http2-plan T7)

- ClientHello gains extension 0x0010 only when `Tls13SalutatioParametri.protocolla` is given; without it the hello is byte-identical (the RFC 8448 replays in probatio_tls13_* still pass, and the ALPN suite checks the length drops by exactly the 18 extension octets for {"h2","http/1.1"}). Names must be 1..255 octets or the writer refuses (fractus).
- `tls13_extensiones_legere` now also returns the ALPN name from EncryptedExtensions; a list that does not hold exactly one non-empty name is malformed (RFC 7301 3.1). CertificateRequest passes NIHIL. The machine (`lib/tls13.c`, STADIUM_EE) checks the name was offered (`tls13_protocollum_oblatum`), else illegal_parameter (RFC 7301 3.2), and keeps it for `tls13_machina_protocollum`.
- BearSSL: `br_ssl_engine_set_protocol_names` before `br_ssl_client_reset`, names copied into the connection piscina; `br_ssl_engine_get_selected_protocol` after the handshake. The descensus path (TLS13 -> BearSSL on the same connection) goes through the same `_parare`, so ALPN survives the fallback (tested against `openssl s_server -tls1_2 -alpn h2`).
- SecureTransport: not wired (C7) - `tls_protocollum` returns empty there.
- Gotcha: `nomen` (typedef macro) as a Cursor variable name -> examen REICE; `servus_tls.h` helpers are `interior` - a suite must call `servum_tls_incipere` or -Wunused-function fails the build.
