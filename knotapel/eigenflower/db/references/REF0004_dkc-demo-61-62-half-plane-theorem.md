---
id: REF0004
title: "DKC Demos 61-62 — 11/13 Half-Plane Theorem"
type: dkc_theorem
source_project: knotapel
demo_number: 62
summary: "For ALL half-plane activations (MLP-equivalent), exactly 11/13 NPN classes are reachable. 0x06 fails via parallelogram (local obstruction), 0x1B via interval-squeeze (global impossibility). Proven analytically."
related: []
---

## Description

Demos 61-62 proved analytically that half-plane activations (the DKC analog of a single linear layer + threshold, i.e. MLP) can compute exactly 11 of the 13 NPN Boolean function classes. The two unreachable classes (0x06 and 0x1B) fail for fundamentally different geometric reasons: 0x1B via interval-squeeze (a global impossibility -- no arrangement of half-plane boundaries can separate the required truth-table values) and 0x06 via parallelogram obstruction (a local constraint violation). The proof uses pure integer arithmetic with zero floating-point.

## Relevance to Eigenflower

- Proves analytically WHY MLP-equivalent architectures have provable limits -- directly relevant to architecture choices for the fluency layer
- The convexity thesis: half-planes are convex, and the two unreachable NPN classes require non-convex decision boundaries. This constrains what kinds of reasoning can be implemented by simple linear classifiers
- Parallels TRM's finding that MLP suffices for fixed-size tasks but attention is needed for variable-size -- DKC provides the formal proof of why
- Cross-ring universality: the 11/13 wall was verified independently at three evaluation levels with three different rings before being proved analytically
