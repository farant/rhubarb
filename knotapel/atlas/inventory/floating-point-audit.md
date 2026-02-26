# Floating-Point Usage Audit

Last updated: 2026-02-26 (97 demos, D01-D109)

## Purpose

DKC's core thesis is exact arithmetic in Z[zeta_8] — zero floating-point error. This audit tracks where floating point actually appears across the codebase, why, and whether it could be eliminated. It serves both as an honesty check on the "exact" claim and as a roadmap for future exactification work.

## Categories

| Category | Meaning |
|----------|---------|
| **INTEGER** | No floating point at all. Pure integer/combinatorial computation. |
| **DISPLAY-ONLY** | FP only for printf formatting (e.g., printing ratios). Core computation is exact. |
| **VALIDATION** | FP used to cross-check exact results (e.g., cyc8_to_cx < 1e-10). Core is exact; floats verify. |
| **EXACT-CORE** | Primary computation IS floating point. Results depend on FP arithmetic. |
| **GEOMETRIC** | FP for inherently continuous geometry (quaternions, S², spherical harmonics, trig). |
| **STATISTICAL** | FP for percentages, ratios, averages over combinatorial/integer data. |
| **MIXED** | Multiple significant categories. Subcategories noted in parentheses. |

## Full Audit

| Demo | Category | What the floats do |
|------|----------|-------------------|
| 01 | INTEGER | Pure TL algebra, no FP |
| 02 | INTEGER | Braid words, no FP |
| 03 | INTEGER | No FP |
| 04 | INTEGER | Spin chain, no FP |
| 05 | INTEGER | Scaling, no FP |
| 06 | INTEGER | Jones-Wenzl, no FP |
| 07 | INTEGER | Braid closure, no FP |
| 08 | INTEGER | Vogel algorithm, no FP (grep matched but no actual FP computation) |
| 09 | INTEGER | Reidemeister, no FP (grep matched but no actual FP computation) |
| 10 | EXACT-CORE | Complex doubles ARE the bracket; numerical engine replaces symbolic polynomials |
| 11 | EXACT-CORE | Theta sweep over 256 points, waveform arrays stored as FP |
| 12 | MIXED (core+stat) | FP bracket values + quantization precision-vs-width tradeoff matrix |
| 13 | EXACT-CORE | Complex bracket at unit-circle angles, greedy angle selection, classification |
| 14 | EXACT-CORE | Bisection root-finding on bracket amplitude function |
| 15 | EXACT-CORE | WRT angle derivation, reduced bracket, separation metrics |
| 16 | EXACT-CORE | Reduced bracket waveform characterization (smoothness, dynamic range) |
| 17 | EXACT-CORE | All classifiers (flat greedy, Arf split, hierarchical, gradient descent) on FP brackets |
| 18 | EXACT-CORE | Complex bracket amplitudes thresholded to get gate truth tables |
| 19 | EXACT-CORE | Bracket amplitudes are the gate outputs; Boolean falls out of cx_abs() |
| 20 | MIXED (core+stat) | FP bracket + binned averages, separation percentages, derivative analysis |
| 21 | EXACT-CORE | FP bracket drives Toffoli/Fredkin/MUX circuits |
| 22 | MIXED (core+val) | FP exhaustive braid search + TL_3 matrix cross-validation |
| 23 | EXACT-CORE | Complex bracket at A=e^{i5pi/4}, threshold truth tables |
| 24 | EXACT-CORE | FP complex TL algebra, rational tangles and their bracket ratios |
| 25 | MIXED (core+stat) | FP bracket + SVD, mutual information, ANOVA F-ratios |
| 26 | MIXED (core+val) | FP backprop trains NN weights; RMS/Pearson validate vs bracket |
| 27 | MIXED (core+val) | Complex-valued NN training (split-sigmoid); gauge-sweep RMS validation |
| 28 | MIXED (core+val) | Four activation functions trained in FP; lattice projection validates alignment |
| 29 | VALIDATION | **TRANSITION POINT**: exact Z[zeta_8] introduced; FP only for cross-checks and sigmoid test |
| 35 | VALIDATION | Exact integer Z[zeta_8]; FP for cross-validation (cyc8_to_cx < 1e-10) and display ratios |
| 38 | DISPLAY-ONLY | Exact integer mod p; FP only for printed ratios in summary tables |
| 39 | INTEGER | Symmetry decomposition, no FP |
| 45 | EXACT-CORE | FP complex throughout: bracket, NN training, gauge sweep, RMS |
| 46 | EXACT-CORE | Per-neuron gauge correction; entire chain (bracket->NN->gauge->pn-RMS) is FP |
| 47 | EXACT-CORE | Three FP activations (split-sigmoid, MVN-continuous, MVN-k8) across 13 NPN classes |
| 48 | MIXED (core+val) | Exact Z[zeta_8] for catalog + FP sigmoid for Boolean search + Spearman/RMS validation |
| 49 | MIXED (core+val) | Exact Cyc8 + FP sigmoid search + angle analysis (atan2) + correlation |
| 50 | EXACT-CORE | FP sector_classify (atan2) determines ALL Boolean results (parity reachable at k=6) |
| 51 | VALIDATION | Exact integer algebra; FP only for Gaussian elimination to find radical dimension |
| 52 | VALIDATION | Exact integer Gram matrices; FP for numerical solver + modular arithmetic fallback |
| 53 | VALIDATION | Exact Eisenstein integers Z[omega]; FP only for cross-checks within tolerance |
| 54 | VALIDATION | Exact Z[zeta_16]; FP only for cyc16_to_cx cross-checks |
| 55 | MIXED (core+geom) | Exact Z[zeta_16] catalog + FP activation functions (sigmoid, sector, magnitude) |
| 56 | MIXED (core+geom) | Exact FK partition function + FP activation (sigmoid, atan2 sector) in DKC search |
| 57 | MIXED (core+geom) | Exact Z[zeta_16] + FP activations for NPN reachability comparison (mult vs add) |
| 58 | MIXED (core+geom) | Exact Z[zeta_5] bracket + FP activation geometry (sector/angle/sigmoid) |
| 59 | MIXED (core+geom) | Exact Z[zeta_24] + FP activation geometry (Re>0, sector, rotated half-plane) |
| 60 | INTEGER | Cubic wall, no FP |
| 61 | INTEGER | Angular proof, no FP |
| 62 | GEOMETRIC | FP angles, semicircles, atan2/sin/cos ARE the proof medium |
| 63 | GEOMETRIC | FP atan2 octants, sector_classify — the "angular alphabet" is inherently FP |
| 64 | MIXED (core+geom) | Exact Z[zeta_8] contraction sums + FP sector_classify for XOR_n activation |
| 65 | GEOMETRIC | FP angular/sector geometry throughout; Z[zeta_16] values built exact but all classification is FP |
| 66 | MIXED (geom+core) | FP quaternion products (SU(2) braid rep) + exact Cyc8 bracket — dual projections |
| 67 | GEOMETRIC | Eigenvector extraction via acos/sqrt on S2, Voronoi cell assignment |
| 68 | GEOMETRIC | Stereographic projection S2->R2 for decision-boundary visualization |
| 69 | GEOMETRIC | cos/sin generators at zeta_16/zeta_32, rotation-axis extraction on S2 |
| 70 | MIXED (geom+display) | Eigenvalue angles -> cents/Hz/MIDI musical mapping + Voronoi XOR6 geometry |
| 71 | GEOMETRIC | Full spherical harmonic engine (Gauss-Legendre quadrature, Y_lm integration on S2) |
| 72 | VALIDATION | Y_lm sums over 13 directions to test t-design property |
| 73 | GEOMETRIC | Quaternion normalization + acos/atan2 for cell trajectory tracking |
| 74 | MIXED (geom+core) | Exact Cyc8 bracket + FP Voronoi cell as independent invariants |
| 75 | GEOMETRIC | acos eigenvalue angle + eigenvector axis form two simultaneous output channels |
| 76 | MIXED (geom+stat) | Voronoi geometry + entropy/mutual-information statistics |
| 77 | GEOMETRIC | rotation_angle (acos) + eigenvector axis across 14 activation functions |
| 78 | GEOMETRIC | Combined Sec(k)xVor activation in minimum-cells-vs-inputs phase diagram |
| 79 | GEOMETRIC | cos/sin for zeta_12 generators + same combined activation capacity ladder |
| 80 | MIXED (geom+stat) | cos/sin generators + atan2 half-angle census + gap statistics for group finiteness |
| 81 | GEOMETRIC | Quaternion group closure in FP; eigenvector extraction (sqrt/atan2/acos); Voronoi cell assignment; log/pow for scaling law analysis; entire computation path is continuous geometry |
| 82 | MIXED (geom+stat) | Same quaternion/geometric engine as D81; depth analysis adds mean-depth statistics (sum_d/(double)cnt) and percentage display across winner tuples; atan2 for per-depth angle vocabulary |
| 83 | MIXED (geom+stat) | Quaternion group closure (cos/sin generators, qmul) + Jones normalization (FP phase rotation per writhe unit); Pearson depth-writhe correlation, mean/mean-abs writhe per depth level, information-loss percentages; acos/atan2 sector in combined_cell |
| 84 | MIXED (geom+val+stat) | Same quaternion/S2 geometric engine as D83; FP threshold (fabs(qa)<1e-10) classifies null states; Part B gap theorem validates min non-null |Re| > 0.4; Part D null-prevalence ratio (mean null vs 9/24 expected) and Part C random-control means are statistical |
| 85 | VALIDATION | Integer TL algebra (link states, Hamiltonian); FP only for Gaussian elimination (rank, null spaces) and Gram matrix analysis to extract Jordan structure. Same pattern as D51-52: exact integer objects, FP linear algebra to determine structural properties |
| 86 | EXACT-CORE | b-parameter computed in double precision with irrational eigenvalues (sqrt2, sqrt3, sqrt3+-1); double-precision RREF for null spaces at non-zero lambda; all b_loop/b_trace results are FP values. Eigenvalues are inherently irrational, computation cannot be exact integer |
| 87 | MIXED (geom+stat) | Quaternion group closure (cos/sin zeta_12 generators); eigenvector extraction (sqrt/acos); Voronoi cell + combined_cell (sector via acos/atan2); spherical harmonics (Y_lm); Jacobi eigenvalue solver; simulated annealing perturbation. Statistics: null/non-null direction partition percentages, mean angle comparisons, XOR capacity loss percentages |
| 88 | MIXED (geom+stat) | Same quaternion/S2 geometric engine; pairwise angle analysis (Gram matrix, eigenspectrum); spherical design residuals (Y_lm); simulated annealing perturbation sensitivity; cross-root comparison (zeta_8 vs zeta_12). Statistics: mean angles by partition, perturbation loss percentages, condition ratios, per-direction residual comparisons |
| 89 | MIXED (geom+stat) | Quaternion group closure + combined_cell for catalog/capacity. Heavy statistics: mean pairwise quaternion dot (coherence), single-depth vs random discriminating experiment, cross-depth coherence matrices, angle variance ratios (stddev/stddev), spherical harmonic power spectra by depth shell |
| 90 | MIXED (geom+stat) | Quaternion engine + extract_angle (acos) for pairwise sum-angle census. Statistics: sum-angle histograms (min/max/mean/distinct counts), depth-stratified angle distributions, random-vs-structured comparison, winner anti-alignment statistics (axis-dot), entries-per-direction concentration |
| 91 | MIXED (geom+stat) | Quaternion engine + combined_cell with k_sec sweep (activation resolution experiment). Core question is about the FP geometric activation (acos sector classification). Statistics: XOR/XNOR/AND/OR/MAJ separation rates as percentages, collision analysis, masks-per-cell ratios |
| 92 | MIXED (geom+stat) | Same quaternion engine + sector_of/voronoi_of for multi-function scaling. FP quaternion sums, sector classification (acos). Statistics: per-function separation rates at each depth, scaling comparison across AND/OR/MAJ/XOR, percentage displays |
| 93 | MIXED (geom+stat) | Exact BFS closure (quaternion group elements stored as FP from generators). FP phase_cell/combined_cell activation (atan2/acos sector classification). FP Voronoi cell assignment. FP comparison for complement-pair sharing test (1e-6 tolerance). Statistics: hit rates, ratios, growth factors across N=3-8 and depths 0-8 |
| 94 | EXACT-CORE | Z[sqrt5] exact integer arithmetic for ALL group operations (Zr5 ring, Q2I quaternion multiply, BFS closure, norm check). FP only at readout boundary: q2i_to_float() converts exact Q2I to FP Quat for phase_cell/Voronoi activation and hit rate computation. Group theory is fully exact; activation/comparison is FP |
| 95 | EXACT-CORE | Z[sqrt2] exact integer arithmetic for all z8 group operations (Zr2 ring, QZ8 quaternion multiply, BFS closure, derived series computation, commutator classification). FP only at readout boundary: qz8_to_float() converts exact QZ8 to FP Quat for phase_cell/Voronoi activation. Same D94 pattern: group algebra fully exact, FP at activation boundary |
| 96 | EXACT-CORE | Reuses D95's exact Z[sqrt2]/QZ8 infrastructure for group structure (derived series, null classification, cell cross-tabulation). FP only at readout: qz8_to_float() for phase_cell capacity testing. Cell membership computation is pure integer comparison |
| 97 | MIXED (exact-core+geom) | Z[sqrt2] exact group algebra (reused from D95) for Cell B extraction and direction computation. FP for parametric angle sweep: synthetic quaternions at 21 angles from 10-90 degrees use cos/sin to generate test catalogs. Phase_cell activation uses acos/atan2. The angle sweep is inherently FP (continuous parameter space) |
| 98 | EXACT-CORE | Full TL_3 5x5 representation over exact Z[zeta_8] (Cyc8). BFS catalog, diagram composition, braid generators, trace computation — all exact integer. FP at two boundaries: (1) cyc8_to_double for trace-based phase_cell activation, (2) quaternion-based 2-strand comparison uses FP Quat type. 3-strand algebra is fully exact; readout is FP |
| 99 | EXACT-CORE | Delta_1 2x2 standard module over exact Z[zeta_8]. Mat2 multiplication, BFS catalog, commutators, Casimir discriminant — all exact integer. FP at activation boundary: 2x2_hash uses sign patterns of integer components (no trig/sqrt), but cyc8_to_cplx for some validation paths. 2-strand comparison uses FP Quat. Core 3-strand computation is fully exact through Casimir analysis |
| 100 | EXACT-CORE | W_{4,2} 3x3 matrices over exact Z[zeta_8]. Mat3 operations, TL generator construction, BFS, radical analysis, Casimir C3, hub commutators — all exact integer. mat3_activate uses sign patterns of 36 integer components (3-valued hash, no FP). FP only for percentage display in statistics. All algebraic computation including activation is exact |
| 101 | EXACT-CORE | W_{5,3} 4x4 matrices over exact Z[zeta_8]. Mat4 operations, TL generator construction, BFS, Casimir C4, hub commutators — all exact integer. mat4_activate uses sign patterns of 64 integer components (3-valued hash, no FP). FP only for percentage display and growth-rate ratios. Most exact demo in the multi-strand series: even activation is integer-only |
| 102 | DISPLAY-ONLY | W_{6,0} vs W_{6,4} radical A/B test. 5x5 matrices over exact Z[zeta_8]. Sign-pattern activation (mat5_activate, mat5_activate_quot). FP only for printf ratios (%.2f) and percentages (%.1f%%) |
| 103 | DISPLAY-ONLY | W_{6,0} (dim=5) vs W_{6,2} (dim=9) dimension scaling. MatN over exact Z[zeta_8]. FP only for printf display of ratios and percentages |
| 104 | DISPLAY-ONLY | Activation coarseness sweep reusing D103 catalogs. Integer Gaussian elimination for rank/sign-rank. FP only for printf percentage display |
| 105 | DISPLAY-ONLY | W_{8,0} (dim=14) and W_{8,2} (dim=28) k-regime test. MatN over exact Z[zeta_8]. All 5 source files (main.c, phase_b.c, rank_retroactive.c, rank_saturation.c, rank_saturation_j0.c) have FP only in printf percentages |
| 106 | MIXED (exact-core+stat) | Topological entropy of XOR-computing braids. Exact Z[zeta_8] TL for bracket/XOR + exact integer 3x3 Burau matrices. FP for spectral_radius (sqrt/fabs eigenvalue estimation), log for entropy per crossing, statistical percentages |
| 107 | DISPLAY-ONLY | Algebraic graph density using Raqiya library. Exact Z[zeta_8] raqiya analysis (root orbits, norm classes, Galois orbits). FP only for printf percentages |
| 108 | MIXED (exact-core+geom) | Parity vocabulary 3-way graph (parity/poison/neutral). Exact Z[zeta_8] bracket + exact raqiya graph analysis. FP for sector_classify (atan2/sqrt via Cx type) at parity partition boundary + percentage displays |
| 109 | MIXED (exact-core+geom) | D55 parity vocabulary graph analysis at delta=sqrt(2). Exact Z[zeta_16] bracket + exact Z[i]-axis graph algebra (product closure, divisibility, Galois orbits). FP for cyc16_to_cx (cos/sin) and Re>0 activation boundary + percentage displays |

## Summary by Category

| Category | Count | Demos |
|----------|-------|-------|
| INTEGER | 12 | 01-09, 39, 60, 61 |
| DISPLAY-ONLY | 6 | 38, 102, 103, 104, 105, 107 |
| VALIDATION | 8 | 29, 35, 51, 52, 53, 54, 72, 85 |
| EXACT-CORE | 24 | 10, 11, 13-19, 21, 23, 24, 45-47, 50, 86, 94, 95, 96, 98, 99, 100, 101 |
| GEOMETRIC | 13 | 62, 63, 65, 67-69, 71, 73, 75, 77-79, 81 |
| MIXED | 34 | 12, 20, 22, 25-28, 48-49, 55-59, 64, 66, 70, 74, 76, 80, 82-84, 87-93, 97, 106, 108, 109 |

## Three Regimes

The codebase shows a clear evolutionary arc in its relationship to floating point:

### Regime 1: Pre-numerical (D01-D09)
Pure integer algebra. Laurent polynomials, braid combinatorics, TL relations. No FP at all. This is the algebraic infrastructure layer.

### Regime 2: FP-native era (D10-D28)
Everything computational is complex doubles. The bracket itself is computed as `cx_abs(bracket_at(A))` where A is on the unit circle. All early DKC results (gate logic, function zoo, reverse DKC, NN training) live here. Every result depends on FP.

### Regime 3: Exact + FP boundary (D29 onward)
D29 introduces exact Z[zeta_8] integer arithmetic. From here, the *values* (bracket weights) are exact. But FP persists at two boundaries:

- **Activation boundary (D45-D65)**: The activation function (split-sigmoid, sector_classify via atan2, magnitude threshold) converts exact cyclotomic values into Boolean outputs. The classification step is inherently FP even though the inputs are exact.
- **Geometric boundary (D62-D93)**: S2 geometry, quaternion rotations, spherical harmonics. The *where* (eigenvector directions on S2) is continuous geometry, even though the *what* (bracket values) is exact algebra. D81-D93 extend this boundary with quaternion group closure, depth-stratified analysis, sum-angle structure, activation resolution sweeps, multi-function scaling, and the complement-blindness/phase-cell recovery analysis — all built on the same FP geometric engine. D94 introduces a notable refinement: the group algebra (Z[sqrt5] quaternion multiplication, BFS closure, norm checking) is fully exact integer arithmetic; FP appears only at the q2i_to_float() readout boundary where exact group elements are converted to the FP activation framework. D95-96 continue this pattern with Z[sqrt2]/QZ8 for z8 group operations (exact through derived series and cell classification, FP only at readout). D97's angle sweep is the one exception: the parametric half-angle exploration is inherently FP because it scans a continuous parameter space. D98-D101 mark a significant evolution: the multi-strand matrix representations (TL_3 through TL_5) operate entirely in exact Z[zeta_8] matrix arithmetic, and crucially, the sign-pattern activations (2x2_hash, 3x3_hash, 4x4_hash) classify integer components directly without any trig or FP conversion — making D100 and D101 the most fully exact demos in the project, with FP reduced to only display-level percentages. D102-D105 continue this trend with 6-strand and 8-strand representations (5x5 through 28x28 matrices) all operating in exact Z[zeta_8] with sign-pattern activation — FP appears only in printf formatting for display ratios and percentages (DISPLAY-ONLY). D107 applies the same pattern through raqiya's exact algebraic graph analysis. D106 is an exception: its topological entropy computation requires FP spectral radius estimation (sqrt/fabs/log on the Burau representation's characteristic polynomial), making it MIXED despite exact TL/XOR algebra. D108-D109 return to the activation boundary theme from D45-D65: exact cyclotomic bracket values partitioned by FP geometric classification (atan2 sector in D108, Re>0 via cos/sin in D109), with the new element being raqiya's exact graph analysis layered on top of the FP-partitioned sets.

## Exactification Opportunities

Three levels of difficulty for eliminating remaining FP:

### Easy: Remove VALIDATION FP
Demos 29, 35, 51-54, 72, 85 use FP only to cross-check exact results. The FP could be removed entirely without affecting any result. The Gaussian elimination in D51-52 and D85 could use exact rational arithmetic (integer num/denom pairs).

### Medium: Exact sector classification
The activation functions in D45-D65 use atan2 to determine which angular sector a cyclotomic integer falls in. But for Z[zeta_8] values, the sector boundaries at multiples of pi/4 align with the ring structure. You can determine which octant a cyclotomic integer (a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3) falls in purely from the integer coefficients — no trig needed. This would make the entire DKC pipeline from bracket to Boolean output fully exact.

### Hard: Exact S2 geometry
Quaternion products are algebraic (polynomial multiplication). Eigenvector extraction from 2x2 unitary matrices is algebraic. Voronoi cell assignment is a finite set of dot-product comparisons. In principle all of D66-D92 could be done in exact algebraic arithmetic. In practice the expressions are complex and the code would be significantly more verbose. D87-D92 add further FP-geometric layers (spherical harmonics, simulated annealing, coherence matrices, sum-angle analysis, activation resolution sweeps, multi-function scaling) that would all need algebraic reformulation.

## Update Process

This file is updated during Phase 2 of the atlas defrag. For each new demo, read main.c and classify its FP usage. Update the full audit table, summary counts, and regime descriptions as needed.
