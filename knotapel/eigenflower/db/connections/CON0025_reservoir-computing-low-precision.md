---
id: CON0025
title: "DKC reservoir computing interpretation validates NVFP4 low-precision training"
connects:
  - "NVFP4: 80%+ of training in FP4 (16 values per block)"
  - "DKC as discrete algebraic reservoir computer with solvability bottleneck (D94)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 5"
---

DKC's reservoir weights are NEVER updated -- derived from topology, computed once, used as-is. Barrington's theorem (D94) confirms the key variable is algebraic structure (solvable vs non-solvable group), not precision.

The binary icosahedral group (2I, order 120, E8) outperforms binary octahedral (z8, order 48) at matched catalog size, with advantage GROWING with difficulty (1.07x at N=3, 1.67x at N=6). This is a capacity result depending entirely on algebraic structure, not numerical precision.

NVFP4's finding that 16 values suffice is consistent: what matters is the algebraic/group-theoretic structure of representable values, not their numerical resolution.

Practical implication: a hybrid DKC-neural architecture where bottom layers use exact algebraic weights (from topology) and top layers use NVFP4-style low-precision training. Exact layers act as reservoir; trained layers act as readout. This splits the precision requirement: exact arithmetic where algebraic structure matters, low-precision where gradient-based learning suffices.
