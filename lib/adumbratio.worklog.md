# adumbratio.c worklog

## 2026-11-05 — negotia-paginae T2: the mock-up format

Pipeline: `stml_legere` -> `stml_expandere` -> walk -> dispositio in
6x8 cells -> figura calls into `Mandata`; same list drawn in vicus and
written to PNG (`tabula_pixelorum_creare_nuda` + `delineare_mandata` +
`imago_png_scribere`).

- **Macros need the template sigil `#@`.** `<#@ordo t="@t">` /
  `<<#@ordo t="x">>`. Without `@` the expander treats the call as a
  CONTENT transclusion (alias, stml-macros-spec §2.0) and leaves it
  untouched - first run: nothing expanded, unknown fragments raised
  nothing. The renderer now names a leftover fragment or transclusion
  as an error ("templum: <<#@nomen>>"), never draws nothing. The
  approved header and the spec example said `<#ordo>`; corrected.
- **Group origin = fines + translatio** (`scaenam_impellere`). Passing
  the box position as BOTH placed the raw `<mandata>` contents twice
  as far and the clip cut them away; the unit test had asserted the
  translation, so it passed while nothing drew. Now fines = box,
  translatio = 0, and the test asserts exactly that. Caught only by
  the look check.
- Raw `<mandata>` uses mandata's own serialisation: text primitives
  carry their words in `textus="..."` (not element content). Colour
  NAMES are accepted there too (converted to theme slots before
  `mandata_legere_stml`).
- Leaves get fixed default sizes in cells (textus = runes x font width,
  titulus 8/6; globulum runes+2 x 2; sagitta 3x2; capsa 2x2); spatium
  and mandata grow. Text sits on cell boundaries (no +4px nudge as in
  hand-written panes) - honest to the grid.
- Errors are Latin "linea N: ..." (approved header); the error page
  shows "Mock-up error" + the message, wrapped.
- Plants (all red in their sections, restored IDEM): expansion skipped
  (fragmenta), double translation (mandata), error swallowed to an
  empty list (vitia).

## 2026-11-05 — T2 commit refused once; T3 embedding

- The radix gate refused the first T2 commit: `probatio_adumbratio`
  wrote `build/probatio_adumbratio.png` ("scripsit extra vestigium").
  Suites may only write inside their own area - `credo_area(c)` (a
  TMPDIR directory per section, deleted after). The PNG section now
  writes there.
- `adumbrationem_inserere(r, md, fines)` (T3) and the `<mandata>`
  escape hatch share one helper, `transcribere`: coetus with sectio,
  fines = box, translatio 0, elements copied. The copied elements keep
  chordae in r's arena, so r must outlive the paint (the negotia pane
  keeps r in its own cached arena).

## 2026-11-06 — labels centred in their rows (reverses the T2 grid choice)

- Fran: labels next to chips sat at the top of the row. Rows are
  `<linea altitudo="2">` (16px); the chip fills them, `<textus>` got a
  fixed 1-cell box and drew at its top. The T2 note above ("text sits
  on cell boundaries, no +4px nudge") was the cause - reversed.
- Now: a `<textus>` whose parent is `<linea>` or `<ordo>` defaults to
  altitudo CRESCENS with minimum = its fixed height (1, titulus 2), so
  it fills the row's height (dispositio: CRESCENS on the cross axis =
  parent's inner extent) and an apta row of text alone keeps height 1.
  Every textus is drawn at y + (box - fons_altitudo) / 2 - +4 for
  parvus in a 2-cell row, as the hand-written panes do. Explicit
  altitudo= still wins (read after the defaults).
- Only rows: a CRESCENS textus in a growing `<columna>` would share the
  leftover space with `<spatium/>` and drift down. The first test used
  a fit-height column, so that plant stayed GREEN; the test column is
  now crescens with a trailing spacer, like real mock-ups.
- Plants (5, all red, restored IDEM): no grow in rows, no centring,
  minimum dropped, bold path not centred, grow in columns too.
