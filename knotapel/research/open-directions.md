# Open Directions for DKC Research

*Updated February 2026. Ranked by promise and testability.*

---

## Tier 1: Natural Next Steps

These are concrete, testable, and could be implemented as demos.

### 1. Scaling Test: Beyond XOR

**Question:** Does forward DKC saturation survive for AND, OR, majority, parity-3?

**Why it matters:** XOR saturation (~35% of triples) might be an artifact of XOR's specific symmetry (balanced Boolean function). If AND/OR are also saturated, the result is general. If not, the Vassiliev hierarchy (see #4) predicts which functions should be harder.

**Prediction:** Simpler functions (AND, OR) will be MORE saturated than XOR. Complex functions (parity-3) will be less saturated. XOR's balance makes it middle-difficulty, not easiest.

**Demo:** Forward DKC with target function as a parameter. Enumerate triples, count valid ones per function. Compare saturation rates.

### 2. Perturbation Snap-Back Test (Mostow Rigidity in Weight Space)

**Question:** Do working bracket triples break under small perturbation and recover when snapped back to the nearest lattice point?

**Why it matters:** This is the testable prediction of Mostow rigidity applied to DKC. If Z[zeta_8] weight triples sit on isolated rigid points (not in basins of attraction), then:
- Small epsilon perturbation → XOR computation fails
- Snap back to nearest Z[zeta_8] lattice point → XOR computation works again
- The "snap-back" IS Mostow rigidity manifesting in weight space

**Demo:** Take 100 working triples. Add Gaussian noise (various sigma). Measure: (a) failure rate vs. sigma, (b) recovery rate after lattice projection.

### 3. Other Cyclotomic Rings

**Question:** How does "computational density" (fraction of valid triples) vary across cyclotomic rings?

**Rings to test:**
- Z[zeta_3] (Eisenstein integers) — degree 2, delta = 1
- Z[zeta_5] (golden ratio field) — degree 4, BQP level 3
- Z[zeta_6] — degree 2, delta = -1
- Z[zeta_12] — degree 4, delta = sqrt(3)

**Why it matters:** Each root of unity gives a different delta value, hence different TL algebra structure. The relationship between delta and computational density would reveal whether DKC is a property of the specific ring or a general phenomenon.

**Prediction (from Vassiliev hierarchy):** delta = 0 (our case) is maximal degeneracy, hence maximal saturation. Other delta values will have lower saturation but richer catalogs (more distinct values).

---

## Tier 2: Deeper Explorations

These require more mathematical machinery but have high potential payoff.

### 4. Vassiliev Decomposition of the Catalog

**Question:** Do low-type Vassiliev invariants predict which bracket values are XOR-capable?

**Background:** Vassiliev invariants form a hierarchy (type 0, 1, 2, ...) that decomposes knot invariants by "resolution level." Jones polynomial coefficients ARE Vassiliev invariants. The bracket catalog has internal hierarchy, not just a flat list of 64 values.

**Prediction:** XOR only needs low-type information. This is WHY the catalog is saturated — most bracket values carry enough low-type data. Complex functions require higher types, thinning the catalog.

**Demo:** Decompose catalog values by Vassiliev type contribution. Correlate low-type dominance with XOR solvability. If confirmed: Vassiliev type predicts computational complexity of achievable functions.

### 5. Multi-Layer Forward DKC

**Question:** Can every weight in a 2+ layer network be a bracket value?

**Why it matters:** Single-neuron XOR uses 3 weights. A 2-layer network for more complex functions might need 10-20 weights. If all are bracket values, the combinatorial explosion of valid configurations is enormous, potentially enabling DKC for classifiers and richer functions.

**Grounded by D1 (bijection chain):** Each weight is a rigid geometric object, not just a number. A multi-layer forward DKC network is a composition of rigid topological configurations.

### 6. Selection Question: Where Does Gradient Descent Land?

**Question:** Given 10,357+ valid XOR triples, which one(s) does gradient descent find?

**Demo:** Train 1000 complex networks on XOR with random initialization. Project each trained weight set onto the Z[zeta_8] lattice. Plot the distribution. Do they cluster? Is there a "preferred" region of the solution manifold?

**Why it matters:** If training always lands near the same few triples, that says something about the loss landscape geometry. If it's uniformly distributed, that says something different. Either answer is informative.

### 7. The Compiler

**Question:** Can we go directly from a target function specification to optimal bracket triples?

**Vision:** Target function → search bracket catalog → select optimal triple (by some criterion: minimal braid length, maximum noise margin, etc.) → output neural network weights. Topology-driven neural architecture search.

**Relationship to WANNs:** This is the dual of Gaier-Ha's weight agnostic neural networks. WANNs fix weights, search topologies. The DKC compiler fixes topology, searches weights (from a finite, pre-computed catalog).

---

## Tier 3: Long-term / Speculative

### 8. Abramsky Depth: Full Semantics

**Question:** What is the complete characterization of which bracket triples compute which functions, using Abramsky's planar lambda calculus?

**Why it matters:** Abramsky proved TL diagrams can encode computation. The bracket state sum evaluates TL diagrams. The full topology → algebra → computation → semantics bridge would make DKC a formal system, not just an empirical observation.

### 9. Topological Error Correction for NN Weights

**Question:** Can Mostow rigidity provide natural error correction for neural network weights?

**Idea:** Weights on the Z[zeta_8] lattice can be "decoded" after perturbation (snap back to nearest lattice point). The cyclotomic ring provides natural error-correcting structure. This is the exact analog of what the algebraic NT + codes literature (Oggier-Viterbo, Tunali et al.) does for communication.

**Formal version:** Define a Z[zeta_8]-linear code over bracket values. Characterize its minimum distance. Prove that perturbations within the minimum distance can be uniquely decoded.

### 10. Volume Conjecture Connection

**Question:** As we scale up braid color (higher representations), do bracket values converge to geometric volumes?

**Why it matters:** If yes, DKC connects to the deepest open problem in knot theory (E5 in the correspondence document). Catalog completeness is secretly Vassiliev completeness.

### 11. Quantum Extension

**Question:** What we do classically with small braids (catalog up to length 8), a quantum computer could do for arbitrary braids in poly time (Freedman-Kitaev-Wang). Can forward DKC scale with quantum evaluation?

**Implication:** The catalog is the classical shadow of a quantum computation. With quantum bracket evaluation, the catalog becomes unlimited — forward DKC for arbitrary functions.

---

## Testable Predictions Summary

| # | Prediction | Falsifiable? | Demo Needed |
|---|-----------|-------------|-------------|
| P1 | AND/OR saturation >= XOR saturation | Yes | Scaling test |
| P2 | Parity-3 saturation < XOR saturation | Yes | Scaling test |
| P3 | Perturbed triples fail, snapped-back triples recover | Yes | Perturbation test |
| P4 | delta=0 gives maximal saturation across rings | Yes | Multi-ring comparison |
| P5 | Low Vassiliev type predicts XOR solvability | Yes | Vassiliev decomposition |
| P6 | Trained networks cluster on a subset of the 10,357 solutions | Yes | Selection experiment |
| P7 | Z[zeta_8] lattice has nonzero minimum distance as a code | Yes | Code-theoretic analysis |

---

## Connection to Rhubarb / Novum Fundamentum

### Coding infrastructure
Demo 29 is ~900 lines of dependency-free C89. Could use piscina for arena allocation if catalog grows. The strict-flag-clean discipline from the library stack carries over directly.

### Show connections
- **Nominalism/Realism:** Forward DKC is the strongest realist result. Bracket values compute XOR by algebraic nature, not by training convention.
- **1x11 Nuntium:** Binary serialization = exact representation. Forward DKC = exact computation. "Sending computation without loss."
- **Borg thesis:** The Borg train (assimilate, optimize, overwrite). Forward DKC doesn't train. It understands.
- **Data:** "The structure was always there. Training merely helped you see it."
