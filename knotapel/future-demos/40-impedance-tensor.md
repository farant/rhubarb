# Impedance Tensor

**Track:** C — TL Matrix Compilation
**Status:** Proposed
**Smaragda ID:** 01KHP7P6BR84S8WYVS6NGQRB73
**Dependencies:** Demo 29 (TL_n matrices), Demo 32 (dimension reduction)
**Code path:** `knotapel/demo_34_impedance_tensor/`

## Hypothesis

The TL_n transfer matrix Z_w for a braid word w is analogous to Kron's impedance tensor Z. The bracket is a linear functional applied to Z_w. Closure changes correspond to Kron-style transformations C^T * Z * C. At delta=0, the natural bilinear form on TL_n degenerates (some vectors have zero norm), matching the dimension reduction. Eigenvalues of Z_w carry information about the Boolean function the braid computes.

## Predictions

1. Changing closure is equivalent to C^T * Z * C where C encodes the closure change.
2. Eigenvalues of Z_w at delta=0 have clean algebraic structure (integers, roots of unity).
3. The metric degeneracy (bilinear form signature) matches dimension reduction from Demo 32.
4. Bracket value is invariant under any decomposition strategy (power conservation analogue).
5. NOT/NAND gate eigenvalue spectra encode the Boolean function computed.

## Approach

- **Part A:** Define braid 'impedance' Z_w as C_n x C_n transfer matrix from generator matrix product.
- **Part B:** Verify composition law Z_{w1*w2} = Z_{w1} * Z_{w2}. Test whether closure changes act as C^T * Z * C.
- **Part C:** Eigenvalue analysis — compute eigenvalues of Z_w for NOT, NAND, and other circuits at delta=0.
- **Part D:** Metric degeneracy — define bilinear form on TL_n (Markov trace of product), compute signature at delta=0 for n=2,3,4,5.
- **Part E:** Power invariance — verify bracket is unchanged under different decomposition strategies.

## Notes from Demo 25

Measurement matrices ARE closure-projected impedance tensors. SVD = eigendecomposition under closure change. This connection was established in Demo 25's closure-as-measurement-basis work.
