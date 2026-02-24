## Demo 87: Null Indispensability at zeta_12

- **Status**: COMPLETE
- **File**: `demo_87_null_indispensability/main.c` (~907 lines)
- **Tests**: 14/14 pass
- **Depends on**: Demo 84 (null states and logarithmic structure at zeta_8, direction partition framework, null indispensability theorem), Demo 79 (zeta_12 capacity, infinite group XOR12)
- **Feeds into**: Demo 88 (anti-correlation mechanism, cross-root comparison uses D87 dispensability finding), Reservoir Computing pillar (null-state role depends on group regime)

### Headline
Null indispensability is a finite-group phenomenon: at zeta_12 (infinite group, 4096 entries), bracket-null entries are directionally indispensable (67 exclusive S2 axes) but computationally dispensable (XOR capacity fully preserved without nulls) — a regime transition from zeta_8 where removing nulls destroys capacity.

### Key Results

- **zeta_8 control baseline (24 entries, 13 directions)**: 9 null entries (37.5%), direction partition 6 null-only + 3 shared + 4 non-null-only = 13. Full catalog achieves XOR6=32, XOR8=32. Non-null-only (15 entries, 7 dirs) achieves XOR6=32 but XOR8=4. Removing nulls destroys XOR8 capacity. Confirms Demo 84's findings as a baseline.
- **zeta_12 catalog (4096 entries, 2043 directions)**: 121 null entries (3.0%), appearing at depths 2-8. Direction partition: 67 null-only + 54 shared + 1922 non-null-only = 2043. Null fraction drops from 37.5% (zeta_8) to 3.0% (zeta_12).
- **zeta_12 capacity preservation**: Full catalog (4096 entries) and non-null-only subset (3975 entries) BOTH achieve XOR6=32, XOR8=32, XOR10=32, XOR12=32. Maximum XOR level and winner counts are identical. Removing 121 null entries has zero effect on computational capacity.
- **Directional vs computational indispensability decoupling**: At zeta_12, nulls are directionally indispensable (67 exclusive axes that no non-null entry reaches) but computationally dispensable. The 1976 non-null directions provide enough combinatorial diversity that losing 67 null-only directions does not constrain XOR solutions.
- **Depth distribution**: Null entries appear at depths 2-8 in zeta_12, with null fraction increasing at greater depths (consistent with zeta_8 pattern where depth-3 = 100% null). Detailed depth x null cross-tabulation computed per phase.
- **Half-angle of null entries**: All null entries have half-angle exactly 90 degrees. This is tautological: null means Re(q)=0 means half_angle = arccos(0) = 90 degrees by definition. Not a discovery.
- **Truncation artifact caught and fixed**: Initial run with MAX_DIR=600 truncated zeta_12's true 2043 directions to 600, producing a wrong partition (39/30/531) and a spurious XOR12 drop (32 to 24). Increasing MAX_DIR to 2048 fixed the issue. The capacity drop was a measurement artifact, not a real finding.

### Theorems/Conjectures

- **Null Dispensability at Infinite Groups (DEMONSTRATED)**: At zeta_12, removing all 121 bracket-null entries preserves full XOR capacity (XOR6/8/10/12 all 32 winners). Null indispensability, demonstrated at zeta_8 in Demo 84, does not generalize to infinite groups.
- **Regime Transition Theorem (DEMONSTRATED)**: There is a qualitative transition between finite groups (zeta_8: nulls computationally critical, 37.5% of catalog, 6/13 exclusive directions in a sparse direction set) and infinite groups (zeta_12: nulls computationally redundant, 3% of catalog, 67/2043 exclusive directions in a dense direction set). The transition mechanism is direction density — when the non-null direction count is large enough, losing null-only directions cannot constrain combinatorial search.
- **Null Fraction Dilution (CONFIRMED)**: zeta_4 = 75%, zeta_8 = 37.5%, zeta_12 = 3.0%. The null fraction decreases sharply as group order grows, consistent with the Demo 84 conjecture that nulls approach measure zero in infinite groups.
- **Directional Indispensability Without Computational Indispensability (NEW OBSERVATION)**: Owning exclusive S2 directions is necessary but not sufficient for computational indispensability. The question is whether the remaining directions provide enough combinatorial diversity for XOR solutions — at zeta_8 they do not (too sparse), at zeta_12 they do (dense enough).

### Data

- zeta_8 catalog: 24 entries, 13 directions, 9 null (37.5%), 4 closure rounds
- zeta_8 direction partition: 6 null-only, 3 shared, 4 non-null-only
- zeta_8 capacity: Full XOR8=32, Non-null XOR8=4 (capacity destroyed)
- zeta_12 catalog: 4096 entries, 2043 directions, 121 null (3.0%)
- zeta_12 direction partition: 67 null-only, 54 shared, 1922 non-null-only
- zeta_12 capacity (full): XOR6=32, XOR8=32, XOR10=32, XOR12=32
- zeta_12 capacity (non-null only, 3975 entries): XOR6=32, XOR8=32, XOR10=32, XOR12=32
- Brute-force limit for zeta_12: bf_limit=100 (first 100 entries for XOR6 triple search)
- K_LADDER values tested: {1, 6, 8, 10, 12, 16, 20, 24}
- MAX_DIR bug: initial 600 truncated to wrong results; fixed at 2048

### Code Assets

- **`classify_null(cat_size)`**: classifies each catalog entry as null (|Re(q)| < 1e-10) or non-null; fills `is_null[]`, `null_idx[]`, `nonnull_idx[]` arrays with counts `n_null_total`, `n_nonnull_total`
- **`compute_dir_partition(cat_size)`**: builds direction catalog and classifies each direction as null-only, non-null-only, or shared; returns `DirPartition` struct with counts; also fills `dir_null_cnt[]` and `dir_nonnull_cnt[]` per-direction arrays
- **`DirPartition` struct**: {total_dirs, null_only, nonnull_only, shared} — reusable partition type
- **`load_subset(indices, count)`**: loads arbitrary subset of saved catalog into `g_cat[]` for capacity measurement; enables null-removal experiments
- **`save_catalog()` / `restore_catalog()`**: snapshot/restore pattern for catalog state including depth tracking
- **`find_capacity(cat_size, bf_limit, CapResult *r)`**: unified capacity pipeline — builds dirs, runs XOR6 brute force then recursive extension to XOR8/10/12; returns `CapResult` with n_dirs, xor6, xor8, xor10, xor12, max_xor
- **`CapResult` struct**: {n_dirs, xor6, xor8, xor10, xor12, max_xor} — reusable capacity result type
- **`test_xor()` with early conflict detection**: XOR test uses `cell_even[]`/`cell_odd[]` counters with touched-cell tracking for fast cleanup; `goto cleanup` on first conflict for early exit
- **`combined_cell()` with k_sec sectors**: Sec x Vor activation using angular sectors and Voronoi direction cells; used with K_LADDER for multi-resolution capacity search
- **`find_xor6()` and `find_recursive(level, cat_limit)`**: two-stage winner search — brute-force triples for XOR6, then recursive extension for XOR8/10/12; stores winners in `g_win[][]` arrays
- **`g_depth[]` tracking through closure**: each catalog entry records its generation depth during `build_closure()`; enables depth-stratified analysis in Phase 4
- **Phase 1-4 modular structure**: `phase1_catalog()`, `phase2_capacity()`, `phase3_direction_analysis()`, `phase4_depth()` — each phase is a self-contained function taking root_name as parameter, reusable for any root of unity

### Literature Touched

- **Gurarie (1993) / Flohr (1996) / Gaberdiel & Kausch (1996)**: LCFT null-state framework — Demo 87 shows the LCFT null-state interpretation (null entries as Jordan-cell anchors) is regime-dependent: critical at finite groups, redundant at infinite groups where direction density absorbs the loss.
- **Habiro (2002) / Z[zeta_8] bracket**: the zeta_8 baseline confirms Habiro's algebraic substrate; the zeta_12 extension tests whether null-state structure persists beyond the finite-group regime.
- **Reservoir Computing (Jaeger 2001, Maass et al. 2002)**: Demo 84's RC null-state hypothesis (null reservoir states hold the manifold open) is refined: this only applies in sparse-direction regimes. In dense-direction regimes (infinite groups), the non-null states alone provide sufficient manifold dimensionality.

### Open Questions

- **Where exactly is the transition?** Between zeta_8 (nulls indispensable) and zeta_12 (nulls dispensable), there must be a crossover point. Is it at a specific catalog size, direction count, or null fraction? Testing at intermediate sizes (e.g., zeta_10 or truncated zeta_12 catalogs) could locate the transition.
- **Are the 67 null-only directions at zeta_12 ever needed?** At larger XOR levels (XOR14, XOR16), could null-only directions become essential again? The current search stops at XOR12.
- **Does the bf_limit=100 truncation affect the result?** The XOR6 brute force only searches the first 100 of 4096 (or 3975 non-null) entries. If null entries cluster in the first 100, this could bias the comparison. The identical 32-winner result for both full and non-null suggests the truncation is not an issue, but exhaustive search would confirm.
- **Null fraction at zeta_10 (Fibonacci anyons)?** Demo 84 raised this question. zeta_10 was identified as infinite in Demo 80. Measuring its null fraction and direction partition would fill in the dilution curve between zeta_8 (37.5%) and zeta_12 (3.0%).
- **Is per-direction residual the right metric for the transition?** Demo 88 found zeta_12's per-direction spherical design residual is 8x lower than zeta_8's. Is direction density (or equivalently per-direction residual) the control parameter for the indispensability transition?
