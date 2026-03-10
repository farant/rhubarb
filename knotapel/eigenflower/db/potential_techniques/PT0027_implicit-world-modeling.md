---
id: PT0027
title: "Implicit World Modeling for logical transition dynamics"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)"]
applicable_to: []
related: [PT0025, PT0026]
---

Train the model to predict next-states from (state, action) pairs, teaching it environment transition dynamics implicitly. The training objective is L_IWM = -sum log p_theta(s'|s,a) -- standard next-token prediction on state-transition data.

For eigenflower: train on (premises, inference-rule) -> (conclusion) prediction. Given premises and a logical rule, predict what follows. This IS syllogistic reasoning as a next-token prediction task. The model learns logical dynamics without explicit reward.

Uses the SAME model parameters for both world modeling and action prediction. The model's own exploratory actions serve as supervision signals without any reward function.

Data amplification: for each expert state, sample K alternative actions, execute them, observe consequences. This multiplies training data by ~K. For eigenflower: each correct derivation step generates K incorrect alternatives with their actual logical consequences.

Key result: IWM with 1/8 expert data surpasses full imitation learning. The augmented data is more valuable than raw expert demonstrations because it explores the consequence space.

Works at 3B model scale with batch size 16, lr 1e-5, 2 epochs. Promising for MacBook training.
