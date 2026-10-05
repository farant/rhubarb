# Visiones extractae — mathematica-bibliothecae-visio.md

*Extracted and classified 2026-10-05 from `project-specs/mathematica-bibliothecae-visio.md`
(Fran, 2026-10-05: "Math Library Map"). Fran's document. One claim per
bullet, worded as close to the source as possible; `Lnn` = source line;
library numbers `#n` are the source's. The last section is my commentary,
kept apart.*

*Marking: put `[+]` on the claims to keep for the distillate.*

*Tags (see `docs/taxonomiae.html`): `{PRINCIPIUM}` prime condition,
`{PRINCIPIUM+intervention, split}` a condition with its cure attached,
`{OCCASIO}` opportunity, `{INDICIUM}` clue, `{REGULA}` commitment, `{VISIO-OPERIS}` project vision, `{DECRETUM}`
load-bearing decision, `{SUPELLEX}` furniture, `{nulla:…}` fits no tier
(ratio, exemplum, quaestio). `> Xn` = the item(s) it is evidence for; `> Rn` = the project-wide regula it bears on.*

## I. The governing rule

- [ ] X1 Floats may help *find* a result, but never *be* the result: anything that is state, a decision, or shared between machines is exact integer data (L33)  {REGULA > R2}
- [ ] X2 Twenty-six zero-dependency C89 libraries in layers, each building only on the layers below; only four live in the float tier (L25)  {VISIO-OPERIS > R3}
- [ ] X3 Everything outside the float tier is exact (or exact at its decision points), so results agree bit for bit across machines (L29)  {PRINCIPIUM}

## II. Three tiers

- [ ] X4 Decisions (over/under, intersection, knot type, crossing code, rank, sign): exact integers, rationals, algebraic numbers, or float filters with exact fallback (L39)  {REGULA}
- [ ] X5 Simulation state (anything fed back, synced, or canonical): integers or fixed point, e.g. Q16.16 with a hand-written 32×32→64 multiply (L40)  {REGULA}
- [ ] X6 Search and presentation (relaxation, rendering, statistics, measurement): a disciplined float subset; results snapped to integers and verified before they count (L41)  {REGULA}

## III. Why floats are not trusted

- [ ] X7 Different hardware can round differently, so floats are "not reliable for lockstep synchronizations" (Dependable C) (L43)  {PRINCIPIUM}
- [ ] X8 Known divergences: x87 extended precision, FMA contraction (GCC on ARM64, Clang within expressions), fast-math, libm last-bit differences, flushed denormals, constant folding, summation order (L45-L51)  {nulla:exemplum > X7}
- [ ] X9 The disciplined subset: `double` only; only +, −, ×, ÷ and `sqrt`; own sin/cos/exp/log; `-ffp-contract=off`, no fast-math, SSE2, `-fexcess-precision=standard`; fixed summation order, Kahan where needed; no `==` on computed floats (L55-L59)  {REGULA}
- [ ] X10 A cross-platform golden-hash test of results is "the only real proof" (L60)  {REGULA}
- [ ] X11 Lockstep networking: integer simulation core; exchange a state hash each tick and resync on mismatch, "needed even when all-integer"; or an authoritative server sending results (L62-L66)  {DECRETUM}

## IV. C89 hazards beyond floats

- [ ] X12 No guaranteed 64-bit integer: own wide arithmetic or the big-integer library (L72)  {PRINCIPIUM+intervention, split}
- [ ] X13 Negative division and modulo, and right shift of negatives, are implementation-defined: own operations (L73-L74)  {PRINCIPIUM+intervention, split}
- [ ] X14 `qsort` is unstable and `rand()` differs across libcs: total orders with tie-breaks, own seeded generator (L75-L76)  {PRINCIPIUM+intervention, split}
- [ ] X15 Type sizes and `char` signedness vary: defined fixed-width types, checked at build time (L77)  {PRINCIPIUM+intervention, split}
- [ ] X16 Padding, endianness and uninitialized bytes leak into hashes; pointer-keyed hash tables reorder: field-by-field serialization, stable ids (L78-L79)  {PRINCIPIUM+intervention, split}
- [ ] X17 Undefined behaviour (signed overflow, uninitialized reads) lets optimizers diverge: avoid it (L80)  {REGULA}
- [ ] X18 For knotapel the executable is canonical integer data: lattice control polygons, crossing code, integer tension and refinement parameters, dyadic subdivision by integer shifts (L83)  {DECRETUM}

## V. The libraries

- [ ] X19 Arithmetic (#1–3): big integers and rationals; modular arithmetic and finite fields (with Chinese remaindering for fast exact linear algebra); exact algebraic numbers (generic Z[ζₙ], Gaussian integers, ℚ(√2), ℚ(√5)) (L91-L93)  {VISIO-OPERIS}
  - [ ] X19.1 one generic Z[ζₙ] on big integers replaces the demos' separate Cyc8, Cyc16 and ζ₁₂ structs, "each with its own overflow behavior" (L93)  {INDICIUM}
- [ ] X20 Algebra (#4–7): polynomials including Laurent; exact linear algebra (Bareiss, rank, kernel, Smith and Hermite forms, Gram matrices); finite groups; quaternions and rotations (exact and float, icosians) (L101-L104)  {VISIO-OPERIS}
- [ ] X21 Geometry and combinatorics (#8–10, #13–15): enumeration; exact predicates (the Shewchuk lesson); graphs; curves and frames (splines, PH curves, subdivision with tension, rotation-minimizing frames); spatial indexing (with Morton and Hilbert encodings); meshes and simplicial complexes (L112-L117)  {VISIO-OPERIS}
- [ ] X22 Measuring shape (#16–18): persistent homology over Z/2 (exact, decision-tier); spectral transforms (FFT, wavelets, NTT for exact fast polynomial multiplication); estimation (log-log fits, bootstrap; float tier) (L125-L127)  {VISIO-OPERIS}
- [ ] X23 Bridging floats and exactness (#19–20): interval arithmetic with outward widening; integer relation finding (LLL, PSLQ) to turn "a suspicious float into an exact conjecture" (L135-L136)  {VISIO-OPERIS}
- [ ] X24 Adaptive predicates: float evaluation with a proven error bound, exact recomputation only near zero; identical on every machine given plain IEEE double (L138)  {DECRETUM > X4}
- [ ] X25 Simulation and search (#21–24): integrators and minimizers for the textile compiler, float linear algebra, automatic differentiation, constraint solving (SAT/IP, exact LP or Fourier–Motzkin over ℚ) (L146-L149)  {VISIO-OPERIS}
  - [ ] X25.1 knot types are open regions cut out by sign inequalities, so "is this pattern realizable?" is a constraint problem (L149)  {OCCASIO}
- [ ] X26 Domain and method (#11–12, #25–26): the knot library ("the destination the rest serves"), experiment controls, deterministic random numbers, output and provenance (L157-L160)  {VISIO-OPERIS}
  - [ ] X26.1 the crossing code is authoritative and invariants double as checksums (L157)  {REGULA}
  - [ ] X26.2 "designing the right control matters as much as any of the math" (L158)  {PRINCIPIUM}
  - [ ] X26.3 every finding traces back to the exact run that produced it (seed, parameters, code version) (L160)  {REGULA}

## VI. Build order

- [ ] X27 (1) foundations: #1, #25, fixed-width types and hazard fixes; (2) exact core: #2, #9, #4, #5; (3) first experiment: #13, #14, #16, #26; (4) discovery: #20; (5) consolidation: #3, #6, #11; (6) the rest as experiments demand (L166-L171)  {DECRETUM}
- [ ] X28 Integer relation finding has the "highest payoff for the kind of discovery the demos kept making by hand" (L169)  {OCCASIO}

## VII. Since then (my commentary, not in the source)

Review given in the session of 2026-10-05, recorded here so it survives
the context window; none of it edits the source.

- **Built when recorded:** #25 is `sors` (PCG32 + SplitMix64, 2026-09-23);
  the integer half of #1 is `magnus` (b603e31d, 2026-10-05: canonical
  values, s64 fast path, Euclidean division by Knuth D, gcd with Bézout
  witnesses, no floats). Fixed-width types exist in `latina.h` (X15).
- **X12 is already decided implicitly.** The house compiles with
  `-Wno-long-long` and uses `long long` throughout; `magnus` relies on
  64-bit intermediates. Recommendation: state it as a platform requirement
  with a build-time size check (negative-array-size trick) rather than
  rebuild on 16-bit limbs at ~4× cost.
- **X9 is not yet house practice.** `tools/vexilla.sh` does not pass
  `-ffp-contract=off`; Clang on ARM64 contracts within expressions by
  default, so house float code can round differently from the same source
  built elsewhere. One flag, but it changes every build (every suite owed):
  an open decision for Fran.
- **X14:** `lib/` has no `qsort` calls (0 uses on 2026-10-05); raqiya's uses
  sort ints only, where equal elements are indistinguishable.
- **#19's widening (L81, L135) needs care.** A relative 2⁻⁵² widening of 0 is
  0, subnormals break it, and the widening multiply itself rounds. Robust
  portable version: step the IEEE bit pattern outward with integer
  operations (a hand-written `nextafter`), plus an absolute floor near zero.
- **#20's PSLQ in doubles finds only tiny relations** (~15 digits).
  Proposal: exact LLL over `magnus` on high-precision approximations held as
  scaled big integers (a big fixed-point layer on #1), keeping even
  discovery deterministic.
- **X24 stated precisely:** the float never decides the sign, only whether
  the exact fallback runs; the filter must be sound for the platform's
  arithmetic (the plain-IEEE proviso).
- **#16 is exact only if the filtration is:** order Vietoris–Rips simplices
  by squared distances in exact rationals.
- **X10 can start partially now:** hash outputs across `-O0`/`-O2` and
  Clang/GCC on the one platform; contraction and folding differences show
  there.
- **Proposed additions:** dyadic rationals inside #1 (subdivision, interval
  endpoints, the #20 fixed-point layer); continued fractions and
  Stern–Brocot with #3/#20 (rational tangles are continued-fraction
  arithmetic); and a fourth determinism rule — decision-tier functions
  return a certificate where cheap (Bézout coefficients, kernel vectors,
  Farkas certificates for #24, SAT proof traces), the bridge to a future
  checker.
- **Build order adjustment proposed:** after `fractio` + dyadics, take #9
  early (it gates every geometric decision) and use "demos 110–112 redone
  on exact arithmetic" as the forcing function for #1, #9, #4 and the first
  slice of #11.
