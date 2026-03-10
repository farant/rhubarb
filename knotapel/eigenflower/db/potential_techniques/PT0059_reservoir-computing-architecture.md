---
id: PT0059
title: "Reservoir computing architecture (fixed reservoir + trained readout)"
status: untested
source: ["Intelligence at the Edge of Chaos paper (Zhang et al., ICLR 2025)", "Knotapel crossover analysis"]
applicable_to: []
related: [PT0015, PT0017]
---

Use a fixed, pretrained network as a "reservoir" that projects inputs into high-dimensional space, then train only a readout layer on top. The ECA-pretrained transformers already exhibit this property: internal representations are fixed, only I/O layers adapt.

DKC from knotapel already implements this pattern: fixed catalog (reservoir) + trained readout (activation function). The five-pillar synthesis (TL algebra + Hilbert space operators + edge of chaos + tensor logic + reservoir computing) provides theoretical justification.

For eigenflower: pretrain the core model on complex data (Bible/Summa or even ECA rules), freeze the internal layers, then train only input/output projections for specific tasks. This dramatically reduces the number of trainable parameters and speeds up training.

Dynamic recompilation could focus on the readout layer only, making syllogism compilation a lightweight operation on top of a fixed computational substrate.

The knotapel crossover identifies this as "DEEPEST" connection: DKC provides the theoretical framework for why a ternary recursive model should work as a reservoir, via the depth law and parity-lock theorem.
