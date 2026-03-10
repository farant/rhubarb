---
id: REF0026
title: "Aquinas's Taxonomy of Reasoning Modes"
type: corpus
source_project: external
summary: "Five modes of reasoning distinguished by Aquinas: demonstration (syllogistic), analogy (proportionality), argument from authority (weighted citation), reductio ad absurdum (backward chaining with negation), and distinguo (namespace resolution). Each has distinct logical structure amenable to different architectural components."
related: []
---

## Description

Aquinas himself distinguishes modes of reasoning with specific structures, each of which maps to a different architectural component in Eigenflower:

1. **Demonstration** -- syllogistic. A is to B, B is to C, therefore A is to C. This is the tensor logic layer. Transparent by construction. Barbara, Celarent, etc. compile to matrix multiplications + step functions.

2. **Analogy** -- works by proportionality. A is to B as C is to D. This is a structural mapping between domains with its own logic (can't conclude identity from analogy, only similarity of relation).

3. **Argument from authority** -- has specific weight depending on source. Scripture is highest, then Church Fathers, then philosophers. Essentially a weighted citation graph.

4. **Reductio ad absurdum** -- show that denying the conclusion produces a contradiction. This is backward chaining through the syllogism graph with negation.

5. **Distinguo** -- distinguishing senses of a term. "X is true in sense A, false in sense B." This is namespace resolution -- the same word maps to different proposition nodes depending on sense.

## Relevance to Eigenflower

- Provides the architectural decomposition for Eigenflower's reasoning layer: each mode could have its own transparent component
- Demonstration is the easy case -- mathematical clarity already exists (tensor logic)
- Analogy requires proportionality circuits that span both the fluency and syllogism layers
- The taxonomy determines what fraction of the Summa's reasoning is fully formalizable vs requiring fuzzy components
- If only 30% is formal syllogisms, the graph is sparse and the fluency layer does heavy lifting for the other 70%
