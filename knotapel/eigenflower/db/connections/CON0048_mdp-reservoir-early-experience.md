---
id: CON0048
title: "DKC reservoir computer has an exactly characterized MDP for early experience"
connects:
  - "Early Experience MDP formalization: S, A, T, R, learning T without R"
  - "DKC five-pillar reservoir synthesis with exact transition dynamics (D94)"
type: structural_parallel
strength: strong
source: "extractions/group-e-knotapel-relevance.md, Connection 9"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Paper 2 formalizes environments as MDPs and learns transition dynamics without reward. DKC has an exactly characterized MDP:
- States S: sets of bracket values (catalog at a given BFS depth)
- Actions A: braid generator multiplications (sigma_i and sigma_i^{-1})
- Transition T: BFS closure -- apply generator, get new bracket value. Exact, deterministic, zero noise.
- Reward R: not needed. Algebraic structure IS the computational content (Training-as-Focusing).

Paper 2's IWM objective (predict next-states from (state, action) pairs) maps to: given bracket catalog and braid generator, predict new bracket values. This is exactly what BFS closure computes.

Paper 2's SR objective (explain WHY expert action is better) maps to: explain WHY depth-8 entries outperform depth-3 (axis cancellation, algebraic coherence).

The five-pillar synthesis connects to: TL algebra (topology), Aizenberg MVN (activation geometry), Habiro (exact arithmetic), Nazer-Gastpar (lattice coding), reservoir computing (fixed reservoir + readout). The RKHS kernel rank test would validate whether 2I's non-solvable structure exceeds z8's solvable structure in the precise mathematical sense reservoir theory requires.
