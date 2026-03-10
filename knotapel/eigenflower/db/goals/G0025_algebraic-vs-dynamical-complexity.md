---
id: G0025
title: "Publish algebraic vs dynamical complexity for reservoir computing"
status: open
priority: low
depends_on: []
blocks: []
related: []
---

## Description

Write up and publish the finding that algebraic complexity (derived series depth, solvability) predicts computational capability in reservoir computing, while dynamical complexity (topological entropy) is completely orthogonal. D106 from knotapel falsifies the naive application of the Edge of Chaos thesis (Paper 3 from Group D) to algebraic reservoirs.

This refines the Edge of Chaos thesis: the correct predictor of reservoir computational power is not dynamical chaos but algebraic non-solvability. This is identified in 01-analysis.md as one of four publishable observations.

## Why It Matters

This is a publishable result that is already substantively completed in the knotapel demo series. D106 provides the empirical evidence, and the theoretical framework (derived series depth as the correct complexity measure) is established across multiple DKC demos.

For eigenflower specifically, this result informs architectural decisions about the fluency layer: if the model uses recursive/reservoir-like computation (TRM style), the algebraic structure of the recurrence matters more than whether it is "at the edge of chaos." This is a concrete, actionable insight for model design.

The result also connects to the solvability bottleneck prediction from Group A crossover: DEQ (Deep Equilibrium) models should fail because fixed-point convergence implies solvable dynamics, and solvable groups cannot compute parity by Barrington's theorem. This is a testable prediction.

## What "Achieved" Looks Like

- Paper drafted with D106 evidence and theoretical framework
- Clear statement of the refined thesis (algebraic not dynamical complexity)
- Comparison with standard Edge of Chaos literature
- The solvability bottleneck prediction stated as a testable corollary
- Submitted to an appropriate venue (reservoir computing, dynamical systems, or algebraic ML)
