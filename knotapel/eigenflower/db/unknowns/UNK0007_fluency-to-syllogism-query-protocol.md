---
id: UNK0007
title: "What the fluency-to-syllogism query protocol should look like"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The architecture describes the fluency layer producing "structured queries" to the syllogism layer: proposition nodes, syllogism structures, graph queries. But the actual protocol is undesigned. What data format do queries take? Are they symbolic (proposition IDs, logical connectives) or vectorial (embeddings that the syllogism layer must decode)? How does the fluency layer know what queries to formulate — is there a fixed query vocabulary or can it compose novel queries? How does error handling work when the fluency layer misparses input into an invalid query? This is described as "probably the most novel and hardest design problem" in the architecture document, and it remains entirely open.
