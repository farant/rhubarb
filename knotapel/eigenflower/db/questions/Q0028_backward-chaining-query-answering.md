---
id: Q0028
title: "Can backward chaining through compiled tensor equations answer specific queries efficiently?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The architecture describes backward chaining as the mechanism for query answering: "does the compiled knowledge say anything about X?" requires recursing through tensor equations to find if X is derivable. Forward chaining computes the full deductive closure (all conclusions), but backward chaining is targeted — it only explores the subgraph relevant to the query. The question is whether backward chaining through tensor equations is efficient in practice. Standard backward chaining in logic programming (Prolog) has well-known performance characteristics and pathologies (infinite loops, redundant computation). Do these same issues arise with tensor equations, and do the same solutions (tabling, loop detection) apply? A prototype with 50-100 compiled syllogisms could benchmark this.
