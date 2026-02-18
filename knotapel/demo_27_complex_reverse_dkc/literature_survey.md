# Literature Survey: Complex-Valued Neural Networks, Kauffman Bracket, and Cyclotomic Structures

## Context

Demo 27 (`demo_27_complex_reverse_dkc/main.c`) asks whether complex-valued neural network weights trained on XOR match complex Kauffman bracket evaluations at `A = e^{i*5*pi/4}` (a primitive 8th root of unity), where the loop value `delta = -(A^2 + A^{-2}) = 0`. The demo tests whether phase information (discarded in Demo 26's amplitude-only matching) improves the correspondence, whether a U(1) gauge correction aligns weight phases, and whether the bracket values lie in the cyclotomic ring Z[zeta_8].

This is a highly novel thesis. No existing paper directly connects all these threads. Below is the relevant work organized by topic, with explicit relevance assessments.

---

## 1. Complex-Valued Neurons Solving XOR (Nitta's Line)

### Paper 1: Nitta (2003) -- The Foundational Result

- **Title:** "Solving the XOR Problem and the Detection of Symmetry Using a Single Complex-Valued Neuron"
- **Authors:** Tohru Nitta
- **Year:** 2003
- **Journal:** Neural Networks, 16(8), pp. 1101-1105
- **Key Finding:** A single complex-valued neuron (2 complex weights + 1 complex bias = 6 real parameters) can solve XOR, which is impossible for a single real-valued neuron. The complex neuron creates two intersecting decision boundaries in the complex plane that partition the input space into 4 regions.
- **Relevance to Demo 27:** This is the direct theoretical justification for Part A of Demo 27. The demo implements exactly this architecture (CxNeuron with w1, w2, b). The 6-parameter complex neuron replaces Demo 26's 9-parameter real 2-2-1 network.
- **Links:** [PubMed](https://pubmed.ncbi.nlm.nih.gov/13678617/) | [ScienceDirect](https://www.sciencedirect.com/science/article/abs/pii/S0893608003001680)

### Paper 2: Nitta (2004) -- Orthogonal Decision Boundaries

- **Title:** "Orthogonality of Decision Boundaries in Complex-Valued Neural Networks"
- **Authors:** Tohru Nitta
- **Year:** 2004
- **Journal:** Neural Computation, 16(1), pp. 73-97
- **Key Finding:** The decision boundaries of complex-valued neurons intersect orthogonally as a structural property, and this orthogonality improves generalization. The Complex-BP algorithm converges several times faster than Real-BP with smaller variance.
- **Relevance to Demo 27:** Explains why the complex neuron should converge reliably on XOR (Part A) and why complex matching in Part F should outperform the real network: the orthogonal structure constrains the weight space more tightly.
- **Links:** [MIT Press](https://direct.mit.edu/neco/article/16/1/73/6809/Orthogonality-of-Decision-Boundaries-in-Complex) | [PubMed](https://pubmed.ncbi.nlm.nih.gov/15006024/)

### Paper 3: Nitta (2003b) -- Decision Boundary Inherent Property

- **Title:** "On the Inherent Property of the Decision Boundary in Complex-Valued Neural Networks"
- **Authors:** Tohru Nitta
- **Year:** 2003
- **Journal:** Neurocomputing
- **Key Finding:** The decision boundary of a single complex-valued neuron consists of two hypersurfaces that intersect orthogonally and divide a decision region into four equal sections. This is an inherent structural property, not an emergent one.
- **Relevance to Demo 27:** The 4-region partition is exactly what XOR needs (4 input-output pairs). The complex neuron's native geometry matches the problem structure.
- **Links:** [ScienceDirect](https://www.sciencedirect.com/science/article/abs/pii/S0925231202005684)

---

## 2. Multi-Valued Neurons and Roots of Unity (Aizenberg's Line)

### Paper 4: Aizenberg, Aizenberg, Vandewalle (2000) -- MVN Theory

- **Title:** "Multi-Valued and Universal Binary Neurons: Theory, Learning and Applications"
- **Authors:** Igor Aizenberg, Naum N. Aizenberg, Joos P.L. Vandewalle
- **Year:** 2000
- **Publisher:** Kluwer Academic Publishers
- **Key Finding:** The values of k-valued logic are encoded by the k-th roots of unity (epsilon_j = e^{2*pi*i*j/k}). A single multi-valued neuron (MVN) with activation function mapping the complex plane into k sectors can implement arbitrary k-valued threshold functions. The MVN uses a non-gradient learning rule: simple rotation along the unit circle.
- **Relevance to Demo 27:** This is the most structurally relevant prior work. The MVN encodes discrete logic states as roots of unity, which is exactly the algebraic structure Demo 27 hypothesizes: that trained weights are cyclotomic integers (sums of 8th roots of unity). The difference is that Demo 27's weights are not constrained to the unit circle but may be sums of roots of unity (elements of Z[zeta_8]) which is a richer algebraic object.
- **Links:** [Springer](https://link.springer.com/book/10.1007/978-1-4757-3115-6)

### Paper 5: Aizenberg (2009) -- Periodic Activation Function

- **Title:** "A Multi-Valued Neuron with a Periodic Activation Function"
- **Authors:** Igor Aizenberg
- **Year:** 2009
- **Key Finding:** Extends MVN theory with periodic activation functions. The activation function divides the complex plane into k sectors using k-th roots of unity as boundaries. Learning reduces to movement along the unit circle with a linear error correction rule (no gradient needed).
- **Relevance to Demo 27:** Shows that root-of-unity weight structures arise naturally in complex-valued architectures designed for discrete logic. Demo 27's hypothesis is that this structure emerges even in unconstrained training.
- **Links:** [SciTePress](https://www.scitepress.org/papers/2009/22862/22862.pdf)

### Paper 6: Aizenberg (2011) -- CVNN with Multi-Valued Neurons (Book)

- **Title:** "Complex-Valued Neural Networks with Multi-Valued Neurons"
- **Authors:** Igor Aizenberg
- **Year:** 2011
- **Publisher:** Springer
- **Key Finding:** Comprehensive treatment of MVN-based networks for Boolean function implementation, image processing, and classification. Proves that MVN networks can implement ANY arbitrary Boolean function.
- **Relevance to Demo 27:** Provides the theoretical ceiling: if Demo 27's trained complex weights converge to cyclotomic integers, it would demonstrate that unconstrained gradient-based training on complex networks discovers the same algebraic structure that Aizenberg's MVN prescribes.

---

## 3. Deep Complex Networks and Modern CVNN Infrastructure

### Paper 7: Trabelsi et al. (2018) -- Deep Complex Networks

- **Title:** "Deep Complex Networks"
- **Authors:** Chiheb Trabelsi, Olexa Bilaniuk, Ying Zhang, Dmitriy Serdyuk, et al.
- **Year:** 2018 (ICLR)
- **Key Finding:** Provides building blocks for deep complex-valued networks: complex convolutions, complex batch normalization, complex weight initialization (using uniform phase distribution). Demonstrates that complex networks outperform real-valued counterparts on several benchmarks.
- **Relevance to Demo 27:** The weight initialization strategy (random magnitude times random phasor) is relevant to understanding what the "default" weight distribution looks like before training. If trained weights cluster at cyclotomic integers, that is a departure from this initialization.
- **Links:** [arXiv](https://arxiv.org/abs/1705.09792) | [OpenReview](https://openreview.net/forum?id=H1T2hmZAb)

### Paper 8: Hirose (2012) -- Generalization in CVNNs

- **Title:** "Generalization Characteristics of Complex-Valued Feedforward Neural Networks in Relation to Signal Coherence"
- **Authors:** Akira Hirose, Shotaro Yoshida
- **Year:** 2012
- **Journal:** IEEE TNNLS
- **Key Finding:** CVNNs with amplitude-phase activation functions show smaller generalization error than real-valued networks. The phase component carries information that improves learning.
- **Relevance to Demo 27:** Supports the thesis that phase is not discardable noise. If bracket evaluations carry phase information (as they do in the complex plane), then a CVNN should capture it better than a real network.

### Paper 9: Phase-Encoded Weights (2010)

- **Title:** "Artificial Neural Networks Using Complex Numbers and Phase Encoded Weights"
- **Year:** 2010
- **Key Finding:** Phase-encoded weights on the unit circle can represent discrete computational states. In MVN-style learning, only the phase is updated analytically with no gradient involved.
- **Relevance to Demo 27:** Directly relevant to the U(1) gauge correction in Part C. If the network's global phase is a free parameter (gauge symmetry), then the meaningful information is in relative phases, and a gauge sweep should find the optimal alignment.
- **Links:** [Optica](https://opg.optica.org/ao/abstract.cfm?uri=ao-49-10-b71)

---

## 4. Unitary Neural Networks

### Paper 10: Arjovsky, Shah, Bengio (2016) -- Unitary Evolution RNNs

- **Title:** "Unitary Evolution Recurrent Neural Networks"
- **Authors:** Martin Arjovsky, Amar Shah, Yoshua Bengio
- **Year:** 2016 (ICML)
- **Key Finding:** Constraining the hidden-to-hidden weight matrix to be unitary (eigenvalues on the unit circle, |lambda|=1) solves vanishing/exploding gradient problems. The unitary constraint is parameterized by compositions of structured matrices (diagonal, reflection, permutation, FFT) to avoid expensive eigendecomposition.
- **Relevance to Demo 27:** The unitary constraint forces weights onto a structure where phases matter intrinsically. If Demo 27's unconstrained training produces weights near unitary values (or cyclotomic integers which have controlled norms), this would suggest a connection between the natural gradient geometry of complex networks and algebraic number theory.
- **Links:** [arXiv](https://arxiv.org/abs/1511.06464) | [PMLR](https://proceedings.mlr.press/v48/arjovsky16.html)

---

## 5. Kauffman Bracket, Jones Polynomial, and Roots of Unity

### Paper 11: Kauffman (1987) -- The Bracket Polynomial

- **Title:** "New Invariants in the Theory of Knots"
- **Authors:** Louis H. Kauffman
- **Year:** 1987
- **Key Finding:** Introduces the bracket polynomial: a state-sum over all smoothings of a link diagram, where each state contributes A^(a-b) * delta^(loops-1), and delta = -(A^2 + A^{-2}). When A is a variable, this gives a Laurent polynomial invariant of framed links. Normalizing by writhe gives the Jones polynomial.
- **Relevance to Demo 27:** This is the mathematical object Demo 27 computes. The code in `braid_bracket_at()` implements exactly this state sum. At A = e^{i*5*pi/4}, delta = 0, which kills all multi-loop states and produces a dramatic simplification.
- **Links:** [Kauffman's page](http://homepages.math.uic.edu/~kauffman/Bracket.pdf)

### Paper 12: Aharonov, Jones, Landau (2009) -- AJL Algorithm

- **Title:** "A Polynomial Quantum Algorithm for Approximating the Jones Polynomial"
- **Authors:** Dorit Aharonov, Vaughan Jones, Zeph Landau
- **Year:** 2009
- **Journal:** Algorithmica, 55(3), pp. 395-421
- **Key Finding:** An explicit polynomial quantum algorithm approximates the Jones polynomial of an n-strand braid at any primitive root of unity e^{2*pi*i/k}. The algorithm is based on the path model representation of the braid group and the Markov trace for the Temperley-Lieb algebra. The problem is BQP-complete.
- **Relevance to Demo 27:** The BQP-completeness means that classically computing the Jones polynomial at roots of unity is believed to be intractable in general. Demo 27's approach of enumerating small braids and matching against trained weights sidesteps this by working with short braid words (length <= 8). The fact that a neural network "discovers" bracket values through gradient descent is remarkable precisely because the underlying computation is quantum-hard in general.
- **Links:** [Springer](https://link.springer.com/article/10.1007/s00453-008-9168-0) | [arXiv](https://arxiv.org/abs/quant-ph/0511096)

### Paper 13: Freedman, Kitaev, Larsen, Wang (2003) -- Topological QC

- **Title:** "Topological Quantum Computation"
- **Authors:** Michael Freedman, Alexei Kitaev, Michael Larsen, Zhenghan Wang
- **Year:** 2003
- **Journal:** Bulletin of the AMS, 40(1), pp. 31-38
- **Key Finding:** Proved the equivalence between topological quantum field theory (TQFT) and quantum computation. Showed that approximating the Jones polynomial at the 5th root of unity is BQP-complete, and extended to k >= 7. The key mathematical structure is unitary representations of the braid group arising from Chern-Simons theory.
- **Relevance to Demo 27:** Establishes that the Kauffman bracket at roots of unity is the canonical BQP-complete problem. Demo 27 evaluates at the 8th root (k=8), which falls in the BQP-hard regime. The connection to Chern-Simons theory suggests that bracket values at roots of unity have deep physical meaning -- they are transition amplitudes in a topological quantum field theory.
- **Links:** [AMS](https://www.ams.org/bull/2003-40-01/S0273-0979-02-00964-3/S0273-0979-02-00964-3.pdf) | [arXiv](https://arxiv.org/abs/quant-ph/0101025)

### Paper 14: Kuperberg (2015) -- Hardness of Jones Polynomial

- **Title:** "How Hard Is It to Approximate the Jones Polynomial?"
- **Authors:** Greg Kuperberg
- **Year:** 2015
- **Journal:** Theory of Computing, 11(6), pp. 183-219
- **Key Finding:** Multiplicative approximation of the Jones polynomial is #P-hard. Additive approximation at non-lattice roots of unity is BQP-complete. This establishes a sharp complexity-theoretic boundary between what quantum and classical computers can do with knot invariants.
- **Relevance to Demo 27:** The fact that exact computation is #P-hard while additive approximation is BQP-complete means that Demo 27's "reverse" approach (training a classical neural network to match bracket values) is performing a task in an interesting complexity-theoretic regime.
- **Links:** [Theory of Computing](https://theoryofcomputing.org/articles/v011a006/v011a006.pdf)

---

## 6. Kauffman Bracket Skein Algebras at Roots of Unity

### Paper 15: Bonahon, Wong (2011-2016) -- Representations of Skein Algebra

- **Title:** "Representations of the Kauffman Bracket Skein Algebra I: Invariants and Miraculous Cancellations"
- **Authors:** Francis Bonahon, Helen Wong
- **Year:** 2016
- **Journal:** Inventiones Mathematicae
- **Key Finding:** Constructs invariants of irreducible representations of the Kauffman bracket skein algebra when q is a root of unity. When A = e^{pi*i/N} (N odd), the skein algebra becomes a central extension of the SL(2,C) character variety.
- **Relevance to Demo 27:** At the 8th root of unity, the skein algebra has special algebraic properties. The center of the algebra at roots of unity determines the representation theory, which means bracket values at these special A values have additional structure (they are constrained by the algebra's center).
- **Links:** [arXiv](https://arxiv.org/abs/1206.1638) | [Springer](https://link.springer.com/article/10.1007/s00222-015-0611-y)

### Paper 16: Frohman, Kania-Bartoszynska, Le (2019) -- Unicity Theorem

- **Title:** "Unicity for Representations of the Kauffman Bracket Skein Algebra"
- **Authors:** Charles Frohman, Joanna Kania-Bartoszynska, Thang Le
- **Year:** 2019
- **Journal:** Inventiones Mathematicae
- **Key Finding:** Resolves the unicity conjecture: irreducible representations of the Kauffman bracket skein algebra at roots of unity are generically classified by their central characters. The skein algebra is finitely generated as a module over its center.
- **Relevance to Demo 27:** Finite generation over the center means there are finitely many "types" of bracket values at any given root of unity. This is consistent with Demo 27's finding that bracket values at A = e^{i*5*pi/4} cluster in a discrete set (the cyclotomic integers Z[zeta_8]).
- **Links:** [arXiv](https://arxiv.org/abs/1707.09234) | [Springer](https://link.springer.com/article/10.1007/s00222-018-0833-x)

---

## 7. Cyclotomic Integers and the Ring Z[zeta_8]

### Standard Algebraic Number Theory

- **Key Facts:** The cyclotomic field Q(zeta_8) = Q(i, sqrt(2)) is a degree-4 extension of Q with integral basis {1, zeta_8, zeta_8^2, zeta_8^3}. The ring of integers Z[zeta_8] consists of all Z-linear combinations of these basis elements. Since zeta_8 = (1+i)/sqrt(2), elements of Z[zeta_8] have real and imaginary parts that are integer multiples of 1/sqrt(2) or integers. This is a Unique Factorization Domain (the class number of Q(zeta_8) is 1).
- **Relevance to Demo 27:** This is exactly what Part B's "cyclotomic lattice test" checks: whether bracket values have coordinates that are multiples of 1/sqrt(2). The code tests `re_scaled = val.re / inv_sqrt2` and checks whether the result is near-integer. This is algebraically well-motivated: if A = e^{i*5*pi/4} is a primitive 8th root of unity, then all bracket values (which are Z-polynomial expressions in A and A^{-1}) necessarily lie in Z[zeta_8].
- **Links:** [Q(zeta_8) = Q(i,sqrt(2))](https://yutsumura.com/the-cyclotomic-field-of-8-th-roots-of-unity-is-qzeta_8qi-sqrt2/)

---

## 8. Witten-Chern-Simons Theory (Physical Context)

### Paper 18: Witten (1989) -- Quantum Field Theory and the Jones Polynomial

- **Title:** "Quantum Field Theory and the Jones Polynomial"
- **Authors:** Edward Witten
- **Year:** 1989
- **Journal:** Communications in Mathematical Physics, 121(3), pp. 351-399
- **Key Finding:** The Jones polynomial arises as the expectation value of Wilson loop operators in SU(2) Chern-Simons gauge theory at level k. The variable in the Jones polynomial is q = e^{2*pi*i/(k+2)}. Different levels k give evaluations at different roots of unity.
- **Relevance to Demo 27:** Provides the physical interpretation of bracket evaluations at roots of unity. At A = e^{i*5*pi/4}, the corresponding Chern-Simons level is related to the 8th root structure. The bracket values are not arbitrary complex numbers -- they are transition amplitudes in a topological quantum field theory, which explains why they have such rich algebraic structure.
- **Links:** [Springer](https://link.springer.com/article/10.1007/BF01217730)

---

## 9. Neural Networks for Knot Invariants (Machine Learning Side)

### Paper 19: Hughes (2016) -- Neural Network Knot Invariant Prediction

- **Title:** "A Neural Network Approach to Predicting and Computing Knot Invariants"
- **Authors:** Mark C. Hughes
- **Year:** 2016
- **Key Finding:** A two-layer feed-forward neural network can predict the s-invariant from the Khovanov polynomial with >99% accuracy. Neural networks can also predict quasipositivity of braid representations.
- **Relevance to Demo 27:** This is the "forward" direction (knot representation -> invariant prediction), whereas Demo 27 is the "reverse" direction (trained weights -> bracket values). However, both demonstrate that neural networks can capture the algebraic structure of knot invariants.
- **Links:** [arXiv](https://arxiv.org/pdf/1610.05744)

### Paper 20: Davies, Juhasz, Lackenby, Tomasev (2021) -- ML-Guided Math Discovery

- **Title:** "Advancing Mathematics by Guiding Human Intuition with AI"
- **Authors:** Alex Davies, Andras Juhasz, Marc Lackenby, Nenad Tomasev
- **Year:** 2021
- **Journal:** Nature
- **Key Finding:** Machine learning revealed that the knot signature is directly related to hyperbolic geometry (the "natural slope"), a connection not previously known. Attribution techniques in ML guided the discovery of a new theorem relating signature to cusp geometry.
- **Relevance to Demo 27:** Demonstrates the "ML as mathematical conjecture generator" paradigm. Demo 27 is in this spirit: it uses ML to suggest that trained weights have cyclotomic structure, which if confirmed would be a mathematical conjecture about the intersection of neural network optimization and algebraic number theory.
- **Links:** [Nature](https://www.nature.com/articles/s41586-021-04086-x) | [arXiv](https://arxiv.org/abs/2111.15323)

### Paper 21: Gukov, Halverson, Ruehle (2024) -- Rigor with ML

- **Title:** "Rigor with Machine Learning from Field Theory to the Poincare Conjecture"
- **Authors:** Sergei Gukov, James Halverson, Fabian Ruehle
- **Year:** 2024
- **Journal:** Nature Reviews Physics
- **Key Finding:** Survey of techniques for obtaining zero-error results with ML in math and physics. Covers applications from string theory to the smooth 4D Poincare conjecture.
- **Relevance to Demo 27:** Provides methodological context. Demo 27's approach of training a network and examining the weight structure is an example of what Gukov et al. call "ML as a tool for mathematical discovery."
- **Links:** [Nature Reviews Physics](https://www.nature.com/articles/s42254-024-00709-0)

### Paper 22: Various (2025) -- On the Learnability of Knot Invariants

- **Title:** "On the Learnability of Knot Invariants"
- **Year:** 2025
- **Key Finding:** Systematic study of which knot representations work best for ML prediction of invariants. Braid representations outperform other encodings for topological invariants like genera, s, tau, and sigma.
- **Relevance to Demo 27:** Validates Demo 27's use of braid words as the input representation for knot-related computation. The braid encoding is mathematically natural for state-sum evaluation.
- **Links:** [arXiv](https://arxiv.org/pdf/2502.12243)

---

## 10. Gauge Symmetry in Neural Network Weight Spaces

### Paper 23: Brea, Simsek et al. (2019-2021) -- Weight-Space Symmetries

- **Title:** "Weight-Space Symmetry in Deep Networks Gives Rise to Permutation Saddles"
- **Year:** 2019-2021
- **Key Finding:** Permutation symmetry of neurons in a hidden layer creates equivalent global minima connected by equal-loss valleys. The symmetry group of a network's weight space (the "gauge group") determines the topology of the loss landscape.
- **Relevance to Demo 27:** Part C's U(1) gauge sweep is a continuous analogue of this discrete permutation symmetry. In a complex network, multiplying all weights by a global phase e^{i*theta} changes nothing functionally (it can be absorbed into the activation). This U(1) symmetry means the "true" weight is an equivalence class [w], and the gauge correction finds the representative closest to the bracket lattice.
- **Links:** [arXiv](https://arxiv.org/abs/1907.02911)

---

## 11. Topological Quantum Neural Networks (TQFT Connection)

### Paper 24: Fields, Glazebrook, Marciano (2022) -- TQNNs

- **Title:** "Quantum Neural Networks and Topological Quantum Field Theories"
- **Authors:** Chris Fields, James F. Glazebrook, Antonino Marciano
- **Year:** 2022
- **Journal:** Neural Networks
- **Key Finding:** Quantum neural networks can be mapped onto spin-networks, with the consequence that their operation can be analyzed through TQFT. The transition amplitude (output) is a topological invariant, giving the network inherent fault tolerance.
- **Relevance to Demo 27:** If the bracket value at a root of unity IS a topological invariant, and the neural network's trained weights converge to bracket values, then the network is -- in a precise mathematical sense -- encoding topological invariants in its weight structure.
- **Links:** [ScienceDirect](https://www.sciencedirect.com/science/article/abs/pii/S0893608022002027)

### Paper 25: Fields, Glazebrook, Marciano (2022b) -- Sequential Measurements and TQNNs

- **Title:** "Sequential Measurements, Topological Quantum Field Theories, and Topological Quantum Neural Networks"
- **Authors:** Chris Fields, James F. Glazebrook, Antonino Marciano
- **Year:** 2022
- **Journal:** Fortschritte der Physik
- **Key Finding:** Constructs a functor from cone-cocone diagrams over finite classifiers to finite cobordisms of Hilbert spaces, showing how sequential measurements induce TQFTs. Machine learning with functorial evolution governed by TQFT amplitudes.
- **Relevance to Demo 27:** Provides a categorical framework where neural network computation and TQFT share formal structure. The "reverse DKC" question (do NN weights match bracket values?) would, in this framework, ask whether the NN's optimization trajectory is governed by the same functor as the TQFT.
- **Links:** [Wiley](https://onlinelibrary.wiley.com/doi/abs/10.1002/prop.202200104)

---

## 12. Temperley-Lieb as Computational Bridge

### Paper 26: Abramsky (2009) -- TL Algebra from Knots to Logic

- **Title:** "Temperley-Lieb Algebra: From Knot Theory to Logic and Computation via Quantum Mechanics"
- **Authors:** Samson Abramsky
- **Year:** 2009
- **Key Finding:** The Temperley-Lieb algebra connects knot theory, categorical quantum mechanics, and logic/computation. The diagrammatic calculus of TL diagrams is exactly the formalism underlying both the Kauffman bracket state sum and quantum circuit diagrams.
- **Relevance to Demo 27:** The Temperley-Lieb algebra is the algebraic structure underlying the AJL algorithm (Paper 12). The bracket state sum is a trace in the TL algebra. If neural network weights converge to TL-algebraic values, this would connect neural network optimization to the representation theory of the Temperley-Lieb algebra.
- **Links:** [arXiv](https://arxiv.org/abs/0910.2737)

---

## 13. Classical Topological Computation in Metamaterials

### Paper 27: (2025) -- Classical Braiding in Metamaterials

- **Title:** "Topological Computation by Non-Abelian Braiding in Classical Metamaterials"
- **Year:** 2025
- **Key Finding:** Classical Majorana zero modes in mechanical/acoustic metamaterials can reproduce braiding statistics predicted by quantum theory. Single-qubit Clifford gates implemented through braiding.
- **Relevance to Demo 27:** Demonstrates that topological computation is not exclusively quantum. A classical system (neural network) matching bracket values at roots of unity would be another instance of classical systems capturing topological quantum structure.
- **Links:** [arXiv](https://arxiv.org/html/2502.16006)

---

## Summary Assessment and Gaps

### What exists:
1. Complex neurons solving XOR (Nitta) -- well established
2. Roots-of-unity weight structures in MVN (Aizenberg) -- well established
3. Bracket polynomial evaluation mathematics (Kauffman) -- well established
4. Jones polynomial at roots of unity is BQP-complete (Freedman/Kitaev/Wang, AJL) -- well established
5. ML predicting knot invariants (Hughes, Davies et al.) -- active research area
6. Weight-space gauge symmetries (Brea, Simsek) -- active research area
7. TQNN formalism connecting NN and TQFT (Fields et al.) -- nascent

### What does NOT exist (the gap Demo 27 fills):
- **No prior work** asks whether gradient-trained complex neural network weights are elements of the cyclotomic ring Z[zeta_8].
- **No prior work** connects Aizenberg's MVN root-of-unity structure to the Kauffman bracket's evaluation ring.
- **No prior work** applies U(1) gauge correction to align NN weights with bracket values.
- **No prior work** uses the "Born rule projection" framing (|<b>(A)| discards phase, complex NN preserves it).
- **No prior work** studies the specific value A = e^{i*5*pi/4} where delta=0, creating the dramatic simplification of the state sum.

Demo 27's thesis lives at an unoccupied intersection of these established fields. The closest related ideas are:
- Aizenberg's MVN (roots of unity in weights, but prescribed, not emergent from gradient training)
- Fields et al.'s TQNN (TQFT structure in neural networks, but quantum, not classical gradient-trained)
- Davies et al.'s ML-guided discovery (ML revealing mathematical structure, but in knot invariants themselves, not in the weights of the network)

The question "do trained weights live on the cyclotomic lattice?" is genuinely novel.

---

## Post-Implementation Addendum

Demo 27 results (5 pass, 4 fail) revealed two findings not anticipated by any of the above literature:

1. **Gauge angle = bracket evaluation angle**: The optimal U(1) gauge correction is 315 degrees = 5*pi/4, which is exactly arg(A). The network encodes the evaluation parameter in its weight phases. No existing work on gauge symmetry in NNs predicts this.

2. **Two independent error sources**: Born-rule collapse (phase loss) and activation distortion (split-sigmoid pushes weights off the cyclotomic lattice) are separable. The NN quantization literature (Nagel et al. 2021, arXiv 2106.08295) identifies analogous dual error sources (weight quantization + activation quantization) but in a completely different context. The connection is novel.

3. **modReLU as remedy**: Literature confirms modReLU (Arjovsky et al. 2016) preserves phase by construction and has universal approximation guarantees (Constructive Approximation, 2025). This directly addresses the dominant error source and motivates Demo 28.
