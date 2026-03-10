---
id: REF0044
title: "Eigenflower Hybrid Architecture — Fluency + Syllogism Layers"
type: other
source_project: eigenflower
summary: "Eigenflower splits at the natural seam: a fluency ANN (~50-100M params, ternary, handles language) coupled to a syllogism ANN (tensor logic engine with compiled exact algebraic operators, handles reasoning). Each solves the other's problem. The fluency layer translates; the syllogism layer reasons."
related: []
---

## Description

The core architectural insight of Eigenflower: split the model at the seam between fluency (pattern matching, tolerates approximation) and reasoning (structural, requires correctness).

**Fluency layer**: ~50-100M params, ternary weights (BitNet b1.58), possibly recursive (TRM-style). Trained on Bible + Summa. Handles parsing, generation, grammar, style, context. Does NOT reason.

**Syllogism layer**: tensor logic engine with compiled exact operators. Graph of propositions connected by logical relationships. Each syllogism = tensor equation. Forward/backward chaining. Can be extended incrementally without retraining the fluency layer.

**The interface**: fluency parses input into structured queries; syllogism layer walks the graph and returns derivation chains; fluency narrates the results back as coherent text. The "walk" metaphor: fluency navigates, syllogism layer is the territory.

**Dynamic recompilation**: new syllogisms are parsed by fluency, compiled into syllogism layer as new tensor equations, and deductive closure is recomputed. Fluency layer only needs retraining if vocabulary changes, not when logical relationships are added.

## Relevance to Eigenflower

- This IS the Eigenflower architecture. Everything else supports or refines it.
- The build order: Phase 1 (syllogism layer, self-contained, testable), Phase 2 (fluency layer, requires training infrastructure), Phase 3 (the interface, novel research)
- DKC provides the exact arithmetic substrate for the syllogism layer
- The split eliminates the fundamental problem of LLMs: unreliable reasoning from pattern-matching
