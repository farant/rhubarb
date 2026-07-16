# K4 EXPLORATIO 3 — usage + intent mining (rami: were branches real?)
# Relatio exploratoris E3, 2026-07-16. Fons pro spec-v2.
# Reality-check charter: K3's E3 killed task-claiming with 0/591.
# This one is worse. The data says branches were DEMO-WARE.

Two smaragda deployments mined:
- HOSTED (claude.ai MCP, live, production reality): Feb–Mar era —
  branches + workspaces + palace. 930+ entities, 95 genera, months
  of real research use. THIS is where real work happened.
- LOCAL (bun-libraries repo, Feb–Apr, 180 commits): the same kernel
  plus the April modules (coordination / projects / sessions-actors).
  Its smaragda.db is a small Feb dev snapshot; sessions.db is empty.
Both tell the same story from different angles.

---

## 1. THE BRANCH CENSUS (live hosted system)

| branch | parent | fork@seq | status | lifetime | real content |
|--------|--------|----------|--------|----------|--------------|
| main | "" | 0 | active | (trunk sentinel) | all work |
| experiment | main | 568 | **merged** | **5m 23s** | 2 events (seq 574-575) |
| palace-experiment | main | 7812 | **active** (zombie) | abandoned at birth | 0 events |

- **2 real branches ever created** in a 7800+-seq, 930-entity system.
- experiment: created 2026-02-10 17:03:36, merged 17:08:58. Its ONLY
  content was seqs 574-575 — the demo renaming a Book chapter.
- palace-experiment: created 2026-02-13 02:21:53, still `active`.
  Never merged, never discarded, zero follow-up. A pure zombie fork.
- **0 branches ever discarded.** The discard path never ran in prod.
- **0 conflicts ever detected; force (vis) never used.** The one
  merge had no overlapping writes (branch-only touched Chapter 1).

## 2. experiment IS THE DEMO SCRIPT (intent = reality, literally)

smaragda-plan.md Demo 11 test scenario (line 1211-1222):
> "Create a branch called experiment" / "Rename Chapter 1 to Prologue"
> (on branch) / "Merge experiment into main"

The live `experiment` branch — same name, 2 events renaming a Book
attribute, merged 5 minutes after creation — is that exact demo run,
executed once and never repeated. The single merged branch in the
entire production history is the demo walking through itself.
palace-experiment is a second "experiment" someone spun up during
palace work in Feb and abandoned the same second it was born.

## 3. THE tessella DISTRIBUTION (local Feb snapshot)

| branch_id | tessellae | distinct res |
|-----------|-----------|--------------|
| main | 358 | 32 |
| (any other) | **0** | **0** |

100% of events on trunk. A branch res existed (genus 004, created
2026-02-13) — **branch machinery bootstrapped, zero events ever
written on a non-main branch.** The 0/591 shape again: the primitive
runs, nobody drives it.

## 4. WORKSPACES ATE THE USE CASE (12 vs 2)

| axis | instances | status | scale |
|------|-----------|--------|-------|
| **branches** | 2 real | 1 merged (demo), 1 zombie | ~2 real events |
| **workspaces** | **12** | all active | 4 → 1415 entities each (~2,300 total) |

Live workspaces: Fil-C Research (75), Paradigm Survey (212),
smaragda-dev (70), smaragda-user (24), Memory Palace Design (69),
Theorycrafting (96), Trade Networks (285), claude-code-smaragda-dev
(55), Exploration Fawcett (4), rhubarb (1415), Genius Loci (44).

The plan (line 2108) claimed "branches and workspaces are independent
axes." In production, one axis carried ALL the parallel-organization
load and the other carried a demo. Workspaces won because the real
need was **durable parallel partitions** (separate research projects
that never recombine), NOT **speculative fork-then-merge**. Nobody
wanted to try-then-merge; everyone wanted to keep N projects apart.

Decisive detail: Demo 13.4 solved the multi-agent-concurrency need —
literally *"multiple agents connect concurrently, each needs its own
context"*, our K4 charter's scenario — with **per-session workspaces**
(`Map<Mcp-Session-Id, SessionContext>`), while branches were left on
the process-global `kernel.currentBranch`. Workspaces got the good
per-session design; branches got the concurrency-unsafe one and never
got fixed. That is where the builders' actual investment went.

## 5. feedback.md: BRANCHES MENTIONED ZERO TIMES

feedback.md is "compiled from a full day of building and navigating"
a real workspace. It asks for temporal anchors, token conservation,
and palace fixes. Branches do not appear once. After a full day of
production use, branches did not register as a thing worth a single
line of feedback — positive or negative. Invisible.

## 6. THE ORACLE CONVERGED ON OUR DECISION (and deleted the rest)

foundation-roadmap.md priority #1 and sessions-actors-design.md
line 11 name the regret as a **real concurrency bug**, not theory:
> "kernel.currentBranch, currentWorkspace, and _currentSessionId are
> process-level globals mutated per request. Concurrent requests from
> different agents can interfere... entities get created in the wrong
> workspace, palace operations target the wrong branch."

The fix they shipped (git 53b0372): *"remove legacy SessionContext,
sessions.db, and kernel.currentBranch/currentWorkspace."* They
**deleted** the mutable branch state. K4 decision 2 (branch = a
PARAMETER, never resident state) is exactly where the oracle ended up
after feeling the pain. We start where they finished. STRONG CONFIRM.

And the sessions-actors module that replaced it? **sessions.db has 0
rows.** The fix for the regret was built, then the whole SessionContext
was ripped back out; sessions/actors never got a real customer. This
validates decision 2's other half: sessio/actor entities = park, no
customer for them.

## 7. BRANCHES DEMOTED OUT OF THE MENTAL MODEL

The April coordination-vision — the doc explicitly designing "what
makes smaragda viable for small-group agent-mediated work" (our K4
charter's exact scenario) — names **four orientation axes: palace,
projects, coordination, session-plans.** Branches are not one of them.
When the builders sat down to design multi-agent speculative work,
they reached for projects + coordination + sessions, NOT branches.
Branches survive in the April code only as a **plumbing invariant**
("all writes forward branchId; all queries scope to branch") — never
a workflow anyone drives.

## 8. THE PLUMBING TAX (what branches actually cost)

branch_id-everywhere is a permanent tax on every future module,
paid whether or not branches are used. It bit them repeatedly:
- git bb05561: *"forward branch_id to all appendTessella calls"* —
  a 13-site fixup in the coordination module.
- git 778e460: *"add branch_id filter to Notice transition UPDATEs"* —
  cross-branch corruption fix.
- git 3e834e2, bc9ef1e: more branch-correctness fixes in new modules.
- projects-module-design line 43: every new module needs a `merge`
  hook to copy its index rows on branch merge.

Across 180 commits, **not one adds a branch FEATURE after the Feb
initial build.** Every post-Feb branch commit is maintenance:
plumbing bug fixes, the context refactor, or deleting the regret.
Branches were feature-frozen at birth and taxed the codebase for two
months of module work that used them zero times. (Telling coda: the
builders used a *git* branch, `fts5-search`, for their own real
speculative work — not their own branch primitive.)

## 9. THE HONEST QUESTION

Charter (visio §VI): branches serve "agent sessions doing speculative
work." **Did the oracle's branches ever serve that? No.** The one
branch that reached a terminus was the demo; the other is a zombie;
production feedback never mentions them; the multi-agent design that
followed routed entirely around them; the concurrency use case went
to workspaces. In the closest simulation of our scenario (the April
four-actor coordination roleplays), no actor ever organically reached
for a branch — it appears only as "tests branch_id propagation."

Our context is if anything MORE skeptical than the oracle's: the
oracle at least ran 12 parallel research workspaces; gesta is a
single-agent stdio resident with ONE project ledger. The same
single-agent shape that left the oracle's branches unused is ours.

What the data says to build: if anything, the **ephemeral sandbox**
(fork → try → **discard**), which is the one genuinely compelling
branch use the plan ever sketched (Demo 15 tutorial: "a branch is
created with sample data... the branch is discarded" — UNBUILT). The
merge machinery (replay, conflict detection, force) is the MOST
speculative, LEAST-validated part: exercised once, on a conflict-free
demo, never again. K4 currently invests most in exactly that.

---

## 10. LECTIONES PRO PORTU (ranked)

1. **Branches were demo-ware.** 2 real branches / 7800 seqs; one IS
   the literal Demo 11 script, one is a zombie; 0 discarded; 0
   conflicts; feedback names them 0 times. Weigh whether K4 ships at
   all, or ships as the smallest possible sandbox. This is the E3
   verdict, same weight K3's 0/591 carried.
2. **Workspaces won the parallel-organization need** (12 heavily-used
   vs 2 demo). The real need was durable partitions, not fork-merge.
   gesta has neither workspaces nor a customer for parallelism — even
   thinner ground than the oracle had.
3. **Decision 2 is VINDICATED, hard.** The oracle hit a real
   concurrency bug from mutable branch state and *deleted* it
   (git 53b0372). branch-as-parameter is where they ended up. Keep it;
   never add resident branch state, not even "just the current one."
4. **Sessio/actor = park is VINDICATED.** sessions.db = 0 rows; the
   module was built then ripped out. Build zero session/actor
   machinery.
5. **Trunk-only indexes (decision 12) is the RIGHT call.** The oracle
   threaded branch_id through every index table and paid a recurring
   corruption-bug tax (bb05561 13-site, 778e460). Declining that tax
   (lazy rami, trunk-only FTS/census/tabula) avoids their exact
   wounds. Do not add branch_id to any eager index.
6. **Merge machinery is the least-validated part** — conflict
   detection and force ran ZERO times in production (the one merge
   was conflict-free). Decision 9 (conflicts + vis) is the highest-
   speculation surface. Candidate to defer/park until a real conflict
   exists to design against.
7. **If branches serve anything, it's DISCARD, not merge.** The only
   compelling sketched use (tutorial sandbox) was fork→discard, and it
   was never even built. gesta's own explorer workflow is discard-
   heavy (most spec explorations are thrown away). Make discard cheap
   and first-class; treat merge as the rarer path, not the centerpiece.
8. **branch_id plumbing is a PERMANENT tax on all future kernel work.**
   Every module thereafter must forward branchId and scope queries or
   corrupt across branches. Budget K4's cost as ongoing, not one-time,
   and keep the branch-aware surface as NARROW as decisions 7-8-12
   already make it.
9. **The oracle's own builders used git, not smaragda branches, for
   real speculative work.** When the primitive competes with a tool
   the user already trusts for the same job, it loses. Ask what a
   gesta branch does that a scratch tabularium query or a git branch
   of annales/tabularium.jsonl doesn't.
10. **Branch genus on trunk + '' sentinel (decision 6) matches oracle
    mechanics 1:1** (branch entities always on main; trunk = literal
    'main' there, '' for us). Mechanically sound; no surprises. Ship
    the mechanics, question the scope.

---

## SPEC V1: BREAKS & CONFIRMS

**No hard mechanical BREAK.** The 14 decisions are well-aligned with
what the oracle actually learned — several are directly vindicated by
the pain the oracle felt and the fixes it shipped. The usage data does
not contradict a mechanism; it challenges the SCOPE.

CONFIRMS (usage data backs the decision):
- D2 (parametric, no resident state) — the oracle's deleted regret.
- D2 (sessio/actor = park) — sessions.db 0 rows, module ripped out.
- D6 (ramus genus on trunk, '' sentinel) — matches oracle 1:1.
- D12 (trunk-only indexes) — avoids the oracle's recurring branch_id
  corruption tax; the single most valuable "we decline this" call.
- D4 (replay-on-merge) — the oracle's own as-built divergence, for
  the same simple-read-path reason.

CHARTER CHALLENGES (spec-v2 author should weigh, not silently adopt):
- **C1 — ship-at-all / minimal scope.** The charter use case ("agent
  sessions doing speculative work") never materialized in the oracle
  and gesta's single-agent shape is thinner ground still. Recommend
  spec-v2 open with an explicit ship/defer decision, and if ship,
  scope to the smallest sandbox (fork + read-isolation + discard),
  with merge + conflict + force explicitly deferred to a later, real-
  demand milestone.
- **C2 — merge is over-weighted, discard is under-weighted.** Decisions
  4-5-9 invest heavily in the merge path (replay, probe sweep, conflict
  detection, force) — the part with ZERO production validation. The
  discard path — the one with any evidence of demand — is decision 6's
  afterthought ("status abiectus, events remain"). Recommend flipping
  the investment: make discard first-class and cheap; make merge the
  minimal viable copy-and-annales, defer conflict/force to a park.
- **C3 — the competing-tool question.** gesta already has the
  tabularium store (parca/quaestiones for speculative capture) and git
  over annales/tabularium.jsonl (real branchable history). Spec-v2
  should state, in one sentence, what a ramus does that those two do
  not — or accept that the honest answer is "ritual completeness,"
  which is a fine reason to build it small and a bad reason to build
  it big.
