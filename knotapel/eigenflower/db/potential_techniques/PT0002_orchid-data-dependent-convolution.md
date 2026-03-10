---
id: PT0002
title: "Orchid data-dependent convolution replacing attention"
status: untested
source: ["Orchid paper (Karami & Ghodsi, NeurIPS 2024)"]
applicable_to: []
related: [PT0001, PT0003]
---

Replace self-attention with a data-dependent global convolution layer. The convolution kernel is generated dynamically by a small conditioning neural network that processes the input sequence. This gives attention-like expressiveness (data-dependent processing) with convolution efficiency (O(L log L) via FFT instead of O(L^2)).

Two conditioning approaches ensure shift-invariance: (1) Phase Suppression -- take magnitude of frequency components to eliminate phase, (2) Cross-Correlation -- compute cross-correlation between two shift-equivariant mappings. Both use small depthwise Conv1d layers (kernel 3-5) in spatial and frequency domains.

The full Orchid block is a chain of order 1.5: two element-wise multiplications sandwiching one data-dependent convolution. Achieves 30% fewer parameters than BERT with better GLUE performance. DCT outperforms DFT for the conditioning network.

For eigenflower: FFT-based convolution is highly optimized on M2 via Apple Accelerate vDSP. The conditioning network could potentially be bypassed to inject fixed kernels representing logical rules. Not yet adapted for autoregressive generation (limitation for GPT-style output, but fine for BERT-style understanding or diffusion).

Implementation: Core is FFT + small Conv1d + element-wise multiply. Code available at github.com/Karami-m/orchid.
