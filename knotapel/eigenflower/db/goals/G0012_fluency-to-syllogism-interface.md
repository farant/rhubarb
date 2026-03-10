---
id: G0012
title: "Build fluency-to-syllogism query interface"
status: open
priority: high
depends_on: [G0007, G0010]
blocks: [G0014]
related: []
---

## Description

Design and implement the protocol by which the fluency layer queries the syllogism graph. This is the "channeling in" mechanism — how domain-specific natural language input gets abstracted into logical form that the syllogism layer can process.

The interface must handle:
- Proposition assertion: "This sentence asserts that all X are Y" -> proposition node
- Argument structure: "This passage argues from premises A, B to conclusion C" -> syllogism structure
- Query formulation: "What does the compiled knowledge say about X?" -> graph query
- Pattern matching: identify which wisdom circuit(s) (G0018) apply to the input

The key design question: what is the representation for propositions at the boundary? Options include Domingos's Boolean tensors, embedding vectors, or something domain-specific to Thomistic theology.

## Why It Matters

This is the hard/novel problem identified in the architecture document. The fluency layer and syllogism layer are only useful together if there is a clean, efficient, and correct interface between them. A bad interface bottlenecks the entire system regardless of how good the individual layers are.

The opaque tokens proposal from knotapel's vision.md is the most concrete idea: tokens flow through compiled weights, the net reasons about logical structure not content, substitution happens at output. This needs to be tested and refined.

## What "Achieved" Looks Like

- A defined protocol (data structures, function signatures) for fluency -> syllogism communication
- The fluency layer can formulate queries that the syllogism graph can execute
- Propositions are represented at the boundary in a way that is both computationally efficient and logically precise
- The interface handles the common cases in the Summa (syllogistic arguments, queries about theological propositions)
- Latency is low enough that the interface does not bottleneck inference
