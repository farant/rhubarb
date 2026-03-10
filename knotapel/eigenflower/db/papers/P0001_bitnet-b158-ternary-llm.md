---
id: P0001
title: "The Era of 1-bit LLMs: All Large Language Models are in 1.58 Bits"
authors: [Shuming Ma, Hongyu Wang, Lingxiao Ma, Lei Wang, Wenhui Wang, Shaohan Huang, Li Dong, Ruiping Wang, Jilong Xue, Furu Wei]
year: 2024
arxiv_id: "2402.17764"
extraction_group: A
relevance: high
key_techniques: [ternary_quantization, bitlinear_layers, integer_only_matmul, absmean_quantization, straight_through_estimator]
summary: "Every weight constrained to {-1, 0, +1} via absmean quantization trained from scratch. Matrix multiply becomes pure integer addition. Matches FP16 LLaMA at 3B params with 3.55x less memory and 71.4x less energy for matmul."
related: []
---

## Key Contributions

BitNet b1.58 is a Transformer variant where every weight parameter is ternary {-1, 0, +1}. Standard `nn.Linear` layers are replaced with `BitLinear` layers. The quantization function scales the weight matrix by its average absolute value (absmean), then rounds: `W_quantized = RoundClip(W / gamma, -1, 1)` where `gamma = mean(|W_ij|)`. Activations are quantized to 8 bits per token. The model is trained from scratch, NOT post-training quantized.

## Key Techniques

- **Ternary weight quantization via absmean**: W_hat = RoundClip(W / (mean(|W|) + epsilon), -1, 1). Extremely simple.
- **Matrix multiply as integer addition**: With {-1, 0, 1} weights, y = W*x becomes pure add/subtract. No floating-point multiply needed.
- **Feature filtering through zero weights**: The 0 value enables explicit feature filtering -- the network can completely ignore certain inputs. Significant improvement over pure binary {-1, +1}.
- **8-bit activations**: Absmax per-token quantization to [-Q_b, Q_b]. Simple symmetric, no zero-point.
- **Straight-through estimator**: For backprop through the non-differentiable quantization step.

## Specific Relevance to Eigenflower

- **Syllogism compilation**: Ternary weights {-1, 0, +1} map naturally to {negate, ignore, affirm} -- directly corresponding to term relationships in syllogisms. The quantization function is trivially invertible.
- **Dynamic weight recompilation**: Each weight is ~1.58 bits, trivial to store, modify, and reload. A syllogism can be a sparse ternary patch applied by modifying a few values.
- **Performant inference on MacBook**: No floating-point multiply at all -- just integer add. M2's NEON integer units handle this blazingly fast. Memory footprint shrinks 3-7x.
- **CPU-friendly**: Paper explicitly calls out that 1.58-bit LLMs are more friendly to CPU devices since they use integer operations.

## Key Results

| Scale | Memory Savings | Latency Savings | Energy Savings (MatMul) |
|-------|---------------|-----------------|------------------------|
| 1.3B  | 2.93x         | 1.51x           | 8.6x                   |
| 3B    | 3.55x         | 2.71x           | --                     |
| 70B   | 7.16x         | 4.10x           | 41.2x                  |

- 13B BitNet b1.58 is more efficient than 3B FP16
- 70B BitNet b1.58 supports 8.9x throughput vs FP16 LLaMA 70B
- INT8 addition consumes 71.4x less energy than FP16 multiply-accumulate (7nm)

## Implementation Notes

- Architecture: Standard Transformer (LLaMA-like) with BitLinear replacing nn.Linear
- Components: RMSNorm, SwiGLU, Rotary Embeddings, no biases
- Training: Straight-through estimator for gradients, trained on RedPajama 100B tokens
- Key sizes tested: 700M, 1.3B, 3B, 3.9B, 7B, 13B, 70B
- KV cache halved (8-bit activations vs 16-bit)
