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

Paper 2 formalizes environments as MDPs and learns transition dynamics without reward. DKC has an exactly characterized MDP:
- States S: sets of bracket values (catalog at a given BFS depth)
- Actions A: braid generator multiplications (sigma_i and sigma_i^{-1})
- Transition T: BFS closure -- apply generator, get new bracket value. Exact, deterministic, zero noise.
- Reward R: not needed. Algebraic structure IS the computational content (Training-as-Focusing).

Paper 2's IWM objective (predict next-states from (state, action) pairs) maps to: given bracket catalog and braid generator, predict new bracket values. This is exactly what BFS closure computes.

Paper 2's SR objective (explain WHY expert action is better) maps to: explain WHY depth-8 entries outperform depth-3 (axis cancellation, algebraic coherence).

The five-pillar synthesis connects to: TL algebra (topology), Aizenberg MVN (activation geometry), Habiro (exact arithmetic), Nazer-Gastpar (lattice coding), reservoir computing (fixed reservoir + readout). The RKHS kernel rank test would validate whether 2I's non-solvable structure exceeds z8's solvable structure in the precise mathematical sense reservoir theory requires.
