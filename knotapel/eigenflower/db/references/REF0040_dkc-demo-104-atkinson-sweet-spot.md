---
id: REF0040
title: "DKC Demo 104 — Atkinson Sweet Spot"
type: dkc_demo
source_project: knotapel
demo_number: 104
summary: "3-strand beats 4-strand at matched catalog size. Optimal component count ~120 for dim-9 modules. The sign-hash = 1-bit compressed sensing (Boufounos-Baraniuk 2008) expands effective rank."
related: []
---

## Description

Demo 104 discovered the Atkinson sweet spot: 3-strand braids outperform 4-strand at matched catalog size, with optimal performance at approximately 120 components for dimension-9 modules. The sign-hash technique (reducing Z[zeta_8] values to their sign pattern) was identified as 1-bit compressed sensing per Boufounos-Baraniuk (2008), and counter-intuitively expands effective rank (244 -> 292 for W_{6,2}).

## Relevance to Eigenflower

- Demonstrates that more strands (more algebraic complexity) is not always better -- there is a sweet spot
- The 1-bit compressed sensing connection provides theoretical underpinning for extreme quantization of DKC weights
- Relevant to Eigenflower's efficiency constraints: optimal reservoir size may be smaller than maximum available
