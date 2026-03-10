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

Demo 72 discovered a striking anti-correlation: the 13 eigenvector directions form a t=0 spherical design (geometrically poor distribution) yet achieve maximal XOR6 computation (36 solutions). Optimizing the directions toward better geometric distribution (higher spherical design order) DESTROYS computational power. This establishes that the "right" activation is algebraically motivated, not geometrically motivated.

## Relevance to Eigenflower

- Warns against optimizing conditioning kernels (Orchid-style) for uniform frequency coverage -- the algebraically "worst" distribution may be computationally best
- The activation function's value comes from algebraic structure, not geometric regularity
- Relevant to Eigenflower's activation/readout design: optimize for algebraic coherence, not geometric smoothness
