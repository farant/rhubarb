---
id: PT0034
title: "Inverse knowledge search over syllogism graph"
status: untested
source: ["Inverse Knowledge Search paper (Li et al., 2026)"]
applicable_to: []
related: [PT0033]
---

Given a target concept, retrieve all reasoning chains that touch that concept. This inverts typical search (which finds conclusions matching keywords) to find reasoning pathways that lead to or through a concept.

For eigenflower: given a theological concept, find all syllogisms in the compiled knowledge that use it as a premise, conclusion, or middle term. This reveals the logical dependency graph of theology and surfaces non-obvious connections.

The Plato Agent pattern applies: inference should be "narration over compiled syllogisms" not "generation from compressed weights." The model combines and selects from compiled knowledge, not confabulating. This dramatically reduces hallucination (~50% reduction vs baseline LLM in the paper).

The knowledge graph emerges from content: the syllogism dependency graph forms naturally from which conclusions serve as premises elsewhere. Use community detection (MODBP or simpler) to discover hierarchical structure.

Recursive expansion: once the model can reason about syllogisms, it could potentially derive NEW syllogisms not explicitly in the training corpus, using compiled rules of inference.
