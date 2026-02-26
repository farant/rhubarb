# Vision Alignment: Current Research → Long-Term Direction

How the completed work (109 demos) positions us relative to the hybrid LLM vision
described in `planning/vision.md`. Generated 2026-02-20, updated 2026-02-26.

---

## Progress Toward Vision

The hybrid LLM vision requires: taxonomy → braids → DKC → compiled weights that
coexist with trained weights in a model that can dynamically update its factual
knowledge.

**What the 109 demos established:**

| Vision Component | Status | Key Evidence |
|-----------------|--------|--------------|
| Exact algebraic computation | PROVEN | Z[zeta_8] arithmetic, zero FP error (D29+) |
| Topology → Boolean logic | PROVEN | Forward DKC, all 13 NPN classes (D29, D50), **4-input and 5-input parity (D63), parity ceiling n=5** |
| Finite compilable weight catalog | PROVEN | 100 values at delta=0, exhaustively searchable |
| Provable correctness | PROVEN | Exhaustive verification, analytical proofs (D48, D62, D65, D71), **parity-lock theorem (D92)** |
| Gate composition / chaining | DEMONSTRATED | Cascade thresholding, 1-bit adder (D19) |
| Parity wall structure | CHARACTERIZED | Matroid minor chain + two-layer anatomy (D64, D65); absolute/convention separation |
| Quaternionic DKC | NEW CAPABILITY | S^3 lift, 24-cell geometry, 35 Voronoi solutions, F4 symmetry (D66-D68) |
| Activation function hierarchy | CHARACTERIZED | S^1->S^3->S^2->S^1xS^2->spectral; 13=13 theorem, bandwidth l=6 (D65-D71, D77) |
| Clifford hierarchy independence | PROVEN | Parity ceiling n=5 unchanged at Z[zeta_16]; ceiling does not climb the hierarchy (D65) |
| DKC as braid invariant | CHARACTERIZED | Bracket vs. cell incomparable; 119 combined classes; 6-type orbit structure (D74) |
| Binocular channel | NEW CAPABILITY | Angle subsumes cell (2.3x capacity); 7 discrete angles for 3-term sums (D75) |
| Scaling beyond 6 inputs | DEMONSTRATED | XOR8 via S^1xS^2 activation (D77); zeta_12 reaches XOR10/12 (D79) |
| Algebraic group structure | CHARACTERIZED | ADE classification; only zeta_4 and zeta_8 finite; infinite groups break the ceiling (D80) |
| Capacity scaling law | QUANTIFIED | Logarithmic law: max_xor ~ 0.62*log2(catalog)+4.6 (D81) |
| Depth = crossing number | PROVEN | Linear depth law: max_xor ~ depth+6; algebraic coherence dominates vocabulary (D82) |
| TL non-semisimplicity quantified | NEW | Indecomposability parameter b = -5/8 confirmed from first principles (D85-D86) |
| Depth law mechanism | EXPLAINED | Axis cancellation + cross-depth algebraic constraint; relational, not positional (D89-D91) |
| Encoding determines computability | PROVEN | Parity-lock theorem: +/-q encoding computes only XOR/XNOR (D92) |
| Regime transition (finite/infinite) | CHARACTERIZED | Null indispensability is finite-group-specific; dispensable at zeta_12 (D87) |
| Perturbation resilience | CHARACTERIZED | K-ladder 14x more robust than Voronoi-only; constrained optimization viable (D88) |
| Catalog optimization / pruning | PROVEN | Derived series cross-classification; A+B+C (21 entries) beats ALL (24); Q8-null = noise (D95-D97) |
| Multi-strand DKC | NEW CAPABILITY | 3-strand XOR14=60 (D99 LANDMARK), 4-strand XOR14=70 (D100), 5-strand XOR12=512 (D101) |
| Non-semisimple extension = resource | PROVEN | W_{4,2} XOR14=70 (radical); W_{5,3} XOR14=0 (simple) — Ext^1 is load-bearing (D99-D101) |
| sl_d functor growth rate | CONFIRMED | BFS growth ~(n-1)x: 2x (3-strand), 3.1x (4-strand), 4x (5-strand), **5x (6-strand, D102-D103)** (D98-D105) |
| Radical-as-resource hypothesis | REFUTED | Simple module beats non-simple at matched dim; radical carries abelian writhe character, provably useless for parity by Barrington (D102) |
| BFS growth is braid group invariant | PROVEN | All modules of same strand count produce identical BFS trees — growth tracks B_n, not module (D102, D103) |
| Sign-hash activation theory | CHARACTERIZED | Curse of dimensionality in sign-hash; Atkinson sweet spot; cross-block branching decomposition; k is the real lever (D103, D104) |
| Multi-strand scaling to n=8 | DEMONSTRATED | W_{8,0} (dim=14), W_{8,2} (dim=28); XOR8=22266 at k=4096; macrame principle regime transition (D105) |
| Topological entropy orthogonality | PROVEN | Entropy classification totally orthogonal to DKC computation; TL visibility filter explains why (D106) |
| Axis-alignment theorem | PROVEN | Z/4Z phase formula: every TL braid matrix entry is axis-aligned at delta=0; constructive nesting-parity proof (D107) |
| Graph-structure predicts parity | DEMONSTRATED | Dual-channel theorem: product closure + v_2 connectivity discriminate parity/poison vocabularies (D108) |
| Amy bridge to quantum circuits | CONJECTURED | DKC dual-channel maps onto T-gate/Hadamard decomposition in quantum circuit synthesis (D108) |
| Encoding-dependent dual-channel | PROVEN | Product closure polarity inverts between additive (D108) and multiplicative (D109) encodings (D108-D109) |
| Raqiya as reusable analysis tool | OPERATIONAL | Single-header C89 library for Z[zeta_8] algebraic analysis, deployed across D107-D109, 208+ unit tests |
| Taxonomy → braid encoding | NOT STARTED | The critical missing link |
| Multi-layer complex composition | NOT TESTED | Only bit-level cascade (D19) exists |
| Integration with trained weights | NOT STARTED | Architecture question, unexplored |
| Dynamic recompilation | NOT TESTED | Should be fast given finite catalog, unverified |

**Assessment:** The "knots → logic" half of the pipeline is now extremely well
characterized, mechanistically understood, generalized to multi-strand braid
representations up to n=8, AND equipped with algebraic diagnostic tools (Raqiya)
that can predict computational capability from graph structure. The D85-92 arc
shifted the research from "what can DKC do?" to "why does it work and what are
its structural limits?" The D95-97 arc resolved the catalog optimization question
through derived-series cross-classification. The D98-101 arc opened multi-strand
DKC on matrix representations of TL algebras. The D102-109 arc represents a
maturation phase: scaling multi-strand DKC to n=8 (D105), decisively refuting
the radical-as-resource hypothesis (D102), establishing the activation/sign-hash
as the critical interface (D103-D104), proving topological entropy is orthogonal
to computation (D106), proving axis-alignment as a theorem with constructive
phase formula (D107), and building the Raqiya toolkit into a reusable algebraic
diagnostic that predicts parity capability from graph structure (D108-D109). The
D102-109 arc also produced the Barrington bridge — the first connection between
DKC and classical computational complexity theory — and the Amy bridge to quantum
circuit synthesis. The parity-lock theorem (D92) and the encoding design question
remain the primary theoretical gaps for non-parity functions. The "taxonomy →
knots" half remains the primary practical gap. The encoding-dependent dual-channel
theorem (D108-D109) is the first result that characterizes what makes a value
vocabulary computationally capable, moving from "search and find" to
"predict and verify."

---

## Progress Toward Vision (D72-82 Additions)

### The scaling arc (D76-82) answers "can DKC scale?"

The question left open after D65-D71 was whether zeta_8 represented a fundamental
ceiling or an instance-specific one. D76-82 resolve this:

- **D76**: XOR8 = 0 at zeta_8 with S^2 activation — appeared to be a hard ceiling
- **D77**: XOR8 = 6 at zeta_8 with S^1xS^2 activation — the wall was the activation,
  not the root of unity (third instance of the "wall was X not Y" pattern)
- **D78**: XOR10 = 0 at zeta_8 exhaustively — the zeta_8 ceiling is real, at n=8
- **D79**: XOR10 = 124 at zeta_12, XOR12 = 50+ — infinite groups break the ceiling
- **D80**: Only zeta_4 and zeta_8 are finite; all other tested roots are infinite
- **D81**: max_xor ~ 0.62*log2(catalog)+4.6 — logarithmic cost per XOR level
- **D82**: max_xor ~ depth+6 — linear in crossing depth; the logarithmic law is
  a corollary of exponential catalog growth per depth level

For the hybrid LLM vision this means compiled DKC neurons can compute arbitrarily
large Boolean functions — at exponential cost. The 3-input syllogism case (well
within zeta_8's n=8 ceiling) remains economical. Larger compiled units are feasible
but expensive, which naturally drives a compositional architecture.

---

## Progress Toward Vision (D85-92 Additions)

### The mechanistic arc (D85-92) answers "why does DKC work and what are its limits?"

The D85-92 arc is qualitatively different from previous arcs. Where D72-82 asked
"how far can DKC scale?", D85-92 asks "what is the underlying mechanism, and what
structural constraints does it impose?" The answers reshape the vision.

- **D85**: Indecomposability parameter b = -5/8 computed from first principles at
  TL_4 via leading-coefficient extraction from delta-parameterized Markov trace.
  The literature "controversy" (b = -2 vs b = -5/8) dissolved: different
  normalization conventions on the same underlying structure (regular rep vs
  per-cell). This is the first quantitative confirmation of the DKC ↔ LCFT
  correspondence — the non-semisimplicity that makes DKC compute is precisely
  the c = -2 dense polymer LCFT.

- **D86**: The delta-parameterized Gram matrix approach universally DIVERGES on
  a single projective cover P_{0,0} at every eigenvalue and lattice size. The
  multiplicity from the regular representation is structurally essential, not
  just a scale factor. This is a novel negative finding — nobody in the
  literature has tried this specific approach. It establishes that b is a
  collective property of the full TL algebra, not extractable from a single
  indecomposable module.

- **D87**: Null indispensability is regime-dependent. At zeta_8 (finite group),
  removing bracket-null entries destroys XOR8 capacity (Demo 84). At zeta_12
  (infinite group, 4096 entries), removing all 121 nulls preserves full
  XOR6/8/10/12 capacity. The transition mechanism is direction density: when
  non-null directions are dense enough on S^2, losing null-only directions
  cannot constrain combinatorial search. Null fraction dilutes: zeta_4 = 75%,
  zeta_8 = 37.5%, zeta_12 = 3.0%.

- **D88**: The anti-correlation mechanism is explained. The 13 zeta_8 directions
  form a cuboctahedron with three computational roles: 4 non-null body diagonals
  are rigid computational anchors (perturbation-sensitive), 6 null-only edge
  midpoints are flexible topological scaffolding (movable with only 2.8% XOR
  loss), 3 shared coordinate axes are intermediate. The k-ladder activation
  provides 14x better perturbation resilience than Voronoi-only (6.4% vs 89%
  loss under the same design-improving gradient).

- **D89**: Systematic investigation of WHY the depth law holds. Tested 8
  mechanistic hypotheses, killed 5 (pairwise coherence, direction coverage,
  Cayley density, sector diversity, angle coherence). Confirmed paired extension
  is real but minority at zeta_12 (6-20% of winners vs 100% at zeta_8). The
  mechanism narrowed to "algebraic coherence" — attacked in D90.

- **D90**: The depth law mechanism is axis cancellation (anti-aligned rotation
  axes producing low sum angles) combined with cross-depth algebraic constraints
  (BFS ancestry creating limited sum-angle vocabularies between related entries).
  The key finding: the advantage is RELATIONAL, not positional. The S^2 point
  cloud bandwidth actually DECREASES with depth (spectral inversion) — ruling
  out all positional/bandwidth explanations. More complex knots produce entries
  whose pairwise relationships create the fine-grained sector separations needed
  for parity, not entries that individually sit in better positions.

- **D91**: The combined_cell activation is NOT the bottleneck. The parity
  constraint itself is the wall. The depth gap (XOR6 to XOR8) is invariant
  across all angular resolutions (k_sec 2-48) and all Voronoi direction counts
  (2-114). This confirms the balanced exponentials explanation: BFS vocabulary
  grows ~2x/depth round, parity demand grows 4x/weight, yielding the linear
  slope of ~2 depth per weight.

- **D92**: The +/-q input encoding is structurally parity-locked. AND/OR/MAJ/
  THRESHOLD = 0 winners at ALL depths, ALL resolutions. The mechanism: masks
  0...0 and 1...1 always produce identical quaternion sums (both zero), so any
  function where f(0...0) != f(1...1) is impossible under ANY activation. XOR
  survives because XOR(0...0) = XOR(1...1) = 0 for even-length inputs. This is
  the first formally PROVED result in the program — a 6-part theorem with
  computational verification across the full zeta_12 catalog.

For the hybrid LLM vision this arc has three major implications:

1. **The encoding is more fundamental than the activation or catalog.** D92 proves
   that the +/-q encoding determines which Boolean functions are computable,
   regardless of activation resolution, catalog depth, or root of unity. This
   means the encoding design is the first architectural decision — it constrains
   everything downstream.

2. **The depth law is mechanistically understood.** D89-D91 close the explanatory
   gap: the mechanism is relational (axis cancellation + cross-depth constraints),
   not positional. For catalog engineering this means optimizing individual entry
   positions is wrong (spectral inversion proves this) — optimizing pairwise
   relationships between entries is the correct approach.

3. **Regime-dependent design.** D87 establishes that design principles differ
   between finite groups (zeta_8: nulls critical, sparse directions, shadow
   pairing dominant) and infinite groups (zeta_12: nulls dispensable, dense
   directions, diversity dominant). A practical hybrid LLM using both regimes
   would need regime-aware compilation strategies.

---

## Progress Toward Vision (D95-101 Additions)

### The commutator/synergy arc (D95-97) answers "how should the catalog be structured?"

The D95-97 arc resolves the catalog optimization question left open by D84 (null
synergy) and D88 (anti-correlation). The key insight: the derived series of the
binary octahedral group provides an independent stratification that, combined
with D84's null/non-null partition, yields a complete 5-cell landscape for
understanding which catalog entries matter and why.

- **D95**: The derived series 24 > 12 > 4 > 1 (2O > 2T > Q8 > {±1}) is computed
  for the first time in the DKC context. Neither the commutator subgroup (COMM)
  nor the non-commutator coset (NON-COMM) carries disproportionate capacity in
  isolation — capacity emerges from CROSS-LAYER interaction. The 90-degree split
  test is the cleanest synergy demonstration: Q8-null (3 entries) and outermost-null
  (6 entries) each give zero XOR, but their union gives 26% — pure 0+0>0 at
  matched geometry. Algebra wins over geometry.

- **D96**: The D84 null/non-null and D95 derived series partitions are INDEPENDENT
  — their cross-product creates a 5-cell landscape (A through E). The optimal
  catalog is A+B+C (21 entries) which BEATS the full 24-entry catalog (ALL) at
  every N from 3-7. The 3 Q8-null entries (Cell E) are pure noise — removing them
  improves performance. This is the first principled catalog pruning result:
  algebraic structure tells you which entries to keep.

- **D97**: Cell B (the 6 non-null non-commutator entries) is geometrically
  PERFECT — an octahedron on S^2 with 3 orthogonal direction pairs. It achieves
  a 50-degree plateau at N=3 (matching its intrinsic angular resolution) and
  exhibits a lever arm mechanism: nonzero real quaternion component enables
  phase_cell discrimination that pure-imaginary entries cannot achieve. Cell B
  is the geometric core of z8's computational power.

For the hybrid LLM vision this arc has two major implications:

1. **Catalog design is now principled, not empirical.** The derived-series
   cross-classification gives a complete map of which catalog entries are useful
   (Cells A+B+C), which are noise (Cell E), and why. This replaces trial-and-error
   pruning with algebraically-motivated selection.

2. **Less can be more.** The 21-entry pruned catalog outperforms the 24-entry
   full catalog — counterintuitive but explained by the noise-removal mechanism.
   For deployment, this means compiled DKC neurons can use smaller catalogs
   without sacrificing capacity, provided the pruning is algebraically informed.

### The multi-strand arc (D98-101) answers "can DKC generalize beyond 2-strand braids?"

The D98-101 arc is the most significant conceptual expansion since D66-D68
(quaternionic DKC). It moves from the scalar Kauffman bracket (2-strand, TL_2)
to matrix braid representations (3-, 4-, 5-strand TL algebras), opening an
entirely new design dimension for compiled DKC neurons.

- **D98**: The 3-strand braid group B_3 acting on the full 5x5 representation
  of TL_3 at delta=0 generates an infinite group (BFS capped at 2048). The trace
  readout retains only 5.1% of catalog entries (105/2048) and achieves ZERO XOR
  at all levels — establishing that trace is an information bottleneck for matrix
  DKC. The column-4 readout retains 100% but the result seeded the search for
  better readout strategies. BFS growth ~2x per round, matching the 2-generator
  (n-1=2) prediction.

- **D99 (LANDMARK)**: The Delta_1 standard module (2x2 indecomposable over
  Z[zeta_8], non-split extension Ext^1(L(0),L(1))) achieves the FIRST 3-strand
  XOR computation: XOR6=500K+, XOR8=48K+, XOR10=485, XOR12=16, XOR14=60. The
  key innovations: (a) working on the irreducible 2x2 module instead of the
  reducible 5x5, (b) a 16-component sign-pattern activation (2x2_hash). Entry
  magnitudes follow exact Fibonacci growth (Jordan block structure). Two regimes
  emerge: combinatorial (XOR6-8, hundreds of thousands of solutions, saturate
  by ~4096 entries) and algebraic (XOR10-14, sparse, require deep entries d>=6).
  The Ext^1 non-split extension is hypothesized to be the computational resource
  — catalytic preparation enabling interference.

- **D100**: The 4-strand braid group B_4 on W_{4,2} (3x3, non-semisimple,
  Loewy length 3) produces BFS growth ~3.1x per round (matching n-1=3
  prediction), sustained XOR capacity through depth, and XOR14=70 on the deep
  sub-catalog. Radical content is CONSTANT across all depths (mean=2) — every
  entry is equally non-semisimple. Casimir correlates positively with XOR success
  (1.36x-1.86x). Six super-hubs form 3 cyclotomic pairs; all hub commutators
  are traceless with zero radical content.

- **D101**: The 5-strand B_5 on W_{5,3} (4x4, SIMPLE, no radical) confirms the
  growth-rate prediction (~4x for n-1=4) but FALSIFIES the hub-count prediction
  (3 hubs, not 12). Critically, XOR14=0 everywhere — in stark contrast to D100's
  70. The Casimir-XOR correlation INVERTS at high XOR levels (winners have LOWER
  Casimir in the simple module). Hub commutators are all traceless; two hubs
  actually commute (zero matrix). The simple module computes differently: the
  non-semisimple extension in D100 is load-bearing, not decorative.

For the hybrid LLM vision this arc has four major implications:

1. **A new architectural dimension.** Multi-strand DKC adds strand count and
   module type (simple vs non-semisimple) as design parameters alongside encoding,
   activation, catalog, and root of unity. A compiled DKC layer can now choose
   not just WHICH algebra (zeta_8, zeta_12, 2I) but HOW MANY strands and WHICH
   module to represent on.

2. **Non-semisimplicity is the computational resource.** The D100 vs D101
   comparison is definitive: matched methodology, W_{4,2} (non-semisimple) gets
   XOR14=70, W_{5,3} (simple) gets XOR14=0. The Ext^1 non-split extension
   provides computational degrees of freedom that simple modules lack. This
   connects directly to D84-D86: the non-semisimplicity that makes 2-strand DKC
   compute (b = -5/8) is the SAME structural feature that makes multi-strand DKC
   compute at the boundary.

3. **The growth rate is predictable.** The sl_d functor thesis — BFS growth tracks
   n-1 — is confirmed across three data points (2x, 3.1x, 4x). This gives a
   concrete cost model: higher strand count means exponentially faster catalog
   growth (larger catalogs per depth level) but potentially different capacity
   characteristics. The hub structure does NOT scale as simply as predicted.

4. **Activation design for matrices is open.** The 2x2_hash, 3x3_hash, and
   4x4_hash activations used in D99-D101 are crude sign-pattern hashes. Better
   activations (analogous to the Sec x Vor k-ladder for quaternions) could
   dramatically improve multi-strand DKC performance. This is the multi-strand
   analog of the D50 activation wall.

---

## Progress Toward Vision (D102-109 Additions)

### The multi-strand maturation arc (D102-105) answers "how does multi-strand DKC scale and what controls its capacity?"

The D102-105 arc takes the multi-strand capability established by D98-101 and
systematically characterizes its scaling behavior, activation theory, and the
role of module structure. It produces four major structural results and one
decisive refutation.

- **D102**: Head-to-head A/B test of W_{6,0} (simple, dim=5) vs W_{6,4}
  (non-simple, dim=5, radical dim=1) on the 6-strand braid group. The radical
  carries an abelian writhe character (M*r = A^{writhe}*r for ALL 32,768 catalog
  entries), which by Barrington's theorem is provably useless for parity/XOR
  computation. The simple module WINS at every XOR level. This is the
  **Barrington-Radical Principle** — the first connection between DKC and
  classical computational complexity theory. It does NOT contradict the D100-D101
  finding that non-semisimplicity is load-bearing; rather, it sharpens the claim:
  the radical is computationally inert (abelian writhe), but the quotient
  structure of non-semisimple modules can still provide advantage through the
  mixing row (8% boost at XOR6). The growth rate at n=6 is ~5x, extending the
  sl_d functor thesis to a fourth data point.

- **D103**: Pure dimension scaling test — W_{6,0} (dim=5) vs W_{6,2} (dim=9),
  both simple. BFS growth profiles are PERFECTLY IDENTICAL at every depth: growth
  is a braid group invariant (B_6 property), not a module dimension property.
  Higher dimension HURTS XOR at every level due to curse of dimensionality in the
  sign-hash activation. However, the subset-hash experiment (Phase 7) reveals that
  the deficit is partly an activation artifact: B@sub7 (196 components from dim-9)
  beats A@full (100 components from dim-5). This establishes that activation
  geometry — WHICH components are hashed, not just how many — is a critical design
  parameter. Also connects to 1-bit compressed sensing (Boufounos-Baraniuk 2008).

- **D104**: Systematic activation coarseness sweep on W_{6,0} vs W_{6,2}. The
  dim-9 module's XOR deficit is a HASH problem, not an algebra problem: at
  optimal component count (120 of 324), W_{6,2} beats W_{6,0}. The key findings:
  (a) the Atkinson sweet spot — non-monotonic component-count curve peaking at
  ~120, (b) sign-rank EXPANSION — sign quantization breaks 48 linear
  dependencies, creating new independent directions (raw rank 244 → sign-rank
  292), (c) branching cross-block components carry the computational signal (off-
  diagonal beats diagonal), (d) k (cell count) is the real lever, not hash
  architecture — multi-hash does NOT beat single-hash, and the cross-block
  advantage at k=128 vanishes at k=4096 where W_{6,0} wins at every k.

- **D105**: Scaling to 8-strand braid groups. W_{8,0} (dim=14) and W_{8,2}
  (dim=28) demonstrate that D93's "XOR dies at N>=7" was a k-regime artifact —
  at k=4096, W_{8,0} produces XOR8=22266 (beating n=6's 21699). The k/2^N ratio
  is the real control parameter. The **macrame principle** regime transition:
  branching interaction reverses at n=8 (within-block > cross-block), opposite to
  D104's n=6 result, because complex blocks (dim=14) are self-sufficient while
  simple blocks (dim=5) need connections. Rank saturation analysis reveals all
  prior cross-module rank comparisons were confounded by under-sampling. W_{8,0}
  sign-rank reaches 97.8% at saturation; W_{8,2} sign expansion is 1.83x and
  still growing.

For the hybrid LLM vision this arc has five major implications:

1. **The Barrington bridge connects DKC to computational complexity theory.**
   The radical's abelian character is provably useless for parity by Barrington's
   theorem (1989). This is the first time DKC has been grounded in classical CS
   theory — providing external validation that DKC's computational structure is
   not an artifact but reflects genuine complexity-theoretic constraints.

2. **Activation design is now the primary engineering challenge for multi-strand
   DKC.** D103-D104 establish that the same algebraic catalog can produce
   dramatically different XOR capacities depending on which components are hashed
   and how. The Atkinson sweet spot, branching decomposition, and sign-rank
   expansion are concrete tools for principled activation engineering.

3. **Multi-strand DKC scales to n=8.** The "XOR dies at N>=7" claim (D93) is
   refuted as a k-regime artifact. At sufficient k, 8-strand DKC achieves XOR8
   comparable to or exceeding 6-strand, confirming that the activation resolution
   (not the strand count) is the scaling bottleneck.

4. **BFS growth is a braid group invariant.** All modules of the same strand
   count produce identical BFS catalogs (proven for all three n=6 modules:
   W_{6,0}, W_{6,2}, W_{6,4}). This means the catalog is a property of B_n, not
   of the chosen module. Module choice affects readout (activation), not the
   reservoir.

5. **Rank saturation is methodologically essential.** D105's retroactive analysis
   shows that single-checkpoint rank measurements are unreliable for infinite
   braid groups. All prior cross-module rank comparisons (D99-D104) are confounded
   to varying degrees. Future multi-strand work must report saturation curves.

### The relational computation arc (D106-109) answers "what predicts computational capability?"

The D106-109 arc shifts from measuring DKC capacity (how many XOR solutions?) to
predicting it (what algebraic structure distinguishes capable from incapable value
sets?). This is the most theory-forward arc since D85-92.

- **D106**: Topological entropy (Thurston-Nielsen classification) is completely
  orthogonal to DKC Boolean computation — both periodic and pseudo-Anosov braids
  compute XOR at 100% participation with near-identical scores. This is a clean
  negative result explained by the TL visibility filter: properties that survive
  the TL quotient (writhe) correlate with DKC output; properties killed by the
  quotient (entropy, Burau spectral radius) do not. The result strengthens the
  relational computation thesis: individual dynamical properties are consistently
  non-predictive; only relational/algebraic properties matter.

- **D107**: The most theorem-rich demo in the program. Universal axis-alignment
  of braid matrices at delta=0 is proved (not just observed) with a constructive
  Z/4Z phase formula based on nesting parity. Five novel results: nesting parity
  = Z/2Z grading (confirmed NOT a consequence of known KLR grading), major index
  bipartite invariant, q-Catalan identity C_{2m+1}(-1) = (-1)^m * C_m,
  orthogonality theorem (product quotient = K_5 = Z/4Z rotation, additive
  quotient = star tree), and 2-adic neutrality of nilpotent TL. The DKC
  implication: computation at delta=0 reduces to integer path counting + a
  three-variable phase formula — effective computation is Z x Z/4Z per matrix
  entry.

- **D108**: Graph structure PREDICTS parity capability. The 41-value parity
  vocabulary and 17-value poison vocabulary show maximum structural contrast on
  product closure (connected P_3 vs empty graph) and near-maximum contrast on
  additive v_2 connectivity (83% vs trivial K_2). This is the **Dual-Channel
  Theorem**: parity requires BOTH multiplicative phase coherence (product closure
  > 0) AND additive magnitude diversity (v_2 connectivity > trivial). Poison
  fails on BOTH. The theorem maps onto T-gate/Hadamard decomposition in quantum
  circuit synthesis (Amy, Glaudell & Ross 2023) — the **Amy bridge**. Partition
  coincidence (root = Galois = norm) is identified as a structural signature of
  computational incapability.

- **D109**: At delta=sqrt(2) with multiplicative encoding, Raqiya graph analysis
  CANNOT discriminate parity from non-parity: 7 of 8 edge types are identical
  between the two vocabularies. Product closure is the sole discriminator — and
  its polarity is INVERTED from D108: parity has LOW product closure (14.8%) vs
  non-parity (46.6%). This establishes the **Encoding-Dependent Dual-Channel
  Theorem**: under additive encoding, parity wants HIGH product closure (self-
  reinforcement through superposition); under multiplicative encoding, parity
  wants LOW product closure (products must escape vocabulary for sector
  alternation). The rich algebra at delta=sqrt(2) (j=0 sector alive, all
  structural prerequisites satisfied) means Re>0 activation suffices — Raqiya
  diagnoses algebraic impoverishment, not algebraic health.

For the hybrid LLM vision this arc has four major implications:

1. **Predictive capability analysis is now possible.** D108-D109 establish that
   algebraic graph structure (product closure, v_2 connectivity, partition
   hierarchy) can predict which value vocabularies support parity computation.
   This moves from brute-force search ("try all triples") to structural
   prediction ("analyze the value set, predict capability"). For the compilation
   pipeline, this means a candidate catalog can be evaluated BEFORE the expensive
   exhaustive XOR search.

2. **The Amy bridge connects DKC to quantum circuit synthesis.** The dual-channel
   theorem's mapping onto T-gate/Hadamard decomposition means DKC's algebraic
   structure is not an isolated construction but connects to the mature theory of
   exact synthesis over Z[zeta_8] (Amy 2023). This provides both theoretical
   validation and practical tools: the sde (smallest denominator exponent)
   resource measure from quantum synthesis may be directly applicable to DKC
   catalog optimization.

3. **Raqiya is a lasting reusable tool.** The single-header C89 library for
   Z[zeta_8] algebraic analysis — with 9+ relationship detectors, graph analysis
   layer, quotient graph computation, and 208+ unit tests — embodies the vision
   of building tools that last. It was deployed across D107-D109 with zero
   regression and continues to grow. This is the kind of library infrastructure
   the long-term vision requires: exact, tested, reusable.

4. **The TL visibility filter is a selection principle for the compilation
   pipeline.** D106 proves that only properties surviving the TL quotient matter
   for DKC computation. For catalog engineering this is a concrete filter: any
   property killed by e_i^2=0 (entropy, spectral radius, Burau dynamics) is
   irrelevant. Only writhe-like (abelian character) and algebraic-structure
   properties (axis alignment, product closure, nesting parity) survive.

---

## Opportunities Opened by Recent Work

### The 11/13 theorem opens activation design space
Demos 61-62 proved the half-plane wall is geometric, not algebraic. This means
the activation function is a design parameter, not a constraint. For the hybrid
LLM vision, this suggests compiled DKC layers could use purpose-built activation
functions (k-sector MVN, or something novel) rather than being forced into
standard sigmoid/ReLU.

### The axiality property simplifies hardware
At delta=0, bracket values are axial — only 1 of 4 Z[zeta_8] coordinates is nonzero.
This means each compiled weight is effectively a single integer x a phase
direction. For hardware implementation, this is a massive simplification:
8 possible directions x integer magnitude = a very compact representation.

### Cross-l universality suggests robustness
The 11/13 wall holds across five different cyclotomic rings (l=2-6), and Demo 60
confirmed all radical structure formulas at l=7 (first cubic number field). This
ring-independence — now tested across quadratic AND cubic extensions — means the
compiled-weight approach isn't fragile. Good for engineering.

### The Fibonacci rank theorem connects to capacity
F(l-1) as the radical's bilinear rank means there's a computable measure of
how much "deductive capacity" each evaluation level provides. Higher l = more
capacity but more complex ring. This could inform choosing the right l for
a given taxonomy size.

### The parity ceiling establishes finite capacity per neuron
Demo 63 proves Z[zeta_8] parity tops out at n=5 inputs for the S^2 activation.
D77 extends this to n=8 with S^1xS^2 activation. D78 confirms zeta_8 walls at n=8
regardless of activation. For the hybrid LLM vision, compiled DKC neurons using
zeta_8 handle up to 8-input Boolean functions; larger operations must be compositional.
The ceiling is a feature: it bounds per-unit complexity, making the system
predictable.

### Formal proofs strengthen the theoretical foundation
Three formal proofs (radical dimension, next-level radical, Markov RT truncation)
move key results from "computationally verified" to "proven theorem." The
parity-lock theorem (D92) is the fourth and most architecturally significant
formal proof: it is the first result that constrains the design space (which
functions are computable at all), not just characterizes capacity (how many
inputs). For the vision's credibility — especially in academic and engineering
contexts — having rigorous proofs of the algebraic underpinnings is essential.

### Parity wall has a matroid structure (D64)
The {XOR2, XOR3, XOR4, XOR5} hierarchy is a deletion-contraction closed matroid
minor chain. Deletion of any weight always yields a parity solution one level down;
contraction always yields an XNOR solution one level down. The wall at n=6 casts
a funnel shadow downward (coverage collapses 99.3% -> 39.2% -> 19.2% -> 0%). This
gives the wall a precise algebraic character — it is not a vague obstruction but
a structured geometric boundary with the self-doubling impossibility theorem
accounting for 89.5% of the closest misses. For the vision this means the
parity ceiling is a well-understood finite capacity, not a mystery.

### Quaternionic DKC opens a new geometry (D66-D68)
Lifting from S^1 (complex trace) to S^3 (full unit quaternion) reveals the 24-cell
polytope as the natural computational geometry. The same braids that produce 100
bracket values produce exactly 24 quaternion vertices — the 24-cell's binary
octahedral group. A 24-cell Voronoi partition (25 cells) finds 35 XOR6 solutions.
For the hybrid LLM vision this means compiled DKC weights have a richer 4D
geometry than previously known, and the bracket and quaternion are complementary
hash functions that resolve each other's collisions. This could enable a
two-channel compiled weight representation.

### S^2 is the computational sweet spot (D67-D69)
The rotation-axis direction (eigenvector S^2) carries all DKC computational
information. The Hopf fiber phase is provably inert — zero solutions at every
resolution. A custom 13-direction Voronoi on S^2 achieves 36 solutions with only
14 cells, outperforming both the full 25-cell S^3 Voronoi and any Platonic-solid
partition. The progression D65->D66->D67 (S^1 25 cells, S^3 25 cells, S^2 14 cells)
shows that the correct 2D projection is more efficient than the ambient 3D or 4D
space. For hardware implementation, this means the compiled weights live on a
sphere in 3D — a more practical computational geometry than the full quaternion.

### The 13=13 theorem gives a spectral characterization (D71)
The minimum bandwidth for XOR6 DKC on S^2 is exactly l=6 because 2*6+1=13 equals
the number of eigenvector directions. The phase transition is sharp and all-or-
nothing. All 36 XOR6 winners share the same spectral envelope (l=6 dominant at
~80%), making the spectrum a structural invariant — a fingerprint of the
computation itself. For the vision this is potentially the cleanest theoretical
result in the program: it connects the topology (13 NPN classes = 13 directions =
l=6 bandwidth) in a single equation. It also explains the trivialization effect:
finer roots require enormous bandwidth, making coarse partitions trivially succeed
(~90% at zeta_16).

### Music connection opens artistic/outreach dimension (D70)
The zeta_8 algebra produces exactly 4 musical notes (C, Eb, E, F#) — consonance to
tritone — and Yang-Baxter equivalent braids produce literally identical melodies.
The Garside element (s_1 s_2)^3 yields the arch motif Eb E F# E Eb C. This is
primarily interesting as outreach: the mathematics has an intrinsic musical
structure that did not have to exist. For the long-term vision of the project it
is a natural interface between knot theory and human perceptual experience.

### DKC computation is algebraic, not geometric (D72)
The anti-correlation between spherical design quality and XOR6 solution count
(better geometric distribution -> fewer solutions: 36 vs 4) establishes that DKC
is algebraically selected. The 13 eigenvector directions are simultaneously the
worst choice for spherical integration (t=0) and the best for DKC computation.
For the hybrid LLM vision this confirms that compiled DKC weights should be
chosen by algebraic criteria (group theory) rather than geometric criteria
(uniform distribution). This simplifies the compilation algorithm: no optimization
over a continuous geometric space is needed — the algebra selects the right points.

### Bracket and cell are incomparable invariants (D74)
The Kauffman bracket (eigenvalue, S^1) and the DKC Voronoi cell (eigenvector, S^2)
capture orthogonal braid information. Their combination gives 119 equivalence
classes vs 100 bracket-alone or 14 cell-alone — a 19% improvement. For the hybrid
LLM vision, this means a compiled DKC layer can be read in two complementary ways,
and their joint information is strictly richer than either alone. The incomparability
result explains why S^1xS^2 product activation (D77) is necessary for XOR8: each
channel contributes independent computational power.

### S^1xS^2 activation is the quaternionic MVN (D77)
The combined eigenvalue-angle (S^1) x eigenvector-direction (S^2) activation is
the natural generalization of Aizenberg's complex MVN to quaternions. It unlocks
XOR8 at zeta_8 — demonstrating that the zeta_8 algebraic structure supports at least
8-input computation when both channels are read. For the vision this establishes
a principled activation design: always read both Hopf components (fiber and base).

### Infinite groups provide unbounded capacity (D79, D80)
zeta_12 generates an infinite (or very large) SU(2) subgroup; its catalog grows
without bound and reaches XOR10 (124 winners), XOR12 (50+ winners). The ADE
survey (D80) shows only zeta_4 and zeta_8 are finite, and all other tested roots are
infinite. For the hybrid LLM vision this is a critical result: if per-neuron
capacity needs to exceed 8 inputs, switching from zeta_8 to any infinite-group root
(zeta_12, zeta_10, etc.) removes the ceiling — at the cost of a larger, more expensive
catalog.

### Crossing depth = computational power (D82)
The linear depth law max_xor ~ depth+6 establishes that the knot-theoretic
crossing number directly governs computational power. A depth-d quaternion
encodes d crossings of the associated knot; more complex knots compute more.
This is the most direct connection yet between knot theory and computational
capacity. For the hybrid LLM vision, it suggests that the "compile a syllogism"
step corresponds to finding a braid of sufficient crossing depth — and that
crossing depth is a computable, predictable measure of compiled weight complexity.

### Algebraic coherence beats vocabulary (D82)
Deep subsets (entries from late closure rounds) outperform wide subsets (maximum
direction/angle coverage) despite having less vocabulary. The determining factor
is shared algebraic structure — deep entries are products of 8+ generators and
share common intermediate factors. For the vision this means the compilation
algorithm should prioritize entries that are algebraically related to each other,
not just maximally diverse. This could significantly reduce the catalog size
needed for practical deployment.

### The indecomposability parameter quantifies non-semisimplicity (D85-D86)
b = -5/8 computed from first principles at TL_4 — the first quantitative extraction
of the LCFT indecomposability parameter from our algebraic framework. The literature
controversy (b = -2 vs b = -5/8) dissolved: different normalization conventions on
the same structure (regular rep b = -5/2, divided by dim(0-TL sector) = 4, gives
per-cell b = -5/8). D86's negative result (divergence on single P_{0,0}) establishes
that b is a collective property requiring the full regular representation's
multiplicity structure. For the hybrid LLM vision this matters because it quantifies
the non-semisimple structure that D84 identified as THE computational resource —
b is a concrete number characterizing the strength of the Jordan-cell coupling that
makes DKC compute.

### The depth law is fully explained (D89-D91)
Three demos systematically identified and confirmed the mechanism behind
max_xor ~ depth+6. D89 killed 5 of 8 candidate mechanisms (pairwise coherence,
direction coverage, Cayley density, sector saturation, angle coherence for deep
shells). D90 found the mechanism: axis cancellation (anti-aligned rotation axes
producing low sum angles that create fine-grained sector separations) combined with
cross-depth algebraic constraints (BFS ancestry restricting pairwise sum-angle
vocabularies to 73 distinct values from 50K gen x deep pairs). The key insight:
the advantage is RELATIONAL, not positional — the S^2 point cloud bandwidth
actually DECREASES with depth (spectral inversion). D91 confirmed the wall is
parity itself, not the activation: the depth gap is invariant across all
resolutions. The balanced exponentials explanation (BFS ~2x/round vs parity
4x/weight) accounts for the linear slope.

For the vision this means: catalog optimization should target pairwise algebraic
relationships between entries (axis cancellation quality, cross-depth ancestry
structure), NOT individual entry positions or coverage diversity. This is a
concrete design principle for the compilation algorithm.

### The encoding determines computability — more fundamental than activation (D92)
The parity-lock theorem proves that the +/-q encoding is structurally locked to
XOR/XNOR. The mechanism: masks 0...0 and 1...1 always produce identical quaternion
sums (both zero), so any f where f(0...0) != f(1...1) is impossible. AND, OR, MAJ
all have f(0...0) != f(1...1). This is not a limitation of the catalog or activation
— it holds at ALL depths, ALL resolutions, ALL roots of unity. The encoding creates
3^k equivalence classes; XOR/XNOR are the only standard Boolean functions constant
on all classes.

For the hybrid LLM vision this is the most consequential single result since D50
(parity wall resolution). It establishes a new hierarchy of design decisions:
encoding > activation > catalog > root of unity. The 1wpi encoding from D48/D50
already computes all 13 NPN classes — so the framework CAN compute non-parity
functions, but only with the right encoding. Encoding design is now the primary
research axis for broadening DKC's function repertoire.

### Regime transition identifies which design principles apply where (D87)
Null indispensability is finite-group-specific. At zeta_8 (24 entries, 13
directions), removing 9 null entries collapses XOR8 from 32 to 4 winners. At
zeta_12 (4096 entries, 2043 directions), removing 121 null entries preserves all
capacity. The transition mechanism is direction density: when non-null directions
are dense enough on S^2, null-only directions become computationally redundant
(even though they remain directionally exclusive — 67 null-only axes at zeta_12
that no non-null entry reaches).

For the vision this means compiled DKC neurons have regime-dependent design rules:
finite-group neurons (zeta_8) must preserve null entries; infinite-group neurons
(zeta_12) can safely prune them. This connects to the RC pillar — the reservoir
separation property interpretation of nulls (D84) applies only in sparse-direction
regimes.

### Circuit complexity hierarchy quantifies function difficulty (D93)
D93 measures the AND/XOR hit-rate ratio explosion (1.01 to infinity across N=3-8)
under phase_cell activation, making the Hastad circuit depth hierarchy visible in
DKC. For the hybrid LLM vision this is significant: AND-type functions (syllogistic
logic) are in AC^0 and are the EASIEST to compute — they degrade gracefully even at
large arity. XOR/parity is the hardest (outside AC^0). MAJ shows a cliff at N=8.
This means compiled DKC neurons for logical inference (AND/OR/implication) are
inherently more robust and efficient than parity neurons — the compilation cost
model is function-dependent, not uniform. The pigeonhole mechanism (84 cells, XOR
dies at N>=7) gives a concrete capacity bound per activation resolution.

### Solvability as a catalog design parameter (D94)
D94 confirms that non-solvable groups (2I) outperform solvable groups (z8) at
matched catalog size, with the advantage growing at the computational boundary.
For the hybrid LLM vision this opens a new catalog design axis: the group used
to generate the weight catalog affects not just capacity ceiling but per-entry
efficiency. The Z[sqrt5] arithmetic infrastructure extends the algebraic substrate
beyond cyclotomic integers. The five-pillar synthesis (Abramsky + Habiro + Aizenberg
+ Nazer-Gastpar + Reservoir Computing) is now complete — the DKC framework has a
precise theoretical home in each of five established research communities.

### The crossover reveals regime-dependent group selection (D94)
The N=6 crossover (z12 wins at small N, 2I wins at large N) means optimal catalog
design depends on the target arity. For low-arity functions (3-5 inputs, typical
for syllogistic reasoning), dense infinite-group catalogs are optimal. For
high-arity functions (6-8 inputs, more complex Boolean operations), non-solvable
finite groups provide better per-entry efficiency. A practical compilation pipeline
might use different catalog sources for different neuron types.

### Derived-series cross-classification gives principled catalog pruning (D95-D97)
The 5-cell landscape (A through E from crossing D84 null/non-null with D95 derived
series) provides the first principled framework for deciding which catalog entries
to keep. Cell E (Q8-null, 3 entries) is pure noise — removing it IMPROVES capacity.
Cell B (non-null non-commutator, 6 entries) is geometrically perfect (octahedron
on S^2). The optimal A+B+C catalog (21 entries) beats the full 24. For the hybrid
LLM vision this means compiled DKC neurons can use algebraically-optimized catalogs
that are both smaller and more effective — a direct deployment advantage.

### Multi-strand DKC opens matrix braid representations (D98-D101)
Moving from the scalar Kauffman bracket (2-strand) to matrix braid representations
(3-5 strand) is the most significant generalization in the program. The 3-strand
Delta_1 module achieves XOR14=60 (D99), the 4-strand W_{4,2} achieves XOR14=70
(D100), and the 5-strand W_{5,3} sustains XOR6-XOR12 but hits XOR14=0 (D101).
For the hybrid LLM vision this means compiled DKC weights can be not just scalars
(quaternions from 2-strand braids) but MATRICES (from multi-strand braids) — a
richer representation space with additional architectural parameters.

### The Barrington bridge grounds DKC in computational complexity (D102)
The radical's abelian writhe character connects DKC to Barrington's theorem (1989):
abelian groups cannot compute parity. This is the first external validation from
classical CS theory. For the hybrid LLM vision, the Barrington bridge means DKC's
computational capacity claims are not self-referential — they connect to the NC^1 /
AC^0 hierarchy that governs all circuit computation. The non-abelian structure of
the quotient (not the radical) is where parity lives, exactly as Barrington
predicts.

### BFS catalogs are module-independent reservoirs (D102-D103)
All three n=6 modules (W_{6,0}, W_{6,2}, W_{6,4}) produce bit-for-bit identical
BFS trees at every depth. The catalog is a B_n property — the reservoir is fixed
once you choose the strand count. Module choice and activation design control the
readout. For the hybrid LLM vision, this cleanly separates "what braids exist?"
(a braid group question, answerable once) from "how do you read them?" (a design
question, optimizable). The reservoir computing frame from D94 gains concrete
support.

### Sign-rank expansion as a computational resource (D104)
Sign quantization (ternary {-1,0,+1} projection) can INCREASE effective rank
(W_{6,2}: raw 244 → sign 292, a gain of 48 independent dimensions). The nonlinear
sign() function breaks Z-linear dependencies. For the hybrid LLM vision, this
means the activation layer does not merely discard information — it can create
new independent dimensions. The practical implication: activation design for multi-
strand DKC should exploit sign-rank expansion, not just minimize information loss.

### The k-regime resolves the multi-strand scaling story (D105)
The ratio k/2^N is the universal control parameter for XOR capacity. At k=4096,
8-strand DKC (n=8, dim=14) achieves XOR8=22266, beating 6-strand's 21699. The
"XOR dies at N>=7" (D93) was a low-k artifact. For the vision this means multi-
strand DKC can scale arbitrarily with activation resolution — there is no strand-
count ceiling analogous to the zeta_8 input-count ceiling. The bottleneck is always
the activation (k), never the algebra.

### Axis-alignment simplifies the compiled weight format (D107)
The Z/4Z axis-alignment theorem proves that every TL braid matrix entry at delta=0
is axis-aligned: a single integer times a single power of zeta_8. DKC computation
reduces to integer path counting + a three-variable phase formula. For hardware
implementation of compiled DKC weights, this means each matrix entry is just (int,
phase in Z/4Z) — dramatically simpler than generic Z[zeta_8] arithmetic. Combined
with D107's constructive nesting-parity formula, the phase is itself computable
from combinatorial data (writhe + nesting counts).

### Graph-structure prediction enables fast catalog screening (D108-D109)
The dual-channel theorem provides algebraic criteria for predicting parity
capability BEFORE running the expensive exhaustive search. Product closure > 0 and
v_2 connectivity > trivial are necessary conditions. Partition coincidence (triple
collapse) is a sufficient condition for incapability. For the compilation pipeline
this means candidate catalogs can be quickly screened: compute the Raqiya graph
analysis (O(n^2) in value count), check the dual-channel criteria, and only proceed
to exhaustive search if the structural prerequisites are met.

### The Amy bridge opens quantum compilation as a resource (D108)
The mapping between DKC's dual-channel structure and T-gate/Hadamard decomposition
in quantum circuit synthesis (Amy, Glaudell & Ross 2023) means DKC can potentially
import tools and results from the mature exact synthesis literature over Z[zeta_8].
The sde (smallest denominator exponent) is a resource measure for quantum circuits;
its DKC analog might provide a principled complexity measure for compiled weights.
For the vision, this validates that DKC is not isolated — it connects to an active
research community with decades of optimization results.

### Encoding-dependent polarity inversion reveals a design principle (D108-D109)
The same algebraic structure (product closure) has OPPOSITE computational
implications under additive vs multiplicative encoding. Under additive encoding,
high product closure enables parity (self-reinforcement). Under multiplicative
encoding, low product closure enables parity (escape). For the compilation pipeline,
this means the encoding choice determines not just WHICH functions are computable
(D92) but HOW the algebra is leveraged — the same catalog engineering principles
reverse sign when the encoding changes.

### Non-semisimple extension is computationally load-bearing (D99-D101)
The D100 vs D101 comparison establishes that the Ext^1 non-split extension (the
radical in W_{4,2}) is not algebraic decoration but a computational resource.
W_{4,2} (non-semisimple, radical dim > 0) achieves XOR14=70; W_{5,3} (simple,
radical dim = 0) achieves XOR14=0. The Casimir-XOR correlation inverts between
the two: winners have higher Casimir in the non-semisimple module but LOWER
Casimir in the simple module at high XOR levels. For the vision this connects
to D84-D86: the non-semisimplicity that underlies all of 2-strand DKC
(b = -5/8, LCFT correspondence) is the SAME feature driving multi-strand
computational capacity at the boundary.

### sl_d growth rate confirmed across three data points (D98-D101)
BFS catalog growth tracks n-1 cleanly: ~2x for 3-strand (2 generators), ~3.1x
for 4-strand (3 generators), ~4x for 5-strand (4 generators). This gives a
predictable cost model for multi-strand DKC: each additional strand roughly
multiplies the catalog growth rate by 1. For the vision this means the catalog
engineering problem scales predictably with strand count — higher strand counts
produce larger catalogs faster, with known tradeoffs in activation complexity.

### K-ladder resilience suggests robustness engineering (D88)
The Sec x Vor k-ladder activation provides 14x better perturbation resilience
than Voronoi-only activation (6.4% vs 89% loss under the same design-improving
gradient). The mechanism: multiple k_sec values provide fallback resolutions when
direction perturbation invalidates the optimal sector assignment. Constrained
optimization (null directions free, non-null directions clamped to 2 degrees)
captures 92% of design improvement with only 2.8% XOR loss.

For the vision this means the compiled DKC layer is not fragile — small
perturbations in weight placement do not catastrophically destroy computation,
provided the activation includes multi-resolution fallback. This is an important
engineering property for any practical deployment.

---

## Course Corrections

### Parity required non-standard activation
The parity wall (D48) and its resolution (D50) showed that standard activations
(sigmoid, half-plane) cannot access all the computation the lattice contains.
For the hybrid LLM vision, this means compiled DKC layers CANNOT use standard
neural network activation functions. They need k-sector or equivalent. This is
both a constraint and an opportunity — the compiled layers are architecturally
distinct from trained layers, which might actually make integration cleaner
(they're clearly separate subsystems).

### Exhaustive search doesn't scale — but has a principled scaling law
The current "compiler" searches |catalog|^(n+1) combinations. D63 shows
exhaustive search is feasible up to n=5 at k=15 sectors, but D78 confirms n=10
at zeta_8 is truly impossible. D81 gives the cost: each +2 XOR inputs requires
~10x more catalog entries (logarithmic law). The vision requires either:
- Compositional compilation (build large functions from small compiled units)
- Predictive compilation (theory for which braid -> which function)
- Restricting compiled weights to small logical units (syllogisms are 3-term)

The third option is actually fine for the vision — syllogisms are 3-term,
well within zeta_8's n=8 ceiling. The logarithmic scaling law now provides a
principled cost model for cases where more inputs are needed.

Additionally, the triskelion generalization (k=2n) was FALSIFIED by D63; the
actual scaling law is k=2M-1 where M is the ray count.

### The parity ceiling does NOT climb the Clifford hierarchy (D65)
An initially plausible conjecture from D64 was that XOR6 might become achievable
at Z[zeta_16] (one rung up the Clifford hierarchy). D65 REFUTES this: the parity
ceiling n=5 and all tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) are
identical at Z[zeta_16]. The gap between pigeonhole bound and actual ceiling widens
from 2 at Z[zeta_8] to 9 at Z[zeta_16]. This significantly constrains the theoretical
picture: the parity ceiling is likely a topological invariant of the state sum,
not an artifact of algebra level. For the vision this means the n=5 per-neuron
limit (under S^2 activation) is a genuine architectural constraint at all cyclotomic
levels — compositional design is not an optional engineering choice but a
mathematical necessity.

### Generalized activations break a convention wall, not the absolute wall (D65)
The two-layer structure revealed in D65 is an important refinement: there is an
absolute wall (no binary partition of k<=23 sectors works for XOR6 under any
labeling) AND a convention wall (standard odd/even labeling fails even where
non-standard labelings could succeed). Generalized XOR6 appears first at k=24
with antipodal-pair weights landing exactly on lattice boundaries — zero margin.
The S^1 minimum of k=24 sectors matches the 24-cell vertex count exactly (D66),
connecting the complex and quaternionic pictures. This two-layer structure means
"parity ceiling" must be stated carefully: the absolute ceiling for parity-as-any-
binary-separation is higher than the ceiling for parity-as-standard-sector-parity.

### zeta_8 is the computational sweet spot — finer roots trivialize (D69)
The direction-nesting theorem shows zeta_8 directions survive exactly into all finer
roots (zeta_16, zeta_32). But at zeta_16 natural Voronoi resolution the computation
trivializes: ~90% of all XOR6 triples become solutions via pigeonhole saturation
(3,457 cells, 64 binary inputs). This means the research program should stay
anchored at zeta_8 for 6-input work — not because finer rings are wrong, but because
at their natural resolution they lose all discriminative structure. For higher
input sizes requiring infinite groups (zeta_12, zeta_10), the analogous trivialization
threshold is yet to be determined.

### Demo 76's "zeta_8 is a 6-input structure" was wrong — activation-specific (D77)
D76 concluded XOR8 = 0 at zeta_8. D77 corrects this: with S^1xS^2 combined activation
(112 cells), XOR8 IS solvable at zeta_8 (6 winners). The zeta_8 wall is at n=8, not n=6.
This is the third instance of the "wall was in the activation/interface, not the
algebraic core" pattern:
- D50: parity wall was in split-sigmoid activation, not the Z[zeta_8] lattice
- D65: XOR6 wall was in the convention (odd/even labeling), not the geometry
- D77: XOR8 wall was in S^2-only activation, not the zeta_8 algebraic structure

This recurring pattern is a structural insight: when DKC hits a wall, the right
response is to examine the activation/interface first, not to abandon the algebra.

### The full catalog is not optimal — less is more (D96)
The assumption throughout D29-D94 was that using the complete 24-entry z8 catalog
was always better than any subset. D96 REFUTES this: the A+B+C subset (21 entries,
removing Q8-null Cell E) outperforms ALL (24 entries) at every N from 3-7. The
3 Q8-null entries are not merely unhelpful — they are NOISE that degrades
combinatorial search by adding entries whose signed sums contaminate the solution
space. This means all prior capacity measurements using the full catalog were
slightly pessimistic. For the vision this is an important course correction:
catalog curation (informed by algebraic structure) is a necessary step in the
compilation pipeline, not an optional optimization.

### Trace readout is an information bottleneck for matrix DKC (D98)
The natural generalization from scalar bracket to matrix representation appeared
to be: use the trace (which reduces a matrix to a scalar). D98 shows trace
retains only 5.1% of the catalog (105/2048 entries) and achieves zero XOR at
all levels. The readout mechanism — how you extract a scalar from a matrix — is
critical for multi-strand DKC. The successful approach (D99-D101) uses sign-pattern
hashing of all matrix components, treating the full matrix as the activation input.
This is the multi-strand analog of the "wall was in the activation" pattern.

### The radical is NOT a computational resource — it is a writhe recorder (D102)
The D100-D101 finding that non-semisimplicity is load-bearing suggested that the
radical (the non-semisimple part) might be the source of the advantage. D102
REFUTES this: the radical carries only the abelian writhe character (M*r =
A^{writhe}*r for all 32768 entries), which by Barrington's theorem cannot compute
parity. The simple module W_{6,0} beats the non-simple W_{6,4} at every XOR level.
The course correction: non-semisimplicity helps at n=4 (D100) through the Loewy
structure (length 3) and the mixing row, NOT through the radical direction itself.
The mixing row provides an 8% boost at XOR6 — real but modest, and regime-dependent
(disappears at higher XOR levels). The refined thesis: non-semisimple modules can
provide advantage through quotient-radical INTERACTION (mixing row), not through
the radical as an independent computational channel.

### Higher module dimension hurts — activation design matters more (D103-D104)
The intuition that higher-dimensional modules (more matrix components = more
information) would produce better XOR capacity is WRONG at face value. W_{6,2}
(dim=9, 324 components) is strictly worse than W_{6,0} (dim=5, 100 components) at
matched k. The curse of dimensionality in sign-hash activation dominates. However,
this is a hash problem, not an algebra problem: at optimal component count (120
of 324), W_{6,2} beats W_{6,0}. The course correction: raw module dimension is
not a useful design parameter. Effective dimension (rank/total, sign-rank
expansion, cross-block structure) is what matters. Activation design for multi-
strand DKC must be principled (branching decomposition, Atkinson sweet spot), not
naive (hash all components).

### "XOR dies at N>=7" was a k-regime artifact (D105)
D93's conclusion that XOR disappears for N>=7 inputs was based on k=128 (84 cells).
D105 shows this was entirely a pigeonhole artifact: at k=4096, W_{8,0} achieves
XOR8=22266. The ratio k/2^N is the real control parameter. At k=128 for N=8,
k/2^N=0.5 (deep pigeonhole); at k=4096, k/2^N=16. The course correction: all
prior "XOR dies at large N" claims must be re-evaluated with the k-regime in mind.
The true capacity scaling is k-limited, not strand-limited.

### Rank comparisons across modules are confounded by under-sampling (D105)
D105's rank saturation analysis reveals that all prior cross-module rank
comparisons (D99-D104) are confounded by varying oversampling ratios. W_{8,2}
at 512 entries was 6.9% rank utilization — severely under-sampled. The "34.9%
match" between W_{8,0} and W_{8,2} at initial checkpoints was coincidental.
The course correction: future multi-strand work must report rank saturation
curves, not single-checkpoint measurements. The retroactive rank table (D105)
provides corrected values for all prior modules.

### Topological entropy is irrelevant to DKC — individual braid properties are non-predictive (D106)
The intuitively plausible prediction that pseudo-Anosov braids (chaotic dynamics)
would be better at computation than periodic braids (simple dynamics) is
COMPLETELY wrong. Both compute XOR at 100% participation with near-identical
scores. The TL visibility filter explains why: the TL quotient at delta=0 erases
all dynamical information (e_i^2=0 kills expanding eigenvalues). This is a course
correction for the entire approach to understanding DKC: individual braid
properties (entropy, spectral radius, word length, writhe) are consistently
non-predictive. Only RELATIONAL/ALGEBRAIC properties of value sets predict
computational capacity. This reinforces the D89-D90 finding that the depth law
mechanism is relational, not positional.

### Raqiya discriminates impoverishment, not health (D109)
The D108 dual-channel theorem appeared to provide universal structural criteria
for parity capability. D109 shows the criteria are encoding-dependent AND that
at delta=sqrt(2) (rich algebra, j=0 alive), Raqiya CANNOT discriminate parity
from non-parity on 7 of 8 edge types. Product closure is the sole discriminator
and its polarity inverts. The course correction: Raqiya is a diagnostic for
algebraic impoverishment (delta=0, sparse algebra) not algebraic health
(delta!=0, rich algebra). When both vocabularies satisfy all structural
prerequisites, Raqiya has nothing to distinguish. The dual-channel theorem is
real but its practical utility is regime-dependent — most useful in the sparse
(zeta_8, delta=0) regime where the compilation pipeline currently operates.

### Hub-count scaling does not follow simple combinatorial formula (D101)
The prediction from D100's 6 hubs was that 5-strand would show 12 hubs (= 2*C(4,2)).
D101 finds only 3. The hub structure is more complex than a simple combinatorial
scaling law — it depends on the module type (simple vs non-semisimple) and possibly
on the activation quality. The sl_d growth rate prediction works; the hub-count
prediction does not. For the vision this means the winner topology of multi-strand
DKC requires per-module characterization, not universal scaling laws.

### The zeta_N -> XOR N capacity conjecture was falsified (D79)
The initial hypothesis that zeta_12 would support at most XOR12 was refuted: zeta_12
already reaches XOR12 and appears to have capacity well beyond it. The true
governing variable is group finiteness, not the subscript N. Finite groups (zeta_4,
zeta_8) impose a ceiling; infinite groups (all others tested) grow logarithmically
with catalog size. The correct framing is: finite group -> finite ceiling (ADE
type determines the ceiling); infinite group -> unbounded capacity (at logarithmic
catalog cost).

### The depth law is relational, not positional — this changes catalog optimization (D89-D90)
The initial intuition (from D82) was that deep entries are better because they
occupy "better positions" on S^2 — more directions, finer angles, greater coverage.
D90 INVERTS this: the S^2 point cloud bandwidth DECREASES with depth (spectral
inversion). Deep entries converge to spherical uniformity — they are individually
LESS informative than shallow entries. The advantage is entirely relational: how
entries combine (axis cancellation, cross-depth algebraic constraints) rather than
where they individually sit. For the vision this means any catalog optimization
scheme based on individual entry quality (coverage, angular spacing, spherical
design residual) is fundamentally misguided. The correct optimization target is
pairwise algebraic relationship quality — a much harder but more principled problem.

### The encoding determines which functions are computable — more fundamental than activation or catalog (D92)
D92's parity-lock theorem is the most significant course correction since D50. It
establishes that encoding design is the most fundamental architectural choice:
- **Encoding** determines which Boolean functions are computable AT ALL
- **Activation** determines how many solutions exist (resolution)
- **Catalog depth** determines maximum arity
- **Root of unity** determines capacity ceiling (finite vs infinite)

The +/-q encoding concentrates all computational power on parity — the single
Boolean function outside AC^0. This is not a limitation to be overcome but a
structural feature: the encoding selects for the hardest function. For any
non-parity function (AND, OR, MAJ, THRESHOLD), a different encoding is required.
The 1wpi encoding from D48/D50 computes all 13 NPN classes — the DKC framework
is not parity-locked, only the +/-q encoding is. Encoding design is now a primary
research axis.

---

## Shortest Paths to Vision Milestones

### Path A: Prove the syllogism-DKC connection (1-2 demos)
```
Current state -> Syllogism->NPN mapping (1 demo) -> Toy taxonomy->weight pipeline (1 demo)
```
This would confirm or deny that classical syllogisms naturally fit the 3-input
DKC framework. If yes, the taxonomy->logic half of the vision becomes concrete.
If no, we need to rethink. The zeta_8 n=8 ceiling (D77-D78) confirms all 3-input
Boolean functions are reachable; the S^1xS^2 geometry (D77) establishes the
activation to use. **D92 adds an encoding consideration**: the syllogism->NPN
mapping must be paired with an encoding that can compute the target NPN class.
The 1wpi encoding (D48/D50) handles all 13 classes; the +/-q encoding handles
only XOR/XNOR.

### Path B: Prove multi-layer composition works (1 demo)
```
Current state -> Multi-layer composition test (1 demo)
```
Feed DKC neuron outputs into other DKC neurons. If exactness survives, the
vision's "reasoning chains" become feasible. DKC capacity per neuron is now
well-characterized (n<=8 for zeta_8 with S^1xS^2 activation), so multi-layer
composition is the clear path for anything beyond 8-input functions.

### Path C: Demonstrate dynamic recompilation (1 demo)
```
Current state -> Toy taxonomy with add/remove fact -> recompile -> verify (1 demo)
```
Even with an ad hoc taxonomy->braid encoding, demonstrating that you can add a
fact, recompile weights, and get correct inference would be a powerful proof
of concept for the dynamic update story.

### Path D: Prove the 13=13 bandwidth theorem analytically (1-2 demos)
```
Current state -> Analytical proof l=ceil((N-1)/2) for N-direction Voronoi on S^2
```
The 13=13 theorem (D71) is computationally verified. A proof that the minimum
bandwidth for any N-direction binary Voronoi on S^2 is l=ceil((N-1)/2) would be a
clean theoretical result connecting spherical harmonics to the NPN count (13=13)
and to the direction count. This proof path has no dependency on the taxonomy
gap and strengthens the program's credibility independently.

### Path E: XOR7 on S^2 — scale test for the spectral framework (1 demo)
```
Current state -> XOR7 weight search with S^2 activation -> bandwidth measurement
```
Demo 65 showed XOR7 requires k=127 sectors on S^1. The DOF formula predicts the
S^2 bandwidth for XOR7 is approximately l=ceil((M-1)/2) where M is the XOR7
eigenvector direction count (unknown). If S^2 again reduces the sector requirement
dramatically, this validates the spectral framework as the right language for
characterizing DKC computation across all arity levels.

### Path F: Depth-targeted catalog generation (1 demo) — NEW from D82
```
Current state -> Direct generation of depth-d quaternions -> verify linear depth law
```
D82 showed deep entries (high crossing depth) are 2x more efficient than shallow
entries for XOR computation. D90's mechanistic explanation (axis cancellation +
cross-depth constraints) now provides concrete criteria for what makes deep entries
useful: they should have anti-aligned axes relative to shallower entries, and
their generator-product structure should create constrained sum-angle vocabularies.
This could guide direct generation without full BFS closure.

### Path G: Union catalog test (1 demo) — NEW from D79
```
Current state -> zeta_8 U zeta_12 union catalog -> XOR capacity test
```
D79 confirmed zeta_8 and zeta_12 are largely non-overlapping (12% quaternion overlap,
15% direction overlap). A union catalog should exploit both independent algebraic
structures. If the union catalog shows superadditive capacity or lower cost per
XOR level, this opens a practical path: different roots of unity contributing
complementary weight families to the compiled layer.

### Path H: Encoding design for non-parity functions (1-2 demos) — NEW from D92
```
Current state -> 1wpi encoding depth law test (1 demo) -> hybrid encoding design (1 demo)
```
D92 proves the +/-q encoding is parity-locked. The 1wpi encoding (D48/D50)
computes all 13 NPN classes but has not been tested for depth-law behavior. Does
max_and or max_maj scale linearly with depth under 1wpi? If so, this encoding
may be the correct choice for the hybrid LLM vision (which needs AND/OR for
syllogistic logic, not just parity). If not, a hybrid encoding (some inputs +/-q,
some 1wpi) might balance function breadth with depth-law scaling. This is now
the most architecturally consequential open question for the compilation pipeline.

### Path I: Prove the balanced exponentials analytically (1 demo) — NEW from D91
```
Current state -> BFS branching factor analysis -> parity demand growth -> slope derivation
```
D91 confirmed balanced exponentials (BFS ~2x/round, parity 4x/weight, slope ~1)
as the mechanism behind the linear depth law. An analytical derivation connecting
BFS branching factor to parity constraint growth would make this a theorem rather
than a demonstrated conjecture. This is the next clean formal proof candidate
after the parity-lock theorem (D92).

### Path K: RKHS kernel rank test — solvability as reservoir quality (1 demo) — NEW from D94
```
Current state -> Compute K(m,m') for 2I and z8 -> rank ratio vs size ratio
```
D94 proposes a single testable prediction: rank(K_2I)/rank(K_z8) should exceed
120/24 = 5x if non-solvability contributes above raw catalog size. The kernel
K(m,m') = quaternion inner product of signed sums. If confirmed, this establishes
non-solvability as a reservoir quality metric — not just "more elements" but
"better algebraic structure per element." This is seeded as D95 and is the
cleanest single-number test of the five-pillar reservoir computing interpretation.

### Path J: Regime transition characterization (1 demo) — NEW from D87
```
Current state -> Intermediate roots (zeta_10, truncated zeta_12) -> locate transition threshold
```
D87 established the regime transition between zeta_8 (nulls indispensable) and
zeta_12 (nulls dispensable). The exact transition point — is it a specific
direction count, catalog size, or null fraction? — is unknown. Locating it would
give a concrete design threshold: below the transition, preserve nulls; above,
prune safely.

### Path L: Multi-strand activation design (1-2 demos) — NEW from D98-D101
```
Current state -> Principled activation for matrix DKC (1 demo) -> activation comparison across modules (1 demo)
```
The sign-pattern hash activations (2x2_hash, 3x3_hash, 4x4_hash) used in D99-D101
are crude — they work but are likely far from optimal. The 2-strand program had
a similar trajectory: crude activations (D29) → optimized k-sector (D50) →
S^1xS^2 product (D77) → k-ladder resilience (D88). Designing a principled
activation for matrix DKC (exploiting matrix structure: eigenvalues, singular
values, radical projection, Casimir) could dramatically improve multi-strand
capacity. This is the multi-strand analog of Path D50.

### Path O: Dual-channel prediction for custom sector labelings (1 demo) — NEW from D108
```
Current state -> Raqiya analysis of candidate value sets at k>=24 -> predict which achieve XOR6
```
D108 established the dual-channel theorem: product closure + v_2 connectivity
predict parity capability. D65 showed generalized XOR6 appears at k=24 with
non-standard labeling. Can the dual-channel analysis predict WHICH custom sector
labelings at k>=24 achieve XOR6? If so, this would be the first predictive use
of Raqiya for activation design — moving from retroactive analysis to prospective
engineering.

### Path P: Additive encoding at delta=sqrt(2) (1 demo) — NEW from D109
```
Current state -> D57-style additive encoding at delta=sqrt(2) -> Raqiya graph analysis
```
D109's encoding-dependent polarity inversion is established by comparing D108
(additive, delta=0) with D109 (multiplicative, delta=sqrt(2)). The cleanest
confirmation would hold delta constant (sqrt(2)) and vary encoding. If additive
encoding at delta=sqrt(2) restores D108-style structural discrimination (high
product closure → parity), the encoding-dependence is confirmed and the
delta-dependence is ruled out. This is a single-demo experiment with high
theoretical payoff.

### Path Q: Macrame principle crossover characterization (1 demo) — NEW from D105
```
Current state -> W_{6,4} or W_{10,2} branching decomposition -> locate crossover between cross-block and within-block regimes
```
D104 (n=6, asymmetric blocks dim 5+4): cross-block wins. D105 (n=8, symmetric
blocks dim 14+14): within-block wins. The crossover lies between block dim 5 and
dim 14. Finding the exact crossover would give a concrete design rule for
activation engineering: at what block dimension should multi-strand activation
switch from cross-block to within-block hashing?

### Path R: Raqiya-guided catalog screening for new roots (1 demo) — NEW from D108
```
Current state -> Build catalog at zeta_12 or 2I -> Raqiya graph analysis -> predict parity boundary -> verify
```
The dual-channel theorem was demonstrated at delta=0 (zeta_8) and delta=sqrt(2)
(zeta_16). Can Raqiya predict the parity boundary at other roots? At zeta_12
(infinite group, 4096 entries), running Raqiya on the full value set and then
predicting which subset supports parity would test whether graph-based prediction
generalizes beyond the zeta_8 regime. If it works, Raqiya becomes a universal
catalog screening tool.

### Path M: 6-strand W_{6,4} — decisive simple vs non-semisimple test (1 demo) — NEW from D100-D101
```
Current state -> W_{6,4} (5x5 non-semisimple, TL_6 at delta=0) -> XOR14 capacity test
```
D100 (n=4, non-semisimple, XOR14=70) vs D101 (n=5, simple, XOR14=0) established
that non-semisimplicity is load-bearing. But the comparison confounds strand count
with module type. W_{6,4} (n=6, even, non-semisimple) would test whether the
non-semisimple advantage persists at higher dimension. If W_{6,4} achieves XOR14>0
despite being 5x5 (larger than W_{5,3}'s 4x4), the non-semisimple thesis is
confirmed. If not, the advantage may be dimension-dependent rather than
structure-dependent. This is the single most decisive test for the multi-strand arc.

### Path N: XOR16 on Delta_1 with better activation (1 demo) — NEW from D99
```
Current state -> Redesigned activation for Delta_1 2x2 -> XOR16 capacity test at larger bf
```
D99 achieved XOR14=60 but XOR16=0 on Delta_1 with the 2x2_hash activation. Is
the XOR16 ceiling genuine (structural, like the 2-strand ceiling) or is it an
activation wall (like D50, D77, D98)? A better-designed activation for the 2x2
representation could resolve this. If XOR16>0, multi-strand DKC has higher
capacity ceilings than 2-strand — a major finding for the vision.

---

## Recommended Next Investigation

**The syllogism -> NPN mapping** remains the highest-leverage single demo for the
vision. It requires no new code infrastructure (existing NPN classification +
DKC search), answers the most critical question (do syllogisms fit 3-input
DKC?), and its answer determines whether Path A is viable. The S^1xS^2 geometry
from D77 gives the correct activation to use for any subsequent demos; the
anti-correlation result from D72 confirms algebraic selection of weights is
principled.

**However, D92 introduces a new urgency for Path H (encoding design).** The
parity-lock theorem means the current +/-q encoding CANNOT compute the
non-parity Boolean functions that syllogistic logic requires (AND, OR,
implication). The 1wpi encoding CAN compute all 13 NPN classes, but its
depth-law behavior is unknown. If the depth law does not hold under 1wpi, the
entire compilation cost model changes. Testing the 1wpi depth law should be
prioritized alongside or even before the syllogism mapping, because a negative
result would fundamentally reshape the compilation strategy.

If the syllogism mapping succeeds AND the 1wpi encoding has a depth law, the
toy taxonomy pipeline (Path C) becomes the natural follow-up:
taxonomy -> syllogisms -> NPN classes -> DKC weights (1wpi encoding, appropriate
activation) — a complete (if toy) end-to-end pipeline.

For the theoretical program, the parity-lock theorem (D92) is the first formal
proof. The next candidates are:
- **Balanced exponentials** (Path I) — the linear depth law slope as a theorem
- **Regime transition** (Path J) — a sharp threshold theorem for null dispensability
- **Spectral inversion** (from D90) — the S^2 bandwidth decreasing with depth
  while computation increases
- **RKHS kernel rank** (Path K) — a single number testing the algebraic reservoir interpretation

D94's five-pillar synthesis completion means the theoretical framework is now
fully instantiated. The next phase is empirical validation (Path K: kernel rank)
and practical deployment (Path A: syllogism mapping, Path H: encoding design).
D93's circuit complexity hierarchy result provides a concrete cost model for
function-dependent compilation: AND-type operations are cheap, parity is expensive,
and the pigeonhole mechanism gives exact capacity bounds per activation resolution.

For the engineering program, Path F (depth-targeted generation) is now informed
by the mechanistic understanding from D89-D91: the target property for deep
entries is not positional quality but relational quality (axis anti-alignment,
cross-depth vocabulary constraint). This makes the generation problem more
precisely stated, though not necessarily easier.

**The multi-strand arc (D98-D105) is now mature.** D102-D105 resolved most of the
open questions from D98-D101: the radical is inert (D102), dimension scaling is
understood (D103), activation theory is principled (D104), n=8 works (D105), and
the k-regime is the real scaling control (D105). The immediate priorities shift:
- **Path L** (activation design) — now informed by Atkinson sweet spot, branching
  decomposition, sign-rank expansion, and k-regime theory. The crude sign-pattern
  hashes can be systematically improved using D103-D105 tools.
- **Path Q** (macrame crossover) — locating the exact crossover between cross-
  block and within-block regimes would give a concrete activation design rule.
- **Path N** (XOR16 on Delta_1) — still determines whether the XOR16 ceiling is
  structural or activation-limited.
- **Path M** (W_{6,4} test) — partially answered by D102 (W_{6,4} loses at
  matched dim=5), but the Loewy-length question at higher dim remains open.

**The relational computation arc (D106-D109) opens a predictive research front.**
This is the most promising new direction for the compilation pipeline:
- **Path O** (dual-channel prediction for custom labelings) — the first
  predictive use of Raqiya for prospective activation design.
- **Path R** (Raqiya-guided catalog screening) — tests whether structural
  prediction generalizes beyond zeta_8.
- **Path P** (additive encoding at delta=sqrt(2)) — the cleanest test of the
  encoding-dependent dual-channel theorem.

The catalog pruning results (D95-D97) remain immediately actionable: any future
demo using the z8 catalog should use A+B+C (21 entries) rather than the full 24.

**The Raqiya library is the first concrete piece of lasting infrastructure.**
The vision emphasizes building tools that last (CLAUDE.md: "programs are like
machines that do not normally experience entropy"). Raqiya — a single-header C89
library for Z[zeta_8] algebraic analysis with 208+ unit tests, exact integer
arithmetic, 9+ relationship detectors, and graph analysis — embodies this
principle. It was deployed across three demos (D107-D109) with zero regression,
growing by adding new detectors rather than modifying existing ones. This is the
kind of library that the long-term monorepo vision requires.

---

## The "Wall Was in the Activation" Pattern — A Structural Insight

Across the full D50-D105 arc, the same pattern has appeared five times for
activation, a sixth time for encoding, and a seventh time for readout:

| Demo | Apparent Wall | True Cause | Resolution |
|------|--------------|------------|------------|
| D50 | Parity unreachable at zeta_8 | Split-sigmoid activation | k=6 sector activation |
| D65 | XOR6 unreachable at k<24 | Odd/even convention, not geometry | Generalized labeling |
| D77 | XOR8 = 0 at zeta_8 | S^2-only activation | S^1xS^2 product activation |
| D92 | Only XOR/XNOR computable | +/-q encoding, not DKC itself | 1wpi encoding (D48/D50) |
| D98-D99 | 3-strand XOR = 0 | Trace readout (5.1% retention) | Sign-pattern hash on Delta_1 2x2 |
| D103-D104 | Higher dim module = worse XOR | Sign-hash curse of dimensionality | Atkinson sweet spot (120 of 324 comp) |
| D105 | XOR dies at N>=7 | Low-k pigeonhole (k/2^N < 1) | k=4096 (k/2^N = 16): XOR8=22266 |

The pattern generalizes: when a DKC computation appears impossible, suspect the
interface layer first — activation, convention, encoding, or readout. The algebraic core
(Z[zeta_8] bracket, binary octahedral quaternions) has consistently proven richer
than any single interface reveals. D92 extends this pattern from the activation
layer to the encoding layer: the +/-q encoding was the wall, not DKC itself.
D98-D99 extend it further to the readout layer: the trace was the wall for
multi-strand DKC, not the 3-strand algebra. D103-D105 add two more instances:
the dimension wall (dim-9 module appearing worse) was in the naive all-component
hash, and the strand-count wall (N>=7 appearing dead) was in the k-regime.

This has a direct implication for the hybrid LLM vision: the interface design
between compiled DKC weights and the broader neural network will face similar
walls — and the right response will be to look for a richer encoding/readout
mechanism, not to abandon the underlying algebra.

---

## Revised Overall Outlook for the Long-Term Vision

After D102-109 the picture adds multi-strand maturation (scaling, activation
theory, Barrington bridge), relational computation diagnostics (Raqiya, dual-
channel theorem), and connections to both classical complexity theory and quantum
circuit synthesis to the already-solid algebraic foundation:

**What we now know that we didn't before D95:**
1. The indecomposability parameter b = -5/8 is extractable from first principles,
   quantifying the LCFT non-semisimple structure that makes DKC compute (D85).
2. b is a collective property of the full algebra, not extractable from a single
   indecomposable module — the multiplicity is structurally essential (D86).
3. Null indispensability is regime-dependent: critical at finite groups,
   dispensable at infinite groups (D87).
4. The anti-correlation between design quality and computation is explained by
   two-role direction architecture (anchors vs scaffolding), and the k-ladder
   provides order-of-magnitude perturbation resilience (D88).
5. The depth law mechanism is axis cancellation + cross-depth algebraic
   constraints — relational, not positional (D89-D90).
6. The depth law wall is parity itself, not the activation — balanced
   exponentials account for the linear slope (D91).
7. The +/-q encoding is structurally parity-locked; encoding determines which
   functions are computable — more fundamental than activation or catalog (D92).
8. The z8 catalog has principled structure: derived-series cross-classification
   yields a 5-cell landscape, optimal pruning removes Q8-null noise, and Cell B
   (6 entries) is geometrically perfect — an octahedron on S^2 (D95-D97).
9. Multi-strand DKC WORKS: 3-strand achieves XOR14=60 on Delta_1 (D99 LANDMARK),
   4-strand achieves XOR14=70 on W_{4,2} (D100), 5-strand sustains XOR6-XOR12
   on W_{5,3} but hits XOR14=0 (D101).
10. Non-semisimple extension (Ext^1) is computationally load-bearing: the
    D100 vs D101 comparison establishes this definitively (D100-D101).
11. BFS growth rate follows the sl_d functor prediction: ~(n-1)x per round,
    confirmed across 3 data points (D98-D101).
12. The "wall was in X not Y" pattern has a 5th instance: 3-strand trace readout
    was the bottleneck, not the algebra — sign-pattern hash on Delta_1 unlocks
    XOR (D98-D99).
13. The radical carries an abelian writhe character, provably useless for parity
    by Barrington's theorem — the Barrington-Radical Principle connects DKC to
    classical computational complexity theory (D102).
14. BFS growth is a braid group invariant — all modules of the same strand count
    produce identical BFS trees. Module choice controls readout, not reservoir
    (D102, D103).
15. Sign-hash activation has a non-monotonic optimal component count (Atkinson
    sweet spot) and sign quantization can EXPAND effective rank by breaking
    Z-linear dependencies (D104).
16. Branching cross-block carries the computational signal at n=6 (D104) but
    within-block dominates at n=8 (D105) — the macrame principle regime
    transition at block dim ~5-14 (D104-D105).
17. k/2^N is the universal control parameter for multi-strand XOR capacity;
    "XOR dies at N>=7" was a low-k artifact; 8-strand DKC achieves XOR8=22266
    at k=4096 (D105).
18. Topological entropy is completely orthogonal to DKC computation — the TL
    visibility filter kills all dynamical information at delta=0 (D106).
19. Axis-alignment at delta=0 is a provable theorem (not empirical) with
    constructive Z/4Z phase formula: entry(r,c) = count * zeta_8^{(w - 2*(nest(r)
    + nest(c))) mod 4}. DKC reduces to Z x Z/4Z per matrix entry (D107).
20. Graph structure predicts parity capability: the dual-channel theorem requires
    product closure > 0 AND v_2 connectivity > trivial; partition coincidence
    signatures incapability (D108).
21. DKC's dual-channel maps onto T-gate/Hadamard decomposition in quantum circuit
    synthesis — the Amy bridge (D108).
22. Dual-channel polarity is encoding-dependent: additive encoding wants HIGH
    product closure for parity, multiplicative wants LOW. Raqiya discriminates
    algebraic impoverishment, not algebraic health (D108-D109).
23. Raqiya is operational as a reusable C89 analysis tool with 208+ unit tests,
    deployed across D107-D109 with zero regression (D107-D109).

**What this changes about the vision:**
- The encoding is the first design decision. It determines which Boolean functions
  the compiled neuron can compute at all. The +/-q encoding specializes in parity
  (the hardest function); the 1wpi encoding handles all 13 NPN classes. A
  practical hybrid LLM needs the 1wpi encoding or a successor for syllogistic
  logic (which requires AND, OR, implication — not just XOR).
- Catalog optimization should target relational quality (axis cancellation,
  cross-depth constraints), not positional quality (coverage, spacing, spherical
  design). The spectral inversion result (D90) proves positional optimization
  is wrong in principle. Additionally, algebraic pruning (removing Q8-null
  entries) actively improves capacity (D96) — smaller catalogs can be better.
- Design principles are regime-dependent. Finite-group compiled neurons (zeta_8)
  and infinite-group compiled neurons (zeta_12) have different null-state
  requirements, different dominant pathways (shadow pairing vs direction
  diversity), and different robustness profiles.
- The depth law is now mechanistically understood, not just empirically observed.
  This means it can be potentially manipulated: a different BFS branching factor
  (different group) or a relational activation (explicitly using axis dot
  products) might change the slope.
- **Multi-strand DKC adds a new design axis.** Compiled DKC neurons are no longer
  limited to scalar (quaternion) weights from 2-strand braids. Matrix weights
  from 3-, 4-, 5-strand braids are now demonstrated, with strand count and module
  type as additional design parameters. The practical question is: when should a
  compiled neuron use 2-strand (scalar, well-understood, proven activations) vs
  multi-strand (matrix, higher-dimensional, crude activations but potentially
  higher capacity)?
- **Non-semisimplicity is a nuanced resource — the radical itself is inert.**
  D84-D86 and D99-D101 established that non-semisimplicity is load-bearing.
  D102 sharpens this: the radical direction carries only the abelian writhe
  character, provably useless for parity by Barrington. The advantage comes from
  the quotient-radical INTERACTION (mixing row), not the radical as independent
  channel. A practical hybrid LLM should use non-semisimple modules for the
  mixing-row advantage (D100, D102), but not expect the radical to compute
  anything on its own.
- **The compilation pipeline can now include a structural screening step.**
  D108-D109's dual-channel theorem means candidate catalogs can be evaluated
  for parity capability using Raqiya graph analysis BEFORE the expensive
  exhaustive search. This is a concrete speedup: O(n^2) graph analysis vs
  O(catalog^{n+1}) exhaustive search. For the compilation pipeline, this means
  "compile a fact" could include a fast screening pass followed by targeted
  search in the predicted-capable space.
- **DKC connects to two external theoretical communities.** The Barrington bridge
  (D102) connects to computational complexity (NC^1, AC^0, Barrington's theorem).
  The Amy bridge (D108) connects to quantum circuit synthesis (exact synthesis
  over Z[zeta_8], T-gate/Hadamard decomposition). These are not just theoretical
  decoration — they provide external validation and importable tools. The program
  is no longer isolated.
- **Activation design for multi-strand DKC is now principled.** D103-D104
  establish concrete tools: Atkinson sweet spot (optimal component count), sign-
  rank expansion (nonlinear rank gain), branching decomposition (cross-block vs
  within-block), k-regime theory (k/2^N as universal control). These replace the
  crude "hash all components" approach of D99-D101.
- **The axis-alignment theorem simplifies the compiled weight format.** Every TL
  braid matrix entry at delta=0 is (integer, phase in Z/4Z). Combined with the
  nesting-parity phase formula, the phase is combinatorially computable. For
  hardware this means compiled DKC weights are integer + 2-bit phase tag per
  matrix entry — the simplest possible exact format.

**What remains unknown:**
- The taxonomy -> braid encoding (Gap 1) — the single most important unknown.
- Whether the 1wpi encoding has a depth law (Gap 10) — if not, the compilation
  cost model for non-parity functions is completely open.
- Multi-layer composition with exact arithmetic (Gap 3).
- Integration architecture (Gap 4) — wired, not pipelined.
- Whether the acceleration thesis (compiled scaffolding speeds statistical training)
  has any empirical grounding.
- Encoding design for targeting specific Boolean functions beyond parity (Gap 10).
- Optimal activation design for multi-strand matrix DKC (Gap 11) — the crude
  sign-pattern hashes are surely suboptimal; better activations could shift the
  capacity frontier significantly.
- Whether the XOR16 ceiling on Delta_1 is structural or activation-limited
  (Gap 12) — determines whether multi-strand DKC has higher capacity ceilings
  than 2-strand.
- Whether W_{6,4} (6-strand, non-semisimple) achieves XOR14>0 (Gap 13) — the
  decisive test for universal non-semisimple load-bearing. D102 partially
  addresses this: W_{6,4} LOSES to W_{6,0} at every XOR level, but the test
  was at matched dimension (both dim=5). The question of Loewy-length-3+
  non-semisimplicity at higher dimension remains open.
- How hub structure scales with strand count and module type (Gap 14) — the
  simple combinatorial prediction (2*C(n-1,2)) was falsified at n=5.
- Whether multi-strand DKC has a depth law analogous to 2-strand's
  max_xor ~ depth+6 (Gap 15) — preliminary evidence from D99-D101 suggests
  deep entries are needed for high XOR, but the law is not yet characterized.
- Where the macrame principle crossover lies (Gap 16) — cross-block wins at
  block dim 5 (D104), within-block wins at block dim 14 (D105). The exact
  crossover would give a concrete design rule for activation engineering.
- Whether the dual-channel theorem generalizes to infinite-group catalogs
  (Gap 17) — D108 demonstrated at zeta_8 (finite, 100 values); untested at
  zeta_12 (infinite, 4096+ values) or 2I (finite, 120 values).
- Whether Raqiya can predict parity capability at other roots of unity (Gap 18)
  — demonstrated at delta=0 (zeta_8) and delta=sqrt(2) (zeta_16). Each regime
  has different discrimination properties.
- The quantitative product-closure threshold for parity (Gap 19) — D108 shows
  parity needs product closure > 0 (qualitative). What is the minimum density
  needed? Is there a sharp threshold or gradual crossover?
- Whether additive encoding at delta=sqrt(2) restores D108-style discrimination
  (Gap 20) — the cleanest test of encoding-dependence vs delta-dependence.

**Confidence level:** The algebraic foundation is extremely solid, mechanistically
understood, generalized to multi-strand representations up to n=8, grounded in
computational complexity theory (Barrington bridge), connected to quantum circuit
synthesis (Amy bridge), and equipped with predictive diagnostic tools (Raqiya +
dual-channel theorem). The knot theory -> Boolean logic half of the pipeline is
essentially proven for parity functions under the +/-q encoding, extended to
matrix braid representations (D99-D105), and now supplemented with structural
prediction capability (D108-D109). The D102-109 arc resolves several open
questions from D98-D101 (radical role, dimension scaling, activation theory,
k-regime) and opens the new research axis of structural prediction — moving from
"search and find" to "predict and verify." The remaining unknowns are: (1) the
taxonomy -> knots mapping (still the bottleneck), (2) the 1wpi encoding's
scaling behavior (still the most urgent theoretical question from D92), (3) the
integration layer, (4) principled multi-strand activation design (now informed
by Atkinson sweet spot, sign-rank expansion, branching decomposition, and
k-regime theory from D103-D105), (5) whether dual-channel prediction generalizes
to other regimes (D108-D109 establish the method; generality is open), and
(6) the macrame principle crossover (D104-D105 bracket it but don't locate it).
The catalog optimization problem remains SOLVED for z8 (D95-D97): use A+B+C.
The activation design problem for multi-strand DKC is now principled (D103-D105)
rather than ad hoc (D99-D101). The structural prediction capability (Raqiya +
dual-channel) is the most significant new tool for the compilation pipeline.

---

*Generated 2026-02-20, updated 2026-02-26. Reads: narrative.md, novelty.md,
connections.md, four-lenses.md, planning/vision.md, inventory/demo-index.md
(via entries for D19, D29, D39G-J, D48, D50, D61-109). D95-D101 additions cover
the commutator/synergy arc (D95-D97: derived series, cross-classification,
catalog pruning, Cell B perfection) and the multi-strand arc (D98-D101: trace
bottleneck, Delta_1 LANDMARK, W_{4,2} non-semisimple, W_{5,3} simple, Ext^1
load-bearing, sl_d growth rate, Casimir inversion, traceless hub universality).
D102-D109 additions cover the multi-strand maturation arc (D102-D105: Barrington-
Radical Principle, BFS growth invariance, sign-hash activation theory, Atkinson
sweet spot, sign-rank expansion, branching decomposition, macrame principle,
k-regime theory, 8-strand scaling to XOR8=22266) and the relational computation
arc (D106-D109: topological entropy null, TL visibility filter, axis-alignment
theorem, nesting parity Z/2Z grading, major index bipartite invariant, q-Catalan
identity, orthogonality theorem, dual-channel theorem, Amy bridge to quantum
circuit synthesis, encoding-dependent polarity inversion, Raqiya as reusable
diagnostic tool).*
