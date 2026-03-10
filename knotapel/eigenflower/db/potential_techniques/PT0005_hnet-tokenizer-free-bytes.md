---
id: PT0005
title: "H-Net tokenizer-free byte-level processing for Latin morphology"
status: untested
source: ["H-Net paper (Hwang, Wang, Gu, 2025)"]
applicable_to: []
related: [PT0006, PT0007]
---

Replace the entire tokenization pipeline with a single end-to-end hierarchical network operating directly on raw bytes. A learned dynamic chunking mechanism replaces handcrafted tokenizers like BPE. Architecture is U-Net-like: small SSM encoder networks compress raw bytes into chunks, a large main network processes the compressed representation, SSM decoder networks decompress back to byte resolution.

Dynamic chunking via cosine similarity routing: boundary probabilities computed by cosine similarity between adjacent encoder outputs through learned projections. Low similarity = boundary. Completely differentiable. A smoothing module (exponential moving average) makes discrete boundaries differentiable for end-to-end training.

For eigenflower: eliminates tokenizer dependency for Latin. No vocabulary design decisions, no unknown tokens, natural handling of Latin inflections and multilingual text. The model learns its own segmentation. Demonstrated 3.6x data efficiency on DNA (specialized domain), which is relevant for small corpus. Excellent on non-English languages and code.

Can be nested recursively (bytes -> sub-words -> phrases). SSM layers for encoder/decoder are very efficient. Ratio loss controls compression ratio. Content-aware chunking naturally rediscovers word boundaries.

DNA models tested at 29-66M params with d=512 -- within M2 range.
