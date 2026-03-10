---
id: G0017
title: "Decompile existing models to understand internal circuits"
status: open
priority: medium
depends_on: []
blocks: [G0018, G0019]
related: []
---

## Description

Reverse-engineer existing open-source language models to understand what structural patterns their weights encode. The goal is not curiosity — it is decompiling to understand what mechanisms ANNs use for fuzzy/analogical reasoning, so those mechanisms can be forward-engineered into eigenflower's architecture with full transparency.

The GPTQ-as-CVP framework provides the concrete tool: decompose trained weight matrices on cyclotomic lattices and look for algebraic signatures that reveal logical/relational patterns the model learned. If a trained LLM has internal circuits that implement analogical reasoning, those circuits should leave algebraic signatures in the weights.

## Why It Matters

Eigenflower aims to build transparent reasoning circuits from first principles. But there may be patterns — especially for fuzzy/analogical reasoning — that are easier to discover by examining what gradient descent finds than by constructing from theory alone.

The 03-transparency document frames this clearly: "Reverse engineering the mechanism so you can forward-engineer it with clarity." DKC's algebraic analysis tools (raqiya, scrutinium) are directly applicable to analyzing weight matrices decomposed on cyclotomic lattices.

This is also potentially publishable work: "CVP-optimal weight quantization on cyclotomic lattices" is identified as one of four publishable ideas in the analysis.

## What "Achieved" Looks Like

- Selected open-source model(s) analyzed (likely small models that fit on M2)
- Weight matrices decomposed on cyclotomic lattices using GPTQ-as-CVP
- Algebraic structure of the decomposition analyzed using DKC tools
- Identifiable circuits correlated with known reasoning patterns
- At least one mechanism understood well enough to be reconstructed from first principles
- Findings documented with implications for eigenflower's architecture
