# TL_n Matrix Representations

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P68NZF13K960AZM8JAH8
**Dependencies:** Demo 19 (braid circuit compiler), Demo 24 (TL_2 algebra)
**Code path:** `knotapel/demo_29_tl_matrices/`

## Hypothesis

The bracket satisfies a skein relation: at each crossing, the result decomposes as A times one smoothing plus A^{-1} times the other. For n strands, the space of partially-resolved diagrams is spanned by TL_n, with dimension = nth Catalan number C_n. Each braid generator sigma_i acts as a C_n x C_n matrix. A braid word with k crossings becomes a product of k matrices. Cost: O(k * C_n^2) — linear in crossings, polynomial in strand count (vs O(2^k) state-sum).

## Predictions

1. TL_n matrices for n=2,3,4 reproduce ALL bracket values from demos 19-24 exactly.
2. At delta=0, generator matrices become very sparse — any diagram composition producing a closed loop gives zero. The effective dimension may be smaller than C_n.
3. For n=2 (C_2=2): NOT gate (5 crossings) requires multiplying five 2x2 matrices instead of summing 32 states. For n=3 (C_3=5): NAND gate uses 5x5 matrices.
4. Closure functionals (trace, plat) become simple dot products with specific basis-coefficient vectors.
5. The TL_n relations (e_i^2 = delta*e_i, e_i*e_{i+1}*e_i = e_i, e_i*e_j = e_j*e_i for |i-j|>=2) are verified computationally for all n up to 4.

## Approach

- **Part A:** TL_n basis enumeration — enumerate C_n basis elements as planar matchings of 2n points. For n=2: 2 matchings (identity, cup-cap e_1). n=3: 5. n=4: 14. Represent as pairing arrays.
- **Part B:** Generator matrices — for each e_i, compute C_n x C_n matrix by composing diagrams and counting closed loops. At delta=0, any composition producing a loop gives coefficient 0.
- **Part C:** Braid generator matrices — sigma_i -> A*Id + A^{-1}*e_i. At A=e^{i*pi/4}, specific complex C_n x C_n matrices.
- **Part D:** Verification against demos 19-24 — compute bracket via matrix multiplication for known braid words, verify exact match with state-sum values.
- **Part E:** Closure functionals — implement trace and plat closures as linear functionals on TL_n. At delta=0, these are simple dot products.
