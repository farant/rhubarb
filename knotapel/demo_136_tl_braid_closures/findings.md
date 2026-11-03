# Demo 136: Brackets of Large Braid Closures through Temperley–Lieb Modules — Findings

Plan: `plan.md` (v1 approved by Fran 2026-10-28; v2 after a DESIGN
REVIEW before any data, consiliarius-d136; T0 and T1 amendments).
Context: project-specs/crossings-contraction-counting-brainstorm.md §12.3
(invariants of large knots), §11 levers (lever 2, fewer primes, was
taken in contractio T5a: the Tait-graph bound);
tools/uncinus_mensura.c section V (the vertex model's projections:
12 × 801 ≈ 32 days).

The Kauffman bracket of a braid closure is a weighted sum of the braid's
traces in the Temperley–Lieb standard modules W_{n,j}, whose dimensions
square-sum to Catalan(n) (208,012 at 12 strands) instead of the vertex
model's ~2^(2n) states. The demo evaluates that sum at points mod primes
near 2^32 and rebuilds the exact bracket with the same interpolation the
vertex model uses.

**Every registered prediction holds: 692 checks, 0 failures.** The TL
bracket equals the state sum (c ≤ 24) and the vertex model in full
wherever the vertex model can run (every braid up to 12 × 201, and
8 × 401, 8 × 801), and passes the off-interpolation, Jones-identity and
Markov-move checks at the large sizes where it runs alone.

**The headline is 12 × 801: the exact Jones polynomial in 78 CPU-minutes**
(4,700 s of `clock()` time for the bracket, 12,949 evaluations = 23
primes × 563 points, 0.36 s per point), where the vertex model was
projected at about a month. The polynomial itself, with every other
exact bracket of the test set, is in `brackets.txt` (see Run). Per point the TL route is 634× faster
than the vertex model there (registered: ≥ 100×), and 86× at 12 × 401
(registered: ≥ 25×).

## Result

1. **The TL closure formula is right in this code** [checked]. R1 (60
   comparisons with the state sum and the vertex model, every one
   exact), R1p (TL value = vertex value = the reduced exact polynomial
   at points of p* that the interpolation never sees), R4 (Jones
   identities including |V(−1)| = det by two independent determinant
   routes), R5 (stabilization front and back; index flip and
   conjugation as smoke tests, since both follow from trace cyclicity),
   R2 and R3 (link-state counts and the TL and braid relations on every
   basis state, n ≤ 16), conv0 (σ ↦ A·1 + A⁻¹·e, the standard bracket).
2. **Cost per point is 1.36–1.64 ns × c × Catalan(n) on a quiet
   machine** [measured, n = 8 … 16 at c = 801; descriptive]. 14 strands:
   3.3 s per point (T0 projected 4.6 s); 16 strands: 43 s per point (T0
   projected 63 s). Under load it is worse: the same 12 × 801 braid ran
   at 0.227 s per point alone, 0.363 s (2.2 ns) as T2's bracket mean, and
   0.527 s in T2's R1p timing. At ~13,000 points and the quiet figure, a
   full 14 × 801 bracket is ~12 h and 16 × 801 ~6.5 days; that is T4
   territory. Stopping early when the answer looks stable is excluded by
   the exactness rule (brainstorm §11), so the levers left are the
   kernel (the in-place normalized update, a division-free modular
   multiply) and spreading the points over processes.
3. **The vertex model is faster on short braids with many strands**
   [observed]. Full brackets, TL against vertex (T2):
   - c ≈ 200: 8 × 201 0.41 s vs 21.8 s; 9 × 202 1.89 vs 16.0; 10 × 201
     4.77 vs 15.3; 11 × 202 18.1 vs 7.49; 12 × 201 51.7 vs 5.65;
   - c ≈ 100: TL faster for n ≤ 8 (8 × 101 0.06 vs 0.34), vertex from
     n = 9 (9 × 102 0.22 vs 0.09; 12 × 101 8.28 vs 0.05).
   The vertex model's cost follows the width of its contraction order
   (14 at 12 × 201, `build/nodi_magni/coniectura.out`; 16 at 8 × 201,
   whose 4× more states predict 22.6 s against the measured 21.8; 26 at
   12 × 801, R0), the TL route's follows Catalan(n) whatever c is. So
   the crossover depends on n and on each braid's width; at 12 strands
   it lies between c = 201 and 401.
4. **The coefficient bound is ~3.7× loose** [measured]. Largest
   coefficient against the Tait-type bound the interpolation uses:
   175 / 654 bits at 12 × 801, 193 / 642 at 8 × 801, 91 / 333 at
   12 × 401, 42 / 165 at 12 × 201. Every prime past the true size is
   paid for (no early stop); a tighter a-priori bound would cut the
   primes, not the points, by at most that factor: 175 bits plus sign is
   6 primes against 23 at 12 × 801. No rigorous a-priori bound will be
   that tight, so this is a ceiling: the slack left after T5a's
   Tait-graph bound is cancellation, which no counting bound can see
   (brainstorm §11). The TL entries themselves barely cancel (Result 5),
   a contrast for §12.1.2's sign-problem meter.
5. **Exact arithmetic without primes stays small** [measured;
   descriptive]. Carrying the traces over Z[A²] (each letter scaled by
   A³ so entries stay integer polynomials), the largest intermediate
   coefficient is within a few bits of the final bracket's: 39 vs 39
   bits at 8 × 201, 27 vs 19 at 12 × 101, 44 vs 42 at 12 × 201, 192 vs
   193 at 8 × 801 (the largest gap, 8 bits, at 12 × 101). Through
   12 × 201 every coefficient fit in 64 bits (no big-number allocation
   at all); 8 × 401 needed 68 bits, 8 × 801 192 bits, and the 12 × 801
   bracket itself has 175-bit coefficients. At the single integer point
   A = 2 the entries grow ~3.7 bits per letter (up to 3 of them the 8^c
   scaling): 2,933 bits at 12 × 801, 105 s. So a prime-free exact route
   would use fixed-width multi-word arithmetic (≈ 200 bits, 3 words at
   c = 801; one word only through about c = 200): no CRT, no bound. It
   is not a clear win. Its cost is the per-point work times the live
   coefficients per entry (up to 3c + 1) times the words, against the
   modular route's per-point work times ~13,000 evaluations, so the gain
   is at best a few-fold (estimate). As written here (one `magnus` per
   coefficient, dense) it was slower at every size it ran: 4–6× at 12
   strands (12 × 201: 194.5 s against 51.7 s; 12 × 101: 48.7 s against
   8.3 s) and 1.1× at 8 × 801 (25.2 s against 23.4 s). The dense buffers
   cost memory too (the polynomials run peaked at 4,966 MB at 12 × 201,
   of which the two buffers account for ~2.6 GB).
6. **No random closure is reduced alternating** [descriptive, as the
   plan expected]. span V / c lies between 0.14 and 0.46 over the 45
   test braids (0.39 at 8 × 801, 0.32 at 12 × 801), around a third at
   the long lengths, except 11 × 20 (span 0); c − span is never 0.
   11 × 20 is the unknot: its braid reduces to one strand by Markov
   moves (the T3 reviewer's `unknot.py`), and V = 1 as it must be.

## Registered predictions

| | Prediction | Verdict |
| --- | --- | --- |
| R0 | test braids = the projection run's (bound, width; 4 fingerprints) | HOLDS (4 of 4) |
| R1 | TL bracket = state sum (c ≤ 24) and = vertex model (full) | HOLDS (60 of 60) |
| R1p | TL = vertex = reduced exact TL polynomial at points of p* | HOLDS (3 value checks over 16 + 8 + 4 = 28 points, plus 3 assertions that p* is below every interpolation prime) |
| R2 | link-state counts, validity, Σ d² = Catalan(n), n ≤ 16 | HOLDS (16 of 16) |
| R3 | TL and braid relations on every basis state, n ≤ 16 | HOLDS (15 of 15) |
| R4 | V(1) = 1, V(e^{2πi/3}) = 1, V(i) = ±1, \|V(−1)\| = det | HOLDS (292 of 292) |
| R5 | stabilization (back, front), index flip, conjugation, at points | HOLDS (294 of 294; the 4 stabilization checks per braid carry it, flip and conjugation are smoke tests) |
| R6 | TL per point ≥ 25× (12 × 401), ≥ 100× (12 × 801) faster than vertex; full 12 × 801 bracket ≤ 8 h CPU | HOLDS (86.4×, 634.5×; 4,701 s CPU) |
| conv0 | trefoil against the state sum at points; one-strand trivial braid = 1 | HOLDS (3 of 3) |

R4 is blind to mirror images (V(t) and V(1/t) pass alike); R1 and R5's
stabilization factor are not. At c = 801 the determinant comes only from
the colouring-matrix route (67 primes at 12 × 801, 171 bits), which
shares nothing with the TL code or the vertex model; for c ≤ 401 it
agrees with `laqueus_alexander_ex_pd`. R6's ratios move with machine
load by up to ~2× (see Result 2); the margins (3.4× and 6.3× over the
thresholds) absorb that. R6 compares CPU time per call at the same
points of p*, interleaved.

Faults that break the interpolation's consistency (wrong writhe class,
a bound too small, a letter dropped) surface as an interpolation
refusal, booked under R1, at every length; the T3 reviewer showed both
(see Review). R1p and R4 are the checks for faults that leave the
values consistent.

## What was seen before the run

- T0 (no bracket): link-state enumeration, kernel speed (~1 ns per
  multiply-add), determinant timings.
- T1 (`D136_MODUS=t1` and `p8`): the small set (n ≤ 6 at lengths 20
  and 50, the named knots, R1p smoke at 8 × 101) and 10 × 401, whose
  bracket took 27 s with largest coefficient 82 bits. So 10 × 401's
  bracket was seen; its R1p comparison and every 12-strand result were
  not.
- Nothing at 12 × 401 or 12 × 801, in any route, before T2.

## Checks (692, all pass)

R0 4, R1 60 (14 state sum + 46 vertex), R1p 6, R2 16, R3 15, R4 292
(47 braids × 6 + the two c = 801 braids × 5, no Alexander route there),
R5 294 (49 × 6), R6 2, conv0 3. R5 at each braid uses (summus − imus)/4
+ 3 points of p* (563 at 12 × 801). Each run asserts the lowest
interpolation prime it used is above p*. R0 records its checks but does
not stop the demo on failure as v2 said; moot, since it passed.

## Plants (silva.planta, T1, all eight as the plan stated)

1 case (c) → 0: R1, R3, R4, R5 red; 2 Δ recurrence with +: R1, R4, R5,
conv0; 3 σ⁻¹ given σ⁺'s image: R1, R4, R5 (4_1 red); 3b global swap: R1,
R4, R5, conv0 (4_1 green, as registered); 4 /δ dropped: R1, conv0;
5 enclosed defects admitted: R1, R2, R4, R5 (rerun after T3, see
below); 6 one coefficient + 1: R4; 7 conjugating pair not inverted: R5;
8 value negated when n ≥ 10 and c > 300 (p8 mode): R1p, R4 red, R1
green. Extra reds beyond the registered minimum are listed in plan.md's
T1 amendment.

Two corrections from T3:
- **Plant 8's fault is not v2's.** v2 registered "one letter skipped"
  with R1 green, but that fault flips the writhe class, so the
  interpolation refuses at the first prime: R1 goes red, and R1p and R4
  never run on that braid (the reviewer planted it: `TL bracket:
  REFUSED, 277 evaluations`). The harness planted "value negated"
  instead, without listing it as a deviation. Recorded now in plan.md's
  T1 deviations.
- **Plant 5's first run was killed** (rc 137, cause unknown, probably the
  same as plant 8's restore kill) and the harness scored it as a
  success: R2 was red, and the only expected-green check (R0) runs
  late, so it never ran. The harness now treats a run that is killed or
  prints no verdicts as inconclusive. Rerun through it
  (`build/d136/plantae5.log`): a complete run, rc 1, red R1, R2, R4,
  R5, R0 green, UT DICTUM; the same red set as the reviewer's own
  complete run.

## Descriptive (no prediction; `D136_MODUS=desc`, run after T2)

Added after the registered run in a separate mode, so none of it is in
the 692. Times printed only in this mode or with `D136_TEMPORA=1`.

**Per-point time at c = 801** (3 points of p*, alone on the machine):

| n | Catalan | largest module | s per point | ns per c × Catalan |
| --- | ---: | ---: | ---: | ---: |
| 8 | 1,430 | 28 | 0.002 | 1.53 |
| 10 | 16,796 | 90 | 0.019 | 1.41 |
| 12 | 208,012 | 297 | 0.227 | 1.36 |
| 13 | 742,900 | 572 | 0.980 | 1.64 |
| 14 | 2,674,440 | 1,001 | 3.340 | 1.56 |
| 15 | 9,694,845 | 2,002 | 11.891 | 1.53 |
| 16 | 35,357,670 | 3,640 | 43.341 | 1.53 |

(n = 4 … 7 are below the clock's resolution to be meaningful.)

**Exact traces without primes** (run concurrently with the span run):

| braid | Z[A²]: max coefficient bits, intermediate / bracket | Z at A = 2: max entry bits (scaling ≤ 3c) | time Z[A²] / Z / TL bracket |
| --- | --- | --- | --- |
| 8 × 201 | 39 / 39 | 744 (603) | 1.1 s / 0.0 s / 0.41 s |
| 10 × 101 | 26 / 24 | – | 4.0 s / – / 0.67 s |
| 12 × 101 | 27 / 19 | – | 48.7 s / – / 8.3 s |
| 12 × 201 | 44 / 42 | 736 (603) | 194.5 s / 7.2 s / 51.7 s |
| 8 × 401 | 68 / 67 | 1,429 (1,203) | 4.6 s / 0.2 s / 2.7 s |
| 8 × 801 | 192 / 193 | 3,083 (2,403) | 25.2 s / 0.7 s / 23.4 s |
| 12 × 401 | – | 1,483 (1,203) | – / 26.3 s / 578 s |
| 12 × 801 | – | 2,933 (2,403) | – / 105 s / 4,701 s |

Every exact trace, both routes, reproduces the modular TL value at A = 2
mod p*. The polynomial route was planned for c ≤ 201; 8 × 401 and
8 × 801 go past that because they turned out cheap at 8 strands. The A =
2 column is one point: a full bracket that way would need ~(summus −
imus)/4 such points, or the polynomial route.

**Span of V against c** (span of V in t; c − span = 0 iff reduced
alternating; requested lengths, the generator bumps parity):

| n | c ≈ 20 | c ≈ 50 | c ≈ 101 | c ≈ 201 | longer |
| --- | ---: | ---: | ---: | ---: | --- |
| 3 | 6 / 20 | 14 / 50 | 24 / 102 | 50 / 202 | |
| 4 | 7 / 21 | 12 / 51 | 28 / 101 | 62 / 201 | |
| 5 | 5 / 20 | 13 / 50 | 27 / 102 | 66 / 202 | |
| 6 | 3 / 21 | 21 / 51 | 40 / 101 | 81 / 201 | |
| 7 | 6 / 20 | 12 / 50 | 39 / 102 | 63 / 202 | |
| 8 | 3 / 21 | 13 / 51 | 46 / 101 | 67 / 201 | 116 / 401, 315 / 801 |
| 9 | 5 / 20 | 15 / 50 | 31 / 102 | 75 / 202 | |
| 10 | 8 / 21 | 21 / 51 | 37 / 101 | 66 / 201 | 128 / 401 |
| 11 | 0 / 20 (V = 1) | 10 / 50 | 34 / 102 | 72 / 202 | |
| 12 | 7 / 21 | 9 / 51 | 39 / 101 | 67 / 201 | 137 / 401, 259 / 801 |

The largest coefficients repeat the T2 run's exactly (same braids, same
brackets).

## Not covered

- 13 strands and up: points only (descriptive timings), no full
  bracket. T4.
- Mirror images: R4 cannot see them; R1 and R5 can, but R1 runs in full
  only where the vertex model does.
- The exact routes are measured, not optimized: no fixed-width
  polynomial route was written.
- Only random braids from one generator (seed 2028 per (n, L)); no
  family where the answer is known in closed form beyond the named
  knots (3_1, 4_1, T(3,4), T(4,5)).

## Run

T2: `D136_MODUS=plene tools/custos.sh build/d136_t2/main` (binary built
from commit 44ac93b6), detached; 692 passed, 0 failed; peak 544 MB
(custos, compressed included; cap 4 GB). Largest single items: 12 × 801
TL bracket 4,700.8 s; 8 × 801 vertex model in full 3,247 s; 12 × 401 TL
bracket 578 s. Times are CPU time (`clock()`). The registered code path
of main.c is unchanged since 44ac93b6: later commits add print gating,
`print_span`, the desc section and the bracket file (checked by the T3
reviewer with `git diff`).

Descriptive: `D136_MODUS=desc D136_PARS=times|spans|integers|polynomials
tools/custos.sh build/d136_desc/main` (main.c as committed with this
file); times alone first (peak 246 MB), then the other three in
parallel (integers peak 286 MB; polynomials 4,966 MB, cap 8 GB, the
dense 12 × 201 buffers; spans 1 h 40 min wall, by the output files'
timestamps, the 12 × 801 bracket again). The Z[A²] times come from
that loaded parallel run, the TL times from T2.

Brackets: after T3 the span run was repeated with
`D136_UNCINI=…`, writing every exact bracket (name, n, c, writhe,
lowest exponent of A, coefficients in decimal). That rerun was killed at
12 × 801 by custos's 4 h wall-clock limit ("mora excessa") while the
machine slept, after the other 44 brackets were written; 12 × 801 was
rerun alone (`D136_SOLUM=12,801`, under `caffeinate -i`), 4,700 s CPU
again, the same 175 bits and span 259. The 45 lines are in
`brackets.txt` next to this file (57,286 bytes, sha256
de53b6e52517e6f38fa60450ea6b11f0a124f01f0d23feca744e229606fa9513). A
wall clock expiring during sleep is the likely cause of the earlier rc
137 kills too (plant 5, plant 8's restore); not verified.

Frozen copy: `demo-snapshot.c` (`knotapel/archive.sh`, `D136_MODUS=t1`).

## Review (T3, 2026-10-30, recensor-d136)

No H: every registered verdict (R0–R6, conv0) and the 12 × 801 headline
stand. Own oracle (Python/numpy, shares no code or structure with
main.c or the design reviewer's oracle), three routes:
- TL in the **regular representation**: all Catalan(n) planar diagrams,
  x ← x·ρ(letter) by composing diagrams and counting loops, then the
  Markov closure ⟨β̂⟩ = Σ_D x_D δ^(loops(closure D) − 1). No link
  states, defects or Δ_j. Mod q for points; polynomial arrays mod 4–8
  primes plus CRT for exact brackets.
- the Kauffman state sum, to validate the TL route (40 of 40 random
  words exact; point mode = exact mode 40 of 40);
- det = |Δ(−1)| by reduced Burau at t = −1 (odd strand count;
  stabilized once if even), exact integers (= |V(−1)| on 52 of 52
  random knots).

Braid words, the demo's polynomials and its `tl_value` at given points
came from a scratch copy of `demo-snapshot.c` with a dump driver.

| check | agreed |
| --- | --- |
| exact bracket = the demo's polynomial | 42 of 42 (named knots; every random braid n ≤ 10 at c ≤ 201, n = 11 at c ≤ 102, 12 × 21; 8 × 401, 8 × 801) |
| the demo's polynomial at 3 random A mod 2,147,483,629 = oracle | 48 of 48 braids (144 points; adds 11 × 202, 12 × 51/101/201, 10 × 401, 12 × 401) |
| the demo's `tl_value` = oracle, 6 points of p* + 2 mod 2³¹ − 1 | 49 of 49 braids (392 points), **including 12 × 801**; the first 4 p* points are T2's R1p points |
| Burau det = \|V(−1)\| of the demo's polynomial | 48 of 48 |
| Burau det² ≡ the demo's colouring det² (3 primes) | 49 of 49; 171 bits at 12 × 801, 186 at 8 × 801, as the demo printed |
| R4 identities recomputed on the demo's polynomials | 48 of 48 |
| span and largest-coefficient rows of the span run | 44 of 44 (12 × 801 not certifiable then: no saved polynomial) |

Also checked: interpolation settings equal `_uncinus_per_rete`'s; check
counts per family; the evaluation counts (23 × 563, 22 × 585); the
registered code path unchanged since 44ac93b6; the generator copy
behaves like the original; colouring prime counts and det bit lengths
for every c ≥ 201 braid. Code review: no bug that makes a reported
number wrong; the arena ping-pong in `exact_traces` is safe (magnus
add, subtract and multiply always allocate the result; only
`magnus_nega` shares limbs, and it is not called); the A³ scaling
tables are right; "agrees at A = 2 mod p*" tests the exact route's
arithmetic and scaling, not its reported bit sizes.

Findings and where they went:
- **M1** plant 8's fault was replaced (v2's "letter skipped" makes the
  interpolation refuse, so "R1 green" was impossible) and not listed as
  a deviation → Plants, plan.md T3 note; the blind-spot sentence on
  refusals under R1 added under Registered predictions.
- **M2** plant 5's killed run was scored a success → harness fixed,
  row corrected, plant 5 rerun (Plants).
- **M3** "word-sized" prime-free arithmetic contradicted by the 192-bit
  intermediates and 175-bit bracket at c = 801 → Result 5 rewritten
  (multi-word, gain at best a few-fold, an estimate).
- **M4** "vertex model faster on short braids" true only with many
  strands; width 14 uncited → Result 3 retitled, both rows of times,
  coniectura.out cited.
- **M5** the headline polynomial was saved nowhere → `brackets.txt`,
  then checked by the reviewer (below).
- **L1** span range exception (11 × 20); **L2** 11 × 20 is provably the
  unknot (Markov moves); **L3** cost range 1.36–1.64 ns, quiet machine,
  load figures; **L4** table times from the polynomials run (1.1 s,
  4.0 s), 4–6× at 12 strands; **L5** spans 1 h 40 min wall; **L6** R6's
  ≤ 8 h clause added to the row; **L7** R1p's 6 = 3 value checks + 3
  assertions; **L8** the index flip is a smoke test; **L9** the
  generator is not verbatim (wrapper inlined); **L10** R0 does not stop
  the demo; **L11** "78 CPU-minutes"; **L12** the bound gain is a
  ceiling (6 primes against 23); **L13** width source cited. All
  applied.

Follow-up on `brackets.txt` (all 45 lines): equal to the oracle's exact
bracket 38 of 38; equal to the oracle at 3 fresh random points mod
2,147,483,587, 44 of 44 (132 points); writhe, span and largest-coefficient
bits equal the span table 45 of 45; V(1) = 1, V(ω) = 1, V(i) = ±1 and
|V(−1)| = Burau det 45 of 45. For 12 × 801: 6 of 6 fresh points over
two primes (2,147,483,587 and 2,147,483,549), 8 of 8 earlier points, det
171 bits, span 259, largest coefficient 175 bits. **Every bracket of the
test set, the headline included, is independently certified.**

Not checked by the reviewer: timings (load average 8–12 during the review);
vertex-model brackets (relied on T2's in-run R1); the Z[A²]
intermediate bit sizes (different basis, code reviewed only); the cause
of the rc 137 kills.
