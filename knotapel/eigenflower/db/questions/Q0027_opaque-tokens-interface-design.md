---
id: Q0027
title: "How do opaque tokens work as a fluency-syllogism interface mechanism?"
status: open
source: "CLAUDE.md"
related: []
---

The eigenflower CLAUDE.md mentions "opaque tokens (from knotapel vision.md)" as the most concrete proposal for the interface: "tokens flow through compiled weights, net reasons about logical structure not content, substitution at output." This implies the fluency layer produces placeholder tokens that encode propositional structure without content, the syllogism layer operates on these structural tokens to perform reasoning, and then content is substituted back in at the output stage. The question is: what exactly are these opaque tokens? How are they produced from natural language input? How does the syllogism layer know what reasoning operations to perform on them? And how is content substituted back without losing logical coherence? This needs a concrete design specification before it can be implemented.
