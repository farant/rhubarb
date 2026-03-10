---
id: CON0051
title: "SciencePedia narration over compiled knowledge = DKC activation as readout over pre-computed weights"
connects:
  - "SciencePedia Plato agent: 'narration' over pre-verified chains, not unconstrained generation"
  - "DKC activation function as readout selecting from pre-computed bracket catalog (D50, D77, D93)"
type: structural_parallel
strength: moderate
source: "extractions/group-e-knotapel-relevance.md, Connection 12"
---

Paper 1's "narration over pre-verified chains" and DKC's "activation over pre-computed weights" are the same pattern: constrained selection from a verified knowledge base rather than unconstrained generation.

The bracket catalog contains no errors (exact arithmetic). The only failure mode is choosing the wrong readout:
- Split-sigmoid reads only magnitude -> 12/13 NPN classes, parity impossible (D48)
- MVN reads phase -> complementary correlation structure (D47)
- k-sector at k=6 reads angular structure at sufficient resolution -> all 13 classes (D50)
- phase_cell reads full S^1 x S^2 -> circuit complexity hierarchy visible (D93)

Each activation is a "narration strategy" over the same compiled knowledge. Quality depends entirely on which narration strategy, not the knowledge base itself.

D77's resolution of XOR8 wall (0 solutions -> 6 solutions, same catalog, different activation) is the clearest demonstration: switching narration strategy unlocks capability that was always present.

Paper 1's insight that narration reduces hallucination corresponds to: the catalog is error-free, so the only failure is wrong readout selection.
