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
