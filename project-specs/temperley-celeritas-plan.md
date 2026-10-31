# temperley celeritas: speed pass for the TL route (D136 T4, step 2)

Status: v1 APPROVED by Fran (2026-10-31); design review next, before
any code.

Agreed with Fran (2026-10-30): T4 goes promote (step 1, done:
project-specs/temperley-lieb-plan.md, 49b08975..79e7fad9), then a speed
pass with registered predictions (this plan), then track A (Jones
statistics on random braid closures, after a literature check).

## Purpose

The TL route costs ~1.5 ns × c × Catalan(n) per point (D136 findings,
Result 2), ~13,000 points for a full 12 × 801 bracket: 78 CPU-minutes
at 12 strands, ~12 h at 14 and ~6.5 days at 16. Exactness rules out
the shortcut of stopping when the answer looks stable (brainstorm §11),
so speed has to come from the kernel and from doing points in parallel.
This plan takes the kernel levers, registers what they should buy before
measuring, and leaves parallel points as a design question.

## Governing rules

1. **Exact only**, as in contractio-plan.md rule 1. No floats anywhere.
2. **Registered before measured.** The predictions below are fixed in
   this plan before any new code is timed. T0 measures the baseline and
   may decide whether T2 happens (its gate is stated here); it may not
   move a threshold.
3. **Same answers.** Every change keeps all suites green and, at the
   end, recomputes all 45 D136 brackets through the library and compares
   them with the certified `knotapel/demo_136_tl_braid_closures/
   brackets.txt` (sha256 de53b6e5…).
4. **No API change for callers** of `laqueus_uncinus_ex_plexu` or the TL
   route; internals (kernel, tables, buffers, interpolation settings for
   the TL route) may change.

## The levers

### L1. In-place normalized update (T1)

Today each letter costs a full scaling of the d × d matrix plus a row
gather: about 1.9 × Catalan(n) multiply-and-reduce operations per
letter. Writing σ_i^ε = A^ε (1 + A^(−2ε) e_i) and pulling A^w (w = the
writhe) out of the trace leaves M ← (1 + c E_i) M with c = A^(−2ε).
E_i is zero on rows without the arc (i, i+1), so only rows WITH that arc
change:

  new M[r] = (1 + c δ) M[r] + c Σ_{s → r} M[s]

where the sources s are rows without the arc, which do not change. So
the update is safe in place, with one buffer instead of two. Exact
operation counts over the module tables (scratch, 2026-10-31):

| n | current (× Catalan per letter) | in-place updated elements (× Catalan) | ratio |
| --- | ---: | ---: | ---: |
| 8 | 1.86 | 0.30 | 6.2 |
| 12 | 1.88 | 0.28 | 6.7 |
| 16 | 1.89 | 0.27 | 6.9 |

(plus ~0.6 × Catalan plain additions for the source sums). Each updated
element costs two multiplications and one reduction. Memory: one buffer
of max d² instead of two (53 MB instead of 106 at 16 strands).

### L2. Division-free reduction (T2, gated by T0)

The kernel reduces with a 64-bit `%` by a runtime prime. Montgomery
multiplication with R = 2^32 needs only 32 × 32 → 64-bit products and
shifts, which C89 can express, provided p < 2^31 (so t + m·p < 2^64).
That needs the TL route's interpolation to use primes below 2^31
(`primus_limes`), costing about 3% more primes (31 bits each instead of
32). With p < 2^31 the source sum and the two products also fit in 64
bits before a single reduction (lazy reduction), which L1 can use even
without Montgomery.

### L3. Points in parallel (NOT in this plan; question for Fran)

Points are independent, so N processes could cut wall time by up to N.
The house runs processes, not threads, and `processus` executes
programs (exec), it does not fork workers. Two shapes:
- a tool-level driver that runs worker copies of itself through
  `processus_incipere`, each evaluating a slice of (prime, point) pairs,
  and feeds the values to `interpolatio` through a new "precomputed
  values" entry point; or
- forking inside the library, which collides with the house IO rules
  (examen 96/97: raw writes outside `filum`).
Recommended: the tool-level driver, as its own plan after this one.

## Registered predictions

Measured with one committed tool (T0 adds a TL section to
`tools/uncinus_mensura.c`, release build -O2, under `tools/custos.sh`,
braids from the R0 generator, seed 2028), per-point time as the median
of 3 points of one prime, on a quiet machine (load average below 2,
recorded with each run; a run above 2 is repeated, not counted).

- **P1 (T1, in-place update).** Per-point time, baseline / new, at least
  **3.0×** at 12 × 801, 14 × 801 and 16 × 801. (Operation ratio
  6.7-6.9; the threshold is under half of it, for memory traffic and
  the extra additions.)
- **P2 (T1, memory).** `laqueus_temperley_sumptus(16, c).octeti` drops by
  at least 45% (one buffer instead of two).
- **P3 (T2, only if adopted).** T0 microbenchmarks the kernel's
  multiply-and-reduce with `%` against Montgomery on the same data. T2
  is done only if Montgomery is at least 1.2× faster there; if so,
  per-point time improves a further **1.15×** at the same three sizes.
  If the microbenchmark shows less than 1.2×, T2 is dropped and that is
  recorded as decided by T0, not as a failed prediction.
- **P4 (primes below 2^31, only with T2 or lazy reduction).** Total
  evaluations for the 12 × 801 bracket rise by at most 5% (12,949
  today).
- **P5 (end to end).** The full 12 × 801 bracket through
  `laqueus_uncinus_ex_plexu` takes at most **T0's measured full-bracket
  time / 2.8** CPU time (≈ 28 minutes if T0 reproduces D136's 78; 2.8 =
  P1's 3.0 less P4's up-to-5% extra evaluations, rounded down), with
  the same polynomial as brackets.txt.

Not predicted (descriptive): per-point times for n = 8 … 16, the
crossover with the vertex model after the speed-up (the dispatcher's
TL constant, 150, will be re-measured and updated in T3 with the old
and new values recorded).

## Tests

- All existing suites stay green (temperley, laqueus, laqueus_rete:
  the TL route is checked against the state sum, the vertex model at
  points, known knots, the certified brackets and Markov moves at
  10 × 401).
- New: the writhe factor A^w at points (a braid and the same braid with
  a σσ⁻¹ pair inserted have equal values; a braid and its mirror give
  A ↔ A⁻¹), which the old kernel never needed.
- If T2: Montgomery conversion round trip on random residues, and the
  kernel at the largest prime below 2^31.
- Plants (registered as literal triples in a script under build/,
  named reds; a run without a summary is inconclusive):
  - in-place hazard: an arc row read after it was updated (for example,
    the update loop reading `M[r]` from the new value for a second
    source) must turn the point tests red;
  - c = A^(+2ε) instead of A^(−2ε);
  - the A^w factor dropped;
  - if T2: Montgomery form not converted back, or R² wrong.

## Tasks

- **T0. Baseline and gates.** TL section in `tools/uncinus_mensura.c`;
  per-point times n = 8 … 16 at c = 801 and full 12 × 801 bracket time
  for the CURRENT library (79e7fad9); the `%` vs Montgomery
  microbenchmark (scratch); a dated amendment with the numbers and the
  T2 decision. No threshold moves.
- **T1. In-place update.** Kernel, one buffer, A^w factor, lazy
  reduction if primes < 2^31 are adopted; sumptus updated; tests and
  plants; measure P1, P2.
- **T2. Montgomery** (only if T0's gate passed). Measure P3, P4.
- **T3. Acceptance.** All 45 D136 brackets recomputed through
  `laqueus_uncinus_ex_plexu`, compared with brackets.txt (P5 is the
  12 × 801 one); update the dispatcher's TL constant from the new
  per-point times (old and new recorded); worklog.
- **T4. Review.** Independent reviewer, own oracle (recensor-d136's or
  recensor-temperley's), not building in the live tree.

## Questions for Fran (2026-10-31: plan approved; Q1 taken as
recommended, Q2 to be decided at T3)

1. **L3, parallel points:** a separate plan after this one, with the
   tool-level driver as recommended? Or fold it in here?
2. **The 14 × 801 headline:** after T1/T2, a full 14 × 801 bracket would
   be a few CPU-hours (estimate). Run it once as part of T3, or leave
   it to track A?

## AUDIENDA

- The operation counts are exact; how much of the 6.7× survives memory
  traffic is the open question P1 answers.
- Montgomery's benefit on this machine is unknown; Apple's integer
  divider is fast, hence the T0 gate.
- Primes below 2^31 change only the TL route's interpolation settings;
  the vertex route keeps its own. Points still agree with the vertex
  route at any prime both accept, which the tests use.
- The measurement tool runs on a machine shared with other sessions;
  the load-average rule is the guard.
