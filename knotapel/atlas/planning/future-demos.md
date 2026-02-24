# DKC Future Demos Backlog

Consolidated 2026-02-20. Updated 2026-02-21 with D64-D71 results and D72-D77 proposals. Updated 2026-02-21 with D72-D82 results and Arc 6 + new proposals. Updated 2026-02-21 with D83-D84 results, Arc 7 renumbering, and new proposals from D83-D84 open questions + explorer's log seeds. Updated 2026-02-24 with D85-D92 results, Arc 8/9 completion, new proposals from D85-D92 open questions, and vision-alignment Paths H/I/J.

82 demos completed (D01-D29, D35, D38-D39, D45-D94). D93-D94 added 2026-02-24.
13 prior proposals remain from future-demos/. 5 new demos added from synthesis open questions. 6 new proposals added from D64-D71 open questions. 7 new proposals added from D72-D82 open questions (2 now SUPERSEDED by actual D83-D84). 8 new proposals added from D83-D84 open questions and explorer's log seeds. 8 of those proposals completed as actual D85-D92; 6 new proposals added from D85-D92 open questions and vision-alignment Paths H/I/J. D93-D94 completed; 1 new proposal (D95 RKHS kernel rank) added from D93-D94 open questions.

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
| D83 (old proposal) | Exact Sector Classification for Cyc8 | SUPERSEDED -- actual D83 is "Framing as Computational Content" (see Arc 7 below). Original proposal (exact Z[zeta_8] sector arithmetic) remains valid as a future demo, see Arc 10 proposal "Exact Sector Classification." |
| D84 (old proposal) | XOR10 at ζ₁₂ with Sec×Vor Activation | SUPERSEDED -- actual D84 is "Null States and Logarithmic Structure" (see Arc 7 below). Original proposal (Sec×Vor activation at ζ₁₂) remains valid as a future demo, see Arc 10 proposal "XOR10 at ζ₁₂ with Sec×Vor." |
| D85 (Arc 8 proposal) | Indecomposability Parameter b | DONE as actual D85 (demo_85_indecomposability/). |
| D86 (Arc 8 proposal) | Dense Polymer Fusion on Lattice | SUPERSEDED -- actual D86 is "Direct b on Single P_{0,0}" (negative result, divergence). Original fusion proposal remains valid, see Arc 10 proposal "Dense Polymer Fusion." |
| D87 (Arc 8 proposal) | Framing Loss at Other Roots (ζ₁₆, ζ₂₄) | SUPERSEDED -- actual D87 is "Null Indispensability at zeta_12." Original framing-loss proposal remains valid, see Arc 10 proposal "Framing Loss at Other Roots." |
| D88 (Arc 8 proposal) | Analytical Proof of +2 Per Writhe Unit | SUPERSEDED -- actual D88 is "Anti-Correlation Mechanism." Original proof proposal remains valid, see Arc 10 proposal "Analytical Proof of +2." |
| D89 (Arc 8 proposal) | Jones-Sector Coincidence at ζ₈ | SUPERSEDED -- actual D89 is "Depth Law Mechanism." Original coincidence proposal remains valid, see Arc 10 proposal "Jones-Sector Coincidence." |
| D90 (Arc 8 proposal) | Writhe Distribution of 24-Cell Vertices | SUPERSEDED -- actual D90 is "Sum-Angle Structure." Original writhe-distribution proposal remains valid, see Arc 10 LOW backlog. |
| D91 (Arc 8 proposal) | Exact Sector Classification for Cyc8 | SUPERSEDED -- actual D91 is "Activation Bottleneck Test." Original proposal remains valid, see Arc 10 "Exact Sector Classification." |
| D92 (Arc 8 proposal) | XOR10 at ζ₁₂ with Sec×Vor Activation | SUPERSEDED -- actual D92 is "Function Scaling / Parity-Lock Theorem." Original proposal remains valid, see Arc 10 "XOR10 at ζ₁₂ with Sec×Vor." |
| D93 (Arc 8 proposal) | ζ₁₂ Null Indispensability Test | DONE -- this was essentially completed by actual D87 (Null Indispensability at zeta_12). D87 tested exactly this: removing all 121 nulls preserves full XOR6/8/10/12 capacity at zeta_12. |

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

### D39 -- Symmetry Decomposition -- DONE
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

### D63 -- Angular Anatomy of DKC -- DONE
- **Completed as demo_63_angular_anatomy/**. 29/29 tests pass.
- 4-input parity at k=8 (96 solutions, tetraskelion). 5-input parity at k=15 (3020 solutions, pentaskelion).
- **Parity ceiling for Z[zeta_8] is n=5.** Triskelion generalization (k=2n) FALSIFIED.
- Oriented matroid classification: alternating OM type required for parity. 21 distinct types.
- Two types of impossibility: constraint wall (n=6,7) vs pigeonhole (n>=8).
- Universal k theorem: k=2M-1 for M-ray lattice (conjectured, proof sketch).
- See atlas/inventory/entries/demo_63.md for full results.

### D64 -- Catalog Completeness Bound -- DONE
- **Completed as demo_64_parity_matroid_recursion/**. 22/22 tests pass.
- Parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a matroid minor chain: deletion gives XOR_{n-1}, contraction gives XNOR_{n-1} (100% closure, all levels). Funnel theorem: upward coverage collapses -- 99.3% -> 39.2% -> 19.2% -> 0% (XOR6). Vocabulary stratification: 44 good values / 31 poison values in the 83 class-1 catalog at k=15. Self-doubling impossibility theorem proven. Minimum wall distance d=4 (exhaustive). Exact Z[zeta_8] arithmetic required to eliminate floating-point boundary artifacts.
- See atlas/inventory/entries/demo_64.md for full results.

### D65 -- Sandwich Theorem Formal Proof / Clifford Staircase -- DONE
- **Completed as demo_65_clifford_staircase/**. 38/38 tests pass.
- Z[zeta_16] catalog: 36 values, 14/16 angular directions. Parity ceiling n=5 is algebra-independent (identical thresholds at Z[zeta_8] and Z[zeta_16]). Gap-of-2 hypothesis refuted: pigeonhole bound rises from 7 to 14 but ceiling stays at 5 (gap widens to 9). XOR6 absolute wall confirmed at every k=2..31 for both catalogs. Generalized binary labeling breaks the wall at k=24 (3/2700 pass; antipodal pairs mandatory). XOR7 first solution at k=91; scaling ratio XOR7/XOR6 ~3.8. Zero margin universality: all passing solutions land exactly on Z[zeta_8] lattice directions.
- NOTE: Sandwich theorem proofs (radical dimension, next-level radical, Markov RT truncation) were also previously recorded here -- those are complete. Remaining algebra isomorphism and nilpotency proofs still open (Paper 3).
- See atlas/inventory/entries/demo_65.md for full results.

### D66 -- Nazer-Gastpar / Quaternionic DKC -- First Contact -- DONE
- **Completed as demo_66_quaternionic_dkc/**. 30/30 tests pass.
- See atlas/inventory/entries/demo_66.md for full results.

### D67 -- Coordinate System Zoo -- SU(2) Projections for DKC -- DONE
- **Completed as demo_67_coordinate_zoo/**. 18/18 tests pass.
- See atlas/inventory/entries/demo_67.md for full results.

### D68 -- Stereographic DKC -- Knot Computation in the Plane -- DONE
- **Completed as demo_68_stereographic_dkc/**. 9/9 tests pass.
- See atlas/inventory/entries/demo_68.md for full results.

### D69 -- Clifford Staircase on S^2 -- Multi-Root Eigenvector Comparison -- DONE
- **Completed as demo_69_clifford_staircase/**. 9/9 tests pass.
- See atlas/inventory/entries/demo_69.md for full results.

### D70 -- Musica Nodorum -- DONE
- **Completed as demo_70_musica_nodorum/**. 8/8 tests pass.
- See atlas/inventory/entries/demo_70.md for full results.

### D71 -- Spectral DKC -- Spherical Harmonic Decomposition -- DONE
- **Completed as demo_71_spectral_dkc/**. 10/10 tests pass.
- See atlas/inventory/entries/demo_71.md for full results.

---

## Arc 5: Quaternion Geometry and Invariant Theory (D72-D75)

These emerged from D64-D71 open questions and explore the geometric and algebraic structure of the zeta_8 DKC system in depth. All four are now DONE.

### D72 -- Spherical Design Test -- DONE
- **Completed as demo_72_spherical_design/**. 8/8 tests pass.
- See atlas/inventory/entries/demo_72.md for full results.

### D73 -- Eigenvector Automaton -- DONE
- **Completed as demo_73_eigenvector_automaton/**. 7/7 tests pass.
- See atlas/inventory/entries/demo_73.md for full results.

### D74 -- Computational Invariant of Braids -- DONE
- **Completed as demo_74_computational_invariant/**. 20/20 tests pass.
- See atlas/inventory/entries/demo_74.md for full results.

### D75 -- Binocular DKC (Multi-Output) -- DONE
- **Completed as demo_75_binocular_dkc/**. 23/23 pass.
- See atlas/inventory/entries/demo_75.md for full results.

---

## Arc 6: Scaling and Depth Arc (D76-D82)

These emerged naturally from the D72-D75 geometric arc and pushed the DKC system to higher input arities, discovering the activation wall, the zeta_12 breakthrough, and the crossing-depth law. All seven are now DONE.

### D76 -- Scaling DKC to 8 Inputs -- DONE
- **Completed as demo_76_scaling_8input/**. 10/10 tests pass.
- See atlas/inventory/entries/demo_76.md for full results.

### D77 -- Activation Zoo for 8-Input DKC -- DONE
- **Completed as demo_77_activation_zoo/**. 14/14 tests pass.
- See atlas/inventory/entries/demo_77.md for full results.

### D78 -- Recursive Scaling and Phase Diagram -- DONE
- **Completed as demo_78_recursive_scaling/**. 9/9 tests pass.
- See atlas/inventory/entries/demo_78.md for full results.

### D79 -- zeta_12 Capacity Test -- DONE
- **Completed as demo_79_zeta12_capacity/**. 14/14 tests pass.
- See atlas/inventory/entries/demo_79.md for full results.

### D80 -- Group Finiteness Survey -- DONE
- **Completed as demo_80_group_finiteness/**. 11/11 tests pass.
- See atlas/inventory/entries/demo_80.md for full results.

### D81 -- Capacity Scaling Law -- DONE
- **Completed as demo_81_capacity_scaling/**. 14/14 tests pass.
- See atlas/inventory/entries/demo_81.md for full results.

### D82 -- Crossing Depth and Computational Role -- DONE
- **Completed as demo_82_crossing_depth/**. 17/17 tests pass.
- See atlas/inventory/entries/demo_82.md for full results.

---

## Arc 7: Framing, Null States, and LCFT (D83-D84)

These emerged from the scaling/depth arc and the LCFT literature study. D83 discovered that framing (writhe) is an independent computational resource worth exactly +2 XOR levels, resolving the "N-2" mystery from D79. D84 proved that bracket-null entries (Re(q)=0) are computationally indispensable despite producing zero scalar output, connecting DKC to logarithmic conformal field theory. Both are DONE.

### D83 -- Framing as Computational Content -- DONE
- **Completed as demo_83_framing_test/**. 12/12 tests pass.
- See atlas/inventory/entries/demo_83.md for full results.

### D84 -- Null States and Logarithmic Structure -- DONE
- **Completed as demo_84_null_states/**. 17/17 tests pass.
- See atlas/inventory/entries/demo_84.md for full results.

---

## Arc 8: Non-Semisimplicity and Indecomposability (D85-D86)

These directly attacked the LCFT quantitative structure: computing the indecomposability parameter b from first principles, and testing whether it could be extracted from a single indecomposable module. D85 succeeded at TL_4; D86 established a novel negative result (divergence on single P_{0,0}).

### D85 -- Indecomposability Parameter b -- DONE
- **Completed as demo_85_indecomposability/**. 58/58 tests pass.
- b = -5/8 (Pearce-Rasmussen) confirmed at TL_4 via leading-coefficient extraction from delta-parameterized Markov trace on full regular representation. Literature controversy dissolved: b = -5/2 on regular rep / dim(0-TL sector) = 4 = -5/8 per cell. Valuation condition fails at TL_6 (diverges as delta^{-2}).
- See atlas/inventory/entries/demo_85.md for full results.

### D86 -- Direct b on Single P_{0,0} -- DONE (NEGATIVE RESULT)
- **Completed as demo_86_b_direct/**. 15/15 tests pass.
- Delta-parameterized Gram matrix approach universally diverges on single projective cover P_{0,0} at every eigenvalue and lattice size. Multiplicity from regular representation is structurally essential. Novel negative finding.
- See atlas/inventory/entries/demo_86.md for full results.

---

## Arc 9: Mechanistic Arc -- Depth Law, Regime Transition, and Encoding (D87-D92)

These answered "why does DKC work and what are its structural limits?" D87 established regime-dependent null dispensability. D88 explained the anti-correlation mechanism. D89-D91 systematically identified and confirmed the depth law mechanism (axis cancellation + cross-depth algebraic constraints, balanced exponentials). D92 proved the parity-lock theorem: the +/-q encoding determines which functions are computable, more fundamental than activation or catalog.

### D87 -- Null Indispensability at zeta_12 -- DONE
- **Completed as demo_87_null_indispensability/**. 14/14 tests pass.
- Null indispensability is regime-dependent: at zeta_12 (infinite group), removing 121 nulls preserves full XOR6/8/10/12 capacity. Regime transition from zeta_8 where removing nulls destroys capacity. Direction density is the mechanism.
- See atlas/inventory/entries/demo_87.md for full results.

### D88 -- Anti-Correlation Mechanism -- DONE
- **Completed as demo_88_anti_correlation/**. 8/8 tests pass.
- 13 zeta_8 directions form cuboctahedron: 4 non-null body diagonals are rigid computational anchors; 6 null-only edge midpoints are flexible scaffolding. K-ladder provides 14x better perturbation resilience than Voronoi-only (6.4% vs 89% loss). Constrained optimization (nulls free, non-nulls clamped 2 deg) captures 92% of design improvement with 2.8% XOR loss.
- See atlas/inventory/entries/demo_88.md for full results.

### D89 -- Depth Law Mechanism -- DONE (PRELIMINARY)
- **Completed as demo_89_depth_law_mechanism/**. 14/14 tests pass.
- Tested 8 mechanistic hypotheses, killed 5 (pairwise coherence, direction coverage, Cayley density, sector diversity, angle coherence). Confirmed paired extension as real but minority (6-20% at zeta_12 vs 100% at zeta_8). Identified "algebraic coherence" as remaining open question.
- See atlas/inventory/entries/demo_89.md for full results.

### D90 -- Sum-Angle Structure -- DONE
- **Completed as demo_90_sum_angle_structure/**. 10/12 tests pass (2 informative fails).
- Depth law mechanism is axis cancellation (anti-aligned rotation axes) + cross-depth algebraic constraints (BFS ancestry restricting sum-angle vocabularies). Advantage is RELATIONAL, not positional. S^2 point cloud bandwidth DECREASES with depth (spectral inversion).
- See atlas/inventory/entries/demo_90.md for full results.

### D91 -- Activation Bottleneck Test -- DONE
- **Completed as demo_91_activation_bottleneck/**. 6/7 tests pass (1 informative fail).
- Parity wall confirmed: max_xor is independent of activation resolution (k_sec 2-48 all give XOR8). Depth gap is k_sec-invariant. Balanced exponentials confirmed: BFS ~2x/round, parity 4x/weight, slope ~1.
- See atlas/inventory/entries/demo_91.md for full results.

### D92 -- Function Scaling / Parity-Lock Theorem -- DONE
- **Completed as demo_92_function_scaling/**. 13/16 tests pass (3 informative fails confirming theorem).
- Parity-Lock Theorem PROVED: +/-q encoding structurally locked to XOR/XNOR. AND/OR/MAJ/THRESHOLD = 0 winners at ALL depths, ALL resolutions. Mechanism: masks 0...0 and 1...1 always produce identical sums (both zero). 3^k equivalence classes; XOR/XNOR are the only standard Boolean functions constant on all classes.
- See atlas/inventory/entries/demo_92.md for full results.

---

## Arc 10: New Proposals from D85-D92 Open Questions and Vision Paths H/I/J

### 1wpi Encoding Depth Law (Path H from vision-alignment)
- Test whether the 1wpi encoding (D48/D50, which computes all 13 NPN classes) exhibits a depth law analogous to max_xor ~ depth+6. Specifically: does max_and or max_maj scale linearly with depth under 1wpi encoding? If so, the 1wpi encoding is the correct choice for the hybrid LLM vision (which needs AND/OR for syllogistic logic). If not, the compilation cost model for non-parity functions is completely open.
- This is THE most architecturally consequential open question for the compilation pipeline, identified by D92's parity-lock theorem. D92 proves +/-q is parity-locked; the 1wpi encoding is the only known alternative that computes non-parity functions; its scaling behavior is unknown.
- Dependencies: D48 (done), D50 (done), D82 (done, depth law infrastructure), D92 (done, parity-lock motivation)
- Priority: **HIGH** -- if 1wpi has no depth law, the entire compilation cost model for non-parity functions must be redesigned; if it does, the path to broadening DKC's function repertoire is clear

### Hybrid Encoding Design (Path H continuation)
- Design a mixed encoding where some weights use +/-q pairing (for depth-law parity scaling) and some use 1wpi (for non-parity function breadth). Test whether a hybrid achieves a richer set of Boolean functions while maintaining some depth-law structure for the parity components.
- D92 established that encoding is more fundamental than activation or catalog. The +/-q encoding concentrates on parity (the hardest function); the 1wpi encoding handles all 13 NPN classes. A hybrid might balance function breadth with depth-law scaling.
- Dependencies: 1wpi Depth Law demo (above), D92 (done)
- Priority: **MEDIUM** -- depends on 1wpi depth law result; only meaningful if 1wpi shows different scaling characteristics than +/-q

### Balanced Exponentials Analytical Proof (Path I from vision-alignment)
- Derive analytically that the linear depth law slope ~1 follows from BFS branching factor ~2x/round vs parity demand growth 4x/weight. D91 confirmed this balance computationally across all activation resolutions. An analytical derivation connecting BFS branching factor to parity constraint growth rate would be a genuine theoretical result -- the next clean formal proof candidate after the parity-lock theorem.
- D90 provided the mechanistic explanation (axis cancellation + cross-depth constraints). D91 confirmed the wall is parity, not activation. The balanced exponentials explanation is consistent with all data but not derived from first principles.
- Dependencies: D90 (done), D91 (done)
- Priority: **HIGH** -- converts a demonstrated conjecture into a theorem; requires characterizing the BFS branching factor of the SU(2) generator construction and connecting it to the parity constraint combinatorics

### Null Dispensability Transition Location (Path J from vision-alignment)
- Locate the exact transition between null-indispensable (zeta_8) and null-dispensable (zeta_12) regimes. Test at intermediate catalog sizes: truncated zeta_12 catalogs at various depths, zeta_10 (Fibonacci anyons, infinite group). Is the control parameter a specific direction count, catalog size, null fraction, or per-direction spherical design residual?
- D87 established the regime transition exists. D88 showed per-direction residual is 8x lower at zeta_12 than zeta_8. The exact threshold is unknown. Knowing it gives a concrete design rule: below transition, preserve nulls; above, prune safely.
- Dependencies: D87 (done), D88 (done)
- Priority: **MEDIUM** -- gives a concrete design threshold for the compilation pipeline; D88's per-direction residual metric is the leading candidate for the control parameter

### b Computation at Larger Lattice Sizes
- Identify and implement the actual Pearce-Rasmussen method for extracting b at lattice sizes beyond TL_4. D85 showed the delta-parameterized Gram matrix approach fails at TL_6 (valuation condition p_tt = 2*p_Tt not satisfied). D86 showed single P_{0,0} also diverges. The literature method likely uses transfer matrix + finite-size scaling, not the diagram-algebraic approach. Implementing their exact method would confirm b = -5/8 at larger sizes.
- D85 open questions: why does the valuation condition fail at TL_6? Can b be confirmed at TL_6 or TL_8? The spin chain approach (D85 Phase 4) at n=4,6,8 should be compared.
- Dependencies: D85 (done), D86 (done)
- Priority: **MEDIUM** -- strengthens the b = -5/8 result beyond the single TL_4 confirmation; the literature method identification is a research question (may require consulting actual PR papers more carefully)

### Depth Law for Non-Parity Functions under 1wpi
- Does the linear depth law hold for ALL Boolean functions, or just parity? D91 and D92 together establish that the depth law is parity-specific under +/-q encoding. Under the 1wpi encoding (which can compute AND, OR, MAJ), does max_and or max_maj scale linearly with depth? Different functions have different constraint growth rates (MAJ has 19/27 class conflicts vs XOR's 0/27), so slopes might differ.
- D92 open question: "Can we construct encodings that target specific Boolean functions other than parity?" This demo would answer the scaling side of that question.
- Dependencies: 1wpi Depth Law demo (above), D92 (done)
- Priority: **MEDIUM** -- only meaningful after the 1wpi depth law demo; if 1wpi has a depth law for AND/MAJ, the slope comparison would reveal how function complexity maps to catalog cost

---

## Continuing Proposals (carried forward, renumbered as needed)

The following proposals from previous arcs remain valid. Some have been partially addressed by D85-D92 results, noted below.

### Framing Loss at Other Roots (zeta_16, zeta_24)
- Test whether the +2 Jones normalization loss holds at zeta_16 and zeta_24 (and potentially zeta_6, zeta_10). D83 demonstrated the loss at zeta_8 and zeta_12 despite very different group sizes (24 vs 4096), suggesting universality. More roots needed to confirm.
- Originally proposed as D87 in Arc 8; actual D87 became "Null Indispensability." Proposal unchanged.
- Dependencies: D83 (done), D54 (done, Z[zeta_16]), D59 (done, Z[zeta_24])
- Priority: **HIGH** -- if the +2 loss is universal across all roots, the Resource Decomposition Conjecture becomes a theorem; if it varies, understanding why would be equally important

### zeta_32 Finiteness Test
- Test the Power-of-Two Finiteness Conjecture from D80: run a single `test_root(32)` call (theta = pi/16) to determine whether zeta_32 generates a finite SU(2) subgroup.
- D80 found only zeta_4 and zeta_8 finite of 7 tested roots and proposed that the finite cases are exactly angles pi/2^k (k >= 1). zeta_32 (k=4) is the next power-of-two case.
- Dependencies: D80 (done)
- Priority: **HIGH** -- single test_root(32) call; extremely low implementation cost, high payoff

### Analytical Proof of +2 Per Writhe Unit
- Prove analytically that the (-A^3)^{-w} Jones phase factor costs exactly 2 XOR levels. D83 demonstrated the result computationally at two roots.
- Originally proposed as D88 in Arc 8; actual D88 became "Anti-Correlation Mechanism." Proposal unchanged.
- Dependencies: D83 (done)
- Priority: **MEDIUM** -- converts a demonstrated result into a proven theorem

### Jones-Sector Coincidence at zeta_8
- Test whether the Jones-normalized zeta_8 capacity (XOR6) is exactly the "standard 6-sector activation" capacity from D63. Would unify two independently discovered XOR6 regimes.
- Originally proposed as D89 in Arc 8; actual D89 became "Depth Law Mechanism." Proposal unchanged.
- Dependencies: D63 (done), D83 (done)
- Priority: **MEDIUM** -- bridges sector-based and quaternionic DKC frameworks

### Exact Sector Classification for Cyc8
- Rewrite sector assignment using exact Z[zeta_8] integer arithmetic (Cyc8 representation) to eliminate all floating-point atan2 calls.
- Originally proposed as D83/D91 in earlier arcs; actual D91 became "Activation Bottleneck Test."
- Dependencies: D65 (done), D72 (done)
- Priority: **MEDIUM** -- closes the last floating-point caveat in the generalized XOR6 result

### XOR10 at zeta_12 with Sec x Vor Activation
- Extend D77's combined Sec(k) x Voronoi activation to the zeta_12 system. Test whether XOR14 becomes reachable.
- Originally proposed as D84/D92 in earlier arcs; actual D92 became "Function Scaling / Parity-Lock Theorem."
- Dependencies: D77 (done), D79 (done), D81 (done)
- Priority: **MEDIUM** -- tests whether the activation-determines-capacity theorem generalizes to infinite groups

### Explicit LCFT Jordan-Cell Pairs
- Construct explicit Jordan-cell pairs from the 3 shared-direction entries (coordinate axes) where both null and non-null entries coexist. Test whether these form a literal Jordan-cell pair in some representation of the braid group algebra.
- Originally proposed as D94. Unchanged.
- Dependencies: D84 (done), D35 (done)
- Priority: **MEDIUM** -- converts D84's LCFT analogy into algebraic proof

### Dense Polymer Fusion on Lattice
- Implement lattice fusion for TL at beta=0 (dense polymer point, p=2, q=i, c=-2) and work out fusion rules explicitly. Gainutdinov et al. (2013) did this for percolation (p=3) but NOT for dense polymers (p=2).
- Originally proposed as D86 in Arc 8; actual D86 became "Direct b on Single P_{0,0}." Proposal unchanged.
- Dependencies: D35 (done), D84 (done)
- Priority: **MEDIUM** -- fills a literature gap; D86's negative result (multiplicity essential) makes the fusion approach even more interesting as an alternative algebraic route

### Direct Deep-Entry Generation
- Investigate whether algebraic properties of depth-d quaternions can be characterized and sampled directly without computing all shallower depths via BFS closure.
- D82 showed deep entries are 2x more efficient. D90 now explains WHY (axis cancellation + cross-depth algebraic constraints). This mechanistic understanding makes the generation problem more precisely stated: target entries with anti-aligned axes relative to shallower entries and constrained sum-angle vocabularies.
- Dependencies: D82 (done), D90 (done, mechanistic understanding)
- Priority: **MEDIUM** -- would dramatically reduce computational cost of extending scaling law to XOR14+; D90's mechanism provides concrete target properties

### Optimal Depth Selection for Target XOR
- Determine whether pure depth-d subsets are optimal for a target XOR level or whether a mixed-depth subset achieves the same XOR level with fewer total entries.
- D89 Phase 11 showed paired extension (which combines entries across depths) is a minority pathway at zeta_12 (6-20% of winners). This suggests mixed-depth may help but is not dominant.
- Dependencies: D82 (done), D89 (done, paired extension data)
- Priority: **MEDIUM** -- refines practical catalog construction recipe

### Union Catalog zeta_8 + zeta_12
- Build a combined catalog merging the finite zeta_8 group (24 elements) with the zeta_12 group and test for superadditive XOR capacity. Union Catalog Conjecture from D79 remains open.
- Dependencies: D79 (done), D80 (done), D82 (done)
- Priority: **MEDIUM** -- tests whether algebraic independence provides superadditive capacity

### zeta_10 Depth Analysis
- Apply the linear depth law analysis from D82 to zeta_10 to determine whether max_xor ~ depth + const holds with the same or a different constant.
- D80 showed zeta_10 has 71 distinct half-angles at 4096 entries. D89 tested the depth law mechanism only at zeta_12; zeta_10 is the Fibonacci anyon / universal quantum computation parameter.
- Dependencies: D80 (done), D81 (done), D82 (done)
- Priority: **MEDIUM** -- determines whether scaling constant is universal or root-dependent

### zeta_10 Null Fraction and Directional Role
- Measure the null entry count, null fraction, and directional role of null entries at zeta_10. Fills in the dilution curve between zeta_8 (37.5%) and zeta_12 (3.0%).
- D87 established the dispensability transition; zeta_10 sits between the two tested roots.
- Dependencies: D80 (done), D84 (done), D87 (done)
- Priority: **LOW** -- characterization at the universal quantum computation root; also helps locate the dispensability transition (see "Null Dispensability Transition Location" above)

### Writhe Distribution of 24-Cell Vertices
- Characterize the writhe distribution of the 24 entries forming the binary octahedral group. Test whether antipodal pairs have opposite writhes.
- Originally proposed as D90 in Arc 8; actual D90 became "Sum-Angle Structure."
- Dependencies: D83 (done), D74 (done)
- Priority: **LOW** -- structural characterization

### Depth-3 -> 100% Null Generalization
- Test whether the maximum-depth stratum of any finite SU(2) subgroup consists entirely of bracket-null entries.
- Dependencies: D80 (done), D84 (done)
- Priority: **LOW** -- small scope (only zeta_4 to test unless zeta_32 is finite)

### RC Exactification with Null-State Geometry
- Use the quaternionic DKC framework to design reservoir weight matrices where null-state geometry is explicitly controlled.
- D87's regime-dependent finding refines this: the RC null-state interpretation applies only in sparse-direction (finite group) regimes.
- Dependencies: D84 (done), D82 (done), D87 (done, regime refinement)
- Priority: **LOW** -- speculative but high-impact if successful

---

## Priority Summary

### HIGH (do these first -- direct publication impact or architecturally consequential)
1. **RKHS Kernel Rank Test (D95)** — NEW from D94. Compute DKC kernel K(m,m') = quaternion inner product of sums for 2I vs z8. If rank(K_2I)/rank(K_z8) > 120/24, non-solvability contributes above raw catalog size. One number confirms or refutes the "discrete algebraic reservoir computer" interpretation. Single demo, extremely high leverage — validates the five-pillar synthesis.
2. **1wpi Encoding Depth Law** (Path H). THE most consequential open question from D92: does the non-parity encoding scale? **D93 context:** D93 confirms all 13 NPN classes under phase_cell at N=3, and demonstrates circuit complexity hierarchy under 1wpi. Depth-law test under 1wpi/phase_cell is the next step.
3. **Balanced Exponentials Analytical Proof** (Path I). Next formal proof candidate after parity-lock theorem.
4. **Framing Loss at Other Roots** (zeta_16, zeta_24). Confirms or refines +2 universality from D83.
5. **zeta_32 Finiteness Test**. Single test, extremely low cost, high payoff.
6. **D65 (remaining)** -- Sandwich Theorem: algebra isomorphism and nilpotency=3 still needed for Paper 3.
7. **Higher k_sec with 2I** — NEW from D94. Does k_sec=24 push the N=8 XOR cliff further for 2I? The 384-cell space (k=12, 31 directions) may not fully exploit 2I's angular richness (9 half-angles). Quick test, high information content.

### MEDIUM (do next -- strengthen claims or open new connections)
6. **Null Dispensability Transition Location** (Path J). Locates the regime boundary for compilation design rules.
7. **Hybrid Encoding Design** (Path H continuation). Depends on 1wpi depth law result.
8. **b Computation at Larger Lattice Sizes**. Strengthens b = -5/8 beyond TL_4.
9. **Depth Law for Non-Parity Functions under 1wpi**. Depends on 1wpi depth law result.
10. **Analytical Proof of +2 Per Writhe Unit**. Converts D83 demonstration into theorem.
11. **Jones-Sector Coincidence at zeta_8**. Bridges sector-based and quaternionic frameworks.
12. **Exact Sector Classification for Cyc8**. Closes floating-point caveat.
13. **XOR10 at zeta_12 with Sec x Vor**. Tests activation generalization to infinite groups.
14. **Explicit LCFT Jordan-Cell Pairs**. Makes D84 LCFT connection algebraically precise.
15. **Dense Polymer Fusion on Lattice**. Fills literature gap; alternative algebraic route after D86's negative result.
16. **Direct Deep-Entry Generation**. Enables XOR14+; D90 provides concrete target properties.
17. **Optimal Depth Selection**. Refines catalog construction recipe.
18. **Union Catalog zeta_8 + zeta_12**. Tests Union Catalog Conjecture.
19. **zeta_10 Depth Analysis**. Tests universality of scaling constant.
20. **D44** -- Path Integral / Ising. Physics interpretation.
21. **D36** -- Long Braid Scaling. Practical ceiling validation.

### RKHS Kernel Rank Test (D95) — NEW (D94)
- Compute the DKC kernel matrix K(m,m') = quaternion inner product of signed sums for 2I (60 entries) vs z8 (24 entries). Rank of kernel matrix predicts reservoir separation capacity. If rank(K_2I)/rank(K_z8) > 120/24 = 5, then non-solvability contributes above raw catalog size — confirming the "discrete algebraic reservoir computer" interpretation from D94.
- Testable prediction from D94's reservoir computing mapping: fixed catalog = reservoir, Cayley graph = connectivity, signed sum = linear readout, activation = nonlinear output.
- Dependencies: D94 (done), D93 (done, 1wpi/phase_cell framework)
- Priority: **HIGH** -- single computation, one number, confirms or refutes an entire theoretical pillar. The five-pillar synthesis (TL-as-computation, Barrington, Nazer-Gastpar, MVN, reservoir computing) is complete in D94; this tests its central quantitative prediction.

### Higher k_sec with 2I — NEW (D94)
- At k_sec=24 instead of k_sec=12, the cell count roughly doubles (768 vs 384). Test whether N=8 XOR becomes nonzero for 2I. 2I has 9 conjugacy classes / half-angles and 31 distinct S2 directions — the angular richness may not be fully exploited at k=12.
- Dependencies: D94 (done)
- Priority: **HIGH** -- quick parameter sweep, tests whether 2I's capacity is activation-limited

### Quantum Dimension Direct Test at Fibonacci Parameter — NEW (D94)
- Test DKC capacity directly at the Fibonacci parameter q = e^{2*pi*i/5} rather than through the 2I finite group. The Fibonacci parameter yields [2]_q = phi^{-1} ~ 0.618 (non-vanishing, non-degenerate) — uniquely "maximally computational" for TQC. How does a dense catalog at this parameter compare to 2I's finite catalog?
- Dependencies: D94 (done), D80 (done, Fibonacci anyon context from zeta_10)
- Priority: **MEDIUM** -- bridges the finite 2I group and its infinite dense counterpart

### Cross-Depth 2I Analysis — NEW (D94)
- Do deeper 2I entries (BFS depth d=5-7) contribute more than shallow ones to XOR capacity? 2I's BFS depth profile is symmetric (5,8,11,12,11,8,4,1). Does the contribution profile mirror this symmetry, or are deeper entries disproportionately valuable as they are for zeta_12 (D82)?
- Dependencies: D94 (done), D82 (done, depth law context)
- Priority: **MEDIUM** -- tests whether the depth law mechanism applies to non-solvable finite groups

### Size vs Solvability Disentangling — NEW (D94)
- Phase 2 controls for size (24 vs 24) and Phase 4 shows crossover, but 2I has 60 entries vs z12's 51 in the three-way comparison. A sharper test: 2I truncated to exactly 51 entries vs z12 at 51 entries. Removes the last confound in the solvability bottleneck demonstration.
- Dependencies: D94 (done)
- Priority: **MEDIUM** -- strengthens the solvability bottleneck claim for publication

### LOW (backlog -- valid but not urgent)
22. **zeta_10 Null Fraction**. Characterization + transition location data.
23. **Writhe Distribution of 24-Cell Vertices**. Structural characterization.
24. **Depth-3 -> 100% Null Generalization**. Small scope.
25. **RC Exactification with Null-State Geometry**. Speculative.
26. **D37** -- Diakoptics
27. **D40** -- Impedance Tensor
28. **D41** -- Axis Elimination
29. **D30** -- Taxonomy-to-Braid Compiler
30. **D31** -- Execution-as-Traversal
31. **D32** -- Two-Thread Coordinates
32. **D33** -- Full Pipeline
33. **D34** -- Torus Knots
34. **D43** -- Landauer Verification

### COMPLETED (D64-D92)
- ~~**D64** -- Parity Matroid Recursion.~~ **DONE** (demo_64_parity_matroid_recursion/)
- ~~**D65** -- Clifford Hierarchy Staircase.~~ **DONE** (demo_65_clifford_staircase/)
- ~~**D66** -- Quaternionic DKC -- First Contact.~~ **DONE** (demo_66_quaternionic_dkc/)
- ~~**D67** -- Coordinate System Zoo.~~ **DONE** (demo_67_coordinate_zoo/)
- ~~**D68** -- Stereographic DKC.~~ **DONE** (demo_68_stereographic_dkc/)
- ~~**D69** -- Clifford Staircase on S^2.~~ **DONE** (demo_69_clifford_staircase/)
- ~~**D70** -- Musica Nodorum.~~ **DONE** (demo_70_musica_nodorum/)
- ~~**D71** -- Spectral DKC.~~ **DONE** (demo_71_spectral_dkc/)
- ~~**D72** -- Spherical Design Test.~~ **DONE** (demo_72_spherical_design/)
- ~~**D73** -- Eigenvector Automaton.~~ **DONE** (demo_73_eigenvector_automaton/)
- ~~**D74** -- Computational Invariant of Braids.~~ **DONE** (demo_74_computational_invariant/)
- ~~**D75** -- Binocular DKC.~~ **DONE** (demo_75_binocular_dkc/)
- ~~**D76** -- Scaling 8-Input DKC.~~ **DONE** (demo_76_scaling_8input/)
- ~~**D77** -- Activation Zoo.~~ **DONE** (demo_77_activation_zoo/)
- ~~**D78** -- Recursive Scaling.~~ **DONE** (demo_78_recursive_scaling/)
- ~~**D79** -- zeta_12 Capacity Test.~~ **DONE** (demo_79_zeta12_capacity/)
- ~~**D80** -- Group Finiteness Survey.~~ **DONE** (demo_80_group_finiteness/)
- ~~**D81** -- Capacity Scaling Law.~~ **DONE** (demo_81_capacity_scaling/)
- ~~**D82** -- Crossing Depth and Computational Role.~~ **DONE** (demo_82_crossing_depth/)
- ~~**D83** -- Framing as Computational Content.~~ **DONE** (demo_83_framing_test/)
- ~~**D84** -- Null States and Logarithmic Structure.~~ **DONE** (demo_84_null_states/)
- ~~**D85** -- Indecomposability Parameter b.~~ **DONE** (demo_85_indecomposability/)
- ~~**D86** -- Direct b on Single P_{0,0}.~~ **DONE** (demo_86_b_direct/) -- NEGATIVE RESULT
- ~~**D87** -- Null Indispensability at zeta_12.~~ **DONE** (demo_87_null_indispensability/)
- ~~**D88** -- Anti-Correlation Mechanism.~~ **DONE** (demo_88_anti_correlation/)
- ~~**D89** -- Depth Law Mechanism.~~ **DONE** (demo_89_depth_law_mechanism/) -- PRELIMINARY
- ~~**D90** -- Sum-Angle Structure.~~ **DONE** (demo_90_sum_angle_structure/)
- ~~**D91** -- Activation Bottleneck Test.~~ **DONE** (demo_91_activation_bottleneck/)
- ~~**D92** -- Function Scaling / Parity-Lock Theorem.~~ **DONE** (demo_92_function_scaling/)
- ~~**D93** -- Function Scaling — 1-Weight-Per-Input Encoding.~~ **DONE** (demo_93_function_scaling_1wpi/) -- LANDMARK: complement-blindness theorem, phase_cell recovery, circuit complexity hierarchy (AND/XOR ratio 1.01 to infinity, N=3-8), pigeonhole mechanism
- ~~**D94** -- Binary Icosahedral Group (2I, E8).~~ **DONE** (demo_94_binary_icosahedral/) -- RESULT: solvability bottleneck confirmed, circuit complexity hierarchy universal across groups, crossover hypothesis demonstrated, "discrete algebraic reservoir computer" framing proposed

---

## Open Questions Not Yet Assigned to Demos

These emerged from synthesis but don't have a clear single-demo scope yet:

- **Multi-layer DKC**: cascading bracket-valued neurons for circuit-level compilation (narrative Section 10). Needs design work before scoping.
- **Galois symmetry conjecture**: sandwich duality from Galois automorphisms of Gal(Q(zeta_8)/Q). Highly speculative; may resolve itself during remaining D65 work.
- **Abramsky depth**: full Geometry of Interaction interpretation to predict which bracket triples compute which functions. Needs significant theoretical development.
- **Costello-Francis-Gwilliam connection**: Witten=RT now proven (2026); DKC compiles QFT transition amplitudes into Boolean logic. Framing question more than a demo. NOTE: D83's framing result adds a new angle -- the "framing anomaly" that TQFT discards is exactly where DKC's +2 capacity lives.
- **OM generalization**: what OM type characterizes n-input parity for n=4,5? (D64 established 21 distinct oriented matroid types; characterization of which types are required is open.)
- **n=6 constraint wall proof**: clean algebraic proof, or inherently finite-verification? (D64 proven d=4 minimum wall distance exhaustively; triple-interaction weight-3+ obstruction not yet characterized.)
- **Hopf base vs eigenvector geometry**: why does Hopf base S^2 (292 solutions at 64 cells) beat eigenvector lat/lon (204) at high resolution? Geometric relationship between the two S^2 parameterizations is not yet understood.
- **k=38 gap**: why is k=38 the only even k>=24 with zero generalized XOR6 solutions? Is 19 special with respect to the Z[zeta_8] lattice?
- **Even/odd k pattern**: generalized XOR6 solutions exist at even k near threshold, not odd k. Provable from pi/4 lattice alignment?
- **Bloch sphere / quantum information**: the computation lives on S^2=CP^1 (the Bloch sphere) at angular mode l=6 (hexadecapole). Is there a quantum-information interpretation?
- **Funnel rate formula**: upward coverage fractions 99.3% -> 39.2% -> 19.2% -> 0% -- algebraic or exponential decay as a function of n? (D64 open conjecture.)
- **Poison octant 4**: D64 showed 31 poison values span octants {1,2,3,5,6,7} but never octant 4. Geometric explanation unknown.
- **Quadrupole symmetry in zeta_8 directions**: D72 found l=2 mode vanishes to machine precision (|S_{2,m}|=5.11e-16) despite t=0 overall. What algebraic identity ensures sum Y_{2,m}(dir_i)=0 for the binary octahedral eigenvector directions?
- **Perturbation non-monotonicity**: D72 found small perturbations (1 deg) slightly increase mean XOR6 count (38 > 36). Can the maximum XOR6 solution count for any 13-point configuration on S^2 be characterized? Is 36 a local or global maximum?
- **Exact 27-state automaton**: D73 found signed 27-cell partition reaches 87.1% determinism, not 100%. Is there a different 27-partition of S^2 that achieves 100% determinism, or does 100% require tracking magnitude information?
- **[*,18,22] dead zone in D74**: triples containing q18 and q22 (edge-type) separate zero bracket collisions. Is this a geometric accident or a group-theoretic obstruction?
- **8-family partition of winning triples (D75)**: can the partition of 36 XOR6 winners into 8 groups based on angle function be explained algebraically from 24-cell geometry?
- **Why do XOR8 winners use only canonical angles?**: D77 found winning quadruples use only 45 deg and 90 deg half-angles (canonical octahedral) while additive sums introduce non-canonical angles. Group-theoretic explanation unknown.
- **Polar 14x6 ceiling**: D77 found Polar 14x6 activation reaches 99.2% accuracy (one misclassified pair from perfection). Is 100% achievable with different magnitude binning, or is there a fundamental obstruction at 84 cells?
- **16 misclassified masks at XOR10 (D78)**: best accuracy leaves 16 masks wrong at 98.4%. Do these form a structured pattern?
- **Information-theoretic lower bound on max_N**: is there a clean formula relating the maximum computable N to catalog size, direction count, and angle count?
- **Does zeta_12 truly generate an infinite group or a very large finite group?**: D79 observed roughly doubling growth over 8 rounds but did not run to closure. Concrete group theory question with implications for the finite-vs-infinite capacity dichotomy.
- **E_6 and E_8 reachability**: binary tetrahedral (order 24, E_6) and binary icosahedral (order 120, E_8) are not reached by the two-perpendicular-axes generator construction. Is there a different natural generator construction that reaches them?
- **Why do perpendicular pi/3 rotations not close?**: D80 identified the geometric intuition (hexagonal vs. octahedral symmetry) but a rigorous algebraic proof of why exactly pi/2 and pi/4 are the special finite angles has not been given.
- **Is scaling truly O(log N) or O(log N / log log N)?**: D81's decreasing transition gap series (3.35 -> 2.43 -> 2.05 doublings) needs at least one more data point (XOR14 transition at ~38K entries) to distinguish these hypotheses.
- **Does the 0.62 scaling constant depend on the root of unity?**: D81 conjectured the constant is a function of angular density per closure round. zeta_10 (71 angles at 4096 entries) vs zeta_12 (43 angles) is the natural comparison.
- **What algebraic property does strided-564 lack?**: D82 showed strided-564 has full vocabulary (512 dirs, 43 angles) but cannot reach XOR12 while deep-564 can. D90 answered this: the missing property is relational (axis anti-alignment, cross-depth algebraic constraints), not positional. But formalizing this algebraically remains open.
- **Depth law for other roots**: D82 demonstrated max_xor ~ depth + 6 for zeta_12. Does the constant 6 shift for zeta_10 or zeta_16? Comparison not yet run.
- **Knot complexity and computational power**: D82 interpreted depth as crossing depth. Does a more complexly knotted strand compute more than many simply knotted strands with equal or greater total vocabulary? Connection to known results about torus knots vs. hyperbolic knots unexplored.
- **Three-level discarding pattern** (from D83-D84): (1) Jones normalizes away writhe -> loses 2 XOR levels. (2) Bracket trace projects onto Re(q) -> loses 6/13 directions. (3) LCFT null states treated as trivial -> loses logarithmic partners. Each level of "simplification" in the literature discards computational content. Is there a fourth level?
- **Does writhe add exactly +2 regardless of depth?** (from D83): D83 shows framing=+2 at the group's maximum capacity. At intermediate depths, does bracket also beat Jones by exactly 2, or does the gap vary with depth?
- **Null entries and knot complexity** (from D84): bracket-null entries are maximally deep (depth-3 in zeta_8). Do depth-d null entries correspond to specific knot types whose Jones polynomial vanishes at delta=0?
- **Non-semisimplicity thesis** (from explorer's log): semisimple = Markov chain (memoryless), non-semisimple = counter machine (nilpotent Jordan blocks accumulate depth history). The radical provides BOTH null entries AND the memory mechanism. The slope in the capacity formula comes from Loewy length / radical depth. Needs formalization.
- **Depth-level framing gap measurement** (from D83): at each individual depth level d, what is the bracket-vs-Jones capacity gap? If it is always exactly 2, the resource decomposition is truly additive. If it varies, depth and framing interact.
- **Why does the valuation condition fail at TL_6?** (NEW from D85): p_tt = 4, p_Tt = 3 (gap = -2) at TL_6. Cross-copy interference from 25-fold multiplicity does not rescue the balance. Is there a modified extraction that works for larger sizes?
- **TL_8 P_{0,0} is completely semisimple** (NEW from D86): no Jordan blocks at lambda=0 on the dim-42 ideal. Is there a periodicity or parity pattern in the non-semisimple structure across TL sizes?
- **Are the 67 null-only directions at zeta_12 ever needed?** (NEW from D87): at XOR14/XOR16, could null-only directions become essential again? Current search stops at XOR12.
- **zeta_12 load-bearing directions** (NEW from D88): at zeta_12 with 2043 directions, is there ANY subset that is computationally load-bearing (analogous to the 4 body-diagonal anchors at zeta_8)?
- **Constrained optimization as SDP** (NEW from D88): can the constrained optimization approach (flexible nulls + rigid non-nulls) be formulated as a semidefinite program rather than simulated annealing?
- **Golden ratio eigenvalue pairs** (NEW from D88): phi and -phi+1 in the Gram spectrum -- do these appear at other roots, or are they specific to zeta_8's octahedral geometry?
- **Can axis cancellation be exploited for weight selection?** (NEW from D90): if the mechanism is anti-aligned axis pairs, can we directly select weight tuples by maximizing anti-alignment rather than brute-force XOR testing?
- **Relational activation** (NEW from D90): could an activation function that explicitly uses axis dot products between entries break the linear depth law? Current activations treat entries independently.
- **Why slope = 1 analytically?** (NEW from D90/D91): the balanced-exponentials argument is consistent with data but not derived from first principles. See "Balanced Exponentials Analytical Proof" proposal above.
- **Does the depth law depend on BFS catalog construction?** (NEW from D91): alternative constructions (random quaternion sampling, algebraically structured non-BFS) might have different growth rates, yielding different depth law slopes.
- **Encoding optimality** (NEW from D92): is the +/-q encoding provably optimal for parity among all 2-bit-per-weight encodings?
- **Phase-sensitive activation** (NEW from D92): the sign-flip symmetry (combined_cell(S) = combined_cell(-S)) is a second parity-lock mechanism. Can a phase-sensitive activation that distinguishes S from -S break the parity lock while preserving depth-law structure?
- **MAJ conflict count (19/27)** (NEW from D92): MAJ has the most class conflicts of any tested function. Is this related to MAJ's computational complexity (MAJ is TC^0-complete)?
