---
id: PT0046
title: "Deep supervision with detached gradients for virtual depth"
status: untested
source: ["TRM paper (Jolicoeur-Martineau, 2025)"]
applicable_to: []
related: [PT0003]
---

Reuse latent features across supervision steps WITHOUT gradients to emulate very deep networks. The model is supervised at each step (up to N_sup=16), with early stopping via learned halting mechanism.

With T=3 supervision steps, n=6 recursions, 2 layers: effective depth = T*(n+1)*n_layers = 3*(6+1)*2 = 42 layers. The model is tiny (7M params) but deep in execution.

Training: for each supervision step, run T-1 recursion processes without gradients (detached), then 1 with gradients. The detached steps provide initialization context without requiring backpropagation through the full depth.

Full backprop through all n+1 recursions within the gradient-enabled step is best (partial backprop hurts). The deep supervision mechanism lets you train effectively deep models without the memory cost of deep backpropagation.

For eigenflower: enables a tiny model to have reasoning depth comparable to much larger models. Each recursion step could apply syllogistic rules, with 42 effective layers of reasoning from 2 actual layers.

The halting mechanism is simplified to Binary Cross-Entropy loss (no Q-learning needed). Train with halting, infer with all N_sup=16 steps.
