---
id: REF0012
title: "DKC Demo 96 — Optimal z8 Catalog (21 Entries, LANDMARK)"
type: dkc_demo
source_project: knotapel
demo_number: 96
summary: "The optimal z8 catalog is 21 entries (A+B+C subset), not the full 24. Removing 3 Q8-null entries IMPROVES computational capacity. More entries is not always better -- algebraic coherence matters more than vocabulary size."
related: []
---

## Description

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level it finds no XOR14 at depths 7–8 (exact cells). D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" is an entry-order effect: in other orders, strided and random 564-entry sets reach XOR12 too. D87's ζ₁₂ null dispensability holds exactly. Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Demo 96 (TL-Group Cross-Classification, LANDMARK) discovered that removing 3 Q8-null entries from the 24-entry binary octahedral group catalog to get the A+B+C subset (21 entries) IMPROVES computational capacity. The removed entries introduce noise that hurts the algebraic coherence needed for parity computation. This parallels the GPTQ finding that 3.125 bits is Pareto-optimal -- the "natural" resolution is smaller than naively expected.

## Relevance to Eigenflower

- Demonstrates the "less is more" principle with exact algebraic proof: structured pruning beats exhaustive inclusion
- Parallels NVFP4's finding that 16 representable values suffice -- what matters is algebraic structure, not numerical resolution
- The Q8-null entries are algebraically "low-importance" directions analogous to low-eigenvalue Hessian directions in GPTQ
- Directly informs Eigenflower's weight/vocabulary design: use structured, algebraically coherent value sets rather than maximally diverse ones
- The 21-element optimal set could be a natural "ternary-plus" alphabet for DKC-inspired weight quantization
