# DKC Fast-Onboarding Briefing

## What Is This

Discrete Knotted Computation (DKC) compiles topological invariants -- Kauffman bracket values evaluated at roots of unity -- into neural network weights that compute Boolean functions without training. At A = e^{i*5pi/4}, delta = 0, and every bracket value is a cyclotomic integer in Z[zeta_8] with exact arithmetic (zero floating-point error, ever). Over 101 demos we proved: forward DKC works (D29), all 13 NPN Boolean classes are computable (D45-50), the parity wall is in activation geometry not the weight lattice (D48-50, D61-62), capacity scales linearly with braid crossing depth (D82), the +/-q encoding is structurally locked to parity (D92, formally proven), the circuit complexity hierarchy manifests as AND/XOR ratio 1→2762→∞ (D93, LANDMARK), solvability is the capacity bottleneck (D94, Barrington confirmed), the optimal z8 catalog is 21 entries not 24 (D96, LANDMARK), Cell B perfection is geometric inevitability from orthogonal frames (D97), and multi-strand DKC works -- first-ever 3-strand XOR via irreducible Delta_1 module, XOR14=60 (D99, LANDMARK). Non-semisimple extensions are computationally load-bearing: D100's W_{4,2} (radical) achieves XOR14=70 while D101's W_{5,3} (simple) caps at XOR12. The sl_d functor thesis (BFS growth ~d*x/round) is confirmed at d=2,3,4. Five published pillars are CONNECTED through DKC as a "discrete algebraic reservoir computer."

## Key Results

| # | Result | Demo | Status |
|---|--------|------|--------|
| 1 | Forward DKC: exact Z[zeta_8] bracket values compute XOR (100+ triples, RMS=0.000) | D29 | PROVEN |
| 2 | All 13 NPN classes computable via MVN activation; parity at k=6 (906 solutions) | D45-50 | PROVEN |
| 3 | 11/13 half-plane theorem: 0x06 and 0x1B unreachable by ANY half-plane activation | D61-62 | PROVEN |
| 4 | Radical dimension: rad(TL_l) = 2l-3 via Graham-Lehrer cellular theory | D51-52,60 | PROVEN |
| 5 | Linear depth law: max_xor ~ depth + 6; algebraic coherence beats raw vocabulary | D82 | DEMONSTRATED |
| 6 | b = -5/8 (Pearce-Rasmussen): first exact first-principles indecomposability parameter | D85-86 | DEMONSTRATED |
| 7 | Null dispensability regime transition: nulls critical at zeta_8 (finite), dispensable at zeta_12 (infinite) | D87 | DEMONSTRATED |
| 8 | Depth law mechanism: axis cancellation + cross-depth algebraic constraint; relational, not positional | D89-91 | DEMONSTRATED |
| 9 | Parity-lock theorem: +/-q encoding structurally locked to XOR/XNOR; AND/OR/MAJ impossible | D92 | **PROVEN (P04)** |
| 10 | Framing = +2 XOR levels: Jones normalization destroys exactly 2 levels at every root tested | D83 | DEMONSTRATED |
| 11 | Complement-blindness theorem: combined_cell maps m↔~m to same cell; phase_cell recovers all 13 NPN | D93 | **PROVED** |
| 12 | Circuit complexity hierarchy: AND/XOR ratio 1.01→2762→∞ (N=3→8); pigeonhole kills XOR at N≥7 | D93 | DEMONSTRATED |
| 13 | Solvability bottleneck: non-solvable 2I beats solvable z8 at matched size (advantage 1.07x→1.67x) | D94 | CONFIRMED |
| 14 | Optimal z8 = A+B+C (21 entries): removing 3 Q8-null entries IMPROVES capacity | D96 | DEMONSTRATED |
| 15 | Cell B perfection: 3 orthogonal directions at 45° = 100% XOR at all N; 50° robustness plateau | D97 | DEMONSTRATED |
| 16 | First 3-strand XOR: Delta_1 irreducible module over Z[zeta_8], XOR14=60, XOR16=0 ceiling | D99 | **LANDMARK** |
| 17 | Ext^1 thesis: non-semisimple extensions are computationally load-bearing (D100 XOR14=70 vs D101 XOR14=0) | D99-101 | DEMONSTRATED |
| 18 | sl_d growth rate: BFS ~d*x per round confirmed at d=2,3,4 (three data points) | D99-101 | CONFIRMED |

## Current Frontier

- **Multi-strand DKC works (D98-D101):** 3-strand Delta_1 achieves XOR14=60. 4-strand W_{4,2} (non-semisimple) achieves XOR14=70. 5-strand W_{5,3} (simple) caps at XOR12. Non-semisimple extensions are computationally load-bearing — the radical is not decorative.
- **Catalog optimization (D95-D97):** Derived series and null/non-null are INDEPENDENT partitions. Optimal z8 = 21 entries (removing 3 Q8-null). Cell B perfection is geometric (orthogonal frames). First principled catalog pruning.
- **Key open question:** Does the sl_d functor thesis (BFS growth ~d*x/round) continue at d=5 (W_{6,4})? This is the decisive test for multi-strand scaling.
- **Next experiments:** (1) 6-strand W_{6,4} — is growth ~5x? Does non-semisimplicity matter? (2) RKHS kernel rank — rank(K_2I) vs rank(K_z8). (3) Ext^1 formalization — prove non-semisimple is strictly more powerful.
- **Paper pipeline:** Paper 7 (mechanistic arc D85-D92), Paper 8 (circuit complexity D93-D94), Paper 9 (multi-strand arc D98-D101). Papers 1-6 at various stages.

## Deeper Reading

All paths relative to `knotapel/`:

- **Full narrative:** `atlas/synthesis/narrative.md` -- the story from D01 to D101
- **Key results & proofs:** `atlas/inventory/theorems.md` | `atlas/inventory/proofs-index.md`
- **Demo index:** `atlas/inventory/demo-index.md` -- all 101 demos with one-line summaries
- **What's next:** `atlas/planning/next-priorities.md` -- tiered priority list with critical paths
- **Cross-demo patterns:** `atlas/synthesis/connections.md` | `atlas/synthesis/four-lenses.md`
- **Novelty claims:** `atlas/synthesis/novelty.md`
- **Open questions & vision:** `atlas/planning/research-questions.md` | `atlas/planning/vision.md`
