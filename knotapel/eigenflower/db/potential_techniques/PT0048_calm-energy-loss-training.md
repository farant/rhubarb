---
id: PT0048
title: "Energy score loss for likelihood-free syllogism training"
status: untested
source: ["CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0006]
---

The energy score is a strictly proper scoring rule that is likelihood-free and sample-based: S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha]. First term encourages diversity (penalizes collapsed predictions), second encourages accuracy.

Trained with N=8 model samples and M=100 target samples from the posterior. Alpha=1.0 is optimal. This works for ANY implicit generative model -- you don't need to compute exact probabilities, just generate samples and compare to targets.

For eigenflower: could train a model to produce valid syllogisms without computing exact token likelihoods. Generate sample conclusions, compare to correct conclusions via energy score. This could work for training a model on syllogistic reasoning where the output space is structured (valid vs invalid) rather than a flat token distribution.

The BrierLM evaluation metric (unbiased estimator using only 2 samples) provides a universal evaluation tool for any implicit generative model.

Caveat: alpha < 1 causes gradient explosion. Alpha=2 causes score to be 0 (not strictly proper). Stay in [1, 1.75].
