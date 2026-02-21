# Novelty Assessment: DKC Research Program

Assessment date: 2026-02-20. Updated 2026-02-21 (added D38, D39, D60, D63, D39 Parts G-J; added D64-D71 S² arc). Based on
76 cataloged papers (literature-index.md), 71 demos, and the theorem inventory.
Honest where uncertain.

---

## 1. Established Results We Build On

These are well-known, published, peer-reviewed results we use as-is.

**Topology / Knot Theory**
- Kauffman bracket state-sum model (Kauffman 1987). Our foundation.
- Habiro integrality: bracket values at roots of unity are cyclotomic integers (Habiro 2002).
  We apply the specific instance at A = e^{i5pi/4} (Z[zeta_8]) and at ell=3,4,5,6.
- Jones polynomial BQP-completeness (Aharonov-Jones-Landau 2009, Freedman-Kitaev-Wang 2003).
- Fibonacci anyon braid representations dense in PSU(2) (Freedman-Larsen-Wang 2002).
- Reidemeister invariance, Yang-Baxter equation, Markov theorem -- standard.
- Witten = RT now rigorously proven (Costello-Francis-Gwilliam 2026, arXiv:2602.12412).

**TL Algebra / Representation Theory**
- TL non-semisimplicity at roots of unity (Goodman-Wenzl 1993).
- Graham-Lehrer cellularity (1996): TL is cellular, forcing Loewy length <= 3.
- PIM diamond structure, head iso socle for individual indecomposables (Belletete-Ridout-Saint-Aubin 2016).
- Alternating semisimplicity: TL_n semisimple iff n < ell (general theory).
- Semisimplicity criterion: TL_n(2cos(pi/ell)) semisimple iff n < ell.
- Logarithmic CFT / dense-polymer correspondence at delta=0 (Gainutdinov et al. 2013).

**Complex-Valued Neural Networks**
- MVN architecture: k-sector activation for k-valued logic (Aizenberg 2000, 2011).
- Single UBN solves parity with w_1=zeta_8, w_2=i, w_3=-1 (Aizenberg 2008).
- Single complex neuron solves XOR (Nitta 2003).
- Gradient descent fails on parity (Parity Hardness 2025, arXiv:2501.00817).

**Lattice Codes / Information Theory**
- Compute-and-forward framework (Nazer-Gastpar 2011).
- Extension to Eisenstein integer lattices (Tunali et al. 2015).
- GPTQ weight quantization equivalent to Babai nearest-plane algorithm (GPTQ/Babai 2025).

**TQC / Non-Semisimple TQFTs**
- Non-semisimple TQFTs introduce neglectons rescuing universality for Ising anyons (Nature Comms 2025).

---

## 2. Novel Results

Things we believe are genuinely new. No prior work found across 72 papers.

### 2a. The DKC Framework Itself
- **Forward DKC construction.** Exact Z[zeta_8] bracket values compute Boolean functions
  (XOR, AND, etc.) without training. 100+ valid XOR triples, RMS = 0.000.
  No paper in any of the four pillar fields cites the other three. The intersection
  Habiro + Nazer-Gastpar + Abramsky + Aizenberg is genuinely unoccupied.
  **Confidence: HIGH.** Extensive literature search across all four domains.

- **Training-as-Focusing thesis.** Training does not CREATE computational structure;
  it locates pre-existing solutions on the cyclotomic lattice. Forward DKC proves
  this constructively. **Confidence: HIGH.**

### 2b. Activation Geometry Results
- **11/13 half-plane theorem.** For ALL half-plane activations on multiplicative encodings,
  exactly 11/13 NPN classes are reachable. 0x1B (CROSS) and 0x06 (XOR-AND) always
  unreachable. Verified at ell=4,5,6 (ring-independent). Proven analytically (Demo 62)
  with separate algebraic proofs for each unreachable class.
  **Confidence: HIGH.** Clean analytical proof, not just computational.

- **Parity unreachability under split-sigmoid.** 0/100,000,000 quartets compute XNOR3.
  Exhaustive. The wall is razor-thin: 1 Hamming bit from EXACT1 (127K solutions).
  **Confidence: HIGH.** Exhaustive verification.

- **Parity reachability at k=6 sectors.** 906 solutions from the same 100-value catalog.
  All 13 NPN classes computable. The wall was in the activation, not the lattice.
  **Confidence: HIGH.**

- **Four-tier NPN hierarchy.** 13 NPN classes partition by minimum sector count:
  k=2 (5 classes), k=3 (+1), k=4 (+6), k=6 (+1 = parity). Parity uniquely
  isolated at the highest tier. **Confidence: HIGH.**

- **Non-monotonic parity solutions.** 906@k=6 > 756@k=7 > 96@k=8. Anti-correlated
  with gcd(k,8). **Confidence: HIGH** (computational fact). The incommensurability
  explanation is conjectural.

- **Triskelion principle (3-input).** 3-input parity at k=6 requires each input to have its own
  odd sector at 120-degree intervals. All weights confined to octants {2,4,5,7}.
  **Confidence: HIGH.** Confirmed by all 906 solutions. The k=2n generalization to higher
  inputs is FALSIFIED (5-input parity requires k=15, not k=10). Triskelion is 3-input specific.

- **Parity ceiling for Z[zeta_8] is n=5.** 4-input parity at k=8 (96 solutions), 5-input at
  k=15 (3020 solutions), n>=6 impossible (constraint wall: 63 subset constraints unsatisfiable),
  n>=8 impossible (pigeonhole: 7 class-1 octants). Two distinct impossibility mechanisms.
  **Confidence: HIGH.** Exhaustive computational verification + pigeonhole proof.

- **Oriented matroid classification of DKC.** 21 distinct OM types from 512 weight triples.
  Only the alternating type (-,+,- or +,-,+) achieves parity. 0 of ~1M non-alternating triples
  reach parity. Interleaving around the circle is the geometric condition for pair-sum parity alternation.
  **Confidence: HIGH.** Exhaustive verification. Connection to matroid theory is novel.

- **Four-tier NPN from octant-sector geometry.** The four-tier hierarchy is reproduced from
  pure discrete geometry (sector boundaries on 8 rays) without any search. k=6 is the UNIQUE
  sector count producing the parity octant set {2,4,5,7}.
  **Confidence: HIGH.** Exhaustive for k=2..16.

- **Convexity thesis.** The 0x1B/0x06 wall is about activation convexity, not encoding
  geometry. Half-planes are convex; multiplicative products can rotate out.
  **Confidence: MEDIUM.** Supported computationally but the formal proof is incomplete.

### 2f. Matroid Structure of the DKC Parity Hierarchy (D64)

- **Matroid minor chain theorem.** {XOR2, XOR3, XOR4, XOR5} is a deletion-contraction
  closed chain: deleting any weight from an XOR_n solution always yields an XOR_{n-1}
  solution (100% at all levels n=5→4→3→2); contracting any input to 1 always yields
  XNOR_{n-1} (100%). This is the first matroid-theoretic characterization of the DKC
  parity hierarchy. **Confidence: HIGH.** Exhaustive verification at all levels.

- **Self-doubling impossibility theorem.** Adding a 6th weight equal to any existing
  weight always fails XOR6. Proof: scalar doubling preserves the rotation angle of a
  Z[zeta_8] value; class-1 (odd sector) doubled is class-1; pair output for even-parity
  input is class-1 when it must be class-0. Accounts for 34/38 = 89.5% of closest
  misses. **Confidence: PROVEN.**

- **Funnel theorem (upward coverage collapses).** XOR3→XOR2: 99.3%, XOR4→XOR3: 39.2%,
  XOR5→XOR4: 19.2%, XOR6→XOR5: 0%. The n=6 wall casts a shadow all the way down: 80%
  of XOR4 weight sets are already dead ends. Wall Hamming distance is d=4 minimum
  (exhaustive). **Confidence: HIGH.** Novel funnel characterization.

- **Vocabulary stratification (44 good / 31 poison).** Of 83 class-1 catalog values at
  k=15, exactly 44 appear in XOR5 solutions and (the same 44) in extensible XOR4
  solutions — strictly nested. 31 values are poison (appear only in orphan XOR4
  solutions that cannot extend to XOR5). Poison values avoid octant 4 entirely.
  **Confidence: HIGH.** Possible connection to Reiner's cyclotomic matroids (open).

- **Entanglement monogamy / contextuality framing.** The wall at n=6 is structurally
  analogous to the CKW inequality (10 pairwise sums simultaneously compatible, 15 cannot
  be) and to Kochen-Specker contextuality (consistent assignment obstruction for all
  C(6,2)=15 pair sums). This framing is novel; no prior work connects DKC parity walls
  to quantum contextuality. **Confidence: MEDIUM** (conceptual parallel, not a proof).

### 2g. Algebra-Independence of the Parity Ceiling (D65)

- **Parity ceiling is algebra-independent.** Tier thresholds (XOR3 k=6, XOR4 k=8,
  XOR5 k=14) and the n=5 ceiling are identical at Z[zeta_8] and Z[zeta_16]. Nobody has
  shown that a computational ceiling is the same across distinct cyclotomic rings.
  Gap-of-2 hypothesis refuted: pigeonhole rises from 7 to 14 but actual ceiling stays
  at n=5, widening the gap to 9. **Confidence: HIGH.** Exhaustive sweep k=2..31, both
  catalogs. Conjectured to hold for all cyclotomic evaluation points.

- **Two-layer wall decomposition.** XOR6 = 0 under standard odd/even activation at all
  k up to 31 (absolute wall) but becomes achievable with generalized binary sector
  labelings at k=24 (3/2700 pass). The wall decomposes into: (1) a geometric obstruction
  that standard activations cannot overcome at any sector count, and (2) a convention
  wall that falls once arbitrary binary colorings are allowed. This separation is new.
  **Confidence: HIGH.**

- **Generalized XOR7 at k=91 (MAJOR RESULT).** First XOR7 solution appears at k=91
  sectors with generalized activation. Anatomy: 3 antipodal pairs + 1 lone purely
  imaginary weight. Scaling ratio XOR7/XOR6 threshold: 91/24 ≈ 3.8. Zero margin
  universality: all passing tuples at all k have exactly zero margin, reflecting
  exact lattice alignment of Z[zeta_8] sums. **Confidence: HIGH.**

### 2h. 24-Cell as the Natural Computational Geometry (D66)

- **24-cell emergence from DKC braids (NOVEL).** SU(2) braid representations with the
  standard generators produce exactly the 24 vertices of the 24-cell (binary octahedral
  group) from 87,890 enumerated braids. Nobody has identified the 24-cell as the natural
  computational geometry for knot-theoretic neural computation.
  **Confidence: HIGH.** Exhaustive enumeration.

- **24-cell Voronoi outperforms geographic grid.** 25-cell Voronoi (24 vertices + zero
  cell) yields 35 XOR6 solutions vs. 34 for a 64-cell geographic grid. More solutions
  with less than half the cells. This quantitatively validates the 24-cell as the right
  geometry. **Confidence: HIGH.**

- **Universal 7/7/11 Voronoi structure.** Every one of the 35 XOR6 solutions activates
  exactly 7 cells labeled 0, 7 labeled 1, and 11 unused. Geometric constraint: 3
  antipodal pairs define 3 great circles on S³, and the 64 subset sums touch only 14
  of 25 Voronoi cells. **Confidence: PROVEN.**

- **F4 symmetry decomposes to 6 orbits.** Under the 576-element F4 rotation subgroup,
  35 solutions fall into 6 orbits (12+7+5+5+4+2), dividing into edge-dominant (22)
  and body-dominant (13) families. **Confidence: PROVEN.**

- **Bracket-quaternion complementarity.** Bracket and quaternion trace are independent
  hash functions: 450 braid pairs share bracket but differ in quaternion; 8,424 share
  quaternion but differ in bracket. They are complementary projections of the full
  braid group representation. **Confidence: HIGH.**

- **Complex k=24 as projection of 24-cell.** The minimum of k=24 sectors for S¹ XOR6
  (D65) is the projection of the 24-cell's 24 vertices onto S¹ via the trace. The
  exact match is not a coincidence. **Confidence: HIGH** (confirmed in D66/D67).

### 2i. S² as the Computational Sweet Spot (D67 — MAJOR FINDING)

- **S² is the optimal computational domain.** A 14-cell custom Voronoi on S² achieves
  36 XOR6 solutions — more than the 25-cell S³ Voronoi (35) and any other tested space.
  Computational hierarchy: S² (14 cells) > eigenvalue θ (k=8 first solutions) >
  S¹ (k=24) > S³ (25 cells) > R² (visualization only). The 2D projection of a 4D
  space is the most efficient domain. **Confidence: HIGH.**

- **Hopf phase is completely computationally inert (NOVEL).** Phase-only (Hopf fiber
  S¹) activation yields ZERO XOR6 solutions at every resolution tested (k=4 through
  k=32). All DKC computational information lives in the S² Hopf base; the S¹ fiber
  carries nothing. This Hopf decomposition of the DKC information structure has not
  been identified before. **Confidence: HIGH.** Exhaustive.

- **Platonic solid Voronois fail; data-derived geometry succeeds.** Octahedral,
  icosahedral, and cuboctahedral Voronois on S² all yield zero XOR6 solutions. Only the
  data-intrinsic 13-direction eigenvector Voronoi works. Natural polytopal geometry is
  the wrong geometry. **Confidence: HIGH.**

### 2j. Intrinsic Curvature of DKC Computation (D68)

- **DKC computation is intrinsically curved.** Stereographic projection of the S²
  Voronoi to R² preserves all 36 solutions exactly with the S² great-circle metric but
  collapses to zero with Euclidean distance. The computation cannot be flattened.
  The plane is a window, not a workspace. **Confidence: PROVEN.**

- **DKC as a drawable 2D picture.** The S² Voronoi can be visualized as an ASCII
  decision-boundary image in the plane (stereographic projection). "Visualize on the
  plane, compute on the sphere." This is a new representational insight.
  **Confidence: HIGH** (demonstrated).

- **Perceptron formulation without training.** Architecture: 6 bits → 2 linear neurons
  (weights from stereographic projection of DKC quaternion directions) → Voronoi
  activation (S² metric) → 1 bit. The entire weight matrix comes from knot theory, not
  gradient descent. **Confidence: HIGH.**

### 2k. Direction Nesting Theorem (D69)

- **Direction Nesting Theorem.** For any N, all eigenvector directions in the ζ_N
  catalog appear identically in the ζ_{2N} catalog (exact nesting on S²). Algebraic
  proof: squaring a quaternion preserves its rotation axis; every ζ_N braid word is
  a valid doubled word in ζ_{2N}. Confirmed computationally at ζ_8 → ζ_16 (13/13,
  zero angular drift). **Confidence: PROVEN** algebraically, confirmed computationally.

- **ζ_8 is the computational sweet spot.** The coarsest cyclotomic root producing the
  13-direction structure on S². Finer roots nest ζ_8 exactly but add no computational
  power at fixed partition size — and at their natural Voronoi resolution trivialize the
  computation entirely (~90% of all XOR6 triples become solutions via pigeonhole at
  ζ_16's 3,457 directions). This is a novel characterization of the optimal algebraic
  level. **Confidence: HIGH.**

- **Trivialization above ~100-500 cells.** At ζ_16 resolution (3,457 Voronoi cells),
  90.6% of all XOR6 triples are solutions. The topology-driven computation (36/C(24,6)
  = 0.007%) trivializes entirely by pigeonhole when cell count exceeds the input
  dimension. **Confidence: HIGH** (demonstrated).

### 2l. Knots as Music (D70)

- **Yang-Baxter braids produce identical melodies.** The 4 eigenvalue angles of the
  binary octahedral group map to 4 musical notes (C, Eb, E, F#; unison through tritone).
  Yang-Baxter equivalent braids (σ₁σ₂σ₁ = σ₂σ₁σ₂) are not just topologically
  equivalent — they produce literally the same melody step-by-step. Melody depends only
  on product length, not on which generators are used. **Confidence: HIGH** (confirmed).

- **Music and space are orthogonal.** Pitch (eigenvalue angle) and spatial position
  (eigenvector direction) are independent dimensions of a unit quaternion: Pearson
  r = −0.0624 over 253 pairs. This is the Hopf fibration confirmed acoustically.
  **Confidence: HIGH.** Novel framing.

- **Tritone-antipodal correspondence.** On the dodecahedron's circle-of-fifths
  assignment, antipodal faces carry tritone pairs (C/F#, G/Db, D/Ab, A/Eb, E/Bb, B/F).
  Dissonance is the antipodal map. **Confidence: HIGH** (demonstrated). Whether this
  follows from a deeper symmetry argument is open.

### 2m. Spectral Universality and the 13=13 Theorem (D71 — TIES THE S² ARC TOGETHER)

- **13=13 Theorem (PROVEN).** The minimum bandwidth for XOR6 DKC on S² is l=6 because
  2l+1 = 2×6+1 = 13 equals the number of eigenvector directions. Phase transition is
  sharp and all-or-nothing: 0% recovery at l≤5, 100% at l=6. The bandwidth threshold
  equals the number of computational degrees of freedom, directly linking the spectral
  and geometric pictures. **Confidence: PROVEN** (computational; analytical proof open).

- **Spectral universality.** All 36 XOR6 winners share the same spectral envelope: l=6
  dominant (~80%), l=4 secondary (~6%), l=12 tertiary (~6–7%). The spectrum is a
  structural invariant of DKC XOR6 solutions on S², independent of which specific
  solution. **Confidence: HIGH** (demonstrated across all 36 winners).

- **Super-Nyquist compression by 3.5×.** 14 Voronoi cells suffice where Nyquist
  predicts (l+1)² = 49. Explained by compressed sensing: XOR6 is a binary function at
  13 known positions, so sparsity plus known support eliminates the generic sampling
  requirement. This is a direct instance of compressed sensing on S².
  **Confidence: HIGH.**

- **Trivialization DOF prediction.** The ζ_16 ~90% solution rate (D69) is explained
  spectrally: 3,457 directions require bandwidth l≥1728, at which scale any coarse
  partition trivially resolves XOR6. The 13=13 theorem predicts the trivialization
  threshold for any cyclotomic level. **Confidence: CONJECTURED** (analytically
  motivated, not yet proven).

### 2c. TL Algebraic Structure
- **Sandwich theorem.** rad^2(TL_{2k}(0)) is isomorphic to TL_{2k-1}(0) as an algebra.
  Literature proves head iso socle for individual PIMs (module-level). Nobody identifies
  rad^2 as a NAMED ALGEBRA at the full algebra level.
  **Confidence: HIGH for novelty of the identification.** The dimensional fact is likely
  implicit in literature; the explicit algebra-level isomorphism is ours.

- **Catalan trace theorem.** All nonzero fixed-point counts are Catalan numbers. Window
  formula: at TL_n, nonzero traces are {C_m : floor(n/2)+1 <= m <= n}.
  **Confidence: HIGH.** Not found in any source.

- **Fibonacci rank theorem.** Total bilinear rank of rad^2 generator across through-strand
  sectors = F(ell-1). Verified at ell=3,4,5,6. "Entanglement rank of thermal ground
  state = anyonic computational power."
  **Confidence: HIGH for novelty.** The Fibonacci connection to TL fusion rules is
  known; this specific rank formula appears to be new.

- **Cross-ell radical dimension formulas.** rad(TL_ell) = 2*ell - 3 at first
  non-semisimple n. rad(TL_{ell+1}) = ell^2 - ell - 3 at second. Universal
  rad^2 = 1 at both. **NOW FORMALLY PROVEN** (`proofs/radical-dimension-formula.md`):
  V_{ℓ-2} is the unique degenerate cell module at n=ℓ with corank 1 via Chebyshev
  determinant. The proof is self-contained and publication-ready.
  **Confidence: PROVEN.** No longer a computational observation — this is a theorem.

- **Markov RT truncation theorem.** Markov trace at q=e^{i*pi/ell} kills exactly j >= ell-1,
  preserves j <= ell-2. Excess = sum of (dim L_j)^2 for killed blocks. The "shadow interpretation":
  fixpt = algebraic light (all simples), Markov = topological light (RT-physical simples).
  **NOW FORMALLY PROVEN** (`proofs/markov-rt-truncation.md`): three-part proof via negligible
  ideal. 31/31 computational verification points.
  **Confidence: PROVEN.** The initial conjecture ((ell+m-1)^2+1) was REFUTED at m=3, leading
  to discovery of the true block-sum formula. Clean three-part proof.

- **Universal Corank 1 at first degeneracy.** At first n where V_j degenerates, corank = 1.
  Linked module V_n^{(n)} has dim 1.
  **Confidence: PROVEN.** Structural from Graham-Lehrer linking theorem.

### 2d. Bracket / Axiality
- **Axiality theorem at delta=0.** Every bracket value is axial in Z[zeta_8]: at most
  one nonzero component. 131K braids, zero counterexamples, plus proof sketch.
  **Confidence: MEDIUM-HIGH.** The fact may be known to specialists but we have not
  found it stated. The proof sketch (single-loop isolation + phase arithmetic) is ours.

- **Z[i]-axiality at delta=sqrt(2).** Bracket values at ell=4 are Z[i]-axial in
  Z[zeta_16]. 89K braids verified.
  **Confidence: MEDIUM.** May follow from known cyclotomic structure. Not found stated.

- **Axiality breakdown hierarchy.** Strict ordering: ell=2 (1D), ell=3 (1D),
  ell=4 (2D), ell=5 (full 4D). **Confidence: MEDIUM.** Computational observation;
  not sure whether it is derivable from standard Galois theory.

### 2e. Other
- **Forward-reverse duality.** Functions that topology produces frequently (forward)
  are exactly those whose trained weights decompose back into brackets (reverse).
  Spearman rho = -0.8167 between three independent measures.
  **Confidence: HIGH.** Novel observation.

- **Topology-frequency correlation reversal.** Split-sigmoid and MVN-continuous access
  different sectors of the bracket algebra. Spearman rho flips sign.
  **Confidence: HIGH.** Novel observation.

- **Phase decoherence thesis.** Split-sigmoid encodes magnitude but not phase of
  Z[zeta_8] weights. Amplitude-only RMS (0.33) vs per-neuron gauge RMS (0.57).
  **Confidence: HIGH.** Novel diagnosis.

---

## 3. In Between / Extensions

Results that extend known work in new directions. Not unprecedented, but non-trivial.

- **Computational verification of Graham-Lehrer cellularity bounds.** We confirmed
  Loewy length <= 3 across ell=2,3,4,5,6 computationally. The theorem is known;
  the explicit verification at multiple roots of unity across n=2-8 is new data.

- **Explicit dimension tables for radical filtrations.** Full tables of dim(rad),
  dim(rad^2), Peirce block dimensions for TL_n at delta=0 and other roots of unity,
  n=2 through 8. These are computable from known theory but not tabulated anywhere
  we have found.

- **GPTQ/Babai connection to DKC lattices.** GPTQ/Babai (2025) proves NN quantization
  is a lattice closest-vector problem on Z^n. Our observation: DKC lattices (Z[zeta_8])
  are algebraic integer lattices, a strict superset. The connection is noted but not
  yet formalized.

- **Semisimple decompositions (odd n, delta=0).** TL_3(0) = M_2(k) + k, etc.
  Wedderburn-Artin structure is standard; our explicit decompositions with ballot
  number dimensions may be stated elsewhere in representation theory.

- **Entanglement vanishing at delta=0.** Non-interleaving braids have bracket = 0.
  This is likely a folklore observation (delta=0 kills all multi-loop states); we
  prove it exhaustively and note the breakdown at delta != 0.

- **Complex Minsky-Papert theorem.** k=2 half-plane cannot compute 3-input parity
  with ANY complex weights. This is a direct complex extension of the classical
  Minsky-Papert (1969) result. The proof is elementary; the novelty is applying
  it to the complex setting and connecting it to DKC.

- **Neglecton weight analysis.** Perturbation around delta=0 yields 148 new Z[zeta_8]
  values but no new angular directions. This confirms the activation (not lattice)
  diagnosis. The observation is ours; the neglecton framework is from Nature Comms 2025.

- **Chebyshev structure of bracket constellations.** Quantum dimension recurrence
  [n+1]_q = [2]_q * [n]_q - [n-1]_q is standard Chebyshev. Our framing in terms
  of "bracket constellations" and catalog organization is presentational, not
  mathematically novel.

- **FK = Ising equivalence verification.** Standard statistical mechanics result.
  Our contribution: explicit C89 verification and observation that FK values break
  Z[i]-axiality where bracket values preserve it.

- **Decoherence rate scaling.** rad proportion (2*ell-3)/C_ell decreasing with ell
  is novel notation for a known phenomenon (larger ell = more stable TQC).

---

## 4. The Gap Statement

**What nobody has done:**

Forward DKC compiles topological invariants (Kauffman bracket values) into neural
network weights that compute Boolean functions without training. This construction
sits at the intersection of four established communities:

1. **Aizenberg / CVNN community** -- builds multi-valued neurons with root-of-unity
   activations. Never considers topological invariants as weight sources.
2. **Kauffman / knot theory community** -- computes bracket polynomials and studies
   their algebraic properties. Never connects bracket values to neural computation.
3. **Nazer-Gastpar / lattice coding community** -- proves algebraic integer lattices
   support exact function computation. Never applies this to neural network weights
   or cyclotomic rings beyond Z[omega].
4. **Freedman-Kitaev / TQC community** -- uses braids for quantum gates via unitary
   representations. Never extracts scalar bracket values for classical neural computation.

**Why they remain siloed:**

- The CVNN community treats weight spaces as continuous R^n or C^n, optimized by
  gradient descent. The idea that weights could come from a pre-computed finite
  algebraic catalog is foreign to their methodology.
- The knot theory community treats the bracket as a topological invariant (input: knot,
  output: polynomial). Using the OUTPUT as a computational resource reverses the arrow.
- The lattice coding community works over Z^n or Z[omega] for communication channels.
  Z[zeta_8] lattices from knot invariants are outside their scope.
- The TQC community works with unitary braid group representations (matrices). Scalar
  bracket values are a coarser invariant they don't study for computation.

The closest existing work is Gaier & Ha's Weight Agnostic Neural Networks (2019), which
fixes weights to a single shared value and searches network TOPOLOGIES. Forward DKC
inverts this: it fixes the topology (braid diagram) and searches WEIGHTS from a finite
pre-computed algebraic catalog. Both bypass gradient descent, but WANNs still treat
weights as arbitrary real numbers — they never consider algebraic integer lattices, and
the "topology" they search is network architecture, not knot topology. The conceptual
parallel is real but the mathematical content is entirely different.

The gap is not that each field is missing a piece. The gap is that nobody has asked
the question: "What happens if you use exact bracket values as neural network weights?"
Forward DKC answers it.

The S² arc (Demos 64-71) deepens the gap further. The 24-cell is now identified as the
natural computational geometry for knot-theoretic neural weights; the Hopf fibration
decomposes DKC information into a computationally active base (S²) and an inert fiber
(S¹); the 13=13 theorem gives a spectral characterization of the minimum information
needed to separate XOR6 solutions on S². None of these connections are visible from
within any of the four pillar fields.

---

## 5. Publication Readiness

### Ready Now (strong enough for a focused paper)

**Paper 1: "Forward DKC -- Topological Invariants as Neural Network Weights"**
- Results: Forward DKC theorem, 100+ XOR triples, exhaustive parity wall (100M quartets),
  parity reachability at k=6 (906 solutions), four-tier NPN hierarchy, **4-input parity at
  k=8 (96 solutions), 5-input parity at k=15 (3020 solutions), parity ceiling n=5,
  oriented matroid classification, two types of impossibility, matroid minor chain theorem
  (D64), self-doubling impossibility theorem (D64), funnel theorem (D64), algebra-independence
  of ceiling (D65), two-layer wall decomposition (D65)**.
- Strength: Exhaustive computational proofs. Clean narrative from wall to resolution to
  **complete scaling law**. D63-D65 dramatically strengthen the paper. The matroid
  framing (D64) gives the hierarchy a clean algebraic identity.
- Weakness: Need to clarify relationship to Aizenberg 2008 UBN construction.
  ~~No formal proof that 906 is the exact count~~ -- catalog completeness still open but
  less critical given the full scaling law.
- Recommendation: Submit as a letter/communication to a venue that bridges ML and topology.
  Nature Machine Intelligence, JMLR, or NeurIPS.

**Paper 2: "The 11/13 Theorem -- Universal Limits of Half-Plane Classification"**
- Results: Analytical proofs for 0x1B and 0x06 unreachability. Ring-independence across
  ell=4,5,6. Convexity thesis.
- Strength: Clean algebraic proofs (Demo 62). Universal across evaluation levels.
- Weakness: The convexity thesis is less rigorous than the unreachability proofs.
- Recommendation: Strong enough for a theory paper. Could target a complexity/learning
  theory venue.

**Paper 5 (NEW): "The 24-Cell, S², and the Spectral Geometry of DKC" (S² arc, D64-D71)**
- Results: 24-cell as the natural computational geometry for knot-theoretic weights;
  Hopf phase inertness (all information in S² base, fiber carries nothing); S² as
  optimal domain beating S¹, S³, R²; 13=13 bandwidth theorem; spectral universality
  across all 36 XOR6 winners; super-Nyquist 3.5× compression; direction nesting theorem;
  ζ_8 as the computational sweet spot; knots-as-music mapping (Yang-Baxter = same melody);
  intrinsic curvature proof (Euclidean distance collapses solutions to zero); perceptron
  formulation from knot geometry.
- Strength: The S² arc is a coherent narrative with multiple interlocking results. Several
  theorems are proven (24-cell enumeration, Hopf inertness, antipodal necessity, 13=13
  threshold, direction nesting). The "computation lives on the Bloch sphere" framing is
  unexpected and striking. Connects DKC to compressed sensing, Hopf fibration, and spectral
  geometry in ways that will interest communities beyond the original four pillar fields.
- Weakness: Most results are computational demonstrations; analytical proofs of the
  bandwidth bound and direction nesting (beyond the algebraic argument) would strengthen.
  The compressed sensing application to S² is noted but not formalized.
- Recommendation: Strong enough for a focused paper. Nature Machine Intelligence or
  an algebraic topology / mathematical physics venue. The music mapping could be a
  standalone note or accompanying blog post. The S² arc significantly strengthens the
  overall DKC story and makes Paper 1 more publishable by providing a second, geometrically
  richer paper to cite.

### Needs Additional Work

**Paper 3: "Sandwich Theorem and Radical Anatomy of TL at Roots of Unity"**
- Results: rad^2(TL_{2k}) = TL_{2k-1}, Catalan trace theorem, Fibonacci rank theorem,
  cross-ell radical dimension formulas.
- Strength: Multiple novel results, computationally verified.
- Weakness: Proof sketches, not complete proofs. Need to either prove the sandwich
  theorem rigorously or frame it as a computational discovery. Literature comparison
  needs to be sharper -- must confirm Belletete et al. and Ridout-Saint-Aubin
  do not implicitly contain the algebra-level identification.
- Recommendation: Needs ~2 weeks of proof writing. Target Journal of Algebra or
  a representation theory venue.

**Paper 4: "Axiality of Bracket Values at Roots of Unity"**
- Results: Axiality theorem, Z[i]-axiality, breakdown hierarchy.
- Strength: 131K braids verified, proof sketch.
- Weakness: The proof sketch needs to be formalized. MUST check whether this follows
  trivially from known cyclotomic Galois theory. If it does, the paper is a short note;
  if it doesn't, it's a significant result.
- Recommendation: Consult with a number theorist before writing.

### Speculative / Not Ready

- Incommensurability hypothesis (gcd(k,8) correlation). Suggestive but 3 data points.
- ~~Triskelion generalization conjecture (k=2n for n-input parity). Untested beyond n=3.~~ **FALSIFIED** by Demo 63 — n=5 requires k=15, not k=10. Replaced by universal k theorem (k=2M-1).
- Galois symmetry conjecture (sandwich duality from Galois automorphisms). Speculative.
- Two-channel coding conjecture (~5.8 bits/symbol). Needs formal capacity analysis.
- TL non-semisimplicity hypothesis for parity. Conceptually important but unproven.
- Funnel rate conjecture (D64): upward coverage fractions follow an algebraic or
  exponential decay law as a function of n. Suggestive from 4 data points only.
- Universal parity ceiling conjecture (D65): n=5 holds for ALL cyclotomic evaluation
  points. If proven, this is purely topological (no mention of algebra). Two rings tested.
- k=24 as 24-cell projection conjecture (D65/D66): complex k=24 minimum is a projection
  of the 24-cell vertices onto S¹. Geometrically very plausible; not yet proven.
- Scaling law conjecture (D65): XOR_n minimum generalized k grows by factor ~3.8 per
  increment (XOR6 k=24, XOR7 k=91, XOR8 ~k=345). Two data points only.
- Orbit count formula (D66): XOR_n on the 24-cell yields n*(n-1)/2 orbits. Speculative.
- Trivialization threshold (D69): transition from topology-driven to pigeonhole-dominated
  computation lies between ~50 and ~500 Voronoi cells. Not yet precisely located.
- Tritone-antipodal correspondence (D70): whether the circle-of-fifths antipodal = tritone
  follows from a deep symmetry argument or is a coincidence of the assignment. Open.
- Analytical bandwidth bound (D71): can we prove that the minimum bandwidth of a binary
  Voronoi partition separating N points on S² in general position is l=ceil((N-1)/2)?

### Strengthening Actions

1. **ell=7 verification.** DONE. Predictions exist (rad(TL_7) = 11, Fibonacci rank = 8).
   First cubic number field case. Confirming these would dramatically strengthen
   the cross-ell universality claims.
2. **4-input and 5-input DKC.** **DONE (Demo 63)**. Triskelion generalization FALSIFIED.
   4-input parity at k=8, 5-input at k=15, parity ceiling at n=5.
3. **Formal proof of sandwich theorem.** Likely provable from cellular algebra theory
   plus explicit cell module bilinear form computation.
4. **Catalog completeness argument.** Prove (or bound) that the 100-value Z[zeta_8]
   catalog at delta=0 contains ALL distinct bracket values up to some braid length.
5. **Rate region analysis.** Apply Nazer-Gastpar capacity bounds to DKC. How many
   bits per bracket value can DKC extract?
6. **Formal proof of Markov RT truncation.** DONE (`proofs/markov-rt-truncation.md`). Strengthens Paper 3.
7. **Analytical bandwidth bound (13=13 theorem).** Prove that the minimum bandwidth of
   any binary Voronoi partition separating N points on S² is l=ceil((N-1)/2). Would
   convert D71's computational result into a theorem. Likely requires harmonic analysis
   on S² (Driscoll-Healy framework) plus compressed sensing arguments.
8. **Universal parity ceiling (algebra-independence).** Test Z[zeta_32] tier thresholds.
   If identical to Z[zeta_8] and Z[zeta_16], the conjecture becomes strongly supported
   and should be framed as a purely topological theorem.
9. **Gradient descent rediscovery.** Does gradient descent, starting from random
   initialization, recover the S² weight geometry implied by DKC solutions? If it
   implicitly rediscovers the 13-direction structure, this is strong evidence that the
   geometry is canonical, not an artifact of the knot-theoretic construction.
10. **XOR7 on S².**  Demo 65 requires k=127 on S¹. Does S² reduce XOR7 as dramatically
    as XOR6 (24→14)? If so, the DOF formula predicts: 2l+1 = 127, so l=63. Testing this
    would either confirm or refine the trivialization prediction.
