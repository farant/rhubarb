# Explorer's Log — DKC Research Arc & Demo 35

Last updated: 2026-02-21
Context: Updated through Demo 82 (Crossing Depth, 17/17 pass). BOMBSHELL: deep entries (born from many generator multiplications) are computationally SUPERIOR to shallow entries at matched catalog sizes. 564 pure depth-8 entries reach XOR12 — the full catalog needed ~1140. Depth IS crossing depth in the knot-theory sense. The logarithmic scaling from Demo 81 reflects the cost of GENERATING deep entries, not an intrinsic capacity limit.

## The Story in One Paragraph

We discovered that the Kauffman bracket polynomial — a topological knot invariant evaluated at A = e^{i5π/4} — produces exact values in the cyclotomic integer ring Z[ζ₈] that can compute Boolean functions WITHOUT neural network training. This is "forward DKC" (Discrete Kakeya Conjecture): topology → algebra → computation. The bracket values live on an algebraic lattice with rigid geometric structure (Mostow rigidity), and the 65,536+ valid XOR triples aren't found by optimization — they're demanded by the algebraic composition rules of braids in 3-space. The parity function was initially unreachable, but Demo 50 proved the wall was in the activation function (split-sigmoid), not the lattice. A 6-sector MVN activation makes all 13 NPN classes computable, including parity (906 solutions). Nobody in the literature connects the four pillars: Habiro (bracket values are cyclotomic integers), Nazer-Gastpar (algebraic integer lattices compute functions), Abramsky (TL diagrams encode computation), and Aizenberg (roots-of-unity neurons for discrete logic).

## Key Concepts

### The Bracket & Z[ζ₈]

- Kauffman bracket: state-sum knot invariant. At A = -ζ₈ = e^{i5π/4}, delta = -(A² + A⁻²) = 0.
- Delta=0 kills all multi-loop states → massive simplification. Only loop-free resolutions survive.
- Bracket values are elements of Z[ζ₈] = {a + bζ₈ + cζ₈² + dζ₈³ : a,b,c,d ∈ Z}. Class number 1 (UFD).

### Forward vs Reverse DKC

- Reverse DKC: Train a neural network, discover weights approximate bracket values. (Demos 26-28)
- Forward DKC: Start from exact bracket values, verify they compute Boolean functions. No training. (Demo 29+)

### The Activation Wall

- Split-sigmoid: classical regime. Phase decoherence. Parity unreachable.
- MVN-continuous (z/|z|): semi-quantum. Topology correlation REVERSES.
- k-sector MVN: k=2→5 NPN classes, k=4→11, k=6→ALL 13 including parity (906 solutions).
- Non-monotonic: 906@k=6 > 756@k=7 > 96@k=8.

### Temperley-Lieb Algebra

- TL_n: algebra of planar diagrams on n strands. Dimension = C_n (Catalan number).
- Generators e_i: connect strand i to strand i+1 on both top and bottom.
- Relations: e_i² = δ·e_i, e_i·e_{i±1}·e_i = e_i, e_i·e_j = e_j·e_i for |i-j|≥2.
- At delta=0: e_i² = 0 (NILPOTENT). Semisimplicity is n-dependent: TL_2 is non-semisimple (radical = span{e_1}), TL_3 is semisimple (radical = {0}). Pattern for n>=4 unknown — this is the central question for Demo 51.
- Braid generators: σ_i = A·I + A⁻¹·e_i.

## Demo 35: TL_n Matrix Representations — COMPLETE

**File:** `knotapel/demo_35_tl_matrices/main.c` (~1000+ lines)
**Tests:** 52/52 pass

### Architecture (6 parts)

- **Part A:** Cyc8 exact arithmetic (copied from demo 29)
- **Part B:** Planar matching enumeration — recursive segment-based algorithm. Generates all C_n non-crossing perfect matchings in boundary order.
- **Part C:** Diagram composition via path-following through glue. Counts closed loops. Builds TL generator matrices (0/1 entries at δ=0).
- **Part D:** Braid generators σ_i = A·I + A⁻¹·e_i over Cyc8.
- **Part E:** Braid word evaluation — matrix product chains, O(k·C_n³).
- **Part F:** Trace closure functional + state-sum cross-validation.

### Key Results

**Sparsity theorem:** TL_n generator matrices at δ=0 have exactly C_n - C_{n-1} nonzero entries. Verified at n=2(1), n=3(3), n=4(9), n=5(28). Reason: C_{n-1} basis elements have a top-top arc at the generator position, creating a loop under composition. The rest survive.

**Cross-validation:** Matrix approach produces EXACTLY the same bracket values as state-sum oracle for all test braids. Exact Cyc8 equality.

**Bracket values at δ=0:**
| Braid | n | Bracket |
|-------|---|---------|
| Hopf link (σ_1²) | 2 | 2 |
| Trefoil (σ_1³) | 2 | -3ζ₈ |
| Figure-eight (σ_1σ_2⁻¹σ_1σ_2⁻¹) | 3 | 5 |
| σ_1σ_2σ_3 | 4 | ζ₈ |
| σ_1²σ_3² | 4 | 0 |

**Compression ratios (distinct matrices / total braids):**
- n=3: 4→13→34→81→182→395 (10.4x at length 6). Exact match to demo 22.
- n=4: 6→27→104→365→1214 (6.4x at length 5). First ever measured.

**Entanglement vanishing theorem:** ALL non-interleaving braids (σ_1 and σ_3 only, no σ_2) at n=4 have bracket = 0. Tested through length 5 (1024 braids, ALL zero). δ=0 is an entanglement detector: only braids where all strands participate in a single connected topological component can have nonzero bracket. At length 5, 40.1% of interleaving braids are nonzero.

**Bracket catalog (n=4, lengths 1-5):** 15 distinct nonzero values, ALL axial (pure integer × single ζ₈ power):
- No mixed elements (like 1+ζ₈). Axiality persists even at length 5.
- Open question: does this break at longer braids or higher n?

### Observations and Open Questions

1. **Figure-eight bracket = 5 = C_3.** Coincidence with TL_3 dimension?
2. **Writhe determines ζ₈ axis:** Even writhe → real bracket, odd writhe → ζ₈ axis.
3. **Axial catalog:** Why are all bracket values pure ζ₈ powers × integers? Is this forced by δ=0, or does it break at longer braids?
4. **Compression scaling:** n=4 compression exceeds n=3 at the same length (3.6x vs 3.2x at length 4). Consistent with hypothesis that larger TL_n has more redundancy.

## What Each Key Demo Established

| Demo | Headline | Why It Matters |
|------|----------|----------------|
| 01 | State-sum bracket works | The O(2^k) oracle. Foundation. |
| 06 | JW idempotents, TL_3 polynomial matrices | First TL matrices. |
| 14 | Bracket zeros ∈ {0,4,8} for torus knots | 8-fold symmetry. |
| 18 | NOT + NAND gates from braids | Universal gate set. |
| 19 | Convergence at A = e^{i5π/4} | The "magic angle" IS -ζ₈. |
| 22 | TL_3 as 5×5 matrices, compression ratios | Direct predecessor to Demo 35. |
| 24 | Rational tangles = TL_2. \|D(n-twist)\| = n. | TL_2 case of Demo 35. |
| 25 | 4 closures always linearly independent | Maximum information extraction. |
| 29 | Forward DKC confirmed. Cyc8 exact arithmetic. | 65,536 XOR triples. |
| **35** | **General TL_n matrices over Cyc8, n=2-5** | **Keystone. Unlocks 7 follow-ons.** |
| 45 | Spearman ρ = -0.82 (topology↔decomposition) | Strongest realist evidence. |
| 47 | MVN reverses topology correlation | Phase coherence, not alignment. |
| 48 | Parity has ZERO forward DKC solutions (split-sig) | Wall is sharp. |
| 50 | Parity at k=6 (906 solutions). Triskelion. | The breakthrough. All 13 NPN. |

### Axiality Theorem (New Discovery)

**Statement:** At δ=0, for ANY braid, the bracket is always axial: ⟨K⟩ = n · A^c for some integer n and some c ∈ {0,1,...,7}. Equivalently, the Cyc8 representation has at most one nonzero component.

**Proof sketch (3 steps):**

**Step 1 — Single-loop isolation.** A single-loop state in the smoothing hypercube has no Hamming-adjacent single-loop neighbors. Proof: from a diagram with exactly 1 closed curve, flipping one crossing smoothing can only SPLIT that curve into 2 (you can't merge because there's nothing to merge with — merging requires 2+ curves). So flipping any single crossing from a 1-loop state always produces a 2-loop state.

**Step 2 — 2-loop states die at δ=0.** A 2-loop state contributes δ^{2-1} = δ = 0. So no Hamming neighbor of a single-loop state contributes to the bracket.

**Step 3 — Phase arithmetic.** Any two single-loop states differ by ≥ 2 crossing flips. Each pair of flips changes σ(s) by 0 or ±4 (depending on flip directions). Since A^4 = -1, states differing by Δσ=4 contribute with opposite signs but on the SAME ζ₈ axis. States with Δσ=0 contribute identically. Therefore ALL contributions lie on a single ζ₈ axis, and the bracket = (algebraic count) × A^c.

**Corollary:** |⟨K⟩| is always a non-negative integer at δ=0. The bracket COUNTS (with signs) the single-loop states, up to a phase.

**Verified:** All 15 distinct bracket values at n=4 through length 5 are axial. Zero mixed elements out of 7776+ braids tested.

### Idempotent Products (Rep Theory Discovery)

Despite generators being nilpotent (e_i² = 0), products of ADJACENT generators are idempotent: (e_i·e_{i+1})² = e_i·e_{i+1}. Proof uses TL relation: (e_1·e_2)² = e_1·(e_2·e_1·e_2) = e_1·e_2.

Consequence: the augmentation ideal J = span{all basis elements except identity} satisfies J² = J. J is NOT nilpotent. Therefore the radical of TL_n at δ=0 is a PROPER subideal of J — it's smaller and more subtle than expected. This is the central question for Demo 51.

### Literature for Demo 51

- **Ridout & Saint-Aubin (2014):** Characterizes TL_n radical via the natural bilinear form. When the Gram matrix of the form degenerates → non-trivial radical. Our δ=0 case is exactly this.
- **Creutzig (2024, arXiv:2411.11383):** Extends Verlinde formula to non-semisimple (logarithmic) CFT using resolutions by standard modules. Our TL_n at δ=0 is the discrete lattice version of his continuum theory.
- **Graham-Lehrer:** TL_n is a cellular algebra. Cell modules and their bilinear forms determine the representation theory completely.

### Four-Lens Summary

- **Rep Theory:** Radical is proper subideal of augmentation ideal. Idempotent products. Cellular algebra structure.
- **Coding Theory:** Axiality collapses Z[ζ₈] to rank-1 sublattice. Message = integer count, phase = writhe label.
- **QM:** δ=0 is topological phase boundary (logarithmic CFT). Entanglement vanishing = product state detection. Axiality = quantization.
- **Approximation Theory:** TL_n is optimal lossless compression of braid group for bracket. Compression → ∞ with braid length.

### Final Demo 35 Statistics

- 52/52 tests passing
- ~98,756 braids tested for axiality (n=2 length 1-10, n=3 length 1-8, n=4 length 1-5)
- Zero counterexamples to axiality
- Corrected n=4 length 5 compression: 1214 distinct / 7776 total = 6.4x

## Demo 51: Radical Anatomy of TL_n at δ=0 — COMPLETE

**File:** `knotapel/demo_51_radical_anatomy/main.c` (~1160 lines)
**Tests:** 57/57 pass (n=2-8, including n=8 at dim 1430)

### The Sandwich Theorem (Confirmed computationally, n=2-8)

For k ≥ 2, TL_{2k}(0) has **Loewy length exactly 3** with three layers:

| Layer | Dimension | Structure |
|-------|-----------|-----------|
| Top: TL_{2k}/rad | C_{2k-1} | ≅ TL_{2k-1}(0) (semisimple) |
| Middle: rad/rad² | C_{2k} - 2·C_{2k-1} | Bimodule connecting top and bottom |
| Bottom: rad² | C_{2k-1} | ≅ TL_{2k-1}(0) as bimodule (Peirce blocks match exactly) |

Top and bottom are dimensional mirrors. Special case: TL_2(0) has Loewy length 2 (dual numbers, rad²=0).

### Confirmed Data Table

| n | dim | rad dim | rad² dim | Gram rank | Nilpotency | Semisimple? |
|---|-----|---------|----------|-----------|------------|-------------|
| 2 | 2 | 1 | 0 | 1=C₁ | 2 | NO |
| 3 | 5 | 0 | — | 5 | — | YES |
| 4 | 14 | 9 | 5=C₃ | 5=C₃ | 3 | NO |
| 5 | 42 | 0 | — | 42 | — | YES |
| 6 | 132 | 90 | 42=C₅ | 42=C₅ | 3 | NO |
| 7 | 429 | 0 | — | 429 | — | YES |
| 8 | 1430 | 1001 | 429=C₇ | 429=C₇ | 3 | NO |

### Sandwich Duality — Peirce Block Verification

rad²(TL_{2k}(0)) has IDENTICAL Peirce block structure to TL_{2k-1}(0):

| Algebra | Peirce blocks (f₀, f₁, f₂) |
|---------|----------------------------|
| TL_3(0) | [[1,1,0],[1,1,0],[0,0,1]] |
| TL_4(0) rad² | [[1,1,0],[1,1,0],[0,0,1]] ← matches TL_3 |
| TL_5(0) | [[5,5,4],[5,5,4],[4,4,6]] |
| TL_6(0) rad² | [[5,5,4],[5,5,4],[4,4,6]] ← matches TL_5 |
| TL_7(0) | [[42,42,48],[42,42,48],[48,48,69]] |
| TL_8(0) rad² | [[42,42,48],[42,42,48],[48,48,69]] ← matches TL_7 |

### Semisimple Decompositions (odd n)
- TL_3(0) ≅ M₂(k) ⊕ k
- TL_5(0) ≅ M₅(k) ⊕ M₄(k) ⊕ k
- TL_7(0) ≅ M₁₄(k) ⊕ M₁₄(k) ⊕ M₆(k) ⊕ k

Simple module dims follow ballot numbers: dim S_{n,t} = C(n,(n-t)/2) - C(n,(n-t)/2-1).

### Catalan Trace Theorem
All nonzero fixed-point counts (traces of left multiplication by basis elements) are Catalan numbers. Confirmed n=2-8. The nonzero values at each n are exactly {C_m : ⌊n/2⌋+1 ≤ m ≤ n} — a window of ⌈n/2⌉ consecutive Catalan numbers:
- n=2: {C₂} = {2}
- n=3: {C₂, C₃} = {2, 5}
- n=4: {C₃, C₄} = {5, 14}
- n=5: {C₃, C₄, C₅} = {5, 14, 42}
- n=6: {C₄, C₅, C₆} = {14, 42, 132}
- n=7: {C₄, C₅, C₆, C₇} = {14, 42, 132, 429}
- n=8: {C₅, C₆, C₇, C₈} = {42, 132, 429, 1430}

### Peirce Decomposition Pattern
Every TL_n(0) (n≥3) admits exactly 3 orthogonal idempotents summing to 1 (from 2(n-1) generator-product idempotents). The Peirce blocks exhibit a consistent 2+1 structure.

### Novelty Assessment
- Alternating semisimplicity: KNOWN (Graham-Lehrer 1996, Ridout-Saint-Aubin 2014)
- Catalan difference formula rad_dim = C_{2k} - C_{2k-1}: likely implicit but not explicitly stated
- Sandwich theorem (Loewy length 3, top≅bottom as bimodule): **NOVEL**
- Catalan trace theorem: **NOVEL**
- Catalan window pattern for fixed-point counts: **NOVEL**
- Connection to DKC framework: **NOVEL** (entirely our territory)

### Architecture
- **Part A:** Planar matching enumeration + composition (from Demo 35)
- **Part B:** Multiplication table (basis × basis → index or -1)
- **Part C:** Integer algebra elements and arithmetic
- **Part D:** Trace form, Gram matrix, radical dimension via Gaussian elimination
- **Part E:** Idempotent search, orthogonal decomposition, Peirce blocks
- **Part F:** Radical filtration (extract null space, compute rad², rad³, ...)
  - Also includes Peirce-restricted-to-subspace computation for sandwich verification

### References
- Graham & Lehrer 1996 — Cellular Algebras (Inventiones Math.)
- Ridout & Saint-Aubin 2014 — Standard Modules, Induction and TL Structure (arXiv:1204.4505)
- Goodman & Wenzl 1993 — TL at roots of unity (Pacific J. Math)

### Next Steps
- Investigate DKC connection: how does the sandwich structure constrain bracket computation?
- Consider cell module radicals (finer than algebra radical) for Vassiliev connection
- The universal nilpotency 3 means the Vassiliev connection isn't through filtration depth — it's through layer dimensions or bimodule structure
- Push to n=9,10 if architecture allows (C_9=4862, C_10=16796 — needs malloc)

### DKC Thermodynamic Connection (Exploratory)

The sandwich structure has a computational interpretation via Landauer's principle:
- The trace form erases the radical (irreversible step)
- The middle layer (rad/rad²) is the "truly lost" information — minimum Landauer cost
- rad² is a shadow copy of the output (redundant by sandwich duality)
- At generic δ: semisimple, no erasure, reversible, but NO Boolean computation
- At δ=0: radical exists, erasure required, irreversible, BUT Boolean functions computable
- **Conjecture:** Computational expressiveness and thermodynamic irreversibility are coupled through the radical

### Runtime Notes
- n=2-7: <1 second total
- n=8: ~12 seconds with -O2 (1430² = 2M mult table entries, 1001-dim radical)
- n=9 (C_9=4862): would need ~4862² ≈ 23M mt entries, ~90MB for mt alone. Feasible with malloc.
- n=10 (C_10=16796): ~280M mt entries, ~1GB mt. Needs serious architecture changes.

## Follow-On Experiments (Demo 52+)

### Demo 52: Compression Scaling
Full compression measurement across n=2-5 at braid lengths 1-8+. Does compression ratio grow with n? Demo 35 data suggests yes (3.6x > 3.2x at length 4 for n=4 vs n=3).

### Demo 53: Catalog Growth
With matrix approach, push to longer braids and higher n. When do mixed Z[ζ₈] elements first appear? Does the catalog saturate?

### Demo 54: Entanglement Classification
Formalize the entanglement vanishing theorem. Classify braids by "entanglement depth" — minimum strand connectivity needed for nonzero bracket. Connect to quantum information theory.

## Four Pillars (Literature)

1. **Habiro 2002** — Bracket values at roots of unity are cyclotomic integers.
2. **Nazer-Gastpar 2011** — Algebraic integer lattices compute functions.
3. **Abramsky 2007** — TL diagrams encode computation. Demo 35 implements this.
4. **Aizenberg 2000** — Roots-of-unity neurons for discrete logic.

## Costello-Francis-Gwilliam (Feb 2026)
Witten = RT is now PROVEN (arXiv:2602.12412). Our Z[ζ₈] computation is rigorously QFT-grounded.

## Realist Thesis
"Bracket values compute Boolean functions because the algebraic composition rules of braids in 3-space DEMAND IT. The neural network is incidental substrate. Computation is in the topology, not the computer."

## Demo 51 — Deep Lens Analyses (Explorer, 2026-02-19)

### QM Lens: Radical as Decoherence Structure

**Core mapping:** The radical at δ=0 is the algebraic analog of decoherence. At generic δ (semisimple), every algebra element is "visible" — the trace form is non-degenerate, like a closed quantum system where all states are distinguishable. At δ=0, the radical appears — some states become invisible to the trace, like quantum coherence lost to the environment.

**Three layers = three quantum sectors:**
- **Quotient** (top): the "classical" sector — observable, extractable by measurement (trace)
- **rad/rad²** (middle): the "quantum" sector — carries information during computation, but irretrievably lost upon measurement. This is genuine decoherence.
- **rad²** (bottom): the "ghost" sector — a redundant copy of the classical sector that lives in the invisible space. Like a quantum clone that exists but can't be accessed.

**No-cloning tension:** The sandwich duality (rad² ≅ quotient) looks like quantum cloning — the output exists twice, once visible and once invisible. But the no-cloning theorem says you can't copy quantum states. Resolution: this isn't a dynamical cloning process. The two copies existed all along in the algebraic structure. The trace "reveals" one copy while "hiding" the other. It's more like complementarity than cloning.

**Quantum error correction connection:** In QEC, you encode information into a subspace such that certain errors (acting on the code space) can be detected. The radical elements are "errors" from the trace form's perspective — they contribute nothing to the measured value. The fact that rad² ≅ quotient is reminiscent of the Knill-Laflamme conditions, where the error space must have specific structure relative to the code space for error correction to work.

**Entanglement stratification:** Basis elements with more cups/caps (fewer through-strands) are "more entangled" and tend to have zero trace. The radical is dominated by highly-entangled elements. The semisimple quotient captures the weakly-entangled ones. So the sandwich layers correspond to entanglement depth: low (quotient), medium (rad/rad²), high but self-similar (rad²).

**Von Neumann limit:** As n→∞, TL_∞ becomes a II₁ factor. The ratio rad_dim/total_dim → 3/4 for even n (since C_{2k}/C_{2k-1} → 4). So asymptotically 75% of the algebra is "decoherent." Only 25% is visible. This 1/4 factor might connect to the channel capacity of the bracket as a quantum channel.

### Algebraic Number Theory Lens: Catalan Windows and Galois Symmetry

**Why Catalan numbers in the trace:** The trace of L_{b_p} counts basis elements fixed by left multiplication by b_p. These fixed elements are planar matchings that are "compatible" with b_p — their strands must thread through b_p's topology without creating closed loops. The count of compatible matchings is always a Catalan number because it reduces to counting planar matchings on a smaller set of strands (the "effective free strands" of b_p).

**The window formula ⌊n/2⌋+1 ≤ m ≤ n:** The maximum trace is C_n (the identity fixes everything). The minimum nonzero trace depends on the minimum number of "effective free strands." Elements with the minimum number of through-strands that still avoid creating loops when composed give trace C_{⌊n/2⌋+1}. Below that threshold, all compositions create loops → zero trace at δ=0.

Window width is ⌈n/2⌉ — approximately half the possible Catalan range. This "half-range" might connect to the Galois theory of the cyclotomic field.

**Galois symmetry conjecture:** Our DKC computation uses Z[ζ₈]. The Galois group Gal(Q(ζ₈)/Q) ≅ (Z/8Z)* ≅ Z/2 × Z/2 has 4 elements. The sandwich duality (top ≅ bottom) could correspond to one of the non-trivial Galois automorphisms (ζ₈ ↦ ζ₈⁻¹ perhaps) exchanging the visible and invisible copies of TL_{2k-1}. The 4-element Galois group and the asymptotic 1/4 visible ratio might not be coincidental.

**Ballot number structure:** The simple module dimensions follow ballot numbers B(n,t) = C(n,(n-t)/2) - C(n,(n-t)/2-1). Ballot numbers are the entries of the Catalan triangle. The fact that semisimple decompositions are given by ballot numbers means the Catalan triangle IS the structure table for TL at δ=0. This connects to the combinatorics of lattice paths, Dyck paths, and the reflection principle.

**Recursive arithmetic:** The Catalan recurrence C_n = Σ C_i · C_{n-1-i} mirrors the recursive structure of the TL algebra (TL_n contains TL_{n-1} as a subalgebra). The sandwich formula C_{2k} = 2·C_{2k-1} + C_{2k-1}·4(k-1)/(2k+1) is a "sandwich recurrence" that decomposes each even Catalan number into its Loewy layers.

### Approximation Theory Lens: Multiresolution Matryoshka

**Telescoping approximation chain:**
TL_8 → TL_7 → TL_6 → TL_5 → TL_4 → TL_3 → TL_2 → k

Each even→odd step: take quotient (project out radical)
Each odd→even step: embed as quotient (the odd algebra sits inside the even one as its semisimple core)

This is a **multiresolution analysis** in the wavelet theory sense:
- V_k = TL_{2k-1}(0) are the "approximation spaces" (semisimple, resolution k)
- W_k = rad/rad² of TL_{2k} are the "detail spaces" (what the even algebra adds beyond the odd core)
- Scaling is by Catalan numbers instead of powers of 2

**Resolution pyramid:**
| Level | Approximation space | Detail space | Dimensions |
|-------|-------------------|-------------|------------|
| k=1   | TL_1 ≅ k (dim 1) | TL_2 detail (dim 1) | 1 + 1 = 2 = C_2 |
| k=2   | TL_3 (dim 5)     | TL_4 detail (dim 4) | 5 + 4 + 5 = 14 = C_4 |
| k=3   | TL_5 (dim 42)    | TL_6 detail (dim 48) | 42 + 48 + 42 = 132 = C_6 |
| k=4   | TL_7 (dim 429)   | TL_8 detail (dim 572) | 429 + 572 + 429 = 1430 = C_8 |

The "detail" grows faster than the "approximation" (ratio → 2), meaning finer resolution adds proportionally more information.

**Chebyshev connection:** TL_n(δ) is closely related to Chebyshev polynomials — quantum dimensions are Chebyshev values, and semisimplicity fails at zeros of Chebyshev polynomials U_{m-1}(δ/2). At δ=0, we're at U_1(0) = 0, the "simplest" root of unity (m=2). The radical structure at other roots of unity (m=3: δ=1, m=4: δ=√2, etc.) would give different Loewy lengths and different sandwich properties. This parametrizes a family of approximation-theoretic structures:
- m=2 (δ=0): Loewy length 3, sandwich with top≅bottom
- m=3 (δ=1): Loewy length ?, potentially deeper filtration
- m=4 (δ=√2): Loewy length ?, etc.

**This is a natural follow-on experiment:** compute the radical structure at δ=1 and δ=√2 and see if the sandwich theorem generalizes.

**Best approximation:** The semisimple quotient TL_{2k-1} is the "best semisimple approximation" to TL_{2k}(0) in a precise sense — it's the largest semisimple quotient. The radical measures the "non-approximability" by semisimple algebras. The sandwich structure says this approximation error has a specific self-similar form (the error's bottom layer IS the approximation).

### Most Actionable Follow-On

**Chebyshev generalization:** Compute radical structure at other roots of unity (δ=1, δ=√2) to see if the sandwich theorem generalizes or if δ=0 is special.

## Demo 51 — Literature Deep Dive (2026-02-19)

### Key Papers Found

1. **Belletête, Ridout & Saint-Aubin (2016), arXiv:1605.05159** — "Restriction and Induction of Indecomposable Modules over the Temperley-Lieb Algebras." Complete classification of all indecomposable TL modules. Section 2.6 (eq. 2.49b): PIM Loewy diagram is diamond with **head ≅ socle = I_k**, Loewy length 3. This is the module-level version of our sandwich theorem.

2. **Gainutdinov, Jacobsen, Read, Saleur & Vasseur (2013), arXiv:1303.2082** — "Logarithmic CFT: a Lattice Approach." Our δ=0 case = **dense polymers, p=2, q=i, central charge c = -2**. Tilting modules (eq. 43) have same diamond structure. Figure 5 shows bimodule structure for exactly our case. Equation 50: scaling limit gives "staggered Virasoro modules" with same head≅socle structure. The lattice (TL) → continuum (Virasoro) correspondence is explicitly shown.

### Notation Translation

| Our notation | Their notation (1605.05159) | Their notation (1303.2082) |
|---|---|---|
| δ = 0 | β = 0 | n = 0, q = i, p = 2 |
| ℓ = 2 | ℓ = 2 | p = 2 |
| TL_n(0) | TL_n(β=0) | TL_{q,N} with q=i |
| Even n non-semisimple | All k ∈ Λ_n non-critical | Tilting modules non-trivial |
| Odd n semisimple | All k ∈ Λ_n critical | Standard = irreducible |
| Loewy length 3 | PIM diamond (eq 2.49b) | Tilting diamond (eq 43) |
| c = -2 CFT | — | c_{1,2} = 1 - 6/2 = -2 |

### Novelty Reassessment

**KNOWN (rediscovered computationally):**
- PIM diamond: head ≅ socle, Loewy length 3 at ℓ=2 (Belletête et al. 2016)
- Module-level sandwich (each PIM individually)
- Alternating semisimplicity (odd/even dichotomy)
- Scaling limit to c=-2 staggered Virasoro modules (Gainutdinov et al. 2013)

**STILL NOVEL (our contributions):**
1. **Algebra-level identification**: rad²(TL_{2k}(0)) ≅ TL_{2k-1}(0) as a specific named algebra (papers prove head≅socle for individual PIMs but never identify the algebra's radical squared AS the previous odd TL algebra)
2. **Peirce block exact matching** — computational verification n=2-8
3. **Catalan trace theorem** — all nonzero fixed-point counts are Catalan numbers
4. **Catalan window formula** — ⌊n/2⌋+1 ≤ m ≤ n
5. **Catalan dimension formulas**: rad dim = C_{2k} - C_{2k-1}, rad² dim = C_{2k-1}
6. **DKC/thermodynamic connection** — entirely our framework
7. **All lens interpretations** (QM decoherence, coding theory, approximation theory, number theory)
8. **Matryoshka/multiresolution structure** — the telescoping chain

### Loewy Length at Higher ℓ (Prediction)

From PIM theory: at level ℓ, PIMs have up to 2ℓ-1 composition factors.
- ℓ=2 (δ=0): Loewy length 3, diamond shape
- ℓ=3 (δ=1): Loewy length up to 5, hexagonal/deeper structure
- ℓ=4 (δ=√2): Loewy length up to 7

**This frames the Chebyshev generalization (Demo 52) as genuinely new computational territory.** Nobody has computed the algebra-level radical anatomy at ℓ=3 or ℓ=4 with explicit dimension formulas.

### Demo 51 Status: COMPLETE

All Build lane steps done. Literature grounded. Novelty confirmed. Ready for Integration lane if needed.

### Seeded: Demo 52 — Chebyshev Generalization

Compute radical anatomy at δ=1 (ℓ=3, Loewy length up to 5) and δ=√2 (ℓ=4, Loewy length up to 7). Key questions:
- Does the named-algebra identification generalize? (Is rad^{2ℓ-2} at level ℓ isomorphic to a specific named TL algebra?)
- What are the intermediate layer dimension formulas?
- Does the Galois coincidence hold? (ℓ=3 → Galois order 2, visible fraction → 1/2?)
- What does a 5-layer or 7-layer multiresolution analysis look like?

## Demo 52: Chebyshev Generalization — Radical Anatomy at δ=1, √2, φ — COMPLETE

**File:** `knotapel/demo_52_chebyshev/main.c` (~1850 lines)
**Tests:** 75/75 pass (n=2-8 at δ=1, n=2-7 at δ=√2 and δ=φ)

### Motivation

Demo 51 established the sandwich theorem at δ=0 (ℓ=2): Loewy length 3, alternating semisimplicity, rad²≅TL_{2k-1}. The Chebyshev connection predicts semisimplicity fails at δ = 2cos(π/ℓ) for each ℓ. Demo 52 asks: does Loewy length 3 persist at other roots of unity?

### Architecture

- **Part A-F:** Same as Demo 51 (planar matchings, mult table, algebra arithmetic, Gram matrix, idempotent search, radical filtration) — refactored to pass-by-pointer for n=8 performance
- **Part G (NEW):** General-delta mod-p framework
  - Parameterized prime `g_mod_p` (global variable, swappable per δ)
  - `mod_sqrt_ts()` — Tonelli-Shanks for modular square roots
  - `alg_mul_delta_mod()` — multiplication with δ^loops weights
  - `compute_radical_dim_delta()` / `extract_radical_basis_delta()` — Gram rank and null space at general δ
  - `analyze_tl_delta()` — full driver: Gram rank → radical basis → filtration

### Key Technique: Mod-p Gaussian Elimination

Instead of exact arithmetic over Z[√2] or Z[φ], we work mod a large prime p where the relevant square root exists. Ranks are invariant mod sufficiently large p (Schwartz-Zippel). Confirmed by cross-checking δ=1 results (general path vs dedicated integer path: identical) and by verifying ℓ=5 results at both p=29 and p=999999751.

### Complete Results

#### δ=1 (ℓ=3), integer arithmetic + mod-p cross-check

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 2 | 3 | 1 | 3 |
| 4 | 14 | 11 | 3 | 1 | 3 |
| 5 | 42 | 18 | 24 | 16 | 3 |
| 6 | 132 | 99 | 33 | 17 | 3 |
| 7 | 429 | 207 | 222 | 170 | 3 |
| 8 | 1430 | 1004 | 426 | 218 | 3 |

#### δ=√2 (ℓ=4), p=10⁹+7

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 5 | 0 | — | semisimple |
| 4 | 14 | 9 | 5 | 1 | 3 |
| 5 | 42 | 33 | 9 | 1 | 3 |
| 6 | 132 | 58 | 74 | 26 | 3 |
| 7 | 429 | 297 | 132 | 36 | 3 |

#### δ=φ (golden ratio, ℓ=5), p=999999751

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2 | 2 | 2 | 0 | — | semisimple |
| 3 | 5 | 5 | 0 | — | semisimple |
| 4 | 14 | 14 | 0 | — | semisimple |
| 5 | 42 | 35 | 7 | 1 | 3 |
| 6 | 132 | 115 | 17 | 1 | 3 |
| 7 | 429 | 270 | 159 | 37 | 3 |

### Universal Nilpotency Conjecture — CONFIRMED

**rad³ = 0 for ALL TL_n at ALL roots of unity tested (ℓ=2,3,4,5).**

Loewy length is universally ≤ 3 (equality when radical is nontrivial and dim > 1). This contradicts the Demo 51 prediction from PIM theory that ℓ=3 might give Loewy length 5 and ℓ=4 might give length 7. The resolution: PIM Loewy length bounds the MODULE filtration depth, but the ALGEBRA radical filtration is shorter because the algebra's Jacobson radical is the intersection of all maximal left ideals — a much smaller object than the sum of all module radicals.

The deeper reason: Graham-Lehrer cellularity. For any cellular algebra, rad² = 0 of each cell module's bilinear form implies Loewy length ≤ 3 for the algebra. TL at any root of unity satisfies this.

### Discovered Formulas (Explorer)

**At the first non-semisimple n (= ℓ):**

| ℓ | δ | n=ℓ rad dim | Formula |
|---|---|------------|---------|
| 2 | 0 | 1 | 2(2)-3 = 1 ✓ |
| 3 | 1 | 3 | 2(3)-3 = 3 ✓ |
| 4 | √2 | 5 | 2(4)-3 = 5 ✓ |
| 5 | φ | 7 | 2(5)-3 = 7 ✓ |

**Formula: rad(TL_ℓ(2cos(π/ℓ))) = 2ℓ - 3**

**At the second non-semisimple n (= ℓ+1):**

| ℓ | n=ℓ+1 rad dim | Formula |
|---|--------------|---------|
| 2 | 1 (n=3 is semisimple at ℓ=2, skip) | — |
| 3 | 3 | 3²-3-3 = 3 ✓ |
| 4 | 9 | 4²-4-3 = 9 ✓ |
| 5 | 17 | 5²-5-3 = 17 ✓ |

**Formula: rad(TL_{ℓ+1}) = ℓ² - ℓ - 3 for ℓ ≥ 3**

**Universal at first pair:** rad² = 1 at n=ℓ and n=ℓ+1 for all ℓ ≥ 3.

**Gram rank at n=ℓ:** C_ℓ - (2ℓ-3)

**Semisimple iff n < ℓ** — confirmed across all four ℓ values.

### Novelty Assessment

**KNOWN (from general theory):**
- Nilpotency ≤ 3 (Graham-Lehrer cellularity, Barth master thesis states it explicitly)
- Semisimple when n < ℓ (standard TL theory)

**NOVEL (our contributions):**
1. **Explicit radical dimension formulas:** rad(TL_ℓ) = 2ℓ-3, rad(TL_{ℓ+1}) = ℓ²-ℓ-3
2. **Cross-ℓ tabulation** with exact dimensions at four different roots of unity
3. **Gram rank sequences** (2,2,11,18,99,207,1004 at ℓ=3) — NOT in OEIS
4. **Universal rad²=1 at first non-semisimple pair** — appears unstated
5. **Mod-p computational methodology** for TL radical anatomy at irrational δ
6. **Confirmation that algebra Loewy length stays at 3** even as module Loewy length predictions grow with ℓ

### Literature Connections

- **Graham-Lehrer 1996:** Cellular algebra theory gives the Loewy length ≤ 3 bound
- **Goodman-Wenzl 1993:** Foundational TL at roots of unity, but no explicit dimension tabulation
- **Leon Barth (master thesis):** States "radical is nilpotent of order 3" — closest explicit match
- **Ridout & Saint-Aubin (arXiv:1204.4505):** Module-level structure, implies our results but doesn't compute them
- **Belletête, Ridout & Saint-Aubin (2016):** PIM diamond structure — module version of our algebra result

### Technical Notes

- Pass-by-pointer refactor eliminated ~23KB stack copies per multiplication at n=8 (AlgElem = 1430 longs = 11,440 bytes)
- n=8 at δ=1: dual-path extraction (float for small dims, mod-p for all dims). Float path got 250/426 radical basis vectors; mod-p completed all 426.
- Prime selection for ℓ=5: 999999751 has 5 as QR, 2 as QR, p≡3(mod 4) for easy Tonelli-Shanks
- Peirce decomposition skipped for n≥8 (dim > 429 threshold) — would need O(dim³) operations on 1430-dim elements

### Open Questions / Next Directions

1. **Extend formulas to n=ℓ+2, ℓ+3, ...** — the rad dims at n=6,7,8 (for ℓ=3) grow in a pattern that might have closed form
2. **Does the sandwich duality generalize?** At δ=0 we had rad²(TL_{2k}) ≅ TL_{2k-1}. At δ=1, is rad²(TL_n) isomorphic to some named algebra?
3. **Peirce blocks at other ℓ** — do they still exhibit the dimensional matching?
4. **Push to n=8 at ℓ=4,5** — feasible with current mod-p architecture but skipped this round
5. **The "1" phenomenon:** Why is rad² always 1-dimensional at the first non-semisimple pair? What generates it?
6. **Connection back to DKC:** The radical at ℓ=3 (δ=1) is the setting where braid evaluation gives Chebyshev polynomials. How does the 3-layer structure constrain bracket computation in this regime?

## Demo 52 — Tensor Product Structure & Fibonacci Rank Theorem (2026-02-19)

### The rad² Generator at n=ℓ

When rad²=1 (at n=ℓ and n=ℓ+1), the single generator has revealing structure:

**Identity-zero property:** At n=ℓ, the rad² generator has zero coefficient on exactly the identity element and nonzero on all other C_ℓ-1 basis elements. This is FORCED: any radical element r must satisfy tr(r·a)=0 for all a; taking a=identity gives tr(r)=0; but tr(r)=δ^loops·(identity coefficient), so the identity coefficient must vanish.

**Zero count at n=ℓ+1:** Exactly ℓ²+1 zero coefficients. Verified: ℓ=3→10, ℓ=4→17, ℓ=5→26, ℓ=6→37. Formula: ℓ²+1.

### Tensor Product Decomposition

The rad² generator's coefficients decompose by **through-strand sectors**. Each planar matching on 2n points has a definite number of through-strands (strands connecting top to bottom). Viewing the coefficient array as a matrix (top matching × bottom matching), it block-decomposes by through-strand count.

**Key discovery:** Each sector's coefficient matrix has **low bilinear rank**, and the extreme sectors (most/fewest through-strands) always have rank 1.

### Quantum Dimension Weights

At ℓ=4 (δ=√2), both sectors are rank 1:
- Sector 0 (0 through-strands, fully nested): factors as u⊗v where u,v have weights proportional to √2 = U₁(√2/2), the quantum dimension
- Sector 2 (maximal through-strands): also rank 1, weights involve √2

At ℓ=3 (δ=1), the single sector (sector 1) is rank 1:
- Signs follow a parity rule: sign = (-1)^(cup_count) × (-1)^(cap_count)
- This is a tensor product: top_sign ⊗ bottom_sign

At ℓ=5 (δ=φ), sector decomposition:
- Sector 3 (3 through-strands): rank 1
- Sector 1 (1 through-strand): rank 2, with φ (golden ratio) appearing in weight vector ratios

At ℓ=6 (δ=√3), sector decomposition:
- Sector 4: rank 1
- Sector 2: rank 3 (palindromic with sector 2 from the other side)
- Sector 0: rank 1

### The Fibonacci Rank Theorem

**Statement:** The total bilinear rank of the rad² generator (sum of ranks across all through-strand sectors) equals F(ℓ-1), the (ℓ-1)th Fibonacci number.

| ℓ | δ | Sectors | Rank per sector | Total rank |
|---|---|---------|-----------------|------------|
| 3 | 1 | {1} | [1] | 1 = F(2) |
| 4 | √2 | {0,2} | [1,1] | 2 = F(3) |
| 5 | φ | {1,3} | [2,1] | 3 = F(4) |
| 6 | √3 | {0,2,4} | [1,3,1] | 5 = F(5) |

**Palindromic conjecture (even ℓ):** The sector rank profile is palindromic for even ℓ: [1,1] at ℓ=4, [1,3,1] at ℓ=6.

**Why Fibonacci?** The TL algebra has an inductive tower structure (TL_n embeds in TL_{n+1}). Fibonacci numbers arise naturally from the fusion rules of TL representations: the number of paths in the Bratteli diagram of the TL tower follows the Fibonacci sequence. The bilinear rank counting the tensor decomposition complexity inherits this recursive structure.

### Rank-2 Sector Decomposition at ℓ=5

Sector 1 (1 through-strand) has 5 top matchings × 5 bottom matchings = 5×5 matrix of rank 2. Decomposes as u1⊗v1 + u2⊗v2 (mod p=999999751).

φ (= U₁(φ/2), the quantum dimension) appears systematically in the ratios of both weight vectors:
- u1 ratios: u1[0]/u1[3] = φ, u1[2]/u1[4] = φ
- v1 ratios: v1[0]/v1[4] = φ, v1[2]/v1[3] = 1/φ

The decomposition is not gauge-unique (depends on row reduction choice), but the φ-weighted structure is robust.

### ℓ=6 Results (δ=√3) — Added This Session

| n | dim | Gram rank | rad | rad² | nilpotency |
|---|-----|-----------|-----|------|------------|
| 2-5 | — | — | 0 | — | semisimple |
| 6 | 132 | 123 | 9 | 1 | 3 |
| 7 | 429 | 402 | 27 | 1 | 3 |

Confirms all formulas with 5th data point (ℓ=2,3,4,5,6):
- rad(TL_6(√3)) = 9 = 2(6)-3 ✓
- rad(TL_7(√3)) = 27 = 6²-6-3 ✓
- rad² = 1 at n=6 and n=7 ✓

### Summary of Novel Contributions This Session

1. **Tensor product structure** of rad² generator by through-strand sector
2. **Quantum dimension weights** (U_k values) in rank-1 sector factorizations
3. **Fibonacci Rank Theorem:** total bilinear rank = F(ℓ-1)
4. **Sector rank profiles** and palindromic conjecture for even ℓ
5. **Identity-zero property** at n=ℓ (forced by trace form)
6. **ℓ²+1 zero count** at n=ℓ+1
7. **ℓ=6 confirmation** of all formulas

### Fibonacci-Anyon Connection (QM Lens)

At ℓ=5 (δ=φ), TL gives the **Fibonacci anyon model** — the simplest universal topological quantum computer. Our F(ℓ-1) bilinear rank equals the number of fusion channels for (ℓ-1) Fibonacci anyons. Both count **length-2 paths through Dynkin diagram A_{ℓ-1}**. The Fibonacci recurrence (rank(ℓ+1) = rank(ℓ) + rank(ℓ-1)) emerges from adding one vertex to the path graph. This is the deepest theoretical insight: the entanglement rank of the thermal ground state follows the same sequence that governs anyonic computational power.

### Decoherence Rate Scaling

The radical proportion (2ℓ-3)/C_ℓ gives a "decoherence rate" that decreases with ℓ:
- ℓ=3: 3/5 = 60%
- ℓ=4: 5/14 = 36%
- ℓ=5: 7/42 = 17%
- ℓ=6: 9/132 = 7%

Higher ℓ = less decoherence = more stable coherent sector. This is WHY Fibonacci anyons (ℓ=5) are better for quantum computing than simpler theories (ℓ=3).

### Galois-Theoretic Mod-p Framework

The mod-p technique is not just a hack — it's **Galois reduction at split primes**. Replacing δ=2cos(π/ℓ) with its F_p image is reduction of an algebraic number modulo a prime that splits completely in Q(δ). The QR condition (e.g., 2 is QR for ℓ=4, 5 is QR for ℓ=5) IS the splitting condition. Chebotarev density theorem guarantees infinitely many valid primes. Agreement at two good primes guarantees true rank via **semicontinuity** — a number-theoretic proof technique, not heuristic.

### ℓ=7 Prediction (Cubic Wall)

Explicit predictions for ℓ=7 (δ = 2cos(π/7), first **cubic** number field):
- **Number field:** Q(2cos(π/7)), degree 3, minimal poly **x³ - x² - 2x + 1 = 0**
- **rad(TL_7)** = 2(7)-3 = **11**
- **rad(TL_8)** = 7²-7-3 = **39**
- **rad²** = 1 at n=7 and n=8
- **Fibonacci Rank Theorem:** bilinear rank = **F(6) = 8**
- **Sector profile:** 3 sectors (1,3,5 through-strands), predict palindromic if odd-ℓ palindromicity holds
- **Technical barrier:** Need completely splitting prime (all 3 roots in F_p). Density 1/6 by Chebotarev.

### Five Lens Summaries

1. **Rep Theory:** Fibonacci from Dynkin A_{ℓ-1} length-2 path compositions. Nilpotency 3 from cellularity (cell modules link in pairs → two drops, never three). Quantum dimensions as categorical weights.
2. **Coding Theory:** Radical as CSS quantum error-correcting code [C_ℓ, 2ℓ-3]. rad² = maximum-weight codeword. Fibonacci tensor rank = channel count. Self-orthogonality under trace form.
3. **QM:** Three quantum sectors (coherent / decoherence / thermal). F(ℓ-1) entanglement rank. Fibonacci anyons at ℓ=5. Decoherence rate decreases with ℓ.
4. **Algebraic Number Theory:** Number field tower Q(2cos(π/ℓ)). Mod-p = Galois reduction. Galois-covariant weights, Galois-invariant ranks. ℓ=7 cubic wall.
5. **Approximation Theory:** Chebyshev double role (parameterizes failure points AND decomposition weights). Superexponential convergence of radical proportion. Universal 3-level multiresolution analysis.

### Open Questions

1. **ℓ=7 cubic wall:** Test all predictions above — this is the strongest validation of universality
2. **Prove Fibonacci:** Can we derive F(ℓ-1) from TL tower fusion rules formally?
3. **Non-palindromic odd ℓ:** ℓ=5 has profile [2,1], ℓ=7 should test whether odd ℓ is always non-palindromic
4. **Interpret rank-2 components:** The φ-weighted vectors at ℓ=5 — are these related to specific cell module morphisms?
5. **DKC connection:** How does the tensor product structure of rad² constrain bracket computation?
6. **CSS code distance:** What is the minimum weight of rad⊥\rad? This determines quantum error-correcting capability.

## Demo 53: DKC at δ=1 (ℓ=3) — Binary Collapse — COMPLETE

**File:** `knotapel/demo_53_dkc_d1/main.c`
**Tests:** 59/59 pass

### Key Results

At δ=1 (ℓ=3), the bracket values collapse to **binary ±1 (times powers of A=ζ₆)**. The catalog has only 6 distinct values: {±1, ±ζ₆, ±ζ₆²}. Magnitudes are always exactly 1.

**Why it's dead for DKC:** Binary values can't separate Boolean function inputs. A split-sigmoid on ±1 just gives constant outputs. k-sector MVN can only distinguish 6 angles (the 6th roots of unity directions), giving at best 6 distinct classification outcomes — not enough for interesting Boolean computation.

**The two-source hypothesis:** DKC computation requires BOTH:
1. Non-trivial radical (nilpotency, from Demo 52) — δ=1 has this ✓
2. Rich bracket catalog (diverse representation) — δ=1 does NOT have this ✗

δ=0 has rich catalog (100 values) but low nilpotency (ℓ=2). δ=1 has higher nilpotency but trivial catalog. δ=√2 (Demo 54) gets BOTH.

### Axiality

100% Z[ζ₆]-axial — all values are pure root-of-unity multiples of ±1.

## Demo 54: DKC at δ=√2 (ℓ=4) — Z[i]-Axiality — COMPLETE

**File:** `knotapel/demo_54_dkc_l4/main.c` (~1800 lines)
**Tests:** 53/53 pass

### Key Results

At δ=√2 (ℓ=4), bracket values live in Z[ζ₁₆] but exhibit **100% Z[i]-axiality**: every value = (a+bi)·ζ₁₆^c for some Gaussian integer (a+bi) and axis c ∈ {0,1,...,7}. Verified across 89,426 braids (n=2 len 1-10, n=3 len 1-8, n=4 len 1-6).

**Catalog:** 56 distinct nonzero bracket values (vs 100 at δ=0, vs 6 at δ=1).

**Gaussian integer catalog structure:**
- n=2: 8 distinct GIs, 4 negation pairs (ALL pair up)
- n=3: 12 distinct GIs, 6 negation pairs (ALL pair up)
- n=4: 15 distinct GIs, 7 negation pairs (one unpaired: (2-2i) missing (-2+2i))

**Complete negation pairing** is algebraically forced: A⁸ = ζ₁₆⁴⁰ = -1, so adding 8 crossings negates the bracket.

### Axiality Hierarchy

| ℓ | δ | Ring | Axiality | Catalog | Ent. Vanish | DKC Richness |
|---|---|------|----------|---------|-------------|-------------|
| 2 | 0 | Z[ζ₈] | Z-axial | 100 | 100% | Rich (all 13 NPN at k=6) |
| 3 | 1 | Z[ζ₆] | Z-axial | 6 | 0% | Dead (binary collapse) |
| 4 | √2 | Z[ζ₁₆] | Z[i]-axial | 56 | 43.7% | **Richest** (all 13 NPN at k=2!) |

**Entanglement vanishing** = % of non-interleaving n=4 braids (σ₁, σ₃ only) with zero bracket.
- δ=0: 100% vanish (perfect entanglement detector)
- δ=1: 0% vanish (all ±1, nothing is zero)
- δ=√2: 43.7% vanish — partial preservation, interpolating between extremes

## Demo 55: DKC Boolean Search at ℓ=4 — PARITY AT Re>0 — COMPLETE

**File:** `knotapel/demo_55_dkc_boolean/main.c` (~1040 lines)
**Tests:** 7/7 pass

### THE HEADLINE RESULT

**Re(z) > 0 — the simplest possible activation function — gives the MOST parity solutions (9,334) at δ=√2.**

At δ=0, parity required k=6 MVN sectors (906 solutions). At δ=√2, a half-plane suffices. The root of unity removes the activation wall entirely.

### Architecture

- **Part A:** Build bracket catalog from state-sum (n=2 len 1-10, n=3 len 1-6, n=4 len 1-6)
- **Part B:** 2-input multiplicative search: z(x₁,x₂) = w₁^x₁ · w₂^x₂, quartet = (1, w₂, w₁, w₁·w₂)
- **Part C:** 3-input multiplicative search with NPN classification (13 non-trivial classes)
- **Part D:** k-sector sweep analysis, comparison with δ=0

### 2-Input Results (16 functions)

8/16 achievable with any angle-based activation. Structural constraint: z(0,0)=1 always → activation fires or doesn't on 1 → bit 0 fixed → two complementary halves:
- Re>0 gets: AND, PROJ_A, PROJ_B, OR, XNOR, A≥B, B≥A, TRUE (bit 0 = 1)
- Sector/sigmoid gets: FALSE, A>B, B>A, XOR, NOR, NOT_A, NOT_B, NAND (bit 0 = 0)

Together they cover all 16 functions.

### 3-Input Results (13 NPN classes) — LANDMARK

| Activation | Reachable | Parity | Notes |
|-----------|-----------|--------|-------|
| Re(z) > 0 | 11/13 | **9,334** | Simplest, most parity |
| Im(z) > 0 | 12/13 | 3,114 | |
| Split-sigmoid | 13/13 | 481 | All classes, least parity |
| Sector k=2 | 13/13 | 5,314 | |
| Sector k=4 | 13/13 | 6,712 | |
| Sector k=6 | 13/13 | 5,314 | |
| Sector k=8 | 13/13 | 8,000 | |
| Magnitude τ=1.0 | 3/13 | 0 | Very weak |

### Difficulty Hierarchy Reshuffling

The computational landscape at δ=√2 is fundamentally different from δ=0:

| Class | δ=0 split-sig | δ=√2 Re>0 | Change |
|-------|--------------|-----------|--------|
| XNOR3 (parity) | 0 (impossible) | 9,334 | **impossible → abundant** |
| ~A(B^C) | 1,402,076 | 0 | **abundant → impossible** |
| 3v-0x1B | 1,690,752 | 0 | **abundant → impossible** |
| EXACT1 | 126,666 | 6,568 | hard → hard |

Key observations:
- Parity goes from the ONLY impossible class at δ=0 to mid-pack at δ=√2
- Two mid-tier classes (~A(B^C), 3v-0x1B) become impossible at δ=√2 with Re>0
- Split-sigmoid at δ=√2 reaches ALL 13 including those two, confirming they're algebraically available — only geometrically hidden from half-plane tests
- The algebra and activation trade difficulty profiles: choose the activation to choose which functions are easy

### The Principle

**The root of unity is a computational resource.** Algebraic dimension (Z→Z[i]) substitutes for activation complexity (k=6→Re>0). The Gaussian integer lattice geometry at ℓ=4 does the computational heavy lifting that the activation function had to do at ℓ=2.

### Trade-off Pattern

Simple activations (Re>0, Im>0) miss 1-2 obscure classes but have MORE parity solutions. Complex activations (sector, sigmoid) get all 13 classes but with fewer parity solutions. The rich Gaussian integer geometry means you don't need activation complexity to separate.

### The Two-Source Hypothesis — CONFIRMED

DKC computation requires BOTH nilpotency AND representation diversity:
- δ=0 (ℓ=2): Rich catalog (100 values) but minimal radical → parity needs k=6
- δ=1 (ℓ=3): Deeper radical but binary catalog → dead zone
- δ=√2 (ℓ=4): Rich catalog (56 values) AND non-trivial radical → parity at Re>0
- Key: ℓ=4 is the sweet spot where both sources of computational power are present

### Strict Dominance Result

**δ=√2 strictly dominates δ=0 for split-sigmoid:** 13/13 NPN classes vs 12/13 (parity gained, nothing lost). This is a clean algebra-level result independent of activation choice.

The Re>0 comparison is more nuanced (trades 2 obscure classes for 9,334 parity solutions), but that's an activation choice, not an algebra limitation — split-sigmoid at δ=√2 reaches all 13 classes.

### Comparison with δ=0

- δ=0: 100 catalog values, parity requires k=6 MVN (906 solutions)
- δ=√2: 56 catalog values, parity with Re>0 half-plane (9,334 solutions)
- The Z[i] structure provides ~10× the parity solution density of Z[ζ₈] despite having fewer catalog values
- Encodings differ (additive at δ=0 vs multiplicative at δ=√2) — not directly comparable, but the structural parallels are striking

## Demo 56: Potts-TL Dictionary — Ising Partition Functions Compute Boolean Functions — COMPLETE

**File:** `knotapel/demo_56_potts_tl/main.c`
**Tests:** 17/17 pass

### Core Idea

δ² = Q in the Potts model, so the Kauffman bracket IS a Potts partition function:
- ℓ=3 (δ=1) → Q=1 = bond percolation (trivial)
- ℓ=4 (δ=√2) → Q=2 = **Ising model**
- ℓ=6 (δ=√3) → Q=3 = 3-state Potts

The Fortuin-Kasteleyn partition function: Z_FK(G; Q, v) = Σ_{A⊆E} v^|A| · Q^{k(A)}

### Headline Results

**1. FK = Ising spin sum: CONFIRMED** (7/7 cross-validation on lattice strips)

**2. Z[i]-axiality is a BRAID property, not an algebra property.**
- Bracket values are Z[i]-axial (2D over Z) — the specific A, A⁻¹ coefficients in σ_i create cancellations
- FK partition functions live in Z[ζ₈] = Z[i, √2] (4D over Z) — the Q^{k(A)} weighting breaks axiality
- Both come from TL_n(√2), but FK uses the FULL even subring

**3. Critical coupling gives real values.**
At v = √2 (self-dual Ising critical temperature), all FK values are in Z[√2]:
- Pattern: a + b·√2 (e.g., S(2,2) = 72 + 48·√2 ≈ 139.88)

**4. Q=1 → Q=2 is a computational phase transition.**
- Q=1: Z_FK = (1+v)^|E| always — no geometric information (confirms Demo 53)
- Q=2: Each strip geometry gives a distinct Z — the lattice topology matters

### FK DKC Boolean Search — THE PHYSICS HEADLINE

**"The Ising partition function computes 12 of 13 NPN Boolean function classes, including parity, with a 2-sector activation."**

FK catalog: 9 distinct partition function values from lattice strips S(w,h) at bracket coupling (Q=2, v=-ζ₁₆⁶).

| Activation | NPN Reachable | Parity | Missing |
|-----------|-----------|--------|---------|
| Re(z) > 0 | 11/13 | 113 | ~A(B^C), 3v-0x1B |
| Split-sigmoid | 10/13 | 19 | ~A(B^C), 3v-0x1B, A^(B\|C) |
| Sector k=2 | **12/13** | 32 | **3v-0x1B only** |
| Sector k=4 | 10/13 | 19 | ~A(B^C), 3v-0x1B, A^(B\|C) |
| Sector k=6 | 11/13 | 32 | ~A(B^C), 3v-0x1B |

### Comparison: FK vs Bracket

- Bracket Re>0 (56 values): 11/13 NPN — same as FK Re>0 (9 values)!
- FK sector k=2 (9 values): 12/13 NPN — **better** than any single bracket activation
- 4D algebraic structure (Z[ζ₈]) gives more angular diversity, which sector activations exploit
- Only 9 catalog values achieve nearly the same coverage as 56 bracket values

### The 0x1B Wall: Five-Lens Analysis — RESOLVED

**CORRECTION:** 3v-0x1B is NOT universally impossible. Full (activation × catalog) matrix:

**Bracket catalog (56 values):**
| Activation | 0x1B solutions |
|-----------|---------------|
| Re(z) > 0 | 0 |
| Im(z) > 0 | 0 |
| Split-sigmoid | **144** |
| Sector k=2 | **480** |
| Sector k=4 | 384 |
| Sector k=6 | 480 |
| Sector k=8 | **2,496** |
| Magnitude | 0 |

**FK catalog (9 values):** 0 across ALL activations (catalog too small).

**δ=0 (Demo 48):** 1,690,752 solutions with split-sigmoid.

**Two-type wall classification:**
1. **Algebraic wall** (parity at δ=0): Fundamental — the algebra itself lacks the structure. Dissolves only by changing ℓ.
2. **Encoding wall** (0x1B at δ=√2 Re>0): Contingent on encoding+activation combination. Dissolves with sector activations at the SAME ℓ.

**Coding theory criterion** — the sharpest lens:
- 0x1B TRUE set = {000, 001, 011, 100}. Check linearity: 001⊕011 = 010 ∉ set. **Non-linear.**
- ~A(B^C) TRUE set (0x06) = {001, 010}. Check: 001⊕010 = 011 ∉ set. **Also non-linear.**
- PARITY TRUE set = {000, 011, 101, 110}. Check: 011⊕101 = 110 ✓, 000⊕011 = 011 ✓, etc. **Linear** (dual of repetition code).
- The two classes that Re>0 misses are exactly the ones with non-linear TRUE sets incompatible with the (Z/2)³ group structure of multiplicative encoding under a single half-plane cut.

**Five-lens summary:**
1. **QM:** 0x1B's TRUE set has asymmetric, W-state-like correlation structure — genuinely 3-party correlated, resists the tensor-product (separable) structure of multiplicative encoding.
2. **Coding theory:** Non-linear code → fights against (Z/2)³ group structure. Linear TRUE sets (parity, XOR2) are compatible with multiplicative encoding; non-linear ones (0x1B, 0x06) are not — under half-plane activations.
3. **Number theory:** Half-plane activation = single arc in angular ordering of group elements. 0x1B requires the TRUE set elements to be in a non-contiguous angular arrangement that can't be captured by a single arc. Sector activations (multiple arcs) resolve this.
4. **Approximation theory:** All three weights must "cross the imaginary axis" in conflicting ways. The angular constraints are over-determined for a single boundary but resolvable with two or more boundaries (sector k≥2).
5. **Rep theory:** Braid symmetry (σ_i symmetric under permutation) constrains which partitions the trace image can produce. The asymmetry of 0x1B conflicts with this, but not fatally — sector activations break the symmetry enough.

**Prediction:** Additive encoding z(a,b,c) = a·w₁ + b·w₂ + c·w₃ would dissolve the Re>0 wall for 0x1B, because addition doesn't impose the group/arc constraint. The TRUE set maps to {0, w₃, w₂+w₃, w₁} — four independent points with no multiplicative coupling.

**Conclusion:** The 0x1B obstruction is understood and NOT fundamental. It's a property of multiplicative encoding + half-plane activation, not of the algebra or physics.

### Updated Axiality Hierarchy

| ℓ | δ | Bracket Ring | FK Ring | Bracket DKC | FK DKC |
|---|---|-------------|---------|-------------|--------|
| 2 | 0 | Z[ζ₈], Z-axial | — | 13/13 at k=6 | — |
| 3 | 1 | Z[ζ₆], Z-axial | Q=1 trivial | Dead | Dead |
| 4 | √2 | Z[ζ₁₆], Z[i]-axial (2D) | Z[ζ₈] (4D) | 13/13 split-sig | **12/13 at k=2** |

### Open Questions for Future Sessions

1. **ℓ=5 (δ=φ, Fibonacci anyons):** What is the axiality ring? Predict Z[ζ₅]-axial or something richer. Extends axiality hierarchy to the topological quantum computation regime.
2. **Prove Z[i]-axiality theoretically:** 89,426 braids confirm computationally. Can we prove it from the state-sum structure at ℓ=4?
3. **Additive encoding experiment:** Verify prediction that additive encoding dissolves the 0x1B and 0x06 Re>0 walls. Would confirm the coding theory criterion.
4. **FK catalog expansion:** More lattice sizes, boundary conditions (periodic, twisted), non-rectangular graphs. Could a richer FK catalog reach 0x1B with sectors?
5. **Q=3 (3-state Potts at ℓ=6):** Does the Potts model get richer at Q=3? The FK ring would live in Z[ζ₁₂].
6. **Complete linearity classification:** Which of the 13 NPN classes have linear/affine TRUE sets? This would fully characterize the multiplicative+half-plane achievability boundary.

---

## Demo 57: Additive Encoding DKC (2026-02-19)

**Headline: Prediction refuted, convexity principle discovered.**

### Background

Demo 56's five-lens analysis predicted that additive encoding z(x₁,...,xₙ) = x₁·w₁ + x₂·w₂ + ... + xₙ·wₙ would dissolve the 0x1B/0x06 Re>0 wall. The reasoning: multiplicative encoding imposes (Z/2)³ group structure that conflicts with non-linear TRUE sets; additive encoding removes that coupling.

### Results (13/13 tests pass)

**The prediction was WRONG.** Additive encoding makes things *worse*, not better.

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | 12/13 | 5/13 | 12/13 | (none) |
| Split-sig | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=2 | 13/13 | 11/13 | 13/13 | (none) |
| Sector k=4 | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=6 | 13/13 | 13/13 | 13/13 | (none) |
| Sector k=8 | 13/13 | 13/13 | 13/13 | (none) |
| Mag τ=1 | 3/13 | 12/13 | 12/13 | 9 classes |

Key observations:
- **Additive Re>0 = only 5/13** (AND3', AND2', ~A~(BC), BUF, MAJ'). Down from 11/13 multiplicative.
- **Additive never reaches a class multiplicative doesn't** — 0 add-only classes at every activation.
- **Magnitude τ=1 is the ONE exception:** additive gets 12/13 vs mult's 3/13. But magnitude doesn't use angular information at all.
- **Union never exceeds multiplicative.** Additive is strictly dominated.

### The Convexity Principle

Why additive is worse: **half-plane activations (Re>0, Im>0) are convex constraints.** If Re(w₁) > 0 and Re(w₂) > 0, then Re(x₁·w₁ + x₂·w₂) > 0 for any x₁,x₂ ≥ 0. Additive sums of points in a convex region stay in that region. This means additive encoding CANNOT produce the angular diversity needed for complex Boolean functions.

Multiplicative products, by contrast, can rotate out of a half-plane: Re(w₁) > 0 and Re(w₂) > 0 does NOT imply Re(w₁·w₂) > 0.

The 5 additive-reachable classes under Re>0 are exactly the "monotone-compatible" functions whose TRUE sets are compatible with all-positive sums. The more complex functions (0x16 EXACT1, 0x18 ISOLATE, etc.) need angular structure that only multiplication provides.

### Theoretical Significance

This refutation is actually *more informative* than confirmation would have been:
1. The wall isn't about coding theory group structure — it's about **activation geometry**
2. Convexity is the mechanism: half-planes are convex, sums preserve convexity, products don't
3. The 0x1B/0x06 obstruction sits at a deeper level than encoding choice

---

## Demo 58: Fibonacci Anyon DKC (2026-02-19)

**Headline: Wall holds at ℓ=5 — 11/13 is likely a theorem, not an empirical accident.**

### Background

ℓ=5 gives δ = φ (golden ratio), the Fibonacci anyon regime. This is where Freedman-Larsen-Wang proved TQC universality: braid representations are dense in PSU(2). The bracket ring is Z[ζ₅] (rank 4, vs rank 2 Z[i] at ℓ=4). If anything could break the 11/13 wall, this should — richer algebra, denser representations, 4D catalog.

### Implementation

- **Cyc5 arithmetic:** Z[ζ₅] with 4 coefficients, minimal polynomial ζ₅⁴ + ζ₅³ + ζ₅² + ζ₅ + 1 = 0
- **Bracket variable:** A = -ζ₅⁴ = (1,1,1,1) in Cyc5, order 10
- **Delta powers via Fibonacci:** φ^k = F(k-1) + F(k)·φ = (F(k-1), 0, -F(k), -F(k)) in Cyc5
- **DKC search uses complex doubles** (not Cyc5) for activation evaluation — avoids integer overflow from Fibonacci-growth coefficients
- **16/16 tests pass, clean compile first try**

### Results

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | 12/13 | 5/13 | 12/13 | (none) |
| Split-sig | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=2 | 13/13 | 11/13 | 13/13 | (none) |
| Sector k=4 | 13/13 | 12/13 | 13/13 | (none) |
| Sector k=6 | 12/13 | 13/13 | 13/13 | 0x1B |
| Sector k=8 | 13/13 | 13/13 | 13/13 | (none) |
| Mag τ=1 | 6/13 | 12/13 | 12/13 | 6 classes |

### The Wall is Algebra-Independent

The most important finding: **Re>0 + mult = 11/13 at BOTH ℓ=4 and ℓ=5.** Same two missing classes (0x1B and 0x06). Same Im>0 pattern (12/13, missing only 0x1B). Same additive behavior (5/13 at Re>0).

This despite:
- Ring rank doubling: Z[i] (2D) → Z[ζ₅] (4D)
- Catalog doubling: 56 values → 116 values
- TQC universality at ℓ=5 (Freedman-Larsen-Wang)
- Fully 4D catalog (all four basis coordinates used)

### Catalog Analysis at ℓ=5

- 116 distinct values (vs 56 at ℓ=4)
- Mostly 3-coefficient (55 entries) or 4-coefficient (31 entries)
- Only 32/116 have their negative also in catalog
- Max |coefficient| = 6

### Im>0 Algebra Independence (Cross-ℓ Investigation)

After Demo 58 completed, we investigated whether Im>0 results also match across ℓ values:

- **ℓ=4 Im>0 mult:** 12/13 (0x1B = 0 solutions, 0x06 = 1248 solutions)
- **ℓ=5 Im>0 mult:** 12/13 (0x1B = 0 solutions, 0x06 = reachable)

Pattern is IDENTICAL. The Re↔Im asymmetry comes from **z(0,0,0) = 1** (positive real, always in Re>0 half-plane but at ±45° to Im axis). This structural fact about multiplicative encoding is algebra-independent.

### 0x1B Resistance Profile

0x1B (truth table 00011011) is the most resistant class:
- **Fails** Re>0 at both ℓ=4 and ℓ=5
- **Fails** Im>0 at both ℓ=4 and ℓ=5
- **Fails** k=6 sectors at ℓ=5 (but reachable at ℓ=4)
- **Fails** magnitude at both ℓ values
- **Reachable** only via: split-sigmoid, sectors k≥2 (ℓ=4) or k≥8 (ℓ=5), or additive k≥6

By contrast, 0x06 fails ONLY Re>0 — it's reachable by Im>0, all sector sizes, and magnitude.

### Updated Axiality Hierarchy

| ℓ | δ | Bracket Ring | DKC Re>0+mult | DKC Im>0+mult | Catalog Size |
|---|---|-------------|---------------|---------------|-------------|
| 2 | 0 | Z[ζ₈], Z-axial | 13/13 (k=6) | — | — |
| 3 | 1 | Z[ζ₆], Z-axial | Dead | Dead | — |
| 4 | √2 | Z[ζ₁₆], Z[i]-axial (2D) | 11/13 | 12/13 | 56 |
| 5 | φ | Z[ζ₁₀], Z[ζ₅]-axial (4D) | **11/13** | **12/13** | 116 |

### Theoretical Implications

1. **The wall is a theorem, not an observation.** Same boundary across two different algebras with very different properties rules out coincidence.
2. **The obstacle is in Boolean function structure + activation geometry**, not in the ring. 0x1B's TRUE set = {000, 001, 011, 100} has an intrinsic property that conflicts with single-boundary activations applied to multiplicative encoding.
3. **TQC universality doesn't help DKC.** Freedman-Larsen-Wang guarantees dense braid representations — but density in PSU(2) doesn't translate to angular diversity in the scalar bracket output. The bracket maps braids to scalars, losing the SU(2) structure.
4. **Demo 61 (linearity classification) is the natural next step** — to prove the wall as a theorem about GF(2) linearity of NPN TRUE sets.

---

## Demo 59: Q=3 Potts DKC at ℓ=6 (2026-02-19)

**Headline: Universal half-plane theorem — 11/13 holds at ALL orientations, ALL algebras.**

### Background

ℓ=6 gives δ = √3, the Q=3 Potts model. The bracket ring is Z[ζ₂₄] (rank 8, double the rank-4 of ℓ=5). The bracket variable A = ζ₂₄⁷ has order 24. This is the most algebraically complex regime we've tested: 8 coefficients per ring element, 24-entry A power table.

### Implementation

- **Cyc24 arithmetic:** Z[ζ₂₄] with 8 coefficients, minimal polynomial Φ₂₄(x) = x⁸ − x⁴ + 1
- **Reduction rule:** ζ₂₄⁸ = ζ₂₄⁴ − 1. For degree k ≥ 8: temp[k−4] += temp[k], temp[k−8] −= temp[k]
- **Delta powers via √3 pattern:** δ^{2k} = 3^k (pure integer), δ^{2k+1} = 3^k·√3 = (0,0,2·3^k,0,0,0,−3^k,0)
- **A power table:** 24 entries, A^k = ζ₂₄^{7k mod 24}. Most entries 1-coeff or 2-coeff.
- **Part F (rotated half-plane sweep):** Tests σ_θ(z) = [Re(e^{iθ}·z) > 0] at 24 angles (every 15°)
- **18/18 tests pass, clean compile first try**

### Results

| Activation | Mult | Add | Union | Add-only |
|-----------|------|-----|-------|----------|
| Re>0 | 11/13 | 5/13 | 11/13 | (none) |
| Im>0 | **13/13** | 5/13 | 13/13 | 0x1B,0x06 |
| Split-sig | 12/13 | — | — | — |
| Sector k=2 | 13/13 | — | — | — |

### The Headline: Rotated Half-Plane Sweep (Part F)

After noticing Im>0 = 13/13 (breaking the ℓ=4/5 pattern of 12/13), we ran a sweep testing all 24 half-plane orientations at 15° intervals:

**ALL 24 angles give EXACTLY 11/13.** Including θ=90° and θ=270° (the Im axis orientations).

The Im>0 = 13/13 from Part C is a **floating-point boundary artifact**:
- cos(270°) in IEEE-754 ≈ −1.8×10⁻¹⁶, not exactly 0
- The rotated activation σ_{270°} has tiny Re contamination: σ ≈ Im(z) − Re(z)×1.8×10⁻¹⁶
- For near-real z-values (|Im| ≈ 0), this contamination flips classifications
- Part C's "pure" Im>0 uses `z.im > 0.0` with zero Re component
- The 13/13 is **infinitely fragile** — any rotation, no matter how small, restores 11/13

### Universal Half-Plane Theorem (Refined)

For ALL half-plane activations σ_θ(z) = [Re(e^{iθ}z) > 0] with multiplicative encoding:
- **Exactly 11/13 NPN classes are reachable**
- **Missing: 0x1B and 0x06**
- **Independent of algebra:** ℓ=4 (Z[i], rank 2), ℓ=5 (Z[ζ₅], rank 4), ℓ=6 (Z[ζ₂₄], rank 8)
- **Independent of orientation:** All 24 tested angles identical
- **Robust to perturbation:** Not a boundary effect; holds under arbitrary rotation
- **Complement symmetry confirmed:** σ_θ and σ_{θ+π} produce complementary truth tables → identical NPN classes

### Catalog Surprise

Despite the rank-8 ring Z[ζ₂₄], only **29 distinct bracket values** were generated (vs 56 at ℓ=4, 116 at ℓ=5). All entries use only 1 or 2 nonzero coefficients. The A power table entries are mostly 1-coeff or 2-coeff, and delta powers are 1-2 coeff, so products stay low-complexity. The catalog is *smaller* despite the ring being *larger*.

### Updated Axiality/DKC Hierarchy

| ℓ | δ | Ring | Rank | Re>0+mult | Im>0+mult | Half-plane sweep | Catalog |
|---|---|------|------|-----------|-----------|-----------------|---------|
| 4 | √2 | Z[ζ₁₆] | 2 (Z[i]-axial) | 11/13 | 12/13 | — | 56 |
| 5 | φ | Z[ζ₁₀] | 4 (Z[ζ₅]-axial) | 11/13 | 12/13 | — | 116 |
| 6 | √3 | Z[ζ₂₄] | 8 (full) | **11/13** | 13/13* | **11/13 all angles** | 29 |

*floating-point boundary artifact; any perturbation drops to 11/13

### Theoretical Implications

1. **The 11/13 wall is a universal half-plane theorem.** It's not about any particular axis or any particular algebra — it's about the geometry of half-planes + multiplicative encoding + Boolean function structure.
2. **Orientation independence is stronger than algebra independence.** Not only does the wall hold across ℓ=4,5,6, it holds across ALL rotations of the decision boundary.
3. **The obstacle is intrinsic to 0x1B and 0x06.** Their TRUE sets have a property that makes them unreachable by ANY half-plane activation on multiplicative z-values, regardless of the underlying ring.
4. **Non-monotonic catalog size:** 56 → 116 → 29 as rank goes 2 → 4 → 8. Algebraic complexity of the ring does NOT predict catalog richness.
5. **Im>0 "breakthrough" was illusory.** The boundary effect at exactly Im=0 is an artifact, not algebra. The clean theorem is about generic half-planes only.

### Next Steps

- Demo 60 (ℓ=7, first cubic number field) — sharp test of radical dimension universality
- Demo 61 (linearity classification) — prove the wall as a theorem about GF(2) linearity
- Analytical proof attempt — the orientation-independence suggests a proof via the multiplicative structure of z(0,0,0) = 1

---

## Demo 61: Angular Proof of 11/13 Half-Plane Theorem (2026-02-20)

**Headline: THEOREM PROVEN — 0x06 and 0x1B are unreachable for ALL angles, ALL orientations.**

### The Theorem

For all half-plane activations σ_θ(z) = [Re(e^{iθ}z) > 0] on multiplicative encodings z(a,b,c) = w₁^a · w₂^b · w₃^c, exactly 11 of 13 non-trivial NPN classes are reachable. The unreachable classes are **0x06 (XOR-AND)** and **0x1B (CROSS)**, independent of angles (φ₁,φ₂,φ₃) and half-plane orientation θ.

### Method: Pure Integer Proof

Key insight: half-plane classification depends ONLY on the angle of each z-value. With multiplicative encoding, angle(z(a,b,c)) = a·φ₁ + b·φ₂ + c·φ₃ (mod 2π). Represent angles as integers mod N. A semicircle [θ, θ+N/2) selects points by pure integer comparison. **Zero floating-point arithmetic** — the entire proof is in exact integer arithmetic.

- Exhaustive search over all (p₁,p₂,p₃) ∈ {0,...,N-1}³
- For each triple: compute 8 angles mod N, test 8 semicircle positions
- NPN-classify each truth table via precomputed lookup table

### Results (12/12 tests pass)

- **N=120** (1.7M triples): 12 classes found, 0x06 and 0x1B absent ✓
- **N=360** (46.7M triples): 12 classes found, 0x06 and 0x1B absent ✓
- Both searches agree perfectly
- All 12 expected achievable classes found with witnesses

### The 12 Achievable Classes

| Canon | Name | Weight | Affine | Witness (p₁,p₂,p₃) |
|-------|------|--------|--------|---------------------|
| 0x00 | FALSE | 0 | Y | (0,0,0) |
| 0x01 | AND3 | 1 | Y | (1,1,1) |
| 0x03 | AND2 | 2 | Y | (0,1,1) |
| 0x07 | OR-NAND | 3 | N | (1,1,2) |
| 0x0F | BUF | 4 | Y | (0,0,1) |
| 0x16 | EXACT1 | 3 | N | (2,179,179) |
| 0x17 | MINORITY | 4 | N | (1,1,1) |
| 0x18 | ISOLATE | 2 | Y | (1,1,179) |
| 0x19 | 3-SELECT | 3 | N | (1,2,179) |
| 0x1E | XOR-OR | 4 | N | (1,1,180) |
| 0x3C | XOR2 | 4 | Y | (0,1,180) |
| 0x69 | PARITY | 4 | Y | (1,180,180) |

Witnesses with 179 or 180 use angles near π — the angular diversity needed for complex Boolean functions. Simpler classes use small angles (points clustered).

### Structural Analysis

**Affinity does NOT distinguish reachability.** 0x06 is affine but unreachable; 0x07 and 0x16 are non-affine but reachable. The obstruction is purely geometric: semicircle separability on an additively-structured circle.

**Why 0x06 is unreachable:** TRUE set = {001, 010}. Requires angles φ₃ and φ₂ in semicircle with their sum φ₂+φ₃ outside. Since the angles are additive, having two angles in a semicircle constrains their sum in a way that makes this impossible for any angular configuration.

**Why 0x1B is unreachable:** TRUE set = {000, 001, 011, 100}. Requires {0, φ₃, φ₂+φ₃, φ₁} in semicircle but φ₂ outside. The gap structure (φ₂ missing between φ₃ and φ₂+φ₃) is incompatible with semicircle separation.

### Proof Completeness

The 8 angles form C(8,2)=28 coincidence hyperplanes on the 3-torus [0,N)³, creating O(10⁴) chambers. Each chamber has a fixed circular order. Grid spacing 1 with N=360 guarantees every chamber contains ~10⁴ grid points. The search is provably exhaustive.

### Significance

This is the culmination of Demos 55-59. The progression:
- Demo 55: Re>0 + mult = 11/13 at ℓ=4 (first observation)
- Demo 57: Additive encoding makes it WORSE (convexity principle)
- Demo 58: Wall holds at ℓ=5 (algebra-independent)
- Demo 59: Wall holds at ℓ=6, ALL orientations (universal)
- **Demo 61: PROVEN for ALL angles AND all orientations (theorem)**

The wall is not an empirical observation — it is a mathematical fact about the geometry of multiplicative encodings and half-plane activations.

---

## Demo 62: Analytical Proof of the 11/13 Wall

**Date:** 2026-02-20
**File:** `demo_62_analytical_proof/main.c`
**Status:** COMPLETE (7/7 pass)

### Overview

Demo 61 proved the 11/13 wall computationally (exhaustive search, pure integer arithmetic). Demo 62 proves it ANALYTICALLY — two deductive proofs explaining WHY 0x06 and 0x1B are the unreachable classes.

### Proof A: 0x1B (CROSS) — Algebraic Contradiction

Three-step squeeze proof:
1. Fix θ=0 (WLOG since 0 ∈ TRUE). φ₂ ∈ [π,2π) → write φ₂=π+x. φ₃=y ∈ [0,π).
2. φ₂+φ₃ ∈ S requires wrapping: x+y ≥ π. Then φ₁+φ₂+φ₃ ∈ S̄ gives φ₁ ≥ 2π-x-y (lower bound). φ₁+φ₂ ∈ S̄ gives φ₁ < π-x (upper bound).
3. Lower < upper requires 2π-x-y < π-x → π < y. But y < π. **Contradiction.**

All 8 constraints are used. The (1,0,1) constraint is redundant in the proof chain (weaker than the (1,1,1) bound) but essential for system infeasibility — consistent with minimal obstruction size = 8.

### Proof B: 0x06 (XOR-AND) — Four-Semicircle Parallelogram

1. 0 ∉ S forces θ > 0. Let α=φ₂, β=φ₃ (WLOG α ≤ β), both in [θ, θ+π).
2. φ₂+φ₃ ∉ S forces α+β ≥ θ+π > π. **(KEY inequality)**
3. The 4 φ₁-dependent FALSE constraints restrict φ₁ to the intersection of 4 semicircles with midpoints at M, M-α, M-β, M-α-β — a **parallelogram** on the circle.
4. Gap analysis: the 4 sorted gaps are α, β-α, α, 2π-α-β. Since α+β > π, all 4 gaps < π. The intersection of semicircles is empty iff no gap ≥ π. **No valid φ₁ exists.**

### Minimal Obstruction Analysis (from Demo 61 Phase 2)

For both 0x06 and 0x1B, the minimal obstruction size is **8** — every proper subset of 7 or fewer constraints is satisfiable. The obstruction is purely global: no small sub-pattern explains the blockage. This is consistent with both proofs, which inherently use all 8 constraints.

### Near-Miss Analysis

- 0x1B nearest achievable: 0x9B (NPN class 0x19 = 3-SELECT), Hamming distance 1
- 0x06 nearest achievable: 0x07 (OR-NAND), Hamming distance 1
- Gap analysis for near-misses shows the parallelogram gap crossing π — confirming the proof predicts exactly where feasibility breaks

### NPN Orbit Coverage

- 0x06 orbit: 24 truth tables (via input perm + input neg + output neg)
- 0x1B orbit: 24 truth tables
- Total unreachable: 48 of 256 (208 reachable, 11 non-trivial NPN classes + FALSE)
- Symmetry argument: input perm = angle relabeling, input neg = angle reflection, output neg = semicircle swap

### Arc Summary

The angular proof arc is complete:
- **Demo 61:** Computational proof (exhaustive N=360 search, pure integer arithmetic, zero floating point)
- **Demo 62:** Analytical proof (two deductive arguments explaining the geometric obstruction)
- The wall is **geometric**, not algebraic: the additive structure of multiplicative encodings creates a parallelogram constraint on the circle incompatible with the alternating parity structure of 0x06 and 0x1B.

## CSS Feasibility Probe (Negative Result)

**Date:** 2026-02-19/20

**Hypothesis:** The TL radical (null space of the Gram/trace form) might define a CSS quantum error-correcting code whose transversal gates predict DKC-computable Boolean functions.

**Data extracted:**
| Setting | dim | Gram rank | radical dim |
|---------|-----|-----------|-------------|
| TL_4, δ=0 | 14 | 5 | 9 |
| TL_4, δ=√2 | 14 | 9 | 5 |
| TL_6, δ=0 | 132 | 42 | 90 |
| TL_6, δ=√2 | 132 | 58 | 74 |

**Approaches tested:**
1. **Support mapping** (nonzero positions → binary vector): Radical basis at δ=0 is too sparse (most weight 1-2). No interesting code structure.
2. **Sign mapping** (coefficient signs → binary): Only works at δ=0 (integer coefficients). Doesn't produce self-orthogonal codes.
3. **Mod-2 reduction** of Z-radical vectors: Weight distribution wrong for self-orthogonality.
4. **GF(2)-null-space approach** (the strongest test): Reduced G mod 2, computed null(G mod 2) over GF(2).
   - G mod 2 has rank 4 (lower than Z-rank 5, because 14 ≡ 0 mod 2).
   - Null space dim = 10 (larger than Z-radical dim 9).
   - **Critical failure:** null(G mod 2) is NOT self-orthogonal under the standard inner product. Contains weight-1 vectors (self-inner-product = 1 over GF(2)), and even the even-weight subspace has non-orthogonal pairs.

**Root cause:** The radical is self-orthogonal under the **Gram form** (this is tautological — that's what "radical" means). But CSS codes require self-orthogonality under the **standard inner product**. These are fundamentally different bilinear forms, and there's no reason they should coincide.

**What survives:** The radical constrains computation (non-semisimple structure is what creates the 11/13 wall), but it doesn't directly encode as a binary error-correcting code. The connection between TL algebra and Boolean computation goes through the bracket evaluation + activation pipeline, not through coding-theoretic constructions on the radical.

**Deferred:** Jones representation approach — working in (C²)^⊗4 (16-dimensional qubit space) instead of the abstract TL basis. This is a bigger project and would require computing the representation matrices of TL generators. Not obviously more promising given the conceptual gap identified above.

## Demo 60: Cubic Wall — ℓ=7 Number Field (2026-02-20)

### Motivation

All previous radical anatomy results (Demos 51-52, ℓ=2..6) used quadratic number fields (δ = 2cos(π/ℓ) satisfies a quadratic or simpler equation for ℓ ≤ 6). At ℓ=7, the minimal polynomial is **cubic**: x³ - x² - 2x + 1 = 0. This is the first non-quadratic case and tests whether all dimension formulas are truly universal or just artifacts of quadratic arithmetic.

### Implementation

C89 program with:
- Polynomial arithmetic over F_p mod f(x) = x³-x²-2x+1
- Cubic root finding via polynomial GCD + Cantor-Zassenhaus splitting
- TL algebra construction for n=2..8 (up to C_8 = 1430 basis elements)
- Gram matrix with δ^loops weighting, Gaussian elimination mod p
- Radical filtration (rad², rad³, nilpotency)
- Through-strand sector decomposition for Fibonacci rank test
- Cross-validation at two independent primes (p1=10^9+7, p2=10^9+9)

### Results

| n | dim | rank | radical | rad² | nilpotency |
|---|-----|------|---------|------|------------|
| 2 | 2 | 2 | 0 | — | — |
| 3 | 5 | 5 | 0 | — | — |
| 4 | 14 | 14 | 0 | — | — |
| 5 | 42 | 42 | 0 | — | — |
| 6 | 132 | 132 | 0 | — | — |
| 7 | 429 | 418 | **11** | 1 | 3 |
| 8 | 1430 | 1391 | **39** | 1 | 3 |

### Formula Verification (6th data point for each)

| Formula | Predicted | Actual | Status |
|---------|-----------|--------|--------|
| Semisimple for n < ℓ | rank = dim for n=2..6 | ✓ | CONFIRMED |
| rad(TL_ℓ) = 2ℓ-3 | 2(7)-3 = 11 | 11 | CONFIRMED |
| rad(TL_{ℓ+1}) = ℓ²-ℓ-3 | 49-7-3 = 39 | 39 | CONFIRMED |
| rad² = 1 | 1 at n=7,8 | 1,1 | CONFIRMED |
| Nilpotency = 3 | 3 at n=7,8 | 3,3 | CONFIRMED |
| Fibonacci rank = F(ℓ-1) | F(6) = 8 | 8 | CONFIRMED |

**The Catalan difference formula (from the original future-demos spec) is WRONG.** It predicted rad(TL_7) = C_7-C_6 = 297 and rad(TL_8) = C_8-C_7 = 1001. The correct formulas are 2ℓ-3 and ℓ²-ℓ-3, discovered during Demo 52 analysis.

### Fibonacci Rank Sector Decomposition

| Sector (through-strands) | Elements | Matrix dim | Rank |
|---------------------------|----------|------------|------|
| 1 | 196 | 14×14 | 3 |
| 3 | 196 | 14×14 | 4 |
| 5 | 36 | 6×6 | 1 |
| 7 (identity) | 1 | 1×1 | 0 |
| **Total** | | | **8 = F(6)** |

Sector rank profile: **[3, 4, 1]** — not palindromic (ℓ=7 is odd, consistent with palindromic conjecture for even ℓ only).

Updated full Fibonacci rank table:

| ℓ | δ | Sectors | Rank profile | Total |
|---|---|---------|-------------|-------|
| 3 | 1 | {1} | [1] | 1 = F(2) |
| 4 | √2 | {0,2} | [1,1] | 2 = F(3) |
| 5 | φ | {1,3} | [2,1] | 3 = F(4) |
| 6 | √3 | {0,2,4} | [1,3,1] | 5 = F(5) |
| **7** | **cubic** | **{1,3,5}** | **[3,4,1]** | **8 = F(6)** |

**Odd ℓ sector rank profiles:** [1] → [2,1] → [3,4,1]. Non-palindromic, but extreme sector (highest through-strand count, excluding identity) always has rank 1.

### Significance

This is the **strongest universality test to date**. The jump from quadratic to cubic number fields is a qualitative change in algebraic complexity (Galois group goes from Z/2 to Z/3). All six formulas — semisimplicity threshold, radical dimensions (two formulas), rad²=1, nilpotency=3, and Fibonacci rank — survive unchanged. The radical anatomy of TL_n depends ONLY on ℓ, not on the number-theoretic properties of δ.

15/15 tests passed, 0 failed.

## Demo 38: Dimension Reduction at δ=0 (2026-02-20)

### Motivation

At δ=0, TL algebras become non-semisimple with a striking alternating pattern: odd n are semisimple, even n are not. Demo 51 revealed that the Gram rank at even n equals C_{n-1} (the previous Catalan number), not ~2^n as originally hypothesized. This demo constructs the explicit quotient TL_n/rad and proves it is algebraically isomorphic to TL_{n-1}.

### Implementation

C89 program with:
- TL algebra construction n=2..8 (planar matching basis, multiplication table)
- Gram matrix via fixpt formula: G[i][j] = fixpt[mt_result[i][j]] where fixpt counts fixed points of left multiplication at δ=0
- Full RREF (forward + backward Gaussian elimination) on augmented [G|I] matrix mod p
- Reduced generator construction: project TL generators onto C_{n-1}-dimensional quotient via RREF
- TL relation verification on reduced generators (nilpotent, adjacent, far commutativity)
- Quotient isomorphism test: compare nonzero product counts between TL_n/rad and TL_{n-1}

### Results

**Part B — Gram Rank (n=2..8):**

| n | C_n  | Gram rank | rad dim | Reduction |
|---|------|-----------|---------|-----------|
| 2 |    2 |       1   |     1   | 2.00x     |
| 3 |    5 |       5   |     0   | 1.00x     |
| 4 |   14 |       5   |     9   | 2.80x     |
| 5 |   42 |      42   |     0   | 1.00x     |
| 6 |  132 |      42   |    90   | 3.14x     |
| 7 |  429 |     429   |     0   | 1.00x     |
| 8 | 1430 |     429   |  1001   | 3.33x     |

Alternating semisimplicity confirmed:
- **Odd n**: Always semisimple (rank = C_n)
- **Even n**: Gram rank = C_{n-1} (previous Catalan number)
- Radical dimension = C_n - C_{n-1} for even n

**Part C — Reduced Generators (even n = 2, 4, 6):**

| n | Quotient dim | Generators | Nonzero per gen | TL relations? |
|---|-------------|------------|-----------------|---------------|
| 2 | 1 | e_0 → 0 | 0 | YES |
| 4 | 5 | e_0, e_1, e_2 | 3 each | YES |
| 6 | 42 | e_0..e_3: 28 each, **e_4: 57** | asymmetric | YES |

Notable: At n=6, the last generator e_4 has 57 nonzero entries vs 28 for e_0..e_3 — boundary asymmetry in the reduced representation.

**Part F — Quotient Isomorphism (THEOREM):**

| TL_n/rad | nonzero products | TL_{n-1} | nonzero products | Match? |
|----------|-----------------|-----------|-----------------|--------|
| TL_2/rad | 1               | TL_1      | 1               | YES    |
| TL_4/rad | 17              | TL_3      | 17              | YES    |
| TL_6/rad | 955             | TL_5      | 955             | YES    |

**TL_{2k}(0) / rad ≅ TL_{2k-1}(0) as algebras.** Not just dimension matching — the full multiplication tables agree.

### Key Observations

1. **Generator sparsity = radical dimension exactly.** Each generator has C_n - C_{n-1} nonzero entries out of C_n total, for every even n tested.

2. **Parts D-E (bracket verification, performance) are theoretically redundant.** The quotient isomorphism implies bracket correctness since the bracket is a trace form that factors through the quotient.

3. **The reduction ratio C_n/C_{n-1} approaches 4** (2.00, 2.80, 3.14, 3.33...), converging to 4 since C_n ~ 4^n/(n^{3/2}√π).

4. **Boundary asymmetry in reduced generators.** At n=6, the last generator has roughly double the nonzero count of the others. This may reflect the boundary between the "quotient-inherited" generators (e_0..e_{n-3}) and the "new" generator (e_{n-2}).

### Significance

This establishes a clean structural theorem: **at δ=0, even-dimensional TL algebras mod their radical collapse to the previous odd-dimensional TL algebra.** Combined with odd n always being semisimple at δ=0, this gives a complete picture of the δ=0 specialization. The radical is exactly the kernel of "forgetting the last strand pair."

13/13 tests passed, 0 failed.

## Demo 39: Symmetry Decomposition of TL_n (2026-02-20)

### Motivation

Demo 38 showed that even n at δ=0 has radical dimension C_n - C_{n-1}. But what is the *internal structure* of this radical? Through-strand count (the number of strands connecting top to bottom in a planar matching) provides a natural decomposition of TL_n into sectors. This demo verifies the cellular algebra structure, maps per-sector Gram ranks, quantifies cross-sector coupling, and compares sector liveness across multiple δ values.

### Implementation

C89 program, 6 parts (18 tests):
- **Part A**: Sector sizes = d(n,j)² where d(n,j) = C(n,(n-j)/2) - C(n,(n-j)/2-1) (cell module dimension)
- **Part B**: Per-sector Gram rank at δ=0
- **Part C**: Cross-sector coupling (sum of sector ranks vs full Gram rank)
- **Part D**: Cell filtration verification: ts(a·b) ≤ min(ts(a), ts(b))
- **Part E**: Multi-δ sector liveness at δ = 0, 1, √2, φ
- **Part F**: Generator block structure in sector-ordered basis

### Results

**Part A — Sector Sizes (n=2..8):**

All sector sizes confirmed as perfect squares d(n,j)². Notable: at n=8, sector j=2 has 784 = 28² elements.

**Part B — Per-sector Gram Rank at δ=0:**

| n | j=0 | j=1 | j=2 | j=3 | j=4 | j=5 | j=6 | j=7 | j=8 |
|---|-----|-----|-----|-----|-----|-----|-----|-----|-----|
| 2 | DEAD | | LIVE | | | | | | |
| 3 | | LIVE | | LIVE | | | | | |
| 4 | DEAD | | 4/9 | | LIVE | | | | |
| 5 | | LIVE | | LIVE | | LIVE | | | |
| 6 | DEAD | | 25/81 | | 20/25 | | LIVE | | |
| 7 | | LIVE | | LIVE | | LIVE | | LIVE | |
| 8 | DEAD | | 196/784 | | 294/400 | | 42/49 | | LIVE |

Pattern: odd n → all sectors fully LIVE (semisimple). Even n → j=0 completely DEAD, j=n LIVE, intermediate sectors PARTIAL.

**Part C — Cross-sector Coupling:**

| n | Sum sector ranks | Full rank | Coupling |
|---|-----------------|-----------|----------|
| 4 | 5 | 5 | 0 |
| 6 | 46 | 42 | 4 |
| 8 | 533 | 429 | 104 |

Sectors are truly independent at n=4, but coupled at n≥6.

**Part E — Multi-δ Comparison (n=4):**

| δ | j=0 | j=2 | j=4 | full rank |
|---|-----|-----|-----|-----------|
| 0 | DEAD | 4/9 | LIVE | 5 |
| 1 | 1/4 | LIVE | LIVE | 11 |
| √2 | LIVE | 7/9 | LIVE | 9 |
| φ | LIVE | LIVE | LIVE | 14 |

Each δ has a unique damage pattern. Only δ=φ (golden ratio, ℓ=5) gives full semisimplicity at n=4.

**Part F — Generator Block Structure:**

Generators reordered by sector (decreasing j). Results:

| n | Generator | Structure | Off-diagonal entries |
|---|-----------|-----------|---------------------|
| 3 | e_1, e_2 | block-lower-tri | 1 each |
| 4 | e_1, e_3 | block-lower-tri | 4 each |
| 4 | e_2 | block-lower-tri | 1 |
| 5 | e_1, e_4 | block-lower-tri | 9 each |
| 5 | e_2, e_3 | block-lower-tri | 5 each |

**Key finding: even semisimple algebras (n=3, n=5) are NOT block-diagonal in the standard sector basis.** The cell filtration guarantees block-lower-triangular structure, but achieving block-diagonal requires a genuine Wedderburn change of basis (computing cell module irreps). The off-diagonal entries represent the "filtration gap" — multiplication mapping higher-j elements into lower-j sectors.

Symmetric generators (e_1, e_{n-1}) consistently have more off-diagonal entries than central generators.

### Key Findings

1. **Cellular structure confirmed computationally:** sector sizes = d(n,j)², validated for all n=2..8.

2. **j=0 complete death at δ=0 is UNIQUE.** No other tested δ value kills an entire sector completely. δ=1 weakens j=0 to 1/4 but doesn't kill it. δ=√2 leaves j=0 fully LIVE.

3. **Different δ values have different damage patterns.** δ=0 kills j=0 and weakens j=2. δ=√2 weakens j=2 instead. δ=1 weakens j=0. Each δ value targets different sectors.

4. **Cross-sector coupling onset at n=6.** At n=4 sectors are independent (coupling=0), but at n≥6 they interact. The coupling grows rapidly (0→4→104).

5. **Standard basis ≠ Wedderburn basis.** Sector ordering gives block-lower-triangular (filtration), not block-diagonal (decomposition). True block-diagonalization requires computing cell module bases.

### Connection to DKC

The sector structure determines the **ceiling** of what's computationally possible with a given δ. At δ=0, the complete death of j=0 (zero-through-strand diagrams) removes algebraic information that no activation function can recover. At δ=√2 where j=0 is alive, parity becomes achievable. The activation function determines how much of the ceiling you actually reach — but the sector liveness pattern sets the upper bound.

18/18 tests passed, 0 failed.

---

## Demo 39 Part G: Three Gram Forms at n=ℓ — MILESTONE

**Date:** 2026-02-20

### The Problem

At the "critical boundary" n=ℓ where δ=2cos(π/ℓ), the Graham-Lehrer criterion says TL_n is non-semisimple. We need to detect this computationally and measure the radical.

### Three Bilinear Forms

We implemented three distinct Gram forms:
1. **Fixpt (regular trace):** B(a,b) = Σ_{k:ab(k)=k} δ^loops — the form used in all previous demos
2. **Markov trace:** B_M(a,b) = δ^{inner_loops + closure_loops}
3. **Cell module Gram:** d(n,j) × d(n,j) matrix — the Graham-Lehrer bilinear form on cell modules V_j

### Critical Bug & Fix

Initially ℓ=5 showed full rank (0 radical). Root cause: **p=10^9+7 has 5 as a quadratic non-residue.** `mod_sqrt(5)` was returning √(-5), making the golden ratio φ garbage. Changed to p=1000002361 (≡1 mod 840), which guarantees all roots of unity for ℓ=2..7 exist in GF(p). Also added QR check to `mod_sqrt`.

### Results: 23/23 tests pass

**Cell module Gram data at n=ℓ:**

| ℓ | δ | Degen module | dim | cell_rk | corank | rad = k(2d-k) |
|---|---|-------------|-----|---------|--------|---------------|
| 2 | 0 | V₀ | 1 | 0 | 1 | 1 |
| 3 | 1 | V₁ | 2 | 1 | 1 | 3 |
| 4 | √2 | V₂ | 3 | 2 | 1 | 5 |
| 5 | φ | V₃ | 4 | 3 | 1 | 7 |
| 6 | √3 | V₄ | 5 | 4 | 1 | 9 |
| 7 | 2cos(π/7) | V₅ | 6 | 5 | 1 | 11 |

**Pattern: V_{ℓ-2} is the UNIQUE degenerate cell module, always with corank 1. rad(TL_ℓ) = 2ℓ-3.**

**Full Gram ranks:**

| ℓ | dim | fixpt_rk | Markov_rk | fixpt_rad | Markov_rad |
|---|-----|----------|-----------|-----------|------------|
| 2 | 2 | 1 | 0 | 1 | 2 |
| 3 | 5 | 2 | 1 | 3 | 4 |
| 4 | 14 | 9 | 8 | 5 | 6 |
| 5 | 42 | 35 | 34 | 7 | 8 |
| 6 | 132 | 123 | 122 | 9 | 10 |
| 7 | 429 | 418 | 417 | 11 | 12 |

**Patterns:**
- fixpt_rad = 2ℓ-3 (matches cell module prediction exactly)
- Markov_rad = 2ℓ-2 (always 1 more than fixpt — Markov detects an extra dimension)

### THEOREM: rad(TL_ℓ) = 2ℓ-3

**Full proof: see `proofs/radical-dimension-formula.md`**

Summary: V_{ℓ-2} is the unique degenerate cell module with corank 1. Tridiagonal Gram (forced by planarity) has Chebyshev determinant U_{ℓ-1}(δ/2) = 0. rad = 1·(2(ℓ-1)-1) = 2ℓ-3.

**Corollary:** rad(TL_{ℓ+1}) = ℓ²-ℓ-3 (V_{ℓ-3} is the unique degenerate module at next level).

### THEOREM: Reshetikhin-Turaev Truncation of Markov Trace

**The Markov trace at q = e^{iπ/ℓ} kills exactly the simple modules L_j with j ≥ ℓ-1, and preserves those with j ≤ ℓ-2.**

This is the standard RT TQFT truncation: the "physical" representations are 0 ≤ j ≤ ℓ-2. Everything above is negligible.

**Consequence:**
- Markov excess = Σ_{j ≥ ℓ-1, j ≡ n mod 2} (dim L_j)²
- Markov rank = Σ_{j ≤ ℓ-2, j ≡ n mod 2} (dim L_j)²
- dim L_j = cell_rk(V_j^{(n)}) = d(n,j) - corank(V_j)

**Verified computationally: 31/31 tests pass across all levels n=ℓ through n=ℓ+3.**

| Level | Killed blocks | Excess | Special cases |
|-------|--------------|--------|---------------|
| n=ℓ | {ℓ} | 1² = 1 | Always 1 killed block |
| n=ℓ+1 | {ℓ-1, ℓ+1} | ℓ²+1 | dim L_{ℓ-1}=ℓ (non-degen) |
| n=ℓ+2 | {ℓ, ℓ+2} | (ℓ+1)²+1 | dim L_ℓ=ℓ+1 (non-degen) |
| n=ℓ+3 | {ℓ-1, ℓ+1, ℓ+3} | Σ 3 blocks | ℓ=3: V₄ degenerates via [2ℓ]=0 |

**n=ℓ+3 data (3 killed blocks each):**

| ℓ | n | killed | dim L values | excess |
|---|---|--------|-------------|--------|
| 3 | 6 | {2,4,6} | 9, 4, 1 | 81+16+1 = 98 |
| 4 | 7 | {3,5,7} | 14, 6, 1 | 196+36+1 = 233 |
| 5 | 8 | {4,6,8} | 20, 7, 1 | 400+49+1 = 450 |

**Key insight from ℓ=3/n=6:** V₄ has corank 1 (degenerates via [2ℓ]=0, not [ℓ]=0), so dim L₄ = 4 not 5. For ℓ ≥ 4 at n=ℓ+3, all killed modules are non-degenerate.

**Why the (ℓ+m-1)²+1 pattern appeared to hold for m=1,2:** At small m, only 2 killed blocks exist. The identity block always contributes 1. The other block has dimension d(n,n-2) = n-1 (at m=1) or d(n,n-2) = n-1 (at m=2), giving (n-1)²+1. At m=3, THREE blocks appear and the formula breaks.

Note: ℓ=3 (δ=1) is extremely degenerate — Markov rank is 1 at both n=5 and n=6 (only L₀ or L₁ survives, both with dim L = 1 after corank subtraction).

### Cell Module Corank Patterns

**First degeneracy:** At the first level n where V_j degenerates, corank is always 1. This is universal — proved via the Graham-Lehrer linking theorem (linked module is V_n^{(n)}, dim 1).

**Second degeneracy of V_{ℓ-2} at n=ℓ+2:** corank = ℓ+1 (verified ℓ=3..6).

| ℓ | Module | dim d | First degen corank | Second degen corank |
|---|--------|-------|-------------------|---------------------|
| 3 | V₁ | 5 (at n=5) | 1 (at n=3) | 4 = ℓ+1 |
| 4 | V₂ | 9 (at n=6) | 1 (at n=4) | 5 = ℓ+1 |
| 5 | V₃ | 14 (at n=7) | 1 (at n=5) | 6 = ℓ+1 |
| 6 | V₄ | 20 (at n=8) | 1 (at n=6) | 7 = ℓ+1 |

### Cross-Sector Markov Kernel Vector (ℓ=4)

The extra Markov kernel vector at ℓ=4 (n=4, δ=√2) is:

**v = -δ·e₂ + e₃ + e₇ - δ·e₉ + e₁₃**

Diagram identities:
- e₂ (j=2): left arc (01), right arc (01) — T0-T1 | T2=B2 T3=B3 | B0-B1
- e₃ (j=2): left arc (01), right arc (12) — T0-T1 | T2=B0 T3=B3 | B1-B2
- e₇ (j=2): left arc (12), right arc (01) — T1-T2 | T0=B2 T3=B3 | B0-B1
- e₉ (j=2): left arc (12), right arc (12) — T1-T2 | T0=B0 T3=B3 | B1-B2
- e₁₃ (j=4): identity — T0=B0 T1=B1 T2=B2 T3=B3

The j=2 component forms a 2×2 grid: coefficient matrix [-δ, 1; 1, -δ] = J - δI (exchange minus diagonal). This has full rank 2 — the kernel vector is NOT a product state in the left⊗right decomposition. Uses first two Chebyshev basis vectors h₁=(01), h₂=(12) but NOT h₃=(23).

G_F·v = (0,...,0,4) — nonzero only at identity position. The fixpt trace sees v but the Markov trace kills it.

### Open Questions (Updated)

1. **Markov excess growth:** Why does excess jump from quadratic (m=1,2) to high-degree (m=3)? The m=1 and m=2 patterns (ℓ²+1 and (ℓ+1)²+1) suggest (n-1)²+1, but this fails at m=3. Possibly related to proliferation of degenerate cell modules.
2. **Second-degeneracy corank = ℓ+1:** Can this be proved from linking theory? The linked module at second degeneracy should have dim > 1.
3. **ℓ=3 Markov rank = 1:** At δ=1, Markov rank stays 1 for n=5,6. Does it stay 1 for all n > ℓ? This connects to the Jones polynomial at q=1.
4. **Block projection:** What is the image of the extra kernel vector in A/J(A) ≅ ⊕ M_{d_j}? Does it live in a single simple block or span multiple?

### Literature (from research agent)

Key reference: Ridout & Saint-Aubin (2014), arXiv:1204.4505 — radical of each cell module is irreducible or zero. Gram determinant formula:

det(G_{n,p}) = ∏_{k=1}^{p} ([n-2p+2k]/[2k])^{d_{n-2k,p-k}}

Graham-Lehrer semisimplicity: TL_n(2cos(π/ℓ)) semisimple iff n < ℓ.

---

## Demo 63: Angular Anatomy of DKC (2026-02-20)

### Summary
Demo 63 explains the four-tier NPN hierarchy from Demo 50 through pure angular/octant geometry of the Z[zeta_8] lattice, then extends to 4-input and 5-input parity. **24/24 tests pass.**

### Key Findings
- **Axiality confirmed**: All 100 catalog values at δ=0 are axial (single nonzero Z[zeta_8] component). Ray distribution: a=28, b=19, c=34, d=19. Values live on exactly 8 discrete rays (octants 0-7, spaced 45°).
- **Sector-octant mapping table**: k-sector boundaries partition the 8 lattice octants into class-0 and class-1 groups. k=6 uniquely produces class-1 = {2,4,5,7} = the parity octants.
- **Four-tier structure reproduced from pure geometry**: Tier 1 (k=2, 5 classes) = linearly separable by hemisphere. Tier 2 (k=3, +1 = MAJ') = first nonlinear, needs 120° boundaries. Tier 3 (k=4, +6 classes) = everything except parity. Tier 4 (k=6, +1 = XNOR3) = parity requires 60° boundaries cutting the 45° lattice at the exact incommensurate angle needed.
- **Part C representative examples**: For each tier, one weight triple displayed with full anatomy — all 8 sums, their Z[zeta_8] coordinates, octant, sector, and class assignments. The parity example shows triskelion structure: weights from octants {5,2,7} landing in odd sectors {s3,s1,s5}, producing the alternating class pattern that IS parity.
- **906 parity triskelions verified**: All parity solutions at k=6 place one weight per odd sector. Only two octant sets: {2,5,7}=636 and {2,4,7}=270.
- **Commensurability**: gcd(k,8) controls structure. k=6 (gcd=2) gives maximum parity; k=8 (gcd=8) gives minimum (96). Non-monotonic because incommensurate angles create richer sector-octant mixing.

### Connection to RT Truncation (Demo 39)
At δ=0 (ℓ=2), the Markov trace kills all j ≥ 1 — topology is almost trivial. But this SAME degeneracy forces bracket values onto single rays, giving DKC its maximally structured angular alphabet. DKC works BECAUSE it reads pre-closure (fixpt) information that RT truncation would discard. The wall between 12/13 and 13/13 NPN classes is an angular geometry fact about the Z[zeta_8] lattice and half-plane activation, not about the algebra's root-of-unity parameter.

### Oriented Matroid Connection
The sector-octant table is oriented matroid data — k-sector boundaries define hyperplane arrangements classifying the 8 lattice rays. The tiers correspond to covector realizability thresholds over the Z[zeta_8] lattice.

### Parts
- Part A: Octant enumeration, axiality verification, ray/octant distribution
- Part B: Sector-octant mapping table for k=2..8, class-1 octant sets
- Part C: Representative weight triple examples from each tier (mechanism visible)
- Part D: Minimum-k per NPN class, four-tier structure, reachability heatmap
- Part E: Parity triskelion anatomy, octant set distribution
- Part F: Commensurability analysis (gcd(k,8) vs parity)
- Part G: Oriented matroid classification (see below)
- Part H: 4-input XOR extension (see below)
- Part I: 5-input XOR extension (see below)

### Part G: Oriented Matroid Classification (added 2026-02-20)

Each weight triple defines an oriented matroid via the sign pattern of 3 pairwise cross products. Since all catalog values are axial, the cross product sign depends only on the octant difference (Δoct mod 8: {1,2,3}→+, {5,6,7}→-, {0,4}→0).

**G1: 21 distinct OM types** from 512 octant triples. Predicted and confirmed: 27 - 6 (two-zero patterns unrealizable in R²) = 21. Geometric constraint: if any two cross products are 0 (parallel pairs), the third must also be 0.

**G2: Parity requires ALTERNATING OM types — prediction falsified**
- Explorer predicted: parity needs (+,+,+) or (-,-,-) (uniform cycling)
- Reality: (-,+,-) = 453 solutions, (+,-,+) = 453 solutions, ALL others = 0
- The alternating type has s12 = s23 = -s13: an INTERLEAVING arrangement, not uniform cycling
- Geometrically: weights span three different 120°-separated odd sectors because they interleave around the circle, not cluster in one arc

**Other findings from the OM×k reachability table:**
1. At k=2: ALL 21 OM types achieve exactly 5/13 — OM type irrelevant for linear separability
2. At k=6: ONLY alternating types reach 13/13; everything else ≤ 12
3. Uniform types (+,+,+) etc get 12/13 at k=6 — everything except parity
4. Degenerate (0,0,0) drops to 4/13 at k=4 and k=8 (perfect alignment kills expressivity)
5. At k=7: six one-degenerate types reach 13/13 (k=7 is less selective than k=6)

### Literature (from research agent, 2026-02-20)

- **Cover's theorem (1965)**: 104 linearly separable functions of 3 variables (OEIS A000609). This is our k=2 case (5 NPN classes = 104 functions under permutation/negation). Our k-sector mechanism generalizes threshold functions.
- **Oriented matroids**: Bjorner et al (Cambridge). Framework NAMES our setup (tope set of cyclotomic arrangement on cube vertices) but doesn't COMPUTE it.
- **Cyclotomic matroids**: Reiner (2004, arXiv:math/0402206). mu_n = direct sum of simplicial matroids. Doesn't connect to Boolean function classification.
- **Gap confirmed**: nobody studies zonotope dual to Z[zeta_n] sector-boundary arrangement evaluated at Boolean cube vertices. Our setup is novel.
- Key authors: Cover, Muroga, Winder (threshold); Reiner (cyclotomic); Bjorner, Las Vergnas, Sturmfels, White, Ziegler (OM).

### Part H: 4-Input XOR Extension (added 2026-02-20)

**XOR4 (0x6996) is reachable — ONLY at k=8, with exactly 96 solutions.**

- Searched k=2..8 with 4-level pruned search over class-1 catalog values
- k=2 through k=7: zero XOR4 solutions
- k=8: 96 solutions — the ONLY k that works

**Tetraskelion structure**: The first example places 4 weights at the 4 odd octants {3,1,5,7}, each in its own odd sector (s3,s1,s5,s7) at k=8. All have coefficient magnitude 4. The 6-sign OM type is (-,+,0,0,-,+) — contains zeros from antipodal pairs (oct 1 + oct 5, oct 3 + oct 7 cancel to zero).

**Key insight**: At k=8, sectors = octants perfectly (gcd(8,8)=8). This forces complete alignment: each sector holds exactly one octant. XOR4 needs all 4 single weights in class-1 sectors, so all 4 must come from the 4 odd octants {1,3,5,7}. Pair sums between different odd octants land in even sectors (class-0) as required.

### Part I: 5-Input XOR Extension — MAJOR FINDING (added 2026-02-20)

**XOR5 IS reachable — at k=15 only, with 3020 solutions.**

- Searched k=2..16 with 5-level pruned search (31 subset constraints per candidate)
- k=2 through k=14: zero XOR5 solutions
- k=15: 3020 solutions
- k=16: zero class-1 values (dead zone)

**Class-1 octant count is the governing principle.** XOR_n needs at least n distinct class-1 octants (since same-octant weights sum to the same ray, violating pair class-0 constraint). Survey:
- k=2..14: at most 4 class-1 octants (NOT enough for n=5)
- k=15: {1,2,3,4,5,6,7} = 7 class-1 octants (360/15 = 24°, every 45° ray lands in an odd sector)
- k=16: 0 class-1 octants

**Pentaskelion structure**: First example uses weights from 5 different octants {6,2,1,7,4} in sectors {s11,s3,s1,s13,s7} at k=15.

**The k=2n conjecture is FALSIFIED.** The scaling is:
- n=3 parity: k=6, 906 solutions (4 class-1 octants)
- n=4 parity: k=8, 96 solutions (4 class-1 octants)
- n=5 parity: k=15, 3020 solutions (7 class-1 octants)

Non-monotonic solution counts: 906 → 96 → 3020. Having 7 class-1 octants at k=15 provides much more combinatorial room than the tight 4-octant fits at k=6 and k=8.

### Part J: Complete Parity Scaling — CEILING THEOREM (added 2026-02-20)

Tested XOR_n for n=3..7 at k=15 using general recursive search with 2^n - 1 subset constraints per candidate. **29/29 tests pass.**

**The Z[zeta_8] parity ceiling is n=5.**

Complete parity scaling table:
```
  n    min_k    sol@k=15   mechanism
  3      6       23,004    triskelion
  4      8       16,108    tetraskelion
  5     15        3,020    pentaskelion
  6    N/A            0    IMPOSSIBLE (constraint wall)
  7    N/A            0    IMPOSSIBLE (constraint wall)
  8    N/A            0    IMPOSSIBLE (pigeonhole)
```

**Key findings:**

1. **k=15 is universal for n=3,4,5**: All achievable there with massive solution counts (23K, 16K, 3K) compared to minimum-k results (906, 96, 3020). k=15 is special because 15 = 2×8 - 1, making floor(15m/8) odd for all m=1..7.

2. **XOR6 is impossible despite 7 class-1 octants**: The wall is NOT pigeonhole (we have enough octants). It's a constraint-satisfaction wall: with 6 weights from 6 different rays, the C(6,2)=15 pair sums + C(6,3)=20 triple sums + ... = 63 total subset constraints cannot all be satisfied simultaneously.

3. **Monotonic decrease at k=15**: 23004 → 16108 → 3020 → 0. Each additional input roughly halves solutions until the wall.

4. **Two types of impossibility**: n=6,7 are impossible by constraint geometry (too many constraints). n=8 is impossible by pigeonhole (only 7 class-1 octants, same-octant pairs violate XOR).

5. **Connection to finer lattices**: The ceiling n=5 is specific to Z[zeta_8] (8 rays). Moving to Z[zeta_16] (16 rays) would provide more angular resolution and potentially raise the ceiling. The parity ceiling is a function of the cyclotomic order.

---

## Demo 64: Parity Matroid Recursion (22/22 pass)

**Date**: 2026-02-20

**Core question**: Is the DKC parity hierarchy (XOR2→XOR3→XOR4→XOR5) a single recursive matroid object?

**Answer**: YES. The hierarchy is fully deletion-contraction closed and forms a funnel-shaped matroid.

### Key Results

**Parts A+B — Matroid Closure (100%)**:
- Deletion: removing any weight from XOR_n always gives XOR_{n-1}. 100% at all levels.
- Contraction: fixing any input to 1 in XOR_n always gives XNOR_{n-1}. 100% at all levels.
- Implementation note: contraction requires exact Cyc8 arithmetic — floating-point produces 420 spurious failures.

**Parts C+D — The Funnel**:
- Coverage upward collapses: XOR3→XOR2 (99.3%) → XOR4→XOR3 (39.2%) → XOR5→XOR4 (19.2%) → 0%
- Distinct unordered weight sets: XOR5=27, XOR4=673, XOR3=3834, XOR2=1469
- Parent multiplicity thins near wall: mean 7.88 (XOR3→XOR2) → 1.79 (XOR4→XOR3) → 1.05 (XOR5→XOR4)

**Part E — Wall Microscope**:
- Minimum Hamming distance from XOR6: **4** (38 near-misses). No d=0,1,2,3.
- Errors at weight 0 and 1: **zero**. Wall manifests only at interaction level (weight 2+).
- 44/83 class-1 values used by XOR5. 31 "poison" values appear only in orphan XOR4 sets.
- Vocabulary perfectly nested: {XOR5 values} = {extensible XOR4 values} ⊂ {all XOR4 values}.

**Parts E4+E5 — Self-Doubling Theorem**:
- All 38 d=4 broken pairs involve the extension weight. 34/38 are self-extensions (6th = duplicate).
- **Theorem**: Self-extensions always fail because 2·w has the same angle as w (class 1), but pair sums need class 0.
- Representative case: exactly 1/15 pairwise sums wrong. The broken sum 2·(0,-12,0,0) = (0,-24,0,0) at 225°, sector 9 (odd).
- Higher-order wall: d=7-8 cases exist with 0 broken pairs — errors only at weight 3+.

### Quantum Connection (T-gate correspondence)
- ζ_8 = T-gate phase. Z[ζ_8] = T-gate algebraic ring.
- Parity ceiling = computational depth of one Clifford hierarchy level.
- Self-doubling: 2·(T-gate phase) = S-gate phase — descending the hierarchy loses power.
- Higher-order wall parallels GHZ entanglement (pairwise OK, three-body fails).

### Open Questions
1. Does XOR6 become achievable at Z[ζ_16] (next Clifford hierarchy level)?
2. Can the triple-interaction wall layer be characterized explicitly?
3. Funnel rate formula: 99.3% → 39.2% → 19.2% → 0% — is this predictable?
4. Are the 27 XOR5 sets built primarily from antipodal pairs?

---

## Demo 65: Clifford Hierarchy Staircase (33/33 pass)

**Date**: 2026-02-20

**Core question**: Does XOR6 become achievable at Z[zeta_16], the next Clifford hierarchy level?

**Answer**: NO. The parity ceiling n=5 is identical at Z[zeta_8] and Z[zeta_16].

### Z[zeta_16] Setup
- phi(16) = 8 components. Basis {1, zeta, ..., zeta^7}, reduction zeta^8 = -1
- A = -zeta_16, delta = -A^2 - A^{-2} = -sqrt(2) (NOT zero!)
- All braid states contribute (not just single-loop)
- Bracket = sum_s A^{a(s)-b(s)} * delta^{loops(s)-1}

### Catalog
- **36 values** from 2-3 strand braids (vs Z[zeta_8]'s 100!)
- 24/36 axial, 12 with 2 nonzero components
- 14/16 angular directions used (missing dirs 2 and 11)
- Only 8/100 Z[zeta_8] values found — catalogs mostly DISJOINT
- Extended to **56 values** with 4-strand braids (+20 new)

### NPN Tier Structure (IDENTICAL to Z[zeta_8])
| XOR_n | First k (Z[zeta_8]) | First k (Z[zeta_16]) |
|-------|---------------------|----------------------|
| XOR3  | k=6                 | k=6                  |
| XOR4  | k=8                 | k=8                  |
| XOR5  | k=14                | k=14                 |
| XOR6  | impossible          | impossible            |

### XOR5 (56-value catalog)
- k=14: 120, k=15: 1080, k=17: 240, k=21: 240, k=31: 960
- Note: k=31 was 0 with 36 values — catalog incompleteness artifact

### XOR6: Zero at ALL k=2..31 with both 36 and 56-value catalogs

### "Gap of 2" Hypothesis: REFUTED
| Ring | Pigeonhole | Ceiling | Gap |
|------|-----------|---------|-----|
| Z[zeta_8]  | 7  | 5 | 2 |
| Z[zeta_16] | 14 | 5 | 9 |

### Key Insight
The parity ceiling is NOT a function of Clifford hierarchy level. Moving from T-gate ring (3rd level) to 4th level provides zero benefit for parity. The ceiling appears to be a universal property of the Kauffman bracket's topological structure, independent of evaluation point.

### Part G: Generalized Activation Check — BREAKTHROUGH

**Question**: Can ANY binary sector labeling (not just odd/even) compute XOR6?

**Method**: 27 known XOR5 sets × 100 catalog extensions = 2700 checks. For each 6-tuple, check if a consistent binary labeling of k sectors exists.

**Results**:
- k=6,8,15,31: **0/2700** — absolute wall
- **k=24: 3/2700** — FIRST generalized XOR6!
- k=63: 20, k=127: 57, k=255: 109, k=500: 165

**Two-layer wall**:
- **Layer 1 (absolute, k≤23)**: No binary partition works. Geometric constraint.
- **Layer 2 (convention, k≥24)**: Standard odd/even fails, but custom labelings succeed.

**Implication**: The parity ceiling n=5 is true ONLY for standard sector activations. With k≥24 custom labelings, XOR6 IS achievable from bracket weights. The wall is in the activation convention, not the weights.

### Part H: Anatomy of 3 Passing Tuples at k=24

**Structure**: All 3 are **antipodal pairs** — the 6 weights form 3 pairs of (w, -w).
- 21/24 sectors used, 12 labeled class-1
- Labeling does NOT match standard odd/even
- Min margin: **0.000000 rad** (exactly on boundaries — concerning)

**Boundary Robustness** (critical):
- k=25: 0/2700 — odd k fails
- k=26: 4/2700 — even k has DIFFERENT passing tuples!
- k=27: 0/2700 — odd k fails
- k=28: 4/2700 — even k
- k=30: 10/2700 — even k, growing
- k=32: 4/2700 — even k

**Even/odd k pattern**: Generalized XOR6 exists at EVEN k (26,28,30,32) but NOT odd k (25,27,29,31) near the threshold. The k=24 zero-margin is a boundary effect, but the phenomenon is REAL — different tuples succeed at nearby even k values. Likely a lattice-sector resonance (45° lattice angles vs sector boundaries).

### Part I: Generalized XOR7

- k=24,48,63: 0/300
- **k=127: 1/300** — FIRST generalized XOR7
- k=255: 12/300, k=500: 24/300

**Minimum k scaling**:
| XOR_n | Min k (generalized) |
|-------|---------------------|
| XOR6  | 24                  |
| XOR7  | 127                 |

Ratio 127/24 ≈ 5.3x. The ceiling keeps climbing — no fixed upper bound with custom activations.

### Open Questions (Revised)
1. Even/odd k pattern — why does it occur? Lattice resonance?
2. Z[zeta_16] generalized check — does the absolute wall threshold change?
3. Scaling law: is min_k exponential in n? Need XOR8 data point.
4. Antipodal necessity: are all passing tuples antipodal pairs?
5. XOR7 anatomy at k=127 — what does the single passing tuple look like?

## Demo 66: Quaternionic DKC — First Contact (30/30 pass)

**Date**: 2026-02-20

**Core question**: What happens when we extract the full quaternion from an SU(2) braid representation instead of collapsing to a scalar bracket?

**Answer**: The **24-cell polytope** emerges as the natural computational geometry, and its Voronoi partition dramatically outperforms arbitrary grids for XOR6.

### SU(2) Braid Representation
- Generators: σ₁ → (1+i)/√2, σ₂ → (1−k)/√2 in unit quaternion space
- 87,890 enumerated braids produce exactly **24 distinct unit quaternions** (mod ±)
- These are the vertices of the **24-cell**, unique self-dual regular 4-polytope
- The 48 quaternions (with negatives) form the **binary octahedral group**
- Vertex types: 4 axis-aligned (±1,±i,±j,±k), 12 edge (two components at ±1/√2), 8 body (all four at ±½)

### Bracket ≠ Trace
- Standard prediction: bracket ∝ trace(ρ(b)) = 2·Re(q). Data says NO.
- Average Re(bracket)/trace ratio: −0.0799 (no clean constant)
- Same quaternion maps to different brackets in 8,424 of 124,750 pairs
- Bracket and quaternion are **complementary projections** — neither subsumes the other

### Antipodal-Only XOR6
- Full combinatorial search over C(24,6) = 475,020 six-tuples: **ZERO** solutions at any grid
- Restricting to **antipodal pairs** (3 pairs of (q,−q)): solutions emerge at 6×6 (3) and 8×8 (34)
- Antipodal constraint is **absolute** — reflects SU(2) → SO(3) double cover

### 24-Cell Voronoi Activation — HEADLINE RESULT
- Assign each subset sum to nearest 24-cell vertex (Voronoi cell on S³)
- 25 cells (24 Voronoi + 1 zero cell)
- **35 solutions at 25 cells** — beats 64-cell geographic grid (34 solutions)!
- The polytope IS the computation, not just the stage
- Connection to complex DKC: k=24 minimum on S¹ (Demo 65) is a projection of the 24-cell

### Universal 7/7/11 Structure
- Every one of the 35 Voronoi winners has EXACTLY: 7 cells labeled 0, 7 cells labeled 1, 11 cells unused
- Zero cell always labeled 0
- All 35 labelings are distinct
- Every cell used in at least one solution; only zero cell in all 35

### F4 Symmetry: 6 Orbits
- Full two-sided symmetry group q → g·q·h has **576 elements** (F4 Coxeter rotation subgroup)
- 35 solutions decompose into **6 orbits**: sizes 12, 7, 5, 5, 4, 2
- Two families:
  - **Edge-dominant** (orbits 1,2,5 — 22 solutions): edge-type cells → label 1
  - **Body-dominant** (orbits 3,4,6 — 13 solutions): body-type cells → label 1
- Complementary labelings of the same cell structure

### Complementary Hash Functions
- Quaternion resolves **450 bracket collisions** (same bracket, different quaternion)
- Bracket resolves **8,424 quaternion collisions** (same quaternion, different bracket)
- Neither strictly more informative — they capture different aspects of braid representation

### Winning Tuple Anatomy
- At 8×8: 15/34 mutually orthogonal (44%), 16/34 all-body type
- Body-type (±½,±½,±½,±½) vertices dominate the solution landscape

### Open Questions
1. Why exactly 6 orbits? 6 = n(n-1)/2 for n=4 pairs? Or partition of 6?
2. Edge/body complementary split — related to 24-cell self-duality?
3. XOR7 on 24-cell Voronoi: does 4D geometry accommodate it at 25 cells?
4. Higher quaternion groups: binary icosahedral (120-cell) at ℓ=5?
5. Zero-margin: do Voronoi solutions sit exactly on cell boundaries like S¹ solutions?

## Demo 67: Coordinate System Zoo — SU(2) Projections (18/18 pass)

**Date**: 2026-02-20

**Core question**: Which SU(2) coordinate system is optimal for DKC activation?

**Answer**: S² (rotation axis direction) is the computational sweet spot. A custom 13-direction Voronoi on S² achieves **36 solutions at 14 cells** — the most efficient activation found. The Hopf phase is completely inert.

### Eigenvector Extraction
- 24 quaternions → 4 eigenvalue angles (0°, 45°, 60°, 90°) + 13 eigenvector directions on S²
- 17 distinct (angle, direction) pairs — mild compression from 24 quaternions
- 3 axis-aligned directions shared across 2 angles; 10 directions unique to one angle
- Direction types: 3 axis-aligned, 6 edge-midpoints (±1/√2), 4 body-diagonals (±1/√3)

### Custom 13-Direction Voronoi — HEADLINE RESULT
- 13 actual eigenvector directions as Voronoi centers + 1 degenerate cell = **14 cells**
- **36 XOR6 solutions** — beats 24-cell Voronoi (35 at 25 cells) with fewer cells
- Solutions-per-cell: **2.57** — best of any activation

### Comparison Across Coordinate Systems (at ~16 cells)
| System | Cells | Solutions | Sol/Cell |
|--------|-------|-----------|----------|
| Custom 13-dir S² Voronoi | 14 | 36 | 2.57 |
| Hopf base 4×4 | 16 | 30 | 1.88 |
| 24-cell Voronoi (S³) | 25 | 35 | 1.40 |
| Eigenvector LatLon 4×4 | 16 | 2 | 0.13 |
| Cayley-Klein 4×4 | 16 | 0 | 0 |

### Hopf Phase = ZERO (Deepest Result)
- Hopf phase-only activation: **0 solutions at ALL k** (k=4 through k=32)
- Hopf base: 292 solutions at 8×8=64 cells (best 2D system at high resolution)
- The Hopf fibration S³→S² has a computationally dead fiber
- ALL DKC information lives in the S² base

### Eigenvalue-Only
- First solutions at k=8 (4 solutions). Peak: k=32 (80 solutions).
- k=24: 72 solutions — more than 24-cell Voronoi but 1D only
- Eigenvalue θ ≠ Hopf phase ξ₁ — this explains why θ works but phase doesn't

### Cayley-Klein (Worst)
- First solutions at 32 cells. 8×8: 106 solutions.
- The "native" SU(2) parameterization is worst for computation.

### Key Theoretical Insight
DKC computation is about **rotation axis direction** (S²), not rotation amount (θ) or phase (ξ₁). The natural computational space for braid data is S² = CP¹ = Bloch sphere.

### Cross-Demo Minimum Cells for XOR6
| Space | Activation | Min cells |
|-------|-----------|-----------|
| S¹ (trace) | k=24 sectors | 25 |
| S³ (quaternion) | 24-cell Voronoi | 25 |
| **S² (eigenvector)** | **13-dir Voronoi** | **14** |

## Demo 68: Stereographic DKC — Knot Computation in the Plane (9/9 pass)

**Date**: 2026-02-20

**Core question**: Can we project S² DKC onto the plane and visualize it?

**Answer**: Yes, but the computation is intrinsically curved. Stereographic projection gives beautiful ASCII art showing decision boundaries, but you MUST use S² metric for cell assignment — Euclidean gives 0 solutions.

### Projection Setup
- South pole (0,0,−1) is in the 13-direction catalog → switched to north pole projection
- Round-trip precision: ~1e-15 (machine epsilon)
- Area distortion ratio: 16.5× (conformal but not area-preserving)

### The Intrinsic Curvature Result
| Metric for cell assignment | Solutions |
|---------------------------|-----------|
| S² great-circle (dot product) | **36** |
| R² Euclidean | **0** |

The computation cannot be flattened. Voronoi cells drawn in R² with Euclidean distance are wrong — the area distortion reassigns points to incorrect cells. Only the S² metric preserves the structure.

### 36/36 Perfect Match
Projected Voronoi with S² metric: 36 solutions at 14 cells — identical to Demo 67's S² result. Difference = 0. All 36 winners show 7 zero / 7 one labeling.

### ASCII Visualization — DKC as a Picture
65×33 character canvas showing:
- 0/1 regions (XOR output), + boundaries, * weights, 0-9/A-C eigenvector centers
- The decision boundary between 0 and 1 is a visible curve in the plane
- Knot computation is literally a drawable picture

### Perceptron Formulation
Explicit 6×2 weight matrices: 6 bits → 2 neurons (x,y) → Voronoi cell → label. Weights from knot theory, not gradient descent.

### Planar Grid Comparison
| Grid | Cells | Solutions |
|------|-------|-----------|
| 4×4 | 16 | 0 |
| 8×8 | 64 | 33 |
| Voronoi (S² metric) | 14 | **36** |

### The Lesson
Visualize on the plane, compute on the sphere. S² is not just convenient — it's the intrinsic geometry.

### Cross-Demo Progression
S¹ (25 cells) → S³ (25 cells) → S² (14 cells) → R² (visualization only, not a further reduction)

## Demo 69: Clifford Staircase on S² — Multi-Root Eigenvector Comparison (9/9 pass)

**Date**: 2026-02-20

**Core question**: Is the 13-direction eigenvector structure universal across roots of unity, or specific to ζ_8?

**Answer**: The 13 directions are perfectly nested inside ζ_16 (RIGIDITY — zero angular drift), and algebraically guaranteed to nest at all power-of-2 roots. But finer roots trivialize XOR6 computation via pigeonhole, making ζ_8 the computational sweet spot.

### Catalog Explosion
| Root | θ | Quaternions | Directions |
|------|---|-------------|------------|
| ζ_8  | π/4  | 24      | 13         |
| ζ_16 | π/8  | 7,952   | 3,457      |
| ζ_32 | π/16 | 32,768* | 4,096*     |

*Still hitting enumeration caps. True ζ_32 group is larger.

### Direction Nesting
- **ζ_8 → ζ_16**: 13/13 match, 0.0000° drift — **RIGIDITY** (algebraically forced: ζ_8 = (ζ_16)²)
- **ζ_8 → ζ_32**: 8/13 match, 2.07° max drift — **DRIFT** (cap artifact; improved from 6/13 at smaller caps)
- Algebraic argument: ζ_8 = (ζ_32)⁴, squaring preserves rotation axis → all 13 must nest

### XOR6 Trivialization
| Root | Voronoi cells | XOR6 solutions | % of triples |
|------|--------------|----------------|--------------|
| ζ_8  | 14           | 36             | 0.007%       |
| ζ_16 | 3,458        | 499,367        | 90.6%        |
| ζ_32 | 4,097        | 493,654        | 89.6%        |

~90% of triples are solutions at 3,000+ cells — pigeonhole makes computation trivial.

### Fixed-Resolution Comparison
At lat/lon 4×4 (16 cells): ζ_16=450 > ζ_32=393. Finer roots NOT monotonically better at fixed resolution.

### Key Insight
ζ_8 is special: coarsest root where the 13-direction structure emerges. Structure is preserved at finer roots, but computational power at fixed resolution peaks at moderate refinement. The trivialization threshold lies between ~50 and ~500 cells.

## Demo 70: Musica Nodorum — Knots as Sound (8/8 pass)

**Date**: 2026-02-21

**Core question**: Can knot invariants be mapped to musical structures?

**Answer**: Yes, and the mapping reveals deep structure. The ζ_8 algebra's 4 eigenvalue angles map to C-Eb-E-F# (consonance to tritone). Yang-Baxter equivalent braids produce identical melodies. Eigenvalue (pitch) and eigenvector (position) are orthogonal dimensions.

### Four Knot Notes
| Angle | Note | Trace | Interval |
|-------|------|-------|----------|
| 0°    | C    | 2.000 | Unison   |
| 45°   | Eb   | √2    | m3       |
| 60°   | E    | 1.000 | M3       |
| 90°   | F#   | 0.000 | Tritone  |

### Yang-Baxter = Identical Melodies
σ₁σ₂σ₁ and σ₂σ₁σ₂ produce the same note at EVERY intermediate step (not just final). Trace is conjugation-invariant → eigenvalue angle sequence depends on product length, not specific generators.

### Music ⊥ Space
Correlation r = -0.0624 (essentially zero). 72 pairs share the same note but point in different directions. Confirms Hopf decomposition: eigenvalue and eigenvector are independent coordinates.

### Dodecahedral Connection — Platonic Failure
| Polyhedron | Eff cells | XOR6 |
|------------|-----------|------|
| Eigenvector | 13 | **36** |
| Icosahedron | 6 | **0** |
| Dodecahedron | 10 | **0** |

Only the knot algebra's own geometry computes XOR6. Dodecahedral symmetry (icosahedral group, order 120) is unrelated to binary octahedral group (order 48).

### Best Melodies
- Garside (s1 s2)³: Eb E F# E Eb C — complete ascending/descending arch
- (s1 s2⁻¹)⁵: Eb E₃ F# E₃ Eb C₃ Eb E₃ F# E₃ — octave-leaping oscillation

---

## Demo 71: Spectral DKC — Spherical Harmonic Decomposition (10/10)

### The 13=13 Theorem
Mode l has (2l+1) independent spherical harmonic components. The minimum bandwidth for XOR6 is l=6 because 2×6+1 = 13 = number of eigenvector directions. This is the lowest l where a single mode has enough DOF to independently address all 13 Voronoi regions.

### Sharp Phase Transition
0% recovery at l≤5, 100% at l=6. No partial recovery — the problem is all-or-nothing at the DOF boundary. l=5 has 11 components for 13 regions (underdetermined); l=6 has exactly 13 (determined).

### l=6 Dominates the Spectrum
~80.7% of XOR6 power (averaged across all 36 winners) sits in mode l=6. Only even modes have power (odd modes exactly zero from antipodal symmetry). Universal across all winners.

### Super-Nyquist Compression
Nyquist for l=6: (6+1)² = 49 cells needed. Actual: 14 cells. Ratio: 0.29 (3.5× compression). This is compressed sensing on S²: the XOR6 labeling is binary at 13 known positions → 13 bits of information, not 49 samples.

### Musical Connection — Inverted
The eigenvalue angles map to l=2 (F#, tritone), l=3 (E, major 3rd), l=4 (Eb, minor 3rd). But the dominant XOR6 mode is l=6 (D, whole tone = 200 cents). The computation requires HIGHER angular frequency than the eigenvalue structure. The notes are at one scale; the computation on those notes is at a finer scale.

### Trivialization Explained
For ζ_16 (3,457 directions): predicted bandwidth l ≥ 1728, Nyquist = 2,989,441 cells. At such bandwidth, ANY partition resolves XOR6 — explaining Demo 69's ~90% solution rate.

### Key Insight
The number 13 (eigenvector directions of the binary octahedral group on S²) determines EVERYTHING: the Voronoi cell count, the bandwidth of computation, and the super-Nyquist compression. It's not just how many directions there are — it's that 13 = 2×6+1 places the computation at exactly the mode that can address each direction independently.

---

## Demo 72: Spherical Design Test — Not Optimal, but Computational (6/6)

### t=0: Not a Spherical Design
The 13 eigenvector directions fail the t-design property at l=1 (centroid is off-center, |S_{1,1}|=2.43). Despite the Delsarte bound allowing t=7 for 13 points, the actual achievement is t=0. However, l=2 passes perfectly (5.11e-16).

### Even Mode Advantage
- t_even = 2 (passes l=2, fails l=4)
- t_odd = 0 (fails immediately)
- Undirected (26 pts): t=3 (odd modes auto-vanish)
- Directions are better at even modes, which is what DKC uses

### DKC Works Despite Design Failure
The gap between t=0 and bandwidth l=6 is huge. The computation succeeds entirely through:
1. DOF counting: 2l+1 = 13 components can label 13 regions
2. Compressed sensing: binary function at known positions = 13 bits, not 49 samples
3. The algebra selects directions for computation, not integration

### S³ Design
48-element binary octahedral group achieves t=1 on S³. Odd degrees exactly zero, even degrees fail at degree 2. The 24-cell (binary tetrahedral, a subgroup) is the known 5-design.

### Optimized Design Gets Fewer Solutions (Part G)
13 optimized points (design residual 0.93, 2.6x better than eigenvector's 2.43) get only **4 XOR6 solutions** vs eigenvector's **36**. Better geometry = fewer solutions. The algebra matters, not the geometry.

### Perturbation Sensitivity (Part H)
| ε | Mean XOR6 | Min | Max |
|---|-----------|-----|-----|
| 1° | 38.0 | 36 | 44 |
| 5° | 35.8 | 32 | 42 |
| 10° | 11.5 | 1 | 25 |
| 20° | 2.1 | 0 | 8 |

Robust to ~5°, collapses at 10-20°. Algebraic placement is sharp. At 1° mean slightly exceeds 36 (perturbation creates lucky near-misses).

### Key Insight
The 13 directions are computationally effective without being geometrically optimal. This separates two concepts that sampling theory usually conflates: having enough DOF to encode information (Demo 71) vs being an optimal quadrature rule (Demo 72). The algebra provides the former, not the latter. Moving directions to "better" positions destroys the computation.

---

## Demo 73: Eigenvector Automaton (2026-02-21)

**Question**: Can DKC be modeled as a finite automaton on the Voronoi cells?

**Answer**: No — but it's close (82.8% deterministic), and the percentage is a structural constant.

### Additive vs Multiplicative
The multiplicative braid product (M = σ₁^{±1}·σ₂^{±1}·...·σ₂^{±1}) **cannot compute XOR6** — every final cell gets exactly 8 even + 8 odd parity inputs. The reason: flipping all bits gives the conjugate braid (same eigenvector, negated angle), so the cell structurally cannot distinguish parity. DKC is fundamentally ADDITIVE (vector sums of quaternion weights), not multiplicative (matrix products).

### Determinism Test
For the additive formulation, at each step k, check: do all inputs in the same cell at step k-1 with the same bit at step k always land in the same cell? Result: 48/58 transitions (82.8%) are deterministic. All non-deterministic transitions are "bit=1" (adding a weight). All splits are binary (2-way). Cell 0 is the main bottleneck.

### Universal 82.8%
ALL 36 winning triples have exactly 82.8% determinism — zero variation. This is a structural property of the 13-direction Voronoi partition, not dependent on the triple. Follows from binary octahedral symmetry.

### Finer Partition
- 27 distinct partial sums at step 5 (theoretical minimum cells for 100%)
- 14-cell (unsigned) partition: 82.8%
- 27-cell (signed) partition: 87.1% (improves but not 100%)
- Bottleneck: partial sum magnitude (compass without odometer)

### Partial Sum State Space
| Step | Distinct sums | Cells used (14-cell) |
|------|--------------|---------------------|
| 0 | 2 | 2 |
| 1 | 3 | 2 |
| 2 | 6 | 5 |
| 3 | 9 | 5 |
| 4 | 18 | 14 |
| 5 | 27 | 14 |

### Key Insight
The Voronoi cell is a "compass without odometer" — it captures the direction of the partial sum's eigenvector but not its magnitude. Two partial sums pointing the same way but at different distances from the origin share a cell, then diverge when the same weight is added. This information loss is structural: it's built into the geometry of projecting 4D quaternion space onto a discrete set of 13 directions.

## Demo 74: Computational Invariant of Braids (2026-02-21)

**Question**: How does the DKC Voronoi cell compare to the Kauffman bracket as a braid invariant?

**Answer**: They are **incomparable** — neither subsumes the other. Combined, they form a strictly finer invariant.

### Bracket Collisions (Part A)
From 1,124,250 braid pairs: 58,078 bracket collisions (same bracket, different quaternion). Of these, **97.8% are separated by the Voronoi cell** (different cells for the two braids). Only 2.2% share a cell — these are quaternion pairs with the same eigenvector direction but different eigenvalue angles.

### Reverse Direction (Part B)
At the quaternion catalog level: 13 pairs share a cell. Of these, 7 have different bracket values. **The cell does NOT subsume the bracket.** Some bracket information (trace + normalization) is invisible to the cell.

### Combined Invariant (Part C)
| Invariant | Classes |
|-----------|---------|
| Voronoi cell | 14 |
| Quaternion | 24 |
| Bracket | 100 |
| **(Bracket, Cell)** | **119** |

119 > max(100, 14) proves the combination is strictly finer. 28/100 bracket values split across multiple cells. Up to 19 distinct brackets per cell.

### Computational Significance (Part D)
33/36 winning triples (92%) XOR6-separate at least some bracket collisions. The 3 exceptions are all [*,18,22] triples. 186 total XOR6-separated collisions across all 36 winners. DKC actively exploits information the bracket discards.

### Information Hierarchy (Part E)
```
Braid → Quaternion → Direction = Cell    (geometric: eigenvector)
Braid → Bracket                          (algebraic: trace + normalization)
Quaternion ↛ Bracket                     (NOT a function: many-to-many)
```
The bracket does not factor through the quaternion. This is why cell and bracket are incomparable — they're genuinely orthogonal projections of the braid.

### Computational Type Vectors (Part G)
36-bit type vector per cell: bit i = parity label under winning triple i. Only **6 distinct types** among 14 cells:
- Type 0,1,4: Three axis cells (0, 1, 10) — each unique
- Type 2: Body cells {2,3,4,5} — **computationally interchangeable**
- Type 3: Edge cells {6,7,8,9,11,12} — **computationally interchangeable**
- Type 5: Identity cell {13}

The DKC computation uses only 6 "roles." The 13-direction Voronoi is geometrically finer than computationally necessary.

### Concrete Examples (Part H)
σ₁⁸ (2-strand, identity quaternion, bracket (0,0,8,0)) collides with shorter 3-strand braids that land in axis cells. The bracket "sees" the algebraic value but is blind to the geometric destination.

### Refinement Chain (Part I)
43 bracket groups → 61 type subgroups → 168 quaternion subgroups. Max 4 types per bracket. Each level genuinely refines the previous.

### Full Hierarchy
```
Type (6) < Cell (14) < Quaternion (24) < Bracket (100) < (Bracket,Cell) (119)
```

### Key Insight
DKC doesn't "use the bracket differently" — it accesses a fundamentally different projection of the braid group representation. The bracket captures algebraic (trace) information; the cell captures geometric (eigenvector direction) information. These are complementary, not redundant. The cell's 14 classes can be organized into a parity-separating binary labeling; the bracket's 100 classes cannot. This is why DKC computes XOR6 through geometry, not algebra.

The [*,18,22] exception pattern: 3 winning triples with edge-type quaternions (q18, q22) have ZERO bracket collision separation — a "computational dead zone" where eigenvector labeling fails. Connects to Demo 70's music ⊥ space finding (r = -0.06).

**20/20 pass. Parts A-I. Wraps the Demo 66-74 invariant arc.**

## Demo 75: Binocular DKC — Multi-Output (2026-02-21)

**Question**: Can we read BOTH eigenvalue angle and eigenvector direction from one braid evaluation? Are they independent channels?

**Answer**: The eigenvalue angle channel **subsumes** the Voronoi cell channel. Binocular DKC is a zoom lens, not stereo vision.

### Eigenvalue Angle (Parts A-D)
Seven distinct angles from the additive sum quaternion: the 4 canonical (0°, 45°, 60°, 90°) plus 3 non-canonical created by quaternion addition: **35.264° (tetrahedral angle = arctan(1/√2))**, 30° (π/6), 65.905°. These are geometric invariants of the cube/octahedron/tetrahedron family, not present in individual 24-cell entries.

The angle perfectly predicts parity for all 36 triples — every angle value maps to a single XOR6 bit. The cell (1 bit) is a coarsening of the angle (~2 bits). Capacity: **2.282 bits per evaluation (2.3× over cell-only)**. The gain comes from reading 7 levels instead of 2.

### Eight-Fold Triple Classification (Part E)
36 winning triples produce only **8 distinct angle functions**, grouped into 8 families of 3-8 triples. Triples sharing an "axis pair" tend to share angle functions. This 8-fold structure is a natural classification beyond the 6-role type vectors of Demo 74.

### Additive Magnitude |S| (Part G)
The magnitude is quantized to exactly **6 values: √{0, 1, 2, 3, 4, 6}** — square roots of small integers. This comes from the inner product structure of the 24-cell (inner products ∈ {-1, -½, 0, ½, 1}). The missing √5 reflects the lattice geometry.

Magnitude does NOT reliably predict parity (16/36 = 44% of triples). H(|S|) = 1.839 bits. MI(|S|; cell) = 0.724 bits. This is Demo 73's "odometer" — the information the automaton lost. The odometer reads in algebraic numbers.

### Multiplicative Product (Part H)
Product M = Π w_i preserves the 24-cell group structure, so eigenvalue angles are restricted to the 4 canonical values only (no non-canonical). The product has HIGH entropy (H(cell) = 2.75 bits, H(angle) = 1.49 bits, total 4.24 bits) but is **completely blind to XOR6** (0/36 triples for both channels). A "kaleidoscope" — beautiful structure, computationally useless for parity.

**Key structural insight**: DKC computation requires BREAKING the group structure (addition creates non-group-element sums with non-canonical angles). Multiplication stays inside the group and cannot compute parity. MI(additive cell; multiplicative cell) = 0.517 bits — moderate correlation, far from redundant.

### Reconciliation with Demo 74
Demo 74: bracket and cell are incomparable at the CATALOG level (individual quaternions). Demo 75: angle subsumes cell at the COMPUTATION level (sums). Different math objects, different refinement relationships. Sums create finer resolution because the sum space is larger (non-unit quaternions with non-canonical angles).

**23/23 pass. Parts A-I (eigenvalue quantization, binocular output, MI, computable functions, triple classification, capacity, magnitude, multiplicative, comparison).**

---

## Demo 76: Scaling DKC to 8 Inputs (2026-02-21)

**Question**: Can DKC at ζ_8 compute XOR8 (8-input parity)?

**Answer**: **No. Zero solutions.** DKC at ζ_8 is a 6-input structure. The failure is total — best accuracy is 50% (random chance).

### XOR8 Enumeration (Part A)
Exhaustive search over all C(24,4) = 10,626 quadruples of the 24-quaternion catalog. Each quadruple encodes 8 inputs (4 × ±1 signs). **Zero of 10,626 quadruples compute XOR8.** Compare: 36 of C(24,3) = 2,024 triples compute XOR6 (1.78% hit rate).

### Failure Analysis (Part B)
The minimum conflict count is 2 (achieved by 11 quadruples, 0.1%). Even with only 2 conflict cells, the best quadruple [0,1,2,3] achieves **50.0% accuracy** (128/256) — exactly random. The conflict distribution: 57% of quadruples have 5-8 conflicts, 36% have 9-14.

### Capacity Argument
At 6 inputs: 64 masks / 14 cells ≈ 4.6 masks/cell. Geometrically possible for every cell to be parity-pure.
At 8 inputs: 256 masks / 14 cells ≈ 18.3 masks/cell. By pigeonhole, most cells see ~18 masks. The probability that ALL masks in a cell share parity drops exponentially. **The 14-cell Voronoi partition lacks the resolution to separate 128 even from 128 odd masks.**

### Direction Structure Collapse (Part C)
The best quadruple uses only **2 of 14 cells**: cell 0 gets 208 masks (81%), cell 13 gets 48 (19%). Compare 6-input winners which spread across 8-10 cells. With 4 quaternions per weight set (vs 3), sum vectors cluster more tightly toward the average direction.

### Algebraic Structure Breakdown (Parts D-E)
- **Eigenvalue angles**: 86 distinct (nearly continuous) at 8 inputs vs 7 (discrete, algebraic) at 6 inputs. The clean 7-angle quantization is specific to 3-term sums from the 24-cell.
- **Magnitudes**: 28 distinct |S| values (dense, non-algebraic) vs 6 (√{0,1,2,3,4,6}) at 6 inputs. The √n quantization breaks down at 4-term sums.

### Scaling Path
From Demo 69: ζ_16 has 3,457 eigenvector directions → 3,458 Voronoi cells. At that resolution, 256 masks / 3,458 cells ≈ 0.07 masks/cell — trivially parity-pure by pigeonhole. The open question: is there an intermediate root of unity with ~56 directions (giving ~4.6 masks/cell for 256 masks) that would yield non-trivial XOR8?

**10/10 pass. Parts A-E (XOR8 enumeration, failure analysis, direction structure, eigenvalue angles, magnitude/capacity).**

---

## Demo 77: Activation Zoo — XOR8 SOLVED (2026-02-21)

**Question**: Can a different activation function enable XOR8 at ζ_8?

**Answer**: **YES.** Combined Sector(8) × Voronoi activation (112 cells) finds **6 XOR8 winners** out of 10,626 quadruples. The wall was in the activation function, not the root of unity — exactly mirroring Demo 50.

### The Headline Result
14 activation functions tested across 10,626 quadruples. Only one succeeds: the product S¹ × S² activation with 8 angle sectors × 14 Voronoi cells = 112 combined cells (2.3 masks/cell). Neither channel alone suffices: pure sector (k=16, 96.9%) and pure Voronoi (14 cells, 90.6%) both fail. This proves Demo 74's incomparable invariants are **computationally necessary**, not just mathematically distinct.

### Accuracy Cascade
Accuracy grows with cell count across all activation types:
- S² Voronoi (14 cells): 90.6%
- Sector k=16 (16 cells): 96.9%
- Polar 14×6 (84 cells): 99.2% — tantalizingly close
- Combined 8×14 (112 cells): **100%** — 6 perfect winners

Phase transition between 84 cells (0 winners) and 112 cells (6 winners). Critical threshold: ~100 cells.

### The 6 Winners: Paired Quaternion Structure
Every winning quadruple contains a PAIR of quaternions sharing the same eigenvector direction. Removing either pair element leaves an XOR6-winning triple. Thus: 12 of 24 triples embedded in winners are XOR6 winners. XOR8 winners are built from XOR6 winners by adding a directional partner.

15 of 24 catalog entries participate. Dominant cells: 0 (axis-x), 1 (axis-z), 10 (edge diagonal) — each appears 6 times across the 24 winner slots. Only 2 eigenvalue half-angles: 45° and 90° (canonical octahedral angles only).

### Correcting Demo 76
"ζ_8 is a 6-input structure" was activation-specific. ζ_8 with S² Voronoi IS limited to 6 inputs. ζ_8 with combined S¹ × S² activation supports at least 8 inputs. The algebraic structure is richer than any single projection reveals.

### Key Structural Findings
- S³ Voronoi (24-cell, 24 cells) underperforms: 90.2%. The S³ partition mixes angle and direction non-optimally.
- Polar (direction × magnitude) at 84 cells reaches 99.2% — magnitude helps enormously but doesn't cross the threshold alone.
- The combined activation is the quaternionic generalization of Aizenberg's MVN: read BOTH eigenvalue (S¹) and eigenvector (S²).
- Open question: can combined activation support 10, 12, or more inputs?

**14/14 pass. Parts A-F (sector, polar, S³, combined, comparison, winner analysis).**

## Demo 78: Recursive Scaling + Phase Diagram — COMPLETE (2026-02-21)

**File:** `knotapel/demo_78_recursive_scaling/main.c`
**Tests:** 9/9 pass (Parts A-E)

### Core Question
Does the recursive structure (XOR8 = XOR6 + shadow) generalize to XOR10, XOR12?

### Answer: **NO.** XOR10 has ZERO winners at ζ_8.

Both recursive search (120 candidates, extending 6 XOR8 winners with remaining catalog entries) and exhaustive brute-force (all C(24,5) = 42,504 quintuples) agree: no combination of 5 catalog entries computes XOR10 with Combined Sec(k) × Voronoi activation, tested at k = 8, 10, 12, 16. The recursive scaling hits a hard wall.

### Brute-Force Results (Part B)
| Sectors | Cells | Winners | Best Accuracy |
|---------|-------|---------|---------------|
| k=8     | 112   | 0       | 94.1%         |
| k=10    | 140   | 0       | 98.4%         |
| k=12    | 168   | 0       | 96.9%         |
| k=16    | 224   | 0       | 98.4%         |

Non-monotonic in k: k=12 (96.9%) worse than k=10 (98.4%). Same pi/4 lattice incommensurability as Demo 50's non-monotonic sector counts.

### Phase Diagram (Part D)
| N inputs | 2^N masks | Min cells | Winners | Activation |
|----------|-----------|-----------|---------|------------|
| 6        | 64        | 14        | 36      | Voronoi    |
| 8        | 256       | 112       | 6       | Sec8 × Vor |
| 10       | 1024      | ---       | 0       | ---        |
| 12       | 4096      | ---       | 0       | ---        |

### Shadow Structure Confirmed (Part E)
- Every XOR8 winner has exactly 2 XOR6 parents (all 6 verified)
- 12/12 shadows share Voronoi cell with parent member (100%)
- ALL shadow half-angles = 45° (the canonical octahedral angle)
- Shadow pairs: {0,1} in cell 0, {4,5} in cell 1, {19,21} in cell 10
- These are the 3 dominant cells from the 24-cell structural skeleton

### Why XOR10 Fails
Three converging explanations:
1. **Catalog exhaustion:** 24 entries with only 13 directions and 2 eigenvalue angles (45°, 90°). Not enough structural diversity for 5-tuple separation.
2. **Information-theoretic limit:** 1024 masks across 224 cells = 4.6 masks/cell. Same ratio where XOR6 barely worked (64/14=4.6), but the catalog can't generate diverse enough sums at 10 inputs.
3. **Incommensurability:** The pi/4 angles create systematic collisions when 5 entries are summed. Non-monotonicity in k confirms destructive interference at certain sector boundaries.

### Key Implication
The root of unity ℓ directly constrains max computable input size:
- ζ_8 catalog (24 entries): max = 8 inputs
- Higher ℓ → larger catalog → potentially higher max inputs
- The scaling law max_inputs = f(|catalog|, n_directions, n_angles) is the open question

### Open Questions
1. What does ℓ=12 or ℓ=16 buy? Larger catalogs with more directions and angles.
2. Is there a different activation geometry (not Sec×Vor) that could break through?
3. What are the 16 misclassified masks at 98.4%? Do they form a structured pattern?
4. Is max_inputs = 2·ℓ a conjecture? (ℓ=4 gave max=8... but ℓ=3 gave max=6 too, so max_inputs = 2·ℓ for ℓ≥3?)

## Demo 79: ζ₁₂ Capacity Test — COMPLETE (2026-02-21)

**File:** `knotapel/demo_79_zeta12_capacity/main.c`
**Tests:** 14/14 pass (Parts A-E)

### Core Question
Does the pattern ζ_N → N-input capacity hold? Test ζ₁₂ (half-angle = π/6 = 30°).

### Answer: **ζ_N → N is REFUTED.** ζ₁₂ gives max 10 inputs, not 12.

But ζ₁₂ DOES break through the ζ₈ wall: XOR10 goes from 0 winners to **124 winners**.

### Catalog Comparison
| Property      | ζ₈          | ζ₁₂          | Ratio  |
|---------------|-------------|---------------|--------|
| Quaternions   | 24          | 256 (CAPPED)  | 10.7x  |
| Directions    | 13          | 64            | 4.9x   |
| Half-angles   | 2 (45°,90°) | 12            | 6x     |
| Voronoi cells | 14          | 65            | 4.6x   |

ζ₁₂ half-angles: 0°, 29°, 30°, 41.4°, 49.5°, 51.3°, 60°, 64.3°, 75.5°, 77.5°, 82.8°, 90° — dramatically richer angular spectrum.

### Capacity Ladder
| Level | Masks | Winners | Min k | Cells | Note         |
|-------|-------|---------|-------|-------|--------------|
| XOR6  | 64    | 1024+   | 1     | 65    | Voronoi-only |
| XOR8  | 256   | 1024+   | 6     | 390   | Sec6 × Vor   |
| XOR10 | 1024  | **124** | 8     | 520   | Sec8 × Vor   |
| XOR12 | 4096  | 0       | ---   | ---   | **WALL**     |

### Phase Diagram (Combined)
| Root | N  | 2^N  | Min cells | Winners | Activation |
|------|----|------|-----------|---------|------------|
| ζ₈   | 6  | 64   | 14        | 36      | Voronoi    |
| ζ₈   | 8  | 256  | 112       | 6       | Sec8×Vor   |
| ζ₈   | 10 | 1024 | WALL      | 0       | ---        |
| ζ₁₂  | 6  | 64   | 65        | 1024+   | Voronoi    |
| ζ₁₂  | 8  | 256  | 390       | 1024+   | Sec6×Vor   |
| ζ₁₂  | 10 | 1024 | 520       | 124     | Sec8×Vor   |
| ζ₁₂  | 12 | 4096 | WALL      | 0       | ---        |

### Nesting: NOT nested
- Only 3/24 ζ₈ quaternions found in ζ₁₂ (12%)
- Only 2/13 ζ₈ directions found in ζ₁₂ (15%)
- Different half-angles (π/4 vs π/6) generate different finite subgroups of SU(2)
- The capacity gain comes from genuinely different algebraic structure, not "more of the same"

### CRITICAL UPDATE: XOR12 IS SOLVABLE (catalog was truncated)

Follow-up test (`xor12_fast.c`) with expanded catalog (4096 entries via group closure):
- **50+ XOR12 winners found** (capped at 50, only 11/124 parents tested)
- Min k = 8 (4104 cells with 513 Voronoi cells)
- The 6th quaternion always comes from the expanded catalog (index > 256)

**The N-2 pattern was WRONG.** The XOR12 "wall" was an artifact of the truncated catalog.

### Finite vs Infinite Group — THE Key Finding

| Round | ζ₈ size | ζ₁₂ size |
|-------|---------|----------|
| 0     | 5       | 5        |
| 1     | 15      | 17       |
| 2     | 23      | 51       |
| 3     | 24 (CLOSED) | 127  |
| 4     | —       | 275      |
| 5     | —       | 564      |
| 6     | —       | 1140     |
| 7     | —       | 2292     |
| 8     | —       | 4096+ (cap) |

- ζ₈ → binary octahedral group (order 48, 24 up to ±). **FINITE.**
- ζ₁₂ → still growing past 4096, roughly doubling each round. Possibly **INFINITE.**
- At 4096: **512 directions**, **43 half-angles** (vs 13 dirs, 4 angles at ζ₈)

**This is the fundamental difference**: finite group = hard capacity ceiling, infinite group = no ceiling (limited only by computational search).

### Updated Phase Diagram
| Root | N  | 2^N  | Min cells | Winners | Activation |
|------|----|------|-----------|---------|------------|
| ζ₈   | 6  | 64   | 14        | 36      | Voronoi    |
| ζ₈   | 8  | 256  | 112       | 6       | Sec8×Vor   |
| ζ₈   | 10 | 1024 | WALL      | 0       | --- (finite group) |
| ζ₁₂  | 6  | 64   | 65        | 1024+   | Voronoi    |
| ζ₁₂  | 8  | 256  | 390       | 1024+   | Sec6×Vor   |
| ζ₁₂  | 10 | 1024 | 520       | 124     | Sec8×Vor   |
| ζ₁₂  | 12 | 4096 | 4104      | 50+     | Sec8×Vor   |

### Open Questions (Updated) → ANSWERED by Demo 80

---

## Demo 80: Group Finiteness Survey — COMPLETE (2026-02-21)

**File:** `knotapel/demo_80_group_finiteness/main.c`
**Tests:** 11/11 pass

### Core Question
Which roots of unity generate finite vs infinite SU(2) subgroups?

### Answer: **Only ζ₄ and ζ₈ are finite. Everything else is infinite.**

| Root  | Angle  | Size   | Dirs | Angles | ADE Type            |
|-------|--------|--------|------|--------|---------------------|
| ζ₄    | 90.0°  | 4      | 3    | 2      | Binary Dihedral 2D₂ |
| ζ₆    | 60.0°  | >4096  | 512  | 37     | INFINITE            |
| ζ₈    | 45.0°  | 24     | 13   | 4      | Binary Octahedral   |
| ζ₁₀   | 36.0°  | >4096  | 512  | 71     | INFINITE            |
| ζ₁₂   | 30.0°  | >4096  | 512  | 43     | INFINITE            |
| ζ₁₆   | 22.5°  | >4096  | 512  | 51     | INFINITE            |
| ζ₂₀   | 18.0°  | >4096  | 512  | 67     | INFINITE            |

### Key Findings

1. **The boundary ALTERNATES**: ζ₄ (finite), ζ₆ (infinite), ζ₈ (finite), then all infinite. NOT monotonic.

2. **ζ₈ = Binary Octahedral (2O, E₇)**, order 48 (24 up to ±). 4 half-angles: 0°, 45°, 60°, 90°.

3. **ζ₁₀ is NOT binary icosahedral** — it's infinite. The Fibonacci/golden ratio angle does not close.

4. **Only π/2 and π/4 generate finite groups** among the tested angles. These are rotational symmetries of the cube/octahedron.

5. **All infinite groups** at 4096 entries: 512 directions (all 8 octants), approaching uniform S² coverage. Angle gaps < 1° (approaching continuum).

### Computational Implication

- **Finite group (ζ₄, ζ₈)** → hard capacity ceiling (XOR4 and XOR8 respectively)
- **Infinite group (all others)** → potentially unbounded capacity, limited only by computational search cost
- The capacity hierarchy isn't "higher roots → more capacity." It's "finite vs infinite."

### Open Questions
1. Is ζ₃₂ (π/16 = another power of 2) also finite? Conjecture: only π/(2^k) generate finite groups.
2. Which infinite group is "best" for computation? ζ₁₀ has 71 angles vs ζ₆'s 37 at same catalog size.
3. Do all infinite groups give unbounded capacity, or do some have structural limitations?

## Demo 81: Capacity Scaling Law — COMPLETE (2026-02-21)

**File:** `knotapel/demo_81_capacity_scaling/main.c`
**Tests:** 14/14 pass

### Core Question
For ζ₁₂ (infinite group), how does computational capacity scale with catalog size?

### Answer: **Logarithmic scaling. 0.62 XOR per doubling.**

| Round | Cat   | Dirs | Ang | MaxXOR | log₂(C) |
|-------|-------|------|-----|--------|---------|
| 0     | 5     | 2    | 2   | 6      | 2.32    |
| 1     | 17    | 6    | 4   | 6      | 4.09    |
| 2     | 51    | 22   | 6   | 8      | 5.67    |
| 3     | 127   | 46   | 10  | 8      | 6.99    |
| 4     | 275   | 114  | 12  | 10     | 8.10    |
| 5     | 564   | 239  | 16  | 10     | 9.14    |
| 6     | 1140  | 507  | 21  | 12     | 10.15   |
| 7     | 2292  | 512  | 35  | 12     | 11.16   |
| 8     | 4096  | 512  | 43  | 12     | 12.00   |

### Key Findings

1. **Logarithmic**: max_xor ≈ 0.62 × log₂(cat_size) + 4.6. Monotonically non-decreasing. 3 clean transitions.

2. **Transitions require ~10x catalog growth**: XOR6→8 at ~51, XOR8→10 at ~275, XOR10→12 at ~1140. Each jump: ~3.2 more doublings.

3. **Directions drive transitions**: 22 dirs → XOR8, 114 dirs → XOR10, 507 dirs → XOR12. Directions saturate at MAX_DIR=512 after round 7.

4. **Angles sustain growth after saturation**: When directions hit 512, angles keep growing (35→43). XOR12 winners go from 12 to 32. Angular diversity is the long-term resource.

5. **Consistent with #P-hardness**: Exponential catalog cost per +2 XOR level matches Kuperberg's result for non-lattice roots.

### Predictions
- XOR14 needs ~38,000 entries (~11 closure rounds)
- XOR16 needs ~615,000 entries (~14 rounds)
- XOR20 needs ~160 million entries (~20 rounds)

### The Two Regimes
- **Finite groups (ζ₄, ζ₈)**: Hard ceiling. No scaling. 24 entries → max XOR8, period.
- **Infinite groups (ζ₁₂, etc.)**: Logarithmic scaling. Catalog size = exponential function of desired XOR level. Tractable but expensive.

## Demo 82: Crossing Depth and Computational Role — 17/17 pass

**File:** `knotapel/demo_82_crossing_depth/main.c`

### The Question
Each quaternion in the closure catalog is "born" at a specific round (depth). Does depth matter for computation, or is it just catalog size?

### Part D Results — The Crux

| Subset      | Size | Dirs | Ang | Max XOR |
|-------------|------|------|-----|---------|
| Shallow 127 | 127  | 46   | 10  | 8       |
| Strided 127 | 127  | 125  | 37  | 10      |
| Deep 127    | 127  | 119  | 19  | 10      |
| Shallow 564 | 564  | 239  | 16  | 10      |
| Strided 564 | 564  | 512  | 43  | 10      |
| Deep 564    | 564  | 476  | 19  | **12**  |

Deep entries BEAT shallow at matched sizes. Deep 564 reaches XOR12 with only 564 entries (full catalog needed ~1140).

### Key Findings

1. **Winners = shallow core + deep extensions**: ALL winners at ALL levels contain depth-0 entries (generators). Mean depth rises: 0.52 (XOR6) → 1.98 (XOR12).

2. **Two vocabulary regimes**: Depths 0-6 add directions (2→507). Depths 7-8 add angles (21→43) after directions saturate at 512. Angular refinement is the bottleneck for higher XOR.

3. **Vocabulary alone insufficient**: Strided 564 has the MOST vocabulary (512 dirs, 43 angles) but doesn't reach XOR12. Deep 564 has fewer dirs (476) and angles (19) but reaches XOR12. Algebraic relationships between deep entries matter beyond raw vocabulary.

4. **Deep entries alone work**: Pure depth-8 entries (without generators!) compute XOR10 and XOR12. They contain "effective generators" from algebraic structure.

5. **Reinterpretation of the scaling law**: Demo 81's "logarithmic in catalog size" is an artifact of exponential growth per round. The TRUE relationship is **linear in depth**: max_xor ≈ depth + 6. Each closure round adds +2 XOR at transition depths 0, 2, 4, 6. The log law was measuring the wrong variable — catalog size instead of crossing depth.

6. **Algebraic coherence**: The strided sample LOSES to deep despite having more vocabulary. Deep entries share common generator factors (algebraic coherence). A more complexly knotted strand computes more than many simply knotted strands covering more frame space.

## Theoretical Framework: Higher-Dimensional Manifolds as 3D Knots

### Core Idea

Higher-dimensional manifolds can be represented as 3D knot configurations. The dimensional formula:

    dimension = strands × 2 + writhe events

where writhe events are ≥180° self-crossings of a single strand. The same dimension admits multiple decompositions (partitions), each corresponding to a different manifold type.

### Examples

- **S³ = 1 strand + 1 writhe.** Our SU(2) catalog lives here. Each quaternion is a single knotted strand configuration. This is the base case for DKC.
- **S⁷ (octonionic) = 2 strands + 3 writhes OR 3 strands + 1 writhe.** Different decompositions = different manifold types within the same dimension.
- The division algebra sequence (R, C, H, O) = (1, 2, 4, 8) maps to crossing complexity, not spatial dimensions.

### Even vs Odd Dimensions

- **Even-dimensional spheres**: Possible with 0 writhes (pure strand interaction).
- **Odd-dimensional spheres**: Must include at least 1 writhe.
- This explains the **hairy ball theorem**: even spheres lack the preferred orientation that writhe provides. No writhe = no global combing direction.

### Connection to the Kauffman Bracket and DKC

The Kauffman bracket is NOT writhe-invariant — it requires a (-A³)^w correction to produce the Jones polynomial. DKC uses **raw bracket values**, meaning we compute WITH the writhe data that most knot theorists normalize away. This may be why DKC works: the writhe carries computational information that the Jones polynomial discards.

**DKC addition (not multiplication)** = linking separate knots together, not composing crossings within one strand. The link interaction is where higher-dimensional structure and computation emerge. When we sum quaternions w₁ + w₂ + w₃, we're computing the combined topology of three linked knot configurations.

### Connection to Our Findings

- **Finite groups (ζ₄, ζ₈)** = crystallized knot types with bounded crossings. The binary octahedral group (ζ₈, 24 elements) = the 24 distinct knot configurations possible in one specific crystallographic symmetry.
- **Infinite groups (everything else)** = dense knot types with unbounded crossings. The group closure never terminates because there's always a new knot configuration reachable by one more crossing.
- The **finite/infinite transition** is the transition from "one specific knot crystal" to "all knots" — from a lattice to a continuum.
- The **logarithmic scaling law** (Demo 81) means each +2 XOR inputs requires exponentially more knot configurations. This is the computational cost of exploring higher-dimensional manifold structure through 3D knots.

### Mathematical Foundations

The framework reinterprets existing results as a visualization paradigm:
- **Alexander's theorem**: Every closed braid is a knot/link. Our braid-word enumeration = systematic knot exploration.
- **Kirby calculus**: Surgery presentations of manifolds. Handle slides = braid relations in our catalog.
- **Piergallini branched coverings**: Every closed oriented 4-manifold is a branched cover of S⁴. The branching structure = our writhe events.
- **Lickorish-Wallace surgery**: Any closed orientable 3-manifold from surgery on links in S³. Our quaternion catalog = surgery instructions.

### Practical Application

A tool to **visualize higher-dimensional manifolds as decorated 3D knot diagrams**. Currently impossible to directly visualize >3D. The decoration (writhe markers, strand labels, bracket values) carries the extra-dimensional information.

### Future Demo Directions

- Take known 4-manifolds (CP², S²×S², K3 surface), extract Kirby diagrams
- Verify dimensional formula against known topology
- Compute bracket values of Kirby link components
- Check if DKC output encodes known topological invariants (signature, Euler characteristic)
- Test whether different decompositions of the same dimension give different DKC capacities

## Conceptual Developments (Post-Demo 82)

### Neural Network Depth Analogy
Crossing depth = synaptic graph wiring, catalog breadth = number of neurons. You can't replace depth with width — same principle as deep learning theory. Deep entries have "algebraic coherence" (shared generator factors) = coherent wiring patterns. Strided/random samples destroy this coherence = random wiring, can't compute higher-order functions. This is the DKC version of the depth-vs-width theorem in neural network theory.

### Octonionic / Division Algebra Connection
The division algebra sequence R(1), C(2), H(4), O(8) maps to crossing complexity, not spatial dimensions. ζ₈ capacity ceiling = 8 = dim(O) — possibly not coincidence. Non-associativity of octonions ↔ non-commutativity of braids: Yang-Baxter equation = controlled exceptions to order-dependence. E8 chain: the 24-cell (our ζ₈ group) is the D₄ root system, which sits inside E8, which gives the optimal sphere packing in 8D. The tower of exceptional structures may be the algebraic spine of DKC.

### Quantum Dimension Singularity Explanation
- At ζ₈: q = A² = i, quantum integer [2]_q = q + q⁻¹ = i + (-i) = 0. Representation theory degenerates → finite group (24 elements).
- At ζ₄: q = -1, [2]_q = -1 + (-1) = -2... actually q = A² and A = ζ₄ = i, so q = -1, [2]_q = -1 + (-1) = -2. But for the Kauffman bracket δ = -(A² + A⁻²) = 0 at ζ₈, and δ = -(i + (-i)) = 0 at ζ₄ too. Both are roots where quantum dimensions vanish.
- All other roots: quantum dimensions nonzero, generic representation theory → infinite groups.
- Finite groups = lattice roots = Kuperberg-tractable. Infinite = non-lattice = #P-hard.

### Hopf Fibration as Projection Pipeline
S⁷ → S⁴ → S² with fibers S³ and S¹. Our computational pipeline mirrors this:
- Full data (pair of quaternions) lives in S⁷
- Direction projection (eigenvector on S²) = second Hopf map
- Angle extraction (eigenvalue on S¹) = fiber of first Hopf map
- "Angle subsumes cell" = information loss at each Hopf projection stage
- "Compass without odometer" = having direction but losing angle = stuck at S² level

### Bracket vs Jones Hypothesis
DKC uses the Kauffman bracket (writhe-dependent), NOT the Jones polynomial (writhe-independent). The writhe data that Jones normalizes away (via (-A³)^w correction) may be exactly why DKC computes. Writhe = odd-dimensional data in the strand framework. Demo 82 supports this: depth (which accumulates writhe through generator multiplication) drives computation. Every crossing product adds writhe; deeper entries carry more writhe information.

### Knot Theory Research Landscape Mapping
- **Kauffman virtual knots**: writhes in our framework may BE virtual crossings (exist on surfaces, not in 3D)
- **Rasmussen invariant**: 4D subtleties invisible in 3D = what the manifold framework makes visible
- **Categorical tangles**: "knots are shadows of higher-dimensional structures" = concrete version of our thesis
- **Grid number complexity**: analogous to our crossing depth measure
- **Polymer entanglement / topological glass**: our crystal (finite group) vs fluid (infinite group) might be literal, not metaphor

### Open Questions for Future Demos
1. Does writhe alone compute? (Extract writhe from catalog entries, test independently)
2. Kirby calculus test: take known 4-manifolds (CP², K3), compute bracket values of their knot diagrams, check if DKC output encodes known topological invariants
3. Virtual crossings test: do the writhes in our framework correspond to Kauffman's virtual crossings?
4. Is the 0.62 ≈ 1/φ coefficient in the (now-superseded) log scaling law meaningful or coincidence?
5. Research PDF deep reading in progress (18 papers in knot-pdfs/ and research-ingress/)

## PDF Research Findings — Key Implications for DKC

*Source: 18 papers across knot-pdfs/ and research-ingress/, analyzed February 2026. Full per-paper notes in knot-pdfs/notes-*.md*

### The Framing Anomaly Thesis (FOUNDATION)

Kirby (1978) proved framing determines 3-manifold topology. Jones (1985) normalizes framing away. Witten's TQFT subtracts the "framing anomaly." DKC KEEPS the framing (uses bracket, not Jones). The computational content of DKC lives in exactly the data that mainstream knot theory and physics discard. This was our bracket-vs-Jones hypothesis from the strand theory discussion — the PDFs confirm it from two independent directions:
- **Kirby's surgery theorem**: framing IS the essential surgical datum. Without it you cannot distinguish 3-manifolds.
- **Witten's TQFT structure**: delta=0 is where TQFT becomes maximally degenerate (level k=0, only trivial representation). The "framing anomaly" that physicists subtract to get topological invariants contains the computational content DKC exploits.

Kirby established this 8 years before Jones, then the field followed Jones anyway. DKC is recovering what was discarded.

### The 4-Manifold Bridge Is Confirmed

Craven et al (2022) show Jones polynomial evaluated at roots of unity e^{πin/(k+2)} for k=3-10 predicts 4D invariants (Rasmussen s-invariant, slice genus) at >96% accuracy. Accuracy is STABLE across k values. Even the raw Jones polynomial (no root evaluation) gets ~95%. Simple 2-layer networks suffice — consistent with our 24-element binary octahedral group being the right-sized structure. This validates the Kirby calculus demo direction and Fran's manifold framework. DKC at roots of unity IS 4-manifold computation.

Central open question from the paper: "What is four-dimensional about the Jones polynomial?" DKC may be part of the answer.

### Circuit Interpretation Is Now Precise

Kirby's blowing-up procedure (Section 5 of his 1978 paper) establishes: k crossings in a knot ↔ unknot + k additional ±1-framed unknotted components. Each R-matrix multiplication in our closure algorithm = one gate. Depth = circuit depth. Demo 82's "capacity linear in depth" = "capacity linear in circuit depth." This makes the neural network analogy exact — not metaphorical, literally a circuit.

Handle slides are non-commutative (sliding γ_i over γ_j ≠ sliding γ_j over γ_i), matching braid group non-commutativity.

### Crossing Depth Bounded by Genus

Ozsváth-Szabó (2004) note in a single sentence (p.316) that Kauffman states can be identified with simultaneous trajectories on Heegaard diagrams. This connects DKC's state-sum bracket evaluation directly to Heegaard Floer homology — the most powerful modern knot invariant. The filtration level in HFK is bounded by the Seifert genus: HFK(K, i) = 0 for |i| > g(K). Our crossing depth measure has a topological interpretation — bounded by genus of the knot. Demo 82's depth structure maps onto genus stratification. L-spaces (minimal HF rank) parallel DKC's finite-group ceiling at lattice roots.

### Two Gates Suffice

Piergallini & Zuddas (2018): only cones and nodes (= positive and negative crossings) needed for ALL 4-manifolds. Minimal gate set. Our R-matrix and its inverse are the two gates. The "borrow-and-return" technique (introduce temporary node to create relabeling room, then remove it) is the 4-manifold version of ancilla qubits — could inspire DKC: temporary crossings to enable otherwise-blocked computations.

### The 24 Coincidence

|S₄| = 24 = |binary octahedral group|. Different groups, same order, both create capacity ceilings:
- DKC: binary octahedral group bounds distinct bracket values → computational ceiling for Boolean functions
- Piergallini: S₄ is the minimum symmetric group to represent all 4-manifolds as branched coverings

Stabilization (adding trivial 5th sheet to resolve node singularities) directly parallels MVN k=4→k=6 unlocking parity. Both are "adding trivial resources that enable topological rearrangements previously blocked." S₄ and the binary octahedral group are both related to the symmetries of the cube/octahedron — the 24 appears structurally, not coincidentally.

### Entanglement Vanishing = Node Singularity

At nodes in branching surfaces, two sheets cross with DISJOINT monodromy transpositions (i j) and (k l) where {i,j}∩{k,l}=∅. This is the covering-space version of DKC's entanglement vanishing (non-interleaving braids have bracket=0 at delta=0). The singularity exists precisely because the two branching sheets DON'T entangle.

### Parity Effect in Signature

Davies et al (2022, DeepMind): σ(K) ≈ -(1/2)Σℓᵢ²qᵢ where ℓᵢ is linking number and qᵢ is twist count. Linking number QUADRATICALLY amplifies signature contribution. Correction terms only come from geodesics with ODD linking number — even-parity contributes nothing. We compute XOR (parity). The parity selectivity in 4D signature might be the same phenomenon viewed from the manifold side.

### Open Leads for Future Demos

1. **Mod-16 vs mod-8**: Rohlin's theorem (signature divisible by 16 for spin 4-manifolds) vs our ζ₈ periodicity. Unexplored relationship.
2. **Cobordism witnesses as computational paths**: Kirby's proof constructs then discards a 5D cobordism. "Proves programs are equivalent but throws away the compiler." The compilation path might carry computational content.
3. **Grid diagram permutation pairs as bridge to catalog**: Grid diagrams = (σ_X, σ_O) ∈ S_n × S_n. Interleaving commutation = single crossing change. Knot fauna phase transition at grid number 9-16 (like DKC four-tier structure).
4. **"What is 4D about Jones?"**: DKC may be part of the answer — the bracket at roots of unity accesses 4-manifold structure via the framing data Jones discards.
5. **3D vs 2D computational complexity**: Only linking number is proven faster to compute in 3D (Bar-Natan et al). Jones, Alexander, HOMFLY-PT all have UNKNOWN 3D computational status. V^{4/3} projection inflation means 2D crossing count is dimensionally distorted.

---
*End of Explorer's Log*
