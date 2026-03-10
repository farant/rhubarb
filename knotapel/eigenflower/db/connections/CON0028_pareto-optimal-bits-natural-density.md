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

Both demonstrate that the "natural" resolution/vocabulary is smaller than naively expected, and that removing elements actively improves performance.

HPTQ's 3.125 bits means ~9 distinct values per weight -- fewer than E2M1's 16 or INT8's 256, yet Pareto-optimal. DKC's 21 entries is fewer than the full 24-cell vertex set (or the 100-value catalog), yet optimal.

The mechanism differs but the principle is the same: algebraic structure creates redundancies that should be exploited rather than preserved. In HPTQ, entropy coding assigns shorter codes to more-likely values. In DKC, Q8-null entries interfere with algebraic coherence needed for parity.

The connection to GPTQ's error bound is direct: the tight bound 1/4 * trace(D) depends on lattice geometry via LDL diagonal, not raw number of representable values. Adding lattice points in "low-D" directions wastes bits -- same as DKC's Q8-null entries lying in algebraically low-importance directions.
