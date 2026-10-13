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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Demo 72 discovered a striking anti-correlation: the 13 eigenvector directions form a t=0 spherical design (geometrically poor distribution) yet achieve maximal XOR6 computation (36 solutions). Optimizing the directions toward better geometric distribution (higher spherical design order) DESTROYS computational power. This establishes that the "right" activation is algebraically motivated, not geometrically motivated.

## Relevance to Eigenflower

- Warns against optimizing conditioning kernels (Orchid-style) for uniform frequency coverage -- the algebraically "worst" distribution may be computationally best
- The activation function's value comes from algebraic structure, not geometric regularity
- Relevant to Eigenflower's activation/readout design: optimize for algebraic coherence, not geometric smoothness
