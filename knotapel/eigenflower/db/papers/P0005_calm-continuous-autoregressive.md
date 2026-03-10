---
id: P0005
title: "CALM: Continuous Autoregressive Language Models"
authors: [Chenze Shao, Darren Li, Fandong Meng, Jie Zhou]
year: 2025
arxiv_id: "2510.27688"
url: "https://github.com/shaochenze/calm"
extraction_group: B
relevance: high
key_techniques: [continuous_next_vector_prediction, token_chunk_autoencoder, energy_transformer_head, variational_regularization, brier_score_evaluation, likelihood_free_sampling]
summary: "Replaces discrete next-token prediction with continuous next-vector prediction. A lightweight VAE compresses K tokens into one vector. Energy Transformer head generates predictions in a single step. At K=4, matches discrete Transformers with 4x fewer autoregressive steps."
related: []
---

## Key Contributions

CALM replaces discrete next-token prediction with continuous next-vector prediction. A lightweight autoencoder compresses K discrete tokens into a single continuous vector z in R^l. The language model operates on sequences of these continuous vectors. An Energy Transformer generative head produces continuous output vectors in a single step (no iterative sampling). BrierLM provides a new likelihood-free evaluation metric.

## Key Techniques

- **Token-chunk autoencoder**: Maps K tokens to one vector of dimension l. Context-free (each chunk encoded independently). K=4, l=128 achieves 99.9% token-level reconstruction. ~75M params, trained for only 30k steps.
- **Variational regularization**: VAE formulation (z ~ N(mu, sigma^2 I)) with KL divergence penalty creates smooth manifold. beta=0.001 (very small). KL clipping at lambda_KL=0.5 prevents posterior collapse.
- **Dual dropout**: (1) Dropout on latent z (p=0.15) forces redundancy. (2) Dropout on input tokens (p=0.15) forces semantic capture. Both critical, orthogonal gains.
- **Energy Transformer generative head**: Stack of L residual MLP blocks. Takes hidden state h + random noise epsilon ~ U[-0.5, 0.5] -> SwiGLU -> residual -> output R^l. Single-step generation.
- **Energy loss (strictly proper scoring rule)**: S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha]. First term = diversity, second = accuracy. Alpha=1.0 optimal. Likelihood-free.
- **Discrete feedback loop**: At inference, predicted vector is decoded to K discrete tokens, re-embedded as input. Grounding in discrete space prevents drift.
- **BrierLM**: Universal evaluation metric using only 2 model samples. Correlates -0.966 with cross-entropy.

## Specific Relevance to Eigenflower

- **Continuous latent space for syllogisms**: A syllogism could be encoded as a continuous vector. "Compiling" = mapping logical structure to latent space point. Dynamic recompilation = updating autoencoder or adding vectors.
- **4x efficiency**: 4x fewer autoregressive steps. Bible+Summa becomes ~4x shorter in vector terms.
- **Diffusion text generation**: Paper explicitly compares Energy head vs diffusion vs flow matching. The continuous latent space is exactly what diffusion operates on. Diffusion works but needs ~100 steps; Energy head needs 1.
- **Autoencoder scaling unnecessary**: Doubling layers/d/training data did NOT improve downstream performance. Lightweight model suffices -- excellent for constrained settings.

## Key Results

- CALM-M (371M, K=4) matches Transformer-S (281M) with 44% fewer training FLOPs and 34% fewer inference FLOPs
- K=4: optimal tradeoff. K=2 nearly halves cost. K=8 degrades quality.
- Energy head outperforms diffusion (~100 steps) and flow matching (~4 steps) in single step
- Autoencoder: 99.9% reconstruction at K=4, l=128

## Key Equations

- Energy score: S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha], alpha=1.0
- VAE: L = L_recon + beta * D_KL(q(z|x) || p(z)), beta=0.001
- KL clipping: max(D_KL_i, lambda_KL), lambda_KL=0.5

## Implementation Notes

- Autoencoder: d=512, ~75M params, 30k steps on 15B tokens, batch 512k
- CALM backbone: S (12 layers, d=768, 281M) to XL (16 layers, d=2560, 1.82B)
- Generative head: L/4 MLP blocks, ~10% of total parameters
- Training: 250k steps, context length 2048 steps (=2048K tokens), AdamW lr=3e-4, weight decay 0.1
- Critical: alpha < 1 causes gradient explosion; alpha=2 causes score to be 0. Stay in [1, 1.75].
- Code: github.com/shaochenze/calm
