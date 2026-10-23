# canalis - an admin channel to the running server (spec)

**Status: IMPLETUM 2026-10-21** - plan `project-specs/canalis-plan.md`
T1-T5 (rulings P1-P13 + rulings while executing); commits spec
59030f92, plan 3b214f02, T1 acc64fa5, T2 c97e0391, T3 0b2a4b4a, T4
f06a216b, T5 close-out. LIVE on exemplar.golden.earth (deployed
2026-10-21): `sudo -u exemplar ... -invitare` through the running
server, `-administrare` bridge, Claude Code `exemplar-admin` over ssh
checked by Fran ("mcp server seemed like it all worked"). Approved by
Fran in conversation (sections II.1-II.3 one by one).
Ledger: desideratum ...ZN (`01M5DA62V8PX50XP29G4E3G6ZN`); related
parked LEGATUSD (`01KXJ2HZCPE2D2SX773M44GWMH`, a socket daemon for the
house tools - NOT this spec, but it can stand on `canalis` later).

*Fran 2026-10-20, during the ianua plan: "should we make it so the main
process can serve requests from cli actions somehow? is the policy just
for volumen in general since it's using sqlite?" - then "are there
socket equivalents on windows, etc? like at some point could the socket
stuff kind of be behind a platform layer?" - then "we can do ianua
first and then this".*

Goal: **the command line stops being a second writer.** The house law
is one writing process per volumen (`include/scrinium.h`; ianua P1
enforces it with the `<data>/sera` lock, so today `exemplar -invitare`
only works while the server is STOPPED). With this spec the CLI becomes
a client: it asks the running server - the single writer - to do the
work over a local channel. The same channel, bridged over ssh, lets
Claude Code administer the app as an MCP server.

Decisions (Fran, 2026-10-20, in conversation):
- **Clients: (b)** - the app's own CLI (`-invitare`) AND Claude Code
  through an ssh stdio bridge (`-administrare`). A generic house admin
  CLI for every app (c) waits for a second app with a channel.
- **Tools: (b)** - the ianua admin set (II.3). Account repair (rename,
  password reset link) is its own later spec: it needs a reset page
  with its own token lifecycle.
- **Authority: same uid only.** A peer is admitted iff its uid equals
  the server's effective uid. On smaragda: `sudo -u exemplar ...`; sudo
  logs who did it (auth.log). No admin group, no root exception (root
  can `sudo -u` anyway). No system change on the brownfield droplet.
- **Approach A:** two small libraries (`canalis`, `mcp_servus_canalis`)
  plus exemplar's tool table. Rejected: growing `tcp.h` (names the
  concept after the transport; named pipes would never fit) and a
  second tiny protocol for the CLI (two parsers for seven operations).
- **`-invitare` fallback:** nobody answering on the socket -> today's
  lock path (bootstrap on a fresh data directory still works). The
  lock decides: socket dead but lock held -> exit 2 "servus currit".
- **Suspension cuts access now** (status + web sessions deleted +
  OAuth grants revoked); a restored person signs in again and
  reconnects Claude.
- **Invitation codes may pass through Claude Code** (the bridge's
  `invitare` returns the code; it then sits in that conversation).
  Single use, at most 30 days, revocable, visible in
  `invitationes_enumerare`. The server, `vocare` and `pons` never log
  what they carry; no password ever crosses the channel. The CLI
  remains the off-transcript way to invite.


## I. What exists (quarta d4975ead, 2026-10-20)

- `include/tcp.h`: `TcpConnexio` (client) and `TcpServus` (listening)
  over TCP only; `tcp_ex_fossa(fd, p)` wraps any socket fd as a
  `TcpConnexio` - `tcp_recipere` / `tcp_mittere` (recv/send) work on
  any stream socket, so an AF_UNIX fd needs no new I/O layer.
- **Defect found while reading:** `tcp_ex_fossa` (lib/tcp_posix.c:545)
  does NOT call `_ponere_nosigpipe`. On macOS a write to a peer that
  hung up raises SIGPIPE and kills the process (Linux is safe:
  `tcp_mittere` sends with MSG_NOSIGNAL). Fixed in this spec (II.1);
  no AF_UNIX code exists anywhere in lib/ yet.
- `include/reactor.h`: poll()-based loop with fd callbacks, timers,
  deferred labour; hospitium and exemplar run on it.
- `include/mcp_servus.h`: transport-agnostic MCP core -
  `mcp_servus_tractare(servus, corpus, persona, initium_postulatum,
  reddere, datum, p)` takes one JSON-RPC body and answers once through
  `reddere` (now or later: `mcp_differre`); tool tables are typed and
  scoped; `McpPersona{usor, cliens, scopi}`. The only local transport,
  `mcp_servus_stdio_currere`, is a BLOCKING `FILE*` loop.
- ianua (in quarta, IMPLETUM): `cives` (no list-all function),
  `invitationes` (`_enumerare` / `_revocare` scoped to ONE author),
  `ianua` (session delete exists only inside the password change),
  `admissio_concessiones(usor)` + `admissio_revocare(usor, concessio)`
  (enough to revoke all of a person's grants).
  Sign-in, sessions and the OAuth adapter all require
  `CIVIS_ACTIVUS` (lib/ianua.c:446, :1194; lib/credentialia.c:357).
- exemplar: `-invitare NOTA [-basis URL]` takes `<data>/sera` and
  writes directly (exit 2 if the server holds the lock). The unit runs
  as `User=exemplar`, `ReadWritePaths=/var/lib/exemplar`,
  `RestrictAddressFamilies=AF_INET AF_INET6 AF_UNIX` - nothing to
  change for a socket under the data directory.
- Naming: "canalis" is also examen code 96's word for the `filum` I/O
  discipline (`canalis_debita.tsv`); no C symbol uses the prefix. The
  library takes it; the examen vocabulary is unaffected.


## II. Design

### II.1 `canalis` - the local channel

Named for the concept ("a channel to the running server, proved by OS
identity"), not for the mechanism. `include/canalis.h`,
`lib/canalis_posix.c` (AF_UNIX; `postulata_posix.h` first).

```c
nomen structura CanalisAuditor CanalisAuditor;

nomen structura {
               b32  successus;
    CanalisAuditor* auditor;
            chorda  error_descriptio;
} CanalisResultus;

CanalisResultus canalis_audire       (constans character* via, Piscina* p);
TcpResultus     canalis_accipere     (CanalisAuditor* a, Piscina* p);
integer         canalis_fossa        (CanalisAuditor* a);
vacuum          canalis_claudere     (CanalisAuditor* a);
TcpResultus     canalis_connectere   (constans character* via, Piscina* p);
b32             canalis_par_uid      (TcpConnexio* c, i32* uid);
b32             canalis_par_admissus (TcpConnexio* c);
```

- `canalis_audire(via)`: PRECONDITION (in the header): the caller holds
  the app's single-writer lock - that is what makes removing a stale
  socket safe (no live server can own it).
  - Parent directory absent -> created 0700. Present with ANY group or
    other bit -> refused ("directorium canalis apertum"), never
    tightened silently.
  - At `via`: a socket -> removed; anything else (regular file,
    directory) -> refused and left alone.
  - Path longer than `sun_path` allows (104 bytes macOS, 108 Linux,
    counting the NUL) -> refused with a message naming the limit,
    never truncated.
  - bind, then `chmod 0600` explicitly (umask-independent), listen,
    non-blocking.
- `canalis_accipere`: non-blocking; nothing pending ->
  `TCP_ERROR_ITERUM`. Returns a `TcpConnexio*` via `tcp_ex_fossa`.
- `canalis_claudere`: closes and removes `via`.
- `canalis_connectere(via)`: blocking connect; any failure ->
  `successus` FALSUM with a description (callers do not need to tell
  "absent" from "refused": the lock arbitrates, see II.3).
- `canalis_par_uid`: `getpeereid` (macOS) / `SO_PEERCRED` (Linux);
  FALSUM if the credentials cannot be read.
  `canalis_par_admissus`: `par_uid` read AND equal to `geteuid()`;
  unreadable -> FALSUM (fail closed).
- **`tcp_ex_fossa` fix:** it calls `_ponere_nosigpipe` like every other
  constructor (covers every caller, not only canalis).
- **Windows: no code now.** The header records the two candidates for
  the port: AF_UNIX through Winsock (Win10 1803+, WSAPoll; no peer
  credentials -> admission by directory ACL instead of uid), or named
  pipes (ACL + `GetNamedPipeClientProcessId`, but IOCP rather than
  poll, and not a `TcpConnexio`). Decided with the port.

### II.2 `mcp_servus_canalis` - MCP over the channel

`include/mcp_servus_canalis.h`, `lib/mcp_servus_canalis.c` (portable:
everything platform-specific is in canalis; the bridge uses `poll()` -
POSIX, `postulata_posix.h`).

```c
nomen structura McpCanalis McpCanalis;

McpCanalis* mcp_canalis_praebere (McpServus* servus, Reactor* reactor,
                constans character* via, chorda* error, Piscina* p);
vacuum      mcp_canalis_claudere (McpCanalis* mc);

s32 mcp_canalis_pons   (constans character* via, integer intra,
                        integer extra);
s32 mcp_canalis_vocare (constans character* via,
                        constans character* instrumentum,
                        JsonValor* argumenta, chorda* textus,
                        Piscina* p);
```

**Server** (`praebere`; reactor-driven, same loop as hospitium):
- Fixed persona for every call: `{ usor "administratio", cliens
  "canalis", scopi "administratio" }` - the admin table's tools carry
  scope `administratio`, so the table cannot be mounted on `/mcp` by
  mistake and still work for an OAuth persona.
- Accept; peer fails `canalis_par_admissus` -> closed at once, one
  stderr line `canalis: par recusatus` (no uid of ours, no payload).
- At most VIII connections; the ninth is closed at accept.
- Newline framing, as stdio. A line over 1 MiB closes that connection.
  `initium_postulatum` VERUM (initialize first, as stdio).
- Each JSON-RPC body -> `mcp_servus_tractare`; each reply is one line
  appended to the connection's write queue; on `EAGAIN` the queue waits
  for writability (reactor write interest).
- Deferred replies work; a connection that closed while a call was
  pending is found gone by generation number, and the reply is dropped.
- No idle timeout: a Claude Code session idles for a long time, and
  the only party able to fill the eight slots is our own uid.
- Never logs request or reply bodies.

**`pons(via, intra, extra)`** - the stdio bridge (`-administrare`):
- connect; failure -> 2 ("nemo audit").
- `poll()` on the raw fds (never `FILE*`, so stdio buffering cannot
  stall it), copying bytes both ways; EOF on `intra` -> `shutdown`
  write side, drain the socket to EOF -> 0. Write failure -> 1.

**`vocare(via, instrumentum, argumenta, &textus, p)`** - one call:
- connect (failure -> 2); send `initialize`, `notifications/
  initialized`, `tools/call`; read lines until the reply with the
  call's id; 30 s overall timeout (-> 3).
- 0 = success, `textus` = the result text; 1 = the tool ran and failed
  (`isError`), `textus` = its message; 3 = protocol error or timeout.

### II.3 exemplar's admin tools, library additions, CLI

**Tools** - `apps/exemplar/exemplar_administratio.c`, a second
`McpServus` (separate table from `/mcp`), every tool scope
`administratio`, served at `<data>/canalis/servus`:

| tool | arguments | does |
|---|---|---|
| `invitare` | `nota`*, `dies` (default VII, max XXX) | creates an invitation (auctor "" = administration, as the CLI today); returns the code, and the link when `-basis` is set |
| `invitationes_enumerare` | - | all invitations, every author: id, author's handle ("" -> "administratio"), note, created, expires, uses, revoked; never codes |
| `invitationem_revocare` | `id`* | revokes any live invitation; unknown / already revoked -> isError |
| `cives_enumerare` | - | handle, display name, status, created; nothing secret |
| `civem_suspendere` | `nomen`* | one transaction: status SUSPENSUS + web sessions deleted + every OAuth grant revoked |
| `civem_restituere` | `nomen`* | status ACTIVUS (no sessions or grants come back) |
| `status` | - | health, event count, people by status, live invitations |

`nomen` arguments go through `cives_nomen_normare` (so "Fran" finds
`fran`); unknown handle -> isError naming it.

**Library additions** (each tested in its own suite):
- `cives_enumerare(c, p)` -> `Xar` of `Civis`, creation order.
- `invitationes_omnes_enumerare(inv, p)` -> every author, newest first.
- `invitationes_revocare_quamlibet(inv, id)` -> INTRA TRANSACTIONEM;
  FALSUM if unknown or already revoked; event as `_revocare`.
- `ianua_sessiones_civis_delere(ia, civis)` -> the delete the password
  change already does, exposed (count of sessions removed).

**CLI** (`apps/exemplar/exemplar.c`):
- `-invitare NOTA [-basis URL]`: `mcp_canalis_vocare(<data>/canalis/
  servus, "invitare", ...)`. 0 -> print code (and link) to stdout,
  exactly as today. 2 -> fall back to the lock path (unchanged:
  server holding the lock -> exit 2 "servus currit"). 1 / 3 -> stderr,
  exit 1.
- `-administrare`: `mcp_canalis_pons(<data>/canalis/servus, 0, 1)`;
  2 -> stderr "nullus servus currit", exit 2.

**Server start** (`exemplar_creare`): after the lock and the
migration, `mcp_canalis_praebere`. Failure -> `exemplar: canalis
fractus: <causa>` on stderr and exemplar refuses to start (through
`_creatio_fracta`), like a failed migration - loud, and `dimittere`'s
health check rolls a bad deploy back. `exemplar_claudere` closes the
channel (socket path removed).

**Deploy:** `tools/dimittere.sh -agere exemplar smaragda` (unit
unchanged). Live: `sudo -u exemplar /opt/exemplar/exemplar -data
/var/lib/exemplar -invitare ...` while running; stale socket survives a
restart (kill -9, start, channel works); Claude Code:
`claude mcp add exemplar-admin -- ssh smaragda sudo -u exemplar
/opt/exemplar/exemplar -data /var/lib/exemplar -administrare`, then
`status` and `cives_enumerare` from Claude.

With this, ianua P1's stop / invite / start is retired; P1's lock stays
(it is what makes the channel's stale-socket removal safe and the
fallback correct). ianua's API does not change.

### II.4 Order of work (shape for the plan)

1. `canalis` + the `tcp_ex_fossa` SIGPIPE fix.
2. `mcp_servus_canalis` (server, `pons`, `vocare`).
3. Library additions (`cives_enumerare`, `invitationes_omnes_enumerare`,
   `invitationes_revocare_quamlibet`, `ianua_sessiones_civis_delere`).
4. exemplar: admin tools, channel at start, `-invitare` via the
   channel with fallback, `-administrare`.
5. Deploy + live checks (including Claude Code over ssh).


## III. Tests

- **canalis** (new suite): round trip; `par_uid` equals our uid (plant:
  uid never read -> red); directory 0700 and socket 0600 after
  `audire`; group-readable directory refused; stale socket replaced;
  regular file at the path refused and still there; too-long path
  refused; `claudere` removes the path; `tcp_ex_fossa` connection
  writing to a closed peer returns an error instead of killing the
  process (forked child; red on macOS without the fix).
  The cross-uid refusal itself cannot run without root: it rests on the
  `par_uid` test plus the one-line comparison - stated, not hidden.
- **mcp_servus_canalis** (new suite; server in a forked child with a
  test tool table, parent as client): `vocare` round trip; deferred
  reply; `isError` -> 1; over-long line closes that connection, the
  next client is served; ninth connection refused; client leaves
  mid-call, server survives and answers the next; `pons` relays a full
  session through pipes; `vocare` with nobody listening -> 2; a
  persona without scope `administratio` cannot call the tools (table
  mounted under `mcp_servus_tractare` with an OAuth-like persona).
- **cives / invitationes / ianua**: one section per addition (list
  order; any-author revoke + event; already-revoked refused; session
  delete count and other people's sessions untouched).
- **exemplar**: each admin tool through `vocare` against a running test
  server; suspension cuts a live web session AND a live grant at once;
  restore does not revive them; `-invitare` while running writes
  through the server (code works on `/inscribere`); `-invitare` while
  stopped uses the lock path; channel failure at start (regular file at
  the socket path) -> refuses to start, lock released.
- Linux: musl + glibc for canalis, mcp_servus_canalis,
  exemplar_applicatio (SO_PEERCRED path).


## IV. Not in this spec

- Generic house admin CLI for every app (decision (c), later).
- Account repair: rename, password-reset link (own spec).
- Windows implementation (header notes only).
- LEGATUSD (house-tool daemon) - may build on canalis, separately.
- Remote admin without ssh (never over TCP: the channel is local by
  definition).


## AUDIENDA (not verified)

1. `getpeereid` on macOS and `SO_PEERCRED` on Linux musl: header
   availability under our `postulata_posix.h` feature macros
   (`_GNU_SOURCE` may be needed for `struct ucred` on glibc/musl).
2. Reactor write interest: whether `reactor_modificare` already
   supports toggling POLLOUT per fd as the write queue needs (hospitium
   has a write path - reuse its pattern).
3. `sudo -u exemplar` over non-interactive ssh on smaragda (no tty;
   earlier `ssh smaragda 'sudo ...'` worked, so NOPASSWD is likely, but
   `-u exemplar` may hit a different sudoers rule).
4. macOS test paths: `volumen_temporarium` lives under /tmp; a socket
   under a long `$TMPDIR` (`/var/folders/...`) could exceed 104 bytes -
   tests use short /tmp paths.
