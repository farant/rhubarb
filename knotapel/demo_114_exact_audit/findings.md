# Demo 114: Exact Audit of Demos 110–112 — Findings

**Date:** 2026-10-07
**Status:** COMPLETE. 51 pass, 0 fail (the checks encode what is verified; the audit verdicts are below).
**Depends on:** Demos 110, 111, 112 (their polygons, as data); rhubarb house libraries `laqueus` (exact knots), `situs`, `fractio`, `polynomium`, `matrix`.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).

## Why this demo

D110–D112 built their construction-word results on homemade arithmetic: `Rat` on `long` with no overflow checks, crossings counted only at strictly interior intersections, a heuristic for self-intersection, and "simplification" that deletes a vertex whenever one projection's determinant is unchanged. rhubarb now has exact, reviewed libraries for all of this, so before building anything new on D110–D112 we re-checked the **same polygons** exactly.

Inputs are D112's own artifacts. `d112_export.c` compiles Demo 112's `main.c` **unmodified** (its `main` renamed) and prints its polygons, its removal sequences, and its per-alternative verdicts; the demo embeds those lines verbatim. The export reproduces D112's published self-intersection census exactly (3₁ 5/11 … 8₁₈ 1/511), and replaying D112's simplification loop reproduces its 44 simplified polygons exactly.

## Verdicts at a glance

| Claim (demo, result) | Verdict |
|---|---|
| D112: the 12 simplified stick polygons are the 12 base knots | **FALLS** — 10 of 12 are singular (the polygon passes through itself); only 4₁ and 6₃ are knots |
| D111/D112: simplification is "topology-preserving" | **FALLS** — 117 of D112's vertex deletions are illegal moves (the swept triangle hits the polygon) |
| D112 R1–R10: 6₃ spectrum = 37 distinct palindromic polynomials (10-crossing knots, non-alternating knots, connected sums, det up to 161) | **FALLS** — exactly 6: unknot, 3₁, 4₁, 5₁, 5₂, 6₃; confirmed by a second, independent exact oracle |
| D112: 7₂ spectrum = 68 polynomials | **FALLS** — 7 |
| D112 Phase 4: reachability (6₃ out-degree 10, 7₂ 9) | **FALLS** — 6₃ 5, 7₂ 6; no knot with a singular base polygon reaches itself, and 3₁, 5₁, 6₁, 7₁, 7₃, 7₄, 8₁₈ reach nothing |
| D112 Phase 5: stability statistics (union 118, rare 73) | **FALLS** — 14 of 20 trial polygons are singular; per-trial spectra collapse (65 → 9, 65 → 6) |
| D112: self-intersection census (SI rates per knot) | **SURVIVES**, except one alternative per knot — the base polygon, which D112 never checked (it substituted the braid's Fox polynomial) |
| D112: "100% palindromic after SI filter" | true but empty — exact Alexander polynomials are palindromic by theorem; D112's wrong polynomials were palindromic too, so palindromy never certified anything |
| D111 Results 3–20 (det spectra, influence, complement closure, Hamming profiles, 8₁₈ 28 dets) | **FALL** with their polygons — computed on singular polygons with overflowing arithmetic and no SI filter (e.g. 6₃ 22 dets → 5; every 8₁₈ alternative is singular) |
| D110 Result 2: the 4-check majority classifier is perfect on the 512 alternatives | **SURVIVES** — all 512 are simple, 64 genuine figure-eights (no cinquefoils), 384 unknots (Alexander = Jones = 1), 32 + 32 trefoils |
| D110: "left-trefoil if b2 = 1" | **NAMES SWAPPED** — Jones says D110's "left" are right-handed and vice versa (consistently: D110's writhe sign convention is reversed); the chirality split itself is real |

## Results

### Result 1: D112's simplified polygons are mostly not knots (Part A)

Exact embedding test on D112's polygons (raw braid polygon / reachability simplification / spectrum simplification, vertices/crossings in a generic projection):

| knot | raw | reach | spec | raw Alexander | reach | spec |
|---|---|---|---|---|---|---|
| 3₁ | 18/8 | 7/— | 7/— | table | **singular** | **singular** |
| 4₁ | 29/13 | 8/4 | 8/4 | table | table | table |
| 5₁ | 26/10 | 10/— | 10/— | table | **singular** | **singular** |
| 5₂ | 37/17 | 11/— | 11/— | table | **singular** | **singular** |
| 6₁ | 48/19 | 12/— | 14/— | table | **singular** | **singular** |
| 6₂ | 37/17 | 10/— | 10/— | table | **singular** | **singular** |
| 6₃ | 37/17 | 11/20 | 11/20 | table | table (Jones = raw) | table |
| 7₁ | 34/12 | 13/— | 13/— | table | **singular** | **singular** |
| 7₂ | 56/21 | 14/— | 14/— | table | **singular** | **singular** |
| 7₃ | 45/17 | 11/— | 11/— | table | **singular** | **singular** |
| 7₄ | 56/21 | 14/— | 14/— | table | **singular** | **singular** |
| 8₁₈ | 49/19 | 12/— | 12/— | table | **singular** | **singular** |

Every raw braid polygon is a genuine knot with the table Alexander polynomial (D112's braid layout is fine). The damage is done by the simplification.

Hand check (D112's simplified trefoil, `(0,0,0) (20,0,60) (0,−1,10) (0,0,20) (20,0,40) (0,−1,50) (0,−105,0)`): segment 0 is (20a, 0, 60a) and segment 3 is (20b, 0, 20+20b); a = b = ½ gives the same point **(10, 0, 30)**, interior to both. The polygon passes through itself.

D112 never noticed because its base case (choices = 0) used the braid's Fox-calculus polynomial instead of the polygon, so the polygon itself was never examined.

### Result 2: the simplification is not an isotopy (Part B)

D112 deletes a vertex when the (2,3,5)-projection determinant is unchanged. Replaying each deletion as a triangle move (legal only if the swept triangle touches nothing else):

| knot | D112 deletions | illegal | first illegal | legal greedy simplification |
|---|---|---|---|---|
| 3₁ | 11 | 6 | #2 | 18 → 7 |
| 4₁ | 21 | 2 | #7 | 29 → 9 |
| 5₁ | 16 | 9 | #2 | 26 → 11 |
| 5₂ | 26 | 8 | #3 | 37 → 12 |
| 6₁ | 36 | 13 | #5 | 48 → 14 |
| 6₂ | 27 | 8 | #3 | 37 → 10 |
| 6₃ | 26 | 5 | #4 | 37 → 12 |
| 7₁ | 21 | 13 | #2 | 34 → 15 |
| 7₂ | 42 | 13 | #3 | 56 → 17 |
| 7₃ | 34 | 9 | #2 | 45 → 14 |
| 7₄ | 42 | 21 | #3 | 56 → 18 |
| 8₁₈ | 37 | 10 | #12 | 49 → 12 |

117 illegal deletions in total. 4₁ and 6₃ also contain illegal deletions (2 and 5) yet end on genuine knots of the right type — an illegal move is not always a type change, which is exactly why "det unchanged" looked like it worked.

A **legal** greedy simplification (same scan order, `laqueus_motus_removere`) always keeps the knot (checked: same Alexander polynomial) and lands near D112's vertex counts (often 1–4 more vertices). These are the honest polygons for a future re-run.

### Result 3: the 6₃ "knot factory" was integer overflow (Part C)

On D112's 6₃ polygon (one of the two that is a knot), all 256 alternatives are simple — D112 agrees. But the exact Alexander spectrum is:

| Alexander | knot | population |
|---|---|---|
| 1 | unknot | 144 |
| t² − t + 1 | 3₁ | 72 |
| 2t² − 3t + 2 | 5₂ | 16 |
| t⁴ − 3t³ + 5t² − 3t + 1 | 6₃ | 8 |
| t⁴ − t³ + t² − t + 1 | 5₁ | 8 |
| t² − 3t + 1 | 4₁ | 8 |

D112 reported 37 polynomials (unknot 76, trefoil 68, …, det 161). **Independent confirmation:** the exact Python oracle written for the laqueus review (its own projection, its own crossing tests, Alexander's original left/right rule — no shared code with laqueus) gives the same six polynomials with the same populations, 144/72/16/8/8/8.

**Mechanism, measured:** (2,3,5), the direction D112 used, is generic for all 256 alternatives (oracle check), so the projection itself was sound. In the alternative compared crossing by crossing (18), D112's 19 crossings match the oracle's except for one over/under flip: the crossing of edges 3 and 7. the crossing of Exact depths there are −51866387140/245776949 (edge 3) and 17651979460/245776949 (edge 7), so edge 7 is over; D112's `rat_cmp` forms −51866387140 × 245776949 = −1.27 × 10¹⁹, beyond `long` range (9.22 × 10¹⁸), and the wrapped value says edge 3. Running D112's code per alternative under the signed-overflow sanitizer (`d112_overflow_probe.c`, one process per alternative): **221 of 256 alternatives overflow; all 144 wrong polynomials are among them**; 77 overflowed and came out right by luck.

(Measurement note: a single sanitizer run over all 256 alternatives reports overflow only once per source line, which first suggested "1 alternative" — wrong; see the probe's header.)

Jones refines the exact spectrum to 13 Alexander|Jones pairs (Jones computed for the 200 alternatives with ≤ 16 crossings); the trefoils split into 24 right-handed and 28 left-handed (20 not computed).

### Result 4: all spectra and the reachability graph shrink (Part C)

| knot | alternatives | simple (D112 / exact) | distinct Alexander (D112 / exact) |
|---|---|---|---|
| 3₁ | 16 | 5 / 4 | 2 / 1 |
| 4₁ | 32 | 32 / 32 | 4 / 3 |
| 5₁ | 128 | 17 / 16 | 2 / 1 |
| 5₂ | 256 | 129 / 128 | 4 / 3 |
| 6₁ | 512 | 1 / 0 | 1 / 0 |
| 6₂ | 128 | 65 / 64 | 4 / 3 |
| 6₃ | 256 | 256 / 256 | 37 / 6 |
| 7₁ | 1024 | 33 / 32 | 2 / 1 |
| 7₂ | 2048 | 1025 / 1024 | 68 / 7 |
| 7₃ | 256 | 1 / 0 | 1 / 0 |
| 7₄ | 2048 | 1 / 0 | 1 / 0 |
| 8₁₈ | 512 | 1 / 0 | 1 / 0 |

The "simple" columns differ by exactly the base polygon wherever it is singular: D112's sign-0 self-intersection heuristic agrees with the exact test on **every** non-base alternative. (This was one of our suspicions going in; it did not hold — the heuristic was fine here.)

Exact reachability out-degrees (D112 in parentheses): 4₁ 2 (3), 5₂ 2 (3), 6₂ 2 (3), 6₃ 5 (10), 7₂ 6 (9), all others 0 (1). A knot whose base polygon is singular never reaches itself (5₂, 6₂ and 7₂ still reach 3₁ and 4₁ through simple alternatives).

### Result 5: the stability trials (Part D)

| trial | verts | alternatives | simple D112/exact | distinct D112/exact | illegal deletions | base polygon |
|---|---|---|---|---|---|---|
| 0 | 11 | 256 | 256/256 | 37/6 | 5 of 26 | 6₃ |
| 1 | 15 | 4096 | 1/0 | 1/0 | 1 of 22 | singular |
| 2 | 11 | 256 | 256/256 | 11/4 | 5 of 26 | 6₃ |
| 3 | 15 | 4096 | 3073/3072 | 6/6 | 5 of 22 | singular |
| 4 | 14 | 2048 | 1025/**0** | 3/0 | 3 of 23 | singular |
| 5 | 13 | 1024 | 1024/1024 | 9/6 | 0 of 24 | 6₃ |
| 6 | 15 | 4096 | 3073/3072 | 21/6 | 3 of 22 | singular |
| 7 | 12 | 512 | 385/384 | 10/5 | 5 of 25 | singular |
| 8 | 13 | 1024 | 673/672 | 9/4 | 6 of 24 | singular |
| 9 | 14 | 2048 | 2048/**0** | 6/0 | 3 of 23 | singular |
| 10 | 15 | 4096 | 1793/1792 | 5/4 | 4 of 22 | singular |
| 11 | 16 | 8192 | 4097/4096 | 65/9 | 1 of 21 | singular |
| 12 | 14 | 2048 | 1345/1344 | 5/4 | 10 of 23 | singular |
| 13 | 13 | 1024 | 1024/1024 | 16/6 | 2 of 24 | 6₃ |
| 14 | 11 | 256 | 256/256 | 27/6 | 0 of 26 | 6₃ |
| 15 | 14 | 2048 | 1025/**512** | 7/4 | 5 of 23 | singular |
| 16 | 15 | 4096 | 3073/3072 | 6/6 | 8 of 22 | singular |
| 17 | 13 | 1024 | 769/768 | 6/5 | 6 of 24 | singular |
| 18 | 13 | 1024 | 1024/1024 | 65/6 | 0 of 24 | 6₃ |
| 19 | 11 | 256 | 192/192 | 5/5 | 1 of 26 | 6₃ |

14 of 20 trial polygons are singular. In trials 4, 9 and 15 D112's self-intersection filter missed most singular alternatives (2048 accepted where none is simple in trial 9) — the one place the heuristic did fail. Trials 5, 14 and 18 have no illegal deletions and yield genuine 6₃ polygons; their exact spectra hold 6 distinct polynomials each, against D112's 9, 27 and 65.

D112's "power of 2 minus 1" SI counts: wherever its heuristic worked, D112's SI count = exact singular count − 1 (e.g. trial 3: 1024 singular, D112 1023; trial 8: 352 vs 351). The "−1" is the base polygon (choices = 0), which D112 always counted as valid without checking.

### Result 6: D110's classifier survives (Part E)

D110's 12-vertex scale-10 figure-eight is a simple figure-eight (6 crossings in the generic projection used; Jones t² − t + 1 − t⁻¹ + t⁻²). All 512 all-base alternatives are simple. Exact census: unknot 384 (Alexander = Jones = 1), figure-eight 64, right trefoil 32, left trefoil 32 — exactly D110's counts. The 4-check classifier assigns every alternative correctly once its chirality names are swapped (512/512); as published, its "left" trefoils are right-handed by Jones (D110 read chirality from the sign of one projection's writhe, with a reversed sign convention — consistent, so the partition is real, only the names are swapped).

So D110's central result stands on exact ground. Its later results (scale survey, influence profiles, 6₃ searches) used the same `long` arithmetic and the det-only classification and were not re-checked here.

### Result 7: D111's table (Part F)

Recomputed on D112's spectrum-path polygons — D112's simplification code is the one it copied from D111, and it reproduces D111's vertex counts (7, 8, 10, 11, 11, 12); D111's own binary was not run, so these are D111's method on D111-sized polygons rather than byte-identical D111 inputs:

| knot | alternatives | distinct dets D111 / exact | self-preserving D111 / exact | singular |
|---|---|---|---|---|
| 3₁ | 16 | 2 / 1 | 2 / 0 | 12 |
| 4₁ | 32 | 4 / 3 | 2 / 4 | 0 |
| 5₁ | 128 | 3 / 1 | 4 / 0 | 112 |
| 5₂ | 256 | 4 / 3 | 4 / 0 | 128 |
| 6₃ | 256 | 22 / 5 | 12 / 8 | 0 |
| 8₁₈ | 512 | 10 / 0 | 8 / 0 | 512 |

(Exact columns count simple alternatives only; "self-preserving" = det equal to the base polygon's, 0 when the base is singular.) Every D111 statistic built on these spectra — influence profiles, complement closure, palindromic Hamming profiles, U-shaped density, the 8₁₈ "consecutive odds" — inherits singular polygons and overflowed determinants and should be considered void.

## Lessons

1. **Unchecked arithmetic produced plausible fiction.** Overflowed determinants still came out odd, palindromic, and matched real knot tables (10₁₃₆, 9₄₂, …). Every sanity check D112 used (oddness, palindromy, |Δ(−1)| = det) passed on wrong data. Exact arithmetic is not a performance detail; it is what makes the result a result.
2. **A shortcut on the base case hid the worst error.** Using the braid's Fox polynomial for "choices = 0" meant the simplified polygon itself was never examined — and 10 of 12 were singular.
3. **"Invariant unchanged" is not "move legal".** A determinant-preserving deletion can pass the polygon through itself. Topology-preserving steps must be checked geometrically (triangle moves).
4. **Not every suspicion was right.** The sign-0 self-intersection heuristic agreed with the exact test on all non-base alternatives of the 12 reachability polygons (it did fail on three of the trial polygons). Overflow was suspected and confirmed only after a measurement mistake was corrected (sanitizer deduplication).
5. **What survives is worth more now.** D110's majority classifier is exact.

## Method notes

- Exact tools: `laqueus_simplex` (any touching or overlap is singular), `laqueus_diagramma_genericum` (provably generic projection), `diagramma_alexander` (Fox calculus over Z[t, t⁻¹], Bareiss), `diagramma_jones` (Kauffman bracket state sum; computed up to 16 crossings in the bulk censuses, 22 for base knots and D110), `laqueus_motus_removere` (checked triangle move).
- Construction words rebuilt exactly with `fractio`: vertex k ≥ 3 mirrored through the plane of vertices 0, 1, 2 iff bit k − 3 is set (D110 all-base = D112 base (0,1,2)).
- Provenance tools in this directory (not part of the demo build): `d112_export.c` (D112's polygons, removal sequences, verdicts → `d112_export.txt`, embedded in `main.c`), `d112_overflow_probe.c` (per-alternative overflow).
- Independent oracle cross-check (Result 3): the exact Python oracle from the laqueus review (scratch, not committed), run over all 256 6₃ alternatives: identical spectrum.
- Runtime ~80 s (dominated by the 7₂ census and the 20 trials).

## Open threads

1. **Re-run the construction-word experiment on honest polygons**: Part B's legal greedy simplifications are genuine knots of the right type for all 12 base knots. The D112 questions (spectra, reachability, stability) can now be asked for real, with Jones for chirality.
2. D110's later results (scale survey 8/196, influence profile {0, 1/8, 1/4}, CF convergents) used the same `long` arithmetic; re-check before relying on them.
3. Whether D110's classifier survives is now a clean question for other geometries (the scale survey's 4 groups).
