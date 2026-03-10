---
id: CON0024
title: "E8 lattice quantization via icosian ring: provably denser than Z^n"
connects:
  - "GPTQ CVP framework applicable to any lattice basis"
  - "DKC binary icosahedral group 2I (order 120) = E8 lattice via icosian ring (D94)"
type: technique_transfer
strength: strong
source: "extractions/group-c-knotapel-relevance.md, Connection 12"
---

D94 establishes that the binary icosahedral group 2I IS the E8 lattice via the icosian ring (Liu & Calderbank 2008). 2I outperforms z8 (binary octahedral, E7) at matched catalog size, with advantage growing at harder computations (1.07x at N=3, 1.67x at N=6).

The E8 lattice has the densest sphere packing in 8 dimensions. Running Babai's nearest plane on E8 (via icosian coordinates) would produce LOWER quantization error than Z^8 -- provably, by the packing density ratio.

Knotapel already has exact arithmetic: the `Zr5` type (Z[sqrt(5)] ring) and `Q2I` quaternion type implement exact icosian ring operations. Converting from quaternion form to 8D integer coordinates (the Coxeter embedding of icosian ring into Z^8 as E8 root lattice) gives a direct implementation of E8-lattice quantization.

This is potentially publishable as an independent result: "E8 lattice quantization of neural network weights via Babai's algorithm on the icosian ring." What's needed: Hessian computation from calibration data, LDL decomposition adapted to E8 lattice basis, nearest-plane in icosian coordinates.
