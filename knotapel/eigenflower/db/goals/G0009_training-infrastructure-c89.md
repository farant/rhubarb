---
id: G0009
title: "Build neural network training infrastructure in C89"
status: open
priority: high
depends_on: []
blocks: [G0001, G0010]
related: []
---

## Description

Implement the core training infrastructure for neural networks in C89 from scratch — no external dependencies. This includes:

- Matrix multiplication (optimized for ternary weights / BitNet)
- Backpropagation
- Optimizer (AdamW or similar)
- Data loading for the Bible + Summa corpus
- Loss computation
- Gradient accumulation
- Checkpointing (save/resume training)

All of this must be written in C89 following rhubarb conventions (Latin identifiers, no dependencies, strict compiler flags).

## Why It Matters

This is the prerequisite for the fluency layer. Without training infrastructure, there is no trained model. The decision to build this from scratch (rather than using PyTorch/JAX) is consistent with the rhubarb philosophy: own your tools, no supply chain risk, no dependency rot.

The ternary weight constraint (BitNet b1.58) actually simplifies the implementation in some ways — multiplication by {-1, 0, +1} is addition, negation, or skip. This means the hot inner loop of training and inference can avoid floating-point multiply entirely, which is a significant performance advantage on M2.

Building from scratch also means total transparency into the training process, which aligns with the project's commitment to understanding every component.

## What "Achieved" Looks Like

- Matrix multiply benchmarked on M2 — operations/second established
- Backpropagation verified against known gradients
- Optimizer produces convergent training on a toy problem
- Data loader handles the full corpus efficiently
- Training can be checkpointed and resumed
- All code compiles under strict C89 flags
- Performance profiled and bottlenecks identified
