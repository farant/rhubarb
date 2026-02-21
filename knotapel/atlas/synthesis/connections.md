# Cross-Demo Connections

How 82 demos feed into each other, what recurs, and where threads unexpectedly converge.

---

## 1. Dependency Graph: The Five Arcs

### Foundation Arc (D01-D09)
Linear chain building the bracket computation pipeline:

```
D01 (state-sum oracle) --> D02 (braid bracket) --> D03 (TL module)
     |                          |                       |
     v                          v                       v
D07 (braid->PD bridge)    D04 (spin chain)         D06 (Jones-Wenzl)
     |                          |
     v                          v
D08 (PD->braid Vogel)     D05 (scaling n=2..5)
     |
     v
D09 (Reidemeister invariance)
```

Every later demo traces back to D01's `Poly` type and state-sum oracle. The chain
has a clear pedagogical structure: compute the bracket (D01), represent it algebraically
(D02-D06), bridge representations (D07-D08), prove invariance (D09).

### Classification Arc (D10-D17)
Numerical pivot and the "what can we do with this" exploration:

```
D10 (numerical engine) --> D11 (waveforms) --> D12 (quantization)
                                |                    |
                                v                    v
                           D14 (zeros)          D13 (DKC test)
                                |                    |
                                v                    v
                           D15 (WRT angles)     D17 (multi-layer)
                                |
                                v
                           D16 (reduced bracket g)
```

D10 is the second major pivot: replaces symbolic `Poly` (808 bytes/entry) with
numerical `Cx` (16 bytes/entry), enabling 50-100x scaling. Everything from here
through D17 explores the bracket as a classification signal.

### Gate Arc (D18-D25)
Braid-as-computer: gates, circuits, closures, measurement:

```
D18 (NOT/NAND gates) --> D19 (circuit compiler) --> D20 (error landscape)
                                                         |
                                                         v
                                                    D21 (reversible gates)
                                                         |
                                                         v
D24 (rational tangles)                              D22 (min complexity)
                                                         |
                                                         v
                                                    D23 (function zoo)
                                                         |
                                                         v
                                                    D25 (closure measurements)
```

D18-D19 prove computational universality via braids. D22-D25 map the landscape of
what topology "prefers" to compute. D24 is a standalone tangent connecting bracket
values to rational arithmetic. The parity barrier first surfaces at D25 (zero
appearances across 1554 braids x 4 closures).

### DKC Arc (D26-D29 --> D45-D50)
The central research arc, split into two phases:

**Phase 1 — Reverse DKC (D26-D29):**
```
D26 (reverse: real weights)
 |
 v
D27 (reverse: complex weights, U(1) gauge)
 |
 v
D28 (activation comparison: modReLU, cardioid)
 |
 v
D29 (FORWARD DKC: exact Z[zeta_8], XOR triples) <-- LANDMARK
```

**Phase 2 — Forward DKC Zoo (D45-D50):**
```
D45 (all 13 NPN classes, forward-reverse duality)
 |
 v
D46 (per-neuron gauge, phase decoherence thesis)
 |
 v
D47 (MVN activation, correlation reversal)
 |
 v
D48 (100M quartets, XNOR3 = 0 solutions) <-- LANDMARK
 |
 v
D49 (neglecton weights, perturbation closed)
 |
 v
D50 (k-sector activation, parity at k=6) <-- LANDMARK
```

D29 is the first landmark: forward DKC confirmed, exact bracket values compute XOR.
D48 sharpens the parity wall (zero solutions out of 100M). D50 resolves it: the
wall was in the activation, not the lattice.

### Radical Anatomy Arc (D38-D39, D51-D52, D60)
Algebraic deep-dive branching from D47's conjecture:

```
D47 conjecture: "TL non-semisimplicity is the parity barrier"
 |
 v
D38 (dimension reduction: Gram rank C_{n-1} at even n, quotient isomorphism)
 |
 v
D39 (symmetry decomposition: cellular sectors, multi-delta comparison, block structure)
 |
 v
D51 (radical anatomy at delta=0, n=2..8)
 |
 v
D52 (Chebyshev generalization: delta=0,1,sqrt(2),phi,sqrt(3))
 |
 v
D60 (ell=7 cubic wall: all formulas confirmed at first cubic number field)
```

D38 proves the dimension reduction theorem (TL_n/rad isomorphic to TL_{n-1}) and
the even/odd dichotomy. D39 decomposes into cellular sectors, verifying d(n,j)^2
sizes and mapping multi-delta Gram ranks. D51 discovers the sandwich theorem and
alternating semisimplicity. D52 extends to the full Chebyshev parameter space.
D60 confirms universality at the first cubic number field (ell=7).

### Cross-ell Arc (D53-D59)
Ring-parametric replication of DKC across evaluation levels:

```
D53 (ell=3, delta=1, Z[omega])
 |
 v
D54 (ell=4, delta=sqrt(2), Z[zeta_16])
 |
 v
D55 (ell=4 Boolean search)
 |               \
 v                v
D56 (Potts-TL)   D57 (additive encoding)
                   |
                   v
                  D58 (ell=5, delta=phi, Z[zeta_5]) -- Fibonacci anyons
                   |
                   v
                  D59 (ell=6, delta=sqrt(3), Z[zeta_24]) -- Q=3 Potts
```

Each demo follows the same template: implement cyclotomic ring, verify TL relations,
build bracket catalog, run Boolean function search. The topology code (planar
matchings, diagram composition, trace closure) is copied verbatim --- completely
ring-independent.

### Proof Arc (D61-D62)
The final theoretical resolution:

```
D55,D58,D59 (11/13 wall observed at 3 independent ell values)
 |
 v
D61 (computational proof: integer arithmetic, N=360 grid)
 |
 v
D62 (analytical proof: algebraic contradictions for 0x06 and 0x1B)
```

D62 closes the investigation: the wall is geometric (parallelogram gap on circle),
not algebraic. Zero floating-point in the proof.

### Quaternionic Arc (D64-D71)
Matroid anatomy, Clifford hierarchy, and quaternionic lift:

```
D63 (angular anatomy, parity ceiling n=5)
 |
 v
D64 (parity matroid recursion: funnel, vocabulary stratification, wall d=4)
 |
 v
D65 (Clifford staircase: Z[zeta_16] test, generalized XOR6 at k=24)
 |
 v
D66 (quaternionic DKC: 24-cell emergence, Voronoi activation, 35 solutions)
 |
 v
D67 (coordinate zoo: S² sweet spot, 13-direction Voronoi 36 solutions, Hopf inertness)
 |
 v
D68 (stereographic DKC: intrinsic curvature proof, ASCII visualization)
 |         \
 v          v
D69         D70 (musica nodorum: 4 notes, music-space orthogonality)
(ζ nesting) |
 \          v
  \--> D71 (spectral DKC: 13=13 theorem, l=6 phase transition)
```

D64 reveals the parity hierarchy as a matroid minor chain and proves the n=6 wall
geometrically (self-doubling impossibility). D65 tests the Clifford hierarchy conjecture
from D64: standard Z[zeta_16] leaves the ceiling unchanged (REFUTED for standard
activations), but generalized binary labelings break it at k=24 (CONFIRMED for
generalized). D66 lifts the bracket from S¹ to S³, discovers the 24-cell as the
natural braid geometry, and explains the k=24 threshold as a projection of its 24
vertices. D67 proves S² (rotation axis) is the computational sweet spot and that the
Hopf fiber phase is completely inert. D68 proves the computation is intrinsically
curved and cannot be flattened to R². D69 proves universal direction nesting (ζ_8 ⊂
ζ_16 ⊂ ζ_32 exactly on S²) and explains trivialization at finer roots. D70 maps the
algebraic structure to music and confirms Hopf orthogonality. D71 closes with a
degrees-of-freedom proof: minimum bandwidth l=6 because 2×6+1=13 equals the
eigenvector direction count.

### Spherical Design and Automaton Arc (D72-D75)
Algebraic optimality, automaton structure, invariant theory, and binocular channels:

```
D71 (DOF argument: 13 directions, l=6 bandwidth)
 |
 v
D72 (spherical design test: t=0 design, anti-correlation between geometry and computation)
 |
 v
D73 (eigenvector automaton: DKC is additive not multiplicative, 82.8% universal determinism)
 |
 v
D74 (computational invariant: bracket vs cell incomparable, 119 combined classes)
 |
 v
D75 (binocular DKC: angle subsumes cell for sums, 2.3× capacity, √n magnitudes)
```

D72 shows that the 13 directions achieve the worst possible geometric distribution (t=0)
yet 2.6× better-distributed directions give 9× fewer solutions (4 vs 36). The algebraic
placement is sharp to ~5° (Voronoi cell boundary). D73 establishes that DKC is
fundamentally additive: the multiplicative braid product cannot compute XOR6 (complement-
all-bits maps M to M⁻¹, which shares the same eigenvector cell). Additive 82.8%
determinism is a universal constant across all 36 winning triples. D74 proves the bracket
and Voronoi cell are incomparable invariants (neither refines the other), with the combined
(bracket, cell) pair giving 119 classes vs 100 bracket-alone. D75 discovers that at the
sum level, the eigenvalue angle subsumes the cell entirely (angle perfectly predicts parity
for all 36 winners), giving 2.3× capacity over cell-only, while the multiplicative channel
(which stays inside the 24-cell group) has zero XOR6 solutions.

### Scaling and Finiteness Arc (D76-D82)
The wall-was-activation pattern recurs, then group finiteness governs capacity:

```
D75 (binocular DKC, 6-input complete)
 |
 v
D76 (scaling to 8 inputs: 0 winners under S² Voronoi) <-- WALL
 |
 v
D77 (activation zoo: XOR8 solved at S¹×S² activation, 6 winners) <-- RESOLUTION
 |
 v
D78 (recursive scaling: XOR10 = 0 at ζ₈, wall confirmed exhaustively) <-- WALL
 |
 v
D79 (ζ₁₂ capacity: XOR10 = 124 winners, XOR12 = 50+) <-- RESOLUTION
 |
 v
D80 (group finiteness: only ζ₄ and ζ₈ finite; ζ₁₂ infinite = no hard ceiling)
 |
 v
D81 (scaling law: max_xor ≈ 0.62 × log₂(catalog) + 4.6 for ζ₁₂)
 |
 v
D82 (crossing depth law: max_xor ≈ depth + 6; log law is corollary of depth law)
```

D76 confirms zero XOR8 solutions at ζ₈ with S²-only activation (pigeonhole: 256 masks /
14 cells ≈ 18.3 per cell). D77 resolves this by adding the S¹ (eigenvalue angle) channel:
S¹×S² product activation with 112 cells gives 6 XOR8 winners. D78 then exhaustively
confirms XOR10 = 0 at ζ₈ under all activation functions tested — the ζ₈ XOR hierarchy
terminates at 8 inputs because the binary octahedral group has only 24 elements and the
catalog's combinatorial diversity is exhausted at 5-element selection. D79 breaks this
wall by switching to ζ₁₂, whose group is infinite (or very large): 124 XOR10 winners,
50+ XOR12 winners. D80 surveys 7 roots and finds only ζ₄ and ζ₈ generate finite SU(2)
subgroups; all others (including ζ₁₂) are infinite. D81 measures the logarithmic scaling
law for ζ₁₂. D82 reinterprets it: depth (group-closure birth round = number of generator
multiplications = crossing number) is the fundamental variable, and the logarithmic law is
a corollary of the linear depth law plus the exponential catalog growth per round.

---

## 2. Code Reuse Patterns

### The "Standard Library" That Emerged

Four code assets appear in 30+ demos and constitute a de facto standard library:

| Asset | Introduced | Used in | Role |
|-------|-----------|---------|------|
| Complex arithmetic (`Cx`) | D10 | 35 demos | Numerical workhorse |
| Union-find loop counter | D02 | 37 demos | State-sum backbone |
| NPN classification | D23 | 13 demos | Boolean function taxonomy |
| Braid word decoder | D22 | 13 demos | Exhaustive enumeration |

### Ring Tower

A progression of cyclotomic rings, each built for a specific evaluation level:

```
Z (integers) -- D01-D09
  |
  Cx (floats) -- D10-D28
  |
  Z[zeta_8] -- D29, D35, D48-D50
  |
  Z[omega] -- D53
  |
  Z[zeta_16] -- D54-D57, D65, D69
  |
  Z[zeta_5] -- D58
  |
  Z[zeta_24] -- D59
  |
  Z[zeta_32] -- D69 (braid group infinite; enumeration cap-limited)
  |
  H (quaternions, SU(2)) -- D66, D67, D68, D69, D70, D71, D72-D82
  |
  ζ₁₂ group (infinite SU(2) subgroup) -- D79, D81, D82
```

D72-D82 all work in the H / SU(2) quaternion setting. D76-D79 additionally introduce
ζ₁₂ (half-angle π/6) as a new generator, revealing that different roots produce
non-nesting finite or infinite SU(2) subgroups with dramatically different capacity
profiles. D80 surveys 7 roots and classifies their finiteness.

Each ring carries the same API surface: zero/one/add/mul/eq/to_cx/print.
The TL matrix engine sits on top, parameterized by ring. Swapping the coefficient
ring is the only change between D53, D54, D58, and D59. The quaternionic arc (D66+)
introduces a genuinely different algebra — unit quaternions as SU(2) braid
representations — complementary to the cyclotomic scalar bracket.

### The Two Oracles

Every new computation method is validated against one of two oracles:

1. **State-sum oracle** (D01): brute-force 2^n enumeration, ground truth for brackets.
   Used to validate the matrix method (D03-D06), numerical engine (D10), TL matrix
   method (D35, D53-D59).

2. **Forward DKC catalog** (D29): exact Z[zeta_8] bracket values, ground truth for
   Boolean function reachability. Used to validate trained network decomposition
   (D45-D47) and activation function analysis (D48-D50).

### Copy-Paste Pattern

There is no shared library extraction. Each demo is standalone `main.c`. This means:
- Bug fixes don't propagate (MAX_TERMS varies: 64 in D01, 120 in D04+)
- Each demo compiles independently (a feature, not a bug)
- The "library" is pattern knowledge, not shared object files
- Ring-parametric code (D53-D59) is the closest to a true abstraction

---

## 3. Recurring Themes

### Exact vs Approximate Arithmetic

This tension runs through the entire project. The symbolic polynomial ring (`Poly`,
D01-D09) is exact but expensive. The numerical engine (`Cx`, D10+) is cheap but
approximate. The cyclotomic rings (D29+) are exact AND efficient for specific
evaluation points.

Key moments: D29 shows exact Z[zeta_8] arithmetic makes forward DKC possible.
D48's 100M-quartet exhaustive search depends on exact equality testing. D61-D62's
proofs use pure integer arithmetic with zero floating-point.

The pattern: approximate methods discover phenomena, exact methods prove them.

### Non-Semisimplicity as Both Obstacle and Resource

At delta=0, TL generators are nilpotent (e_i^2 = 0). This makes the algebra
non-semisimple for even n (D51), which:

- **As obstacle**: The radical kills degrees of freedom needed for parity (D47
  conjecture, D48 confirmation). Bracket evaluation factors through the semisimple
  quotient TL/rad, losing radical information.

- **As resource**: Nilpotency at delta=0 means only single-loop states survive,
  enabling the delta=0 simplification (D29) that makes exact computation feasible.
  The very constraint that blocks parity is what enables exact DKC for 12/13 classes.

D49 tests whether radical elements (neglectons) can break the parity wall. They
cannot --- the perturbation approach is closed. D50 resolves this by changing the
activation function instead of the weight space.

### The Activation Function as the Key Variable

The project converges on activation geometry as the determining factor for
computational capability:

- Split-sigmoid (D27-D29, D45-D48): sees magnitudes, destroys phase. 12/13 NPN
  classes via forward DKC, parity impossible.
- MVN-continuous (D47): sees phases, different correlation structure. Complementary
  to split-sigmoid.
- k-sector (D50): parity reachable at k=6 with 906 solutions from the same catalog
  that split-sigmoid searched 100M times finding zero.
- Re>0 half-plane (D55, D58, D59, D61-D62): 11/13 universal across all ell values.
  Wall proven geometrically.

The lattice is fixed; the activation chooses which subset of its structure to access.

### Topology -> Algebra -> Computation Pipeline

The project's central pipeline:

```
Knot/braid topology  -->  TL algebra / bracket values  -->  Boolean computation
     (D01-D09)              (D29, D35, D51-D52)              (D18-D25, D45-D50)
```

Each arrow is non-trivial. The first collapses the infinite braid group to a
finite-dimensional TL algebra (compression ratios measured in D22, D35). The second
maps cyclotomic integers to truth tables via activation-mediated classification.

### Angular Structure and Its Geometric Meaning

Bracket values at delta=0 are axial: they lie on 8 rays in the complex plane
(D35 axiality theorem). At delta=sqrt(2), values are Z[i]-axial: 2D subspace
of the 8D ring (D54). At delta=phi, full 4D usage (D58).

This angular structure determines computational capability:
- The 8 axes of Z[zeta_8] create a discrete angular alphabet
- k-sector activation reads this alphabet directly (D50)
- Half-plane activation projects it to a binary partition (D61-D62)
- The parity function requires 3 odd sectors at 120-degree spacing (triskelion
  principle, D50) which is incommensurable with the pi/4 lattice at k=2

### Dimensional Projection and the S² Sweet Spot

The quaternionic arc (D66-D71) establishes a hierarchy of projections:

```
SU(2) / S³ (full quaternion)  -->  S² (rotation axis / Hopf base)
     D66: 35 solutions               D67: 36 solutions (more, fewer cells)
         |                                |
         v                                v
     S¹ (complex trace)             R² (stereographic)
     D65: 25 cells needed           D68: visualization only, not workspace
```

The counter-intuitive result: projecting DOWN increases computational efficiency.
The full S³ geometry is redundant; the Hopf fiber phase (S¹) is completely inert
(r = -0.06 with spatial direction, D67/D70). The computation is entirely determined
by the rotation axis on S². Moreover, R² is insufficient — the computation is
intrinsically curved (D68: Euclidean metric gives zero solutions, S² metric gives 36).

The 13-direction S² Voronoi is data-derived, not Platonic: octahedral (6 cells),
icosahedral (12 cells), cuboctahedral (12 cells), dodecahedral (10 cells) all give
zero XOR6 solutions (D67, D70). The natural polyhedral symmetries are wrong; only
the braid-intrinsic 13 eigenvector directions work.

### The 13-Direction Structure and Its Rigidity

The binary octahedral group produces exactly 13 distinct rotation-axis directions
(mod ±) from the 24 SU(2) quaternions. This 13 is not arbitrary:
- It matches the minimum sectors needed for XOR6 on S¹ after projection (D65 k=24
  connected to D66 24-cell vertex count → D67 13-direction Voronoi)
- 2×6+1=13, so l=6 spherical harmonics are exactly sufficient to address them (D71,
  the 13=13 theorem)
- All 13 directions nest exactly into ζ_16 and ζ_32 catalogs (D69 direction nesting
  theorem), with ζ_8 the coarsest root producing this structure

Finer cyclotomic roots produce thousands of additional directions, causing
pigeonhole trivialization (~90% XOR6 rate at ζ_16) — the 13-direction structure is
the computational optimum, not a stepping stone.

### Music-Space Orthogonality

The quaternionic structure separates into two orthogonal dimensions (D67, D70):
- **Eigenvalue angle θ** (trace): maps to pitch. Four values → four notes (C, Eb,
  E, F#), spanning unison to tritone. Yang-Baxter equivalent braids produce
  identical melodies step by step.
- **Eigenvector direction** (rotation axis): maps to space. 13 distinct directions
  determine all DKC computational information.

Pearson r = -0.06 between pitch and spatial direction across 253 pairs — effectively
zero, confirming these are independent dimensions of the quaternion. The Hopf
fibration S³ → S² physically realizing this split: fiber (S¹, phase ξ₁) carries the
pitch; base (S², rotation axis) carries the computation. Music is the fiber; logic
is the base.

### Algebra Beats Geometry

D72 crystallizes a principle latent throughout the project: the 13 eigenvector directions
are algebraically selected, not geometrically optimized, and geometric improvement
destroys computation. The 13 directions achieve t=0 design (worst possible for sampling)
but stochastic optimization to t≈1 drops XOR6 solutions from 36 to 4. The computation is
algebraic, not geometric. Moving directions to "better" positions breaks the Voronoi
partition structure.

This principle recurs at every level:
- D68: Euclidean metric on R² gives zero solutions; S² geodesic metric gives 36
- D71: the DOF coincidence 2×6+1=13 is algebraic, not a sampling theorem
- D72: geometric distribution quality and computational quality are anti-correlated
- D82: algebraic coherence (shared intermediate products at depth d) beats vocabulary
  (more directions and angles from strided sampling)

### The "Wall Was X Not Y" Pattern

A structural pattern that recurs three times, always with the same form: an apparent
capacity ceiling is identified, exhaustive search confirms zero solutions, then the
wall is located in a specific component and bypassed by changing that component:

1. **D48→D50**: XOR parity at ζ₈ has zero solutions out of 100M quartets with split-sigmoid
   activation. Wall was in the **activation** (split-sigmoid destroys phase), not the
   lattice. Resolution: k-sector activation, 906 solutions at k=6.

2. **D76→D77**: XOR8 at ζ₈ has zero solutions out of 10,626 quadruples with S²-Voronoi
   activation. Wall was in the **activation** (S²-only discards eigenvalue angle), not
   the root of unity. Resolution: S¹×S² product activation, 6 solutions.

3. **D78→D79**: XOR10 at ζ₈ has zero solutions exhaustively confirmed under all tested
   activations. Wall was in **group finiteness** (binary octahedral group closes at 24
   elements), not the architecture. Resolution: ζ₁₂ (infinite group), 124 solutions.

The pattern teaches: before concluding a function is unreachable, locate the specific
component enforcing the wall. The topology/algebra is almost never the ultimate bottleneck.

### DKC is Additive

D73 proves formally what D29 and D50 assumed operationally: DKC is fundamentally an
additive computation (weight vector sum), not a multiplicative one (braid matrix product).
The algebraic reason: complementing all input bits maps the braid matrix M to its inverse
M⁻¹, which shares the same eigenvector cell (antipodal identification), but changes
parity. Multiplicative braid products are structurally parity-blind. This connects
retroactively to the entire forward DKC arc (D29, D48-D50): those demos always used
additive weight vectors and the present demo explains why multiplicative formulations were
never viable.

The same finding appears independently in the Nazer-Gastpar connection noted from D29: the
"compute-and-forward" structure of lattice codes is additive by design. D73 confirms DKC
is a concrete instance of that framework.

### Finite vs Infinite Group as the Capacity Governor

D79 and D80 establish that the fundamental variable governing maximum XOR capacity is
group finiteness, not the root of unity index N or the arithmetic details of the ring.
A finite quaternion group (ζ₄: Q₄ with 4 elements; ζ₈: binary octahedral with 24) imposes
a hard ceiling — the catalog closes, combinatorial diversity saturates, and XOR capacity
terminates. An infinite group (all other tested roots: ζ₆, ζ₁₀, ζ₁₂, ζ₁₆, ζ₂₀) has no
such ceiling within the explored range.

The two finite cases correspond to quantum-dimension-zero points ([2]_q = 0 at ζ₄ and ζ₈),
where the TL algebra is maximally non-semisimple. These are exactly the lattice roots where
Kuperberg's #P-hardness theorem does NOT apply — the tractable computational regime. The
non-lattice infinite-group roots are where hardness applies, and those roots are also where
unbounded XOR capacity is possible.

### Crossing Depth as Computational Capacity

D82 reinterprets the scaling law from D81 through the lens of knot theory: each closure
round = one generator multiplication = one crossing in the braid/knot sense. A depth-d
quaternion encodes d crossings of the associated knot diagram. The capacity law
max_xor ≈ depth + 6 says computational power scales linearly with crossing depth.

This connects knot topology directly to computational capacity: more complexly knotted
strands compute more than many simply-knotted strands with the same or greater vocabulary.
The logarithmic scaling law from D81 is a corollary: catalog grows exponentially per depth
round (roughly 2× each round), so log₂(catalog) ≈ depth, giving
max_xor ≈ 0.62 × log₂(catalog) + const.

Consistent with Kuperberg's #P-hardness: the hardness is in *generating* deep entries
(exponential BFS cost to compute d crossings), while the algebraic gain from those entries
is linear — the #P structure reflects the generation cost, not the usage cost.

---

## 4. Surprise Connections

### Fibonacci Golden Ratio in ell=5 Ring (D58)

At ell=5, delta = phi = (1+sqrt(5))/2. Delta powers are literal Fibonacci numbers:
phi^k = F(k-1) + F(k)*phi. The "Fibonacci anyon" name is not categorical --- it is
arithmetically literal. This connects number theory (Fibonacci sequence), representation
theory (TL algebra at ell=5), and topological quantum computing (anyonic universality).

### WRT Convergence (D15)

Greedy angle selection (optimizing an information-theoretic criterion) independently
converges to within 1.5% of the WRT level-3 evaluation point from Chern-Simons theory.
The bracket "knows" which angles are informationally optimal, and those angles are
the algebraically special ones. Topology knows information theory.

### Forward-Reverse Duality (D45)

Spearman rho = -0.82 between topology frequency (how often a braid produces a function)
and decomposition quality (how well trained weights match bracket values). Functions the
bracket "likes" to produce are functions whose trained solutions live near the bracket
lattice. This was measured independently from two directions and arrived at the same
ordering.

### Correlation Reversal Under Activation Change (D47)

Split-sigmoid and MVN-continuous produce opposite correlation signs between topology
frequency and decomposition quality (rho = -0.78 vs +0.55). The two activations
literally access complementary sectors of the bracket algebra. This was not predicted.

### Neglecton Closure (D49)

The perturbation approach (adding radical-element weights from loops=2 states) yields
148 new Z[zeta_8] values from 15,242 previously invisible braids --- but ALL new values
share angles with existing standard values. No new angular directions at any perturbation
order. The weight space already contains everything; only the activation geometry limits
computation.

### 11/13 Wall Universality (D55, D58, D59, D61-D62)

Three independent evaluation levels (ell=4, 5, 6) with three different cyclotomic rings
(Z[i], Z[zeta_5], Z[zeta_24]) all produce exactly 11/13 reachable NPN classes under
Re>0 multiplicative encoding, with the same two classes (0x06, 0x1B) unreachable. This
universality was unexpected: the rings have different dimensions (2, 4, 8), different
algebraic properties, different delta values. The wall is purely activation-geometric.

### Two Different Proof Structures for Two Unreachable Classes (D62)

0x06 and 0x1B fail for different geometric reasons:
- 0x06: local parallelogram argument (four semicircle gaps all < pi)
- 0x1B: global interval squeeze (lower bound > upper bound on one angle)

Despite both being "unreachable by half-plane," they are obstructed by fundamentally
different mechanisms. This hints at a richer obstruction theory.

### Matroid Theory Enters DKC (D63-D64)

D63 introduces oriented matroids to explain the parity scaling law. D64 deepens
this by proving the parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a legitimate
matroid minor chain: deletion gives XOR_{n-1}, contraction gives XNOR_{n-1}, both
100% verified exhaustively. The upward funnel (99.3% → 39.2% → 19.2% → 0%)
collapses catastrophically at n=6, and the wall geometry is exactly explained by the
self-doubling impossibility theorem. This connects DKC for the first time to the
classical theory of matroids (Reiner's cyclotomic matroids mu_N are the candidate
framework for the 44 good / 31 poison vocabulary split).

### The k=24 Coincidence (D65↔D66)

D65 discovers generalized XOR6 first appears at k=24 sectors on S¹ — an empirical
fact with no immediate explanation. D66 independently discovers that SU(2) braid
representations produce exactly 24 distinct quaternions (the vertices of the 24-cell
in S³). These two 24s are not coincidental: the S¹ minimum sector count is the
projection (via trace) of the 24-cell's 24 vertices onto the circle. The complex
S¹ computation is a shadow of the S³ geometry.

### The 24-Cell as Braid Geometry (D66↔D29)

D29 established that the Kauffman bracket at delta=0 takes values in Z[zeta_8],
computing exact XOR triples from scalar bracket values. D66 lifts this to full SU(2):
the same braids that produce 100 distinct scalar values in Z[zeta_8] produce exactly
24 distinct unit quaternions — the binary octahedral group, vertices of the 24-cell.
The scalar bracket is the trace of the quaternion (a 1D projection of 4D geometry).
D66 Part J quantifies the information content: bracket and quaternion agree on only
12.86% of braid pairs, resolve each other's collisions at different rates (450 vs
8,424), confirming they are genuinely complementary.

### Hopf Phase Inertness (D67↔D70)

D67 proves computationally that the Hopf fiber phase ξ₁ carries zero DKC information
(zero XOR6 solutions at all k=4..32). D70 confirms this musically: pitch (eigenvalue
angle θ = arccos(a), related to trace) and space (eigenvector direction) have
Pearson r = -0.06. Two independent methods — exhaustive combinatorial search and
continuous correlation over 253 pairs — both find the fiber inert. The Hopf
fibration S³ → S² is not just a geometric description; it is the exact decomposition
separating computationally active (base) from inert (fiber) information.

### The 13=13 Theorem (D71↔D67↔D69)

D67 discovers empirically that a 13-direction Voronoi solves XOR6 at 14 cells.
D69 shows these 13 directions are universal across all ζ_N (nesting theorem) and
that ζ_8 is the coarsest root producing them. D71 explains WHY 13 is the magic
number: spherical harmonic mode l has (2l+1) independent components; 2×6+1=13;
therefore l=6 is the exact bandwidth threshold with a sharp 0%→100% phase
transition. The three demos form a closed explanatory loop: data-derived directions
(D67) → algebraic universality (D69) → harmonic-analytic necessity (D71). The DOF
formula l≥(N-1)/2 also predicts D69's trivialization: ζ_16 has 3,457 directions,
requiring l≥1728, which makes any coarse partition trivially sufficient.

### Vocabulary Stratification and the Poison Octant (D64)

Of 83 class-1 catalog values at k=15, exactly 44 appear in XOR5 solutions
("good") and 31 appear only in dead-end XOR4 solutions ("poison"). The 31 poison
values span octants {1,2,3,5,6,7} but never octant 4 — an asymmetry with no
obvious geometric explanation that connects to Reiner's cyclotomic matroid mu_8
theory (values at 8th roots of unity as matroid elements, octant 4 as a
distinguished cocircuit). The exact-arithmetic requirement (D64) — floating-point
produces 420 spurious failures at sector boundaries — echoes D29's discovery that
exact Z[zeta_8] arithmetic was essential for forward DKC.

### 4 Notes Spanning All Musical Tension (D70)

The binary octahedral group's 4 eigenvalue angles (0°, 45°, 60°, 90°) map to
exactly the four foundational interval types: unison (C, perfect consonance),
minor third (Eb), major third (E), tritone (F#, maximum dissonance). The six
pairwise intervals cover all basic dyad categories. The Garside element (σ₁σ₂)³
produces the melody Eb E F# E Eb C — a complete arch visiting all four knot notes.
The tritone-antipodal correspondence on the dodecahedron (antipodal faces carry
tritone pairs) suggests the circle of fifths and the braid group share a deeper
symmetry argument, not merely a notational coincidence.

### D72 Confirms D71's DOF Argument from the Negative Direction (D72↔D71)

D71 explains why 13 directions suffice for XOR6: the DOF coincidence 2×6+1=13 means
l=6 spherical harmonics can exactly address 13 independent directions. D72 tests this
from the opposite direction: can better geometric distribution (higher t-design) do
better? The answer is definitively no — the eigenvector configuration achieves t=0 (worst
possible), yet stochastic optimization to t≈1 (2.6× better geometry) produces 9× fewer
XOR6 solutions (4 vs 36). D72 thus confirms D71 by contradiction: if the DOF coincidence
were irrelevant and sampling quality were what mattered, better-distributed directions
would compute MORE functions, not fewer. The fact that they compute fewer proves that the
mechanism is algebraic DOF matching, not sampling theory.

### D73's Additive Finding Retroactively Connects All Forward DKC Demos (D73↔D29, D48-D50)

D73 proves that DKC is fundamentally additive. This is not a new discovery operationally
— all forward DKC demos since D29 used weight-vector sums — but D73 provides the algebraic
explanation for why multiplication was never viable: the complement-bits symmetry maps M to
M⁻¹, which shares the same eigenvector cell, making parity invisible to any multiplicative
formulation. This retroactively explains a silent design choice in D29 and every subsequent
DKC demo. It also explains why the split-sigmoid wall (D48) and the S²-only wall (D76) were
in the *activation* and not the *algebra*: the algebra was always additive-capable; the
question was always which activation could read it.

### D74 Incomparability and D75 Subsumption Concern Different Mathematical Objects (D74↔D75)

D74 proves bracket and Voronoi cell are incomparable at the individual-quaternion catalog
level (neither refines the other: 80% of bracket collisions separated by cell, 54% of cell
collisions separated by bracket). D75 proves eigenvalue angle subsumes Voronoi cell at the
sum-quaternion level (angle perfectly predicts parity for all 36 winners). These two results
are not in tension — they concern different objects. D74 is about individual catalog entries
(24 quaternions from the binary octahedral group); D75 is about additive sums of three such
entries. The subsumption in D75 emerges from the addition process breaking the 24-cell group
structure and creating 7 distinct angles (including 3 non-canonical ones) that carry strictly
more information than the 13-direction Voronoi partition.

### D76→D77 is the Same Pattern as D48→D50 (D76↔D48, D77↔D50)

D48 proved XOR parity has zero solutions out of 100M quartets with split-sigmoid activation
at ζ₈. D50 resolved this by changing the activation to k-sector, finding 906 solutions from
the same catalog. D76 proved XOR8 has zero solutions under S²-Voronoi activation at ζ₈.
D77 resolved this by changing the activation to S¹×S² product, finding 6 solutions from the
same catalog. In both cases: the topology and algebra were never the obstruction; the wall was
entirely in the activation function's choice of what to read from the quaternion. Both
resolutions use a finer activation that reads angular information the original activation
discarded. The two instances separated by 27 demos are structurally identical.

### D78→D79 is the Same Pattern Again, but the Wall Was Group Finiteness (D78↔D79↔D48↔D76)

D78 exhaustively confirms XOR10 = 0 at ζ₈ under all tested activations. Unlike D48 and D76
(where the wall was in the activation), this wall is in the group structure: the binary
octahedral group closes at 24 elements, and the combinatorial diversity of 24 elements is
exhausted at 5-element selection (quintuples). No activation change can solve XOR10 at ζ₈ —
there is no 5-element subset of 24 elements with the required parity structure. D79 resolves
this by switching to ζ₁₂ (infinite group), confirming 124 XOR10 winners. The "wall was
finiteness not architecture" resolution is the third instance of the pattern.

### D80's ADE Classification Connects to D66's 24-Cell (D80↔D66)

D66 discovered that SU(2) braid representations from ζ₈ produce exactly 24 distinct unit
quaternions — identified as the binary octahedral group, vertices of the 24-cell. D80
formally classifies this: ζ₈ (half-angle π/4, cube/octahedron symmetry angle) generates
the Binary Octahedral group 2O (ADE type E₇, order 48 as a group in SU(2) with ±
identification giving 24 distinct quaternion directions). D66 arrived at the 24-cell through
braid geometry; D80 arrives at it through the ADE classification of finite SU(2) subgroups.
The quantum dimension vanishing at ζ₈ ([2]_q = i + (-i) = 0) that D80 identifies as the
algebraic singularity is the same non-semisimplicity that the entire radical anatomy arc
(D38-D39, D51-D52) was studying from the TL side.

### D81 Scaling Law and D82 Depth Reinterpretation Form a Single Discovery Arc (D81↔D82)

D81 measures the empirical scaling law for ζ₁₂: max_xor ≈ 0.62 × log₂(catalog) + 4.6.
D82 reveals the mechanism: catalog size grows exponentially per closure round (roughly 2×),
so log₂(catalog) ≈ round_number ≈ depth. The true linear variable is crossing depth
(max_xor ≈ depth + 6), and the logarithmic law is its corollary. D81 discovers the
phenomenology; D82 identifies the underlying cause. Together they establish that
computational power in this framework scales linearly with knot complexity (crossing number),
while the cost of generating depth-d entries scales exponentially — exactly the structure
that Kuperberg's #P-hardness theorem would predict for the non-lattice regime.

### √n Magnitude Quantization (D75↔D73)

D75 finds that the magnitude of additive quaternion sums takes only 6 values: √{0,1,2,3,4,6}
(√5 is absent). D73 finds that distinct partial sums after 6 steps number exactly 27 (not
64). Both results reflect the same algebraic constraint: the inner product structure of the
24-cell allows only half-integer values {-1, -½, 0, ½, 1}, so integer sums can only reach
specific norms. The magnitude constraint (D75) explains why the partial sum space is
compressed (D73): even though 6 weights from a 24-element catalog could in principle produce
2^6 = 64 distinct sums, the lattice structure reduces this to 27 — the "odometer" that
D73's compass-without-odometer metaphor describes.

### Non-Monotonicity of Activation Count Recurs at Every Scale (D50↔D78)

D50 found that parity solutions at ζ₈ are non-monotone in k (sector count): 906 at k=6,
756 at k=7, 96 at k=8. D78 finds the same non-monotonicity for XOR10 accuracy across sector
counts: k=10 achieves 98.4%, k=12 drops to 96.9%, k=16 returns to 98.4%. Both instances
are attributed to the π/4 incommensurability of the ζ₈ lattice: certain sector boundaries
align destructively with the lattice angles, reducing rather than increasing the number of
correctly classified inputs. The pattern appears at both the "near-success" regime (D78, best
accuracy near 100%) and the "success" regime (D50, counting exact solutions), confirming
it is a structural feature of ζ₈ arithmetic, not a boundary artifact.

---

## 5. Convergence Points

### D29: The Forward DKC Moment

Threads converging:
- Exact arithmetic (D01-D09 symbolic pipeline, refined to Z[zeta_8])
- Delta=0 simplification (only single-loop states survive)
- Split-sigmoid activation (from D27 complex neuron)
- Gauge symmetry (U(1) rotation from D27)

Result: 100+ exact XOR triples from bracket values alone. Training-as-focusing thesis
confirmed. This is where the project shifts from "can braids compute?" to "what can't
they compute?"

### D50: Parity Resolution

Threads converging:
- Parity wall (D25 closure measurements, D48 exhaustive proof of zero solutions)
- Activation analysis (D28 modReLU, D47 MVN, D49 neglecton closure)
- k-sector classification (Aizenberg's MVN theory)
- Z[zeta_8] angular structure (D35 axiality, D50 triskelion geometry)

Result: Same 100-value catalog, different activation, parity solved with 906 solutions
at k=6. Four-tier NPN hierarchy. Incommensurability hypothesis. The wall was in the
activation all along.

### D51-D52: Radical Structure Meets DKC

Threads converging:
- D47's conjecture (TL non-semisimplicity as parity barrier)
- D35's TL matrix representations
- Graham-Lehrer cellularity theory (from literature)

Result: Sandwich theorem (rad^2 isomorphic to previous odd TL algebra), Catalan trace
theorem, Fibonacci rank theorem, cross-ell radical dimension formulas. Multiple novel
results connecting abstract algebra to computational barriers.

### D61-D62: The Proof

Threads converging:
- Three independent 11/13 observations (D55 at ell=4, D58 at ell=5, D59 at ell=6)
- Multiplicative encoding angular structure (angles add, from D55)
- NPN classification machinery (from D23)
- Convexity thesis (from D57)

Result: Clean analytical proofs via integer arithmetic and geometric arguments. Zero
floating-point. The 48 unreachable truth tables (orbits of 0x06 and 0x1B) are
characterized by parallelogram and interval-squeeze obstructions on the circle.

### D63: Angular Anatomy and Parity Ceiling

Threads converging:
- Triskelion geometry (D50 parity at k=6, 906 solutions)
- Z[zeta_8] axiality (D35, octant structure)
- MVN k-sector activation (Aizenberg 2008, sector-octant mapping)
- Oriented matroid theory (Bjorner et al., matroid covector lattices)
- RT truncation connection (D39 Parts G-J, Markov trace kills j >= ell-1)

Result: Complete parity scaling law for Z[zeta_8] — ceiling n=5. Extends DKC from
3-input (Demo 50) to 4-input (k=8, 96 solutions) and 5-input (k=15, 3020 solutions).
Falsifies k=2n triskelion conjecture (n=5 needs k=15 not k=10). Discovers that parity
requires the alternating oriented matroid type — the first connection between DKC and
matroid theory. Two types of impossibility: constraint-geometric wall (n=6,7) vs
pigeonhole (n>=8).

### D39 Parts G-J: The Proof Trilogy

Threads converging:
- Cellular algebra theory (Graham-Lehrer 1996)
- Fixpt vs Markov trace (two "lights" on TL_n)
- RT truncation (Reshetikhin-Turaev 1991, negligible ideal)
- Wrong conjecture as discovery tool ((ell+m-1)^2+1 refuted at m=3)

Result: Three formal proofs anchoring the algebraic side of DKC. Markov RT truncation
theorem identifies the Markov kernel as J(A) + negligible ideal. The "shadow
interpretation" — fixpt trace is algebraic light, Markov trace is topological light,
excess = what topology cannot see — provides the deepest framing of how DKC uses
pre-closure information that RT truncation would discard.

### D64: Angular Anatomy Meets Matroid Theory

Threads converging:
- D63's octant geometry and parity ceiling (n=5, oriented matroid type)
- Deletion-contraction operations from classical matroid theory
- Exact Z[zeta_8] arithmetic (from D29, critical again for sector-boundary precision)
- Vocabulary stratification from the 83 class-1 catalog values at k=15

Result: The parity hierarchy is a funnel-shaped matroid minor chain. Deletion =
XOR_{n-1}, contraction = XNOR_{n-1}, both 100% verified. Upward coverage collapses
to zero at n=6 (XOR6→XOR5: 0% of 27 extension attempts). Self-doubling impossibility
theorem explains 89.5% of closest wall misses geometrically. Vocabulary splits into
44 good / 31 poison values, with poison values avoiding octant 4.

### D65-D66: The k=24 Resolution

Threads converging:
- D64's open question: does XOR6 become achievable at Z[zeta_16]?
- D65's empirical discovery: generalized XOR6 first appears at k=24 sectors
- D66's independent discovery: braids produce exactly 24 quaternions (24-cell)

Result: Standard Z[zeta_16] activation leaves parity ceiling at n=5 (Clifford
staircase conjecture from D64 REFUTED for standard activations). But generalized
binary labelings break the wall at k=24, and D66 explains this: complex k=24 is
the projection of the 24-cell's 24 vertices onto S¹ via the trace. Two experiments
arrive at the same number from opposite directions.

### D67: The S² Discovery

Threads converging:
- D66's 24 quaternions and their eigenvector decomposition
- Hopf fibration structure (S³ → S² → S¹)
- Systematic comparison of every natural SU(2) coordinate system
- The Platonic-solid-failure pattern (octahedral, icosahedral, cuboctahedral all give 0)

Result: The computation is entirely determined by the rotation axis on S². Custom
13-direction Voronoi achieves 36 solutions at 14 cells — more than the full S³
Voronoi with 25 cells. The Hopf fiber phase is completely inert. Natural polyhedral
geometries all fail; only the data-derived 13 eigenvector directions work.

### D71: The 13=13 Theorem

Threads converging:
- D67's 13-direction Voronoi (empirical sweet spot)
- D69's direction nesting theorem (ζ_8 → ζ_16 rigid, 13/13 exact)
- D69's trivialization observation (ζ_16 at ~90% because too many directions)
- Spherical harmonic theory (2l+1 components per mode)

Result: Minimum bandwidth l=6 because 2×6+1=13. The phase transition is sharp and
all-or-nothing. Trivialization at ζ_16 is explained: 3,457 directions require
l≥1728, making any coarse partition sufficient. The 13=13 theorem closes the
loop from D67's discovery through D69's universality to D71's harmonic-analytic
necessity. Three demos form one argument.

### D72-D73: The Algebraic Optimality Loop

Threads converging:
- D71's DOF argument (13=13 theorem, bandwidth l=6)
- D67's 13-direction empirical discovery
- Stochastic spherical design optimization (D72)
- Mealy/Moore automaton theory and Nazer-Gastpar additive structure (D73)

Result: D72 confirms D71's DOF argument by demonstrating the anti-correlation between
sampling quality and computational quality — better-distributed directions destroy the
computation. D73 proves the additive formulation is the only viable one, explaining why
all forward DKC demos used weight sums rather than braid products. Together they close
the explanatory gap between the empirical 13-direction discovery (D67), the harmonic-
analytic necessity (D71), and the algebraic mechanism (D73). The computation works because
of specific angular relationships between 13 algebraically-selected directions, not
because of their geometric distribution.

### D74-D75: Two Levels of Invariant Structure

Threads converging:
- D66's bracket-quaternion relationship (12.86% agreement on braid pairs)
- D67's Voronoi cell as braid invariant
- D70's music-space orthogonality (eigenvalue and eigenvector independent)
- Braid invariant theory (Kauffman bracket, Jones polynomial)

Result: D74 proves incomparability at the catalog level: the bracket (eigenvalue /
trace operation) and the Voronoi cell (eigenvector direction) discard orthogonal
information. The combined (bracket, cell) invariant gives 119 classes vs 100
bracket-alone — 19% finer. D75 proves subsumption at the sum level: eigenvalue angle
perfectly predicts parity for all 36 winning triples, and the cell is a coarsening of
it. The two results form a layered picture: individual catalog entries are incomparable
by bracket and cell; additive sums of those entries are fully ordered (angle subsumes
cell). The additive operation creates a hierarchy not present in the individual entries.

### D77: Product Activation as Quaternionic MVN

Threads converging:
- D74's incomparability theorem (bracket and cell carry independent information)
- D50's k-sector resolution (parity needed k=6 to access angular information)
- D75's binocular channel (angle and cell are orthogonal, each carrying ~1 bit)
- Aizenberg MVN theory (k-sector reads S¹ argument)

Result: The S¹×S² product activation is the quaternionic generalization of Aizenberg's
MVN: instead of reading the complex argument (S¹), read both the eigenvalue half-angle
(S¹) and eigenvector direction (S²). The incomparability result from D74 is not merely
structural — it is computationally load-bearing. Neither channel alone can compute XOR8;
their product (112 cells) can (6 solutions). D74's proof that the two invariants are
incomparable is the exact reason their combination is necessary.

### D79-D80: Finite Group Ceiling Explained

Threads converging:
- D78's exhaustive XOR10=0 confirmation at ζ₈
- D66's identification of the 24-quaternion binary octahedral group
- ADE classification of finite SU(2) subgroups (from mathematical literature)
- Kuperberg #P-hardness and lattice root tractability

Result: The ζ₈ XOR wall at 8 inputs is explained completely by group finiteness: the
binary octahedral group has 24 elements (ADE type E₇), and 5-element subsets of 24
cannot provide the parity structure needed for XOR10. ζ₁₂ (infinite group) has no such
ceiling. D80 identifies the exact boundary: only ζ₄ (Q₄, 4 elements) and ζ₈ (2O, 24
elements) are finite under the two-generator construction — both are quantum-dimension-
zero points. The tractable-computation regime (lattice roots, [2]_q ∈ {-2, 0}) is
exactly the finite-group regime, and finiteness is what imposes the capacity ceiling.

### D81-D82: Logarithm and Depth Form One Argument

Threads converging:
- D79's confirmed scaling from XOR10 to XOR12 at ζ₁₂
- D80's exponential group closure growth (~2× per round)
- Knot theory crossing number interpretation
- Kuperberg #P-hardness (exponential generation cost, linear algebraic gain)

Result: D81 measures the log scaling empirically (0.62 XOR per log₂ doubling). D82
identifies the mechanism: depth = round number = crossing number is the linear variable;
the log appearance is an artifact of expressing the exponential catalog size as a
logarithm. The two constants are consistent: catalog ≈ 2^depth implies
0.62 × log₂(2^depth) = 0.62 × depth, giving the linear depth law with slope 0.62.
D82 also demonstrates algebraic coherence beats vocabulary: deep entries (same depth,
fewer total) outperform strided entries (full vocabulary, mixed depth) by two XOR levels.
The framework explains Kuperberg's hardness: you need exponentially many crossings to
reach depth d, but each crossing unit buys only a linear increment in XOR capacity.

---

## Summary: The Project's Logic

The project follows a single thread:

**Can topological structure compute?**

D01-D09 build the bracket. D10-D17 extract information from it. D18-D25 prove
classical universality but discover the parity barrier. D26-D29 connect bracket
values to neural network weights and prove forward DKC. D45-D50 map the complete
landscape and resolve the parity wall via activation geometry. D51-D52 ground the
algebraic explanation. D53-D59 verify universality across evaluation levels.
D61-D62 prove the half-plane wall analytically. D63 establishes the parity scaling
ceiling at n=5 inputs. D39 Parts G-J provide three formal proofs anchoring the
algebraic theory.

D64 reveals the parity hierarchy as a matroid minor chain and proves the n=6 wall
via the self-doubling impossibility theorem. D65 tests the Clifford staircase
conjecture: standard Z[zeta_16] leaves the ceiling unchanged, but generalized
binary labelings first break XOR6 at k=24 sectors — with zero margin, exact lattice
alignment. D66 lifts the scalar bracket to full SU(2) quaternions, discovering the
24-cell as the natural braid geometry and resolving the k=24 mystery as a projection
of 24-cell vertices. D67 proves S² is the computational sweet spot: a 13-direction
Voronoi on the rotation-axis sphere achieves more solutions (36) with fewer cells
(14) than the full S³ or the S¹ approaches, while the Hopf fiber phase is
completely inert. D68 proves the computation is intrinsically curved — stereographic
projection preserves solutions only when using the S² great-circle metric, not
Euclidean distance. D69 proves universal direction nesting (ζ_8 ⊂ ζ_16 ⊂ ζ_32
exactly on S²) and explains why ζ_8 is the computational sweet spot: finer roots
trivialize XOR6 through pigeonhole saturation. D70 maps the algebra to music:
4 eigenvalue angles give 4 notes spanning consonance to tritone, Yang-Baxter
equivalent braids produce identical melodies, and music-space orthogonality
(r = -0.06) confirms the Hopf base/fiber split physically. D71 closes with the
13=13 theorem: minimum spherical harmonic bandwidth l=6 because 2×6+1=13, the
eigenvector direction count — a sharp, all-or-nothing phase transition explained
entirely by degrees of freedom.

D72 tests the 13 directions as a spherical design and finds they achieve t=0 (worst
possible), yet geometrically optimized directions produce 9× fewer solutions — confirming
D71's DOF argument from the negative direction and establishing that algebraic placement,
not geometric distribution, governs DKC computation. D73 proves DKC is fundamentally
additive (not multiplicative), establishes 82.8% universal automaton determinism across
all 36 XOR6 winners, and identifies the "compass without odometer" structure: the Voronoi
cell captures eigenvector direction but discards magnitude, the precise source of the 17%
non-determinism. D74 proves the Kauffman bracket and Voronoi cell are incomparable braid
invariants, with the combined (bracket, cell) pair giving 119 equivalence classes vs 100
bracket-alone, and identifies 6 distinct computational orbit types in the 14-cell partition.
D75 establishes the binocular channel: for additive sums, the eigenvalue angle subsumes the
cell (angle perfectly predicts parity for all 36 winners), giving 2.3× capacity over
cell-only, while the multiplicative channel (staying inside the 24-cell group) has zero XOR6
solutions.

D76 scales to 8 inputs and confirms 0 solutions under S²-only activation (pigeonhole: 256
masks / 14 cells). D77 resolves this with S¹×S² product activation (112 cells, 6 winners) —
the same "wall was activation not algebra" discovery as D50, one level up. D78 exhaustively
confirms XOR10 = 0 at ζ₈ under all activations, revealing a new type of wall: group
finiteness. The binary octahedral group's 24 elements are simply insufficient for 5-element
XOR10. D79 breaks this wall by switching to ζ₁₂ (infinite group): 124 XOR10 winners,
50+ XOR12. D80 surveys 7 roots of unity and finds only ζ₄ and ζ₈ generate finite SU(2)
subgroups — both corresponding to quantum-dimension-zero points, ADE types D and E₇ —
while all other roots generate infinite groups with no capacity ceiling within the explored
range. D81 measures the ζ₁₂ scaling law: max_xor ≈ 0.62 × log₂(catalog) + 4.6. D82
reinterprets this: depth (closure round = generator multiplication count = knot crossing
number) is the linear variable; catalog grows exponentially per round, making the logarithm
of catalog equal to depth. The linear depth law (max_xor ≈ depth + 6) reveals that
computational power scales linearly with crossing complexity, while the cost of generating
depth-d entries scales exponentially — consistent with Kuperberg's #P-hardness structure.

The answer: yes, with a complete capacity theory. The bracket algebra computes Boolean
functions through a mechanism that is additive (not multiplicative), algebraic (not
geometric), and governed by activation choice (not topology). For ζ₈: 12/13 NPN classes
via forward DKC, parity requires k-sector activation, maximum XOR inputs is 8 (hard
ceiling from binary octahedral group finiteness). For infinite-group roots (ζ₁₂ and
beyond): XOR capacity scales logarithmically with catalog size (equivalently: linearly
with knot crossing depth), with no observed ceiling. The 13-direction structure on S² is
the computational sweet spot for ζ₈: algebraically forced, geometrically worst, and
exactly explained by the 13=13 spherical harmonic DOF theorem.
