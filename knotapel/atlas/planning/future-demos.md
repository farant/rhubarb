# DKC Future Demos Backlog

Consolidated 2026-02-20. Updated 2026-02-21 with D64-D71 results and D72-D77 proposals.

59 demos completed (D01-D29, D35, D38-D39, D45-D71). Updated 2026-02-21.
13 prior proposals remain from future-demos/. 5 new demos added from synthesis open questions. 6 new proposals added from D64-D71 open questions.

---

## Obsolete Proposals (completed or superseded)

| # | Title | Status |
|---|-------|--------|
| 35 | TL Matrix Representations | DONE (demo_35_tl_matrices/) |
| 38 | Dimension Reduction at Delta=0 | DONE (demo_38_dimension_reduction/) |
| 42 | Cyclotomic Exact Arithmetic | SUPERSEDED -- Z[zeta_8] exact arithmetic built in D29; Z[zeta_16]/Z[zeta_5]/Z[zeta_24] built in D54/D58/D59. The ring tower exists. |
| 46 | Per-Neuron Gauge Correction | DONE (demo_46_per_neuron_gauge/) |
| 47 | Parity Investigation | DONE (demo_47_parity_investigation/) |
| 57 | Additive Encoding DKC | DONE (demo_57_additive_dkc/) |
| 58 | Fibonacci Anyon DKC | DONE (demo_58_fibonacci_dkc/) |
| 59 | Q=3 Potts DKC | DONE (demo_59_q3_potts_dkc/) |
| 60 | Cubic Wall ell=7 | DONE (demo_60_cubic_wall/) |
| 61 | Linearity Classification | DONE (demo_61_angular_proof/) |
| 62 | Radical CSS Code | DONE (demo_62_analytical_proof/) |
| 39 | Symmetry Decomposition | DONE (demo_39_symmetry_decomposition/) |

---

## Arc 1: TL Matrix Engineering (Track C remnants)

Demo 35 is complete, unblocking this entire chain. These extend the TL matrix method toward practical braid computation at scale.

### D36 -- Long Braid Scaling
- O(k) matrix pipeline for 10,000+ crossing braids; repeated squaring for sigma^k
- Dependencies: D35 (done)
- Priority: **MEDIUM** -- engineering demo, validates practical ceiling
- Addresses: demonstrates DKC at circuit scale (publication credibility)

### D37 -- Diakoptics (Parallel Braid Computation)
- Kron tearing: cut braid into sections, compute in parallel, compose
- Dependencies: D35 (done), D36
- Priority: **LOW** -- performance optimization, not research frontier

### D39 -- Symmetry Decomposition — DONE
- **Completed as demo_39_symmetry_decomposition/**. 18/18 tests pass.
- Verified cellular sector sizes = d(n,j)^2, proved cell filtration, showed semisimple algebras NOT block-diagonal in standard basis, mapped radical dims at n=ell boundary for ell=2..7.
- See atlas/inventory/entries/demo_39.md for full results.

### D40 -- Impedance Tensor
- Transfer matrix as Kron impedance; closure = C^T Z C; eigenvalue spectra
- Dependencies: D35 (done), D38 (done)
- Priority: **LOW** -- Kron analogy is interesting but not on the publication path

### D41 -- Axis Elimination (Schur Complement)
- Eliminate internal strands to reduce wide braids to 2D effective description
- Dependencies: D35 (done), D38 (done), D40
- Priority: **LOW** -- blocked behind D40, engineering benefit only

---

## Arc 2: Foundation / Pipeline (Tracks A and B)

Original proposals from the pre-DKC era. Still valid as pedagogical and infrastructure demos but no longer on the critical research path.

### D30 -- Taxonomy-to-Braid Compiler
- Convert taxonomy (is-a edges) into braid words via linear logic implications
- Dependencies: D01, D03 (done)
- Priority: **LOW** -- interesting for the Abramsky connection (TL as computational calculus) but tangential to current research arc

### D31 -- Execution-as-Traversal
- Run entity state vectors through braid matrix product for taxonomy classification
- Dependencies: D01-D04 (done)
- Priority: **LOW** -- same as D30; the DKC arc has moved far past this

### D32 -- Two-Thread Coordinate System
- Milnor-style frozen coordinate interpolation, 2 threads = 4D coordinates
- Dependencies: none
- Priority: **LOW** -- standalone geometric exploration

### D33 -- Full Pipeline (PD -> Braid -> Spin Chain -> Bracket)
- End-to-end composition of D01+D05+D08 into single pipeline
- Dependencies: D01, D05, D08 (done)
- Priority: **LOW** -- mostly engineering/cleanup; all pieces exist individually

### D34 -- Torus Knots and Connected Sums
- Extend knot table (T(2,5), T(3,4), T(3,5)), test bracket product formula for connected sums
- Dependencies: D01, D05, D07, D08 (done)
- Priority: **LOW** -- nice verification but not advancing the research

---

## Arc 3: Physics Connections (Track D remnants)

### D43 -- Landauer Verification
- Fully reversible Toffoli from Fredkin primitives; measure Landauer tax of current approach
- Dependencies: D21 (done)
- Priority: **LOW** -- interesting physics question but separate from DKC core

### D44 -- Path Integral / Ising Connection
- Bracket = Ising partition function at criticality; verify via Tait graph and transfer matrix
- Dependencies: D18, D20 (done)
- Priority: **MEDIUM** -- connects DKC to statistical mechanics; strengthens the Costello-Francis-Gwilliam (2026) link mentioned in narrative Section 10
- Addresses: physical interpretation open question (narrative "what is next")

---

## Arc 4: Publication Strengthening (NEW -- from synthesis)

These directly address the strengthening actions and open questions identified in novelty.md Section 5 and the narrative frontier.

### D63 -- Angular Anatomy of DKC — DONE
- **Completed as demo_63_angular_anatomy/**. 29/29 tests pass.
- 4-input parity at k=8 (96 solutions, tetraskelion). 5-input parity at k=15 (3020 solutions, pentaskelion).
- **Parity ceiling for Z[zeta_8] is n=5.** Triskelion generalization (k=2n) FALSIFIED.
- Oriented matroid classification: alternating OM type required for parity. 21 distinct types.
- Two types of impossibility: constraint wall (n=6,7) vs pigeonhole (n>=8).
- Universal k theorem: k=2M-1 for M-ray lattice (conjectured, proof sketch).
- See atlas/inventory/entries/demo_63.md for full results.

### D64 -- Catalog Completeness Bound — DONE
- **Completed as demo_64_parity_matroid_recursion/**. 22/22 tests pass.
- Parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a matroid minor chain: deletion gives XOR_{n-1}, contraction gives XNOR_{n-1} (100% closure, all levels). Funnel theorem: upward coverage collapses — 99.3% → 39.2% → 19.2% → 0% (XOR6). Vocabulary stratification: 44 good values / 31 poison values in the 83 class-1 catalog at k=15. Self-doubling impossibility theorem proven. Minimum wall distance d=4 (exhaustive). Exact Z[zeta_8] arithmetic required to eliminate floating-point boundary artifacts.
- See atlas/inventory/entries/demo_64.md for full results.

### D65 -- Sandwich Theorem Formal Proof / Clifford Staircase — DONE
- **Completed as demo_65_clifford_staircase/**. 38/38 tests pass.
- Z[zeta_16] catalog: 36 values, 14/16 angular directions. Parity ceiling n=5 is algebra-independent (identical thresholds at Z[zeta_8] and Z[zeta_16]). Gap-of-2 hypothesis refuted: pigeonhole bound rises from 7 to 14 but ceiling stays at 5 (gap widens to 9). XOR6 absolute wall confirmed at every k=2..31 for both catalogs. Generalized binary labeling breaks the wall at k=24 (3/2700 pass; antipodal pairs mandatory). XOR7 first solution at k=91; scaling ratio XOR7/XOR6 ~3.8. Zero margin universality: all passing solutions land exactly on Z[zeta_8] lattice directions.
- NOTE: Sandwich theorem proofs (radical dimension, next-level radical, Markov RT truncation) were also previously recorded here — those are complete. Remaining algebra isomorphism and nilpotency proofs still open (Paper 3).
- See atlas/inventory/entries/demo_65.md for full results.

### D66 -- Nazer-Gastpar / Quaternionic DKC — First Contact — DONE
- **Completed as demo_66_quaternionic_dkc/**. 30/30 tests pass.
- SU(2) braid representation produces exactly the 24-cell (binary octahedral group, 24 vertices). Bracket and quaternion are complementary hash functions: 12.86% agree on both, 80.03% differ on both. Antipodal pairs mandatory for XOR6. 24-cell Voronoi (25 cells) yields 35 solutions — more than 64-cell geographic grid (34). Universal 7/7/11 structure: every winning tuple activates exactly 7 zero / 7 one / 11 unused cells. F4 symmetry (576 elements) decomposes solutions into 6 orbits in two families: edge-dominant (22) and body-dominant (13). Complex S¹ minimum k=24 confirmed as projection of 24-cell vertex count.
- See atlas/inventory/entries/demo_66.md for full results.

### D67 -- Coordinate System Zoo — SU(2) Projections for DKC — DONE
- **Completed as demo_67_coordinate_zoo/**. 18/18 tests pass.
- 24 binary octahedral quaternions decompose into 4 eigenvalue angles and 13 eigenvector directions. S² is the computational sweet spot: custom 13-direction Voronoi achieves 36 solutions at only 14 cells. Hopf phase is completely computationally inert (zero solutions at all resolutions k=4..32). Platonic solid Voronois (octahedral, icosahedral, cuboctahedral) all yield zero solutions. Eigenvalue k-sector activation: first solutions at k=8. Progression: S¹ (25 cells) → S³ (25 cells) → S² (14 cells) → R² (visualization only). Computational hierarchy: S² primary, eigenvalue angle secondary, Hopf phase inert.
- See atlas/inventory/entries/demo_67.md for full results.

### D68 -- Stereographic DKC — Knot Computation in the Plane — DONE
- **Completed as demo_68_stereographic_dkc/**. 9/9 tests pass.
- Stereographic projection of eigenvector S² to R² preserves all 36 solutions exactly when using S² great-circle metric, but gives zero solutions with Euclidean distance — computation is intrinsically curved. Conformality sufficiency: 16.5× area distortion does not affect solution count when the S² metric is used. Perceptron formulation: 6 bits → 2 neurons (stereographic weight coordinates) → Voronoi activation (S² metric) → 1 bit, derived from knot theory without training. ASCII visualization: decision boundary is a visible curve in the plane. Planar grids require 64 cells for 33 solutions vs 14 cells for 36 solutions with S² Voronoi.
- See atlas/inventory/entries/demo_68.md for full results.

### D69 -- Clifford Staircase on S² — Multi-Root Eigenvector Comparison — DONE
- **Completed as demo_69_clifford_staircase/**. 9/9 tests pass.
- Direction Nesting Theorem proven: all ζ_N eigenvector directions appear exactly in ζ_{2N} (algebraic proof from σ_i(2θ)=σ_i(θ)²; confirmed computationally ζ_8→ζ_16 at 13/13, 0.000° drift). Catalog explosion: ζ_8 = 24 quaternions / 13 directions; ζ_16 = 7,952 quaternions / 3,457 directions; ζ_32 = 32,768+ quaternions / 4,096+ directions. XOR6 trivialization at ζ_16: 90.6% of all triples are solutions (pigeonhole saturation, not topology). ζ_8 is the computational sweet spot: coarsest root with structured directions. Fixed-resolution comparison: ζ_16 > ζ_32 at 16 cells.
- See atlas/inventory/entries/demo_69.md for full results.

### D70 -- Musica Nodorum — DONE
- **Completed as demo_70_musica_nodorum/**. 8/8 tests pass.
- The ζ_8 binary octahedral group produces exactly 4 musical notes: C (0°, unison), Eb (45°, minor 3rd), E (60°, major 3rd), F# (90°, tritone). Yang-Baxter equivalent braids produce identical melodies step-by-step (melody depends only on partial-product lengths, not generator choices). Music (eigenvalue/pitch) and space (eigenvector/direction) are orthogonal: Pearson r = −0.0624. Tritone-antipodal correspondence on dodecahedron: dissonance IS the antipodal map. Garside element (s1 s2)³ melody: Eb E F# E Eb C. Platonic solids with wrong symmetry (icosahedral, dodecahedral) yield zero XOR6 solutions; only data-intrinsic 13-direction Voronoi gives 36.
- See atlas/inventory/entries/demo_70.md for full results.

### D71 -- Spectral DKC — Spherical Harmonic Decomposition — DONE
- **Completed as demo_71_spectral_dkc/**. 10/10 tests pass.
- Sharp phase transition at bandwidth l=6: 0% winner recovery at l≤5, 100% at l=6. Explained by DOF argument: 2×6+1=13 = number of eigenvector directions (the 13=13 Theorem, proven). Mode l=6 dominates all 36 winners at ~80% power — spectral universality (structural invariant of XOR6 solutions). Antipodal symmetry forces all odd modes to zero. Super-Nyquist compression: 14 cells suffice where Nyquist predicts 49 (3.5× compression via compressed sensing). Trivialization prediction: ζ_16 with 3,457 directions predicts l≥1728 minimum bandwidth, explaining Demo 69's ~90% solution rate. Musical connection: computation lives at l=6 (200 cents, note D), above the eigenvalue-resonant modes.
- See atlas/inventory/entries/demo_71.md for full results.

---

## Arc 5: New Proposals from D64-D71 Open Questions

### D72 -- Exact Cyc8 Generalized Activation
- Rewrite Demo 65's generalized XOR6 check using exact Z[zeta_8] integer arithmetic (Cyc8 representation) to eliminate all floating-point atan2 calls and formally remove zero-margin boundary ambiguity.
- The zero-margin universality result (all passing solutions land exactly on lattice directions) makes this tractable: no approximate sector boundaries exist, so exact arithmetic is both necessary and sufficient.
- Dependencies: D65 (done)
- Priority: **HIGH** -- closes the last floating-point caveat in the generalized XOR6 result; would formalize the k=24 absolute-wall theorem and allow the even/odd-k pattern to be tested at odd k without boundary ambiguity

### D73 -- XOR7 on S²
- Test the spectral framework from Demo 71 for 7-input parity: build the eigenvector catalog at the required depth, find the minimum S² bandwidth, and compare with Demo 65's S¹ result (k=127 minimum).
- Demo 67 showed S² reduces XOR6 from 25 cells to 14; the DOF formula predicts the S² bandwidth for XOR7 from the number of eigenvector directions in the extended catalog.
- Dependencies: D65 (done), D67 (done), D71 (done)
- Priority: **MEDIUM** -- tests the DOF bandwidth formula at one higher arity; validates or refutes the spectral universality conjecture for XOR7

### D74 -- Trivialization Threshold Sweep
- Systematic sweep from 14 to 3,458 Voronoi cells to precisely locate where XOR6 transitions from topology-driven (36 solutions at 14 cells) to pigeonhole-dominated (~90% at 3,457+ cells).
- Demo 69 brackets the threshold at 50–500 cells but does not locate it. Non-monotonicity (analogous to Demo 50's k=6 result) is possible.
- Dependencies: D69 (done), D71 (done)
- Priority: **MEDIUM** -- locates the computational-geometric boundary precisely; may reveal a non-monotonic peak analogous to the four-tier NPN result

### D75 -- Analytical Bandwidth Proof
- Attempt a formal proof that the minimum S² bandwidth for any binary Voronoi partition separating N points on S² in general position is l=⌈(N-1)/2⌉.
- Demo 71's 13=13 Theorem is computationally verified but not analytically proven. The conjecture would make it a theorem about arbitrary point configurations, not just the 13-direction ζ_8 catalog.
- Dependencies: D71 (done)
- Priority: **MEDIUM** -- would convert the 13=13 Theorem from a verified computational fact to an analytic result; likely requires harmonic analysis on S² (spherical t-designs or polynomial approximation theory)

### D76 -- 120-Cell / Binary Icosahedral Group
- Test the third exceptional binary polyhedral group (binary icosahedral group, order 120) for DKC: build the SU(2) generators, enumerate the braid quaternion image, and check whether it produces the vertices of the 120-cell and whether XOR6-or-higher becomes solvable.
- Demo 66 covered the binary octahedral group (24-cell). The binary tetrahedral group (16-cell) and binary icosahedral group (600-cell) are the two remaining exceptional cases. Demo 70 showed icosahedral Voronoi gives zero XOR6 solutions, but that tested the wrong object (icosahedron, not the braid image of the binary icosahedral group).
- Dependencies: D66 (done)
- Priority: **LOW** -- extends the exceptional-group survey; may reveal whether the 24-cell is uniquely suited to DKC or whether the 120-cell analogue also computes something interesting

### D77 -- Gradient Descent vs. Knot Weights
- Attempt standard gradient descent training on the XOR6 two-neuron perceptron architecture from Demo 68 (6 inputs → 2 hidden neurons with Voronoi S² activation → 1 output bit) to determine whether training can rediscover the stereographic-projection weight matrices derived from knot theory.
- If gradient descent converges to the DKC weights, this suggests the knot-theoretic solution is a natural attractor in the loss landscape. If it converges elsewhere, the knot weights are a non-gradient-discoverable structure.
- Dependencies: D68 (done)
- Priority: **LOW** -- connects DKC to standard ML training; answers whether the knot-derived weights are "findable" by conventional optimization or require algebraic construction

---

## Priority Summary

### HIGH (do these first -- direct publication impact)
1. **D72** -- Exact Cyc8 Generalized Activation. Closes floating-point caveat in D65's k=24 wall result.
2. **D65 (remaining)** -- Sandwich Theorem: algebra isomorphism and nilpotency=3 still needed for Paper 3.

### MEDIUM (do next -- strengthen claims or open new connections)
3. **D73** -- XOR7 on S². Tests spectral DOF formula at higher arity.
4. **D74** -- Trivialization Threshold Sweep. Locates the topology→pigeonhole boundary.
5. **D75** -- Analytical Bandwidth Proof. Converts 13=13 Theorem from computational to analytic result.
6. **D44** -- Path Integral / Ising. Physics interpretation.
7. **D36** -- Long Braid Scaling. Practical ceiling validation.

### LOW (backlog -- valid but not urgent)
8. **D76** -- 120-Cell / Binary Icosahedral Group. Third exceptional group survey.
9. **D77** -- Gradient Descent vs. Knot Weights. Connects DKC to standard ML training.
10. **D37** -- Diakoptics
11. **D40** -- Impedance Tensor
12. **D41** -- Axis Elimination
13. **D30** -- Taxonomy-to-Braid Compiler
14. **D31** -- Execution-as-Traversal
15. **D32** -- Two-Thread Coordinates
16. **D33** -- Full Pipeline
17. **D34** -- Torus Knots
18. **D43** -- Landauer Verification

### COMPLETED (D64-D71)
- ~~**D64** -- Parity Matroid Recursion.~~ **DONE** (demo_64_parity_matroid_recursion/)
- ~~**D65** -- Clifford Hierarchy Staircase.~~ **DONE** (demo_65_clifford_staircase/)
- ~~**D66** -- Quaternionic DKC — First Contact.~~ **DONE** (demo_66_quaternionic_dkc/)
- ~~**D67** -- Coordinate System Zoo.~~ **DONE** (demo_67_coordinate_zoo/)
- ~~**D68** -- Stereographic DKC.~~ **DONE** (demo_68_stereographic_dkc/)
- ~~**D69** -- Clifford Staircase on S².~~ **DONE** (demo_69_clifford_staircase/)
- ~~**D70** -- Musica Nodorum.~~ **DONE** (demo_70_musica_nodorum/)
- ~~**D71** -- Spectral DKC.~~ **DONE** (demo_71_spectral_dkc/)

---

## Open Questions Not Yet Assigned to Demos

These emerged from synthesis but don't have a clear single-demo scope yet:

- **Multi-layer DKC**: cascading bracket-valued neurons for circuit-level compilation (narrative Section 10). Needs design work before scoping.
- **Galois symmetry conjecture**: sandwich duality from Galois automorphisms of Gal(Q(zeta_8)/Q). Highly speculative; may resolve itself during remaining D65 work.
- **Abramsky depth**: full Geometry of Interaction interpretation to predict which bracket triples compute which functions. Needs significant theoretical development.
- **Costello-Francis-Gwilliam connection**: Witten=RT now proven (2026); DKC compiles QFT transition amplitudes into Boolean logic. Framing question more than a demo.
- **OM generalization**: what OM type characterizes n-input parity for n=4,5? (D64 established 21 distinct oriented matroid types; characterization of which types are required is open.)
- **n=6 constraint wall proof**: clean algebraic proof, or inherently finite-verification? (D64 proven d=4 minimum wall distance exhaustively; triple-interaction weight-3+ obstruction not yet characterized.)
- **Hopf base vs eigenvector geometry**: why does Hopf base S² (292 solutions at 64 cells) beat eigenvector lat/lon (204) at high resolution? Geometric relationship between the two S² parameterizations is not yet understood.
- **k=38 gap**: why is k=38 the only even k≥24 with zero generalized XOR6 solutions? Is 19 special with respect to the Z[zeta_8] lattice?
- **Even/odd k pattern**: generalized XOR6 solutions exist at even k near threshold, not odd k. Provable from pi/4 lattice alignment?
- **Bloch sphere / quantum information**: the computation lives on S²=CP¹ (the Bloch sphere) at angular mode l=6 (hexadecapole). Is there a quantum-information interpretation?
- **Funnel rate formula**: upward coverage fractions 99.3% → 39.2% → 19.2% → 0% — algebraic or exponential decay as a function of n? (D64 open conjecture.)
- **Poison octant 4**: D64 showed 31 poison values span octants {1,2,3,5,6,7} but never octant 4. Geometric explanation unknown.
