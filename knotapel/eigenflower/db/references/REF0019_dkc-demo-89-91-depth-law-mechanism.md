---
id: REF0019
title: "DKC Demos 89-91 — Depth Law Mechanism"
type: dkc_demo
source_project: knotapel
demo_number: 91
summary: "Depth law mechanism identified: axis cancellation + cross-depth algebraic constraint. The mechanism is relational, not positional. Balanced exponentials: supply ~2x/depth, demand 4x/weight, giving slope ~1. Activation is not the bottleneck; parity itself is the wall."
related: []
---

## Description

Demos 89-91 investigated the mechanism behind the depth law (max_xor ~ depth + 6). Eight hypotheses were tested; axis cancellation was confirmed as the mechanism. Deep entries contain anti-aligned quaternion rotation axes whose sums produce the low angles needed for parity separation.

Key findings:
- The mechanism is relational (cross-depth algebraic constraint from shared BFS ancestry), not positional
- Balanced exponentials explain the slope: vocabulary grows ~2x per depth round, parity demand grows ~4x per weight, yielding approximately linear slope
- D91 confirmed activation resolution (varying k_sec from 2 to 48) does not shift the depth gap -- finer resolution reveals more solutions but not higher arity
- Parity constraint itself is the wall, not the activation

## Relevance to Eigenflower

- The "8 hypotheses killed, 1 confirmed" process is a concrete example of self-reflection (Paper 2's SR) -- grounded comparison of alternatives
- The balanced exponentials formula could inform TRM's learning rate and recursion count scheduling
- The finding that parity is the wall (not activation) parallels the Eigenflower insight that reasoning correctness is the hard problem, not text generation fluency
