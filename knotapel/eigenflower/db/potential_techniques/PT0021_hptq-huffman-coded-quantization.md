---
id: PT0021
title: "HPTQ Huffman-coded variable-bitwidth quantization"
status: untested
source: ["GPTQ-as-CVP paper (Chen et al., 2025)"]
applicable_to: []
related: [PT0018]
---

Quantize weights to unconstrained integers (no clipping) and use Huffman entropy coding to compress, achieving variable bitwidth per weight. Select the quantization scale s via entropy-guided binary search so the Huffman-coded integers meet a target average bitwidth.

3.125 bits emerges as Pareto-optimal for perplexity vs compression across model sizes (0.6B to 14B). This is a finding about the "natural" information density of LLM weights.

For eigenflower: a small model (100-500M params) quantized to 3 bits would be tiny in memory. On M2, the Neural Engine can process quantized matrix multiplies for significant speedup over FP32.

Even tiny models survive aggressive quantization: Qwen3-0.6B at 4.125 bits has perplexity 22.72 vs 20.96 baseline.

The HPTQ approach avoids the weight clipping that breaks Babai's error bounds in standard GPTQ, providing better theoretical guarantees.
