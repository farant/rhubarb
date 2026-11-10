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

## 2026-11-10 - exemplaria only inside the model's box

Fran found exemplaria "interesting in motion", shimmer tolerable, the
frame rate the distracting part. The pane handed exemplaria the whole
frame although the model covers only 10-20 % of it.

- `statuae_fines_tecti`: smallest rectangle around the covered
  pixels, 8-pixel margin, corners snapped outward to an 8-pixel grid,
  clamped to the picture. `_exemplaria_intra_fines` copies that
  rectangle out, dithers it, puts it back; the paper pass that already
  existed cleans everything uncovered. Other modes untouched.
- The snap is not optional. Exemplaria ties its patterns to an
  8-pixel grid: unsnapped (margin 3), 37 % of covered pixels differ
  from the full-frame dither; snapped to 8 or to 16, 6-7 %.
- The cropped result is NOT the full-frame result: 6-17 % of covered
  pixels differ (golem 7 %, mushroom man 12 %, dragon giraffe 17 %),
  because exemplaria looks at the whole image when choosing. Side by
  side the stills are indistinguishable. Whether a box that changes
  size adds shimmer in motion is for Fran's eye.
- Measured, whole frame through `statuae_quadrum_pingere` (pose +
  render + dither), 20 frames, model turning 3 degrees a frame, -O2:

  | model, clip | size | before | after |
  |---|---|---|---|
  | golem, clip 0 | 360x438 | ~63 ms | 25.5 ms |
  | golem, clip 1 | 360x438 | ~65 ms | 32.7 ms |
  | dragon giraffe, walk | 360x438 | ~68 ms | 37.6 ms |
  | mushroom man, clip 0 | 360x438 | ~67 ms | 44.5 ms |
  | golem, clip 0 | 480x541 | ~100 ms | 39.1 ms |
  | mushroom man, clip 0 | 480x541 | ~106 ms | 71.5 ms |

  ("before" is the dither alone on the full frame, from the scratch
  comparison; add under 1 ms for pose and render.) The gain is 1.5x
  to 2.6x, less than hoped: a box around outstretched limbs is still
  mostly empty (box 25-56 % of the frame). 30 frames a second needs
  about 30 ms; only the golem's first clip at 360 wide gets there.
- Next lever, not built: exemplaria skipping uncovered pixels itself
  (a mask through its stages). From the per-pixel cost, an estimate
  of 10-20 ms at 360 wide; unmeasured.
- Plants ("fines" red each time): left edge not snapped; crop read
  one pixel to the right; whole frame dithered in place of the box.

