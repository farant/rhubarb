# Axis Elimination (Schur Complement)

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P6CDG3JQRY02JWJ5X0ME
**Dependencies:** Demo 29 (TL_n matrices), Demo 32 (dimension reduction), Demo 34 (impedance tensor)
**Code path:** `knotapel/demo_35_axis_elimination/`

## Hypothesis

Kron's axis elimination (Schur complement Z_eff = Z_1 - Z_2 * Z_4^{-1} * Z_3) can eliminate 'internal' strands from wide braids while preserving the bracket value under a given closure. For a 6-strand braid, progressive elimination 6->4->2 dramatically reduces matrix dimension (C_6=132 -> C_4=14 -> C_2=2). For Boolean computation, the 2D effective description should suffice.

## Predictions

1. Axis elimination works and dramatically reduces computation for wide braids.
2. Progressive 6->4->2 elimination preserves bracket exactly.
3. Z_4 may be singular at delta=0 (loop-killing condition), requiring pseudoinverse or alternative elimination order.
4. Singularity at delta=0 would mean internal strands can't be cleanly separated — topology enforces holism.
5. For Boolean computation (final bit only), 2D effective description suffices regardless of original strand count.

## Approach

- **Part A:** Strand partitioning — for 4-strand braid, partition TL_4 basis into 'external' (affect trace closure) and 'internal' elements.
- **Part B:** Block decomposition of Z into [[Z_1, Z_2], [Z_3, Z_4]] external/internal blocks.
- **Part C:** Schur complement elimination Z_eff = Z_1 - Z_2 * Z_4^{-1} * Z_3. Extract bracket from Z_eff alone.
- **Part D:** Progressive elimination — 6-strand braid, eliminate 2 strands at a time (6->4->2). Verify bracket preserved.
- **Part E:** Information compression — compare full Z_w, after elimination to TL_4 effective, after elimination to TL_2 effective.
