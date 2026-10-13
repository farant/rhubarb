# Demo 124: D84's Null States, Exactly — Findings

D84 asked whether the bracket-null entries of the ζ₈ catalog are
computational dead weight. These are the 9 of 24 entries with Re(q) = 0,
whose half-angle is exactly 90°. Its recorded answer, in the atlas entry
and the headline, is that they are not:
- removing them drops capacity from XOR8 to XOR6;
- that is worse than removing a random 15 entries (mean 7.8);
- the null-only subset still reaches XOR6;
- null-only XOR6 is flat in k_sec and beats non-null at k_sec = 1;
- nulls are "specifically required" for XOR8 (winner ratio 0.79).

D87 built its "regime transition" (nulls indispensable at ζ₈,
dispensable at ζ₁₂) on that baseline.

**None of these claims survives exact arithmetic, and D84's own code
does not reproduce them.** Rebuilt from its only commit (384e44e3) and
run on this machine, it prints:
- non-null max XOR8, equal to the full catalog;
- null-only XOR6 = 0;
- 0 = 0 at k_sec = 1.

Where the atlas figures came from is **unknown**. The review rebuilt
D84 on every variant it could (arm64 and x86 under Rosetta, −O0 / −O2,
three fp-contract modes, an emulated glibc `rand()`), and every build
prints the same as here.

The atlas's own Part D figures (XOR6 0.344 / 0.31, XOR8 1.188 / 0.79)
equal this machine's output exactly. So the original run made the same
full-catalog float decisions bit for bit, and only its Part C numbers
differ. A different code version, or a transcription error, is more
likely than the platform. Whatever their origin, the claims are false
exactly.

Run: `./bin/aedilis knotapel/demo_124_exact_null_states/main.c &&
bash build/aedilis/main/struere.sh && ./build/aedilis/main/main`.
It takes about 55 s and runs 14 checks.

## What D84 recorded, what its code prints, and what is exact

| | D84 atlas | D84's code here | exact rule | robust |
|---|---|---|---|---|
| full catalog (24) | XOR8 | XOR8 | XOR8 | XOR8 |
| non-null (15) | **XOR6** | XOR8 | **XOR8** | **XOR8** |
| null-only (9) | **XOR6** | 0 | **0** | **0** |
| random 15-subsets, mean max | 7.8 | 7.7 | **8.000** (all 10,000 sampled at XOR8) | 7.984 |
| non-null vs random | "worse" | equal or better | **equal** | equal |
| null-only XOR6, k = 1, 2, 4, 6, 8 | flat, beats non-null at k = 1 | 0 everywhere | 0 everywhere | 0 everywhere |
| nulls per XOR8 winner / random | 0.79 | (first 32 winners) | **0.50** (all 402 winners) | |

Definitions:
- **Exact rule:** D84's own semantics (`combined_cell`) in exact
  arithmetic. The sector is the floor of 2h·k/360, and the axis is the
  first maximal |v·u|.
- **Robust:** passes under every resolution of every tie.
- **Possible:** passes under some resolution, chosen per sum.

## One float decision in eight is a tie

> **Correction (2026-10-13, review of `knotapel/claims.md`).** The
> heading and the sentence "one decision in eight is a tie" below
> misread the table: 2,686,124 (12.8%) are the ties that the floats
> resolve *differently from the exact rule*. Ties overall are more:
> the three sources listed below (3,977,481 null-sum, 1,395,665 axis,
> 4,285,855 non-null sector ties) make at least a fifth of all
> decisions. See KC 22.5.

**Part C** replays every cell decision D84's float search makes, all
20,965,173 of them, against the exact cell of the same sum:

| | decisions |
|---|---|
| float cell = exact rule cell | 18,279,049 |
| float ≠ rule, but inside the exact tie set | **2,686,124** (12.8%) |
| float outside the tie set (a real float error) | **0** |

So the float code never miscomputes. But one decision in eight is a
tie, and the side it lands on is an accident of rounding. It is
IEEE-deterministic, so every build tried gives the same result, but
it is not the exact rule.

The largest source is the **null sums**: 3,977,481 decisions at even k.
- A sum with Re = 0 has half-angle exactly 90°, so it sits on the
  boundary between sectors k/2 − 1 and k/2 for every even k.
- In float its real part is closure noise of order 10⁻¹⁷. Its size
  decides the sector, deterministically: below about 1.1 × 10⁻¹⁶
  relative, acos rounds to exactly π/2 and the sum lands in k/2.
- Float put 578,730 of these decisions (14.6%) in sector k/2 − 1.
- **Every one of them had a nonzero float real part.** Whenever the
  float Re was exactly 0, the sum landed in k/2, where the exact rule
  puts it.

**Axis ties** (equal |v·u| to two axes of the cuboctahedron) add
1,395,665 decisions, and **sector-boundary ties** of non-null sums
4,285,855.

These ties change counts substantially (Part F), but they do **not**
explain the atlas. The first version of this file said they did, and
the review showed otherwise (M1, above).

**None of the four fixed tie rules gives the atlas's numbers.** Rules
(sector lower/upper × axis first/last, as in D122) all give (full,
non-null, null-only) = (8, 8, 0), the same as the exact rule. That is
the statement, and no more. D84's search is greedy (first 32 winners),
so per-sum adversarial resolutions are not bounded by these four. 84
"possible" triples precede the first triple of a robust non-null XOR8
set, enough to fill all 32 slots (review L1).

## Capacity exactly (Part D)

The search covers every weight set of every size, level by level.
Separability is downward closed, so the level-wise search is
exhaustive. D84's greedy search extends only its first 32 winners
(MAX_WIN), so its counts of 32 are a cap.

Sets passing at some ladder k, by weight count n = 3 … 7:

| | rule | robust | possible |
|---|---|---|---|
| full (24, 13 axes) | 1132 / 402 / 0 / 0 / 0 | 992 / 218 / 0 / 0 / 0 | 1785 / 2589 / 236 / 25 / 3 |
| non-null (15, its 7 axes) | 304 / 106 / 0 / 0 / 0 | 280 / 36 / 0 / 0 / 0 | 424 / 353 / 38 / 1 / 0 |
| null-only (9, its 9 axes) | 0 | 0 | 80 / 102 / 72 / 25 / 3 |

- **Removing the nulls keeps the maximum (XOR8) and removes winners.**
  XOR8 sets drop from 402 to 106 under the rule and from 218 to 36
  robustly. It's a loss of count, not of capacity. D87's "removing nulls
  destroys XOR8 capacity" read exactly this drop: its non-null count of
  XOR8 = 4 is greater than 0, and its full-catalog 32 is MAX_WIN.
- **Null-only computes nothing under the rule or robustly.** It reaches
  XOR14 only under "possible", where every null sum is a two-way tie and
  each one may take whichever side its parity needs. That is close to
  D122's vacuity at 90°, not a capability.
- **D84's greedy search with exact rule cells** gives the same maxima
  (8, 8, 0). The search heuristic is not what differs; the ties are.
- **The activation-table confound.** D84 rebuilds the Voronoi table
  from the subset, so dropping the nulls also drops 6 activation axes.
  With the full 13-axis table:
  - non-null stays at XOR8 (rule: 328 / 184 sets);
  - null-only reaches XOR6 with 4 sets (rule and robust).

  The table matters for counts, and for null-only's last 4 sets, but it
  changes no conclusion above.

## Random controls (Part E)

D84's own 100 + 100 subsets come from the same `rand()` sequence, and
10,000 + 10,000 more from a fixed splitmix64. Both keep D84's load
order (permutation order), which fixes the axis table and so the
first-maximum rule.

| | exact rule | robust |
|---|---|---|
| random-15, D84's 100 | all XOR8 (mean 8.00) | 98 XOR8, 2 XOR6 (7.96) |
| random-15, 10,000 | **all XOR8** (8.000) | 9,922 XOR8, 78 XOR6 (7.984) |
| random-9, D84's 100 | 36 XOR8, 64 XOR6 (6.72) | 23 / 77 (6.46) |
| random-9, 10,000 | 3,992 / 6,008 (6.798) | 2,387 / 7,613 (6.477) |

All 10,000 sampled 15-subsets reach exactly XOR8 under the rule, and
none goes higher. That is a sample of 1,307,504, not all of them. The
review counts 20,115 distinct ordered axis tables, so an exhaustive
check in C is feasible but has not been run. The non-null subset
equals the sampled distribution.

D84's float search finds a mean of 7.7 here: its tie resolutions lose
XOR8 in some subsets. The atlas records 7.8.

## The k_sec sweep and null prevalence (Part F)

XOR6 sets per k_sec (all triples of the subset):

| | k = 1 | 2 | 4 | 6 | 8 |
|---|---|---|---|---|---|
| full, float (this machine) | 36 | 137 | 185 | 370 | 459 |
| full, exact rule | 36 | 168 | 180 | 324 | 484 |
| full, robust | 36 | 36 | 36 | 68 | 176 |
| non-null, float / rule / robust | 0/0/0 | 12/12/0 | 22/14/0 | 46/70/12 | 66/46/44 |
| null-only, float / rule / robust | 0 | 0 | 0 | 0 | 0 |
| null-only, possible | 0 | 80 | 80 | 80 | 80 |

- The counts are heavily tie-dependent. At k = 8 the float run finds
  459, the exact rule 484, and only 176 hold under every resolution.
- Null-only is "flat" only under "possible", where every null triple
  that separates at all does so by choosing sides at its ties. Under
  the rule it is 0 at every k.
- **The k = 1 "synergy" (null-only 0 + non-null 0, full 36) is
  confounded by the activation table (review L4).** The full catalog's
  36 XOR6 sets at k = 1 have 0, 1, 2 and 3 nulls in 8, 12, 12 and 4
  cases. The 8 pure non-null and 4 pure null triples pass only because
  the full catalog's 13-axis table is finer than the subsets' own; with
  that table, non-null alone gives 8 and null-only 4. Like-for-like,
  the synergy is the **24 mixed sets**, not "0 + 0 = 36". The demo
  checks this decomposition.

**Null prevalence, among all winners:**
- XOR6: 0.933 nulls per set, against 1.125 for random sets of 3 (ratio
  0.83).
- XOR8: 0.746 against 1.500 (ratio **0.50**).

Winners *under*-use nulls. D84's 0.79 came from its first 32 XOR8
winners. "Specifically required" read a ratio below 1 the wrong way.

## Checks

- **Part A:** D84's float search is replicated bit for bit. The full,
  non-null and null-only maxima and counts, D84's 100 + 100 random
  subsets (printed means, min and max), and the k_sec sweep all equal
  D84's output on this machine.
- **Part B:** the exact ζ₈ closure in Z[√2] (components (p + q√2)/2)
  equals D84's float catalog entry by entry: BFS order, depths,
  writhes. The census holds exactly:
  - 9 null;
  - |Re| ∈ {0, 1/2, √2/2, 1} for 9 / 8 / 6 / 1 entries, gap 1/2;
  - nulls by depth 0/5, 2/10, 6/8, 1/1;
  - 13 axes = 6 null-only + 4 non-null-only + 3 shared.

  D84's Parts A, B and E (census, gap theorem, axis geometry) are
  exact facts and stand.
- **Part C:** every float decision is the exact rule cell or inside its
  exact tie set.
- **Parts D–G:** the claims above.
- **Exact sector:** the boundaries tan²(mπ/k) are roots of
  S_k(u) = Σ C(k, 2l+1)(−1)^l u^l, identified by position (D123). They
  are compared with `radices` against tan²h = |v|²/a² ∈ Q(√2). k = 10,
  16 and 20 need nested radicals, which radices handles; 6,697 compares
  in all.
- **Oracle:** `python3 -I oracle.py` (about 2.5 min) is independent:
  - Fractions over Q(√2) and its own BFS;
  - exact comparison where tan²(mπ/k) ∈ Q(√2) (m/k ∈ {1/8, 1/6, 1/4,
    1/3, 3/8}), 60-digit Decimal elsewhere, where no Q(√2) value can
    equal the boundary.

  It agrees with the demo on the rule and robust counts at every level
  (full, non-null, null-only, and with the 13-axis table), the k_sec
  sweep, the null members of all winners (1056 and 300) and the four
  fixed rules. Its numbers are embedded as constants.

## Plants (11)

Each plant is applied to main.c, built, run and restored, with a cmp
against the backup each time.

**Red (10):**

| plant | caught by |
|---|---|
| null sum in the lower sector under the rule | oracle check |
| sector tie never flagged | Part C (float outside the tie set) |
| sector tie counted below the boundary | Part C |
| axis rule = last maximum | oracle check |
| axis ties dropped | Part C |
| parity only for +q | Part D maxima (5 checks) |
| real sum in sector 1 | Part C |
| antipodal axes not merged | fatal: float and exact axis counts differ |
| level-wise search skips a candidate | oracle check |
| replica sector rounding (+1e-9) | Part A replica |

**Equivalent (1):** the apriori pruning disabled, so every candidate
is tested. Output is identical. That is expected, and it shows
downward closure in practice: no candidate with a failing subset ever
separates.

## Review (2026-10-10, recensor-extensio)

The exact numbers are right and the overturn holds. The reviewer's
independent recount (int-pair Z[√2], own BFS, own SAT) equals the demo
on every rule, robust and possible count it finished, including the
13-axis tables.

- **M1 (applied): the explanation of the atlas figures was
  unsupported.** The first version said float tie noise on another
  libm, compiler or machine produced them. Against that:
  - every rebuild prints the same: arm64 and x86/Rosetta, −O0/−O2,
    three fp-contract modes, emulated glibc `rand()`;
  - cos/sin(π/4) are correctly rounded on both libms, so the catalog is
    bit-identical;
  - a null sum's side is IEEE-deterministic;
  - the atlas's own Part D ratios match this machine exactly.

  Now: "not reproduced on any build tried; cause unknown".
- **L1 (applied):** "no consistent tie rule" became "none of the four
  fixed rules". D84's greedy search is not bounded by robust XOR8.
- **L2 (applied):** "every 15-subset" became "all 10,000 sampled". An
  exhaustive pass over the 1,307,504 subsets is feasible but not run.
- **L3 (applied):** the extended sample no longer sorts its subsets; it
  keeps D84's permutation order. random-9 under the rule went from
  3,966 to 3,992 XOR8; random-15 was unchanged.
- **L4 (applied, and checked in code):** the k = 1 synergy is
  confounded by the activation table; like-for-like it is 24 mixed sets.
- **P1 (noted):** same-point clashes exist. Among nulls q₁₅ + q₂₂ = q₁₇
  exactly, giving 32 full 3-sets, 4 null 3-sets and 24 null 4-sets
  where two masks of opposite parity have the same sum. Per-point
  semantics excludes them under every rule. A per-mask float evaluation
  could in principle split them at a tie; on this machine none passes.
- **P2 (noted):** D84's search stops at XOR12, so "possible XOR14" is
  outside anything D84 could have reported. It is an exact statement
  only.

## Not covered

- **D84's Part F (ζ₁₂ and ζ₄ census) and D87's ζ₁₂ claim.** D117
  already corrected D84's ζ₁₂ axis cap (512 → 2,043 axes). D87's
  "nulls dispensable at ζ₁₂" compares capped MAX_WIN counts (32 = 32)
  and belongs to the D82 audit (ζ₁₂, exact Q(√3)).
- **D84's LCFT / reservoir-computing interpretation.** It rested on
  "removing nulls destroys capacity", which does not hold.
- **D88's perturbation study** (continuous by design).
