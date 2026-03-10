---
id: PT0063
title: "Block-diagonal matrices replacing MLP layers"
status: untested
source: ["Orchid paper (Karami & Ghodsi, NeurIPS 2024)"]
applicable_to: []
related: [PT0002]
---

Replace dense MLP matrices with block-diagonal matrices (b=4 blocks). This reduces MLP parameter count significantly while maintaining or improving performance. In Orchid BERT experiments, this combined with data-dependent convolution gave 30% parameter reduction with better GLUE scores.

For eigenflower: if using a transformer-based fluency layer, block-diagonal MLPs reduce parameter count where it matters most (MLP layers are typically 2/3 of transformer parameters). The block structure could also be interpretable -- different blocks might specialize for different linguistic features.

Implementation: straightforward -- instead of a full d x 4d matrix, use 4 separate (d/4 x d) matrices. No architectural novelty beyond dimension slicing.

Combines with ternary weights (PT0001): block-diagonal ternary MLPs would be extremely efficient -- sparse (block structure) + ternary (integer-only arithmetic).
