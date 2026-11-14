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

## 2026-11-13 — hosted form (ostium-plan T2)

Another pane (the world) can now show a scene in its own ramus:
`scaena_latus_montare_hospitem`, `_agere_hospitis`,
`_pingere_hospitis`, `_areas_addere`, `_valet`. No second copy of
the drawing or input code: the pane's own `agere` and figure are thin
wrappers over `eventum_tractare` and `latus_pingere`, and the hosted
entry points call the same two.

Three places branch on `m->hospes`:

- `locus_prospectus`: an unknown `prospectus` is -1, never the first
  view. `prospectum_ignotum_curare` then sets the reason. To keep the
  1-second pulse from reporting a change every time, the key
  `"?<title>"` is stored in `sigillum_lecti` (a real seal is hex, so
  it cannot collide).
- `rem_tangere`: a `mundus` exit writes nothing and sets `m->foras`;
  `_agere_hospitis` clears it before the event and turns it into
  `SCAENA_LATUS_FORAS` after. The flag is needed because Enter
  reaches `rem_tangere` through the command table, whose functions
  take only the context.
- `eventum_tractare`: the `[` `]` commands are skipped.

`disponere` computes picture and text position first and only then
branches for the header, so those two lines exist once (a plant
anchor from T8 went ambiguous when the hosted branch duplicated
them).

**Test host.** The hosted sections build a small host inside the
test: its own action registry, compositor and figure, on a ramus the
real pane prepared. The dispatcher can deliver further events after
the one sent (they change nothing), so the host keeps the last
result that was not NIHIL and each NIHIL expectation clears it
first. Counters live in the `Orbis` struct, not in file statics:
examen refuses statics written in one section and never reset.

The writer name is honoured: a host passing an action the manifest
does not name cannot write the fields (`hospes_scriptor`).

## 2026-11-14 — wider picture; narrow panes show the middle

Picture is 480 x 320 (constants from graphis.h; default surface 480
x 576). `Dispositio.sectio_x` is the first picture column shown: a
pane narrower than the picture cuts it equally on both sides, where
it used to show the left part. The cut is applied in two places that
must agree: `quadrum_curare` (pixels) and `res_in_puncto` (which
thing is under the mouse). Things in the cut part are out of the
mouse's reach; keys still reach them (tested with the hut's "turn"
arrow in a pane 200 wide).

## 2026-11-14 — picture inside the border

Fran, in the real window: the 480 picture in a 480 pane covered the
focus border on both sides ("widescreen"). The picture's visible
width is now the pane's width minus the margin on each side, so a
pane of 480 shows 468 columns (6 cut from each side by the same
centred cut) and the border stays whole. A pane of 492 or more shows
the whole picture. Test helpers now take PICTURE columns and subtract
the cut; a mouse move that starts in a cut column lands outside the
picture area, so `_pixelum` skips the first and last seven columns.
