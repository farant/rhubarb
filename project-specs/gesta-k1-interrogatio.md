# GESTA K1 + TABULARIUM — INTERROGATIO (raw Q&A)

Date: 2026-07-14 (evening, post macro-domesticum fascis)
Participants: Fran + Claude
Companion docs: tabularium-visio.md (what the store is),
tabularium-gradus.md (phase ladder; PROXIMA staged this interview's
agenda). Six rounds, 22 questions. House law: raw transcript precedes
spec — decisions keep their reasoning attached.

NAMES SEALED THIS INTERVIEW: **gesta** (kernel), **tabularium**
(store/tenant/MCP instance).

---

## Round 1 — architecture shapers

**Q1: Event log shape (decisions-owed #3) — universal tessella table
vs per-family; JSON vs nuntium encoding?**
A: **Universal + JSON.** One tessella table, JSON data column via
house json lib. Grep-friendly raw db, matches TS oracle, sqlite3 CLI
can inspect everything. nuntium stays out of the kernel.
(Not chosen: per-family tables — schema churn per genus; nuntium
blobs — opaque raw db; hybrid promoted columns — note that res_id/
genus/actor/branch_id DO get real columns anyway; "universal + JSON"
here means the VALUE payload is one JSON column.)

**Q2: Three-body decision — entitas_repositorium vs gesta vs
tabularium?**
A: **Ignore for K1, converge later.** K1 builds fresh on scrinium;
entitas_repositorium keeps its consumers untouched; absorption =
named pull once the kernel proves itself. No coupling risk now.

**Q3: DB file vs git — what carries durable history?**
A: **Unversioned .db + committed text-dump fold.** The .db lives
untracked (build/-class); a fold projects the event log to committed
plain text (JSONL); git carries durable history; the .db is
rebuildable from the JSONL. Store self-verifies both directions.

**Q4: K1 surface (multi-select)?**
A: **C library + MCP tools day one. NO CLI twin.** Skip rule-of-two:
legati proved the reach pattern and Claude is the primary writer of
notes mid-session. (Hook write-paths deferred — noise risk day one.)
This is the interview's one reshaping of the gradus assumption
(which had said CLI first).

## Round 2 — kernel semantics

**Q5: Genus engine depth at K1 — data-defined vs compiled-in?**
A: **Data-defined day one.** Genera live as rows (TS parity; TS
tests transfer). The tenant's genus-lite = a seed script defining 5
genera. New genera never need a recompile.

**Q6: Reducer seam shape (the question gradus flagged hardest) —
how does K3 machinula slot in without an API break?**
A: **Reducer named in genus row (reducer identity as DATA).**
"ordinarius" = native default; later a digest naming stored C source.
K1 resolves names against a tiny native registry; K3 adds machinula
resolution. Fold-version-as-digest falls out naturally; zero API
break. (Rejected: function-pointer registry — reducer identity in
program code not data; no-seam — Eskil law risk.)

**Q7: Links in K1 (relationships are K2 but issues want
blocks/blocked-by now)?**
A: **Generic link events, no engine.** Event vocabulary includes
(res_a, verbum, res_b) link/unlink; fold materializes a plain links
table; queries join it. Health/cascade/constraint machinery stays K2.

**Q8: FTS scope — events, materialized state, or both?**
A: **Materialized state only.** Search answers "what is" (current
titulus/corpus/tags per res, dirty-queue maintained from the fold).
History queryable by res timeline, not full-text. Smallest index;
libraries.ts pattern.

## Round 3 — tenant UX

**Q9: Write path — scribe autonomy + resident-down story?**
A: **Autonomous scribe + colloquium fallback.** Claude files parks/
asperitates/decisions during debriefs WITHOUT per-note confirmation
(Fran reviews via queries, not gates). Resident down → a #!
colloquium script linking scrinium/gesta is the cold write path —
house interpreter as scripting layer, no new binary.

**Q10: Tenant genus seed + lifecycles?**
A: **Per-genus tiny machines** (free under data-defined engine):
issue apertum→laborans→clausum(+relictum); park parcatum→tractum→
clausum; decision/note stateless records; desideratum apertum→
impletum. States mean something in queries ("what's parked and never
pulled").

**Q11: Tag vocabulary?**
A: **Free + seeded + census.** Free-form tags, seed list for known
vocabulary (lib names, 'vectis', 'lsp', 'perf'...), census query
(tag → count) as the gardening tool — drift visible, never blocked.

**Q12: Anchor failure modes (symbol renamed/deleted after a note
anchored to it)?**
A: **Stale-flag + similitudo suggestion.** Note renders with a
CAUTIO-style line ('anchor unresolved; simillima: ...') — the
hover-vicinitas pattern applied to annotations. Nothing blocks,
nothing orphans silently; standing query lists unresolved anchors.
(Rejected: orphan triage queue — chore rot; auto-rebind — silent
identity changes in a trust store.)

## Round 4 — operations + names

**Q13: Backfill at birth?**
A: **Harvest OPEN items only.** One deliberate session: Claude reads
phase-log tails + MEMORY.md pull boards, files currently-open parks/
pulls/desiderata (~50-100 entries) with anchors and dates-as-known.
History stays in prose. The ledger bar gets real data day one.

**Q14: Dump-fold cadence?**
A: **Append on every write.** The log is append-only, so the dump is
an O(1) append of one JSONL line per event — always commit-ready,
always in sync. Refold-from-JSONL == db becomes a standing
self-verification bar. (Rejected: on-demand — "forgot to dump" =
silent history loss; pre-commit hook — debounce tar pit known.)

**Q15: Repo layout?**
A: **Own subsystem directory** (gesta/ at root with fontes/
probationes/phase-log.md like silva/officina/tessera — the house
pattern for anything with milestones). MCP binary and seed scripts
get a home; lib/ stays for single-file libraries.

**Q16: Names — seal now?**
A: **SEALED: gesta + tabularium.** Kernel = gesta (gesta/ dir,
gesta.h; Res Gestae Divi Augusti = THE Roman deeds-record — the pun
is load-bearing). Store/MCP = tabularium (the Roman records office);
tools namespace mcp__tabularii__*.

## Round 5 — remaining deep threads

**Q17: Genus definitions — events or config rows?**
A: **Genera are events too.** One log, one truth: definitio/
emendatio-generis events; the genus table is itself a fold;
refold-from-empty rebuilds EVERYTHING including schema. Seed script
= just events. Recorded as a deliberate departure/upgrade from TS
(which uses rows).

**Q18: Actor column semantics?**
A: **actor + origo pair.** actor ∈ {fran, claude, machina} (small,
queryable) + free origo string for fine provenance (session id, hook
name, script path). Two columns, each doing one job.

**Q19: MCP tool surface? (also ratified: tabularium = second
resident beside legatus at K1; LEGATUSD convergence stays uncoupled)**
A: **Five tools**: mcp__tabularii__{addere, gerere, quaerere, res,
census} — create entity / append event (nota, status, nexus, ...) /
search (FTS+filters) / one entity's state+timeline / tag+genus+
status counts. Grows by debrief demand like legati did.

**Q20: TS oracle mechanics?**
A: **Hand-derived goldens.** Read TS reducer/materialization
semantics; credo suites whose expected values are hand-computed to
match, commented 'TS: smaragda.ts:NNN'. No bun in the build;
divergences deliberate and recorded.

## Round 6 — sweep + sequencing

**Q21: Open sweep + spec-level defaults veto?**
A: **Covered — defaults accepted.** Standing defaults ratified:
gesta/ owns kernel+MCP server code; tabularium INSTANCE = untracked
tabularium.db + committed annales JSONL; MCP server speaks KERNEL
vocabulary configured by tenant seed (not hardcoded issue
vocabulary); retention = keep everything at K1.

**Q22: Sequencing after spec→explorers→spec-v2?**
A: **Next session, K1 is the work.** K1 chunks become the standing
work the way officina milestones were — capture-then-implement at
milestone scale. (~4 chunks: kernel core, folds+FTS, MCP+seed,
harvest+bars.)

---

## Interview observations (Claude)

- All 20 substantive recommendations accepted; the one design
  reshaping came from Fran's multi-select at Q4: NO CLI twin (the
  gradus had assumed nexus.sh-style CLI first). Colloquium #! scripts
  as the cold path is a deliberate dogfood of officina's charter.
- The genera-as-events answer (Q17) is the purest architectural
  decision of the set — it makes the refold bar total and the seed
  script trivial, at the cost of a slightly harder fold bootstrap
  (the genera fold must exist before events validate against genera —
  ordering note for the spec).
- Second-resident ratification (Q19) keeps the LEGATUSD convergence
  properly uncoupled per gradus §later-by-pull.
