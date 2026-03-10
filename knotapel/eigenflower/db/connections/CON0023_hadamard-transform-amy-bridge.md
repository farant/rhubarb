---
id: CON0023
title: "NVFP4 Random Hadamard Transforms connect to DKC Amy Bridge (T-gate/Hadamard decomposition)"
connects:
  - "NVFP4 Random Hadamard Transforms for outlier redistribution before quantization"
  - "DKC Dual-Channel Theorem mapping to T-gate/Hadamard in quantum circuit synthesis (D108)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 4"
---

NVFP4 uses RHT to spread outlier weights into approximately Gaussian distribution before quantization, with the transform canceling in the dot product. The Dual-Channel Theorem (D108) maps onto T-gate/Hadamard decomposition in quantum circuit synthesis (Amy, Glaudell & Ross 2023).

DKC's product closure channel corresponds to T-gate (phase rotation) and the additive v_2 channel corresponds to Hadamard (magnitude mixing). The activation function provides the "Hadamard-equivalent" computational resource enabling delta=0 values to compute parity.

The structural connection: the Hadamard matrix H (orthogonal, information-preserving) parallels braid generators satisfying the Yang-Baxter equation. NVFP4's insight that RHT is needed only on gradients (not forward pass) parallels DKC's finding that the activation (readout) is the computational bottleneck, not the weight lattice: "the wall was in the activation, not the lattice" (D48->D50).
