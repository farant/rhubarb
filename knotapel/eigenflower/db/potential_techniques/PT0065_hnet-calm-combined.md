---
id: PT0065
title: "H-Net + CALM combined: tokenizer-free with continuous vectors"
status: untested
source: ["H-Net paper (Hwang, Wang, Gu, 2025)", "CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0005, PT0006]
---

Use H-Net's dynamic chunking to determine WHERE to split text into chunks (variable K, content-aware boundaries), then CALM's autoencoder to compress each chunk into a continuous vector. This combines the best of both: learned boundaries + continuous representation.

H-Net provides: tokenizer-free byte-level input, natural morphological boundary detection for Latin, speculative-decoding-like inference efficiency.

CALM provides: continuous latent space for the main network to operate in, 4x sequence compression, energy-based single-step generation head.

For eigenflower: the combined system would process raw Latin/English bytes, discover natural chunking boundaries (Latin morphological units), compress each chunk to a continuous vector, then reason over the vector sequence. No tokenizer, no vocabulary decisions, natural handling of Latin inflections.

Caveat: novel combination, untested. The variable-length chunks from H-Net may not align with CALM's fixed-K assumption. Would need to either fix K at a level compatible with H-Net's learned boundaries, or adapt CALM for variable-length inputs.
