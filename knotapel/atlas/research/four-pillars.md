# The Four Pillars: Habiro / Nazer-Gastpar / Abramsky / Aizenberg

The DKC thesis lives at an unoccupied intersection of four established fields. Nobody in the literature connects these four results. Each pillar is well-established in its own domain; the novelty is assembling them into the forward DKC construction.

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

## The Gap: What Nobody Has Done

The four pillars each have rich literature in their respective domains:
- Habiro's integrality is a standard result in quantum topology
- Nazer-Gastpar's compute-and-forward is widely cited in information theory
- Abramsky's TL-as-computation is well-known in categorical quantum mechanics
- Aizenberg's MVN is established in complex-valued neural network theory

**No paper in any of these fields cites papers from the other three.** The intersection is genuinely unoccupied. Specifically:

1. Nobody has proposed using cyclotomic integers as neural network weight spaces (Habiro + Aizenberg)
2. Nobody has applied the compute-and-forward lattice framework to neural network weights (Nazer-Gastpar + Aizenberg)
3. Nobody has compiled topological invariants INTO weights as an alternative to training (Abramsky + Aizenberg, reversed direction)
4. Nobody has connected bracket evaluation rings to lattice coding theory (Habiro + Nazer-Gastpar)
5. Nobody has identified the TL planar lambda calculus as the computational substrate for forward DKC (Abramsky + Habiro)

**Forward DKC assembles all four.** The construction is:

```
Abramsky: TL diagrams encode computation
    ↓ (evaluate via state sum)
Habiro: bracket values are cyclotomic integers
    ↓ (use as NN weights)
Aizenberg: roots-of-unity neurons compute Boolean functions
    ↓ (ring structure guarantees exactness)
Nazer-Gastpar: algebraic integer lattices support exact computation
```

This is the four-pillar chain. Each step is individually grounded in established mathematics. The assembly is novel.

---

## Verification Status

| Pillar | Theoretically Grounded | Computationally Verified | Demo |
|--------|----------------------|-------------------------|------|
| Habiro integrality | Proven (2002) | All bracket values in Z[ζ₈] | D29 |
| Nazer-Gastpar exactness | Proven (2011) | Zero floating-point error in forward DKC | D29 |
| Abramsky bridge | Proven (2007) | TL matrices produce same brackets as state-sum | D35 |
| Aizenberg MVN | Proven (2000) | All 13 NPN classes at k=6 sectors | D50 |
| **Combined (forward DKC)** | **Novel thesis** | **65,536+ XOR triples, 906 parity solutions** | **D29, D50** |
