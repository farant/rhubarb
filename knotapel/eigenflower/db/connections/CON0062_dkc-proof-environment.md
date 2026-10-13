---
id: CON0062
title: "DKC as mathematical proof environment for neural computation ideas"
connects:
  - "All eigenflower papers: empirically validated architectures"
  - "DKC: exact algebraic computation with provable properties across 109 demos"
type: cross_pollination
strength: strong
source: "notes/01-analysis.md, Part 3 (Central Observation)"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

DKC is not just "relevant to" these papers -- DKC is a concrete, exactly-analyzable instance of the systems they describe theoretically. Tensor logic, Hilbert space operators, reservoir computing, compressed sensing, edge-of-chaos dynamics -- DKC already implements all of these with exact arithmetic.

DKC provides something no one else has: a mathematical PROOF ENVIRONMENT for neural computation ideas. If you want to know whether ternary recursive models should work for logical reasoning, DKC can give a provable answer via depth law and parity-lock theorem, rather than an empirical one via training runs.

The papers converge on something real:
- Domingos tensor logic + Hilbert space operators = theory
- BitNet + TRM = architecture
- GPTQ-as-CVP = compilation mechanism
- Edge of Chaos = corpus suitability
- The pieces fit -- not a Frankenstein assembly

The exact arithmetic advantage is decisive: all computations are zero-error over algebraic integer rings, eliminating the entire class of approximation-based objections that theoretical frameworks usually face.
