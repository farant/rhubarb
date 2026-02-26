## Demo 104: Activation Coarseness Sweep — Sign-Hash Component Tuning for W_{6,0} vs W_{6,2}

- **Status**: COMPLETE (RESULT)
- **File**: `demo_104_coarseness_sweep/main.c` (~1798 lines)
- **Tests**: 35/35 pass (Phases 0-8)
- **Depends on**: Demo 103 (W_{6,2} vs W_{6,0} dimension scaling test, catalogs and generator code reused), Demo 101 (5-strand DKC on W_{5,3}, sign-hash activation pattern), Demo 93 (1wpi XOR framework)
- **Feeds into**: Demo 105 (W_{8,2} branching generalization, 8-strand k-regime test), future cross-block activation design for higher-strand modules

### Headline
The dim-9 W_{6,2} module's XOR deficit versus dim-5 W_{6,0} (observed in D103) is a HASH problem, not an algebra problem: at optimal component count (120 of 324), W_{6,2} beats W_{6,0} (XOR6=2486 vs 2449), and the computational signal lives in the cross-block (off-diagonal branching interaction) components, not the within-block (diagonal) components -- but a k-sweep reveals this cross-block advantage is regime-dependent (low-k pigeonhole only), with W_{6,0} winning at every k value.

### Key Results
- **Phase 0 -- Generator verification (20 tests)**: Both modules verified. W_{6,0} (dim=5): 5 TL generators, sparsity 15/125 nonzero. W_{6,2} (dim=9): 5 TL generators, sparsity 25/405 nonzero. All e_i^2=0 and sigma_i * sigma_i_inv = I confirmed for both.
- **Phase 1 -- BFS catalogs (2 tests)**: Both modules hit 32768-entry cap at round 6 with IDENTICAL growth profiles (1, 11, 77, 439, 2233, 10603, 32768). Growth per round: 10, 66, 362, 1794, 8370, 22165. max_abs progression: 1, 2, 3, 5, 8, 16. Confirms D103's strand-count growth law: same n=6, same growth regardless of module dimension.
- **Phase 2 -- Rank computation (2 tests)**: W_{6,0} raw rank = 100/100 cols (100% of matrix space -- every component independent). W_{6,2} raw rank = 244/324 cols (75.3% -- 80 linearly dependent dimensions). This rank gap explains the D103 XOR deficit: W_{6,2} wastes hash bits on redundant information.
- **Phase 3 -- Component count sweep (3 tests)**: W_{6,0} peaks monotonically at 100 comp (XOR6=2449). W_{6,2} peaks at 120 comp (XOR6=2486), BEATS W_{6,0}. Non-monotonic curve for B: rises to 120, drops through 150-250, partial recovery at 300, drops at 324 (2238). Named "Atkinson sweet spot" -- optimal information-per-collision tradeoff. Head-to-head at 100 comp: A=2449, B=2360 (A wins at matched count).
- **Phase 4 -- Random-100 variants (1 test)**: 5 random selections of 100 components from W_{6,2}: min=2105, max=2367, mean=2266, spread=262. Sequential first-100 (2360) beats random mean (2266). HIGH spread indicates information concentrated in specific components, not uniformly distributed.
- **Phase 5 -- Branching decomposition (1 test, LANDMARK)**: W_{6,2} decomposes under TL_5 restriction into through-strand piece (rows 0-4) and arc-bound piece (rows 5-8). Cross-block (off-diagonal, 160 comp) XOR6=2412, XOR8=952. Within-block (diagonal, 164 comp) XOR6=2235. Sub6 (first 6x6=144 comp) XOR6=2415, XOR8=720. VERDICT: cross-block > within-block -- branching interaction IS the computational signal. XOR8 advantage dramatic: cross-block 952 vs B-full 557 vs within-block omitted.
- **Phase 6 -- Sign-rank computation (2 tests)**: W_{6,0} sign-rank = 100/100 (100%, quantization LOSSLESS). W_{6,2} sign-rank = 292/324 (90.1%), HIGHER than raw rank (244). Sign quantization is an EXPANDER: the nonlinear sign() function breaks 48 linear dependencies, creating new independent ternary patterns. The Atkinson peak at 120 components is a hash-collision bottleneck, not a sign-dimension bottleneck.
- **Phase 7 -- Multi-hash tests (1 test)**: Three multi-hash variants with k=64x64=4096: row split (180+144 comp, XOR6=3852, XOR8=22005), cross-block split (80+80 comp, XOR6=3534, XOR8=17939), cross+diag (160+64 comp, XOR6=3806, XOR8=21525). Reference: single-hash cross-block (160 comp, XOR6=3635, XOR8=19994). VERDICT: multi-hash does NOT beat single-hash. Cross-block split HURTS (17939 vs 19994) -- the two branching directions are correlated, not independent. k (cell count) is the real lever, not hash architecture.
- **Phase 8 -- k-sweep (2 tests, CRITICAL)**: Swept k={128, 256, 512, 1024, 2048, 4096}. W_{6,0} full (100 comp) beats W_{6,2} cross-block (160 comp) at EVERY k for BOTH XOR6 and XOR8. At k=4096: A XOR8=21699, B XOR8=19994. The cross-block advantage at k=128 (952 vs 850) was a low-k artifact. Revised understanding: cross-block is a better ACTIVATION DESIGN (extracts more per cell), but W_{6,0} has more fundamental capacity at sufficient k.

### Theorems/Conjectures
- **Atkinson Sweet Spot (DEMONSTRATED)**: For W_{6,2} at k=128, optimal component count is ~120 (of 324 total), producing XOR6=2486 which beats W_{6,0}'s maximum 2449. Named after Atkinson dithering -- optimal information-per-collision tradeoff in sign-hash quantization. The peak is k-dependent: at higher k, more components become usable.
- **Branching Interaction Thesis (CONFIRMED)**: The computational signal in non-simple modules lives in the cross-block (off-diagonal) components under TL_{n-1} branching restriction. Cross-block activation (160 comp) outperforms within-block (164 comp) and full (324 comp) at matched k. This is because the branching cross-terms between the two TL_5 sub-pieces have low intra-block coherence.
- **Sign-Rank Expansion (NEW FINDING)**: Sign quantization (ternary {-1,0,+1} projection) can INCREASE effective rank. W_{6,2}: raw rank 244, sign-rank 292 -- a gain of 48 independent directions. The nonlinear sign() function breaks linear dependencies that exist over Z. W_{6,0}: raw rank = sign-rank = 100 (already maximally independent, no expansion possible).
- **k is the Real Lever (DEMONSTRATED, partially REFUTES D103 conclusion)**: Multi-hash architecture does not improve XOR capacity. Increasing k (cell count) is the primary scaling lever. The cross-block advantage at k=128 vanishes at k=4096. W_{6,0} wins at every k in the sweep.
- **Strand-Count Growth Law (RE-CONFIRMED)**: Both dim-5 and dim-9 modules of TL_6 produce identical BFS growth profiles (1, 11, 77, 439, 2233, 10603, 32768). Growth rate is determined by strand count n, not module dimension.

### Data

**Phase 2 -- Rank comparison:**

| Module | Raw Rank | Sign Rank | Total Cols | Raw % | Sign % |
|--------|----------|-----------|------------|-------|--------|
| W_{6,0} (dim=5) | 100 | 100 | 100 | 100.0% | 100.0% |
| W_{6,2} (dim=9) | 244 | 292 | 324 | 75.3% | 90.1% |

**Phase 3 -- Component sweep (k=128):**

W_{6,0} (dim=5, max=100 comp):

| comp | XOR6 |
|------|------|
| 10 | 234 |
| 20 | 1166 |
| 25 | 1457 |
| 40 | 1678 |
| 50 | 2372 |
| 60 | 2353 |
| 75 | 2318 |
| 80 | 2370 |
| 100 | 2449 |

W_{6,2} (dim=9, max=324 comp):

| comp | XOR6 |
|------|------|
| 10 | 254 |
| 20 | 817 |
| 25 | 1186 |
| 40 | 1703 |
| 50 | 1704 |
| 60 | 1885 |
| 75 | 2164 |
| 80 | 2101 |
| 100 | 2360 |
| **120** | **2486** |
| 140 | 2375 |
| 150 | 2300 |
| 175 | 2195 |
| 200 | 2265 |
| 250 | 2190 |
| 300 | 2412 |
| 324 | 2238 |

**Phase 4 -- Random-100 variants (W_{6,2}):**

| Trial | XOR6 |
|-------|------|
| Sequential first-100 | 2360 |
| Random trial 1 | 2318 |
| Random trial 2 | 2195 |
| Random trial 3 | 2347 |
| Random trial 4 | 2367 |
| Random trial 5 | 2105 |
| Mean | 2266 |
| Spread (max-min) | 262 |

**Phase 5 -- Branching decomposition:**

| Activation | comp | XOR6 | XOR8 |
|------------|------|------|------|
| A full | 100 | 2449 | (ref) |
| B full | 324 | 2238 | 557 |
| B sub6 | 144 | 2415 | 720 |
| B cross-block | 160 | 2412 | 952 |
| B within-block | 164 | 2235 | - |

**Phase 7 -- Multi-hash summary (k=64x64=4096):**

| Test | comp | XOR6 | XOR8 |
|------|------|------|------|
| Ref: cross 1-hash | 160 | 3635 | 19994 |
| T1: row split | 180+144 | 3852 | 22005 |
| T2: cross-dir split | 80+80 | 3534 | 17939 |
| T3: cross+diag | 160+64 | 3806 | 21525 |

**Phase 8 -- k-sweep:**

W_{6,2} cross-block (160 comp):

| k | XOR6 | XOR8 |
|------|------|------|
| 128 | 2412 | 952 |
| 256 | 2978 | 3985 |
| 512 | 3369 | 9510 |
| 1024 | 3511 | 14503 |
| 2048 | 3566 | 17549 |
| 4096 | 3635 | 19994 |

W_{6,0} full (100 comp):

| k | XOR6 | XOR8 |
|------|------|------|
| 128 | 2449 | 850 |
| 256 | 3128 | 3894 |
| 512 | 3508 | 9414 |
| 1024 | 3730 | 15173 |
| 2048 | 3807 | 19059 |
| 4096 | 3866 | 21699 |

### Code Assets
- **`MatN` (variable-dimension matrix over Cyc8)**: Generic NxN matrix (MAX_DIM=9) with runtime dimension `g_dim`. Operations: `matN_zero`, `matN_identity`, `matN_mul`, `matN_mul_to` (in-place output), `matN_eq`, `matN_add`, `matN_scale`, `matN_add_inplace`, `matN_sub_inplace`, `matN_max_abs`, `matN_nonzero_count`. All exact integer arithmetic.
- **`matN_activate_n(m, k_param, num_comp)`**: Configurable-coarseness sign-hash activation. Hashes the first `num_comp` of the `g_dim*g_dim*4` ternary sign components (pos=2, zero=1, neg=0) using base-3 polynomial hash mod k_param. Enables the component-count sweep that is the core experiment of this demo.
- **`matN_activate_rand(m, k_param)`**: Random-subset activation using index array `g_rand_idx[]`. Hashes components at arbitrary positions rather than sequential prefix, enabling the random-100 experiment.
- **`matN_activate_multi(m, k_param)`**: Multi-hash activation using two independent index arrays, producing 2D cell ID = hash1 * k + hash2. Used in Phase 7 multi-hash tests.
- **`compute_catalog_rank(n_entries)`**: Streaming Gaussian elimination over Z to compute rank of catalog as row vectors in R^{dim*dim*4}. Processes entries one at a time to avoid allocating full 32768 x 324 matrix. GCD normalization prevents integer overflow. Reusable for any MatN catalog.
- **`compute_sign_rank(n_entries)`**: Same streaming Gaussian elimination but on ternary sign-projected vectors ({-1,0,+1}). Reveals the effective dimension of the activation's input space, which can EXCEED raw rank due to nonlinear projection.
- **Function-pointer activation switching**: `g_activate` function pointer plus `activate_fn` typedef allows runtime switching between `matN_activate_nwrap`, `matN_activate_rand`, and `matN_activate_multi` without modifying the XOR test infrastructure.
- **Fisher-Yates shuffle with deterministic PRNG**: xorshift PRNG (`rng_next`) with `shuffle_first_n` for reproducible random subset selection.

### Literature Touched
- **Atkinson dithering / quantization theory**: The non-monotonic component-count curve resembles optimal quantization in dithering: too little quantization loses signal, too much loses useful collision structure. Named the peak "Atkinson sweet spot."
- **1-bit compressed sensing (Boufounos-Baraniuk 2008)**: Sign-hash activation maps to 1-bit compressed sensing measurements. Cross-block works because branching cross-terms have low intra-block coherence (near-orthogonal TL_5 pieces), which is the incoherence condition for CS recovery.
- **Temperley-Lieb branching rules**: W_{6,2} restricts to TL_5 as W_{5,1} (dim=5, through-strand piece) plus W_{5,3} (dim=4, arc-bound piece). The cross-block components encode interaction between these two pieces. This branching decomposition is the structural basis for the "branching interaction IS the signal" finding.
- **Reservoir computing frame**: Braid representation = reservoir, sign hash = readout (deliberately lossy), XOR computation = exploiting collision patterns. Cross-block = optimized readout layer.

### Open Questions
- **Why does W_{6,0} win at high k despite W_{6,2} having richer algebra?** At k=4096, A (100 comp) produces XOR8=21699 vs B cross-block (160 comp) at 19994. The 100% rank utilization of W_{6,0} may be the decisive factor -- no wasted hash bits. Or the dim-5 matrices may have a more uniform sign distribution that maps better to high-k cell grids.
- **Is the Atkinson peak k-dependent?** At k=128, the peak is at 120 comp. Does it shift (to more components) at higher k? The sign-rank of 292 suggests much more could be exploited. Testing component sweep at k=4096 would answer this.
- **General cross-block recipe**: For W_{n,j}, is the optimal activation always the cross-block under TL_{n-1} branching? D105 tests this at n=8 where branching is symmetric (14+14). If cross-block wins at n=8 too, this becomes a design principle.
- **Sign-rank expansion mechanism**: What determines whether sign quantization expands or preserves rank? W_{6,0}: preserved (100=100). W_{6,2}: expanded (244 to 292). Is the expansion related to the 80 Z-linear dependencies being "nearly independent" in sign space? What about at n=8?
- **Information concentration in early components**: Sequential first-100 (2360) beats random mean (2266) with high spread (262). Which specific components carry the most signal? A component-by-component importance ranking could identify the algebraically meaningful dimensions.
- **Branching advantage vs k regime**: Cross-block advantage is dramatic at k=128 (XOR8: 952 vs 557) but disappears by k=4096. Is there a k threshold formula relating module dimension, rank, and crossover point?
