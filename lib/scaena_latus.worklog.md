# scaena_latus worklog

## 2026-11-13 - first build (graphis-plan T8)

- Modelled on `lib/mundus_latus.c`: root `scaena` (PARTES_TABULA, one
  figure draws everything) with hit areas `pictura`,
  `sagitta_prior`, `sagitta_proxima`. No clock port: the pulse just
  compares `volumen_seq_ultimum` once a second.
- Everything that depends on the shown view lives in
  `piscina_prospectus` (the read view, the RGBA picture, the touched
  thing's text and its key). `prospectum_tollere` empties it and
  resets all of them together; a view is re-read when the sigillum
  of the shown title changes, so `bin/scaena condere` shows up
  without a restart.
- Status field `tactum` is `<view>/<thing>`. The view part matters:
  after an exit the shown view is the destination, but the line on
  screen belongs to the view you left ("You turn your back to the
  fire."). `tactum_curare` reads that other view's blob once to get
  the text. View titles cannot contain `/`, so the first `/` splits.
- The picture is built at the size of its visible part (pane
  narrower than 360 or shorter than 252): the image never extends
  past the pane, and a pointer position inside the `pictura` area
  is a picture pixel directly (`des->punctum_locale`, no offset).
- Mouse motion: the dispatcher sends EXIIT to the old area, INTRAVIT
  to the new, then MOTUS to the target. MOTUS inside `pictura` sets
  the pointed thing; MOTUS anywhere else and EXIIT clear it;
  INTRAVIT is ignored.
- The description was first drawn in COLOR_TEXT_DIM; on the pane's
  dithered background it was hard to read (seen in a headless
  screenshot). Both texts are COLOR_TEXT now, separated by a blank
  line.
- `Mora` comes from `fasti.h`; the header needs it.
- Screenshot helper (not in git): a copy of the vicus headless
  recipe in `build/iactura/scaena_imago.c`. `bin/aedilis` refuses a
  source file outside the tree ("plagula imparsabilis"), so
  throwaway tools must sit under the worktree.
- `ludus_tessera/compile_probationes.sh` keeps a hand list of the
  libraries vicus links; `scaena_latus`, `scaena_repositorium`,
  `scaena`, `graphis`, `graphis_lumen` were added there.

## 2026-11-13 - layout tweaks from Fran's first look

- `figura_mensam_pingere` paints the focus texture over the WHOLE
  pane when it has focus. Like negotia and chronica, the pane now
  fills a flat COLOR_BACKGROUND rectangle inside a one-cell margin
  (6 x 8), so the texture is only a border. Header, arrows and
  picture moved down by the margin (picture at y = 20).
- Text starts one line (10 px) under the picture, two cells in from
  the background's sides (x = 18), and stops at the bottom margin.
- A plant that let text run into the bottom margin stayed green:
  the short-pane test only looked for a fragment of the last line.
  It now counts the lines in the band and checks each one's box.
