# DKC Fast-Onboarding Briefing

## What Is This

Discrete Knotted Computation (DKC) compiles topological invariants -- Kauffman bracket values evaluated at roots of unity -- into neural network weights that compute Boolean functions without training. At A = e^{i*5pi/4}, delta = 0, and every bracket value is a cyclotomic integer in Z[zeta_8] with exact arithmetic (zero floating-point error, ever). Over 92 demos we proved: forward DKC works (D29), all 13 NPN Boolean classes are computable (D45-50), the parity wall is in activation geometry not the weight lattice (D48-50, D61-62), capacity scales linearly with braid crossing depth (D82), and the +/-q encoding is structurally locked to parity -- the single Boolean function outside AC^0 (D92, formally proven). The mechanism is now understood: axis cancellation and cross-depth algebraic constraints produce a relational depth law where deep entries are individually less informative but collectively more powerful (D89-91). Five published pillars (Abramsky, Habiro, Aizenberg, Nazer-Gastpar, Reservoir Computing) are each established; their intersection is unoccupied.

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

## Current Frontier

- **Just proved (D92):** The parity-lock theorem -- the +/-q encoding concentrates ALL computational power on parity, the hardest Boolean function (outside AC^0). Formal proof via 3^k equivalence class argument.
- **Key open question:** Does the depth law generalize beyond parity? The 1wpi encoding (D48/D50) computes all 13 NPN classes but has never been tested for depth-law scaling. This determines whether the compilation cost model extends to AND/OR for syllogistic logic.
- **Next experiments:** (1) 1wpi encoding depth law test -- does max_and or max_maj scale linearly with depth? (2) Balanced exponentials analytical proof -- formalize BFS ~2x/round vs parity 4x/weight. (3) Regime transition characterization -- locate the exact null-dispensability threshold between zeta_8 and zeta_12.
- **Paper pipeline:** Paper 7 (D85-D92 mechanistic arc: "why does DKC work and what are its limits?") is the immediate publication target. Papers 1-6 are at various stages of readiness (see next-priorities.md for critical paths).
- **Architectural implication:** Encoding is more fundamental than activation or catalog -- it determines which functions are computable at all (D92). This is the first design decision in any DKC pipeline.

## Deeper Reading

All paths relative to `knotapel/`:

- **Full narrative:** `atlas/synthesis/narrative.md` -- the story from D01 to D92
- **Key results & proofs:** `atlas/inventory/theorems.md` | `atlas/inventory/proofs-index.md`
- **Demo index:** `atlas/inventory/demo-index.md` -- all 92 demos with one-line summaries
- **What's next:** `atlas/planning/next-priorities.md` -- tiered priority list with critical paths
- **Cross-demo patterns:** `atlas/synthesis/connections.md` | `atlas/synthesis/four-lenses.md`
- **Novelty claims:** `atlas/synthesis/novelty.md`
- **Open questions & vision:** `atlas/planning/research-questions.md` | `atlas/planning/vision.md`
