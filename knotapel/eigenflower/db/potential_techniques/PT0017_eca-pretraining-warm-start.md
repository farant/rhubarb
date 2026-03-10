---
id: PT0017
title: "Elementary Cellular Automata pretraining as warm start"
status: untested
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)"]
applicable_to: []
related: [PT0015, PT0016]
---

Pretrain a tiny transformer on binary spatiotemporal data from Wolfram Class IV Elementary Cellular Automata rules (e.g., Rule 110, which is Turing-complete) before fine-tuning on the actual text corpus. The ECA pretraining develops rich internal representations that transfer to reasoning tasks.

Architecture modification: replace token embeddings with linear projection layer mapping binary vectors into embedding space. Output is also linear projection back to binary. No vocabulary, no softmax.

For eigenflower: use ECA pretraining to give the fluency layer a "warm start" with rich internal representations before training on Bible/Summa. The pretrained representations provide the substrate for syllogism compilation.

ECA data generation is trivial in C89: each rule is an 8-bit lookup table, each step is a single pass over the array. Hyperparameters: lr=2e-6, weight_decay=0.01, batch_size=64, seq_len=60, spatial_dim=100.

The 67k model architecture: 1 layer, 1 attention head, 64-dim embeddings. Runs trivially on any hardware.

Reservoir computing connection: the ECA-pretrained transformer acts like a reservoir -- internal representations are fixed, only I/O layers adapt. This suggests dynamic recompilation could focus on outer layers only.
