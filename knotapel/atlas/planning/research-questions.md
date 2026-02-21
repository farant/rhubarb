# DKC Research Questions

Collected from 82 demos (incl. D63-D71, D72-D82, D39 Parts G-J), four-lenses.md, novelty.md, connections.md. Updated 2026-02-21.

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

**Q1.14. Funnel rate formula.** Upward extensibility collapses: XOR3→XOR2 99.3%, XOR4→XOR3 39.2%, XOR5→XOR4 19.2%, XOR6→XOR5 0%. Is there a closed-form formula for the coverage fraction as a function of n (exponential decay, algebraic, other)?
- Source: D64 | Difficulty: MODERATE | Publication: structural property of the parity matroid chain

**Q1.15. k=38 anomaly.** k=38 is the only even k≥24 with zero generalized XOR6 solutions (among k=24,26,...,64 tested). 38=2×19. Is 19 special with respect to the Z[zeta_8] lattice, or the pi/4 alignment constraint?
- Source: D65 | Difficulty: MODERATE | Publication: odd gap in the even-k landscape

**Q1.16. Odd k for generalized XOR6.** Near the threshold (k=24..32), generalized XOR6 solutions exist only at even k. Does any odd k≥25 produce solutions? Is the even/odd pattern provable from pi/4 lattice alignment?
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

**Q2.7. rad^2 generator vs neglecton objects.** D49 neglecton weights from radical; D51 characterizes rad^2. Same objects?
- Source: D49, D52 | Difficulty: MODERATE | Publication: unifies DKC arc with radical anatomy arc

**Q2.8. Markov RT truncation: cross-sector kernel structure.** At ell=4, coefficient matrix is J-deltaI (rank 2). Does this pattern generalize to other ell? Does the kernel vector always span exactly two sectors?
- Source: D39 Part J | Difficulty: MODERATE | Publication: refines Markov RT truncation theorem

**Q2.9. Second-degeneracy corank proof.** Corank = ell+1 at n=ell+2, verified ell=3..6. Provable from linking theory?
- Source: D39 Part H | Difficulty: MODERATE | Publication: extends radical structure formulas

---

## 3. Coding Theory

**Q3.1. Two-channel coding conjecture.** Magnitude (~4.3 bits) + phase (~1.5 bits). Joint decoder achieves ~5.8 bits/symbol?
- Source: D46-47, four-lenses | Difficulty: HARD | Publication: standalone info-theory contribution

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

**Q4.3. Is axiality at delta=0 known?** 131K braids verified. Must check if trivially follows from cyclotomic theory.
- Source: D35 | Difficulty: APPROACHABLE (expert consultation) | Publication: determines Paper 4's significance

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

**Q6.7. Solution count monotonicity at k=15.** Solutions decrease 23004→16108→3020→0 as n increases from 3 to 6. Is monotonic decrease a theorem, or could counts increase for larger n in other lattices?
- Source: D63 | Difficulty: MODERATE | Publication: structural property of DKC

**Q6.8. Clifford hierarchy universality.** ~~Does the parity ceiling n=5 climb the Clifford hierarchy one rung per additional input?~~ **ANSWERED/REFUTED by D65**: ceiling is algebra-independent — it does NOT climb. n=5 is unchanged at Z[zeta_16]. If confirmed at Z[zeta_32] and beyond, the proof is purely topological.
- Source: D64, D65 | Difficulty: N/A | Publication: answered; proof target shifts to topology

**Q6.9. F4 orbit count formula.** The 35 Voronoi XOR6 solutions on the 24-cell decompose under F4 symmetry into exactly 6 orbits (12+7+5+5+4+2). The conjecture is that XOR_n on the 24-cell always yields n*(n-1)/2 orbits (6=5*4/2... wait, 6=C(4,2)). Connection to the partition structure of 6 (the XOR arity)?
- Source: D66 | Difficulty: MODERATE | Publication: symmetry classification of DKC solution space

**Q6.10. Hopf phase computational inertness.** ~~The Hopf fiber phase carries zero DKC computational information at every resolution tested (k=4..32). Why?~~ **ADDRESSED by D75**: angle subsumes cell for sums — angle channel perfectly predicts parity for all 36 winning triples, and the cell is a coarsening of the angle channel. The S² Voronoi cell carries no information beyond what the angle (S¹ component) encodes at the computation level. Full proof from antipodal symmetry / even spherical harmonics argument still needed.
- Source: D67, D75 | Difficulty: MODERATE | Publication: theoretical heart of the S² arc

**Q6.11. 13=13 theorem analytical proof.** Computationally: minimum bandwidth for XOR6 on S² is l=6 because 2×6+1=13=number of eigenvector directions (sharp phase transition: 0% recovery at l≤5, 100% at l=6). Can this be proven analytically? Specifically: for N points on S² in general position, is the minimum bandwidth of any separating binary Voronoi partition l=⌈(N-1)/2⌉?
- Source: D71 | Difficulty: HARD | Publication: transforms a computational observation into a theorem about binary Voronoi on S²

**Q6.12. Trivialization threshold.** XOR6 transitions from topology-driven (~36 solutions at 14 cells) to pigeonhole-dominated (~90% at 3,458 cells for ζ_16). The exact threshold lies between ~50 and ~500 Voronoi cells but has not been pinned down. D72 provides an additional data point: t=0 anti-correlation shows the algebraically-selected 13-cell configuration is locally maximal for XOR6 (36 solutions vs 4 for a geometrically optimized design). Is the transition sharp (like the l=6 bandwidth transition) or gradual?
- Source: D69, D71, D72 | Difficulty: APPROACHABLE | Publication: practical guidance for DKC activation design

**Q6.13. Gradient descent and knot-derived weights.** Demo 68 constructs two-neuron perceptron weight matrices solving XOR6 from stereographic projections of DKC solutions — no training, from knot theory. Can gradient descent from random initialization rediscover these weights? Does it converge to the same S² structure implicitly?
- Source: D68 | Difficulty: APPROACHABLE | Publication: bridges DKC to practical neural network training

**Q6.14. XOR7 bandwidth on S².** Demo 65 showed XOR7 requires k=91 sectors on S¹. Demo 67 showed XOR6 drops from 24 sectors (S¹) to 14 cells (S²). What is the S² bandwidth (and cell count) for XOR7? Does the DOF formula predict it correctly? *Note: D73 (eigenvector automaton) did not address this question.*
- Source: D65, D67, D71 | Difficulty: APPROACHABLE | Publication: tests generality of the 13=13 theorem

**Q6.15. Bloch sphere / quantum information connection.** DKC computation lives on S²=CP¹ (the Bloch sphere of a qubit). The minimum bandwidth l=6 corresponds to hexadecapole operators in quantum optics. Is there a direct quantum-information interpretation of the XOR6 bandwidth result?
- Source: D67, D71 | Difficulty: OPEN PROBLEM | Publication: connects DKC to quantum information theory

---

## 7. Scaling and Capacity

**Q7.1. Anti-correlation mechanism.** D72 establishes that the worst spherical t-design (t=0) produces the most XOR6 solutions (36 vs 4 for a geometrically better design). The mechanism is "algebraic not geometric" — directions are selected by the binary octahedral group acting on Z[ζ_8] quaternions. Can this be proven as a theorem: that any t≥1 spherical design on S² yields strictly fewer XOR6 solutions than the algebraically-selected 13-direction configuration?
- Source: D72 | Difficulty: MODERATE | Publication: formal version of the anti-correlation theorem

**Q7.2. Hidden quadrupole symmetry at l=2.** The 13 eigenvector directions achieve t=0 overall but l=2 passes to machine precision (residual 5.11e-16). This suggests a hidden quadrupole symmetry in the binary octahedral group action. What algebraic identity ensures Σ Y_{2,m}(dir_i)=0 over the 13 directions?
- Source: D72 | Difficulty: MODERATE | Publication: explains the t=2 even-mode property algebraically

**Q7.3. Perturbation non-monotonicity and local maximum.** At 1° perturbation the mean XOR6 count rises to 38 (above the unperturbed 36), indicating the 13-direction algebraic configuration is a local but not global maximum. What is the maximum XOR6 solution count achievable for any 13-point configuration on S²? Is 36 a local or global maximum?
- Source: D72 | Difficulty: APPROACHABLE | Publication: characterizes the optimality landscape

**Q7.4. Universal automaton determinism.** D73 shows 82.8% (48/58) determinism is a structural constant of the ζ_8 DKC system — identical across all 36 winning triples — following from the binary octahedral group acting transitively on the 13 directions. Can the 82.8% fraction be improved by choosing a different entry rule (next state function) while keeping the 13-cell partition? Is there a threshold automaton step count where determinism degrades?
- Source: D73 | Difficulty: MODERATE | Publication: characterizes the automaton optimality boundary

**Q7.5. Minimum state count for 100% determinism.** D73 shows 27 cells (signed partition, no antipodal identification) achieve 87.1% determinism, still not 100%. The remaining non-determinism comes from partial sums with equal eigenvector direction but different magnitudes. Is there a 27-state (or finite-state) automaton that achieves 100% determinism for XOR6 at ζ_8? Or does 100% require tracking magnitude information and thus infinitely many states?
- Source: D73 | Difficulty: HARD | Publication: resolves minimum-state question for DKC automaton

**Q7.6. The 6 unseparated pairs.** At the qcat level, 6 braid pairs have the same Voronoi cell AND the same Kauffman bracket value. These correspond to body-type cells (2–5) with 60° angles. Do these pairs represent genuinely equivalent braids under some stronger invariant, or is there a finer invariant that separates all 24 quaternion catalog entries?
- Source: D74 | Difficulty: MODERATE | Publication: closes the incomparability theorem

**Q7.7. Is 119 near-optimal?** With 24 quaternions and 13 cells, the combined (bracket, cell) invariant yields 119 equivalence classes. Is this close to the theoretical maximum achievable with the length-1-to-8 braid catalog? Does the combined class count grow without bound as longer braid words are included?
- Source: D74 | Difficulty: APPROACHABLE | Publication: sharpens the combined invariant result

**Q7.8. [*,18,22] dead zone.** Winning triples containing catalog entries q18 and q22 (edge-type) separate zero bracket collisions — they are the 3 exceptions among 36 winners for which the cell labeling exploits no bracket collisions. Is this a geometric accident (those edge cells happen to share labels) or is there a group-theoretic reason rooted in the orbit structure of edge-type cells?
- Source: D74 | Difficulty: MODERATE | Publication: completes the computational-type classification

**Q7.9. sqrt(n) magnitude growth continuation.** D75 shows the additive sum magnitude |S| takes exactly 6 values — {0, 1, √2, √3, 2, √6} — reflecting the inner product structure of the 24-cell (pairwise inner products from {-1, -½, 0, ½, 1}). Does this discrete quantization of magnitude values continue for 4-term sums (XOR8 at ζ_8) or does it collapse to near-continuous values as observed in D76? Is the discreteness specific to 3-term sums, or is there a combinatorial argument for why √5 is unreachable regardless of sum length?
- Source: D75 | Difficulty: MODERATE | Publication: characterizes the magnitude channel algebraically

**Q7.10. Paired-quaternion channel necessity.** D77 shows every XOR8 winning quadruple contains exactly one pair of quaternions sharing the same eigenvector direction but differing in eigenvalue angle, and removing either pair member leaves an XOR6-winning triple. Is the paired-quaternion structure necessary for XOR8 (i.e., does every XOR8 solution decompose this way), or is it a feature specific to the 6 solutions found? Does the recursive structure extend: XOR10 winners = XOR8 winners plus another paired partner?
- Source: D75, D77, D78 | Difficulty: MODERATE | Publication: proves the recursive shadow conjecture at the next level

**Q7.11. Activation determines capacity — formalization.** D77 demonstrates that the same ζ_8 root and same 24-element catalog yield 0 XOR8 solutions under S²-only activation and 6 solutions under S¹×S² activation. Can "Activation Determines Capacity" be formalized as a theorem? Specifically: for a fixed algebraic catalog, is there a lattice ordering on activation functions under which higher activations always achieve at least as many XOR solutions, with the S¹×S² product sitting above pure S¹ and pure S² in that ordering?
- Source: D77 | Difficulty: HARD | Publication: theoretical foundation for activation design

**Q7.12. Critical cell count universality.** D77 observes a phase transition between 84 cells (0 XOR8 winners) and 112 cells (6 winners), conjecturing a critical threshold near 100 cells related to masks-per-cell dropping below ~2.56. Is the critical cell count ~100 a universal threshold for 8-input DKC regardless of root of unity, or does it scale with catalog and activation structure? Does the masks-per-cell ≈ 4.6 sweet spot observed for XOR6 at ζ_8 hold across other input sizes and roots of unity?
- Source: D76, D77 | Difficulty: MODERATE | Publication: practical design rule for DKC activation

**Q7.13. ζ_8 XOR ceiling — wall characterization.** D78 confirms XOR10 = 0 at ζ_8 across all 42,504 quintuples and all tested sector counts. The diagnosis is catalog exhaustion: 24 quaternions with only 13 directions and 2 half-angles cannot produce 1024-mask separation. Is there a clean information-theoretic lower bound formula: max_n = f(|catalog|, n_directions, n_angles)? Is the wall at XOR8 specific to the binary octahedral group, or is it a general consequence of any ADE-type finite group?
- Source: D78 | Difficulty: MODERATE | Publication: closes the ζ_8 capacity story

**Q7.14. Infinite group XOR capacity.** D79 shows ζ_12 (infinite group) breaks the ζ_8 wall entirely: XOR10 goes from 0 to 124 winners, XOR12 yields 50+ winners. Do ALL infinite groups under the two-perpendicular-axes construction provide unbounded XOR capacity? Or can some infinite groups have structural limitations imposing their own ceilings?
- Source: D79, D80 | Difficulty: MODERATE | Publication: characterizes the finite/infinite capacity dichotomy

**Q7.15. Computationally optimal infinite group.** Among the infinite-group roots surveyed (ζ_6, ζ_10, ζ_12, ζ_16, ζ_20), which is computationally optimal — fewest catalog entries needed to reach XOR(n) for given n? D80 notes ζ_10 has the most distinct half-angles at 4096 entries (71 vs ζ_12's 43), suggesting richer angular diversity. Does more angular diversity per entry directly translate to lower minimum catalog size for each XOR level?
- Source: D79, D80 | Difficulty: APPROACHABLE | Publication: practical guidance for choosing the base root of unity

**Q7.16. Power-of-two finiteness conjecture.** D80 proposes that the finite groups under the two-perpendicular-axes construction are exactly those with rotation angle π/2^k for integer k≥1. This predicts ζ_32 (θ=π/16) should be finite. Is ζ_32 finite? Why does the geometric characterization (cube/octahedron symmetries at π/2 and π/4) not extend to π/3, given that cos(π/3)=1/2 is rational — what algebraic property is actually determining finiteness?
- Source: D80 | Difficulty: MODERATE | Publication: closes the ADE finiteness classification

**Q7.17. E_6 and E_8 reachability.** The two-perpendicular-axes construction reaches E_7 (binary octahedral, ζ_8) and the D-series (binary dihedral, ζ_4) but not E_6 (binary tetrahedral, order 24) or E_8 (binary icosahedral, order 120). Is there a different natural generator construction that reaches E_6 or E_8? What XOR capacity would E_8 (the richest finite SU(2) subgroup) provide?
- Source: D80 | Difficulty: HARD | Publication: completes the ADE DKC survey

**Q7.18. Scaling constant dependence on root.** D81 establishes max_xor ≈ 0.62 × log₂(catalog_size) + 4.6 for ζ_12. Does the 0.62 constant change with different infinite-group roots of unity? Is it a function of angular density per closure round, of the quaternion dimension (4D), or of the specific Sec×Vor activation geometry? Could a union catalog mixing ζ_10 + ζ_12 entries improve the constant or change the logarithmic character?
- Source: D81 | Difficulty: MODERATE | Publication: generalizes the scaling law

**Q7.19. Logarithmic vs sub-logarithmic scaling.** D81 observes decreasing gaps between XOR transitions (3.35 → 2.43 → 2.05 doublings for XOR6→8→10→12), suggesting scaling may be O(log N / log log N) rather than pure O(log N). Does the XOR14 transition (predicted at ~38K entries) follow the pattern? Is there a fundamental ceiling where logarithmic scaling saturates?
- Source: D81 | Difficulty: APPROACHABLE | Publication: resolves the scaling law character

**Q7.20. Direct generation of deep entries.** D82 shows pure depth-8 entries are 2× more efficient than shallow or strided entries at matched catalog size — 564 deep entries reach XOR12 while 1140 mixed entries first reach XOR12 at round 6. But deep entries are currently obtained only by running the full BFS closure. Can depth-d quaternions be characterized algebraically and generated directly without computing all shallower depths? What algebraic property does strided-564 lack that deep-564 possesses (presumably shared common-factor structure from the many-generator products)?
- Source: D82 | Difficulty: HARD | Publication: practical shortcut for DKC catalog construction

**Q7.21. Depth law universality.** D82 demonstrates max_xor ≈ depth + 6 for ζ_12. Does the linear depth law hold for other infinite-group roots (ζ_10, ζ_16)? Does the constant ("+6") change with root, or is it a universal offset related to the XOR6 baseline? Does it hold for finite groups with ζ_8 as control (confirmed: ζ_8 closes at depth 3, max XOR=8, consistent with depth+5)?
- Source: D82 | Difficulty: MODERATE | Publication: establishes depth law as a general principle

**Q7.22. Direction explosion / angle refinement phase transition.** D82 observes a clean regime boundary at depth 6–7 for ζ_12: below depth 6, new directions dominate the catalog growth (direction explosion); above depth 6, directions saturate at MAX_DIR=512 and angle refinement dominates. Is this phase transition present for all infinite-group roots? Is the transition depth related to the point at which the group's direction catalog achieves dense S² coverage?
- Source: D82 | Difficulty: MODERATE | Publication: structural insight into group closure dynamics

---

## Priority Ranking

**Tier 1 -- Immediate (APPROACHABLE, high impact):**
Q1.10 (universal k proof), Q1.4 (k=9..16 counts), Q6.2 (duality at 4 inputs), Q4.3 (axiality known?), Q1.7 (multi-sector multiplicative), Q1.16 (odd k generalized XOR6), Q6.12 (trivialization threshold — D72 provides new data point), Q6.13 (gradient descent to knot weights), Q6.14 (XOR7 bandwidth on S²), Q7.3 (perturbation non-monotonicity), Q7.7 (119 near-optimal?), Q7.15 (optimal infinite group), Q7.16 (ζ_32 power-of-two conjecture), Q7.19 (logarithmic vs sub-logarithmic — XOR14 test)

**Tier 2 -- Soon (MODERATE, strengthens papers):**
Q2.1 (sandwich proof), Q3.4 (catalog completeness), Q1.9 (OM generalization), Q2.8 (cross-sector kernel), Q6.6 (non-parity ceiling), Q1.13 (n=6 higher-order constraints), Q1.14 (funnel rate formula), Q1.15 (k=38 anomaly), Q6.9 (F4 orbit count formula), Q6.10 (Hopf phase inertness — partially addressed by D75, proof still needed), Q7.1 (anti-correlation mechanism), Q7.2 (hidden quadrupole symmetry), Q7.4 (universal automaton determinism), Q7.6 (6 unseparated pairs), Q7.8 ([*,18,22] dead zone), Q7.9 (sqrt(n) magnitude continuation), Q7.10 (paired-quaternion channel necessity), Q7.12 (critical cell count universality), Q7.13 (ζ_8 XOR ceiling formula), Q7.14 (infinite group capacity), Q7.18 (scaling constant vs root), Q7.21 (depth law universality), Q7.22 (direction explosion phase transition)

**Tier 3 -- Deep work (HARD, transformative):**
Q1.5 (incommensurability theorem), Q3.1 (two-channel coding), Q5.3 (TQC/DKC independence), Q2.2 (Galois symmetry), Q1.11 (n=6 wall proof), Q6.11 (13=13 theorem analytical proof), Q6.15 (Bloch sphere / quantum info connection), Q7.5 (minimum state count for 100% automaton determinism), Q7.11 (activation determines capacity — formalization), Q7.17 (E_6 and E_8 reachability), Q7.20 (direct generation of deep entries)

---

*Source: demo-index.md (82 demos incl. D63-D82, D39 Parts G-J), four-lenses.md, novelty.md, connections.md.*
