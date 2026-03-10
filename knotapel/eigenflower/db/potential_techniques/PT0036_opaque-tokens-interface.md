---
id: PT0036
title: "Opaque tokens for fluency-syllogism interface"
status: untested
source: ["Architecture Insight notes"]
applicable_to: []
related: [PT0035, PT0037]
---

Use opaque tokens as the interface protocol between the fluency layer and the syllogism layer. The fluency layer produces structured representations (proposition nodes, syllogism structures, graph queries) that the syllogism layer consumes, and vice versa.

The interface is the hardest novel design problem in the hybrid architecture. Two directions:

Fluency -> Syllogism (Parsing/Query):
- "This sentence asserts that all X are Y" -> proposition node
- "This passage argues from premises A and B to conclusion C" -> syllogism structure
- "What does compiled knowledge say about X?" -> graph query

Syllogism -> Fluency (Narration):
- "From premises P1, P2, conclusion C follows by Barbara" -> text template
- "Query Q has no derivation" -> negative response
- "Premises P1, P3 are contradictory" -> error response

The representation at the boundary could be: Boolean tensors (Domingos), embedding vectors (Hilbert space), or domain-specific Thomistic theology tokens. Open question requiring experimentation.
