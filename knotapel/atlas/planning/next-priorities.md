# DKC Research: Next Priorities

Generated 2026-02-20. Updated 2026-02-26 after D102-D109 arc. Informed by synthesis documents (narrative, novelty, connections, four-lenses, vision-alignment, obstructions, arc-summaries, conjecture-graph). D64–D71 added: matroid recursion, Clifford staircase (Z[zeta_16]), 24-cell / quaternionic arc (D66–D71), spectral DKC. D72–D82 added: spherical design, eigenvector automaton, computational invariant, binocular DKC, 8-input scaling, activation zoo, recursive scaling, ζ₁₂ capacity, group finiteness survey, capacity scaling law, crossing depth. D83–D84 added: framing as computational content (+2 writhe loss), null states and logarithmic structure (null indispensability, LCFT Jordan-cell interpretation). D85–D92 added: indecomposability parameter b = -5/8 (D85-D86), regime transition for null dispensability (D87), anti-correlation mechanism and k-ladder resilience (D88), depth law mechanism fully explained (D89-D91: axis cancellation, cross-depth algebraic constraint, balanced exponentials, parity wall), parity-lock theorem (D92: +/-q encoding structurally locked to XOR/XNOR). D93–D94 added: complement-blindness theorem and phase_cell recovery (D93: 1wpi circuit complexity hierarchy, AND/XOR ratio explosion N=3-8, pigeonhole mechanism), solvability as capacity bottleneck confirmed via binary icosahedral group 2I (D94: Barrington's theorem computationally validated, five-pillar synthesis complete, "discrete algebraic reservoir computer" framing proposed). D95–D101 added: multi-strand DKC arc — commutator depth and cross-layer synergy (D95), 2D synergy landscape and catalog pruning (D96, LANDMARK), Cell B geometric perfection (D97), 3-strand readout bottleneck (D98), Delta_1 first-ever 3-strand XOR and Ext^1 catalytic preparation thesis (D99, LANDMARK), 4-strand non-semisimple DKC on W_{4,2} (D100), 5-strand sl_d functor thesis with Casimir inversion (D101). D102–D109 added: multi-strand scaling/diagnostics arc — 6-strand radical A/B test and Barrington-Radical Principle (D102), BFS growth as braid group invariant (D103), Atkinson sweet spot and branching decomposition (D104), 8-strand k-regime and macrame principle (D105); relational computation arc — topological entropy orthogonality and TL visibility filter (D106), Z/4Z Axis-Alignment Theorem with constructive proof + 5 novel results (D107), Dual-Channel Theorem and Amy bridge (D108), Encoding-Dependent Dual-Channel and polarity inversion (D109). Raqiya library created and deployed across D107-D109.

---

## Tier 1: Do Next

### ~~1.1 Formal proof of the sandwich theorem~~ — PARTIALLY DONE
The **radical dimension formula** (rad(TL_ℓ) = 2ℓ−3) is now formally proven in `proofs/radical-dimension-formula.md`. Method: Graham-Lehrer cellular theory, unique degenerate cell module V_{ℓ-2} with corank 1, Chebyshev determinant. Corollary: ℓ²−ℓ−3 at n=ℓ+1.
**Additionally proven:** Markov RT truncation (`proofs/markov-rt-truncation.md`) and next-level radical formula (`proofs/next-level-radical-formula.md`). These strengthen Paper 3.
**Remaining:** The full sandwich theorem (rad^2(TL_{2k}(0)) ≅ TL_{2k-1}(0) as algebras) is a stronger claim than the dimension formula. The proof proves dimensions match; the algebra isomorphism still needs formal proof. Also: nilpotency=3 and Fibonacci rank remain computationally verified, not proven.
**Conjecture-graph priority:** Rank 1 (C3). Unlocks C6, C7, C5, C9 downstream.

### ~~1.2 4-input parity: test the triskelion generalization~~ — DONE (Demo 63)
4-input parity reachable at k=8 (96 solutions, tetraskelion). 5-input parity at k=15 (3020 solutions, pentaskelion). **Parity ceiling for Z[zeta_8] is n=5.** Triskelion generalization (k=2n) **FALSIFIED** — n=5 requires k=15, not k=10. True scaling: k=2M-1 where M is ray count. Oriented matroid classification: alternating OM type required for parity. Two types of impossibility: constraint wall (n=6,7) vs pigeonhole (n>=8).

### 1.3 Analytical proof of the 13=13 bandwidth bound
**What:** Prove analytically that the minimum bandwidth for a binary Voronoi partition separating N points on S² in general position is l = ⌈(N−1)/2⌉. For XOR6 DKC, N=13 (eigenvector directions) giving l=6. D71 verified this computationally with a sharp phase transition (0% recovery at l≤5, 100% at l=6) and 80.7% average power in mode l=6 across all 36 solutions.
**Why:** The 13=13 theorem is the cleanest standalone publication target in the arc — self-contained, striking, and proven at the computational level already. An analytical proof would make it a rigorous theorem rather than a verified result, and would immediately imply the trivialization prediction for finer roots (ζ_16 → l≥1728) analytically.
**Effort:** SMALL to MEDIUM (key insight is degrees-of-freedom counting; the hard part is formalizing "general position" and proving no sub-Nyquist trick works at l=5).
**Unblocks:** Strengthens Paper 5 (24-Cell, S², Spectral Geometry). Turns D71's computational phase transition into a publishable theorem.
**Conjecture-graph priority:** Rank 4 (C19). Downstream: C20 (trivialization DOF prediction).

### 1.4-NEW XOR7 on S² — context updated by D78/D80
**What:** Test the spectral framework scaling on XOR7. D65 showed XOR7 requires k=127 sectors on S¹. D67 showed XOR6 drops from k=24 (S¹) to 14 cells (S²). If the DOF formula generalizes, XOR7 on S² should need l=⌈(N_7−1)/2⌉ where N_7 is the number of XOR7-relevant eigenvector directions. Measure: does S² reduce XOR7 from k=127 as dramatically as it did XOR6?
**Why:** Directly tests whether the spectral framework is a systematic tool for climbing the parity hierarchy, or whether XOR6 on S² is a special case.
**Context update (D78/D80):** At ζ₈, D78 confirmed XOR10 = 0 (ζ₈ wall is hard). D80 established that ζ₈ generates a finite group (24 elements) and this is the fundamental ceiling. XOR7 on S² is therefore a question about what can be achieved within the finite-group regime.
**Effort:** MEDIUM.
**Unblocks:** Determines whether Paper 5 covers a theorem or a single data point.

### 1.5 Catalog completeness argument
**What:** Prove or bound that the 100-value Z[zeta_8] catalog at delta=0 contains all distinct bracket values up to braid length L. Alternatively, prove asymptotic saturation (new values per additional crossing approaches zero).
**Why:** Every forward DKC exhaustive search ("0 out of 100M") depends on the catalog being complete enough. Without a completeness argument, a reviewer can ask "what if you missed a value?" This is the single most important gap for Paper 1.
**D107 context:** D107 found 186 distinct Z[zeta_8] values from 32768-entry catalog, with 72 new values at depth 8 (still growing). The value set has NOT converged — this makes the completeness argument harder but the characterization more urgent.
**Effort:** SMALL (1 demo — extend enumeration to length 10-12, track saturation curve, attempt an algebraic bound using axiality + single-loop selection).
**Unblocks:** Strengthens Paper 1 from "exhaustive over our catalog" to "exhaustive over all bracket values up to length L."

### 1.6 Convexity thesis: complete the formal proof
**What:** Formalize the argument that the 11/13 wall is a consequence of activation convexity (not just encoding geometry). D57 and D62 have the ingredients; what's missing is a single clean theorem statement with a self-contained proof.
**Why:** The 11/13 theorem (Paper 2) has clean proofs for the two unreachable classes, but the UNIFYING explanation (convexity) is informal. A formal convexity theorem would elevate Paper 2 from "two separate impossibility results" to "one structural theorem with two corollaries."
**Effort:** SMALL (1 demo — mostly writing, possibly one verification).
**Unblocks:** Paper 2 becomes a stronger, more unified submission.
**Conjecture-graph priority:** Rank 3 (C14). Downstream: C15 (unified 0x06/0x1B proof).

### 1.7 ζ₁₂ Sec×Vor activation for XOR10+ — **HIGH PRIORITY**
**What:** D79 demonstrated that ζ₁₂ breaks the ζ₈ wall for XOR10 (124 winners) and XOR12 (50+ winners) using Sec×Vor activation with the default sector count. D77 showed that Sec(8)×Vor (112 cells) is far more powerful than Voronoi alone (14 cells) at ζ₈. The combination has not yet been tested at ζ₁₂.
**Why:** This is the direct next step in the scaling arc. HIGH priority: immediate publication impact for Paper 6.
**Effort:** SMALL to MEDIUM (Sec×Vor infrastructure exists from D77; port to ζ₁₂ catalog, sweep k, measure capacity per round).
**Unblocks:** Paper 6 capacity chapter.

### 1.8 ζ₃₂ finiteness test
**What:** D80 proposed the Power-of-Two Finiteness Conjecture: finite groups occur exactly at angles of the form π/2^k (k ≥ 1), predicting ζ₃₂ (θ = π/16) should also be finite. A single test_root(32) call confirms or refutes.
**Why:** Extremely low implementation cost, high payoff either way.
**Effort:** SMALL (single test).
**Unblocks:** Confirms or refutes Power-of-Two Conjecture. Completes the ADE/finiteness survey for Paper 6.

### 1.9 Framing test at ζ₁₆/ζ₂₄
**What:** D83 demonstrated that Jones normalization destroys exactly 2 XOR levels at both ζ₈ and ζ₁₂. Testing at ζ₁₆ and ζ₂₄ would confirm whether the loss is truly universal across all roots.
**Why:** If universal, the Resource Decomposition Conjecture (C26) becomes stronger. Two additional data points close the main reviewer concern.
**Effort:** SMALL (D83 infrastructure auto-adapts).
**Unblocks:** Universality claim for resource decomposition. Paper 6 framing chapter.

### 1.10 Analytical proof of +2 writhe loss
**What:** Prove analytically that the (-A³)^{-w} Jones phase factor costs exactly 2 XOR levels through angular vocabulary reduction. The mechanism is identified (D83); what remains is counting angle-class orbits.
**Effort:** MEDIUM.
**Unblocks:** Elevates framing result from empirical to proven. High publication value.

### ~~1.11 ζ₁₂ null indispensability~~ — DONE (Demo 87)
D87 demonstrated that null indispensability is regime-dependent. At ζ₁₂ (infinite group, 4096 entries), removing all 121 bracket-null entries preserves full XOR6/8/10/12 capacity.

### 1.12-NEW 1wpi encoding depth law test — **HIGH PRIORITY (from D92)**
**What:** D92 proves the +/-q encoding is parity-locked. The 1wpi encoding (D48/D50) computes all 13 NPN classes but has never been tested for depth-law behavior. Does max_and or max_maj scale linearly with crossing depth under 1wpi encoding?
**Why:** THE most architecturally consequential open question for the compilation pipeline. The hybrid LLM vision needs AND/OR for syllogistic logic, not just parity. If the depth law holds under 1wpi, the entire scaling framework extends to non-parity functions.
**Effort:** SMALL to MEDIUM.
**Unblocks:** Determines whether the depth law is parity-specific or encoding-general. Critical for Paper 7 and the hybrid LLM vision.

### 1.13-NEW Encoding design for non-parity functions — **(from D92)**
**What:** The +/-q encoding is parity-locked; the 1wpi encoding computes all 13 NPN classes. Hybrid and novel encodings targeting specific Boolean functions are unexplored.
**Why:** Encoding determines computability more fundamentally than activation or catalog (D92). For any non-parity function, a different encoding is required.
**Effort:** MEDIUM (1-2 demos).
**Unblocks:** Opens the path from parity-only DKC to general Boolean function DKC.

### 1.14-NEW Prove balanced exponentials analytically — **HIGH PRIORITY (from D91)**
**What:** D91 confirmed the balanced exponentials explanation for the linear depth law: BFS vocabulary grows ~2x/round, parity demand grows 4x/weight, yielding slope ~1. Derive this analytically.
**Why:** Next clean formal proof candidate after the parity-lock theorem. Conjecture-graph Rank 2 (C23). Downstream: C26, C27, C28, C37.
**Effort:** SMALL to MEDIUM.
**Unblocks:** Elevates the depth law from "demonstrated at ζ₁₂" to a theorem.

### 1.15-NEW Regime transition characterization — **(from D87)**
**What:** D87 established the regime transition between ζ₈ (nulls indispensable) and ζ₁₂ (nulls dispensable). The exact threshold is unknown.
**Why:** A sharp threshold theorem gives a concrete design rule: below transition, preserve nulls; above, prune safely.
**Effort:** SMALL (port D87 infrastructure to ζ₁₀ and truncated ζ₁₂; sweep; locate crossover).
**Unblocks:** Completes finite/infinite group regime characterization.

### 1.16-NEW Paper 7 potential: D85-D92 mechanistic arc
**What:** The D85-D92 arc forms a coherent publication answering "why does DKC work and what are its structural limits?"
**Effort:** MEDIUM (writing + 1-2 confirming experiments from 1.12 and 1.14).

### 1.17-NEW RKHS kernel rank test (D95) — **HIGHEST PRIORITY (from D94)**
**What:** Compute DKC kernel matrix K(m,m') for 2I (60 entries) vs z8 (24 entries). If rank(K_2I)/rank(K_z8) > 120/24 = 5, non-solvability contributes above raw catalog size — confirming the "discrete algebraic reservoir computer" interpretation.
**Why:** One number validates or falsifies the entire five-pillar synthesis. The five-pillar synthesis is the most important theoretical result since the original DKC discovery.
**Effort:** SMALL (single demo).
**Unblocks:** Validates reservoir computing pillar. Critical for Paper 8.

### 1.18-NEW Higher k_sec with 2I — **(from D94)**
**What:** At k_sec=24 instead of 12. Does N=8 XOR become nonzero for 2I?
**Effort:** SMALL (parameter sweep in D94 infrastructure).
**Unblocks:** Tests whether 2I's angular richness is under-exploited at k=12.

### 1.19-NEW Depth law under phase_cell — **(from D93)**
**What:** Does the linear depth law hold under phase_cell activation (which recovers all 13 NPN classes)? Does the slope differ for different Boolean functions?
**Effort:** SMALL to MEDIUM.
**Unblocks:** Completes depth law generalization to all functions.

### ~~1.20-NEW 6-strand W_{6,4} decisive test~~ — RESOLVED (D102)
**D102 tested W_{6,4} head-to-head against W_{6,0}: Barrington-Radical Principle established.** Radical carries abelian writhe character, provably useless for parity. Simple W_{6,0} WINS at every XOR level. The question is now RESOLVED: non-semisimplicity provides only modest (+8%) mixing-row advantage at XOR6, not deep capacity. See Arc 11b (D102) for full results.

### 1.21-NEW Ext^1 thesis formalization — **(from D99)**
**What:** Formalize the Ext^1 catalytic preparation thesis: prove that the Ext^1 class in Ext^1(L(0), L(1)) for u_q(sl_2) at ell=2 is the unique obstruction to Property F, and that Jordan block structure is computationally necessary for XOR levels beyond the combinatorial regime.
**Why:** The Ext^1 thesis is the deepest theoretical finding of the multi-strand arc. D102 STRENGTHENS this: the radical itself is inert (abelian writhe), but the quotient structure of non-semisimple modules (the extension, not the radical) is the actual computational resource.
**Effort:** MEDIUM to LARGE (theoretical work).
**Unblocks:** Central theoretical contribution of Paper 9.

### 1.22-NEW Simple vs non-semisimple computational difference — **(from D100/D101)**
**What:** Systematic characterization of D100 vs D101 pattern. W_{6,4} test (D102) resolved the question partially but showed the distinction is more nuanced: it's not about radical content (which is inert) but about quotient structure.
**D102-D105 update:** D102 proved radical-as-resource REFUTED. D103 proved BFS is braid-group-invariant. D101's simple-module-deeper-capacity-deficiency is confirmed as structural. The systematic framework is now: radical = inert (abelian writhe character), extension = resource (Ext^1 catalytic preparation), simple = broader shallow but shallower deep.
**Effort:** SMALL (analysis + comparison table from existing D99-D105 data).
**Unblocks:** Systematic framework for representation choice. Paper 9.

### 1.23-NEW Additive encoding at delta=sqrt(2) — **NEW HIGH PRIORITY (from D109)**
**What:** D109 showed product closure polarity INVERTS between additive (D108) and multiplicative (D109) encodings. The cleanest confirmation: hold delta=sqrt(2) constant, switch to additive encoding. If D108-style discrimination (parity = HIGH product closure) returns, encoding-dependence is confirmed by a single controlled experiment.
**Why:** Single controlled experiment confirms the encoding-dependent dual-channel theorem. Low implementation cost (reuse D109 catalog, change encoding). Directly validates the polarity inversion discovered in D108-D109, which is the first result predicting computational capability from algebraic structure.
**Effort:** SMALL (reuse D109 infrastructure, change encoding parameter).
**Unblocks:** Confirms the encoding-dependent dual-channel theorem. Essential for Paper 10 (Raqiya arc). Validates the predictive capability analysis framework.

### 1.24-NEW Amy Bridge formalization — **NEW HIGH PRIORITY (from D108, conjecture-graph Rank 5)**
**What:** Map DKC dual-channel (D108) onto T-gate/Hadamard decomposition in Amy-Glaudell-Ross 2023. Product closure → sde (smallest denominator exponent, T-gate count); activation → Hadamard gate. Show the resource theory matches.
**Why:** Would connect DKC to quantum circuit synthesis resource theory — a major publication target. The dual-channel theorem (D108) provides the empirical foundation; D109's encoding-dependent polarity inversion constrains the mapping. Conjecture-graph Rank 5 (C51). Downstream: C53 (dual-channel generalization).
**Effort:** MEDIUM (requires detailed reading of Amy et al. 2023 + formal mapping).
**Unblocks:** Places DKC in established quantum circuit synthesis framework. Paper 10.

### 1.25-NEW Major index theorem for general W_{n,j} — **NEW (from D107, conjecture-graph Rank 8)**
**What:** D107 verified maj mod 2 = bipartite coloring at W_{4,2} (3/3) and W_{6,2} (9/9, uniquely winning among 4 candidates). Proof for general n,j is open. Need verification at W_{8,2}, W_{6,4}, W_{10,2} for more evidence.
**Why:** Novel algebraic invariant of TL link states. Researcher confirmed NOT a consequence of known KLR Z-grading (Plaza-Ryom-Hansen 2013). Low-cost verification at more modules, high publication value for proof. Conjecture-graph Rank 8 (C41).
**Effort:** SMALL (verification at additional modules) to MEDIUM (proof).
**Unblocks:** Novel algebraic result. Generalizes nesting parity grading.

### 1.26-NEW W_{8,2} rank saturation and head-to-head — **NEW (from D105)**
**What:** D105 showed W_{8,2} rank was severely under-sampled (34.9% at 16384 entries, sign expansion at 1.83x and still climbing). Head-to-head W_{8,0} vs W_{8,2} XOR comparison at matched k and oversampling has NOT been performed. Need ~130K entries for comparable oversampling.
**Why:** Unresolved empirical question from D105. Would settle whether dim-28 algebraic information provides advantage at sufficient activation resolution. D105's rank saturation confound means all prior cross-module comparisons are unreliable.
**Effort:** MEDIUM (requires larger BFS cap or smarter sampling).
**Unblocks:** Resolves the dim-28 vs dim-14 question at sufficient activation resolution.

### 1.27-NEW Macrame principle crossover location — **NEW (from D104-D105)**
**What:** D104 showed cross-block branching dominates at n=6 (asymmetric blocks dim 5+4). D105 showed within-block dominates at n=8 (symmetric blocks dim 14+14). Where is the exact crossover? W_{10,2} could bracket it.
**Why:** Gives a design rule for multi-strand activation: when to use cross-block vs within-block components. The crossover likely depends on block complexity.
**Effort:** MEDIUM.
**Unblocks:** Principled activation design rule for multi-strand DKC.

---

## Tier 2: Do Soon

### ~~2.1 ell=7 verification~~ — DONE (Demo 60)
All predictions confirmed. Paper 3 strengthened.

### 2.2 Axiality: determine if it follows from known Galois theory
**What:** Consult the cyclotomic Galois theory literature to determine whether the axiality theorem at delta=0 is a trivial consequence of Gal(Q(zeta_8)/Q).
**D107 update:** D107's Z/4Z Axis-Alignment Theorem STRENGTHENS the axiality question: the constructive nesting-parity phase formula provides a proof at the matrix-entry level (stronger than bracket-level axiality). Researcher confirmed the nesting parity Z/2Z grading is NOT a consequence of known KLR Z-grading. This makes formal upgrade of T1 proof sketch to rigorous proof (C1) more tractable — the proof strategy from D107 (bipartiteness + TL generator {0,1} structure) may directly yield the bracket-level proof.
**Conjecture-graph priority:** Rank 7 (C1). Downstream: C2 (axiality hierarchy).
**Effort:** SMALL (research task).
**Unblocks:** Determines whether Paper 4 exists as standalone.

### 2.3 Sharpen the relationship to Aizenberg 2008
**What:** Explicitly state how forward DKC relates to Aizenberg's known UBN parity construction. Show that these specific weights ARE bracket values in the catalog. Measure how many of the 906 parity solutions at k=6 reduce to Aizenberg's construction under gauge equivalence.
**Effort:** SMALL (1 demo).
**Unblocks:** Precise novelty claim for Paper 1.

### 2.4 Rate region analysis
**What:** Apply Nazer-Gastpar capacity bounds to the DKC lattice. How many bits per bracket value can DKC extract?
**D108-D109 update:** D108's Dual-Channel Theorem provides the algebraic anatomy of the two channels: multiplicative phase coherence (product closure) corresponds to the phase channel and additive magnitude diversity (v_2 connectivity) corresponds to the magnitude channel. D109 refines with encoding-dependence. The information-theoretic quantification remains open but the structural foundation is stronger.
**Effort:** MEDIUM (1-2 demos).
**Unblocks:** Two-channel coding conjecture.

### 2.5 Multi-layer DKC
**What:** Cascade two bracket-valued neurons. Can a 2-layer DKC network compute functions that a single layer cannot?
**D92 context:** Multi-layer composition is especially important because D92 shows the +/-q encoding limits single-layer to parity. Multi-layer with 1wpi encoding could be the path to arbitrary Boolean functions with depth-law scaling.
**Effort:** MEDIUM (2 demos).
**Unblocks:** Determines whether DKC's limits are single-layer phenomena or fundamental.

### ~~2.6 Z[zeta_16] scaling~~ — DONE (Demo 65)

### 2.7 Exact Cyc8 arithmetic for generalized activation check — still open
**What:** Rewrite D65's generalized XOR6 check with exact Cyc8 arithmetic (no atan2) to formally eliminate boundary ambiguity.
**Effort:** SMALL.
**Unblocks:** Formalizes all generalized XOR6 results.

### 2.8 Trivialization threshold localization — still open
**What:** Map the solution fraction as a function of cell count in the ~50–500 cell range.
**Effort:** SMALL.
**Unblocks:** Confirms or refutes the ζ_16 prediction from D71's DOF formula.

### 2.9 k=38 anomaly investigation — still open
**What:** Investigate whether 19 is special with respect to the Z[zeta_8] lattice.
**Effort:** SMALL.

### 2.10 Direct deep-entry generation — still open
**What:** Can depth-d entries be characterized and generated directly without computing all shallower depths?
**D89-D90 context:** Deep entries are valuable for their pairwise relationships (relational, not positional). This may make direct generation harder.
**Effort:** MEDIUM.
**Unblocks:** Reduces computational cost of high-XOR DKC.

### 2.11 Optimal depth for target XOR level — still open
**What:** Is pure depth-8 truly optimal, or would a mixed depth strategy achieve XOR12 with fewer entries?
**Effort:** SMALL.
**Unblocks:** Practical optimization for high-XOR computation.

### 2.12 Union catalog ζ₈ + ζ₁₂ — still open
**What:** Does combining two largely non-overlapping catalogs provide superadditive XOR capacity?
**Effort:** SMALL.
**Unblocks:** Tests Union Catalog Conjecture.

### 2.13 Depth law at other roots of unity — still open, updated by D89-D91
**What:** Does max_xor ≈ depth + 6 hold at ζ₁₀? ζ₁₆? With a different constant?
**D89-D91 context:** The balanced exponentials argument predicts slope should vary with BFS branching factor.
**Effort:** SMALL to MEDIUM.
**Unblocks:** Universality vs. root-dependence of the depth law. Critical for Paper 6.

### 2.14 Formal "activation determines capacity" theorem — still open, updated by D92 and D103-D105
**What:** Can D77's empirical result be proven formally, accounting for D92's encoding hierarchy?
**D103-D105 update:** D103 proves BFS growth is module-independent (activation geometry matters, not dimension). D104 identifies Atkinson sweet spot. D105's k-regime theory (k/2^N ratio controls capacity) provides the parametric foundation for formalizing what "activation determines capacity" means.
**Effort:** MEDIUM.
**Unblocks:** Paper 6's theoretical core.

### 2.15 Explicit Jordan-cell pairs — still open, updated by D85-D86
**What:** Do the 3 shared-direction null/non-null pairs at ζ₈ form literal Jordan cells?
**D85-D86 context:** b = -5/8 strengthens LCFT connection; D86 shows b is collective.
**Effort:** MEDIUM.
**Unblocks:** Formally connects DKC to LCFT.

### 2.16 ζ₁₀ null census — still open
**What:** What is the null fraction and directional role at the Fibonacci anyon root?
**Effort:** SMALL.
**Unblocks:** Completes null census.

### 2.17 Resource decomposition at intermediate depths — still open
**What:** Does the +2 bracket-vs-Jones gap hold at every depth level?
**Effort:** SMALL.
**Unblocks:** Confirms or refines additive resource decomposition.

### 2.18-NEW Spectral inversion formal proof — **(from D90)**
**What:** Prove BFS closure of finite SU(2) generators converges to uniform distribution on S² (bandwidth → 0) as depth → ∞.
**Why:** Spectral inversion is the cleanest evidence that depth law mechanism is relational, not positional.
**Effort:** SMALL to MEDIUM.
**Unblocks:** Strengthens relational computation thesis.

### 2.19-NEW Robustness engineering: constrained optimization at ζ₁₂ — **(from D88)**
**What:** At ζ₁₂ (2043 directions), can constrained optimization identify load-bearing subsets analogous to ζ₈'s body-diagonal anchors?
**Effort:** MEDIUM.
**Unblocks:** Engineering design rules for infinite-group compiled neurons.

### 2.20-NEW Quantitative product-closure threshold — **NEW (from D108-D109)**
**What:** D108 showed parity requires product closure > 0. D109 showed polarity inverts at delta=sqrt(2). At what product closure density does the additive-to-multiplicative transition occur? Is there a sharp threshold or gradual crossover?
**Why:** Would make the dual-channel theorem quantitatively predictive, not just qualitatively discriminative. Test by interpolating between encoding regimes.
**Effort:** MEDIUM.
**Unblocks:** Quantitative dual-channel theorem. Paper 10.

### 2.21-NEW TL visibility at delta != 0 — **NEW (from D106)**
**What:** D106's TL visibility filter is sharp at delta=0 where e_i^2=0. At delta!=0, e_i^2=delta*e_i != 0, meaning TL quotient is less destructive. Do dynamical properties (entropy, spectral radius) become visible to DKC at delta!=0?
**Why:** If entropy correlates with DKC at delta!=0, the TL visibility filter becomes a delta-dependent design parameter. Would test at delta=sqrt(2) using D109's infrastructure.
**Effort:** MEDIUM.
**Unblocks:** Delta-dependent visibility theory.

### 2.22-NEW Sign-rank expansion theory — **NEW (from D104-D105)**
**What:** D104 showed sign quantization EXPANDS effective rank (244→292 for W_{6,2}). D105 showed W_{8,2} expansion at 1.83x and still growing. What determines whether sign quantization expands or preserves rank?
**Why:** Would provide foundational principle for multi-strand activation design.
**Effort:** MEDIUM.
**Unblocks:** Principled activation engineering for multi-strand DKC.

### 2.23-NEW Radical Content XOR Correlation Study — **(from D100)**
**What:** Hub commutators have zero radical content — they annihilate the radical direction. Is this a general theorem for non-semisimple TL modules?
**D102 update:** D102's Barrington-Radical Principle (radical carries abelian writhe character) provides the theoretical framework: commutators of non-abelian group elements must annihilate the 1D abelian radical component.
**Effort:** MEDIUM.
**Unblocks:** Structural theorem for non-semisimple TL modules.

### 2.24-NEW Fibonacci Direct Test at q=exp(2pi*i/5) — **(from D99/D94)**
**What:** Test DKC capacity directly at the Fibonacci parameter on Delta_1 module. Does non-vanishing quantum dimension avoid Jordan blocks?
**Effort:** MEDIUM.
**Unblocks:** Bridges finite 2I group and Fibonacci anyon parameter through multi-strand lens.

### 2.25-NEW Non-Semisimple TQFT DKC Formalization — **(from D99-D101)**
**What:** Formalize connection to Voss-Kim-Bhatt 2025: is there a functor from non-semisimple TQFT category to DKC computation category?
**D102 update:** Barrington-Radical Principle sharpens the mapping: alpha-anyons → extension structure (not radical itself), sigma-anyons → qubits, semisimple readout → Boolean output.
**Effort:** MEDIUM.
**Unblocks:** Connects DKC to cutting edge of non-semisimple TQFT.

---

## Tier 3: Someday

### 3.1 Abramsky depth: Geometry of Interaction interpretation
**Effort:** LARGE.

### ~~3.2 5-input parity and beyond~~ — DONE (Demo 63)

### 3.3 Physical interpretation via Costello-Francis-Gwilliam
**D83 update:** Framing result adds a new angle — the "framing anomaly" that TQFT discards is exactly where DKC's +2 capacity lives.
**Effort:** LARGE.

### 3.4 Galois symmetry conjecture
**Effort:** MEDIUM.

### 3.5 GPTQ/Babai formalization
**Effort:** MEDIUM.

### 3.6 Volume conjecture connection
**Effort:** LARGE.

### 3.7 Exact 27-state DKC automaton
**Effort:** MEDIUM.

### 3.8-NEW Value set convergence at depth > 8 — **NEW (from D107)**
**What:** D107 found 186 distinct Z[zeta_8] values from 32768-entry catalog, with 72 new at depth 8 (still growing fast). Does the value set converge?
**Effort:** SMALL (increase BFS cap).

### 3.9-NEW 2-adic neutrality formal proof — **NEW (from D107)**
**What:** Five candidate proof frameworks identified (Kummer carry-counting, nilpotent TL, Goodman-Wenzl, unipotent, Stirling). Select and formalize.
**Effort:** MEDIUM.

### 3.10-NEW Raqiya Z[zeta_16] extension — **NEW (from D109)**
**What:** Full Raqiya support for Z[zeta_16] (8-component cyclotomic ring). Only worthwhile if delta!=0 investigation continues.
**Effort:** MEDIUM.

### 3.11-NEW Universal BFS Tree Conjecture Verification — **NEW (from D102-D103)**
**What:** All THREE n=6 modules produce identical BFS trees. Does this hold for ALL simple TL_n modules at delta=0? Test at n=8 and n=10.
**Effort:** SMALL.

### 3.12-NEW TL Visibility Filter Completeness — **NEW (from D106)**
**What:** Is the TL quotient the COMPLETE characterization of what DKC can "see"?
**Effort:** LOW (requires identifying candidate properties to test).

### 3.13-NEW Fibonacci Max-Abs Radical Constraint — **NEW (from D102)**
**What:** Does the radical act as a constraint keeping coefficients on Fibonacci sequence?
**Effort:** LOW.

---

## Summary: Critical Path

**Conjecture-graph Top 10 proof targets (from conjecture-graph.md Section 3.1):**

| Rank | ID | Statement | Downstream Impact |
|------|-----|-----------|-------------------|
| 1 | C3 | Sandwich theorem proof | Unlocks C6, C7, C5, C9 (4+ results) |
| 2 | C23 | Depth law slope=1 analytical | Unlocks C26, C27, C28, C37 |
| 3 | C14 | Convexity thesis | Unlocks C15, strengthens T32 |
| 4 | C19 | 13=13 analytical proof | Unlocks C20 |
| 5 | C51 | Amy Bridge (DKC = T-gate/Hadamard) | Unlocks C53 |
| 6 | C4 | Universal nilpotency 3 | Unlocks C3 transitively |
| 7 | C1 | Axiality formal proof | Unlocks C2, strengthens T5 |
| 8 | C41 | Major index bipartite invariant | Generalizes T101 |
| 9 | C7 | Fibonacci rank proof | Unlocks C5 |
| 10 | C26 | Resource decomposition exact | Depends on C23 |

**The shortest path to Paper 1 (Forward DKC):**

```
1.5 (catalog completeness) + 2.3 (Aizenberg relationship)
            |
            v
    [D63-D65 DONE: parity ceiling, matroid chain, algebra-independence]
            |
            v
      Paper 1 submission
```

**The shortest path to Paper 2 (11/13 theorem):**

```
1.6 (convexity proof)
      |
      v
Paper 2 submission
```

**The shortest path to Paper 3 (radical anatomy):**

```
[1.1 rad dim formula PROVEN] --> [2.1 ell=7 DONE] --> [Markov RT PROVEN]
         |
         v
  1.1 remaining (algebra isomorphism, nilpotency, Fibonacci rank)
         |
         v
  Paper 3 submission
```

**The shortest path to Paper 5 (24-Cell, S², Spectral Geometry):**

```
D66-D72, D74 (DONE)
         |
         v
1.3 (13=13 analytical proof)
1.4-NEW (XOR7 on S²)
2.7 (Cyc8 arithmetic)
         |
         v
  Paper 5 submission
```

**The shortest path to Paper 6 (Crossing Depth, Group Finiteness, Capacity Scaling):**

```
D76-D84 (DONE)
         |
         v
1.7 (ζ₁₂ Sec×Vor)
1.8 (ζ₃₂ finiteness)
1.9 (framing at ζ₁₆/ζ₂₄)
1.10 (analytical +2 proof)
2.14 (activation determines capacity — updated by D92 + D103-D105)
         |
         v
  Paper 6 submission
```

**The shortest path to Paper 7 (Mechanism and Limits: D85-D92 Arc):**

```
D85-D92 (DONE)
         |
         v
1.12 (1wpi depth law)
1.14 (balanced exponentials proof)
1.15 (regime transition)
2.18 (spectral inversion proof)
         |
         v
  Paper 7 submission
```

**The shortest path to Paper 8 (Circuit Complexity and Reservoir Computing: D93-D94):**

```
D93-D94 (DONE)
         |
         v
1.17 (RKHS kernel rank — ONE NUMBER validates five-pillar synthesis)
1.18 (higher k_sec with 2I)
1.19 (depth law under phase_cell)
         |
         v
  Paper 8 submission
```

**The shortest path to Paper 9 (Multi-strand DKC: D95-D105):**

```
D95-D105 (DONE)
         |
         v
1.21 (Ext^1 formalization)
1.22 (simple vs non-semisimple systematic framework)
1.25 (major index theorem — novel algebraic invariant)
1.26 (W_{8,2} rank saturation)
1.27 (macrame principle crossover)
         |
         v
  Paper 9 submission
```

**The shortest path to Paper 10 (Raqiya and Algebraic Diagnostics: D106-D109) — NEW:**

```
D106-D109 (DONE)
         |
         v
1.23 (additive encoding at delta=sqrt(2) — confirms polarity inversion)
1.24 (Amy Bridge formalization — connects to QCS resource theory)
2.20 (quantitative product-closure threshold)
2.21 (TL visibility at delta != 0)
         |
         v
  Paper 10 submission
```

---

## Open Questions

### Resolved by D64–D71
- ~~Z[zeta_16] parity ceiling~~: n=5, unchanged (D65). Gap-of-2 refuted; gap = 9 at Z[zeta_16].
- ~~Clifford staircase conjecture (D64)~~: PARTIALLY REFUTED (D65). Standard activations: ceiling stays at n=5 at all cyclotomic levels.
- ~~Complex k=24 as projection of 24-cell~~: CONFIRMED (D66).
- ~~ζ_8 vs ζ_16 eigenvector nesting~~: PROVEN algebraically (D69).

### Resolved by D72–D84
- ~~Is ζ₈ the only root supporting XOR8?~~: RESOLVED (D77). XOR8 solvable at ζ₈ with Sec(8)×Voronoi.
- ~~Is XOR10 solvable?~~: RESOLVED (D79). XOR10 = 124 at ζ₁₂.
- ~~Why does the 24-cell generate a finite group?~~: RESOLVED (D80). Binary octahedral group (E₇, ADE).
- ~~What is the scaling law?~~: RESOLVED (D81). Logarithmic: max_xor ≈ 0.62 × log₂(catalog) + 4.6.
- ~~Why logarithmic?~~: RESOLVED (D82). Linear depth law: max_xor ≈ depth + 6.
- ~~DKC additive or multiplicative?~~: RESOLVED (D73). Additive.
- ~~Does sampling quality explain 13 directions?~~: RESOLVED (D72). No, anti-correlated.
- ~~What is the N-2 pattern?~~: RESOLVED (D83). Jones capacity. Framing = +2.
- ~~Are bracket-null entries dead weight?~~: RESOLVED (D84). No, directionally efficient and indispensable at ζ₈.

### Resolved by D85–D92
- ~~Null indispensability universal or regime-specific?~~: RESOLVED (D87). Regime-specific.
- ~~Anti-correlation mechanism?~~: RESOLVED (D88). Two-role direction architecture.
- ~~Depth law mechanism?~~: RESOLVED (D89-D91). Axis cancellation + cross-depth algebraic constraints. Relational, not positional.
- ~~Depth law an activation artifact?~~: RESOLVED (D91). No. Parity itself is the wall.
- ~~Can DKC compute AND/OR/MAJ?~~: RESOLVED (D92). Not under +/-q. Parity-lock theorem. 1wpi computes all 13.
- ~~What is b?~~: RESOLVED (D85). b = -5/8 (per-cell). Normalization controversy dissolved.
- ~~Can b be extracted from single P_{0,0}?~~: RESOLVED (D86). No. Universal divergence. Novel negative result.

### Resolved by D93–D94
- ~~AND/OR/MAJ under 1wpi?~~: RESOLVED (D93). Yes, via phase_cell.
- ~~Circuit complexity hierarchy?~~: RESOLVED (D93-D94). AND graceful, MAJ cliff, XOR pigeonhole. Universal across groups.
- ~~E₈ (2I) reachable?~~: RESOLVED (D94). Yes, via Z[sqrt5] generators.
- ~~Non-solvability predicts capacity (Barrington)?~~: RESOLVED (D94). Yes.

### Resolved by D95–D101
- ~~Trace readout sufficient for multi-strand?~~: RESOLVED (D98). No. Catastrophically lossy (5.1%). Column/block readout needed.
- ~~Does non-semisimplicity provide computational advantage?~~: RESOLVED (D99-D101). Yes. W_{4,2} (non-semisimple) XOR14=70, W_{5,3} (simple) XOR14=0.
- ~~sl_d growth rate confirmed?~~: RESOLVED (D98-D101). Yes, ~(n-1)x at 3 data points.
- ~~Optimal z8 catalog?~~: RESOLVED (D96). A+B+C (21 entries). Q8-null = noise.

### Resolved by D102–D109
- ~~6-strand W_{6,4} decisive test (radical as resource)?~~: RESOLVED (D102). Radical carries abelian writhe character, provably useless for parity. Simple W_{6,0} wins at every XOR level. Barrington-Radical Principle named.
- ~~Does higher module dimension help?~~: RESOLVED (D103). No. BFS growth is braid group invariant. Higher dimension HURTS XOR via curse of dimensionality in sign-hash.
- ~~"XOR dies at N>=7"?~~: RESOLVED (D105). REFUTED as k-regime artifact. W_{8,0} XOR8=22266 at k=4096. k/2^N ratio is the control parameter.
- ~~Does topological entropy predict DKC capability?~~: RESOLVED (D106). No. Completely orthogonal. TL visibility filter explains why.
- ~~Is axis-alignment of braid matrices provable?~~: RESOLVED (D107). YES — Z/4Z Axis-Alignment Theorem PROVEN with constructive nesting-parity phase formula.
- ~~Can algebraic structure predict parity capability?~~: RESOLVED (D108). YES — Dual-Channel Theorem: product closure + v_2 connectivity discriminate parity/poison.
- ~~Is the dual-channel theorem encoding-independent?~~: RESOLVED (D109). NO — product closure polarity INVERTS between additive and multiplicative encodings. Encoding-Dependent Dual-Channel Theorem established.

### Still Open
- Analytical proof of the 13=13 bandwidth bound (D71/D72 — see 1.3).
- n=6 constraint wall algebraic proof: clean proof or inherently finite-verification?
- Trivialization threshold: where (50–500 cells) does topology-driven → pigeonhole-dominated? (see 2.8)
- k=38 anomaly: why is k=38 the only even k ≥ 24 with zero generalized XOR6? (see 2.9)
- XOR7 on S²: does DOF formula generalize within ζ₈ finite-group regime? (see 1.4)
- Exact Cyc8 arithmetic for generalized activation — eliminates zero-margin boundary ambiguity (see 2.7).
- OM generalization: what OM type characterizes n=4,5 parity?
- Funnel rate formula: is coverage decay exponential, algebraic, or other?
- Poison octant 4 gap (D64).
- Edge/body F4 orbit split under 24-cell self-duality (D66).
- ζ₃₂ finiteness: Power-of-Two Conjecture (see 1.8).
- ζ₁₂ Sec×Vor capacity (see 1.7).
- Depth law universality at ζ₁₀, ζ₁₆ (see 2.13).
- Formal "activation determines capacity" theorem (see 2.14, updated by D92 + D103-D105).
- Union catalog ζ₈ + ζ₁₂ superadditivity (see 2.12).
- Optimal depth composition for target XOR (see 2.11).
- Direct deep-entry generation (see 2.10).
- 6 unseparated pairs (D74): what stronger invariant separates them?
- 27-state DKC automaton: 100% determinism possible? (see 3.7)
- Is ζ₁₂ truly infinite or very large finite?
- Binocular 2.3× capacity gain root-dependence (D75).
- Info-theoretic lower bound for ζ₈ XOR ceiling.
- Framing universality at ζ₁₆ and ζ₂₄ (see 1.9).
- Analytical +2 writhe loss proof (see 1.10).
- Explicit Jordan-cell pairs at ζ₈ (see 2.15).
- ζ₁₀ null census (see 2.16).
- Resource decomposition at intermediate depths (see 2.17).
- Writhe distribution of 24-cell vertices.
- Depth-3 → 100% null generalization.
- **1wpi encoding depth law: does max_and/max_maj scale linearly? (see 1.12 — HIGH PRIORITY)**
- **Encoding design space: can encodings target specific Boolean functions? (see 1.13)**
- **Balanced exponentials analytical proof (see 1.14 — HIGH PRIORITY)**
- **Regime transition threshold (see 1.15)**
- **Spectral inversion proof (see 2.18)**
- **Robustness at ζ₁₂ (see 2.19)**
- Valuation condition failure at TL_6 (D85 open).
- Literature b method at larger lattice sizes (D85/D86 open).
- TL_8 P_{0,0} semisimplicity pattern (D86 open).
- Axis cancellation for direct weight selection (D90 open).
- Phase-sensitive activation to break parity lock (D92 open).
- Hybrid encodings (D92 open).
- **RKHS kernel rank: rank(K_2I)/rank(K_z8) > 120/24? (see 1.17 — HIGHEST PRIORITY)**
- **Higher k_sec with 2I (see 1.18)**
- **Depth law under phase_cell (see 1.19)**
- MAJ cliff mechanism: threshold or gradual? (D93 open)
- N=6-7 crossover: why does 2I overtake z12 at N=6? (D94 open)
- Size vs solvability clean disentangling (D94 open).
- Quantum dimension direct test at Fibonacci parameter (D94 open).
- Cross-depth 2I analysis (D94 open).
- **Additive encoding at delta=sqrt(2): confirms polarity inversion? (see 1.23 — NEW HIGH PRIORITY)**
- **Amy Bridge formalization: DKC dual-channel = T-gate/Hadamard? (see 1.24 — NEW HIGH PRIORITY)**
- **Major index theorem for general W_{n,j}: novel algebraic invariant (see 1.25 — NEW)**
- **W_{8,2} rank saturation at sufficient oversampling (see 1.26 — NEW)**
- **Macrame principle crossover location (see 1.27 — NEW)**
- **Quantitative product-closure threshold (see 2.20 — NEW)**
- **TL visibility at delta != 0 (see 2.21 — NEW)**
- **Sign-rank expansion theory (see 2.22 — NEW)**
- **Encoding-dependent dual-channel generalization: holds at all delta? (D108-D109 open, C53)**
- **j=0 liveness implies structural universality? (D109 open, C52)**
- Value set convergence at depth > 8: do 186 values saturate? (D107 open, see 3.8)
- 2-adic neutrality formal proof (D107 open, see 3.9).
- Universal BFS tree conjecture: identical at n=8, n=10? (D102-D103 open, see 3.11)
- Fibonacci max-abs radical constraint (D102 open, see 3.13).
- Raqiya Z[zeta_16] extension for delta!=0 investigation (D109 open, see 3.10).
