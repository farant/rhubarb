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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

In GPTQ, quantization order determines error propagation. The greedy "orthogonal walk" produces different errors depending on dimension ordering. In DKC, BFS generation order determines algebraic coherence: deep entries (depth 8, 476 directions, 19 angles) outperform strided entries (maximum diversity, 512 directions, 43 angles) because they share BFS ancestry.

Both exhibit ordering-dependent resources: GPTQ's resource is error reduction per step; DKC's is computational capacity per entry. Both find that greedy ordering respecting algebraic structure outperforms naive orderings.

The D90 axis cancellation mechanism is particularly suggestive. GPTQ's error propagation projects the residual onto the next hyperplane -- canceling the component aligned with the just-quantized dimension. DKC's depth law works because deeper entries undergo more axis cancellations (anti-aligned rotation axes summing to small resultants). Both are greedy cancellation processes on lattice-structured spaces.
