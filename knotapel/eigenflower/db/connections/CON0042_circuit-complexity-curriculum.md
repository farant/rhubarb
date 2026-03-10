---
id: CON0042
title: "DKC circuit complexity hierarchy IS a natural difficulty curriculum"
connects:
  - "SciencePedia curriculum scaffolding: ~200 courses x ~200 topics for systematic coverage"
  - "DKC circuit complexity hierarchy: AND/XOR ratio explosion and four-tier NPN hierarchy (D50, D93)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 3"
---

The 13 NPN equivalence classes of 3-input Boolean functions form a natural curriculum with intrinsic difficulty ordering. D93 revealed AND/XOR ratio explosion: 1.01 (N=3) to 2762 (N=7) to infinity (N=8).

The four-tier hierarchy (D50) provides curriculum scaffolding: k=2 (5 NPN classes, easy), k=3 (+1), k=4 (+6), k=6 (+1 = parity, full). This is the DKC version of Paper 1's "200 courses": at each tier, a specific subset of computational capabilities becomes reachable.

AC^0 functions (AND, OR) are "introductory courses," TC^0 (MAJ) are "intermediate," and functions outside AC^0 (parity/XOR) are "advanced." The same truth table can be "derived" at different difficulty levels depending on activation, encoding, and catalog depth -- exactly Paper 1's multi-level derivation from the same endpoint.

The hierarchy is universal across group structures (z8, 2I, z12, D94), making it a genuine invariant of the curriculum, not an artifact of one algebraic setting.
