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
