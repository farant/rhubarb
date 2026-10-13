---
id: Q0025
title: "Is Krylov complexity the correct measure for DKC reservoir computational power?"
status: open
source: "notes/01-analysis.md"
related: []
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The crossover analysis lists Krylov complexity as one of four things knotapel gains from Group D papers. Krylov complexity measures the effective dimensionality explored by iterating an operator — how many truly independent directions the dynamics visits. For DKC reservoirs, this could be a more principled measure than ad-hoc metrics like "max XOR computable." The question is whether Krylov complexity correlates with (and perhaps subsumes) existing DKC measures like the depth law's max_xor ~ depth + 6, or whether it captures something different. Computing Krylov complexity for several DKC algebras and comparing against known computational capability would answer this.
