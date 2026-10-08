# Demo 117: Exact Audit of the Cyclotomic Demos — Findings

**Date:** 2026-10-07
**Status:** COMPLETE. `main.c` (Parts A–B): 22 pass, 0 fail, about 1 minute. Parts 0 and C–F are scripts in this folder (below); their verdicts are recorded here.
**Depends on:** Demos 29, 56, 79–83, 90, 100, 106 (their code, copied verbatim or patched in scratch copies; no shipped demo file is modified); rhubarb house libraries `cyclotomia`, `polynomium`, `magnus`.
**Frozen copy:** `demo-snapshot.c` (knotapel/archive.sh).

## Why this demo

57 knotapel demos do cyclotomic arithmetic by hand: `Cyc8` and friends on `long`, quaternions and 3 × 3 matrices on `double` or `long`, with fixed-size arrays everywhere. rhubarb now has an exact, reviewed ring library (`cyclotomia`, Z[ζ<sub>n</sub>] on big integers). D114 showed what `long` overflow did to D111–D112. This demo asks the same question of the whole cyclotomic line: where did undefined behaviour or a silent limit change a published number?

The method: triage all 57 under UBSan, then audit each demo that reported something, by an independent exact computation (Parts A–B) or by intervention (Parts C–F: the shipped code against the same code with the fault removed, compared byte for byte). Along the way the audit found a second family of faults UBSan cannot see: **silent caps**, fixed arrays that stop accepting entries without a word.

## Verdicts at a glance

| Claim (demo) | Verdict |
|---|---|
| D29: exact brackets of the braid catalog | **SURVIVES** — all 87,890 braids agree with a second route (symbolic δ, evaluated in `cyclotomia`); the float filter \|z\| > 0.5 keeps exactly the nonzero ones |
| D29: "8,192 braids, 64 distinct bracket values" | **CAP** — `MAX_CATALOG 8192` stops at 8,192 of 69,960 nonzero brackets; all of them take **100** distinct values (36 hidden) |
| D56: Boolean search over its nine partition functions (Re > 0, split-sigmoid, sectors k = 4, 6) | **SURVIVES** exactly: 11 / 10 / 10 / 11 classes, parity 113 / 19 / 19 / 32 |
| D56: "12 of 13 classes, including parity, with a 2-sector activation" | **FALLS to 11 of 13** — class 0x06 ~A(B^C) (3 solutions in D56) has 0 exact solutions; 92 of 729 weight triples overflow `long`. Parity (32) survives |
| D79: "64 directions / 65 cells", "512 directions" | **CAP** — `MAX_DIR` 64 and 512; the catalogs have **112** and **2,043** axes |
| D79: XOR10 = 124 at k = 8 | **CAP** — 214 at k = 6 with all 112 axes (and main.c's cell budget `MAX_ACT_CELLS 1024` still cuts its k ladder) |
| D79: XOR12 solvable with the 4,096 catalog ("50+, min k = 8, 4,104 cells") | **SURVIVES, relabelled** — reproduced exactly; "50+" is `xor12_fast`'s stop at 50. With all 2,043 axes, 50 winners appear after 192 candidates, but need ≥ 12,264 cells: the capped table (≈ one cell per mask) is the *stronger* result, of a coarser activation |
| D80–D83: direction counts "512 (saturated)" | **CAP** — 1,708 to 2,431 directions; max XOR, finiteness verdicts, the D81 ladder and D82's matched-size control **survive** |
| D81/D82: "after round 7 directions saturate; XOR10 → XOR12 needs finer angles, not more directions" | **FALLS** — the saturation is the cap; uncapped, depth 7 adds 468 new directions and depth 8 adds 1,068 |
| D90: phase 2d results | **SURVIVE** — the stack overrun crashes the plain -O0 build; at -O2 the output equals a patched build byte for byte |
| D100, D106: results of the 3 × 3 sign hash | **SURVIVE as computed** (a defined hash reproduces every byte) **but are hash-specific** — the hash overflows `int`; the intended hash moves most counts (D100 deep XOR8 1,169 → 3,576) and restores an AND/OR symmetry the overflow breaks |

## Part 0: UBSan triage of all 57 demos (`triage.sh`)

Every demo whose `main.c` mentions cyclotomic arithmetic, built unmodified with `clang -std=c89 -O1 -fsanitize=undefined` and run under a time limit (600 s, then 3,600 s for the seven that timed out or failed to build). The merged table is `triage.tsv`. UBSan reports each source location once per process, so the counts are sites, not occurrences: a demo with one site has exactly one faulting operation in its code.

- **52 demos clean** (D101 took 31 minutes).
- **UB in 5:** D56 (`long` overflow in its triple products), D79 (out-of-bounds read `dir_count[64]`), D90 (out-of-bounds writes into a stack `double[4096]`), D100 and D106 (`int` overflow in the activation hash).
- Nonzero exits elsewhere (D27, D47, D49, D50, D89, D91, D92, D102, D103) are each demo's count of failed predictions, as shipped; D90's abort is its overrun.
- UBSan cannot see silent caps. Those were found by reading (D29, D79) and then by a sweep (Part F).

## Part A: demo 29, every bracket by a second route (`main.c`)

D29's code is copied verbatim (its `Cyc8` type, multiplication and state sum). The second route: tally the states of each braid by (writhe contribution, loop count), build the bracket as a Laurent polynomial in A with δ = −A² − A⁻² kept symbolic (`polynomium`), and evaluate it at A = ζ<sub>8</sub><sup>5</sup> in Z[ζ<sub>8</sub>] (`cyclotomia`).

- D29's A = (0, −1, 0, 0) is ζ<sub>8</sub><sup>5</sup>, and its 16-product multiplication table equals Z[ζ<sub>8</sub>] multiplication (10,000 random pairs, coefficients up to 1,000).
- **All 87,890 braids** of D29's enumeration agree exactly between the two routes.
- The float filter \|z\| > 0.5 keeps exactly the 69,960 nonzero brackets.
- D29's catalog stops at `MAX_CATALOG` = 8,192 entries, so "Total braids with \|bracket\| > 0.5: 8192" is the cap. The first 8,192 take 64 distinct values (D29's number, reproduced); all 69,960 take **100**.

This is the second cap in D29: note 16 of `docs/knotapel.html` already corrects the triple count (65,536 was the array size).

## Part B: demo 56, exact products in its Boolean search (`main.c`)

D56's strip graphs, Fortuin–Kasteleyn sums, activations and NPN classification are copied verbatim (renamed only where D56's locals collide with latina.h macros: `si` → `s_re`/`s_im`). Each weight triple product w₁w₂w₃ is recomputed in Z[ζ<sub>8</sub>] and classified by exact predicates: real when z equals its conjugate, imaginary when z equals minus its conjugate, sector index ⌊q·k/4⌋ from the exact quadrant and axis tests.

- D56's ten partition functions are recomputed exactly: 9 distinct nonzero values, as D56 says.
- **92 of 729** triple products overflow `long` in D56's arithmetic (UBSan's site main.c:106).
- No exact value lies within float error of a boundary, and D56's float classification of every exact value agrees, so the only damage is the overflow.

| Activation | D56 classes / parity | exact |
|---|---|---|
| Re(z) > 0 | 11 / 113 | 11 / 113 |
| split-sigmoid | 10 / 19 | 10 / 19 |
| sector k = 2 | **12** / 32 | **11** / 32 |
| sector k = 4 | 10 / 19 | 10 / 19 |
| sector k = 6 | 11 / 32 | 11 / 32 |

At k = 2 the exact per-class counts are 0x01:4 0x03:129 0x06:0 0x07:36 0x0F:93 0x16:54 0x17:22 0x18:30 0x19:75 0x1B:0 0x1E:129 0x3C:93 0x69:32. D56's twelfth class, 0x06 ~A(B^C), had 3 solutions, all from overflowed products: **0 exactly**. Other classes moved too (AND3' 20 → 4, AND2' 114 → 129, ~A~(BC) 57 → 36, BUF 69 → 93) without changing which classes are reached. The headline becomes "11 of 13, including parity".

## Part C: demo 90's stack overrun (`audit_d90.sh`)

Phase 2d writes n_gen × n_depth cross-depth sums into `double depth_angles[MAX_QCAT]` (4,096) on the stack: 5 × 1,152 = 5,760 at depth 7 and 9,020 at depth 8.

- Built the way knotapel builds (plain `cc`, -O0), D90 **segfaults** in phase 2d (exit 139).
- At -O2 the overrun happens not to corrupt anything that is printed: the output equals a patched build (array static, 20 × 4,096) byte for byte (288 lines). The published numbers stand, including "73 angles in 50,000 pairs" (phase 2c, before the overrun).
- Minor: one overlap line prints "4/3" (a numerator above its denominator), and `count_distinct` saturates at 2,048.

## Part D: demo 79's direction caps (`audit_d79.sh`, about 55 minutes)

D79 reads each quaternion through a Voronoi table of the catalog's rotation axes. The helper that collects axes stops adding at `MAX_DIR` without a word: 64 in `main.c`, 512 in `catalog_test.c`, `xor12_fast.c`, `xor12_retest.c`. Directions beyond the cap are read as their nearest kept axis, so a capped run is a valid computation of a **coarser** activation, but the counts it prints are the cap, not the geometry.

- The 256-entry catalog has **112** axes (main.c reports 64); the 4,096-entry catalog has **2,043** (the companions report 512).
- main.c's out-of-bounds read `dir_count[64]` only changes the identity row of its table at -O0 (2 instead of 1); -O2 equals the fixed build byte for byte.
- main.c also caps cells at `MAX_ACT_CELLS 1024`, which cuts the k ladder {1, 6, 8, 10, 12, 16, 20, 24} at k ≤ 12 with 65 cells and k ≤ 8 with 113.
- Winner lists stop at `MAX_WIN 1024` (XOR6 and XOR8 saturate in every variant); `xor12_fast` stops at 50 winners (the published "50+"), `xor12_retest` at 20.

| main.c | capped (64 axes) | all 112 axes |
|---|---|---|
| XOR6 | 1,024 (k = 1, 65 cells) | 1,024 (k = 1, 113) |
| XOR8 | 1,024 (k = 6, 390) | 1,024 (k = 6, 678) |
| XOR10 | 124 (k = 8, 520) | **214** (k = 6, 678) |
| XOR12 | 0 | 0 |

All 14 tests pass in every variant. XOR12 = 0 belongs to the 256-entry catalog, as D79's own findings already say.

XOR12 with the 4,096 catalog, from D79's 124 XOR10 parents (`xor12_fast`):

| | axes | winners (stop) | candidates tried | minimum k (cells) | time |
|---|---|---|---|---|---|
| shipped | 512 | 50 | 46,874 | k = 8 (4,104) | 16.5 min |
| uncapped (axes and cells) | 2,043 | 50 | 192 | k = 6 (12,264) | 20 s |

The shipped run reproduces the published result exactly. The qualitative claim stands: ζ<sub>12</sub> passes XOR12. But the capped activation separates 4,096 masks with about one cell per mask, while every uncapped winner needs at least three cells per mask. Fewer, coarser cells that still separate are the stronger statement; the label "512 directions" describes the activation, not the group. (`xor12_retest`, which finds its own XOR10 parents on 112 axes with an 8,192-cell budget, gets 1,024 XOR10 parents, the `MAX_WIN` cap, not D79's 124, and finds 20 XOR12 winners either way.)

## Part E: the activation hash of demos 100 and 106 (`audit_hash.sh`, about 9 minutes)

`mat3_activate` reads the signs of the 36 integers of a 3 × 3 matrix over Z[ζ<sub>8</sub>] as base-3 digits: `h = h * 3 + t` in an `int`, then `if (h < 0) h = -h; return h % k`. Since 3<sup>36</sup> ≈ 1.5 × 10<sup>17</sup>, every call overflows. UBSan reports this site and **no other** in both full runs, so no `long` arithmetic overflowed anywhere else in D100 or D106.

1. **What was computed.** A defined version (the digits in `unsigned int`, which wraps mod 2³² by the standard, the magnitude taken in unsigned arithmetic, then mod k) reproduces both demos byte for byte (336 and 185 lines). The one value where the shipped code would produce a negative cell index (a wrap onto INT_MIN) never occurs. The published numbers are exactly what the code computes under wrapping.
2. **What was intended.** H = Σ t<sub>i</sub>3<sup>35−i</sup> < 3<sup>36</sup> fits a 64-bit `unsigned long`, and the cell is H mod k. Every k used is a power of two dividing 2³², so the wrap alone loses nothing; what changes the cells is the sign fold, reachable only through the overflow (patterns whose wrapped value has bit 31 set land in (−H) mod k). Under the intended hash all tests still pass (36/36, 20/20), but most counts move:
   - D100 capacity scan, k = 64…512: XOR6 1,158/1,937/2,510/2,961 → 1,985/2,564/2,944/3,147; XOR8 393/822/2,376/6,187 → 1,163/2,879/6,533/10,355; XOR12 48 at every k → 48/48/67/106. Deep entries: XOR6 2,017 → 2,598, XOR8 1,169 → 3,576, XOR10 654 → 883; XOR12 305 and XOR14 70 unchanged.
   - D106: XOR6 triples 168,100 → 218,375; mean scores, periodic 3,969 / pseudo-Anosov 3,857 → 5,180 / 4,970. The Boolean sweep's largest periodic-minus-pseudo-Anosov gap is 3.7 points shipped, but 6.6 (AND2) and 7.3 (XOR2) under the intended hash, both favouring the periodic braids. The verdict (pseudo-Anosov braids do not compute better) is unchanged; "no gap above 3.7 points" is a property of the hash that happened to run.
3. **A free check the demos could assert.** Negating every input flips every trit, H → 3<sup>36</sup> − 1 − H, a bijection on cells; so AND3 and OR3 (likewise NAND3 and NOR3) must score the same. Under the intended hash they do (320,265 each); the shipped hash breaks it (AND3 313,332, OR3 320,929).

## Part F: the direction cap in demos 74–97 (`audit_dircap.sh`, `audit_cap_impact.sh`)

24 demos (D74–D97, 25 programs with D79's companions) copy D79's axis helper, with `MAX_DIR` 32, 64, 512 or 2,048. `audit_dircap.sh` rewrites the test so every refused direction is counted on stderr without changing the program's behaviour (`g_nd < MAX_DIR || (fprintf(stderr, "DIRCAP\n"), 0)`), and runs all of them (about 30 minutes on 6 cores; table `dircap.tsv` in its work directory).

- **The cap bites in D79–D83 only** (all at 512, D79's main.c at 64).
- It never bites in D74–D78 (32), D84 (512), D87–D94 (2,048) or D95–D97 (64).

`audit_cap_impact.sh` then runs D80–D83 shipped and uncapped (`MAX_DIR` 65,536; in D81–D83 also `MAX_ACT` 131,072, because more axes push k × cells past the 16,384-cell budget and cut the k ladder, failing one test in each). All tests pass uncapped (11, 14, 17, 12).

- **D80** (group finiteness): direction counts 512 → ζ<sub>6</sub> 1,965, ζ<sub>10</sub> 2,431, ζ<sub>12</sub> 2,043, ζ<sub>16</sub> 1,708, ζ<sub>20</sub> 1,938. Every finiteness verdict unchanged.
- **D81** (capacity scaling): only the direction column changes (round 7: 975, round 8: 2,043). The max-XOR ladder and its fit are unchanged. Its "after round 7, directions saturate at 512" is the cap.
- **D82** (crossing depth): new directions per closure round at depths 7 and 8 are **468 and 1,068**, not 5 and 0. The "angle refinement" reading of depths 7–8 ("XOR10 → XOR12 requires not more directions but finer angles") rests on the cap and falls. The matched-size control **survives**: strided 564 has 517 axes and 43 angles against deep 564's 476 and 19, and only the deep set reaches XOR12. The individual XOR12 winners change (their mean depth 1.98 → 1.68).
- **D83** (framing): bracket 2,043 axes, "Jones-normalized" 1,389 (both reported as 512). Angles (43 → 31) and max XOR (12 vs 10) unchanged; mixed-writhe XOR12 winners 4/32 → 0/32.

## Reproducing

| Script | Covers | Time |
|---|---|---|
| `triage.sh <work> [limit] [jobs] [demo …]` | Part 0 | ~1 h at 6 jobs, plus reruns |
| `main.c` (aedilis) | Parts A, B | 1 min |
| `audit_d90.sh <work>` | Part C | ~1 min |
| `audit_d79.sh <work>` | Part D | ~55 min |
| `audit_hash.sh <work>` | Part E | ~9 min |
| `audit_dircap.sh <work> [limit] [jobs]` | Part F sweep | ~30 min at 6 jobs |
| `audit_cap_impact.sh <work>` | Part F impact | ~6 min at 8 jobs |

Every script copies the demo sources into the work directory, checks that its patch anchors exist exactly once, and leaves the shipped demos untouched.

## Lessons

- **Silent caps are the dominant fault in this line**, not arithmetic: D29 (catalog), D79 (axes, cells, winners, stop counts), D80–D83 (axes). A cap is harmless when it prints that it was hit; every one of these was silent, and three demos built interpretations on the saturated count.
- **Tests that assert thresholds survive any of this.** Every audited demo passes all its tests in every variant (capped, uncapped, either hash). The tests could not have caught these faults; the numbers in the logs are what changed.
- **Symmetries are free checks.** Permutation invariance (note 33 of `docs/knotapel.html`) and the AND/OR negation symmetry of Part E would each have flagged a fault at no cost.
- The ring arithmetic itself is sound wherever UBSan was quiet: Part A's 87,890 brackets agree exactly, and D56's float classification never disagreed with exact values.
