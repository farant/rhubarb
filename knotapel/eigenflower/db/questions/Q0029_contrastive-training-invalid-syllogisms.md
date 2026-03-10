---
id: Q0029
title: "Does contrastive training with invalid syllogism variants improve the fluency layer's parsing accuracy?"
status: open
source: "notes/01-analysis.md"
related: []
---

The Early Experience paper recommends generating invalid syllogism variants as contrastive examples and training in two stages (logical dynamics first, then theological content). The question is whether this training recipe transfers to eigenflower's setting. Specifically: (1) can invalid syllogism variants be generated automatically from the Summa's valid syllogisms (e.g., by swapping premises, changing quantifiers, introducing equivocation)? (2) does contrastive training meaningfully improve the fluency layer's ability to distinguish valid from invalid arguments? (3) does the 1/8 data efficiency result hold for theological Latin, which may have different statistical properties than the original paper's training data? This is testable once even a small fluency model exists.
