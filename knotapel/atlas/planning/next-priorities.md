# DKC Research: Next Priorities

Generated 2026-02-20. Updated 2026-02-21 after D72-D84. Informed by synthesis documents (narrative, novelty, connections, four-lenses). D64–D71 added: matroid recursion, Clifford staircase (Z[zeta_16]), 24-cell / quaternionic arc (D66–D71), spectral DKC. D72–D82 added: spherical design, eigenvector automaton, computational invariant, binocular DKC, 8-input scaling, activation zoo, recursive scaling, ζ₁₂ capacity, group finiteness survey, capacity scaling law, crossing depth. D83–D84 added: framing as computational content (+2 writhe loss), null states and logarithmic structure (null indispensability, LCFT Jordan-cell interpretation).

---

## Tier 1: Do Next

### ~~1.1 Formal proof of the sandwich theorem~~ — PARTIALLY DONE
The **radical dimension formula** (rad(TL_ℓ) = 2ℓ−3) is now formally proven in `proofs/radical-dimension-formula.md`. Method: Graham-Lehrer cellular theory, unique degenerate cell module V_{ℓ-2} with corank 1, Chebyshev determinant. Corollary: ℓ²−ℓ−3 at n=ℓ+1.
**Additionally proven:** Markov RT truncation (`proofs/markov-rt-truncation.md`) and next-level radical formula (`proofs/next-level-radical-formula.md`). These strengthen Paper 3.
**Remaining:** The full sandwich theorem (rad^2(TL_{2k}(0)) ≅ TL_{2k-1}(0) as algebras) is a stronger claim than the dimension formula. The proof proves dimensions match; the algebra isomorphism still needs formal proof. Also: nilpotency=3 and Fibonacci rank remain computationally verified, not proven.

### ~~1.2 4-input parity: test the triskelion generalization~~ — DONE (Demo 63)
4-input parity reachable at k=8 (96 solutions, tetraskelion). 5-input parity at k=15 (3020 solutions, pentaskelion). **Parity ceiling for Z[zeta_8] is n=5.** Triskelion generalization (k=2n) **FALSIFIED** — n=5 requires k=15, not k=10. True scaling: k=2M-1 where M is ray count. Oriented matroid classification: alternating OM type required for parity. Two types of impossibility: constraint wall (n=6,7) vs pigeonhole (n>=8).

### 1.3 Analytical proof of the 13=13 bandwidth bound — **NEW**
**What:** Prove analytically that the minimum bandwidth for a binary Voronoi partition separating N points on S² in general position is l = ⌈(N−1)/2⌉. For XOR6 DKC, N=13 (eigenvector directions) giving l=6. D71 verified this computationally with a sharp phase transition (0% recovery at l≤5, 100% at l=6) and 80.7% average power in mode l=6 across all 36 solutions.
**Why:** The 13=13 theorem is the cleanest standalone publication target in the arc — self-contained, striking, and proven at the computational level already. An analytical proof would make it a rigorous theorem rather than a verified result, and would immediately imply the trivialization prediction for finer roots (ζ_16 → l≥1728) analytically.
**Effort:** SMALL to MEDIUM (key insight is degrees-of-freedom counting; the hard part is formalizing "general position" and proving no sub-Nyquist trick works at l=5).
**Unblocks:** Strengthens Paper 5 (24-Cell, S², Spectral Geometry). Turns D71's computational phase transition into a publishable theorem.
**D72 context:** D72's anti-correlation result (better t-design geometry → fewer XOR6 solutions) directly supports this theorem: the algebraic placement of the 13 directions is precisely what creates the l=6 bandwidth need. The DOF-not-Design mechanism (t-design property is irrelevant; the 2l+1=13=directions DOF coincidence is what matters) is a key component of the analytical proof argument.

### 1.4-NEW XOR7 on S² — context updated by D78/D80
**What:** Test the spectral framework scaling on XOR7. D65 showed XOR7 requires k=127 sectors on S¹. D67 showed XOR6 drops from k=24 (S¹) to 14 cells (S²). If the DOF formula generalizes, XOR7 on S² should need l=⌈(N_7−1)/2⌉ where N_7 is the number of XOR7-relevant eigenvector directions. Measure: does S² reduce XOR7 from k=127 as dramatically as it did XOR6?
**Why:** Directly tests whether the spectral framework is a systematic tool for climbing the parity hierarchy, or whether XOR6 on S² is a special case. A positive result would establish S² spectral DKC as the natural framework for the entire parity arc.
**Context update (D78/D80):** At ζ₈, D78 confirmed XOR10 = 0 (ζ₈ wall is hard). D80 established that ζ₈ generates a finite group (24 elements) and this is the fundamental ceiling. XOR7 on S² is therefore a question about what can be achieved within the finite-group regime — the S² spectral framework reduces the cell count but cannot overcome the group-finiteness ceiling. This narrows the question: does the S² DOF argument give an intermediate XOR level (7, 8) that Voronoi-only misses, or does it find a different path to XOR8 than the Sec×Vor activation of D77?
**Effort:** MEDIUM (build on D67/D69 Voronoi machinery; requires expanding catalog to XOR7-weight 7-tuples).
**Unblocks:** Determines whether Paper 5 covers a theorem or a single data point. Feeds into the scaling law conjecture.

### 1.5 Catalog completeness argument
**What:** Prove or bound that the 100-value Z[zeta_8] catalog at delta=0 contains all distinct bracket values up to braid length L. Alternatively, prove asymptotic saturation (new values per additional crossing approaches zero).
**Why:** Every forward DKC exhaustive search ("0 out of 100M") depends on the catalog being complete enough. Without a completeness argument, a reviewer can ask "what if you missed a value?" This is the single most important gap for Paper 1.
**Effort:** SMALL (1 demo — extend enumeration to length 10-12, track saturation curve, attempt an algebraic bound using axiality + single-loop selection).
**Unblocks:** Strengthens Paper 1 from "exhaustive over our catalog" to "exhaustive over all bracket values up to length L."

### 1.6 Convexity thesis: complete the formal proof
**What:** Formalize the argument that the 11/13 wall is a consequence of activation convexity (not just encoding geometry). D57 and D62 have the ingredients; what's missing is a single clean theorem statement with a self-contained proof.
**Why:** The 11/13 theorem (Paper 2) has clean proofs for the two unreachable classes, but the UNIFYING explanation (convexity) is informal. A formal convexity theorem would elevate Paper 2 from "two separate impossibility results" to "one structural theorem with two corollaries."
**Effort:** SMALL (1 demo — mostly writing, possibly one verification).
**Unblocks:** Paper 2 becomes a stronger, more unified submission.

### 1.7 ζ₁₂ Sec×Vor activation for XOR10+ — **NEW (HIGH PRIORITY)**
**What:** D79 demonstrated that ζ₁₂ breaks the ζ₈ wall for XOR10 (124 winners) and XOR12 (50+ winners) using Sec×Vor activation with the default sector count. D77 showed that Sec(8)×Vor (112 cells) is far more powerful than Voronoi alone (14 cells) at ζ₈ — XOR8 goes from 0 to 6 solutions. The combination has not yet been tested at ζ₁₂: does Sec(k)×Vor at ζ₁₂ dramatically outperform Voronoi-only ζ₁₂, just as it did at ζ₈? What is the optimal k? What is the XOR capacity ceiling at ζ₁₂ with product activation?
**Why:** This is the direct next step in the scaling arc. D79 used k=8 and found 124 XOR10 winners; optimizing k and using the full product activation should substantially increase this count and potentially reveal XOR14+ at manageable catalog sizes. HIGH priority: immediate publication impact for Paper 6.
**Effort:** SMALL to MEDIUM (Sec×Vor infrastructure exists from D77; port to ζ₁₂ catalog, sweep k, measure capacity per round).
**Unblocks:** Paper 6 capacity chapter. Confirms whether the logarithmic scaling law (D81) holds under optimized product activation or shifts. Direct comparison to D81's k=8 baseline reveals how much capacity is left on the table.

### 1.8 ζ₃₂ finiteness test — **NEW**
**What:** D80 established that ζ₄ and ζ₈ are the only finite groups among the roots tested (N=4,6,8,10,12,16,20). D80 proposed the Power-of-Two Finiteness Conjecture: finite groups occur exactly at angles of the form π/2^k (k ≥ 1), predicting ζ₃₂ (θ = π/16) should also be finite. A single test_root(32) call with the existing D80 infrastructure confirms or refutes this conjecture.
**Why:** If ζ₃₂ is finite, the power-of-two pattern holds and the ADE/finiteness story gains a third data point, making it publishable on its own. If ζ₃₂ is infinite, the pattern breaks and the real criterion must be found — equally important. Either outcome directly impacts the Paper 6 framing.
**Effort:** SMALL (single additional test in D80's framework; ~1 hour of work).
**Unblocks:** Confirms or refutes Power-of-Two Conjecture. Completes the ADE/finiteness survey for Paper 6. If finite: adds to the story that only lattice roots (quantum dimension vanishes) generate finite groups.

### 1.9 Framing test at ζ₁₆/ζ₂₄ — **NEW (D83)**
**What:** D83 demonstrated that Jones normalization destroys exactly 2 XOR levels at both ζ₈ (bracket XOR8 → Jones XOR6) and ζ₁₂ (bracket XOR12 → Jones XOR10). The +2 loss is exact and constant across these two roots despite very different group sizes (24 vs. 4096 entries). Testing at ζ₁₆ and ζ₂₄ would confirm whether the loss is truly universal across all roots of unity.
**Why:** The "+2 per writhe unit" is the most elegant single result in the framing arc. Two data points (ζ₈, ζ₁₂) strongly suggest universality, but a reviewer can always ask for more. Two additional data points at very different roots (ζ₁₆: infinite group, finer lattice; ζ₂₄: even finer) would close this concern. If the loss is NOT +2 at some root, that is equally important — it means framing cost depends on lattice structure, which changes the resource decomposition formula.
**Effort:** SMALL (D83 infrastructure auto-adapts via `g_half_angle`; `jones_normalize_catalog()` already works for any root; one demo, two test runs).
**Unblocks:** Universality claim for the resource decomposition conjecture. Directly feeds Paper 6 framing chapter.

### 1.10 Analytical proof of +2 writhe loss — **NEW (D83)**
**What:** D83 showed that Jones normalization costs exactly 2 XOR levels through angular vocabulary reduction (43 → 31 angles at ζ₁₂, directions unchanged at 512). The mechanism is the (-A³)^{-w} phase factor, which acts as a rotation by `-(3*half_angle + pi)*w` per entry. Prove analytically that this rotation collapses exactly enough angle equivalence classes to destroy 2 XOR levels for any root.
**Why:** The "+2 per writhe unit" is currently a demonstrated computational result at two roots. An analytical proof would elevate it from "verified at ζ₈ and ζ₁₂" to a theorem about the bracket-to-Jones relationship. The key insight is that the Jones phase per unit writhe creates angle equivalence classes whose count reduction corresponds to exactly 2 XOR levels — this should be provable by counting orbits of the phase rotation on the angular lattice.
**Effort:** MEDIUM (the mechanism is identified — angle reduction via phase rotation — but formalizing "2 XOR levels worth of angle classes" requires connecting the D82 depth law to angle counting).
**Unblocks:** Elevates the framing result from empirical to proven. High publication value — directly connects Kirby's framing (1978) and Witten's anomaly (1989) to a computational capacity theorem.

### 1.11 ζ₁₂ null indispensability — **NEW (D84)**
**What:** D84 demonstrated null indispensability at ζ₈: 9/24 entries are bracket-null (Re(q)=0), they contribute 6 exclusive directions (cube-edge-midpoints), and removing them drops capacity from XOR8 to XOR6 — worse than removing a random equal-size subset. At ζ₁₂ (4096-entry truncation), 121 entries (3%) are null with 29 null-only directions. Are those 29 null-only directions similarly essential for the XOR12 solutions found in D79? The null fraction dilution (75% → 37.5% → 3.0% across ζ₄ → ζ₈ → ζ₁₂) suggests nulls become rarer but possibly remain critical.
**Why:** If ζ₁₂ nulls are indispensable despite being only 3% of the catalog, it establishes null indispensability as a universal structural property rather than a finite-group artifact. If they are dispensable, the LCFT interpretation is specific to the finite-group regime and the reservoir computing connection narrows accordingly.
**Effort:** SMALL (D84's `load_subset()` and `find_capacity()` infrastructure already exists; port to ζ₁₂ catalog, run non-null-only capacity test, compare to random controls).
**Unblocks:** Determines whether null indispensability is universal or ζ₈-specific. Critical for the LCFT/Reservoir Computing narrative in Paper 6.

---

## Tier 2: Do Soon

### ~~2.1 ell=7 verification~~ — DONE (Demo 60)
All predictions confirmed: rad(TL_7) = 11 = 2*7-3, rad(TL_8) = 39 = 49-7-3, rad^2 = 1-dimensional, nilpotency = 3, Fibonacci rank = F(6) = 8. Cross-validated at two independent primes. Universality confirmed beyond quadratic fields. Paper 3 strengthened.

### 2.2 Axiality: determine if it follows from known Galois theory
**What:** Consult the cyclotomic Galois theory literature (or a number theorist) to determine whether the axiality theorem at delta=0 is a trivial consequence of Gal(Q(zeta_8)/Q) acting on bracket values. Write up the answer either way.
**Why:** If axiality is derivable from standard theory, Paper 4 is a short note and the novelty claim must be downgraded. If it is NOT trivially derivable, it is a significant result. Either way, knowing the answer is critical before claiming novelty in print.
**Effort:** SMALL (research task, not a coding demo).
**Unblocks:** Determines whether Paper 4 exists as a standalone paper or is absorbed into Paper 1/3.

### 2.3 Sharpen the relationship to Aizenberg 2008
**What:** Explicitly state how forward DKC relates to Aizenberg's known UBN parity construction (w1=zeta_8, w2=i, w3=-1). Show that these specific weights ARE bracket values in the catalog. Measure how many of the 906 parity solutions at k=6 reduce to Aizenberg's construction under gauge equivalence.
**Why:** A reviewer in the CVNN community will immediately ask this. If all 906 solutions are gauge-equivalent to Aizenberg's, the novelty is "the weights come from topology" (still novel, but narrower). If many are gauge-inequivalent, the catalog provides genuinely new solutions Aizenberg's framework doesn't predict.
**Effort:** SMALL (1 demo — gauge-equivalence classification of the 906 solutions).
**Unblocks:** Precise novelty claim for Paper 1. Prevents a reviewer objection from derailing the submission.

### 2.4 Rate region analysis
**What:** Apply Nazer-Gastpar capacity bounds to the DKC lattice. How many bits per bracket value can DKC extract? Formalize the two-channel (magnitude + phase) coding conjecture with actual achievable rate calculations.
**Why:** The ~4.3-bit magnitude + ~1.5-bit phase = ~5.8 bits/symbol claim is currently informal. A rate region analysis connects DKC to the information-theoretic community and could be a standalone result or a strong section in Paper 1.
**Effort:** MEDIUM (1-2 demos — capacity computation + comparison to Shannon bound).
**Unblocks:** Two-channel coding conjecture (confirm or refine). Potential Paper 5 or strong section in Paper 1.

### 2.5 Multi-layer DKC
**What:** Cascade two bracket-valued neurons: output of layer 1 feeds layer 2. Can a 2-layer DKC network compute functions that a single layer cannot (e.g., parity under split-sigmoid)?
**Why:** Single-layer DKC with split-sigmoid hits the parity wall. If multi-layer circumvents it WITHOUT changing activation, the wall is about depth, not just activation geometry. This would be a significant structural result.
**Effort:** MEDIUM (2 demos — 2-layer architecture + systematic search).
**Unblocks:** Determines whether DKC's computational limits are single-layer phenomena or fundamental to the bracket algebra.

### ~~2.6 Z[zeta_16] scaling~~ — DONE (Demo 65)
Parity ceiling n=5 is unchanged at Z[zeta_16]. Gap-of-2 hypothesis REFUTED: pigeonhole bound rises from 7 to 14 but ceiling stays at 5, widening gap to 9. Standard XOR6 = 0 at all k=2..31 (both 36-value and 56-value catalogs). Generalized (non-standard) activations break the wall at k=24. Direction Nesting Theorem (D69): ζ_8 directions nest exactly into ζ_16 and ζ_32 catalogs (algebraically forced).

### 2.7 Exact Cyc8 arithmetic for generalized activation check — still open
**What:** D65's generalized XOR6 check (binary labeling of k sectors) uses floating-point atan2 for sector assignment. All passing solutions have exactly zero margin to sector boundaries, making boundary cases ambiguous under floating-point. Rewrite the generalized check with exact Cyc8 arithmetic (no atan2) to formally eliminate boundary ambiguity.
**Why:** Zero margin universality is a theorem (D65 proven), but if the sector assignments are made by floating-point atan2, there is a non-zero probability that a case at exact boundary is misclassified, creating ghost solutions or missed solutions. Cyc8 arithmetic eliminates this concern entirely and removes the last floating-point gap in the generalized DKC results.
**Note:** D72 is a spherical design test, NOT an exact activation verification. This item remains open. A floating-point audit of the generalized activation framework identifies this as medium-difficulty exactification: the Cyc8 infrastructure exists from D29/D64, but porting sector classification from atan2 to exact rational angle comparison requires careful rethinking of boundary conditions.
**Effort:** SMALL (Cyc8 infrastructure exists from D29/D64; port sector classification from atan2 to exact angle comparison using numerator/denominator rational arithmetic).
**Unblocks:** Formalizes all generalized XOR6 results. Required before claiming the k=24 threshold is proven rather than verified.

### 2.8 Trivialization threshold localization — still open
**What:** D69 shows ζ_8 Voronoi (14 cells) gives 36 solutions (topology-driven) and ζ_16 Voronoi (3,457 cells) gives ~90% solutions (pigeonhole-dominated). The transition lies somewhere in ~50–500 cells. Map the solution fraction as a function of cell count in this range to locate the transition precisely.
**Why:** The trivialization threshold is where the geometric structure of DKC ceases to be the controlling factor and statistical abundance takes over. Knowing this threshold tells you exactly how fine a resolution you can use before the computation becomes trivial — relevant to both theory (where does topology end?) and practice (how coarse can an implementation be?).
**Note:** D74 is a computational invariant result (bracket/cell incomparability, 119 classes), NOT a trivialization threshold measurement. This item remains open.
**Effort:** SMALL (extend D69's fixed-resolution comparison across the 50–500 cell range using the existing Voronoi search harness).
**Unblocks:** Confirms or refutes the ζ_16 prediction from D71's DOF formula. Closes a key open question from D69.

### 2.9 k=38 anomaly investigation — still open
**What:** In D65's even-k generalized XOR6 sweep, k=38 is the only even k ≥ 24 with zero solutions. Every other even k in the range (24, 26, 28, ..., 62) has at least 2 solutions. k=38 = 2×19 where 19 is prime. Investigate whether 19 is special with respect to the Z[zeta_8] lattice (e.g., 19 ≡ 3 mod 8, or some incommensurability with the pi/4 angular spacing).
**Why:** Anomalies at specific k values in the generalized search indicate deeper lattice structure. The k=38 gap is unexplained and may hide a nontrivial number-theoretic obstruction — or it may be a coincidence that disappears with a larger candidate set. Either outcome is informative.
**Effort:** SMALL (focused arithmetic investigation; no new infrastructure needed).
**Unblocks:** Either resolves as a coincidence or reveals a structural result about 19-fold symmetry and the Z[zeta_8] lattice.

### 2.10 Direct deep-entry generation — **NEW**
**What:** D82 showed that deep entries (depth 8 in the ζ₁₂ closure) are 2× more efficient than shallow or strided entries at equal catalog size: 564 pure depth-8 entries reach XOR12, while mixed catalogs need ~1140 entries. Can depth-d entries be characterized and generated/sampled directly without first computing all shallower depths via full BFS closure? The algebraic signature of a depth-8 entry is that it is a product of exactly 8 generators — but can we enumerate these without traversing the full exponential BFS tree?
**Why:** If direct deep-entry generation is possible, it eliminates the exponential BFS cost and allows immediate access to the high-capacity part of the catalog. This would be a practical breakthrough for high-XOR computation and would separate the "hard part" (generating deep entries) from the "easy part" (searching for winners).
**Effort:** MEDIUM (requires characterizing the algebraic structure of depth-d entries; may involve coset enumeration or lattice-basis techniques).
**Unblocks:** Reduces the computational cost of high-XOR DKC. Potential connection to group theory (generator word enumeration without full BFS).

### 2.11 Optimal depth for target XOR level — **NEW**
**What:** D82 demonstrated that pure depth-8 entries outperform shallow and strided at equal catalog size (564 entries → XOR12 vs XOR10). But is pure depth-8 truly optimal, or would a mixed depth (e.g., pure depth-6 or depth-7, or a calibrated shallow-core + deep-extensions mix) achieve XOR12 with fewer total entries? The two-component winner architecture (shallow core + deep extensions, D82) suggests a hybrid strategy might be near-optimal.
**Why:** The optimal depth strategy minimizes the catalog size needed for a target XOR level, which directly determines the computational cost of building the catalog (exponential BFS). If XOR12 is reachable at 200 entries with the right depth mix, instead of 564 pure-deep entries, that is a 2.8× efficiency gain.
**Effort:** SMALL (sweep depth composition at matched catalog sizes; D82 infrastructure already handles arbitrary subsets).
**Unblocks:** Practical optimization for high-XOR computation. Feeds into the XOR14 search (D81 predicts ~38K entries; optimal depth composition may reduce this significantly).

### 2.12 Union catalog ζ₈ + ζ₁₂ — **NEW**
**What:** D79 confirmed that ζ₈ and ζ₁₂ catalogs are largely non-overlapping (12% quaternion overlap, 15% direction overlap). D82 showed algebraic coherence (shared intermediate products within a root) determines capacity. A union catalog ζ₈ ∪ ζ₁₂ exploits two independent algebraic structures simultaneously — does this provide superadditive XOR capacity, or does the incoherence between the two roots prevent synergy?
**Why:** If the union catalog outperforms either alone at matched size, it establishes that algebraic diversity (multiple roots) and algebraic coherence (depth within a root) are complementary, not competing, mechanisms. This would be a significant practical result: the best strategy combines roots rather than going deeper within one.
**Effort:** SMALL (combine D66's ζ₈ and D79's ζ₁₂ catalogs; run the existing search harness on the union).
**Unblocks:** Directly tests D79's Union Catalog Conjecture. Feeds into Paper 6 as the "best known" capacity baseline.

### 2.13 Depth law at other roots of unity — **NEW**
**What:** D82 established max_xor ≈ depth + 6 for ζ₁₂. Does this linear law hold at ζ₁₀? With a different constant? D80 showed ζ₁₀ has 71 distinct half-angles at 4096 entries (most of any surveyed root) — its angular richness may shift the constant or alter the depth law character entirely. Similarly, does the law hold at ζ₁₆?
**Why:** If the depth law is universal (same constant across all infinite-group roots), it is a fundamental property of the quaternionic DKC mechanism. If the constant varies, it is a function of angular density per closure round — which would give a formula relating root of unity to computational efficiency, directly informing root selection for practical applications.
**Effort:** SMALL to MEDIUM (port D82's depth-tracking closure and capacity measurement to ζ₁₀ and ζ₁₆; D80 already has the closure infrastructure).
**Unblocks:** Universality vs. root-dependence of the depth law. Critical for Paper 6's claims about scaling.

### 2.14 Formal "activation determines capacity" theorem — **NEW**
**What:** D77 empirically demonstrated across 14 activation functions that the activation choice, not the root of unity, determines computational capacity: same ζ₈, same 24-cell, 0 XOR8 solutions with S²-Voronoi, 6 solutions with S¹×S²-Sec×Vor. Can this be stated and proven as a formal theorem? The statement would be roughly: for any finite quaternion group G and any target Boolean function f, there exists an activation function A such that DKC(G, A) computes f if and only if f is below the group's structural capacity ceiling.
**Why:** "Activation determines capacity" is currently a striking empirical result (14 activations compared, monotone phase transition measured). A formal theorem would make it the central theoretical contribution of Paper 6 — elevating it from "we found an activation that works" to "we proved why activation is the key variable."
**Effort:** MEDIUM (requires formalizing the capacity ceiling of a finite quaternion group; the incomparability result from D74 — that eigenvalue and eigenvector directions capture orthogonal information — is the key ingredient).
**Unblocks:** Paper 6's theoretical core. Connects D74 (incomparability), D77 (activation zoo), and D80 (finiteness) into a unified theorem.

### 2.15 Explicit Jordan-cell pairs — **NEW (D84)**
**What:** D84's LCFT interpretation identifies 3 shared-direction entries (coordinate axes) where both null and non-null entries exist on the same S² direction. These are the candidate Jordan-cell pairs: the null entry is |N⟩ (trace=0) and the non-null entry is its logarithmic partner |L⟩. Do these 3 pairs form literal Jordan cells in some representation of the braid group algebra? Specifically, is there a natural operator O on the 24-dimensional space (indexed by catalog entries) such that O acts on the null/non-null pair with a 2×2 Jordan block?
**Why:** Making the LCFT connection explicit (from "interpretation" to "theorem") would be a significant result connecting DKC to logarithmic CFT in a formally verifiable way. If no such operator exists, the LCFT analogy is suggestive but not structural.
**Effort:** MEDIUM (requires constructing the braid group representation on the 24-element catalog space and checking for non-diagonalizable blocks at the shared directions).
**Unblocks:** Formally connects DKC to LCFT. Determines whether the Jordan-cell interpretation is analogy or isomorphism.

### 2.16 ζ₁₀ null census — **NEW (D84)**
**What:** D84 measured null fractions at ζ₄ (75%), ζ₈ (37.5%), and ζ₁₂ (3.0%) but not ζ₁₀. ζ₁₀ is the Fibonacci anyon root with infinite group and 71 distinct half-angles at 4096 entries (D80). The null fraction and directional role of null entries at ζ₁₀ is unknown. Does the null fraction follow the dilution pattern, or does ζ₁₀'s anomalous angular richness (71 half-angles vs. 43 at ζ₁₂) create a different null geometry?
**Why:** ζ₁₀ is the canonical "Fibonacci anyon" root with deep connections to topological quantum computation. Its null structure completes the cross-root comparison and tests whether null dilution is monotonic or shows anomalies at specific roots. The 71-angle richness may make the ζ₁₀ null geometry qualitatively different.
**Effort:** SMALL (D84's cross-root comparison loop already handles arbitrary roots; add ζ₁₀ as a third test case).
**Unblocks:** Completes the null census across all major roots. Feeds into the null dilution conjecture and LCFT narrative.

### 2.17 Resource decomposition at intermediate depths — **NEW (D83)**
**What:** D83 established the additive resource decomposition at the maximum capacity level: lattice base (XOR6) + 2 × depth_rounds + 2 × framing_present. The "+2 per resource unit" pattern is confirmed at maximum depth. But does the bracket-vs-Jones gap hold at every depth level, or does the +2 writhe contribution vary with depth? At intermediate depths (e.g., depth-4 or depth-6 in the ζ₁₂ catalog), does the bracket outperform Jones by exactly 2, or does the gap open gradually?
**Why:** If the +2 gap is constant across all depths, the resource decomposition is truly additive and depth-independent. If the gap varies, the interaction between depth and writhe is nontrivial, which would modify the resource formula and may reveal a depth-writhe coupling term.
**Effort:** SMALL (D83's `save_catalog()` / `restore_catalog()` and `jones_normalize_catalog()` infrastructure supports arbitrary depth-limited subsets; run bracket vs. Jones at each depth stratum).
**Unblocks:** Confirms or refines the additive resource decomposition. Determines whether depth and writhe are truly independent axes or coupled.

---

## Tier 3: Someday

### 3.1 Abramsky depth: Geometry of Interaction interpretation
**What:** Develop the Geometry of Interaction interpretation to predict which bracket triples compute which functions BEFORE exhaustive search. The goal is a type-theoretic criterion.
**Why:** Currently, "which weights compute which function" is answered by brute force. A GoI interpretation would give a principled theory of DKC computability — the equivalent of a type system for topological neural networks.
**Effort:** LARGE (research program — requires deep engagement with Abramsky's categorical framework).
**Unblocks:** Theoretical prediction of computability without exhaustive enumeration. The "grand theory" of DKC.

### ~~3.2 5-input parity and beyond~~ — DONE (Demo 63)
5-input parity resolved as part of Demo 63 (k=15, 3020 solutions). Triskelion generalization FALSIFIED. Z[zeta_16] scaling is the natural next step.

### 3.3 Physical interpretation via Costello-Francis-Gwilliam
**What:** Costello-Francis-Gwilliam (2026) proved Witten = Reshetikhin-Turaev rigorously. Interpret forward DKC in terms of compiling QFT transition amplitudes into Boolean logic.
**Why:** "DKC compiles quantum field theory into Boolean logic" is an extraordinary claim that the CFG result makes rigorous. But the connection is conceptual, not yet formal.
**Effort:** LARGE (research program — requires engaging with derived algebraic geometry).
**Unblocks:** The deepest possible framing of DKC. Connects to physics community.

### 3.4 Galois symmetry conjecture
**What:** Investigate whether the sandwich duality (top layer isomorphic to bottom layer in the Loewy filtration) corresponds to a Galois automorphism of Gal(Q(zeta_8)/Q).
**Why:** If true, it explains a structural coincidence as an algebraic necessity. But it is currently pure speculation with no clear approach.
**Effort:** MEDIUM (requires number theory expertise — likely needs a collaborator).
**Unblocks:** Deep algebraic explanation for the sandwich theorem. Potential connection to Langlands-type phenomena.

### 3.5 GPTQ/Babai formalization
**What:** Formalize the observation that GPTQ weight quantization (Babai nearest-plane on Z^n) and DKC lattice computation (exact arithmetic on Z[zeta_8]) are two instances of lattice-based neural network theory.
**Why:** Connects DKC to the practical ML quantization community. Could yield a joint framework where training = lattice closest-vector search and DKC = exact lattice computation.
**Effort:** MEDIUM (1-2 demos + writing).
**Unblocks:** Bridge to applied ML community. Potential high-impact framing.

### 3.6 Volume conjecture connection
**What:** Investigate whether the volume conjecture (WRT invariants at large r relate to hyperbolic volume) has DKC implications. Does hyperbolic volume predict computational capacity?
**Why:** Speculative but would connect DKC to the deepest open problem in quantum topology.
**Effort:** LARGE (research program).
**Unblocks:** Connection to hyperbolic geometry and the broader quantum topology program.

### 3.7 Exact 27-state DKC automaton
**What:** D73 showed the 13-cell Voronoi partition achieves 82.8% automaton determinism for XOR6. The signed 27-cell partition achieves 87.1%. A fully deterministic 27-state automaton (if it exists) would compute XOR6 with guaranteed state transitions — no probabilistic ambiguity. Is there a 27-partition of S² achieving 100% determinism, or does 100% require tracking magnitude information beyond direction?
**Why:** A fully deterministic finite automaton for DKC would be the cleanest possible formal model of the computation. The 82.8% universal constant (identical across all 36 winning triples) is a compelling structural result; 100% would be a theorem.
**Effort:** MEDIUM (requires searching over 27-cell partitions of S²; no new infrastructure needed but the search space is large).
**Unblocks:** Formal automaton model of DKC. Potential connection to the 13=13 bandwidth theorem.

---

## Summary: Critical Path

The shortest path to a strong first publication (Paper 1):

```
1.5 (catalog completeness) + 2.3 (Aizenberg relationship)
            |
            v
    [1.2 DONE: 4-input, 5-input, ceiling n=5, OM classification]
    [D64 DONE: matroid minor chain, funnel theorem, self-doubling impossibility]
    [D65 DONE: Z[zeta_16] parity ceiling confirmed, gap-of-2 refuted]
            |
            v
      Paper 1 submission
```

Paper 1 is significantly strengthened by D63–D65. Remaining gaps: catalog completeness (1.5) and Aizenberg relationship (2.3).

The shortest path to Paper 2 (11/13 theorem):

```
1.6 (convexity proof)
      |
      v
Paper 2 submission
```

The shortest path to Paper 3 (radical anatomy):

```
[1.1 rad dim formula PROVEN] --> [2.1 ell=7 DONE] --> [Markov RT truncation PROVEN]
         |
         v
  1.1 remaining (algebra isomorphism, nilpotency, Fibonacci rank)
         |
         v
  Paper 3 submission
```

Paper 3 strengthened by Markov RT truncation proof and next-level radical formula proof.

The shortest path to Paper 5 (24-Cell, S², and Spectral Geometry of DKC):

```
D66 (24-cell emergence, F4 orbits, 7/7/11 theorem)
D67 (S² as computational sweet spot, Hopf phase inertness)
D68 (stereographic: computation intrinsically curved)
D69 (Direction Nesting Theorem, ζ_8 = sweet spot)
D70 (musical mapping, Yang-Baxter melody invariance)
D71 (spectral DKC, 13=13 bandwidth theorem — COMPUTED)
D72 (anti-correlation: worst t-design = most solutions — STRENGTHENS Paper 5)
D74 (bracket/cell incomparability, 119 classes — STRENGTHENS Paper 5)
         |
         v
1.3 NEW (analytical proof of 13=13 bandwidth bound)
1.4-NEW (XOR7 on S² — tests framework scaling, updated by D78/D80 context)
2.7 (Cyc8 arithmetic for generalized activation check)
         |
         v
  Paper 5 submission
```

Paper 5 is entirely new — enabled by D66–D71. D72's anti-correlation result (algebraic eigenvector placement is anti-correlated with sampling quality, proving the computation is algebraic not geometric) and D74's incomparability theorem (bracket and cell are incomparable invariants with 119 combined classes) both strengthen the Paper 5 narrative. The S² arc significantly strengthens Paper 1 as well.

The shortest path to Paper 6 (Crossing Depth, Group Finiteness, and Capacity Scaling in DKC) — **NEW, updated D72-D84**:

```
D76 (XOR8 = 0 at ζ₈ S²-Voronoi: capacity limit)
D77 (XOR8 = 6 with Sec8×Vor: wall was activation, not root — 14 activation comparison)
D78 (XOR10 = 0 at ζ₈: finite group ceiling confirmed)
D79 (XOR10 = 124, XOR12 = 50+ at ζ₁₂: infinite group breaks wall)
D80 (only ζ₄/ζ₈ finite: ADE classification, quantum dimension vanishes at ζ₈)
D81 (0.62 XOR per doubling: logarithmic scaling law)
D82 (max_xor ≈ depth+6: linear depth law, algebraic coherence > vocabulary)
D83 (framing = +2 XOR levels: bracket vs Jones, writhe as independent resource axis)
D84 (null indispensability: 9/24 nulls essential for XOR8, LCFT Jordan-cell interpretation)
         |
         v
1.7 NEW (ζ₁₂ Sec×Vor activation — optimize product activation at ζ₁₂)
1.8 NEW (ζ₃₂ finiteness test — confirm/refute Power-of-Two Conjecture)
1.9 NEW (framing test at ζ₁₆/ζ₂₄ — confirm +2 universality)
1.10 NEW (analytical proof of +2 writhe loss — theorem-level framing result)
1.11 NEW (ζ₁₂ null indispensability — universal or ζ₈-specific?)
2.14 NEW (formal "activation determines capacity" theorem)
         |
         v
  Paper 6 submission
```

Paper 6 arc is the most complete new arc since the original DKC discovery. The key narrative: the research shifted from "can DKC compute parity?" (answered at Demo 50) to "how far does DKC scale?" (D76–D84). The scaling arc established: (1) the finite-group ceiling at ζ₈ (XOR8 maximum), (2) logarithmic scaling at ζ₁₂ (0.62 XOR per catalog doubling), (3) the linear depth law (max_xor ≈ depth+6, explaining the logarithmic law as an artifact of exponential BFS growth), (4) the activation-determines-capacity principle (D77), (5) framing as a discrete computational resource worth +2 XOR levels, independent of and additive with depth (D83), and (6) null states as structurally indispensable despite zero trace, with LCFT Jordan-cell interpretation (D84). The paper scope is now D72–D84. Key remaining experiments: framing universality at additional roots (1.9, small), analytical writhe loss proof (1.10, medium, high publication value), ζ₁₂ null test (1.11, small), product activation optimization at ζ₁₂ (1.7, high impact), and the ζ₃₂ finiteness test (1.8, small effort, high clarity). D85 (indecomposability parameter b) is currently in development and will further extend the LCFT connection.

---

## Open Questions

### Resolved by D64–D71
- ~~Z[zeta_16] parity ceiling~~: n=5, unchanged (D65). Gap-of-2 refuted; gap = 9 at Z[zeta_16].
- ~~Clifford staircase conjecture (D64)~~: PARTIALLY REFUTED (D65). Standard activations: ceiling stays at n=5 at all cyclotomic levels. Generalized activations: wall breaks at k=24 on Z[zeta_8], not by Clifford level.
- ~~Complex k=24 as projection of 24-cell~~: CONFIRMED (D66). S¹ k=24 minimum is the trace projection of the 24-cell's 24 vertices.
- ~~ζ_8 vs ζ_16 eigenvector nesting~~: PROVEN algebraically (D69). All 13 ζ_8 directions appear identically in ζ_16 (13/13, 0.000° drift).

### Resolved by D72–D84
- ~~Is ζ₈ the only root supporting XOR8?~~: RESOLVED (D77). XOR8 is solvable at ζ₈ with Sec(8)×Voronoi activation. The Demo 76 failure was activation-specific, not root-specific.
- ~~Is XOR10 solvable?~~: RESOLVED (D79). XOR10 = 124 winners at ζ₁₂. The wall was finite-group exhaustion at ζ₈, not a fundamental impossibility.
- ~~Why does the 24-cell generate a finite group?~~: RESOLVED (D80). Binary octahedral group (E₇, ADE classification). Only ζ₄ and ζ₈ generate finite groups under the two-generator construction; these are exactly the lattice points where quantum dimension [2]_q vanishes.
- ~~What is the scaling law for XOR capacity vs. catalog size?~~: RESOLVED (D81). Logarithmic: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6 at ζ₁₂.
- ~~Why logarithmic?~~: RESOLVED (D82). The logarithm is an artifact: catalog grows exponentially per closure round, and capacity grows linearly with depth, so log(exponential) = linear. The fundamental law is max_xor ≈ depth + 6.
- ~~DKC is additive or multiplicative?~~: RESOLVED (D73). Additive. Multiplicative braid product formulation cannot compute XOR6 — flipping all bits maps M→M⁻¹, which shares the same cell but changes parity.
- ~~Does sampling quality (t-design) explain why 13 directions suffice?~~: RESOLVED (D72). No. The 13 eigenvector directions are the worst possible t-design (t=0). Sampling quality is anti-correlated with computational quality. The DOF coincidence (2l+1=13=directions) explains DKC, not the t-design property.
- ~~What is the N-2 pattern?~~: RESOLVED (D83). The "N-2" observation from D79 was Jones capacity, not a bracket truncation artifact. The bracket genuinely reaches XOR(N) at each root; earlier demos that stopped at XOR(N-2) were inadvertently computing Jones-normalized quantities. Framing (writhe) is a discrete computational resource worth exactly +2 XOR levels: bracket XOR12 → Jones XOR10 at ζ₁₂, bracket XOR8 → Jones XOR6 at ζ₈.
- ~~Are bracket-null entries computational dead weight?~~: RESOLVED (D84). No. At ζ₈, 9/24 entries are bracket-null (Re(q)=0) but contribute 6 exclusive S² directions (cube-edge-midpoints). Removing them drops capacity from XOR8 to XOR6 — worse than removing a random equal-size subset. Null entries are directionally efficient (1.00 dir/entry vs. 0.47 for non-null) and disproportionately important for higher-capacity solutions.

### Still Open
- Analytical proof of the 13=13 bandwidth bound (D71/D72 — top priority, see 1.3).
- n=6 constraint wall algebraic proof: is there a clean proof that the 63 constraints are unsatisfiable, or is it inherently a finite-verification result?
- Trivialization threshold: exactly where (50–500 cells) does the computation transition from topology-driven to pigeonhole-dominated? (see 2.8)
- k=38 anomaly: why is k=38 the only even k ≥ 24 with zero generalized XOR6 solutions? (see 2.9)
- XOR7 on S²: does the DOF formula generalize, and does S² reduce the k=127 S¹ requirement within the ζ₈ finite-group regime? (see 1.4-NEW, updated by D78/D80)
- Exact Cyc8 arithmetic for generalized activation sector classification — eliminates zero-margin boundary ambiguity (see 2.7).
- Oriented matroid generalization: what OM type characterizes n-input parity for n=4,5? (D64 open, not yet answered)
- Funnel rate formula: is the coverage fraction decay (99.3% → 39.2% → 19.2% → 0%) exponential, algebraic, or other?
- Poison octant 4 gap: why do poison values avoid octant 4 entirely (D64)?
- Edge-dominant/body-dominant F4 orbit split (22/13) in D66: does this exchange under 24-cell self-duality?
- ζ₃₂ finiteness: does the Power-of-Two Conjecture (finite groups at π/2^k only) hold? (see 1.8)
- ζ₁₂ Sec×Vor capacity: how much does product activation increase XOR capacity at ζ₁₂ beyond the k=8 baseline of D79? (see 1.7)
- Depth law universality: does max_xor ≈ depth + 6 hold at ζ₁₀, ζ₁₆? Different constant? (see 2.13)
- Formal "activation determines capacity" theorem: can D77's empirical result across 14 activations be proven formally? (see 2.14)
- Union catalog ζ₈ + ζ₁₂: does combining two largely non-overlapping catalogs provide superadditive XOR capacity? (see 2.12)
- Optimal depth composition for target XOR: is pure depth-8 the best, or does a mixed shallow-core + deep-extensions strategy reach XOR12 with fewer entries? (see 2.11)
- Direct deep-entry generation: can depth-d entries be sampled without full BFS closure? (see 2.10)
- 6 unseparated pairs (D74): what stronger invariant separates the 6 braid pairs sharing both Voronoi cell and bracket value?
- Exact 27-state DKC automaton: does a 27-partition of S² exist achieving 100% automaton determinism for XOR6? (see 3.7)
- Is ζ₁₂ truly infinite or very large finite? (D79 open — concrete group theory question)
- Does the 2.3× binocular capacity gain over cell-only hold for other roots, or is it ζ₈-specific? (D75 open)
- Information-theoretic lower bound for ζ₈ XOR ceiling: clean formula relating max computable N to catalog size, direction count, and angle count?
- Framing universality: does the +2 writhe loss hold at ζ₁₆ and ζ₂₄? (D83 open, see 1.9)
- Analytical proof of +2 writhe loss: can the (-A³)^{-w} phase rotation's angle-class collapse be counted to prove the 2-level loss? (D83 open, see 1.10)
- ζ₁₂ null indispensability: are the 29 null-only directions at ζ₁₂ essential for XOR12 solutions? (D84 open, see 1.11)
- Explicit Jordan-cell pairs: do the 3 shared-direction null/non-null pairs at ζ₈ form literal Jordan cells in a braid group representation? (D84 open, see 2.15)
- ζ₁₀ null census: what is the null fraction and directional role at the Fibonacci anyon root? (D84 open, see 2.16)
- Resource decomposition at intermediate depths: does the +2 bracket-vs-Jones gap hold at every depth level, or does it vary? (D83 open, see 2.17)
- Writhe distribution of 24-cell vertices: do antipodal pairs (q and -q) have opposite writhes? (D83 open)
- Depth-3 → 100% null generalization: does the maximum-depth stratum of any finite SU(2) subgroup consist entirely of null entries? (D84 open)
- D85 (indecomposability parameter b) currently in development — will extend the LCFT connection from D84
