---
id: G0001
title: "Train in 30-60 minutes on M2 laptop"
status: open
priority: high
depends_on: [G0009, G0010]
blocks: []
related: []
---

## Description

The complete training pipeline for the fluency ANN must complete in 30-60 minutes on an Apple M2 MacBook. This is a hard constraint that shapes every architectural decision downstream — model size, precision, optimizer choice, data loading strategy.

## Why It Matters

This is the forcing function that makes eigenflower fundamentally different from mainstream AI development. If training takes days on a GPU cluster, you are locked into the existing paradigm. If it takes 30-60 minutes on a laptop, you can iterate rapidly, experiment freely, and maintain complete independence from cloud infrastructure. It also means the entire system can be rebuilt from scratch at any time — no dependence on pre-trained checkpoints.

The Group C paper extractions establish this is achievable: 50-100M params is the sweet spot, with 75% token masking and low-precision (ternary) training providing the efficiency margin.

## What "Achieved" Looks Like

- Full training run on the Bible + Summa corpus completes in under 60 minutes on M2
- The resulting model produces coherent Latin/English text
- Training can be re-run from scratch without special hardware
- No GPU required — M2's unified memory and Neural Engine are sufficient
