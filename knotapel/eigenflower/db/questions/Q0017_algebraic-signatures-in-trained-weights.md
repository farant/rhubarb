---
id: Q0017
title: "Do trained LLM weights contain algebraic signatures of reasoning circuits detectable via cyclotomic decomposition?"
status: open
source: "notes/03-transparency-and-wisdom-circuits.md"
related: []
---

The "decompile" goal reframed: decompose trained weight matrices on cyclotomic lattices using GPTQ-as-CVP and look for patterns that correspond to known abstract reasoning structures. If a trained model has internal circuits for analogical reasoning, those circuits should leave algebraic signatures in the weights — patterns that are interpretable when viewed through the cyclotomic lens. This is a concrete experiment: take a small open-source model (GPT-2 scale), decompose its attention/MLP weights onto Z[zeta_8] lattice, and look for structure. Does the decomposition reveal anything interpretable, or is it noise? This would validate or refute the entire "decompile to understand" approach.
