# K4 EXPLORATIO 1 — oraculum (TS branch/merge + workspaces)
# Relatio exploratoris verbatim, 2026-07-16. Fons pro spec-v2.
# File: /Users/francisarant/Documents/projects/bun-libraries/smaragda.ts (11691 lines)

HEADLINE: replay-on-merge is real and matches our decision, but three
things drift hard from spec-v1's picture. (1) The regret the spec cites —
`kernel.currentBranch` mutable state — NO LONGER EXISTS; the oracle
already refactored branch into `kernel.currentContext.branchId` (a
session context object). Branch is still resident state, just session-
scoped. (2) The nested-branch read is LEAKY: intermediate branches in a
parent chain get NO upper-bound on seq, so a grandchild sees its
parent's post-fork writes. Our spec's per-link `seq<=p` ceiling is a
CORRECTION, not a port. (3) The oracle maintains SIX eager per-branch
index families rebuilt/copied at merge by FOUR module hooks — this is
exactly the machinery decision 3 (rami lazy) + decision 12 (trunk-only
indexes) declines, and it's where every merge bug lives (NPC asymmetry,
cache-invalidation scars). We inherit none of it. Also: oracle merge
OVERWRITES actor attribution (single `source` field), does NOT re-trigger
processes, and does NOT refuse genus-on-branch (just pins genus reads to
main). Our decisions 5/8 and actor-preservation are additions/corrections.

Branch module lives at 5740-6076 (spec-v1 said 93-95 — those are the
export-manifest COMMENT lines; real code is 5740+). Bootstrap 1265-1307.
Read path 638-721. Schema 269-424.

## 1. Branch identity & storage
- `BRANCH_GENUS_ID = "00000000000000000000000004"` (869), a sentinel res
  under `META_GENUS_ID` (1270-1272). Bootstrap `_bootstrapBranchGenus`
  1265-1307.
- Genus meta (1274-1277): name "Branch", kind "entity",
  `hidden_infrastructure: true`, `fts: "none"`. Excluded from
  listGenera/listEntities via the hidden_infrastructure filter (2353),
  NOT a hardcoded id list.
- Attributes (1278-1286): `name` (text, required), `parent_branch`
  (text, optional), `branch_point` (number, optional).
- States (1287-1295): `active` (initial), `merged`, `discarded`.
  Transitions (1296-1301): active→merged, active→discarded ONLY.
  merged and discarded are TERMINAL — no reactivation path.
- **"main" is itself a branch res** (1304-1306): bootstrap does
  `createRes(BRANCH_GENUS_ID, "main")` then sets name="main",
  status="active". So the trunk is a materializable entity.
  DIVERGENCE from our decision 6: our trunk is the pure sentinel `''`
  with NO ramus res; the oracle's trunk = the string "main" AND a real
  branch entity. Our `truncus` is not a res. (Implication: `listBranches`
  returns "main" as a row; our `gesta_ramos_enumerare` must decide
  whether truncus appears in the list — oracle includes it.)
- `_findBranchByName` (5788-5827): O(n) scan of ALL branch res
  (`SELECT id FROM res WHERE genus_id=? ORDER BY created_at ASC`),
  materializes EACH on branch_id:"main", matches by `state.name`. No
  name index. Special-cases "main" (5792-5809) returning a synthetic
  {parent_branch:"", branch_point:0}. **COST**: this scan runs once per
  parent-chain link on every non-main materialize (see §2), so branch
  reads are O(total_branches × chain_depth) metadata lookups on top of
  the tessella fold. Our lazy design must not replicate this — resolve
  the chain from the ramus res state directly, cache the walk per read.

## 2. The read path — `_collectBranchTessellae` (678-721)  ★load-bearing
Verbatim algorithm:
1. Walk parent chain (686-692): `current = branch_name`; while
   `current !== "main"`: `_findBranchByName(current)`; push
   `{branch_id: current, branch_point}`; `current = parent_branch ?? "main"`.
   chain[0] = the leaf branch itself; chain[last] = the branch that
   forked directly off main.
2. Build clauses (695-711):
   - `mainLimit = chain.length>0 ? chain[chain.length-1].branch_point : null`
   - main clause: if mainLimit!==null →
     `"(branch_id = 'main' AND id <= ?)"` param mainLimit (700-702);
     else `"(branch_id = 'main')"` (704).
   - per-branch clause, loop `i = chain.length-1 … 0` (708-711):
     `"(branch_id = ?)"` param `chain[i].branch_id`.
     **NO id ceiling on any branch clause.**
3. SQL (713-718):
   `SELECT * FROM tessella WHERE res_id = ? AND (<clauses joined OR>)`
   `[+ " AND id <= ?" if upTo]` `ORDER BY id ASC`. Fold in caller.

★ **BREAK — leaky nested isolation.** For main→A→B reading B, chain =
[{B,bp_B},{A,bp_A}], mainLimit=bp_A. Clauses:
`(main AND id<=bp_A) OR (branch_id=A) OR (branch_id=B)`. The `branch_id=A`
clause has NO `id<=bp_B` bound, so **B sees every A tessella including
those A wrote AFTER B forked**. Correct isolation needs each parent link
capped at the CHILD's branch point (A capped at bp_B). Single-level
(main→A) is fine because A is the leaf and wants all of A. Our spec §III
`(branch_id=a AND seq<=p2)` is the fix — but the implementer must be
clear that p on each link is the *fork point of the next branch down the
chain*, not that branch's own fork point. This is a genuine oracle bug
we should not port.

- The Demo-11 plan step 4 ("Also include tessellae from any merged
  branches", 1156) is DEAD: replay-on-merge copies merged events onto
  the target with the target's branch_id, so they land via the ordinary
  `(branch_id=target)` clause. No merge-aware read logic exists.

## 3. materialize + entity cache (638-676)
- branch_id defaults "main" (643). Fast path for main: direct
  `SELECT … branch_id='main'` (655-663). Non-main → §2.
- Entity cache keyed `${res_id}\0${branch_id}` (649,672), only when
  `upTo===undefined && reducer===defaultReducer` (648,671). So the cache
  is per-(res,branch). A branch read caches the FOLDED branch state.
- **Cold-cache staleness note** (carried from K3): appendTessella's
  process-cache invalidation (594-609) only fires if the genus is
  already known-process; irrelevant to entity branch reads but the same
  class of trap. For branches the relevant hazard is that the entity
  cache is keyed by branch but the CHAIN it folded is not versioned — a
  write to a PARENT branch after a child cached does not invalidate the
  child's cache key. Oracle gets away with it because branch reads are
  rare and merge explicitly deletes target keys (5989-5992). We fold
  lazily with no persistent cache (decision 3, no cache v1) → we sidestep
  this entirely.

## 4. Write path — branch tagging (`appendTessella` 578-611)
- `branch_id = opts.branch_id ?? "main"` (585). `source = opts.source ??
  (currentContext ? "actor:${actorId}" : null)` (586-587). So provenance
  and actor share ONE column (`tessella.source`).
- Single INSERT … RETURNING (588-590). Then `_routeFtsDirty(res, branch)`
  (592) — EAGER per-branch FTS dirty mark on every write. Then entity/
  genus/process cache invalidation (594-609), keyed by `res\0branch`.
- `createRes` (507-527) takes branch_id (default "main") AND workspace_id
  independently; writes the "created" tessella on that branch (525).
- `createEntity` (2082-2096): `effectiveBranch = branch_id ??
  currentContext?.branchId ?? "main"` (2082);
  `createRes(genus, effectiveBranch, currentContext?.workspaceId)` (2096).
  Branch and workspace pulled from context separately — no coupling.

## 5. Lifecycle ops (verbatim)
- `createBranch(kernel, name, parent?)` (5831-5861): unique-name guard
  (5837); `parentBranch = parent ?? currentContext?.branchId ?? "main"`
  (5841) — **forks from CURRENT branch by default → nested branches are
  native**; `branchPoint = MAX(tessella.id) ?? 0` (5844-5845); creates
  branch res ON "main" (5848); sets name/parent_branch/branch_point
  attrs + status active (5849-5852). Returns BranchSummary.
- `switchBranch(kernel, name)` (5863-5879): **requires currentContext**
  (5864, throws otherwise); writes `current_branch` attr on the SESSION
  res (5868/5877) AND mutates `currentContext.branchId` (5869/5878).
  Refuses non-active branches (5874). This is the resident mutable state
  our decision 2 eliminates — no gesta analog.
- `listBranches` (5881-5896): scan all branch res, materialize each on
  main, map to BranchSummary. Includes "main".
- `findBranchByName` (5898-5905): thin export of `_findBranchByName`.
- `detectConflicts(kernel, source, target)` (5907-5949):
  `sourceRows = SELECT DISTINCT res_id FROM tessella WHERE branch_id=source`
  (5916-5918); per res_id, `SELECT COUNT(*) … WHERE res_id=? AND
  branch_id=target AND id > sourceBranch.branch_point` (5923-5925);
  cnt>0 → conflict. Materializes BOTH sides (5929-5930), looks up genus
  name in try/catch (5932-5937). ConflictInfo carries FULL source_state
  + target_state (whole reducer output), not just status. Matches
  decision 9 (same-res-both-sides-after-punctum); note ours returns
  `status_ex_utroque`, oracle returns whole state.
- `discardBranch(kernel, name)` (6054-6065): refuses "main" (6055) and
  non-active (6058); transition→"discarded" on main (6061); if it's the
  current branch, reset `currentContext.branchId="main"` (6062-6064).
  **Tessellae remain and stay physically readable** — nothing checks
  status in `_collectBranchTessellae`; "projections stop following" is
  enforced only at `switchBranch` (5874), not at read. materialize on a
  discarded branch_id still folds it.
- `compareBranches(kernel, res_id, branch_a, branch_b)` (6067-6076):
  two materializes, returns `{branch_a, branch_b}`. Trivial.

## 6. `mergeBranch` — replay-on-merge (5951-6001)  ★
- `targetBranch = target ?? "main"` (5957). Refuse if source not active
  (5960-5962).
- Conflict gate (5964-5970): if `!opts.force`, detectConflicts; if any →
  **return `{merged:false, conflicts}` (no throw)**.
- Copy (5972-5986): `SELECT * FROM tessella WHERE branch_id=source
  ORDER BY id ASC` (5974-5976); inside `db.transaction`, per row:
  `INSERT INTO tessella (res_id, branch_id, type, data, created_at,
  source) VALUES (?, targetBranch, type, data, created_at,
  "merge:${source}")` (5980-5983).
  - **New id auto-assigned** (AUTOINCREMENT) → new seq. ✔ replay-on-merge.
  - **created_at is COPIED** from the source row (5982), NOT regenerated.
  - **`source` is OVERWRITTEN** with `merge:${source}` (5973,5982) →
    **original actor attribution is DESTROYED**. The oracle has one
    provenance column; gesta separates actor / origo / genus_eventus, so
    our decision (preserve actor+datum+genus_eventus, rewrite ONLY origo
    to `merge:<titulus>`) is a genuine improvement the oracle can't make.
- **Transaction scope is the copy loop ONLY** (5978-5986). Cache
  invalidation (5988-5992), `_rebuildIndexesForMerge` (5995), and
  `transitionStatus(source,"merged")` (5998) all run OUTSIDE the txn.
  → **merge is NOT atomic**: a failure after the copy commits leaves
  duplicated tessellae with source unmarked and indexes stale. Our K1
  crash-law + scrinium must wrap copy + status + probe-sweep in ONE
  transaction (E2's "no nested BEGIN" concern is real; the oracle simply
  doesn't).
- Cache invalidation (5988-5992): per copied row, delete
  `_entityCache[res\0target]` and `_processCache[res\0target]`. Only
  target-branch keys.
- `transitionStatus(sourceBranch.id, "merged", {branch_id:"main"})`
  (5998) — branch entity lives on main.
- Returns `{merged:true, tessellae_copied: rows.length}` (6000).
- **NO process advancement / probe sweep.** Oracle processes advance
  only on completeTask (K3 §3). Merged status events do NOT re-enter
  process-land. Our decision 5 (advancement-probe sweep over copied
  STATUS events) is a NET-NEW addition layered on K3's completeTask
  generalization — the oracle has no such hook.

## 7. `_rebuildIndexesForMerge` (6003-6052) — eager index reconciliation
Runs after the copy (outside its txn). Steps:
1. Module merge hooks: `for (mod of kernel.modules) mod.merge?.(kernel,
   source, target)` (6011-6013). See §8.
2. `DELETE FROM relationship_member WHERE branch_id=source` (6016) —
   source-branch relationship-index cleanup.
3. Relationship-member rebuild (6018-6041): find res_ids whose
   member_added/member_removed tessellae were copied
   (`t.source='merge:X' AND t.branch_id=target AND t.type IN
   ('member_added','member_removed')`, 6019-6022); per res_id DELETE its
   target index rows, re-materialize on target, re-INSERT each
   (relationship_id, role, entity_id, target) (6024-6040). EAGER rebuild.
4. FTS dirty marking (6043-6050): find ALL copied res_ids
   (`t.source='merge:X' AND t.branch_id=target`), `_routeFtsDirty(res,
   target)` each. FTS is marked DIRTY (lazily flushed at search), not
   rebuilt in place.
This whole function exists ONLY because the oracle keeps denormalized
per-branch indexes that the raw tessella copy doesn't touch. Under our
choice (copy onto trunk, eager trunk folds resume via K1) this function
has NO analog — the K1/K2/K3 trunk reducers re-project on the copied
events. **We delete this entire problem class.**

## 8. Module merge hooks — the eager-index tax (contract at 8937-8938)
`ModuleDef.merge?: (kernel, sourceBranch, targetBranch) => void`, "Called
from mergeBranch per registered module." Four live implementations, each
reconciling a per-branch index the tessella copy left stale:
- **PALACE_MODULE** `_palaceMergeCleaner` (7378-7469, registered 7482):
  DELETE source-branch palace_room_index + palace_scroll_index (7387-88);
  rebuild palace_room/scroll/npc index rows for merged res on target by
  re-materializing. **Carries a live bug** (TODO 7380-7384): source
  cleanup deletes room+scroll index rows but NOT palace_npc_index →
  **stale NPC index rows after every branch merge**. Textbook example of
  eager-index-at-merge fragility.
- **COORDINATION_MODULE** merge (9244-9276): `INSERT OR REPLACE` copies
  exchange_index + message_index rows from source branch_id to target
  (9255-9267); `INSERT OR IGNORE` copies exchange_context_index
  (9269-9275). Three per-branch indexes hand-copied.
- **PROJECTS_MODULE** merge (10683-10691): `INSERT OR REPLACE` copies
  project_index rows source→target.
- Core (not a module) handles relationship_member + FTS inline in §7.

## 9. Eager per-branch index COST INVENTORY (what we decline)
Every table below carries `branch_id` and is maintained eagerly on the
write path and reconciled at merge. Decisions 3 (rami lazy) + 12
(trunk-only indexes) decline ALL of them for branches; gesta keeps only
trunk (branch_id `''`) projections via K1 eager folds.

| Index family | schema | grain | write-path upkeep | merge upkeep |
|---|---|---|---|---|
| relationship_member | 305-312 (+idx 311-312) | (rel,role,entity,branch) | eager on member_add/remove | §7 rebuild |
| smaragda_entity_fts (+_meta,_dirty) | 403-405 | doc_id×branch (PK incl branch) | eager dirty-mark | §7 dirty-mark → lazy flush |
| smaragda_scroll_fts (+_meta,_dirty) | 406-408 | doc_id×branch | eager dirty | palace hook / lazy flush |
| smaragda_palace_fts (+_meta,_dirty) | 409-411 | doc_id×branch | eager dirty | palace hook / lazy flush |
| exchange_context_index | 416-423 | (exchange,context,branch) | eager on exchange write | §8 coord copy |
| exchange_index | 9713-9726 | branch_id col | eager | §8 coord copy |
| message_index | 10181-10193 | branch_id col | eager | §8 coord copy |
| palace_room/scroll/npc_index | 361-401 | branch_id in PK | eager | §8 palace rebuild (npc bug) |
| project_index | module | branch_id col | eager | §8 projects copy |

What the eager indexes BUY: fast branch-scoped search (FTS), fast
relationship-member lookups, fast exchange/message/project queries —
WITHOUT folding tessellae, at any branch depth.
What DECLINING costs us: a branch has NO indexed search and NO fast
membra lookup; branch reads fold from tessellae each request (decision
12: FTS/census/tabula trunk-only, branch reads via `res`/`gerere` + ramus
param, single-res grain). **Where it bites:** a long-lived branch that
wants to be searched, or a wide branch-scoped membra query. Given our
single-agent resident and short-lived hypothesis branches, this is the
right trade — and E3's usage mining should confirm branches were
short-lived. NOTE: FTS is already LAZY even in the oracle (dirty-mark
eager, flush at search) — so the "eager" cost for FTS specifically is
just a dirty row per write per branch, cheap; the expensive eager index
is relationship_member (full row set per branch).

## 10. WORKSPACES layer — orthogonal, no leak into branch semantics
- Plan Demos 13.3/13.4 (2043-2172). Key law (plan NOTE 2108): "Branches
  and workspaces are independent axes… `branch_id` scopes tessellae;
  `workspace_id` scopes res."
- `workspace_id` is a nullable column on the **res** table (327, migration
  `ALTER TABLE res ADD COLUMN workspace_id TEXT`; idx 328), NOT on
  tessella. Denormalized for the listEntities hot path (plan NOTE 2106).
- `WORKSPACE_GENUS_ID = "…08"`, sentinel like branch. `currentContext`
  carries `workspaceId` alongside `branchId` (8917-8922).
- **No leak into branch fold**: `materialize`, `replay`,
  `_collectBranchTessellae`, `detectConflicts`, `mergeBranch`,
  `_rebuildIndexesForMerge` NEVER reference workspace_id. The branch
  read path is workspace-blind. Workspace filtering happens only in
  `listEntities` (2326-2334: `AND workspace_id=?` or `… OR workspace_id
  IS NULL`) and in FTS meta payload.
- `switchWorkspace` (2667-2671) mutates `currentContext.workspaceId`;
  `switchBranch` mutates `.branchId`. Both are session-scoped mutable
  context, independent.
- `mergeWorkspaces` (2716-2727) is UNRELATED to branch merge: it's a bare
  `UPDATE res SET workspace_id=target WHERE workspace_id=source` +
  deleteWorkspace. No tessellae, no replay. Do not conflate with
  mergeBranch.
- **For K4**: workspaces are OUT of scope (decision 2 — sessio/actor
  entities parked; single-agent resident has no customer). gesta has no
  workspace axis. The only lesson: keep the branch parameter orthogonal
  to any future scoping axis, exactly as the oracle kept branch and
  workspace from touching each other's read paths.

## 11. currentBranch → currentContext DRIFT (spec-v1 cites stale regret)
- Spec-v1 §I and interrogatio stratum 3 cite the roadmap regret as
  `kernel.currentBranch` mutable field. **That field no longer exists.**
  Live oracle: `kernel.currentContext: KernelContext | null` (212), where
  `KernelContext = {sessionId, actorId, workspaceId, branchId, modules}`
  (8917-8923). The roadmap's priority #1 "session model" (docs/
  foundation-roadmap.md:18-20) is at least PARTIALLY built — there IS a
  session context object; branch/workspace are fields ON it.
- So branch is STILL resident state in the oracle, just session-scoped
  (mutated by switchBranch, 5878) rather than a global kernel field.
  Our decision 2 (branch = explicit PARAMETER on every API, never
  resident) goes FURTHER than even the healed oracle: gesta threads
  `ramus` explicitly with NO `currentContext.branchId` analog.
- Practical note for the implementer: don't grep the oracle for
  `currentBranch` (gone) and don't treat `currentContext.branchId` as the
  thing to port — it's the thing we're deliberately NOT porting.
- Also: `appendTessella` now derives actor from context
  (`source = "actor:${currentContext.actorId}"`, 587). The oracle DOES
  attribute actors now — but only via that single `source` column, which
  merge then clobbers (§6). Our separate actor field is what lets us
  preserve attribution across merge.

## 12. Demo 11 notes vs live code — drift audit
Plan Demo 11 (1121-1222) is largely accurate; drift since the as-built
notes were written:
- 1141 "Added `currentBranch: string` to Kernel… `initKernel` sets
  main" — **SUPERSEDED**: now `currentContext.branchId` (§11). The
  low-level default is still literal "main".
- 1150-1159 materialization-with-DAG NOTE — accurate to
  `_collectBranchTessellae`, but the note does NOT mention the missing
  intermediate-branch ceiling (§2 break); the bug predates or survived
  the note.
- 1156 "Also include tessellae from any merged branches" — DEAD under
  replay-on-merge (§2).
- 1162 replay-on-merge NOTE — accurate; "copy all source branch
  tessellae to target, source='merge:{name}'". Confirms actor clobber
  is by design-omission not oversight.
- 1178 "Does not use conflict_resolutions — force flag is the only
  override" — accurate (5964-5970). Per-res resolution never built
  (matches our decision 9 park).
- 1222 "describe_system includes branches:{current,total,active}" — the
  surface counts; our tabula RAMI section (decision 12) is the analog.

## SPEC V1 BREAKS
B1: **`kernel.currentBranch` DOES NOT EXIST.** Refactored to
    `kernel.currentContext.branchId` (212, 8917-8923); roadmap session
    model partly built. The cited regret is stale. Our "branch as pure
    parameter" is stricter than the current oracle, which still holds a
    session-scoped mutable branchId. Implementer: do not port
    currentContext.branchId.
B2: **Nested-branch reads are LEAKY in the oracle.**
    `_collectBranchTessellae` (708-711) applies NO seq ceiling to
    intermediate parent branches → a grandchild sees its parent's
    post-fork writes. Our §III per-link `seq<=p` ceiling is a CORRECTION.
    Spec-v2 must state clearly: cap each parent link at the fork point of
    the NEXT branch down the chain (leaf gets no cap). This is the one
    place our read path must be *more* careful than the oracle, not less.
B3: **Merge is NOT atomic in the oracle.** The db.transaction wraps only
    the copy loop (5978-5986); cache invalidation, index rebuild, and the
    source→merged status transition run outside it (5988-5998). Our K1
    crash-law demands copy + status + probe-sweep in ONE scrinium. Do not
    port the oracle's narrow txn.
B4: **Merge DESTROYS actor attribution.** The single `tessella.source`
    column is overwritten with `merge:${source}` (5982), losing the
    original `actor:X`. gesta's separate actor/origo/genus_eventus fields
    let us preserve actor+datum+genus_eventus and rewrite ONLY origo —
    decision 5's "preserved" claim is CORRECT and is an improvement the
    oracle structurally cannot make.
B5: **Merge does NOT advance processes.** Oracle has no post-merge
    process hook (advancement is completeTask-only, K3 §3). Decision 5's
    advancement-probe sweep over copied STATUS events is NET-NEW — a
    generalization on top of K3's B2, not a port. Budget it as new build,
    not extraction.
B6: **Oracle does NOT refuse genus-on-branch.** Decision 8 says
    "generis events off-trunk REFUSED, mechanical." The oracle has no
    such refusal — it simply PINS genus reads to branch_id:"main" (plan
    1141: getGenusDef/getActionDef/getProcessDef pinned to main). A genus
    "defined on a branch" would write a branch tessella that the pinned
    reader never sees (silently invisible), not an error. Our mechanical
    refusal is stricter/cleaner; own it as a deliberate divergence.
B7: **"main" trunk is a real branch res in the oracle; our trunk is the
    `''` sentinel with no res.** Bootstrap creates a "main" branch entity
    (1304-1306) and listBranches returns it. gesta decision 6: trunk =
    sentinel `''` rendered "truncus", NOT a ramus res. Spec-v2 must
    decide whether `gesta_ramos_enumerare` surfaces truncus (oracle does)
    — and none of the ramus-lifecycle machinery (creare/fundere/abicere)
    may ever target the sentinel.
B8: **Discarded branches stay physically readable in the oracle.**
    discardBranch only flips status + resets current branch (6061-6064);
    `_collectBranchTessellae` never checks status, so materialize on a
    discarded branch_id still folds. "Projections stop following" is
    enforced only at switchBranch (5874). Decision 6/discard says events
    remain + chain reads stop — spec-v2 must decide WHERE we enforce the
    stop (at read, unlike the oracle) or accept oracle-parity
    (archaeology-readable).
B9: **`_findBranchByName` is O(n) per chain link.** Called inside the
    chain walk (688) on every non-main materialize; each call scans+folds
    all branch res (5811-5825). Our lazy read must resolve the parens
    chain from the ramus res state directly and walk it once per read,
    not re-scan the whole ramus population per link.

LOAD-BEARING FOR E2: (a) the SIX eager per-branch index families (§9)
are exactly what decision 3/12 declines — E2 confirms gesta's trunk-only
K1 folds re-project correctly on merge-copied events so §7/§8's entire
reconciliation layer has no gesta analog. (b) The merge-atomicity gap
(B3) is the sharpest interaction with K1 crash-law: E2 must specify the
single-transaction shape (copy + status + plicare + sweep) with no
nested BEGIN. (c) Actor preservation (B4) depends on gesta keeping actor
in its own field through the copy — E2 verifies the copy path carries
actor/datum/genus_eventus and rewrites only origo.
