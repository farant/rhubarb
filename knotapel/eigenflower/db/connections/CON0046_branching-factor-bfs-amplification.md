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

Paper 2's branching factor K multiplies training data by ~K per expert state. DKC's BFS closure multiplies the "action space" exponentially: ~(n-1) new entries per existing entry per round (D99-D103 confirmed as braid group invariant).

The depth law max_xor ~ depth + 6 is DKC's version of "more alternatives = more computational power." But DKC discovered non-trivial structure: deep entries (all from depth 8) outperform strided entries (maximum diversity from all depths) at matched catalog size (D82). Deep entries share algebraic coherence from common BFS ancestry.

Paper 2's finding that SR is non-monotonic in K (best at K=2-4) may be the same phenomenon: too many unrelated alternatives dilute signal, while structured expansion preserves coherence.

D96's catalog pruning (21 beats 24) shows structured subset selection outperforms exhaustive inclusion. D91's "balanced exponentials" (vocabulary ~2x/round, parity demand 4x/weight) quantifies when amplification helps versus when it saturates.
