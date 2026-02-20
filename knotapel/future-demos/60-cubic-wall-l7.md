# ℓ=7 Cubic Wall

**Track:** E — Radical Anatomy
**Status:** Proposed
**Dependencies:** Demo 51 (radical anatomy at δ=0), Demo 52 (Chebyshev anatomy at δ=1,√2,√3)

## Hypothesis

At ℓ=7, the Chebyshev arithmetic enters the first cubic number field. The minimal polynomial is x³-x²-2x+1=0, and δ is a root. This is the first non-quadratic case and tests whether all dimension formulas from Demos 51-52 extend universally.

## Predictions

1. rad(TL_7(δ)) has dimension given by the Catalan difference formula: C_{2k} - C_{2k-1} (if even n) or 0 (if odd n, semisimple).
2. rad² = Catalan dimension C_{2k-1} (the Gram rank).
3. Nilpotency index = 3 for all even n ≥ 4 at this ℓ.
4. Fibonacci rank formula F(ℓ-1) = F(6) = 8 orthogonal idempotents.
5. Quotient conjecture: TL_{2k}(δ)/rad ≅ TL_{2k-1}(δ) (dimension match).

All predictions are sharp pass/fail. If any fails, the universality of the radical anatomy breaks at the cubic boundary.

## Approach

- **Part A:** Implement cubic number field arithmetic for δ = root of x³-x²-2x+1.
- **Part B:** Build TL multiplication tables for n=2 through n=8 (C_8 = 1430 basis elements).
- **Part C:** Compute rad, rad², Gram matrix rank. Test all dimension predictions.
- **Part D:** Find orthogonal idempotents. Verify Peirce decomposition.
- **Part E:** Test quotient conjecture by comparing dimensions.

## Notes

The n=8 case (1430 basis elements, 1430×1430 multiplication) will be the computational bottleneck. The pass-by-pointer refactoring from Demo 52 is essential. Cubic arithmetic adds complexity: each coefficient is a triple (a, b, c) representing a + b·δ + c·δ².
