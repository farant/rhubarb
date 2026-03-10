# Group D (Logic & Symbolic Reasoning) -- Knotapel Relevance Map

Date: 2026-02-26

This document maps connections between the three Group D papers (tensor logic, Hilbert space operators, edge-of-chaos complexity) and the knotapel DKC research program (109 demos, Temperley-Lieb algebras, Jones polynomials, cyclotomic Z[zeta_8] computation, forward DKC, reservoir computing).

These connections are NOT surface-level keyword overlap. Group D turns out to be the most deeply relevant extraction group for knotapel, because DKC is already doing -- computationally and exactly -- many of the things these papers propose theoretically.

---

## Connection 1: DKC IS Tensor Logic Over Boolean Tensors (Paper 1 <-> D29, D45-50, D92-93)

### The Paper 1 Claim
Domingos proposes that a Datalog rule like `Aunt(x,z) <- Sister(x,y), Parent(y,z)` is an einsum over Boolean tensors with a step function: `A_xz = H(S_xy * P_yz)`. Syllogisms compile to matrix multiplications + nonlinearity. Forward chaining reaches the deductive closure.

### What Knotapel Already Has
Forward DKC (D29) compiles knot-bracket-derived Z[zeta_8] values into neural network weights that compute Boolean functions WITHOUT training. The computation is:

1. Bracket catalog = a finite set of Z[zeta_8] values (100 distinct entries from TL_3 braids)
2. Weight assignment = select weights from catalog (3 per neuron for N=3 Boolean functions)
3. Weighted sum = S = w1*x1 + w2*x2 + w3*x3 where x_i in {0,1} (additive encoding) or {+q,-q} (paired encoding)
4. Activation = k-sector MVN (D50) or phase_cell (D93)
5. Output = Boolean function value

This IS a tensor equation. The weighted sum is exactly Domingos's einsum: `Output[mask] = Activation(Weight[i] * Input[mask, i])` where the join is over input index i.

### Why This Is Deeper Than It Looks
- **DKC's Boolean tensors are cyclotomic, not just {0,1}.** Paper 1 works over Boolean tensors with real-valued relaxation. DKC works over Z[zeta_8] -- an exact algebraic integer ring with 4 basis elements and zero floating-point error. The tensor entries are not learned; they come from topology (Habiro 2002).
- **DKC's "forward chaining" is the BFS catalog closure.** The BFS process (D82, D103) that generates the bracket catalog by composing braid generators IS forward chaining: start with generators, apply all possible compositions, accumulate new values until closure (finite group) or cap (infinite group). The catalog IS the deductive closure of the braid group representation.
- **The parity-lock theorem (D92) IS a tensor logic impossibility result.** Under +/-q encoding, the weight sum for input masks 00...0 and 11...1 are both zero, so any function where f(0...0) != f(1...1) is structurally impossible. This is a constraint on the tensor equation that Domingos's framework does not anticipate but that DKC proves exactly.
- **The circuit complexity hierarchy (D93) quantifies the "hardness landscape" of tensor equations.** AND/XOR ratio goes from 1.01 (N=3) to 2762 (N=7) to infinity (N=8). This hierarchy is universal across group structures (z8, 2I, zeta_12) -- it is a property of the readout mechanism, not the weight algebra. This is the kind of result that tensor logic would predict should exist but cannot yet compute.

### Leverage Points
- **DKC's exact arithmetic could validate tensor logic claims computationally.** Paper 1 has no implementation. DKC's Z[zeta_8] arithmetic (16 integer muls per product, zero floats) is a working exact tensor algebra engine. The code assets in `demo_29_cyclotomic_arithmetic/main.c` (Cyc8 type, ~200 lines) could serve as the substrate for a tensor logic interpreter.
- **Tucker decomposition (Paper 1's predicate invention) could be applied to the DKC catalog.** The 100-value bracket catalog has rich internal structure (41 parity / 17 poison / 42 neutral per D108, with product closure, Galois orbits, norm classes). Tucker decomposition of the catalog-as-tensor could reveal latent algebraic relations.
- **Temperature-controlled reasoning (Paper 1 Sec 5) maps onto k-sector activation.** Paper 1's temperature T controls deductive vs analogical reasoning (T=0 deductive, T=inf analogical). DKC's sector count k controls computational selectivity (k=2 simple, k=6 parity-capable, k=8 finest). The non-monotonic behavior (906@k=6 > 756@k=7 > 96@k=8, D50) suggests the mapping is nontrivial -- there is an "optimal temperature" that the tensor logic framework would need to accommodate.

### Relevant Demos
D29 (forward DKC), D45-50 (all 13 NPN classes, k-sector activation, parity reachability), D92 (parity-lock theorem), D93 (circuit complexity hierarchy), D94 (solvability bottleneck)

---

## Connection 2: The Hilbert Space Framework IS the S^2 Spectral DKC (Paper 2 <-> D67-71, D82)

### The Paper 2 Claim
Kiruluta et al. propose that learning is operator estimation in infinite-dimensional Hilbert spaces. Reasoning operators T map entity embeddings f_A to f_B where Tf_A ~ f_B encodes "A implies B." Spectral decomposition reveals which frequency components each relation operates on. Scattering transforms require NO training.

### What Knotapel Already Has
Demo 71 (Spectral DKC) performed exactly this analysis on S^2: the XOR6 computation was decomposed into spherical harmonics, revealing:

1. **Sharp bandwidth threshold at l=6** (the 13=13 theorem): 0% recovery at l<=5, 100% at l=6, because 2*6+1 = 13 = number of eigenvector directions
2. **Spectral universality**: all 36 XOR6 solutions share the same spectral envelope (l=6 dominant at ~80%, l=4 secondary at ~6%, l=12 tertiary at ~6-7%)
3. **Super-Nyquist compression**: 14 Voronoi cells suffice where Nyquist predicts 49, a 3.5x compression via compressed sensing

The DKC computation lives on S^2 = CP^1 (the Bloch sphere). The Hilbert space is L^2(S^2) with spherical harmonic basis {Y_lm}. The "entity embeddings" are quaternion eigenvector directions on S^2. The "reasoning operator" is the XOR6 Voronoi labeling function -- a binary function on S^2 that EXACTLY satisfies the Hilbert space operator framework.

### Why This Is Not Analogy -- It Is Identity
- **Paper 2's "spectral reasoning"** (Sec 8, p.13): Represent entity as spectral expansion f_x = sum f_k^(x) * phi_k. A reasoning operator R acts as sparse modulation: R[f_x] = sum gamma_k^(r) * f_k^(x) * phi_k.

  **DKC realization**: The XOR6 labeling function IS such a sparse spectral modulation. Mode l=6 carries ~80% of the power. The "operator" that maps 13 eigenvector directions to binary parity labels IS a band-limited function on S^2 with bandwidth exactly l=6. The gamma_k coefficients are the spherical harmonic expansion coefficients of the Voronoi partition.

- **Paper 2's "scattering transforms require NO training"** (Sec 6.2): Cascading wavelet transforms with modulus nonlinearities produces stable features without learned parameters.

  **DKC realization**: Forward DKC requires NO training. The bracket catalog IS the "scattering" of braid words through the TL representation. The "wavelet" is the Yang-Baxter R-matrix. The "cascading" is the sequential application of crossing generators. The "modulus nonlinearity" is the bracket trace closure. The output features (Z[zeta_8] values) are translation-invariant (under Reidemeister moves, D09) and deformation-stable (robustness plateau at ~50 degrees, D97).

- **Paper 2's "Representer Theorem"** (Sec 5, p.6): The minimizer of a regularized risk in RKHS lies in the span of kernel sections at training points.

  **DKC realization**: The knotapel briefing mentions an explicit open question about RKHS kernel rank: "Does rank(K_2I)/rank(K_z8) > 120/24, validating the reservoir computing interpretation?" (D94 feeds into this). The DKC catalog values ARE the kernel evaluation points. The "kernel" is the inner product in the cyclotomic ring. The Representer Theorem would say that the optimal XOR classifier lies in the span of the 100 (or 24, or 120) catalog values -- which is EXACTLY what forward DKC proves constructively.

### Leverage Points
- **Paper 2's closed-form kernel ridge regression could solve the "optimal activation" problem.** The DKC program has identified that activation function design is the critical variable (D50, D77, D91). Paper 2's framework gives alpha = (K + lambda*I)^{-1} y where K is the kernel matrix on catalog values. For the 100-value z8 catalog, this is a 100x100 matrix inversion -- trivial. The resulting alpha vector IS the optimal activation function, derived without search.
- **The RKHS kernel rank test (listed as next experiment in briefing.md) IS Paper 2's core measurement.** Rank(K_2I) vs rank(K_z8) measures whether the non-solvable group (2I, E_8, order 120) provides a richer RKHS than the solvable group (z8, E_7, order 48). If rank(K_2I)/rank(K_z8) > 120/24 = 5, then non-solvability contributes above raw size -- confirming both the DKC reservoir thesis and Paper 2's operator capacity theory.
- **Paper 2's Koopman operators (Sec 7.2) could formalize the D82 depth law.** The depth law (max_xor ~ depth + 6) describes how computational capacity grows with sequential dynamical steps. Koopman operators are linear operators on function spaces that capture nonlinear dynamics. The BFS closure of braid generators IS a discrete dynamical system; Koopman analysis of this system could yield a spectral decomposition of the depth law, potentially proving it analytically.
- **Demo 71's code assets (spherical harmonic engine, Gauss-Legendre quadrature) are directly reusable** for implementing Paper 2's spectral filtering and bandwidth analysis on new data.

### Relevant Demos
D67 (S^2 sweet spot, 13-direction Voronoi), D68 (intrinsic curvature proof), D69 (direction nesting), D71 (spectral DKC, 13=13 theorem, spherical harmonics), D72 (anti-correlation with geometric quality), D82 (crossing depth law), D94 (RKHS kernel rank open question)

---

## Connection 3: DKC Occupies the Edge of Chaos -- Quantitatively (Paper 3 <-> D79-80, D82, D106, Five Pillars)

### The Paper 3 Claim
Zhang et al. show that GPT-2 models trained on Elementary Cellular Automata data at intermediate complexity (Wolfram Class IV, "edge of chaos") develop the best general-purpose representations. Models trained on too-simple or too-chaotic data perform worse. The effect holds even at 67k parameters. The key metric is data complexity (Lempel-Ziv, Lyapunov exponent), not data source.

### What Knotapel Already Has
The five-pillars document (five-pillars.md) already identifies the edge-of-chaos connection as a STRUCTURAL IDENTITY, not a metaphor:

> "ζ₈ = ordered regime: Finite group (24 elements), capacity ceiling (XOR8), highly structured. Beautiful but limited. The reservoir closes."
>
> "ζ₁₂ = edge of chaos: Infinite group, capacity breaks through (XOR12+), 43 half-angles, 512 directions. Rich enough to compute everything but still structured enough to be useful. The reservoir never closes but algebraic structure prevents true chaos."

This is the exact ordered -> edge-of-chaos -> chaotic trichotomy from Paper 3:

| Paper 3 (ECA) | DKC |
|---|---|
| Class I/II (ordered, simple) | zeta_4: binary dihedral, 8 elements, XOR capacity trivial |
| Class IV (edge of chaos, Turing-complete) | zeta_8/zeta_12: structured-but-growing, XOR8-XOR12+ |
| Class III (chaotic, unlearnable) | No DKC analog -- algebraic structure prevents true chaos |

### Why This Is Deeper Than It Looks
- **Paper 3's "models learn complex solutions even when simple ones exist" (Sec 5.2) has an exact DKC counterpart.** D82 shows that deep catalog entries (depth-8, products of 8 generators) are 2x more efficient than shallow entries, even though shallow entries ALREADY contain the computational structure. The deep entries learn "more than they need to" -- they develop algebraic coherence (shared intermediate products) that transfers to harder problems. This IS Paper 3's finding that complex representations transfer better, applied to algebraic rather than neural representations.
- **Paper 3's "data complexity > data quantity" IS the DKC depth law.** max_xor ~ depth + 6 says computational capacity scales with the COMPLEXITY of the catalog entries (how many generator compositions deep), not the SIZE of the catalog. D82 proves this directly: 564 pure depth-8 entries (less vocabulary: 476 directions, 19 angles) outperform 564 strided entries (maximum vocabulary: 512 directions, 43 angles). Complexity beats quantity. This is Paper 3's central finding, confirmed with exact arithmetic on algebraic objects rather than neural network training loss.
- **D106 (topological entropy null) is the sharpest possible version of Paper 3's complexity claim.** Paper 3 says DYNAMICAL complexity (Lyapunov exponent, Lempel-Ziv) predicts downstream performance. D106 directly tests this: topological entropy (the dynamical complexity measure for braids) is COMPLETELY ORTHOGONAL to DKC computation. Both periodic and pseudo-Anosov braids compute XOR at 100% participation. The reason: the TL quotient at delta=0 erases all dynamical information while preserving algebraic structure. DKC's "complexity" is ALGEBRAIC (representation-theoretic structure, derived series strata, cross-depth constraints) not DYNAMICAL (Lyapunov exponents, topological entropy). This is a refinement Paper 3 does not make.
- **The finite/infinite group transition (D79-D80) IS the phase transition from ordered to edge-of-chaos.** Paper 3 shows a sharp transition at Class IV. DKC shows an equally sharp transition: zeta_8 (finite group, 24 elements) caps at XOR8; zeta_12 (infinite group) breaks through to XOR12+. The transition variable is not "data complexity" but GROUP FINITENESS -- a well-defined algebraic property with an ADE classification (D80). The binary octahedral group (E_7, zeta_8) is the last finite subgroup before the edge-of-chaos transition.

### Leverage Points
- **Paper 3's Lempel-Ziv complexity could be applied to DKC braid words.** Each braid word is a string over the alphabet {sigma_1, sigma_1^{-1}, sigma_2, sigma_2^{-1}}. The Lempel-Ziv complexity of this string is a computable measure of how "complex" the braid is. D106 showed topological entropy is orthogonal to computation. Would LZ-complexity of the braid WORD (as a string, not as a dynamical system) correlate with computational participation? This would bridge Paper 3's string-complexity measures to DKC's algebraic framework.
- **Paper 3's 67k-parameter tiny model validates DKC's approach.** DKC uses NO parameters -- the weights come from topology. Paper 3 shows that even 67k parameters suffice for rich representations when data is complex. DKC goes further: zero parameters suffice when the algebraic structure is rich enough. This is the extreme limit of Paper 3's insight.
- **Paper 3's reservoir computing connection (Sec 6) directly names what DKC already IS.** Paper 3 notes that ECA-pretrained transformers resemble reservoir computers -- fixed internal representations with only I/O layers trained. DKC is explicitly identified as a "discrete algebraic reservoir computer" (D94, five-pillars.md). The braid group representation is the fixed reservoir; the activation function is the trained readout. The only conceptual gap is that Paper 3's reservoirs are RANDOM, while DKC's reservoir is ALGEBRAIC (constrained by Yang-Baxter, Habiro, Graham-Lehrer). The algebraic reservoir is provably better: D94 shows non-solvable groups (2I) outperform solvable groups (z8) at matched catalog size, confirming that ALGEBRAIC STRUCTURE (not just dynamical complexity) determines reservoir quality.

### Relevant Demos
D79 (zeta_12 breaks zeta_8 ceiling), D80 (finite/infinite group boundary, ADE classification), D81 (logarithmic scaling law), D82 (depth law, algebraic coherence > vocabulary), D94 (solvability bottleneck = algebraic reservoir quality), D106 (topological entropy null -- dynamical complexity is orthogonal)

---

## Connection 4: The Dual-Channel Theorem IS Operator Decomposition (Paper 2 <-> D107-109)

### The Paper 2 Claim
Reasoning operators decompose into spectral components. Different frequency bands carry different relational information. The operator T can be written as T = sum gamma_k * |phi_k><phi_k| where gamma_k are spectral modulation coefficients.

### What Knotapel Already Has
The Dual-Channel Theorem (D108-D109) establishes that parity computation requires BOTH:
1. **Multiplicative phase coherence** (product closure > 0): Z[zeta_8] values whose products stay within the vocabulary
2. **Additive magnitude diversity** (v_2 connectivity > trivial): values connected by addition that span different 2-adic valuation levels

These are two independent algebraic channels that measure different properties of the Z[zeta_8] catalog. They map onto the T-gate/Hadamard decomposition in quantum circuit synthesis (Amy-Glaudell-Ross 2023).

### Why This Maps to Paper 2's Operator Decomposition
Paper 2 decomposes reasoning operators into spectral components (frequency bands). The dual-channel theorem decomposes DKC's "parity operator" into:
- **Phase channel** = Z/4Z rotation (multiplication by zeta_8 powers, the product closure graph). This IS a spectral operation -- it modulates the cyclotomic phase.
- **Magnitude channel** = 2-adic valuation connectivity (additive closure with v_2 tracking). This IS a scale operation -- it measures how "divisible by 2" the values are.

The orthogonality theorem (D107 Claim #15) makes this explicit: the multiplicative quotient graph is K_5 (complete graph, Z/4Z multiplication table) while the additive quotient graph is a star tree centered on 0. Multiplication = Z/4Z rotation across axes; addition = growth within each axis. These are orthogonal operators in the algebraic graph Hilbert space.

### Leverage Points
- **Paper 2's Hilbert-Schmidt norm (Sec 4, p.5) could regularize DKC activation design.** The activation function maps Z[zeta_8] sums to Boolean outputs. Treating this as an operator T: L^2(S^2) -> {0,1}, the HS norm ||T||_HS provides a principled regularization. Minimizing ||T||_HS subject to correct XOR output would find the "simplest" activation, potentially explaining why k=6 is optimal.
- **The encoding-dependent polarity inversion (D109) is a frequency-domain phenomenon.** Under additive encoding, parity wants HIGH product closure; under multiplicative encoding, parity wants LOW product closure. This sign flip is exactly what happens when you change the phase convention in a Fourier transform. Paper 2's framework could formalize this as a conjugation operation on the reasoning operator.

### Relevant Demos
D107 (Z/4Z axis-alignment, orthogonality theorem), D108 (dual-channel theorem, parity vocabulary graph analysis), D109 (encoding-dependent polarity inversion)

---

## Connection 5: Axiality Is Compressed Sensing on an Algebraic Ring (Paper 2 <-> D107, D71)

### The Paper 2 Claim
Compressed sensing (Sec 6, p.7): if a signal is sparse in some basis, it can be exactly recovered from far fewer measurements than Nyquist requires. Solve via L1 minimization.

### What Knotapel Already Has
Two independent compressed sensing results:
1. **D71 (Super-Nyquist compression):** 14 Voronoi cells suffice for XOR6 on S^2 where Nyquist predicts 49. The 3.5x compression is because XOR6 is a BINARY function at 13 KNOWN positions -- sparsity + known support.
2. **D107 (Axis-alignment):** Every braid matrix entry at delta=0 is axis-aligned (exactly one nonzero cyclotomic component: n*zeta_8^k). This means Z[zeta_8] values are 1-sparse in the {1, zeta_8, zeta_8^2, zeta_8^3} basis. The entire 186-value catalog is 1-sparse.

### Why This Matters
The axiality theorem (D107) says DKC at delta=0 reduces to INTEGER PATH COUNTING + THREE-VARIABLE PHASE FORMULA: Entry(r,c) = (path count) * zeta_8^{(w - 2*(nest(r) + nest(c))) mod 4}. This is the most extreme possible compressed sensing: each Z[zeta_8] value, which lives in a 4-dimensional ring, is compressed to a 1-dimensional integer plus a discrete phase label from Z/4Z.

Paper 2's L^2 framework would express this as: the "signal" (the braid representation matrix) is maximally sparse in the cyclotomic basis. The 2-adic neutrality observation (D107 Claim #13: geometric decay ~0.5x per valuation level) constrains the integer magnitudes. The Z/4Z phase formula constrains the phases. Together, these give a representation that is both sparse AND structured -- exactly the conditions under which compressed sensing provides the maximum advantage.

### Leverage Points
- **D104's sign-hash = 1-bit compressed sensing** (Boufounos-Baraniuk 2008) is already explicitly using compressed sensing theory. Paper 2's RKHS framework could provide the theoretical underpinning for why sign quantization EXPANDS effective rank (244->292 for W_{6,2}).
- **The axiality theorem could enable extremely efficient inference.** If every Z[zeta_8] value is 1-sparse, then the "kernel matrix" K in Paper 2's framework has a special structure: K[i,j] depends only on the integer magnitudes and the Z/4Z phase difference. This would make the kernel matrix highly structured (block-diagonal with at most 4 blocks), dramatically reducing the O(n^3) matrix inversion to O(n) per block.

### Relevant Demos
D71 (super-Nyquist compression on S^2), D104 (sign-hash = 1-bit compressed sensing), D107 (axiality theorem, Z/4Z phase formula)

---

## Connection 6: The Reservoir Computing Identification Is Exact (Paper 3 <-> D73, D82, D94, D106, Five Pillars)

### The Paper 3 Claim
Reservoir computing: a fixed, untrained dynamical system processes sequential input; only the readout is trained. Computational power comes from the STRUCTURE of the dynamics. Edge-of-chaos reservoirs are optimal.

### What Knotapel Already Has
DKC IS a reservoir computer, with every component identified exactly (from five-pillars.md):

| Reservoir Computing | DKC |
|---|---|
| Fixed reservoir | Fixed braid catalog from topology |
| Only readout trained | Only activation function tuned |
| High-dimensional state | Z[zeta_8] lattice / quaternion catalog |
| Edge of chaos = max capacity | zeta_8->zeta_12 = finite->infinite transition |
| Separation property | XOR capacity / NPN class coverage |
| Echo state property | Axiality theorem: bounded, structured outputs |
| Memory capacity | max_xor ~ depth + 6 (D82) |
| Reservoir quality | Algebraic coherence > vocabulary (D82) |

### What Paper 3 Adds to This
Paper 3's specific contribution is the COMPLEXITY MEASURE. DKC identifies the edge-of-chaos transition qualitatively (finite group = ordered, infinite group = edge-of-chaos) and quantitatively (capacity scaling laws). Paper 3 adds Lempel-Ziv complexity, Lyapunov exponent, and Krylov complexity as measurable correlates.

However, D106 FALSIFIES the naive application of Paper 3's dynamical complexity measures to DKC. Topological entropy (the Lyapunov exponent analog for braids) is completely orthogonal to DKC computation. The correct complexity measure for DKC is ALGEBRAIC, not DYNAMICAL:
- Derived series depth (D93)
- Solvability vs non-solvability (D94 -- Barrington's theorem)
- BFS catalog depth (D82)
- Galois orbit structure (D107)
- Product closure and v_2 connectivity (D108)

This is a genuine refinement of Paper 3: for algebraic reservoirs, the relevant complexity is algebraic (representation-theoretic), not dynamical (Lyapunov, topological entropy). The DKC program has 109 demos of evidence for this claim.

### Leverage Points
- **Krylov complexity (Paper 3) could be the RIGHT complexity measure for DKC.** Unlike Lyapunov/topological entropy (which track state-space divergence), Krylov complexity tracks OPERATOR growth -- how many independent operators are needed to describe the time evolution. This is much closer to what DKC measures: the BFS catalog growth (D103) is ~(n-1)x per round, which IS a Krylov-like growth rate for the operator algebra generated by braid generators. The braid group Krylov complexity might correlate with DKC capacity where topological entropy does not.
- **Paper 3's tiny model (67k params) validates the DKC reservoir readout philosophy.** DKC freezes the reservoir (topology) and only tunes the readout (activation). Paper 3 confirms this is sufficient even for general-purpose representations. The combination suggests: train a 67k-parameter readout head on top of a DKC reservoir, and you get the best of both worlds -- exact algebraic features from topology + learned nonlinear readout from Paper 3's edge-of-chaos principles.
- **The D94 solvability bottleneck IS Barrington's theorem applied to reservoirs.** Paper 3 shows that Wolfram Class IV (Turing-complete) rules produce the best reservoirs. D94 shows that non-solvable groups (which are the algebraic analog of Turing-completeness via Barrington's theorem) produce better DKC reservoirs. This is the same insight in different mathematical clothing.

### Relevant Demos
D73 (eigenvector automaton, 82.8% universal determinism), D79-80 (finite/infinite group transition), D82 (depth law), D94 (solvability = reservoir quality), D103 (BFS growth = reservoir invariant), D106 (entropy null)

---

## Connection 7: Tucker Decomposition and the DKC Catalog (Paper 1 <-> D96, D108)

### The Paper 1 Claim
Tucker decomposition of a learned tensor reveals invented predicates. Factor matrices M, M', M'' and core tensor C form a low-rank approximation. Thresholding to Boolean yields new logical relations.

### What Knotapel Has
The DKC catalog at zeta_8 has exactly 100 values. D96 shows the OPTIMAL catalog is 21 entries (the A+B+C subset), not 24 -- removing 3 Q8-null entries IMPROVES capacity. D108 partitions the 100 values into 41 parity / 17 poison / 42 neutral, with dramatic structural differences (product closure: 17 edges for parity, 0 for poison).

### The Connection
The 100-value catalog, viewed as a 100x100x100 tensor (indexed by weight triple, with Boolean XOR output), has natural Tucker structure. The 41/17/42 partition IS a discovered "predicate" -- parity-capability is a latent property of Z[zeta_8] values that Raqiya's graph analysis detects. Tucker decomposition would formalize this: the factor matrices would separate values into parity-capable, poison, and neutral along each mode, and the core tensor would encode which COMBINATIONS work.

### Leverage Points
- **Tucker decomposition of the DKC solution tensor could discover new algebraic relations.** The 906 parity solutions at k=6 (D50) form a sparse Boolean tensor over the 41 parity values. Its Tucker decomposition would reveal which subsets of values have combinatorial synergy -- potentially uncovering the algebraic mechanism behind the non-monotonic k behavior.
- **The D96 result (21 > 24) IS a rank-1 Tucker update.** Removing 3 Q8-null entries improved capacity -- the optimal low-rank approximation has fewer entries than the full set. This violates the naive assumption that more data = better. Paper 1's Tucker framework explains why: the removed entries contributed noise to the core tensor without increasing effective rank.

### Relevant Demos
D96 (optimal z8 = 21 entries), D108 (41/17/42 parity/poison/neutral partition)

---

## Connection 8: Temperature = Sector Count (Paper 1 <-> D50, D91)

### The Paper 1 Claim
Applying sigmoid with temperature T: T->0 makes reasoning purely deductive; T->infinity makes reasoning analogical. Different T values for different rules.

### What Knotapel Has
The k-sector MVN activation (D50) partitions the complex plane into k angular sectors. At k=2 (coarsest): 5 NPN classes computable, no parity. At k=6: all 13 classes including parity (906 solutions). At k=8: parity drops to 96 solutions. Non-monotonic.

D91 confirms: varying k_sec from 2 to 48 does not shift the depth gap between XOR levels -- finer resolution reveals more solutions but not higher arity. The parity constraint itself is the wall; activation resolution controls visibility, not capability.

### The Connection
Paper 1's temperature T is DKC's sector count k. Low T (deductive, sharp boundaries) = high k (many sectors, fine discrimination). High T (analogical, soft boundaries) = low k (few sectors, coarse discrimination). The non-monotonic behavior (906@k=6 > 756@k=7 > 96@k=8) is Paper 1's no-free-lunch for temperature: too sharp loses capacity through over-discrimination; too soft loses capacity through under-discrimination. The optimal "temperature" is k=6 for the Z[zeta_8] lattice because gcd(6,8)=2 creates the right incommensurability.

### Leverage Points
- **Paper 1's per-rule temperature could translate to per-function k-sector.** Different Boolean functions might have different optimal k values. D50 already has partial data for this (the four-tier NPN hierarchy: k=2 for 5 classes, k=3 for +1, k=4 for +6, k=6 for +1=parity). A systematic temperature/k sweep per NPN class would complete the picture.
- **The gcd(k,8) anti-correlation (D50) is an incommensurability result that tensor logic's temperature framework cannot yet predict.** This is genuinely new -- no framework in Paper 1 accounts for the interaction between the temperature parameter and the lattice structure of the weight algebra.

### Relevant Demos
D50 (k-sector activation, non-monotonic parity), D63 (four-tier NPN hierarchy), D91 (activation not the bottleneck)

---

## Synthesis: What Group D Papers Would Gain From Knotapel

1. **Paper 1 (Tensor Logic) has no implementation.** DKC's 109 demos of exact cyclotomic tensor arithmetic provide 20,000+ lines of working C89 code that implements the core operations (einsum over algebraic integer tensors, forward chaining via BFS closure, activation functions as step functions).

2. **Paper 2 (Hilbert Space) lacks concrete examples.** DKC provides the most detailed spectral analysis of a computational S^2 function in the literature: 36 solutions decomposed into spherical harmonics with 3.5x compressed sensing, universal spectral envelope, and a proven bandwidth bound (13=13 theorem).

3. **Paper 3 (Edge of Chaos) tests only dynamical complexity.** DKC proves that ALGEBRAIC complexity (non-solvability, derived series depth, BFS depth) predicts computational capacity where dynamical complexity (topological entropy, Lyapunov exponents) is completely orthogonal. This is a fundamental refinement of the edge-of-chaos thesis.

4. **All three papers assume continuous approximation.** DKC works with EXACT arithmetic throughout (zero floating-point error, ever). The exact algebraic integer ring Z[zeta_8] provides a substrate where the papers' theoretical frameworks can be validated without approximation error.

---

## Synthesis: What Knotapel Would Gain From Group D Papers

1. **Paper 2's closed-form kernel regression** could solve the optimal activation problem without search -- one matrix inversion on the 100x100 catalog kernel matrix.

2. **Paper 1's Tucker decomposition** could discover latent structure in the DKC solution space that Raqiya's pairwise graph analysis cannot see (higher-order correlations among triples/quadruples).

3. **Paper 3's Krylov complexity** (as opposed to topological entropy) may be the right complexity measure for algebraic reservoirs -- it tracks operator growth rather than state-space divergence.

4. **Paper 2's Koopman operators** could formalize the depth law (max_xor ~ depth + 6) as a spectral theorem about the braid group reservoir.

5. **The RKHS kernel rank test** (already listed as a next experiment in knotapel's briefing) is precisely Paper 2's core measurement applied to DKC. Running it would simultaneously validate the DKC reservoir thesis AND Paper 2's operator capacity theory.

---

## Priority Ranking of Connections

| Rank | Connection | Reason |
|---|---|---|
| 1 | S^2 spectral DKC = Hilbert space operators (Conn 2) | Mathematical identity, not analogy. D71's spherical harmonic analysis IS Paper 2's spectral reasoning. RKHS kernel rank test is already planned. |
| 2 | DKC as algebraic reservoir at edge of chaos (Conn 6) | Five pillars already identify this. Paper 3 adds measurable complexity (Krylov). D106 falsifies naive dynamical measures. |
| 3 | Forward DKC = tensor logic (Conn 1) | DKC's exact Z[zeta_8] arithmetic IS an implementation of Paper 1's tensor equations over algebraic integers. |
| 4 | Dual-channel = operator decomposition (Conn 4) | D107-109's orthogonality theorem IS Paper 2's spectral decomposition of reasoning operators. |
| 5 | Tucker decomposition of DKC catalog (Conn 7) | New capability: higher-order correlations in the solution space beyond Raqiya's pairwise analysis. |
| 6 | Compressed sensing connections (Conn 5) | D71 and D107 already use CS; Paper 2 provides the RKHS framework to unify them. |
| 7 | Temperature = sector count (Conn 8) | Illuminating analogy but less immediately actionable than the above. |
