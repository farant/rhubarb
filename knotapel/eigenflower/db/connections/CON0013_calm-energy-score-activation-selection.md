---
id: CON0013
title: "CALM energy score parallels DKC activation function selection problem"
connects:
  - "CALM energy-based generative head (strictly proper scoring rule)"
  - "DKC activation function selection: split-sigmoid -> MVN -> Voronoi -> k-ladder (D48-D91)"
type: structural_parallel
strength: moderate
source: "extractions/group-b-knotapel-relevance.md, Connection 3"
---

CALM's energy score S(P,y) = E[||x'-x''||^alpha] - 2E[||x-y||^alpha] balances diversity against accuracy, operating purely on samples with no density evaluation. DKC's activation selection problem is structurally identical: given a fixed Z[zeta_8] catalog, which activation best separates parity?

The progression: split-sigmoid (0/100M solutions), k-sector at k=6 (906 solutions), S^2 Voronoi (36 XOR6), S^1 x S^2 product (6 XOR8), k-ladder (14x perturbation resilience).

The energy score's strict propriety (uniquely minimized by the true distribution) has a DKC analog: the 13-direction Voronoi is the UNIQUE configuration maximizing XOR6 solutions while maintaining physical meaning (D72). The anti-correlation between geometric quality and computational quality (t=0 design with 36 solutions vs t=1 design with 4) shows the DKC "scoring rule" is algebraic, not geometric.

The k-ladder activation (D88, D91) is effectively an energy-based evaluation: it tries multiple resolutions {k=6, k=12, k=24} and takes the best, balancing coverage against precision.
