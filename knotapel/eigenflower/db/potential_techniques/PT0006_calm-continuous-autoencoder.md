---
id: PT0006
title: "CALM continuous autoencoder for premise encoding"
status: untested
source: ["CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0005, PT0007, PT0008]
---

Replace discrete next-token prediction with continuous next-vector prediction. A lightweight VAE compresses K discrete tokens into a single continuous vector z in R^l. The language model operates on sequences of continuous vectors, predicting the next vector at each step. An Energy Transformer generative head produces continuous vectors in a single step (no iterative sampling).

Key specifications: K=4 tokens per vector, l=128 latent dimensions. Autoencoder achieves 99.9% token-level reconstruction. VAE regularization with beta=0.001 (very small KL weight). KL clipping at lambda_KL=0.5 prevents posterior collapse. Dual dropout (p=0.15 on latent vector and on input tokens) forces robust representation.

Energy score loss is likelihood-free and sample-based: S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha]. Alpha=1.0 is optimal. Single-step generation (no iterative denoising).

For eigenflower: premises could be encoded as continuous vectors, conclusion predicted by the energy head. Creates a continuous-space version of modus ponens. 4x fewer autoregressive steps means 4x shorter sequences. Autoencoder is tiny (~75M params) and trains fast (30k steps). Autoencoder scaling is unnecessary -- the task is inherently simple.

Discrete feedback loop at inference is critical: predicted vector is decoded to K tokens, then re-embedded as input for next step. Using continuous vectors directly as input degrades performance.

Caveat: alpha < 1 in energy score causes gradient explosion. Stay in [1, 1.75].
