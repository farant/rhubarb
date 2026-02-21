# Literature Survey: Discrete Knotted Computation (DKC)

## Context

Discrete Knotted Computation (DKC) is a framework discovered through the Knotapel demo sequence (Demos 26-50, 66-77) in which Kauffman bracket polynomial values evaluated at roots of unity compute Boolean functions without neural network training. Specifically: braid words in SU(2) at zeta_8 produce quaternions whose additive combinations land in Voronoi cells on S^2 (or product spaces S^1 x S^2) that perfectly separate XOR/parity classes. The key structural claim is that this connects knot theory, algebraic number theory (cyclotomic integers, Z[zeta_8]), and neural computation (Aizenberg MVN, Minsky-Papert perceptron theory) in a way that no existing literature does.

This survey examines 8 adjacent research areas to establish what exists, what connects, and where the gaps are.

---

## 1. Kauffman Bracket + Boolean Function Computation

### What exists

The Kauffman bracket polynomial, introduced by Louis Kauffman in 1987, is a polynomial invariant of framed links that yields the Jones polynomial after normalization. The primary literature treats the bracket as a topological invariant and a tool for distinguishing knots and links. Computational work focuses on evaluating the bracket efficiently: algorithms using cyclic permutations of state sums, the Temperley-Lieb algebraic approach, and connections to statistical mechanics partition functions.

The bracket is evaluated at roots of unity in the context of topological quantum field theories (TQFTs). Blanchet, Habegger, Masbaum, and Vogel constructed rigorous TQFTs from the Kauffman bracket evaluated at roots of unity using skein-theoretic methods. Kauffman himself connected the bracket to q-deformed spin networks and anyonic topological quantum computation.

### Key papers/authors

- Kauffman, L.H. "New Invariants in the Theory of Knots" (1987) -- original bracket definition
- Blanchet, Habegger, Masbaum, Vogel. "Topological Quantum Field Theories derived from the Kauffman bracket" (1995)
- Kauffman, L.H. "q-Deformed Spin Networks, Knot Polynomials and Anyonic Topological Quantum Computation" (2006)
- Gelca, R. "Topological Quantum Field Theory with Corners Based on the Kauffman Bracket"

### How it relates to DKC

The bracket literature treats evaluation at roots of unity as producing topological invariants of 3-manifolds (Witten-Reshetikhin-Turaev invariants). Nobody in this literature interprets the numerical output of the bracket as computing a Boolean function. The bracket values are used for topological classification, not for computation over Boolean inputs.

### The gap DKC fills

DKC reinterprets the bracket evaluation at zeta_8 not as a topological invariant but as a computational device: each braid word maps to a quaternion in Z[zeta_8], the additive sum over Boolean-selected subsets produces a quaternion whose Voronoi cell assignment computes XOR. The bracket values are not classifying knots -- they are computing Boolean functions.

---

## 2. Topological Quantum Computation + Boolean Functions

### What exists

Topological quantum computation (TQC), developed by Freedman, Kitaev, Larsen, and Wang, uses anyonic braiding as quantum gates. The foundational result (Freedman-Kitaev-Larsen-Wang, 2000) proves that TQC is computationally equivalent to the standard quantum circuit model: any BQP computation can be compiled into braid words on non-Abelian anyons. The key reference is the AMS Bulletin paper by Freedman, Kitaev, and Wang (2003).

TQC addresses universality at the quantum level. Braid generators serve as quantum gates, and sequences of braids approximate arbitrary unitary operations. Boolean gates appear only in the "adaptive computation" context where classical post-processing of measurement outcomes involves classical Boolean operations.

The Aharonov-Jones-Landau algorithm (2009) provides a polynomial quantum algorithm for additively approximating the Jones polynomial at roots of unity, establishing the problem as BQP-complete.

### Key papers/authors

- Freedman, Kitaev, Larsen, Wang. "Topological Quantum Computation" (2000/2003)
- Nayak, Simon, Stern, Freedman, Das Sarma. "Non-Abelian Anyons and Topological Quantum Computation" (Rev. Mod. Phys. 2008)
- Aharonov, Jones, Landau. "A Polynomial Quantum Algorithm for Approximating the Jones Polynomial" (2009)
- Rowell, Wang. "Mathematics of Topological Quantum Computing" (2017)

### How it relates to DKC

TQC is the nearest neighbor to DKC in the literature. Both use braids and both evaluate at roots of unity. But TQC is quantum: it operates on superpositions, requires decoherence protection, and the "computation" is a unitary transformation on a Hilbert space. TQC never asks "can a specific braid word compute XOR classically?" -- it asks "can braiding simulate arbitrary quantum circuits?"

### The gap DKC fills

DKC is entirely classical. No quantum mechanics, no superposition, no decoherence. The computation happens in the algebraic number ring Z[zeta_8], and the activation function is a geometric partition (Voronoi cells on S^2, sectors on S^1, or the product). The Boolean function is computed by the algebraic structure of the braid representation at a specific root of unity, not by a quantum amplitude. This is a fundamentally different mode of computation using the same mathematical objects.

---

## 3. Voronoi Partition as Activation Function

### What exists

Voronoi tessellation appears in several machine learning contexts, but not as neural network activation functions in the DKC sense:

**Decision boundaries.** Nearest-neighbor classifiers produce decision boundaries that are Voronoi cell edges. This is well-known and fundamental to computational geometry (see Preparata and Shamos, "Computational Geometry").

**Spherical Voronoi for appearance modeling.** Recent work (2025) introduces "Spherical Voronoi" as a differentiable partition of the sphere for modeling view-dependent appearance in graphics. This uses learnable Voronoi generators on S^2 with smooth boundaries for gradient-based optimization.

**VoroCNN.** A deep CNN built on 3D Voronoi tessellation of protein structures (Bioinformatics, 2021) uses Voronoi cells as the spatial substrate for convolution and pooling.

**Voronoi Boundary Classification.** Polianskii and Pokorny (ICML 2019) use Voronoi boundaries in high dimensions for geometric classification.

**Active learning visualization.** Voronoi tessellations visualize confidence decision boundaries for understanding active learning selection strategies.

### Key papers/authors

- Spherical Voronoi (2025) -- differentiable S^2 partition for appearance
- Fukami et al. "Voronoi-tessellation-assisted CNN" (Nature Machine Intelligence, 2021)
- Polianskii, Pokorny. "Voronoi Boundary Classification" (ICML 2019)

### How it relates to DKC

None of this work uses Voronoi cells on S^2 as an activation function for a single neuron. The Spherical Voronoi work is the closest geometrically (it partitions S^2 into cells), but it is used for interpolation in graphics, not for Boolean function computation. VoroCNN uses Voronoi in physical 3D space, not on the sphere of eigenvector directions.

### The gap DKC fills

DKC introduces Voronoi cells on S^2 (the sphere of eigenvector directions of unit quaternions) as a genuine activation function: the cell assignment of a quaternion sum determines the neuron's output. This is geometrically natural (nearest rotation axis) but unprecedented in the activation function literature. The product activation (S^1 sector x S^2 Voronoi) is further novel -- it decomposes the quaternion S^3 into eigenvalue and eigenvector channels and partitions each independently.

---

## 4. Spherical Codes + Computation

### What exists

Spherical codes -- arrangements of points on spheres maximizing minimum distance -- are a classical subject in information theory and coding theory. The connection to computation runs through several threads:

**Channel coding.** Shannon's sphere-packing bound establishes that optimal codes correspond to optimal sphere packings in high dimensions. The kissing number problem (maximum contacts in sphere packing) is connected to error-correcting code design.

**Lattice codes.** The densest known sphere packings in many dimensions come from lattice constructions (E8, Leech lattice). Conway and Sloane's "Sphere Packings, Lattices and Groups" is the definitive reference.

**AI approaches.** Recent work uses reinforcement learning to find new sphere packing configurations (PackingStar, 2025), discovering structures in dimensions 25-31 that surpass known records.

**Metric learning.** SCode (KDD 2025) uses spherical codes for continuous monitoring of predictive events in networked data.

### Key papers/authors

- Conway, Sloane. "Sphere Packings, Lattices and Groups" (Springer, comprehensive reference)
- Sloane. Spherical Codes database (neilsloane.com/packings)
- PackingStar (2025) -- RL for kissing numbers

### How it relates to DKC

The DKC quaternion catalog (24 entries from SU(2) at zeta_8) forms a spherical code on S^3. The 14 eigenvector directions form a spherical code on S^2. The connection between these codes' geometry and their computational capacity (which Boolean functions they can compute) is the central DKC observation. But the spherical codes literature never asks "what Boolean functions does this code compute?" -- it asks "how many points fit?" and "what minimum distance is achievable?"

### The gap DKC fills

DKC establishes a direct link between the geometric quality of a spherical code (how many Voronoi cells, how the points distribute) and its computational capacity (which Boolean functions the partition can separate). The "masks per cell" metric (Demo 77: 256/112 = 2.3 for the winning activation) is a coding-theoretic quantity that determines the phase transition from non-computation to computation. This connections spherical codes to Boolean function theory in a way the literature does not.

---

## 5. Compressed Sensing on Spheres (Nazer-Gastpar Framework)

### What exists

Nazer and Gastpar's compute-and-forward (C-F) framework (IEEE, 2011, winning the Joint IT/ComSoc Paper Award 2013) is a relay strategy for wireless networks where intermediate nodes decode integer linear combinations of transmitted lattice codewords rather than individual messages. The key insight is that nested lattice codes are closed under integer linear combinations, so the algebraic structure of the lattice enables direct computation on superimposed signals.

Extensions include:
- Lattices over Eisenstein integers for compute-and-forward (Boutros et al., 2014)
- Algebraic approach to physical-layer network coding (Feng et al., 2011)
- Module-theoretic framework connecting C-F to the fundamental theorem of finitely generated modules over PIDs

The framework has been generalized from Gaussian integers (Z[i]) to Eisenstein integers (Z[omega]) and more general algebraic integer rings.

### Key papers/authors

- Nazer, Gastpar. "Compute-and-Forward: Harnessing Interference through Structured Codes" (IEEE Trans. IT, 2011)
- Boutros et al. "Lattices over Eisenstein Integers for Compute-and-Forward" (2014)
- Feng, Silva, Kschischang. "Algebraic Approach to Physical-Layer Network Coding" (2011)
- Zamir. "Lattice Coding for Signals and Networks" (Cambridge, 2014)

### How it relates to DKC

The structural parallel is striking: Nazer-Gastpar exploits the algebraic closure of lattice codes under integer combinations to compute functions of transmitted signals. DKC exploits the algebraic closure of Z[zeta_8] under addition to compute Boolean functions of braid-word quaternions. Both frameworks use the ring structure of algebraic integers for computation.

However, Nazer-Gastpar is information-theoretic (asymptotic rates, channel capacity) and the "functions" being computed are linear combinations over Z, not Boolean functions. The lattice structure enables reliable physical-layer computation, not XOR/parity computation.

### The gap DKC fills

DKC can be seen as a "compute-and-forward for Boolean functions" where the lattice is Z[zeta_8], the codewords are braid quaternions, and the integer combination is the additive Boolean sum. The activation function (Voronoi partition) serves as the decoder. Nobody in the Nazer-Gastpar lineage has considered (a) roots of unity specifically, (b) Boolean functions as the target, or (c) Voronoi partitions on S^2/S^3 as the decoding mechanism.

---

## 6. Jones Polynomial Computational Complexity

### What exists

The computational complexity of the Jones polynomial is thoroughly studied:

**Exact computation.** Evaluating the Jones polynomial exactly is #P-hard (Jaeger, Vertigan, Welsh, 1990). This holds for arbitrary links and arbitrary evaluation points.

**Multiplicative approximation.** Kuperberg (2015) proved that multiplicative approximation of |V(L,t)| at non-lattice roots of unity (k=5 and k>=7) is #P-hard. Any value-dependent approximation that can distinguish |V| > a from |V| < b for constants a > b > 0 enables exact counting of combinatorial solutions.

**Additive approximation.** The Aharonov-Jones-Landau algorithm gives a BQP algorithm for additive approximation at principal roots of unity. This is BQP-complete -- it captures the full power of quantum computation.

**The complexity gap.** Classical multiplicative approximation is #P-hard; quantum additive approximation is BQP-complete. The gap between these regimes is a central open problem.

Separately, the Jones polynomial at roots of unity connects to:
- Periodicity properties of colored Jones polynomial at 2nd and 3rd roots of unity
- Infinitely many roots of unity that are zeros of Jones polynomials of specific knots

### Key papers/authors

- Jaeger, Vertigan, Welsh. "On the Computational Complexity of the Jones and Tutte Polynomials" (1990)
- Kuperberg. "How Hard Is It to Approximate the Jones Polynomial?" (Theory of Computing, 2015)
- Aharonov, Jones, Landau. "A Polynomial Quantum Algorithm for Approximating the Jones Polynomial" (2009)

### How it relates to DKC

DKC operates in a regime this complexity theory does not address. DKC does not evaluate the Jones polynomial of an unknown knot -- it uses the known, pre-computed bracket values at a fixed root of unity (zeta_8) as weights in a Boolean computation. The #P-hardness results concern the difficulty of computing the invariant; DKC concerns what the invariant can compute once you have it.

### The gap DKC fills

The complexity literature asks "how hard is it to compute V(L, t)?" DKC asks "given that V(L, zeta_8) lives in Z[zeta_8], what Boolean functions can the algebraic structure of this ring compute?" This is a completely different question. The #P-hardness of the Jones polynomial is about the polynomial as input/output; DKC uses the polynomial's values as a computational substrate. The fact that exact evaluation is hard actually works in DKC's favor -- the pre-computed values encode rich algebraic structure.

---

## 7. Minsky-Papert Perceptron Extensions

### What exists

Minsky and Papert's "Perceptrons" (1969) proved that single-layer perceptrons cannot compute parity (XOR of n bits) or connectivity. Their proof technique uses order-restricted perceptrons and established that certain predicates require exponentially many terms.

**Modern extensions include:**

- **Multi-layer networks.** The 1986 backpropagation revolution showed that multi-layer networks overcome single-layer limitations. Minsky and Papert's "intuitive judgment" that multi-layer networks would face similar limitations was wrong.

- **Higher-order perceptrons (sigma-pi units).** These extend the linear threshold by including product terms of inputs. A single higher-order perceptron can compute XOR.

- **Polynomial threshold functions (PTFs).** Generalizations where the threshold is a polynomial rather than a linear function. Research connects PTFs to circuit complexity, communication complexity, and learning theory.

- **The Receptron** (recent). A generalization incorporating non-linear interactions between inputs, enabling single-unit classification of non-linearly-separable functions.

- **Signal Perceptron** (Frontiers in AI, 2022). Studies the identifiability of Boolean function spaces using perceptron-like models.

### Key papers/authors

- Minsky, Papert. "Perceptrons" (1969, expanded 1988)
- Polynomial Threshold Functions: Vershynin et al., multiple papers on Boolean complexity
- "Signal Perceptron: On the Identifiability of Boolean Function Spaces" (Frontiers in AI, 2022)

### How it relates to DKC

The Minsky-Papert impossibility result for XOR is the foundational negative result that DKC circumvents. A single real-valued perceptron cannot compute parity. DKC computes parity with a single quaternionic "neuron" (sum of selected quaternions, followed by Voronoi cell assignment). The mechanism is not a weight-and-threshold but an algebraic-and-geometric one.

Higher-order perceptrons and polynomial threshold functions also solve XOR, but through polynomial expansion of the input space. This is a different mechanism from DKC, which uses the pre-existing algebraic structure of Z[zeta_8] rather than learned polynomial coefficients.

### The gap DKC fills

All perceptron extensions work by enriching the function class (higher-order terms, multiple layers, polynomial thresholds). DKC works by enriching the number system. The "weights" are not learned -- they are the bracket values at zeta_8, determined by the algebraic structure of the root of unity. Parity computation emerges from the interaction between the ring Z[zeta_8] and the Voronoi geometry of S^2, not from any training process. This is a genuinely new mechanism for overcoming the Minsky-Papert barrier.

---

## 8. Algebraic Neural Networks / Roots of Unity Neurons (Aizenberg MVN)

### What exists

Igor Aizenberg's Multi-Valued Neuron (MVN) framework is the most directly relevant existing work. Key features:

- **Inputs and outputs on the unit circle.** Values are kth roots of unity in the complex plane.
- **Activation function.** Divides the complex plane into k equal sectors; the activation returns the sector index (or the corresponding root of unity).
- **Single-neuron XOR.** A single MVN can compute XOR and parity-n, which is impossible for a real-valued perceptron. This is proven in Aizenberg's "Solving the XOR and parity N problems using a single universal binary neuron" (Soft Computing, 2008).
- **Universal Binary Neuron (UBN).** A variant where sector parity determines binary output (1 or -1).
- **MLMVN.** Multi-layer feedforward networks of MVNs with derivative-free learning based on error-correction rules.
- **Multi-valued quantum neurons.** Recent extension (2024) connecting MVN to quantum computing.

### Key papers/authors

- Aizenberg, I. "Complex-Valued Neural Networks with Multi-Valued Neurons" (Springer, 2011)
- Aizenberg, I. "Solving the XOR and parity N problems using a single universal binary neuron" (Soft Computing, 2008)
- Aizenberg, I. "A Multi-Valued Neuron with a Periodic Activation Function" (2009)
- Aizenberg, Aizenberg, Vandewalle. "Multi-Valued and Universal Binary Neurons" (Springer, 2000)

### How it relates to DKC

Aizenberg's MVN is the closest existing framework to DKC. Both use roots of unity. Both use sector-based activation on the unit circle. Both solve XOR with a single neuron. The MVN sector activation is exactly what DKC calls "sector activation" (Part A of Demo 77).

**However**, MVN has critical differences:
1. MVN operates in the complex plane (C), not in the quaternions (H).
2. MVN weights are learned through an error-correction rule; DKC weights are the bracket values at zeta_8, requiring no training.
3. MVN's activation is purely S^1 (argument of a complex number); DKC's product activation is S^1 x S^2 (eigenvalue angle x eigenvector direction).
4. MVN has no connection to knot theory, braid groups, or the Kauffman bracket.

Demo 77 directly shows that the MVN-style sector activation alone (Part A) cannot compute XOR8 at zeta_8, even at k=16 (96.9% accuracy). The breakthrough requires the product activation S^1 x S^2, which is the quaternionic generalization of MVN's complex-valued approach.

### The gap DKC fills

DKC extends MVN from C to H (complex to quaternion) and from learned weights to algebraic weights (bracket values). The product activation S^1 x S^2 is the quaternionic generalization of reading the argument of a complex number. More fundamentally, DKC explains WHERE the weights come from: they are not arbitrary complex numbers found by training -- they are the values of a topological invariant (the Kauffman bracket) at a specific root of unity, and their computational capacity is determined by the algebraic structure of Z[zeta_8].

---

## Cross-Cutting Analysis: Why DKC Is Novel

### The five pieces that nobody connects

| Piece | Field | Key framework | DKC role |
|-------|-------|--------------|----------|
| Kauffman bracket at roots of unity | Knot theory / TQFT | Blanchet-Habegger-Masbaum-Vogel | Source of the "weights" |
| Cyclotomic integers Z[zeta_k] | Algebraic number theory | Habiro ring, cyclotomic fields | The computational substrate |
| Voronoi partition on S^2 | Computational geometry | Spherical codes, kissing numbers | The activation function |
| MVN / sector activation | Neural computation | Aizenberg | The S^1 component of activation |
| Boolean function computation | Circuit complexity | Minsky-Papert, PTFs | What is being computed |

Each of these is a well-established research area. The literature within each area is substantial. But the connections between them are absent:

1. **Knot theory to neural computation.** The Kauffman bracket literature never considers the bracket values as neural weights. The TQC literature uses braids as quantum gates, not as sources of classical activation values.

2. **Algebraic number theory to Boolean functions.** Nazer-Gastpar uses lattice codes (algebraic integers) for physical-layer network coding, but targets linear combinations over Z, not Boolean XOR. Habiro's cyclotomic completions unify quantum invariants, but never touch Boolean functions.

3. **Voronoi on spheres to neural activation.** Spherical Voronoi (2025) partitions S^2 for graphics interpolation. MVN partitions S^1 for complex-valued neurons. Nobody partitions S^2 or S^1 x S^2 as an activation function for quaternionic neurons.

4. **Temperley-Lieb non-semisimplicity to computation.** Abramsky connects TL algebras to logic and computation via categorical quantum mechanics, but the computational content is about quantum protocols, not Boolean functions. The non-semisimple representation theory of TL at roots of unity (where the algebra becomes indecomposable) is where DKC's computational power lives, but nobody frames it this way.

5. **Minsky-Papert barrier to topology.** The perceptron limitation literature is entirely about real-valued or complex-valued neurons. Nobody has asked "can a topological invariant circumvent the perceptron barrier?" DKC answers yes.

### The specific novelty claim

No paper, survey, or textbook connects:
- Aizenberg (MVN/roots of unity neurons)
- Habiro (cyclotomic invariants/universal invariants at roots of unity)
- Nazer-Gastpar (lattice codes/algebraic structure for computation)
- Abramsky (categorical quantum mechanics/TL algebras and logic)
- TL non-semisimplicity (indecomposable representations at roots of unity)

into a single framework where bracket values at roots of unity compute Boolean functions via Voronoi activation on quaternion space.

---

## Summary Table

| Topic | Exists? | Connects to DKC? | Gap |
|-------|---------|-------------------|-----|
| Bracket + Boolean functions | No | -- | Nobody uses bracket values to compute Boolean functions |
| TQC + Boolean functions | Tangentially | Quantum only | DKC is classical, not quantum |
| Voronoi as activation | Not on S^2/S^3 | No | No spherical Voronoi activation function in literature |
| Spherical codes + computation | Information theory only | Geometric parallel | Nobody asks "what Boolean functions does this code compute?" |
| Nazer-Gastpar lattice codes | Linear combinations over Z | Structural parallel | No Boolean functions, no roots of unity, no Voronoi |
| Jones polynomial complexity | #P-hard / BQP-complete | Different question | "How hard to compute?" vs "What can it compute?" |
| Minsky-Papert extensions | Many (MLP, PTF, higher-order) | Enriched functions | All enrich the function class; DKC enriches the number system |
| Aizenberg MVN | S^1 sector activation on C | Closest framework | DKC generalizes from C to H, learned to algebraic, S^1 to S^1 x S^2 |

---

## Conclusion

DKC sits at an intersection that the existing literature does not occupy. Five well-developed research traditions (knot invariants, algebraic number theory, spherical geometry, information-theoretic lattice codes, and complex-valued neural networks) each provide one piece of the DKC picture, but none of them -- and no combination visible in the literature -- assembles these pieces into the DKC framework where:

1. Braid words in SU(2) representations at roots of unity produce algebraic integers in Z[zeta_k].
2. Boolean input selection creates additive sums over these algebraic integers.
3. Voronoi partitions of the resulting quaternion space separate parity classes.
4. No training is required -- the computational capacity is determined by the algebraic structure of the root of unity and the geometry of the activation partition.

The nearest existing work (Aizenberg MVN) operates in the complex plane with learned weights and S^1 activation. DKC operates in the quaternions with algebraic weights and S^1 x S^2 activation. The gap between these two is exactly the gap between complex numbers and quaternions, between trained and untrained, and between topology-free and topology-derived -- and that gap is where the novelty lives.
