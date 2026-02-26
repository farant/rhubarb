# DKC Research Questions

Collected from 109 demos (incl. D63-D71, D72-D84, D85-D94, D95-D101, D102-D109, D39 Parts G-J), four-lenses.md, novelty.md, connections.md. Updated 2026-02-26.

---

## 1. Activation Geometry

**Q1.1. Convexity thesis formalization.** The 11/13 wall is about activation convexity (half-planes convex; multiplicative products rotate out). Can this be a formal theorem?
- Source: D57, D61-D62 | Difficulty: MODERATE | Publication: upgrades Paper 2

**Q1.2. Triskelion generalization.** ~~Does n-input parity require k=2n sectors?~~ **ANSWERED/FALSIFIED**: n=5 parity requires k=15, not k=10. The scaling law is k=2M-1 (universal k for M-ray lattice), not k=2n. FALSIFIED by Demo 63.
- Source: D50, D63 | Difficulty: N/A | Publication: superseded by Q1.10

**Q1.3. Minimum k per NPN class.** Can sector count be predicted from truth table properties (nonlinearity, correlation immunity)?
- Source: D49, D50 | Difficulty: MODERATE | Publication: systematic activation-requirement classification

**Q1.4. Non-monotonicity continuation.** 906@k=6 > 756@k=7 > 96@k=8. What happens k=9..16? Does gcd(k,8) anti-correlation persist? *Note: D63 provides k=2..16 data (Part D), but the continuation k=9..16 for 3-input is already in D50 data.*
- Source: D50, D63 | Difficulty: APPROACHABLE | Publication: strengthens incommensurability hypothesis

**Q1.5. Incommensurability as theorem.** Prove gcd(k,8) correlation rigorously (currently 3 data points).
- Source: D50, four-lenses | Difficulty: HARD | Publication: theoretical centerpiece of sector geometry

**Q1.6. Unified proof for 0x06 and 0x1B.** They fail for different reasons (parallelogram vs interval-squeeze). Single obstruction framework?
- Source: D62 | Difficulty: HARD | Publication: elevates Paper 2

**Q1.7. Multi-sector recovery of 0x06/0x1B.** k>2 recovered parity under additive (D50). Works under multiplicative?
- Source: D61 | Difficulty: APPROACHABLE | Publication: completes activation-geometry picture

**Q1.8. 11/13 deeper meaning.** Does 11/13 relate to Boolean lattice structure, S_3, or something else?
- Source: D62 | Difficulty: OPEN PROBLEM | Publication: connects combinatorics to geometry

**Q1.9. Oriented matroid type for n-input parity.** Alternating OM type characterizes 3-input parity. What type characterizes n=4 (6-sign encoding) and n=5?
- Source: D63 | Difficulty: MODERATE | Publication: generalizes OM classification

**Q1.10. Universal k theorem proof.** k=2M-1 for M-ray lattice. Proof sketch exists; needs formalization.
- Source: D63 | Difficulty: APPROACHABLE | Publication: closes the sector-geometry story

**Q1.11. n=6 constraint wall algebraic proof.** 63 subset-sum constraints on 6 integer magnitudes have no solution in Z[zeta_8] at k=15. Exhaustive search confirms; is there a clean algebraic proof?
- Source: D63 | Difficulty: HARD | Publication: distinguishes geometric from combinatorial impossibility

**Q1.12. Z[zeta_16] parity ceiling.** ~~At 16 directions (M=16), pigeonhole ceiling rises to n=15. What is the actual constraint ceiling? Is the gap always 2?~~ **ANSWERED by D65**: ceiling unchanged at n=5; gap-of-2 hypothesis refuted. Z[zeta_16] pigeonhole rises to 14 but actual ceiling stays at 5, widening gap to 9. Algebra-independent.
- Source: D63, D65 | Difficulty: N/A | Publication: answered

**Q1.13. n=6 higher-order interaction constraints.** Self-doubling (w[5]=w[i]) accounts for 89.5% of closest wall misses (34/38 cases at d=4). What algebraic or geometric structure explains the remaining 10.5%? D64 identifies them as weight-3+ errors (GHZ-type higher-order obstructions) distinct from pair-level self-doubling, but no characterization theorem exists.
- Source: D64 | Difficulty: MODERATE | Publication: completes the wall anatomy story

**Q1.14. Funnel rate formula.** Upward extensibility collapses: XOR3->XOR2 99.3%, XOR4->XOR3 39.2%, XOR5->XOR4 19.2%, XOR6->XOR5 0%. Is there a closed-form formula for the coverage fraction as a function of n (exponential decay, algebraic, other)?
- Source: D64 | Difficulty: MODERATE | Publication: structural property of the parity matroid chain

**Q1.15. k=38 anomaly.** k=38 is the only even k>=24 with zero generalized XOR6 solutions (among k=24,26,...,64 tested). 38=2x19. Is 19 special with respect to the Z[zeta_8] lattice, or the pi/4 alignment constraint?
- Source: D65 | Difficulty: MODERATE | Publication: odd gap in the even-k landscape

**Q1.16. Odd k for generalized XOR6.** Near the threshold (k=24..32), generalized XOR6 solutions exist only at even k. Does any odd k>=25 produce solutions? Is the even/odd pattern provable from pi/4 lattice alignment?
- Source: D65 | Difficulty: APPROACHABLE | Publication: completes even/odd k characterization

---

## 2. TL Algebra / Radical Structure

**Q2.1. Sandwich theorem proof.** rad^2(TL_{2k}(0)) iso TL_{2k-1}(0). Verified computationally; needs rigorous proof from cellular algebra theory.
- Source: D51 | Difficulty: MODERATE | Publication: core of Paper 3

**Q2.2. Galois symmetry conjecture.** Does sandwich duality correspond to Gal(Q(zeta_8)/Q) automorphism?
- Source: D51, four-lenses | Difficulty: HARD | Publication: rep theory meets number theory

**Q2.3. ell=7 predictions.** ~~rad(TL_7) = 11, Fibonacci rank = 8.~~ **DONE** (Demo 60, all predictions confirmed).
- Source: D52, D60 | Difficulty: N/A | Publication: confirmed

**Q2.4. Palindromic sector rank conjecture.** For even ell. Not yet tested beyond current range.
- Source: four-lenses | Difficulty: MODERATE | Publication: adds structure to Fibonacci rank theorem

**Q2.5. Catalan trace theorem novelty check.** All nonzero trace values are Catalan numbers. Needs deeper literature search.
- Source: D51 | Difficulty: APPROACHABLE | Publication: contributes to Paper 3

**Q2.6. Radical dimension formula as f(n, delta).** Universal formula across all ell values?
- Source: D52 | Difficulty: MODERATE | Publication: strong contribution if found

**Q2.7. rad^2 generator vs neglecton objects.** D49 neglecton weights from radical; D51 characterizes rad^2. Same objects? *Note: D85 characterizes the projective cover P_{0,0} structure (composition series, eigenvalue spectrum) which constrains how radical generators relate to the projective cover decomposition, but does not directly identify rad^2 generators with neglecton objects.*
- Source: D49, D52, D85 | Difficulty: MODERATE | Publication: unifies DKC arc with radical anatomy arc

**Q2.8. Markov RT truncation: cross-sector kernel structure.** At ell=4, coefficient matrix is J-deltaI (rank 2). Does this pattern generalize to other ell? Does the kernel vector always span exactly two sectors?
- Source: D39 Part J | Difficulty: MODERATE | Publication: refines Markov RT truncation theorem

**Q2.9. Second-degeneracy corank proof.** Corank = ell+1 at n=ell+2, verified ell=3..6. Provable from linking theory?
- Source: D39 Part H | Difficulty: MODERATE | Publication: extends radical structure formulas

**Q2.10. Valuation condition and larger lattices.** Why does the valuation condition p_tt = 2*p_Tt fail at TL_6? The gap is structural (cross-copy interference from 25-fold multiplicity vs 4-fold at TL_4). Is there a modified leading-coefficient extraction that avoids the divergence at larger lattice sizes?
- Source: D85, D86 | Difficulty: HARD | Publication: extends indecomposability computation beyond TL_4

**Q2.11. Literature method for b beyond TL_4.** How does Pearce-Rasmussen actually compute b = -5/8 at larger lattice sizes? The method likely uses transfer matrix + finite-size scaling, not diagram algebra. Identifying and implementing their exact finite-lattice method is essential for connecting our algebraic approach to the physics literature.
- Source: D85, D86 | Difficulty: MODERATE | Publication: bridges diagram-algebraic and lattice-physics approaches

**Q2.12. Single P_{0,0} divergence structural reason.** The delta-parameterized Gram matrix universally diverges on a single projective cover at every eigenvalue and every lattice size. The multiplicity from the regular representation is structurally essential. What representation-theoretic property makes the dim(V_0)^2 multiplicity participate in the valuation balance? Is there a different bilinear form that avoids this?
- Source: D86 | Difficulty: HARD | Publication: novel negative result about projective cover bilinear forms

**Q2.13. TL_8 P_{0,0} semisimplicity.** Why is TL_8 P_{0,0} completely semisimple (no Jordan blocks at lambda=0) when TL_4 and TL_6 have Jordan blocks? Is there a periodicity or parity pattern in the non-semisimple structure of projective covers across lattice sizes?
- Source: D86 | Difficulty: MODERATE | Publication: structural theorem about TL non-semisimplicity

**Q2.14. Spin chain b at larger sizes.** Does the spin chain approach on (C^2)^{tensor n} at q=i give b = -2 at n=6,8? The conformal (transpose) bilinear form may yield different normalization than the algebraic approach.
- Source: D85 | Difficulty: APPROACHABLE | Publication: cross-validates b via different representation

---

## 3. Coding Theory

**Q3.1. Two-channel coding conjecture.** Magnitude (~4.3 bits) + phase (~1.5 bits). Joint decoder achieves ~5.8 bits/symbol? **RELATED: D108's Dual-Channel Theorem provides the algebraic anatomy of these two channels**: multiplicative phase coherence (product closure) corresponds to the phase channel and additive magnitude diversity (v_2 connectivity) corresponds to the magnitude channel. D109 refines this with encoding-dependence: under multiplicative encoding the product closure polarity inverts. The information-theoretic quantification remains open.
- Source: D46-47, four-lenses, D108, D109 | Difficulty: HARD | Publication: standalone info-theory contribution

**Q3.2. Covering radius of Z[zeta_8] bracket catalog.** Determines quantization error bounds.
- Source: four-lenses | Difficulty: MODERATE | Publication: connects to lattice coding directly

**Q3.3. Catalog size vs theta function.** Does the hierarchy 6/56/100/116 obey a lattice theta function?
- Source: four-lenses | Difficulty: HARD | Publication: deep number-theoretic connection

**Q3.4. Catalog completeness.** Prove the 100-value catalog contains ALL distinct bracket values up to some braid length.
- Source: novelty assessment | Difficulty: MODERATE | Publication: addresses Paper 1's main weakness

**Q3.5. Rate region via Nazer-Gastpar.** How many bits per bracket value? Apply compute-and-forward capacity bounds.
- Source: novelty assessment | Difficulty: MODERATE | Publication: connects DKC to established info theory

---

## 4. Cross-ell Universality

**Q4.1. Why 11/13 is ring-independent.** Z[i], Z[zeta_5], Z[zeta_24] (dims 2,4,8) all give same wall. D62 answers geometrically; can this be sharpened?
- Source: D55, D58, D59, D61-62 | Difficulty: MODERATE | Publication: central to Paper 2

**Q4.2. Axiality hierarchy explanation.** ell=2 (1D), ell=3 (1D), ell=4 (2D), ell=5 (4D). Derivable from cyclotomic Galois theory?
- Source: D35, D53-54, D58 | Difficulty: MODERATE | Publication: strengthens Paper 4

**Q4.3. Is axiality at delta=0 known?** 131K braids verified. Must check if trivially follows from cyclotomic theory. **SUBSTANTIALLY ADDRESSED by D107**: The Z/4Z Axis-Alignment Theorem provides a constructive proof that every braid matrix entry over a TL module at delta=0 is axis-aligned, with a phase formula Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}. This is stronger than the bracket-level axiality. Researcher confirmed the nesting parity Z/2Z grading is NOT a consequence of known KLR Z-grading (Plaza-Ryom-Hansen 2013), so the result appears genuinely novel. *Formal upgrade of the bracket-level proof sketch (T1) to rigorous proof (C1) remains open.*
- Source: D35, D107 | Difficulty: APPROACHABLE (expert consultation) | Publication: D107 strengthens significance substantially

**Q4.4. Optimal ell for DKC.** Which level maximizes reachability across activations? ell=5 has rich ring but density kills DKC angular diversity.
- Source: D58 | Difficulty: MODERATE | Publication: practical DKC guidance

---

## 5. Quantum Mechanics

**Q5.1. Volume conjecture.** WRT at large r relates to hyperbolic volume. DKC implications?
- Source: D15, four-lenses | Difficulty: OPEN PROBLEM | Publication: links DKC to hyperbolic geometry

**Q5.2. Entanglement vanishing crossover.** Zero bracket for non-interleaving braids at delta=0; breaks at delta != 0. Precise crossover?
- Source: D53, four-lenses | Difficulty: MODERATE | Publication: entanglement phase transition

**Q5.3. TQC vs DKC universality independence.** PSU(2) density at ell=5 doesn't give angular diversity in bracket. Make this precise.
- Source: D05, D58 | Difficulty: HARD | Publication: distinguishes two universality notions

**Q5.4. Multi-readout as measurement basis.** Different braid closures = different TQFT measurement bases?
- Source: D21, D25 | Difficulty: MODERATE | Publication: physical meaning for closure choice

---

## 6. DKC Extensions

**Q6.1. 4-input DKC.** ~~Does parity need k=8?~~ **DONE** (Demo 63, 96 solutions at k=8; D65 confirms tier threshold k=8 at Z[zeta_16] also).
- Source: D50, D63, D65 | Difficulty: N/A | Publication: confirmed

**Q6.2. Forward-reverse duality at 4 inputs.** Spearman rho = -0.82 at 3 inputs. Holds at 222 NPN classes?
- Source: D45, D47 | Difficulty: APPROACHABLE | Publication: universality of the duality

**Q6.3. Correlation reversal at 4 inputs.** Split-sig vs MVN-cont sign flip at 3 inputs. Persists?
- Source: D47 | Difficulty: APPROACHABLE | Publication: strengthens complementary-channel thesis

**Q6.4. Minimal catalog for guaranteed XOR.** Smallest catalog guaranteeing at least one XOR triple?
- Source: D29 | Difficulty: MODERATE | Publication: constructive lower bound for DKC

**Q6.5. Solution manifold structure.** 100+ XOR triples, 906 parity solutions -- lattice, group, Z[zeta_8] automorphism orbits?
- Source: D29, D50 | Difficulty: HARD | Publication: hidden symmetry in DKC solution space

**Q6.6. Non-parity ceiling.** Parity is the hardest function (last reachable). Are there NPN classes achievable at n=6 that are NOT parity?
- Source: D63 | Difficulty: MODERATE | Publication: characterizes the full scaling landscape

**Q6.7. Solution count monotonicity at k=15.** Solutions decrease 23004->16108->3020->0 as n increases from 3 to 6. Is monotonic decrease a theorem, or could counts increase for larger n in other lattices?
- Source: D63 | Difficulty: MODERATE | Publication: structural property of DKC

**Q6.8. Clifford hierarchy universality.** ~~Does the parity ceiling n=5 climb the Clifford hierarchy one rung per additional input?~~ **ANSWERED/REFUTED by D65**: ceiling is algebra-independent -- it does NOT climb. n=5 is unchanged at Z[zeta_16]. If confirmed at Z[zeta_32] and beyond, the proof is purely topological.
- Source: D64, D65 | Difficulty: N/A | Publication: answered; proof target shifts to topology

**Q6.9. F4 orbit count formula.** The 35 Voronoi XOR6 solutions on the 24-cell decompose under F4 symmetry into exactly 6 orbits (12+7+5+5+4+2). The conjecture is that XOR_n on the 24-cell always yields n*(n-1)/2 orbits (6=5*4/2... wait, 6=C(4,2)). Connection to the partition structure of 6 (the XOR arity)?
- Source: D66 | Difficulty: MODERATE | Publication: symmetry classification of DKC solution space

**Q6.10. Hopf phase computational inertness.** ~~The Hopf fiber phase carries zero DKC computational information at every resolution tested (k=4..32). Why?~~ **ADDRESSED by D75**: angle subsumes cell for sums -- angle channel perfectly predicts parity for all 36 winning triples, and the cell is a coarsening of the angle channel. The S^2 Voronoi cell carries no information beyond what the angle (S^1 component) encodes at the computation level. Full proof from antipodal symmetry / even spherical harmonics argument still needed.
- Source: D67, D75 | Difficulty: MODERATE | Publication: theoretical heart of the S^2 arc

**Q6.11. 13=13 theorem analytical proof.** Computationally: minimum bandwidth for XOR6 on S^2 is l=6 because 2x6+1=13=number of eigenvector directions (sharp phase transition: 0% recovery at l<=5, 100% at l=6). Can this be proven analytically? Specifically: for N points on S^2 in general position, is the minimum bandwidth of any separating binary Voronoi partition l=ceil((N-1)/2)?
- Source: D71 | Difficulty: HARD | Publication: transforms a computational observation into a theorem about binary Voronoi on S^2

**Q6.12. Trivialization threshold.** XOR6 transitions from topology-driven (~36 solutions at 14 cells) to pigeonhole-dominated (~90% at 3,458 cells for zeta_16). The exact threshold lies between ~50 and ~500 Voronoi cells but has not been pinned down. D72 provides an additional data point: t=0 anti-correlation shows the algebraically-selected 13-cell configuration is locally maximal for XOR6 (36 solutions vs 4 for a geometrically optimized design). Is the transition sharp (like the l=6 bandwidth transition) or gradual?
- Source: D69, D71, D72 | Difficulty: APPROACHABLE | Publication: practical guidance for DKC activation design

**Q6.13. Gradient descent and knot-derived weights.** Demo 68 constructs two-neuron perceptron weight matrices solving XOR6 from stereographic projections of DKC solutions -- no training, from knot theory. Can gradient descent from random initialization rediscover these weights? Does it converge to the same S^2 structure implicitly?
- Source: D68 | Difficulty: APPROACHABLE | Publication: bridges DKC to practical neural network training

**Q6.14. XOR7 bandwidth on S^2.** Demo 65 showed XOR7 requires k=91 sectors on S^1. Demo 67 showed XOR6 drops from 24 sectors (S^1) to 14 cells (S^2). What is the S^2 bandwidth (and cell count) for XOR7? Does the DOF formula predict it correctly? *Note: D73 (eigenvector automaton) did not address this question.*
- Source: D65, D67, D71 | Difficulty: APPROACHABLE | Publication: tests generality of the 13=13 theorem

**Q6.15. Bloch sphere / quantum information connection.** DKC computation lives on S^2=CP^1 (the Bloch sphere of a qubit). The minimum bandwidth l=6 corresponds to hexadecapole operators in quantum optics. Is there a direct quantum-information interpretation of the XOR6 bandwidth result?
- Source: D67, D71 | Difficulty: OPEN PROBLEM | Publication: connects DKC to quantum information theory

**Q6.16. Jones XOR6 at zeta_8 vs standard sector activation.** At zeta_8, Jones normalization reduces bracket XOR8 to Jones XOR6 (D83). Is this Jones XOR6 exactly the same regime as the "standard sector activation" XOR6 capacity identified in Demo 63? If so, Jones normalization is not degradation but recovery of the baseline lattice-only capacity -- a structural coincidence linking TQFT normalization to the sector-activation framework.
- Source: D63, D83 | Difficulty: APPROACHABLE | Publication: unifies Jones normalization with activation geometry

**Q6.17. Writhe distribution of 24-cell vertices.** The 24 entries of the zeta_8 closure form the binary octahedral group. Do antipodal pairs (q and -q) have opposite writhes (writhe inversion under negation)? What is the full writhe distribution across the 24-cell, and does it reflect the symmetry group structure?
- Source: D83 | Difficulty: APPROACHABLE | Publication: connects 24-cell combinatorics to writhe geometry

**Q6.18. Encoding design for targeted Boolean functions.** The parity-lock theorem (D92) identifies the structural mechanism (+/-q per-weight pair cancellation) that restricts the encoding to XOR/XNOR. Can encodings be constructed that target specific Boolean functions (AND, OR, MAJ)? The question is whether the pair-cancellation structure can be broken while preserving the depth-law framework.
- Source: D92 | Difficulty: MODERATE | Publication: opens encoding design as a research direction

**Q6.19. +/-q encoding optimality for parity.** Is the +/-q paired encoding provably optimal for parity among all 2-bit-per-weight encodings? Among all encodings with the same number of input bits?
- Source: D92 | Difficulty: MODERATE | Publication: characterizes encoding efficiency

**Q6.20. Phase-sensitive activation to break parity lock.** The sign-flip symmetry combined_cell(S) = combined_cell(-S) is a second parity-lock mechanism. Can a phase-sensitive activation that distinguishes S from -S break the parity lock while preserving depth-law structure?
- Source: D92 | Difficulty: MODERATE | Publication: activation design for non-parity functions

---

## 7. Scaling and Capacity

**Q7.1. Anti-correlation mechanism.** ~~D72 establishes that the worst spherical t-design (t=0) produces the most XOR6 solutions (36 vs 4 for a geometrically better design). The mechanism is "algebraic not geometric" -- directions are selected by the binary octahedral group acting on Z[zeta_8] quaternions. Can this be proven as a theorem: that any t>=1 spherical design on S^2 yields strictly fewer XOR6 solutions than the algebraically-selected 13-direction configuration?~~ **ANSWERED by D88**: The anti-correlation mechanism is the two-role direction theorem: 4 non-null body-diagonal directions are rigid computational anchors (8% XOR loss under 10-deg perturbation), while 6 null-only edge-midpoint directions are flexible topological scaffolding (-2.4% loss = slight gain under perturbation). Constrained optimization (nulls free, non-nulls clamped 2 deg) achieves 48% design improvement with only 2.8% XOR loss. K-ladder activation provides 14x better perturbation resilience than Voronoi-only (6.4% vs 89% loss). *Formal theorem (any t>=1 design strictly worse) remains open but the mechanism is identified.*
- Source: D72, D88 | Difficulty: MODERATE (formal proof) | Publication: the mechanism is clear; formal version upgrades it

**Q7.2. Hidden quadrupole symmetry at l=2.** The 13 eigenvector directions achieve t=0 overall but l=2 passes to machine precision (residual 5.11e-16). This suggests a hidden quadrupole symmetry in the binary octahedral group action. What algebraic identity ensures Sum Y_{2,m}(dir_i)=0 over the 13 directions?
- Source: D72 | Difficulty: MODERATE | Publication: explains the t=2 even-mode property algebraically

**Q7.3. Perturbation non-monotonicity and local maximum.** At 1-deg perturbation the mean XOR6 count rises to 38 (above the unperturbed 36), indicating the 13-direction algebraic configuration is a local but not global maximum. What is the maximum XOR6 solution count achievable for any 13-point configuration on S^2? Is 36 a local or global maximum?
- Source: D72 | Difficulty: APPROACHABLE | Publication: characterizes the optimality landscape

**Q7.4. Universal automaton determinism.** D73 shows 82.8% (48/58) determinism is a structural constant of the zeta_8 DKC system -- identical across all 36 winning triples -- following from the binary octahedral group acting transitively on the 13 directions. Can the 82.8% fraction be improved by choosing a different entry rule (next state function) while keeping the 13-cell partition? Is there a threshold automaton step count where determinism degrades?
- Source: D73 | Difficulty: MODERATE | Publication: characterizes the automaton optimality boundary

**Q7.5. Minimum state count for 100% determinism.** D73 shows 27 cells (signed partition, no antipodal identification) achieve 87.1% determinism, still not 100%. The remaining non-determinism comes from partial sums with equal eigenvector direction but different magnitudes. Is there a 27-state (or finite-state) automaton that achieves 100% determinism for XOR6 at zeta_8? Or does 100% require tracking magnitude information and thus infinitely many states?
- Source: D73 | Difficulty: HARD | Publication: resolves minimum-state question for DKC automaton

**Q7.6. The 6 unseparated pairs.** At the qcat level, 6 braid pairs have the same Voronoi cell AND the same Kauffman bracket value. These correspond to body-type cells (2-5) with 60-deg angles. Do these pairs represent genuinely equivalent braids under some stronger invariant, or is there a finer invariant that separates all 24 quaternion catalog entries?
- Source: D74 | Difficulty: MODERATE | Publication: closes the incomparability theorem

**Q7.7. Is 119 near-optimal?** With 24 quaternions and 13 cells, the combined (bracket, cell) invariant yields 119 equivalence classes. Is this close to the theoretical maximum achievable with the length-1-to-8 braid catalog? Does the combined class count grow without bound as longer braid words are included?
- Source: D74 | Difficulty: APPROACHABLE | Publication: sharpens the combined invariant result

**Q7.8. [*,18,22] dead zone.** Winning triples containing catalog entries q18 and q22 (edge-type) separate zero bracket collisions -- they are the 3 exceptions among 36 winners for which the cell labeling exploits no bracket collisions. Is this a geometric accident (those edge cells happen to share labels) or is there a group-theoretic reason rooted in the orbit structure of edge-type cells?
- Source: D74 | Difficulty: MODERATE | Publication: completes the computational-type classification

**Q7.9. sqrt(n) magnitude growth continuation.** D75 shows the additive sum magnitude |S| takes exactly 6 values -- {0, 1, sqrt(2), sqrt(3), 2, sqrt(6)} -- reflecting the inner product structure of the 24-cell (pairwise inner products from {-1, -1/2, 0, 1/2, 1}). Does this discrete quantization of magnitude values continue for 4-term sums (XOR8 at zeta_8) or does it collapse to near-continuous values as observed in D76? Is the discreteness specific to 3-term sums, or is there a combinatorial argument for why sqrt(5) is unreachable regardless of sum length?
- Source: D75 | Difficulty: MODERATE | Publication: characterizes the magnitude channel algebraically

**Q7.10. Paired-quaternion channel necessity.** D77 shows every XOR8 winning quadruple contains exactly one pair of quaternions sharing the same eigenvector direction but differing in eigenvalue angle, and removing either pair member leaves an XOR6-winning triple. Is the paired-quaternion structure necessary for XOR8 (i.e., does every XOR8 solution decompose this way), or is it a feature specific to the 6 solutions found? Does the recursive structure extend: XOR10 winners = XOR8 winners plus another paired partner?
- Source: D75, D77, D78 | Difficulty: MODERATE | Publication: proves the recursive shadow conjecture at the next level

**Q7.11. Activation determines capacity -- formalization.** D77 demonstrates that the same zeta_8 root and same 24-element catalog yield 0 XOR8 solutions under S^2-only activation and 6 solutions under S^1xS^2 activation. Can "Activation Determines Capacity" be formalized as a theorem? Specifically: for a fixed algebraic catalog, is there a lattice ordering on activation functions under which higher activations always achieve at least as many XOR solutions, with the S^1xS^2 product sitting above pure S^1 and pure S^2 in that ordering? *Note: D83's resource decomposition (lattice base + depth + framing = additive capacity) partially formalizes this. D91 confirms activation resolution controls solution COUNT but not maximum arity -- the parity wall is the true limit.*
- Source: D77, D83, D91 | Difficulty: HARD | Publication: theoretical foundation for activation design

**Q7.12. Critical cell count universality.** D77 observes a phase transition between 84 cells (0 XOR8 winners) and 112 cells (6 winners), conjecturing a critical threshold near 100 cells related to masks-per-cell dropping below ~2.56. Is the critical cell count ~100 a universal threshold for 8-input DKC regardless of root of unity, or does it scale with catalog and activation structure? Does the masks-per-cell ~ 4.6 sweet spot observed for XOR6 at zeta_8 hold across other input sizes and roots of unity?
- Source: D76, D77 | Difficulty: MODERATE | Publication: practical design rule for DKC activation

**Q7.13. zeta_8 XOR ceiling -- wall characterization.** D78 confirms XOR10 = 0 at zeta_8 across all 42,504 quintuples and all tested sector counts. The diagnosis is catalog exhaustion: 24 quaternions with only 13 directions and 2 half-angles cannot produce 1024-mask separation. Is there a clean information-theoretic lower bound formula: max_n = f(|catalog|, n_directions, n_angles)? Is the wall at XOR8 specific to the binary octahedral group, or is it a general consequence of any ADE-type finite group? **PARTIALLY ADDRESSED by D83-D84**: D83's resource decomposition provides the formula max_xor = lattice_base(6) + 2xdepth_rounds + 2xframing_present. D84's null indispensability theorem shows that n_directions is critical (removing 6 null-only directions drops capacity by one full level), and the formula must account for directional coverage quality, not just count. **Further addressed by D85-D86**: the indecomposability parameter b = -5/8 was computed exactly at TL_4 but the method diverges at TL_6+, suggesting the wall may have deeper non-semisimplicity structure. A clean information-theoretic formalization remains open.
- Source: D78, D83, D84, D85, D86 | Difficulty: MODERATE | Publication: closes the zeta_8 capacity story

**Q7.14. Infinite group XOR capacity.** D79 shows zeta_12 (infinite group) breaks the zeta_8 wall entirely: XOR10 goes from 0 to 124 winners, XOR12 yields 50+ winners. Do ALL infinite groups under the two-perpendicular-axes construction provide unbounded XOR capacity? Or can some infinite groups have structural limitations imposing their own ceilings?
- Source: D79, D80 | Difficulty: MODERATE | Publication: characterizes the finite/infinite capacity dichotomy

**Q7.15. Computationally optimal infinite group.** Among the infinite-group roots surveyed (zeta_6, zeta_10, zeta_12, zeta_16, zeta_20), which is computationally optimal -- fewest catalog entries needed to reach XOR(n) for given n? D80 notes zeta_10 has the most distinct half-angles at 4096 entries (71 vs zeta_12's 43), suggesting richer angular diversity. Does more angular diversity per entry directly translate to lower minimum catalog size for each XOR level?
- Source: D79, D80 | Difficulty: APPROACHABLE | Publication: practical guidance for choosing the base root of unity

**Q7.16. Power-of-two finiteness conjecture.** D80 proposes that the finite groups under the two-perpendicular-axes construction are exactly those with rotation angle pi/2^k for integer k>=1. This predicts zeta_32 (theta=pi/16) should be finite. Is zeta_32 finite? Why does the geometric characterization (cube/octahedron symmetries at pi/2 and pi/4) not extend to pi/3, given that cos(pi/3)=1/2 is rational -- what algebraic property is actually determining finiteness?
- Source: D80 | Difficulty: MODERATE | Publication: closes the ADE finiteness classification

**Q7.17. E_6 and E_8 reachability.** The two-perpendicular-axes construction reaches E_7 (binary octahedral, zeta_8) and the D-series (binary dihedral, zeta_4) but not E_6 (binary tetrahedral, order 24) or E_8 (binary icosahedral, order 120). Is there a different natural generator construction that reaches E_6 or E_8? What XOR capacity would E_8 (the richest finite SU(2) subgroup) provide? **PARTIALLY RESOLVED by D94**: E_8 (2I, binary icosahedral, order 120) reached via explicit Z[sqrt5] generators (s = (1+i+j+k)/2, t = (phi + phi_inv*i + j)/2). Capacity confirmed higher than z8 at matched size. E_6 (binary tetrahedral) still unreached.
- Source: D80, D94 | Difficulty: MODERATE (E_6 remaining) | Publication: completes the ADE DKC survey

**Q7.18. Scaling constant dependence on root.** D81 establishes max_xor ~ 0.62 x log_2(catalog_size) + 4.6 for zeta_12. Does the 0.62 constant change with different infinite-group roots of unity? Is it a function of angular density per closure round, of the quaternion dimension (4D), or of the specific SecxVor activation geometry? Could a union catalog mixing zeta_10 + zeta_12 entries improve the constant or change the logarithmic character?
- Source: D81 | Difficulty: MODERATE | Publication: generalizes the scaling law

**Q7.19. Logarithmic vs sub-logarithmic scaling.** D81 observes decreasing gaps between XOR transitions (3.35 -> 2.43 -> 2.05 doublings for XOR6->8->10->12), suggesting scaling may be O(log N / log log N) rather than pure O(log N). Does the XOR14 transition (predicted at ~38K entries) follow the pattern? Is there a fundamental ceiling where logarithmic scaling saturates?
- Source: D81 | Difficulty: APPROACHABLE | Publication: resolves the scaling law character

**Q7.20. Direct generation of deep entries.** D82 shows pure depth-8 entries are 2x more efficient than shallow or strided entries at matched catalog size -- 564 deep entries reach XOR12 while 1140 mixed entries first reach XOR12 at round 6. But deep entries are currently obtained only by running the full BFS closure. Can depth-d quaternions be characterized algebraically and generated directly without computing all shallower depths? What algebraic property does strided-564 lack that deep-564 possesses (presumably shared common-factor structure from the many-generator products)?
- Source: D82 | Difficulty: HARD | Publication: practical shortcut for DKC catalog construction

**Q7.21. Depth law universality.** D82 demonstrates max_xor ~ depth + 6 for zeta_12. Does the linear depth law hold for other infinite-group roots (zeta_10, zeta_16)? Does the constant ("+6") change with root, or is it a universal offset related to the XOR6 baseline? Does it hold for finite groups with zeta_8 as control (confirmed: zeta_8 closes at depth 3, max XOR=8, consistent with depth+5)? **SUBSTANTIALLY ADDRESSED by D89-D91**: D89 killed 5 candidate mechanisms (pairwise coherence, direction coverage, Cayley density, sector saturation, angle coherence); D90 identified the actual mechanism (axis cancellation + cross-depth algebraic constraint + spectral inversion proving relational not positional); D91 confirmed the wall is parity, not activation, and validated the balanced exponentials explanation (vocabulary ~2x/round vs parity demand 4x/weight). The "+6" offset may decompose as lattice_base(6) = lattice(6) + framing(+2) - 2 per D83. *Cross-root universality (zeta_10, zeta_16) still untested.*
- Source: D82, D83, D89, D90, D91 | Difficulty: APPROACHABLE (cross-root test) | Publication: mechanism identified; universality across roots is the remaining piece

**Q7.22. Direction explosion / angle refinement phase transition.** D82 observes a clean regime boundary at depth 6-7 for zeta_12: below depth 6, new directions dominate the catalog growth (direction explosion); above depth 6, directions saturate at MAX_DIR=512 and angle refinement dominates. Is this phase transition present for all infinite-group roots? Is the transition depth related to the point at which the group's direction catalog achieves dense S^2 coverage?
- Source: D82 | Difficulty: MODERATE | Publication: structural insight into group closure dynamics

**Q7.23. Framing loss universality across roots.** D83 shows Jones normalization costs exactly 2 XOR levels at both zeta_8 and zeta_12. Does the +2 loss hold at zeta_16 and zeta_24? The loss is constant despite very different group sizes (24 vs 4096), suggesting universality, but verification at additional roots is needed. If universal, does the proof follow directly from the (-A^3)^w factor's action on angular vocabulary?
- Source: D83 | Difficulty: APPROACHABLE | Publication: extends framing-as-resource to all roots

**Q7.24. Analytical proof of "+2 per writhe unit."** The Jones phase factor (-A^3)^{-w} acts as a rotation by -(3xhalf_angle + pi)xw per entry, reducing distinct angles from 43 to 31 at zeta_12 while leaving directions unchanged (512 -> 512). Can the 2-level capacity loss be proved analytically from a direct count of angle equivalence classes before and after normalization? The mechanism is angular vocabulary reduction -- is there a closed-form expression for the number of collapsed angle classes as a function of the half-angle and writhe distribution?
- Source: D83 | Difficulty: MODERATE | Publication: theoretical heart of the framing resource

**Q7.25. Writhe constancy across depth.** D83 shows framing = +2 at the group's maximum capacity level. But at intermediate depths, does the bracket also beat Jones by exactly 2, or does the gap vary with depth? If the gap is not constant across depths, what determines the depth at which the full +2 framing resource becomes available?
- Source: D83 | Difficulty: APPROACHABLE | Publication: completes the depth x writhe interaction picture

**Q7.26. Null indispensability at zeta_12.** ~~D84 shows 121/4096 entries (3%) are bracket-null at zeta_12, with 29 null-only directions (vs 453 non-null-only, 30 shared). Are any of those 29 null-only directions essential for the XOR12 solutions found in Demo 79?~~ **ANSWERED by D87**: Null entries are directionally indispensable (67 exclusive S^2 axes) but computationally dispensable at zeta_12. Full catalog and non-null-only subset (3975 entries) both achieve identical XOR6/8/10/12 = 32 winners. The regime transition is from finite groups (zeta_8: nulls critical) to infinite groups (zeta_12: nulls redundant), driven by direction density.
- Source: D84, D87 | Difficulty: N/A | Publication: answered

**Q7.27. LCFT Jordan-cell structure made explicit.** D84 identifies 3 shared S^2 directions (coordinate axes) containing both null and non-null entries. Do these null/non-null pairs on the same axis form a literal Jordan-cell pair in some representation of the braid group algebra? Making the LCFT connection rigorous would connect DKC directly to logarithmic conformal field theory. *D85's P_{0,0} Jordan block analysis provides partial infrastructure: Jordan blocks at nonzero eigenvalues on P_{0,0} at TL_4, but the connection to the S^2 axis partition is not yet established.*
- Source: D84, D85 | Difficulty: HARD | Publication: bridges DKC to LCFT rigorously

**Q7.28. Null fraction and directional role at zeta_10.** zeta_10 (Fibonacci anyons) was identified as infinite in Demo 80 with 71 distinct half-angles at 4096 entries. The null fraction and directional role of null entries at zeta_10 has not been measured. Does the zeta_10 null fraction follow the dilution trend (75% -> 37.5% -> 3.0% for zeta_4 -> zeta_8 -> zeta_12)? *D87 confirmed the zeta_12 data point; zeta_10 remains the key gap in the dilution curve.*
- Source: D80, D84, D87 | Difficulty: APPROACHABLE | Publication: extends null-state census to Fibonacci anyons

**Q7.29. Maximum-depth null generalization.** In zeta_8, every depth-3 entry is bracket-null (100%). Is this a general theorem: does the maximum-depth stratum of any finite SU(2) subgroup consist entirely of bracket-null entries? If so, maximal depth <-> vanishing trace is a structural property of finite subgroup closures. *D87 shows zeta_12 null entries appear at depths 2-8 with null fraction increasing at greater depths (consistent), but zeta_12 is infinite so the finite-group theorem is not tested beyond zeta_8.*
- Source: D84, D87 | Difficulty: MODERATE | Publication: structural theorem about finite group closures

**Q7.30. Null entries and knot complexity.** D82 established crossing depth governs XOR capacity; D84 shows bracket-null entries are maximally deep (depth-3 in zeta_8). Do depth-d null entries correspond to specific knot types whose Jones polynomial vanishes at delta=0? If the nullness/depth/LCFT connection has a direct knot-theoretic interpretation, it would link DKC computational anatomy to classical knot theory.
- Source: D82, D84 | Difficulty: HARD | Publication: connects null-state structure to knot classification

**Q7.31. RC exactification via null-state geometry.** The RC null-state hypothesis now has a precise DKC analog (D84). Can the quaternionic DKC framework be used to design reservoir weight matrices where null-state geometry is explicitly controlled, yielding provably optimal XOR capacity per parameter? This would be a constructive application of the null indispensability theorem. *D87's finding that null dispensability is regime-dependent (critical at finite groups, redundant at infinite) refines the design space: RC exactification via null-state geometry is most impactful in the sparse-direction (finite group) regime.*
- Source: D84, D87 | Difficulty: HARD | Publication: applied DKC for reservoir computing design

**Q7.32. Null dispensability transition location.** Between zeta_8 (nulls indispensable) and zeta_12 (nulls dispensable), there must be a crossover point. Is the transition at a specific catalog size, direction count, or null fraction? Testing at intermediate sizes (e.g., zeta_10 or truncated zeta_12 catalogs) could locate it. Is per-direction spherical design residual the control parameter (D88 shows zeta_12 has 8x lower per-direction residual than zeta_8)?
- Source: D87, D88 | Difficulty: APPROACHABLE | Publication: locates the finite/infinite regime boundary precisely

**Q7.33. Zeta_12 load-bearing directions.** At zeta_12 with 2043 directions, is there ANY subset that is computationally load-bearing (analogous to the 4 body-diagonal anchors at zeta_8), or is the direction set fully redundant?
- Source: D88 | Difficulty: MODERATE | Publication: extends two-role theorem to infinite groups

**Q7.34. Golden ratio eigenvalues at other roots.** The zeta_8 |dot| Gram matrix has golden ratio eigenvalue pairs (phi=1.618, -phi+1=-0.618). Do these appear at other roots (zeta_10, zeta_12), or are they specific to zeta_8's octahedral geometry?
- Source: D88 | Difficulty: APPROACHABLE | Publication: probes universality of spectral structure

**Q7.35. Constrained optimization as SDP.** Can the constrained optimization approach from D88 (flexible null directions + rigid non-null directions) be formulated as a semidefinite program rather than simulated annealing? The constraint structure (angular clamps on some directions, freedom on others) is naturally expressible as linear matrix inequalities.
- Source: D88 | Difficulty: MODERATE | Publication: principled optimization framework for DKC direction sets

**Q7.36. Depth law slope = 1 analytical derivation.** The balanced-exponentials argument (supply ~2x/depth, demand 4x/weight -> slope 1) is consistent with all data and confirmed from complementary directions (D89 mechanism, D90 axis cancellation, D91 parity wall). But it is not derived from first principles. Connecting BFS branching factor to parity constraint growth rate would be a genuine theoretical result.
- Source: D89, D90, D91 | Difficulty: HARD | Publication: proves the depth law from first principles

**Q7.37. Depth law for non-parity Boolean functions.** D92 shows the +/-q encoding is parity-locked: AND/OR/MAJ = 0 winners at all depths. Under different encodings (e.g., 1wpi from D48/D50), does a linear depth law hold for non-parity Boolean functions? Different functions might have different constraint growth rates, giving different slopes.
- Source: D91, D92 | Difficulty: MODERATE | Publication: generalizes depth law beyond parity

**Q7.38. Depth law group dependence.** How does the depth law change with a different group (not SU(2))? Different groups have different BFS branching factors and algebraic structures, which would alter the supply side of the balanced-exponentials ratio.
- Source: D89, D91 | Difficulty: HARD | Publication: tests universality of the depth law mechanism

**Q7.39. Axis cancellation for weight selection.** D90 identifies anti-aligned axis pairs (mean min_dot = -0.75 for winners vs -0.65 for non-winners) as the geometric mechanism for the depth law. Can this be exploited directly for weight selection -- choosing tuples by maximizing anti-alignment rather than brute-force XOR testing?
- Source: D90 | Difficulty: APPROACHABLE | Publication: practical speedup for DKC weight search

**Q7.40. MAJ class conflict significance.** D92 shows MAJ has 19/27 class conflicts (the most of any tested function), vs 1/27 for AND and OR. Is this related to MAJ's computational complexity (MAJ is TC^0-complete, strictly between AC^0 and NC^1)? Is there a general formula relating a Boolean function's class conflict count to its circuit complexity?
- Source: D92 | Difficulty: MODERATE | Publication: connects encoding theory to circuit complexity

---

## 8. Resource Decomposition (D83-D84, refined by D89-D91)

**Q8.1. Additive resource decomposition proof.** D83 proposes max_xor = lattice_base(6) + 2xdepth_rounds + 2xframing_present. The "+2 per resource unit" pattern matches across depth (D82) and writhe (D83). Is this decomposition exact, or approximate? Does it hold across all roots, or only zeta_8 and zeta_12? **REFINED by D89-D91**: The depth law's mechanism is now identified (axis cancellation + cross-depth algebraic constraint, D90) and the wall is confirmed as parity, not activation (D91). The balanced exponentials explanation provides the supply/demand framework. A full proof would require showing that depth, writhe, and lattice structure contribute independently to angular vocabulary -- no interaction terms.
- Source: D82, D83, D89, D90, D91 | Difficulty: MODERATE | Publication: foundational formula for DKC capacity theory

**Q8.2. N-2 pattern as Jones capacity.** ~~The "N-2" observation from D79 (capacity appeared to stop 2 levels below what the bracket achieves).~~ **RESOLVED by D83**: the N-2 pattern was the Jones polynomial's capacity all along, not a bracket truncation artifact. Earlier demos that stopped at XOR10 at zeta_12 were inadvertently computing Jones-normalized quantities. The bracket genuinely reaches XOR12.
- Source: D79, D83 | Difficulty: N/A | Publication: resolved

**Q8.3. TQFT anomaly as computational resource -- deeper implications.** D83 demonstrates that what TQFT calls a "framing anomaly" (the writhe-dependent phase factor removed by Jones normalization) is not noise but computation -- exactly the component contributing 2 XOR levels. Does this have implications for topological quantum computation? If the framing anomaly carries computation, should TQC schemes preserve rather than normalize it?
- Source: D83 | Difficulty: OPEN PROBLEM | Publication: potentially transformative for TQC design philosophy

---

## 9. Parity-Lock and Encoding Theory (NEW -- D92)

**Q9.1. 1wpi encoding depth law.** Does the 1-weight-per-input encoding from D48/D50 (which computes all 13 NPN classes) follow the depth law? Under 1wpi, the encoding is not parity-locked, so the depth law may have different characteristics (slope, intercept) for different Boolean functions.
- Source: D48, D50, D91, D92 | Difficulty: APPROACHABLE | Publication: bridges the two encoding regimes

**Q9.2. Hybrid encoding design.** Can a mixed encoding (some weights +/-q paired, some 1wpi) achieve a richer set of Boolean functions while maintaining some depth-law structure? The trade-off between parity-optimality and function diversity is unexplored.
- Source: D92 | Difficulty: MODERATE | Publication: encoding design space exploration

**Q9.3. Complement-blindness under +/-q encoding.** D92 seeds D93 on complement-blindness: the sign-flip symmetry combined_cell(S) = combined_cell(-S) means the encoding cannot distinguish a mask from its bitwise complement. This is the second parity-lock mechanism. Full characterization of which Boolean function families are complement-blind vs complement-sensitive.
- Source: D92 | Difficulty: APPROACHABLE | Publication: completes the parity-lock analysis

---

## 10. Indecomposability and Non-Semisimplicity (NEW -- D85-D86)

**Q10.1. b and DKC computational capacity.** The k_sec=1 synergy value (36) from Demo 84 was hypothesized to encode the indecomposability parameter b. b = -5/8 is now computed at TL_4 (D85). Is there a quantitative relationship between b and the DKC XOR capacity?
- Source: D84, D85 | Difficulty: MODERATE | Publication: connects LCFT structure constant to computational capacity

**Q10.2. P_{0,0} dimension = next Catalan number.** D86 observes dim(P_{0,0}) at TL_n = C_{n/2+1}: TL_4 -> 5 = C_3, TL_6 -> 14 = C_4, TL_8 -> 42 = C_5. Is this a theorem or coincidence? If a theorem, what is the proof from cellular algebra structure?
- Source: D86 | Difficulty: APPROACHABLE | Publication: structural result about TL projective covers

**Q10.3. Projective-cover-exclusive eigenvalues.** TL_6 P_{0,0} has eigenvalues +/-(sqrt(3)+/-1) that do not appear on the standard module W_0. Do these eigenvalues encode representation-theoretic information about the projective cover's non-semisimple structure? Is there a formula for P_{0,0} eigenvalues in terms of lattice size?
- Source: D86 | Difficulty: MODERATE | Publication: spectral theory of TL projective covers

**Q10.4. Dense polymer fusion calculation.** Gainutdinov et al. (2013) describe dense polymer fusion at p=2. Can this algebraic framework describe DKC composition (how bracket values combine under quaternion sums)?
- Source: D86 | Difficulty: HARD | Publication: LCFT fusion rules for DKC

---

## 11. Circuit Complexity and Group Structure (NEW -- D93-D94)

**Q11.1. Does the depth law hold under phase_cell?** The depth law (max_xor ~ depth + 6) was established under combined_cell (D82-D91). Phase_cell removes the sign-flip symmetry (sector spans [0,360) instead of [0,180)). Does the linear relationship still hold? If so, does the slope change? Phase_cell recovers all 13 NPN classes, so the depth law may have different characteristics for different Boolean functions.
- Source: D93 | Difficulty: APPROACHABLE | Publication: generalizes the depth law to the full-function activation regime

**Q11.2. Does higher k_sec push the pigeonhole wall?** ~~Is the pigeonhole wall a hard boundary or an activation-parameter-dependent threshold?~~ **ANSWERED by D105**: The pigeonhole wall IS activation-parameter-dependent. D93's "XOR dies at N>=7" is a k-regime artifact. At k=4096, W_{8,0} produces XOR8=22266. The controlling parameter is the ratio k/2^N: at k=128 for N=8, k/2^N=0.5 (deep pigeonhole); at k=4096, k/2^N=16 (collision avoidance regime) and XOR is fully alive. Confirmed across both n=6 and n=8.
- Source: D93, D94, D105 | Difficulty: N/A | Publication: ANSWERED; k-regime theory established

**Q11.3. Is the MAJ cliff a threshold phenomenon?** MAJ drops from 68.7% (N=7) to 2.4% (N=8) under phase_cell — a 28x collapse. Is this a sharp threshold (TC^0-complete meets pigeonhole at a critical N) or gradual degradation that happens to cross a visibility threshold? The mechanism may be related to MAJ's full-degree Fourier structure (intermediate between AND's low-degree and XOR's full-degree).
- Source: D93 | Difficulty: MODERATE | Publication: connects DKC circuit complexity to TC^0 theory

**Q11.4. RKHS kernel rank: does rank(K_2I)/rank(K_z8) > 120/24?** Compute the DKC kernel K(m,m') = quaternion inner product of signed sums for 2I vs z8. If the rank ratio exceeds the catalog size ratio, non-solvability contributes above raw size. One number that validates or falsifies the "discrete algebraic reservoir computer" interpretation.
- Source: D94 | Difficulty: APPROACHABLE | Publication: THE quantitative test of the reservoir computing pillar

**Q11.5. What is the N=6-7 crossover mechanism for 2I vs z12?** In Phase 4, zeta_12 (51 entries, infinite) wins at N=3-5 (angular variety), but 2I (60 entries, non-solvable finite) overtakes at N=6-7 (computational boundary). Is the crossover related to 2I's 9 conjugacy classes (discrete angular structure) vs z12's continuous distribution? What structural feature of non-solvable finite groups is load-bearing at the computational boundary?
- Source: D94 | Difficulty: MODERATE | Publication: identifies the algebraic property enabling non-solvable group advantage

**Q11.6. Circuit complexity hierarchy universality.** D93 demonstrated the AND/XOR ratio explosion (1.01 to infinity, N=3-8) under phase_cell with z12. D94 confirmed the same hierarchy with 2I. Is the hierarchy identical under combined_cell (where XOR is the ONLY function)? The hierarchy is a property of the readout mechanism (1wpi + activation), not the group — but does the group structure affect the RATE of divergence?
- Source: D93, D94 | Difficulty: MODERATE | Publication: separates readout-determined hierarchy from group-dependent scaling

**Q11.7. XOR growth curve shape universality.** D93 found N=6 XOR hit rate grows logarithmically with depth (growth factors 3.4x to 1.1x, d=1 to d=6). Is this shape universal across N, or does it change? Does the logarithmic saturation connect to Barrington scaling (NC^1 programs over non-solvable groups)?
- Source: D93 | Difficulty: MODERATE | Publication: refines the depth law for individual functions

**Q11.8. Size vs solvability clean disentangling.** D94 Phase 2 controls for size (24 vs 24), Phase 4 shows crossover, but catalog sizes differ (2I=60 vs z12=51). A sharper test: 2I truncated to exactly 51 entries vs z12 at 51 entries. Also: can a solvable group of order 120 be constructed to test solvability independent of order?
- Source: D94 | Difficulty: APPROACHABLE | Publication: removes the last confound in the Barrington validation

---

## 12. Multi-Strand DKC and Representation Theory (NEW -- D95-D101)

**Q12.1. Does the sl_d growth rate hold at n=6?** ~~Three data points confirm the pattern; a fourth would establish it beyond coincidence.~~ **ANSWERED by D102-D103-D105**: All three n=6 modules (W_{6,0}, W_{6,2}, W_{6,4}) produce bit-for-bit IDENTICAL BFS depth profiles with growth rate ~5x (10.00, 6.60, 5.48, 4.96, 4.67), confirming sl_5 prediction. Furthermore, D105 extends to n=8: W_{8,0} shows ~7x growth, consistent with sl_7. Five data points now: n=3 (~2.2x), n=4 (~3.1x), n=5 (~4.0x), n=6 (~5.0x), n=8 (~7x). Growth rate is confirmed as a braid group invariant, independent of module choice or dimension.
- Source: D98, D100, D101, D102, D103, D105 | Difficulty: N/A | Publication: ESTABLISHED

**Q12.2. Why does hub count NOT scale as predicted?** The sl_d functor thesis predicted 2*C(n-1,2) super-hubs (6 for sl_3, 12 for sl_4). D100 confirmed 6 for sl_3 (W_{4,2}, non-semisimple). D101 found only 3 for sl_4 (W_{5,3}, simple). Three explanations: (1) simple modules have fundamentally different hub structure, (2) bf=30 sampling window misses hubs, (3) hub-count prediction was wrong. The decisive test: W_{6,4} (non-semisimple, sl_5) -- if it has more hubs, explanation (1) wins. **UPDATED by D102-D103**: Both W_{6,0} and W_{6,4} at n=6 show ZERO super-hubs at >33% threshold; frequency distributions are flat (max freq 198/2449 for W_{6,0}, 173-186 for W_{6,2}). W_{6,4} had 0 XOR10 winners entirely. The hub phenomenon appears to diminish with strand count -- qualitatively different from D100-D101. Whether this is a strand-count effect or a dimension/sampling effect remains open.
- Source: D100, D101, D102, D103 | Difficulty: MODERATE | Publication: determines whether hub count tracks semisimplicity or something else

**Q12.3. Is the Casimir inversion universal for simple modules?** D100 (non-semisimple W_{4,2}): Casimir-XOR correlation is uniformly positive (1.36x-1.86x, winners have HIGHER Casimir). D101 (simple W_{5,3}): correlation INVERTS at XOR10-12 (0.89x, 0.54x, winners have LOWER Casimir). Does Casimir inversion occur in every simple TL module, or is it specific to W_{5,3}? **UPDATED by D102**: W_{6,0} (simple, dim=5) Casimir ratio at XOR6 = 1.15x (winners slightly higher), but higher XOR levels are untestable (only 1 XOR10 winner). The n=6 data is consistent with either weak positive correlation or inversion at depth, but insufficient to resolve. Testing at W_{3,1} (simple, 2-strand) or with larger catalogs at n=6 remains the decisive experiment.
- Source: D100, D101, D102 | Difficulty: MODERATE | Publication: identifies semisimplicity as the control variable for Casimir-computation relationship

**Q12.4. Can optimal catalogs be constructed from first principles?** D96 showed the optimal z8 catalog is A+B+C (21 entries, beats ALL 24 by pruning Q8-null noise). D97 showed Cell B's perfection arises from orthogonal-frame geometry at any angle in a 50-degree plateau. Can the pruning principle + orthogonal-frame recipe be combined into a constructive procedure that produces optimal catalogs for any DKC group without brute-force XOR testing?
- Source: D96, D97 | Difficulty: HARD | Publication: constructive catalog optimization from algebraic structure

**Q12.5. What activation recovers XOR14 on W_{5,3}?** D101 has XOR14=0 at all k values (128-4096) with 4x4_hash activation on the simple module. D100 achieves XOR14=70 on the non-semisimple 3x3 module. Is D101's XOR14=0 a module effect (simple = less capacity) or an activation effect (64 sign components too fine-grained)? **UPDATED by D104-D105**: D104 demonstrates the "Atkinson sweet spot" -- optimal component count for W_{6,2} at k=128 is 120/324, and k is the real lever, not hash architecture. D105 shows W_{8,0} (dim=14, 784 components) achieves XOR8=22266 at k=4096 despite massive component count. The k-regime theory suggests D101's XOR14=0 may be a k-regime artifact rather than a module effect, since k/2^N ratios were likely too low. Testing W_{5,3} with high-k (k=4096+) combined with Atkinson-optimal component selection would be decisive.
- Source: D99, D100, D101, D104, D105 | Difficulty: APPROACHABLE | Publication: separates module from activation effects in multi-strand DKC

**Q12.6. Readout bottleneck generalization.** D98 showed trace readout catastrophically lossy for 3-strand (5.1% retention). D99 showed column-vector readout preserves 100% diversity (column 4). For higher strands (4, 5, ...), does the trace become progressively worse? Is column-vector readout always lossless for the last column? Is there a general formula for trace information loss as a function of strand count?
- Source: D98, D99 | Difficulty: APPROACHABLE | Publication: characterizes information-theoretic cost of standard readouts

**Q12.7. Two-regime universality.** D99 discovered two computational regimes: combinatorial (XOR6-8, shallow, diversity-driven) and algebraic (XOR10-14, deep, Ext^1-catalyzed). Does this two-regime structure appear in D100 and D101? D100's Casimir data shows consistently positive correlation (no inversion) -- does D100 have a different transition point, or is the two-regime structure specific to the 2x2 Delta_1 representation?
- Source: D99, D100, D101 | Difficulty: MODERATE | Publication: tests whether the two-regime hypothesis is representation-dependent

**Q12.8. Commuting hubs and commutative subalgebra.** D101's three super-hubs MUTUALLY COMMUTE (all commutators = zero matrix). D100's six super-hubs have traceless but nonzero commutators. Is the commuting structure specific to simple modules? Does the commutative subalgebra of the hub set have a name in the representation theory of B_5 on W_{5,3}? Is it the image of a maximal abelian subalgebra of the group ring?
- Source: D100, D101 | Difficulty: MODERATE | Publication: structural theorem about hub algebra

**Q12.9. Broad vs deep capacity -- simple vs non-semisimple.** D101 has MORE XOR6-8 solutions than D100 (2579 vs 2017, 2228 vs 1169) but FEWER XOR10-14 solutions (203 vs 654, 27 vs 305, 0 vs 70). Is this "broader but shallower" pattern general for simple modules? Does non-semisimplicity specifically enable deep (high-XOR) computation at the expense of shallow breadth? **UPDATED by D102**: W_{6,0} (simple, dim=5) beats W_{6,4} (non-simple, dim=5) at EVERY XOR level: XOR6 2449>2370, XOR8 850>793, XOR10 1>0. This contradicts the D100/D101 pattern and supports the Barrington-Radical Principle: the radical carries only abelian writhe character, which is useless for parity by Barrington's theorem. At n=6, the simple module is uniformly better, not just "broader but shallower." The D100/D101 pattern may be specific to lower strand counts or non-semisimple modules with richer radical structure.
- Source: D100, D101, D102 | Difficulty: MODERATE | Publication: characterizes the computational signature of non-semisimplicity

**Q12.10. Cross-layer synergy universality.** D95 showed that COMM and NON-COMM subsets perform similarly alone but ALL(24) dramatically outperforms both (2.7x). D84 showed 0+0=36 null synergy. D96 confirmed these are independent partitions creating a 2D landscape. Does this cross-layer synergy mechanism appear in 2I? In multi-strand matrix catalogs? **UPDATED by D102**: The mixing row of W_{6,4} provides +8% XOR6 (2370 full vs 2195 quotient-only), demonstrating that the radical/quotient mixing is a weak computational resource. However, the simple module W_{6,0} still beats the non-simple W_{6,4} overall. Cross-layer mixing contributes but does not overcome the Barrington-Radical disadvantage at n=6.
- Source: D84, D95, D96, D102 | Difficulty: MODERATE | Publication: establishes cross-layer synergy as a universal DKC design principle

---

## 13. Radical Structure and Fibonacci Constraints (NEW -- D102)

**Q13.1. Why does W_{6,0} break Fibonacci at d=6?** W_{6,4} max coefficient magnitude follows strict Fibonacci (1,1,2,3,5,8,13) through depth 6, but the simple module W_{6,0} reaches 16 at d=6 (breaking from expected 13). Does the radical act as a constraint that keeps coefficients on the Fibonacci sequence? If so, this is another manifestation of the radical carrying structured (abelian) information.
- Source: D102 | Difficulty: MODERATE | Publication: Fibonacci constraint from radical structure

**Q13.2. Mixing row as weak computational resource -- does it scale?** W_{6,4} B_full (2370) > B_quotient-only (2195) at XOR6, an 8% boost from the mixing row encoding how the quotient block "leaks" into the radical direction. Is this boost consistent across strand counts? Does the mixing row effect grow, shrink, or stay constant at n=8, n=10?
- Source: D102 | Difficulty: APPROACHABLE | Publication: quantifies radical computational contribution

**Q13.3. Optimal activation component count -- coincidence or design principle?** D102 uses 100 components (5x5 x 4) for W_{6,0} full activation, and D104 shows ~100-120 components is the Atkinson sweet spot. Is this a coincidence, or does W_{6,0}'s natural dimensionality happen to hit the optimal activation regime? For arbitrary modules, is the sweet spot always near dim^2 * (cyclotomic rank)?
- Source: D102, D104 | Difficulty: MODERATE | Publication: connects module dimensionality to activation design

---

## 14. BFS Growth and Activation Design (NEW -- D103-D105)

**Q14.1. Why does B@sub7 beat A@full?** W_{6,2} at 196 sign components (7x7 subblock of 9x9) achieves XOR6=2538, beating W_{6,0}'s 2449 from 100 components of a 5x5 matrix. The 7x7 subblock contains algebraic information absent from the 5x5 representation. Is this related to the 2 through-lines in W_{6,2}? What specific algebraic information is in the sub7 block?
- Source: D103 | Difficulty: MODERATE | Publication: identifies algebraic information gain from higher-dimensional modules

**Q14.2. Universal BFS tree conjecture.** Do ALL simple TL_n modules at delta=0 produce identical BFS trees for the same n? D102-D103 show all three n=6 modules (W_{6,0}, W_{6,2}, W_{6,4}) have identical BFS trees. Testing at n=8 (W_{8,0} vs W_{8,2}, both confirmed simple at delta=0) would extend the conjecture. If true for all simple modules, is it also true for non-simple modules? (D102's W_{6,4} is non-simple and matches.)
- Source: D102, D103, D105 | Difficulty: MODERATE | Publication: structural theorem about braid group image independence from module choice

**Q14.3. Is the Atkinson peak k-dependent?** At k=128, W_{6,2}'s peak is at 120 components. Does the peak shift to more components at higher k? The sign-rank of 292 suggests much more information could be exploited at higher k. A component sweep at k=4096 would answer this.
- Source: D104 | Difficulty: APPROACHABLE | Publication: practical activation design guidance

**Q14.4. Sign-rank expansion mechanism.** Sign quantization (ternary {-1,0,+1}) can INCREASE effective rank beyond raw Z-linear rank: W_{6,2} goes from 244 to 292 (+48 independent directions). What determines whether sign quantization expands or preserves rank? W_{6,0}: preserved (100=100). W_{8,2}: dramatic expansion (1.83x at 16384 entries, still growing). Is expansion related to the number of "nearly independent" Z-linear dependencies?
- Source: D104, D105 | Difficulty: MODERATE | Publication: foundational result for sign-hash activation theory

**Q14.5. Macrame principle crossover exact location.** Branching interaction (cross-block) dominates at block dim 4-5 (D104, W_{6,2}), within-block dominates at block dim 14 (D105, W_{8,2}). Where is the exact crossover? W_{6,4} (block dims involving 1+4) or W_{10,2} could bracket it. Is the crossover a function of block dimension ratio, absolute dimension, or some other structural property?
- Source: D104, D105 | Difficulty: MODERATE | Publication: regime boundary for activation design

**Q14.6. W_{8,2} true saturated rank.** At 16384 entries / 3136 cols = 5.2x oversampling, W_{8,2} is still at 34.9% raw rank and 63.9% sign-rank, both still growing. Would need ~130K entries for comparable oversampling to W_{8,0}. What is the true saturated rank fraction? Does sign-rank eventually approach 100% as for W_{8,0}?
- Source: D105 | Difficulty: APPROACHABLE | Publication: honest rank measurement for large TL modules

**Q14.7. General cross-block recipe for W_{n,j}.** Is the optimal activation always the cross-block under TL_{n-1} branching when blocks are small? D104 says yes at n=6 (asymmetric 5+4), D105 says no at n=8 (symmetric 14+14). What about intermediate cases? Is there a formula: cross-block wins when max(block_dim) < threshold?
- Source: D104, D105 | Difficulty: MODERATE | Publication: design principle for multi-strand DKC activation

---

## 15. TL Visibility and Dynamical Properties (NEW -- D106)

**Q15.1. What predicts per-entry XOR participation strength?** If not topological entropy, what algebraic property of the TL matrix determines how often an individual entry appears in XOR-computing triples? D106 shows entropy is completely orthogonal; D107's Raqiya analysis measures relational structure between values; D108 identifies the dual-channel as the discriminator. But the per-ENTRY (not per-value-set) predictor remains unknown.
- Source: D106 | Difficulty: MODERATE | Publication: closes the individual-entry question from D106

**Q15.2. Does the entropy null extend to higher strand counts?** D106 uses W_{4,2} (4-strand). The TL visibility filter argument (e_i^2=0 kills expanding eigenvalues) is representation-independent, so the null should hold on W_{5,3}, W_{6,4}, etc. Confirmation at n=6 or n=8 would strengthen the claim from "demonstrated" to "universal."
- Source: D106 | Difficulty: APPROACHABLE | Publication: universality of the TL visibility filter

**Q15.3. Can entropy predict ANYTHING in DKC?** The null is total for Boolean function computation. But could entropy correlate with other catalog properties (hub frequency, co-occurrence topology, Raqiya graph structure)? Testing entropy vs D107's graph density metrics would be informative.
- Source: D106 | Difficulty: APPROACHABLE | Publication: exhausts the entropy investigation

---

## 16. Algebraic Graph Structure and Nesting Parity (NEW -- D107)

**Q16.1. Does the value set converge?** W_{4,2} has 186 distinct Z[zeta_8] values at depth 8 with 72 new at that depth -- still growing fast. Need deeper catalogs (beyond 32768 cap) to check whether the value set is finite or infinite for this module.
- Source: D107 | Difficulty: APPROACHABLE | Publication: fundamental finiteness question for DKC value algebra

**Q16.2. zeta_12 axis-alignment prediction.** At delta=sqrt(3) != 0, expect mostly axis-aligned values but with occasional mixing. Amount of mixing proportional to |delta|. Pre-registered prediction from D107 -- needs computation.
- Source: D107 | Difficulty: APPROACHABLE | Publication: tests scope of axis-alignment beyond delta=0

**Q16.3. Major index theorem for all W_{n,j}.** Verified at W_{4,2} (3/3 match) and W_{6,2} (9/9, the ONLY winning candidate out of 4 tested). Need W_{8,2}, W_{6,4}, etc. for more evidence. A proof for general n,j is open. For j=0 modules, maj mod 2 = nest mod 2 (special case).
- Source: D107 | Difficulty: MODERATE | Publication: novel combinatorial theorem on TL link-state graphs

**Q16.4. 2-adic neutrality formal proof.** v_2 distribution of TL matrix entries decays geometrically (95->48->24->11->7, approx halving per level). Five converging proof frameworks identified (Kummer carry-counting, nilpotent TL, Goodman-Wenzl, unipotent, Stirling) -- need to select one and formalize.
- Source: D107 | Difficulty: MODERATE | Publication: novel number-theoretic property of TL algebras

**Q16.5. Strongly regular graph parameters.** Some subgraphs of D107's Raqiya analysis may have (n,k,lambda,mu) strongly regular parameters. Not yet checked. Would connect DKC value algebra to algebraic graph theory.
- Source: D107 | Difficulty: APPROACHABLE | Publication: connects DKC to algebraic combinatorics

**Q16.6. q-Catalan at other roots of unity.** The q-Catalan identity C_{2m+1}(-1) = (-1)^m * C_m gives bipartition sizes. Does q-Catalan have clean evaluations at other roots (q = zeta_3, zeta_4, i, etc.)? Would give bipartition refinements for finer group actions on TL bases.
- Source: D107 | Difficulty: MODERATE | Publication: extends the q-Catalan identity family

---

## 17. Dual-Channel Theory and Encoding Dependence (NEW -- D108-D109)

**Q17.1. Dual-channel prediction for XOR6 at k>=24.** Can the dual-channel analysis (product closure + v_2 connectivity) predict which custom sector labelings at k>=24 achieve XOR6? This extends D65's custom-activation work with D108's structural framework.
- Source: D108 | Difficulty: MODERATE | Publication: predictive application of dual-channel theory

**Q17.2. Z[i] product closure at delta=sqrt(2).** Does the Z[i] lattice have intrinsically better product closure than Z[zeta_8]? D55's 9334-solution explosion at delta=sqrt(2) might be explained by richer multiplicative structure in Z[i].
- Source: D108 | Difficulty: APPROACHABLE | Publication: connects dual-channel theory to cross-delta capacity differences

**Q17.3. Parity-lock as dual-channel constraint.** Is D92's parity-lock (+-q encoding -> only parity achievable) precisely the statement that +-q encoding constrains to exactly one dual-channel configuration? If so, parity-lock is a special case of dual-channel theory.
- Source: D108 | Difficulty: MODERATE | Publication: unifies parity-lock with dual-channel framework

**Q17.4. Product closure threshold for parity.** How much product closure and v_2 connectivity does a value set need for parity? Is there a minimum graph density threshold? Quantitative version of the dual-channel necessary conditions.
- Source: D108 | Difficulty: MODERATE | Publication: makes dual-channel quantitative

**Q17.5. Non-monotonic solution count prediction.** Does dual-channel structure predict 906@k=6 > 756@k=7 > 96@k=8? The graph structure of the parity vocabulary changes with k (different sector boundaries produce different parity/poison/neutral classifications). Can the dual-channel metrics be computed per-k and correlated with solution count?
- Source: D108 | Difficulty: MODERATE | Publication: bridges dual-channel to the incommensurability phenomenon

**Q17.6. Overlap mechanism between k=6 and k=15.** 14 values are simultaneously parity-capable (k=6) and D64-poison (k=15). What changes in graph structure that flips their computational role? Is it the product closure of the VALUE SET at different k, or the product closure of the value within its k-specific PARTITION?
- Source: D108 | Difficulty: MODERATE | Publication: resolution-dependent computational role mechanics

**Q17.7. Module-specific dual-channel test.** Does the dual-channel distinction hold for module-specific value sets (e.g., W_{8,0} vs W_{8,2})? D108 uses the standard 2-strand bracket catalog; testing matrix-level value sets from higher-strand modules would generalize the result.
- Source: D108 | Difficulty: APPROACHABLE | Publication: extends dual-channel from bracket to multi-strand DKC

**Q17.8. Additive encoding at delta=sqrt(2).** D108 shows parity wants HIGH product closure under additive encoding. D109 shows parity wants LOW product closure under multiplicative encoding at delta=sqrt(2). Does additive encoding at delta=sqrt(2) restore D108-style structural discrimination? This would directly confirm encoding-dependence by holding delta constant and varying encoding.
- Source: D108, D109 | Difficulty: APPROACHABLE | Publication: THE decisive test of encoding-dependent dual-channel

**Q17.9. Angular discriminator for Z[zeta_16] parity.** D109 shows parity and non-parity are structurally indistinguishable on 7/8 edge types at delta=sqrt(2). What distinguishes the 28 parity values from 28 non-parity if not algebraic structure? Is it purely angular position relative to Re>0? Can the Re(Gaussian) bias be quantified into a single scalar discriminator?
- Source: D109 | Difficulty: MODERATE | Publication: identifies the discriminating feature at rich-algebra regimes

**Q17.10. Activation invariance of the 7/8 symmetry.** Does the perfect structural symmetry between parity and non-parity at delta=sqrt(2) hold at other activations (k=4, k=8, Im>0)? If so, the symmetry is intrinsic to the Z[zeta_16] algebra and not an artifact of Re>0 activation choice.
- Source: D109 | Difficulty: APPROACHABLE | Publication: tests intrinsicality of the algebraic symmetry

**Q17.11. 50% vocabulary as structural constant.** Is the 28/28 = 50% parity/non-parity split an artifact of Re>0, or does every activation at delta=sqrt(2) give exactly 50%? If universal, this reflects a deeper algebraic automorphism of the Z[zeta_16] bracket value set.
- Source: D109 | Difficulty: APPROACHABLE | Publication: potential algebraic automorphism result

**Q17.12. Quantitative product-closure threshold.** At what product closure density does the additive-to-multiplicative encoding transition occur? Is there a sharp threshold or gradual crossover between the D108 regime (parity=high closure) and D109 regime (parity=low closure)?
- Source: D108, D109 | Difficulty: MODERATE | Publication: quantifies the encoding-dependent dual-channel boundary

**Q17.13. Raqiya Z[zeta_16] extension.** Is it worth building full Raqiya support for Z[zeta_16] (currently handled by ad-hoc ZiAxis type in D109) if more delta!=0 analysis is needed? The Scrutinium generic framework could absorb Z[zeta_16] as a new genus.
- Source: D109 | Difficulty: APPROACHABLE | Publication: tooling decision

**Q17.14. j=0 liveness implies structural universality.** At delta=0, j=0 dead implies impoverished algebra implies structural asymmetry detectable by Raqiya. At delta!=0, j=0 alive implies rich algebra implies structural universality (7/8 symmetry). Does this hold at other delta values (delta=1, delta=2)? Would generalize the D52 j=0 liveness observation into a structural dichotomy theorem.
- Source: D109, D52 | Difficulty: MODERATE | Publication: structural dichotomy linking j=0 to algebraic richness

---

## Priority Ranking

**Tier 1 -- Immediate (APPROACHABLE, high impact):**
Q1.10 (universal k proof), Q1.4 (k=9..16 counts), Q6.2 (duality at 4 inputs), Q4.3 (axiality -- SUBSTANTIALLY ADDRESSED by D107), Q1.7 (multi-sector multiplicative), Q1.16 (odd k generalized XOR6), Q6.12 (trivialization threshold -- D72 provides new data point), Q6.13 (gradient descent to knot weights), Q6.14 (XOR7 bandwidth on S^2), Q6.16 (Jones XOR6 vs standard sector activation), Q6.17 (writhe distribution of 24-cell), Q7.3 (perturbation non-monotonicity), Q7.7 (119 near-optimal?), Q7.15 (optimal infinite group), Q7.16 (zeta_32 power-of-two conjecture), Q7.19 (logarithmic vs sub-logarithmic -- XOR14 test), Q7.23 (framing loss universality -- test at zeta_16, zeta_24), Q7.25 (writhe constancy across depth), Q7.28 (null fraction at zeta_10), Q7.32 (null dispensability transition), Q7.34 (golden ratio eigenvalues at other roots), Q7.39 (axis cancellation for weight selection), Q7.21 (depth law cross-root universality -- mechanism identified by D89-D91, cross-root test remaining), Q2.14 (spin chain b at larger sizes), Q9.1 (1wpi encoding depth law), Q9.3 (complement-blindness), Q10.2 (P_{0,0} = next Catalan), Q11.1 (depth law under phase_cell), ~~Q11.2~~ (ANSWERED by D105), Q11.4 (RKHS kernel rank test -- HIGHEST PRIORITY: one number validates five-pillar synthesis), Q11.8 (size vs solvability disentangling), ~~Q12.1~~ (ANSWERED by D102-D103-D105), Q12.5 (activation for XOR14 on W_{5,3} -- UPDATED by D104-D105), Q12.6 (readout bottleneck generalization), Q13.2 (mixing row scaling -- NEW from D102), Q14.3 (Atkinson peak k-dependence -- NEW from D104), Q14.6 (W_{8,2} true saturated rank -- NEW from D105), Q15.2 (entropy null at higher strands -- NEW from D106), Q15.3 (entropy vs Raqiya metrics -- NEW from D106), Q16.1 (value set convergence -- NEW from D107), Q16.2 (zeta_12 axis-alignment prediction -- NEW from D107), Q16.5 (strongly regular parameters -- NEW from D107), Q17.2 (Z[i] product closure at delta=sqrt(2) -- NEW from D108), Q17.7 (module-specific dual-channel -- NEW from D108), Q17.8 (additive encoding at delta=sqrt(2) -- NEW from D109, HIGHEST PRIORITY for dual-channel: THE decisive test), Q17.10 (activation invariance of 7/8 symmetry -- NEW from D109), Q17.11 (50% vocabulary structural constant -- NEW from D109), Q17.13 (Raqiya Z[zeta_16] extension -- NEW from D109)

**Tier 2 -- Soon (MODERATE, strengthens papers):**
Q2.1 (sandwich proof), Q3.4 (catalog completeness), Q1.9 (OM generalization), Q2.8 (cross-sector kernel), Q6.6 (non-parity ceiling), Q1.13 (n=6 higher-order constraints), Q1.14 (funnel rate formula), Q1.15 (k=38 anomaly), Q6.9 (F4 orbit count formula), Q6.10 (Hopf phase inertness -- partially addressed by D75, proof still needed), Q7.1 (anti-correlation formal proof -- mechanism identified by D88), Q7.2 (hidden quadrupole symmetry), Q7.4 (universal automaton determinism), Q7.6 (6 unseparated pairs), Q7.8 ([*,18,22] dead zone), Q7.9 (sqrt(n) magnitude continuation), Q7.10 (paired-quaternion channel necessity), Q7.12 (critical cell count universality), Q7.13 (zeta_8 XOR ceiling formula -- PARTIALLY ADDRESSED by D83/D84/D85/D86), Q7.14 (infinite group capacity), Q7.18 (scaling constant vs root), Q7.22 (direction explosion phase transition), Q7.24 (analytical proof of +2 per writhe unit), Q7.29 (maximum-depth null generalization -- D87 provides zeta_12 data), Q8.1 (resource decomposition proof -- REFINED by D89-D91), Q6.18 (encoding design for targeted functions), Q6.19 (+/-q optimality for parity), Q6.20 (phase-sensitive activation), Q7.33 (zeta_12 load-bearing directions), Q7.35 (constrained optimization as SDP), Q7.37 (depth law for non-parity), Q7.40 (MAJ class conflict significance), Q2.11 (literature method for b), Q2.13 (TL_8 semisimplicity), Q9.2 (hybrid encoding design), Q10.1 (b and DKC capacity), Q10.3 (P_{0,0} exclusive eigenvalues), Q11.3 (MAJ cliff mechanism), Q11.5 (N=6-7 crossover mechanism 2I vs z12), Q11.6 (circuit complexity hierarchy universality), Q11.7 (XOR growth curve shape), Q12.2 (hub count scaling -- UPDATED by D102-D103), Q12.3 (Casimir inversion universality -- UPDATED by D102), Q12.7 (two-regime universality), Q12.8 (commuting hubs structure), Q12.9 (broad vs deep capacity -- UPDATED by D102), Q12.10 (cross-layer synergy universality -- UPDATED by D102), Q13.1 (Fibonacci constraint from radical -- NEW from D102), Q13.3 (optimal activation component count -- NEW from D102/D104), Q14.1 (B@sub7 beats A@full -- NEW from D103), Q14.2 (universal BFS tree conjecture -- NEW from D102-D103), Q14.4 (sign-rank expansion mechanism -- NEW from D104-D105), Q14.5 (macrame principle crossover -- NEW from D104-D105), Q14.7 (general cross-block recipe -- NEW from D104-D105), Q15.1 (per-entry XOR predictor -- NEW from D106), Q16.3 (major index for all W_{n,j} -- NEW from D107), Q16.4 (2-adic neutrality proof -- NEW from D107), Q16.6 (q-Catalan at other roots -- NEW from D107), Q17.1 (dual-channel for XOR6 at k>=24 -- NEW from D108), Q17.3 (parity-lock as dual-channel -- NEW from D108), Q17.4 (product closure threshold -- NEW from D108), Q17.5 (non-monotonic prediction -- NEW from D108), Q17.6 (overlap mechanism k=6 vs k=15 -- NEW from D108), Q17.9 (angular discriminator -- NEW from D109), Q17.12 (product-closure threshold quantitative -- NEW from D108-D109), Q17.14 (j=0 liveness structural universality -- NEW from D109)

**Tier 3 -- Deep work (HARD, transformative):**
Q1.5 (incommensurability theorem), Q3.1 (two-channel coding -- RELATED to D108 dual-channel), Q5.3 (TQC/DKC independence), Q2.2 (Galois symmetry), Q1.11 (n=6 wall proof), Q6.11 (13=13 theorem analytical proof), Q6.15 (Bloch sphere / quantum info connection), Q7.5 (minimum state count for 100% automaton determinism), Q7.11 (activation determines capacity -- formalization, PARTIALLY ADDRESSED by D83/D91), Q7.17 (E_6 and E_8 reachability), Q7.20 (direct generation of deep entries), Q7.27 (LCFT Jordan-cell structure explicit -- D85 provides infrastructure), Q7.30 (null entries and knot complexity), Q7.31 (RC exactification via null-state geometry -- D87 refines scope), Q8.3 (TQFT anomaly as computational resource), Q2.10 (valuation condition at larger lattices), Q2.12 (single P_{0,0} divergence reason), Q7.36 (depth law slope = 1 analytical), Q7.38 (depth law group dependence), Q10.4 (dense polymer fusion for DKC), Q12.4 (optimal catalog from first principles)

---

*Source: demo-index.md (109 demos incl. D63-D109, D39 Parts G-J), four-lenses.md, novelty.md, connections.md.*
