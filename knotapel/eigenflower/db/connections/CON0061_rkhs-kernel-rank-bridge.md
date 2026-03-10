---
id: CON0061
title: "RKHS kernel rank test bridges DKC reservoir thesis AND Hilbert space operator capacity"
connects:
  - "Kiruluta et al.: Representer Theorem and RKHS kernel rank as operator capacity measure"
  - "DKC open question Q11.4: does rank(K_2I)/rank(K_z8) > 120/24? (D94)"
type: mathematical_identity
strength: strong
source: "extractions/group-d-knotapel-relevance.md, Connection 2 (Representer Theorem)"
---

The RKHS kernel rank test (seeded as D95, partially completed) IS Paper 2's core measurement applied to DKC.

The Representer Theorem says the optimal classifier lies in the span of kernel sections at training points. DKC's catalog values ARE the kernel evaluation points. The "kernel" is the inner product in the cyclotomic ring.

Running the test would simultaneously validate:
1. DKC reservoir thesis: whether non-solvable group (2I, E8, order 120) provides richer RKHS than solvable (z8, E7, order 48)
2. Paper 2's operator capacity theory: whether kernel rank predicts computational capability

If rank(K_2I)/rank(K_z8) > 120/24 = 5, then non-solvability contributes above raw group size -- confirming that ALGEBRAIC STRUCTURE, not just dimensionality, determines reservoir/operator quality.

This is the single most direct experimental bridge between the eigenflower paper extractions and the knotapel computational program. It requires only one computation (two kernel matrix rank calculations) and produces a clean yes/no answer.
