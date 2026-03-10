---
id: Q0008
title: "Can E8 lattice quantization via icosian ring achieve provably denser packing than Z^n for weight quantization?"
status: open
source: "notes/01-analysis.md"
related: []
---

The crossover analysis identifies E8 lattice quantization as a concrete implementation path: the icosian ring code exists, Babai's algorithm is known, what's needed is the bridge (Hessian computation + LDL decomposition in icosian coordinates). E8 is the densest lattice packing in 8 dimensions, so quantizing weights to E8 lattice points should give better precision per bit than Z^n quantization. The question is whether this theoretical advantage translates to measurable improvement in model quality for eigenflower-scale models (50-100M params), and whether the implementation complexity is justified.
