---
id: CON0050
title: "Paper 2 data efficiency (1/8 data beats full) mirrors DKC catalog pruning"
connects:
  - "Early Experience: 1/8 expert data matches full imitation learning on WebShop"
  - "DKC catalog pruning: 21 entries > 24 entries, 6 entries achieve 100% XOR (D96, D97)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 11"
---

Paper 2 demonstrated more data is not always better -- structured augmentation of less data beats brute-force more. DKC discovered the same in multiple forms:

1. Pruning helps (D96): 21 entries > 24 because the 3 removed Q8-null entries contribute noise without directional coverage.
2. Minimal sufficiency (D97): 6 entries (3 orthogonal directions at 45-degree half-angle) achieve 100% capacity.
3. Null indispensability is regime-dependent (D84 vs D87): at finite groups (z8) nulls are indispensable; at infinite groups (z12) they are redundant.

This maps to Paper 2's finding that data efficiency depends on environment structure: structured environments (DKC's exact algebraic world) need less data than stochastic environments.

The pattern is consistent: algebraic structure determines how much data/catalog you need, and excess entries actively interfere with the algebraic coherence required for hard computation.
