---
id: Q0026
title: "Can Koopman operators provide a continuous-time formulation of DKC's depth law?"
status: open
source: "notes/01-analysis.md"
related: []
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The crossover analysis identifies Koopman operators as one of four things knotapel gains from Group D papers. The Koopman framework lifts nonlinear dynamics into a linear (but infinite-dimensional) operator acting on observables. DKC's depth law (max_xor ~ depth + 6) describes a discrete relationship between algebraic depth and computational capability. A Koopman formulation could: (1) explain WHY the depth law is linear, (2) predict what happens at intermediate "depths" (non-integer, interpolated), (3) connect to spectral theory (Koopman eigenvalues = dynamical modes). The question is whether the Koopman lifting is well-defined for DKC's discrete algebraic structures or whether it only applies to continuous dynamical systems.
