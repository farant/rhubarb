# DKC Fast-Onboarding Briefing

## What Is This

Discrete Knotted Computation (DKC) compiles topological invariants -- Kauffman bracket values evaluated at roots of unity -- into neural network weights that compute Boolean functions without training. At A = e^{i*5pi/4}, delta = 0, and every bracket value is a cyclotomic integer in Z[zeta_8] with exact arithmetic (zero floating-point error, ever). Over 109 demos we proved: forward DKC works (D29), all 13 NPN Boolean classes are computable (D45-50), the parity wall is in activation geometry not the weight lattice (D48-50, D61-62), capacity scales linearly with braid crossing depth (D82), the +/-q encoding is structurally locked to parity (D92, formally proven), the circuit complexity hierarchy manifests as AND/XOR ratio 1->2762->inf (D93, LANDMARK), solvability is the capacity bottleneck (D94, Barrington confirmed), the optimal z8 catalog is 21 entries not 24 (D96, LANDMARK), Cell B perfection is geometric inevitability from orthogonal frames (D97), and multi-strand DKC works -- first-ever 3-strand XOR via irreducible Delta_1 module, XOR14=60 (D99, LANDMARK). The radical-as-resource hypothesis was REFUTED: the radical carries only abelian writhe data (Barrington-Radical Principle, D102). BFS growth is a braid group invariant tracking strand count, not module dimension (D103). The Atkinson sweet spot shows 3-strand beats 4-strand at matched catalog (D104). "XOR dies at N>=7" was REFUTED as a k-regime artifact (D105, macrame principle). Topological entropy is completely orthogonal to DKC -- computation is algebraic/relational, not dynamical (D106). Universal axis-alignment PROVEN with constructive Z/4Z phase formula, plus nesting parity Z/2Z grading and q-Catalan identity (D107). The Dual-Channel Theorem establishes that parity requires BOTH phase coherence AND magnitude diversity, with encoding-dependent polarity inversion (D108-D109). Five published pillars are CONNECTED through DKC as a "discrete algebraic reservoir computer."

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
| 11 | Complement-blindness theorem: combined_cell maps m<->~m to same cell; phase_cell recovers all 13 NPN | D93 | **PROVED** |
| 12 | Circuit complexity hierarchy: AND/XOR ratio 1.01->2762->inf (N=3->8); pigeonhole kills XOR at N>=7 | D93 | DEMONSTRATED |
| 13 | Solvability bottleneck: non-solvable 2I beats solvable z8 at matched size (advantage 1.07x->1.67x) | D94 | CONFIRMED |
| 14 | Optimal z8 = A+B+C (21 entries): removing 3 Q8-null entries IMPROVES capacity | D96 | DEMONSTRATED |
| 15 | Cell B perfection: 3 orthogonal directions at 45 deg = 100% XOR at all N; 50 deg robustness plateau | D97 | DEMONSTRATED |
| 16 | First 3-strand XOR: Delta_1 irreducible module over Z[zeta_8], XOR14=60, XOR16=0 ceiling | D99 | **LANDMARK** |
| 17 | Barrington-Radical Principle: radical carries abelian writhe character, provably useless for parity | D102 | **PROVEN** |
| 18 | BFS growth is braid group invariant: ~(n-1)x/round, independent of module dimension (4 data points) | D99-103 | CONFIRMED |
| 19 | "XOR dies at N>=7" REFUTED: k-regime artifact; W_{8,0} XOR8=22266 at k=4096; macrame principle | D105 | **REFUTED** |
| 20 | Topological entropy null: computation is algebraic/relational; TL visibility filter explains why | D106 | DEMONSTRATED |
| 21 | Z/4Z Axis-Alignment Theorem: constructive proof + nesting parity Z/2Z grading (NOVEL) | D107 | **PROVEN** |
| 22 | Dual-Channel Theorem: parity requires phase coherence + magnitude diversity; polarity encoding-dependent | D108-109 | DEMONSTRATED |

## Current Frontier

- **Raqiya diagnostics arc complete (D106-D109):** Topological entropy is orthogonal to DKC (TL visibility filter). Axis-alignment proven universally. Dual-channel theorem predicts parity capability from algebraic graph structure, with encoding-dependent polarity inversion at delta=sqrt(2).
- **Multi-strand framework settled (D102-D105):** Radical carries only abelian writhe (Barrington-Radical Principle). BFS growth is a braid group invariant. Atkinson sweet spot at ~120 components for dim-9. k/2^N ratio controls XOR viability (k-regime theory). Macrame principle: branching interaction reverses between dim 5+4 (cross-block) and dim 14+14 (within-block).
- **Key open questions:** (1) Does the dual-channel polarity inversion follow a general encoding rule? Additive encoding at delta=sqrt(2) is the single controlled test. (2) Amy Bridge: does the dual-channel map onto T-gate/Hadamard decomposition in quantum circuit synthesis? (3) RKHS kernel rank: does rank(K_2I)/rank(K_z8) > 120/24, validating the reservoir computing interpretation?
- **Next experiments:** (1) Additive encoding at delta=sqrt(2) -- confirms polarity inversion (SMALL). (2) RKHS kernel rank test -- one number validates five-pillar synthesis (SMALL). (3) Amy Bridge formalization -- connects DKC to QCS resource theory (MEDIUM). (4) Major index theorem at W_{8,2} -- novel algebraic invariant (SMALL).
- **Paper pipeline:** Papers 1-6 at various stages. Paper 7 (mechanistic arc D85-D92), Paper 8 (circuit complexity D93-D94), Paper 9 (multi-strand arc D95-D105), Paper 10 (Raqiya/diagnostics arc D106-D109, NEW).

## Deeper Reading

All paths relative to `knotapel/`:

- **Full narrative:** `atlas/synthesis/narrative.md` -- the story from D01 to D109
- **Key insights (10 levels):** `atlas/synthesis/key-insights.md` -- progressive depth from one sentence to one paragraph per idea
- **Arc summaries:** `atlas/synthesis/arc-summaries.md` -- 20 research arcs with question/answer/journey/artifacts
- **Key results & proofs:** `atlas/inventory/theorems.md` | `atlas/inventory/proofs-index.md`
- **Demo index:** `atlas/inventory/demo-index.md` -- all 109 demos with one-line summaries
- **What's next:** `atlas/planning/next-priorities.md` -- tiered priority list with critical paths and conjecture-graph rankings
- **Prerequisites & glossary:** `atlas/synthesis/prerequisites.md` | `atlas/synthesis/glossary.md`
- **Cross-demo patterns:** `atlas/synthesis/connections.md` | `atlas/synthesis/four-lenses.md`
- **Novelty claims:** `atlas/synthesis/novelty.md`
- **Open questions & vision:** `atlas/planning/research-questions.md` | `atlas/planning/vision.md`
