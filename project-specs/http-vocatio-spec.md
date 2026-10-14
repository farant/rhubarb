# http vocatio - a non-blocking HTTP(S) client on the reactor (spec)

*Fran 2026-10-14, after route A of streaming `$question` shipped
(quaestio-rivus, ff 806da4bd): "start thinking about how we could have
the streaming anthropic client in-process instead of having to fork out
to a different binary". Question that prompted it: what does an
in-process client need that the helper process got for free? This is
route B of `project-specs/quaestio-rivus-spec.md` §V (ledger wish
…7EHS).*

Goal: **a general-purpose, non-blocking HTTP(S) client library** driven
by `reactor.h`, with **vicus `$question` as its first (and in this arc
only) user**. `$question` behaves exactly as today (steady pieces, page
locked, Esc cancels, page-full stop, one undo step), but with no child
process, and the window never stalls - not during DNS, connect, the TLS
handshake, or reading.

Decisions (Fran, 2026-10-14):
- **The API key is read in the vicus process** (ledger decision
  …AAQVQ3): once per request, buffer zeroed after the request is built,
  send buffer zeroed after it is sent, never written to a message,
  status, annals or insula. The old rule "only `bin/quaestio` reads the
  key" is relaxed for route B. Claude still never reads or prints the
  key; Fran runs live checks.
- **General library, narrow first use** ("(b) to implement (a)"): the
  client is reusable (several concurrent calls, timers, a reactor shared
  with anything else), but only `$question` moves onto it now.
- **The host owns the reactor.** The client is handed a `Reactor*` and
  registers its fds and timers there; vicus creates one reactor and
  polls it each frame. One loop per thread; libraries attach, they do
  not own loops. (hospitium predates this and owns its reactor; giving
  it a "use this reactor" constructor is not in this arc.)
- **Handle + callbacks**: a call is an `HttpVocatio*`; body bytes go to
  the existing `HttpRivus` sink as they arrive, one `finis` callback at
  the end, and `http_vocatio_status(v)` for callers that poll (the
  vicus port does).
- **DNS via `DNSServiceGetAddrInfo` behind a small platform API**
  (ledger decision …CYT9A8), not getaddrinfo, not our own resolver.
- **Stay on SecureTransport** (deprecated, but its I/O callbacks already
  speak `errSSLWouldBlock`). Network.framework is rejected: it calls
  back on GCD queues (threads). Our own TLS is a far-horizon item.

## I. What exists (measured 2026-10-14, quarta 309e2fad)

- **reactor** (`include/reactor.h`, `lib/reactor.c`, 716 lines):
  `poll()` loop, `REACTOR_FD_MAXIMA` M fds, `REACTOR_TIMER_MAXIMA`
  CXXVIII timers, `reactor_poll(r, timeout_ms)` one pass
  (`lib/reactor.c:554`). Used only by hospitium (`lib/hospitium.c:88`,
  `hospitium_gressus` = one `reactor_poll`, `lib/hospitium.c:1134`).
  Test: `probationes/probatio_reactor.c` (102 CREDO assertions; re-entrancy
  covers a callback removing ITSELF and adding a different fd,
  `probationes/probatio_reactor.c:80-96`).
- **Two reactor dispatch hazards found reading the code (not yet
  pinned by a test):**
  - fd dispatch walks `fd_entries` and the rebuilt `poll_fds` side by
    side, advancing `poll_idx` only for entries that are active NOW
    (`lib/reactor.c:599-617`). A callback that removes ANOTHER active fd
    or adds one into an earlier-free slot desynchronizes the two arrays:
    later revents go to the wrong callback (or are dropped).
  - one-shot timers: after the callback returns, the slot is set
    inactive (`lib/reactor.c:291`). A callback that cancels its own
    timer and adds a new one can get the SAME slot back, which is then
    switched off - the new timer silently never fires. Re-arming an
    idle timeout from inside a timer callback is exactly this.
- **TCP** (`lib/tcp_posix.c`): `tcp_connectere_cum_optionibus`
  (`:151`) resolves with blocking `getaddrinfo` (`:207`) and connects
  with blocking `connect` (`:228`); timeouts are `SO_RCVTIMEO` /
  `SO_SNDTIMEO` (`:100-101`). `tcp_ponere_non_blocans` exists (`:867`).
  `tcp_recipere` maps EAGAIN to `TCP_ITERUM` (-2, `include/tcp.h:116`),
  `tcp_mittere` maps it to 0. `tcp_servus_*` (`:545-844`) gives a
  listening socket with an fd (`tcp_servus_obtinere_fd`, `:806`).
- **TLS** (`lib/tls_macos.m`, 808 lines): SecureTransport with I/O
  callbacks over our TCP (`SSLSetIOFuncs`, `:360`). The read callback
  turns `TCP_ITERUM` into `errSSLWouldBlock` AND sets
  `tempus_excessum` (`:58-63`) - on today's blocking sockets EAGAIN
  means `SO_RCVTIMEO` fired. The handshake is a loop until done or
  timed out (`:422-437`); `tls_recipere` loops the same way (`:586-589`).
  On a non-blocking socket "would block" must NOT mean "timed out".
- **HTTP**: incremental, transport-free response parser `http_lector`
  (`include/http.h:106-135`, quaestio-rivus T1) feeding `HttpRivus`
  (`include/http.h:90`); the blocking streamed exchange
  `http_exsequi_rivus` (`include/http.h:287`); request timeout
  `http_petitio_tempus_ponere` (`include/http.h:219`). Error codes
  `HTTP_ERROR_{URL,CONNEXIO,TIMEOUT,PARSE,IO,REDIRECTIO}`
  (`include/http.h:44-49`).
- **SSE + Anthropic**: `rivus_eventuum` (T3) and the vates collector
  (T4) are push parsers; `vates_mittere_rivum` (`include/vates.h:313`)
  is the blocking streamed call with retry only before the first
  delta; error codes `VATES_ERROR_{RETE,TEMPUS,STATUS,PARSE,LIMES}`
  (`include/vates.h:183-187`).
- **vicus port**: `VicusPortaQuaestionis {incipere, pulsare,
  abrumpere, ctx}` (`include/vicus_quaestio.h:32-41`); route A is
  `vicus_quaestio_porta` (`include/vicus_quaestio_porta.h`), wired in
  `apps/vicus/vicus.c:165-172`.
- **Frame pacing**: a live app with a pulse callback waits at most
  `MORA_PULSUS` (16 ms) per frame (`lib/ludus_fenestra.c:233`), so one
  `reactor_poll(r, 0)` per pulse adds <= 16 ms latency.
- **No DNS code in the tree** (no `dns_sd` include anywhere); no UDP.
- **credo II signa** (`include/credo.h:656-680`): a section tagged
  `signa "rete"` is skipped with `CREDO_SIGNA_OMITTE="rete"`.

## II. Layers

Each layer is non-blocking and uses only the one below. Names are
proposals; the plan fixes them.

### II.0 reactor: dispatch safe under mutation

Fix both hazards in §I so that callbacks may freely add/remove fds and
add/cancel timers (including their own) during dispatch:
- fd dispatch iterates a snapshot of (fd, entry) pairs taken when
  `poll_fds` was built; before calling, it re-checks that the entry is
  still active AND still the same registration (a generation counter
  per slot), so removed fds are skipped and fds added mid-pass wait for
  the next poll.
- a one-shot timer is marked inactive BEFORE its callback runs; a
  repeating timer's next deadline is set before the callback. The
  callback may then cancel/re-add freely.

Tests first (red on today's code): callback removes another live fd;
callback adds an fd into a lower free slot; timer callback cancels
itself and adds a new timer - the new one fires.

### II.1 `nomina` - non-blocking name resolution (new)

`include/nomina.h`, `lib/nomina_macos.m` (platform file, like
`tls_macos.m`):

```c
NominaQuaestio* nomina_quaerere_incipere(Reactor* r, chorda hospes,
    NominaFinis finis, vacuum* datum, Piscina* p);
vacuum nomina_quaerere_abrumpere(NominaQuaestio* q);
/* finis(datum, addressus[], numerus, error) - exactly once, unless
 * abrumpere came first (then never) */
```

`DNSServiceGetAddrInfo` (IPv4 + IPv6), its `DNSServiceRefSockFD`
registered for read, `DNSServiceProcessResult` when readable; results
collected until `kDNSServiceFlagsMoreComing` is clear, then `finis`
once. Literal IP addresses skip the lookup. A lookup that answers
nothing within the caller's deadline is the caller's timeout (II.4).

### II.2 TCP: non-blocking connect

`tcp_incipere(addressus, piscina)`: non-blocking socket +
`connect` -> `EINPROGRESS`; the caller waits for writability, then
`tcp_incipere_perficere(conn)` reads `SO_ERROR` (0 = connected, else a
named error). No `SO_RCVTIMEO` on these sockets: timeouts belong to the
caller's reactor timers.

### II.3 TLS: stepped handshake and non-blocking I/O

`tls_incipere(tcp, hospes, piscina)` sets up the context on a connected
non-blocking socket; `tls_gradus(conn)` takes one `SSLHandshake` step
and returns `TLS_GRADUS_{PERFECTUS, LEGERE, SCRIBERE, ERROR}` (what to
wait for next). `tls_recipere` / `tls_mittere` on such a connection
return a distinct "would block" value instead of looping. The stepped
mode never sets `tempus_excessum`; the blocking path (`tls_connectere`)
keeps today's behavior unchanged. Certificate trust as today: system
store, host name checked.

### II.4 `http_vocatio` - the call state machine (new)

`include/http_vocatio.h`, `lib/http_vocatio.c`:

```c
HttpVocatio* http_vocare(Reactor* r, HttpPetitio* petitio,
    HttpRivus rivus, vacuum* rivi_datum,
    HttpVocatioFinis finis, vacuum* finis_datum, Piscina* p);
HttpVocatioStatus http_vocatio_status(HttpVocatio* v);
HttpResultus* http_vocatio_resultus(HttpVocatio* v); /* after finis */
vacuum http_vocationem_abrumpere(HttpVocatio* v);
```

States move forward only; every path ends in exactly one `finis`:

```
NOMINA -> CONNECTIO -> PACTIO (https only) -> MITTENS -> LEGENS -> FINITA
                                                       \-> IRRITA | ABRUPTA
```

- NOMINA: II.1; addresses tried in order.
- CONNECTIO: II.2 on the first address; on failure the next one. No
  happy-eyeballs racing (non-goal).
- PACTIO: II.3, waiting on read or write as `tls_gradus` asks.
- MITTENS: request bytes written as the socket accepts them (partial
  writes handled).
- LEGENS: whatever is readable goes to `http_lector`; body bytes reach
  `HttpRivus` as they arrive, only for 2xx (same rule as T2). Done when
  `http_lector` reports finished, or at connection close when the body
  is delimited by close.

Timeouts (reactor timers, re-armed on progress):
- connect phase (NOMINA + CONNECTIO + PACTIO): `tempus_ms` of the
  request if set, else 30 s;
- idle while MITTENS/LEGENS: 60 s without a byte (Anthropic sends `ping`
  events during long streams);
- no total-duration limit (long answers are legitimate).
A timeout ends IRRITA with `HTTP_ERROR_TIMEOUT`, body so far kept.

`http_vocationem_abrumpere` is idempotent and synchronous: cancels the
lookup, closes TLS and the socket, removes every fd and timer of the
call, and calls `finis` with ABRUPTA before returning. After `finis`
nothing of the call is registered; a stale event already in the
dispatch pass is ignored (II.0 makes that safe).

Errors reuse the existing codes: `HTTP_ERROR_CONNEXIO` (with a named
message: unknown name, refused, TLS description), `HTTP_ERROR_TIMEOUT`,
`HTTP_ERROR_PARSE` (`http_lector` rejects), `HTTP_ERROR_IO` (read/write
failure mid-stream). Each call has its own piscina; the caller frees it
when done with the result. Plain `http://` is supported (no PACTIO) -
tests and local services use it.

### II.5 vates: `vates_vocare_rivum`

```c
VatesVocatio* vates_vocare_rivum(Vates* v, VatesPetitio* petitio,
    chorda propositum, Reactor* r, VatesRivus rivus, vacuum* datum,
    VatesVocatioFinis finis, vacuum* finis_datum, Piscina* p);
VatesResponsum* vates_vocatio_responsum(VatesVocatio* vv);
vacuum vates_vocationem_abrumpere(VatesVocatio* vv);
```

Same contract as `vates_mittere_rivum`: same request body
(`"stream":true`), same collector, the final response equal to the
blocking path's for the same events; truncated stream =
`VATES_ERROR_RETE`, `error` event = `VATES_ERROR_STATUS`. Retry only
before the first delta, backoff on a reactor timer (never a sleep), a
retry is a fresh `HttpVocatio`. The request builder and the event
handling are factored out of `vates_mittere_rivum` and shared, so the
two paths cannot drift; a test feeds one fixture through both and
asserts equal responses. The base URL is an option (tests point it at
a local server).

### II.6 vicus port: `vicus_quaestio_interna`

`lib/vicus_quaestio_interna.c` implements `VicusPortaQuaestionis`:
- `incipere`: reads `~/.rhubarb/anthropic.clavis`, builds the request,
  zeroes the key buffer; after the request bytes are sent the send
  buffer (which held the header) is zeroed; starts
  `vates_vocare_rivum` on the vicus reactor; text is planned as it
  arrives (route A ruling R9).
- `pulsare`: `reactor_poll(r, 0)`, then report new text / error /
  usage / finished exactly as route A's port does.
- `abrumpere`: `vates_vocationem_abrumpere`.

Wiring: vicus creates one reactor at startup and polls it every pulse
(nothing registered = a no-op); route B becomes the default `$question`
port; `-quaestio adiutor` keeps route A selectable; `bin/quaestio`
stays as a CLI tool.

## III. Tests and plants

No test touches the network or the key, except one opt-in check.
- **Scripted local server** (`probationes/servus_scriptus.h`, test
  helper): a `tcp_servus` on 127.0.0.1 registered on the SAME reactor
  as the client under test (one process, one loop, deterministic). It
  plays a script: chunks with delays (reactor timers), a mid-stream
  close, silence (idle timeout), garbage. hospitium cannot stream or
  delay responses yet, hence a helper.
- **reactor**: the three II.0 cases, red first.
- **nomina**: `localhost` resolves (offline); a literal IP skips the
  lookup; a cancelled lookup never calls `finis` and leaves nothing
  registered.
- **TCP**: connect to a listening port; a refused port gives a named
  error; nothing left registered.
- **TLS steps**: handshake over a socketpair whose peer sends garbage
  fails cleanly, within a bound, without blocking; stepped mode never
  reports a timeout on would-block.
- **http_vocatio**: every scripted response delivered at every chunk
  split (as T1 does); short-limit connect and idle timeouts; abrumpere
  in every state; after `finis` the reactor has no fds or timers of
  the call; one `finis` per call, always.
- **vates**: SSE fixtures through the scripted server; equal responses
  vs `vates_mittere_rivum` on the same fixture; retry before the first
  delta, none after.
- **port**: route A's scenarios replayed (bonum, mors = mid-stream
  close, pravum, error, lente + abrumpere, page full).
- **Opt-in network check** (section `signa "rete"`, omitted unless
  enabled): HTTPS POST to `api.anthropic.com/v1/messages` with NO key;
  the expected 401 `authentication_error` proves DNS + TLS + HTTP end
  to end with no secret.
- **Plants** per task via `silva.planta`, as in quaestio-rivus.
- **Fran's live check** of `$question` in vicus closes the arc.

Task order (the plan refines it): reactor II.0 -> nomina -> TCP connect
-> stepped TLS -> http_vocatio + scripted server -> vates_vocare_rivum
-> port + wiring.

## IV. Not in this spec

- Connection reuse (keep-alive), HTTP/2, proxies, redirects (the
  streamed path already refuses redirects).
- Happy-eyeballs (racing IPv4/IPv6 connects).
- Waking the window on fd readiness instead of per-pulse polling
  (integrating reactor fds into `fenestra_expectare_eventus`).
- hospitium on a host-owned reactor; other users of the client
  (optchat, norma, herbarium capture).
- Our own TLS; Network.framework.
- Removing route A or `bin/quaestio`.

## AUDIENDA (not verified)

- The two reactor hazards are read from code, not yet shown by a
  failing test (II.0's tests do that first).
- `DNSServiceGetAddrInfo` resolving `localhost` with no network: assumed
  (mDNSResponder answers from /etc/hosts); the nomina test confirms it.
- Whether silva/examen can parse `<dns_sd.h>`: not checked - hence a
  `.m` platform file like `tls_macos.m`. New POSIX names for TCP
  (`SO_ERROR`, `EINPROGRESS`, `getsockopt`) may need entries in the
  examen lexicon (`silva/fontes/systema_posix.h`), as in analytica T2.
- SecureTransport's behavior when `SSLHandshake` returns
  `errSSLWouldBlock` on a non-blocking socket mid-record: the read
  callback already handles partial reads, but stepped use is untested.
- That `ping` events arrive often enough for a 60 s idle limit:
  Anthropic documents pings during streams; interval not measured.
- The cost of `reactor_poll(r, 0)` per frame with nothing registered:
  it returns early only while `timer_numerus` is 0
  (`lib/reactor.c:586`), and `timer_numerus` is a high-water mark that
  never shrinks (`:517-519`) - after the first call, every idle frame
  does a zero-timeout `poll()` with no fds. Assumed negligible (one
  syscall per <= 16 ms); not measured. II.0 may make the early return
  count ACTIVE timers instead.
