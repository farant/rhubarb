---
id: G0004
title: "Build tensor logic interpreter"
status: open
priority: high
depends_on: []
blocks: [G0005, G0006, G0007]
related: []
---

## Description

Implement a tensor logic interpreter in C89 that can represent syllogisms as tensor equations and execute them with exact arithmetic. The core equation is Domingos's formulation:

    SP[s,p] = H(SM[s,m] * MP[m,p])

where SP, SM, MP are predication matrices (subject-predicate, subject-middle, middle-predicate) and H is a step function (Heaviside activation). This equation IS Barbara — the most fundamental syllogistic form.

All 256 possible syllogistic forms (4 figures x 64 mood combinations) should be expressible, with the 24 valid forms identified and executable.

## Why It Matters

This is the foundation everything else sits on. It is build priority #1 from the analysis. The tensor logic interpreter is what makes "compile syllogisms to ANN weights" a concrete operation rather than a metaphor. Without it, the syllogism layer has no engine.

The Group D paper extraction established that there is no gap to bridge between "syllogism" and "ANN weight" — they are the same thing in two notations. The interpreter makes this identity operational.

DKC provides the exact arithmetic substrate. The interpreter may use Z[zeta_8] (cyclotomic integers) or plain integer arithmetic — the right choice is an open question, but the interpreter should be designed to support either.

## What "Achieved" Looks Like

- All 4 syllogistic figures representable as tensor equations
- All 24 valid syllogistic forms (Barbara, Celarent, Darii, Ferio, etc.) execute correctly
- Invalid forms correctly identified as invalid
- Exact arithmetic throughout — no floating point
- Concrete test cases from the Summa verified
- Clean C89 implementation following rhubarb conventions
