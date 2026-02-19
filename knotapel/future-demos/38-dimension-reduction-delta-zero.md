# Dimension Reduction at Delta=0

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P6AG6746TCJXQJBR4E8Q
**Dependencies:** Demo 29 (TL_n matrices)
**Code path:** `knotapel/demo_32_dimension_reduction/`

## Hypothesis

At delta=0, any diagram composition producing a closed loop gives zero, so the effective algebra TL_n/(loop=0) is smaller than full TL_n. The effective dimension may grow as ~2^n instead of C_n ~ 4^n/n^{3/2}, roughly doubling the practical strand-count ceiling. The effective algebra at delta=0 IS the algebraic structure underlying the Ising model (q=2 Potts model).

## Predictions

1. Significant dimension reduction at delta=0, especially for larger n.
2. For n=2: both basis elements survive but e_1 is nilpotent (e_1^2=0). Effective dimension 2 but constrained.
3. Jones-Wenzl projector p_2 = Id - e_1/delta is undefined at delta=0 (division by zero), but the limit has specific algebraic meaning.
4. Reduced matrix construction gives d x d matrices where d < C_n, further speeding computation.
5. The effective dimension sequence (n=2..10) follows a recognizable combinatorial pattern related to Ising model representation theory.

## Approach

- **Part A:** Null space analysis — for n=2..8, compute TL_n generator matrices at delta=0, identify null space.
- **Part B:** Effective dimension — determine dimension of TL_n/(loop=0) for each n.
- **Part C:** Jones-Wenzl projectors — investigate which projectors exist at delta=0, how they constrain the algebra.
- **Part D:** Reduced matrix construction — for each n, construct minimal-dimension faithful representation at delta=0.
- **Part E:** Comparison table — for n=2..10: C_n (full), effective dimension, reduction ratio, resulting speedup.
