# TABULARIUM — GRADUS (phase ladder)
## (planning seed — names unsealed; companion to tabularium-visio.md)

Born 2026-07-14, evening conversation (Fran + Claude), same day rung 5
+ comments-on-cards + corpus/inclusiones demonstrated three visio theses
live (see §Echoes below). This document exists so the phasing REASONING
survives compaction. The visio says what the store IS; this says the
order it gets built in and why each rung is independently worth having.

ORGANIZING RULES (house law applied to phasing):
1. Every phase independently valuable — stop anywhere, keep everything.
2. Every phase falsifiable against an incumbent or a golden oracle.
3. NOVEL utility preferred over replicative utility (Fran, this
   conversation): a phase that creates a capability that exists nowhere
   beats a phase that speeds up something that works. This reordered the
   ladder — build fold DEMOTED behind the issue/doc store.
4. Schema and identity are the real API (visio §I) — they absorb real
   churn as a passenger before anything depends on them.

---

## PRIOR ART HARVEST (../bun-libraries, read 2026-07-14)

Fran already built this kernel once in TypeScript. libraries.ts +
smaragda.ts are the strongest de-risking the plan has: B' is a PORT
WITH UPGRADES, not a first invention.

**Adopt (libraries.ts):**
- ULID record IDs (sortable, ts-embedded, same-ms monotonic). Resolves
  a §III subtlety: CODE entities = content-hash identity; RECORDS
  (issues/notes/events) = creation-ordered ULID. Two id-kinds by role.
- Append-only migration runner (index-tracked, no down migrations).
- HWM per-consumer watermarks over AUTOINCREMENT sequence (monotonic
  even after deletes). KEY INSIGHT: a fold = a consumer with a
  watermark. Fold maintenance becomes boring.
- FTS5 + dirty-queue incremental indexing + BM25 + porter stemming.
  FTS5 ships INSIDE the SQLite amalgamation — SQLITE_ENABLE_FTS5 is a
  PHASE-A COMPILE FLAG (decide at vendor time, cheap then, painful
  later). Makes B' prose-searchable day one.

**Adopt as structure (smaragda.ts):**
- res + tessella (entity streams + append-only facts); branch_id and
  source/actor columns ON EVERY ROW from day one — dormant schema
  presence for §VI agent branches + actor model (decisions-owed #3),
  no engine until pulled.
- Genus-as-data for lifecycle state machines — but GENUS-LITE only:
  hardcoded genera v1 (issue/park/decision/note/desideratum), small
  states, prose escape hatch. NOT the meta-engine (features/actions/
  processes/lanes = the taxonomy-engine road §X forbids).
- Palace tables = phase-E blueprint (rooms/scrolls/NPC + per-family
  FTS). Tabularium's upgrade over smaragda: anchor to ENTITIES the
  index resolves, not slugs → mechanical doc-rot detection (§VIII).
  Reframe: a scroll IS an annotation anchored to a room; B' annotations
  anchored to symbols are scrolls-with-better-anchors. Spatial layer
  is the only part that waits.
- Lesson received verbatim: smaragda's own TODO(phase 3) regrets core
  migrations owning tenant tables. Core schema minimal; tenant tables
  composable, from the start.

**Decline:** sync/Litestream (single machine, single writer — the
visio's whole simplification), HTTP transport (stdio is the house MCP
idiom, legati-proven).

**SCOPE EXPANSION (Fran, same conversation): THE KERNEL IS ITS OWN
ARTIFACT.** The full smaragda engine — genus meta-system, features,
actions, processes/lanes, relationships, health, tasks, branches,
palace — is in scope as a GENERAL C89 LIBRARY (smaragda-in-C, name
unsealed), useful for many future applications beyond tabularium.
This dissolves the earlier §X tension by LAYERING: §X's "not a
taxonomy engine" constrains TABULARIUM-THE-TENANT (small genus set,
small vocabulary); the KERNEL beneath it is generic and maximalist.
Tabularium's issue/doc store becomes the kernel's first consumer, not
its owner. Genus-lite above = the tenant's CONFIGURATION of a full
genus engine, not a cut-down engine.

## KERNEL MILESTONE LADDER (smaragda-C; the TS file is the spec)

~11.7k lines TS across eleven modules = tessera-class subsystem with
its own milestones, each credo-suited, TS behavior as oracle:
- **K1**: res/tessella (append/replay/materialize) + genus engine +
  FTS. ENOUGH FOR THE TABULARIUM ISSUE STORE — first consumer arrives
  at K1, novel utility stays early.
- **K2**: relationships, health, tasks.
- **K3**: actions (handler registry, actions-as-data) +
  processes/lanes.
- **K4**: branches/merge (parent-chain materialization, conflict
  detection) — visio §VI agent-session machinery arrives as a side
  effect.
- **K5**: palace (rooms/scrolls/NPC + per-family FTS; entity anchors
  replacing slug anchors when consumed via tabularium).

C-port design questions for the K1 interview: reducer API (see
MACHINULA REDUCERS below — K1 ships native default reducer only);
side-effect handler registry; JSON data column stays (grep-friendly
raw db, house json lib exists). OPEN COLLISION (three-body now):
entitas_repositorium (existing in-house C event sourcing w/ nuntium
persistence + RetentionPolicy) vs smaragda-C vs tabularium — instinct:
smaragda-C absorbs that role eventually (SQLite durability >
hand-rolled), decide at K1 interview, path recorded either way.

## MACHINULA REDUCERS (Fran, same conversation): DYNAMIC BEHAVIOR VIA
## THE HOUSE INTERPRETER

The TS kernel's closures (custom reducers, action preconditions,
process gates) do NOT degrade to a compiled-in registry: reducers are
C89 SOURCE STORED AS DATA (tessellae), silva-parsed, demissio-lowered,
machinula-interpreted. Behavior-as-data in the system's own language —
the Emacs/Smalltalk move without a foreign embedded language. This is
officina's charter verbatim ("REPLs, scripting, live tooling") gaining
its first production consumer, and the "runtime C inside solarium"
VISIO pin gaining its second.

Convergences:
- Fold versioning becomes LITERAL (visio §IV): fold version = digest
  of reducer source (phase A lib); materializations cached by
  (reducer-hash, stream-watermark); invalidation automatic.
- Interpreted reducers use HOUSE LIBS (json lib is corpus C89 — same
  json_legere as the host; no FFI dialect).
- Sandbox primitives EXIST (M4b: RECUSATIO, refusal flags, ansae
  capture) — pure-core enforced by VM; side effects returned as
  DESCRIPTIONS, host executes (matches smaragda actions-as-data).
- vindex can DEBUG A REDUCER against the real event log, step by
  step; deterministic replay = the time-travel v2 flagship with a
  business purpose.

Hybrid law (from TS defaultReducer fast-path): NATIVE default reducer
for common attribute-event folds; machinula for the dynamic minority;
lowered medulla cached per reducer-hash. Reducers write in the Waves
0+1 corpus subset. PHASING: K1 = native default only; machinula
integration lands ~K3 with actions/gates (where TS leans on closures
hardest). Perf reality: interpretation ~10-100x native — fine at ERP
event volumes, hybrid covers the hot path, M5 aarch64 backend is the
eventual escape hatch.

---

## PHASE A — FUNDAMENTA (two-and-a-half libs; start anytime, no ritual)

1. **Digest lib** (name unsealed; friatio is a TABLE hash, not an
   identity hash — when hash = identity, collision resistance is
   load-bearing). SHA-256-class, C89, Latin API. Bar: NIST vectors as
   goldens. Standalone value: excubitor could content-compare today.
2. **Vendored SQLite + Latin wrapper.** Amalgamation compiled relaxed
   in its own object (stb_image precedent; treat like libc, visio §II).
   COMPILE FLAGS DECIDED HERE: SQLITE_ENABLE_FTS5 on. Wrapper carries
   house idiom (aperire/exsequi/praeparare/gradi...), WAL + busy_timeout
   + foreign_keys pragmas (libraries.ts sqliteOpen), append-only
   migration runner, single-writer discipline documented in the header.
   Bar: round-trip probatio + WAL smoke + FTS5 smoke.
3. **ULID-class ID** (may live inside the wrapper lib). Same-ms
   monotonicity; randomness via posix seam (the ONE seam file pattern).

Phase A settles decisions-owed #1 (SQLite exception) by doing.
Deliverable even if tabularium stalls: a database library + a digest
library the whole monorepo can use.

## PHASE B' — KERNEL K1 + ISSUE/DOC STORE AS FIRST TENANT (FULL
## RITUAL: interview → simulatio → spec before schema freeze)

(Restructured by the scope expansion above: B' = kernel milestone K1
plus the tabularium tenant configured on it. The tenant description
below stands; the store machinery beneath it is the general kernel.)

The first tenant is the thing with NO incumbent: issues, parks,
decisions-owed, desiderata, debrief asperitates, dated notes — today
scattered as prose across phase-logs/worklogs/MEMORY.md, retrievable
only by grep or by Claude. (Evidence: the "what's outstanding?" ledger
2026-07-14 was compiled by hand-scanning four documents.)

- Schema v1: entities (res) + eventus (tessella) + genus-lite + tags +
  FTS5-with-dirty-queue. branch_id/actor present-but-dormant.
- ANCHORS: (kind, id) key space from day one (§III richness). Symbol
  anchors resolved AT READ TIME through legati's warm index (shipped
  2026-07-13/14 — the anchor infrastructure already exists). Content
  hash of the anchored entity pinned AT WRITE TIME (phase A digest) →
  "this note predates a change to its target" = annotations that know
  they are stale (§VIII) nearly free.
- Stratum discipline exercised for real: human/agent judgments =
  canonical events; the searchable current-state table = FOLD #1
  (materialized view, HWM-maintained, refoldable in microseconds).
- Interface v1: CLI (nexus.sh-style) + write-paths from the debrief
  pipeline; MCP tool(s) when the reach pattern shows itself (rule of
  two — probably fast).
- Bonus targets: MEMORY.md de-bloat (the giant officina line is an
  issue DB in prose); post-compact orientation as queries (CLAUDE.md's
  prophesied "project-specific MCP server").
- Bars: refold-from-log == standing table (self-verifying store, §IV);
  FTS returns known-planted notes by prose + tag + anchor; ledger
  question ("what's outstanding for legati?") answered by ONE query,
  cross-checked against the hand-compiled 2026-07-14 ledger.

## PHASE C' — OBSERVATIONAL STRATUM (second novel tenant)

cursor/captare/probationes record run outcomes keyed by (state hash,
action, actor) — visio §VII. Novel queries that exist nowhere: "which
edit flipped this test", "when did this suite last fail and on what
tree", flake correlation. Recording side = hooks into existing runners;
stdout blobs enter the CAS (first real CAS exercise). Retention shapes
from unified-state-paradigm-v2 vocabulary.

## PHASE D' — BUILD FOLD (§IX proper, landing on a proven store)

Node = (action, input hashes) → artifact hash; artifacts in CAS;
dependency discovery from silva's include graph (inclusiones.tsv
machinery). First consumer: one pool (celer or officina suite objects).
Bars: byte-identical binaries vs incumbent scripts; EARLY-CUTOFF demo
(comment-edit a header → recompile → byte-identical object → downstream
relink STOPS — structurally impossible for mtimes); excubitor as
falsification oracle (fold-fresh ↔ excubitor-fresh across a patrol).
Retires the stale-object bug class; AEDILIS dissolves into this phase.
Bootstrap scripts remain until proven (visio §IX rule).

DEMOTION RATIONALE (recorded): D' was originally phase C. Build fold =
high but REPLICATIVE utility (scripts work; rung 5 made the index fast).
Issue store + observational log = novel utility. Also D' lands better
third: by then the store has survived two tenants' worth of churn.

## LATER, BY PULL (§X discipline — consumer named, path preserved)

- **Palace projections** (rooms/routes over B' annotations; smaragda
  pattern with entity anchors; non-commutative-context ordering as
  route records). Pulls when doc-problem bites at toolchain scale.
- **Annales-as-view** (git stays until log depth makes queries win).
- **Checkout leases / agent branches** (branch_id awakens).
- **LEGATUSD convergence**: single-writer store + warm daemon are the
  same project from two ends; the daemon becomes the store's tenant.
  DO NOT couple phases A-D' to it — two risky things holding hands.
- **Index-as-fold migration** (nexus tsv → store): optional, maybe
  never — working incumbent, 0.3s quiet runs post-rung-5.

## ECHOES (why 2026-07-14 raised confidence)

- Rung 5 = §IX rehearsed in mtime vocabulary (watermark, reverse
  closure, stream-merge); its GENERATUM-stamp retreat from mtimes =
  §III's law learned by scar. Early cutoff is the one thing it
  structurally cannot do.
- Comments-on-cards = §VIII v0 (in-source comments as first annotation
  stratum; chorda gap = retrieval surface creates the incentive to
  write anchored docs).
- The sediment thesis felt from inside: nexus.tsv, inclusiones.tsv,
  fabrica.tsv, tempora_capitum — each with hand-rolled invalidation;
  rung 5 was manual fold maintenance.
- declarans shape lesson: "declaration" is already a first-class thing
  legatus reasons about — one step from wanting durable identity.

## PROXIMA

**PHASE A SHIPPED 2026-07-14 (same evening it was pulled).**
vendor/sqlite3.{c,h} = SQLite 3.53.3, relaxed own-object compile
(compile_tests.sh VENDOR_SOURCES; flags recorded there, FTS5 ON);
lib/sigillum.{c}+include (SHA-256, NIST goldens 4 vectors +
boundary sweep); lib/scrinium.{c}+include (aperire/exsequi/
praeparare/ligare/gradi/columnae/migrare/ulid; WAL pragmas;
append-only migration runner; ULID monotonic). Suites: sigillum
green first run; scrinium 73/73. FINDS (worklogs): ① Roman-numeral
macros ate sqlite3.h ("char C" -> "char 100") — vendor headers
BEFORE latina.h, always (second instance of the class after stb);
② porter stemming is ENGLISH — Latin corpus search idiom = prefix
queries ('parsur*'); custom Latin FTS5 tokenizer = named distant
pull. Names scrinium/sigillum WORKING, unsealed — Fran may rename.
Decision-owed #1 (SQLite exception) settled by doing.

Next rung: B' B' (= kernel K1 + tenant) waits on
the interview → simulatio → spec ritual; the K1 interview should press
on: reducer/handler API shape in C, entitas_repositorium three-body
decision, anchor semantics + resolution failure modes, genus initial
set + issue lifecycle in a one-person+Claude shop, write-path
ergonomics (where do notes come from mid-session?), tag vocabulary
seed, retention defaults, and the eventus/tessella table shape
(decisions-owed #3). Kernel milestones K2-K5 follow by pull, TS
modules as their specs.
