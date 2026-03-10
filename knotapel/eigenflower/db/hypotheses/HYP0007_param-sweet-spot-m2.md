---
id: HYP0007
title: "50-100M parameters is the sweet spot for training on M2 MacBook within 30-60 minutes"
status: proposed
source: "notes/01-analysis.md"
evidence_for: []
evidence_against: []
testable_by: []
related: []
---

## Claim

For the eigenflower project's constraints — training from scratch on an M2 MacBook in 30-60 minutes, with a corpus of Bible + Summa (~30MB), targeting 1-2GB RAM and ~70 tok/s inference — the sweet spot for model size is 50-100M parameters.

This is large enough to handle theological Latin and English at a competent level for the fluency layer's tasks (parsing, understanding, generation), but small enough to train within the time and hardware constraints.

## Evidence For

- **Group C extraction (efficiency envelope):** Identifies 50-100M params as the sweet spot based on the intersection of training time, hardware capabilities, and model capacity requirements. With 75% token masking + low-precision training, this range fits the 30-60 minute window.
- **TRM paper (Group A):** 7M params with recursive refinement beats billion-parameter models on reasoning tasks. This suggests that with the right architecture (recursion, ternary weights), even the low end of 50-100M may be overkill for reasoning — though the fluency layer needs more capacity for language.
- **BitNet b1.58:** Ternary weights reduce effective parameter count by ~10x compared to fp32, meaning a 100M ternary model stores and processes like a 10M fp32 model in terms of memory and compute.
- **Edge of Chaos paper (Group D):** 67k-parameter Tiny model (1 layer, 1 head, 64-dim) shows the effect. Validates that large models are not necessary.

## Evidence Against

- **Vocabulary inflation:** TRM's 7M works for grid puzzles with tiny vocabularies. Latin theological text has a much larger vocabulary (~30,000+ word forms due to Latin inflection), which inflates the embedding layer. A 50M model might spend most of its parameters on embeddings rather than computation.
- **No one in the literature has trained a 50-100M ternary model on medieval Latin from scratch.** The parameter count is interpolated from existing results, not demonstrated.
- **The "from scratch" constraint is harsh.** Most small model results benefit from pretrained embeddings or transfer learning. Eigenflower explicitly refuses off-the-shelf models.
- **H-Net tokenizer-free operation** could change the calculus — eliminating the embedding layer changes where parameters go, potentially shifting the sweet spot.
- **M2 specifics matter:** The exact training throughput depends on Metal GPU utilization, memory bandwidth, batch size, and many implementation details not yet determined.

## What Would Confirm It

- Actually training a 50-100M parameter ternary model on Bible + Summa on M2 in 30-60 minutes and showing it achieves competent Latin parsing and generation.
- Benchmarking training throughput on M2 with the actual C89 training infrastructure to get real numbers instead of estimates.
- Demonstrating that models below 50M lack sufficient capacity for Latin, and models above 100M exceed the training time budget.

## What Would Refute It

- Finding that 50M parameters is insufficient for competent Latin — that the vocabulary + grammatical complexity requires 200M+ even with ternary weights.
- Finding that M2 training throughput is lower than estimated, pushing the training time over 60 minutes even at 50M.
- Discovering an architecture (e.g., TRM-style recursion) that achieves the same capability at 10-20M, making 50-100M unnecessarily large.

## Implications If True

- The project has a concrete, achievable target for the fluency layer.
- The 1-2GB RAM and ~70 tok/s constraints are simultaneously achievable at this scale with ternary weights.
- Training infrastructure development in C89 has a clear target to optimize for.
- The model is small enough to understand fully — no black-box opacity from sheer scale.
