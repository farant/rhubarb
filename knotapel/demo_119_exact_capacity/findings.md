# Demo 119: Exact Capacity — Findings

Demo 94 compared the capacity of the binary octahedral catalog (zeta_8,
24 entries) with the binary icosahedral group 2I (random 24-subsets, the
first 24 by BFS, all 60). It counted the weight sets that compute XOR,
AND or MAJ through a cell activation:

- the cell of a sum S = sum +-q_i is (sector of the rotation angle
  2 acos(a/|S|), k sectors) x (nearest catalog axis by |cos|);
- a set passes if, for some k in {6, 12, 24}, no cell receives both
  truth values.

Everything was in doubles. Demo 118 found that D66's Voronoi counts were
decided by EXACT ties, so this demo recounts D94's tables exactly.

Run: `./bin/aedilis knotapel/demo_119_exact_capacity/main.c && bash
build/aedilis/main/struere.sh && ./build/aedilis/main/main`
- 46 min, 17 checks, 2.05 billion mask evaluations;
- `DEMO119_CELER=1` runs Part A plus zeta_8 N <= 4 in 1 s (12 checks;
  used by the plants).

Oracle: `python3 -I oracle.py [maxN]` re-derives the zeta_8 verdicts
(N = 3, 4: 18 counts) with its own exact arithmetic in Q(sqrt2, sqrt3)
(zero exact, signs by 80-digit decimals), no float filter, every
boundary compared, and "possible" per distinct vector by backtracking.
It agrees with main.c on all 18 counts.

## Headline

**D94's comparison survives exactly, and gets stronger. D94's float
counts are not exact, though: ties are everywhere and the floats break
them arbitrarily.**

| claim (D94) | D94 float | exact rule | robust | possible |
|---|---|---|---|---|
| N=6 XOR: 2I random-24 mean vs zeta_8 | 23137 vs 12983 (+78.2%) | 22414 vs 12268 (+82.7%) | 19552 vs 10070 (**+94.2%**) | 31521 vs 22979 (+37.2%) |
| N=7 XOR: 2I random-24 mean vs zeta_8 | 672 vs 197 (3.41x) | 626 vs 161 | 449 vs 122 (**3.68x**) | 2666 vs 796 (3.35x) |
| depth-matched first-24 / zeta_8, N=6 XOR | 1.67x | 1.71x | **1.69x** | - |
| depth-matched first-24 / zeta_8, N=7 XOR | 0.84x | 0.94x | **0.84x** | - |

- "2I beats zeta_8 at the same catalog size" holds under every tie
  rule, with a larger margin when robust.
  - The earlier audit (notes 47-48) already refuted the *interpretation*
    ("solvability is the bottleneck") with a Haar control.
  - This demo settles only the *numbers*.
- **The float counts are tie-rule artifacts:**
  - zeta_8 N=6 XOR is 12,983 in floats against 12,268 for D94's formula
    evaluated exactly (5.8% overcount) and 10,070 robust (28.9%).
  - D94's single N=8 XOR success for the first 24 of 2I is 0 exactly.
  - For 2I with all 60 the float count is close to robust (N=6 XOR
    74,250 vs 72,876, +1.9%).
- **The exact rule can exceed the float count:**
  - zeta_8 N=8 MAJ: 22 float, 23 rule;
  - first-24 N=5 AND: 38,688 float, 38,702 rule;
  - first-24 N=8 MAJ: 215 float, 228 rule.
  The floats are not uniformly pessimistic or optimistic; they are
  arbitrary at ties.
- **Ties are the rule, not the exception.**
  - 397,565,108 sector decisions fell within 1e-9 of a boundary, and
    EVERY one was an exact boundary hit, i.e. a tie.
  - 93,638,928 direction decisions fell within 1e-9 of a second axis,
    and EVERY one was an exact tie.
  - The filter never met a near-miss: all 2.05 billion float cells not
    flagged were certified correct by the margin argument below.
  - 138,718 sums were exactly zero.
  - In zeta_8 at N = 3, 1,217 of 2,024 sets contain a tied sum.

## Tables (counts; D94-float = D94's printed numbers, reproduced)

zeta_8 (24 entries, 13 axes); N <= 6 exhaustive, N = 7, 8 D94's 100,000
samples:

| N fn | D94-float | exact rule | robust | possible |
|---|---|---|---|---|
| 3 XOR | 1480 | 1456 | 1456 | 1938 |
| 4 XOR | 8010 | 7908 | 7696 | 8678 |
| 5 XOR | 17201 | 16479 | 15497 | 24714 |
| 6 XOR | 12983 | 12268 | 10070 | 22979 |
| 7 XOR | 197 | 161 | 122 | 796 |
| 8 XOR | 1 | 1 | 1 | 27 |
| 6 AND | 111290 | 110495 | 107663 | 116323 |
| 6 MAJ | 10031 | 9635 | 7911 | 18245 |

2I, first 24 by BFS (12 axes):

| N fn | D94-float | exact rule | robust | possible |
|---|---|---|---|---|
| 3 XOR | 1580 | 1580 | 1580 | 1960 |
| 6 XOR | 21679 | 20936 | 17034 | 28850 |
| 7 XOR | 165 | 151 | 103 | 519 |
| 8 XOR | 1 | **0** | **0** | 5 |

2I, all 60 (31 axes); N = 3 exhaustive, N >= 4 D94's 200,000 samples:

| N fn | D94-float | exact rule | robust | possible |
|---|---|---|---|---|
| 3 XOR | 25595 | 25595 | 25595 | 33690 |
| 6 XOR | 74250 | 72876 | 72876 | 80617 |
| 7 XOR | 4760 | 4561 | 4561 | 8297 |
| 8 XOR | 2 | 2 | 2 | 24 |

The full tables (AND, MAJ, every N, and the 10-trial means) are in the
demo's output.

## Method

- **Same sets as D94.**
  - D94's arithmetic (Z[sqrt5]/4 integers, its float quaternions, its
    BFS, its build_dirs) is copied with names changed for latina.h.
  - Its LCG streams (seeds 77777+N, 88888+N, 55555+N, 99999+7t+N; trials
    by Fisher-Yates from seed 42) and its combination order are
    replicated.
  - The float verdicts reproduce every printed count: three tables and
    the XOR trial means (4 checks).
- **Exact catalogs.**
  - The same BFS runs in Q(sqrt 2) (zeta_8) and Q(sqrt 5) (2I) with
    quaternio, and the result is checked entry by entry against the
    floats (values within 2e-16).
  - The directions are D94's float dedup and an exact eadem_axis dedup,
    checked to agree index by index (13, 12, 31).
- **Certification.**
  - For each mask: the float sum (D94's order). When the float value is
    within 1e-9 of a sector boundary, of a second-best axis, or of zero,
    the decision is exact.
  - Sector: sign(a/|S| - cos(m pi/k)) by sign analysis and squares. a and
    |S|^2 are embedded into Q(cos 2pi/48) (zeta_8) or Q(cos 2pi/240)
    (2I) through the cached image of sqrt d (extensio's abelian
    embedding), and cos(m pi/k) comes from algebraicus_cosinus.
  - Axis: (v.u_j)^2 |u_l|^2 against (v.u_l)^2 |u_j|^2 in the small field.
  - Zero: exact.
  - Far from every boundary the float cell is exact. A sum of at most 8
    unit quaternions has absolute error around 1e-15 (8 additions of
    terms <= 1), so a value farther than 1e-9 from a boundary (or a gap
    larger than 1e-9 between the two best axes) cannot have the wrong
    side. Boundaries are at least about 0.008 apart, so one exact
    comparison decides the sector.
- **Verdicts per k, then OR-ed over k = 6, 12, 24 as D94 does.**
  - Exact rule: D94's formula on the exact values. The sector is the
    floor of the exact angle, so a boundary hit takes the upper sector;
    the axis is the first maximal one.
  - Robust: no cell is reachable, over all admissible cells, from both
    truth values. OR-ed over k this is a sufficient condition ("some k
    is robust").
  - Possible: a tie rule is a function of the POINT. Two masks with the
    same exact sum and different truth values fail outright (exact
    check, with float candidates). Otherwise the question is SAT: x_c is
    the label of cell c, and each mask needs a tied cell with its
    label. It is solved by DPLL with unit propagation and a decision
    budget; no set exceeded it ("undecided" stays 0).
- **Plants (12, all red, on the 1 s mode):**
  - filter margin 1e-18 (filter blind)
  - boundary tie without m-1
  - axis ties dropped
  - opposite-sign branch flipped
  - vector clash disabled
  - unit propagation inverted
  - rule on a boundary takes m-1
  - axis rule takes the maximal index
  - zero sum not special
  - wrong generator image in the big field
  - only k = 12
  - robust computed as the rule
  
  The opposite-sign and boundary-side plants first survived: no set in
  zeta_8 N <= 4 reaches those branches. Direct unit checks now kill
  them (sigma_1 lies exactly on the k = 12 boundary m = 3).

## History

- **First version: 31 minutes for zeta_8 up to N = 5.**
  - The exact sector check compared all 23 boundaries.
  - Every comparison re-embedded the sum, and every embedding recomputed
    sqrt d by a Gauss sum.
  - Fixes: one embedding per mask through a cached generator image, one
    comparison (only the boundary within the margin), axes once per mask
    instead of per k.
  - The library cost (immergere recomputes the generator's image on
    every call) is noted in extensio.h; an embedding handle is a wish.
- **"Possible" by brute force over 2^20 labelings left sets undecided.**
  Backtracking over masks still blew up at N >= 5. DPLL with unit
  propagation decides every set.
- A pool-lifetime bug in my own code: boundaries cached inside a
  per-mask pool mark were freed by the rollback ("sign refused"). They
  are now prepared once per field, outside every mark.

## Not covered

- D94 Phase 4 (zeta_12 truncated): a non-braid group (notes 41-45).
- The sampled rows are D94's samples, not the full population.
- k = 6, 12 and 24 are OR-ed as in D94; per-k tables are not printed.
