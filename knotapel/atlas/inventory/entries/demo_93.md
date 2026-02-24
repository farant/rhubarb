## Demo 93: Function Scaling — 1-Weight-Per-Input Encoding

- **Status**: COMPLETE (LANDMARK)
- **File**: `demo_93_function_scaling_1wpi/main.c` (~1636 lines)
- **Tests**: 37/37 pass (Phases 1-8)
- **Depends on**: Demo 92 (parity-lock theorem, +/-q encoding is XOR-only), Demo 82-91 (depth-law arc, combined_cell activation framework), Demo 48/50 (forward DKC, 1wpi encoding computes all 13 NPN classes with split-sigmoid/MVN), Demo 80 (finite/infinite boundary)
- **Feeds into**: Demo 94 (binary icosahedral group, solvability as capacity bottleneck), Demo 95 (RKHS kernel computation), activation zoo research, encoding design theory, depth law under phase_cell

### Headline
The 1-weight-per-input encoding under combined_cell is complement-blind (only complement-invariant functions achievable); removing the sign flip (phase_cell) recovers all 13 NPN classes, and a circuit complexity hierarchy emerges as hit-rate divergence: AND/XOR ratio explodes from 1.01 to infinity as N grows from 3 to 8.

### Key Results

- **Complement-Blindness Theorem (Phases 1-4)**: Under combined_cell, mask m and ~m produce sums S and -S; the `if (qa < 0)` normalization maps both to the same cell. ANY function where f(m) != f(~m) is impossible. At odd N, NO standard Boolean function survives (complement of any odd-length mask has different popcount parity). At N=3, only 2 obscure NPN classes (out of 14) are complement-invariant — none contains AND, OR, XOR, or MAJ.
- **Phase 1 — N=3,4,5 under combined_cell**: N=3 (odd): ALL ZERO for every function at every depth. N=4 (even): XOR scales 5 to 4608 (depths 0-8), AND/OR/MAJ all zero. N=5 (odd): ALL ZERO. Confirms complement-blindness: even N allows XOR (complement-invariant), odd N kills everything.
- **Phase 2 — N=6,7,8 under combined_cell**: N=6 (even): XOR 90 to 630. N=7 (odd): ALL ZERO. N=8 (even): XOR 0 to 3, first appearing at depth 4.
- **Phase 3 — Complement-pair cell sharing**: 100% sharing at all N tested (N=3-6, 400-3200 pairs each). Every complement pair maps to the same cell under combined_cell.
- **Phase 4 — Complement-invariant diagnostic (N=3)**: 14 NPN classes enumerated, 16 complement-invariant truth tables, only 2 non-trivial CI NPN classes, neither is AND/OR/XOR/MAJ.
- **Phase 5 — Phase-sensitive activation recovery (N=3)**: Removing the `if (qa < 0)` flip creates phase_cell: sector spans [0,360) not [0,180). Complement-pair sharing drops from 100% to 1.1%. ALL 13 NPN classes recovered (12/13 at depth 0, last at depth 1).
- **Phase 6 — Onset depth sweep (N=4,5,6 under phase_cell)**: Onset depths nearly flat (all d=0 or d=1). Hierarchy manifests as hit rates, not onset depths. N=6 d=1: AND=8867, XOR=1219 (7.3x gap). N=6 d=6: AND=99.4%, XOR=92.2%, MAJ=89.0%.
- **Phase 7 — Hit rate ratio scaling**: AND/XOR ratio at depth 1: 1.01 (N=3), 1.08 (N=4), 1.44 (N=5), 7.27 (N=6). Super-exponential divergence. N=6 XOR growth curve is logarithmic approach to saturation (3.4x to 1.1x growth factors d=1 to d=6). AND/XOR ratio converges with depth: 7.27 to 1.08 (d=1 to d=6). MAJ/XOR crossover at depth ~3-4 (N=6): MAJ easier at low depth (49.6% vs 9.9% at d=1), XOR overtakes at d~4. Depth disproportionately helps parity.
- **Phase 8 — N=7,8 confirmation at depth 1 (17 entries, 84 cells)**: N=7: XOR=4/19448 (0.02%), AND=11049 (56.8%), MAJ=13353 (68.7%). AND/XOR = 2762. N=8: XOR=0/24310 (0.00%), AND=10102 (41.6%), MAJ=585 (2.4%). XOR dead, MAJ cliff. Pigeonhole: 84 cells at depth 1, XOR dies when 2^N > cells (N>=7), AND survives because 1-hot only needs one unique cell.

### Theorems/Conjectures

- **Complement-Blindness Theorem (PROVED + computationally verified)**: Under 1wpi encoding, mask m produces sum S = sum_i (2*bit_i - 1)*q_i, and mask ~m produces -S. combined_cell(S) = combined_cell(-S) due to |qa| normalization. Therefore f(m) must equal f(~m) for any achievable f. AND, OR, MAJ, and XOR-at-odd-N all violate this constraint. Extends D92's Parity-Lock Theorem: encoding provides algebra, activation selects extractable functions.
- **Phase-Cell Recovery (DEMONSTRATED)**: Removing the qa sign flip (phase_cell) breaks S <-> -S symmetry. Complement-pair sharing drops from 100% to 1.1%. All 13 NPN classes become achievable at N=3.
- **Circuit Complexity Hierarchy in DKC (DEMONSTRATED)**: AND (AC^0) degrades gracefully 97% to 42%. MAJ (TC^0) shows cliff at N=8 (2.4%). XOR (parity, not in AC^0) killed by pigeonhole at N=7 (0.02%) and N=8 (0%). The Fourier degree determines the geometric complexity of the required partition. Hierarchy is universal across group structures (confirmed by D94).
- **Pigeonhole Mechanism (DEMONSTRATED)**: With 84 cells at depth 1, XOR requires balanced bisection of 2^N masks into cells. When 2^N > cells, impossible. AND requires only 1 unique cell for mask 1...1. This IS the circuit depth bottleneck in finite discrete systems.

### Data

- **Catalog**: zeta_12, 4096 entries, 9 BFS rounds
- **Phase 8 headline table (depth 1, 17 entries, 84 cells, exhaustive)**:

| N | masks | XOR% | AND% | MAJ% | AND/XOR ratio |
|---|-------|------|------|------|---------------|
| 3 | 8 | 96.8% | 97.7% | 99.7% | 1.01 |
| 4 | 16 | 85.6% | 92.3% | 91.1% | 1.08 |
| 5 | 32 | 58.3% | 84.1% | 94.3% | 1.44 |
| 6 | 64 | 9.9% | 71.7% | 49.6% | 7.27 |
| 7 | 128 | 0.02% | 56.8% | 68.7% | 2762 |
| 8 | 256 | 0.00% | 41.6% | 2.4% | infinity |

- **Phase 1 combined_cell (N=4, even, depths 0-8)**: XOR 5 to 4608; AND/OR/MAJ = 0 at all depths
- **Phase 2 combined_cell (N=6, even)**: XOR 90 to 630; N=8: XOR 0 to 3 (first at depth 4)
- **Phase 3**: 100% complement-pair sharing at all N (combined_cell)
- **Phase 4**: 14 NPN classes, 16 CI truth tables, 2 CI NPN classes, 0 standard functions
- **Phase 5**: 1.1% sharing under phase_cell, 13/13 NPN classes, 12/13 at d=0
- **Phase 6 onset depths**: Nearly flat (d=0 or d=1) for all functions at N=4,5,6
- **Phase 7 N=6 AND/XOR ratio by depth**: d=1: 7.27, d=2: 2.77, d=3: 1.75, d=4: 1.27, d=5: 1.13, d=6: 1.08 (convergence)
- **Phase 7 N=6 XOR growth factors**: d=1 to d=6: 3.4x, 1.6x, 1.4x, 1.1x, 1.1x (logarithmic saturation, not exponential)
- **Phase 7 MAJ/XOR crossover (N=6)**: MAJ easier at d=1 (49.6% vs 9.9%), XOR overtakes at d~4 (76.9% vs 73.6%)
- **K-ladder**: K_SHORT = {6, 12, 24}
- **bf_max limits**: Phase 1: 30/20/15 (N=3/4/5), Phase 2: 12/10/10 (N=6/7/8)
- **Pigeonhole**: 84 cells at depth 1. N=7: 128 masks > 84 cells. N=8: 256 masks > 84 cells.

### Code Assets

- **`phase_cell(sa, sb, sc, sd, k_sec)`**: Phase-sensitive activation — identical to combined_cell but WITHOUT the `if (qa < 0)` sign flip. Sector spans [0,360) instead of [0,180). Breaks complement symmetry, enabling computation of all Boolean functions. THE key activation for the circuit complexity hierarchy.
- **`test_1wpi(indices, n_weights, k_sec, truth_table)`**: 1wpi Boolean function test under combined_cell. Checks whether a weight tuple separates class-0 from class-1 masks. Returns 1 if separable, 0 if not.
- **`test_1wpi_phase(indices, n_weights, k_sec, truth_table)`**: Same as test_1wpi but uses phase_cell instead of combined_cell. The activation used for Phases 5-8.
- **`count_phase_ex(n_weights, bf_limit, truth_table)`**: Exhaustive winner counting under phase_cell across K_SHORT ladder.
- **`sample_phase(n_weights, bf, n_samples, truth_table, rng)`**: Monte Carlo winner sampling under phase_cell for larger N where exhaustive enumeration is intractable.
- **`init_npn()` / `npn_xform()`**: NPN equivalence class enumeration for 3-input Boolean functions. Finds all 14 classes (including constant). Uses all 6 input permutations x 8 input complementations x output negation.
- **`sweep_triple_npn(idx, k_sec, cls_hit)`**: For a given 3-weight triple under phase_cell, enumerates all achievable NPN classes by testing all possible cell-to-output assignments. Used in Phase 5.
- **`comb_nk(n, k)`**: Binomial coefficient computation (long arithmetic).
- **Truth table generators**: `make_xor_tt`, `make_and_tt`, `make_or_tt`, `make_maj_tt` — reusable from D92.
- **`build_closure(verbose)`**: SU(2) group closure with BFS depth tracking. Shared infrastructure.
- **`save_catalog()` / `load_up_to_depth(max_d)`**: Snapshot/filter pattern for cumulative depth experiments.

### Literature Touched

- **Hastad (1986/1987)**: Parity requires exp(Omega(n^{1/(d-1)})) gates at bounded depth. Explains AND/XOR ratio explosion with N: AND is AC^0 (low-degree Fourier), XOR is not (full-degree Fourier). D93 makes this concrete: AND/XOR = 1.01, 1.08, 1.44, 7.27, 2762, infinity for N=3-8.
- **Barrington (1989)**: NC^1 = programs over non-solvable groups. Non-commutativity yields computational power. DKC's multiplicative catalog construction is Barrington-like. The solvable/non-solvable boundary predicts capacity walls (confirmed by D94).
- **LMN / Linial-Mansour-Nisan (1993)**: AC^0 Fourier mass concentrates below degree (log n)^d. Explains AND's robustness to cell count limits (low-degree = simple partition geometry).
- **Furst-Saxe-Sipser (1984)**: Original parity-not-in-AC^0 result. Theoretical backdrop for XOR's special difficulty.
- **Habiro (2002) / Z[zeta_8] bracket**: Algebraic substrate underlying the weight catalog. D93 shows that the readout mechanism (1wpi + activation) determines what is extractable from this substrate.
- **Aizenberg (2008) / MVN neurons**: k-sector activation framework. D93 decomposes the bottleneck: combined_cell's sign-flip symmetry restricts achievable functions; phase_cell (no sign flip) recovers full function hierarchy.
- **D48/D50 (forward DKC NPN classes)**: Critical contrast — 1wpi with split-sigmoid/MVN computes all 13 NPN classes. D93 shows 1wpi with combined_cell is complement-blind; 1wpi with phase_cell also computes all 13 and reveals the circuit complexity hierarchy.
- **Nazer-Gastpar compute-and-forward**: Closest coding theory analog to DKC's additive readout mechanism. DKC = multiplicative catalog construction + additive readout + activation. Novel hybrid model.

### Open Questions

- **Higher k_sec**: Does increasing cell count push the pigeonhole wall to larger N? At k_sec=24 vs 12, cell count doubles — does N=8 XOR become nonzero?
- **Depth law under phase_cell**: Does max_xor ~ depth + 6 still hold without the sign flip? The depth law was established under combined_cell (D82-D91).
- **Activation zoo**: Systematic sweep of activation types (combined_cell, phase_cell, split-sigmoid, MVN sector) x functions x depths. Which activation properties (sign-flip symmetry, sector count, Voronoi resolution) control which aspects of the hierarchy?
- **MAJ cliff mechanism**: MAJ drops from 68.7% (N=7) to 2.4% (N=8) — a 28x collapse. Is this a threshold phenomenon (TC^0-complete meets pigeonhole) or gradual degradation that happens to cross a visibility threshold?
- **XOR growth curve shape**: The N=6 logarithmic saturation (3.4x to 1.1x growth factors) — is this universal across N, or does the shape change? Connection to Barrington scaling.
- **Hybrid encodings**: Can a mixed encoding (some weights +/-q paired, some 1wpi) achieve richer function sets while maintaining depth-law structure?
- **RKHS kernel**: DKC kernel K(m,m') = sum of quaternion inner products. Rank of kernel matrix should predict capacity. Seeded as D95.
