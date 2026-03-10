---
id: PT0072
title: "Kernelized relation reasoning via tensor product space"
status: untested
source: ["Hilbert Space paper (Kiruluta et al., 2025)"]
applicable_to: []
related: [PT0012, PT0013]
---

For binary relation R on domain X, define kernel K_R((x,y),(x',y')) = <Phi(x) tensor Phi(y), Phi(x') tensor Phi(y')>. Reasoning over chains (x -> y -> z) = composition in tensor product space: K_{R2 o R1}(x,z) = integral K_{R1}(x,y) K_{R2}(y,z) d mu(y).

This mirrors tensor-product kernels in knowledge base completion. The composition integral gives transitive inference: if R1 relates x to y and R2 relates y to z, the composed kernel relates x to z.

For eigenflower: each syllogistic relation (All-A-are-B, Some-A-are-B, etc.) defines a kernel. Chaining syllogisms = composing kernels in tensor product space. The kernel captures both the relational structure and the entity representations.

The continuous integral over intermediate entities (y) handles the "binding" problem -- which middle term connects the premises. The kernel automatically marginalizes over all possible middle terms weighted by their relevance.

Implementation: kernel evaluations are inner products, kernel compositions are integrals approximated by sums over training data. All standard numerical operations.
