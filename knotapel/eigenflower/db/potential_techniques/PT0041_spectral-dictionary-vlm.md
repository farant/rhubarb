---
id: PT0041
title: "Spectral dictionary replacing self-attention"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025), referencing SDict-VLM"]
applicable_to: []
related: [PT0002, PT0013]
---

Replace self-attention with spectral decomposition: x = sum alpha_i * phi_i where phi_i are learnable basis functions (Gabor, cosine, wavelet atoms) and alpha_i are sparse codes. Achieves 85% of BLIP-2 performance with 60% fewer parameters and 2.3x less peak memory. Eliminates quadratic attention complexity.

For eigenflower: an alternative to both standard attention and Orchid convolution. The sparse coding gives interpretability -- you can see which spectral components are active for each input. Fewer parameters means faster training on M2.

The learnable basis functions could be adapted to capture the spectral structure of theological Latin text -- specific frequency patterns corresponding to different argument types, grammatical structures, etc.

Referenced paper: SDict-VLM (arXiv:2506.18943) by same authors as Hilbert Space paper. Would need to evaluate the full paper for implementation details.
