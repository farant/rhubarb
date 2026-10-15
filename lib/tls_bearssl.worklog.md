# tls_bearssl worklog

## 2026-10-15 - T4: stepped path + validator (tls-bearssl-plan)

The pump (`_antlia`) is ONE function for both modes: SENDREC first (always
flush records), then the wanted state (`sitis`: SENDAPP for handshake /
send, RECVAPP for receive), then RECVREC. "Would block" = LEGERE/SCRIBERE;
in blocking mode the socket's own SO_RCVTIMEO makes tcp_recipere return
TCP_ITERUM, so the same code reads as a timeout (ruling R9, T5 uses it).

Validator (`Iudex`, a br_x509_class): collects every certificate's DER into
the piscina, feeds the LEAF to br_x509_decoder (get_pkey returns its key,
usages KEYX|SIGN), and in end_chain hands the chain to fiducia_iudicare -
the same SecTrust policy the SecureTransport backend now uses (T2).
verificare=FALSE skips only the fiducia call. Limits: 16 certs x 16 KB ->
named "catena certificatorum nimis magna aut vacua".

EOF during the handshake vs. after: before `pacta` it is the named error
"connexio a pare clausa in pactione"; after, a clean close (0), as
SecureTransport's errSSLClosedNoNotify was treated.

First run of probatio_tls_bearssl: 8/8. Running the EXISTING tls_servus
suite with RHUBARB_TLS=bearssl: 7/9 - every section except the blocking
path (obstruens) and certificate details (certificatum), i.e. T5. That
includes the 256 KB body (many records per read) and all rejections.

Plants: fiducia skipped, decoder not fed, flush dropped, recvapp not acked
- all red. The plan's "EOF -> LEGERE" plant was MUTE twice, for two
reasons worth keeping:
 1. the first test version closed the server before reading ClientHello ->
    RST -> the I/O error branch, never EOF (test gap: fixed - the server
    now reads the hello, then closes cleanly);
 2. even reaching EOF, returning LEGERE is harmless: the next read on the
    closed tcp returns -1 and the ERROR keeps the first (EOF) message
    (_errorem_ponere sets only an empty error) - redundant by design.
The sharper plant (EOF treated as clean close: `si (c->pacta)` -> VERUM)
turns red against the tightened assertion "a pare clausa in pactione".
Version floor plant (TLS10..12) is mute against s_server (negotiates 1.2):
closed in T6 by a -tls1_1 server section.
