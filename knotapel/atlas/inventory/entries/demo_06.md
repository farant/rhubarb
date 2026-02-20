## Demo 06: Jones-Wenzl Idempotents

- **Status**: COMPLETE
- **File**: `demo_06_jones_wenzl/main.c` (~800 lines)
- **Tests**: 19/19 pass (4 parts: A=4, B=3, C=4, D=4x2 braids = 8 minimum, varies)
- **Depends on**: Demo 03 (TL standard modules, bracket formula), Demo 04 (tensor product R-matrix, quantum trace)
- **Feeds into**: Any demo needing projectors onto irreducible components; Markov trace decomposition

### Headline
Constructs scaled Jones-Wenzl idempotents p_2 (4x4) and p_3 (8x8) over Laurent polynomials, proving they are the projectors that explain Demo 03/04's quantum dimension weights via Schur-Weyl decomposition.

### Key Results
- TL relations verified on V^{tensor 3} (8x8): U_i^2 = delta*U_i, U_1*U_2*U_1 = U_1, U_2*U_1*U_2 = U_2
- Scaled JW p_2 (Q_2 = delta*I - U, D_2 = delta): Q_2^2 = delta*Q_2, U*Q_2 = 0, tr_q(Q_2) = delta*[3]_q
- Scaled JW p_3 (Q_3 = [3]_q*iota(Q_2) - iota(Q_2)*U_2*iota(Q_2), D_3 = delta*[3]_q): Q_3^2 = D_3*Q_3, U_1*Q_3 = 0, U_2*Q_3 = 0, tr_q(Q_3) = D_3*[4]_q
- **Markov trace decomposition**: W_3 component tr_q(Q_3*rho) = D_3*[4]_q*tr_{W_3}(rho) verified for identity, sigma_1, trefoil, figure-eight
- W_1 component verified divisible by D_3*[2]_q, confirming both module traces factor correctly
- Sum W_1 + W_3 components = D_3*tr_q(rho) verified (decomposition is exhaustive)

### Theorems/Conjectures
- Jones-Wenzl recursion stays polynomial via scaling trick: Q_n = D_n*p_n avoids rational function entries
- Key simplification: [2]_q/delta = -1 keeps n=3 recursion entirely in Z[A, A^{-1}]
- On W_3 (all through-lines, 1D module): each braid generator acts as A^{-1}, so tr_{W_3}(rho(beta)) = product of A^{-sgn(word[i])}
- Quantum dimensions [n+1]_q computed via recurrence: [n+1]_q = [2]_q*[n]_q - [n-1]_q

### Code Assets
- `Poly` Laurent polynomial ring with `p_neg`, `p_sub`, `p_div_exact` (exact polynomial division with remainder check) — extensions beyond Demo 04
- `Mat` polynomial matrix library (same MAXN=8 infrastructure) with `mat_is_zero`
- Quantum dimension computation via Chebyshev recurrence ([n+1]_q = [2]_q*[n]_q - [n-1]_q)
- `braid_product()` mapping braid words to 8x8 matrix products
- Scaled Jones-Wenzl construction pattern: avoids rational functions entirely

### Literature Touched
- Jones-Wenzl idempotents (original construction)
- Temperley-Lieb algebra relations
- Schur-Weyl duality decomposition for U_q(sl_2)
- Markov trace and its decomposition into irreducible components
- Quantum dimensions as traces of idempotents
