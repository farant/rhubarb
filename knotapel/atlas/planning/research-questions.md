# DKC Research Questions

Collected from 51 demos (incl. D63, D39 Parts G-J), four-lenses.md, novelty.md, connections.md. Generated 2026-02-20.

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

**Q1.12. Z[zeta_16] parity ceiling.** At 16 directions (M=16), pigeonhole ceiling rises to n=15. What is the actual constraint ceiling? Is the gap always 2?
- Source: D63 | Difficulty: LARGE | Publication: first test of scaling beyond Z[zeta_8]

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

**Q6.1. 4-input DKC.** ~~Does parity need k=8?~~ **DONE** (Demo 63, 96 solutions at k=8).
- Source: D50, D63 | Difficulty: N/A | Publication: confirmed

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

---

## Priority Ranking

**Tier 1 -- Immediate (APPROACHABLE, high impact):**
Q1.10 (universal k proof), Q1.4 (k=9..16 counts), Q6.2 (duality at 4 inputs), Q4.3 (axiality known?), Q1.7 (multi-sector multiplicative)

**Tier 2 -- Soon (MODERATE, strengthens papers):**
Q2.1 (sandwich proof), Q3.4 (catalog completeness), Q1.9 (OM generalization), Q2.8 (cross-sector kernel), Q6.6 (non-parity ceiling)

**Tier 3 -- Deep work (HARD, transformative):**
Q1.5 (incommensurability theorem), Q3.1 (two-channel coding), Q5.3 (TQC/DKC independence), Q2.2 (Galois symmetry), Q1.11 (n=6 wall proof), Q1.12 (Z[zeta_16] scaling)

---

*Source: demo-index.md (51 demos incl. D63, D39 Parts G-J), four-lenses.md, novelty.md, connections.md.*
