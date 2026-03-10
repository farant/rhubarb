---
id: PT0029
title: "Random Hadamard Transforms for outlier redistribution"
status: untested
source: ["NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0028]
---

Apply a random Hadamard transform H (orthogonal, +/-1 entries) before quantization to redistribute weight outliers into an approximately Gaussian distribution. The transform cancels in the dot product: (AH)(H^T B) = AB since HH^T = I.

Key details: (a) only needed on Wgrad inputs, not Fprop/Dgrad; (b) d=16 matrix size works well; (c) a single random sign vector shared across all layers suffices; (d) implemented as tiled matrix multiplication, cost is small when d << tensor dimensions.

For eigenflower: the Hadamard transform is hardware-independent, implementable in C89 as a simple matrix multiplication with {-1, +1} entries. Makes weight distributions more Gaussian, which is ideal for lattice-based quantization (more uniform distribution within Babai's hyper-cuboid from GPTQ-as-CVP).

Combines well with GPTQ-as-CVP: RHT during training -> more Gaussian weights -> better quantization post-training.
