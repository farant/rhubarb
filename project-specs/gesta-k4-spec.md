# GESTA K4 SPEC v1 — RAMI (branches + merge) [SUPERSEDED by gesta-k4-spec-v2.md]

2026-07-16. **EXPLORER TARGET — scope document for E1/E2/E3, NOT the
build source.** Spec-v2 (post-explorers) will be the implementation
source. Decisions: gesta-k4-interrogatio.md (14, all recommendations
accepted). Oracle strata: smaragda-plan.md Demo 11 (intent + as-built
notes), smaragda.ts branch module (93-95 exports; branch_id plumbing
273-423; createRes 510+), docs/foundation-roadmap.md (regrets), live
db (E3's).

## I. WHAT K4 IS

Event-log branches: parallel timelines of project state inside the
one store. Fork from a seq point, write judged events on the branch
without touching the trunk, compare, then merge (replay-on-merge) or
discard. Branch is a PARAMETER on every surface — never resident
state (the roadmap regret, healed). Trunk = branch_id '' (unchanged
across all live events), rendered "truncus".

## II. DECISIONS (interview, 4 rounds)

| # | Decision |
|---|----------|
| 1 | Park split: K4 rami (retitled in place, ritual binding holds) / K5 palatium (new park) |
| 2 | Rami only; parametric everywhere; sessio/actor entities = named park |
| 3 | Folds: TRUNK EAGER (untouched), RAMI LAZY (parent-chain query per read); no cache v1 |
| 4 | Merge = replay-on-merge (copies, new seq, origo "merge:<ramus>") |
| 5 | Merged events = raw copies + annales + ONE plicare + advancement-probe sweep over copied STATUS events |
| 6 | Branch = res of genus "ramus" ON TRUNK {titulus, parens, punctum; activus->fusus|abiectus}; trunk sentinel '' stays |
| 7 | Engine trunk-only v1 (branch writes never trigger; processes = trunk citizens) |
| 8 | Genera trunk-only (generis events off-trunk REFUSED, mechanical) |
| 9 | Conflict = same res touched both sides after punctum; report both states; force (vis) only |
| 10 | Surface: new tool ramus {actus: creare|enumerare|comparare|fundere|abicere} + optional 'ramus' param on addere/gerere/res |
| 11 | Annales: one interleaved jsonl, branch_id per line (format ready since K1); verificare untouched |
| 12 | FTS/census/tabula trunk-only; tabula gains RAMI section |
| 13 | Custodia judges on BRANCH state (lazy per-res fold on write path); notae travel at merge |
| 14 | Full ritual: this spec -> 3 explorers -> spec-v2 |

## III. KERNEL DESIGN (sketch — E2 audits every claim)

### Schema
- tessellae.branch_id EXISTS (K1) — starts being written with real
  values. res/genera/membra/consumptores/sordidae stay single-
  timeline (trunk projections). NO new tables except none — ramus
  metadata is ordinary res on trunk. New INDEX probably:
  idx_tessellae_ramus ON tessellae(branch_id, seq) (oracle had
  replay+sync indexes; E2 sizes).

### The read path (rami lazy)
- _catena_rami(m, ramus): walk parens chain from ramus res state ->
  [(truncus, punctum_1), (ramus_a, punctum_2), ..., ramus]. Nested
  branches per oracle.
- _res_in_ramo_capere(m, res_id, ramus): SELECT events for res_id
  along the chain — (branch_id='' AND seq<=p1) OR (branch_id=a AND
  seq<=p2) OR (branch_id=ramus) — ORDER BY seq, fold in memory.
- REFACTOR REQUIRED: _rei_applicare mutates the res TABLE. The fold
  logic must decompose into pure state-transform (JSON in/out) +
  table-write wrapper, so the same reducer serves eager trunk folds
  AND in-memory branch folds. Same shape as chunk A's praeparare/
  validare/inserere decomposition. E2: exact seam.
- Membra/status/genus projections for branch validation come from
  the same in-memory fold (the branch state object carries membra —
  K2 reducer already folds them into state).

### The write path (branch writes judged)
- Core: ramus param threaded through _fascis_scribere/_eventum_*
  (NIHIL/'' = truncus, today's behavior byte-identical — the G9-bar
  pattern again).
- Validation reads swap _res_capere -> _res_in_ramo_capere when
  ramus given. Custodia notae land ON the branch (same bundle).
- generis events + ramus -> _fractum (decision 8).
- creatio duplicata: judged against branch state (a res created on
  trunk AFTER punctum is invisible to the branch — honest fork
  semantics; E2 verifies the dup-check reads through the chain).
- Eager folds SKIP branch events: _consumptorem_plicare gets WHERE
  branch_id = '' (trunk consumers never see branch rows). HWM
  semantics with interleaved seq: hwm advances past branch seqs
  without applying them — E2 checks this carefully (skipped seq
  must still advance hwm or refold loops).
- Sordidae/FTS: branch writes add NO sordidae (trunk index only).
- Public API shape (E2 evaluates): parallel entry points
  (gesta_scribere_ramo, gesta_res_datum_ramo, ...) vs ramus param
  added to existing signatures (call-site churn like the actor
  param — mechanical but wide). v1 instinct: param on the few
  surfaces the tenant needs; kernel internals take ramus
  throughout.

### Ramus lifecycle (kernel ops)
- gesta_ramum_creare(m, titulus, parens, piscina, out): punctum =
  gesta_seq_ultima(m); creatio res genus ramus ON TRUNK.
- gesta_ramos_enumerare: rami by status.
- gesta_confligentia(m, ramus, piscina): res_ids with events on
  both (trunk after punctum) and (ramus); per conflict, both
  materialized states (ConflictInfo).
- gesta_ramum_fundere(m, ramus, vis, piscina, out): conflicts &&
  !vis -> refuse w/ report. Else ONE transaction: copy ramus events
  -> trunk (new seq, branch_id '', origo "merge:<titulus>", actor/
  datum/genus_eventus preserved, annales line per copy, K1
  crash-law order), status ramus -> fusus (also in txn? or after —
  E2: the status event itself is a trunk write; sequencing). Then
  plicare once + probe sweep over copied status events.
- gesta_ramum_abicere: status -> abiectus; events remain, chain
  reads stop (only activus rami readable? abiectus readable for
  archaeology — decide in v2).

## IV. TENANT (tabularium.c sketch)

- Seed v4: + genus ramus (SEMINA_NUMERUS IX->X, non-board scope).
- Tool ramus: {actus, titulus?, parens?, vis?, res?}; fundere
  reports copied count or conflict list; comparare {res, ramus} ->
  both states.
- Optional 'ramus' param on addere/gerere/res: writes/reads through
  branch path. quaerere/census unchanged (trunk).
- Tabula: RAMI section (activi + puncta + event counts).
- Doctrine v5: branch grammar + "truncus = veritas, rami =
  hypothetica" + merge/conflict behavior.

## V. GOLDENS (sketch — v2 finalizes)

Fork isolation (trunk blind to branch, branch sees trunk-to-punctum
only); post-punctum trunk res invisible to branch; nested chains;
branch write judged on branch state (custodia on branch); generis
refuse; merge happy (copies + annales + folds + origo); merge
conflict refuse + vis override; merged status advances a trunk
process (probe sweep); discard; verificare TENET through all of it;
replicare/restitutio rebuild branch state identically (lazy folds
deterministic); G9-bar: all existing suites byte-identical when no
ramus given; annales line branch_id correctness; tabula RAMI;
MCP ramus tool + params.

## VI. EXPLORER BRIEFS

**E1 — oracle deep-read** (smaragda.ts): branch module internals
verbatim (createBranch/switchBranch/mergeBranch/detectConflicts/
compareBranches/discardBranch, _collectBranchTessellae chain-walk
SQL, _rebuildIndexesForMerge, merge cache invalidation); the
WORKSPACES layer (Demos 13.3/13.4 — what sits on branches vs beside
them; does any workspace machinery leak into branch semantics);
per-branch FTS/meta/dirty plumbing COST INVENTORY (what we're
declining); how relationship_member/exchange indexes handle
branch_id (their per-branch eager indexes vs our lazy choice —
what breaks that they fixed); branch entities pinned to main
mechanics. Report: verbatim file+line map, breaks vs our decisions.

**E2 — our seams** (gesta.c/h, tabularium.c): every SELECT/INSERT
that silently assumes one timeline (inventory with line numbers);
the _rei_applicare pure-fold decomposition (exact refactor seam,
what chunk-A-style split it needs, how K2 membra + K3 cursus
branches fold in-memory); HWM-skips-branch-seqs semantics
(consumptores WHERE branch_id='' — does hwm advance correctly past
foreign rows); validation-read swap points (_res_capere call sites
that must become branch-aware on the write path); fascis × ramus;
annales append branch_id (currently hardcoded ''); merge
transaction shape under scrinium (no nested BEGIN — copy txn +
plicare + sweep sequencing); API shape recommendation (param vs
parallel entry points, call-site counts); chunk boundaries with
a G9-style inertness bar per chunk.

**E3 — usage + intent mining** (live smaragda db + docs): how many
branches ever created, lifetimes, merged vs discarded vs zombie;
what was ON them (genera of branched tessellae — did anyone branch
anything besides entities); workspaces-vs-branches actual usage
(did workspaces eat branches' use cases?); conflict/force frequency;
plan-vs-roadmap diff on branches (what Demo 11 promised vs what the
roadmap regrets — beyond currentBranch); anything in feedback.md
about branch UX. Ranked lessons like K3's E3.

## VII. CHUNKS (sketch)

A — fold decomposition + branch read path + branch write path
    (kernel; G9 bar: no-ramus behavior byte-identical).
B — ramus lifecycle: creare/enumerare/confligentia/fundere/abicere
    + probe sweep (kernel; inert without rami).
C — tenant: seed v4, ramus tool + params, tabula RAMI, doctrine,
    live deploy, seal via the RUNNING ritual (porta-sigilli is
    already waiting on frusta + exploratores).
