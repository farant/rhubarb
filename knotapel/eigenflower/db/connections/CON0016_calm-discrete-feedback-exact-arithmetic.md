---
id: CON0016
title: "CALM discrete feedback loop mirrors DKC's exact arithmetic requirement"
connects:
  - "CALM: continuous vectors must decode to discrete tokens for correctness"
  - "DKC: floating-point must ground to Z[zeta_8] exact integers for correctness (D29, D48, D64)"
type: structural_parallel
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 8"
---

CALM discovers that at inference, predicted continuous vectors must be decoded to K discrete tokens then re-embedded as input. Using continuous vectors directly as input DEGRADES performance because the latent space is too compact/brittle for the Transformer to unpack.

DKC's progression from approximate to exact arithmetic is the same discovery: D10-D28 (floating-point) discovered phenomena, D29+ (Z[zeta_8] exact) proved them. D48's 100M-quartet exhaustive search REQUIRES exact equality testing -- floating-point comparison would produce false positives at sector boundaries. D64's exact arithmetic eliminated 420 spurious failures from floating-point association-order errors.

The lesson is identical: working in the "continuous" representation (floating-point for DKC, continuous vectors for CALM) is useful for discovery but fails for rigorous computation. You must "ground" back to the discrete/exact representation to maintain correctness. DKC literally implements CALM's discrete feedback loop: compute in Z[zeta_8], decode to Boolean truth table, use truth table to verify.
