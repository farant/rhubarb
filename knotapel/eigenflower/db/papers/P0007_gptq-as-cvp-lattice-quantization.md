---
id: P0007
title: "The Geometry of LLM Quantization: GPTQ as Babai's Nearest Plane Algorithm"
authors: [Jiale Chen, Yalda Shabanzadeh, Elvir Crncevic, Torsten Hoefler, Dan Alistarh]
year: 2025
arxiv_id: "2507.18553"
extraction_group: C
relevance: high
key_techniques: [closest_vector_problem, babai_nearest_plane, hessian_lattice, gptq_equivalence, hptq_huffman, ssqr_sparse_outlier, min_pivot_ordering]
summary: "First geometric interpretation of GPTQ: it is exactly Babai's nearest plane algorithm for the Closest Vector Problem on a lattice defined by the input Hessian. Derives tight error bounds, proposes HPTQ (Huffman-coded quantization) achieving Pareto-optimal 3.125-bit quantization."
related: []
---

## Key Contributions

This paper proves that GPTQ (the dominant post-training quantization method) is mathematically identical to Babai's nearest plane algorithm for the classical Closest Vector Problem (CVP) on a lattice defined by the Hessian matrix of the layer's inputs. This means decades of lattice algorithm research can be imported directly into neural network quantization. The paper derives a tight closed-form error bound and proposes two new methods: SSQR (sparse outlier representation) and HPTQ (Huffman-coded unconstrained integer quantization).

## Key Techniques

- **Quantization = CVP**: Weight quantization is a geometric search problem on a lattice whose basis is X * diag(s_i), where X is calibration input and s_i are quantization scales. The weight vector maps to a target vector; integer quantized weights map to lattice coordinates.
- **Hessian defines lattice geometry**: The lattice basis is determined by X^T * X. Any decomposition factor can be used without changing CVP properties.
- **GPTQ = orthogonal walk through nested affine subspaces**: Each greedy quantize-and-update step projects the residual onto the nearest hyperplane.
- **No-clipping error bound**: Worst-case layer error bounded by 1/4 * trace(D) where D is diagonal of LDL of permuted Hessian. Expected error = 1/3 of worst case.
- **Min-pivot ordering**: Choose dimensions with minimum diagonal entry in Cholesky/LDL. Consistently minimizes error vs GPTQ's default act-order.
- **HPTQ**: Quantize to unconstrained integers (no clipping), use Huffman coding for compression. 3.125 bits is Pareto-optimal across 0.6B-14B models.
- **SSQR**: Store overflowed weights as sparse full-precision outliers. Binary search for target outlier rate.

## Specific Relevance to Eigenflower

- **Syllogism-to-weight compilation as CVP**: Compiling discrete logical structures to continuous weights IS a closest vector problem. The integer lattice IS the discrete structure; the continuous weight space IS the target; the Hessian IS the importance weighting. This turns compilation from a vague aspiration into a well-defined optimization problem with known algorithms and provable bounds.
- **Dynamic recompilation via error propagation**: GPTQ-style greedy processing -- quantize new weight, propagate error to rest -- could serve as on-the-fly weight update for new syllogisms.
- **3.125-bit inference**: HPTQ at 3.125 bits consistently best across model sizes. A 100-500M param eigenflower model at 3 bits would be tiny in memory.
- **Only 256 calibration samples needed**: Minimal data requirement to compute the Hessian defining the quantization lattice.

## Key Results

- 3.125-bit HPTQ is Pareto-optimal for perplexity vs compression (0.6B-14B)
- Qwen3-0.6B at HPTQ 4.125 bits: perplexity 22.72 vs 20.96 baseline -- even tiny models survive
- Composing Babai + GPTQ gives nothing extra (algebraically redundant)
- Min-pivot ordering consistently minimizes trace(D) vs default act-order
- SSQR CUDA kernel: ~2x speedup vs PyTorch BF16 for decoding on A6000

## Key Equations

- CVP: find q in Z^c minimizing ||X(w - diag(s)*q)||_2
- Error bound: E_WC <= 1/4 * trace(D), where D from LDL decomposition
- GPTQ step: project residual onto nearest hyperplane (orthogonal to b_i*)
- HPTQ: W = s*Z, Z in Z (unconstrained integers), s chosen via entropy-guided binary search

## Implementation Notes

- Layer-by-layer quantization (embarrassingly parallel across layers)
- O(c^3) in column/row dimension c, not in model size
- Only 256 calibration sequences of length 2048 needed
- SSQR CUDA kernel handles dense low-bit inliers + sparse FP outliers
- LLL basis reduction is impractical for batched quantization; simple permutation works
- Framework generalizes to per-channel, per-group, and per-tensor quantization
