---
id: PT0051
title: "kappa as reasoning-vs-creativity dial"
status: untested
source: ["Free Transformer paper (Fleuret, 2025)"]
applicable_to: []
related: [PT0007, PT0008]
---

The Free Transformer's kappa parameter (free bits threshold) controls how much information flows through the latent variable Z. This creates a built-in "reasoning vs creativity" dial:

- Lower kappa = less structure in Z = more freedom for decoder = more creative/varied text
- Higher kappa = more structure in Z = more deterministic/logical output
- Sweet spot: 1/2 to 1 bit per token for balanced improvement

At 4 bits (kappa = 4*log(2)), the encoder encodes the ENTIRE sequence in Z, making the decoder trivial and downstream tasks collapse.

For eigenflower: use different kappa values for different modes:
- Deductive reasoning: high kappa (deterministic, Z encodes the syllogistic form exactly)
- Analogical reasoning: medium kappa (structured but flexible)
- Creative generation: low kappa (more freedom)

This maps to Domingos's temperature-controlled reasoning but through a different mechanism -- information bottleneck vs sigmoid temperature.
