---
id: REF0011
title: "DKC Demo 94 — Solvability Bottleneck, Barrington Confirmed"
type: dkc_demo
source_project: knotapel
demo_number: 94
summary: "Non-solvable binary icosahedral group (2I, order 120, E8 lattice) outperforms solvable binary octahedral (z8, order 48) at matched catalog size. Advantage grows: 1.07x at N=3, 1.67x at N=6. Barrington's theorem confirmed as capacity predictor. DKC formalized as 'discrete algebraic reservoir computer' with five-pillar synthesis."
related: []
---

## Description

Demo 94 established the solvability bottleneck: the algebraic structure of the weight group (solvable vs non-solvable) determines computational capacity, confirming Barrington's theorem as a predictor. The binary icosahedral group 2I (non-solvable, E8 lattice, order 120) beats the binary octahedral group z8 (solvable, order 48) with growing advantage at harder tasks.

The five-pillar synthesis formalizes DKC as a "discrete algebraic reservoir computer":
1. Habiro integrality -- exact cyclotomic arithmetic
2. Nazer-Gastpar lattice codes -- lattice structure for computation
3. Abramsky categorical computation -- compositional structure
4. Aizenberg MVN activation -- multi-valued nonlinear readout
5. Reservoir computing -- fixed dynamics + trained readout

## Relevance to Eigenflower

- The deepest architectural connection: DKC provides the theoretical framework for WHY a ternary recursive model should work
- The solvability bottleneck predicts that DEQ models (convergent to fixed point = solvable dynamics) fail for hard reasoning -- directly relevant to architecture choices
- The five-pillar synthesis connects DKC to every Group B paper's compression axis
- The RKHS kernel rank test (open question) would validate the reservoir computing interpretation
- Practical: a hybrid DKC-neural architecture where exact algebraic weights (from topology) form the reservoir and trained layers form the readout
