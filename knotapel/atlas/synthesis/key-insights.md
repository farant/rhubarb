# Key Insights of Discrete Knotted Computation

Ten ideas at progressive depth, distilled from 109 demos.

---

## Insight 1: Knot invariants can compute

**Level 1** (one sentence):
When you evaluate a knot polynomial at a specific number, the result can serve directly as a neural network weight that computes logical functions -- no training required.

**Level 2** (one paragraph):
The Kauffman bracket is a polynomial assigned to every knot or braid diagram by summing over all possible "smoothings" of crossings. When the formal variable A is specialized to a root of unity (specifically A = e^{i*5pi/4}, where the loop factor delta vanishes), the bracket collapses to a single number per braid. These numbers turn out to be cyclotomic integers in the ring Z[zeta_8]. The central discovery of Discrete Knotted Computation (Demo 29, confirmed across 100+ exact solutions) is that triples of such bracket values, used as weights in a complex-valued neuron, compute XOR and other Boolean functions with exactly zero error. No gradient descent, no backpropagation, no training data -- the topological structure of knots *is* a computational structure. Neural network training, when applied, merely locates pre-existing solutions on a lattice that topology already provided.

---

## Insight 2: The weights are exact

**Level 1** (one sentence):
Every weight in this system is an exact integer combination of powers of a root of unity, so the computation has zero floating-point error -- not approximately zero, literally zero.

**Level 2** (one paragraph):
At A = e^{i*5pi/4}, every bracket value lives in Z[zeta_8] = {a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 : a,b,c,d in Z}, a ring of cyclotomic integers with class number 1 (a unique factorization domain). Arithmetic in this ring is 16 integer multiplies per product -- no floats anywhere. This exactness is not a convenience but a necessity: Demo 48's exhaustive search over 100 million quartets requires exact equality testing to certify zero parity solutions, and Demo 61-62's proofs of the 11/13 theorem use pure integer arithmetic. The mathematical guarantee comes from Habiro's integrality theorem (2002): colored Jones polynomials admit cyclotomic expansions, so evaluation at any root of unity produces an algebraic integer. Approximate methods discovered the phenomena; exact methods proved them. The progression from symbolic polynomials (Demos 01-09) to floating-point numerics (Demo 10+) to exact cyclotomic rings (Demo 29+) is the enabling backbone of the entire research program.

---

## Insight 3: Activation geometry determines what is computable

**Level 1** (one sentence):
The same set of knot-derived weights can compute different families of logical functions depending purely on how you read the answer -- the question is not what the weights contain, but what the activation function can see.

**Level 2** (one paragraph):
The Z[zeta_8] bracket catalog of 100 values is fixed. Under split-sigmoid activation (a half-plane classifier), 12 of 13 Boolean function classes are computable, but parity has exactly zero solutions out of 100 million tested (Demo 48). Under k-sector MVN activation at k=6, parity becomes computable with 906 solutions from the same catalog (Demo 50). Under S^2 Voronoi activation, XOR6 (6-input parity) yields 36 solutions; switching to S^1 x S^2 product activation extends this to XOR8 with 6 solutions (Demo 77). This "wall was in the activation, not the lattice" pattern recurs three times across the program: Demo 48 to 50 (parity wall), Demo 76 to 77 (XOR8 wall), and Demo 78 to 79 (XOR10 wall, where the resolution required switching to an infinite group). The lattice always contained what computation needed; each wall turned out to be a limitation in how the activation function projected the algebraic structure onto a binary decision.

---

## Insight 4: There is a hard wall, and we know where it is

**Level 1** (one sentence):
Exactly 11 of 13 families of three-input Boolean functions are computable under any half-plane activation with any weights whatsoever, and the two missing families are blocked by geometry, not algebra.

**Level 2** (one paragraph):
The 11/13 half-plane theorem (Demos 61-62) states that for all half-plane activations on multiplicative encodings, truth tables 0x06 (XOR-AND) and 0x1B (CROSS) are unreachable regardless of the cyclotomic ring, the evaluation level, or the specific weight angles chosen. This was first observed independently at three evaluation levels (ell=4, 5, 6) with three different rings (Z[i], Z[zeta_5], Z[zeta_24]), then proved analytically with zero floating-point. The two unreachable classes fail for different geometric reasons: 0x1B via an interval-squeeze contradiction (a global impossibility where lower and upper bounds cross), and 0x06 via a parallelogram argument (four semicircle gaps on a circle all forced below pi, making their intersection empty). Despite both being "half-plane unreachable," they are obstructed by fundamentally different mechanisms -- a local versus global obstruction -- hinting at a richer theory of activation-geometric impossibility.

---

## Insight 5: Depth is the fundamental resource

**Level 1** (one sentence):
The computational power of a collection of knot-derived weights grows linearly with how many times the braid strands cross over each other -- more complex knots compute harder functions.

**Level 2** (one paragraph):
The linear depth law (Demo 82) states max_xor approximately equals depth + 6, where depth is the number of braid generator multiplications (equivalently, the crossing number of the associated knot diagram). This was established by comparing three 564-entry subsets of the zeta_12 catalog: shallow entries (depths 0-5) reach XOR10, strided entries (maximum vocabulary diversity: 512 directions, 43 angles) also reach XOR10, but deep entries (all from depth 8, with strictly less vocabulary: 476 directions, 19 angles) reach XOR12. Algebraic coherence -- shared intermediate BFS products among entries from the same depth -- beats raw vocabulary. The mechanism (Demo 90) is axis cancellation: deep entries preferentially contain anti-aligned quaternion rotation axes whose sums produce the low angles needed for fine-grained parity separation. The linearity arises from balanced exponentials (Demo 91): BFS closure adds vocabulary at ~2x per round while parity demand grows at 4x per weight, yielding a slope of approximately 1. The logarithmic scaling law (max_xor ~ 0.62 * log_2(catalog) + 4.6) observed in Demo 81 is a corollary, since catalog size grows exponentially with depth.

---

## Insight 6: The algebra is non-semisimple, and that is a feature

**Level 1** (one sentence):
The mathematical structure underlying these knot computations has a "defective" part called the radical that physicists normally throw away -- but that defective part is exactly what makes the computation work.

**Level 2** (one paragraph):
At delta=0, the Temperley-Lieb algebra is non-semisimple: generators are nilpotent (e_i^2 = 0), and the algebra has a nontrivial radical whose dimension follows Catalan number differences (Demo 51). The Sandwich Theorem shows TL_{2k}(0) has Loewy length exactly 3, with top and bottom layers each isomorphic to the previous odd TL algebra -- a novel structural result. This non-semisimplicity is simultaneously the obstacle (the bracket factors through the semisimple quotient TL/rad, losing radical degrees of freedom that parity needs) and the resource (nilpotency at delta=0 means only single-loop states survive, enabling the exact computation). Bracket-null entries -- 37.5% of the zeta_8 catalog, 100% of the deepest stratum -- are invisible to the scalar trace yet computationally indispensable: removing them drops XOR capacity from XOR8 to XOR6, below even the random-removal baseline (Demo 84). The LCFT interpretation identifies these as analogs of logarithmic null states whose Jordan-cell coupling with non-null partners carries the computational content. What TQFT discards as anomalous (framing, null states, non-semisimple structure), DKC computes with.

---

## Insight 7: The computation is intrinsically curved

**Level 1** (one sentence):
The logical computation lives on the surface of a sphere and cannot be flattened to a plane without destroying it -- the curvature is essential, not a representational choice.

**Level 2** (one paragraph):
The quaternionic arc (Demos 66-71) established that DKC's computational geometry is the 2-sphere S^2, not the complex plane or the full 3-sphere. SU(2) braid representations produce exactly 24 unit quaternions (the 24-cell / binary octahedral group, Demo 66). The Hopf fibration S^3 -> S^2 decomposes each quaternion into a rotation axis (S^2 base) and a phase (S^1 fiber). The fiber is computationally inert -- zero XOR6 solutions at all resolutions tested, Pearson r = -0.06 between pitch and spatial direction (Demo 67). All computation lives in the 13 distinct eigenvector directions on S^2. Stereographic projection to R^2 using Euclidean nearest-neighbor distance yields zero solutions; using the S^2 great-circle metric restores all 36 solutions (Demo 68). The 13=13 theorem (Demo 71) explains the direction count: spherical harmonic mode l has 2l+1 components, and 2*6+1 = 13, making l=6 the exact bandwidth threshold with a sharp 0%-to-100% phase transition. The computation cannot exist on a surface with the wrong curvature.

---

## Insight 8: Algebraic structure beats geometric optimization

**Level 1** (one sentence):
The 13 directions on the sphere that make computation work are geometrically terrible -- as poorly distributed as possible -- yet replacing them with better-distributed points destroys 89% of the computational power.

**Level 2** (one paragraph):
The 13 eigenvector directions of the binary octahedral group achieve a spherical t-design level of t=0 -- literally the worst possible score for uniform sampling on the sphere (Demo 72). Stochastic optimization produces a 13-point configuration with 2.6x better geometric distribution. But this geometrically superior configuration yields only 4 XOR6 solutions versus the original 36 -- a 9x reduction. The anti-correlation between sampling quality and computational quality is sharp and fundamental. The computation depends on specific angular relationships among directions dictated by the group algebra, not on how uniformly those directions cover the sphere. Moving any direction by more than about 5 degrees (roughly the angular width of one Voronoi cell) begins to collapse the solution count (Demo 72 robustness tests). This principle recurs throughout the program: the Euclidean metric fails where the S^2 geodesic metric succeeds (Demo 68), vocabulary diversity loses to algebraic coherence (Demo 82), and no Platonic solid Voronoi partition works -- only the data-intrinsic 13-direction partition derived from the braid group itself (Demo 67, 70).

---

## Insight 9: The encoding determines the function

**Level 1** (one sentence):
How you feed input bits into the system determines which logical function it can compute -- one encoding computes all 13 function families, another is structurally locked to computing only parity.

**Level 2** (one paragraph):
Under the 1-weight-per-input (1wpi) encoding, each input bit selects an independent catalog entry as its weight, giving full combinatorial freedom. With appropriate activation (k-sector MVN at k=6), all 13 NPN classes of 3-input Boolean functions become computable, including parity (Demo 50). Under the +/-q paired encoding, each weight contributes +q, -q, or 0 depending on its two input bits. The Parity-Lock Theorem (Demo 92, formally proved) shows this encoding creates a collision: input masks 00...0 and 11...1 always produce identical weight sums (both zero), so any function where f(00...0) differs from f(11...1) -- including AND, OR, MAJ, and all threshold functions -- is structurally impossible under any activation whatsoever. Zero solutions for these functions at all depths tested (0-8). The encoding concentrates all computational power into XOR/XNOR -- the single Boolean function family outside AC^0 (constant-depth circuits). The depth law max_xor ~ depth + 6 describes capacity scaling for this encoding's only achievable function. The complement-blindness theorem (Demo 93) further shows that the combined activation cell maps each input mask m to the same cell as its bitwise complement, so the activation itself is parity-blind at the cell level -- only the phase channel can recover parity.

---

## Insight 10: Non-semisimple extensions are computationally load-bearing

**Level 1** (one sentence):
When you go from two strands to three or more, the mathematical modules that carry extra "radical" structure outperform the clean simple modules -- the messy part does the work.

**Level 2** (one paragraph):
Multi-strand DKC (Demos 98-101) extends the computation from 2-strand braids (scalar bracket values) to 3-, 4-, and 5-strand braids (matrix-valued representations over TL modules). The first 3-strand XOR was achieved via the irreducible Delta_1 module over Z[zeta_8], reaching XOR14=60 (Demo 99). At 4 strands, the non-semisimple module W_{4,2} -- which contains a radical (non-simple) submodule -- achieves XOR14=70, while the 5-strand simple module W_{5,3} caps at XOR12 with XOR14=0 (Demos 100-101). The radical direction carries only abelian (writhe) data by the Barrington-Radical Principle (Demo 102): the braid group acts on the radical as a 1-dimensional character through A^{writhe}, so the radical itself cannot compute parity. Yet the non-semisimple module outperforms the simple one because the radical constrains coefficient growth to strict Fibonacci bounds, maintaining the algebraic coherence that enables DKC. BFS growth rate tracks strand count (~(n-1)x per round, confirmed at n=3,4,5 in Demos 99-103), not module dimension -- growth is a braid group invariant. The sl_d functor thesis predicts continued scaling at higher strand counts, with the decisive test at d=5 (6-strand W_{6,4}) still pending.

---

*Generated 2026-02-26 from atlas sources: briefing.md, narrative.md, connections.md, theorems.md.*
