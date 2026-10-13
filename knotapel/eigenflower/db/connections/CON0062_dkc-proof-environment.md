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

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

DKC is not just "relevant to" these papers -- DKC is a concrete, exactly-analyzable instance of the systems they describe theoretically. Tensor logic, Hilbert space operators, reservoir computing, compressed sensing, edge-of-chaos dynamics -- DKC already implements all of these with exact arithmetic.

DKC provides something no one else has: a mathematical PROOF ENVIRONMENT for neural computation ideas. If you want to know whether ternary recursive models should work for logical reasoning, DKC can give a provable answer via depth law and parity-lock theorem, rather than an empirical one via training runs.

The papers converge on something real:
- Domingos tensor logic + Hilbert space operators = theory
- BitNet + TRM = architecture
- GPTQ-as-CVP = compilation mechanism
- Edge of Chaos = corpus suitability
- The pieces fit -- not a Frankenstein assembly

The exact arithmetic advantage is decisive: all computations are zero-error over algebraic integer rings, eliminating the entire class of approximation-based objections that theoretical frameworks usually face.
