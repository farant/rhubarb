---
id: CON0008
title: "Combined BitNet+TRM+Orchid 'Eigenflower Stack' IS a DKC-type reservoir computer"
connects:
  - "Proposed Eigenflower Stack: ternary recursive Orchid (7M params, 1.4MB)"
  - "DKC as discrete algebraic reservoir computer (D94, five-pillar synthesis)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 8"
---

The Eigenflower Stack (ternary recursive Orchid) is, at its core, a reservoir computer: a fixed nonlinear transformation applied recursively, with a simple readout. DKC is already proven to be a reservoir computer -- the catalog IS the reservoir, the signed-sum IS the linear readout, and the Voronoi/sector activation IS the nonlinear output.

DKC provides what the Eigenflower Stack currently lacks: a theoretical framework for WHY the reservoir works. The five connected pillars (Habiro integrality, Nazer-Gastpar lattice codes, Abramsky categorical computation, Aizenberg MVN activation, TL non-semisimplicity) explain exactly which algebraic properties the reservoir must have.

Three specific predictions from DKC for the Eigenflower Stack:
1. Solvability bottleneck (D94): the recursive transformation should NOT converge to a fixed point (non-solvable dynamics > solvable for hard computation). This matches TRM's finding that DEQ models hurt.
2. Barrington-Radical Principle (D102): zero entries in ternary weights carry only abelian information, useless for parity. Only {-1, +1} contribute to hard functions.
3. Optimal catalog (D96): REMOVING certain entries IMPROVES capacity. Pruning the reservoir can improve computational power.

The RKHS kernel rank test (Q11.4) would validate this mapping.
