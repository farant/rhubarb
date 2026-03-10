---
id: G0005
title: "Compile syllogisms to ANN weights"
status: open
priority: high
depends_on: [G0004]
blocks: [G0007, G0016]
related: []
---

## Description

Develop the compilation pipeline that takes a syllogism expressed in logical form and produces ANN weights that compute it — using tensor logic (exact, no training loop). This is the forward direction: logic to weights.

The GPTQ-as-CVP insight from the Group C papers gives this a concrete mathematical framing: compiling a syllogism to weights is literally a Closest Vector Problem on a Hessian-defined lattice, with known algorithms and provable error bounds. This transforms a vague aspiration into a concrete mathematical optimization.

## Why It Matters

This is the explicit core goal stated in CLAUDE.md. If this works, the syllogism layer is not a trained black box — it is a compiled, transparent, exact system where every weight has a known logical meaning. This is the fundamental departure from mainstream AI: reasoning by construction rather than by approximation.

The compilation mechanism also enables dynamic recompilation (G0016) — adding new syllogisms at runtime without retraining.

## What "Achieved" Looks Like

- Given a syllogism in logical form, the system produces weight matrices
- The produced weights correctly compute the syllogism when executed through the tensor logic interpreter
- Compilation is closed-form (no iterative optimization) — instant, not trained
- Error bounds are known and exact (or the computation is exact with zero error)
- The process works for all 24 valid syllogistic forms
- Weights can be inspected and their logical meaning understood (transparency)
