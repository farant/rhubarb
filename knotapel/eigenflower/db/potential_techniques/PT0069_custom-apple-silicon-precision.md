---
id: PT0069
title: "Custom 4-6 bit precision format for Apple Silicon"
status: untested
source: ["NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0028, PT0001]
---

Design a custom low-precision arithmetic format optimized for Apple Silicon rather than using NVFP4 (which requires Blackwell tensor cores). Use the microscaling principle (block + global scale) but with INT4 or custom fixed-point to leverage Apple's integer ALUs and AMX/Neural Engine.

Apple M2 supports:
- FP16 natively on Neural Engine and GPU
- INT8 quantized inference on Neural Engine
- Excellent integer/NEON performance for ternary (BitNet) operations
- Accelerate framework for optimized BLAS

Potential format: 4-bit integer with block scaling (16 elements share an 8-bit scale factor) + per-tensor FP32 global scale. This gives effective 6-bit precision with 4-bit storage, implementable in software.

The NVFP4 insight that stochastic rounding, Hadamard transforms, and 2D block scaling are hardware-independent means these stabilization techniques transfer directly to a custom Apple format.

Switching to FP16 for final 20% of training recovers most of the precision gap.
