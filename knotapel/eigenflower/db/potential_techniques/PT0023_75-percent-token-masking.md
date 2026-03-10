---
id: PT0023
title: "75% token masking for efficient training"
status: untested
source: ["MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0022]
---

Mask 75% of input tokens/patches during training, reducing compute by 4x. The backbone transformer only processes 25% of the sequence. Masking is independent of model size (unlike model downscaling which reduces N), providing a clean 4x speedup.

For text: this is essentially what BERT-style masked language modeling does. Apply to eigenflower's Bible/Summa corpus. The model learns to predict masked tokens from context, which forces understanding of linguistic and logical structure.

Random token masking outperforms block/contiguous masking (better coverage of text regions across training batches).

When combined with deferred masking (PT0022), the quality degradation from high masking ratios is eliminated. Without deferred masking, 75% naive masking degrades quality significantly.

Feasibility estimate: 50-100M parameter model is the sweet spot for 30-60 minute training on M2 with 75% masking + low-precision training.
