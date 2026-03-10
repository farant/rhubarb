---
id: PT0001
title: "BitNet b1.58 ternary weight quantization for syllogistic logic"
status: promoted_to_kit
promoted_to: KIT0018
source: ["BitNet b1.58 paper (Ma et al. 2024)"]
applicable_to: []
related: [PT0002, PT0003, PT0004]
---

Constrain all weight parameters to ternary values {-1, 0, +1} using BitNet b1.58's absmean quantization: W_quantized = RoundClip(W / mean(|W|), -1, 1). Matrix multiplication reduces to integer addition only (no floating-point multiply needed). When a weight is +1, add the activation; when -1, subtract; when 0, skip.

For eigenflower, ternary weights map naturally to syllogistic structure: {-1, 0, +1} corresponds to {negate, ignore, affirm}, which maps to term relationships in syllogisms. A syllogism "All A are B" could be compiled as a sparse ternary connection pattern.

Training uses straight-through estimator for gradients through the quantization function. Activations are quantized to 8 bits per token with absmax per-token quantization.

Key numbers: at 3B params, matches FP16 LLaMA in perplexity while using 3.55x less memory and 2.71x faster. CPU-friendly since it uses integer operations. M2 MacBook has excellent integer/NEON performance for this.

Caveats: Only tested with large corpora (100B+ tokens). Unknown behavior with small data, though reduced capacity may act as natural regularizer preventing overfitting.
