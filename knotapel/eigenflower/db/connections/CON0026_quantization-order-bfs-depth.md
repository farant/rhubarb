---
id: CON0026
title: "GPTQ quantization order parallels DKC BFS depth ordering and axis cancellation"
connects:
  - "GPTQ min-pivot quantization ordering on Hessian lattice"
  - "DKC BFS depth as fundamental computational resource with axis cancellation (D82, D90)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 6"
---

In GPTQ, quantization order determines error propagation. The greedy "orthogonal walk" produces different errors depending on dimension ordering. In DKC, BFS generation order determines algebraic coherence: deep entries (depth 8, 476 directions, 19 angles) outperform strided entries (maximum diversity, 512 directions, 43 angles) because they share BFS ancestry.

Both exhibit ordering-dependent resources: GPTQ's resource is error reduction per step; DKC's is computational capacity per entry. Both find that greedy ordering respecting algebraic structure outperforms naive orderings.

The D90 axis cancellation mechanism is particularly suggestive. GPTQ's error propagation projects the residual onto the next hyperplane -- canceling the component aligned with the just-quantized dimension. DKC's depth law works because deeper entries undergo more axis cancellations (anti-aligned rotation axes summing to small resultants). Both are greedy cancellation processes on lattice-structured spaces.
