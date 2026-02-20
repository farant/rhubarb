# Q=3 Potts (ℓ=6, δ=√3)

**Track:** D — DKC Extensions
**Status:** Proposed
**Dependencies:** Demo 52 (anatomy at ℓ=6), Demo 56 (Potts-TL dictionary)

## Hypothesis

At ℓ=6, δ=√3, Q=δ²=3. This is the 3-state Potts model. We have radical anatomy data (Demo 52) but no DKC search or axiality analysis. The FK ring lives in Z[ζ₁₂]. Does the extra algebraic dimension dissolve encoding walls for non-linear functions?

## Predictions

1. Bracket values live in Z[ζ₂₄] (since A = e^{i·7π/12} = ζ₂₄⁷). Axiality ring TBD.
2. FK partition functions live in Z[ζ₁₂] (even subring), richer than ℓ=4's Z[ζ₈].
3. Catalog size larger than ℓ=4 (56 values) — more representation diversity at higher ℓ.
4. Q=3 FK catalog may reach 0x1B with sector activations (where Q=2 FK couldn't with 9 values).
5. The Q=1→Q=2→Q=3 hierarchy shows increasing computational richness.

## Approach

- **Part A:** Build bracket catalog at δ=√3 using Cyc24 arithmetic.
- **Part B:** Determine axiality ring. Check Z[ζ₃]-axial or richer.
- **Part C:** FK partition function catalog at Q=3, bracket coupling v = -A⁻².
- **Part D:** Full DKC search (bracket and FK) with multiplicative encoding.
- **Part E:** Q-hierarchy comparison: Q=1 (dead) → Q=2 (12/13) → Q=3 (?). Does computational power grow monotonically with Q?

## Notes

Cyc24 arithmetic (Z[ζ₂₄]) has a 12-dimensional basis (ζ₂₄¹² = -1), more expensive than Cyc16 (8D). May need larger structs. Demo 52 confirmed radical anatomy exists at ℓ=6; this demo extends to the DKC computation question.
