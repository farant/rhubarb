# Demo 121: Exact Derived Series — Findings

D95 and D96 split ζ₈ (the 24 elements of 2O mod sign) two ways:
- by its derived series 24 > 12 > 4 > 1 (levels 12 / 8 / 3 / 1);
- by bracket-null, i.e. Re = 0 (9 null, 15 not).

They then measured the XOR (and, in D95, AND) capacity of the pieces
with D94's float activation:
- COMM against NON-COMM;
- the 90° "0 + 0 = 26%" split;
- ζ₈-COMM against the first 12 elements of 2I;
- cells A–E, their pairs, and their triples.

Their group theory is exact integer arithmetic. Their capacity counts
are floats. This demo recounts every one of those numbers exactly, on
D120's engine.

Run: `./bin/aedilis knotapel/demo_121_exact_derived_series/main.c &&
bash build/aedilis/main/struere.sh && ./build/aedilis/main/main`
- full run: 34 s, 26 checks;
- `DEMO121_CELER=1`: Part A plus D96's cells and pairs, 0.4 s, 10
  checks (for plants).

Oracle: `python3 -I oracle.py CELLS N` (XOR).
- It uses the D119 review agent's exact arithmetic in Q(√2, √3, √5):
  every boundary is compared, there is no float filter, and "possible"
  is decided by its own search.
- It defines the cells from first principles, not from main.c:
  - E = the identity;
  - B: |Re| = √2/2;
  - C: |Re| = 1/2;
  - D = Q₈ (±i, ±j, ±k);
  - A = the other Re = 0 elements.
- It agrees with main.c on all 16 rows checked: B N = 3, 6; C N = 5,
  6; D N = 3; A N = 3, 4; A+D N = 3–6; B+C N = 3, 4; C+D N = 3, 4;
  A+B+C+E N = 3.

## Headline

**Most of D95/D96's qualitative claims hold exactly, even under every
tie resolution. The ones that rest on the 90° (null) elements do not:
those elements put every sum exactly on a sector boundary. One more, D96's
"optimal catalog is A+B+C", fails outright, on D96's own float numbers
as well.**

| claim | float (D95/D96) | exact rule | robust | possible | verdict |
|---|---|---|---|---|---|
| Cell B is 100% at every N | 20/20, 15/15, 6/6, 1/1 | same | same | same | holds under every resolution |
| B+C wins at every N | 96.7, 94.0, 73.2, 31.0% | | 96.7, 92.8, 71.7, 29.0% | | holds robustly |
| A+B+C(+E) beats ALL at every N ("Q₈ is noise") | 74.1 vs 71.9 … | yes | 74.1/71.9, 74.8/72.4, 40.9/36.5, 10.3/7.5% | yes | holds under rule, robust and possible |
| removing C hurts most at N = 3–5, B at N = 6 | | | same ranking | | holds robustly |
| COMM survives N = 7 (8), NON-COMM dies (0) | 8 / 0 | 8 / 0 | 8 / 0 | 8 / 0 | holds under every resolution |
| 2I first 12 beats ζ₈-COMM at N = 3–5, crossover at 6–7 | 1.56, 1.64, 2.15× | | 1.56, 1.63, 1.98× | | holds robustly |
| "0 + 0 = 26%" (all-null N = 4) | 33/126 | 27 | **15 (11.9%)** | **102 (81%)** | magnitude is a tie artifact |
| all-null "zero elsewhere" (N = 3, 5, 6) | 0, 0, 0 | 0, 0, 0 | 0, 0, 0 | **80, 72, 25** | robust only |
| null halves "dead alone" | 0 | 0 | 0 | outer-null N = 3, 4: **16/20, 3/15**; Q₈ (one set, N = 3 only): **1/1** | robust only |
| cell C at N = 6 | 16/28 (57%) | 16 | **4 (14%)** | **28 (100%)** | decided by ties |
| A is a "synergy enhancer"; optimal catalog = A+B+C | — | | B+C 96.7, 92.8, 71.7, 29.0% vs A+B+C(+E) 74.1, 74.8, 40.9, 10.3% | | **fails**, in D96's own floats too (B+C 96.7, 94.0, 73.2, 31.0 vs 74.1, 77.5, 45.1, 13.0%) |

## The 90° mechanism

A null element has real part exactly 0, so any signed sum of null
elements also has real part exactly 0. Its rotation angle is then
exactly 180°, which is the sector boundary m = k/2 for every k in
{6, 12, 24}.

**Every set of every null subset is tied** (checked). So the "0 + 0"
result is decided by how acos(0) rounds and by the axis tie-breaks:
- robustly, the mix computes where the outer half does not (15/126
  at N = 4 against 0/15; the Q₈ half has 3 elements and no N = 4
  sets at all);
- that robust gain is 11.9%, not 26%;
- under "possible" the outer half computes at N = 3 and 4 (16/20, 3/15;
  0 at N = 5, 6), Q₈'s single N = 3 set passes, and the mix computes at
  every N.

"Algebra wins over geometry at matched half-angle" survives only in
its robust, weaker form.

## D94 and D95 count the same table differently

| ζ₈ ALL(24) | D94 floats | D95 floats | exact rule | robust | possible |
|---|---|---|---|---|---|
| N = 3 XOR | 1,480 | 1,456 | 1,456 | 1,456 | 1,938 |
| N = 4 XOR | 8,010 | 7,974 | 7,908 | 7,696 | 8,678 |
| N = 5 XOR | 17,201 | 17,325 | 16,479 | 15,497 | 24,714 |
| N = 6 XOR | 12,983 | 12,975 | 12,268 | 10,070 | 22,979 |
| N = 3 AND | 1,907 | 1,799 | 1,799 | 1,799 | 1,970 |

- Both demos count the same 24 elements, with the same activation and
  the same rule.
- D94 builds its floats by float BFS (products of cos/sin generators).
  D95 converts its exact integers ((a + b√2)/2).
- The floats differ in the last bit, and at exact ties that bit
  decides. Both demos list the axes in the same order; the exact rule
  (which does depend on that order) gives the same counts for both, so
  the float representation alone accounts for the difference.
- D121's exact counts equal D119's exact counts at every N ≤ 6, XOR
  and AND (checked). Once exact, the two implementations agree.
- New exact numbers at N = 7 (exhaustive; D94 only sampled):
  - XOR: rule 606, robust 456, possible 2,777;
  - AND: rule 247,903, robust 238,848, possible 265,616.

## Method

- **Group.** D95's integer code, ported with names changed for latina.h,
  gives:
  - 12 single commutators, so G1 = 12;
  - the series 24 > 12 > 4 > 1, with levels 12/8/3/1;
  - cells A–E = 6/6/8/3/1, with 9 null.

  G1 and G2 are checked closed under the house `quaternio` product,
  independently of D95's code. 2I is checked perfect: all 60 elements
  are single commutators.
- **Catalogs.**
  - Each subset is its own catalog in BFS order with its own axes, as
    D95/D96 do.
  - Floats use D95's own conversion.
  - The exact quaternions are parsed from the integers, and the
    integer copy is checked against them.
  - The axis counts match the numbers D95/D96 printed for every subset.
- **Float replica.** Every float count D95 and D96 printed is reproduced,
  for all 22 subsets × N, XOR and AND.
- **Exact layer.** D120's: integer sums, surdus signs, and extensio as a
  counted fallback, which was never used. The cross-validation of
  surdus against the forced fallback is repeated in Part A (3,640
  decisions).
- **Verdicts.** Exact rule, robust, possible (DPLL), OR-ed over k as
  D95 does.

## Plants (11, all red, full mode)

- the replica's √2 constant shortened
- null defined by the rational part only
- cells C and D swapped
- the integer scale set to 1
- outer-null including Q₈
- the B+C pair including A
- the quaternion lookup ignoring sign
- the rate comparison inverted
- "all tied" reading the float count
- 2I-first-12 with 13 elements
- G2 without the identity (not a subgroup)

The first G2 plant (`level >= 1 && level != 1`) was an equivalent
mutant: it is the same set, so it survived correctly. It was re-planted.

## Review (2026-10-09, recensor-extensio)

No errors in numbers or checks. The reviewer confirmed the subset
construction against D95/D96 (BFS and generator order, cells from
level × null, triples including E) and found that the float and axis
match cannot be coincidence. It also confirmed each judge_claims check
and the 90° argument; no axis ties are needed for "every set tied".
The oracle's free-reuse step was shown sound, and 14 more oracle rows
agree. LOW findings, applied:
- the "0 + 0" wording (Q₈ has no N = 4 sets; outer-null "possible" at
  N = 3–4 only);
- the axis-order attribution for the D94/D95 difference, removed;
- "Q₈-null 0 at all N" is vacuous beyond N = 3;
- an unjudged D96 headline ("A is a synergy enhancer, optimal catalog
  A+B+C") fails on D96's own numbers, so it is now a claims row and a
  check.

## Not covered

- D97 ("why is Cell B perfect?") is left out. Cell B's 100% holds
  under every resolution, so D97's question stands.
- MAJ is not counted; neither demo tested it.
- The interpretive text in the explorers' log is not judged beyond the
  claims in the table above. Examples are the Barrington reading and
  the E₇ link.
