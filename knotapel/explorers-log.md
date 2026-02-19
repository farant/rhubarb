# Explorer's Log — DKC Research Arc & Demo 35

Last updated: 2026-02-19
Context: Written after completing Demo 35 (TL_n matrix representations over Z[ζ₈]).

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
- At delta=0: e_i² = 0 (NILPOTENT). Non-semisimple. Has a radical.
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

## Follow-On Experiments (Demo 51+)

### Demo 51: Radical Anatomy & Vassiliev Connection
The radical filtration of TL_n at delta=0 (rad ⊃ rad² ⊃ ...) may encode Vassiliev type. Demo 35's machinery makes this computable. The nilpotent generators are confirmed; next step is computing radical powers and correlating with bracket behavior.

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

---
*End of Explorer's Log*
