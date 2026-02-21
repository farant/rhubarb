## Demo 81: Capacity Scaling Law

- **Status**: COMPLETE
- **File**: `demo_81_capacity_scaling/main.c` (~604 lines)
- **Tests**: 14/14 pass
- **Depends on**: Demo 66 (quaternionic DKC, 24-cell geometry), Demo 77 (activation zoo, XOR8=6 at ζ₈), Demo 78 (recursive scaling, XOR10=0 at ζ₈, wall confirmed), Demo 79 (ζ₁₂ capacity, XOR10=124 and XOR12=50+), Demo 80 (finiteness survey, only ζ₄ and ζ₈ finite)
- **Feeds into**: Demo 82 (crossing depth interpretation of the scaling law), future work verifying XOR14 prediction at ~38K entries

### Headline
For ζ₁₂ (infinite group), XOR capacity scales logarithmically with catalog size: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6, meaning each +2 XOR inputs requires approximately 10× more entries.

### Key Results
- **Logarithmic scaling law**: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6. Each doubling of catalog size adds ~0.62 to max XOR; each +2 XOR level costs ~3.2 more doublings (~10× more entries).
- **Transition points**: XOR6 at ~5 entries (log₂ ≈ 2.3), XOR8 at ~51 (log₂ ≈ 5.7), XOR10 at ~275 (log₂ ≈ 8.1), XOR12 at ~1140 (log₂ ≈ 10.2).
- **Monotonicity confirmed**: capacity never decreases as more entries are added.
- **Winner saturation**: XOR6–XOR10 reach the 32-winner cap at every level where they appear; XOR12 emerges at 12 winners at round 6 then saturates at 32.
- **Two-regime vocabulary**: directions scale as ~40–45% of catalog size until saturating at MAX_DIR=512 (rounds 7–8); angles grow much more slowly, roughly as sqrt(catalog_size). Angular resolution is the limiting factor for higher XOR levels.
- **Decreasing transition gap**: doublings between XOR transitions are 3.35, 2.43, 2.05 — possibly sub-logarithmic behavior, but more data needed.
- **Finite vs. infinite contrast**: ζ₈ (binary octahedral group, ADE type E₇) hard-caps at XOR8; ζ₁₂ grows logarithmically with no hard ceiling within the explored range.

### Theorems/Conjectures
- **Logarithmic Scaling Law (DEMONSTRATED)**: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6 across 9 snapshot rounds (cat=5 to cat=4096). Equivalently catalog_size ≈ 3^M in the XOR level M.
- **Possibly Sub-Logarithmic (CONJECTURE)**: The decreasing gaps between transitions (3.35 → 2.43 → 2.05 doublings) suggest scaling may be O(log N / log log N) rather than pure O(log N). Requires more data points (XOR14+) to distinguish.
- **Direction Bottleneck Theorem (OBSERVED)**: Each XOR transition is gated by direction count — 22 dirs for XOR8, 114 for XOR10, 507 for XOR12. After direction saturation (round 7), angular diversity alone drives continued winner-count growth.
- **Kuperberg Consistency**: The exponential cost per XOR level is consistent with Kuperberg's #P-hardness result for non-lattice roots of unity.

### Data
- Snapshot table (round, catalog, dirs, angles, max XOR): (0,5,2,2,6), (1,17,6,4,6), (2,51,22,6,8), (3,127,46,10,8), (4,275,114,12,10), (5,564,239,16,10), (6,1140,507,21,12), (7,2292,512,35,12), (8,4096,512,43,12)
- Scaling constant: 0.62 XOR per doubling
- Intercept: 4.6 (at catalog=1)
- Transition costs in doublings: XOR6→XOR8: 3.35, XOR8→XOR10: 2.43, XOR10→XOR12: 2.05
- XOR14 prediction: ~38,000 entries (~11 closure rounds), feasible in minutes
- XOR16 prediction: ~615,000 entries (~14 rounds), feasible in hours
- XOR20 prediction: ~160 million entries, memory-limited
- XOR32 prediction: ~10^14 entries, infeasible
- Direction saturation: MAX_DIR=512 reached at round 7 (cat=2292)
- Angles at saturation: 35 (round 7), 43 (round 8)

### Code Assets
- **Group closure with snapshot tracking**: builds ζ₁₂ catalog via BFS-style generator multiplication; records `round_end[]` for per-snapshot analysis; identity + 4 generators (σ₁, σ₁⁻¹, σ₂, σ₂⁻¹)
- **`build_dirs()` / `count_angles()`**: extract unique rotation axes (Voronoi directions on S²) and unique half-angles from a catalog prefix; run independently per snapshot
- **`combined_cell()`**: Sec×Vor activation — maps a quaternion sum to a (sector, Voronoi-cell) pair; sector from rotation angle divided into k_sec bins, Voronoi cell from nearest direction
- **`test_xor()`**: early-exit XOR verification with sparse cell cleanup using `touched_cells[]` array; checks that all 2^(2n) input masks map even/odd parity to distinct activation cells
- **`find_xor6()` + `find_recursive()`**: brute-force XOR6 search over triples, then recursive extension to XOR8/10/12 by appending one entry to each winner; sorted-tuple deduplication
- **`Snap` struct + `test_snapshot()`**: per-round snapshot driver that runs the full XOR6→12 pipeline and records results for the scaling table
- **Logarithmic fit analysis**: inline computation of XOR-per-doubling slope and prediction for next XOR level

### Literature Touched
- **Kuperberg (2015)**: #P-hardness of approximating the Jones polynomial for non-lattice roots of unity; the exponential catalog cost per XOR level is consistent with this hardness result
- **Aizenberg (2008)**: k-sector MVN activation (Sec×Vor is the S² generalization); the logarithmic scaling characterizes how the S² analog scales with group complexity
- **Habiro (2002) / Z[ζ_8] bracket theory**: the finite-group ceiling at ζ₈ vs. logarithmic growth at ζ₁₂ maps to the algebraic difference between lattice (E₇/binary octahedral) and non-lattice roots

### Open Questions
- **Does scaling continue to XOR14?** Prediction of ~38K entries at ~11 closure rounds is directly testable by increasing MAX_QCAT; would add one more data point to the transition gap series.
- **Is scaling truly O(log N) or O(log N / log log N)?** The decreasing gap series (3.35 → 2.43 → 2.05) needs at least one more data point (XOR14 transition) to distinguish these hypotheses.
- **Does the 0.62 constant depend on the root of unity?** ζ₁₀ might give a different slope (ζ₁₀ has 71 angles at 4096 entries vs ζ₁₂'s 43); the constant may be a function of angular density per round.
- **What determines the ~0.62 constant?** Is it a consequence of the quaternion dimension (4D) or the specific Sec×Vor activation geometry?
- **Can union catalogs improve the constant?** Mixing ζ₁₀ + ζ₁₂ entries might give denser angular coverage per entry and change the scaling constant without altering the logarithmic character.
