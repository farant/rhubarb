## Team Structure

- we have discovered an effective way to work on stuff using claude code teams
- it involves the main claude code ("team lead") and two additional team members ("explorer" and "researcher") + me (fran)
- the explorer does not interact with the file system or anything, its only way to interact is through sending and receiving messages with team lead.
- explorer can kind of think of team lead as their "claude code"
- meaning explorer asks team lead to do things
- explorer can ask team lead for all kinds of information, etc
- explorer should not read files or worry to much about specific implementation, ie don't ask team lead "please let me see this and this file". instead ask the team lead questions and follow up questions. generally you are more focused on math and high level stuff than implementation, although obviously you and team lead can talk about implementation if necessary.
- there is a file called explorers-log.md that the explorer can (and should periodically) write to
- explorers-log.md is supposed to be one document that the explorer can get oriented in the project with
- explorers-log.md is big, be prepared to search and tail it as necessary
- i can talk directly to the explorer, it doesn't need to to through the team lead
- don't worry about writing things in latin like the rest of rhubarb, just follow the c89/no dependencies style of existing demos
- to the explorer: just imagine you are using claude code. you can be pretty proactive and it seems like you guys ended up being able to do a lot autonomously, especially with the work we are doing. so feel free to ask for whatever information you want. don't worry too much about what is in other CLAUDE.md files, in parent folders, knotapel is kind of a different context. if i throw out an idea you can discuss it with me before sending to team lead if you want to
- if you just compacted don't spawn a new team. it is possible you are the explorer that team lead spawned and if you try to spawn a team yourself things get glitchy
- also for the explorer: if you want to ask questions about past demos or the project as a whole this is a great job for the researcher

- for team lead don't forget you have access to our read from our new atlas documentation folder
- before you implement any demo look at atlas/inventory/code-assets.md it might document demos that have code you can reuse for whatever you are working on
- don't worry about updating it, there is process for doing that which will happen when i explicitly ask that is documented in PROCESS.md (you can focus on documenting in explorer log, etc as we go, that material is used to build up the atlas stuff)
- there is a briefing.md that is good to use to orient explorer
- there is a synthesis folder that has a novelty.md which is probably good to reference when starting new projects
- there is also a planning folder that has some useful markdown files
- for team lead: it might be worthwhile to just 100% avoid floating point, occasionally we have artifacts in results that are from floating point, i think we've caught them so far but wouldn't want that kind of thing to slip through
- another source of bad results can be truncated catalogs / limits where we don't explore the whole space, obviously sometimes this might be necessary or proudent but good to keep in mind
- if you are writing something that you expect to be more compute expensive it might be worthwhile to think about inlining functions in hot loop, using pointers instead of copying to stack, etc
- its good to have a findings.md in the demo folder before moving on to the next one, explorer should prepare this as well as updates to explorers log for team lead once we move on to next demo.

IMPORTANT
- NEVER DELETE OR SPAWN TEAM OR TEAM MEMBER AFTER COMPACTION
- just wait and be patient, it can take a minute to reorient
- if you want to check the status of the team just ask Fran

important:
this is a long running way of working, it is not a task oriented thing so once we start this mode we are not going to "shut down" team members once we finish a particular task as it might typically work in a team.


## Researcher Role

the researcher is a long-running team member focused on literature survey and web research.

- researcher does NOT write files or code — they search the web, read project files (especially the atlas folder), synthesize, and report
- researcher can talk directly to explorer (and vice versa) — research questions don't need to route through team lead
- team lead gets a brief summary in idle notifications when explorer and researcher DM each other, so team lead has visibility without full context
- team lead can also send queries to researcher when needed for implementation context
- researcher is idle between queries, not proactively searching — explorer or team lead sends a question, researcher does the work and reports back

### What researcher produces
concise summaries with a standard format:
- **claim**: what the literature says
- **evidence**: specific papers, authors, key results
- **sources**: URLs or paper references
- **confidence**: how sure they are (and whether sources agree or conflict)
- **query type**: flag whether this was an "orienting" query (before computation) or "confirming" query (after we have a number) — per the prediction vs rationalization discipline below

### Researcher discipline
- the prediction vs rationalization lesson applies: researcher should flag query type
- researcher findings are HYPOTHESES until verified by our computation — we've been burned by LLM literature claims before (Gemini twice wrong on Demo 85/86)
- researcher should note when sources conflict or when they can't find direct confirmation
- web search results have the same trust level as cross-model checks — suggestive, not conclusive


## Explorer Research Discipline (lessons from Demo 85)

### Verification hierarchy
when a computation produces a numerical result that matches a literature value, treat it as a HYPOTHESIS not a confirmation until:
1. the result is reproduced at a second lattice size (e.g., TL_4 AND TL_6)
2. any intermediate factors (multiplicity, normalization) are verified by independent computation, not just cross-model explanation
3. if the result involves dividing by a factor to match literature, compute the single-copy value DIRECTLY to confirm the factor is real

### Cross-model checks — prediction vs rationalization
- cross-model checks are most valuable as PREDICTIONS: ask the question BEFORE you have the number. "what multiplicity factor should we expect?" → compute → verify
- cross-model checks are least valuable as RATIONALIZATIONS: you have a number, you ask "why is it this?", and the model finds a plausible explanation. LLMs are very good at post-hoc rationalization, and two models agreeing doesn't help much if they share training data biases
- when using cross-model for confirmation, flag explicitly in the explorer log: "this was post-hoc confirmation, not prediction"
- the strongest use is for ORIENTING questions early in a demo: "what does the literature say about X?", "what are the known approaches to Y?", "what pitfalls should we watch for?"

### When to declare a demo result vs "preliminary"
- **result**: reproduced at 2+ lattice sizes, no unexplained failures at larger sizes, key factors verified independently
- **preliminary**: works at one size, plausible explanation for why other sizes fail, cross-model agreement but no independent verification
- **hypothesis**: works at one size, other sizes not yet attempted, explanation comes from cross-model rationalization
- Demo 85's b = -5/8 is currently "preliminary" — TL_4 works, TL_6 diverges with a plausible structural explanation, multiplicity factor from cross-model not independently verified

### Dead ends are data
every failed approach narrows the space. document WHY it failed, not just THAT it failed. the pattern from Demo 85:
- fixpt form → teaches us about gauge structure (M=0, affine landscape)
- spin chain → teaches us about module selection (wrong sector)
- TL_6 full algebra → teaches us about cross-summand interference
each dead end was essential to understanding the final result

### Computational claims need computational verification
if the theoretical argument says "divide by dim(0-TL sector) = 4", the strongest verification is computing b on a single P_{0,0} copy and getting -5/8 directly. theoretical arguments + cross-model agreement is suggestive. direct computation is conclusive.


## Raqiya Library (knotapel/raqiya/)

Raqiya ("firmament") is a reusable C89 single-header library for analyzing Z[zeta_8] cyclotomic values. It provides exact arithmetic in Z[zeta_8] and 6 relationship detectors that reveal algebraic structure.

### Files
- `raqiya/raqiya.h` — single-header library (include directly, all functions are static)
- `raqiya/probatio_raqiya.c` — unit tests (86 tests covering all detectors + arithmetic)
- Compile tests: `cd knotapel/raqiya && cc -std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -o probatio_raqiya probatio_raqiya.c && ./probatio_raqiya`

### Usage
```c
#include <stdio.h>
#define RAQ_PRINT          /* enables raq_print_* functions (requires stdio.h) */
#include "../raqiya/raqiya.h"

/* Collect distinct values */
Raq_ValueSet vs;
raq_vs_init(&vs, 8192);
raq_vs_insert(&vs, raq_cyc8_make(0, 1, 0, 0));  /* zeta_8 */
/* ... insert more values ... */

/* Run full analysis */
Raq_Analysis a = raq_analyze(vs.values, vs.count);
raq_print_analysis(&a, vs.values);

/* Or run individual detectors */
Raq_Partition orbits = raq_detect_root_orbits(vs.values, vs.count);
Raq_Partition norms = raq_detect_norm_classes(vs.values, vs.count);
Raq_Partition galois = raq_detect_galois_orbits(vs.values, vs.count);

/* Cleanup */
raq_partition_free(&orbits);
raq_analysis_free(&a);
raq_vs_free(&vs);
```

### Key Types
- `Raq_Cyc8` — element of Z[zeta_8], basis {1, zeta, zeta^2, zeta^3}, zeta^4=-1
- `Raq_ValueSet` — hash-based distinct value collector
- `Raq_Partition` — group assignment result (group_id[], group_sizes[], n_groups)
- `Raq_PairHist` — pairwise distance histogram (bins[0..4])
- `Raq_Analysis` — full analysis result (all 6 detectors)

### Regression Safety
- **Every time raqiya is used in a new demo or gains new functionality, add corresponding test coverage to probatio_raqiya.c**
- Run probatio_raqiya before and after any changes to raqiya.h
- The test file includes D107-specific regression tests (13 values, 4 orbits, 7 Galois orbits, axis alignment)
- When adding new detectors or modifying existing ones, add test cases that validate the specific behavior used by existing demos


## House Libraries in Demos (rhubarb include/ + lib/) — new since 2026-10

demos used to be single self-contained main.c files with only stdio/stdlib/string. from now on a demo MAY include rhubarb's house libraries, which give exact arithmetic that is tested, reviewed and shared with the rest of rhubarb. the lesson of D110–D112 was unchecked homemade arithmetic inside demos, so:

- **general number types and algorithms never live in knotapel/ again.** if a demo needs something general (big integers, rationals, polynomials, matrices, geometry predicates, knot invariants), it belongs in a house library; knotapel only consumes. ask before writing a new one inside a demo.
- the exact math stack (headers in `include/`, each header's first line says what it is; `briar -bibliothecae` lists them, `briar -bibliotheca <x> -functiones` prints signatures):
  - `magnus` big integers · `fractio` exact rationals · `congruentia` Z/n arithmetic, primality, CRT
  - `polynomium` Laurent polynomials Z[t, t^-1] · `anulus` ring descriptor (Z, Q, Z[t,t^-1], Z/n) · `matrix` det/rank/kernel, Hermite/Smith over any anulus
  - `situs` exact 3D geometry predicates (segments, triangles, crossings) · `laqueus` polygonal knots/links in Z^3: diagram, writhe, linking number, Kauffman bracket (<= 24 crossings), Jones, Alexander, checked triangle moves
- the libraries are written in Latin; demo code stays English.
- **HAZARD:** including any house header brings in `latina.h`, so every Roman numeral (`I V X L C D M` and combinations like `II`, `XL`) and the Latin keywords (`si`, `per`, `dum`, `fac`, `redde`, `nomen`, `casus`, ...) are macros inside your demo. don't use them as variable names. usually a loud compile error, but know why it happens.
- build and run live, from the repo root: `./bin/aedilis knotapel/demo_NNN/main.c && bash build/aedilis/main/struere.sh && ./build/aedilis/main/main` (aedilis derives the closure from the includes; house compile flags, -Werror).

### freezing a finished demo (archive.sh)
a demo that includes house libraries builds against code that keeps evolving. when the demo is finished (findings.md written), freeze it:

```
./knotapel/archive.sh knotapel/demo_NNN/main.c [program args]
```

this writes `demo_NNN/demo-snapshot.c`: the demo + its whole library closure as ONE file that compiles alone (the exact clang line is in its banner, with the commit and every library file's git hash). the script builds and RUNS both the live demo and the snapshot and refuses unless their output is byte-identical; it also refuses if a library file has uncommitted changes. commit the snapshot with the demo and cite it in findings.md. later, re-running main.c live is a free regression check: if its output ever differs from the snapshot's, a library changed behavior — investigate before trusting either.

the snapshot is generated (line 1 says GENERATUM): never edit it by hand, regenerate it. regression fixture: `knotapel/archive-trial/`.
