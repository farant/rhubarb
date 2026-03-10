---
id: G0014
title: "Build end-to-end walk: text in, reasoned text out"
status: open
priority: high
depends_on: [G0012, G0013]
blocks: []
related: []
---

## Description

Implement the complete end-to-end reasoning pipeline — the "walk":

1. Fluency layer parses input text: "Is it true that X?"
2. Fluency layer formulates query to syllogism layer: find_derivation(X)
3. Syllogism layer walks the graph: X <- P1, P2 by Barbara; P1 <- P3, P4 by Celarent...
4. Syllogism layer returns the derivation chain (or "no derivation found")
5. Fluency layer narrates the chain: "X follows from P1 and P2. P1 in turn follows from..."

This is the integration point where all three architectural components (fluency layer, syllogism layer, interface) work together as a unified system.

## Why It Matters

This is the moment eigenflower becomes a SYSTEM rather than a collection of components. The individual pieces — tensor logic, kernel operators, the fluency model, the graph infrastructure — are all means to this end. The walk is what the user actually interacts with.

The "walk" metaphor is precise: the fluency layer does not reason — it NAVIGATES. The syllogism graph is the territory. The fluency layer is a guide who can read the map and describe the landscape in natural language. The reasoning happens in the graph; the language happens in the fluency layer; the walk coordinates them.

## What "Achieved" Looks Like

- A user can pose a theological question in Latin or English
- The system parses it, queries the syllogism graph, and returns a reasoned answer
- The answer includes the derivation chain (verifiable step-by-step reasoning)
- When no derivation exists, the system says so rather than confabulating
- The end-to-end latency is acceptable (within the 70 tok/s output budget)
- Multiple queries can be asked in sequence, building on previous context
- Demo with at least 10 questions from across the Summa's subject matter
