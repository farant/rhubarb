## Demo 96: TL-Group Cross-Classification — 2D Synergy Landscape

- **Status**: COMPLETE (LANDMARK)
- **File**: `demo_96_tl_group_cross/main.c` (~1022 lines)
- **Tests**: 14 pass, 0 fail (Phases 1-4)
- **Depends on**: Demo 84 (null/non-null split, 9/15 partition), Demo 95 (derived series, 12/8/3/1 partition, Z[sqrt2] quaternion arithmetic, QZ8 type), Demo 93 (circuit complexity hierarchy, 1wpi framework), Demo 88 (anti-correlation, body-diagonal directions)
- **Feeds into**: Demo 97 (Cell B perfection analysis), catalog pruning for 2I (D94 follow-up), cross-derived synergy principle as general DKC design rule

### Headline
The D84 null/non-null split and the D95 derived series split are INDEPENDENT partitions of the z8 bracket values. Their 2D cross-product creates a 5-cell landscape (A through E) that fully explains which entries carry computational capacity, which are synergy enhancers, and which are noise. The optimal z8 catalog is A+B+C (21 entries), not ALL (24) — removing the 3 Q8-null entries IMPROVES capacity.

### Key Results

- **Phase 1 — Cross-Tabulation (8 tests)**: The 24 z8 bracket values partition into 5 cells: A = D0-null (6 entries, half-angle 90 deg), B = D0-nonnull (6 entries, 45 deg), C = D1-nonnull (8 entries, 60 deg, 2T commutators), D = D2-null (3 entries, 90 deg, Q8 deep), E = D3-identity (1 entry, 0 deg). Cross-checks: null total (A+D) = 9, commutator total (C+D+E) = 12, non-commutator total (A+B) = 12, grand total = 24. All counts verified exact.

- **Phase 2 — Individual Cell Capacity (1 test)**: Cell B achieves 100% XOR at EVERY N (3 through 6) with only 6 entries and 3 directions. Cell C achieves 100% at N=3,4 but drops to 42.86% at N=5 and 57.14% at N=6 (4 directions). Null cells A and D are completely dead individually (0% at all N). The rule: non-null is necessary AND sufficient for individual capacity; among non-null cells, 45 deg (B) outperforms 60 deg (C).

- **Phase 3 — Pairwise Synergy Matrix (2 tests)**: B+C wins at EVERY N despite having fewer directions (7) than A+B (9) or A+C (10). B+C rates: 96.70% (N=3), 94.01% (N=4), 73.18% (N=5), 31.00% (N=6). A+D (both null, 90 deg) is worst pair: 0% at N=3,5,6 with only 26.19% at N=4. Dimension analysis shows crossing the null boundary matters more than crossing the derived boundary at every N.

- **Phase 4 — Triple Combinations (2 tests)**: A+B+C (skip Q8/D) BEATS the full ALL(24) at EVERY N: 74.06% vs 71.94% (N=3), 77.51% vs 75.04% (N=4), 45.10% vs 40.76% (N=5), 12.98% vs 9.64% (N=6). The 3 Q8-deep-null entries are noise that actively degrades capacity. Missing cell impact: C (2T) is most important at N=3-5 (removing it loses 18-25%), B (D0-nonnull) is most important at N=6.

### Theorems/Conjectures

- **Two Independent Stratifications (CONFIRMED)**: The bracket-null/non-null partition (9/15 from D84) and the derived series partition (12/8/3/1 from D95) cut ACROSS each other, creating a genuine 2D classification. Confirmed by character table analysis of 2O (8 conjugacy classes, Kirillov-Ostrik 2002). Independence at element level, unity at categorical level via ADE/E7 Dynkin diagram.

- **Non-null = Load-bearing (CONFIRMED)**: Null cells carry zero individual XOR capacity. Non-null is necessary AND sufficient for solo capacity. This sharpens D84's "null indispensability" finding: Cell A (D0-null, outermost) enhances synergy when combined with non-null cells; Cell D (Q8-null, deep) is pure noise.

- **Cross-derived Synergy Principle (CONFIRMED)**: Non-null entries from DIFFERENT derived levels (B from D0, C from D1) produce maximum synergy. B+C dominates all 6 pairwise combinations at every N tested.

- **Catalog Pruning Principle (PROPOSED)**: For any DKC group, prune bracket-null entries in deep derived subgroups. Quality > quantity. Optimal z8 catalog is 21 entries (A+B+C), not 24 (ALL). Testable prediction: analogous pruning improves 2I capacity.

- **Prediction Scoreboard**: Three pre-computation predictions tested. Barrington depth=capacity: WRONG (B beats C, A=D=0, depth hurts). Angular diversity outer>inner: HALF RIGHT (B is best but A is also outer and dead). Dual-info both-dimensions: CLOSEST (B>C>>A=D confirmed, but null/non-null binary distinction overpowers derived dimension).

### Data

**Phase 1 — Cross-tabulation (5 cells):**

| Cell | Description | Count | Half-angle | Derived | Null? |
|------|-------------|-------|------------|---------|-------|
| A | D0-null (outermost, bracket-null) | 6 | 90 deg | D0 | Yes |
| B | D0-nonnull (outermost, non-null) | 6 | 45 deg | D0 | No |
| C | D1-nonnull (2T commutators) | 8 | 60 deg | D1 | No |
| D | D2-null (Q8 deep) | 3 | 90 deg | D2 | Yes |
| E | D3-identity | 1 | 0 deg | D3 | No |

Cross-checks: Null total (A+D) = 9, COMM total (C+D+E) = 12, NONCOMM total (A+B) = 12, Grand total = 24.

**Phase 2 — Individual cell XOR capacity:**

| Cell | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|------|------|------|-----|-----|-----|-----|
| A (D0-null) | 6 | 6 | 0/20 (0.00%) | 0/15 (0.00%) | 0/6 (0.00%) | 0/1 (0.00%) |
| B (D0-nonnull) | 6 | 3 | 20/20 (100.00%) | 15/15 (100.00%) | 6/6 (100.00%) | 1/1 (100.00%) |
| C (D1-nonnull) | 8 | 4 | 56/56 (100.00%) | 70/70 (100.00%) | 24/56 (42.86%) | 16/28 (57.14%) |
| D (D2-null) | 3 | 3 | 0/1 (0.00%) | n/a | n/a | n/a |
| E (D3-identity) | 1 | 0 | n/a | n/a | n/a | n/a |

**Phase 3 — Pairwise synergy XOR rates:**

| Pair | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|------|------|------|-----|-----|-----|-----|
| A+B (outermost shell) | 12 | 9 | 94/220 (42.73%) | 255/495 (51.52%) | 108/792 (13.64%) | 52/924 (5.63%) |
| A+C (cross both dims) | 14 | 10 | 168/364 (46.15%) | 750/1001 (74.93%) | 300/2002 (14.99%) | 40/3003 (1.33%) |
| A+D (both null, 90-deg) | 9 | 9 | 0/84 (0.00%) | 33/126 (26.19%) | 0/126 (0.00%) | 0/84 (0.00%) |
| B+C (both nonnull, cross derived) | 14 | 7 | 352/364 (96.70%) | 941/1001 (94.01%) | 1465/2002 (73.18%) | 931/3003 (31.00%) |
| B+D (cross both dims) | 9 | 3 | 20/84 (23.81%) | 15/126 (11.90%) | 6/126 (4.76%) | 1/84 (1.19%) |
| C+D (comm subgroup) | 11 | 7 | 80/165 (48.48%) | 162/330 (49.09%) | 96/462 (20.78%) | 34/462 (7.36%) |

Best pair at every N: B+C.

Dimension analysis (average rates for cross-category pairs):

| N | CrossNull (A+B, C+D) | CrossDerived (A+C, B+D) |
|---|----------------------|-------------------------|
| 3 | 45.61% | 34.98% |
| 4 | 50.30% | 43.41% |
| 5 | 17.21% | 9.87% |
| 6 | 6.49% | 1.26% |

CrossNull > CrossDerived at every N: the null/non-null boundary matters more than the derived level boundary.

**Phase 4 — Triple combinations vs ALL:**

| Subset | Size | Dirs | N=3 | N=4 | N=5 | N=6 |
|--------|------|------|-----|-----|-----|-----|
| ALL (A+B+C+D+E) | 24 | 13 | 1456/2024 (71.94%) | 7974/10626 (75.04%) | 17325/42504 (40.76%) | 12975/134596 (9.64%) |
| A+B+C (skip Q8) | 21 | 13 | 985/1330 (74.06%) | 4639/5985 (77.51%) | 9178/20349 (45.10%) | 7043/54264 (12.98%) |
| A+B+D (skip 2T) | 16 | 9 | 288/560 (51.43%) | 920/1820 (50.55%) | 976/4368 (22.34%) | 318/8008 (3.97%) |
| A+C+D (skip D0nn) | 18 | 13 | 452/816 (55.39%) | 2007/3060 (65.59%) | 1977/8568 (23.07%) | 614/18564 (3.31%) |
| B+C+D (skip D0null) | 18 | 7 | 613/816 (75.12%) | 2180/3060 (71.24%) | 2744/8568 (32.03%) | 1369/18564 (7.37%) |

Missing cell impact (triple rate minus ALL rate):

| Missing | N=3 | N=4 | N=5 | N=6 |
|---------|-----|-----|-----|-----|
| D (Q8) | +2.12% | +2.47% | +4.34% | +3.34% |
| C (2T) | -20.51% | -24.49% | -18.42% | -5.67% |
| B (D0nn) | -16.54% | -9.45% | -17.69% | -6.33% |
| A (D0null) | +3.19% | -3.80% | -8.73% | -2.27% |

Most important cell: C (2T) at N=3-5, B (D0-nonnull) at N=6.

### Code Assets

- **`Zr2` (Z[sqrt2] ring arithmetic, from D95)**: Exact integer type representing a + b*sqrt(2). Operations: `zr2_make`, `zr2_add`, `zr2_sub`, `zr2_neg`, `zr2_mul`, `zr2_eq`, `zr2_div2`. Multiplication uses the identity sqrt(2)^2 = 2. Reused from D95 without modification.
- **`QZ8` (quaternion over Z[sqrt2]/2, from D95)**: Four-component quaternion with Zr2 entries (representing (a + b*sqrt(2))/2 per component). Operations: `qz8_make`, `qz8_eq`, `qz8_neg`, `qz8_conj`, `qz8_mul`. Exact arithmetic, reused from D95.
- **`qz8_to_float()`**: Converts exact Z[sqrt2] quaternion to floating-point `Quat` for use with phase_cell framework.
- **`build_z8()`**: BFS closure from 4 generators (two generator pairs + conjugates). Produces 24 bracket values (mod sign).
- **`z8_commutator()` / `close_z8_subgroup()` / `compute_derived_series()`**: Derived series computation: generates commutator subgroups, closes under multiplication, assigns derived level to each element. Produces the D0/D1/D2/D3 partition (12/8/3/1).
- **`compute_null_classification()`**: Tests if scalar component (Re part) of each quaternion is zero. Produces the null/non-null partition (9/15).
- **`compute_cells()`**: Cross-product of derived level x null classification -> 5-cell assignment (A through E). Core new contribution of D96.
- **`load_cells(cell_mask)`**: Flexible subset loader: given a 5-element bitmask, loads the union of selected cells into the global catalog. Enables arbitrary cell combination testing.
- **`run_subset_capacity()` / `print_subset_result()`**: Runs exhaustive XOR capacity test at N=3,4,5,6 for the currently loaded catalog. k_sec sweep over {6, 12, 24}. Reports hit counts, totals, and percentages.
- **`build_dirs()` / `vor_cell()` / `phase_cell()`**: Voronoi direction extraction and 1wpi phase-cell activation framework. Shared with D93/D94/D95.
- **`count_phase_ex()` / `next_combo()`**: Exhaustive combinatorial weight-tuple testing. Iterates all C(n,k) combinations.

### Literature Touched

- **Kirillov-Ostrik (2002) — character table of 2O**: 8 conjugacy classes of the binary octahedral group. The two stratifications (bracket-null/non-null = TL radical lens, derived series = group-theoretic) connect to E7 Dynkin diagram through different pathways — representation-theoretic for TL, group-theoretic for derived series. Independent at element level, unified at categorical level.
- **Barrington (1989) — branching programs**: D96 partially refutes naive depth=capacity interpretation. Cell B (shallowest non-null, D0) is the BEST individual performer, while deeper cells (D, D2) are noise. Depth in the derived series HURTS capacity for null entries. The correct reading: non-solvability matters at the group level (D94), but within a solvable group, derived depth is secondary to the null/non-null distinction.
- **D84 null indispensability (reinterpreted)**: The original finding that null entries are "indispensable" specifically refers to Cell A (D0-null, outermost). Cell D (Q8-null, deep) is not indispensable — it is noise. The null/non-null distinction is load-bearing only in combination with derived level.
- **D95 COMM vs NONCOMM (reinterpreted)**: The near-equal capacity of commutator vs non-commutator subgroups in isolation is explained by contamination from null cells in both partitions. The true signal is B+C (non-null x cross-derived synergy), which was masked when averaging over all entries in each partition.

### Open Questions

- **Cell B perfection mechanism**: WHY does every combination of 6 Cell B entries compute XOR? What geometric/algebraic property of 3 mutually orthogonal directions at 45 deg half-angle makes this work? (Seeded as D97, now completed.)
- **Catalog pruning for 2I**: Does removing analogous "noise" entries (bracket-null in deep derived subgroups) from the binary icosahedral group 2I improve its capacity? If the pruning principle is universal across DKC groups, this is a general optimization rule.
- **Non-monotonicity of missing-cell impact**: Cell A (D0-null) shows +3.19% at N=3 (removing it helps) but -8.73% at N=5 (removing it hurts). Why does the synergy-enhancer role of outermost null entries depend on arity?
- **BFS-as-braids dogfooding demo**: Seeded from researcher manifold report during D96 session.
