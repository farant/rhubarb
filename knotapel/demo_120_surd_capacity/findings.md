# Demo 120: Exact Capacity on Surds — Findings

Demo 119 recounted D94's capacity tables exactly, with its exact layer
in extensio: bignum rationals, sums embedded into Q(cos 2π/48) or
Q(cos 2π/240) (degree 8 or 32). It took 46 minutes. This demo changes
only that layer: integer copies of the catalogs and exact signs in
Z[√2, √3, √5] by the house library `surdus`.

Run: `./bin/aedilis knotapel/demo_120_surd_capacity/main.c && bash
build/aedilis/main/struere.sh && ./build/aedilis/main/main`
- full: 17 min 46 s, 33 checks;
- `DEMO120_MEDIUS=1`: Parts A–C, 2 min 16 s, 22 checks (for timing);
- `DEMO120_CELER=1`: Part A + ζ₈ N ≤ 4, 0.5 s, 19 checks (for plants).

## Headline

**Same results, 2.6× faster, and the exact arithmetic is no longer
where the time goes.**

- Every table row (sets, D94-float, exact rule, robust, possible, tied
  sets), every trial mean, and the certification statistics equal
  D119's. The printed tables and statistics are byte-identical (99
  lines).
- The statistics match to the last decision: 2,046,598,563 masks,
  224,397,659 exact sums, 138,718 zero sums, 397,565,109 exact sector
  decisions and 93,638,928 exact axis decisions (all exact ties, as in
  D119). D120 therefore makes the same exact decisions at the same
  places; only the arithmetic underneath changed.
- `surdus` never refused (0 fallbacks to extensio).

| | D119 (extensio) | D120 (surdus) | |
|---|---|---|---|
| Parts A–C (ζ₈, 2I first 24) | 374 s | 136 s | 2.75× |
| full run | 46 min 20 s | 17 min 46 s | 2.6× |

D119's own split (a timestamped run beside other jobs, 48.5 min in
all): Parts A–C 374 s, Part D 268 s, Part E (ten trials of random 2I
24-subsets) 2,270 s, which is 78%. Part E is where any further speedup
has to come from.

## Where the time goes now

Profile of the medium run (`sample`, 30 s):

- the float replica of D94 and the verdict logic: about 88%
  (`mask_cell`, `judge_set`, `mask_base`, `acos`);
- the exact layer (`surdus`, integer sums, dispatch): about 10%.

A microbenchmark on D119's own code (a scratch copy) measured one exact
sector decision:

| | exact ties (D119's actual case) | random decisions (mostly not ties) |
|---|---|---|
| extensio, as D119 | 6.8 µs | 600–670 µs |
| surdus | 0.24 µs | 0.35 µs |

All 360,000 benchmark signs agreed with D119's pipeline. For D119's
workload the gain per decision is 29×. Away from ties, extensio needs
a certified numerical sign in a degree-32 field (~0.6 ms); surdus does
not care. An audit whose near cases are not all ties would be
impractical on extensio and cheap on surdus.

The remaining time is the part kept identical on purpose: D94's float
arithmetic in D94's order (sums, `acos`, axis dot products), which must
reproduce D94's printed numbers bit for bit, because hundreds of
millions of exact ties are decided by its rounding. The exact verdicts
do not need that order, only a sound filter. Next speedups, if wanted:
exact verdicts from Gray-code integer sums and a cos table (no `acos`),
the D94 replica kept as a one-time check, and sets spread over
processes.

## Method

- **Integer catalogs.** Coordinate = (p + q√d)/s: ζ₈ d = 2, s = 2; 2I
  d = 5, s = 4. Derived from the exact catalog, with a check that every
  coefficient times s is an integer. For 2I the derived integers equal
  D94's own hand-written integers entry by entry (an independent
  construction).
- **Sector.** sign(a/|S| − cos(mπ/k)) has the same branches as D119.
  When a and the boundary have the same sign: sign(8A² − (4 + 4
  cos(jπ/12))·N), with A = s·a, N = s²·|S|², j = 24m/k, and
  4cos(jπ/12) ∈ Z[√2, √3] from a table (√6 ± √2, 2√3, 2√2, 2, 0).
- **Axis.** (v·u_j)²|u_l|² against (v·u_l)²|u_j|²: integer dot
  products in Z[√d], then the sign by surdus (scale s⁶ on both sides).
- **Zero, vector zero, vector clash:** integer comparisons.
- **Fallback.** When surdus refuses (s64 overflow), D119's extensio
  path decides; counted (0 in the full run). Part A cross-validates
  surdus against the forced fallback on 3,680 random sector and axis
  decisions of both catalogs (all agree), so the fallback wiring itself
  is exercised. Those decisions are excluded from the statistics, which
  are saved and restored around the cross-check.
- **Unchanged from D119:** the float replica (D94's order), the 10⁻⁹
  filter, the verdicts (rule, robust, possible by DPLL), the seeds and
  sampling, Part F, and the norm-bound check.

## Plants (12, all red, on the 0.5 s mode)

- the sign of √2 in cos(π/12)
- the factor 8 → 4 in the boundary comparison
- j = 12m/k instead of 24m/k
- √5 stored in √3's basis slot
- the √d term dropped from integer dot products
- the axis norms swapped
- the mask sign inverted in the integer sums
- an exact zero vector part not recognised
- vector clashes never detected
- the fallback's sign inverted (sector)
- the fallback's sign inverted (axis)
- the m = k/2 boundary (cos = 0) given sign +

The first attempt at "axis norms swapped" did not compile (an unused
variable under -Werror); it was re-planted as a full swap of the two
norms.

## Not covered

- Same scope as D119: D94 Phase 4 is not covered, sampled rows are
  D94's samples, and k is OR-ed as in D94.
- No new mathematics. This demo is the engine change; the ζ₈
  derived-series audit (D95/D96) is next, on this engine.
