---
id: Q0022
title: "What is the computational cost of forward chaining to compute deductive closure over a large syllogism graph?"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

When a new syllogism is added to the graph, forward chaining recomputes the deductive closure — all derivable conclusions. For a small graph (50-100 syllogisms), this is trivial. But the Summa has 3,000+ articles, each potentially contributing multiple syllogisms. The graph could grow to tens of thousands of nodes. Forward chaining on a graph this size could be expensive, especially if adding one syllogism triggers a cascade of new derivations. The question is whether incremental update strategies exist that avoid recomputing the full closure — only propagating the new consequences of the added syllogism. This is a standard problem in deductive databases (Datalog evaluation), so known solutions may apply.
