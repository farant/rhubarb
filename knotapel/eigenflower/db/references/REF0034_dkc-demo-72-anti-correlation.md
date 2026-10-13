---
id: REF0034
title: "DKC Demo 72 — Spherical Design Anti-Correlation"
type: dkc_demo
source_project: knotapel
demo_number: 72
summary: "Anti-correlation between geometric distribution quality and computational power. t=0 spherical design with 36 XOR6 solutions; t=1 design (geometrically 'better') with only 4 solutions. The algebraically 'worst' distribution is computationally best."
related: []
---

## Description

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Demo 72 discovered a striking anti-correlation: the 13 eigenvector directions form a t=0 spherical design (geometrically poor distribution) yet achieve maximal XOR6 computation (36 solutions). Optimizing the directions toward better geometric distribution (higher spherical design order) DESTROYS computational power. This establishes that the "right" activation is algebraically motivated, not geometrically motivated.

## Relevance to Eigenflower

- Warns against optimizing conditioning kernels (Orchid-style) for uniform frequency coverage -- the algebraically "worst" distribution may be computationally best
- The activation function's value comes from algebraic structure, not geometric regularity
- Relevant to Eigenflower's activation/readout design: optimize for algebraic coherence, not geometric smoothness
