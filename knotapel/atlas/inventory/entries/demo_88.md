## Demo 88: Anti-Correlation Mechanism

- **Status**: COMPLETE
- **File**: `demo_88_anti_correlation/main.c` (~1175 lines)
- **Tests**: 8/8 pass
- **Depends on**: Demo 72 (Voronoi optimization destroys computation, 36->4 XOR6), Demo 84 (null states and direction classification: 6 null-only + 3 shared + 4 non-null), Demo 87 (null indispensability at zeta_12)
- **Feeds into**: Demo 89 (depth law mechanism), reservoir computing robustness pillar, future constrained optimization work, zeta_12 Sec*Vor optimization priority assessment

### Headline
The algebraic directions that are worst for spherical integration are best for DKC computation because the 4 non-null body-diagonal directions are the rigid computational anchors while the 6 null-only edge-midpoint directions are flexible topological scaffolding; constrained optimization (nulls free, non-nulls clamped to 2 deg) achieves 48% design improvement with only 2.8% XOR loss, and the k-ladder activation provides 14x better perturbation resilience than D72's Voronoi-only activation (6.4% vs 89% loss under design-improving gradient).

### Key Results

- **Phase 1 (Geometric Characterization)**: The 13 zeta_8 directions form a cuboctahedron on S^2. Three orbits under octahedral symmetry: 3 coordinate axes (shared, mutual angle 90 deg), 4 body diagonals (non-null-only, mutual angle 70.5 deg = arccos(1/3)), 6 edge midpoints (null-only, mutual angle 60 deg or 90 deg). Pairwise angle statistics: min=35.3 deg, max=90.0 deg, mean=63.1 deg over 78 pairs.
- **Phase 1 (Gram eigenspectrum)**: The 13x13 |dot| Gram matrix has eigenvalues including golden ratio pairs (phi=1.618 x2 and -phi+1=-0.618 x2), leading eigenvalue 6.110, three eigenvalues at 2.000, three at -1/3. Negative eigenvalues expected for |dot| kernel on RP^2 (not PSD). The golden ratio eigenvalues arise from the specific angular ratios of zeta_8's three Oh orbits (54.7/45/35.3 deg).
- **Phase 1 (Spherical design quality)**: Residual(t=6) = 2.225. NOT a 2-design (residual > 0.01). The algebraic directions are among the worst possible for 13 points on S^2 in terms of spherical integration quality.
- **Phase 2 (Algebraic Characterization)**: Direction partition confirmed: 6 null-only (type 0, cube edge midpoints), 3 shared (type 1, coordinate axes), 4 non-null-only (type 2, body diagonals). Intra-group mean angles: null-only=66.0 deg, non-null=70.5 deg, shared=90.0 deg, null-to-non-null inter=62.6 deg. Null and non-null groups have geometrically distinct clustering (difference > 1.0 deg).
- **Phase 3 SURPRISE (Perturbation Sensitivity)**: Baseline XOR6 = 1239 out of 2024 triples. At eps=10 deg: null-only perturbation causes -2.4% loss (i.e., a GAIN -- perturbing nulls slightly improves computation), non-null-only perturbation causes 8.0% loss. The prediction that null directions would be more sensitive (radical-driven anti-correlation) was INVERTED. Non-null body-diagonal directions are the fragile, load-bearing computational anchors. Null edge-midpoint directions are redundant scaffolding.
- **Phase 3b (Unconstrained design gradient)**: Starting from algebraic directions, 5000-step simulated-annealing optimization toward lower spherical design residual: 52% residual improvement (2.23 -> 1.06) with only 6.4% XOR loss (1239 -> 1160). This is dramatically less catastrophic than D72's 89% destruction (36 -> 4).
- **Phase 3e (Constrained optimization -- KEY RESULT)**: Null directions move freely, non-null and shared clamped to 2 deg from original. Result: 48% design improvement (residual 2.23 -> 1.16) with only 2.8% XOR loss (1239 -> 1204). Null directions carry nearly all the design-improvement capacity (48% vs 52% unconstrained) while non-null rigidity cuts computational loss in half (2.8% vs 6.4%).
- **Phase 4 (Cross-root comparison)**: zeta_8 has 13 directions with Res(t=6)=2.225, Res/N=0.171. zeta_12 has 2043 directions with Res(t=6)=42.400, Res/N=0.021. Per-direction residual at zeta_12 is 8x lower than zeta_8. More directions = better per-direction integration = nulls dispensable at larger groups. Connects directly to Demo 87's dispensability finding.
- **K-Ladder Robustness (standalone insight)**: D72 (Voronoi-only activation) shows catastrophic 89% destruction under design-improving optimization. D88 (k-ladder activation with K_LADDER={1,6,8,10,12,16,20,24}) shows only 6.4% loss for the same gradient. The k-ladder provides perturbation resilience: Voronoi-only gives each triple ONE chance at a specific angular resolution (knife-edge sensitivity), k-ladder gives each triple 8 chances at different resolutions (small direction shifts absorbed by switching k_sec). The Sec*Vor activation introduced for capacity also provides computational robustness. Dual benefit.

### Theorems/Conjectures

- **Two-Role Direction Theorem (DEMONSTRATED)**: In the zeta_8 cuboctahedral direction set, the 13 directions serve two distinct computational roles: the 4 non-null body-diagonal directions are rigid computational anchors (perturbation-sensitive, load-bearing for XOR), and the 6 null-only edge-midpoint directions are flexible topological scaffolding (perturbation-tolerant, movable, provide S^2 coverage for spherical integration).
- **Constrained Optimality (DEMONSTRATED)**: Constraining non-null directions to within 2 deg of algebraic placement while allowing null directions to move freely captures 92% of unconstrained design improvement (48% vs 52%) while reducing computational loss by 56% (2.8% vs 6.4%).
- **K-Ladder Resilience Theorem (DEMONSTRATED)**: The Sec*Vor k-ladder activation provides an order-of-magnitude better perturbation resilience than Voronoi-only activation under the same design-improving gradient (6.4% loss vs 89% loss). The mechanism is that multiple k_sec values provide fallback resolutions when direction perturbation invalidates the optimal sector assignment.
- **Anti-Correlation Persistence Conjecture (SUPPORTED)**: The anti-correlation between spherical design quality and computational power persists at zeta_12 (per-direction residual 8x lower but still nonzero), though the mechanism shifts from null-direction-driven to sheer direction-count-driven as the group grows.
- **Activation Function as Variable (CONFIRMED)**: The dramatic difference between D72 (89% loss) and D88 (6.4% loss) under the same geometric optimization confirms that the activation function (Voronoi-only vs Sec*Vor k-ladder) is the primary variable controlling perturbation sensitivity, not the optimization procedure itself.

### Data

- zeta_8 catalog: 24 entries, 13 directions
- Direction partition: 6 null-only + 3 shared + 4 non-null-only = 13
- Cuboctahedral orbits: 3 coordinate axes (90 deg mutual), 4 body diagonals (70.5 deg mutual), 6 edge midpoints (60/90 deg mutual)
- Pairwise angle stats: min=35.3 deg, max=90.0 deg, mean=63.1 deg (78 pairs)
- Gram eigenspectrum: 6.110, 2.000(x3), phi(x2), 0.520, -1/3(x3), -phi+1(x2), -0.630
- Spherical design residual(t=6): 2.225 (NOT a 2-design)
- Baseline XOR6 count: 1239 / 2024 triples
- Perturbation at eps=10 deg: null-only = -2.4% loss (gain), non-null-only = 8.0% loss, all-random = 3.7% loss
- Unconstrained design gradient (5000 steps): residual 2.23 -> 1.06 (52% improvement), XOR6 1239 -> 1160 (6.4% loss)
- Constrained optimization (nulls free, non-nulls clamped 2 deg): residual 2.23 -> 1.16 (48% improvement), XOR6 1239 -> 1204 (2.8% loss)
- D72 Voronoi-only design optimization: 36 -> 4 XOR6 solutions (89% loss)
- zeta_12: 2043 directions, Res(t=6)=42.400, Res/N=0.021 (8x lower per-direction than zeta_8)
- zeta_12 pairwise angle (sampled): min~0 deg, max~90 deg, mean=57.3 deg
- K_LADDER = {1, 6, 8, 10, 12, 16, 20, 24} (8 values)

### Code Assets

- **`build_closure(verbose)`**: SU(2) group closure from two generators with inverse augmentation; returns catalog size in `g_cat_size`, round count in `n_rounds`; supports verbose output per round
- **`classify_null(cat_size)`**: classifies each catalog entry as null (|Re(q)| < 1e-10) or non-null; fills `is_null[]` array with counts `n_null_total`, `n_nonnull_total`
- **`build_dirs(cat_size)`**: extracts distinct S^2 axis directions from catalog (antipodal-identified); populates `g_dir[]` with `g_nd` count
- **`classify_directions(cat_size)`**: per-direction null/non-null counts; classifies each direction as null-only (type 0), shared (type 1), or non-null-only (type 2); fills `dir_type[]`, counts `n_null_only_dirs`, `n_shared_dirs`, `n_nonnull_only_dirs`
- **`combined_cell(sa,sb,sc,sd,k_sec)`**: Sec*Vor activation function mapping quaternion sum to integer cell index via angular sector (k_sec divisions of [0,360)) crossed with Voronoi nearest-direction; returns `sec * (n_dir+1) + vor`
- **`test_xor(indices, n_weights, k_sec)`**: XOR parity test for a given weight tuple and k_sec; uses cell-based even/odd accumulation with touched-cell cleanup
- **`count_xor6(bf_limit)`**: brute-force XOR6 counter over all 3-element subsets with k-ladder fallback (tests 8 k_sec values, counts success on first hit)
- **`design_residual(dirs, nd, t)`**: spherical t-design residual via real spherical harmonics Y_l^m; max over all (l,m) with 1<=l<=t of |sum_i Y_l^m(dir_i)|; uses `assoc_legendre` and `ylm_real`
- **`jacobi_eigen(a, n, evals)`**: Jacobi eigendecomposition for small symmetric matrices (up to 200 iterations); returns sorted (descending) eigenvalues
- **Phase 3 perturbation framework**: Gaussian direction perturbation with normalize3; supports selective perturbation by direction type (null-only, non-null-only, or all); constrained mode with angular clamp relative to original algebraic placement
- **Simulated annealing optimizer**: temperature-decay random jitter, accept-if-better for spherical design residual; with optional angular clamping for constrained optimization

### Literature Touched

- **Demo 72 (Voronoi Design Destruction)**: D72 showed that optimizing 13 eigenvector directions for spherical design quality using Voronoi-only activation destroys computation (36->4 XOR6, 89% loss). D88 identifies the mechanism: the activation function (Voronoi-only vs k-ladder) is the variable, and k-ladder provides order-of-magnitude better resilience.
- **Demo 84 (Null States)**: D84 established the 6+3+4 direction partition (null-only, shared, non-null-only) and the directional efficiency of null entries. D88 extends this by showing the partition maps to distinct computational roles (scaffolding vs anchors).
- **Demo 87 (Null Indispensability at zeta_12)**: D87 showed nulls become dispensable at zeta_12. D88's Phase 4 cross-root comparison provides the geometric explanation: per-direction residual is 8x lower at zeta_12, so null directions are no longer needed for coverage.
- **Spherical t-designs (Delsarte, Goethals, Seidel 1977)**: the design residual metric quantifies how far a point set is from a t-design; D88 shows the algebraic directions are maximally far from a 2-design but this is precisely what makes them computationally optimal.
- **Cuboctahedron / Archimedean solids**: the 13 zeta_8 directions form a cuboctahedron (3+4+6 orbits under Oh symmetry); recognized as the complete orbit structure of the octahedral group acting on RP^2.
- **Golden ratio in eigenspectrum**: phi and -phi+1 eigenvalue pairs in the |dot| Gram matrix arise from the specific angular ratios of the three Oh orbits, connecting Fibonacci-like structure to the octahedral geometry.

### Open Questions

- At zeta_12 with 2043 directions, is there ANY subset that is computationally load-bearing (analogous to the 4 body-diagonal anchors at zeta_8), or is the direction set fully redundant?
- The algebraic placement is slightly off-peak for null directions (~6% improvement accessible at eps=10 deg, with the -2.4% gain suggesting room for optimization). Is this universal across roots or specific to zeta_8?
- Does the k-ladder robustness finding change the priority of "zeta_12 Sec*Vor optimization" as a research direction? (The resilience may make optimization less necessary.)
- Can the constrained optimization approach (flexible nulls + rigid non-nulls) be formulated as a principled optimization problem rather than simulated annealing, e.g., as a semidefinite program preserving non-null directions?
- The golden ratio eigenvalue pairs (phi, -phi+1) in the Gram spectrum: do these appear at other roots (zeta_10, zeta_12), or are they specific to zeta_8's octahedral geometry?
