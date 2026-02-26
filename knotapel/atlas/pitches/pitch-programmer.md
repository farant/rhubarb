# Programmer Pitch: New Atlas Documents

Date: 2026-02-26

---

## Pitch 1: Demo Cookbook -- `synthesis/demo-cookbook.md`

### What it would contain

A practical guide for writing the NEXT demo, distilled from patterns across 101 existing demos. Organized as a sequence of decisions the programmer faces, with recommended approaches for each:

1. **Skeleton structure.** The standard demo layout: header comment (thesis, parts, theoretical foundation), includes, test infrastructure, type definitions, arithmetic module, computation core, search/enumeration harness, output/verification, main(). Every demo follows this shape but nobody has written it down. Include a ~50-line annotated template.

2. **Ring selection guide.** A decision tree: "What evaluation level are you working at?" -> "Which cyclotomic ring?" -> "Copy this arithmetic module." Currently a programmer has to read 6+ demos to figure out that Z[zeta_8] is the default, Z[zeta_16] is for ell=4, Eisenstein for ell=3, etc. The guide would map (ell, delta) -> (ring type, dimension, key properties, source demo to copy from, known pitfalls).

3. **Search space estimation.** Before writing a brute-force enumeration, you need to know if it will finish. A table of actual search space sizes encountered across demos: braid enumeration (4^k for 3-strand, 6^k for 4-strand), catalog sizes at each ring, combinatorial searches (C(100,3) = 161,700 triples, C(24,4) = 10,626 quadruples, 100^4 = 100M quartets). Include wall-clock estimates where available. This lets someone estimate feasibility before committing to an approach.

4. **Activation function selection.** A matrix: activation x encoding x what it can compute. Currently spread across Demos 28, 47, 50, 55, 57, 65, 66, 67, 77. The cookbook would consolidate: "If you want all 13 NPN classes, use MVN k=6 with additive encoding. If you want XOR8, use combined Sec(k)xVor. If you want just 11/13, Re>0 multiplicative works at every ring tested."

5. **Common pitfalls.** Extracted from debugging sessions and worklog entries:
   - Float sector boundaries: exact Cyc8 arithmetic eliminates 420 spurious failures near sector boundaries (D64 lesson)
   - M_PI definition: always guard with `#ifndef M_PI`
   - Union-find: path compression matters at scale; path-halving variant in later demos
   - Modular arithmetic prime selection: not all primes split all cyclotomic polynomials (the p=10^9+7 vs ell=5 bug from D39)
   - Search space overflow: when 4^8 = 65,536 braids per config vs 6^8 = 1.7M, plan memory accordingly

### Why it matters

Right now, writing a new demo means reading 2-3 existing demos to find the right boilerplate, type definitions, and patterns. This is the biggest time cost in the workflow. A cookbook reduces new-demo bootstrapping time from ~30 minutes of reading to ~5 minutes of lookup. It also reduces the risk of copying an older, buggier version of a pattern when a refined version exists in a later demo.

### What gap it fills

The code-assets document catalogs WHAT exists and WHERE. It does not explain WHEN to use each asset or HOW to combine them. The cookbook bridges code-assets (reference) to actual demo construction (workflow).

---

## Pitch 2: Computational Methodology Patterns -- `synthesis/methodology-patterns.md`

### What it would contain

Recurring computational STRATEGIES (not code snippets) that appear across multiple demos, abstracted to the level where they could guide approach selection for new problems.

**Pattern: Exact-then-float validation.** Demo 29 established the workflow: compute in exact cyclotomic arithmetic, cross-validate with float conversion (cyc8_to_cx < 1e-10). This pattern repeats in D35, D48, D49, D50, D53, D54. The document would name this pattern, describe its invariant, and specify when it applies and when it doesn't.

**Pattern: Exhaustive-then-characterize.** The dominant research methodology. Enumerate all X. Count solutions per category. Look for zeros, universals, and hierarchies. Demos 48, 50, 61, 63, 64, 65, 76, 77 all follow this shape. The document would describe the common structure: build catalog -> enumerate combinations -> classify results -> find the surprise.

**Pattern: Negative-result-as-positive.** Many landmark results are impossibility theorems: XNOR3 has 0 solutions (D48), neglecton weights add no new angles (D49), XOR6 impossible at Z[zeta_16] (D65), Hopf phase carries zero DKC information (D67), multiplicative braid product cannot compute XOR6 (D73). The document would describe how to design an experiment whose negative result is as informative as a positive one.

**Pattern: Dual-invariant refinement.** Compute two independent invariants of the same object, then analyze their separation and overlap. D74 (bracket vs quaternion: 97.8% separation), D75 (angle vs cell: independent partitions), D77 (S^1 x S^2: neither alone suffices).

**Pattern: Modular arithmetic for exact algebra at scale.** When exact integer arithmetic overflows or when you need Gaussian elimination over a number field, work mod a large prime. Demos 38, 39, 52, 60, 85, 86 all use this. Choose p > 10^9, verify the cyclotomic polynomial splits mod p, use two independent primes for cross-validation.

**Pattern: Recursive capacity ladder.** Build XOR_n from XOR_{n-2} parent solutions. D78 and D79 establish this: find all XOR6 winners, extend each by 2 new weights to XOR8, extend XOR8 to XOR10.

### Why it matters

Code assets tell you what tools exist. Methodology patterns tell you what APPROACH to take. A new demo writer needs to decide: "Should I do exhaustive enumeration or modular arithmetic? Should I work in exact cyclotomic or float? Should I compute one invariant or two?" These are design decisions that happen before a single line of code is written. Currently they require reading the explorer's log or narrative to absorb implicitly. Making them explicit accelerates decision-making and reduces false starts.

### What gap it fills

This sits between code-assets (implementation reference) and narrative (story of what happened). It captures the reusable intellectual infrastructure -- the THINKING that recurs across demos, independent of the specific mathematical content.

---

## Pitch 3: Memory and Performance Atlas -- `inventory/performance-atlas.md`

### What it would contain

A systematic catalog of computational cost across the demo space, organized by bottleneck type. Three sections:

**Section 1: Search space sizes and runtimes.** For every demo that does exhaustive enumeration, record: what was enumerated, how many items, actual or estimated wall-clock time, memory high-water mark. Include the scaling laws: braid enumeration is O(g^k) where g = 2*(n-1) generators. Catalog growth saturates (100 values from 65K braids at zeta_8). Quaternion closure is finite (24 elements from 87K braids).

**Section 2: Memory layout analysis.** Per-element sizes: Cyc8 = 32 bytes, Cyc16 = 64 bytes, Cx = 16 bytes. TL matrix at n=5 is 42x42 = 1,764 entries. TL matrix at n=8 is 1430x1430 = 2M entries. At what n does the TL multiplication table exceed available memory? These are the constraints that determine whether a demo is feasible.

**Section 3: Known performance cliffs.** Transitions where computation goes from seconds to hours or from feasible to infeasible. For each cliff, note what optimization technique was used to push the boundary (precomputation, modular arithmetic, delta=0 simplification, early exit).

### Why it matters

Two of the most expensive mistakes in computational research are: (1) starting an enumeration that will not finish, and (2) running out of memory mid-computation with no checkpoint. A performance atlas makes the boundaries visible before you hit them. It also documents which optimization techniques were effective at which boundaries.

### What gap it fills

The floating-point audit tracks a qualitative property (what kind of FP usage). The performance atlas tracks quantitative properties (how much time, how much memory, where are the walls). Neither code-assets nor the FP audit captures this dimension.

---

## Pitch 4: Test Strategy Guide -- `synthesis/test-strategy.md`

### What it would contain

A catalog of verification strategies used across 101 demos, organized from strongest to weakest:

**Tier 1: Algebraic identity.** The result is proven by exact arithmetic. Examples: cyc8_mul verified by checking zeta_8^4 = -1 (D29), TL generator relations verified by exact matrix multiplication (D35), deletion-contraction closure verified for every solution at every level (D64). The test IS the proof.

**Tier 2: Cross-representation agreement.** The same quantity computed two independent ways. Examples: state-sum bracket == TL matrix trace closure (verified at every ring), braid closure PD bracket == braid state-sum bracket (D07), modular arithmetic rank at two independent primes (D60).

**Tier 3: Float cross-validation.** Exact result converted to float matches independent float computation within tolerance. The standard cyc8_to_cx < 1e-10 check.

**Tier 4: Regression counts.** "Demo N found K solutions; after changes, still K solutions." Catches regressions but doesn't prove initial correctness.

**Tier 5: Statistical consistency.** Spearman correlations, solution count ratios, non-monotonicity patterns. Weakest individually but useful as a sanity check ensemble.

For each tier: which demos use it, a concrete example, what class of bugs it catches, what class it misses.

**Section 2: When tests fail.** A troubleshooting decision tree distilled from actual debugging:
- Segfault in test suite -> check if it's actually in the NEXT test (stdout buffers, stderr doesn't)
- Exact arithmetic disagrees with float -> check for integer overflow in cyclotomic multiplication
- Modular arithmetic gives wrong rank -> check that the prime splits the relevant cyclotomic polynomial
- Search finds 0 solutions where expected -> check activation function boundary handling

### Why it matters

The project has 101 demos and no cross-demo regression suite. Each demo has inline tests. As shared infrastructure grows (cyclotomic arithmetic, union-find, NPN classification), the risk of subtle bugs increases. A test strategy guide makes explicit which verification strategies are available and how strong they are, so the programmer can choose the right level for each new demo. It also captures debugging knowledge that would otherwise be lost across context compactions.

### What gap it fills

Code-assets documents WHAT code exists. The test strategy guide documents HOW to verify that code is correct. This is the "quality assurance" dimension of the atlas, which currently has no dedicated document.

---

## Summary Table

| # | Document | Location | Gap Filled |
|---|----------|----------|------------|
| 1 | Demo Cookbook | `synthesis/demo-cookbook.md` | How to USE code assets to build a new demo |
| 2 | Methodology Patterns | `synthesis/methodology-patterns.md` | Reusable computational STRATEGIES above the code level |
| 3 | Performance Atlas | `inventory/performance-atlas.md` | Time/memory/feasibility data for computational planning |
| 4 | Test Strategy Guide | `synthesis/test-strategy.md` | Verification approaches and debugging knowledge |
