# Additive Encoding DKC

**Track:** D — DKC Extensions
**Status:** Proposed
**Dependencies:** Demo 55 (multiplicative DKC at ℓ=4)

## Hypothesis

The 0x1B and 0x06 NPN classes are unreachable at Re>0 with multiplicative encoding due to non-linear TRUE sets fighting against the (Z/2)³ group structure. The coding theory analysis (Demo 56 five-lens) predicts that additive encoding z(a,b,c) = a·w₁ + b·w₂ + c·w₃ dissolves this wall because addition doesn't impose the arc-contiguity constraint that multiplication does.

## Predictions

1. Additive encoding + Re>0 achieves 0x1B and 0x06 (currently unreachable).
2. Some currently-easy multiplicative functions may become hard under addition (encoding complementarity).
3. The union of multiplicative and additive achievable sets covers all 13 NPN classes with just Re>0.
4. Parity (0x69) remains achievable under additive encoding.
5. The z(0,0,0) = 0 constraint (vs multiplicative z(0,0,0) = 1) flips the structural bias: bit 0 is now fixed to 0 instead of dependent on activation.

## Approach

- **Part A:** Build additive DKC framework using existing bracket catalog (56 values at δ=√2).
- **Part B:** 2-input additive search: z(x₁,x₂) = x₁·w₁ + x₂·w₂. Compare with multiplicative 8/16.
- **Part C:** 3-input NPN search with all activations. Focus on 0x1B and 0x06 status.
- **Part D:** Encoding complementarity analysis: which functions are multiplicative-only, additive-only, or both?
- **Part E:** Combined achievability: what's the minimum activation complexity to reach all 13 NPN with the optimal encoding choice?

## Priority

**First** — direct prediction test. Quick to implement (reuse Demo 55 framework, swap encoding). Confirms or refutes the coding theory criterion from the five-lens analysis.
