---
id: CON0010
title: "TRM MLP-for-fixed-size provably limited by DKC 11/13 half-plane theorem"
connects:
  - "TRM finding that MLP replaces attention for fixed-size tasks but fails for variable-size"
  - "DKC 11/13 half-plane theorem (D61-D62, PROVEN analytically)"
type: mathematical_identity
strength: strong
source: "extractions/group-a-knotapel-relevance.md, Connection 10"
---

TRM shows MLP suffices for fixed-size tasks (Sudoku: 87.4% with MLP vs 74.7% with attention) but fails for variable-size. DKC proves analytically WHY the MLP-equivalent has limits.

For ALL half-plane activations (the MLP-equivalent), exactly 11/13 NPN classes are reachable. The two unreachable classes (0x06 and 0x1B) fail for fundamentally different geometric reasons: 0x1B via interval-squeeze (global impossibility) and 0x06 via parallelogram (local obstruction).

The convexity thesis (D57, D61-62) is the formal bridge: half-planes are convex, and the two unreachable NPN classes require non-convex decision boundaries. TRM's MLP variant is a convex classifier; its failures on variable-size tasks may occur for the same reason -- classification boundaries for variable-length reasoning are non-convex.

The four-tier hierarchy (D50: k=2 gives 5 classes, k=3 gives 6, k=4 gives 12, k=6 gives 13) provides a roadmap for TRM activation design: each tier of activation complexity unlocks specific computational capabilities. DKC proves the two failures are geometrically distinct (global vs local), suggesting TRM's attention solves two separable problems.
