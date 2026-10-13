---
id: PT0040
title: "Koopman operators for modeling reasoning dynamics"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Use Koopman operators -- linear operators on function spaces that capture nonlinear dynamics -- for modeling the dynamics of reasoning chains. Trained via Extended Dynamic Mode Decomposition (EDMD). Enable long-term prediction, spectral interpretability, eigendecomposition for mode analysis.

Koopman operators outperform RNNs and LSTMs with fewer parameters for dynamical system prediction.

For eigenflower: model the progression of a syllogistic argument as a dynamical system. Each reasoning step (applying an inference rule) is a state transition. The Koopman operator captures the linearized dynamics of these transitions. Eigendecomposition reveals the "modes" of reasoning -- which patterns of inference dominate.

The knotapel crossover identifies Koopman operators as relevant to the "depth law" -- understanding how recursive application of reasoning operators builds computational power.

Implementation: EDMD requires computing matrix products and eigendecompositions. Available via Apple Accelerate LAPACK routines.
