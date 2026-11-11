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

## 2026-11-10 - the crop passes the renderer's coverage as a mask

`_exemplaria_intra_fines` now calls
`dithering_exemplaria_velox_cum_larva` with the cropped `tectum`.
Whole frame, 360x438: 20-33 ms (was 25-45 with the crop alone).
Numbers and what the mask does and does not skip are in
`lib/dithering_exemplaria_velox.worklog.md`. The "fines" section of
the suite compares against the same masked call made by hand.

## 2026-11-10 - a binding budget adds shimmer; the pane uses 45 %

Fran, watching the masked build: "there might be more shimmer after
the mask, maybe because the mask is creating more edges somehow?"
Measured instead of guessed (scratch `stabilitas.c`: share of the
model's pixels whose colour index changes between consecutive frames,
30 frames, 360x438; left three columns = model still, view turning
0.3 degrees a frame; right three = walking, view still):

| version | golem | mushroom | dragon | golem | mushroom | dragon |
|---|---|---|---|---|---|---|
| nearest colour, no dither | 2.7 | 1.8 | 6.0 | 20.8 | 20.2 | 14.6 |
| whole frame (first watched) | 13.3 | 11.7 | 21.6 | 35.0 | 38.0 | 33.4 |
| crop (committed) | 13.4 | 11.5 | 21.9 | 35.3 | 38.7 | 34.2 |
| mask, base + final only | 13.5 | 11.7 | 20.5 | 34.8 | 38.3 | 32.8 |
| mask + nothing on paper, box budget | 13.7 | 11.7 | 21.1 | 35.0 | 38.4 | 33.3 |
| budgets 15 / 30 % of the model | 14.5 | 13.2 | 20.8 | 35.9 | 39.4 | 36.9 |
| budgets 45 / 90 % of the model | 13.7 | 11.7 | 21.1 | 35.0 | 38.4 | 33.3 |

- The mask itself adds nothing: crop, mask and the restrictions are
  within 0.3 points of the whole frame (the dragon giraffe is even a
  point calmer with the final stage masked).
- The budget is what adds shimmer, once it binds. Regions are taken
  largest first and skipped when they do not fit, so near the limit a
  region is hatched in one frame and not in the next - a whole patch
  changes at once, which reads worse than the 1-3 points suggest.
  Before today the budget never bound (hatching reached 12 / 24 / 39 %
  of the three models, unlimited in effect).
- The pane now passes 45 % for both budgets
  (`STATUAE_PRAESUPPUTATIO_*`): above anything these models reach
  (hatching up to 39 %, pictographs up to 34 %), so nothing binds and
  the numbers return to the unlimited row. Frame time is unchanged
  from the 15 / 30 % build: 18.6 / 23.2 / 30 / 22 ms (golem clip 0,
  clip 1, mushroom man, dragon giraffe).
- If less hatching is wanted, the stable knob is which pixels are
  candidates (the energy / coherence thresholds in exemplaria), not
  the budget.

## 2026-11-10 - hatching budget 25 % of the model (Fran's try)

Fran asked to lower the hatching budget a little to see it. Set to
25 % (pictographs stay 45 %). Unlimited hatching was 12 / 25 / 39 %
(golem / mushroom man / dragon giraffe), so only the dragon giraffe
is cut; the mushroom man sits right at the limit. Flips between
frames, dragon giraffe: 21.1 -> 22.5 % turning, 33.3 -> 34.8 %
walking; the other two unchanged. At 20 % the mushroom man starts to
pay too. Fran: "these changes are good, lets commit them for now."

Outline against interior (band within 3 pixels of the outline, same
motion): the outline flips about twice as often as the interior in
every version (golem 25 % against 11 %). Masking the final stage
leaves the interior identical and makes the outline calmer when
walking (52.7 -> 49.6, 55.8 -> 52.6, 45.4 -> 41.5 %); standing, the
dragon giraffe is calmer (30.0 -> 26.4), the mushroom man slightly
worse (15.6 -> 16.8). Fran's guess that Atkinson is more volatile
under a mask is not what the numbers show.


## 2026-11-10 - statuae_picturam_dithering (mundus-plan T7)

The exemplaria path of this file (crop to the mask's box with margin
and 8-pixel snapping, mask, budgets as a share of the masked pixels)
is now a public function that takes a picture and a mask the caller
rendered and writes palette INDICES only where the mask is set.
`_exemplaria_intra_fines` became a thin caller of it; the pinned
digests of `statuae_quadrum_pingere` and the "fines" comparison did
not move. The world's frame uses it for each body with the mask
"pixels of this body that nothing nearer covers". Test section
"larva_picturae"; plant (write outside the mask) red.
