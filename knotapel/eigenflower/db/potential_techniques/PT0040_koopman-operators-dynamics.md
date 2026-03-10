---
id: PT0040
title: "Koopman operators for modeling reasoning dynamics"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

Use Koopman operators -- linear operators on function spaces that capture nonlinear dynamics -- for modeling the dynamics of reasoning chains. Trained via Extended Dynamic Mode Decomposition (EDMD). Enable long-term prediction, spectral interpretability, eigendecomposition for mode analysis.

Koopman operators outperform RNNs and LSTMs with fewer parameters for dynamical system prediction.

For eigenflower: model the progression of a syllogistic argument as a dynamical system. Each reasoning step (applying an inference rule) is a state transition. The Koopman operator captures the linearized dynamics of these transitions. Eigendecomposition reveals the "modes" of reasoning -- which patterns of inference dominate.

The knotapel crossover identifies Koopman operators as relevant to the "depth law" -- understanding how recursive application of reasoning operators builds computational power.

Implementation: EDMD requires computing matrix products and eigendecompositions. Available via Apple Accelerate LAPACK routines.
