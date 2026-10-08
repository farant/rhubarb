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
