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

> **Correction (2026-10-10, Demo 124):** D84's "null indispensability" at ζ₈ does not hold exactly. Removing the nulls keeps max XOR8, null-only computes nothing, and nulls are under-represented among winners; the recorded figures are not reproduced by D84's own code on any build tried (cause unknown). D87's "regime transition" therefore has no ζ₈ side. Statements here that rely on either are superseded; see `knotapel/demo_124_exact_null_states/findings.md`.

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Paper 2 demonstrated more data is not always better -- structured augmentation of less data beats brute-force more. DKC discovered the same in multiple forms:

1. Pruning helps (D96): 21 entries > 24 because the 3 removed Q8-null entries contribute noise without directional coverage.
2. Minimal sufficiency (D97): 6 entries (3 orthogonal directions at 45-degree half-angle) achieve 100% capacity.
3. Null indispensability is regime-dependent (D84 vs D87): at finite groups (z8) nulls are indispensable; at infinite groups (z12) they are redundant.

This maps to Paper 2's finding that data efficiency depends on environment structure: structured environments (DKC's exact algebraic world) need less data than stochastic environments.

The pattern is consistent: algebraic structure determines how much data/catalog you need, and excess entries actively interfere with the algebraic coherence required for hard computation.
