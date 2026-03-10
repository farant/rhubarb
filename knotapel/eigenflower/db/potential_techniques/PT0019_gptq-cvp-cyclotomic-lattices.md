---
id: PT0019
title: "GPTQ-as-CVP on cyclotomic lattices (publishable idea)"
status: untested
source: ["GPTQ-as-CVP paper (Chen et al., 2025)", "Knotapel crossover analysis"]
applicable_to: []
related: [PT0018, PT0020]
---

Extend the GPTQ-as-CVP framework from standard integer lattices to cyclotomic lattices (Z[zeta_8], Gaussian integers Z[i], Eisenstein integers Z[omega]). The paper identifies quantization as a lattice problem but hasn't made the leap to non-standard lattices.

Cyclotomic lattices have denser packing than Z^n, which means provably better quantization. The knotapel codebase already has exact arithmetic on these lattices (raqiya.h, Cyc8 type). The bridge needed: Hessian computation + LDL decomposition in cyclotomic coordinates.

This is identified as a potentially publishable result at the intersection of three communities: lattice algorithms + algebraic number theory + neural network quantization.

Implementation path exists: icosian ring code from knotapel, Babai's algorithm is known, what's needed is the Hessian-in-cyclotomic-coordinates computation.

The "3.125 bits as natural information density" parallel with knotapel's optimal z8 catalog of 21 (not 24) entries suggests both demonstrate that the natural resolution is smaller than naively expected.
