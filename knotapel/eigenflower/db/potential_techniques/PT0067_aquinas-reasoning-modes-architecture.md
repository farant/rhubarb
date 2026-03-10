---
id: PT0067
title: "Aquinas's reasoning modes as separate architectural components"
status: untested
source: ["Transparency and Wisdom Circuits notes"]
applicable_to: []
related: [PT0037, PT0038]
---

Aquinas distinguishes five modes of reasoning, each with specific structure that could map to a separate architectural component:

1. **Demonstration** -- syllogistic (A->B, B->C, therefore A->C). The tensor logic layer. Transparent by construction.

2. **Analogy** -- proportionality (A:B :: C:D). Structural mapping between domains. Has its own logic (can't conclude identity from analogy, only similarity of relation).

3. **Argument from authority** -- weighted citation graph. Scripture highest, then Church Fathers, then philosophers. Essentially a PageRank-like authority propagation.

4. **Reductio ad absurdum** -- backward chaining through syllogism graph with negation. Show that denying the conclusion produces contradiction.

5. **Distinguo** -- namespace resolution. "X is true in sense A, false in sense B." Same word maps to different proposition nodes depending on sense.

Each could have its own transparent-ish layer, rather than one opaque fuzzy layer for everything. The architecture doesn't need a single black-box -- it could have multiple interpretable components.

This is a design principle, not a specific technique. Implementation would require defining the mathematical structure for each reasoning mode.
