---
id: LIM0023
title: "DKC activation selects Boolean output; theological reasoning selects from conclusion space"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0013, LIM0014]
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

DKC's activation function selects a Boolean output — true or false, one of a small finite set. Theological reasoning selects from a space of possible conclusions that is much larger and structured differently.

From 01-analysis.md: "DKC's 'activation' selects a Boolean output. Theological reasoning selects from a space of possible conclusions."

In DKC, the activation is a k-sector partition that maps cyclotomic values to Boolean outputs. The output space is tiny and finite. In theological reasoning, the "output" is a proposition — potentially novel, potentially complex, potentially expressible in many equivalent ways.

This difference matters because:
- DKC's proofs about what functions are computable (parity-lock theorem, depth law) depend on the finite, enumerable output space.
- Theological conclusions live in a much richer space where enumeration is impractical.
- The step function H in Domingos's tensor logic (SP[s,p] = H(SM[s,m] * MP[m,p])) produces a Boolean — "this predication holds" or "it doesn't." This is actually fine for individual syllogistic conclusions, but the AGGREGATION across many syllogisms produces a conclusion space that is combinatorially large.

Implications:
- The syllogism layer's individual operations (single syllogisms) can use Boolean output. The complexity is in the graph traversal that chains many syllogisms together.
- Forward chaining through a large syllogism graph produces a deductive closure whose size is bounded by the number of propositions, not by 2^n Boolean outputs.
- The activation problem may be less severe than it first appears — Boolean is the right granularity for individual logical steps, even if the aggregate reasoning is complex.
