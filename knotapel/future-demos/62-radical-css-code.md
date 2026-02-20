# Radical → CSS Code → Computable Functions

**Track:** E — Radical Anatomy + D — DKC Extensions (bridge)
**Status:** Proposed
**Dependencies:** Demo 51 (radical anatomy), Demo 52 (Chebyshev anatomy), Demos 55-59 (DKC results)

## Hypothesis

The radical of TL_n(δ) at each ℓ determines a CSS (Calderbank-Shor-Steane) quantum error-correcting code. The code parameters [n, k, d] should be [C_ℓ, 2ℓ-3, d_ℓ] where C_ℓ is the ℓ-th Catalan number and k = 2ℓ-3 is the number of logical qubits. The CSS code structure should predict which Boolean functions are naturally computable at that ℓ.

## Predictions

1. The radical basis vectors (from Demo 51/52) define the parity check matrix H of a CSS code.
2. The code distance d_ℓ corresponds to the nilpotency index of the radical.
3. Transversal gates on the CSS code correspond to achievable Boolean functions in DKC.
4. The CSS code's stabilizer group structure matches the Peirce decomposition from Demo 51.
5. The radical → CSS → computation pipeline formalizes why "nilpotency + representation diversity" (the two-source hypothesis) is needed for DKC computation.

## Approach

- **Part A:** Extract radical basis from Demo 51 anatomy (n=4,6 at δ=0; n=4,6 at δ=√2).
- **Part B:** Construct CSS code: radical basis → parity check matrix → code parameters [n,k,d].
- **Part C:** Compute transversal gate set of the CSS code. Compare with DKC-achievable Boolean functions.
- **Part D:** Test across multiple ℓ values: do CSS code properties track DKC achievability?
- **Part E:** Formalize the pipeline: ℓ → radical → CSS code → transversal gates → computable functions.

## Notes

This is the most theoretical of the batch. It bridges the radical anatomy track (Demos 51-52) with the DKC computation track (Demos 55-59). Success would mean the radical structure PREDICTS computation — not just enables it.

The CSS construction is standard: given a classical code C₁ ⊇ C₂, the CSS code is CSS(C₁, C₂) with X-stabilizers from C₂⊥ and Z-stabilizers from C₁. The radical's self-orthogonality (rad ⊆ rad⊥ in the Gram form) is exactly the CSS compatibility condition.
