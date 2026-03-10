---
id: CON0022
title: "NVFP4's 16 representable values and DKC's optimal 21-entry catalog: finite vocabularies suffice"
connects:
  - "NVFP4 E2M1: only 16 distinct values per block, yet billion-param models train to FP8 accuracy"
  - "DKC optimal z8 catalog: 21 entries (not 24) from binary octahedral group (D96, D50)"
type: structural_parallel
strength: strong
source: "extractions/group-c-knotapel-relevance.md, Connection 3"
---

Both demonstrate the same phenomenon from opposite directions. NVFP4 shows projecting continuous weights onto 16 values preserves training convergence. DKC shows starting with a finite catalog (24-100 values) suffices for universal Boolean computation.

The key insight: "resolution" needed for computation is determined by algebraic structure, not numerical precision.

NVFP4's 16 values are ad-hoc (hardware efficiency). DKC's 24 values are the vertices of the 24-cell, a mathematically optimal polytope (self-dual, densest lattice packing in 4D). The 24-cell Voronoi partition outperforms geographic grids (D66: 35 solutions from 25 cells vs 34 from 64 cells).

The non-monotonic parity solutions (906@k=6 > 756@k=7 > 96@k=8, D50) show more representable values is NOT always better -- it depends on commensurability between value set and algebraic structure. This parallels NVFP4's finding that stochastic rounding helps gradients but HURTS weights.

D96 (LANDMARK): removing 3 Q8-null entries from the 24-entry catalog IMPROVES capacity. More entries can mean more noise -- the "natural" vocabulary is smaller than the algebraic maximum.
