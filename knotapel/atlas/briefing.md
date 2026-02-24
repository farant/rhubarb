# DKC Fast-Onboarding Briefing

## What Is This

Discrete Knotted Computation (DKC) compiles topological invariants -- Kauffman bracket values evaluated at roots of unity -- into neural network weights that compute Boolean functions without training. At A = e^{i*5pi/4}, delta = 0, and every bracket value is a cyclotomic integer in Z[zeta_8] with exact arithmetic (zero floating-point error, ever). Over 94 demos we proved: forward DKC works (D29), all 13 NPN Boolean classes are computable (D45-50), the parity wall is in activation geometry not the weight lattice (D48-50, D61-62), capacity scales linearly with braid crossing depth (D82), the +/-q encoding is structurally locked to parity (D92, formally proven), the combined_cell activation is complement-blind under 1wpi encoding (D93, proved), and the circuit complexity hierarchy (AC^0/TC^0/parity) manifests as hit-rate divergence: AND/XOR ratio 1→2762→∞ (D93, LANDMARK). The binary icosahedral group 2I confirms solvability as the capacity bottleneck predicted by Barrington's theorem (D94). Five published pillars (Abramsky, Habiro, Aizenberg, Nazer-Gastpar, Reservoir Computing) are now CONNECTED through DKC as a "discrete algebraic reservoir computer" — their intersection was unoccupied; we occupy it.

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

## Current Frontier

- **Milestone (D94):** Five-pillar synthesis COMPLETE — DKC is a "discrete algebraic reservoir computer." Abramsky (TL=computation), Habiro (bracket=cyclotomic), Aizenberg (MVN=readout), Nazer-Gastpar (lattice=coding), RC (depth=capacity). Their intersection was unoccupied; we occupy it.
- **Just confirmed (D93-D94):** Circuit complexity hierarchy is universal across group structures. Solvability (Barrington) is the capacity bottleneck. Phase_cell activation recovers all functions.
- **Key open question:** Does depth law hold under phase_cell? D95 (RKHS kernel rank) is the single highest-leverage experiment — one number validates the reservoir interpretation.
- **Next experiments:** (1) D95 RKHS kernel rank test — rank(K_2I)/rank(K_z8) > 120/24? (2) Higher k_sec with 2I — does 384-cell space fully exploit 2I? (3) Depth law under phase_cell — does max_xor ≈ depth+6 generalize?
- **Paper pipeline:** Paper 7 (D85-D92 mechanistic arc), Paper 8 (D93-D94 circuit complexity + algebraic reservoir). Papers 1-6 at various stages.

## Deeper Reading

All paths relative to `knotapel/`:

- **Full narrative:** `atlas/synthesis/narrative.md` -- the story from D01 to D94
- **Key results & proofs:** `atlas/inventory/theorems.md` | `atlas/inventory/proofs-index.md`
- **Demo index:** `atlas/inventory/demo-index.md` -- all 94 demos with one-line summaries
- **What's next:** `atlas/planning/next-priorities.md` -- tiered priority list with critical paths
- **Cross-demo patterns:** `atlas/synthesis/connections.md` | `atlas/synthesis/four-lenses.md`
- **Novelty claims:** `atlas/synthesis/novelty.md`
- **Open questions & vision:** `atlas/planning/research-questions.md` | `atlas/planning/vision.md`
