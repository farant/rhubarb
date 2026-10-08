# extensio worklog

## 2026-10-08 - E1: number fields Q(alpha), arithmetic, norm and trace

**Why.** #3 of the math map (`project-specs/mathematica-bibliothecae-visio.md`)
was half done: `cyclotomia` covers Z[zeta_n], but the quaternion demos
(D66-D97) live in Q(sqrt 2), Q(sqrt 3) and Q(sqrt 5), and their faults
(D117: axes deduplicated by float tolerance, silent caps) come from doing
that arithmetic in doubles. Quaternions over any ring (#7) come next and
need an exact ordered field underneath.

**Fran's decisions (API approved 2026-10-08).**
- Names: library `extensio`, field `Extensio`, element `Algebraicus`.
- Generic number field Q(alpha) = Q[t]/f, f monic irreducible in Z[t], with
  a degree-2 fast path. A field, not a ring of integers (sqrt 3 / 2 is not
  integral), so the descriptor has `corpus` = VERUM and `matrix` can divide.
- Exact real order through a chosen real root, selected by INDEX (k-th
  smallest real root), never by a caller-supplied interval. Certified digits
  for display.
- General purpose: the generic constructor `extensio_ex_polynomio(f, radix)`
  goes in (E2), with honest irreducibility checks (squarefree always; no
  rational root, which decides degrees 2-3; above degree 3 the caller
  asserts it, and a zero divisor is refused at inversion).

**Representation.** numerator(alpha) / denominator: numerator a polynomium in
alpha of degree < d, denominator > 0, gcd(content, denominator) = 1, zero =
0/1. Unique, so equality is structural. Elements carry their field (as in
cyclotomia v2): mixing fields gives an invalid element.

**Algorithms.**
- Reduction mod monic f by Horner from the top (t^d -> -(f - t^d)); any
  degree.
- Inverse: degree 1 trivial; degree 2 closed form, f = t^2 + bt + c:
  (p + q a)^-1 = (p - bq - q a) / (p^2 - bpq + cq^2); otherwise Cramer on
  the integer multiplication matrix (columns = numerator * alpha^j), d + 1
  Bareiss determinants. det = 0 -> FALSUM (zero, or a zero divisor if f is
  reducible).
- Norm = det of the multiplication matrix / den^d; trace = its trace / den.
- alpha^-1 = -(f - f(0))/(t f(0)), so Laurent input (negative exponents) is
  accepted when f(0) != 0.
- Q(cos 2pi/n): Psi_n from Phi_n by peeling (t + 1/t)^j off t^-m Phi_n;
  alpha = 2cos(2pi/n) is the largest root; n <= 2 handled as degree 1.

**Tests (78, 0.5 s).** Oracles independent of the code: minimal polynomials
written out (Psi_5, Psi_7, Psi_9, Psi_10, n = 1..12); for n = 3..120, degree
= phi(n)/2 by gcd count and every 2cos(2 pi j/n), j a unit, is a root in
f64; norm and trace of random elements against the product and sum over the
conjugate roots in f64 (cosine fields) and against p^2 - d q^2, 2p
(quadratic fields, imaginary ones too); field identities on 40 random
elements in 15 fields (inverse, a^-3 a^3, associativity, distributivity, N
multiplicative, Tr additive, text round trip, N(q) = q^d, Tr(q) = d q,
alpha^-1 alpha = 1); facts (phi^2 = phi + 1, N(phi) = -1, N(1 + sqrt 2) =
-1, N(alpha) = 1 and Tr(alpha) = -1 in Q(cos 2pi/7)); a 4x4 Vandermonde over
Q(sqrt 5) equals the product of differences, a singular matrix has rank 3,
and a foreign-field element makes the determinant refuse.

**Plants (10, all red):** reduction sign, gcd skipped, denominator sign
skipped, quadratic inverse with +b, Cramer row-for-column, norm / den^1,
trace off the diagonal, squarefree test `<` for `<=`, mixed fields allowed,
alpha^-1 sign.
- E10 SURVIVED the first suite: the only Laurent test was in Q(sqrt 2),
  where f(0) < 0 takes the other branch and overwrites the planted line.
  Added 1/i = -i, 1/sqrt(-3) = -sqrt(-3)/3 and alpha^-1 alpha = 1 in every
  field; now red.
- The first red run of the identities was the ORACLE: `magnus_ad_s64` fails
  silently for norms beyond s64 (degree-12 norms of random elements reach
  1e50) and the f64 conversion returned 0. The oracle now converts through
  the decimal string; the library was right.

**Tooling found on the way.**
- `VOCABULA_VIAE_ADDITAE` is SPACE-separated; a comma list is one path that
  does not exist, and the lint reports "NOVA 0 - nihil novi" having judged
  nothing. Caught by planting a nonsense identifier.
- venenum gate: the macOS SDK marks `sprintf` deprecated only under the
  sanitizers, so any suite linking cyclotomia (or extensio) failed to BUILD
  there; C89 has no snprintf. The gate now passes
  `-Wno-deprecated-declarations` and runs extensio.
- A new library needs `./tools/compile_tests_fontes_generare.sh` (the
  runner's source list is generated from aedilis closures), else the test
  links without it.

**Next (E2).** Sturm sequences over Z (primitive PRS), real-root isolation
by bisection on exact rationals, `extensio_radices_reales`,
`extensio_ex_polynomio(f, radix)` with its checks, `algebraicus_signum`,
`algebraicus_compara`. Then E3: certified decimal digits.

## 2026-10-08 - review I (E1) fixes + E2: Sturm, root isolation, exact order

**Review I (fresh recensor-mathematicus, own Python oracle).** No wrong
arithmetic: ~20k operations over 131 fields (60-digit coefficients, ASan /
UBSan / venenum), Psi_n correct for every n <= 1000 by a separate Chebyshev
derivation, 360 random matrices over 6 fields. Fixed:
- F1 (UB): `extensio_quadratica(INT64_MIN)` negated INT64_MIN before the
  range check and returned an out-of-contract field. Range now checked first.
- F2 (DoS): text "a^100000" took 1.1 s / 427 MB through dense Horner (one
  step per exponent); "a^1073741823" never returned. Inputs of degree >= 2d
  now go term by term with `algebraicus_potentia` in a scratch pool
  (`_ex_positivo`).
- F3 (perf): the inverse ran d + 1 Bareiss determinants (Cramer): d = 24,
  40-digit coefficients, 1.1 s and 1.5 MB of garbage. Now ONE kernel solve
  of [M | -e_0] (`matrix_nucleus`), x / lambda, in a scratch pool; norm and
  trace also use scratch pools (`_officina_aperire` + `_transcribere`).
- F4: "a + 1/2" was read as (a + 1)/2. Unparenthesised "P/D" now requires a
  single-term P (refused otherwise).
- F5: Psi_n by the Chebyshev recurrence C_(j+1) = x C_j - C_(j-1), O(m^2)
  (was O(m^3)).
- F6: header documents Laurent refusal when f(0) = 0 and that field identity
  is by POINTER (two calls of extensio_quadratica(5) are different fields).
- Test gaps (14 surviving mutants): `aequalis` was never asserted FALSUM
  within one field (identity tests vacuous); est_rationalis negative;
  canonical `nega`; degree-1 fields (n = 3, 4, 6) now in the identity loop;
  generator of a degree-1 field reduced; denominator 0 via ex_polynomio;
  foreign elements in matrix add/multiply/aequalis; n = M boundary.

**E2 (API approved 2026-10-08).** `extensio_ex_polynomio(f, radix)`,
`extensio_radices_reales`, `extensio_radix`, `algebraicus_signum`,
`algebraicus_compara`.
- Sturm chain: primitive PRS with POSITIVE pseudo-remainder scaling
  |lc|^k (signs preserved). Isolation by bisection from the Cauchy bound,
  left to right, so roots come out ascending and "radix = k-th smallest" is
  well defined.
- Generic constructor checks: monic, squarefree (last chain element
  constant), radix range, and no rational root: every real root refined to
  width < 1 and the integer next to it tested (monic: rational roots are
  integers). (t - 3)(t^2 - 2) is refused although 3 is never a bisection
  midpoint.
- `signum`: interval Horner of the numerator over the root's interval,
  bisecting until 0 is excluded. ZERO CERTIFICATE instead of a blind limit:
  the numerator is integral and alpha an algebraic integer, so N(num(alpha))
  is a nonzero integer unless num(alpha) = 0, and every conjugate is bounded
  by M = sum |c_i| B^i (B = Cauchy bound): |num(alpha)| >= 1/M^(d-1). An
  enclosure narrower than that still containing 0 PROVES num(alpha) = 0,
  i.e. f reducible -> FALSUM. The test of that case went from 4 s (2000
  blind bisections) to milliseconds.
- Named families do not isolate all roots: a candidate interval (sqrt d in
  (r, r + 1]; 2cos(2pi/n) from f64 +- 2^-16 with denominator 2^20) is
  CERTIFIED by Descartes' rule, which is exact because every root is real:
  roots above infra = d - radix, above supra = d - radix - 1 (Taylor shift
  of the integer polynomial q^d f((p + w)/q), O(d^2) small multiplications).
  If the certificate fails the constructor returns NIHIL (loud) rather than
  falling back to Sturm.

**Memory and time (Fran saw ~80 GB during the plant run).**
- The 80 GB was plant E11 (signed scaling): leading terms never cancel, the
  `dum` loop in `_residuum` never ended and coefficients doubled in an
  unbounded test pool. The shipped test peaked at 15 MB. Fixed at the root:
  `_residuum` is STRUCTURALLY bounded (deg a - deg b + 1 passes), the Sturm
  chain cannot run past its array. The plant runner now has a memory
  watchdog (kills a test above 2 GB, counts it red).
- The first E2 candidate check used a Sturm chain: degree 498 (n = 997) took
  3.3 s and 2.85 GB (chains grow like d^3). Descartes: 0.11 s; n = 1000 is
  0.01 s / 15 MB. A Sturm fallback for the named families was itself a
  multi-GB bomb at degree 200 (plants E20, E24): now refused instead.
- I briefly deleted the "use the candidate" branch while editing, so every
  certified candidate fell into full isolation (n = 500: 4.3 GB). Results
  were identical, so no correctness test could see it; two CREDO_NON_PENDET
  deadline tests (cosinus(997), "a^200000 + 1") now catch any return to a
  slow path, and run in a child process, which also caps runaway memory.

**Tests: 152, 0.7 s.** Oracles: hand root counts (incl. non-squarefree
counting distinct roots), Psi_n all-real for n <= 60, refusals (non-monic,
rational roots, not squarefree, radix range, Laurent f), cube root of 2
bounds, signs of (1 + sqrt 2)^k and phi^k conjugates up to k = 100 (sign
(-1)^k, magnitude down to 1e-38, far below f64), random signs against f64 at
known roots in 6 generic fields, compara antisymmetry, a reducible quartic
accepted by assertion with its zero divisor refused by inverse and signum.

**Plants: 24, 22 red, 2 equivalent.** E13 (zeros counted as sign changes in
a Sturm chain) is equivalent by the Sturm property (a zero inside the chain
sits between opposite signs). E16 (radix range `>` for `>=`) is caught
downstream: `_creare` refuses an index >= the roots found.

**Not done (E3 next):** certified decimal digits for display. Generic
constructor still uses Sturm: high degree is expensive (document; the named
families avoid it).

## 2026-10-08 - review II fixes (E2b)

Review II (same reviewer, own exact oracles: Sturm over Q, rigorous sign by
refinement + Lipschitz bound, 60-digit Decimal cosines): no wrong sign, root
order or arithmetic anywhere; zero certificate and Descartes use valid; all
loops structurally bounded. Contract and cost problems, all fixed:
- **H1 (signum refused valid input):** the fixed 2000-bisection cap ran out
  for small values ((1 - sqrt 2)^1000: FALSUM after 0.9 s / 47 MB; generic
  fields up to 18 s / 548 MB) although the header promised FALSUM only for a
  proven zero. Now:
  - degree 2: exact closed form, sign of X + Y sqrt D by the signs of X, Y
    or X^2 against Y^2 D (instant; (1 - sqrt 2)^3000 was 12 s by bisection);
  - degree >= 3: CENTRED FORM |num(alpha) - num(m)| <= D w / 2 with D =
    sum |c_i| i R^(i-1) (R = max |endpoint|), exact num(m); the loop limit
    is COMPUTED: log2(w0 D M^(d-1)) + 8, which the certificate argument
    proves sufficient (nonzero values are decided before D w drops below
    1/M^(d-1); below it, 0 in the enclosure PROVES zero);
  - state in two scratch pools (status / opus) alternating: zero bytes left
    in the caller's pool (tested).
  - D was first computed by interval Horner; dropping two of its four
    corner products (plants E17/E28) survived every test because on a
    2^-16 interval the corners nearly coincide. The direct bound needs no
    interval arithmetic, so the proof no longer depends on corner logic;
    `_horner_intervalli` is gone.
- **M1 (valid polynomial refused):** isolation depth capped at 2000:
  Mignotte t^5 - 2(10^200 t - 1)^2 was NIHIL even with radix -1. Depth now
  from the Mahler-Mignotte separation bound (bit lengths via the new
  `magnus_bitorum`); 10^200 accepted in 1 s.
- **M2 (generic constructor cost):** the Sturm chain was built four times in
  the caller's pool (all-real degree 40: 4.8 s / 1.4 GB; degree 60: 43 s /
  10.8 GB of pool). Now built once in scratch; point evaluation by integer
  homogeneous Horner q^g p(n/q) (no fractions, no gcds) that rolls back its
  own allocations; the chosen root's interval is passed to `_creare` as
  certified. Degree 40: 0.33 s, 87 KB left in the caller's pool.
- **M3 (my F2 fix regressed dense input):** one `potentia` per term made
  the all-ones degree-40000 polynomial 17 s / 2.2 GB. Now sparse Horner in
  Z[t] mod f: acc t^gap + c, gap <= 2d by shift + reduce, larger gaps by
  t^gap through squaring; two scratch pools alternate. 0.2 s / 13 MB.
- **Rational-root check** now tests EVERY integer in the narrowed interval,
  so its correctness no longer depends on the width (reviewer's N30 /
  plant E31 is now equivalent by design).
- New tests: degree-3 near-zero signs at negative roots with mixed-sign
  coefficients; a self-consistency test independent of floats (c = 17-digit
  approximation of alpha, sign of (alpha - c)^e (alpha^2 + 5) must be
  sign(alpha - c)^e); Mignotte close roots ordered around 1/a; (t - r)(t^2 -
  2) refused for r = 2..40; "(a^20 + 1)/2"; caller-pool growth of a sparse
  high power bounded; deadline tests for degree-40 construction and dense
  degree-20000 input. 171 tests, 2.2 s.
- Plants: 30 run, 24 red. Equivalent or caught downstream: E8 (perfect
  squares pass the squarefree loop but fail the Descartes certificate), E13
  (Sturm property), E16 (radix range, refused by _creare), E31 (all integers
  tested), E35 (R from |infra| only: on a 2^-16 interval |infra| ~ |supra|
  and the absolute-sum bound already overestimates |num'|).

## 2026-10-08 - review III fixes (E2c)

Review III: no wrong result anywhere (4200+ signs, 1267 compara, 188
generic fields, (1 - sqrt 2)^10000, (alpha - c)^500 in degree 3-12, ASan /
UBSan / venenum); centred-form argument, computed limit, degree-2 sigma
mapping, Mahler-Mignotte depth and the rollback all confirmed. Fixed:
- **M1 (crash):** the recursive isolation overflowed the C stack for
  Mignotte a = 10^1000 (17,434 frames). Now iterative with an explicit
  stack of pending intervals (right child pushed first so roots stay
  ascending; stack <= depth + 1 frames). a = 10^1000 accepted (161 s,
  400 MB - extreme input).
- **M2 (near-zero signs cubic):** numerator evaluated at every bisection
  through fractions with gcds. Now the interval is DYADIC integers
  (A, C) / 2^S (every root interval in the library is dyadic by
  construction: integers, /2^20, bisections from an integer bound; a
  non-dyadic interval refuses, unreachable): a bisection is one addition
  and one doubling, f's sign by integer homogeneous Horner, and the
  numerator is evaluated EXACTLY only at doubling checkpoints (iterations
  1, 2, 4, 8, ... and the limit), so at most twice the needed bisections
  but O(log) numerator evaluations. (alpha - c)^500: cos 2pi/27 (degree
  9) 220 s -> 2.5 s; cube root of 2 43 s -> 14 s. The remaining cost is
  O(bits^3) inherent to exact bisection (8850 steps at 9000-bit points);
  interval Newton (quadratic convergence) is the next step if needed.
- **T1 (wrong signs not caught):** R forced to 1 in D gave wrong signs
  where |alpha| >> 1. The consistency test now runs at all four roots
  (-9, -7, 2, 17) of t^4 - 3t^3 - 207t^2 - 653t + 2141; plant E36 red.
- **L1, L2 (docs):** the generic constructor grows ~d^7 (40: 0.3 s, 60:
  4 s, 80: 34 s), not d^3; exact values like a^(2^30) in Q(sqrt 5) are
  ~10^9 bits by nature.
- Plants: 33, 28 red; equivalent or caught downstream: E8, E13, E16, E31,
  E35 (reasons in the E2b entry). New red: E36 (radius 1), E37 (isolation
  stack in reverse order), E38 (dyadic step without doubling).

## 2026-10-08 - review IV (E2c) + predicted checkpoints

Review IV (focused on E2c): iterative isolation ascending and its stack
bound (<= limes + 1; the + 3 guard can never fire), the dyadic invariant
unreachable-to-violate on every path, the final-limit test always decides,
no aliasing across the status rollback; ~3400 signs / 1267 compara, 0
wrong, 0 refused; T1 kills C5, C6, C3, C1, D3. One perf note (P1): doubling
checkpoints overshoot up to 2x in iterations (Q(cos 2pi/19), (alpha - c)^500
~150 s). Now the next checkpoint is PREDICTED from the current miss: the
error halves per bisection, so i + log2(error / |num(m)|) + 2, capped by the
doubling schedule (never worse) and doubling when num(m) = 0; correctness
still rests on the test at the limit. (alpha - c)^500: cos 2pi/19 150 s ->
6.6 s, cube root of 2 14 s -> 4.0 s, cos 2pi/27 2.4 s. New deadline test:
(alpha - c)^250 in Q(cos 2pi/27) (~0.3 s, limit 2 s) - kills the
"test only at the limit" plant (E39). Doubling-only (E40, the E2c
behaviour) survives: a 2x difference is not reliably separable by a
deadline; recorded as a bounded performance mutant. 172 tests.

## 2026-10-08 - E3: certified decimal display

`algebraicus_ad_ostendendum(a, digiti, p)`: a to `digiti` decimals,
CORRECTLY ROUNDED, every printed digit certain.
- Rational elements: exact rounding, ties to even (as `fractio_rotunda`).
- Ordered fields: `_propinquum` refines the root interval (the dyadic
  bisection of signum, in scratch pools) until the centred-form error D w /
  (2 den) is below 1/(4 10^k), giving a rational m. R = round(m 10^k) is
  then CERTIFIED exactly: signum(a 10^k - (R + 1/2)) < 0 < signum(a 10^k -
  (R - 1/2)), with a +-1 correction (at most a few steps). The checks reuse
  signum, so they are bounded by the zero certificate, and a 10^k for
  irrational a is never exactly a half-integer, so they always decide.
- Unordered fields (Q(i)...): exact text (no real embedding chosen);
  invalid: "invalidum". Format "-1.4142", ASCII minus; a value rounding to
  zero prints without sign ("0.00000").
- Shared helpers factored out of signum: `_derivata_limes` (D) and
  `_intervallum_binarium` (interval as A / 2^S, C / 2^S).

Tests (oracle: Python decimal at 250 digits, ROUND_HALF_EVEN; 2cos(2pi/7)
by Newton in Decimal): sqrt 2 to 50 digits, phi to 40, -sqrt3/7 to 30,
2cos(2pi/7) to 30, cube root of 2 to 40, (1 + sqrt 2)^50 (an integer minus
1e-19) to 10, (1 - sqrt 2)^21 rounding to "0.00000", rational ties
(5/2 -> 2, 7/2 -> 4, -1/8 -> -0.12), NEAR-HALF cases 0.123455 +- (sqrt 2 -
1)^30 at 5 digits ("0.12346" / "0.12345"), 80 random elements against f64
(within one unit in the last of 8 places). 193 tests, 1.6 s.

Plants (6, all red): decimal point misplaced, zero padding short,
certification skipped, correction in the wrong direction, minus sign lost,
rationals floored instead of rounded.

**v1 complete** (E1 arithmetic, E2 order, E3 display; four review passes,
no wrong arithmetic found in any). Next library: quaternions over any ring
descriptor (#7), which needs an optional `signum` hook in `anulus` for
ordered rings.

## 2026-10-08 - review V (E3) fixes

Review V: 0 wrong digits in ~7,100 displays against an independent
exact-rounding oracle (fuzz over every field kind and radix, digiti 0..200,
1,600 rational ties, 1,032 near-half constructions at k = 0..150);
certification exact, a wrong decimal can never be returned (fallback = the
exact text); signum results unchanged. Fixed:
- **T1:** certifying with the lower bound at R - 1 (O11b) printed 193 wrong
  roundings in the reviewer's near-half sweep while the suite stayed green
  (my two hand-made cases happened to approximate on the right side). Now a
  SWEEP: 12345.5 / 10^k +- u^n (u = sqrt 2 - 1 or (sqrt 5 - 1)/2, u^n <=
  10^-(k+1) or 10^-(k+3)), k = 0..40, both directions, two fields; expected
  strings from the separately tested RATIONAL path. O11b red.
- **P1 (my review-IV change regressed most fields):** near zero |num(m)|
  halves with the error, so the prediction kept saying "a few more steps":
  one evaluation every 3-5 bisections (870 vs 13). The next checkpoint is
  now at least i + i/2 + 1 (logarithmic) and at most the doubling.
  (alpha - c)^500: cube root 2 2.5 s, cos 2pi/7 0.94 s, cos 2pi/13 0.43 s,
  cos 2pi/19 8.2 s (E2c 1.7 / 2.2 / 9.3 / ~150).
- **P2:** each display refined the root interval three times from 2^-16.
  `signum` is now `_signum_ex(a, infra, supra, ...)` with the public wrapper
  passing the field's interval; `_propinquum` returns its refined interval
  and both certification signs start from it. 1000 digits: degree 9 1.6 s,
  18 6.5 s, 21 9.1 s (was 5-14 s at 9-12); ~3 KB left in the caller's pool.
- **L1:** digiti > 100000 returns the exact text (was 10^(2^32 - 1) for
  (i32)-1).
- Plants: 41; 34 red; equivalent / performance-only: E8, E13, E16, E31,
  E35, E40 (doubling only), E41 (approximation error underestimated - the
  exact certification corrects R, so approximation quality affects speed,
  never correctness). 195 tests, 1.6 s.
