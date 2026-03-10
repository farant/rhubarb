---
id: CON0064
title: "DKC solvability bottleneck predicts DEQ model failure via Barrington's theorem"
connects:
  - "TRM finding that DEQ (Deep Equilibrium) models hurt generalization"
  - "DKC solvability bottleneck: non-solvable groups outperform solvable (D94, Barrington's theorem)"
type: technique_transfer
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 8 (detail)"
---

DEQ models converge to a fixed point (solvable dynamics). Barrington's theorem says solvable groups cannot compute parity. If TRM is computing parity-like functions, DEQ failure is PREDICTED by the algebraic theory.

D94's solvability bottleneck demonstrates this: binary icosahedral group (2I, non-solvable, order 120) outperforms binary octahedral (z8, solvable, order 48) at matched catalog size. The advantage GROWS with computational difficulty (1.07x at N=3, 1.67x at N=6).

The Barrington-Radical Principle (D102) makes it precise: the "radical" (nilpotent) component of any algebra carries only abelian information, which is useless for parity. Only the semisimple quotient contributes to hard computation.

For the Eigenflower Stack, this means:
1. Recursive transformation should NOT converge to a fixed point
2. Non-solvable internal dynamics are required for hard reasoning tasks
3. Zero entries in ternary weights ({0} from BitNet) carry only abelian information

This is a testable prediction: compare fixed-point-converging vs non-converging recursive architectures on parity-like reasoning tasks.
