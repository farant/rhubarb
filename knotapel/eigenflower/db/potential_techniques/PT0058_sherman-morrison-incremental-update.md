---
id: PT0058
title: "Sherman-Morrison formula for incremental syllogism compilation"
status: promoted_to_kit
promoted_to: KIT0005
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012]
---

When adding a single new syllogism (a new (premise, conclusion) training pair), use the Sherman-Morrison formula for rank-1 matrix updates instead of re-solving the full kernel regression. This gives O(n^2) update instead of O(n^3) full re-solve.

Formula: (A + uv^T)^{-1} = A^{-1} - (A^{-1} u v^T A^{-1}) / (1 + v^T A^{-1} u)

For eigenflower: this enables real-time dynamic recompilation. When a new syllogism is parsed, its reasoning operator can be incorporated into the existing kernel matrix in O(n^2) time. For n ~ 3,500 entities (Bible + Summa), this is ~12M operations -- near-instantaneous on M2.

Multiple new syllogisms can be batched using the Woodbury matrix identity (rank-k update): (A + UCV)^{-1} = A^{-1} - A^{-1} U (C^{-1} + V A^{-1} U)^{-1} V A^{-1}.

This is one of the strongest arguments for the kernel ridge regression approach: incremental updates are mathematically principled and computationally cheap.
