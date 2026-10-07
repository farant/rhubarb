# Demo 116: Named Spectra — Findings

**Date:** 2026-10-07
**Status:** COMPLETE. 90 pass, 0 fail. Runtime about 5.5 minutes (D115: 7–18).
**Depends on:** Demo 115 (the honest polygons, spectra and mirror theorem; its Alexander-level numbers are reproduced exactly here). House libraries: `laqueus` with the new `laqueus_pd_simplificare` (greedy Reidemeister I/II on PD codes, commit 81aa0eef), and `tabula_nodorum`, a table of the 250 prime knots up to 10 crossings. Names, symmetry and PD codes come from KnotInfo via database_knotinfo 2026.10.5; all polynomials are computed by laqueus. Commits e6700cad and df4c6348.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).

## What this demo does

Demo 115 left D112's construction-word spectra half named. Classes could only be matched against the 12 source knots, plus hand-checked trefoil composites and 8_20. In addition, 1,432 of 7_2's alternatives had no Jones polynomial, because their best projections had 21–30 crossings. D116 re-runs D115 with two new house tools:

- **Jones and unknot proofs.** Each projection becomes a PD code, which is reduced by Reidemeister I/II moves. This is exact (an isotopy).
  - A reduced diagram with **0 crossings proves the unknot**.
  - The reduced crossing count is an **upper bound** on the crossing number.
  - The direction search of D115 runs only if the reduced diagram is still above 20 crossings.
- **Names.** `tabula_nodorum_agnoscere` matches Alexander + Jones against every prime knot up to 10 crossings, as drawn or mirrored, and against sums of two table knots.
  - Names follow KnotInfo's chirality: `K` is the KnotInfo diagram, `K*` its mirror.
  - `a|b` means several table knots share both invariants; every such ambiguity is printed.
- **Beyond the table.** For every knot, span(Jones) ≤ crossing number (Kauffman, Murasugi, Thistlethwaite). So every class that the table cannot name gets the interval **[span(Jones), reduced crossings]** for its crossing number.

## Results

### Result 1: the chirality translation (Part A)

D115 named knots in D112's braid chirality. Against the KnotInfo table (Alexander + Jones of each raw braid polygon):

| D112 source | KnotInfo name | raw polygon: generic → reduced crossings |
|---|---|---|
| 3₁ | **3₁\*** | 8 → 3 |
| 4₁ | 4₁ (amphichiral) | 13 → 4 |
| 5₁ | **5₁\*** (also matches 10₁₃₂) | 10 → 6 |
| 5₂ | 5₂ | 17 → 10 |
| 6₁ | 6₁ | 19 → 7 |
| 6₂ | 6₂ | 17 → 10 |
| 6₃ | 6₃ (amphichiral) | 17 → 10 |
| 7₁ | **7₁\*** | 12 → 8 |
| 7₂ | 7₂ | 21 → 9 |
| 7₃ | **7₃\*** | 17 → 11 |
| 7₄ | **7₄\*** | 21 → 9 |
| 8₁₈ | 8₁₈ (amphichiral) | 19 → 17 |

The table names every source as itself, up to chirality; the demo asserts this. D112's braids for 3₁, 5₁, 7₁, 7₃ and 7₄ are KnotInfo's mirror images. Like the 8_20 question earlier, this is a convention, not an error. Every honest polygon has D115's vertex count and the raw polygon's invariants.

### Result 2: D115 reproduced, and its gaps closed (Part B)

D115's Alexander-level numbers are reproduced exactly for all 12 knots: all 9,768 alternatives simple, the distinct-Alexander counts (7₂: 47), the maximum determinants (7₂: 85), and the trivial-Alexander counts. These numbers do not depend on Jones or naming. The mirror theorem again holds on every independently computed pair.

| knot | alternatives | classes (D115) | unknots proven / trivial Alexander (D115) | Jones missing (D115) |
|---|---|---|---|---|
| 3₁ … 7₄ except 7₂ | as D115 | as D115, except 8₁₈ 9 (10) | all, as D115 | 0 (0) |
| 7₂ | 4,096 | 101 (103) | **1,706** / 1,844 (106) | **0** (1,432) |
| 8₁₈ | 512 | 9 (10) | **336** / 336 (288) | 0 (0) |

- **7₂ now has Jones for every alternative.** Reduced diagrams brought the 21–30-crossing projections below the cap; only 20 Jones polynomials were filled from mirror partners (D115: 268).
- **Unknots.** 1,600 more of 7₂'s trivial-Alexander alternatives are proven unknots, through the reduced diagrams: reduction to 0 crossings, or Δ = 1 on a reduced diagram of ≤ 10 crossings. The demo does not count the two routes separately. D115's unknot certificate needed an unreduced ≤ 10-crossing projection and rarely found one. 8₁₈'s 48 `unknot?` are all proven, so its class count drops from 10 to 9. 138 of 7₂'s alternatives have Δ = V = 1 but are not proven here: their smallest reduced diagram has 11 crossings, and greedy R1/R2 is stuck there. Their Jones polynomial was computed on a diagram of ≤ 20 crossings, and the Jones polynomial is known to detect the unknot for every knot up to 22 crossings (Tuzun & Sikora, *J. Knot Theory Ramifications* 27(3), 2018; extended to 24 crossings, arXiv:2003.06724). So they are unknots **by citation**, labelled `unknot (TS)` and kept apart from what this demo proves. This is the same footing as the "first Δ = 1 knots are 11n34 and 11n42" fact behind the unknot certificate. A proof inside the house would need R3 or a better minimizer.
- **7₂ itself.** The polygon's own knot and its mirror occur 32 + 32 times; D115 saw 8 + 8, the rest hidden among the missing Jones.

### Result 3: what the construction words reach (Part C)

**All 9,768 alternatives by status:**

| status | classes | alternatives |
|---|---|---|
| unknot (proven) | 12 | 5,084 |
| `unknot (TS)` (Δ = V = 1 on ≤ 20 crossings; by citation) | 1 | 138 |
| named by the table | 136 | 4,458 |
| no table match | 26 | 88 |

**Every source except 7₂ reaches only small knots.** Its own knot, its mirror, simpler knots and the unknot, as in D115. The one exception is 8₁₈'s 8₂₀/8₂₀\* pair (4 + 4), found in D115 by hand and now named by the table. The reachability matrix among the 12 sources is D115's.

**7₂'s honest 15-vertex polygon reaches 37 distinct prime knots of the table** (first candidate, up to mirror). The names beyond the 12 sources, with alternatives per class:
- **7 crossings:** 7₅/7₅\* (28 each), 7₆/7₆\* (12 each), 7₇/7₇\* (16 each).
- **8 crossings:**
  - 8₄/8₄\* (8 each), 8₆/8₆\*, 8₇/8₇\* (6 each);
  - 8₁₄/8₁₄\*, 8₂₁/8₂₁\*;
  - 8₂₀/8₂₀\* (34 each);
  - 8₁₆|10₁₅₆ (12, both mirrors), 8₈|10₁₂₉ (2, both mirrors).
- **9 crossings:**
  - 9₇, 9₂₇, 9₄₃;
  - 9₄₄, 9₄₅/9₄₅\* (16 each), 9₄₆, 9₄₈;
  - 9₄₂|9₄₂\* (8; Jones cannot tell 9₄₂ from its mirror).
- **10 crossings:**
  - 10₁₂₅|10₁₂₅\* (4), 10₁₃₁, 10₁₃₇;
  - 10₁₄₃ (8 each), 10₁₅₈, 10₁₅₉, 10₁₆₀.
- **Sums of two knots:** 3₁#3₁\* (square knot, 24), 3₁#4₁ and 3₁\*#4₁ (2 each).

Every 10-crossing name here (10₁₂₅–10₁₆₀) is non-alternating, as are 8₂₀, 8₂₁ and 9₄₂–9₄₈ (Rolfsen numbering: 8₁₉–8₂₁, 9₄₂–9₄₉, 10₁₂₄–10₁₆₅). The construction words reach well beyond the alternating knots.

**Consistency with theory.** For every table-named prime class, some candidate's crossing number is at most the reduced crossing count (asserted). The reduced diagram is a diagram of the knot, so a wrong name, or a reduction that cuts too deep, would typically break this. A plant understating the reduced count by 3 turns the check red. The other half, span(Jones) ≤ c(K), holds by construction for named classes, since their Jones *is* the table knot's. It is used only for the unnamed ones.

**What the table cannot name: 13 mirror pairs (26 classes, 88 alternatives), all from 7₂.**
- All have Jones span 8–10, so none is *proven* beyond 10 crossings by the span bound alone.
- All have reduced diagrams of 12–18 crossings (asserted: none ≤ 10).
- They are neither prime knots of ≤ 10 crossings nor sums of two table knots.
- So each is one of: a prime knot of ≥ 11 crossings; a sum with a factor of ≥ 11 crossings; or a sum of three or more knots. Those with Alexander degree 4 cannot be sums of three nontrivial table knots, since each factor adds ≥ 2 to the degree.
- Several share their Alexander polynomial with a table knot but not its Jones, e.g. det 39 with 10₁₅₉'s Alexander.
- **External identification (review I, not computed by this demo).** The reviewer matched all 13 pairs by Alexander + Jones against KnotInfo's own 11–13-crossing columns in the pinned CSV. All are non-alternating primes, and each lies inside its interval [span, reduced]:
  - 11 crossings: 11n89, 11n100, 11n102, 11n111, 11n121, and 11n11|11n112;
  - 12 crossings: 12n347, 12n351, 12n377, 12n468, 12n805, and 12n20|12n634;
  - mixed: 12n719|13n1575.

  I re-checked two against the CSV: 11n89 = our det-61 class (exact Jones), and 11n111 = our det-7 pair (one exact, one mirror). Extending the house table to 13 crossings would let the demo itself name them.

### Result 4: stability, named (Part D)

- **6₃ (15 polygons).** The core is again {6₃, 3₁, 3₁\*, unknot}. D115's unnamed det-31 mirror pair from polygon +23 is **8₁₄ / 8₁₄\***. The union is now fully named: 6₃, 3₁, 3₁\*, unknot, 4₁, 5₂, 5₂\*, 5₁|10₁₃₂\*, 5₁\*|10₁₃₂, 6₂, 6₂\*, 7₁, 7₁\*, 6₁, 6₁\*, 8₁₄, 8₁₄\*.
- **4₁ (22 polygons).** The core is {4₁, unknot}; polygon r17's 5₂/5₂\* as in D115.

## Lessons

1. **Table and simplification were the missing tools, not more search.** D115's open threads (39 "unknown" classes, 1,432 missing Jones, 1,200 + 538 `unknot?`) mostly came from having no table and from computing on unreduced projections. With both tools, 37 distinct table knots appear in 7₂'s spectrum, and 1,600 more alternatives are proven unknots.
2. **Chirality conventions differ per table and per construction.** Five of D112's braid knots are KnotInfo's mirrors. Naming against one fixed table, with a translation row, removes the ambiguity.
3. **Bounds turn "unknown" into a statement.** An unnamed class now carries an interval for its crossing number. The bracket span(V) ≤ c ≤ reduced also checks every named class.
4. **Greedy R1/R2 has a visible limit.** 138 alternatives are stuck at 11+ crossings with Δ = V = 1 (unknots only by citation), and a few unmatched classes reduce only to 15–18 crossings while their spans are 8–10. R3 or a better minimizer is the next lever.

## Method notes

- **Jones:**
  1. Take the generic projection, convert it to a PD code and reduce it greedily by R1/R2.
  2. If still > 20 crossings: take the fewest-crossing projection with direction components in −1..1, then −2..2, each reduced the same way.
  3. Compute Jones from the reduced PD (`laqueus_jones_ex_pd`) if ≤ 20 crossings.
  4. Raw source polygons use a cap of 22.
- **Unknot proven:** reduced to 0 crossings, or Δ = 1 on a reduced diagram of ≤ 10 crossings. No nontrivial knot with ≤ 10 crossings has Δ = 1; the first are 11n34 and 11n42.
- **Mirror filling** works as in D115: first checked on independent pairs, then used to fill Jones, the unknot proof and the reduced count from the partner.
- **Names:** `tabula_nodorum_agnoscere`, up to 8 candidates printed.
- **"Distinct table knots reached":** first candidate, prime only, up to mirror.
- **Regression against D115:** D115's numbers are hard-coded and asserted. Equality holds for the Alexander-level numbers. For proofs, Jones and self counts, ≥ or ≤ is asserted where D116's tools can only do better (7₂, unknot proofs).

## Open threads

1. **Extend the table to 11–13 crossings** (KnotInfo has them). The demo would then name the 13 unmatched pairs itself; the review has already identified them externally.
2. **R3 or a better minimizer**, to prove the 138 `unknot (TS)` alternatives in-house and to shrink the 15–18-crossing unmatched classes.
3. **Why 7₂?** It is still the only polygon that explodes. Its stability study (all fewest-vertex 7₂ polygons) is now affordable: about 5 minutes per 4,096 alternatives.

## Revisions (after review I, 2026-10-07)

The reviewer verified the demo against KnotInfo's own columns: the chirality translation, all 73 named 7₂ classes, the 37 distinct primes, the det-31 pair = 8₁₄, and the hard-coded D115 numbers. It also independently checked the simplifier, with about 4,000 random planar diagrams and 1,000 spliced composites. It found no bugs. Changes:

- **Library (94c5d5d7).** PD codes must now be planar: `_pd_legere` counts faces, F = c + 2. A valid but non-planar (virtual) code had been accepted, and the simplifier could "prove" such codes unknots; no projection in this demo is affected. Also added a mirrored non-face R2 test; the plant "R2 only for even slots" had survived the suite.
- **Bracket check:** reduced to its informative half, c(K) ≤ reduced crossings. The span half is a tautology for named classes. Re-planted: understating the reduced count turns it red.
- **Unmatched check:** the wording no longer claims more than is searched. Sums of three or more knots (e.g. 3₁#3₁#3₁) are not searched.
- **`unknot?` is now `unknot (TS)`:** by citation of the Jones unknot verification to 22 (24) crossings; the citation was checked.
- **The 13 unmatched pairs are identified externally** as 11–13-crossing non-alternating primes (reviewer; two re-checked here).

