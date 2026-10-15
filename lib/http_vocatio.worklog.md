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

## 2026-10-15 - staggered parallel connects ("happy eyeballs lite", RFC 8305)

The latent failure Fran agreed to fix first: CONNEXIO tried addresses
strictly one after another and moved on only when an attempt FAILED.
An address that never answers (IPv6 on a broken network: SYNs vanish,
no RST) held the call until the connect-phase deadline (30 s), and the
call failed without ever trying the IPv4 address that would have
worked.

Now each address is an attempt slot (`HttpConatus`: own fd
registration, own callback). The first starts at once; if none has
succeeded after `mora_gradus_ms` (default CCL = RFC 8305's 250 ms
"connection attempt delay"), the next starts IN PARALLEL, and so on. A
failed attempt starts the next one IMMEDIATELY (RFC 8305), not at the
next stagger tick. The first attempt to connect wins; every other
pending attempt is removed from the reactor and closed at once (not at
finis - a loser would otherwise hold an fd for a whole stream). The
call fails only when every address has failed (last named error) or
the existing connect deadline expires. Addresses are interleaved by
family (`nomina_familias_alternare`, RFC 8305 §4) - nomina now applies
it before finis, and it is public for pre-resolved addresses.

`HttpVocatioOptiones` gained `mora_gradus_ms` and pre-resolved
`addressus`/`addressus_numerus` (test seam - started through a 0 ms
timer so finis is still never inside http_vocare). Callers must zero
the struct (`memset`) - two old tests set fields one by one and would
have read garbage in the new ones.

Test trick: a local BLACKHOLE without network. macOS completes connects
into a listener's queue until `kern.ipc.somaxconn` (128) and then drops
SYNs silently - `_foramen_parare` fills a never-accepting listener
until a connect really stalls, after which every new connect to that
port hangs like a dead IPv6 address (~130 fds; default limit 256).
Sections: foramen ([blackhole, live] finishes < M ms, and mid-call the
loser is already gone), recusatus deinde vivus (refusal moves on < L
ms, not at the stagger), omnes mortui (named timeout, nothing left).
The mid-call check first expected 3 fds and saw 2: the client had sent
before the server's accept callback ran - the assertion now counts the
accepted client only if it exists.
