---
id: Q0010
title: "What is the right representation for propositions at the fluency-syllogism interface?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The interface between the fluency layer and syllogism layer needs a shared representation for propositions. Three candidates are identified: (1) Domingos's Boolean tensors — propositions as truth-value assignments across a universe of discourse, (2) embedding vectors — propositions as points in a learned vector space, (3) something domain-specific to Thomistic theology. The choice has cascading consequences: Boolean tensors give exact reasoning but may not scale to the Summa's propositional complexity. Embedding vectors are flexible but introduce approximation into what should be an exact layer. A domain-specific representation might be best but requires designing from scratch with no prior art to guide it.
