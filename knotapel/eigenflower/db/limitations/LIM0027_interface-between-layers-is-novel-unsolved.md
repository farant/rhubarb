---
id: LIM0027
title: "The interface between fluency and syllogism layers is a novel unsolved problem"
type: discovered
source: "notes/02-architecture-insight.md"
impacts: []
related: [LIM0026, LIM0015, LIM0025]
---

The interface between the fluency ANN and the syllogism ANN — how the fluency layer queries the syllogism graph, how it narrates results back — is identified as "probably the most novel and hardest design problem" in the architecture.

From 02-architecture-insight.md: "How does the fluency layer query the syllogism graph? How does it narrate results back? This is probably the most novel and hardest design problem."

The interface has two directions:
1. **Fluency -> Syllogism (parsing/query)**: The fluency layer reads text and produces structured queries. "This sentence asserts that all X are Y" becomes a proposition node. "What does compiled knowledge say about X?" becomes a graph query.
2. **Syllogism -> Fluency (narration)**: The syllogism layer produces logical states ("from P1 and P2, C follows by Barbara") and the fluency layer must narrate these into coherent text.

The "walk" metaphor: the fluency layer doesn't reason — it navigates. The syllogism graph is the territory. The fluency layer is the guide who can read the map and describe the landscape in natural language.

The most concrete proposal is "opaque tokens" (from knotapel vision.md): tokens flow through compiled weights, the net reasons about logical structure not content, and substitution happens at output.

Implications:
- This is Phase 3 of the build order — deliberately deferred until both the syllogism layer (Phase 1) and fluency layer (Phase 2) are working independently.
- The interface protocol — how propositions are represented at the boundary — is a critical design decision that affects both layers.
- Template-constrained generation (the narration direction) may be simpler than fully free generation, since the logical structure constrains what needs to be said.
- This is where eigenflower is most likely to produce genuinely novel research contributions.
