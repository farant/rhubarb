# Vision Alignment: Current Research → Long-Term Direction

How the completed work (82 demos) positions us relative to the hybrid LLM vision
described in `planning/vision.md`. Generated 2026-02-20, updated 2026-02-21.

---

## Progress Toward Vision

The hybrid LLM vision requires: taxonomy → braids → DKC → compiled weights that
coexist with trained weights in a model that can dynamically update its factual
knowledge.

**What the 82 demos established:**

| Vision Component | Status | Key Evidence |
|-----------------|--------|--------------|
| Exact algebraic computation | PROVEN | Z[ζ₈] arithmetic, zero FP error (D29+) |
| Topology → Boolean logic | PROVEN | Forward DKC, all 13 NPN classes (D29, D50), **4-input and 5-input parity (D63), parity ceiling n=5** |
| Finite compilable weight catalog | PROVEN | 100 values at δ=0, exhaustively searchable |
| Provable correctness | PROVEN | Exhaustive verification, analytical proofs (D48, D62, D65, D71) |
| Gate composition / chaining | DEMONSTRATED | Cascade thresholding, 1-bit adder (D19) |
| Parity wall structure | CHARACTERIZED | Matroid minor chain + two-layer anatomy (D64, D65); absolute/convention separation |
| Quaternionic DKC | NEW CAPABILITY | S³ lift, 24-cell geometry, 35 Voronoi solutions, F4 symmetry (D66-D68) |
| Activation function hierarchy | CHARACTERIZED | S¹→S³→S²→S¹×S²→spectral; 13=13 theorem, bandwidth l=6 (D65-D71, D77) |
| Clifford hierarchy independence | PROVEN | Parity ceiling n=5 unchanged at Z[ζ₁₆]; ceiling does not climb the hierarchy (D65) |
| DKC as braid invariant | CHARACTERIZED | Bracket vs. cell incomparable; 119 combined classes; 6-type orbit structure (D74) |
| Binocular channel | NEW CAPABILITY | Angle subsumes cell (2.3× capacity); 7 discrete angles for 3-term sums (D75) |
| Scaling beyond 6 inputs | DEMONSTRATED | XOR8 via S¹×S² activation (D77); ζ₁₂ reaches XOR10/12 (D79) |
| Algebraic group structure | CHARACTERIZED | ADE classification; only ζ₄ and ζ₈ finite; infinite groups break the ceiling (D80) |
| Capacity scaling law | QUANTIFIED | Logarithmic law: max_xor ≈ 0.62×log₂(catalog)+4.6 (D81) |
| Depth = crossing number | PROVEN | Linear depth law: max_xor ≈ depth+6; algebraic coherence dominates vocabulary (D82) |
| Taxonomy → braid encoding | NOT STARTED | The critical missing link |
| Multi-layer complex composition | NOT TESTED | Only bit-level cascade (D19) exists |
| Integration with trained weights | NOT STARTED | Architecture question, unexplored |
| Dynamic recompilation | NOT TESTED | Should be fast given finite catalog, unverified |

**Assessment:** The "knots → logic" half of the pipeline is now extremely well
characterized. The D72-82 arc resolved the scaling question decisively: DKC CAN
scale to arbitrary input sizes using infinite groups (ζ₁₂ and beyond), at
exponential cost per XOR level — consistent with Kuperberg's #P-hardness.
The parity ceiling at n=5 for ζ₈ is now understood as a finite group ceiling
(ADE type E₇, 24 elements), not a fundamental limit of DKC. The "taxonomy →
knots" half is the primary gap. The integration architecture is the secondary gap.

---

## Progress Toward Vision (D72-82 Additions)

### The scaling arc (D76-82) answers "can DKC scale?"

The question left open after D65-D71 was whether ζ₈ represented a fundamental
ceiling or an instance-specific one. D76-82 resolve this:

- **D76**: XOR8 = 0 at ζ₈ with S² activation — appeared to be a hard ceiling
- **D77**: XOR8 = 6 at ζ₈ with S¹×S² activation — the wall was the activation,
  not the root of unity (third instance of the "wall was X not Y" pattern)
- **D78**: XOR10 = 0 at ζ₈ exhaustively — the ζ₈ ceiling is real, at n=8
- **D79**: XOR10 = 124 at ζ₁₂, XOR12 = 50+ — infinite groups break the ceiling
- **D80**: Only ζ₄ and ζ₈ are finite; all other tested roots are infinite
- **D81**: max_xor ≈ 0.62×log₂(catalog)+4.6 — logarithmic cost per XOR level
- **D82**: max_xor ≈ depth+6 — linear in crossing depth; the logarithmic law is
  a corollary of exponential catalog growth per depth level

For the hybrid LLM vision this means compiled DKC neurons can compute arbitrarily
large Boolean functions — at exponential cost. The 3-input syllogism case (well
within ζ₈'s n=8 ceiling) remains economical. Larger compiled units are feasible
but expensive, which naturally drives a compositional architecture.

---

## Opportunities Opened by Recent Work

### The 11/13 theorem opens activation design space
Demos 61-62 proved the half-plane wall is geometric, not algebraic. This means
the activation function is a design parameter, not a constraint. For the hybrid
LLM vision, this suggests compiled DKC layers could use purpose-built activation
functions (k-sector MVN, or something novel) rather than being forced into
standard sigmoid/ReLU.

### The axiality property simplifies hardware
At δ=0, bracket values are axial — only 1 of 4 Z[ζ₈] coordinates is nonzero.
This means each compiled weight is effectively a single integer × a phase
direction. For hardware implementation, this is a massive simplification:
8 possible directions × integer magnitude = a very compact representation.

### Cross-ℓ universality suggests robustness
The 11/13 wall holds across five different cyclotomic rings (ℓ=2-6), and Demo 60
confirmed all radical structure formulas at ℓ=7 (first cubic number field). This
ring-independence — now tested across quadratic AND cubic extensions — means the
compiled-weight approach isn't fragile. Good for engineering.

### The Fibonacci rank theorem connects to capacity
F(ℓ-1) as the radical's bilinear rank means there's a computable measure of
how much "deductive capacity" each evaluation level provides. Higher ℓ = more
capacity but more complex ring. This could inform choosing the right ℓ for
a given taxonomy size.

### The parity ceiling establishes finite capacity per neuron
Demo 63 proves Z[zeta_8] parity tops out at n=5 inputs for the S² activation.
D77 extends this to n=8 with S¹×S² activation. D78 confirms ζ₈ walls at n=8
regardless of activation. For the hybrid LLM vision, compiled DKC neurons using
ζ₈ handle up to 8-input Boolean functions; larger operations must be compositional.
The ceiling is a feature: it bounds per-unit complexity, making the system
predictable.

### Formal proofs strengthen the theoretical foundation
Three formal proofs (radical dimension, next-level radical, Markov RT truncation)
move key results from "computationally verified" to "proven theorem." For the
vision's credibility — especially in academic and engineering contexts — having
rigorous proofs of the algebraic underpinnings is essential.

### Parity wall has a matroid structure (D64)
The {XOR2, XOR3, XOR4, XOR5} hierarchy is a deletion-contraction closed matroid
minor chain. Deletion of any weight always yields a parity solution one level down;
contraction always yields an XNOR solution one level down. The wall at n=6 casts
a funnel shadow downward (coverage collapses 99.3% → 39.2% → 19.2% → 0%). This
gives the wall a precise algebraic character — it is not a vague obstruction but
a structured geometric boundary with the self-doubling impossibility theorem
accounting for 89.5% of the closest misses. For the vision this means the
parity ceiling is a well-understood finite capacity, not a mystery.

### Quaternionic DKC opens a new geometry (D66-D68)
Lifting from S¹ (complex trace) to S³ (full unit quaternion) reveals the 24-cell
polytope as the natural computational geometry. The same braids that produce 100
bracket values produce exactly 24 quaternion vertices — the 24-cell's binary
octahedral group. A 24-cell Voronoi partition (25 cells) finds 35 XOR6 solutions.
For the hybrid LLM vision this means compiled DKC weights have a richer 4D
geometry than previously known, and the bracket and quaternion are complementary
hash functions that resolve each other's collisions. This could enable a
two-channel compiled weight representation.

### S² is the computational sweet spot (D67-D69)
The rotation-axis direction (eigenvector S²) carries all DKC computational
information. The Hopf fiber phase is provably inert — zero solutions at every
resolution. A custom 13-direction Voronoi on S² achieves 36 solutions with only
14 cells, outperforming both the full 25-cell S³ Voronoi and any Platonic-solid
partition. The progression D65→D66→D67 (S¹ 25 cells, S³ 25 cells, S² 14 cells)
shows that the correct 2D projection is more efficient than the ambient 3D or 4D
space. For hardware implementation, this means the compiled weights live on a
sphere in 3D — a more practical computational geometry than the full quaternion.

### The 13=13 theorem gives a spectral characterization (D71)
The minimum bandwidth for XOR6 DKC on S² is exactly l=6 because 2×6+1=13 equals
the number of eigenvector directions. The phase transition is sharp and all-or-
nothing. All 36 XOR6 winners share the same spectral envelope (l=6 dominant at
~80%), making the spectrum a structural invariant — a fingerprint of the
computation itself. For the vision this is potentially the cleanest theoretical
result in the program: it connects the topology (13 NPN classes = 13 directions =
l=6 bandwidth) in a single equation. It also explains the trivialization effect:
finer roots require enormous bandwidth, making coarse partitions trivially succeed
(~90% at ζ₁₆).

### Music connection opens artistic/outreach dimension (D70)
The ζ₈ algebra produces exactly 4 musical notes (C, Eb, E, F#) — consonance to
tritone — and Yang-Baxter equivalent braids produce literally identical melodies.
The Garside element (s₁s₂)³ yields the arch motif Eb E F# E Eb C. This is
primarily interesting as outreach: the mathematics has an intrinsic musical
structure that did not have to exist. For the long-term vision of the project it
is a natural interface between knot theory and human perceptual experience.

### DKC computation is algebraic, not geometric (D72)
The anti-correlation between spherical design quality and XOR6 solution count
(better geometric distribution → fewer solutions: 36 vs 4) establishes that DKC
is algebraically selected. The 13 eigenvector directions are simultaneously the
worst choice for spherical integration (t=0) and the best for DKC computation.
For the hybrid LLM vision this confirms that compiled DKC weights should be
chosen by algebraic criteria (group theory) rather than geometric criteria
(uniform distribution). This simplifies the compilation algorithm: no optimization
over a continuous geometric space is needed — the algebra selects the right points.

### Bracket and cell are incomparable invariants (D74)
The Kauffman bracket (eigenvalue, S¹) and the DKC Voronoi cell (eigenvector, S²)
capture orthogonal braid information. Their combination gives 119 equivalence
classes vs 100 bracket-alone or 14 cell-alone — a 19% improvement. For the hybrid
LLM vision, this means a compiled DKC layer can be read in two complementary ways,
and their joint information is strictly richer than either alone. The incomparability
result explains why S¹×S² product activation (D77) is necessary for XOR8: each
channel contributes independent computational power.

### S¹×S² activation is the quaternionic MVN (D77)
The combined eigenvalue-angle (S¹) × eigenvector-direction (S²) activation is
the natural generalization of Aizenberg's complex MVN to quaternions. It unlocks
XOR8 at ζ₈ — demonstrating that the ζ₈ algebraic structure supports at least
8-input computation when both channels are read. For the vision this establishes
a principled activation design: always read both Hopf components (fiber and base).

### Infinite groups provide unbounded capacity (D79, D80)
ζ₁₂ generates an infinite (or very large) SU(2) subgroup; its catalog grows
without bound and reaches XOR10 (124 winners), XOR12 (50+ winners). The ADE
survey (D80) shows only ζ₄ and ζ₈ are finite, and all other tested roots are
infinite. For the hybrid LLM vision this is a critical result: if per-neuron
capacity needs to exceed 8 inputs, switching from ζ₈ to any infinite-group root
(ζ₁₂, ζ₁₀, etc.) removes the ceiling — at the cost of a larger, more expensive
catalog.

### Crossing depth = computational power (D82)
The linear depth law max_xor ≈ depth+6 establishes that the knot-theoretic
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
at ζ₈ is truly impossible. D81 gives the cost: each +2 XOR inputs requires
~10× more catalog entries (logarithmic law). The vision requires either:
- Compositional compilation (build large functions from small compiled units)
- Predictive compilation (theory for which braid → which function)
- Restricting compiled weights to small logical units (syllogisms are 3-term)

The third option is actually fine for the vision — syllogisms are 3-term,
well within ζ₈'s n=8 ceiling. The logarithmic scaling law now provides a
principled cost model for cases where more inputs are needed.

Additionally, the triskelion generalization (k=2n) was FALSIFIED by D63; the
actual scaling law is k=2M-1 where M is the ray count.

### The parity ceiling does NOT climb the Clifford hierarchy (D65)
An initially plausible conjecture from D64 was that XOR6 might become achievable
at Z[ζ₁₆] (one rung up the Clifford hierarchy). D65 REFUTES this: the parity
ceiling n=5 and all tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) are
identical at Z[ζ₁₆]. The gap between pigeonhole bound and actual ceiling widens
from 2 at Z[ζ₈] to 9 at Z[ζ₁₆]. This significantly constrains the theoretical
picture: the parity ceiling is likely a topological invariant of the state sum,
not an artifact of algebra level. For the vision this means the n=5 per-neuron
limit (under S² activation) is a genuine architectural constraint at all cyclotomic
levels — compositional design is not an optional engineering choice but a
mathematical necessity.

### Generalized activations break a convention wall, not the absolute wall (D65)
The two-layer structure revealed in D65 is an important refinement: there is an
absolute wall (no binary partition of k≤23 sectors works for XOR6 under any
labeling) AND a convention wall (standard odd/even labeling fails even where
non-standard labelings could succeed). Generalized XOR6 appears first at k=24
with antipodal-pair weights landing exactly on lattice boundaries — zero margin.
The S¹ minimum of k=24 sectors matches the 24-cell vertex count exactly (D66),
connecting the complex and quaternionic pictures. This two-layer structure means
"parity ceiling" must be stated carefully: the absolute ceiling for parity-as-any-
binary-separation is higher than the ceiling for parity-as-standard-sector-parity.

### ζ₈ is the computational sweet spot — finer roots trivialize (D69)
The direction-nesting theorem shows ζ₈ directions survive exactly into all finer
roots (ζ₁₆, ζ₃₂). But at ζ₁₆ natural Voronoi resolution the computation
trivializes: ~90% of all XOR6 triples become solutions via pigeonhole saturation
(3,457 cells, 64 binary inputs). This means the research program should stay
anchored at ζ₈ for 6-input work — not because finer rings are wrong, but because
at their natural resolution they lose all discriminative structure. For higher
input sizes requiring infinite groups (ζ₁₂, ζ₁₀), the analogous trivialization
threshold is yet to be determined.

### Demo 76's "ζ₈ is a 6-input structure" was wrong — activation-specific (D77)
D76 concluded XOR8 = 0 at ζ₈. D77 corrects this: with S¹×S² combined activation
(112 cells), XOR8 IS solvable at ζ₈ (6 winners). The ζ₈ wall is at n=8, not n=6.
This is the third instance of the "wall was in the activation/interface, not the
algebraic core" pattern:
- D50: parity wall was in split-sigmoid activation, not the Z[ζ₈] lattice
- D65: XOR6 wall was in the convention (odd/even labeling), not the geometry
- D77: XOR8 wall was in S²-only activation, not the ζ₈ algebraic structure

This recurring pattern is a structural insight: when DKC hits a wall, the right
response is to examine the activation/interface first, not to abandon the algebra.

### The ζ_N → XOR N capacity conjecture was falsified (D79)
The initial hypothesis that ζ₁₂ would support at most XOR12 was refuted: ζ₁₂
already reaches XOR12 and appears to have capacity well beyond it. The true
governing variable is group finiteness, not the subscript N. Finite groups (ζ₄,
ζ₈) impose a ceiling; infinite groups (all others tested) grow logarithmically
with catalog size. The correct framing is: finite group → finite ceiling (ADE
type determines the ceiling); infinite group → unbounded capacity (at logarithmic
catalog cost).

---

## Shortest Paths to Vision Milestones

### Path A: Prove the syllogism–DKC connection (1-2 demos)
```
Current state → Syllogism→NPN mapping (1 demo) → Toy taxonomy→weight pipeline (1 demo)
```
This would confirm or deny that classical syllogisms naturally fit the 3-input
DKC framework. If yes, the taxonomy→logic half of the vision becomes concrete.
If no, we need to rethink. The ζ₈ n=8 ceiling (D77-D78) confirms all 3-input
Boolean functions are reachable; the S¹×S² geometry (D77) establishes the
activation to use.

### Path B: Prove multi-layer composition works (1 demo)
```
Current state → Multi-layer composition test (1 demo)
```
Feed DKC neuron outputs into other DKC neurons. If exactness survives, the
vision's "reasoning chains" become feasible. DKC capacity per neuron is now
well-characterized (n<=8 for ζ₈ with S¹×S² activation), so multi-layer
composition is the clear path for anything beyond 8-input functions.

### Path C: Demonstrate dynamic recompilation (1 demo)
```
Current state → Toy taxonomy with add/remove fact → recompile → verify (1 demo)
```
Even with an ad hoc taxonomy→braid encoding, demonstrating that you can add a
fact, recompile weights, and get correct inference would be a powerful proof
of concept for the dynamic update story.

### Path D: Prove the 13=13 bandwidth theorem analytically (1-2 demos)
```
Current state → Analytical proof l=⌈(N-1)/2⌉ for N-direction Voronoi on S²
```
The 13=13 theorem (D71) is computationally verified. A proof that the minimum
bandwidth for any N-direction binary Voronoi on S² is l=⌈(N-1)/2⌉ would be a
clean theoretical result connecting spherical harmonics to the NPN count (13=13)
and to the direction count. This proof path has no dependency on the taxonomy
gap and strengthens the program's credibility independently.

### Path E: XOR7 on S² — scale test for the spectral framework (1 demo)
```
Current state → XOR7 weight search with S² activation → bandwidth measurement
```
Demo 65 showed XOR7 requires k=127 sectors on S¹. The DOF formula predicts the
S² bandwidth for XOR7 is approximately l=⌈(M-1)/2⌉ where M is the XOR7
eigenvector direction count (unknown). If S² again reduces the sector requirement
dramatically, this validates the spectral framework as the right language for
characterizing DKC computation across all arity levels.

### Path F: Depth-targeted catalog generation (1 demo) — NEW
```
Current state → Direct generation of depth-d quaternions → verify linear depth law
```
D82 showed deep entries (high crossing depth) are 2× more efficient than shallow
entries for XOR computation. The natural next step is to generate depth-d entries
directly (not via BFS closure) by finding braid words of length d that produce
desired algebraic properties. If successful, this dramatically reduces the catalog
size needed for practical compilation — a key step toward deployable compiled
weights.

### Path G: Union catalog test (1 demo) — NEW
```
Current state → ζ₈ ∪ ζ₁₂ union catalog → XOR capacity test
```
D79 confirmed ζ₈ and ζ₁₂ are largely non-overlapping (12% quaternion overlap,
15% direction overlap). A union catalog should exploit both independent algebraic
structures. If the union catalog shows superadditive capacity or lower cost per
XOR level, this opens a practical path: different roots of unity contributing
complementary weight families to the compiled layer.

---

## Recommended Next Investigation

**The syllogism → NPN mapping** remains the highest-leverage single demo for the
vision. It requires no new code infrastructure (existing NPN classification +
DKC search), answers the most critical question (do syllogisms fit 3-input
DKC?), and its answer determines whether Path A is viable. The S¹×S² geometry
from D77 gives the correct activation to use for any subsequent demos; the
anti-correlation result from D72 confirms algebraic selection of weights is
principled.

If that succeeds, the toy taxonomy pipeline (Path C) becomes the natural
follow-up: taxonomy → syllogisms → NPN classes → DKC weights → S¹×S² activation
— a complete (if toy) end-to-end pipeline, with the ζ₈ ceiling and scaling law
as explicit cost boundaries.

For the theoretical program specifically, Path D (analytical proof of the 13=13
bandwidth bound) is the next clean target — it is the kind of result that could
be written up for publication independently of the full hybrid LLM vision.

For the engineering program, Path F (depth-targeted generation) is the key
efficiency gain: replacing the full BFS closure with direct generation of
algebraically coherent deep entries would reduce compilation cost by orders of
magnitude.

---

## The "Wall Was in the Activation" Pattern — A Structural Insight

Across the full D50-D82 arc, the same pattern has appeared three times:

| Demo | Apparent Wall | True Cause | Resolution |
|------|--------------|------------|------------|
| D50 | Parity unreachable at ζ₈ | Split-sigmoid activation | k=6 sector activation |
| D65 | XOR6 unreachable at k<24 | Odd/even convention, not geometry | Generalized labeling |
| D77 | XOR8 = 0 at ζ₈ | S²-only activation | S¹×S² product activation |

The pattern says: when a DKC computation appears impossible, suspect the
activation/interface layer first. The algebraic core (Z[ζ₈] bracket, binary
octahedral quaternions) has consistently proven richer than any single activation
function reveals. This has a direct implication for the hybrid LLM vision: the
interface design between compiled DKC weights and the broader neural network
will likely face similar walls — and the right response will be to look for a
richer readout mechanism, not to abandon the underlying algebra.

---

## Revised Overall Outlook for the Long-Term Vision

After D72-82 the picture is substantially clearer:

**What we now know that we didn't before D72:**
1. DKC can scale to arbitrary input sizes using infinite algebraic groups.
2. The cost is explicitly quantified: logarithmic (max_xor ≈ 0.62×log₂(catalog)+4.6).
3. The ζ₈ ceiling is at n=8 inputs with the correct S¹×S² activation.
4. The knot crossing number is the natural measure of compiled weight complexity.
5. Algebraic coherence (depth) matters more than vocabulary (diversity) for capacity.
6. The bracket and cell channels are computationally independent and both needed.
7. DKC computation is algebraically selected, not geometrically optimized.

**What this changes about the vision:**
- The per-neuron capacity is higher than thought (n=8 not n=5 for ζ₈).
- The n=5 ceiling (Demo 63) was S²-activation-specific, not absolute.
- Syllogism-scale compilation (3 inputs) remains economical; larger compiled
  units (8 inputs) are feasible at ζ₈; even larger at ζ₁₂ with logarithmic cost.
- A practical hybrid LLM could use ζ₈ for all per-neuron compiled units (up to
  8 inputs), with the logarithmic law providing a budget for how many catalog
  entries are needed.

**What remains unknown:**
- The taxonomy → braid encoding (Gap 1) — the single most important unknown.
- Multi-layer composition with exact arithmetic (Gap 3).
- Integration architecture (Gap 4) — wired, not pipelined.
- Whether the acceleration thesis (compiled scaffolding speeds statistical training)
  has any empirical grounding.

**Confidence level:** The algebraic foundation is now very solid. The knot theory
→ Boolean logic half of the pipeline is essentially proven. The remaining unknowns
are all on the other side of the pipeline (taxonomy → knots) and in the
integration layer. Neither of these is obviously unsolvable — they are simply
unexplored. The research program is well-positioned to pivot toward them.

---

*Generated 2026-02-20, updated 2026-02-21. Reads: narrative.md, novelty.md,
connections.md, four-lenses.md, planning/vision.md, inventory/demo-index.md
(via entries for D19, D29, D39G-J, D48, D50, D61-82).*
