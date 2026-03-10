---
id: CON0007
title: "TRM EMA stabilization explained by DKC two-role direction theorem"
connects:
  - "TRM EMA (decay=0.999) for training stability"
  - "DKC two-role direction theorem: anchor vs scaffolding (D88)"
type: technique_transfer
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 7"
---

EMA in TRM prevents "sharp collapse" (87.4% with EMA vs 79.9% without on Sudoku). DKC's two-role direction theorem (D88) explains WHY this works at a structural level.

4 "rigid anchor" directions (body-diagonals) suffer 8% XOR loss under 10-degree perturbation. 6 "flexible scaffolding" directions (edge-midpoints) show slight GAIN under perturbation. K-ladder activation provides 14x better perturbation resilience than Voronoi-only.

EMA protects the anchors (preventing sharp weight moves in critical directions) while allowing gradual adjustment of the scaffolding. The 14x resilience from k-ladder activation shows that activation-level fragility is at least as important as weight-level stability.

Technique transfer: implement position-dependent EMA decay in TRM -- high decay (0.999+) for weight dimensions corresponding to "anchor" directions, lower decay (0.99) for "scaffolding" directions. DKC's constrained optimization approach (nulls free, non-nulls clamped to 2 degrees) provides the template.
