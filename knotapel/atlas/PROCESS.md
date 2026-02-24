# Atlas Defrag Process

The atlas is an organized synthesis of all knotapel research. It is regenerated periodically as demos accumulate, ensuring the research remains navigable without re-reading every demo from scratch.

## Principles

- **Readonly outside atlas/**: Defrag agents READ from demo dirs, proofs/, explorer's log, research/, future-demos/. They WRITE only to atlas/. Nothing outside atlas/ is ever modified.
- **Inventory vs Synthesis**: Some artifacts are pure extraction ("what do we have"), others are interpretation ("what does it mean"). Both are necessary.
- **Repeatable**: The process can be re-run. Incremental updates are preferred (check demo-index.md for what's already cataloged), but full rebuilds are fine too.
- **Parallelizable**: Phases 1 and 2 split across agents. Phases 3-5 are sequential.

## Directory Structure

```
atlas/
  PROCESS.md                    <- this file
  briefing.md                   <- fast onboarding (~50 lines)

  inventory/                    <- "what do we have"
    demo-index.md               <- every demo, standardized entry
    proofs-index.md             <- formal proofs with status and cross-references
    theorems.md                 <- all proven/conjectured results + status
    data-tables.md              <- key numerical tables
    code-assets.md              <- reusable code patterns across demos
    floating-point-audit.md     <- FP usage classification per demo

  synthesis/                    <- "what does it mean"
    narrative.md                <- the research story arc
    four-lenses.md              <- rep theory / coding / QM / approx theory
    connections.md              <- cross-demo themes, how things feed each other
    novelty.md                  <- what's known vs novel vs in between
    vision-alignment.md         <- how current work relates to long-term vision

  research/                     <- external sources
    literature-index.md         <- all papers, one-line each, tagged
    five-pillars.md             <- Habiro / Nazer-Gastpar / Abramsky / Aizenberg / Reservoir Computing
    key-sources.md              <- particularly valuable papers with notes

  planning/                     <- todo lists + direction
    future-demos.md             <- demo backlog with dependencies and priorities
    research-questions.md       <- open theoretical questions
    next-priorities.md          <- what to work on next
    vision.md                   <- long-term directions, application vision, architectural ideas
```

## Sources

The defrag reads from these directories (all readonly during defrag):

- `demo_*/` — Demo source code and findings
- `proofs/` — Formal mathematical proofs (*.md). Each proof upgrades a result from VERIFIED/CONJECTURED to PROVEN in the atlas. Proofs should be indexed in `inventory/proofs-index.md` and propagated to theorems.md, narrative.md, novelty.md, and next-priorities.md.
- `future-demos/` — Proposed demo ideas with dependencies
- `research/` — External literature notes (read by Phase 2 literature agent)
- `explorers-log.md` — Running research log

## Phases

### Phase 1: Inventory Extraction (parallelizable)

**Input**: Each demo's main.c, findings.md, README.md, and any other .md files in the demo dir. Also: new files in `proofs/`.
**Output**: Entries appended to `inventory/demo-index.md`. New proofs indexed in `inventory/proofs-index.md`.

Each demo gets a standardized entry:

```markdown
## Demo NN: <title>

- **Status**: COMPLETE | IN PROGRESS | PLANNED
- **File**: `demo_NN_name/main.c` (~lines)
- **Tests**: X/X pass
- **Depends on**: demo numbers this builds on
- **Feeds into**: demo numbers that build on this

### Headline
One sentence: the key result.

### Key Results
- Bullet list of specific findings, data points, confirmed/refuted hypotheses

### Theorems/Conjectures
- Named results with status (PROVEN / CONJECTURED / REFUTED)

### Data
- Key numerical values, tables, or catalogs produced

### Code Assets
- Reusable code patterns introduced (e.g., "Cyc8 exact arithmetic", "planar matching enumeration")

### Literature Touched
- Papers referenced or discovered during this demo

### Open Questions
- Questions raised but not answered by this demo
```

**Parallelization**: Use a **worker pool** pattern: 4 persistent agents, each assigned 1 demo at a time. When an agent finishes, feed it the next unprocessed demo. Each agent writes to `inventory/entries/demo_NN.md`. Team lead merges all entries into `demo-index.md` when complete.

Why worker pool, not fixed batches: Agent context windows cannot handle 10+ demos in one session. One demo per assignment keeps each agent focused and prevents context blowouts. 4 workers gives good throughput without overwhelming coordination.

**Spot check**: After the first 4-8 entries are written, read 2-3 at random to verify quality before processing all remaining demos.

### Phase 2: Data Consolidation (parallelizable)

**Input**: demo-index.md from Phase 1 + original demo files as needed.
**Output**: Four files, one per agent.

- Agent A -> `inventory/theorems.md`: Extract all theorems/conjectures from demo-index entries, organize by topic, track status.
- Agent B -> `inventory/data-tables.md`: Extract all key numerical tables (axiality hierarchy, radical dimensions, DKC landscape matrix, catalog sizes, compression ratios, etc.)
- Agent C -> `research/literature-index.md` + `research/five-pillars.md` + `research/key-sources.md`: Consolidate all literature references across demos, deduplicate, tag.
- Agent D -> `inventory/code-assets.md`: Catalog reusable code patterns with which demo introduced them and which demos reuse them.
- Agent E -> `inventory/floating-point-audit.md`: For each new/modified demo, read main.c and classify its floating-point usage (INTEGER, DISPLAY-ONLY, VALIDATION, EXACT-CORE, GEOMETRIC, STATISTICAL, or MIXED). Update the audit table, summary counts, and regime descriptions. This tracks the boundary between exact and FP computation across the codebase.

**IMPORTANT — Assign tasks explicitly per agent.** Do NOT use a shared task list that agents can self-assign from. In the initial run, one agent finished its own task and then grabbed another agent's task, producing duplicate files that had to be manually merged. Set explicit ownership before launching.

### Phase 3: Synthesis (agent drafts + conversational review)

**Input**: All inventory and research files from Phases 1-2.
**Output**: synthesis/ directory files.

Effective workflow: spawn 4-5 agents in parallel to draft initial versions (they do the heavy reading of inventory material), then review each draft conversationally with the human. The agents handle extraction; the human handles judgment. Expect minor edits, not rewrites — if the inventory phase was thorough, the synthesis drafts are usually solid.

- `narrative.md`: The research story arc, organized by conceptual progression not chronology
- `four-lenses.md`: Current state of each lens (rep theory, coding, QM, approx theory)
- `connections.md`: Cross-demo themes and how things feed each other
- `novelty.md`: What's known in the literature, what's novel, what's in between
- `vision-alignment.md`: How current work relates to the long-term vision (`planning/vision.md`). This bridges synthesis (backward-looking) and planning (forward-looking). Reads the other four synthesis docs + the vision doc and assesses: progress toward vision, new opportunities, course corrections, shortest paths to vision milestones, and recommended next investigations.

### Phase 4: Planning (sequential)

**Input**: All above + existing future-demos/ folder.
**Output**: planning/ directory files.

- `future-demos.md`: Consolidated from future-demos/ folder + new ideas from synthesis, with dependencies and priorities
- `research-questions.md`: All open questions extracted from demos, organized by topic
- `next-priorities.md`: What to work on next, informed by the synthesis
- `vision.md`: Review and update long-term directions. This file captures high-level vision discussions (application directions, architectural ideas, the hybrid LLM thesis, etc.) that are above the level of individual demos. Append new ideas during defrag; don't overwrite existing ones unless they're superseded.

### Phase 5: Briefing (sequential)

**Input**: Everything above.
**Output**: `briefing.md` (~50 lines).

The fast-onboarding document. Read this on every cold start. Contains:
- One-paragraph story
- Key results table (5-10 rows)
- Current frontier (what we just proved, what's next)
- Pointers to deeper reading

## Phase 0: Change Detection (always run first)

Before any defrag work, run a git-based change scan to identify what needs updating:

```bash
# 1. What's changed since last defrag commit?
git diff --stat <last-defrag-commit>..HEAD -- demo_* proofs/ future-demos/ explorers-log.md

# 2. What's new/untracked?
git status -s -- demo_* proofs/ future-demos/

# 3. Recent commits touching demo/proof dirs (human-readable)
git log --oneline <last-defrag-commit>..HEAD -- demo_* proofs/
```

This produces three categories of work:

| Category | Example | Action |
|----------|---------|--------|
| **New demo dirs** | `?? demo_63_four_input/` | Phase 1: create new entry |
| **New non-demo dirs** | `?? proofs/something.md` | Phase 1: index in appropriate file |
| **Modified existing demos** | `M demo_51_radical_anatomy/main.c` | Phase 1: RE-READ and update existing entry |
| **No changes** | (nothing) | Skip defrag |

The "modified existing demo" case is the one the old process missed — if someone adds tests, fixes bugs, or extends results in an already-cataloged demo, the entry needs updating. The git diff tells you exactly which entries are stale.

**Recording the baseline:** After each defrag, note the HEAD commit hash in the defrag history table. This becomes `<last-defrag-commit>` for the next run.

## Re-running the Defrag

1. **Run Phase 0** (change detection) to identify new, modified, and untracked items.
2. Run Phase 1 on NEW items (create entries) and MODIFIED items (update existing entries).
3. Re-run Phase 2 (full rebuild — these are fast).
4. Re-run Phase 3-5 (synthesis always needs refresh with new data).
5. Update defrag history with date, commit hash, and notes.

## Defrag History

| Date | Commit | Demos Covered | Notes |
|------|--------|---------------|-------|
| 2026-02-20 | — | 01-62 | Initial defrag. Worker pool for Phase 1 (4 agents, 1 demo each). Phase 2 duplication issue fixed by merging. Phase 3 agent drafts + review worked well. |
| 2026-02-20 | — | +38,39,60 | Incremental defrag. 3 missed demos cataloged. Phase 1: 3 parallel agents. Phase 2: 4 parallel agents (incremental updates). Phase 3-5: targeted edits (not full rebuilds). ell=7 moved from "next" to "done" across all docs. |
| 2026-02-20 | `1548c73` | +proofs/ | First proof ingestion. Created proofs-index.md. Radical dimension formula (2ℓ−3) upgraded from VERIFIED→PROVEN. D65 marked partially done. Phase 0 (git change detection) added to process. |
| 2026-02-20 | `1548c73` | +D63, D39 updated, +P02, +P03 | Major incremental defrag. D63 (Angular Anatomy): parity ceiling n=5, 4/5-input parity, OM classification, triskelion k=2n FALSIFIED, universal k=2M−1. D39 Parts G-J: three Gram forms, Markov RT truncation, next-level radical. Two new proofs indexed (P02, P03). 51 demos total. All phases parallel where possible. |
| 2026-02-21 | `76b0f62` | +D64-D71 (8 new demos) | Largest incremental defrag. Two arcs: wall (D64-65: matroid funnel, self-doubling impossibility, algebra-independence of ceiling, two-layer wall, generalized XOR6 at k=24) and quaternionic-spherical (D66-71: 24-cell, S² sweet spot, Hopf inertness, 13-direction Voronoi, stereographic visualization, direction nesting, knots-as-music, 13=13 bandwidth theorem). 59 demos, 86 literature refs. Paper 5 proposed. |
| 2026-02-21 | `1548c73` | +D72-D82 (11 new demos) | Two arcs: characterization (D72-75: anti-correlation/t-design, eigenvector automaton, bracket/cell incomparability 119 classes, binocular DKC angle-subsumes-cell) and scaling (D76-82: XOR8=6 via S¹×S² activation, ζ₈ finite ceiling at XOR10, ζ₁₂ breaks wall XOR12, only ζ₄/ζ₈ finite ADE, 0.62 XOR/doubling log law, depth law max_xor≈depth+6). FP audit added as Phase 2 Agent E. 70 demos, 101 literature refs. Paper 6 proposed. |
| 2026-02-21 | `8e5a93a` | +D83-D84 (2 new demos) | Decomposition arc: D83 (framing = +2 XOR, writhe/depth independent r=0.14, resource decomposition lattice+depth+writhe, TQFT anomaly = computation) and D84 (null indispensability, 9/24 bracket-null at ζ₈, 6 exclusive S² directions, LCFT Jordan-cell interpretation, RC Pillar 5 connection, null fraction dilution law). Phase 1: 2 parallel agents with explorer's log as source. Phase 2: 5 parallel agents. Phase 3: 4 parallel agents. Phase 4-5: 4 parallel + sequential. 72 demos, 108 literature refs. Paper 6 scope expanded D72-D84. |
| 2026-02-24 | `1050d75` | +D85-D92 (8 new demos), +P04 | Largest defrag. Three arcs: indecomposability (D85: b=-5/8 exact at TL₄, D86: single P₀₀ diverges — negative result), null regime (D87: nulls dispensable at ζ₁₂, D88: anti-correlation mechanism, two-role directions, k-ladder resilience), depth law mechanism (D89: 8 hypotheses 5 killed, axis cancellation + cross-depth constraint, D90: sum-angle structure, D91: activation bottleneck test, D92: parity-lock theorem PROVEN as P04). Phase 1: 2 batches of 4 agents. Phase 2: first attempt failed (API socket errors), re-run successful with 5 parallel agents. Phases 3-5: fully parallel. 80 demos, 113 literature refs. Paper 7 proposed (mechanistic arc). |
| 2026-02-24 | `3119351` | +D93-D94, P04 extended | Incremental defrag. D93 LANDMARK: complement-blindness theorem (PROVED), phase_cell recovery, circuit complexity hierarchy (AND/XOR 1→2762→∞), pigeonhole mechanism. D94: binary icosahedral 2I (E₈), solvability bottleneck confirmed (Barrington), five-pillar synthesis COMPLETE, discrete algebraic reservoir computer framing. P04 extended with complement-blindness. Phase 1: 2 parallel agents. Phases 2-5: 5 parallel agents (solo, no team). 82 demos, 120 literature refs. Paper 8 proposed. |
