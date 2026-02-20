# Novelty Assessment: DKC Research Program

Assessment date: 2026-02-20. Updated 2026-02-20 (added D38, D39, D60). Based on
73 cataloged papers (literature-index.md), 50 demos, and the theorem inventory.
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

- **Triskelion principle.** 3-input parity at k=6 requires each input to have its own
  odd sector at 120-degree intervals. All weights confined to octants {2,4,5,7}.
  **Confidence: MEDIUM-HIGH.** Observed in all 906 solutions; geometric explanation
  is informal.

- **Convexity thesis.** The 0x1B/0x06 wall is about activation convexity, not encoding
  geometry. Half-planes are convex; multiplicative products can rotate out.
  **Confidence: MEDIUM.** Supported computationally but the formal proof is incomplete.

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
  rad^2 = 1 at both. Demo 60 confirmed at ell=7 (first cubic number field),
  proving independence from number field degree.
  **Confidence: HIGH.** Six data points (ell=2,3,4,5,6,7) spanning quadratic
  and cubic extensions. Likely derivable from known theory but not found stated.

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

---

## 5. Publication Readiness

### Ready Now (strong enough for a focused paper)

**Paper 1: "Forward DKC -- Topological Invariants as Neural Network Weights"**
- Results: Forward DKC theorem, 100+ XOR triples, exhaustive parity wall (100M quartets),
  parity reachability at k=6 (906 solutions), four-tier NPN hierarchy.
- Strength: Exhaustive computational proofs. Clean narrative from wall to resolution.
- Weakness: No formal proof that 906 is the exact count (depends on catalog completeness).
  Need to clarify relationship to Aizenberg 2008 UBN construction.
- Recommendation: Submit as a letter/communication to a venue that bridges ML and topology.
  Nature Machine Intelligence, JMLR, or NeurIPS.

**Paper 2: "The 11/13 Theorem -- Universal Limits of Half-Plane Classification"**
- Results: Analytical proofs for 0x1B and 0x06 unreachability. Ring-independence across
  ell=4,5,6. Convexity thesis.
- Strength: Clean algebraic proofs (Demo 62). Universal across evaluation levels.
- Weakness: The convexity thesis is less rigorous than the unreachability proofs.
- Recommendation: Strong enough for a theory paper. Could target a complexity/learning
  theory venue.

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
- Triskelion generalization conjecture (k=2n for n-input parity). Untested beyond n=3.
- Galois symmetry conjecture (sandwich duality from Galois automorphisms). Speculative.
- Two-channel coding conjecture (~5.8 bits/symbol). Needs formal capacity analysis.
- TL non-semisimplicity hypothesis for parity. Conceptually important but unproven.

### Strengthening Actions

1. **ell=7 verification.** Predictions exist (rad(TL_7) = 11, Fibonacci rank = 8).
   First cubic number field case. Confirming these would dramatically strengthen
   the cross-ell universality claims.
2. **4-input and 5-input DKC.** Does parity require k=8 sectors for 4 inputs?
   Would test triskelion generalization.
3. **Formal proof of sandwich theorem.** Likely provable from cellular algebra theory
   plus explicit cell module bilinear form computation.
4. **Catalog completeness argument.** Prove (or bound) that the 100-value Z[zeta_8]
   catalog at delta=0 contains ALL distinct bracket values up to some braid length.
5. **Rate region analysis.** Apply Nazer-Gastpar capacity bounds to DKC. How many
   bits per bracket value can DKC extract?
