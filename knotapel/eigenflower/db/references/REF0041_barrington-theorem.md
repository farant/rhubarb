---
id: REF0041
title: "Barrington's Theorem — Non-Solvable Groups Compute Parity"
type: other
source_project: external
summary: "Barrington's theorem: any function in NC^1 (including parity) can be computed by a branching program of polynomial width over any non-solvable group, but NOT over solvable groups. The algebraic key: non-solvability is necessary and sufficient for parity computation via group products."
related: []
---

## Description

Barrington's theorem (1989) establishes that branching programs of polynomial width over a non-solvable group can compute any function in the complexity class NC^1 (which includes parity). Crucially, solvable groups CANNOT compute parity -- the solvability of the group is the exact algebraic property that determines computational capacity.

DKC confirmed this in Demo 94: the binary icosahedral group 2I (non-solvable, order 120) outperforms the binary octahedral group z8 (solvable, order 48) with growing advantage at harder tasks (1.07x at N=3, 1.67x at N=6).

## Relevance to Eigenflower

- Provides the theoretical basis for the solvability bottleneck in DKC
- Predicts that DEQ models (convergent = solvable dynamics) will fail on parity-like reasoning tasks
- The Barrington-Radical Principle (D102) extends this: the radical (nilpotent component) carries only solvable/abelian information, provably useless for parity
- Informs Eigenflower architecture: the algebraic structure of the weight group must be non-solvable for hard reasoning
