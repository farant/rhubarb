---
id: CON0059
title: "Koopman operators could formalize the DKC depth law analytically"
connects:
  - "Kiruluta et al.: Koopman operators linearize nonlinear dynamics in function space"
  - "DKC linear depth law: max_xor ~ depth + 6 (D82, demonstrated but not analytically proved)"
type: technique_transfer
strength: moderate
source: "notes/01-analysis.md, Part 2 (Group D gains #4)"
---

The depth law (max_xor ~ depth + 6) describes how computational capacity grows with sequential dynamical steps (BFS rounds of braid generator multiplication). It is demonstrated empirically across multiple settings but not analytically proved.

Koopman operators are linear operators on function spaces that capture nonlinear dynamics. The BFS closure of braid generators IS a discrete dynamical system. Koopman analysis could yield a spectral decomposition of the depth law, potentially proving it analytically.

The spectral decomposition would explain:
- WHY the slope is 1 (each BFS round adds exactly one unit of computational capacity)
- WHY the intercept is 6 (the "free" capacity from the initial generator set)
- Whether the balanced exponentials mechanism (D91: supply ~2x/depth, demand 4x/weight) has a spectral interpretation

This would connect the depth law to the Hilbert space framework, making DKC's most important scaling result a theorem rather than an empirical observation.
