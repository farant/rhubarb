---
id: REF0007
title: "DKC Demo 73 — Eigenvector Automaton (82.8% Determinism)"
type: dkc_demo
source_project: knotapel
demo_number: 73
summary: "DKC is fundamentally additive (weight vector sum, not braid matrix product). The 13-cell Voronoi partition functions as a near-automaton: 82.8% deterministic transitions, universal across all 36 winning triples. Additive accumulation IS a state space model."
related: []
---

## Description

Demo 73 proved that DKC is fundamentally additive, not multiplicative. The multiplicative braid product formulation CANNOT compute XOR6 (complement-all-bits sends M to M^{-1}, same eigenvector cell). Only the additive weight sum S_k = S_{k-1} + w_k computes parity. The 82.8% universal automaton determinism quantifies the memory-computation tradeoff. The non-determinism arises from discarded magnitude ("compass without odometer").

## Relevance to Eigenflower

- The additive accumulation IS a State Space Model: hidden state = partial sum in Z[zeta_8], transition = additive update. Directly relevant to H-Net's SSM compression bias
- The 82.8% determinism quantifies the fading memory property for DKC as a reservoir computer
- The additive nature means DKC operations are inherently parallelizable -- relevant to efficient inference on M2
- The "compass without odometer" finding informs representation design: direction matters more than magnitude for computation
