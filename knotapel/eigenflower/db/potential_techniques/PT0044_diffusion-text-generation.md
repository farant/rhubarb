---
id: PT0044
title: "Diffusion-based text generation via latent denoising"
status: untested
source: ["MicroDiT paper (Sehwag et al., 2024)", "CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0006, PT0022]
---

Adapt image diffusion techniques for text generation. Instead of image patches, use text token embeddings. Instead of a VAE for images, use a text autoencoder (CALM-style). The noise distribution, guidance mechanism, and training schedule transfer from image diffusion.

Text is 1D (sequence of tokens) not 2D (grid of patches), so "patch masking" becomes "token masking" -- exactly what BERT-style pretraining does. Deferred masking (PT0022) applies directly.

CALM's continuous latent space is exactly what diffusion operates on. The energy-based generative head produces continuous vectors in a single step, but diffusion could be used as an alternative (requiring ~100 steps for comparable quality, or ~4 with flow matching + midpoint sampler).

Orchid's bidirectional global convolution is a natural fit for diffusion text generation -- the model denoises the whole sequence simultaneously.

Classifier-free guidance: during training, randomly drop 10% of captions/prompts to learn unconditional generation. At inference, combine conditional and unconditional: F_hat = F_unconditional + w * (F_conditional - F_unconditional).

Caveat: diffusion inference requires many forward passes (20-50 denoising steps). For MacBook deployment, single-step energy head (CALM) may be preferable.
