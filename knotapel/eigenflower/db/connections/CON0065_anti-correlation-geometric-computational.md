---
id: CON0065
title: "Anti-correlation between geometric quality and computational quality"
connects:
  - "Orchid conditioning kernel design for uniform frequency coverage"
  - "DKC anti-correlation: t=0 spherical design with 36 solutions vs t=1 with 4 (D72)"
type: cross_pollination
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 3 (detail)"
---

D72 establishes that the algebraically-derived 13-direction Voronoi is geometrically the WORST possible configuration (t-design quality = 0), yet it maximizes XOR6 solutions (36 vs 4 for the geometrically best t=1 design).

This anti-correlation warns against optimizing Orchid's conditioning kernel for uniform frequency coverage. The algebraically "worst" distribution may be computationally best.

The general principle: computational power comes from algebraic structure (group orbits, Galois action, nesting), not geometric optimality (uniform distribution, low discrepancy, high t-design order). Optimizing for geometric quality actively destroys the algebraic correlations that enable computation.

This has broad implications for any architecture where kernel or attention patterns are designed: naive geometric optimization (uniform coverage, balanced attention) may be counterproductive for hard computational tasks. DKC provides a concrete, quantified example of exactly how much it costs (36 solutions -> 4, an 89% reduction).
