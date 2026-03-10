---
id: CON0029
title: "NVFP4 mixed precision maps to DKC cross-ell arc (mixed algebraic precision)"
connects:
  - "NVFP4: first 2 and last 8 blocks in BF16, rest in FP4"
  - "DKC cross-ell arc: different cyclotomic rings of different ranks (D53-D59, D109)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 9"
---

NVFP4 assigns different numerical precision to different layers based on sensitivity. The cross-ell arc demonstrates an algebraic analog: different "algebraic precision" (cyclotomic ring rank) produces different computational capacities, but qualitative structure is universal.

The wall at 11/13 NPN classes under half-plane activation is the same at ell=4 (Z[zeta_8]), ell=5 (Z[zeta_5]), and ell=6 (Z[zeta_24]).

This suggests mixed-algebraic-precision: use low-rank rings (Z[i], rank 2) where qualitative structure suffices, high-rank rings (Z[zeta_16], rank 8) where finer resolution matters. D109: at delta=sqrt(2) (Z[zeta_16]), 9334 parity solutions exist with just Re>0 activation, vs 906 requiring k=6 sectors at delta=0 (Z[zeta_8]). The richer algebra compensates for simpler activation -- exactly the tradeoff NVFP4 exploits.
