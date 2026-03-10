---
id: UNK0010
title: "How incremental update of deductive closure works when a new syllogism is added"
status: open
source: "notes/02-architecture-insight.md"
related: []
---

The dynamic recompilation section states that adding a new syllogism triggers "recompute deductive closure (forward chaining picks up new consequences)." But the cost and algorithm for this are unspecified. For a graph of N syllogisms with D derivation depth, full recomputation is potentially expensive (O(N * D) or worse). Incremental strategies — only propagating forward from the new syllogism's conclusions — could be much cheaper but need to handle cases where a new conclusion serves as a premise for an existing syllogism, creating a cascade. Whether existing techniques from deductive databases (semi-naive evaluation, Datalog) apply directly to tensor equation graphs is unclear.
