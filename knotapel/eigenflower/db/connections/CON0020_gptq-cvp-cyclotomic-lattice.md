---
id: CON0020
title: "GPTQ weight quantization as CVP applied to cyclotomic integer lattices"
connects:
  - "GPTQ-as-CVP: weight quantization = Babai's nearest plane on Hessian lattice"
  - "DKC Z[zeta_8] as algebraic integer lattice with exact weight arithmetic (D29, D107)"
type: technique_transfer
strength: strong
source: "extractions/group-c-knotapel-relevance.md, Connection 1"
---

GPTQ is mathematically identical to Babai's nearest plane algorithm for CVP on a lattice defined by the input Hessian. But GPTQ only considers the trivial lattice Z^n (plain integers).

DKC weights live on the cyclotomic integer lattice Z[zeta_8] = {a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3}, rank 4 over Z. The training-as-focusing thesis (D29) means the CVP has exact solutions -- the target vector already lies on a lattice point (quantization error = exactly zero).

The connection runs both directions:
1. GPTQ -> DKC: If quantization is CVP and DKC weights are already lattice points, DKC is the limiting case where quantization error vanishes.
2. DKC -> GPTQ: Algebraic integer lattices (Z[zeta_8] rank 4, Z[zeta_16] rank 8, icosian/E8 rank 8) are denser than Z^n. Babai's algorithm on Z[zeta_8] finds closer vectors for the same bitwidth.

This identifies a genuine literature gap: nobody has applied CVP solvers to cyclotomic lattices for neural network quantization. The GPTQ community works with Z^n; coding theory uses algebraic lattices for communication; knotapel uses them for Boolean computation. The three-community intersection is unoccupied -- potentially publishable as "CVP-optimal weight quantization on cyclotomic lattices."

Knotapel has complete exact arithmetic: Cyc8, Cyc16, Cyc5, Cyc24, and the icosian/E8 ring.
