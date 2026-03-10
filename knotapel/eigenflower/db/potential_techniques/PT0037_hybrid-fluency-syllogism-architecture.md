---
id: PT0037
title: "Hybrid fluency-syllogism two-layer architecture"
status: untested
source: ["Architecture Insight notes"]
applicable_to: []
related: [PT0009, PT0012, PT0036]
---

Split eigenflower into two coupled systems:

1. Fluency ANN (~50-100M params, ternary weights, trained on Bible+Summa) -- handles parsing, understanding, generation. Statistical, tolerates approximation.

2. Syllogism ANN -- structured graph of compiled logical relationships. Tensor logic engine with exact algebraic operators. Not a neural network in the traditional sense.

The split solves each system's problems:
- Fluency solves EXTRACTION (parsing Latin prose into logical structure)
- Fluency solves GENERATION (producing text from logical conclusions)
- Syllogism solves REASONING (valid inference with exact arithmetic)

Dynamic recompilation becomes natural: parse new syllogism with fluency layer, compile into syllogism layer (new tensor equation), recompute deductive closure. Fluency layer doesn't need retraining unless VOCABULARY changes.

The "walk" metaphor: fluency parses input -> formulates query -> syllogism layer walks graph -> returns derivation chain -> fluency narrates. The fluency layer navigates, the syllogism graph is the territory.

Build order: Phase 1 (syllogism layer, self-contained, testable), Phase 2 (fluency layer, requires training infrastructure), Phase 3 (interface, novel research).
