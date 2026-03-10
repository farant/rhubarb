---
id: REF0013
title: "DKC Demo 106 — Topological Entropy Null"
type: dkc_demo
source_project: knotapel
demo_number: 106
summary: "Topological entropy is completely orthogonal to DKC computation. Both periodic and pseudo-Anosov braids compute XOR at 100% participation. Computation is algebraic/relational, not dynamical. The TL visibility filter explains why: most braid structure is invisible to the trace operation."
related: []
---

## Description

Demo 106 directly tested whether dynamical complexity (topological entropy, the Lyapunov exponent analog for braids) predicts DKC computational capacity. Result: completely orthogonal. Both periodic braids (zero entropy) and pseudo-Anosov braids (positive entropy) compute XOR at 100% participation rate. The TL quotient at delta=0 erases all dynamical information while preserving algebraic structure.

The TL visibility filter explains the mechanism: the trace operation projects the full braid representation matrix onto a single number, killing all dynamical/topological information while preserving all computational information (axis alignment, nesting parity via D107).

## Relevance to Eigenflower

- FALSIFIES the naive application of Edge-of-Chaos complexity measures to algebraic systems
- Establishes that the correct complexity measure for DKC (and by extension algebraic reasoning architectures) is ALGEBRAIC (derived series depth, solvability, BFS depth, Galois orbits) not DYNAMICAL (Lyapunov, topological entropy)
- This is a genuine refinement of the Edge-of-Chaos thesis -- potentially publishable
- The TL visibility filter is structurally identical to MicroDiT's deferred masking: process all information, then mask to retain only the computationally relevant projection
- For Eigenflower: if building an algebraic reasoning layer, optimize for algebraic structure quality, not dynamical complexity
