---
id: G0019
title: "Build transparent analogical reasoning"
status: open
priority: medium
depends_on: [G0017, G0018]
blocks: [G0023]
related: []
---

## Description

Design and implement an architectural component for analogical reasoning that is transparent in the way DKC is transparent for Boolean computation. Analogy works by proportionality: A is to B as C is to D. It has its own logic — you cannot conclude identity from analogy, only similarity of relation.

Aquinas uses analogy extensively, especially for reasoning about God: "Grace is to the soul as light is to the eye." The mathematical structure is morphisms between relations — a well-studied area of mathematics.

The research program from 03-transparency:
1. Study analogical wisdom circuits — proportionality relations (A:B::C:D)
2. Determine what algebraic structure they need
3. Can they be compiled transparently (like syllogisms)?
4. If not, what is the minimum opacity and can it be bounded?

## Why It Matters

The 01-analysis raises a critical question: "How much of the Summa's reasoning is actually syllogistic vs analogical/authoritative/other? If only 30% is formal syllogisms, the graph is sparse and the fluency layer is doing a lot of heavy lifting for the other 70%."

If analogical reasoning cannot be handled transparently, a large fraction of the Summa's argumentation falls outside eigenflower's core capability. Building transparent analogy extends the system's reach from pure deduction to the full range of Thomistic reasoning.

Analogy is also the spanning case — the most interesting wisdom circuit type because it requires BOTH the fluency layer (parse the four terms) and the syllogism layer (encode the proportionality relation).

## What "Achieved" Looks Like

- Mathematical formalization of analogical reasoning as morphisms between relations
- Architectural component that computes analogical inferences with known transparency bounds
- Test cases from the Summa's analogical arguments
- Clear statement of what the component CAN and CANNOT do (no pretending analogy is deduction)
- Integration with the syllogism graph (analogical relationships as a distinct edge type)
- Comparison with deductive transparency: where does analogy fall on the opacity spectrum?
