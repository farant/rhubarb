# Cross-Demo Connections

How 51 demos feed into each other, what recurs, and where threads unexpectedly converge.

---

## 1. Dependency Graph: The Five Arcs

### Foundation Arc (D01-D09)
Linear chain building the bracket computation pipeline:

```
D01 (state-sum oracle) --> D02 (braid bracket) --> D03 (TL module)
     |                          |                       |
     v                          v                       v
D07 (braid->PD bridge)    D04 (spin chain)         D06 (Jones-Wenzl)
     |                          |
     v                          v
D08 (PD->braid Vogel)     D05 (scaling n=2..5)
     |
     v
D09 (Reidemeister invariance)
```

Every later demo traces back to D01's `Poly` type and state-sum oracle. The chain
has a clear pedagogical structure: compute the bracket (D01), represent it algebraically
(D02-D06), bridge representations (D07-D08), prove invariance (D09).

### Classification Arc (D10-D17)
Numerical pivot and the "what can we do with this" exploration:

```
D10 (numerical engine) --> D11 (waveforms) --> D12 (quantization)
                                |                    |
                                v                    v
                           D14 (zeros)          D13 (DKC test)
                                |                    |
                                v                    v
                           D15 (WRT angles)     D17 (multi-layer)
                                |
                                v
                           D16 (reduced bracket g)
```

D10 is the second major pivot: replaces symbolic `Poly` (808 bytes/entry) with
numerical `Cx` (16 bytes/entry), enabling 50-100x scaling. Everything from here
through D17 explores the bracket as a classification signal.

### Gate Arc (D18-D25)
Braid-as-computer: gates, circuits, closures, measurement:

```
D18 (NOT/NAND gates) --> D19 (circuit compiler) --> D20 (error landscape)
                                                         |
                                                         v
                                                    D21 (reversible gates)
                                                         |
                                                         v
D24 (rational tangles)                              D22 (min complexity)
                                                         |
                                                         v
                                                    D23 (function zoo)
                                                         |
                                                         v
                                                    D25 (closure measurements)
```

D18-D19 prove computational universality via braids. D22-D25 map the landscape of
what topology "prefers" to compute. D24 is a standalone tangent connecting bracket
values to rational arithmetic. The parity barrier first surfaces at D25 (zero
appearances across 1554 braids x 4 closures).

### DKC Arc (D26-D29 --> D45-D50)
The central research arc, split into two phases:

**Phase 1 — Reverse DKC (D26-D29):**
```
D26 (reverse: real weights)
 |
 v
D27 (reverse: complex weights, U(1) gauge)
 |
 v
D28 (activation comparison: modReLU, cardioid)
 |
 v
D29 (FORWARD DKC: exact Z[zeta_8], XOR triples) <-- LANDMARK
```

**Phase 2 — Forward DKC Zoo (D45-D50):**
```
D45 (all 13 NPN classes, forward-reverse duality)
 |
 v
D46 (per-neuron gauge, phase decoherence thesis)
 |
 v
D47 (MVN activation, correlation reversal)
 |
 v
D48 (100M quartets, XNOR3 = 0 solutions) <-- LANDMARK
 |
 v
D49 (neglecton weights, perturbation closed)
 |
 v
D50 (k-sector activation, parity at k=6) <-- LANDMARK
```

D29 is the first landmark: forward DKC confirmed, exact bracket values compute XOR.
D48 sharpens the parity wall (zero solutions out of 100M). D50 resolves it: the
wall was in the activation, not the lattice.

### Radical Anatomy Arc (D38-D39, D51-D52, D60)
Algebraic deep-dive branching from D47's conjecture:

```
D47 conjecture: "TL non-semisimplicity is the parity barrier"
 |
 v
D38 (dimension reduction: Gram rank C_{n-1} at even n, quotient isomorphism)
 |
 v
D39 (symmetry decomposition: cellular sectors, multi-delta comparison, block structure)
 |
 v
D51 (radical anatomy at delta=0, n=2..8)
 |
 v
D52 (Chebyshev generalization: delta=0,1,sqrt(2),phi,sqrt(3))
 |
 v
D60 (ell=7 cubic wall: all formulas confirmed at first cubic number field)
```

D38 proves the dimension reduction theorem (TL_n/rad isomorphic to TL_{n-1}) and
the even/odd dichotomy. D39 decomposes into cellular sectors, verifying d(n,j)^2
sizes and mapping multi-delta Gram ranks. D51 discovers the sandwich theorem and
alternating semisimplicity. D52 extends to the full Chebyshev parameter space.
D60 confirms universality at the first cubic number field (ell=7).

### Cross-ell Arc (D53-D59)
Ring-parametric replication of DKC across evaluation levels:

```
D53 (ell=3, delta=1, Z[omega])
 |
 v
D54 (ell=4, delta=sqrt(2), Z[zeta_16])
 |
 v
D55 (ell=4 Boolean search)
 |               \
 v                v
D56 (Potts-TL)   D57 (additive encoding)
                   |
                   v
                  D58 (ell=5, delta=phi, Z[zeta_5]) -- Fibonacci anyons
                   |
                   v
                  D59 (ell=6, delta=sqrt(3), Z[zeta_24]) -- Q=3 Potts
```

Each demo follows the same template: implement cyclotomic ring, verify TL relations,
build bracket catalog, run Boolean function search. The topology code (planar
matchings, diagram composition, trace closure) is copied verbatim --- completely
ring-independent.

### Proof Arc (D61-D62)
The final theoretical resolution:

```
D55,D58,D59 (11/13 wall observed at 3 independent ell values)
 |
 v
D61 (computational proof: integer arithmetic, N=360 grid)
 |
 v
D62 (analytical proof: algebraic contradictions for 0x06 and 0x1B)
```

D62 closes the investigation: the wall is geometric (parallelogram gap on circle),
not algebraic. Zero floating-point in the proof.

---

## 2. Code Reuse Patterns

### The "Standard Library" That Emerged

Four code assets appear in 30+ demos and constitute a de facto standard library:

| Asset | Introduced | Used in | Role |
|-------|-----------|---------|------|
| Complex arithmetic (`Cx`) | D10 | 35 demos | Numerical workhorse |
| Union-find loop counter | D02 | 37 demos | State-sum backbone |
| NPN classification | D23 | 13 demos | Boolean function taxonomy |
| Braid word decoder | D22 | 13 demos | Exhaustive enumeration |

### Ring Tower

A progression of cyclotomic rings, each built for a specific evaluation level:

```
Z (integers) -- D01-D09
  |
  Cx (floats) -- D10-D28
  |
  Z[zeta_8] -- D29, D35, D48-D50
  |
  Z[omega] -- D53
  |
  Z[zeta_16] -- D54-D57
  |
  Z[zeta_5] -- D58
  |
  Z[zeta_24] -- D59
```

Each ring carries the same API surface: zero/one/add/mul/eq/to_cx/print.
The TL matrix engine sits on top, parameterized by ring. Swapping the coefficient
ring is the only change between D53, D54, D58, and D59.

### The Two Oracles

Every new computation method is validated against one of two oracles:

1. **State-sum oracle** (D01): brute-force 2^n enumeration, ground truth for brackets.
   Used to validate the matrix method (D03-D06), numerical engine (D10), TL matrix
   method (D35, D53-D59).

2. **Forward DKC catalog** (D29): exact Z[zeta_8] bracket values, ground truth for
   Boolean function reachability. Used to validate trained network decomposition
   (D45-D47) and activation function analysis (D48-D50).

### Copy-Paste Pattern

There is no shared library extraction. Each demo is standalone `main.c`. This means:
- Bug fixes don't propagate (MAX_TERMS varies: 64 in D01, 120 in D04+)
- Each demo compiles independently (a feature, not a bug)
- The "library" is pattern knowledge, not shared object files
- Ring-parametric code (D53-D59) is the closest to a true abstraction

---

## 3. Recurring Themes

### Exact vs Approximate Arithmetic

This tension runs through the entire project. The symbolic polynomial ring (`Poly`,
D01-D09) is exact but expensive. The numerical engine (`Cx`, D10+) is cheap but
approximate. The cyclotomic rings (D29+) are exact AND efficient for specific
evaluation points.

Key moments: D29 shows exact Z[zeta_8] arithmetic makes forward DKC possible.
D48's 100M-quartet exhaustive search depends on exact equality testing. D61-D62's
proofs use pure integer arithmetic with zero floating-point.

The pattern: approximate methods discover phenomena, exact methods prove them.

### Non-Semisimplicity as Both Obstacle and Resource

At delta=0, TL generators are nilpotent (e_i^2 = 0). This makes the algebra
non-semisimple for even n (D51), which:

- **As obstacle**: The radical kills degrees of freedom needed for parity (D47
  conjecture, D48 confirmation). Bracket evaluation factors through the semisimple
  quotient TL/rad, losing radical information.

- **As resource**: Nilpotency at delta=0 means only single-loop states survive,
  enabling the delta=0 simplification (D29) that makes exact computation feasible.
  The very constraint that blocks parity is what enables exact DKC for 12/13 classes.

D49 tests whether radical elements (neglectons) can break the parity wall. They
cannot --- the perturbation approach is closed. D50 resolves this by changing the
activation function instead of the weight space.

### The Activation Function as the Key Variable

The project converges on activation geometry as the determining factor for
computational capability:

- Split-sigmoid (D27-D29, D45-D48): sees magnitudes, destroys phase. 12/13 NPN
  classes via forward DKC, parity impossible.
- MVN-continuous (D47): sees phases, different correlation structure. Complementary
  to split-sigmoid.
- k-sector (D50): parity reachable at k=6 with 906 solutions from the same catalog
  that split-sigmoid searched 100M times finding zero.
- Re>0 half-plane (D55, D58, D59, D61-D62): 11/13 universal across all ell values.
  Wall proven geometrically.

The lattice is fixed; the activation chooses which subset of its structure to access.

### Topology -> Algebra -> Computation Pipeline

The project's central pipeline:

```
Knot/braid topology  -->  TL algebra / bracket values  -->  Boolean computation
     (D01-D09)              (D29, D35, D51-D52)              (D18-D25, D45-D50)
```

Each arrow is non-trivial. The first collapses the infinite braid group to a
finite-dimensional TL algebra (compression ratios measured in D22, D35). The second
maps cyclotomic integers to truth tables via activation-mediated classification.

### Angular Structure and Its Geometric Meaning

Bracket values at delta=0 are axial: they lie on 8 rays in the complex plane
(D35 axiality theorem). At delta=sqrt(2), values are Z[i]-axial: 2D subspace
of the 8D ring (D54). At delta=phi, full 4D usage (D58).

This angular structure determines computational capability:
- The 8 axes of Z[zeta_8] create a discrete angular alphabet
- k-sector activation reads this alphabet directly (D50)
- Half-plane activation projects it to a binary partition (D61-D62)
- The parity function requires 3 odd sectors at 120-degree spacing (triskelion
  principle, D50) which is incommensurable with the pi/4 lattice at k=2

---

## 4. Surprise Connections

### Fibonacci Golden Ratio in ell=5 Ring (D58)

At ell=5, delta = phi = (1+sqrt(5))/2. Delta powers are literal Fibonacci numbers:
phi^k = F(k-1) + F(k)*phi. The "Fibonacci anyon" name is not categorical --- it is
arithmetically literal. This connects number theory (Fibonacci sequence), representation
theory (TL algebra at ell=5), and topological quantum computing (anyonic universality).

### WRT Convergence (D15)

Greedy angle selection (optimizing an information-theoretic criterion) independently
converges to within 1.5% of the WRT level-3 evaluation point from Chern-Simons theory.
The bracket "knows" which angles are informationally optimal, and those angles are
the algebraically special ones. Topology knows information theory.

### Forward-Reverse Duality (D45)

Spearman rho = -0.82 between topology frequency (how often a braid produces a function)
and decomposition quality (how well trained weights match bracket values). Functions the
bracket "likes" to produce are functions whose trained solutions live near the bracket
lattice. This was measured independently from two directions and arrived at the same
ordering.

### Correlation Reversal Under Activation Change (D47)

Split-sigmoid and MVN-continuous produce opposite correlation signs between topology
frequency and decomposition quality (rho = -0.78 vs +0.55). The two activations
literally access complementary sectors of the bracket algebra. This was not predicted.

### Neglecton Closure (D49)

The perturbation approach (adding radical-element weights from loops=2 states) yields
148 new Z[zeta_8] values from 15,242 previously invisible braids --- but ALL new values
share angles with existing standard values. No new angular directions at any perturbation
order. The weight space already contains everything; only the activation geometry limits
computation.

### 11/13 Wall Universality (D55, D58, D59, D61-D62)

Three independent evaluation levels (ell=4, 5, 6) with three different cyclotomic rings
(Z[i], Z[zeta_5], Z[zeta_24]) all produce exactly 11/13 reachable NPN classes under
Re>0 multiplicative encoding, with the same two classes (0x06, 0x1B) unreachable. This
universality was unexpected: the rings have different dimensions (2, 4, 8), different
algebraic properties, different delta values. The wall is purely activation-geometric.

### Two Different Proof Structures for Two Unreachable Classes (D62)

0x06 and 0x1B fail for different geometric reasons:
- 0x06: local parallelogram argument (four semicircle gaps all < pi)
- 0x1B: global interval squeeze (lower bound > upper bound on one angle)

Despite both being "unreachable by half-plane," they are obstructed by fundamentally
different mechanisms. This hints at a richer obstruction theory.

---

## 5. Convergence Points

### D29: The Forward DKC Moment

Threads converging:
- Exact arithmetic (D01-D09 symbolic pipeline, refined to Z[zeta_8])
- Delta=0 simplification (only single-loop states survive)
- Split-sigmoid activation (from D27 complex neuron)
- Gauge symmetry (U(1) rotation from D27)

Result: 100+ exact XOR triples from bracket values alone. Training-as-focusing thesis
confirmed. This is where the project shifts from "can braids compute?" to "what can't
they compute?"

### D50: Parity Resolution

Threads converging:
- Parity wall (D25 closure measurements, D48 exhaustive proof of zero solutions)
- Activation analysis (D28 modReLU, D47 MVN, D49 neglecton closure)
- k-sector classification (Aizenberg's MVN theory)
- Z[zeta_8] angular structure (D35 axiality, D50 triskelion geometry)

Result: Same 100-value catalog, different activation, parity solved with 906 solutions
at k=6. Four-tier NPN hierarchy. Incommensurability hypothesis. The wall was in the
activation all along.

### D51-D52: Radical Structure Meets DKC

Threads converging:
- D47's conjecture (TL non-semisimplicity as parity barrier)
- D35's TL matrix representations
- Graham-Lehrer cellularity theory (from literature)

Result: Sandwich theorem (rad^2 isomorphic to previous odd TL algebra), Catalan trace
theorem, Fibonacci rank theorem, cross-ell radical dimension formulas. Multiple novel
results connecting abstract algebra to computational barriers.

### D61-D62: The Proof

Threads converging:
- Three independent 11/13 observations (D55 at ell=4, D58 at ell=5, D59 at ell=6)
- Multiplicative encoding angular structure (angles add, from D55)
- NPN classification machinery (from D23)
- Convexity thesis (from D57)

Result: Clean analytical proofs via integer arithmetic and geometric arguments. Zero
floating-point. The 48 unreachable truth tables (orbits of 0x06 and 0x1B) are
characterized by parallelogram and interval-squeeze obstructions on the circle.

### D63: Angular Anatomy and Parity Ceiling

Threads converging:
- Triskelion geometry (D50 parity at k=6, 906 solutions)
- Z[zeta_8] axiality (D35, octant structure)
- MVN k-sector activation (Aizenberg 2008, sector-octant mapping)
- Oriented matroid theory (Bjorner et al., matroid covector lattices)
- RT truncation connection (D39 Parts G-J, Markov trace kills j >= ell-1)

Result: Complete parity scaling law for Z[zeta_8] — ceiling n=5. Extends DKC from
3-input (Demo 50) to 4-input (k=8, 96 solutions) and 5-input (k=15, 3020 solutions).
Falsifies k=2n triskelion conjecture (n=5 needs k=15 not k=10). Discovers that parity
requires the alternating oriented matroid type — the first connection between DKC and
matroid theory. Two types of impossibility: constraint-geometric wall (n=6,7) vs
pigeonhole (n>=8).

### D39 Parts G-J: The Proof Trilogy

Threads converging:
- Cellular algebra theory (Graham-Lehrer 1996)
- Fixpt vs Markov trace (two "lights" on TL_n)
- RT truncation (Reshetikhin-Turaev 1991, negligible ideal)
- Wrong conjecture as discovery tool ((ell+m-1)^2+1 refuted at m=3)

Result: Three formal proofs anchoring the algebraic side of DKC. Markov RT truncation
theorem identifies the Markov kernel as J(A) + negligible ideal. The "shadow
interpretation" — fixpt trace is algebraic light, Markov trace is topological light,
excess = what topology cannot see — provides the deepest framing of how DKC uses
pre-closure information that RT truncation would discard.

---

## Summary: The Project's Logic

The project follows a single thread:

**Can topological structure compute?**

D01-D09 build the bracket. D10-D17 extract information from it. D18-D25 prove
classical universality but discover the parity barrier. D26-D29 connect bracket
values to neural network weights and prove forward DKC. D45-D50 map the complete
landscape and resolve the parity wall via activation geometry. D51-D52 ground the
algebraic explanation. D53-D59 verify universality across evaluation levels.
D61-D62 prove the half-plane wall analytically. D63 establishes the parity scaling
ceiling at n=5 inputs. D39 Parts G-J provide three formal proofs anchoring the
algebraic theory.

The answer: yes, with caveats. The bracket algebra computes 12/13 NPN classes
through its natural structure. The 13th (parity) requires reading angular information
that half-plane activations cannot access but k-sector activations can. The wall is
geometric, not topological. Demo 63 establishes the fundamental capacity limit:
parity up to n=5 inputs in Z[zeta_8], with two distinct impossibility mechanisms
beyond. Three formal proofs (radical dimension, next-level radical, Markov RT
truncation) now anchor the algebraic theory.
