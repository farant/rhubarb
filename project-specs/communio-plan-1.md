# communio phase 1 (local foundation) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** acts keep their identity across volumes: two volumes on one Mac exchange acts in-process (no network yet), a picture drawn in volume A opens read-only in volume B with identical pixels and keeps following A's later strokes and undos.

**Architecture:** `volumen` gains a per-volume replica id, a writer's-own sequence number on every act, an import that keeps a foreign act's identity (with the clock bound), and a separate manifest namespace for foreign plagulae. `historia` learns to open and follow a document of another replica. A small new library `communio` moves acts and blobs between two open volumes - the same export/import the network protocol of phase 2 will carry.

**Tech Stack:** C89 in Latin (latina.h), SQLite via `scrinium`, credo v2 suites, `silva.commissio`.

**Spec:** `project-specs/communio-spec.md` (APPROBATUM 2026-10-26) - D2, D2b, D4, D8 (act log is the truth); modules II.1, II.2 (replica part), II.9. Background: `project-specs/genera-datorum.md` VIII-b/VIII-c.

## Global Constraints

- C89, house flags (-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual ...); Latin identifiers; `i32`/`i64` UNSIGNED; Roman-numeral macros; `nomen` is a macro; chorda not NUL-terminated.
- Volume migrations are append-only (MIGRATIONES_VOLUMINIS: never edit an old one, only add).
- Acts are immutable: an imported act is stored byte-for-byte (genus, datum, momentum) with its identity; never rewritten.
- A document is identified by (replica, spatium). Own documents keep today's API and behaviour unchanged.
- References inside a document's acts (`<cursor ad=N>`, `<ramus ab=N>`) are in the WRITER's numbering (its own seq); they stay valid on every volume.
- Derived data never syncs (spec D2): plagulae whose origo ends in `:checkpoint` are not exported (phase 1 rule until the category registry, genera-datorum D6).
- Future bound for imported acts: physical part of `hora` more than `VOLUMEN_FUTURUM_MAXIMUM_MS` = 600000 (10 minutes) ahead of the local clock is refused.
- No network, no server, no UI changes in this phase.

## Review Focus

1. Two volumes on the SAME machine (standalone pictor + vicus pane) must never be confused: distinct replica ids, no act dropped as a "duplicate" -> T1 test "replicae distinctae", T3 test "eadem machina".
2. A device whose clock runs ahead: within the bound its acts are accepted and the next local act sorts after them; beyond it they are refused and nothing changes -> T2 tests "hora futura".
3. The same batch imported twice, or a batch retried after a partial failure: no duplicates, cursor safe -> T2 "idempotens", T3 "iterum".
4. Undo/redo on the writer's device: the follower shows exactly the writer's state, not the pre-undo strokes -> T5 "revocatio sequitur".
5. A volume made before this phase (no replica, no seq_originis): opens, its acts get this volume's replica, and it exports correctly -> T1 "volumen vetus", T3 uses an upgraded volume.

---

### Task 1: replica identity on every act

**Files:**
- Modify: `lib/volumen.c` (migrations XII-XV, open, append, old-act filling), `include/volumen.h`, `lib/historia.c` (basis copy), `lib/volumen.worklog.md`
- Test: `probationes/probatio_volumen.c` (new sections)

**Interfaces:**
- Produces:
  - `chorda volumen_replica(constans Volumen* volumen);` - this volume's replica id (26-char ULID from `moneta_ulid`), created on first open, stable across reopen; stored in `praesentia` under `volumen/replica` (device-local, never synced).
  - `VolumenIdentitas` gains `chorda replica;` (identity = (hora, replica); `machina` kept for the record).
  - New columns: `acta.replica`, `acta.basis_replica`; unique index on `(hora, replica)` replaces `acta_hora (hora, machina)`.
  - `volumen_actum_identitas` and `volumen_acta_legere` fill `replica` (and `basis.replica`).

- [ ] **Step 1: failing tests** - sections: "replica (communio D2b)": a fresh volume has a 26-char replica; reopen -> same; a second volume on the same machine -> different replica; every appended act's identity carries the volume's replica; `volumen_actum_super_appendere` with a basis records `basis.replica`. "replicae distinctae": two volumes, same machina, acts appended alternately so their `hora` values coincide -> importing is not part of T1, but both volumes keep all their acts and identities differ by replica. "volumen vetus": a fixture volume created by the pre-phase migrations (copy of the existing "supplementum horarum" fixture pattern) opens, all old acts get `replica` = this volume's, and the unique index holds.
- [ ] **Step 2:** `./compile_tests.sh 'probatio_volumen$'` -> FAIL (no `volumen_replica`).
- [ ] **Step 3:** migrations appended: `ALTER TABLE acta ADD COLUMN replica TEXT`, `ALTER TABLE acta ADD COLUMN basis_replica TEXT`, `DROP INDEX acta_hora`, `CREATE UNIQUE INDEX acta_replica ON acta (hora, replica)`; `MIGRATIONES_VOLUMINIS_NUMERUS` XV. Open: read or create `volumen/replica`; extend `_horas_supplere` to fill NULL `replica`. Append stamps `replica` (and `basis_replica` from the basis). `historia.c` copies `basis.replica` the way it copies `basis.machina` (buffer, lines ~620-647).
- [ ] **Step 4:** PASS; `./compile_tests.sh volumen historia pictor scriba` green.
- [ ] **Step 5:** commit (plan file goes in this commit).

### Task 2: importing a foreign act

**Files:**
- Modify: `lib/volumen.c`, `include/volumen.h`
- Test: `probationes/probatio_volumen.c`

**Interfaces:**
- Consumes: T1 replica columns.
- Produces:
  - Column `acta.seq_originis INTEGER` (migration XVI): the writer's own seq; local acts store NULL and read back as their own `seq`.
  - `VolumenActum` gains `s64 seq_originis;` (= `seq` for own acts).
  - `#define VOLUMEN_FUTURUM_MAXIMUM_MS 600000`
  - `nomen enumeratio { VOLUMEN_IMPORTATUM, VOLUMEN_IAM_ADEST, VOLUMEN_HORA_FUTURA, VOLUMEN_CONFLICTUS, VOLUMEN_IMPORTATIO_MALA } VolumenImportatio;`
  - `VolumenImportatio volumen_actum_importare(Volumen* volumen, constans VolumenActum* actum);` - stores genus, datum, momentum, identity (hora, replica, machina), basis and `seq_originis` as given; new local seq. Same identity already present with identical genus+datum -> `VOLUMEN_IAM_ADEST` (no-op); same identity, different content -> `VOLUMEN_CONFLICTUS` (nothing written); own replica -> `VOLUMEN_IMPORTATIO_MALA`; physical ms of `hora` > now + bound -> `VOLUMEN_HORA_FUTURA`.
  - Imported `plagula-condita` / `plagula-remota` acts project into the manifest under `aliena/<replica>/<via>` (never the local `via`). `volumen_plagulas_enumerare` keeps listing everything; callers filter by prefix.

- [ ] **Step 1: failing tests** - section "importatio (communio II.1)": act read from volume A, imported into B -> `VOLUMEN_IMPORTATUM`, B's copy has A's hora/replica/machina/momentum/genus/datum and `seq_originis` = A's seq, a NEW B seq; importing again -> `VOLUMEN_IAM_ADEST`, act count unchanged ("idempotens"); same identity with altered datum -> `VOLUMEN_CONFLICTUS`, count unchanged; B's own act -> `VOLUMEN_IMPORTATIO_MALA`. "hora futura": an act stamped now+5 min imports, and B's next local append has a larger `hora`; one stamped now+20 min -> `VOLUMEN_HORA_FUTURA`, nothing written. "plagula aliena": A's `plagula-condita` for `documentum` imported into B appears as `aliena/<A>/documentum`; B's own `documentum` untouched.
- [ ] **Step 2:** FAIL. **Step 3:** implement (INSERT with explicit hora; the existing `MAX(hora)+1` rule then pushes B's clock past imported acts - the HLC receive rule needs no extra code; check it with the test). **Step 4:** PASS + `volumen historia pictor` green. **Step 5:** commit.

### Task 3: `communio` - moving acts and blobs between two volumes

**Files:**
- Create: `include/communio.h`, `lib/communio.c` (via `./silva/scribe.sh`), `lib/communio.worklog.md`
- Modify: `include/volumen.h`, `lib/volumen.c` (export reader, blob presence)
- Test: create `probationes/probatio_communio.c`

**Interfaces:**
- Consumes: T2 import.
- Produces:
  - `Xar* volumen_acta_propria_legere(Volumen* volumen, s64 post_seq, Piscina* piscina);` - own-replica acts after local seq, ordered by seq.
  - `b32 volumen_massam_habet(Volumen* volumen, chorda sigillum);`
  - `communio.h`: `nomen structura { s64 cursor; i32 importata; i32 iam_aderant; i32 massae; i32 refusa; } CommunioRelatio;` and `b32 communio_transferre(Volumen* fons, Volumen* destinatio, s64* cursor, CommunioRelatio* relatio, Piscina* piscina);` - exports `fons`' own syncable acts after `*cursor` (skipping plagulae with origo ending `:checkpoint`), copies every referenced massa missing in `destinatio` first - generic rule, no knowledge of any app's format: every run of exactly 64 lowercase hex characters in an act's `datum` that names a massa present in `fons` (covers `plagula-condita`'s `sigillum` and pictor's `<imago massa=...>`; a false match only copies an extra blob) - then imports in order, advances `*cursor` only past acts that imported or were already present; stops at the first refusal (cursor stays before it).

- [ ] **Step 1: failing tests** (`probatio_communio.c`, two temporary volumes): "transferre": A appends 5 acts + a plagula -> transfer to B: 5+1 imported, cursor = A's last seq, B lists `aliena/<A>/...`; "iterum": transfer again -> 0 imported, cursor unchanged; "eadem machina": A and B both write acts whose hora coincide, transfer both ways -> every act present on both sides, none dropped; "checkpoint non transit": a plagula with origo `pictor:checkpoint` stays home; "massae": a plagula's blob arrives before its act (B can `volumen_plagulam_promere` it); "refusa": an act beyond the clock bound stops the batch, cursor before it, a later retry (after the clock catches up - simulate by setting the test's bound low) continues.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 4: opening another replica's document

**Files:**
- Modify: `include/historia.h`, `lib/historia.c`, `include/pictor_documentum.h`, `lib/pictor_documentum.c`
- Test: `probationes/probatio_communio.c` (pictor sections), `probationes/probatio_historia.c`

**Interfaces:**
- Consumes: T2 (`seq_originis`, foreign manifest), T3 (`communio_transferre`).
- Produces:
  - `Historia* historia_aperire_alienam(Piscina*, InternamentumChorda*, Volumen*, chorda replica, constans character* spatium, constans character* genus, constans character* origo_checkpoint, i32 intervallum, HistoriaProiectio proiectio);` - reads only that replica's acts of those genera and works in the writer's numbering (`seq_originis` used as seq for cursor/branch logic); read-only: `historia_actum`, `historia_revocare`, `historia_reficere` on it return failure and write nothing; never writes checkpoints.
  - `Xar* volumen_acta_generum_replicae_legere(Volumen*, chorda replica, s64 post_seq, constans character* constans* genera, Piscina*);`
  - `PictorDocumentum* pictor_documentum_aperire_alienum(Piscina*, InternamentumChorda*, Volumen*, chorda replica, constans character* spatium);` (dimensions from `aliena/<replica>/<spatium>/documentum`).
  - `Xar* pictor_documenta_aliena_enumerare(Volumen*, Piscina*);` - Xar of `PictorPicturaAliena { chorda replica; chorda spatium; }` from the `aliena/` manifest.

- [ ] **Step 1: failing tests** - "pictura aliena": in A draw 3 strokes + an inserted image layer in a picture, undo once, redo once, draw again; transfer to B; `pictor_documenta_aliena_enumerare(B)` lists (A, spatium); `pictor_documentum_aperire_alienum` in B -> `pictor_documentum_sigillum_compositi_hex` equals A's; B's own root picture unchanged (sigillum before == after); stroke via `pictor_documentum_actum` on the foreign doc -> refused, B's act count unchanged. historia suite: "historia aliena" with the toy projection - branches and cursor in writer numbering give the same state as in the writer's volume.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + `historia pictor` suites green. **Step 5:** commit.

### Task 5: following a document as acts arrive

**Files:**
- Modify: `include/historia.h`, `lib/historia.c`, `include/pictor_documentum.h`, `lib/pictor_documentum.c`
- Test: `probationes/probatio_communio.c`

**Interfaces:**
- Consumes: T4.
- Produces:
  - `b32 historia_sequi(Historia* h);` - catches up with acts that arrived after open (or the last `sequi`); VERUM if the state changed. Result must equal a fresh `historia_aperire_alienam` (implementer may re-project from scratch when a cursor/branch act arrived, and apply incrementally otherwise).
  - `b32 pictor_documentum_sequi(PictorDocumentum* doc);` - historia_sequi + strata/composite refresh.

- [ ] **Step 1: failing tests** - "sequitur": B opens A's picture; A draws 2 more strokes; transfer; `pictor_documentum_sequi` -> VERUM and B's composite sigillum = A's; nothing new -> FALSUM. "revocatio sequitur": A undoes twice and draws once (a branch); transfer; sequi -> equals A and equals a fresh open in B. "strata sequuntur": A adds and hides a layer; follow -> equal.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS; `volumen historia pictor scriba communio` green; time a 500-act picture follow (print ms). **Step 5:** commit; spec gets a "phase 1 built" note.
