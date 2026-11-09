# dithering_exemplaria_velox worklog

## 2026-11-07 — T1: harness, fast = copy

Plan: `project-specs/exemplaria-velox-plan.md` (spec
`project-specs/exemplaria-velox.md`).

- `lib/dithering_exemplaria_velox.c` is classic's file with the public
  name changed; `dithering_exemplaria_optiones_ordinariae` is classic's
  (not copied), so the default material table has one home.
- Classic's ten synthetic sections now live in
  `probationes/dithering_exemplaria_commune.h`; the function under test
  comes through the credo suite fixture (`c->suita` ->
  `DitheringExemplariaProbandum`, a struct because C89 can't cast a
  function pointer to `vacuum*`). Both suites list them with
  `DITHERING_EXEMPLARIA_SECTIONES_COMMUNES`. Classic suite: 56 -> 57
  assertions (the new `CREDO_NECESSE` in parare_suitae), same sections.
- Classic pinned on the two fixtures (blake2b-256 of the indices,
  default options, all colours) in the velox suite, "classica fixa".
  Taken before anything moved. The fixture runs classic once per suite
  and the "par classicae" section compares the copy against that.
- **Memory finding:** one classic call at 600x751 uses ~247 MB of arena
  (360x451: 89, 720x901: 356, meadow 600x800: 260). "par classicae"
  runs two such calls in one section (507 MiB) and needed the "gravis"
  sign (1 GiB roof). Task 3's "memoria minor" is where this must drop.
- **Plant note:** `CANDIDATI_MAX` XII -> XI changed nothing: at most 10
  candidates are ever gathered (6 nearest the area mean + 2 nearest each
  extreme), so the limit is unreachable — a silent plant, not a test
  hole. Replaced by `POENA_CONTRASTUS` 0.004 -> 0.0041 (red). The other
  plants: classic `POENA_CONTRASTUS` -> "classica fixa" red; alpha read
  for blue -> "alpha neglecta" (and "lateres") red; `+ rand()` on the
  seed -> "sine memoria inter vocationes" (and "determinatum") red.
- `tools/exemplaria_mensura.c` (aedilis, like xar_mensura): fresh arena
  per call, timing of the call only. Baseline at T1, N = 10:

  | imago | magnitudo | atkinson | classica | velox | arena c/v MB |
  |---|---|---|---|---|---|
  | lapis | 360x451 | 3.5 ms | 241 ms | 238 ms | 89/89 |
  | lapis | 600x751 | 10.0 ms | 619 ms | 590 ms | 247/247 |
  | lapis | 720x901 | 13.7 ms | 847 ms | 867 ms | 356/356 |
  | pratum | 600x800 | 10.6 ms | 685 ms | 678 ms | 260/260 |

  Differentia 0.0 % on both fixtures (copy).

## 2026-11-07 — T2: shared pattern sources + build-time tables

- Patterns moved to `lib/dithering_exemplaria_fontes.c` (one home for
  classic, velox and the generator). "classica fixa" stayed green, so
  the move didn't change classic's output.
- `tools/dithering_tabulae_generare.{sh,c}` (aedilis-built, fabrica
  generator action `dithering_exemplaria_tabulae`) writes
  `lib/dithering_exemplaria_tabulae.c`: Bayer/pictogram orders,
  densities, both weight tables (same loop as classic's
  `_exemplar_finire`), and the 256-entry sRGB->linear table as f64
  `%.17g` (exact round trip). Velox copies its `Exemplar`s from the
  tables (no `exp`) and converts sRGB by table. "par classicae" stayed
  green: the tables equal classic's numbers bit for bit.
- **Build-freshness trap (again):** the first plants on the generator
  stayed green / stayed red after restore - aedilis' struere.sh judges
  by `-nt` in whole seconds, so a source planted and restored within a
  second kept the planted object (memory plant-restore-mtime). The
  generator script now deletes its two covered objects
  (`tools__dithering_tabulae_generare.o`,
  `lib__dithering_exemplaria_fontes.o`) before building. Plants then:
  Bayer entry 32 -> 31 -> "classica fixa" + "par classicae" red, and
  `-probare` RANCIDA; generator sigma 1.0 -> 1.1 -> `-probare` RANCIDA.
- **Finding, not changed:** `color_octetus_ad_linearem` (lib/color.c)
  keeps a lazy `hic_manens` sRGB table - a cache by Fran's rule. Classic
  and many others call it, so it stays for now; velox no longer uses it.
  Candidate: replace with the generated table.
- No speed change (N = 10): lapis 600 classic 601 ms, velox 600 ms;
  meadow 709 / 688. The `exp` work was a few ms and sRGB was already
  table-backed. Differentia still 0.0 %. Big wins start in T3.

## 2026-11-07 — T3: f32 planes, bitsets, cube root without pow

- Every per-pixel buffer is now an `f32` plane per channel (`Plana`,
  `Structura`); masks are `i32`-word bitsets (`_bit`, `_bit_ponere`);
  `Opus` carries `larva_activorum` and builds `mixtio` (f32) only for
  active pairs. OKLab is `_oklab` in f32 with `_radix_cubica`: bit
  guess (`bits / 3 + 709921077`) + two Newton steps. Measured max
  relative error 1.7e-6 on [1e-6, 4]; only subnormals (< 1.2e-38) are
  off, and their cube root (~1e-13) can't move a colour.
- The source image's OKLab is only read for L (structure tensor), so
  only an L plane (`lumen`) is kept. `_gauss_planum` is per plane and
  may write in place (`meta == fons`): the horizontal pass reads the
  whole source into `tmp` first - the structure tensor and aspersio
  blur in place. Region side arrays (`initium`, `nm`, `RegioOrdo`)
  are sized `n / area_min + 1`, not `n`.
- sRGB table is f32 (`%#.9gf` - the `#` keeps "0." so `0f` never
  appears).
- Tests: "par classicae" retired; "colores puri" (16 flat images ->
  own index; passed on the copy, a guard) and "memoria minor" (gravis;
  FRACTA on the copy at 247 = 247). Plants: a into the b plane ->
  "colores puri" red; `_bit` `p >> VI` -> "determinatum" + "alpha
  neglecta" red. Why "determinatum": with wrong bits, aspersio reads
  `atramentum`/`charta` where nothing was written (uninitialised arena
  bytes) - only in the planted state; normally they're read only under
  `larva`.
- Drift from classic: 2 px on the golem, 14 px on the meadow (the
  tool's differentia was `%.1f` and showed 0.0 %; now `%.2f` + pixel
  count). Memory 247 -> 81 MB (golem 600). Speed only ~1.1x - expected:
  blur, the base pair search and the insertion-sort nearest colours
  are T4-T6.

  | imago | magnitudo | atkinson | classica | velox | arena c/v MB |
  |---|---|---|---|---|---|
  | lapis | 360x451 | 3.8 ms | 233 ms | 210 ms | 89/29 |
  | lapis | 600x751 | 9.5 ms | 589 ms | 517 ms | 247/81 |
  | lapis | 720x901 | 13.4 ms | 843 ms | 736 ms | 356/116 |
  | pratum | 600x800 | 10.2 ms | 652 ms | 594 ms | 260/84 |

- Formatter: the copy carried classic's 92-ish report-only notes
  (60-char banners, includes after system headers, long lines); the
  rewritten file is CONFORMIS.

## 2026-11-07 — T4: blur split + "velox fixa" pins

- `_gauss_planum`: horizontal pass split into left edge / interior
  [radius, lat - radius) / right edge (`_gauss_orae` clamps); when
  lat < 2 radius + 1 everything is edge. Vertical pass needs no split:
  the clamp depends only on the row, so it is hoisted to the row and
  whole rows are summed (`exitus[x] += pondus * ordo[x]`, k outer) -
  row-major instead of column walks.
- **Exact refactor:** per-pixel summation order unchanged (start 0.0f,
  k ascending), so the output is bit-identical to T3. Pinned before
  the change (Fran approved, an addition to the plan): "velox fixa"
  (blake2b of velox indices on both fixtures, T3 commit 6cc3089d5) and
  "magnitudines oris" (12x12, 13x13, 25x3, 3x25, 601x2 scaena, grey
  palette: success + active colours + digest). The plan's version
  (success + active only) would have stayed green on its own second
  plant: one-too-wide interior reads the next row's first pixel, a
  small wrong value, not a crash. Re-pin only when a task means to
  drift, and say why.
- Plants: kernel not normalised (`/ (0.0 * summa + 1.0)` - dropping the
  divide entirely left `summa` unused, -Werror refused the plant) -> 7
  sections red incl. "colores puri" and both pins; `finis = lat -
  radius + I` -> "velox fixa", "magnitudines oris", "determinatum" red.
- Timing (N = 10): lapis 600 classic 618 / velox 488 ms (T3 517); meadow
  646 / 490 (T3 594); lapis 720 870 / 632. Drift still 2 / 14 px.

## 2026-11-07 — T5: nearest colours + base pair loop (no packed key)

- Profiled first (scratch build with `-fno-inline`, `sample`; under -O2
  the static helpers inline into the public function and `sample`
  credits them to wrong lines). Golem + meadow: base 54% (pair loop
  ~47%, `_proximi` 7%), `_proximus` **20%** (residual 11% + pictograms
  9%) - it ran the full 16-colour insertion sort to take the first.
- `_proximus` is now a plain minimum; `_proximi_k` is partial selection
  (k passes of "first minimum among the rest"), writing only min(k,
  active) entries. Ties -> lower index in both, as the stable insertion
  sort gave. Base candidates go straight into `cand`; hatching and
  pictogram arrays shrank to VIII / IX.
- Base reads `DITHERING_BAYER_{DENSITATES,PONDERA,ORDINES}` directly;
  the `bayer` Exemplar copy in the main function is gone. Exact
  pruning: level 0 (density 0, all weights 0) is solid a for every b,
  scored once per a at its first place; levels 1..15 of a pair are
  skipped (`frange`) once `poena_minima` (penalty at the smallest
  nonzero minor, computed once per pair) is >= the best. Nothing
  skipped could be strictly better, so the choice can't change. The
  penalty keeps classic's operation order (`poena * minor / 32.0`).
- **Packed key dropped (plan said `a | b << IV | gradus << VIII`).**
  Back-to-back A/B in scratch binaries: task 5 as first written was
  *slower* than T4 on the golem (438 -> 460 ms). Bisected from "T4 +
  new nearest functions only" (E: 433 -> 354): weights-from-table and
  pair pointer neutral, skip rules removed still slow, the packed key
  alone +33% overall; `sample` put it all in the base (4848 -> 7765
  samples, +60%). Not chased into the asm (all inlined into one
  1600-line function); three separate `opt_a/opt_b/opt_e` avoid it.
  Lesson: time each step against the previous commit in the same
  session - single runs drift by ~5% and the 720 golem looked "slower"
  for noise reasons before this.
- "pauci colores" (scaena 80x60, active {V}, {0,V}, {0,V,VI}: only
  active colours; flat colours inside each set exact) - green on T4, a
  guard. Pins "velox fixa" / "magnitudines oris" held: bit-identical.
- Plants: `_proximus` tie `<` -> `<=` stayed GREEN (no exact
  equidistant pixel in any fixture - the tie rule is unpinned; noted,
  not chased); replaced by `_proximus` loop from i = 2 (skips the
  second active colour) -> velox fixa + magnitudines oris red ("colores
  puri" stays green: on a flat image the residual threshold keeps the
  base colour). Pair skip inverted -> both pins red. `_proximi_k`
  writes the position instead of `activi[position]` -> "colores
  activi", "pauci colores", "magnitudines oris" red; "velox fixa" green
  (all 16 active, position == index) - the case pauci colores exists
  for.
- Timing back to back (velox ms; T4 / T5): golem 360 175 / 135, golem
  600 435 / 328, golem 720 623 / 468, meadow 494 / 344. Classic ~580 /
  650: velox now ~1.8x classic. After-profile (no-inline): base 56%
  (still the pair loop), `_campum_striarum` 10%, residual 9%
  (`_proximus` 5%), structure 7% (atan2 3%) - T6 has the hatching
  field.

## 2026-11-07 — T6: hatching field, unused angle, census

- Most of the plan's T6 had already landed (`_proximus` in T5, f32
  planes and bitsets in T3). Left: `_campum_striarum` (~10%) and
  `atan2` (~3%).
- `_campum_striarum`: each active pixel's kernel index (from
  `st_campi.angulus`) is computed once into an `i8` plane instead of
  every iteration; the 13x13 convolution skips the column clamps when
  all taps are inside the row (x in [radius, lat - radius)); `tanh` /
  normalise only on active pixels (inactive: g = 0, tanh(0) = 0, so the
  0.0f they start with is already right). Summation order unchanged.
- `_structuram_computare` skips the angle when `st->angulus == NIHIL`;
  the detail tensor passes NIHIL (nobody reads `st.angulus`) and loses
  that plane (arena 81 -> 79 MB).
- Tests: "census in fixis" (plan; green on T5 as a guard: golem
  hatching 0.023, pictograms 0.289 / 101 regions, aspersio 0.029;
  meadow 0.000 / 0.299 / 40 / 0.075). **Added "hachurae fixae"** (not in
  the header): the third plant (field interior one column too wide -
  reads one past the row, past the plane on the last row) stayed GREEN,
  because fixture hatching never reaches the right edge. The shared
  "hachurae" scene (128x128, budget 0.50) has a stripe band across the
  full width (hatching 0.358); its velox digest was taken by running
  the suite on the T5 commit's source (swapped in, restored, cmp) and
  matched on T6. The plant is then red on "hachurae fixae".
- Plants: iterations VIII -> 0 -> "velox fixa" red ("hachurae" itself
  stays green - it only checks that something hatched); residual
  `_proximus` -> first active colour -> "velox fixa" + "magnitudines
  oris" red; interior one column wider -> "hachurae fixae" red.
- Timing, four back-to-back pairs (velox ms, T5 / T6): golem 360 137 /
  130, golem 600 338 / 324, golem 720 478 / 452, meadow 353 / 341 - 4-5%.
  The field only dropped 10% -> 9% (no-inline): the clamps weren't its
  cost, the 169 taps x 8 iterations over active pixels are. Structure
  7.2% -> 5.4%. Base now 58%. Velox ~1.9x classic.

## 2026-11-07 — T7: exact speedups from a fresh-eyes review (-43%)

Fran asked for a fresh pair of eyes. A read-only subagent (scratch
builds only) counted the base loop and prototyped changes; I measured in
parallel. Counts per 2x2 area (golem 600): 6 candidates (the 2+2
"extremes" almost never add one), 30 pairs, ~454 levels entered, 73% of
levels stop after one pixel, the `poena_minima` pair skip fires on 0.8%
of pairs. The cost is ~51M tiny level evaluations per frame (~3.7 ns
each), not distance math. My own triangle bound (ball around a / b)
skipped 52% of pairs but gained only 9% (superseded). The agent's v6
(scratch) reproduced at -44% with identical bytes; T7 is that, rewritten
in house style, step by step with A/B after each (ms golem 600 / meadow):

- **A. Skip covered base areas** (T6 316 / 331 -> 254 / 270, -20%). The
  base runs after layers II and III and skips 2x2 areas wholly in
  `tectum = pictus | larva`. `pictus` (new, set by `_hachuras_pingere`)
  is "really painted": a hatched 4x4 with no qualifying pair keeps the
  base, so `hachuratus` would be wrong. Both `memcpy(pictura_*, basis)`
  gone; compositio takes the hatching colour under `pictus`, the
  hatching threshold stays under `hachuratus` (oddity kept, see below).
  Readers checked: `pictura_hachurarum` is written exactly at `pictus`,
  `pictura_stili` exactly at `larva`; aspersio and the residual read
  `basis` only after compositio.
- **B. Capsule bound per pair** (-> 217 / 207). Per active pair
  (`_capsulam_parare`, in `Opus`, ~16 KB f64): segment P = mix(w 0) ->
  mix(w 32), rho = max distance of the 33 stored f32 mixes from it x (1
  + 1e-6) + 1e-6. Distance to the capsule is convex, so for every level
  `err_e >= pen_e + area * (dist(mean, segment) - rho)_+^2`.
  `_limites_parium` computes all cand x cand bounds per area, branch-
  free; a pair is skipped when `pen_min * (1 - 1e-12) + LB * (1 - 1e-5)
  >= optimum`. Margins: f32 d2 >= true * (1 - 3e-7); the 1e-12 on
  pen_min closes a theoretical f64 rounding gap the agent found in its
  own first version.
- **C. First-term sweep** (-> 219 / 207, only -2..3%). Level 0 out of
  the loop (once per a, at its first place); `post_primum[e] = pen_e +
  d2(pixel 0)` for levels 1..15 with no branches, the same f64 sum the
  loop made, then only levels with `post_primum < optimum` are walked
  from q = 1. The agent measured -12% before B; B leaves it little.
- **D. Horizontal blur interior row by row** (-> 198 / 183, -7%): k
  outer, x inner, per-pixel order unchanged (0, k ascending);
  vectorises. Safe in place: the pass reads `fons`, writes `tmp`.
- **E. Hatching field, 4 interior pixels at once** (-> 184 / 186, -7%
  golem; meadow has no hatching). `_campi_quattuor` (4 independent sums,
  own kernel and order each) and `_campi_summa` (clamped single, edges
  and tails). **My first port was 70% slower** on the golem: I'd left out
  the agent's "no pixel of the four active -> skip" early-out, so it
  convolved every inactive pixel.

- New pin "tecta fixa" (scaena 128, both budgets 1.0; hatching 0.358,
  pictograms 0.468), taken on T6 c1c6498f9 before any change.
- Plants: A (area skipped when only the first pixel is covered) -> 7
  sections red; B (rho dropped) -> velox fixa; C (first term uses the
  previous level's weight) -> 4 pins; D (interior seeded with the pixel
  instead of 0) -> 6 incl. colores puri, lateres. E as first planted
  ("+ I" on the right group limit) was a **silent plant**: groups start
  at 6 and step 4, and at widths 128 and 600 the last group already
  ends where the old condition allows, so the planted code never ran.
  Replaced by the left edge (groups may start at radius - 1) -> hachurae
  fixae + tecta fixa red.
- Back to back, 3 pairs (T6 / T7): golem 360 129 / 74, golem 600 313 /
  179, golem 720 446 / 255, meadow 331 / 181. Velox 3.3x classic. After
  profile (no-inline): base 37%, residual 17%, field 10%, pictograms 7%,
  structure 7%, aspersio 6%.
- **Oddity kept (inherited, would drift):** a hatched 4x4 with no
  qualifying pair keeps its base colours but gets `HACHURAE_LIMEN` 0.30
  in the residual.

Unmeasured ideas from the review (each ~1-3%, all exact), for later:
- hatching angle only at `activus` pixels and only when `regiones_h >
  0` (meadow then never computes it);
- the two `_structuram_computare` calls build identical gradient
  planes: build once, blur copies;
- `_regiones_stilizare`: `m / lat`, `m % lat` per member per pair per
  pattern - precompute member positions per region;
- `_regiones_colligere`: push x, y instead of div/mod per popped pixel;
- `_residuum`: skip both sqrts when `proximus == b` (limina >= 0, so the
  answer is b anyway);
- `_aspergere`: `_oklab` on every pixel but read only under `larva`;
- eye-image OKLab only for areas not skipped;
- `_proximi_k` branch-free rank counting; one distance pass for mean
  and both extremes;
- base: a per-pixel capsule bound (on golem 42-46% of pairs that pass
  the mean bound still die in the sweep);
- base: seed `optimum` with an upper bound U (accept the first err <=
  U, then strict <) - identical result;
- `CANDIDATI_MAX` XII is never reached (max 10).

## 2026-11-07 — tried, dropped: identical base blocks within a frame

Fran asked whether in-frame repeats could be skipped (low-poly renders
have flat faces). Counted in scratch: a base block's full input (4 eye
pixels, 4 linear pixels, 8x8 phase, size) repeats bit-for-bit in 38-40%
of the computed blocks on the golem, 0% on the meadow photo. An exact
prototype (hash + full-key memcmp, result reused) kept the bytes
identical but gained only 3-4% on the golem and cost 7% on the meadow:
the repeats are flat background, already the cheapest blocks (the
bounds end them almost at once), and photos pay for the hashing. Also
leans on the spec's anti-goals (no memoization; no golem-only wins).
Not adopted. A lower render resolution for the 3D pane (Fran is open to
"a short hike" style) is the bigger lever: 360x451 already runs at ~74
ms (~13.5 fps).

## 2026-11-07 — tried, dropped: seeded optimum in the base

Exact scheme (scratch): seed = the pair with the smallest capsule bound,
at the level nearest its projection t * 16 (clamped 1..15), err summed
exactly as the loop would; scan starts at U = min(solid, seed), accepts
the first err <= U, then strict < (pruning uses <= until then; the
capsule test is strict, so unchanged). Bytes identical. Work: levels
walked per area golem 21.4 -> 18.0, meadow 46.1 -> 30.6; the seed beats
the solid start in 47% / 98% of areas. Time: no change (within noise).
After T7 the base is mostly fixed per-area work - three `_proximi_k`
calls, 36 capsule distances (sqrt each), the 15-level first-term sweep
per surviving pair (~7.5 pairs) - which the seed doesn't touch, and the
seed pays for one full evaluation. Not adopted. Further base gains need
that fixed cost cut (fewer candidates would drift; e.g. a cheaper
capsule test or sweeping only surviving pairs' plausible levels).

## 2026-11-07 — T8: verdict and docs

Final run (N 30, ms): golem 360 76, golem 600 185, golem 720 258, meadow
193; classic 226 / 581 / 840 / 663 -> 3.0-3.4x. The spec's 12 fps target
at 600x751 (<= 83 ms) is missed (5.4 fps); 360x451 meets it (13 fps).
Fran is open to a lower 3D render resolution ("a short hike" style,
shown at 2x), which is the path for trying exemplaria in motion.
Photos-vs-golem speed-up condition met (3.4x vs 3.1x). The spec has a
dated "VIII. Result" section (table, verdict, per-task progression,
departures, tried-and-dropped, what's left); the public header carries
the table. Tabularium wishes: pictor paste B -> velox (after Fran's look
verdict on the `_par.png` images); the remaining 1-3% exact list.
