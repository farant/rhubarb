# statuae_quadrum worklog

## 2026-11-09 - T6 of project-specs/statuae-plan.md

- **Why this file exists** (not in the plan): `bin/statuae pingere`
  and the pane both need "model + clip time + view + dither mode ->
  finished picture". In the tool alone it would be duplicated in the
  pane and tested by nothing; here it is one function with a suite,
  and the tool stays arguments and output (the negotia pattern).
- Pipeline: `statua_positura` -> `statua_pingere` -> dither by mode ->
  `dithering_indices_ad_rgba` -> every pixel the renderer did not
  cover becomes paper (palette 5, Albus), opaque or alpha 0. The
  paper pass is what keeps Atkinson's diffusion from speckling the
  background; the suite checks it pixel for pixel against the
  renderer's own coverage mask, in every mode.
- `STATUAE_MODUS_CRUDUS` (undithered) is for the tool only.
- Every mode survives pictures down to 1x1 (CREDO_NON_RUIT),
  including the fast exemplaria.
- The tool (`tools/statuae.c`) is checked by `tools/statuae_fumus.sh`
  (hand-run, temporary volume only): usage errors exit 2, refusals
  exit 1 and leave the volume untouched, every `pingere` mode writes
  a 360 x 450 PNG. No suite launches the binary; none in the house
  does.
- `-volumen` has no default, unlike `bin/negotia`.
- First look at exemplaria on a rendered model (mushroom man): brick
  on the cap, scales on the jacket. Whether it holds still in motion
  is T8/T9's question.
- Renderer digests in `probatio_statua_pictura.c` section "fixa" were
  pinned in this task, after Fran had seen the frames.
- Plants: paper pass skipped for some pixels, and transparency
  ignored -> "quadrum" red both times.
