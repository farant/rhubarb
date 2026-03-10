---
id: LIM0011
title: "Full transparency — no black boxes accepted"
type: chosen
source: "notes/03-transparency-and-wisdom-circuits.md"
impacts: []
related: [LIM0005]
---

Every component of the system should be understood well enough to explain WHY it works, not just THAT it works. The standard is DKC-level transparency: know what every weight does, what the activation does, and why certain combinations compute certain functions.

This is stated explicitly in note 03: "Never accept a black box."

Why:
- DKC achieves total transparency for Boolean computation. Eigenflower aims to extend that level of transparency to reasoning in general.
- If you don't understand how a component works, you can't debug it, improve it, or trust it for reasoning about theology (where correctness matters enormously).
- The entire Catholic philosophical tradition emphasizes understanding over mere utility. A tool you don't understand is not really yours.

Implications:
- The syllogism layer is transparent by construction — tensor equations with exact arithmetic, every step verifiable.
- The fluency layer is the challenge. Neural networks are inherently less transparent than algebraic equations. The "decompile existing models" goal is about understanding mechanisms well enough to build them deliberately.
- The wisdom circuits concept (note 03) is an attempt to make the fluency layer's learned patterns legible — identifying domain-independent circuits and understanding what they compute.
- This constraint may slow development but produces a system that can be trusted, taught, and maintained across generations (consistent with the millennial thinking philosophy).
- Tension with practicality: at some point the fluency layer WILL have learned representations that are hard to fully interpret. The goal is to minimize the opaque surface area, not eliminate it entirely.
