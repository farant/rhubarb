---
id: PT0055
title: "Self-reflection training for syllogistic reasoning principles"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)"]
applicable_to: []
related: [PT0026, PT0027]
---

Train the model to generate chain-of-thought explanations of WHY valid syllogisms are valid and invalid ones are invalid, grounded in actual logical consequences. The model learns decision principles rather than rote action patterns.

Adapted self-reflection template for syllogistic reasoning:
1. State the premises
2. Compare possible conclusions and why each invalid one fails
3. Justify the valid conclusion grounded in rules of inference
4. Highlight specific logical principles at work (undistributed middle, illicit process, etc.)

L_SR = -sum log p_theta(explanation, correct_conclusion | premises).

Critical: reflections must be GROUNDED in observed consequences (what actually follows or what contradiction results), not hallucinated explanations. Ungrounded rationales (STaR-style) DEGRADE performance.

SR is better than IWM for reasoning-heavy tasks. Non-monotonic with branching factor: K=2-4 alternatives is optimal.

For eigenflower: teaches the model WHY logical moves are correct, not just WHICH moves to make. This understanding should generalize to new syllogisms better than pattern matching.
