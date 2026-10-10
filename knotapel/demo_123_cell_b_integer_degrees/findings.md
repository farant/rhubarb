# Demo 123: Cell B at Every Integer Degree, Placed Exactly — Findings

D122 computed Cell B's capacity at every half-angle in (0°, 90°], with
every region boundary in closed form. It placed only D97's multiples of
15° into that diagram exactly. The other 15 of D97's 21 angles went in
by a float margin of 10⁻⁹ over libm's tan, which D122's findings called
"a margin, not a proof".

This demo removes the float from placement entirely and extends it to
**every integer degree 1°–90°**. It is the first consumer of the house
root isolator `radices`.

Run: `./bin/aedilis knotapel/demo_123_cell_b_integer_degrees/main.c &&
bash build/aedilis/main/struere.sh && ./build/aedilis/main/main`.
It takes about 5 s and runs 19 checks.

## Method: identification by position

S_N(u) = Σ_l C(N, 2l+1)(−1)^l u^l has exactly the real roots
tan²(kπ/N) for k = 1 … N/2 − 1. They are distinct and increase with k.
(tan(Nt) = 0 ⇔ Im(1 + i tan t)^N = 0; divide by tan t, put u = tan²t.)

- **Integer degrees:** tan²(k°) is root number k of S₁₈₀, which has
  degree 89. The root is identified by its position among the isolated
  roots, not by a float near it. The demo checks that there are exactly
  89 roots, that they increase, and that k = 30, 45, 60 equal 1/3, 1
  and 3 exactly.
- **Breakpoints:** u* = n₀² tan²(jπ/24)/r is root number j of
  S₂₄(ru/n₀²), cleared to integers. The scaling by n₀²/r > 0 keeps the
  order.
- **Placement:** each degree is placed by `radix_compara`, using a
  binary search over the 132 distinct breakpoint values. Equality
  (15°, 30°, 45°, 60°, 75°) is decided by a common factor of the two
  polynomials, never by interval width.

Parts A–C and E are D122's, unchanged; D122 itself is not modified.
Part D is the exact placement, and Part F the cross-check and the
integer-degree claims.

## Cell B at every integer degree

| degrees | rule N = 3/4/5/6 | robust | possible |
|---|---|---|---|
| 1–4 | 0 3 0 0 | 0 3 0 0 | 8 15 0 0 |
| 5 | 8 3 0 0 | 8 3 0 0 | 8 15 0 0 |
| 6–7 | 8 3 0 1 | 8 3 0 1 | 8 15 0 1 |
| 8–10 | 20 3 0 1 | 20 3 0 1 | 20 15 0 1 |
| 11–21 | 20 15 0 1 | 20 15 0 1 | 20 15 0 1 |
| **22–33** | **20 15 6 1** | **20 15 6 1** | 20 15 6 1 |
| 34–38 | 20 15 0 1 | 20 15 0 1 | 20 15 0 1 |
| **39–79** | **20 15 6 1** | **20 15 6 1** | 20 15 6 1 |
| 80 | 20 15 6 0 | 20 15 6 0 | 20 15 6 0 |
| 81–82 | 20 3 6 0 | 20 3 6 0 | 20 3 6 0 |
| 83–84 | 20 0 6 0 | 20 0 6 0 | 20 0 6 0 |
| 85 | 20 0 0 0 | 20 0 0 0 | 20 0 0 0 |
| 86–87 | 12 0 0 0 | 12 0 0 0 | 20 0 0 0 |
| 88–89 | 0 0 0 0 | 0 0 0 0 | 8 0 0 0 |
| 90 | 0 0 0 0 | 0 0 0 0 | 20 15 6 1 (vacuous, D122) |

15°, 30°, 45°, 60° and 75° lie exactly **on** breakpoints. At each of
them the signature equals that of both neighbouring degrees.

## The claims, at integer resolution

- **D97's "35° resonance"** is, at integer degrees, exactly **34°–38°**.
  There N = 5 fails robustly and under every tie resolution. It holds
  at 33° and at 39°.
- **Robust 100% at every N** holds at exactly the integer degrees
  **22–33 and 39–79**. These are D122's two plateaus, (21.552°,
  33.355°) and (38.794°, 79.453°), read at integers.
- **D97's float sweep equals the exact rule** at all 21 of its angles,
  now with no margin anywhere.
- **D122's float placement was right but unproven.** On k = 1…89 it
  agrees with the exact placement at 84 degrees. It refuses at 5: the
  multiples of 15°, which lie exactly on breakpoints. It differs
  nowhere. The demo checks that the refusals are exactly the on-
  breakpoint degrees.
- **Nothing was fragile.** The closest any integer degree comes to a
  breakpoint value is a relative gap in u of 1.0 × 10⁻³, at 69°. Next
  are 37° (3.0 × 10⁻³), 53°, 10° and 29°. These gaps are from the
  oracle and are informational only; the placements are exact.

## Cross-checks

- **Two exact sorts agree.** All 11,781 pairs of the 154 breakpoints
  are compared by radices: root isolation on scaled S₂₄, integer
  arithmetic. The signs equal D122's `surdus` order, from signs in
  Z[√2, √3], including every equal-value merge. The two methods share
  no arithmetic.
- **Radices equals surdus at the multiples of 15°.** Placement through
  S₁₈₀ matches D122's `place_exact` placement through Z[√2, √3].
- **Oracle:** `python3 -I oracle.py` re-derives the 14 (|n₀|, r) pairs
  from Cell B's masks. It computes every value with 100-digit Decimal
  (no radices, no surdus, no S_N) and places all 89 integer degrees.
  Its positions are embedded as constants and equal the demo's at
  every k. It also reports the gaps above. Its equality test (10⁻⁸⁰)
  is sound for an exact reason (review P2). Every breakpoint lies in
  Q(√2, √3), and tan²(k°) lies in Q(√2, √3) only when 15 | k. So off
  the multiples of 15 equality is impossible, and on them it holds
  exactly.
- **What the pairwise check does not cover:** both sorts take the same
  breakpoint list (collect_breakpoints). So it does not revalidate that
  the list is complete; D122's review covered that. It does validate
  the order that place_degree relies on.

## Plants (7)

**Red (6):**
- the n₀²/r scaling swapped;
- the degree roots taken in reverse order;
- equality ignored in placement;
- the binary search's `hi = mid − 1`;
- the rank representative shifted to the next breakpoint;
- S_N's alternating sign pattern changed (fatal: S₁₈₀ no longer has 89
  real roots).

**Survives (1): radices' H1 bug reintroduced in the library.** H1 was
fixed in 2b71cfed: `radix_compara` returned 0 when b was an exact root
of a nonlinear f and a was another root of that f. The plant survives,
and the pre-fix library passes 19/19 at every pre-narrowing tried
(none, 2⁰, 2⁻¹, 2⁻², 2⁻⁴; checked by the review). H1 **cannot fire in
this family**.

Exact roots are not rare here. build_roots yields 24 of them, all with
nonlinear f:
- 45° = 1 and 60° = 3, with f = S₁₈₀;
- 22 breakpoints with dyadic-reachable rational values (1/2, 1, 3/2, 2,
  3, 4, 6, 8, 9, 12, 24, 25, 27, 75), with f a scaled S₂₄.

H1 needs an inexact x that is a root of an exact y's polynomial, with
y strictly inside x's isolating interval. The structure rules that out:
- **Between breakpoints:** T(j) = tan²(jπ/24) is irrational except at
  j = 6, 8 and 4 (values 1, 3 and 1/3). So for an irrational value,
  sharing a root with f_y forces the same ratio n₀²/r, hence the same
  primitive polynomial, hence the same isolation. y is then a root of
  f_x too, and cannot lie strictly inside x's interval.
- **Between a degree and a breakpoint:** tan²(k°) lies in Q(√2, √3)
  only when 15 | k, and those values are themselves roots of S₁₈₀.

The review checked this exhaustively over all (inexact x, exact y)
pairs of the 243 roots:
- at 2⁻²⁴ and at 2⁻¹: no y strictly inside any x;
- with no pre-narrowing: 12 such pairs, 4 with x a root of f_y. All 4
  are equal values (breakpoint 3 against 60° = 3), where 0 is the
  correct answer.

The library's own tests cover H1; this demo does not, and structurally
cannot. (A first version of this paragraph said "the only exact point
is u = 1". That was false, as the review found; the conclusion stands.)

## Cost, and a radices finding

The run takes about 5 s, against D122's 0.15 s. Isolating S₁₈₀ takes
4.2 s of that. Peak memory is 3.1 GB (4.2 GB with pre-narrowing at
2⁻⁴⁸): nothing allocated during isolation is freed.

A first version pre-narrowed every root to 2⁻²⁰⁰ and took 48 s.
Profiling showed nearly all of it in `radix_angusta` → `_seca_medio` →
`polynomium_valor`. That is degree-89 evaluation in `Fractio`
arithmetic at dyadic points, with a gcd on every multiply. The demo now
pre-narrows to 2⁻²⁴; `radix_compara` narrows further on demand. At 2⁻⁸
the run also takes 4.9 s, and at 2⁻⁴⁸ it takes 6.3 s.

**Recorded for radices:**
- evaluation at a dyadic point m/2^e can be done in integers,
  Σ cᵢ mⁱ 2^(e(d−i)), with no gcds;
- half-degrees (S₃₆₀, degree 179) ran out of memory in a probe, and
  integer degrees already peak at 3.1 GB. The piscina keeps every
  bisection's garbage.

## Not covered

- Half-degrees and finer (see above).
- Placement of angles that are not rational multiples of π. Any angle
  whose tan² is a real algebraic number works the same way: give its
  polynomial and its root's position.
