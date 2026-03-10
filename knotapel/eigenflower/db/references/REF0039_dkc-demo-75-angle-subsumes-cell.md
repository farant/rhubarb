---
id: REF0039
title: "DKC Demo 75 — Angle Subsumes Cell"
type: dkc_demo
source_project: knotapel
demo_number: 75
summary: "The S1 angle channel provides finer discrimination than discrete Voronoi cell assignment. sqrt(n) magnitude quantization constrains the Z[zeta_8] trajectory to specific shells. The S1 x S2 'continuous' activation outperforms discrete Voronoi."
related: []
---

## Description

Demo 75 established that the continuous S1 angle channel provides finer computational discrimination than discrete Voronoi cell assignment on S2. The magnitudes of Z[zeta_8] values take values from {0, 1, sqrt(2), sqrt(3), 2, sqrt(6)} -- a sqrt(n) quantization that constrains the computational trajectory to specific shells, exactly the kind of structured latent geometry that CALM's VAE regularization aims to produce.

## Relevance to Eigenflower

- The sqrt(n) shell structure is a natural "continuous-like" geometry arising from exact integer arithmetic -- relevant to CALM's continuous vector prediction framework
- Demonstrates that continuous activation outperforms discrete -- relevant to activation function design choices for Eigenflower
- The shell quantization provides structured latent geometry without explicit regularization
