# The Five Pillars: Habiro / Nazer-Gastpar / Abramsky / Aizenberg / Reservoir Computing

The DKC thesis lives at an unoccupied intersection of five established fields. Nobody in the literature connects these results. Each pillar is well-established in its own domain; the novelty is assembling them into the forward DKC construction.

*Updated 2026-02-21: Reservoir Computing added as Pillar 5, motivated by the scaling arc (D73, D76-D82). The braid-as-reservoir connection gives DKC its temporal/dynamical dimension.*

*Updated 2026-02-23: Demos 85-92 added. D85-86 connect to Pillar 3 (TL non-semisimplicity deepened via indecomposability parameter). D87-88-90 refine Pillar 5 (null dispensability regime transition, relational-not-positional mechanism). D89-91 confirm balanced exponentials (supply/demand linearity). D92 adds circuit complexity connection (AC^0 parity lower bounds).*

*Updated 2026-02-26: Demos 102-109 added. D102 connects Pillar 3 to Barrington's theorem (radical carries abelian character, provably useless for parity). D103-105 deepen Pillar 5 (BFS growth is braid group invariant, sign-hash = 1-bit compressed sensing, macrame principle for branching interaction). D106 strengthens Pillar 5 via negative: topological entropy is orthogonal to DKC computation (TL visibility filter). D108-109 add dual-channel theorem connecting Pillar 2 (lattice product closure) and Pillar 4 (activation/magnitude diversity) with quantum circuit synthesis bridge (Amy-Glaudell-Ross 2023).*

---

## Pillar 1: Habiro (2002) — Bracket Values Are Cyclotomic Integers

**Paper:** "On the quantum sl_2 invariants of knots and integral homology spheres"
**Journal:** Geometry & Topology Monographs
**Link:** [arXiv:math/0209324](https://arxiv.org/abs/math/0209324)

**What Habiro proved:** The colored Jones polynomial has a "cyclotomic expansion" — it can be written as a sum over cyclotomic polynomials with integer coefficients. This means that evaluating the Jones polynomial (or equivalently, the Kauffman bracket) at any root of unity A = e^{iπ(2k+1)/2ℓ} necessarily produces a value in the cyclotomic integer ring Z[ζ_{2ℓ}].

**What this gives DKC:** The mathematical guarantee that our bracket catalog is a catalog of ALGEBRAIC INTEGERS, not floating-point approximations. At A = e^{i5π/4} (our primary case), all bracket values are elements of Z[ζ₈] = {a + bζ₈ + cζ₈² + dζ₈³ : a,b,c,d ∈ Z}. This ring has class number 1 (unique factorization domain), making exact arithmetic straightforward.

**The specific instance:** Habiro's integrality theorem is general; our Demo 29 implements it for the specific case ℓ=2 (8th root of unity, δ=0). The 64-element catalog of distinct bracket values is a finite subset of Z[ζ₈], computed by exact integer operations on 4-tuples — zero floating-point error.

**Strengthened by:** Costello-Francis-Gwilliam (2026, arXiv:2602.12412) proving Witten = RT, which means the bracket values are not just algebraic integers but rigorously quantum field theory transition amplitudes.

**Which demos use it:** D29 (forward DKC, exact Z[ζ₈] arithmetic), D35 (TL matrices over Z[ζ₈]), D48 (exhaustive 100M quartet search with exact arithmetic), D49 (neglecton weights in Z[ζ₈]), D50 (parity reachability with Z[ζ₈] catalog), D53 (Z[ω] at ℓ=3), D54 (Z[ζ₁₆] at ℓ=4), D58 (Z[ζ₅] at ℓ=5), D59 (Z[ζ₂₄] at ℓ=6), D85 (delta-parameterized b extraction uses Z[ζ₈] infrastructure), D87 (zeta_8 vs zeta_12 null fraction), D90 (algebraic substrate for cross-depth constraints), D92 (weight catalog for parity-lock analysis), D102 (6-strand exact Z[ζ₈] matrix representation), D105 (8-strand exact Z[ζ₈] matrices), D107 (186 distinct Z[ζ₈] values, axis-alignment theorem), D108 (parity vocabulary graph analysis on Z[ζ₈] catalog), D109 (Z[ζ₁₆] at δ=√2)

---

## Pillar 2: Nazer-Gastpar (2011) — Algebraic Integer Lattices Compute Functions

**Paper:** "Compute-and-Forward: Harnessing Interference Rather Than Treating It as Noise"
**Journal:** IEEE Transactions on Information Theory
**Link:** [arXiv:1107.0300](https://arxiv.org/pdf/1107.0300)

**Also:** Tunali et al. (2015), "Lattices over Eisenstein Integers for Compute-and-Forward" — extends to Z[ω] (Eisenstein integers). [arXiv:1404.1312](https://arxiv.org/abs/1404.1312)

**What Nazer-Gastpar proved:** In a wireless relay network, relays should compute INTEGER LINEAR COMBINATIONS of codewords rather than decode individual messages. Lattice codes (codewords closed under integer linear combinations) naturally support this "compute-and-forward" paradigm. The algebraic structure of the lattice does the computational heavy lifting.

**Key extension (Tunali et al.):** Replacing Z^n with Z[ω] (Eisenstein integers, the ring of integers of Q(ζ₃)) gives BETTER codes. The algebraic integer lattice structure provides natural error correction beyond what ordinary integer lattices offer.

**What this gives DKC:** The information-theoretic frame. Forward DKC proposes the same principle applied to neural networks: Z[ζ₈] lattice points as weights, with the algebraic ring structure ensuring exact computation. The difference: compute-and-forward computes LINEAR functions at relays; DKC computes NONLINEAR functions (XOR, parity) through bracket evaluation. But the foundational insight is shared — algebraic integer rings enable computation through their ring structure, not through optimization.

**The specific analogy:**
- Compute-and-forward: channel noise → lattice decoding → exact linear function
- Forward DKC: bracket evaluation → cyclotomic lattice → exact Boolean function
- Both: the algebraic closure properties of the ring guarantee correctness

**Which demos use it:** D29 (bracket as lattice sum), D46 (coding theory capacity analysis: ~7.3 bits/symbol in Z[ζ₈]), D47 (two-channel coding thesis), D48 (100M quartet search uses lattice structure), D55 (DKC at ℓ=4 with Z[ζ₁₆] lattice)

---

## Pillar 3: Abramsky (2007) — TL Diagrams Encode Computation

**Paper:** "Temperley-Lieb Algebra: From Knot Theory to Logic and Computation via Quantum Mechanics"
**Link:** [arXiv:0910.2737](https://arxiv.org/abs/0910.2737)

**What Abramsky proved:** The Temperley-Lieb algebra connects knot theory, categorical quantum mechanics, and logic/computation. Abramsky gives a "fully abstract presentation" of the TL category using Geometry of Interaction, and introduces a "planar lambda calculus" interpreted in TL algebras. TL diagrams are a natural computational calculus — composition of diagrams IS composition of computations.

**What this gives DKC:** The theoretical bridge from topology to computation. Abramsky proved that TL diagrams can encode computation. The bracket state sum evaluates TL diagrams. Therefore bracket values are not just topological invariants — they are evaluations of a computational calculus. Forward DKC's step from "TL diagrams encode computation" to "bracket values of TL diagrams can be compiled into NN weights" is our specific contribution building on Abramsky's bridge.

**Demo 35 implements Abramsky directly:** General TL_n matrices over Cyc8, verified for n=2-5. The matrix representation of TL generators IS Abramsky's computational primitives, made concrete in C89 exact arithmetic.

**Demos 85-86 deepen the non-semisimple structure:** The indecomposability parameter b = -5/8 (Pearce-Rasmussen) computed via delta-parameterized forms on the TL_4 regular representation. This connects to the LCFT structure constants that govern how TL modules compose — the non-semisimplicity that Abramsky's framework predicts but does not explicitly compute. The b parameter quantifies the "computational obstruction" in the TL radical: it measures the coupling between head and socle of projective indecomposable modules, which is the algebraic mechanism behind DKC's parity wall (D47-50).

**Demos 102 deepens the non-semisimple structure further:** The Barrington-Radical Principle — the radical of W_{6,4} carries an abelian character (the writhe homomorphism B_n → Z/8Z), and by Barrington's theorem abelian groups cannot compute parity. This PROVES that the radical direction is computationally inert for XOR, closing the loop from D47's hypothesis (non-semisimplicity limits computation) through D85's quantification (b = -5/8) to D102's proof (radical ≡ abelian ≡ useless by Barrington).

**Demo 107 provides the algebraic fine structure:** Universal axis-alignment at δ=0 is a provable theorem: every braid matrix entry is n·ζ₈^k (single axis). The Z/4Z phase formula reduces DKC at δ=0 to integer path counting + three-variable phase lookup. The nesting parity Z/2Z grading is novel (confirmed vs Plaza-Ryom-Hansen 2013 KLR grading).

**Which demos use it:** D27 (reverse DKC conceptual framework), D29 (forward DKC), D35 (TL matrices as computational engine), D47-50 (TL non-semisimplicity as parity barrier), D51-52 (radical anatomy — the algebraic obstruction theory), D85-86 (indecomposability parameter — quantifying the non-semisimple coupling), D102 (Barrington-Radical Principle — abelian radical provably useless), D107 (Z/4Z axis-alignment theorem, nesting parity Z/2Z grading)

**The formal chain:**
1. Abramsky: TL diagrams = computation
2. Our bracket evaluation: TL computation → Z[ζ₈] values (via Habiro)
3. Forward DKC: Z[ζ₈] values → Boolean function weights

---

## Pillar 4: Aizenberg (2000) — Roots-of-Unity Neurons for Discrete Logic

**Paper:** "Multi-Valued and Universal Binary Neurons: Theory, Learning and Applications"
**Authors:** Igor Aizenberg, Naum N. Aizenberg, Joos P.L. Vandewalle
**Publisher:** Kluwer Academic Publishers
**Link:** [Springer](https://link.springer.com/book/10.1007/978-1-4757-3115-6)

**Also:** Aizenberg (2011), "Complex-Valued Neural Networks with Multi-Valued Neurons" (Springer).

**What Aizenberg proved:** The values of k-valued logic are naturally encoded by the k-th roots of unity (ε_j = e^{2πij/k}). A single multi-valued neuron (MVN) with activation function mapping the complex plane into k angular sectors can implement arbitrary k-valued threshold functions. The MVN uses a non-gradient learning rule: simple rotation along the unit circle.

**What this gives DKC:** The neural network architecture. Aizenberg's MVN PRESCRIBES root-of-unity structure — you design the neuron to use k sectors. Forward DKC DISCOVERS that bracket values (which are cyclotomic integers, sums of roots of unity) naturally compute Boolean functions when used as MVN-style weights. The key difference:
- Aizenberg: Design the activation → roots of unity follow
- Forward DKC: Topology provides the weights → activation choice selects which functions are computable

**The Demo 50 result:** k=6 MVN sectors make ALL 13 non-trivial NPN classes computable from Z[ζ₈] bracket values, including parity (906 solutions). This is Aizenberg's architecture populated with Habiro's values, computing functions that Abramsky's framework predicts, over lattices that Nazer-Gastpar's theory validates.

**Key extension — Aizenberg (2008):** "Solving the XOR and parity N problems using a single universal binary neuron." *Soft Computing*, 12(3). This paper proved parity solvable with a single UBN, and its weight construction (w₁=ζ₈, w₂=i, w₃=−1) lives IN the Z[ζ₈] bracket catalog — confirming that the lattice contains parity-capable weights even though split-sigmoid cannot access them.

**Demo 91 confirmation:** The k-sector MVN activation is NOT the bottleneck for the depth law. Varying k_sec from 2 to 48 does not shift the depth gap between XOR levels — finer resolution reveals more solutions but not higher arity. The parity constraint itself is the wall, and the activation resolution only controls how many solutions are visible. This confirms Aizenberg's architecture as sufficient but not limiting.

**Demo 92 parity-lock:** The +/-q input encoding concentrates all computational power on parity — the single Boolean function outside AC^0 (Furst-Saxe-Sipser 1984, Hastad 1987). Under the 1-weight-per-input encoding (D48/D50), all 13 NPN classes including parity are achievable. The encoding-dependence of function accessibility is a refinement of Aizenberg's general MVN theory: the activation architecture (Aizenberg) determines what functions ARE computable in principle; the encoding determines which subset is ACCESSIBLE from a given weight catalog.

**Demo 108 dual-channel theorem:** Graph structure PREDICTS parity capability. Product closure (multiplicative phase coherence) and additive v₂ connectivity (magnitude diversity) are both independently necessary for parity. This maps onto the T-gate/Hadamard decomposition of Amy-Glaudell-Ross (2023) for quantum circuit synthesis over Z[ζ₈]. The activation function plays the Hadamard role: providing magnitude diversity that enables "Clifford-level" values to participate in parity computation.

**Demo 109 encoding dependence:** At δ=√2 (Z[ζ₁₆]), parity and non-parity vocabularies are structurally identical on 7/8 edge types. Product closure is the sole discriminator, but with INVERTED polarity from D108: parity wants LOW product closure under multiplicative encoding (products must escape vocabulary for sector alternation). The dual-channel theorem holds in both regimes but the sign of the multiplicative channel is encoding-sensitive.

**Which demos use it:** D45 (NPN classification with complex neurons), D47 (MVN activation comparison, topology-frequency correlation reversal), D48 (parity wall under split-sigmoid), D49 (neglecton weights — lattice contains parity weights), D50 (k-sector MVN resolves parity at k=6), D91 (activation not the bottleneck — k_sec invariance), D92 (encoding-dependent function selection), D108 (dual-channel theorem — graph structure predicts parity), D109 (encoding-dependent polarity inversion at δ=√2)

---

## Pillar 5: Reservoir Computing (Jaeger 2001 / Maass 2002) — Fixed Dynamical Systems Compute Through Their Dynamics

**Papers:**
- Jaeger (2001), "The 'echo state' approach to analysing and training recurrent neural networks." GMD Technical Report 148.
- Maass, Natschläger, Markram (2002), "Real-time computing without stable states: A new framework for neural computation based on perturbations." *Neural Computation*, 14(11).
- Lukoševičius & Jaeger (2009), "Reservoir computing approaches to recurrent neural network training." *Computer Science Review*, 3(3).

**What reservoir computing established:** A fixed, untrained dynamical system (the "reservoir") processes sequential input through its internal dynamics. Only a simple readout layer is trained. The reservoir provides a high-dimensional nonlinear mapping of input sequences into a state space where linear readout suffices. The key insight: computational power comes from the STRUCTURE of the dynamics, not from optimization of the dynamics.

**What this gives DKC:** The temporal/dynamical dimension that the other four pillars lack. A braid word IS a sequential input: each crossing σ_i is a time step, and the SU(2) representation processes crossings one at a time through matrix multiplication. The quaternion at the end of the braid word is the reservoir state. The Voronoi cell / sector activation is the readout.

**The specific connections:**

- **D73 (Eigenvector Automaton) IS reservoir dynamics.** Crossing-by-crossing quaternion multiplication tracks Voronoi cell transitions. The 82.8% universal determinism IS the echo state property — the reservoir state is mostly determined by recent input, with 17.2% sensitivity to deeper history.
- **D82 (Crossing Depth) IS a reservoir memory capacity result.** max_xor ≈ depth+6 says computational power scales linearly with the number of dynamical steps through the reservoir. This is exactly what RC theory predicts: more time steps = richer transient dynamics = more computational separation between input classes.
- **D82's "algebraic coherence beats vocabulary"** is a reservoir quality result. Deep entries (many generator multiplications = many reservoir steps) with shared intermediate products outperform diverse entries (many distinct directions) with no shared structure. In RC terms: a reservoir with coherent internal dynamics computes more than a reservoir with maximal state diversity.

**Demos 102-106 extend the RC connection:**

- **D102 (Barrington-Radical Principle):** The radical direction carries an abelian character (writhe), which by Barrington's theorem is provably useless for parity. This is an algebraic reservoir quality result: the radical is a "frozen" 1-dimensional sub-reservoir that only tracks framing data. The simple module W_{6,0} outperforms the non-simple W_{6,4} at every XOR level because its entire 5-dimensional state space participates in computation, while W_{6,4} wastes one dimension on the frozen radical.
- **D103 (BFS Growth Invariant):** BFS catalog growth is identical across all modules of the same braid group B_n (W_{6,0}, W_{6,2}, W_{6,4} all produce the same depth profile). Growth rate ≈ (n-1)x converges to the sl_{n-1} functor thesis. In RC terms: the reservoir dynamics are determined by the input group (B_n), not the output representation. The readout (activation function) is fully separable from the reservoir.
- **D104 (Sign-Hash = 1-Bit CS):** Sign-hash activation is 1-bit compressed sensing (Boufounos-Baraniuk 2008). Cross-block activation outperforms within-block at n=6 because branching cross-terms have low intra-block coherence (the CS incoherence condition). Sign quantization can EXPAND effective rank (244→292 for W_{6,2}). The Atkinson sweet spot (~120 of 324 components) is the optimal information-per-collision tradeoff. But k (cell count) is the real scaling lever, not hash architecture.
- **D105 (Macrame Principle):** Branching interaction REVERSES at n=8: within-block dominates (dim-14 blocks are self-sufficient). Regime transition between dim-5 (cross-block wins) and dim-14 (within-block wins). The D93 "XOR dies at N≥7" is REFUTED as a k-regime artifact: at k=4096, n=8 beats n=6.
- **D106 (Topological Entropy Null):** STRONGEST negative result for the RC analogy. Topological entropy (Thurston-Nielsen classification) is completely orthogonal to DKC computation. Both periodic and pseudo-Anosov braids compute XOR at 100% participation. The TL quotient at δ=0 erases dynamical information (e_i²=0 kills expanding eigenvalues) while preserving algebraic structure (writhe). DKC computation is collective/algebraic, not individual/dynamical — sharpening the relational computation thesis.

**Demos 87-88-89-90 refine the RC connection:**

- **D87 (Null Dispensability):** Null reservoir states (bracket-null entries) are indispensable at finite groups (zeta_8) but dispensable at infinite groups (zeta_12). The regime transition — from null-critical to null-redundant — maps to RC capacity theory: when the reservoir is small (finite group, 24 states), every state matters including null ones that maintain the manifold; when the reservoir is large (infinite group, 4096+ states), null states become redundant because non-null states alone provide sufficient dimensionality. This refines the "logarithmic partner hypothesis" from D84.
- **D88 (Anti-Correlation):** The directions worst for spherical integration (design residual) are best for computation (XOR capacity). In RC terms: the reservoir state distribution that maximizes the separation property (computational utility) is NOT the distribution that maximizes entropy (uniform coverage). The 4 non-null body-diagonal directions are the rigid computational anchors; the 6 null-only edge-midpoint directions are flexible scaffolding. This is a concrete instance of the RC no-free-lunch theorem applied to the DKC braid reservoir.
- **D89 (Depth Law Mechanism):** Systematic elimination of 5 candidate mechanisms (pairwise coherence, direction coverage, Cayley density, sector diversity, angle coherence) plus confirmation that paired extension is real but minority (6-20% at zeta_12 vs 100% at zeta_8). The surviving explanation — "algebraic coherence" — IS the RC separation property: deep entries maintain richer relational structure because BFS ancestry creates algebraic constraints between entries.
- **D90 (Relational, Not Positional):** The S² point cloud bandwidth DECREASES with depth while computation INCREASES. This is the cleanest proof that the depth law mechanism is relational (how entries combine via axis cancellation and cross-depth algebraic constraints), not positional (where entries sit in state space). In RC terms: reservoir quality is about the dynamics of state transitions, not the static geometry of the state space.

**The key difference from classical RC:**
- Classical RC: the reservoir is random or physical (water, memristors, optical cavities). The computational power is empirically discovered.
- DKC: the reservoir is ALGEBRAIC (braid group representation in SU(2)). The computational power is EXACTLY characterizable via the cyclotomic integer structure. The reservoir dynamics are not arbitrary — they are constrained by the braid group relations (Yang-Baxter equation), which GUARANTEE certain computational properties (e.g., D70's melody invariance under Yang-Baxter moves).

**The fit with the pillar chain:**
- Abramsky tells you TL diagrams encode computation (what CAN be computed)
- Habiro tells you the values are cyclotomic integers (what the VALUES are)
- Aizenberg tells you how to read out Boolean functions (the READOUT)
- Nazer-Gastpar tells you the ring structure guarantees exactness (the MEDIUM)
- **Reservoir Computing tells you WHY DEPTH MATTERS (the DYNAMICS)**

Without the RC pillar, the other four treat braid evaluation as a black box: input a braid, get a bracket value. The RC pillar opens the box: the SEQUENTIAL PROCESSING of crossings is itself a computation, and the depth/length of the braid word directly determines computational capacity. D82's linear depth law (max_xor ≈ depth+6) is the RC memory capacity theorem applied to the braid reservoir. D89-91 confirm the mechanism: balanced exponentials (vocabulary ~2x/depth vs parity 4x/weight) with axis cancellation as the geometric realization of the separation property.

**Which demos use it:** D73 (eigenvector automaton — reservoir cell transitions), D76-78 (capacity ceilings — finite reservoir limits), D79 (infinite reservoir breaks ceiling), D81 (logarithmic scaling — reservoir capacity law), D82 (depth law — reservoir memory capacity, algebraic coherence as reservoir quality), D87 (null dispensability — regime transition in reservoir state importance), D88 (anti-correlation — RC no-free-lunch in DKC geometry), D89 (depth law mechanism — RC separation property surviving 5 elimination tests), D90 (relational-not-positional — the cleanest RC alignment), D102 (radical = frozen abelian sub-reservoir), D103 (BFS growth = reservoir dynamics invariant), D104 (sign-hash = 1-bit CS readout with rank expansion), D105 (macrame principle — branching interaction regime transition), D106 (entropy null — dynamical classification invisible to bracket reservoir)

**The structural identity (exact, not analogical):**

| Reservoir Computing | DKC |
|---------------------|-----|
| Fixed reservoir (not trained) | Fixed braid catalog (from topology) |
| Only readout layer optimized | Only activation function tuned (k_sec, combined_cell) |
| High-dimensional dynamical state | Z[ζ₈] lattice / quaternion catalog |
| Edge of chaos = max capacity | ζ₈→ζ₁₂ = finite→infinite group transition |
| Separation property | XOR capacity / NPN class coverage |
| Echo state property | Axiality theorem (bounded, structured outputs) |
| Null reservoir states maintain manifold | D87: true at finite groups, redundant at infinite |
| Reservoir quality = dynamics, not state geometry | D90: relational, not positional |

**Edge of chaos mapping — the quantitative bridge:**

In RC theory, computational capacity is maximized at the critical transition between ordered dynamics (stable, low-dimensional attractors) and chaotic dynamics (sensitive to initial conditions, information destroyed). DKC has EXACTLY this transition:

- **ζ₈ = ordered regime:** Finite group (24 elements), capacity ceiling (XOR8), highly structured (axiality, 4 half-angles, 13 directions). Beautiful but limited. The reservoir closes — same inputs eventually cycle through the same states.
- **ζ₁₂ = edge of chaos:** Infinite group, capacity breaks through (XOR12+), 43 half-angles, 512 directions. Rich enough to compute everything but still structured enough to be useful. The reservoir never closes — new states keep appearing — but the algebraic structure (cyclotomic integers) prevents true chaos.

This is not metaphorical. The ordered→edge-of-chaos transition in RC is characterized by the spectral radius of the reservoir weight matrix approaching 1. The DKC analog: at ζ₈ the group is finite (spectral radius < 1 in some appropriate norm — dynamics contract to the 24-element attractor), and at ζ₁₂ the group is infinite (spectral radius = 1 — dynamics never contract, the state space keeps growing).

**Null states and the logarithmic partner hypothesis:**

In RC literature, reservoir null states — directions in state space that don't contribute to a specific readout — are recognized as structurally necessary for the reservoir's dynamical richness. They're not dead weight; they maintain the high-dimensional manifold that ENABLES the separation property. This is exactly the logarithmic partner hypothesis from D84: bracket-null entries (Re(q) = 0 for some readout) look useless to that specific readout but maintain the S² directional vocabulary that enables XOR computation.

**D87 refinement:** This hypothesis is REGIME-DEPENDENT. At finite groups (zeta_8, 24 entries), null states are indispensable — removing them destroys XOR8 capacity. At infinite groups (zeta_12, 4096 entries), null states are dispensable — removing 121 nulls has zero effect on XOR12 capacity. The transition mechanism is direction density: when non-null states alone provide enough S² coverage, null states become redundant scaffolding.

**No free lunch prediction — the k=6 sweet spot:**

RC has a no free lunch theorem: you can't maximize memory capacity and nonlinear processing capacity simultaneously. In DKC terms, this may explain the non-monotonic k=6 result from D50 (906 solutions at k=6 > 756 at k=7 > 96 at k=8). Increasing k_sec trades angular resolution (more sectors = more precision) against effective dimensionality (too many sectors = most cells empty = wasted capacity). k=6 is the sweet spot where the tradeoff is optimized for the Z[ζ₈] lattice with its pi/4 angular structure.

If correct, this PREDICTS: the optimal k_sec for ζ₁₂ is different from ζ₈'s optimal k=6, because ζ₁₂ has 43 half-angles instead of 4 — it can "fill" more S¹ sectors before hitting diminishing returns.

**D88 confirmation:** The k-ladder activation (8 k_sec values tested simultaneously) provides 14x better perturbation resilience than Voronoi-only activation (6.4% XOR loss vs 89% under the same design-improving gradient). The k-ladder IS a multi-scale readout that mitigates the no-free-lunch tradeoff by covering multiple resolution scales simultaneously.

**Memory capacity bound prediction:**

RC memory capacity bounds (Jaeger 2001) depend on reservoir dimensionality and spectral radius. The DKC analog: XOR capacity bounded by effective dimension of the catalog (number of independent directions × angular diversity), with the "spectral radius" being the group order or the number of distinct half-angles. The 11/13 theorem (two NPN classes unreachable by half-plane activation) might fall out of an RC separation bound applied to the specific dimensionality of the Z[ζ₈] catalog.

**Novel claim:** Nobody has proposed braid groups as reservoirs. The braid group has the ideal properties for an algebraic reservoir: (1) the Yang-Baxter equation constrains dynamics without making them trivial, (2) the SU(2) representation provides a compact continuous state space (S³) from discrete sequential input, (3) the group structure (finite at ζ₈, infinite at ζ₁₂) directly controls computational capacity, and (4) the Hopf fibration S³→S² naturally separates the computational channel (S² base) from the inert channel (S¹ fiber).

---

## The Gap: What Nobody Has Done

The five pillars each have rich literature in their respective domains:
- Habiro's integrality is a standard result in quantum topology
- Nazer-Gastpar's compute-and-forward is widely cited in information theory
- Abramsky's TL-as-computation is well-known in categorical quantum mechanics
- Aizenberg's MVN is established in complex-valued neural network theory
- Reservoir computing is a major paradigm in recurrent neural network theory

**No paper in any of these fields cites papers from the other four.** The intersection is genuinely unoccupied. Specifically:

1. Nobody has proposed using cyclotomic integers as neural network weight spaces (Habiro + Aizenberg)
2. Nobody has applied the compute-and-forward lattice framework to neural network weights (Nazer-Gastpar + Aizenberg)
3. Nobody has compiled topological invariants INTO weights as an alternative to training (Abramsky + Aizenberg, reversed direction)
4. Nobody has connected bracket evaluation rings to lattice coding theory (Habiro + Nazer-Gastpar)
5. Nobody has identified the TL planar lambda calculus as the computational substrate for forward DKC (Abramsky + Habiro)
6. Nobody has proposed braid groups as reservoirs for computation (Reservoir Computing + Abramsky + Habiro)
7. Nobody has connected reservoir memory capacity to knot crossing number (Reservoir Computing + Habiro)
8. Nobody has characterized an algebraic (non-random, non-physical) reservoir with provable capacity laws (Reservoir Computing + Nazer-Gastpar)

**Forward DKC assembles all five.** The construction is:

```
Abramsky: TL diagrams encode computation
    ↓ (evaluate via state sum)
Habiro: bracket values are cyclotomic integers
    ↓ (use as NN weights)
Aizenberg: roots-of-unity neurons compute Boolean functions
    ↓ (ring structure guarantees exactness)
Nazer-Gastpar: algebraic integer lattices support exact computation
    ↓ (sequential crossing processing = reservoir dynamics)
Reservoir Computing: braid depth determines computational capacity
```

This is the five-pillar chain. Each step is individually grounded in established mathematics. The assembly is novel.

**Circuit complexity connection (D92):** The parity-lock theorem adds a sixth field to the intersection — circuit complexity. Parity is the canonical function outside AC^0 (Furst-Saxe-Sipser 1984, Hastad 1987). The +/-q encoding naturally concentrates all computational power on this hardest-possible function. The DKC framework computes parity WITHOUT gradient descent, WITHOUT constant-depth circuits — it uses the algebraic structure of the braid reservoir to bypass both the gradient hardness (D47 lit.) and the circuit depth barrier. This is not a sixth pillar (circuit complexity does not contribute a constructive ingredient) but it is a uniquely strong validation: the framework naturally targets the function that is provably hardest for conventional approaches.

---

## Verification Status

| Pillar | Theoretically Grounded | Computationally Verified | Demo |
|--------|----------------------|-------------------------|------|
| Habiro integrality | Proven (2002) | All bracket values in Z[ζ₈] | D29 |
| Nazer-Gastpar exactness | Proven (2011) | Zero floating-point error in forward DKC | D29 |
| Abramsky bridge | Proven (2007) | TL matrices produce same brackets as state-sum | D35 |
| Abramsky non-semisimple | Proven (via Goodman-Wenzl/GRS) | b = -5/8 at TL_4, diverges at TL_6 | D85-86 |
| Aizenberg MVN | Proven (2000) | All 13 NPN classes at k=6 sectors | D50 |
| Aizenberg not bottleneck | — | k_sec invariance of depth gap | D91 |
| RC / braid reservoir | Established (2001) | max_xor ≈ depth+6; 82.8% echo state determinism | D73, D82 |
| RC / null dispensability | — | Regime transition confirmed (zeta_8 vs zeta_12) | D87 |
| RC / relational mechanism | — | Spectral inversion + axis cancellation | D90 |
| RC / balanced exponentials | — | Activation invariance confirms supply/demand ratio | D91 |
| Abramsky / Barrington radical | Proven (D102) | Radical = abelian writhe character; simple module wins at all XOR levels | D102 |
| RC / BFS growth invariant | — | Identical BFS trees across all n=6 modules (5x5, 9x9) | D102, D103 |
| RC / sign-hash = 1-bit CS | Established (Boufounos-Baraniuk 2008) | Sign-rank expansion 244→292; Atkinson sweet spot at 120/324 | D104 |
| RC / macrame principle | — | Cross-block wins at dim 5; within-block wins at dim 14 | D104, D105 |
| RC / entropy null | — | 100% XOR participation for both periodic and pA braids | D106 |
| Aizenberg / dual-channel | — | Product closure + v₂ connectivity predict parity capability | D108 |
| Aizenberg / encoding dependence | — | Product closure polarity inverts between additive and multiplicative encoding | D108, D109 |
| **Combined (forward DKC)** | **Novel thesis** | **65,536+ XOR triples, 906 parity solutions, XOR12 at ζ₁₂** | **D29, D50, D79** |
| **Parity-lock theorem** | **Proved (D92)** | **AND/OR/MAJ = 0 winners; XOR/XNOR only under +/-q** | **D92** |
| **Barrington-Radical Principle** | **Proved (D102)** | **Radical abelian → Barrington → useless for parity** | **D102** |
| **Dual-channel theorem** | **Novel (D108-D109)** | **Graph structure predicts parity; encoding flips polarity** | **D108, D109** |
