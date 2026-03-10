---
id: CON0017
title: "CALM K (tokens per vector) as scaling axis parallels DKC sector count k"
connects:
  - "CALM K as new scaling axis orthogonal to model size"
  - "DKC sector count k as scaling axis for NPN class reachability (D50, D63, D65)"
type: structural_parallel
strength: strong
source: "extractions/group-b-knotapel-relevance.md, Connection 9"
---

CALM shows K (tokens per vector) is a new scaling axis: K=1 to K=4 gives dramatic efficiency gains, K=8 degrades quality at current model sizes.

DKC's sector count k is an exactly analogous axis:
- k=2: 5/13 NPN classes
- k=3: +1 class (6/13)
- k=4: +6 classes (12/13)
- k=6: +1 = parity (13/13)
- k=8: solution count DROPS (906 -> 96) -- non-monotonic

The non-monotonicity (k=6 better than k=8 for 3-input parity) parallels CALM's finding that K=8 degrades quality. Both systems have a sweet spot determined by interaction between compression granularity and underlying algebraic structure. For DKC, the sweet spot is at k=6 because gcd(6,8)=2 creates the right incommensurability with the Z[zeta_8] lattice.

This connection is quantitative: both systems show the same non-monotonic scaling behavior with the same structural explanation (mismatch between granularity and domain structure).
