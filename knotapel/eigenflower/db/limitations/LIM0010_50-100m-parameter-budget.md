---
id: LIM0010
title: "~50-100M parameter budget for fluency layer"
type: chosen
source: "notes/02-architecture-insight.md, notes/01-analysis.md"
impacts: []
related: [LIM0001, LIM0002, LIM0003, LIM0009, LIM0017]
---

The fluency ANN is sized at approximately 50-100M parameters. This is the sweet spot identified by the Group C analysis for training within 30-60 minutes on M2 hardware.

Why this range:
- Below 50M: may not have enough capacity to handle theological Latin + English vocabulary and grammar.
- Above 100M: training time exceeds the 30-60 minute constraint on M2.
- The TRM paper shows 7M params with recursive refinement can beat billion-param models on reasoning — but TRM operates on grid puzzles with tiny vocabularies. Natural language requires more capacity for the embedding layer alone.
- With ternary weights at 1.58 bits/param, 100M params = ~20MB storage, well within RAM budget.

Implications:
- The embedding layer is a significant fraction of total parameters. Vocabulary size directly impacts how many parameters are "left over" for the transformer layers.
- Recursive architectures (TRM-style) help: instead of more parameters, apply the same parameters more times. This gets more computational depth from a fixed parameter budget.
- The hybrid architecture helps enormously: the fluency layer doesn't need to store knowledge or reason — it only needs to parse and generate. This means more of its capacity can go to linguistic competence rather than world knowledge.
- May need to experiment with the exact size within this range to find the right balance of linguistic capability vs training time.
