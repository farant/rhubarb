---
id: PT0043
title: "Mixture-of-Experts for sparse capacity increase"
status: untested
source: ["MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0024]
---

Replace every other feedforward layer with an 8-expert MoE layer using expert-choice routing (each expert picks its top tokens, no auxiliary load-balancing loss needed). MoE increases parameter count (model capacity) without proportionally increasing FLOPs since only 2/8 experts fire per token.

For eigenflower: a 200M dense model becomes a ~500M sparse model with the same inference cost. Each expert could specialize: some for Latin grammar, some for logical structure, some for theological vocabulary, etc.

Caveat: the TRM paper found that MoE HURTS when data is scarce (too much capacity = overfitting). MicroDiT found MoE helps at large scale (250K+ steps) but only slightly at small scale (60K steps). For eigenflower's small corpus, MoE may be counterproductive. Test carefully.

Expert-choice routing eliminates the need for auxiliary load-balancing losses, simplifying implementation.
