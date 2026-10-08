# cyclotomia worklog

## 2026-10-07 - v1: exact cyclotomic integers Z[zeta_n]

**Why.** 57 knotapel demos use cyclotomic arithmetic: Z[zeta_8] (`Cyc8`,
873 uses), Z[zeta_16], Z[zeta_5], Z[zeta_24], zeta_12 and zeta_32, plus 3x3
matrices over Z[zeta_8]. All of it is homemade, with `long` coefficients:
the D112 overflow failure mode. The house-library rule says this belongs in
a tested library.

**Fran's decisions.**
- Names: `cyclotomia` (context and library), `Cyclotomicus` (element).
- The element is a wrapper struct around the reduced polynomium, so an
  unreduced polynomial cannot be passed by mistake.
- Integers only in v1; the field Q(zeta_n) later if Temperley-Lieb work
  needs inverses.
- Floats only for display: `cyclotomicus_ad_ostendendum` returns TEXT
  ("0.7071 + 0.7071i"), so it cannot feed comparisons. Everything that
  decides is exact: `aequalis`, `est_radix` (a = +-zeta^k, exact angle),
  `modulus_quadratus` (|a|^2 in the real subfield), `norma`, `vestigium`.
- Certified rational intervals were discussed and are deferred.

**Representation.** Z[t]/Phi_n: the power basis 1..zeta^(phi-1), with magnus
coefficients. The canonical form is unique, so equality is polynomial
equality.
- Context: Phi_n, phi(n), the power table zeta^k (k < n), the negated table
  (so `est_radix` needs no piscina; `magnus_nega` can allocate for large
  values), the units, and an `Anulus` descriptor (`contextus` = the
  context, like `anulus_residuorum`).
- Phi_n is built bottom-up over the divisors:
  Phi_d = (t^d - 1) / prod_{e | d, e < d} Phi_e.
- The power table comes from repeated shift-and-reduce by the monic Phi_n.
- **One core routine**, `_reducere(r, p, k)`: substitute t -> zeta^k into
  any Laurent polynomial, mapping each exponent (k e) mod n through the
  table. The same routine does canonical reduction (k = 1), evaluating knot
  polynomials at roots of unity, and the Galois automorphisms sigma_k.
- Division: q = a * prod_{sigma != 1} sigma(b) / N(b), exact per
  coefficient or FALSUM.
- Not Euclidean in general, so the Anulus Euclid hooks are NIHIL; `matrix`
  determinants (Bareiss) work, and are tested with a Vandermonde.

**Tests (41, 0.4 s).** The oracles are independent of the code:
- Phi_1, Phi_2, Phi_6, Phi_8, Phi_12 literally.
- For n <= 300: deg = phi(n) by gcd count; Phi_n(1) = p for n = p^k, else 1;
  coefficients in {-1, 0, 1} for n < 105; Phi_2p(t) = Phi_p(-t).
- Phi_105 has -2 at t^7 and t^41 (the first coefficient outside
  {-1, 0, 1}).
- Ring, for n in {1, 2, 3, 4, 5, 8, 12, 16, 24, 105}:
  - zeta^n = 1, zeta^-1 * zeta = 1, and the roots of unity sum to 0;
  - N(1 - zeta) = Phi_n(1);
  - Tr(zeta) = mu(n) (Ramanujan sum) and Tr(1) = phi;
  - random elements: sigma_a sigma_b = sigma_ab, N multiplicative,
    Tr additive, (a b) / b = a, text round trip;
  - 1 / (1 - zeta) is refused for n = p^k.
- Gauss sums g^2 = p (p = 1 mod 4) or -p, for p = 3, 5, 7, 11, 13;
  (zeta_8 + zeta_8^-1)^2 = 2; est_radix signs; |1 + i|^2 = 2; the display
  strings.
- **Knots: all 12,966 table knots.**
  - V(e^(2 pi i/3)) = 1;
  - V(i) = (-1)^Arf, with Arf = 0 iff det = +-1 mod 8 (Levine). Arf is 1
    for 6,418 knots, so the check is not vacuous;
  - |V(-1)| = det.

  Jones (Kauffman bracket from PD) and Alexander (Fox matrix) are
  independent computations, now joined through the new ring.

**Plants (all red):**
- C1: sign error in the power-table reduction (17);
- C2: conjugate = identity (2);
- C3: division without the remainder check (1);
- C4: est_radix sign always +1 (1);
- C5: one divisor skipped in Phi_d (12);
- C6: identity not skipped in the conjugate product (2);
- C7: negative exponent mirrored instead of wrapped mod n (5);
- C8: display with cos and sin swapped (1).

**Next.** Redo knotapel's DKC line on the house ring (the D114-style audit
of D29 / D100-109), and Q(zeta_n) if the TL matrices need inverses.

## 2026-10-07 - review I (fresh recensor-mathematicus)

The exact arithmetic is correct. The reviewer checked it with its own Python
oracle, also under ASan/UBSan:
- Phi_n for n = 1..1000 against the Moebius product; 105 is the first n
  with a coefficient outside {-1, 0, 1};
- 17,908 ring checks over 30 values of n, including 30-digit coefficients:
  every automorphism, refusal of non-units, divisibility decided by an
  integral solve of the multiplication matrix;
- 63 Bareiss determinants against a Leibniz oracle (Vandermonde 4x4/5x5 at
  n = 3, 5, 8, 12, 16, 24, a 6x6 at n = 8, random, singular, pivot swap);
- the knot facts are theorems: Jones, Murakami, Levine.

Fixed:
- **HIGH, stack overflow in the display.** `sprintf("%.*f")` into a
  128-byte buffer prints every integer digit of a large double:
  (1 + zeta_8)^230, or 10^100 at 15 digits, wrote 253 bytes (rc 133 without
  ASan). C89 has no snprintf, so |x| >= 10^15 now goes through %e, the two
  parts are formatted separately, and the line buffer is 256 bytes.
- **Magnus -> f64 truncated at 255 characters**, so 10^300 displayed as
  about 1e254. It now builds a 17-digit mantissa plus an exponent for
  strtod.
- **The f64 cancellation is silent:** (sqrt2 - 1)^60 = 1e-23 displays as
  millions. The header now states the error bound
  (~phi * max|c| * 2^-52); the display stays display-only.
- **Division computed the conjugate product twice** (inside norma and again
  directly). It is now computed once and N = b * product, about 2x faster
  (10.8 ms per division at n = 105 before).
- **Documented:** the embedding Z[zeta_8] -> Z[zeta_16] via ex_polynomio
  with k = 2; that radix shares the context table (immutable); the measured
  memory (n = 840: 6 MB, creation < 5 ms).
- **Test gaps (mutations that survived, now red):**
  - P54, automorphism = identity, and P55, j -> j^2: the composition law
    alone held for both. Now sigma_3(zeta_8) = zeta_8^3 and sigma_-1 =
    conjugate are pinned;
  - P1, gcd refusal removed: now j = 2, 0, 8 must be refused;
  - P13, est_integer checking imus: now 1 + zeta must not be an integer;
  - P4, n = M refused: now creare(M) must work;
  - display: zeta_8^6 = "0.0000 - 1.0000i", 10^100 and 10^300 strings, and
    a length check for (1 + zeta_8)^230. The plant restoring %f for large
    values is red.

  53 tests.
- **Performance, recorded:** one Z[zeta_8] multiply costs 180-270 ns and
  allocates 288 B, against under 1 ns for the demos' `long`. That is fine
  for the D114-style audit (1e6-1e7 operations). Sweeps of 1e8-1e9 would
  need a fixed-width layer later, with this library as its oracle.

Open for Fran (the API): elements carry no ring, so aequalis(zeta_8,
zeta_16) = VERUM and rings mix silently; and there is no s64-array
constructor or reader for demo interop.

## 2026-10-07 - v2 API: ring-tagged elements, s64 interop (Fran approved)

**Ring tags.** `Cyclotomicus` now carries `constans Cyclotomia* anulus`.
- Operations read the ring from their elements, so `multiplica(a, b, p)`
  and the like no longer take a context. Constructors (nullum, integer,
  radix, ex_polynomio, ex_chorda, ex_s64) do.
- Mixing rings gives an INVALID element: `anulus == NIHIL`, tested with
  `cyclotomicus_est_validum`, and it propagates like NaN. Functions that
  return b32 refuse instead: aequalis (zeta_8 vs zeta_16 is now FALSUM; it
  used to be VERUM because both are the polynomial t), divide_exacte,
  automorphismus, est_radix, est_integer.
- ad_chordam / ad_ostendendum print "invalidum".
- `norma` and `vestigium` now return b32 with an out-parameter: a trace of
  0 is legitimate, so a Magnus return value could not signal an error.
- The Anulus hooks refuse elements of another ring (`_an_bonum` checks the
  ring of every result). **Lesson from plant T2:** a mixed Bareiss
  determinant is ALSO caught by the exact-division hook, so a det-only test
  could not see the hook check. matrix_multiplica and matrix_adde have no
  division, so only the hook guards them. Tests now pin a mixed product and
  a mixed sum.

**s64 interop**, for the D114-style audit of the old `Cyc8` demos:
- `cyclotomicus_ex_s64(r, c, numerus)` takes any length and reduces, so
  {0,0,0,0,1} in Z[zeta_8] = -1;
- `cyclotomicus_ad_s64(a, out)` writes phi(n) coefficients and refuses a
  coefficient outside s64, or an invalid element.

The embedding Z[zeta_8] -> Z[zeta_16] via ex_polynomio(k = 2) is tested as a
homomorphism.

**Tests: 78.** Plants: T1 (rings never compared) red, 14 failures; T2 (hook
accepts foreign elements) red after the product/sum tests were added; T3
(ad_s64 ignores overflow) and T4 (ex_s64 skips reduction) red.
