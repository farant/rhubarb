---
id: PT0066
title: "Free Transformer + CALM: latent conditioning with continuous prediction"
status: untested
source: ["Free Transformer paper (Fleuret, 2025)", "CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0006, PT0007]
---

Combine continuous vector prediction (CALM) with latent conditioning (Free Transformer). The latent Z decides WHAT to generate (logical mode, syllogistic form, topic). The energy head generates the continuous vector representing HOW to express it. The decoder produces tokens from the vector.

This separates content decision (Z) from expression decision (energy head) from token realization (decoder). Each component can be understood and modified independently.

For eigenflower: Z encodes "which syllogism to apply." The energy head generates the continuous representation of the conclusion. The decoder produces the Latin text. Compiling a new syllogism means assigning a Z value and training the energy head to produce the right output.

The three axes of compression (from Group B synthesis) combine: spatial (H-Net chunking), representational (CALM vectors), structural (Free Transformer Z). Together: ~24x fewer autoregressive steps than character-level baseline.

Risk: novel three-way combination. Test each independently first.
