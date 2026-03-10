---
id: KIT0017
title: "BrierLM likelihood-free evaluation metric"
status: planned
tier: 0
input: "2 samples from generative model, target value"
output: "Unbiased Brier score estimate"
depends_on: []
effort: trivial
source: ["CALM paper (Shao et al., 2025)"]
migrated_from: PT0049
applicable_to: [G0001, G0002]
related: []
---

## Description

Unbiased estimator of the Brier score using only 2 samples: Brier(P,y) ~ I{x1=y} + I{x2=y} - I{x1=x2}. Extended to n-grams (Brier-n) and combined as geometric mean (BrierLM). Correlates nearly perfectly with cross-entropy loss (Pearson -0.966, Spearman -0.991).

Works for ANY implicit generative model, not just those with tractable likelihoods. Important for evaluating models using energy-based heads, diffusion, or other non-standard generation methods.

For eigenflower: enables evaluation of the continuous-vector generation model (CALM-style) or any experimental generation approach without requiring log-likelihood computation.

## Implementation

Draw 2 samples, check equality with target, compute average. Extremely simple.

```c
duplex brier_aestimare(constans character *x1, constans character *x2, constans character *y);
duplex brierlm_aestimare(constans character **x1s, constans character **x2s,
                         constans character **ys, longus n, longus max_ngram);
```

~50 LOC. Trivial but valuable.
