# Demo 115: Honest Construction-Word Spectra — Findings

**Date:** 2026-10-07
**Status:** COMPLETE. 75 pass, 0 fail (revised after review I the same day; see Revisions).
**Depends on:** Demo 114 (which voided D112's numbers and verified D112's raw braid polygons); Demo 112 (its raw polygons, as data); house libraries `laqueus` (incl. `laqueus_simplificare`, `laqueus_diagramma_minimum`, `diagramma_determinans`), `situs` (`situs_reflexio`), `polynomium`.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).

## What this demo does

D112 asked: take a stick knot, mirror any subset of its non-base vertices through the plane of vertices 0, 1, 2, and look at the 2^(n−3) "alternatives" — which knots come out? Demo 114 showed D112's answers were built on singular polygons and overflowing arithmetic. Here the same questions are asked on **honest polygons**:

- **Inputs:** D112's 12 raw braid polygons (D114: all simple, table Alexander polynomials), embedded verbatim.
- **Simplification by legal moves only** (`laqueus_simplificare`): an isotopy, so each polygon stays its knot — checked: same Alexander and Jones as the raw polygon. Each raw polygon is simplified from every start vertex and both orientations; the **main polygon is the variant with the fewest vertices** (ties: first by orientation, then start vertex) — a stated rule, chosen before looking at spectra, that also keeps the largest spectrum at 4,096 alternatives.
- **Every alternative computed exactly:** simplicity, Alexander, Jones. Missing Jones polynomials and unknot certificates are then filled from each alternative's mirror partner (exact, by the mirror theorem below — checked first on independently computed pairs). Classes are named by Alexander + Jones — a name means both match, not a proof of type (5₁ and 10₁₃₂ share both): `K` = D112's braid chirality, `K*` = its mirror, `unknot` = certified (Alexander 1 on a diagram of ≤ 10 crossings; no nontrivial knot of crossing number ≤ 10 has Δ = 1 — Rolfsen / KnotInfo, first are 11n34 and 11n42), `unknot?` = Δ = V = 1 but no ≤ 10-crossing diagram found, `?` = anything else. Beyond the 12: the trefoil composites `3_1#3_1`, `3_1*#3_1*`, `3_1#3_1*` (Jones from the demo's own trefoil Jones, multiplicative under connected sum) and `8_20`/`8_20*` (Jones from the Knot Atlas page 8_20, fetched 2026-10-07; the Atlas 3₁ has D112's trefoil chirality) — all with Alexander (t² − t + 1)².

## Verdicts against Demo 112

| D112 claim | Honest answer |
|---|---|
| Self-intersection rates of 50–100% for most knots (3₁ 69%, 5₁ 87%, 6₁ 100%, 7₁ 97%, 7₄ 100%, 8₁₈ 100%) | **Artifact.** On honest polygons **every alternative of every knot is simple** (0%). D112's rates came from its degenerate, mostly singular simplified polygons. |
| 6₃ is a "knot factory": 37 polynomials, 10-crossing and non-alternating knots | **False for 6₃ on the chosen polygon:** 6 distinct Alexander polynomials (unknot, 3₁, 4₁, 5₁, 5₂, 6₃) — the same six D114 found on D112's own 6₃ polygon. Across all 15 equally honest 6₃ polygons the union is still small (Part D), though one of them (+23) yields a mirror pair of determinant 31. |
| "Crossing-number explosion": alternatives far more complex than the source | **Real for 7₂, and polygon-dependent.** The honest 15-vertex 7₂ polygon yields 47 distinct Alexander polynomials; 33 of them (69 classes, 492 of 4,096 alternatives) lie above its source's determinant, up to 85, Alexander degree up to 8. On the chosen polygons only 6₁ also exceeds its source (7₄, det 15 > 9); but in Part D one of the 15 6₃ polygons (+23, det 31) and one of the 22 4₁ polygons (r17, 5₂, det 7 > 5) exceed theirs too. |
| Reachability: 6₃ out-degree 10 (mega-hub), 7₂ 9; seven knots are "islands" | **Redrawn.** 7₂ reaches 11 of the 12 knots, plus the square knot and 8₂₀/8₂₀*; 6₁, 6₃ and 7₃ reach 5; on the chosen polygons only 3₁ and 4₁ reach nothing but themselves (plus mirror and unknot). |
| Stability: "universal core {6₃}", union 118 over 20 trials | **Core {6₃, 3₁, 3₁*, unknot}** over all 15 fewest-vertex 6₃ polygons; union 18 classes. 4₁: core {4₁, unknot} over 22 polygons. The spectrum depends on the polygon, around a small core. |
| D110 "complement chirality theorem" / D111 complement closure statistics | **A theorem, now explained and verified for all 12 knots:** vertices 0, 1, 2 lie on the mirror plane, so alternative c XOR (all ones) is the mirror image of alternative c — same simplicity, same Alexander, mirror Jones. Every spectrum is chirality-symmetric by construction. |

## Results

### Result 1: honest polygons (Part A)

| knot | raw vertices | default start | fewest | variants reaching it | chosen |
|---|---|---|---|---|---|
| 3₁ | 18 | 7 | **6** | 5 | reversed, start 2 |
| 4₁ | 29 | 9 | 8 | 22 | forward 3 |
| 5₁ | 26 | 11 | 10 | 8 | reversed 6 |
| 5₂ | 37 | 12 | 11 | 17 | forward 4 |
| 6₁ | 48 | 14 | 11 | 3 | forward 26 |
| 6₂ | 37 | 10 | 10 | 6 | forward 0 |
| 6₃ | 37 | 12 | 11 | 15 | forward 2 |
| 7₁ | 34 | 15 | 14 | 10 | reversed 10 |
| 7₂ | 56 | 17 | 15 | 1 | forward 14 |
| 7₃ | 45 | 14 | 13 | 9 | reversed 0 |
| 7₄ | 56 | 18 | 13 | 1 | forward 27 |
| 8₁₈ | 49 | 12 | 12 | 5 | forward 0 |

Every chosen polygon is simple with the raw polygon's Alexander **and** Jones (so also the same chirality). The 6-vertex trefoil meets the trefoil's stick number (6). The reference Jones polynomials (D112's braid chirality) are those of the raw polygons.

### Result 2: the spectra (Part B)

| knot | vertices | alternatives | classes | distinct Alexander (honest / D112 / D114 on D112's polygon) | itself / mirror | unknots certified | max det (source) |
|---|---|---|---|---|---|---|---|
| 3₁ | 6 | 8 | 3 | 2 / 2 / 1 | 1 / 1 | 6 / 6 | 3 (3) |
| 4₁ | 8 | 32 | 2 | 2 / 4 / 3 | 4 (amphichiral) | 28 / 28 | 5 (5) |
| 5₁ | 10 | 128 | 5 | 3 / 2 / 1 | 4 / 4 | 80 / 80 | 5 (5) |
| 5₂ | 11 | 256 | 6 | 4 / 4 / 3 | 8 / 8 | 176 / 176 | 7 (7) |
| 6₁ | 11 | 256 | 10 | 6 / 1 / 0 | 4 / 4 | 156 / 156 | **15 (9)** |
| 6₂ | 10 | 128 | 6 | 4 / 4 / 3 | 4 / 4 | 80 / 80 | 11 (11) |
| 6₃ | 11 | 256 | 9 | 6 / 37 / 6 | 8 (amphichiral) | 144 / 144 | 13 (13) |
| 7₁ | 14 | 2048 | 7 | 4 / 2 / 1 | 16 / 16 | 1120 / 1120 | 7 (7) |
| 7₂ | 15 | 4096 | 103 | 47 / 68 / 7 | 8 / 8 | 106 / 1844 | **85 (11)** |
| 7₃ | 13 | 1024 | 10 | 6 / 1 / 0 | 2 / 2 | 676 / 676 | 13 (13) |
| 7₄ | 13 | 1024 | 7 | 4 / 1 / 0 | 32 / 32 | 576 / 576 | 15 (15) |
| 8₁₈ | 12 | 512 | 10 | 6 / 1 / 0 | 8 (amphichiral) | 288 / 336 | 45 (45) |

Typical honest spectra are small and orderly — the knot, its mirror, simpler knots, and mostly unknots. Examples:

- 3₁ (6 vertices): {3₁ ×1, 3₁* ×1, unknot ×6} — the only alternatives that stay knotted are the polygon and its mirror.
- 4₁: {4₁ ×4, unknot ×28}.
- 7₄: {7₄ ×32, 7₄* ×32, 5₂ ×64, 5₂* ×64, 3₁ ×128, 3₁* ×128, unknot ×576} — exactly mirror-symmetric, as the theorem requires.

Uncertified trivial-Alexander alternatives (`unknot?`, all with Jones = 1): 7₂ 1,200 (+538 with no Jones), 8₁₈ 48 — consistent with the unknot but not proven here (no ≤ 10-crossing diagram among directions with components in −2..2 for either partner of the mirror pair). Mirror filling certified the remaining ones for 6₁, 6₃ and 7₃.

**7₂ note.** 1,700 of 7₂'s 4,096 alternatives had no projection with ≤ 20 crossings among the searched directions (the reviewer's oracle finds best projections of 19–30 crossings for the high-determinant ones). The searched direction set is not invariant under the reflection through the base plane, so Jones can be found for one partner of a mirror pair and not the other: filling from the partner recovered 268, leaving **1,432 without Jones** (classed by Alexander alone, Jones "-"). After filling, 7₂ itself and its mirror are 8 / 8. The mirror theorem is asserted on independently computed pairs only (where both Jones polynomials exist), and holds; simplicity and Alexander pair up for all 4,096.

**8₁₈'s (t² − t + 1)² pair is 8₂₀ / 8₂₀\*.** That Alexander polynomial is shared by the granny knot, the square knot and 8₂₀. The demo computes the composites' Jones from its own trefoil (V(3₁)², V(3₁)V(3₁\*)): neither matches; the class's Jones equals Knot Atlas's 8₂₀ exactly (−t + 2 − t⁻¹ + 2t⁻² − t⁻³ + t⁻⁴ − t⁻⁵), and its partner the mirror. (First version of these findings: "granny, square or 8₂₀ — no table to say which"; the review pointed out the demo already had what it needed for the composites.) 7₂ also produces 8₂₀, 8₂₀\* (16 each) and the square knot 3₁#3₁\* (12).

### Result 3: reachability and the explosion question (Part C)

| from \ to | 3₁ | 4₁ | 5₁ | 5₂ | 6₁ | 6₂ | 6₃ | 7₁ | 7₂ | 7₃ | 7₄ | 8₁₈ | out (D112) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 3₁ | x | | | | | | | | | | | | 1 (1) |
| 4₁ | | x | | | | | | | | | | | 1 (3) |
| 5₁ | x | | x | | | | | | | | | | 2 (1) |
| 5₂ | x | x | | x | | | | | | | | | 3 (3) |
| 6₁ | x | x | | x | x | | | | | | x | | 5 (1) |
| 6₂ | x | x | | | | x | | | | | | | 3 (3) |
| 6₃ | x | x | x | x | | | x | | | | | | 5 (10) |
| 7₁ | x | | x | | | | | x | | | | | 3 (1) |
| 7₂ | x | x | x | x | x | x | x | x | x | x | x | | 11 (9) |
| 7₃ | x | x | x | x | | | | | | x | | | 5 (1) |
| 7₄ | x | | | x | | | | | | | x | | 3 (1) |
| 8₁₈ | x | | | x | | | x | | | | | x | 4 (1) |

("x" = the knot or its mirror, by Alexander + Jones.) Every knot reaches itself — on honest polygons the base alternative is the knot, which D112 could only assume. On the chosen polygons, determinants above the source's occur only for 7₂ (69 classes, 33 distinct Alexander polynomials, 492 alternatives, up to 85) and 6₁ (7₄ and 7₄*, 4 alternatives). 7₂'s classes not named among the 12: 3 named beyond them (square knot, 8₂₀, 8₂₀*), 40 classes whose Jones could not be computed (some with table Alexander polynomials — named by Alexander alone they would be ambiguous), 39 genuinely unknown, and the `unknot?` class.

### Result 4: stability (Part D)

Spectra of **all** fewest-vertex polygons of the same knot (equally honest, same size):

- **6₃** (15 polygons, 11 vertices): core in all 15: {6₃, 3₁, 3₁*, unknot}; 4₁ in 14; 5₂/5₂* in 12; uncertified `unknot?` in 8 (after mirror filling); 5₁/5₁* in 7; 6₂/6₂* in 5; 6₁/6₁*, 7₁/7₁* and one unnamed mirror pair (det 31, Alexander 2t⁴ − 8t³ + 11t² − 8t + 2, polygon +23) in 1 each — 18 classes in the union.
- **4₁** (22 polygons, 8 vertices): core {4₁, unknot}; 3₁/3₁* in 19; 5₂/5₂* (det 7, above 4₁'s 5) in 1 (polygon r17).
- One polygon of each knot produces a class above its source's determinant: complexity growth is a property of the polygon, not of the knot.

The spectrum is a property of the polygon, but every polygon of a knot shares a small core: the knot itself, the unknot, and (for 6₃) the trefoils.

## Lessons

1. **Self-intersection was a symptom, not a phenomenon.** D112's high SI rates — and its "islands" — came from singular simplified polygons. Honest polygons have none.
2. **Some of D112's intuitions survive in honest form.** Complexity growth is real — strongly for 7₂'s polygon, occasionally for others (6₁, one 6₃ polygon, one 4₁ polygon) — and spectra do depend on the polygon; but the specific numbers, the 6₃ "factory", and the islands do not.
3. **The complement symmetry is geometry, not statistics.** Mirroring every vertex reflects the whole polygon; any "complement closure" count is chirality bookkeeping.
4. **Certification needs diagrams with few crossings.** The unknot certificate (Δ = 1 on ≤ 10 crossings) works for most alternatives, and the mirror partner's diagram counts too; where neither partner has one in the searched directions (7₂, 8₁₈), Δ = V = 1 is reported as "unknot?" — consistent, not proven.
5. **A theorem is also a tool.** The mirror pairing first served as a check (asserted on independently computed pairs), then as an exact way to fill gaps — in that order.

## Method notes

- Jones: first generic projection if ≤ 16 crossings; otherwise the fewest-crossing projection with direction components in −1..1, then −2..2 (`laqueus_diagramma_minimum`), if ≤ 20 crossings; otherwise not computed (reported).
- Determinant: `diagramma_determinans` (|Δ(−1)|).
- Reflection: `situs_reflexio` through the plane of vertices 0, 1, 2.
- Mirror filling: after the independent check, an alternative without Jones takes its mirror partner's Jones with t → 1/t; an uncertified trivial-Alexander alternative takes its partner's certificate (the mirrored diagram has the same crossings).
- Runtime 7–18 minutes depending on the machine (7₂'s 4,096 alternatives dominate: the direction searches for > 16-crossing projections).

## Open threads

1. **Knot identification:** 39 of 7₂'s classes are genuinely unknown (Jones computed, no match among the 12 knots, composites of trefoils, or 8₂₀), plus the det-31 pair from 6₃'s polygon +23. A house knot table (Alexander + Jones + chirality, prime knots to 8–10 crossings, plus small composites) would name them — the "knot identification" library we deferred. Note that some unknown classes may need more than 10 crossings.
2. **7₂'s missing Jones:** 1,432 alternatives (40 classes) still lack Jones; a wider direction search (radius 3) or a faster Jones for 20–30 crossings would complete chirality there.
3. **Why 7₂?** Its honest polygon is the largest (15 vertices) and the only one producing knots well beyond its source. Is it the vertex count, the specific geometry, or the knot? The stability method (all fewest-vertex polygons) answers that question for 7₂ too — expensive at 4,096 alternatives per polygon.

## Revisions (after review I, 2026-10-07)

The review re-derived every number with an independent exact oracle (own simplification, mirror, projection, Fox matrix and bracket): the variant table, all 9,768 simple alternatives, every per-Alexander population (7₂'s 47, det 85, degree 8), 6₁ → 7₄, and both stability studies variant by variant. It found reporting errors, now fixed:

- "Every other knot stays at or below its source's determinant" held only for the chosen polygons: 6₃'s polygon +23 yields a det-31 mirror pair; the re-run also flags 4₁'s polygon r17 (5₂, det 7 > 5), which neither of us had noted. Part D now reports determinants and above-source polygons.
- "80 unnamed 7₂ classes because the demo only knows 12 knots" conflated missing Jones with unknown knots: now 40 lack Jones, 39 are unknown, 3 are named beyond the 12 (square knot, 8₂₀, 8₂₀*). "67 classes above the source" is 33 distinct Alexander polynomials (69 classes after mirror filling, 492 alternatives).
- The 8₁₈ pair was left as "granny, square or 8₂₀" although the demo's own trefoil Jones rules out the composites; it is 8₂₀/8₂₀* (Knot Atlas value, cited).
- Added at the review's suggestion: mirror filling of Jones and certificates (exact); the "a name means Alexander + Jones match" caveat; a check on `situs_reflexio`'s return value; an overflow check for the Part D union.
