---
id: Q0026
title: "Can Koopman operators provide a continuous-time formulation of DKC's depth law?"
status: open
source: "notes/01-analysis.md"
related: []
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The crossover analysis identifies Koopman operators as one of four things knotapel gains from Group D papers. The Koopman framework lifts nonlinear dynamics into a linear (but infinite-dimensional) operator acting on observables. DKC's depth law (max_xor ~ depth + 6) describes a discrete relationship between algebraic depth and computational capability. A Koopman formulation could: (1) explain WHY the depth law is linear, (2) predict what happens at intermediate "depths" (non-integer, interpolated), (3) connect to spectral theory (Koopman eigenvalues = dynamical modes). The question is whether the Koopman lifting is well-defined for DKC's discrete algebraic structures or whether it only applies to continuous dynamical systems.
