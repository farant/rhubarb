# Vision Alignment: Current Research → Long-Term Direction

How the completed work (59 demos) positions us relative to the hybrid LLM vision
described in `planning/vision.md`. Generated 2026-02-20, updated 2026-02-21.

---

## Progress Toward Vision

The hybrid LLM vision requires: taxonomy → braids → DKC → compiled weights that
coexist with trained weights in a model that can dynamically update its factual
knowledge.

**What the 59 demos established:**

| Vision Component | Status | Key Evidence |
|-----------------|--------|--------------|
| Exact algebraic computation | PROVEN | Z[ζ₈] arithmetic, zero FP error (D29+) |
| Topology → Boolean logic | PROVEN | Forward DKC, all 13 NPN classes (D29, D50), **4-input and 5-input parity (D63), parity ceiling n=5** |
| Finite compilable weight catalog | PROVEN | 100 values at δ=0, exhaustively searchable |
| Provable correctness | PROVEN | Exhaustive verification, analytical proofs (D48, D62, D65, D71) |
| Gate composition / chaining | DEMONSTRATED | Cascade thresholding, 1-bit adder (D19) |
| Parity wall structure | CHARACTERIZED | Matroid minor chain + two-layer anatomy (D64, D65); absolute/convention separation |
| Quaternionic DKC | NEW CAPABILITY | S³ lift, 24-cell geometry, 35 Voronoi solutions, F4 symmetry (D66-D68) |
| Activation function hierarchy | CHARACTERIZED | S¹→S³→S²→spectral; 13=13 theorem, bandwidth l=6 (D65-D71) |
| Clifford hierarchy independence | PROVEN | Parity ceiling n=5 unchanged at Z[ζ₁₆]; ceiling does not climb the hierarchy (D65) |
| Taxonomy → braid encoding | NOT STARTED | The critical missing link |
| Multi-layer complex composition | NOT TESTED | Only bit-level cascade (D19) exists |
| Integration with trained weights | NOT STARTED | Architecture question, unexplored |
| Dynamic recompilation | NOT TESTED | Should be fast given finite catalog, unverified |

**Assessment:** The "knots → logic" half of the pipeline is solid and now much better
characterized — the activation function design space is mapped, the parity wall is
understood at depth, and quaternionic DKC opens a new geometry. The "taxonomy →
knots" half is the primary gap. The integration architecture is the secondary gap.

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

### The parity ceiling establishes finite capacity
Demo 63 proves Z[zeta_8] parity tops out at n=5 inputs. For the hybrid LLM vision,
this means individual DKC neurons handle at most 5-input parity functions. Larger
logical operations must be built compositionally — which aligns with the syllogism
approach (3 terms per syllogism, well within the n=5 ceiling). The ceiling is a
feature, not a limitation: it bounds the complexity of each compiled unit, making
the system predictable.

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

### Exhaustive search doesn't scale — but has a proven ceiling
The current "compiler" searches |catalog|^(n+1) combinations. D63 shows
exhaustive search is feasible up to n=5 at k=15 sectors, but n=6+ is truly
impossible — not just intractable but zero solutions exist (the parity ceiling).
The vision requires either:
- Compositional compilation (build large functions from small compiled units)
- Predictive compilation (theory for which braid → which function)
- Restricting compiled weights to small logical units (syllogisms are 3-term)

The third option is actually fine for the vision — if compiled weights encode
individual syllogisms (3 terms each), and reasoning chains are built by
composition, then the 3-input scale is sufficient per unit.

Additionally, the triskelion generalization (k=2n) was FALSIFIED by D63; the
actual scaling law is k=2M-1 where M is the ray count. This is an important
course correction — the real relationship is to lattice geometry, not input count.

### The parity ceiling does NOT climb the Clifford hierarchy (D65)
An initially plausible conjecture from D64 was that XOR6 might become achievable
at Z[ζ₁₆] (one rung up the Clifford hierarchy). D65 REFUTES this: the parity
ceiling n=5 and all tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) are
identical at Z[ζ₁₆]. The gap between pigeonhole bound and actual ceiling widens
from 2 at Z[ζ₈] to 9 at Z[ζ₁₆]. This significantly constrains the theoretical
picture: the parity ceiling is likely a topological invariant of the state sum,
not an artifact of algebra level. For the vision this means the n=5 per-neuron
limit is a genuine architectural constraint at all cyclotomic levels — compositional
design is not an optional engineering choice but a mathematical necessity.

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
anchored at ζ₈ — not because finer rings are wrong, but because at their natural
resolution they lose all discriminative structure. The computational richness of
ζ₈ comes precisely from its coarseness: 24 quaternions, 13 directions, 14 cells.

---

## Shortest Paths to Vision Milestones

### Path A: Prove the syllogism–DKC connection (1-2 demos)
```
Current state → Syllogism→NPN mapping (1 demo) → Toy taxonomy→weight pipeline (1 demo)
```
This would confirm or deny that classical syllogisms naturally fit the 3-input
DKC framework. If yes, the taxonomy→logic half of the vision becomes concrete.
If no, we need to rethink. The parity ceiling result (D63) gives confidence: any
3-input Boolean function is reachable. The S² geometry (D67) gives a cleaner
picture of what the activation looks like geometrically for 3-input syllogisms.

### Path B: Prove multi-layer composition works (1 demo)
```
Current state → Multi-layer composition test (1 demo)
```
Feed DKC neuron outputs into other DKC neurons. If exactness survives, the
vision's "reasoning chains" become feasible. If not, cascade thresholding
(D19 style, collapse to bits between layers) is the fallback — still works,
just less elegant. DKC capacity per neuron is now well-characterized (n<=5
for parity in Z[zeta_8]), so multi-layer composition is the clear path for
anything beyond 5-input functions.

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

---

## Recommended Next Investigation

**The syllogism → NPN mapping** remains the highest-leverage single demo for the
vision. It requires no new code infrastructure (existing NPN classification +
DKC search), answers the most critical question (do syllogisms fit 3-input
DKC?), and its answer determines whether Path A is viable. The S² geometry
from D67 gives a clean geometric picture of what the compiled weight space looks
like for 3-input syllogisms.

If that succeeds, the toy taxonomy pipeline (Path C) becomes the natural
follow-up: taxonomy → syllogisms → NPN classes → DKC weights → S² Voronoi
activation — a complete (if toy) end-to-end pipeline, now with a richer
geometric understanding of the activation layer than was available before D66-D71.

For the theoretical program specifically, Path D (analytical proof of the 13=13
bandwidth bound) is the next clean target — it is the kind of result that could
be written up for publication independently of the full hybrid LLM vision.

---

*Generated 2026-02-20, updated 2026-02-21. Reads: narrative.md, novelty.md,
connections.md, four-lenses.md, planning/vision.md, inventory/demo-index.md
(via entries for D19, D29, D39G-J, D48, D50, D61-71).*
