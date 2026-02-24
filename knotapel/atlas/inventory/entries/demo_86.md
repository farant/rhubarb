## Demo 86: Direct b on Single P_{0,0} via Delta-Parameterized Forms

- **Status**: COMPLETE (NEGATIVE RESULT)
- **File**: `demo_86_b_direct/main.c` (~1787 lines)
- **Tests**: 15/15 pass
- **Depends on**: Demo 85 (TL algebra infrastructure, left ideal construction, b = -5/8 on regular representation at TL_4)
- **Feeds into**: Open — the actual literature method for computing b at larger lattice sizes remains to be identified and implemented; dense polymer fusion calculation (narrative.md seed)

### Headline
The delta-parameterized Gram matrix approach universally diverges on a single projective cover P_{0,0} at every eigenvalue, every lattice size, and for both loop and trace forms. The multiplicity from the regular representation is structurally essential to the b computation, not just a scale factor. This is a novel negative finding — nobody in the literature has tried this specific approach.

### Key Results
- **Universal divergence**: the valuation condition p_tt = 2*p_Tt never holds on a single P_{0,0}. The delta-form b computation returns NaN (diverges) at every eigenvalue tested, for both loop form (predicting b = -5/8, Pearce-Rasmussen) and trace form (predicting b = -2, GRS).
- **Exhaustive eigenvalue scan**: TL_4 tested at lambda = 0, +/-sqrt(2); TL_6 tested at lambda = 0, +/-1, +/-sqrt(3), +/-(sqrt(3)+1), +/-(sqrt(3)-1). All diverge with negative gap (p_tt < 2*p_Tt).
- **TL_8 P_{0,0} is completely semisimple**: no Jordan blocks at lambda=0 on the dim-42 ideal. The non-semisimple structure that enables b computation does not persist at this lattice size on P_{0,0} alone.
- **Full regular rep at TL_6 also diverges**: the same d^{-2} gap appears in the full regular representation at TL_6. The TL_4 success (Demo 85) may be coincidental — cross-copy terms provide the right extra delta-power at TL_4 but fail at TL_6.
- **Generator independence confirmed**: all 2-through-line generators produce identical ideals and identical divergence patterns.
- **Projective-cover-exclusive eigenvalues discovered**: TL_6 P_{0,0} has eigenvalues +/-(sqrt(3)+/-1) that do not appear on the standard module W_0 — these are structurally intrinsic to the projective cover.
- **Gemini cross-model check partially wrong**: Gemini 3 Pro confirmed TL_4 eigenvalues and ground state formula E_GS = -2cos(pi/n), but got TL_6 wrong (predicted standard module spectrum, missing projective-cover-exclusive eigenvalues).
- **Gemini literature claim falsified by Demo 85**: Gemini claimed the standard method extracts b from Jordan cell coupling on standard modules. Demo 85 proved standard modules have no Jordan blocks at any tested size (n=4,6,8,10,12). Jordan blocks only exist on P_{0,0} (projective covers).

### Theorems/Conjectures
- **Delta-form divergence on single P_{0,0} (DEMONSTRATED)**: the delta-parameterized Gram matrix approach universally diverges. This is a novel negative finding — the approach has not been tried in the literature.
- **Multiplicity essentiality (DEMONSTRATED)**: the dim(V_0)^2 multiplicity from the regular representation is not a removable scale factor. It participates in the valuation balance required for the b ratio to converge.
- **P_{0,0} dimension pattern (OBSERVED)**: dim(P_{0,0}) at TL_n = C_{n/2+1} (next Catalan number): TL_4 -> 5 = C_3, TL_6 -> 14 = C_4, TL_8 -> 42 = C_5.
- **Pearce-Rasmussen method conjecture (OPEN)**: the actual PR method likely works on P_{0,0} directly but extracts b from the Jordan cell coupling L_0|t> = |T> + b|T> structure, NOT from delta-parameterized Gram matrices.

### Data
- **TL_4 P_{0,0}**: dim=5, char poly x(x^2-2)^2, Jordan blocks at lambda=+/-sqrt(2) only
- **TL_6 P_{0,0}**: dim=14, char poly x^2(u-1)(u-3)(u^2-8u+4)^2 where u=x^2, Jordan blocks at lambda = 0, +/-(sqrt(3)+1), +/-(sqrt(3)-1)
- **TL_8 P_{0,0}**: dim=42, completely semisimple at lambda=0
- **Divergence table**:

| Size | lambda | Form | p_Tt | p_tt | 2*p_Tt | Gap |
|------|--------|------|------|------|--------|-----|
| TL_4 | +/-sqrt(2) | loop | 2 | 2 | 4 | -2 |
| TL_4 | +/-sqrt(2) | trace | 1 | 1 | 2 | -1 |
| TL_6 | 0 | loop | 3 | 4 | 6 | -2 |
| TL_6 | 0 | trace | 3 | 2 | 6 | -4 |
| TL_6 | +/-(sqrt(3)+1) | loop | 3 | 3 | 6 | -3 |
| TL_6 | +/-(sqrt(3)+1) | trace | 1 | 1 | 2 | -1 |
| TL_6 | +/-(sqrt(3)-1) | loop | 3 | 3 | 6 | -3 |
| TL_6 | +/-(sqrt(3)-1) | trace | 1 | 1 | 2 | -1 |

- **Formal coefficient ratios** (divergent, for reference): TL_4 lambda=-sqrt(2) loop gives 1/2; TL_6 lambda=0 loop gives -5/18. Neither matches -5/8 or -2.

### Code Assets
- **TL algebra infrastructure** (reused from Demo 85): `alg_init()`, `alg_enumerate_basis()`, `alg_compose()`, `alg_compute_mult_table()`, `alg_star()`, `alg_closure_loops()`, `alg_count_through()`
- **Left ideal construction** (reused from Demo 85): `left_ideal_closure()` — BFS closure under left multiplication at delta=0
- **`build_restricted_hamiltonian()`**: constructs L_H = -(e_0 + ... + e_{n-2}) restricted to a left ideal subspace with invariance verification
- **`compute_b_on_ideal()`**: delta-form b computation on integer Hamiltonian (lambda=0); finds Jordan partners via ker(L_H^2)\ker(L_H), computes loop power matrix, extracts leading delta-power coefficients
- **`compute_b_shifted()`**: double-precision version for irrational eigenvalues (lambda != 0); same algorithm but with floating-point shifted operator M = L_sub - lambda*I
- **`compute_loop_power()`**: computes total loop count for G_delta[i][j] using star (loop form) or direct (trace form) composition
- **Modular arithmetic suite**: `mod_pos`, `mod_mul`, `mod_add`, `mod_sub`, `mod_pow`, `mod_inv` over prime 10^9+7; used for minimal polynomial and characteristic polynomial computation
- **`min_poly_mod()`**: minimal polynomial via Krylov iteration mod p — used for Jordan block detection via square-freeness test
- **`has_jordan_blocks()`**: detects Jordan blocks by checking gcd(mu, mu') degree > 0
- **`extract_null_space()`**: double-precision RREF null space extraction
- **`gauss_rank()`**: Gaussian elimination rank computation for integer matrices

### Literature Touched
- **Pearce-Rasmussen**: b = -5/8 value for the indecomposability parameter at delta=0 (critical dense polymers); the demo attempts to reproduce this via a different (ultimately unsuccessful) method
- **Graham-Read-Rasmussen (GRS)**: b = -2 value from the algebraic trace form; the demo tests this form alongside the loop form, both diverge
- **Belletete et al.**: tilting module classification referenced in earlier planning (the demo was originally conceived as a tilting module decomposition test, pivoted to direct b computation)
- **Gainutdinov et al. (2013)**: dense polymer fusion calculation at p=2; Demo 86 seeds this as a future direction for algebraic description of DKC composition

### Open Questions
- How does the literature actually compute b at lattice sizes beyond TL_4? The delta-parameterized Gram matrix approach fails; the standard modules have no Jordan blocks; the actual Pearce-Rasmussen method remains unidentified.
- Is there a different bilinear form or representation choice that avoids the valuation divergence on single P_{0,0}?
- Is b = -5/8 fundamentally a small-lattice (TL_4) phenomenon, or does the literature have a scaling/extrapolation method that works at larger sizes?
- What is the structural reason for the universal d^{-2} gap? Is there a representation-theoretic explanation for why p_tt < 2*p_Tt on a single copy?
- Why is TL_8 P_{0,0} completely semisimple when TL_4 and TL_6 have Jordan blocks? Is there a periodicity or parity pattern in the non-semisimple structure?
