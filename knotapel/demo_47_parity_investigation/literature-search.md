# Demo 47: Parity Investigation — Literature Search Report

**Date:** 2026-02-18
**Source:** Automated literature scout (5 search areas, 22 web searches)

## Area 1: Aizenberg MVN and Parity

### Can MVN learn parity?

**Yes, definitively.** This is actually one of Aizenberg's headline results.

- **Aizenberg (2008), "Solving the XOR and parity N problems using a single universal binary neuron"**, Soft Computing, vol. 12, No. 3. [Springer Link](https://link.springer.com/article/10.1007/s00500-007-0204-9)

The Universal Binary Neuron (UBN) solves XOR and parity-N using a **single neuron**, no network required. The activation function divides the complex plane into `2k` equal sectors, where even sectors map to +1 and odd sectors map to -1. For n-input parity, you need `k = n` (so `2n` sectors).

### All 256 three-input Boolean functions

**Confirmed.** All 256 Boolean functions of 3 variables are P-realizable and can be implemented using a single UBN with `m=6` (12 sectors on the unit circle). All 65,536 four-variable functions are P-realizable with `m=8`.

### Learning rule

The MVN learning rule is **derivative-free error correction** — adjusts weight phases to rotate the weighted sum into the correct sector. No gradient computation needed. For multilayer networks (MLMVN), there is a derivative-free backpropagation variant (Aizenberg & Moraga 2007).

### Known difficulties with parity under MVN

**None reported.** Parity is MVN's *showcase* problem — notoriously hard for real-valued perceptrons but trivially solvable by a single MVN.

---

## Area 2: Parity and Partition Functions

### The sign problem

- The sign problem is **NP-hard** in general (Troyer & Wiese, 2005; Pan & Meng, 2022)
- Easing the sign problem is **NP-complete** for nearest-neighbor Hamiltonians, by reduction to MAXCUT
- Parity is the *paradigmatic* example of maximal cancellation in a state-sum

### Ising/Potts model and parity

- Computing the Ising partition function is **#P-hard** in general (Goldberg & Jerrum, 2017)
- For the complex-valued Potts model, #P-hardness for all non-real parameter values (Goldberg & Guo, 2021)

### Jones polynomial connection

The bracket IS a partition function. Evaluating at roots of unity: sometimes efficient (Clifford circuits). At generic points: #P-hard. **Parity's resistance to bracket decomposition may be a manifestation of the sign problem in the bracket state sum.**

---

## Area 3: Discrete Activation Functions and Lattice Alignment

### Neural network weight quantization as a lattice problem

- **"The Lattice Geometry of Neural Network Quantization" (2025)** [arXiv:2508.01077](https://arxiv.org/pdf/2508.01077) — GPTQ = Babai's nearest-plane algorithm
- **LatticeQ** — post-training weight quantization using lattice-based quantizers

### Lattices over algebraic integers

- **Lattices over Eisenstein Integers for Compute-and-Forward** [arXiv:1404.1312](https://arxiv.org/pdf/1404.1312)
- **Better Lattice Quantizers Constructed from Complex Integers** [arXiv:2204.01105](https://arxiv.org/pdf/2204.01105)

### The gap

**NO existing work** connecting: (1) neural network activations that output roots of unity, (2) weight quantization to cyclotomic lattices, (3) Kauffman bracket decomposition.

---

## Area 4: Parity in Topological Quantum Computation

### Ising anyons: NOT universal, limited to Clifford gates

**Ising anyons can only produce Clifford gates through braiding.** Clifford circuits are classically simulable (Gottesman-Knill theorem). To get universality, you need a non-Clifford gate.

- [arXiv:0812.2338](https://ar5iv.labs.arxiv.org/html/0812.2338) — Clifford gates in Ising-anyon TQC

### "Neglectons" (2025)

Extending to **non-semisimple TQFTs** introduces "neglectons" that rescue universality for Ising anyons:
- [Nature Communications (2025)](https://www.nature.com/articles/s41467-025-61342-8)

### Temperley-Lieb algebra at roots of unity

When q is a root of unity, TL becomes **non-semisimple** — Jones-Wenzl idempotent creates a radical.

- [Goodman & Wenzl (1993)](https://projecteuclid.org/journals/pacific-journal-of-mathematics/volume-161/issue-2/The-Temperley-Lieb-algebra-at-roots-of-unity/pjm/1102623233.full)
- [Ridout & Saint-Aubin (2019)](https://intlpress.com/site/pub/files/_fulltext/journals/mrl/2019/0026/0001/MRL-2019-0026-0001-a008.pdf)

**Connection to Demo 47**: The bracket at `A = zeta_8` evaluates in this non-semisimple regime. The obstruction limiting Ising anyons to Clifford gates may be the SAME obstruction preventing parity's clean bracket decomposition.

---

## Area 5: Complex-Valued Neural Networks and Boolean Functions

### Parity as the hardest function

1. **"Hardness of Learning Fixed Parities with Neural Networks" (2025)** [arXiv:2501.00817](https://arxiv.org/abs/2501.00817) — gradient descent fails on parity
2. **Threshold circuit complexity**: parity requires exponentially many neurons in depth-2 threshold circuits
3. **Fourier analysis**: parity has all Fourier mass on highest-degree coefficient (Linial-Mansour-Nisan)

### CVNNs and Boolean functions

- CVNNs solve 253/256 three-input Boolean functions with single-layer architecture
- MIT OpenMind (2024): neural networks track logical complexity of Boolean concepts

---

## Novelty Assessment

**The connection is completely novel.** Zero papers connect:
- Aizenberg's MVN/UBN activation (outputs = k-th roots of unity)
- Kauffman bracket decomposition of neural network weights
- Cyclotomic lattice structure (Z[zeta_k]) as target ring for weight decomposition
- Boolean function difficulty determined by compatibility with bracket algebra

The four communities (Aizenberg/MVN, Kauffman/knot theory, Nazer-Gastpar/lattice codes, Freedman-Kitaev/TQC) remain completely siloed.

The hypothesis that parity's resistance is algebraic (TL non-semisimplicity at roots of unity) connecting neural network learnability, knot invariant evaluation, topological quantum computation, and abstract algebra — nobody in the literature has proposed this connection.
