---
id: PT0042
title: "Precomputed embeddings for corpus as one-time cost"
status: promoted_to_kit
promoted_to: KIT0016
source: ["MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0022, PT0025]
---

Precompute text embeddings for the entire Bible + Summa corpus once and store as fixed tensors. Training then operates purely on embeddings, never touching raw text. This amortizes encoding cost and simplifies the training loop.

The embedding cost is paid once regardless of how many training experiments are run. For eigenflower's iterative development process (many training experiments on the same corpus), this is a significant time saver.

For diffusion-based text generation, both text (encoder) and "image" (text VAE) embeddings would be precomputed. For standard LM training, input embeddings are precomputed by whatever encoder is chosen.

Implementation: straightforward -- run encoder once over corpus, save results as binary arrays. Training loop loads precomputed arrays instead of raw text. Requires choosing an encoder (or using byte-level processing from H-Net which would make precomputation unnecessary).
