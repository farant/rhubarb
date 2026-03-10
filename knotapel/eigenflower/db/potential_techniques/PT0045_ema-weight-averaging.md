---
id: PT0045
title: "EMA weight averaging for training stability"
status: untested
source: ["TRM paper (Jolicoeur-Martineau, 2025)", "MicroDiT paper (Sehwag et al., 2024)"]
applicable_to: []
related: [PT0003]
---

Maintain an Exponential Moving Average of model weights during training. TRM uses decay=0.999, critical for preventing sharp collapse and improving generalization (79.9% to 87.4% on Sudoku-Extreme). MicroDiT uses EMA with 0.995 smoothing over last 10K steps.

For eigenflower: especially critical for small-data training where sharp parameter updates can cause catastrophic forgetting. The EMA model is used for evaluation/inference, while the live model continues training.

Implementation: maintain a shadow copy of all parameters. After each optimization step: ema_param = decay * ema_param + (1 - decay) * live_param. Doubles memory for parameters but parameter memory is typically small compared to activation memory.

TRM specifically found EMA critical -- without it, the recursive refinement model fails to generalize despite training well.
