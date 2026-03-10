---
id: PT0050
title: "CALM discrete feedback loop for grounded generation"
status: untested
source: ["CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0006]
---

At inference, the predicted continuous vector z is immediately decoded to K discrete tokens, which are then re-embedded as input for the next step. This "grounds" the autoregressive process in discrete token space.

Critical finding: using continuous vectors directly as input DEGRADES performance because the latent space is too compact/brittle for the Transformer to unpack. The discrete feedback loop forces stable representations.

Input compression MLP: the K decoded tokens are embedded and compressed by a 2-layer MLP into a single representation. Maintains efficiency gain (1 position per K tokens) while using stable discrete representations.

For eigenflower: mirrors the DKC insight that exact/discrete arithmetic is necessary for correctness even when continuous representations are useful for exploration. Both CALM and DKC discover you must "ground" back to discrete for rigorous computation.

The fluency-syllogism interface could use this pattern: continuous representations for processing, discrete grounding at the boundary between layers.
