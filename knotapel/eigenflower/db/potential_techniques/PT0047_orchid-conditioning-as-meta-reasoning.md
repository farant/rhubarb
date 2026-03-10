---
id: PT0047
title: "Orchid conditioning network as meta-reasoning selector"
status: untested
source: ["Orchid paper (Karami & Ghodsi, NeurIPS 2024)"]
applicable_to: []
related: [PT0002, PT0007]
---

The small conditioning network in Orchid that generates the convolution kernel is essentially a meta-network that decides HOW to process the input. This is analogous to how syllogistic reasoning selects which logical rules to apply.

The conditioning network could be compiled from syllogisms: it examines the input, determines which syllogistic form applies, and generates the appropriate convolution kernel. The kernel IS the reasoning rule.

Static positional bias (h_0 = FFN(PosEmb(t))) combined with data-dependent kernel (h_theta(x)) gives both fixed structural priors and adaptive processing.

Cross-attention alternative: the kernel can be conditioned on a DIFFERENT sequence u: y(x, u) = NN_theta(u) * x. This replaces cross-attention with convolution. For eigenflower: u could be the syllogism graph output, x the text input. The syllogism layer's output conditions how the fluency layer processes text.

Speculative -- the mapping from "syllogistic rules" to "frequency-domain convolution kernels" is not mathematically established.
