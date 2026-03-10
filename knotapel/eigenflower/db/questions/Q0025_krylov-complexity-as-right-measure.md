---
id: Q0025
title: "Is Krylov complexity the correct measure for DKC reservoir computational power?"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis lists Krylov complexity as one of four things knotapel gains from Group D papers. Krylov complexity measures the effective dimensionality explored by iterating an operator — how many truly independent directions the dynamics visits. For DKC reservoirs, this could be a more principled measure than ad-hoc metrics like "max XOR computable." The question is whether Krylov complexity correlates with (and perhaps subsumes) existing DKC measures like the depth law's max_xor ~ depth + 6, or whether it captures something different. Computing Krylov complexity for several DKC algebras and comparing against known computational capability would answer this.
