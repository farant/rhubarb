# tls_tls13.c worklog

## 2026-10-16 - T6: the backend (engine pumped over TcpConnexio)

19/19 against openssl 3.3.2 s_server on the first run, and every
existing TLS suite (bearssl, securetransport matrix, gradus, vectura)
still green. The engine was already right; the backend is
`lib/tls_bearssl.c` transcribed onto the engine's buffer API.

- **Pump order:** records to send ALWAYS first, even after an engine
  error (that is how the fatal alert gets out); then the error check;
  then "is what the caller wants here"; then CLAUSA; then read. The
  plant that only sends when the engine is not expecting input
  deadlocks the handshake (ClientHello never leaves) - red, but every
  section then waits out its timeout, so it was run with
  CREDO_SECTIO=gradus.
- **EOF rule** keyed on the ENGINE (`tls13_machina_versio != 0`), not on
  a `pacta` flag the stepped/blocking callers must remember to set:
  after the handshake a bare FIN is a clean end (0), during it a named
  error.
- **Errors after the handshake** (e.g. the server's certificate_required
  alert arrives on the first read, since in TLS 1.3 the client finishes
  its handshake before the server judges the client's Certificate):
  `tls_recipere` returns -1 and `tls_gradus_error` carries the engine's
  causa ("nuntius fatalis a pare: 116 (certificate_required)").
- **`s_server -WWW` DOES send close_notify** after the body, so it can
  not test "EOF without close_notify". servus_tls.h gained
  `servum_tls_incipere_modo` ("-rev" echo mode): the test reads the echo,
  then kills s_server - the kernel closes the socket with a FIN and no
  alert. That is the honest EOF test (section eof).
