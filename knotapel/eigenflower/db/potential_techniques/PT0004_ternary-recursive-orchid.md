---
id: PT0004
title: "Ternary Recursive Orchid combined architecture"
status: untested
source: ["BitNet b1.58 paper", "Orchid paper", "TRM paper"]
applicable_to: []
related: [PT0001, PT0002, PT0003]
---

Combine all three Group A techniques into one model: a 2-layer network with Orchid convolution (instead of attention) + SwiGLU MLP, all weights ternary {-1, 0, +1} via BitNet absmean quantization, with TRM recursive refinement (n=6, T=3, N_sup=16).

Proposed specs: hidden dimension 256-512, ~2-5M parameters. At 1.58 bits per param: 0.4-1.0 MB total model size. Forward pass: 42 applications of a 2-layer Orchid block using FFT-based convolution with ternary kernels. Inference: pure integer addition + FFT (no floating-point multiply).

This is the "Eigenflower Stack" proposal from the Group A synthesis. Novel combination -- no one has tried this specific stack. The interaction between ternary quantization and FFT-based convolution is an open question (kernel in frequency domain might need more precision than {-1, 0, 1}).

Risk: unvalidated combination. Each component works individually but interactions are unpredictable. Start with one, add others incrementally.
