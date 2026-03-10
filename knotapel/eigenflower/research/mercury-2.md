# Mercury 2 — Inception Labs (February 2026)

## What It Is

Mercury 2 is the first commercial **diffusion-based reasoning LLM**. Instead of generating
text token-by-token (autoregressive), it generates through **parallel refinement** — producing
multiple tokens simultaneously and iteratively converging toward the final output. Think of
an editor revising an entire draft at once rather than writing word by word.

## Key Numbers

- **Speed**: 1,009 tok/s on NVIDIA Blackwell GPUs
- **Latency**: 1.7 seconds end-to-end (vs 14.4s Gemini 3 Flash, 23.4s Claude Haiku 4.5)
- **Context window**: 128K tokens
- **Pricing**: $0.25/M input, $0.75/M output (very cheap)
- **Benchmarks**: AIME 91.1, GPQA 74, LCB 67, SciCode 38, IFBench 71

## How Diffusion LLMs Work (General)

### Forward Process (Training)
Start with clean text. Gradually corrupt it over T timesteps by probabilistically replacing
tokens with mask/noise tokens via categorical transition matrices. This is the "adding noise"
phase — analogous to image diffusion but operating on discrete tokens.

### Reverse Process (Inference)
Start with fully masked/noisy sequence. A learned denoiser (typically a bidirectional
transformer) iteratively recovers the clean text over several refinement steps. Because the
model sees the WHOLE sequence at each step (bidirectional attention), it can generate all
positions in parallel rather than left-to-right.

### Why It's Faster
Autoregressive: N tokens = N forward passes (sequential, can't parallelize)
Diffusion: N tokens = ~10-30 refinement steps (parallel across all positions)

For long outputs, diffusion wins by a large margin. The catch is each refinement step
processes the full sequence, so for very short outputs autoregressive can still be faster.

### Key Variants
- **MDLM** (Masked Diffusion Language Models): uses masking as the noise process, similar
  to BERT training. NeurIPS 2024. Encoder-only architecture.
- **Discrete diffusion**: operates directly on token IDs via categorical transitions
- **Continuous diffusion on embeddings**: map tokens to continuous space, diffuse there,
  map back (closer to image diffusion)

## Mercury 2 Specifics (What We Know)

Inception Labs hasn't published full architectural details. What's public:
- Diffusion-based, not autoregressive
- Reasoning capabilities (chain-of-thought equivalent via iterative refinement?)
- Tool usage and JSON output support
- Model size: undisclosed
- Training data: undisclosed
- The parallel refinement process is their core innovation

## Previous Work: Mercury 1 (2025)

Mercury 1 was the prototype/proof-of-concept. Mercury 2 adds reasoning capabilities,
bringing it to competitive quality with frontier models while maintaining the speed advantage.

## Relevance to Eigenflower

### Direct Relevance
1. **Diffusion text generation is on our list** — Mercury 2 proves the approach works at
   commercial quality
2. **Parallel generation** — could be very efficient on M2's unified memory architecture
   since you're doing batched matrix ops rather than sequential decoding
3. **Bidirectional context** — the model sees the whole sequence at each step, which could
   be powerful for a structured corpus like the Summa where cross-references matter

### Architecture Implications
- A small diffusion LM (50-100M params) could potentially be trained on Bible+Summa
- MDLM approach (masked diffusion) is simpler to implement than full discrete diffusion
- The denoiser is just a bidirectional transformer — architecturally simpler than
  autoregressive (no causal masking needed)
- Could combine with BitNet ternary weights for extreme efficiency

### Syllogism Connection
Diffusion's iterative refinement is conceptually interesting for syllogistic reasoning:
- Start with a noisy/incomplete logical chain
- Each refinement step applies logical constraints
- Converge to valid syllogism
- This is more natural than left-to-right generation for logical structures

### Open Questions
- Can we implement discrete diffusion efficiently in C89?
- What's the minimum number of refinement steps needed for coherent output?
- How does diffusion interact with very small corpora?
- Can we combine compiled syllogism weights with a diffusion denoiser?

## Sources
- https://www.infoworld.com/article/4137528/inceptions-mercury-2-speeds-around-llm-latency-bottleneck.html
- https://the-decoder.com/inception-launches-mercury-2-the-first-diffusion-based-language-reasoning-model/
- https://www.morningstar.com/news/business-wire/20260224034496/inception-launches-mercury-2-the-fastest-reasoning-llm-5x-faster-than-leading-speed-optimized-llms-with-dramatically-lower-inference-cost
- https://s-sahoo.com/mdlm/ (MDLM reference)
- https://spacehunterinf.github.io/blog/2025/diffusion-language-models/ (general diffusion LM explainer)
