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

## 2026-10-16 - T7: fallback to BearSSL 1.2 (dispatcher in lib/tls.c)

- **Trigger** only SOLUM_1_2 (a 1.2 ServerHello, or protocol_version in
  answer to our ClientHello). On a stepped connection: the 1.3 backend
  hands its TcpConnexio back unclosed (`tls_tls13_tcp_relinquere`),
  `tcp_redintegrare(tcp, NIHIL)` starts a new connect onto the same fd
  number, tls_gradus returns SCRIBERE until a zero-timeout poll says
  writable, then `tcp_incipere_perficere` and BearSSL with the sentinel
  flag; `tls_vectura` flips to BEARSSL. Blocking: same, with the
  caller's timeout as socket options and BearSSL's blocking handshake.
- **Old 1.2 servers** (pre-1.3 code that ignores supported_versions)
  would see only 1.3 cipher suites and answer handshake_failure - not a
  trigger. So our ClientHello also offers BearSSL's six 1.2 suites
  (R23), like browsers do: such a server picks one and answers with a
  1.2 ServerHello -> fallback. OpenSSL 3 limited to 1.2 answers
  protocol_version either way (checked with s_client -tls1_3).
- **Sentinel:** after a fallback, BearSSL's server_random[24..31] is
  checked for "DOWNGRD" + 0x00/0x01. OpenSSL 3.3.2 really sets it, so
  the `signum` test (forced fallback against a 1.3-capable s_server)
  sees the real thing. A connection that fails the check refuses
  mittere/recipere too (`vetita`), not only gradus.
