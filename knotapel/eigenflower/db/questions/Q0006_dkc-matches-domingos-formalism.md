---
id: Q0006
title: "Does DKC's forward computation precisely match Domingos's tensor logic formalism?"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis claims "DKC IS tensor logic over Boolean tensors" — that Output[mask] = Activation(Weight[i] * Input[mask, i]) is a tensor equation in Domingos's formalism. This is claimed to be a mathematical identity, not an analogy. The question is whether this can be verified formally: write the DKC forward computation in Domingos's notation and confirm that every step matches. If it does, DKC's 109 demos become an empirical validation suite for tensor logic — something Domingos's paper lacks. If there are discrepancies, they reveal either limitations of the tensor logic framework or structural features of DKC not captured by it.
