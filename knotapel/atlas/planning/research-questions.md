# DKC Research Questions

Collected from 71 demos (incl. D63-D71, D39 Parts G-J), four-lenses.md, novelty.md, connections.md. Updated 2026-02-21.

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

**Q6.10. Hopf phase computational inertness.** The Hopf fiber phase carries zero DKC computational information at every resolution tested (k=4..32). Why? The S² Hopf base carries all the information. Is this provable from the antipodal symmetry of the eigenvector catalog (undirected directions → only even spherical harmonics → fiber phase integrates to zero)?
- Source: D67 | Difficulty: MODERATE | Publication: theoretical heart of the S² arc

**Q6.11. 13=13 theorem analytical proof.** Computationally: minimum bandwidth for XOR6 on S² is l=6 because 2×6+1=13=number of eigenvector directions (sharp phase transition: 0% recovery at l≤5, 100% at l=6). Can this be proven analytically? Specifically: for N points on S² in general position, is the minimum bandwidth of any separating binary Voronoi partition l=⌈(N-1)/2⌉?
- Source: D71 | Difficulty: HARD | Publication: transforms a computational observation into a theorem about binary Voronoi on S²

**Q6.12. Trivialization threshold.** XOR6 transitions from topology-driven (~36 solutions at 14 cells) to pigeonhole-dominated (~90% at 3,458 cells for ζ_16). The exact threshold lies between ~50 and ~500 Voronoi cells but has not been pinned down. Is the transition sharp (like the l=6 bandwidth transition) or gradual?
- Source: D69, D71 | Difficulty: APPROACHABLE | Publication: practical guidance for DKC activation design

**Q6.13. Gradient descent and knot-derived weights.** Demo 68 constructs two-neuron perceptron weight matrices solving XOR6 from stereographic projections of DKC solutions — no training, from knot theory. Can gradient descent from random initialization rediscover these weights? Does it converge to the same S² structure implicitly?
- Source: D68 | Difficulty: APPROACHABLE | Publication: bridges DKC to practical neural network training

**Q6.14. XOR7 bandwidth on S².** Demo 65 showed XOR7 requires k=91 sectors on S¹. Demo 67 showed XOR6 drops from 24 sectors (S¹) to 14 cells (S²). What is the S² bandwidth (and cell count) for XOR7? Does the DOF formula predict it correctly?
- Source: D65, D67, D71 | Difficulty: APPROACHABLE | Publication: tests generality of the 13=13 theorem

**Q6.15. Bloch sphere / quantum information connection.** DKC computation lives on S²=CP¹ (the Bloch sphere of a qubit). The minimum bandwidth l=6 corresponds to hexadecapole operators in quantum optics. Is there a direct quantum-information interpretation of the XOR6 bandwidth result?
- Source: D67, D71 | Difficulty: OPEN PROBLEM | Publication: connects DKC to quantum information theory

---

## Priority Ranking

**Tier 1 -- Immediate (APPROACHABLE, high impact):**
Q1.10 (universal k proof), Q1.4 (k=9..16 counts), Q6.2 (duality at 4 inputs), Q4.3 (axiality known?), Q1.7 (multi-sector multiplicative), Q1.16 (odd k generalized XOR6), Q6.12 (trivialization threshold), Q6.13 (gradient descent to knot weights), Q6.14 (XOR7 bandwidth on S²)

**Tier 2 -- Soon (MODERATE, strengthens papers):**
Q2.1 (sandwich proof), Q3.4 (catalog completeness), Q1.9 (OM generalization), Q2.8 (cross-sector kernel), Q6.6 (non-parity ceiling), Q1.13 (n=6 higher-order constraints), Q1.14 (funnel rate formula), Q1.15 (k=38 anomaly), Q6.9 (F4 orbit count formula), Q6.10 (Hopf phase inertness proof)

**Tier 3 -- Deep work (HARD, transformative):**
Q1.5 (incommensurability theorem), Q3.1 (two-channel coding), Q5.3 (TQC/DKC independence), Q2.2 (Galois symmetry), Q1.11 (n=6 wall proof), Q6.11 (13=13 theorem analytical proof), Q6.15 (Bloch sphere / quantum info connection)

---

*Source: demo-index.md (71 demos incl. D63-D71, D39 Parts G-J), four-lenses.md, novelty.md, connections.md.*
