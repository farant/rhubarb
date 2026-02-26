# Cross-Demo Connections

How 109 demos feed into each other, what recurs, and where threads unexpectedly converge.

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

### Scaling and Finiteness Arc (D76-D84)
The wall-was-activation pattern recurs, group finiteness governs capacity, and framing
and null states reveal two additional independent computational resources:

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
 |        \
 v         v
D83        D84
(framing:  (null states:
Jones→     bracket-null
bracket    entries maintain
-2 XOR,    XOR8 via S²
writhe as  directional
+2 axis)   vocab)
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

D83 branches from D82 by asking whether the bracket-to-Jones normalization step costs
computational capacity: yes, exactly 2 XOR levels at every root tested (ζ₁₂: bracket
XOR12 → Jones XOR10; ζ₈: bracket XOR8 → Jones XOR6). The "N-2 pattern" first observed
in D79 is resolved — it was Jones capacity all along, not a bracket truncation artifact.
Writhe (braid chirality, independent of depth at r = 0.14) is a third independent
computational axis, contributing +2 XOR levels identical in magnitude to one depth unit.
The TQFT framing anomaly that physicists normalize away is precisely the term DKC uses
for computation.

D84 branches from D82 on the same ζ₈ catalog to examine why bracket-null entries (Re(q)
= 0, trace vanishes) do not impair XOR8 capacity. They do not impair it — they enable it.
The 9 null entries cover 6 S² directions (cube-edge midpoints) absent from all 15 non-null
entries; removing nulls drops capacity from XOR8 to XOR6, below even the random-removal
baseline of 7.8. Depth and nullness are correlated (depth-3 stratum = 100% null) but the
mechanism is directional: null entries are maximally directionally efficient (1.00 dir/entry
vs. 0.47 for non-null). Cross-root comparison shows null fraction decreases sharply as the
group grows (ζ₄=75%, ζ₈=37.5%, ζ₁₂=3%), consistent with nulls being a finite-group
boundary effect that LCFT Jordan-cell structure illuminates.

### Non-Semisimplicity and Depth Law Arc (D85-D92)
Two interleaved chains investigating the algebraic anatomy (D85-D87) and the
depth law mechanism (D89-D92), with D88 bridging the two:

```
D85 (b = -5/8 indecomposability parameter, TL_4 only)
 |
 v
D86 (direct b on single P_{0,0} -- NEGATIVE RESULT, universal divergence)

D84 (null states, directional vocabulary)
 |
 v
D87 (null indispensability at ζ₁₂ -- regime transition, dispensable at infinite groups)
 |
 v
D88 (anti-correlation mechanism -- rigid anchors vs flexible scaffolding)

D82 (crossing depth law)
 |
 v
D89 (depth law mechanism -- 8 hypotheses, 5 killed)
 |
 v
D90 (sum-angle structure -- axis cancellation, spectral inversion)
 |
 v
D91 (activation bottleneck -- parity is the wall, balanced exponentials confirmed)
 |
 v
D92 (function scaling -- parity-lock theorem, encoding determines function)
 |
 v
D93 (1wpi function scaling -- complement-blindness, circuit complexity hierarchy)
 |
 v
D94 (binary icosahedral group -- solvability bottleneck, five-pillar synthesis)
```

D85→D86 form a two-demo chain on the indecomposability parameter b: D85 computes
b = -5/8 (Pearce-Rasmussen) at TL_4 via leading-coefficient extraction from the
delta-parameterized Markov trace, resolving the b = -2 vs b = -5/8 "controversy" as
a normalization difference. D86 attempts to isolate the computation on a single
projective cover P_{0,0} — and fails universally. The multiplicity from the regular
representation is structurally essential, not removable. A novel negative result.

D84→D87→D88 form a null-state investigation chain. D84 established null indispensability
at ζ₈; D87 tests this at ζ₁₂ and discovers a regime transition — nulls are directionally
indispensable (67 exclusive S² axes) but computationally dispensable (XOR capacity fully
preserved without them). D88 explains why D72's Voronoi optimization catastrophically
destroyed computation: the 4 non-null body-diagonal directions are rigid computational
anchors while the 6 null-only edge-midpoint directions are flexible scaffolding; the
k-ladder activation provides 14× better perturbation resilience than Voronoi-only.

D82→D89→D90→D91→D92 form the depth law mechanism arc. D89 systematically tests 8
mechanistic hypotheses and kills 5 (pairwise coherence, direction coverage, Cayley
density, sector diversity, angle coherence), leaving "algebraic coherence" as the
remaining candidate. D90 cracks it: the mechanism is axis cancellation (anti-aligned
rotation axes producing low sum angles) combined with cross-depth algebraic constraints
from BFS ancestry, and the advantage is relational not positional — the S² point cloud
bandwidth actually DECREASES with depth (spectral inversion). D91 confirms the balanced
exponentials explanation from the complementary direction: the activation is not the
bottleneck. D92 closes the arc by showing that the +/-q encoding is structurally
parity-locked — only XOR/XNOR are computable, AND/OR/MAJ/THRESHOLD = 0 winners at all
depths. The depth law is parity-specific.

### Matrix Braid DKC Arc (D98-D109)
Higher-strand scaling, BFS invariants, activation anatomy, and graph-theoretic
parity prediction:

```
D98 (3-strand trace readout bottleneck)
 |
 v
D99 (Delta_1 module, XOR14, non-split extension)
 |
 v
D100 (W_{4,2}, 4-strand, non-semisimple) ----> D101 (W_{5,3}, 5-strand, simple)
 |                                                |
 v                                                v
D102 (6-strand A/B: W_{6,0} vs W_{6,4}, Barrington-Radical Principle)
 |
 v
D103 (W_{6,0} vs W_{6,2}: BFS growth is braid group invariant)
 |
 v
D104 (activation coarseness sweep, branching decomposition, sign-rank expansion)
 |
 v
D105 (8-strand W_{8,0}/W_{8,2}: k-regime refutes D93, macrame principle)
 |
 v
D106 (topological entropy null -- capacity is relational, not dynamical)
 |
 v
D107 (algebraic graph density, Z/4Z axis-alignment theorem, nesting parity)
 |
 v
D108 (parity vocabulary graph, dual-channel theorem)
 |
 v
D109 (D55 graph analysis, encoding-dependent dual-channel)
```

D102 is the pivot: the Barrington-Radical Principle (radical carries an abelian writhe
character, provably useless for parity by Barrington's theorem) decisively refutes
radical-as-resource and establishes that simple modules outperform non-simple at matched
dimension. D103 settles the growth law: BFS tree is a braid group invariant, identical
across all simple modules of the same strand count regardless of dimension (confirmed
for all three n=6 modules). D104 anatomizes the activation as a compressed sensing
readout: sign-rank expansion (sign quantization breaks linear dependencies, expanding
effective dimension), the Atkinson sweet spot (optimal component count for hash collision
tradeoff), and branching decomposition (cross-block interaction IS the computational
signal at n=6). D105 extends to n=8, refuting D93's "XOR dies at N>=7" as a k-regime
artifact and discovering the macrame principle (branching interaction reverses from
cross-block to within-block as sub-module complexity increases).

D106 opens a new thread: topological entropy (Thurston-Nielsen classification) is
completely orthogonal to DKC Boolean computation, falsifying the prediction that
pseudo-Anosov braids compute better. The TL visibility filter explains why: writhe
survives the TL quotient (correlates with DKC), entropy does not (erased by e_i^2=0).

D107-D109 form a trilogy: D107 proves universal axis-alignment of braid matrices at
delta=0 via a constructive Z/4Z phase formula, discovers nesting parity as a novel Z/2Z
grading, and introduces the major index bipartite invariant. D108 shows graph structure
predicts parity capability (dual-channel theorem: parity requires both multiplicative
phase coherence and additive magnitude diversity). D109 tests this at delta=sqrt(2) and
discovers the encoding-dependent dual-channel theorem: under multiplicative encoding,
parity wants LOW product closure (inverted from D108's additive encoding result).

---

## 2. Code Reuse Patterns

### The "Standard Library" That Emerged

Four code assets appear in 30+ demos and constitute a de facto standard library:

| Asset | Introduced | Used in | Role |
|-------|-----------|---------|------|
| Complex arithmetic (`Cx`) | D10 | 35 demos | Numerical workhorse |
| Union-find loop counter | D02 | 40+ demos | State-sum backbone |
| NPN classification | D23 | 15 demos | Boolean function taxonomy |
| Braid word decoder | D22 | 15 demos | Exhaustive enumeration |

Two newer assets are emerging as cross-demo standards:

| Asset | Introduced | Used in | Role |
|-------|-----------|---------|------|
| Raqiya (Z[zeta_8] analysis) | D107 | D107, D108, D109 | Algebraic graph analysis |
| MatN (variable-dim matrix) | D103 | D103, D104, D105 | Variable-dimension TL representations |

### Ring Tower

A progression of cyclotomic rings, each built for a specific evaluation level:

```
Z (integers) -- D01-D09
  |
  Cx (floats) -- D10-D28
  |
  Z[zeta_8] -- D29, D35, D48-D50, D100-D108
  |
  Z[omega] -- D53
  |
  Z[zeta_16] -- D54-D57, D65, D69, D109
  |
  Z[zeta_5] -- D58
  |
  Z[zeta_24] -- D59
  |
  Z[zeta_32] -- D69 (braid group infinite; enumeration cap-limited)
  |
  H (quaternions, SU(2)) -- D66, D67, D68, D69, D70, D71, D72-D84
  |
  ζ₁₂ group (infinite SU(2) subgroup) -- D79, D81, D82, D83
```

D72-D84 all work in the H / SU(2) quaternion setting. D76-D79 additionally introduce
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

4. **D93→D105**: D93's "XOR dies at N>=7" appeared to be a fundamental DKC capacity limit.
   Wall was in the **k-regime** (k=128 at N=8 gives k/2^N=0.5, deep pigeonhole), not
   strand count or algebra. Resolution: k=4096 at n=8 gives XOR8=22266, beating n=6.

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

D83 extends this picture: writhe (net signed crossing count, independent of depth at
r = 0.14) is a second crossing-level resource. Jones normalization removes the writhe-
dependent phase factor (-A³)^{-w} — the TQFT framing anomaly — and costs exactly 2 XOR
levels at every root tested. Framing = +2 XOR, depth unit = +2 XOR: the two resources
are equal in magnitude, independent, and additive. The resource decomposition conjecture
(supported) is: DKC capacity ≈ lattice base (XOR6) + 2 × depth_rounds + 2 ×
framing_present.

### Null States as Directional Vocabulary

At ζ₈, 9 of 24 catalog entries are bracket-null (Re(q) = 0, Kauffman trace vanishes).
These are not computational dead weight. D84 demonstrates that removing them drops XOR
capacity from XOR8 to XOR6 — below the random-removal baseline of 7.8. The mechanism is
purely directional: null entries cover 6 S² directions (cube-edge midpoints) absent from
all non-null entries, with 1.00 unique direction per null entry vs. 0.47 for non-null.
All null entries sit at half-angle 90°, a single point on S¹ — their entire computational
contribution comes from S² directional diversity, not angular variation.

The depth-null correlation is striking: depth-3 stratum (the maximally deep entries) is
100% null. D82 and D84 together imply that the deepest crossings — the ones contributing
the most to capacity per depth-law — are simultaneously the algebraically invisible ones
(zero trace). The LCFT interpretation (Gurarie 1993 / Gaberdiel-Kausch 1996) is that null
entries are the Jordan-cell partners of "logarithmic" non-null entries sharing the same S²
axis; removing the null state removes the Jordan-cell anchor, collapsing the coupled pair.

The null fraction dilutes sharply with group size (ζ₄=75%, ζ₈=37.5%, ζ₁₂=3%): nulls are
a finite-group boundary phenomenon concentrated exactly where the TL algebra is maximally
non-semisimple (quantum-dimension-zero points). The ζ₈ regime is where nulls represent the
most structurally significant fraction.

### Null Indispensability as Regime-Dependent Phenomenon

D84 established null indispensability at ζ₈. D87 reveals this is a finite-group phenomenon:
at ζ₁₂ (infinite group, 4096 entries), removing all 121 bracket-null entries preserves full
XOR capacity (XOR6/8/10/12 all 32 winners). The transition mechanism is direction density —
ζ₈ has 13 total directions with 6 null-only (sparse, loss is catastrophic), ζ₁₂ has 2043
directions with 67 null-only (dense, non-null coverage absorbs the loss). D88 provides the
geometric explanation: per-direction spherical design residual at ζ₁₂ is 8× lower than ζ₈.

This refines the LCFT interpretation from D84: null entries as Jordan-cell anchors are
structurally critical only in the sparse-direction finite-group regime. In the dense-direction
infinite-group regime, the non-null states alone provide sufficient manifold dimensionality.
The RC (Reservoir Computing) null-state hypothesis similarly applies only in sparse regimes.

### Rigid Anchors vs Flexible Scaffolding

D88 anatomizes the 13 ζ₈ directions into two computational roles:
- **4 non-null body-diagonal directions** (70.5° mutual angle): rigid computational anchors.
  Perturbation causes 8% XOR loss at 10°. Load-bearing for parity satisfaction.
- **6 null-only edge-midpoint directions** (60°/90° mutual angles): flexible topological
  scaffolding. Perturbation causes -2.4% XOR loss (a slight GAIN). Movable without damage.

This explains D72's catastrophic Voronoi destruction (36→4 XOR6, 89% loss): D72 used
Voronoi-only activation, which has knife-edge sensitivity to direction perturbation. D88's
k-ladder activation (8 k_sec values) provides 14× better resilience (6.4% loss vs 89%)
because small direction shifts are absorbed by switching angular resolution. Constrained
optimization (nulls free, non-nulls clamped to 2°) achieves 48% design improvement with
only 2.8% XOR loss.

### Axis Cancellation and Relational Computation

D90 identifies the concrete mechanism behind "algebraic coherence" (the unexplained term
from D82): XOR winners preferentially contain anti-aligned quaternion pairs (axis dot
product ≈ -0.75 vs -0.65 for random). Nearly-opposite rotation axes partially cancel,
yielding sum quaternions with low rotation angles (30-66°) that create the fine-grained
sector separations needed for parity.

The mechanism is relational, not positional. D90's spectral inversion proves this: the S²
point cloud bandwidth DECREASES with depth (BW_90% drops from l=4 to l=2, power
concentrates at l=0 = uniform), yet computation improves linearly. Individual entry positions
become LESS informative as the catalog grows; what matters is how entries combine. Cross-depth
algebraic constraints (BFS ancestry restricts gen × deep sums to 73 distinct angles from
50K pairs) provide the relational structure.

### Balanced Exponentials and the Parity Wall

D90 proposes and D91 confirms the balanced exponentials explanation for the linear depth law:
BFS closure adds vocabulary at ~2×/round; parity constraints demand 4×/XOR weight level.
The ratio 2:4 = 1:2 predicts ~2 depth rounds per additional weight, giving max_xor ≈
depth + 6. D91 proves this from the complementary direction: the activation is not the
bottleneck. Sweeping k_sec from 2 to 48 and Voronoi directions from 2 to 114 changes
XOR8 winner count from hundreds to thousands but never changes max_xor. Finer resolution
reveals more solutions to the same parity constraint; it does not enable higher parity.

D92 closes the arc: the +/-q encoding is structurally parity-locked. The (0,0)≡(1,1)
collision (masks 0...0 and 1...1 always produce identical sums = zero) means any function
where f(0...0) ≠ f(1...1) is impossible — ruling out AND, OR, MAJ, THRESHOLD under any
activation whatsoever. The depth law describes the capacity curve for the encoding's ONLY
achievable function family: parity.

### Encoding Determines Function

D48/D50's 1-weight-per-input (1wpi) encoding with split-sigmoid/MVN activation computes
all 13 NPN classes including parity. D92's +/-q paired encoding is parity-locked. The
contrast is encoding-specific, not a fundamental DKC limit. The 1wpi encoding assigns one
catalog entry per input bit, giving each weight independent combinatorial freedom. The +/-q
encoding constrains each weight to contribute ±q or 0, creating per-weight pair
cancellations (sign-flip symmetry: combined_cell(S) = combined_cell(-S)) that funnel all
computational power into parity — the single Boolean function outside AC⁰.

### BFS Growth as Braid Group Invariant

D102-D103 establish that BFS catalog growth is a property of the braid group B_n, not of
the TL module chosen. All three tested n=6 modules (W_{6,0} dim=5, W_{6,2} dim=9,
W_{6,4} dim=5 non-simple) produce BIT-FOR-BIT identical BFS depth profiles: 1, 11, 77,
439, 2233, 10603, 32768. Max coefficient magnitudes are also identical across simple
modules. Growth rate converges toward (n-1)x per round, confirming the sl_d functor thesis
from D101:

| n (strands) | Demo | Generators | Growth rate | Predicted |
|-------------|------|------------|-------------|-----------|
| 3 | D98 | 4 | ~2.2x | 2 (sl_2) |
| 4 | D100 | 6 | ~3.1x | 3 (sl_3) |
| 5 | D101 | 8 | ~4.0x | 4 (sl_4) |
| 6 | D102-103 | 10 | ~5.0x | 5 (sl_5) |
| 8 | D105 | 14 | ~5.1x | 7 (sl_7) |

The implication is profound: the "reservoir" (BFS catalog) is invariant across modules.
Only the "readout" (activation function = sign hash of matrix entries) changes. This
cleanly separates reservoir dynamics from readout design, exactly as the reservoir
computing framework predicts.

### Sign-Rank Expansion and Compressed Sensing

D104 discovers that sign quantization (ternary {-1,0,+1} projection) can INCREASE
effective dimension: W_{6,2} has raw rank 244/324 (75.3%) but sign-rank 292/324 (90.1%)
— a gain of 48 independent dimensions. The nonlinear sign() function breaks linear
dependencies that exist over Z. D105 extends this: W_{8,2} at 16384 entries shows
sign-rank 2003 vs raw 1096, expansion factor 1.83x and still growing.

This connects to 1-bit compressed sensing (Boufounos-Baraniuk 2008): sign-hash activation
is a 1-bit quantization of algebraic integer components. The cross-block activation works
because branching cross-terms have low intra-block coherence (near-orthogonal TL_5
pieces), satisfying the incoherence condition for CS recovery. The sign-rank table across
all modules:

| Module | dim | Raw % | Sign % | Expansion |
|--------|-----|-------|--------|-----------|
| Delta_1 | 2 | 100% | 100% | 1.00x |
| W_{4,2} | 3 | 77.8% | 88.9% | 1.14x |
| W_{5,3} | 4 | 96.9% | 100% | 1.03x |
| W_{6,0} | 5 | 100% | 100% | 1.00x |
| W_{6,2} | 9 | 75.3% | 90.1% | 1.20x |
| W_{8,0} | 14 | 90.6% | 97.8% | 1.08x |
| W_{8,2} | 28 | 34.9% | 63.9% | 1.83x |

The pattern: expansion is greatest for high-dimensional modules with redundancy (many
Z-linear dependencies to break), and smallest for low-dimensional modules that are already
maximally independent.

### Relational vs Individual: Computation is Collective

D106 crystallizes a theme running through the project: DKC capacity is determined by
relational/algebraic properties of the catalog, never by individual/dynamical properties
of single entries. The evidence is cumulative:

- **D106**: topological entropy (an individual dynamical property) is completely orthogonal
  to DKC Boolean computation — both periodic and pseudo-Anosov braids compute XOR at 100%
  participation. The TL visibility filter erases all dynamical information.
- **D90**: spectral inversion — S² bandwidth DECREASES with depth while computation
  IMPROVES. Individual entry positions become less informative; relational structure
  (cross-depth composition) is what matters.
- **D95**: COMM-only and NON-COMM-only each perform similarly in isolation; the synergy
  emerges only from their combination.
- **D107-D108**: graph-theoretic parity prediction works precisely because it measures
  relational properties (product closure, additive v_2 connectivity) rather than single-
  value properties.

The TL visibility filter from D106 provides the mechanism: properties that survive the
TL quotient at delta=0 (writhe, algebraic phase) correlate with DKC output; properties
that do not survive (entropy, spectral radius) are invisible. The quotient is the filter
that separates relational from individual structure.

### Graph-Theoretic Parity Prediction (The D107-D109 Trilogy)

D107-D109 form a trilogy establishing that algebraic graph structure predicts parity
capability:

**D107** (Algebraic Graph Density) builds the analytical foundation: every entry of a
braid representation matrix at delta=0 is axis-aligned (exactly one nonzero Z[zeta_8]
component), proved via a constructive Z/4Z phase formula. The nesting parity Z/2Z
grading, major index bipartite invariant, and q-Catalan identity are novel mathematical
results. DKC at delta=0 reduces to integer path counting plus a three-variable phase
formula: effective computation is Z × Z/4Z per matrix entry.

**D108** (Parity Vocabulary) shows graph structure PREDICTS parity capability. Maximum
structural contrast: parity has 17 product closure edges (connected P_3), poison has
ZERO (empty graph). The dual-channel theorem: parity requires BOTH multiplicative phase
coherence (product closure > 0) AND additive magnitude diversity (v_2 connectivity >
trivial). Maps onto T-gate/Hadamard decomposition in quantum circuit synthesis.

**D109** (Encoding Dependence) tests the dual-channel theorem at delta=sqrt(2) and
discovers the encoding-dependent refinement: at delta=sqrt(2) with multiplicative encoding,
parity and non-parity are structurally indistinguishable on 7/8 edge types. Product closure
is the sole discriminator — but its polarity is INVERTED (parity wants LOW product closure,
opposite of D108). The sign flips with encoding type. When algebra is rich (j=0 sector
alive, D52), the simplest activation (Re>0) suffices because no structural deficits need
compensating.

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

### D79's "N-2 Pattern" Resolved by D83 (D79↔D83)

D79 first observed that ζ₁₂ capacity seemed to cap at XOR10 while the bracket catalog
appeared to reach XOR12, a puzzling 2-level discrepancy. D83 resolves it: D79 was
inadvertently working with Jones-normalized quantities (the bracket evaluation used in
D79's capacity measurement implicitly applies the writhe phase factor). The bracket
genuinely reaches XOR12; the Jones normalization costs exactly 2 levels. The N-2 pattern
was Jones capacity all along, not a truncation artifact or a property of ζ₁₂ itself. D83
is the retroactive explanation for a mystery that persisted from D79 through D82.

### Depth Law and Writhe Are Independent Axes (D82↔D83)

D82 established max_xor ≈ depth + 6: each closure round contributes +2 XOR levels. D83
establishes that writhe (net signed crossing count) contributes an additional +2 XOR levels
independently of depth. Depth-writhe correlation r = 0.14 across 4096 ζ₁₂ entries confirms
these are genuinely separate quantities — a deep entry can have low writhe (crossings cancel)
or high writhe (crossings accumulate). The "+2 per resource unit" pattern holds for both axes.
The resource decomposition conjecture (D83) gives a unified formula: DKC capacity ≈ XOR6
(lattice base) + 2 × depth_rounds + 2 × framing_present. Reservoir computing theory (Jaeger
2001, Maass 2002) predicts exactly this additivity for independent channels.

### 24-Cell Structure Survives Jones Normalization (D66↔D83)

D66 established that the ζ₈ braid catalog produces exactly 24 distinct unit quaternions —
the binary octahedral group, vertices of the 24-cell. D83 shows that Jones normalization
reduces distinct quaternion count from 4096 to 2710 at ζ₁₂ (-33.8%) and angles from 43 to
31 (-28%), but leaves direction count unchanged (512 → 512). At ζ₈: the same 24 directions
persist through Jones normalization. The 24-cell geometry (and thus the S² directional
structure that the quaternionic arc from D66 to D71 depends on) is framing-invariant.
Normalization collapses angular vocabulary but cannot collapse the directional skeleton.

### D81 Logarithmic Law Was Measuring Jones Capacity, Not True Drivers (D81↔D83)

D81's empirical scaling law (max_xor ≈ 0.62 × log₂(catalog) + 4.6) was fit to data that
implicitly used bracket evaluations without Jones correction. D82 reinterpreted log as a
proxy for depth. D83 adds a third layer: the "+4.6" intercept in D81's formula and the
"+6" in D82's depth law may both absorb the framing correction — the true lattice base
(without any correction) is at XOR6, and the constants in those formulas were fit to
bracket-but-not-Jones-adjusted measurements where writhe was partially mixed in. Together
D81→D82→D83 form a progressive sharpening: logarithm (phenomenology) → depth (mechanism)
→ writhe (the residual that depth alone didn't explain).

### Depth = Null (D82↔D84)

D82 established that depth (closure round) is the fundamental variable governing XOR
capacity: each additional round contributes +2 levels. D84 finds that every maximally-deep
ζ₈ entry is bracket-null (depth-3 stratum = 100% null). The depth hierarchy is also a null
hierarchy — the entries that contribute most capacity per depth unit are simultaneously the
algebraically invisible ones (Re(q)=0, trace=0). This is not a coincidence: deep entries
are built by multiplying many generators; repeated multiplication drives the quaternion's
real part toward zero (the fixed-point attractor of the depth recursion is the equatorial
S² where Re(q)=0). The computational mechanism lives in the Jordan-cell coupling between
null entries and their S²-sharing non-null partners.

### Finite/Infinite Distinction Maps Onto Null Fraction (D80↔D84)

D80 established that only ζ₄ and ζ₈ generate finite SU(2) subgroups, while all other roots
(including ζ₁₂) generate infinite groups. D84 finds that the null fraction maps directly onto
this finiteness boundary: ζ₄=75% null (finite, smallest group), ζ₈=37.5% null (finite, 24
elements), ζ₁₂=3.0% null (infinite, 4096+ elements). As the group grows toward infinity, the
null fraction approaches zero. Nulls are a finite-group boundary effect — in an infinite group
the discrete S¹ spectrum collapses and the half-angle=90° "null point" becomes an infinitesimal
measure. The capacity ceiling imposed by finite group order (D78→D79 resolution) and the
directional indispensability of null entries (D84) are two faces of the same finite-group
constraint.

### Non-Semisimplicity Drives Computation (D50↔D84)

D50 established that XOR parity is reachable at ζ₈ using k-sector activation at the
maximally non-semisimple point (δ=0). D84 reveals the internal anatomy of what makes
this possible: 37.5% of the ζ₈ catalog consists of bracket-null entries (Re(q)=0),
which are the most direct manifestation of non-semisimplicity in the quaternionic setting
(they correspond to the TL radical elements that D51-D52 studied algebraically). The null
entries carry XOR8 capacity via directional coverage; the ζ₄ case (75% null) is entirely
saturated by nulls and achieves zero XOR capacity (all information is directional but
too few entries to fill the parity structure). Non-semisimplicity (δ=0) is thus both the
obstacle (D47 conjecture) and the mechanism (D84): the radical elements that block scalar
computation redirect computation into the geometric channel.

### 13=13 Bandwidth = 6 Null-Only + 4 Non-Null-Only + 3 Shared (D71↔D84)

D71 proved the 13=13 theorem: minimum spherical harmonic bandwidth l=6 because 2×6+1=13
equals the eigenvector direction count. D84 now anatomizes those 13 directions by null/non-
null participation: 6 null-only (cube-edge midpoints, 1/√2 components), 4 non-null-only
(tetrahedral axes, 1/√3 components), 3 shared (coordinate axes). The 13 directions do not
form a uniform algebraic class — they are a superposition of three geometrically distinct
families. The l=6 bandwidth theorem counts all 13 as equal; D84 reveals that 6 of them are
accessible only through the null entries. If nulls were removed, only 7 directions remain,
requiring l≥3 bandwidth — which still technically suffices but the Voronoi partition would
be coarser, losing the XOR8 solutions that depend on fine-grained null-direction coverage.

### Three-Level Discarding: Jones, Null, and LCFT

The project has now documented three distinct "discarding" operations that TQFT/CFT perform
on topological data, each of which turns out to carry computational content:

1. **Jones normalization (Jones 1985, D83)**: removes the writhe-dependent phase factor
   (-A³)^{-w} as a "framing anomaly." D83 proves this factor = +2 XOR levels of capacity.
2. **Bracket-null entries (D84)**: entries with Re(q)=0 are invisible to the scalar
   Kauffman trace (the Jones polynomial's building block). D84 proves they maintain the
   S² directional vocabulary essential for XOR8.
3. **LCFT null states (D84 interpretation)**: in logarithmic CFT, null states ⟨N|N⟩=0 are
   paired with logarithmic partners via Jordan cells. The null entry (DKC analog: Re(q)=0)
   is computationally invisible in the scalar channel but anchors the Jordan-cell coupling
   that carries the logarithmic partner's content.

All three operations are motivated in physics by a desire to obtain a clean topological
invariant. All three discard exactly the structures that DKC uses for computation. The
"maximally degenerate point" (δ=0) that enables tractable DKC is the same point where all
three discarding mechanisms are most aggressive.

### RC Pillar 5 Connects Four Demos (D73↔D82↔D83↔D84)

The Reservoir Computing interpretation (Pillar 5 in the project's framework) receives its
strongest support from four demos together:

- **D73**: DKC is additive (not multiplicative), with 82.8% universal automaton determinism
  — exactly the "fading memory" property of a good reservoir.
- **D82**: computational capacity scales linearly with crossing depth — the reservoir's
  "depth" (processing time) determines its computational power, as in temporal RC.
- **D83**: writhe and depth are independent axes each contributing +2 XOR, like independent
  channels in a multi-dimensional reservoir.
- **D84**: null entries maintain the reservoir's separation property in directions that
  active states do not cover — exactly the RC requirement that null reservoir states hold
  the manifold open for linear readout.

Together these four results characterize the ζ₈/ζ₁₂ braid closure process as a concrete,
exactly-analyzable instance of reservoir computing, where the reservoir geometry is
algebraically forced (not learned) and the capacity theory is completely explicit.

### D85→D86: b Computation to Structural Failure (D85↔D86)

D85 computes the indecomposability parameter b = -5/8 (Pearce-Rasmussen) at TL_4 via
leading-coefficient extraction from the delta-parameterized Markov trace on the full
14-dimensional regular representation. The "controversy" between b = -2 (GRS) and b = -5/8
(PR) dissolves: b_delta = -5/2 on the regular rep divides by dim(0-TL sector) = 4 to give
b_PR = -5/8. D86 attempts to isolate this on a single projective cover P_{0,0} and finds
universal divergence — the valuation condition p_tt = 2*p_Tt never holds on a single copy.
The multiplicity from the regular representation is structurally essential, not removable.
The TL_6 regular rep also diverges (gap = -2), and TL_8 P_{0,0} is completely semisimple.
The b = -5/8 success at TL_4 may be a "smallest nontrivial case" coincidence where cross-copy
terms accidentally provide the right extra delta-power.

### D85↔D87: Non-Semisimplicity Bridges TL Algebra and LCFT (D85↔D87)

D85's indecomposability parameter b quantifies the non-semisimple structure of TL_n at
delta=0: Jordan blocks exist only on projective covers, never on standard modules (verified
at n=4,6,8,10,12). D87's null indispensability regime transition is the computational
manifestation of the same non-semisimplicity viewed from the LCFT side: bracket-null entries
(Re(q)=0) are the Jordan-cell partners whose indispensability depends on direction density.
Both demos explore different faces of the same object — TL non-semisimplicity at the
quantum-dimension-zero point — one algebraically (diagram composition, Gram matrices, Jordan
analysis) and one computationally (XOR capacity, direction partition, group regime).

### D72↔D88: Voronoi Catastrophe Explained (D72↔D88)

D72 found that optimizing 13 eigenvector directions for spherical design quality destroys
computation (36→4 XOR6, 89% loss) but offered no mechanistic explanation beyond "algebra
beats geometry." D88 provides the anatomy: the 4 non-null body-diagonal directions are rigid
computational anchors (8% XOR loss at 10° perturbation) while the 6 null-only edge-midpoint
directions are flexible scaffolding (-2.4% loss = slight gain). D72's Voronoi-only activation
has knife-edge sensitivity because each XOR triple gets ONE chance at a specific angular
resolution; D88's k-ladder activation gives 8 chances at different resolutions, providing
14× better resilience. The catastrophe was an activation fragility, not a geometric necessity.

### D48/D50↔D92: 1wpi vs ±q Encoding Contrast (D48↔D50↔D92)

D48 proved zero parity solutions out of 100M quartets using split-sigmoid activation with
1-weight-per-input (1wpi) encoding. D50 resolved this by changing the activation to k-sector,
finding 906 parity solutions — demonstrating all 13 NPN classes are reachable under 1wpi.
D92 shows the +/-q paired encoding is structurally parity-locked: ONLY XOR/XNOR are
computable, with AND/OR/MAJ/THRESHOLD at zero winners across all depths. The 1wpi encoding
gives each weight independent combinatorial freedom (different catalog entries per input bit);
the +/-q encoding constrains each weight to ±q or 0, creating the (0,0)≡(1,1) collision that
blocks all non-parity functions. The depth law (max_xor ≈ depth + 6) describes capacity for
the +/-q encoding's only function; the 1wpi encoding accesses a fundamentally richer function
space but has its own activation-dependent constraints.

### D89→D90→D91→D92: The Depth Law Arc Closure (D89↔D90↔D91↔D92)

Four demos form a single progressive argument resolving D82's depth law:
- **D89**: tests 8 mechanistic hypotheses, kills 5 (coherence, coverage, Cayley, sector,
  angle), identifies paired extension as real but minority (6-20% at ζ₁₂), leaves "algebraic
  coherence" as the remaining open question.
- **D90**: cracks it — axis cancellation (anti-aligned axes → low sum angles) plus cross-depth
  algebraic constraints (BFS ancestry → 73 distinct angles from 50K gen×deep pairs). Spectral
  inversion (bandwidth DECREASES with depth) proves the mechanism is relational not positional.
- **D91**: confirms from the complementary direction — the activation is not the bottleneck.
  max_xor is k_sec-invariant. Balanced exponentials: vocabulary ~2×/round, demand 4×/weight.
- **D92**: identifies the wall as parity itself. The +/-q encoding is parity-locked; the depth
  law is function-specific, not universal.

The four demos complete a diagnostic sequence: what is the mechanism (D90), what is NOT the
bottleneck (D91), and what IS the wall (D92). The depth law is now fully characterized:
linear because of balanced exponentials, relational because of axis cancellation, and
parity-specific because of encoding structure.

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

### D83: The N-2 Pattern, Writhe, and Framing as Resource

Threads converging:
- D79's unexplained N-2 discrepancy (XOR12 apparent bracket ceiling, XOR10 measured)
- D82's "+2 per depth unit" pattern and algebraic coherence principle
- D66's 24-cell geometry (Jones normalization acts on the same 24-entry ζ₈ catalog)
- D81's logarithmic law (whose intercept may absorb mixed framing correction)
- Kirby (1978) and Witten (1989) framing anomaly in TQFT
- Reservoir computing independent-channel additivity (Jaeger 2001, Maass 2002)

Result: Jones normalization costs exactly 2 XOR levels at both ζ₈ and ζ₁₂. The N-2
pattern is resolved: D79 was computing Jones-normalized capacity, not the true bracket
ceiling. Writhe is identified as a third independent computational axis (r = 0.14 with
depth). The resource decomposition conjecture (supported): DKC capacity ≈ XOR6 (lattice
base) + 2 × depth_rounds + 2 × framing_present. The framing anomaly that TQFT removes
first is exactly what DKC computes with. The direction count (512 at ζ₁₂) is framing-
invariant; only the angular vocabulary (43 → 31 angles) is destroyed by normalization.

### D84: Null States, Directional Vocabulary, and LCFT

Threads converging:
- D82's depth law (depth-3 stratum = 100% null: deepest entries are null)
- D80's finite/infinite classification (null fraction maps onto finiteness: ζ₄=75%, ζ₈=37.5%, ζ₁₂=3%)
- D71's 13=13 theorem (the 13 directions decompose as 6 null-only + 4 non-null-only + 3 shared)
- D51-D52 radical anatomy (bracket-null entries are the quaternionic analog of TL radical elements)
- D50's non-semisimplicity as resource (δ=0 enables XOR parity; D84 shows null entries are the mechanism)
- Gurarie (1993) / Gaberdiel-Kausch (1996) LCFT Jordan-cell structure
- Reservoir computing separation property (null states maintain manifold coverage)

Result: In the ζ₈ catalog, the 9 bracket-null entries (Re(q)=0) are indispensable. They
cover 6 S² directions (cube-edge midpoints) absent from all 15 non-null entries; removing
them drops XOR8 → XOR6, worse than random removal (baseline 7.8). The null entries are
directionally efficient (1.00 dir/entry vs. 0.47 for non-null) and occupy a single S¹ point
(half-angle 90°), deriving all capacity from S² diversity. At XOR8 they participate at 0.79×
expected rate (disproportionately required). The LCFT interpretation is consistent: null
entries are Jordan-cell partners; removing them collapses the partner's anchor and the
coupled computational content. Cross-root comparison confirms this is a finite-group effect
that dilutes toward zero as the group grows.

### D85: Indecomposability Parameter b

Threads converging:
- TL algebra infrastructure from D35 (integer TL generators at delta=0)
- Radical anatomy from D51-D52 (TL non-semisimplicity, Gram matrices)
- Projective cover construction (left ideal closure from regular representation)
- Delta-parameterized trace methods (perturbation analysis at delta→0)

Result: b = -5/8 confirmed at TL_4 via leading-coefficient extraction. The b = -2 (GRS) vs
b = -5/8 (PR) "controversy" resolved as normalization: b_delta = -5/2 on the regular rep
divided by dim(0-TL sector) = 4. Standard modules have no Jordan blocks at any tested size
(n=4,6,8,10,12). Jordan blocks live exclusively on projective covers. The fixed-point form
is too degenerate (affine 3D gauge freedom at TL_4). The delta-parameterized Markov trace
breaks the degeneracy. D86 then proves the single-P_{0,0} extraction universally diverges.

### D87-D88: Null State Regime Transition and Anti-Correlation Anatomy

Threads converging:
- D84 null indispensability (ζ₈: 6 null-only directions, removing nulls drops XOR8→XOR6)
- D72 Voronoi destruction (36→4 XOR6 under design-improving optimization)
- D80 finite/infinite group classification (ζ₈ finite, ζ₁₂ infinite)
- D79 ζ₁₂ capacity (XOR12 at infinite group)
- Cuboctahedral geometry (octahedral symmetry orbits on S²)
- Spherical design theory (Delsarte-Goethals-Seidel, t-design residuals)

Result: D87 discovers the regime transition — null indispensability is finite-group-specific.
At ζ₁₂ (2043 directions, 67 null-only), removing nulls preserves full capacity. D88
anatomizes the mechanism: 4 non-null body-diagonal directions are rigid computational anchors,
6 null-only edge-midpoint directions are flexible scaffolding. Constrained optimization
(nulls free, non-nulls clamped to 2°) gets 48% design improvement with 2.8% XOR loss. The
k-ladder activation provides 14× better resilience than D72's Voronoi-only (6.4% vs 89%).

### D89-D90-D91: The Depth Law Mechanism

Threads converging:
- D82 crossing depth law (max_xor ≈ depth + 6, algebraic coherence)
- D77 paired extension at ζ₈ (shadow pairing in sparse-direction regime)
- D75 binocular DKC (non-canonical sum angles required for computation)
- D66 24-cell geometry (BFS closure of SU(2) generators)
- Altschuler-Parrilo spherical configuration theory (tested and INVERTED)
- Spherical harmonic decomposition of S² point clouds
- Cayley graph theory (BFS structure implies zero intra-depth edges)
- Reservoir computing separation property

Result: D89 kills 5 of 8 mechanistic hypotheses and finds paired extension is real but
minority (6-20% at ζ₁₂ vs 100% at ζ₈). D90 identifies axis cancellation as the geometric
mechanism: XOR winners contain anti-aligned axis pairs (mean min_dot = -0.75 vs -0.65 for
random), and cross-depth algebraic constraints from BFS ancestry restrict sum-angle
vocabularies. Spectral inversion (S² bandwidth DECREASES with depth) proves the mechanism
is relational not positional. D91 confirms the balanced exponentials explanation: vocabulary
~2×/round, demand 4×/weight → linear slope. The activation is not the bottleneck — max_xor
is invariant under k_sec sweep from 2 to 48.

### D92: Parity-Lock Theorem and Encoding Contrast

Threads converging:
- D89-D91 depth law arc (balanced exponentials, parity as the wall)
- D48/D50 forward DKC NPN class zoo (1wpi encoding achieves all 13 classes)
- D82 +/-q encoding convention (paired input encoding)
- AC⁰ circuit complexity (parity outside AC⁰, Furst-Saxe-Sipser/Hastad)
- Aizenberg MVN k-sector theory (combined_cell decomposition)

Result: The +/-q encoding is structurally parity-locked. The (0,0)≡(1,1) collision (masks
0...0 and 1...1 always produce identical sums) blocks any function where f(0...0) ≠ f(1...1).
AND/OR/MAJ/THRESHOLD all have this property — zero winners at all depths. XOR/XNOR satisfy
the constraint because parity is constant on all 3^k equivalence classes. Sign-flip symmetry
(combined_cell(S) = combined_cell(-S)) is a second lock mechanism. The encoding concentrates
all computational power on parity — the single function outside AC⁰. The 1wpi encoding from
D48/D50, which assigns independent catalog entries per input, escapes this lock entirely.

### D102: The Barrington-Radical Principle

Threads converging:
- D94 Barrington solvability bottleneck (non-solvable 2I beats solvable z8)
- D100 W_{4,2} radical anatomy (constant radical content, writhe character on generators)
- D101 simple module comparison (sl_d functor thesis, BFS growth rate prediction)
- D51-D52 TL non-semisimplicity theory (radical anatomy, sandwich theorem)

Result: Head-to-head A/B test of W_{6,0} (simple, dim=5) vs W_{6,4} (non-simple, dim=5,
radical dim=1) on 6-strand braids at delta=0. The radical carries an abelian writhe character
(B_6 → Z/8Z), proven for ALL 32,768 catalog entries. By Barrington's theorem, abelian groups
cannot compute parity. The simple module wins at every XOR level (2449 vs 2370 at XOR6, 1 vs
0 at XOR10). The Barrington-Radical Principle is the first theoretical result connecting TL
representation structure directly to computational complexity theory — not just measuring
capacity empirically but proving incapacity from first principles.

### D103-D104: BFS Invariance and Activation Anatomy

Threads converging:
- D102 identical BFS trees across W_{6,0} and W_{6,4}
- D101 sl_d functor thesis (growth rate = n-1)
- 1-bit compressed sensing (Boufounos-Baraniuk 2008)
- Reservoir computing readout/reservoir decomposition (Jaeger 2001)

Result: D103 adds W_{6,2} (dim=9) and finds IDENTICAL BFS to W_{6,0} (dim=5): same depth
profile, same max coefficient magnitudes. Growth is a braid group property. But dim-9
has WORSE XOR at matched activation — the curse of dimensionality in sign-hash. D104
anatomizes this: the Atkinson sweet spot (120 of 324 components), sign-rank expansion
(244→292), and the branching interaction thesis (cross-block > within-block). k is the
real lever, not hash architecture. Together these demos establish that the BFS catalog is
the braid-group-invariant reservoir; all variation in capacity comes from readout design.

### D105: k-Regime Theory and the Macrame Principle

Threads converging:
- D93 "XOR dies at N>=7" (refuted)
- D104 branching decomposition (cross-block vs within-block)
- D104 sign-rank expansion (scaling with module dimension)
- D82 depth law (crossing depth as capacity governor)

Result: 8-strand braids on W_{8,0} (dim=14) and W_{8,2} (dim=28) revive XOR8 at high k,
refuting D93's strand-count wall as a k-regime artifact (k/2^N is the control parameter).
Branching interaction reverses: within-block > cross-block (opposite of D104), establishing
the macrame principle. Rank saturation analysis invalidates all prior single-checkpoint rank
comparisons. The methodological lesson (saturation curves required) is as important as the
computational results.

### D106-D108: From Entropy Null to Parity Prediction

Threads converging:
- D83 writhe correlation (TL-visible property correlates with DKC)
- D90 relational computation thesis (spectral inversion, cross-depth constraints)
- D95 synergy universality (capacity is collective, not individual)
- D50 k-sector parity (906 solutions, 41 parity values)
- D64 poison vocabulary (31 values, octant structure)
- D107 Raqiya toolkit (13 edge types, graph analysis)

Result: D106 falsifies topological entropy as a predictor (clean null across ALL Boolean
functions), establishing the TL visibility filter. D107 proves universal axis-alignment
and introduces the Z/4Z phase formula + nesting parity + major index bipartite invariant.
D108 shows graph structure predicts parity capability: maximum contrast on product closure
(connected vs empty) and additive v_2 connectivity (83% vs trivial K_2). The dual-channel
theorem is the synthesis: parity requires both multiplicative phase coherence and additive
magnitude diversity, mapping onto Amy's T-gate/Hadamard decomposition.

### D109: Encoding-Dependent Dual-Channel

Threads converging:
- D108 dual-channel theorem (product closure + v_2 connectivity)
- D55 bracket catalog at delta=sqrt(2) (9334 parity solutions, Z[zeta_16])
- D52 j=0 sector liveness (delta≠0 enriches algebra)
- D107 Raqiya toolkit (reused for Z[i]-axial Z[zeta_16] analysis)

Result: At delta=sqrt(2), Raqiya CANNOT discriminate parity from non-parity: 7/8 edge
types identical, perfect 28/28 symmetry. Product closure is the sole discriminator with
INVERTED polarity (parity=low, non-parity=high — opposite of D108). The encoding-dependent
dual-channel theorem: additive encoding wants high product closure (self-reinforcement),
multiplicative encoding wants low (vocabulary escape for sector alternation). When algebra
is rich, the simplest activation suffices. Raqiya diagnoses impoverishment, not health.

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

D83 resolves a longstanding mystery (the N-2 gap observed in D79) and reveals that the
bracket-to-Jones normalization step costs exactly 2 XOR levels at every root tested —
the TQFT framing anomaly is not bookkeeping noise but one discrete unit of computational
resource. Writhe (independent of depth at r = 0.14) is a third capacity axis, contributing
+2 XOR levels identically to one depth unit: DKC capacity ≈ XOR6 + 2 × depth_rounds +
2 × framing_present. The direction count is framing-invariant; only angular vocabulary
is destroyed by Jones normalization.

D84 opens the internal anatomy of the 24-element ζ₈ catalog: 9 of 24 entries are
bracket-null (Re(q)=0), covering 6 S² directions unavailable to the 15 non-null entries.
These null entries are indispensable — removing them drops XOR8 → XOR6, below the random-
removal baseline. The depth-null identity (depth-3 stratum = 100% null) connects D82 and
D84: the crossings that contribute most to capacity per the depth law are simultaneously
the ones algebraically invisible to the scalar trace. LCFT Jordan-cell structure explains
the coupling: null entries anchor the partner states that carry the directional content.
Null fraction dilutes with group size (ζ₄=75%, ζ₈=37.5%, ζ₁₂=3%), confirming this is a
finite-group boundary effect concentrated at exactly the quantum-dimension-zero points
where TL non-semisimplicity is maximal and DKC computation is most tractable.

D85 connects TL non-semisimplicity to the LCFT indecomposability parameter b, computing
b = -5/8 at TL_4 via leading-coefficient extraction from the delta-parameterized Markov
trace and resolving the b = -2 vs -5/8 controversy as a normalization difference. D86
proves the single-copy approach universally diverges — a novel negative result showing
the regular-representation multiplicity is structurally essential. D87 reveals that null
indispensability is a finite-group phenomenon: at ζ₁₂ (infinite group, 4096 entries),
removing all bracket-null entries preserves full XOR capacity. D88 anatomizes the anti-
correlation between spherical design quality and computation: 4 non-null body-diagonal
directions are rigid computational anchors while 6 null-only edge-midpoints are flexible
scaffolding, and the k-ladder activation provides 14× better perturbation resilience than
D72's Voronoi-only activation, explaining the D72 catastrophe.

D89-D92 form a four-demo arc resolving the depth law mechanism from D82. D89 kills 5 of
8 hypotheses via systematic testing. D90 identifies the mechanism: axis cancellation
(anti-aligned quaternion pairs produce low sum angles needed for parity) combined with
cross-depth algebraic constraints from BFS ancestry. The spectral inversion (S² bandwidth
DECREASES with depth while computation improves) proves the mechanism is relational not
positional. D91 confirms from the complementary direction that the activation is not the
bottleneck — balanced exponentials (vocabulary ~2×/round, demand 4×/weight) produce the
linear slope. D92 closes the arc with the parity-lock theorem: the +/-q encoding is
structurally locked to XOR/XNOR only (AND/OR/MAJ/THRESHOLD = 0 winners at all depths),
contrasting sharply with D48/D50's 1wpi encoding that achieves all 13 NPN classes. The
depth law is parity-specific, and the encoding determines which Boolean functions are
accessible from the same underlying algebraic structure.

D93-D94 complete the circuit complexity and group-theory arcs simultaneously.

D93 discovers the complement-blindness theorem (combined_cell maps m and ~m to the same
cell via |qa| normalization) and its phase_cell resolution, then measures the circuit
complexity hierarchy (AND/XOR ratio explosion from 1.01 to infinity across N=3-8). D94
extends this to the binary icosahedral group 2I (the unique non-solvable finite SU(2)
subgroup), confirms the hierarchy is universal across all tested groups, and demonstrates
the solvability bottleneck predicted by Barrington's theorem. The crossover at N=6 — where
non-solvable finite 2I overtakes truncated-infinite z12 — shows that non-solvability
provides specifically the structure parity needs at the computational boundary. The five-
pillar synthesis (Abramsky + Habiro + Aizenberg + Nazer-Gastpar + Reservoir Computing) is
completed with a precise, testable mapping to discrete algebraic reservoir computers.

D102-D109 form the matrix braid DKC arc, scaling from 4 to 8 strands and introducing
algebraic graph-theoretic parity prediction.

D102 is the pivot: the Barrington-Radical Principle — the radical of W_{6,4} carries an
abelian writhe character (B_6 → Z/8Z), provably useless for parity by Barrington's theorem
— refutes radical-as-resource and establishes that simple modules outperform non-simple at
matched dimension. D103 settles the growth law: BFS catalog depth profiles are identical
across all three n=6 modules (dim 5, dim 5 non-simple, dim 9), confirming that the BFS tree
is a braid group invariant. The reservoir computing interpretation becomes precise: the
reservoir is invariant; only the readout varies.

D104 anatomizes the activation as a compressed sensing readout. Sign-rank expansion (sign
quantization breaks 48 linear dependencies in W_{6,2}, expanding effective rank from 244 to
292), the Atkinson sweet spot (optimal component count ~120 at k=128), and branching
decomposition (cross-block interaction IS the signal at n=6 but NOT at n=8) provide a
detailed understanding of what the sign-hash activation extracts from the matrix
representation. D105 extends to 8 strands, refuting D93's "XOR dies at N>=7" as a k-regime
artifact (k/2^N is the real parameter) and discovering the macrame principle (branching
interaction reverses from cross-block to within-block as sub-module blocks become complex).

D106 opens the graph-theoretic prediction arc: topological entropy is completely orthogonal
to DKC Boolean computation (falsified prediction, clean null across ALL 8 functions tested).
The TL visibility filter explains why: writhe survives the quotient (correlates with DKC),
entropy does not (e_i^2=0 kills it). Computation is collective and algebraic, not individual
and dynamical.

D107-D109 form a trilogy establishing graph-theoretic parity prediction. D107 proves the
Z/4Z axis-alignment theorem (every braid matrix entry at delta=0 is axis-aligned, with a
constructive phase formula based on nesting parity), discovers the major index bipartite
invariant, and proves a novel q-Catalan identity. D108 shows graph structure predicts parity
capability: maximum structural contrast between parity and poison vocabularies on product
closure (connected P_3 vs empty graph) and additive v_2 connectivity (83% vs trivial K_2),
establishing the dual-channel theorem (parity requires both multiplicative phase coherence
and additive magnitude diversity). D109 tests this at delta=sqrt(2) and finds the polarity
INVERTS under multiplicative encoding: parity wants low product closure (vocabulary escape),
non-parity wants high (vocabulary self-reinforcement). The encoding-dependent dual-channel
theorem unifies D108 and D109.

### D93↔D92: Parallel Parity Locks — Encoding vs Activation (D93↔D92)

D92 proved the parity-lock theorem: the +/-q encoding creates 3^k equivalence classes,
making XOR/XNOR the only computable functions regardless of activation. D93 discovers an
analogous lock operating on a different axis: the combined_cell activation's sign-flip
normalization creates complement-pair collisions, making complement-invariant functions the
only achievable class. D92's lock is in the encoding (applies to all activations); D93's
lock is in the activation (applies to all encodings under combined_cell). The two locks
are independent constraints: the encoding determines WHICH function family, the activation
determines WHICH subfamilies within that family. Phase_cell breaks D93's activation lock
just as 1wpi encoding breaks D92's encoding lock — the same "wall was in X not Y" pattern
at its fourth occurrence.

### D93↔D48/D50: 1wpi Encoding Across Activations (D93↔D48↔D50)

D48 proved zero parity solutions under split-sigmoid with 1wpi encoding. D50 resolved this
with k-sector activation. D93 shows a different activation restriction on the same 1wpi
encoding: combined_cell kills all non-complement-invariant functions through the sign-flip
symmetry. Phase_cell recovers all 13 NPN classes (matching D50's result from a different
activation pathway). The 1wpi encoding is a versatile substrate — it supports all Boolean
functions — but only under activations that preserve the full S^1 phase information. Any
activation that normalizes sign (split-sigmoid via magnitude, combined_cell via |qa|)
collapses the function repertoire.

### D93 Pigeonhole ↔ D63 Parity Ceiling (D93↔D63)

D63 established the parity ceiling at n=5 for Z[zeta_8] under k-sector activation, with
two types of impossibility: constraint-geometric wall (n=6,7) and pigeonhole (n>=8). D93
independently discovers a pigeonhole mechanism at the phase_cell level: at 84 cells (depth
1, 17 entries), XOR requires balanced bisection of 2^N masks. When 2^N > 84 (N>=7), XOR
dies. AND survives because it only needs one unique cell (for the all-1s mask). The two
pigeonhole mechanisms operate at different levels — D63's operates on the Z[zeta_8] angular
lattice structure, D93's operates on the Voronoi × sector cell count — but they share the
same mathematical form: computation fails when the function's partition complexity exceeds
the discrete partition capacity of the activation. Both confirm that parity's difficulty is
fundamentally a capacity problem, not an algebraic one.

### D94↔D80: Finite/Infinite = Solvable/Non-Solvable (D94↔D80)

D80 established the finite/infinite group boundary as the capacity governor: only ζ₄ and ζ₈
generate finite SU(2) subgroups, and finite groups impose hard XOR ceilings while infinite
groups scale without bound. D94 adds a new dimension to this picture: within the finite
groups, solvability determines capacity. The binary octahedral group z8 (E₇, order 48,
solvable) has lower XOR capacity than the binary icosahedral group 2I (E₈, order 120,
non-solvable) at matched catalog size. The D80 landscape now has three regimes:
- **Solvable finite** (z8, E₇): hard ceiling, lowest per-entry capacity
- **Non-solvable finite** (2I, E₈): hard ceiling, higher per-entry capacity
- **Infinite** (z12, z10, etc.): no ceiling, logarithmic scaling
The finite/infinite boundary governs whether a ceiling exists; the solvable/non-solvable
boundary governs how much computation is available within that ceiling.

### D94↔Barrington: Theoretical Prediction Confirmed Computationally (D94↔Barrington)

Barrington's theorem (1989) proves NC¹ = programs over non-solvable groups, and that
solvable groups are strictly weaker for branching program computation. D94 is the first
computational confirmation of this prediction in a topological computation model. The DKC
catalog construction (BFS closure, multiplicative group generation) is Barrington-like:
braid words are branching programs, group elements are states, and the additive readout
separates Boolean function values. At matched catalog size (24 entries), 2I (non-solvable)
beats z8 (solvable) at XOR with the advantage accelerating precisely as the computation
becomes harder — exactly Barrington's prediction made concrete in DKC.

### Circuit Complexity AC⁰/TC⁰/Parity Hierarchy in DKC (D93↔D94↔Hastad)

D93's AND/XOR ratio table is Hastad's theorem (1987) made visible. AND is in AC⁰ (low
Fourier degree, simple partition geometry). MAJ is TC⁰-complete (intermediate Fourier
degree). XOR is parity (full Fourier degree, requires exponential depth in bounded-width).
The LMN theorem (1993) explains AND's robustness: AC⁰ Fourier mass concentrates below
degree (log n)^d, so AND needs only coarse cell partitions. XOR requires balanced global
partitions — the most demanding geometric constraint. D94 confirms this hierarchy is
universal: it manifests identically in z8, 2I, and z12. The readout mechanism (1wpi +
phase_cell) is the bottleneck, and the group merely modulates how much capacity is available
to overcome that bottleneck. The circuit complexity hierarchy is not an abstract classification
for DKC — it is a measurable, quantitative prediction about hit-rate divergence.

### D93-D105 Complete the "Wall Was X Not Y" Pattern (D93↔D50↔D77↔D79↔D105)

The pattern now has seven instances. D93's combined_cell activation kills non-complement-
invariant functions under 1wpi. Phase_cell recovers all 13 NPN classes. D105 refutes
D93's "XOR dies at N>=7" as a k-regime artifact. The full pattern:
1. D48→D50: wall in split-sigmoid activation, not Z[zeta_8] lattice
2. D76→D77: wall in S²-only activation, not ζ₈ root of unity
3. D78→D79: wall in group finiteness, not DKC architecture
4. D92: wall in +/-q encoding, not the activation (parity lock)
5. D93: wall in combined_cell normalization, not 1wpi encoding (complement blindness)
6. D98→D99: wall in trace readout / reducible representation, not multi-strand algebra
7. D93→D105: wall in k-regime (k/2^N ratio), not strand count or algebra

Each instance deepens understanding of the modular architecture: encoding, activation,
group structure, normalization convention, readout channel, and k-regime are independent
design variables, each with its own constraint set.

### D94 Quantum Dimension Table Extends D80 (D94↔D80↔D58)

D80's survey classified roots by finiteness and quantum dimension. D94 extends this to the
non-solvable case: 2I at the Fibonacci parameter has [2]_q = phi^{-1} ≈ 0.618 (non-zero,
non-degenerate), placing it in the "hard" regime where Kuperberg's #P-hardness applies.
Yet 2I IS finite (120 elements). The quantum dimension table now maps directly onto
computational capacity: [2]_q = 0 (z8, solvable, ceiling), [2]_q ≠ 0 + finite + non-
solvable (2I, higher ceiling), [2]_q ≠ 0 + infinite (z12/z10, no ceiling). The Fibonacci
parameter from D58 (ell=5, delta=phi, universal TQC) connects to D94's 2I through the
same golden ratio: phi is the quantum dimension at the Fibonacci point, and 2I IS the
Fibonacci anyon group realized as a finite SU(2) subgroup.

### D94 Reservoir Computing Mapping Completes Five-Pillar Synthesis (D94↔D73↔D82↔D83↔D84)

D73 (additive DKC), D82 (depth = crossing depth), D83 (writhe as independent axis), and
D84 (null states maintain manifold separation) each contributed one aspect of the reservoir
computing interpretation. D94 completes the mapping with precise (not metaphorical)
identification: fixed catalog = reservoir state matrix, Cayley graph = connectivity
topology, signed weight sum = linear readout, phase_cell = nonlinear output function,
BFS depth = memory depth. The testable prediction — rank(K_2I)/rank(K_z8) > 120/24 if
non-solvability contributes above raw size — is a single RKHS kernel computation (seeded
as D95) that could confirm or refute the entire algebraic reservoir interpretation.

### D95 Synergy Universality: D84 and D95 Are Independent Partitions (D95↔D84)

D84 established 0+0=36 pure synergy: null-only and non-null-only each produce zero XOR6
at k_sec=1, but their union produces 36. D95 found the same pattern at the derived series
level: COMM-only (12 entries) and NON-COMM-only (12 entries) perform similarly in isolation,
but ALL(24) gives 2.7x the rate at N=5. The question was whether these are the same split
seen through different lenses. D96 answered definitively: they are INDEPENDENT partitions.
The 9 bracket-null entries come from TWO derived levels (6 from D0, 3 from Q8/D2), and
the derived series cuts across the null boundary. Their cross-product creates the 5-cell
landscape that fully explains DKC capacity structure.

### D96 Cross-Classification Resolves D84+D95 (D96↔D84↔D95)

D96 is the synthesis of D84 and D95. D84's partition (null/non-null, 9/15) and D95's
partition (derived series, 12/8/3/1) are confirmed independent by cross-tabulation. The
5-cell landscape (A-E) reveals that D84's "null indispensability" specifically refers to
Cell A (D0-null, outermost) -- these are synergy enhancers when mixed with non-null entries.
Cell D (Q8-null, deep) is not indispensable; it is noise. Removing Cell D improves capacity
at every N. D95's "COMM vs NON-COMM parity" is explained by contamination: both partitions
contain null cells that drag down their individual scores. The true signal is B+C (non-null
x cross-derived synergy), which was masked when averaging over all entries in each partition.

### D97 Geometric Explanation Connects to D66/D68 (D97↔D66↔D68)

D66 discovered the 24-cell polytope as the natural DKC geometry, with binary octahedral
group vertices. D68 established intrinsic curvature of DKC on S^2. D97 identifies the
specific geometric mechanism: Cell B's 6 entries form a regular octahedron (the dual of
the cube, one of the 24-cell's constituent polytopes). The 3 mutually orthogonal directions
at the S^2 level provide maximally separated Voronoi cells. The lever arm mechanism (nonzero
real component enables phase_cell discrimination) is a concrete realization of D67's Hopf
phase inertness finding: the S^1 fiber is irrelevant, but the magnitude of the S^1 projection
(cosine of half-angle) determines the lever arm strength.

### D98 Readout Bottleneck Connects to D35 and D67 (D98↔D35↔D67)

D35 established entanglement vanishing (non-interleaving braids have zero bracket) and the
axiality theorem (bracket values have single nonzero Cyc8 component). D98 confirmed both
at scale: 31/31 non-interleaving entries have zero bracket, and traces are axial. D67
showed that the Hopf phase is inert (wrong readout kills computation even when data is rich).
D98 is the multi-strand analog: the trace readout is catastrophically lossy (5.1% retention),
destroying computation that the algebra fully supports (column-4 is lossless). The lesson
from D67 — wrong readout kills computation — applies at the representation level, not just
the activation level.

### D99 Delta_1 Module Connects to D51 Semisimplicity (D99↔D51)

D51 established that TL_3 at delta=0 is semisimple (n=3 odd). D99 builds on this: TL_3 is
semisimple, but the braid group image through the Delta_1 standard module is infinite. The
Jordan block in sigma_1 proves the extension does not split: Ext^1(L(0), L(1)) is non-zero.
This is not a contradiction — TL_3 semisimplicity governs the TL algebra, while the braid
group image has richer structure. The non-split extension is the computational resource
that enables XOR14, something impossible in the 2-strand finite group.

### D99 Depth Scaling Connects to D82 Depth Law (D99↔D82)

D82 established the linear depth law: max_xor ~ depth + 6 for 2-strand DKC. D99 shows
that depth is equally critical for 3-strand: XOR14 requires deep entries (d>=6), and
shallow entries give definitively zero. The depth law mechanism (algebraic coherence from
cross-depth composition) generalizes to matrix-valued DKC: super-hubs at d=6 combine with
satellites at d=7, and 100% of XOR14 winners are mixed-depth. The crossing depth interpretation
from D82 extends directly to multi-strand braids.

### D100↔D101 Simple vs Non-Semisimple Comparison (D100↔D101)

D100 (W_{4,2}, non-semisimple, n=4 even) and D101 (W_{5,3}, simple, n=5 odd) provide the
first controlled comparison of DKC on modules of different types. Key contrasts:
- Growth rate: ~3.1x (D100) vs ~4x (D101), tracking n-1 as predicted
- Hub count: 6 (D100) vs 3 (D101), falsifying the sl_d scaling prediction
- XOR14: 70 (D100) vs 0 (D101), demonstrating non-semisimple advantage at depth
- Casimir correlation: uniformly positive (D100) vs inverted at high XOR levels (D101)
- Radical content: constant at all depths (D100) vs absent (D101)
- Hub commutators: traceless with zero radical content (D100) vs zero matrix (D101)
The simple module has broader shallow capacity but strictly less deep capacity, confirming
that the non-semisimple extension structure is computationally load-bearing.

### Ext^1 Thesis Across D99-D101 (D99↔D100↔D101↔D84)

D84 first identified non-semisimplicity as the computational resource (null states = LCFT
Jordan-cell structure). D99 made this precise: the non-split extension Ext^1(L(0), L(1)) is
the catalytic preparation mechanism. D100 confirmed: constant radical content at all depths,
hub commutators annihilate the radical. D101 provided the negative control: the simple module
(no radical, no extension) achieves zero XOR14 despite larger dimension and faster growth.
The causal chain is now established across four demos:
1. Non-semisimplicity creates the extension structure (D84 principle)
2. Extension creates Jordan blocks giving infinite group (D99 mechanism)
3. Deep entries accumulate algebraic complexity (Casimir growth, D100 data)
4. Cross-depth composition with extension-prepared entries produces high-XOR capacity
5. Without extension, the chain breaks at step 2 and deep capacity vanishes (D101 control)

### D93-D109 Complete the "Wall Was X Not Y" Pattern (Extension)

The wall-identification pattern now has SEVEN instances: D98's trace readout gives ZERO
XOR despite the 3-strand algebra being infinite and rich. D99's Delta_1 module + sign-hash
activation recovers 500K+ XOR6 solutions. D105 refutes D93's "XOR dies at N>=7" by
showing the wall was in the k-regime, not the strand count:
1. D48→D50: wall in split-sigmoid activation, not Z[zeta_8] lattice
2. D76→D77: wall in S²-only activation, not ζ₈ root of unity
3. D78→D79: wall in group finiteness, not DKC architecture
4. D92: wall in +/-q encoding, not the activation (parity lock)
5. D93: wall in combined_cell normalization, not 1wpi encoding
6. D98→D99: wall in trace readout / reducible representation, not multi-strand algebra
7. D93→D105: wall in k-regime (k/2^N ratio), not strand count or algebra

### Traceless Hub Commutator Universal Pattern (D99↔D100↔D101)

A new recurring pattern: super-hub commutators are traceless in EVERY multi-strand demo
where hubs appear. D99: 2 hubs, commutator traceless with strong sl_2 bracket structure
(Cartan L1=84, e-root L1=108). D100: 6 hubs, ALL six commutators traceless AND radical
content = 0 (commutators live entirely in the semisimple quotient). D101: 3 hubs, ALL
commutators are the ZERO matrix (hubs mutually commute). The pattern strengthens across
demos: from traceless (D99) to traceless + radical-annihilating (D100) to zero (D101).
Whether this is a theorem (commutators of hub-type entries on TL standard modules are
always traceless) or specific to these examples remains open.

### Barrington's Theorem Made Concrete in Topology (D94↔D102)

D94 first connected Barrington's theorem to DKC by showing the solvability bottleneck
computationally: non-solvable 2I outperforms solvable z8 at matched catalog size. D102
makes this structural: the radical of W_{6,4} carries an abelian writhe character
(B_6 → Z/8Z via A = -zeta_8), computationally proven for ALL 32,768 catalog entries.
By Barrington's theorem, abelian groups cannot compute parity. Therefore the radical
direction is theoretically guaranteed useless for XOR — and empirically, the simple
module W_{6,0} wins at every XOR level. This is the first time a specific TL-algebraic
structure has been identified as provably useless by appeal to classical complexity
theory, rather than empirical refutation alone.

### BFS Growth Independence Across Three n=6 Modules (D102↔D103)

D102 showed W_{6,0} and W_{6,4} have identical BFS depth profiles. D103 adds W_{6,2}
(dim=9, nearly double the dimension): STILL identical at every depth. The BFS catalog
is a braid group property. The matrices can be 5×5 or 9×9, simple or non-simple, 0
or 4 through-lines — the BFS tree is the same. This is the reservoir computing
interpretation made precise: the reservoir (braid group orbit) is invariant; only the
readout (sign hash of which matrix representation) varies.

### Fibonacci Max-Abs Pattern and Radical Constraint (D102↔D103)

Both simple n=6 modules follow max coefficient magnitude 1,1,2,3,5,8,16 through depth
6. The non-simple W_{6,4} follows strict Fibonacci (1,1,2,3,5,8,13) at depth 6 instead
of breaking to 16. The radical appears to constrain coefficient growth to strict Fibonacci
— another manifestation of the radical carrying structured (abelian) information. The
abelian writhe character from the Barrington-Radical Principle literally constrains the
radical row to A^writhe, preventing the "wild" growth seen in simple modules.

### Branching Interaction Reversal: The Macrame Principle (D104↔D105)

D104 showed that at n=6, the computational signal lives in the cross-block (off-diagonal)
components under TL_{n-1} branching restriction: W_{6,2} cross-block (160 comp) beats
within-block (164 comp) at k=128. D105 tested this at n=8 (symmetric 14+14 branching)
and found the OPPOSITE: within-block dominates (XOR6=2940 vs cross-block=532).

The regime transition is the "macrame principle": simple sub-module blocks (dim 4-5) need
connections between them for computational fabric, like simple knots needing macrame
cord. Complex sub-module blocks (dim 14) are self-sufficient, like complex knots that
hold structure without external connections. The crossover lies between dim 5 and dim 14.

### k-Regime Rescues D93's "Dead" Functions (D93↔D105)

D93 concluded "XOR dies at N>=7" based on experiments at k=128. D105 showed this was a
k-regime artifact: at k=4096 with n=8, XOR8=22266 (actually BEATING n=6's 21699 at
matched k). The ratio k/2^N is the real control parameter:
- k=128, N=8: k/2^N = 0.5 (pigeonhole regime — too few cells for 256 masks)
- k=4096, N=8: k/2^N = 16 (collision avoidance regime — XOR fully alive)

This is the fourth instance of the "wall was X not Y" pattern in the activation/k-regime
family: the apparent strand-count wall was entirely in the activation granularity.

### Topological Entropy Null Extends the TL Visibility Filter (D106↔D83)

D83 established that writhe (which survives the TL quotient) correlates with DKC output.
D106 is the negative control: topological entropy (which does NOT survive the TL quotient,
because e_i^2=0 kills expanding eigenvalues) is completely orthogonal to DKC computation.
Both periodic and pseudo-Anosov braids compute XOR at 100% participation with near-identical
scores. The TL quotient at delta=0 is the visibility filter: properties that pass through it
(writhe, algebraic phase) are DKC-relevant; properties killed by it (entropy, Burau spectral
radius) are DKC-invisible. D106 thus confirms D83 from the complementary direction.

### Axis-Alignment Theorem: Universal Structure at Delta=0 (D107↔D35)

D35 established the axiality theorem: bracket values have single nonzero Cyc8 component.
D107 elevates this from observation to theorem with a constructive proof: the Z/4Z phase
formula Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4} gives the
phase exactly. Three numbers — writhe, nesting count of source state, nesting count of
target state — fully determine which cyclotomic axis a matrix entry lies on. This reduces
DKC at delta=0 from cyclotomic arithmetic to integer arithmetic plus a phase lookup,
explaining why the computation is so tractable despite living in Z[zeta_8].

### Dual-Channel Theorem and the Amy Bridge (D108↔D65↔D50)

D108's dual-channel theorem — parity needs both multiplicative phase coherence AND additive
magnitude diversity — connects to quantum circuit synthesis through the Amy-Glaudell-Ross
(2023) framework. The activation function provides the Hadamard-equivalent magnitude
diversity that enables sde=0 ("Clifford") values to compute parity. D65's generalized XOR6
at k=24 is reframed: k=24 provides sufficient "Hadamard resource" to complement the phase
coherence of the Z[zeta_8] lattice. D50's k=6 breakthrough is the minimal dual-channel
configuration.

### Product Closure Polarity Inversion (D108↔D109)

D108 (delta=0, additive encoding) found parity wants HIGH product closure (21% density,
connected P_3) while poison wants ZERO. D109 (delta=sqrt(2), multiplicative encoding) found
the opposite: parity wants LOW product closure (14.8%) while non-parity wants HIGH (46.6%).
The sign flip is encoding-dependent: under additive encoding, products that stay within the
vocabulary reinforce superposition; under multiplicative encoding, products must ESCAPE the
vocabulary for sector alternation. The dual-channel theorem holds in both regimes but the
polarity of the multiplicative channel reverses.

### Perfect Algebraic Symmetry at Delta≠0 (D109↔D52)

D109's 7/8 edge-type identity between parity and non-parity at delta=sqrt(2) connects to
D52's observation that j=0 sector liveness at delta≠0 enriches the algebra. When j=0 is
alive, the algebra is so rich that both parity and non-parity vocabularies satisfy all
structural prerequisites (K_4 quotient, non-trivial v_2 connectivity, adequate additive
closure). Raqiya diagnoses algebraic impoverishment (D108, delta=0), not algebraic health
(D109, delta≠0). The diagnostic power of graph analysis is strongest exactly where the
algebra is most degenerate.

### Rank Saturation Confound Retroactively Affects All Prior Comparisons (D105)

D105's rank saturation analysis revealed that ALL prior cross-module rank comparisons
(D99b through D104) were confounded by varying oversampling ratios. The rank of W_{8,2}
at 512 entries was only 6.9% (severely under-sampled), but the coincidental 34.9% match
between W_{8,0} and W_{8,2} at initial checkpoints could have been mistaken for a
structural similarity. The methodological lesson: single-checkpoint rank measurements are
unreliable for infinite braid groups. Saturation curves at multiple checkpoints are now
required — a methodological advancement that retrospectively invalidates some D103-D104
rank claims as lower bounds rather than structural truths.
