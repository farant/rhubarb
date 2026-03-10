---
id: G0007
title: "Build syllogism graph infrastructure"
status: open
priority: high
depends_on: [G0004, G0005, G0006]
blocks: [G0012, G0013, G0014]
related: []
---

## Description

Build the graph infrastructure for the syllogism layer: proposition nodes connected by logical relationships (syllogism edges), with forward chaining for deductive closure and backward chaining for query answering. The graph functions as an internal database the fluency layer can walk.

Key capabilities:
- Proposition nodes with unique identifiers
- Syllogism edges linking premises to conclusions (with the syllogistic form identified)
- Forward chaining: compute the full deductive closure (all derivable conclusions from a set of premises)
- Backward chaining: given a query proposition, find derivation chains that reach it
- Incremental update: adding a new syllogism does not require recomputing the entire closure

## Why It Matters

This is what makes the syllogism layer a SYSTEM rather than a collection of individual equations. Individual tensor equations compute individual syllogisms; the graph connects them into a reasoning network where chains of inference produce conclusions no single syllogism could reach.

The incremental update requirement is critical for dynamic recompilation (G0016) — you need to add a new syllogism and efficiently propagate its consequences through the existing graph without starting from scratch.

## What "Achieved" Looks Like

- Graph data structure in C89 with proposition nodes and syllogism edges
- Forward chaining correctly computes deductive closure for test cases
- Backward chaining correctly finds derivation chains for query propositions
- Incremental addition of syllogisms propagates consequences correctly
- Performance is adequate for the scale of the Summa (~3000 articles, potentially thousands of propositions)
- The graph can be serialized and restored (persistence)
