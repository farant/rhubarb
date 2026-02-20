# Radical of TL_{ℓ+1}(2cos(π/ℓ)): The Next-Level Formula

## Theorem

For ℓ ≥ 3,

$$\dim \operatorname{rad}(TL_{\ell+1}(2\cos(\pi/\ell))) = \ell^2 - \ell - 3$$

## Setup

Same framework as the base-level proof (see `radical-dimension-formula.md`). We work with TL_n(δ) at δ = 2cos(π/ℓ) and n = ℓ+1. Quantum integers [m] = sin(mπ/ℓ)/sin(π/ℓ), with [ℓ] = 0 as the unique vanishing quantum integer in [1, 2ℓ-1].

Cell modules V_j^{(n)} indexed by through-strand count j ≡ n (mod 2), 0 ≤ j ≤ n, with dimension d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1). The Jacobson radical dimension decomposes as:

rad(TL_n) = Σ_j k_j(2d_j - k_j)

where k_j = dim rad(V_j) = corank of the Gram bilinear form on V_j.

## Proof

### Part A: V_{ℓ-3} is the unique degenerate cell module

At n = ℓ+1, cell modules V_j exist for j ∈ {ℓ+1, ℓ-1, ℓ-3, ..., 1 or 0}.

By the Graham-Lehrer criterion, V_j is degenerate iff there exists k with 1 ≤ k ≤ (n-j)/2 such that [j+k+1] = 0, equivalently j+k+1 = ℓ, i.e., k = ℓ-j-1.

The constraint 1 ≤ k ≤ (ℓ+1-j)/2 requires:

- **Lower bound** (k ≥ 1): j ≤ ℓ-2
- **Upper bound** (2k ≤ ℓ+1-j): 2(ℓ-j-1) ≤ ℓ+1-j, giving j ≥ ℓ-3

So j must satisfy ℓ-3 ≤ j ≤ ℓ-2, with the parity constraint j ≡ ℓ+1 (mod 2).

Since ℓ-3 ≡ ℓ+1 (mod 2) and ℓ-2 ≡ ℓ (mod 2) ≠ ℓ+1 (mod 2), only j = ℓ-3 survives.

**V_{ℓ-3} is the unique degenerate cell module at n = ℓ+1.** □

### Part B: Dimension

d(ℓ+1, ℓ-3) = C(ℓ+1, 2) - C(ℓ+1, 1) = (ℓ+1)ℓ/2 - (ℓ+1) = **(ℓ+1)(ℓ-2)/2** □

### Part C: Corank = 1

**Key observation**: n = ℓ+1 is the *first* level where V_{ℓ-3} becomes degenerate.

*Proof*: V_{ℓ-3} exists at levels n ∈ {ℓ-3, ℓ-1, ℓ+1, ...} (same parity as ℓ-3).
- At n = ℓ-3: (n-j)/2 = 0, no Gram factors, trivially nondegenerate.
- At n = ℓ-1: (n-j)/2 = 1, sole factor [(ℓ-3)+1+1] = [ℓ-1] = 1 ≠ 0.
- At n = ℓ+1: first appearance of factor [ℓ] = 0. ✓

Since this is the first degeneracy, the **Graham-Lehrer linking theorem** applies: there exists a non-trivial homomorphism

θ: V_{j'}^{(ℓ+1)} → V_{ℓ-3}^{(ℓ+1)}

whose image equals rad(V_{ℓ-3}), where the linked index is:

j' = (ℓ-3) + 2k = (ℓ-3) + 2(ℓ-(ℓ-3)-1) = (ℓ-3) + 2·2 = **ℓ+1 = n**

But V_n^{(n)} is the trivial cell module with **dim 1**. Since θ is non-trivial and its domain is 1-dimensional, θ is injective. Therefore:

**dim rad(V_{ℓ-3}) = 1**, i.e., corank = 1. □

### Conclusion

rad(TL_{ℓ+1}) = 1 · (2 · (ℓ+1)(ℓ-2)/2 - 1) = (ℓ+1)(ℓ-2) - 1 = **ℓ² - ℓ - 3** □

## Structural Parallel with Base Level

| | n = ℓ | n = ℓ+1 |
|---|---|---|
| Unique degenerate module | V_{ℓ-2} | V_{ℓ-3} |
| Dimension | ℓ-1 | (ℓ+1)(ℓ-2)/2 |
| Critical factor index | k=1 | k=2 |
| Vanishing factor | [ℓ]=0 | [ℓ]=0 |
| Linked module | V_ℓ^{(ℓ)} (dim 1) | V_{ℓ+1}^{(ℓ+1)} (dim 1) |
| Corank | 1 | 1 |
| Radical formula | 2(ℓ-1)-1 = 2ℓ-3 | (ℓ+1)(ℓ-2)-1 = ℓ²-ℓ-3 |

**The linked module is V_n^{(n)} in both cases.** This is structural, not coincidental:

When V_j first degenerates at level n = 2ℓ-j-2, the linked index is j' = j + 2(ℓ-j-1) = 2ℓ-j-2 = n. So the linked module is always V_n^{(n)}, which always has dimension 1.

**Corollary (Universal Corank 1).** At the first degeneracy level of *any* cell module V_j, the corank is exactly 1. This holds universally for TL algebras at roots of unity.

## Verification

| ℓ | n=ℓ+1 | V_{ℓ-3} | dim | ℓ²-ℓ-3 | Computed |
|---|---|---|---|---|---|
| 3 | 4 | V_0 | 2 | 3 | 3 ✓ |
| 4 | 5 | V_1 | 5 | 9 | 9 ✓ |
| 5 | 6 | V_2 | 9 | 17 | 17 ✓ |
| 6 | 7 | V_3 | 14 | 27 | 27 ✓ |
| 7 | 8 | V_4 | 20 | 39 | 39 ✓ |

Computational verification: Demo 39 Parts G+H (p = 1000002361 ≡ 1 mod 840), 28/28 tests pass. Part H verified the n=ℓ+1 formula directly for ℓ=3..7, including full fixpt Gram rank and cell module Gram corank=1 for V_{ℓ-3}.

## Answers to Open Questions (from Demo 39 Parts I+J)

1. **Beyond first degeneracy (ANSWERED)**: At n = ℓ+2, V_{ℓ-2} has second-degeneracy corank = **ℓ+1** (verified ℓ=3..6). V_{ℓ-4} has first-degeneracy corank = 1 (universal). The fixpt radical at n=ℓ+2 is:

   rad = (ℓ+1)·(2d₁-(ℓ+1)) + 1·(2d₂-1)

   where d₁ = (ℓ+2)(ℓ-1)/2 and d₂ = (ℓ+2)(ℓ+1)(ℓ-3)/6. Verified for ℓ=4,5,6.

2. **General formula for n = ℓ+m**: No clean closed form exists for arbitrary m. At m=3, multiple modules degenerate with high corank. The fixpt_rad grows rapidly and doesn't reduce to a simple polynomial.

3. **Markov radical at n = ℓ+1 (ANSWERED)**: markov_rad = 2ℓ²-ℓ-2. The "+1" pattern does NOT persist — the Markov excess is ℓ²+1 (quadratic, not constant). At n=ℓ+2 the excess is (ℓ+1)²+1, but the pattern breaks at n=ℓ+3 (excess 98, 233, 450 for ℓ=3,4,5 — not (ℓ+2)²+1).

## Related

- Base proof: `radical-dimension-formula.md` (this folder)
- Computational verification: Demo 39 Parts G, H, I, J
- DKC research arc: Demos 26-29, 45-50
