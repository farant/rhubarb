---
id: REF0020
title: "DKC Demo 97 — Cell B Perfection"
type: dkc_demo
source_project: knotapel
demo_number: 97
summary: "6 entries (3 orthogonal directions at 45-degree half-angle) achieve 100% XOR at all N tested. Geometric inevitability from orthogonal frames. 50-degree robustness plateau."
related: []
---

## Description

Demo 97 explained why Cell B (the D^0-nonnull subset identified in D96) achieves perfect 100% XOR capacity. The mechanism is geometric inevitability: 3 orthogonal directions at exactly 45-degree half-angle create the optimal separation geometry. This is the minimum sufficient structure -- 6 entries from a catalog of 21 suffice for maximal computation.

The 50-degree robustness plateau means the structure tolerates significant angular perturbation before degrading -- it is not a fragile optimum but a robust geometric basin.

## Relevance to Eigenflower

- Demonstrates that minimal sufficiency (6 entries with right structure) can match or exceed exhaustive inclusion (21 or 100 entries)
- Parallels Paper 2's (Early Experience) 1/8 data efficiency: structured data dramatically outperforms brute-force quantity
- The robustness plateau suggests that exact precision is not required for the geometric structure to work -- relevant to low-precision training feasibility
- With perfect algebraic structure, even the simplest activation achieves 100% -- the ultimate "narration over pre-compiled knowledge" result
