---
id: REF0001
title: "DKC Demo 29 — Forward DKC Proven"
type: dkc_demo
source_project: knotapel
demo_number: 29
summary: "Exact Z[zeta_8] bracket values compute XOR without training. 100+ triples, RMS=0.000. The 'training-as-focusing' thesis: gradient descent locates pre-existing solutions on the cyclotomic lattice."
related: []
---

## Description

Demo 29 is the foundational LANDMARK of the DKC research program. It proved that Kauffman bracket values evaluated at A = e^{i*5pi/4} (delta=0), stored as exact cyclotomic integers in Z[zeta_8], can serve as neural network weights that compute Boolean XOR without any training.

The key transition: from floating-point approximate arithmetic (Demos 10-28) to exact Z[zeta_8] integer arithmetic. The Cyc8 type uses 16 integer multiplies per product with zero floating-point error. This enabled the "training-as-focusing" thesis -- the computational structure pre-exists in the algebraic lattice; training merely locates it.

## Relevance to Eigenflower

- The exact arithmetic substrate (Cyc8) is the foundation for any DKC-based reasoning layer
- The "training-as-focusing" thesis parallels the Eigenflower insight that compiled syllogisms are pre-existing algebraic structure, not learned approximations
- The bracket catalog (100 distinct Z[zeta_8] values) functions as a fixed reservoir in the reservoir computing interpretation
- Forward DKC IS "inverse knowledge search" over the braid group -- searching backward from Boolean endpoints through braid-space
- The BFS catalog construction is analogous to forward chaining in tensor logic: start with generators, compose, accumulate until closure
