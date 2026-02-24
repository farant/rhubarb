## Demo 101: 5-Strand DKC Scaling Verification on W_{5,3} — sl_d Functor Thesis

- **Status**: COMPLETE (RESULT)
- **File**: `demo_101_5strand_dkc/main.c` (~1569 lines)
- **Tests**: 44 pass, 0 fail (Phases 0-5)
- **Depends on**: Demo 80 (finite/infinite capacity boundary, quantum dimension), Demo 93 (circuit complexity hierarchy, 1wpi framework), Demo 100 (4-strand DKC on W_{4,2}, non-semisimple matrix braid representation, radical analysis, Casimir-XOR correlation, hub anatomy methodology)
- **Feeds into**: Higher-strand W_{n,k} modules, sl_d functor series completion, radical-free vs radical-containing DKC comparison, Casimir-XOR correlation in simple modules, hub-count scaling law

### Headline
PREDICTION-DENSE DEMO testing the sl_d functor thesis. 5-strand braid group B_5 acting on W_{5,3} (the SIMPLE standard module of TL_5 at delta=0 -- no radical, key structural difference from D100) produces an infinite catalog of 4x4 matrices over Z[zeta_8] with ~4x BFS growth per round (CONFIRMING the growth-rate prediction), sustained XOR6-XOR12 capacity, and all-traceless hub commutators. However, the hub-count prediction is FALSIFIED (3 hubs, not 12), XOR14 vanishes entirely (vs 70 in D100), and the Casimir-XOR correlation INVERTS at higher XOR levels. The simple module computes differently from D100's non-semisimple module: the radical extension is computationally load-bearing, not decorative.

### Key Results

- **Phase 0 -- Representation verification (30 tests)**: W_{5,3} is the 4-dimensional standard module of TL_5 at delta=0. KEY STRUCTURAL DIFFERENCE: W_{5,3} is SIMPLE (no radical). ker(e_1) intersect ... intersect ker(e_4) = {0} (radical dimension verified = 0). Basis: four half-diagrams for 5 strands with 3 through-lines (h_0: {1,2} paired, h_1: {2,3} paired, h_2: {3,4} paired, h_3: {4,5} paired). Four TL generators e_1..e_4 as 4x4 integer matrices, following A_3 Dynkin diagram (path graph) pattern. All satisfy e_i^2 = 0 (delta=0), Jones-Wenzl relations (e_i*e_{i+1}*e_i = e_i and e_{i+1}*e_i*e_{i+1} = e_{i+1} for 3 adjacent pairs), and far commutativity (e_1*e_3, e_1*e_4, e_2*e_4). Braid generators sigma_i = A*I + A^{-1}*e_i where A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1). Verified: A*A^{-1} = 1, A^2 + A^{-2} = 0 (delta=0 condition). All four sigma_i*sigma_i_inv = I. Braid relations s_i*s_{i+1}*s_i = s_{i+1}*s_i*s_{i+1} for 3 adjacent pairs. Far braid commutativity for 3 far pairs. Hecke relations s_i - s_i_inv = (A-A_inv)(I-e_i) for all 4 generators.
- **Phase 1 -- BFS catalog (1 test)**: 8 generators (sigma_1..4 and inverses). BFS hits 32768-entry cap at depth 7 -- group is INFINITE. Depth profile: d0=1, d1=8, d2=44, d3=206, d4=884, d5=3600, d6=14198, d7=13827. Max entry magnitude at cap: 21. Growth rate confirms prediction: ~4x per round (8/1=8.00, 44/8=5.50, 206/44=4.68, 884/206=4.29, 3600/884=4.07, 14198/3600=3.94, 13827/14198=0.97* -- last round truncated by cap).
- **Phase 2 -- Catalog statistics (1 test)**: Growth rate stabilizes near 4x per round (contrasting with D100's ~3.1x for 6 generators/3-dim). sigma_1 has infinite order: max_abs grows linearly with power (n=1: 1, n=2: 2, n=3: 3, n=4: 4, n=5: 5, n=6: 6, n=7: 7, n=8: 8 -- exactly linear, matching D100's pattern). sigma_1^8 != identity confirmed. Linear max_abs growth indicates Jordan block structure in the representation.
- **Phase 3 -- XOR capacity scan (2 tests)**: Activation function: 4x4_hash -- sign pattern of all 64 integer components (16 entries x 4 Cyc8 components), 3-valued (pos=2, zero=1, neg=0), hash mod k_param. K-sweep at bf=30 (first 30 entries): XOR6 peaks at k=512 (3297), XOR8 peaks at k=512 (12229), XOR10 peaks at k=512 (5729), XOR12 peaks at k=512 (512). Deep sub-catalog (d>=4, bf=30, k=128): XOR6=2579, XOR8=2228, XOR10=203, XOR12=27, XOR14=0. XOR14 ZERO at all k values (k=256 through 4096). Key finding: deep entries sustain XOR6-XOR12 capacity but XOR14 vanishes entirely -- contrasting with D100 where XOR14=70 on 3x3 matrices.
- **Phase 4 -- Casimir analysis (3 tests)**: No radical (W_{5,3} simple), so no radical_content analysis (unlike D100). C4(M) = 4*tr(M^2) - (tr M)^2 computed integer-exact for all 32768 entries. Mean |C4| = 91 across full catalog. C4 by depth: d0=0, d1=0, d2=4, d3=9, d4=20, d5=41, d6=83, d7=118 -- approximately 2x per round, matching D100's exponential growth pattern. XOR10 winners (203) vs control (1000): mean C4 = 57 vs 67 -- winners have LOWER Casimir (ratio 0.85x). Casimir-by-XOR-level: XOR6 ratio 1.17x (winners higher), XOR8 ratio 1.51x (winners higher), XOR10 ratio 0.89x (winners LOWER), XOR12 ratio 0.54x (winners LOWER). INVERSION of D100 pattern: Casimir-XOR correlation reverses at higher XOR levels in simple modules.
- **Phase 5 -- Hub anatomy (7 tests)**: 203 XOR10 winners from deep bf=30 sub-catalog. Only 3 super-hubs (frequency > 1/3 of winners) -- NOT the predicted 12. Hub 0: entry 0, freq=87 (42.9%), d=4, w=4, |C4|=0, Frob=8. Hub 1: entry 26, freq=137 (67.5%), d=5, w=5, |C4|=0, Frob=9. Hub 2: entry 27, freq=142 (70.0%), d=5, w=3, |C4|=11, Frob=12. Hub traces by Cyc8 component: hub 0 real (-4,0,0,0), hub 1 zeta_8 (0,4,0,0), hub 2 zeta_8^3 (0,0,0,-5). All 3 hub-pair commutators traceless (3/3). First commutator [hub_0, hub_1] is the ZERO MATRIX (hubs 0 and 1 COMMUTE). Co-occurrence: 247 edges, max co-occurrence 102, 62 hub-involved edges, 185 satellite-only edges -- strongly NOT a star topology. XOR12: 27 winners, entry 0 and entries 26-27 dominate (freq=26 each). sl_4 hub-count prediction FALSIFIED: 3 hubs, not 12.

### Theorems/Conjectures

- **sl_d Functor Thesis (PARTIALLY CONFIRMED, PARTIALLY FALSIFIED)**: Growth rate prediction CONFIRMED: ~4x per round converging toward 4 as expected for sl_4 (D100 showed ~3.1x for sl_3). Hub count prediction FALSIFIED: only 3 super-hubs found (not the predicted 12 = 6 pairs for sl_4). The growth rate tracks n-1 cleanly across three data points (2, 3, 4), but the hub count does NOT scale as 2*C(n-1,2). The discrepancy may be due to the simple-module context (W_{5,3} has no radical) vs D100's non-semisimple module (W_{4,2} has radical), or to the bf=30 deep-entry sampling which may miss hubs at other depths.
- **Simple Module DKC (DEMONSTRATED)**: W_{5,3} is simple (radical dimension = 0), in contrast to D100's W_{4,2} which is non-semisimple (radical dim > 0, Loewy length 3). DKC still produces infinite catalogs and sustained XOR6-XOR12 capacity. However, the detailed structure differs: only 3 hubs (vs 6 for D100), XOR14 vanishes entirely (vs 70 for D100), and the Casimir-XOR correlation INVERTS at higher XOR levels. Simplicity changes the computational character, not just removes the radical.
- **BFS Growth Rate Prediction (CONFIRMED)**: Predicted ~4x per round for 4 TL generators (n-1=4 for 5 strands). Observed: 8.00, 5.50, 4.68, 4.29, 4.07, 3.94 -- converging toward 4 as expected. D100 showed ~3.1x converging toward 3 for 3 generators. The growth rate tracks the number of TL generators (n-1), consistent with the sl_{n-1} thesis. This is now confirmed across THREE data points (n=3,4,5).
- **No Radical Content, Still Computes (CONFIRMED)**: All catalog entries live in a simple module -- there is no radical to measure. XOR6-XOR12 capacity is sustained through depth. However, XOR14=0 everywhere suggests a capacity ceiling that D100 (non-semisimple) does not have. The radical extension in D100 may provide additional computational degrees of freedom that the simple module lacks.
- **Traceless Commutator Universality (CONFIRMED for all 3 hub pairs)**: All 3 super-hub commutators are traceless (3/3). Moreover, [hub_0, hub_1] is the ZERO MATRIX -- these two hubs actually commute. This is stronger than tracelessness and suggests hub_0 and hub_1 live in a commutative subalgebra. Note that hub_0 (entry 0) is near-scalar: diagonal (-1,-1,-1,-1) with a single off-diagonal (0,0,4,0) entry. Hub_1 is similarly structured: diagonal (0,1,0,0) entries with a single off-diagonal (0,0,0,-5).
- **Casimir-XOR Correlation INVERSION (NEW FINDING)**: D100 showed winners consistently have HIGHER Casimir (1.36x-1.86x ratio). D101 shows the pattern INVERTS: XOR6 ratio 1.17x and XOR8 ratio 1.51x (winners higher, matching D100), but XOR10 ratio 0.89x and XOR12 ratio 0.54x (winners LOWER). In the simple module, the hardest XOR computations select for LOW-Casimir entries. This is the opposite of D100's non-semisimple module. Possible explanation: in simple modules, near-scalar entries (low C4) provide the stability that the radical direction provided in D100.
- **XOR14 Vanishing in Simple Module (NEW FINDING)**: XOR14=0 at ALL k values tested (128 through 4096) on the deep sub-catalog. D100 achieved XOR14=70 on 3x3 matrices with the same bf=30 and k=128. The simple module (no radical) has strictly less XOR14 capacity than the non-semisimple module. This suggests the non-semisimple extension structure in D100 is computationally load-bearing at the XOR14 boundary, not just algebraic decoration.

### Data

**W_{5,3} module structure:**
- TL_5 at delta=0, n=5 (ODD) -- W_{5,3} is SIMPLE
- dim(W_{5,3}) = 4 (Catalan path count for 5 strands, 3 through-lines)
- Radical dimension = 0 (ker(e_1) intersect ... intersect ker(e_4) = {0})
- Basis: 4 half-diagrams h_0..h_3 (one pair each from {1,2}, {2,3}, {3,4}, {4,5})

**TL generators (4x4 over Z):**

| Generator | Matrix | Pattern |
|-----------|--------|---------|
| e_1 | [[0,1,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]] | h_1 -> h_0 |
| e_2 | [[0,0,0,0],[1,0,1,0],[0,0,0,0],[0,0,0,0]] | h_0 -> h_1, h_2 -> h_1 |
| e_3 | [[0,0,0,0],[0,0,0,0],[0,1,0,1],[0,0,0,0]] | h_1 -> h_2, h_3 -> h_2 |
| e_4 | [[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,1,0]] | h_2 -> h_3 |

**Braid generators (4x4 over Z[zeta_8]):**
- A = -zeta_8 = (0,-1,0,0), A^{-1} = (0,0,0,1)
- sigma_i = A*I + A^{-1}*e_i, sigma_i_inv = A^{-1}*I + A*e_i
- 8 generators indexed 0-7: sigma_1, sigma_1_inv, sigma_2, sigma_2_inv, sigma_3, sigma_3_inv, sigma_4, sigma_4_inv
- Writhe tracking: +1 for sigma, -1 for sigma_inv

**BFS catalog (cap 32768):**

| Depth | New entries | Cumulative | Growth ratio | Max magnitude |
|-------|------------|------------|--------------|---------------|
| 0 | 1 | 1 | -- | 1 |
| 1 | 8 | 9 | 8.00 | 1 |
| 2 | 44 | 53 | 5.50 | 2 |
| 3 | 206 | 259 | 4.68 | 3 |
| 4 | 884 | 1143 | 4.29 | 5 |
| 5 | 3600 | 4743 | 4.07 | 8 |
| 6 | 14198 | 18941 | 3.94 | 13 |
| 7 | 13827 | 32768 | 0.97* | 21 |

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

Linear growth confirms infinite order (Jordan block structure). Matches D100 exactly.

**Growth rate comparison across strand count (sl_d thesis):**

| n (strands) | Demo | Module | dim | Generators | Growth rate | Predicted (sl_{n-1}) |
|-------------|------|--------|-----|------------|-------------|---------------------|
| 3 | D98 | TL_3 | 5x5 | 4 | ~2.2x | 2 (sl_2) |
| 4 | D100 | W_{4,2} | 3x3 | 6 | ~3.1x | 3 (sl_3) |
| 5 | D101 | W_{5,3} | 4x4 | 8 | ~4.0x | 4 (sl_4) |

**Phase 3 -- XOR capacity scan (bf=30, first 30 entries):**

| k_cells | XOR6 | XOR8 | XOR10 | XOR12 |
|---------|------|------|-------|-------|
| 64 | 1916 | 1330 | 732 | 441 |
| 128 | 2600 | 3316 | 844 | 441 |
| 256 | 3080 | 7918 | 1876 | 465 |
| 512 | 3297 | 12229 | 5729 | 512 |

**Deep sub-catalog (d>=4, bf=30, k=128):**

| XOR level | Winners |
|-----------|---------|
| XOR6 | 2579 |
| XOR8 | 2228 |
| XOR10 | 203 |
| XOR12 | 27 |
| XOR14 | 0 |

**Deep XOR14 k-sweep (all zero):**

| k | XOR14 |
|------|-------|
| 256 | 0 |
| 512 | 0 |
| 1024 | 0 |
| 2048 | 0 |
| 4096 | 0 |

**Comparison: D100 (3x3 non-semisimple) vs D101 (4x4 simple) deep bf=30 k=128:**

| XOR level | D100 (W_{4,2}) | D101 (W_{5,3}) |
|-----------|---------------|---------------|
| XOR6 | 2017 | 2579 |
| XOR8 | 1169 | 2228 |
| XOR10 | 654 | 203 |
| XOR12 | 305 | 27 |
| XOR14 | 70 | 0 |

D101 wins at XOR6-XOR8, D100 wins at XOR10-XOR14. The simple module has broader but shallower capacity.

**Phase 4 -- Casimir C4 by depth:**

| Depth | mean_C4 | Count |
|-------|---------|-------|
| 0 | 0 | 1 |
| 1 | 0 | 8 |
| 2 | 4 | 44 |
| 3 | 9 | 206 |
| 4 | 20 | 884 |
| 5 | 41 | 3600 |
| 6 | 83 | 14198 |
| 7 | 118 | 13827 |

Mean |C4| across full catalog: 91. Growth approximately 2x per round (matching D100 pattern).

**Phase 4 -- Casimir-by-XOR-level (deep bf=30, k=128):**

| XOR level | Winners | Win C4 | Ctrl C4 | Ratio |
|-----------|---------|--------|---------|-------|
| XOR6 | 2579 | 55 | 47 | 1.17x |
| XOR8 | 2228 | 71 | 47 | 1.51x |
| XOR10 | 203 | 57 | 64 | 0.89x |
| XOR12 | 27 | 43 | 79 | 0.54x |

INVERSION at XOR10-XOR12: winners have LOWER Casimir. Contrast with D100 where all ratios > 1 (1.36x-1.86x).

**XOR10 winners vs control (deep bf=30, k=128):**

| Metric | Winners (203) | Control (1000) |
|--------|-------------|----------------|
| mean_C4 | 57 | 67 |

**Phase 5 -- Super-hubs (3 hubs, frequency > 67 = 1/3 of 203 winners):**

| Hub | Entry | Freq | Freq% | Depth | Writhe | Trace | |C4| | Frob L1 |
|-----|-------|------|-------|-------|--------|-------|------|---------|
| 0 | 0 | 87 | 42.9% | 4 | 4 | (-4,0,0,0) | 0 | 8 |
| 1 | 26 | 137 | 67.5% | 5 | 5 | (0,4,0,0) | 0 | 9 |
| 2 | 27 | 142 | 70.0% | 5 | 3 | (0,0,0,-5) | 11 | 12 |

Hub traces by Cyc8 component: hub 0 = real (1-component), hub 1 = zeta_8 (b-component), hub 2 = zeta_8^3 (d-component). Three of four Cyc8 basis elements represented. Hubs 0 and 1 have |C4|=0 (nearly scalar).

**Phase 5 -- Hub commutators (all traceless):**

| Pair | Trace | |C4| | Frob L1 |
|------|-------|------|---------|
| [h0,h1] | (0,0,0,0) | 0 | 0 |
| [h0,h2] | (0,0,0,0) | 0 | 0 |
| [h1,h2] | (0,0,0,0) | 0 | 0 |

ALL commutators are the zero matrix -- all three hubs MUTUALLY COMMUTE. This is much stronger than D100's tracelessness. The three hubs live in a commutative subalgebra of Mat_4(Z[zeta_8]).

**Phase 5 -- Co-occurrence topology:**
- 247 edges total, max co-occurrence 102
- 62 hub-involved edges, 185 satellite-only edges
- NOT a star topology (satellites connect to each other extensively)
- 30/30 entries participate in at least one winner

**Phase 5 -- XOR12 hub frequency (27 winners):**

| Entry | Freq12 | Depth | C4 |
|-------|--------|-------|----|
| 0 | 26 | 4 | 0 |
| 26 | 26 | 5 | 0 |
| 27 | 26 | 5 | 11 |

The same 3 hubs dominate XOR12 even more strongly (26/27 = 96.3% each). All three hubs appear in almost every XOR12 winner.

### Code Assets

- **`Cyc8` (Z[zeta_8] cyclotomic arithmetic)**: Exact integer type representing a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 with zeta_8^4 = -1. Operations: `cyc8_make`, `cyc8_zero`, `cyc8_one`, `cyc8_add`, `cyc8_sub`, `cyc8_neg`, `cyc8_mul`, `cyc8_eq`, `cyc8_is_zero`, `cyc8_conj`, `cyc8_max_abs`, `cyc8_l1`. Shared with D100, D98, D35.
- **`Mat4` (4x4 matrix over Cyc8)**: Matrix type for W_{5,3} representation. 16 Cyc8 entries = 64 long integers. Operations: `mat4_zero`, `mat4_identity`, `mat4_mul`, `mat4_eq`, `mat4_add`, `mat4_neg`, `mat4_scale`, `mat4_max_abs`, `mat4_print`, `mat4_trace`, `mat4_tr_sq`, `mat4_casimir`, `mat4_frob_l1`. All operations exact -- no floating point.
- **Hash table for Mat4 BFS**: FNV-style hash over 64 long components, chained-bucket lookup. MAX_CAT = 32768, HASH_SIZE = 65537. Used for BFS deduplication.
- **`build_tl_generators()`**: Constructs 4x4 integer matrices e_1..e_4 for TL_5 on W_{5,3} basis {h_0, h_1, h_2, h_3}. Pattern: A_3 Dynkin diagram (path graph on 4 nodes). Each e_i has rank 1: maps specific basis elements to a single target.
- **`build_braid_generators()`**: Constructs 8 braid generators sigma_1..4 and inverses over Z[zeta_8]. Writhe tracked per generator (+1/-1).
- **`build_catalog(max_depth)`**: BFS closure with 8 generators. Depth and writhe tracking per entry. Overflow protection at max_abs > 10^11. Reports per-round growth, max magnitude, and closure/cap/depth-limit status.
- **`mat4_activate(m, k_param)`**: 4x4_hash activation -- sign pattern of all 64 integer components (4x4 matrix x 4 Cyc8 components), 3-valued (pos=2, zero=1, neg=0), base-3 hash mod k_param. Novel activation for 4x4 matrix-valued DKC.
- **`test_xor_mat4(indices, n_weights, k_param)`**: 1wpi XOR test for Mat4 catalog. N weights -> 2N inputs (pos/neg pairs). Signed sum in Mat4 space, activation via mat4_activate. Supports up to 8 weights (16 inputs). Touch-list cleanup for efficient cell reuse.
- **`count_xor_bf(n_weights, k_param, bf_limit)`**: Brute-force combinatorial XOR search over C(bf,n_weights) tuples. Supports n_weights = 3-7 (XOR6 through XOR14).
- **`mat4_casimir(m)`**: C4(M) = 4*tr(M^2) - (tr M)^2. Integer-exact Casimir computation for 4x4 matrices. Uses `mat4_tr_sq` (trace of matrix squared via double sum) and `mat4_trace`. Note the coefficient 4 = dim(W_{5,3}), matching the D100 pattern where C3 used coefficient 3 = dim(W_{4,2}).

### Connection to Earlier Demos

| Demo | Finding | Relevance to D101 |
|------|---------|-------------------|
| D98 | 3-strand BFS infinite, trace readout catastrophically lossy | D101 avoids trace readout entirely -- uses sign-hash activation on full matrix |
| D100 | 4-strand W_{4,2}: ~3.1x growth, 6 hubs, Casimir-XOR correlation, constant radical content, all commutators traceless | D101 tests same phenomena at 5 strands on simple module (no radical) -- isolates sl_d structure from non-semisimple effects |
| D100 | Hub pairing by Cyc8 component (zeta_8, zeta_8^2, zeta_8^3) | D101 tests whether hub pairing extends to 12 hubs in 6 pairs for sl_4 |
| D93 | 1wpi framework, circuit complexity hierarchy | D101 reuses activation/XOR framework adapted for 4x4 matrices |
| D80 | Finite/infinite capacity boundary at quantum dimension | W_{5,3} is infinite (simple, n odd), confirming infinite group from simple TL module |

### Literature Touched

- **Temperley-Lieb algebra TL_5 at delta=0**: TL_5 with n=5 (odd) has different semisimplicity behavior than even n. W_{5,3} (standard module with 3 through-lines, dim=4) is SIMPLE -- no radical, no extensions. This contrasts sharply with D100's W_{4,2} (n=4 even, non-semisimple, radical dim=9). The simplicity means D101 tests DKC capacity in the "clean" case without non-semisimple complications.
- **Representation theory of braid groups on TL modules**: sigma_i = A*I + A^{-1}*e_i gives B_5 acting on the 4-dimensional W_{5,3}. The 4 TL generators form an A_3 Dynkin diagram (path graph on 4 nodes), and the sl_d functor thesis predicts the braid image inherits sl_4 structure: growth rate ~4x (Weyl group size ratio), hub count = 2*C(4,2) = 12 (positive roots of A_3).
- **Root systems and Lie algebra structure**: The positive roots of sl_4 (type A_3) are: e_1-e_2, e_2-e_3, e_3-e_4, e_1-e_3, e_2-e_4, e_1-e_4 -- six roots. The sl_d functor thesis predicts 12 super-hubs (6 pairs, one pair per positive root). D100 confirmed 6 hubs = 3 pairs for sl_3 (type A_2, 3 positive roots). D101 tests the A_3 case.
- **Habiro (2002) / Z[zeta_8] bracket**: The algebraic substrate. D101 continues the matrix-valued extension: each catalog entry is a 4x4 matrix over Z[zeta_8]. The DKC signed-sum framework operates in the 4x4 matrix ring, which is associative but non-commutative -- the non-commutativity is what makes hub commutators a meaningful diagnostic.
- **Casimir invariants**: C4(M) = 4*tr(M^2) - (tr M)^2 generalizes D100's C3(M) = 3*tr(M^2) - (tr M)^2. The coefficient matches the module dimension (3 for W_{4,2}, 4 for W_{5,3}), suggesting a uniform formula C_d(M) = d*tr(M^2) - (tr M)^2 for d-dimensional modules. Casimir measures "non-scalar-ness" of the matrix -- distance from scalar multiples of identity in the space of d x d matrices.

### Open Questions

- **Why only 3 hubs, not 12?**: The sl_4 prediction (12 super-hubs = 6 pairs) was falsified. Three possible explanations: (1) simple modules have fundamentally different hub structure than non-semisimple ones; (2) the bf=30 deep sub-catalog at d>=4 misses hubs that appear at shallower depths or in different sampling windows; (3) the hub-count prediction from sl_d was wrong and the growth-rate prediction was the real signal. Testing: repeat with bf=30 from ALL depths (not just d>=4), and try different hub thresholds.
- **Commuting hubs and commutative subalgebra**: All three hub commutators are the zero matrix, not just traceless. The hubs live in a commutative subalgebra. Is this related to hub 0's near-scalar structure (diagonal -I with one off-diagonal entry)? Does the commutative subalgebra have a name in the representation theory of B_5 on W_{5,3}?
- **Casimir inversion mechanism**: Why does the Casimir-XOR correlation invert at XOR10-12? At XOR6-8, high-Casimir entries win (like D100). At XOR10-12, low-Casimir entries win. Hypothesis: harder XOR computations in simple modules require near-scalar entries (low C4) for stability, while in non-semisimple modules the radical extension provides that stability automatically. Test: compute Casimir-by-XOR-level in D100 at XOR6 and XOR8 to see if the same inversion trend starts there.
- **XOR14 vanishing -- simple vs non-semisimple capacity ceiling**: D100 has XOR14=70 on 3x3 non-semisimple, D101 has XOR14=0 on 4x4 simple. Is this a dimension effect (64 vs 36 activation components -- too fine-grained?), a module effect (simple vs non-semisimple), or a sampling effect (different entries at d>=4)? Test: try D101 with mat3-style activation (reduce to 36 or fewer sign components) and see if XOR14 appears.
- **6-strand W_{6,4} as decisive test**: W_{6,4} (n=6, even, non-semisimple) should have ~5x growth (sl_5 thesis). If it also has 6+ super-hubs and XOR14 > 0, this confirms that non-semisimplicity is the key variable for hub count and deep XOR capacity. If it matches D101's pattern (few hubs, XOR14=0), the explanation is dimensional/structural rather than semisimplicity.
- **Growth rate convergence**: The ratios (8.00, 5.50, 4.68, 4.29, 4.07, 3.94) are converging toward 4 but not yet stabilized (3.94 at d=6, truncated at d=7). Larger cap or deeper BFS needed to confirm asymptotic rate is exactly 4.
- **Hub entry 0 structure**: Hub 0 (entry 0 at d=4) is near-scalar: M = -I + (0,0,4,0)*e_{01}. This is sigma_1^4 or a related simple braid word. Its dominance as a hub (42.9% of XOR10 winners, 26/27 of XOR12 winners) suggests a structural role. Is it the image of a central element or a power of a generator? Its near-scalar form (C4=0) is consistent with the Casimir inversion finding.
- **Broad vs deep capacity**: D101 has MORE XOR6-8 solutions than D100 (2579 vs 2017, 2228 vs 1169) but FEWER XOR10-14 solutions (203 vs 654, 27 vs 305, 0 vs 70). The simple module has broader shallow capacity but less deep capacity. Is this a general simple-vs-non-semisimple pattern?
