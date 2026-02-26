# Research Arc Summaries

Backbone document grouping all demos into natural research arcs by intellectual unit.

---

## Arc 1: Foundation (D01-D09)

**Question**: Can the Kauffman bracket be computed exactly from scratch using multiple algebraic representations?
**Answer**: Yes -- five independent computation methods (state-sum, braid, TL module, spin chain, Jones-Wenzl) all agree, and the bracket is verified invariant under Reidemeister moves.

### Journey
- D01 built the state-sum oracle (brute-force 2^n enumeration), the ground truth for all later work
- D02-D06 constructed algebraic representations: braid words, TL standard modules, tensor-product spin chains, Jones-Wenzl projectors
- D07-D08 bridged PD-code and braid-word representations bidirectionally (Vogel algorithm)
- D09 verified Reidemeister invariance, closing the foundation
- The quantum trace formula tr_q = (-1)^n * delta * bracket(oracle) was confirmed at every scale by D05

### Key Artifacts
- `Poly` type: symbolic Laurent polynomial ring (808 bytes/entry), exact but expensive
- State-sum oracle: ground truth for all bracket computations
- Union-find loop counter: used in 37+ subsequent demos
- TL matrix representation infrastructure (reused through D109)

### What It Unlocked
- Arc 2 (Classification) built directly on this for numerical exploration
- The state-sum oracle remained the validation target through D59
- TL infrastructure became the backbone of the entire project

### Open Threads
- Symbolic `Poly` type too expensive for large-scale work (resolved by D10's numerical pivot)

---

## Arc 2: Classification (D10-D17)

**Question**: What information can be extracted from bracket values when treated as numerical signals?
**Answer**: The bracket encodes quantized angular structure, waveform classification, and a discrete information-theoretic landscape that converges to algebraically special evaluation points (WRT).

### Journey
- D10 replaced symbolic `Poly` (808 bytes) with numerical `Cx` (16 bytes), enabling 50-100x scaling
- D11-D12 explored bracket waveforms and discovered quantization
- D13 performed the first "DKC test" -- can bracket values distinguish Boolean functions?
- D14-D15 mapped zero structure and WRT angles; greedy angle selection converged to within 1.5% of the Chern-Simons evaluation point
- D16-D17 explored reduced brackets and multi-layer architectures

### Key Artifacts
- `Cx` complex arithmetic: numerical workhorse used in 35+ demos
- WRT convergence result: topology "knows" information theory (D15)
- NPN classification machinery introduced (D17, refined in D23)

### What It Unlocked
- Arc 3 (Gate) used the numerical engine for circuit compilation
- The WRT convergence hinted at the algebraic evaluation points that Arc 4-6 would exploit

### Open Threads
- Floating-point approximation limits (resolved by exact cyclotomic rings in D29+)

---

## Arc 3: Gate (D18-D25)

**Question**: Can braids serve as universal classical logic gates, and what does topology prefer to compute?
**Answer**: Yes -- braids achieve computational universality (NOT, NAND), but the parity function has zero appearances across 1554 braids x 4 closures, establishing the parity barrier.

### Journey
- D18-D19 proved universality via NOT/NAND gate construction and circuit compilation
- D20 mapped the error landscape of approximate gate realization
- D21-D22 explored reversible gates and minimum braid complexity for each Boolean function
- D23 built the "function zoo" with NPN classification (13 non-trivial classes)
- D24 was a standalone tangent connecting brackets to rational tangle arithmetic
- D25 measured closure-type effects and first surfaced the parity barrier (zero appearances)

### Key Artifacts
- NPN classification: used in 13+ subsequent demos
- Braid word decoder for exhaustive enumeration (13+ demos)
- Parity barrier: the central mystery driving Arcs 4-6

### What It Unlocked
- Arc 4 (Reverse DKC) attacked the parity barrier from the neural network direction
- The function zoo and NPN machinery became standard infrastructure

### Open Threads
- Parity barrier unexplained (resolved across Arcs 5-6 and Arc 9)

---

## Arc 4: Reverse DKC (D26-D28)

**Question**: Can trained neural network weights be decomposed into Kauffman bracket values?
**Answer**: Yes -- reverse decomposition works for real and complex weights, with U(1) gauge symmetry discovered, and different activations (split-sigmoid, modReLU, cardioid) access different parts of the bracket algebra.

### Journey
- D26 demonstrated reverse DKC with real-valued weights
- D27 extended to complex weights, discovering U(1) gauge symmetry in the decomposition
- D28 compared activation functions (modReLU, cardioid, split-sigmoid), finding each accesses different algebraic sectors
- The "training as focusing" thesis emerged: trained weights converge to bracket lattice points

### Key Artifacts
- U(1) gauge symmetry of bracket-weight correspondence
- Split-sigmoid activation as the primary DKC readout
- Training-as-focusing thesis

### What It Unlocked
- Arc 5 (Forward DKC) reversed the direction: bracket values compute directly without training
- Activation function comparison seeded the "wall was activation" pattern

### Open Threads
- Can forward DKC work without training? (Answered immediately by D29)

---

## Arc 5: Forward DKC (D29, D45-D48)

**Question**: Can exact bracket values compute Boolean functions without any training?
**Answer**: Yes -- forward DKC is confirmed with 100+ exact XOR triples from Z[zeta_8] bracket values. All 13 NPN classes are computable. But parity has zero solutions out of 100M quartets under split-sigmoid, and XNOR3 has exactly zero solutions under the k-sector search.

### Journey
- D29 (LANDMARK): forward DKC confirmed -- exact Z[zeta_8] values at delta=0 compute XOR with RMS=0.000
- D45 mapped all 13 NPN classes and discovered forward-reverse duality (Spearman rho = -0.82)
- D46 investigated per-neuron gauge and phase decoherence
- D47 tested MVN activation, finding correlation reversal (rho = -0.78 vs +0.55 under split-sigmoid)
- D48 (LANDMARK): exhaustive 100M quartet search proved zero parity solutions under split-sigmoid, sharpening the wall

### Key Artifacts
- Forward DKC catalog: 100 exact Z[zeta_8] bracket values (ground truth for all Boolean reachability)
- Forward-reverse duality measurement
- Split-sigmoid parity wall (zero out of 100M)
- D47 conjecture: TL non-semisimplicity is the parity barrier

### What It Unlocked
- Arc 6 (Activation Breakthrough) resolved the parity wall
- D47's conjecture drove Arc 7 (Radical Anatomy)
- The catalog became the substrate for Arcs 8-20

### Open Threads
- Parity unreachable under split-sigmoid (resolved by D50)
- Non-semisimplicity conjecture (explored in Arc 7, partially confirmed, partially superseded)

---

## Arc 6: Activation Breakthrough (D49-D50)

**Question**: Is the parity wall in the weight lattice or the activation function?
**Answer**: The wall is entirely in the activation. The same 100-value catalog that yielded zero parity solutions under split-sigmoid yields 906 solutions at k=6 sectors. Neglecton weights (radical elements) add no new angular directions.

### Journey
- D49 tested perturbation via radical elements ("neglectons"): 148 new Z[zeta_8] values from 15,242 braids, but ALL share angles with existing values -- the perturbation approach is closed
- D50 (LANDMARK): k-sector activation solves parity at k=6 with 906 solutions from the same catalog
- Four-tier NPN hierarchy discovered: k=2(5 classes), k=3(+1), k=4(+6), k=6(+1 = all 13)
- Non-monotonicity: 906 at k=6 > 756 at k=7 > 96 at k=8 (pi/4 incommensurability)

### Key Artifacts
- 906 parity solutions at k=6
- Four-tier NPN hierarchy
- Pi/4 incommensurability hypothesis
- First instance of "wall was activation, not lattice" pattern (recurs 5+ more times)

### What It Unlocked
- All subsequent DKC work builds on the confirmed 13/13 class reachability
- The "wall was X not Y" pattern became a recurring structural insight

### Open Threads
- 41/100 catalog values participate in parity solutions, octants {2,4,5,7} only (explored in Arc 20)
- Incommensurability with pi/4 lattice (explored in Arc 9)

---

## Arc 7: Radical Anatomy (D38-D39, D51-D52, D60)

**Question**: What is the algebraic structure of TL non-semisimplicity at delta=0, and how does it connect to computational barriers?
**Answer**: The radical has dimension 2l-3 (Graham-Lehrer), the sandwich theorem holds (rad^2 isomorphic to previous odd TL), and the alternating semisimplicity pattern (even=non-semisimple, odd=semisimple) extends across all tested delta values including the first cubic number field.

### Journey
- D38 proved the dimension reduction theorem: TL_n/rad is isomorphic to TL_{n-1} at even n
- D39 decomposed TL into cellular sectors, verified d(n,j)^2 sizes, mapped multi-delta Gram ranks, and proved three formal theorems (Markov RT truncation, shadow interpretation)
- D51 discovered the sandwich theorem and alternating semisimplicity at delta=0
- D52 extended to the full Chebyshev parameter space: delta=0,1,sqrt(2),phi,sqrt(3)
- D60 confirmed universality at the first cubic number field (ell=7)

### Key Artifacts
- Sandwich theorem: rad^2(TL_{2m}) isomorphic to TL_{2m-3}
- Catalan trace theorem, Fibonacci rank theorem
- Cross-ell radical dimension formulas: rad(TL_l) = 2l-3
- Markov RT truncation theorem (D39)

### What It Unlocked
- Arc 8 (Cross-ell Survey) used the radical framework for ring-parametric DKC
- Arc 14 (Mechanistic) built on the non-semisimplicity understanding
- The radical anatomy informed the null-state analysis in Arc 12

### Open Threads
- Connection between radical structure and computational capacity (explored in Arcs 14, 17)

---

## Arc 8: Cross-ell Survey (D53-D59)

**Question**: Does DKC work at evaluation levels beyond ell=2 (delta=0)?
**Answer**: Yes -- the same topology code works verbatim across five cyclotomic rings, and the same 11/13 NPN wall appears universally under Re>0 activation with the same two unreachable classes (0x06, 0x1B).

### Journey
- D53 (ell=3, Z[omega], delta=1), D54 (ell=4, Z[zeta_16], delta=sqrt(2)), D58 (ell=5, Z[zeta_5], delta=phi), D59 (ell=6, Z[zeta_24], delta=sqrt(3)) -- each implemented a new ring
- D55 ran Boolean search at ell=4, D56-D57 explored Potts-TL and additive encoding
- The topology code (planar matchings, diagram composition, trace closure) was ring-independent
- Fibonacci golden ratio discovered at ell=5: delta powers are literal Fibonacci numbers
- 11/13 wall observed independently at ell=4, ell=5, ell=6

### Key Artifacts
- Five cyclotomic ring implementations with common API
- Universal 11/13 wall: same two classes unreachable regardless of ring
- Ring tower: Z -> Cx -> Z[zeta_8] -> Z[omega] -> Z[zeta_16] -> Z[zeta_5] -> Z[zeta_24]

### What It Unlocked
- Arc 9 (Analytical Wall) proved the 11/13 universality analytically
- The cross-ell data established that the wall is activation-geometric, not algebraic

### Open Threads
- Why exactly 0x06 and 0x1B? (Answered by D62's two distinct proof structures)

---

## Arc 9: Analytical Wall (D61-D65)

**Question**: Can the 11/13 wall be proven analytically, and what governs parity scaling?
**Answer**: Yes -- integer-arithmetic proofs show 0x06 and 0x1B are unreachable by ANY half-plane activation (two different geometric obstructions), and the parity hierarchy is a matroid minor chain with a hard ceiling at n=5 inputs.

### Journey
- D61 gave a computational proof via N=360 integer grid
- D62 gave analytical proofs: 0x06 fails by local parallelogram argument, 0x1B by global interval squeeze -- two fundamentally different mechanisms
- D63 established the complete parity scaling law for Z[zeta_8]: ceiling n=5, extending from D50's n=3
- D64 revealed the parity hierarchy as a matroid minor chain (deletion = XOR_{n-1}, contraction = XNOR_{n-1}) with vocabulary stratification (44 good / 31 poison)
- D65 tested the Clifford staircase conjecture: standard Z[zeta_16] leaves ceiling at n=5, but generalized labelings break XOR6 at k=24

### Key Artifacts
- Zero-floating-point analytical proofs of the 11/13 wall
- Matroid minor chain for parity hierarchy
- Self-doubling impossibility theorem (D64)
- k=24 coincidence (explained by D66)

### What It Unlocked
- Arc 10 (Quaternionic) explained the k=24 coincidence as 24-cell projection
- The matroid framework connected DKC to classical combinatorics

### Open Threads
- k=24 mystery (resolved by D66)
- Cyclotomic matroid mu_8 for the 44/31 vocabulary split (open)

---

## Arc 10: Quaternionic/Spherical (D66-D71)

**Question**: What is the natural geometric setting for DKC, and why does the 13-direction structure work?
**Answer**: SU(2) quaternions are the natural setting. The 24-cell (binary octahedral group) is the braid geometry. S^2 (rotation axis) is the computational sweet spot. The Hopf fiber phase is inert. 2*6+1=13 is the exact DOF coincidence explaining why 13 directions suffice.

### Journey
- D66 lifted the bracket from S^1 to S^3, discovering the 24-cell as braid geometry and resolving the k=24 coincidence
- D67 proved S^2 is the computational sweet spot: 13-direction Voronoi gives 36 solutions at 14 cells, beating full S^3
- D68 proved the computation is intrinsically curved (Euclidean metric gives zero solutions)
- D69 proved universal direction nesting (zeta_8 subset of zeta_16 subset of zeta_32) and trivialization at finer roots
- D70 mapped the algebra to music: 4 notes, Yang-Baxter melodies, pitch-space orthogonality (r = -0.06)
- D71 closed with the 13=13 theorem: minimum bandwidth l=6 because 2*6+1=13, sharp phase transition

### Key Artifacts
- 24-cell as the natural DKC geometry (binary octahedral group, 24 quaternions)
- S^2 sweet spot: Hopf base carries computation, fiber carries pitch
- 13=13 theorem: harmonic-analytic necessity for the direction count
- Music-space orthogonality

### What It Unlocked
- Arc 11 (Characterization) tested the algebraic optimality of the 13 directions
- Arc 12 (Scaling) used the quaternionic framework for capacity theory
- The Hopf fibration decomposition informed all subsequent activation design

### Open Threads
- Are the 13 directions truly optimal? (D72 shows they are algebraically optimal but geometrically worst)

---

## Arc 11: Characterization (D72-D75)

**Question**: What makes the 13 eigenvector directions computationally optimal, and what is the full invariant theory of DKC?
**Answer**: The directions are algebraically selected, not geometrically optimized -- geometric improvement destroys computation (t=0 design, 9x fewer solutions). DKC is fundamentally additive (not multiplicative). Bracket and Voronoi cell are incomparable invariants that become ordered at the sum level.

### Journey
- D72 confirmed anti-correlation: t=0 design (worst possible) yet 2.6x better geometry gives 9x fewer solutions
- D73 proved DKC is additive: multiplicative braid product cannot compute XOR6 (complement maps M to M^{-1}, same eigenvector cell). 82.8% universal determinism constant
- D74 proved bracket and cell are incomparable: combined gives 119 classes vs 100 bracket-alone
- D75 discovered binocular DKC: eigenvalue angle subsumes cell at the sum level, 2.3x capacity, sqrt(n) magnitude quantization

### Key Artifacts
- Anti-Correlation Theorem: sampling quality anti-correlates with computation
- Additive DKC proof: 82.8% universal determinism
- Incomparability Theorem: 119 combined classes
- Binocular channel: angle + direction = complete readout

### What It Unlocked
- Arc 12 (Scaling) used the product activation (S^1 x S^2) derived from the binocular insight
- The additive formulation retroactively explained all forward DKC since D29

### Open Threads
- sqrt(n) magnitude quantization mechanism (partially explained by inner product structure of 24-cell)

---

## Arc 12: Scaling and Finiteness (D76-D84)

**Question**: How does XOR capacity scale with inputs, and what governs the capacity ceiling?
**Answer**: Group finiteness is the capacity governor. Finite groups (zeta_4, zeta_8) impose hard ceilings; infinite groups (zeta_12) scale as max_xor ~ depth + 6. Framing and null states are independent computational resources that TQFT discards but DKC uses.

### Journey
- D76-D77: XOR8 = 0 under S^2-only -> resolved by S^1xS^2 product activation (6 solutions). Second "wall was activation" instance
- D78-D79: XOR10 = 0 at zeta_8 exhaustively -> resolved by switching to zeta_12 (infinite group, 124 solutions). Wall was group finiteness
- D80 surveyed 7 roots: only zeta_4 and zeta_8 generate finite SU(2) subgroups (ADE classification)
- D81 measured logarithmic scaling law: max_xor ~ 0.62 * log2(catalog) + 4.6
- D82 reinterpreted as depth law: max_xor ~ depth + 6 (log is corollary of exponential catalog growth)
- D83 discovered framing = +2 XOR levels (Jones normalization costs exactly 2), writhe is independent axis
- D84 proved null states are directionally indispensable at zeta_8 (removing them drops XOR8 to XOR6)

### Key Artifacts
- Linear depth law: max_xor ~ depth + 6
- Resource decomposition: capacity ~ XOR6 + 2*depth + 2*framing
- Null indispensability at zeta_8 (6 exclusive S^2 directions)
- ADE classification of DKC capacity regimes

### What It Unlocked
- Arc 14 (Mechanistic) explained the depth law mechanism
- Arc 15 (Circuit Complexity) built on the function-specific capacity theory
- Arc 16 (Catalog Optimization) used the null/non-null anatomy

### Open Threads
- What is the mechanism behind algebraic coherence? (Answered by D89-D90)
- Does null indispensability generalize? (D87 shows it is regime-dependent)

---

## Arc 13: Framing/Null (D83-D84)

*Folded into Arc 12 above. D83 and D84 are integral parts of the Scaling arc -- D83 resolved the N-2 mystery from D79 and established writhe as an independent axis; D84 opened the internal anatomy of the catalog that drove Arcs 14 and 16.*

---

## Arc 14: Mechanistic (D85-D92)

**Question**: What is the algebraic mechanism behind the depth law, and what determines which Boolean functions are computable?
**Answer**: Axis cancellation (anti-aligned rotation axes) combined with cross-depth algebraic constraints produces the linear depth law. The activation is not the bottleneck. The +/-q encoding is structurally parity-locked: only XOR/XNOR are computable, AND/OR/MAJ = 0 winners at all depths.

### Journey
- D85 computed b = -5/8 (Pearce-Rasmussen indecomposability parameter) at TL_4, resolving the b = -2 vs -5/8 controversy as normalization
- D86 proved single-copy P_{0,0} extraction universally diverges -- the regular-representation multiplicity is structurally essential
- D87 discovered null indispensability is regime-dependent: dispensable at zeta_12 (infinite group)
- D88 anatomized rigid anchors (4 body-diagonal) vs flexible scaffolding (6 edge-midpoint), explaining D72's Voronoi catastrophe
- D89 tested 8 mechanistic hypotheses, killed 5, identified algebraic coherence as the remaining candidate
- D90 cracked it: axis cancellation + cross-depth constraints; spectral inversion proves mechanism is relational, not positional
- D91 confirmed balanced exponentials: vocabulary ~2x/round, demand 4x/weight -> linear slope
- D92 closed with parity-lock theorem: +/-q encoding is structurally locked to XOR/XNOR only

### Key Artifacts
- b = -5/8 indecomposability parameter (first exact first-principles computation)
- Parity-lock theorem (PROVEN): encoding determines function family
- Axis cancellation mechanism for depth law
- Balanced exponentials explanation
- Null dispensability regime transition

### What It Unlocked
- Arc 15 (Circuit Complexity) extended the function-specificity insight
- The parity-lock theorem distinguished encoding effects from activation effects

### Open Threads
- b computation does not extend to larger lattices via diagram-algebraic approach
- Transfer-matrix finite-size scaling likely needed for larger TL sizes

---

## Arc 15: Circuit Complexity (D93-D94)

**Question**: How does the classical circuit complexity hierarchy (AC^0 / TC^0 / parity) manifest in DKC?
**Answer**: AND/XOR ratio explodes from 1.01 to infinity (N=3 to 8) -- Hastad's theorem made visible. The binary icosahedral group (2I, non-solvable) outperforms solvable groups at matched size, confirming Barrington's theorem computationally. DKC maps precisely to a discrete algebraic reservoir computer.

### Journey
- D93 discovered complement-blindness theorem under combined_cell activation; phase_cell (no sign flip) recovers all 13 NPN classes; AND/XOR ratio explosion measured across N=3-8
- D94 introduced the binary icosahedral group 2I (order 120, unique non-solvable finite SU(2) subgroup) with exact Z[sqrt5] arithmetic; confirmed solvability bottleneck at matched catalog size; three-way comparison (z8 vs 2I vs z12) showed crossover at N=6
- D94 completed the five-pillar synthesis: Abramsky + Habiro + Aizenberg + Nazer-Gastpar + Reservoir Computing

### Key Artifacts
- Complement-blindness theorem; phase_cell activation
- Circuit complexity hierarchy table (AND/XOR = 1.01 to infinity)
- Solvability bottleneck confirmation (Barrington's theorem in DKC)
- Five-pillar synthesis with precise reservoir computing mapping
- Z[sqrt5] quaternion arithmetic for 2I

### What It Unlocked
- Arc 16 (Catalog Optimization) used the commutator/derived-series framework from D94's group theory
- The reservoir computing mapping became the theoretical frame for all subsequent work

### Open Threads
- RKHS kernel rank test: rank(K_2I)/rank(K_z8) > 120/24? (seeded, not yet computed)

---

## Arc 16: Catalog Optimization (D95-D97)

**Question**: What is the optimal composition of the z8 bracket catalog, and why is Cell B perfect?
**Answer**: Null/non-null and derived series are INDEPENDENT partitions creating a 5-cell landscape. Optimal z8 = 21 entries (removing 3 Q8-null improves capacity). Cell B perfection is geometric inevitability: 3 orthogonal directions at any half-angle in a 50-degree plateau.

### Journey
- D95 computed derived series of z8 (24 > 12 > 4 > 1, matching 2O > 2T > Q8 > center); confirmed 2I is perfect (every element is a commutator); found cross-layer synergy (0+0 > 0 pattern)
- D96 (LANDMARK): cross-tabulated null/non-null with derived series, producing 5-cell taxonomy (A-E); optimal z8 = A+B+C (21 entries), removing Cell D (Q8-null noise) IMPROVES capacity
- D97 explained Cell B perfection: 6 entries = regular octahedron on S^2, 3 mutually orthogonal directions, 50-degree robust plateau (25-75 degrees)

### Key Artifacts
- 5-cell catalog taxonomy (A through E)
- Optimal z8 = 21 entries (first principled pruning)
- Cell B geometric perfection thesis
- Cross-layer synergy principle: mixing algebraic strata creates capacity

### What It Unlocked
- Catalog pruning principle applicable to any DKC group
- Cell B geometry informed activation design in subsequent arcs

### Open Threads
- Does analogous pruning improve 2I capacity?
- 35-degree N=5 resonance anomaly unexplained

---

## Arc 17: Multi-strand (D98-D101)

**Question**: Does DKC work beyond 2-strand braids, and does non-semisimplicity provide computational advantage?
**Answer**: Yes -- first-ever 3-strand XOR via irreducible Delta_1 module (XOR14=60). Non-semisimple extensions are computationally load-bearing: W_{4,2} (radical) achieves XOR14=70 while W_{5,3} (simple) caps at XOR12. BFS growth rate tracks n-1 (sl_d functor thesis confirmed at d=2,3,4).

### Journey
- D98 proved 3-strand algebra is rich (infinite group, 2048 entries) but trace readout is catastrophically lossy (5.1% retention, zero XOR6). Column-4 is lossless (100% retention)
- D99 (LANDMARK): built Delta_1 (2x2 irreducible) directly; 16-component sign-hash activation; XOR14=60 from deep entries (d>=6); XOR16=0 (genuine ceiling). Jordan block proves non-split extension; Fibonacci growth of entry magnitudes; star-graph hub topology
- D100 extended to 4-strand W_{4,2} (3x3, non-semisimple): ~3.1x BFS growth, XOR14=70, 6 super-hubs with traceless commutators, constant radical content at all depths
- D101 tested 5-strand W_{5,3} (4x4, simple): ~4x growth (confirming sl_4), but only 3 hubs, XOR14=0, Casimir-XOR correlation INVERTS at high XOR levels

### Key Artifacts
- First 3-strand XOR (D99, LANDMARK)
- Ext^1 catalytic preparation thesis: non-semisimple = infinite group = deep capacity
- sl_d growth rate: ~(n-1)x per BFS round (confirmed at 3 data points)
- Two-regime hypothesis: combinatorial (XOR6-8) vs algebraic (XOR10-14)
- Traceless hub commutator universal pattern

### What It Unlocked
- Arc 18 (Multi-strand Scaling) tested the sl_d thesis at higher strand counts
- The Ext^1 thesis reframed non-semisimplicity from obstacle to resource

### Open Threads
- Hub count prediction falsified (3 vs predicted 12 for sl_4)
- Is XOR16 truly unreachable on Delta_1?

---

## Arc 18: Multi-strand Scaling/Diagnostics (D102-D105)

**Question**: How does multi-strand DKC scale with strand count, and what role does the radical play at higher dimensions?
**Answer**: BFS growth is a braid group invariant (not module dimension). The radical carries an abelian writhe character -- by Barrington's theorem, it is provably useless for parity (Barrington-Radical Principle). Higher-k regimes revive XOR at large N, refuting earlier pigeonhole claims.

### Journey
- D102 ran head-to-head A/B test of W_{6,0} (simple) vs W_{6,4} (radical dim=1): simple WINS at every XOR level, refuting radical-as-resource. Discovered writhe character: M*r = A^{writhe}*r for all 32768 entries
- D103 proved BFS growth is strand-count-invariant: W_{6,0} (dim=5) and W_{6,2} (dim=9) have identical BFS trees. Higher dimension HURTS XOR via curse of dimensionality in sign-hash
- D104 identified the Atkinson sweet spot (~120 of 324 components optimal for W_{6,2}); cross-block (branching interaction) is the computational signal; but k is the real lever, not hash architecture
- D105 tested 8-strand W_{8,0} and W_{8,2}: revived XOR8 at high k, REFUTING D93's "XOR dies at N>=7" as a k-regime artifact. Discovered macrame principle: branching interaction reverses at n=8 (within-block > cross-block). Rank saturation confound identified in all prior cross-module comparisons

### Key Artifacts
- Barrington-Radical Principle: radical carries abelian character, provably useless for parity
- Strand-count growth law: ~(n-1)x per BFS round, independent of module dimension (now 4 data points)
- k-regime theory: k/2^N ratio controls capacity (pigeonhole vs collision-avoidance)
- Macrame principle: branching interaction regime transition based on block complexity
- Sign-rank expansion: sign quantization increases effective rank

### What It Unlocked
- Arc 19 (Topological Entropy) used the relational computation thesis established here
- The k-regime discovery means prior XOR ceiling claims need re-examination at higher k

### Open Threads
- Optimal activation design for matrix-valued DKC remains open
- Does the macrame principle crossover occur between dim 5 and dim 14, or is it more nuanced?

---

## Arc 19: Topological Entropy (D106)

**Question**: Does the dynamical classification of braids (periodic vs pseudo-Anosov) predict DKC computational capability?
**Answer**: No -- topological entropy is completely orthogonal to DKC Boolean computation. Both periodic and pseudo-Anosov braids compute XOR at 100% participation with near-identical scores, because the TL quotient at delta=0 erases all dynamical information.

### Journey
- Pre-registered prediction falsified: entropy classification has zero discriminative power across ALL 8 Boolean functions tested
- Fibonacci pseudo-Anosov entries discovered at depth 2 (spectral radius phi^2, optimal entropy per crossing)
- Pseudo-Anosov fraction increases with depth (0% to 77.2%), confirming Caruso genericity theorem
- TL visibility filter identified: properties surviving the TL quotient (writhe) correlate with DKC; properties erased (entropy) do not

### Key Artifacts
- TL visibility filter principle
- Strengthened relational computation thesis: individual dynamical properties never predict capacity
- Fibonacci pseudo-Anosov classification

### What It Unlocked
- Arc 20 (Raqiya Graph Analysis) measured the relational algebraic structure that D106 showed matters

### Open Threads
- Is the TL visibility filter the complete characterization of what DKC can "see"?

---

## Arc 20: Raqiya Graph Analysis (D107-D109)

**Question**: Can algebraic graph structure predict which bracket values compute parity?
**Answer**: Yes -- the dual-channel theorem: parity capability requires BOTH multiplicative phase coherence (product closure) AND additive magnitude diversity (v2 connectivity). But the polarity is encoding-dependent: at delta=sqrt(2) with multiplicative encoding, the product closure discriminator INVERTS.

### Journey
- D107 proved universal axis-alignment of braid matrices at delta=0 as a theorem with constructive Z/4Z phase formula; discovered nesting parity = Z/2Z grading, major index bipartite invariant, and q-Catalan identity
- D108 classified 100 bracket values into parity/poison/neutral: product closure is maximum contrast discriminator (17 edges vs 0 for poison); additive v2 connectivity also discriminates (83% vs trivial)
- D109 tested at delta=sqrt(2) (Z[zeta_16]): parity and non-parity are structurally identical on 7/8 edge types; product closure is sole discriminator but with INVERTED polarity (parity = LOW closure vs D108's HIGH)

### Key Artifacts
- Z/4Z Axis-Alignment Theorem (proven)
- Nesting Parity Z/2Z Grading (novel)
- Major Index Bipartite Invariant (novel)
- q-Catalan Identity (proven)
- Dual-Channel Theorem: phase coherence + magnitude diversity, encoding-dependent polarity
- Partition coincidence as incapacity signature

### What It Unlocked
- Raqiya library (raqiya.h) created as reusable cyclotomic analysis tool
- The dual-channel theorem provides a principled vocabulary for predicting DKC capability at new evaluation points

### Open Threads
- Does the dual-channel polarity inversion follow a general rule based on encoding type?
- Can Raqiya diagnostics predict optimal activation function choice?
- j=0 liveness implies structural universality conjecture (from D109)
