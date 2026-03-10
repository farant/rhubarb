---
id: PT0064
title: "Orchid bidirectional convolution for diffusion text generation"
status: untested
source: ["Orchid paper (Karami & Ghodsi, NeurIPS 2024)"]
applicable_to: []
related: [PT0002, PT0044]
---

Orchid's bidirectional global convolution is a natural fit for diffusion-based text generation, where the model denoises the entire sequence simultaneously. Unlike autoregressive models (which Orchid is not yet adapted for), diffusion processes the whole sequence at once -- exactly what Orchid's global convolution does.

The O(L log L) complexity means diffusion denoising steps are cheaper than with attention-based models. Each denoising step applies data-dependent convolution to the full sequence.

For eigenflower: if pursuing diffusion text generation (PT0044), Orchid provides the backbone architecture. The data-dependent kernel could adapt to the noise level at each denoising step, providing different processing for heavily noised vs nearly clean text.

Caveat: this combination is speculative -- neither paper demonstrates it. The interaction between Orchid's shift-equivariance and diffusion's noise-level conditioning needs investigation.
