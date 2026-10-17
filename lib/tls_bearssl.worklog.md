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

## 2026-10-15 - T5: blocking path + certificate details

`tls_bearssl_connectere`: tcp_connectere_cum_optionibus (timeout_ms ->
socket timeouts), then the SAME `_antlia` until SENDAPP. In blocking mode
LEGERE/SCRIBERE can only mean the socket timed out -> TLS_ERROR_TCP
"Tempus handshake excessum" (SecureTransport's category and text);
rejection by fiducia -> TLS_ERROR_CERTIFICATUM with fiducia's reason;
other failures -> TLS_ERROR_HANDSHAKE with the engine's named error.
`tls_bearssl_certificatum`: catena[0] (collected by the Iudex during the
handshake, lives in the piscina) -> fiducia_folium_describere - the same
code path as the SecureTransport backend.

Parity check (no code from T6 yet): with RHUBARB_TLS=bearssl the EXISTING
suites pass in full - tls_servus 9/9 (incl. obstruens + certificatum),
tls_gradus 3/3, http_vocatio 14/14. Plants: category CERTIFICATUM ->
HANDSHAKE, timeout TCP -> HANDSHAKE, subject dropped - all red.
Known mute (recorded in plan T5): catena[last] instead of catena[0] -
s_server sends the leaf only; needs an intermediate-CA fixture.

## 2026-10-15 - T6: matrix, BearSSL default, 1.2 floor, leaks, network

Matrix: probatio_tls_servus and probatio_tls_gradus run their whole
section table twice (RHUBARB_TLS=bearssl, then =securetransport) -
10/10 and 3/3 under each. Plant: fiducia skipped in the BearSSL backend
only -> tls_servus red in its [bearssl] run (the pre-T6 suite, which
ran SecureTransport only, would have stayed green).

Default flipped: TLS_VECTURA_DOMUS = BEARSSL (lib/tls.c);
probatio_tls_vectura changed first and seen red.

1.2 floor: new section tls11 against `s_server -tls1_1 -cipher
DEFAULT:@SECLEVEL=0` (OpenSSL 3.3 refuses 1.1 at the default level -
"no protocols available"). Both backends refuse. The floor is enforced
TWICE in BearSSL (versions 1.2..1.2 AND an AEAD-only TLS 1.2 suite list:
the 1.1 server offers ECDHE-ECDSA-AES256-SHA, CBC) - so widening the
version range alone is mute. The plant that proves the test bites
replaces the whole config with br_ssl_client_init_full (TLS 1.0-1.2,
all suites incl. CBC): red at IRRITA. Closes T4's mute version plant.

Leaks (tools/tls_effluvia.sh, both binaries, both backends via the
matrix): residuum 0. tls_servus 23 instances, all the tolerated Apple
CryptKit::FEEKeyInfoProvider (one per SecTrust evaluation with an EC
key - more evaluations now, since both backends go through fiducia and
the suite runs twice). Runner: silenced a harmless grep race (log
polled before `leaks` created it).

Network (by hand, public hosts): rete_fumus -tls -badssl -> identical
verdict tables under both backends (howsmyssl TLS 1.2, no insecure
suites, "Improvable" for both - no TLS 1.3; badssl expired, wrong.host,
self-signed, untrusted-root, tls-v1-0, tls-v1-1 refused; tls-v1-2
accepted). The self-signed/untrusted-root rows exercise the SYSTEM-store
path T2's offline AnchorsOnly plant could not reach. probatio_tls and
probatio_http_vocatio_rete green under both.
