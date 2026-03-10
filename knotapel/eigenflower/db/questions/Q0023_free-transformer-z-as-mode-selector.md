---
id: Q0023
title: "Can Free Transformer's latent Z serve as a syllogistic mode selector in the fluency layer?"
status: open
source: "notes/01-analysis.md, notes/02-architecture-insight.md"
related: []
---

Free Transformer's latent Z variable conditions generation on a learned discrete mode. The crossover analysis identifies D107's constructive phase formula as "literally a 3-bit binary mapper (writhe parity + 2 nesting bits -> Z/4Z phase)" — exactly what the Free Transformer's Z does. In the eigenflower architecture, Z could encode: parsing vs generating, Latin vs English, premise vs conclusion, which syllogistic figure is active. The question is whether a small number of Z modes (8-16) is sufficient to capture the relevant modal distinctions for theological reasoning, or whether the space of modes is too large/continuous for a discrete latent variable. Testable by training a Free Transformer variant on the Summa and examining whether the learned Z modes correspond to interpretable categories.
