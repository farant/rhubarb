## Demo 95: Commutator Depth and XOR Capacity

- **Status**: COMPLETE (RESULT)
- **File**: `demo_95_commutator_depth/main.c` (~1242 lines)
- **Tests**: 19 pass, 0 fail (Phases 1-4 + 3b)
- **Depends on**: Demo 94 (binary icosahedral 2I vs solvable z8, solvability bottleneck confirmed), Demo 93 (1wpi phase_cell framework, circuit complexity hierarchy), Demo 84 (0+0=36 null synergy), Demo 88 (anti-correlation, load-bearing non-null directions), Demo 89 (depth law: max_xor ~ depth + 6)
- **Feeds into**: Demo 96 (TL-group cross-classification: are D84 null/non-null and D95 derived series the same partition?), unify D84/D95 synergy, cross-depth synergy matrix (all 4 level-pairs of derived series), RKHS kernel rank (does kernel rank predict synergy?), zeta_12 stratification question

### Headline
The commutator structure of solvable vs non-solvable groups is made VISIBLE through derived series computation and subset XOR capacity testing. The key result: neither the commutator subgroup nor the non-commutator coset carries disproportionate XOR capacity in isolation. Computational power emerges from CROSS-LAYER interaction between algebraic strata. This is the same 0+0>0 synergy pattern as D84, now traced to its algebraic origin.

### Key Results

- **Phase 1 -- Commutator Classification (5 tests)**: z8 (binary octahedral, 24 bracket values): 12/24 entries are single commutators (exactly the binary tetrahedral 2T subgroup). G1 (commutator subgroup) = 12 entries. G0\G1 (non-commutator coset) = 12 entries. 2I (binary icosahedral, 60 bracket values): 60/60 entries are single commutators -- PERFECT GROUP confirmed computationally. Matches Liebeck-O'Brien-Shalev-Tiep (2011): since 2I is isomorphic to SL(2,5), every element is a commutator.
- **Phase 2 -- Derived Series (5 tests)**: z8 derived series: 24 > 12 > 4 > 1. Matches the chain 2O > 2T > Q8 > {+/-1} (binary octahedral > binary tetrahedral > quaternion group > center). Level membership partition: Level 0 (D0\D1) = 12 entries (outermost, non-commutator), Level 1 (D1\D2) = 8 entries (first commutator layer), Level 2 (D2\D3) = 3 entries (deep commutator, Q8), Level 3 (D3) = 1 entry (identity). 2I: G1 = G0 = 60 -> PERFECT. Derived series terminates immediately. First direct computation of this partition in the DKC context.
- **Phase 3 -- COMM vs NON-COMM XOR Capacity (3 tests)**: COMM (G1, 12 entries) and NON-COMM (G0\G1, 12 entries) perform SIMILARLY in isolation (~1.1x COMM edge). But ALL(24) dramatically outperforms both -- 2.7x the rate at N=5. The computational power is in the CROSS-TERMS between solvable layers, not in either layer alone. This is the same DKC synergy mechanism as D84's 0+0=36. COMM survives at N=7 (1.01%) while NON-COMM dies (0%) -- algebraic depth provides last-resort structure at the computational boundary.
- **Phase 3b -- 90-Degree Split Test (3 tests)**: 9 bracket-null entries (half-angle 90 degrees, Re=0) come from TWO derived levels: 3 from Q8 (algebraically deep, D2) and 6 from outermost shell (algebraically shallow, D0). Same half-angle, different algebraic origin. Q8-null (3 entries): XOR = 0 at all N. Outermost-null (6 entries): XOR = 0 at all N. All-null combined (9 entries): XOR = 33/126 = 26.19% at N=4, zero elsewhere. NEITHER subset can do XOR alone. Only the MIX produces capacity. Cleanest synergy demonstration in the project: pure 0+0=26% at matched geometry. Algebra wins over geometry.
- **Phase 4 -- Matched Comparison z8-COMM-12 vs 2I-first-12 (3 tests)**: 2I-first-12 crushes z8-COMM at N=3-5 (1.56x to 2.15x). Crossover at N=6-7 mirrors D94 Phase 4 pattern (z8-COMM slightly leads at N=6 with 6.28% vs 5.95%). Non-solvable 2I wins at matched size because it has no stratification bottleneck -- every element is a commutator, so all 60 entries participate equally.

### Theorems/Conjectures

- **Cross-Layer Synergy Mechanism (DEMONSTRATED computationally)**: Neither the commutator subgroup (COMM) nor the non-commutator coset (NON-COMM) of z8 carries disproportionate XOR capacity. Capacity emerges from mixing algebraic strata. The Barrington mechanism works through CROSSING between algebraic layers, not through commutators alone.
- **Solvability Bottleneck via Stratification (REFINED from D94)**: Solvable groups waste combinatorial budget on cross-layer constraints. Having derived series layers forces cross-layer dependence which constrains how many useful signed sums exist. 2I's perfection (G1=G0) means every pair (i,j) contributes commutator-quality interference. Solvability bottleneck operates THROUGH stratification.
- **90-Degree Algebra-over-Geometry (DEMONSTRATED)**: Within the same half-angle (90 degrees), algebraic origin creates computational capacity. Entries from Q8 (D2) and outermost shell (D0) are geometrically identical but algebraically distinct. Neither subset alone produces XOR solutions, but their union does. Geometry is necessary but not sufficient; algebraic depth is the active ingredient.
- **Synergy Universality Conjecture (PROPOSED)**: D84 synergy (TL null/non-null = 0+0=36) and D95 synergy (derived series layers = 0+0=26%) may be the same algebraic split seen through different lenses. Seeded D96 to test this.

### Data

**z8 group structure (Phase 1-2):**
- |z8| = 24 bracket values (binary octahedral 2O, order 48 mod sign)
- Generators: g1 = (sqrt2/2)(1 + i), g2 = (sqrt2/2)(1 - k), plus their conjugates
- 12/24 entries are single commutators
- G1 (commutator subgroup) = 12 entries (binary tetrahedral 2T)
- Derived series: 24 > 12 > 4 > 1

**z8 derived series level membership:**

| Level | Size | Algebraic identity |
|-------|------|--------------------|
| D0\D1 | 12 | Outermost, non-commutator |
| D1\D2 | 8 | First commutator layer (2T\Q8) |
| D2\D3 | 3 | Deep commutator (Q8\center) |
| D3 | 1 | Identity |

**2I group structure (Phase 1):**
- |2I| = 60 bracket values (binary icosahedral, order 120 mod sign)
- 60/60 entries are single commutators -- PERFECT GROUP
- Derived series: 60 = 60 (terminates immediately)

**Phase 3 -- COMM vs NON-COMM vs ALL XOR hit rates:**

| N | COMM(12) | NON-COMM(12) | ALL(24) | COMM/NONC |
|---|----------|--------------|---------|-----------|
| 3 | 48.64% | 42.73% | 71.94% | 1.14x |
| 4 | 44.04% | 51.52% | 75.04% | 0.85x |
| 5 | 15.15% | 13.64% | 40.76% | 1.11x |
| 6 | 6.28% | 5.63% | 9.64% | 1.12x |
| 7 | 1.01% | 0.00% | 0.18% | inf |

**Phase 3b -- 90-degree split (Re=0 entries, matched half-angle):**

| Subset | Size | N=3 XOR | N=4 XOR | N=5 XOR | N=6 XOR |
|--------|------|---------|---------|---------|---------|
| Q8-null (D2) | 3 | 0 | 0 | 0 | 0 |
| Outermost-null (D0) | 6 | 0 | 0 | 0 | 0 |
| All-null (mixed) | 9 | 0 | 33/126 = 26.19% | 0 | 0 |

**Phase 4 -- Matched comparison (12 entries each):**

XOR hit rates:

| N | z8-COMM (12) | 2I-first-12 | ratio (2I/z8) |
|---|-------------|-------------|---------------|
| 3 | 48.64% | 75.91% | 1.56x |
| 4 | 44.04% | 72.12% | 1.64x |
| 5 | 15.15% | 32.58% | 2.15x |
| 6 | 6.28% | 5.95% | 0.95x |
| 7 | 1.01% | 0.00% | 0.00x |

### Code Assets

- **`Zr2` (Z[sqrt2] ring arithmetic)**: Exact integer type representing a + b*sqrt(2). Operations: `zr2_make`, `zr2_add`, `zr2_sub`, `zr2_neg`, `zr2_mul`, `zr2_eq`, `zr2_div2`. Multiplication uses sqrt(2)^2 = 2. Reusable for any computation in Z[sqrt2].
- **`QZ8` (quaternion over Z[sqrt2])**: Four-component quaternion with Zr2 entries (representing (a + b*sqrt2)/2 per component). Operations: `qz8_make`, `qz8_eq`, `qz8_neg`, `qz8_conj`, `qz8_mul`. Multiplication includes `zr2_div2` for /4 -> /2 renormalization. Exact arithmetic, no floating point in group operations.
- **`Zr5` / `Q2I` (from D94)**: Z[sqrt5] ring and quaternion arithmetic for binary icosahedral group. Carried forward unchanged.
- **`build_z8(verbose)`**: BFS closure of binary octahedral group from generators (sqrt2/2)(1+i) and (sqrt2/2)(1-k) plus conjugates. Stores elements in global array with depth tracking. Handles mod-sign identification (48 elements -> 24 bracket values).
- **`build_2i(verbose)`**: BFS closure of binary icosahedral group from generators s and t (from D94). 120 elements -> 60 bracket values.
- **`z8_commutator(a, b)` / `i2_commutator(a, b)`**: Compute [a,b] = a*b*a^{-1}*b^{-1} for z8 and 2I respectively. Uses conjugate as inverse (unit quaternions).
- **`close_z8_subgroup(in_set)`**: Takes a subset indicator array, closes under multiplication and inverse to produce a subgroup. Used for derived series computation. Returns size of closed subgroup.
- **`phase1_commutators()`**: Enumerates all single commutators [g_i, g_j] in both z8 and 2I. Classifies each group element as commutator or not. Verifies 2I is perfect (all elements are commutators).
- **`phase2_derived_series()`**: Iteratively computes derived series G0 > G1 > G2 > ... by taking commutator subgroup at each level. Tracks level membership (which derived layer each element belongs to). Verifies z8 series terminates at 1 and 2I is perfect.
- **`phase3_capacity_comparison()`**: Tests XOR and AND capacity for COMM subset, NON-COMM subset, and full z8 group independently. Uses 1wpi phase_cell framework with k_sec shortlist {6, 12, 24}. Produces comparison table with hit rates.
- **`phase3b_ninety_degree_split()`**: Isolates Re=0 (90-degree half-angle) entries and splits them by derived series level (Q8-level vs outermost). Tests each subset and their union for XOR. Demonstrates algebra-over-geometry synergy.
- **`phase4_matched_comparison()`**: Size-controlled comparison: z8-COMM (12 entries) vs first 12 entries from 2I (by BFS depth). Tests XOR and AND. Demonstrates 2I advantage at matched size using the commutator subset as the solvable baseline.
- **`load_z8_subset(mask)` / `load_2i_first_n(n)`**: Catalog loading helpers that populate the global float catalog from subset masks or first-N selection.
- **`qz8_to_float()` / `q2i_to_float()`**: Convert exact algebraic quaternions to floating-point Quat for use with 1wpi phase_cell framework. Shared with D94.
- **`phase_cell()` / `vor_cell()` / `build_dirs()` / `count_phase_ex()`**: 1wpi activation and testing infrastructure. Shared pattern across D92-D95.
- **`make_xor_tt()` / `make_and_tt()`**: Truth table generators for XOR and AND Boolean functions.
- **`next_combo()` / `comb_nk()`**: Combinatorial utilities for exhaustive weight-tuple enumeration.

### Literature Touched

- **Barrington (1989) -- branching programs over groups**: NC1 = programs over non-solvable groups. D95 refines the mechanism: the bottleneck is not that commutators are individually weaker, but that stratification (having derived series layers) forces cross-layer dependence which wastes combinatorial budget. Non-solvable groups avoid this by being perfect (G1=G0).
- **Liebeck, O'Brien, Shalev, Tiep (2011) -- Ore conjecture for finite simple groups**: Every element of a finite simple group is a commutator. Computationally confirmed for 2I (isomorphic to SL(2,5) / perfect group): 60/60 bracket values are single commutators.
- **ADE classification of finite SU(2) subgroups**: z8 derived series 24 > 12 > 4 > 1 corresponds exactly to 2O > 2T > Q8 > {+/-1} in the ADE chain (E7 > E6 > D4 > A1). First direct computation of this partition in the DKC context.
- **Barrington mechanism (refinement from D95)**: Cross-layer interaction, not commutators in isolation, is the active mechanism. The commutator subgroup and its coset have nearly equal XOR capacity when tested alone. The 2.7x boost from ALL(24) over either 12-element subset demonstrates that the signed-sum interference between algebraic strata is load-bearing.
- **Habiro (2002) / Z[zeta_8] bracket**: Algebraic substrate for the z8 group; D95 uses exact Z[sqrt2] arithmetic to compute derived series without floating-point error.

### Open Questions

- **Are D84 and D95 synergy the same split?** D84 synergy (TL null/non-null = 0+0=36) and D95 synergy (derived series layers = 0+0=26%) -- are they the same algebraic partition seen through different lenses, or independent? (Directly seeded D96.)
- **Cross-depth synergy matrix**: For z8, test ALL level-pair combinations (D0xD1, D0xD2, D1xD2, etc.) to map which crossings are most productive. The 90-degree split (Phase 3b) tested D0xD2; the full matrix would reveal the synergy landscape.
- **Layer combination map**: Which 2-layer and 3-layer combinations of derived series produce the most synergy? Is there a hierarchy of layer crossings?
- **RKHS kernel rank**: Does kernel rank predict synergy? If the kernel rank of the COMM+NON-COMM union exceeds the sum of individual ranks, that would mathematically quantify the cross-term mechanism.
- **zeta_12 stratification**: Does the dense (non-solvable, infinite) group have algebraic strata? If not, does that explain its high capacity?
- **N=7 COMM survival mechanism**: COMM survives at N=7 (1.01%) while NON-COMM dies (0%). What structural property of the commutator subgroup (2T) provides this last-resort capacity? Is it related to 2T's own commutator structure (Q8)?
