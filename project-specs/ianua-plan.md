# ianua plan - house accounts as general components

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> implementer subagents (a review subagent at the end is allowed). Steps
> use checkbox (`- [ ]`) syntax for tracking.

**Goal:** accounts leave exemplar and become four reusable libraries -
`cives`, `credentialia`, `invitationes`, `ianua` - with per-person
invitations, stable ULIDs, no password hash in any event; exemplar is
migrated in place on smaragda with Fran's and Amanda's logins, notes and
Claude grants intact.

**Architecture:** three pure data libraries, each owning
`CREATE TABLE IF NOT EXISTS` tables in the app's volumen and writing its
own secret-free events, every write inside a transaction the CALLER
holds (`volumen_transactionem_*`). `ianua` composes them, holds the
sessions table and the security plumbing, and mounts the account pages
on hospitium with policy passed in as options and callbacks. exemplar
keeps notes, MCP and `/applicationes`, and calls `ianua` for identity.

**Tech stack:** C89 + latina.h; volumen + scrinium (SQLite, WAL), custodia
(argon2id on the reactor, session tokens, CSRF, Origin, frenum), reactor
(timers), hospitium v3 (routes, deferred replies, cookies, forms),
sigillum (SHA-256, hex), moneta (random bytes), base64url (mcp-remotus
T2), json, filum (`filum_seram_capere` lock), credo v2; Linux via
`tools/linux/` (OrbStack); deploy via `tools/dimittere.sh` to smaragda.

**Spec:** `project-specs/ianua-spec.md` (quarta a4af9c14). Read it with
this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh`; identifiers,
  comments, messages Latin; never a latina.h macro as an identifier
  (`nomen`, `structura`, `magnitudo`, `per`, any Roman numeral
  0-3999...); numerals above 3999 are NOT macros (`IV * MXXIV`); `i32`/
  `i64` UNSIGNED, `s32`/`s64` signed; chorda not NUL-terminated.
- POSIX .c files include `postulata_posix.h` FIRST; no raw
  `fopen/open/stat/access` outside filum (examen code 96).
- **No secret in any event, error, page or log line:** no PHC string, no
  password, no invitation code, no session token (SHA-256 hex of the
  token only, as today). Invitation codes stored as SHA-256 hex only.
- **Every write function runs inside the caller's transaction**
  (`volumen_transactionem_incipere/committere/revolvere`); libraries
  never open their own (P3).
- Handles: 1-32 of `a-z0-9._-` (exemplar's rule), lower-cased on input
  (P13); once used, reserved forever (P4).
- Times: `s64` ms; every library takes `s64 (*nunc_ms)(vacuum)` (NIHIL =
  real clock) so tests inject time, as admissio does.
- Log BEFORE `colloquium_respondere` (the colloquium piscina is recycled
  after); deferred work keeps its own state (slots), never the
  colloquium's piscina.
- Each library: `include/X.h` with `/* <aedilis corpus="lib/X.c"/> */`,
  `lib/X.c`, `lib/X.worklog.md`, `probationes/probatio_X.c` (credo v2:
  `CredoSectio` table + `credo_suitam_currere`); after adding a suite,
  regenerate `./tools/compile_tests_fontes_generare.sh` and commit
  `compile_tests_fontes_generata.sh` WITH the task (T3 of mcp-remotus
  missed it once).
- New words go through `VOCABULA_VIAE_ADDITAE="a.c b.h" ./oratio/vocabula.sh -nova`
  (space-separated) before commit; rename rather than add a word that is
  real English.
- Commits through `silva.commissio` (docs first, gates last); Linux build
  (musl + glibc) before a task is called done.

## Rulings made while planning

- **P1 - first invitation: CLI under the single-writer lock.**
  `include/scrinium.h` states the house law "processus unus scribit".
  exemplar takes `filum_seram_capere("<data>/sera", 0)` at start and
  refuses a second start; `exemplar -data D -invitare "<nota>"` takes the
  same lock and refuses while the server runs ("servus currit - siste
  prius"). Bootstrap = stop, invite, start. The code goes to the
  terminal only - never the journal, never a file. Resolves spec
  AUDIENDA 1. Cost if wrong: a minute of downtime per CLI invitation;
  routine invitations use the web page.
- **P2 - schemas via `CREATE TABLE IF NOT EXISTS`**, no use of
  `scrinium_migrare` (its counter is shared with volumen's own
  migrations - admissio's precedent, lib/admissio.c header). A later
  schema change adds a `<lib>_versio` table then.
- **P3 - caller holds the transaction** for every write (see Global
  constraints). Cost if wrong: a caller who forgets gets per-statement
  commits (row and event not atomic) - tests open transactions
  explicitly so the convention is exercised.
- **P4 - handles reserved forever** through `cives_nomina (nomen_usoris
  PRIMARY KEY, civis)`: creation and rename insert there; delete keeps
  the row; creation/rename refuse a handle already present.
- **P5 - `credentialia_verificare` returns a start code synchronously**
  (`COEPTUM` / `FRENATUM` / `OCCUPATUM` / `PRAVUM`); the callback fires
  only after `COEPTUM`, always later from the reactor - never from
  inside the call.
- **P6 - frenum keys unchanged:** `"par:<ip>"` and `"usor:<handle>"`
  (exemplar :865-869), built inside `credentialia`. Resolves AUDIENDA 4.
- **P7 - note authors:** migration fills `exemplar_nomina_vetera
  (nomen_usoris PRIMARY KEY, civis)` and logs `exemplar.migratio`
  (handle -> ULID map, for audit). Replay maps an old `nota.scripta`
  `usor` through the table; new notes write `civis`. The log is never
  rewritten. Resolves AUDIENDA 3.
- **P8 - admissio re-keying** = `UPDATE admissio_concessiones SET usor
  = ? WHERE usor = ?` and the same on `admissio_codices`; admissio
  `concessio.*` events keep the handle (history). `/applicationes` reads
  only `admissio_concessiones` by `usor`, so it follows. Resolves
  AUDIENDA 2.
- **P9 - HTML escape copied into ianua** (exemplar, admissio, organum,
  stml_html each have one). A shared helper would be a
  `chorda_aedificator.h` edit owing many gates; filed as a ledger note
  instead.
- **P10 - sign-up redirects to sign-in** (no auto sign-in), as today.
- **P11 - no `credentialia_removere`** yet (no consumer); the event name
  `credentiale.remotum` is reserved in the header comment.
- **P12 - sessions live in ianua's table**; old `sessio.*` events are no
  longer replayed - everyone signs in once after the cutover.
- **P13 - handles lower-cased on input** at sign-up and sign-in (Review
  focus 1).
- **P14 - exemplar's `-invitatio` / `-invitatio-via` removed**; the unit
  file loses `-invitatio-via` and is shipped with `dimittere.sh
  -cum-unitate` in Task 6 (an old unit would hit the usage error).

## Rulings made while executing

- **T1: plant swapped.** The planned plant "event written before the
  uniqueness check" cannot go red by design: with the guard removed,
  the UNIQUE / PRIMARY KEY constraints still refuse and the caller's
  rollback (P3) removes the partial row. Replaced by "normalization does
  not lower-case" (`nomina` red). Cost if wrong: none - the guard is a
  fast path, the constraints are the safety, and both are exercised.
- **T3: Linux detour (Fran chose "real Linux body").** After merging main
  (bf073999) every Linux test link failed: credo calls
  `memoria_processus_legere`, which had only a macOS body. Added
  `lib/memoria_processus_posix.c` (/proc/<pid>/status) + a glibc fix in
  its existing suite, as its own commit 045b7159 before T3. Cost if
  wrong: none for ianua; the body reaches main with quarta.
- **T3: empty author/note accepted.** The first draft refused an empty
  chorda (datum NIHIL) for `auctor`/`nota`, which would have refused the
  CLI bootstrap invitation (P1, empty auctor). Normalized to "", pinned
  in section `creare`.
- **T4: password change runs both jobs at once.** The plan had verify
  then hash; that would hold the NEW password in ianua's slot while the
  current one is checked. Now `credentialia_verificare(current)` and
  `credentialia_digerere(new)` start together and the slot joins both
  callbacks; a wrong current password discards the hash. Cost if wrong:
  one wasted hash per failed attempt (bounded by frenum).
- **T4: extra race test.** The planned `inscriptio_atomica` race (same
  handle, two codes) fails at the first write and never exercises
  rollback. Added the same-code / two-handles race, which does; it is
  the one the commit-instead-of-rollback plant turns red.
- **T4: CLI bootstrap test moves to T5** (`exemplar_invitare`, P1) - the
  CLI is an app concern, not ianua's.
- **T5: Claude Code e2e moves to T6.** The local OAuth round trip needs
  a browser; the curl e2e covered CLI invite (stopped / running),
  sign-up, sign-in, notes, /mcp 401. T6 step 4 (Claude still connected
  after the deploy) is the real check of the re-keyed grant.
- **T5: two admissio event assertions changed** from `"usor":"fran"` to
  the person's ULID - grants are keyed by ULID now; events written
  before the migration keep the handle.
- **T3: section names.** `expiratio`/`revocatio` are new words to the
  lint; the sections are `terminus` and `revocare`.

## Review focus

Inputs the spec implies but no test of the spec's list exercises; each
is pinned by a test in the task named.

1. **"Fran" typed with a capital** at sign-in -> signs in as `fran`
   (today: silently "nomen aut tessera falsa"). Test in Task 4
   `nomen_maiusculum`.
2. **An invitation link opened while someone else is signed in on that
   device** -> the page says who is signed in and asks to sign out
   first; the POST refuses (409) instead of quietly creating a second
   account under the same browser session. Task 4 `alius_intratus`.
3. **A used, expired or revoked invitation link** -> the GET page says
   the invitation is not valid and shows NO password form (nobody types
   a password into a dead form). Task 4 `invitatio_mortua`.
4. **Suspended or deleted person with a live session** -> the next
   request is signed out (session refused), and sign-in fails with the
   same timing as a wrong password. Task 2 `status_non_activus` +
   Task 4 `suspensus`.
5. **Migration fails halfway** (a conflicting row) -> nothing changes,
   exemplar refuses to start with a named reason, the next start retries
   cleanly. Task 5 `migratio_fracta`.

---

### Task 1: `cives` - who someone is

**Files:** Create `include/cives.h`, `lib/cives.c`, `lib/cives.worklog.md`,
`probationes/probatio_cives.c`; regenerate `compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** volumen (`volumen_scrinium`,
`volumen_actum_appendere`), scrinium (prepare/bind/step, `scrinium_ulid`),
json. **Produces:**
```c
#define CIVES_NOMEN_MAXIMUM XXXII
#define CIVES_ID_MENSURA    XXVI       /* ULID */

nomen enumeratio {
    CIVIS_ACTIVUS = ZEPHYRUM,
    CIVIS_SUSPENSUS,
    CIVIS_DELETUS
} CivisStatus;

nomen structura {
         chorda id;            /* ULID */
         chorda nomen_usoris;  /* handle praesens */
         chorda titulus;       /* nomen ostensum */
    CivisStatus status;
            s64 creatus_ms;
} Civis;

nomen structura Cives Cives;

/* tabulas creat si desunt (cives, cives_nomina) */
Cives* cives_aperire (Volumen* volumen, s64 (*nunc_ms)(vacuum),
    Piscina* piscina);
/* 1..XXXII signa a-z 0-9 . _ - (minusculae iam) */
b32 cives_nomen_validum (chorda nomen_usoris);
/* minusculae ASCII in piscinam (P13) */
chorda cives_nomen_normare (chorda nomen_usoris, Piscina* piscina);
/* INTRA TRANSACTIONEM VOCATORIS. FALSUM: nomen invalidum aut iam
 * usum (etiam olim - P4). id_exitus in piscina. */
b32 cives_creare (Cives* c, chorda nomen_usoris, chorda titulus,
    chorda* id_exitus, Piscina* piscina);
b32 cives_per_id (Cives* c, chorda id, Civis* exitus, Piscina* p);
/* handle PRAESENS solum */
b32 cives_per_nomen (Cives* c, chorda nomen_usoris, Civis* exitus,
    Piscina* p);
b32 cives_nomen_mutare (Cives* c, chorda id, chorda nomen_novum);
b32 cives_statum_ponere (Cives* c, chorda id, CivisStatus status);
b32 cives_delere (Cives* c, chorda id);   /* status DELETUS */
```
Tables: `cives (id TEXT PRIMARY KEY, nomen_usoris TEXT NOT NULL UNIQUE,
titulus TEXT NOT NULL, status INTEGER NOT NULL, creatus_ms INTEGER NOT
NULL)`, `cives_nomina (nomen_usoris TEXT PRIMARY KEY, civis TEXT NOT
NULL)`. Events: `civis.creatus {id, nomen_usoris, titulus}`,
`civis.renominatus {id, nomen_usoris}`, `civis.status {id, status}`,
`civis.deletus {id}`.

- [ ] **Step 1: Failing test** `probatio_cives.c` sections:
  - `nomina`: `cives_nomen_validum` accepts `fran`, `a`, `a.b_c-9`, a
    32-char handle; refuses ``, 33 chars, `Fran` (normalize first),
    `fr an`, `frän`, `fran/`; `cives_nomen_normare("FrAn")` = `fran`.
  - `creare`: inside a transaction create `fran` (titulus "Fran") ->
    26-char id; `per_id` and `per_nomen` return the same row, status
    ACTIVUS; creating `fran` again -> FALSUM.
  - `renominare`: rename `fran` -> `franciscus`; id unchanged;
    `per_nomen("fran")` FALSUM, `per_nomen("franciscus")` VERUM; a new
    person cannot take `fran` (reserved, P4).
  - `delere`: delete -> status DELETUS; handle still reserved.
  - `acta`: the four event kinds appear with exactly the documented
    keys; no other keys.
  - `restitutio`: close and reopen the volumen **twice** (by path,
    `volumen_aperire`); rows and statuses survive both.
  - `revolutio`: create inside a transaction, roll back -> no row, no
    event, handle free.
- [ ] **Step 2: Run** `./compile_tests.sh cives` - expect FAIL (no
  `cives.h`).
- [ ] **Step 3: Implement** `lib/cives.c`: SCHEMA array as in
  `lib/admissio.c:54-79`; ULID via `scrinium_ulid`; every write = row
  statement(s) + `volumen_actum_appendere` (JSON via `json_scribere`);
  return FALSUM on any step failure (caller rolls back).
- [ ] **Step 4: Run** - expect PASS; `CREDO_SECTIO=revolutio` alone also
  passes.
- [ ] **Step 5: Plants** (`silva.planta`): rename frees the old handle ->
  `renominare` red; event written before the uniqueness check (a
  refused duplicate still logs `civis.creatus`) -> `creare` red; status
  not persisted -> `restitutio` red.
- [ ] **Step 6: Commit** `cives: personae rationum (ULID, nomina reservata, acta sine secretis) (ianua T1)`.

---

### Task 2: `credentialia` - how someone proves it

**Files:** Create `include/credentialia.h`, `lib/credentialia.c`,
`lib/credentialia.worklog.md`, `probationes/probatio_credentialia.c`;
regenerate the sources list.

**Interfaces - Consumes:** T1 (`Cives`, `cives_per_nomen`, `cives_per_id`,
`Civis`, `CivisStatus`), custodia (`custodia_tesseram_digerere`,
`custodia_tesseram_verificare`, `custodia_frenum`,
`custodia_frenum_notare`, `CustodiaHashReditus`), reactor
(`reactor_timer_adicere`). **Produces:**
```c
#define CREDENTIALIA_OPERA LXIV
#define CREDENTIALE_TESSERA "tessera"

nomen structura Credentialia Credentialia;

/* tabula credentialia creatur si deest */
Credentialia* credentialia_aperire (Volumen* volumen, Cives* cives,
    Custodia* custodia, Reactor* reactor, s64 (*nunc_ms)(vacuum),
    Piscina* piscina);

/* hash in labore; reditus cum phc (vacua = error). FALSUM sine
 * reditu: tessera invalida (custodia_tessera_valida), cauda plena. */
b32 credentialia_digerere (Credentialia* cr, chorda tessera,
    CustodiaHashReditus reditus, vacuum* datum);

/* INTRA TRANSACTIONEM VOCATORIS: tessera civis (prior deletur - una
 * per personam); actum credentiale.positum {civis, genus} */
b32 credentialia_tesseram_ponere (Credentialia* cr, chorda civis,
    chorda phc);

nomen enumeratio {
    CREDENTIALIA_COEPTUM = ZEPHYRUM,   /* reditus postea vocabitur */
    CREDENTIALIA_FRENATUM,             /* conatus nimis multi (429) */
    CREDENTIALIA_OCCUPATUM,            /* opera plena (503) */
    CREDENTIALIA_PRAVUM                /* tessera nimis longa (400) */
} CredentialiaInitium;

/* civis valida solum intra reditum; bona FALSUM -> civis vacua */
nomen vacuum (*CredentialiaReditus)(vacuum* datum, b32 bona,
    chorda civis);

/* nomen_usoris iam normatum; clavis_paris = IP clientis. Persona
 * ignota, non activa, aut sine tessera: hash ficticium plenum, bona
 * FALSUM. Mora freni per timer (numquam obstat). Successus cum
 * parametris veteribus: re-hash et tesseram_ponere (transactione
 * propria - hic solum, post responsum). */
CredentialiaInitium credentialia_verificare (Credentialia* cr,
    chorda nomen_usoris, chorda tessera, chorda clavis_paris,
    CredentialiaReditus reditus, vacuum* datum);
```
Table: `credentialia (id TEXT PRIMARY KEY, civis TEXT NOT NULL, genus
TEXT NOT NULL, phc TEXT NOT NULL, creatum_ms INTEGER NOT NULL,
ultimus_usus_ms INTEGER NOT NULL)` + index on `(civis, genus)`. Events:
`credentiale.positum {civis, genus}` (and reserved, unused:
`credentiale.remotum`, P11). Slots: `CREDENTIALIA_OPERA` entries holding
the tessera copy (zeroed on release), handle, both frenum keys, the
looked-up civis id, callback + datum - moved from exemplar's `Opus`
(exemplar_internum.h) and `_intrare_post` / `_verificatio_facta` /
`_renovatio_facta` (exemplar_nucleus.c :698-906).

- [ ] **Step 1: Failing test** `probatio_credentialia.c` (reactor pumped
  with a `_currere` helper like `probatio_custodia.c:77`; custodia with
  CHEAP argon2 parameters for speed, e.g. `m_cost_kib` 64, `t_cost` 1):
  - `ponere_verificare`: create civis `fran`; digerere("tessera-bona")
    -> phc; in a transaction `tesseram_ponere`; verificare(`fran`,
    "tessera-bona") -> COEPTUM, callback bona VERUM with fran's id;
    wrong password -> bona FALSUM, civis empty.
  - `ignotus`: unknown handle -> COEPTUM, bona FALSUM, and
    `custodia_gradus_summa` grows by the SAME amount as a known-handle
    failure (full dummy hash).
  - `status_non_activus` (Review focus 4): suspended and deleted persons
    with the right password -> bona FALSUM, same work done.
  - `frenum`: frenum_conatus 2 on custodia; failures until
    `custodia_frenum` returns a delay -> the call still returns COEPTUM
    and the callback arrives only after the timer (pump count > 0 before
    it); past the refusal threshold -> FRENATUM, no callback ever.
  - `occupatum`: fill all `CREDENTIALIA_OPERA` slots (callbacks not yet
    pumped) -> next call OCCUPATUM; `PRAVUM` for a 1025-byte password.
  - `renovanda`: phc made with OLD parameters (a second custodia with
    different `t_cost` builds it), verify with the current one -> bona,
    and after pumping the stored phc has the new parameters and a second
    `credentiale.positum` event exists.
  - `una_per_personam`: `tesseram_ponere` twice -> one row for the civis;
    the old password no longer verifies.
  - `acta_sine_secretis`: after the sections' operations, scan every
    event's `datum` and every column except `credentialia.phc`: none
    contains `$argon2` or the plaintext password.
- [ ] **Step 2: Run** `./compile_tests.sh credentialia` - expect FAIL.
- [ ] **Step 3: Implement**, moving the slot/deferral logic from
  exemplar (exemplar keeps its copy until Task 5). Lookup order inside
  `verificare`: frenum on both keys -> slot -> `cives_per_nomen` ->
  status check -> phc row (none = empty phc = dummy) ->
  `custodia_tesseram_verificare`. On completion: `custodia_frenum_notare`
  both keys, update `ultimus_usus_ms` on success, callback, then rehash
  if `renovanda` (slot kept until the rehash callback, as exemplar does
  :796-801).
- [ ] **Step 4: Run** - expect PASS.
- [ ] **Step 5: Plants:** phc written into the event ->
  `acta_sine_secretis` red; status check removed ->
  `status_non_activus` red; delay applied synchronously (callback
  inside the call) -> `frenum` red; dummy hash skipped for unknown
  handles -> `ignotus` red.
- [ ] **Step 6: Commit** `credentialia: tesserae per personam, opera dilata et frenum ex exemplare, re-hash (ianua T2)`.

---

### Task 3: `invitationes` - who may join

**Files:** Create `include/invitationes.h`, `lib/invitationes.c`,
`lib/invitationes.worklog.md`, `probationes/probatio_invitationes.c`;
regenerate the sources list.

**Interfaces - Consumes:** T1 (`CIVES_ID_MENSURA` for sizes only),
moneta (random bytes), base64url (`base64url_scribere`), sigillum
(SHA-256 hex), scrinium, volumen. **Produces:**
```c
#define INVITATIONES_CODEX_MENSURA XXII     /* XVI octeti base64url */
#define INVITATIONES_VITA_ORDINARIA ((s64)VII * XXIV * LX * LX * M)

nomen structura {
    chorda id;           /* ULID */
    chorda auctor;       /* civis; vacua = CLI */
    chorda nota;
       s64 creata_ms;
       s64 expirat_ms;
       i32 usus_max;
       i32 usus_numerus;
    chorda usa_a;        /* civis ultimus qui usus est; vacua */
       s64 revocata_ms;  /* 0 = non revocata */
} Invitatio;

nomen structura Invitationes Invitationes;

Invitationes* invitationes_aperire (Volumen* volumen,
    s64 (*nunc_ms)(vacuum), Piscina* piscina);
/* INTRA TRANSACTIONEM VOCATORIS. codex SEMEL redditur (in piscina);
 * vita_ms 0 = ordinaria; usus_max 0 = I */
b32 invitationes_creare (Invitationes* inv, chorda auctor, chorda nota,
    s64 vita_ms, i32 usus_max, chorda* codex_exitus, chorda* id_exitus,
    Piscina* piscina);
/* valida (non expirata, non revocata, usus reliqui) - nihil mutat */
b32 invitationes_inspicere (Invitationes* inv, chorda codex,
    Invitatio* exitus, Piscina* piscina);
/* INTRA TRANSACTIONEM: UPDATE custoditum (usus_numerus < usus_max,
 * expirat_ms > nunc, revocata_ms = 0); FALSUM si nulla linea mutata */
b32 invitationes_consumere (Invitationes* inv, chorda codex,
    chorda civis);
/* INTRA TRANSACTIONEM: auctoris solum; FALSUM si aliena aut iam */
b32 invitationes_revocare (Invitationes* inv, chorda auctor,
    chorda id);
/* auctoris, creatione ordinatae (novissimae primum) */
Xar* invitationes_enumerare (Invitationes* inv, chorda auctor,
    Piscina* piscina);
```
Table: `invitationes (id TEXT PRIMARY KEY, sigillum TEXT NOT NULL
UNIQUE, auctor TEXT NOT NULL, nota TEXT NOT NULL, creata_ms INTEGER NOT
NULL, expirat_ms INTEGER NOT NULL, usus_max INTEGER NOT NULL,
usus_numerus INTEGER NOT NULL DEFAULT 0, usa_a TEXT NOT NULL DEFAULT '',
revocata_ms INTEGER NOT NULL DEFAULT 0)`. Events: `invitatio.creata {id,
auctor, expirat_ms, usus_max}`, `invitatio.usa {id, civis}`,
`invitatio.revocata {id}`.

- [ ] **Step 1: Failing test** `probatio_invitationes.c` (injected clock):
  - `creare`: code is 22 base64url chars; `inspicere` VERUM with the
    nota and auctor; a second code differs.
  - `semel`: `consumere` VERUM then FALSUM; `inspicere` after use FALSUM;
    `usa_a` = the civis; `usus_max` 3 allows three.
  - `expiratio`: clock past `expirat_ms` -> `inspicere` and `consumere`
    FALSUM; one ms before -> VERUM.
  - `revocatio`: another auctor's revoke FALSUM; own revoke VERUM, then
    consume FALSUM, revoke again FALSUM.
  - `certamen`: two `invitationes_aperire` handles on the SAME volumen
    each `inspicere` VERUM, then both `consumere` in turn (each in its
    own transaction) -> exactly one VERUM (the guarded UPDATE, not the
    earlier inspect, decides).
  - `codex_occultus`: scan every column of `invitationes` and every
    event `datum` for the plaintext code -> absent; the code's SHA-256
    hex IS in `sigillum`.
  - `enumerare`: three invitations by fran, one by amanda -> fran's
    list has three, newest first.
- [ ] **Step 2: Run** `./compile_tests.sh invitationes` - expect FAIL.
- [ ] **Step 3: Implement:** 16 random bytes (`moneta`), base64url, SHA-256
  hex as the key; `consumere` = `UPDATE invitationes SET usus_numerus =
  usus_numerus + 1, usa_a = ? WHERE sigillum = ? AND usus_numerus <
  usus_max AND expirat_ms > ? AND revocata_ms = 0` then check
  `changes()` == 1 (via `SELECT changes()`), then the event.
- [ ] **Step 4: Run** - expect PASS.
- [ ] **Step 5: Plants:** guard `usus_numerus < usus_max` dropped ->
  `semel` + `certamen` red; code stored plain -> `codex_occultus` red;
  expiry compared with `>=` the wrong way -> `expiratio` red.
- [ ] **Step 6: Commit** `invitationes: codices per personam (semel, expiratio, revocatio, quis quem), digesta sola (ianua T3)`.

---

### Task 4: `ianua` - the door

**Files:** Create `include/ianua.h`, `lib/ianua.c`, `lib/ianua.worklog.md`,
`probationes/probatio_ianua.c`; regenerate the sources list.

**Interfaces - Consumes:** T1-T3 (all produced names), custodia (sessions,
CSRF, Origin, `custodia_signi_sigillum`), hospitium (`hospitium_praebere`,
`colloquium_*`, `colloquium_differre`, `dilatum_colloquium`,
`colloquium_crustulum_ponere`), admissio (`AdmissioIntratus` signature
only). **Produces:**
```c
#define IANUA_SESSIO_VITA_S 2592000   /* XXX dies */

nomen structura {
    /* "" = radix: /intrare, /inscribere, /exire, /tessera,
     * /invitationes */
    constans character* praefixum;
                    s64 sessio_vita_s;          /* 0 = ordinaria */
    constans character* via_post_intrationem;   /* NIHIL = "/" */
                    b32 invitatio_necessaria;
                    s64 vita_invitationis_ms;   /* 0 = ordinaria */
    /* pagina: corpus HTML (iam evasum) in fabricam applicationis
     * involvit et respondet; NIHIL = pagina simplex ianuae */
    vacuum (*pagina)(vacuum* datum, HospitiumColloquium* col,
        i32 status, constans character* titulus, chorda corpus);
    vacuum (*post_inscriptionem)(vacuum* datum, chorda civis);
    /* NIHIL = quivis civis activus */
    b32 (*potest_invitare)(vacuum* datum, constans Civis* civis);
                vacuum* datum;
                    s64 (*nunc_ms)(vacuum);
} IanuaOptiones;

nomen structura Ianua Ianua;

/* cives, credentialia, invitationes et tabulam sessionum aperit */
Ianua* ianua_creare (Volumen* volumen, Custodia* custodia,
    Reactor* reactor, constans IanuaOptiones* optiones,
    Piscina* piscina);
b32 ianua_montare (Ianua* ianua, Hospitium* hospitium);

Cives*         ianua_cives (Ianua* ianua);
Credentialia*  ianua_credentialia (Ianua* ianua);
Invitationes*  ianua_invitationes (Ianua* ianua);

/* sessio valida et civis ACTIVUS: civis + signum CSRF */
b32 ianua_intratus (Ianua* ianua, HospitiumColloquium* col,
    Civis* civis, chorda* csrf);
/* POST tutum: Origin congruens + intratus + CSRF formae ("csrf");
 * FALSUM = refutatum (vocator 403 respondet) */
b32 ianua_custos_postis (Ianua* ianua, HospitiumColloquium* col,
    Civis* civis);
/* AdmissioIntratus: datum = Ianua*; usor = civis ULID */
b32 ianua_admissionis_intratus (vacuum* ianua,
    HospitiumColloquium* col, chorda* usor, chorda* csrf);
```
Sessions table: `ianua_sessiones (sigillum TEXT PRIMARY KEY, civis TEXT
NOT NULL, creata_ms INTEGER NOT NULL, expirat_ms INTEGER NOT NULL,
ultima_ms INTEGER NOT NULL)`; events `sessio.creata {sigillum, civis,
expirat}` and `sessio.deleta {sigillum}` (hash only). Cookie `sid`, as
exemplar's `_crustulum_ponere` (HttpOnly, Secure on https, SameSite
Lax). Routes and their rules - spec II.4; plumbing copied from exemplar
(`_origo_bona`, `_csrf_bonum`, `_redire_tutum`, `_evadere` (P9), the
`?redire=` hidden field). ianua's own slot pool (`LXIV`) carries the
`HospitiumDilatum`, `redire`, and sign-up fields across the async
callbacks; the password copy lives only in credentialia's slot.

Sign-up transaction (in the `credentialia_digerere` callback):
`volumen_transactionem_incipere` -> `invitationes_consumere` (if
required) -> `cives_creare` -> `credentialia_tesseram_ponere` ->
`committere`; any FALSUM -> `revolvere` + error page; after commit
`post_inscriptionem`, then redirect `/intrare`.

Password change (`/tessera`): `ianua_custos_postis`; verify the CURRENT
password through `credentialia_verificare` (frenum applies, P6 keys);
then `credentialia_digerere(new)`; in its callback one transaction:
`tesseram_ponere` + delete every OTHER session of the civis (+
`sessio.deleta` each).

- [ ] **Step 1: Failing test** `probatio_ianua.c` - a bare hospitium +
  reactor + custodia (cheap argon2) + temporary volumen; HTTP client
  helpers adapted from `probatio_exemplar_applicatio.c:40-430`
  (`_rogare_et_legere`, `_extrahere`, cookie/Origin headers) but taking
  `Hospitium*`. An invitation for each test is made directly with
  `invitationes_creare` inside a transaction. Sections:
  - `inscriptio`: GET `/inscribere?invitatio=C` shows the form with the
    code hidden; POST nomen/titulus/tessera/invitatio -> 303 `/intrare`;
    civis exists, credential row exists, invitation used - all three.
  - `inscriptio_atomica`: two sign-ups with the SAME new handle and two
    different codes, both POSTed before the reactor is pumped (both pass
    the early handle check, both hash) -> one 303, one 409; the loser's
    invitation is still unused and no second credential row exists (its
    transaction rolled back at `cives_creare`). Plus the fast path: a
    handle already taken at POST time -> 409 without hashing.
  - `invitatio_mortua` (Review focus 3): used / expired / revoked /
    unknown code -> GET page "invitatio non valet", NO
    `type="password"` in the body; POST with it -> 403.
  - `alius_intratus` (Review focus 2): with fran's `sid`, GET
    `/inscribere?invitatio=C` names fran and offers sign-out, no form;
    POST -> 409; invitation unused.
  - `intrare_exire`: sign in -> 303 to `via_post_intrationem`, `sid`
    cookie; `ianua_intratus` VERUM; `/exire` with CSRF -> session row
    gone, `sessio.deleta` logged; old `sid` refused.
  - `nomen_maiusculum` (Review focus 1): sign in as `FRAN` -> signed in
    as fran.
  - `redire`: `/intrare?redire=/notae%3Fx%3D1` keeps it through sign-in;
    `//evil`, `/\evil`, `http://evil` -> default landing.
  - `origo_csrf`: POST without Origin / wrong Origin -> 403 on every
    POST route; `/exire`, `/tessera`, `/invitationes` without CSRF ->
    403.
  - `tessera_mutata`: two sessions for fran (two sign-ins); change
    password from session A (current password right) -> A still valid,
    B refused; old password fails, new works; wrong current password ->
    403 and counts toward frenum.
  - `invitationes_pagina`: fran creates one (nota "pro Amanda") -> the
    response shows the code and `/inscribere?invitatio=` link ONCE;
    GET the page again -> the nota and "non usa" but NOT the code;
    after amanda signs up with it -> "usa a amanda"; revoke works for
    fran, refused for amanda (403/404).
  - `suspensus` (Review focus 4): suspend fran via `cives_statum_ponere`
    -> her next request with a live `sid` is not signed in; sign-in
    fails.
  - `admissio_adapter`: `ianua_admissionis_intratus` returns the ULID
    and a CSRF equal to the page's.
  - `sine_secretis`: after all sections' traffic, no event contains the
    `sid` value, a code, `$argon2`, or a password.
- [ ] **Step 2: Run** `./compile_tests.sh ianua` - expect FAIL.
- [ ] **Step 3: Implement** `lib/ianua.c` (routes, pages, slots,
  sessions, the three transactions above). Default `pagina`: minimal
  `<!doctype html>` with the same security headers as exemplar's
  `_capita_tuta`.
- [ ] **Step 4: Run** - expect PASS; run each section alone with
  `CREDO_SECTIO` once (no hidden ordering).
- [ ] **Step 5: Plants:** sign-up commits the civis before consuming the
  invitation outside the transaction -> `inscriptio_atomica` red;
  password change keeps other sessions -> `tessera_mutata` red;
  `_redire_tutum` accepts `//` -> `redire` red; status not checked in
  `ianua_intratus` -> `suspensus` red; GET of a dead invitation renders
  the form -> `invitatio_mortua` red.
- [ ] **Step 6: Linux:** `./tools/linux/probatio.sh -musl` and `-glibc`
  with `cives credentialia invitationes ianua`.
- [ ] **Step 7: Commit** `ianua: ianua rationum in hospitio (inscriptio per invitationem, sessiones, tessera mutata, invitationes, adapter admissionis) (ianua T4)`.

---

### Task 5: exemplar cutover + migration

**Files:** Modify `apps/exemplar/exemplar_nucleus.c` (account code out,
ianua in, replay of notes, migration), `apps/exemplar/exemplar_internum.h`
(`Usor`, `Sessio`, `Opus` removed; `Nota.usor` -> `Nota.civis`;
`Exemplar` gains `Ianua* ianua`, `FilumSera* sera`),
`apps/exemplar/exemplar_nucleus.h` (`invitatio` removed from config;
`exemplar_invitare` added), `apps/exemplar/exemplar.c` (`-invitare`;
`-invitatio*` removed, P14), `apps/exemplar/exemplar_mcp.c` (persona
`usor` is now the civis ULID - check every `usor` use :291, :349, :402,
:424, :479), `apps/exemplar/exemplar.service` (drop `-invitatio-via`),
`probationes/probatio_exemplar_applicatio.c`,
`apps/exemplar/exemplar.worklog.md`.

**Interfaces - Consumes:** T1-T4. **Produces:**
```c
/* exemplar_nucleus.h */
/* CLI: seram data/sera capit (servus currens -> II), volumen aperit,
 * invitationem (auctor vacua) creat; codex in piscina (exemplar.c
 * codicem + nexum <basis>/inscribere?invitatio=... ad stdout scribit,
 * numquam ad stderr/acta). 0 bene | 1 error | 2 servus currit */
s32 exemplar_invitare (constans character* via_datorum,
    constans character* nota, chorda* codex_exitus, Piscina* piscina);

#ifdef EXEMPLAR_PROBATIO
/* uncus probationis: invitatio in exemplare currente */
b32 exemplar_invitationem_probationis (Exemplar* e, chorda* codex,
    Piscina* piscina);
#endif
```
Migration table `exemplar_migrationes (titulus TEXT PRIMARY KEY,
facta_ms INTEGER NOT NULL)` row `cives`; map table
`exemplar_nomina_vetera (nomen_usoris TEXT PRIMARY KEY, civis TEXT NOT
NULL)`; event `exemplar.migratio {cives: {handle: ULID, ...}}`.

Migration, run at the END of `exemplar_creare` (after
`exemplar_mcp_praebere` has created admissio's tables) when the marker
row is absent, in ONE transaction: read all events; latest PHC per
handle from `usor.creatus` / `tessera.mutata` in seq order;
`cives_creare(handle, titulus = handle)`;
`credentialia_tesseram_ponere`; map row; P8 UPDATEs; the event; the
marker. Any FALSUM -> rollback, `exemplar_creare` returns NIHIL with
"exemplar: migratio civium fracta: <reason>" (Review focus 5).

- [ ] **Step 1: Failing tests** in `probatio_exemplar_applicatio.c`:
  - existing sections switch from the shared `INVITATIO` to
    `exemplar_invitationem_probationis` (every test that signs up);
    `sine_invitatione` becomes "no code / dead code -> 403".
  - `migratio`: build a pre-migration volumen by hand
    (`volumen_aperire_aut_creare` + `volumen_actum_appendere` of
    `usor.creatus {usor:"fran", phc}`, `tessera.mutata`, `usor.creatus
    {usor:"amanda", ...}`, `nota.scripta {usor:"fran", ...}` with PHCs
    made by custodia, and admissio rows via
    `admissio_concessionem_creare_probationis` for "fran") ->
    `exemplar_creare` -> fran and amanda sign in with their passwords;
    fran's `/notae` shows her note and not amanda's; `/applicationes`
    lists fran's grant; `admissio_concessiones` by her ULID returns it.
  - `migratio_iterata`: close, `exemplar_creare` again -> no new
    `exemplar.migratio` event, same ULIDs.
  - `migratio_fracta` (Review focus 5): pre-migration volumen plus a
    pre-existing `cives` row taking handle `amanda` -> `exemplar_creare`
    NIHIL; no `fran` civis, no marker; remove the conflicting row ->
    next `exemplar_creare` succeeds.
  - `sera`: a second `exemplar_creare` on the same data dir -> NIHIL;
    `exemplar_invitare` while the first runs -> 2; after
    `exemplar_claudere` -> 0 and a 22-char code that `inspicere`
    accepts.
  - `mcp_nota_civis`: a note added through the MCP tool is stored with
    `civis` = ULID and listed by `notas_enumerare` for that person only.
- [ ] **Step 2: Run** `./compile_tests.sh exemplar_applicatio` - expect
  FAIL.
- [ ] **Step 3: Implement:** remove exemplar's account routes, `Usor`,
  `Sessio`, `Opus`, `_usor*`, `_sessionem_ponere`, `_intratus`,
  `_crustulum_ponere`, the opera functions, and the `usor.`/`tessera.`/
  `sessio.` replay branches; `ianua_creare` + `ianua_montare` with
  `pagina` = exemplar's frame (`_paginam_incipere`/`_paginam_mittere`
  around the body), `via_post_intrationem` "/notae",
  `invitatio_necessaria` VERUM; note routes use `ianua_custos_postis` /
  `ianua_intratus`; `exemplar_intratus_usor` becomes a wrapper over
  `ianua_admissionis_intratus`; "salve, <titulus>"; replay maps
  `nota.scripta` (`civis` if present, else `usor` through
  `exemplar_nomina_vetera`); new notes and `filum.receptum` write
  `civis`. Lock `<data>/sera` taken first in `exemplar_creare`, released
  in `exemplar_claudere`.
- [ ] **Step 4: Run** - expect PASS (all old sections + new ones).
  Then `wc -l apps/exemplar/exemplar_nucleus.c` - record the before/after
  in the worklog (expected: well under 1719).
- [ ] **Step 5: Plants:** migration without the transaction (first
  handle commits alone) -> `migratio_fracta` red; replay ignoring the
  map -> `migratio` red (fran's note missing); lock not taken ->
  `sera` red.
- [ ] **Step 6: Linux:** `./tools/linux/probatio.sh -musl` and `-glibc`
  with `cives credentialia invitationes ianua admissio admissio_fluxus
  exemplar_applicatio`; static build `tools/linux/aedificare.sh
  exemplar`.
- [ ] **Step 7: Local end to end:** start a scratch exemplar
  (`./apps/exemplar/exemplar.sh -data /tmp/ianua-e2e -portus 3010`),
  `exemplar -invitare` refused while running; stop, invite, start; sign
  up in a browser via the link; Claude Code `claude mcp add --transport
  http exemplar-loc http://127.0.0.1:3010/mcp` signs in and lists notes.
- [ ] **Step 8: Commit** `exemplar: rationes per ianuam, migratio civium (ULID, tesserae ex actis in tabulam, concessiones re-clavatae), sera scriptoris unius, -invitare (ianua T5)`.

---

### Task 6: deploy to smaragda + live checks

**Files:** Modify `apps/exemplar/exemplar.worklog.md`,
`project-specs/ianua-spec.md` (status IMPLETUM + commits), this plan
(rulings while executing); ledger notes.

Every remote command shown as a dry run first; executed only on Fran's
OK. Touch only exemplar's own user, directories, unit and nginx site.

- [ ] **Step 1: Backup:** `sudo cp -p /var/lib/exemplar/exemplar.volumen
  /var/lib/exemplar/exemplar.volumen.ante-ianua` (after a WAL
  checkpoint: stop the service first; verify with `sudo ls -l`).
- [ ] **Step 2: Deploy:** `tools/dimittere.sh -agere -cum-unitate
  exemplar smaragda` (new unit without `-invitatio-via`, P14); health
  `/salus` ok; `journalctl -u exemplar` shows no "migratio civium
  fracta".
- [ ] **Step 3: Smoke:** `tools/exemplar_mcp_fumus.sh
  https://exemplar.golden.earth` all BENE; `GET /inscribere` without a
  code -> "invitatio necessaria".
- [ ] **Step 4: Fran checks** (he does these; I watch the journal):
  sign in on the web, notes there; Claude (web/Desktop) still connected
  - list notes works without reconnecting (grant re-keyed); then Fran
  and Amanda change their passwords on `/tessera` (the dead-hash step,
  spec II.5).
- [ ] **Step 5: Invitation live test:** Fran creates one on
  `/invitationes`, opens the link in a private window, sees the form
  (does not need to complete it), revokes it, reloads -> "invitatio non
  valet".
- [ ] **Step 6: Close-out:** spec status IMPLETUM with commits; worklog
  entry; ledger: note on ...MK0 (piece #1 done), note for the shared HTML
  escape (P9); ask Fran before removing the old `/var/lib/exemplar/
  invitatio` file and the `.ante-ianua` backup.
- [ ] **Step 7: Commit** `ianua: vivum in exemplar.golden.earth - migratio facta, tesserae mutatae, invitationes per paginam (ianua T6)`.
