---
id: CON0046
title: "Paper 2 branching factor K parallels DKC BFS catalog amplification"
connects:
  - "Early Experience: branching factor K multiplies training data, SR non-monotonic at K=2-4"
  - "DKC BFS catalog growth: ~(n-1)x per round, coherence beats diversity (D82, D96)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 7"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Paper 2's branching factor K multiplies training data by ~K per expert state. DKC's BFS closure multiplies the "action space" exponentially: ~(n-1) new entries per existing entry per round (D99-D103 confirmed as braid group invariant).

The depth law max_xor ~ depth + 6 is DKC's version of "more alternatives = more computational power." But DKC discovered non-trivial structure: deep entries (all from depth 8) outperform strided entries (maximum diversity from all depths) at matched catalog size (D82). Deep entries share algebraic coherence from common BFS ancestry.

Paper 2's finding that SR is non-monotonic in K (best at K=2-4) may be the same phenomenon: too many unrelated alternatives dilute signal, while structured expansion preserves coherence.

D96's catalog pruning (21 beats 24) shows structured subset selection outperforms exhaustive inclusion. D91's "balanced exponentials" (vocabulary ~2x/round, parity demand 4x/weight) quantifies when amplification helps versus when it saturates.
