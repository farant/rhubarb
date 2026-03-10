---
id: Q0012
title: "Can the syllogism layer handle degrees of certainty (demonstration vs probable vs authority)?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

Aquinas distinguishes between demonstrations (certain), probable arguments, and arguments from authority. A pure Boolean tensor logic only handles the first — propositions are true or false, conclusions follow necessarily or not at all. But theological reasoning regularly operates with probable arguments and weighted authority. The question is whether the tensor logic framework can be extended to handle certainty grades without losing its exactness guarantees. Options: (1) separate subgraphs per certainty level, (2) weighted edges where weights encode certainty, (3) a richer algebraic structure than Boolean that can encode degrees. Each has different implications for the interface and for what queries the system can answer.
