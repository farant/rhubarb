---
id: PT0035
title: "Narration over compiled syllogisms (not unconstrained generation)"
status: untested
source: ["Inverse Knowledge Search paper (Li et al., 2026)", "Architecture Insight notes"]
applicable_to: []
related: [PT0034]
---

Shift the model's inference task from "generation" to "narration" -- weaving pre-verified reasoning chains rather than generating claims from scratch. The Plato Agent pattern: retrieve relevant compiled syllogisms, then narrate/combine them into coherent text.

This dramatically reduces hallucination because the model is constrained to combine pre-verified logical structures. The fluency layer handles HOW to say it; the syllogism layer provides WHAT to say.

In the hybrid architecture: fluency parses input -> formulates query to syllogism graph -> syllogism layer walks the graph -> returns derivation chain -> fluency narrates the chain into natural language.

The fluency layer is a translator between human language and logical structure. It doesn't need to reason (syllogism layer does that) or store theological knowledge (syllogism graph does that). It needs to parse well and generate coherently.

Closer to template-constrained generation than free generation -- the logical structure constrains content, fluency handles expression.
