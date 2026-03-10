---
id: PT0039
title: "Decompile existing models to find wisdom circuits"
status: untested
source: ["Transparency and Wisdom Circuits notes", "GPTQ-as-CVP paper"]
applicable_to: []
related: [PT0038, PT0018]
---

Reverse-engineer trained LLMs to understand what structural patterns (wisdom circuits) they use for fuzzy/analogical reasoning, then build those patterns deliberately into a transparent architecture.

The GPTQ-as-CVP framework gives a concrete tool: decompose trained weight matrices on cyclotomic lattices and look for algebraic signatures of logical/relational patterns. If a trained LLM has internal circuits implementing analogical reasoning, those circuits should leave algebraic signatures in the weights.

The Gram-Schmidt vectors of the lattice basis reveal "independent directions of variation" in weight space, weighted by importance (Hessian diagonal). Eigendecomposition of weight matrices reveals spectral structure. Koopman-style analysis of model dynamics could reveal learned operators.

The DKC standard for transparency: know what every weight does, what the activation does, and why certain combinations compute certain functions. Apply this standard to decompiled circuits.

Goal: forward-engineer transparent architectures informed by what decompilation reveals. Not decompiling for curiosity -- decompiling to understand mechanisms so you can build them deliberately.
