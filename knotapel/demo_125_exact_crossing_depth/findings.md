# Demo 125: D82's Crossing Depth, Exactly — Findings

D82 ("Crossing Depth and Computational Role") recorded four things about
the ζ₁₂ closure catalog (4,096 entries, round 8 cut by `MAX_QCAT`):
- **the depth law:** with D81's per-round ladder (max XOR 6 6 8 8 10 10
  12 12 12), "max_xor ≈ depth + 6": capacity grows linearly in depth,
  and D81's logarithmic law is an artifact of exponential growth;
- **Part D, "the crux":** at matched sizes, deep subsets beat shallow
  ones. Deep 564 alone reaches XOR12, and the strided 564 set, which has
  the most vocabulary, does not. Hence "algebraic coherence beats
  vocabulary", and "deep entries are 2× more efficient" than the
  1,140-entry prefix where XOR12 first appears;
- **Part B:** every winner is a "shallow core + deep extensions"; the
  generators are in every winner; "XOR12 is the first level to use
  depth-8 entries";
- (D87, same catalog) removing the 121 null entries preserves capacity:
  XOR12, 32 = 32 winners.

All of these are outputs of **one greedy search**, the `find_capacity` of
D81, D82 and D87:
- triples come only from the first 100 entries;
- each level extends the previous level's winners in order, against the
  whole catalog in order;
- a set's k is the first of {1, 6, 8, 10, 12, 16, 20, 24} that
  separates;
- each level stops after 32 winners;
- the search never goes past 6 weights (XOR12).

Cells are D82's `combined_cell` in floating point, with the direction
table capped at 512 (D87: 2,048).

Exhaustive search is out of reach at this size. This demo therefore
audits what that SEARCH finds, under exact cells and under changes of
order. It never says what "the capacity" is.

**Summary.**
- **Exact cells change D82's tables only at the margin.** The catalog is
  exact and equal to D82's float catalog entry for entry. Under the
  exact rule (D82's semantics), every max XOR of D81 and D82 is the
  same except one: round 1 reaches XOR8, through exact ties, and does
  not under robust. Counts move: deep 564 finds 21 XOR12 winners under
  the rule, 13 robustly, 19 in float.
- **The depth law's top is the search's ceiling.** "depth + 6" fits
  only the even rounds 0, 2, 4, 6. At rounds 7 and 8 it predicts XOR13
  to XOR14, but D81/D82 never searched past XOR12. Extended one level
  under the exact rule (from its first 32 XOR12 winners; robust mode was
  not extended), the search finds **no XOR14 at round 7 or 8**, with
  D82's 512 axes and with all axes (975 / 2,043). As far as this search
  reaches, max XOR is 12 at depths 6, 7 and 8; that is a statement about
  the search, not about the catalog.
- **"Algebraic coherence" does not survive a change of entry order.**
  In D82's order, Part D holds under every semantics. In 10 seeded
  orders each:
  - strided 564 reaches XOR12 in **8 of 10**, under the rule and
    robustly;
  - deep 564 in 10 of 10 under the rule but **7 of 10 robustly** (robust
    takes no axis-tie choice, so it isolates the order; the rule's extra
    three are tie decisions);
  - shallow 564 (rounds 0–5) in 1 of 10;
  - **9 of 10** random 564-entry samples reach it (81–88% of their
    entries from rounds 6–8).

  What survives is that the first 564 entries (239 axes) rarely reach
  XOR12 and most sets drawn from deeper rounds do. Deep is not better
  than strided.
- **"XOR12 is the first level to use depth-8 entries" comes from the
  512-axis cap.** Without the cap no XOR12 winner uses a depth-8 entry,
  under rule or robust. The depth-0 "core" of every winner comes from
  the search: triples from the first 100 entries, then extension in
  order.
- **D87's ζ₁₂ result holds exactly, as far as its search reaches.**
  Non-null reaches XOR12 under the rule and robustly. Both catalogs sit
  at the search's ceiling (XOR12, 32 winners per level, 6 weights; XOR14
  not tried for non-null). "32 = 32" is that stop, not a count of
  capacity.

Run: `./bin/aedilis knotapel/demo_125_exact_crossing_depth/main.c &&
bash build/aedilis/main/struere.sh && ./build/aedilis/main/main`.
It takes about 14 min on 8 cores and runs 28 checks.
The 149 searches run as child processes of the demo's own binary
(`--opus N`, `processus`), at most 8 at once, longest first. Output is
printed afterwards from the collected results, so it is byte-identical
to the one-process run (`D125_SERIAL=1`, about 32 min), checked on the
quick plan with `cmp` and on the full run through every pinned checksum.
The floor is the single slowest search: round 8, rule mode, uncapped,
to XOR14, about 9 min. `D125_VICTORES=<path>` writes
the winners for `oracle.py`; `D125_TEMPORA=1` prints timings on stderr.

## The catalog and the replica (Parts A, B)

The exact catalog is `cellae`'s `catalogus_clausura` over Q(√3), with
D82's generators and BFS order:
- it equals D82's float catalog entry for entry (all four components
  within 1e-12, same order, same depths);
- the census is 5, 12, 34, 76, 148, 289, 576, 1,152 and 1,804, so the
  round prefixes are D81's 5, 17, 51, 127, 275, 564, 1,140, 2,292 and
  4,096;
- there are 121 nulls (Re q = 0), the same as D87's |a| < 1e-10;
- axes, uncapped: 2,043 full, 1,976 non-null, 975 at round 7, 517
  strided 564, 476 deep 564.

The float search is copied verbatim from D81/D82/D87. Only `MAX_DIR` and
`MAX_ACT` became run-time values. The originals were rebuilt and run on
this machine first (D81 4 min, D82 3.5 min, D87 54 s), and the replica
reproduces all of them:
- D81's table, dirs and angles included;
- D82's Part B depth histograms and Part D table, vocabulary included;
- D87's full / non-null: 2,043 / 1,976 directions, 32 32 32 32 both.

## Exact cells (Part C)

The same search was run with `cellae`'s exact cells under each semantics:
- **rule** = D82's: sector floor, first maximal axis;
- **robust** = every tie resolution must separate;
- **capped** = D82's first 512 axes, with k skipped when k (axes + 1)
  > 16,384;
- **uncapped** = all axes, all k. This lifts BOTH caps: it is not D82
  with only `MAX_DIR` removed, which at 2,043 axes would still skip
  every k ≥ 10 (k · 2,044 > 16,384).

With 512 axes the cell cap never binds (24 · 513 = 12,312 < 16,384), so
"capped" differs from "uncapped" only through the axis table.

Max XOR:

| subset | float | rule capped | rule uncapped | robust capped | robust uncapped |
|---|---|---|---|---|---|
| round 0 (5) | 6 | 6 | 6 | 6 | 6 |
| round 1 (17) | 6 | **8** | **8** | 6 | 6 |
| round 2 (51) | 8 | 8 | 8 | 8 | 8 |
| round 3 (127) | 8 | 8 | 8 | 8 | 8 |
| round 4 (275) | 10 | 10 | 10 | 10 | 10 |
| round 5 (564) | 10 | 10 | 10 | 10 | 10 |
| round 6 (1,140) | 12 | 12 | 12 | 12 | 12 |
| round 7 (2,292) | 12 | 12 | 12 | 12 | 12 |
| round 8 (4,096) | 12 | 12 | 12 | 12 | 12 |
| strided 127 | 10 | 10 | 10 | 10 | 10 |
| deep 127 | 10 | 10 | 10 | 10 | 10 |
| strided 564 | 10 | 10 | 10 | 10 | 10 |
| deep 564 | 12 | 12 | 12 | 12 | 12 |
| non-null (3,975) | 12 | — | 12 | — | 12 |

XOR12 winners found (the search stops at 32):

| subset | float | rule capped | rule uncapped | robust capped | robust uncapped |
|---|---|---|---|---|---|
| round 6 | 12 | 14 | 14 | 8 | 8 |
| round 7 | 32 | 32 | 32 | 27 | 32 |
| round 8 | 32 | 32 | 32 | 29 | 32 |
| deep 564 | 19 | 21 | 21 | 13 | 13 |

Other counts that move: strided 127 XOR10 is 26 in float, 22 under the
rule and 20 robustly; round 0 XOR6 is 8 under the rule and 4 robustly.

**The differences are exact ties, decided by float rounding.** The
float and exact rule winner lists differ at every subset, already at
XOR6. At round 0 they hold the same sets with different first k:
- [0, 1, 3] passes at k = 24 in float and at k = 12 exactly;
- generator sums sit exactly on sector boundaries (60° × 12 / 360 =
  2), and float rounding (59.999…) puts them one sector lower.

Many cell decisions are ties. Over the whole full-catalog rule search
with the cap, 19.3 M of 105 M cell decisions needed the exact path, and
15.5 M of those were ties. At round 1, XOR8 exists only by such ties:
the rule finds 3 winners, robust none.

## The depth law (Part C extended, Part G)

| semantics | max XOR, rounds 0–8 | XOR14 at r7, r8 | max XOR = depth + 6 at |
|---|---|---|---|
| float (D82) | 6 6 8 8 10 10 12 12 12 | never searched | 0 2 4 6 |
| rule capped | 6 8 8 8 10 10 12 12 12 | 0, 0 | 0 2 4 6 |
| rule uncapped | 6 8 8 8 10 10 12 12 12 | 0, 0 | 0 2 4 6 |
| robust capped | 6 6 8 8 10 10 12 12 12 | not searched | 0 2 4 6 |
| robust uncapped | 6 6 8 8 10 10 12 12 12 | not searched | 0 2 4 6 |

"max_xor ≈ depth + 6" is a line through the even rounds. At odd rounds
it is depth + 5, and at round 1 under the rule it is depth + 7. At
rounds 7 and 8 the recorded 12 is the search's own ceiling, since
`find_capacity` stops at 6 weights.

The one direct test of the law here searches one level further, at the
two depths where it predicts more. It finds XOR12 winners at both
depths (32 each) but **no XOR14**, under the exact rule, with D82's 512
axes and with every axis. Robust mode was not extended. That is a
statement about this greedy search, and a narrow one:
- triples from the first 100 entries;
- 32 winners per level, and the XOR14 level extends only the first 32
  XOR12 winners, all [0, 1, 3, 6, …] or [0, 1, 4, …] (Part D): one small
  neighbourhood of the catalog;
- 3⁷ sums per candidate.

(The reviewer's spot check: 2,000 random 7-weight extensions of those
winners at each of rounds 7 and 8, none separates.)

It is not a proof that no XOR14 set exists. But the law's only support
above depth 6 was a number the search could not exceed, and extending
the search does not supply XOR14 either.

D81's logarithmic law (0.62 XOR per doubling) is a fit to the same nine
points. It shares the ceiling, and its extrapolations (XOR14 at about
38,000 entries) rest on it.

## Matched sizes and order (Parts C, E)

In D82's own order, Part D holds under every semantics:
- shallow 127 / deep 127 reach XOR8 / XOR10;
- shallow / strided / deep 564 reach XOR10 / XOR10 / XOR12.

But the greedy search depends on entry order, through the triples from
the first 100 entries and the first winners extended. Part E reruns the
three 564 subsets in 10 seeded orders each, plus 10 random 564-entry
samples kept in catalog order, uncapped, up to XOR12.

A shuffle also reorders the axis table, and the rule breaks axis ties
by first maximum, so under the rule a new order changes the search AND
the cells. Robust mode takes no tie choice: every tie resolution must
separate. Its rows therefore isolate the effect of the order.

| subset | axes | XOR12 winners per order (rule) | reach (rule) | XOR12 winners per order (robust) | reach (robust) |
|---|---|---|---|---|---|
| shallow 564 (rounds 0–5) | 239 | 0 0 1 0 0 0 0 0 0 0 | 1/10 | 0 0 1 0 0 0 0 0 0 0 | 1/10 |
| strided 564 | 517 | 20 1 27 4 1 0 7 1 17 0 | 8/10 | 20 1 27 3 2 0 6 1 16 0 | 8/10 |
| deep 564 | 476 | 3 2 2 1 7 7 4 9 3 8 | 10/10 | 0 0 0 1 3 2 1 7 2 4 | 7/10 |
| random 564 samples | 496–525 | 4 18 12 23 20 0 6 32 7 5 | 9/10 | 1 11 12 22 12 0 6 32 5 1 | 9/10 |

The random samples are mostly deep: 81–88% of their entries come from
rounds 6–8 (mean depth 6.7–7.0, 217–268 depth-8 entries). They refute
the deep-versus-strided contrast; they do not show that depth is
irrelevant.

- **Finding 3 ("algebraic coherence beats vocabulary") does not
  survive.** The strided set's failure to reach XOR12 is a property of
  its order: 8 of 10 orders reach it, under the rule and robustly. Deep
  564 reaches it robustly in only 7 of 10. Its 10 of 10 under the rule
  includes three orders that pass only through axis-tie decisions.
  Nothing here distinguishes "coherent" deep entries from a strided or
  random draw.
- **Finding 1 survives in a weaker form.** Shallow 564 (the first five
  rounds, 239 axes) reaches XOR12 once in 10 orders, and the sets from
  deeper rounds almost always do. This demo does not separate depth from
  vocabulary as the cause: the shallow set also has half the axes.
- **Finding 2 ("2× more efficient")** compares deep 564 with the
  shallow prefix. Most 564-entry sets from deeper rounds reach XOR12,
  so the comparison measures how poor the first 564 entries are, not
  something special about depth 8.
- D82's 19 deep XOR12 winners (21 exact rule, 13 robust) are high for
  that subset: other orders find 1–9 under the rule and 0–7 robustly.

## Winner depths (Part D)

D82 Part B's histograms are reproduced in float. Exact rule, full
catalog, XOR12:

| semantics | XOR12 winners | mean depth | depth-8 entries |
|---|---|---|---|
| float (D82) | 32 | 1.98 | 4 |
| rule capped | 32 | 1.96 | 4 |
| rule uncapped | 32 | 1.68 | 0 |
| robust capped | 29 | 2.06 | 2 |
| robust uncapped | 32 | 1.68 | 0 |

**"XOR12 is the first level to use depth-8 entries" holds only with the
512-axis cap.** Without it, no XOR12 winner uses a depth-8 entry, under
the rule or robustly. (D117 saw the mean depth drop to 1.68 uncapped.)

**Every winner has three depth-0 entries (d0:96 at every level above
XOR6) because of the search.** The 32 XOR6 winners are triples [0, 1,
x] from the first 100 entries. Each later level extends the first
winners against the whole catalog in order, so the 32 XOR12 winners are
[0, 1, 3, 6, …] and [0, 1, 4, …] extensions of the first few. "The
generators are irreplaceable" and "shallow core + deep harmonics"
describe this construction. D82's own Part D shows that deep subsets
with no generator reach XOR12.

## D87 (Part F)

| catalog | float (2,048 / 65,536) | rule | robust |
|---|---|---|---|
| full (4,096, 2,043 axes) | XOR12, 32 32 32 32 | XOR12, 32 32 32 32 | XOR12, 32 32 32 32 |
| non-null (3,975, 1,976 axes) | XOR12, 32 32 32 32 | XOR12, 32 32 32 32 | XOR12, 32 32 32 32 |

D87's caps (2,048 directions, 65,536 cells) do not bind, so its float
run is the uncapped semantics. Its ζ₁₂ result holds exactly: the search
reaches XOR12 without nulls under the rule and robustly. Both catalogs
sit at the search's ceiling (XOR12, 32 winners per level, 6 weights),
and XOR14 was not tried for non-null, so "nulls are dispensable" holds
as far as this search reaches.
- "32 = 32" is the stop at 32 winners per level, the D84 MAX_WIN
  pattern, so it says nothing about how many winners exist.
- The other half of D87's "regime transition", nulls indispensable at
  ζ₈, was already voided by D124.

## Oracle

`oracle.py` is independent exact code in Python (Fractions over Q(√3)).
It builds on the cellae reviewer's ζ₁₂ code (recensor-extensio, cellae
T8 review) and shares no code with `cellae`.

It rebuilds the catalog (census, truncation, 121 nulls, axis counts) and
re-decides every winner the demo writes:
- all levels of every Part C search;
- the XOR12 winners of every Part E order and sample.

Each winner must separate at its recorded k and at no earlier ladder k,
under its mode (rule, or robust over every tie resolution) and its axis
table (the first 512 axes, or all).

Sectors use exact boundaries where tan²(mπ/k) lies in Q(√3), and
60-digit decimals otherwise, with an asserted margin. numpy only
shortlists axes within a relative 1e-6 of the float maximum; the choice
among them is exact.

Result on the full run: census OK; **every winner re-decided, 0 wrong**
(5,149 winners in 107 searches, the robust order control included).

With 512 axes the cell cap never binds, so the oracle's branch that
skips k when k (axes + 1) > 16,384 is never exercised on this data.

## Checks

- Part A (5): catalog size, truncation, entry-for-entry equality with
  D82's floats, census = D81's rounds, nulls = D87's.
- Part B (7): D81's ladder, D82's Part D counts and vocabulary, D87's
  full and non-null, and the pinned float table (15 searches).
- Part C (4): the pinned exact tables (13 + 14 + 13 + 14 searches).
- Part D (2): D82's histograms, "first depth-8 level" in float.
- Part E (2): the pinned order control, rule and robust (40 + 40
  searches).
- Part G (8): no XOR14 at rounds 7–8 (rule, one level); the search's
  max XOR is 12 at rounds 6–8 under every semantics; round-1 XOR8 is
  tie-only; Part D holds in D82's order under every semantics; the
  order-control counts (rule, robust); the depth-8 claim with and
  without the cap; D87.

Every search is pinned: n found per level, max XOR, axes, and a checksum
of every winner set with its k. A live rerun against changed libraries
is therefore a regression check of `cellae`. The checksum was added
after a plant survived: taking triples from the first 30 entries changes
which sets win but not how many, so count-only pins passed it.

## Plants (10)

Each plant is one exact replacement in a scratch copy of `main.c`, or of
`lib/cellae.c`. The copy is built with clang against the house
libraries and run in quick mode (`D125_CELER=1`: subsets up to 600
entries, 2 orders and 2 samples in Part E), and then the oracle is run
on its winner dump. The tree is never touched.

| plant | caught by |
|---|---|
| replica sector rounding (+1e-9) | pinned float table |
| replica axis = last maximum (`>=`) | pinned float table |
| exact generators g2 / g2* swapped | catalog equality with D82's floats |
| triples from the first 30 entries, not 100 | winner checksums of the exact tables (5 checks); survived the first, count-only pins |
| axis cap 511, not 512 | pinned capped tables (rule, robust) |
| winner file: k moved one rung later | oracle: 2,184 of 3,046 winners wrong |
| strided indices rounded | pinned float and exact tables (6 checks) |
| shuffle off by one | pinned order control |
| `cellae`: rule takes the lower sector at a tie | pinned rule tables, order control; oracle: 79 wrong |
| `cellae`: rule takes the last maximal axis | pinned rule tables, order control; oracle: 77 wrong |

The two library plants leave the robust tables green, as they should:
robust mode already ranges over every tie resolution.

## Review (2026-10-13, recensor-extensio)

No high-severity finding. The reviewer reran everything in scratch from
the snapshot:
- the full demo, 26/26;
- `oracle.py`, 0 wrong;
- parallel against serial, identical;
- its own independent exact greedy search, which reproduced the pins,
  checksums included, on 14 searches;
- 2,000 random XOR14 candidates at each of rounds 7 and 8, none of
  which separates.

It also checked the float replica line by line against D81/D82/D87.

Fixed after the review:
- **M1:** the correction notes stated "no XOR14 at depths 7–8" and "max
  XOR is 12 under every semantics" as facts about the catalog. The XOR14
  probe ran under the rule only, extending the first 32 XOR12 winners.
  The notes and the Part G check now say so.
- **L1:** under the rule, a shuffle changes the axis tie order too, so
  the order control mixed search order with cell semantics. Part E now
  also runs in robust mode: deep 564 drops from 10/10 to 7/10, below
  strided's 8/10. The random samples' composition (mostly rounds 6–8)
  is stated next to them.
- **L2:** D87's "dispensable" is qualified "as far as this search
  reaches".
- **L3:** the cap sentence ("uncapped" lifts both caps; the cell cap
  never binds at 512 axes).

## Not covered

- **Exhaustive capacity.** Nothing here says how many XOR12 or XOR14
  sets exist, only what this search finds.
- **The 127-entry subsets under other orders**, and orders for the
  round prefixes.
- **Depth versus vocabulary as the cause** of shallow 564's weakness.
- **D89–D93** build "mechanisms of the depth law" on D82's ladder. They
  are flagged, not re-audited.
