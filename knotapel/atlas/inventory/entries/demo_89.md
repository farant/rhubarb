## Demo 89: Depth Law Mechanism

- **Status**: PRELIMINARY
- **File**: `demo_89_depth_law_mechanism/main.c` (~1964 lines)
- **Tests**: 14/14 pass (12 phase completions + catalog built + direction coverage; Phase 5 intra>inter coherence FAIL is expected negative result, Phase 8 deep>strided Cayley FAIL is expected negative result)
- **Depends on**: Demo 82 (depth law: max_xor ~ depth + 6, algebraic coherence), Demo 84 (null states, direction geometry), Demo 66 (24-cell geometry, zeta_8 catalog), Demo 79 (zeta_12 capacity, infinite group), Demo 77 (paired extension at zeta_8)
- **Feeds into**: Demo 90 (sum-angle structure, axis cancellation mechanism), Demo 91 (activation bottleneck), future representation-theoretic analysis of "algebraic coherence"

### Headline
Systematic investigation of WHY the depth law holds, testing 8 mechanistic hypotheses across 11 computational phases — killing pairwise coherence, direction coverage, Cayley density, sector diversity, and angle coherence as explanations, while confirming paired extension as a real but minority pathway (6-20% at zeta_12 vs 100% at zeta_8) and identifying "algebraic coherence" as the remaining open question attacked in Demo 90.

### Key Results

- **Phase 1 — Depth decomposition (zeta_12)**: Single-depth XOR6 capacity grows with depth (d=0: 8 winners, d=8: 2011 winners at bf=24). Direction count grows exponentially across depth shells: 2->6->22->46->114->239->507->975->2043. Cumulative XOR6 capacity saturates around 1903.
- **Phase 2 — Discriminating experiment**: Deep single-depth entries (d>=4-5) beat random at matched sample size (N=20, N=50) by 5-10%. Shallow entries (d=2, d=3) LOSE to random. Confirms deep entries have genuine per-entry advantage.
- **Phase 3 — Pairwise coherence**: KILLED. Mean pairwise |qdot| is flat at ~0.42 across all depths, identical to random baseline. Depth-shell vs random-sample coherence ratio ~1.0. Quaternion alignment is NOT the mechanism.
- **Phase 4 — Direction coverage (zeta_8)**: KILLED as intercept explanation. All 13 directions in zeta_8 covered by depth 2. The +6 intercept relates to XOR6 base capacity, not direction coverage depth. Also classifies directions into null-only, shared, and non-null types with first-appearance depth.
- **Phase 5 — Cross-depth coherence matrix**: Intra-depth coherence (0.4214) < inter-depth (0.4254). Depth shells are MORE internally orthogonal than across shells — the opposite of what a coherence-based mechanism would predict. Expected negative result.
- **Phase 6 — Cell diversity**: Pairwise sum cell diversity grows EXPONENTIALLY with depth. log2(distinct_cells) ~ 1.17d + 2, which IS linear and tracks max_xor. Both directions and sectors contribute. ~66% fill ratio at k=6 regardless of depth. Quantitative correspondence is real but causality unclear (both may be driven by same underlying cause).
- **Phase 7 — Sector diversity**: KILLED. Pairwise sum sectors saturate at 4/6 (k=6), 7/12 (k=12), 13/24 (k=24). Voronoi directions always 100% covered. Sectors are NOT the bottleneck. Single-entry angle range: generator half-angle = pi/6 = 30deg, so max angle at depth d = 60d degrees.
- **Phase 8 — Cayley graph density**: KILLED and INVERTED. Same-depth entries have ZERO mutual Cayley edges (mathematically necessary from BFS construction). Deep-564 has 0 edges; strided-564 has 58. Yet deep beats strided on XOR capacity. Edges/entry converges to 4/3. Cross-depth connectivity: 100% of depth-d entries connected to depth-(d-1) entries.
- **Phase 9 — Multi-element sum sector diversity**: KILLED. Sectors slightly DECREASE with depth for multi-element sums. BUT: each additional weight adds ~2 sectors to coverage, matching the +2 XOR input step per additional weight.
- **Phase 10 — Angle coherence within shells**: KILLED for deep entries. Shallow shells (d=0, d=1) ARE angle-coherent (d=1 has half the variance of random). Deep shells (d>=3) have approximately the same angle variance as random. Distinct angles per depth listed explicitly. Full catalog: distinct rotation angles grow linearly by cumulative depth: 2->4->6->10->12->16->21->35->44 (~5/depth).
- **Phase 11 — Paired extension verification**: At zeta_12 cumulative depth 4 (275 entries, 114 dirs): XOR6 has 3736 winners, 234 (6%) with shadow pairs. XOR8 has 2200 winners, 430 (20%) with shadow pairs, 430/430 (100%) nested. Paired extension is REAL but MINORITY at zeta_12 — direction diversity provides an alternative dominant pathway. At zeta_8 (24 entries, 13 dirs) it was 100% because direction scarcity forces shadow pairing as the only viable strategy.

### Theorems/Conjectures

- **Pairwise Coherence Non-Mechanism (DEMONSTRATED)**: Mean |qdot| is flat (~0.42) across all depth shells and identical to random baseline. Quaternion alignment does not explain depth advantage.
- **Direction Coverage Non-Mechanism (DEMONSTRATED)**: All 13 zeta_8 directions covered by depth 2; the +6 intercept is not from direction coverage timing.
- **Cayley Density Non-Mechanism (DEMONSTRATED, INVERTED)**: Same-depth entries have zero Cayley edges by BFS construction. Deep subsets have FEWER Cayley edges than strided subsets of equal size, yet higher XOR capacity. Graph density and computational capacity are anti-correlated.
- **Sector Saturation Non-Mechanism (DEMONSTRATED)**: Pairwise sum sectors saturate early and do not grow with depth. Voronoi coverage is always 100%.
- **Angle Coherence Non-Mechanism (DEMONSTRATED for deep)**: Deep shells have random-like angle variance. Only shallow shells (d=0, d=1) show coherence.
- **Paired Extension as Minority Pathway (DEMONSTRATED)**: Shadow pairing is real (100% nesting when present) but constitutes only 6-20% of winners at zeta_12. Dominant pathway (80-94%) uses all-distinct-direction tuples.
- **Paired Extension / Diversity Duality (OBSERVED)**: Shadow pairing dominates when direction vocabulary is small (zeta_8); direction diversity dominates when large (zeta_12). Both depend on same underlying algebra.
- **Combinatorial-Algebraic Balance Conjecture (PROPOSED)**: Exponential catalog growth provides exponentially more candidate tuples per depth round; parity constraint is exponentially hard per XOR level; these two exponentials balance to give linear depth growth (one XOR level per ~2 depth rounds). Algebraic coherence tilts the balance favorably for deep entries.
- **log2(cell_diversity) ~ 1.17d + 2 (OBSERVED)**: Linear in depth, tracks max_xor quantitatively. Causality vs epiphenomenality unresolved.
- **+2 Sectors per Weight (OBSERVED)**: Each additional weight in multi-element sums adds ~2 sectors to coverage, matching the +2 XOR input step.
- **Casimir Balance Hypothesis (SUGGESTED by researcher)**: Catalog doubling vs linear eigenvalue gap yields ~1 new representation per depth. Consistent with angle data but not proven.

### Data

- zeta_12 catalog: 3888 entries, 9 closure rounds (depths 0-8)
- Direction count by cumulative depth: 2, 6, 22, 46, 114, 239, 507, 975, 2043
- Pairwise |qdot| coherence: ~0.42 at all depths (flat, matches random)
- Cross-depth coherence: intra=0.4214, inter=0.4254 (intra < inter)
- zeta_8 directions: 13 total, all covered by depth 2
- Cell diversity (k=6): ~66% fill at all depths
- log2(distinct_cells) slope: ~1.17 per depth
- Cayley edges/entry: converges to 4/3
- Deep-564 Cayley edges: 0; strided-564: 58
- Cumulative distinct angles: 2->4->6->10->12->16->21->35->44
- XOR6 winners (Phase 11): 3736 total, 234 shadow (6%)
- XOR8 winners (Phase 11): 2200 total, 430 shadow (20%), 430/430 nested (100%)
- Phase 2 deep advantage: 5-10% over random at matched N
- Structural constants: ~66% cell fill, 100% Voronoi coverage, 4/3 edges/entry, +2 sectors/weight

### Code Assets

- **`build_closure(verbose)`**: BFS-based SU(2) group closure with per-entry depth tracking (`g_depth[]`), per-round counts (`round_count[]`, `round_start[]`). Reused across all phases with `save_catalog()`/`restore_catalog()`.
- **`load_single_depth(d)` / `load_cumulative(max_d)`**: Subset extraction by depth — loads entries at exactly depth d, or all entries up to depth d, into `g_cat[]` for analysis. Enables controlled depth-stratified experiments.
- **`load_random_from_depth(d, n)` / `load_random_full(n)`**: Fisher-Yates shuffle-based random sampling from specific depth shell or full catalog. Used in discriminating experiments (Phase 2) and random baselines (Phases 3, 10).
- **`mean_pairwise_qdot(cat_size)`**: Mean absolute quaternion dot product across all pairs. Coherence metric used in Phases 3, 5.
- **`combined_cell(sa, sb, sc, sd, k_sec)`**: S1 x S2 activation cell assignment for quaternion sums. Maps 4D quaternion to (sector, voronoi_cell) index. Core of XOR test and cell diversity analysis.
- **`is_cayley_neighbor(a, b, gens, n_gens)`**: Tests whether two quaternions are one generator step apart. Used in Phase 8 Cayley density analysis.
- **`extract_angle(q)`**: Extracts rotation angle in degrees from quaternion. Used in Phase 10 angle variance analysis and Phase 11 shadow pair detection.
- **`quat_vor(q)`**: Extracts Voronoi cell index from quaternion rotation axis. Used in Phase 11 shadow pair detection.
- **Phase 11 paired extension framework**: Shadow pair detection (same Voronoi direction, different angle) + nesting test (remove shadow element, test remaining as lower-arity XOR winner). Generalizable to any catalog/direction setup.
- **LCG random number generator**: Simple `g_rng` state with `rng_uniform()` and `shuffle()`. Self-contained, no stdlib rand dependency.

### Literature Touched

- **Demo 82 depth law**: max_xor ~ depth + 6 (linear). Demo 89 investigates the mechanism.
- **Demo 77 paired extension**: 100% paired extension at zeta_8 (24 entries, 13 dirs). Demo 89 Phase 11 shows this drops to 6-20% at zeta_12 (275 entries, 114 dirs) — the mechanism shifts from shadow pairing to direction diversity as vocabulary grows.
- **Tahmasebi & Jegelka (NeurIPS 2023)**: Group invariance implies dimension reduction. SU(2)/U(1) = S2. Consistent with DKC's independent discovery (Demo 67) that S2 directional structure matters.
- **Lauda et al. (2025, Nature Comms)**: Neglecton anyons for universal QC. External validation of Demo 84's null-indispensability findings.
- **Reservoir Computing (Jaeger 2001, Maass et al. 2002)**: Echo state networks / liquid state machines. The depth law's "algebraic coherence" parallels the reservoir separation property — deep entries maintain richer computational manifold.
- **Casimir operator theory**: Casimir balance hypothesis (catalog doubling vs linear eigenvalue gap) suggested as possible explanation for ~1 new representation per depth round.

### Open Questions

- **What is "algebraic coherence" precisely?** Deep tuples are more likely to satisfy parity than random tuples at matched catalog size. Demo 89 eliminated 5 candidate mechanisms but did not identify the actual one. Attacked in Demo 90 (sum-angle structure, axis cancellation).
- **Why is the slope exactly 1?** One XOR level per ~2 depth rounds. The combinatorial-algebraic balance conjecture proposes exponential catalog growth cancels exponential parity difficulty, but the precise coefficient is unexplained.
- **Does the +6 intercept connect to the 13=13 theorem's l=6 bandwidth?** Phase 4 ruled out direction coverage timing. The connection, if any, remains open.
- **At what depth does XOR14 (7 weights) first appear?** Extrapolated from depth law but not directly verified.
- **Is log2(cell_diversity) ~ 1.17d + 2 causal or epiphenomenal?** Both cell diversity and max_xor grow linearly with depth. Are they causally linked or both consequences of the same underlying algebraic structure?
- **Does the paired extension / diversity duality generalize to other roots of unity?** Tested at zeta_8 (shadow dominant) and zeta_12 (diversity dominant). What about zeta_10, zeta_16?
