# canalis plan - an admin channel to the running server

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> implementer subagents (a review subagent at the end is allowed). Steps
> use checkbox (`- [ ]`) syntax for tracking.

**Goal:** the CLI stops being a second writer - `exemplar -invitare`
asks the running server over a local channel (same uid only), and the
same channel, bridged over ssh, makes exemplar's admin tools an MCP
server for Claude Code.

**Architecture:** `canalis` is the platform layer (AF_UNIX listen /
accept / connect, 0700 directory, 0600 socket, peer uid) handing back
ordinary `TcpConnexio*`; `mcp_servus_canalis` runs an existing
`McpServus` over it on the reactor (newline framing, write queues,
deferred replies) and provides two blocking clients, `pons` (stdio
bridge) and `vocare` (one tool call). exemplar adds a second tool table
(scope `administratio`) served at `<data>/canalis/servus`; four small
library additions give the admin view across authors.

**Tech stack:** C89 + latina.h; tcp (`tcp_ex_fossa`, `tcp_recipere`,
`tcp_mittere`), reactor (`reactor_adicere/modificare/removere`,
`REACTOR_LEGERE | REACTOR_SCRIBERE`), mcp_servus (`mcp_servus_tractare`,
`McpPersona`, scopes, `mcp_differre`), json, filum (directories, delete,
`filum_seram_capere`), cives / invitationes / ianua / admissio (quarta),
credo v2; Linux via `tools/linux/` (OrbStack); deploy via
`tools/dimittere.sh` to smaragda.

**Spec:** `project-specs/canalis-spec.md` (quarta 59030f92). Read it with
this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh`; identifiers,
  comments, messages Latin; never a latina.h macro as an identifier
  (`nomen`, `structura`, `magnitudo`, `per`, any Roman numeral
  0-3999...); numerals above 3999 are NOT macros (`IV * MXXIV`); `i32`/
  `i64` UNSIGNED, `s32`/`s64` signed; chorda not NUL-terminated.
- POSIX .c files include `postulata_posix.h` FIRST; no raw
  `fopen/open/stat/access` outside filum (examen code 96) - directory
  creation, mode and delete go through `filum_*`.
- **Same uid only:** a peer is admitted iff `canalis_par_uid` reads it
  AND it equals `geteuid()`; unreadable -> refused (fail closed).
- **Nothing the channel carries is logged:** no request or reply body,
  no invitation code, in the server, `pons` or `vocare`. One stderr
  line per refused peer (`canalis: par recusatus`), nothing else.
- **No password ever crosses the channel** (no tool takes or returns
  one).
- The admin table is NEVER mounted on `/mcp`; every admin tool carries
  scope `administratio`; the channel's fixed persona is
  `{ usor "administratio", cliens "canalis", scopi "administratio" }`.
- Every library write stays inside the caller's transaction
  (ianua P3); admin tools open and commit/roll back their own.
- Each library: `include/X.h` with `/* <aedilis corpus="lib/X.c"/> */`
  (or `lib/X_posix.c`), `lib/X.c`, `lib/X.worklog.md`,
  `probationes/probatio_X.c` (credo v2: `CredoSectio` table +
  `credo_suitam_currere`); after adding a suite run
  `./tools/compile_tests_fontes_generare.sh` and commit
  `compile_tests_fontes_generata.sh` WITH the task.
- New words through `VOCABULA_VIAE_ADDITAE="a.c b.h" ./oratio/vocabula.sh -nova`
  before commit; rename rather than add a real English word.
- Commits through `silva.commissio` (docs first, gates last); Linux
  (musl + glibc) green before a task is called done.

## Rulings made while planning

- **P1 - Linux peer credentials:** `struct ucred` needs `_GNU_SOURCE` on
  glibc and musl; `lib/canalis_posix.c` defines it under `__linux__`
  right after `postulata_posix.h` and before any system header. macOS
  `getpeereid` is in `<unistd.h>` under the default feature set. T1
  step 1 compiles a probe on all three targets first. Resolves spec
  AUDIENDA 1. Cost if wrong: one header shuffle in one file.
- **P2 - write interest = `reactor_modificare(fd, LEGERE | SCRIBERE)`**
  while a connection's queue is non-empty, back to `LEGERE` when it
  drains - hospitium's pattern (lib/hospitium.c:388, :416). Resolves
  AUDIENDA 2.
- **P3 - `sudo -n -u exemplar true` works over ssh on smaragda**
  (checked 2026-10-20, no tty). Resolves AUDIENDA 3.
- **P4 - test socket paths** are `/tmp/canalis-<pid>-<n>/servus`
  (well under 104 bytes), removed at the end of each section.
  Resolves AUDIENDA 4.
- **P5 - exemplar's admin tools are tested in-process.** The exemplar
  suite drives its server by pumping `hospitium_gressus`, so the
  blocking `vocare` cannot run in it; the suite gets a non-blocking
  channel client helper (`_administrare(e, instrumentum, argumenta)`:
  `canalis_connectere`, write the three lines, pump until the reply
  line). The running-server `-invitare` test forks a child that calls
  `exemplar_invitare` (blocking) while the parent pumps.
- **P6 - the scope test moves to exemplar.** That a persona without the
  scope cannot run a scoped tool is mcp_servus's existing guarantee
  (its own suite); the real risk is exemplar mounting the admin table
  on `/mcp`. Pinned there: `/mcp` `tools/list` lacks `invitare`, and
  `tools/call invitare` on `/mcp` is refused.
- **P7 - `invitare` replies with JSON** (`mcp_respondere_json`):
  `{id, codex, expirat_ms, nexus}`, `nexus` = server basis +
  `/inscribere?invitatio=` + code. The CLI prints exactly today's
  lines (`codex: X`, and `nexus: <CLI -basis>/inscribere?invitatio=X`
  only when the CLI got `-basis`).
- **P8 - `vocare`'s text** = `result.content[0].text` of the matching
  reply; `result.isError` true -> 1; a JSON-RPC `error` -> 3 with its
  message in `textus`.
- **P9 - `exemplar_invitare` keeps its signature and codes** (0 / 1 /
  2): channel first; `vocare` 2 -> the lock path, unchanged; 2 now
  means "lock held and the channel did not answer".
- **P10 - suspension is one transaction:** `cives_statum_ponere`
  (writes `civis.status`), `ianua_sessiones_civis_delere` (sessions are
  not evented, ianua P12), then each live grant of
  `admissio_concessiones(civis)` through `admissio_revocare` (its own
  `concessio.revocata` events). Any failure -> rollback, isError.
- **P11 - tool input rules:** `dies` 1..XXX (default VII), checked in
  the handler (McpArgumentum has no range); `nomen` normalized with
  `cives_nomen_normare`; suspending or restoring a DELETUS person ->
  isError "civis deletus" (a tombstone never changes state);
  suspending an already suspended person / restoring an active one ->
  success, no event (idempotent).
- **P12 - `status` reply:** `{salus "ok", acta N, cives {activi,
  suspensi, deleti}, invitationes_vivae N}`; live = not revoked, not
  expired, `usus_numerus < usus_max`.
- **P13 - author shown in `invitationes_enumerare`:** civis id ->
  current handle via `cives_per_id`; "" -> `administratio`; unknown id
  -> the id itself.

## Rulings made while executing

- **T1: FilumStatus grew `modus` + `est_socket`.** `filum_status`
  reported neither permission bits nor a socket type, and a raw `stat`
  in canalis would violate examen code 96. Two fields added at the end
  (no caller initializes the struct), pinned by a new filum section.
  Cost if wrong: the filum.h gate fan-out on this commit, nothing else.
- **T1: silva's system-header model extended** (sockaddr_un, AF_UNIX,
  ECONNABORTED, getpeereid, geteuid, socketpair, S_ISSOCK) and certified
  by `officina/auspex_posix.sh` (plant red, restored green) - the tool
  gap is closed in the tool, not by an exemption.
- **T1: presence checks use `filum_status`, not `filum_existit`**
  (fopen-based; a socket never "exists" by it - two first-draft
  assertions were vacuous).
- **T1: Linux filum failures are environmental.** 7 assertions of
  `filum_seram_capere` fail in the container (tree mounted read-only,
  lock in build/); older than this task; the new filum section and
  canalis/tcp are green on musl and glibc.

- **T2: MSG_NOSIGNAL in `tcp_mittere` on every platform.** Darwin
  ignores SO_NOSIGPIPE on a socket accepted after its peer closed
  (signal 13 killed the test server in `fuga`). Fix in lib/tcp_posix.c,
  pinned by a second case in probatio_canalis `nosigpipe` (red before).
  Cost if wrong: none - Linux already sent with it.
- **T2: half-close semantics** (pons shuts its write side and waits):
  EOF stops reading only; close when nothing is owed. Not in the plan's
  step 3 text; required by the `pons` section.
- **T2: extra section `locus_iteratus`** - the plan's generation-check
  plant cannot go red on `fuga` (the slot is not reused while the reply
  is pending); the new section closes a connection with a reply pending
  (over-long line) and reuses its slot.
- **T2: servus-wide `initiatum`** accepted (mcp_servus tracks initialize
  per servus, not per connection) - documented in the header, no
  transport-side method sniffing.
- **T2: both pons sections wait with a deadline** - a broken pons must
  fail the suite, not hang it (the first pons_finis plant hung 19 min).

- **T3: P10 corrected - sessions ARE evented.** `_sessionem_delere`
  writes `sessio.deleta {sigillum}` (SHA-256 hex). `ianua_sessiones_
  civis_delere` deletes each session through it (same events as the
  password change, which now shares the helper `_sessiones_civis_delere`).
  T4's suspension therefore leaves one `sessio.deleta` per session.
- **T3: same-ms tiebreaks use `rowid`**, not the ULID id (cives creation
  order; all-invitations newest-first) - the fixed test clock makes ties
  the normal case.

- **T4: admissio joins the caller's transaction.** `admissio_revocare`
  opened its own sqlite transaction, so P10's one-transaction suspension
  failed. New `volumen_in_transactione`; admissio's revoke path opens its
  own transaction only when the caller holds none. Cost if wrong: none
  for /applicationes (same path); suspension stays atomic.
- **T4: grant revocation is load-bearing** (plant evidence): without it
  the bearer token still answers 200 after suspension - /mcp does not
  check the civis status.
- **T4: `exemplar_administrare` in the nucleus** (public header) rather
  than mcp_servus_canalis in exemplar.c; `-invitare` exit 2 message now
  says "lock held but the channel did not answer".
- **T4: `sera` section updated** - "-invitare while running -> 2" is
  obsolete; the section removes the socket to produce lock-held-no-answer.
- **T4: /mcp plant run by hand** (needs an include + a swap; silva.planta
  takes one edit).

## Review focus

Inputs the spec implies but no test of the spec's list exercises; each
is pinned by a test in the task named.

1. **Server restarts while Claude Code's bridge is connected** -> `pons`
   sees EOF and exits within a second instead of hanging (Claude Code
   then shows the server down and respawns the bridge). Test in Task 2
   (`pons_finis`).
2. **A client writes a last line without a newline, then closes** ->
   the partial line is dropped, no crash, the next client is served.
   Test in Task 2 (`linea_imperfecta`).
3. **Two CLI invitations at the same moment while running** -> both
   succeed with distinct codes (the server serializes). Test in Task 4
   (`invitare_simul`).
4. **Suspending a deleted person, or an unknown handle** -> isError
   naming the reason; nothing changes. Test in Task 4
   (`suspensio_limites`).
5. **A data directory whose socket path exceeds the limit** -> exemplar
   refuses to start with the message naming the limit (never starts
   without its channel, never truncates), lock released. Test in Task 4
   (`canalis_longus`).

---

### Task 1: `canalis` - the local channel (+ `tcp_ex_fossa` SIGPIPE fix)

**Files:** Create `include/canalis.h`, `lib/canalis_posix.c`,
`lib/canalis.worklog.md`, `probationes/probatio_canalis.c`; modify
`lib/tcp_posix.c` (`tcp_ex_fossa`: call `_ponere_nosigpipe`);
regenerate `compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** tcp (`tcp_ex_fossa`, `TcpResultus`,
`TcpError`, `tcp_recipere`, `tcp_mittere`, `tcp_claudere`), filum
(`filum_directorium_existit`, `filum_directorium_creare_cum_modo`,
`filum_status`, `filum_modum_ponere`, `filum_delere`). **Produces:**
```c
/* canalis.h - canalis ad servum currentem, identitate systematis
 * operandi probatus (canalis-spec II.1). Implementatio:
 * canalis_posix.c (AF_UNIX). Windows (nondum): AF_UNIX per Winsock
 * (sine credentialibus paris -> ACL directorii) aut named pipes (ACL +
 * GetNamedPipeClientProcessId; IOCP, non TcpConnexio) - cum portatione
 * decernendum. */
/* <aedilis corpus="lib/canalis_posix.c"/> */

nomen structura CanalisAuditor CanalisAuditor;

nomen structura {
               b32  successus;
    CanalisAuditor* auditor;
            chorda  error_descriptio;
} CanalisResultus;

/* PRAECONDITIO: vocans seram scriptoris unius tenet (socket vetus tunc
 * nullius servi vivi est). Directorium parens absens -> 0700;
 * praesens cum bit gregis/aliorum -> recusatur. In via: socket ->
 * deletur; aliud -> recusatur (intactum). Via longior sun_path ->
 * recusatur (limes nominatur). Socket 0600, non-blocans. */
CanalisResultus canalis_audire (constans character* via, Piscina* p);
/* non-blocans: nihil -> successus FALSUM, error TCP_ERROR_ITERUM */
TcpResultus canalis_accipere (CanalisAuditor* a, Piscina* p);
integer canalis_fossa (CanalisAuditor* a);
/* claudit et viam delet; NIHIL tutum */
vacuum canalis_claudere (CanalisAuditor* a);
/* obstruens; quaelibet defectio -> successus FALSUM + descriptio */
TcpResultus canalis_connectere (constans character* via, Piscina* p);
/* getpeereid (macOS) / SO_PEERCRED (Linux); FALSUM = legi nequit */
b32 canalis_par_uid (TcpConnexio* c, i32* uid);
/* par_uid lectum ET == geteuid(); aliter FALSUM */
b32 canalis_par_admissus (TcpConnexio* c);
```

- [ ] **Step 1: Probe (P1):** a scratch C file in the scratchpad that
  includes `postulata_posix.h`, the `__linux__` `_GNU_SOURCE` define,
  `<sys/socket.h>`, `<sys/un.h>`, `<unistd.h>` and references
  `getpeereid` (macOS) / `struct ucred` + `SO_PEERCRED` (Linux);
  compile with house flags on macOS and through `tools/linux/` musl +
  glibc. Expected: all three compile clean. If not, rule in the ledger
  before step 2.
- [ ] **Step 2: Failing test** `probatio_canalis.c` sections (paths per
  P4; a helper `_via(p, n)` builds `/tmp/canalis-<pid>-<n>/servus`):
  - `iter`: `audire`; `connectere` from the same process; `accipere`
    (retry until not ITERUM, bounded); bytes "salve\n" each way through
    `tcp_mittere` / `tcp_recipere`.
  - `par`: on the accepted connection `canalis_par_uid` -> VERUM and
    `uid == geteuid()`; `canalis_par_admissus` VERUM.
  - `modi`: after `audire`, the directory's mode & 0777 == 0700 and the
    socket's == 0600 (`filum_status`).
  - `directorium_apertum`: create the parent 0750 first -> `audire`
    FALSUM, description contains "apertum"; mode still 0750.
  - `vetus`: `audire`, then abandon it (close the fd without
    `claudere`, socket file left) -> second `audire` on the same path
    VERUM and a connect works.
  - `alienum`: a regular file at the path -> `audire` FALSUM; the file
    still exists with its content.
  - `longus`: a 120-byte path -> FALSUM, description names the limit
    (contains "sun_path").
  - `claudere`: after `canalis_claudere` the path no longer exists;
    `connectere` -> FALSUM.
  - `nosigpipe`: fork; child: `socketpair` -> wrap one end with
    `tcp_ex_fossa`, close the other, `tcp_mittere` twice, `_exit(0)` if
    it returned < 0 both times; parent: `waitpid`, expect normal exit 0
    (a SIGPIPE death shows as `WIFSIGNALED`).
- [ ] **Step 3: Run** `./compile_tests.sh canalis` - expect FAIL (no
  `canalis.h`).
- [ ] **Step 4: Implement** `lib/canalis_posix.c`: `struct
  CanalisAuditor { integer fd; character via[CANALIS_VIA_MAXIMA]; }`;
  `audire`: length check against `magnitudo(((structura
  sockaddr_un*)0)->sun_path)` (NUL counted), parent directory via
  filum (exists -> mode check; absent -> create 0700), existing path:
  `lstat`-equivalent through `filum_status` (socket -> `filum_delere`,
  other -> refuse), `socket(AF_UNIX, SOCK_STREAM)`, `bind`,
  `filum_modum_ponere(via, 0600)`, `listen(fd, VIII)`, `O_NONBLOCK`;
  `accipere`: `accept`, EAGAIN -> ITERUM, else `tcp_ex_fossa`;
  `connectere`: blocking `connect` then `tcp_ex_fossa`; `par_uid`:
  `getpeereid` / `getsockopt(SO_PEERCRED)`. In `lib/tcp_posix.c`
  `tcp_ex_fossa`: `_ponere_nosigpipe(fossa)` before wrapping.
  (If `filum_status` cannot tell a socket from a file, add the
  smallest field to filum and rule it in the ledger.)
- [ ] **Step 5: Run** - expect PASS; each section alone with
  `CREDO_SECTIO=<s>` also passes.
- [ ] **Step 6: Plants** (`silva.planta`): `par_uid` returns VERUM
  without reading (uid 0) -> `par` red; chmod 0600 dropped -> `modi`
  red; non-socket deleted instead of refused -> `alienum` red;
  `_ponere_nosigpipe` call removed from `tcp_ex_fossa` -> `nosigpipe`
  red (macOS).
- [ ] **Step 7: Linux:** `./tools/linux/probatio.sh -musl canalis tcp`
  and `-glibc canalis tcp` - green (SO_PEERCRED path).
- [ ] **Step 8: Commit** `canalis: canalis localis ad servum currentem (AF_UNIX, 0700/0600, uid paris), tcp_ex_fossa SO_NOSIGPIPE (canalis T1)`.

---

### Task 2: `mcp_servus_canalis` - MCP over the channel

**Files:** Create `include/mcp_servus_canalis.h`,
`lib/mcp_servus_canalis.c`, `lib/mcp_servus_canalis.worklog.md`,
`probationes/probatio_mcp_servus_canalis.c`; regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** Task 1 (`canalis_audire`, `_accipere`,
`_fossa`, `_claudere`, `_connectere`, `_par_admissus`); mcp_servus
(`mcp_servus_tractare`, `McpReddere`, `McpReditus`, `McpPersona`,
`mcp_differre`, `mcp_dilatum_vocatio`, `mcp_respondere_textum`), reactor,
json (`json_legere`, `json_capere_*`, `json_scribere`). **Produces:**
```c
/* <aedilis corpus="lib/mcp_servus_canalis.c"/> */
#define MCP_CANALIS_CONNEXIONES  VIII
#define MCP_CANALIS_LINEA_MAXIMA (MXXIV * MXXIV)   /* 1 MiB */

nomen structura McpCanalis McpCanalis;

/* servus applicationis super canalem; persona fixa { "administratio",
 * "canalis", "administratio" }; NIHIL + *error si canalis_audire
 * deficit. Nulla corpora in actis. */
McpCanalis* mcp_canalis_praebere (McpServus* servus, Reactor* reactor,
    constans character* via, chorda* error, Piscina* p);
/* connexiones claudit, fossam e reactore removet, viam delet */
vacuum mcp_canalis_claudere (McpCanalis* mc);
/* pons stdio <-> canalis (obstruens, poll super fossas crudas):
 * 0 EOF ordinatus, I scriptura fracta, II nemo audit */
s32 mcp_canalis_pons (constans character* via, integer intra,
    integer extra);
/* vocatio una (initialize, notifications/initialized, tools/call),
 * XXX s summum: 0 bene, I isError, II nemo audit, III protocollum aut
 * tempus; textus = content[0].text aut causa (P8) */
s32 mcp_canalis_vocare (constans character* via,
    constans character* instrumentum, JsonValor* argumenta,
    chorda* textus, Piscina* p);
```

- [ ] **Step 1: Failing test** `probatio_mcp_servus_canalis.c`. Helper
  `_servus_filius(via)`: fork; child builds a reactor, an `McpServus`
  with a test table (`echo {textus}` -> textus; `tarde {ms}` -> defers,
  a reactor timer answers "tarde" after ms; `frange` -> isError
  "fractum"; all scope `administratio`), `mcp_canalis_praebere`,
  `reactor_currere`; parent waits until `canalis_connectere` succeeds
  (bounded). Teardown: `kill(SIGTERM)` + `waitpid`. Sections:
  - `vocare`: `vocare(echo, {textus:"salve"})` -> 0, textus "salve".
  - `dilatum`: `vocare(tarde, {ms:50})` -> 0, "tarde".
  - `vitium`: `vocare(frange)` -> 1, textus "fractum".
  - `nemo`: `vocare` on a path with no server -> 2; `pons` -> 2.
  - `linea_longa`: raw connect, send `initialize`, then 1 MiB + 1 bytes
    without newline -> the connection reaches EOF (bounded wait); a
    following `vocare(echo)` -> 0.
  - `linea_imperfecta` (Review focus 2): raw connect, send initialize
    line, then `{"jsonrpc":"2.0"` with no newline, close -> a following
    `vocare(echo)` -> 0.
  - `nonus`: eight raw connections held open; a ninth reaches EOF within
    1 s; close one of the eight -> `vocare(echo)` -> 0.
  - `fuga`: raw connect, initialize, send `tools/call tarde {ms:200}`,
    close at once -> after 300 ms `vocare(echo)` -> 0 (child alive:
    `waitpid(WNOHANG)` == 0).
  - `pons`: `pipe()` x2; fork a child running `pons(via, in_r, out_w)`;
    parent writes initialize + initialized + `tools/call echo` lines,
    closes `in_w`, reads `out_r` to EOF -> two reply lines (initialize
    result, call result containing "salve"); child exit 0.
  - `pons_finis` (Review focus 1): `pons` child connected (initialize
    sent, `in_w` kept open), then kill the server child -> the `pons`
    child exits within 1 s (`waitpid` with a deadline), code 0 or 1.
- [ ] **Step 2: Run** `./compile_tests.sh mcp_servus_canalis` - expect
  FAIL (no header).
- [ ] **Step 3: Implement:** per connection `{TcpConnexio* c; i32
  generatio; Xar/alveus lectionis; alveus scriptionis + offset; b32
  initium;}` in a fixed array of VIII; accept callback: `accipere` loop,
  `par_admissus` else close + stderr line, slot full -> close; read
  callback: `tcp_recipere` into the buffer, split on `\n`, each line ->
  `mcp_servus_tractare(servus, linea, &persona, VERUM, _reddere, ctx,
  p)` with `ctx = {mc, locus, generatio}`; buffer over the limit ->
  close; EOF -> close (pending partial line dropped). `_reddere`: slot
  generation mismatch -> drop; else append `corpus` + `\n`, try
  `tcp_mittere`, remainder queued with `reactor_modificare(LEGERE |
  SCRIBERE)` (P2); write callback drains, back to `LEGERE`.
  MCP_REDITUS_NUNTIATIO writes nothing. Per-request piscina freed after
  the reply. `pons`: `canalis_connectere`, `poll` on {intra, socket},
  copy with `read`/`write` loops; intra EOF -> `shutdown(SHUT_WR)`,
  continue until socket EOF. `vocare`: connect, write the three lines
  (ids 1 and 2), read with `poll` deadline 30 s, parse each line, the
  one with id 2 decides (P8).
- [ ] **Step 4: Run** - expect PASS; each section alone also passes.
- [ ] **Step 5: Plants:** generation check removed (reply into a reused
  slot) -> `fuga` red; line limit not enforced -> `linea_longa` red;
  ninth connection accepted -> `nonus` red; `pons` ignores socket EOF
  while intra is open -> `pons_finis` red. (Skipping the peer check
  cannot go red without a second uid - covered by T1's `par` and the
  one-line comparison, as the spec states.)
- [ ] **Step 6: Linux:** `./tools/linux/probatio.sh -musl
  mcp_servus_canalis canalis` and `-glibc ...` - green.
- [ ] **Step 7: Commit** `mcp_servus_canalis: MCP super canalem (reactor, lineae, responsa dilata), pons stdio et vocatio una (canalis T2)`.

---

### Task 3: library additions for the admin view

**Files:** Modify `include/cives.h`, `lib/cives.c`,
`probationes/probatio_cives.c`; `include/invitationes.h`,
`lib/invitationes.c`, `probationes/probatio_invitationes.c`;
`include/ianua.h`, `lib/ianua.c`, `probationes/probatio_ianua.c`;
worklogs of the three.

**Interfaces - Consumes:** existing tables and event writers of each
library. **Produces:**
```c
/* cives.h: omnes (etiam suspensi, deleti), ordine creationis */
Xar* cives_enumerare (Cives* c, Piscina* p);              /* de Civis */
/* invitationes.h: omnium auctorum, novissimae primum */
Xar* invitationes_omnes_enumerare (Invitationes* inv, Piscina* p);
/* INTRA TRANSACTIONEM: quamlibet (administratio); FALSUM si ignota aut
 * iam revocata; actum ut invitationes_revocare */
b32 invitationes_revocare_quamlibet (Invitationes* inv, chorda id);
/* ianua.h: INTRA TRANSACTIONEM; sessiones civis omnes deletae;
 * numerus deletarum aut -I si error */
s32 ianua_sessiones_civis_delere (Ianua* ia, chorda civis);
```

- [ ] **Step 1: Failing tests:**
  - cives `enumerare`: create `beta`, `alpha`, `gamma`; suspend `alpha`,
    delete `gamma` -> three rows in creation order (`beta`, `alpha`,
    `gamma`) with their statuses; empty volumen -> empty Xar (not NIHIL).
  - invitationes `omnes`: authors "" , `A`, `B` each create one ->
    `omnes_enumerare` returns three, newest first; `enumerare(A)` still
    returns only A's.
  - invitationes `quamlibet`: B's invitation revoked by
    `revocare_quamlibet` -> VERUM, `inspicere` shows revoked, the event
    has the same keys as `_revocare`'s; second call -> FALSUM; unknown
    id -> FALSUM; inside a rolled-back transaction -> not revoked.
  - ianua `sessiones_civis`: sign in `fran` twice and `amanda` once
    (three cookies) -> `ianua_sessiones_civis_delere(fran)` == 2; both
    fran cookies now get the sign-in redirect, amanda's still works.
- [ ] **Step 2: Run** `./compile_tests.sh cives`, `invitationes`,
  `ianua` - expect FAIL (undeclared functions).
- [ ] **Step 3: Implement:** `SELECT ... FROM cives ORDER BY creatus_ms,
  id`; `SELECT ... FROM invitationes ORDER BY creata_ms DESC, id DESC`;
  `quamlibet` = `_revocare`'s UPDATE without the author clause (shared
  static helper with an `auctor` flag, so both paths write the same
  event); ianua: `DELETE FROM ianua_sessiones WHERE civis = ?` + `SELECT
  changes()`; the password change calls the same helper (keeping its
  "except the current session" clause).
- [ ] **Step 4: Run** - all three suites PASS.
- [ ] **Step 5: Plants:** `quamlibet` keeps the author clause ->
  `quamlibet` red; session delete without `WHERE civis` -> amanda's
  cookie dies -> `sessiones_civis` red.
- [ ] **Step 6: Linux:** musl + glibc for `cives invitationes ianua`.
- [ ] **Step 7: Commit** `cives, invitationes, ianua: conspectus administrationis (cives_enumerare, invitationes omnium, revocatio quaelibet, sessiones civis deletae) (canalis T3)`.

---

### Task 4: exemplar - admin tools, channel at start, `-invitare` via the channel, `-administrare`

**Files:** Create `apps/exemplar/exemplar_administratio.c`,
`apps/exemplar/exemplar_administratio.h`; modify
`apps/exemplar/exemplar_nucleus.c` (`exemplar_creare`,
`exemplar_claudere`, `exemplar_invitare`), `apps/exemplar/exemplar_internum.h`
(`McpServus* administratio; McpCanalis* canalis;`),
`apps/exemplar/exemplar.c` (`-administrare`, header comment),
`apps/exemplar/exemplar_nucleus.h` (comments: channel, P9),
`apps/exemplar/exemplar.worklog.md`,
`probationes/probatio_exemplar_applicatio.c`; the build lists that name
exemplar's sources (find with `bin/fabrica quid apps/exemplar/exemplar_mcp.c`
and mirror every list `exemplar_mcp.c` is in).

**Interfaces - Consumes:** Task 2 (`mcp_canalis_praebere`,
`_claudere`, `_vocare`, `_pons`), Task 3 (all four), ianua
(`ianua_cives`, `ianua_invitationes`), admissio
(`admissio_concessiones`, `admissio_revocare`), volumen transactions.
**Produces:**
```c
/* exemplar_administratio.h */
/* tabula administrationis (VII instrumenta, scopus "administratio") -
 * numquam in /mcp */
McpServus* exemplar_administrationem_creare (Exemplar* e, Piscina* p);
/* via canalis: <data>/canalis/servus */
#define EXEMPLAR_VIA_CANALIS "canalis/servus"
```
CLI: `exemplar -data D -administrare` (bridge; 2 if no server).

- [ ] **Step 1: Failing tests** in `probatio_exemplar_applicatio.c`.
  Helper `_administrare(e, instrumentum, argumenta_json, p)` (P5):
  `canalis_connectere(<data>/canalis/servus)`, write initialize +
  initialized + tools/call lines, pump `hospitium_gressus` until the
  id-2 line arrives; returns the parsed reply. Sections:
  - `adm_status`: `status` -> salus "ok", `cives.activi` matches.
  - `adm_invitare`: `invitare {nota:"A", dies:3}` -> codex works on
    `/inscribere` (sign-up 303); `expirat_ms` = now + 3 days;
    `dies:0` and `dies:31` -> isError; nothing created.
  - `adm_invitationes`: two invitations (one by web user `fran`, one by
    `invitare`) -> `invitationes_enumerare` lists both, authors `fran`
    and `administratio` (P13), and no element contains a code;
    `invitationem_revocare` on fran's -> the link says "invitatio non
    valet"; again -> isError.
  - `adm_cives`: `cives_enumerare` lists handles and statuses, no key
    named like a hash or token.
  - `suspensio`: fran signed in on the web AND holding an OAuth grant
    (the suite's existing `_quaestio_auctorizandi` flow) ->
    `civem_suspendere {nomen:"Fran"}` -> the web cookie gets the sign-in
    redirect, the bearer token gets 401, sign-in with the right password
    fails; `civem_restituere` -> old cookie and token still dead; a
    fresh sign-in works.
  - `suspensio_limites` (Review focus 4): unknown handle -> isError
    naming it; deleted person -> isError "civis deletus", status
    unchanged; suspending twice -> success, one `civis.status` event.
  - `mcp_separatum` (P6): `/mcp` `tools/list` lacks `invitare`;
    `tools/call invitare` on `/mcp` -> error, no invitation made.
  - `invitare_currens`: fork a child calling `exemplar_invitare(data,
    "B", &codex, p)` (writes the code to a pipe, exits 0) while the
    parent pumps -> code arrives, works on `/inscribere`.
  - `invitare_simul` (Review focus 3): two such children at once -> two
    distinct codes, both valid.
  - `invitare_sistens`: no server (after `exemplar_claudere`) ->
    `exemplar_invitare` takes the lock path -> 0 and a code; socket path
    absent afterwards.
  - `canalis_fractus`: a regular file at `<data>/canalis/servus` ->
    `exemplar_creare` NIHIL, stderr names "canalis fractus"; the lock is
    free (a second `exemplar_creare` after removing the file succeeds).
  - `canalis_longus` (Review focus 5): a data path making the socket
    path 110 bytes -> `exemplar_creare` NIHIL, message names the limit,
    lock free.
- [ ] **Step 2: Run** `./compile_tests.sh exemplar_applicatio` - expect
  FAIL (no admin table / channel).
- [ ] **Step 3: Implement:** the seven handlers in
  `exemplar_administratio.c` (each opens and closes its own
  transaction; P10-P13 rules); `exemplar_creare`: after the migration,
  `e->administratio = exemplar_administrationem_creare(e, p)`,
  `e->canalis = mcp_canalis_praebere(e->administratio, e->reactor,
  <data>/canalis/servus, &err, p)`; NIHIL -> stderr `exemplar: canalis
  fractus: <err>` and `_creatio_fracta(e)`; `exemplar_claudere` closes
  the channel before the hospitium. `exemplar_invitare` (P9): `vocare`
  first; 0 -> parse the JSON text for `codex`; 2 -> existing lock path;
  1/3 -> 1. `exemplar.c`: `-administrare` -> `mcp_canalis_pons(...,
  0, 1)`, 2 -> stderr "exemplar: nullus servus currit", exit 2; usage
  and header comment updated.
- [ ] **Step 4: Run** - expect PASS (all old sections + new ones).
- [ ] **Step 5: Plants:** suspension skips the grant revocation ->
  `suspensio` red (token still 200); admin table mounted on `/mcp` too
  -> `mcp_separatum` red; channel failure ignored (server starts
  without it) -> `canalis_fractus` red.
- [ ] **Step 6: Linux:** musl + glibc for `exemplar_applicatio
  mcp_servus_canalis canalis`; static build
  `./tools/linux/aedificare.sh apps/exemplar/exemplar.c`.
- [ ] **Step 7: Local end to end:** scratch data, port 3011: start the
  server; `exemplar -data D -invitare "x"` while running -> code on
  stdout; journal shows no code; a hand-written session through
  `exemplar -data D -administrare` (initialize, tools/list -> seven
  tools, `status`); stop; `-invitare` while stopped -> lock path code;
  `-administrare` while stopped -> exit 2.
- [ ] **Step 8: Commit** `exemplar: canalis administrationis (VII instrumenta, scopus administratio), -invitare per servum currentem, -administrare pons (canalis T4)`.

---

### Task 5: deploy to smaragda + live checks

**Files:** Modify `apps/exemplar/exemplar.worklog.md`,
`project-specs/canalis-spec.md` (status IMPLETUM + commits),
`project-specs/ianua-plan.md` (a line under P1: stop/invite/start
retired by canalis), this plan (rulings while executing); ledger notes.

Every remote command shown as a dry run first; executed only on Fran's
OK. Touch only exemplar's own user, directories, unit and binary.

- [ ] **Step 1: Backup:** service stopped, `sudo cp -p
  /var/lib/exemplar/exemplar.volumen
  /var/lib/exemplar/exemplar.volumen.ante-canalis`; `sudo ls -l`.
- [ ] **Step 2: Deploy:** `tools/dimittere.sh -agere exemplar smaragda`
  (unit unchanged - no `-cum-unitate`); `/salus` ok; journal shows no
  "canalis fractus"; `sudo ls -l /var/lib/exemplar/canalis` -> dir
  0700, socket 0600, owner exemplar.
- [ ] **Step 3: Smoke:** `tools/exemplar_mcp_fumus.sh
  https://exemplar.golden.earth` all BENE (public `/mcp` unchanged).
- [ ] **Step 4: Channel live:** `ssh smaragda 'sudo -n -u exemplar
  /opt/exemplar/exemplar -data /var/lib/exemplar -invitare "probatio
  canalis"' | sed 's/=.*/=[redactum]/; s/codex: .*/codex: [redactum]/'`
  -> exit 0 and the two redacted lines (the code never reaches my
  output); then over the bridge (three lines piped into
  `-administrare`) `status` -> JSON with the person count, and
  `invitationes_enumerare` -> the test invitation's id (no code);
  `invitationem_revocare` on it.
- [ ] **Step 5: Restart with a stale socket:** `sudo systemctl kill -s
  KILL exemplar` (socket left behind), unit restarts it (or `start`);
  `/salus` ok; channel works again (`status`).
- [ ] **Step 6: Claude Code:** Fran runs `claude mcp add exemplar-admin
  -- ssh smaragda sudo -n -u exemplar /opt/exemplar/exemplar -data
  /var/lib/exemplar -administrare`; from a Claude Code session,
  `status` and `cives_enumerare` work.
- [ ] **Step 7: Close-out:** spec IMPLETUM with commits; worklog entry;
  ianua-plan P1 note; ledger: close desideratum ...ZN with a note;
  ask Fran before removing `.ante-canalis` and `.ante-ianua`.
- [ ] **Step 8: Commit** `canalis: vivum in exemplar.golden.earth - -invitare per servum currentem, Claude Code per pontem ssh (canalis T5)`.
