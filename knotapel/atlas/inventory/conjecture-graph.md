# Conjecture Graph: Directed Graph of Open Conjectures and Proven Theorems

A directed graph of all significant conjectures and theorems with implication edges, organized by topic chain. Generated 2026-02-26 from theorems.md, proofs-index.md, research-questions.md, narrative.md, and connections.md.

---

## 1. Node List

### 1.1 Axiality Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T1 | Axiality Theorem: At delta=0, every braid bracket is axial (single nonzero Z[zeta_8] component). | VERIFIED (131K braids) | D35 | State-sum analysis; proof sketch via single-loop isolation + phase arithmetic |
| T2 | Z[i]-Axiality at delta=sqrt(2): every bracket value factors as (a+bi)*zeta_16^c. | VERIFIED (89K braids) | D54 | Exhaustive enumeration |
| T3 | Full Z[zeta_5]-Axiality at delta=phi: bracket values populate all 4 basis coordinates. | VERIFIED | D58 | Exhaustive enumeration |
| T4 | Axiality Breakdown Hierarchy: axiality strictness decreases with ell (1D->1D->2D->4D). | VERIFIED | D35,53,54,58 | Cross-ell comparison |
| T5 | Z/4Z Axis-Alignment Theorem: every braid matrix entry over TL module at delta=0 is axis-aligned. Constructive phase formula. | PROVEN | D107 | Constructive proof: TL generators have {0,1} entries; braid generators give {0,A,A^{-1}}; bipartiteness ensures same-axis addition |
| C1 | Axiality formal proof: T1 proof sketch upgraded to rigorous proof. | CONJECTURED | D35 | Likely: Graham-Lehrer cellularity + delta=0 loop-killing argument |
| C2 | Axiality hierarchy explanation: derivable from cyclotomic Galois theory (Q4.2). | CONJECTURED | D35,53,54,58 | Likely: Galois group action on cyclotomic coordinates |

### 1.2 Radical Structure Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T6 | Sandwich Theorem: TL_{2k}(0) has Loewy length 3 with top iso bottom iso TL_{2k-1}(0). | VERIFIED (n=2-8) | D51 | Peirce block exact matching |
| T7 | Alternating Semisimplicity: TL_n(0) semisimple iff n odd. | PROVEN | D51 | Graham-Lehrer 1996 |
| T8 | Radical Dimension at delta=0: rad(TL_{2k}(0)) = C_{2k} - C_{2k-1}. | VERIFIED (n=2-8) | D51 | Computational |
| T9 | Universal Nilpotency 3: rad^3=0 for ALL TL_n at ALL tested roots of unity. | VERIFIED (ell=2..7) | D52,60 | Exhaustive at each ell |
| T10 | Cross-ell Radical Dimension: rad(TL_ell) = 2ell-3 for all ell >= 2. | PROVEN (P01) | D52,60; proofs/ | Graham-Lehrer cellular theory; V_{ell-2} unique degenerate cell module with corank 1 |
| T11 | Next-Level Radical Formula: rad(TL_{ell+1}) = ell^2-ell-3 for all ell >= 3. | PROVEN (P02) | D39H; proofs/ | Graham-Lehrer linking theorem |
| T12 | Universal Corank 1 at first degeneracy. | PROVEN | D39H; proofs/ | Structural via linking theorem |
| T13 | Quotient Isomorphism: TL_{2k}(0)/rad iso TL_{2k-1}(0). | PROVEN (n=2,4,6) | D38 | Structural invariant comparison + TL relation checking |
| T14 | Cellular Decomposition: sector j has d(n,j)^2 basis elements. | PROVEN (n=2-8) | D39 | Exhaustive verification |
| T15 | Cell Filtration Monotonicity: ts(a*b) <= min(ts(a),ts(b)). | PROVEN (n=4,6) | D39 | Exhaustive pair verification |
| T16 | Markov RT Truncation Theorem: dim ker(B_M) = dim J(A) + sum (dim L_j)^2 for j >= ell-1. | PROVEN (P03) | D39I-J; proofs/ | Three-part: Markov vanishes on negligible ideal; negligible kills j >= ell-1; excess formula |
| T17 | Even-n Gram Rank Reduction: rank = C_{n-1} at even n at delta=0. | PROVEN | D38 | Goodman-de la Harpe-Jones theory |
| T18 | Sector-0 Death at delta=0: j=0 sector entirely in radical for even n. | PROVEN | D38,39 | Gram rank 0 in j=0 sector |
| T19 | Catalan Trace Theorem: all nonzero fixed-point counts are Catalan numbers. Window formula. | VERIFIED (n=2-8) | D51 | Computational |
| T20 | Fibonacci Rank Theorem: total bilinear rank of rad^2 generator = F(ell-1). | VERIFIED (ell=3-7) | D52,60 | Computational; conjectured from TL tower fusion rules |
| C3 | Sandwich Theorem proof: rad^2(TL_{2k}(0)) iso TL_{2k-1}(0) rigorously (Q2.1). | CONJECTURED | D51 | Likely: cellular algebra theory; known head iso socle for PIMs, need algebra-level identification |
| C4 | Universal Nilpotency 3 proof: rad^3=0 for ALL TL_n at ALL roots of unity. | CONJECTURED | D52,60 | Likely: Graham-Lehrer cellularity forces Loewy length <= 3 for cellular algebras where cell bilinear forms have rad^2=0 |
| C5 | Palindromic Sector Rank Conjecture: for even ell, sector rank profile is palindromic. | CONJECTURED | D52 | Verified ell=4,6 only |
| C6 | Catalan Trace Theorem proof: formal proof for window formula. | CONJECTURED | D51 | Likely: trace form + cellular structure |
| C7 | Fibonacci Rank formal proof: relate F(ell-1) to length-2 paths in Bratteli diagram. | CONJECTURED | D52,60 | Likely: anyonic fusion rules + Bratteli diagram structure |
| C8 | Second-Degeneracy Corank = ell+1 at n=ell+2 (Q2.9). | VERIFIED (ell=3-6) | D39H | Likely provable from linking theory |
| C9 | Galois symmetry conjecture: sandwich duality corresponds to Gal(Q(zeta_8)/Q) (Q2.2). | CONJECTURED | D51 | Speculative: rep theory meets number theory |
| C10 | Radical dimension as f(n,delta): universal formula across all ell (Q2.6). | CONJECTURED | D52 | No clean closed form for arbitrary n=ell+m known |

### 1.3 Indecomposability Chain (D85-D86)

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T21 | Indecomposability parameter b=-5/8 at TL_4 from first principles. | DEMONSTRATED | D85 | Leading-coefficient extraction from delta-parameterized Markov trace |
| T22 | Valuation Condition: b extraction requires p_tt = 2*p_Tt. Holds at TL_4, FAILS at TL_6. | VERIFIED/FAILS | D85 | Direct computation |
| T23 | Standard modules have no Jordan blocks at delta=0 (n=4-12). | DEMONSTRATED | D85 | Minimal polynomial square-free |
| T24 | Fixed-point form gauge manifold is flat: 3D gauge freedom at TL_4. | DEMONSTRATED | D85 | Quadratic coefficient matrix M=0 |
| T25 | Delta-form divergence on single P_{0,0}: universal negative result. | DEMONSTRATED | D86 | Exhaustive over eigenvalues, lattice sizes, forms |
| T26 | Projective Cover Dimension = C_{n/2+1}: TL_4->5=C_3, TL_6->14=C_4, TL_8->42=C_5. | OBSERVED (3 pts) | D86 | Pattern recognition |
| C11 | Valuation Condition Repair: modified method to avoid divergence at larger lattice sizes (Q2.10). | CONJECTURED | D85,86 | Likely requires transfer matrix + finite-size scaling (Pearce-Rasmussen method) |
| C12 | P_{0,0} dimension = next Catalan number as theorem (Q10.2). | CONJECTURED | D86 | Likely provable from cellular algebra structure |
| C13 | b and DKC computational capacity: quantitative relationship (Q10.1). | CONJECTURED | D84,85 | Open |

### 1.4 Forward DKC Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T27 | Forward DKC Theorem: exact Z[zeta_8] bracket values compute XOR without training. | PROVEN | D29 | Exhaustive construction, RMS=0.000 |
| T28 | Parity Unreachability (split-sigmoid): XNOR3 has 0 solutions in 100M quartets. | PROVEN | D48 | Exhaustive search |
| T29 | Parity Reachability at k=6: 906 solutions for XOR/XNOR from same catalog. | PROVEN | D50 | Exhaustive search |
| T30 | Four-Tier NPN Hierarchy: k=2(5), k=3(+1), k=4(+6), k=6(+1=parity). | PROVEN | D50 | Exhaustive k=2..8 |
| T31 | Complex Minsky-Papert: k=2 half-plane cannot compute 3-input parity with any complex weights. | PROVEN | D49 | Analytical proof |
| T32 | 11/13 Half-Plane Theorem: exactly 11/13 NPN reachable for all half-plane activations on multiplicative encodings. | PROVEN | D61,62 | Computational (N=360) + analytical |
| T33 | 0x1B Unreachability: algebraic contradiction (interval squeeze). | PROVEN | D62 | 3-step algebraic proof |
| T34 | 0x06 Unreachability: parallelogram argument. | PROVEN | D62 | Four-semicircle proof |
| T35 | Braid Computational Universality: NOT+NAND via bracket amplitudes. | PROVEN | D18 | Exhaustive gate search |
| T36 | Boolean Circuit Compiler: full 1-bit adder correct. | PROVEN | D19 | 8/8 test cases |
| T37 | Forward-Reverse Duality: rho=-0.82. | VERIFIED | D45 | Spearman correlation |
| T38 | Phase Decoherence Thesis: split-sigmoid destroys phase. | VERIFIED | D46 | Amplitude-only RMS << gauge RMS |
| T39 | Four-Tier NPN from Octant-Sector Geometry: purely geometric derivation. | PROVEN | D63 | Exhaustive k=2..16 |
| T40 | Parity Requires Alternating OM Type: only alternating oriented matroid achieves parity. | PROVEN | D63G | Exhaustive over ~1M non-alternating triples |
| T41 | 4-Input Parity at k=8: 96 solutions, tetraskelion. | PROVEN | D63H | Exhaustive |
| T42 | 5-Input Parity at k=15: 3020 solutions, pentaskelion. | PROVEN | D63I | Exhaustive |
| T43 | Parity Ceiling for Z[zeta_8] is n=5: pigeonhole for n>=8, constraint wall for n=6,7. | PROVEN | D63J | Exhaustive + pigeonhole |
| T44 | Parity Ceiling is Algebra-Independent: identical tier thresholds at Z[zeta_8] and Z[zeta_16]. | PROVEN | D65 | Exhaustive k=2..31 both catalogs |
| T45 | Matroid Minor Chain: {XOR2..XOR5} is deletion-contraction closed. | PROVEN | D64 | Exhaustive all levels |
| T46 | Self-Doubling Impossibility: doubled weight preserves class-1, contradicts parity. | PROVEN | D64 | Algebraic + exhaustive |
| T47 | Minimum Wall Distance d=4: closest XOR6 miss is 4 Hamming errors. | PROVEN | D64 | Exhaustive |
| C14 | Convexity Thesis formalization: 0x1B/0x06 wall is fundamentally about activation convexity (Q1.1). | CONJECTURED | D57,61,62 | Likely: formal convexity theory applied to activation geometry |
| C15 | Unified proof for 0x06 and 0x1B: single obstruction framework (Q1.6). | CONJECTURED | D62 | Hard: two currently distinct proof structures |
| C16 | Funnel Rate Conjecture: upward extensibility follows algebraic/exponential decay (Q1.14). | OPEN | D64 | Four data points: 99.3%, 39.2%, 19.2%, 0% |
| C17 | Incommensurability as theorem: prove gcd(k,8) correlation rigorously (Q1.5). | CONJECTURED | D50 | Hard: currently 3 data points |
| C18 | Universal k Theorem: k=2M-1 sectors places every non-zero ray in odd sector (Q1.10). | CONJECTURED | D63J | Proof sketch provided; formal proof needed |

### 1.5 Quaternionic / Geometric Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T48 | 24-Cell is the Braid Image: SU(2) reps produce exactly 24-cell vertices. | PROVEN | D66 | Exhaustive enumeration (87,890 braids) |
| T49 | Antipodal Necessity for quaternionic XOR6: only antipodal weight pairs. | PROVEN | D66 | Exhaustive C(24,6) |
| T50 | Universal 7/7/11 Structure: every 24-cell XOR6 solution uses 7+7+11 cells. | PROVEN | D66 | All 35 verified |
| T51 | F4 Decomposes to 6 Orbits in two families. | PROVEN | D66 | Group action computation |
| T52 | Hopf Phase is Computationally Inert: zero XOR6 at all k=4..32. | PROVEN | D67 | Exhaustive |
| T53 | S2 is the Computational Sweet Spot: 13-direction Voronoi beats S3. | DEMONSTRATED | D67 | 36 vs 35 solutions with fewer cells |
| T54 | Intrinsic Curvature Theorem: Euclidean metric -> 0 solutions; S2 metric -> 36. | PROVEN | D68 | Exhaustive |
| T55 | Direction Nesting Theorem: zeta_N eigenvector directions nest into zeta_{2N}. | PROVEN algebraically | D69 | sigma_i(2theta) = sigma_i(theta)^2; axis preserved |
| T56 | The 13=13 Theorem: minimum bandwidth l=6 because 2*6+1=13. Sharp 0->100% transition. | PROVEN computationally | D71 | Phase transition analysis |
| T57 | Spectral Universality: all 36 XOR6 share same spectral envelope (l=6 ~80%). | DEMONSTRATED | D71 | All 36 winners |
| T58 | Yang-Baxter produces identical melodies. | PROVEN | D70 | Step-by-step verification |
| C19 | 13=13 Theorem analytical proof: for N points in general position, min bandwidth = ceil((N-1)/2) (Q6.11). | CONJECTURED | D71 | Hard: requires binary Voronoi theory on S2 |
| C20 | Trivialization DOF Prediction: min bandwidth l >= (N-1)/2 (Q6.12). | CONJECTURED | D71 | Not yet proven analytically |
| C21 | F4 orbit count formula: XOR_n on 24-cell yields n*(n-1)/2 orbits? (Q6.9). | CONJECTURED | D66 | One data point: 6 orbits for XOR6 |
| C22 | Optimized Design Lower Bound: 36 is strict optimum over all 13-point S2 configs? (Q7.1). | CONJECTURED | D72 | Mechanism identified (D88: rigid anchors vs flexible scaffolding); formal proof open |

### 1.6 Scaling / Depth Law Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T59 | Linear Depth Law: max_xor ~ depth + 6 at zeta_12. | DEMONSTRATED | D82 | Empirical fit |
| T60 | Logarithmic Scaling Law: max_xor ~ 0.62*log2(catalog) + 4.6 at zeta_12. | DEMONSTRATED | D81 | Empirical fit (corollary of T59) |
| T61 | Balanced Exponentials Linearity: BFS ~2x/round vs parity 4x/weight -> slope 1. | CONFIRMED | D89,90,91 | Supply (D90) + demand (D91) sides |
| T62 | Parity Wall Theorem: max_xor determined by parity constraint, not activation resolution. | DEMONSTRATED | D91 | k_sec sweep 2-48, Voronoi 2-114 dirs |
| T63 | Depth Gap Invariance: gap between XOR levels constant across activation resolutions. | DEMONSTRATED | D91 | 9x9 interaction grid |
| T64 | Spectral Inversion: S2 bandwidth DECREASES with depth while computation improves. | DEMONSTRATED | D90 | BW_90% drops l=4 -> l=2 |
| T65 | Axis Cancellation: XOR winners select anti-aligned quaternion pairs (min_dot -0.75 vs -0.65). | DEMONSTRATED | D90 | Statistical comparison |
| T66 | zeta_8 Wall Theorem: XOR10=0 exhaustively; finite group ceiling at 24 elements. | PROVEN | D78 | Exhaustive all 42,504 quintuples |
| T67 | Finite Group Ceiling: finite quaternion group imposes hard capacity ceiling; infinite has none. | DEMONSTRATED | D79,80 | 7 roots surveyed |
| T68 | Framing = +2 XOR levels: Jones normalization costs exactly 2 levels at every root tested. | DEMONSTRATED | D83 | zeta_8 and zeta_12 |
| T69 | Writhe and depth are independent computational axes: r=0.139 near-zero correlation. | DEMONSTRATED | D83 | 4096 entries |
| C23 | Depth law slope=1 analytical derivation from first principles (Q7.36). | CONJECTURED | D89-91 | Hard: connect BFS branching factor to parity constraint growth rate |
| C24 | Combinatorial-Algebraic Balance: precise coefficient from BFS branching factor. | CONJECTURED | D89 | Supported but not analytically derived |
| C25 | Possibly Sub-Logarithmic Scaling: O(log N / log log N) rather than O(log N) (Q7.19). | CONJECTURED | D81 | 3 data points; XOR14 test needed |
| C26 | Resource Decomposition: max_xor = lattice_base(6) + 2*depth + 2*framing exactly? (Q8.1). | CONJECTURED | D82,83 | Supported; independence of resources unproven |
| C27 | Depth law universality across roots: same linear law at zeta_10, zeta_16? (Q7.21). | CONJECTURED | D82 | Mechanism identified; cross-root test remaining |
| C28 | Framing loss universality: +2 loss at all roots? (Q7.23). | CONJECTURED | D83 | 2 data points; need zeta_16, zeta_24 |
| C29 | Casimir Balance Hypothesis: linear depth law from representation-theoretic capacity growth. | CONJECTURED | D89 | Suggested by researcher; consistent but unproven |

### 1.7 Null State / Anti-Correlation Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T70 | Null Indispensability at zeta_8: removing nulls drops XOR8 -> XOR6. | DEMONSTRATED | D84 | Removal experiment |
| T71 | Null Directional Efficiency: 1.00 dir/entry for nulls vs 0.47 for non-null. | DEMONSTRATED | D84 | Census |
| T72 | Null Dispensability at zeta_12: infinite group nulls computationally dispensable. | DEMONSTRATED | D87 | Removal preserves all XOR levels |
| T73 | Two-Role Direction Theorem: 4 body-diagonal anchors (rigid) + 6 edge-midpoint scaffolding (flexible). | DEMONSTRATED | D88 | Perturbation analysis |
| T74 | K-Ladder Resilience: Sec x Vor provides 14x better perturbation resilience than Voronoi-only. | DEMONSTRATED | D88 | 6.4% vs 89% loss |
| T75 | Anti-Correlation Theorem: better spherical sampling produces fewer XOR6 solutions. | DEMONSTRATED | D72 | t=0 vs t~1 comparison |
| C30 | Anti-correlation formal theorem: any t>=1 design on S2 yields strictly fewer XOR6 solutions than eigenvectors. | CONJECTURED | D72,88 | Mechanism identified; formal proof open |
| C31 | Null fraction dilution: ζ4=75%, ζ8=37.5%, ζ12=3.0%; trend toward zero in infinite groups. | CONJECTURED | D84 | 3 data points; zeta_10 gap |
| C32 | Null dispensability transition location: where between zeta_8 and zeta_12? (Q7.32). | CONJECTURED | D87,88 | Per-direction residual as control parameter? |
| C33 | Maximum-depth null generalization: max-depth stratum of any finite SU(2) subgroup = all bracket-null? (Q7.29). | CONJECTURED | D84 | Verified only at zeta_8 |
| C34 | Logarithmic partner hypothesis: LCFT null/partner structure explains null geometry. | CONJECTURED | D84 | Consistent but not proven |
| C35 | LCFT Jordan-cell structure explicit: null/non-null on same axis = literal Jordan-cell pair? (Q7.27). | CONJECTURED | D84,85 | Hard: need representation-theoretic identification |

### 1.8 Parity-Lock / Encoding Chain (D92-D93)

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T76 | Parity-Lock Theorem: +/-q encoding locked to XOR/XNOR only; AND/OR/MAJ impossible. | PROVEN (P04) | D92; proofs/ | 3^k equivalence classes; Hamming parity preservation |
| T77 | Complement-Blindness Theorem: 1wpi with combined_cell, m and ~m map to same cell. | PROVEN | D93 | S(~m) = -S(m); |qa| normalization collapses |
| T78 | Phase-Cell Recovery: removing qa sign flip recovers all 13 NPN classes. | DEMONSTRATED | D93 | Complement-pair sharing 100% -> 1.1% |
| T79 | Circuit Complexity Hierarchy in DKC: AND degrades gracefully, MAJ cliff, XOR pigeonhole. | DEMONSTRATED | D93 | AND/XOR ratio 1.01 -> infinity, N=3-8 |
| T80 | Pigeonhole Mechanism: 2^N masks > cells kills XOR; AND needs only 1 unique cell. | DEMONSTRATED | D93 | Direct counting |
| C36 | Depth law under phase_cell: does linear law hold when full function set recoverable? (Q11.1). | CONJECTURED | D93 | Approachable test |
| C37 | Depth law for non-parity functions: different slopes for different Boolean functions? (Q7.37). | CONJECTURED | D91,92 | Different constraint growth rates expected |
| C38 | MAJ cliff as threshold phenomenon: sharp TC0 meets pigeonhole, or gradual? (Q11.3). | CONJECTURED | D93 | Related to MAJ's Fourier structure |

### 1.9 Group Structure / Solvability Chain (D94-D97)

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T81 | Solvability Bottleneck: non-solvable 2I consistently outperforms solvable z8 at matched size. | CONFIRMED | D94 | 24 vs 24 matched catalog |
| T82 | Circuit Complexity Hierarchy Universality: AND/XOR ratio explosion in ALL group structures. | CONFIRMED | D94 | z8, 2I, zeta_12 all tested |
| T83 | Crossover Hypothesis: small N -> dense/infinite wins; large N -> non-solvable finite wins. | DEMONSTRATED | D94 | Crossover at N=6 |
| T84 | 2I Perfect Group: 60/60 elements are single commutators. | CONFIRMED | D95 | Computational verification |
| T85 | Cross-Layer Synergy: neither COMM nor NON-COMM alone; capacity from mixing strata. | DEMONSTRATED | D95 | ALL(24) 2.7x vs either subset |
| T86 | 90-Degree Algebra-over-Geometry: geometrically identical entries from different depths -> union computes. | DEMONSTRATED | D95 | 0+0=26% at N=4 |
| T87 | Optimal z8 Catalog is A+B+C (21 entries, not 24). | DEMONSTRATED | D96 | Pruning Q8-null improves capacity |
| T88 | Cell B Perfection: 6 entries, 3 mutually orthogonal directions, 100% XOR at N=3-6. | DEMONSTRATED | D96 | Exhaustive |
| T89 | Orthogonal-Frame Perfection: Cell B's 100% arises from geometric inevitability, not algebraic fine-tuning. 50-degree robust plateau (25-75 deg). | CONFIRMED | D97 | Angular sweep |
| T90 | Lever Arm Mechanism: nonzero real component provides lever for phase_cell discrimination. | DEMONSTRATED | D97 | 90 deg = collapse |
| C39 | Synergy Universality: D84 synergy (null/non-null 0+0=36) and D95 synergy (derived layers 0+0=26%) are the same split through different lenses. | CONJECTURED | D95 | Two independent observations |
| C40 | Optimal catalog from first principles: constructive procedure without brute-force XOR testing (Q12.4). | CONJECTURED | D96,97 | Hard: combine pruning + orthogonal-frame recipe |

### 1.10 Multi-Strand / Representation Theory Chain (D98-D107)

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T91 | Readout Bottleneck: 3-strand trace readout catastrophically lossy (5.1% retention). | DEMONSTRATED | D98 | Column-4 vs trace comparison |
| T92 | Delta_1 XOR Capacity: first 3-strand XOR at all levels; XOR6-14 achieved. | DEMONSTRATED | D99 | 2x2 standard module + 16-component sign-pattern |
| T93 | Ext^1 Catalytic Preparation: non-split extension is computational resource. | ESTABLISHED | D99 | End-to-end causal chain |
| T94 | Constant Radical Content: W_{4,2} mean radical_content=2 at all depths. | CONFIRMED | D100 | All entries verified |
| T95 | sl_d Growth Rate: BFS growth converges to n-1 for n-strand. | CONFIRMED (3 pts) | D98,100,101 | 3-strand ~2.2x, 4-strand ~3.1x, 5-strand ~4.0x |
| T96 | BFS Growth is Braid Group Invariant: identical across modules for same n. | PROVEN (n=6) | D102,103 | Three modules bit-for-bit identical |
| T97 | Barrington-Radical Principle: radical carries abelian writhe character; by Barrington, useless for XOR. | NAMED THEOREM | D102 | Writhe character + Barrington's theorem |
| T98 | Writhe Character Theorem: M*r = A^{writhe}*r for all 32,768 entries. | PROVEN | D102 | Exhaustive verification |
| T99 | Radical-as-Resource Hypothesis REFUTED: simple W_{6,0} beats non-simple W_{6,4}. | REFUTED | D102 | Head-to-head comparison |
| T100 | Simple vs Non-Semisimple Difference: W_{5,3} broader shallow, less deep capacity. | DEMONSTRATED | D101 | XOR level comparison |
| T101 | Nesting Parity Z/2Z Grading: bipartite coloring = nesting count mod 2. | PROVEN (n=2-6) | D107 | 195 matchings, 0 mismatches |
| T102 | q-Catalan Identity: C_{2m+1}(-1) = (-1)^m * C_m. | PROVEN | D107 | Algebraic |
| T103 | Orthogonality Theorem: mult quotient = K_5 (Z/4Z), add quotient = star tree. | PROVEN | D107 | Computational |
| C41 | Major Index Bipartite Invariant: maj mod 2 = BFS coloring for general n,j (Q107). | CONJECTURED | D107 | Verified W_{4,2}(3/3), W_{6,2}(9/9) |
| C42 | 2-Adic Neutrality of Nilpotent TL: 2-adic valuation decays geometrically. | CONJECTURED | D107 | 5 candidate proof frameworks |
| C43 | Effective Dimension Hypothesis: action density = effective_dim/dim^2 is key metric. | CONJECTURED | D103 | Would explain j=0 module superiority |
| C44 | Macrame Principle: branching interaction dominates for simple sub-blocks (dim 4-5); within-block for complex (dim 14). | CONJECTURED | D105 | 2 data points: n=6 vs n=8 |
| C45 | Sign-Rank Expansion Scaling: increases with module dimension, decreases with oversampling. | CONJECTURED | D104,105 | 6 modules tested |
| C46 | k-Regime Theory: k/2^N ratio controls XOR capacity. D93 "XOR dies" was pigeonhole artifact. | CONFIRMED | D105 | Revives XOR at high k |
| C47 | Hub Count Prediction FALSIFIED: predicted 12 for sl_4, found 3. | REFUTED | D101 | Simple module context? |
| C48 | Casimir-XOR Inversion in simple modules: correlation inverts at high XOR levels. | DEMONSTRATED | D101 | Non-semisimple all positive; simple inverts at XOR10+ |
| C49 | Broad vs deep capacity general: simple modules broader shallow, less deep? (Q12.9). | CONJECTURED | D100,101 | 2 data points |
| C50 | sl_d growth at n=6: ~5x predicted (Q12.1). | CONJECTURED | D98-101 | 3 points confirm; 4th establishes |

### 1.11 Dual-Channel / Encoding Lock Chain (D108-D109)

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T104 | Dual-Channel Theorem: parity requires BOTH multiplicative phase coherence AND additive magnitude diversity. | NOVEL | D108 | Graph structure analysis |
| T105 | Product Closure as Parity Discriminator: connected graph necessary; max contrast 17 vs 0 edges. | NOVEL | D108 | Computational |
| T106 | Partition Coincidence as Incapacity Signature: triple collapse = incapacity. | NOVEL | D108 | Raqiya analysis |
| T107 | Resolution Dependence: same value can be parity-capable at k=6, poison at k=15. | NOVEL | D108 | 14 dual-classified values |
| T108 | Encoding-Dependent Dual-Channel: polarity of multiplicative channel flips with encoding type. | NOVEL | D109 | D108+D109 paired experiment |
| T109 | Perfect Algebraic Symmetry at delta=sqrt(2): parity/non-parity identical on 7/8 metrics. | NOVEL | D109 | Product closure sole discriminator |
| C51 | Amy Bridge: DKC dual-channel = T-gate/Hadamard decomposition in quantum circuit synthesis. | CONJECTURED | D108 | Would connect DKC to quantum circuit resource theory |
| C52 | j=0 Liveness implies Structural Universality: delta!=0 -> rich algebra -> Raqiya can't distinguish. | CONJECTURED | D109 | Supported by D108 vs D109 contrast |
| C53 | Dual-Channel Theorem generalization: holds across all evaluation levels? | CONJECTURED | D108,109 | 2 delta values tested |

### 1.12 Finiteness / Group Theory Chain

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T110 | Only zeta_4 and zeta_8 finite under two-perpendicular-axes construction. ADE: E_7 and Binary Dihedral. | DEMONSTRATED | D80 | 7 roots surveyed |
| T111 | Quantum dimension vanishing at zeta_8: [2]_q = 0 exactly. | PROVEN | D80 | Exact algebraic |
| C54 | Power-of-Two Finiteness Conjecture: finite groups exactly at angles pi/2^k. Predicts zeta_32 finite. | CONJECTURED | D80 | 2 finite cases confirmed; zeta_32 untested |
| C55 | Union Catalog Conjecture: combined zeta_8 + zeta_12 -> superadditive capacity? | CONJECTURED | D79 | 12% quaternion overlap, 15% direction overlap |

### 1.13 Miscellaneous / Bracket Properties

| ID | Statement | Status | Demo | Technique |
|----|-----------|--------|------|-----------|
| T112 | Entanglement Vanishing at delta=0: ALL non-interleaving braids have bracket=0. | PROVEN | D35 | Exhaustive (1024 braids) |
| T113 | Entanglement Vanishing Breakdown at delta!=0. | VERIFIED | D53 | delta=1 non-interleaving nonzero |
| T114 | FK = Ising Equivalence. | PROVEN | D56 | Exhaustive subset enumeration |
| T115 | Fibonacci-Delta Connection: delta=phi, phi^k = F(k-1)+F(k)*phi. | PROVEN | D58 | Exact Cyc5 arithmetic |
| T116 | Compressed Sensing on S2: 3.5x super-Nyquist compression. | EXPLAINED | D71 | Compressed sensing argument |
| C56 | Sparsity Formula: TL generator e_1 has C_n-C_{n-1} nonzero entries. | CONJECTURED | D35,38 | Verified n=2-5,8 |
| C57 | Sweet-Spot Cell-Count Formula: masks/cells ~ 4.6 at XOR6 sweet spot. | CONJECTURED | D76,77 | 1 data point; XOR8 threshold ~100 cells |
| C58 | DKC as Discrete Algebraic Reservoir Computer: precise mapping to RC framework. | PROPOSED | D94 | Testable prediction: rank ratio > size ratio |

---

## 2. Edge List

### 2.1 Implication Edges (IMPLIES: if X proven, Y follows or is strengthened)

| From | To | Type | Note |
|------|----|------|------|
| C1 (axiality proof) | T5 | IMPLIES | Formal axiality proof would provide foundation for axis-alignment theorem |
| C3 (sandwich proof) | C6 (Catalan trace proof) | IMPLIES | Sandwich duality constrains trace structure |
| C3 (sandwich proof) | C7 (Fibonacci rank proof) | IMPLIES | Sandwich identification links rad^2 to TL_{2k-1} fusion rules |
| C4 (nilpotency 3 proof) | C3 (sandwich proof) | IMPLIES | Universal Loewy length 3 is a prerequisite for sandwich identification |
| C14 (convexity thesis) | C15 (unified 0x06/0x1B proof) | IMPLIES | Convexity framework would provide single obstruction language |
| C14 (convexity thesis) | T32 | WOULD_STRENGTHEN | Would give conceptual foundation for 11/13 wall |
| C19 (13=13 analytical) | C20 (trivialization DOF) | IMPLIES | General bandwidth formula would predict trivialization threshold |
| C23 (depth law slope proof) | C26 (resource decomposition) | IMPLIES | Analytical slope would make decomposition exact |
| C23 (depth law slope proof) | C27 (depth law universality) | IMPLIES | First-principles derivation would predict cross-root behavior |
| C26 (resource decomposition) | C28 (framing universality) | IMPLIES | Exact decomposition includes framing as independent term |
| C51 (Amy Bridge) | C53 (dual-channel generalization) | IMPLIES | T-gate/Hadamard analogy would predict behavior at other delta |
| C54 (power-of-two finiteness) | T67 (finite ceiling) | WOULD_STRENGTHEN | Would classify all finite capacity ceilings |
| T76 (parity-lock) | C36 (depth law under phase_cell) | IMPLIES | Parity-lock shows depth law is parity-specific; phase_cell unlocks other functions |
| T76 (parity-lock) | C37 (non-parity depth law) | IMPLIES | Different functions have different constraint growth -> different slopes |
| T97 (Barrington-Radical) | T99 (radical-as-resource refuted) | USED_IN_PROOF | Abelian character -> Barrington -> parity impossible -> radical useless |
| C35 (LCFT Jordan-cell) | C34 (logarithmic partner) | EQUIVALENT | Making Jordan-cell explicit IS the logarithmic partner structure |
| C40 (optimal catalog) | C22 (design lower bound) | IMPLIES | Constructive catalog procedure would establish optimality |

### 2.2 Proof-Chain Edges (USED_IN_PROOF: X was used to establish Y)

| From | To | Type | Note |
|------|----|------|------|
| T7 (alternating semisimplicity) | T10 (radical dim 2ell-3) | USED_IN_PROOF | Semisimplicity criterion identifies first non-semisimple n |
| T10 (radical dim 2ell-3) | T11 (next-level radical) | USED_IN_PROOF | V_{ell-2} contribution from P01 propagates to P02 |
| T10 (radical dim 2ell-3) | T17 (Gram rank reduction) | USED_IN_PROOF | Gram rank = C_ell - rad = C_ell - (2ell-3) |
| T14 (cellular decomposition) | T15 (cell filtration) | USED_IN_PROOF | d(n,j)^2 sizes underlie filtration structure |
| T15 (cell filtration) | T16 (Markov RT truncation) | USED_IN_PROOF | Filtration provides sector structure for truncation argument |
| T12 (universal corank 1) | T11 (next-level radical) | USED_IN_PROOF | Corank 1 gives contribution shape at first degeneracy |
| T27 (forward DKC) | T28 (parity unreachability) | USED_IN_PROOF | DKC framework establishes the search space |
| T28 (parity unreachability) | T29 (parity reachability) | USED_IN_PROOF | Wall location (activation) motivates k-sector design |
| T31 (complex Minsky-Papert) | T32 (11/13 theorem) | USED_IN_PROOF | Half-plane limitation is the mechanism |
| T33 (0x1B proof) | T32 (11/13 theorem) | USED_IN_PROOF | Component proof 1 of 2 |
| T34 (0x06 proof) | T32 (11/13 theorem) | USED_IN_PROOF | Component proof 2 of 2 |
| T39 (four-tier geometry) | T30 (four-tier hierarchy) | USED_IN_PROOF | Geometric derivation reproduces exhaustive result |
| T40 (alternating OM) | T43 (parity ceiling n=5) | USED_IN_PROOF | OM type constrains which weight configurations can achieve parity |
| T45 (matroid minor chain) | T46 (self-doubling) | USED_IN_PROOF | Minor chain structure reveals the dominant failure mode |
| T48 (24-cell) | T49 (antipodal necessity) | USED_IN_PROOF | 24-cell geometry constrains viable weight structures |
| T48 (24-cell) | T52 (Hopf inert) | USED_IN_PROOF | Binary octahedral group structure separates base from fiber |
| T52 (Hopf inert) | T53 (S2 sweet spot) | USED_IN_PROOF | Fiber inertness proves S2 sufficiency |
| T53 (S2 sweet spot) | T56 (13=13 theorem) | USED_IN_PROOF | 13 directions on S2 is the starting point for bandwidth analysis |
| T55 (direction nesting) | T56 (13=13 theorem) | USED_IN_PROOF | Nesting shows 13 directions are universal across zeta_N |
| T61 (balanced exponentials) | T59 (linear depth law) | USED_IN_PROOF | Provides the mechanistic explanation |
| T64 (spectral inversion) | T65 (axis cancellation) | USED_IN_PROOF | Inversion proves mechanism is relational not positional |
| T76 (parity-lock) | T79 (circuit complexity) | USED_IN_PROOF | Lock mechanism determines which functions are possible |
| T77 (complement-blindness) | T78 (phase-cell recovery) | USED_IN_PROOF | Identifying the symmetry shows how to break it |
| T81 (solvability bottleneck) | T82 (hierarchy universality) | USED_IN_PROOF | Tested across solvable and non-solvable groups |
| T96 (BFS invariant) | T97 (Barrington-Radical) | USED_IN_PROOF | Identical BFS shows radical doesn't affect growth |
| T98 (writhe character) | T97 (Barrington-Radical) | USED_IN_PROOF | 1D character identification enables Barrington argument |
| T103 (orthogonality) | T104 (dual-channel) | USED_IN_PROOF | Mult/add orthogonality is the structural basis for dual channels |
| T5 (Z/4Z axis-alignment) | T103 (orthogonality) | USED_IN_PROOF | Axis-alignment determines the quotient graph structure |
| T5 (Z/4Z axis-alignment) | T101 (nesting parity) | USED_IN_PROOF | Bipartite coloring defined on axis-aligned entries |

### 2.3 Would-Strengthen Edges (proof of X elevates Y)

| From | To | Type | Note |
|------|----|------|------|
| C1 (axiality proof) | C2 (axiality hierarchy) | WOULD_STRENGTHEN | Proof technique for T1 would likely extend to explain hierarchy |
| C3 (sandwich proof) | T6 (sandwich theorem) | WOULD_STRENGTHEN | Upgrades VERIFIED to PROVEN |
| C4 (nilpotency 3 proof) | T9 (universal nilpotency) | WOULD_STRENGTHEN | Upgrades VERIFIED to PROVEN |
| C6 (Catalan trace proof) | T19 (Catalan trace) | WOULD_STRENGTHEN | Upgrades VERIFIED to PROVEN |
| C7 (Fibonacci rank proof) | T20 (Fibonacci rank) | WOULD_STRENGTHEN | Upgrades VERIFIED to PROVEN |
| C14 (convexity thesis) | T38 (phase decoherence) | WOULD_STRENGTHEN | Convexity framework explains why split-sigmoid fails |
| C19 (13=13 analytical) | T56 (13=13 theorem) | WOULD_STRENGTHEN | Upgrades computational to analytical proof |
| C23 (depth law slope) | T59 (linear depth law) | WOULD_STRENGTHEN | Upgrades DEMONSTRATED to PROVEN |
| C26 (resource decomposition) | T68 (framing +2) | WOULD_STRENGTHEN | Places framing in exact capacity formula |
| C30 (anti-correlation formal) | T75 (anti-correlation) | WOULD_STRENGTHEN | Upgrades DEMONSTRATED to PROVEN |
| C41 (major index) | T101 (nesting parity) | WOULD_STRENGTHEN | Generalizes from j=0 to arbitrary j |
| C51 (Amy Bridge) | T104 (dual-channel) | WOULD_STRENGTHEN | Places dual-channel in established QCS framework |

### 2.4 Equivalence Edges

| Node A | Node B | Type | Note |
|--------|--------|------|------|
| C34 (logarithmic partner) | C35 (LCFT Jordan-cell) | EQUIVALENT | Same structure stated differently |
| T60 (logarithmic scaling) | T59 (linear depth law) | EQUIVALENT | Corollary relationship: log law = depth law + exponential BFS growth |
| C26 (resource decomposition) | C24 (combinatorial-algebraic balance) | EQUIVALENT | Same thesis from capacity vs mechanism perspectives |

---

## 3. Critical Path Analysis

### 3.1 Highest-Leverage Proof Targets

**Rank 1: C3 -- Sandwich Theorem Proof**
- Downstream: C6 (Catalan trace proof), C7 (Fibonacci rank proof), C5 (palindromic conjecture), C9 (Galois symmetry)
- Upgrades: T6 (sandwich) from VERIFIED to PROVEN
- Method: Graham-Lehrer cellular algebra theory; the key step is identifying rad^2 as an algebra isomorphic to TL_{2k-1}, not just dimensionally matching. The Peirce block matching (T6 verified) provides the computational scaffold.
- Impact: Unlocks 4+ downstream results and completes the radical anatomy arc.

**Rank 2: C23 -- Depth Law Slope = 1 Analytical Derivation**
- Downstream: C26 (resource decomposition exact), C27 (cross-root universality), C28 (framing universality), C37 (non-parity depth law)
- Upgrades: T59 (depth law) from DEMONSTRATED to PROVEN
- Method: Connect BFS branching factor (~2x at zeta_12) to parity constraint growth rate (4x per weight). The balanced exponentials argument (T61) provides the framework; what's missing is the algebraic derivation of the BFS branching factor from SU(2) generator structure.
- Impact: Would establish the foundational scaling law of DKC from first principles. Highest theoretical payoff.

**Rank 3: C14 -- Convexity Thesis Formalization**
- Downstream: C15 (unified 0x06/0x1B proof), strengthens T32 (11/13 wall)
- Method: D57 showed additive encoding worse than multiplicative at Re>0. D62 proved 0x06 and 0x1B analytically. A convexity framework would unify these under one obstruction: half-plane partitions are convex sets, and additive weight sums preserve convexity, while multiplicative products can rotate out.
- Impact: Elevates Paper 2 from two independent proofs to a single conceptual framework.

**Rank 4: C19 -- 13=13 Theorem Analytical Proof**
- Downstream: C20 (trivialization DOF prediction)
- Upgrades: T56 (13=13) from computational to analytical
- Method: For N points on S2 in general position, prove min bandwidth of separating binary Voronoi partition = ceil((N-1)/2). The result 2*6+1=13 would follow. Likely requires binary Voronoi theory + spherical harmonic analysis.
- Impact: Transforms the key spectral result into a general theorem about binary partitions on S2.

**Rank 5: C51 -- Amy Bridge (DKC = T-gate/Hadamard)**
- Downstream: C53 (dual-channel generalization)
- Upgrades: T104 (dual-channel) by placing it in established QCS framework
- Method: Map product closure -> sde (T-gate count), activation -> Hadamard, and show the resource theory matches Amy-Glaudell-Ross 2023.
- Impact: Would connect DKC to quantum circuit synthesis resource theory -- a major publication target.

**Rank 6: C4 -- Universal Nilpotency 3 Proof**
- Downstream: C3 (sandwich proof), and transitively everything C3 unlocks
- Method: Graham-Lehrer cellularity: cell module bilinear forms have rad^2=0, which forces algebra Loewy length <= 3. The key is showing this holds for ALL cellular structures at roots of unity, not just delta=0.
- Impact: Foundational result for TL algebra structure theory.

**Rank 7: C1 -- Axiality Formal Proof**
- Downstream: C2 (axiality hierarchy), strengthens T5 (Z/4Z alignment)
- Method: Single-loop isolation + phase arithmetic from the proof sketch. Need to formalize the step "surviving states differ by >=2 flips" and "A^4=-1 keeps contributions on same axis."
- Impact: Upgrades the founding observation of the program from VERIFIED to PROVEN.

**Rank 8: C41 -- Major Index Bipartite Invariant**
- Downstream: strengthens T101 (nesting parity) by generalizing from j=0 to all j
- Method: Verified at W_{4,2} and W_{6,2}. Need to prove maj mod 2 = bipartite coloring for general W_{n,j} TL standard modules.
- Impact: Novel algebraic invariant of TL link states.

**Rank 9: C7 -- Fibonacci Rank Formal Proof**
- Downstream: C5 (palindromic conjecture)
- Method: Connect F(ell-1) to length-2 paths in the TL Bratteli diagram. Fusion rules for anyonic sectors should give the Fibonacci recurrence.
- Impact: Deepest connection between entanglement rank and anyonic computational power.

**Rank 10: C26 -- Resource Decomposition Exact Proof**
- Downstream: C28 (framing universality)
- Depends on: C23 (depth law slope)
- Method: Prove depth, writhe, and lattice base contribute independently to angular vocabulary with no interaction terms.
- Impact: Foundational capacity formula for DKC.

---

## 4. Equivalence Classes

### Class E1: Depth/Scaling Laws
Results that are essentially the same phenomenon measured differently:
- T59 (linear depth law) <-> T60 (logarithmic scaling) <-> T61 (balanced exponentials)
- C23 (slope proof) <-> C24 (combinatorial-algebraic balance) <-> C26 (resource decomposition)
- These are ONE result: depth law. The log law is a corollary; balanced exponentials is the mechanism; resource decomposition is the capacity formula.

### Class E2: Null State Structure
Results that are aspects of the same null/non-null partition:
- T70 (null indispensability at zeta_8) <-> T73 (two-role theorem) <-> T74 (k-ladder resilience)
- C31 (null dilution) <-> C32 (dispensability transition) <-> C33 (max-depth null)
- C34 (logarithmic partner) <-> C35 (LCFT Jordan-cell)
- These describe the same underlying structure: bracket-null entries as Jordan-cell partners in the non-semisimple radical.

### Class E3: Parity Lock Mechanisms
Different formulations of why specific encodings are locked to specific functions:
- T76 (parity-lock) <-> T77 (complement-blindness)
- Both are encoding-creates-collisions arguments. T76: +/-q creates (0,0)=(1,1) collision -> only parity survives. T77: 1wpi creates m=~m collision -> only complement-invariant functions survive.
- They are structurally parallel: encoding symmetry -> equivalence class collapse -> function restriction.

### Class E4: Activation-is-the-Wall
Three instances of the same pattern:
- T28+T29 (D48->D50): split-sigmoid wall, k-sector resolution
- T66 (D78) -> T67 (D79): finite group wall, infinite group resolution
- D76 -> D77: S2-only wall, S1xS2 resolution
- Same meta-theorem: before concluding unreachability, identify the specific component enforcing the wall.

### Class E5: Algebra-over-Geometry
Multiple demonstrations that algebraic selection beats geometric optimization:
- T75 (anti-correlation) <-> T73 (two-role theorem) <-> T54 (intrinsic curvature) <-> T53 (S2 sweet spot)
- All show that data-intrinsic algebraic structure outperforms geometrically optimal or uniform arrangements.

### Class E6: Radical Anatomy
The sandwich theorem, radical dimensions, and quotient isomorphisms are all facets of the same structure:
- T6 (sandwich) <-> T8 (rad dim) <-> T13 (quotient iso) <-> T17 (Gram rank) <-> T18 (sector-0 death)
- These describe the same object: the radical of TL_{2k}(0), from different angles.

### Class E7: Dual-Channel
Aspects of the same discriminator:
- T104 (dual-channel) <-> T105 (product closure) <-> T106 (partition coincidence) <-> T108 (encoding-dependent) <-> T109 (perfect symmetry)
- These are all measurements of the same underlying structure: parity capability requires both multiplicative and additive algebraic diversity.

---

## 5. Upgrade Paths

### Path U1: Axiality Theorem (C1) -- VERIFIED -> PROVEN
**Current state:** Proof sketch exists (3 steps: single-loop isolation, 2-loop death at delta=0, phase arithmetic). 131K braids zero counterexamples.
**Likely technique:** Formalize the state-sum argument. Key step: prove that surviving (single-loop) states differ by >=2 crossing flips, and that each pair of flips changes sigma by 0 or +/-4, so A^{4}=-1 keeps all contributions on the same zeta_8 axis.
**Difficulty:** MODERATE. The argument is clean; the formalization is straightforward algebra.

### Path U2: Sandwich Theorem (C3) -- VERIFIED -> PROVEN
**Current state:** Dimensional match + Peirce block matching verified for n=4,6,8.
**Likely technique:** Graham-Lehrer cellular theory. The key insight is that rad^2(TL_{2k}) should be identified as the cell module bilinear form's kernel composed with itself, and the resulting object carries the TL_{2k-1} algebra structure via the cellular basis.
**Difficulty:** MODERATE. The Peirce block data gives strong guidance.

### Path U3: 13=13 Analytical Proof (C19) -- COMPUTATIONAL -> ANALYTICAL
**Current state:** Sharp 0->100% phase transition at l=6 verified computationally. 2*6+1=13.
**Likely technique:** Prove that for N points on S2 in general position, the minimum bandwidth of any binary Voronoi partition separating them is ceil((N-1)/2). This is a statement about the minimum number of spherical harmonic degrees of freedom needed to represent a binary indicator function with N transition regions.
**Difficulty:** HARD. Requires novel spherical harmonic analysis.

### Path U4: Depth Law Slope (C23) -- CONFIRMED -> PROVEN
**Current state:** Balanced exponentials confirmed from supply (D90) and demand (D91) sides. Slope ~1 empirically.
**Likely technique:** Derive the BFS branching factor ~2x algebraically from the SU(2) generator construction (two perpendicular-axis generators at half-angle theta). Show that at zeta_12, the number of new group elements per closure round converges to 2. Then: 2^d catalog size, 4^w parity constraint size -> d = 2w -> max_xor ~ d/2 + base.
**Difficulty:** HARD. The BFS branching factor depends on group structure in ways that are not algebraically transparent.

### Path U5: Convexity Thesis (C14) -- VERIFIED -> PROVEN
**Current state:** Additive encoding worse than multiplicative (D57). Two independent proofs for 0x06/0x1B (D62).
**Likely technique:** Define convexity precisely for activation partitions on S1. Prove: (1) half-plane partition is convex; (2) additive weight sums preserve convexity of input sets; (3) the truth tables of 0x06 and 0x1B require non-convex activation regions. The convexity obstruction then follows.
**Difficulty:** MODERATE. The key challenge is the precise definition of "convexity" for angular partitions.

### Path U6: Fibonacci Rank (C7) -- VERIFIED -> PROVEN
**Current state:** F(ell-1) verified at ell=3,4,5,6,7 including cubic number field.
**Likely technique:** The Bratteli diagram for the TL tower at a given root of unity has connection matrix entries from fusion rules. Length-2 paths in this Bratteli diagram count as Fibonacci numbers because the adjacency matrix has spectral radius phi (golden ratio, from the ADE classification). The bilinear rank of rad^2 counts exactly these length-2 paths.
**Difficulty:** MODERATE. The connection to Bratteli diagram path counting is the key insight.

### Path U7: Parity-Lock -> Non-Parity Depth Law (C36, C37)
**Current state:** Parity-lock proven (P04). Phase_cell recovers all 13 NPN classes.
**Likely technique:** Run the depth law measurement under phase_cell activation (breaks complement-blindness). Different functions will have different constraint growth rates (AND is AC0 -> constant depth; MAJ is TC0 -> polynomial depth; XOR is not in AC0 -> exponential depth). Predict slopes from Fourier degree of truth table.
**Difficulty:** APPROACHABLE (computational); MODERATE (theoretical prediction of slopes).

### Path U8: Dual-Channel -> Amy Bridge (C51)
**Current state:** Dual-channel theorem novel (D108). T-gate/Hadamard analogy proposed.
**Likely technique:** Map DKC's product closure graph to the sde (smallest denominator exponent) resource counting in Amy-Glaudell-Ross 2023. Show that the product closure metric is equivalent to sde for the Z[zeta_8] ring, and that the activation channel providing magnitude diversity plays the role of the Hadamard gate (sde=0 compensation).
**Difficulty:** MODERATE. Requires detailed reading of Amy et al. 2023.

---

## 6. Open Questions Mapped to Graph Nodes

| Research Question | Related Node(s) | Priority |
|-------------------|-----------------|----------|
| Q1.1 (convexity formalization) | C14 | Tier 2 |
| Q1.5 (incommensurability theorem) | C17 | Tier 3 |
| Q1.6 (unified 0x06/0x1B proof) | C15 | Tier 3 |
| Q2.1 (sandwich proof) | C3 | Tier 2 |
| Q2.2 (Galois symmetry) | C9 | Tier 3 |
| Q2.6 (radical dim f(n,delta)) | C10 | Tier 2 |
| Q2.9 (second-degeneracy corank proof) | C8 | Tier 2 |
| Q2.10 (valuation condition repair) | C11 | Tier 3 |
| Q2.12 (P_{0,0} divergence reason) | C11 | Tier 3 |
| Q4.2 (axiality hierarchy) | C2 | Tier 2 |
| Q6.9 (F4 orbit formula) | C21 | Tier 2 |
| Q6.11 (13=13 analytical proof) | C19 | Tier 3 |
| Q7.1 (anti-correlation formal) | C30 | Tier 2 |
| Q7.19 (sub-logarithmic scaling) | C25 | Tier 1 |
| Q7.21 (depth law cross-root) | C27 | Tier 1 |
| Q7.23 (framing universality) | C28 | Tier 1 |
| Q7.27 (LCFT Jordan-cell) | C35 | Tier 3 |
| Q7.29 (max-depth null) | C33 | Tier 2 |
| Q7.32 (null dispensability transition) | C32 | Tier 1 |
| Q7.36 (depth law slope proof) | C23 | Tier 3 |
| Q8.1 (resource decomposition proof) | C26 | Tier 2 |
| Q10.1 (b and DKC capacity) | C13 | Tier 2 |
| Q10.2 (P_{0,0} = next Catalan) | C12 | Tier 1 |
| Q11.1 (depth law under phase_cell) | C36 | Tier 1 |
| Q11.3 (MAJ cliff) | C38 | Tier 2 |
| Q11.4 (RKHS kernel rank) | C58 | Tier 1 |
| Q12.1 (sl_d growth n=6) | C50 | Tier 1 |
| Q12.4 (optimal catalog) | C40 | Tier 3 |
| Q12.9 (broad vs deep) | C49 | Tier 2 |

---

## 7. Summary Statistics

- **Total nodes:** 115 proven/verified theorems (T-series) + 58 conjectures (C-series) = 173 nodes
- **Proven (formal proof in proofs/):** 4 (P01: T10, P02: T11, P03: T16, P04: T76)
- **Proven (computational/analytical):** ~65
- **Verified/Demonstrated/Confirmed:** ~46
- **Conjectured/Open:** ~58
- **Refuted:** 3 (T99 radical-as-resource, C47 hub count, Clifford staircase partially)
- **Implication edges:** 17
- **Proof-chain edges:** 33
- **Would-strengthen edges:** 12
- **Equivalence classes:** 7

The graph has two major connected components:
1. **The Radical-Algebraic Component:** T6-T20, C3-C10 (radical anatomy) connecting through T10-T16 (formal proofs) to the cellular/sandwich theory
2. **The DKC-Computational Component:** T27-T109, C14-C58 (everything from forward DKC through dual-channel) connecting through the depth law chain and encoding theory

The bridge between them is the **Barrington-Radical Principle** (T97), which directly connects radical structure (component 1) to computational capacity (component 2) via the argument that the radical carries only abelian (writhe) information and is therefore computationally inert by Barrington's theorem.

---

*Source: theorems.md (130 novelty items), proofs-index.md (4 formal proofs), research-questions.md (95 open questions across 12 categories), narrative.md, connections.md. Generated 2026-02-26.*
