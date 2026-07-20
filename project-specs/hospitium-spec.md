# hospitium — spec v1 (house HTTP server)

*2026-07-20. From the interview (hospitium-interrogatio.md) over
the three-explorer audit (servus-http-explorationes.md — the
evidence companion; every "today" claim below is cited there with
file:line). Name SEALED: **hospitium** (the guest-house — the
server receives guests and hosts them). Board: quaestio
01KY02N97R. Spec v2 (line-level surgery plan) follows the
explorer refinement pass.*

## 1. Thesis

An HTTP/1.1 server library joining four existing shelf halves —
tcp_servus (sockets), http.c (parse/serialize), router (match),
reactor (poll loop) — into the missing middle: a reactor-driven,
single-threaded, non-blocking connection lifecycle. **Infra
first**: the day-one consumers are the local test fixture that
replaces httpbin.org (01KXZTX6D3) and the vitrea dev-mode static
server with ACAO (01KXZZW15P). App-backend hardening (limits,
timeouts, smuggling guards) is built in v1, but the exit bars are
infra's.

Greenfield advantage (audit verdict): reactor and tcp_servus have
ZERO consumers outside their own tests — their contracts change
freely. The manual blueprint exists: probatio_integratio_http
runs the whole chain synchronously once; hospitium is that test
as a loop, plus the gap fixes.

Pure C89 POSIX — no .m file, no platform split (tcp_posix is the
platform layer).

## 2. Shape

- `include/hospitium.h` + `lib/hospitium.c` — the library.
- `include/mimen.h` + `lib/mimen.c` — MIME table extracted from
  vitrea_macos.m (char* + chorda variants); vitrea switches to it.
- Surgeries on the shelf (each its own phase, see §8):
  tcp_posix.c (SIGPIPE, EAGAIN contracts, NODELAY), reactor.c
  (fd-slot reuse), http.c (guards, lenient-LF, limits, reliquiae,
  serializer), router.{h,c} (generic handler-data, 405 mask).
- Worklogs per house rule; fructus counters (§10).

## 3. Sealed decisions (interview, 2026-07-20)

1. **Single-threaded by construction** (vitrea precedent) —
   handlers fast; slow work = app's actor problem. gressus/currere
   split so a host loop can pump hospitium (a vitrea app
   self-hosting its dev assets is the named future consumer).
2. **Keep-alive, no pipelining** — per-connection piscina reset
   between requests; leftover bytes after a complete request ⇒
   finish the response, then honest close.
3. **Strictness**: chunked request ⇒ 411; duplicate CL ⇒ 400;
   CL+TE ⇒ 400; unknown method ⇒ 501; garbage/missing version ⇒
   400; bare-LF ACCEPTED (lenient: match \n, strip optional \r —
   simpler than CRLF-only and kills the hang).
4. **Limits configurable with defaults**: max petitio ~1MB, URI
   ~8KB, idle timeout 30s, header-read timeout 10s, header cap 64
   (exists), connection cap ~128.
5. **Handler ABI = context struct**: HospitiumTractator(
   Colloquium*) carrying petitio/params/per-request piscina/
   userdata; router demoted to PURE MATCHER storing vacuum* per
   route.
6. **Router extension owns matching truth**: via_inventa +
   methodi-permissae mask ⇒ hospitium renders 405 + Allow;
   HEAD = GET-without-body automatically.
7. **Respondere one-shot + caput_addere**; serializer hardened
   (Content-Length ALWAYS incl. 0, Connection per keep-alive
   state, dup-CL guard). No Date header in v1.
8. **Static = directory only** (capsula-serving stays vitrea's):
   ..-traversal refusal, index.html default document, generated
   listing page when index absent, shared mimen table.
9. **Dev conveniences all four**: access log to stderr (flag),
   ACAO knob (`Access-Control-Allow-Origin: *`), index default,
   directory listing.
10. **Bind default 127.0.0.1**; portus 0 = auto (fixture);
    hospitium_portus() reads the bound port.
11. **Fixture = fork() in test setup**, port 0, port reported
    through a pipe, kill in teardown. (In-process same-thread
    deadlocks: http_exsequi blocks on recv.)

## 4. Connection lifecycle (the core design)

Per-connection state: `ConnexioHospitii` { TcpConnexio*, fd,
HttpParser*, Piscina* piscina_connexionis, PiscinaNotatio basis,
write-tail {chorda, offset}, deadlines {otium, capita}, status
{LEGENS, SCRIBENS, CLAUDENDA}, keep_alive }.

- **Accept**: listener POLLIN → accipere loop until would-block
  (distinct drained signal — §8 Phase 0). Per accepted fd:
  SO_NOSIGPIPE + TCP_NODELAY + non-blocking (exists), register
  REACTOR_LEGERE, allocate connection piscina, mark basis, create
  parser inside it. Connection cap: at capacity, accept + respond
  503 + close (visible, not a silent SYN backlog).
- **Read**: on LEGERE, recv into a stack buffer → parser_adicere.
  COMPLETA → guards (400/411/501) → dispatch. ERROR → 400 +
  close. Would-block → return to loop (distinct from EOF — §8).
- **Dispatch**: router match on petitio->via (query already split
  by parser — contract honored in the integration test). Miss
  with via_inventa → 405 + Allow. Miss → 404 (or static-route
  fallthrough → directory serving). HEAD → run GET handler,
  serialize headers, drop body. Handler gets Colloquium; its
  piscina IS the connection piscina (allocations die at the
  request-boundary reset). **Param/petitio lifetime = the
  request** — named contract (audit: params are views into the
  parser buffer).
- **Write**: serialize into connection piscina; send immediately;
  on partial/EAGAIN stash the tail and arm REACTOR_SCRIBERE
  (implemented + tested in reactor); on drained: keep-alive →
  piscina_reficere(basis) + fresh parser (kills the audit's
  unbounded-arena leak structurally — parser, buffer, request,
  response all die together); reliquiae (leftover bytes after the
  request) or Connection: close or guard-reject → close. NOTE:
  reset is DEFERRED until the write tail drains — the response
  bytes live in the piscina being reset.
- **Timeouts**: ONE repeating reactor sweep timer (1s) walks
  connections checking deadlines (header-read, idle). NOT
  per-connection timers (128-timer cap; linear sweep at 128
  connections is nothing). Expiry → close (408 optional — v1
  just closes).
- **Close**: reactor_removere + tcp_claudere + piscina_destruere.

## 5. API sketch (C89; ≤31-char names)

```c
nomen structura Hospitium Hospitium;
nomen structura HospitiumColloquium HospitiumColloquium;

nomen vacuum (*HospitiumTractator)(HospitiumColloquium* colloquium);

nomen structura {
    constans character* hospes;      /* NIHIL = "127.0.0.1" */
    i32 portus;                      /* 0 = auto-selectus */
    i32 connexiones_maximae;         /* 0 = CXXVIII */
    i32 petitio_maxima;              /* 0 = 1MB */
    i32 uri_maxima;                  /* 0 = 8KB */
    s32 otium_maximum_ms;            /* 0 = 30000 */
    s32 capita_maxima_ms;            /* 0 = 10000 */
    b32 acao;                        /* ACAO: * in omni responso */
    b32 acta_accessus;               /* linea per petitionem, stderr */
} HospitiumConfiguratio;

Hospitium* hospitium_creare    (Piscina*, constans HospitiumConfiguratio*);
vacuum     hospitium_destruere (Hospitium*);
b32  hospitium_praebere        (Hospitium*, HttpMethodus,
                                constans character* via,
                                HospitiumTractator, vacuum* datum);
b32  hospitium_directorium_servire(Hospitium*,
                                constans character* praefixum,
                                constans character* directorium);
b32  hospitium_gressus         (Hospitium*, s32 ms_maximae); /* gyrus unus */
vacuum hospitium_currere       (Hospitium*);  /* gressus in ansa */
vacuum hospitium_sistere       (Hospitium*);
i32  hospitium_portus          (constans Hospitium*);
HospitiumFructus hospitium_fructus(constans Hospitium*);

/* intra tractatorem (omnia in vitam petitionis valida) */
constans HttpPetitioServeri* colloquium_petitio(HospitiumColloquium*);
chorda   colloquium_param      (HospitiumColloquium*, constans character*);
Piscina* colloquium_piscina    (HospitiumColloquium*);
vacuum*  colloquium_datum      (HospitiumColloquium*);
vacuum   colloquium_caput_addere(HospitiumColloquium*,
                                constans character*, constans character*);
vacuum   colloquium_respondere (HospitiumColloquium*, i32 status,
                                constans character* mimen, chorda corpus);

/* ===== mimen.h ===== */
constans character* mimen_pro_via        (constans character* via);
constans character* mimen_pro_via_chorda (chorda via);
```

## 6. Shelf surgeries (evidence: explorationes §1-2)

**tcp_posix.c**: SO_NOSIGPIPE on accepted fds (must-fix #1);
distinct would-block signal for recv (EAGAIN vs EOF both 0 today
— API change, no consumers) and for servus_accipere (EAGAIN ≠
TCP_ERROR_IO); TCP_NODELAY on accepted fds. tcp_mittere_omnia's
usleep busy-spin is NOT used by hospitium (write path is
queue+SCRIBERE); leave it but document.

**reactor.c**: fd-slot reuse on adicere (fd_numerus never shrinks
today — churn exhausts the 1000 table). Timer drift + timer
high-water: leave (sweep timer is one repeating timer).

**http.c**: guards at parse-complete (dup-CL, CL+TE, 411-chunked,
501-method, 400-version); lenient-LF line matching; limits
(petitio/uri caps checked during growth); `http_parser_reliquiae`
accessor (bytes buffered past the completed request — drives the
honest-close rule); serializer: CL always, Connection header,
dup-CL guard, keep status-table behavior.

**router.{h,c}**: handler storage → `vacuum*` (matcher purity);
RoutaResultus += via_inventa + methodi_permissae mask; document
param-view lifetime.

## 7. Fixture + localization (retires 01KXZTX6D3)

probationes fixture (fork harness): `GET /get` → 200 JSON >20B;
`POST /post` → 200 (echoes JSON); `GET /headers` → 200 +
Content-Type; `GET /status/:codex` → that status (generalizes
/status/404). probatio_http network asserts point at
`http://127.0.0.1:<portus>` (https:// today — host swap is NOT
enough). **TLS stays -reticularis** (tls is client-only,
kSSLClientSide hardcoded — a local fixture cannot serve TLS);
probatio_tls + any https:// asserts = named live-internet
exclusions, run on demand.

## 8. Phasing

- **Phase 0 — shelf hardening (pre-arc, independent).** The flake
  fix (01KXJ2QJ8X: non_blocans=FALSUM in the two racy tests +
  drop the live httpbin call) + tcp/reactor surgeries §6, each
  with probationes (incl. the audit's named test debt:
  callback-reentrant add/remove, fd churn past reuse, POLLERR).
  Exit: suite green repeatedly, no flake, no live-internet call
  in probatio_tcp_servus.
- **Phase A — parser + serializer hardening.** §6 http.c items +
  probationes for every guard/limit/LF case (the audit's untested
  list: reset, >64 headers, CL:0, split-CRLF feeds). Exit: parser
  suite green, all guards pinned.
- **Phase B — router surgery + mimen extraction.** §6 router
  items + mimen.{h,c} + vitrea switched to mimen (behavior
  identical, demo re-smoked). Exit: router suite green incl. 405
  mask + probatio_vitrea smoke unchanged.
- **Phase C — hospitium core.** Lifecycle §4 + API §5 + demo
  (tools/ or probatio GUI-less: routes + directory serving;
  manual curl inch: keep-alive two-requests-one-connection,
  405+Allow, 411, slow-reader no-hang, idle ~0%). Exit: demo
  green + fructus counters plausible.
- **Phase D — fixture + localization + vitrea posture.** §7 +
  ACAO knob verified against a capsula-origin page (calibration
  verdict re-proven live). Exit: probatio_http green OFFLINE;
  tls exclusions named in the script; 01KXZTX6D3 + 01KXZZW15P
  closed.

## 9. Testing

House pattern: probatio_hospitium.c (credo) — lifecycle unit
tests use loopback + fork clients where needed (blocking client
side, port 0 always). Parser/router/reactor/tcp additions extend
their existing suites. The canonical integration
(probatio_integratio_http) gets rewritten onto hospitium and
becomes the reactor-driven version of itself.

## 10. Metrics — HospitiumFructus

connexiones_acceptae, connexiones_plenae_recusatae (503),
petitiones_tractatae, petitiones_reiectae (guards 400/411/501),
responsa_missa, clausae_otio (timeouts), clausae_reliquiis
(pipelining honest-close). Perf decisions read these.

## 11. Named deferrals

- pipelining (consumed-bytes API) — the reliquiae accessor is
  its seed; chunked request decode (411 until then)
- TLS server side (client-only today; would need cert story)
- async handlers / actor suspension (single-thread v1 posture)
- Date header; capsula-over-HTTP serving; streaming/SSE;
  compression (flatura exists if ever wanted); IPv6 listener
  (AF_INET today); Range requests
- hospitium-sonda equivalent (fructus + access log suffice)
