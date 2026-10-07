# Demo 115: Honest Construction-Word Spectra — Findings

**Date:** 2026-10-07
**Status:** COMPLETE. 62 pass, 0 fail.
**Depends on:** Demo 114 (which voided D112's numbers and verified D112's raw braid polygons); Demo 112 (its raw polygons, as data); house libraries `laqueus` (incl. `laqueus_simplificare`, `laqueus_diagramma_minimum`, `diagramma_determinans`), `situs` (`situs_reflexio`), `polynomium`.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).

## What this demo does

D112 asked: take a stick knot, mirror any subset of its non-base vertices through the plane of vertices 0, 1, 2, and look at the 2^(n−3) "alternatives" — which knots come out? Demo 114 showed D112's answers were built on singular polygons and overflowing arithmetic. Here the same questions are asked on **honest polygons**:

- **Inputs:** D112's 12 raw braid polygons (D114: all simple, table Alexander polynomials), embedded verbatim.
- **Simplification by legal moves only** (`laqueus_simplificare`): an isotopy, so each polygon stays its knot — checked: same Alexander and Jones as the raw polygon. Each raw polygon is simplified from every start vertex and both orientations; the **main polygon is the variant with the fewest vertices** (ties: first by orientation, then start vertex) — a stated rule, chosen before looking at spectra, that also keeps the largest spectrum at 4,096 alternatives.
- **Every alternative computed exactly:** simplicity, Alexander, Jones. Classes are named against the 12 knots by Alexander + Jones: `K` = D112's braid chirality, `K*` = its mirror, `unknot` = certified (Alexander 1 on a diagram of ≤ 10 crossings; no nontrivial knot of crossing number ≤ 10 has Δ = 1 — Rolfsen / KnotInfo, first are 11n34 and 11n42), `unknot?` = Δ = V = 1 but no ≤ 10-crossing diagram found, `?` = anything else.

## Verdicts against Demo 112

| D112 claim | Honest answer |
|---|---|
| Self-intersection rates of 50–100% for most knots (3₁ 69%, 5₁ 87%, 6₁ 100%, 7₁ 97%, 7₄ 100%, 8₁₈ 100%) | **Artifact.** On honest polygons **every alternative of every knot is simple** (0%). D112's rates came from its degenerate, mostly singular simplified polygons. |
| 6₃ is a "knot factory": 37 polynomials, 10-crossing and non-alternating knots | **False for 6₃.** 6 distinct Alexander polynomials (unknot, 3₁, 4₁, 5₁, 5₂, 6₃) — the same six D114 found on D112's own 6₃ polygon. |
| "Crossing-number explosion": alternatives far more complex than the source | **Real — for 7₂, and once for 6₁.** The honest 15-vertex 7₂ polygon yields 47 distinct Alexander polynomials, 67 classes above its source's determinant (492 of 4,096 alternatives), determinants up to 85 and Alexander degree up to 8. 6₁ reaches 7₄ (det 15 > 9). Every other knot stays at or below its source's determinant. |
| Reachability: 6₃ out-degree 10 (mega-hub), 7₂ 9; seven knots are "islands" | **Redrawn.** 7₂ reaches 11 of the 12 knots; 6₁, 6₃ and 7₃ reach 5; no knot is an island except 3₁ and 4₁, which reach only themselves (plus mirror and unknot). |
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
| 6₁ | 11 | 256 | 11 | 6 / 1 / 0 | 4 / 4 | 152 / 156 | **15 (9)** |
| 6₂ | 10 | 128 | 6 | 4 / 4 / 3 | 4 / 4 | 80 / 80 | 11 (11) |
| 6₃ | 11 | 256 | 10 | 6 / 37 / 6 | 8 (amphichiral) | 142 / 144 | 13 (13) |
| 7₁ | 14 | 2048 | 7 | 4 / 2 / 1 | 16 / 16 | 1120 / 1120 | 7 (7) |
| 7₂ | 15 | 4096 | 101 | 47 / 68 / 7 | 6 / 8 (see note) | 77 / 1844 | **85 (11)** |
| 7₃ | 13 | 1024 | 11 | 6 / 1 / 0 | 2 / 2 | 670 / 676 | 13 (13) |
| 7₄ | 13 | 1024 | 7 | 4 / 1 / 0 | 32 / 32 | 576 / 576 | 15 (15) |
| 8₁₈ | 12 | 512 | 10 | 6 / 1 / 0 | 8 (amphichiral) | 288 / 336 | 45 (45) |

Typical honest spectra are small and orderly — the knot, its mirror, simpler knots, and mostly unknots. Examples:

- 3₁ (6 vertices): {3₁ ×1, 3₁* ×1, unknot ×6} — the only alternatives that stay knotted are the polygon and its mirror.
- 4₁: {4₁ ×4, unknot ×28}.
- 7₄: {7₄ ×32, 7₄* ×32, 5₂ ×64, 5₂* ×64, 3₁ ×128, 3₁* ×128, unknot ×576} — exactly mirror-symmetric, as the theorem requires.

Uncertified trivial-Alexander classes (`unknot?`, all with Jones = 1): 6₁ 4, 6₃ 2, 7₂ 1,106 (+661 with no Jones), 7₃ 6, 8₁₈ 48 — consistent with the unknot but not proven here (no ≤ 10-crossing diagram among directions with components in −2..2).

**7₂ note.** 1,700 of 7₂'s 4,096 alternatives have no projection with ≤ 20 crossings among the searched directions, so their Jones polynomial (and chirality) was not computed; they are classed by Alexander alone (Jones "-"). This is also why "itself 6 / mirror 8" is not exactly symmetric: Jones may be computable for an alternative but not for its mirror partner (the searched direction set is not invariant under the reflection). The mirror theorem itself is asserted only where both Jones polynomials exist, and holds there; simplicity and Alexander pair up for all 4,096.

**The one unnamed class family below 7₂:** in 8₁₈, two mirror classes (4 alternatives each) with Alexander (t² − t + 1)², det 9. That polynomial is shared by the granny knot, the square knot and 8₂₀; this demo has no knot table to say which (a knot-identification library would).

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

("x" = the knot or its mirror, by Alexander + Jones.) Every knot reaches itself — on honest polygons the base alternative is the knot, which D112 could only assume. Determinants above the source's occur only for 7₂ (67 classes, 492 alternatives, up to 85) and 6₁ (7₄ and 7₄*, 4 alternatives).

### Result 4: stability (Part D)

Spectra of **all** fewest-vertex polygons of the same knot (equally honest, same size):

- **6₃** (15 polygons, 11 vertices): core in all 15: {6₃, 3₁, 3₁*, unknot}; 4₁ in 14; 5₂/5₂* in 12; uncertified `unknot?` in 10; 5₁/5₁* in 7; 6₂/6₂* in 5; 6₁/6₁*, 7₁/7₁* and two unnamed classes in 1 each — 18 classes in the union. Class counts per polygon range 5–14.
- **4₁** (22 polygons, 8 vertices): core {4₁, unknot}; 3₁/3₁* in 19; 5₂/5₂* in 1.

The spectrum is a property of the polygon, but every polygon of a knot shares a small core: the knot itself, the unknot, and (for 6₃) the trefoils.

## Lessons

1. **Self-intersection was a symptom, not a phenomenon.** D112's high SI rates — and its "islands" — came from singular simplified polygons. Honest polygons have none.
2. **Some of D112's intuitions survive in honest form.** Complexity growth is real for 7₂ (and slightly for 6₁), and spectra do depend on the polygon; but the specific numbers, the 6₃ "factory", and the islands do not.
3. **The complement symmetry is geometry, not statistics.** Mirroring every vertex reflects the whole polygon; any "complement closure" count is chirality bookkeeping.
4. **Certification needs diagrams with few crossings.** The unknot certificate (Δ = 1 on ≤ 10 crossings) works for most alternatives; where the direction search finds only larger diagrams (7₂, 8₁₈), Δ = V = 1 is reported as "unknot?" — consistent, not proven.

## Method notes

- Jones: first generic projection if ≤ 16 crossings; otherwise the fewest-crossing projection with direction components in −1..1, then −2..2 (`laqueus_diagramma_minimum`), if ≤ 20 crossings; otherwise not computed (reported).
- Determinant: `diagramma_determinans` (|Δ(−1)|).
- Reflection: `situs_reflexio` through the plane of vertices 0, 1, 2.
- Runtime ~16½ minutes (7₂'s 4,096 alternatives dominate: the direction searches for > 16-crossing projections).

## Open threads

1. **Knot identification:** 80 of 7₂'s classes and two 8₁₈ classes are unnamed because the demo only knows the 12 base knots. A house knot table (Alexander + Jones + chirality, prime knots to 8–10 crossings, plus small composites) would name them — the "knot identification" library we deferred.
2. **7₂'s missing Jones:** a wider direction search (radius 3) or a faster Jones for 20–24 crossings would complete chirality for its 1,700 alternatives.
3. **Why 7₂?** Its honest polygon is the largest (15 vertices) and the only one producing knots well beyond its source. Is it the vertex count, the specific geometry, or the knot? The stability method (all fewest-vertex polygons) answers that question for 7₂ too — expensive at 4,096 alternatives per polygon.
