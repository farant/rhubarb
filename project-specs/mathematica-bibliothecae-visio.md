# Math Library Map — Fran's vision, verbatim

2026-10-05 · Fran

*Fran's map of the planned C89 math libraries and the determinism rules,
preserved WORD FOR WORD as a shared reference (pasted into a Claude Code
session on 2026-10-05; the embedded diagram "library map · 26 libraries in
4 layers" was not part of the paste). Nothing here is decided by being
recorded. Commentary, corrections and decisions live elsewhere — the
extraction sheet `project-specs/visiones-extractae/mathematica-bibliothecae-visio.md`
(with Claude's review in its commentary section) and the tabularium — and
cite this file; do not edit the text below, add a dated note at the end
instead. Companion: `project-specs/knotapel-plexitas-visio.md`. Already
built when recorded: #25 as `sors` (2026-09-23), the integer half of #1 as
`magnus` (b603e31d, 2026-10-05).*

---

# Math Library Map

Oct 5, 2026 · @Fran Arant

## At a glance

Twenty-six zero-dependency C89 libraries: items 1–12 are the original list, 13–26 are additions. Each layer builds only on the layers below it, and only four libraries live in the float tier.

&#91;embedded content: library map · 26 libraries in 4 layers\]

Everything outside the tinted chips is exact (or exact at its decision points), so results agree bit for bit across machines.

## Determinism rules

The governing rule: floats may help *find* a result, but never *be* the result. Anything that is state, a decision, or shared between machines is exact integer data.

**Three tiers.**

| Tier | What lives here | Arithmetic |
| --- | --- | --- |
| Decisions | Over/under, intersection, knot type, crossing code, rank, sign | Exact: integers, rationals, algebraic numbers, or float filters with exact fallback |
| Simulation state | Anything fed back into the next step, anything synced over a network, any canonical program | Integers or fixed point (e.g. Q16.16 with a hand-written 32×32→64 multiply) |
| Search and presentation | Textile relaxation, rendering, interpolation, statistics, measurement | Disciplined float subset; results snapped to integers and verified before they count |

**Why floats are not trusted for shared state.** Per [Dependable C](https://dependablec.org/#floats), different hardware, sometimes from the same vendor, can round differently, so floats are "not reliable for lockstep synchronizations"; some small platforms have no FPU or only 32-bit floats. Known sources of divergence:

- x87 extended precision on 32-bit x86 (intermediates kept in 80-bit registers).
- Fused multiply-add contraction (GCC default on ARM64; Clang within expressions).
- Fast-math reordering.
- libm transcendental functions differ across libraries in the last bit.
- Denormals flushed to zero on some ARM and SIMD units.
- Compile-time constant folding versus run-time evaluation.
- Summation order in reductions.

**The disciplined float subset** (for the search and presentation tier):

- `double` only, never `long double`; only +, −, ×, ÷ and `sqrt`, which IEEE 754 requires to be correctly rounded.
- Own implementations of sin, cos, exp, log: explicit range reduction plus fixed polynomials, using only the operations above.
- Build with `-ffp-contract=off`, no fast-math, SSE2 on x86, `-fexcess-precision=standard` on GCC.
- Fixed summation order; Kahan summation where accuracy matters.
- Never `==` on computed floats; only against an assigned value, or `x == x` to detect NaN.
- A cross-platform golden-hash test of results as the only real proof.

**Lockstep networking.** Every client must compute bit-identical state each tick.

- Integer simulation core; floats only in presentation that never feeds back.
- Exchange a hash of full state each tick (or every N); on mismatch, resync from an authoritative snapshot. Needed even when all-integer.
- Alternative architecture: an authoritative server sends results rather than inputs, trading bandwidth for not needing bit-agreement.

**C89 hazards beyond floats.**

| Hazard | Fix |
| --- | --- |
| No guaranteed 64-bit integer (`long long` is C99; `long` is 32-bit on Windows) | Own wide arithmetic, or the big-integer library |
| Negative division and modulo round in an implementation-defined direction | Own divide and modulo for negative operands |
| Right shift of negative integers is implementation-defined | Explicit arithmetic |
| `qsort` is unstable; libcs order equal elements differently | Total ordering with explicit tie-breaks, or own stable sort |
| `rand()` differs across libcs | Own seeded generator |
| `int`/`long` sizes and plain `char` signedness vary | Defined fixed-width types, checked at build time |
| Struct padding, endianness, uninitialized bytes leak into hashes | Field-by-field serialization in a fixed byte order |
| Hash-table order keyed by pointer addresses varies run to run | Key by stable ids |
| Undefined behavior (signed overflow, uninitialized reads) | Avoid; optimizers diverge on it |
| Directed rounding (`fesetround`) is C99 | Widen interval bounds outward by a relative 2⁻⁵² per operation |

For knotapel this means the executable is canonical integer data: lattice control polygons, the crossing code, integer tension and refinement parameters, and dyadic-rational subdivision done with integer shifts.

## Arithmetic

The exact foundation everything else is built on; all three are decision-tier.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 1 | Big integers and rationals | Arbitrary-precision integers, exact rationals | Foundation for everything below; also supplies the 64-bit arithmetic C89 lacks |
| 2 | Modular arithmetic and finite fields (Z/p) | Arithmetic mod a prime; ranks and determinants mod p | Fast and fraction-free; demo 60 computed at two primes. Running modulo several primes and combining (Chinese remaindering) is the standard way to make exact linear algebra fast |
| 3 | Exact algebraic numbers | Cyclotomic integers Z\[ζₙ\] for any n, Gaussian integers, quadratic fields such as ℚ(√2) and ℚ(√5) | The demos have separate Cyc8, Cyc16 and ζ₁₂ structs, each with its own overflow behavior; one generic Z\[ζₙ\] on big integers replaces them all. ℚ(√5) gives exact icosahedral symmetry; tangent half-angle keeps angles rational |

## Algebra

Generic structures over the rings in Arithmetic; each was rebuilt ad hoc in several demos.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 4 | Polynomials | Univariate over Z, ℚ or Z/p, plus Laurent polynomials (the Kauffman bracket lives in Z\[A, A⁻¹\]): arithmetic, exact division, gcd, evaluation, normalization | Demos 1–9 and 112 each built this |
| 5 | Exact linear algebra | Matrices over any ring above: Bareiss determinant, rank, kernel, Smith and Hermite normal forms, Gram matrices | Note 52's module structure was exactly this computation, done in a scratch script; also the engine for persistent homology (#16) |
| 6 | Finite groups | Permutation groups, conjugacy classes, derived series, orbits | Demos 94–97 essentially rediscovered the conjugacy classes of the binary octahedral group by hand |
| 7 | Quaternions and rotations | Exact and floating point; integer quaternions give rotations with rational matrices; icosians in ℚ(√5) | Used everywhere from demo 66 on; also generates Pythagorean-hodograph curves (#13) |

## Geometry and combinatorics

The combinatorial and geometric substrate for threads; #13–15 are additions.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 8 | Combinatorial enumeration | Subsets, Gray codes, matchings, Catalan objects, braid words | Note 55's counting needed it |
| 9 | Exact geometric predicates | Orientation tests, segment intersection, coplanarity, triangle–segment tests, decided exactly (the Shewchuk lesson) | Demo 110's crossing finder is a homemade, partly broken version; also gates triangle moves and over/under decisions |
| 10 | Graphs | Component analysis and graph algorithms over the structures above | Raqiya started this |
| 13 | Curves and frames *(new)* | Bézier and B-splines, Pythagorean-hodograph curves, subdivision schemes (Chaikin, four-point with tension), arc length; Frenet frames plus rotation-minimizing / parallel-transport frames (Frenet breaks on straight segments) | Core of the "one head, one thread" stage; tension parameter sets plexity |
| 14 | Spatial indexing *(new)* | Uniform grid hashing, k-d trees or bounding-volume hierarchies, Morton and Hilbert encodings | Coverage, Hausdorff and collision tests at scale; the Hilbert curve is also the baseline generator |
| 15 | Meshes and simplicial complexes *(new)* | Half-edge meshes, marching cubes for implicit surfaces, tube-mesh export | Looking at knotapels and outputs; implicit Milnor-polynomial knot rendering |

## Measuring shape

Tools that check what an experiment actually produced; all three are additions.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 16 | Computational topology | Simplicial complexes, Vietoris–Rips filtrations, persistent homology by boundary-matrix reduction over Z/2 | Built on #2 and #5. Verifies "this output really is S⁴"; drives the linked-rings experiment. Exact, so decision-tier |
| 17 | Spectral transforms | FFT; Haar and Daubechies wavelets; number-theoretic transform (FFT mod p) | Wavelet-decay plexity measure and Fourier knots; the NTT gives exact fast polynomial multiplication for #4 |
| 18 | Estimation | Log-log slope fitting for refinement exponents and box-counting dimension; bootstrap confidence intervals | #12 builds the controls; this measures the effect. Float tier |

## Bridging floats and exactness

These let fast float work feed exact conclusions without ever letting a float be the answer.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 19 | Interval arithmetic *(new)* | Certified lower/upper bounds; outward widening by a relative 2⁻⁵² per operation, since C89 has no `fesetround` | Proves a computed curve or relaxation does not touch another strand |
| 20 | Integer relation finding *(new)* | LLL lattice reduction, PSLQ | Turns a suspicious float into an exact conjecture ("this is (1+√5)/2", "3a − 2b + c = 0") to prove with #1–5; replaces by-hand discovery |

**Adaptive predicates** (the technique behind #9): evaluate the predicate in floats with a proven error bound; accept a clear sign; recompute exactly only when the result is too close to zero. The answer is identical on every machine, provided intermediates are plain IEEE double (no x87 extended precision, no FMA contraction).

## Simulation and search

The float-tier machinery for the textile compiler and ML experiments, plus exact search over sign patterns; all are additions.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 21 | Numerical integration and optimization | Fixed-step integrators (RK4, symplectic Verlet), energy minimization (gradient descent, L-BFGS), Kahan summation | The textile compiler; output snapped to the integer lattice and verified exactly |
| 22 | Float linear algebra | SVD, symmetric eigendecomposition, PCA, Kabsch/Procrustes alignment (best rotation between point sets) | Activation analysis and pose-invariance tests; #5 does not cover these |
| 23 | Automatic differentiation | Dual numbers (forward mode), small reverse-mode tape | Exact curvature and torsion derivatives; trains tiny networks for linked-rings and compiled-versus-trained without a framework |
| 24 | Constraint solving | Small SAT or integer-programming solver; exact LP or Fourier–Motzkin elimination over ℚ | Knot types are open regions cut out by sign inequalities: "is this pattern realizable?", "find a minimal stick trefoil on this lattice". Exact, decision-tier |

## Domain and method

The knot library is the destination the rest serves; the remaining three keep experiments honest and reproducible.

| # | Library | Contents | Why |
| --- | --- | --- | --- |
| 11 | Knot library | Braid words, diagram codes (Gauss, PD), polygons with isotopy-safe triangle moves, lattice knots, invariants (bracket, Jones, Alexander, determinant, linking number, writhe) | The library the whole knotapel project rests on; crossing code is authoritative, invariants double as checksums |
| 12 | Experiment controls | Size-matched random subsets, permutation tests, baselines (Hilbert, Fourier, wavelet) | Notes 16, 20, 47 and 56 were all missing controls; designing the right control matters as much as any of the math |
| 25 | Deterministic random numbers *(new)* | Seeded, splittable generator (SplitMix or PCG) | Every run reproduces from its seed; #12 depends on it; replaces libc `rand()` |
| 26 | Output and provenance *(new)* | Plain-text SVG, PLY/OBJ and CSV writers; a run log of seed, parameters and code version | Every finding traces back to the exact run that produced it |

## Build order

Start with the exact core and the reproducibility plumbing, then the pieces the ball/cube experiment needs.

1. **Foundations:** big integers and rationals (#1), deterministic random numbers (#25), fixed-width types and C89 hazard fixes (Determinism rules).
2. **Exact core:** modular arithmetic (#2), exact predicates (#9), polynomials (#4), exact linear algebra (#5).
3. **First experiment:** curves and frames (#13), spatial indexing (#14), persistent homology (#16), output and provenance (#26).
4. **Discovery:** integer relation finding (#20), highest payoff for the kind of discovery the demos kept making by hand.
5. **Consolidation:** generic algebraic numbers (#3), finite groups (#6), knot library (#11) absorbing the demos' scattered implementations.
6. **As experiments demand:** the rest, in the order their experiments come up.
