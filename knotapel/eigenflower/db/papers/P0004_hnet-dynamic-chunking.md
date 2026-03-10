---
id: P0004
title: "H-Net: Dynamic Chunking for End-to-End Hierarchical Sequence Modeling"
authors: [Sukjun Hwang, Brandon Wang, Albert Gu]
year: 2025
arxiv_id: "2507.07955"
url: "https://github.com/goombalab/hnet"
extraction_group: B
relevance: high
key_techniques: [dynamic_chunking, tokenizer_free, hierarchical_unet, cosine_similarity_routing, ssm_encoder_decoder, recursive_nesting, smoothing_module]
summary: "Replaces BPE tokenization with a learned, content-aware hierarchical chunking system operating on raw bytes. U-Net architecture with SSM encoder/decoder. 2-stage H-Net matches BPE Transformer of 2x its size. 3.6x data efficiency on DNA."
related: []
---

## Key Contributions

H-Net replaces the entire tokenization-LM-detokenization pipeline with a single end-to-end hierarchical network operating directly on raw bytes. Dynamic chunking (DC) is a learned, content-aware, context-dependent segmentation mechanism that replaces handcrafted tokenizers. The architecture is U-Net-like with small encoder networks compressing raw bytes into chunks, a large main network processing compressed representation, and decoder networks decompressing back to byte resolution.

## Key Techniques

- **Tokenizer-free byte-level operation**: No vocabulary design, no unknown tokens, natural multilingual handling. Model learns its own segmentation.
- **Dynamic chunking via cosine similarity**: Boundary probabilities computed by cosine similarity between adjacent encoder outputs: p_t = 1/2(1 - q_t^T k_{t-1} / (||q_t|| ||k_{t-1}||)). Lightweight and fully differentiable.
- **Smoothing module for differentiable chunking**: EMA z_bar_t = P_t * z_hat_t + (1-P_t) * z_bar_{t-1} makes discrete boundaries differentiable. Enables end-to-end training.
- **Recursive nesting**: Main network M can itself be an H-Net, creating arbitrary depth hierarchies (characters -> morphemes -> words -> clauses).
- **SSM (Mamba-2) encoder/decoder**: State Space Models dramatically outperform Transformers for encoder/decoder role due to natural "compression bias."
- **Ratio loss**: Load-balancing loss guiding toward target compression ratio N. Prevents trivial solutions.
- **Straight-Through Estimator**: For discrete boundary decisions in continuous optimization.

## Specific Relevance to Eigenflower

- **Latin corpus**: No tokenizer needed for Latin -- dynamic chunking handles Latin morphological inflections naturally without specialized tokenizer design.
- **Data efficiency**: 3.6x data efficiency on DNA. For eigenflower's small Bible+Summa corpus, this is critical.
- **Speculative-decoding inference**: Small encoder runs on every byte, big network only on chunk boundaries (~1/6 of positions). Efficient on MacBook.
- **Emergent boundaries**: 1-stage rediscovers word boundaries. 2-stage groups multi-word phrases. Could discover syllogistic structure boundaries.

## Key Results

- 2-stage H-Net on bytes matches BPE Transformer of 2x its size
- 3.6x data efficiency on DNA (HG38 dataset)
- 1-stage rediscovers word boundaries; 2-stage groups phrases like "the backbone"
- Dramatically outperforms tokenized Transformers on corrupted text (AntSpeak, RandomCase, etc.)
- DNA models: 29M-66M params with d=512

## Key Equations

- Boundary probability: p_t = 1/2(1 - q_t^T k_{t-1} / (||q_t|| ||k_{t-1}||))
- Smoothing: z_bar_t = P_t * z_hat_t + (1 - P_t) * z_bar_{t-1}
- Ratio loss: L_ratio = N/(N-1) * ((N-1)FG + (1-F)(1-G)), alpha=0.03

## Implementation Notes

- Encoder/Decoder: 4 Mamba-2 layers each, d=512-1024
- Main network: 22-28 Transformer layers at d=1024-1536
- Training: AdamW, warmup-stable-decay schedule, batch 256, 8192 bytes/sequence
- Smallest viable config (DNA): d_model=512, 64M params total
- Dimension expansion via shared trainable vector (simple, no learned projection)
- Post-network RMSNorm critical for hierarchical stability
- Code: github.com/goombalab/hnet (PyTorch)
