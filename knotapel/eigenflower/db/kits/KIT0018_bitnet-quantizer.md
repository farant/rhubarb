---
id: KIT0018
title: "BitNet b1.58 ternary weight quantizer"
status: planned
tier: 1
input: "Weight matrix W (floating point)"
output: "Quantized weights Q in {-1, 0, +1}"
depends_on: []
effort: small
source: ["BitNet b1.58 paper (Ma et al. 2024)"]
migrated_from: PT0001
applicable_to: [G0001, G0002]
related: [KIT0003, KIT0006]
---

## Description

Constrain all weight parameters to ternary values {-1, 0, +1} using BitNet b1.58's absmean quantization:

W_quantized = RoundClip(W / mean(|W|), -1, 1)

Matrix multiplication reduces to integer addition only (no floating-point multiply needed). When a weight is +1, add the activation; when -1, subtract; when 0, skip.

For eigenflower, ternary weights map naturally to syllogistic structure: {-1, 0, +1} = {negate, ignore, affirm}. A syllogism "All A are B" compiles as a sparse ternary connection pattern.

Training uses straight-through estimator for gradients through the quantization function. Activations quantized to 8 bits per token with absmax per-token quantization.

Key numbers: at 3B params, matches FP16 LLaMA in perplexity using 3.55x less memory and 2.71x faster. CPU-friendly since it uses integer operations. M2 has excellent integer/NEON performance.

**Caveat**: Only tested with large corpora (100B+ tokens). Unknown behavior with small data, though reduced capacity may act as natural regularizer.

## API Sketch

```c
vacuum bitnet_quantizare(constans duplex *W, character *Q, longus rows, longus cols);
vacuum bitnet_multiplicare(constans character *Q, constans duplex *x, duplex *y, longus rows, longus cols);
```

~100 LOC for quantizer + ternary matmul.
