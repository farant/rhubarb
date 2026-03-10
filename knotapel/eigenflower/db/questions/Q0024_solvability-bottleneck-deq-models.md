---
id: Q0024
title: "Does the solvability bottleneck prediction hold — do DEQ models fail on parity because fixed-point convergence requires solvable dynamics?"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis makes a "testable and striking claim": DEQ (Deep Equilibrium) models fail because fixed-point convergence requires solvable dynamics, and solvable groups can't compute parity by Barrington's theorem. This predicts that any model architecture requiring convergence to a fixed point will be unable to compute parity-like functions. The claim connects DKC's parity-lock theorem to a concrete prediction about existing model architectures. An experiment: take a DEQ model, test it on parity tasks of increasing input size, and compare against a TRM-style recursive (non-convergent) model. If DEQ fails where TRM succeeds, the solvability bottleneck is empirically confirmed.
