# Dead Ends & Wall Patterns

Negative results registry and "wall was X not Y" pattern catalog for the DKC research program.
Updated 2026-02-26: Covers 109 demos.

**Purpose:** Prevent duplicate work. Every dead end narrows the search space. Every wall resolution teaches a generalizable principle. Before attempting something new, check if the approach has been tried and what specific mechanism caused the failure.

---

## Part 1: Dead Ends (Negative Results Registry)

Organized by research question attacked.

---

### A. Parity Reachability

#### DE-01: Split-Sigmoid Cannot Compute Parity (D48)

- **What was tried**: Exhaustive search of 100,000,000 exact Z[zeta_8] bracket quartets (w1, w2, w3, bias) with split-sigmoid activation for 3-input parity (XNOR3).
- **Why it failed**: Split-sigmoid treats Re and Im independently, producing weights whose magnitudes align with bracket magnitudes but whose phases are essentially random relative to Z[zeta_8]. A k=2 half-plane classifier provably cannot compute 3-input parity with ANY complex weights (complex Minsky-Papert: if y1>d and y2>d then y1+y2>2d>d, contradicting the parity requirement).
- **What it taught us**: The wall is in the activation geometry (number of decision sectors), not the weight space. The Z[zeta_8] lattice contains parity-capable weights (Aizenberg's construction: zeta_8, i, -1 are all in the catalog). The wall is razor-thin: EXACT1 is 1 Hamming bit away and has 127K solutions.
- **Demo reference**: D48
- **Do not re-attempt unless**: You change the activation function (k-sector MVN was the resolution — D50) or move to a fundamentally different readout.

#### DE-02: Neglecton Weight Expansion (D49)

- **What was tried**: Extract radical elements of TL_4 at delta=0 via first-order bracket perturbation. Use the 148 new Z[zeta_8] values (from 15,242 previously invisible braids) to break the parity wall. Searched 2.18 billion quartets with the combined 216-value catalog.
- **Why it failed**: All 116 genuinely new neglecton values share angles with existing standard values — zero new angular directions at any perturbation order. The perturbation approach is closed at all orders because higher-order terms (loops=3,4,...) also produce A^(a-b) sums scaled by real factors, preserving the same angular structure. The parity wall is in activation geometry, not weight-space angular vocabulary.
- **What it taught us**: The weight space already contains everything parity needs. Adding more values at the same angles cannot help. The solution is changing what the activation reads (angular sectors vs half-plane), not what the catalog provides.
- **Demo reference**: D49
- **Do not re-attempt unless**: You find a perturbation that introduces genuinely new angular directions (proven impossible for the delta-perturbation approach).

#### DE-03: XNOR3 Split-Sigmoid is Exactly Zero (D48)

- **What was tried**: Test whether XNOR3 has any solutions at all with split-sigmoid activation and Z[zeta_8] weights.
- **Why it failed**: Exactly 0 out of 100,000,000 quartets. Not approximately zero — exactly zero. The only NPN class with this property.
- **What it taught us**: The parity function is uniquely singled out by the split-sigmoid activation. Every other NPN class (12/13) has solutions. EXACT1 (Hamming distance 1 from parity) has 127K solutions. The algebra reaches within 1 bit flip but cannot cross.
- **Demo reference**: D48
- **Do not re-attempt unless**: You change the activation function.

#### DE-04: Triskelion k=2n Generalization (D63)

- **What was tried**: Predict that n-input parity requires k=2n sectors (generalizing 3-input parity at k=6).
- **Why it failed**: 5-input parity requires k=15, not k=10. The triskelion (one weight per odd sector at 120-degree intervals) is specific to 3-input parity. 4-input uses tetraskelion at k=8, 5-input uses pentaskelion at k=15.
- **What it taught us**: The parity scaling law is more nuanced than simple doubling. k=2M-1 (for M=8 rays) is the universal k that places every non-zero ray in an odd sector. The geometric structure changes qualitatively at each arity.
- **Demo reference**: D63
- **Do not re-attempt unless**: Working with a different lattice geometry (different M).

#### DE-05: Parity Ceiling Beyond n=5 at Z[zeta_8] (D63)

- **What was tried**: Find n=6 or n=7 parity solutions at Z[zeta_8] with any sector count k=2..16.
- **Why it failed**: Two distinct mechanisms. For n=6,7: constraint-geometric wall — enough octants available (7>=6) but the 2^n-1 sector-parity constraints are simultaneously unsatisfiable. For n>=8: pigeonhole — only 7 class-1 octants, need distinct rays. The gap between pigeonhole bound (n<=7) and actual ceiling (n<=5) is the "geometric surplus."
- **What it taught us**: Distinguishes combinatorial impossibility (pigeonhole, trivial) from geometric impossibility (constraint wall, non-trivial). The Z[zeta_8] parity ceiling is n=5, period.
- **Demo reference**: D63
- **Do not re-attempt unless**: Using a different cyclotomic ring (but D65 showed the ceiling is algebra-independent: Z[zeta_16] also has n=5 ceiling).

#### DE-06: Clifford Staircase Conjecture — Standard Activations (D65)

- **What was tried**: Predict XOR6 becomes achievable at Z[zeta_16] (next cyclotomic ring) with standard odd/even sector activations.
- **Why it failed**: XOR6 = 0 solutions at ALL k=2..31 with both 36-value and 56-value Z[zeta_16] catalogs under standard activations. The parity ceiling n=5 is algebra-independent.
- **What it taught us**: Moving to a richer cyclotomic ring does not help under standard activations. The ceiling is structural, not algebraic. However, generalized activations (arbitrary binary sector labeling) DO break the wall at k=24 — revealing a two-layer structure separating geometric obstruction from labeling convention.
- **Demo reference**: D65
- **Do not re-attempt unless**: Using generalized (non-standard) activations (k=24 works) or moving to the quaternionic S^2 framework.

---

### B. Indecomposability Parameter b

#### DE-07: Direct b on Single P_{0,0} (D86)

- **What was tried**: Compute the indecomposability parameter b via delta-parameterized Gram matrix on a single projective cover P_{0,0} (instead of the full regular representation used in D85).
- **Why it failed**: Universal divergence. The valuation condition p_tt = 2*p_Tt never holds on a single P_{0,0}. Tested at every eigenvalue, every lattice size (TL_4, TL_6), and for both loop and trace forms. All diverge with negative gap (p_tt < 2*p_Tt). The dim(V_0)^2 multiplicity from the regular representation participates in the valuation balance — it is structurally essential, not a removable scale factor.
- **What it taught us**: The regular representation's multiplicity is not just a normalization convention. TL_8's P_{0,0} is completely semisimple at lambda=0 — no Jordan blocks at all. The non-semisimple structure needed for b computation does not persist on single copies at larger lattice sizes. Projective cover dimensions follow a Catalan pattern: dim(P_{0,0}) = C_{n/2+1}.
- **Demo reference**: D86
- **Do not re-attempt unless**: You find a fundamentally different bilinear form or representation choice, or implement the actual Pearce-Rasmussen transfer-matrix finite-size scaling method.

#### DE-08: Spin Chain Approach for b (D85)

- **What was tried**: Extract b from the spin chain representation on (C^2)^{tensor n} at q=i, using the conformal (transpose) bilinear form.
- **Why it failed**: The fixed-point trace bilinear form G[i][j] = fixpt(star(i)*j) is degenerate at delta=0 — almost all products create loops that evaluate to 0. At TL_4 it gives an affine b with 3-dimensional gauge freedom (M=0 quadratic coefficient matrix, no unique critical point). Both b=-2 and b=-5/8 are reachable on this flat manifold. At TL_6 it gives all zeros.
- **What it taught us**: The fixed-point form has gauge freedom that the delta-parameterized form resolves. The delta-parameterized Markov trace (D85's successful approach) is the correct form because it is non-degenerate at delta!=0 and the leading-coefficient extraction as delta->0 gives the physical b.
- **Demo reference**: D85
- **Do not re-attempt unless**: You can break the gauge degeneracy by a mechanism other than delta-parameterization.

#### DE-09: Valuation Condition Failure at TL_6 (D85)

- **What was tried**: Extend the b=-5/8 computation (which succeeded at TL_4) to TL_6 via the regular representation.
- **Why it failed**: At TL_6: p_tt=4, p_Tt=3, so 4 != 2*3=6. The valuation condition fails. b diverges as delta^{-2}. All 14 kernel directions introduce lower-power cross-terms; 0 safe directions exist. The 25-fold multiplicity (dim(0-TL for TL_6)^2 = 25) does not rescue the valuation condition as the 4-fold multiplicity did at TL_4.
- **What it taught us**: The leading-coefficient method on the regular representation is specific to TL_4. Cross-copy interference grows faster than the multiplicity rescue at larger lattice sizes. The actual literature method for computing b at larger sizes likely uses transfer-matrix finite-size scaling, not the diagram-algebraic approach.
- **Demo reference**: D85
- **Do not re-attempt unless**: You develop a modified extraction that accounts for the valuation gap, or use a completely different method (transfer matrix).

---

### C. Multi-Strand DKC Readout

#### DE-10: 3-Strand Trace Readout Zero (D98)

- **What was tried**: Use the standard trace readout (Tr(M) as a single Cyc8 value) for 3-strand DKC on the full TL_3 representation (5x5 matrices).
- **Why it failed**: Trace collapses 25 Cyc8 entries (100 integers) to 1 Cyc8 (4 integers) — 96% information loss. The 3-strand bracket values are axial (single nonzero Cyc8 component), making the trace effectively 1D. Zero XOR6 at every k_sec tested (4,6,8,10,12,16,20,24,32).
- **What it taught us**: The readout, not the algebra, is the bottleneck for multi-strand DKC. 3-strand algebra is rich (infinite group, 2048 entries at cap, 98.5% interleaving, column-4 has PERFECT 100% diversity). The 2-strand pipeline succeeds because quaternions ARE the readout — SU(2) is isomorphic to S^3. For 3+ strands, the trace readout is catastrophically lossy.
- **Demo reference**: D98
- **Do not re-attempt unless**: You use a richer readout: column vectors (100% diversity at column 4), multiple traces (Tr(M), Tr(M^2), Tr(M^3)), or irreducible module representations (D99b resolved this with Delta_1).

#### DE-11: 5-Strand Simple Module XOR14=0 (D101)

- **What was tried**: Achieve XOR14 on the simple standard module W_{5,3} (dim=4, radical dim=0) of TL_5 at delta=0.
- **Why it failed**: XOR14=0 at ALL k values tested (k=256 through k=4096) on the deep sub-catalog. The simple module has broader but shallower capacity: it beats W_{4,2} at XOR6-XOR8 (2579 vs 2017, 2228 vs 1169) but loses at XOR10-XOR14 (203 vs 654, 27 vs 305, 0 vs 70).
- **What it taught us**: The radical extension in non-semisimple modules (like D100's W_{4,2}) is computationally load-bearing, not decorative. Simple modules lack the deep XOR capacity that non-semisimple extensions provide. The Casimir-XOR correlation inverts at higher XOR levels in simple modules (winners have LOWER Casimir at XOR10-12).
- **Demo reference**: D101
- **Do not re-attempt unless**: Using a non-semisimple module, or significantly increasing k to push into the collision-avoidance regime.

---

### D. Radical and Non-Semisimple Structure

#### DE-12: Radical-as-Resource Hypothesis (D102)

- **What was tried**: Predict that non-semisimple structure (radical dim >= 1) provides computational advantage for XOR. Head-to-head A/B test of W_{6,0} (simple, dim=5) vs W_{6,4} (non-simple, dim=5, radical dim=1) on the 6-strand braid group.
- **Why it failed**: Simple module W_{6,0} WINS at every XOR level. XOR6: 2449 vs 2370, XOR8: 850 vs 793, XOR10: 1 vs 0. The radical carries an abelian writhe character (M*r = A^{writhe}*r for ALL 32,768 entries). By Barrington's theorem, abelian groups cannot compute parity — the radical direction is theoretically guaranteed useless for XOR.
- **What it taught us**: The Barrington-Radical Principle. The radical is computationally inert because it carries only abelian (writhe) data. However, the non-semisimple *extension structure* in indecomposable modules IS computationally useful (D100 vs D101 comparison) — the load-bearing component is the extension, not the radical itself.
- **Demo reference**: D102
- **Do not re-attempt unless**: You find a module where the radical carries non-abelian data (unlikely for TL at delta=0 since writhe is always abelian).

---

### E. Multiplicative and Automaton Approaches

#### DE-13: Multiplicative Braid Product for XOR6 (D73)

- **What was tried**: Compute XOR6 by assigning one braid per input bit and taking the 6-step braid product sigma_1^{+/-1} * sigma_2^{+/-1} * ... * sigma_2^{+/-1}, then classifying the resulting quaternion's eigenvector cell.
- **Why it failed**: Every final cell contains exactly 8 even-parity and 8 odd-parity inputs — maximally non-separating. The algebraic reason: complementing all input bits maps the braid matrix M to its inverse M^{-1}, which shares the same eigenvector cell (antipodal identification). The braid product is structurally parity-blind.
- **What it taught us**: DKC is fundamentally additive (weight vector sum), not multiplicative (braid matrix product). This retroactively explains why all successful forward DKC demos since D29 used additive weight vectors. The Nazer-Gastpar "compute-and-forward" lattice code framework is additive by design — DKC is a concrete instance.
- **Demo reference**: D73
- **Do not re-attempt unless**: You break the antipodal identification (M and M^{-1} sharing an eigenvector cell), which would require a readout that distinguishes a quaternion from its inverse.

---

### F. Geometric Optimization

#### DE-14: Voronoi Optimization Catastrophe (D72)

- **What was tried**: Improve the 13 eigenvector directions on S^2 by stochastic optimization toward a better spherical t-design (the eigenvector configuration is t=0 — the worst possible).
- **Why it failed**: Optimization to t~1 (2.6x better geometry) produces 9x FEWER XOR6 solutions (4 vs 36). Better geometric distribution destroys the computation.
- **What it taught us**: The Anti-Correlation Theorem: sampling quality and computational quality are anti-correlated for DKC. The directions are algebraically selected by the binary octahedral group, not geometrically optimized. The computation depends on specific angular relationships between directions, not uniform distribution. Moving directions to "better" positions breaks the Voronoi partition structure.
- **Demo reference**: D72
- **Do not re-attempt unless**: You constrain non-null (body diagonal) directions to within 2 degrees while letting null (edge midpoint) directions move freely (D88 showed this captures 92% of design improvement with only 2.8% XOR loss).

---

### G. Topological and Dynamical Properties

#### DE-15: Topological Entropy Prediction (D106)

- **What was tried**: Predict that pseudo-Anosov braids (positive topological entropy, chaotic mixing) compute XOR better than periodic braids (zero entropy).
- **Why it failed**: Total null. Both periodic (88 entries, mean score 3969) and pseudo-Anosov (39 entries, mean score 3857) braids compute XOR at 100% participation rate with near-identical scores. Maximum gap between periodic and pA rates across all 8 Boolean functions tested: 3.7 percentage points.
- **What it taught us**: The TL Visibility Filter. Properties visible in the Kauffman bracket (writhe) correlate with DKC output; properties invisible in the bracket (topological entropy, Burau spectral radius) do not. The TL quotient at delta=0 erases all dynamical information (e_i^2=0 kills expanding eigenvalues) while preserving algebraic structure. DKC computation is collective and relational, not individual and dynamical.
- **Demo reference**: D106
- **Do not re-attempt unless**: Working at delta!=0 where the TL quotient is less destructive (e_i^2 = delta*e_i != 0).

---

### H. Scaling and Capacity

#### DE-16: "XOR Dies at N>=7" Under Low k (D93, refuted by D105)

- **What was tried**: Conclude from D93's Phase 8 data (17 entries, 84 cells) that XOR at N>=7 inputs is fundamentally impossible for finite discrete DKC systems.
- **Why it failed**: It was a k-regime artifact. At k=128 with 84 cells, 2^N masks exceed cells at N>=7 (pigeonhole). But at k=4096, W_{8,0} produces XOR8=22266. The ratio k/2^N is the real control parameter: at k/2^N << 1 (pigeonhole regime) XOR dies; at k/2^N >> 1 (collision avoidance regime) XOR revives.
- **What it taught us**: k-Regime Theory. Never conclude a function is unreachable without sweeping k to the collision-avoidance regime. All prior "XOR dies" claims at finite N were pigeonhole artifacts of low k.
- **Demo reference**: D93 (original claim), D105 (refutation)
- **Do not re-attempt unless**: You have verified the claim holds at k/2^N >> 1.

#### DE-17: dim(W) Law Hypothesis (D102-D103)

- **What was tried**: Predict that higher-dimensional TL modules produce faster BFS growth and better XOR capacity.
- **Why it failed**: BFS growth profile is identical across all tested TL_n modules at delta=0 for the same n. At n=6: W_{6,0} (dim=5), W_{6,2} (dim=9), and W_{6,4} (dim=5, non-simple) all produce bit-for-bit identical BFS trees at every depth. Growth rate tracks strand count (n-1), not module dimension.
- **What it taught us**: BFS growth is intrinsic to the braid group B_n, not the module. Higher-dimensional modules produce WORSE XOR capacity at fixed k due to the curse of dimensionality in sign-hash activation (more sign components create sparser hash space).
- **Demo reference**: D102, D103
- **Do not re-attempt unless**: Using an activation function that is not sign-hash based and can exploit higher dimensions.

#### DE-18: Standard Module Jordan Blocks for b (D85)

- **What was tried**: Extract the indecomposability parameter b from Jordan cell coupling on standard modules (as claimed by Gemini 3 Pro).
- **Why it failed**: Standard modules W_{n,0} have NO Jordan blocks at delta=0 at any size tested (n=4,6,8,10,12). The minimal polynomial is square-free at all sizes. Jordan blocks exist exclusively on projective covers (P_{0,0}) and the full regular representation.
- **What it taught us**: The Gemini literature claim was wrong. The non-semisimple structure needed for b computation lives in the projective cover, not the standard module. Cross-model checks are most valuable as predictions, least valuable as rationalizations.
- **Demo reference**: D85
- **Do not re-attempt unless**: Working on projective covers or the full regular representation.

---

### I. Encoding and Activation

#### DE-19: Complement-Blindness Under combined_cell (D93)

- **What was tried**: Compute arbitrary Boolean functions under 1-weight-per-input encoding with combined_cell activation (which includes the `if (qa < 0)` sign normalization).
- **Why it failed**: Under combined_cell, mask m and complement ~m produce sums S and -S; the absolute-value normalization maps both to the same cell. At odd N: NO standard Boolean function survives. At even N: only XOR survives (complement-invariant). 100% complement-pair cell sharing verified exhaustively at N=3-6.
- **What it taught us**: Activation determines what is extractable from the algebra. Removing the sign flip (phase_cell) breaks complement symmetry: sharing drops from 100% to 1.1%, all 13 NPN classes recovered.
- **Demo reference**: D93
- **Do not re-attempt unless**: Using phase_cell or another phase-sensitive activation.

#### DE-20: Parity-Lock of +/-q Encoding (D92)

- **What was tried**: Compute AND, OR, MAJ, or threshold functions under the +/-q paired input encoding.
- **Why it failed**: The (0,0)=(1,1) collision: both input pair (0,0) and pair (1,1) produce zero contribution to the quaternion sum. Masks 0...0 and 1...1 always produce identical sums. Any f where f(0...0) != f(1...1) is impossible. AND/OR/MAJ/threshold all have exactly zero winners at every depth 0-8 for both 6-input and 8-input arities.
- **What it taught us**: The Parity-Lock Theorem (formally proven, P04). The +/-q encoding channels all computational power into parity — the single Boolean function outside AC^0. This is structural, not a limitation to overcome.
- **Demo reference**: D92
- **Do not re-attempt unless**: Using a different encoding (1wpi encoding computes all 13 NPN classes).

---

### J. Miscellaneous

#### DE-21: Platonic Solid Voronoi for Quaternionic XOR6 (D67, D70)

- **What was tried**: Use Platonic solid Voronoi partitions (octahedral/6 cells, icosahedral/12 cells, cuboctahedral/12 cells, dodecahedral/10 cells) for XOR6 DKC on S^2.
- **Why it failed**: All yield zero XOR6 solutions. Platonic symmetry group mismatch: icosahedral order 120 vs binary octahedral order 48. Only the data-intrinsic 13-direction eigenvector Voronoi gives 36 solutions.
- **What it taught us**: The correct symmetry group is essential, not just the cell count. Natural polytopal symmetries are wrong for DKC; only braid-intrinsic eigenvector directions work.
- **Demo reference**: D67, D70
- **Do not re-attempt unless**: Using a Voronoi derived from the actual algebraic structure (eigenvector directions of the braid group representation).

#### DE-22: Hopf Phase as Computational Signal (D67)

- **What was tried**: Use the Hopf fiber phase xi_1 (the S^1 fiber of the Hopf fibration S^3 -> S^2) as a DKC activation signal.
- **Why it failed**: Zero XOR6 solutions at all resolutions tested (k=4 through k=32, exhaustive). The Hopf fiber phase carries zero DKC computational information.
- **What it taught us**: All DKC information lives in the S^2 Hopf base (rotation axis), not the fiber (phase). The Hopf fibration exactly separates computationally active (base) from inert (fiber) information. Pearson r = -0.06 between pitch (eigenvalue angle) and spatial direction (eigenvector) — effectively zero.
- **Demo reference**: D67
- **Do not re-attempt unless**: Working at delta!=0 where the fiber-base decomposition may differ.

#### DE-23: Euclidean Metric on Stereographic Projection (D68)

- **What was tried**: Transfer DKC computation from S^2 to R^2 via stereographic projection using Euclidean nearest-neighbor distance.
- **Why it failed**: Euclidean metric yields 0 solutions; S^2 great-circle metric yields 36 (identical to D67). The computation is intrinsically curved.
- **What it taught us**: Conformality is sufficient (area distortion up to 16.5x doesn't matter when using the S^2 metric). The plane is a window onto the computation, not a workspace. "Visualize on the plane, compute on the sphere."
- **Demo reference**: D68
- **Do not re-attempt unless**: Using the intrinsic S^2 metric even when projecting to the plane for visualization.

#### DE-24: Five Depth Law Hypotheses (D89)

- **What was tried**: Five hypotheses for why deep entries compute better: (1) pairwise coherence, (2) direction coverage, (3) Cayley density, (4) sector saturation, (5) deep-shell angle coherence.
- **Why it failed**: All five killed. (1) Mean pairwise |qdot| flat at ~0.42, identical to random baseline. (2) All 13 directions covered by depth 2. (3) Same-depth entries have zero mutual Cayley edges. (4) Sectors saturate early. (5) Only shallow shells show angle coherence.
- **What it taught us**: The depth law mechanism is axis cancellation (anti-aligned rotation axes producing low sum angles) combined with cross-depth algebraic constraints from BFS ancestry. The advantage is relational not positional — the S^2 point cloud bandwidth actually DECREASES with depth (spectral inversion). D90 found the actual mechanism.
- **Demo reference**: D89
- **Do not re-attempt unless**: You have a hypothesis that is explicitly relational (about how entries combine) rather than positional (about where entries sit).

#### DE-25: sl_d Hub Count Prediction (D101)

- **What was tried**: Predict 12 super-hubs (6 pairs for sl_4 positive roots) in W_{5,3} XOR10 winners.
- **Why it failed**: Only 3 super-hubs found, not 12. All three mutually commute (zero matrices, not just traceless commutators). The hub-count prediction from sl_d was wrong.
- **What it taught us**: The growth-rate prediction from sl_d was the real signal (confirmed: ~4x for n=5 at sl_4), but the hub-count prediction does not scale as 2*C(n-1,2). Simple modules may have fundamentally different hub structure from non-semisimple ones.
- **Demo reference**: D101
- **Do not re-attempt unless**: Testing on a non-semisimple module at the same strand count, or sampling from broader depth ranges.

#### DE-26: Self-Doubling for XOR6 Extension (D64)

- **What was tried**: Extend XOR5 solutions to XOR6 by adding a 6th weight equal to any existing weight (w[5]=w[i]).
- **Why it failed**: Always fails. Scalar doubling preserves angle; class-1 values (odd sector) doubled remain class-1, making the pair output for weight-2 input class-1 when it must be class-0 for parity.
- **What it taught us**: The self-doubling impossibility theorem accounts for 34/38 = 89.5% of all closest misses at the XOR6 wall. The dominant failure mode at the wall is this specific geometric constraint.
- **Demo reference**: D64
- **Do not re-attempt unless**: Using weights from a different lattice where doubling can change the sector classification.

#### DE-27: Gap-of-2 Hypothesis (D65)

- **What was tried**: Hypothesize that the gap between pigeonhole bound and actual parity ceiling is constant at 2 (Z[zeta_8]: 7-5=2).
- **Why it failed**: Z[zeta_16] has gap=9 (14-5). The gap is not constant; the ceiling n=5 is algebra-independent while the pigeonhole bound varies.
- **What it taught us**: The parity ceiling is a structural property of the computation, not of the specific cyclotomic ring. The ceiling does not track the ring's pigeonhole bound.
- **Demo reference**: D65
- **Do not re-attempt unless**: You have a specific structural argument for why a particular ring would shift the ceiling.

---

## Part 2: Wall Patterns ("The Wall Was X Not Y")

A structural pattern recurring throughout the DKC program: an apparent capacity ceiling is identified, exhaustive search confirms zero solutions, then the wall is located in a specific component and bypassed by changing that component while everything else stays fixed.

---

### WP-01: Parity Under Split-Sigmoid — Wall in Activation, Not Lattice

- **Observed**: 3-input parity has exactly 0 solutions out of 100M quartets with split-sigmoid activation at Z[zeta_8]. Appeared to be a fundamental limit of the bracket lattice.
- **Diagnosis**: Split-sigmoid is a k=2 half-plane classifier. Complex Minsky-Papert theorem proves k=2 sectors cannot separate parity with ANY complex weights. The Z[zeta_8] lattice actually contains Aizenberg's parity-capable weights (zeta_8, i, -1).
- **Resolution**: k-sector MVN activation. 906 solutions at k=6 from the same 100-value catalog.
- **Mechanism**: k=6 sectors create 3 odd sectors at approximate 120-degree intervals (triskelion geometry). The parity function requires each input weight to occupy its own odd sector. Split-sigmoid only has 2 sectors (half-plane) which is provably insufficient.
- **Demo chain**: D48 (wall identified) -> D49 (neglecton expansion fails, confirming wall is not in the lattice) -> D50 (resolution)
- **Lesson**: When the weight space contains known solutions (Aizenberg's construction) but exhaustive search finds zero, the activation function is the first suspect.

### WP-02: XOR8 Under S^2-Only — Wall in Activation, Not Root

- **Observed**: Zero XOR8 solutions from all C(24,4) = 10,626 quadruples with S^2-Voronoi activation at zeta_8. Pigeonhole argument: 256 masks / 14 cells ~ 18.3 per cell makes parity-pure cells geometrically impossible.
- **Diagnosis**: S^2-only activation discards the eigenvalue angle (S^1 information). The eigenvalue angle carries independent computational information (D74 incomparability theorem).
- **Resolution**: S^1 x S^2 product activation (sector x Voronoi) with 112 cells. 6 XOR8 winners.
- **Mechanism**: Adding the S^1 angular channel breaks the pigeonhole: 112 cells for 256 masks (2.3 masks/cell) is sparse enough for balanced parity cells to exist. Neither sector-only (even at k=16) nor Voronoi-only achieves XOR8; both channels are required.
- **Demo chain**: D76 (wall identified) -> D77 (resolution)
- **Lesson**: Identical structural pattern to WP-01, separated by 27 demos. When a pigeonhole argument blocks computation, adding independent activation dimensions can break it.

### WP-03: XOR10 at Zeta_8 — Wall in Group Finiteness, Not Architecture

- **Observed**: XOR10 = 0 at zeta_8 under ALL tested activations (S^2, S^1 x S^2, sector-only). Exhaustively confirmed across all C(24,5) = 42,504 quintuples.
- **Diagnosis**: The binary octahedral group closes at 24 elements. The combinatorial diversity of 24 elements is exhausted at 5-element selection. No activation change can solve XOR10 at zeta_8.
- **Resolution**: Switch to zeta_12, whose SU(2) subgroup is infinite (4096+ entries). 124 XOR10 winners, 50+ XOR12 winners.
- **Mechanism**: An infinite group generates an ever-growing catalog via BFS, providing the combinatorial diversity that parity at higher arities demands. Only zeta_4 and zeta_8 generate finite SU(2) subgroups; all other tested roots generate infinite groups.
- **Demo chain**: D78 (wall confirmed exhaustively) -> D79 (resolution) -> D80 (finiteness survey, 7 roots classified)
- **Lesson**: The third instance of the same pattern, but the wall was in a different component (group finiteness rather than activation). Before concluding a function is unreachable, check whether the algebraic substrate is large enough.

### WP-04: 3-Strand Trace Readout — Wall in Readout, Not Algebra

- **Observed**: 3-strand algebra produces zero XOR6 at every k_sec despite being algebraically richer than 2-strand (infinite group, 2048 entries, 98.5% interleaving, lossless column-4 diversity).
- **Diagnosis**: The trace readout collapses 20 integer DOFs (column vector) to 4 integer DOFs (trace). For 3+ strands, this 96% information loss is catastrophic. The 2-strand pipeline succeeds because quaternions ARE the readout — the module dimension (2) matches the SU(2) representation dimension (2).
- **Resolution**: Work on irreducible Delta_1 module (dim=2 over Cyc8) instead of the full 5x5 representation. D99b achieved the first-ever 3-strand XOR.
- **Mechanism**: The irreducible module avoids the lossy trace reduction. Matrix entries over Z[zeta_8] in a 2x2 module provide 8 integer DOFs with no reduction, rich enough for XOR separation.
- **Demo chain**: D98 (wall identified, readout bottleneck thesis) -> D99b (resolution via irreducible module)
- **Lesson**: When the algebra is rich but computation fails, the bottleneck may be in the readout/measurement, not the algebraic content. Match readout dimensionality to the module's information content.

### WP-05: Self-Doubling Impossibility — Wall in Geometric Constraint, Not Vocabulary

- **Observed**: XOR5 solutions cannot be extended to XOR6 by adding any 6th weight from the Z[zeta_8] catalog. Minimum Hamming distance to XOR6 is 4 (not 0).
- **Diagnosis**: 89.5% of closest misses are self-doublings (w[5]=w[i]). Scalar doubling preserves angle; class-1 doubled remains class-1, contradicting the required class-0 for even-parity input weight 2. A second wall layer exists at d>=7 with 0 broken pairs but weight-3+ errors (GHZ-type higher-order obstruction).
- **Resolution**: Generalized activations at k=24 with non-standard binary sector labeling (D65), or quaternionic S^3 framework (D66). The wall is absolute for standard k<=23 activations.
- **Mechanism**: The Z[zeta_8] lattice has only 8 discrete angular directions. For 6+ weights, the combinatorial constraints on 2^6-1 = 63 subset sums exceed what 8 angles can satisfy. The fundamental limitation is the angular discreteness of the lattice.
- **Demo chain**: D64 (wall anatomy, self-doubling theorem) -> D65 (two-layer wall, generalized activation at k=24)
- **Lesson**: When the wall manifests at a specific pairwise interaction level (weight-2 inputs), check whether the angular vocabulary of the lattice is sufficient for the required number of independent constraints.

### WP-06: Voronoi Optimization Catastrophe — Wall in Algebraic-vs-Geometric Mismatch

- **Observed**: Stochastic optimization of S^2 direction placement (improving t-design quality from t=0 to t~1) destroys XOR6 computation (36 solutions -> 4).
- **Diagnosis**: The 13 eigenvector directions are algebraically selected by the binary octahedral group. Their specific angular relationships (not their uniform distribution) encode the computation. Geometric optimization moves directions away from the algebraically correct positions.
- **Resolution**: Not optimizing. Or: constrain non-null (body diagonal) directions within 2 degrees while letting null (edge midpoint) directions move freely (D88: 48% design improvement with only 2.8% XOR loss).
- **Mechanism**: Non-null body-diagonal directions are rigid computational anchors (8% XOR loss at 10 degrees perturbation). Null edge-midpoint directions are flexible scaffolding (-2.4% loss = slight gain at 10 degrees). The k-ladder activation provides 14x better perturbation resilience than Voronoi-only.
- **Demo chain**: D72 (catastrophe observed) -> D88 (two-role architecture explains it, constrained optimization resolves it)
- **Lesson**: Algebra beats geometry. Optimizing geometric distribution quality can destroy algebraically constrained computation. Always preserve algebraically critical structures.

### WP-07: "XOR Dies at N>=7" — Wall in k-Regime, Not Fundamental Limit

- **Observed**: D93 Phase 8 showed XOR=0% at N=8 and 0.02% at N=7 with 17 entries and 84 cells at depth 1. Appeared to be a fundamental circuit-complexity boundary.
- **Diagnosis**: Pigeonhole at low k: 2^N masks > k*cells when N>=7. But the pigeonhole bound depends on k, which was fixed at a small value.
- **Resolution**: Increase k to the collision-avoidance regime (k/2^N >> 1). At k=4096, W_{8,0} produces XOR8=22,266 (n=8 beats n=6's 21,699 at matched k).
- **Mechanism**: The ratio k/2^N is the real control parameter. At k/2^N << 1: pigeonhole kills XOR (too many masks per cell). At k/2^N >> 1: cells outnumber masks, balanced parity bisection becomes possible.
- **Demo chain**: D93 (apparent wall) -> D104 (k is the real lever) -> D105 (D93 claim refuted at high k)
- **Lesson**: Pigeonhole-based impossibility claims are parametric — they hold at the tested k but not necessarily at higher k. Always check whether increasing the activation parameter restores capacity.

---

## Summary of Patterns

### The Recurring Template

Every wall resolution in the DKC program follows the same structure:

1. **Observe** an apparent impossibility (zero solutions, exhaustive search).
2. **Diagnose** which specific component enforces the wall: activation, group finiteness, readout, geometric constraint, or k-regime.
3. **Change** only that component while holding everything else fixed.
4. **Verify** the wall breaks.

### Frequency of Wall Types

| Wall Location | Instances | Demo Chains |
|---|---|---|
| Activation function | 3 | D48->D50, D76->D77, D93->phase_cell |
| Group finiteness | 1 | D78->D79 |
| Readout/measurement | 1 | D98->D99b |
| Geometric constraint | 2 | D64->D65, D72->D88 |
| k-regime (pigeonhole) | 1 | D93->D105 |

### The Master Lesson

The topology and algebra are almost never the ultimate bottleneck. The bracket lattice has always been richer than the readout can extract. Every wall so far has been in the interface between the algebra and the observer (activation, readout, regime), not in the algebra itself.
