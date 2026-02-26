# Theorems, Conjectures & Key Results

Extracted from 92 demos (demo-index.md) and explorer's log. Organized by topic.
Updated 2026-02-20: Added results from Demos 38 (dimension reduction), 39 (symmetry decomposition), 60 (ell=7 cubic wall), 63 (Angular Anatomy), 39 Parts G-J (Markov RT truncation, second-degeneracy coranks).
Updated 2026-02-21: Added results from Demos 64 (parity matroid recursion), 65 (Clifford staircase), 66 (quaternionic DKC), 67 (coordinate zoo), 68 (stereographic DKC), 69 (Clifford staircase on S²), 70 (Musica Nodorum), 71 (spectral DKC).
Updated 2026-02-21: Added results from Demos 72 (spherical design test), 73 (eigenvector automaton), 74 (computational invariant), 75 (binocular DKC), 76 (8-input scaling), 77 (activation zoo), 78 (recursive scaling + phase diagram), 79 (ζ₁₂ capacity), 80 (group finiteness survey), 81 (capacity scaling law), 82 (crossing depth).
Updated 2026-02-21: Added results from Demos 83 (framing as computational content), 84 (null states and logarithmic structure).
Updated 2026-02-23: Added results from Demos 85 (indecomposability parameter b), 86 (direct b on P_{0,0} — negative result), 87 (null indispensability at ζ₁₂), 88 (anti-correlation mechanism), 89 (depth law mechanism), 90 (sum-angle structure), 91 (activation bottleneck), 92 (function scaling / parity-lock theorem). P04 Parity-Lock Theorem upgraded to PROVEN (formal proof in proofs/).
Updated 2026-02-24: Added results from Demos 93 (complement-blindness, phase-cell recovery, circuit complexity hierarchy, pigeonhole mechanism), 94 (solvability bottleneck, hierarchy universality, crossover hypothesis, DKC as discrete algebraic reservoir computer).
Updated 2026-02-26: Added results from Demos 102 (Barrington-Radical Principle, writhe character, radical-as-resource refuted), 103 (BFS growth = braid group invariant, dim(W) law eliminated, curse of dimensionality), 104 (Atkinson sweet spot, branching interaction, sign-rank expansion, k is the real lever), 105 (k-regime theory, macrame principle, rank saturation, D93 XOR-dies refuted), 106 (TL visibility filter, topological entropy null, relational computation strengthened), 107 (Z/4Z axis-alignment theorem, nesting parity, major index invariant, q-Catalan identity, orthogonality theorem), 108 (dual-channel theorem, product closure discriminator, partition coincidence), 109 (encoding-dependent dual-channel, perfect algebraic symmetry at delta=sqrt(2)).

---

## 1. Axiality

### Axiality Theorem
- **Statement:** At delta=0 (A = e^{i*5pi/4}), for ANY braid, the Kauffman bracket is always axial: <K> = n * A^c for some integer n and some c in {0,1,...,7}. Equivalently, the Z[zeta_8] representation has at most one nonzero component.
- **Status:** COMPUTATIONALLY VERIFIED (exhaustive for n=2 length 1-10, n=3 length 1-8, n=4 length 1-5; ~131K braids, zero counterexamples)
- **Demo provenance:** Demo 35 (discovery), explorer's log (proof sketch)
- **Significance:** Collapses the 4D cyclotomic ring Z[zeta_8] to a rank-1 sublattice for bracket values. The bracket COUNTS (with signs) single-loop states up to a phase. All bracket magnitudes are non-negative integers.

**Proof sketch (3 steps):**
1. Single-loop isolation: flipping one crossing of a 1-loop state always produces a 2-loop state (splits the curve)
2. 2-loop states die at delta=0 (contribute delta^1 = 0)
3. Phase arithmetic: surviving states differ by >=2 flips, each pair changes sigma by 0 or +/-4, and A^4 = -1 keeps contributions on the SAME zeta_8 axis

### Z[i]-Axiality at delta=sqrt(2)
- **Statement:** At delta=sqrt(2) (ell=4), every bracket value in Z[zeta_16] factors as (a+bi) * zeta_16^c for some Gaussian integer (a+bi) and axis c in {0,...,7}. All values are Z[i]-axial.
- **Status:** COMPUTATIONALLY VERIFIED (89,426 braids; n=2 len 1-10, n=3 len 1-8, n=4 len 1-6; 100% Z[i]-axial)
- **Demo provenance:** Demo 54
- **Significance:** Even at non-zero delta the bracket values don't use the full cyclotomic ring; they live in a 2D subspace (Gaussian integers times axes)

### Z[zeta_6]-Axiality at delta=1
- **Statement:** At delta=1 (ell=3), all bracket values are pure root-of-unity multiples of +/-1. The catalog collapses to 6 distinct values: {+/-1, +/-zeta_6, +/-zeta_6^2}.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 53
- **Significance:** Binary collapse — too few distinct values for meaningful DKC computation

### Full Z[zeta_5]-Axiality at delta=phi
- **Statement:** At delta=phi (ell=5, Fibonacci anyons), bracket values populate all 4 basis coordinates of Z[zeta_5]. Full axiality, not restricted to a 2D subspace.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 58
- **Significance:** Dramatically richer catalog than ell=4; the rank-4 ring is fully utilized

### Axiality Breakdown Hierarchy
- **Statement:** Axiality strictness decreases with ell: ell=2 (delta=0) has strict 1D axiality in Z[zeta_8]; ell=3 (delta=1) has strict 1D axiality in Z[zeta_6]; ell=4 (delta=sqrt(2)) has 2D Z[i]-axiality in Z[zeta_16]; ell=5 (delta=phi) has full 4D usage of Z[zeta_5].
- **Status:** COMPUTATIONALLY VERIFIED across ell=2,3,4,5
- **Demo provenance:** Demos 35, 53, 54, 58
- **Significance:** Establishes that delta=0 is the most constrained evaluation point

---

## 2. Radical Structure

### The Sandwich Theorem (Loewy Length 3)
- **Statement:** For k >= 2, TL_{2k}(delta=0) has Loewy length exactly 3 with three layers: Top (TL_{2k}/rad, dim C_{2k-1}, isomorphic to TL_{2k-1}(0) which is semisimple), Middle (rad/rad^2, dim C_{2k} - 2*C_{2k-1}), Bottom (rad^2, dim C_{2k-1}, isomorphic to TL_{2k-1}(0) as bimodule).
- **Status:** COMPUTATIONALLY VERIFIED (n=2 through 8; Peirce block exact matching confirms top ≅ bottom)
- **Demo provenance:** Demo 51, explorer's log
- **Significance:** NOVEL result. Papers prove head≅socle for individual PIMs but never identify the algebra's rad^2 AS the previous odd TL algebra. Top and bottom are dimensional mirrors.

### Alternating Semisimplicity at delta=0
- **Statement:** TL_n(delta=0) is semisimple for odd n (n=3,5,7,...) and non-semisimple for even n (n=2,4,6,8,...). TL_3 is semisimple DESPITE having nilpotent generators.
- **Status:** PROVEN (computationally verified n=2-8; known from Graham-Lehrer 1996)
- **Demo provenance:** Demo 51
- **Significance:** TL_3 semisimplicity was the explorer's key discovery; generators can be nilpotent while the algebra is semisimple

### Radical Dimension at delta=0
- **Statement:** rad(TL_{2k}(0)) has dimension C_{2k} - C_{2k-1} (difference of consecutive Catalan numbers). rad^2(TL_{2k}(0)) has dimension C_{2k-1}.
- **Status:** COMPUTATIONALLY VERIFIED (n=2-8)
- **Demo provenance:** Demo 51, explorer's log
- **Significance:** NOVEL explicit formulas; likely implicit in literature but not explicitly stated

### Universal Nilpotency 3 Conjecture
- **Statement:** rad^3 = 0 for ALL TL_n at ALL roots of unity (tested ell=2,3,4,5,6,7). The algebra Loewy length is universally <= 3.
- **Status:** COMPUTATIONALLY VERIFIED (all n tested at ell=2,3,4,5,6,7 — including first cubic number field case at ell=7)
- **Demo provenance:** Demo 52, Demo 60 (ell=7), explorer's log
- **Significance:** Contradicts PIM theory prediction of Loewy length 2*ell-1. Resolution: Graham-Lehrer cellularity forces Loewy length <= 3 for cellular algebras where cell module bilinear forms have rad^2 = 0.

### Semisimple Decompositions (odd n, delta=0)
- **Statement:** TL_3(0) ≅ M_2(k) + k. TL_5(0) ≅ M_5(k) + M_4(k) + k. TL_7(0) ≅ M_14(k) + M_14(k) + M_6(k) + k. Simple module dimensions follow ballot numbers.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 51, explorer's log
- **Significance:** Explicit Wedderburn-Artin structure

### TL_2(delta=0) is Dual Numbers
- **Statement:** TL_2(delta=0) = k[eps]/(eps^2), the dual number algebra. Radical dim = 1, nilpotency index 2.
- **Status:** PROVEN
- **Demo provenance:** Demo 51
- **Significance:** Simplest non-semisimple case

### Catalan Trace Theorem
- **Statement:** All nonzero fixed-point counts (traces of left multiplication by basis elements in TL_n(0)) are Catalan numbers. The nonzero values at each n are exactly {C_m : floor(n/2)+1 <= m <= n} — a window of ceil(n/2) consecutive Catalan numbers.
- **Status:** COMPUTATIONALLY VERIFIED (n=2-8)
- **Demo provenance:** Demo 51, explorer's log
- **Significance:** NOVEL. Not found in existing literature. Window formula: n=2→{C_2}, n=3→{C_2,C_3}, ..., n=8→{C_5,C_6,C_7,C_8}

### Sandwich Duality (Peirce Block Matching)
- **Statement:** rad^2(TL_{2k}(0)) has IDENTICAL Peirce block structure to TL_{2k-1}(0), verified for k=2,3,4 (i.e., n=4,6,8 vs n=3,5,7).
- **Status:** COMPUTATIONALLY VERIFIED (n=4-8)
- **Demo provenance:** Demo 51, explorer's log
- **Significance:** NOVEL. Computational verification of the algebra-level isomorphism

### Even-n Gram Rank Reduction at delta=0
- **Statement:** The Gram matrix of the TL_n(0) trace form has rank exactly C_{n-1} at even n (and full rank C_n at odd n). The bracket factors through a quotient TL_n(0)/rad isomorphic to TL_{n-1}(0), yielding asymptotic ~4x dimension reduction and ~64x speedup for O(dim^3) matrix operations.
- **Status:** PROVEN (verified n=2,4,6,8 for even; n=3,5,7 for odd; follows from Goodman-de la Harpe-Jones theory)
- **Demo provenance:** Demo 38
- **Significance:** Reduction factors: 2.00x (n=2), 2.80x (n=4), 3.14x (n=6), 3.33x (n=8) — C_n/C_{n-1} grows toward 4x asymptotically. Directly relevant to DKC weight search at scale.

### Sector-0 Death at delta=0
- **Statement:** For even n, the through-strand sector j=0 (no propagating strands) lies entirely in the radical — Gram rank 0. All j=0 basis elements are "dead."
- **Status:** PROVEN (verified n=2,4,6,8)
- **Demo provenance:** Demos 38, 39
- **Significance:** Identifies WHERE the radical lives at the sector level. Odd n has all sectors fully LIVE.

### Quotient Isomorphism TL_n(0)/rad ~ TL_{n-1}(0)
- **Statement:** The quotient algebra TL_{2k}(0)/rad is isomorphic to TL_{2k-1}(0). Verified via: (1) nonzero product counts in quotient multiplication table match TL_{n-1} exactly, (2) reduced generators in quotient satisfy all TL relations (nilpotency, adjacent, far commutativity).
- **Status:** PROVEN (verified n=2,4,6 via structural invariant comparison and relation checking)
- **Demo provenance:** Demo 38
- **Significance:** Strengthens the Sandwich Theorem — not just dimensional agreement but full algebra isomorphism with verified TL relation preservation.

### Cellular Decomposition Theorem
- **Statement:** In TL_n, the through-strand sector with j propagating strands contains exactly d(n,j)^2 basis elements, where d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1) is the Graham-Lehrer cell module dimension formula.
- **Status:** PROVEN (exhaustively verified n=2..8)
- **Demo provenance:** Demo 39
- **Significance:** Confirms the Graham-Lehrer cellular structure computationally — sector sizes are perfect squares of cell module dimensions.

### Cell Filtration Monotonicity
- **Statement:** For any two TL basis diagrams a, b: ts(a*b) <= min(ts(a), ts(b)), where ts counts through-strands. The through-strand count defines a genuine two-sided ideal filtration.
- **Status:** PROVEN (exhaustive verification for all diagram pairs at n=4 and n=6)
- **Demo provenance:** Demo 39
- **Significance:** Foundation for the block-triangular structure of TL generators in sector-ordered basis.

### Block-Lower-Triangular Structure
- **Statement:** In decreasing-j sector ordering of the standard basis, all TL generators are block-lower-triangular. However, at odd n (semisimple), generators are NOT block-diagonal — a true change-of-basis is needed to achieve Wedderburn block-diagonal form.
- **Status:** PROVEN (verified n=3,4,5; n=3 has 1 off-diagonal entry per generator; n=5 has 5-9)
- **Demo provenance:** Demo 39
- **Significance:** Semisimple does NOT imply block-diagonal in the natural basis — the standard basis respects the filtration but not the direct-sum decomposition.

### Cross-Sector Coupling Growth
- **Statement:** At delta=0, the sum of per-sector Gram ranks exceeds the full Gram rank by an amount that grows with n: n=4 coupling=0, n=6 coupling=4, n=8 coupling=104.
- **Status:** COMPUTATIONALLY VERIFIED (n=4,6,8)
- **Demo provenance:** Demo 39
- **Significance:** Cross-sector dependencies prevent naive sector-by-sector dimension reduction at larger n.

### Indecomposability Parameter b = -5/8 for Dense Polymers
- **Statement:** The indecomposability parameter b = -5/8 (Pearce-Rasmussen) is computed exactly from first principles at TL_4 via leading-coefficient extraction from the delta-parameterized Markov trace on the full regular representation. The regular-representation value b_delta = -5/2 divides by dim(0-TL sector) = 4 to give b_PR = -5/2 / 4 = -5/8, exact to 12 decimal places. The literature "controversy" between b = -2 (GRS normalization) and b = -5/8 (PR normalization) dissolves: they are the same structure under different conventions.
- **Status:** DEMONSTRATED (at TL_4; TL_6 regular rep diverges)
- **Demo provenance:** Demo 85
- **Cross-references:** Valuation Condition (below), Delta-Form Divergence on P_{0,0} (Demo 86 negative result)
- **Significance:** First exact first-principles computation of b from the diagram algebra. Resolves the GRS/PR normalization question.

### Valuation Condition for b Extraction
- **Statement:** The leading-coefficient b extraction requires p_tt = 2 * p_Tt (where p_tt and p_Tt are the minimum delta-powers in the bilinear forms). At TL_4: p_tt = 2, p_Tt = 1, satisfied. At TL_6: p_tt = 4, p_Tt = 3, FAILS — b diverges as delta^{-2}.
- **Status:** VERIFIED (at TL_4); FAILS (at TL_6)
- **Demo provenance:** Demo 85
- **Cross-references:** Indecomposability Parameter b (above), Delta-Form Divergence on P_{0,0} (Demo 86)
- **Significance:** Identifies the structural condition governing when the leading-coefficient method produces a finite b. The TL_6 failure with 25-fold multiplicity (vs TL_4's 4-fold) shows cross-copy interference grows faster than the multiplicity rescue.

### Standard Modules Have No Jordan Blocks at delta=0
- **Statement:** The Hamiltonian H on the standard module W_{n,0} (link states) has NO Jordan blocks at any size tested (n=4,6,8,10,12). The minimal polynomial is square-free (gcd(mu,mu') degree = 0) at all sizes. Jordan blocks exist exclusively on projective covers (P_{0,0}) and the full regular representation.
- **Status:** DEMONSTRATED (n=4,6,8,10,12)
- **Demo provenance:** Demo 85
- **Cross-references:** Indecomposability Parameter b (above), Delta-Form Divergence on P_{0,0} (Demo 86)
- **Significance:** FALSIFIES the claim (Gemini 3 Pro) that b is extracted from standard module Jordan structure. The non-semisimple structure needed for b computation lives in the projective cover, not the standard module.

### Fixed-Point Form Gauge Manifold is Flat
- **Statement:** At TL_4, the fixed-point trace bilinear form G[i][j] = fixpt(star(i)*j) is degenerate at delta=0 with 3-dimensional gauge freedom. The quadratic coefficient matrix M = 0, making b_fixpt an affine function of gauge parameters with no unique critical point. Both b = -2 and b = -5/8 are reachable points on this flat manifold. The delta-parameterized Gram matrix breaks the degeneracy.
- **Status:** DEMONSTRATED (at TL_4)
- **Demo provenance:** Demo 85
- **Cross-references:** Indecomposability Parameter b (above)
- **Significance:** Explains why different normalization conventions can produce different b values — the fixpt form has gauge freedom that the delta-form resolves.

### Delta-Form Divergence on Single P_{0,0} (Negative Result)
- **Statement:** The delta-parameterized Gram matrix approach universally diverges on a single projective cover P_{0,0} at every eigenvalue, every lattice size (TL_4, TL_6), and for both loop and trace forms. The valuation condition p_tt = 2*p_Tt never holds on a single copy. The multiplicity from the regular representation is structurally essential to the b computation, not just a scale factor.
- **Status:** DEMONSTRATED (novel negative finding — this approach has not been tried in the literature)
- **Demo provenance:** Demo 86
- **Cross-references:** Indecomposability Parameter b (Demo 85), Valuation Condition (Demo 85)
- **Significance:** The dim(V_0)^2 multiplicity from the regular representation participates in the valuation balance required for the b ratio to converge. TL_8 P_{0,0} is additionally found to be completely semisimple at lambda=0 (no Jordan blocks).

### Projective Cover Dimension Pattern
- **Statement:** dim(P_{0,0}) at TL_n equals C_{n/2+1} (the next Catalan number): TL_4 → 5 = C_3, TL_6 → 14 = C_4, TL_8 → 42 = C_5.
- **Status:** OBSERVED (three data points)
- **Demo provenance:** Demo 86
- **Cross-references:** Catalan Number Dimensions (Section: Verified Facts)
- **Significance:** Connects projective cover dimension to the Catalan sequence — one step up from the algebra dimension itself.

### Barrington-Radical Principle
- **Statement:** The radical of a non-semisimple TL module at delta=0 carries an abelian character (the writhe homomorphism B_n -> Z/8Z via A = -zeta_8). By Barrington's theorem, abelian groups cannot compute parity. Therefore the radical direction is theoretically guaranteed useless for XOR computation. CONFIRMED computationally: simple module W_{6,0} beats non-simple W_{6,4} at every XOR level despite same dimension (5).
- **Status:** NAMED THEOREM (computationally confirmed)
- **Demo provenance:** Demo 102
- **Cross-references:** Radical-as-Resource Hypothesis (REFUTED, below), Writhe Character Theorem (below)
- **Significance:** Connects TL representation structure to computational complexity theory. The radical is computationally inert because it carries only abelian (writhe) data.

### Writhe Character Theorem
- **Statement:** For W_{6,4}, M*r = A^{writhe(M)}*r for ALL 32,768 catalog entries (not just generators). The radical is "frozen" — it carries only the writhe data, acting as a 1-dimensional representation of the braid group through A = -zeta_8.
- **Status:** PROVEN (computationally, all 32,768 entries verified)
- **Demo provenance:** Demo 102
- **Cross-references:** Barrington-Radical Principle (above), Radical Content Constancy (below)
- **Significance:** The radical direction is a 1D character of the braid group, carrying only writhe/framing information.

### Radical Content Constancy (W_{6,4})
- **Statement:** The L1 norm of M*r equals 3 for every entry in the W_{6,4} catalog. The radical direction has constant magnitude regardless of braid word length or complexity.
- **Status:** PROVEN (all 32,768 entries verified)
- **Demo provenance:** Demo 102
- **Cross-references:** Writhe Character Theorem (above), Constant Radical Content in W_{4,2} (Demo 100)
- **Significance:** Extends the constant-radical-content observation from W_{4,2} (Demo 100, mean=2) to W_{6,4} (constant=3).

### Radical-as-Resource Hypothesis
- **Statement:** The prediction that non-semisimple structure (radical dim >= 1) provides computational advantage for XOR was decisively REFUTED. Simple module W_{6,0} outperforms non-simple W_{6,4} at XOR6 (2449 vs 2370), XOR8 (850 vs 793), and XOR10 (1 vs 0). The radical direction is computationally inert.
- **Status:** REFUTED
- **Demo provenance:** Demo 102
- **Cross-references:** Barrington-Radical Principle (above)
- **Significance:** Settles a major structural question: the radical does not help DKC computation; it carries only abelian (writhe) data.

### Fibonacci Max-Abs in Non-Simple Module
- **Statement:** W_{6,4} max coefficient magnitude follows strict Fibonacci (1,1,2,3,5,8,13) through depth 6. W_{6,0} follows Fibonacci through depth 5, breaks to 16 at depth 6. The radical constrains coefficient growth.
- **Status:** CONFIRMED (D102 for W_{6,4}/W_{6,0}; extended by D103 — both simple modules W_{6,0} and W_{6,2} follow 1,1,2,3,5,8,16)
- **Demo provenance:** Demo 102, Demo 103
- **Significance:** Fibonacci growth is universal for simple TL_6 modules; the non-simple module's radical constrains coefficients to strict Fibonacci.

---

## 3. DKC Computation

### Forward DKC Theorem
- **Statement:** Exact Z[zeta_8] bracket values at A = e^{i*5pi/4} compute XOR (and other Boolean functions) without any neural network training. 100+ valid XOR triples found from exact bracket values with gauge rotation.
- **Status:** PROVEN (exhaustive construction, RMS = 0.000)
- **Demo provenance:** Demo 29
- **Significance:** LANDMARK. Confirms that topological structure is sufficient for computation; training merely locates pre-existing solutions on the Z[zeta_8] lattice. "Training-as-Focusing thesis."

### Parity Unreachability (Split-Sigmoid)
- **Statement:** A single complex neuron with split-sigmoid activation and weights drawn from the Z[zeta_8] bracket catalog CANNOT compute 3-input parity (XNOR3). Exactly 0 solutions out of 100,000,000 quartets.
- **Status:** PROVEN (exhaustive, 100M quartets)
- **Demo provenance:** Demo 48
- **Significance:** XNOR3 is the ONLY 3-input Boolean function with zero valid forward DKC constructions. The wall is razor-thin (1 Hamming bit from EXACT1 which has 127K solutions)

### Parity Reachability at k=6 Sectors
- **Statement:** Replacing split-sigmoid with k-sector MVN classification makes parity reachable at k=6 with 906 solutions from the same 100-value Z[zeta_8] bracket catalog. All 13 NPN classes computable.
- **Status:** PROVEN (exhaustive search)
- **Demo provenance:** Demo 50
- **Significance:** BREAKTHROUGH. The parity wall was in the activation function, not the weight lattice.

### Four-Tier NPN Hierarchy
- **Statement:** 13 NPN classes partition into 4 tiers by minimum sector count: k=2 (5 classes: AND3', AND2', ~A~(BC), BUF, MAJ'), k=3 (+1: ~A(B^C)), k=4 (+6: EXACT1, ISOLATE, 3v-0x19, 3v-0x1B, A^(B|C), XOR2), k=6 (+1: XNOR3 = parity).
- **Status:** PROVEN (exhaustive search across k=2..8)
- **Demo provenance:** Demo 50
- **Significance:** Parity uniquely isolated at highest tier

### Non-Monotonic Parity Solutions
- **Statement:** Parity solution count is non-monotonic in k: 906@k=6 > 756@k=7 > 96@k=8. Anti-correlated with gcd(k,8).
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 50
- **Significance:** Sector boundaries that misalign with the lattice's pi/4 angular structure break the symmetry that blocks parity (incommensurability hypothesis)

### Triskelion Principle
- **Statement:** For 3-input parity at k=6, each of the 3 inputs requires its own odd sector (at 120-degree intervals). All three weights are confined to octants {2,4,5,7} (class-1 angles).
- **Status:** COMPUTATIONALLY VERIFIED (3-input specific; k=2n generalization FALSIFIED — see below). Demo 63 confirms triskelion is specific to 3-input parity; 4-input uses tetraskelion at k=8, 5-input uses pentaskelion at k=15.
- **Demo provenance:** Demo 50, Demo 63 (generalization falsified)
- **Significance:** Geometric explanation of why k=6 is the minimum for 3-input parity

### Four-Tier NPN from Octant-Sector Geometry
- **Statement:** The four-tier NPN reachability structure (k=2:5, k=3:+1, k=4:+6, k=6:+1) is reproduced entirely from pure octant-sector geometry: sector boundaries partition the 8 discrete Z[zeta_8] rays into class-0 and class-1 sets. k=6 is the UNIQUE sector count producing the parity octant set {2,4,5,7}.
- **Status:** PROVEN (exhaustive verification, k=2..16)
- **Demo provenance:** Demo 63 (Parts B, D)
- **Significance:** Reduces the four-tier hierarchy to discrete angular combinatorics — no search required

### Parity Requires Alternating Oriented Matroid Type
- **Statement:** Among the 21 distinct oriented matroid types of Z[zeta_8] weight triples, only the ALTERNATING type (sign sequence -,+,- or +,-,+, where s12=s23=-s13) achieves parity. The alternating condition means the three generators interleave around the circle. Uniform OM types achieve 12/13 NPN classes — everything except parity.
- **Status:** PROVEN (exhaustive: 0 of ~1M non-alternating triples achieve parity)
- **Demo provenance:** Demo 63 (Part G)
- **Significance:** First characterization of parity in terms of oriented matroid theory. The interleaving condition ensures pair sums swing to opposite parity from single and triple sums.

### 4-Input Parity (XOR) at k=8
- **Statement:** 4-input parity (truth table 0x6996) is reachable at k=8 with exactly 96 solutions from the Z[zeta_8] catalog. Tetraskelion structure: 4 weights placed in all 4 odd octants {1,3,5,7}. Not reachable at any k < 8.
- **Status:** PROVEN (exhaustive search)
- **Demo provenance:** Demo 63 (Part H)
- **Significance:** Extends DKC from 3-input to 4-input. Resolves Priority 1.2 from the research roadmap.

### 5-Input Parity at k=15
- **Statement:** 5-input parity is reachable only at k=15 (the universal k for M=8 rays) with 3020 solutions. k=15 makes all 7 non-zero octants class-1. Not reachable at any k in {2,...,14,16}.
- **Status:** PROVEN (exhaustive search over k=2..16)
- **Demo provenance:** Demo 63 (Part I)
- **Significance:** Pentaskelion structure. Requires the maximum possible class-1 coverage.

### Parity Ceiling for Z[zeta_8] is n=5
- **Statement:** The maximum number of inputs for which parity is computable by a single Z[zeta_8] MVN neuron is n=5. For n>=6, no sector count k in {2,...,16} produces any parity solutions. Two impossibility mechanisms: constraint-geometric wall (n=6,7: enough octants available but 63+ subset-sum constraints unsatisfiable) and pigeonhole (n>=8: only 7 class-1 octants, need distinct rays).
- **Status:** PROVEN (exhaustive search for n=6,7; pigeonhole argument for n>=8)
- **Demo provenance:** Demo 63 (Part J)
- **Significance:** Establishes the fundamental capacity limit of Z[zeta_8] DKC for the hardest boolean function.

### Universal k Theorem (k = 2M-1)
- **Statement:** For a lattice with M equally-spaced ray directions, k = 2M-1 sectors places every non-zero ray in an odd sector. For Z[zeta_8] (M=8): k=15. Proof: at k=2M-1, ray j falls in sector 2j-1 (always odd) for j=1,...,M-1.
- **Status:** CONJECTURED (proof sketch provided, not yet fully formal)
- **Demo provenance:** Demo 63 (Part J)
- **Significance:** Identifies the "maximal coverage" sector count for any regular lattice

### Two Types of Impossibility
- **Statement:** The n>=6 impossibility and n>=8 impossibility for Z[zeta_8] parity have fundamentally different characters. Pigeonhole (n>=8): only 7 class-1 octants, trivially provable. Constraint wall (n=6,7): enough octants available (7>=6) but the system of 2^n-1 sector-parity constraints is unsatisfiable. The gap between pigeonhole bound (n<=7) and actual bound (n<=5) is the "geometric surplus."
- **Status:** PROVEN (exhaustive + pigeonhole)
- **Demo provenance:** Demo 63 (Part J, findings.md)
- **Significance:** Distinguishes combinatorial from geometric impossibility

### Complex Minsky-Papert Theorem
- **Statement:** A k=2 half-plane classifier CANNOT compute 3-input parity with ANY complex weights. If y1 > d and y2 > d then y1+y2 > 2d > d, contradicting the parity requirement.
- **Status:** PROVEN (analytically)
- **Demo provenance:** Demo 49
- **Significance:** Establishes that the activation geometry, not the weight space, is the obstruction

### 11/13 Half-Plane Theorem
- **Statement:** For ALL half-plane activations on multiplicative encodings z(a,b,c) = w1^a * w2^b * w3^c, exactly 11 of 13 non-trivial NPN classes are reachable. The unreachable classes are 0x06 (XOR-AND) and 0x1B (CROSS), independent of angles or half-plane orientation.
- **Status:** PROVEN (computational proof at N=360, then analytical proof)
- **Demo provenance:** Demo 61 (computational), Demo 62 (analytical)
- **Significance:** Universal across ALL evaluation levels (verified at ell=4, 5, 6)

### 0x1B Unreachability Proof
- **Statement:** For any angles phi1, phi2, phi3 and any semicircle S, the truth table of in_semi(a*phi1 + b*phi2 + c*phi3, theta) cannot be NPN-equivalent to 0x1B (CROSS).
- **Status:** PROVEN (3-step algebraic contradiction: interval squeeze forces phi3 > pi and phi3 < pi simultaneously)
- **Demo provenance:** Demo 62
- **Significance:** Clean algebraic proof, not just computational

### 0x06 Unreachability Proof
- **Statement:** For any angles and any semicircle, truth table cannot be NPN-equivalent to 0x06 (XOR-AND).
- **Status:** PROVEN (four-semicircle parallelogram argument: alpha+beta > pi forces all gaps < pi, making intersection empty)
- **Demo provenance:** Demo 62
- **Significance:** Different proof structure from 0x1B — local vs global obstruction

### Forward-Reverse Duality
- **Statement:** Functions that topology produces frequently (forward map via braid enumeration) are exactly those whose trained weights decompose cleanly back into bracket values (reverse map via gauge correction). Spearman rho = -0.8167.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 45
- **Significance:** Strongest evidence that the bracket algebra has intrinsic computational structure measurable from either direction

### Bracket Self-Consistency
- **Statement:** Forward construction, forward enumeration, and reverse decomposition produce the same preference ordering over Boolean functions. rho(solutions, topo_freq) = +0.78, rho(solutions, reverse_RMS) = -0.79.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 48
- **Significance:** Three independent measures agree

### Topology-Frequency Correlation Reversal
- **Statement:** Split-sigmoid and MVN-continuous access DIFFERENT sectors of the bracket algebra. Spearman rho(pn-RMS, topo_freq): split-sig = -0.78, MVN-cont = +0.55, MVN-k8 = -0.10.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 47
- **Significance:** The two activations are literally complementary

### Phase Decoherence Thesis
- **Statement:** Split-sigmoid activation treats Re and Im independently, producing weights whose magnitudes align with bracket magnitudes but whose phases are essentially random relative to Z[zeta_8]. No post-training rotation can recover what training never encoded.
- **Status:** COMPUTATIONALLY VERIFIED (amplitude-only RMS 0.33 << per-neuron gauge RMS 0.57)
- **Demo provenance:** Demo 46
- **Significance:** Settles the path forward: fix activation (MVN) or accept amplitude-only matching

### Convexity Thesis
- **Statement:** The 0x1B/0x06 wall is fundamentally about activation convexity, not encoding geometry. Half-planes are convex; additive sums preserve convexity; multiplicative products can rotate out of half-planes (anti-convex). Non-convex activations bypass the wall.
- **Status:** COMPUTATIONALLY VERIFIED (additive worse than multiplicative at Re>0)
- **Demo provenance:** Demo 57
- **Significance:** Explains why encoding swap doesn't help

### Braid Computational Universality
- **Statement:** Specific braid words exist whose bracket amplitude encodes NOT and NAND truth tables. NOT + NAND achieves classical computational universality via braids.
- **Status:** PROVEN (exhaustive search finds gates; composition verified)
- **Demo provenance:** Demo 18
- **Significance:** NOT gate: 2 strands, 6 crossings. NAND gate: 3 strands, 5 crossings.

### Boolean Circuit Compiler
- **Statement:** Arbitrary boolean expressions can be compiled and evaluated via recursive Kauffman bracket thresholding of braid gates, including a full 1-bit adder (all 8 input combinations correct) at A = e^{i*5pi/4}.
- **Status:** PROVEN (full adder passes 8/8)
- **Demo provenance:** Demo 19
- **Significance:** Gate composition preserves correctness through cascade thresholding

### Matroid Minor Chain Theorem
- **Statement:** The parity hierarchy {XOR2, XOR3, XOR4, XOR5} is deletion-contraction closed: deleting any weight from an XOR_n solution always yields an XOR_{n-1} solution (100%, all levels n=5→4→3→2); contracting any input to 1 always yields an XNOR_{n-1} solution (100%, all levels).
- **Status:** PROVEN (exhaustive verification, all levels)
- **Demo provenance:** Demo 64
- **Significance:** The parity hierarchy is a legitimate matroid minor chain. Downward closure is perfect; upward extensibility is what collapses at the wall.

### Self-Doubling Impossibility
- **Statement:** Adding a 6th weight equal to any existing XOR5 weight always fails XOR6. Proof: scalar doubling preserves angle; class-1 values (odd sector) doubled remain class-1 (odd sector), making the pair output for weight-2 input class-1 when it must be class-0 for parity. This accounts for 34/38 = 89.5% of all closest misses at XOR6.
- **Status:** PROVEN (algebraic proof + exhaustive verification)
- **Demo provenance:** Demo 64
- **Significance:** First analytic characterization of a specific wall mechanism; explains the dominant failure mode at d=4

### Minimum Wall Distance d=4
- **Statement:** The closest any 6-weight candidate from the Z[zeta_8] catalog comes to satisfying XOR6 is 4 output errors (60/64 = 93.75% correct). No candidate achieves d=0, 1, 2, or 3. A second wall layer exists: d≥7 cases with 0 broken pairs but weight-3+ errors (GHZ-type higher-order obstruction).
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 64
- **Significance:** The XOR6 wall is not sharp in Hamming distance — there is a gap of at least 4. Self-doubling accounts for 89.5% of the closest misses.

### Funnel Rate Conjecture
- **Statement:** The coverage fractions for upward extensibility (XOR3→XOR2: 99.3%, XOR4→XOR3: 39.2%, XOR5→XOR4: 19.2%, XOR6→XOR5: 0%) follow some algebraic or exponential decay law as a function of n.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 64
- **Significance:** The 6-input wall casts a shadow all the way down: 80% of XOR4 weight sets are already dead ends for extension to XOR5. A closed-form decay rate would explain the shadow effect quantitatively.

### Clifford Staircase Conjecture
- **Statement:** XOR6 becomes achievable at Z[zeta_16] (next cyclotomic ring, fourth-level Clifford hierarchy), suggesting n-input parity climbs the Clifford hierarchy one rung per additional input.
- **Status:** PARTIALLY REFUTED by Demo 65 — standard (odd/even) sector activations: NO (XOR6 = 0 solutions at all k=2..31 with either the 36-value or 56-value Z[zeta_16] catalog). Generalized (arbitrary binary labeling of k sectors): YES at k=24. The two-layer wall separates a geometric obstruction (absolute for k≤23, both rings) from a labeling convention (solvable at k=24 with non-standard partition). The parity ceiling n=5 is itself algebra-independent (see below).
- **Demo provenance:** Demo 64 (conjecture), Demo 65 (partial refutation + two-layer analysis)
- **Significance:** Revealed the distinction between the geometric wall (k≤23 absolute) and the labeling convention (k≥24 generalized). The "Clifford staircase" framing is not quite right for standard activations.

### Parity Ceiling is Algebra-Independent
- **Statement:** The parity ceiling n=5 and tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) are identical at Z[zeta_8] and Z[zeta_16]. The "gap of 2" hypothesis between pigeonhole bound and actual ceiling is refuted: Z[zeta_8] has gap=2 (7−5), Z[zeta_16] has gap=9 (14−5). The ceiling is not set by the ring's pigeonhole bound.
- **Status:** PROVEN (exhaustive sweep k=2..31, both catalogs)
- **Demo provenance:** Demo 65
- **Significance:** The parity ceiling is a structural property of the computation, not of the specific cyclotomic evaluation ring. Conjectured to hold for all evaluation levels.

### Absolute Wall k≤23
- **Statement:** No binary partition of k sectors (for any k ≤ 23) can separate the XOR6 truth table using Z[zeta_8] catalog weights — regardless of whether standard (odd/even) or any other binary coloring of sectors is used.
- **Status:** PROVEN (exhaustive, 2700 checks per k=6,8,15,16..23)
- **Demo provenance:** Demo 65
- **Significance:** The geometric obstruction is absolute below k=24, independent of the labeling convention

### Generalized XOR6 at k=24
- **Statement:** At k=24, exactly 3 of 2700 candidate (XOR5-set, extension-weight) pairs admit a binary coloring of 24 sectors realizing XOR6. All 3 consist of exactly 3 antipodal pairs (w, −w) and land with exactly zero margin to sector boundaries (exact lattice alignment). The minimum even-k threshold is k=24.
- **Status:** PROVEN
- **Demo provenance:** Demo 65
- **Significance:** The absolute wall at k≤23 breaks at k=24 only with the antipodal-pair structure and non-standard coloring. The k=24 threshold matches the 24-cell vertex count (see Demo 66).

### Generalized XOR7 at k=91
- **Statement:** The first generalized XOR7 solution (with arbitrary binary sector labeling) appears at k=91. The winning tuple consists of 3 antipodal pairs + 1 lone weight at 90 degrees (purely imaginary). Scaling ratio XOR7/XOR6 minimum k ≈ 91/24 ≈ 3.8.
- **Status:** PROVEN
- **Demo provenance:** Demo 65
- **Significance:** Seeds a conjectured scaling law for higher-parity functions; the lone imaginary weight at 90 degrees may generalize.

### Zero Margin Universality
- **Statement:** All generalized XOR6 and XOR7 solutions at all tested k values have exactly zero margin to the nearest sector boundary, reflecting exact alignment of Z[zeta_8] lattice sums to lattice directions (multiples of pi/4).
- **Status:** PROVEN (all passing tuples at all tested k verified)
- **Demo provenance:** Demo 65
- **Significance:** Solutions are algebraically exact, not approximate. The lattice sum always lands exactly on a sector boundary for compatible k values.

### 24-Cell is the Braid Image
- **Statement:** SU(2) braid representations with generators σ₁ → (1+i)/√2 and σ₂ → (1−k)/√2 produce exactly the 24 vertices of the 24-cell (binary octahedral group) from all enumerated braids (87,890 braids; 4 axis-aligned, 12 edge, 8 body). The same braids produce 100 distinct Kauffman bracket values in Z[ζ₈]. Bracket and quaternion are complementary projections.
- **Status:** PROVEN (exhaustive enumeration)
- **Demo provenance:** Demo 66
- **Significance:** The 24-cell emerges as the natural computational geometry for DKC; the complex k=24 minimum from Demo 65 is a projection of these 24 vertices onto S¹ via the trace.

### Antipodal Necessity for Quaternionic XOR6
- **Statement:** XOR6 solutions on the quaternionic S³ exist only among antipodal weight pairs (q, −q). Full combinatorial search over all C(24,6) = 475,020 six-tuples unrestricted yields zero solutions at any grid resolution. Restricting to 3 antipodal pairs yields solutions (3 at 6×6, 34 at 8×8 geographic grid; 35 at 24-cell Voronoi). The constraint reflects the SU(2) → SO(3) double cover.
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 66
- **Significance:** The antipodal geometry seen in Demo 65 (all k=24 solutions are antipodal pairs) generalizes fully to the quaternionic setting.

### 24-Cell Voronoi Beats Geographic Grid
- **Statement:** The 24-cell Voronoi partition (25 cells: 24 vertices + 1 zero cell) yields 35 XOR6 solutions, versus 34 at a 64-cell geographic grid. The data-intrinsic partition outperforms the larger uniform grid.
- **Status:** PROVEN
- **Demo provenance:** Demo 66
- **Significance:** Confirms the 24-cell as the natural computational geometry; more cells does not always mean more solutions.

### Universal 7/7/11 Structure
- **Statement:** Every one of the 35 Voronoi XOR6 solutions on the 24-cell has exactly 7 cells labeled 0, 7 cells labeled 1, and 11 cells unused. The zero cell is always labeled 0. This is a geometric constraint: 3 antipodal pairs define 3 great circles on S³ and the 64 subset sums touch only 14 of 25 Voronoi cells.
- **Status:** PROVEN (all 35 verified)
- **Demo provenance:** Demo 66
- **Significance:** A universal structural fingerprint of DKC XOR6 solutions on S³; the 11 unused cells quantify the geometric "slack" of the 24-cell for this computation.

### F4 Decomposes to 6 Orbits
- **Statement:** Under the 576-element F4 Coxeter rotation subgroup (full two-sided symmetry group of the 24-cell), the 35 winning XOR6 tuples decompose into exactly 6 orbits (sizes 12, 7, 5, 5, 4, 2) in two complementary families: edge-dominant (orbits 1, 2, 5 — 22 solutions) and body-dominant (orbits 3, 4, 6 — 13 solutions).
- **Status:** PROVEN
- **Demo provenance:** Demo 66
- **Significance:** The F4 decomposition reveals two distinct computational strategies (edge-dominant vs. body-dominant); the orbit count 6 = 1+2+3 may reflect the partition structure of arity 6.

### Hopf Phase is Computationally Inert
- **Statement:** The Hopf fiber phase ξ₁ (the S¹ fiber of the Hopf fibration S³ → S²) carries zero DKC computational information for XOR6 at all resolutions tested (k=4 through k=32, exhaustive). Phase-only activation yields exactly 0 solutions. All DKC information lives in the S² Hopf base; the fiber is irrelevant.
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 67
- **Significance:** The Hopf fibration's fiber-base decomposition exactly separates the computationally inert (fiber) from the computationally essential (base). S² is the true computational arena, not S³.

### S² is the Computational Sweet Spot
- **Statement:** A custom 13-direction Voronoi on S² (eigenvector directions of the binary octahedral group) achieves 36 XOR6 solutions at only 14 cells (2.57 sol/cell), more than the full 25-cell S³ Voronoi (35 solutions). Platonic solid Voronois (octahedral=6, icosahedral=12, cuboctahedral=12 cells) yield zero solutions — natural polytopal geometry fails; data-derived directions are required.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 67
- **Significance:** Lower-dimensional projection (S²) outperforms the ambient space (S³). The computation is exactly as hard as the eigenvector directions dictate — not harder (Hopf inert), not easier (Platonic fails).

### Eigenvalue Paradox Resolution
- **Statement:** The eigenvalue angle θ = arccos(a) (depending on the real quaternion component, which combines base and fiber information) succeeds as a DKC activation (first solutions at k=8); the Hopf phase ξ₁ = atan2(b, a) (pure fiber) fails completely. These are distinct quantities: the real component is not the fiber alone but a combination.
- **Status:** PROVEN
- **Demo provenance:** Demo 67
- **Significance:** Resolves what initially seems paradoxical — both quantities involve the quaternion components but only one carries computation. The key is which combination of base and fiber.

### Intrinsic Curvature Theorem
- **Statement:** DKC computation on the eigenvector S² cannot be transferred to R² by stereographic projection when using Euclidean nearest-neighbor distance: Euclidean metric yields 0 solutions; S² great-circle metric yields 36 (identical to Demo 67). The computation is intrinsically curved. Area distortion up to 16.5× from stereographic projection does not affect solution count when using the S² metric — angles are sufficient, areas are irrelevant.
- **Status:** PROVEN (exhaustive over all 36 solutions)
- **Demo provenance:** Demo 68
- **Significance:** The plane is a window onto the computation, not a workspace. Establishes that the curvature of S² is essential to DKC, not merely a representational convenience.

### Conformality Sufficiency
- **Statement:** Stereographic projection is angle-preserving (conformal) but not area-preserving. Despite 16.5× area ratio between largest and smallest projected Voronoi cells, solution count and cell assignments are unchanged when using the S² metric. Conformality is sufficient; area preservation is not required.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 68
- **Significance:** Settles what geometric property of the map matters: angle (conformal), not area (equiareal). "Visualize on the plane, compute on the sphere."

### Direction Nesting Theorem
- **Statement:** For any N, all eigenvector directions in the ζ_N SU(2) braid catalog appear in the ζ_{2N} catalog. The nesting ζ_N ⊂ ζ_{2N} is exact on S². Algebraic proof: σ_i(2θ) = σ_i(θ)² as quaternions; squaring preserves the rotation axis; every ζ_N braid word is a valid doubled word in ζ_{2N}. Computationally confirmed for ζ_8 → ζ_16 at 13/13 directions, 0.000° drift.
- **Status:** PROVEN algebraically; COMPUTATIONALLY CONFIRMED for ζ_8 → ζ_16 (rigidity); strongly supported for ζ_8 → ζ_32 (8/13, drift attributed to cap artifact)
- **Demo provenance:** Demo 69
- **Significance:** The eigenvector directions are a universal nested structure across the Clifford hierarchy. ζ_8 is the coarsest root producing the 13-direction structure; finer roots add no new computational geometry at the same Voronoi resolution.

### Trivialization at High Direction Counts
- **Statement:** At ~3,457+ Voronoi cells (the natural ζ_16 Voronoi resolution), approximately 90% of all XOR6 triples become solutions — pigeonhole statistics dominate topology. For ζ_8 (14 cells) only 0.007% of triples succeed; for ζ_16 (3,458 cells) 90.6% succeed. The transition from topology-driven to pigeonhole-dominated computation lies somewhere between ~50 and ~500 cells.
- **Status:** DEMONSTRATED (ζ_8 and ζ_16 measured; threshold not precisely located)
- **Demo provenance:** Demo 69
- **Significance:** Explains why ζ_8 is the computational sweet spot: the coarsest root with the right structure, at a resolution where the computation is genuinely constrained by geometry rather than trivially satisfied.

### Yang-Baxter Equation Produces Identical Melodies
- **Statement:** The Yang-Baxter relation σ₁σ₂σ₁ = σ₂σ₁σ₂ produces not just equal final quaternion products, but identical step-by-step melodies (sequences of eigenvalue angles). The melody of a braid word depends only on the length of each partial product's trace, not on which specific generators are used. Pearson r between pitch (eigenvalue) and spatial direction (eigenvector) is −0.0624 over 253 pairs — music and space are orthogonal (r ≈ 0).
- **Status:** PROVEN (Yang-Baxter melody identity); CONFIRMED (r = −0.0624 for music-space independence)
- **Demo provenance:** Demo 70
- **Significance:** Establishes a direct correspondence between topological invariance (Yang-Baxter) and musical invariance (identical melody). Pitch and position are independent dimensions of a unit quaternion.

### Platonic Solids Fail for Quaternionic XOR6
- **Statement:** Dodecahedral (10 effective cells after antipodal collapse) and icosahedral (6 effective cells) Voronoi partitions yield zero XOR6 solutions. Only the data-intrinsic 13-direction eigenvector Voronoi gives 36 solutions. Platonic symmetry group mismatch (icosahedral order 120 vs binary octahedral order 48) is the obstruction.
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 70
- **Significance:** Confirms that the 13-direction S² Voronoi is not interchangeable with any natural polytopal geometry. The correct symmetry group is essential, not just the cell count.

### The 13=13 Theorem
- **Statement:** The minimum bandwidth for XOR6 DKC on S² is l=6. Phase transition is exact and sharp: 0% of XOR6 winners recovered at l≤5, 100% at l=6. The reason: mode l has 2l+1 independent spherical harmonic components, and 2×6+1=13 equals exactly the number of eigenvector directions. At l=5 there are 11 components trying to address 13 regions (underdetermined); at l=6 there are exactly 13 (determined). The transition is all-or-nothing at the degrees-of-freedom boundary.
- **Status:** PROVEN (computationally; analytical proof open)
- **Demo provenance:** Demo 71
- **Significance:** Gives a harmonic-analytic characterization of the 13-direction structure. The l=6 bandwidth is not a coincidence — it is forced by the DOF count of the eigenvector catalog.

### Spectral Universality
- **Statement:** All 36 XOR6 winning tuples share the same spectral envelope: l=6 dominant (~80.7% average, 70–84% range), l=4 secondary (~6%), l=12 tertiary (~6–7%). Mode l=2 varies (0.02%–17.6%, average 4.1%). All odd modes are exactly zero (antipodal symmetry). The spectrum is a structural invariant of XOR6 solutions.
- **Status:** DEMONSTRATED (all 36 winners)
- **Demo provenance:** Demo 71
- **Significance:** The spectral shape is universal, not solution-specific. The 80% figure for l=6 power is a property of XOR6 DKC on S², not of any particular winning tuple.

### Compressed Sensing on S²
- **Statement:** The 13-direction eigenvector Voronoi achieves XOR6 with only 14 cells where the Nyquist prediction for l=6 bandwidth requires (6+1)²=49 cells — a 3.5× super-Nyquist compression. This follows from compressed sensing: XOR6 is a binary function at 13 known positions, so sparsity plus known support eliminates the generic sampling requirement.
- **Status:** EXPLAINED (compressed sensing argument; not yet proven as a general theorem)
- **Demo provenance:** Demo 71
- **Significance:** Connects DKC to compressed sensing theory on S². The Voronoi cells need only distinguish 13 directions from each other, not generically sample an arbitrary l=6 function.

### Parity-Lock Theorem (±q Encoding)
- **Statement:** Under the ±q paired input encoding with k weights (2k input bits), the (0,0)≡(1,1) collision per weight creates 3^k equivalence classes. XOR/XNOR are the only standard Boolean functions constant on all classes. AND, OR, MAJ, and threshold functions are structurally impossible — zero winners at ALL depths (0-8) for both 6-input and 8-input arities. The mechanism: masks 0...0 and 1...1 always produce identical sums (both zero), so any f with f(0...0) ≠ f(1...1) is impossible under ANY activation. Sign-flip symmetry (combined_cell(S) = combined_cell(-S)) collapses 3^k classes to at most (3^k+1)/2 distinguishable groups.
- **Status:** **PROVEN** (formal proof in `proofs/parity-lock-theorem.md`; computationally verified: 0 winners for AND/OR/MAJ/THR2 at all depths 0-8, 76/76 collisions same-parity, 0/27 class conflicts for XOR, all 16 numerical tests confirm theorem, 13/13 sign-flip pairs verified identical)
- **Demo provenance:** Demo 92. **Proof:** `proofs/parity-lock-theorem.md` (P04)
- **Cross-references:** Parity Unreachability (Demo 48), Parity Reachability at k=6 (Demo 50), Depth Law (Demo 82/89/91)
- **Significance:** The ±q encoding concentrates all computational power on parity — the single Boolean function outside AC⁰. The depth law max_xor ≈ depth + 6 describes scaling for the ONLY achievable function family under this encoding. Contrasts with 1-weight-per-input encoding (Demos 48/50) which computes all 13 NPN classes.

### Encoding Concentrates on Hardest Function
- **Statement:** The ±q paired encoding channels all computational power into parity — the single Boolean function outside AC⁰ (constant-depth polynomial-size circuits). 83.7% of random 3-weight triples are XOR6-separable; AND/OR/MAJ separability is exactly 0.0%. This is not a limitation to be overcome but a structural feature.
- **Status:** DEMONSTRATED (10,000 random triple census)
- **Demo provenance:** Demo 92
- **Cross-references:** Parity-Lock Theorem (above), Furst-Saxe-Sipser/Hastad parity-not-in-AC⁰
- **Significance:** The encoding naturally selects for the computationally hardest Boolean function, providing a bridge between DKC and circuit complexity.

### Parity Wall Theorem (Activation Is Not the Bottleneck)
- **Statement:** The maximum achievable XOR arity at a given catalog depth is determined entirely by the parity constraint (whether any weight tuple separates even vs odd input subsets). Activation resolution (angular sectors k_sec = 2 to 48, Voronoi direction counts 2 to 114) controls the number of solutions but not their existence. max_xor = 8 at ALL k_sec values with 275 entries at ζ₁₂.
- **Status:** DEMONSTRATED (exhaustive k_sec sweep 2-48, Voronoi sweep 2-114 dirs)
- **Demo provenance:** Demo 91
- **Cross-references:** Depth Law (Demo 82), Balanced Exponentials Linearity (below), Axis Cancellation Mechanism (Demo 90)
- **Significance:** Confirms that the linear depth law is a property of the parity constraint and BFS vocabulary growth, not an activation geometry artifact.

### Depth Gap Invariance
- **Statement:** The depth gap between consecutive XOR levels (e.g., XOR6 first appearing at depth d₁ and XOR8 first appearing at depth d₂) is constant across all tested activation resolutions (k_sec = 2 to 48, direction counts 2 to 114). The depth law slope is fixed by parity, not activation geometry.
- **Status:** DEMONSTRATED (9×9 depth × k_sec interaction grid)
- **Demo provenance:** Demo 91
- **Cross-references:** Parity Wall Theorem (above), Balanced Exponentials Linearity (below)
- **Significance:** Key diagnostic: if activation were the bottleneck, finer resolution would shift the depth gap — it does not.

### Balanced Exponentials Linearity
- **Statement:** The linear depth law max_xor ≈ depth + 6 arises because BFS closure rounds add vocabulary at rate ~2×/round while parity demand grows at 4×/weight (2^{2N} masks). The ratio yields ~2 depth rounds per additional weight, producing the observed slope ≈ 1. Confirmed from three independent directions: D90 (supply side — vocabulary growth), D91 (demand side — activation invariance), and the quantitative match.
- **Status:** CONFIRMED (from both supply and demand sides)
- **Demo provenance:** Demo 89 (proposed), Demo 90 (supply analysis), Demo 91 (demand confirmation)
- **Cross-references:** Linear Depth Law (Demo 82), Parity Wall Theorem (Demo 91), Axis Cancellation Mechanism (Demo 90)
- **Significance:** Provides the mechanistic explanation for the linear depth law. The linearity emerges from the ratio of two exponential processes (BFS branching vs parity constraint growth).

### Trivialization DOF Prediction
- **Statement:** The minimum bandwidth scales as l ≥ (N−1)/2 where N is the number of eigenvector directions. For ζ_16 (N=3,457 directions) this predicts l ≥ 1728, with Nyquist cell count ~3M. At such enormous bandwidth any coarse partition trivially resolves the computation, explaining the ~90% solution rate seen in Demo 69 for the ζ_16 Voronoi.
- **Status:** CONJECTURED (not yet proven analytically)
- **Demo provenance:** Demo 71
- **Significance:** Provides a quantitative mechanism for trivialization — the DOF count predicts when topology-driven computation transitions to pigeonhole-dominated.

### BFS Growth is a Braid Group Invariant
- **Statement:** BFS growth profile (depth-by-depth entry counts and max coefficient magnitudes) is identical across all tested TL_n modules at delta=0 for the same n. At n=6: W_{6,0} (dim=5), W_{6,2} (dim=9), and W_{6,4} (dim=5, non-simple) all produce bit-for-bit identical BFS trees at every depth (1, 11, 77, 439, 2233, 10603, 32768). Growth rate (~5x converging for n=6) tracks strand count (n-1), not module dimension.
- **Status:** PROVEN (for n=6, three modules tested; extends sl_d Growth Rate from Demos 98-101)
- **Demo provenance:** Demo 102 (W_{6,0} vs W_{6,4}), Demo 103 (W_{6,0} vs W_{6,2}), Demo 104 (re-confirmed), Demo 105 (n=8 growth ~7x for W_{8,0})
- **Cross-references:** sl_d Growth Rate (Demos 98-101)
- **Significance:** Growth is intrinsic to the braid group B_n, not the module. ELIMINATES the dim(W) law hypothesis that higher-dimensional modules might produce faster growth.

### Curse of Dimensionality in Sign-Hash Activation
- **Statement:** Higher-dimensional TL modules produce WORSE XOR capacity at fixed k despite identical BFS catalogs. At k=128: W_{6,0} (dim=5, 100 sign components) beats W_{6,2} (dim=9, 324 sign components) at every XOR level (XOR6: 2449 vs 2238, XOR8: 850 vs 557, XOR10: 1 vs 0). More sign components create a sparser hash space, reducing collision-based XOR capacity.
- **Status:** CONFIRMED (D103); partially REFUTED at optimal component count by D104 (W_{6,2} beats W_{6,0} at 120 components)
- **Demo provenance:** Demo 103, Demo 104
- **Cross-references:** Atkinson Sweet Spot (below)
- **Significance:** Raw module dimension hurts DKC under naive full-hash activation; optimal component selection can recover and surpass lower-dimensional performance.

### Effective Dimension Hypothesis
- **Statement:** The key metric for DKC capacity is not raw dimension but effective_dim/dim^2 — action density. W_{6,0} (j=0) may have highest action density because all TL generators create/destroy arcs with no through-lines to preserve.
- **Status:** CONJECTURED
- **Demo provenance:** Demo 103
- **Cross-references:** Curse of Dimensionality (above), Branching Interaction Thesis (below)
- **Significance:** Would explain why j=0 modules consistently outperform at matched conditions.

### Atkinson Sweet Spot
- **Statement:** For W_{6,2} at k=128, optimal sign-hash component count is ~120 (of 324 total), producing XOR6=2486 which beats W_{6,0}'s maximum 2449. Named after Atkinson dithering — optimal information-per-collision tradeoff in sign-hash quantization. The peak is k-dependent: at higher k, more components become usable.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 104
- **Cross-references:** Curse of Dimensionality (above), k is the Real Lever (below)
- **Significance:** Non-monotonic component-count curve resembles optimal quantization in dithering: too little quantization loses signal, too much loses useful collision structure.

### Branching Interaction Thesis
- **Statement:** The computational signal in higher-dimensional TL modules lives in the cross-block (off-diagonal) components under TL_{n-1} branching restriction. For W_{6,2}: cross-block activation (160 comp) outperforms within-block (164 comp) and full (324 comp) at matched k=128. However, this reverses at n=8 (Macrame Principle, below).
- **Status:** CONFIRMED for n=6 (D104); REVERSED at n=8 (D105)
- **Demo provenance:** Demo 104 (confirmed), Demo 105 (reversed)
- **Cross-references:** Macrame Principle (below)
- **Significance:** Branching cross-terms between TL_{n-1} sub-pieces have low intra-block coherence at n=6, making them informationally rich.

### Sign-Rank Expansion
- **Statement:** Sign quantization (ternary {-1,0,+1} projection) can INCREASE effective rank beyond raw Z-linear rank. W_{6,2}: raw rank 244/324, sign-rank 292/324 — a gain of 48 independent directions. W_{6,0}: raw rank = sign-rank = 100 (already maximally independent). The nonlinear sign() function breaks linear dependencies that exist over Z.
- **Status:** NEW FINDING (D104); scaling confirmed at D105 (W_{8,2} at 1.83x, W_{8,0} at 1.08x)
- **Demo provenance:** Demo 104, Demo 105
- **Cross-references:** Sign-Rank Expansion Scaling (below)
- **Significance:** Sign quantization is not just lossy compression — it is an information EXPANDER for modules with Z-linear dependencies.

### k is the Real Lever
- **Statement:** Multi-hash architecture does not improve XOR capacity. Increasing k (cell count) is the primary scaling lever. The cross-block advantage at k=128 vanishes at k=4096 — W_{6,0} wins at every k in the sweep. The D103 conclusion that W_{6,2} is fundamentally inferior is partially refuted (it can beat W_{6,0} at optimal component count), but W_{6,0} wins at all k.
- **Status:** DEMONSTRATED (D104 k-sweep k={128, 256, 512, 1024, 2048, 4096})
- **Demo provenance:** Demo 104
- **Cross-references:** Atkinson Sweet Spot (above), k-Regime Theory (below)
- **Significance:** Separates hash architecture from activation granularity as scaling levers — only k matters.

### k-Regime Theory
- **Statement:** The ratio k/2^N controls XOR capacity: pigeonhole regime (k/2^N << 1) kills XOR, collision avoidance regime (k/2^N >> 1) revives it. D93's "XOR dies at N>=7" is a k-regime artifact — at k=4096, W_{8,0} produces XOR8=22266 (n=8 beats n=6's 21699 at matched k). Confirmed across n=6 and n=8.
- **Status:** CONFIRMED (at n=8, D105; revives XOR at high k)
- **Demo provenance:** Demo 105
- **Cross-references:** k is the Real Lever (above), D93 "XOR dies at N>=7" (REFUTED)
- **Significance:** The k/2^N ratio is the real control parameter, not absolute k or absolute N. All prior "XOR dies" claims were pigeonhole artifacts of low k.

### Macrame Principle
- **Statement:** Branching interaction (cross-block) dominates when individual sub-module blocks are small/simple (dim 4-5, as in W_{6,2}). Within-block dominates when blocks are complex (dim 14, as in W_{8,2}). Named by analogy: simple knots need connections for computational fabric; complex knots are self-sufficient. Crossover regime between dim 5 and dim 14.
- **Status:** CONJECTURED (two data points: D104 n=6, D105 n=8)
- **Demo provenance:** Demo 105
- **Cross-references:** Branching Interaction Thesis (above)
- **Significance:** Regime transition in optimal activation design based on sub-module block complexity.

### Sign-Rank Expansion Scaling
- **Statement:** Sign expansion ratio increases with module dimension and decreases with oversampling. W_{8,2} at 1.83x (still growing at 16384 entries) vs W_{8,0} at 1.08x (near saturation at 32768 entries). Larger modules with more Z-linear dependencies show more dramatic expansion.
- **Status:** CONJECTURED (supported by D104-D105 data across 6 modules)
- **Demo provenance:** Demo 105
- **Cross-references:** Sign-Rank Expansion (above)
- **Significance:** Would predict that still-larger modules (W_{10,2} etc.) show even greater sign-rank expansion.

### Rank Saturation Methodology
- **Statement:** Single-checkpoint rank measurements are unreliable for infinite braid groups. Must report saturation curves at multiple checkpoints. All prior cross-module rank comparisons (D99b through D104) are confounded by varying oversampling ratios.
- **Status:** ESTABLISHED (D105 retroactive analysis)
- **Demo provenance:** Demo 105
- **Significance:** Methodological correction — honest rank measurement requires saturation curves, not single snapshots.

### TL Visibility Filter
- **Statement:** Properties visible in the Kauffman bracket (writhe) correlate with DKC output; properties invisible in the bracket (topological entropy, Burau spectral radius) do not. The TL quotient at delta=0 is the filter: e_i^2=0 kills expanding eigenvalues, erasing all dynamical information while preserving algebraic structure. Both periodic and pseudo-Anosov braids compute XOR at 100% participation with near-identical scores.
- **Status:** DEMONSTRATED (D106: prediction that entropy predicts DKC was FALSIFIED; total Boolean function null across all 8 functions tested)
- **Demo provenance:** Demo 106
- **Cross-references:** Relational Computation Thesis (below)
- **Significance:** The TL quotient at delta=0 defines precisely what the bracket can and cannot see. Dynamical properties (chaos, mixing) are invisible; algebraic properties (writhe, algebraic coherence) are visible.

### Relational Computation Thesis (Strengthened)
- **Statement:** Across the DKC program, the strongest predictors of computational capacity are always relational/algebraic (derived series strata, cross-stratum mixing, orthogonal frame geometry, k-granularity, non-semisimplicity). Individual dynamical properties (entropy, spectral radius, writhe, word length) are consistently non-predictive or invisible. D106 sharpens this: the question is never "is this braid good?" but "does this collection span the right algebraic structure?"
- **Status:** STRENGTHENED (D106 adds entropy null to the evidence chain)
- **Demo provenance:** Demo 106 (entropy null), Demos 89-90-91 (depth law mechanism), Demo 95 (cross-layer synergy)
- **Significance:** Central organizing principle of DKC: computation is collective and relational, not individual and dynamical.

### Z/4Z Axis-Alignment Theorem
- **Statement:** Every entry of a braid representation matrix over a TL module at delta=0 is axis-aligned: exactly one nonzero cyclotomic component (each entry = n*zeta_8^k for integer n and k in {0,1,2,3}). Proof chain: (1) TL generators have {0,1} entries, (2) braid generators give entries from {0, A, A^{-1}}, (3) axis-aligned * axis-aligned = axis-aligned, (4) same-axis addition preserves axis, (5) bipartiteness ensures only same-axis values are added at each (r,c).
- **Status:** PROVEN (constructive proof + verified for ALL 186 distinct values from 32768-entry W_{4,2} catalog and all TL_n left regular modules for n=2..6)
- **Demo provenance:** Demo 107
- **Cross-references:** Axiality Theorem (Section 1), Nesting Parity Z/2Z Grading (below)
- **Significance:** Axis-alignment at the matrix-entry level is stronger than the bracket-level axiality theorem (Section 1). The constructive Z/4Z phase formula Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4} gives three numbers that fully determine the cyclotomic phase.

### Nesting Parity Z/2Z Grading
- **Statement:** The bipartite 2-coloring of TL link-state transition graphs IS nesting count mod 2 (a nesting = pair of arcs (a,b),(c,d) with a < c < d < b). Verified for ALL n=2,3,4,5,6 (195 matchings, 0 mismatches). Confirmed by researcher as NOT a consequence of known KLR Z-grading (Plaza-Ryom-Hansen 2013).
- **Status:** PROVEN (computationally, n=2..6)
- **Demo provenance:** Demo 107
- **Cross-references:** Z/4Z Axis-Alignment Theorem (above), Major Index Bipartite Invariant (below)
- **Significance:** NOVEL. The nesting parity is a new algebraic invariant of TL link states that governs the phase structure of braid matrices.

### Major Index Bipartite Invariant
- **Statement:** For TL standard modules W_{n,j}, the bipartite invariant is maj mod 2 = sum of right-endpoint positions of arcs, mod 2. Verified: W_{4,2} (3/3 match), W_{6,2} (9/9 match, the ONLY winning candidate out of 4 tested). For full matchings (j=0), maj mod 2 = nest mod 2 (special case).
- **Status:** CONJECTURED for general n,j (verified at W_{4,2} and W_{6,2})
- **Demo provenance:** Demo 107
- **Cross-references:** Nesting Parity Z/2Z Grading (above)
- **Significance:** NOVEL. Generalizes nesting parity from j=0 modules to arbitrary through-strand count j.

### q-Catalan Identity
- **Statement:** C_{2m+1}(-1) = (-1)^m * C_m. Gives exact bipartition class sizes for W_{n,0} modules (full-matching TL link-state graphs).
- **Status:** PROVEN (algebraically; verified against all 5 data points n=2,3,4,5,6)
- **Demo provenance:** Demo 107
- **Significance:** NOVEL identity relating q-Catalan specializations to ordinary Catalan numbers.

### Orthogonality Theorem (Multiplicative vs Additive Closure)
- **Statement:** For the 186-value Z[zeta_8] set from W_{4,2}: multiplicative quotient graph (modulo axis) = K_5 (Z/4Z multiplication table, complete 4-regular); additive quotient graph (modulo axis) = star tree centered on 0 (axis-preserving). Multiplication = Z/4Z rotation across axes; addition = growth within each axis.
- **Status:** PROVEN (computationally)
- **Demo provenance:** Demo 107
- **Cross-references:** Dual-Channel Theorem (Demo 108), Z/4Z Axis-Alignment Theorem (above)
- **Significance:** Establishes the fundamental orthogonality between multiplicative (phase-rotating) and additive (magnitude-growing) algebraic operations on Z[zeta_8] values.

### 2-Adic Neutrality of Nilpotent TL
- **Statement:** The 2-adic valuation distribution of TL matrix entries at delta=0 decays geometrically (95->48->24->11->7, approximately halving per level). TL nilpotency (e_i^2=0) introduces factors of 2 symmetrically.
- **Status:** CONJECTURED (five candidate proof frameworks identified but not formalized)
- **Demo provenance:** Demo 107
- **Significance:** NOVEL observation on the p-adic structure of TL algebras at delta=0.

### Dual-Channel Theorem
- **Statement:** Parity capability requires BOTH multiplicative phase coherence (product closure > 0) AND additive magnitude diversity (v_2 connectivity > trivial). Poison fails on BOTH simultaneously. Maximum structural contrast: parity has 17 product closure edges (connected, P_3 path structure) vs poison 0 edges (empty graph). Parity connects 4/5 v_2 classes (83% density) vs poison connecting 2/6 (trivial K_2). Maps onto T-gate (phase) / Hadamard (magnitude) decomposition in quantum circuit synthesis.
- **Status:** NOVEL (confirmed at delta=0, D108; refined with encoding-dependence at delta=sqrt(2), D109)
- **Demo provenance:** Demo 108
- **Cross-references:** Orthogonality Theorem (Demo 107), Encoding-Dependent Dual-Channel Theorem (below), Amy Bridge (below)
- **Significance:** CENTRAL CLAIM. Graph structure predicts parity capability. The two channels measure genuinely different algebraic properties (phase rotation vs magnitude reach) and both are independently necessary.

### Product Closure as Parity Discriminator
- **Statement:** Connected product closure graph is necessary for parity capability. Maximum structural contrast at delta=0: 17 edges (connected) vs 0 edges (empty) for poison values.
- **Status:** NOVEL, CONFIRMED (D108)
- **Demo provenance:** Demo 108
- **Cross-references:** Dual-Channel Theorem (above)
- **Significance:** Product closure is the single strongest algebraic discriminator between parity-capable and parity-incapable values.

### Partition Coincidence as Incapacity Signature
- **Statement:** Triple partition collapse (root = Galois = norm, all yielding the same partition) is a structural signature of computational incapacity. Parity values have three distinct partition levels (root/norm at 21 groups, Galois at 37, v_2 at 5); poison values collapse to 15 groups on all three.
- **Status:** NOVEL, CONFIRMED (D108)
- **Demo provenance:** Demo 108
- **Cross-references:** Dual-Channel Theorem (above)
- **Significance:** Zero internal algebraic differentiation (partition coincidence) correlates with zero computational utility.

### Resolution Dependence of Computational Role
- **Statement:** Same Z[zeta_8] value can be parity-capable at one sector resolution and poison at another. 14 values are simultaneously parity-capable (k=6) and D64-poison (k=15). Computational role is not intrinsic to the value but depends on sector resolution.
- **Status:** NOVEL, CONFIRMED (D108)
- **Demo provenance:** Demo 108
- **Cross-references:** Dual-Channel Theorem (above)
- **Significance:** Computational role is activation-dependent, not value-intrinsic.

### Amy Bridge: T-gate/Hadamard Decomposition
- **Statement:** DKC's dual-channel structure is the analog of T-gate/Hadamard decomposition in quantum circuit synthesis (Amy, Glaudell & Ross 2023). Product closure = T-gate (phase rotation, sde resource). Activation function provides Hadamard-equivalent magnitude diversity enabling sde=0 ("Clifford") values to compute parity.
- **Status:** CONJECTURED
- **Demo provenance:** Demo 108
- **Cross-references:** Dual-Channel Theorem (above)
- **Significance:** Would connect DKC to quantum circuit synthesis resource theory.

### Encoding-Dependent Dual-Channel Theorem
- **Statement:** The polarity of the multiplicative channel in the Dual-Channel Theorem is encoding-dependent. Under additive encoding (delta=0, D108), parity wants HIGH product closure (self-reinforcement through superposition). Under multiplicative encoding (delta=sqrt(2), D109), parity wants LOW product closure (products must escape vocabulary for sector alternation). The dual-channel theorem holds in both regimes but the sign of the multiplicative discriminator flips with encoding type. CONFIRMED by D108+D109 paired experiment.
- **Status:** NOVEL, CONFIRMED (D108+D109 paired experiment)
- **Demo provenance:** Demo 109 (refines D108)
- **Cross-references:** Dual-Channel Theorem (above)
- **Significance:** Encoding type determines whether product closure is a positive or negative predictor of parity capability.

### Perfect Algebraic Symmetry at delta=sqrt(2)
- **Statement:** At delta=sqrt(2) with Re>0 multiplicative encoding, parity and non-parity vocabularies are structurally indistinguishable on 7 of 8 edge types and all classification metrics (axis distribution, norm classes, v_2 classes, Galois orbits all identical in both sets). Product closure is the sole discriminator (parity: 56 edges/14.8%, non-parity: 176 edges/46.6%). The 28/28 = 50% parity/non-parity split is perfectly symmetric.
- **Status:** NOVEL, DEMONSTRATED (D109)
- **Demo provenance:** Demo 109
- **Cross-references:** Encoding-Dependent Dual-Channel Theorem (above)
- **Significance:** The Z[zeta_16] algebra at delta!=0 is so rich (j=0 sector alive) that Raqiya detectors cannot distinguish parity from non-parity except through product closure.

### j=0 Liveness implies Structural Universality
- **Statement:** At delta=0, j=0 sector dead implies impoverished algebra implies structural asymmetry detectable by Raqiya. At delta!=0, j=0 sector alive implies rich algebra implies structural universality (parity/non-parity indistinguishable on most metrics). Raqiya diagnoses algebraic impoverishment, not algebraic health.
- **Status:** CONJECTURED (extends D52; supported by D108 vs D109 contrast)
- **Demo provenance:** Demo 109
- **Cross-references:** Sector-0 Death at delta=0 (Section 2), Perfect Algebraic Symmetry (above)
- **Significance:** Would explain why Raqiya is maximally discriminating at delta=0 and minimally discriminating at delta!=0.

### Re Bias as Activation Signature
- **Statement:** At delta=sqrt(2) with Re>0 multiplicative encoding, parity values have Re<=0 bias (26/28 with a<=0), non-parity have Re>=0 bias (24/28 with a>=0). The Re>0 activation selects values whose products must cross from negative to positive real half-plane (self-escape behavior), correlating with low product closure.
- **Status:** NOVEL, DEMONSTRATED (D109)
- **Demo provenance:** Demo 109
- **Significance:** The activation function creates a selection pressure that is visible in the Gaussian real-part distribution.

### Caruso Genericity
- **Statement:** Pseudo-Anosov braids become generic at higher word lengths, crossing from minority to majority at depth 5 (50.6%) and reaching 77.2% at depth 8. Consistent with Caruso (2013) genericity theorem.
- **Status:** CONFIRMED (D106 depth-by-depth pA fraction data)
- **Demo provenance:** Demo 106
- **Significance:** Empirical confirmation of a known theoretical result, in the specific context of 4-strand braids.

---

## 4. Bracket Properties

### Entanglement Vanishing Theorem
- **Statement:** At delta=0, ALL non-interleaving braids (using only far-apart generators, e.g., sigma_1 and sigma_3 only in B_4) have bracket = 0. Delta=0 is an entanglement detector.
- **Status:** PROVEN (exhaustive for n=4, length 1-5; 1024 braids ALL zero)
- **Demo provenance:** Demo 35
- **Significance:** Only braids where all strands participate in a single connected topological component can have nonzero bracket. At length 5, 40.1% of interleaving braids are nonzero.

### Entanglement Vanishing Breakdown at delta!=0
- **Statement:** The entanglement vanishing theorem breaks at non-zero delta. At delta=1, non-interleaving braids CAN have nonzero bracket because delta^L = 1 means closed loops don't kill states.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 53
- **Significance:** Delta=0 is uniquely special for entanglement detection

### Zero Structure of Torus Knots
- **Statement:** Zero counts of |bracket(e^{i*theta})| for torus knots T(2,n) partition into {0, 4, 8} only. Pattern for n=3,5,7,9,11,13 is 8,0,0,8,4,4. All zeros occur at universal angles: multiples of pi/4 (delta zeros) and pi/8.
- **Status:** COMPUTATIONALLY VERIFIED (1024 samples, bisection refinement)
- **Demo provenance:** Demo 14
- **Significance:** Zero count is NOT monotonic with crossing number — cyclic pattern

### Chiral Zero Reflection
- **Statement:** Chiral knot pairs have perfectly reflected zero positions: theta <-> 2pi-theta. All pairs have 100% matched positions.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 14
- **Significance:** Zero positions carry chirality information

### Chiral Amplitude Invariance
- **Statement:** |bracket(A)| = |bracket(A^{-1})| on the unit circle. Chiral pairs share identical amplitude waveforms — only phase distinguishes them.
- **Status:** COMPUTATIONALLY VERIFIED (256-sample waveforms, 10 knots)
- **Demo provenance:** Demo 11
- **Significance:** Born-rule projection destroys chirality information

### Amphichiral Bracket Symmetry
- **Statement:** Amphichiral knots (figure-eight) have amplitude symmetric about theta=0: |f(theta)| = |f(-theta)|.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 11
- **Significance:** Detectable symmetry signature

### Mirror Bracket Relation
- **Statement:** bracket_mirror(e^{i*theta}) = bracket(e^{-i*theta}). The mirror bracket is the time-reversed waveform.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 11
- **Significance:** Links chirality to complex conjugation

### Braid Concatenation != Diagram Connected Sum
- **Statement:** bracket(K1#K2) = bracket(K1)*bracket(K2)/delta does NOT hold for braid concatenation (error ~6.35 for tested cases). Braid concatenation is structurally different from diagram connected sum.
- **Status:** PROVEN (tested numerically)
- **Demo provenance:** Demo 14
- **Significance:** Rules out naive factorization approach for composites

### Bracket Non-Multiplicativity (Trace Closure)
- **Statement:** bracket(b1*b2) != bracket(b1)*bracket(b2) for >= 80% of tested braid pairs under trace closure. But all composed values remain in Z[zeta_8] by construction.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 29
- **Significance:** Trace closure breaks multiplicativity

### Quantum Trace Formula
- **Statement:** bracket(beta) = tr_q(rho(beta)) / (A^2 + A^{-2}), where tr_q = tr(K^{tensor n} * M) with K = diag(A^2, A^{-2}). Generalizes with (-1)^n sign factor: tr_q = (-1)^n * delta * bracket.
- **Status:** PROVEN (computationally verified n=2 through 5)
- **Demo provenance:** Demo 04, Demo 05
- **Significance:** Unifies separate standard-module traces into single computation

### Delta Error Landscape
- **Statement:** |delta| = 2|cos(2*theta)| universally predicts gate viability. Delta-zero angles (8th roots of unity) are exactly where braid circuits compute exact Boolean logic.
- **Status:** PROVEN (analytical formula verified at 360 angles)
- **Demo provenance:** Demo 20
- **Significance:** |delta| cleanly separates universal-computation angles from NOT-only angles

### Compression Scaling
- **Statement:** Distinct TL_n matrices grow strictly slower than the number of braids (subexponential compression). n=4 compression exceeds n=3 at the same length (3.6x vs 3.2x at length 4).
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demos 22, 35
- **Significance:** TL_n is optimal lossless compression of braid group for bracket evaluation

### Sparsity Formula
- **Statement:** TL generator e_1 at n strands has C_n - C_{n-1} nonzero entries. Verified at n=2(1), n=3(3), n=4(9), n=5(28), n=8(1001). Density converges to ~70%.
- **Status:** CONJECTURED (verified n=2-5, n=8)
- **Demo provenance:** Demo 35, Demo 38 (n=8 data: 1001 of 1430 entries nonzero)
- **Significance:** Basis elements with top-top arcs at generator position create loops; the rest survive

### Parity Barrier in Braid Computation
- **Statement:** The parity function (XOR of all 3 inputs, truth tables 0x96/0x69) is found ZERO times across all braids (lengths 1-4) and all 4 closure types (1554 braids). Parity is unreachable via bracket amplitudes with closure thresholding.
- **Status:** COMPUTATIONALLY VERIFIED (exhaustive)
- **Demo provenance:** Demo 25
- **Significance:** Fundamental limitation of bracket amplitude thresholding

### Universal Full Rank (Closure Measurement)
- **Statement:** ALL 1554 4-strand braids (lengths 1-4) have measurement matrix rank 4 — the 4 closure types are always linearly independent as amplitude vectors over 8 inputs.
- **Status:** COMPUTATIONALLY VERIFIED (exhaustive)
- **Demo provenance:** Demo 25
- **Significance:** 4 closures always provide maximal information about a braid

### Convergence at WRT Angles
- **Statement:** Greedy-selected optimal angle (1.805*pi) independently converges to within 1.5% of the WRT level-3 evaluation point (11*pi/6). Algebraically special angles ARE informationally optimal angles.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 15
- **Significance:** "Topology knows information theory"

---

## 5. Algebraic Structure

### Idempotent Products in Nilpotent Algebra
- **Statement:** Despite TL generators being nilpotent (e_i^2 = 0 at delta=0), products of ADJACENT generators are idempotent: (e_i * e_{i+1})^2 = e_i * e_{i+1}. The augmentation ideal J = span{all non-identity elements} satisfies J^2 = J; J is NOT nilpotent.
- **Status:** PROVEN (algebraic: (e_1*e_2)^2 = e_1*(e_2*e_1*e_2) = e_1*e_2 by TL relation)
- **Demo provenance:** Demo 35, explorer's log
- **Significance:** The radical is a PROPER subideal of the augmentation ideal — smaller and more subtle than expected

### Peirce Decomposition Structure
- **Statement:** Every TL_n(0) for n >= 3 admits exactly 3 orthogonal idempotents summing to 1 (from generator-product idempotents). Peirce blocks exhibit a consistent 2+1 structure.
- **Status:** COMPUTATIONALLY VERIFIED (n=3-8)
- **Demo provenance:** Demo 51
- **Significance:** Structural decomposition confirmed computationally

### Fibonacci Rank Theorem
- **Statement:** The total bilinear rank of the rad^2 generator (sum of ranks across through-strand sectors) equals F(ell-1), the (ell-1)th Fibonacci number. ell=3→1=F(2), ell=4→2=F(3), ell=5→3=F(4), ell=6→5=F(5), ell=7→8=F(6).
- **Status:** COMPUTATIONALLY VERIFIED (ell=3,4,5,6,7 — including first cubic number field case)
- **Demo provenance:** Explorer's log (Demo 52 session), Demo 60 (ell=7 confirmation)
- **Significance:** NOVEL. Fibonacci numbers arise from TL tower fusion rules (length-2 paths in Bratteli diagram). Deepest insight: entanglement rank of thermal ground state = anyonic computational power. ell=7 sector decomposition: sector 1 (rank 3) + sector 3 (rank 4) + sector 5 (rank 1) + sector 7 (rank 0) = 8 = F(6).

### Palindromic Sector Rank Conjecture (Even ell)
- **Statement:** For even ell, the sector rank profile of the rad^2 generator is palindromic: ell=4→[1,1], ell=6→[1,3,1].
- **Status:** CONJECTURED (verified ell=4,6)
- **Demo provenance:** Explorer's log
- **Significance:** Structural symmetry in through-strand decomposition

### TL Relations at delta=0 (Nilpotency)
- **Statement:** At delta=0, TL generators satisfy: e_i^2 = 0 (nilpotent), e_i*e_{i+1}*e_i = e_i (adjacent), e_i*e_j = e_j*e_i for |i-j|>=2 (far commutativity).
- **Status:** PROVEN (verified n=2,3,4,5)
- **Demo provenance:** Demo 35
- **Significance:** Foundation for all TL computations at delta=0

### TL Relations at delta=1 (Idempotency)
- **Statement:** At delta=1, TL generators satisfy e_i^2 = e_i (idempotent). Adjacent and far commutativity relations hold.
- **Status:** PROVEN (verified n=2-8)
- **Demo provenance:** Demo 52, Demo 53
- **Significance:** Qualitatively different algebraic regime — every smoothing state contributes

### Cyclotomic Bracket Values
- **Statement:** Bracket values at A = e^{i*5pi/4} are cyclotomic integers in Z[zeta_8] (coordinates are multiples of 1/sqrt(2) or integers). Z[zeta_8] has class number 1 (UFD).
- **Status:** PROVEN (exact Cyc8 arithmetic confirms)
- **Demo provenance:** Demo 27, Demo 29
- **Significance:** Exact computation is possible — no floating point needed

### Delta=0 Simplification
- **Statement:** At A = -zeta_8, delta = -(A^2 + A^{-2}) = 0 exactly. This reduces the bracket state sum to single-loop states only, massively simplifying computation.
- **Status:** PROVEN (exact algebraic identity)
- **Demo provenance:** Demo 29
- **Significance:** The key simplification that makes forward DKC feasible

### Jones-Wenzl Scaling Trick
- **Statement:** Jones-Wenzl recursion stays polynomial via scaling: Q_n = D_n * p_n avoids rational function entries. Key simplification: [2]_q / delta = -1 keeps n=3 recursion in Z[A, A^{-1}].
- **Status:** PROVEN
- **Demo provenance:** Demo 06
- **Significance:** Enables polynomial computation of JW idempotents

### Quantum Dimension Recurrence
- **Statement:** [n+1]_q = [2]_q * [n]_q - [n-1]_q, and [n]_q * (A^2 - A^{-2}) = A^{2n} - A^{-2n}.
- **Status:** PROVEN (verified n=1..6)
- **Demo provenance:** Demo 05
- **Significance:** Chebyshev recurrence for quantum dimensions

### Neglecton Bracket Values
- **Statement:** First-order perturbation around delta=0 yields "neglecton" bracket values from loops=2 states with scale factor 4 = d(delta)/d(epsilon). 148 new Z[zeta_8] values from 15,242 previously invisible braids. But ALL 116 genuinely new values share angles with existing standard values — no new angular directions.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 49
- **Significance:** Perturbation approach is CLOSED at all orders — higher-order terms also preserve the same angular structure

### FK = Ising Equivalence
- **Statement:** The Fortuin-Kasteleyn partition function Z_FK(G; Q=2, v) equals the Ising spin sum Z_Ising(G; v) for all lattice strips tested.
- **Status:** PROVEN (exhaustive subset enumeration, 7 cases)
- **Demo provenance:** Demo 56
- **Significance:** Cross-validates the Potts-TL dictionary

### FK Axiality Breaking
- **Statement:** FK partition function values use all 4 dimensions of Z[zeta_8] where bracket values use only 2 (Z[i]-axial). The Q^{k(A)} weighting breaks axiality.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 56
- **Significance:** FK values are algebraically richer than bracket values

### Fibonacci-Delta Connection
- **Statement:** At ell=5, delta = phi = golden ratio. Delta powers are literal Fibonacci numbers: phi^k = F(k-1) + F(k)*phi. The Fibonacci anyon name is literal, not just categorical.
- **Status:** PROVEN (exact Cyc5 arithmetic)
- **Demo provenance:** Demo 58
- **Significance:** Deep structural connection between Fibonacci sequence and TL algebra at ell=5

---

## 6. Cross-ell Universality

### Cross-ell Radical Dimension Formulas
- **Statement:** At the first non-semisimple n (n=ell): rad(TL_ell(2cos(pi/ell))) = 2*ell - 3 for all ell >= 2. At the second non-semisimple n (n=ell+1): rad(TL_{ell+1}) = ell^2 - ell - 3 for all ell >= 3.
- **Status:** **PROVEN** (formal proofs: 2ℓ−3 in `proofs/radical-dimension-formula.md`, ℓ²−ℓ−3 in `proofs/next-level-radical-formula.md`). Method: Graham-Lehrer cellular algebra theory — V_{ℓ-2} is the unique degenerate cell module at n=ℓ with corank 1 via Chebyshev determinant U_{ℓ-1}(cos(π/ℓ))=0. Contribution: 1·(2(ℓ-1)−1) = 2ℓ−3. The ℓ²−ℓ−3 formula has its own independent proof. Computationally verified at ell=2..7.
- **Demo provenance:** Demo 52, Demo 60 (ell=7), explorer's log. **Proofs:** `proofs/radical-dimension-formula.md` (2ℓ−3), `proofs/next-level-radical-formula.md` (ℓ²−ℓ−3)
- **Significance:** NOVEL. First formal proofs of these formulas. The proof identifies V_{ℓ-2} as the unique degenerate cell module — a structural result stronger than the formula alone.

### Markov RT Truncation Theorem
- **Statement:** For TL_n(2cos(π/ℓ)) with n ≥ ℓ ≥ 3: dim ker(B_M) = dim J(A) + Σ_{j ≥ ℓ-1, j ≡ n mod 2} (dim L_j)². The Markov trace vanishes on the negligible ideal (generated by Jones-Wenzl JW_{ℓ-1}) and kills exactly the simple blocks with j ≥ ℓ-1, preserving those with j ≤ ℓ-2.
- **Status:** **PROVEN** (formal proof in `proofs/markov-rt-truncation.md`). Three-part proof: (A) Markov vanishes on negligible ideal since [ℓ]=0. (B) Negligible ideal kills j ≥ ℓ-1 by through-strand argument. (C) Excess = Σ (dim L_j)² from combining.
- **Demo provenance:** Demo 39 Parts I-J (discovery and verification, 31/31 data points). **Proof:** `proofs/markov-rt-truncation.md`
- **Significance:** NOVEL. Identifies the Markov kernel as J(A) + negligible ideal — connecting fixpt trace (algebraic light) to Markov trace (topological light). The excess = what topology cannot see that algebra can.

### Universal Corank 1 at First Degeneracy
- **Statement:** At the first n where any cell module V_j degenerates, its corank is always 1. This holds because the linked module is V_n^{(n)} (dim 1).
- **Status:** **PROVEN** (structural, via Graham-Lehrer linking theorem)
- **Demo provenance:** Demo 39 Part H, `proofs/next-level-radical-formula.md`
- **Significance:** Explains why rad contributions at first degeneracy have the uniform shape 1·(2j-1)

### Second-Degeneracy Corank = ℓ+1
- **Statement:** At n=ℓ+2, the cell module V_{ℓ-2} degenerates for the second time with corank ℓ+1. Values: ℓ=3→4, ℓ=4→5, ℓ=5→6, ℓ=6→7.
- **Status:** COMPUTATIONALLY VERIFIED (ℓ=3..6)
- **Demo provenance:** Demo 39 Part H, `proofs/next-level-radical-formula.md`
- **Significance:** No clean closed form for rad at arbitrary n=ℓ+m

### Universal rad^2 = 1 at First Non-Semisimple Pair
- **Statement:** rad^2 = 1 at n=ell and n=ell+1 for all ell >= 3 tested.
- **Status:** COMPUTATIONALLY VERIFIED (ell=3,4,5,6,7 — including first cubic number field case)
- **Demo provenance:** Demo 52, Demo 60 (ell=7), explorer's log
- **Significance:** NOVEL. The one-dimensional rad^2 has revealing tensor product structure. Confirmed at ell=7 for both n=7 and n=8.

### Gram Rank at n=ell
- **Statement:** Gram rank of TL_ell at delta=2cos(pi/ell) equals C_ell - (2*ell - 3).
- **Status:** **PROVEN** (immediate corollary of the radical dimension formula, now formally proved)
- **Demo provenance:** Demo 52, Demo 60 (ell=7). Proof: corollary of `proofs/radical-dimension-formula.md`
- **Significance:** Immediate corollary of radical dimension formula

### Semisimplicity Criterion
- **Statement:** TL_n(2cos(pi/ell)) is semisimple iff n < ell.
- **Status:** COMPUTATIONALLY VERIFIED (ell=2,3,4,5,6,7 — six evaluation points, including cubic field at ell=7)
- **Demo provenance:** Demo 52, Demo 39 (n=ell boundary data), Demo 60 (ell=7: semisimple for n=2..6, non-semisimple at n=7)
- **Significance:** Known from general theory; explicitly confirmed across 6 evaluation points including first cubic number field

### Identity-Zero Property
- **Statement:** At n=ell, the rad^2 generator has zero coefficient on exactly the identity element and nonzero on all other C_ell - 1 basis elements.
- **Status:** COMPUTATIONALLY VERIFIED (ell=3,4,5,6,7)
- **Demo provenance:** Explorer's log, Demo 60 (ell=7)
- **Significance:** Forced by trace form: radical elements must have trace zero

### Zero Count Formula at n=ell+1
- **Statement:** At n=ell+1, the rad^2 generator has exactly ell^2 + 1 zero coefficients. Verified: ell=3→10, ell=4→17, ell=5→26, ell=6→37.
- **Status:** COMPUTATIONALLY VERIFIED (ell=3,4,5,6)
- **Demo provenance:** Explorer's log
- **Significance:** NOVEL formula

### 11/13 Re>0 Wall Universality
- **Statement:** The Re>0 multiplicative encoding reaches exactly 11/13 NPN classes at EVERY evaluation level tested (ell=4, ell=5, ell=6), with 0x1B and 0x06 always unreachable. The wall is ring-independent and activation-structural.
- **Status:** COMPUTATIONALLY VERIFIED (3 independent ell values) + ANALYTICALLY PROVEN (Demo 62)
- **Demo provenance:** Demos 55, 58, 59, 61, 62
- **Significance:** The strongest universality result — holds regardless of the underlying cyclotomic ring

### Decoherence Rate Scaling
- **Statement:** The radical proportion (2*ell-3)/C_ell decreases with ell: ell=3→60%, ell=4→36%, ell=5→17%, ell=6→7%, ell=7→2.6%.
- **Status:** COMPUTATIONALLY VERIFIED (ell=3,4,5,6,7)
- **Demo provenance:** Explorer's log, Demo 60 (ell=7: 11/429 ≈ 2.6%)
- **Significance:** Higher ell = less decoherence = more stable coherent sector. This is WHY Fibonacci anyons (ell=5) are better for quantum computing.

### Q=1 Percolation Universality
- **Statement:** Z_FK(G; Q=1, v) = (1+v)^|E| regardless of graph structure — geometrically trivial.
- **Status:** PROVEN (verified 4 strips, algebraic identity)
- **Demo provenance:** Demo 56
- **Significance:** Computational phase transition at Q=2 (Ising is first non-trivial Q value)

### Cross-ell Catalog Size Hierarchy
- **Statement:** Bracket catalog richness: ell=3 (6 values, binary), ell=4 (56 values, Z[i]-axial), ell=5 (116+ values, full Z[zeta_5]), ell=infinity/delta=0 (100 values, strict axiality). Non-monotonic: delta=0 has richer catalog than ell=3,4 despite simpler algebra.
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demos 29, 53, 54, 58
- **Significance:** Establishes that delta=0 has uniquely favorable properties for DKC

### Additive Bit-0 Constraint (Universal)
- **Statement:** Under additive encoding z = sum(xi*wi), z(0,...,0) = 0 always, constraining bit 0 of the truth table. This holds at ALL ell values.
- **Status:** PROVEN (trivial algebraic fact)
- **Demo provenance:** Demos 57, 58, 59
- **Significance:** Universal limitation of additive encoding independent of ring structure

---

## 7. DKC Geometry and Capacity (Demos 72–84)

### Anti-Correlation Theorem (Design Quality vs Computational Quality)
- **Statement:** The 13 eigenvector directions are simultaneously the worst-possible choice for spherical sampling (t=0 directed t-design, failing at l=1) and a locally optimal choice for DKC computation (36 XOR6 solutions vs 4 for a better-distributed design found by stochastic optimization). Sampling quality and computational quality are anti-correlated: the directions that make integration worse make computation better.
- **Status:** DEMONSTRATED (stochastic optimizer achieved 2.6× better design residual with 2.6× fewer XOR6 solutions)
- **Demo provenance:** Demo 72
- **Significance:** DKC computation is algebraic, not geometric — the directions are selected by the binary octahedral group acting on Z[ζ_8] quaternions; they compute because of specific angular relationships, not uniform distribution. Moving points to "better" geometric positions destroys the Voronoi partition structure needed for XOR6.

### DOF-Not-Design Principle
- **Statement:** The reason DKC operates at bandwidth l=6 with only 13 directions (well below the Nyquist prediction for l=6 of (6+1)²=49 cells) is the DOF coincidence 2×6+1=13=directions, combined with compressed sensing for binary functions at known positions. The t-design property of the direction set is irrelevant to this mechanism.
- **Status:** SUPPORTED (undirected t=3 achieved; first DKC-relevant failure at l=4 even mode, residual 0.576; Delsarte maximum with 13 points would be t=7 — none achieved)
- **Demo provenance:** Demo 72
- **Significance:** Classical sampling theory (t-design, Nyquist) does not govern DKC. The algebraic placement of directions by group theory bypasses geometric sampling requirements entirely.

### DKC is Additive, Not Multiplicative
- **Statement:** The multiplicative braid product formulation is structurally incapable of computing XOR6 via the Voronoi cell mechanism. Flipping all input bits (changing parity) maps M→M⁻¹, which shares the same Voronoi cell (antipodal eigenvectors are identified), making parity structurally invisible. The additive weight-sum formulation does compute XOR6 — the 36 winning triples all use the additive channel.
- **Status:** PROVEN (exhaustive: every cell under multiplicative product is maximally non-separating, containing exactly 8 even-parity and 8 odd-parity inputs)
- **Demo provenance:** Demo 73
- **Significance:** Computation requires additive weight sums, not matrix products. This connects DKC to lattice coding (Nazer-Gastpar compute-and-forward) rather than group-element computation.

### 82.8% Universal Determinism
- **Statement:** The Voronoi cell transition function for the additive DKC trajectory is deterministic in 48 of 58 (82.8%) of (prev_cell, bit, step) triples. This fraction is identical across all 36 XOR6 winning triples — a structural constant of the ζ_8 system. The 17.2% non-determinism arises entirely from discarded magnitude information (the "compass without odometer" problem): two quaternion sums with the same eigenvector direction but different magnitude produce different results when a weight is added.
- **Status:** PROVEN (all 36 winners verified; universality follows from binary octahedral group acting transitively on the 13 directions)
- **Demo provenance:** Demo 73
- **Significance:** The 13-cell Voronoi is an approximately-deterministic finite automaton. 100% determinism would require tracking magnitude information modulo the weight lattice, implying infinitely many states.

### Incomparability Theorem (Bracket vs Cell)
- **Statement:** The Kauffman bracket (at A=−ζ_8) and the DKC Voronoi cell are incomparable braid invariants. The bracket is a trace/normalization operation; the cell is an eigenvector direction projection. These operations discard orthogonal information. Neither refines the other: cell separates 97.8% of bracket collisions at the braid level (80% at catalog level); bracket separates 54% of cell collisions at the catalog level.
- **Status:** PROVEN (exhaustive: 2,000 braid correspondence records, 1,124,250 pairs analyzed)
- **Demo provenance:** Demo 74
- **Significance:** The combined invariant (bracket, cell) produces 119 equivalence classes vs 100 bracket-alone and 14 cell-alone — 19% finer than bracket alone. The DKC computation accesses a projection of the braid group representation that the bracket cannot see.

### 6 Computational Types
- **Statement:** Among the 14 Voronoi cells, only 6 distinct computational type vectors exist (each cell's binary parity label across all 36 winning triples). Types correspond to: axis σ₁ orbit (3 quaternions), axis σ₂ orbit (3 quaternions), body 60° cells (8 quaternions), edge cells (6 quaternions), third-axis orbit (3 quaternions), identity cell (1 quaternion). The DKC computation is invariant under permutation within each symmetry orbit.
- **Status:** PROVEN (exhaustive type-vector computation across all 36 winners and 14 cells)
- **Demo provenance:** Demo 74
- **Significance:** The binary octahedral symmetry group acts transitively within each orbit type. Only 6 computational roles exist despite 14 cells and 36 winners.

### Angle Subsumption
- **Statement:** For all 36 XOR6 winning triples, the eigenvalue angle of the sum quaternion perfectly predicts parity. The Voronoi cell assignment is a coarsening of the angle assignment — knowing the angle tells you the cell parity but not vice versa. Seven distinct eigenvalue angles appear in sum quaternions: 0°, 30°, 35.264°, 45°, 60°, 65.905°, 90°; three are non-canonical (arising from vector addition breaking the 24-cell group closure).
- **Status:** DEMONSTRATED (all 36 winners verified)
- **Demo provenance:** Demo 75
- **Significance:** The angle channel (2.004 bits entropy) subsumes the cell channel (1.000 bit entropy). Joint capacity is 2.282 bits — 2.3× over cell-only, with 24% lost to inter-channel mutual information.

### √n Magnitude Quantization
- **Statement:** The magnitude of additive weight sums |S| from Z[ζ_8] winning triples takes exactly 6 values: √0, √1, √2, √3, √4, √6. The value √5 is absent. This follows from the inner product structure of the 24-cell: pairwise inner products are drawn from {-1, -½, 0, ½, 1}, so |S|² = Σ w_i · w_j reaches only {0, 1, 2, 3, 4, 6} — integer sums achievable from half-integer building blocks.
- **Status:** PROVEN (algebraic argument; computationally verified across all sum quaternions of 36 winners)
- **Demo provenance:** Demo 75
- **Significance:** √5 is unreachable. Magnitude carries 1.839 bits of entropy but predicts parity for only 16/36 triples (44%) — it is not a reliable computational channel alone.

### DKC Requires Group-Breaking
- **Statement:** The multiplicative channel (quaternion product of weights) produces only 4 eigenvalue angles (the canonical octahedral set: 0°, 45°, 60°, 90°) and has zero XOR6 solutions. The additive channel produces 7 angles (including 3 non-canonical: 30°, 35.264°, 65.905°) and has 36 solutions. The non-canonical angles arise only through quaternion addition, which breaks the 24-cell group closure.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 75
- **Significance:** Computation requires the non-canonical angles that addition creates. Staying inside the group (multiplicative product) is computationally blind to parity; escaping it (additive sum) enables computation.

### ζ_8 S²-Capacity Limit (XOR8 = 0 at S²)
- **Statement:** The 14-cell S² Voronoi partition is a hard geometric ceiling for 6-input computation at ζ_8. For 8 inputs, 256 masks / 14 cells ≈ 18.3 masks per cell — parity-pure cells are geometrically impossible by pigeonhole. Exhaustive search over all C(24,4) = 10,626 quadruples confirms 0 XOR8 solutions with S² Voronoi activation. Best achievable accuracy is 50.0%.
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 76 (initial result), Demo 77 (shows this is activation-specific, not a root-of-unity limit)
- **Significance:** The 6-input capacity of ζ_8 at S²-only activation is not a fundamental ceiling — it is activation-specific. The correct framing is that S² Voronoi (14 cells) cannot separate 256 masks, not that ζ_8 cannot compute XOR8.

### Activation Determines Capacity
- **Statement:** The root of unity alone does not fix computational capacity. The same ζ_8 catalog (24 quaternions) gives 0 XOR8 solutions with S² Voronoi activation (14 cells) and 6 XOR8 solutions with S¹ × S² combined activation (112 cells). The activation function — how you read the quaternion — determines what functions the computation can express.
- **Status:** DEMONSTRATED (6 winning quadruples, all 100% accurate on all 256 input masks)
- **Demo provenance:** Demo 77
- **Significance:** Same discovery pattern as Demo 50 (parity blocked by split-sigmoid, not the lattice). The quaternionic S¹ × S² activation is the generalization of Aizenberg's complex-valued MVN from ℂ to ℍ.

### Paired Quaternion Structure for XOR8
- **Statement:** Every XOR8 winning quadruple at ζ_8 contains exactly one pair of quaternions sharing the same eigenvector direction but differing in eigenvalue angle. Removing either element of the pair leaves an XOR6-winning triple (100% verified across all 6 winners). All 6 winning quadruples use only the two canonical octahedral angles (45°, 90°) in their individual catalog entries, though their sums produce non-canonical angles.
- **Status:** PROVEN (all 6 XOR8 winners fully decomposed; 12/24 embedded triples are XOR6 winners)
- **Demo provenance:** Demo 77
- **Significance:** Seeds the Paired Extension Conjecture — XOR(N+2) winners may be systematically buildable from XOR(N) winners by adding a "shadow" quaternion sharing direction but providing additional eigenvalue resolution.

### Incomparability Implies Product Necessity
- **Statement:** Because the eigenvalue (S¹) and eigenvector (S²) channels of a quaternion are incomparable invariants (Demo 74), neither channel alone can compute XOR8, but their S¹ × S² product can. The incomparability proven at the catalog level is computationally load-bearing at the activation level.
- **Status:** DEMONSTRATED (S¹ alone: 0 XOR8 winners at any k tested; S² alone: 0 XOR8 winners; S¹ × S²: 6 XOR8 winners)
- **Demo provenance:** Demo 77
- **Significance:** The geometric incomparability result from Demo 74 has a direct computational consequence — you must read both channels jointly to unlock the full computational capacity.

### ζ_8 Wall Theorem (XOR10 = 0)
- **Statement:** XOR10 is uncomputable at ζ_8 under any tested activation. Exhaustive search over all C(24,5) = 42,504 quintuples at k=8, 10, 12, 16 sectors finds zero winners. Best achievable accuracy is 98.4% (1008/1024 masks) at k=10 and k=16. The ζ_8 XOR hierarchy terminates at 8 inputs.
- **Status:** PROVEN (exhaustive)
- **Demo provenance:** Demo 78
- **Significance:** The binary octahedral group (24 elements) generates a fixed catalog whose combinatorial diversity is exhausted at 5-element selection. XOR8 is the maximum computable XOR function at ζ_8.

### Non-Monotonicity in Sector Count (XOR10)
- **Statement:** XOR10 accuracy at ζ_8 is non-monotonic in sector count: k=10 achieves 98.4% but k=12 achieves only 96.9% — a regression. The pi/4 incommensurability of the ζ_8 lattice creates destructive interference at specific k values, exactly mirroring Demo 50's finding (906 solutions at k=6 > 756 at k=7 > 96 at k=8 for XOR3 parity).
- **Status:** COMPUTATIONALLY VERIFIED
- **Demo provenance:** Demo 78
- **Significance:** The incommensurability principle (Demo 50) is a recurring structural feature of ζ_8, not an accident of the XOR3 case.

### Phase Diagram of ζ_8 XOR Hierarchy
- **Statement:** The complete ζ_8 XOR capacity phase diagram is: XOR6 (14 cells, Voronoi activation, 36 winners) → XOR8 (112 cells, Sec8×Vor activation, 6 winners) → XOR10 (WALL, 0 winners, all activations exhausted). Masks/cells ratios: XOR6 = 4.6, XOR8 = 2.3, XOR10 = not achievable.
- **Status:** PROVEN (exhaustive at each level)
- **Demo provenance:** Demo 78
- **Significance:** Complete characterization of the ζ_8 computational regime. The wall is not an artifact of activation — it is the finite group ceiling of the binary octahedral group.

### Finite Group Ceiling Theorem
- **Statement:** A finite quaternion group imposes a hard capacity ceiling on DKC: once the catalog is fixed, combinatorial diversity is exhausted at some XOR(N). An infinite group has no such ceiling — capacity is limited only by computational search resources. Demonstrated by contrast: ζ_8 (finite, 24 elements) caps at XOR8; ζ_12 (infinite) reaches XOR10 (124 winners) and XOR12 (50+ winners).
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 79
- **Significance:** The determining variable for DKC capacity is group finiteness, not the subscript N of ζ_N.

### ζ_12 Breaks the ζ_8 Wall
- **Statement:** ζ_12 achieves XOR10 with 124 winners (vs 0 for ζ_8) and XOR12 with 50+ winners using an expanded 4096-entry catalog. XOR6 and XOR8 both have 1024+ winners at ζ_12 (capped). The ζ_12 group closure roughly doubles each round, reaching 4096+ entries by round 8 (growth: 5 → 17 → 51 → 127 → 275 → 564 → 1140 → 2292 → 4096+).
- **Status:** DEMONSTRATED (all XOR levels confirmed exhaustively)
- **Demo provenance:** Demo 79
- **Significance:** The capacity gain from ζ_12 is from genuinely new algebraic structure, not "more of the same." Only 3/24 ζ_8 quaternions and 2/13 ζ_8 directions appear in the ζ_12 catalog (12% and 15% overlap respectively).

### Only ζ_4 and ζ_8 Generate Finite SU(2) Subgroups
- **Statement:** Under the standard two-generator construction (generators σ_1, σ_2 from braid group with half-angle 2π/N), only ζ_4 (4 elements, Binary Dihedral 2D₂ = quaternion group Q₈) and ζ_8 (24 elements, Binary Octahedral = ADE type E₇) generate finite subgroups of SU(2). All other tested roots (ζ_6, ζ_10, ζ_12, ζ_16, ζ_20) generate infinite (or very large) groups, with direction counts saturating at 512 by round 8.
- **Status:** COMPUTATIONALLY DEMONSTRATED (7 roots tested)
- **Demo provenance:** Demo 80
- **Significance:** Finiteness is non-monotone in N: ζ_6 is infinite despite sitting between ζ_4 and ζ_8. The finite cases correspond exactly to angles that are rotational symmetries of the cube/octahedron (π/2 and π/4).

### ADE Classification of Finite DKC Groups
- **Statement:** The two finite cases under the two-generator SU(2) construction map to specific ADE Dynkin diagram types: ζ_4 → Binary Dihedral 2D₂ (D-series, order 8); ζ_8 → Binary Octahedral 2O (E₇, order 48). E₆ (binary tetrahedral, 24 elements) and E₈ (binary icosahedral, 120 elements) are not reachable under this generator construction.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 80
- **Significance:** ζ_10 (Fibonacci anyon territory) is NOT the binary icosahedral group — it generates an infinite group. The ADE classification exhausts the finite cases reachable by this construction.

### Quantum Dimension Vanishing at ζ_8
- **Statement:** At ζ_8, the quantum parameter is q = i, and the quantum dimension [2]_q = q + q⁻¹ = i + (−i) = 0. The quantum dimension vanishes exactly at the two finite cases ζ_4 and ζ_8, which correspond to lattice (singular) points of quantum group parameter space. All non-lattice roots (all infinite groups) are where Kuperberg's #P-hardness applies.
- **Status:** PROVEN (exact algebraic identity)
- **Demo provenance:** Demo 80
- **Significance:** The finite group structure of ζ_8 — and therefore its DKC capacity ceiling — is directly connected to quantum dimension vanishing in the Temperley-Lieb algebra. The algebraic singularity at [2]_q = 0 produces the group finiteness.

### Logarithmic Scaling Law
- **Statement:** For ζ_12 (infinite group), XOR capacity scales logarithmically with catalog size: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6. Each doubling of catalog size adds ~0.62 to max XOR; each additional +2 XOR inputs requires approximately 10× more entries. Transition points: XOR6 at ~5 entries, XOR8 at ~51, XOR10 at ~275, XOR12 at ~1140.
- **Status:** DEMONSTRATED (9 snapshot rounds, catalog size 5 to 4096)
- **Demo provenance:** Demo 81
- **Significance:** Connects DKC scaling to Kuperberg's #P-hardness — the exponential catalog cost per XOR level is consistent with the hardness of the Jones polynomial evaluation at non-lattice roots. XOR14 requires ~38,000 entries (feasible); XOR32 requires ~10^14 (infeasible).

### Direction Bottleneck
- **Statement:** Each XOR transition in the ζ_12 catalog is gated by direction count: XOR8 requires ~22 directions, XOR10 requires ~114, XOR12 requires ~507. After direction saturation (round 7, 512 directions), angular diversity alone drives continued winner-count growth. The transition from direction explosion to angle refinement is a clean phase boundary at closure rounds 6–7.
- **Status:** OBSERVED
- **Demo provenance:** Demo 81
- **Significance:** Angular resolution is the limiting factor for higher XOR levels, not raw catalog size. This constrains which optimization strategies can efficiently expand capacity.

### Linear Depth Law
- **Statement:** Maximum XOR capacity scales linearly with quaternion "depth" (birth round in group closure = number of generator multiplications = crossing depth in braid/knot sense): max_xor ≈ depth + 6. Each unit of crossing depth adds a fixed increment to computational capacity across all measured transition points (depth 0 → XOR6, depth 2 → XOR8, depth 4 → XOR10, depth 6 → XOR12).
- **Status:** DEMONSTRATED (all four transition points confirmed at ζ_12)
- **Demo provenance:** Demo 82
- **Significance:** The logarithmic scaling law from Demo 81 is a corollary: catalog grows as ~2^depth per round, so log₂(catalog) ≈ depth, giving max_xor ≈ 0.62 × log₂(catalog) + const. The depth law is the more fundamental statement.

### Algebraic Coherence Beats Vocabulary
- **Statement:** A subset of 564 entries from deep closure rounds (depth 8) reaches XOR12 with 19 winners. A strided 564-entry subset spanning all closure rounds (maximum possible vocabulary: 512 directions, 43 angles — matching the full 4096-entry catalog) reaches only XOR10. More vocabulary with less algebraic structure loses to less vocabulary with more depth. Deep entries are ~2× more efficient per entry for XOR capacity.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 82
- **Significance:** The determining factor is shared algebraic structure of deep entries — every depth-d entry is a product of d generators, creating common intermediate factors. Strided sampling destroys this structure despite maximizing geometric coverage.

### Depth IS Crossing Depth
- **Statement:** Each closure round equals one generator multiplication, which equals one crossing in the braid/knot sense. A depth-d quaternion encodes d crossings of the associated braid diagram. The linear capacity law (max_xor ≈ depth + 6) says computational power scales linearly with knot crossing complexity. Consistent with Kuperberg's #P-hardness: the hardness is in generating deep entries (exponential BFS cost), not in their algebraic structure itself (linear gain).
- **Status:** STATED (interpretation; not yet a formal theorem)
- **Demo provenance:** Demo 82
- **Significance:** Connects DKC computation directly to knot theory — more complexly knotted braid words compute more than many simply knotted words with equal or greater total vocabulary.

### Two-Component Winner Architecture
- **Statement:** Every XOR winner at all levels has a two-component structure: a shallow core (depth-0 entries = generators + identity appear in every winner at every level) plus deep extensions that provide the additional angular resolution. Mean entry depth rises monotonically: 0.52 (XOR6) → 0.63 (XOR8) → 1.00 (XOR10) → 1.98 (XOR12).
- **Status:** DEMONSTRATED (all winner tuples at all XOR levels analyzed)
- **Demo provenance:** Demo 82
- **Significance:** Deep entries function as "effective generators" encoding complex rotation axes from their algebraic structure as many-generator products. The shallow core anchors the computation while deep extensions refine the angular discrimination.

### Framing = +2 XOR Levels
- **Statement:** Jones normalization (removing the writhe-dependent phase factor (−A³)^{−w}) costs exactly 2 XOR levels at every root tested: bracket XOR12 → Jones XOR10 at ζ₁₂; bracket XOR8 → Jones XOR6 at ζ₈. The loss is discrete, constant, and root-independent. Framing is one unit of computational resource, comparable in magnitude to one crossing-depth unit (Demo 82).
- **Status:** DEMONSTRATED (ζ₈ and ζ₁₂ both confirmed; constant loss despite very different group sizes 24 vs 4096)
- **Demo provenance:** Demo 83
- **Significance:** Resolves the "N−2 pattern" observed in early demos — those demos were inadvertently computing Jones-normalized quantities. The bracket genuinely reaches higher XOR levels that Jones cannot; framing is not a bookkeeping anomaly but active computational content.

### Resource Decomposition Conjecture
- **Statement:** DKC computational power decomposes additively as: lattice base (XOR6) + 2 × depth_rounds + 2 × framing_present. The "+2 per resource unit" pattern matches across crossing depth (Demo 82) and writhe. These three resources — lattice, depth, and framing — are independent and additive.
- **Status:** SUPPORTED (matches at both ζ₈ and ζ₁₂; further verification across roots and depths required)
- **Demo provenance:** Demo 83
- **Significance:** Provides a unified accounting framework for DKC capacity. Each discrete resource contributes exactly +2 XOR levels; no resource substitutes for another.

### Angular Vocabulary is the Mechanism
- **Statement:** Jones normalization reduces distinct quaternion angles from 43 to 31 (−28%) while leaving direction count unchanged (512 → 512). The 2-level capacity loss maps entirely onto the angle reduction. Framing's computational contribution operates through the angular coordinate of the quaternion, not the directional coordinate — consistent with Demo 82's finding that angular refinement (not direction count) drives XOR10 → XOR12.
- **Status:** DEMONSTRATED (ζ₁₂ information collapse fully characterized: quaternions 4096 → 2710, angles 43 → 31, directions 512 → 512)
- **Demo provenance:** Demo 83
- **Significance:** Separates the mechanism of framing loss from direction-based explanations. The writhe-dependent phase rotates quaternions in the angular coordinate only; the S² directional structure is preserved.

### Writhe Independence from Depth
- **Statement:** Depth and writhe are genuinely independent computational axes: depth-writhe correlation r = 0.139 across 4096 ζ₁₂ entries. Mean |writhe| grows linearly with depth (0.80 → 1.33 → 1.82 → 2.14 → 2.61 → 3.19 for depths 0–8), but the low correlation confirms that a deep entry can have low writhe (crossings cancel) or high writhe (crossings accumulate) depending on chirality structure.
- **Status:** DEMONSTRATED (r = 0.139 at 4096 entries; strong evidence of genuine independence)
- **Demo provenance:** Demo 83
- **Significance:** Establishes that depth and framing are independent computational resources that can be combined without interference, supporting the additive resource decomposition conjecture.

### TQFT Anomaly is Computational Resource
- **Statement:** The Jones polynomial removes the writhe phase factor as a "framing anomaly" — the first term discarded at TQFT's maximally degenerate point (δ=0). Demo 83 demonstrates that this anomaly term is not noise but computation: it contributes exactly the 2 XOR levels that distinguish bracket from Jones capacity.
- **Status:** INTERPRETATION (consistent with all data; not a formal theorem)
- **Demo provenance:** Demo 83
- **Significance:** Connects to the TQFT/Witten (1989) framing anomaly literature. What field theory discards first for "purity" is precisely what DKC exploits for computation.

### Null Indispensability Theorem
- **Statement:** In the ζ₈ catalog, the 9 bracket-null entries (Re(q)=0, Kauffman trace vanishes) cannot be removed without destroying XOR capacity. The non-null-only subset (15 entries, 7 directions) achieves only XOR6; the full catalog (24 entries, 13 directions) achieves XOR8. The non-null-only subset performs below the random-15-entry baseline (mean max_xor 7.8), confirming nulls are disproportionately important rather than merely incidentally present.
- **Status:** DEMONSTRATED (exhaustive; non-null-only subset tested alongside 100-trial random controls)
- **Demo provenance:** Demo 84
- **Significance:** Refutes the assumption that bracket-null states are computational dead weight. In the ζ₈ regime, nulls contribute 6 directions (cube-edge-midpoints) exclusively unavailable to the 15 non-null entries, and these directions are necessary for XOR8 solutions.

### Null Directional Efficiency
- **Statement:** Each null entry in the ζ₈ catalog covers a unique S² direction not shared with other null entries (1.00 direction per null entry), while non-null entries share directions (0.47 direction per non-null entry). Null entries are maximally efficient as directional vocabulary; their contribution to S² coverage is strictly greater per entry than non-null entries.
- **Status:** DEMONSTRATED (direction geometry fully characterized: 6 null-only, 4 non-null-only, 3 shared directions)
- **Demo provenance:** Demo 84
- **Significance:** Explains mechanistically why removing nulls costs more than removing random entries: nulls hold exclusive geometric territory that non-null entries cannot cover.

### S¹ Flatness of Null-Only
- **Statement:** All bracket-null entries in the ζ₈ catalog have half-angle exactly 90°, placing them at a single point on S¹. The null-only subset (9 entries) occupies a single S¹ sector and derives its entire computational contribution from S² directional diversity. XOR6 count for the null-only subset is flat across all tested k_sec values (k=1,2,4,6,8), confirming S¹ provides zero additional discriminating power for this subset.
- **Status:** DEMONSTRATED (k_sec sweep confirmed; flatness is exact, not approximate)
- **Demo provenance:** Demo 84
- **Significance:** Provides a clean decomposition: nulls live on one point of S¹ (zero angular vocabulary) but cover 9 distinct directions on S² (maximal directional vocabulary). They are pure S² resources.

### Logarithmic Partner Hypothesis
- **Statement:** The LCFT (logarithmic conformal field theory) interpretation of null states is consistent with all Demo 84 results. In LCFT, null states |N⟩ satisfy ⟨N|N⟩=0 but |N⟩≠0, paired with logarithmic partners via Jordan-cell L₀ action. The 6 null-only directions (cube-edge-midpoints, no non-null partner in the catalog) correspond to null states lacking logarithmic partners — and removing them destroys capacity rather than merely reducing it.
- **Status:** SUPPORTED (consistent with all geometric and capacity results; Jordan-cell structure not yet made explicit)
- **Demo provenance:** Demo 84
- **Significance:** Connects DKC null states to LCFT (Gurarie 1993, Flohr 1996, Gaberdiel-Kausch 1996). The three shared-direction entries (coordinate axes, appearing in both null and non-null subsets) are candidate Jordan-cell pairs — null state with vanishing trace, non-null partner carrying computational content.

### Null Fraction Dilution Conjecture
- **Statement:** The fraction of bracket-null entries in the DKC catalog decreases sharply as the group grows: ζ₄ = 75.0% (3/4), ζ₈ = 37.5% (9/24), ζ₁₂ = 3.0% (121/4096). In an infinite group the null fraction approaches zero, consistent with the hypothesis that nulls are a finite-group boundary effect — the discrete S¹ spectrum collapses and the half-angle=90° entries become an infinitesimal measure.
- **Status:** OBSERVED (three data points; mechanism consistent with finite-group boundary interpretation)
- **Demo provenance:** Demo 84
- **Significance:** Predicts that the null-indispensability result is specific to the ζ₈ (finite group) regime. At ζ₁₂, with only 3% null fraction but 29 null-only directions, whether nulls remain indispensable for XOR12 solutions is an open question.

### Null Dispensability at Infinite Groups
- **Statement:** At ζ₁₂ (infinite group, 4096 entries, 121 null entries = 3.0%), removing all bracket-null entries preserves full XOR capacity (XOR6/8/10/12 all 32 winners, identical to full catalog). Null entries are directionally indispensable (67 exclusive S² axes) but computationally dispensable. This contrasts with ζ₈ where removing nulls destroys XOR8 capacity (32 → 4 winners).
- **Status:** DEMONSTRATED (exhaustive; non-null-only subset tested at all XOR levels)
- **Demo provenance:** Demo 87
- **Cross-references:** Null Indispensability Theorem (Demo 84), Null Fraction Dilution Conjecture (Demo 84), Regime Transition Theorem (below)
- **Significance:** Null indispensability does NOT generalize from finite to infinite groups. The transition mechanism is direction density — when the non-null direction count is large enough (1976 out of 2043), losing 67 null-only directions cannot constrain combinatorial search.

### Regime Transition Theorem (Null Indispensability)
- **Statement:** There is a qualitative transition between finite groups (ζ₈: nulls computationally critical, 37.5% of catalog, 6/13 exclusive directions in a sparse direction set) and infinite groups (ζ₁₂: nulls computationally redundant, 3% of catalog, 67/2043 exclusive directions in a dense direction set). Directional indispensability (owning exclusive S² axes) is necessary but not sufficient for computational indispensability.
- **Status:** DEMONSTRATED (two-root comparison)
- **Demo provenance:** Demo 87
- **Cross-references:** Null Indispensability Theorem (Demo 84), Null Dispensability at Infinite Groups (above)
- **Significance:** Establishes that the controlling variable for null indispensability is direction density, not null fraction per se. At ζ₈ the non-null directions are too sparse; at ζ₁₂ they are dense enough.

### Two-Role Direction Theorem
- **Statement:** In the ζ₈ cuboctahedral direction set, the 13 directions serve two distinct computational roles: the 4 non-null body-diagonal directions are rigid computational anchors (perturbation-sensitive, load-bearing for XOR), and the 6 null-only edge-midpoint directions are flexible topological scaffolding (perturbation-tolerant, movable, provide S² coverage). At eps=10° perturbation: null-only causes -2.4% XOR loss (actually a gain), non-null-only causes 8.0% loss.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 88
- **Cross-references:** Anti-Correlation Theorem (Demo 72), Null Indispensability Theorem (Demo 84), Null Directional Efficiency (Demo 84)
- **Significance:** The initial prediction (null directions more sensitive) was INVERTED. Non-null body-diagonal directions are the fragile, load-bearing anchors; null edge-midpoint directions are redundant scaffolding.

### Constrained Optimality (Rigid Anchors, Flexible Scaffolding)
- **Statement:** Constraining non-null directions to within 2° of algebraic placement while allowing null directions to move freely captures 92% of unconstrained spherical design improvement (48% vs 52% design residual reduction) while reducing computational loss by 56% (2.8% vs 6.4% XOR loss). Null directions carry nearly all the design-improvement capacity.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 88
- **Cross-references:** Two-Role Direction Theorem (above), Anti-Correlation Theorem (Demo 72)
- **Significance:** Provides a practical optimization strategy: improve geometric quality (for sampling/integration) while preserving computational structure by only moving non-load-bearing directions.

### K-Ladder Resilience Theorem
- **Statement:** The Sec×Vor k-ladder activation (K_LADDER = {1,6,8,10,12,16,20,24}) provides an order-of-magnitude better perturbation resilience than Voronoi-only activation under the same design-improving gradient: 6.4% XOR loss vs 89% XOR loss (Demo 72). The mechanism: multiple k_sec values provide fallback angular resolutions when direction perturbation invalidates the optimal sector assignment.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 88
- **Cross-references:** Anti-Correlation Theorem (Demo 72), Activation Determines Capacity (Demo 77)
- **Significance:** The activation function (Voronoi-only vs k-ladder) is the primary variable controlling perturbation sensitivity, not the optimization procedure. The same Sec×Vor activation introduced for capacity (Demo 77) also provides computational robustness — dual benefit.

---

## 8. Depth Law Mechanism (Demos 89–91)

### Pairwise Coherence Non-Mechanism
- **Statement:** Mean pairwise |qdot| (quaternion alignment) is flat at ~0.42 across all depth shells (0-8) and identical to random baseline. Intra-depth coherence (0.4214) is actually LESS than inter-depth coherence (0.4254) — depth shells are more internally orthogonal than across shells. Quaternion alignment does NOT explain the depth advantage.
- **Status:** DEMONSTRATED (KILLED as explanation)
- **Demo provenance:** Demo 89 (Phase 3, Phase 5)
- **Cross-references:** Linear Depth Law (Demo 82), Axis Cancellation Mechanism (Demo 90)
- **Significance:** One of 5 candidate mechanisms eliminated in the D89-D90-D91 depth law arc.

### Direction Coverage Non-Mechanism
- **Statement:** All 13 ζ₈ directions are covered by depth 2. The +6 intercept in max_xor ≈ depth + 6 relates to XOR6 base capacity, not direction coverage timing.
- **Status:** DEMONSTRATED (KILLED as intercept explanation)
- **Demo provenance:** Demo 89 (Phase 4)
- **Cross-references:** Linear Depth Law (Demo 82)
- **Significance:** Direction coverage completes much earlier than the depth law begins to operate.

### Cayley Density Non-Mechanism
- **Statement:** Same-depth entries have ZERO mutual Cayley edges (mathematically necessary from BFS construction). Deep-564 subset has 0 internal Cayley edges; strided-564 has 58. Yet deep beats strided on XOR capacity. Graph density and computational capacity are ANTI-CORRELATED.
- **Status:** DEMONSTRATED (KILLED and INVERTED)
- **Demo provenance:** Demo 89 (Phase 8)
- **Cross-references:** Algebraic Coherence Beats Vocabulary (Demo 82)
- **Significance:** Cayley graph connectivity is not the mechanism; in fact, less graph connectivity correlates with more computational power.

### Sector Saturation Non-Mechanism
- **Statement:** Pairwise sum sectors saturate early and do not grow with depth. Voronoi coverage is always 100%. Sectors are NOT the bottleneck for the depth law.
- **Status:** DEMONSTRATED (KILLED)
- **Demo provenance:** Demo 89 (Phases 7, 9)
- **Cross-references:** Linear Depth Law (Demo 82)
- **Significance:** Individual sectors saturate quickly; the bottleneck is relational (how entries combine), not positional.

### Axis Cancellation Mechanism
- **Statement:** XOR winners preferentially contain anti-aligned quaternion pairs. Mean min_dot for XOR8 winners = -0.75 vs -0.65 for random non-winners. Two quaternions with nearly opposite rotation axes partially cancel, yielding a sum quaternion with a small rotation angle (30-66°) — exactly what sector activation needs for parity satisfaction. This is the geometric content of "algebraic coherence" identified in Demo 82.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 90 (Phase 2b)
- **Cross-references:** Algebraic Coherence Beats Vocabulary (Demo 82), Cross-Depth Algebraic Constraint (below), Relational Computation Thesis (below)
- **Significance:** First identification of the geometric mechanism underlying the depth law. Winners select for anti-alignment, which produces low sum angles needed for fine-grained sector discrimination.

### Cross-Depth Algebraic Constraint
- **Statement:** BFS-derived entries at depth d have constrained pairwise sums with generators: generator × deep (d=7-8) produces only 73 distinct angles from 50K pairs, compared to 1313 for deep × deep and 15 for shallow × shallow. The BFS ancestry creates algebraic relationships that restrict the sum-angle vocabulary between related entries.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 90 (Phase 2c)
- **Cross-references:** Axis Cancellation Mechanism (above), Algebraic Coherence Beats Vocabulary (Demo 82)
- **Significance:** The concrete realization of "algebraic coherence": BFS ancestry forces entries to share intermediate algebraic structure, constraining their pairwise sums to a small, well-structured vocabulary.

### Spectral Inversion Theorem
- **Statement:** The S² point cloud bandwidth (spherical harmonic decomposition of rotation axis positions) DECREASES as depth increases. BW_90% drops from l=4 (depth 0 only) to l=2 (cumulative all depths). BW_99% drops from l=11 to l=6. The point cloud converges to spherical uniformity (l=0 dominant). Computational power grows while positional information content shrinks.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 90 (Phase 5)
- **Cross-references:** Relational Computation Thesis (below), 13=13 Theorem (Demo 71)
- **Significance:** RULES OUT all positional/bandwidth-based explanations for the depth law. The Altschuler-Parrilo hypothesis (deeper = higher bandwidth = more computational modes) is INVERTED: deeper entries are more uniformly distributed, meaning individual positions carry less information. The mechanism must be relational.

### Relational Computation Thesis
- **Statement:** The depth law is relational: what matters is how entries combine (axis cancellation, cross-depth algebraic constraints), not where they individually sit on S². The spectral inversion is the cleanest evidence — positional information degrades monotonically while computation improves linearly.
- **Status:** DEMONSTRATED (synthesis of D89/D90/D91 results)
- **Demo provenance:** Demos 89, 90, 91
- **Cross-references:** Axis Cancellation Mechanism (D90), Cross-Depth Algebraic Constraint (D90), Spectral Inversion (D90), Parity Wall Theorem (D91)
- **Significance:** Resolves the "algebraic coherence" mystery from Demo 82. Deep entries compose better not because they are individually special, but because their algebraic ancestry creates favorable relational structure (anti-aligned axes, constrained sum vocabularies).

### Paired Extension as Minority Pathway
- **Statement:** Shadow pairing (two quaternions sharing the same Voronoi direction but different eigenvalue angle) is real (100% nesting when present) but constitutes only 6-20% of XOR winners at ζ₁₂ (cumulative depth 4, 275 entries, 114 dirs). At ζ₈ (24 entries, 13 dirs) it was 100%. The dominant pathway (80-94% at ζ₁₂) uses all-distinct-direction tuples.
- **Status:** DEMONSTRATED (ζ₈ and ζ₁₂ compared)
- **Demo provenance:** Demo 89 (Phase 11)
- **Cross-references:** Paired Extension Conjecture (Demo 77), Paired Quaternion Structure for XOR8 (Demo 77)
- **Significance:** Shadow pairing dominates when direction vocabulary is small (ζ₈); direction diversity dominates when large (ζ₁₂). Both depend on the same underlying algebra but represent different computational strategies.

### +2 Sectors per Weight
- **Statement:** Each additional weight in multi-element quaternion sums adds approximately 2 sectors to the coverage count, matching the +2 XOR input step per additional weight in the depth law.
- **Status:** OBSERVED
- **Demo provenance:** Demo 89 (Phase 9)
- **Cross-references:** Balanced Exponentials Linearity (D91), Linear Depth Law (D82)
- **Significance:** Provides a per-weight sector accounting that connects the depth law slope to the activation cell structure.

### log2(Cell Diversity) Linear in Depth
- **Statement:** Pairwise sum cell diversity (number of distinct cells occupied by pairwise quaternion sums) grows exponentially with depth: log2(distinct_cells) ≈ 1.17d + 2, which is linear and tracks max_xor quantitatively. ~66% cell fill ratio at k=6 regardless of depth.
- **Status:** OBSERVED (causality vs epiphenomenality unresolved)
- **Demo provenance:** Demo 89 (Phase 6)
- **Cross-references:** Linear Depth Law (D82), Balanced Exponentials Linearity (D91)
- **Significance:** Quantitative correspondence with max_xor is real, but both may be driven by the same underlying cause (BFS vocabulary growth).

### Activation Resolution–Solution Count Scaling
- **Statement:** XOR8 winner count at fixed catalog depth scales roughly linearly with activation cell count (k_sec × n_dirs), growing from hundreds at minimal resolution (k=2, 4 total cells) to tens of thousands at maximum resolution (k=48). Winner count grows 13× from k=2 to k=48 while max_xor remains unchanged.
- **Status:** DEMONSTRATED
- **Demo provenance:** Demo 91
- **Cross-references:** Parity Wall Theorem (D91), Depth Gap Invariance (D91)
- **Significance:** Resolution reveals MORE solutions to the same parity constraint; it does not enable HIGHER parity. This is the key diagnostic separating the activation's role (counting) from the parity constraint's role (existence).

---

## Named Conjectures (Open)

### TL Non-Semisimplicity Hypothesis (for Parity)
- **Statement:** Parity's residual resistance under MVN may reflect the non-semisimple structure of TL at q=zeta_8. The Clifford boundary — bracket evaluation factors through TL/radical (semisimple quotient), killing exactly the degrees of freedom needed for parity.
- **Status:** CONJECTURED
- **Demo provenance:** Demos 47, 48
- **Significance:** Would provide the algebraic explanation for the parity wall

### Two-Channel Coding Conjecture
- **Statement:** The bracket catalog has a magnitude channel (~4.3 bits, accessible via split-sigmoid) and a phase channel (~1.5 bits, accessible via MVN-continuous). A joint decoder could achieve ~5.8 bits/symbol.
- **Status:** CONJECTURED
- **Demo provenance:** Demo 47
- **Significance:** Information-theoretic framing of the activation question

### Incommensurability Hypothesis
- **Statement:** Parity solutions anti-correlate with gcd(k,8) — sector boundaries that misalign with the lattice's pi/4 angular structure break the symmetry that blocks parity. k=6 (gcd=2) optimal, k=8 (gcd=8, perfect alignment) minimal.
- **Status:** CONJECTURED (supported by data: 906 > 756 > 96)
- **Demo provenance:** Demo 50
- **Significance:** Could be made rigorous as a theorem about gcd(k,8) and NPN reachability

### Triskelion Generalization Conjecture
- **Statement:** Does n-input parity require k=2n sectors (n odd sectors for n inputs)?
- **Status:** **FALSIFIED** — n=5 parity requires k=15, not k=10. The triskelion pattern is specific to 3-input parity; generalization follows a different law (k=2M-1 where M is the ray count, not 2n). On S² (Demo 66-67), parity (XOR6) is achievable with only 14 Voronoi cells using the eigenvector geometry, showing the S¹ k-sector framework is not the only path.
- **Demo provenance:** Demo 50 (original conjecture), Demo 63 (falsification), Demo 65-67 (S¹/S² comparison)
- **Significance:** The scaling is lattice-geometric (k=2M-1) on S¹, not input-count-proportional (k=2n); S² geometry offers an entirely different (and more efficient) route

### Galois Symmetry Conjecture
- **Statement:** The sandwich duality (top ≅ bottom) could correspond to a non-trivial Galois automorphism of Gal(Q(zeta_8)/Q). The 4-element Galois group and the asymptotic 1/4 visible ratio may not be coincidental.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Explorer's log
- **Significance:** Would connect number theory to radical structure

### ell=7 Predictions (Cubic Wall)
- **Statement:** At ell=7 (delta = 2cos(pi/7), cubic number field): rad(TL_7) = 11, rad(TL_8) = 39, rad^2 = 1 at n=7,8. Fibonacci Rank = F(6) = 8. Sector profile: 4 sectors (sector 1 rank 3, sector 3 rank 4, sector 5 rank 1, sector 7 rank 0). Nilpotency = 3 at both n=7 and n=8.
- **Status:** ALL CONFIRMED (Demo 60 — every prediction verified exactly, cross-validated at two independent primes)
- **Demo provenance:** Explorer's log (prediction), Demo 60 (verification)
- **Significance:** Strongest validation of universal formulas. First cubic number field case confirms radical structure is independent of number field degree.

### Number Field Degree Independence Conjecture
- **Statement:** All radical structure formulas (dimension 2*ell-3, dimension ell^2-ell-3, rad^2=1, nilpotency=3, Fibonacci rank=F(ell-1)) hold for ALL ell values regardless of the degree of the number field Q(2cos(pi/ell))/Q. Tested: degree 1 (ell=2,3,4,6), degree 2 (ell=5), degree 3 (ell=7).
- **Status:** CONJECTURED (verified through ell=7 across three distinct number field degrees)
- **Demo provenance:** Demo 60
- **Significance:** Would mean the radical structure is purely combinatorial (Temperley-Lieb algebra structure), not dependent on the arithmetic of the evaluation ring. Next test: ell=11 (degree 5 number field).

### Cross-Sector Coupling Growth Conjecture
- **Statement:** At delta=0, the cross-sector coupling (sum of per-sector Gram ranks minus full Gram rank) grows rapidly with n: n=4→0, n=6→4, n=8→104. Does this follow a closed-form formula?
- **Status:** OPEN CONJECTURE (three data points)
- **Demo provenance:** Demo 39
- **Significance:** Quantifies how the radical's structure becomes increasingly cross-sectoral at larger n, limiting naive sector-by-sector reduction.

### Universal Parity Ceiling Conjecture
- **Statement:** The parity ceiling n=5 (maximum inputs for which parity is computable by a single MVN neuron with DKC-derived weights) holds for ALL cyclotomic evaluation points — not just Z[zeta_8] and Z[zeta_16]. If confirmed, this becomes a theorem about topological state sums, with no reference to any specific evaluation ring.
- **Status:** OPEN CONJECTURE (confirmed at Z[zeta_8] and Z[zeta_16]; not yet tested at Z[zeta_32] or higher)
- **Demo provenance:** Demo 65
- **Significance:** Would elevate the ceiling from an algebraic fact to a topological one — the ceiling reflects something about the braid group itself, not any particular quantum group evaluation.

### Clifford Scaling Law Conjecture
- **Statement:** The minimum k for generalized (non-standard) parity activation grows roughly by factor ~3.8 per increment: XOR6 k=24, XOR7 k=91, XOR8 ~k=345. Two data points only; the growth rate could discriminate polynomial from exponential.
- **Status:** OPEN CONJECTURE (two data points)
- **Demo provenance:** Demo 65
- **Significance:** If confirmed, would provide a predictive framework for higher-arity parity in the generalized DKC setting.

### k=24 as 24-Cell Projection Conjecture
- **Statement:** The minimum of k=24 sectors for S¹ XOR6 generalized solutions (Demo 65) is a projection of the 24-cell's 24 vertices onto S¹ via the quaternion trace. The 24-cell vertex count and the S¹ threshold coincide because the trace projection maps the 24 binary octahedral group elements to the 24 S¹ sectors.
- **Status:** CONJECTURED — supported by Demo 66 (24-cell emergence from same braid generators)
- **Demo provenance:** Demo 65 (conjecture), Demo 66 (supporting evidence)
- **Significance:** Would unify the S¹ and S³ pictures of DKC into a single geometric explanation.

### Orbit Count Formula Conjecture (XOR_n on 24-cell)
- **Statement:** The F4 orbit count for XOR_n solutions on the 24-cell Voronoi is n*(n−1)/2: XOR6 yields 6=1+2+3 orbits, matching the partition count of 6. XOR_n on the 24-cell may always yield exactly n*(n−1)/2 orbits.
- **Status:** OPEN CONJECTURE (one data point: n=6 → 6 orbits)
- **Demo provenance:** Demo 66
- **Significance:** Would connect the F4 symmetry decomposition to the combinatorics of the input arity.

### 14-Cell Minimum Conjecture (XOR6 on S²)
- **Statement:** The natural 13+1 eigenvector Voronoi (13 data-derived directions + 1 zero cell) is the true minimum cell count on S² for XOR6 DKC. Platonic geometries at 6 and 12 cells fail; 14 cells suffice.
- **Status:** OPEN CONJECTURE (lower bound not proven; 6 and 12 confirmed failures)
- **Demo provenance:** Demo 67
- **Significance:** Would establish 14 as a fundamental geometric constant for XOR6 on S², analogous to k=6 on S¹ for standard-activation XOR3.

### Trivialization Threshold Conjecture
- **Statement:** The transition from topology-driven (few solutions, geometry constrains computation) to pigeonhole-dominated (~90% solution rate, statistics dominate) occurs between approximately 50 and 500 Voronoi cells on S². Below this threshold the geometry of directions matters; above it cell count alone predicts the solution rate.
- **Status:** OPEN CONJECTURE (threshold not yet precisely located; ζ_8 at 14 cells is far below, ζ_16 at 3,458 cells is far above)
- **Demo provenance:** Demo 69, Demo 71 (DOF prediction)
- **Significance:** Locating the threshold precisely would characterize the "useful" regime of DKC computation where topology provides genuine computational advantage.

### S² Bandwidth Minimum Theorem (Analytical Proof Open)
- **Statement:** For any binary Voronoi partition separating N points on S² in general position, the minimum spherical harmonic bandwidth is l = ⌈(N−1)/2⌉. For N=13 eigenvector directions, this gives l=6, matching the observed sharp phase transition. Computationally verified; no analytical proof yet.
- **Status:** OPEN CONJECTURE (computational evidence; general proof open)
- **Demo provenance:** Demo 71
- **Significance:** Would turn the 13=13 Theorem into a general statement about Voronoi partitions on S², applicable beyond the DKC context.

### Minimum States Conjecture (Automaton for XOR6)
- **Statement:** The theoretical minimum cell count for a fully deterministic automaton computing XOR6 via DKC at ζ_8 is 27 (maximum distinct partial sums at step 5). Whether a 27-state partition achieves 100% determinism is not yet proven — the signed partition with 27 cells reaches only 87.1%. The remaining 12.9% non-determinism may require tracking magnitude information modulo the weight lattice.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 73
- **Significance:** Would characterize the minimum state-space representation of XOR6 DKC as a finite automaton, and determine whether the magnitude loss (the "missing odometer") can be encoded finitely.

### Paired Extension Conjecture
- **Statement:** XOR(N+2) winners at ζ_8 are built by extending XOR(N) winners with one "shadow" quaternion sharing the same eigenvector direction as an existing entry but differing in eigenvalue angle. Confirmed for XOR6→XOR8 (all 6 XOR8 winners decompose this way, shadows all have half-angle 45°). Whether this recursion extends to larger input sizes and other roots of unity is open.
- **Status:** CONFIRMED for one step (XOR6→XOR8); not yet tested at ζ_12 or higher
- **Demo provenance:** Demo 77, Demo 78
- **Significance:** Would give a constructive algorithm for building higher-arity DKC solutions from lower-arity ones, potentially replacing exhaustive search.

### Power-of-Two Finiteness Conjecture
- **Statement:** Under the two-perpendicular-axis generator construction, the finite SU(2) subgroups are generated exactly by angles of the form π/2^k for k ≥ 1. This predicts ζ_4 (k=1, π/2) and ζ_8 (k=2, π/4) are finite, and further predicts ζ_32 (θ=π/16) should also be finite. Not yet tested.
- **Status:** OPEN CONJECTURE (two finite cases confirmed; ζ_32 not yet tested)
- **Demo provenance:** Demo 80
- **Significance:** Would give a clean algebraic criterion for predicting which roots of unity impose hard capacity ceilings on DKC.

### Union Catalog Conjecture
- **Statement:** A combined ζ_8 + ζ_12 catalog (largely non-overlapping: only 12% quaternion overlap, 15% direction overlap) might push XOR capacity beyond what either achieves alone, since the two algebraic structures are independent.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 79
- **Significance:** Would determine whether independent algebraic structures can be superimposed for superadditive computational capacity.

### Possibly Sub-Logarithmic Scaling
- **Statement:** The decreasing gaps between XOR transitions at ζ_12 (3.35 doublings for XOR6→XOR8, 2.43 for XOR8→XOR10, 2.05 for XOR10→XOR12) suggest the scaling may be O(log N / log log N) rather than pure O(log N). Requires more data points (XOR14 at ~38,000 entries) to distinguish.
- **Status:** OPEN CONJECTURE (three data points; XOR14 test feasible)
- **Demo provenance:** Demo 81
- **Significance:** Would refine the scaling law characterization and improve predictions for the feasibility of higher XOR levels.

### Sweet-Spot Cell-Count Formula
- **Statement:** The non-trivial DKC sweet spot (neither too coarse for parity-pure cells nor trivially satisfied by pigeonhole) occurs when masks/cells ≈ 4.6 (the value at XOR6). For n inputs (2^n masks) this requires cells ≈ 2^n / 4.6, and for XOR8: cells ≈ 56. The actual critical threshold observed at Demo 77 is ~100 cells (between 84 for k=6×Vor and 112 for k=8×Vor).
- **Status:** OPEN CONJECTURE (ratio 4.6 from one data point; XOR8 threshold approximately characterized)
- **Demo provenance:** Demos 76, 77
- **Significance:** Would provide a predictive formula for the minimum activation resolution required for any XOR level at any root of unity.

### Optimized Design Lower Bound Conjecture
- **Statement:** The stochastic optimizer found 4 solutions for a "better" geometric design (2.6× lower design residual than eigenvectors). Is 0 achievable? Is there a geometric argument that any t≥1 spherical design on S² yields fewer XOR6 solutions than the algebraic eigenvector configuration?
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 72
- **Significance:** Would establish whether 36 is a strict optimum over all 13-point configurations on S², or merely a local maximum.

### Radical Dimension at n=ell Pattern
- **Statement:** Radical dimensions at the semisimplicity boundary n=ell follow the sequence: ell=2→1, ell=3→3, ell=4→5, ell=5→0, ell=6→9, ell=7→11. These are 2*ell-3 except at ell=5 where the algebra is semisimple (radical=0).
- **Status:** **PROVEN** — rad = 2ell-3 is now formally proved in `proofs/radical-dimension-formula.md`. The ell=5 exception is explained: delta=phi satisfies no Chebyshev zero condition for n=ell=5.
- **Demo provenance:** Demo 39 (multi-ell boundary data), Demo 60 (ell=7 confirmation). **Proof:** `proofs/radical-dimension-formula.md`
- **Significance:** The ell=5 exception (semisimple at n=ell) is the Fibonacci anyon case — a structural anomaly in the radical landscape.

### Valuation Condition Repair Conjecture (b at Larger Lattices)
- **Statement:** The delta-parameterized leading-coefficient method for computing b succeeds only at TL_4 (where p_tt = 2*p_Tt holds) and diverges at TL_6 and on single P_{0,0} copies. The actual Pearce-Rasmussen method likely uses transfer matrix + finite-size scaling rather than the diagram-algebraic approach. Identifying and implementing their exact method remains open.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 85 (valuation condition), Demo 86 (universal divergence on P_{0,0})
- **Significance:** Would resolve how to compute b from first principles at lattice sizes beyond TL_4.

### Combinatorial-Algebraic Balance Conjecture
- **Statement:** The linear depth law (max_xor ≈ depth + 6, slope ≈ 1) arises from the ratio of two exponential processes: catalog growth ~2×/depth round (BFS branching) vs parity constraint difficulty ~4×/weight level (2^{2N} masks). These two exponentials balance to give ~2 depth rounds per XOR level. The precise coefficient (slope = 1) comes from the BFS branching factor of the specific SU(2) generator construction. Different generators or groups could yield different slopes.
- **Status:** SUPPORTED (confirmed from supply and demand sides in D90/D91; not analytically derived from first principles)
- **Demo provenance:** Demo 89 (proposed), Demo 90 (supply side), Demo 91 (demand confirmation)
- **Significance:** Would be a genuine theoretical result if the BFS branching factor could be connected to the parity constraint growth rate analytically.

### Casimir Balance Hypothesis
- **Statement:** Catalog doubling vs linear eigenvalue gap yields ~1 new representation per depth round, potentially explaining the linear depth law via representation-theoretic capacity growth.
- **Status:** CONJECTURED (suggested by researcher; consistent with angle data but not proven)
- **Demo provenance:** Demo 89
- **Significance:** Would provide a representation-theoretic rather than combinatorial explanation for the depth law.

### Anti-Correlation Persistence Conjecture
- **Statement:** The anti-correlation between spherical design quality and computational power (better sampling = fewer XOR solutions) persists at ζ₁₂, though the mechanism shifts from null-direction-driven to sheer direction-count-driven as the group grows. Per-direction residual at ζ₁₂ is 8× lower than ζ₈.
- **Status:** SUPPORTED (two data points: ζ₈ and ζ₁₂)
- **Demo provenance:** Demo 88 (Phase 4)
- **Significance:** Would establish that algebraic DKC computation is fundamentally anti-correlated with geometric sampling quality across all roots of unity.

### Depth Law Generalization to Non-Parity Functions
- **Statement:** Does the linear depth law hold for ALL Boolean functions, or just parity? Different functions have different constraint growth rates (parity is hardest — outside AC⁰), so different slopes are expected. Sub-linear slopes for easier functions, potentially no depth dependence for the simplest.
- **Status:** OPEN CONJECTURE
- **Demo provenance:** Demo 91 (open question), Demo 92 (parity-lock shows only XOR achievable under ±q encoding)
- **Significance:** Would characterize the full Boolean function landscape under the depth law framework.

---

## Verified Computational Facts (Not Theorems Per Se)

### Kauffman Bracket State-Sum Agreement
- Braid closure state-sum = PD-code state-sum for all tested knots (Demo 02, 07)
- Matrix method = state-sum bracket for all tested braids at delta=0 (Demo 35)
- Matrix method = state-sum at delta=1 (Demo 53), delta=sqrt(2) (Demo 54)

### Known Bracket Values at delta=0
| Braid | n | Bracket |
|-------|---|---------|
| Hopf link (sigma_1^2) | 2 | 2 |
| Trefoil (sigma_1^3) | 2 | -3*zeta_8 |
| Figure-eight | 3 | 5 (= C_3, coincidence?) |
| sigma_1*sigma_2*sigma_3 | 4 | zeta_8 |
| sigma_1^2*sigma_3^2 | 4 | 0 (entanglement vanishing) |

### Catalan Number Dimensions
TL_n basis: C_2=2, C_3=5, C_4=14, C_5=42, C_6=132, C_7=429, C_8=1430

### Yang-Baxter Equation
Verified in tensor-product form (R_i R_{i+1} R_i = R_{i+1} R_i R_{i+1}) at every scale n=2 through n=8, both symbolically and numerically (Demos 03-05, 10)

### Reidemeister Invariance
Jones polynomial (writhe-normalized bracket) is invariant under all three Reidemeister moves. R1 changes bracket by -A^{3s} factor (corrected by writhe). R2 and R3 leave bracket unchanged. (Demo 09)

### Multi-delta Gram Ranks (Demo 39)
- n=4: delta=0→5, delta=1→11, delta=sqrt(2)→9, delta=phi→14 (semisimple)
- n=6: delta=0→42, delta=1→99, delta=sqrt(2)→58, delta=phi→132 (semisimple)
- Gram rank hierarchy is non-monotonic across delta values

### ell=7 Cubic Field Arithmetic (Demo 60)
- Minimal polynomial: x^3 - x^2 - 2x + 1 = 0 (irreducible over Q)
- Cross-validated at two independent primes: p1=1,000,000,007 and p2=1,000,000,009
- All radical structure results agree at both primes (characteristic-independent)

### Quotient Nonzero Products at delta=0 (Demo 38)
- TL_2/rad=1 nonzero product (matches TL_1)
- TL_4/rad=17 nonzero products (matches TL_3)
- TL_6/rad=955 nonzero products (matches TL_5)

---

## Novelty Summary

**Literature-confirmed NOVEL contributions (no prior work connects these):**
1. Axiality theorem and proof sketch (Demo 35)
2. Sandwich theorem — algebra-level rad^2 identification (Demo 51)
3. Catalan trace theorem and window formula (Demo 51)
4. Fibonacci rank theorem, now verified through cubic number fields (Demo 52 session, Demo 60)
5. Cross-ell radical dimension formulas: 2ell-3, ell^2-ell-3 — **FORMALLY PROVEN** (`proofs/radical-dimension-formula.md`)
6. Forward DKC — bracket values compute Boolean functions (Demo 29)
7. Four-tier NPN hierarchy by sector count (Demo 50)
8. 11/13 half-plane theorem with analytical proofs (Demos 61, 62)
9. The entire DKC framework connecting Habiro/Nazer-Gastpar/Abramsky/Aizenberg
10. Topology-frequency correlation reversal under activation change (Demo 47)
11. Quotient isomorphism TL_n(0)/rad ~ TL_{n-1}(0) with TL-relation verification (Demo 38)
12. Cellular sector decomposition with d(n,j)^2 size verification and cell filtration proof (Demo 39)
13. Number field degree independence of radical structure — cubic wall confirmation (Demo 60)
14. Markov RT truncation theorem — Markov kernel = J(A) + negligible ideal, **FORMALLY PROVEN** (`proofs/markov-rt-truncation.md`)
15. Next-level radical formula rad(TL_{ℓ+1}) = ℓ²−ℓ−3, **FORMALLY PROVEN** (`proofs/next-level-radical-formula.md`)
16. Parity ceiling for Z[zeta_8] is n=5 — extends DKC to 4-input (k=8) and 5-input (k=15) (Demo 63)
17. Oriented matroid classification of DKC — alternating OM type required for parity (Demo 63)
18. Two types of impossibility — constraint-geometric vs pigeonhole wall (Demo 63)
19. Matroid minor chain theorem — {XOR2..XOR5} is deletion-contraction closed with 100% downward coverage (Demo 64)
20. Self-doubling impossibility — first analytic mechanism for the XOR6 wall; scalar doubling preserves class-1, contradicts required class-0 (Demo 64)
21. Minimum wall distance d=4 — exhaustive proof that XOR6 closest-miss errors are at least 4 Hamming bits (Demo 64)
22. Parity ceiling is algebra-independent — identical tier thresholds at Z[zeta_8] and Z[zeta_16]; gap-of-2 hypothesis refuted (Demo 65)
23. Two-layer wall — absolute geometric obstruction at k≤23 / labeling convention at k≥24; XOR6 achievable with generalized activation at k=24 (Demo 65)
24. Zero margin universality — all generalized XOR6/XOR7 solutions land exactly on lattice sector boundaries (Demo 65)
25. 24-cell is the braid image — SU(2) braid reps produce exactly the binary octahedral group / 24-cell vertices (Demo 66)
26. Universal 7/7/11 structure — every 24-cell Voronoi XOR6 solution uses exactly 7+7+11 cells (Demo 66)
27. F4 decomposes to 6 orbits in two families (edge-dominant / body-dominant) (Demo 66)
28. Hopf phase is computationally inert — all DKC information lives on the S² Hopf base, the S¹ fiber carries nothing (Demo 67)
29. S² efficiency — 14-cell S² Voronoi outperforms 25-cell S³ Voronoi for XOR6 (Demo 67)
30. Intrinsic curvature theorem — Euclidean metric gives 0 solutions; S² metric gives 36; curvature is essential (Demo 68)
31. Direction nesting theorem — ζ_N eigenvector directions nest exactly into ζ_{2N}; algebraically proven, computationally confirmed for ζ_8→ζ_16 (Demo 69)
32. The 13=13 Theorem — minimum bandwidth l=6 because 2l+1=13=number of eigenvector directions; sharp phase transition (Demo 71)
33. Spectral universality — all 36 XOR6 solutions share the same spectral envelope with l=6 dominant at ~80% (Demo 71)
34. Compressed sensing on S² — 3.5× super-Nyquist compression of the Voronoi explained by sparsity + known support (Demo 71)
35. Anti-correlation theorem — better spherical sampling quality (t-design) produces fewer XOR6 solutions; algebraic structure beats geometry (Demo 72)
36. DOF-not-Design principle — classical t-design/Nyquist theory is irrelevant to DKC; the DOF coincidence 2l+1=13=directions plus compressed sensing governs bandwidth (Demo 72)
37. DKC is additive not multiplicative — multiplicative product is structurally blind to parity; computation requires additive weight sums (Demo 73)
38. 82.8% universal determinism — structural constant of the ζ_8 system, identical across all 36 XOR6 winners; sourced in magnitude loss ("compass without odometer") (Demo 73)
39. Incomparability theorem (bracket vs cell) — 119 combined equivalence classes vs 100 bracket-alone; cell and bracket access orthogonal information; DKC is bracket-blind computation (Demo 74)
40. 6 computational types — binary octahedral symmetry collapses 14 cells to 6 distinct computational roles (Demo 74)
41. Angle subsumption — eigenvalue angle subsumes Voronoi cell; 2.3× capacity gain from binocular (S¹ × S²) over cell-only; 7 discrete sum angles including 3 non-canonical (Demo 75)
42. √n magnitude quantization — |S|² ∈ {0,1,2,3,4,6}, √5 unreachable, from 24-cell inner product structure (Demo 75)
43. Activation determines capacity — same ζ_8 catalog gives 0 XOR8 at S²-Voronoi, 6 XOR8 at S¹×S² combined; activation is the capacity gate (Demo 77)
44. Paired quaternion structure for XOR8 — every winner = XOR6 triple + shadow sharing direction; all 6 winners decompose this way (Demo 77)
45. ζ_8 wall theorem — XOR10 = 0 exhaustively; finite group ceiling at 24 elements; XOR8 is the maximum (Demo 78)
46. Non-monotonicity in sector count (XOR10) — pi/4 incommensurability recurs; k=12 achieves 96.9% vs k=10 at 98.4% (Demo 78)
47. Finite group ceiling theorem — finite quaternion group imposes hard capacity ceiling; infinite group has no ceiling; ζ_12 breaks the ζ_8 wall (Demo 79)
48. Only ζ_4 and ζ_8 finite under two-generator SU(2) construction; ADE type E₇ (ζ_8) and Binary Dihedral (ζ_4); ζ_6 infinite despite sitting between them (Demo 80)
49. Quantum dimension vanishing at ζ_8 — [2]_q = 0 exactly; finite groups are the lattice (singular) points of quantum group parameter space (Demo 80)
50. Logarithmic scaling law — max_xor ≈ 0.62 × log₂(catalog_size) + 4.6 for ζ_12; each +2 XOR costs ~10× more entries (Demo 81)
51. Linear depth law — max_xor ≈ depth + 6; crossing depth is the fundamental variable; Demo 81 logarithmic law is a corollary (Demo 82)
52. Algebraic coherence beats vocabulary — deep-564 (476 dirs, 19 angles) reaches XOR12; strided-564 (512 dirs, 43 angles) reaches only XOR10; shared algebraic structure dominates geometric coverage (Demo 82)
53. Framing = +2 XOR levels — Jones normalization costs exactly 2 XOR levels at ζ₈ and ζ₁₂; constant loss despite 100× different group sizes; bracket genuinely exceeds Jones capacity (Demo 83)
54. Angular vocabulary is the mechanism — framing loss maps entirely onto angle reduction (43 → 31, −28%) while directions are unchanged (512 → 512); writhe phase rotates in the angular coordinate only (Demo 83)
55. Writhe and depth are independent computational axes — r = 0.139 near-zero correlation at 4096 entries; chirality structure is not determined by crossing length (Demo 83)
56. Resource decomposition conjecture — lattice base (XOR6) + 2 × depth_rounds + 2 × framing; three independent additive resources each contributing exactly +2 XOR levels (Demo 83)
57. TQFT anomaly is computational resource — the first term TQFT discards (writhe phase) is the term DKC uses for 2 additional XOR levels; TQFT's maximally degenerate point δ=0 discards computation first (Demo 83)
58. Null indispensability theorem — bracket-null entries in ζ₈ cannot be removed without destroying XOR capacity; non-null-only (15 entries) achieves only XOR6; non-null-only performs below random-15-entry baseline (Demo 84)
59. Null directional efficiency — 1.00 dir/entry for nulls vs 0.47 for non-null entries; nulls hold exclusive geometric territory (6 cube-edge-midpoint directions) unavailable to non-null entries (Demo 84)
60. S¹ flatness of null-only — all ζ₈ nulls have half-angle exactly 90°; k_sec sweep flat across k=1,2,4,6,8; nulls are pure S² resources with zero S¹ angular vocabulary (Demo 84)
61. Logarithmic partner hypothesis — LCFT null/partner structure (Gurarie 1993) is consistent with all null-geometry results; 6 null-only directions have no logarithmic partner and their removal destroys capacity (Demo 84)
62. Null fraction dilution conjecture — ζ₄=75%, ζ₈=37.5%, ζ₁₂=3.0%; null fraction decreases toward zero in infinite groups; null indispensability is a finite-group boundary effect (Demo 84)
63. Indecomposability parameter b = -5/8 from first principles — exact leading-coefficient extraction at TL_4 via delta-parameterized Gram matrix; resolves GRS/PR normalization controversy (Demo 85)
64. Standard modules have no Jordan blocks at delta=0 — falsifies Gemini's claim; non-semisimple structure lives exclusively in projective covers (Demo 85)
65. Delta-form divergence on P_{0,0} — novel negative finding; multiplicity from regular rep is structurally essential, not a removable scale factor (Demo 86)
66. Null dispensability at infinite groups — ζ₁₂ nulls are directionally indispensable but computationally dispensable; regime transition from ζ₈ where nulls are critical (Demo 87)
67. Two-role direction theorem — non-null body diagonals are rigid computational anchors (8% loss under perturbation); null edge midpoints are flexible scaffolding (-2.4% = gain under perturbation) (Demo 88)
68. K-ladder resilience — Sec×Vor activation provides 14× better perturbation resilience than Voronoi-only (6.4% vs 89% loss); dual benefit of capacity + robustness (Demo 88)
69. Axis cancellation mechanism — depth law explained: XOR winners select anti-aligned quaternion pairs (min_dot -0.75 vs -0.65 random); partial cancellation produces discriminating low sum angles (Demo 90)
70. Cross-depth algebraic constraint — BFS ancestry constrains sum-angle vocabulary: gen×deep = only 73 distinct angles from 50K pairs vs 1313 for deep×deep; the concrete "algebraic coherence" (Demo 90)
71. Spectral inversion — S² point cloud bandwidth DECREASES with depth (BW_90% l=4→l=2); positional information degrades while computation improves; rules out ALL positional explanations (Demo 90)
72. Relational computation thesis — depth law is relational (axis cancellation + cross-depth constraints), not positional; spectral inversion is the cleanest evidence (Demos 89-90-91)
73. Parity-Lock Theorem — ±q encoding structurally locked to XOR/XNOR only; AND/OR/MAJ impossible via 3^k equivalence class argument — **FORMALLY PROVEN** (`proofs/parity-lock-theorem.md`) (Demo 92)
74. Balanced exponentials linearity — BFS ~2×/round vs parity 4×/weight balances to linear depth law; confirmed from supply side (D90) and demand side (D91)
75. Complement-Blindness Theorem — **PROVED**: under 1wpi encoding with combined_cell, mask m produces sum S and mask ~m produces -S; |qa| normalization maps both to the same cell, so f(m) must equal f(~m) for any achievable f; AND, OR, MAJ, and XOR-at-odd-N all violate this; at odd N ALL standard Boolean functions are killed (Demo 93)
76. Phase-Cell Recovery — **DEMONSTRATED**: removing the qa sign flip (phase_cell) breaks S↔-S symmetry; complement-pair sharing drops from 100% to 1.1%; all 13 NPN classes recovered at N=3 (12/13 at depth 0, last at depth 1) (Demo 93)
77. Circuit Complexity Hierarchy in DKC — **DEMONSTRATED**: AND (AC⁰) degrades gracefully 97%→42% (N=3→8); MAJ (TC⁰) shows cliff at N=8 (2.4%); XOR (parity, not in AC⁰) killed by pigeonhole at N=7 (0.02%) and N=8 (0%); AND/XOR ratio explodes 1.01→∞ for N=3→8; Fourier degree determines geometric complexity of required partition (Demo 93)
78. Pigeonhole Mechanism — **DEMONSTRATED**: with 84 cells at depth 1, XOR requires balanced bisection of 2^N masks into cells; when 2^N > cells, impossible; AND requires only 1 unique cell for mask 1...1; this IS the circuit depth bottleneck in finite discrete systems (Demo 93)
79. Solvability Bottleneck — **CONFIRMED** computationally: at matched catalog size (24 entries each), non-solvable 2I consistently outperforms solvable z8 for XOR, advantage growing with arity (1.07× at N=3 to 1.67× at N=6); confirms Barrington's theorem prediction that non-solvable groups are strictly more powerful for branching program computation (Demo 94)
80. Circuit Complexity Hierarchy Universality — **CONFIRMED**: the AND/XOR ratio explosion (AND easy, XOR hard, ratio grows exponentially with arity) occurs in ALL tested group structures — z8, 2I, zeta_12; it is a property of the readout mechanism (1wpi + phase_cell), not the group (Demo 94)
81. Crossover Hypothesis — **DEMONSTRATED**: at small N, dense/infinite groups (zeta_12) beat finite groups via angular variety; at large N (computational boundary), non-solvable finite 2I overtakes truncated zeta_12 despite fewer entries; non-solvability and density contribute differently to capacity; crossover at N=6 (Demo 94)
82. DKC as Discrete Algebraic Reservoir Computer — **PROPOSED**: precise (not metaphorical) mapping: fixed catalog = reservoir, Cayley graph = connectivity, signed sum = linear readout, activation = nonlinear output, BFS depth = memory depth; testable prediction: rank(K_2I)/rank(K_z8) > 120/24 if non-solvability contributes above raw size (Demo 94)
83. Cross-Layer Synergy Mechanism — **DEMONSTRATED**: neither commutator subgroup (COMM) nor non-commutator coset (NON-COMM) of z8 carries disproportionate XOR capacity alone; capacity emerges from mixing algebraic strata; Barrington mechanism works through CROSSING between derived series layers, not through commutators alone (Demo 95)
84. Solvability Bottleneck via Stratification — **REFINED**: solvable groups waste combinatorial budget on cross-layer constraints; derived series layers force cross-layer dependence; 2I's perfection (G1=G0, every element a commutator) avoids this; solvability bottleneck operates THROUGH stratification (Demo 95)
85. 2I Perfect Group — **CONFIRMED** computationally: 60/60 binary icosahedral bracket values are single commutators; derived series terminates immediately (60=60); matches Liebeck-O'Brien-Shalev-Tiep (2011) prediction for SL(2,5) (Demo 95)
86. 90-Degree Algebra-over-Geometry — **DEMONSTRATED**: within the same half-angle (90°), algebraic origin creates computational capacity; Q8 entries (D2) and outermost-shell entries (D0) are geometrically identical but algebraically distinct; neither alone produces XOR, their union does (0+0=26% at N=4); algebraic depth is the active ingredient (Demo 95)
87. Synergy Universality Conjecture — **PROPOSED**: D84 synergy (TL null/non-null = 0+0=36) and D95 synergy (derived series layers = 0+0=26%) may be the same algebraic split seen through different lenses (Demo 95)
88. Two Independent Stratifications — **CONFIRMED**: bracket-null/non-null partition (9/15 from D84) and derived series partition (12/8/3/1 from D95) cut ACROSS each other, creating genuine 2D classification of z8; confirmed via 2O character table analysis (Kirillov-Ostrik 2002); independence at element level, unity at categorical level via ADE/E7 Dynkin diagram (Demo 96)
89. Optimal z8 Catalog is A+B+C (21 entries, not 24) — **DEMONSTRATED**: removing 3 Q8-deep-null entries (Cell D) IMPROVES capacity at every N tested; A+B+C beats ALL(24) by +2-4% across N=3-6; Q8-null entries are pure noise that actively degrades computation (Demo 96)
90. Cell B Perfection — **DEMONSTRATED**: Cell B (D0-nonnull, 6 entries, 3 mutually orthogonal directions at 45° half-angle) achieves 100% XOR at EVERY N=3 through 6; non-null is necessary AND sufficient for individual capacity; among non-null cells, 45° (B) outperforms 60° (C) (Demo 96)
91. Cross-Derived Synergy Principle — **CONFIRMED**: non-null entries from DIFFERENT derived levels (B from D0, C from D1) produce maximum synergy; B+C dominates all 6 pairwise combinations at every N tested (Demo 96)
92. Orthogonal-Frame Perfection Theorem — **CONFIRMED** computationally: Cell B's 100% XOR perfection arises from geometric inevitability of 3 mutually orthogonal directions, not algebraic fine-tuning; the 45° half-angle is the midpoint of a ~50-degree robust plateau (25°-75°) where ANY half-angle gives identical 100% results; perfection is the octahedron (Demo 97)
93. Lever Arm Mechanism — **DEMONSTRATED**: nonzero real component of quaternion provides the "lever arm" for phase_cell to distinguish XOR=0 from XOR=1 signed sums; at 90° (null), real component = 0, lever arm vanishes, total separation collapse; geometrically explains why null entries are dead and non-null entries are load-bearing (Demo 97)
94. 50-Degree Robustness Plateau — **DEMONSTRATED**: full perfection (100% XOR at all N through 6) holds for any half-angle in the 25°-75° range; N=3 perfection extends from 10°-85°; 90° = total collapse; isolated anomaly at 35° (N=5 only drops to 0%) remains unexplained (Demo 97)
95. Readout Bottleneck Thesis — **DEMONSTRATED**: 3-strand TL algebra contains MORE computational structure than 2-strand (infinite group, 2048 entries at cap, 98.5% interleaving) but standard trace readout is catastrophically lossy — ZERO XOR6 at any k_sec; trace collapses 20-integer column vector (100% retention at column 4) to 4-integer trace (5.1% retention); readout, not algebra, is the bottleneck (Demo 98)
96. Column-4 Losslessness — **NEW FINDING**: column 4 of the TL_3 5×5 representation preserves 100% of algebraic diversity (2048 distinct vectors from 2048 matrices, 20 integer DOFs); 11-20× more diversity than trace readout; directly seeds multi-strand activation design (Demo 98)
97. Delta_1 XOR Capacity — **LANDMARK**: first-ever 3-strand XOR computation at ALL levels; XOR6=500,261, XOR8=48,143, XOR10=485, XOR12=16, XOR14=60 (best across activations); key was irreducible 2×2 standard module Delta_1 over Z[zeta_8] with 16-component sign-pattern activation (2x2_hash); XOR16=0 is genuine ceiling for this representation (Demo 99)
98. Ext^1 Catalytic Preparation Thesis — **ESTABLISHED**: 3-strand DKC at delta=0 lives on the non-trivial extension Ext^1(L(0), L(1)) of restricted quantum group u_q(sl_2) at ell=2; non-split extension is the computational resource; end-to-end causal chain: Ext^1 → complementary pairing → high-C sums → eigenvalue discrimination → Boolean computation (Demo 99)
99. Two-Regime Casimir Hypothesis — **CONFIRMED**: combinatorial regime (XOR6-8) uses nilpotent-proximate, diversity-driven computation with shallow entries (Casimir INVERTED: winners lower); algebraic regime (XOR10-14) uses Ext^1 catalytic preparation with deep entries required (Casimir 3.3×-5.3× higher in winners); phase transition at XOR8→XOR10 coincides with shallow→deep entry transition (Demo 99)
100. Fibonacci Growth in Delta_1 — **OBSERVED**: max_abs of catalog entries follows exact Fibonacci sequence (1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233); connected to golden ratio φ through spectral radius of trace-zero SL_2 elements and the 2×2 transfer matrix; Jordan block drives infinite group (sigma_1^8 ≠ I despite diagonal cycling with period 8) (Demo 99)
101. Star-Graph XOR Topology — **DEMONSTRATED**: XOR14 winners form a star graph with 2 irreplaceable super-hubs (idx=7 and idx=9, both d=6, 100% frequency) + rotating satellites from ~12 d=7 entries; 0/1 and 0/7 substitution survival rates; structurally different from 2-strand Cell B's mutual orthogonality graph (Demo 99)
102. Non-Semisimple W_{4,2} Capacity — **DEMONSTRATED**: 4-strand B_4 acting on 3-dimensional W_{4,2} (non-semisimple, radical dim=9, Loewy length 3) produces infinite catalog with ~3.1× BFS growth per round; sustained XOR6-XOR14 capacity through depth (XOR14=70 at d≥4); every braid matrix preserves radical content (constant mean=2 at all depths) (Demo 100)
103. Constant Radical Content — **CONFIRMED**: all W_{4,2} catalog entries have identical mean radical_content=2 regardless of BFS depth; the non-semisimple extension structure (0→L_{4,4}→W_{4,2}→L_{4,2}→0) is equally present in every braid matrix (Demo 100)
104. Casimir Exponential Growth with Depth — **OBSERVED**: mean |C3| grows approximately as 2^d (doubling per round) while radical content stays constant; deeper entries are more algebraically complex without changing module-theoretic character; Casimir consistently 1.36×-1.86× higher in XOR winners across all levels tested (Demo 100)
105. Hub Pairing by Cyclotomic Component — **OBSERVED**: 6 super-hubs in W_{4,2} pair by Cyc8 basis element dominating their trace: {h0,h1} in ζ₈², {h2,h5} in ζ₈³, {h3,h4} in ζ₈; all six hub commutators traceless with zero radical content (commutators annihilate radical direction); hub topology is dense graph (121 edges), not pure star (Demo 100)
106. sl_d Growth Rate — **CONFIRMED** across three data points: BFS growth rate converges to n-1 for n-strand braid on standard module; 3-strand ~2.2× (sl_2), 4-strand ~3.1× (sl_3), 5-strand ~4.0× (sl_4); the growth rate tracks the number of TL generators (n-1) (Demos 98, 100, 101)
107. Hub Count Prediction — **FALSIFIED**: sl_d functor thesis predicted 2×C(n-1,2) super-hubs (6 for sl_3, 12 for sl_4); D100 confirmed 6 hubs for sl_3 but D101 found only 3 hubs for sl_4 (not 12); discrepancy may stem from simple-module context (W_{5,3} has no radical vs W_{4,2} non-semisimple) (Demo 101)
108. Casimir-XOR Inversion in Simple Modules — **NEW FINDING**: Casimir-XOR correlation INVERTS at higher XOR levels in simple module W_{5,3}; XOR6 ratio 1.17× and XOR8 ratio 1.51× (winners higher, matching D100), but XOR10 ratio 0.89× and XOR12 ratio 0.54× (winners LOWER); in simple modules, hardest XOR computations select for near-scalar (low Casimir) entries; opposite of D100's non-semisimple pattern where all ratios >1 (Demo 101)
109. Simple vs Non-Semisimple Computational Difference — **DEMONSTRATED**: W_{5,3} (simple, 4×4) has broader shallow capacity (MORE XOR6-8 than D100) but strictly less deep capacity (XOR14=0 vs 70 in D100); the radical extension in W_{4,2} is computationally load-bearing at the XOR14 boundary, not decorative; simple modules compute differently, not just "minus the radical" (Demo 101)
110. Traceless Hub Commutators Universality — **CONFIRMED** across 4-strand and 5-strand: all hub commutators are traceless in both D100 (6 hub pairs) and D101 (3 hub pairs); D101 goes further — all 3 hub commutators are the ZERO MATRIX (hubs mutually commute), living in a commutative subalgebra of Mat_4(Z[zeta_8]) (Demos 100, 101)
111. Barrington-Radical Principle — **NAMED THEOREM**: radical of non-semisimple TL module carries abelian writhe character; by Barrington's theorem, abelian groups can't compute parity; radical is provably useless for XOR; simple module W_{6,0} beats non-simple W_{6,4} at every XOR level (Demo 102)
112. Writhe Character Theorem — **PROVEN** computationally: M*r = A^{writhe(M)}*r for ALL 32,768 W_{6,4} catalog entries; radical is 1D braid character carrying only writhe/framing (Demo 102)
113. BFS Growth is a Braid Group Invariant — **PROVEN** for n=6: three modules (W_{6,0}, W_{6,2}, W_{6,4}) produce identical BFS trees; growth rate tracks strand count n-1, not module dimension; eliminates dim(W) law hypothesis (Demos 102, 103)
114. Curse of Dimensionality in Sign-Hash — **CONFIRMED**: higher-dimensional modules produce worse XOR at fixed k due to sparser hash space; W_{6,0}(dim 5) beats W_{6,2}(dim 9) at every XOR level at k=128; partially recoverable via optimal component selection (Demos 103, 104)
115. Atkinson Sweet Spot — **DEMONSTRATED**: non-monotonic component-count curve with peak at ~120 of 324 components for W_{6,2}, beating W_{6,0}'s 2449 at XOR6=2486; optimal information-per-collision tradeoff in sign-hash quantization (Demo 104)
116. Branching Interaction Thesis — **CONFIRMED** at n=6, **REVERSED** at n=8: cross-block (off-diagonal) components dominate for small sub-module blocks (dim 4-5), within-block dominates for complex blocks (dim 14); regime transition = Macrame Principle (Demos 104, 105)
117. Sign-Rank Expansion — **NEW FINDING**: sign quantization (ternary projection) INCREASES effective rank beyond raw Z-linear rank; W_{6,2}: raw 244, sign 292 (+48); W_{8,2}: expansion 1.83x and growing; the nonlinear sign() function breaks linear dependencies (Demos 104, 105)
118. k-Regime Theory — **CONFIRMED** at n=8: k/2^N ratio controls XOR capacity; D93's "XOR dies at N>=7" REFUTED as pigeonhole artifact; W_{8,0} at k=4096 produces XOR8=22266, beating n=6 (Demo 105)
119. Macrame Principle — **CONJECTURED**: branching interaction dominates for simple sub-module blocks (dim 4-5); within-block dominates for complex blocks (dim 14); crossover regime between dim 5 and dim 14 (Demo 105)
120. TL Visibility Filter — **DEMONSTRATED**: topological entropy (Thurston-Nielsen) is completely orthogonal to DKC; both periodic and pseudo-Anosov braids compute XOR at 100% participation; TL quotient at delta=0 erases dynamical info while preserving algebraic structure (Demo 106)
121. Z/4Z Axis-Alignment Theorem — **PROVEN**: every braid matrix entry over TL module at delta=0 is axis-aligned; constructive phase formula Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4} (Demo 107)
122. Nesting Parity Z/2Z Grading — **PROVEN** computationally (n=2..6, 195 matchings, 0 mismatches): bipartite 2-coloring = nesting count mod 2; NOT a consequence of known KLR Z-grading (Demo 107)
123. Major Index Bipartite Invariant — **CONJECTURED** for general n,j: for TL standard modules W_{n,j}, maj mod 2 = BFS bipartite coloring; verified at W_{4,2}(3/3) and W_{6,2}(9/9); generalizes nesting parity (Demo 107)
124. q-Catalan Identity — **PROVEN** algebraically: C_{2m+1}(-1) = (-1)^m * C_m; gives exact bipartition class sizes for TL full-matching link-state graphs (Demo 107)
125. Orthogonality Theorem (mult vs add closure) — **PROVEN** computationally: multiplicative quotient graph = K_5 (Z/4Z), additive quotient graph = star tree; multiplication rotates axes, addition preserves axes (Demo 107)
126. Dual-Channel Theorem — **NOVEL**: parity requires BOTH multiplicative phase coherence (product closure > 0) AND additive magnitude diversity (v_2 connectivity > trivial); maximum contrast: parity 17 edges (connected) vs poison 0 edges (empty); maps onto T-gate/Hadamard decomposition (Demo 108)
127. Encoding-Dependent Dual-Channel Theorem — **NOVEL**: dual-channel polarity flips with encoding type; additive encoding wants HIGH product closure, multiplicative encoding wants LOW; confirmed by D108+D109 paired experiment (Demos 108, 109)
128. Perfect Algebraic Symmetry at delta=sqrt(2) — **NOVEL**: parity and non-parity vocabularies structurally identical on 7/8 edge types; product closure is the sole discriminator (14.8% vs 46.6%); Z[zeta_16] algebra too rich for Raqiya to distinguish except through products (Demo 109)
129. Partition Coincidence as Incapacity Signature — **NOVEL**: triple partition collapse (root=Galois=norm) is a structural signature of computational incapacity; parity has three distinct levels, poison collapses to one (Demo 108)
130. Resolution Dependence of Computational Role — **NOVEL**: same Z[zeta_8] value can be parity-capable at k=6 and poison at k=15; 14 values have dual classification; computational role is activation-dependent, not value-intrinsic (Demo 108)
