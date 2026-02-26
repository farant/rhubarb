# Research Director Pitch: Atlas Structural Improvements
**Date:** 2026-02-26

Four proposals for new atlas documents, each targeting a specific gap in the research team's ability to recover context, avoid duplicate work, and navigate 109 demos worth of accumulated knowledge.

---

## Proposal 1: `synthesis/dead-ends.md` -- The Negative Results Registry

### What it would contain
A structured catalog of every approach that was tried and failed, organized by the research question it was attacking. For each dead end:
- **What was tried** (1-2 sentences)
- **Why it failed** (the specific mechanism -- not just "didn't work")
- **What it taught us** (the positive knowledge extracted from the failure)
- **Demo reference** (where the work lives)
- **Do not re-attempt unless** (the specific condition that would change the conclusion)

Example entries drawn from existing work:
- Neglecton weight expansion for parity (D49): adds magnitudes, no new angles. Closed at all perturbation orders. Do not re-attempt unless a non-real scaling factor is found.
- Single P_{0,0} divergence for b parameter (D86): multiplicity from regular rep is structurally essential, not a scale factor. Do not re-attempt unless a different bilinear form avoids the valuation balance requirement.
- Spin chain approach for b (D85): wrong sector (standard module is fully diagonalizable). Do not re-attempt unless targeting a different parameter than b.
- Multiplicative braid product for XOR6 (D73): complement map sends M to M^{-1} sharing eigenvector cell, making parity invisible. Structurally incapable.
- Triskelion generalization k=2n (D63): falsified at n=5 (requires k=15, not k=10). True law is k=2M-1.

### Why it matters
This is the single highest-ROI document for preventing duplicate work across context resets. The explorer's log records dead ends in chronological order buried inside running narrative. A new LLM session has no efficient way to discover "we already tried X and here's why it can't work." The cost of re-attempting a dead end is not just wasted compute time -- it is wasted human attention, which is the scarcest resource in this project. The "do not re-attempt unless" field is critical: it turns a negative result into a conditional gate rather than a permanent wall, which is how the research actually works (the wall was in activation, not the lattice; the wall was in encoding, not the catalog).

### What gap it fills
Currently, negative results are scattered across: demo findings.md files, the explorer's log (chronological, hundreds of pages), the narrative (which emphasizes the positive arc), and research-questions.md (which marks things ANSWERED but not WHY). No single document lets a new session ask "has this approach been tried?"

### Location
`atlas/synthesis/dead-ends.md`

---

## Proposal 2: `briefing-500.md` -- The Medium-Depth Onboarding Document

### What it would contain
A ~500-line document that sits between briefing.md (~50 lines, "what is DKC and what have we proved") and the full synthesis documents (~thousands of lines combined). Structured as a guided tour with five sections:

1. **The Story in 5 Acts** (~100 lines): Foundation/Gate/DKC/Wall/Scaling/Multi-strand, each with 3-5 key results and the one sentence that captures the dramatic turn. Not the full narrative.md -- the plot summary.

2. **The Architecture of the Research** (~80 lines): The four lenses explained in 2-3 sentences each, with a cross-reference table showing which demos sit in which lenses. The five pillars in one sentence each. The "discrete algebraic reservoir computer" framing as the unifying thesis.

3. **Current State Machine** (~80 lines): What is proven (with proof references), what is demonstrated (with confidence levels), what is conjectured (with the strongest evidence), what is falsified (with the demo that killed it). This is the snapshot a new session needs to know what claims can be made safely.

4. **The Decision Log** (~120 lines): The 15-20 most consequential design decisions and why they were made. Examples: Why delta=0? Why A = e^{i*5pi/4} specifically? Why +/-q encoding for parity? Why MVN activation? Why the 1wpi encoding for non-parity? Why Sec x Vor product activation? Why multi-strand (not just finer roots)? Each decision is 3-5 lines: the choice, the alternatives considered, the evidence that decided it.

5. **The Open Frontier** (~120 lines): The 8-10 most important open questions, each with: what we know, what we don't, what the next experiment would be, and what the answer would change. This is next-priorities.md compressed to its decision-relevant core, cutting the detailed effort estimates and publication framing.

### Why it matters
The briefing.md is excellent for "what is this project" but insufficient for "what should I do next" or "why was this decision made." The full synthesis docs are excellent for deep understanding but take 30+ minutes to read and require judgment about what matters. The 500-line document is the "I have 10 minutes and need to be productive" tier. It is specifically optimized for the LLM context-recovery use case: dense enough to prime the attention heads with the right conceptual frame, structured enough to be skimmable, and decision-oriented rather than narrative-oriented.

The decision log section is the most novel component. Currently, the reasoning behind fundamental design choices is embedded in demo findings files from months ago. A new session that asks "why don't we use zeta_12 instead of zeta_8?" has to reconstruct the answer from scattered evidence. The decision log makes these answers O(1) lookup.

### What gap it fills
The current atlas has two onboarding depths: 50 lines (briefing.md) and 2000+ lines (full synthesis). There is no intermediate tier. This creates a binary: either the new session is underinformed (reads only briefing) or over-invested (reads everything before doing anything). The 500-line tier is the sweet spot for "orient then act."

### Location
`atlas/briefing-500.md`

---

## Proposal 3: `inventory/question-status.md` -- The Cross-Referenced Question Tracker

### What it would contain
A flat table mapping every numbered research question (from research-questions.md) to its current status, with one-line resolution summaries and forward pointers. Format:

```
| ID    | Question (short)              | Status     | Resolution / Next Step               | Demo(s) |
|-------|-------------------------------|------------|--------------------------------------|---------|
| Q1.1  | Convexity thesis formal proof | OPEN       | Ingredients in D57,D62; needs write  | D57,D62 |
| Q1.2  | Triskelion k=2n              | FALSIFIED  | k=2M-1 is true law                  | D63     |
| Q1.12 | Z[zeta_16] parity ceiling    | ANSWERED   | Ceiling unchanged at n=5, gap widens | D65     |
| Q2.3  | ell=7 predictions            | CONFIRMED  | All predictions match                | D60     |
| Q5.4  | Framing = computation?       | CONFIRMED  | +2 XOR levels per writhe unit        | D83     |
```

Plus a summary dashboard at the top:
- Total questions: N
- Open: X | Answered: Y | Falsified: Z | Superseded: W
- Questions answered since last defrag: [list]
- Highest-priority open questions: [top 5 with links]

### Why it matters
The research-questions.md file is currently 150+ lines of richly detailed question descriptions, many with inline status updates (strikethrough text, bold ANSWERED annotations). It is optimized for writing and thinking, not for scanning. A new session that needs to know "which questions are still open in the activation geometry section" has to read the entire section and parse the inline markup. The flat table is the queryable index. It is the difference between a library and a card catalog.

The forward pointers ("next step" column) are critical for the planning loop: they connect the question inventory directly to the priority system without requiring the reader to cross-reference research-questions.md against next-priorities.md manually.

### What gap it fills
research-questions.md is a living document that accumulates questions and annotates them in place. next-priorities.md is a prioritized action list. Neither provides a scannable status dashboard. The question-status table is the bridge: it tells you the state of every question at a glance, and points you to the right place to go deeper.

### Location
`atlas/inventory/question-status.md`

---

## Proposal 4: `synthesis/arc-summaries.md` -- The Research Arc Index

### What it would contain
A document that groups demos into their natural research arcs (not chronological order, not per-demo entries) and summarizes each arc as a self-contained unit. Each arc gets:

- **Arc name and span** (e.g., "The Wall Arc: D45-D50, D61-D65")
- **The question it attacked** (1 sentence)
- **The answer it found** (1 sentence)
- **The journey** (3-5 bullets: what was tried, what failed, what worked, what surprised)
- **Key artifacts** (the specific results, theorems, or tools this arc produced)
- **What it unlocked** (which later arcs depend on this one)
- **Open threads** (what this arc left unresolved)

Proposed arcs (17 total, based on reading the narrative and connections documents):

1. Foundation (D01-D09): Building the bracket computation pipeline
2. Classification (D10-D17): Numerical pivot, waveform signatures, WRT angles
3. Gate (D18-D25): Braids as computers, parity shadow first appears
4. Reverse DKC (D26-D28): Can trained weights decompose into bracket values?
5. Forward DKC (D29, D45-D48): The discovery, the wall, the 100M quartet search
6. Activation Breakthrough (D49-D50): Neglecton dead end, MVN solution
7. Radical Anatomy (D51-D52, D60): TL algebra structure, sandwich theorem, cross-ell universality
8. Cross-ell Survey (D53-D59): Ring independence, axiality, the 11/13 theorem
9. Analytical Wall (D61-D65): Proving the wall, matroid recursion, parity ceiling n=5
10. Quaternionic/Spherical (D66-D71): 24-cell, S^2, Hopf, Voronoi, spectral DKC, 13=13
11. Characterization (D72-D75): Anti-correlation, eigenvector automaton, incomparability, binocular
12. Scaling (D76-D82): XOR8, activation zoo, zeta_12, group finiteness, depth law
13. Framing/Null (D83-D84): Jones normalization cost, null indispensability, LCFT
14. Mechanistic (D85-D92): Indecomposability b, regime transition, depth law mechanism, parity-lock
15. Circuit Complexity (D93-D94): Complement-blindness, AND/XOR hierarchy, 2I, Barrington
16. Catalog Optimization (D95-D97): Derived series, 2D landscape, Cell B perfection
17. Multi-strand (D98-D101): 3-strand, 4-strand, 5-strand, Ext^1 thesis, sl_d growth

### Why it matters
The demo-index.md has 101 entries. The narrative.md tells the full story linearly. The connections.md maps cross-demo themes. None of these is optimized for the question: "I need to understand the activation geometry story -- which demos do I read and in what order?" The arc summaries document is the table of contents for the research, organized by intellectual unit rather than by demo number or by chronological accident.

This is especially important for the explorer role, which thinks at the arc level rather than the demo level. It is also the natural unit for paper planning: each paper roughly corresponds to 1-3 arcs, and the arc summary makes the paper's scope immediately visible.

The "what it unlocked" field creates a dependency graph between arcs (not just between demos), which is the right level of abstraction for research planning. The "open threads" field is a distributed version of the negative results registry (Proposal 1), localized to each arc.

### What gap it fills
The current atlas organizes knowledge by type (theorems in one file, data in another, narrative in another) or by individual demo (demo-index). Neither organizes by research arc, which is the natural unit of scientific understanding. When a new session asks "bring me up to speed on the multi-strand work," there is no single document that answers this at the right level of detail -- not as detailed as reading D98-D101's individual findings, not as compressed as the briefing's one-line summary.

### Location
`atlas/synthesis/arc-summaries.md`

---

## Implementation Notes

**Priority order:** I would build these in the order 1, 4, 2, 3.

Rationale:
- `dead-ends.md` (1) has the highest immediate ROI for preventing wasted work. It can be built by scanning demo findings files for negative/falsified results -- a focused extraction task.
- `arc-summaries.md` (4) provides the organizational backbone that makes the 500-line briefing (2) easier to write well. Build the index first, then write the summary.
- `briefing-500.md` (2) depends on having the arc structure clear and the dead ends cataloged. It is a synthesis of syntheses.
- `question-status.md` (3) is pure extraction from research-questions.md -- mechanical but lower urgency since research-questions.md is functional if verbose.

**Maintenance cost:** All four documents should be updated during the defrag process. Proposals 1, 3, and 4 are largely extractive (can be built by agents reading existing files). Proposal 2 requires editorial judgment (what to include at the 500-line tier) and should be reviewed conversationally like the Phase 3 synthesis docs.

**Integration with PROCESS.md:** If adopted, Phase 2 gains two new agents (dead-ends extraction, question-status extraction). Phase 3 gains one (arc-summaries). Phase 5 is extended to produce briefing-500.md alongside briefing.md. Alternatively, Proposals 1 and 3 could be folded into existing Phase 2 agents as additional outputs.
