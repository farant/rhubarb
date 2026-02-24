## Demo 100: 4-Strand DKC on W_{4,2} — Non-Semisimple Matrix Braid Representation

- **Status**: COMPLETE (RESULT)
- **File**: `demo_100_4strand_dkc/main.c` (~1749 lines)
- **Tests**: 36 pass, 0 fail (Phases 0-5)
- **Depends on**: Demo 80 (finite/infinite capacity boundary, quantum dimension), Demo 92 (parity-lock theorem), Demo 93 (circuit complexity hierarchy, 1wpi framework), Demo 94 (binary icosahedral group, solvability bottleneck)
- **Feeds into**: Higher-strand W_{n,k} modules, radical-content XOR correlation studies, non-semisimple TL DKC theory, Casimir-guided weight selection

### Headline
4-strand braid group B_4 acting on the 3-dimensional standard module W_{4,2} of TL_4 at delta=0 (non-semisimple) produces an infinite catalog of 3x3 matrices over Z[zeta_8] with exponential BFS growth (~3.1x per round), sustained XOR capacity through depth, and a 6-hub star-like winner topology where all hub commutators are traceless with zero radical content.

### Key Results

- **Phase 0 -- Representation verification (19 tests)**: TL_4 at delta=0 is non-semisimple (n=4 even). Radical dim=9, Loewy length 3. Two simple modules: L_{4,2} (dim 2), L_{4,4} (dim 1). W_{4,2} is the standard module with 2 through-lines, dim=3. Extension: 0 -> L_{4,4} -> W_{4,2} -> L_{4,2} -> 0. Three TL generators e_1, e_2, e_3 verified as 3x3 integer matrices. All satisfy e_i^2 = 0 (delta=0), Jones-Wenzl relations (e_i*e_{i+1}*e_i = e_i), and far commutativity (e_1*e_3 = e_3*e_1). Braid generators sigma_i = A*I + A^{-1}*e_i where A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1). Verified: A*A^{-1} = 1, A^2 + A^{-2} = 0 (delta=0 condition). All sigma_i*sigma_i_inv = I. Braid relations s1*s2*s1 = s2*s1*s2, s2*s3*s2 = s3*s2*s3 verified. Far commutativity s1*s3 = s3*s1 verified. Hecke relations sigma_i - sigma_i_inv = (A-A_inv)(I-e_i) verified for all three generators.
- **Phase 1 -- BFS catalog (1 test)**: 6 generators (sigma_1, sigma_1_inv, sigma_2, sigma_2_inv, sigma_3, sigma_3_inv). BFS hits 32768-entry cap at depth 8 -- group is INFINITE. Depth profile: d0=1, d1=6, d2=26, d3=98, d4=338, d5=1110, d6=3444, d7=10390, d8=17355. Max entry magnitude at cap: 34. sigma_1 has infinite order (max_abs grows linearly with power: n=k gives max_abs=k; sigma_1^8 != I).
- **Phase 2 -- Catalog statistics (1 test)**: Growth rate stabilizes near 3.1x per round (d1: 6.00, d2: 4.33, d3: 3.77, d4: 3.45, d5: 3.28, d6: 3.10, d7: 3.02, d8: 1.67 -- last round truncated by cap). 15.0% of entries (4905/32768) have at least one zero diagonal element.
- **Phase 3 -- XOR capacity scan (3 tests)**: Activation function: 3x3_hash -- sign pattern of all 36 integer components (9 entries x 4 Cyc8 components), 3-valued (pos=2, zero=1, neg=0), hash mod k_param. At bf=30 (first 30 entries): XOR6 peaks at k=512 (2961 solutions), XOR8 also peaks at k=512 (6187), XOR10 peaks at k=512 (541), XOR12 steady at 48 across all k values. Deep sub-catalog (d>=4, bf=30): XOR6=2017, XOR8=1169, XOR10=654, XOR12=305, XOR14=70. Key finding: deep entries SUSTAIN XOR capacity -- XOR14=70 at depth>=4 demonstrates non-trivial computational power at 7-weight XOR.
- **Phase 4 -- Radical analysis (6 tests)**: Radical direction r = (1, 0, -1) verified: e_i * r = 0 for all three TL generators (r spans the L_{4,4} submodule). Mean radical_content = 2 across all depths (constant). Mean |rMr| = 2 across all depths (constant). Mean |C3| (Casimir C3(M) = 3*tr(M^2) - (tr M)^2) grows exponentially with depth: d0=0, d1=0, d2=3, d3=7, d4=16, d5=36, d6=80, d7=176, d8=308. XOR14 winners (70 total) vs control (1000): mean_rad identical (14 vs 14), mean_rMr identical (14 vs 14), mean_C3 slightly higher for winners (172 vs 148). Casimir-by-XOR-level: XOR6 win/ctrl C3 ratio 1.36x, XOR8 ratio 1.86x, XOR10 ratio 1.62x, XOR12 ratio 1.44x -- Casimir consistently higher in winners across all XOR levels.
- **Phase 5 -- Winner anatomy (5 tests)**: 70 XOR14 winners from bf=30 deep sub-catalog. 6 super-hubs (frequency > 50% of winners): entry 4 (91.4%, d=4, w=2), entry 5 (91.4%, d=4, w=2), entry 10 (54.3%, d=5, w=3), entry 21 (65.7%, d=5, w=1), entry 24 (65.7%, d=5, w=-3), entry 25 (54.3%, d=5, w=-1). Hub pairing by trace Cyc8 component: hubs 4,5 are zeta_8^2 pair (traces (0,0,3,0) and (0,0,6,0)), hubs 10,25 are zeta_8^3 pair (traces (0,0,0,-7) and (0,0,0,5)), hubs 21,24 are zeta_8 pair (traces (0,-5,0,0) and (0,3,0,0)). ALL six hub commutators are traceless. ALL six hub commutators have radical_content = 0 (commutators annihilate the radical direction). Co-occurrence graph: 121 edges, max co-occurrence 58, 77 hub-involved edges, 44 satellite-only edges -- NOT a pure star topology. 19 participating entries out of 30, 11 entries with zero frequency (never appear in any winner).

### Theorems/Conjectures

- **Infinite Group from Non-Semisimple TL (CONFIRMED computationally)**: The 4-strand braid group B_4 acting on W_{4,2} (non-semisimple standard module of TL_4 at delta=0) generates an infinite group of 3x3 matrices over Z[zeta_8]. BFS does not close -- hits 32768 cap at depth 8 with sustained ~3.1x growth per round. sigma_1 has infinite order (max_abs grows linearly). This contrasts with the 2-strand case (TL_2 at delta=0) where the catalog is finite.
- **Constant Radical Content (CONFIRMED)**: All catalog entries have mean radical_content = 2 regardless of BFS depth. The radical direction r = (1, 0, -1) is preserved uniformly. This means the non-semisimple structure (the extension 0 -> L_{4,4} -> W_{4,2} -> L_{4,2} -> 0) is equally present in every braid matrix -- "every BFS entry is already non-semisimple" as predicted in the header.
- **Casimir Exponential Growth with Depth (OBSERVED)**: Mean |C3| grows approximately as 2^d (doubling per depth): 0, 0, 3, 7, 16, 36, 80, 176, 308. Casimir measures "non-scalar-ness" of the matrix. Deeper entries are more algebraically complex while maintaining constant radical content.
- **Casimir-XOR Correlation (OBSERVED)**: XOR-winning weight tuples consistently have higher mean Casimir than non-winners, across all XOR levels tested (1.36x-1.86x ratio). Winners need algebraically complex entries. However, radical content does NOT distinguish winners from non-winners (identical at 14 vs 14).
- **Traceless Commutator Universality (CONFIRMED for all hub pairs)**: All six hub commutators [h_i, h_j] have zero trace and zero radical content. The tracelessness is expected for commutators in GL(3). The zero radical content means commutators live entirely in the semisimple quotient L_{4,2} -- they annihilate the radical direction.
- **Hub Pairing by Cyclotomic Component (OBSERVED)**: The 6 super-hubs pair naturally by which Cyc8 basis element dominates their trace: {h0,h1} traces in zeta_8^2, {h2,h5} traces in zeta_8^3, {h3,h4} traces in zeta_8. Within each pair, writhes are conjugate (sum to 4, 2, -2 respectively).

### Data

**TL_4 at delta=0 structure:**
- Radical dim = 9, Loewy length = 3
- Simple modules: L_{4,2} (dim 2), L_{4,4} (dim 1)
- W_{4,2}: dim 3, extension 0 -> L_{4,4} -> W_{4,2} -> L_{4,2} -> 0
- Radical direction: r = (1, 0, -1)

**TL generators (3x3 over Z):**

| Generator | Matrix |
|-----------|--------|
| e_1 | [[0,1,0],[0,0,0],[0,0,0]] |
| e_2 | [[0,0,0],[1,0,1],[0,0,0]] |
| e_3 | [[0,0,0],[0,0,0],[0,1,0]] |

**Braid generators (3x3 over Z[zeta_8]):**
- A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1)
- sigma_i = A*I + A^{-1}*e_i, sigma_i_inv = A^{-1}*I + A*e_i
- 6 generators indexed 0-5: sigma_1, sigma_1_inv, sigma_2, sigma_2_inv, sigma_3, sigma_3_inv

**BFS catalog (cap 32768):**

| Depth | New entries | Cumulative | Growth ratio | Max magnitude |
|-------|------------|------------|--------------|---------------|
| 0 | 1 | 1 | -- | 1 |
| 1 | 6 | 7 | 6.00 | 1 |
| 2 | 26 | 33 | 4.33 | 2 |
| 3 | 98 | 131 | 3.77 | 3 |
| 4 | 338 | 469 | 3.45 | 5 |
| 5 | 1110 | 1579 | 3.28 | 8 |
| 6 | 3444 | 5023 | 3.10 | 13 |
| 7 | 10390 | 15413 | 3.02 | 21 |
| 8 | 17355 | 32768 | 1.67* | 34 |

*Truncated by 32768 cap.

**sigma_1 power growth:**

| n | max_abs(sigma_1^n) |
|---|-------------------|
| 1 | 1 |
| 2 | 2 |
| 3 | 3 |
| 4 | 4 |
| 5 | 5 |
| 6 | 6 |
| 7 | 7 |
| 8 | 8 |

Linear growth confirms infinite order (Jordan block structure).

**Phase 3 -- XOR capacity scan (bf=30, first 30 entries):**

| k_cells | XOR6 | XOR8 | XOR10 | XOR12 |
|---------|------|------|-------|-------|
| 64 | 1158 | 393 | 176 | 48 |
| 128 | 1937 | 822 | 208 | 48 |
| 256 | 2510 | 2376 | 251 | 48 |
| 512 | 2961 | 6187 | 541 | 48 |

**Deep sub-catalog (d>=4, bf=30, k=128):**

| XOR level | Winners |
|-----------|---------|
| XOR6 | 2017 |
| XOR8 | 1169 |
| XOR10 | 654 |
| XOR12 | 305 |
| XOR14 | 70 |

**Radical/Casimir by depth:**

| Depth | mean_rad | mean_rMr | mean_C3 | Count |
|-------|----------|----------|---------|-------|
| 0 | 2 | 2 | 0 | 1 |
| 1 | 2 | 2 | 0 | 6 |
| 2 | 2 | 2 | 3 | 26 |
| 3 | 2 | 2 | 7 | 98 |
| 4 | 2 | 2 | 16 | 338 |
| 5 | 2 | 2 | 36 | 1110 |
| 6 | 2 | 2 | 80 | 3444 |
| 7 | 2 | 2 | 176 | 10390 |
| 8 | 2 | 2 | 308 | 17355 |

**Casimir-by-XOR-level (deep bf=30, k=128):**

| XOR level | Winners | Win C3 | Ctrl C3 | Ratio |
|-----------|---------|--------|---------|-------|
| XOR6 | 2017 | 64 | 47 | 1.36x |
| XOR8 | 1169 | 93 | 50 | 1.86x |
| XOR10 | 654 | 123 | 76 | 1.62x |
| XOR12 | 305 | 148 | 103 | 1.44x |

**XOR14 winners vs control (deep bf=30, k=128):**

| Metric | Winners (70) | Control (1000) |
|--------|-------------|----------------|
| mean_rad | 14 | 14 |
| mean_rMr | 14 | 14 |
| mean_C3 | 172 | 148 |

**Super-hubs (6 hubs, frequency > 50% of 70 winners):**

| Hub | Entry | Freq | Freq% | Depth | Writhe | Trace | |C3| | Frob L1 |
|-----|-------|------|-------|-------|--------|-------|------|---------|
| 0 | 4 | 64 | 91.4% | 4 | 2 | (0,0,3,0) | 0 | 15 |
| 1 | 5 | 64 | 91.4% | 4 | 2 | (0,0,6,0) | 36 | 19 |
| 2 | 10 | 38 | 54.3% | 5 | 3 | (0,0,0,-7) | 56 | 17 |
| 3 | 21 | 46 | 65.7% | 5 | 1 | (0,-5,0,0) | 20 | 19 |
| 4 | 24 | 46 | 65.7% | 5 | -3 | (0,3,0,0) | 0 | 21 |
| 5 | 25 | 38 | 54.3% | 5 | -1 | (0,0,0,5) | 20 | 11 |

**Hub commutators (all traceless, all radical_content = 0):**

| Pair | Trace | |C3| | Rad | Frob L1 |
|------|-------|------|-----|---------|
| [h0,h1] zeta_8^2 pair | (0,0,0,0) | 24 | 0 | 44 |
| [h2,h5] zeta_8^3 pair | (0,0,0,0) | 426 | 0 | 55 |
| [h3,h4] zeta_8 pair | (0,0,0,0) | 216 | 0 | 86 |
| [h0,h3] top x mid | (0,0,0,0) | 96 | 0 | 84 |
| [h0,h2] top x low | (0,0,0,0) | 384 | 0 | 84 |
| [h3,h2] mid x low | (0,0,0,0) | 552 | 0 | 94 |

**Co-occurrence topology:**
- 121 edges total, max co-occurrence 58
- 77 hub-involved edges, 44 satellite-only edges
- NOT a pure star topology
- 19/30 entries participate in at least one winner

### Code Assets

- **`Cyc8` (Z[zeta_8] cyclotomic arithmetic)**: Exact integer type representing a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 with zeta_8^4 = -1. Operations: `cyc8_make`, `cyc8_add`, `cyc8_sub`, `cyc8_neg`, `cyc8_mul`, `cyc8_eq`, `cyc8_is_zero`, `cyc8_conj`, `cyc8_max_abs`. Multiplication uses the relation zeta_8^4 = -1 for reduction. Fully reusable for any computation in Z[zeta_8].
- **`Mat3` (3x3 matrix over Cyc8)**: Matrix type for W_{4,2} representation. 9 Cyc8 entries = 36 long integers. Operations: `mat3_zero`, `mat3_identity`, `mat3_mul`, `mat3_eq`, `mat3_add`, `mat3_neg`, `mat3_scale`, `mat3_max_abs`, `mat3_print`. All operations exact -- no floating point.
- **Hash table for Mat3 BFS**: FNV-style hash over 36 long components, chained-bucket lookup. MAX_CAT = 32768, HASH_SIZE = 65537. Used for BFS deduplication of matrix catalog.
- **`build_tl_generators()`**: Constructs 3x3 integer matrices e_1, e_2, e_3 for TL_4 on W_{4,2} basis {h_0, h_1, h_2} (three half-diagrams with 2 through-lines).
- **`build_braid_generators()`**: Constructs 6 braid generators sigma_i = A*I + A^{-1}*e_i and sigma_i_inv = A^{-1}*I + A*e_i over Z[zeta_8]. Tracks writhe (+1 for sigma, -1 for sigma_inv).
- **`build_catalog(max_depth)`**: BFS closure with 6 generators. Overflow protection at max_abs > 10^11. Reports per-round growth and closure/cap/depth-limit status.
- **`mat3_activate(m, k_param)`**: 3x3_hash activation -- sign pattern of all 36 integer components (3-valued: pos/zero/neg), base-3 hash mod k_param. Novel activation function designed for matrix-valued DKC.
- **`test_xor_mat3(indices, n_weights, k_param)`**: 1wpi XOR test for Mat3 catalog. N weights -> 2N inputs. Signed sum in Mat3 space, activation via mat3_activate. Touch-list cleanup pattern for efficient cell reuse.
- **`count_xor_bf(n_weights, k_param, bf_limit)`**: Brute-force combinatorial XOR search over C(bf,n_weights) tuples. Supports n_weights = 3-7 (XOR6 through XOR14).

### Literature Touched

- **Temperley-Lieb algebra at delta=0 (non-semisimple regime)**: TL_n at delta=0 with n even is the most singular point -- radical has maximal dimension, Loewy length reaches maximum. Standard modules W_{n,k} are indecomposable but not simple. D100 works with W_{4,2} (dim 3), which has the non-split extension 0 -> L_{4,4} -> W_{4,2} -> L_{4,2} -> 0. Every braid matrix preserves this structure (constant radical content = 2 at all depths).
- **Braid group representations via Kauffman bracket**: sigma_i = A*I + A^{-1}*e_i gives a representation of the braid group B_n on TL_n modules. At the special value A = -zeta_8 (corresponding to delta = -(A^2 + A^{-2}) = 0), this representation is infinite-dimensional on W_{4,2}. The linear growth of max_abs(sigma_1^n) = n indicates a non-trivial Jordan block in the representation.
- **Habiro (2002) / Z[zeta_8] bracket**: The algebraic substrate. D100 extends the scalar Z[zeta_8] bracket (used in D80-D94) to matrix-valued computation: each catalog entry is now a 3x3 matrix over Z[zeta_8] rather than a single Z[zeta_8] element. The DKC signed-sum framework generalizes directly to matrix addition.
- **Casimir invariant as complexity measure**: The modified Casimir C3(M) = 3*tr(M^2) - (tr M)^2 measures deviation from scalar matrices. Its exponential growth with BFS depth (~2x per round) while radical content stays constant shows that braid words become algebraically richer without changing their module-theoretic character. Its correlation with XOR capacity (1.36x-1.86x higher in winners) suggests Casimir as a heuristic for weight selection in matrix DKC.
- **Reservoir computing connection**: The infinite catalog (vs finite groups in D80-D94) means the reservoir has unbounded size. The 3x3_hash activation is a novel nonlinear readout designed for matrix-valued reservoirs. The 36-component sign pattern provides much finer discrimination than the scalar phase_cell used in D80-D94.

### Open Questions

- **Higher-strand generalization**: W_{6,2} (dim 5) and W_{6,4} (dim 9) on TL_6 at delta=0 -- does the Casimir-XOR correlation persist? Does the hub pairing structure generalize to larger representations?
- **Optimal activation for matrix DKC**: The 3x3_hash activation (sign pattern of 36 components) is ad hoc. Is there a natural activation that respects the module structure (e.g., projects onto radical vs semisimple components separately)?
- **Radical-annihilating commutators**: All hub commutators have zero radical content. Is this a general theorem (commutators of braid matrices on W_{n,k} always annihilate the radical) or specific to these hub entries?
- **Connection to D80-D94 scalar DKC**: Can the matrix DKC be reduced to scalar DKC by projecting onto specific module components? The radical content being constant suggests the extension structure adds something that projection would lose.
- **Casimir-guided weight selection**: Can Casimir be used as a pre-filter to reduce the combinatorial search space for XOR-winning tuples? The 1.36x-1.86x ratio suggests moderate predictive power but not strong enough to be a standalone selector.
- **Cap-free growth rate**: The d=8 growth ratio (1.67x) is depressed by the 32768 cap. What is the true asymptotic growth rate? The stabilization toward ~3.0x suggests the free growth rate may be exactly 3 (related to 3 non-adjacent generator pairs: sigma_1, sigma_2, sigma_3).
