---
id: CON0006
title: "TRM 'less is more' principle formalized by DKC parity-lock theorem"
connects:
  - "TRM finding that MoE hurts generalization by excess capacity"
  - "DKC parity-lock theorem: constraint concentrates on hardest function (D92, D93)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 6"
---

TRM shows that reducing capacity improves generalization on hard tasks (MoE hurts, weight tying hurts, sweet spot is minimal). DKC's parity-lock theorem (D92, PROVED) gives the exact algebraic formalization.

The +/-q encoding creates 3^k equivalence classes where parity is constant on every class (0 conflicts) but AND has 1/27 class conflicts, OR has 1/27, and MAJ has 19/27. The extreme constraint does not merely "regularize" -- it structurally selects for parity by making all non-parity functions literally impossible. All AND/OR/MAJ produce ZERO winners at every depth.

This is stronger than TRM's empirical observation. DKC proves that constraint concentration works: constraining the encoding provably eliminates easy functions (AND/OR/MAJ, all in AC^0) and retains only the hardest function (parity, outside AC^0).

The circuit complexity hierarchy (D93) makes this quantitative: AND/XOR ratio goes from 1.01 at N=3 to 2762 at N=7 to infinity at N>=8. Without encoding constraint, a network would be drawn to exponentially more abundant easy solutions. TRM's observation that MoE hurts may be the neural network manifestation: excess capacity allows "cheating" by solving easy sub-problems.
