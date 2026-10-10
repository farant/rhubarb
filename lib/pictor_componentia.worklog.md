# pictor_componentia.worklog.md

## 2026-09-05 — natus (ludus P3 T7)

Pictor's `componere`: four nodes of ONE record type, roles as data
(brainstorm §XVI §5). `radix` carries the root action
(`instrumentum.eligere`, where hotkeys bubble), `prospectus` is the
viewport (clips; translatio = the pan from `Motus`; scala = the
zoom from the ephemera island, floored at I), `tabula` is the
canvas (document size from the durabilis island, PROPRIUS,
focusable), `status` is a `PARTES_TITULUS` strip whose title is the
current tool. No per-componens struct, no function table: the
figura registry (T2) keys on the role.

Two decisions live here. The tabula's ACTION IS CHOSEN AT COMPOSE
TIME from `ephemera.instrumentum` (`pictor_actio_instrumenti`:
"penicillus" → "penicillus.ictus", unknown → none), so handlers
never branch on the tool — the name is a pure function of state
(spec §5.1). And the pending stroke is COPIED from `Motus` into
`tabula.puncta` (the field the polygon predicate already
serializes), so `pingere` and every figura read the tree alone
(decision 2026-09-05); the tier-3 snapshot therefore shows a stroke
in flight. `puncta` is assigned directly — `Componens` is a plain
record and has no setter; the dual use (polygon hit shape on a
non-PROPRIUS node, pending stroke on the PROPRIUS tabula) is named
here rather than given a second field.

The viewport size comes from the ctx (`PictorCompositio`); the
document size and the zoom from the islands. P4 hangs the palettes
as children of `radix` before `status`.

Gate: `probatio_pictor_componentia` (40); exemplar
`probationes/pictor/pictor.arbor.stml` born on the first run, read,
promoted — it shows the stroke in flight (`puncta="10,20 12,22"`)
and the pan as `translatio_x="-5"`, the signed serializer at work.
Planted fault: the stroke copy skipped — red at `numerus_punctorum`
and at the exemplar, green on revert. Examen ACCIPE; formator 0.

## 2026-10-08 - canvas one cell in from the prospect (border visible, like scriba)

Fran: no border around the pictor canvas in vicus. The border existed
(figura_tabulae draws the page margin one whole CELL outside the
document) but S2c sized new canvases to fill the whole prospect, so the
margin lay outside the view. Now the canvas node sits at (cw, ch) in
the prospect (like scriba's page at its margin cell) and new canvases
are the prospect minus one cell on each side (pictor_applicatio). The
margin cell and border show on all sides in a fitting pane.

Goldens moved with it, each inspected before promotion: pictor.arbor
(only the tabula's x/y), specimen pictor_prima (border now visible top
and right; stroke and pending line shifted with the canvas), and the
tessellation scene VII in probatio_tessellatio (canvas outline one
column/row in). Pixel pins in probatio_pictor_figurae moved by (6, 8)
and now also pin the TOP border pixel - the visible proof.

## 2026-10-09 - P1a: the tool bar

Fran: pictor's controls live at the bottom - squares showing the
current selection, palettes on click (P1b). The status line became 3
cells (STATUS_LINEAE in pictor_applicatio.c): children
`quadratum.instrumentum`, `quadratum.color_primus`,
`quadratum.color_secundus` (PARTES_BOTTONE, 20 x 20), each TITLED with
what to draw ("instrumentum:aspergillum", "color:0", "color:-1") -
data, like the canvas points; `figura_quadrati` draws icon / swatch /
cross, `figura_tituli` starts its text after the last child. Icons are
16 x 16 '.#' string grids (brush 16 rows; spray 44 pixels). Swatches
use COLOR_MANDATI_INDEX - the COLOURING palette, the one stroke colours
use (not the semantic palette). The app's initial `color_secundus` was
5 ("white" = the yellow, same confusion as the canvas) - now -1
(none) until patterns use it. New canvases are 16 px shorter (taller
bar). Tests: probatio_pictor_aspergillum VIII (titles, swatch fill,
cross, icon pixel count, text offset); goldens: pictor.arbor,
pictor_prima, tessellation scene VII line 29, montatio 440, twin text
column 14 on the bar's middle line.

## 2026-10-09 - P1b: palettes

A click on a bar square opens its palette (ephemeral `palette` =
instrumentum / color_primus / color_secundus; same square again
closes, another switches). The composer adds node `palette`
(PARTES_DIALOGUS) as the LAST child of the root - drawn over the
canvas, hit first - just above the square: options are squares
(`optio.<genus>.<valor>`, same title encoding and figure as the bar),
6 per row, the current one titled `...:electum` (accent border).
Ownership drives the action names: each option carries the action
that OWNS its attribute in domini.stml (`color_primus.ponere`,
`color_secundus.ponere`, `instrumentum.eligere`), so one colour
handler is registered under both colour names and reads which from
the option id. Closing: option click, Esc (`instrumentum.eligere` key
path - the root's action sees keys the canvas does not take), and a
canvas press while open (closes only, draws nothing, like a menu).
Note: the theme's colouring palette repeats entries (slots 0, 13, 14,
15 are the same dark green), so the colour palette shows duplicates.

## 2026-10-10 - pictor-strata L3: layers panel

- Bar button "quadratum.strata" (LXX wide) at the right end of the
  bar, a child of the ROOT, not of the status node: figura_tituli puts
  the tool name after the LAST square of the status node, so as a
  status child the button pushed the name off-screen.
- Panel = genus "strata" of the existing palette mechanism (open/close
  via palette.aperire, Esc, canvas click); right edge aligned to the
  button. Rows top layer first: 'oculus.<id>' + 'stratum.<id>', then
  'strata.novum' / 'strata.deletum'. Every panel node runs
  "strata.agere" (one owner for stratum_activum); the panel stays open.
- The composer needed the document (layer list): PictorCompositio.doc,
  kept in step with drawing swaps (pictor_picturam_ponere). Flagged to
  Fran - the field was not in the approved header text.
- Current layer = stratum_activum if that layer exists, else the TOP
  layer; strokes log stratum="<id>" unless it is 1.
- Bug on the way: the button code was inserted between creating the
  size square (variable q) and the eraser's "empty action on q" line,
  and reused q - the eraser's dead size square opened a palette again
  (probatio_pictor_aspergillum IX caught it). Own variable now.
