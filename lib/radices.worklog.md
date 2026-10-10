# radices worklog

## 2026-10-10 - slice 1: real root isolation, exact

API approved by Fran (session sketch): `RadixRealis` = (f squarefree
primitive lc > 0, isolating interval), `radices_reales`,
`radix_ex_fractione`, `radix_compara` (across DIFFERENT polynomials),
`radix_signum_polynomii`, `radix_angusta`, `radix_ad_chordam`. Public
additions to polynomium: derivata, divisor_communis (primitive PRS,
extensio's positive-scale pseudo-remainder), pars_libera, taylor.

**Found first: extensio already isolates roots privately** (Sturm chain,
Cauchy bound, narrowing - for its own ordered fields). What it lacks is
comparing roots of different polynomials (mixed fields refused); that is
this library's reason to exist. Speed: radices isolates all 41 roots of
2T_80(x/2) - 2 in 42 ms; extensio's Sturm COUNT is instant there - the
"34 s at degree 80" in extensio.h is field construction, not isolation.
No speed claim is made against extensio.

**VCA.** Positive roots of f in (0, 2^k), k = bits(max|a_i|) + 1 (2^k >
2 max >= 1 + max|a_i| >= Cauchy). Q(x) = f(2^k x); var((x+1)^n Q(1/(x+1)))
counts (bounds) roots in (0, 1); bisection L = 2^n Q(x/2), R = L(x+1);
L(1) = 0 (coefficient sum) is an exact root at the midpoint. Stack order
right, [midpoint], left gives ascending output with no sort. Negative
roots via f(-t), reversed; 0 via f(0) = 0.

**The invariant I first wrote was false.** "Interval endpoints are never
roots of f" - they can be: an exact midpoint root becomes an endpoint of
its sibling intervals (sqrt3 in (1, 2) where 1 and 2 are roots of
2T_12(x/2) - 2; -5/3 in (-16, 0) where 0 is a root). Bisection then read
sign f(l) = 0 and slid to the endpoint (decimals -1.0000 for -sqrt3).
Fix: f is squarefree, so roots are simple: the sign just inside an
endpoint x is sign f(x), or sign f'(x) (right of x) / -sign f'(x) (left
of x) when f(x) = 0. Bisection and the common-root test use it.

**Two unsigned traps caught by examen ("comparatio vana"),** i32 is
unsigned: a reverse loop `i >= 0` (would wrap and run off the array) and
a digit position. Both s32 now. And `casus` (latina.h: case) as a test
identifier - renamed.

Oracle (scratchpad radices/oraculum.py, Python Decimal 120 digits):
2cos(2 pi k/n) roots of 2T_n(x/2) - 2 for n = 5, 7, 12, 17, 24 (exact
rational roots -2..2 included) and t^7 - 2(50t - 1)^2 (twins 3.2e-8
apart near 1/50, third root 5.48...), 25 digits truncated toward zero.
All equal; every root count equals extensio's Sturm count; adjacent
roots compare strictly ascending.

**Plants (14: 13 red, 1 equivalent).** R1 zero coefficients counted as
signs, R2 Descartes without the shift, R3 midpoint root ignored, R4
stack order, R5 bisection with the endpoint sign, R6 left sign not
negated, R7 containment test inverted, R8 negative decimal without the
mirror, R9 root 0 dropped, R10 common root ignored in signum; P2-P4 in
polynomium (squarefree undivided, Taylor t - c, derivative exponent).
Equivalent: P1 (gcd without its final normalization - every remainder is
already primitive with lc > 0).
Two first SURVIVED and were real gaps:
- R6: no test had a root whose interval ENDS on another root and asked
  for a sign there. Added: sqrt3 from 2T_12(x/2) - 2 in (1, 2), h =
  (t - 2)(t^2 - 3) -> 0, h = (t - 1)(t - 2) -> -1.
- R1: overcounting variations INVENTS roots. A mutant-vs-real run over
  4000 random small polynomials (many zero coefficients) differed on 184:
  2t^4 - 2t^2 + 1 (0 roots, mutant 2), -3t^2 + 2t - 1 (0 vs 1), 2t^3 +
  t^2 + 2t (1 vs 2) are now tests, counts by discriminant.

**A plant was left in the file once:** the first batch ran under
`timeout`, which killed Python before silva.planta's finally - R7's `> 0`
stayed in lib/radices.c, caught by cmp against a scratch backup and
restored. Iteration limits lowered then (VCA depth 4096, bisections
2048; separation 2^-2048 is far beyond use) so a broken path refuses in
seconds instead of grinding for minutes.

Next consumer: exact placement of D97's non-15-degree angles (tan^2 of
10, 20, 25, 35 ... degrees as roots of small integer polynomials from
cos(n t)) against D122's Q(sqrt2, sqrt3) breakpoints - replaces D122's
float margin.

## 2026-10-09 - names fixed before the first commit

`polynomium_taylor` -> `polynomium_translatum` (see polynomium.worklog.md:
the `taylor` glossary entry lowered the English oracle by one permille) and
`_cartesius_unum` -> `_variationes_unum` (Cartesius is a real Latin form;
glossary entries for real forms shadow the oracle). Both glossary entries
removed; no glossary change in this commit.
