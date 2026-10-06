# fractio.worklog.md

## 2026-10-05 — natus

`fractio` (Latin "a breaking", the medieval word for a fraction): exact
rational numbers over `magnus`. Second of the house math foundations
(plan: magnus → fractio → polynomials → exact linear algebra; Fran's map in
`project-specs/mathematica-bibliothecae-visio.md`, #1). API reviewed with
Fran before code; three choices left to "whatever is best for a general
purpose library":

- **Round half to even** (`fractio_rotunda`). Exact rationals land on exact
  halves often (anything over 2), so half-away-from-zero's upward bias in
  magnitude is a real effect over many roundings; half-to-even is unbiased
  and the IEEE default. The schoolbook rule composes from floor and ceiling.
- **Accessors, not readable fields**, as in magnus: the invariant cannot be
  broken from outside and the representation can change.
- **Signed exponent** in `fractio_potentia`: rationals are a field, so a⁻ⁿ
  is natural; only 0 to a negative power fails (b32 return). Negating the
  exponent is done as −(e+1)+1 in unsigned so S32_MIN does not overflow.

Design:

- **Canonical form is the invariant**: denominator > 0, gcd 1, zero = 0/1.
  Equality is then equality of parts. The tests assert it after every
  result (`_canonica`).
- **Henrici's reduction** (Knuth TAOCP II, 4.5.1) for + and ×: the gcds are
  taken BEFORE multiplying, so intermediates stay small and the result needs
  no final reduction. Edge cases checked by hand and by test: sums that
  cancel to zero come out 0/1 (since gcd(0, d1) = d1); products with a zero
  factor come out 0/1 (gcd(x, 0) = |x|).
- **Comparison never divides**: a/b vs c/d is a·d vs c·b (positive
  denominators). It needs a piscina for large values — the one change from
  the sketch.
- **Floor is magnus's Euclidean quotient** (positive denominator); ceiling
  and rounding come from the Euclidean remainder (0 ≤ r < d), ties decided
  by the parity of the quotient.
- Parsing: "a" or "a/b", sign only on the numerator ("3/-4" is refused,
  not guessed), non-canonical input reduced ("6/4" → 3/2).
- Not here on purpose: dyadic rationals (own small type later — shifts, no
  gcds), mediants / continued fractions / Stern–Brocot (their own library,
  using this one).

Verification:

- **Independent oracle**: 20 vector pairs from Python's `fractions.Fraction`
  (sum, difference, product, quotient, floor, ceiling, round — Python's
  `round` is also half-to-even — and order), covering s64 boundaries, big
  numerators/denominators, signs, zero. One random divisor came out 0 in
  the generator; the oracle marks it "-" and the test now expects a refusal
  there (a free zero-divisor case).
- Tables for parsing (8 malformed inputs, output untouched), rounding (13
  cases incl. ±1/2, ±3/2, ±5/2, 7/2), powers (incl. exponent S32_MIN on −1).
- **Field laws over 1000 random triples** (sors seed 2026, stream 1;
  numerators and denominators up to 40 digits plus boundary values):
  round trip through text, n/(−d) = −(n/d), (a+b)−b = a, commutativity,
  distributivity, order agrees with the sign of a−b, (a/b)·b = a,
  b·(1/b) = 1, floor ≤ a < floor+1, |a − round(a)| ≤ ½, canonical form
  everywhere.
- **Planted faults** (each red, restored byte-identical): Henrici's second
  gcd skipped (8 checks red, properties from case 0); halves rounded up
  (6 red); sign normalization skipped (first caught by ONE check only —
  the parser can't produce a negative denominator — so the n/(−d) property
  was added, after which the properties catch it from case 0).
- 958 checks green; formator CONFORMIS; Latin lint NOVA 0 with the index
  refreshed (word count rose 8498 → 8503, so the new files were read).

Note: in zsh `$F` holding several paths is ONE word (formator said
"plagula illegibilis"); use `${=F}`.

## 2026-10-05 — first review agent: findings and fixes

No wrong results in 20k differential cases. Acted on:

- **`fractio_aequalis` comparing numerators only passed the suite.** Added
  falsifiability checks (1/2 vs 1/3, 1/2 vs 2/3, −1/2 vs 1/2) and two
  properties: a ≠ a + 1, and a ≠ a/2 for a ≠ 0 (an odd numerator stays the
  same, so only the denominator differs). The first version of the fix was
  caught by ONE check only; with the a/2 property the mutant fails from
  random case 0.
- **Zero-filled Fractio is 0/0, not 0** and propagates silently (0/0 + 1/2 =
  0/0). Fran's decision: document, don't change the representation — the
  header now says always construct with `fractio_ex_*`.
- Null-data chorda test added. gcd memory fix in magnus benefits every
  fractio operation (they all call gcd). 972 checks.
