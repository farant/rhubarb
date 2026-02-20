# Knotapel Demo Index — Batch A (Demos 01–25)

Generated: 2026-02-19

---

## Demo 01: Kauffman Bracket

- **Status**: Complete
- **File**: `demo_01_kauffman_bracket/main.c` (~497 lines)
- **Tests**: 9/9 (polynomial arithmetic, trefoil, figure-eight, Hopf link, discrimination)
- **Dependencies**: None (stdio, stdlib, string)
- **Headline**: State-sum computation of the Kauffman bracket as a Laurent polynomial from PD notation.
- **Key Results**:
  - Laurent polynomial type (Poly) with add/sub/mul/shift operations
  - PD (planar diagram) notation: 4 arc labels per crossing
  - State-sum: iterate over 2^n resolutions, count loops via union-find
  - bracket(trefoil) = -A^16 + A^12 + A^4
  - bracket(figure-eight) = A^8 - A^4 + 1 - A^{-4} + A^{-8}
  - bracket(Hopf) = -A^4 - A^{-4}
  - Bracket discriminates trefoil from figure-eight
- **Theorems/Conjectures**: State-sum formula for Kauffman bracket; bracket is invariant under R2, R3 (not R1)
- **Data**: Hard-coded trefoil (3 crossings), figure-eight (4 crossings), Hopf link (2 crossings)
- **Code Assets**: `Poly` (Laurent polynomial), `Knot` (PD notation), `state_sum_bracket()`, union-find
- **Literature Touched**: Kauffman (1987), state-sum model
- **Open Questions**: None

---

## Demo 02: Braid Words & Markov Trace

- **Status**: Complete
- **File**: `demo_02_braid_words/main.c` (~953 lines)
- **Tests**: ~9 (braid bracket, Jones polynomial, Markov property)
- **Dependencies**: None (stdio, stdlib, string, math)
- **Headline**: Braid group representation and bracket/Jones computation from braid words via state-sum with union-find loop counting.
- **Key Results**:
  - Braid type with word array (generators sigma_i, negative = inverse)
  - braid_bracket() via state-sum: resolution bits select A-smoothing or B-smoothing per crossing
  - braid_jones() via writhe normalization: J(K) = (-A^3)^{-w} * bracket(K)
  - Union-find loop counting for braid closures
  - Writhe computation from braid word (positive = +1, negative = -1)
- **Theorems/Conjectures**: Markov trace property; Jones polynomial from bracket via writhe normalization
- **Data**: Trefoil, figure-eight, torus knots as braid words
- **Code Assets**: `Braid`, `braid_bracket()`, `braid_jones()`, `braid_writhe()`, union-find
- **Literature Touched**: Jones (1985), Alexander/Markov theorems
- **Open Questions**: None

---

## Demo 03: TL Module & Braid Representation

- **Status**: Complete (labeled v3)
- **File**: `demo_03/main.c` (~604 lines)
- **Tests**: Multiple (TL algebra, module, braid rep, trace decomposition)
- **Dependencies**: None
- **Headline**: TL_3 standard module W_1, braid representation g_i = A*U_i + A^{-1}*I, trace decomposition.
- **Key Results**:
  - TL_3 generators U_i satisfying U_i^2 = delta * U_i
  - Standard module W_1 (4x4 matrices)
  - Braid representation: g_i = A*U_i + A^{-1}*I
  - **Discovery**: bracket = 1*tr_W1 + (A^{-4}+A^4)*tr_W3
  - Trace decomposition into irreducible TL modules
- **Theorems/Conjectures**: Bracket decomposes as weighted sum of traces over TL modules
- **Data**: Fixed 4x4 matrices for TL_3 W_1 module
- **Code Assets**: 4x4 matrix arithmetic (Poly entries), TL generator construction, trace
- **Literature Touched**: Temperley-Lieb algebra, Jones (1983)
- **Open Questions**: None

---

## Demo 04: Spin Chain (Tensor Product)

- **Status**: Complete
- **File**: `demo_04_spin_chain/main.c` (~587 lines)
- **Tests**: Multiple (R-matrix, Yang-Baxter, quantum trace)
- **Dependencies**: None
- **Headline**: V^{tensor 3} = C^8 representation with R-matrix and quantum trace formula for the bracket.
- **Key Results**:
  - 8x8 tensor product representation V^{⊗3}
  - R-matrix: R = A*U + A^{-1}*I (4x4, embedded in 8x8)
  - Yang-Baxter equation verified: R1*R2*R1 = R2*R1*R2
  - **Quantum trace formula discovered**: bracket(b) = (-1)^{n+1} * tr(K^{⊗n} * rho(b)) / [2]_q
  - K = diag(A^2, A^{-2}), [2]_q = A^2 + A^{-2}
- **Theorems/Conjectures**: Quantum trace formula for Kauffman bracket from spin chain
- **Data**: Fixed 8x8 polynomial matrices
- **Code Assets**: Polynomial matrix multiplication, tensor products, quantum trace
- **Literature Touched**: Kauffman/Temperley-Lieb R-matrix, quantum groups
- **Open Questions**: None

---

## Demo 05: Scaling the Spin Chain

- **Status**: Complete
- **File**: `demo_05_scaling/main.c` (~678 lines)
- **Tests**: Multiple (n=2..5, sign factor, Yang-Baxter at each n)
- **Dependencies**: None
- **Headline**: Heap-allocated matrices scale quantum trace formula to n=2..5 strands (up to 32x32).
- **Key Results**:
  - Heap-allocated NxN polynomial matrices (no stack limit)
  - Scales to n=5 (32x32 matrices, ~800KB per matrix)
  - **Sign factor discovered**: tr_q = (-1)^n * delta * oracle
  - Verified quantum trace matches state-sum oracle at all n
  - Memory: n=5 matrix = 32*32*808 bytes ≈ 800KB
- **Theorems/Conjectures**: Sign factor formula for quantum trace
- **Data**: Braids at n=2..5 strands
- **Code Assets**: Heap polynomial matrices, tensor product scaling, `build_Ri()`, `build_Kn()`
- **Literature Touched**: Scaling limits of symbolic computation
- **Open Questions**: Scaling beyond n=5 impractical with symbolic polynomials (motivates Demo 10)

---

## Demo 06: Jones-Wenzl Idempotents

- **Status**: Complete
- **File**: `demo_06_jones_wenzl/main.c` (~800 lines)
- **Tests**: Multiple (idempotent property, Markov trace, decomposition)
- **Dependencies**: None
- **Headline**: Scaled Jones-Wenzl idempotents Q_2 (4x4) and Q_3 (8x8) with Markov trace decomposition.
- **Key Results**:
  - Jones-Wenzl projector p_n via recursive formula
  - Scaled idempotents Q_2 (4x4), Q_3 (8x8) to avoid division
  - Polynomial exact division for JW coefficients
  - Markov trace decomposition verified
- **Theorems/Conjectures**: Jones-Wenzl recursive formula; projector properties (p^2 = p, U_i*p = 0)
- **Data**: Fixed matrices
- **Code Assets**: JW idempotent construction, polynomial exact division
- **Literature Touched**: Jones (1983), Wenzl (1987)
- **Open Questions**: None

---

## Demo 07: Braid Closure Bridge

- **Status**: Complete
- **File**: `demo_07_braid_closure/main.c` (~791 lines)
- **Tests**: Multiple (PD construction, Seifert circles, component counting)
- **Dependencies**: None
- **Headline**: braid_to_pd() conversion with Seifert circle computation, bridging braid and diagram worlds.
- **Key Results**:
  - `braid_to_pd()`: converts braid word to PD notation via strand tracking
  - Seifert circle computation from PD diagram
  - Link component counting
  - Verified: bracket(braid_to_pd(b)) = bracket_from_braid(b)
- **Theorems/Conjectures**: Alexander's theorem (every link is a closed braid)
- **Data**: Multiple test braids
- **Code Assets**: `braid_to_pd()`, Seifert circle finder, strand tracking
- **Literature Touched**: Alexander (1923), Seifert (1934)
- **Open Questions**: None

---

## Demo 08: Vogel's Algorithm

- **Status**: Complete
- **File**: `demo_08_vogel/main.c` (~887 lines)
- **Tests**: Multiple (round-trip verification, Seifert membership)
- **Dependencies**: None
- **Headline**: pd_to_braid() compiler via Vogel's algorithm; round-trip verification with Demo 07's braid_to_pd().
- **Key Results**:
  - `pd_to_braid()`: compiles PD notation into braid word
  - Union-find for Seifert circle membership
  - Round-trip verified: pd_to_braid(braid_to_pd(b)) produces equivalent bracket
  - Handles both positive and negative crossings
- **Theorems/Conjectures**: Vogel's algorithm correctness
- **Data**: Multiple test knots/links
- **Code Assets**: `pd_to_braid()`, Seifert membership via union-find
- **Literature Touched**: Vogel (1990)
- **Open Questions**: None

---

## Demo 09: Reidemeister Move Invariance

- **Status**: Complete
- **File**: `demo_09_reidemeister/main.c` (~717 lines)
- **Tests**: 30 (R1, R2, R3 moves; bracket, writhe, Jones polynomial)
- **Dependencies**: None
- **Headline**: Systematic verification of Reidemeister move invariance for bracket, writhe, and Jones polynomial.
- **Key Results**:
  - R1: bracket changes by (-A^3)^{±1}, writhe changes by ±1, Jones invariant
  - R2: bracket, writhe, Jones all invariant
  - R3: bracket, writhe, Jones all invariant
  - 30 tests across 5 parts (A-E)
  - Jones polynomial confirmed as true knot invariant (invariant under all 3 moves)
- **Theorems/Conjectures**: Jones polynomial is a knot invariant (R1+R2+R3 invariance)
- **Data**: Knots before/after each Reidemeister move
- **Code Assets**: R1/R2/R3 move generators, invariance test framework
- **Literature Touched**: Reidemeister (1927), Jones (1985)
- **Open Questions**: None

---

## Demo 10: Numerical Evaluation Engine

- **Status**: Complete
- **File**: `demo_10_numerical/main.c` (~955 lines)
- **Tests**: ~30+ (complex arithmetic, R-matrix, Yang-Baxter, bracket oracle, scaling n=2..8, multi-angle)
- **Dependencies**: math.h
- **Headline**: Replace symbolic polynomials with numerical complex evaluation; 50-100x memory reduction enables scaling to n=8+.
- **Key Results**:
  - C89-compatible complex arithmetic library (Cx type)
  - Numerical R-matrix and quantum trace at fixed A = e^{iθ}
  - Each Poly (808 bytes) becomes one Cx (16 bytes) — 50x reduction
  - Validated against state-sum oracle at multiple angles
  - Scales to n=8 (256x256 matrices) easily
  - Yang-Baxter verified numerically at each n
  - Part F: 2 sample points suffice to distinguish 6 test knots
- **Theorems/Conjectures**: Numerical bracket matches symbolic bracket at all tested angles
- **Data**: 6 knots (unknot through granny), 8 sample angles, scaling n=2..8
- **Code Assets**: `Cx` (complex type), `CMat` (heap NxN complex matrix), `build_R4()`, `build_Ri()`, `build_Kn()`, `numerical_bracket()`, `braid_bracket_at()`
- **Literature Touched**: Bridge from symbolic to numerical evaluation
- **Open Questions**: How many sample points needed for larger knot tables? (→ Demo 11)

---

## Demo 11: Waveform Signatures

- **Status**: Complete
- **File**: `demo_11_waveform/main.c` (~648 lines)
- **Tests**: ~15 (symmetry, chiral amplitude identity, greedy angles, cross-validation)
- **Dependencies**: math.h
- **Headline**: Sweep θ from 0 to 2π; chiral pairs share amplitude waveforms; greedy optimal angle selection.
- **Key Results**:
  - 10 knots, 256-sample waveforms f(θ) = bracket(e^{iθ})
  - **Chiral pair amplitude identity**: |bracket_trefoil(θ)| = |bracket_mirror(θ)| (verified exactly)
  - Amphichiral figure-eight: amplitude symmetric about θ=0
  - Phase relation: bracket_mirror(e^{iθ}) = bracket(e^{-iθ})
  - Greedy optimal angle: θ ≈ 1.805π separates all 10 knots (min pairwise distance 0.907)
  - ASCII waveform visualization
- **Theorems/Conjectures**: Chiral pairs have identical amplitude; phase carries chirality information
- **Data**: 10 knots (unknot through single-crossing), 256 waveform samples each
- **Code Assets**: `compute_waveforms()`, `greedy_angles()`, `pairwise_separation()`, ASCII visualization
- **Literature Touched**: Chiral knot theory, amplitude vs phase
- **Open Questions**: How aggressively can we quantize? (→ Demo 12)

---

## Demo 12: Quantization

- **Status**: Complete
- **File**: `demo_12_quantization/main.c` (~779 lines)
- **Tests**: ~8 (bit-depth sweep, Pareto optimum, classifier, invariance)
- **Dependencies**: math.h
- **Headline**: Bit-depth sweep and component analysis; amplitude-only fails (granny/square collision); Pareto optimum found.
- **Key Results**:
  - Bit-depth sweep: 16→1 bits per component
  - Minimum bits for 100% at 1 angle: found via fine sweep
  - **Amplitude-only fails**: granny/square have identical |bracket| (predicted from Demo 11)
  - Phase-only also insufficient alone
  - Precision × width tradeoff matrix (1-8 bits × 1-8 angles)
  - Pareto optimum: minimal total bits per knot for full classification
  - Nearest-neighbor classifier with quantized weights
  - Invariance test: alternate braid words correctly classified
- **Theorems/Conjectures**: Phase carries topology that amplitude cannot; Pareto-optimal quantization exists
- **Data**: 10 knots, 8 greedy angles, bit-depth 1-16
- **Code Assets**: `quantize()`, `cx_quantize()`, `cx_ternary()`, `cx_sign()`, `classify_braid()`, tradeoff matrix
- **Literature Touched**: BitNet (1.58-bit weights parallel), quantization theory
- **Open Questions**: Does Pareto frontier hold at 28+ knots? (→ Demo 13)

---

## Demo 13: DKC Test

- **Status**: Complete
- **File**: `demo_13_dkc_test/main.c` (~948 lines)
- **Tests**: ~6 (separation, multi-angle, Pareto, DKC validation, baseline, scaling law)
- **Dependencies**: math.h
- **Headline**: 28 candidate braids deduplicated to ~23 unique knots; DKC thesis validated: algebraic weights generalize without training.
- **Key Results**:
  - 28 candidate 3-strand braids: torus knots T(2,3)..T(2,13) + mirrors, figure-eight, connected sums, misc
  - Deduplication via bracket at 2 reference angles → ~23 unique knots
  - Multi-angle separation: number of angles needed for full separation
  - Precision × width tradeoff at scale
  - **DKC validation**: 70/30 train/test split, 100% self-classification on training set
  - Alt-braid invariance confirmed (s2.s1.s2 → trefoil)
  - Random baseline comparison (100 trials)
  - Scaling law: min_sep decreases with knot count (< 0.907 at 23 knots)
- **Theorems/Conjectures**: DKC thesis: compiled algebraic weights generalize without training because they encode topology
- **Data**: ~28 braids, ~23 unique knot types, 8 greedy angles
- **Code Assets**: `add_torus_2n()`, `add_connected_sum()`, `dedup_knots()`, `select_greedy_angles()`, `classify_against_table()`, LCG RNG
- **Literature Touched**: DKC (original thesis)
- **Open Questions**: What invariant structure underlies zeros? (→ Demo 14)

---

## Demo 14: Zero Structure

- **Status**: Complete
- **File**: `demo_14_zero_structure/main.c` (~830 lines)
- **Tests**: ~6 (zero count pattern, figure-eight, chiral reflection, factorization, classification)
- **Dependencies**: math.h
- **Headline**: Bracket zeros as knot invariant; bisection root-finding; bracket = δ^k * g factorization; braid ≠ diagram connected sum.
- **Key Results**:
  - High-resolution zero finding via bisection (1024 samples)
  - Torus knot zero counts from {0, 4, 8} only — NOT monotonic with crossing number
  - Figure-eight: 0 zeros (amphichiral, min amplitude well above 0)
  - Chiral pairs have reflected zero positions (θ ↔ 2π-θ)
  - Delta zeros at π/4, 3π/4, 5π/4, 7π/4 (= -2cos(2θ) = 0)
  - **Connected sum factorization**: bracket(K1#K2) = bracket(K1)*bracket(K2)/δ does NOT hold for braid-encoded sums
  - Zero count separates ~40-60% of pairs; count + positions improves further
  - Clear gap between zero and non-zero knots
- **Theorems/Conjectures**: bracket = δ^k * g factorization; braid concatenation ≠ diagram connected sum
- **Data**: ~23 knots with crossing numbers, zero counts, zero positions
- **Code Assets**: `bracket_amp()`, `bisect_zero()`, zero position arrays, chiral reflection test
- **Literature Touched**: Bracket zero structure, connected sum formula
- **Open Questions**: Which angles optimize g? (→ Demo 15)

---

## Demo 15: Angle Optimization — WRT Convergence

- **Status**: Complete
- **File**: `demo_15_angle_optimization/main.c` (~883 lines)
- **Tests**: ~7 (WRT convergence, single-angle, reduced bracket, multi-angle, survey)
- **Dependencies**: math.h
- **Headline**: Greedy-optimal angle (1.805π) is 1.5% from WRT level r=3 angle; reduced bracket g outperforms raw bracket.
- **Key Results**:
  - WRT angle for level r: θ = 2π - π/(2r)
  - Closest WRT to greedy: r=3 at 11π/6 = 1.833π, distance 1.5% of π
  - Greedy outperforms all WRT angles on min_dist (as expected: greedy is min_dist-optimal)
  - **Reduced bracket g outperforms raw bracket**: tighter dynamic range, better separation
  - Delta exponent k computed from zero count at δ-zero positions
  - g = bracket / δ^k never vanishes on unit circle (for k > 0 knots)
  - 128-angle survey: top-5 raw and g angles identified
  - 3 WRT angles on g separate all pairs
- **Theorems/Conjectures**: Algebraically special (WRT) angles ≈ informationally optimal angles; g outperforms raw bracket
- **Data**: ~21 knots (deduped), WRT levels r=3..12, 128-angle survey
- **Code Assets**: `delta_exponent()`, `reduced_bracket_at()`, `wrt_angle()`, `SepMetrics`, `measure_separation()`
- **Literature Touched**: Witten-Reshetikhin-Turaev invariant, Chern-Simons theory
- **Open Questions**: What IS g? (→ Demo 16)

---

## Demo 16: Reduced Bracket Characterization

- **Status**: Complete
- **File**: `demo_16_reduced_bracket/main.c` (~974 lines)
- **Tests**: ~8 (waveform, peak, Pareto, factorization, minimal angles, amplitude stats)
- **Dependencies**: math.h, string.h
- **Headline**: g characterization: broad peak at ~1.266π; Pareto frontier; connected sum non-factorization confirmed; 6 bits at 1 angle for full 210/210 classification.
- **Key Results**:
  - g waveform has ≤ raw zeros (removes δ-zeros for k>0 knots)
  - g peak at ~1.266π: broad plateau (width > 0.05π at half-max)
  - g peak min_dist > raw peak min_dist (4.3x improvement at best angle)
  - **Pareto frontier**: g reaches full classification in fewer bits than raw bracket
  - **Connected sum non-factorization confirmed**: g(K1#K2) ≠ g(K1)*g(K2) for braid-encoded sums
  - Greedy min_dist selection: g achieves higher min_dist than raw at each round
  - Family amplitude statistics: torus ≈ mirror (chirality), composites differ from primes
  - **6 bits at 1 angle for full 210/210 classification** (baseline for Demo 17)
- **Theorems/Conjectures**: g is smoother, more informative than raw bracket; peak is plateau not spike
- **Data**: ~21 knots, 256-sample waveforms (raw + g), composite tracking
- **Code Assets**: `CompositeInfo`, `dedup_map[]`, `min_dist_from_vals()`, family classification
- **Literature Touched**: Reduced bracket theory
- **Open Questions**: Can hierarchy beat flatness? (→ Demo 17)

---

## Demo 17: Multi-Layer DKC

- **Status**: Complete
- **File**: `demo_17_multilayer_dkc/main.c` (~1467 lines)
- **Tests**: ~5 (flat baseline, Arf split, hierarchical tree, gradient descent, Pareto comparison)
- **Dependencies**: math.h, string.h
- **Headline**: Hierarchical DKC: Arf split (k value as Layer 1), greedy binary tree, gradient descent comparison; all methods classified within 10 bits.
- **Key Results**:
  - **Part A**: Flat g baseline — greedy 4 angles, Pareto at 1/2/4 angles
  - **Part B**: Arf split — k=0 vs k>0 groups; cross-group pairs separated FREE by 1 Arf bit; per-group optimal angles
  - **Part C**: Greedy hierarchical binary tree — find angle+threshold that maximally bisects; recurse; tree structure alone separates many pairs
  - **Part D**: Gradient descent comparison — 50 random starts, 100 iterations each, simulated annealing; analytical matches or beats GD
  - **Part E**: Bit budget Pareto — flat-1A, flat-2A, Arf split, tree on same axes; all achieve full classification
  - Craven et al. (2025): neural networks CANNOT learn Arf invariant; DKC uses it for free analytically
- **Theorems/Conjectures**: Arf invariant as free analytical Layer 1; hierarchy saves bits at low budgets; analytical ≥ gradient descent
- **Data**: ~21 knots (deduped), 256-sample survey, quantization 1-10 bits
- **Code Assets**: `TreeNode`, `find_best_bisection()`, `build_tree()`, `QuantVal`, `quantize()`, `count_separated_pairs()`, gradient descent hill-climber
- **Literature Touched**: Craven et al. (2025), Arf invariant, hierarchical classification
- **Open Questions**: Can braids compute logic? (→ Demo 18)

---

## Demo 18: Braid Logic Gates

- **Status**: Complete
- **File**: `demo_18_braid_logic/main.c` (~1037 lines)
- **Tests**: ~20+ (NOT search, NAND search, composition, XOR search, characterization)
- **Dependencies**: math.h
- **Headline**: Can braids compute classical logic? Exhaustive search finds NOT (2-strand) and NAND (3-strand) gates — universal gate set.
- **Key Results**:
  - **Part A**: NOT gate search — 2 strands, up to 6 crossings; found NOT at length 6: [-1,-1,-1,-1,-1,-1]
  - **Part B**: NAND gate search — 3 strands, up to 6 crossings; found NAND at length 5: [-1,-1,-1,2,2]
  - **Part C**: Gate composition — NOT(NAND(x,y)) = AND; verified all 4 input combos
  - **Part D**: XOR gate search — hardest (non-threshold); searched exhaustively
  - **Part E**: Characterization — separation margins, angle sensitivity analysis
  - Input encoding: identity=0, sigma_i=1; output: |bracket| thresholded at delta=0
  - At A = e^{i5π/4} (8th root of unity), delta = 0 → exact Boolean logic
  - **NOT + NAND = universal gate set** for classical computation
- **Theorems/Conjectures**: Braid closures at 8th root of unity compute exact Boolean functions; NOT+NAND is universal
- **Data**: Exhaustive search: 2^L (2-strand) and 4^L (3-strand) gate templates, L=1..6
- **Code Assets**: `GateResult`, `search_gates()`, `eval_gate()`, gate composition framework
- **Literature Touched**: Topological quantum computation, Freedman-Kitaev-Wang
- **Open Questions**: Can we compile arbitrary Boolean expressions? (→ Demo 19)

---

## Demo 19: Braid Circuit Compiler

- **Status**: Complete
- **File**: `demo_19_braid_compiler/main.c` (~1016 lines)
- **Tests**: ~15+ (gate catalog, universal set, expression compiler, full adder, efficiency)
- **Dependencies**: math.h
- **Headline**: Boolean expression compiler using braid gates; root-of-unity gate catalog; full adder verification at A = e^{i5π/4}.
- **Key Results**:
  - **Part A**: Root-of-unity gate catalog — 4th, 6th, 8th, 10th, 12th roots
  - **Part B**: Single-angle universal set at 5π/4 (A = e^{i5π/4}, delta=0)
  - **Part C**: Boolean expression compiler — recursive eval from expression tree
    - AND(x,y) = NOT(NAND(x,y))
    - OR(x,y) = NAND(NOT(x), NOT(y))
    - XOR(x,y) = NAND(NAND(x, NAND(x,y)), NAND(y, NAND(x,y)))
  - **Part D**: Full adder — sum = XOR(XOR(a,b),cin), carry = OR(AND(a,b), AND(cin,XOR(a,b))); all 8 combos verified
  - **Part E**: Efficiency analysis — gate count, depth, evaluation time
  - Expression tree with pool allocator (Expr type, MAX_POOL=1024)
  - Known gates: NOT_WORD = [-1,-1,-1,-1,-1,-1] (len 6), NAND_WORD = [-1,-1,-1,2,2] (len 5)
- **Theorems/Conjectures**: Any Boolean function compilable from braid NOT+NAND; 8th root of unity is algebraic sweet spot
- **Data**: Full adder truth table (8 rows), gate catalog at 5 root-of-unity orders
- **Code Assets**: `Expr` (expression tree), `ExprPool`, `eval_expr()`, `make_and/or/xor()`, `NOT_WORD[]`, `NAND_WORD[]`
- **Literature Touched**: Convergence of topology and computation
- **Open Questions**: How does error behave away from delta=0? (→ Demo 20)

---

## Demo 20: Braid Error Landscape

- **Status**: Complete
- **File**: `demo_20_braid_error/main.c` (~1016 lines)
- **Tests**: ~15+ (delta landscape, single-gate error, cascade error, universal delta curve, phase transition)
- **Dependencies**: math.h
- **Headline**: |δ| = 2|cos(2θ)|; zeros at 8th roots of unity; sharp phase transition from universal to non-universal angles.
- **Key Results**:
  - **Part A**: Delta landscape — |δ| = 2|cos(2θ)|, analytically verified across 360 angles
  - Delta zeros at θ = π/4, 3π/4, 5π/4, 7π/4 (8th roots of unity)
  - **Part B**: Single-gate error curves — NOT/NAND separation vs |δ|
  - **Part C**: Cascade error — full adder fidelity vs |δ|; fidelity drops sharply away from δ=0
  - **Part D**: Universal delta curve — parameterize all gates by |δ|, overlay on single plot
  - **Part E**: Phase transition detection — threshold |δ| value where classification fails
  - |δ| cleanly separates universal from non-universal angles
- **Theorems/Conjectures**: |δ| is the single control parameter for braid computation quality; sharp phase transition at critical |δ|
- **Data**: 360 angle sweep, full adder at varying |δ|
- **Code Assets**: Delta formula verification, error curve computation, fidelity metrics
- **Literature Touched**: Error analysis in topological computation
- **Open Questions**: Can we do reversible computation? (→ Demo 21)

---

## Demo 21: Reversible Gates

- **Status**: Complete
- **File**: `demo_21_reversible_gates/main.c` (~857 lines)
- **Tests**: ~15+ (Toffoli, Fredkin, multi-readout, double-gate identity, efficiency)
- **Dependencies**: math.h
- **Headline**: Toffoli and Fredkin gates compiled from NAND/NOT expressions; multi-readout: different closures on same braid yield different Boolean functions.
- **Key Results**:
  - **Part A**: Toffoli gate — (A,B,C) → (A, B, C XOR (A AND B)); all 8 inputs verified
  - **Part B**: Fredkin gate — conditional swap via MUX: emux(sel,a,b) = NAND(NAND(sel,a), NAND(NOT(sel),b))
  - **Part C**: Multi-readout — 4-strand braids with 4 closure types (trace, plat_a, plat_b, plat_c)
    - CL_TRACE: standard trace closure
    - CL_PLAT_A: pairs 01|23
    - CL_PLAT_B: pairs 02|13
    - CL_PLAT_C: pairs 03|12
    - Different closures yield different Boolean functions from same braid
  - **Part D**: Double-Toffoli = identity, double-Fredkin = identity (reversibility verified)
  - **Part E**: Efficiency — Toffoli < 132 crossings (compare Bonesteel et al.)
- **Theorems/Conjectures**: Braid-bracket supports reversible computation; closure type = readout basis
- **Data**: 8-row truth tables for Toffoli/Fredkin, multi-readout catalog
- **Code Assets**: Closure type enum (CL_TRACE/PLAT_A/B/C), `eval_3bit_gate()`, MUX expression builder
- **Literature Touched**: Bonesteel et al. (Toffoli gate complexity), reversible computation
- **Open Questions**: What are the minimum braid complexities? (→ Demo 22)

---

## Demo 22: Minimum Braid Complexity

- **Status**: Complete
- **File**: `demo_22_braid_optimizer/main.c` (~975 lines)
- **Tests**: ~15+ (exhaustive search, TL_3 oracle, atlas, parity barrier, extended search)
- **Dependencies**: math.h
- **Headline**: "Atomic weights" for topological computation: minimum braid word length per truth table; TL_3 nilpotent oracle at δ=0.
- **Key Results**:
  - **Part A**: Exhaustive min-length search — 4 strands, lengths 1-5, all 4 closure types
  - **Part B**: TL_3 nilpotent oracle — 5x5 matrices at δ=0; e_i^2 = 0 (nilpotent!); basis {1, e1, e2, e1*e2, e2*e1}
  - Matrix equality implies bracket equality: 0 cross-validation errors
  - Distinct TL_3 matrices grow slower than 4^k (nilpotent compression)
  - **Part C**: Atlas — minimum braid word length for each of 256 truth tables
  - Named function lookup table (fn_name) covering ~40 Boolean functions
  - NAND expression cost comparison
  - **Parity barrier**: XOR-containing functions (0x96, 0x69) never found
  - **Part D**: Extended targeted search at length 6 (trace closure only)
- **Theorems/Conjectures**: TL_3 at δ=0 is nilpotent (dim 5); parity barrier for braid computation
- **Data**: Atlas of 256 truth tables with min lengths, TL_3 matrix catalog
- **Code Assets**: TL_3 5x5 matrix representation, named function table, atlas construction, NAND cost comparison
- **Literature Touched**: Temperley-Lieb nilpotency, Boolean function complexity
- **Open Questions**: Which Boolean functions does topology prefer? (→ Demo 23)

---

## Demo 23: Function Zoo

- **Status**: Complete
- **File**: `demo_23_function_zoo/main.c` (~770 lines)
- **Tests**: ~10+ (exhaustive catalog, NPN classification, closure tuples, extended search)
- **Dependencies**: math.h
- **Headline**: Exhaustive catalog of braid-computable Boolean functions; NPN equivalence classification; >80% of braids are closure-dependent.
- **Key Results**:
  - **Part A**: Exhaustive catalog — all braids len 1-4, all 4 closures
  - **Part B**: NPN equivalence — 6 perms × 8 input-neg masks × 2 output-neg = 96 transforms per class
  - 13 NPN equivalence classes for 3-input Boolean functions
  - **Part C**: Closure tuple structure — per-braid tuple of 4 truth tables (one per closure)
  - >80% of braids are closure-dependent (different closures → different functions)
  - **Part D**: Extended search at length 5 discovers new functions and NPN classes
  - Named function table covering ~40 named Boolean functions
  - Top 20 most common functions
- **Theorems/Conjectures**: Closure type is a genuine computational resource; most braids compute different functions under different closures
- **Data**: All braids len 1-5, 4 closure types, 256 possible truth tables, 13 NPN classes
- **Code Assets**: NPN transform enumeration, closure tuple storage, frequency analysis
- **Literature Touched**: NPN equivalence classes, Boolean function taxonomy
- **Open Questions**: How do rational tangles relate? (→ Demo 24)

---

## Demo 24: Rational Tangle Arithmetic

- **Status**: Complete
- **File**: `demo_24_rational_tangles/main.c` (~889 lines)
- **Tests**: ~15+ (integer tangles, twist, rotation, continued fractions, angle sweep, Fibonacci)
- **Dependencies**: math.h
- **Headline**: Conway's rational tangle operations (Twist, Rotate) in TL_2 representation; continued fraction assembly; |D(n)| = n at δ=0.
- **Key Results**:
  - TL_2 representation: TL2 struct {Cx alpha, Cx beta}
  - tl2_twist_pos/neg: r → r+1 or r-1
  - tl2_rotate: r → -1/r
  - Numerator closure: N(T) = alpha + beta*delta
  - Denominator closure: D(T) = alpha*delta + beta
  - **Integer tangles at δ=0**: alpha_n = A^n, beta_n = n*A^{n-2}, |D(n)| = n
  - Twist is multiplicative on alpha
  - **Continued fraction assembly**: inside-out construction from CF digits
  - |D|/|N| tracks rational p/q
  - Angle sweep for best Pearson correlation
  - Fibonacci tangles: F(n)/F(n-1) → golden ratio
  - State-sum cross-verification at multiple angles
- **Theorems/Conjectures**: Conway rational tangle theorem via TL_2; |D(n)| = n is exact at δ=0
- **Data**: Integer tangles n=0..10, continued fractions for various rationals, Fibonacci sequence
- **Code Assets**: `TL2` struct, `tl2_twist_pos/neg()`, `tl2_rotate()`, `tl2_num_closure()`, `tl2_den_closure()`, CF assembly
- **Literature Touched**: Conway (1970), rational tangle theory
- **Open Questions**: What does the full closure-measurement space look like? (→ Demo 25)

---

## Demo 25: Closure Measurement Matrices

- **Status**: Complete
- **File**: `demo_25_closure_measurement/main.c` (~907 lines)
- **Tests**: ~10+ (measurement matrices, SVD, constraint analysis, Hamming weight, parity barrier)
- **Dependencies**: math.h
- **Headline**: "Closure = measurement basis" made precise; 4×8 amplitude matrices per braid; all braids rank 4; parity barrier confirmed.
- **Key Results**:
  - MeasMatrix: 4×8 amplitude matrix M[closure][input] + thresholded truth tables
  - 1554 braids cataloged (lengths 1-4, 4 strands)
  - **Part B**: SVD via Jacobi eigenvalue algorithm (4×4 and 8×8 symmetric matrices)
    - **All 1554 braids have rank 4** — 4 closures are always linearly independent (full rank)
  - **Part C**: Constraint analysis + complementarity
    - Mutual information between closure pairs via hash-based quadruple counting
    - Hard constraints: tiny fraction of 2^32 possible quadruples are achievable
  - **Part D**: Hamming weight structure + parity test
    - ANOVA F-ratio for Hamming weight dependence
    - Monotonicity analysis
    - **Parity barrier confirmed**: 0x96 (XOR) and 0x69 (XNOR) never found in any closure
  - **Part E**: Closure subspace dimensions — 8×8 covariance, Jacobi eigenvalues
- **Theorems/Conjectures**: Closure measurement matrices are always full rank (4 closures = 4 independent measurements); parity functions unreachable at δ=0 with 4-strand braids
- **Data**: 1554 braids, 4×8 matrices each, SVD spectra, quadruple frequency counts
- **Code Assets**: `MeasMatrix`, Jacobi eigenvalue solver (4×4 and 8×8), hash-based counting, ANOVA F-ratio, `compute_mutual_info()`
- **Literature Touched**: Measurement theory, parity barrier in topological computation
- **Open Questions**: Can parity be reached with more sectors (k>4)? (→ Demo 50 answers YES at k=6)

---

## Summary Statistics

| Metric | Value |
|--------|-------|
| Demos in batch | 25 |
| Total lines of C | ~21,300 |
| Total tests (approx) | 300+ |
| External dependencies | math.h only (all demos) |
| Shared infrastructure | Cx (complex), Braid, union-find, state-sum bracket |
| Major transitions | Symbolic→Numerical (Demo 10), Classification→Computation (Demo 18), Flat→Hierarchical (Demo 17) |

### Arc Structure

**Phase 1 (Demos 01-09): Symbolic Foundations**
- Laurent polynomials → braid words → TL algebra → spin chain → scaling → Jones-Wenzl → braid↔PD bridge → Reidemeister invariance

**Phase 2 (Demos 10-17): Numerical DKC**
- Numerical engine → waveforms → quantization → DKC test → zero structure → angle optimization → reduced bracket → multi-layer hierarchy

**Phase 3 (Demos 18-25): Topological Computation**
- Braid logic gates → circuit compiler → error landscape → reversible gates → minimum complexity → function zoo → rational tangles → closure measurements
