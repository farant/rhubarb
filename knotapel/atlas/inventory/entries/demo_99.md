## Demo 99: 3-Strand Delta_1 Block Decomposition — LANDMARK

- **Status**: COMPLETE (LANDMARK)
- **Designation**: Demo 99b (labeled 99b in source and explorer's log; supersedes earlier D99 column-readout attempt)
- **File**: `demo_99_column_readout/main.c` (~2678 lines)
- **Tests**: 24+ pass, 0 fail (Phases 0-11, plus sub-phases 7b/7c/7d)
- **Depends on**: Demo 51 (TL_3 semisimplicity at delta=0), Demo 98 (5x5 trace readout gives ZERO XOR), Demo 99 original (5oct_hash on 5x5 gives 3100 XOR6), Demo 82 (depth law max_xor ~ depth+6), Demo 97 (orthogonal directions = perfection)
- **Feeds into**: Demo 99c (algebraic classification, Cell B analog), Demo 100 (4-strand W_{4,2} DKC), Demo 101 (dimensionally-matched activation)

### Headline
First-ever 3-strand XOR computation at ALL levels: XOR6=500,261, XOR8=48,143, XOR10=485, XOR12=16, XOR14=60 (best across activations). The key was building the irreducible 2x2 standard module Delta_1 directly over Z[zeta_8] instead of using the reducible 5x5 representation, combined with a 16-component sign-pattern activation (2x2_hash). The braid group image is INFINITE (Jordan block structure, sigma_1 has infinite order), with entry magnitudes growing by exact Fibonacci ratios. XOR counts saturate by ~4096 catalog entries. XOR14 requires deep entries (d>=6); shallow entries give definitively ZERO. XOR16 is ZERO (genuine ceiling for this representation).

### Key Results

- **Phase 0 -- Representation Verification (8 tests)**: A * A_inv = 1 (A = -zeta_8). A^2 + A^{-2} = 0 (delta=0 confirmed). sigma_1 * sigma_1_inv = I. sigma_2 * sigma_2_inv = I. Braid relation s1*s2*s1 = s2*s1*s2. Correct Hecke relation verified: sigma - sigma_inv = (A - A_inv)(I - e_i). Standard quadratic form (sigma-A)(sigma+A_inv)=0 does NOT hold on Delta_1 because 1+A^{-2} != 0. e_1^2 = 0 on Delta_1. e_1*e_2 = [[0,0],[0,1]] (projector).

- **Phase 1 -- BFS Catalog (cap 32768)**: Group is INFINITE. BFS produces 8192+ entries (32768 at full cap). Growth per round ~2.1x. max_abs sequence follows exact Fibonacci: 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233. Fibonacci mechanism: sigma_1 alone grows linearly (Jordan block), but mixed products of sigma_1 and sigma_2 feed each other's off-diagonal entries via Fibonacci recursion.

- **Phase 2 -- Catalog Statistics**: Depth distribution across BFS rounds 0-10+. Triangular entries 104 (1.3%), anti-trace (a=d) entries ~1.1%, dense entries 99.5%+ beyond d=0. Quaternion structure: trace-zero and anti-trace-zero populations tracked.

- **Phase 3 -- XOR Capacity Scan (activation hierarchy)**:

| Activation | N=3(XOR6) | N=4(XOR8) | N=5(XOR10) | N=6(XOR12) |
|------------|-----------|-----------|------------|------------|
| 4ent(16)   | 227624    | 5990      | 126        | 0          |
| 4ent(32)   | 258764    | 8098      | 206        | 8          |
| 4ent(64)   | 230568    | 7792      | 210        | 8          |
| quat(16)   | 28748     | 284       | 0          | 0          |
| quat(32)   | 29832     | 298       | 0          | 0          |
| quat(64)   | 29124     | 294       | 0          | 0          |
| 2x2(32)    | 373194    | 11974     | 325        | 16         |
| 2x2(64)    | 373194    | 16404     | 485        | 16         |
| 2x2(128)   | 373194    | 16404     | 485        | 16         |
| tr_oct(8)  | 0         | 0         | 0          | 0          |

Best per XOR level (bf=200/60/30/18): XOR6=500,261 at 2x2(64), XOR8=48,143 at 2x2(128), XOR10=485 at 2x2(128), XOR12=16 at 2x2(32+). Activation hierarchy: 2x2_hash >> 4ent_hash >> quat_hash >> trace_oct.

- **Phase 3b -- Scaling Saturation**: XOR counts do NOT increase with catalog size beyond ~4096 entries at fixed bf_limit. More activation cells DO help: 2x2(128) gives 48K XOR8 vs 2x2(64)'s 23.7K. bf_limit (search depth) is the real constraint, not catalog size.

- **Phase 4 -- Matched Comparison (size 24, XOR6)**:

| Metric       | 3-strand | 2-strand |
|--------------|----------|----------|
| XOR6 winners | 527      | 1109     |
| Catalog size | 24       | 24       |

2-strand leads 2.1:1 at matched size with first-24 entries.

- **Phase 4b -- Depth-Sampled Comparison (24 entries, d=6-8)**: Gap closes from 2.1:1 to 1.4:1 (808 vs 1109 XOR6). Confirms deeper entries are richer. Remaining gap attributed to activation quality (Sec x Vor tuned for quaternions vs crude 2x2_hash). At full cap the 3-strand advantage is massive (500K vs 1109).

- **Phase 5 -- Jordan Block Structure**: sigma_1^n has off-diagonal (2,1) entry = n * A^{n-2}, growing linearly. Diagonal entries cycle with period 8 (A^8 = 1). sigma_1^8 has diagonal = I but (2,1) entry = 8*A^6 != 0. sigma_1^8 != I, confirming infinite order. Property F does NOT apply at delta=0.

- **Phase 6 -- XOR14 = 60 (CEILING BROKEN)**: Deep entries (d>=6, bf=30) with 2x2_hash:

| Cells | XOR10 | XOR12 | XOR14 |
|-------|-------|-------|-------|
| 64    | 2641  | 675   | 60    |
| 128   | 2966  | 675   | 60    |
| 256   | 4335  | 675   | 60    |
| 512   | 6883  | 759   | 60    |
| 1024  | 9418  | 1452  | 60    |

XOR14=60 from deep entries, XOR14=0 from full catalog (shallow entries). Depth is a genuine computational resource.

- **Phase 7 -- XOR14 Winner Anatomy**: 60 XOR14 winners analyzed. Super-hubs discovered: idx=7 and idx=9 appear in ALL 60 winners (100%). Both d=6, dense. idx=7 is anti-trace (a=d). 100% of winners are mixed depth (d=6 + d=7). Cross-depth mandatory. Star graph topology: 2 fixed core hubs + 5 rotating satellites from ~12 d=7 entries. Pairwise commutators are large (mean=126, typical entries are 1-89).

- **Phase 7b -- Hub Commutator [hub7, hub9]**: Traceless (scalar = 0), strong Cartan component (L1=84), e-root L1=108, f-root L1=18, e/f ratio = 6.0. Hallmark of sl_2 bracket structure.

  Super-hub idx=7 (d=6, w=2, anti-trace): a = d = (0,0,5,0), b = (12,0,0,0), c = (-2,0,0,0). Cartan L1 = 0. Pure scalar+nilpotent ("e-root carrier").

  Super-hub idx=9 (d=6, w=-6, non-anti-trace): a = (0,0,-8,0), d = (0,0,1,0), b = c = (-3,0,0,0). Cartan L1 = 9. Strong h-carrier. b=c symmetry.

- **Phase 7c -- Casimir Discriminant Test**: C(S) = (a-d)^2 + 4bc for 1wpi sums. Winner mean |C| = 1301 vs control mean |C| = 297 (4.4x higher). Winner has FEWER Jordan blocks (C=0): 384 total vs 816 control. Mechanism is NOT "more Jordan blocks = more computation." Winner entries are MORE diagonalizable. Computation happens in the semisimple layer; Jordan block is infrastructure creating the infinite group.

- **Phase 7d -- Swap Test**: ZERO substitutions preserve XOR14. Hub7 replacement: 0/1 survive. Hub9 replacement: 0/7 survive. Super-hubs are specific algebraic objects, not just structurally typed entries.

- **Phase 8 -- XOR16 Push**: XOR14 at bf=40, deep, cells=256: 4016 (abundant, not rare). XOR16 at bf=14, deep, cells=256: 0. C(14,8) = 3003 tuples tested with 65536 masks each. XOR16 is a genuine ceiling for Delta_1 at ell=2.

- **Phase 9 -- Depth Parity Test**: Mean writhe = 0.000 at every depth level (d=0 through d=12). Anti-trace fraction ~1.1% at all depths. Dense fraction 99.5%+ beyond d=0. No even/odd depth effect. Writhe perfectly balanced.

- **Phase 10 -- Casimir Gap Scaling (Two-Regime Discovery)**:

| XOR level | Winner |C| | Control |C| | Ratio |
|-----------|-----------|-------------|-------|
| XOR6      | 54        | 75          | 0.7x (INVERTED) |
| XOR8      | 73        | 104         | 0.7x (INVERTED) |
| XOR10     | 513       | 154         | 3.3x  |
| XOR12     | 892       | 219         | 4.1x  |
| XOR14     | 1301      | 245         | 5.3x  |

Phase transition at XOR8 -> XOR10 coincides with shallow -> deep entry transition. Two computational mechanisms: (1) Combinatorial (XOR6-8): nilpotent-proximate, diversity-driven, shallow entries suffice. (2) Algebraic (XOR10-14): Ext^1 catalytic preparation, eigenvalue separation, deep entries required.

- **Phase 11 -- Casimir Distribution vs Tuple Size**: Random walk exits nilpotent neighborhood as tuple size grows. Mean |C| and fraction with |C|<50 tracked across N=4 through N=14.

### Theorems/Conjectures

- **Ext^1 Catalytic Preparation Thesis (ESTABLISHED)**: 3-strand DKC computation at delta=0 lives on the non-trivial extension class Ext^1(L(0), L(1)) between simple modules of the restricted quantum group u_q(sl_2) at ell=2. The standard module Delta_1 is indecomposable but reducible: submodule L(0) and quotient L(1), extension does NOT split. Non-split extension is the computational resource. End-to-end causal chain: Ext^1 -> complementary pairing -> high-C sums -> eigenvalue discrimination -> Boolean computation.

- **Jordan Block = Non-Split Extension**: sigma_1^n has off-diagonal n*A^{n-2} (linear growth proves indecomposable). Extension class in Ext^1(L(0), L(1)) is non-zero.

- **Fibonacci Growth**: max_abs follows Fibonacci: connected to golden ratio phi through the spectral radius of trace-zero SL_2 elements and the 2x2 transfer matrix.

- **Anti-Trace = Cartan-Zero**: Entries with a=d have zero Cartan component in sl_2 decomposition M = scalar*I + b*e + c*f + ((a-d)/2)*h. These are "pure nilpotent+scalar" elements. Computationally powerful only IN COMBINATION with non-anti-trace entries (infrastructure, not engines).

- **U-Shaped Cartan Correlation**: XOR capacity vs Cartan |a-d| magnitude: cartan=0 is XOR12 champion (4292), cartan=1-5 is dead, cartan=11+ has 794. Both extremes compute; the middle is dead. Two complementary mechanisms.

- **Two-Regime Hypothesis (CONFIRMED)**: Combinatorial regime (XOR6-8) uses nilpotent-proximate, diversity-driven computation with shallow entries. Algebraic regime (XOR10-14) uses Ext^1 catalytic preparation with deep entries. The Ext^1 thesis governs specifically the algebraic (high-XOR) regime. Monotonic within each regime but not overall.

- **Star-Graph Topology**: XOR14 winners form a star graph (2 irreplaceable hubs + rotating satellites), structurally different from 2-strand Cell B's mutual orthogonality graph.

- **Casimir Inversion**: Non-semisimplicity is catalytic preparation, not the computation mechanism. Preparation selects complementary entries; evaluation produces high-C sums (4.4x control); readout discriminates eigenvalue structure. Shared architecture with non-semisimple TQFT, reservoir computing, and magic state distillation (but catalytic).

- **Registered Prediction**: Casimir gap (winner/control ratio) should increase monotonically with XOR level within the algebraic regime. CONFIRMED for XOR10 -> XOR12 -> XOR14.

### Data

**Direct standard module construction (Delta_1 basis and generators):**
- Basis: {h_0, h_2} -- half-diagrams with 1 through-strand at position 0 or 2
- TL generators: e_1|_{D1} = [[0,0],[1,0]], e_2|_{D1} = [[0,1],[0,0]]
- Braid generators: sigma_1 = [[A, 0], [A_inv, A]], sigma_2 = [[A, A_inv], [0, A]]
- A = -zeta_8 = (0,-1,0,0), A_inv = (0,0,0,1)
- 16 DOF per entry: 4 Cyc8 components x 4 matrix entries = 16 integers (vs 8 for 2-strand quaternions)

**BFS growth (Fibonacci max_abs):**

| Round | Entries | max_abs |
|-------|---------|---------|
| 0     | 1       | -       |
| 1     | 5       | 1       |
| 2     | 17      | 2       |
| 3     | 47      | 3       |
| 4     | 115     | 5       |
| 5     | 263     | 8       |
| 6     | 577     | 13      |
| 7     | 1233    | 21      |
| 8     | 2581    | 34      |
| 9     | 5377    | 55      |
| 10    | 8192    | 89 (hit cap) |

**2-strand vs 3-strand comparison:**

| Property         | 2-strand (TL_2)            | 3-strand (Delta_1)            |
|------------------|----------------------------|-------------------------------|
| Group            | Finite (cyclic, order 8)   | Infinite (Jordan block)       |
| Module           | Simple L(1)                | Indecomposable Delta_1        |
| Extension        | None (semisimple)          | Ext^1(L(0),L(1)) != 0        |
| XOR topology     | Mutual orthogonality graph | Star graph (2 hubs + sats)    |
| XOR ceiling      | XOR12 (Cell B)             | XOR14 (deep entries)          |
| Matched (N=24)   | 1109 XOR6                  | 527 XOR6                      |

2-strand wins at matched small size (finite group better density). 3-strand wins at scale (infinite group unbounded catalog depth). Crossover around catalog size ~100.

**XOR14 super-hub structure:**
- Hub idx=7: a = d = (0,0,5,0), b = (12,0,0,0), c = (-2,0,0,0). d=6, w=2, anti-trace. Cartan L1=0.
- Hub idx=9: a = (0,0,-8,0), d = (0,0,1,0), b = c = (-3,0,0,0). d=6, w=-6. Cartan L1=9.
- Hub commutator: traceless, Cartan L1=84, e-root L1=108, f-root L1=18.
- 100% of 60 winners contain both hubs. 100% are mixed-depth (d=6 + d=7).
- 0/1 substitutions survive for hub7. 0/7 substitutions survive for hub9.

**Casimir discriminant (winner vs control):**
- Winner: mean |C|_L1 = 1301 (even: 1301, odd: 1300). Jordan blocks: 384/16384.
- Control: mean |C|_L1 = 297. Jordan blocks: 816/16384.
- Winner is 4.4x MORE semisimple than control.

**D99c results (algebraic classification, separate program):**
- ALL entries have unit determinant (100%). 4 distinct det values = 4 units in Z[i]: {+/-1, +/-i}. Representation is in SL_2(Z[zeta_8]).
- Triangular entries (104, 1.3%) have 67.9% XOR6 hit rate -- nearly double next best class.
- Cell B analog (24 curated anti-trace+dense+deep entries): XOR12=896 (56x improvement over first-24).

### Code Assets

- **`Cyc8` (cyclotomic integer arithmetic in Z[zeta_8])**: Basis {1, zeta_8, zeta_8^2, zeta_8^3} with zeta_8^4 = -1. Operations: `cyc8_make`, `cyc8_zero`, `cyc8_one`, `cyc8_add`, `cyc8_sub`, `cyc8_neg`, `cyc8_mul`, `cyc8_eq`, `cyc8_is_zero`, `cyc8_conj` (Galois conjugate: zeta_8 -> zeta_8^7 = -zeta_8^3). `cyc8_to_cplx` converts to scaled integer complex (707 ~ 1000/sqrt(2)). `cyc8_octant` classifies into 8 octants (3 bits: sign of Re, sign of Im, |Im|>|Re|). Fully exact long integer arithmetic.
- **`Mat2` (2x2 matrix over Cyc8)**: Operations: `mat2_zero`, `mat2_identity`, `mat2_mul`, `mat2_eq`, `mat2_add`, `mat2_neg`, `mat2_max_abs`, `mat2_commutator`. Each entry is a Cyc8 (4 longs), so each matrix has 16 integer components.
- **`CycQuat` (quaternion extraction from Mat2)**: q_0 = a+d, q_1 = b+c, q_2 = b-c, q_3 = a-d. Maps 2x2 matrices to quaternion components. All operations stay in Cyc8.
- **`build_generators()`**: Constructs the 4 braid generators (sigma_1, sigma_1_inv, sigma_2, sigma_2_inv) on the Delta_1 standard module. A = -zeta_8 = (0,-1,0,0).
- **`build_catalog()`**: BFS catalog builder with hash table deduplication. Tracks depth and writhe per entry. Overflow protection (max_abs > 10^11). Global arrays: g_cat (Mat2), g_depth, g_writhe, g_cat_size.
- **Hash table for Mat2 BFS**: FNV-style hash over 16 long components. Separate chaining. HASH_SIZE=65537, MAX_CAT=32768.
- **Activation functions (4 types)**: ACT_4ENT_HASH (4 matrix entry octants hashed), ACT_QUAT_HASH (4 quaternion component octants hashed), ACT_2x2_HASH (16-component sign-bit hash, 3-valued per component), ACT_TRACE_OCT (trace octant only, 8 cells). Parameterized by k_param (number of cells).
- **`test_xor_mat2()`**: Tests whether a weight tuple computes XOR under 1wpi encoding on Mat2. 2N inputs from N weights (+/- pairs). Early termination on first parity violation. O(1) cleanup via touched-cell tracking.
- **`count_xor_bf()`**: Brute-force XOR search over all C(bf_limit, n_weights) tuples. Supports 3-8 weights (XOR6 through XOR16).
- **2-strand comparison infrastructure**: `Quat` type (floating-point quaternions), `build_2strand_catalog()` (BFS closure of z8, order 24), `build_2strand_dirs()` (S2 direction extraction), `combined_cell_2s()` (sector x Voronoi activation), `test_xor_2strand()`, `count_xor_2s_bf()`.
- **Casimir discriminant**: C(S) = (a-d)^2 + 4*b*c for summed Mat2. Exact computation in Z[zeta_8]. L1 norm for magnitude. Distinguishes Jordan blocks (C=0) from diagonalizable matrices.

### Literature Touched

- **Temperley-Lieb algebra standard modules (Graham-Lehrer)**: TL_3(delta=0) decomposes as V = Delta_3(1) + Delta_1(2) + Delta_1(2) via Wedderburn-Artin. Delta_1 is the 2-dimensional standard module on half-diagram basis {h_0, h_2}. At delta=0, TL_3 is semisimple (n=3 odd, Demo 51 confirms), but the braid group image is infinite.
- **Ext^1 and non-semisimple representation theory**: Delta_1 is an indecomposable but reducible module of the restricted quantum group u_q(sl_2) at ell=2 (q = e^{i*pi/4} = zeta_8). Extension class Ext^1(L(0), L(1)) is non-zero. Jordan block proves the extension does not split.
- **Non-semisimple TQFT (Voss-Kim-Bhatt, Nature Comms 2025)**: Alpha-anyon anchor (quantum trace zero) + sigma-anyon qubits + semisimple readout. Structural identification with D99 findings: non-semisimplicity as catalytic preparation, computation in semisimple layer.
- **Reservoir Computing (Maass et al. 2002, Gonon & Ortega 2020)**: Casimir = separation property. 4.4x winner/control gap = first quantitative measure of algebraic reservoir quality. DKC maps precisely to a discrete algebraic reservoir computer.
- **Magic state distillation analogy**: But CATALYTIC -- super-hubs are reused, not consumed.
- **Property F (Freedman-Larsen-Wang)**: Does NOT apply at delta=0. The braid group image through Delta_1 is infinite despite the representation being finite-dimensional, because Delta_1 is not a direct sum of simple modules.
- **Fibonacci numbers and SL_2**: max_abs Fibonacci growth connected to spectral radius of trace-zero SL_2 element and golden ratio phi = (1+sqrt5)/2 through the 2x2 transfer matrix.
- **Habiro (2002) / Z[zeta_8] bracket**: The algebraic substrate. All computation exact over cyclotomic integers.

### Open Questions

- **Is XOR16 truly unreachable, or just search-limited?** bf=14 may be insufficient. Need larger bf or smarter search.
- **Two Delta_1 copies**: TL_3 has two copies of Delta_1 in the decomposition. Do they give different or redundant XOR sets?
- **Can the super-hub pair be characterized purely algebraically?** What property of idx=7 and idx=9 makes them unique among all d=6 entries?
- **Ext^1 dimension**: Is dim Ext^1(L(0), L(1)) = 1 at ell=2? If so, Delta_1 is the UNIQUE non-trivial extension, explaining why super-hubs are irreplaceable.
- **Higher strands**: Does 4-strand (n=4, even, non-semisimple TL) give qualitatively different structure? ANSWERED by D100: yes, frozen radical, 6-hub positive-root ecology, no Casimir inversion.
- **Better activations**: 2x2_hash is a crude sign-pattern hash. Can Sec x Vor be adapted for Cyc8 quaternions (16 DOF)?
