---
id: CON0053
title: "PUBLISHABLE: Algebraic complexity predicts reservoir quality where dynamical complexity fails"
connects:
  - "Edge-of-chaos thesis: dynamical complexity (Lyapunov, LZ) predicts representation quality"
  - "DKC D106: topological entropy orthogonal to computation; derived series depth is correct measure"
type: cross_pollination
strength: strong
source: "notes/01-analysis.md, Publishable Ideas #2"
---

D106 falsifies topological entropy (the Lyapunov analog for braids) as a predictor of DKC computation: both periodic and pseudo-Anosov braids compute XOR at 100% participation. The TL quotient at delta=0 erases ALL dynamical information while preserving algebraic structure.

The correct measures are algebraic/representation-theoretic:
- Derived series depth (D93)
- Solvability vs non-solvability (D94, Barrington's theorem)
- BFS catalog depth (D82)
- Galois orbit structure (D107)
- Product closure and v_2 connectivity (D108)

This refines the edge-of-chaos thesis: for algebraic reservoirs, relevant complexity is representation-theoretic, not dynamical. 109 demos provide evidence.

The solvability bottleneck (D94: 2I outperforms z8 at matched size, advantage GROWS with difficulty) IS Barrington's theorem applied to reservoirs -- the algebraic version of "Turing-complete dynamics produce best representations."

Krylov complexity may bridge the gap: it tracks operator growth rather than state-space divergence, closer to what DKC measures (BFS catalog growth).
