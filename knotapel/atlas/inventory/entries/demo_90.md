## Demo 90: Sum-Angle Structure — Why Deep Entries Compose Better

- **Status**: COMPLETE
- **File**: `demo_90_sum_angle_structure/main.c` (~1713 lines)
- **Tests**: 10/12 pass (2 informative fails: Phase 1 count hypothesis — all subsets produce 2048+ distinct sum angles, so deep vs strided/random distinct-count comparisons fail because count is not the discriminating feature)
- **Depends on**: Demo 89 (depth law mechanism elimination — narrowed to "algebraic coherence"), Demo 82 (crossing depth law, deep-564 vs strided-564), Demo 75 (computation requires non-canonical sum angles), Demo 79 (zeta_12 capacity, infinite group)
- **Feeds into**: Demo 91 (activation bottleneck test — tests whether linear depth law is activation artifact or parity wall), D89-D90-D91 arc summary (depth law explained)

### Headline
The depth law mechanism is **axis cancellation** (anti-aligned rotation axes producing low sum angles) combined with **cross-depth algebraic constraints** (BFS ancestry creating limited sum-angle vocabularies between related entries), and the computational advantage is **relational, not positional** — the S² point cloud bandwidth actually DECREASES with depth (spectral inversion), ruling out all positional/bandwidth explanations.

### Key Results

- **Axis cancellation mechanism (Phase 2b)**: XOR8 winners select anti-aligned axis pairs. Mean min_dot for winners = -0.75 vs -0.65 for random non-winners. Two quaternions with nearly opposite rotation axes partially cancel, yielding a sum quaternion with a small rotation angle (30-66°) — exactly what sector activation needs for parity satisfaction. This is the geometric mechanism underlying "algebraic coherence."
- **Cross-depth algebraic constraint (Phase 2c)**: Generator × deep-entry (d=7-8) sums produce only 73 distinct angles from 50K pairs. Compare: shallow × shallow = 15 distinct, deep × deep = 1313 distinct. BFS-derived entries are algebraically related to generators, constraining their pairwise sums to a small vocabulary. This is the concrete realization of the abstract "algebraic coherence" identified in D82.
- **Relational, not positional (Phase 5)**: Spherical harmonic decomposition of the S² point cloud (rotation axes on the sphere) shows bandwidth DECREASES with depth. BW_90% drops from l=4 (depth 0 only) to l=2 (cumulative all depths). BW_99% drops from l=11 to l=6. The point cloud converges to uniform — individual entry positions become LESS informative with depth, yet computation improves. The mechanism is relational (axis alignment between entries, cross-depth algebraic constraints), not positional.
- **Vocabulary is not the bottleneck (Phases 2d, 2e)**: New cross-depth angle vocabulary grows exponentially with depth: 3 → 6 → 10 → 22 → 33 → 72 → 120 → 258 → 381 new distinct angles per depth level (~2x per depth round). The useful subset (<70°) also grows exponentially but its fraction drops from 80% to 32%. The linear depth law is NOT caused by vocabulary running out.
- **Pairwise sum-angle census (Phase 1)**: All subsets (deep-564, strided-564, shallow-564, random-564) produce 2048+ distinct pairwise sum angles. Raw count is not discriminating. However distributions differ: deep skews toward 135-180° range. The informative result is that WHICH angles appear matters, not HOW MANY.
- **XOR winner sum-angle diversity (Phase 2)**: XOR6 winners have mean 2.80 distinct pairwise sum angles; XOR8 winners have 5.18 distinct. Higher-level winners use more angular diversity within each tuple.
- **Direction concentration null result (Phase 3)**: Entries-per-direction is flat at ~2 across all depths. There is no concentration effect — depth does not cause entries to cluster onto fewer directions.
- **Balanced exponentials linearity (PRELIMINARY)**: Supply (vocabulary, entries) grows ~2× per depth round via BFS closure. Demand (parity constraints) grows 4× per XOR weight (2^(2N) masks). Ratio gives ~2 depth rounds per weight → max_xor ≈ depth + 6. Consistent with all data but not analytically derived. (Later confirmed by D91 from the activation side.)

### Theorems/Conjectures

- **Axis Cancellation Mechanism (DEMONSTRATED)**: XOR winners preferentially contain anti-aligned quaternion pairs (axis dot product ≈ -0.75). The partial cancellation of nearly-opposite rotation axes produces low sum angles that create the fine-grained sector separations needed for parity satisfaction. This is the geometric content of "algebraic coherence."
- **Cross-Depth Algebraic Constraint (DEMONSTRATED)**: BFS-derived entries at depth d have constrained pairwise sums with generators: only 73 distinct angles from 50K gen × deep pairs, compared to 1313 for deep × deep. The BFS ancestry creates algebraic relationships that restrict the sum-angle vocabulary between related entries.
- **Spectral Inversion (DEMONSTRATED)**: The S² point cloud bandwidth DECREASES as depth increases. The point cloud converges to spherical uniformity (l=0 dominant in spherical harmonics). Computational power grows while positional information content shrinks — ruling out ALL positional/bandwidth-based explanations for the depth law.
- **Relational Computation Thesis (DEMONSTRATED)**: The depth law is relational: what matters is how entries combine (axis cancellation, cross-depth constraints), not where they individually sit on S². The spectral inversion is the cleanest evidence — positional information degrades monotonically while computation improves linearly.
- **Balanced Exponentials Conjecture (PRELIMINARY)**: BFS branching produces ~2× entries per depth round; parity constraints require 4× masks per XOR weight. The ratio 2:4 = 1:2 predicts ~2 depth per weight → linear slope ≈ 1. Consistent with all data; later supported by D91's activation invariance test.
- **Altschuler-Parrilo Point Cloud Hypothesis (DEAD as formulated)**: The hypothesis that deeper point clouds access higher spherical harmonic modes (giving quadratic capacity growth) is INVERTED — bandwidth shrinks, not grows. The AP framework may still apply via kernel rank analysis of the activation matrix, but the point-cloud-spectrum version is dead.

### Data

- Lattice: ζ₁₂ (pi/6 half-angle), >3000 entries in catalog, 8-9 closure rounds
- Subset size for comparative tests: N=564 (matching D82 protocol)
- **Phase 1 sum-angle census**: deep-564, strided-564, shallow-564, random-564 all produce 2048+ distinct angles (0.01° tolerance). Discriminating feature is distribution shape, not count.
- **Phase 2 winner diversity**: XOR6 winners (3 weights, bf=30): mean 2.80 distinct pairwise sum angles. XOR8 winners (4 weights, bf=20): mean 5.18 distinct.
- **Phase 2b axis cancellation**: XOR8 winners mean(min_dot) = -0.75, non-winners mean(min_dot) = -0.65. Winners mean(mean_dot) lower than non-winners. Sum angle range for winners: 30-66°.
- **Phase 2c cross-depth**: shallow×shallow (d=0-1) = 15 distinct angles. deep×deep (d=7-8) = 1313 distinct. shallow×deep (cross-depth) = 73 distinct from 50K pairs.
- **Phase 2d vocabulary growth**: New distinct angles per depth level: 3→6→10→22→33→72→120→258→381 (~2× per round).
- **Phase 2e useful (<70°) angle growth**: Exponential growth but fraction drops from 80% (depth 1) to 32% (depth 8).
- **Phase 3 direction concentration**: Entries/direction ≈ 2 at all depths (flat, no concentration effect).
- **Phase 5 spectral decomposition**: Per-depth BW_90% decreases from l=4 (d=0) toward l=2 (cumulative). BW_99% decreases from l=11 to l=6. Power concentrates at l=0 (uniform) as depth increases.

### Code Assets

- **`build_closure(verbose)`**: BFS closure of SU(2) generators with depth tracking. Builds `g_cat[]` with `g_depth[]`, `round_start[]`, `round_count[]`. Supports ζ₁₂ catalogs up to MAX_QCAT=4096 entries.
- **`save_catalog()` / `restore_catalog()`**: Snapshot/restore pattern for switching between subsets while preserving the full catalog.
- **`load_deep(n)` / `load_shallow(n)` / `load_strided(n)` / `load_random(n)`**: Four subset-loading strategies for comparative experiments (SUBSET_N=564).
- **`extract_angle(q)`**: Extracts full rotation angle in degrees [0,360) from a (possibly non-unit) quaternion.
- **`extract_axis(q, ax, ay, az)`**: Extracts unit rotation axis from quaternion (normalizing and canonicalizing sign).
- **`axis_dot(p, q)`**: Computes dot product between rotation axes of two quaternions — the key metric for axis cancellation analysis.
- **`count_distinct(angles, n, tol)`**: Counts distinct angles within tolerance, using g_distinct[] buffer (MAX_DISTINCT=2048).
- **`sum_angle_census(label, out_n_distinct)`**: Computes all pairwise quaternion sum angles for current catalog subset, prints histogram (12 bins, 0-180°), min/max/mean, and distinct count.
- **`entry_angle_census(label)`**: Individual entry angle statistics for comparison with sum-angle census.
- **`find_winners(n_weights, bf_limit)`**: Brute-force XOR winner search at given weight count with k-ladder optimization. Stores winning tuples in `g_winners[]`.
- **`phase2b_winning_anatomy()`**: Axis cancellation analysis comparing XOR8 winners vs random non-winners on min_dot, mean_dot, and mean sum angle.
- **`phase2c_cross_depth_sum_angles()`**: Cross-depth sum-angle analysis: shallow×shallow vs deep×deep vs shallow×deep.
- **`phase2d_cross_depth_by_level()`**: Per-depth-level vocabulary growth tracking (gen × depth_d), including cumulative distinct set and Phase 2e low-angle tail (<70°) growth.
- **`assoc_legendre(l, m, x)`**: Associated Legendre polynomial P_l^m(x) for spherical harmonic computation.
- **`ylm_real(l, m, theta, phi)`**: Real spherical harmonic Y_lm evaluation, LMAX=16 (289 coefficients).
- **`phase5_spectral_decomposition()`**: Full spherical harmonic power spectrum P(l) = sum_m f_lm^2 of S² point cloud per depth, with BW_90% and BW_99% bandwidth metrics. Both per-depth and cumulative tables.
- **`combined_cell()` / `vor_cell()` / `test_xor()`**: Standard DKC XOR test infrastructure (Voronoi cell + angular sector combined activation, parity verification).
- **K_LADDER[]**: k-sector ladder {1, 6, 8, 10, 12, 16, 20, 24} for multi-resolution XOR testing.
- **LCG RNG**: Simple deterministic `rng_uniform()` for reproducible random subset generation and random baseline sampling.

### Literature Touched

- **Altschuler & Parrilo (2023)**: Spherical point configurations and polynomial energy — the hypothesis that S² point cloud quality (spherical harmonic bandwidth) drives computational capacity. Demo 90 INVERTS this: bandwidth decreases with depth while computation increases. The AP framework may still apply via kernel rank analysis of the activation matrix rather than the point cloud geometry.
- **Habiro (2002) / Z[ζ₈] bracket**: The algebraic substrate; Demo 90 reveals that BFS ancestry within the group algebra creates cross-depth algebraic constraints (73 distinct angles from 50K gen × deep pairs) that are the concrete manifestation of algebraic coherence.
- **Reservoir Computing (Jaeger 2001, Maass et al. 2002)**: The relational-not-positional finding connects to RC theory — reservoir quality is about the separation property (how inputs map to distinguishable states), not about where reservoir states sit in space. The axis cancellation mechanism is the DKC analog of reservoir separation.
- **BFS closure and Cayley graphs**: The depth-tracking BFS closure is a walk on the Cayley graph of the SU(2) subgroup. The cross-depth algebraic constraint (gen × deep = constrained vocabulary) reflects the algebraic structure of Cayley graph neighborhoods.

### Open Questions

- **Can axis cancellation be exploited for weight selection?** If the mechanism is anti-aligned axis pairs, can we directly select weight tuples by maximizing anti-alignment rather than brute-force XOR testing?
- **Kernel rank analysis (seeded for D91+)**: Apply Altschuler-Parrilo to the Gram matrix of combined_cell activations rather than the S² point cloud. The point cloud spectrum is dead as an explanation, but the activation kernel might reveal quadratic potential.
- **Relational activation**: If the mechanism is axis cancellation (relational), could an activation function that explicitly uses axis dot products between entries break the linear depth law? Current sector/Voronoi activations treat entries independently.
- **Why slope = 1 analytically**: The balanced-exponentials argument (supply ~2×/depth, demand 4×/weight → slope 1) is consistent with data but not derived from first principles. Connecting BFS branching factor to parity constraint growth rate would be a genuine theoretical result.
- **Generalization beyond parity**: Does the linear depth law hold for ALL Boolean functions, or just parity? Different functions might have different constraint growth rates, giving different slopes.
- **Group dependence**: How does the depth law change with a different group (not SU(2))? Different groups have different BFS branching factors and algebraic structures.
