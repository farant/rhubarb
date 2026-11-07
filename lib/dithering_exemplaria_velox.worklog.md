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
