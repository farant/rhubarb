---
id: REF0036
title: "DKC Demo 66 — Quaternionic 24-Cell"
type: dkc_demo
source_project: knotapel
demo_number: 66
summary: "The binary octahedral group generates exactly 24 unit quaternions -- the vertices of the 24-cell (self-dual, densest lattice packing in 4D). 24-cell Voronoi partition outperforms geographic grids: 35 solutions from 25 cells vs 34 from 64 cells."
related: []
---

## Description

Demo 66 established the quaternionic foundation of DKC at delta=0. The binary octahedral group (SU(2) subgroup of order 48, with 24 unit quaternion states) forms the vertices of the 24-cell -- a mathematically optimal polytope that is self-dual with the densest lattice packing in 4 dimensions. The 24-cell Voronoi partition on S3 outperformed ad-hoc geographic grids (35 solutions from 25 Voronoi cells vs 34 solutions from 64 geographic cells).

## Relevance to Eigenflower

- The 24-element quaternion set is the "natural vocabulary" of DKC at z8 -- a mathematically optimal alphabet
- The 24-cell's self-dual densest packing property suggests it as a quantization target for weight lattices
- NVFP4's 16 representable values are ad-hoc; DKC's 24 values are the vertices of a mathematically optimal polytope
- Relevant to the GPTQ-as-CVP connection: the 24-cell lattice provides provably better quantization than uniform grids
