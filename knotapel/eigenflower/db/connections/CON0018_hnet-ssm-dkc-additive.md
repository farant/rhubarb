---
id: CON0018
title: "H-Net SSM compression bias maps to DKC's additive (not multiplicative) nature"
connects:
  - "H-Net finding that SSM encoders outperform Transformers for compression"
  - "DKC is fundamentally additive: S_k = S_{k-1} + w_k (D73)"
type: structural_parallel
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 6"
---

H-Net finds SSMs outperform Transformers for encoding because SSMs have a natural "compression bias" -- they compress information into fixed-size states.

D73 proves DKC is fundamentally additive, not multiplicative. The multiplicative braid product formulation CANNOT compute XOR6 (complement-all-bits sends M to M^{-1}, which shares the same eigenvector cell). Only the additive weight sum S_k = S_{k-1} + w_k computes parity.

This additive accumulation IS a state space model: the hidden state is the partial sum in Z[zeta_8], and the transition function is addition of the next weight selected by the input bit. The 82.8% universal determinism measures the quality of state compression -- the S^2 Voronoi cell captures 82.8% of state information while discarding magnitude.

The SSM's compression bias corresponds to DKC's magnitude discarding: direction is retained (13 cells) but magnitude is lost. The 17.2% non-determinism IS the compression loss. This is a precisely quantified version of the SSM compression bias.
