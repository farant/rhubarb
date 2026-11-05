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
