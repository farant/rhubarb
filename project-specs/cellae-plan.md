# cellae: plan (house tier)

Approved by Fran (2026-10-10): the API sketch, the house tier (Latin,
credo, gates), and three API decisions:
- `Capacitas` keeps TRUE counts separate from stored winners, with the
  greedy stop as a named search parameter, never as a count;
- the greedy search takes a fixed ladder, "first k that passes", and
  records k per winner;
- `catalogus_clausura` is general (generator array plus writhe array),
  with D82/D84's order documented and a truncation flag.

## Purpose

This is the shared engine for the activation audits: D84 (done as D124,
standalone), D82 (D125), then D83 and D89–D93. Today every demo copies
its own catalog, cell and search code. That copying is how D117's silent
caps spread (24 copies of D79's axis helper) and how D84's MAX_WIN count
passed for capacity.

## Semantics (D82/D84's `combined_cell`, exact)

For a sum s = (a, v) at k:
- **zero sum:** cell (k − 1)·(nd + 1) + nd;
- **real sum** (v = 0): sector 0, axis nd;
- **null sum** (a = 0): sector k/2. At even k this is a tie with
  k/2 − 1; at odd k there is no tie;
- **otherwise:** sector = #{m < k/2 : tan²h ≥ tan²(mπ/k)} with
  tan²h = |v|²/a². A tie is equality at some m;
- **axis:** the first maximum of (v·u)²/|u|² over the table, in
  first-appearance order with antipodes merged. Ties are all maxima.

Separability of a weight set is decided over 3ⁿ coefficient vectors,
with parity = number of nonzero coefficients mod 2, and folded by ±
(s and −s always share a cell). Modes:
- rule;
- robust (every tie resolution);
- possible (some resolution per exact point, by SAT);
- four fixed rules (sector lower/upper × axis first/last).

## v1 scope

Catalogs over ordered quadratic fields: Q(√2) for ζ₈, Q(√3) for ζ₁₂.
Placing tan²h against tan²(mπ/k) needs a quadratic minimal polynomial;
other fields are refused (FALSUM) until the extensio ↔ radices bridge
exists. Exact work uses integer Z[√d] coordinates (magnus, no overflow)
at a common scale; a double copy feeds the certified filter.

## Tasks

Each task follows the house cycle:
1. implement;
2. tests red, then green;
3. planted faults;
4. worklog;
5. commit behind the gates;
6. stop for Fran's go-ahead.

- **T1. `quaternio_proximi`.** All maximal axes, where today
  `quaternio_proximus` returns only the first. Credo: the cuboctahedral
  ties of ζ₈, against `quaternio_proximus`.
- **T2. `catalogus`.** General BFS closure:
  - identity, then generators (depth 0), then rounds of entry ×
    generator in array order;
  - equality up to sign;
  - limit with a truncation flag;
  - depth, writhe and null per entry, and an order-preserving
    sub-catalog.

  Credo: ζ₈ (24) and ζ₁₂ truncated at 4,096 equal D84's / D82's float
  catalogs entry for entry, in order (frozen fixtures).
- **T3. `activatio`: exact cells.** The axis table (first appearance,
  optional cap), boundaries as S_k roots (radices, by position), the
  rule cell plus the tie set, and a cache per exact point.

  Credo: hand cells at the ζ₈ ties (null sums at every even k,
  cuboctahedral axis ties, sector-boundary sums with tan² in Q(√2)),
  and the cap.
- **T4. Separability.** Every mode, 3ⁿ with the ± fold. The SAT is
  D121/D124's DPLL, clauses per exact point, with a decision budget →
  FALSUM.

  Credo: sets with known verdicts from D124 (rule ≠ robust ≠ possible
  cases, a same-point opposite-parity clash such as q₁₅ + q₂₂ = q₁₇).
- **T5. Searches and `Capacitas`.**
  - exhaustive, level-wise (downward closed per k, union over the
    ladder);
  - greedy, D82/D84's order: a stop after the first m winners per level
    as a named parameter, k recorded per winner, incremental extension
    (a third of the sums reused).

  `Capacitas`: true counts per n, stored winners (tuples and k) up to an
  explicit storage limit, and the maximum.

  **Regression credo:** D124's exact tables are reproduced. Full,
  non-null, null-only and the 13-axis variants under rule and robust;
  the k_sec sweep; the four fixed rules; D84's greedy search with exact
  cells (8, 8, 0).
  *(2026-10-12: incremental extension moved out of T5 into T6/T7,
  measured on zeta_12 next to the filter. T5 landed as 4 decisions: a
  `Quaestio` parameter struct, exhaustive search for <= 64 entries.)*
- **T6. Certified float filter and statistics.** A float cell with a
  proven margin to every boundary decides alone; anything inside the
  margin is decided exactly.

  Credo: on every ζ₈ sum the filtered cell equals the exact cell; the
  filter never decides a tie; the statistics count filter, exact and
  tied decisions.
- **T7. Axis index.** Bucketed by region of S², exact fallback on near
  ties.

  Credo: equal to the linear scan on all ζ₁₂ sums of a sample, with
  timing reported. Measured first: if the scan is not the bottleneck
  after T5–T6, T7 is dropped.
  *(2026-10-12: the scan was 86% of the deep D82 run. A lossless
  two-stage scan replaced the index: stage 1 prunes with plain doubles
  under a proven margin, stage 2 uses intervals. 75 s -> 17 s. The
  bucketed index is deferred; see the worklog.)*
- **T8. Review** (recensor-extensio), then fixes.
  *(2026-10-12: no H/M. L1 (zeta_12 oracle + independent D82 winner
  fixture) and L2 (cap / triple limit binding) are fixed with tests. P1
  arena churn is proposed, not done.)* *(P1 done the same day, after
  Fran's go-ahead: one arena per search, tacta / literals / scan scratch
  in the activation. Deep 564: 20.2 -> 14.3 s, sys 4.7 -> 0.08 s, peak
  149 -> 76 MB, identical output.)*
- **T9. D125:** D82's depth law and matched-size control, plus D87's ζ₁₂
  claim, on `cellae`. Separate sketch, already approved in outline: the
  replica, exact rule / robust, the 512 cap and uncapped.

## Gates

- `radix`, `silva` (new lib/*.c), `generata` and `venenum` (the cache
  and scratch arenas).
- `oratio` if the glossary is touched.

## Out of scope for v1

- Fields beyond quadratic (needs the bridge).
- House-tier exact hashing as a separate library: T3 starts with a local
  hash on Z[√d] coordinates. If a second user appears,
  `magnus_dispersio` and friends get their own sketch.
- Parallel processes per subset.
