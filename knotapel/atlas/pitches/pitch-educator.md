# Educator / Science Communicator Pitch: New Atlas Documents

*2026-02-26*

---

## The Problem

The knotapel atlas currently has two layers of depth: the briefing (~50 lines, fast onboarding) and the full synthesis documents (narrative.md alone exceeds 40,000 tokens). There is nothing in between. A reader who finishes the briefing and wants to understand *why* things work the way they do has to jump straight into a document that assumes familiarity with Temperley-Lieb algebras, cyclotomic integer rings, Kauffman brackets, NPN classification, and half a dozen other specialized concepts.

The human running this project explicitly says: "human needs to understand all the code." The atlas serves not just as institutional memory for LLM context restarts but as a *teaching tool* for the human. Right now it teaches by immersion (read the 101-demo narrative). It does not teach by scaffolding (here is what you need to understand first, here is the simplest version of the key idea, here is where to go deeper).

---

## Pitch 1: Concept Dependency Map

**Name:** `atlas/synthesis/prerequisites.md`

**What it contains:**

A directed acyclic graph (rendered as indented text with arrows) showing which concepts depend on which other concepts. Not demo dependencies -- *concept* dependencies. For example:

```
State Sum (what a bracket IS)
  -> Cyclotomic Evaluation (plugging in roots of unity)
    -> Z[zeta_8] Exact Arithmetic (bracket values as 4-tuples of integers)
      -> Forward DKC (using those 4-tuples as neural network weights)

Temperley-Lieb Algebra (the diagrams)
  -> Non-Semisimplicity (what happens when delta=0)
    -> The Radical (the "invisible" part of the algebra)
      -> Null States (bracket values that are zero but computationally essential)

Activation Function (how you read out a Boolean answer)
  -> k-Sector MVN (slicing the complex plane into pie slices)
    -> The Parity Wall (why split-sigmoid cannot see parity)
    -> 11/13 Theorem (why half-plane activation misses exactly 2 functions)

Braid Group (strands crossing over each other)
  -> SU(2) Representation (braids as quaternion multiplication)
    -> Hopf Fibration (splitting quaternions into direction + phase)
      -> S^2 Sweet Spot (computation lives on the sphere, not in full 4D)
      -> 13-Direction Structure (the specific directions that matter)
```

Each node gets a one-sentence definition and a pointer to where it is introduced in the demo sequence. The whole document should be under 200 lines.

**Why it matters:**

Right now, if you encounter the phrase "null indispensability at zeta_8" in the briefing, you need to already know: what a bracket value is, what zeta_8 means, why some values are null, what "indispensability" means computationally, and why the group being finite matters. Those five prerequisite concepts are scattered across narrative.md sections 1, 3, 7, 10, and 11. The dependency map tells you: "to understand null indispensability, first understand null states, which requires understanding non-semisimplicity, which requires understanding Temperley-Lieb algebras." You can then read exactly those sections in order instead of reading 40 pages hoping to find what you need.

This is the single most impactful document for a cold-start scenario (new LLM context or human returning after a break). It turns the atlas from a library into a curriculum.

**What gap it fills:**

There is no document that answers "what do I need to understand before I can understand X?" The briefing gives results. The narrative gives the full story. The connections document gives cross-demo links. None of them answer the prerequisite question.

---

## Pitch 2: Glossary of Key Terms

**Name:** `atlas/synthesis/glossary.md`

**What it contains:**

An alphabetical glossary of approximately 40-60 terms that appear throughout the atlas, each with:

1. A plain-language definition (one sentence, no jargon)
2. A precise definition (one sentence, technical)
3. The demo where it is first introduced or most clearly demonstrated
4. A cross-reference to related terms

Example entries:

```
## Axiality Theorem
Plain: Bracket values at delta=0 only point in 8 directions in the complex plane.
Precise: At A = e^{i*5pi/4}, all Kauffman bracket values lie on 8 equally-spaced
  rays {k*pi/4 : k=0..7} in the complex plane, with magnitudes that are non-negative
  integers.
Introduced: D35
See also: Z[zeta_8], Angular Structure

## Forward DKC
Plain: Using knot invariant values as neural network weights to compute Boolean
  functions, without any training step.
Precise: Given a catalog of Kauffman bracket values in Z[zeta_8], assign one value
  per input bit, form the signed sum w = sum(+/-q_i), and classify via activation
  function to produce a Boolean output. The weights are topologically determined,
  not learned.
Introduced: D29
See also: Reverse DKC, Bracket Catalog, Activation Function

## Parity Wall
Plain: The observation that certain activation functions cannot compute parity (XOR
  of all inputs), no matter which weights you choose.
Precise: Under split-sigmoid activation, zero solutions for parity exist among 100M+
  exhaustive quartets of Z[zeta_8] bracket values. The wall is in the activation
  geometry, not the weight lattice.
Introduced: D48, resolved D50
See also: k-Sector MVN, 11/13 Theorem, Parity-Lock Theorem
```

**Why it matters:**

The briefing uses 30+ specialized terms (cyclotomic integer, NPN class, parity-lock theorem, Ext^1, complement-blindness, phase_cell, etc.) without defining them. The narrative defines them implicitly by telling the story of their discovery, but if you need a definition NOW -- mid-thought, mid-conversation -- there is no place to look it up quickly.

A glossary is the lowest-overhead, highest-value reference document you can build. It costs very little to maintain (append new entries as demos are added) and it prevents the most common failure mode in LLM onboarding: using a term confidently but incorrectly because the context window contains the word but not its precise meaning.

**What gap it fills:**

No atlas document is organized for lookup by term. Everything is organized by story (narrative), by demo (demo-index), by theme (connections), or by lens (four-lenses). A glossary is organized by "I just encountered this word and I need to know what it means in 10 seconds."

---

## Pitch 3: The 10 Key Insights (Progressive Depth Version)

**Name:** `atlas/synthesis/key-insights.md`

**What it contains:**

The 10 most important ideas from the entire 101-demo research program, each presented at three levels of depth:

- **Level 1 (one sentence):** What a bright undergraduate could understand
- **Level 2 (one paragraph):** What a graduate student in a related field could follow
- **Level 3 (one page):** What a specialist would need to engage seriously

The 10 insights (draft selection):

1. **Knot invariants can compute.** The Kauffman bracket, evaluated at specific roots of unity, produces exact algebraic integers that work as neural network weights -- no training required.

2. **The weights are exact, not approximate.** Everything is in the cyclotomic integer ring Z[zeta_8]. Four integers per weight. Zero floating-point error. Correctness is guaranteed by algebra, not by gradient convergence.

3. **Activation geometry determines what is computable.** The same weight catalog computes 12/13 Boolean function classes under one activation, 13/13 under another, and is locked to parity-only under a third. The lattice is fixed; the lens chooses what you see.

4. **There is a hard wall, and we know exactly where it is.** Under half-plane activation, exactly 2 of 13 Boolean function classes are unreachable. This is proven analytically (not just observed). The wall is geometric: a parallelogram gap on a circle.

5. **Depth is the fundamental resource.** Computational capacity scales linearly with crossing depth (how many times the braid strands cross). More complex knots compute more. The logarithmic scaling law everyone measures first is just a corollary.

6. **The algebra is non-semisimple, and that is a feature.** The "defective" part of the Temperley-Lieb algebra (the radical) produces bracket-null entries that are invisible to the scalar trace but essential for computation. Removing them destroys capacity.

7. **The computation is intrinsically curved.** It lives on a sphere (S^2), not a plane. Flattening it to R^2 with Euclidean distance gives zero solutions. The Hopf fibration naturally splits the quaternion into a computational channel (the sphere) and an inert channel (the phase).

8. **Algebraic structure beats geometric optimization.** The 13 directions on S^2 that the braid algebra produces have the worst possible geometric distribution (t=0 design). Improving the distribution by even a few degrees destroys the computation. The algebra knows something geometry does not.

9. **The encoding determines the function.** The +/-q paired encoding is provably locked to parity (XOR/XNOR) -- AND, OR, majority are impossible regardless of activation. The 1-weight-per-input encoding accesses all 13 classes. What you compute is set by how you wire the inputs, not by the weights.

10. **Non-semisimple extensions are computationally load-bearing.** In multi-strand DKC, modules with a radical (non-semisimple) reach XOR14. Simple modules (no radical) cap at XOR12. The "defect" is the resource.

**Why it matters:**

The briefing is a compressed reference. The narrative is a 101-chapter story. Neither is designed for someone who says: "I have 20 minutes. What are the most important things to understand?" The progressive depth structure means you can read 10 sentences and get the shape, or read 10 paragraphs and get the substance, or read 10 pages and get enough to engage technically. This is the document you hand to a collaborator, a reviewer, or yourself after three weeks away.

It also serves a specific LLM onboarding function: the Level 1 sentences are compact enough to fit in a system prompt. If you need a new Claude context to understand the project in 500 tokens instead of reading the full briefing, the 10 Level-1 sentences are the answer.

**What gap it fills:**

No document is optimized for "teach me the most important things at the depth I choose." The briefing is fixed at one depth. The narrative is fixed at maximum depth. This document is the missing intermediate layer that makes the atlas a genuine progressive-disclosure system: briefing (50 lines) -> key insights (10-100 lines) -> narrative (800+ lines).

---

## Pitch 4: The "Wall Was X Not Y" Pattern Catalog

**Name:** `atlas/synthesis/wall-patterns.md`

**What it contains:**

A catalog of every instance where the project hit an apparent impossibility, diagnosed it, and resolved it by changing a specific component. The connections.md document already identifies this as a recurring structural pattern (Section 3: "The Wall Was X Not Y Pattern"). This document expands it into a standalone reference with a consistent format for each instance:

```
## Wall #N: [descriptive name]

**Observed:** [what looked impossible]
**Exhaustive confirmation:** [how thoroughly zero-ness was verified]
**Diagnosis:** [which specific component was responsible]
**Resolution:** [what was changed]
**Mechanism:** [why the change works]
**Demo chain:** [the demo numbers]
**Lesson:** [the generalizable principle]
```

The known instances (at least 6):

1. D48->D50: Parity under split-sigmoid (wall in activation, not lattice)
2. D76->D77: XOR8 under S^2-only (wall in activation, not root)
3. D78->D79: XOR10 at zeta_8 (wall in group finiteness, not architecture)
4. D98->D99: 3-strand trace readout (wall in readout, not algebra)
5. D64: Self-doubling impossibility (wall in geometric constraint, not vocabulary)
6. D72: Voronoi optimization catastrophe (wall in algebraic-vs-geometric mismatch)

Plus several near-miss variants (D65's Clifford staircase, D86's negative result on single P_{0,0}).

**Why it matters:**

This is the most *pedagogically powerful* pattern in the entire project. It teaches a way of thinking: when you hit an apparent impossibility, do not conclude the thing is impossible. Instead, decompose the system into components and figure out which specific component is enforcing the wall. The resolution is almost always: change that one component, keep everything else fixed.

For a learner, this pattern is more valuable than any individual result because it is a *transferable skill*. It applies to debugging, to research strategy, to system design. And in the specific context of DKC, understanding the wall-resolution pattern is the fastest way to understand why the project took the path it did. The sequence of walls and resolutions IS the narrative, compressed to its logical skeleton.

For LLM onboarding specifically: an agent that has read this document will not prematurely conclude "this is impossible" when hitting a zero-result computation. It will instead ask "which component is enforcing this?" -- which is exactly the research methodology this project has developed over 101 demos.

**What gap it fills:**

The connections.md document identifies the pattern but only covers 3 instances in a few paragraphs. The narrative covers all instances but embedded in 40,000 tokens of other material. This document extracts the pattern, catalogs all instances, and makes the methodology explicit and repeatable. It is a research methodology document disguised as a catalog.

---

## Summary: The Four Documents as a Learning Architecture

| Document | Function | Size | When to read |
|----------|----------|------|-------------|
| `prerequisites.md` | "What do I need to know first?" | ~200 lines | Before diving into any specific topic |
| `glossary.md` | "What does this word mean?" | ~300 lines | During reading, on demand |
| `key-insights.md` | "What are the big ideas?" | ~100-400 lines | After briefing, before narrative |
| `wall-patterns.md` | "How does this project think?" | ~200 lines | After key insights, for methodology |

Together with the existing documents, these create a five-layer progressive disclosure system:

```
briefing.md          (~50 lines)    "What is this?"
  |
key-insights.md      (~100 lines)   "What are the big ideas?"
  |
wall-patterns.md     (~200 lines)   "How does the research work?"
prerequisites.md     (~200 lines)   "What concepts depend on what?"
glossary.md          (~300 lines)   "What does X mean?"
  |
narrative.md         (~800 lines)   "Tell me the whole story"
connections.md       (~700 lines)   "How does everything connect?"
four-lenses.md                      "What does each framework say?"
  |
demo-index.md                       "Give me every detail of every demo"
```

A human returning after three weeks reads: briefing -> key insights -> prerequisites for the area they are working on -> glossary as needed -> relevant sections of narrative. An LLM cold-starting reads: briefing -> key insights Level 1 -> glossary entries for terms in the current task -> relevant wall patterns if hitting a zero result.

The total cost to produce these four documents is moderate (each is a synthesis/extraction task, not original research), and the maintenance cost is low (append to glossary and wall-patterns as new demos are added; key-insights is stable once written; prerequisites updates only when a genuinely new concept cluster emerges).
