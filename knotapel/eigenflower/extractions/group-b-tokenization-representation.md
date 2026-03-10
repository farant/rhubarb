# Group B: Tokenization & Representation
## Paper Extractions for Eigenflower

Date: 2026-02-26

---

## Paper 1: H-Net — Dynamic Chunking for End-to-End Hierarchical Sequence Modeling

**Authors:** Sukjun Hwang (CMU), Brandon Wang (Cartesia AI), Albert Gu (CMU / Cartesia AI)
**ArXiv:** 2507.07955v2 (July 2025)
**Code:** https://github.com/goombalab/hnet
**Checkpoints:** https://huggingface.co/cartesia-ai

### Core Technique

H-Net replaces the entire tokenization-LM-detokenization pipeline with a single end-to-end hierarchical network that operates directly on raw bytes. The key innovation is **dynamic chunking (DC)**: a learned, content-aware, context-dependent segmentation mechanism that replaces handcrafted tokenizers like BPE. The architecture is a U-Net-like hierarchy with small encoder networks that compress raw bytes into chunks, a large main network that processes the compressed representation, and decoder networks that decompress back to byte resolution. The chunking boundaries are learned jointly with the rest of the model through standard differentiable optimization, with no external supervision or heuristics.

The architecture can be nested recursively: the main network itself can be another H-Net, creating multi-level hierarchies (e.g., bytes -> sub-words -> phrases). A 2-stage H-Net operating on raw bytes matches or exceeds the performance of a BPE-tokenized Transformer of twice its size, with better data scaling properties.

### Relevant Ideas

- **Tokenizer-free operation on raw bytes** (Section 1, p1): Eliminates the entire BPE/tokenizer dependency. For eigenflower's small Latin corpus, this means no vocabulary design decisions, no unknown tokens, and natural handling of multilingual text (Latin, English, any language). The model learns its own segmentation.

- **Hierarchical U-Net architecture** (Section 2.1, p4-5): Three components -- encoder E (compresses), main network M (reasons), decoder D (decompresses). The main network gets the bulk of parameters and operates on compressed sequences (L^S << L^0). This is an efficient parameter allocation strategy relevant to fitting models on M2.

- **Recursive nesting of H-Nets** (Section 2.1, p4): The main network M can itself be an H-Net, creating arbitrary depth hierarchies. Characters -> morphemes -> words -> clauses -> sentences. This mirrors natural linguistic hierarchy and could model syllogistic structure at different abstraction levels.

- **Dynamic chunking via cosine similarity routing** (Section 2.2.1, p6, Eq 4): Boundary probabilities are computed by measuring cosine similarity between adjacent encoder outputs through learned projections. Low similarity = boundary. This is elegant and lightweight: p_t = 1/2(1 - q_t^T k_{t-1} / (||q_t|| ||k_{t-1}||)). Completely differentiable. Could be implemented in C89 without complex dependencies.

- **Smoothing module for differentiable chunking** (Section 2.2.2, p7, Eq 5): An exponential moving average z_bar_t = P_t * z_hat_t + (1-P_t) * z_bar_{t-1} that makes discrete boundary decisions differentiable. This is the key trick that enables end-to-end training. It also provides adaptive error correction: low-confidence boundaries get smoothed with context from previous chunks.

- **Straight-Through Estimator (STE)** (Section 2.2.2, p8, Eq 7): Used in the upsampler to maintain discrete forward pass but continuous gradients during backpropagation. A fundamental technique for any discrete-decision-in-continuous-optimization scenario.

- **Ratio loss for controlling compression** (Section 2.2.3, p8, Eq 10): A load-balancing loss (inspired by MoE) that guides the model toward a target compression ratio N. The loss L_ratio = N/(N-1) * ((N-1)FG + (1-F)(1-G)) where F is actual fraction retained and G is average boundary probability. This prevents trivial solutions (keeping everything or nothing). Fixed alpha=0.03 in all experiments.

- **Mamba-2 (SSM) layers for encoder/decoder** (Section 2.1.2, p5-6): State Space Models dramatically outperform Transformers for the encoder/decoder role. SSMs have a natural "compression bias" -- they compress information into fixed-size states, which aligns perfectly with the chunking task. Even on BPE-tokenized inputs, SSM encoders/decoders outperform Transformer ones (Figure 10, p19).

- **Monotone width hierarchy** (Section 2.1.2, p5-6): Model width increases with depth: D^0 <= D^1 <= ... <= D^S. Inner stages get wider models since they process more compressed (more semantic) representations. Outer stages use narrow, efficient SSM layers.

- **Dimension expansion via shared trainable vector** (Section 2.1.2, p6): To go from D^s to D^{s+1}, they append a shared trainable vector to all tokens. To reduce, they take the first D^s dimensions. Very simple, no learned projection needed at the interface.

- **Network normalization** (Section 2.3, p8-9): RMSNorm placed at the END of each component (E^s, D^s, M). This "post-network norm" is critical for hierarchical architectures where residual stream magnitudes would otherwise grow unbounded across stages.

- **Separation of two streams** (Section 2.3, p9): Encoder outputs serve dual purposes: (1) residual connection to decoder and (2) compressed input to next stage. They apply a learned Linear projection only to the residual connection path, keeping the main computational path clean. Key insight: maintain intact gradient flow through the main path.

- **Residual connection initialized near zero** (Section 2.3, p9): The residual from encoder to decoder starts near zero, letting the main network dominate early training. The decoder gradually learns to incorporate fine-grained details.

- **Learning rate modulation across stages** (Section 2.3, p9): Outer stages (handling longer sequences) get proportionally higher learning rates than inner stages. Scaling accounts for effective batch size AND hidden dimensions at each stage. This is critical for stable training of hierarchical models.

- **Autoregressive inference as speculative decoding** (Section 2.4, p10): At inference, H-Net resembles speculative decoding: a small "draft model" (encoder) processes every byte, and a large "verification model" (main network) processes only chunk boundaries. The model dynamically allocates compute per token based on difficulty.

- **Content-aware chunking learns meaningful boundaries** (Section 3.1, Figure 4, p14-15): 1-stage H-Net rediscovers word boundaries (spaces). 2-stage H-Net's first stage finds character+space boundaries, second stage groups multi-word phrases like "the backbone" and "such as". This emergent behavior validates the approach without linguistic priors.

- **3.6x data efficiency on DNA** (Section 3.2, Table 5, Figure 6, p16-17): H-Net achieves the same perplexity as isotropic models with 3.6x less data on DNA (HG38 dataset). Very relevant for small-corpus scenarios.

- **Excellent performance on non-English languages** (Section 3.2, p15-16): H-Net outperforms BPE Transformers on Chinese (which has no space-based word boundaries) and code. Since eigenflower targets Latin, a language with non-trivial morphology, dynamic chunking could handle Latin inflections naturally without a specialized tokenizer.

- **Small model sizes tested** (Table 5, p17): DNA experiments used models from 29M to 66M parameters with d_model=512. These are within M2 MacBook range.

- **SSM-based H-Net encoder outperforms pure Mamba encoder on DNA** (Table 5, p17): A hybrid M3T1 (3 Mamba + 1 Transformer) encoder architecture slightly outperforms pure Mamba M4 for the encoder role.

- **Byte-level models scale better long-term** (Section 3.1, p12): All byte-level H-Net methods start off worse than tokenized Transformer but cross over at 30B-200B training bytes. The gap widens progressively after crossover. For a small corpus (Bible + Summa), this crossover might not be reached, but the data efficiency on specialized domains (DNA result) suggests it might work.

- **Robustness to textual perturbations** (Table 3, p14): H-Net (2-stage) dramatically outperforms tokenized Transformers on corrupted text (AntSpeak, Drop, RandomCase, Repeat, UpperCase). This robustness comes for free from byte-level operation.

- **Hybrid main network (Mamba + Transformer)** (Section 3.3, Figure 11, p20): A hybrid main network with 20 Mamba-2 + 7 Transformer layers (3:1 ratio) scales as well as pure Transformer. Relevant for balancing efficiency on M2.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | HIGH | DNA models are 29-66M params with d=512. SSM encoders/decoders are very efficient. The hierarchical structure means the expensive main network processes sequences ~6x shorter than input. |
| 2. Small corpus (Bible + Summa) | HIGH | 3.6x data efficiency demonstrated on DNA. Tokenizer-free means no vocabulary mismatch for Latin. No preprocessing pipeline to build. |
| 3. Compile syllogisms to weights | MEDIUM | The hierarchical chunking could naturally discover syllogistic boundaries (premise, middle term, conclusion). The nesting could model premise-level vs argument-level structure. |
| 4. Dynamic weight recompilation | LOW-MEDIUM | The ratio loss mechanism could be adapted: instead of controlling compression ratio, control which regions get updated when new syllogisms are compiled in. |
| 5. Performant inference on MacBook | HIGH | Speculative-decoding-like inference is inherently efficient. Small encoder runs on every byte, big network runs only on chunk boundaries (~1/6 of positions). |
| 6. Diffusion text generation | LOW | Paper doesn't address diffusion. But the hierarchical structure could be combined with diffusion at the chunk level (see CALM paper). |
| 7. Build from scratch | HIGH | The encoder/decoder are just small SSM stacks (4 layers each). The routing module is a single cosine similarity computation. The smoothing module is a single EMA. All very implementable in C89. |
| 8. Decompile existing models | LOW | Not directly relevant, though the hierarchical analysis of what boundaries models learn (Figure 4) is a form of interpretability. |

### Implementation Notes

- **Model sizes tested:** 190M to 1.6B parameters (Table 1, p11). DNA models: 29M-66M.
- **Training:** AdamW optimizer with warmup-stable-decay (WSD) schedule. 10% linear warmup, 20% inverse-square-root decay. Learning rates: 6.25e-4 (Large), 5.0e-4 (XL).
- **Batch size:** 256 for all models. 8192 bytes per sequence for byte-level models.
- **Data budget:** Trained on 100B token subset of FineWeb-Edu (p11). But DNA models trained on much less.
- **Architecture details for Large scale:** Main network M = 22-28 Transformer layers at d=1024-1536. Encoder/Decoder = 4 Mamba layers each at d=1024. Total ~680M-870M params.
- **Smallest viable config (DNA):** d_model=512 for everything, 64M params total, achieves strong results.
- **Code available:** PyTorch implementation at github.com/goombalab/hnet

### Combinability

- **H-Net + CALM:** H-Net provides the tokenizer-free input processing. Instead of the Transformer main network predicting individual tokens, the main network could predict continuous vectors (CALM-style) representing the next chunk. This combines tokenizer-free input with continuous-space output.
- **H-Net + Free Transformer:** The hierarchical chunking could discover natural "decision points" where latent variables (Free Transformer style) are most valuable. The H-Net encoder could also serve as the VAE encoder for the Free Transformer.
- **H-Net encoder as syllogism boundary detector:** The routing module's cosine-similarity boundary detection could be trained (or pretrained) to identify syllogistic structure boundaries in text -- premises, middle terms, conclusions.

---

## Paper 2: CALM — Continuous Autoregressive Language Models

**Authors:** Chenze Shao, Darren Li, Fandong Meng, Jie Zhou (WeChat AI / Tencent; Tsinghua University)
**ArXiv:** 2510.27688v1 (October 2025)
**Code:** https://github.com/shaochenze/calm
**Project page:** https://shaochenze.github.io/blog/2025/CALM

### Core Technique

CALM replaces discrete next-token prediction with continuous next-vector prediction. A lightweight autoencoder compresses K discrete tokens into a single continuous vector z in R^l. The language model (a standard Transformer backbone) then operates on sequences of these continuous vectors, predicting the next vector at each step. The predicted vector is decoded back into K discrete tokens by the frozen autoencoder decoder.

The key challenge is that in continuous space there is no softmax distribution, making likelihood intractable. CALM addresses this with three innovations: (1) a variational autoencoder with carefully tuned regularization to produce a robust, smooth latent space; (2) an Energy Transformer generative head that produces continuous vectors in a single step (no iterative sampling); and (3) BrierLM, a new likelihood-free evaluation metric based on the Brier score.

At K=4 tokens per vector, CALM matches the performance of discrete Transformers of comparable size while using ~4x fewer autoregressive steps, creating a new scaling axis for efficiency.

### Relevant Ideas

- **Token-chunk autoencoder** (Section 2.1, p3): A simple, lightweight autoencoder that maps K tokens to a single vector of dimension l. Encoder: K token embeddings -> FFN -> flatten K*d to R^l -> FFN -> linear to R^l. Decoder mirrors this. With K=4 and l=128, achieves 99.9% token-level reconstruction accuracy. The autoencoder has hidden dimension d=512, ~75M parameters, trained for only 30k steps.

- **Autoencoder is context-free** (Section 2.1, p3): Each chunk of K tokens is encoded independently, without seeing surrounding context. This is a deliberate simplicity choice. The authors note context-aware autoencoders are a promising future direction.

- **Variational regularization for smooth latent space** (Section 2.2, p4, Eq 2-3): A reconstruction-only autoencoder learns a "brittle" latent space where tiny perturbations cause complete reconstruction failure. The VAE formulation (z ~ N(mu, sigma^2 I)) with KL divergence penalty creates a smooth manifold. Critical insight: the generative model needs to predict into this space, so smoothness matters enormously.

- **KL divergence weight beta = 0.001** (Section 2.2, p4): Very small KL weight -- just enough to regularize the space without destroying reconstruction fidelity. At beta=0.1, reconstruction drops to ~99% and downstream performance collapses.

- **KL clipping to prevent posterior collapse** (Section 2.2, p4, Eq 4): Individual latent dimensions can collapse to the prior (zero information). They clip each dimension's KL loss at a floor of lambda_KL = 0.5. This ensures every dimension actively participates in encoding.

- **Dual dropout for robustness** (Section 2.2, p4): (1) Dropout on the latent vector z (p=0.15) before decoding forces redundant representation. (2) Dropout on input tokens (p=0.15) forces the encoder to capture semantic context (like CBOW), not just memorize token indices. Both are critical -- each contributes orthogonal performance gains (Table 2, p17).

- **Latent dimension l=128 is optimal for K=4** (Section 7.3, Figure 7, p17): l=32 is too compact (brittle), l=256 wastes capacity (noise encoding). The sweet spot is l=128 for K=4 tokens. Scaling the autoencoder (more layers, bigger d, more data) does NOT improve downstream performance -- the task is inherently simple.

- **Energy Transformer generative head** (Section 3.3, p6-7): The generative head takes the Transformer's hidden state h and a random noise vector epsilon ~ U[-0.5, 0.5] and produces the predicted continuous vector through a stack of L residual MLP blocks. Each block: fuse epsilon with h via linear layers -> SwiGLU -> residual connection -> final linear to R^l. Single-step generation, no iterative denoising.

- **Energy loss (strictly proper scoring rule)** (Section 3.3.2, p6, Eq 9-10): The energy score S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha]. First term encourages diversity (penalizes collapsed predictions), second encourages accuracy (predictions close to truth). Trained with N=8 model samples and M=100 target samples from the posterior. Alpha=1.0 is optimal. This is likelihood-free -- only requires samples, not density evaluations.

- **Discrete feedback loop at inference** (Section 3.3.3, p7): At inference, the predicted vector z is immediately decoded to K discrete tokens, which are then re-embedded as the input for the next step. This "grounds" the autoregressive process in discrete token space. Using continuous vectors directly as input DEGRADES performance (Table 5, p19) because the latent space is too compact/brittle for the Transformer to unpack.

- **Input compression MLP** (Section 3.3.3, p7): Instead of feeding continuous vectors to the Transformer, the K decoded tokens are embedded and compressed by a 2-layer MLP into a single representation. This maintains the efficiency gain (1 position per K tokens) while using stable discrete representations.

- **BrierLM evaluation metric** (Section 4.2, p8-9, Eq 14-15): An unbiased estimator of the Brier score using only 2 samples from the model: Brier(P,y) ~ I{x1=y} + I{x2=y} - I{x1=x2}. Extended to n-grams (Brier-n) and combined as geometric mean (BrierLM). Correlates nearly perfectly with cross-entropy loss (Pearson -0.966, Spearman -0.991). This is a universal evaluation tool that works for ANY implicit generative model.

- **Likelihood-free temperature sampling** (Section 5, p10-12, Algorithms 1-2): A rejection sampling algorithm that achieves exact temperature scaling P_T(x) proportional to P(x)^{1/T} using only a black-box sampler. Two stages: integer part (draw n identical samples) and fractional part (Bernoulli factory). Batch approximation (Algorithm 2) is practical for low temperatures.

- **Performance-compute frontier** (Section 7.2, Table 1, p15): CALM-M (371M params, K=4) matches Transformer-S (281M) BrierLM with 44% fewer training FLOPs and 34% fewer inference FLOPs. CALM-L (735M, K=4) matches Transformer-L (849M) at lower cost.

- **K as a new scaling axis** (Section 7.2, Figure 4, p16): Increasing K from 1 to 4 provides dramatic efficiency gains. K=2 nearly halves cost with marginal quality drop. K=8 degrades quality, likely needs bigger models. This is a fundamentally new dimension for scaling.

- **Autoencoder scaling is unnecessary** (Section 7.3, p17): Doubling encoder/decoder layers, doubling d, expanding training to 100B tokens -- NONE improved downstream BrierLM. The autoencoder task is simple enough that a lightweight model suffices. This is excellent for resource-constrained settings.

- **Energy-based head outperforms diffusion and flow matching** (Section 7.4, Figures 8-9, p18): Energy head produces best results in a single step. Diffusion needs ~100 steps for comparable quality. Flow matching with midpoint sampler needs ~4 steps but still doesn't match energy head quality. Single-step generation is essential for inference efficiency.

- **Training details** (Section 7.1, p15): Autoencoder: hidden d=512, latent 32K, trained 30k steps on 15B tokens with batch 512k. CALM: trained 250k steps on remaining data, context length 2048 steps (= 2048K tokens), batch 2M tokens, AdamW with lr=3e-4, constant schedule, 2000 warmup, weight decay 0.1, grad clip 1.0.

- **Four model scales** (Section 7.1, p15): S (12 layers, d=768, 281M), M (16 layers, d=1024, 465M), L (16 layers, d=1536, 849M), XL (16 layers, d=2560, 1.82B). Generative head = L/4 MLP blocks, ~10% of total parameters.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | HIGH | 4x fewer autoregressive steps means 4x shorter sequences for the same text. The autoencoder is tiny and trains fast (30k steps). The Transformer backbone at S scale (12 layers, d=768) is reasonable for M2. |
| 2. Small corpus (Bible + Summa) | HIGH | The autoencoder is context-free and simple -- it would work well on any corpus. K=4 means Bible+Summa in token terms becomes ~4x shorter in vector terms, making context windows much more effective. |
| 3. Compile syllogisms to weights | VERY HIGH | The continuous latent space is the key insight. A syllogism could be encoded as a continuous vector (or small set of vectors). "Compiling" a syllogism means mapping its logical structure to a point in latent space. Dynamic recompilation means updating the autoencoder or adding vectors. |
| 4. Dynamic weight recompilation | HIGH | The autoencoder is separate from the LM. You could retrain the autoencoder to incorporate new logical structures (new syllogisms) without touching the LM weights. Or add new vectors to the latent space. |
| 5. Performant inference on MacBook | VERY HIGH | Single-step energy head (no iterative denoising). 4x fewer autoregressive steps. Autoencoder overhead is negligible. This is designed for efficiency. |
| 6. Diffusion text generation | HIGH | The paper explicitly compares Energy head vs diffusion vs flow matching for the generative head. Diffusion works but needs ~100 steps. Flow matching with midpoint needs ~4. Energy head needs 1. All are drop-in replacements. The continuous latent space is exactly what diffusion operates on. |
| 7. Build from scratch | HIGH | The autoencoder is extremely simple (2 FFNs + linear). The energy head is a stack of residual MLPs. The backbone is standard Transformer. All implementable in C89. |
| 8. Decompile existing models | MEDIUM | The autoencoder could potentially be trained to reconstruct token sequences from any model's hidden states, creating a "decoder" for understanding what representations mean. BrierLM could evaluate black-box models. |

### Implementation Notes

- **Autoencoder training:** 30k steps, batch 512k tokens, d=512, ~75M params. On M2 this would be fast (minutes, not hours).
- **Generative head:** ~10% of total model parameters. L/4 MLP blocks where L = number of Transformer layers.
- **Critical hyperparameters:** beta=0.001 (KL weight), lambda_KL=0.5 (KL clipping floor), dropout=0.15 (both latent and token), l=128 (latent dim for K=4), N=8 (model samples), M=100 (target samples), alpha=1.0 (energy score exponent).
- **Training cost:** 250k steps for the LM. At S scale (281M) this is tractable on good GPUs in hours, potentially on M2 in a day or two with careful optimization.
- **Key failure mode:** alpha < 1 in energy score causes gradient explosion. alpha=2 causes score to be 0 (not strictly proper). Stay in [1, 1.75].

### Combinability

- **CALM + H-Net:** Use H-Net's dynamic chunking to determine WHERE to split text into chunks (variable K), then CALM's autoencoder to compress each chunk into a continuous vector. This combines the best of both: learned boundaries + continuous representation.
- **CALM + Free Transformer:** The continuous latent space could contain both the autoencoder's semantic information AND Free Transformer-style latent variables that capture global structure (sentiment, topic, logical mode). The latent Z could control "what kind of syllogism to generate."
- **CALM autoencoder for syllogism compilation:** Train one autoencoder for natural language chunks, and another specialized autoencoder that maps syllogistic structures (premise-premise-conclusion triples) to continuous vectors. The LM backbone learns to operate on both.
- **Energy loss for syllogism training:** The energy score is likelihood-free and sample-based. This means you don't need to compute exact probabilities -- just generate samples and compare to targets. This could work for training a model to produce valid syllogisms.

---

## Paper 3: The Free Transformer — Latent Variables via Conditional VAE

**Authors:** Francois Fleuret (FAIR at Meta)
**ArXiv:** 2510.17558v1 (October 2025)
**Correspondence:** fleuret@meta.com

### Core Technique

The Free Transformer extends a standard decoder-only Transformer by conditioning its generative process on learned latent random variables Z. During generation, Z is sampled from a uniform prior over 2^H values (H=16, so 65,536 possible values). During training, Z is inferred by an encoder (a single non-causal Transformer block that shares most of its computation with the decoder) and optimized via a VAE objective with KL divergence penalty.

The key insight is that standard autoregressive models must implicitly infer latent structure (topic, sentiment, logical mode) from the token stream itself, which wastes capacity and causes errors when early tokens are ambiguous. The Free Transformer makes these latent decisions EXPLICIT and UP-FRONT: sample Z once, then generate conditioned on Z. This is implemented with minimal overhead (~3% compute/memory) by reusing the decoder's first L/2 layers as shared computation, adding only one non-causal encoder block and two linear layers.

The binary mapper trick (encoding H independent Bernoulli bits as a one-hot vector of dimension 2^H) enables gradient pass-through via the monotonicity of the sigmoid, avoiding the need for Gumbel-Softmax or other relaxation tricks.

### Relevant Ideas

- **Latent variables condition autoregressive generation** (Section 2, p1-2): A purely autoregressive model has no way to "decide" global properties of the sequence before generating it. It must maintain implicit running estimates of latent quantities, requiring unnecessarily complex computation. The Free Transformer makes these decisions explicit by sampling Z first.

- **The coin-flip example** (Section 2, p1-2, Eq 1-2): A brilliant motivating example. If X_1,...,X_T are conditionally independent given a hidden coin flip Z, then P(X_{t+1}|Z) is trivial, but P(X_{t+1}|X_1,...,X_t) involves an increasingly complex posterior calculation. This directly parallels syllogistic reasoning: once you know the logical mode (Z), the tokens follow simply.

- **Conditional VAE formulation** (Section 3.1, p3, Eq 3): P(S) = integral P(S|Z=z)P(Z=z)dz. The encoder Q(Z|S) provides training signal for the decoder P(S|Z), while the KL divergence D_KL(Q(Z|S)||P(Z)) limits information flow through Z. This is the standard VAE framework but applied to autoregressive text generation.

- **Split decoder architecture** (Section 3.2, p4-5, Figure 2, Algorithm 2): The decoder is split at its midpoint (layer L/2). First half processes tokens causally as normal. Then Z is injected: the latent representation R is added to the keys/values of the (L/2+1)-th block. Second half continues causally with Z's influence. This means the first half builds context-independent representations, the second half conditions on the latent structure.

- **Encoder reuses decoder computation** (Section 3.2, p4-5): The encoder gets the output of the decoder's first L/2 layers as keys/values, but uses a LEARNED CONSTANT QUERY (zeta) replicated across all positions. This prevents the encoder from building token-wise mappings and forces it to capture GLOBAL properties. Only one additional non-causal Transformer block is needed for the encoder.

- **Learned constant query zeta** (Section 3.3, p5): The encoder block uses a single trained embedding zeta (replicated to match sequence length) as queries, with decoder activations as keys/values. This is critical: it prevents the encoder from copying individual tokens and forces it to extract global, transferable features like topic, sentiment, or logical structure.

- **Binary mapper for discrete latent space** (Section 3.4, p5-6, Eq 6-8): The encoder outputs H logits per position, each interpreted as a Bernoulli probability for one bit. H independent bits give 2^H possible values. The one-hot encoding of the resulting binary number serves as Z. Gradient pass-through uses: Y_{t,d} + G_{t,d} - detach(G_{t,d}) where G is the soft probability. The sigmoid's monotonicity makes this well-behaved.

- **Free bits method prevents KL collapse** (Section 3.3, p5-6, Eq 5): KL divergence per position is thresholded at kappa: only positions where KL exceeds kappa contribute to the loss. This prevents the common VAE failure mode where all latent dimensions collapse to the prior and Z becomes meaningless.

- **kappa controls information rate** (Section 4.4, p9): kappa = log(2)/64 gives 1/64 bit per token. kappa = log(2)/8 gives 1/8 bit. kappa = log(2) gives 1 bit. kappa = 8*log(2) gives 8 bits. At 4 bits (kappa = 4*log(2)), the encoder encodes the ENTIRE sequence in Z, making the decoder trivial and downstream tasks collapse. The sweet spot is 1/2 to 1 bit per token.

- **Massive improvements on code/math tasks** (Tables 1-3, p10-11): At 1.5B params, 1/4 bit Free Transformer improves HumanEval by +44%, MBPP by +28%, GSM8K by +12% over baseline. At 8B with 1T tokens, MBPP improves +6%, HumanEval +11%. These are tasks requiring planning/reasoning -- exactly where up-front latent decisions help most.

- **Minimal overhead** (Section 4.2, p8): Free Transformer requires only ~3.1-3.6% more compute and memory than baseline. One extra Transformer block (non-causal) for encoder + two linear layers. This is remarkable efficiency for significant performance gains.

- **Scaling to 8B / 1T tokens** (Section 4.5, Table 3, p11): Even at large scale with extensive training, the Free Transformer maintains improvements on reasoning tasks. This isn't just a small-model trick.

- **Synthetic dataset reveals latent structure** (Section 4.1, p7-8, Figures 3-4): Training on sequences with hidden structure (repeated letter at random position), the Free Transformer progressively encodes more information in Z as kappa increases: first nothing (vanilla model), then target position, then target position + noise pattern. At kappa=8*log(2) it encodes the full sequence and generation breaks.

- **Non-causal encoder is necessary** (Section 3.3, p5): The encoder must see the full sequence bidirectionally because the latent variables may depend on long-range structure. The choice of what Z should represent is a global property of S.

- **Z is sampled per-position** (Section 3.2, p4-5): Z is actually a sequence of T one-hot vectors, one per position. This means different positions can have different latent values. At generation time, each Z_t is sampled independently from the uniform prior.

- **Prior is uniform** (Section 3.2, p5): P(Z_t = z) = 1/2^H for all z. This is maximally uninformative -- the decoder learns to use whatever Z it gets. The encoder's job during training is to pick the Z that best explains S.

- **Information bottleneck prevents encoder from dominating** (Section 3.1, p3): Without KL penalty, the encoder could learn to copy S entirely into Z, making the decoder trivial but useless at generation time (when the encoder isn't available). The KL penalty forces Z to contain only the most useful latent information.

### Relevance to Eigenflower Goals

| Goal | Relevance | Notes |
|------|-----------|-------|
| 1. Train in 30-60 min on M2 | MEDIUM | 3% overhead is negligible. But the 1.5B models tested are too large for M2. Scaling down to ~100M with the same architectural idea should work. The technique improves sample efficiency, meaning less training data needed. |
| 2. Small corpus (Bible + Summa) | HIGH | The latent variables could capture corpus-level structure: theological topic, argument type (syllogistic vs analogical vs authoritative), language register. With a small specialized corpus, Z has to capture less variation, so H can be smaller. |
| 3. Compile syllogisms to weights | VERY HIGH | This is potentially the most relevant paper for syllogism compilation. Z could explicitly represent the LOGICAL MODE: what kind of syllogism is being expressed, what the middle term is, whether this is a major premise / minor premise / conclusion. The encoder learns to extract this from text, the decoder learns to generate text conditioned on it. Compiling a syllogism means finding the Z that produces it. |
| 4. Dynamic weight recompilation | HIGH | To record a new syllogism: (1) encode it with the encoder to find its Z, (2) update weights so the decoder better generates from that Z, or (3) expand the Z space (increase H) to accommodate new logical structures. The separation between Z (what to say) and decoder (how to say it) is exactly the right factorization. |
| 5. Performant inference on MacBook | HIGH | Only 3% overhead. Z is sampled once (or a few times) per sequence, then generation proceeds as normal autoregressive. Could even pre-select Z for known syllogisms (deterministic inference). |
| 6. Diffusion text generation | MEDIUM | The Free Transformer operates in discrete token space with discrete latent variables. But the authors note (Section 6, p12) that combining this with chains-of-thought in latent space and RL is promising. The binary Z could be replaced with continuous Z for diffusion-style generation. |
| 7. Build from scratch | HIGH | Architecture is extremely simple: standard decoder Transformer + one extra non-causal block + one learned query embedding + two linear layers + binary mapper (sigmoid + thresholding). The binary mapper is perhaps 20 lines of C89. |
| 8. Decompile existing models | VERY HIGH | The encoder's job is literally to extract latent structure from text. If trained on the output of an existing model, the encoder would reveal what latent decisions the model is making implicitly. The information rate (controlled by kappa) tells you how many bits of latent structure are being used. |

### Implementation Notes

- **Model sizes tested:** 1.5B (28 layers, d=1536, 12 query heads, 2 KV heads) and 8B (32 layers, d=4096, 32 query heads, 8 KV heads). Both use standard Llama-3-style architecture.
- **H = 16 in all experiments** (Section 4.3, p9): 2^16 = 65,536 possible latent values per position. This was chosen to be comparable to vocabulary size (~130k).
- **Training infrastructure:** 32 H100s for 12 hours (1.5B), 256 H100s for 24 hours (8B). These are large-scale experiments, but the technique should scale DOWN gracefully.
- **Key hyperparameter:** kappa (free bits threshold). Start with kappa = log(2)/2 for 1/2 bit per token. This is the validated sweet spot for balanced improvement.
- **Optimizer:** Standard Llama training recipe -- AdamW, RMSNorm, SwiGLU, RoPE, GQA. No hyperparameter tuning for the Free Transformer modifications.
- **The encoder is evaluated only during training and KV cache pre-filling.** During autoregressive generation, only the decoder runs. Z is sampled from the uniform prior.
- **Weight tying** (1.5B model only): Embeddings tied with logit readout.

### Combinability

- **Free Transformer + H-Net:** Use H-Net's hierarchical chunking for input processing, and inject Free Transformer latent variables at the chunk level. Z could represent the "intent" or "logical structure" of each chunk, while H-Net handles the byte-to-chunk mapping.
- **Free Transformer + CALM:** Combine continuous vector prediction (CALM) with latent conditioning (Free Transformer). The latent Z decides WHAT to generate (logical mode, topic), and the energy head generates the continuous vector representing HOW to say it. The decoder then produces tokens from the vector.
- **Free Transformer for syllogism encoding:** Train with a corpus of labeled syllogisms. The encoder should learn Z values that correspond to syllogistic figures (Barbara, Celarent, Darii, Ferio, etc.). At inference, you select the Z for the desired syllogistic form and generate the instantiated argument.
- **Binary Z as syllogism address:** With H=16, you have 65k possible "logical modes." A syllogism library could assign specific Z values to specific syllogistic forms. "Compiling" a new syllogism means training the decoder to produce it when given that Z.
- **kappa as knowledge control:** Lower kappa = less structure in Z = more freedom for decoder = more creative/varied text. Higher kappa = more structure in Z = more deterministic/logical output. This is a built-in "reasoning vs creativity" dial.

---

## Cross-Paper Synthesis: Combined Architecture Ideas

### The "Eigenflower Stack" — A Possible Combined Architecture

Reading these three papers together suggests a layered architecture:

1. **Input Layer (from H-Net):** Raw bytes (Latin/English UTF-8) enter an SSM-based encoder that learns to chunk text into semantically meaningful units (words, phrases, logical atoms). No tokenizer. The routing module's cosine similarity naturally discovers Latin morphological boundaries.

2. **Compression Layer (from CALM):** Each chunk is compressed by a lightweight VAE into a continuous vector. The variational regularization ensures smooth, robust latent space. For K=4 Latin tokens, l=128 dimensions suffices.

3. **Latent Conditioning (from Free Transformer):** Before the main network processes the vector sequence, latent variables Z are sampled (generation) or inferred (training). Z captures global logical structure -- syllogistic form, argument type, theological topic. The binary mapper with H bits gives 2^H possible logical modes.

4. **Main Network:** A modest Transformer (or hybrid SSM-Transformer) processes the compressed, Z-conditioned sequence. This is where the bulk of parameters live, but it operates on dramatically shortened sequences.

5. **Output Head (from CALM):** An energy-based generative head produces the next continuous vector in a single step. No iterative sampling.

6. **Decoding:** The frozen autoencoder decoder maps the continuous vector back to K discrete tokens (or bytes, if using H-Net-style decoding).

### Key Insight: Three Axes of Compression

The three papers together reveal three orthogonal axes for making LMs more efficient:

1. **Spatial compression** (H-Net): Reduce sequence length by chunking bytes into meaningful units. ~6x compression.
2. **Representational compression** (CALM): Increase information per autoregressive step by predicting K tokens as one vector. ~4x compression.
3. **Structural compression** (Free Transformer): Factor out global structure into latent variables so the decoder doesn't waste capacity re-inferring it. ~3% overhead for significant quality gains.

Applied together, an eigenflower model could potentially achieve 24x fewer autoregressive steps than a character-level baseline while maintaining quality, making 30-60 minute training on M2 more feasible.

### Syllogism Compilation Path

The Free Transformer's Z variable is the most direct path to "compiling syllogisms to weights":

1. **Parse syllogism** into logical form (can be done with existing rhubarb parsers)
2. **Encode** the logical form as a specific Z value (or learn the mapping)
3. **Train** the decoder to produce correct instantiated text when given that Z + premises
4. **At inference**, "running" a syllogism means: set Z to its code, provide premises, generate conclusion

The CALM autoencoder adds a second layer: premise-pairs could be encoded as continuous vectors, and the energy head could be trained to produce the conclusion vector from premise vectors -- essentially a continuous-space version of modus ponens.

### Data Efficiency Estimates

For a Bible + Summa corpus (~4-5M tokens of Latin + English):

- **H-Net approach:** ~4-5M raw bytes per language = ~8-10M bytes total. H-Net achieves strong results at 30B bytes on DNA (a highly specialized domain). The corpus is 3000x smaller, but the domain is much more structured. The 3.6x data efficiency advantage helps.
- **CALM approach:** 4-5M tokens / K=4 = ~1M vectors. Context length 2048 vectors = 8192 tokens. Training for even 50k steps with batch size 64 = 3.2B token-equivalents of computation (repeated passes over corpus).
- **Free Transformer:** The ~3% overhead is negligible. The key question is whether the small corpus provides enough variation for the encoder to learn useful latent structure. Theological text is highly structured, which should help.

### Hardware Considerations for M2 MacBook

- **CALM-S equivalent (12 layers, d=768):** ~281M params = ~1.1GB in float32, ~562MB in float16. Fits easily in M2's unified memory.
- **Autoencoder (75M params):** ~300MB float32. Negligible.
- **Energy head (~10% of backbone):** ~28M params. Negligible.
- **H-Net encoder/decoder (4 SSM layers each, d=512):** ~20-30M params. Negligible.
- **Total for combined architecture:** ~400-500M params = ~1.6-2GB float32. M2 MacBook Pro with 16GB has plenty of room.
- **Training throughput:** Metal/MPS acceleration for matrix ops. SSM layers are particularly efficient on Apple Silicon due to sequential nature matching the hardware's strength with unified memory.

### Risk Assessment

- **H-Net:** Well-validated (760M-1.6B scale), code released, strong ablations. Risk: may not match tokenized models at very small scales.
- **CALM:** Code released, thorough ablation study. Risk: energy loss training can be unstable (alpha < 1 causes explosion). Autoencoder design is critical.
- **Free Transformer:** Minimal code complexity, but tested only at 1.5B+ scale with 47B+ tokens. Risk: may not work well with very small models or corpora. Performance during training is "often unstable" per the conclusion.
- **Combined architecture:** Novel combination, no one has tried this specific stack. Risk: interactions between the three techniques are unpredictable. Start with one, add others incrementally.

### Recommended Implementation Order

1. **Start with Free Transformer** (simplest modification, most relevant for syllogism compilation)
2. **Add CALM autoencoder** (independent training, provides continuous representation)
3. **Add H-Net dynamic chunking** (most complex, but eliminates tokenizer dependency)
4. **Combine all three** once each is validated independently
