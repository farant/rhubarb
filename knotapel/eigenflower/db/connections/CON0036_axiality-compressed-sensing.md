---
id: CON0036
title: "DKC axiality theorem IS compressed sensing on an algebraic ring"
connects:
  - "Hilbert space paper: compressed sensing for sparse signal recovery"
  - "DKC axiality (1-sparse in cyclotomic basis) + super-Nyquist compression (D71, D107)"
type: mathematical_identity
strength: strong
source: "extractions/group-d-knotapel-relevance.md, Connection 5"
---

Two independent compressed sensing results in DKC:

1. D71 (Super-Nyquist): 14 Voronoi cells suffice where Nyquist predicts 49. The 3.5x compression works because XOR6 is a binary function at 13 known positions -- sparsity plus known support.

2. D107 (Axis-alignment): Every braid matrix entry at delta=0 is axis-aligned (exactly one nonzero cyclotomic component: n*zeta_8^k). The entire 186-value catalog is 1-SPARSE in the {1, zeta_8, zeta_8^2, zeta_8^3} basis.

The axiality theorem reduces DKC to INTEGER PATH COUNTING + THREE-VARIABLE PHASE FORMULA. Each Z[zeta_8] value in a 4-dimensional ring is compressed to a 1D integer plus a discrete Z/4Z phase label. This is the most extreme possible compressed sensing.

Paper 2's L^2 framework would express this as: the "signal" (braid representation matrix) is maximally sparse in the cyclotomic basis. 2-adic neutrality (D107: geometric decay ~0.5x per valuation level) constrains integer magnitudes. Z/4Z phase formula constrains phases. Together: both sparse AND structured.

D104's sign-hash = 1-bit compressed sensing (Boufounos-Baraniuk 2008) is already explicitly using CS theory. The axiality theorem could enable extremely efficient inference: kernel matrix K becomes block-diagonal with at most 4 blocks, reducing O(n^3) to O(n) per block.
