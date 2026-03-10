---
id: PT0049
title: "BrierLM likelihood-free evaluation metric"
status: promoted_to_kit
promoted_to: KIT0017
source: ["CALM paper (Shao et al., 2025)"]
applicable_to: []
related: [PT0048]
---

An unbiased estimator of the Brier score using only 2 samples from the model: Brier(P,y) ~ I{x1=y} + I{x2=y} - I{x1=x2}. Extended to n-grams (Brier-n) and combined as geometric mean (BrierLM). Correlates nearly perfectly with cross-entropy loss (Pearson -0.966, Spearman -0.991).

This is a universal evaluation tool that works for ANY implicit generative model, not just those with tractable likelihoods. Important for evaluating models using energy-based heads, diffusion, or other implicit generation methods.

For eigenflower: enables evaluation of the continuous-vector generation model (CALM-style) or any non-standard generation approach without requiring log-likelihood computation. Can evaluate black-box models too.

Implementation: extremely simple -- draw 2 samples, check equality with target, compute average. No special infrastructure needed.
