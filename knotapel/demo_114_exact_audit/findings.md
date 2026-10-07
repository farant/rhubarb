# Demo 114: Exact Audit of Demos 110–112 — Findings

**Date:** 2026-10-07 (revised the same day after review I)
**Status:** COMPLETE. 77 pass, 0 fail (the checks encode what is verified; the audit verdicts are below).
**Depends on:** Demos 110, 111, 112 (their polygons, as data); rhubarb house libraries `laqueus` (exact knots), `situs`, `fractio`, `polynomium`, `matrix`.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).
**Reviewed (two passes):** independently re-derived by the math reviewer with its own exact oracle (own embedding test, projection, Fox matrix, bracket and mirror construction — no code shared with laqueus); every number below reproduces. The review corrected two errors and several overstatements in the first version of this file (see "Revisions").

## Why this demo

D110–D112 built their construction-word results on homemade arithmetic: `Rat` on `long` with no overflow checks, crossings counted only at strictly interior intersections, a heuristic for self-intersection, and "simplification" that deletes a vertex whenever one projection's determinant is unchanged. rhubarb now has exact, reviewed libraries for all of this, so before building anything new on D110–D112 we re-checked the **same polygons** exactly.

Inputs are D112's own artifacts. `d112_export.c` compiles Demo 112's `main.c` **unmodified** (its `main` renamed) and prints its polygons, its removal sequences, and its per-alternative verdicts; the demo embeds those lines verbatim. The export reproduces D112's published self-intersection census exactly (3₁ 5/11 … 8₁₈ 1/511), and replaying D112's simplification loop reproduces its 44 simplified polygons exactly. (D111's own code, compiled unmodified by the reviewer, produces simplified polygons byte-identical to these, so Part F uses D111's actual inputs too.)

## Verdicts at a glance

| Claim (demo, result) | Verdict |
|---|---|
| D112: the 12 simplified stick polygons are the 12 base knots | **FALLS** — 10 of 12 are singular (the polygon passes through itself); only 4₁ and 6₃ are knots |
| D111/D112: simplification is "topology-preserving" | **FALLS** — every one of the 12 paths stops being an embedding, at a deletion the triangle-move check refuses (22 such steps); 4₁ and 6₃ later return to simple polygons of the right type, the other ten never do |
| D112 R1–R10: 6₃ spectrum = 37 distinct palindromic polynomials (10-crossing knots, non-alternating knots, connected sums, det up to 161) | **FALLS** — exactly 6: unknot, 3₁, 4₁, 5₁, 5₂, 6₃; caused by `long` overflow (proven by intervention: the same code with 128-bit integers gives the exact spectrum) |
| D112: 7₂ spectrum = 68 polynomials | **FALLS** — 7 |
| D112 Phase 4: reachability (6₃ out-degree 10, 7₂ 9) | **FALLS** — 6₃ 5, 7₂ 6; no knot with a singular base polygon reaches itself, and 3₁, 5₁, 6₁, 7₁, 7₃, 7₄, 8₁₈ reach nothing |
| D112 Phase 5: universal core {6₃} 20/20, union 118, 73 rare | **FALLS** — 6₃ occurs in only 11 of 20 trials (D112's 20/20 came from filling the base in by Fox calculus); exact union 9, rare 3; 13 of 20 trial polygons are singular |
| D112: self-intersection census (SI rates per knot) | **SURVIVES** for the 12 base knots, except the base polygon itself, which D112 never checked |
| D112: "100% palindromic after SI filter" | true but empty — exact Alexander polynomials are palindromic by theorem; D112's wrong polynomials were palindromic too, so palindromy never certified anything |
| D111 Results 3–20 (det spectra, influence, complement closure, Hamming profiles, 8₁₈ 28 dets) | **VOID / UNSUPPORTED** — computed on singular polygons with overflowing arithmetic and no SI filter; the det spectra recomputed here collapse (6₃ 22 dets → 5; every 8₁₈ alternative is singular); the derived statistics were not recomputed |
| D110 Result 2: the 4-check majority classifier is perfect on the 512 alternatives | **SURVIVES** — all 512 are simple: 64 genuine figure-eights (no cinquefoils), 384 certified unknots, 32 + 32 trefoils |
| D110: "left-trefoil if b2 = 1" | **NAMES SWAPPED** — D110's crossing signs are the standard ones; it named positive-writhe trefoils "left", and Jones says they are right-handed (32/32 each way) |

## Results

### Result 1: D112's simplified polygons are mostly not knots (Part A)

Exact embedding test on D112's polygons (raw braid polygon / reachability simplification / spectrum simplification; vertices / crossings of the projection Jones was computed on):

| knot | raw | reach | spec | raw Alexander | reach | spec |
|---|---|---|---|---|---|---|
| 3₁ | 18/8 | 7/— | 7/— | table | **singular** | **singular** |
| 4₁ | 29/13 | 8/4 | 8/4 | table | table, raw's Jones | table |
| 5₁ | 26/10 | 10/— | 10/— | table | **singular** | **singular** |
| 5₂ | 37/17 | 11/— | 11/— | table | **singular** | **singular** |
| 6₁ | 48/19 | 12/— | 14/— | table | **singular** | **singular** |
| 6₂ | 37/17 | 10/— | 10/— | table | **singular** | **singular** |
| 6₃ | 37/17 | 11/20 | 11/20 | table | table, raw's Jones | table |
| 7₁ | 34/12 | 13/— | 13/— | table | **singular** | **singular** |
| 7₂ | 56/21 | 14/— | 14/— | table | **singular** | **singular** |
| 7₃ | 45/17 | 11/— | 11/— | table | **singular** | **singular** |
| 7₄ | 56/21 | 14/— | 14/— | table | **singular** | **singular** |
| 8₁₈ | 49/19 | 12/— | 12/— | table | **singular** | **singular** |

Every raw braid polygon is a genuine knot with the table Alexander polynomial (D112's braid layout is fine). The damage is done by the simplification. The singularities are genuine 3D intersections (interior crossings of two segments, or a segment through a vertex), e.g. 3₁ has two: segments 0 and 3 meet at (10, 0, 30) and segments 1 and 4 at (40/3, −1/3, 130/3).

Hand check of the first (D112's simplified trefoil `(0,0,0) (20,0,60) (0,−1,10) (0,0,20) (20,0,40) (0,−1,50) (0,−105,0)`): segment 0 is (20a, 0, 60a), segment 3 is (20b, 0, 20 + 20b); a = b = ½ gives the same point (10, 0, 30), interior to both.

D112 never noticed because its base case (choices = 0) used the braid's Fox-calculus polynomial instead of the polygon, so the polygon itself was never examined.

### Result 2: every simplification breaks the embedding (Part B)

D112 deletes a vertex when the (2,3,5)-projection determinant is unchanged. Replaying each deletion as a triangle move: a deletion is a guaranteed isotopy step if the swept triangle touches nothing else. The check presumes a simple polygon — once the polygon is singular, a refusal only says the triangle meets a singular polygon — so refusals are counted on simple polygons only, together with the steps where a simple polygon becomes singular:

| knot | deletions | refused (any) | refused on a simple polygon | simple → singular (first) | singular → simple | ends | legal greedy |
|---|---|---|---|---|---|---|---|
| 3₁ | 11 | 6 | 2 | 2 (#2) | 1 | singular | 18 → 7 |
| 4₁ | 21 | 2 | 1 | 1 (#7) | 1 | simple | 29 → 9 |
| 5₁ | 16 | 9 | 3 | 3 (#2) | 2 | singular | 26 → 11 |
| 5₂ | 26 | 8 | 1 | 1 (#3) | 0 | singular | 37 → 12 |
| 6₁ | 36 | 13 | 1 | 1 (#5) | 0 | singular | 48 → 14 |
| 6₂ | 27 | 8 | 1 | 1 (#3) | 0 | singular | 37 → 10 |
| 6₃ | 26 | 5 | 1 | 1 (#4) | 1 | simple | 37 → 12 |
| 7₁ | 21 | 13 | 4 | 4 (#2) | 3 | singular | 34 → 15 |
| 7₂ | 42 | 13 | 1 | 1 (#3) | 0 | singular | 56 → 17 |
| 7₃ | 34 | 9 | 3 | 3 (#2) | 2 | singular | 45 → 14 |
| 7₄ | 42 | 21 | 3 | 3 (#3) | 2 | singular | 56 → 18 |
| 8₁₈ | 37 | 10 | 1 | 1 (#12) | 0 | singular | 49 → 12 |

Over the 12 paths: 117 refused deletions, of which **22 on a simple polygon — and each of those 22 is exactly a step that turns a simple polygon into a singular one** (asserted: refusal-on-simple ⇔ break). Such a step is definitively not an isotopy step: its output is not an embedding. After the first break D112 keeps deleting vertices from a singular polygon; 4₁ and 6₃ happen to return to simple polygons of the right type, the other ten never do.

A refusal does not by itself change the knot type: in trial 19 (Part D) one refused deletion leaves the polygon simple, and the path ends on a 6₃ (Alexander and Jones).

A **legal** greedy simplification (same scan order, `laqueus_motus_removere`) always keeps the knot (checked: same Alexander polynomial) and lands 0–4 vertices above D112's counts (3₁, 6₂ and 8₁₈ need none). These are the honest polygons for a future re-run.

### Result 3: the 6₃ "knot factory" was integer overflow (Part C)

On D112's 6₃ polygon (one of the two that is a knot), all 256 alternatives are simple — D112 agrees. The exact spectrum, refined by Jones (computed for every alternative, on a fewest-crossing projection when needed):

| Alexander | Jones | knot | population |
|---|---|---|---|
| 1 | 1 | unknot (certified) | 144 |
| t² − t + 1 | −t⁴ + t³ + t | right trefoil | 36 |
| t² − t + 1 | t⁻¹ + t⁻³ − t⁻⁴ | left trefoil | 36 |
| 2t² − 3t + 2 | (both chiralities) | 5₂ | 8 + 8 |
| t⁴ − 3t³ + 5t² − 3t + 1 | (amphichiral) | 6₃ | 8 |
| t⁴ − t³ + t² − t + 1 | (both chiralities) | 5₁ | 4 + 4 |
| t² − 3t + 1 | (amphichiral) | 4₁ | 8 |

D112 reported 37 polynomials (unknot 76, trefoil 68, …, det 161).

- **Unknots are proven, not assumed:** each of the 144 has Δ = 1 on a diagram with ≤ 10 crossings, so its crossing number is ≤ 10; and every nontrivial knot of crossing number ≤ 10 has a nontrivial Alexander polynomial — by the knot tables (Rolfsen; KnotInfo), the first knots with Δ = 1 are 11n34 and 11n42. The certificate rests on that external table fact, which the demo does not check.
- **The chirality split is exact by symmetry:** vertices 0, 1, 2 lie on the mirror plane, so alternative c XOR (all ones) is exactly the mirror image of alternative c. The demo asserts this pairing (same simplicity, mirror Jones) for all 12 knots.
- **Independent confirmation:** the reviewer's oracle, with its own mirror construction (vertex-for-vertex identical to D112's `evaluate_inplace` on all 256), gives the same spectrum.

**Mechanism.** (2,3,5), the direction D112 used, is generic for all 256 alternatives, so the projection was sound. In the alternative compared crossing by crossing (18), D112's 19 crossings match the exact ones except for one over/under flip, at the crossing of edges 3 and 7. Exact depths there are −51866387140/245776949 (edge 3) and 17651979460/245776949 (edge 7), so edge 7 is over; D112's `rat_cmp` forms −51866387140 × 245776949 = −1.27 × 10¹⁹, beyond `long` range (9.22 × 10¹⁸), and the wrapped value says edge 3.

- **Correlation:** run per alternative under the signed-overflow sanitizer (`d112_overflow_probe.c`, one process per alternative), 221 of 256 alternatives overflow; all 144 wrong polynomials are among them; 77 overflowed and came out right by luck.
- **Cause (the reviewer's intervention):** D112's code with only `typedef long i64` changed to `__int128` — no overflow anywhere under the sanitizer — gives exactly the exact spectrum, 0 of 255 non-base alternatives differing. Same algorithm, same input, only the width changes: overflow is the whole cause. (D112's Alexander matrix itself is correct: laqueus's rows with in/out swapped for both signs, which leaves Δ unchanged.)

### Result 4: all spectra and the reachability graph shrink (Part C)

| knot | alternatives | simple (D112 / exact) | distinct Alexander (D112 / exact) | Alexander \| Jones pairs |
|---|---|---|---|---|
| 3₁ | 16 | 5 / 4 | 2 / 1 | 1 |
| 4₁ | 32 | 32 / 32 | 4 / 3 | 4 |
| 5₁ | 128 | 17 / 16 | 2 / 1 | 1 |
| 5₂ | 256 | 129 / 128 | 4 / 3 | 4 |
| 6₁ | 512 | 1 / 0 | 1 / 0 | 0 |
| 6₂ | 128 | 65 / 64 | 4 / 3 | 4 |
| 6₃ | 256 | 256 / 256 | 37 / 6 | 9 |
| 7₁ | 1024 | 33 / 32 | 2 / 1 | 1 |
| 7₂ | 2048 | 1025 / 1024 | 68 / 7 | 12 |
| 7₃ | 256 | 1 / 0 | 1 / 0 | 0 |
| 7₄ | 2048 | 1 / 0 | 1 / 0 | 0 |
| 8₁₈ | 512 | 1 / 0 | 1 / 0 | 0 |

The "simple" columns differ by exactly the base polygon wherever it is singular: D112's sign-0 self-intersection heuristic agrees with the exact test on **every** non-base alternative of these 12 polygons. (One of our suspicions going in; it did not hold here.) 7₂'s 664 trivial-Alexander alternatives all have Jones = 1; 423 of them are certified unknots by the ≤ 10-crossing rule, the rest are not certified by this demo.

Exact reachability out-degrees (D112 in parentheses): 4₁ 2 (3), 5₂ 2 (3), 6₂ 2 (3), 6₃ 5 (10), 7₂ 6 (9), all others 0 (1). A knot whose base polygon is singular never reaches itself (5₂, 6₂ and 7₂ still reach 3₁ and 4₁ through simple alternatives).

### Result 5: the stability trials (Part D)

| trial | verts | alternatives | simple D112/exact | distinct D112/exact | breaks / refusals on simple | base polygon |
|---|---|---|---|---|---|---|
| 0 | 11 | 256 | 256/256 | 37/6 | 1/1 | 6₃ |
| 1 | 15 | 4096 | 1/0 | 1/0 | 1/1 | singular |
| 2 | 11 | 256 | 256/256 | 11/4 | 1/1 | 6₃ |
| 3 | 15 | 4096 | 3073/3072 | 6/6 | 1/1 | singular |
| 4 | 14 | 2048 | 1025/**0** | 3/0 | 1/1 | singular |
| 5 | 13 | 1024 | 1024/1024 | 9/6 | 0/0 | 6₃ |
| 6 | 15 | 4096 | 3073/3072 | 21/6 | 1/1 | singular |
| 7 | 12 | 512 | 385/384 | 10/5 | 2/2 | singular |
| 8 | 13 | 1024 | 673/672 | 9/4 | 1/1 | singular |
| 9 | 14 | 2048 | 2048/**0** | 6/0 | 1/1 | singular |
| 10 | 15 | 4096 | 1793/1792 | 5/4 | 1/1 | singular |
| 11 | 16 | 8192 | 4097/4096 | 65/9 | 1/1 | singular |
| 12 | 14 | 2048 | 1345/1344 | 5/4 | 1/1 | singular |
| 13 | 13 | 1024 | 1024/1024 | 16/6 | 1/1 | 6₃ |
| 14 | 11 | 256 | 256/256 | 27/6 | 0/0 | 6₃ |
| 15 | 14 | 2048 | 1025/**512** | 7/4 | 1/1 | singular |
| 16 | 15 | 4096 | 3073/3072 | 6/6 | 1/1 | singular |
| 17 | 13 | 1024 | 769/768 | 6/5 | 3/3 | singular |
| 18 | 13 | 1024 | 1024/1024 | 65/6 | 0/0 | 6₃ |
| 19 | 11 | 256 | 192/192 | 5/5 | 0/1 | 6₃ |

13 of 20 trial polygons are singular; the 7 simple ones (trials 0, 2, 5, 13, 14, 18, 19) are 6₃ by Alexander and Jones. In trials 4, 9 and 15 D112's self-intersection filter accepted alternatives that are singular (2048 accepted in trial 9, where none is simple) — the one place the heuristic failed.

**Union over the trials** (number of trials containing each polynomial): 3₁, 4₁ and the unknot 17; 5₂ 16; 5₁ 11; **6₃ 11**; and once each (trial 11) the Alexander polynomials of 6₁ (2t² − 5t + 2), 6₂ (t⁴ − 3t³ + 3t² − 3t + 1) and 7₆ (t⁴ − 5t³ + 7t² − 5t + 1). Union **9**, rare **3** — against D112's union 118 and 73 rare. D112's "universal core {6₃}, 20/20" held only because the base was filled in by Fox calculus; 6₃ actually occurs in 11 trials.

D112's "power of 2 minus 1" SI counts: in the trials whose base polygon is singular and where the heuristic worked, D112's SI count = exact singular count − 1 (e.g. trial 3: 1024 vs 1023; trial 8: 352 vs 351) — the "−1" is the base polygon, always counted valid. Where the base polygon is simple the counts agree exactly (trial 19: 64 vs 64).

### Result 6: D110's classifier survives (Part E)

D110's 12-vertex scale-10 figure-eight is a simple figure-eight (Jones t² − t + 1 − t⁻¹ + t⁻²). All 512 all-base alternatives are simple. Exact census: unknot 384 (certified: Δ = 1 on ≤ 10 crossings, and Jones = 1), figure-eight 64, right trefoil 32, left trefoil 32 — exactly D110's counts, and the 4-check classifier assigns every alternative correctly once its chirality names are swapped (512/512).

The swap: D110's crossing signs are the standard ones. The reviewer matched them to the exact physical convention crossing by crossing on all 512; algebraically, on D110's projection (5x − 2z, 5y − 3z) the 2D cross product of the projected over and under directions equals 5·det[over, under, (2,3,5)] (Binet–Cauchy: the projection rows (5,0,−2) and (0,5,−3) have cross product 5·(2,3,5)), so D110's sign is the sign of that determinant, with over = larger 2x + 3y + 5z — the same viewer side as laqueus. D110 simply **named positive writhe "left"**. All 32 of its "left" trefoils have positive writhe along (2,3,5) and right-handed Jones; all 32 "right" ones have writhe ≤ 0 and left-handed Jones. (The writhe of one projection is not a chirality invariant; here it happens to agree with Jones on all 64.)

So D110's central result stands on exact ground. Its later results (scale survey, influence profiles, 6₃ searches) used the same `long` arithmetic and the det-only classification and were not re-checked here.

### Result 7: D111's table (Part F)

On D111's own polygons:

| knot | alternatives | distinct dets D111 / exact | self-preserving D111 / exact | singular |
|---|---|---|---|---|
| 3₁ | 16 | 2 / 1 | 2 / 0 | 12 |
| 4₁ | 32 | 4 / 3 | 2 / 4 | 0 |
| 5₁ | 128 | 3 / 1 | 4 / 0 | 112 |
| 5₂ | 256 | 4 / 3 | 4 / 0 | 128 |
| 6₃ | 256 | 22 / 5 | 12 / 8 | 0 |
| 8₁₈ | 512 | 10 / 0 | 8 / 0 | 512 |

(Exact columns count simple alternatives only; "self-preserving" = determinant equal to the knot's table determinant.) The statistics D111 derived from these spectra — influence profiles, complement closure, palindromic Hamming profiles, U-shaped density, the 8₁₈ "consecutive odds" — were not recomputed; they rest on singular polygons and overflowed determinants and are unsupported.

## Lessons

1. **Unchecked arithmetic produced plausible fiction.** Overflowed determinants still came out odd, palindromic, and matched real knot tables (10₁₃₆, 9₄₂, …). Every sanity check D112 used (oddness, palindromy, |Δ(−1)| = det) passed on wrong data. Exact arithmetic is not a performance detail; it is what makes the result a result.
2. **A shortcut on the base case hid the worst error.** Using the braid's Fox polynomial for "choices = 0" meant the simplified polygon itself was never examined — and 10 of 12 were singular. The same shortcut manufactured the "universal core" of the stability study.
3. **"Invariant unchanged" is not "move legal".** A determinant-preserving deletion broke the embedding on every one of the 12 paths. Topology-preserving steps must be checked geometrically, and a move check is only meaningful on an embedding.
4. **Not every suspicion was right, and some of ours were wrong.** The sign-0 self-intersection heuristic agreed with the exact test on all non-base alternatives of the 12 reachability polygons. Our first mechanism guess (non-generic projections) was wrong; overflow was confirmed only after correcting a measurement mistake (the sanitizer reports each source line once per process); and the first version of these findings counted refusals on already-singular polygons and misattributed D110's naming swap to its sign convention — corrected by review.
5. **What survives is worth more now.** D110's majority classifier is exact.

## Method notes

- Exact tools: `laqueus_simplex` (any touching or overlap is singular), `laqueus_diagramma_genericum` (provably generic projection), `diagramma_alexander` (Fox calculus over Z[t, t⁻¹], Bareiss), `diagramma_jones` (Kauffman bracket state sum), `laqueus_motus_removere` (checked triangle move).
- Jones: on the first generic projection when it has ≤ 16 crossings (22 for base knots and D110); otherwise on the fewest-crossing generic projection among directions with components in −2..2. Every simple alternative of the 12 reachability polygons gets a Jones polynomial this way.
- Unknot certification: Δ = 1 on a diagram with ≤ 10 crossings (searched the same way when the first projection has more), using the table fact that no nontrivial knot of crossing number ≤ 10 has Δ = 1. For 7₂, widening the search to components in −4..4 would certify 171 more (594 of 664, per the reviewer); not done here.
- Construction words rebuilt exactly with `fractio`: vertex k ≥ 3 mirrored through the plane of vertices 0, 1, 2 iff bit k − 3 is set (D110 all-base = D111 default word = D112 base (0,1,2)).
- Provenance tools in this directory (not part of the demo build): `d112_export.c` (D112's polygons, removal sequences, verdicts → `d112_export.txt`, embedded in `main.c`), `d112_overflow_probe.c` (per-alternative overflow). The 128-bit intervention and the independent oracle are the reviewer's (scratch, not committed).
- Runtime: 1–3 minutes depending on the machine (68 s and 2 min 50 s measured, both -O2; the 7₂ census and the 20 trials dominate).

## Revisions (after reviews I and II, 2026-10-07)

- "117 illegal deletions" → 22 refusals on simple polygons, each the step that breaks the embedding (95 were judged on already-singular polygons, where the move check has no isotopy meaning).
- D110: the sign convention is standard; the label for positive writhe was reversed.
- Jones refinement completed (it had counted "not computed" placeholders as pairs): 6₃ 9 pairs, trefoils 36/36 (a theorem, by the c ↔ ~c mirror symmetry), 7₂ 12 pairs.
- "SI count = exact − 1" restricted to trials with a singular base polygon.
- The stability union was claimed to fall without being computed; now computed (9, rare 3, 6₃ in 11/20 trials).
- D111's inputs confirmed identical (hedge removed); D111's derived statistics marked unsupported rather than "fall"; overflow shown causal by the reviewer's 128-bit intervention; unknots certified.
- Review II: the unknot certificate's table fact named and sourced; the D110 sign sentence made precise; trial base polygons now get Jones too (the 7 simple ones are 6₃ by Alexander and Jones; this check also caught an old miscount: 13 of 20 trial polygons are singular, not 14); a skipped knot in Part C now fails a check instead of dropping out of the reachability table. Review II also reproduced all 12 raw-polygon Jones polynomials.
