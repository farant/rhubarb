---
id: P0008
title: "Pretraining Large Language Models with NVFP4"
authors: [NVIDIA Research Team]
year: 2025
arxiv_id: "2509.25149"
extraction_group: C
relevance: medium
key_techniques: [nvfp4_format, e2m1_quantization, random_hadamard_transform, two_level_microscaling, 2d_block_scaling, stochastic_rounding, mixed_precision_training]
summary: "First successful pretraining in 4-bit floating point (E2M1 = 16 representable values per weight). 12B hybrid Mamba-Transformer trained on 10T tokens matches FP8 baseline. Four essential pillars: mixed-precision layers, Hadamard outlier redistribution, 2D block scaling, stochastic rounding on gradients only."
related: []
---

## Key Contributions

First demonstration of pretraining a billion-parameter LLM entirely in 4-bit floating point. The NVFP4 format uses E2M1 (4 bits: 1 sign, 2 exponent, 1 mantissa) giving only 16 representable values: +/-{0, 0.5, 1, 1.5, 2, 3, 4, 6}. Two-level microscaling: blocks of 16 elements share an E4M3 scale factor (8-bit), plus a per-tensor FP32 scale. Yet training converges to match FP8 accuracy.

## Key Techniques

- **E2M1 format**: Only 16 distinct values. Yet neural networks can be trained successfully with this extreme quantization.
- **Random Hadamard Transforms**: Redistribute weight outliers into approximately Gaussian distribution before quantization. H is orthogonal (+/-1 entries), cancels in dot product: (AH)(H^T B) = AB. Only needed on Wgrad inputs.
- **2D block scaling**: Scale in 16x16 blocks for weights. Ensures consistency between forward and backward passes (weight transpose doesn't break scaling). 1D (1x16) for activations and gradients.
- **Stochastic rounding ONLY on gradients**: Deterministic round-to-nearest-even for weights and activations. Stochastic rounding adds noise that exceeds bias for forward-pass tensors but provides unbiased estimation for gradients.
- **Mixed precision layers**: Keep first 2 and last 8 blocks (of 62) in BF16 (~15% of linear layers). Last layers most sensitive.
- **Three-phase data blending**: Phase 1 (70%): diverse. Phase 2 (20%): higher quality. Phase 3 (10%): highest quality + synthetic.

## Specific Relevance to Eigenflower

- **Principles transfer to M2**: M2 lacks NVFP4 tensor cores, but the principles are universal: low-precision for most layers, keep first/last in high precision, stochastic rounding on gradients, Hadamard transforms for spreading outliers.
- **16 values suffice**: Striking signal about information requirements of NN training. For syllogism compilation, weights might only need a small discrete set.
- **Data blending strategy**: For eigenflower corpus: Phase 1 = full Bible+Summa, Phase 2 = high-quality theological text, Phase 3 = curated syllogism pairs + Q&A.
- **Switching precision late recovers gap**: 80%+ training in low precision, finish in high precision. Could do same on M2 with custom format.

## Key Results

- 12B hybrid Mamba-Transformer at FP4 matches FP8 baseline on 10T tokens
- NVFP4 is 36% more token-efficient than MXFP4
- Switching to BF16 at 82% training recovers loss to match FP8
- Each technique independently necessary for stability at 10T tokens
- Forward-pass quantization causes most of the gap (not backward)

## Key Equations

- NVFP4 encode: s_enc = (6 * 448) / amax_x (global scale)
- Block scale: s_dec,b = amax_b / 6 (local E4M3 scale per 16 elements)
- Hadamard: x' = q(x * H * s), HH^T = I

## Implementation Notes

- 1.2B Transformer tested: 20 blocks, dim 2048, FFN dim 6144, seq 8192, batch 768
- 12B hybrid: 62 blocks (6 Self-Attention, 28 FFN, 28 Mamba-2), dim 5120
- Optimizer: Adam beta1=0.9, beta2=0.95, weight decay 0.1
- Warmup-Stable-Decay LR: constant 80%, decay to 1/100th over 20%
- High-precision ops: embeddings, output head, normalization, nonlinearities, attention score-value GEMMs, softmax
- Blackwell Tensor Cores: 4-6x speedup vs BF16
- NVIDIA Transformer Engine for training code
