---
id: Q0025
title: "Is Krylov complexity the correct measure for DKC reservoir computational power?"
status: open
source: "notes/01-analysis.md"
related: []
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The crossover analysis lists Krylov complexity as one of four things knotapel gains from Group D papers. Krylov complexity measures the effective dimensionality explored by iterating an operator — how many truly independent directions the dynamics visits. For DKC reservoirs, this could be a more principled measure than ad-hoc metrics like "max XOR computable." The question is whether Krylov complexity correlates with (and perhaps subsumes) existing DKC measures like the depth law's max_xor ~ depth + 6, or whether it captures something different. Computing Krylov complexity for several DKC algebras and comparing against known computational capability would answer this.
