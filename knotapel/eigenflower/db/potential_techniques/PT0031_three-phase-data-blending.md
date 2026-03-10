---
id: PT0031
title: "Three-phase data blending for training"
status: untested
source: ["NVFP4 paper (NVIDIA, 2025)"]
applicable_to: []
related: [PT0025, PT0028]
---

Progressive quality increase across training phases:
- Phase 1 (70% of training): diverse, broad corpus
- Phase 2 (20%): higher-quality subset
- Phase 3 (10%): highest quality + synthetic/curated data

For eigenflower:
- Phase 1: full Bible (multilingual) + full Summa for general language patterns
- Phase 2: high-quality theological text only (core Summa articles, key Scripture)
- Phase 3: curated syllogism pairs + Q&A, explicit logical structures

Combined with Warmup-Stable-Decay learning rate schedule: constant LR for 80%, then decay to 1/100th over final 20%. Simpler than cosine schedules.

The progressive quality increase matches the two-stage training approach (PT0025) but adds a third refinement stage for the highest-quality material.
