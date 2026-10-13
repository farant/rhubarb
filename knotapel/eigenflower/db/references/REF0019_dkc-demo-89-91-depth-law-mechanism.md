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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

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
