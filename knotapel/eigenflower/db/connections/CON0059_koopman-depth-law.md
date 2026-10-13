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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The depth law (max_xor ~ depth + 6) describes how computational capacity grows with sequential dynamical steps (BFS rounds of braid generator multiplication). It is demonstrated empirically across multiple settings but not analytically proved.

Koopman operators are linear operators on function spaces that capture nonlinear dynamics. The BFS closure of braid generators IS a discrete dynamical system. Koopman analysis could yield a spectral decomposition of the depth law, potentially proving it analytically.

The spectral decomposition would explain:
- WHY the slope is 1 (each BFS round adds exactly one unit of computational capacity)
- WHY the intercept is 6 (the "free" capacity from the initial generator set)
- Whether the balanced exponentials mechanism (D91: supply ~2x/depth, demand 4x/weight) has a spectral interpretation

This would connect the depth law to the Hilbert space framework, making DKC's most important scaling result a theorem rather than an empirical observation.
