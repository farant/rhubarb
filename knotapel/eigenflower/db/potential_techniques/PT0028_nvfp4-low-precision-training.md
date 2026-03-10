---
id: PT0028
title: "NVFP4-inspired low-precision training principles"
status: untested
source: ["NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0029, PT0030, PT0031]
---

Four essential principles for stable low-precision training, transferable to any precision boundary:

1. Keep ~15% of layers in higher precision (first 2 + last 8 blocks out of 62). Final layers are most sensitive -- need more dynamic range.
2. Random Hadamard Transforms redistribute outliers before quantization, converting non-Gaussian distributions to approximately Gaussian. The transform cancels in dot products: (AH)(H^T B) = AB.
3. 2D block scaling (16x16 for weights) ensures chain rule consistency between forward and backward passes.
4. Stochastic rounding ONLY on gradients (not weights or activations). Counterintuitive but validated: gradients accumulate bias from systematic rounding, stochastic rounding provides unbiased estimation.

M2 MacBook doesn't have NVFP4 tensor cores, but the PRINCIPLES transfer: use low-precision arithmetic on Apple Silicon's AMX/Neural Engine. Design custom 4-6 bit format optimized for Apple Silicon using microscaling principle (block + global scale) with INT4 or custom fixed-point.

E2M1 format has only 16 representable values per block, yet training converges. This is a powerful signal about the low information requirements of neural network training.

Switching to higher precision at end of training (last 20%) recovers most of the precision gap.
