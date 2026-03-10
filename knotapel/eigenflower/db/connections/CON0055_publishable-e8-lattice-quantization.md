---
id: CON0055
title: "PUBLISHABLE: E8 lattice quantization via icosian ring"
connects:
  - "GPTQ CVP framework applicable to any lattice"
  - "DKC binary icosahedral group 2I = E8 via icosian ring (D94)"
  - "Liu & Calderbank 2008: E8/icosian has 'maximal distinguishability'"
type: cross_pollination
strength: strong
source: "notes/01-analysis.md, Publishable Ideas #4"
---

Provably denser packing than Z^n quantization. The E8 lattice has the densest sphere packing in 8 dimensions (Viazovska 2016 Fields Medal result), meaning maximum distance from any point to nearest lattice point is minimized. Running Babai's nearest plane on E8 via icosian coordinates would produce lower quantization error than Z^8.

Code exists on both sides:
- Knotapel: `Zr5` (Z[sqrt(5)] ring) and `Q2I` quaternion type implement exact icosian ring operations
- GPTQ: Babai's algorithm is well-known and implemented

What's needed for the bridge:
1. Hessian computation from calibration data
2. LDL decomposition adapted to E8 lattice basis
3. Nearest-plane implementation in icosian coordinates
4. Coxeter embedding of icosian ring into Z^8

Liu & Calderbank (2008, literature entry #82) proved the icosian ring has "maximal distinguishability" -- exactly the property GPTQ needs for minimal quantization error. D94 shows 2I outperforms z8 at matched catalog size (1.67x at N=6), demonstrating the E8 advantage computationally.
