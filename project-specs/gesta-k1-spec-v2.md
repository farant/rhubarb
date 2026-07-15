# GESTA K1 + TABULARIUM — SPEC v2 (SELF-CONTAINED)

Date: 2026-07-14/15. Status: **IMPLEMENTATION READS THIS** — supersedes
gesta-k1-spec.md (v1); grounded by three explorer reports (infra, MCP
pattern, TS distillation) folded in below. Lineage: tabularium-visio.md
→ tabularium-gradus.md → gesta-k1-interrogatio.md (22 Q&A) → v1 → v2.
Precedent: mcp-legati-spec-v2 (self-contained law — a cold session
implements from THIS document plus the named source files only).

**NOMINA SEALED**: **gesta** = general C89 event-sourcing/entity
kernel (gesta/ subsystem; the log = res gestae). **tabularium** = the
first tenant (monorepo issue/doc/decision store) + its MCP server;
tools mcp__tabularii__*.

**EXPLORER CORRECTIONS vs v1** (the reasons v2 exists):
1. ⚠ **Cold path corrected**: colloquium/machinula CANNOT call SQLite
   (fixed 42-fn libc registry, officina_machinula.c:1599-1659; no FFI;
   external symbol → "vocatio: symbolum insanum" halt :2742-2751).
   Cold path = natively-COMPILED gesta/scriptum/nota_frigida.c.
2. TS smaragda has NO standing materialized table and NO HWM folds —
   it recomputes state lazily (LRU) and drains FTS at search time.
   K1's persisted-fold+HWM pipeline ASSEMBLES smaragda's reducer
   (oracle: smaragda.ts) with libraries.ts's esStore watermark pattern
   (oracle: libraries.ts). Attribute goldens accordingly.
3. TS genus definitions are ALREADY event streams (on a meta-res via
   genusReducer). K1's departure = elevating them to first-class event
   kinds in the ONE universal log, not events-vs-rows.
4. **seq must be 1-based** (AUTOINCREMENT starts at 1): HWM default 0
   + replay filter `seq > hwm` — a 0-based first event would be
   silently skipped by every fresh consumer (libraries.ts:349-378).
5. Anchor resolution = read build/nexus.tsv directly; resident-to-
   resident is IMPOSSIBLE (both MCP servers are stdio children, no
   address) until LEGATUSD, which stays uncoupled.

## I. QUID SIT

K1 = kernel + first tenant shipped together: append-only event log
(tessellae) over scrinium/SQLite; persisted folds (res state, links,
genera, FTS) advanced by per-consumer watermarks; genus definitions as
events; committed JSONL projection (annales) written in the same
transaction as every event; an MCP resident (five tools); a compiled
cold-path writer; a seed + a one-session harvest of the open prose
ledgers. Independently valuable: a queryable issue/park/decision store
with FTS and anchored notes + a reusable general kernel.

## II. DECISUS (consolidated; ← marks explorer-corrected)

1. ONE universal event table; payload = one JSON column (house json
   lib). nuntium stays out.
2. entitas_repositorium untouched; convergence = named pull post-K1.
3. tabularium.db UNTRACKED (root; + .db-wal/.db-shm gitignore lines —
   events.db precedent); **annales JSONL COMMITTED under
   gesta/annales/** ←; one line appended per event IN THE WRITE
   TRANSACTION. Refold-from-JSONL == db is a standing bar.
4. Surfaces: C library + MCP resident. NO CLI twin. ← Cold path = a
   small NATIVELY-COMPILED program (correction 1), not colloquium.
5. Genus engine data-defined day one; genera themselves are EVENTS
   (definitio-generis/emendatio-generis) in the same log; the genera
   table is a fold; refold-from-empty rebuilds everything incl schema.
6. Reducer identity is DATA in the genus definition ("ordinarius" =
   native default; K3: digest naming stored C source → machinula).
   Fold version = reducer digest.
7. Links = generic nexus/denexus events (res_a, verbum, res_b) + a
   links fold. No health/constraints (K2).
8. FTS indexes MATERIALIZED STATE only. ← Latin idiom = PREFIX
   queries ('term*'); porter stems English only. Caller appends '*'
   (fts5 has no prefix= shortcut in our config).
9. Claude = autonomous scribe (no per-note gates; review-by-query).
10. Tenant genera + lifecycles (seed script, §XI): quaestio apertum→
    laborans→clausum|relictum; parcum parcatum→tractum→clausum;
    decretum, nota stateless; desideratum apertum→impletum.
11. Tags free-form + seed vocabulary + census gardening.
12. Anchors carry write-time sigillum content hash; resolution READ
    TIME in the MCP layer via nexus.tsv ←; failure = CAUTIO line +
    similitudo suggestions; standing unresolved-anchors query. Never
    blocks, never silently rebinds. Stale tsv ⇒ at worst a false
    CAUTIO — acceptable, honest.
13. Birth content = harvest of OPEN items only (~50-100).
14. Layout: gesta/ subsystem (fontes/, probationes/, scriptum/,
    annales/, phase-log.md, compile_probationes.sh, tabularium.sh).
15. actor ∈ {fran, claude, machina} + free origo string.
16. Five MCP tools (§IX). Second resident ratified — and it is
    LIGHTWEIGHT ← (no silva/praeparator; §IX).
17. TS oracle = hand-derived goldens, 'TS: file:NNN' comments;
    divergence ledger in §XIII; no bun in the build.
18. Retention: keep everything.
19. Record-don't-block validation posture (inverts TS, which throws:
    smaragda.ts:2123,2129,2152,2162,2167 — divergence comments at the
    equivalent C sites).
20. Sequencing: K1 = the standing work, 4 chunks (§XV).

## III. SCHEMA (DDL v2 — implement verbatim, refine only with cause)

```sql
CREATE TABLE tessellae(
  seq        INTEGER PRIMARY KEY AUTOINCREMENT,  -- 1-based; THE HWM axis
  id         TEXT NOT NULL UNIQUE,               -- ULID (scrinium_ulid)
  res_id     TEXT NOT NULL,
  genus_eventus TEXT NOT NULL,                   -- §IV vocabulary
  datum      TEXT NOT NULL,                      -- JSON (compact)
  actor      TEXT NOT NULL,                      -- fran|claude|machina
  origo      TEXT NOT NULL DEFAULT '',           -- session/script/hook
  branch_id  TEXT NOT NULL DEFAULT '',           -- dormant (TS: 'main')
  creatum    TEXT NOT NULL DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now'))
);
CREATE INDEX idx_tessellae_res ON tessellae(res_id, seq);   -- replay
-- TS shape: smaragda.ts:276-286 (id/res_id/type/data/created_at/source;
-- K1 adds ULID id, splits source→actor+origo, branch default ''≠'main')

CREATE TABLE consumptores(titulus TEXT PRIMARY KEY, seq INTEGER NOT NULL);
-- TS: libraries.ts:290-296 ({prefix}_hwm); hwmGet absent→0 :373-378

-- FOLDS (rebuildable; refold drops + replays):
CREATE TABLE genera(titulus TEXT PRIMARY KEY, datum TEXT NOT NULL);
CREATE TABLE res(
  res_id  TEXT PRIMARY KEY, genus TEXT NOT NULL, titulus TEXT NOT NULL,
  status  TEXT NOT NULL DEFAULT '', datum TEXT NOT NULL,  -- JSON: attrs+tags+ancorae+notae
  creatum TEXT NOT NULL, mutatum TEXT NOT NULL);
CREATE TABLE nexus(res_a TEXT, verbum TEXT, res_b TEXT,
  PRIMARY KEY(res_a, verbum, res_b));
CREATE VIRTUAL TABLE res_fts USING fts5(
  res_id UNINDEXED, titulus, corpus, notae,
  tokenize='unicode61 remove_diacritics 2');   -- NO porter: Latin prefix idiom
CREATE TABLE sordidae(res_id TEXT PRIMARY KEY);  -- dirty queue
-- FTS pattern: libraries.ts:937-1198 (delete-then-insert upsert keeps
-- BM25 honest :867; dirty INSERT OR IGNORE :1059; drain in one txn
-- :1074-1098; empty query → nihil :1117; bm25 ascending=better :870)
```

Timestamps: SQLite strftime defaults (matches TS oracle; no C clock
code). res.creatum/mutatum copied from the folding events' creatum.

## IV. EVENT VOCABULARY + THE DEFAULT REDUCER (fold semantics — the
## goldens' beating heart; oracle = smaragda.ts:723-774)

Kinds (genus_eventus) and their datum:
- `creatio` {genus, titulus, status?, ...attributa} — K1 COMPACTION of
  TS created{}+attribute_sets+separate initial status_changed
  (smaragda.ts:2081-2103: initial status is its OWN event in TS —
  divergence ledger). Folds: state := {} then apply fields; status :=
  given, else genus initialState, else ''.
- `mutatio` {clavis: valor, ...} — SHALLOW top-level merge (TS
  attribute_set :727-730). Arrays/objects REPLACED WHOLE, never deep-
  merged (:729, :8386). `null` value = key present with null.
- `remotio` {clavis} — deletes the key (TS attribute_removed
  :731-736). **remotio ≠ mutatio-to-null. Never conflate.**
- `status` {novus} — sets state.status blindly; NO machine check in
  the fold (TS :737-739). Validation happens at APPEND time in the
  record-don't-block posture: illegal transition still appends, plus
  a violation `nota` (actor=machina) on the res.
- `nota` {textus} — appends to state.notae stream. **K1 DEPARTURE: TS
  has NO note event** (notes were whole-array attribute rewrites,
  :8373-8390). notae materialize into the res.datum and the FTS notae
  column.
- `nexus` {verbum, alterum} / `denexus` {verbum, alterum} — links fold
  (flat triples; TS models role-keyed members on relationship-
  entities :759-770 — deliberate simplification, ledger).
- `definitio-generis` {titulus, status_initialis, machina:[[ex,ad]..],
  reducer:"ordinarius", ...} / `emendatio-generis` (same shape,
  upsert) — genera fold. Content mirrors TS GenusDef (:846-853):
  states/transitions(upsert by (ex,ad) pair :1819-1822)/meta;
  attribute-schemas + roles OMITTED at K1 (K2 pull). Definition-time
  validation kept: exactly-one-initial law (:1911-1916), transition
  endpoints must be defined states.
- **Unknown genus_eventus = silent no-op in the fold** (TS default
  :771-772) — forward-compatible by law.
- A second `creatio` on an existing res = fold RESET to {} (TS :726
  literal). Append layer refuses duplicate creatio (record-don't-
  block: refuse THIS one — identity, not validation); golden pins the
  fold behavior anyway.

## V. FOLD PIPELINE (assembled: reducer smaragda + HWM libraries)

Write path (ONE scrinium transaction — new scope, no TS equivalent:
TS appendTessella is a bare INSERT :578-611):
```
BEGIN → INSERT tessellae → seq = scrinium_ultimum_id
      → append one compact-JSON line (json_scribere) to
        gesta/annales/tabularium.jsonl
      → INSERT OR IGNORE sordidae(res_id) → COMMIT
```
Fold advance (per consumer: genera, res, nexus): read hwm
(consumptores; absent → 0) → replay tessellae seq > hwm in order →
apply → upsert fold rows → set hwm = last seq **in the same
transaction as the application** (= exactly-once; recorded UPGRADE
over TS at-least-once advance-after-process, libraries.ts:228-233).
**BOOTSTRAP ORDER LAW**: within any batch the genera consumer
advances FIRST, then res/nexus — so events validate against genera
defined earlier in the same batch. FTS drains sordidae lazily at
quaerere time (TS pattern :1024): re-read res row → delete-then-
insert FTS → clear sordidae, one transaction.

Refold = drop fold tables + consumptores rows, replay from seq 1.
Refold-from-annales = fresh db, replay JSONL lines through the same
append path (ULIDs/seq/creatum PRESERVED from the lines — the JSONL
carries all tessellae columns verbatim).

## VI. GESTA API SKETCH (C; names WORKING — final at chunk A)

```
GestaMundus* gesta_aperire(Piscina*, constans character* via_db,
                           constans character* via_annalium);
b32  gesta_scribere(mundus, GestaEventum* e, chorda* id_out);
      /* the §V write path; e = {res_id?, genus_eventus, datum JSON,
         actor, origo}; res_id minted (ULID) for creatio */
b32  gesta_plicare(mundus);          /* advance all folds (§V order) */
b32  gesta_replicare(mundus);        /* full refold */
/* reads: thin wrappers over scrinium prepared stmts (res_capere,
   quaerere_fts, nexus_legere, census, annales walk per res) */
```
scrinium additions (chunk A, optional-but-do): transaction trio
`scrinium_incipere/committere/revolvere` (today: raw exsequi
BEGIN/COMMIT — lib/scrinium.c:248-272 precedent). Resident discipline:
prepare-once, scrinium_retexere per reuse, finire at shutdown.

## VII. ANNALES (the committed projection)

File: gesta/annales/tabularium.jsonl (committed). One event = one
compact JSON object line via json_scribere (VERIFIED: compact mode
emits no raw newlines; strings escaped via chorda_aedificator
_evadere_json — lib/json.c:1783, lib/chorda_aedificator.c:446-476).
Line carries ALL tessellae columns {seq,id,res_id,genus_eventus,
datum,actor,origo,branch_id,creatum}. Written inside the write
transaction (fsync discipline: fflush after line; the .db is the
local truth, the JSONL is the durable projection — a torn final line
on crash is detected at refold and reported, never silently skipped).

## VIII. ANCHORS (kernel stores, server resolves)

Stored in res.datum: ancorae:[{genus:"symbolum"|"via"|"commissio",
scopus:"chorda_secare", sigillum:"<sha256hex>"}...]; hash computed at
write (sigillum lib) over the anchored content (symbol corpus / file
bytes / commit unreachable→sha itself). READ-TIME resolution in the
MCP layer: parse build/nexus.tsv — 7 tab columns
`titulus \t sedes|usus \t genus \t via \t linea \t columna \t n`,
line 1 = `# ... GENERATUM <unixtime>` (loader shape:
officina/instrumenta/legatus.c:782-819; tabularium needs only
titulus→(via,linea) for sedes rows, ~40 lines). Outcomes: resolved+
hash-match → silent; resolved+hash-differ → CAUTIO "contentum motum";
unresolved → CAUTIO + similitudo top-3 over sedes tituli. tsv
staleness (post-commit hook, GENERATUM-stamp debounce) ⇒ worst case
a false CAUTIO — never a silent rebind. Resident-to-resident legati
query: IMPOSSIBLE until LEGATUSD (both stdio children); recorded.

## IX. MCP RESIDENT — mcp__tabularii__* (copy map from legatus)

LIGHTWEIGHT binary: links tabellarius + json + piscina + chorda(+aed)
+ xar + tabula_dispersa + similitudo + sigillum + scrinium +
vendor/sqlite3.o + gesta units. NO silva/praeparator/nexus_ordines.
If file mtimes needed, copy the 3-line stat helper
(praeparator_tempus_plagulae shape) — don't link praeparator.

COPY (~250 lines from officina/instrumenta/legatus.c, adapt
Legatus*→Tabularium*): read/dispatch loop shape (:6795-6832; EOF =
exit 0), `_mcp_tractare` gating (:6577-6638; initialize/ping pre-init,
else -32002), `_mcp_toolscall_tractare` ladder shape (:6526-6575),
`_mcp_instrumentum` schema builder (:4941-4982), `_mcp_textum_
respondere` result+CAUTIO wrapper (:5112-5153), `_mcp_initialize_
tractare` skeleton (:4984-5049) with body swapped: open db + prepare
statements + fold catch-up — NOT a repo sweep. protocolVersion pin
"2025-06-18". Doctrine TABULARII_DOCTRINA must state THE SCRIBE
CONTRACT (autonomous filing; review-by-query) so future sessions
inherit the workflow. lib/tabellarius links UNCHANGED
(tabellarius_lineam_legere/scribere + nuntium/responsum/errorem;
id verbatim as JsonValor*; compact-JSON law enforced at :229-237).
RECORDED SEED: this copy is the embryo of lib/mcp_servus — extract
when server #3 appears, not before.

Tools (all answers capped text; CAUTIO channel = anchor + torn-line
findings):
- addere {genus, titulus, corpus?, tags?, ancorae?, nexus?} →
  creatio (+link events); returns res_id + echo.
- gerere {res, actus: nota|status|nexus|denexus|mutatio|remotio, ...}
  → one event; violation notes per §IV.
- quaerere {textus?, genus?, tag?, status?} → drain sordidae, then
  FTS MATCH (caller-side '*' for Latin stems) + filters; capped rows.
- res {res|titulus} → state + anchors (resolved, CAUTIO lines) +
  timeline (capped); similitudo on miss.
- census {} → genus×status counts + tag census + unresolved-anchor
  count + hwm/seq status line.

Registration: .mcp.json sibling entry {"tabularii": {"command":
"./gesta/tabularium.sh", "args": ["-mcp"]}}. Launcher = legatus.sh
template: stderr-only chatter, mtime-gated build, `exec "$BIN" "$@"
-radix "$RADIX_DIR"` (the -radix append is LOAD-BEARING: no LSP
rootUri in MCP — officina/legatus.sh:88-91). Prebuild once before
first registration; recovery = /mcp reconnect / reload, never
kill-the-process. NO vigilia at K1: the index IS the db, fresh by
construction; binary-vs-source self-check = named park.

## X. COLD PATH (corrected — compiled, not interpreted)

gesta/scriptum/nota_frigida.c — a small native program: argv →
one gesta_scribere (nota or addere) → exit. Built/linked by
gesta/scriptum/frigida.sh (object list = the resident's minus
tabellarius). Rationale on file: machinula's host registry is a
curated 42-fn libc list, no sqlite, no FFI (officina_machinula.c:
1599-1659, 2742-2751) — colloquium stays for pure-house-lib scripts.

## XI. TENANT SEED + HARVEST

Seed = events through the kernel (definitio-generis ×5 per DECISUS
10 + a nota with the tag seed: lib names + vectis/lsp/mcp/perf/
corpus-law/...). Runs via nota_frigida-style seed program or first
MCP session. Core schema stays tenant-free (smaragda's own
TODO-regret honored). Harvest (chunk D): Claude reads phase-log
tails + MEMORY.md boards + worklog asperitates; files OPEN items,
actor=claude origo="messis-2026-07"; anchors where targets exist.

## XII. GOLDENS (hand-derived; comment each 'TS: file:NNN')

1. Shallow merge: creatio → mutatio{a:1} → mutatio{b:2} →
   mutatio{a:9} ⇒ {a:9,b:2}. TS: smaragda.ts:727-730.
2. remotio ≠ null: mutatio{k:5}+remotio{k} ⇒ k absent;
   mutatio{k:null} ⇒ k present null. TS: :731-736.
3. Whole-array replacement: mutatio{tags:["a"]} → mutatio{tags:
   ["b","c"]} ⇒ ["b","c"]. TS: :729, :8386.
4. Unknown kind no-op mid-stream. TS: :771-772.
5. status folds blind (no machine check in fold); illegal transition
   appends + violation nota (K1 posture; TS blocks — :2152,:2167).
6. Genus fold: definitio + emendatio upserting a (ex,ad) transition;
   exactly-one-initial enforced at definition. TS: :1802-1839,
   :1852-1858, :1819-1822, :1911-1916.
7. Bootstrap order: definitio-generis + creatio-against-it in ONE
   batch → genera consumer first → status_initialis lands. TS
   content: :2099-2100.
8. HWM: 3 events (seq 1,2,3), fresh consumer hwm=0 replays all 3;
   set 3; append seq 4; replay yields only 4. Seq is 1-BASED. TS:
   libraries.ts:373-378, :349-350, :245-247.
9. Exactly-once upgrade: fold+advance in one txn; simulated crash
   (no advance) → replay → identical state (idempotent). TS contract:
   libraries.ts:228-233.
10. nexus fold: nexus×2 same triple + denexus ⇒ one row remains
    correct. TS mapping: smaragda.ts:759-769.
11. FTS: planted notes found by prose, by tag, by 'praefix*'; res
    with status flip re-indexed after drain; empty query ⇒ nihil.
    TS: libraries.ts:1117, :990-1038.
12. ULID: 26 chars, timestamp prefix non-decreasing, same-ms pair
    lexicographically ordered; NEVER literal values. TS:
    libraries.ts:82-122; C: lib/scrinium.c:326-382.
13. Refold == standing folds (row-identical); refold-from-annales ==
    db; torn final JSONL line → detected + reported.
14. creatio-resets-fold pinned (TS :726); duplicate creatio refused
    at append.

## XIII. DIVERGENCE LEDGER (deliberate; comment at each C site)

- Persisted folds + HWM pipeline (TS: lazy recompute + LRU; HWM
  pattern unused by smaragda) — assembly, not copy.
- Genera as first-class event kinds in the universal log (TS: event
  streams on a meta-res via separate reducer).
- creatio compacts created+attributes+initial-status (TS: 3 events).
- nota stream (TS: whole-array attribute rewrites).
- nexus flat triples (TS: relationship-entities w/ role-keyed
  members, cardinality).
- Record-don't-block (TS throws at :2123,:2129,:2152,:2162,:2167).
- Exactly-once folds (TS at-least-once).
- branch_id default '' (TS 'main'); branch machinery NOT ported.
- actor+origo (TS single nullable source).
- Annales projection + same-transaction write (no TS equivalent).
- No porter tokenizer (Latin prefix idiom).
- attribute-schemas + roles omitted from genus defs at K1 (K2).

## XIV. BUILD/BOOTSTRAP/PLACEMENT

- gesta/compile_probationes.sh = tessera template (GCC_FLAGS verbatim
  incl -Wno-long-long; INCLUDE_FLAGS += -I vendor; RADIX_FONTES =
  piscina chorda chorda_aedificator xar tabula_dispersa internamentum
  json similitudo sigillum scrinium credo; VENDOR sqlite3.o compiled
  relaxed own-object per compile_tests.sh pattern — NEVER under house
  flags; excubitor -tacitus gesta/build/ tail line).
- fabrica.tsv: binarium rows for gesta/build/tabularium (every linked
  unit) + nota_frigida + vendor object mapping.
- .gitignore: tabularium.db, tabularium.db-wal, tabularium.db-shm
  (events.db precedent). db at ROOT (live state, not build/).
- NO amalgamation at K1 (internal consumer only; amalgama = later
  pull with full VERIFICATUM ceremony).
- Latin everywhere; worklogs at find-time; censor/examen hooks cover
  the new pool automatically.

## XV. CHUNKS (INTENTIO → build → RELATIO each)

- **A — gesta core**: schema migration (scrinium_migrare), write path
  w/ transaction trio + annales line, genera+res folds w/ bootstrap
  order, refold, goldens 1-9, 13-14.
- **B — folds infra**: nexus fold, FTS+sordidae drain, census
  queries, anchors (sigillum at write; resolution deferred to C),
  goldens 10-12.
- **C — surfaces**: MCP resident (copy map §IX), nexus.tsv anchor
  resolver + similitudo, doctrine, .mcp.json + launcher,
  nota_frigida + frigida.sh, tenant seed.
- **D — harvest + seal**: open-items harvest, THE LEDGER BAR
  ("what's outstanding for legati?" = one quaerere/census call,
  cross-checked vs the hand-compiled 2026-07-14 ledger), live
  shakedown (scribe files a real debrief note autonomously), refold
  + refold-from-annales bars, RELATIO + instrument debrief.

## XVI. BARRAE (K1 seal) — as v1 §VII plus explorer additions

1. Refold == standing folds; refold-from-annales == db; torn-line
   detection.
2. Goldens 1-14 green (TS refs in comments).
3. FTS prose/tag/anchor/prefix finds; census exact.
4. Anchor demo: rename fixture symbol → CAUTIO + similitudo;
   unresolved-anchors query lists it.
5. LEDGER BAR (§XV-D).
6. Suites green under house flags; existing suites untouched;
   excubitor/fabrica rows live; NO new exclusiones entries expected
   (gesta is pure house C89; sqlite vendor object pre-excluded).
7. Live shakedown: both residents registered simultaneously; five
   tools exercised; cold path writes with resident down.

## XVII. PARKS (named, with owners)

- lib/mcp_servus extraction (owner: server #3's arc).
- Binary-vs-source vigilia for tabularium (cheap, deferred).
- machinula reducers (K3, with actions); reducer-digest fold
  versioning activates then.
- K2: relationships proper (roles/cardinality/health), tasks;
  attribute-schemas in genus defs.
- Latin FTS tokenizer (distant; prefix idiom suffices).
- LEGATUSD convergence (uncoupled; enables resident-to-resident
  anchor resolution).
- entitas_repositorium absorption decision (post-K1 evidence).
