---
id: CON0028
title: "HPTQ 3.125-bit Pareto optimality parallels DKC optimal catalog being smaller than expected"
connects:
  - "HPTQ 3.125 average bits per weight as Pareto-optimal across model sizes"
  - "DKC optimal z8 catalog: 21 entries, not 24 -- removing elements improves performance (D96)"
type: structural_parallel
strength: moderate
source: "extractions/group-c-knotapel-relevance.md, Connection 8"
---

> **Correction (2026-10-12, Demo 125):** D82's "linear depth law" (max_xor ≈ depth + 6) is not supported above depth 6: the D81/D82 search never went past XOR12, and extended one level under the exact rule (from its first 32 XOR12 winners; robust not extended) it finds no XOR14 at depths 7–8 — a statement about that search, not the catalog. D82's "algebraic coherence beats vocabulary" / "deep entries 2× more efficient" does not survive a change of entry order: in other seeded orders strided 564 reaches XOR12 as often as deep 564 (robustly 8/10 vs 7/10), and so do 9/10 random 564-entry samples (mostly rounds 6–8). D87's ζ₁₂ result holds exactly, as far as its search reaches (both catalogs at the XOR12 ceiling). Statements here that rely on the depth law or on coherence are superseded; the D89–D93 "mechanisms" are not yet audited. See `knotapel/demo_125_exact_crossing_depth/findings.md`.

Both demonstrate that the "natural" resolution/vocabulary is smaller than naively expected, and that removing elements actively improves performance.

HPTQ's 3.125 bits means ~9 distinct values per weight -- fewer than E2M1's 16 or INT8's 256, yet Pareto-optimal. DKC's 21 entries is fewer than the full 24-cell vertex set (or the 100-value catalog), yet optimal.

The mechanism differs but the principle is the same: algebraic structure creates redundancies that should be exploited rather than preserved. In HPTQ, entropy coding assigns shorter codes to more-likely values. In DKC, Q8-null entries interfere with algebraic coherence needed for parity.

The connection to GPTQ's error bound is direct: the tight bound 1/4 * trace(D) depends on lattice geometry via LDL diagonal, not raw number of representable values. Adding lattice points in "low-D" directions wastes bits -- same as DKC's Q8-null entries lying in algebraically low-importance directions.
