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

## 2026-10-09 - review (recensor-extensio, 4680ff65): H1 M1 L1 L2 fixed

**H1 (HIGH): radix_compara returned 0 for DIFFERENT numbers.** When b was
exact, the loop took `signum(b.f, a) == 0` to mean a == b ("f_b linear").
But exact points come from dyadic midpoints that land on a root (in
radices_reales, radix_angusta, the internal bisection), and those keep the
FULL f. So b.f(a) = 0 for ANY root of b.f. Repro: sqrt2 from t^2 - 2
against exact 1 from (t - 1)(t^2 - 2) gave 0. Fix: when b is exact, the
answer IS sign(a - q) = sign of (den t - num) at a, with a single call and
no loop. The in-loop exact branch is dead and removed. Reviewer fuzz: 13
of 9797 queries were wrong before the fix, 0 of 10242 after (their
one-line variant). Lesson: "infra == supra" says only "rational root",
never "f is linear".

**M1: radix_ad_chordam refused non-dyadic terminating decimals** (1/5,
-3/10). When r = k/10^d exactly, `infra < r <= supra` holds forever, so
supra * 10^d <= m + 1 never fires; it gave up after 2048 bisections. Fix:
before each bisection, test c = (m+1)/10^d strictly inside (infra, supra)
with f(c) = 0, and if so the root is exactly c. The strict `c < supra` is
an EQUIVALENT mutant against `<=`: c == supra means supra * 10^d == m + 1,
and the termination check returns first.

**L1:** digiti = 0 now prints "1", not "1.". "-0.00" for -0.003 stays
(documented): the sign is information, the value lies in (-0.01, 0).
**L2:** the struct comment now states that the implication runs one way
only (1/5 from 5t - 1 comes back as an interval) and that the root sits
in the OPEN interval. **P1:** documented that ad_chordam refuses beyond
about 600 digits (LIMES_BISECTIONUM). Every limit path refuses and never
answers wrong (reviewer checked: Mignotte a = 3^440 -> FALSUM at depth
4096). **P2** (s == 0 branch cannot clear a non-dyadic endpoint of b
that is a root of f_b; only reachable with a hand-built RadixRealis):
not fixed; noted here.

Plants: H1 (old b.f path), M1 (check off), L1 (point always): all red.

Independent re-check of THIS fix (not the reviewer's variant), using the
reviewer's harness (hrad.c + Sturm/Fraction oracle orac.py) built against
the tree in the scratchpad. fuzz_cmp seeds 1-6 x 60: 15029 queries, 0
wrong. fuzz_roots seeds 1-2 x 90: 928 roots, 0 wrong, 0 refused, 0
decimal refusals. Same seeds on the pre-fix binary: 13 wrong compares
(seeds 1-3) and 6 decimal refusals (seed 1), so the fuzz still detects
both bugs.

## 2026-10-10 - first consumer (knotapel D123): cost profile and two parks

D123 isolates S_180(u) = sum_l C(180, 2l+1)(-1)^l u^l (degree 89; 89
real roots tan^2(k pi/180)) in 4.2 s, and 154 scaled S_24 isolations
plus 12,499 compares in under 1 s. A first version narrowed all 89 roots
to 2^-200 and took 48 s. `sample` put nearly all of it in radix_angusta
-> _seca_medio -> polynomium_valor -> fractio_multiplica ->
magnus_divisor_communis: evaluation at a dyadic point in Fractio
arithmetic, with a gcd on every multiply.

**Park 1 (speed):** evaluate f at m/2^e in integers as
sum c_i m^i 2^(e(d-i)) (only its sign is needed, so no gcds at all).
Every point VCA and _seca_medio evaluate is dyadic, except exact rational
roots from radix_ex_fractione and decimal tests in ad_chordam.

**Park 2 (memory):** S_360 (degree 179) killed the probe (exit 137),
with all garbage kept in one piscina. Candidates: a scratch piscina per
isolation (copy the results out, then reset it), or fewer temporaries
in _seca_medio.

Observed in D123: H1 (2b71cfed) cannot fire there, because the only
exact point (u = 1) is a root of every polynomial whose intervals touch
it. Consumer demos are no substitute for the library's own H1 test.

## 2026-10-10 - correction to the entry above (D123 review L1, L2)

"The only exact point (u = 1)" was FALSE. D123's isolations yield 24
exact roots, all with nonlinear f (45 and 60 degrees from S_180; 22
breakpoints with values such as 1/2, 3/2, 2, 3, 9, 75 from scaled
S_24). Dyadic rational roots routinely become exact: never assume an
exact RadixRealis has a linear f (that was H1 itself).

H1 still cannot fire in D123, for a structural reason. Values sharing
an irrational root force the same primitive polynomial, so the
isolations are identical. tan^2(k degrees) is in Q(sqrt2, sqrt3) only
when 15 | k, and those values are roots of S_180. The reviewer checked
every (inexact, exact) pair exhaustively.

Park 2 has an addendum: integer degrees (S_180) already peak at 3.1 GB
RSS (4.2 GB when pre-narrowed to 2^-48).

## 2026-10-10 - parks 1 and 2 done: integer signs, in-place Taylor shift, scratch arenas

Probe: S_N(u) = sum C(N, 2l+1)(-1)^l u^l (roots tan^2(k pi/N)), isolate,
then 10-digit decimals of every root, -O2:

| N (degree) | before | after |
|---|---|---|
| 120 (59) | 0.55 s, 864 MB | 0.14 s, 8 MB |
| 180 (89) | 4.05 s, 3.85 GB | 0.79 s, 32 MB |
| 360 (179) | killed (out of memory) | 23.7 s, 450 MB, all 179 agree with libm |

D123 (live build): 4.8 s / 3.12 GB -> 1.37 s / 33 MB, output byte-identical.

Three changes:
1. `_signum_ad`: sign of f(p/q), q > 0, as a homogeneous Horner in
   integers, sum c_i p^i q^(d-i). There are no gcds; Fractio Horner
   took a gcd on every multiply. `_signum_iuxta` uses it for f and f'.
   This cut memory (3.85 -> 2.98 GB), not isolation time, which was the
   Taylor shift.
2. `polynomium_translatum` (polynomium.c) is now the in-place Taylor
   shift, a_j += c a_(j+1) for i = 0..n-1, j = n-1..i, with additions
   only when c = 1 (VCA). It used to be Horner through
   polynomium_multiplica, building a new polynomial at every step.
   Isolation at N = 180 went from 4.1 s to 0.92 s.
3. Scratch arenas.
   - Every public function works in its own piscinae (`status` holds
     surviving endpoints; `opus` is reset after every iteration). Only
     results are transcribed into the caller's piscina, and
     compara/signum allocate nothing there.
   - VCA (`_separa_intra`) gives each stack entry a mark (PiscinaNotatio)
     taken before its q and c are transcribed. Popping an entry resets
     the stack arena to its mark: entries above it are already popped,
     and entries below it were allocated earlier. This is safe because
     the children are computed in `opus` first, then the stack is reset,
     then the children are transcribed. Memory is now depth x one
     polynomial, not the sum of all nodes.

Guards:
- `venenum` gate: radices added (sanitizers + 0xA5 fill after reset).
- Test section MEMORIA: the caller's piscina holds 1,880 bytes after a
  degree-24 isolation (1,355,208 with the old code, red). Compare (now
  including a cross-polynomial pair that needs the narrowing loop) and
  signum leave usage unchanged. Narrowing by 200 bisections and a
  60-digit decimal each add < 1 KB.

Plants (9, all red):
- V1 stack entry not transcribed; V2 bisection endpoint not
  transcribed; V3 stack reset before the children are computed; V4
  angusta endpoints not transcribed; V5 root f left in the scratch
  arena: all caught by venenum (0xA5 loads / sanitizer).
- M1 compara working in the caller's piscina: radix suite (memory
  test). First SURVIVED, because both compares in the test were decided
  by disjoint intervals with no loop. Added the sqrt3 vs
  sqrt(2.999999) compare.
- S1 Horner power of p instead of q: radix suite.
- S2 shift with c always 1, S3 shift with j > i: polynomium suite.

Remaining cost at N = 360 (450 MB) is mostly the opus arena of one
degree-179 Taylor shift (d^2/2 big additions) plus the stack of deep
polynomials; not chased further.

## 2026-10-10 - review II (recensor-extensio, e52e2f3f): L1 fixed, P1 documented

No H/M findings, and no lifetime bug, by reading and under fuzz on an
ASan + UBSan + PISCINA_VENENUM build:
- roots: 1,398;
- compares: 10,242;
- gcd: 1,600;
- translatum vs exact binomial shift: 3,000;
- _signum_ad vs Fraction: 3,000.

The reviewer confirmed LIFO soundness (children are pushed only after
the reset) and piscina_reficere across alvei.

**L1: MEMORIA guarded only the caller's piscina, not the scratch
arenas** - which is where the gain is. Three plants stayed green: T1 VCA
without the opus reset (S_180: 38 MB -> 1.05 GB), T2 no stack reset
before the children, T3 angusta without the opus reset.
Fix: include/radices_interna.h (house *_interna.h pattern, NOT API)
exposes radicum_apices_purgare / radicum_apex_acervi /
radicum_apex_officinae. These are the high-water marks
(piscina_summa_apex_usus), recorded when each scratch piscina is
destroyed.

The test isolates S_120 (degree 59), built in the test from binomials.
Byte counts are deterministic, so the bounds can be tight:

| measure | now | bound | under its plant |
|---|---|---|---|
| stack | 3,937,096 | < 4.5 MB | 5,692,220 (T2) |
| work | 2,400,116 | < 6 MB | hundreds of MB (T1) |
| angusta (200 bisections, degree 59), work | 360,064 | < 1 MB | over it (T3) |

All three plants are now red.

**P1:** documented in radices.h. FALSUM covers only the failure to
create a scratch piscina; an allocation failure inside one is fatal, as
with every piscina.

**P2 (noted, not changed):** every public call creates 2 dynamic
piscinae (4 for compara with an exact b, through the public signum),
and _separa_intra allocates the full 3 x 4098 Opus stack (~0.7 MB). That
is fine at D123's scale (12k compares in 1.4 s). If a consumer ever
makes millions of compares, pass arenas down (an _intra variant of
signum) or size the stack from the degree.

Reviewer's measurements:
- D123: 4.8 s / 3.12 GB -> 1.64 s / 34 MB;
- S_120: 679 MB -> 10.5 MB;
- Mignotte a = 3^350: 31.9 s / 1.03 GB -> 22.0 s / 16 MB;
- a = 3^440: still refuses cleanly (19 MB).
