---
id: Q0003
title: "Can a 7M-param TRM-style recursive model handle Latin theological vocabulary?"
status: open
source: "notes/01-analysis.md"
related: []
---

TRM achieves strong reasoning at 7M parameters on grid puzzles with tiny vocabularies. Latin theological text has a much larger vocabulary (10,000+ words with rich morphology), which inflates the embedding layer. A 7M-param model might not have enough capacity for language once the embedding layer consumes most of the parameter budget. The question is: what is the minimum parameter count for a recursive model that can handle theological Latin + English at a competent level? And does the H-Net tokenizer-free approach (which eliminates the embedding layer bottleneck) change this threshold significantly?
