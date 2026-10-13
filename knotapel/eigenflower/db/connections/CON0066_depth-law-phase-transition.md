---
id: CON0066
title: "DKC depth law predicts TRM's optimal recursion count for N-input parity"
connects:
  - "TRM using n=6 recursion steps for Sudoku (81 cells)"
  - "DKC depth law: max_xor = depth + 6, so depth = max_xor - 6 (D82)"
type: technique_transfer
strength: moderate
source: "extractions/group-a-knotapel-relevance.md, Connection 5 (quantitative prediction)"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

The DKC depth law (max_xor ~ depth + 6) makes a quantitative prediction for TRM: if the task requires N-input parity, TRM should need approximately N - 6 recursion steps.

TRM uses n=6 recursions for Sudoku (81 cells). Does the DKC depth law predict this? Sudoku is not pure parity, but it involves constraint satisfaction that includes parity-like constraints (each row/column/box must contain each digit exactly once).

The depth law also predicts a two-phase dynamic: early recursions (0-6) establish broad coverage (direction explosion in DKC), later recursions (7+) refine precision (angle refinement). TRM should show a similar transition where early recursion steps build representations and later steps refine them.

The balanced exponentials mechanism (D91: supply ~2x/depth, demand 4x/weight) could inform TRM's learning rate and recursion count scheduling: match the growth rate of representational capacity to the growth rate of task difficulty.
