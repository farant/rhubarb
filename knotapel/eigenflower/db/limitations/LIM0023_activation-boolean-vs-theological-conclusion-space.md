---
id: LIM0023
title: "DKC activation selects Boolean output; theological reasoning selects from conclusion space"
type: discovered
source: "notes/01-analysis.md"
impacts: []
related: [LIM0013, LIM0014]
---

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
