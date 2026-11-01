# http2_nexus worklog

## 2026-10-31 - T8: long-lived HTTP/2 connection on the reactor

NOMINA -> CONNEXIO -> PACTIO (TLS with ALPN {"h2"} required) -> VIVUS -> CLAUSUS, modelled on `http_vocatio.c`, but addresses are tried one after another (no RFC 8305 parallel attempts - one long-lived connection, not a request per connection). The `Http2Connexio` is created at open, so requests made before the handshake simply queue in its output (preface first) and go out when VIVUS.

- **ALPN decides**: anything but "h2" -> status(FALSUM, "hospes h2 non elegit (ALPN)") and all requests fail; no HTTP/1.1 fallback inside this object (spec III.4). Tested against `openssl s_server` without -alpn.
- **Callbacks**: a request's slot is cleared *before* its finis runs, and after every callback the loop re-checks state, so a finis may call `http2_nexus_petere` or `http2_nexum_claudere`. Requests made after GOAWAY or close finish through a 0 ms timer, never inside `petere` (contract from `http_vocatio`), with `retentabilis` set.
- **Status callback**: VERUM once when live; FALSUM once on death *or* on GOAWAY (rivus-0 DISCESSUS event) - pending lower streams still finish; when the last one finishes after a GOAWAY the connection is closed quietly.
- **Timers**: connect timer covers NOMINA+CONNEXIO+PACTIO; afterwards one idle timer, armed only while requests are pending and reset on every received chunk. Idle expiry kills the connection (for APNs a silent connection with a pending push is broken).
- **Reading**: loop `tls_recipere` until TCP_ITERUM (SecureTransport can hold decrypted bytes the socket no longer signals), then flush output (ACKs, WINDOW_UPDATEs) once.
- **Tap** (`tabula_via`, tests only): decrypted octets each direction to `<via>.servus.bin` / `<via>.cliens.bin` via `filum_scriptor_*` (examen 97: no raw IO).
- Go's server advertises MAX_FRAME_SIZE 1 MiB; our DATA frames are still bounded by free output space (256 KiB buffer) - fine.
- Live: `RHUBARB_RETE=1 ./build/probationes/probatio_http2_nexus` fetches nghttp2.org and www.google.com over our own HTTP/2 + TLS 1.3 (2026-10-31: both 2xx/3xx).

## 2026-10-31 - final review fixes

`http2_nexum_claudere` now destroys the h2 connection (and the creation-failure path frees what it made); the header says claudere must be called even after death. `_perire` keeps a deferred request's own causa/retentabilis (as claudere already did) and marks never-sent requests (still queued above the peer's MAX_CONCURRENT_STREAMS) retryable. Test: 251 slow GETs against Go's default limit of 250, server killed -> the 251st comes back `retentabilis` VERUM, the others FALSUM. Go 1.23's stdlib can't lower MaxConcurrentStreams (http.Server.HTTP2 is 1.24), hence 251.
