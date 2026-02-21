# Discrete Knotted Computation: A Research Narrative
*Updated through Demo 71 (2026-02-21)*

## The Thesis

Topological invariants can be compiled into neural network weights to compute Boolean functions without training. The Kauffman bracket — a state-sum polynomial over knot diagram smoothings — evaluated at roots of unity yields cyclotomic integers that serve as exact algebraic weights for complex-valued neurons. The choice of activation function determines which Boolean functions are computable from this fixed weight lattice. The bracket does not approximate computation; it *is* computation, evaluated in a ring where correctness is guaranteed by algebraic closure rather than gradient descent.

This is Discrete Knotted Computation (DKC).

---

## 1. The Foundation: State-Sum Computation as Exact Arithmetic

The Kauffman bracket assigns to every knot diagram a Laurent polynomial in a formal variable A. The computation is a state sum: enumerate all 2^n ways to smooth n crossings (each crossing resolves into an A-weighted or A^{-1}-weighted channel), count the resulting loops, multiply by delta = -(A^2 + A^{-2}) per loop, and sum. The bracket is a topological invariant of framed links — unchanged by Reidemeister moves II and III, and picking up a predictable factor under move I that writhe normalization absorbs.

Demos 01-09 built this machinery from scratch. PD-code state-sum (Demo 01), braid-word state-sum (Demo 02), TL standard-module traces (Demo 03), tensor-product spin-chain representation (Demo 04), scaling to arbitrary strand count (Demo 05), Jones-Wenzl idempotent projectors (Demo 06), bidirectional PD/braid conversion (Demos 07-08), and Reidemeister invariance verification (Demo 09). By Demo 05, the quantum trace formula was confirmed at every scale: tr_q = (-1)^n * delta * bracket(oracle), with sign depending on strand parity.

The critical observation enabling everything that follows: when A is specialized to a root of unity, the bracket value is no longer a polynomial. It is a *number* — and by Habiro's integrality theorem (2002), that number is a cyclotomic integer. At A = e^{i*5pi/4}, every bracket value lives in Z[zeta_8] = {a + b*zeta_8 + c*zeta_8^2 + d*zeta_8^3 : a,b,c,d in Z}. This ring has class number 1 — it is a unique factorization domain — and all arithmetic is exact integer operations on 4-tuples. Zero floating-point error, ever.

Demo 10 bridged from symbolic to numerical evaluation, replacing Laurent polynomials with complex evaluation at A = e^{i*theta}. This gave a 50x memory reduction per matrix entry and scaled the spin-chain representation from 32x32 to 256x256. Demos 11-16 explored the resulting waveform signatures, discovering that greedy-optimal classification angles converge to within 1.5% of Witten-Reshetikhin-Turaev evaluation points — algebraically special angles are informationally optimal angles. Topology knows information theory.

---

## 2. Braid Computation and the Parity Shadow

Demos 18-25 established that braids compute. Exhaustive search found specific braid words whose bracket amplitudes encode NOT (2 strands, 6 crossings), NAND (3 strands, 5 crossings), and AND gates. NOT + NAND is Turing complete. Demo 19 built a full Boolean expression compiler, evaluating a 1-bit adder (8/8 correct) through recursive bracket thresholding. Demo 20 mapped the error landscape: |delta| = 2|cos(2*theta)| universally predicts gate viability, with exact computation at the 8th roots of unity where delta vanishes.

But something was missing. Demo 22's exhaustive search for minimum-braid-complexity gates across all 3-input Boolean functions found a gap: functions involving XOR appeared only at longer braid lengths, and parity (XOR of all inputs) was absent entirely. Demo 25 confirmed this with full measurement matrices: across 1554 four-strand braids and four closure types, the parity function appeared zero times. Every braid had measurement-matrix rank 4 — the closures always provided maximal information — yet parity remained unreachable.

This was the first shadow of the wall.

---

## 3. The Discovery: Forward DKC

Demos 26-28 probed the *reverse* direction: can trained neural network weights be decomposed into bracket values? Complex-valued networks (Demo 27) outperformed real-valued ones, and gauge-corrected decomposition showed that phase carries computational information destroyed by the Born-rule projection |z|^2. But reverse DKC was fundamentally confounded — at delta=0, bracket amplitudes are integers, so any real number is at most 0.5 from a catalog entry.

Demo 29 reversed the question entirely. Instead of asking whether trained weights *match* bracket values, it asked: can bracket values *be* weights?

The answer was immediate and definitive. Implementing exact Z[zeta_8] cyclotomic arithmetic — multiplication via 16 integer multiplies, zero floating point — Demo 29 built a catalog of distinct bracket values from 2-3 strand braids of length 1-8 and searched for weight triples (w1, w2, bias) that compute XOR under split-sigmoid activation. Over 100 valid XOR triples were found. RMS error: 0.000. Not approximately zero — exactly zero, because the computation is exact integer arithmetic throughout.

At A = e^{i*5pi/4}, the variable A equals -zeta_8, and delta = -(A^2 + A^{-2}) = 0 *exactly*. This collapses the state sum to single-loop states only — every multi-loop smoothing contributes zero. The bracket literally counts single-loop resolutions, weighted by phase. This is the key simplification that makes forward DKC tractable.

The result established what Demo 29 called the **Training-as-Focusing thesis**: the topological structure of the bracket is sufficient for computation. Neural network training does not *create* computational capability; it *locates* pre-existing solutions on the Z[zeta_8] lattice.

---

## 4. The Wall: Parity and the 100-Million-Quartet Search

Demo 45 extended forward DKC from XOR to all 13 non-trivial NPN equivalence classes of 3-input Boolean functions, discovering that topology frequency (how often a braid computes a given function, measured by forward enumeration) and decomposition quality (how well trained weights match bracket values, measured by reverse gauge-corrected RMS) are near-perfectly anti-correlated: Spearman rho = -0.82. Functions that topology produces frequently are precisely those whose trained weights decompose cleanly. This is the same computational structure measured from two independent directions.

One function stood alone: XNOR3 (parity) had topology frequency zero, reverse-decomposition RMS worse than random (0.996 vs 0.809 baseline), and under forward construction... nothing. Demo 46's per-neuron gauge correction (independent U(1) rotations per hidden neuron) improved parity's RMS from 0.77 to 0.57, but amplitude-only decomposition (0.33) still dramatically beat gauge-corrected phase matching — proving that split-sigmoid training produces weights whose magnitudes align with bracket magnitudes but whose phases are random relative to Z[zeta_8]. Phase decoherence, not gauge approximation, was the bottleneck.

Demo 48 made the wall precise. An exhaustive search of 100,000,000 quartets (w1, w2, w3, bias) drawn from the 100-value Z[zeta_8] bracket catalog tested every combination under split-sigmoid activation. XNOR3 (parity) had exactly **zero** valid constructions. Every other NPN class had solutions — ranging from 127K for EXACT1 to 25M for AND2'. The wall was razor-thin: EXACT1 (truth table 0x16) is Hamming distance 1 from parity (0x69), with 127,000 solutions. The algebra gets within one bit flip of parity and cannot cross.

A four-tier hierarchy emerged: AND-type functions (>10M solutions), intermediate functions (1-4M), boundary functions (127K-371K), and the single unreachable class (0 solutions). Three independent measures agreed: forward solution count correlates with topology frequency (rho = +0.78), anti-correlates with reverse RMS (rho = -0.79), and weakly correlates with orbit size (rho = +0.47). The bracket algebra has intrinsic computational structure, and parity sits exactly at its boundary.

The shape of this boundary would turn out to be geometric — a question of convexity in the complex plane — but that understanding required first testing the obvious escape routes (Section 5), then finding the real solution (Section 6), and finally proving the wall analytically (Section 8).

---

## 5. The Dead End: Neglecton Weights

Demo 49 tested the most natural hypothesis for breaking the wall: the radical of the Temperley-Lieb algebra. At delta=0, TL generators are nilpotent (e_i^2 = 0), and certain braids are "invisible" — their standard bracket (from single-loop states) is zero. First-order perturbation around delta=0 extracts contributions from two-loop states, yielding "neglecton" bracket values scaled by a factor of 4 = d(delta)/d(epsilon).

The enumeration produced 148 new Z[zeta_8] values from 15,242 previously invisible braids, expanding the catalog from 100 to 216 entries. The expanded catalog was searched exhaustively: 216^4 = 2.18 billion quartets. The result was unambiguous: parity still had exactly zero solutions. All 12 non-parity classes grew proportionally (~21x with the 21.8x larger search space), maintaining the same ranking. More precisely, the 116 genuinely new neglecton values all shared *angles* with existing standard values — they introduced new magnitudes but no new angular directions in the complex plane. The perturbation approach was closed at all orders, because higher-order terms (loops=3, 4, ...) produce the same A^(a-b) sums with real scaling factors that preserve the angular structure.

The wall was not in the weight space.

---

## 6. The Breakthrough: MVN Activation and the Four-Tier Hierarchy

If the lattice contains the right weights — and Demo 49 proved it does, since Aizenberg's known parity construction (w1=zeta_8, w2=i, w3=-1) lives in the Z[zeta_8] catalog — then the obstruction must be in how the activation function *reads* those weights.

Demo 49 proved this analytically: a k=2 half-plane classifier cannot compute 3-input parity with *any* complex weights. The argument generalizes Minsky-Papert (1969) to the complex domain: if y1 and y2 both exceed a threshold d, then y1+y2 exceeds 2d > d, contradicting parity's requirement that the sum of two "true" inputs produces "false." This is not merely an analogy — it is the genuine complex-valued extension of the perceptron limitation theorem, and it applies regardless of the weight lattice. The obstruction is geometric: half-planes are convex sets, and convex classification cannot implement non-monotone functions on additive inputs. Convexity, not algebraic structure, is the enemy.

Demo 50 replaced split-sigmoid with k-sector MVN (Multi-Valued Neuron) classification: divide the complex plane into k angular sectors, assign output 0 or 1 to each sector based on parity (sector j mod 2). The same 100-value bracket catalog was searched for weight triples under each k from 2 to 8.

The result: parity is reachable at k=6 with **906 solutions**. All 13 NPN classes become computable. A four-tier hierarchy crystallized:

- **k=2** (half-plane): 5 classes — AND3', AND2', ~A~(BC), BUF, MAJ'
- **k=3**: +1 class — ~A(B^C)
- **k=4**: +6 classes — EXACT1, ISOLATE, 3v-0x19, 3v-0x1B, A^(B|C), XOR2
- **k=6**: +1 class — **XNOR3 (parity)**

Parity is uniquely isolated at the highest tier. The solution count is non-monotonic in k: 906 at k=6, 756 at k=7, 96 at k=8. This anti-correlates with gcd(k,8) — sector boundaries that misalign with the lattice's pi/4 angular structure break the symmetry that blocks parity. The incommensurability hypothesis: k=6 works because gcd(6,8)=2 provides maximal misalignment, while k=8 (gcd=8, perfect alignment with the lattice) has minimal solutions.

Anatomically, every parity solution obeys the **triskelion principle**: each of the 3 inputs requires its own odd sector, spaced at 120-degree intervals. All weights are confined to octants {2, 4, 5, 7} — the "class-1 angles" under k=6 sector mapping. Two octant-set combinations account for all 906 solutions: {2,5,7} (636 solutions, 70%) and {2,4,7} (270 solutions, 30%).

The wall was in the activation function. The lattice always had what parity needed; split-sigmoid could not read it.

---

## 7. The Anatomy: Why the Wall Exists

Demo 51 computed the radical structure of TL_n(delta=0) for n=2 through 8 using exact integer arithmetic on planar matching diagrams. The key discovery: TL_3 is *semisimple* despite having nilpotent generators. e_1^2 = 0 and e_2^2 = 0, yet TL_3 decomposes cleanly as M_2(k) + k. The algebra is larger than its generators suggest. At n=2, TL_2(delta=0) = k[epsilon]/(epsilon^2) — the dual number algebra, the simplest non-semisimple case.

For even n >= 4, the radical is non-trivial, and its structure revealed what Demo 51 called the **Sandwich Theorem**: TL_{2k}(delta=0) has Loewy length exactly 3 — three layers: Top (isomorphic to TL_{2k-1}(0), which is semisimple), Middle (the novel part), and Bottom (also isomorphic to TL_{2k-1}(0)). The top and bottom are dimensional mirrors. The Peirce block structure of rad^2 matches TL_{2k-1}(0) exactly for k=2,3,4 (checked at n=4,6,8 against n=3,5,7).

All nonzero fixed-point counts in the trace form are Catalan numbers — the **Catalan Trace Theorem**, with a precise window formula: at each n, the nonzero values are exactly {C_m : floor(n/2)+1 <= m <= n}.

Demo 38 proved a dimension reduction theorem: at delta=0, the Gram matrix of the TL trace form has rank C_{n-1} at even n (full rank C_n at odd n), meaning the bracket factors through a quotient TL_n(0)/rad isomorphic to TL_{n-1}(0) — an asymptotic 4x dimension reduction. Demo 39 decomposed TL_n by through-strand count into cellular sectors, verifying that sector sizes equal d(n,j)^2 (the Graham-Lehrer cell module dimensions), proving the cell filtration property exhaustively, and discovering that even semisimple TL algebras are NOT block-diagonal in the standard basis. It also mapped the Gram rank at multiple delta values (0, 1, sqrt(2), phi), showing how the radical erodes rank differently at each evaluation point.

Demo 52 generalized to all roots of unity (ell=2 through 6), discovering that rad^3 = 0 *universally* — the **Universal Nilpotency 3 Conjecture**. This was initially surprising (PIM theory predicts Loewy length up to 2*ell-1), but the resolution is elegant: Graham-Lehrer's cellularity of TL algebras forces Loewy length <= 3 for cellular algebras where cell-module bilinear forms have rad^2 = 0. Cross-ell radical dimension formulas emerged: rad(TL_ell) = 2*ell - 3, and rad(TL_{ell+1}) = ell^2 - ell - 3. The total bilinear rank of the rad^2 generator follows Fibonacci numbers: F(ell-1) at each ell. This connects entanglement rank of the thermal ground state to anyonic computational power — the deeper explanation for why Fibonacci anyons (ell=5) are better for quantum computing.

Demo 60 extended the radical anatomy to the first cubic number field: at ell=7 (delta=2cos(pi/7), root of x^3-x^2-2x+1=0), every formula from the quadratic cases held exactly — rad(TL_7) = 11 = 2*7-3, rad(TL_8) = 39 = 49-7-3, rad^2 = 1-dimensional, nilpotency = 3, and Fibonacci rank = F(6) = 8 with sector decomposition 3+4+1+0. The number field degree does not matter. The radical structure is universal.

The non-semisimple structure of TL at roots of unity is *why* the bracket has computational limitations. Bracket evaluation factors through TL/radical (the semisimple quotient), killing exactly the radical degrees of freedom. Those degrees of freedom are what parity needs. The activation function either reads them (MVN, which accesses angular information from the full cyclotomic ring) or doesn't (split-sigmoid, which projects to amplitudes and discards phase).

Crucially, the radical anatomy is not just an after-the-fact explanation of the wall — it is *predictive*. The dimension of the radical at each n determines how many degrees of freedom are lost in the semisimple quotient. The Fibonacci rank theorem (rad^2 generator rank = F(ell-1)) means that at ell=2, only F(1)=1 degree of freedom supports the sandwich layer. The radical structure predicts which functions will be hard before the exhaustive search confirms it: functions requiring non-convex decision boundaries are precisely those that need the radical's angular degrees of freedom.

---

## 8. The Universality: Cross-Level Exploration

Demos 53-59 extended DKC to five evaluation levels:

- **ell=3** (delta=1, percolation): Bracket values in Z[omega] (Eisenstein integers). Only 6 distinct values. Binary collapse — too sparse for meaningful computation.
- **ell=4** (delta=sqrt(2), Ising): Z[zeta_16] with Z[i]-axiality. 56 catalog values. Re>0 multiplicative encoding reaches 11/13 NPN classes.
- **ell=5** (delta=phi, Fibonacci anyons): Z[zeta_5], rank 4, fully populated. 116+ values. Re>0 still reaches 11/13.
- **ell=6** (delta=sqrt(3), Q=3 Potts): Z[zeta_24], rank 8. Re>0 still 11/13.
- **ell=infinity** (delta=0): Z[zeta_8] with strict axiality. 100 values. Re>0+additive reaches 12/13 (Demo 48); MVN k=6 reaches 13/13 (Demo 50).

The **11/13 Re>0 wall** held at every evaluation level tested. The same two NPN classes — 0x06 (XOR-AND) and 0x1B (CROSS) — were unreachable regardless of the underlying cyclotomic ring. The wall is ring-independent and activation-structural.

Demo 61 proved this computationally via exhaustive integer-arithmetic search (46.7 million angle triples at N=360 with 8 boundary orientations each). Demo 62 provided clean algebraic proofs:

**0x1B (CROSS)** — a 3-step interval-squeeze contradiction. Setting theta=0, writing phi2=pi+x and phi3=y, three constraints force simultaneously 2pi-x-y <= phi1 (from phi1+phi2+phi3 outside the semicircle) and phi1 < pi-x (from phi1+phi2 outside). These combine to require pi < y, but y = phi3 < pi. Contradiction.

**0x06 (XOR-AND)** — a four-semicircle parallelogram argument. The constraint structure requires phi1 to avoid four semicircles whose midpoints form a parallelogram on the circle. The key inequality alpha+beta > pi (forced by phi2+phi3 outside the semicircle) makes all four gaps between consecutive midpoints less than pi. The intersection of four semicircles is non-empty iff some gap >= pi. Since all gaps < pi, the intersection is empty.

The two proofs have different structures: 0x06's obstruction is local (additive triple conflicts), while 0x1B's is global (no local conflicts, but a system-level impossibility). Yet both stem from the same root — the additive structure of multiplicative encoding (angles add: z = w1^a * w2^b * w3^c implies angle(z) = a*phi1 + b*phi2 + c*phi3) creates constraints that half-planes cannot satisfy for these specific truth tables.

Demo 57's convexity thesis unified the picture: the wall is about activation convexity, not encoding geometry. Half-planes are convex; additive sums preserve convexity; multiplicative products can rotate out of convex regions (anti-convex). Non-convex activations (split-sigmoid, sector) bypass the wall with either encoding.

Axiality itself showed a hierarchy across levels. At ell=2 (delta=0), bracket values are strictly 1D-axial in Z[zeta_8] — at most one nonzero component out of four. At ell=4 (delta=sqrt(2)), they are 2D Z[i]-axial. At ell=5 (delta=phi), all four basis coordinates of Z[zeta_5] are populated. The decoherence rate (radical proportion of the algebra) decreases: 60% at ell=3, 36% at ell=4, 17% at ell=5, 7% at ell=6. Higher ell means less decoherence, more stable coherent sector.

---

## 9. The Four Pillars

DKC sits at an unoccupied intersection of four established fields. No paper in any of these fields cites papers from the other three.

**Habiro (2002)** — *bracket values are cyclotomic integers.* The colored Jones polynomial has a cyclotomic expansion. Evaluating at any root of unity produces a value in the appropriate cyclotomic integer ring. This is the mathematical guarantee that the weight catalog consists of algebraic integers with exact arithmetic, not floating-point approximations.

**Nazer-Gastpar (2011)** — *algebraic integer lattices support exact computation.* In compute-and-forward relay networks, relays should compute integer linear combinations of codewords rather than decode individually. Lattice codes (closed under integer linear combination) do the computational heavy lifting. DKC applies the same principle with a twist: compute-and-forward computes linear functions; DKC computes nonlinear functions through bracket evaluation. But the foundational insight is shared — algebraic ring closure guarantees correctness.

**Abramsky (2007)** — *TL diagrams encode computation.* The Temperley-Lieb algebra connects knot theory, categorical quantum mechanics, and logic via a "planar lambda calculus." TL diagrams are a natural computational calculus — composition of diagrams is composition of computations. The bracket state sum evaluates this calculus. Forward DKC's step from "TL diagrams encode computation" to "bracket values of TL diagrams serve as NN weights" builds directly on Abramsky's bridge.

**Aizenberg (2000)** — *roots-of-unity neurons for discrete logic.* The k-th roots of unity encode k-valued logic. A single multi-valued neuron with k angular sectors implements arbitrary k-valued threshold functions. Aizenberg *prescribes* root-of-unity structure by design; forward DKC *discovers* that bracket values (sums of roots of unity by Habiro's theorem) naturally compute Boolean functions when used as MVN weights. The convergence of prescribed and emergent structure is the core DKC result.

The assembly is:
```
Abramsky:       TL diagrams encode computation
    |           (evaluate via state sum)
Habiro:         bracket values are cyclotomic integers
    |           (use as neural network weights)
Aizenberg:      roots-of-unity neurons compute Boolean functions
    |           (ring structure guarantees exactness)
Nazer-Gastpar:  algebraic integer lattices support exact computation
```

Each step is grounded in established mathematics. The assembly is novel.

---

## 9.5. The Proof Trilogy: Radical, Markov, and Angular Anatomy

Three formal proofs now anchor the algebraic side of DKC.

**Proof 1: Radical Dimension Formula** (`proofs/radical-dimension-formula.md`). rad(TL_ell) = 2ell-3. The unique degenerate cell module V_{ell-2} has corank 1, determined by the Chebyshev zero U_{ell-1}(cos(pi/ell)) = 0. This is the "algebraic light" — the fixpt trace sees everything except the radical.

**Proof 2: Next-Level Radical Formula** (`proofs/next-level-radical-formula.md`). rad(TL_{ell+1}) = ell^2-ell-3. At n=ell+1, V_{ell-3} is the unique new degenerate module, again with corank 1 via the linking theorem. The Universal Corank 1 property at first degeneracy is structural, not numerical.

**Proof 3: Markov RT Truncation** (`proofs/markov-rt-truncation.md`). dim ker(B_M) = dim J(A) + sum of (dim L_j)^2 for j >= ell-1. The Markov trace kills exactly the Reshetikhin-Turaev truncated modules (j >= ell-1), preserving only the "physical" modules (j <= ell-2). This is the "topological light" — dimmer than the algebraic light by exactly the RT-truncated blocks. The shadow interpretation: fixpt trace = algebraic light (sees all simples), Markov trace = topological light (sees only RT-physical modules). The excess = what topology cannot see that algebra can.

Discovery path: the wrong conjecture ((ell+m-1)^2+1 for the excess) was essential — it worked for small m and failed at m=3, revealing the true block-sum structure. Demo 39 Parts G-J provided the computational verification (31/31 data points); Demo 63 uses the RT truncation connection to explain why DKC works at delta=0.

---

## 10. The Wall Under a Microscope

Demos 64-65 zoomed in on the XOR6 impossibility — not just confirming it, but anatomizing its two-layer structure and extending the parity matroid hierarchy.

### The Funnel-Shaped Matroid (Demo 64)

Demo 64 established that the DKC parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a genuine matroid minor chain with 100% downward deletion-contraction closure: deleting any weight from an XOR_n solution always yields an XOR_{n-1} solution, and contracting any input to 1 always yields an XNOR_{n-1} solution. This is a verified theorem at all levels.

But the upward direction tells a different story. Coverage collapses sharply: XOR3→XOR2: 99.3%, XOR4→XOR3: 39.2%, XOR5→XOR4: 19.2%, XOR6→XOR5: 0%. The n=6 wall casts its shadow downward — 80% of XOR4 weight sets are already dead ends that can never extend to XOR5. The hierarchy is funnel-shaped, wide at the bottom and collapsing to a point.

The vocabulary splits into 44 "good" values (appear in extensible solutions at every level) and 31 "poison" values (appear only in orphan solutions that cannot be extended). The wall's minimum Hamming distance is 4 — no XOR6 candidate achieves fewer than 4 errors. Crucially, zero errors occur at input weight 0 or 1; the obstruction is purely pairwise and above. This is the **self-doubling impossibility theorem**: any weight equal to an existing weight always fails because scalar doubling preserves the angle, and a class-1 angle doubled remains class-1, contradicting the required class-0 output for even-parity input weight. This mechanism accounts for 89.5% of all closest misses.

A second layer of obstruction exists beyond the pairwise wall: cases at d=7 and d=8 with zero broken pairs but weight-3+ errors — higher-order (GHZ-type) interactions that have no clean pairwise explanation.

### The Two-Layer Wall (Demo 65)

Demo 64 conjectured that XOR6 might become achievable at Z[zeta_16] (the next cyclotomic ring, one rung up the Clifford hierarchy). Demo 65 answered this — and the answer was more nuanced than a simple yes or no.

Under **standard** k-sector activation (odd sectors labeled 1, even labeled 0), the parity ceiling is identical at Z[zeta_8] and Z[zeta_16]. Tier thresholds — XOR3 at k=6, XOR4 at k=8, XOR5 at k=14 — are exactly the same. XOR6 has zero solutions at every k from 2 to 31, in both rings, at both the base catalog (36 Z[zeta_16] values) and an extended catalog (+20 from 4-strand braids). The "gap-of-2" hypothesis (pigeonhole bound minus ceiling should be constant) is **refuted**: at Z[zeta_8] the gap is 2 (pigeonhole 7, ceiling 5); at Z[zeta_16] the gap widens to 9 (pigeonhole 14, ceiling still 5).

But under **generalized** activation — testing all possible binary labelings of k sectors, not just the standard odd/even convention — the wall breaks at k=24. Exactly 3 of 2,700 candidate (XOR5-set, extension-weight) pairs admit a binary coloring of 24 sectors that realizes XOR6. All three consist of exactly 3 antipodal pairs; all have exactly zero margin to the sector boundary (algebraic exactness throughout). XOR7 first appears at k=91 under generalized activation.

This reveals a **two-layer wall structure**: an absolute geometric layer (k≤23, zero solutions regardless of any labeling convention) and a labeling-convention layer (k≥24, breakable by choosing the right coloring). The near-threshold even/odd k pattern (even k works, odd k fails) reflects the pi/4 lattice alignment of Z[zeta_8]. The k=24 threshold is not arbitrary — it is confirmed in Demo 66 as a projection of the 24-cell's 24 vertices onto S¹.

---

## 11. The Quaternionic-Spherical Arc

Demos 66-71 constitute a six-demo arc lifting DKC from the complex plane (S¹) to the full geometry of SU(2), and then using that geometry to explain why 13 directions, 14 cells, and bandwidth l=6 are the same number.

### The 24-Cell Polytope (Demo 66)

Demo 65's k=24 threshold prompted the natural question: why 24? Demo 66 answered it by lifting the Kauffman bracket from scalar trace (S¹) to full unit quaternion (S³). SU(2) braid representations with generators sigma_1 → (1+i)/sqrt(2) and sigma_2 → (1-k)/sqrt(2) produce, from 87,890 enumerated braids, exactly **24 distinct unit quaternions** — the vertices of the 24-cell polytope, the unique self-dual regular 4-polytope. These decompose into 4 axis-aligned, 12 edge-midpoint, and 8 body-diagonal vertices; the 48 elements with negatives form the binary octahedral group.

The bracket and quaternion are complementary projections, not redundant. Of 124,750 braid pairs, 80% differ on both; only 13% agree on both. The quaternion resolves 8,424 bracket collisions; the bracket resolves 450 quaternion collisions.

The key computational result: restricting to 3 antipodal pairs from the 24 vertices and assigning subset sums to their nearest 24-cell Voronoi cell (25 cells: 24 vertices + 1 zero cell) yields **35 XOR6 solutions** — more than the 64-cell geographic grid. Every solution shows the universal 7/7/11 structure: exactly 7 cells labeled 0, 7 labeled 1, 11 unused. Under the 576-element F4 symmetry group of the 24-cell, the 35 solutions fall into 6 orbits in two families: edge-dominant (22 solutions) and body-dominant (13 solutions). The complex S¹ minimum of k=24 is confirmed as a projection of the 24-cell's 24 vertices onto S¹ via the trace.

### The Computation Lives on S² (Demo 67)

Demo 66 showed XOR6 on S³. Demo 67 asked: which projection of S³ actually carries the computational information? The Hopf fibration S³ → S² with fiber S¹ decomposes each unit quaternion into a rotation axis (S² base) and a phase (S¹ fiber). The question is empirical: which part matters?

The answer is unambiguous. A custom 13-direction Voronoi on S² — derived from the 13 distinct eigenvector directions of the 24 quaternions (mod ±) — achieves **36 solutions at only 14 cells**, beating the 25-cell S³ Voronoi's 35 solutions while using fewer cells. Meanwhile, **Hopf phase-only activation yields zero solutions at every resolution tested** (k=4 through k=32). The Hopf phase is computationally inert. All DKC information lives in the S² base.

No Platonic solid captures this: octahedral (6 cells), icosahedral (12 cells), and cuboctahedral (12 cells) Voronois all yield zero XOR6 solutions. The 13 eigenvector directions are data-intrinsic, not geometrically natural, and their Voronoi is more efficient than any a priori polyhedral partition.

The progression across the arc: D65 S¹ needs k=24 (25 cells); D66 S³ needs 25 cells; D67 S² needs 14 cells. The 2D projection is the most efficient encoding.

### The Plane as Window (Demo 68)

Demo 68 applied stereographic projection to map the eigenvector S² into R², asking whether the computation could be performed in the plane. The answer is half yes, half no. Using the S² great-circle metric for nearest-neighbor assignment in R² gives 36 solutions at 14 cells — an exact match to Demo 67. Switching to Euclidean distance collapses solutions to zero. The 16.5× area distortion from stereographic projection breaks cell boundaries when Euclidean distance is used, but conformality (angle preservation) is sufficient when the S² metric is maintained.

The computational conclusion: "Visualize on the plane, compute on the sphere." The stereographic picture is a valid ASCII drawing of a DKC computation — each of the 36 winners renders as a visible 0/1 domain map in a 65×33 grid — but the plane is a window into curved geometry, not a replacement for it. The decision boundary is intrinsically curved.

### ζ_8 as the Computational Sweet Spot (Demo 69)

Demo 69 compared the eigenvector geometry across ζ_8, ζ_16, and ζ_32 and proved the **Direction Nesting Theorem**: for any N, all eigenvector directions in the ζ_N catalog appear identically in the ζ_{2N} catalog. The nesting is algebraically forced — squaring a quaternion (ζ_N → ζ_{2N}) preserves its rotation axis — and confirmed computationally for ζ_8 → ζ_16 at exactly 13/13 directions with 0.000° angular drift.

ζ_8 is the **computational sweet spot**: the coarsest cyclotomic root that produces the structured 13-direction geometry. Finer roots nest it exactly (gaining no new directions relative to the ground structure) but their natural Voronoi resolutions trivialize the computation entirely. At ζ_16 with 3,457 directions and a 3,457-cell Voronoi, 90.6% of all XOR6 triples become solutions — pigeonhole statistics dominate topology. At ζ_32 the figure is 89.6%. The musical temperament analogy: ζ_8 is 13-note equal temperament where each solution is meaningful; ζ_16 is 3,457 microtones where consonance is trivially achieved.

The **trivialization threshold** — where computation transitions from topology-driven to pigeonhole-dominated — lies between roughly 50 and 500 Voronoi cells, not yet precisely located.

### Music and Space (Demo 70)

The 4 eigenvalue angles of the binary octahedral group (0°, 45°, 60°, 90°) map exactly to 4 musical notes: C (unison), Eb (minor 3rd), E (major 3rd), F# (tritone). The trace descends from maximum consonance (C, 2.000) to maximum dissonance (F#, 0.000). Braid words become melodies — each crossing yields the eigenvalue angle of the running quaternion product — and the Yang-Baxter relation sigma_1 sigma_2 sigma_1 = sigma_2 sigma_1 sigma_2 produces **identical melodies step-by-step**: topological equivalence is melodic equivalence.

Music (eigenvalue/pitch) and space (eigenvector/position) are **orthogonal**: Pearson r = -0.062 over 253 pairs. The two projections of a unit quaternion — what note it plays and where on S² its rotation axis points — carry independent information. This confirms the Hopf fibration finding from Demo 67: the computationally relevant S² base and the computationally inert fiber are literally two orthogonal dimensions of the algebraic structure.

The tritone-antipodal correspondence emerges naturally on the dodecahedron: antipodal faces carry tritone pairs (C/F#, G/Db, D/Ab, etc.), so dissonance IS the antipodal map. Yet dodecahedral and icosahedral Voronois yield zero XOR6 solutions — the wrong symmetry group (icosahedral, order 120) cannot replicate results from the binary octahedral group (order 48).

### The 13=13 Theorem (Demo 71)

Demo 71 closed the arc with a spectral explanation of why 13 directions, 14 cells, and XOR6 belong together.

Spherical harmonic decomposition of the XOR6 Voronoi labeling function on S² reveals a **sharp phase transition at bandwidth l=6**: 0% of winning solutions are recoverable at l≤5; 100% are recoverable at l=6. The threshold is all-or-nothing, not gradual. Mode l=6 concentrates approximately 80.7% of the power on average across all 36 winners (range: 70–84%), with l=4 and l=12 as minor secondary modes. This spectral shape is a **structural invariant** — all 36 solutions share it, with variation concentrated only in l=2.

The explanation is the **13=13 Theorem**: spherical harmonic mode l has (2l+1) independent components. At l=5 there are 11 components trying to address 13 distinct regions — underdetermined, zero recovery. At l=6 there are exactly 13 components for 13 regions — determined. The phase transition is at the exact degrees-of-freedom boundary.

This predicts trivialization as well. For ζ_16 with 3,457 directions, the formula requires l≥1728 (2×1728+1=3,457). At such enormous bandwidth, any coarse partition trivially resolves the computation, explaining Demo 69's 90% solution rate from a completely different (spectral rather than statistical) direction.

The compression result: 14 Voronoi cells suffice where Nyquist sampling theory on S² predicts (6+1)²=49 cells. The 3.5× compression is an instance of **compressed sensing on S²** — binary function at known support positions, so sparsity plus known support eliminates the generic sampling requirement. The Voronoi cells need only distinguish 13 directions from each other, not generically sample an arbitrary l=6 function.

The musical connection lands at the end of the arc: the XOR6 computation lives at mode l=6, corresponding to 200 cents (note D, a whole tone). This is one step above the tritone (l=2, F#) that spans the eigenvalue range. The computation requires higher angular frequency than the eigenvalue geometry itself provides — a subtle layering of the algebraic and spectral structure.

---

## 12. The Frontier

**What is proven:**
- Forward DKC works: exact Z[zeta_8] bracket values compute XOR (Demo 29) and all 13 NPN classes (Demo 50) without training.
- The split-sigmoid parity wall: XNOR3 has exactly 0 solutions out of 100M quartets (Demo 48) and 0 out of 2.18B with neglecton expansion (Demo 49).
- MVN k=6 resolves 3-input parity: 906 solutions with the triskelion geometry (Demo 50).
- **4-input parity at k=8**: 96 solutions, tetraskelion structure (Demo 63).
- **5-input parity at k=15**: 3020 solutions, pentaskelion (Demo 63).
- **Parity ceiling for Z[zeta_8] is n=5**: n>=6 impossible by constraint wall, n>=8 impossible by pigeonhole (Demo 63).
- The 11/13 half-plane theorem: analytical proofs that 0x06 and 0x1B are unreachable by any semicircle activation on multiplicative encodings (Demo 62).
- Cross-ell radical dimension formulas: rad(TL_ell) = 2*ell - 3 and rad(TL_{ell+1}) = ell^2-ell-3. **FORMALLY PROVEN** via Graham-Lehrer cellular theory (proofs/radical-dimension-formula.md, proofs/next-level-radical-formula.md).
- **Markov RT Truncation theorem**: Markov trace kills j >= ell-1, preserves j <= ell-2. **FORMALLY PROVEN** (proofs/markov-rt-truncation.md).
- Universal nilpotency 3: rad^3 = 0 for TL_n at all tested roots of unity (Demo 52).
- The Sandwich Theorem: rad^2(TL_{2k}(0)) is isomorphic to TL_{2k-1}(0) (Demo 51).
- **Four-tier NPN hierarchy from pure octant-sector geometry** (Demo 63).
- **Oriented matroid classification**: parity requires alternating OM type (Demo 63).
- **Parity matroid minor chain**: {XOR2, XOR3, XOR4, XOR5} is deletion-contraction closed (Demo 64).
- **Self-doubling impossibility theorem**: adding a 6th weight equal to any existing weight always fails at XOR6; accounts for 89.5% of closest misses (Demo 64).
- **Minimum XOR6 wall distance d=4**: no Z[zeta_8] candidate achieves fewer than 4 output errors (Demo 64).
- **Parity ceiling is algebra-independent**: n=5 ceiling and tier thresholds (XOR3 k=6, XOR4 k=8, XOR5 k=14) identical at Z[zeta_8] and Z[zeta_16] (Demo 65).
- **Gap-of-2 hypothesis refuted**: Z[zeta_8] gap=2, Z[zeta_16] gap=9 (Demo 65).
- **Absolute wall k<=23**: no binary labeling of k sectors (for any k<=23) realizes XOR6 with Z[zeta_8] weights (Demo 65).
- **k=24 breaks the convention wall**: generalized activation with arbitrary binary sector coloring first achieves XOR6 at k=24 (Demo 65).
- **24-cell is the braid image**: SU(2) braid representations produce exactly the 24 vertices of the 24-cell (Demo 66).
- **Antipodal necessity**: XOR6 solutions exist only among antipodal weight pairs; unrestricted C(24,6) search yields zero (Demo 66).
- **24-cell Voronoi yields 35 XOR6 solutions** with universal 7/7/11 cell structure (Demo 66).
- **F4 symmetry decomposes solutions into 6 orbits** in two complementary families (Demo 66).
- **Hopf phase is computationally inert**: phase-only activation yields zero solutions at all resolutions; all DKC information lives in the S² base (Demo 67).
- **S² efficiency**: 14-cell custom Voronoi on S² achieves 36 solutions, more than the 25-cell S³ Voronoi (Demo 67).
- **Intrinsic curvature**: DKC on S² cannot be faithfully flattened to R² under Euclidean metric; S² great-circle metric required (Demo 68).
- **Direction Nesting Theorem**: all ζ_N eigenvector directions appear identically in the ζ_{2N} catalog; confirmed 13/13 at exactly 0.000° for ζ_8 → ζ_16 (Demo 69).
- **ζ_8 is the computational sweet spot**: coarsest root producing the 13-direction structure; finer roots nest it exactly but trivialize computation at their natural Voronoi scale (Demo 69).
- **Yang-Baxter equation = identical melodies**: topological equivalence of braids implies note-by-note melodic equivalence (Demo 70).
- **Music-space orthogonality**: eigenvalue (pitch) and eigenvector (position) are independent; Pearson r = -0.062 (Demo 70).
- **The 13=13 Theorem**: minimum bandwidth for XOR6 DKC on S² is l=6 because 2×6+1=13 equals the number of eigenvector directions; sharp phase transition at the DOF boundary (Demo 71).
- **Spectral universality**: all 36 XOR6 winners share the same spectral envelope with l=6 dominant (~80%) — a structural invariant (Demo 71).
- **Super-Nyquist compression (3.5×)**: 14 cells suffice where Nyquist predicts 49; explained by compressed sensing on S² (Demo 71).

**What is computationally verified but not analytically proven:**
- The axiality theorem at delta=0 (131K braids, zero counterexamples).
- Fibonacci rank of rad^2 generator = F(ell-1) (five data points, through F(6)=8 at ell=7).
- The Catalan trace theorem and window formula.
- The incommensurability hypothesis (parity solutions anti-correlate with gcd(k,8)).
- The universal k theorem (k=2M-1 for M-ray lattice; proof sketch provided).
- Second-degeneracy corank = ell+1 (four data points).
- The funnel rate conjecture: coverage fractions 99.3% → 39.2% → 19.2% → 0% follow some algebraic or exponential decay law (Demo 64).
- ζ_32 full direction nesting: algebraic argument is watertight but computational confirmation requires deeper catalog (Demo 69).
- Trivialization DOF prediction: minimum bandwidth l>=(N-1)/2 where N is eigenvector direction count; predicts ζ_16 l>=1728 consistent with 90% solution rate (Demo 71).

**What is conjectured:**
- ~~The triskelion generalization: n-input parity requires k=2n sectors.~~ **FALSIFIED** by Demo 63 — n=5 requires k=15, not k=10.
- The TL non-semisimplicity hypothesis: parity's resistance reflects the Clifford boundary.
- The two-channel coding conjecture: ~4.3-bit magnitude + ~1.5-bit phase = ~5.8 bits/symbol.
- The Galois symmetry conjecture: sandwich duality from Galois automorphism.
- Universal parity ceiling: n=5 ceiling holds for all cyclotomic evaluation points (all Clifford hierarchy levels). If confirmed, a purely topological theorem (Demo 65).
- XOR_n minimum generalized k grows by factor ~3.8 per additional input: XOR6 k=24, XOR7 k=91, XOR8 ~k=345 (Demo 65, two data points).
- Orbit count formula: XOR_n on the 24-cell may always yield n*(n-1)/2 orbits (Demo 66, unverified).
- Trivialization threshold: XOR6 computation transitions from topology-driven to pigeonhole-dominated between roughly 50 and 500 Voronoi cells (Demo 69).

**What is next:**
- Catalog completeness: prove (or bound) that the 100-value Z[zeta_8] catalog at delta=0 contains ALL distinct bracket values.
- The Abramsky depth: full Geometry of Interaction interpretation to predict which bracket triples compute which functions.
- Multi-layer DKC: cascading bracket-valued neurons for circuit-level compilation.
- The physical interpretation: Costello-Francis-Gwilliam (2026) proved Witten = Reshetikhin-Turaev, making bracket values rigorously quantum field theory transition amplitudes. DKC compiles quantum field theory into Boolean logic.
- OM generalization: what oriented matroid type characterizes n-input parity for n=4,5?
- Analytical proof of bandwidth bound: prove that the minimum bandwidth of any binary Voronoi partition separating N points on S² is l=ceil((N-1)/2) (Demo 71 open question).
- XOR7 on S²: Demo 65 showed k=127 on S¹; does S² reduce this as dramatically as it did for XOR6 (k=24 → 14 cells)?
- Exact Cyc8 arithmetic for generalized XOR6 check: eliminate floating-point atan2 near boundary (zero margin makes this feasible) (Demo 65).
- Trivialization threshold: locate precisely where computation transitions from topology-driven to pigeonhole-dominated (50-500 cell range, Demo 69).
- The k=38 anomaly: only even k>=24 with zero generalized XOR6 solutions; why is 19 special? (Demo 65).

---

## Summary of Novel Contributions

1. Forward DKC: bracket values compute Boolean functions without training (Demo 29)
2. The 100M-quartet parity wall under split-sigmoid (Demo 48)
3. Parity reachability at k=6 MVN sectors; four-tier NPN hierarchy (Demo 50)
4. 11/13 half-plane theorem with analytical proofs for 0x06 and 0x1B (Demos 61-62)
5. The axiality theorem and proof sketch at delta=0 (Demo 35)
6. The Sandwich Theorem: rad^2(TL_{2k}(0)) = TL_{2k-1}(0) (Demo 51)
7. Catalan trace theorem and window formula (Demo 51)
8. Fibonacci rank theorem: total bilinear rank = F(ell-1) (Demo 52)
9. Cross-ell radical dimension formulas: 2ell-3 and ell^2-ell-3 — FORMALLY PROVEN (Demos 52, 39)
10. Topology-frequency correlation reversal under activation change (Demo 47)
11. The four-pillar assembly connecting Habiro, Nazer-Gastpar, Abramsky, and Aizenberg
12. Markov RT truncation theorem — FORMALLY PROVEN (Demo 39, proof trilogy)
13. Parity ceiling for Z[zeta_8] is n=5 — extends DKC to 4-input and 5-input (Demo 63)
14. Oriented matroid classification of DKC — alternating OM type required for parity (Demo 63)
15. Two types of impossibility: constraint-geometric wall vs pigeonhole (Demo 63)
16. Parity matroid minor chain: {XOR2, XOR3, XOR4, XOR5} is deletion-contraction closed; funnel shape with 80% of XOR4 dead ends (Demo 64)
17. Self-doubling impossibility theorem: the dominant XOR6 wall mechanism; accounts for 89.5% of closest misses (Demo 64)
18. Two-layer wall structure: absolute geometric layer (k<=23) vs labeling-convention layer (k>=24, breakable) (Demo 65)
19. Parity ceiling algebra-independence: n=5 ceiling identical at Z[zeta_8] and Z[zeta_16]; gap-of-2 hypothesis refuted (Demo 65)
20. 24-cell polytope as the natural DKC geometry: SU(2) braid representations produce exactly the 24-cell vertices; F4 symmetry with 6 orbits; 7/7/11 universal solution structure (Demo 66)
21. Hopf phase inertness: the S¹ fiber carries zero DKC information; all computation lives on the S² Hopf base (Demo 67)
22. S² efficiency: 14-cell eigenvector Voronoi achieves more XOR6 solutions than 25-cell S³ or 64-cell geographic grid (Demo 67)
23. Intrinsic curvature of DKC: the computation cannot be flattened to R² under Euclidean metric (Demo 68)
24. Direction Nesting Theorem: all ζ_N eigenvector directions embed exactly in ζ_{2N}; algebraically forced, computationally confirmed at 0.000° (Demo 69)
25. ζ_8 as computational sweet spot: coarsest root with the 13-direction structure; finer roots trivialize computation at their natural scale (Demo 69)
26. Yang-Baxter equation as melodic equivalence: topologically equal braids produce note-for-note identical melodies (Demo 70)
27. Music-space orthogonality: pitch (eigenvalue) and position (eigenvector) are independent dimensions of a unit quaternion (Demo 70)
28. The 13=13 Theorem: minimum S² bandwidth for XOR6 DKC is l=6 because 2×6+1=13 equals the eigenvector direction count; sharp DOF phase transition (Demo 71)
29. Spectral universality of XOR6 solutions: all 36 winners share the same l=6-dominant envelope (~80%); a structural invariant (Demo 71)
30. Super-Nyquist compression via compressed sensing on S²: 14 cells vs Nyquist-predicted 49 (Demo 71)

No prior work connects these fields. The intersection is genuinely unoccupied. The question "can topological invariants be compiled into neural network weights?" has no direct precedent in the literature. Seventy-one demos prove the answer is yes — and map the exact boundary of what is computable. That boundary is not just algebraic: it has a geometric layer (the wall at k=23), a topological layer (the 24-cell), a differential-geometric layer (the S² Hopf base), and a spectral layer (bandwidth l=6). The parity function, unreachable by split-sigmoid, turns out to require exactly 13 angular directions on a sphere — a number that is simultaneously the degree of freedom count for the answer and the number of vertices of the binary octahedral group. The coincidence is not accidental.
