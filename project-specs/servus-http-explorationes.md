# servus HTTP — explorationes (pre-spec verdicts)

*2026-07-20. Three-explorer audit of the existing halves BEFORE
interview/spec, per the vitrea calibration-first method. Board:
quaestio 01KY02N97R (arc), 01KXJ2QJ8X (flaky test, diagnosed
below). Name NOT sealed (servitium is taken; candidates
hospitium/ministerium — Fran seals). Every claim carries file:line
in the underlying explorer reports; load-bearing lines are cited
here directly.*

## 0. The headline

All four shelf libraries are **prototype-shaped**: happy paths
work and are partially test-pinned, but nobody has ever joined
them. `reactor_*` and `tcp_servus_*` have ZERO consumers outside
their own tests — **the server is greenfield and contracts may be
changed freely**. The manual blueprint already exists:
`probatio_tcp_servus.c::probatio_integratio_http` (:541) runs the
complete chain (servus → accipere → recipere → petitio_parse →
router_matching → handler → responsum_serialize → mittere)
synchronously, once. The library to build = that test as a
reactor-driven loop, plus the gap fixes below.

## 1. MUST-FIX (crash/hang class — server dies without these)

1. **SIGPIPE absent** (tcp_posix.c: no SO_NOSIGPIPE/SIG_IGN
   anywhere in lib/): write to a closed peer = process death.
   Fix: SO_NOSIGPIPE on accepted fds.
2. **Send path busy-spins**: tcp_mittere_omnia on EAGAIN does
   usleep(1000)+retry forever (:284-289) — one slow reader hangs
   the whole event loop. Fix: queued tail + REACTOR_SCRIBERE
   arming (the reactor's POLLOUT path is implemented and tested).
3. **Reactor fd table leaks slots**: reactor_removere only sets
   activus=FALSUM; fd_numerus NEVER shrinks (:409) — connection
   churn permanently exhausts the 1000-slot table. Fix: slot
   reuse on adicere (timer slots already do this).
4. **recv EAGAIN and EOF both return 0** (tcp_posix.c:319-332;
   only EOF sets clausa) — a read loop treating 0 as EOF drops
   live connections. Fix: distinct would-block signal (API
   change; no consumers to break).
5. **Parser reset drops pipelined bytes + arena grows unbounded
   per connection**: body path never advances cursor past the
   body (http.c:1850), reset wipes the buffer (:1913), there is
   NO bytes-consumed accessor, and per-request allocs come from
   the parser's piscina with no reset point. Fix: per-connection
   piscina mark/reset between requests + a consumed-bytes
   contract (or explicit "no pipelining v1" + Connection: close
   on leftover bytes).

## 2. SHOULD-FIX (correctness/protocol class)

- **Chunked request bodies are a TODO stub** (http.c:1817-1821 —
  marks COMPLETA, body silently discarded). v1 decision: reject
  411/400 (client-side _decodificare_chunked exists if we ever
  want real support).
- **No resource limits**: unbounded buffer doubling, >64 headers
  SILENTLY dropped (:1574), no URI/request caps, incomplete
  requests wait forever (no deadline). The server owns a limits
  layer + idle/header timeouts (reactor timers).
- **Request-smuggling exposures**: duplicate Content-Length
  last-wins (:1606), CL+TE coexistence unchecked. Reject 400.
- **Silent garbage acceptance**: unknown method → IGNOTUS parse
  success; missing/HTTP-2 version → silently 1.1 (:1537). Server
  must 400/501.
- **Bare-LF requests hang** (CRLF-only line matching :1398);
  header folding = hard error. Decide lenient-LF policy.
- **Router: 404 vs 405 impossible** — RoutaResultus is only
  {invenit, handler, params}; no method-mismatch signal, no
  Allow list. API addition required. **No HEAD→GET fallback.**
- **Router param lifetime trap**: param values are chorda VIEWS
  into the request buffer (router.c:210-262) — reusing the recv
  buffer before the handler runs corrupts them. Must become a
  named contract in the server's memory story.
- **Accept EAGAIN = TCP_ERROR_IO** ("Nullae connexiones",
  :682-686) — same code as real errors; accept loop needs a
  distinct drained signal.
- **TCP_NODELAY never set on accepted conns** (only clients) —
  header-then-body writes eat Nagle delay.
- **Serializer thin**: always HTTP/1.1; ignores caller
  status_descriptio (table wins); Content-Length auto ONLY for
  non-empty bodies (no `Content-Length: 0`); no Connection/Date;
  no duplicate-CL guard; NO response-builder ergonomics (tests
  hand-allocate capita arrays). A responsum-builder API is part
  of the server library.

## 3. Flaky test DIAGNOSED (01KXJ2QJ8X)

Root cause: **non-blocking listener + synchronous accept with no
retry**. probatio_multi_connexiones (:440,:485) and
probatio_integratio_http (:567,:589) accept immediately after
connect on a default-non-blocking listener; if the loopback
connection hasn't crossed SYN→accept queue yet, accept → EAGAIN →
hard assert. Smoking gun: probatio_accipere_connexio already
forces blocking with the comment "Blocking mode evitat race
condition" (:171-172) — diagnosed once, never propagated.
Secondary: 10ms-gated non-blocking recv (EAGAIN→0, §1.4) and a
LIVE httpbin.org call inside probatio_ponere_non_blocans (:274).
Empirical: 60/60 green idle-box runs — rare race, widens under
load. Cheapest fix: non_blocans=FALSUM in the two tests + drop
the httpbin call; production fix: reactor-driven accept.

## 4. Sound parts (verified, lean on them)

- Reactor: dispatch/timer/poll-timeout math sound; POLLOUT path
  implemented AND pinned; POLLHUP→CLAUSUM pinned; callback
  reentrancy (add/remove from inside a callback) is MEMORY-SAFE —
  cost is only one-turn deferral of not-yet-dispatched fds
  (revents zeroed on rebuild, level-trigger re-fires).
- tcp_servus: SO_REUSEADDR set; accepted fds correctly made
  non-blocking (explicit, macOS-correct); port-0 auto-select
  works (tests use it — no fixed ports, TIME_WAIT ruled out).
- http parser: request-line/query-split/version, incremental
  3-part feed, case-insensitive header lookup, Content-Length
  bodies — all test-pinned in probatio_http_parser.c (no
  network). Serializer CRLF discipline + status table pinned.
- router: exact+pattern matching, params, i32 coercion, caps
  (128 routes pinned), NULL hardening pinned. Query split is the
  PARSER's job (via vs quaestio) — contract already honored in
  the integration test.
- tabellarius: orthogonal (JSON-RPC over FILE* framing) —
  composable as body content, no dependency either way.
- servitium (registry) is NOT an integration obligation —
  standalone library is the clean shape.
- MIME table precedent: vitrea_macos.m MIMINA (:75-130),
  char*-based — extract to a shared home, add chorda variant.

## 5. Fixture endpoint contract (retires 01KXZTX6D3, plaintext)

From probatio_http.c live asserts: GET /get → 200 + JSON >20B +
Content-Type; POST /post → 200 (accepts JSON body); GET /headers
→ 200 + Content-Type echo; GET /status/404 → 404. Plus the
DNS-failure path (already offline-green). CAVEAT: those asserts
currently use https:// URLs — localization means http:// fixture
URLs, not a host swap. **TLS stays -reticularis**: tls.h/tls_macos
are CLIENT-ONLY (kSSLClientSide hardcoded, no accept/cert path) —
a local fixture cannot exercise the TLS client; all four TLS
network asserts stay named live-internet exclusions.

## 6. Queued for the interview (design decisions)

- Name (servitium taken; hospitium / ministerium / other).
- Threading posture: single-thread reactor only? (house lean:
  yes, actor.c for workers — vitrea precedent).
- Pipelining: support via consumed-bytes API, or v1 = one
  request per read-burst + Connection: close on leftovers?
- Chunked requests: 411-reject v1?
- Limits defaults (max request size, header count, timeouts).
- Lenient-LF: accept bare LF or hard-400?
- Router 405/HEAD: extend router.h API (no consumers — free) or
  wrap in server?
- Static serving: directory-based (dev mode) + capsula-based?
  MIME extraction home (mimen.h?).
- Fixture server: standalone tool or probationes helper?
- Relationship to vitrea dev-mode + ACAO knob (01KXZZW15P).
