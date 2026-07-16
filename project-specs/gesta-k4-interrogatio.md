# GESTA K4 — INTERROGATIO (raw Q&A)

2026-07-16. Interview for K4 (rami — event-log branches + merge).
14 questions, 4 rounds. FIRST INTERVIEW CONDUCTED INSIDE THE RITUAL:
the ritus-signaculi instance's 'interrogatio' opus was claimed
(susceptum) before round one and completed (perfectum, effectus =
this file) after round four — the process advanced itself to 'spec'.

Strata read before questioning (FOUR this time):
1. INTENT: smaragda-plan.md Demo 11 (1121-1226) — original branch
   design WITH as-built divergence notes per item.
2. IMPLEMENTATION: smaragda.ts branch module (exports line 93-95;
   schema 273-423 — branch_id plumbed through EVERY index table).
3. REGRETS: docs/foundation-roadmap.md (whole) — priority #1
   "sessions + actors as entities; workspace/branch become
   session-scoped PARAMETERS instead of mutable kernel state
   (kernel.currentBranch)"; mergeBranch cache-invalidation scar.
4. USAGE: live db — left for explorer E3.

## Round 1 — scope + the two big forks

**Q1. Park split.** K4-K5 bundled in one park, bound as the running
ritual's propositum.
**A: SPLIT NOW.** Existing park RETITLED to "Gesta K4: rami" via
mutatio (res_id unchanged — the ritual's snapshot binding stays
valid; sigillare will close it). K5 "Gesta K5: palatium" filed
fresh with the palace docs (~800 lines, 4 files) anchored. One
milestone per ritual.

**Q2. Scope.** Roadmap priority #1 entangles sessions with branches.
**A: RAMI ONLY, PARAMETRIC.** Branch is an explicit PARAMETER on
every API (the roadmap's fix — no kernel.currentBranch mutable
state, ever). Sessio/actor entities = named park; our single-agent
stdio resident has no customer for them (E3 law: no lifecycle
machinery ahead of users).

**Q3. Fold strategy — THE architecture fork.** K1 folds are eager,
persistent, HWM-tracked, single-timeline; TS materialized lazily.
**A: TRUNK EAGER, RAMI LAZY.** Trunk folds untouched (zero surgery
on K1-K3 machinery). Branch reads fold on demand via the oracle's
parent-chain query: (truncus AND seq <= punctum) OR (ramus), single
SQL, fold in memory per request. Branch cost proportional to branch
use. Merge lands events on trunk where eager folds resume. Matches
oracle's "main keeps the fast path". No cache in v1 (the TS
_processCache cold-staleness trap is documented — park it).

**Q4. Merge model.** Original plan: DAG-reads forever. As-built:
replay-on-merge.
**A: REPLAY-ON-MERGE** (keep the oracle's own divergence). Merge
copies source events onto the trunk: NEW seq, annales lines
appended (K1 law holds), origo "merge:<ramus>". Trunk reads stay
simple. Cost = duplication; the acta honestly record provenance.

## Round 2 — merge mechanics + boundaries

**Q5. Merged events vs the write path.**
**A: RAW COPIES + PROBE SWEEP.** No re-validation (the branch lived
record-don't-block; its custodia notae are IN the copied stream).
Annales per line, ONE plicare after the copy transaction, then one
advancement-probe sweep over copied STATUS events — a merged "opus
perfectum" still advances its process. Decision 10 holds: the
status just arrived on the trunk.

**Q6. Branch identity.**
**A: GENUS RAMUS, ON TRUNK.** Branch = res of seeded genus "ramus"
{titulus, parens, punctum (fork seq); machina activus -> fusus |
abiectus}, ALWAYS trunk-resident (branch metadata is never
branched — oracle law). Trunk keeps branch_id '' (all live events
already carry it — no migration; annales immutable), rendered
"truncus" in surfaces.

**Q7. Engine × branches.**
**A: TRUNK-ONLY v1.** Branch writes never trigger the engine;
processes are trunk citizens. Branch work re-enters process-land
via merge (Q5 sweep). "Speculative processes" = park, zero
evidence of need.

**Q8. Schema × branches.**
**A: GENERA TRUNK-ONLY.** definitio-/emendatio-generis with a
branch = REFUSED (mechanical — schema integrity is not
record-don't-block territory; oracle pinned schema to main).
Branch events validate against trunk genera.

## Round 3 — conflicts, surface, indexes

**Q9. Conflicts.**
**A: SAME-RES + FORCE.** Conflict = res touched on BOTH sides after
punctum (oracle). Detection returns both materialized states;
merge refuses while conflicts stand unless vis=VERUM. Per-res
resolution = park (oracle never built it either).

**Q10. MCP surface.**
**A: TOOL RAMUS + PARAM.** New tool 'ramus' {actus: creare |
enumerare | comparare | fundere | abicere, ...} for lifecycle;
optional 'ramus' parameter on addere/gerere/res (per-call,
parametric — no session state). census/quaerere/tabula stay trunk.

**Q11. Annales.**
**A: ONE INTERLEAVED FILE.** Branch events append to the same
jsonl in seq order with branch_id (every line has carried the
field since K1). verificare untouched (physical 1:1, branch-
agnostic). Restitutio rebuilds branches for free.

**Q12. Search/board.**
**A: TRUNK-ONLY INDEXES.** FTS/census/tabula = trunk projections;
tabula gains a RAMI section (active branches list). Branch reads
via res/gerere + ramus param. TS built THREE per-branch FTS
indexes — skipped until a branch lives long enough to want search.

## Round 4 — judgment + ritual

**Q13. Custodia × branches.**
**A: JUDGE ON BRANCH STATE.** Branch writes validate against the
branch's own materialized state of that res (lazy per-res
parent-chain fold on the write path — cheap at single-res grain).
Custodia notae land on the branch, travel with it at merge. A
timeline without judgment would merge unjudged history onto the
trunk.

**Q14. Ritual weight.**
**A: FULL — spec v1 -> 3 EXPLORERS -> spec-v2.** E1: TS deep-read
(branch internals + the WORKSPACES layer 13.3/13.4 that sits on
branches, merge cache-invalidation scars, _rebuildIndexesForMerge).
E2: our seams (every query assuming one timeline; lazy-fold read
path; merge as kernel op writing annales; validation reads on the
write path). E3: live db mining (branch count/lifetimes/merged vs
discarded; workspaces usage; plan-vs-roadmap intent diff).

## Carried into spec v1

Trunk sentinel '' + display "truncus"; ramus genus in seed v4;
GestaEventum grows no field (branch is a PARAMETER: gesta_scribere/
fascis/agere signatures or a parallel *_in_ramo path — spec
decides); punctum = seq at creation; merge copies preserve original
actor + datum, origo rewritten "merge:<ramus>"; discard = status
abiectus (events remain, projections stop following); conflict
report shape = ConflictInfo {res_id, status_ex_utroque}.
