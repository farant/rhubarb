---
id: KIT0005
title: "Sherman-Morrison incremental matrix update"
status: planned
tier: 2
input: "Existing inverse matrix A^{-1}, vectors u and v"
output: "Updated inverse (A + uv^T)^{-1}"
depends_on: [KIT0007]
effort: small
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
migrated_from: PT0058
applicable_to: [G0005, G0019]
related: [KIT0004]
---

## Description

When adding a single new syllogism (a new (premise, conclusion) training pair), use the Sherman-Morrison formula for rank-1 matrix updates instead of re-solving the full kernel regression. O(n^2) update instead of O(n^3) full re-solve.

Formula: (A + uv^T)^{-1} = A^{-1} - (A^{-1} u v^T A^{-1}) / (1 + v^T A^{-1} u)

For n ~ 3,500 entities (Bible + Summa), this is ~12M operations -- near-instantaneous on M2.

Multiple new syllogisms can be batched using the Woodbury matrix identity (rank-k update): (A + UCV)^{-1} = A^{-1} - A^{-1} U (C^{-1} + V A^{-1} U)^{-1} V A^{-1}.

This is one of the strongest arguments for the kernel ridge regression approach: incremental updates are mathematically principled and computationally cheap. It enables real-time dynamic recompilation -- when a new syllogism is parsed, its reasoning operator can be incorporated immediately.

## API Sketch

```c
vacuum sherman_morrison_renova(duplex *inversa, longus n, duplex *u, duplex *v);
vacuum woodbury_renova(duplex *inversa, longus n, duplex *U, duplex *V, longus k);
```

~80 LOC for Sherman-Morrison, ~150 LOC with Woodbury.
