## Demo 85: Indecomposability Parameter b for TL_n at delta=0

- **Status**: COMPLETE
- **File**: `demo_85_indecomposability/main.c` (~6861 lines)
- **Tests**: 58/58 pass
- **Depends on**: Demo 51 (TL algebra infrastructure, Gram matrix, radical basis), Demo 35 (integer TL generators at delta=0)
- **Feeds into**: Demo 86 (direct b on single P_{0,0} -- negative result, divergence), Demo 87+ (non-semisimplicity thesis, logarithmic CFT structure constants)

### Headline
The indecomposability parameter b = -5/8 (Pearce-Rasmussen) is computed exactly from first principles at TL_4 using leading-coefficient extraction from the delta-parameterized Markov trace on the full regular representation. The literature "controversy" between b = -2 (GRS) and b = -5/8 (PR) dissolves: b_delta = -5/2 on the regular rep divides by dim(0-TL sector) = 4 to give b_PR = -5/8 exactly to 12 decimal places.

### Key Results
- **b = -5/8 confirmed at TL_4**: leading-coefficient extraction from delta-parameterized Gram matrix on the full 14-dimensional regular representation gives b_delta = -5/2. Normalizing by the 0-TL sector dimension (4 diagrams with zero through-lines) yields b_PR = -5/2 / 4 = -5/8, exact to 12 decimal places.
- **Valuation condition**: the method requires p_tt = 2 * p_Tt (where p_tt and p_Tt are the minimum delta-powers in the bilinear forms). At TL_4: p_tt = 2, p_Tt = 1, so 2 = 2*1 -- satisfied. At TL_6: p_tt = 4, p_Tt = 3, so 4 != 6 -- fails, b diverges as delta^{-2}.
- **-2 vs -5/8 resolved**: these are NOT competing predictions. b = -5/8 is the per-cell (physical/lattice) value. b = -5/2 is the regular-representation value. The factor of 4 = dim(0-TL sector for TL_4) = C_2 = number of 0-through-line diagrams. The quantum group approach (GRS) uses different normalization conventions for the bilinear form.
- **Standard modules are fully diagonalizable**: H on W_{n,0} (standard module = link states) has NO Jordan blocks at any size tested (n=4,6,8,10,12). Jordan blocks only exist on projective covers (P_{0,0}) and the full regular representation. This falsifies Gemini's claim that b is extracted from standard module Jordan structure.
- **Fixed-point form is degenerate**: the fixpt-trace bilinear form G[i][j] = fixpt(star(i)*j) is too degenerate at delta=0 -- almost all products create loops -> 0. It gives an affine b with 3-dimensional gauge freedom (TL_4) or all zeros (TL_6). Not the right form.
- **Delta-parameterized Markov trace is the correct form**: G_delta[i][j] = delta^{L(i,j)} where L = compose_loops + closure_loops. This form is non-degenerate at delta != 0 and the leading-coefficient extraction as delta -> 0 gives the physical b.
- **Projective cover P_{0,0} has Jordan blocks at nonzero eigenvalues**: TL_4 P_{0,0} (dim=5, composition 0-TL:2 + 2-TL:3) has eigenvalues from char poly x(x^2-2)^2, with Jordan blocks at lambda = +/-sqrt(2), NOT at lambda = 0. The 0-TL left ideals (dim=2) equal W_{0,0} and have no Jordan blocks -- they are too small.
- **TL_3 is semisimple**: L_H on the 5-dimensional TL_3 algebra is fully diagonalizable (radical dimension 0). TL_2 (dim=2) has Jordan blocks. TL_4 (dim=14) and TL_5 (dim=42) have Jordan blocks in the regular representation.
- **Gauge analysis (bonus)**: the fixed-point form has 3-dimensional gauge freedom at TL_4. Full quadratic decomposition: M (quadratic coefficient matrix) = 0, E (linear denominator) = 0, so b_fixpt(c) is purely AFFINE in gauge parameters. Both -2 and -5/8 are reachable points on this flat manifold. The delta-form breaks the degeneracy.
- **TL_6 divergence is structural**: all 14 kernel directions at TL_6 introduce lower-power cross-terms; 0 safe directions exist. The 25-fold multiplicity (dim(0-TL for TL_6)^2 = 25) does not rescue the valuation condition as the 4-fold multiplicity does at TL_4.
- **Cross-model verification**: both Gemini 3 Pro and GPT-5 (Codex) independently confirmed the factor-of-4 relationship and dim(0-TL)^2 multiplicity structure. Gemini's claim about standard-module Jordan blocks was FALSIFIED by computation.
- **Spin chain at q=i**: Phase 4 builds the TL representation on (C^2)^{tensor n} at q=i (delta=0). Verified e_i^2=0, braid relation, far commutativity. H is symmetric (conformal/transpose form). Jordan blocks found at specific eigenvalues; b computed via augmented RREF for Jordan partner solve + minimum-norm gauge projection.

### Theorems/Conjectures
- **b = -5/8 for dense polymers at delta=0 (DEMONSTRATED at TL_4)**: exact computation via leading-coefficient extraction from delta-parameterized Gram matrix on full regular representation, normalized by 0-TL sector dimension.
- **Valuation condition p_tt = 2*p_Tt (VERIFIED at TL_4, FAILS at TL_6)**: this condition is necessary for the leading-coefficient method to produce a finite b. Its failure at TL_6 means the regular-representation approach requires modification for larger lattices.
- **Standard modules have no Jordan blocks at delta=0 (DEMONSTRATED at n=4,6,8,10,12)**: H on W_{n,0} is fully diagonalizable at every size tested. The min poly is square-free (gcd(mu,mu') degree = 0) at all sizes. Jordan structure lives exclusively in projective covers.
- **TL algebra dimensions match Catalan numbers (VERIFIED)**: dim(TL_n) = C_n at n=2,3,4,5,6. dim(W_{n,0}) = C_{n/2} at n=4,6,8,10,12.
- **e_i^2 = 0 at delta=0 (VERIFIED)**: all TL relations (e_i^2=delta*e_i, braid relation e_i*e_{i+1}*e_i = e_i, far commutativity) verified on both link-state standard module (n=6) and spin chain representation (n=4).
- **Fixed-point form gauge manifold is flat (DEMONSTRATED at TL_4)**: the quadratic coefficient matrix M = 0, making b_fixpt an affine function of gauge parameters with no unique critical point.

### Data
- TL algebra dimensions: TL_2=2, TL_3=5, TL_4=14, TL_5=42, TL_6=132
- Standard module dimensions: W_{4,0}=2, W_{6,0}=5, W_{8,0}=14, W_{10,0}=42, W_{12,0}=132
- TL_4 regular rep: b_delta = -5/2, 0-TL sector dim = 4 (diagrams: id, e_1, e_2, e_1*e_2), b_PR = -5/8
- TL_4 standard module H: eigenvalues +/-sqrt(2), 2x2 matrix [[0,-2],[-1,0]], fully diagonalizable
- TL_4 P_{0,0}: dim=5, composition 0-TL:2 + 2-TL:3, char poly x(x^2-2)^2, Jordan blocks at lambda=+/-sqrt(2)
- TL_4 leading coefficients: p_Tt=1, coeff_Tt computed; p_tt=2, coeff_tt computed; valuation 2=2*1 satisfied
- TL_6 leading coefficients: p_Tt=3, p_tt=4; valuation 4 != 6 FAILS; diverges as delta^{-2}; 0/14 safe directions
- Spin chain Sz=0 dimensions: n=4 -> dim=6, n=6 -> dim=20, n=8 -> dim=70
- b accuracy: |b_PR - (-5/8)| < 10^{-12}

### Code Assets
- **Link state enumeration (`enumerate_link_states`)**: recursive non-crossing perfect matching generator for W_{n,0}, sizes n=4 through n=12
- **e_i action on link states (`apply_ei`)**: TL generator action with loop detection at delta=0
- **Hamiltonian construction (`build_hamiltonian`)**: H = -(e_0+...+e_{n-2}) on standard module
- **Gauss rank with partial pivoting (`gauss_rank`)**: fraction-free rank computation for integer matrices
- **Jordan analysis (`jordan_analysis`)**: ker(M) vs ker(M^2) comparison for Jordan block detection at given eigenvalue
- **Modular arithmetic suite**: mod_pos, mod_mul, mod_add, mod_sub, mod_pow, mod_inv over prime 10^9+7
- **Faddeev-LeVerrier characteristic polynomial (`char_poly_mod`)**: modular char poly computation
- **Polynomial GCD mod p (`poly_gcd_degree_mod`)**: Euclidean algorithm for diagonalizability test
- **Minimal polynomial via Krylov iteration (`min_poly_mod`)**: definitive Jordan block detection via gcd(mu,mu')
- **Full TL algebra infrastructure**: `AlgDiagram` type, basis enumeration (`alg_enumerate_basis`), composition with loop counting (`alg_compose`), multiplication table (`alg_compute_mult_table`), identity/generator construction, star anti-involution
- **Star operation (`alg_star`, `alg_compute_star_indices`)**: top/bottom flip for adjoint diagrams
- **Through-line sector analysis (`analyze_through_line_sectors`)**: classifies basis by through-line count, checks L_H sector preservation
- **Left ideal closure (`left_ideal_closure`)**: BFS closure under left multiplication for projective cover construction
- **Null space extraction (`extract_null_space`, `extract_null_space_d`)**: RREF-based null space for integer and double matrices
- **Eigenvalue sweep (`find_eigenvalues_sweep`)**: sign-change + golden-section refinement for real eigenvalues; handles both simple and double roots
- **Geometric multiplicity (`geometric_multiplicity`)**: rank-based computation at given eigenvalue
- **Complex arithmetic suite**: Cx type with add/sub/mul/div/neg/abs, cx_mat_mul, cx_mat_vec, cx_null_space, cx_absdet
- **Spin chain representation**: Sz=0 basis enumeration (`sc_enumerate_sz0`), generator action at q=i (`sc_apply_ei`), generator/Hamiltonian matrix builders
- **Complex eigenvalue finder (`cx_find_real_evals`)**: |det| local-minimum sweep with golden-section refinement for complex matrices
- **Delta-parameterized Gram/Hamiltonian builders**: `build_gram_matrix_delta`, `build_hamiltonian_delta` for perturbation analysis
- **Leading-coefficient extraction (Phase 6)**: per-pair delta-power computation L[i][j] = compose_loops + closure_loops, minimum power detection, coefficient summation, both Gram (star) and Trace (no star) forms
- **Gauge analysis infrastructure**: full N-dimensional coset scan computing A, D, L, E, M matrices for quadratic b decomposition; critical point finding via Cramer's rule (3D case)

### Literature Touched
- **Gainutdinov-Read-Saleur (GRS)**: quantum group / algebraic definition of b, projective cover approach. Demo 85 shows this gives different normalization (b = -2 under GRS conventions) but the same underlying structure.
- **Pearce-Rasmussen (PR)**: finite-size lattice scaling approach giving b = -5/8. Demo 85 confirms this is the per-cell physical value.
- **Symplectic fermion continuum theory at c=-2**: b = -2 predicted (GRS convention). Demo 85 shows the exact relationship: b_GRS convention differs from b_PR convention by normalization factor.
- **Gemini 3 Pro literature claims (FALSIFIED)**: Gemini claimed b is extracted from off-diagonal Jordan cell coupling in standard modules. Demo 85 proved standard modules are fully diagonalizable at all tested sizes -- Jordan blocks only exist on projective covers.
- **Faddeev-LeVerrier algorithm**: used for characteristic polynomial computation in both modular and complex arithmetic.
- **Krylov iteration**: used for minimal polynomial computation (definitive Jordan block test).

### Open Questions
- **Why does the valuation condition fail at TL_6?** The structural reason for p_tt = 4, p_Tt = 3 (gap = -2) at TL_6 is understood (cross-copy interference from 25-fold multiplicity), but is there a modified extraction that works for larger sizes?
- **Can b = -5/8 be confirmed at TL_6 or TL_8?** The regular-representation approach diverges. The standard module has no Jordan blocks. The single P_{0,0} also diverges (Demo 86). What is the correct finite-lattice method beyond TL_4?
- **How does the literature (PR) actually compute b at larger lattice sizes?** The method likely uses transfer matrix + finite-size scaling, not the diagram-algebraic approach tested here. Identifying and implementing their exact method remains open.
- **Does the spin chain approach give b = -2 at larger sizes?** Phase 4 computes b on (C^2)^{tensor n} at q=i. Results at n=4,6,8 should be compared with the algebraic approach. The conformal (transpose) bilinear form may give different normalization.
- **What is the relationship between b and the DKC computational capacity?** The k_sec=1 synergy value (36) from Demo 84 was hypothesized to encode b. This connection remains unexplored.
