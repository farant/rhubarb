# http_vocatio.worklog.md

## 2026-10-14 - born (http-vocatio T5, spec II.4)

The call state machine of the in-process HTTP(S) client (route B of
$question). One `HttpVocatio` = one request on the CALLER's reactor:
NOMINA (nomina.h) -> CONNECTIO (tcp_incipere, wait writable,
tcp_incipere_perficere; on failure the next address) -> PACTIO
(tls_gradus, https only) -> MITTENS -> LEGENS (the existing
http_lector from quaestio-rivus T1) -> FINITA | IRRITA | ABRUPTA.
Exactly one `finis` per call, never inside `http_vocare`.

- **The sink is guarded.** `http_lector_adicere` may deliver several
  body pieces from one read; if the caller aborts inside the first
  (Esc during a delta), the lector would keep calling the sink with the
  rest of the buffer. `_rivus_custos` drops everything after `finita`.
  Section `abrumpere intra rivum` (Review focus II); plant red.
- **Finish on the lector, not on close.** A keep-alive server answers
  fully and keeps the socket open; waiting for a close would stall to
  the idle timeout. Section `manet apertum` (Review focus I).
- **Drain until ITERUM** in `_legere` (TLS may hold decrypted bytes the
  socket no longer signals) - same loop serves TCP.
- **Remove from the reactor BEFORE closing** (`_connexionem_dimittere`):
  fd numbers are reused at once by the next socket - a stale
  registration under a reused number would dispatch to the wrong call.
- **Timeouts (plan R8):** one connect deadline over NOMINA + CONNECTIO +
  PACTIO; an idle limit re-armed on every byte moved in MITTENS/LEGENS;
  no total limit. Early outcomes (DNS service failed to start) go
  through a 0 ms timer (`_errorem_differre`) so `finis` is never
  synchronous with `http_vocare`.
- **On error `responsum` is NIHIL** (plan R7) - like
  `http_exsequi_rivus`; the streamed bytes already reached the sink.
- **The serialized request is zeroed** after sending and at finis (it
  may carry `x-api-key`, plan R2).
- **finis must not destroy the call's piscina** - code still runs after
  the callback returns (`_legere` checks `finita` after the lector).

Tests: `probationes/probatio_http_vocatio.c` (11 sections, all offline,
against `probationes/servus_scriptus.h` - a scripted TCP server on the
SAME reactor: one process, one loop, deterministic) passed on its first
run; every chunk split of a chunked response; Review focus I-V each
have a section. `probationes/probatio_http_vocatio_rete.c` (facultas
rete, run by hand): POST to api.anthropic.com WITHOUT a key -> 401
authentication_error = first real DNS + stepped TLS + stepped read
end to end; `.invalid` name -> named DNS error. The second section
found a nomina bug (no negative answers without
`kDNSServiceFlagsReturnIntermediates`) - see nomina_macos.worklog.md.
