# The Five Pillars: Habiro / Nazer-Gastpar / Abramsky / Aizenberg / Reservoir Computing

The DKC thesis lives at an unoccupied intersection of five established fields. Nobody in the literature connects these results. Each pillar is well-established in its own domain; the novelty is assembling them into the forward DKC construction.

*Updated 2026-02-21: Reservoir Computing added as Pillar 5, motivated by the scaling arc (D73, D76-D82). The braid-as-reservoir connection gives DKC its temporal/dynamical dimension.*

---

## Pillar 1: Habiro (2002) — Bracket Values Are Cyclotomic Integers

**Paper:** "On the quantum sl_2 invariants of knots and integral homology spheres"
**Journal:** Geometry & Topology Monographs
**Link:** [arXiv:math/0209324](https://arxiv.org/abs/math/0209324)

**What Habiro proved:** The colored Jones polynomial has a "cyclotomic expansion" — it can be written as a sum over cyclotomic polynomials with integer coefficients. This means that evaluating the Jones polynomial (or equivalently, the Kauffman bracket) at any root of unity A = e^{iπ(2k+1)/2ℓ} necessarily produces a value in the cyclotomic integer ring Z[ζ_{2ℓ}].

**What this gives DKC:** The mathematical guarantee that our bracket catalog is a catalog of ALGEBRAIC INTEGERS, not floating-point approximations. At A = e^{i5π/4} (our primary case), all bracket values are elements of Z[ζ₈] = {a + bζ₈ + cζ₈² + dζ₈³ : a,b,c,d ∈ Z}. This ring has class number 1 (unique factorization domain), making exact arithmetic straightforward.

**The specific instance:** Habiro's integrality theorem is general; our Demo 29 implements it for the specific case ℓ=2 (8th root of unity, δ=0). The 64-element catalog of distinct bracket values is a finite subset of Z[ζ₈], computed by exact integer operations on 4-tuples — zero floating-point error.

**Strengthened by:** Costello-Francis-Gwilliam (2026, arXiv:2602.12412) proving Witten = RT, which means the bracket values are not just algebraic integers but rigorously quantum field theory transition amplitudes.

**Which demos use it:** D29 (forward DKC, exact Z[ζ₈] arithmetic), D35 (TL matrices over Z[ζ₈]), D48 (exhaustive 100M quartet search with exact arithmetic), D49 (neglecton weights in Z[ζ₈]), D50 (parity reachability with Z[ζ₈] catalog), D53 (Z[ω] at ℓ=3), D54 (Z[ζ₁₆] at ℓ=4), D58 (Z[ζ₅] at ℓ=5), D59 (Z[ζ₂₄] at ℓ=6)

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

**Which demos use it:** D27 (reverse DKC conceptual framework), D29 (forward DKC), D35 (TL matrices as computational engine), D47-50 (TL non-semisimplicity as parity barrier), D51-52 (radical anatomy — the algebraic obstruction theory)

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

**Which demos use it:** D45 (NPN classification with complex neurons), D47 (MVN activation comparison, topology-frequency correlation reversal), D48 (parity wall under split-sigmoid), D49 (neglecton weights — lattice contains parity weights), D50 (k-sector MVN resolves parity at k=6)

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

**The key difference from classical RC:**
- Classical RC: the reservoir is random or physical (water, memristors, optical cavities). The computational power is empirically discovered.
- DKC: the reservoir is ALGEBRAIC (braid group representation in SU(2)). The computational power is EXACTLY characterizable via the cyclotomic integer structure. The reservoir dynamics are not arbitrary — they are constrained by the braid group relations (Yang-Baxter equation), which GUARANTEE certain computational properties (e.g., D70's melody invariance under Yang-Baxter moves).

**The fit with the pillar chain:**
- Abramsky tells you TL diagrams encode computation (what CAN be computed)
- Habiro tells you the values are cyclotomic integers (what the VALUES are)
- Aizenberg tells you how to read out Boolean functions (the READOUT)
- Nazer-Gastpar tells you the ring structure guarantees exactness (the MEDIUM)
- **Reservoir Computing tells you WHY DEPTH MATTERS (the DYNAMICS)**

Without the RC pillar, the other four treat braid evaluation as a black box: input a braid, get a bracket value. The RC pillar opens the box: the SEQUENTIAL PROCESSING of crossings is itself a computation, and the depth/length of the braid word directly determines computational capacity. D82's linear depth law (max_xor ≈ depth+6) is the RC memory capacity theorem applied to the braid reservoir.

**Which demos use it:** D73 (eigenvector automaton — reservoir cell transitions), D76-78 (capacity ceilings — finite reservoir limits), D79 (infinite reservoir breaks ceiling), D81 (logarithmic scaling — reservoir capacity law), D82 (depth law — reservoir memory capacity, algebraic coherence as reservoir quality)

**The structural identity (exact, not analogical):**

| Reservoir Computing | DKC |
|---------------------|-----|
| Fixed reservoir (not trained) | Fixed braid catalog (from topology) |
| Only readout layer optimized | Only activation function tuned (k_sec, combined_cell) |
| High-dimensional dynamical state | Z[ζ₈] lattice / quaternion catalog |
| Edge of chaos = max capacity | ζ₈→ζ₁₂ = finite→infinite group transition |
| Separation property | XOR capacity / NPN class coverage |
| Echo state property | Axiality theorem (bounded, structured outputs) |

**Edge of chaos mapping — the quantitative bridge:**

In RC theory, computational capacity is maximized at the critical transition between ordered dynamics (stable, low-dimensional attractors) and chaotic dynamics (sensitive to initial conditions, information destroyed). DKC has EXACTLY this transition:

- **ζ₈ = ordered regime:** Finite group (24 elements), capacity ceiling (XOR8), highly structured (axiality, 4 half-angles, 13 directions). Beautiful but limited. The reservoir closes — same inputs eventually cycle through the same states.
- **ζ₁₂ = edge of chaos:** Infinite group, capacity breaks through (XOR12+), 43 half-angles, 512 directions. Rich enough to compute everything but still structured enough to be useful. The reservoir never closes — new states keep appearing — but the algebraic structure (cyclotomic integers) prevents true chaos.

This is not metaphorical. The ordered→edge-of-chaos transition in RC is characterized by the spectral radius of the reservoir weight matrix approaching 1. The DKC analog: at ζ₈ the group is finite (spectral radius < 1 in some appropriate norm — dynamics contract to the 24-element attractor), and at ζ₁₂ the group is infinite (spectral radius = 1 — dynamics never contract, the state space keeps growing).

**Null states and the logarithmic partner hypothesis:**

In RC literature, reservoir null states — directions in state space that don't contribute to a specific readout — are recognized as structurally necessary for the reservoir's dynamical richness. They're not dead weight; they maintain the high-dimensional manifold that ENABLES the separation property. This is exactly the logarithmic partner hypothesis from D84: bracket-null entries (Re(q) = 0 for some readout) look useless to that specific readout but maintain the S² directional vocabulary that enables XOR computation.

**No free lunch prediction — the k=6 sweet spot:**

RC has a no free lunch theorem: you can't maximize memory capacity and nonlinear processing capacity simultaneously. In DKC terms, this may explain the non-monotonic k=6 result from D50 (906 solutions at k=6 > 756 at k=7 > 96 at k=8). Increasing k_sec trades angular resolution (more sectors = more precision) against effective dimensionality (too many sectors = most cells empty = wasted capacity). k=6 is the sweet spot where the tradeoff is optimized for the Z[ζ₈] lattice with its pi/4 angular structure.

If correct, this PREDICTS: the optimal k_sec for ζ₁₂ is different from ζ₈'s optimal k=6, because ζ₁₂ has 43 half-angles instead of 4 — it can "fill" more S¹ sectors before hitting diminishing returns.

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

---

## Verification Status

| Pillar | Theoretically Grounded | Computationally Verified | Demo |
|--------|----------------------|-------------------------|------|
| Habiro integrality | Proven (2002) | All bracket values in Z[ζ₈] | D29 |
| Nazer-Gastpar exactness | Proven (2011) | Zero floating-point error in forward DKC | D29 |
| Abramsky bridge | Proven (2007) | TL matrices produce same brackets as state-sum | D35 |
| Aizenberg MVN | Proven (2000) | All 13 NPN classes at k=6 sectors | D50 |
| RC / braid reservoir | Established (2001) | max_xor ≈ depth+6; 82.8% echo state determinism | D73, D82 |
| **Combined (forward DKC)** | **Novel thesis** | **65,536+ XOR triples, 906 parity solutions, XOR12 at ζ₁₂** | **D29, D50, D79** |
