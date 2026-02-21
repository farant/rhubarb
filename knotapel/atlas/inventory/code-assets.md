# Code Assets Catalog

Reusable code patterns across 59 knotapel demos. Updated 2026-02-21 (added Demos 64-71).

---

## 1. Polynomial / Ring Arithmetic

### 1.1 Laurent Polynomial Ring (Poly)

- **What it does**: Integer-coefficient Laurent polynomials (negative and positive exponents) with full ring operations — the symbolic backbone of Demos 01-09.
- **Introduced in**: Demo 01
- **Reused in**: Demos 02, 03, 04, 05, 06, 07, 08, 09
- **Key types/functions**:
  - `Poly` struct: `lo` (lowest exponent) + `coeffs[]` array, `MAX_TERMS=64` (120 in Demo 04+)
  - `p_zero()`, `p_mono(coeff, exp)`, `p_trim()` — constructors
  - `p_add()`, `p_mul()`, `p_eq()`, `p_print()` — core algebra
  - `p_neg()`, `p_sub()` — added Demo 06
  - `p_div_exact()` — exact polynomial division with remainder check (Demo 06)
- **Approximate size**: ~120 lines
- **Notes**: Copied verbatim between demos (no shared library extraction). MAX_TERMS varies (64 in early demos, 120 in Demo 04+). Replaced by numerical `Cx` type from Demo 10 onward for performance.

### 1.2 Complex Arithmetic (Cx)

- **What it does**: Double-precision complex number type with full arithmetic, unit-circle evaluation, and approximate equality — the numerical workhorse for all demos from 10 onward.
- **Introduced in**: Demo 10
- **Reused in**: Demos 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 35, 45, 46, 47, 48, 49, 50, 53, 54, 55, 56, 57, 58, 59, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71
- **Key types/functions**:
  - `Cx` struct: `re`, `im` doubles
  - `cx_make()`, `cx_zero()`, `cx_one()`, `cx_real()` — constructors
  - `cx_add()`, `cx_sub()`, `cx_mul()`, `cx_div()`, `cx_neg()` — ring operations
  - `cx_abs()`, `cx_abs2()` — magnitude
  - `cx_exp_i(theta)` — unit circle point
  - `cx_pow_int(z, n)` — binary exponentiation (handles negative n)
  - `cx_near(a, b, tol)` — approximate equality
  - `cx_phase(z)` — argument in [0, 2pi)
  - `cx_scale(z, r)` — real scalar multiplication
  - `cx_print()` — formatted output
- **Approximate size**: ~100 lines
- **Notes**: 16 bytes per entry (vs ~808 bytes for Poly) — 50x memory reduction. Used as the float companion to every exact cyclotomic ring.

### 1.3 Cyclotomic Z[zeta_8] (Cyc8)

- **What it does**: Exact integer arithmetic in the 8th cyclotomic ring — basis {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1. The natural home for bracket values at delta=0 (A = e^{i*5pi/4}).
- **Introduced in**: Demo 29
- **Reused in**: Demos 35, 48, 49, 50, 63, 64
- **Key types/functions**:
  - `Cyc8` struct: 4 `long` coefficients `(a, b, c, d)`
  - `cyc8_make()`, `cyc8_zero()`, `cyc8_one()` — constructors
  - `cyc8_add()`, `cyc8_sub()`, `cyc8_neg()`, `cyc8_mul()`, `cyc8_scale()` — ring operations
  - `cyc8_mul()`: 16 integer muls + 12 adds, zero floating-point
  - `cyc8_eq()` — exact equality
  - `cyc8_conj()` — complex conjugation: conj(a,b,c,d) = (a,-d,-c,-b); doubles as inverse for units
  - `cyc8_pow_int(z, n)` — positive and negative exponents (via conjugate for negative)
  - `cyc8_to_cx()` — convert to float for verification
  - `cyc8_is_axial()` — tests single-component property (at most one nonzero coordinate)
  - `cyc8_print()` — formatted output
- **Approximate size**: ~200 lines
- **Notes**: All bracket values at A = -zeta_8 are cyclotomic integers in this ring. The delta=0 condition (A^2 + A^{-2} = 0) means only single-loop smoothing states survive, massively simplifying exact computation. Reused in Demo 64 for matroid deletion-contraction verification — exact arithmetic is critical to eliminate floating-point association-order failures near sector boundaries (420 spurious failures eliminated).

### 1.4 Eisenstein Integers Z[omega] (Eis)

- **What it does**: Exact arithmetic in the ring of Eisenstein integers — basis {1, omega} where omega = e^{i*2pi/3} is a cube root of unity, omega^2 + omega + 1 = 0.
- **Introduced in**: Demo 53
- **Reused in**: (Demo 53 only — ring-specific to ell=3/delta=1)
- **Key types/functions**:
  - `Eis` struct: 2 integer coefficients `(a, b)` representing a + b*omega
  - `eis_add()`, `eis_sub()`, `eis_neg()`, `eis_mul()` — ring operations
  - `eis_conj()` — conjugation, norm = a^2 - ab + b^2
  - `eis_a_power(n)` — A^n where A = omega, cycling mod 3
  - `eis_is_axial()` — tests if value is a scalar multiple of {1, omega, omega^2}
  - `eis_to_cx()` — convert to float
- **Approximate size**: ~80 lines
- **Notes**: 2-dimensional over Z (simplest non-trivial cyclotomic ring). At delta=1, TL generators are idempotent (e_i^2 = e_i) and all smoothing states contribute.

### 1.5 Cyclotomic Z[zeta_16] (Cyc16)

- **What it does**: Exact 8-dimensional cyclotomic integer arithmetic for bracket evaluation at ell=4 (delta=sqrt(2), Ising anyons).
- **Introduced in**: Demo 54
- **Reused in**: Demos 55, 56, 57, 65
- **Key types/functions**:
  - `Cyc16` struct: 8 `long` coefficients, basis {1, zeta_16, ..., zeta_16^7}, zeta^8 = -1
  - `cyc16_zero()`, `cyc16_one()`, `cyc16_make()`, `cyc16_zeta_power()` — constructors
  - `cyc16_add()`, `cyc16_neg()`, `cyc16_scale()`, `cyc16_mul()` — ring operations
  - `cyc16_eq()`, `cyc16_is_zero()` — comparison
  - `cyc16_a_power(n)` — A^k = zeta^{5k} with reduction (order 16)
  - `cyc16_delta_power(k)` — delta^k where delta=sqrt(2): even powers → integer (2^{k/2}), odd → integer*delta
  - `cyc16_is_axial()` — strict axiality (1 nonzero component)
  - `cyc16_is_zi_axial()` — Z[i]-axiality (at most one active pair (k, k+4))
  - `cyc16_zi_decompose()` — factors as (a+bi)*zeta^c
  - `cyc16_to_cx()`, `cyc16_print()` — output
  - `cyc16_is_real()` — tests zero imaginary part (odd components = 0)
  - `cyc16_is_zeta8_valued()` — tests Z[zeta_8] subring membership (even-indexed only)
- **Approximate size**: ~250 lines
- **Notes**: A = zeta_16^5, delta = zeta^2 - zeta^6 = sqrt(2). Generators are delta-potent (e_i^2 = sqrt(2)*e_i). Contains Z[zeta_8] and Z[i] as subrings.

### 1.6 Cyclotomic Z[zeta_5] (Cyc5)

- **What it does**: Exact 4-dimensional cyclotomic arithmetic for the Fibonacci anyon evaluation level (ell=5, delta=phi=golden ratio).
- **Introduced in**: Demo 58
- **Reused in**: (Demo 58 only — ring-specific to ell=5)
- **Key types/functions**:
  - `Cyc5` struct: 4 `long` coefficients, basis {1, zeta_5, zeta_5^2, zeta_5^3}
  - Reduction: zeta_5^4 = -(1 + zeta_5 + zeta_5^2 + zeta_5^3)
  - `cyc5_zero()`, `cyc5_one()`, `cyc5_add()`, `cyc5_neg()`, `cyc5_mul()` — ring operations
  - `cyc5_a_power(n)` — A = (1,1,1,1) = -zeta_5^4 = zeta_10^3, order 10; precomputed `A_table[10]`
  - `cyc5_delta_power(k)` — delta^k via Fibonacci: phi^k = F(k-1) + F(k)*phi
  - `cyc5_to_cx()`, `cyc5_print()` — output
- **Approximate size**: ~150 lines
- **Notes**: Delta = phi = (1+sqrt(5))/2 — Fibonacci numbers appear as delta power coefficients. Coefficients grow with braid length (unlike bounded coefficients at delta=0).

### 1.7 Cyclotomic Z[zeta_24] (Cyc24)

- **What it does**: Exact 8-dimensional cyclotomic arithmetic for Q=3 Potts model evaluation (ell=6, delta=sqrt(3)).
- **Introduced in**: Demo 59
- **Reused in**: (Demo 59 only — ring-specific to ell=6)
- **Key types/functions**:
  - `Cyc24` struct: 8 `long` coefficients, basis {1, zeta_24, ..., zeta_24^7}
  - Reduction: Phi_24(x) = x^8 - x^4 + 1, so zeta^8 = zeta^4 - 1
  - `cyc24_zero()`, `cyc24_one()`, `cyc24_add()`, `cyc24_neg()`, `cyc24_mul()` — ring operations
  - `A_table[24]` — precomputed A^k for k=0..23 (A = zeta_24^7, order 24)
  - `cyc24_delta_power(k)` — delta^k where delta=sqrt(3): delta^{2k}=3^k, delta^{2k+1}=3^k*sqrt(3)
  - `cyc24_to_cx()`, `cyc24_print()` — output
- **Approximate size**: ~200 lines
- **Notes**: Third independent ring confirming 11/13 Re>0 wall. Q = delta^2 = 3 connects to 3-state Potts model.

### 1.8 Gaussian Integer Tracking (GaussInt)

- **What it does**: Lightweight Gaussian integer Z[i] catalog with negation/conjugation pair detection.
- **Introduced in**: Demo 54 (Experiment 5)
- **Reused in**: (Demo 54 only)
- **Key types/functions**:
  - `GaussInt` struct: `a`, `b` integers (a + bi)
  - `gi_catalog_contains()` — deduplication
  - Negation pair / conjugation pair tracking
- **Approximate size**: ~30 lines
- **Notes**: Z[i] appears as a subring of Z[zeta_16] (via zeta^4 = i). Used to strip the zeta^c axis from Z[i]-axial bracket values.

### 1.9 Cubic Extension Field Arithmetic (F_p[x]/(f))

- **What it does**: Polynomial arithmetic over F_p modulo an irreducible cubic — enables exact computation in cubic number fields like Q(2cos(pi/7)) without floating point.
- **Introduced in**: Demo 39 (specialized for ell=7 cubic root finding)
- **Reused in**: Demo 60 (full cubic extension field TL analysis)
- **Key types/functions**:
  - `Poly3` type (Demo 39) / inline polynomial arrays (Demo 60): coefficients of degree <= 2 polynomials in F_p[x]/(x^3 - x^2 - 2x + 1)
  - `poly_mul()`, `poly_pow()` — multiplication with reduction mod f(x), repeated-squaring exponentiation
  - `pdeg()`, `pmod()`, `pgcd()` — degree, polynomial reduction, polynomial GCD over F_p
  - `p3_mul()`, `p3_pow()` (Demo 39) — specialized Poly3 operations
  - Reduction rule: x^3 = x^2 + 2x - 1, x^4 = 3x^2 + x - 1
- **Approximate size**: ~100 lines
- **Notes**: First non-quadratic number field in the Chebyshev parameter hierarchy. The minimal polynomial x^3 - x^2 - 2x + 1 for 2cos(pi/7) is well-known in algebraic number theory.

---

## 2. Knot / Braid Computation

### 2.1 PD Notation & State-Sum Bracket (Symbolic)

- **What it does**: Represents knots via planar diagram (PD) notation and computes Kauffman brackets by brute-force state-sum over all 2^n crossing resolutions using symbolic Laurent polynomials.
- **Introduced in**: Demo 01
- **Reused in**: Demos 02, 07, 08, 09
- **Key types/functions**:
  - `Knot` struct: array of `Xing` (crossing) structs with arc labels and signs, MAX_X=16
  - `Xing` struct: 4 arc labels + crossing sign
  - `kauffman_bracket(knot)` — O(2^n) state-sum over crossing resolutions
  - `count_loops(knot, state)` — traces cycles through arc connectivity for a given resolution state
  - `jones_from_bracket(knot)` — writhe normalization to get f-polynomial
  - `make_trefoil()`, `make_figure_eight()`, `make_hopf()` — standard PD constructors
- **Approximate size**: ~200 lines
- **Notes**: The ground-truth oracle for all later demos. Limited to ~16 crossings by exponential state enumeration.

### 2.2 Union-Find Loop Counter

- **What it does**: Path-compressed union-find for counting connected components (loops) in resolved knot/braid diagrams. The most-reused utility across all demos.
- **Introduced in**: Demo 02
- **Reused in**: Demos 03, 04, 05, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 45, 46, 47, 48, 49, 50, 53, 54, 55, 56, 57, 58, 59, 64, 65, 66, 67, 68, 69, 70, 71
- **Key types/functions**:
  - `uf_parent[]` — global or local parent array
  - `uf_init(n)`, `uf_find(x)`, `uf_union(x, y)` — standard union-find with path compression
  - `braid_loops(word, len, n_strands)` / `braid_count_loops()` — counts loops in resolved braid closure
- **Approximate size**: ~40 lines
- **Notes**: Path-halving variant in later demos. Used in every state-sum bracket computation. Sometimes local arrays, sometimes global.

### 2.3 Braid Word State-Sum Bracket (Numerical)

- **What it does**: Evaluates Kauffman bracket from braid word via O(2^n) state-sum, using either symbolic polynomials or numerical complex evaluation at a specific angle.
- **Introduced in**: Demo 02 (symbolic), Demo 10 (numerical)
- **Reused in**: Nearly every demo from 02 onward
- **Key types/functions**:
  - `Braid` struct: `word[]` (signed generator indices), `len`, `n` (strand count)
  - `braid_bracket(word, len, n)` — symbolic state-sum (Poly result)
  - `braid_bracket_at(word, len, n, A)` — numerical state-sum at complex A (Cx result)
  - `braid_writhe()`, `braid_jones()` — writhe computation and normalization
- **Approximate size**: ~100 lines (numerical version)
- **Notes**: The numerical version (`braid_bracket_at`) is the primary validation oracle for all later demos. Evaluates at specific A = e^{i*theta} on the unit circle.

### 2.4 Braid-to-PD Bridge

- **What it does**: Converts braid words to PD notation via strand closure, enabling cross-validation between the two computation worlds.
- **Introduced in**: Demo 07
- **Reused in**: Demos 08, 09
- **Key types/functions**:
  - `braid_to_pd(braid)` — core bridge: tracks per-position crossing lists, assigns arc labels, builds PD with correct over/under conventions
  - `seifert_circles(knot)` — Seifert resolution on PD notation (sign-dependent pairing)
  - `link_components(knot)` — traces strands via (p+2)%4 exit rule
- **Approximate size**: ~200 lines
- **Notes**: Confirmed: braid closure PD bracket = braid state-sum bracket for all tested knots.

### 2.5 PD-to-Braid Compiler (Vogel's Algorithm)

- **What it does**: Compiles PD notation to braid word via Seifert circle decomposition — the reverse direction of Demo 07's bridge.
- **Introduced in**: Demo 08
- **Reused in**: (Demo 08 + round-trip tests)
- **Key types/functions**:
  - `pd_to_braid()` — full 7-step compiler (Seifert membership → crossing-circle map → Seifert graph → circle ordering → crossing trace → braid ordering → emit)
  - `seifert_membership()` — union-find Seifert resolution
  - `trace_circles()` — cyclic crossing order per Seifert circle
  - `find_braid_order()` — topological sort for braid word ordering
- **Approximate size**: ~250 lines
- **Notes**: Only handles path-graph Seifert graphs. Full Vogel moves for non-path graphs not implemented.

### 2.6 Reidemeister Move Infrastructure

- **What it does**: Applies R1 (twist/kink) and R2 (poke) moves to PD notation and verifies bracket/Jones invariance computationally.
- **Introduced in**: Demo 09
- **Reused in**: (Demo 09 only)
- **Key types/functions**:
  - `writhe(knot)` — sum of crossing signs
  - `jones_poly(knot)` — (-A^3)^{-w} * bracket(K)
  - `apply_r1(knot, edge, sign)` — adds kink to specified edge
  - `apply_r2(knot, edge1, edge2)` — adds opposite-sign crossing pair
  - `remap_second()` — edge splitting utility
- **Approximate size**: ~150 lines

### 2.7 Numerical Bracket Engine (Matrix Method)

- **What it does**: Computes Kauffman bracket via tensor product R-matrix representation and quantum trace — the scalable alternative to state-sum for large strand counts.
- **Introduced in**: Demo 10
- **Reused in**: Demos 11-17 (via numerical_bracket), indirectly by all later demos using the unit-circle evaluation pattern
- **Key types/functions**:
  - `CMat` struct: heap-allocated NxN complex matrix
  - `CMAT(m, i, j)` macro for row-major access
  - `cmat_alloc()`, `cmat_free()`, `cmat_zero()`, `cmat_id()`, `cmat_mul()`, `cmat_scale()`, `cmat_add()`, `cmat_qtrace()`, `cmat_tensor()`, `cmat_eq_approx()`
  - `build_R4()` — 4x4 R-matrix and inverse for given A
  - `build_Ri(R4, i, n)` — I^{x(i-1)} x R x I^{x(n-i-1)} via tensor products
  - `build_Kn(n)` — K^{xn} diagonal weights for quantum trace
  - `braid_product_n(word, len, n, A)` — braid word to matrix product in 2^n dimensions
  - `numerical_bracket(word, len, n, A)` — full pipeline: braid → product → qtrace → sign correction
- **Approximate size**: ~300 lines
- **Notes**: Scales to n=8 (256x256). Memory is ~50MB per matrix at n=6. Sign convention: (-1)^{n+1} correction.

### 2.8 Bracket with Closure Support

- **What it does**: Extends the state-sum bracket to support 4 different closure types (trace, plat_A, plat_B, plat_C), enabling multi-readout experiments from a single braid.
- **Introduced in**: Demo 21
- **Reused in**: Demos 22, 23, 25
- **Key types/functions**:
  - `Closure` enum: `CL_TRACE`, `CL_PLAT_A` (01|23), `CL_PLAT_B` (02|13), `CL_PLAT_C` (03|12)
  - `bracket_cl(word, len, n, A, closure)` — state-sum bracket with configurable closure
  - `braid_loops_cl(word, len, n, state, closure)` — loop counting with 4 closure types
- **Approximate size**: ~100 lines
- **Notes**: Key discovery: different closures of the same braid typically yield different Boolean functions (>=80% closure-dependent). Parity function (0x96/0x69) found zero times across all braids and closures.

### 2.9 Exact Bracket at delta=0 (Cyc8)

- **What it does**: Computes exact Kauffman bracket in Z[zeta_8] arithmetic, exploiting delta=0 simplification where only single-loop smoothing states survive.
- **Introduced in**: Demo 29
- **Reused in**: Demos 35, 48, 49, 50
- **Key types/functions**:
  - `braid_bracket_exact(word, len, n)` — exact state-sum in Cyc8 at A = -zeta_8; skips all multi-loop states
- **Approximate size**: ~50 lines
- **Notes**: Critical optimization: at delta=0, delta^{loops-1} = 0 for loops > 1, so only single-loop states contribute. This makes exact computation feasible.

### 2.10 Exact Bracket at General Delta (Ring-Parametric)

- **What it does**: State-sum bracket evaluation using exact cyclotomic arithmetic at various evaluation levels where delta != 0.
- **Introduced in**: Demo 53 (Eis/delta=1), Demo 54 (Cyc16/delta=sqrt(2)), Demo 58 (Cyc5/delta=phi), Demo 59 (Cyc24/delta=sqrt(3))
- **Key types/functions**:
  - `state_sum_bracket_d1()` — delta=1, all states contribute equally (delta^{L-1} = 1)
  - `state_sum_bracket()` at Cyc16 — delta=sqrt(2), weights by delta^{loops-1} using exact delta powers
  - `state_sum_bracket()` at Cyc5 — delta=phi, weights via Fibonacci numbers
  - `state_sum_bracket()` at Cyc24 — delta=sqrt(3), weights via 3^k scaling
- **Approximate size**: ~80 lines per ring
- **Notes**: All use planar diagram composition instead of union-find for loop counting. Cross-validated against float evaluation.

### 2.11 Dual Bracket (Standard + Neglecton)

- **What it does**: Evaluates both the standard bracket (loops=1 states) and the first-order neglecton bracket (4 x loops=2 states) simultaneously, for investigating TL_4 radical contributions.
- **Introduced in**: Demo 49
- **Reused in**: (Demo 49 only)
- **Key types/functions**:
  - `DualBracket` struct: paired standard + neglecton Cyc8 values
  - `braid_bracket_dual(word, len, n)` — evaluates BOTH brackets in exact arithmetic, skipping loops >= 3
- **Approximate size**: ~80 lines
- **Notes**: Neglecton bracket = 4 * Sum_{states with loops=2} A^{a-b}. Scale factor 4 = d(delta)/d(epsilon). Found 148 new values from 15,242 previously invisible braids, but all share angles with standard catalog values.

### 2.12 Braid Word Decoder/Enumerator

- **What it does**: Converts integer indices to braid crossing sequences for exhaustive enumeration of all braids up to a given length.
- **Introduced in**: Demo 22
- **Reused in**: Demos 23, 25, 35, 48, 49, 50, 53, 54, 55, 56, 57, 58, 59, 64, 65, 66, 67, 68, 69, 70, 71
- **Key types/functions**:
  - `decode_word(code, len, word)` — maps integer to braid word using generator map
  - `decode_word3()` — 3-strand variant (4 generators: {1,-1,2,-2})
  - `decode_word4()` — 4-strand variant (6 generators: {1,-1,2,-2,3,-3})
  - `decode_braid()` — general version for n strands
  - `gen_map[]` — generator alphabet array
- **Approximate size**: ~25 lines
- **Notes**: Search spaces: 4^k for 3-strand, 6^k for 4-strand. Used in every exhaustive braid enumeration.

### 2.13 Knot Table Builder

- **What it does**: Constructs collections of knots (torus knots, connected sums, mirrors) for classification experiments with deduplication.
- **Introduced in**: Demo 13
- **Reused in**: Demos 14, 15, 16, 17
- **Key types/functions**:
  - `add_torus_2n(table, n, sign)` — generates T(2,n) torus knot braid word
  - `add_connected_sum(table, k1, k2)` — concatenates braid words for connected sum
  - `dedup_knots(table)` — removes duplicates by bracket comparison at 2 reference angles
  - `init_knots(table)` — full table initialization
- **Approximate size**: ~100 lines
- **Notes**: Dedup finds interesting equivalences (e.g., T23#fig8 == T(2,5)*). Braid concatenation != diagram connected sum (factorization fails).

---

## 3. TL Algebra

### 3.1 Planar Matching Enumeration

- **What it does**: Recursively enumerates non-crossing perfect matchings (planar diagrams) on 2n boundary points — the basis for Temperley-Lieb algebra representations.
- **Introduced in**: Demo 35
- **Reused in**: Demos 38, 39, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
- **Key types/functions**:
  - `PlanarMatch` struct: array of pairs, each matching endpoint a with endpoint b
  - `enumerate_basis(n, basis, count)` — segment-based recursive algorithm
  - `enum_segments()`, `build_boundary_order()` — internal recursion helpers
  - Produces Catalan numbers: C_2=2, C_3=5, C_4=14, C_5=42, C_6=132, C_7=429, C_8=1430
- **Approximate size**: ~80 lines
- **Notes**: Ring-independent — pure combinatorics. Used identically across all cyclotomic ring demos and all radical analysis demos. Supports n up to 8 (C_8=1430 basis elements).

### 3.2 Diagram Composition

- **What it does**: Stacks two TL diagrams vertically, connects matching endpoints at the glue, and counts closed loops formed at the junction.
- **Introduced in**: Demo 35
- **Reused in**: Demos 38, 39, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60
- **Key types/functions**:
  - `compose_diagrams(d1, d2, n, result, loops)` — composes two PlanarMatch diagrams, returns resulting diagram + loop count
  - `find_basis_index()` — lookup composed result in enumerated basis
- **Approximate size**: ~50 lines
- **Notes**: The loop count determines the delta^loops coefficient. At delta=0, any composition producing loops > 0 contributes zero.

### 3.3 TL Matrix Algebra (Ring-Parametric)

- **What it does**: Matrix algebra over TL_n basis elements with coefficients in various cyclotomic rings.
- **Introduced in**: Demo 35 (Cyc8)
- **Reused in**: Demos 51 (integer), 52 (mod-p), 53 (Eis), 54 (Cyc16), 55, 58 (Cyc5), 59 (Cyc24)
- **Key types/functions**:
  - `TLMatrix` / `TLMat` type: up to C_n x C_n matrix over the coefficient ring
  - `tlmat_zero()`, `tlmat_identity()` — constructors
  - `tlmat_add()`, `tlmat_scale()`, `tlmat_mul()` — algebra operations (mul uses loop-weighted delta^k)
  - `tlmat_eq()`, `tlmat_is_zero()`, `tlmat_nonzero_count()`, `tlmat_print()`
- **Approximate size**: ~200 lines per ring variant
- **Notes**: Matrix dimensions are Catalan numbers. The multiplication kernel is identical across rings; only the coefficient arithmetic changes. At n=5, matrices are 42x42.

### 3.4 TL Generator & Braid Generator Builders

- **What it does**: Constructs TL generator matrices e_i and braid generator matrices sigma_i = A*I + A^{-1}*e_i in the planar matching basis.
- **Introduced in**: Demo 35
- **Reused in**: Demos 51, 52, 53, 54, 55, 58, 59
- **Key types/functions**:
  - `build_generator_matrix(basis, dim, i, n, delta)` — constructs e_i via diagram composition
  - `build_braid_generator(basis, dim, i, n, A)` — sigma_i = A*I + A^{-1}*e_i
  - `eval_braid_word(word, len, generators, dim)` — multiplies generator matrices for arbitrary crossing sequences
- **Approximate size**: ~80 lines
- **Notes**: At delta=0: e_i^2 = 0 (nilpotent). At delta=1: e_i^2 = e_i (idempotent). At delta=sqrt(2): e_i^2 = sqrt(2)*e_i (delta-potent).

### 3.5 Trace Closure Functional

- **What it does**: Extracts the Kauffman bracket from a TL matrix representation via the trace closure operation.
- **Introduced in**: Demo 35
- **Reused in**: Demos 51, 52, 53, 54, 55, 58, 59
- **Key types/functions**:
  - `compute_bracket_trace(matrix, basis, dim, n, delta)` — sums identity column with delta^{trace_loops-1} weights
  - `trace_closure_loops(diagram, n)` — counts loops in trace closure of a planar matching diagram
- **Approximate size**: ~40 lines
- **Notes**: Cross-validated against state-sum bracket for every ring.

### 3.6 TL_3 5x5 Representation (at delta=0)

- **What it does**: Hardcoded 5x5 TL_3 representation for fast braid evaluation and nilpotent compression at delta=0.
- **Introduced in**: Demo 22
- **Reused in**: (Demo 22 only — superseded by general TL_n from Demo 35)
- **Key types/functions**:
  - `Mat5` type: 5x5 complex matrix
  - `mat5_zero()`, `mat5_id()`, `mat5_scale()`, `mat5_add()`, `mat5_mul()`, `mat5_eq()`
  - `tl3_init(A)` — builds e1, e2 and sigma matrices
  - `tl3_braid_matrix(word, len, A)` — evaluates braid as TL_3 matrix product
- **Approximate size**: ~100 lines
- **Notes**: TL_3 at delta=0 is SEMISIMPLE (radical dim = 0), an anomaly — most TL_n at delta=0 are non-semisimple. Used to prove nilpotent compression: distinct TL_3 matrices grow subexponentially vs 4^k braids.

### 3.7 Polynomial Matrix Algebra (Mat, Mat2, Mat4)

- **What it does**: Matrix algebra over Laurent polynomials for symbolic tensor product, quantum trace, and Yang-Baxter verification.
- **Introduced in**: Demo 03 (Mat2 2x2), Demo 03 (Mat4 4x4), Demo 04 (Mat NxN up to 8x8)
- **Reused in**: Demos 04, 05, 06
- **Key types/functions**:
  - `Mat2` (2x2): identity, multiply, scale, add, trace, equality, print
  - `Mat4` (4x4): zero, multiply, equality, tensor product
  - `Mat` (NxN, up to 8x8 or heap-allocated):
    - `mat_alloc()`, `mat_free()` — heap allocation (Demo 05+)
    - `mat_zero()`, `mat_id()`, `mat_mul()`, `mat_scale()`, `mat_add()`, `mat_trace()`, `mat_eq()`
    - `mat_tensor()` — general Kronecker product
    - `mat_qtrace()` — quantum trace with diagonal weights
    - `MAT(m, i, j)` macro for row-major access
  - `braid_product()` — maps braid word to matrix product
  - `build_Ri()` — tensor product embedding of R-matrix for n-strand evaluation
  - `build_Kn()` — K^{tensor n} weights
- **Approximate size**: ~300 lines (cumulative)
- **Notes**: Superseded by numerical CMat from Demo 10 for all practical computation, but the symbolic version proved key identities (Yang-Baxter, quantum trace formula).

### 3.8 Jones-Wenzl Idempotents

- **What it does**: Constructs scaled Jones-Wenzl idempotents p_2 (4x4) and p_3 (8x8) over Laurent polynomials, avoiding rational functions via scaling trick.
- **Introduced in**: Demo 06
- **Reused in**: (Demo 06 only)
- **Key types/functions**:
  - Scaled JW recursion: Q_n = D_n * p_n (integer coefficients only)
  - Quantum dimension computation via Chebyshev recurrence: [n+1]_q = [2]_q * [n]_q - [n-1]_q
  - Markov trace decomposition verification
- **Approximate size**: ~100 lines
- **Notes**: Key simplification: [2]_q/delta = -1 at delta = -(A^2 + A^{-2}).

---

## 4. Radical / Representation Theory

### 4.1 TL Algebra Full Structure (Integer Arithmetic)

- **What it does**: Computes the complete algebraic structure of TL_n(delta=0) — multiplication table, radical, radical filtration, orthogonal idempotents, and Peirce decomposition — using exact integer arithmetic.
- **Introduced in**: Demo 51
- **Reused in**: Demo 52 (extended to general delta via modular arithmetic), Demo 60 (TLAlgebra struct and init_tl() reused for cubic field analysis)
- **Key types/functions**:
  - `TLAlgebra` struct: basis, identity/generator indices, full multiplication table
  - `AlgElem` type: integer-coefficient algebra elements
  - `init_tl_algebra(n)` / `init_tl(n)` — full initialization (enumerate, find identity/generators, build multiplication table)
  - `alg_mul(a, b, algebra)` — multiplication via precomputed table
  - `compute_radical_dim(algebra)` — trace form Gram matrix + Gaussian elimination for rank
  - `extract_radical_basis(algebra)` — null space of Gram matrix via augmented row reduction
  - `radical_filtration(algebra)` — computes rad^1, rad^2, ... dimensions until zero
  - `compute_next_radical_power()` — generates rad^{k+1} from rad^k x rad products
  - `find_gen_product_idempotents()` — searches length-2 products for idempotents
  - `build_orthogonal_set()` — greedy maximal orthogonal idempotent set
  - `compute_peirce_dim()` — Peirce block dimensions f_i * A * f_j via rank computation
  - `try_add_to_echelon()` — incremental echelon basis for independence testing
- **Approximate size**: ~600 lines
- **Notes**: Key discovery: TL_3(delta=0) is SEMISIMPLE despite nilpotent generators (radical dim = 0). TL_2 is NOT semisimple (dual numbers). TL_4+ have non-trivial radicals.

### 4.2 General-Delta TL Analysis (Modular Arithmetic)

- **What it does**: Extends radical anatomy to arbitrary delta values using modular arithmetic (mod large prime) for exact computation.
- **Introduced in**: Demo 52
- **Reused in**: (Demo 52 only)
- **Key types/functions**:
  - `analyze_tl_delta(n, delta_mod_p, prime)` — full analysis at arbitrary delta
  - Weighted Gram matrix with delta^loops weighting (not just 0/1 for delta=0)
  - Gaussian elimination mod p for exact radical dimension
  - Radical filtration and Peirce decomposition at general delta
- **Approximate size**: ~300 lines
- **Notes**: Tests 5 delta values: 0, 1, sqrt(2), phi, sqrt(3) for n=2..8. Reveals how semisimplicity varies across the Chebyshev parameter space.

### 4.3 Modular Arithmetic Utilities

- **What it does**: Prime-field arithmetic needed for exact computation at algebraic delta values.
- **Introduced in**: Demo 52
- **Reused in**: Demos 38, 39, 60
- **Key types/functions**:
  - `mod_reduce(x, p)` — reduction to [0, p)
  - `mod_inv(x, p)` — modular inverse via extended Euclidean algorithm
  - `mod_pow(base, exp, p)` — modular exponentiation
  - `mod_sqrt_ts(n, p)` / `mod_sqrt()` — Tonelli-Shanks algorithm for modular square roots
- **Approximate size**: ~100 lines
- **Notes**: Default prime p = 10^9+7. Separate prime (999999751) used for phi/sqrt(5). Demo 60 uses two independent primes (10^9+7 and 10^9+9) for cross-validation of cubic field results.

### 4.4 Gaussian Elimination / Gram Matrix

- **What it does**: Computes rank of trace form Gram matrix to determine radical dimension — the core tool for semisimplicity detection.
- **Introduced in**: Demo 51
- **Reused in**: Demos 38, 39, 52, 60
- **Key types/functions**:
  - Gram matrix construction from multiplication table + trace form fixed points
  - `compute_fixpt()` — precomputes sum of delta^loops over elements p such that result(p,k)=k, enabling efficient Gram matrix construction (Demo 39, 60)
  - Float-path Gaussian elimination (for small dims, gives integer vectors)
  - Mod-p Gaussian elimination (exact for any dim)
  - `compute_rank()` / `matrix_rank_modp()` — forward elimination rank computation mod p (Demo 39, 60)
  - Null space extraction for radical basis
  - Augmented [G|I] row reduction for simultaneous radical basis and projection extraction (Demo 38)
- **Approximate size**: ~150 lines
- **Notes**: Radical dimension = algebra dimension - rank(Gram). All nonzero trace form fixed-point values are Catalan numbers.

### 4.5 Dimension Reduction at delta=0 (Quotient Algebra)

- **What it does**: Proves and exploits the fact that at delta=0, TL_n(0)/rad is isomorphic to TL_{n-1}(0) for even n, yielding asymptotic ~4x dimension reduction (~64x speedup for O(dim^3) matrix operations).
- **Introduced in**: Demo 38
- **Reused in**: (Demo 38 only — feeds into Demo 45-50 DKC research arc)
- **Key types/functions**:
  - `count_through_strands()` — counts propagating (top-to-bottom matched) strands in a planar matching diagram; classifies basis elements by through-strand sector
  - `sector_gram_rank()` — extracts per-sector Gram submatrix and computes rank independently
  - `build_reduced_gen()` — projects TL generators onto the quotient basis using RREF pivot columns, yielding C_{n-1} x C_{n-1} reduced generators
  - `verify_reduced_tl_relations()` — checks nilpotency (e_i^2=0), adjacent relation (e_i*e_{i+1}*e_i=e_i), and far commutativity on reduced generators
  - `quotient_nonzero_products()` — counts nonzero products in the quotient multiplication table, verifying structural isomorphism with TL_{n-1}
  - Modular RREF with augmented [G|I] matrix — full row reduction over Z/pZ, tracks pivot columns, extracts radical basis from null rows and projection from RREF columns
  - Flat matrix algebra mod p: `mat_zero`, `mat_mul`, `mat_eq`, `mat_is_zero` over long integers mod prime
- **Approximate size**: ~781 lines (full demo)
- **Notes**: Even/odd dichotomy: odd n is semisimple (full rank C_n), even n has Gram rank C_{n-1}. Reduction factors: 2.00x (n=2), 2.80x (n=4), 3.14x (n=6), 3.33x (n=8), approaching 4x asymptotically. Through-strand sector j=0 is entirely DEAD (radical) at even n. Verified n=2 through n=8.

### 4.6 Symmetry Decomposition (Cellular Structure)

- **What it does**: Decomposes TL_n by through-strand count into cellular sectors, verifying sector sizes match cell module dimension formula d(n,j)^2, computing per-sector Gram ranks at multiple delta values, proving cell filtration monotonicity, and mapping the semisimplicity boundary.
- **Introduced in**: Demo 39
- **Reused in**: (Demo 39 only — provides algebraic explanation of DKC parity wall)
- **Key types/functions**:
  - `cell_module_dim(n, j)` — computes d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1), the Graham-Lehrer cell module dimension formula
  - `classify_sectors()` — partitions basis elements by through-strand count, returns sector j-values, counts, and number of distinct sectors
  - `build_gram_sub()` — constructs the bilinear form restricted to a subset of basis elements, using fixpoint table and delta powers
  - `compute_fixpt()` — precomputes sum of delta^loops over elements p such that result(p,k)=k, enabling efficient Gram matrix construction
  - Cell filtration verifier: exhaustively checks ts(a*b) <= min(ts(a), ts(b)) for all diagram pairs
  - Generator block structure analyzer: checks block-lower-triangular vs block-diagonal form in sector-ordered basis
  - Multi-delta comparison: tests ranks at delta=0, 1, sqrt(2), phi simultaneously
  - `find_l7_root()` — Cantor-Zassenhaus style cubic root finder for 2cos(pi/7) (minimal polynomial x^3-x^2-2x+1) over F_p
- **Approximate size**: ~1072 lines (full demo)
- **Notes**: Sector sizes verified: |V_j| = d(n,j)^2 for n=2..8. Cross-sector coupling grows: 0 (n=4), 4 (n=6), 104 (n=8). Critical finding: semisimple algebras are NOT block-diagonal in the standard basis — true change-of-basis needed. Graham-Lehrer semisimplicity boundary confirmed: TL_n semisimple iff n < ell.

### 4.7 Cubic Number Field TL Analysis (ell=7)

- **What it does**: First test of Temperley-Lieb radical anatomy at a cubic number field — extends all dimension formulas from quadratic cases (ell=3-6) to the cubic case (ell=7), confirming universality of radical structure across number field degree.
- **Introduced in**: Demo 60
- **Reused in**: (Demo 60 only — feeds into Demo 61 angular proof)
- **Key types/functions**:
  - `find_cubic_root()` — Cantor-Zassenhaus algorithm for finding roots of x^3-x^2-2x+1 mod p; uses x^p mod f(x) followed by polynomial GCD and randomized factoring
  - `find_good_prime(start, root_out)` — searches for primes where the cubic minimal polynomial splits, returning both prime and verified root
  - `analyze()` — complete radical anatomy pipeline: delta power precomputation, weighted fixed-point trace form, augmented Gram matrix, Gaussian elimination for rank, radical basis extraction, rad^2/rad^3 computation via product echelon, nilpotency determination
  - `fibonacci_rank_test()` — decomposes rad^2 generator by through-strand sectors into bilinear matrices, computes rank of each sector, sums to verify F(ell-1) prediction
  - `count_through_strands()` — through-strand classifier (reused from Demo 38/39)
  - `matrix_rank_modp()` — generic Gaussian elimination rank computation on flat matrices mod p
  - Full polynomial arithmetic over F_p mod f(x): `poly_mul()`, `poly_pow()` (repeated squaring), `pdeg()`, `pmod()`, `pgcd()`
- **Approximate size**: ~1012 lines (full demo)
- **Notes**: All formulas confirmed at ell=7: rad(TL_7)=11=2*7-3, rad(TL_8)=39=7^2-7-3, rad^2 is 1-dimensional, nilpotency=3, Fibonacci rank=F(6)=8. Cross-validated at two independent primes (10^9+7 and 10^9+9). Semisimple for n<7, non-semisimple for n>=7. Universality conjecture strengthened — radical structure independent of number field degree.

### 4.8 Three Gram Forms Infrastructure (Demo 39 Parts G-J)

- **What it does**: Computes fixpt, Markov, and cell module Gram matrices at the semisimplicity boundary, along with multi-level radical dimensions and Markov excess analysis.
- **Introduced in**: Demo 39 (Parts G-J)
- **Reused in**: (foundational for future RT truncation studies)
- **Key types/functions**:
  - `build_markov_gram()`: constructs Markov trace bilinear form B_M(a,b) using closure loop counting
  - `closure_loops()`: counts loops created when a TL diagram is planar-closed (top connected to bottom)
  - Cell module Gram verification: per-sector d(n,j) × d(n,j) Gram matrix for Graham-Lehrer bilinear form
  - Multi-level radical computation: radical at n=ℓ through n=ℓ+3 for each ℓ=2..7
- **Approximate size**: ~400 lines (Parts G-J combined)
- **Notes**: The prime p=1000002361 (≡1 mod 840) was chosen to guarantee all roots of unity for ℓ=2..7 exist as quadratic residues. Previous p=10^9+7 had 5 as QNR, making ℓ=5 appear semisimple erroneously.

---

## 5. Neural / Boolean

### 5.1 NPN Equivalence Classification

- **What it does**: Classifies all 256 3-input Boolean truth tables into 14 NPN equivalence classes (13 non-trivial + constant) via exhaustive application of input permutation, input negation, and output negation.
- **Introduced in**: Demo 23
- **Reused in**: Demos 45, 46, 47, 48, 49, 50, 55, 56, 57, 58, 59, 61, 62, 64, 65, 66, 67, 68, 69, 70, 71
- **Key types/functions**:
  - `npn_canon[256]` — precomputed canonical form for all truth tables
  - `npn_init()` — computes canonical forms via 96 transforms (6 perms x 8 input-neg x 2 output-neg)
  - `npn_transform(tt, perm, neg_mask, out_neg)` — applies a single transform
  - `npn_build_classes()` — extracts 13 non-trivial classes with orbit sizes
  - `NPNClass` struct: canon_tt, orbit_size, topo_freq, convergence info
  - `fn_name(tt)` — human-readable name lookup (~40 entries: AND, OR, XOR, MUX, MAJ, etc.)
  - `are_perm_related(tt1, tt2)` — checks input-permutation equivalence
- **Approximate size**: ~130 lines
- **Notes**: 13 non-trivial classes: AND3' (0x01), AND2' (0x03), ~A(B^C) (0x06), ~A~(BC) (0x07), BUF (0x0F), EXACT1 (0x16), MAJ' (0x17), ISOLATE (0x18), 3v-0x19 (0x19), 3v-0x1B (0x1B), A^(B|C) (0x1E), XOR2 (0x3C), XNOR3 (0x69).

### 5.2 Expression Tree Boolean Compiler

- **What it does**: Compiles arbitrary Boolean expressions into braid-bracket evaluation trees, with derived gates (AND, OR, XOR) from the {NOT, NAND} universal set.
- **Introduced in**: Demo 19
- **Reused in**: Demos 20, 21
- **Key types/functions**:
  - `Expr` type: tagged union with `EXPR_VAR`, `EXPR_NOT`, `EXPR_NAND` nodes
  - Arena-based expression pool (`EXPR_POOL_SIZE=1024`, `expr_pool_reset()`)
  - `expr_var(i)`, `expr_not(e)`, `expr_nand(a, b)` — constructors
  - `expr_and(a, b)`, `expr_or(a, b)`, `expr_xor(a, b)` — derived gates
  - `eval_expr(expr, inputs, A, gate_count)` — recursive evaluator with amplitude thresholding per gate
  - `expr_size(expr)` — counts expression tree nodes
- **Approximate size**: ~150 lines
- **Notes**: Gate braid words: NOT = [-1,-1,-1,-1,-1,-1] (6 crossings), NAND = [-1,-1,-1,2,2] (5 crossings). Full adder verified (8/8 truth table entries). Evaluation angle: A = e^{i*5pi/4}.

### 5.3 Braid Logic Gate Infrastructure

- **What it does**: Searches for and evaluates specific braid words that compute classical logic gates via bracket amplitude thresholding.
- **Introduced in**: Demo 18
- **Reused in**: Demos 19, 20, 21, 22
- **Key types/functions**:
  - `GateResult` struct: gate word, strand count, length, best separation, optimal angle, threshold, found flag
  - `gate_output(word, len, n, input_bit, A)` — 1-bit gate: prepends input sub-braid, evaluates bracket amplitude
  - `gate_output_2bit(word, len, n, input_a, input_b, A)` — 2-bit gate
  - `make_gate_braid(input_word, gate_word, full_braid)` — concatenates input encoding + gate template
  - `compute_thresholds()` — dynamic threshold per angle (midpoint of high/low amplitudes)
- **Approximate size**: ~80 lines
- **Notes**: Input encoding: identity braid = 0, single crossing = 1. Gates work at multiple angles (not single fragile point). XOR not found at short lengths (non-threshold gate).

### 5.4 Complex-Valued Neural Network (Single Neuron)

- **What it does**: Single complex neuron with split-sigmoid activation that solves XOR with 6 real parameters where real networks need 9.
- **Introduced in**: Demo 27
- **Reused in**: Demo 28
- **Key types/functions**:
  - `CxNeuron` struct: w1, w2 complex weights + b complex bias = 6 real parameters
  - `cx_neuron_init()` — random initialization
  - `cx_neuron_forward(x1, x2)` — z = w1*x1 + w2*x2 + b → split-sigmoid → smooth XOR readout
  - `cx_neuron_train_xor(epochs, lr)` — full backpropagation
  - `cx_sigmoid(z)` — sigmoid applied independently to Re and Im
  - Smooth XOR decision: p = h.re*(1-h.im) + (1-h.re)*h.im
- **Approximate size**: ~150 lines
- **Notes**: Reproduces Nitta (2003) result. 20 training trials tracked with convergence rate.

### 5.5 Complex-Valued Neural Network (3-Hidden)

- **What it does**: 3-input complex network with 3 hidden neurons for training all 13 NPN classes of Boolean functions.
- **Introduced in**: Demo 45
- **Reused in**: Demos 46, 47
- **Key types/functions**:
  - `CxNet3` struct: wh[3][3] complex hidden weights, bh[3] complex biases, vo[6] real output weights, bo real bias (31 real parameters)
  - `cxnet3_init()`, `cxnet3_forward()`, `cxnet3_train()` — initialization, forward pass with split-sigmoid, backprop
- **Approximate size**: ~200 lines
- **Notes**: All 13 NPN classes converge with 10 trials per class, max 50000 epochs.

### 5.6 Variable-Width Complex Network (CxNetVar)

- **What it does**: Variable hidden-neuron-count complex network (1-6 neurons) supporting multiple activation functions with activation-specific gradient computation.
- **Introduced in**: Demo 47
- **Reused in**: (Demo 47 only)
- **Key types/functions**:
  - `CxNetVar` struct: variable hidden neuron count (nh=1..6)
  - Three activations: `cx_sigmoid()` (split-sigmoid), `cx_mvn_cont()` (z/|z|), `cx_mvn_k8()` (snap to nearest 8th root)
  - `cx_activate(z, ActType)` — unified dispatch
  - Activation-specific backprop: sigmoid derivative, Jacobian of normalization (MVN), STE (MVN-k8)
- **Approximate size**: ~250 lines
- **Notes**: MVN converges 5-10x faster than split-sigmoid. More neurons help parity: nh=3→nh=6 drops pn-RMS from 0.89 to 0.51.

### 5.7 Multiple Activation Functions

- **What it does**: Collection of complex-valued activation functions with uniform classification interface for DKC Boolean function search.
- **Introduced in**: Demo 28 (split-sigmoid, modReLU, cardioid, phase-only)
- **Extended in**: Demo 47 (MVN-continuous, MVN-k8), Demo 50 (k-sector), Demo 55+ (Re>0, Im>0, magnitude), Demo 65 (generalized binary labeling — arbitrary binary sector coloring as activation), Demo 66 (quaternion Voronoi, 24-cell nearest-vertex), Demo 67 (eigenvalue k-sector, Hopf base/phase, Cayley-Klein, custom S² Voronoi), Demo 68 (stereographic Voronoi with S²/Euclidean metric), Demo 70 (dodecahedral/icosahedral Voronoi), Demo 71 (spherical harmonic reconstruction threshold)
- **Key types/functions**:
  - `split_sigmoid_classify(z)` — smooth XOR of Re/Im sigmoids > 0.5
  - `sector_classify(z, k)` — k-sector MVN: angle → sector → j mod 2
  - `re_positive_classify(z)` — Re(z) > 0
  - `im_positive_classify(z)` — Im(z) > 0
  - `magnitude_classify(z, tau)` — |z| > tau
  - `cx_sigmoid(z)` — sigmoid(Re) + i*sigmoid(Im)
  - `cx_mvn_cont(z)` — z/|z| (unit circle normalization)
  - `cx_mvn_k8(z)` — snap to nearest 8th root of unity
  - `apply_activation(z, type)` — multi-activation dispatch
  - `activation_backward()` — per-activation Jacobian for backprop
- **Approximate size**: ~120 lines
- **Notes**: Different activations access different sectors of the bracket algebra. Topology-frequency correlation REVERSES under MVN vs split-sigmoid.

### 5.8 Bracket Catalog Builder (Float)

- **What it does**: Enumerates braids across multiple strand counts and lengths, storing distinct complex bracket values as a codebook for DKC experiments.
- **Introduced in**: Demo 26
- **Reused in**: Demos 27, 28, 45, 46, 47
- **Key types/functions**:
  - `CxCatalogEntry` struct: braid word + full complex bracket + amplitude + phase
  - `build_complex_catalog(max_strands, max_len, A)` — enumerates braids, stores values with |bracket| > 0.5
  - `find_nearest_cx(z, catalog)` — nearest-neighbor search in complex plane
  - `find_nearest_amp(r, catalog)` — nearest-neighbor in amplitude only
- **Approximate size**: ~80 lines
- **Notes**: Typical catalog size: 64 distinct complex values from 2-3 strand braids length 1-8 at A = e^{i*5pi/4}. Covering radius estimated from random probes.

### 5.9 Exact Bracket Catalog Builder (Cyc8)

- **What it does**: Enumerates braids and builds catalog of distinct exact Z[zeta_8] bracket values with paired float representations.
- **Introduced in**: Demo 29
- **Reused in**: Demos 48, 49, 50
- **Key types/functions**:
  - `CatalogEntry` struct: exact Cyc8 value + float Cx value + source braid info
  - `build_exact_catalog(max_strands, max_len)` — enumerates up to 100,000 braids per configuration
  - Deduplication by exact Cyc8 equality
- **Approximate size**: ~100 lines
- **Notes**: ~100 distinct values from 2-3 strand braids length 1-8. At delta=0, amplitudes are integers.

### 5.10 Forward DKC Search Engine

- **What it does**: Exhaustive search over bracket catalog quartets/triples to find exact weight configurations that compute Boolean functions without training.
- **Introduced in**: Demo 29 (XOR triples)
- **Extended in**: Demo 48 (all 13 NPN classes, 100M quartets), Demo 50 (k-sector activation), Demo 64 (matroid deletion-contraction), Demo 65 (generalized binary labeling), Demo 66 (quaternionic S³), Demo 67 (multi-coordinate-system), Demo 68 (stereographic), Demo 69 (multi-root comparison)
- **Key types/functions**:
  - Quartet search: O(n^4) with precomputed partial sums (s12 outside w3 loop)
  - `test_xor_triple(w1, w2, b)` — split-sigmoid XOR verification
  - `search_for_k(k)` — exhaustive triple search with k-sector MVN activation
  - Per-NPN-class solution counting and example storage
  - Parity distance tracking (Hamming distance to 0x69/0x96)
- **Approximate size**: ~150 lines (varies by demo)
- **Notes**: Demo 48: 100M quartets, XNOR3 has exactly 0 solutions (split-sigmoid). Demo 50: parity reachable at k=6 with 906 solutions.

### 5.11 Multiplicative/Additive DKC Batch Search

- **What it does**: Unified search engine testing multiple activation functions simultaneously with both multiplicative (z = w1^x1 * w2^x2 * w3^x3) and additive (z = x1*w1 + x2*w2 + x3*w3) encodings.
- **Introduced in**: Demo 55
- **Reused in**: Demos 57, 58, 59
- **Key types/functions**:
  - `search_3input_batch(catalog, encode_mode, activations)` — runs ALL activations in single pass through catalog triples
  - `search_2input(catalog, encode_mode, activations)` — 2-input variant
  - Encode modes: `ENCODE_MUL`, `ENCODE_ADD`
  - `Activation` table: name/function-pointer/param for batch dispatch
  - Per-NPN-class aggregation across activations
- **Approximate size**: ~150 lines
- **Notes**: Key discovery: additive Re>0 reaches only 5/13 NPN classes (convexity constraint). Multiplicative Re>0 reaches 11/13 universally across ell=4,5,6.

### 5.12 Truth Table from Braid

- **What it does**: Evaluates a braid word with closure and input encoding, determines optimal amplitude threshold, and returns an 8-bit truth table.
- **Introduced in**: Demo 22
- **Reused in**: Demos 23, 25
- **Key types/functions**:
  - `compute_tt(word, len, n, closure, A)` — evaluates all 8 input combinations, sorts amplitudes, finds largest-gap threshold, returns truth table (or -1 if no clean threshold)
- **Approximate size**: ~50 lines
- **Notes**: Input encoding: single crossing for bit=1, identity for bit=0. The largest-gap method finds the natural binary partition of amplitude values.

### 5.13 U(1) Gauge Sweep (Global)

- **What it does**: Finds the optimal global phase rotation that minimizes RMS distance from trained network weights to nearest bracket catalog entries.
- **Introduced in**: Demo 27
- **Reused in**: Demos 28, 45, 46
- **Key types/functions**:
  - 360-angle sweep: rotates all weights by e^{i*theta}, computes RMS to nearest catalog entry
  - `find_best_gauge(weights, catalog)` — returns best angle and RMS
- **Approximate size**: ~50 lines
- **Notes**: Optimal gauge angles cluster at pi/4 multiples (aligned with Z[zeta_8] structure). Single-parameter optimization (1D sweep).

### 5.14 Per-Neuron Gauge Sweep

- **What it does**: Independent U(1) rotation per hidden neuron (3D optimization for 3 neurons), improving on global gauge by ~20%.
- **Introduced in**: Demo 46
- **Reused in**: Demo 47
- **Key types/functions**:
  - `pn_gauge_3d()` — coarse 36^3 grid (10-degree steps) + refinement 11^3 (1-degree steps)
  - `pn_gauge_coord()` — coordinate descent for nh > 3 (72 steps/axis, 3 iterations)
  - Three-way comparison: global gauge vs per-neuron gauge vs amplitude-only
- **Approximate size**: ~100 lines
- **Notes**: Key finding: amplitude-only (RMS 0.33) still beats per-neuron gauge (0.57) — phase decoherence from split-sigmoid is the bottleneck, not gauge approximation.

### 5.15 Measurement Matrix Framework

- **What it does**: Computes 4x8 amplitude matrices (4 closures x 8 inputs) per braid, then analyzes rank structure, mutual information, and SVD.
- **Introduced in**: Demo 25
- **Reused in**: (Demo 25 only)
- **Key types/functions**:
  - `MeasMatrix` struct: 4x8 amplitude matrix + 4 thresholded truth tables + braid word
  - `compute_meas_matrix()` — full measurement matrix for one braid across all closures and inputs
  - `svd_4x8()` — SVD via M*M^T eigendecomposition
  - `sym4x4_eigenvalues()` — Jacobi eigenvalue algorithm for 4x4 symmetric matrices
  - 8x8 Jacobi eigenvalue solver for covariance analysis
  - Mutual information between closure pairs (joint/marginal frequency tables)
  - ANOVA (between/within class variance) for Hamming weight grouping
- **Approximate size**: ~200 lines
- **Notes**: All 1554 braids (lengths 1-4) have universal full rank 4. Parity confirmed unreachable (zero appearances).

### 5.16 Spearman Rank Correlation

- **What it does**: Computes Spearman rank correlation coefficient with tie handling for comparing ordinal rankings (e.g., topology frequency vs decomposition quality).
- **Introduced in**: Demo 45
- **Reused in**: Demos 46, 47, 48, 49
- **Key types/functions**:
  - `spearman_rho(x, y, n)` — Spearman rank correlation
  - `assign_ranks(values, ranks, n)` — ranking with average tie-breaking
- **Approximate size**: ~40 lines
- **Notes**: Key finding: rho(RMS, topo_freq) = -0.8167 (forward-reverse duality).

### 5.17 Real Backprop Network (2-2-1)

- **What it does**: Standard 2-hidden-neuron real-valued sigmoid network for XOR, serving as baseline for complex network comparisons.
- **Introduced in**: Demo 26
- **Reused in**: Demo 27
- **Key types/functions**:
  - `RealXorNet` struct: 2-2-1 architecture (9 parameters)
  - `real_net_init()`, `real_net_forward()`, `real_net_train_xor()` — full backprop
- **Approximate size**: ~100 lines
- **Notes**: Baseline: 9 parameters, amplitude-only RMS = 0.2641.

### 5.18 Semicircle / Half-Plane Proof Framework

- **What it does**: Pure integer-arithmetic framework for proving which NPN classes are reachable by semicircle (half-plane) activations on multiplicative angle-sum encodings.
- **Introduced in**: Demo 61
- **Reused in**: Demo 62
- **Key types/functions**:
  - `truth_table(p1, p2, p3, theta, N)` — computes 8-bit truth table from integer angles mod N
  - `in_semi(angle, theta, N)` — semicircle membership test
  - `search(grid_n)` — exhaustive (p1, p2, p3) in Z_N^3 with 8 boundary positions
  - `build_npn()` — NPN canonical form (includes output negation)
  - `is_affine(tt)` — tests GF(2) affine subspace membership
  - `masked_feasible(target, mask, grid_n)` — partial truth table feasibility
  - `find_min_obstruction(target, grid_n)` — minimum infeasible constraint subset
  - `wrap(angle)` — angle normalization
- **Approximate size**: ~200 lines
- **Notes**: Proves exactly 11/13 NPN classes reachable. 0x06 obstruction: parallelogram gap argument. 0x1B obstruction: 3-step algebraic contradiction. Zero floating-point in proof.

### 5.19 Octant-Sector Classification (Demo 63)

- **What it does**: Classifies Z[zeta_8] lattice points into 8 discrete octants, maps sector boundaries to class-1 octant sets, and computes oriented matroid types for weight triples.
- **Introduced in**: Demo 63
- **Reused in**: (foundational for future Z[zeta_16] scaling demos)
- **Key types/functions**:
  - `angle_to_octant()`: classifies a Cyc8 value into one of 8 discrete octants (0-7)
  - `cross_sign(a, b)`: sign of 2D cross product of two octant vectors
  - `om_encode(s12, s13, s23)`: encodes oriented matroid type as a single integer from 3 pairwise cross-product signs
  - `om_decode()`: decodes OM integer back to sign triple
  - Sector-octant mapping: iterates k-sector boundaries to compute class-1 octant set for arbitrary k
- **Approximate size**: ~80 lines
- **Notes**: Oriented matroid classification is the key tool for characterizing parity — alternating OM type is necessary and sufficient for 3-input parity.

### 5.20 Recursive N-Input Parity Search (Demo 63)

- **What it does**: General recursive search for n-input parity solutions in Z[zeta_8] MVN networks. Tests all 2^n-1 non-empty subset sums against sector-parity constraints.
- **Introduced in**: Demo 63
- **Reused in**: (foundational for future parity scaling studies)
- **Key types/functions**:
  - `collect_class1(k)`: collects all class-1 sector vectors for given sector count k
  - `compute_tt4_at_k(k)`: 4-input DKC truth table search with 4-level pruning (checks 1-weight, 2-weight, 3-weight, 4-weight sum parities)
  - `pj_search(n, k)`: general recursive n-input parity search — iterates over all n-tuples of catalog values, checks all 2^n-1 subset sums for correct sector parity
- **Approximate size**: ~200 lines (Part H: 4-input ~80 lines, Part I: 5-input ~60 lines, Part J: recursive ~60 lines)
- **Notes**: Search pruning is critical for performance — 4-input uses early rejection at each weight addition. 5-input at k=15 has 7 class-1 octants (100 values each), making brute force feasible.

### 5.21 Matroid Deletion-Contraction Infrastructure (Demo 64)

- **What it does**: Exhaustive weight-set subtraction and contraction across the XOR parity hierarchy {XOR2, XOR3, XOR4, XOR5}, verifying 100% deletion-contraction closure (minor chain), tracking collision multiplicity, and analyzing vocabulary stratification.
- **Introduced in**: Demo 64
- **Reused in**: (foundational for future matroid/parity scaling studies)
- **Key types/functions**:
  - Deletion map with collision tracking: for each XOR_n solution set, subtracts each weight and records which XOR_{n-1} child it maps to; counts distinct children vs. total instances to identify collisions
  - Upward extension search: for each XOR_n weight set, tests all 83 class-1 catalog values as candidate (n+1)-th weight, records Hamming distance to XOR_{n+1} truth table
  - Vocabulary stratification analysis: separates catalog values into good (appear in extensible sets), poison (appear only in orphan sets), and absent categories; annotates by octant
  - Error-weight decomposition: bins output errors by input Hamming weight (weight-0 through weight-6) to isolate interaction-level vs. single-weight failures
  - Self-doubling classifier: detects w[n+1]=w[i] cases; computes angle-preservation argument symbolically showing scalar doubling preserves sector class
- **Approximate size**: ~400 lines
- **Notes**: Key findings — 100% downward closure (deletion→XOR_{n-1}, contraction→XNOR_{n-1}) verified at all levels. Upward coverage collapses funnel-shaped: 99.3% → 39.2% → 19.2% → 0% from XOR3→XOR2 up to XOR6→XOR5. 44 good values / 31 poison values in the class-1 k=15 catalog; poison values avoid octant 4. Minimum wall Hamming distance is d=4; self-doubling accounts for 89.5% of closest misses. Exact Cyc8 arithmetic is mandatory — float arithmetic introduces 420 spurious failures near sector boundaries.

### 5.22 Generalized Binary Sector Labeling (Demo 65)

- **What it does**: Tests arbitrary binary colorings of k sectors (not just the standard odd/even convention) as DKC activations, enabling a much larger search space for XOR6 and XOR7 solutions.
- **Introduced in**: Demo 65
- **Reused in**: (Demo 65 only — new framework for non-standard MVN colorings)
- **Key types/functions**:
  - `generalized_xor6_check(k)`: iterates over all (XOR5-set, extension-weight) candidate pairs at arbitrary k, tests all 2^k binary colorings for whether any coloring of k sectors realizes XOR6; reports passing count
  - Even/odd k landscape sweep: systematic sweep of even k=2..64 generalized XOR6 pass counts
  - Antipodal structure analyzer: decomposes n-tuple into antipodal pairs and lone weights, reports margin to sector boundaries
  - XOR7 broader search: extends from single extensions to all C(100,2) weight pairs, 136,350 7-tuples per k
  - Multi-catalog XOR search: runs full sector sweep with both 36-value and 56-value catalogs, distinguishes catalog incompleteness artifacts from genuine walls
- **Approximate size**: ~400 lines
- **Notes**: Key findings — absolute wall at k<=23 (zero solutions under ANY binary labeling); first generalized XOR6 at k=24 (3 of 2700 pairs). All passing solutions have exactly zero margin to sector boundary (lattice sums land exactly on lattice directions). Even k works near threshold; odd k (25, 27, 29, 31) all fail. k=38 is sole even exception with zero solutions. Parity ceiling n=5 is algebra-independent: identical at Z[zeta_8] and Z[zeta_16]. Gap-of-2 hypothesis refuted: pigeonhole rises to 14 at Cyc16 but ceiling stays at n=5 (gap widens to 9).

### 5.23 SU(2) Braid Representation and 24-Cell Geometry (Demo 66)

- **What it does**: Lifts the Kauffman bracket from S^1 (complex trace) to S^3 (unit quaternion) via an SU(2) braid representation, generates the 24-cell polytope from braid enumeration, and searches for XOR6 solutions using quaternionic Voronoi activation.
- **Introduced in**: Demo 66
- **Reused in**: Demos 67, 68, 69, 70, 71 (24-cell catalog and quaternion arithmetic used throughout the arc)
- **Key types/functions**:
  - `quat_mul(p, q)`: quaternion multiplication (4-component real vector)
  - `quat_norm(q)`: quaternion normalization to unit sphere S^3
  - `quat_nearest_24cell(q, catalog)`: finds nearest 24-cell vertex by dot-product maximization
  - `enumerate_braids(max_len, gen_sigma1, gen_sigma2, catalog)`: generates all braids up to target length, applies SU(2) generators, collects distinct quaternion outputs (finds exactly 24 = binary octahedral group)
  - Voronoi partition on S^3: assigns arbitrary unit quaternions to nearest 24-cell vertex; 25 cells = 24 vertices + 1 zero cell
  - Antipodal-pair XOR6 search: restricts combinatorial search to 3-antipodal-pair sextets; evaluates XOR6 over Voronoi cells
  - F4 symmetry orbit computation: generates all 576 two-sided symmetry elements of the 24-cell and groups solutions into orbits
  - Hash collision analysis: pair-table construction and four-way bracket/quaternion intersection counting
- **Approximate size**: ~500 lines
- **Notes**: SU(2) generators sigma_1 -> (1+i)/sqrt(2), sigma_2 -> (1-k)/sqrt(2). Produces exactly 24 quaternions from 87,890 braids (binary octahedral group). 35 Voronoi XOR6 solutions; universal 7/7/11 cell-label structure. F4 symmetry group (576 elements) decomposes 35 solutions into 6 orbits in two families: edge-dominant (22) and body-dominant (13). Complementary with bracket: quaternion resolves 450 bracket collisions; bracket resolves 8,424 quaternion collisions.

### 5.24 Eigenvector Extraction and Multi-Coordinate Activation (Demo 67)

- **What it does**: Decomposes SU(2) quaternions into eigenvector direction (S^2) and eigenvalue angle, then systematically compares all natural coordinate systems (Hopf, Cayley-Klein, eigenvalue k-sector, custom S^2 Voronoi) as DKC activations.
- **Introduced in**: Demo 67
- **Reused in**: Demos 68, 69, 70, 71 (13-direction S^2 Voronoi is the canonical activation going forward)
- **Key types/functions**:
  - `eigenvector_extract(q, dir_out, theta_out)`: decomposes SU(2) quaternion into eigenvector direction (mod +-) and eigenvalue angle theta = arccos(a)
  - Custom Voronoi on S^2: assigns quaternion to nearest of 13 data-derived directions (3 axis + 6 edge-midpoint + 4 body-diagonal); handles identity/zero as degenerate cell 0
  - Platonic solid Voronois: octahedral (6 cells), icosahedral (12 cells), cuboctahedral (12 cells) — all yield zero XOR6 solutions
  - Hopf coordinate decomposition: converts quaternion to Hopf (eta, xi_1, xi_2); separates base (eta, xi_2) and fiber (xi_1) activations
  - Cayley-Klein parameterization: (|alpha|, arg(alpha)) grid activation
  - Eigenvalue k-sector activation: partitions [0, pi] into k equal sectors on eigenvalue angle theta
  - Comparative activation harness: unified XOR6 DKC search applicable across all coordinate systems; reports solutions and sol/cell efficiency
- **Approximate size**: ~400 lines
- **Notes**: Key finding: Hopf PHASE (fiber xi_1) carries zero DKC information at ALL tested resolutions (k=4..32, exhaustive). Computation lives entirely on S^2 Hopf base. Custom 13-dir Voronoi achieves 36 solutions at 14 cells (2.57 sol/cell) — more than full 25-cell S^3 Voronoi (35). Platonic solids fail because they have wrong symmetry group. Eigenvalue paradox: theta = arccos(a) succeeds; Hopf fiber xi_1 = atan2(b,a) fails — they extract different information. High-resolution rankings at 64 cells: Hopf base (292) > eigenvector lat-lon (204) > Cayley-Klein (106).

### 5.25 Stereographic Projection and ASCII Voronoi Renderer (Demo 68)

- **What it does**: Projects the S^2 eigenvector Voronoi onto R^2 via stereographic projection, renders solutions as ASCII images, extracts perceptron weights, and measures conformality via Monte Carlo area estimation.
- **Introduced in**: Demo 68
- **Reused in**: (Demo 68 only — visualization and planarity-proof infrastructure)
- **Key types/functions**:
  - Stereographic projection / inverse: north-pole projection of S^2 to R^2 and back; round-trip precision ~1e-15 (machine epsilon); south pole is excluded as it coincides with a catalog direction
  - Projected Voronoi with S^2 metric: assigns R^2 query point to nearest of 13 projected centers using back-projected dot product; preserves all 36 solutions identical to Demo 67
  - Euclidean Voronoi in R^2: same structure but using Euclidean distance; yields ZERO solutions (preserved as negative-result comparison proving intrinsic curvature)
  - ASCII renderer: 65x33 grid over bounding box; renders 0/1 regions, Voronoi boundaries (+), weight positions (*), and center labels (0-9, A-C)
  - Perceptron weight extractor: builds 6x2 matrix from stereographic projections of winning solution weight quaternions
  - Monte Carlo area estimator: samples Voronoi cell areas in R^2 for conformality analysis; largest cell 17%, smallest 1%, ratio 16.5x
- **Approximate size**: ~350 lines
- **Notes**: Key finding: Euclidean metric in R^2 gives zero solutions; S^2 great-circle metric gives 36. Area distortion (16.5x) is irrelevant when S^2 metric is used — conformality is sufficient, area-preservation is not needed. The stereographic image is a valid visualization but not a valid computational domain. "Visualize on the plane, compute on the sphere." 13 catalog direction centers project to bounding box x in [-2.41, 1.37], y in [-1.00, 2.41].

### 5.26 Multi-Root SU(2) Catalog Comparison (Demo 69)

- **What it does**: Generates quaternion and eigenvector catalogs for multiple cyclotomic roots (zeta_8, zeta_16, zeta_32), verifies direction nesting (zeta_N subset zeta_{2N}), and compares XOR6 solution counts across roots at both natural Voronoi and fixed cell counts.
- **Introduced in**: Demo 69
- **Reused in**: (Demo 69 only — multi-root comparison infrastructure)
- **Key types/functions**:
  - Multi-root catalog enumeration: generates quaternion and eigenvector catalogs for arbitrary zeta_N with configurable MAX_QCAT and MAX_ECAT caps; fair comparison at same braid enumeration depth across roots
  - Direction nesting comparator: for each zeta_8 direction, finds nearest match in zeta_{2N} catalog and reports angular distance; classifies as RIGIDITY (0 degrees), DRIFT (small positive), or INDEPENDENCE (large positive)
  - Adaptive Voronoi XOR6 search: builds natural Voronoi from catalog directions, enumerates all weight triples, reports sol/cell and percentage of C(N,3)
  - Fixed-resolution grid activation: lat-lon grid with specified cell count applied to all roots for apples-to-apples comparison
  - Catalog explosion enumeration: counts quaternions and directions as braid length grows for each root
  - Trivialization detection: identifies when solution rate approaches 100% (pigeonhole saturation — too many cells, too few constraint inputs)
- **Approximate size**: ~300 lines
- **Notes**: Key finding: zeta_8 -> zeta_16 nesting is RIGID (13/13, 0.000 degrees drift). Algebraic proof: (zeta_16)^2 = zeta_8 so every zeta_8 braid word is a doubled word in zeta_16; squaring a quaternion preserves the rotation axis. zeta_8 is the computational sweet spot: coarsest root producing the 13-direction structure. At natural Voronoi resolution, zeta_16 (3,457 cells): 90.6% solutions; zeta_32 (4,096+ cells): 89.6% — pigeonhole trivialization. At fixed 16 cells: zeta_16 (450) > zeta_32 (393).

### 5.27 Eigenvalue-to-Pitch Mapping and Braid Melody Extraction (Demo 70)

- **What it does**: Maps SU(2) quaternion eigenvalue angles to musical intervals and note names, extracts melodies from braid words, and provides spatial sound mapping from eigenvector directions to stereo pan and pitch.
- **Introduced in**: Demo 70
- **Reused in**: Demo 71 (musical note connection in spectral analysis)
- **Key types/functions**:
  - Eigenvalue-to-interval mapping: quaternion trace -> theta = arccos(trace/2) -> cents = theta * (1200/pi) -> note name (C=0, Eb=45, E=60, F#=90 degrees); covers unison through tritone
  - Braid word melody generator: running quaternion product per crossing; extracts note at each step from the partial product eigenvalue angle
  - Spatial sound mapper: eigenvector direction -> latitude/longitude -> MIDI pitch (C3 south to C5 north) + stereo pan (longitude)
  - Platonic solid vertex generators: icosahedron (12 vertices) and dodecahedron (20 vertices / 10 effective post-antipodal-collapse) for comparative Voronoi testing — both give zero XOR6 solutions
  - XOR6 checker: evaluates XOR6 truth table against arbitrary Voronoi cell assignments
  - ABC notation formatter: outputs top braid melodies in standard ABC music notation for rendering/playback
- **Approximate size**: ~300 lines
- **Notes**: 4 eigenvalue angles (0, 45, 60, 90 degrees) map to 4 notes (C, Eb, E, F#): maximum consonance to maximum dissonance. Conjugation-invariance of melody proven: tr(q) = 2cos(theta) is conjugation-invariant so melody depends only on product length not which generators used. Yang-Baxter relation sigma_1*sigma_2*sigma_1 = sigma_2*sigma_1*sigma_2 produces identical melodies step-by-step. Hopf orthogonality: pitch and spatial position are independent (Pearson r = -0.0624). Garside element (s1*s2)^3 melody: Eb E F# E Eb C. Tritone-antipodal correspondence: antipodal dodecahedron faces carry tritone pairs.

### 5.28 Spherical Harmonic Engine and Spectral Analysis (Demo 71)

- **What it does**: Full spherical harmonic analysis infrastructure for decomposing arbitrary S^2 functions into per-l-mode power, finding the phase-transition bandwidth for DKC solutions, and characterizing XOR6 Voronoi labelings spectrally.
- **Introduced in**: Demo 71
- **Reused in**: (foundational for future analytical S^2 bandwidth proofs)
- **Key types/functions**:
  - Spherical harmonic engine: real Y_lm via associated Legendre polynomials with stable three-term recurrence; numerically stable to l=12+; orthonormality max error 6.82e-14
  - Gauss-Legendre quadrature on S^2: 64-point integration in theta; uniform 128-point grid in phi; 64x128=8,192 total grid points; produces integration weights on S^2
  - `spectrum_of_function(voronoi_label_fn, l_max)`: decomposes arbitrary S^2 function (given as Voronoi label array evaluated over grid) into power per l-mode from l=0 to l_max
  - `bandwidth_test(winners, l_cut)`: reconstructs function from spherical harmonic coefficients up to l_cut, evaluates winner recovery fraction; finds sharp phase-transition threshold
  - von Mises-Fisher density on S^2: direction density representation for the 13-direction catalog; high-kappa (kappa=50) approximation of point-like directions
  - Comparative spectrum harness: runs spectral decomposition over all 36 winners, aggregates power-per-mode statistics (mean, min, max per mode)
- **Approximate size**: ~450 lines
- **Notes**: Key finding: sharp phase transition at l=6 (0% recovery at l<=5, 100% at l=6). The 13=13 theorem: minimum bandwidth l=6 because 2*6+1=13 = number of eigenvector directions — at l=5 system is underdetermined (11 components for 13 regions). l=6 mode carries ~80.7% of power on average across all 36 winners (spectral universality). Only even modes carry power (antipodal symmetry of the Voronoi partition). 3.5x super-Nyquist compression explained by compressed sensing (binary function at known support positions). Musical connection: l=6 maps to 200 cents (note D, whole tone) — above the eigenvalue-resonant modes (l=2 tritone, l=3 major 3rd, l=4 minor 3rd). Trivialization prediction for zeta_16: minimum bandwidth l>=1728 (2*1728+1=3,457 directions), Nyquist ~3M cells — explains Demo 69's ~90% solution rate.

---

## 6. Utility

### 6.1 LCG Random Number Generator

- **What it does**: Linear Congruential Generator for reproducible random experiments across all DKC and neural network demos.
- **Introduced in**: Demo 13
- **Reused in**: Demos 26, 27, 28, 45, 46, 47, 48, 49, 50
- **Key types/functions**:
  - `rng_seed(s)` — set seed
  - `rng_uniform()` — uniform [0, 1) double
  - `rng_normal()` — standard normal via Box-Muller transform
- **Approximate size**: ~25 lines
- **Notes**: Standard LCG constants (multiplier, increment, modulus). Seed 12345 used for most baseline experiments.

### 6.2 Quantization

- **What it does**: Uniform quantize-dequantize for encoding bracket values at reduced bit depths.
- **Introduced in**: Demo 12
- **Reused in**: Demos 13, 16, 17
- **Key types/functions**:
  - `quantize(x, lo, hi, bits)` — scalar uniform quantization
  - `cx_quantize(z, lo, hi, bits)` — independent Re/Im quantization
  - `cx_ternary(z, threshold)` — {-1, 0, +1} per component (BitNet-style 1.58-bit)
  - `cx_sign(z)` — quadrant / 1-bit per component
  - `QuantVal` struct — integer quantized value
- **Approximate size**: ~40 lines
- **Notes**: Key finding: granny/square knots have identical amplitude (need phase to distinguish). Pareto frontier: precision x width trade-off.

### 6.3 Greedy Angle Selection

- **What it does**: Iteratively selects evaluation angles that maximize minimum pairwise bracket distance across all knot pairs.
- **Introduced in**: Demo 11
- **Reused in**: Demos 12, 13, 15, 16, 17
- **Key types/functions**:
  - `select_greedy_angles()` / `greedy_angles()` — max-min-distance greedy selection from 256-sample waveforms
  - `count_separated_pairs()` — multi-angle pairwise distance check
  - `pairwise_separation()` — finds best single angle, hardest pair, full separation matrix
  - `min_dist_from_vals()` — minimum pairwise distance from precomputed values
- **Approximate size**: ~80 lines
- **Notes**: Best single angle (1.805*pi) independently converges to within 1.5% of WRT level-3 evaluation point (11*pi/6). 3 WRT angles on reduced bracket separate all 21 knot types.

### 6.4 WRT Angle Computation

- **What it does**: Computes Witten-Reshetikhin-Turaev evaluation angles for level r.
- **Introduced in**: Demo 15
- **Reused in**: (Demo 15 only)
- **Key types/functions**:
  - `wrt_angle(r)` — theta = 2*pi - pi/(2r)
  - `SepMetrics` struct: separated count, min_dist, mean_dist, sum_log_dist
  - `measure_separation()` — full pairwise metrics for a set of angles
- **Approximate size**: ~50 lines

### 6.5 Delta Exponent Detection

- **What it does**: Determines how many factors of delta = -(A^2 + A^{-2}) divide the bracket polynomial.
- **Introduced in**: Demo 15
- **Reused in**: Demos 16, 17
- **Key types/functions**:
  - `delta_exponent(braid, A)` — evaluates bracket at 4 delta-zero angles (pi/4, 3pi/4, 5pi/4, 7pi/4) + perturbation test
  - `reduced_bracket_at(braid, A)` — g(A) = bracket(A) / delta^k with zero-avoidance fallback
- **Approximate size**: ~50 lines
- **Notes**: Delta exponents take values {0, 1, 2} for test knots. The reduced bracket g has fewer zeros and better classification power.

### 6.6 Waveform Analysis

- **What it does**: Sweeps bracket evaluation around the full unit circle to produce waveform signatures for knot classification.
- **Introduced in**: Demo 11
- **Reused in**: Demo 12
- **Key types/functions**:
  - `compute_waveforms()` — 256-sample sweep for all knots
  - `waveform_stats()` — min/max amplitude, dynamic range, zero count
  - `ascii_waveforms()` — ASCII art visualization (60-column, 5 intensity bands)
  - `cross_validate()` — consistency check with direct evaluation
  - `test_symmetries()` — chiral amplitude identity, amphichiral reflection
- **Approximate size**: ~100 lines
- **Notes**: Chiral pairs have identical amplitude waveforms; only phase distinguishes them.

### 6.7 Fortuin-Kasteleyn Partition Function

- **What it does**: Computes the FK cluster expansion of the Potts model partition function, bridging knot invariants to statistical mechanics.
- **Introduced in**: Demo 56
- **Reused in**: (Demo 56 only)
- **Key types/functions**:
  - `LatticeStrip` struct: rectangular grid with edge arrays
  - `build_strip(w, h)` — constructs lattice with horizontal + vertical edges
  - `fk_partition(strip, Q, v)` — full 2^|E| subset enumeration with precomputed powers
  - `ising_spin_sum(strip, v)` — direct 2^|V| spin configuration sum for integer-coupling cross-validation
- **Approximate size**: ~150 lines
- **Notes**: Establishes Q=delta^2 dictionary: ell=3→Q=1 (percolation, trivial), ell=4→Q=2 (Ising), ell=6→Q=3 (3-state Potts). FK values are algebraically richer (4D) than bracket values (2D Z[i]-axial).

### 6.8 Pairwise Separation Metrics

- **What it does**: Computes minimum pairwise distance, mean distance, and separation counts for knot classification evaluation.
- **Introduced in**: Demo 11
- **Reused in**: Demos 12, 13, 14, 15, 16, 17
- **Key types/functions**:
  - `pairwise_separation()` — best angle, hardest pair, full matrix
  - `min_dist_from_vals()` — minimum pairwise distance from value arrays
  - `count_separated_pairs()` — threshold-based pair counting
  - `min_dist_subset()` / `min_dist_all()` — for knot subsets
- **Approximate size**: ~40 lines

### 6.9 Cantor-Zassenhaus Root Finding

- **What it does**: Probabilistic algorithm for finding roots of polynomials over finite fields, used to compute modular representations of algebraic numbers like 2cos(pi/7).
- **Introduced in**: Demo 39 (specialized cubic for ell=7)
- **Reused in**: Demo 60 (generalized with prime search)
- **Key types/functions**:
  - `find_l7_root()` (Demo 39) / `find_cubic_root()` (Demo 60) — finds root of x^3-x^2-2x+1 mod p using x^p mod f(x) followed by polynomial GCD
  - `find_good_prime(start, root_out)` (Demo 60) — searches for primes where the cubic splits completely, returns both prime and verified root
  - Polynomial GCD and reduction over F_p
- **Approximate size**: ~80 lines
- **Notes**: The cubic x^3-x^2-2x+1 is the minimal polynomial of 2cos(pi/7), the first non-quadratic case in the Chebyshev parameter hierarchy. Not all primes split the cubic — the search routine finds suitable primes automatically. Two independent primes used for cross-validation.

---

## Cross-Cutting Patterns

### Copy-Paste Codebase Pattern
Each demo is a standalone `main.c` file that copies needed infrastructure from prior demos. There is no shared library extraction. This means:
- Code evolves independently per demo (e.g., MAX_TERMS varies)
- Bug fixes in one demo don't propagate
- Each demo compiles and runs independently
- The "library" is the accumulated pattern knowledge, not shared object files

### Ring-Parametric Architecture
Demos 53-59 follow a consistent pattern:
1. Implement cyclotomic ring arithmetic (Cyc_n type)
2. Reuse identical planar matching / diagram composition code
3. Instantiate TL matrix algebra over the new ring
4. Build state-sum bracket with ring-specific delta powers
5. Build catalog and run DKC Boolean function search

The topology code (planar matchings, diagram composition, trace closure) is completely ring-independent.

### Radical Anatomy Pipeline
Demos 38, 39, 51, 52, 60 follow a consistent pattern for analyzing TL algebra structure:
1. Enumerate planar matching basis (Section 3.1)
2. Build multiplication table with loop counts
3. Construct Gram matrix of trace form (fixpoint-based or delta-weighted)
4. Gaussian elimination mod p for rank / radical dimension
5. Extract radical basis, compute radical filtration (rad^2, rad^3, ...)
6. Sector decomposition by through-strand count

This pipeline works identically across number field degree (rational, quadratic, cubic) with only the delta representation changing.

### Validation Chain
Every new computation method is cross-validated against prior oracles:
- Symbolic bracket (Demo 01) validates braid bracket (Demo 02)
- Braid bracket validates matrix method (Demos 03-06)
- Matrix method validates numerical engine (Demo 10)
- State-sum validates TL matrix method (Demos 35, 53-59)
- Exact Cyc8 validates float Cx (Demo 29)
- Two-prime cross-validation for cubic field results (Demo 60)

### Quaternionic / S^2 Arc Architecture (Demos 66-71)

Demos 66-71 follow a consistent pattern that lifts DKC from complex S^1 to quaternionic S^3 and then projects onto S^2:
1. Generate the SU(2) braid representation (Demo 66) — produces exactly 24 quaternions (24-cell / binary octahedral group)
2. Decompose quaternions into eigenvector direction (S^2) and eigenvalue angle (Demo 67) — identifies 13 data-intrinsic directions
3. Build the custom 13-direction Voronoi on S^2 — the canonical activation for the arc (36 XOR6 solutions at 14 cells)
4. Verify the computation lives on S^2, not S^1 fiber (Hopf phase inert, Demo 67) and not R^2 Euclidean (Demo 68)
5. Confirm direction nesting across cyclotomic roots — zeta_8 is the computational sweet spot (Demo 69)
6. Analyze spectral structure: l=6 bandwidth threshold = 13=13 theorem (Demo 71)

Key progression: D65 S^1 needs k=24 sectors (25 cells) -> D66 S^3 Voronoi needs 25 cells -> D67 S^2 Voronoi needs only 14 cells -> D68 R^2 is visualization only (zero solutions with Euclidean metric).

### Catalog Size Summary
| Ring | Delta | Catalog | Distinct Values |
|------|-------|---------|-----------------|
| Z[zeta_8] | 0 | 2-3 strand, len 1-8 | ~100 |
| Z[omega] | 1 | 2-4 strand, len 1-8 | varies |
| Z[zeta_16] | sqrt(2) | 2-4 strand, len 1-6 | ~56 |
| Z[zeta_5] | phi | 2-4 strand, len 1-6 | >56 |
| Z[zeta_24] | sqrt(3) | 2-4 strand, len 1-10 | varies |

### SU(2) / Quaternionic Catalog Summary (Demos 66-69)
| Root | Quaternions | Directions on S^2 | XOR6 at natural Voronoi | XOR6 at 16 cells |
|------|-------------|-------------------|--------------------------|------------------|
| zeta_8 | 24 | 13 | 36 (14 cells, 0.007%) | — |
| zeta_16 | 7,952 | 3,457 | 499,367 (90.6%) | 450 |
| zeta_32 | 32,768+ | 4,096+ | 493,654 (89.6%) | 393 |
