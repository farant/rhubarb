---
id: PT0052
title: "SSM-based encoder/decoder for hierarchical compression"
status: untested
source: ["H-Net paper (Hwang, Wang, Gu, 2025)"]
applicable_to: []
related: [PT0005]
---

Use State Space Models (Mamba-2) for the encoder and decoder in the hierarchical architecture. SSMs dramatically outperform Transformers for the encoder/decoder role because they have a natural "compression bias" -- they compress information into fixed-size states, aligning with the chunking task.

Even on BPE-tokenized inputs, SSM encoders/decoders outperform Transformer ones. A hybrid M3T1 (3 Mamba + 1 Transformer) encoder architecture slightly outperforms pure Mamba M4.

Monotone width hierarchy: model width increases with depth (D^0 <= D^1 <= ... <= D^S). Inner/deeper stages get wider models since they process more compressed (more semantic) representations.

For eigenflower: SSM layers are very efficient, particularly on Apple Silicon due to sequential nature matching unified memory's strengths. Small SSM stacks (4 layers each) for encoder/decoder add minimal parameter overhead while providing effective compression.

Dimension expansion via shared trainable vector: append a shared vector to all tokens when going from D^s to D^{s+1}. Very simple, no learned projection needed at the interface.
