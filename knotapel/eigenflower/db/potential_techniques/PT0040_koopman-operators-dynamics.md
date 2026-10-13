---
id: PT0040
title: "Koopman operators for modeling reasoning dynamics"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Use Koopman operators -- linear operators on function spaces that capture nonlinear dynamics -- for modeling the dynamics of reasoning chains. Trained via Extended Dynamic Mode Decomposition (EDMD). Enable long-term prediction, spectral interpretability, eigendecomposition for mode analysis.

Koopman operators outperform RNNs and LSTMs with fewer parameters for dynamical system prediction.

For eigenflower: model the progression of a syllogistic argument as a dynamical system. Each reasoning step (applying an inference rule) is a state transition. The Koopman operator captures the linearized dynamics of these transitions. Eigendecomposition reveals the "modes" of reasoning -- which patterns of inference dominate.

The knotapel crossover identifies Koopman operators as relevant to the "depth law" -- understanding how recursive application of reasoning operators builds computational power.

Implementation: EDMD requires computing matrix products and eigendecompositions. Available via Apple Accelerate LAPACK routines.
