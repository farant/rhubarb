---
id: PT0030
title: "Stochastic rounding on gradients only"
status: untested
source: ["NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0028]
---

Apply stochastic rounding to gradients during low-precision training, but use deterministic round-to-nearest-even for weights and activations. This is counterintuitive but validated:

- Gradients accumulate bias from systematic rounding errors; stochastic rounding provides unbiased estimation
- For weights/activations, stochastic rounding adds noise that exceeds the bias it removes

At 10T tokens, removing stochastic rounding on gradients causes training divergence. The effect is more critical the longer you train.

For eigenflower: implementable in software at any precision. A simple technique that costs almost nothing but prevents training instability during low-precision training.

Implementation: for each gradient element g, round to floor(g) with probability (ceil(g) - g), or to ceil(g) with probability (g - floor(g)). Unbiased estimator of the true gradient.
