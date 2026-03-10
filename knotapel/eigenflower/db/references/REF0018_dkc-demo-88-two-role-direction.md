---
id: REF0018
title: "DKC Demo 88 — Two-Role Direction Theorem"
type: dkc_demo
source_project: knotapel
demo_number: 88
summary: "4 'rigid anchor' directions (body-diagonals, 8% XOR loss under 10-degree perturbation) and 6 'flexible scaffolding' directions (edge-midpoints, slight GAIN under perturbation). K-ladder activation provides 14x better perturbation resilience than Voronoi-only."
related: []
---

## Description

Demo 88 established the two-role direction theorem for DKC's computational geometry. The 13 eigenvector directions partition into two functional roles:
- 4 rigid anchors (body-diagonal directions): must not be perturbed; 10-degree displacement costs 8% XOR capacity
- 6 flexible scaffolding (edge-midpoint directions): tolerate or even benefit from perturbation

The k-ladder activation (trying multiple resolutions k=6, k=12, k=24 and taking the best) provides 14x better perturbation resilience than Voronoi-only.

## Relevance to Eigenflower

- Explains WHY TRM's EMA stabilization works at a structural level: EMA protects rigid anchor weights while allowing gradual adjustment of scaffolding weights
- Suggests position-dependent EMA decay: high decay for anchor-like weights, lower for scaffolding-like weights
- The constrained optimization approach (nulls free, non-nulls clamped) provides a template for structured weight updates in Eigenflower's fluency layer
- Informs structured sparsity patterns for BitNet: some zero-weight positions should be "locked" (structural support), others "flexible"
