---
id: Q0021
title: "Can Tucker decomposition reveal higher-order correlation structure in DKC computations?"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis identifies Tucker decomposition (from tensor logic) as one of four concrete things knotapel gains from Group D papers. DKC computations involve multi-dimensional structure (weights, inputs, outputs, activations, algebra elements) that could have higher-order correlations not visible in pairwise analysis. Tucker decomposition factors a tensor into a core tensor multiplied by a matrix along each mode, revealing the underlying rank structure. The question is whether applying Tucker decomposition to DKC's computed output tensors (e.g., the full truth table of computed Boolean functions as a function of weight and activation parameters) reveals interpretable structure beyond what existing analysis (Raqiya, Scrutinium) already captures.
