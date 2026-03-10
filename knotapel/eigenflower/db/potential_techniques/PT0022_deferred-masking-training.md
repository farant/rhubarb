---
id: PT0022
title: "Deferred masking with patch-mixer for efficient training"
status: untested
source: ["MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0023, PT0024]
---

Instead of naively dropping input tokens/patches (which degrades quality), first process ALL tokens through a lightweight "patch-mixer" (4 transformer blocks, <10% of backbone parameters), THEN mask 75% before feeding to the main backbone. The unmasked tokens retain global context despite most being dropped.

At 75% masking, deferred masking achieves FID 5.03 vs naive masking's 16.5. Training cost reduces by 4x (proportional to masking ratio). The patch-mixer is training-only overhead -- not used at inference.

For eigenflower text: the concept transfers directly. Process all tokens through a lightweight mixer (2-4 transformer layers), then mask 50-75% for the main backbone. This dramatically reduces training cost on Bible/Summa corpus.

Random masking > block masking (better coverage across training). Combined with low-precision training (2-3x speedup), total cost reduction could be 8-12x.

Unmasked finetuning after masked pretraining (short phase) recovers any remaining performance gap.

Key numbers: MicroDiT 1.16B sparse model trained for $1,890 (2.6 days on 8xH100), competitive with Stable Diffusion 1.5 ($160K equivalent). For eigenflower's smaller model, this compression is what makes 30-60 minute training feasible.
