# hospitium — spec v2 (SELF-CONTAINED — build from this)

*2026-07-20. Supersedes hospitium-spec.md (v1). v2 = v1 + three
line-level surgery plans (chirurgi: tcp/reactor, http.c, router/
mimen — anchors verified against the working tree at edeb8ba) +
Fran's three rulings (O1 carve-out / O2 additive / O3 hard-501).
Companions: servus-http-explorationes.md (audit evidence),
hospitium-interrogatio.md (raw interview). Name SEALED:
**hospitium** (decretum 01KY04PXRZ). Board: quaestio 01KY02N97R.*

*Base-type law that drives several designs below: i32 = UNSIGNED
int (s32 signed); every subtraction on i32 must be proven
non-underflowing; function pointers may NOT be stored in vacuum*
under -pedantic (ISO C) — hence the CAPSA pattern (§C-B1).*

## 1. Thesis, decisions, architecture — unchanged from v1

§1-§5 of v1 stand: infra-first HTTP/1.1 server; single-thread
reactor; keep-alive without pipelining (reliquiae ⇒ honest
close); guards 411/501/400×2 + dup-CL/CL+TE; lenient-LF; limits
configurable w/ defaults (petitio 1MB, uri 8KB, otium 30s, capita
10s, connexiones 128); handler = HospitiumTractator(Colloquium*);
router = pure matcher; respondere one-shot; directory static +
mimen; all four dev conveniences; bind 127.0.0.1; fixture =
fork-in-test port 0; gressus+currere; API sketch v1 §5. The
lifecycle (v1 §4) gains the v2 precisions marked below.

**Interview rulings (2026-07-20, v2 round):**
- **O1**: Content-Length ALWAYS incl. 0, EXCEPT body-forbidden
  statuses (1xx/204/304) where HTTP forbids it — carve-out via
  `_http_corpus_prohibitum`; the pinned 204 test survives.
- **O2**: serializer change is ADDITIVE —
  `http_responsum_serialize_cum_conexione(responsum, keep_alive,
  piscina)`; the 2-arg form stays byte-identical (6 call sites
  untouched).
- **O3**: unknown method = hard 501 at parse (uniform guard);
  extension methods are a named deferral.
- O4 (chorda_ad_i32 unsigned-wrap on giant CL) → filed as
  desideratum, not Phase-A-blocking (the 413 fail-fast + total
  cap backstop it).

═══════════════════════════════════════════════════════
## PHASE 0 — shelf hardening (tcp_posix.c, reactor.c, flake)
═══════════════════════════════════════════════════════

Caller census (exhaustive): tcp_recipere ← tls_macos.m:55,
http.c:750, tests. tcp_servus_accipere ← tests only. Only ONE
switch on TcpError exists (tcp_error_descriptio, tcp_posix:375).
http.c:750 recv loop branches ONLY on n>0 ⇒ NO http.c edit.

**0.1 SIGPIPE.** New helper after _applicare_optiones (~:85):
```c
interior vacuum
_ponere_nosigpipe(integer fd)
{
    integer flag = 1;
    setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, &flag,
               (socklen_t)magnitudo(flag));
}
```
Unconditional (NOT inside _applicare_optiones — that early-returns
on !opt). Apply: accepted fds (tcp_servus_accipere, before the
non-blocking-inherit block ~:691) AND client connect fds (after
_applicare_optiones call at :203) — symmetry decided YES.

**0.2 TCP_NODELAY on accepted fds.** Reuse _applicare_optiones
with a local TcpOptiones {0, VERUM, FALSUM} right after
_ponere_nosigpipe in accipere.

**0.3 recv contract.** include/tcp.h ~:108:
`#define TCP_ITERUM (-2)` — recv returns: >0 data, 0 EOF (clausa),
TCP_ITERUM would-block, -1 error. Edit tcp_recipere :319-324:
EAGAIN branch `redde TCP_ITERUM;` (was 0). EOF path unchanged.
**tcp_mittere stays asymmetric on purpose**: 0 = would-block (no
EOF collision on send) — hospitium arms SCRIBERE on 0; document
in header.
RIPPLE (the only one): tls_macos.m:55-67 read callback — order
matters:
```c
n = tcp_recipere(conn->tcp, (i8*)data, (i32)petitum);
si (n == TCP_ITERUM) { *dataLength = 0; redde errSSLWouldBlock; }
si (n < 0)           { *dataLength = 0; redde errSSLClosedAbort; }
si (n == 0)          { *dataLength = 0; redde errSSLClosedGraceful; }
```
(Fixes a latent bug: EOF was mis-mapped to would-block. CAVEAT:
validated only by the live -reticularis TLS run — manual re-smoke
required.)

**0.4 accept would-block.** APPEND enum value (never mid-enum):
`TCP_ERROR_ITERUM` after TCP_ERROR_IO (tcp.h:37). accipere
:682-686 EAGAIN → _creare_error(TCP_ERROR_ITERUM, ...). Add casus
to tcp_error_descriptio. TCP_ITERUM (s32 return sentinel) and
TCP_ERROR_ITERUM (TcpError) are deliberately distinct channels.

**0.5 reactor fd-slot reuse.** reactor_adicere :364-381: scan for
!activus slot FIRST (mirror the timer-slot pattern :456-470);
extend fd_numerus only when none free; fold the capacity check
into the extension branch. AUDIT VERDICT: _rebuilere_poll_fds and
the dispatch loop need ZERO changes (both compact by activus in
index order — alignment recomputed every poll; reentrancy
one-turn-deferral guarantee unchanged).

**0.6 Flake fix (01KXJ2QJ8X).**
- probatio_multi_connexiones :440 and probatio_integratio_http
  :567: `tcp_servus_creare(0,...)` → creare_cum_optionibus with
  `opt.non_blocans = FALSUM` (propagates the existing "Blocking
  mode evitat race condition" fix at :171-172).
- probatio_ponere_non_blocans :266-293: DELETE the live
  httpbin.org call; rewrite as loopback (blocking servus port 0,
  connect 127.0.0.1, toggle non_blocans VERUM/FALSUM on the
  client fd, both TCP_OK). No accept needed.
- probatio_non_blocans :253: add
  `CREDO_VERUM(accept_res.error == TCP_ERROR_ITERUM)`.
Exit: zero live hosts in probatio_tcp_servus.c; suite ≥20× green.

**0.7 New probationes.** tcp_servus: probatio_nosigpipe (close
peer, write twice; reaching the final assert IS the survival
proof; last mittere == -1), probatio_nodelay (getsockopt
TCP_NODELAY != 0 on accepted), probatio_recipere_iterum (pins all
three recv classes: ITERUM empty / >0 data / 0 EOF). reactor:
probatio_fd_churn_reuse (1200 adicere/removere cycles, 2 fds in
flight; fails at #1001 without 0.5), probatio_callback_reentrans
(remove-self + add-spare inside callback via g_reactor_ref; no
crash, swap reflected), probatio_poll_error (write-end of pipe
with read-end closed → REACTOR_ERROR; OR-tolerant like the
POLLHUP test).

═══════════════════════════════════════════════════════
## PHASE A — http.c parser + serializer hardening
═══════════════════════════════════════════════════════

**A.1 Types.** HttpParseResultus (http.h:115-121) gains
`i32 status_suggestus;` (HTTP status the server should send; 0 =
none). DESIGN: field, NOT an HttpParseStatus extension — phases
stay a state machine, guard class is orthogonal metadata; every
phase consumer stays untouched. HttpParser struct (+5):
status_suggestus, vidit_content_length, vidit_transfer_encoding
(b32), petitio_maxima, uri_maxima (i32, config, survive reset).
Defaults: `HTTP_PETITIO_MAXIMA_DEFALTA (M*M)`,
`HTTP_URI_MAXIMA_DEFALTA (VIII*M)`.

**A.2 Constructor.** New public
`http_parser_creare_cum_limitibus(piscina, petitio_maxima,
uri_maxima)` (0 = default — matches HospitiumConfiguratio
convention); http_parser_creare becomes the (0,0) wrapper.

**A.3 Guards** (each sets parser->status_suggestus, returns
FALSUM; the two state-machine FALSUM branches (:1783-1791 request
line, :1830-1838 header) mirror it into
`res.status_suggestus = parser->status_suggestus ? ... : CD;`):
- 501 unknown method — after :1448 methodus assignment.
- 400 version — REPLACE the silent default :1537-1540 (alioquin
  → status_suggestus = CD; redde FALSUM).
- 400 dup-CL + 400 CL+TE — rewrite the special-header arms
  :1603-1626 with the vidit_* flags (both arrival orders
  covered); PLUS 413 fail-fast when announced CL >
  petitio_maxima (catches before buffering).
- 411 chunked — REPLACE the TODO stub :1817-1821 (was: silently
  COMPLETA with body discarded).
Existing lenient tests :542/:566 are print-only — stay green;
strict asserts come in A.8.

**A.4 Lenient-LF.** _parser_invenire_crlf → _parser_invenire_lf
(find '\n'; drop the <II early-out — lone '\n' at 0 is a valid
empty line). Per-iteration derivation in http_parser_adicere:
```c
nl_pos = _parser_invenire_lf(parser, parser->cursor);
/* NON_INVENIT -> wait */
linea_finis = nl_pos;
si (linea_finis > parser->linea_initium
    && parser->buffer[linea_finis - I] == '\r')
{
    linea_finis--;
}
```
Eight sites: :1775/:1799 find; :1783/:1830 pass linea_finis to
the helpers; :1793/:1810/:1840 cursor = nl_pos + I (was + II);
:1808 empty-line test against linea_finis. The two parse helpers
need ZERO changes — their param is already an EXCLUSIVE content
end. Split-CRLF-across-feeds safe (wait for '\n', strip '\r'
retroactively in the accumulated buffer).

**A.5 Limits.** Total cap in adicere before growth (:1753/1756),
underflow-proof form (invariant buffer_mensura <= petitio_maxima):
`si (longitudo > parser->petitio_maxima - parser->buffer_mensura)`
→ 413. URI cap in _parser_parse_linea_petitionis after the
empty-URI check :1464-1467: `uri_finis - uri_initium >
uri_maxima` → 414 (no underflow: uri_finis >= uri_initium
guaranteed by the line above).

**A.6 Reliquiae.** FIRST make end-of-request honest: body path
:1853-1858 adds `parser->cursor += content_length;` before
COMPLETA (safe: branch requires buffer_mensura - cursor >=
content_length). Then:
```c
i32 http_parser_reliquiae(HttpParser* parser)
{   /* 0 nisi COMPLETA; cursor <= buffer_mensura invariant */
    redde parser->buffer_mensura - parser->cursor;  }
```
v1 semantics: reliquiae > 0 ⇒ hospitium finishes the response
then closes (pipelining = deferral; bytes are NOT carried across
reset).

**A.7 Serializer.** Refactor :983-1043 into internal
`_serialize_impl(responsum, emittere_connexionem, keep_alive,
piscina)`; `http_responsum_serialize` = (FALSUM, FALSUM) wrapper
— byte-identical for all 6 existing callers;
`http_responsum_serialize_cum_conexione` = (VERUM, keep_alive)
[O2]. Inside: (a) CL emitted UNLESS `_http_corpus_prohibitum
(status)` = 1xx/204/304 [O1] — empty 200 now gets
`Content-Length: 0` (the real keep-alive framing fix); (b)
caller-supplied Content-Length/Connection headers SKIPPED in the
user-header loop (serializer owns framing); (c) Connection:
keep-alive|close emitted before the blank line when
emittere_connexionem. Status table :961-970 adds 411 "Length
Required", 413 "Payload Too Large", 414 "URI Too Long".

**A.8 Init/reset parity (latent-bug checklist).** res default
block :1732-1738 + both manual res constructions in
http_petitio_parse (:1946, :1958) gain status_suggestus = 0.
http_parser_reset clears status_suggestus + both vidit_* flags
(NOT the two config fields). NOTE: hospitium NEVER calls reset
(§C memory story — re-create); reset stays for the client path,
now flag-hygienic.

**A.9 New probatio_http_parser.c tests.** Guards: dup-CL, CL+TE
both orders, chunked→CDXI, FROB→DI, HTTP/9.9 + missing version→CD
(each asserts status_suggestus exactly). LF: bare-LF full
request; mixed; SPLIT feed ("...\r" then "\n..." across adicere
calls). Limits: URI>8KB→CDXIV; tiny-cap creare_cum_limitibus
(64,32) →CDXIII; announced-CL-over-cap→CDXIII fail-fast.
Reliquiae: trailing bytes after no-body and after body; at-exact-
COMPLETA → 0. Serializer: empty-200 has "Content-Length: 0"; 204
still CL-free (test :494 kept); caller CL/Connection skipped;
_cum_conexione both polarities. Reset hygiene: two requests
through one parser, different CLs, no false 400.

═══════════════════════════════════════════════════════
## PHASE B — router surgery + mimen extraction
═══════════════════════════════════════════════════════

**B.1 THE CAPSA PATTERN (ISO-C constraint).** Function pointers
must NOT be stored in vacuum* (-pedantic violation). The router
stores OBJECT pointers only; hospitium allocates a box per route
in its piscina:
`nomen structura { HospitiumTractator tractator; vacuum* datum; } CapsaTractatoris;`
and registers `&capsa`. Dispatch casts back. Tests preview the
same pattern.

**B.2 router.h.** DELETE the RoutaHandler typedef (:54-58). KEEP
#include "http.h" (HttpMethodus still used). Six registration
fns: `RoutaHandler handler` → `vacuum* datum` (:100/:107/:114/
:121/:128/:144). New result + mask macro (replaces :69-74):
```c
#define ROUTA_METHODUS_BIT(m)  ((i32)I << (i32)(m))
nomen structura {
    b32          invenit;           /* via + methodus congruunt */
    vacuum*      datum;             /* opacum; NIHIL nisi invenit */
    RoutaParams  params;
    b32          via_inventa;       /* via congruit, methodo neglecta */
    i32          methodi_permissae; /* OR bitium omnium methodorum viae */
} RoutaResultus;
```
(Casts keep -Wsign-conversion quiet; HttpMethodus max = 7 < 32.)

**B.3 router.c.** Routa.handler (:26) → vacuum* datum; adicere
(:316) param/guard/assign; five wrappers (:360-403).
router_matching REWRITTEN: no method-skip, no early-return —
both passes accumulate `via_inventa` + mask on every
path-match; winner assigned under `!res.invenit` guard (exact
pass runs first ⇒ exact-over-pattern preserved;
first-registered-wins preserved). Pattern candidates match into
a SCRATCH `RoutaParams candidatae` (memset'd — silences
-Wconditional-uninitialized), copied to res.params only on win —
a wrong-method pattern route can no longer clobber a winner's
params. _match_pattern signature unchanged.

**B.4 HEAD + Allow are hospitium's** (router stays
method-literal): HEAD request ⇒ match as HTTP_GET, run handler,
serialize headers, drop body; Allow rendered from
methodi_permissae (+HEAD when GET present, +OPTIONS). Phase C.

**B.5 Consumers.** probatio_router.c: box rewrite (CapsaTractatoris
per handler + _vocare(res) helper — keeps all g_handler_vocatus
assertions) + FIVE new mask asserts: wrong-method mask;
multi-method same path (mask complete even after a win);
exact+pattern both contribute (three sub-asserts incl. params
isolation); total miss (via_inventa FALSUM, mask 0); HEAD
literality (HEAD vs GET-only route ⇒ invenit FALSUM, mask GET).
probatio_tcp_servus.c :533-539/:622: minimal box (test is
rewritten onto hospitium in Phase C anyway).

**B.6 mimen.** include/mimen.h + lib/mimen.c: mimen_pro_via
(char*, verbatim port of _mimen_pro_via) +
mimen_pro_via_chorda (length-bounded: last-'.'-after-last-'/'
scan over datum/mensura, memcmp with strlen(extensio) equality
check; trailing-dot → ext_len 0 → octet-stream, no i32
underflow). Table = the 14 rows verbatim. vitrea_macos.m: add
include; DELETE :75-130; :303 → mimen_pro_via(via_l). Byte-
identical behavior ⇒ probatio_vitrea smoke must stay green.

**B.7 Build wiring.** compile_tests.sh SOURCE_FILES += lib/mimen.c
AND lib/hospitium.c (STUB in Phase B — one comment line — so the
wiring is proven early; decided). That ONE edit propagates
everywhere: get_object_files links all objects into every
binary; compile_tools.sh globs build/*.o; captare.sh extracts
SOURCE_FILES from compile_tests.sh; the narrow scripts
(lector/sputnik/benchmark) don't list router/vitrea — no edits.
compile_library.sh menu array: optional discoverability add.
probatio_mimen.c: glob-discovered, no script change. Asserts:
css hit; unknown ext, no-ext, trailing-dot, dot-before-slash
("a.b/c") → octet-stream; char*/chorda PARITY on the same inputs
with non-NUL-terminated chordae.

**B.8 Commit order.** (1) mimen files + probatio + SOURCE_FILES
(+hospitium stub); (2) vitrea swap + re-smoke; (3) router.h/.c
surgery; (4) both consumer tests + 5 mask asserts; (5) full
suite green.

═══════════════════════════════════════════════════════
## PHASE C — hospitium core (v1 §4-§5 + v2 precisions)
═══════════════════════════════════════════════════════

- **Registration**: hospitium_praebere allocates CapsaTractatoris
  {tractator, datum} in the hospitium piscina, registers &capsa
  with the router (B.1).
- **Clocks**: tempus.h has ONLY `f64 tempus_nunc(vacuum)` —
  monotonic SECONDS (mach ticks; auto-init). Deadlines are kept
  as f64 seconds compared against tempus_nunc() directly; no ms
  integer clock exists or is needed. Access-log ms = delta *
  1000.0 at print time.
- **Traversal defense** (directory serving):
  url_decodificare(request via) FIRST (%2e%2e cannot smuggle) →
  strip route praefixum → via_iungere(root, rel) →
  via_normalizare (resolves ../. lexically — EXISTS, via.h) →
  REFUSE unless root is a byte-prefix of the normalized result.
  Lexical only; symlink escape = named deferral.
- **Listing page**: iter_directoria simple iterator (aperire/
  proximum/claudere). LIFETIME TRAP: each DirectoriumIntroitus +
  its titulus is valid only until the next proximum — emit each
  entry into the response builder INSIDE the loop (or copy
  titulus to the connection piscina first).
- **Parser per connection**:
  http_parser_creare_cum_limitibus(piscina_connexionis,
  cfg.petitio_maxima, cfg.uri_maxima) created AFTER
  basis = piscina_notare; at request boundary (response fully
  drained — reset DEFERRED past the write tail, which lives in
  the same piscina): piscina_reficere(basis) + fresh
  creare_cum_limitibus. NEVER http_parser_reset (would dangle
  after reficere; never shrinks a ballooned buffer). CONFIRMED:
  every parser allocation is from its one piscina — nothing
  dangles, nothing leaks (audit exhaustive).
- **Rejection rule**: parse FALSUM ⇒ respond
  `res.status_suggestus ? res.status_suggestus : CD` via
  serialize_cum_conexione(keep_alive=FALSUM), then close.
  reliquiae > 0 after COMPLETA ⇒ serve, respond, close.
- **Serialize**: always http_responsum_serialize_cum_conexione.
- **Fixture harness** (Phase D consumer, prep here): fork() is
  GREENFIELD in probationes (no precedent); the port-report pipe
  IS precedented (9 pipe() sites in probatio_reactor.c). Child:
  hospitium port 0 + currere; parent: read port from pipe,
  http_exsequi against 127.0.0.1:port, kill+waitpid teardown.

## PHASE D — unchanged from v1 §7/§8

Fixture endpoints (GET /get JSON>20B, POST /post echo, GET
/headers, GET /status/:codex), probatio_http → http:// fixture
URLs, TLS -reticularis named exclusions, ACAO re-proof against a
capsula-origin page, close 01KXZTX6D3 + 01KXZZW15P.

## Exit bars (per phase — unchanged v1 §8 plus)

Phase 0 adds: tls_macos.m ripple flagged for manual -reticularis
re-smoke. Phase B adds: probatio_vitrea smoke green post-mimen.

## Deferrals (v1 §11 plus v2 additions)

- symlink-escape in directory serving (lexical refusal only)
- extension methods (hard-501 posture, O3)
- chorda_ad_i32 bounded parse (O4 desideratum)
- carrying reliquiae across reset (true pipelining)
