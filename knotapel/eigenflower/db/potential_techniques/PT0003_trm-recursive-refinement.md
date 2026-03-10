---
id: PT0003
title: "TRM recursive refinement architecture"
status: untested
source: ["TRM paper (Jolicoeur-Martineau, 2025)"]
applicable_to: []
related: [PT0001, PT0004]
---

A single tiny network (2 layers, ~5-7M parameters) reasons by recursively refining its answer. Given input x, the model maintains a proposed solution y and a latent reasoning feature z. It recursively updates z given (x, y, z), then updates y given (y, z). Deep supervision: latent features from each step are detached and used as initialization for the next step.

Key results: 7M params achieves 45% on ARC-AGI-1, beating Deepseek R1 (671B), Claude 3.7, o3-mini. 2 layers beats 4 layers (less is more with scarce data). Single network beats two separate networks. EMA (0.999) is critical for stability.

For eigenflower: the recursive refinement IS syllogistic reasoning -- each recursion step applies the same logical rules to refine the answer. z accumulates the reasoning trace, y is the current conclusion. Optimal: T=3, n=6 gives 42 effective depth. MLP variant works for fixed-size tasks; attention needed for variable-size.

At 7M params with BitNet ternary weights = ~1.4 MB model. Training: AdamW, lr=1e-4, batch 768, hidden 512. EMA=0.999. Sudoku trains in <36 hours on single L40S with 60K epochs.

Caveat: supervised model producing deterministic answers, not generative. Extension to language generation is explicitly flagged as open problem. MoE and weight tying both hurt. Deep Equilibrium Models hurt (fixed-point convergence is harmful).
