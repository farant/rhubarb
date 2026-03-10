---
id: LIM0001
title: "Training time limited to 30-60 minutes on M2"
type: chosen
source: "CLAUDE.md, hardware/time constraint"
impacts: []
related: [LIM0002, LIM0003, LIM0010]
---

The fluency ANN must compile and train within approximately 30-60 minutes on an Apple M2 laptop. This is a hard forcing function that shapes every architectural decision downstream.

This constraint exists because the development cycle needs to be fast enough for a single person to iterate. If training takes hours, you lose the ability to experiment rapidly. The 30-60 minute window means you can start a training run, do something else, and come back to evaluate results within the span of a focused work session.

Implications:
- Model size is capped around 50-100M parameters (Group C analysis confirms this is the sweet spot)
- Training must use efficient techniques: 75% token masking, low-precision training, ternary weights
- Rules out any architecture that requires multi-GPU or multi-day training
- Kernel ridge regression for the syllogism layer is attractive precisely because it's closed-form (no training loop at all)
- The corpus size constraint (LIM0004) is partially motivated by this — smaller corpus = fewer training steps
