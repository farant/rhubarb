# Demo 127: Group Catalogs against Matched Nulls (audit P1) — Findings

Audit P1 (claims.md KC 47, 48, 19.7; ledger ...E72XG3) asks: under ONE
fixed exact activation, do the group catalogs compute better or worse
than knot-free point sets, and what carries a difference? This demo
judges every set with the same exact engine (`cellae`, house library):
- D94's phase cell, `CELLAE_CELLA_D94`;
- the signed encoding S = Σ ±q_i over all 2^n masks;
- the ladder k ∈ {6, 12, 24}, where "passes" means some k passes;
- XOR, AND and MAJ, under rule, robust and possible.

Every subset of every set is judged, N = 3–7 exhaustive (346,104
subsets at N = 7), so no row mixes a sampled scale with an exhaustive
one. Plan: `plan.md` (approved 2026-10-14; its "Review" section
records what changed after the independent review).

**Words.** The counts are **passing subsets per N**: how many of the
C(N_set, N) weight subsets compute the function. They are not D94's
"capacity" (the largest N with a pass). That is at least 7 for every
N = 24 set here, except LIN seeds where the count reaches 0.

**Only XOR is a property of a point set.** Under the signed encoding,
flipping one weight's sign relabels the masks. Every XOR label flips,
so XOR verdicts are unchanged; this is checked under D119's signs and
5 random sign draws each of 2O and 2I-D94. AND and MAJ change with the
signs, so they describe a choice of representatives (see "AND and
MAJ"). XOR is the headline function throughout.

## Result

Robust XOR, passing subsets at N = 7 (of 346,104), under the two common
tables:

| set | T13 | R13 | height |
|---|---|---|---|
| G, generic rational points (5 seeds) | 1,060–1,641 | 1,026–1,627 | 48–65 digits |
| AX, 2O's axes, random angles (5) | 715–2,210 | 592–1,843 | 15–26 digits |
| 2I-D94, D94's first 24 of 2I | 754 | 758 | 4 |
| 2O | 456 | 265 | 2 |
| AN, 2O's angles, random axes (5) | 219–442 | 205–537 | 13,860 |
| H5, height-5 sphere (20 seeds) | 27–320 | 18–272 | 5 |
| 2I-R, random 24-subsets of 2I (20) | 51–283 | 30–192 | 4 |
| 2I-B, braid-correct first 24 | 257 | 194 | 4 |
| H6, height-6 sphere (20 seeds) | 22–120 | 9–162 | 6 |
| LIN, A·2O (5) | 0–5 | 0–1 | 2, not unit |

- **The catalogs lose to generic points.** Under both common tables,
  every group catalog passes XOR robustly in fewer subsets than every
  generic rational null (G) at every N = 3–7. The margin is 1.3–6× at
  N = 5–7, and no catalog is above AX's range.
  - This is KC 19.7's float ordering (Haar > 2I > 2O), now exact. G is
    generic, but it is not Haar: it is the inverse stereographic image
    of a rational box (D126 Part D).
- **Against height-matched nulls there is no consistent difference.**
  - At N = 3 the catalogs fall inside the H5/H6 range.
  - At N = 4, 2O (7,696) and 2I-B (8,549) are below all 40 seeds, and
    2I-D94 (8,914) is at the bottom edge of the range (T13).
  - At N = 5–6 the catalogs sit inside the null range, except
    2I-D94, which is above it.
  - At N = 7, 2O is above all 40 seeds only under T13, which is 2O's
    own table: under R13, H5 reaches 272 against 2O's 265. 2I-D94
    stays above them under both tables.
  - Each catalog is one draw; the nulls are 20 seeds each, and there
    is no significance test. N = 7 passes are 0.003–0.6% of the
    subsets: the extreme tail.
- **2I's standing is entry order, not 2I.** 20 random 24-subsets of 2I
  pass 51–283 (T13) and 30–192 (R13) at N = 7, inside the H5/H6 range.
  - Only D94's BFS prefix (754 / 758) stands out.
  - The braid-correct first 24 (2I-B) pass 257 / 194, below 2O under
    T13. It shares 19 of its 24 points with D94's prefix.
  - So KC 19.7's "2I beats 2O" at N = 7 holds for D94's prefix, not
    for 2I.
- **The deficit against generic points is at least largely a height
  effect.**
  - At N = 5–6, robust XOR roughly follows height: G and AX highest, AN
    next, the catalogs with H5 and H6 below, LIN lowest. At N = 7, AN
    falls among the catalogs.
  - These rows do not show that group structure, as such, lowers XOR
    counts below those of equally low-height points.
- **Coincidences alone do not decide the counts.**
  - LIN has 2O's entire relation lattice, so its XOR concursus equals
    2O's in every row (asserted). Yet it falls to 0–5. LIN also changes
    the weights' norms (max/min |Ap| 2.5–5.0), not only directions.
  - 2O has XOR concursus in 168,864 of the N = 7 subsets (49%), about
    4–17× H6's 10,046–38,116, and it still passes more often than H6.

## Point sets (Part A)

All built exactly. Every set has two sign conventions: D119's BFS
signs (the anchor) and the canonical rule "first nonzero part positive"
(the comparison, one rule for all sets).

| set | N | construction |
|---|---|---|
| 2O | 24 | D119's ζ₈ catalog (D66's generators, cellae closure) |
| 2I-D94 | 24 | D119's first 24 of 2I (D94's generators) |
| 2I-B | 24 | first 24 of 2I closed from a braid-correct pair |
| 2I-R | 24 | 20 random 24-subsets of 2I (control, after review) |
| 2T | 12 | the rational points of 2O |
| H6, H5 | 24 | D126 Part F: random subsets of the height-H sphere, 20 seeds |
| G | 24 | D126 Part D (b): inverse stereographic image of a rational box, 5 seeds |
| AX | 24 | each 2O point replaced by a random rational angle on its own axis |
| AN | 24 | each 2O point's real part on a random integer axis not in T13 |
| LIN | 24 | A·2O, A ∈ GL4(Q) with entries in [−3, 3], every det ≠ 0 |
| 2O sgn, 2I-D94 sgn | 24 | 5 random sign draws each (control, after review) |
| H6, H5 | 12 | the same at N = 12, against 2T |

- **Braid-correct generators.** Two order-10 elements of 2I with
  aba = bab exactly and their axes at cos²φ = 1/5 (note 41). Found in
  2I itself as elements 3 and 6; a is D94's own t. Closed to 60 points.
  D94's own pair has orders 6 and 10 and fails sts = tst (KC 53.3,
  checked).
- **Tables.**
  - T13 is 2O's own table in its order, written as integer directions.
    2O judged with T13 reproduces D119's rows.
  - R13 is 13 seeded random integer directions, none parallel to T13.
    It has one near-duplicate pair, about 7.2° apart (T13's smallest
    angle is 35.3°), so T13 against R13 also compares how well a table
    is spread.
  - Own tables differ in size: 2O 13, the 2I first-24 sets 12, 2I-R
    16–21, nulls 13–24. Own-table rows are never compared across sets.
- **Heights.** "Height" is the lcm of the coordinate denominators. Each
  H6 set holds a few points of height 1–3 (seed 0: {1: 1, 2: 1, 3: 9,
  6: 13}), including 1–2 points of 2T; each H5 set holds 0–2.

## Relation profiles (Part B)

Computed with `reticulum`, as in D126: per set, the norm-3 relations
and the odd-norm {−1, 0, 1} relations (the only coincidences that can
matter for XOR) within norm 8 (N = 24) or 10 (N = 12).

| set | norm 3 | odd ±1 within R |
|---|---:|---:|
| 2O | 32 | 2,720 |
| 2I-B | 18 | 1,976 |
| 2I-D94 | 15 | 1,755 |
| 2T | 16 | 320 |
| H6 | 1–2 | ~1,000 (seeds 0–7 = D126's oracle pins) |
| H5, G, AX | 0 | 0 |
| AN | 0–1 | 0–1 |
| LIN | 32 | 2,720 (the same lattice as 2O) |

AX and AN are not height-matched: random angles and random axes give
heights of 10⁴–10²⁶ and essentially no coincidences. They remove the
coincidences and change geometry at once.

## Passing subsets (Part C)

264 jobs. Every set runs under T13 and R13. The own table runs for
all sets except the 60 control sets, since own-table rows are never
compared across sets. Each job runs in a child process of the demo's
binary; the output does not depend on scheduling (two runs agreed
byte for byte).

**Ties.** Tied XOR subsets at N = 7 (of 346,104):

| set | T13 | R13 |
|---|---|---|
| G | 0 | 0 |
| AX | 0–11 | 0–11 |
| AN | ~84k | ~84k |
| catalogs | 164k–291k | 155k–165k |
| H5 | 111k–167k | 111k–167k |
| H6 | 290k–303k | 290k–303k |

- H6's ties are all sector ties: identical under T13 and R13.
- 2O gains axis ties under T13, its own table (290,947 against
  155,535).
- **"Possible" is uninformative where ties are dense** (D122): H6
  passes 82–474 possible against 22–120 robust. Robust is the headline
  mode.

**AND and MAJ.** These verdicts depend on each weight's sign:

| robust at N = 7, T13 | canonical | D119's signs | 5 random signs |
|---|---|---|---|
| 2O MAJ | 3,603 | 3,603 | 27–489 |
| 2I-D94 MAJ | 43,620 | 23,173 | 262–3,671 |
| 2O AND | 238,848 | 238,848 | 121,656–170,580 |

- The canonical rule puts every point in one lexicographic half-space.
- That makes AND easy (no AND concursus, a theorem, asserted).
- No AND or MAJ ranking here is a group property.

**2T (N = 12)** against H6 and H5 at N = 12, robust XOR:
- at N = 3–5, below every null seed under every table;
- at N = 6, inside their range;
- at N = 7, above them (T13: 16 against 0; R13: 6 against 0).

## Reading the registered rule

`plan.md` registered: "KC 47.1 is supported if no catalog beats its
matched nulls robustly under a common table, and falls if one does."
The review found this cannot be applied as written:

1. **"Matched" is undefined, and the verdict flips with the choice.**
   Against G the catalogs lose at every N. Against H5 and H6 they are
   at or below the range at N = 3–4 and partly above it at N = 7.
2. **N is unspecified.** For example, 2O against H6 under T13 goes
   below, below, inside, inside, above for N = 3–7.
3. **The rule cannot reach KC 47.1's first clause ("not of the
   knots").** No catalog entry is a knot invariant (KC 53.4), so
   neither outcome supports or refutes it.

So no verdict "47.1 supported" or "47.1 falls" is drawn. What this
demo does settle, for KC 47.1's second clause (does group structure
help or hurt against a fair null):
- against generic nulls the catalogs lose;
- against height-matched nulls there is no consistent difference, and
  the direction depends on N and on entry order.

**For KC 48.2** ("group structure lowers capacity through ties and
coincident sums"):
- the generic nulls differ from the catalogs in height as much as in
  group;
- among height-matched sets, more coincidences did not mean fewer
  passes: 2O has more than H6 and passes more.

The prediction is not confirmed.

## Interpretation [interpretation]

- Under one exact activation, point sets with no small-height
  structure compute XOR in the most subsets. Sets of small height
  (catalogs and spheres alike) are dense with exact ties and do worse.
  This is consistent with KR 47's "the order is the order of
  genericity", read now as height rather than group.
- Within small height, which particular points are taken matters more
  than whether they form a group: D94's 2I prefix stands out, while
  random 2I subsets and the braid-correct prefix do not.
- Nothing here bears on knots.

## Checks (Parts A–C)

32 in the full run, all passing. Among them:
- **D119 anchors:** own tables reproduce D119's exact rows for N = 3–4
  (Part A) and, through the worker processes, N = 3–6 (2O own and T13,
  2I-D94 own, D119's signs).
- **R13 anchor:** 2O under R13 equals the review's independent oracle
  in every row, N = 3–7, all six numbers.
- **XOR identical under every sign choice:** BFS signs and 10 random
  draws.
- **Coinciding sums:**
  - LIN's XOR concursus = 2O's in every row (the same relation
    lattice). AND/MAJ equality is reported as observed: it holds in
    150 of 150 rows, but is not a theorem.
  - Concursus is 0 for every null with no ±1 relation within norm 8.
  - H5's XOR concursus is 0 in every row, 20 seeds (the odd-height
    lemma).
  - AND concursus is 0 for every canonical set (theorem).
- **Profiles:** 2O and 2T match D126's oracle pins; H6 seeds 0–7 and
  H5 seeds 0–2 match D126 Part F's pins; G has no relation within norm
  8; LIN's profile equals 2O's.
- **Constructions:** the braid relation and orders; T13 is 2O's table
  in its order; R13 is not parallel to T13; AX keeps the axes and AN
  the angles; det ≠ 0 for every seed; the registry sizes; every sign
  draw flips at least one point.

## Plants

20 planted faults, each run with `silva.planta` against the quick
mode (`D127_CELER=1`, N = 3–4). All are red. The first versions of two
checks were too weak, and both were strengthened:
- **Canonical check:** it called the same `canonical()` it was meant
  to check, so it was circular. It now reads the sign of the first
  nonzero part directly.
- **Random signs:** they used the LCG's lowest bit, which only
  alternates, so every "random" draw flipped the same 12 of 24 points.
  A high bit is used now, and a draw that flips nothing is refused.

## Review (2026-10-14)

An independent reviewer (recensor-d127) wrote its own Python Fraction
oracle, without the house library:
- closed-form cos² boundaries, exact in Z[√d], with certified brackets
  where a boundary lies outside the field;
- possible decided by backtracking per exact point.

It rebuilt all 48 original sets and both tables point by point. It
agreed on 918 exhaustive rows and 9,900 single subsets at N = 5–7: 0
disagreements.

Findings: 0 H, 6 M, 6 L, 6 N.
- **M1:** the registered rule was underspecified.
- **M2:** entry order.
- **M3:** too few seeds.
- **M4:** AND/MAJ depend on signs.
- **M5:** the height confound.
- **M6:** LIN changes norms.
- **L1–L6:**
  - the LIN AND/MAJ identity is empirical;
  - there was no R13 anchor;
  - the det check covered seed 0 only;
  - the plan's axis count was wrong;
  - R13 has a near-duplicate pair;
  - H6's heights are mixed.
- **N1–N6:**
  - G is not Haar;
  - the N = 5–6 anchor was missing;
  - AND concursus is 0 by a theorem;
  - LIN seed 4 has 13 own axes;
  - 2O's concursus does not bind;
  - runtimes.

Every finding was acted on: the 2I-R, seed and sign controls were
added; R13 and D119 N = 5–6 are anchored; the theorem checks were
narrowed to their scope; the wording was changed. The reading above is
the review's, with the confounds named.

## Not covered

- Fields beyond quadratic, and catalogs beyond these four.
- A height-matched non-group control at the catalogs' own heights (2
  and 4): there the rational unit points are 2T itself (D126).
- Significance tests. Every comparison is a catalog (one draw) against
  a seed range.
- Mechanisms: why small-height sets are tie-dense at the cell
  boundaries, and why D94's 2I prefix stands out.
- Knots (KC 53.4) and P3 (verdicts on knot classes).

## Run

```
./bin/aedilis knotapel/demo_127_matched_nulls/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
```

Part C runs 264 jobs in up to 8 worker processes (`D127_OPERARII`).
Each N = 24 job takes about 55 s alone. On a loaded machine the run
takes about 1.5–2 hours; `D127_TEMPORA=1` prints the wall time.
- `D127_CELER=1` runs N = 3–4 only (about 20 s; used by the plants).
- `D127_SERIAL=1` runs every job in one process.

Frozen in `demo-snapshot.c` via `knotapel/archive.sh`.
