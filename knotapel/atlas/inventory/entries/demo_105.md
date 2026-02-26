## Demo 105: 8-Strand W_{8,0} and W_{8,2} k-Regime Test — Rank Saturation and Branching Generalization

- **Status**: COMPLETE
- **File**: `demo_105_8strand_k_regime/main.c` (~1046 lines) + phase_b.c (~1067 lines), rank_retroactive.c (~500 lines), rank_saturation.c (~543 lines), rank_saturation_j0.c (~393 lines)
- **Tests**: 46 pass, 0 fail (Phase A / main.c); 50 pass, 0 fail (Phase B / phase_b.c)
- **Depends on**: Demo 85 (link state enumeration infrastructure, j=0 programmatic TL generators), Demo 93 (1wpi XOR framework, circuit complexity hierarchy, "XOR dies at N>=7" claim), Demo 100 (W_{4,2} DKC, non-semisimple matrix braid representation), Demo 101 (W_{5,3} DKC, simple module comparison), Demo 104 (W_{6,2} k-regime discovery, cross-block branching decomposition, sign-rank expansion, Atkinson peak)
- **Feeds into**: Demo 106 (topological entropy — clean negative), Demo 107 (algebraic graph density / Raqiya axis-alignment theorem), tangle tree DKC demo (planned), W_{8,2}/W_{6,4} major index theorem verification (open)

### Headline
8-strand braid groups on W_{8,0} (dim=14, simple) and W_{8,2} (dim=28, simple) revive XOR8 at high k, REFUTING D93's "XOR dies at N>=7" as a k-regime artifact; rank saturation analysis reveals all prior cross-module rank comparisons were confounded by under-sampling; branching interaction reverses at n=8 (within-block > cross-block), discovering the "macrame principle" regime transition.

### Key Results

- **D93's "XOR dies at N>=7" REFUTED**: Was a k-regime artifact. At k=4096, W_{8,0} produces XOR8=22266 (n=8 beats n=6's 21699 at matched k). The ratio k/2^N is the real control parameter: at k=128 for N=8, k/2^N=0.5 (deep pigeonhole), but at k=4096, k/2^N=16 (collision avoidance regime), and XOR is fully alive.
- **Phase A (W_{8,0}, dim=14)**: 14 link states (C_4=14), 7 TL generators, radical dim=0 (simple). BFS hits 32768 cap at round 5 (growth: 1->15->137->987->6189->32768). 14 generators fill catalog faster than n=6's 10. Raw rank=274/784 (34.9%) at 1024 entries, sign-rank=425 (54.2%), sign expansion 1.55x.
- **Phase A k-sweep (bf=30)**: XOR6 peaks at k=4096 (3721), XOR8 peaks at k=4096 (22266). At k=128: XOR6=2898, XOR8=5824.
- **Phase A component sweep (k=128)**: Tests partial sign-hash activation from 50 to 784 components. Identifies Atkinson peak pattern.
- **Phase A sign-rank expansion**: Sign quantization EXPANDS information — ternary sign vectors have higher rank than raw Cyc8 coefficient vectors.
- **Phase B (W_{8,2}, dim=28)**: 28 link states (14 W_{7,1} block + 14 W_{7,3} block = symmetric branching). New j=2 link state enumerator with 5-case TL generator action. All TL/braid relations verified. Radical dim=0 (also simple at delta=0). BFS hits 16384 cap at round 5.
- **Phase B branching decomposition (k=128, bf=30)**: Full: XOR6=2898, XOR8=5664. Cross-block: XOR6=532, XOR8=733. Within-block: XOR6=2940, XOR8=4743. **VERDICT: within-block > cross-block** — OPPOSITE of D104's W_{6,2}.
- **Branching interaction reversal ("macrame principle")**: At n=6 (D104, asymmetric blocks dim 5+4), cross-block dominates. At n=8 (D105, symmetric blocks dim 14+14), within-block dominates. Regime transition based on block complexity: simple blocks need connections (edges), complex blocks are self-sufficient (nodes). Crossover between dim 5 and dim 14.
- **Phase B k-sweep (full activation, bf=30)**: k=128->XOR6=2898/XOR8=5664, k=4096->XOR6=3703/XOR8=22133.
- **Rank saturation CONFOUND discovered**: W_{8,2} rank at 512 entries was 217/3136 (6.9%) — severely under-sampled. Saturation test shows rank still growing at 16384 entries: raw=1096 (34.9%), sign=2003 (63.9%), expansion=1.83x and increasing. The 34.9% match between W_{8,0} and W_{8,2} at initial checkpoints was coincidental.
- **W_{8,0} rank saturation**: Also under-reported. At 32768 entries: raw=710/784 (90.6%), sign=767/784 (97.8%). Sign-rank approaches near-complete utilization of matrix space.
- **Retroactive rank comparison across all modules**: D99b Delta_1 (dim 2): 16/16 raw (100%), 16/16 sign (100%). D100 W_{4,2} (dim 3): 28/36 raw (77.8%), 32/36 sign (88.9%). D101 W_{5,3} (dim 4): 62/64 raw (96.9%), 64/64 sign (100%). D102 W_{6,0} (dim 5): 100/100 raw (100%), 100/100 sign (100%). D104 W_{6,2} (dim 9): 244/324 raw (75.3%), 292/324 sign (90.1%). D105 W_{8,0} (dim 14): 710/784 raw (90.6%), 767/784 sign (97.8%).

### Theorems/Conjectures

- **k-Regime Theory (CONFIRMED at n=8)**: The ratio k/2^N controls XOR capacity: pigeonhole regime (k/2^N << 1) kills XOR, collision avoidance regime (k/2^N >> 1) revives it. Confirmed across n=6 and n=8.
- **Macrame Principle (CONJECTURED)**: Branching interaction (cross-block) dominates when individual sub-module blocks are small/simple (dim 4-5). Within-block dominates when blocks are complex (dim 14). Crossover regime between dim 5 and dim 14. Named by analogy: simple knots need connections for computational fabric; complex knots are self-sufficient.
- **Sign-Rank Expansion Scaling (CONJECTURED)**: Sign expansion ratio increases with module dimension and decreases with oversampling. W_{8,2} at 1.83x (growing) vs W_{8,0} at 1.08x (near saturation). Larger non-semisimple modules show more dramatic expansion.
- **Rank Saturation Methodology (ESTABLISHED)**: Single-checkpoint rank measurements are unreliable for infinite braid groups. Must report saturation curves at multiple checkpoints. All prior cross-module rank comparisons (D99b through D104) are confounded by varying oversampling ratios.

### Data

**W_{8,0} BFS catalog (cap 32768, 14 generators):**

| Round | Cumulative | New | max_abs |
|-------|-----------|-----|---------|
| 0 | 1 | 1 | 1 |
| 1 | 15 | 14 | 1 |
| 2 | 137 | 122 | 2 |
| 3 | 987 | 850 | 3 |
| 4 | 6189 | 5202 | 5 |
| 5 | 32768 | 26579 | 8 |

**W_{8,0} k-sweep (bf=30):**

| k | XOR6 | XOR8 | k/2^8 |
|------|------|-------|-------|
| 128 | 2898 | 5824 | 0.5 |
| 256 | 3345 | 10413 | 1.0 |
| 512 | 3529 | 15982 | 2.0 |
| 1024 | 3663 | 19403 | 4.0 |
| 2048 | 3705 | 21332 | 8.0 |
| 4096 | 3721 | 22266 | 16.0 |

**W_{8,0} rank saturation (784 cols, 32768 catalog entries):**

| Entries | Raw rank | Raw% | Sign rank | Sign% |
|---------|----------|------|-----------|-------|
| 128 | 81 | 10.3% | 91 | 11.6% |
| 256 | 139 | 17.7% | 186 | 23.7% |
| 512 | 211 | 26.9% | 310 | 39.5% |
| 1024 | 274 | 34.9% | 425 | 54.2% |
| 2048 | 416 | 53.1% | 651 | 83.0% |
| 4096 | 498 | 63.5% | 686 | 87.5% |
| 8192 | 614 | 78.3% | 729 | 93.0% |
| 16384 | 684 | 87.2% | 757 | 96.6% |
| 32768 | 710 | 90.6% | 767 | 97.8% |

**W_{8,2} rank saturation (3136 cols, 16384 catalog entries):**

| Entries | Raw rank | Raw% | Sign rank | Sign% |
|---------|----------|------|-----------|-------|
| 128 | 90 | 2.9% | 128 | 4.1% |
| 256 | 180 | 5.7% | 256 | 8.2% |
| 512 | 310 | 9.9% | 443 | 14.1% |
| 1024 | 499 | 15.9% | 783 | 25.0% |
| 2048 | 902 | 28.8% | — | — |
| 4096 | 1178 | 37.6% | — | — |
| 8192 | 1702 | 54.3% | — | — |
| 16384 | 2003 | 63.9% | — | — |

Note: sign-rank checkpoint values for W_{8,2} at entries > 1024 are from rank_saturation.c; final sign-rank at 16384 = 2003 (63.9%), expansion 1.83x.

**W_{8,2} branching decomposition (k=128, bf=30):**

| Activation | Components | XOR6 | XOR8 |
|------------|-----------|------|------|
| Full | 3136 | 2898 | 5664 |
| Cross-block | 1568 | 532 | 733 |
| Within-block | 1568 | 2940 | 4743 |

**Corrected retroactive rank table (all modules, at catalog saturation):**

| Module | dim | cols | catalog | raw | raw% | sign | sign% | exp |
|--------|-----|------|---------|-----|------|------|-------|-----|
| Delta_1 | 2 | 16 | 32768 | 16 | 100% | 16 | 100% | 1.00x |
| W_{4,2} | 3 | 36 | 32768 | 28 | 77.8% | 32 | 88.9% | 1.14x |
| W_{5,3} | 4 | 64 | 32768 | 62 | 96.9% | 64 | 100% | 1.03x |
| W_{6,0} | 5 | 100 | 32768 | 100 | 100% | 100 | 100% | 1.00x |
| W_{6,2} | 9 | 324 | — | 244 | 75.3% | 292 | 90.1% | 1.20x |
| W_{8,0} | 14 | 784 | 32768 | 710 | 90.6% | 767 | 97.8% | 1.08x |
| W_{8,2} | 28 | 3136 | 16384 | 1096 | 34.9% | 2003 | 63.9% | 1.83x |

### Code Assets

- **j=2 link state enumerator (`enumerate_ls_j2`)**: Enumerates non-crossing perfect matchings with 2 through-strands on n=8 boundary points. Produces 28 states split into two blocks (W_{7,1} with through[1]=7, and W_{7,3} with through[1]<7). Uses segment-based recursive enumeration with Cartesian products of left/mid/right matchings. First implementation of j>0 link states in knotapel.
- **5-case TL generator action for j=2 (`apply_ei_j2`)**: Handles all cases of e_i acting on a link state with 2 through-strands: (1) both arc-paired to each other -> loop/killed; (2) both arc-paired to others -> redirect; (3) i through, i+1 arc-paired -> swap; (4) i arc-paired, i+1 through -> swap; (5) both through -> loop/killed. Through-strand positions are updated and re-sorted.
- **Cross-block / within-block activation functions**: `matN_activate_cross` hashes only off-diagonal blocks (rows < BLOCK_SIZE, cols >= BLOCK_SIZE and vice versa). `matN_activate_within` hashes only diagonal blocks. Used for branching decomposition analysis.
- **Partial sign-hash activation (`matN_activate_partial`)**: Activates using only the first `comp_limit` sign components. Used for component sweep / Atkinson peak analysis.
- **Rank computation with saturation checkpoints (`compute_rank_with_checkpoints`)**: Streaming Gaussian elimination over Z with GCD normalization, reporting rank at specified entry-count checkpoints. Essential for honest rank measurement on infinite groups.
- **Retroactive rank framework (`rank_retroactive.c`)**: Generic tridiagonal TL generator builder + BFS + rank computation, parameterized by (dim, n_tl). Runs D99b/D100/D101 modules with identical infrastructure for fair comparison.

### Literature Touched

- **Temperley-Lieb algebra TL_8 at delta=0**: W_{8,0} (dim=14 = C_4, simple) and W_{8,2} (dim=28, also simple at delta=0). Both modules produce infinite braid groups. The branching rule W_{8,2} |_{TL_7} = W_{7,1}(14) + W_{7,3}(14) is symmetric, unlike W_{6,2}'s asymmetric 5+4 decomposition.
- **k-regime theory / pigeonhole-to-collision-avoidance transition**: The ratio k/2^N determines XOR viability. This is an information-theoretic observation about sign-hash activation: when k << 2^N, hash collisions between even/odd parity inputs are unavoidable (pigeonhole); when k >> 2^N, the hash has enough bins to separate them.
- **Compressed sensing / 1-bit CS analogy**: Sign-hash activation = 1-bit compressed sensing measurement. Cross-block activation works when sub-module pieces have low intra-block coherence. Within-block dominance at n=8 suggests coherence structure changes with block dimension.
- **Habiro (2002) / Z[zeta_8] bracket**: The algebraic substrate. A = zeta_8^5 = -zeta_8, A^{-1} = zeta_8^3. Braid generators sigma_i = A*I + A^{-1}*e_i produce exact Z[zeta_8]-valued matrices.

### Open Questions

- **W_{8,2} rank saturation**: At 16384 entries / 3136 cols = 5.2x oversampling, W_{8,2} is still at 34.9% raw rank. Would need ~130K entries for comparable oversampling to W_{8,0}. What is the true saturated rank fraction?
- **Macrame principle crossover**: The branching interaction regime transition occurs between block dim 5 (cross-block wins, D104) and dim 14 (within-block wins, D105). Where is the exact crossover? W_{6,4} or W_{10,2} could bracket it.
- **Sign expansion vs module structure**: W_{8,2} shows dramatic sign expansion (1.83x at 16384 entries, still growing). Is this a function of module dimension, non-semisimplicity, or the specific block structure of the branching decomposition?
- **Phase B XOR at matched conditions**: Head-to-head W_{8,0} vs W_{8,2} XOR comparison at matched k and oversampling has not been performed. Listed as open in explorer log.
- **All prior cross-module rank comparisons confounded**: The retroactive rank table only corrects modules that were recomputed. Other demos' rank claims should be treated as lower bounds until saturation-verified.
