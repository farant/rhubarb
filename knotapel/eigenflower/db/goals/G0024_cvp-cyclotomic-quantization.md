---
id: G0024
title: "CVP-optimal weight quantization on cyclotomic lattices"
status: open
priority: low
depends_on: [G0017]
blocks: []
related: []
---

## Description

Develop CVP-optimal weight quantization using cyclotomic lattices — extending the GPTQ-as-CVP framework from standard integer lattices to non-standard algebraic lattices (Z[zeta_8], icosian ring / E8). This is identified in 01-analysis.md as a publishable three-community intersection: lattice algorithms + algebraic number theory + neural network quantization.

The Group C crossover identifies the gap: "They notice quantization IS a lattice problem but haven't made the leap to non-standard lattices." The E8 lattice quantization idea has a concrete implementation path — the icosian ring code exists in knotapel, Babai's algorithm is known, what is needed is the bridge (Hessian computation + LDL decomposition in icosian coordinates).

Provably denser packing than Z^n quantization means better weight quantization with less error for the same bit budget.

## Why It Matters

This serves both eigenflower and the broader research community:

For eigenflower: better quantization means the fluency layer's ternary weights can be more expressive at the same memory cost. If cyclotomic lattice quantization reduces quantization error by even 10-20%, that translates to a better fluency model within the same 1-2GB RAM budget.

For publication: this is one of four publishable ideas identified in the analysis. The implementation path exists from knotapel code, making it relatively low-hanging fruit for a novel result.

The "3.125 bits as natural information density" parallel with DKC's optimal z8 catalog of 21 (not 24) entries suggests there are structural reasons why cyclotomic lattices should be better for neural network quantization — the natural resolution is smaller than naively expected.

## What "Achieved" Looks Like

- GPTQ framework extended to cyclotomic lattice targets (Z[zeta_8], icosian ring)
- Hessian computation + LDL decomposition in cyclotomic coordinates
- Babai's algorithm adapted for non-standard lattices
- Quantization error compared against standard Z^n quantization on the same models
- Provable error bounds derived from lattice geometry
- Paper-ready results with benchmark comparisons
