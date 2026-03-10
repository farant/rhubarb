---
id: CON0009
title: "Orchid cross-attention conditioning parallels DKC encoding-dependent polarity inversion"
connects:
  - "Orchid cross-attention variant: kernel conditioned on sequence u while convolving x"
  - "DKC Dual-Channel Theorem with encoding-dependent polarity inversion (D108-D109)"
type: structural_parallel
strength: speculative
source: "extractions/group-a-knotapel-relevance.md, Connection 9"
---

Orchid's cross-attention variant conditions the kernel on sequence u while convolving sequence x -- the same convolution produces different results depending on which sequence provides conditioning. DKC's polarity inversion (D109) shows the same algebraic catalog produces different computational capabilities depending on encoding: additive encoding gives one set of parity solutions, multiplicative encoding inverts the polarity.

The encoding IS the conditioning. The resolution dependence (D108: same Z[zeta_8] value can be parity-capable at k=6 and poison at k=15) further reinforces this: the activation function is a third conditioning variable. Orchid's conditioning network does the combined work of DKC's encoding choice + activation selection.

The dual-channel requirement (both phase coherence AND magnitude diversity) could constrain Orchid kernel design: ensure the conditioning network outputs have both rotational diversity and scale diversity.
