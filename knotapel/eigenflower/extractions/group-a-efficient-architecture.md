# Group A: Efficient Architecture — Paper Extractions

Date: 2026-02-26

---

## Paper 1: BitNet b1.58 — "The Era of 1-bit LLMs: All Large Language Models are in 1.58 Bits"

**Authors:** Shuming Ma, Hongyu Wang, Lingxiao Ma, Lei Wang, Wenhui Wang, Shaohan Huang, Li Dong, Ruiping Wang, Jilong Xue, Furu Wei (Microsoft Research / Chinese Academy of Sciences)

**ArXiv:** 2402.17764v1 (Feb 2024)

### Core Technique

BitNet b1.58 is a Transformer variant where every weight parameter is constrained to ternary values {-1, 0, +1}. This is accomplished by replacing standard `nn.Linear` layers with `BitLinear` layers. The key quantization function scales the weight matrix by its average absolute value (absmean), then rounds to the nearest integer among {-1, 0, +1}. Activations are quantized to 8 bits per token. The architecture otherwise follows LLaMA (RMSNorm, SwiGLU, rotary embeddings, no biases). The model is trained from scratch — this is NOT post-training quantization.

The critical insight is that matrix multiplication with ternary weights reduces to integer addition only (no multiplication needed). When a weight is +1, add the activation; when -1, subtract; when 0, skip. This eliminates the most expensive operation in neural networks (floating-point multiply-accumulate) and replaces it with simple integer addition.

### Relevant Ideas

- **Ternary weight quantization via absmean** (Section 2, Eq. 1-3): W_quantized = RoundClip(W / gamma, -1, 1) where gamma = mean(|W_ij|). Extremely simple — just divide by average absolute value and round. This is the entire quantization function.

- **Matrix multiply becomes integer addition** (Section 1, Figure 1): With {-1, 0, 1} weights, y = W*x becomes pure addition/subtraction. No floating point multiply needed at all. The computation is x0 - x1 - x2 + x3 instead of 0.2961*x0 - 0.0495*x1 - 0.0924*x2 - 0.4765*x3.

- **Feature filtering through zero weights** (Section 1, p2): The inclusion of 0 (vs original BitNet's {-1, +1}) enables explicit feature filtering — the network can learn to completely ignore certain inputs. This significantly improves modeling capability over pure binary {-1, +1}.

- **8-bit activations with absmax per-token quantization** (Section 2, p2-3): Activations scaled to [-Q_b, Q_b] per token. Simple symmetric quantization. No zero-point needed, which simplifies implementation.

- **Matches FP16 performance at 3B parameters** (Section 3, Table 1): At 3B model size, BitNet b1.58 matches full-precision LLaMA in perplexity while using 3.55x less memory and being 2.71x faster. At 3.9B it actually outperforms LLaMA 3B.

- **Memory reduction scales with model size** (Section 3, Figure 2): Memory savings grow from 2.93x at 1.3B to 7.16x at 70B. Latency savings grow from 1.51x to 4.10x. The bigger the model, the bigger the win.

- **Energy: 71.4x less for matrix multiply** (Section 3, Figure 3): On 7nm chips, INT8 addition (BitNet's operation) consumes 71.4x less energy than FP16 multiply-accumulate (standard operation). End-to-end energy savings grow with model size (8.6x at 1.3B to 41.2x at 70B).

- **New scaling law** (Section 3, p5): 13B BitNet b1.58 is more efficient than 3B FP16. 30B BitNet b1.58 beats 7B FP16. 70B beats 13B FP16. Roughly a 4-5x parameter efficiency multiplier.

- **Throughput: 8.9x at 70B** (Section 3, Table 3): 70B BitNet b1.58 supports 11x the batch size and 8.9x throughput vs FP16 LLaMA 70B. This is on two 80GB A100s.

- **CPU-friendly** (Section 4, p6): 1.58-bit LLMs are more friendly to CPU devices since they use integer operations. This is explicitly called out as enabling edge/mobile deployment. M2 MacBook has excellent integer/NEON performance.

- **KV cache halved** (Section 4, p6): Since activations are 8-bit instead of 16-bit, the KV cache (dominant memory cost for long sequences) is halved. Context length can be doubled for the same memory.

- **1-bit MoE** (Section 4, p5): Authors propose combining 1.58-bit with Mixture-of-Experts as future work. The memory reduction solves MoE's main deployment bottleneck.

- **Native long sequence support** (Section 4, p6): 8-bit activations → KV cache compressed → longer context for same memory. Could compress further to 4-bit for 1.58-bit models.

- **Training recipe** (Section 3, p3): Trained on RedPajama for 100B tokens. Used FasterTransformer + Ladder 2-bit kernel for inference benchmarks.

- **Trained with 2T tokens** (Section 3, Table 4): When trained with 2 trillion tokens following StableLM recipe, BitNet b1.58 3B outperforms StableLM-3B on all benchmarks. More tokens help ternary models generalize.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | **HIGH** | Ternary weights mean the model fits in dramatically less memory. Forward/backward passes use integer ops that M2 handles efficiently. A small ternary model could train very fast. |
| 2. Small corpus (Bible + Summa) | **MEDIUM** | The paper only tests with large corpora (100B+ tokens). Unknown how ternary models behave with small data. But the reduced capacity might actually help avoid overfitting on small corpora. |
| 3. Compile syllogisms to weights | **CRITICAL** | Ternary weights {-1, 0, +1} are essentially logical: affirm, deny, ignore. This maps naturally to syllogistic structure. A syllogism "All A are B, all B are C, therefore all A are C" could potentially be compiled directly to ternary connection patterns. The quantization function (absmean + round) is trivially invertible. |
| 4. Dynamic weight recompilation | **CRITICAL** | Ternary weights are trivial to store, modify, and reload. Each weight is ~1.58 bits. You could store a syllogism as a sparse ternary patch and apply it by modifying a few {-1, 0, +1} values. Hot-swapping is cheap since there's no floating point precision to worry about. |
| 5. Performant inference on MacBook | **CRITICAL** | This is the paper's main selling point for eigenflower. No floating-point multiply at all — just integer add. M2's NEON integer units can process this blazingly fast. Memory footprint shrinks 3-7x. |
| 6. Diffusion text generation | **LOW** | Paper doesn't address diffusion. But ternary weights could work with diffusion denoising since the forward pass is the same. |
| 7. Build from scratch | **HIGH** | BitNet b1.58 IS trained from scratch, not quantized post-hoc. The paper explicitly states this is important — post-training quantization is suboptimal. The training recipe (absmean quantization + straight-through estimator) is simple to implement. |
| 8. Decompile existing models | **MEDIUM** | The absmean quantization function could be applied to extract ternary structure from existing model weights. This would be a form of "decompilation" — revealing the logical skeleton of learned representations. |

### Implementation Notes

- Architecture: Standard Transformer (LLaMA-like) with BitLinear replacing nn.Linear
- Components: RMSNorm, SwiGLU, Rotary Embeddings, no biases
- Quantization: W_hat = RoundClip(W / (mean(|W|) + epsilon), -1, 1)
- Activations: 8-bit absmax per-token quantization to [-Q_b, Q_b]
- Optimizer: Standard (paper doesn't specify but likely AdamW given LLaMA lineage)
- Training uses straight-through estimator for gradients through quantization
- Code: Integrable with llama.cpp, Huggingface, vLLM
- Inference kernel: Ladder 2-bit kernel for GPU, but CPU-native integer ops work too
- Key sizes tested: 700M, 1.3B, 3B, 3.9B, 7B, 13B, 70B

### Combinability

- **BitNet + TRM recursion**: A tiny ternary recursive model could be extraordinarily efficient. 7M params at 1.58 bits = ~1.4 MB of weights. Recursive application multiplies effective depth without parameter cost.
- **BitNet + Orchid**: Replace attention with data-dependent convolution AND use ternary weights. Double efficiency win — O(L log L) complexity with integer-only operations.
- **BitNet + syllogism compilation**: Ternary weights are the natural substrate for compiled logical rules. {-1, 0, +1} maps to {negate, ignore, affirm} which maps directly to term relationships in syllogisms.
- **BitNet + small corpus training**: Reduced parameter expressiveness of ternary weights may act as a natural regularizer, preventing overfitting on small data like Bible+Summa.

---

## Paper 2: Orchid — "Orchid: Flexible and Data-Dependent Convolution for Sequence Modeling"

**Authors:** Mahdi Karami (Google Research), Ali Ghodsi (University of Waterloo)

**ArXiv:** 2402.18508v3 (published at NeurIPS 2024)

**Code:** https://github.com/Karami-m/orchid

### Core Technique

Orchid replaces the self-attention mechanism in Transformers with a data-dependent global convolution layer. The key innovation is that the convolution kernel is not fixed — it is generated dynamically by a small conditioning neural network that processes the input sequence. This makes the convolution "data-dependent" (like attention, which produces different attention patterns for different inputs) while maintaining the computational efficiency of convolution (O(L log L) via FFT instead of O(L^2) for attention).

The conditioning network is designed to be shift-invariant, meaning shifting the input shifts the output by the same amount. Two approaches ensure this: (1) Phase Suppression — take the magnitude of frequency components to eliminate phase information, or (2) Cross-Correlation — compute the cross-correlation between two shift-equivariant mappings of the input. Both are implemented via small depthwise Conv1d layers in spatial and frequency domains.

The full Orchid block is a chain of order 1.5: two element-wise multiplications sandwiching one data-dependent convolution. This is analogous to attention's gating but more efficient.

### Relevant Ideas

- **Data-dependent convolution replaces attention** (Section 3.1, Eq. 1): y = h_theta(x) * x, where h_theta is a neural network that generates the convolution kernel from the input. The kernel adapts to the data, giving attention-like expressiveness without O(L^2) cost.

- **O(L log L) complexity via FFT** (Section 3.3, p6): All global convolutions computed in frequency domain using FFT. Element-wise multiplications add O(L). Total: O(M * L log L) where M = number of layers in the block.

- **Shift-equivariance preservation** (Section 3.2, p4): Shift equivariance is a key inductive bias for sequence modeling. Two conditioning network designs maintain it: phase suppression (Eq. 2) and cross-correlation (Eq. 3). This is a deep insight — it means the model naturally handles positional relationships without explicit position encoding.

- **Phase suppression conditioning** (Section 3.2, Eq. 2): h^F(x) = Conv1d(|F(Conv1d(x))|). Take absolute value of frequency components to kill phase. Simple, effective. Uses orthonormal DCT for best results.

- **Cross-correlation conditioning** (Section 3.2, Eq. 3): h^F(x) = Conv1d(F*(Conv1d(x)) . F(Conv1d(x))). Compute cross-correlation of two mappings of x. This generalizes the phase suppression approach (Remark 3.1: when k=q, cross-correlation reduces to |g^F(x)|^2).

- **Static positional bias** (Remark 3.2, p5): The conditioning network is augmented with a static positional term h_0 = FFN(PosEmb(t)). Final kernel is h = h_theta(x) + h_0. This combines data-dependent and position-dependent information.

- **Cross-attention alternative** (Remark 3.3, p5): The kernel can be conditioned on a DIFFERENT sequence u instead of x itself: y(x, u) = NN_theta(u) * x. This replaces cross-attention with a convolution, enabling efficient encoder-decoder models.

- **Chain of order 1.5 gating** (Section 3.3, p6): The Orchid block is y = (f^2 o f_* o f^1)(x) where f_* is data-dependent convolution and f^1, f^2 are element-wise multiplications (gating). This cascade of circulant and diagonal matrices approximates dense linear layers.

- **Block-diagonal matrices replace MLP** (Section 4.2, p7): In the BERT experiments, Orchid replaces the two dense matrices in MLP layers with block-diagonal matrices (b=4 blocks). This reduces MLP parameter count significantly.

- **30% fewer parameters, better performance** (Table 4.3, p8): Orchid-BERT-base (77M params) outperforms BERT-base (110M params) on GLUE by 1.0 points. Orchid-BERT-large (254M) outperforms BERT-large (340M) by 0.6 points. 25-30% parameter reduction with better results.

- **Handles sequence lengths up to 131K** (Section 4.1, Figure 4.1): Orchid successfully learns associative recall even with 131K token sequences, a scale where Transformers fail due to memory. This is the O(L log L) advantage in practice.

- **DCT outperforms DFT** (Appendix C.2, Figure C.3): Orthonormal DCT works better than DFT for the conditioning network. DCT produces real-valued transforms and handles boundaries better (even-symmetric padding vs circular padding). This is a practical implementation detail.

- **Spatial + spectral mixing** (Appendix C.2, Figure C.1): The conditioning network works best when operating in BOTH spatial and frequency domains (Conv1d in time then Conv1d in frequency). Pure spatial or pure spectral is worse.

- **No nonlinearity needed in Type II conditioning** (Appendix C.2, Figure C.2): For cross-correlation conditioning, identity (no nonlinearity) performs best or near-best. The cross-correlation itself provides sufficient nonlinearity.

- **Residual long convolution** (Section 4.2, p7): Each Orchid layer includes a residual long convolution in addition to the data-dependent one. This helps with training stability.

- **Small conditioning kernels** (Appendix C.1, p17): Conv1d kernels of length 3-5 are used in the conditioning network. Very small local operations generate the global adaptive kernel.

- **Image classification** (Section 4.3, p8): Orchid applied to ViT achieves 80.2% Top-1 on ImageNet-1k with 48M params (ViT-b is 87M for 78.5%). Works across modalities.

- **Trained on single P100 for small tasks** (Appendix C.1, p17): The associative recall experiments ran on a single P100 GPU. Small-to-medium sequence lengths fit on one GPU.

- **BERT pre-training: 4xA100 for 70k steps** (Appendix C.3, p19): Orchid-BERT-base pre-trained on 4xA100 GPUs for 70k steps with batch size 4096, sequence length 128.

- **Not yet adapted for autoregressive generation** (Section 6, p10): The paper notes that Orchid is not inherently compatible with causal/autoregressive models due to how data-dependent convolution handles dependencies. This is a limitation for GPT-style generation but not for BERT-style understanding.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | **HIGH** | O(L log L) complexity means much faster training than attention. 30% fewer parameters for same performance. FFT is highly optimized on Apple silicon (Accelerate framework). Could train a small Orchid model very fast. |
| 2. Small corpus (Bible + Summa) | **HIGH** | The 30% parameter reduction means less overfitting risk on small data. BERT-style masked LM training is well-suited to understanding theological text structure. |
| 3. Compile syllogisms to weights | **MEDIUM** | Convolution kernels have interpretable structure in frequency domain. A syllogism might correspond to specific frequency patterns in the kernel. The conditioning network could be bypassed to inject fixed kernels representing logical rules. |
| 4. Dynamic weight recompilation | **MEDIUM** | The data-dependent kernel IS dynamically generated at runtime. If you could compile syllogisms into conditioning network weights, the model would dynamically produce appropriate kernels. Alternatively, you could directly inject kernels as ternary patterns. |
| 5. Performant inference on MacBook | **HIGH** | FFT-based convolution is extremely well-optimized on M2 (Apple's Accelerate/vDSP framework). No quadratic attention overhead. Integer-FFT or fixed-point FFT could combine with BitNet for extreme efficiency. |
| 6. Diffusion text generation | **MEDIUM** | Orchid's bidirectional nature actually SUITS diffusion text generation better than autoregressive models. Diffusion denoises the entire sequence simultaneously, which is exactly what Orchid's global convolution does. This is a strong match. |
| 7. Build from scratch | **HIGH** | Code available on GitHub. The core is just FFT + small Conv1d + element-wise multiply. Very implementable in C89 using vDSP/Accelerate for FFT. |
| 8. Decompile existing models | **LOW** | Not directly applicable to decompiling, though the conditioning network's interpretable frequency-domain structure could help understand what attention layers are doing. |

### Implementation Notes

- Core operation: FFT-based convolution with data-dependent kernel
- Conditioning network: 1D depthwise Conv1d (kernel 3-5) + frequency-domain Conv1d
- Gating: Two element-wise multiplications (order 1.5 chain)
- MLP replacement: Block-diagonal matrices with b=4 blocks
- Positional encoding: Learned positional embedding + FFN for static kernel component
- Frequency transform: Orthonormal DCT (better than DFT)
- Optimizer: Adam (beta1=0.9, beta2=0.999), lr=5e-4 to 8e-4, linear warmup + decay
- Code: https://github.com/Karami-m/orchid (PyTorch)
- Apple Accelerate has vDSP_DCT and FFT — directly usable for C89 implementation
- Key sizes: Orchid-s (735K for CIFAR), Orchid-BERT-base (77M), Orchid-BERT-large (254M)

### Combinability

- **Orchid + BitNet**: Replace attention AND use ternary weights. The convolution kernel itself could be ternary {-1, 0, +1}, meaning it's just addition/subtraction in the time domain (no FFT even needed for sparse ternary kernels). This could be spectacularly efficient.
- **Orchid + TRM recursion**: Use Orchid as the backbone network in a recursive reasoning model. The O(L log L) convolution makes each recursion step cheaper than with attention.
- **Orchid + diffusion**: Orchid's bidirectional global convolution is a natural fit for diffusion-based text generation, where the model denoises the whole sequence at once.
- **Orchid + syllogism compilation**: The frequency-domain kernel representation is interesting — a syllogism might be representable as a specific spectral pattern that can be injected directly into the kernel.
- **Orchid conditioning as "meta-reasoning"**: The small conditioning network that generates the convolution kernel is essentially a meta-network that decides HOW to process the input. This is analogous to how syllogistic reasoning selects which logical rules to apply.

---

## Paper 3: TRM — "Less is More: Recursive Reasoning with Tiny Networks"

**Authors:** Alexia Jolicoeur-Martineau (Samsung SAIL Montreal)

**ArXiv:** 2510.04871v1 (Oct 2025)

### Core Technique

TRM (Tiny Recursion Model) is a remarkably simple architecture: a single tiny network (2 layers, ~5-7M parameters) that reasons by recursively refining its answer. Given an input question x, the model maintains two features: a proposed solution y (the current answer) and a latent reasoning feature z (a "chain of thought" in latent space). The model recursively updates z given (x, y, z), then updates y given (y, z).

The key insight over the prior HRM (Hierarchical Reasoning Model, which used two 4-layer networks at different frequencies) is massive simplification: one network instead of two, 2 layers instead of 4, no fixed-point theorem needed, no biological justification needed, no hierarchy needed. Despite having 4x fewer parameters than HRM, TRM achieves significantly better generalization on hard reasoning tasks.

Training uses "deep supervision": the latent features (y, z) from each supervision step are detached from the computation graph and used as initialization for the next step. The model is supervised at each step (up to N_sup=16 steps), with early stopping via a learned halting mechanism. This emulates very deep networks (42 effective layers) without backpropagating through them all.

### Relevant Ideas

- **Recursive refinement with one tiny network** (Section 4, Figure 3): A single 2-layer transformer applied repeatedly. Each step: z = net(x, y, z) then y = net(y, z). This is the entire algorithm. Stunningly simple.

- **7M parameters beats billion-parameter LLMs on reasoning** (Abstract, Table 5): TRM with 7M params achieves 45% on ARC-AGI-1 and 8% on ARC-AGI-2, beating Deepseek R1 (671B, 15.8%), Claude 3.7 (28.6%), o3-mini (34.5%), and Gemini 2.5 Pro (37.0%) on ARC-AGI-1 (except Bespoke Grok-4 at 79.6%).

- **Deep supervision = virtual depth** (Section 2.4, p3): By reusing latent features across supervision steps (without gradients), the model effectively has T*(n+1)*n_layers depth. With T=3, n=6, 2 layers: effective depth = 3*(6+1)*2 = 42 layers. The model is tiny but deep in execution.

- **Two features explained simply** (Section 4.2, p6): z_H is the current answer (can be decoded to a solution), z_L is a latent reasoning feature (cannot be decoded directly, is transformed into a solution by f_H). The model needs BOTH because: y remembers the current solution, z remembers how it got there (like chain-of-thought). Neither alone suffices.

- **Less is more — 2 layers beats 4 layers** (Section 4.4, Table 1): Increasing layers DECREASES generalization. With scarce data, tiny networks with deep recursion beat larger networks. 2 layers with n=6 recursions: 87.4%. 4 layers with n=3 recursions (same compute): 79.5%.

- **Single network beats two separate networks** (Section 4.3, Table 1): Using one network for both z-update and y-update is BETTER than separate networks (87.4% vs 82.4%). Halves parameters and improves generalization.

- **EMA for stability** (Section 4.7, Table 1): Exponential Moving Average of weights (decay=0.999) prevents sharp collapse and improves generalization significantly (79.9% to 87.4%). Critical for small-data training.

- **Attention-free variant for small contexts** (Section 4.5, Table 1): Replacing self-attention with MLP works well for fixed-size tasks (Sudoku 9x9: 87.4% with MLP vs 74.7% with attention). For variable-size tasks (Maze 30x30, ARC-AGI), attention is needed.

- **Adaptive Computational Time simplified** (Section 4.6, p7): HRM used complex Q-learning for early stopping. TRM simplifies to just a Binary Cross-Entropy halting loss, removing the need for a second forward pass entirely. Train with halting, infer with all N_sup=16 steps.

- **No fixed-point theorem needed** (Section 4.1, p5-6): HRM relied on the Implicit Function Theorem (assuming convergence to a fixed point) to justify backpropagating only through last 2 steps. TRM simply backpropagates through all n+1 recursion steps, removing this theoretical crutch. The paper shows HRM doesn't actually converge to a fixed point anyway.

- **Heavy data augmentation** (Section 5, p8): Sudoku uses 1000 shuffles per example. Maze uses 8 dihedral transformations. ARC-AGI uses 1000 augmentations (color permutation, dihedral group, translations). Augmentation is critical for small-sample learning.

- **MoE hurts** (Section, "Ideas that failed", p11): Replacing SwiGLU MLPs with SwiGLU Mixture-of-Experts decreased generalization massively. Too much capacity = overfitting when data is scarce.

- **Weight tying hurts** (Section, "Ideas that failed", p11): Tying input embedding and output head caused massive generalization drop. Too constraining.

- **Deep Equilibrium Models hurt** (Section, "Ideas that failed", p11): Using TorchDEQ for fixed-point iteration slowed training and led to worse generalization. Converging to a fixed point is not essential and may even be harmful.

- **Full backprop through recursions is best** (Section, "Ideas that failed", p11): Partial backprop (only through last k recursions) doesn't help. Back-propagating through all n+1 recursions works best.

- **Training details** (Section 6 / Hyperparameters, p11): AdamW optimizer, beta1=0.9, beta2=0.95, 2K iteration warmup, batch-size 768, hidden-size 512, N_sup=16, stable-max loss. EMA=0.999. For Sudoku: 60K epochs, lr=1e-4, weight decay 1.0. For ARC-AGI: 100K epochs, lr=1e-4 (1e-2 for embeddings), weight decay 0.1.

- **Hardware requirements** (Section, Hyperparameters, p11): Sudoku-Extreme: 1 L40S, 40GB RAM, <36 hours. Maze-Hard: 4 L40S, 40GB RAM, <24 hours. ARC-AGI: 4 H100, 80GB RAM, ~3 days. The Sudoku experiments are very accessible.

- **Supervised learning, not generative** (Section 6, Conclusion, p9): TRM is a supervised model — it produces a single deterministic answer, not a distribution. Extending to generative tasks is flagged as future work.

- **RMSNorm, no bias, rotary embeddings, SwiGLU** (Section 2.1, p2): Same LLaMA-style components as BitNet b1.58. Shared architectural vocabulary.

- **Architecture**: 2-layer transformer, hidden size 512, each layer has self-attention (or MLP for fixed-size tasks) + MLP with SwiGLU + RMSNorm + rotary embeddings.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | **CRITICAL** | 7M parameters is tiny. Sudoku training takes <36 hours on an L40S, and that's with 60K epochs on augmented data. For a small corpus like Bible+Summa, with fewer epochs and simpler augmentation, training on M2 in 30-60 minutes is very plausible. |
| 2. Small corpus (Bible + Summa) | **CRITICAL** | TRM is specifically designed for small-sample learning (~1000 examples for Sudoku). Deep recursion + tiny network + EMA + heavy augmentation = strong generalization from little data. Bible+Summa is larger than TRM's training sets. |
| 3. Compile syllogisms to weights | **HIGH** | The recursive refinement process IS syllogistic reasoning. Each recursion step applies the same logical rules to refine the answer. A syllogism could be compiled as a specific weight pattern that, when recursively applied, performs the correct inference chain. z accumulates the reasoning trace. |
| 4. Dynamic weight recompilation | **HIGH** | With only 7M parameters, recompiling the entire model is feasible in real-time. But more interesting: you could have a base model and apply syllogism-specific weight deltas. Each recursion step could use different compiled weights (though TRM uses the same weights for all steps). |
| 5. Performant inference on MacBook | **CRITICAL** | 7M parameters at FP16 = ~14MB. At 1.58 bits (with BitNet) = ~1.4MB. Inference is N_sup applications of a 2-layer network — trivially fast even on CPU. 16 forward passes through 2 layers = 32 effective layer-applications, each tiny. |
| 6. Diffusion text generation | **MEDIUM** | TRM's iterative refinement is conceptually similar to diffusion denoising. Both start with noise/guess and iteratively refine toward the answer. The deep supervision mechanism could be adapted for diffusion-style text generation where each "denoising step" refines the entire sequence. |
| 7. Build from scratch | **HIGH** | The architecture is extremely simple to implement from scratch. One 2-layer transformer, applied in a loop. The training loop is slightly more complex (deep supervision with detached gradients) but straightforward. |
| 8. Decompile existing models | **MEDIUM** | TRM's recursive structure might help understand what large models are doing internally — are they also iteratively refining an answer across layers? If so, you could "decompile" a deep model into a recursive application of a simpler pattern. |

### Implementation Notes

- Architecture: 2-layer transformer (self-attention or MLP + SwiGLU MLP), hidden dim 512
- Input: Embedded question x, initialized answer y, initialized latent z
- Forward pass: For each supervision step, run T-1 recursion processes without gradients, then 1 with gradients
- Each recursion process: n evaluations of f_L (z update) + 1 evaluation of f_H (y update)
- Optimal: T=3, n=6 (42 effective depth per supervision step)
- Loss: Softmax cross-entropy on y + binary cross-entropy halting loss
- EMA: 0.999 decay on weights — critical for stability
- Optimizer: AdamW, lr=1e-4, batch 768, hidden 512
- Training: 60K-100K epochs depending on task
- Parameters: 5M (MLP variant for Sudoku) to 7M (attention variant) to 19M (MLP for Maze)
- Hardware: Single L40S (40GB) for Sudoku, 4xH100 for ARC-AGI

### Combinability

- **TRM + BitNet**: A 7M-parameter ternary recursive model. Weights would be ~1.4MB. Each recursion step uses only integer addition. This could run inference in microseconds on M2. Training would also be fast due to tiny parameter count.

- **TRM + Orchid**: Replace self-attention in TRM's 2-layer network with Orchid convolution. For fixed-size tasks (which TRM already handles with MLP), this might not help. For variable-length text reasoning, Orchid gives O(L log L) instead of O(L^2) at each recursion step.

- **TRM + syllogism compilation**: This is the most exciting combination. Each recursion step in TRM applies the same transformation. If that transformation encodes a syllogistic rule, then n recursions = chaining n syllogistic inferences. You could compile a set of syllogisms into the 7M weights such that recursive application performs multi-step logical reasoning. z would accumulate the logical state, y would be the current conclusion.

- **TRM recursion as "rumination"**: The deep supervision mechanism (refine answer iteratively, sometimes stopping early) is analogous to how one might "ruminate" on a theological question — applying the same principles repeatedly until the answer stabilizes.

- **TRM + diffusion**: The iterative refinement could be viewed as deterministic denoising. Start with a corrupted/incomplete text, recursively refine it. This bridges the gap between TRM's supervised approach and generative diffusion.

---

## Cross-Paper Synthesis: Combined Architecture for Eigenflower

### The "Eigenflower Stack"

The three papers suggest a natural architecture when combined:

1. **Weight representation**: BitNet b1.58 ternary {-1, 0, +1} weights throughout
2. **Sequence mixing**: Orchid data-dependent convolution replacing attention (O(L log L))
3. **Reasoning strategy**: TRM recursive refinement with deep supervision (tiny network, many passes)

### Concrete Proposal

A **Ternary Recursive Orchid** model:
- 2-layer network with Orchid convolution (instead of attention) + SwiGLU MLP
- All weights ternary {-1, 0, +1} via BitNet absmean quantization
- Hidden dimension 256-512, ~2-5M parameters
- At 1.58 bits per param: 0.4-1.0 MB total model size
- Recursive refinement: n=6, T=3, N_sup=16
- Forward pass: 42 applications of a 2-layer Orchid block, each using FFT-based convolution with ternary kernels
- Training: Deep supervision with EMA (0.999)
- Inference: Pure integer addition + FFT (no floating-point multiply)
- Corpus: Bible (multilingual) + Summa Theologica (Latin/English)
- Training time estimate: With 2-5M ternary params, small corpus, M2 MacBook — plausibly under 60 minutes

### Syllogism Compilation Path

With ternary weights, a syllogism compilation scheme becomes concrete:

1. **Parse** syllogism from text (e.g., "Omnis homo est animal, omne animal est substantia, ergo omnis homo est substantia")
2. **Encode** terms as embedding indices
3. **Compile** the logical relationship as a sparse ternary weight patch: +1 connections for affirming relationships, -1 for negating, 0 for irrelevant
4. **Apply** the patch to the model's weights (or to a specific recursion step's state)
5. **Verify** by running inference — the recursive refinement should chain the compiled syllogisms

### Key Implementation Order for C89

1. **FFT/DCT library** — needed for Orchid convolution (or use Apple Accelerate vDSP)
2. **Ternary matrix operations** — BitNet's core: matmul as conditional add/subtract
3. **2-layer Orchid block** — the base network unit
4. **Recursive inference loop** — TRM's forward pass
5. **Deep supervision training loop** — with detached gradient trick
6. **Absmean quantization** — for training with ternary weights
7. **Syllogism parser + compiler** — from text to weight patches
8. **EMA weight averaging** — critical for training stability

### Open Questions

- How does BitNet ternary quantization interact with FFT-based convolution? The kernel in frequency domain might need more precision than {-1, 0, 1}.
- Can TRM's recursive refinement work for language generation, or only for question-answering? The paper explicitly notes this is an open problem.
- How few parameters can we go while still capturing the structure of theological Latin? TRM works at 5-7M for grid puzzles, but language has much higher vocabulary.
- Does Orchid's shift equivariance help or hurt for theological text where position matters (e.g., structure of syllogistic arguments)?
- Can the conditioning network in Orchid be itself compiled from syllogisms, so the model dynamically selects which logical rules to apply based on the input?
