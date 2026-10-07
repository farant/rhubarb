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
