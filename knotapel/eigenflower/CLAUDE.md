## Vision

Eigenflower is the applied AI / engineering department of the knotapel project. Knotapel is the pure mathematics research department (DKC — 109 demos, 4 formal proofs, novel results at the intersection of topology, algebra, neural computation, and information theory). Eigenflower takes those results and builds a system.

### Goals
- Build a hybrid AI from scratch — NOT taking an off-the-shelf model
- Compile/train in ~30-60 minutes on M2 laptop
- Small corpus: Bible (multilingual) + Summa Theologica (Latin/English)
- Run performantly on MacBook while doing other things (1-2GB RAM, ~70 tok/s)
- Compile syllogisms to ANN weights (tensor logic — exact, no training loop)
- Extract syllogisms from natural language
- Dynamically recompile weights on the fly to record new syllogisms
- "Decompile" existing models to understand their internal circuits
- Eventually: transparent reasoning across all modes (deduction, analogy, authority, reductio, distinguo)

### Architecture (Hybrid Model)
Two coupled systems that solve each other's problems:

1. **Fluency ANN** — small neural network (~50-100M params, ternary/BitNet) for language parsing, understanding, generation. Trained on Bible + Summa corpus.

2. **Syllogism ANN** — structured graph of compiled logical relationships. Functions like an internal graph database the fluency layer can walk. Tensor equations, exact arithmetic, no training — compilation via kernel ridge regression (closed-form).

3. **The Interface** — the hard/novel problem. How the fluency layer queries the syllogism graph, how it narrates results back. Opaque tokens (from knotapel vision.md) are the most concrete proposal: tokens flow through compiled weights, net reasons about logical structure not content, substitution at output.

See `notes/` for detailed architecture notes:
- `notes/01-analysis.md` — paper extractions + knotapel crossover review + synthesis
- `notes/02-architecture-insight.md` — hybrid architecture, build order, open questions
- `notes/03-transparency-and-wisdom-circuits.md` — wisdom circuits, generalization, research program

### Relationship to Knotapel
- Eigenflower references knotapel results but doesn't duplicate them
- DKC proves the core mechanism (exact algebraic weights compute Boolean functions)
- Eigenflower's job is scaling from 3-5 input Boolean functions to propositional complexity
- The atlas (`knotapel/atlas/`) is the reference for DKC results, theorems, and proofs
- Key knotapel results for eigenflower: parity-lock theorem (D92), depth law (D82), dual-channel theorem (D108-109), axis-alignment theorem (D107), forward DKC (D29), five-pillar synthesis

### Key Papers (from extractions)
- **Tensor Logic** (Domingos): SP[s,p] = H(SM[s,m] * MP[m,p]) — syllogism = matrix multiply + step function
- **Hilbert Space Framework**: closed-form kernel ridge regression for reasoning operators
- **BitNet b1.58**: ternary weights {-1, 0, +1} = affirm/deny/ignore
- **TRM**: 7M params recursive, beats billion-param models on reasoning
- **GPTQ-as-CVP**: weight quantization = Closest Vector Problem on Hessian-defined lattice
- **Free Transformer**: latent Z conditions generation on logical mode
- **Edge of Chaos**: data complexity > data quantity; Bible + Summa are high complexity

---

## Working Philosophy — Maximalist Scope

LLMs (including Claude) are trained toward a conservatism of scope — "focus, minimize, ship." This is the cultural norm in software and it's wrong here. The correct approach for this project is maximalist:

- **Broad scope is the method, not a distraction.** You don't find the answer to problem A by staring at it harder — you find it by working on problem B and noticing the connection. The wider the scope, the more surface area for productive connections.
- **Many modules running simultaneously is the goal.** Cross-pollination and fluid switching between modules is an engine of productivity, not a sign of unfocus. Working on one module naturally produces side results that unblock others.
- **The system should surface what's ripe, not restrict what's open.** Don't gate new modules. Don't push to "finish things before starting new ones." Instead, maintain visibility across all modules so you can always pick the ripest fruit regardless of which tree it's on.
- **Ambitious scope uncovers answers.** It's by increasing scope that you discover solutions to other problems. This is proven by knotapel: the five-pillar synthesis only emerged because the scope was wide enough to stumble across connections between topology, algebra, neural computation, and information theory.
- **The database externalizes this intuition.** The broad/fluid/intuitive approach already works — knotapel proved it over 109 demos. But it's bounded by what one person can hold in their head and by luck. The database makes the same strategy legible, persistent, and scalable — so it works at 500 experiments as well as 50, and across context resets where a new Claude instance needs to make picks that are almost as good as the ones Fran would make intuitively.

The discipline is NOT "focus more." The discipline is "record what you notice, tag the connections, keep the status current" — so the system can do the pattern matching that a human brain does naturally but can't scale infinitely.

---

## Project Database — Design Principles

Eigenflower uses a filesystem database for project organization: folders as tables, markdown files as rows. This is built on lessons from knotapel's atlas (which was retrofitted and text-file based) but designed from the start to be more granular, queryable, and agent-friendly.

### Why a Database, Not Just Documents
- Documents (like knotapel's atlas) are denormalized views — readable but not queryable, hard to update atomically
- A folder-of-markdown-files gives granularity of a database with readability and git-friendliness of plain text
- Readable summary documents can be GENERATED from the granular layer, not the other way around

### Primary Keys
Every record has a unique ID: `PREFIX` + monotonically increasing 4-digit number.
- Example: `Q0001_something.md`, `Q0002_something-else.md`
- Prefix gives type at a glance, number gives ordering and uniqueness
- The `_something` suffix is a human-readable hint, never parsed programmatically
- `Q0012` is always Q0012 forever, even if the title changes
- Foreign key references: just use the ID (e.g., `Q0012`) anywhere — grep finds all references

### Purpose — The Two Questions
Everything in the taxonomy serves one or both of:
1. **"Help us pick the best next experiment to do"** — what's open, what's blocked, what's high-value, what's ready, what depends on what
2. **"Help us do the best experiment possible"** — what do we know, what's been tried, what worked/didn't, what does the literature say, what are the constraints

Anything that doesn't feed one of these two moments is overhead. Anything missing that would make one of these moments better is a gap.

### Three Input Channels
The database is populated from three kinds of work, each producing different mixes of the same entity types:

1. **Experiments** — produce empirical facts: this worked, this didn't, this number came out, this assumption was wrong. Generate: answered questions, new questions, confirmed/refuted hypotheses, updated component status.

2. **Research** — produces external knowledge: this paper says, this technique exists, this has been tried. Generate: new references, technique options, constraints.

3. **Brainstorming / Analysis** — produces internal synthesis: these two things connect, this reframes the problem, here's a new approach. Generate: new hypotheses, architectural alternatives, connections.

All three channels land in the same database with cross-references between them. A hypothesis from brainstorming gets tested by an experiment informed by research.

### Entity Extraction Pipeline
After each experiment/research/brainstorming session, specialized agents can do entity extraction:
- Each agent owns a TABLE, not a specific record — "you are the questions agent, here's the questions folder, here are the results, do your job"
- Agents scan existing records, find which ones the new results bear on, update statuses, notice new entities, flag connections
- Agents run in parallel (writing to different folders = no conflicts)
- The taxonomy IS the instruction set for extraction agents — well-chosen types = clear agent mandates

### Taxonomy

Each prefix is a folder ("table"). Each file in the folder is a record ("row"). Filename format: `PREFIX####_short-description.md` (e.g., `G0001_compile-syllogisms.md`). The prefix+number is the permanent unique ID — the `_short-description` suffix is a human-readable hint, never parsed.

#### Entity Types

| Prefix | Name | What It Captures |
|--------|------|-----------------|
| **G** | Goals | What we want to achieve. Top-level objectives that other records serve. |
| **P** | Papers | ArXiv / academic papers. Specific metadata: authors, arXiv ID, year, abstract. |
| **REF** | References | Other external knowledge: DKC demo results, theorems, textbook sections, web resources. Anything external that isn't an arXiv paper. |
| **Q** | Questions | Well-articulated things we want to answer. Specific enough to design an experiment around. |
| **UNK** | Unknowns | Broader than questions — things we don't understand yet, untested assumptions, gaps in knowledge, blind spots. May be vague. Can mature into Q records once articulated. |
| **COMP** | Complications | Emergent complexity discovered during implementation. The "oh this should take 5 minutes" walls. Always emerges FROM something and affects something. Patterns in COMPs reveal where real complexity lives. |
| **HYP** | Hypotheses | Testable claims. Has a status: proposed / tested / confirmed / refuted. Links to evidence (EXP, REF). |
| **EXP** | Experiments | Things we tried and their results. Links to what motivated it (G, Q, HYP) and what it produced (answered Qs, new UNKs, COMPs, VTs). |
| **DEC** | Decisions | Design choices with rationale. Records what was decided, what alternatives were considered, and why. Links to what informed it (P, REF, EXP) and what it constrains (G, future DECs). |
| **PT** | Potential Techniques | Ideas or approaches from papers/brainstorming that MIGHT work but haven't been tried. Cheap to create. Can be promoted to VT, or killed (linked to COMP). Untested PTs are the menu of possible experiments. |
| **VT** | Validated Techniques | Techniques we actually made work in our context. Earned, not assumed. Links to the EXP that validated it and any caveats. The gap between PT and VT is where COMPs live. |
| **RES** | Results | Empirical findings from experiments. A result is a referenceable fact — "Barbara compiles to a working tensor equation in exact Z[zeta_8] arithmetic." Lives independently of the EXP that produced it so many records can point to it. |
| **FR** | Frames | High-level lenses / perspectives that cut across the project. E.g., training performance, ANN compilation/decompilation, runtime performance, dynamic weights. A frame contains goals, modules, experiments. The same work can be relevant to multiple frames. |
| **MOD** | Modules | Granular areas of work within or across frames. Organic — they emerge, can be dormant, can become ripe. Not bounded like software modules. The ripeness view across all MODs is the primary steering artifact. |
| **CON** | Connections | Cross-references between things that don't obviously belong together. The highest-value records in the database. "EXP0045 in MOD0003 produced a result relating to UNK0012 in MOD0017." Connections are what make broad scope productive — they turn "happened to notice" into "systematically found." |
| **MILE** | Milestones | Defined checkpoints within or across modules. What "ripe" means concretely. "Working tensor logic interpreter for 4 syllogistic figures." Milestones in different modules can depend on each other. The ripeness view shows: which milestones are closest to completion across all modules? |
| **LIM** | Limitations | Boundaries that shape all downstream work. Can be chosen ("C89 only," "train in 30-60 min on M2," "1-2GB RAM") or discovered through experiments ("parity ceiling is n=5," "naive thresholding doesn't work on cyclotomic values"). Both kinds constrain decisions, techniques, and goals. DEC records reference LIMs to explain why alternatives were rejected. |
| **TASK** | Tasks | Reusable process templates — not one-off todos but classes of repeatable work. Three classes: **extract** (decompose material into records), **quality** (cross-reference, validate, fix anomalies), **aggregate** (generate steering views and reports). Each TASK documents procedure, inputs, outputs, and agent/script configuration. |
| **WF** | Workflows | Multi-step processes (sagas) that chain TASKs together with decision gates and branching. Like Smaragda processes. Each stage has an actor (fran/team_lead/agents/scripts), inputs, outputs, and optional decision gates where humans review and choose direction. Workflows are the orchestration layer — TASKs are individual operations, WFs are the sequencing. |

#### Lifecycle Patterns
- **PT → VT**: a potential technique gets validated by an experiment. The most common experiment motivation.
- **PT → killed (+ COMP)**: a potential technique fails, complication explains why.
- **UNK → Q**: a vague unknown gets articulated into a precise question.
- **Q → answered (via EXP)**: an experiment answers a question.
- **HYP → confirmed/refuted (via EXP)**: an experiment tests a hypothesis.
- **EXP → RES + new UNKs, Qs, COMPs**: experiments produce results and always generate new records alongside them.
- **Cross-pollination**: work in one MOD produces CON records linking to other MODs, surfacing ripeness elsewhere.

### Schema & Validation
- Each entity folder contains a `schema.md` (or `schema.json`) defining required/optional fields, controlled vocabularies, and ID format
- Validation scripts parse YAML frontmatter across all records and check against schema
- Anomalous records (missing fields, invalid references, unknown status values) are aggregated and handed to an LLM cleanup agent that can infer intent and fix them
- Schema is evolvable: add a field, update the schema, run an agent pass to populate it across existing records from context

### Generated Views (Three Layers)
1. **Records** — the ground truth. Individual markdown files with structured frontmatter.
2. **Summary documents** — readable narratives generated from records. Like knotapel's atlas text files, but built from the granular layer.
3. **Steering views** — generated on demand by scripts (bash or bun.js) that parse frontmatter:
   - **Ripeness map**: across all MODs, which milestones are closest? What just became unblocked?
   - **Critical path**: from a goal, walk dependencies backward to find the longest unresolved chain
   - **Ready queue**: all records with status "open" and no unresolved dependencies — what can be worked on now
   - **Blocking analysis**: which records appear most often in others' depends_on — the bottlenecks
   - **Orphan scan**: records with no recent inbound references — forgotten but potentially important
   - **Connection distance**: cross-references spanning otherwise unrelated modules — where cross-pollination is happening
   - **Branch health**: per-module trending of VT/PT/COMP ratios over recent experiments
   - **Phase detection**: shifting ratios of record types being created over time

#### Taxonomy Status: INITIAL SET (19 types)
This is the starting taxonomy. It will evolve as we use it — new types may be added, existing types may be split or merged. The schema validation + LLM cleanup pipeline means we can evolve without fear of breaking existing records.

---

## Database — Current State

### Record Counts (as of initial extraction)
First extraction pass (TASK0001) produced 329 records. With TASK and WF records: **340 total**.

| Table | Prefix | Count | Has Schema |
|-------|--------|-------|------------|
| potential_techniques | PT | 73 | yes |
| connections | CON | 71 | yes |
| references | REF | 45 | yes |
| unknowns | UNK | 30 | yes |
| questions | Q | 29 | yes |
| goals | G | 28 | yes |
| limitations | LIM | 27 | yes |
| papers | P | 14 | yes |
| hypotheses | HYP | 12 | yes |
| tasks | TASK | 6 | yes |
| workflows | WF | 5 | yes |
| complications | COMP | 0 | **no** |
| decisions | DEC | 0 | **no** |
| experiments | EXP | 0 | **no** |
| frames | FR | 0 | **no** |
| milestones | MILE | 0 | **no** |
| modules | MOD | 0 | **no** |
| results | RES | 0 | **no** |
| validated_techniques | VT | 0 | **no** |

The 8 empty tables are empty by design — they get populated as actual experiments happen, not from existing notes. Schemas for these tables need to be created before agents can populate them.

### What's Been Done
- TASK0001 (entity extraction): COMPLETE — 8 parallel agents extracted from notes/, extractions/, crossovers/
- TASK0002 (cross-reference pass): NOT YET RUN — most `related` fields are still `[]`, dependency graph is sparse. Run this before relying on dependency-based tools.
- TASK0003-0006: not yet run

### What Needs Doing Next
1. Create schemas for the 8 empty tables (COMP, DEC, EXP, FR, MILE, MOD, RES, VT)
2. Run TASK0002 (cross-reference pass) to populate the link graph
3. Run TASK0003 (schema validation) to clean up any extraction anomalies
4. Then TASK0005 steering views will produce useful results

---

## Query Tools (`tools/`)

Zero-dependency Bun scripts for querying the database. All parse YAML frontmatter directly. Run with `bun tools/<name>.ts [args]`.

### Shared Module
- `parse-frontmatter.ts` — YAML frontmatter parser, `readAllRecords()`, `getDbPath()`, `getId()`, `getRelated()`. Handles simple values, inline lists `[a, b]`, and multi-line lists `- a\n- b`.

### Overview & Browse
- `count-records-per-table.ts` — record counts per table, quick database overview
- `list-all-records-in-table.ts <table>` — list all records in a table with ID/title/status
- `show-record-detail-by-id.ts <ID>` — full frontmatter + body for a single record
- `show-records-with-status.ts <status> [table]` — filter by status, optional table filter

### Search
- `search-records-by-text-in-title-or-body.ts <term>` — case-insensitive full-text search, title matches ranked first

### Graph & Dependencies
- `find-all-references-to-id.ts <ID>` — all records that reference an ID (in frontmatter or body)
- `show-dependency-chain-for-goal.ts <GOAL_ID>` — walk depends_on tree, show ready leaf nodes
- `show-ready-queue-of-actionable-items.ts` — open/untested/proposed items with no unresolved dependencies
- `show-blocking-bottleneck-analysis.ts` — records that appear most in dependency fields (top 30 bottlenecks)
- `find-orphan-records-with-no-inbound-references.ts [table]` — records nothing points to

### Adding a New Tool
Follow the pattern: `#!/usr/bin/env bun`, import from `./parse-frontmatter`, self-documenting long filename, zero dependencies, console output only.

---

## Defined Tasks & Workflows

### Tasks (reusable process templates)
| ID | Name | Class | Purpose |
|----|------|-------|---------|
| TASK0001 | Extract entities from material | extract | Fan-out agents, each owns a table, scan new material |
| TASK0002 | Cross-reference pass | quality | Populate `related`/`depends_on`/`blocks` fields across all records |
| TASK0003 | Schema validation and cleanup | quality | Validate frontmatter against schemas, LLM cleanup agent fixes anomalies |
| TASK0004 | Experiment debrief | extract | Post-experiment entity extraction into all relevant tables |
| TASK0005 | Generate steering views | aggregate | Ready queue, critical path, blocking analysis, orphan scan, etc. |
| TASK0006 | Gap detection | quality | Find undocumented unknowns and implicit questions |

### Workflows (multi-step processes chaining tasks)
| ID | Name | Stages | When |
|----|------|--------|------|
| WF0001 | Experiment cycle | select → design → execute → debrief → cross-ref → views → decide next | Every experiment |
| WF0002 | Paper ingestion | discover → extract summary → entity extraction → gap detection → cross-ref | New paper arrives |
| WF0003 | Brainstorm harvest | brainstorm → entity extraction → gap detection → review and connect | After brainstorming |
| WF0004 | Maintenance cycle | schema validation → cross-ref → gap detection → steering views | Periodically |
| WF0005 | Module recognition | notice pattern → formalize module → define milestones → assign frame | When pattern emerges |

Full details in `db/tasks/` and `db/workflows/`.

---

## Existing Materials

### Corpus (`corpus/`)
- Douay-Rheims Bible (English, ~5MB)
- Greek Septuagint (~4.5MB)
- Latin Vulgate (~4.4MB)
- Summa Theologica parts (English): Prima Pars, Prima Secundae, Secunda Secundae, Tertia Pars, plus supplements
- TODO: Latin Summa text still needed
- TODO: Secunda Secundae audit (708KB seems small)

### Research (`research/`, `extractions/`)
- 14 arXiv papers collected across 5 groups (A-E)
- 5 paper extraction files (detailed summaries with eigenflower relevance)
- 5 knotapel crossover files (cross-referencing papers against 109 DKC demos)
- `research/mercury-2.md` — diffusion-based reasoning LLM notes

### Notes (`notes/`)
- `01-analysis.md` — comprehensive review of all extractions and crossovers
- `02-architecture-insight.md` — hybrid architecture, build order, open questions
- `03-transparency-and-wisdom-circuits.md` — wisdom circuits concept, research program


