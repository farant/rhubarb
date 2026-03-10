---
id: PT0025
title: "Two-stage training: logical dynamics first, then content"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)", "MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0026, PT0027]
---

Train in two stages: first on logical transition dynamics (what follows from what), then on actual theological content in context.

Stage 1 (World Model / IWM): train on (premises, inference-rule) -> (conclusion) triples and their incorrect variants. This teaches the model logical entailment patterns. 1 epoch on rollout data.

Stage 2 (Expert Data / SFT): fine-tune on actual Summa syllogisms in full context. Remaining epochs.

Optional Stage 3 (RL): use syllogism validity checker as reward signal.

The two-stage approach from MicroDiT is analogous: Phase 1 at lower resolution/quality, Phase 2 at higher resolution/quality, reusing Phase 1 weights. For text: short context -> long context, or logical structure -> full text.

The Early Experience paper shows this is dramatically more data-efficient: 1/8 expert data with two-stage training matches full imitation learning on the complete dataset.

Same total compute budget as single-stage training, but better results because the model develops structural understanding before content knowledge.
