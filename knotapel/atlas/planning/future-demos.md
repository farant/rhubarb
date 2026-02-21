# DKC Future Demos Backlog

Consolidated 2026-02-20. Updated 2026-02-21 with D64-D71 results and D72-D77 proposals. Updated 2026-02-21 with D72-D82 results and Arc 6 + new proposals.

70 demos completed (D01-D29, D35, D38-D39, D45-D82). D72-D82 added 2026-02-21.
13 prior proposals remain from future-demos/. 5 new demos added from synthesis open questions. 6 new proposals added from D64-D71 open questions. 7 new proposals added from D72-D82 open questions.

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
| D72 (old proposal) | Exact Cyc8 Generalized Activation | SUPERSEDED -- became different demo. Actual D72 is "Spherical Design Test" (see Arc 5 below). |
| D73 (old proposal) | XOR7 on S² | SUPERSEDED -- became different demo. Actual D73 is "Eigenvector Automaton" (see Arc 5 below). |
| D74 (old proposal) | Trivialization Threshold Sweep | SUPERSEDED -- became different demo. Actual D74 is "Computational Invariant" (see Arc 5 below). |
| D75 (old proposal) | Analytical Bandwidth Proof | SUPERSEDED -- became different demo. Actual D75 is "Binocular DKC" (see Arc 5 below). |
| D76 (old proposal) | 120-Cell / Binary Icosahedral Group | SUPERSEDED -- became different demo. Actual D76 is "Scaling 8-input" (see Arc 6 below). |
| D77 (old proposal) | Gradient Descent vs. Knot Weights | SUPERSEDED -- became different demo. Actual D77 is "Activation Zoo" (see Arc 6 below). |

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

## Arc 5: Quaternion Geometry and Invariant Theory (D72-D75)

These emerged from D64-D71 open questions and explore the geometric and algebraic structure of the ζ_8 DKC system in depth. All four are now DONE.

### D72 -- Spherical Design Test — DONE
- **Completed as demo_72_spherical_design/**. 8/8 tests pass.
- The 13 eigenvector directions achieve t=0 (worst possible spherical design) yet stochastic optimization of 13 points to minimize design residual yields only 4 XOR6 solutions vs. 36 for the algebraic eigenvector placement. Anti-Correlation Theorem: sampling quality and computational quality are anti-correlated. The computation is algebraic (binary octahedral group geometry), not geometric (uniform S² distribution). Perturbation robustness: robust to 5° (mean 35.8/36), collapses at 10° (mean 11.5/36). l=2 mode passes to machine precision despite t=0 overall (hidden quadrupole symmetry).
- See atlas/inventory/entries/demo_72.md for full results.

### D73 -- Eigenvector Automaton — DONE
- **Completed as demo_73_eigenvector_automaton/**. 7/7 tests pass.
- DKC is fundamentally additive (weight vector sum), not multiplicative (braid matrix product). Multiplicative formulation fails entirely: every cell is maximally non-separating for XOR6. Additive formulation: 82.8% universal determinism (48/58 transitions) — a structural constant identical across all 36 winning triples. Cell is not a sufficient statistic (Compass Without Odometer): the Voronoi cell captures direction but discards magnitude, and the 17% non-determinism is the missing magnitude information. 27 cells needed for 100% direction-level determinism (signed partition achieves 87.1%). DKC is additive, not multiplicative: proven theorem.
- See atlas/inventory/entries/demo_73.md for full results.

### D74 -- Computational Invariant of Braids — DONE
- **Completed as demo_74_computational_invariant/**. 20/20 tests pass.
- The DKC Voronoi cell and the Kauffman bracket are incomparable braid invariants — neither subsumes the other. Combined (bracket, cell) invariant: 119 equivalence classes vs. 100 bracket-alone and 14 cell-alone. Cell separates 97.8% of bracket collisions at braid level; bracket separates 53.8% of cell collisions. 6-Type Orbit Theorem: the DKC computation uses only 6 distinct computational roles (3 axis types, 4 body cells, 6 edge cells, 1 identity). 92% of winning triples exploit bracket-blind information in the cell.
- See atlas/inventory/entries/demo_74.md for full results.

### D75 -- Binocular DKC (Multi-Output) — DONE
- **Completed as demo_75_binocular_dkc/**. 23/23 pass.
- A single braid evaluation yields two simultaneous output channels: eigenvalue angle and eigenvector cell. Angle Subsumption: the angle perfectly predicts parity for all 36 XOR6 winners (angle subsumes cell). Seven eigenvalue angles in sum quaternions, 3 non-canonical (30°, 35.264°, 65.905°) arising from addition breaking the 24-cell group closure. Capacity: angle entropy 2.004 bits, joint 2.282 bits — 2.3× over cell-only. √n Quantization: |S|² values confined to {0,1,2,3,4,6} (√5 unreachable from 24-cell inner product structure). Multiplicative channel: 0 XOR6 solutions at all configurations — multiplication preserves group closure, which prevents computation.
- See atlas/inventory/entries/demo_75.md for full results.

---

## Arc 6: Scaling and Depth Arc (D76-D82)

These emerged naturally from the D72-D75 geometric arc and pushed the DKC system to higher input arities, discovering the activation wall, the ζ₁₂ breakthrough, and the crossing-depth law. All seven are now DONE.

### D76 -- Scaling DKC to 8 Inputs — DONE
- **Completed as demo_76_scaling_8input/**. 10/10 tests pass.
- XOR8 = 0 solutions at ζ_8 under S² Voronoi-only activation. Capacity argument: 256 masks / 14 cells ≈ 18.3 masks per cell — pigeonhole makes parity-pure cells geometrically impossible. Algebraic structure breakdown: 3-term sums give 7 discrete eigenvalue angles, 4-term sums give 86 near-continuous angles. Direction concentration: best quadruple sends 81.3% of masks to a single cell. NOTE: corrected by D77 — the wall was activation-specific, not root-of-unity-specific.
- See atlas/inventory/entries/demo_76.md for full results.

### D77 -- Activation Zoo for 8-Input DKC — DONE
- **Completed as demo_77_activation_zoo/**. 14/14 tests pass.
- XOR8 IS computable at ζ_8 with combined Sec(8) × Voronoi activation (S¹ × S², 112 cells): 6 winning quadruples, all 100% accurate. The wall in D76 was activation, not root of unity — same discovery pattern as D50. Phase transition: accuracy monotonically climbs S²Vor(50%) → Sec8(85.2%) → Polar14×6(99.2%) → Sec8×Vor(100%). Critical threshold ~100 cells. Paired quaternion structure: every XOR8 winner contains exactly one quaternion pair sharing eigenvector direction but differing in eigenvalue angle. Activation Determines Capacity: proven theorem.
- See atlas/inventory/entries/demo_77.md for full results.

### D78 -- Recursive Scaling and Phase Diagram — DONE
- **Completed as demo_78_recursive_scaling/**. 9/9 tests pass.
- XOR10 = 0 at ζ_8, confirmed exhaustively (all C(24,5)=42,504 quintuples, all k values). ζ_8 XOR hierarchy terminates at 8 inputs — finite group ceiling confirmed. Non-monotonicity: k=12 accuracy (96.9%) worse than k=10 (98.4%), echoing D50's pi/4 incommensurability. Recursive shadow structure confirmed: all 6 XOR8 winners = XOR6 triple + shadow sharing eigenvector direction but differing in half-angle (all shadows at 45°). Phase diagram: XOR6(14 cells, 36 winners) → XOR8(112 cells, 6 winners) → XOR10(WALL).
- See atlas/inventory/entries/demo_78.md for full results.

### D79 -- ζ₁₂ Capacity Test — DONE
- **Completed as demo_79_zeta12_capacity/**. 14/14 tests pass.
- ζ₁₂ breaks the ζ_8 XOR wall entirely: XOR10 goes from 0 winners (ζ_8) to 124 winners (ζ_12). XOR12 yields 50+ winners with expanded 4096-entry catalog. Fundamental cause: ζ_8 generates a finite group (24 elements, binary octahedral); ζ_12 generates an infinite/very-large group still growing past 4096 entries (roughly doubling each closure round). Non-nesting confirmed: only 3/24 ζ_8 quaternions appear in ζ_12 catalog (12%), 2/13 directions overlap (15%). N-2 capacity hypothesis invalidated: the real factor is group finiteness, not subscript N.
- See atlas/inventory/entries/demo_79.md for full results.

### D80 -- Group Finiteness Survey — DONE
- **Completed as demo_80_group_finiteness/**. 11/11 tests pass.
- Only ζ_4 and ζ_8 generate finite SU(2) subgroups under the standard two-generator construction (of 7 roots tested). Non-monotonic boundary: ζ_6 is infinite despite sitting between the two finite cases — finiteness is not monotone in N. ADE identification: ζ_4 → Binary Dihedral 2D_2 (Q_8, order 8); ζ_8 → Binary Octahedral 2O (E_7, order 48). ζ_10 is NOT binary icosahedral (generates infinite group despite golden ratio angle). Quantum Dimension Vanishing confirmed: at ζ_8, [2]_q = q + q⁻¹ = i + (-i) = 0 exactly — the two finite groups are the lattice/singular points of quantum group parameter space. Power-of-Two Finiteness Conjecture proposed: finite cases are exactly π/2^k rotations (ζ_32 not yet tested).
- See atlas/inventory/entries/demo_80.md for full results.

### D81 -- Capacity Scaling Law — DONE
- **Completed as demo_81_capacity_scaling/**. 14/14 tests pass.
- For ζ_12 (infinite group), XOR capacity scales logarithmically with catalog size: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6. Each doubling of catalog size adds ~0.62 to max XOR; each +2 XOR level costs ~3.2 doublings (~10× more entries). Transition points: XOR6 at ~5 entries, XOR8 at ~51, XOR10 at ~275, XOR12 at ~1140. Direction Bottleneck: each XOR transition gated by direction count (22 dirs for XOR8, 114 for XOR10, 507 for XOR12). After direction saturation, angular diversity alone drives continued growth. XOR14 prediction: ~38K entries (~11 rounds).
- See atlas/inventory/entries/demo_81.md for full results.

### D82 -- Crossing Depth and Computational Role — DONE
- **Completed as demo_82_crossing_depth/**. 17/17 tests pass.
- Linear Depth Law: max_xor ≈ depth + 6 (D81's log law is a corollary: catalog grows as ~2^depth, so log₂(catalog) ≈ depth). Deep entries 2× more efficient: 564 pure depth-8 entries reach XOR12 with 19 winners; mixed catalog needs ~1140 entries. Algebraic Coherence Dominance: strided-564 achieves max vocabulary (512 dirs, 43 angles) yet only reaches XOR10; deep-564 has less vocabulary (476 dirs, 19 angles) but reaches XOR12 — shared intermediate products matter more than coverage. Depth IS crossing depth: each closure round = one generator multiplication = one knot crossing. Two vocabulary regimes: depths 0–6 (direction explosion) vs depths 7–8 (angle refinement, directions saturated).
- See atlas/inventory/entries/demo_82.md for full results.

---

## Arc 7: New Proposals from D72-D82 Open Questions

### D83 -- Exact Sector Classification for Cyc8
- Rewrite sector assignment using exact Z[zeta_8] integer arithmetic (Cyc8 representation) to eliminate all floating-point atan2 calls and formally remove zero-margin boundary ambiguity from the generalized XOR6 result.
- The zero-margin universality result (all passing solutions land exactly on lattice directions) makes this tractable: no approximate sector boundaries exist, so exact arithmetic is both necessary and sufficient.
- Direct follow-up to the floating-point audit open question surfaced by D72 (l=2 passes to machine precision, quadrupole symmetry suggests algebraic identity).
- Dependencies: D65 (done), D72 (done)
- Priority: **HIGH** -- closes the last floating-point caveat in the generalized XOR6 result; would formalize the k=24 absolute-wall theorem and allow even/odd-k pattern to be tested at odd k without boundary ambiguity

### D84 -- XOR10 at ζ₁₂ with Sec×Vor Activation
- Extend D77's combined Sec(k) × Voronoi activation approach to the ζ_12 system and test whether XOR10 (which has 124 winners at ζ_12 under Voronoi-only) can be solved with a finer activation and whether XOR14 becomes reachable.
- D77 proved activation determines capacity at ζ_8; D79 proved ζ_12 breaks the ζ_8 wall under Voronoi. The natural question is whether product activation provides the same kind of unlock one level up.
- Dependencies: D77 (done), D79 (done), D81 (done)
- Priority: **HIGH** -- directly tests whether the activation-determines-capacity theorem (D77) generalizes to infinite groups; could unlock XOR14 at ζ_12 and validate the recursive architecture

### D85 -- ζ₃₂ Finiteness Test
- Test the Power-of-Two Finiteness Conjecture from D80: run a single `test_root(32)` call (θ = π/16) to determine whether ζ_32 generates a finite SU(2) subgroup under the two-generator construction.
- D80 found only ζ_4 and ζ_8 finite of 7 tested roots and proposed that the finite cases are exactly angles π/2^k (k ≥ 1). ζ_32 (k=4) is the next power-of-two case and has not been tested.
- Dependencies: D80 (done)
- Priority: **HIGH** -- single test_root(32) call; either confirms the conjecture (adding a third finite case with potentially new ADE type) or refutes it; extremely low implementation cost, high payoff

### D86 -- Direct Deep-Entry Generation
- Investigate whether algebraic properties of depth-d quaternions (shared intermediate products, angular refinement structure) can be characterized and sampled directly without computing all shallower depths via BFS closure.
- D82 showed deep entries are 2× more efficient than shallow or strided entries at matched size, and that the critical property is algebraic coherence (shared common-factor structure), not vocabulary coverage. Generating all shallower entries as scaffolding is computationally wasteful.
- Dependencies: D82 (done)
- Priority: **MEDIUM** -- would dramatically reduce the computational cost of extending the scaling law to XOR14+; requires characterizing what algebraic property "depth-d entry" encodes that can be sampled without full BFS

### D87 -- Optimal Depth Selection for Target XOR
- Determine whether pure depth-d subsets are optimal for a target XOR level or whether a mixed-depth subset (e.g., some depth-5 + depth-7 entries) achieves the same XOR level with fewer total entries.
- D82 demonstrated pure depth-8 beats shallow and strided at matched size, but did not compare pure depth-8 against pure depth-6 or depth-7 at 564 entries. The linear depth law predicts depth-6 should cap at XOR12 (depth+6=12), so depth-7 and depth-8 should both work.
- Dependencies: D82 (done)
- Priority: **MEDIUM** -- refines the practical recipe for constructing XOR-capable catalogs; pure depth may not be optimal and mixed-depth structures might offer better winner counts at smaller catalog sizes

### D88 -- Union Catalog ζ₈ + ζ₁₂
- Build a combined catalog merging the finite ζ_8 group (24 elements) with the ζ_12 group (first N rounds) and test whether the union provides superadditive XOR capacity beyond what either achieves alone.
- D79 confirmed only 3/24 ζ_8 quaternions appear in ζ_12 (12% overlap, 15% direction overlap) — the two groups are largely independent. D73 showed DKC is additive; independent algebraic structures in a union catalog might combine constructively.
- Union Catalog Conjecture from D79 remains open.
- Dependencies: D79 (done), D80 (done), D82 (done)
- Priority: **MEDIUM** -- tests whether algebraic independence of the two finite/infinite systems provides superadditive capacity; directly addresses D79's Union Catalog Conjecture

### D89 -- ζ₁₀ Depth Analysis
- Apply the linear depth law analysis from D82 to ζ_10 to determine whether max_xor ≈ depth + const holds with the same or a different constant, and whether the 0.62 scaling constant from D81 is root-of-unity-dependent.
- D80 showed ζ_10 has 71 distinct half-angles at 4096 entries — more than ζ_12 (43) or ζ_16 (51), suggesting richer angular diversity. D81 proposed the 0.62 constant may depend on angular density per closure round. ζ_10 is also the Fibonacci anyon / universal quantum computation parameter.
- Dependencies: D80 (done), D81 (done), D82 (done)
- Priority: **MEDIUM** -- determines whether the logarithmic scaling constant is universal or root-dependent; ζ_10's higher angular density may give a steeper slope; also characterizes the "computationally optimal infinite group" question from D80

---

## Priority Summary

### HIGH (do these first -- direct publication impact)
1. **D83** -- Exact Sector Classification for Cyc8. Closes floating-point caveat in D65's k=24 wall result.
2. **D84** -- XOR10 at ζ₁₂ with Sec×Vor Activation. Tests whether activation-determines-capacity generalizes to infinite groups; may unlock XOR14.
3. **D85** -- ζ₃₂ Finiteness Test. Single test_root(32) call confirms or refutes Power-of-Two Finiteness Conjecture from D80.
4. **D65 (remaining)** -- Sandwich Theorem: algebra isomorphism and nilpotency=3 still needed for Paper 3.

### MEDIUM (do next -- strengthen claims or open new connections)
5. **D86** -- Direct Deep-Entry Generation. Enables efficient extension of scaling law to XOR14+.
6. **D87** -- Optimal Depth Selection. Refines practical catalog construction recipe for target XOR levels.
7. **D88** -- Union Catalog ζ₈ + ζ₁₂. Tests Union Catalog Conjecture from D79.
8. **D89** -- ζ₁₀ Depth Analysis. Determines whether 0.62 scaling constant is universal or root-dependent.
9. **D44** -- Path Integral / Ising. Physics interpretation.
10. **D36** -- Long Braid Scaling. Practical ceiling validation.

### LOW (backlog -- valid but not urgent)
11. **D37** -- Diakoptics
12. **D40** -- Impedance Tensor
13. **D41** -- Axis Elimination
14. **D30** -- Taxonomy-to-Braid Compiler
15. **D31** -- Execution-as-Traversal
16. **D32** -- Two-Thread Coordinates
17. **D33** -- Full Pipeline
18. **D34** -- Torus Knots
19. **D43** -- Landauer Verification

### COMPLETED (D64-D82)
- ~~**D64** -- Parity Matroid Recursion.~~ **DONE** (demo_64_parity_matroid_recursion/)
- ~~**D65** -- Clifford Hierarchy Staircase.~~ **DONE** (demo_65_clifford_staircase/)
- ~~**D66** -- Quaternionic DKC — First Contact.~~ **DONE** (demo_66_quaternionic_dkc/)
- ~~**D67** -- Coordinate System Zoo.~~ **DONE** (demo_67_coordinate_zoo/)
- ~~**D68** -- Stereographic DKC.~~ **DONE** (demo_68_stereographic_dkc/)
- ~~**D69** -- Clifford Staircase on S².~~ **DONE** (demo_69_clifford_staircase/)
- ~~**D70** -- Musica Nodorum.~~ **DONE** (demo_70_musica_nodorum/)
- ~~**D71** -- Spectral DKC.~~ **DONE** (demo_71_spectral_dkc/)
- ~~**D72** -- Spherical Design Test.~~ **DONE** (demo_72_spherical_design/)
- ~~**D73** -- Eigenvector Automaton.~~ **DONE** (demo_73_eigenvector_automaton/)
- ~~**D74** -- Computational Invariant of Braids.~~ **DONE** (demo_74_computational_invariant/)
- ~~**D75** -- Binocular DKC.~~ **DONE** (demo_75_binocular_dkc/)
- ~~**D76** -- Scaling 8-Input DKC.~~ **DONE** (demo_76_scaling_8input/)
- ~~**D77** -- Activation Zoo.~~ **DONE** (demo_77_activation_zoo/)
- ~~**D78** -- Recursive Scaling.~~ **DONE** (demo_78_recursive_scaling/)
- ~~**D79** -- ζ₁₂ Capacity Test.~~ **DONE** (demo_79_zeta12_capacity/)
- ~~**D80** -- Group Finiteness Survey.~~ **DONE** (demo_80_group_finiteness/)
- ~~**D81** -- Capacity Scaling Law.~~ **DONE** (demo_81_capacity_scaling/)
- ~~**D82** -- Crossing Depth and Computational Role.~~ **DONE** (demo_82_crossing_depth/)

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
- **Quadrupole symmetry in ζ_8 directions**: D72 found l=2 mode vanishes to machine precision (|S_{2,m}|=5.11e-16) despite t=0 overall. What algebraic identity ensures Σ Y_{2,m}(dir_i)=0 for the binary octahedral eigenvector directions?
- **Perturbation non-monotonicity**: D72 found small perturbations (1°) slightly increase mean XOR6 count (38 > 36). Can the maximum XOR6 solution count for any 13-point configuration on S² be characterized? Is 36 a local or global maximum?
- **Exact 27-state automaton**: D73 found signed 27-cell partition reaches 87.1% determinism, not 100%. Is there a different 27-partition of S² that achieves 100% determinism, or does 100% require tracking magnitude information?
- **[*,18,22] dead zone in D74**: triples containing q18 and q22 (edge-type) separate zero bracket collisions. Is this a geometric accident or a group-theoretic obstruction?
- **8-family partition of winning triples (D75)**: can the partition of 36 XOR6 winners into 8 groups based on angle function be explained algebraically from 24-cell geometry?
- **Why do XOR8 winners use only canonical angles?**: D77 found winning quadruples use only 45° and 90° half-angles (canonical octahedral) while additive sums introduce non-canonical angles. Group-theoretic explanation unknown.
- **Polar 14×6 ceiling**: D77 found Polar 14×6 activation reaches 99.2% accuracy (one misclassified pair from perfection). Is 100% achievable with different magnitude binning, or is there a fundamental obstruction at 84 cells?
- **16 misclassified masks at XOR10 (D78)**: best accuracy leaves 16 masks wrong at 98.4%. Do these form a structured pattern?
- **Information-theoretic lower bound on max_N**: is there a clean formula relating the maximum computable N to catalog size, direction count, and angle count?
- **Does ζ₁₂ truly generate an infinite group or a very large finite group?**: D79 observed roughly doubling growth over 8 rounds but did not run to closure. Concrete group theory question with implications for the finite-vs-infinite capacity dichotomy.
- **E₆ and E₈ reachability**: binary tetrahedral (order 24, E₆) and binary icosahedral (order 120, E₈) are not reached by the two-perpendicular-axes generator construction. Is there a different natural generator construction that reaches them?
- **Why do perpendicular π/3 rotations not close?**: D80 identified the geometric intuition (hexagonal vs. octahedral symmetry) but a rigorous algebraic proof of why exactly π/2 and π/4 are the special finite angles has not been given.
- **Is scaling truly O(log N) or O(log N / log log N)?**: D81's decreasing transition gap series (3.35 → 2.43 → 2.05 doublings) needs at least one more data point (XOR14 transition at ~38K entries) to distinguish these hypotheses.
- **Does the 0.62 scaling constant depend on the root of unity?**: D81 conjectured the constant is a function of angular density per closure round. ζ₁₀ (71 angles at 4096 entries) vs ζ₁₂ (43 angles) is the natural comparison.
- **What algebraic property does strided-564 lack?**: D82 showed strided-564 has full vocabulary (512 dirs, 43 angles) but cannot reach XOR12 while deep-564 can. The missing property is presumably shared common-factor structure — can this be made algebraically precise?
- **Depth law for other roots**: D82 demonstrated max_xor ≈ depth + 6 for ζ_12. Does the constant 6 shift for ζ_10 or ζ_16? Comparison not yet run.
- **Knot complexity and computational power**: D82 interpreted depth as crossing depth. Does a more complexly knotted strand compute more than many simply knotted strands with equal or greater total vocabulary? Connection to known results about torus knots vs. hyperbolic knots unexplored.
