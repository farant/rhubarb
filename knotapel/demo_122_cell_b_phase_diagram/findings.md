# Demo 122: Cell B's Exact Phase Diagram — Findings

D97 asked why D96's Cell B, the six ±45° rotations of ζ₈ about three
orthogonal axes, computes XOR for every set. Its key evidence was an
angle sweep: the same three axes at half-angles 10°, 15°, …, 90°, with
capacity at N = 3–6. It concluded:
- a perfection plateau from 25° to 75°;
- "45° is the midpoint, not a magic angle";
- an "isolated resonance" at 35°, where N = 5 fails;
- total collapse at 90°.

This demo computes Cell B's capacity at every half-angle in
(0°, 90°], exactly. Every boundary is given in closed form.

Run: `./bin/aedilis knotapel/demo_122_cell_b_phase_diagram/main.c &&
bash build/aedilis/main/struere.sh && ./build/aedilis/main/main`.
It takes 0.15 s and runs 13 checks.

## Why the sweep has an exact answer

Each swept element is (cos θ, ±sin θ · d_j). A signed sum is
(n₀ cos θ, sin θ Σ n_j d_j), with small integers n₀ and n_j fixed by
the mask, independent of θ. So:
- **The axis cell never depends on θ.** It is the largest |n_j|, the
  rule taking the first in D97's order, with ties where maxima are
  equal.
- **n₀ = 0:** the angle is exactly 180° for every θ, a tie at sector
  m = k/2.
- **Otherwise the sector depends only on u = tan²θ.** It changes at
  u* = n₀² tan²(jπ/24) / r, with r = |n|² and j = 1–11. Every sector
  boundary of k = 6, 12, 24 is a multiple of π/24.

**Method:**
1. All 154 breakpoints, 132 distinct values, are sorted exactly.
   tan²(jπ/24) = (4 − 4C)/(4 + 4C) with C = cos(jπ/12) in Z[√2, √3], so
   each comparison is a `surdus` sign.
2. Rule, robust and possible are then evaluated on every open interval
   and at every breakpoint by **rank**: u's position among the sorted
   breakpoints answers every sector question.

## The diagram (robust: passes under every tie resolution)

| half-angle θ | N = 3 / 4 / 5 / 6 (of 20 / 15 / 6 / 1) | upper end, closed form |
|---|---|---|
| (0°, 4.347°] | 0 / 3 / 0 / 0 | arctan(tan(π/24)/√3) |
| (4.347°, 5.318°] | 8 / 3 / 0 / 0 | arctan(tan(π/24)/√2) |
| (5.318°, 7.5°] | 8 / 3 / 0 / 1 | π/24 |
| (7.5°, 10.547°] | 20 / 3 / 0 / 1 | arctan(2 tan(π/24)/√2) |
| (10.547°, 20.424°) | 20 / 15 / 0 / 1 | arctan(4 tan(π/24)/√2) |
| [20.424°, 20.754°] | 20 / 3 / 0 / 1 | arctan(2 tan(π/12)/√2) |
| (20.754°, 21.552°] | 20 / 15 / 0 / 1 | arctan(3 tan(π/24)) |
| **(21.552°, 33.355°)** | **20 / 15 / 6 / 1** | arctan(5 tan(π/24)) |
| [33.355°, 38.794°] | 20 / 15 / 0 / 1 | arctan(3 tan(π/12)) |
| **(38.794°, 79.453°)** | **20 / 15 / 6 / 1** | arctan(tan(11π/24)/√2) |
| [79.453°, 80.840°) | 20 / 15 / 6 / 0 | arctan(2 tan(11π/24)/√6) |
| [80.840°, 82.5°) | 20 / 3 / 6 / 0 | 11π/24 |
| [82.5°, 84.396°) | 20 / 0 / 6 / 0 | arctan(3 tan(11π/24)/√5) |
| [84.396°, 85.653°) | 20 / 0 / 0 / 0 | arctan(3 tan(11π/24)/√3) |
| [85.653°, 87.487°) | 12 / 0 / 0 / 0 | arctan(3 tan(11π/24)) |
| [87.487°, 90°] | 0 / 0 / 0 / 0 | |

The exact rule equals robust on every open interval. The rule's tie
choices matter only at breakpoints. The demo prints the "possible"
diagram as well.

## The claims

| D97 | exactly |
|---|---|
| plateau 25°–75°, 100% at every N | **two plateaus**: (arctan(3 tan π/24), arctan(5 tan π/24)) = (21.552°, 33.355°) and (arctan(3 tan π/12), arctan(tan(11π/24)/√2)) = (38.794°, 79.453°) |
| 35°: "isolated resonance" (N = 5 fails) | a **5.44°-wide band** [33.355°, 38.794°] where N = 5 fails robustly and under every resolution ("possible" 0 inside it). It is a genuine geometric band, not a resonance and not a tie |
| 45° is the plateau midpoint, not a magic angle | 45° sits **on** a breakpoint (u = 1) and is robustly 100%. It is not the midpoint (the upper plateau's is 59.12°). "Not magic" holds |
| total collapse at 90° | robust 0 at every N, but **possible 100% at every N**: every sum is exactly at 180°, and some tie resolution separates everything. The collapse is a tie artifact |
| below 25°, N = 5 fails while N = 6 is 100% (10°–20°) | correct: N = 5 robustly fails below 21.552°, and N = 6 holds on (5.318°, 79.453°) |

**D97's float sweep was right.** At all 21 of its angles, its float
counts equal the exact rule. Only "possible" differs, at 10° (N = 4:
15, not 3) and at 90°. What was wrong was the reading: interpolating
from samples 5° apart missed the plateau's true edges, called a band a
resonance, and took a tie collapse at 90° as real.

## Checks

- **Cell B's anatomy:** a = √2/2 with one nonzero vector coordinate,
  6 elements on 3 orthogonal axes.
- **D97's sweep:** reproduced at all 21 angles, N = 3–6, from its own
  floats.
- **Breakpoints:** sorted exactly. Equal values share one rank, and
  distinct values strictly increase.
- **D97's angles placed exactly:** multiples of 15° exactly (tan² is in
  Q(√2, √3)); the others by a certified float gap (refused below
  10⁻⁹).
- **Claim checks:** the two plateaus exactly, the band, 45° on a
  breakpoint, 90°, rule = robust on every open interval, and D97's
  floats = the exact rule.
- **Oracle:** `python3 -I oracle.py U` uses a different formulation: no
  tan, no breakpoints. It compares cos² of each sum's half-angle,
  n₀²/(n₀² + r·u), with cos²(mπ/k) in Q(√2, √3), with Fractions and
  80-digit Decimal signs. It agrees with the diagram at 11 rational
  points (incl. the breakpoints 30°, 45°, 60°) and at 7 region
  boundaries given exactly as A tan²(jπ/24)/B (7.5°, 20.42°, 21.55°,
  33.36°, 38.79°, 79.45°, 82.5°). The rule, robust and possible counts
  differ from each other there.

## Plants (12)

**Red (10):**
- the sign of √2 in cos(π/12)
- the tan² numerator
- intervals shifted by one rank
- n₀ = 0 not a tie
- axis ties dropped
- sort order inverted
- equal breakpoints not merged
- N = 6 breakpoints omitted
- exact placement ignoring equality
- an n₀ < 0 tie not counted in the sector

The last two red ones first **survived** and showed real gaps. Nothing
checked the rank invariant, and the oracle had been compared only at
rational points (30°, 45°, 60° lie inside a plateau). Hence the rank
self-check and the boundary-point oracle check.

**Equivalent mutants (2):**
- **The replica's √2 shortened.** D97's axes are b/|b| = ±1 for any
  √2 constant.
- **The 90° clash key ignoring the real part.** At 90° no two masks
  with the same vector part have different truth values, so the clash
  test never fires either way.

## Not covered

- D97's Phase 2 "margin" is a difference of cell indices, which mixes
  sector and axis numbering. It has no geometric meaning and is not
  recomputed.
- MAJ and AND were not swept, as in D97.
