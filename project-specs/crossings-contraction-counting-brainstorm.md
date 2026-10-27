# Crossings, Contraction, and Counting

An experimental program in rhubarb, following the October 2026 ETH papers · Oct 27, 2026 · @Fran Arant

*Revised Oct 27, 2026 (Claude) after reading both papers in full at their core, both Lean statement files, and the README of Anthropic's formalization. Changes are marked* (rev. 10-27); *§11 records what was measured and what was not.*

## 1. Purpose and scope

This program tests whether the techniques behind the two October 2026 refutations of the Exponential Time Hypothesis give real speedups on real problems, built in rhubarb in C89. The first concrete target is an exact tensor-network contraction library, validated on knot invariants, then used to test the papers' tricks on non-planar networks.

Sigla used below:

| Siglum | Work |
| --- | --- |
| ANT | Anthropic, *A 2^o(n) Time Algorithm for 3-SAT* (Oct 26, 2026) |
| OAI | OpenAI, *The Exponential Time Hypothesis is False* (Oct 17, rev. Oct 25, 2026) |
| ANT-L | ANT's Lean 4 formalization and README, `../formal-math/eth/` (clone of github.com/anthropics/formal-math, 16a5ce4) *(rev. 10-27)* |
| OAI-L | OAI's Lean 4 formalization, `../math/lean/OAI/Algorithms/SubexponentialSAT/` (clone of github.com/openai/math, 7ef5c0cc5) *(rev. 10-27)* |
| AVW | Alman and Vassilevska Williams, *Truly Subquadratic 3SUM and Truly Subcubic APSP via Triangles in Sparse Lopsided Graphs* (arXiv:2610.06783), formalized in `../formal-math/3sum-apsp/` *(rev. 10-27)* |
| FTS | OpenAI, *Finite tensor savings and exact Fourier circuits* (Sep 25, 2026), `../math/preprints/` *(rev. 10-27)* |

Both clones are read-only research data: nothing in them is built or run from inside rhubarb.

**In scope**

- Exact counting and sum-product evaluation: model counting, partition functions, knot polynomials, graph polynomials.
- Contraction-order search, slicing, and crossing decompositions in the style of ANT.
- Evaluate-then-interpolate arithmetic in the style of OAI, used as an engineering tool.

**Out of scope**

- Implementing either paper's full recursion. Both are galactic: ANT's bound beats brute force only past n > 10^10000 for 3-CNFs with 5n clauses.
- Optimization problems in the max-plus sense (shortest paths, MAP inference), except by encoding them as counting (see 3).
- Approximate or floating-point inference.

**Definition of success:** a contraction engine that reproduces every tabulated Jones polynomial, breaks `laqueus`'s 24-crossing ceiling, and gives a measured answer, positive or negative, on whether a crossing decomposition beats plain slicing on at least one non-planar benchmark.

## 2. Background: the two techniques

Both papers break the same working assumption: that a sum over a structured problem costs about 2^(width of the best cut). Each rewrites the problem as a **signed combination** of related problems that decompose better than the original, and lets cancellation do the work.

### 2.1 ANT: crossing decomposition with a per-term contraction order

1. The formula becomes a network: nodes hold local variables, edges are equality tests, and the count is a sum of products over the network (ANT eq. 2, Definition 2).
2. A function **factors through c bits** if it is a sum over 2^c products of a row-side function and a column-side function (Definition 1). Merging and cutting nodes only rearrange the sum; the cost is set by bits crossing between groups.
3. Nodes are placed on a grid. The two diagonals of each rectangle form a **crossing** \[x = v\]\[y = u\], which costs full price whichever way the grid is merged.
4. The **eight-square identity** (Degen 1818, Graves 1843; octonion sign table σ) rewrites 2·\[crossing\] = R + C, where R is cheap with rows merged and C is cheap with columns merged (ANT eq. 31).
5. Per 3-bit block, both directions together cost 11 bits instead of 12. Grouping terms by how many blocks take C gives polynomially many pieces A\_0 … A\_t (Lemma 2).
6. Each expansion term merges rows or columns, whichever is cheaper, then recurses. Exponents shrink by ρ = 11/12 per level (Proposition 3).
7. This escapes the Austrin–Kaski–Kubjas lower bound for single-contraction-tree algorithms because different terms are contracted in different orders (ANT Appendix G).
8. Appendix B improves ρ to 1/√2 using operators on Q\[t1, t2, …\] (the Heisenberg vertex algebra); bit costs become logarithms of partition numbers.
9. *(rev. 10-27)* **The one-bit form (ANT-L README, ANT Appendix H).** Per bit, the crossing is a SWAP, and SWAP = I − 2·P, where P projects onto the singlet (|01⟩ − |10⟩)/√2; 2P has integer entries 0, ±1. In indicator form: \[x = v\]\[y = u\] = \[x = u\]\[y = v\] − \[x ≠ y\](−1)^x · \[u ≠ v\](−1)^u. Grouping the 2^λ terms by the number i of singlet factors gives λ+1 pieces, each with (row rank)·(column rank) ≤ 12^λ, against 16^λ for the crossing: about log₂ 12 ≈ 3.58 bits per bit instead of 4. Counted by ranks this beats the eight-square route (3 + 6 + 2 = 11 of 12, about 3.67 per bit). Counted naively by bits it gains nothing (Appendix H, "for N = 2 … no gain"); the gain lives in the singlet/triplet eigenbasis. The README offers this paragraph as the whole key idea, from which a model can rederive the refutation of ETH. Measured in §11.

### 2.2 OAI: deform, separate, interpolate back

1. Each variable occurrence is replaced by a linear form depending on a parameter T, equal to the original variable at T = 0 (OAI §2.3).
2. At chosen points T = 1 … Q, each clause depends only on its own group plus a few shared integer "measurements"; enumerating measurement values and per-group assignments evaluates the sum there (§2.4).
3. The value at T = 0 is recovered by Hermite interpolation from Taylor coefficients at those points (§2.5).
4. Keeping the deformation's degree R much smaller than Q uses commuting swap permutations and binomial concentration (§2.7, §4).
5. General formulas use sparsification, expander-code parity tests, and the Beigel–Tarui polynomial for counting mod 2^(n+1) (§5, §6).
6. Appendix D gets the sharper bound by Boolean tensor contraction with 2-adic tapered Hermite recovery.

### 2.3 Side by side

|  | ANT | OAI |
| --- | --- | --- |
| Core move | Exact identity splits crossings; each term picks its own merge direction | One-parameter deformation; evaluate at easy points; interpolate to T = 0 |
| Key algebra | Octonions (eight-square identity), or per bit the singlet projector *(rev. 10-27)*; Heisenberg operators | Low-degree polynomials in commuting permutations; Beigel–Tarui |
| Arithmetic | Exact integers | Exact rationals and residues mod 2^p |
| Bound | 2^O(min{km/√log m, k²n log log n/√log n}) | 2^O(n log log n/√log n), or 2^O(n/√log n) at fixed density |
| Portability | Local gadget; slots into tensor contraction | Heavier machinery; likelier to feed theory first |
| Lean statement *(rev. 10-27)* | `MainResults.lean`, 182 lines, Mathlib only, on Mathlib's `Turing.FinTM2` | `SqrtLog/StatementCanonical.lean` on its own `PrimitiveMachine` (multi-tape, {0, 1, blank}, one cell per step; about 30 lines in `Computation.lean`) |
| Lean proof *(rev. 10-27)* | 1,047 files, about 152k lines; Comparator with the `nanoda` kernel on | 3,977 files, about 710k lines; Comparator config has `nanoda` off |

Both papers state Lean checks of their main theorems, and both note that AI models wrote and reviewed the formalizations. Treat the results as strongly supported but not yet human-reviewed. *(rev. 10-27)* What a machine cannot check is whether each statement file says what the paper says; both are short enough for a person to read, and that reading is the highest-value review either result can get. An independent group led by Impagliazzo, Paturi and Williams is examining OAI (`../math/history.md`, Oct 26).

### 2.4 Lineage *(rev. 10-27)*

Where ANT's move sits among older ideas (a conversation with Fran, 2026-10-27):

- **Kron, tensor analysis of networks (1939) and diakoptics.** Treat a circuit as a tensor with a connection tensor between reference frames; *tear* it along a few branches, solve the pieces, reconnect with a correction whose size depends only on the torn interface (in modern terms a low-rank, Woodbury-type update). The shape is ANT's: cut at the expensive interface, pay only for the interface, and choose the frame in which the interface is cheap.
- **What changes for counting.** Kron's algebra is linear: tearing k links costs a rank-k correction, polynomial. Counting is multilinear: cutting k indices means summing over all 2^k values on the cut. That is *slicing* in tensor-network practice (the T5 follow-on "min-fill and slicing").
- **ANT = slicing with a better basis on the cut.** A crossing does not need full rank across the cut if expanded in the right terms (eight-square identity; SWAP = I − 2·singlet per bit), so each cut bit costs less than a factor 2: 16 → 12 per bit pair, ρ = 11/12, then 2^o(n) by recursion.
- **Closest relatives.** Valiant's holographic algorithms (2004; Cai and others): change the basis on every edge so the problem becomes a planar perfect-matching count (FKT). "Change basis so the hard part factors" and "planarity is what is cheap" are both ANT's themes. The vertex model's turning numbers (5) are a small cousin: local edge phases turn the global loop count into a product of local weights.
- **The Kauffman bracket as a model of both papers.** Its skein relation (crossing = A·smoothing + A⁻¹·smoothing) is a crossing decomposition into planar pieces, ANT's move; the state sum expands every crossing, the vertex model expands none and contracts, and ANT sits between. Computing it by evaluation at chosen points and interpolating back is OAI's move. Phase 1 built both halves exactly, with an answer key at every step.

## 3. Target domains

The techniques apply wherever a problem is a large sum of products over structured variables and the cost is set by cut width or treewidth. Rows are ordered by how directly the ideas plug in; the fit column is a judgment, not a result from either paper.

| Domain | Example algorithms and tools | Fit |
| --- | --- | --- |
| Tensor-network contraction | Quantum-circuit simulation (cotengra-style path search, slicing); Ising/Potts partition functions; LDPC weight enumerators and ML decoding | Direct: same cut and slicing vocabulary as ANT |
| Model counting | #SAT and weighted model counting (sharpSAT, d4, ADDMC, DPMC, TensorOrder); probabilistic programming (ProbLog, Dice); reliability analysis; quantitative information flow | Direct |
| Exact probabilistic inference | Variable elimination, junction tree, Bayesian-network and MRF marginals; pedigree likelihood (Lander–Green, Elston–Stewart) | Strong: ETH-based optimality of treewidth DP is gone |
| Database queries | Counting query answers; functional aggregate queries (FAQ); Marx's ETH-based submodular-width hardness | Theoretical opening; practical engines differ |
| Combinatorial counting | Chromatic and Tutte polynomials, network reliability, independent sets, matchings, colorings, the permanent | Strong; OAI already gets subexponential colorings and matchings |
| Parameterized algorithms | Treewidth DP (Dominating Set, Steiner Tree, Hamiltonian Cycle); 2^O(√n) planar-separator algorithms | Worth revisiting; many were ETH-optimal |
| Algebraic complexity | Matrix-multiplication tensor rank; sum-of-squares (Hurwitz) problems; decomposition search | Design principle from ANT Appendix H |
| Cryptanalysis | Goldreich-style local one-way functions | Narrow (OAI Cor. C.2); deployed crypto unaffected |

**Where the techniques do not help**

1. **Optimization in max-plus form.** The identities need subtraction, which max-plus arithmetic lacks. Viterbi, shortest paths, MAP inference and side-chain packing must first be encoded as counting (weights as exponents of a polynomial), at a cost.
2. **Low-treewidth structure.** HMMs, phylogenetic trees and other near-tree problems are already cheap; there is nothing to save.
3. **Orthogonal-Vectors-based bounds.** Lower bounds for edit distance and longest common subsequence mostly also follow from the OV conjecture, which neither paper refutes. *(rev. 10-27)* The other two pillars of fine-grained complexity fell the same month: AVW gives 3SUM in O(n^1.9992) and APSP in O(n^2.99942) on a word RAM (Lean statements in `../formal-math/3sum-apsp/EndStatement.lean`). So lower bounds resting on 3SUM or APSP alone are also gone; OV is the hypothesis left standing.
4. **Deployed cryptography.** AES, RSA, elliptic curves and lattice schemes never rested on ETH.

## 4. Companion techniques

Six techniques share the papers' grain: exact identities, signed cancellation, factorization across cuts, and bit-level group structure. They are ordered by expected payoff; all are hypotheses to test, not established results.

| # | Technique | Pairs with | Expected payoff | Risk |
| --- | --- | --- | --- | --- |
| 4.1 | Walsh–Hadamard transform, including an octonion-twisted variant | ANT's parity hyperedge \[x ⊕ y ⊕ u ⊕ v = 0\]; the octonions are a twisted group algebra of Z₂³ | Local group sums in O(N log N) instead of O(N²); branch-free XOR/add/subtract inner loops | *(rev. 10-27)* Associativity is not needed: σ acts block by block, so the transform on 2^(3t) entries is a Kronecker product of t 8×8 matrices and is fast by that structure alone (FTS shows even the axis-by-axis algorithm is not optimal, but only existentially). With the singlet route (4.7) the twisted variant may not be needed at all |
| 4.2 | Wrapping arithmetic mod 2^64, and multimodular CRT | Both papers' exact arithmetic; OAI works mod 2^(n+1) natively | Exact signed cancellation at machine speed; C89 unsigned overflow is defined | Counts above 64 bits need several primes plus reconstruction. *(rev. 10-27)* Incompatible with crossing decompositions as stated: they compute 2^(t\|R\|)·Z (or 2^λ·\[crossing\] per singlet expansion) and divide at the end, and mod 2^64 that division destroys the low bits. Mod 2^64 serves plain contraction (C1–C4); C5 needs odd primes, or t\|R\| extra bits of 2-adic precision |
| 4.3 | Holographic algorithms and planar Pfaffians (FKT) | ANT crossing gadgets; precedent Curticapean–Xia | Handle a few crossings in near-planar graphs cheaper than the 4^g Pfaffians of genus g | Most speculative; needs a Pfaffian |
| 4.4 | Component caching and hash-consing across terms | ANT's (t+1)^\|R\| expansion terms, which share most of their structure | Large practical cut in distinct terms, as in sharpSAT's component cache | Hashing overhead; unclear hit rates |
| 4.5 | Contraction-order search extended to choose decompositions | ANT's single row/column choice per term | A finer optimizer picks which crossings to expand and how each term contracts | Search cost; needs good heuristics |
| 4.6 | Monte Carlo over expansion terms | ANT's term sum | Sample terms instead of summing all | The sign problem from quantum Monte Carlo; long shot |
| 4.7 *(rev. 10-27)* | Block-sparse tensors with sector labels | The one-bit form (2.1, item 9): after a per-bit-pair change to the singlet/triplet basis, each piece A\_i is block-diagonal by the number of singlet positions | The rank saving becomes real index-space saving; this is how DMRG and ITensor handle conserved quantum numbers | Sector bookkeeping overhead; the sectors are Hamming-weight classes, not products of bits, so C1 cannot stay purely dense-binary |

**Main bet:** 4.1 + 4.2 + 4.4 together, inside a small C89 contraction engine, are the cheapest path to a measured answer (see 8). *(rev. 10-27)* Revised: 4.7 + 4.4 with multimodular arithmetic (4.2, odd primes) for the decomposition experiments; 4.1 only if the eight-square route is pursued as a comparison.

## 5. Existing rhubarb libraries

rhubarb already has an exact-arithmetic stack that covers the reference path and most of the reconstruction path. Surveyed from `include/` on 2026-10-27.

| Library | What it provides | Role in this program |
| --- | --- | --- |
| `congruentia` | Z/n for n < 2^32, 64-bit products, inverses, deterministic Miller–Rabin, prime search, CRT (`congruentia_restitue`) | Hot-path arithmetic; multimodular reconstruction |
| `magnus` | Exact big integers, s64 fast path, Euclidean division | Reconstructed counts; reference values |
| `fractio` | Exact rationals in canonical form | Interpolation weights on the reference path |
| `polynomium` | Exact Laurent polynomials Z\[t, t⁻¹\] | Knot polynomials, generating functions. *(rev. 10-27)* Has evaluation (`polynomium_valor`) but no interpolation |
| `anulus` | Ring vtable: Z, Q, Z\[t, t⁻¹\], Z/n (`anulus_residuorum`) | Ring-generic reference algorithms |
| `matrix` | Exact matrices over any `anulus`: Bareiss determinant, rank, nullspace, Hermite and Smith forms | Interpolation systems; FKT determinants; *(rev. 10-27)* exact ranks of decomposition pieces (§11) |
| `cyclotomia` | Exact Z\[ζₙ\] with big-integer coefficients | Knot invariants at roots of unity (knotapel uses ζ₈) |
| `quaternio` | Quaternions over any commutative ring | Template for an octonion sibling |
| `reticulum` | Exact Gram–Schmidt, LLL, short vectors | Not needed initially |
| `sors` | Seeded deterministic PRNG (PCG32, SplitMix64) | Reproducible random instances and fuzzing |
| `mensura` | Nested measurements bound to runs, machine and commit | Benchmark recording across phases |
| `laqueus` | Exact polygonal knots in Q³; Kauffman bracket, Jones, Alexander. Bracket by 2^c state sum, capped at 24 crossings | First consumer and first oracle. *(rev. 10-27)* Its PD readers already build a rotation system to check planarity (V − E + F = 2, `include/laqueus.h:229-236`), so faces of the diagram are available to the vertex-model builder (C9) |
| `tabula_nodorum` | 12,966 prime knots up to 13 crossings (KnotInfo PD codes), Jones and Alexander computed via `laqueus` and matched to KnotInfo | Correctness oracle |
| `plexus` | Exact braid groups B\_n | Braid-closure test networks |

**Why knot invariants come first.** The Kauffman bracket is a tensor-network contraction: each crossing is a small tensor and the diagram is the network. Knot diagrams are planar, so a good contraction order drops the cost from 2^c to roughly 2^O(√c). Correct answers are already tabulated, and breaking the 24-crossing cap pays off for knotapel whatever the research outcome.

*(rev. 10-27)* **The bracket is not local as a state sum.** Each crossing's two smoothings are local, but the factor d^(number of loops) counts loops globally, so "each crossing is a small tensor" needs one of two formulations:

1. **Vertex model (recommended).** Binary index per arc, the U_q(sl₂) R-matrix at each crossing, and a turning factor (−A²)^(±1/4 per quarter turn) along each arc. Every smoothing loop is a simple closed curve with turning number ±1, so summing the label over each loop gives −A² − A⁻² = d from purely local factors. This needs turning numbers, which a PD code does not carry directly: either derive an orthogonal drawing from the rotation system (Tamassia-style flow: angles of 90° at crossings, face sums of ±360°), or start from braid words (`plexus`), whose closures have a natural height function and give the quantum trace.
2. **Temperley–Lieb contraction.** Indices are crossingless matchings of tangle boundaries, with Laurent-polynomial coefficients; the classical fast method, but not binary-indexed.

Option 1 keeps C1's binary indices, which is ANT's setting, so the same engine serves the knot phase and the decomposition phase.

*(rev. 10-27)* **The skein relation is ANT's move in miniature.** Kauffman's crossing = A·(one smoothing) + A⁻¹·(the other) writes a crossing as a signed sum of two terms, each cheap in a different direction, and the bracket sums over per-crossing choices: the same shape as ANT's row/column choice. In knot diagrams the crossings are vertices of a planar network, so no trick is needed to contract them; G3 asks whether the generalized move (singlet or octonion pieces) buys anything on non-planar graphs.

## 6. New components

Eight components are needed; C1 and C2 are the core, and the rest arrive by phase (see 8). Names are working names, unsealed. *(rev. 10-27)* Nine with C9. General algorithms (C1–C4, C6, C8) belong in the house tier (`include/` + `lib/`), as `magnus` → `fractio` → `polynomium` did; knotapel consumes them.

| ID | Component | Requirements | Phase |
| --- | --- | --- | --- |
| C1 | Contraction core | Dense tensors with labeled binary or small-domain indices; pairwise contraction; slicing over chosen indices; element type = u32 residue mod p (hot) or `anulus` element (reference); deterministic output; peak-intermediate-size accounting. *(rev. 10-27)* Phase 3 adds block-sparse tensors with sector labels (4.7) | 1 (3 for sectors) |
| C2 | Contraction-order finder | Greedy (min size growth) and min-fill heuristics; cost model = peak intermediate size and total multiply-adds; emits a contraction tree consumed by C1; deterministic tie-breaking | 1 |
| C3 | Multimodular driver | Prime selection via `congruentia_primus_infra`; run C1 once per prime; reconstruct with `congruentia_restitue` into `magnus`; stop when reconstruction stabilizes across one extra prime | 1 |
| C4 | Interpolation mod p | Lagrange (and Hermite, if needed) interpolation over Z/p from evaluation points; feeds `polynomium` after CRT. *(rev. 10-27)* `polynomium` has no interpolation, so C4 starts from scratch, probably as a sibling of `polynomium` | 1 |
| C5 | Crossing decomposition | *(rev. 10-27)* First the one-bit singlet pieces of 2.1 item 9 (integer entries, no σ table), realized through C1's sectors; then the eight-square pieces of ANT Lemma 2 (σ table, R and C per block, A\_i coefficients) as a comparison; expand chosen crossings in a network into signed sub-networks, each with its own contraction order; later, the ρ = 1/√2 pieces of ANT Appendix B | 3 |
| C6 | Walsh–Hadamard transform | In-place FWHT over Z/p and mod 2^64; then an octonion-twisted variant using σ (*rev. 10-27*: as a Kronecker product of 8×8 blocks, see 4.1); used inside C1 for parity-structured local sums | 3 |
| C7 | DIMACS CNF reader | Parse standard `.cnf` files into C1 networks (clause tensors plus variable equality trees) for #SAT benchmarks | 2 |
| C8 | Pfaffian | Exact Pfaffian over an `anulus` (sign-correct, not √det) for the FKT route of 4.3 | 4, optional |
| C9 *(rev. 10-27)* | Vertex-model builder | PD code (via `laqueus`'s rotation system) or braid word (`plexus`) → binary-index network: R-matrix per crossing, turning factors per arc; checked against the `laqueus` state sum before anything else | 1 |

**Not needed:** a full ANT or OAI recursion, floating-point paths, or a general hypergraph partitioner in phase 1.

## 7. Architecture

Every result is computed twice, on a fast specialized path and a slow generic one, and accepted only when the two agree exactly.

&#91;embedded content: architecture · hot path and reference path\]

The hot path never touches big integers or polynomials in its inner loop; the reference path never needs to be fast.

1. **Hot path.** C1 kernels are specialized to u32 residues mod a prime p, with `congruentia` operations inlined: no function pointer and no pool allocation per element operation.
2. **Polynomials by evaluation.** For polynomial-valued answers such as the Kauffman bracket in Z\[A, A⁻¹\], evaluate at enough points mod each prime, contract scalars, then interpolate (C4) and reconstruct (C3) into `polynomium`. This is OAI's evaluate-then-interpolate idea used as an engineering tool. *(rev. 10-27)* The vertex model's quarter-turn factor is a fourth root of −A². Work mod primes p ≡ 1 (mod 4), so that i exists, evaluate at points ω ∈ Z/p and set A = i·ω²: then −A² = ω⁴, each quarter turn contributes ω^(±1), and C4 interpolates the result as a Laurent polynomial in ω. With a height function instead (cups and caps, as in braid closures) only half turns occur, each contributing (−A²)^(±1/2) = ±iA.
3. **Reference path.** The same network contracted with `anulus` elements. It is ring-generic and slow, and it is the oracle for the hot path.
4. **Mod 2^64 shortcut.** When the answer fits in 64 bits, one pass with wrapping unsigned arithmetic replaces the multimodular loop. C89 defines unsigned wraparound; a 64-bit `unsigned long` assumes LP64, which holds on macOS arm64 and Linux x86-64. *(rev. 10-27)* Not for decomposed networks: their final division by a power of two needs odd moduli (4.2).
5. **Determinism.** Fixed tie-breaking in C2 and seeded `sors` instances give the same output on every machine and optimization level, matching rhubarb's existing convention for `magnus` and `cyclotomia`.
6. **Measurement.** Every run records peak intermediate size, multiply-adds and wall time through `mensura`, tied to machine and commit.

## 8. Work plan

Four phases, each closed by a gate with an exact pass condition; phase 1 has value on its own even if every later gate fails.

&#91;embedded content: roadmap · 4 phases, 3 gates\]

G3 is the research question. A negative verdict is a valid outcome and ends the program at phase 3 (see 10).

### Phase 1: Core

- [ ] Verify the eight-square identity over all 4096 settings and emit the σ table as C89 data *(rev. 10-27: verified in Python, §11; the C89 table remains)*
- [ ] *(rev. 10-27)* Settle the vertex-model formulation (5): turning numbers from the rotation system, or braid words first
- [ ] Build C1 with u32-mod-p and `anulus` element types
- [ ] Build C2 with greedy and min-fill ordering
- [ ] Build C3 (multimodular driver) and C4 (interpolation mod p)
- [ ] *(rev. 10-27)* Build C9 and check it against the `laqueus` state sum on small knots
- [ ] Re-implement the Kauffman bracket from PD codes on C1–C4
- [ ] Run all 12,966 `tabula_nodorum` knots against existing Jones polynomials
- [ ] Run the single-crossing microbenchmark (see 9)
- [ ] Record timings against the `laqueus` state sum at 12, 16, 20 and 24 crossings

### Phase 2: Scale

- [ ] Generate test knots and braid closures above 24 crossings from `plexus` words
- [ ] Build C7 and check #SAT on random k-CNFs against brute force for n ≤ 30
- [ ] Count proper colorings of K5, K3,3 and the Petersen graph against chromatic polynomials
- [ ] Plot peak intermediate size against crossing count

### Phase 3: Papers' tricks

- [ ] *(rev. 10-27)* Add sector-labelled block-sparse tensors to C1 (4.7)
- [ ] Build C5 with the Lemma 2 pieces; expand bottleneck crossings one level deep *(rev. 10-27: singlet pieces first, Lemma 2 pieces as a comparison)*
- [ ] Build C6 (plain FWHT, then the twisted variant if 10, question 2 resolves)
- [ ] Add component caching across expansion terms (4.4)
- [ ] Run the J1–J2 Ising lattice for growing L: plain slicing versus decomposition *(rev. 10-27: as a control, see 9)*
- [ ] *(rev. 10-27)* Run random 3-regular graphs (#independent sets, Ising) and random 3-SAT at the threshold: plain contraction versus decomposition, against brute force
- [ ] *(rev. 10-27)* Try the ANT Appendix B pieces (ρ = 1/√2) if the singlet pieces show a measurable gain; they are where a gain at real sizes is likeliest
- [ ] Write up the G3 verdict with measurements

### Phase 4: Optional

- [ ] C8 Pfaffian and the FKT route for near-planar graphs (4.3)
- [ ] ANT Appendix B pieces (ρ = 1/√2) *(rev. 10-27: moved to phase 3, conditionally)*
- [ ] Comparison on standard model-counting or partition-function benchmark sets

## 9. Testbeds and oracles

Every testbed has an independent way to check answers; no result counts unless it matches its oracle exactly.

| Testbed | What it tests | Oracle | Phase |
| --- | --- | --- | --- |
| Eight-square identity, all 4096 settings of (x, y, u, v) | σ table and R + C = 2·\[crossing\] | Brute force | 1 |
| *(rev. 10-27)* Singlet pieces for λ = 1 … 6 | Pieces sum to the crossing; row rank × column rank of each piece | Brute force; exact ranks via `matrix` (§11 has λ ≤ 4) | 1 |
| `tabula_nodorum`: 12,966 prime knots up to 13 crossings | C1–C4 end to end on planar networks | Existing Jones and Alexander polynomials (matched to KnotInfo) | 1 |
| Knots and braid closures above 24 crossings (from `plexus` words) | Scaling past the `laqueus` cap | Agreement across primes and orders; invariance under Reidemeister moves via `laqueus` | 2 |
| Random small k-CNFs from `sors` | C7 + C1 on #SAT | Brute-force enumeration for n ≤ 30 | 2 |
| Proper q-colorings of small non-planar graphs (K5, K3,3, Petersen) | Non-planar contraction | Chromatic polynomial by deletion–contraction | 2 |
| Ising model with diagonal couplings (J1–J2) on L × L lattices | Crossing decomposition where crossings are the real bottleneck *(rev. 10-27: they are not; a row transfer matrix cuts through spins, not bonds, and costs 2^L per step with or without the diagonals, so this is a control where decomposition should not win)* | Transfer-matrix computation for small L | 3 |
| *(rev. 10-27)* Random 3-regular graphs, n ≤ 40: #independent sets and Ising partition functions | Decomposition where every balanced cut is crossed by Θ(n) edges, which is ANT's setting | Brute force | 3 |
| *(rev. 10-27)* Random 3-SAT at the satisfiability threshold, n ≤ 30 | Same, on #SAT | Brute force | 3 |
| *(rev. 10-27)* ANT's (K, λ)-clique instance with random node tables | The decomposition on exactly the instance the paper recurses on | Brute force for small K and λ | 3 |
| Standard model-counting and partition-function benchmark sets | Comparison against established tools | Published counts | 4, optional |

**Single-crossing microbenchmark.** Before C5 touches a full network, contract one 2×2 crossing naively and via the R + C split, and report intermediate table sizes. This shows whether the bit savings survive real constants.

*(rev. 10-27)* A single crossing cannot show a gain: the saving appears only when many crossing bits share one cut and the terms are grouped into pieces. Revised microbenchmark: one cut crossed by λ crossing bits, λ = 1 … 30, comparing (a) the naive cost 2^(4λ) against (b) the sum over the λ+1 pieces of each piece's cheaper direction. Measure **total work summed over all pieces**, not peak size per piece: per-piece size always falls while the number of pieces grows. Expected size of the effect: per level, the bound moves from 4λ toward about 3.58λ bits (§11); for the eight-square route at t = 10 (λ = 30) the bound is 2^60 against 11·2^55 ≈ 2^58.5, about 3×.

## 10. Risks, open questions, kill criteria

The main risk is that the papers' savings are real only asymptotically; the phase gates exist to find that out cheaply.

**Risks**

| Risk | Effect | Mitigation |
| --- | --- | --- |
| Savings vanish at real sizes | Per-crossing gain (11/12, or about 0.71 with Appendix B pieces) is eaten by term count and overhead | Single-crossing microbenchmark before any full network |
| Term explosion | (t+1)^\|R\| terms even when polynomial | Expand only bottleneck crossings, one level deep; component caching (4.4) |
| `anulus` dispatch dominates | Function pointer and pool allocation per element operation | Two-tier design (see 7): specialized u32 kernels on the hot path |
| Interpolation precision | Too few points or primes gives a wrong polynomial | Degree bounds from the diagram; one extra prime and point as a stability check |
| Papers contain an error | Background claims could be wrong | Engineering here does not depend on ETH being false; each phase stands on its oracles |
| *(rev. 10-27)* Rank savings need sectors | The singlet pieces save only in the singlet/triplet basis; dense binary tensors see no gain | C1 sectors (4.7) before C5; the microbenchmark measures ranks first, then real tables |
| *(rev. 10-27)* Turning numbers from PD codes | The vertex model needs a planar drawing the PD code does not carry | Braid words first if the orthogonal-drawing route stalls; `laqueus` state sum as oracle throughout |

**Open questions**

1. Does `polynomium` already provide interpolation, or does C4 start from scratch? *(rev. 10-27: answered, from scratch; it has evaluation only)*
2. Does a fast transform exist for the octonion-twisted Z₂³ algebra, given that σ is not associative? *(rev. 10-27: dissolved; the blockwise Kronecker structure is fast without associativity, see 4.1)*
3. What is the measured contraction width of the 13-crossing knots under greedy versus min-fill ordering?
4. Which gives a better pedigree or Bayesian-network testbed later: the J1–J2 lattice or a standard benchmark set?
5. *(rev. 10-27)* Vertex model from an orthogonal drawing of the PD rotation system, or from braid words: which reaches the 12,966-knot check first?
6. *(rev. 10-27)* How large is the sector overhead of block-sparse tensors against the rank saving, at λ ≤ 30?
7. *(rev. 10-27)* Do the Appendix B pieces have an integer, computable factorization of polynomial bit length that C5 can use? ANT says Appendix B constructs them explicitly; this has not been checked here.

**Kill criteria**

- If the single-crossing microbenchmark shows no reduction in peak intermediate size for λ up to 30 bits, drop C5 and C6 and ship C1–C4 as a contraction library for knotapel alone. *(rev. 10-27: read "the revised microbenchmark of 9 shows no reduction in total work summed over pieces")*
- If C1 with C2 does not beat the `laqueus` state sum at 20 crossings, revisit the contraction-order finder before anything else.

## 11. Review notes and AUDIENDA *(rev. 10-27)*

**Measured on 2026-10-27** (Python, brute force, scratch scripts not committed):

| Check | Result |
| --- | --- |
| Eight-square identity, ANT eq. 12, with ANT's φ(p, q) = Σ_{i≤j} p_i q_j + q₁p₂p₃ + p₁q₂p₃ + p₁p₂q₃ | 0 mismatches in 4,096 settings |
| ANT Appendix H one-bit identity \[x = v\]\[y = u\] = \[x = u\]\[y = v\] − \[x ≠ y\](−1)^x \[u ≠ v\](−1)^u | 0 mismatches in 16 settings |
| ANT §2.5 bit accounting: rows-merged plus columns-merged cost of A\_i | 11t for every i (t = 10: 110 against 12t = 120) |
| Singlet pieces: the λ+1 pieces sum to the crossing | Exact for λ = 1, 2, 3, 4 |
| Singlet pieces: largest (row rank)·(column rank) over the pieces, against 12^λ and the crossing's 16^λ | λ=1: 4 / 12 / 16 · λ=2: 49 / 144 / 256 · λ=3: 370 / 1,728 / 4,096 · λ=4: 4,489 / 20,736 / 65,536 |

At λ = 4 the worst piece costs about 3.03 bits per bit in the two directions together, against 4 for the crossing; the asymptotic figure is log₂ 12 ≈ 3.58, so small λ does better than the bound. Ranks were computed in floating point (numpy) on integer matrices; an exact recheck with `matrix` belongs to phase 1.

**Read on 2026-10-27:** ANT §1–2 and Appendices E–H; OAI §1–2; ANT-L `README.md`, `Challenge.lean`, `Solution.lean`, `MainResults.lean` (as printed in ANT Appendix E), `comparator.json`; OAI-L `Computation.lean` (`PrimitiveMachine`) and `SqrtLog/StatementCanonical.lean`, the Comparator config; the headers of `polynomium`, `laqueus`, `congruentia`.

**AUDIENDA (not verified):**

- ANT §3–6 and Appendices A–D (the recursion, balancing, sparsification, the Appendix B pieces) and OAI §3–6 and Appendices A–D: not read in full; nothing here checks either proof.
- Neither Lean development was built; each needs elan and a downloaded Mathlib cache of several GB (ANT-L: about 8 minutes on 24 CPUs, 7 GB peak).
- That `MainResults.lean` and `StatementCanonical.lean` say what the papers say: read once by Claude, not by a person.
- The 12^λ bound beyond λ = 4, and whether the pieces' rank factorizations are computable with integer entries of polynomial size.
- The turning-number construction for the vertex model (5): sketched, not tried. *(Done 10-27: contractio-plan T4, 62a02ef5; Jones of all 12,966 table knots, connected sums to c = 1,606 in review.)*
- That J1–J2 gives no decomposition gain: argued from the transfer matrix, not measured.
- AVW's results and the 3SUM/APSP claim in 3: taken from the README and statement file of `../formal-math/3sum-apsp/`, not from the paper.
- The rest of the OpenAI drop (721 manuscripts, three withdrawn on Oct 7): unread except for overview entries.

**Phase 1 engine measured (2026-10-27).** `tools/uncinus_mensura.c` (CPU time, this Mac; contractio/laqueus at 3dcabca8). Kauffman bracket by vertex model + `contractio` + `interpolatio`, against the 2^c state sum. Every network result below was equal to the state sum where that runs (c ≤ 24).

*I. Against the state sum* (connected sums of table knots; random 5-strand braid closures):

| c | state sum | network | ratio |
| --- | --- | --- | --- |
| 12 | 0.001 s | 0.0002 s | 5× |
| 16 | 0.011 s | 0.0003 s | 40× |
| 20 | 0.22 s | 0.0004 s | 550× |
| 24 | 4.2 s | 0.0009 s | 4,600× |

*II. Greedy width on the table* (all 12,966 knots): mean latitudo 4.0 (c ≤ 7) rising to 5.6 (c = 13), maximum 8 (apex 256). Building the network takes ~0.01 ms per knot.

*III. Connected sums, c = 50 → 800* (width stays 6–8): total 0.004, 0.02, 0.14, 1.1, 8.6 s; contractions counted 255, 800, 2,660, 10,368, 40,651 (≈ c²/16); 2.1 ns per multiplication throughout; arena 10 MB at c = 800; largest coefficient 429 bits at c = 800 (bound 2c = 1,600).

*IV. Random braid closures* (the shape T5 cares about; width grows with strands):

| strands × crossings | latitudo | per point | full bracket |
| --- | --- | --- | --- |
| 4 × 201 | 8 | 0.6 ms | 0.5 s |
| 6 × 201 | 12 | 2.2 ms | 4.8 s |
| 8 × 201 | 16 | 22 ms | 43 s |
| 12 × 201 | 18 | 98 ms | ~3.6 min (projected) |
| 10 × 201 | 20 | 2.1 s | ~77 min (projected) |

Largest coefficient at 201 crossings: 31–39 bits against a bound of 402.

*Kernel with offset tables* (T3 review prototype, same inputs, same answers): 1.3× on sums, 1.6–2.1× on braids up to apex 2^16, only 1.08–1.3× at apex 2^18–2^20, where the kernel is memory-bound.

**Cost model.** time ≈ (points × primes) × multiplications per contraction × ~1.5–2 ns. Points per prime ≈ window/4 ≈ c/2; primes ≈ 2c/31 + 2 from the 2c-bit bound; so ≈ c²/16 contractions, each costing ≈ apex × (a small factor) multiplications.

**Levers, largest first:**
1. *Width (contraction order).* Exponential: apex 2^16 → 2^20 cost 100× per point. Greedy gives ~2n on random n-strand braids. Min-fill or slicing is the T5 follow-on the plan anticipated.
2. *Fewer primes.* The 2c-bit bound is loose by 3.7× at c = 800 (sums) and ~10× on 201-crossing braids. A tighter *provable* bound, or 62-bit primes (two-word residues), cuts the c²/16 factor directly. Stopping when the answer looks stable is excluded by the exactness rule.
3. *Several points per sweep.* At large apex the kernel is memory-bound; reading each element once for B points amortizes the traffic that offset tables cannot.
4. *Offset-table kernel.* 1.1–2×: worth adopting, but it is the smallest lever.

Projection for the original T5 target (12 strands, ~800 crossings, greedy apex 2^26–2^28 per the design review): ~40,000 contractions at minutes each, i.e. weeks of CPU with today's engine. Not attempted. Levers 1–3 together are what would make it hours.

**Lever 2 taken (T5a, 2026-10-27).** The coefficient bound is now Σ|cₖ| ≤ τ(Tait graph) ≤ Π deg (signed Thistlethwaite expansion; contractio-plan §T5a), min over both colours. Same seeds: contractions and primes ~2.1–2.4× fewer (c = 800 sums 8.6 → 4.9 s; braids 8 × 201 43 → 21 s; 10 × 201 projected 77 → 34 min). Remaining slack ~4× on braids (limes ~150 vs ~35 bits used): cancellation, invisible to any counting bound.

**Incident.** The first braid run asked for 4 strands × 50 crossings, which cannot close to one component (an n-cycle needs word length ≡ n − 1 mod 2). The generator retried forever, allocating each time, and reached ~120 GB before macOS killed it; an RSS-based watcher did not see it (compressed pages). Fixed (parity, arena reset per attempt, 1,000-attempt cap), and long runs now go through `tools/custos.sh`, which kills on phys_footprint (compressed included).

## 12. Experiment families *(rev. 10-27)*

The phases of 8 build tools; these are the questions the tools serve. Four families, plus one meta-experiment. Order of work: the phase 1 engine first (every family needs it), the decomposition lab beside it (it needs only `matrix`), then the single-versus-many test on expanders (the G3 verdict), then the grammar experiment.

### 12.1 Does the cancellation survive at human scale?

1. **Finite-size cost curves.** For the singlet, eight-square and Appendix B pieces, exact ranks and real contraction work for λ = 1 … 30. Small λ already beats the asymptotic figure (§11: about 3.0 bits per bit at λ = 4 against 3.58); the λ at which a decomposition starts to pay is the most useful single number this program can produce.
2. **Sign-problem meter.** For each decomposition, Σ\|terms\| against \|result\|: how much of the answer is cancellation. Decides whether 4.6 (sampling terms) is hopeless or plausible. Needs no engine.
3. **One tree versus many.** On expanders (§9), the best single contraction tree against a sum of networks each contracted in its own order. The Austrin–Kaski–Kubjas bound is about the single tree, so this is the most direct empirical test of the papers' central claim.

### 12.2 Better pieces than the papers'

ρ is set by how cheaply a crossing splits; ANT Appendix H shows the eight-square identity was found by searching for such splits, and the one-bit singlet form already beats it under rank counting.

1. **Decomposition lab.** Exact search for signed decompositions of the crossing on small N minimizing (row cost, column cost), with exact ranks over Q and Z/p via `matrix`. The underlying problem (the least M in Hurwitz's sums-of-squares problem for general N) is open; a negative result such as "the singlet form is optimal among decompositions of this shape" is also informative.
2. **Appendix B made concrete.** The Fock-space pieces built exactly for small h; watch the ratio approach 1/√2, and check whether the pieces factor with integer entries of polynomial size (open question 7).

These run as knotapel-style demos with the usual pre-data design review.

### 12.3 What the engine unlocks for knotapel

1. **Exact invariants of large knots.** Braid closures plus the vertex model make exact Jones polynomials at 50–150 crossings routine: coefficient distributions over random braids, growth rates, how often the polynomial degenerates.
2. **Beyond Jones.** Higher-representation R-matrices give colored Jones; sl(N) gives HOMFLY specializations. Same engine with larger index domains; the sectors of 4.7 serve these too.
3. **Roots of unity.** Exact quantum invariants via `cyclotomia` (knotapel already works at ζ₈).

### 12.4 Crossings as a common language: knots, grammar, weights

D134 measured umbra width as open state at a boundary, and non-projective dependency arcs are crossings; Latin hyperbaton is crossing structure that case endings make cheap to resolve. ANT turns a crossing that is expensive both ways into a signed sum of terms each cheap one way, each term choosing its own direction.

1. **Contraction over D131's toy world.** Count analyses as a tensor contraction with crossing arcs decomposed. Question: does the per-term direction choice align with what a reader does? D134's U1 found Latin endings work better as forward keys than as backward lookups; a match would make that asymmetry the grammar's own row/column choice. A mismatch is a clean negative.

### 12.5 Meta-experiment: seeds for ideas

ANT-L's README claims a model at Claude Opus 5.5's level rederives the ETH refutation from a ten-line description. How short can the seed get before the derivation fails, and does the order of its hints matter (context is not commutative)? Cheap to run; Claude is the subject.
