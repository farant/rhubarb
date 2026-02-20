# Radical Dimension Formula for Temperley-Lieb Algebras at Roots of Unity

## Theorem

For all ℓ ≥ 2,

**rad(TL_ℓ(2cos(π/ℓ))) = 2ℓ − 3**

where TL_n(δ) is the Temperley-Lieb algebra on n strands with loop parameter δ, and rad denotes the dimension of the Jacobson radical (equivalently, the kernel of the trace form).

**Key intuition:** At the critical level n = ℓ, exactly one cell module (V_{ℓ-2}) develops a one-dimensional kernel in its bilinear form. The radical dimension 2ℓ − 3 is entirely determined by this single degeneration, via the cellular algebra structure.

## Definitions

**Temperley-Lieb algebra** TL_n(δ): the algebra with basis = Catalan-many (C_n) planar matchings on 2n points, multiplication by diagram composition with loop parameter δ (each closed loop contributes a factor of δ).

**Cell modules** V_j (j ≡ n mod 2, 0 ≤ j ≤ n): the Graham-Lehrer cell modules indexed by through-strand count j. Dimension d(n,j) = C(n,(n-j)/2) − C(n,(n-j)/2−1). Basis: planar half-diagrams with j through-strands and (n-j)/2 non-crossing adjacent arcs.

**Cell module bilinear form** φ: V_j × V_j → k: defined by composing half-diagrams and extracting the coefficient in the j-th cell ideal. If ker(φ) has dimension k_j, then V_j contributes k_j(2d_j − k_j) to the radical dimension.

**Quantum integer** [m] = sin(mπ/ℓ)/sin(π/ℓ) at δ = 2cos(π/ℓ). Note [ℓ] = 0.

## Proof

### Part A: V_{ℓ-2} has corank exactly 1

**Step 1 (Basis).** V_{ℓ-2} at level n = ℓ has dimension d(ℓ, ℓ-2) = ℓ − 1. Its basis consists of half-diagrams h_1, …, h_{ℓ-1} where h_i has a single arc connecting adjacent nodes (i, i+1) and through-strands at all other positions. These are the only valid half-diagrams because a non-adjacent arc would enclose a through-strand, violating planarity.

**Step 2 (Tridiagonal structure).** The cell module bilinear form has entries:

- φ(h_i, h_i) = δ — the matching arcs form one closed loop
- φ(h_i, h_{i±1}) = 1 — overlapping arcs create a path through the interface with zero loops; the through-strand matching preserves cell order
- φ(h_i, h_k) = 0 for |i−k| ≥ 2 — the through-strand matching creates a permutation that drops the result into a lower cell ideal

This gives the (ℓ-1) × (ℓ-1) tridiagonal Gram matrix:

```
G = [[δ, 1, 0, …, 0],
     [1, δ, 1, …, 0],
     [0, 1, δ, …, 0],
     …
     [0, …, 0, 1, δ]]
```

**Step 3 (Chebyshev determinant).** This tridiagonal matrix has determinant satisfying the recurrence D_m = δ · D_{m-1} − D_{m-2} with D_0 = 1, D_1 = δ. This is the Chebyshev polynomial of the second kind:

D_m = U_m(δ/2)

At δ = 2cos(π/ℓ):

**D_{ℓ-1} = U_{ℓ-1}(cos(π/ℓ)) = sin(ℓπ/ℓ)/sin(π/ℓ) = sin(π)/sin(π/ℓ) = 0**

**Step 4 (Corank = 1).** The leading (ℓ-2) × (ℓ-2) principal minor is:

D_{ℓ-2} = U_{ℓ-2}(cos(π/ℓ)) = sin((ℓ-1)π/ℓ)/sin(π/ℓ) = 1

Since D_{ℓ-2} ≠ 0 and D_{ℓ-1} = 0, the matrix has rank exactly ℓ − 2, hence corank exactly 1.

**Kernel vector:** v_i = (−1)^i · sin(iπ/ℓ), the Chebyshev eigenvector for mode k = ℓ−1 (zero eigenvalue). For ℓ = 5: v ∝ (1, −φ, φ, −1) where φ is the golden ratio.

### Part B: No other cell module degenerates at n = ℓ

**Step 5 (Graham-Lehrer factor structure).** The Gram determinant of cell module V_j at level n involves quantum integer factors [j+k+1] for k = 1, …, (n−j)/2, with positive multiplicities. The maximum index appearing is:

max{j + k + 1} = j + (n−j)/2 + 1 = (n + j + 2)/2

**Step 6 (Uniqueness).** At n = ℓ, the maximum factor index for V_j is (ℓ + j + 2)/2. This reaches ℓ (where [ℓ] = 0) if and only if j ≥ ℓ − 2.

- For j < ℓ − 2: max index < ℓ, so all factors [j+k+1] are nonzero. **V_j is non-degenerate.**
- For j = ℓ − 2: max index = ℓ, and [ℓ] = 0. **V_{ℓ-2} degenerates** (as shown in Part A).
- For j = ℓ: dim = 1, Gram = [[1]]. **Trivially non-degenerate.**

Verified computationally:
- V_0 at n = 4: det = [2]² · [3] = δ²(δ²−1), nonzero at δ = √2 (ℓ = 4) ✓
- V_2 at n = 4: det = [4] = δ³ − 2δ, zero at δ = √2 (ℓ = 4) ✓

### Conclusion

**rad(TL_ℓ) = Σ_j k_j(2d_j − k_j) = k_{ℓ-2} · (2d_{ℓ-2} − k_{ℓ-2}) = 1 · (2(ℓ−1) − 1) = 2ℓ − 3** ∎

## Corollary: Next-Level Formula

**Corollary.** For all ℓ ≥ 3, rad(TL_{ℓ+1}(2cos(π/ℓ))) = ℓ² − ℓ − 3.

*Proof sketch.* At n = ℓ+1, the unique degenerate cell module is V_{ℓ-3} (shifted parity). Its dimension is d(ℓ+1, ℓ-3) = (ℓ+1)(ℓ-2)/2. The Graham-Lehrer factors include [(ℓ-3)+((ℓ+1-(ℓ-3))/2)+1] = [ℓ] = 0 with multiplicity d(ℓ-3, ℓ-3) = 1, giving corank 1. Contribution: 2 · (ℓ+1)(ℓ-2)/2 − 1 = (ℓ+1)(ℓ-2) − 1 = ℓ² − ℓ − 3. ∎

## Computational Verification

| ℓ | δ | C_ℓ | Fixpt rank | Radical | 2ℓ−3 | Match |
|---|---|-----|-----------|---------|-------|-------|
| 2 | 0 | 2 | 1 | 1 | 1 | ✓ |
| 3 | 1 | 5 | 2 | 3 | 3 | ✓ |
| 4 | √2 | 14 | 9 | 5 | 5 | ✓ |
| 5 | φ | 42 | 35 | 7 | 7 | ✓ |
| 6 | √3 | 132 | 123 | 9 | 9 | ✓ |
| 7 | 2cos(π/7) | 429 | 418 | 11 | 11 | ✓ |

Cell module verification (23/23 tests pass):
- At each ℓ, V_{ℓ-2} is the unique degenerate cell module with corank 1
- All other cell modules have full rank
- Tridiagonal Gram structure confirmed by direct matrix construction
- Computed using mod-p arithmetic (p = 1000002361 ≡ 1 mod 840)

## Related Demos

- **Demo 51**: Initial Gram rank computation establishing the 2ℓ−3 pattern
- **Demo 52**: Verification across quadratic number fields (ℓ = 3, 4, 5, 6)
- **Demo 60**: Cubic number field verification (ℓ = 7), cross-validated at two primes
- **Demo 39 Part G**: Cell module Gram matrix computation and verification (23/23 tests)

## Open Questions

1. **Markov trace radical**: The Markov trace gives radical dimension 2ℓ−2 (always one more). Why? Possibly related to the Jones polynomial at roots of unity.
2. **Nilpotency index**: rad² is 1-dimensional and rad³ = 0 (nilpotency 3). Can this be proved from the single-kernel structure?
3. **Fibonacci rank**: The bilinear rank of the rad² generator equals F(ℓ−1). Connection to the kernel vector?

## Notes

- The ℓ = 5 (golden ratio) case initially appeared semisimple due to a field arithmetic bug: p = 10⁹+7 has 5 as a quadratic non-residue, so √5 ∉ GF(p). Fixed by using p = 1000002361 (≡ 1 mod 840).
- The proof was developed in the "triumvirate" working mode (explorer + team-lead + Fran) during the Knotapel DKC research arc, February 2026.
