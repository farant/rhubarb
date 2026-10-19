# hospitium v3 plan - web apps on the droplet (exemplar.golden.earth)

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> subagents. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** a small C89 web app with login (`apps/exemplar/`) runs on Fran's
droplet `smaragda` behind nginx as `https://exemplar.golden.earth`, built
only on house libraries that this arc makes reusable.

**Architecture:** one process, one `reactor.h` loop: hospitium serves HTTP
with deferred responses and streamed bodies; CPU work (argon2id) runs as
cooperative reactor jobs; state is one volumen (events + blobs); the app is
a static musl amd64 binary under systemd, deployed by script over ssh, DNS
via a house DigitalOcean client.

**Tech stack:** C89 + latina.h; reactor (poll), hospitium, http parser,
url, json, scrinium + vendored SQLite (incrblob available), volumen,
sigillum (SHA-256, incremental, HMAC, constant-time compare), moneta
(/dev/urandom, no fallback), tls13 client + fiducia_macos (outbound from
the Mac only), credo v2; oracles: RFC 7693 and RFC 9106 texts committed
unchanged, curl as differential client, a local fake of the DigitalOcean
API; Linux via OrbStack Docker (`tools/linux/` glibc + musl images).

**Spec:** `project-specs/hospitium-v3-spec.md` (quarta dfd5ff25). Read it
with this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh` (-pedantic -Wall
  -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual
  -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings);
  identifiers, comments, messages Latin; never a latina.h macro as an
  identifier; `i32`/`i64` UNSIGNED, `s32`/`s64` signed; chorda not
  NUL-terminated; new words pass the Latin lint (proper nouns and
  abbreviations -> `ignotum-permissum`, real Latin or English unknown to
  the oracle -> rename).
- POSIX .c files include `postulata_posix.h` FIRST.
- No raw `fopen`/`open`/`stat`/`access` outside filum (examen code 96) -
  EXCEPT the DigitalOcean token read, which must NOT go through filum or
  lectiones (fabrica seals the content of paths lectiones records) and is
  a tolerated raw read (`<tolera>`, the anthropic.clavis precedent). Never
  read a secret from the environment (`lectiones_ambitus` records values).
- Secrets (token, server secret, passwords, session tokens) are never
  printed, logged, put in error messages, or written by a test into
  output.
- Nothing above `hospitium.h`/`reactor.h` breaks: existing hospitium
  users and `probationes/probatio_hospitium.c` (credo v1, 11 groups) stay
  green untouched; new behaviour is opt-in.
- One loop, no threads: CPU work is sliced into reactor jobs (~1-2 ms per
  step).
- Droplet `smaragda` is brownfield: we create only our own user
  (`exemplar`), directories (`/opt/exemplar`, `/var/lib/exemplar`), unit
  and nginx site; `nginx -t` before every reload; every remote change is
  shown as a dry run first and executed only after Fran's OK.
- The app listens on 127.0.0.1:3010 only; argon2id at most 2 at once
  (droplet: 1 CPU, ~420 MB available).
- Commits via detached `silva.commissio(nuntius, viae)` (absolute-path
  worktree guard); formator `-scribere` + `-vitia` and the Latin lint
  before; plants via `silva.planta` (compile pre-check; red, revert,
  green); a mute plant is a test gap, fixed before commit; NO installs
  from the worktree; main untouched until Fran's go.

## Rulings made while planning

- **P1 - lectiones and secrets.** `lectiones_fopen` records PATHS only,
  only when `FABRICA_LECTIONES` is set, and never paths under `/dev/`:
  `moneta_octeti_fortuiti` (/dev/urandom) is safe for secrets. The
  token file is read raw (tolerated) because a recorded path gets its
  content sealed by fabrica; `lectiones_ambitus` records env VALUES, so
  no secret from the environment.
- **P2 - SQLite.** Vendored build flags (compile_tests.sh): FTS5,
  THREADSAFE=0, DQS=0, DEFAULT_MEMSTATUS=0, OMIT_LOAD_EXTENSION,
  OMIT_DEPRECATED, DEFAULT_WAL_SYNCHRONOUS=1 - no OMIT_INCRBLOB, so
  `sqlite3_blob_open/read/write` exist.
- **P3 - parser hook.** The server parser's states LINEA -> CAPITA ->
  CORPUS -> COMPLETA have a clean seam at the empty line. A
  `capita_solum` mode stops there (new status
  `HTTP_PARSE_CAPITA_COMPLETA`) and exposes the body bytes already
  buffered; hospitium takes the rest. Chunked REQUEST bodies stay 411
  (browsers and `curl -F` send Content-Length).
- **P4 - reactor.** Fixed tables with a generation counter per fd slot
  already exist - the colloquium generation (T2) follows that precedent.
  Signals: ONE process-wide self-pipe (signals are process-wide by
  nature) - the only static state added; one reactor per process owns
  signals (a second registration from another reactor is refused).
- **P5 - hashing.** `sigillum` has incremental SHA-256
  (`sigillum_incipere/addere/finire`), `sigillum_hmac`, constant-time
  `sigillum_aequale`: custodia and the upload hasher use sigillum, not
  BearSSL.
- **P6 - DigitalOcean.** Scopes `domain:read`, `domain:create`,
  `domain:update` (and `domain:delete` if wanted) cover domains AND their
  records; write scopes need `domain:read`. Personal tokens start
  `dop_v1_` - the client refuses a file without that prefix (sanity, not
  security).
- **P7 - Docker** is OrbStack; its daemon must be running for T9-T11
  (Fran starts it; Claude never starts GUI apps).
- **P8 - tests.** New suites are credo v2 files (`CredoSectio` tables,
  `credo_suitam_currere[_cum]`); old suites untouched.
- **P9 - RFC oracles** as for RFC 8448: `probationes/fixa/argon2/rfc9106.txt`
  and `probationes/fixa/blake2/rfc7693.txt` committed unchanged (sha256
  in the test header), tests extract the vectors from the text - no
  hand-copied hex.
- **P10 - plan location** `project-specs/hospitium-v3-plan.md` (house).
- **P11 - names** (working, lint decides): `multipartita` (multipart
  parser), `crustula` (cookies), `blake2b`, `argon2id`, `custodia`,
  `oceanus` (DigitalOcean DNS), `exemplar` (app).

## Rulings made while executing

- **R1 (T1)** - "pass snapshot dropped" is an EQUIVALENT mutation: the
  per-cycle birth check (`cyclus_natalis`) already skips every job born
  in the current cycle, appended or in a reused slot. Section beyond the
  plan: `locus_reusus` (a job reusing a freed slot ahead of the loop
  cursor) - the case the birth check exists for; its plant is red.
- **R2 (T1)** - `poll()` returns EINTR on a signal even with SA_RESTART
  (Darwin and Linux): `reactor_poll` drains the signal pipe on EINTR;
  `signum_solum` pins it.
- **R3 (T1)** - the signal pipe is an internal fd entry (`internus`):
  excluded from `reactor_numerus_fd`, still polled; `reactor_destruere`
  releases signal ownership (SIG_DFL restored, pipe closed).
- **R4 (T1)** - examen's POSIX model gained SIGUSR1/SIGUSR2/SA_RESTART
  (Darwin values) instead of tolerating the code; the test's static
  became a struct field (examen's credo rule: no state across
  sections).
- **R5 (T1)** - Latin lint: `reusus` (coinage) -> `locus_iteratus`;
  POSIX names `SIGUSR1`, `SIGUSR2`, `SA_RESTART` go to the tolerated copy
  (system words live there, like cloexec/nonblock/sigwinch - not the
  glossary): deliberate regeneration, diff exactly +restart +sigusr1
  +sigusr2 and the two reported evanida (-hebd -sanctorale).
- **R6 (T2)** - deferral is a VALUE handle `HospitiumDilatum {hospitium,
  locus, generatio}`, not a colloquium pointer + `colloquium_valet`: the
  colloquium lives inside its connection slot, so a stale pointer to a
  reused slot would look valid. `dilatum_valet` / `dilatum_colloquium`
  compare the slot's generation (bumped at accept, at each request, at
  close, and at the 503 deadline). The `generatio` section defers a
  SECOND request in the same slot - with an immediate second response
  the slot's `dilatum` flag alone rejects the old handle and the plant
  was mute; strengthened until red.
- **R7 (T2)** - (a) the test client's receive timeout is 20 ms (was 2 s):
  the harness pumps the server between reads, a blocking client starved
  it (expiratum/molliter failed on the harness, not the code); (b) a
  waiting (EXSPECTANS) connection keeps read interest so a client that
  leaves invalidates the handle at once instead of at the deadline;
  (c) `colloquium_respondere_filum` returns b32 and sends the 404 itself
  (the handler cannot forget it); (d) filum gained a sequential byte
  reader (`filum_octeti_aperire/legere/claudere`, via `lectiones_fopen`)
  - file sources stream 16 KB at a time and never load whole.
- **R8 (T3)** - multipart FIELDS get an accessor in T3:
  `HospitiumCampus {campus, valor}` + `colloquium_campi(col, &campi)`
  beside `colloquium_fila`. The plan's T3 interface stored fields but
  exposed nothing, while T3's own tests (and curl's) must read them;
  T4's `colloquium_forma` reads the same array.
- **R9 (T3)** - the boundary matcher is KMP over D = CRLF "--" limes:
  the held bytes ARE a prefix of D, so there is no lookbehind buffer to
  shorten. The plan's "lookbehind - 1" plant became the failure it was
  aiming at: "flush the held prefix as data at a fragment end" ->
  `secta` red. A full D is followed by a short PROSPECTUS ("--", LWSP*
  CRLF, else data), so `\r\n--limesX` is data. The 413 plant removed
  the parser's existing Content-Length check (hospitium has none of its
  own) -> `nimis_magnum` red. The filename* plant keeps the decode call
  (else -Wunused-function) and discards its result.
- **R10 (T3)** - semantics: in `capita_solum` mode `petitio_maxima`
  bounds the BODY (Content-Length, 413 before reading) and the header
  block has its own cap `HTTP_CAPITA_SOLUM_MAXIMA` 64 KB -> 431 (an app
  setting 64 MB must not allow 64 MB of headers). The v1 suite stays
  green. Multipart bodies leave `petitio->corpus` empty (fields +
  files instead). `Expect: 100-continue` is answered at once (curl -F
  above 1 MB otherwise waits 1 s) - latency only, no red plant.
- **R11 (T3)** - `filum_scriptor_octetos_scribere(scriptor, constans
  i8*, n)` joins the T2 byte reader instead of casting away const.
  Temp files: `<directorium_temporarium>/hospitium-<nanoid>.tmp` via
  filum (mode from umask) - the droplet app sets a private 0700
  directory on the same filesystem as its storage (rename). Client
  filenames: last segment after `/` or `\`, control bytes dropped, "."
  and ".." -> empty. Lint: `multipartitus` glossed (Late Latin, like
  bipartitus/tripartitus); `charset` -> `codex_signorum`,
  `Transcriptio` -> `Vestigium`, `_sectio_curl` ->
  `_sectio_cliens_alienus` (curl is real English - the taylor lesson).
- **R12 (T4)** - header injection guard (not in the plan): every
  response header goes through one `_caput_ponere` - the title must be
  a token, the value may not contain CR, LF or NUL (before T4
  `colloquium_caput_addere` passed CR/LF through). Refused or over-cap
  headers are counted (`HospitiumFructus.capita_abiecta`, new) and the
  first is named in the access log. Cap = 32 app headers + 2 reserved
  slots (Content-Type, Location), so a full set never loses its
  Content-Type. `colloquium_redirigere` with CR/LF in the location ->
  500, never an injected header. A fourth plant pins it (CR/LF check
  removed -> `capita_tectum` red).
- **R13 (T4)** - crustula beyond the plan: `__Secure-` / `__Host-`
  prefix rules enforced; Path/Domain refused with `;` or CTL (attribute
  injection). Absent vs present-empty: absent = `datum NIHIL`, present
  but empty = non-NIHIL with mensura 0 (crustulum_valor,
  colloquium_quaestio, colloquium_forma). All `Cookie` headers are read.
  `colloquium_cliens` takes the LAST X-Forwarded-For header's last hop
  (what nginx appended); loopback = peer "127.*" (IPv4 -
  `tcp_obtinere_peer_address` is AF_INET only). `colloquium_forma`
  parses urlencoded only from an in-memory body.
- **R14 (T4)** - lint: `redirigere` glossed as a house verb (spec name
  kept); renames `caput_cookie` -> `caput_crustulorum`, `xff` ->
  `procurati`, `_injectio` -> `_locus_malus`, `_sectio_curl_vas` ->
  `_sectio_vas_crustulorum`. The first header plant was mis-written (my
  mutation broke the compare loop instead of making it case-sensitive,
  so it stayed green); rewritten as a plain byte compare -> red.
- **R15 (T5)** - `argon2id_digestum(opus, exitus)` as the plan has it
  (the spec's extra `n` dropped: the length is `parametri.longitudo`).
  Salt 8..64 bytes, tag 4..64 (RFC 9106 allows longer tags; one BLAKE2b
  output is plenty for passwords). The work memory and H0 are zeroed
  after the digest. The RFC test checks the pre-hashing digest H0 -
  recomputed IN THE TEST from the RFC formula with blake2b, so no
  test-only API - and the final tag; the RFC's per-pass block dumps are
  not compared. Production parameters (19 MiB, t=2, p=1) take ~25-35 ms
  on the Mac.
- **R16 (T5)** - (a) the "resume off by one" plant skips one block
  forward on each pause; a backward step loops forever at slice size 1
  (first attempt hung, was killed, its "red" discarded). (b) RFC text
  trap: "5.3.  Argon2id Test Vectors" also appears in the table of
  contents, and the scan from there read section 5.1 (Argon2d: same
  inputs, different digest) - the reader anchors at the heading line.
  (c) Lint: blake2b, blake2, argon2id, argon2, phc, blamka ->
  ignotum-permissum (the sha256/hmac precedent); helpers renamed
  (`_le32*` -> `_quaterni*`, `_le64` -> `_octoni_legere`, `ROTR64` ->
  `CIRCUMVOLVERE`, `_hprime` -> `_digestum_varium`).
- **R17 (T6)** - the incremental blob API lives in SCRINIUM
  (`ScriniumMassa`: `scrinium_massam_aperire/legere/scribere/claudere`,
  `scrinium_ligare_massam_nullam` = zeroblob), not as raw sqlite3 calls
  in volumen: volumen never sees sqlite3.h, the layering stays.
  `volumen_scrinium(vol)` added (the test reads `PRAGMA synchronous`;
  apps will want `VACUUM INTO` for backups). The streamed insert runs in
  a SAVEPOINT, so a refusal inside the caller's transaction does not
  undo the caller's work (tested). The parameter is `sigillum_notum`
  (the plan's `sigillum_hex` shadowed the function `sigillum_hex()`).
  Extra section `mensura_mendax` (file longer, shorter, missing).
- **R18 (T6)** - the blob READER opens and closes the sqlite3_blob per
  chunk. My first reason (an open handle would delay autocommit
  commits) was WRONG - a plant holding the handle stayed green, modern
  SQLite commits with reads pending. The real cost of a held handle is
  a pinned WAL snapshot: `PRAGMA wal_checkpoint(TRUNCATE)` fails during
  a long download and the WAL grows. The section
  `scriptio_inter_lectionem` now pins that (held handle -> red).
- **R19 (T7)** - the server secret is read and written RAW (open /
  read / fstat / write / fsync; create with O_CREAT|O_EXCL|O_NOFOLLOW,
  0600; refuse a wider mode, a non-regular file, a size other than
  32), NOT through filum as T7's step said: P1 and the global
  constraint win - a path lectiones records gets its CONTENT sealed by
  fabrica, and this is a secret. Two `<tolera
  codex="CANALIS_PRAETERITUS">` (the anthropic.clavis precedent).
  examen's POSIX model gained O_EXCL, O_NOFOLLOW (Darwin values), fstat
  and fsync; the tolerated-word copy was regenerated deliberately:
  exactly +excl +fstat +fsync +nofollow.
- **R20 (T7)** - API and policy details:
  - `custodia_tesseram_hashare` -> `custodia_tesseram_digerere` (lint:
    "hashare" is a coinage; digerere is Latin and matches digestum).
  - The unknown-user hash uses the CONFIGURED parameters (production by
    default), not a fixed PHC string, so it always costs what a real
    current verify costs; a PHC that does not parse, or asks for more
    than m 256 MiB / t 10 / p 8, is treated the same way (a corrupt
    row cannot make the server allocate gigabytes).
  - Jobs live in a fixed table of 64 (running + waiting); a full table
    returns FALSUM (the app answers 503) - memory stays bounded.
  - Throttle: delay 250 ms doubling from the N-th failure, refusal
    from 2N, window counted from the LAST failure; fixed table of 1024
    entries keyed by SHA-256(key), expired or oldest entry evicted.
  - Test hooks: `custodia_opera_currentia` (plan) and
    `custodia_gradus_summa` (step count for the unknown-user check).
- **R21 (T7)** - the plan's `DCCCC` is `CM` (900); `csrf` glossed as
  ignotum-permissum. "Constant-time compare used" is by construction
  (OR-accumulated XOR over the full length), not timing-tested.
- **R22 (T8)** - naming:
  - CLI flags are single-dash like every house app (`-data -portus
    -hospes -invitatio -procurator -acta`), not the spec's `--data`.
  - Config gained `acta_accessus` and `frenum_conatus` (0 = V; the test
    uses III so "delayed then refused" takes 7 attempts, not 11).
  - The test is `probatio_exemplar_applicatio.c`: `probatio_exemplar.c`
    already EXISTS (lib `exemplar`, the pattern library) and my first
    `scribe` overwrote it - caught in `git diff HEAD`, restored from
    HEAD, source list regenerated (lib/exemplar.c had dropped out).
    Also: compile_tests' filter is a substring, so `exemplar` matched
    five suites and broke `CREDO_SECTIO` plants (ledger park ...GF9X).
  - OPEN for Fran: the app's `exemplar_*` functions share the prefix of
    lib `exemplar` (pattern matching). No symbol clash today; a rename
    of the app code is cheap (`renominare.sh`) if wanted.
- **R23 (T8)** - behaviour choices:
  - Session expiry uses wall-clock `time()` (persisted across
    restarts); `tempus_nunc` is monotonic per process. Absolute 30-day
    lifetime only - the spec's idle limit is NOT implemented (deferred).
  - Downloads are always `application/octet-stream` + `attachment` +
    nosniff + `CSP: sandbox`, whatever MIME the uploader claimed. Pages
    carry `CSP default-src 'none'; style-src 'unsafe-inline';
    form-action 'self'; frame-ancestors 'none'`, nosniff,
    Referrer-Policy same-origin.
  - No session, unknown file and someone else's file all answer the
    same 404. Throttle delays are reactor timers (deferred, never
    blocking); both the client IP and the account name are keys.
- **R24 (T8)** - test harness: the client and the server share one
  thread and argon2id advances one step per reactor cycle, so the first
  version (20 ms client timeout, one pump per wait) took 675 s; the
  client now waits 1 ms and pumps 20 cycles between reads -> ~10 s.
- **R25 (T9)** - portability findings, each fixed at its source:
  - glibc `_FORTIFY_SOURCE` (Ubuntu gcc at -O2) marks read/write
    `warn_unused_result`, which gcc does NOT silence with `(vacuum)`:
    best-effort calls now assign to a variable (`lib/lectiones.c`,
    `lib/processus_posix.c` x2, `probatio_reactor_labores.c`). Same
    pattern remains outside T9's closures (`lib/pseudoterminale_posix.c`,
    `probatio_manus.c`, `probatio_reactor_mutatio.c`) - untouched.
  - musl's FD_SET macros trip -Wsign-conversion: `processus_posix.c`
    moved from select() to poll() (also lifts FD_SETSIZE 1024; same
    API as the reactor). Its musl pin in `exspectata_musl.txt` became
    VANA and was removed.
  - `lib/credo.c` -Wformat-overflow (gcc -O2 cannot see the length
    check through sprintf): memcpy after the existing check.
  - No POSIX variants existed for three macOS-only units; added
    `lib/fenestra_tempus_posix.c` (real: clock_gettime MONOTONIC,
    nanosleep), `lib/fiducia_posix.c` and `lib/tls_securetransport_posix.c`
    (honest refusals with named reasons - outbound HTTPS from Linux is
    out of scope, spec IV). aedilis picks them by the `_posix` suffix;
    the macOS closure is unchanged (source list identical).
  - `lib/http.c`: reason phrase for 303 ("See Other") was missing
    ("303 Unknown" on the wire).
- **R26 (T9)** - runner and build: `tools/linux/probatio.sh` gained
  `-musl`, `--varians posix` closures, include roots from `bin/aedilis
  --radices` (apps/ included), vendor objects with the compile_tests
  vendor flags, and -O2. Both images gained curl (glibc too, so the
  differential sections run there as well). BearSSL's amalgam bakes in
  `inner.h` once, before any x86 intrinsics file can set
  `BR_ENABLE_INTRINSICS`, so on amd64 `BR_TARGETS_X86_DOWN` is
  undefined; the Linux vendor build passes `-DBR_ENABLE_INTRINSICS=1`
  (the generated amalgam is not hand-edited; secunda is working on its
  generator). Plain alpine has no curl - the in-container check uses
  busybox wget.
- **R27 (T9)** - `./porta_linux.sh -musl` is RED with 11-12 fractures in
  files T9 never touched (missing include roots in the porta runner,
  *_macos.c compiled on Linux, macOS util.h, -Wformat-overflow in
  diurna.c, thesaurus.c ...): drift since 2026-08-03, parked
  (...JWP9A), not fixed here.
- **R28 (T10)** - deployment shape:
  - The spec's ExecStart has no invite code, which would leave
    registration closed. exemplar gained `-invitatio-via <file>`; the
    unit passes `/var/lib/exemplar/invitatio`, which the setup script
    creates with a random code ON THE DROPLET (0600, owner exemplar) -
    never in the unit, in `ps`, or on the Mac. Fran reads it with
    `ssh smaragda sudo cat /var/lib/exemplar/invitatio`.
  - Scripts use scp (rsync may be absent) into /tmp, then `sudo install`
    with owner and mode; `-siccum` is the default and prints every
    local and remote command; the deploy dry run also prints the
    rollback it would run. On the very first deploy there is no
    `.prior`, so a failed health check cannot roll back - the script
    says so by name.
  - The unit adds hardening beyond the spec list (PrivateDevices,
    ProtectKernel*, ProtectControlGroups, RestrictAddressFamilies,
    MemoryMax 256M - two argon2id jobs at 19 MiB fit easily).
- **R29 (T10)** - oceanus safety: the next-page URL must start with
  the configured base (otherwise error, not followed - the token is
  never sent to another host); two records with the same type and name
  -> ERROR, nothing touched; IDEM compares data AND ttl; the domain is
  validated `[a-z0-9.-]` before it enters a URL; the token file is
  opened O_NOFOLLOW, must be regular, mode `& 077 == 0`, prefix
  `dop_v1_`; the Bearer value is zeroed right after the request copies
  it. The parameter is `scheda` (`registrum` is a latina.h macro).
  The hostname split is "first label / rest" (exemplar | golden.earth),
  enough for this domain; multi-label public suffixes would need
  explicit arguments.
- **R30 (T10)** - tests: the fake DigitalOcean API is hospitium in a
  forked child, seeded before the fork, counting writes behind `GET
  /_contator`. Lint: `ttl` glossed (DNS abbreviation); renames
  `_scriptiones` -> `_scripta_numerare`, `_sectio_crud` ->
  `_sectio_cyclus`. The first mode plant was a tautology clang refused
  to compile; rewritten as a never-set mode bit -> red.
- **R31 (T11, live, 2026-10-19)** - certbot needs Fran's email on a
  terminal the tool does not have: `praeparare_hospitem.sh` gained
  `-sine-certbot` (prints the command; default and committed dry-run
  fixture unchanged); Fran ran certbot himself (Let's Encrypt, valid to
  2027-01-17, http -> 301 https). Setup ran once with `-agere`: DNS
  `CREATUM`, a second `assecurare` `IDEM`, the record resolved at once
  on ns1/1.1.1.1/8.8.8.8. The first deploy attempt stopped at its local
  build step (OrbStack not running) - nothing remote ran; the retry
  deployed, `/salus` green on the droplet. Live measurements: `/salus`
  155-180 ms round trip (TLS to NYC), unknown-user login 250-350 ms ->
  ~90-180 ms of argon2id on the single CPU (target ~150 ms met, no
  parameter change); RSS ~2.2 MB idle; `systemctl restart` -> SIGTERM,
  `sistere molliter`, `clausum`, deactivated cleanly, back in < 1 s.
  Fetched `/salus` through the house client (TLS 1.3 default AND
  `RHUBARB_TLS=bearssl` 1.2) and curl. Fran's phone check: all good.

## Review focus

Inputs no task's spec-derived tests would otherwise reach, most likely to
bite first:

1. **Client vanishes mid-upload or while its login is hashing:** temp
   files deleted, the argon2id job's result discarded, the slot safe to
   reuse (generation guard), no leak.
2. **Real browser multipart quirks:** `filename*=UTF-8''...`, a part with
   no Content-Type, quoted names containing `;` or `"`, data that
   contains a near-boundary (`--bound` + other bytes), preamble and
   epilogue text.
3. **A burst of logins:** more than 2 concurrent hashes queue, memory
   stays capped, other requests keep being served meanwhile.
4. **A large download while others are served:** the body source is
   pulled per writable event, the loop never blocks, a second client's
   small request completes during the download.
5. **SIGTERM during an upload or a pending login:** either finishes
   within the grace period or is cut cleanly (temp file removed), the
   volumen closes consistent, next start replays it.

Pinned by tests: 1 -> T2 `generatio` + T3 `abruptum`; 2 -> T3
multipartita sections `navigator`; 3 -> T7 `concurrentia` + T8 `turba`;
4 -> T2 `fons_par`; 5 -> T2 `molliter` + T3 `molliter_corpus`.

---

### Task 1: reactor - cooperative jobs and signals

**Files:** Modify `include/reactor.h`, `lib/reactor.c`,
`lib/reactor.worklog.md`; Create `probationes/probatio_reactor_labores.c`.

**Interfaces - Produces:**
```c
#define REACTOR_LABOR_MAXIMA  LXIV
nomen b32 (*ReactorLabor)(vacuum* datum);   /* VERUM = finitum, tollitur */
b32 reactor_laborem_adicere (Reactor* reactor, ReactorLabor labor,
    vacuum* datum);                          /* FALSUM si tabula plena */
i32 reactor_numerus_laborum (Reactor* reactor);
/* signum ut eventus ansae (fistula sui): tractator in ansa vocatur, numquam
 * in contextu signi. Reactor UNUS per processum signa possidet. */
b32 reactor_signum_adicere (Reactor* reactor, integer signum,
    vacuum (*tractator)(vacuum* datum), vacuum* datum);
```

- [ ] **Step 1: Failing test** (credo v2) sections:
  - `gradatim`: a job counting to 5 runs exactly one step per
    `reactor_poll` and is removed after returning VERUM;
    `reactor_numerus_laborum` 1 -> 0.
  - `sine_mora`: with a job pending and no fd ready, `reactor_poll(r,
    1000)` returns in < 50 ms; without a job and with one idle fd it
    waits ~100 ms when called with 100.
  - `alternatio`: a pipe whose write end is written during step 2 of a
    10-step job -> its read callback fires before the job finishes
    (I/O interleaves).
  - `plena`: the 65th `reactor_laborem_adicere` returns FALSUM.
  - `in_labore`: a job that adds another job -> the new one runs from the
    NEXT cycle, not in the same pass.
  - `signum`: register SIGUSR1; `raise(SIGUSR1)`; the tractator has NOT
    run before `reactor_poll`; after one poll it ran once; two raises
    before a poll -> it runs at least once and at most twice (coalescing
    allowed, loss of the event not).
  - `signum_solum`: a reactor with no fds and no timers but a signal
    registration -> `reactor_poll` blocks (does not return 0 at once) and
    returns when the signal arrives (raise from a timer callback is not
    possible here - use `alarm(1)` with SIGALRM registered).
  - `signum_alter`: a second reactor registering a signal -> FALSUM.
- [ ] **Step 2: Run - expect FAIL** (undeclared functions).
- [ ] **Step 3: Implement.**
  - `ReactorLaborEntry {ReactorLabor labor; vacuum* datum; b32 activus;}
    labores[REACTOR_LABOR_MAXIMA]; i32 labor_numerus;`.
  - `_calculare_timer_timeout`: any active job -> 0.
  - The early `active_count == 0 && timers == 0 -> return 0` check also
    counts jobs and the signal pipe.
  - After `_processare_timers`, `_processare_labores`: iterate a snapshot
    count (jobs added during the pass run next cycle); call; VERUM ->
    inactive.
  - Signals: `hic_manens integer _fistula[II] = {-1, -1}; hic_manens
    Reactor* _possessor;` `_signum_tractare(int s)`: `unsigned char c =
    (unsigned char)s; (vacuum)write(_fistula[I], &c, 1);` (write end
    non-blocking; errno saved and restored). `reactor_signum_adicere`:
    first call creates the pipe (both ends O_NONBLOCK, FD_CLOEXEC),
    registers the read end as a normal fd with an internal callback that
    drains bytes and dispatches to the table `{signum, tractator, datum}`;
    installs `sigaction` with `SA_RESTART`. Another reactor -> FALSUM.
- [ ] **Step 4: Run - expect PASS**; `probatio_reactor` (existing) green.
- [ ] **Step 5: Plants:** job timeout not forced to 0 -> `sine_mora` red;
  snapshot count dropped (jobs added in-pass run at once) -> `in_labore`
  red; pipe write removed from the handler -> `signum` red.
- [ ] **Step 6: Commit** `reactor: labores cooperativi et signa per fistulam (hospitium v3 T1)`.

---

### Task 2: hospitium - deferred response, generation, shared reactor, body sources, graceful stop

**Files:** Modify `include/hospitium.h`, `lib/hospitium.c`,
`lib/hospitium.worklog.md`; Create `probationes/probatio_hospitium_dilatum.c`.

**Interfaces - Consumes:** T1. **Produces:**
```c
/* HospitiumConfiguratio gains: */
              Reactor* reactor;                /* NIHIL = proprius */
          MoraAngusta  responsum_maximum_ms;   /* 0 = 30000 (dilata) */
Reactor* hospitium_reactor (Hospitium*);
vacuum colloquium_differre (HospitiumColloquium*);
b32    colloquium_valet    (HospitiumColloquium*);
nomen structura {
               i64 mensura;
    s32 (*legere)(vacuum* datum, i8* capsa, i32 n);  /* >0, 0 finis, <0 error */
    vacuum (*claudere)(vacuum* datum);
           vacuum* datum;
} HospitiumFons;
vacuum colloquium_respondere_fontem (HospitiumColloquium*, i32 status,
    constans character* mimen, HospitiumFons fons);
vacuum colloquium_respondere_filum (HospitiumColloquium*, i32 status,
    constans character* mimen, constans character* via);
vacuum hospitium_sistere_molliter (Hospitium*, i32 mora_ms);
b32    hospitium_sistit (constans Hospitium*);  /* VERUM post finem */
```

- [ ] **Step 1: Failing test** sections (in-process client with 2 s
  SO_RCVTIMEO + manual `hospitium_gressus` pumping, as probatio_hospitium):
  - `dilatum`: handler defers and adds a 3-step reactor job that answers
    `200 "tardum"` -> the client receives it; meanwhile a second client's
    GET to another route is answered first (interleaving).
  - `expiratum`: `responsum_maximum_ms = 200`, handler defers and never
    answers -> 503 + close within ~1.2 s (vigilia granularity);
    afterwards `colloquium_valet` FALSUM and a late
    `colloquium_respondere` writes nothing (no crash).
  - `generatio` (Review focus 1): handler defers; client closes; server
    notices EOF on the next read pass ... the deferred connection is not
    reading, so closure is detected when the deadline fires or on write;
    a NEW client takes the slot; the old colloquium answers -> nothing
    reaches the new client (its own request gets its own answer).
  - `reactor_communis`: the app creates the reactor, passes it in the
    config, adds a timer and a job; `hospitium_gressus` drives all three.
  - `filum_magnum`: 6 MB file via `colloquium_respondere_filum` -> the
    client reads exactly the bytes; the connection piscina never grows
    past 256 KB (assert via `piscina_summa_allocata` or the equivalent
    house accessor - check `piscina.h` in step 3 and use what exists).
  - `fons_par` (Review focus 4): during a 6 MB download that the client
    reads slowly, a second client's small GET is answered.
  - `directorium_magnum`: directory serving of the 6 MB file now
    streams (same assertions).
  - `molliter` (Review focus 5): a deferred request in flight +
    `hospitium_sistere_molliter(h, 2000)` -> the in-flight request still
    gets its answer; a new connect is refused; `hospitium_sistit` VERUM
    after drain.
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement.**
  - `ConnexioHospitii` gains `i32 generatio` (incremented in
    `_connexionem_claudere`), status `CONNEXIO_EXSPECTANS`, `f64
    terminus_responsi`, `HospitiumColloquium* colloquium`, and a body
    source state (`HospitiumFons fons; b32 fons_activus; i8*
    capsa_fontis` 16 KB in the connection piscina).
  - `_tractatorem_vocare`: allocate the colloquium in the connection
    piscina; record `generatio`. After the handler: if deferred and not
    answered -> status EXSPECTANS, `reactor_modificare(fd, 0)` (no
    interest), `terminus_responsi = nunc + responsum_maximum`.
  - `colloquium_respondere*`: if `!conn->activa || colloquium->generatio
    != conn->generatio` -> return (no-op). Otherwise as today.
  - `_vigilia_callback`: EXSPECTANS past its deadline -> 503 (simple) +
    close-after.
  - Body source: serialize headers with the source's `mensura` as
    Content-Length; write tail = headers; when the tail drains and a
    source is active, `legere` into `capsa_fontis` and continue;
    `legere` 0 -> `claudere`, finish as before; <0 -> close connection.
    HEAD: headers only, `claudere` at once.
  - File source: `filum_lector`-style chunked read via filum (no raw
    fopen - check `filum.h` for a binary chunk reader; if none, add
    `filum_legere_partem(via, offset, capsa, n)` to filum in this task
    with its own test section).
  - `_filum_regulare_servire` switches to the file source.
  - Shared reactor: `hospitium_creare` uses `cfg.reactor` when given
    (then `hospitium_destruere` does NOT destroy it).
  - `hospitium_sistere_molliter`: remove + close the listening socket,
    set `molliter_terminus`; `hospitium_gressus` returns FALSUM once no
    connection is EXSPECTANS/SCRIBENS or the deadline passed (then all
    closed).
- [ ] **Step 4: Run - expect PASS**; `probatio_hospitium` (v1) and
  `probatio_vitrea_hospes` green.
- [ ] **Step 5: Plants:** generation check removed -> `generatio` red;
  deadline branch for EXSPECTANS removed -> `expiratum` red; source pull
  replaced by one big read -> `filum_magnum` red (piscina limit);
  listening socket left open in molliter -> `molliter` red.
- [ ] **Step 6: Commit** `hospitium: responsum dilatum cum generatione, reactor communis, fontes corporis, sistere molliter (hospitium v3 T2)`.

---

### Task 3: streamed request bodies + `multipartita`

**Files:** Create `include/multipartita.h`, `lib/multipartita.c`,
`probationes/probatio_multipartita.c`,
`probationes/probatio_hospitium_corpus.c`; Modify `include/http.h`,
`lib/http.c` (capita_solum hook), `include/hospitium.h`,
`lib/hospitium.c`.

**Interfaces - Produces:**
```c
/* http.h */
vacuum http_parser_capita_solum (HttpParser* parser, b32 modus);
/* status HTTP_PARSE_CAPITA_COMPLETA; then: */
chorda http_parser_corpus_initium (HttpParser* parser);  /* octeti iam lecti */

/* multipartita.h - parser incrementalis multipart/form-data (RFC 7578) */
nomen structura {
    i32 partes_maximae;          /* 0 = LXIV */
    i32 caput_partis_maximum;    /* 0 = 8 KB */
    i32 campus_maximus;          /* 0 = 64 KB (non-fila) */
} MultipartitaLimites;
nomen structura {
    b32 (*initium)(vacuum* d, chorda nomen_campi, chorda titulus_fili,
                   b32 est_filum, chorda mimen);
    b32 (*data)(vacuum* d, constans i8* o, i32 n);
    b32 (*finis)(vacuum* d);
    vacuum* datum;
} MultipartitaReditus;
nomen structura MultipartitaParser MultipartitaParser;
chorda multipartita_limes_ex_typo (chorda content_type, Piscina*); /* boundary; vacua = mala */
MultipartitaParser* multipartita_creare (chorda limes,
    constans MultipartitaLimites*, MultipartitaReditus, Piscina*);
b32 multipartita_adicere (MultipartitaParser*, constans i8* o, i32 n);
b32 multipartita_finire  (MultipartitaParser*);   /* VERUM = clausum per --limes-- */
chorda multipartita_error (MultipartitaParser*);

/* hospitium.h - config */
i32 corpus_in_memoria_maximum;    /* 0 = 1 MB */
constans character* directorium_temporarium;  /* NIHIL = $TMPDIR aut /tmp */
nomen structura {
    chorda campus; chorda titulus_fili; chorda mimen;
    constans character* via; i64 mensura;
    character sigillum_hex[SIGILLUM_HEX_MENSURA];
} HospitiumFilum;
i32 colloquium_fila (HospitiumColloquium*, constans HospitiumFilum** fila);
b32 colloquium_filum_retinere (HospitiumColloquium*,
    constans HospitiumFilum* filum, constans character* via_nova);
/* HttpPetitioServeri gains: constans character* corpus_via; (NIHIL = in memoria) */
```

- [ ] **Step 1: Failing tests.**
  - `probatio_multipartita.c`:
    - `simplex`: two fields + one file part -> callbacks in order, bytes
      exact.
    - `secta` (the fragmenting fixture): the same body fed split at EVERY
      byte position (2 pieces) and one byte at a time -> identical
      callback transcript.
    - `custodia_regionis`: guard-region sweep (mmap + munmap upper half,
      each prefix flush against it) -> no over-read (CREDO_NON_RUIT), as
      tls13 R11.
    - `navigator` (Review focus 2): `filename*=UTF-8''%E2%82%AC.txt`;
      part without Content-Type (-> text/plain default); quoted name with
      `;` and escaped `"`; data containing `\r\n--limesX` (not a
      boundary); preamble and epilogue; LF-only line ends refused
      (RFC 7578 requires CRLF) with a named error.
    - `limites`: 65 parts, oversized part header, oversized non-file
      field -> FALSUM + named error; missing final `--` -> `finire`
      FALSUM.
    - `limes`: `multipartita_limes_ex_typo` for quoted, unquoted, missing,
      over-70-chars boundaries.
  - `probatio_hospitium_corpus.c`:
    - `multipart_magnum`: 10 MB file part via the in-process client ->
      `colloquium_fila` lists it with the right size and SHA-256; the
      connection piscina stays < 256 KB.
    - `corpus_in_filo`: a 2 MB `application/octet-stream` body with a 1 MB
      memory limit -> `corpus_via` non-NIHIL, file content equal.
    - `nimis_magnum`: Content-Length above `petitio_maxima` -> 413 before
      the body is read.
    - `temporaria_deleta`: after the request the temp files are gone;
      `colloquium_filum_retinere` -> file moved and kept.
    - `abruptum` (Review focus 1): client sends half the body and closes
      -> temp file removed, slot freed.
    - `molliter_corpus` (Review focus 5): upload in progress +
      `hospitium_sistere_molliter(h, 300)` and the client stops sending
      -> after the deadline temp file removed, server stopped.
    - `curl`: `curl -s -F campus=valor -F filum=@<6 MB file>
      http://127.0.0.1:<port>/upload` (child via processus; curl from
      `/usr/bin/curl`, section skipped with a named note if absent) ->
      same field and file hash as the C client.
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement.**
  - http.c: `capita_solum` flag; at the empty line with `content_length
    > 0` and the flag set -> `HTTP_PARSE_CAPITA_COMPLETA`, `res.completa`
    FALSE, new `res.capita_completa` VERUM; `http_parser_corpus_initium`
    returns `buffer[cursor..buffer_mensura)`.
  - hospitium: parsers created with `capita_solum`; on
    CAPITA_COMPLETA: Content-Length > `petitio_maxima` -> 413 + close;
    else choose a sink:
    - `multipart/form-data` -> multipartita with callbacks: fields
      accumulate in the request piscina; file parts open a temp file in
      `directorium_temporarium` (name from `moneta_nanoid`), write via
      filum, hash with `sigillum_addere`;
    - else body <= memory limit -> memory (as today);
    - else temp file -> `corpus_via`.
    Body bytes (initial chunk, then every read) go to the sink until
    Content-Length is consumed; bytes beyond = pipelined remainder ->
    keep-alive off (as today). Then dispatch the handler.
  - Request end (`_paratus_novae` and `_connexionem_claudere`): delete
    every temp file not retained.
  - multipartita: a small state machine (PRAEAMBULUM, CAPUT_PARTIS,
    DATA, POST_LIMITEM, FINIS) with a boundary matcher that keeps up to
    `limes + 4` bytes of lookbehind so a split anywhere is handled;
    part headers parsed for `Content-Disposition` (`name`, `filename`,
    `filename*` RFC 5987 decoding) and `Content-Type`.
- [ ] **Step 4: Run - expect PASS**; T2 suite and v1 suite green.
- [ ] **Step 5: Plants:** lookbehind shortened by one byte -> `secta` red;
  temp deletion on close removed -> `abruptum` red; 413 check moved after
  reading -> `nimis_magnum` red; `filename*` decoding skipped ->
  `navigator` red.
- [ ] **Step 6: Commit** `multipartita + hospitium corpora per rivum (fila temporaria, sigillum in itinere) (hospitium v3 T3)`.

---

### Task 4: request helpers + `crustula`

**Files:** Create `include/crustula.h`, `lib/crustula.c`,
`probationes/probatio_crustula.c`,
`probationes/probatio_hospitium_auxilia.c`; Modify `include/hospitium.h`,
`lib/hospitium.c`.

**Interfaces - Produces:**
```c
/* crustula.h - RFC 6265 */
nomen enumeratio { CRUSTULUM_EADEM_LAXA = 0, CRUSTULUM_EADEM_STRICTA,
                   CRUSTULUM_EADEM_NULLA } CrustulumEadem;
nomen structura {
    s64 max_aetas_s;                 /* <0 = sine Max-Age (sessionis); 0 = delere */
    constans character* via;         /* NIHIL = "/" */
    constans character* dominium;    /* NIHIL = sine Domain */
    b32 http_solum; b32 securum; CrustulumEadem eadem;
} CrustulumOptiones;
chorda crustulum_valor (chorda caput_cookie, constans character* nomen);
chorda crustulum_scribere (constans character* nomen, chorda valor,
    constans CrustulumOptiones* o, Piscina*);   /* vacua = recusatum */
/* hospitium.h */
chorda colloquium_quaestio (HospitiumColloquium*, constans character* clavis);
UrlParams colloquium_quaestiones (HospitiumColloquium*);
chorda colloquium_caput (HospitiumColloquium*, constans character* titulus);
chorda colloquium_forma (HospitiumColloquium*, constans character* campus);
chorda colloquium_crustulum (HospitiumColloquium*, constans character* nomen);
b32 colloquium_crustulum_ponere (HospitiumColloquium*, constans character* nomen,
    chorda valor, constans CrustulumOptiones* o);  /* NIHIL = defaltae tutae */
chorda colloquium_cliens (HospitiumColloquium*);
chorda colloquium_schema (HospitiumColloquium*);
vacuum colloquium_redirigere (HospitiumColloquium*, i32 status,
    constans character* locus);
/* config */ b32 procurator_fidus;
```

- [ ] **Step 1: Failing tests.**
  - `probatio_crustula.c`: RFC 6265 §4.2 `Cookie` parsing (multiple
    pairs, spaces, a quoted value, a missing name); `crustulum_scribere`
    attribute order and formatting; invalid name chars (space, `;`,
    `=`), invalid value chars (`,`, `;`, control) -> vacua;
    SameSite=None without Secure -> vacua; Max-Age 0 for deletion.
  - `probatio_hospitium_auxilia.c`: query with `+`, `%20`, repeated keys
    (first wins in `colloquium_quaestio`, all in `quaestiones`); header
    lookup is case-insensitive; urlencoded form; multipart fields via
    `colloquium_forma` (uses T3); cookies set two at once (two
    Set-Cookie lines) and read back; defaults (HttpOnly, SameSite=Lax,
    Path=/, Secure only when schema https); `procurator_fidus` FALSE ->
    `X-Forwarded-For` ignored, `colloquium_cliens` = 127.0.0.1;
    TRUE -> last hop of `1.2.3.4, 5.6.7.8` = 5.6.7.8, schema from
    `X-Forwarded-Proto: https`; redirect 303 with Location; curl cookie
    jar round trip (`-c jar -b jar`, two requests, the second sends the
    cookie the first set); response-header cap 32 (33rd dropped, named
    in the access log).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** (lazy caches in the colloquium; urlencoded
  form via `url_params_parse`; `colloquium_cliens` from the accepted
  socket's peer via `tcp_obtinere_peer_address` unless trusted XFF).
- [ ] **Step 4: Run - expect PASS** (T2, T3, v1 green).
- [ ] **Step 5: Plants:** case-sensitive header compare -> red; XFF used
  without `procurator_fidus` -> red; value validation removed -> red.
- [ ] **Step 6: Commit** `crustula + auxilia petitionis hospitii (quaestio, capita, forma, crustula, procurator) (hospitium v3 T4)`.

---

### Task 5: `blake2b` + `argon2id`

**Files:** Create `include/blake2b.h`, `lib/blake2b.c`,
`include/argon2id.h`, `lib/argon2id.c`, `probationes/probatio_blake2b.c`,
`probationes/probatio_argon2id.c`,
`probationes/fixa/blake2/rfc7693.txt`, `probationes/fixa/argon2/rfc9106.txt`
(fetched unchanged from rfc-editor.org, sha256 recorded in the tests).

**Interfaces - Produces:**
```c
/* blake2b.h */
nomen structura { ... } Blake2b;
b32  blake2b_incipere (Blake2b*, i32 exitus_n, constans i8* clavis, i32 clavis_n);
vacuum blake2b_addere (Blake2b*, constans i8* o, i32 n);
vacuum blake2b_finire (Blake2b*, i8* exitus);
vacuum blake2b (i8* exitus, i32 exitus_n, constans i8* o, i32 n,
    constans i8* clavis, i32 clavis_n);
/* argon2id.h */
nomen structura { i32 t_cost; i32 m_cost_kib; i32 lanae; i32 longitudo;
    constans i8* secretum; i32 secretum_n; constans i8* ad; i32 ad_n; } Argon2idParametri;
nomen structura Argon2idOpus Argon2idOpus;
Argon2idOpus* argon2id_incipere (constans Argon2idParametri*,
    constans i8* tessera, i32 tn, constans i8* sal, i32 sn, Piscina*);
b32  argon2id_gradus (Argon2idOpus*, i32 caudices_maximi);   /* VERUM = finitum */
vacuum argon2id_digestum (Argon2idOpus*, i8* exitus);        /* longitudo octeti */
b32  argon2id_computare (constans Argon2idParametri*, constans i8* tessera,
    i32 tn, constans i8* sal, i32 sn, i8* exitus, Piscina*);  /* uno ictu */
chorda argon2id_phc_scribere (constans Argon2idParametri*, constans i8* sal,
    i32 sn, constans i8* digestum, Piscina*);
b32  argon2id_phc_legere (chorda phc, Argon2idParametri* p, i8* sal, i32* sn,
    i8* digestum, i32* dn);
```

- [ ] **Step 1: Failing tests.**
  - blake2b: RFC 7693 Appendix A (`"abc"`, 64-byte digest) and the
    Appendix E self-test (the RFC's own C procedure re-expressed in the
    test: input lengths {0,3,128,129,255,1024} x output lengths
    {20,32,48,64}, unkeyed and keyed, fed into one outer BLAKE2b-256 ->
    the grand hash printed in the RFC), both extracted from the
    committed RFC text; streaming = one-shot for every split point of a
    300-byte input.
  - argon2id: RFC 9106 §5.3 vector (password 32 x 0x01, salt 16 x 0x02,
    secret 8 x 0x03, ad 12 x 0x04, m = 32 KiB, t = 3, p = 4, tag 32
    bytes) byte for byte from the committed text; `argon2id_gradus` with
    slice sizes {1, 7, 64, unlimited} -> identical tag; PHC round trip
    (scribere then legere returns the same parameters, salt, digest;
    malformed strings refused: wrong algorithm, v != 19, missing fields,
    bad base64); timing on the Mac for m = 19 MiB, t = 2, p = 1 printed
    and asserted < 1 s (pathology guard, not a benchmark).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** RFC 7693 (G function, 12 rounds, 2b offsets)
  and RFC 9106: H0 from parameters, H' variable-length hash, block fill
  with the data-independent addressing for the first half of pass 0 and
  data-dependent afterwards (argon2id), lane reference rules, final XOR
  of last column; lanes computed sequentially; the opus keeps (pass,
  slice, lane, index) so `gradus` resumes at any block; memory = one
  piscina allocation of m_cost_kib x 1 KiB.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** one BLAKE2b rotation constant changed ->
  Appendix A red; addressing switch to data-dependent one slice early ->
  RFC 9106 vector red; resume index off by one -> slice-size equality
  red.
- [ ] **Step 6: Commit** `blake2b + argon2id proprii (RFC 7693, RFC 9106 octetim), opus gradatim (hospitium v3 T5)`.

---

### Task 6: scrinium/volumen - durable option + streamed blobs

**Files:** Modify `include/scrinium.h`, `lib/scrinium.c`,
`include/volumen.h`, `lib/volumen.c`, `lib/volumen.worklog.md`; Create
`probationes/probatio_volumen_rivi.c`.

**Interfaces - Produces:**
```c
/* scrinium.h */
nomen structura { b32 synchronus_plenus; } ScriniumOptiones;
Scrinium* scrinium_aperire_cum (Piscina*, constans character* via,
    constans ScriniumOptiones*);
/* volumen.h */
nomen structura { b32 durabilitas_plena; } VolumenOptiones;
Volumen* volumen_aperire_aut_creare_cum (Piscina*, constans character* via,
    constans VolumenOptiones*);
/* massa ex filo: sigillum_hex notum (in itinere computatum); si iam est,
 * nihil legitur (dedup); aliter zeroblob + rivus + verificatio sigilli */
b32 volumen_massam_condere_ex_filo (Volumen*, constans character* via_fili,
    i64 mensura, constans character* sigillum_hex);
nomen structura VolumenMassaLector VolumenMassaLector;
VolumenMassaLector* volumen_massam_aperire (Volumen*, chorda sigillum_hex,
    i64* mensura, Piscina*);
s32  volumen_massam_legere (VolumenMassaLector*, i8* capsa, i32 n);
vacuum volumen_massam_claudere (VolumenMassaLector*);
```

- [ ] **Step 1: Failing test** sections: `plenus` (`PRAGMA synchronous`
  reads 2 with the option, 1 without); `rivus` (3 MB random file in ->
  read back in 7 KB chunks equal); `dedup` (same hash twice -> one row,
  second call does not read the file - verify by deleting the file
  before the second call); `sigillum_falsum` (wrong hex -> FALSUM,
  rollback, no row); `vacua` (0-byte file); existing volumen and
  scrinium suites green.
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** via `sqlite3_blob_open(db, "main", "massae",
  "contentum", rowid, flags, &blob)`, `sqlite3_blob_write/read` in 64 KB
  chunks; insert `zeroblob(mensura)` in a transaction, stream, re-hash
  with sigillum, compare, commit or roll back; file read through filum
  (chunk reader from T2).
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** hash verification skipped -> `sigillum_falsum`
  red; pragma not applied -> `plenus` red.
- [ ] **Step 6: Commit** `scrinium/volumen: durabilitas plena optionalis, massae per rivum (hospitium v3 T6)`.

---

### Task 7: `custodia` - passwords, sessions, CSRF, throttling

**Files:** Create `include/custodia.h`, `lib/custodia.c`,
`probationes/probatio_custodia.c`.

**Interfaces - Consumes:** T1 (jobs), T5 (argon2id), sigillum, moneta.
**Produces:**
```c
nomen structura {
    i8 secretum[XXXII];
    Argon2idParametri tessera_parametri;   /* m 19456, t 2, p 1, longitudo 32 */
    i32 opera_concurrentia;                /* 0 = II */
    i32 caudices_per_gradum;               /* 0 = XXXII */
    i32 frenum_conatus;                    /* 0 = V */
    i32 frenum_fenestra_s;                 /* 0 = DCCCC (15 min) */
} CustodiaConfiguratio;
nomen structura Custodia Custodia;
Custodia* custodia_creare (constans CustodiaConfiguratio*, Reactor*, Piscina*);
nomen vacuum (*CustodiaHashReditus)(vacuum* d, chorda phc);       /* vacua = error */
nomen vacuum (*CustodiaVerificatioReditus)(vacuum* d, b32 bona, b32 renovanda);
b32 custodia_tesseram_hashare (Custodia*, chorda tessera, CustodiaHashReditus, vacuum*);
b32 custodia_tesseram_verificare (Custodia*, chorda phc_aut_vacua, chorda tessera,
    CustodiaVerificatioReditus, vacuum*);   /* vacua = usor ignotus: hash ficticium */
b32 custodia_tessera_valida (chorda tessera);   /* 8..1024 octeti */
nomen structura { character signum[XLIV]; Sigillum sigillum; } CustodiaSessio;
b32 custodia_sessionem_creare (CustodiaSessio* exitus);  /* moneta; FALSUM si fons deficit */
Sigillum custodia_signi_sigillum (chorda signum);
chorda custodia_csrf (Custodia*, constans Sigillum* sessio, Piscina*);
b32 custodia_csrf_verificare (Custodia*, constans Sigillum* sessio, chorda valor);
b32 custodia_origo_congruit (chorda origo, chorda schema, chorda hospes);
s32 custodia_frenum (Custodia*, chorda clavis, s64 nunc_ms);   /* 0 ok, >0 ms mora, <0 recusa */
vacuum custodia_frenum_notare (Custodia*, chorda clavis, b32 successus, s64 nunc_ms);
b32 custodia_secretum_parare (constans character* via, i8* secretum);  /* legit aut creat 0600 */
```

- [ ] **Step 1: Failing test** sections: `hash_verifica` (hash then verify
  via the reactor; wrong password -> FALSUM; PHC string parses);
  `concurrentia` (Review focus 3: 5 hashes queued -> never more than 2
  running at once - expose `custodia_opera_currentia` for the test -
  all 5 complete; a reactor timer keeps firing meanwhile);
  `ignotus` (verify with vacua phc runs a full hash - steps counted
  equal to a real verify - and returns FALSUM); `renovanda` (a PHC with
  t=1 verifies and reports renovanda VERUM); `sessio` (token 43 chars
  base64url, two tokens differ, hash = SHA-256 of the token string);
  `csrf` (own session ok, other session refused, tampered refused,
  constant-time compare used); `origo` (exact match, port, schema
  mismatch refused, missing Origin refused for unsafe methods);
  `frenum` (5 failures -> delay grows, then refusal; success resets;
  window expiry resets - time injected); `secretum` (first call creates
  0600 file with 32 bytes; second reads the same; wider mode refused).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** (jobs: one reactor job per running hash,
  `argon2id_gradus(opus, caudices_per_gradum)`; a FIFO queue beyond the
  concurrency cap; the dummy hash uses a fixed PHC with production
  parameters; secret file via filum + `filum_modum_ponere`).
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** cap check removed -> `concurrentia` red; dummy
  path returns early -> `ignotus` red; CSRF compare with memcmp of
  different session -> `csrf` red.
- [ ] **Step 6: Commit** `custodia: tesserae argon2id per labores, sessiones, CSRF, origo, frenum (hospitium v3 T7)`.

---

### Task 8: `exemplar` - the proving app

**Files:** Create `apps/exemplar/exemplar.c` (thin `principale`: args,
signals, run), `apps/exemplar/exemplar_nucleus.h`,
`apps/exemplar/exemplar_nucleus.c` (routes, state, pages),
`apps/exemplar/exemplar.sh` (build + run locally), `apps/exemplar/unitas.stml`
if the unit judges require it, `probationes/probatio_exemplar.c`.

**Interfaces - Consumes:** T1-T7. **Produces:**
```c
nomen structura {
    constans character* via_datorum;    /* --data */
    constans character* hospes;         /* --hospes, NIHIL = 127.0.0.1 */
    i32 portus;                         /* --portus */
    constans character* invitatio;      /* --invitatio; NIHIL = clausa */
    b32 procurator_fidus;               /* VERUM in productione */
    Reactor* reactor;                   /* NIHIL = novum */
} ExemplarConfiguratio;
nomen structura Exemplar Exemplar;
Exemplar* exemplar_creare (constans ExemplarConfiguratio*, Piscina*);
Hospitium* exemplar_hospitium (Exemplar*);
vacuum exemplar_claudere (Exemplar*);   /* volumen clausum (checkpoint) */
```
Routes: `GET /` (redirect to /notae or /intrare), `GET|POST /inscribere`
(invite), `GET|POST /intrare`, `POST /exire`, `GET /notae`,
`POST /notae` (multipart: textus + optional filum), `POST /notae/:id/delere`,
`GET /fila/:sigillum` (owner check, attachment, nosniff), `GET /salus`
(JSON `{"status":"ok","acta":N}`). Events as spec II.4; state rebuilt from
`volumen_acta_legere` at start; sessions persisted as events (`sessio.creata`
with the token HASH, never the token).

- [ ] **Step 1: Failing test** (`probatio_exemplar.c`, scratch volumen,
  in-process client): `iter` (register with invite -> login -> add note
  with a 2 MB file -> list shows it -> download equals -> logout ->
  /notae redirects to /intrare); `sine_invitatione` (register without or
  with a wrong code -> 403); `csrf` (POST /notae without token -> 403;
  with another session's token -> 403); `origo` (wrong Origin -> 403);
  `frenum` (6 wrong passwords -> delayed then refused); `alienus` (user
  B cannot download user A's file -> 404, not 403); `restitutio` (close,
  reopen the same volumen -> users, sessions, notes, files intact;
  the session cookie still works); `salus`; `turba` (Review focus 3: 4
  logins at once + a GET /salus -> /salus answered before the logins
  finish).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** (server-rendered HTML with an inline
  `<style>`; every user-provided string HTML-escaped by one helper;
  files stored via `volumen_massam_condere_ex_filo` with the hash from
  T3; downloads via a `HospitiumFons` over `VolumenMassaLector`).
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** HTML escaping off for the note text -> a section
  asserting `<script>` arrives escaped goes red (add it to `iter`); owner
  check removed -> `alienus` red; session hash stored instead as token ->
  a section asserting the volumen never contains the cookie value
  (`acta` scanned) goes red (add as `secreta`).
- [ ] **Step 6: Commit** `exemplar: applicatio probans (inscriptio, sessiones, notae, fila) super hospitium v3 (T8)`.

---

### Task 9: Linux - static musl build + suites under glibc/musl

**Files:** Create `tools/linux/aedificare.sh`; Modify
`tools/linux/Dockerfile.musl` (add `curl` for the differential sections),
`tools/linux/exspectata_*.txt` only with named causes.

**Prerequisite:** OrbStack running (P7).

- [ ] **Step 1: Failing check:** `./tools/linux/probatio.sh
  reactor_labores multipartita crustula blake2b argon2id custodia
  volumen_rivi hospitium_dilatum hospitium_corpus hospitium_auxilia
  exemplar` under glibc and musl - expected to FAIL first (portability
  findings: `SO_NOSIGPIPE`-style guards, `sigaction` visibility under
  `-std=c89`, `MSG_NOSIGNAL`, `mmap` flags, `stat` modes).
- [ ] **Step 2: Fix** each finding at its source (feature macros via
  `postulata_posix.h`, not per-file defines), each fix with a ruling line.
- [ ] **Step 3:** `tools/linux/aedificare.sh apps/exemplar/exemplar.c` ->
  `build/linux/exemplar` (closure from `bin/aedilis --partes`, `gcc
  -static -O2` + house flags, `--platform linux/amd64`); `file` says
  "ELF 64-bit ... x86-64 ... statically linked".
- [ ] **Step 4:** run the static binary inside a plain `alpine` container
  (`--data /tmp/e --portus 3010`) and `curl -fsS
  http://127.0.0.1:3010/salus` inside it -> 200 JSON.
- [ ] **Step 5: Commit** `linux: exemplar static musl amd64, suitae hospitii v3 sub glibc et musl (hospitium v3 T9)`.

---

### Task 10: `oceanus` (DigitalOcean DNS) + host setup + deploy scripts

**Files:** Create `include/oceanus.h`, `lib/oceanus.c`,
`probationes/probatio_oceanus.c`, `tools/oceanus.c` (CLI `bin/oceanus`,
built by aedilis, not installed), `apps/exemplar/exemplar.service`,
`apps/exemplar/exemplar.nginx`, `tools/praeparare_hospitem.sh`,
`tools/dimittere.sh`.

**Interfaces - Produces:**
```c
nomen structura { i64 id; chorda genus; chorda titulus; chorda data; i32 ttl; } OceanusRegistrum;
nomen structura Oceanus Oceanus;
/* via_clavis NIHIL = ~/.rhubarb/digitalocean.clavis; basis NIHIL =
 * https://api.digitalocean.com (probationes: http://127.0.0.1:N) */
Oceanus* oceanus_creare (constans character* via_clavis,
    constans character* basis, Piscina*);
b32 oceanus_registra (Oceanus*, constans character* dominium, Xar* exitus);
b32 oceanus_registrum_creare (Oceanus*, constans character* dominium,
    constans OceanusRegistrum*, i64* id);
b32 oceanus_registrum_mutare (Oceanus*, constans character* dominium, i64 id,
    constans OceanusRegistrum*);
b32 oceanus_registrum_delere (Oceanus*, constans character* dominium, i64 id);
nomen enumeratio { OCEANUS_IDEM = 0, OCEANUS_CREATUM, OCEANUS_MUTATUM,
                   OCEANUS_ERROR } OceanusExitus;
OceanusExitus oceanus_assecurare (Oceanus*, constans character* dominium,
    constans character* genus, constans character* titulus,
    constans character* data, i32 ttl);
constans character* oceanus_error (Oceanus*);   /* numquam clavem continet */
```

- [ ] **Step 1: Failing tests** (`probatio_oceanus.c`; fake DO API =
  hospitium in a forked child, canned JSON shaped from DO's documented
  responses): list with paging (`links.pages.next` followed), create,
  update, delete, `assecurare` (missing -> CREATUM; different data ->
  MUTATUM; equal -> IDEM, no write request sent - the fake counts),
  `Authorization: Bearer` sent; token file mode 0644 -> refused; missing
  prefix `dop_v1_` -> refused; 401 from the fake -> error text without
  the token (assert the token string appears in no output, error, or
  captured log); dry-run outputs of both scripts compared to expected
  command lists (`tools/dimittere.sh -siccum exemplar smaragda`,
  `tools/praeparare_hospitem.sh -siccum exemplar smaragda
  exemplar.golden.earth 165.227.210.190`).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement.**
  - oceanus over `http_exsequi` (blocking - a CLI context) with JSON
    from `json.h`; token read with a tolerated raw read + `fstat` mode
    check, kept in a piscina buffer zeroed at `oceanus_claudere`.
  - `exemplar.service` (spec II.5 hardening, `User=exemplar`,
    `ExecStart=/opt/exemplar/exemplar --data /var/lib/exemplar
    --portus 3010`, `KillSignal=SIGTERM`, `TimeoutStopSec=15`).
  - `exemplar.nginx` (server_name, proxy_pass, the three headers,
    `client_max_body_size 64m`; certbot adds TLS).
  - `praeparare_hospitem.sh <app> <alias> <hostname> <ip>` (`-siccum`
    default prints; `-agere` executes): DNS via `bin/oceanus
    assecurare`; over ssh with sudo: `useradd --system --no-create-home
    --shell /usr/sbin/nologin exemplar` if absent, dirs with owners
    and modes, install unit + `daemon-reload` + `enable`, install site
    into `sites-available` + symlink, `nginx -t` then `systemctl reload
    nginx`, `certbot --nginx -d <hostname>` (interactive the first
    time - Fran types the email).
  - `dimittere.sh <app> <alias>` (`-siccum` default): build (T9) ->
    `rsync` to `/opt/<app>/<app>.new` -> `cp` current to `.prior` ->
    `mv .new` into place -> `systemctl restart <app>` -> `curl -fsS
    http://127.0.0.1:3010/salus` over ssh; failure -> restore `.prior`,
    restart, exit 1 with a named message.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** IDEM check removed (always PUT) -> `assecurare`
  red (write counted); mode check removed -> mode test red; token
  echoed into the error -> the leak assertion red.
- [ ] **Step 6: Commit** `oceanus (DigitalOcean DNS) + praeparare_hospitem + dimittere (hospitium v3 T10)`.

---

### Task 11: live - DNS, first-time setup, deploy, Fran's check

**Prerequisites:** Fran creates the scoped token (`domain:read`,
`domain:create`, `domain:update`) into `~/.rhubarb/digitalocean.clavis`
(0600); OrbStack running; ssh key in the agent.

- [ ] **Step 1:** `bin/oceanus registra golden.earth` (read-only) - show
  the existing records to Fran.
- [ ] **Step 2:** `tools/praeparare_hospitem.sh -siccum ...` -> show ->
  Fran's OK -> `-agere`; certbot run by Fran (email prompt).
- [ ] **Step 3:** `tools/dimittere.sh -siccum exemplar smaragda` -> show
  -> Fran's OK -> `-agere`; `/salus` green over ssh.
- [ ] **Step 4:** from the Mac: `curl -fsS
  https://exemplar.golden.earth/salus` (through our own TLS 1.3 client:
  `RHUBARB_TLS` default) and with curl; register with the invite, log
  in, upload, download, log out; `systemctl restart exemplar` during an
  idle moment -> journal shows the drain; state intact after restart.
  Measure the login round trip with `curl -w '%{time_total}'` (spec
  AUDIENDA: argon2id on the droplet's single CPU, target under ~150 ms
  of hashing); if far above, a ruling lowers `caudices_per_gradum` or m
  within OWASP's alternatives.
- [ ] **Step 5:** Fran's check from his phone.
- [ ] **Step 6:** close-out: spec status IMPLETUM, ledger note on parcum
  ...AAAA (first step toward Forum online), memory; commit
  `hospitium v3: exemplar.golden.earth vivum (Franus probavit)`.
  No merge to main without Fran's go and `fusio_probare`.
