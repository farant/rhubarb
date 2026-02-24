# Key Sources: Particularly Valuable Papers with Notes

Papers that have directly shaped DKC research, with specific notes on what we extracted and how they connect.

---

## Tier 1: The Five Pillars

### Habiro (2002) — Cyclotomic Expansion
- **arXiv:** [math/0209324](https://arxiv.org/abs/math/0209324)
- **Why it matters:** Mathematical foundation for ALL of forward DKC. Proves bracket values at roots of unity are cyclotomic integers.
- **What we use:** The specific instance at A = e^{i5π/4}: Z[ζ₈] integrality. Basis {1, ζ₈, ζ₈², ζ₈³}, class number 1 (UFD).
- **What we DON'T use (yet):** The full colored Jones polynomial expansion. We only evaluate the uncolored bracket. Higher representations would give richer catalogs.
- **Status:** Core theorem, fully exploited at ℓ=2. Extensions at ℓ=3,4,5,6 confirmed computationally (Demos 53-59).

### Nazer-Gastpar (2011) — Compute-and-Forward
- **arXiv:** [1107.0300](https://arxiv.org/pdf/1107.0300)
- **Why it matters:** Proves that algebraic integer lattices support exact function computation. The information-theoretic frame for DKC.
- **What we use:** The conceptual parallel — algebraic ring closure enables computation without optimization.
- **What we DON'T use (yet):** The formal capacity bounds. The compute-and-forward rate region could potentially bound DKC saturation rates.
- **Best companion:** Tunali et al. (2015), extending to Eisenstein integers Z[ω]. Closest existing work to DKC's spirit.

### Abramsky (2007/2009) — TL as Computation
- **arXiv:** [0910.2737](https://arxiv.org/abs/0910.2737)
- **Why it matters:** The ONLY paper that proves TL diagrams encode computation. The theoretical ancestor of forward DKC.
- **What we use:** The existence proof — TL diagrams are a computational calculus. Our bracket evaluation IS evaluation in Abramsky's planar lambda calculus.
- **What we DON'T use (yet):** The full formal semantics. Abramsky's Geometry of Interaction interpretation could characterize WHICH bracket triples compute WHICH functions.
- **Open direction:** Tier 3 in our research priorities — full Abramsky depth.

### Aizenberg (2000, 2011) — MVN Theory
- **Springer:** [book](https://link.springer.com/book/10.1007/978-1-4757-3115-6)
- **Why it matters:** The neural network architecture. MVN = roots-of-unity activation → arbitrary Boolean functions.
- **What we use:** k-sector activation function. Demo 50 result (all 13 NPN at k=6) is direct application.
- **Key insight we extracted:** Aizenberg prescribes root-of-unity structure by design. We show it emerges from topology. The convergence of prescribed and emergent structure is the core DKC result.

---

## Tier 2: Structural Foundations

### Kauffman (1987) — The Bracket Polynomial
- **Link:** [Kauffman's page](http://homepages.math.uic.edu/~kauffman/Bracket.pdf)
- **Why it matters:** The mathematical object we compute. State-sum over all 2^k smoothings.
- **What we use:** The exact formula: ⟨K⟩ = Σ_s A^{σ(s)} · δ^{|s|-1}. At δ=0, only single-loop states survive.
- **Implementation:** `braid_bracket_at()` in every DKC demo from Demo 01 onward.

### Freedman, Kitaev, Larsen, Wang (2003) — Topological QC
- **arXiv:** [quant-ph/0101025](https://arxiv.org/abs/quant-ph/0101025)
- **Why it matters:** BQP-completeness of Jones polynomial at roots of unity. Establishes that what we compute is quantum-hard in general.
- **Key nuance:** At ℓ=5 (δ=φ, Fibonacci anyons), braid representations are DENSE in PSU(2) → TQC universality. But Demo 58 showed this DOESN'T help DKC — density in PSU(2) doesn't translate to angular diversity in scalar bracket output.

### Costello, Francis, Gwilliam (2026) — Witten = RT Proven
- **arXiv:** 2602.12412
- **Why it matters:** Rigorously proves the algebraic path (state sum = composing quantum group reps) and the physical path (Chern-Simons path integral) give identical answers. Our bracket computation is now QFT-grounded.
- **Status:** Preliminary version (75 pages), but authors are leading figures (Costello = foundational mathematical QFT).

### Witten (1989) — QFT and the Jones Polynomial
- **Springer:** [article](https://link.springer.com/article/10.1007/BF01217730)
- **Why it matters:** Physical interpretation — bracket values are Chern-Simons transition amplitudes, not arbitrary numbers.

### Nitta (2003) — Complex XOR Neuron
- **PubMed:** [13678617](https://pubmed.ncbi.nlm.nih.gov/13678617/)
- **Why it matters:** A single complex neuron (6 real params) solves XOR. Direct theoretical justification for our complex-valued DKC architecture. The 4-region partition from orthogonal decision boundaries is EXACTLY what XOR needs.

### Aizenberg (2008) — Solving XOR and Parity-N with a Single UBN
- **Springer:** [article](https://link.springer.com/article/10.1007/s00500-007-0204-9)
- **Why it matters:** Proves a single Universal Binary Neuron with 2n sectors solves n-input parity. Crucially, the weight construction (w₁=ζ₈, w₂=i, w₃=−1) for 3-input parity uses values IN the Z[ζ₈] bracket catalog.
- **What we extracted:** This paper reframes the Demo 48 parity wall from "lattice limitation" to "activation limitation" — the lattice contains parity-capable weights; split-sigmoid just can't read them.
- **Status:** Key result for understanding the Demo 48→50 arc. Extended by D91 (activation not the bottleneck for depth law) and D92 (encoding determines function accessibility).

---

## Tier 2.5: Indecomposability / LCFT Structure (NEW — D85-86)

### Gainutdinov, Read, Saleur (GRS) — Quantum Group b Parameter
- **Why it matters:** Defines the indecomposability parameter b algebraically via the quantum group / projective cover approach. Predicts b = -2 under their normalization conventions for the symplectic fermion theory at c = -2.
- **What we extracted (D85):** The "controversy" between b = -2 (GRS) and b = -5/8 (PR) dissolves when normalization is accounted for. b_delta = -5/2 on the regular representation divides by dim(0-TL sector) = 4 to give b_PR = -5/8. GRS and PR measure the same thing at different scales.
- **What we extracted (D86):** The multiplicity from the regular representation is structurally essential — it participates in the valuation balance, not just a scale factor. Single P_{0,0} universally diverges.
- **Status:** Key for understanding how TL non-semisimplicity governs DKC's computational obstruction structure.

### Pearce, Rasmussen — Lattice b = -5/8
- **Why it matters:** The finite-size lattice scaling computation of b for critical dense polymers (c = -2, p = 2). This is the physical/lattice value that Demo 85 confirms from first principles.
- **What we extracted (D85):** b_PR = -5/8 confirmed at TL_4 to 12 decimal places via leading-coefficient extraction from delta-parameterized Gram matrix. Valuation condition p_tt = 2*p_Tt holds at TL_4 but fails at TL_6.
- **What we extracted (D86):** The method diverges on a single P_{0,0} — a novel negative finding. The actual PR lattice method likely uses transfer matrices + finite-size scaling, not diagram algebra.
- **Open direction:** Identifying and implementing the actual PR method for sizes beyond TL_4.

### Symplectic Fermion Theory at c = -2
- **Why it matters:** The continuum CFT that the TL algebra at delta=0 discretizes. Predicts b = -2 (GRS convention) as a structure constant in the logarithmic CFT.
- **What we extracted:** The symplectic fermion theory is the c = -2 member of the dense polymer family (p = 2 in Gainutdinov et al. 2013 classification). Our computation at TL_4 reaches the correct physical value after proper normalization.

---

## Tier 3: TL Algebra / Radical Anatomy

### Goodman, Wenzl (1993) — TL at Roots of Unity
- **Link:** [Project Euclid](https://projecteuclid.org/journals/pacific-journal-of-mathematics/volume-161/issue-2/The-Temperley-Lieb-algebra-at-roots-of-unity/pjm/1102623233.full)
- **Why it matters:** Proves TL is non-semisimple at roots of unity, identifying the radical structure that Demo 51 computes exactly.
- **What we extracted:** This non-semisimplicity is the algebraic obstruction that limits which Boolean functions the bracket can compute (Demo 47's hypothesis, Demo 48's confirmation).

### Graham, Lehrer (1996) — Cellular Algebras
- **Journal:** Inventiones Mathematicae
- **Why it matters:** TL_n is cellular → Loewy length ≤ 3. This is the theoretical ceiling that our Demos 51-52 confirmed computationally across all roots of unity tested (ℓ=2,3,4,5,6,7).
- **What we extracted:** The cellularity bound explains WHY rad³ = 0 universally.

### Belletete, Ridout, Saint-Aubin (2016)
- **arXiv:** [1605.05159](https://arxiv.org/abs/1605.05159)
- **Why it matters:** Complete classification of indecomposable TL modules. PIM diamond structure (head ≅ socle, Loewy length 3) is the MODULE-level version of our sandwich theorem.
- **Our contribution beyond this:** We identified the ALGEBRA-level structure (rad²(TL_{2k}) ≅ TL_{2k-1} as a named algebra), which this paper doesn't do.

### Gainutdinov et al. (2013) — Logarithmic CFT Lattice Approach
- **arXiv:** [1303.2082](https://arxiv.org/abs/1303.2082)
- **Why it matters:** Our δ=0 case = dense polymers, p=2, q=i, central charge c = -2. The TL → Virasoro correspondence is explicitly shown. Tilting modules have the same diamond structure.
- **Key figure:** Figure 5 shows bimodule structure for exactly our case.
- **Extended by D86:** Dense polymer fusion calculation seeded as a future direction for algebraic description of DKC composition.

### Ridout, Saint-Aubin (2014) — Standard Modules and Induction
- **arXiv:** [1204.4505](https://arxiv.org/abs/1204.4505)
- **Why it matters:** Module-level structure that implies our results but doesn't explicitly compute dimension tables or identify named algebra isomorphisms.

### Creutzig (2024) — Non-Semisimple Verlinde
- **arXiv:** [2411.11383](https://arxiv.org/abs/2411.11383)
- **Why it matters:** Extends Verlinde formula to logarithmic CFT. Our TL_n at δ=0 is the discrete lattice version of his continuum theory.

### Gurarie (1993) / Flohr (1996) / Gaberdiel-Kausch (1996) — LCFT Foundations
- **Why they matter:** The logarithmic CFT framework where null states pair with logarithmic partners via Jordan-cell L₀ action. Directly relevant to D84's null-state analysis and D87's regime transition.
- **What we extracted (D84):** Null entries as Jordan-cell anchors — the LCFT interpretation of bracket-null catalog entries.
- **What we extracted (D87):** The LCFT null-state interpretation is regime-dependent: critical at finite groups where direction sparsity makes every null direction essential, redundant at infinite groups where direction density absorbs the loss.

---

## Tier 3.5: Circuit Complexity (NEW — D92)

### Furst, Saxe, Sipser (1984) — Parity Not in AC^0
- **Why it matters:** Proved that parity cannot be computed by constant-depth polynomial-size circuits of AND/OR/NOT gates. The theoretical backdrop for why parity is hard.
- **What we extracted (D92):** The +/-q encoding naturally concentrates all DKC computational power on EXACTLY this function — the canonical function outside AC^0. The encoding does not just happen to compute parity; it is structurally incapable of computing anything else (AND/OR/MAJ = 0 winners at all depths).

### Hastad (1987) — Exponential Lower Bounds for Parity in AC^0
- **Why it matters:** Strengthened Furst-Saxe-Sipser with exponential lower bounds. Any depth-d circuit computing parity on n bits requires exp(n^{1/(d-1)}) gates.
- **What we extracted (D92):** DKC bypasses this barrier entirely. The braid reservoir is not a circuit — it is a fixed algebraic dynamical system with a single-neuron readout. The linear depth law (max_xor ~ depth+6) is linear in depth, not exponential, because the "depth" is braid word length (reservoir steps), not circuit depth.

---

## Tier 3.6: Geometric Analysis (NEW — D88-90)

### Altschuler, Parrilo (2023) — Spherical Point Configurations
- **Why it matters:** Framework for analyzing point configurations on S² via spherical harmonic decomposition and polynomial energy minimization.
- **What we extracted (D90):** The AP hypothesis — that deeper S² point clouds access higher spherical harmonic modes — is INVERTED for DKC. Bandwidth DECREASES with depth while computation INCREASES. The depth law mechanism is relational (axis cancellation, cross-depth algebraic constraints), not positional (point cloud geometry).
- **What survives:** The AP framework may still apply via kernel rank analysis of the activation matrix (Gram matrix of combined_cell outputs), but the point-cloud-spectrum version is dead.

### Tahmasebi, Jegelka (NeurIPS 2023) — Group Invariance Implies Dimension Reduction
- **Why it matters:** SU(2)/U(1) = S² — group invariance reduces the effective dimensionality of the representation space.
- **What we extracted (D89):** Consistent with DKC's independent discovery (D67) that S² directional structure matters. The SU(2) → S² projection via the Hopf fibration is an instance of this general principle.

---

## Tier 4: Adjacent / Methodological

### GPTQ/Babai (2025) — NN Quantization = Lattice Problem
- **arXiv:** [2508.01077](https://arxiv.org/abs/2508.01077)
- **Why it matters:** Closest the ML community has come to our territory. Proves GPTQ (standard weight quantization) is equivalent to Babai's nearest-plane algorithm for the closest vector problem on a lattice. But they only consider Z^n, not algebraic integer lattices like Z[ζ₈].
- **The gap:** They notice quantization IS a lattice problem but haven't made the leap to non-standard lattices.

### Davies et al. (2021) — ML-Guided Math Discovery
- **Nature:** [article](https://www.nature.com/articles/s41586-021-04086-x)
- **Why it matters:** ML revealed knot signature ↔ hyperbolic geometry connection. The "ML as mathematical conjecture generator" paradigm. Our DKC arc is in this spirit.

### Gaier, Ha (2019) — Weight Agnostic Neural Networks
- **arXiv:** [1906.04358](https://arxiv.org/abs/1906.04358)
- **Why it matters:** Closest conceptual neighbor to forward DKC, but from the opposite direction. WANNs fix weights, search topologies. Forward DKC fixes topology, searches weights from a finite pre-computed catalog. Both bypass gradient descent.

### Neglectons (Nature Communications 2025) — Non-Semisimple TQFTs
- **Link:** [Nature Comms](https://www.nature.com/articles/s41467-025-61342-8)
- **Why it matters:** Extending to non-semisimple TQFTs introduces "neglectons" — radical elements that rescue universality for Ising anyons (limited to Clifford gates otherwise).
- **What we extracted:** Demo 49 tested whether neglecton weights enable parity — they add 148 new Z[ζ₈] values but no new angles. The wall is in the activation, not the lattice. D89 references Lauda et al. (2025) as external validation of Demo 84's null-indispensability findings.

### Costello, Francis, Gwilliam (2026) — Witten = RT Proven
- **arXiv:** 2602.12412
- **Why it matters:** Rigorously proves the algebraic path (state sum = composing quantum group reps) and the physical path (Chern-Simons path integral) give identical answers.
- **What we extracted:** Our bracket computation is now QFT-grounded. Preliminary version (75 pages), but authors are leading figures in mathematical QFT.

### Parity Hardness (2025)
- **arXiv:** [2501.00817](https://arxiv.org/abs/2501.00817)
- **Why it matters:** Gradient descent FAILS on parity. Forward DKC SUCCEEDS on parity (906 solutions at k=6, 9,334 at δ=√2 Re>0). The algebraic approach bypasses the gradient hardness entirely.

### Faddeev-LeVerrier Algorithm (1840/1853)
- **Why it matters:** Classical method for computing characteristic polynomials via a sequence of matrix-trace operations. Used in D85 for modular and complex characteristic polynomial computation as part of the Jordan block analysis infrastructure.
- **What we extracted:** Efficient char poly computation enabling the definitive diagonalizability test (gcd of min poly and its derivative) at the heart of the b = -5/8 extraction.

---

## Cross-Reference: Most-Cited Across Our Documents

| Paper | Times Referenced | In Which Documents |
|-------|-----------------|-------------------|
| Abramsky 2007 | 6+ | D27 survey, D29 survey, prior-art, integration notes, explorer's log, open directions |
| Habiro 2002 | 8+ | D29 survey, prior-art, integration notes, README, explorer's log, five-pillars, D85, D87, D90, D92 |
| Aizenberg 2000 | 5+ | D27 survey, D29 survey (implicit), prior-art, README, explorer's log |
| Aizenberg 2008 | 6+ | D47-D50, D63-D84, D91, D92 |
| Nazer-Gastpar 2011 | 5+ | D29 survey, prior-art, integration notes, README, open directions |
| Freedman-Kitaev-Wang 2003 | 4+ | D27 survey, prior-art, integration notes, correspondence |
| Witten 1989 | 4+ | D27 survey, prior-art, correspondence, integration notes |
| Kauffman 1987 | 4+ | D27 survey, prior-art, all demos |
| Graham-Lehrer 1996 | 4+ | Explorer's log (D51, D52), prior-art |
| Jaeger/Maass RC 2001/2002 | 5+ | D83, D84, D87, D89, D90, five-pillars |
| Nitta 2003 | 3+ | D27 survey (twice), prior-art |
| Belletete-Ridout-SA 2016 | 3+ | Explorer's log (D51), prior-art, D86 |
| Gainutdinov-Read-Saleur (GRS) | 2+ | D85, D86 (NEW) |
| Pearce-Rasmussen | 2+ | D85, D86 (NEW) |
| Furst-Saxe-Sipser / Hastad | 1+ | D92 (NEW) |

---

## The Novelty Statement

From the D29 literature survey summary table:

| Territory | Status |
|-----------|--------|
| Cyclotomic integers + NNs | **UNEXPLORED** (GPTQ/Babai 2025 is adjacent) |
| Forward compilation of topological invariants into NN weights | **UNEXPLORED** (WANNs tangential) |
| Lattice codes + NN computation | Established for communication, NOT for NNs |
| Bracket + exact cyclotomic arithmetic | Established in topology, NOT applied to computation |
| TL algebra + computation | Abramsky made the bridge; we cross it concretely |
| Algebraic NT + error-correcting codes | Established framework we apply |
| Braid groups as reservoirs | **UNEXPLORED** (D73, D82, D87-90) |
| Indecomposability parameter + computation | **UNEXPLORED** (b connects to DKC obstruction, D85-86) |
| Parity-lock from input encoding | **UNEXPLORED** (D92 — +/-q encoding computes ONLY parity) |

**Bottom line:** The question "can topological invariants be compiled into neural network weights?" has no direct precedent in the literature. The five pillars prove it should work; the demos prove it does. The D85-92 arc adds: the non-semisimple structure quantifies the computational obstruction (b = -5/8), the depth law mechanism is relational algebra not positional geometry, and the encoding naturally selects for the hardest Boolean function (parity, outside AC^0).
