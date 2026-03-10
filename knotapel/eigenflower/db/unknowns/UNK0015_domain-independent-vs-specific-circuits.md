---
id: UNK0015
title: "How to distinguish domain-independent wisdom circuits from domain-specific ones at the weight level"
status: open
source: "notes/03-transparency-and-wisdom-circuits.md"
related: []
---

The wisdom circuits concept requires distinguishing circuits that handle abstract patterns (domain-independent) from those that handle specific content (domain-specific). In a trained model's weights, this distinction may not be clear-cut. A circuit that handles "parent-child" relationships might be partially domain-independent (the abstract asymmetric dependency) and partially domain-specific (specific to family/genealogy vocabulary). Decomposing weights on cyclotomic lattices might reveal the abstract component, but how do you separate it from the domain-specific component? This is related to the broader problem of disentanglement in learned representations, which is an active research area with no clean solutions.
