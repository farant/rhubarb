# Discrete Knotted Computation: A Research Narrative
*Updated through Demo 84 (2026-02-21)*

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

## 12. The Characterization Arc: What S² DKC Actually Is

Demos 66-71 established that DKC lives on S² — that the computation is carried by the eigenvector direction of the quaternion sum, not the complex scalar trace, and that 13 algebraically selected directions on the sphere suffice for XOR6. Demos 72-75 asked the harder question: what does that *mean*, and what kind of mathematical object is it?

### Algebra Beats Geometry: The Anti-Correlation Theorem (Demo 72)

The 13 eigenvector directions that compute XOR6 are beautiful algebraically — they are the distinct rotation axes of the binary octahedral group — but they are geometrically terrible. They fail the spherical t-design property at l=1: their centroid is off-origin, and they only happen to pass at l=2 (to machine precision, likely from a hidden quadrupole symmetry). The first genuine failure relevant to DKC is at l=4 (residual 0.576). They are, formally, a t=0 design — the worst possible for integration.

This provokes the question: what if you replaced these algebraically-selected directions with ones that are better distributed geometrically? Stochastic optimization finds a 13-point configuration with 2.6× better design residual. XOR6 solutions: 4. Against the eigenvectors' 36.

Better geometry destroys the computation. The **Anti-Correlation Theorem** holds: sampling quality and computational quality are anti-correlated for this configuration. This is because DKC is algebraic, not geometric — the 13 directions are selected by the binary octahedral group acting on Z[ζ_8] quaternions, and they work because of specific angular relationships, not uniform distribution. Moving the points to "better" positions breaks the Voronoi partition structure that XOR6 needs. The compressed sensing and DOF mechanisms that explain why 13 points suffice (Demo 71's 2×6+1=13 argument) require those specific 13 points, not just any 13.

Robustness tests confirm the algebraic sharpness: the computation holds through ~5° perturbation (mean 35.8 of 36 solutions) and collapses at 10° (mean 11.5). The placement is sharp to roughly the angular size of a Voronoi cell.

### DKC is Additive, Not Multiplicative: The 82.8% Universal Constant (Demo 73)

The eigenvector Voronoi partition on S² defines a 13-state space. Inputs get accumulated into a running quaternion sum, and each step maps the current state to the next via the Voronoi assignment of the updated sum. Is this a finite automaton?

Almost. Demo 73 shows that under the *additive* formulation (sum weights, read Voronoi cell of sum), 82.8% of cell transitions are deterministic — a specific input bit in a specific current cell always produces the same next cell. The remaining 17.2% arise from non-determinism: the Voronoi cell captures the eigenvector *direction* (the compass) but discards the eigenvalue angle and magnitude (the odometer). Two partial sums can be in the same cell but produce different cells when the same weight is added, because their magnitudes or angles differ.

The **multiplicative** formulation fails entirely: the braid product σ₁^±1·σ₂^±1·...·σ₂^±1 assigns exactly 8 even-parity and 8 odd-parity inputs to *every* final cell. The product cannot compute XOR6 at all. The reason is algebraic: flipping all bits maps M → M⁻¹, which shares the same eigenvector cell as M (antipodal identification), but changes parity. Multiplicative accumulation is blind to parity by symmetry.

What makes this result striking is that 82.8% is a **universal structural constant**: all 36 winning triples have exactly 48/58 deterministic transitions (to the significant digit shown). Zero variation. This follows from the binary octahedral group acting transitively on the 13 directions — the combinatorial collision structure is identical across all triples. The computation is invariant under the symmetry group that defines its weight space.

A 27-cell signed partition (no antipodal identification) increases determinism to 87.1% but still falls short of 100%. Full determinism would require tracking magnitude modulo the weight lattice, which demands infinitely many states — the "odometer reading" that the current architecture deliberately discards. The DKC computation is an almost-automaton: a compass without an odometer.

### Bracket and Cell are Incomparable Invariants: Six Computational Types (Demo 74)

The Kauffman bracket and the DKC Voronoi cell are both braid invariants — functions that are constant on certain equivalence classes of braids. Are they related? Does one refine the other?

Demo 74 proves they are **incomparable**: neither subsumes the other. At the braid level (analyzing 2,000 braids, 1.1 million pairs), the cell separates 97.8% of bracket collisions — pairs with the same bracket value that land in different Voronoi cells. The remaining 2.2% are pairs with the same eigenvector direction but different eigenvalue angle, precisely the information the cell discards. Conversely, the bracket separates 54% of cell collisions. Neither invariant refines the other; they capture orthogonal information.

The combination is strictly finer: (bracket, cell) yields 119 equivalence classes versus 100 for bracket alone and 14 for cell alone — a 19% improvement that represents genuinely new information. The bracket is a trace (collapses to a scalar), and the cell is an eigenvector projection (collapses to a direction); these two operations discard complementary aspects of the full quaternion.

Among the 14 cells, only **6 distinct computational types** exist. When all 36 winning triples vote on which parity label each cell gets, the 14 cells fall into 6 symmetry classes corresponding to the orbits of the binary octahedral group: the two axis orbits (σ₁-axis, σ₂-axis), a third axis, the body-diagonal cells, the edge cells, and the identity. The DKC computation is invariant under permutation within each orbit. 92% of winning triples actively use the cell's extra resolving power — they separate braid pairs that the bracket treats as equal. The computation is exploiting geometric information the algebraic invariant cannot see.

### Angle Subsumes Cell; √n Magnitudes; Multiplicative Channel Blind (Demo 75)

Demo 74 characterized the bracket and cell as incomparable at the level of individual catalog quaternions. Demo 75 turns to *sum* quaternions — the actual objects that appear in DKC computation — and discovers a different structure.

For additive weight sums, the **angle subsumes the cell**: the eigenvalue rotation angle of the sum quaternion perfectly predicts parity for all 36 winning triples. Every angle value maps to exactly one parity bit, with zero ambiguity. The 13-cell Voronoi assignment is a coarsening of the angle assignment; it carries no information beyond what the angle already encodes at computation time. The sum produces exactly 7 distinct eigenvalue angles: 0°, 30°, 35.264° (tetrahedral), 45°, 60°, 65.905°, 90°. Three of these (30°, 35.264°, 65.905°) are **non-canonical** — they do not appear in any individual catalog quaternion, because quaternion addition breaks the 24-cell group closure and produces vectors not in the group.

The magnitude of the sum is algebraically quantized: |S|² is always an integer from {0, 1, 2, 3, 4, 6}. The value √5 is unreachable — it would require an inner product not achievable from the half-integer inner products of the 24-cell ({-1, -½, 0, ½, 1}). The six magnitude values have entropy 1.839 bits, but magnitude alone predicts parity in only 44% of winning triples.

The **multiplicative channel** — the quaternion *product* of all weights rather than their sum — has more total entropy (4.241 bits), covers all 13 Voronoi cells, but produces zero XOR6 solutions. The multiplicative channel is computation-blind because it stays inside the 24-cell group (only 4 canonical angles appear) and symmetric under parity flip (M → M⁻¹ maps to the same eigenvector cell). The non-canonical angles that make additive computation work arise precisely because addition breaks the group structure. Breaking the group is not a flaw — it is the mechanism.

The characterization arc thus resolves: DKC is an additive operation on a quaternionic lattice, exploiting group-breaking in vector sums to produce non-canonical angles that encode parity. The cell is a coarse readout; the angle is the fine readout; neither the bracket nor any single geometric invariant fully captures the computation. It is something new.

---

## 13. The Scaling Arc: Can DKC Scale Beyond 6 Inputs?

The characterization arc settled what XOR6 DKC is. The scaling arc asked whether the structure generalizes. Can you compute XOR8, XOR10, XOR12 with the same machinery? Is the Z[ζ_8] lattice just a special case, or does DKC scale?

### The First Wall: XOR8 = 0 at S² Voronoi (Demo 76)

The most direct approach is to extend the 6-input construction: select 4 weights from the 24-element ζ_8 catalog and search for XOR8 solutions using the same 14-cell S² Voronoi activation that resolved XOR6. Exhaustive search over all C(24,4) = 10,626 quadruples finds zero solutions. The best achievable accuracy is 50% — random chance.

The failure has a clean information-theoretic diagnosis. At 6 inputs, 64 masks distributed among 14 cells is ~4.6 masks per cell — sparse enough for parity-pure cell assignment. At 8 inputs, 256 masks among 14 cells is ~18.3 masks per cell — parity-pure cells are geometrically impossible by pigeonhole. The S² Voronoi at ζ_8 is exhausted. The 14-cell partition does not have the resolution to separate 256 masks.

The algebraic structure also fragments: 3-term weight sums produce 7 discrete eigenvalue angles (algebraic, clean). 4-term sums produce 86 nearly-continuous angles (values like 7.861°, 9.736°), and the √n magnitude quantization collapses from 6 clean values to 28 including non-algebraic floats. The clean structure that enables 6-input DKC loses its discreteness at 8 inputs under this formulation.

But this is where the scaling arc learns its first lesson: a wall hit by one activation function is not a wall.

### Breaking the First Wall: Activation, Not Root of Unity (Demo 77)

Demo 50 showed that parity (3-input XOR) was unreachable under split-sigmoid but computable at k=6 MVN sectors — the wall was the activation, not the lattice. Demo 77 repeats this discovery one level up.

Under **combined Sec(8) × Voronoi activation** — binning the eigenvalue angle into 8 sectors (S¹) and the eigenvector into 14 Voronoi cells (S²), giving 112 total cells — XOR8 is computable at ζ_8 with exactly **6 winning quadruples**. The phase transition between 0 and non-zero solutions occurs between 84 cells (k=6 sectors, 0 winners) and 112 cells (k=8 sectors, 6 winners). The S¹ component is computationally necessary: sector activation alone at k=16 (16 cells) achieves zero XOR8 solutions at 96.9% accuracy; S² Voronoi alone achieves 50%. Both channels are required.

This is not coincidental — Demo 74 proved that bracket (eigenvalue angle) and cell (eigenvector direction) are *incomparable* invariants. The incomparability is computationally load-bearing: XOR8 requires both pieces of information simultaneously, and neither channel can stand in for the other. The combined S¹ × S² activation is the quaternionic generalization of Aizenberg's MVN: reading both the angle and the direction of a unit quaternion rather than just its complex argument.

The 6 winning quadruples share a remarkable structural property: every one contains exactly one **paired quaternion** — a catalog entry that shares the same eigenvector direction as another entry in the tuple but has a different eigenvalue angle. Remove either member of the pair and you get a valid XOR6 triple. Every XOR8 winner is an XOR6 winner plus one "shadow" that extends it by providing additional angle resolution. The paired structure is exact: 12 of the 24 embedded triples from the 6 winning quadruples are XOR6 winners (50%).

The conclusion mirrors Demo 50: ζ_8 with S¹ × S² activation supports at least 8-input computation. The root of unity did not impose the wall; the activation did.

### The Second Wall: XOR10 = 0 at ζ_8 (Demo 78)

Does the recursive shadow structure extend to XOR10? Demo 78 searches exhaustively: all C(24,5) = 42,504 quintuples under every tested activation (k=8, 10, 12, 16 sector counts). Zero XOR10 winners at any activation.

The best accuracy is 98.4% — 1008 of 1024 masks correct, 16 masks wrong — but not 100%. The non-monotonicity pattern from Demo 50 reappears: k=12 accuracy (96.9%) is worse than k=10 (98.4%), because the pi/4 incommensurability of the ζ_8 lattice creates destructive interference at specific sector counts, just as Demo 50 found 906 solutions at k=6, 756 at k=7, 96 at k=8 for 3-input parity.

The diagnosis: the binary octahedral group has exactly 24 elements, 13 distinct eigenvector directions, and 2 distinct eigenvalue half-angles (45° and 90°). Selecting 5 entries from 24 exhausts the structural diversity of this group. There are not enough distinct algebraic "voices" in the ζ_8 catalog to compute XOR10. The wall this time is in the root of unity — or more precisely, in the **finiteness** of the group it generates.

### Breaking the Second Wall: ζ_12 and the Infinite Group (Demo 79)

At ζ_8, the SU(2) subgroup generated by the standard two-generator braid construction closes at 24 elements after 4 rounds. At ζ_12 (rotation angle π/6), the same construction generates a group that roughly doubles each round: 5 → 17 → 51 → 127 → 275 → 564 → 1140 → 2292 → 4096+. It has not closed by 4096 entries. It appears to be infinite (or extremely large finite).

Demo 79 tests XOR capacity at ζ_12. XOR10 goes from 0 (at ζ_8) to **124 winners** at ζ_12 with a 256-entry catalog. XOR12 is solvable: 50+ winners found from an expanded 4096-entry catalog. XOR6 and XOR8 each produce 1024+ winners (search cap hit). The wall that blocked ζ_8 at XOR10 does not exist for ζ_12.

The catalogs are largely disjoint: only 3 of 24 ζ_8 quaternions appear in the ζ_12 catalog (12%), only 2 of 13 directions overlap (15%). The capacity gain is from genuinely new algebraic structure, not from "more of the same." The finite-group ceiling is real and specific to ζ_8; switching roots of unity removes it.

The conceptual lesson is clean: **a finite quaternion group imposes a hard capacity ceiling**. Once the catalog is fixed, combinatorial diversity is exhausted at some XOR(N). An infinite group has no such ceiling — capacity is limited only by catalog size and computational search resources.

### Only ζ_4 and ζ_8 Generate Finite Groups (Demo 80)

Which roots of unity generate finite SU(2) subgroups under this construction? Demo 80 surveys seven cases (N = 4, 6, 8, 10, 12, 16, 20) by running group closure to cap.

The result: **only ζ_4 and ζ_8** close. ζ_4 generates the 4-element binary dihedral group Q_4 (equivalently {±1, ±i} as unit quaternions). ζ_8 generates the 24-element binary octahedral group, ADE type E_7. Every other tested root generates a group that grows past 4096 elements without closing, approaching a dense subgroup of SU(2).

The pattern is not monotone in N. ζ_6 (sitting between the two finite cases) is infinite, despite cos(π/3) = 1/2 being rational. The **power-of-two conjecture** characterizes the finite cases: rotations by π/2^k (k ≥ 1) generate finite groups under perpendicular-axis composition. π/2 and π/4 are symmetries of the cube; π/3 is a symmetry of the hexagonal lattice but two perpendicular hexagonal axes generate an infinite group.

The algebraic diagnosis is decisive: at ζ_8, the quantum dimension [2]_q = q + q^{-1} where q = i gives [2]_q = i + (1/i) = i - i = 0 exactly. The quantum dimension **vanishes**. The finite cases ζ_4 and ζ_8 are the lattice points (singular points) of quantum group parameter space — exactly the regime where Kuperberg's #P-hardness result for the Jones polynomial does *not* apply. Non-lattice roots (all infinite groups) live where the hardness kicks in.

The ADE connection: ζ_8 reaches E_7 (binary octahedral). ζ_4 reaches the D-series (binary dihedral). E_6 (binary tetrahedral, 24 elements) and E_8 (binary icosahedral, 120 elements) are not reachable by the two-perpendicular-axes construction — they require a different generator structure. The Fibonacci anyon parameter ζ_10 generates an infinite group (71 distinct half-angles at 4096 entries), consistent with its use for universal quantum computation via SU(2)-dense braiding.

### The Logarithmic Scaling Law (Demo 81)

For an infinite-group root like ζ_12, how does XOR capacity scale with catalog size? Demo 81 runs the full XOR6→XOR12 search at 9 snapshot rounds as the group closure builds, recording transition points.

The result is a **logarithmic scaling law**: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6. The transition points confirm:

- XOR6 first appears at ~5 entries
- XOR8 at ~51 entries
- XOR10 at ~275 entries
- XOR12 at ~1140 entries

Each doubling of catalog size adds ~0.62 to the maximum computable XOR. Equivalently, each +2 XOR inputs requires approximately 10× more entries (3.2 additional doublings). The scaling is monotone: capacity never decreases as more entries are added.

The two-regime vocabulary structure clarifies what is scaling. In early rounds, directions grow as ~40-45% of catalog size — the group is exploring S² and finding new rotation axes. Direction saturation hits at MAX_DIR = 512 (around round 7). After that, only angle refinement continues. Angular diversity is the limiting factor for high XOR levels: the ζ_12 catalog has 12 distinct half-angles at 256 entries and 43 at 4096 entries.

The gap between successive XOR transitions decreases: 3.35 doublings between XOR8 and XOR6, 2.43 between XOR10 and XOR8, 2.05 between XOR12 and XOR10. This is **possibly sub-logarithmic** (O(log N / log log N) rather than O(log N)) but more data points are needed.

Extrapolating: XOR14 requires ~38,000 entries (~11 closure rounds, feasible). XOR16 requires ~615,000 entries (~14 rounds, feasible in hours). XOR20 requires ~160 million entries (memory-limited). XOR32 requires ~10^14 entries (infeasible). The capacity is real and growing, but the exponential catalog cost per XOR level is entirely consistent with Kuperberg's #P-hardness theorem — the hardness is in generating deep entries, not in using them.

### Depth, Not Catalog Size, Is the Real Variable (Demo 82)

The logarithmic law from Demo 81 raises the question: what determines capacity for a fixed catalog *size*? Not all catalogs of the same size are equivalent. Demo 82 compares three 564-entry subsets:

- **Shallow**: the first 564 entries by closure order (depths 0-5)
- **Strided**: every 7th entry from the full 4096-entry catalog (maximum vocabulary coverage)
- **Deep**: the last 564 entries (all from depth 8, the deepest round)

Results at 564 entries: shallow reaches XOR10, strided reaches XOR10, deep reaches **XOR12**. The deep subset achieves 2 additional XOR levels over equally-sized shallow or strided subsets.

The surprising result is the comparison with strided: the 564 strided entries have maximum possible vocabulary — 512 directions, 43 angles, matching the full 4096-entry catalog. The 564 deep entries have strictly less vocabulary: 476 directions, 19 angles. Yet deep reaches XOR12 and strided only XOR10. **Algebraic coherence beats vocabulary**.

The explanation is the **Linear Depth Law**: max_xor ≈ depth + 6. Each closure round (each unit of crossing depth) adds a fixed increment to computational capacity. A depth-d quaternion encodes d generator multiplications — d crossings in the braid/knot sense. Deep entries share algebraic structure: they are all products of approximately 8 generators, giving them common intermediate factors. Shallow entries from different rounds lack this coherence even when they have equivalent vocabulary.

The logarithmic scaling law from Demo 81 is a corollary of the linear depth law: because catalog size grows as ~2^depth, taking log₂ of an exponential gives a linear function, so max_xor ≈ 0.62 × log₂(catalog) follows from max_xor ≈ depth + const.

Winner anatomy confirms the story: every XOR winner has a **shallow core + deep extensions** structure. Depth-0 entries (the original generators plus identity) appear in every winner at every XOR level. Mean entry depth rises monotonically: 0.52 (XOR6), 0.63 (XOR8), 1.00 (XOR10), 1.98 (XOR12). A sample XOR12 winner has depths [0, 0, 0, 1, 3, 5] — three generators, one shallow element, and two progressively deeper "harmonic" entries.

The depth law provides an interpretation of Kuperberg's #P-hardness: the hardness is precisely in generating deep entries (exponential BFS cost in the crossing number), while the algebraic gain from depth is only linear. The cost to compute is exponential in the depth; the computational power gained is linear. This matches the #P structure exactly.

---

## 14. The Decomposition Arc: What DKC Power Is Made Of

The scaling arc (Demos 76-82) established *how much* computation is possible — a logarithmic law governed by crossing depth. Demos 83 and 84 ask the orthogonal question: *what* are the distinct components of that computational power? The answer is a resource decomposition: three independent, additive axes that together determine capacity.

### Jones Normalization Destroys Exactly 2 XOR Levels (Demo 83)

The Jones polynomial is defined as the Kauffman bracket multiplied by a writhe-dependent phase: Jones(L) = (-A³)^{-w} × bracket(L), where w is the writhe (algebraic crossing number). This normalization removes the "framing anomaly" — the bracket's sensitivity to Reidemeister move I — yielding a genuine link invariant rather than a framed-link invariant.

Demo 83 asked whether that normalization has a computational cost.

The answer is unambiguous: **yes, exactly 2 XOR levels, at every root tested**.

| Root | Bracket capacity | Jones capacity | Loss |
|------|-----------------|----------------|------|
| ζ₁₂  | XOR12           | XOR10          | 2    |
| ζ₈   | XOR8            | XOR6           | 2    |

This resolves a pattern that had been puzzling since Demo 79. The "N-2" observation — that ζ₁₂ seemed to top out at XOR10 in early work — was an artifact of inadvertently computing Jones-normalized quantities. The bracket genuinely reaches XOR12 at ζ₁₂. The Jones polynomial's "maximally degenerate" normalization (at the TQFT point δ=0 where framing anomaly is strongest) removes exactly the angular information that enables the top two XOR levels.

The mechanism is specific: Jones normalization reduces distinct quaternion count by 33.8% (4096 → 2710) and angle count by 28% (43 → 31), but leaves direction count **unchanged** (512 → 512). Framing lives in *how* quaternions rotate, not *where* they point. Angular vocabulary is the limiting resource, and the writhe phase destroys it at the angular dimension.

Writhe and crossing depth are **independent computational axes** (r = 0.14 across 4096 entries at ζ₁₂). Mean absolute writhe grows linearly with depth (0.80 → 3.19 over depths 0–8), confirming that deeper braid words tend to accumulate more writhe — but the low correlation means a deep entry can have low writhe (crossings cancel) or high writhe (crossings accumulate). Chirality structure is not determined by length.

Writhe alone, used as a standalone feature, can compute XOR6: 32 winners found in a brute-force writhe-sum test. But writhe alone cannot reach XOR8 — full power requires writhe interacting with the quaternion lattice. XOR12 winners use both positive and negative writhe entries (chirality variation), with mixed-writhe fractions declining as capacity rises (41% at XOR6, 12% at XOR12).

The **resource decomposition** now reads:

1. **Lattice structure** — base capacity (XOR6 achievable from the lattice alone)
2. **Depth** — +2 XOR levels per closure round (Demo 82: linear depth law)
3. **Writhe/framing** — +2 XOR levels additional (Demo 83)

Three independent axes, each additive, each worth exactly the same increment. The phrase "what TQFT calls an anomaly" is exactly the component that DKC computes with. Kirby (1978) established that framing determines 3-manifold topology; Witten (1989) designated δ=0 as TQFT's maximally degenerate point where framing is first discarded; Rasmussen (2004) confirmed that categorified knot theory (Khovanov homology) does *not* normalize away writhe — the categorified version preserves precisely what the Jones polynomial discards, and what DKC uses for computation.

### Null States and the Logarithmic Structure (Demo 84)

The ζ₈ catalog contains 24 entries. Demo 84 revealed that **9 of them (37.5%) are bracket-null**: their quaternion real part Re(q) = 0, causing the Kauffman bracket trace to vanish. These are not degenerate entries — they are unit quaternions with full directional information, but they are invisible to the scalar bracket invariant. The question was whether they contribute to computation or are dead weight.

The answer is decisive: **bracket-null entries are indispensable**.

The null census shows a monotone depth gradient: 0% of depth-0 entries are null, 20% of depth-1, 75% of depth-2, 100% of depth-3. The deepest stratum of the ζ₈ catalog is entirely composed of bracket-null entries. The |Re(q)| spectrum is discrete — exactly 4 values {0, 0.5, 0.707, 1.0} — with a hard gap of 0.5 between null and nearest non-null. Nullity is a binary property, not a continuum.

The geometric picture is clean. The 13 S² directions across the 24-entry catalog partition into three types:
- **6 null-only directions**: cube-edge midpoints (components ±1/√2), covered only by null entries
- **4 non-null-only directions**: tetrahedral axes (components ±1/√3)
- **3 shared directions**: coordinate axes

Null entries are maximally directionally efficient: each one points to a unique region of S² (1.00 direction per entry). Non-null entries share directions (0.47 per entry). Removing the 9 null entries removes 6 exclusive directions.

**The crux experiment**: splitting the catalog by null status and testing XOR capacity reveals the full picture. The non-null subset (15 entries, 7 directions) retains max_xor=8 but drops from 32 to 4 XOR8 winners — an 87.5% reduction. The null-only subset (9 entries, 9 directions) reaches **zero** XOR capacity at all k_sec values tested, including k=1. Random 15-entry subsets achieve mean max_xor = 7.8; the non-null-only subset at max_xor=6 falls below the random baseline. Removing the specific null entries is strictly worse than removing random entries of equal count.

The k_sec sweep resolves the mechanism. The null-only subset is flat at zero winners across all k_sec (k=1,2,4,6,8) — confirming that all bracket-null entries share the same S¹ coordinate (half-angle = 90°, a single point on S¹) and contribute nothing to S¹ angular discrimination. Their computational contribution is purely S² directional. Conversely, the non-null subset can reach XOR8, but requires k_sec = 16–20 to do so — three times the angular resolution that the full catalog needs (k=6). Null entries are **efficiency amplifiers**: they expand directional vocabulary, reducing the angular resolution cost of XOR discrimination.

The winner anatomy confirms the level-dependence. At XOR6, winners actively avoid null entries (mean null count 0.344 per winner, ratio 0.31 relative to the random expectation of 9/24 ≈ 0.375). At XOR8, winners approach the random expectation (ratio 0.79). Null entries are not needed for the lower level; they become essential for the higher one.

The cross-root comparison establishes a systematic pattern: as the group order grows (ζ₄ → ζ₈ → ζ₁₂), the null fraction decreases sharply (75% → 37.5% → 3%). In the infinite-group limit, the null fraction approaches zero. The ζ₈ regime — the computationally sweet spot established in Demo 69 — is precisely the regime where nulls represent a significant structural fraction. The ζ₄ case (semisimple, δ=2) has 75% null entries but **zero XOR capacity**. Non-semisimplicity IS the computational resource.

**The LCFT interpretation**: In logarithmic conformal field theory, null states |N⟩ satisfy ⟨N|N⟩ = 0 but |N⟩ ≠ 0. They are paired with logarithmic partners |L⟩ via a Jordan-cell action of the Virasoro zero-mode L₀ (non-diagonalizable: L₀|L⟩ = h|L⟩ + |N⟩). The two-point function of |N⟩ vanishes, but the mixed function ⟨N|L⟩ is nonzero — the computational content lives in the Jordan-cell coupling. In the DKC context: bracket-null entries (Re(q) = 0, trace = 0) are the algebraic analog of LCFT null states. The 6 null-only directions (cube-edge midpoints) are entries with no non-null partner in the same direction — no logarithmic partner in the catalog. Removing them does not merely reduce capacity; it collapses it, consistent with the LCFT prediction that removing a null state removes its logarithmic partner's anchor.

The 3 shared directions (coordinate axes, appearing in both null and non-null entries) are candidate Jordan-cell pairs: a null entry on axis X and a non-null entry on axis X may form a literal Jordan-cell pair in some braid group algebra representation. The k_sec=1 synergy result — full catalog achieves 36 XOR6 winners at k_sec=1, non-null achieves 0, null-only achieves 0 — is the precise DKC manifestation of the LCFT relation ⟨T|t⟩ = b: the null state and its partner together produce output that neither generates alone.

The decomposition arc thus adds a fourth component to the resource picture: **non-semisimplicity**, which provides both the null-entry directional infrastructure and the Jordan-block memory mechanism that enables depth scaling. Semisimple algebras (ζ₄) are Markov-like — memoryless — and cannot build deep catalogs or achieve XOR capacity. Non-semisimple algebras carry a radical, and it is the radical that does the computational work.

---

## 15. The Frontier

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
- **Anti-Correlation Theorem**: the 13 eigenvector directions are the worst spherical t-design (t=0, fails at l=1) yet locally optimal for DKC (36 vs 4 solutions for better-distributed directions); sampling quality and computational quality anti-correlate (Demo 72).
- **DKC is additive, not multiplicative**: the multiplicative braid product cannot compute XOR6 at all; complement-all-bits maps M → M⁻¹ sharing the same eigenvector cell, blocking parity structurally (Demo 73).
- **82.8% universal determinism constant**: all 36 winning triples have exactly 48/58 deterministic cell transitions; a structural constant of the ζ_8 DKC system following from binary octahedral group transitivity on the 13 directions (Demo 73).
- **Incomparability Theorem**: the Kauffman bracket and the DKC Voronoi cell are incomparable braid invariants; the combination (bracket, cell) yields 119 classes vs 100 bracket-alone and 14 cell-alone, a strictly finer 19% improvement (Demo 74).
- **6-Type Orbit Theorem**: the DKC computation uses only 6 distinct computational roles among its 14 Voronoi cells, corresponding to the 3 axis orbits, body-diagonal, edge, and identity cells of the binary octahedral group (Demo 74).
- **Bracket-Blind Computation**: 92% of XOR6 winning triples exploit geometric information the bracket cannot see, separating braid pairs the bracket treats as equal (Demo 74).
- **Angle Subsumption**: for all 36 XOR6 winning triples, the eigenvalue rotation angle of the sum quaternion perfectly predicts parity; the Voronoi cell carries no additional information at computation time (Demo 75).
- **√n Magnitude Quantization**: sum quaternion magnitudes are always drawn from {√0, √1, √2, √3, √4, √6}; √5 is unreachable from the 24-cell inner product structure (Demo 75).
- **Multiplicative channel is XOR-blind**: the quaternion product of weights stays inside the 24-cell group (only 4 canonical angles), is symmetric under parity flip, and achieves zero XOR6 solutions despite 4.241 bits of total entropy (Demo 75).
- **XOR8 = 0 at ζ_8 under S² activation**: all C(24,4) = 10,626 quadruples yield 50% accuracy; pigeonhole argument (256 masks / 14 cells ≈ 18.3) proves parity-pure cells are geometrically impossible (Demo 76).
- **XOR8 = 6 at ζ_8 under Sec(8) × Voronoi (S¹ × S²) activation**: the wall was the activation, not the root of unity; same discovery pattern as Demo 50 (Demo 77).
- **S¹ × S² product necessity**: neither sector-only (even at k=16) nor Voronoi-only achieves XOR8; both channels are required, confirmed by the Demo 74 incomparability result (Demo 77).
- **Paired quaternion structure of XOR8 winners**: every XOR8 winning quadruple is an XOR6 triple plus one "shadow" quaternion sharing the same eigenvector direction but differing in eigenvalue angle; removes the shadow and recovers a valid XOR6 triple (Demo 77).
- **XOR10 = 0 at ζ_8 exhaustively**: all C(24,5) = 42,504 quintuples, tested at k=8/10/12/16, yield zero solutions; best accuracy 98.4%; ζ_8 capacity ceiling confirmed at XOR8 (Demo 78).
- **ζ_12 breaks the XOR10 wall**: XOR10 goes from 0 (ζ_8) to 124 winners (ζ_12, 256-entry catalog); XOR12 yields 50+ winners with expanded catalog; the wall was a finite-group ceiling, not a DKC ceiling (Demo 79).
- **Finite group ceiling theorem**: a finite quaternion group imposes a hard capacity ceiling at its combinatorial exhaustion point; an infinite group has no such ceiling (Demo 79, 80).
- **Only ζ_4 and ζ_8 generate finite SU(2) subgroups**: of all tested roots (N=4,6,8,10,12,16,20) the two finite cases are exactly the power-of-two angles (π/2 and π/4); all others generate infinite groups (Demo 80).
- **Quantum dimension vanishing at ζ_8**: [2]_q = q + q^{-1} = i + (-i) = 0 exactly; the two finite-group roots are the lattice (singular) points of quantum group parameter space where Kuperberg #P-hardness does not apply (Demo 80).
- **Logarithmic scaling law**: for ζ_12, max_xor ≈ 0.62 × log₂(catalog_size) + 4.6; each +2 XOR inputs requires ~10× more catalog entries (Demo 81).
- **Direction bottleneck**: each XOR transition is gated by direction count (22 dirs for XOR8, 114 for XOR10, 507 for XOR12); after direction saturation, angular refinement drives growth (Demo 81).
- **Linear depth law**: max_xor ≈ depth + 6; one closure round (one crossing depth unit) adds a fixed increment to XOR capacity regardless of total catalog size (Demo 82).
- **Algebraic coherence beats vocabulary**: 564 deep entries (476 dirs, 19 angles) reach XOR12; 564 strided entries (512 dirs, 43 angles, matching the full 4096-entry vocabulary) reach only XOR10; more vocabulary loses to less vocabulary with more depth (Demo 82).
- **Shallow core + deep extensions architecture**: every XOR winner contains depth-0 generators plus progressively deeper elements; mean winner depth rises monotonically 0.52 → 0.63 → 1.00 → 1.98 across XOR6 → XOR8 → XOR10 → XOR12 (Demo 82).
- **Framing = +2 XOR levels**: Jones normalization (writhe phase removal) costs exactly 2 XOR levels at both ζ₈ and ζ₁₂; bracket XOR8 → Jones XOR6, bracket XOR12 → Jones XOR10; the loss is constant and root-independent (Demo 83).
- **N-2 pattern resolved**: the "N-2" capacity observation from Demo 79 was Jones capacity, not a bracket truncation artifact; the bracket genuinely reaches XOR12 at ζ₁₂ (Demo 83).
- **Framing lives in angles, not directions**: Jones normalization reduces angle count by 28% (43 → 31) but leaves direction count unchanged (512 → 512) at ζ₁₂; angular vocabulary is the mechanism of capacity loss (Demo 83).
- **Writhe and depth are independent axes**: depth-writhe correlation r = 0.14 across 4096 ζ₁₂ entries; mean |writhe| grows linearly with depth (0.80 → 3.19 over depths 0–8) but the low correlation confirms these are genuinely separate resources (Demo 83).
- **Resource decomposition**: DKC computational power decomposes additively as lattice base (XOR6) + depth contribution (+2 per closure round) + writhe/framing contribution (+2); three independent axes, each worth the same increment (Demo 83).
- **TQFT anomaly is computational content**: the writhe phase factor that TQFT treats as a framing anomaly and normalizes away contributes exactly 2 XOR levels; what TQFT discards, DKC computes with (Demo 83).
- **Bracket-null census**: 9 of 24 ζ₈ catalog entries are bracket-null (Re(q) = 0, 37.5%); null fraction increases monotonically with depth: 0% at d=0, 20% at d=1, 75% at d=2, 100% at d=3; every maximally-deep entry is bracket-null (Demo 84).
- **Gap theorem**: the |Re(q)| spectrum of the ζ₈ catalog is discrete with exactly 4 values {0, 0.5, 0.707, 1.0} and a hard gap of 0.5; nullity is binary, not a continuum (Demo 84).
- **Null indispensability**: removing the 9 bracket-null entries from the ζ₈ catalog drops XOR8 winners from 32 to 4 (87.5% reduction); the non-null-only subset performs below random 15-entry controls (mean 7.8 vs 6.0); nulls are disproportionately important (Demo 84).
- **Null directional efficiency**: each bracket-null entry covers exactly 1.00 unique S² direction (cube-edge midpoints, 6 null-only directions); non-null entries cover only 0.47 directions per entry and share axes; nulls are maximally efficient as directional vocabulary (Demo 84).
- **S¹ flatness of null-only**: all bracket-null entries share half-angle = 90°, placing them at a single point on S¹; their entire computational contribution is from S² directional diversity; confirmed by flat k_sec sweep at zero winners across k=1,2,4,6,8 (Demo 84).
- **Null efficiency amplification**: the non-null-only subset can reach XOR8, but requires k_sec = 16–20 (vs k=6 for the full catalog); null entries reduce the angular resolution cost of XOR discrimination by a factor of ~3 (Demo 84).
- **Level-dependent null usage**: XOR6 winners avoid null entries (ratio 0.31 vs random expectation); XOR8 winners approach the random rate (ratio 0.79); null entries are not needed at low capacity but become essential at high capacity (Demo 84).
- **Non-semisimplicity IS the computational resource**: ζ₄ (semisimple, δ=2) has 75% null entries but zero XOR capacity; ζ₈ (non-semisimple, δ=0) achieves full capacity; non-semisimplicity provides both the null-entry directional infrastructure and the Jordan-block memory mechanism (Demo 84).
- **Null fraction dilution**: null fraction decreases as group order grows — ζ₄: 75%, ζ₈: 37.5%, ζ₁₂: 3.0%; in the infinite-group limit the null fraction approaches zero (Demo 84).

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
- The anti-correlation between t-design quality and XOR6 solution count (36 eigenvector vs 4 optimal-geometry); the result is demonstrated but no closed-form bound has been given (Demo 72).
- 82.8% determinism universality follows from binary octahedral group transitivity (argument given but not formalized as a theorem) (Demo 73).
- l=2 vanishes to machine precision in the eigenvector direction set (quadrupole symmetry suspected but not proven algebraically) (Demo 72).
- The power-of-two finiteness conjecture: ζ_32 (θ = π/16) should be finite; not yet tested (Demo 80).
- Logarithmic scaling constant 0.62 appears specific to ζ_12; whether it depends on the root of unity and what determines it has not been derived analytically (Demo 81).
- Sub-logarithmic behavior conjecture: decreasing transition gaps (3.35 → 2.43 → 2.05 doublings) suggest O(log N / log log N) rather than O(log N); requires XOR14 data point to distinguish (Demo 81).

**What is conjectured:**
- ~~The triskelion generalization: n-input parity requires k=2n sectors.~~ **FALSIFIED** by Demo 63 — n=5 requires k=15, not k=10.
- The TL non-semisimplicity hypothesis: parity's resistance reflects the Clifford boundary.
- The two-channel coding conjecture: ~4.3-bit magnitude + ~1.5-bit phase = ~5.8 bits/symbol.
- The Galois symmetry conjecture: sandwich duality from Galois automorphism.
- Universal parity ceiling: n=5 ceiling holds for all cyclotomic evaluation points (all Clifford hierarchy levels). If confirmed, a purely topological theorem (Demo 65).
- XOR_n minimum generalized k grows by factor ~3.8 per additional input: XOR6 k=24, XOR7 k=91, XOR8 ~k=345 (Demo 65, two data points).
- Orbit count formula: XOR_n on the 24-cell may always yield n*(n-1)/2 orbits (Demo 66, unverified).
- Trivialization threshold: XOR6 computation transitions from topology-driven to pigeonhole-dominated between roughly 50 and 500 Voronoi cells (Demo 69).
- Minimum states conjecture: the theoretical minimum for a fully deterministic DKC automaton computing XOR6 is 27 states; the signed 27-cell partition reaches 87.1% but not 100% (Demo 73).
- Recursive shadow conjecture: XOR(n) winners at ζ_8 are built by extending XOR(n-2) winners with a paired quaternion partner sharing the same eigenvector direction; confirmed for XOR6 → XOR8, not yet tested beyond (Demo 77).
- ~~N-2 capacity law: ζ_N supports XOR(N-2).~~ **FALSIFIED** by Demo 79 — ζ_12 supports at least XOR12, well beyond XOR10.
- Union catalog conjecture: a combined ζ_8 + ζ_12 catalog (largely disjoint algebraic structures) might provide superadditive XOR capacity beyond either alone (Demo 79).
- Do all infinite groups give unbounded XOR capacity, or do some infinite groups have structural limitations of their own? (Demo 80, open).
- The 0.62 scaling constant may depend on the root of unity; ζ_10 (71 half-angles at 4096 entries vs ζ_12's 43) may give a different constant (Demo 81).
- Optimal depth for a target XOR level: pure depth-8 is better than shallow or strided at equal size, but whether pure depth-d is optimal vs. mixed depths for each XOR transition is not yet determined (Demo 82).
- Connection to knot complexity: a deeply knotted strand computes more than many shallowly knotted strands with equal vocabulary; whether this connects to known results about torus knots vs. hyperbolic knots is open (Demo 82).
- The +2 framing loss holds at ζ₈ and ζ₁₂; whether it is truly universal across all roots (ζ₁₆, ζ₂₄, etc.) has not been tested; the (-A³)^{-w} rotation acts differently at each root, and the 2-level constancy may be an algebraic coincidence at the tested values or a structural theorem (Demo 83).
- At ζ₈, Jones XOR6 may be exactly the "standard sector activation" capacity from Demo 63 — if 6-sector activation saturating at XOR6 and Jones-normalized ζ₈ saturating at XOR6 are the same regime, this would be a non-trivial structural coincidence (Demo 83).
- The Resource Decomposition Conjecture: DKC power = lattice base (XOR6) + 2 × depth_rounds + 2 × framing_present may be additively exact across all roots and all depth ranges; confirmed at two data points but the cross-term behavior at intermediate depths has not been measured (Demo 83).
- The "+6 constant" in max_xor ≈ depth + 6 is the spectral bandwidth l=6 from the 13=13 Theorem (Demo 71) — the two independent discoveries (D71 spectral, D82 depth) appear unified by one equation, but this connection is inferential, not proven (Demo 84).
- The LCFT Jordan-cell structure may be explicitly constructible for the 3 shared-axis direction pairs in the ζ₈ catalog (one null entry + one non-null entry pointing the same S² direction); if these form literal Jordan-cell pairs in the braid group algebra, the LCFT mapping becomes exact rather than analogical (Demo 84).
- Null entries in the ζ₁₂ catalog: at 3% null fraction and 29 null-only directions, some of those directions may be essential for XOR12 solutions analogously to how 6 null-only directions are essential for XOR8 at ζ₈; not yet tested (Demo 84).
- The depth-3 → 100% null result at ζ₈ may generalize: whether the maximum-depth stratum of any finite SU(2) subgroup consists entirely of bracket-null entries is an open question (Demo 84).
- The Reservoir Computing 5th Pillar: DKC architecture (fixed braid catalog + tunable readout activation) is structurally a fixed reservoir with tunable readout; the radical may serve as the reservoir memory mechanism; Jaeger (2001) memory capacity bounds may formalize the 11/13 theorem (Demo 84).

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
- Analytical proof of why l=2 vanishes exactly in the eigenvector direction set (suspected hidden quadrupole symmetry of the binary octahedral group) (Demo 72).
- Exact 27-state automaton for XOR6: can any partition of S² into 27 regions achieve 100% deterministic transitions, or is magnitude information irreducible? (Demo 73).
- Algebraic characterization of the 97.8% bracket-collision separation: which Z[ζ_8] elements share eigenvector direction but differ in eigenvalue angle? (Demo 74).
- XOR14 at ζ_12: the scaling law predicts ~38,000 entries; directly testable (Demo 81).
- Direct deep-entry generation: can depth-d quaternions be sampled without building all shallower depths? (Demo 82).
- ζ_32 finiteness test: the power-of-two conjecture predicts ζ_32 (θ=π/16) should be finite — a single test_root(32) call confirms or refutes (Demo 80).
- ζ_10 scaling constant: does ζ_10 (71 half-angles at 4096 entries) give a different 0.62 constant, and does it provide the densest XOR capacity per entry? (Demo 81).
- Analytical proof of "+2 per writhe unit": the (-A³)^{-w} factor acts as a rotation by -(3×half_angle + π)×w per entry; the 2-level loss should follow from counting angle equivalence classes before and after normalization; a closed-form bound would confirm universality (Demo 83).
- Framing loss at ζ₁₆ and ζ₂₄: direct tests to confirm or refute the "+2 constant" across additional roots; if the loss is not always 2, the structural reason will reveal the underlying algebraic mechanism (Demo 83).
- Does writhe add exactly +2 regardless of depth? The demo measures the gap at the group's maximum capacity; at intermediate depth levels the bracket vs Jones gap may vary (Demo 83).
- Indecomposability parameter b for DKC: compute the dense polymer analog of the LCFT b parameter (b = ⟨t|G|t⟩ after gauge-fixing ⟨T|G|t⟩ = 1) from finite TL lattice systems; continuum theory predicts b = -2 for c=-2 symplectic fermions; the literature has not yet computed this value (Demo 84, Demo 85 seed).
- Dense polymer fusion calculation: the open Gainutdinov et al. (2013) calculation for n=0 (dense polymers, DKC's operating point) has not been done; performing it would give the first algebraic description of how DKC catalog entries combine under braid composition (Demo 84, Demo 86 seed).
- Jordan-cell pair identification: do the 3 shared-axis (null + non-null) direction pairs in the ζ₈ catalog form literal Jordan-cell pairs in any explicit braid group algebra representation? (Demo 84).
- RC exactification: can the quaternionic DKC framework be used to design reservoir weight matrices where null-state geometry is explicitly controlled, yielding provably optimal XOR capacity per parameter? (Demo 84).

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
31. Anti-Correlation Theorem: sampling quality (t-design) and computational quality (XOR6 solutions) anti-correlate; algebraic placement beats geometric optimality (Demo 72)
32. DKC is fundamentally additive: multiplicative braid product cannot compute XOR6 by symmetry; the computation requires group-breaking through vector addition (Demo 73)
33. 82.8% universal automaton determinism: a structural constant of the ζ_8 DKC system following from binary octahedral group transitivity (Demo 73)
34. Incomparability Theorem: Kauffman bracket and DKC Voronoi cell are incomparable braid invariants; combination yields 19% more equivalence classes than either alone (Demo 74)
35. 6-Type Orbit Theorem: the DKC computation uses only 6 distinct computational roles among 14 cells, corresponding to binary octahedral group orbits (Demo 74)
36. Angle Subsumption: eigenvalue rotation angle perfectly subsumes Voronoi cell at the computation level; seven sum angles, three of them non-canonical (Demo 75)
37. √n magnitude quantization: DKC sum magnitudes are always algebraically constrained to {√0,√1,√2,√3,√4,√6}; √5 unreachable (Demo 75)
38. XOR8 at ζ_8 via S¹ × S² product activation: first proof that the quaternionic generalization of MVN (reading both eigenvalue angle and eigenvector direction) unlocks computation unreachable by either channel alone (Demo 77)
39. Paired quaternion architecture: XOR8 winners = XOR6 triples + one shadow quaternion sharing eigenvector direction but differing in eigenvalue angle (Demo 77)
40. Finite group ceiling theorem: only ζ_4 and ζ_8 generate finite SU(2) subgroups under two-perpendicular-axes construction; all other roots generate infinite groups; finite groups impose hard XOR capacity ceilings (Demos 79, 80)
41. Quantum dimension vanishing at the finite cases: [2]_q = 0 exactly at ζ_8 and ζ_4; finite-group roots are the singular (lattice) points of quantum group parameter space where Kuperberg hardness does not apply (Demo 80)
42. Logarithmic scaling law: max_xor ≈ 0.62 × log₂(catalog_size) + 4.6 for ζ_12; each +2 XOR inputs costs ~10× more entries (Demo 81)
43. Linear depth law: max_xor ≈ depth + 6; crossing depth in the knot-theory sense is the fundamental variable governing XOR capacity, with the logarithmic law a corollary of exponential catalog growth per depth unit (Demo 82)
44. Algebraic coherence dominance: deep entries (fewer directions and angles) outperform stride-sampled entries (full vocabulary) by 2 XOR levels at equal catalog size (Demo 82)
45. Framing as discrete computational resource: Jones normalization costs exactly 2 XOR levels at both ζ₈ and ζ₁₂; the "N-2 pattern" from early demos was the Jones polynomial's capacity all along; what TQFT calls a framing anomaly is what DKC computes with (Demo 83)
46. Resource decomposition of DKC power: three independent additive axes — lattice base (XOR6), crossing depth (+2 per closure round), and writhe/framing (+2) — each contributing the same discrete increment; writhe and depth are confirmed independent (r=0.14) (Demo 83)
47. Angular vocabulary as the framing mechanism: Jones normalization destroys 28% of angle count (43 → 31 at ζ₁₂) while leaving direction count unchanged (512 → 512); framing lives in how quaternions rotate, not where they point (Demo 83)
48. Bracket-null indispensability theorem: the 9 bracket-null entries (Re(q)=0) in the ζ₈ catalog cannot be removed without destroying XOR capacity; they contribute 6 exclusive S² directions (cube-edge midpoints) absent from non-null entries, and removing them performs worse than removing random entries of equal count (Demo 84)
49. S¹ flatness of null entries: all bracket-null entries have half-angle = 90°, a single point on S¹; their entire computational contribution comes from S² directional diversity rather than angular discrimination; confirmed by flat k_sec sweep across k=1 through k=8 (Demo 84)
50. Non-semisimplicity as the computational resource: ζ₄ (semisimple, δ=2) has 75% null entries and zero XOR capacity; ζ₈ (non-semisimple, δ=0) achieves full capacity; the radical provides both the null-entry directional infrastructure and the Jordan-block memory mechanism for depth scaling (Demo 84)
51. Null efficiency amplification (3× cost): the non-null-only subset reaches XOR8 but requires k_sec = 16–20 (vs k=6 for the full catalog); null entries reduce the angular resolution cost of XOR discrimination by a factor of approximately 3; nulls are "efficiency amplifiers," not dead weight (Demo 84)
52. Pure synergy in DKC: neither null-only nor non-null-only subsets can compute XOR6 at k_sec=1 alone; their union achieves 36 winners; the k_sec=1 result is a pure-synergy (Williams & Beer 2010) instance — the computational contribution of each part is zero, but the combination is non-zero; the DKC manifestation of LCFT Jordan-cell coupling ⟨T|t⟩ = b (Demo 84)

No prior work connects these fields. The intersection is genuinely unoccupied. The question "can topological invariants be compiled into neural network weights?" has no direct precedent in the literature. Eighty-four demos prove the answer is yes — and map the exact boundary of what is computable, characterize the algebraic structure of the computation, establish how that computation scales, and decompose its sources into independent resources. That boundary is not just algebraic: it has a geometric layer (the wall at k=23), a topological layer (the 24-cell), a differential-geometric layer (the S² Hopf base), a spectral layer (bandwidth l=6), a group-theoretic layer (only ζ_4 and ζ_8 generate finite SU(2) subgroups), a depth layer (crossing depth governs capacity linearly), and now a framing layer (the writhe phase the Jones polynomial normalizes away contributes exactly 2 XOR levels). The parity function, unreachable by split-sigmoid, requires exactly 13 angular directions on a sphere — a number simultaneously equal to the DOF count for the answer, the number of vertices of the binary octahedral group, and the minimum bandwidth index for S² reconstruction. None of these coincidences are accidental. The computation is additive, group-breaking, and algebraically coherent. It does not approximate; it is exact. Its power decomposes into three independent additive resources: lattice structure, crossing depth, and writhe. Its directional vocabulary requires bracket-null entries that are invisible to the scalar invariant but indispensable for higher-capacity computation — a concrete realization of the LCFT prediction that null states carry computational content through Jordan-cell coupling. And for infinite-group roots of unity, it scales logarithmically without a ceiling.
