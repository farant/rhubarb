# Floating-Point Usage Audit

Last updated: 2026-02-24 (82 demos, D01-D94)

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

## Summary by Category

| Category | Count | Demos |
|----------|-------|-------|
| INTEGER | 12 | 01-09, 39, 60, 61 |
| DISPLAY-ONLY | 1 | 38 |
| VALIDATION | 8 | 29, 35, 51, 52, 53, 54, 72, 85 |
| EXACT-CORE | 18 | 10, 11, 13-19, 21, 23, 24, 45-47, 50, 86, 94 |
| GEOMETRIC | 13 | 62, 63, 65, 67-69, 71, 73, 75, 77-79, 81 |
| MIXED | 30 | 12, 20, 22, 25-28, 48-49, 55-59, 64, 66, 70, 74, 76, 80, 82-84, 87-93 |

## Three Regimes

The codebase shows a clear evolutionary arc in its relationship to floating point:

### Regime 1: Pre-numerical (D01-D09)
Pure integer algebra. Laurent polynomials, braid combinatorics, TL relations. No FP at all. This is the algebraic infrastructure layer.

### Regime 2: FP-native era (D10-D28)
Everything computational is complex doubles. The bracket itself is computed as `cx_abs(bracket_at(A))` where A is on the unit circle. All early DKC results (gate logic, function zoo, reverse DKC, NN training) live here. Every result depends on FP.

### Regime 3: Exact + FP boundary (D29 onward)
D29 introduces exact Z[zeta_8] integer arithmetic. From here, the *values* (bracket weights) are exact. But FP persists at two boundaries:

- **Activation boundary (D45-D65)**: The activation function (split-sigmoid, sector_classify via atan2, magnitude threshold) converts exact cyclotomic values into Boolean outputs. The classification step is inherently FP even though the inputs are exact.
- **Geometric boundary (D62-D93)**: S2 geometry, quaternion rotations, spherical harmonics. The *where* (eigenvector directions on S2) is continuous geometry, even though the *what* (bracket values) is exact algebra. D81-D93 extend this boundary with quaternion group closure, depth-stratified analysis, sum-angle structure, activation resolution sweeps, multi-function scaling, and the complement-blindness/phase-cell recovery analysis — all built on the same FP geometric engine. D94 introduces a notable refinement: the group algebra (Z[sqrt5] quaternion multiplication, BFS closure, norm checking) is fully exact integer arithmetic; FP appears only at the q2i_to_float() readout boundary where exact group elements are converted to the FP activation framework.

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
