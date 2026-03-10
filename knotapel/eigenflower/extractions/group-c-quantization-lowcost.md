# Group C: Quantization, Low-Precision Training, and Micro-Budget Training

Date: 2026-02-26

---

## Paper 1: The Geometry of LLM Quantization: GPTQ as Babai's Nearest Plane Algorithm

**arXiv:** 2507.18553v2
**Authors:** Jiale Chen, Yalda Shabanzadeh, Elvir Crncevic, Torsten Hoefler, Dan Alistarh (ISTA, Red Hat, ETH Zurich)

### Core Technique

This paper provides the first geometric interpretation of GPTQ, the dominant post-training quantization method for LLMs. The key finding is that GPTQ, when executed back-to-front (last dimension to first), is mathematically identical to Babai's nearest plane algorithm for the classical Closest Vector Problem (CVP) on a lattice defined by the Hessian matrix of the layer's inputs. The equivalence holds without LLL basis reduction and independently of weight clipping. This means the entire field of lattice algorithms (decades of work on CVP, basis reduction, etc.) can now be imported directly into neural network quantization. The paper derives a tight, closed-form error bound for GPTQ in the no-clipping regime, proposes two new overflow-tolerant quantization methods (SSQR and HPTQ), and provides efficient CUDA inference kernels.

### Relevant Ideas

- **Quantization = Closest Vector Problem (Section 4.1).** Linear-layer quantization with L2 objective on the output is equivalent to CVP on a lattice whose basis is X * diag(s_i), where X is the calibration input and s_i are the quantization scales. The weight vector maps to a target vector, the integer quantized weights map to lattice coordinates. This is a deep structural insight: weight quantization is not ad-hoc rounding, it is a geometric search problem on a lattice.

- **Hessian matrix defines the lattice geometry (Section 3.1).** The lattice basis is determined by X^T * X (the Hessian). Any decomposition factor of the Hessian can be used as the basis without changing the geometric properties of the CVP. This means you can work with a square c x c factor instead of the full n x c matrix, saving computation.

- **GPTQ's error propagation = orthogonal projection onto nearest hyperplane (Section 4.2, Theorem 2).** Each step of GPTQ's greedy quantize-and-update procedure is actually projecting the residual target vector onto the nearest hyperplane in the lattice's activation space. This gives an intuitive geometric picture: GPTQ performs an "orthogonal walk" through nested affine subspaces.

- **Quantization order matters and is controllable (Section 4.5).** The error bound depends on the diagonal of the LDL decomposition of the permuted Hessian. The paper proposes "min-pivot" ordering (Algorithm 3): at each step, choose the dimension with the minimum diagonal entry in the Cholesky/LDL decomposition. This consistently minimizes the trace of D (the error proxy) compared to GPTQ's default "act-order." The ordering has cubic complexity and doesn't increase overall quantization time.

- **No-clipping error bound is tight (Section 4.4, Theorem 5).** In the no-clipping regime (integer grid = all of Z), the worst-case layer-wise error is bounded by 1/4 * trace(D) where D is the diagonal of the LDL of the permuted Hessian. The expected error under uniform distribution is exactly 1/3 of this worst case (Section C.2). This is the first provable guarantee for GPTQ.

- **Weight clipping breaks guarantees but is standard practice (Section 5).** Original GPTQ clips overflowed integers, which violates the Babai error bound. The paper proposes two alternatives: (a) SSQR: scale-adjusted SpQR that stores overflowed weights as sparse full-precision outliers, adjusting scale via binary search to hit a target outlier rate; (b) HPTQ: Huffman-encoded PTQ that quantizes to unconstrained integers and uses entropy coding (Huffman) to compress, achieving variable bitwidth per weight.

- **HPTQ: Huffman-coded quantization to unconstrained integers (Section 5).** Quantize W = s*Z where Z is in Z (all integers, no clipping). Select s via entropy-guided binary search so the Huffman-coded Z meets a target average bitwidth. This yields non-uniform effective bitwidths: 3.125 bits emerges as Pareto-optimal for perplexity vs compression across model sizes.

- **Composing Babai + GPTQ gives nothing extra (Section 4.3, B.4).** A seemingly appealing idea is to run Babai's algorithm then apply one more GPTQ-style error propagation step. The paper proves this extra step is algebraically redundant -- the rounding decisions don't change. Once you've done the orthogonal walk, you're done.

- **LLL basis reduction is impractical for batched quantization (Appendix A.2).** LLL generates different transformations for different scale vectors s_i, which prevents reuse across output channels. Basis reduction is also incompatible with clipping. The paper shows you can replace LLL with a simple permutation matrix (quantization order), which IS reusable.

- **Calibration data defines the lattice (Section 3.1).** Only a small calibration set (256 sequences of length 2048) is needed to compute the Hessian X^T * X that defines the lattice for quantization. The lattice structure is completely determined by the input distribution, not the weights themselves.

- **Group quantization with per-group scales (Section 3.1).** The framework generalizes to arbitrary grouping patterns: each weight element w_i[j] can have its own scaling factor s_i[j]. This subsumes per-channel, per-group, and per-tensor quantization as special cases.

- **CUDA kernel for sparse-quantized inference (Section 5, D.4).** The SSQR kernel handles dense low-bit inliers + sparse FP outliers. In low-batch regime (batch=1), uses SIMT cores; for batch >= 8 uses tensor cores. Achieves ~2x speedup vs PyTorch BF16 for decoding on A6000.

- **3.125-bit HPTQ as Pareto optimal (Figure 4b).** Across model sizes from 0.6B to 14B, HPTQ at 3.125 average bits per weight consistently achieves the best perplexity-vs-compression tradeoff. This is a striking finding about the "natural" information density of LLM weights.

### Relevance to Eigenflower Goals

**Goal 1 (Train in 30-60 min on M2).** The lattice/CVP framework applies directly to post-training quantization, which could be used to compress a trained eigenflower model for fast inference. More importantly, understanding quantization as a geometric problem could inform how to design weight structures that are inherently quantization-friendly from the start -- i.e., training directly in a lattice-aware manner.

**Goal 3 (Compile syllogisms to weights).** This is perhaps the most relevant connection. If you're compiling discrete logical structures (syllogisms) to continuous weights, the CVP framework gives you the exact mathematical tool: you have a lattice of "integer weight configurations" and you want to find the closest one to your target (the desired behavior). The Hessian of the input data defines which configurations are "close" in the output space. This could make syllogism-to-weight compilation a well-posed optimization problem rather than a heuristic.

**Goal 4 (Dynamic weight recompilation).** GPTQ-style quantization processes one weight at a time with error correction. This greedy, sequential structure could be adapted for on-the-fly weight updates when recording new syllogisms: quantize the new weight, propagate the error to the rest, maintaining overall model quality.

**Goal 5 (Performant inference on MacBook).** The 3.125-bit HPTQ representation with Huffman coding is directly relevant. A small eigenflower model (say 100M-500M parameters) quantized to 3 bits would be tiny in memory. The sparse outlier representation (SSQR with 1-5% outliers) is also attractive: store 95%+ of weights at 2-4 bits, keep a sparse set at full precision.

**Goal 8 (Decompile existing models).** The lattice/Hessian decomposition provides a structured view of trained weights. The Gram-Schmidt vectors of the lattice basis reveal the "independent directions of variation" in the weight space, weighted by their importance (the Hessian diagonal). This could be a tool for understanding what structure exists in trained weights.

### Implementation Notes

- All quantization methods are applied layer-by-layer (embarrassingly parallel across layers)
- Requires only 256 calibration samples to compute the Hessian
- GPTQ has O(c^3) complexity in the column/row dimension c, not in model size
- HPTQ achieves near-BF16 perplexity at 3.125 bits on models from 0.6B to 14B
- SSQR CUDA kernel code is provided; targets NVIDIA Ampere GPUs
- Min-pivot ordering adds O(c^3) cost (same as existing GPTQ)
- The Qwen3-0.6B model quantized with HPTQ at 4.125 bits: perplexity 22.72 (vs 20.96 baseline) -- even tiny models survive aggressive quantization

### Combinability

- **With Paper 2 (NVFP4):** Train in 4-bit precision, then post-training quantize even further with HPTQ to 2-3 bits for inference. The lattice perspective could also inform better 4-bit training: design the training so weight distributions are naturally lattice-friendly.
- **With Paper 3 (MicroDiT):** The patch masking reduces training cost; GPTQ reduces inference cost. Together they bookend the full pipeline. Also, if eigenflower uses diffusion for text generation, quantized diffusion inference would be essential for MacBook deployment.
- **With syllogism compilation:** The CVP framework is almost tailor-made for the "compile discrete structure to continuous weights" problem. The integer lattice IS the discrete structure; the continuous weight space IS the target; the Hessian IS the importance weighting.

---

## Paper 2: Pretraining Large Language Models with NVFP4

**arXiv:** 2509.25149v1
**Authors:** NVIDIA (large team, key contributors in numerics, SW, infrastructure)

### Core Technique

This paper presents the first successful demonstration of pretraining a billion-parameter language model entirely in 4-bit floating point (FP4) precision, using NVIDIA's NVFP4 format on Blackwell GPUs. They train a 12B-parameter hybrid Mamba-Transformer model on 10 trillion tokens, matching FP8 baseline accuracy. The training methodology has four essential pillars: (1) keep ~15% of layers in higher precision (first/last blocks), (2) apply Random Hadamard Transforms to redistribute outliers before quantization, (3) use 2D block scaling for weight consistency between forward and backward passes, (4) use stochastic rounding for gradients (not weights or activations). The NVFP4 format itself improves over MXFP4 with smaller blocks (16 vs 32), more precise E4M3 scale factors, and a two-level scaling strategy (per-block E4M3 + per-tensor FP32).

### Relevant Ideas

- **NVFP4 format: 4 bits with two-level microscaling (Section 2).** Each element is E2M1 (1 sign, 2 exponent, 1 mantissa bits), representable values: +/-{0, 0.5, 1, 1.5, 2, 3, 4, 6}. Blocks of 16 elements share an E4M3 scale factor (8-bit). A per-tensor FP32 scale provides global range. This two-level approach captures local dynamic range (block) and global range (tensor). The 6.25% of values that are block maxima are effectively stored at near-FP8 precision.

- **E2M1 is just 16 representable values (Section 2).** With 4 bits and the E2M1 format, you get only 16 distinct values (8 positive, 8 negative including zero). Yet training converges to match FP8. This is a powerful signal about the information requirements of neural network training.

- **Random Hadamard Transforms to spread outliers (Section 4.2).** Weight distributions have outliers that are hard to represent in FP4. Applying a random Hadamard transform H (orthogonal, +/-1 entries) before quantization redistributes outliers into an approximately Gaussian distribution. The transform cancels in the dot product: (AH)(H^T B) = AB since HH^T = I. Key details: (a) only needed on Wgrad inputs, not Fprop/Dgrad; (b) d=16 matrix size works well; (c) a single random sign vector shared across all layers suffices; (d) implemented as tiled matrix multiplication, cost is small when d << tensor dims.

- **2D block scaling for chain rule consistency (Section 4.3).** A critical subtlety: during backprop, the weight tensor gets transposed (row scaling becomes column scaling). If you scale along rows for forward pass, the backward pass sees different quantized values -- breaking the chain rule (w_fprop != w_bprop). Solution: scale in 2D blocks (16x16 for weights). Each block's scale is valid regardless of access direction. Activations and gradients use 1D (1x16) scaling since they're less sensitive.

- **Stochastic rounding ONLY on gradients (Section 4.4, E.3).** Critical finding: stochastic rounding helps gradients but HURTS weights and activations. Deterministic round-to-nearest-even is better for forward-pass tensors. The reason: gradients accumulate bias from systematic rounding errors, and stochastic rounding provides unbiased estimation. But for weights/activations, stochastic rounding adds noise that exceeds the bias it removes. This is a clean, counterintuitive result.

- **Mixed precision: keep ~15% of layers in BF16 (Section 4.1).** Training diverges if ALL layers are FP4. The final layers are most sensitive (they need more dynamic range and mantissa than FP4 provides). Keep the first 2 and last 8 blocks (out of 62) in BF16 -- that's 16% of linear layers. The first 4 blocks being high-precision is actually less important than the last blocks.

- **Which operations stay in high precision (Section 4.1).** Embeddings, output projection head, normalization layers, non-linearities, attention score-value GEMMs, softmax, and query-key attention all stay in BF16/FP32. Main weights, weight gradients, and optimizer states stay in FP32 (the master copy). Only the three GEMMs per linear layer (Fprop, Dgrad, Wgrad) are done in FP4.

- **Warmup-Stable-Decay learning rate schedule (Appendix A.1).** Constant LR for 80% of training, then decay to 1/100th over final 20%. This is simpler than cosine schedules and works well. For the 12B model: LR = 4.5e-4, decaying to 4.5e-6.

- **Switching to higher precision at end of training recovers most loss gap (Appendix D).** Switching from FP4 to BF16 at 82% through training (8.2T tokens out of 10T) recovers the loss to match FP8 baseline. Most of the gap comes from forward-pass quantization (not backward). This means you can do 80%+ of training in FP4, then finish in higher precision.

- **NVFP4 is 36% more token-efficient than MXFP4 (Section 5).** To match NVFP4's final loss, MXFP4 needs 1.36T tokens vs NVFP4's 1T tokens (on 8B model). The superior token efficiency comes from NVFP4's smaller blocks (16 vs 32) and more precise E4M3 scale factors vs MXFP4's power-of-two UE8M0 scales.

- **Ablation: each technique is necessary for stability at 10T tokens (Figure 4).** At shorter training (< 1T tokens), some techniques can be omitted. But at 10T tokens, removing ANY of {stochastic rounding, Hadamard transforms, 2D weight scaling, high-precision final layers} causes divergence. The longer you train, the more each technique matters.

- **Hybrid Mamba-Transformer architecture (Appendix A).** The 12B model has 62 blocks: 6 Self-Attention, 28 FFNs, 28 Mamba-2 (each block has 2 linear layers). Model dim 5120, FFN dim 20480, 40 Q heads, 8 KV heads. Squared ReLU activations. RMSNorm. No position embeddings, no dropout, no biases.

- **Three-phase data blending (Appendix A.1).** Phase 1 (70% of training): diverse web/wiki/code/math. Phase 2 (20%): higher-quality subset. Phase 3 (10%): highest quality + synthetic SFT. Progressive quality increase.

### Relevance to Eigenflower Goals

**Goal 1 (Train in 30-60 min on M2).** The M2 MacBook does NOT have NVFP4 tensor cores (that's Blackwell). However, the PRINCIPLES transfer directly: (a) you can use low-precision arithmetic (even lower than FP32) for most of training on Apple Silicon's AMX/Neural Engine; (b) the specific insight that only ~15% of layers need high precision is universal; (c) stochastic rounding for gradients is implementable in software at any precision; (d) the Hadamard transform trick for spreading outliers works at any precision boundary.

**Goal 2 (Small corpus: Bible + Summa).** The three-phase data blending is directly relevant. For eigenflower: Phase 1 could be the full Bible (multilingual) + Summa for general language patterns; Phase 2 could be high-quality theological text only; Phase 3 could be curated syllogism pairs + Q&A.

**Goal 3 (Compile syllogisms to weights).** The finding that neural networks can be trained with only 16 distinct weight values (E2M1) per block is remarkable. It suggests that the "resolution" needed for knowledge encoding is much lower than assumed. For syllogism compilation, this means you might only need to place weights into a small discrete set, not a full continuous range.

**Goal 5 (Performant inference on MacBook).** Apple's M2 supports FP16 natively on the Neural Engine and the GPU. If eigenflower trains in a custom 4-8 bit format, inference could be done in INT8 or INT4 using the Neural Engine's quantized inference support, giving significant speedup over FP32.

**Goal 7 (Build from scratch).** The NVFP4 quantization procedure is described in full algorithmic detail (Appendix B). The two-level scaling (global FP32 + local E4M3), the encode/decode formulas, and the Hadamard transform mechanics (Appendix C) are all specified precisely enough to implement from scratch in C89. The key formulas: s_enc = (6 * 448) / amax_x for global scale; s_dec,b = amax_b / 6 for local block scale; Hadamard: x' = q(x * H * s).

### Implementation Notes

- Model sizes tested: 1.2B, 8B, 12B -- the 1.2B is most relevant to eigenflower
- 1.2B Transformer: 20 blocks, dim 2048, FFN dim 6144, 16 Q heads, 8 KV heads
- Sequence length: 8192, batch size: 768 for 1.2B
- Adam optimizer: beta1=0.9, beta2=0.95, weight decay 0.1
- NVFP4 training code: available via NVIDIA Transformer Engine
- Blackwell Tensor Cores do FP4 at 4x (GB200) or 6x (GB300) speedup vs BF16
- Memory is approximately halved with FP4 operands vs FP8
- The techniques (RHT, 2D scaling, SR, mixed precision) are described independently of hardware

### Combinability

- **With Paper 1 (GPTQ-as-CVP):** Train in FP4 (or a software-emulated low-precision format), then post-training quantize to even lower bits with HPTQ. The Hadamard transforms from NVFP4 training would make the weight distribution more Gaussian, which is actually ideal for lattice-based quantization (more uniform distribution within Babai's hyper-cuboid).
- **With Paper 3 (MicroDiT):** If exploring diffusion text generation, could combine FP4-like training precision with patch masking for extreme training cost reduction. The MicroDiT masking reduces FLOPs by 4x; FP4 reduces per-FLOP cost by 2-3x; combined could give 8-12x total speedup.
- **Custom low-precision format for eigenflower:** Rather than NVFP4 (which needs Blackwell), design a custom 4-6 bit format optimized for Apple Silicon. Could use the microscaling principle (block + global scale) but with INT4 or custom fixed-point to leverage Apple's integer ALUs. The stochastic rounding and Hadamard transforms are hardware-independent.

---

## Paper 3: Stretching Each Dollar: Diffusion Training from Scratch on a Micro-Budget

**arXiv:** 2407.15811v1
**Authors:** Vikash Sehwag, Xianghao Kong, Jingtao Li, Michael Spranger, Lingjuan Lyu (Sony AI, UC Riverside)

### Core Technique

This paper trains a competitive text-to-image diffusion model from scratch for only $1,890 in GPU costs (2.6 days on a single 8xH100 machine), achieving 12.7 FID on COCO -- competitive with Stable Diffusion 1.5 (11.18 FID, 4800 A100-days). The core innovation is "deferred masking": instead of naively dropping input patches (which degrades quality at high masking ratios), they first process ALL patches through a lightweight "patch-mixer" (a small 4-block transformer, <10% of backbone parameters), THEN mask 75% of patches before feeding to the main diffusion transformer. This lets the unmasked patches retain global image context despite most patches being dropped. They also use mixture-of-experts layers, layer-wise scaling (wider layers deeper in network), and synthetic data augmentation. The 1.16B parameter sparse model (MicroDiT) is trained on only 37M images.

### Relevant Ideas

- **Deferred masking with a patch-mixer (Section 2.3).** The key insight: masking patches at the input layer means the backbone transformer NEVER sees 75% of the image. But the computational savings from masking are only needed at the expensive backbone, not at a lightweight preprocessor. Solution: add a small "patch-mixer" (4 transformer blocks, <10% of backbone cost) that processes ALL patches first, then mask. The unmasked patches now carry information about the whole image. At 75% masking, deferred masking achieves FID 5.03 vs naive masking's 16.5 (on CIFAR ablation).

- **Masking as a training cost reduction strategy (Section 2.2).** Training cost of transformers is proportional to M * N * S where M=samples, N=parameters, S=sequence length (patches). Masking reduces S by 4x (75% masking), giving 4x training speedup. This is independent of model size -- unlike model downscaling, which reduces N. The paper shows masking > downscaling for equivalent compute budgets up to 75% masking ratio.

- **Patch-mixer architecture (Figure 3, Section 2.3).** A small transformer (4 blocks of MHA+MLP) with residual connections. Input: all patch embeddings + positional embeddings. Output: contextualized embeddings. Then mask. Then feed unmasked patches to backbone. During inference: no masking, no patch-mixer. The patch-mixer is training-only overhead.

- **Mixture-of-Experts layers (Section 2.3, 4.3).** Replace every other feedforward layer with an 8-expert MoE layer using expert-choice routing (each expert picks its top tokens, no auxiliary load-balancing loss needed). MoE increases parameter count (hence model capacity) without proportionally increasing FLOPs. At 60K steps (small scale), MoE shows only slight improvement; at large scale (250K+ steps), MoE significantly helps (FID 13.7 -> 12.7).

- **Layer-wise scaling (Section 2.3, Table 1).** Instead of constant width across all transformer blocks, linearly increase width (hidden dimension) from early to later layers. Deeper layers learn more complex features and benefit from more parameters. This outperforms constant-width at equal compute (FID 15.9 vs 19.6 with 75% naive masking).

- **Two-phase training with resolution upscaling (Section 6).** Phase 1: 250K steps on 256x256 with 75% masking + 30K unmasked finetuning. Phase 2: 50K steps on 512x512 with 75% masking + 5K unmasked finetuning. Total: 3.45e20 FLOPs. The resolution upscaling reuses Phase 1 weights, amortizing early training cost.

- **Unmasked finetuning after masked pretraining (Section 2.3).** A short phase of unmasked training after masked pretraining recovers any remaining performance gap. At 75% masking, this is not strictly necessary (deferred masking alone matches unmasked), but it helps at very high masking ratios and catches artifacts.

- **Synthetic data is critical for quality at small dataset size (Section 6.1).** Only 37M total images: 22M real (CC12M, SA1B, TextCaps) + 15M synthetic (JourneyDB, DiffusionDB). Under standard metrics (FID, CLIP), real+synthetic appears equal to real-only. But human preference evaluation (GPT-4o judge) shows synthetic data dramatically improves perceived quality: 63% preference for combined model vs 21% for real-only on PartiPrompts.

- **CLIP-based text encoder over T5 (Section 3).** They use CLIP ViT-H/14 (DFN-5B) for text encoding instead of T5-xxl. CLIP is faster to compute and 6x less storage. T5 is better at text synthesis but too expensive for micro-budget. Text embeddings and image latents are precomputed offline as a one-time cost, amortized over training runs.

- **Lower-dimensional latent space is better for micro-budget (Figure 10).** 4-channel VAE outperforms 16-channel VAE for micro-budget training. Even though 16-channel has better reconstruction, the higher latent dimensionality requires more training budget to learn. This suggests that for eigenflower's budget, working in a lower-dimensional latent space is preferable.

- **EDM noise distribution and its tuning (Section 3, 4.2).** Noise sigma is sampled from log-normal: ln(sigma) ~ N(P_mean, P_std). Default: (-1.2, 1.2). After tuning: (-0.6, 1.2). Shifting P_mean rightward (more noise) improves caption-image alignment. The noise schedule significantly affects what the model learns to denoise.

- **SwiGLU activation outperforms GELU (Section 4.2).** Following LLM conventions, SwiGLU in the feedforward layers gives better image generation than GELU. This confirms the convergence of best practices between LLM and diffusion transformer architectures.

- **Higher weight decay and higher learning rate both help (Section 4.2).** With few training steps, aggressive regularization (higher weight decay) and aggressive learning (higher LR to maximum stable value) both independently improve quality. Also, higher AdamW beta2 (running average for second moment) helps, contrary to LLM convention of beta2 ~ 0.95.

- **Classifier-free guidance (Section 2.1).** During training, randomly drop 10% of captions to learn unconditional generation. At inference, combine conditional and unconditional predictions: F_hat = F_unconditional + w * (F_conditional - F_unconditional). The guidance weight w > 1 controls fidelity-diversity tradeoff. This is a standard technique but essential for any diffusion implementation.

- **Random patch masking > block masking (Section 4.2, Figure 5b).** Masking individual patches at random outperforms masking contiguous blocks. Random masking provides better coverage of image regions across training.

- **Latent diffusion with VAE compression (Section 2.1).** Images are first compressed to latent space via a pretrained VAE (4-channel, from Stable-Diffusion-XL). Compression factor n, so latent is R^(h/n x w/n x c). This means the diffusion model operates on a much smaller sequence than pixel space. For eigenflower text: an analogous "text VAE" could compress token sequences to a latent space for diffusion.

- **Cosine learning rate schedule with AdamW (Section 3, Appendix A).** Standard optimizer setup. Weight decay 0.1-0.2. The training hyperparameters are surprisingly similar to LLM training.

- **Precomputing embeddings as one-time cost (Section 3).** Both text (CLIP) and image (VAE) embeddings are computed once and stored. Training then operates purely on embeddings, never touching raw images or text. This amortization is critical for micro-budget: the embedding cost is paid once regardless of how many training experiments you run.

### Relevance to Eigenflower Goals

**Goal 1 (Train in 30-60 min on M2).** This is the most directly relevant paper for training cost. Key takeaways: (a) 75% patch/token masking with deferred processing can reduce training FLOPs by 4x; (b) precomputing embeddings offline eliminates encoder cost from training loop; (c) two-phase training (low-res then high-res, or equivalently short-context then long-context) amortizes early training; (d) the total cost of their pipeline is 3.45e20 FLOPs for a 1.16B model -- eigenflower's smaller model (say 100-300M params) on a smaller dataset would be proportionally cheaper.

**Goal 2 (Small corpus: Bible + Summa).** The paper shows that 37M images (small by diffusion standards) can produce competitive results. The analogous small text corpus for eigenflower (Bible ~780K verses, Summa ~3M words) is even smaller, but the masking and data efficiency techniques should help. The synthetic data finding is interesting: could augment the Bible/Summa corpus with synthetic theological text.

**Goal 5 (Performant inference on MacBook).** Diffusion inference involves many forward passes (typically 20-50 denoising steps). The MoE architecture is interesting: 1.16B total parameters but much lower FLOPs per forward pass since only 2/8 experts fire per token. Combined with quantization (Paper 1), a MoE diffusion model could be fast at inference.

**Goal 6 (Diffusion text generation).** This is the core relevance. The entire MicroDiT pipeline -- latent diffusion, deferred masking, MoE, classifier-free guidance -- could be adapted for text. Instead of image patches, use text token embeddings. Instead of a VAE for images, use a text autoencoder. The noise distribution, guidance mechanism, and training schedule would transfer. Key adaptation: text is 1D (sequence of tokens) not 2D (grid of patches), so "patch masking" becomes "token masking" which is exactly what BERT-style pretraining does. The deferred masking insight (process all tokens through a lightweight mixer first, then mask) would apply directly.

**Goal 7 (Build from scratch).** The paper describes the full pipeline from first principles. The DiT architecture is just a standard transformer with timestep/noise conditioning. The diffusion loss (Equation 3) is simple: L = E[||F_theta(x + noise) - x||^2]. The deferred masking is a minor architectural addition. The MoE routing is expert-choice (no auxiliary loss). All of this is implementable in C89 with custom BLAS.

### Implementation Notes

- 1.16B sparse model (MicroDiT), DiT-Xl/2 backbone with 8-expert MoE in alternating blocks
- Patch size: 2 (for 32x32 latent = 256 patches per 256x256 image)
- Patch-mixer: 4 transformer blocks (<10% of backbone parameters)
- Training: AdamW, cosine LR, weight decay 0.01, EMA with 0.995 smoothing (last 10K steps)
- Phase 1 (256x256): 250K masked + 30K unmasked steps
- Phase 2 (512x512): 50K masked + 5K unmasked steps
- Total wall-clock: 2.6 days on 8xH100 (= 6.6 days on 8xA100)
- Cost: $1,890 at $30/hr for 8xH100
- Dataset: 37M images total (22M real + 15M synthetic), all open-source
- Code: https://github.com/SonyResearch/micro_diffusion
- VAE: 4-channel from SDXL (pretrained, used as-is)
- Text encoder: CLIP ViT-H/14 (pretrained, used as-is)

### Combinability

- **With Paper 1 (GPTQ-as-CVP):** Train the diffusion model, then HPTQ-quantize to 3 bits for deployment. The MoE architecture has natural sparsity (only 2/8 experts active), complementing the SSQR sparse outlier representation.
- **With Paper 2 (NVFP4):** Train in low precision (4-8 bit) AND use masking for a multiplicative cost reduction. If masking gives 4x and low precision gives 2-3x, total could be 8-12x cheaper. For eigenflower on M2: software-emulated low precision + token masking could push a 100-300M model into the 30-60 minute training range.
- **Deferred masking for text transformers:** The patch-mixer concept transfers directly to text. Process all tokens through a lightweight mixer (2-4 transformer layers), then mask 50-75% for the main backbone. This could dramatically reduce eigenflower training cost on the Bible/Summa corpus.
- **Precomputed embeddings for eigenflower:** Precompute Bible verse embeddings (using any encoder) once. Store them. Training then never touches the raw text -- only operates on fixed embeddings. This amortizes encoding cost and simplifies the training loop.

---

## Cross-Paper Synthesis: Combined Strategies for Eigenflower

### The Full Pipeline

1. **Corpus preparation:** Precompute embeddings for Bible (multilingual) + Summa Theologica once. Store as fixed tensors. (Paper 3 principle)

2. **Architecture:** Small transformer (100-300M params) with optional MoE (double capacity, same FLOPs). Use layer-wise scaling (wider deeper layers). (Paper 3)

3. **Training precision:** Design a custom 4-6 bit format for Apple Silicon, inspired by NVFP4's block + global scaling. Keep first/last 15% of layers in FP16. Stochastic rounding on gradients only. (Paper 2)

4. **Training efficiency:** Deferred token masking at 50-75% ratio. Lightweight token-mixer processes all tokens first. Two-phase training (short context -> long context). (Paper 3)

5. **Syllogism compilation:** Frame as CVP on the Hessian lattice. Each syllogism defines a target behavior vector; find the nearest lattice point (quantized weight configuration) that produces it. Use GPTQ-style greedy error propagation for one-at-a-time compilation. (Paper 1)

6. **Dynamic recompilation:** When a new syllogism arrives, compute its effect on the Hessian (rank-1 update), re-quantize affected weights using Babai's nearest plane, propagate error to neighbors. (Paper 1)

7. **Inference:** HPTQ-quantize the final model to 3-4 bits. On M2, use the Neural Engine for quantized matrix multiplies. Sparse outliers stored separately. (Paper 1)

### Key Numbers for Feasibility Estimation

- MicroDiT (1.16B sparse, 37M images): 3.45e20 FLOPs, 2.6 days on 8xH100
- Eigenflower target: ~200M dense params, ~5M text samples (Bible + Summa + augmentation)
- Ratio: ~6x fewer params, ~7x fewer samples = ~42x fewer FLOPs (rough)
- Estimated: ~8e18 FLOPs for eigenflower
- M2 MacBook: ~3.6 TFLOPS (FP16 GPU), = 3.6e12 FLOPS
- Time: 8e18 / 3.6e12 = 2.2e6 seconds = ~25 days at 100% utilization
- With 75% masking: ~6 days
- With masking + low-precision 2x speedup: ~3 days
- With masking + low precision + smaller model (50M params): hours range
- **Conclusion:** 50-100M parameter model is the sweet spot for 30-60 minute training on M2, with masking + low precision

### Most Novel Combined Idea

**Lattice-aware syllogism compilation.** Paper 1 shows that weight quantization IS the Closest Vector Problem on a lattice defined by the input Hessian. Paper 2 shows that only 16 distinct values per weight are needed. Together: compile each syllogism as a CVP instance where the lattice has only 16 points per dimension (a very sparse lattice). The Hessian from the training data tells you which lattice configurations are "close" in behavior space. This turns syllogism-to-weight compilation from a vague aspiration into a well-defined mathematical optimization problem with known algorithms (Babai's nearest plane) and provable error bounds (Theorem 5).
