# surdus worklog

## 2026-10-09 - slice 1: multi-quadratic surds, exact sign in s64

API approved by Fran (sketch in session): k <= 3 distinct primes fixed
in the struct (D119 needs 2, 3, 5; raising k changes the struct = API
change, decided now), integer s64 coefficients over the basis
sqrt(P_S), value type, no pool. REFUSE, never fall back internally: the
caller chooses the bignum fallback (extensio), so the hot path never
allocates and refusals stay countable. A double filter inside an exact
library was named and approved explicitly.

**Three sign tiers.** (1) zero/rational: free, basis independence. (2)
certified double filter: E = 2 (n + 4) u M (derivation in lib/surdus.c
header: three roundings per term, recursive summation, factor 2 for M's
own rounding; FMA only removes roundings). (3) x = a + b sqrt(p): signs
of a, b recursively (each with the filter); opposite -> sign(a)
sign(a^2 - p b^2), never 0.

**Multiplication guard.** Bit-length precheck bA + bB + bP + k <= 63
selects an unchecked fast path (no overflow possible: 2^k terms each <
2^(bA+bB+bP)); otherwise checked per operation with division and
FALSUM. Plant S6 (threshold 80) is red via the 2^31 sqrt2 squared test.

**Oracle = extensio in Q(cos 2 pi/120)** (contains sqrt 2, 3, 5 and
their products; basis via algebraicus_radix_quadrata - Gauss sums, an
unrelated algorithm). Every VERUM sign is compared with and without the
filter: 2172 cases, 61 decided in tier 3, 768 refused, 0 disagreements.

**Pell units force tier 3 on purpose:** x - y sqrt d = 1/(x + y sqrt d)
is tiny while the coefficients are large, so the filter must yield; the
true sign is known (+). Small ones (x < 2^31, squares < 2^62) are
asserted DECIDED (35), so refusing too much is caught too. Products and
differences across sqrt2/sqrt3/sqrt5 drive the recursion to depth 3.

**Plants (13 red).** Three lessons:
- S2 "roots off by 1e-12" first SURVIVED: it scaled sqrt(1) too, and a
  uniform positive scale never changes a sign - the plant was wrong.
  S2b (irrational roots only) is red exactly as predicted: tier 2 said
  -1 on a Pell unit whose sign is +1. Evidence the bound is what keeps
  the filter honest.
- S11 "drop the -2^63 guard in subtract" SURVIVED by undefined
  behaviour (negating -2^63; clang happened to refuse anyway). No test
  can pin UB, so the CODE changed: `_subtrahe_tuta` checks a - b
  directly, no negation, no special case.
- Guard plants that REMOVE a check land in UB too; the sound plants make
  a guard one step too STRICT (S7b, S11b), killed by new tests that
  accept results exactly at the s64 limits.
- S9 first did not compile (removing the prime check left _est_primus
  unused under -Werror); re-planted keeping the call.

Lint: subset -> selectio, ampl -> amplitudo, ia/ib/ir -> imago_*,
xn/yn -> x_proximus/y_proximus.

Next: D120 uses surdus for D119's exact path (benchmark + regression:
D119's counts reproduced), then the zeta_8 derived-series audit.

## 2026-10-09 - review (recensor-extensio, 102a393f)

Verified sound by the reviewer: the filter bound (incl. |c| > 2^53, FMA
contraction; ~29,700 signs placed within +-3E of zero under -O2, ASan
and -ffp-contract=fast, 0 wrong), the tier-3 split and recursion, add/sub
refusing exactly at overflow; 20,249 fuzz signs over 9 prime spaces vs
an exact oracle, 0 wrong.

- S1 (MEDIUM, UB): `_est_primus` formed d*d in s32 before the 2^15 bound
  was checked - surdi_spatium({2^31 - 1}) overflowed (46341^2). Now the
  bound comes first and the loop runs d <= p/d. Test: 2^31 - 1 refused.
- S2 (MEDIUM, test gap): dropping k from the fast-path precheck survived
  the suite and is real UB: all 8 coefficients 2^29 - 1 over {2, 3, 5}
  give 29 + 29 + 5 = 63 without k, yet the product overflows. That
  product is now a must-refuse test; plant "0 * k" red.
- S3 (LOW): coefficients with S >= 2^k were read as 0 by signum and
  multiplica but as nonzero by est_nullum. multiplica, signum and
  compara now refuse them; adde/subtrahe/scala (no space argument) only
  carry them. Plant red.
