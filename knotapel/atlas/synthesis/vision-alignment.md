# Vision Alignment: Current Research → Long-Term Direction

How the completed work (94 demos) positions us relative to the hybrid LLM vision
described in `planning/vision.md`. Generated 2026-02-20, updated 2026-02-24.

---

## Progress Toward Vision

The hybrid LLM vision requires: taxonomy → braids → DKC → compiled weights that
coexist with trained weights in a model that can dynamically update its factual
knowledge.

**What the 94 demos established:**

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
| Taxonomy → braid encoding | NOT STARTED | The critical missing link |
| Multi-layer complex composition | NOT TESTED | Only bit-level cascade (D19) exists |
| Integration with trained weights | NOT STARTED | Architecture question, unexplored |
| Dynamic recompilation | NOT TESTED | Should be fast given finite catalog, unverified |

**Assessment:** The "knots → logic" half of the pipeline is now extremely well
characterized AND increasingly well understood mechanistically. The D85-92 arc
shifted the research from "what can DKC do?" to "why does it work and what are
its structural limits?" The parity-lock theorem (D92) is the first formally
PROVEN result with direct implications for encoding design — it establishes that
the +/-q encoding is structurally locked to parity, and any non-parity function
requires a different encoding (the 1wpi encoding from D48/D50 already computes
all 13 NPN classes). The depth law is now fully explained (D89-D91): the
mechanism is relational (axis cancellation + cross-depth algebraic constraints),
the wall is parity itself (not the activation), and the slope comes from
balanced exponentials (BFS vocabulary ~2x/round vs parity demand 4x/weight).
The "taxonomy → knots" half remains the primary gap. The encoding design
question (D92) is the new secondary gap — more fundamental than activation
choice because encoding determines WHICH functions are computable at all.

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

---

## The "Wall Was in the Activation" Pattern — A Structural Insight

Across the full D50-D92 arc, the same pattern has appeared three times for
activation, and a fourth time for encoding:

| Demo | Apparent Wall | True Cause | Resolution |
|------|--------------|------------|------------|
| D50 | Parity unreachable at zeta_8 | Split-sigmoid activation | k=6 sector activation |
| D65 | XOR6 unreachable at k<24 | Odd/even convention, not geometry | Generalized labeling |
| D77 | XOR8 = 0 at zeta_8 | S^2-only activation | S^1xS^2 product activation |
| D92 | Only XOR/XNOR computable | +/-q encoding, not DKC itself | 1wpi encoding (D48/D50) |

The pattern generalizes: when a DKC computation appears impossible, suspect the
interface layer first — activation, convention, or encoding. The algebraic core
(Z[zeta_8] bracket, binary octahedral quaternions) has consistently proven richer
than any single interface reveals. D92 extends this pattern from the activation
layer to the encoding layer: the +/-q encoding was the wall, not DKC itself.

This has a direct implication for the hybrid LLM vision: the interface design
between compiled DKC weights and the broader neural network will face similar
walls — and the right response will be to look for a richer encoding/readout
mechanism, not to abandon the underlying algebra.

---

## Revised Overall Outlook for the Long-Term Vision

After D85-92 the picture adds mechanistic depth to the scaling characterization
from D72-82:

**What we now know that we didn't before D85:**
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

**What this changes about the vision:**
- The encoding is the first design decision. It determines which Boolean functions
  the compiled neuron can compute at all. The +/-q encoding specializes in parity
  (the hardest function); the 1wpi encoding handles all 13 NPN classes. A
  practical hybrid LLM needs the 1wpi encoding or a successor for syllogistic
  logic (which requires AND, OR, implication — not just XOR).
- Catalog optimization should target relational quality (axis cancellation,
  cross-depth constraints), not positional quality (coverage, spacing, spherical
  design). The spectral inversion result (D90) proves positional optimization
  is wrong in principle.
- Design principles are regime-dependent. Finite-group compiled neurons (zeta_8)
  and infinite-group compiled neurons (zeta_12) have different null-state
  requirements, different dominant pathways (shadow pairing vs direction
  diversity), and different robustness profiles.
- The depth law is now mechanistically understood, not just empirically observed.
  This means it can be potentially manipulated: a different BFS branching factor
  (different group) or a relational activation (explicitly using axis dot
  products) might change the slope.

**What remains unknown:**
- The taxonomy -> braid encoding (Gap 1) — the single most important unknown.
- Whether the 1wpi encoding has a depth law (Gap 10) — if not, the compilation
  cost model for non-parity functions is completely open.
- Multi-layer composition with exact arithmetic (Gap 3).
- Integration architecture (Gap 4) — wired, not pipelined.
- Whether the acceleration thesis (compiled scaffolding speeds statistical training)
  has any empirical grounding.
- Encoding design for targeting specific Boolean functions beyond parity (Gap 10).

**Confidence level:** The algebraic foundation is extremely solid and now
mechanistically understood. The knot theory -> Boolean logic half of the pipeline
is essentially proven for parity functions under the +/-q encoding. The parity-
lock theorem (D92) establishes that broadening to non-parity functions is an
encoding design problem, not a fundamental limitation. The remaining unknowns
are: (1) the taxonomy -> knots mapping, (2) the 1wpi encoding's scaling behavior,
and (3) the integration layer. The first is the bottleneck for the vision. The
second is newly identified by D92 and is the most urgent theoretical question.
The third is an engineering challenge that can wait until the first two are
resolved.

---

*Generated 2026-02-20, updated 2026-02-23. Reads: narrative.md, novelty.md,
connections.md, four-lenses.md, planning/vision.md, inventory/demo-index.md
(via entries for D19, D29, D39G-J, D48, D50, D61-92).*
