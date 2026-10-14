# Demo 127: Group catalogs against matched nulls (audit P1) (plan)

Status: approved by Fran (2026-10-14), with every recommendation
below. Task T6 of
`project-specs/cellae-plan-2.md`; audit P1 (ledger ...E72XG3); settles
or narrows KC 47, 48 and 19.7. First consumer of `cellae`'s signed
encoding (`activatio_iudica`) outside its credo.

## Question

Under ONE fixed exact activation, do the group catalogs compute better
or worse than knot-free point sets? And if they differ, what carries
the difference:
- the axis table (every audited demo built it from the set judged, KC
  47.1.4);
- coincident sums (D126: the catalogs have 2.2-30x more odd-norm,
  XOR-relevant coincidences than nearby-height controls);
- or the geometry itself (angles, directions)?

Registered in advance (KC 47.obj.2.1): KC 47.1 is supported if no
catalog beats its matched nulls ROBUSTLY under a common table, and it
falls if one does. The float prediction (KR 47) is that the catalogs
lose.

**Not in scope:** knots. No catalog entry is a knot invariant (KC
53.4), so no knot-specific claim is possible here. P3 (verdicts on
knot classes) is a separate audit.

## Fixed activation

- **Encoding: signed** (D94, D119-D123): S = sum +-q_i over 2^n masks.
  Coincidences are therefore relations with entries in {-1, 0, 1}
  (KC 48.4.1.1). Named in every table header.
- **Cell: D94's phase cell** (`CELLAE_CELLA_D94`), ladder k in
  {6, 12, 24}, "computes" = some k passes (D94).
- **Functions:** XOR, AND, MAJ.
- **Modes:** rule, robust, possible; plus tied, concursus (same exact
  sum with different values, i.e. a coincidence that bites this
  function) and undecided (SAT budget; expected 0, asserted).
- **D122 caveat, carried:** "possible" is vacuous where every vector
  is tied two ways. Robust is the headline mode; possible is reported,
  not used for verdicts.

## Point sets (N = 24 each unless noted; modulo sign; exact)

**Catalogs**
| name | built from | field |
|---|---|---|
| 2O | D66's generators (= D119's zeta_8 catalog) | Q(sqrt2) |
| 2I-D94 | D94's generators, first 24 in BFS order (= D119's) | Q(sqrt5) |
| 2I-B | **braid-correct** zeta_10 generators: two order-10 elements of 2I whose axes meet at cos phi = 1/sqrt5 (note 41), chosen from 2I itself; aba = bab checked exactly; first 24 in BFS order | Q(sqrt5) |
| 2T | the 12 rational elements of 2O (N = 12; its own nulls) | Q |

2I-B is the only catalog here that is a braid-group image with a
correct generator pair (KC 53.3). It is also a second "first 24 of
2I", so entry order (D125's lesson) is visible too.

**Nulls** (5 seeds each, fixed LCG; heights reported for every set):
| null | construction | what it keeps / removes |
|---|---|---|
| H6 | random rational unit points of height 6 (D126 Part F sphere) | small height, no group; has coincidences (~1,000 odd-norm at N = 24) |
| H5 | the same at height 5 | odd height: NO odd-norm coincidence by theorem (KC 48.4.8), so no XOR coincidence at all |
| G | high-height rational points (D126 Part D, denominators ~1e30) | generic: no relation within radius; the exact version of KR 47's Haar control |
| AX | 2O's 13 axis directions with random rational angles (rational points on the conics c^2 + abs(u)^2 t^2 = 1), axis multiplicities as in 2O | 2O's directions and table, not its angles |
| AN | 2O's angles (its real parts) on random integer axes of the right norm class | 2O's angles, not its directions |
| LIN | A·2O for random A in GL4(Q), small integer entries | 2O's ENTIRE relation lattice (every coincidence), different geometry, not unit (D126 Part E) |

Rational nulls are written in the compared catalog's field (Q embeds
in each). AX, AN and LIN are 2O-specific. For 2T: H6 and H5 at
N = 12 only.

## Tables (each set judged three ways)

- **own**: built from the set itself, as every audited demo did;
- **T13**: 2O's 13 axes, written as rational directions, so the same
  table works in every field;
- **R13**: 13 group-free directions, random integer vectors with
  entries in [-9, 9], fixed seed. Its minimum axis angle is reported
  beside 2O's.

The common tables (T13, R13) are the "one fixed activation". "own" is
reported only to connect with D119; 2I's own table (31 axes) changes
the cell count, so no own-table comparison across catalogs is made.

## Parts

**A. Construction and checks** (assert characterizations, not counts):
- D119 anchor: 2O and 2I-D94 with their own tables reproduce D119's
  exact rows N = 3-4 (lentus: 5-6);
- 2I-B: both generators have order 10, aba = bab exactly, closure is
  120 elements, its first 24 differ from 2I-D94's;
- nulls: unit norm exactly (except LIN), distinct modulo sign, height
  as stated, none closed under multiplication (near-vacuous, said so);
- AX's axes are exactly T13; AN's angle multiset is exactly 2O's;
- LIN's relation lattice equals 2O's (same reticulum shells);
- R13 shares no direction with T13.

**B. Relation profiles** (reticulum, as D126): per set, the norm-3
count and the odd-norm {-1, 0, 1} count within D126's radius (8 for
N = 24, 10 for N = 12). These are the coincidences that the rows of
Part C can then be read against.

**C. Capacity rows.** For every set and both common tables (plus
own): N = 3-6 exhaustive over C(24, N); N = 7 sampled, 100,000 sets
from D120's LCG stream and marked as SAMPLED. Comparisons stay on one
scale (D119's lesson). Per row: sets, rule, robust, possible, tied,
concursus, undecided. Nulls: min-max over seeds, no significance test.
The work runs in child processes of the demo's own binary
(`--opus N`, as in D125); the output is byte-identical to serial.

**D. Reading.** Three contrasts, at robust:
1. catalog against H6/G under T13 and R13: does the group win or lose
   with the table fixed?
2. 2O against LIN: coincidences fixed, geometry changed;
3. 2O against AX and AN: directions against angles.

Each conclusion carries its own sample and seed range.

## Size and time (estimate, to be measured)

About 30 point sets x 3 tables. One (set, table) through N = 7 is
about 60 s on T4b's numbers, so ~90 min serial and ~20 min with
workers. G may run slower: its denominators exceed the integer image,
so its points take the algebraic path; it is measured first.

## Tasks (one per turn, Fran's go-ahead each)

1. **T6a**: Parts A and B, with checks and plants.
2. **T6b**: Part C (workers, timing), plants.
3. **T6c**: findings.md, claims.md updates (KC 19.7, 47, 48), ledger,
   freeze with archive.sh.
4. **T6d**: independent review with its own oracle (a Python Fraction
   recount of a sample of rows), then fixes.

## Open for Fran

1. The group-free table: random R13 (proposed), or a deterministic
   well-spread set?
2. The nulls above, including LIN (the strongest separator, but not
   unit) and the 5 seeds per kind.
3. 2I-B (braid-correct zeta_10), proposed: yes.
4. 2T at N = 12: proposed yes, since it is cheap and rigid (KC
   48.4.4.2).

## Decisions made during T6a

- **Representatives.** Under the signed encoding the sign of a weight
  matters for AND and MAJ: flipping q_i relabels the masks, so AND
  becomes AND with input i negated. XOR is invariant, since every
  label flips. cellae's closure keeps the first element BFS produced
  from each {q, -q}, so D119's catalogs have mixed signs (2 of 2O's
  24 and 3 of 2I-D94's 24 differ from "first nonzero part positive").
  So:
  - Part C compares every set under ONE rule (canonical: first nonzero
    part positive);
  - the catalogs in D119's BFS signs are kept as the anchor;
  - AND and MAJ are also reported for the catalogs in BFS signs, so
    the effect of the convention is visible.
- **AX and AN are not height-matched.** Random angles (AX) and random
  axes (AN) give heights of 10^4 to 10^26 and essentially no
  coincidences. They remove coincidences AND change geometry; LIN is
  the null that keeps every coincidence.

## Decisions made during T6b

- **N = 7 exhaustive, not sampled.** One (set, table) job through N = 7
  over all 346,104 subsets takes ~55 s, so every row is exhaustive.
  No comparison mixes a sampled scale with an exhaustive one (D119's
  lesson).
- **Checks that tie the parts together:**
  - the workers' rows reproduce D119, which proves that jobs, sets and
    tables line up;
  - XOR is identical under BFS and canonical signs (a theorem);
  - LIN's concursus = 2O's in every row (the same relation lattice);
  - concursus is 0 wherever Part B found no +-1 relation within norm 8;
  - H5 XOR concursus is 0 (the odd-height lemma).
- `D127_CELER=1` (plants): N = 3-4, about 20 s.

