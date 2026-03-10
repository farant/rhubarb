---
id: Q0002
title: "Can the GPTQ-as-CVP mechanism compile syllogisms to weights on cyclotomic lattices?"
status: open
source: "notes/01-analysis.md"
related: []
---

GPTQ frames weight quantization as a Closest Vector Problem with known algorithms and provable error bounds. The crossover analysis identifies that nobody in the GPTQ literature has made the leap to non-standard lattices (cyclotomic, E8, icosian). If "compile syllogism to weights" is literally a CVP, then there are concrete algorithms (Babai's algorithm, LDL decomposition) that could do it with exact error control. The question is whether the Hessian computation + LDL decomposition can be performed in cyclotomic coordinates, and whether the resulting quantized weights preserve the logical correctness of the compiled syllogism.
