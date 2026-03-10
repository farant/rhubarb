---
id: PT0026
title: "Contrastive training with invalid syllogism variants"
status: untested
source: ["Early Experience paper (Zhang et al., 2025)"]
applicable_to: []
related: [PT0025, PT0027]
---

For each valid syllogism in the training corpus, generate K=3-4 invalid variants: wrong middle term, wrong figure, illicit major/minor, undistributed middle, etc. Train the model to distinguish valid from invalid by observing the logical consequences of each variant.

Two modes:
1. IWM (Implicit World Modeling): train on (premises, rule, variant-conclusion) triples showing what WOULD follow under different logical moves. L_IWM = -sum log p_theta(s'|s,a).
2. Self-Reflection (SR): generate chain-of-thought explanations of WHY valid syllogisms are valid and invalid ones are invalid, grounded in specific logical errors. L_SR = -sum log p_theta(c, a_expert | s).

Critical warning from the paper: ungrounded rationales (STaR-style, generated without executing alternatives) actually DEGRADE performance. Rationales must be grounded in actual logical consequences. "If you used this invalid form, HERE is the contradiction that results" >> "this form is invalid because [hallucinated reason]."

IWM learns environment dynamics (what happens); SR learns decision principles (why to choose). Both needed: IWM for mechanical validity checking, SR for understanding semantic content.

Branching factor K=2-4 is optimal for self-reflection (non-monotonic -- too many alternatives degrades quality).
