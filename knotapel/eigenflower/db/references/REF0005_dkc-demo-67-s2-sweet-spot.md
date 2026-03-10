---
id: REF0005
title: "DKC Demo 67 — S2 Sweet Spot and Hopf Phase Inertness"
type: dkc_demo
source_project: knotapel
demo_number: 67
summary: "DKC computation lives on S2 (rotation axis direction on the Bloch sphere). The S1 phase fiber is computationally inert. 13-direction Voronoi partition established as the natural computational domain."
related: []
---

## Description

Demo 67 established that the computational domain of DKC is S2 (the 2-sphere of rotation axis directions), not S3 (the full quaternion space) or S1 (the phase). The Hopf decomposition S3 -> S2 x S1 revealed that the S1 phase fiber carries no computational information. The 13-direction Voronoi partition on S2 became the foundational geometric structure for subsequent demos.

## Relevance to Eigenflower

- The discovery that phase information should be suppressed and directional information preserved parallels Orchid's phase suppression conditioning
- The 13-direction Voronoi is the natural "routing" structure for DKC computation -- analogous to data-dependent kernel selection in Orchid
- Demonstrates that the "right" representation space for computation is lower-dimensional than the full algebraic structure -- relevant to choosing representation dimensions for Eigenflower's reasoning layer
