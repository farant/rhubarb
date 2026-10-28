# pictor_actiones.worklog.md

## 2026-09-05 — natus (ludus P3 T8)

The first two handlers pictor registers by name. `penicillus.ictus`
is the stroke as ONE handler with phases by event genus (spec §5.2):
depressus captures on the tabula, clears the pending points, adds
the local point; motus adds a point while captured; liberatus
writes ONE `<ictus>` actum (instrument, colour from
`ephemera.color_primus`, size from `ephemera.magnitudo`, every
point) through `pictor_documentum_actum`, clears the points, clears
`sordida` (a finished stroke never touches the ephemera island),
and releases capture; Escape while captured clears and releases
with no actum. `instrumentum.eligere` answers the `p` key by writing
`instrumentum="penicillus"` through `mutare_ephemera` — it owns that
attribute in `domini.stml`, and the dispensator sets the scriptor
to the action's name around the call, so ownership holds without
the handler knowing about it.

Both are `<tractator/>`: no I/O. The document write is not I/O in
the lint's sense — it is the document's own gate, the way
`mutare_*` are the islands' — and the worklog says so because L5
will need the list. The tool-name literal is a file-scope
`character[]` that is never written, the ctx-literal pattern
`ludus_toy.h` set; L7 treats it as immutable by use.

Tier 2 runs through the real loop: real islands with the real
canon and owner table, the real `componere`, the hand. A three-point
drag yields exactly one `ictus` actum with three `punctum` children;
the ephemera island's version moves only by the focus write; pan is
absent; a click on the canvas is a one-point stroke (MacPaint paints
a dot); Escape mid-stroke leaves the cursor where it was. Stroke
counts are read from `numerus_vivorum`, never from sequence
numbers (the store interleaves its own acta — T4's finding); the
theme's palette black is not channel zero, so the pixel probe
compares against `thema_color_ex_indice_colorationis`.

Gate: `probatio_pictor_actiones` (35). Planted faults: the point
loop emptied — red at the three-points count; capture not released
on liberatus — red at the empty-capture assertion. Both green on
revert. Examen ACCIPE; formator 0 after five long lines.

## 2026-10-08 - drag off the canvas jumped to the opposite edge (the real cause)

Fran, after the clipping fix in pictor_documentum: the line still
jumped, already in the preview, while the mouse hovered scriba (other
pane) or pictor's own status line with the button down. The points
themselves were wrong: `pictor_penicillus_ictus` read
`destinatio->punctum_locale`, which destinatio.h documents as local to
the GEOMETRIC hit - during a capture that is whatever lies under the
mouse (scriba's page, the status line at y ~ 3), not the captured
canvas. Now the point is `destinatio_ad_locale(nodus, screen point)`:
same value on a press, the canvas's own space while captured (negative
or beyond the edges, then clipped when drawn). pictor was the only
reader of punctum_locale. Test: probatio_pictor_actiones last section,
a drag from the canvas onto the status line must store y=207.

## 2026-10-09 - aspergillum (MacPaint spray can), variant A

Fran: a MacPaint spray can. Key `a` selects it (`p` the brush); the
status line shows the tool. Strokes are the log (`<ictus ...>`), so
the randomness must replay: the act carries `semen` (from the press
time and the history cursor - the same stroke at another time sprays
differently, undo + redo redraws identically) and each point carries
`t` (ms since the press). `pictor_gutta(semen, i, k, r)` is the ONE
dot generator (sors PCG32, derived stream per (point, dot), rejection
into the disc) used by both the renderer (pictor_documentum) and the
live preview (pictor_figurae) - so the preview's dots are exactly the
first GUTTAE_PUNCTO of each point's dots. Dwell: one extra dot per
GUTTA_MS between points; the RELEASE point is recorded for the spray
so holding still before releasing counts.

Variant A limits (Fran chose A, B later): dots from dwell appear only
at the next move/release; the preview shows per-point dots only. B =
a timer that appends the current position every ~30 ms while the
button is held (pictor needs a pulse hook; vicus pulses panes, the
standalone app would need its own) - then the dwell term can go.

The shared stroke routine `ictum_tractare(..., aspergillum)` keeps the
spec rule "the handler never branches on the tool STATE": the tool is
fixed by which action runs (penicillus.ictus / aspergillum.ictus).
Preview data travels in the canvas node like the points: the composer
puts "semen radius" in the tabula's title while spraying.

## 2026-10-09 - spray preview in the real colour (Fran)

The preview dots were drawn in COLOR_ACCENT_PRIMARY (green), copying
the brush preview line's convention; Fran: draw them in the real
colour. The composer's canvas title is now "semen radius color" (the
foreground palette index) and the figure draws each preview dot in
that palette colour; foreground "none" (-1) previews nothing, as the
stroke paints nothing. The brush's thin preview line is still accent
green (not asked).

## 2026-10-09 - line tool (linea, Fran)

Click-click with a rubber-band preview, Shift+click chains, Esc or a
click outside the canvas cancels (Fran). Mechanics:
- The canvas keeps the motus CAPTURE between the two clicks:
  destinatio routes every mouse-position event to the captured node
  regardless of buttons, so hover motion reaches linea.ictus for the
  preview. The first click's release returns VERUM and keeps the
  capture; nothing else releases it except vicus motum_relinquere
  (tab/pane switch), which also clears pending points - free cancel.
- "Outside the canvas" = destinatio->id_geometricum != the tabula id
  while captured; that click is consumed (the palette square under it
  does NOT open).
- Each segment is its own stroke (two points, undo per segment),
  logged instrumentum="linea" with magnitudo from magnitudo_lineae
  (1 2 4 8 16). The document needed NO change: unknown tools already
  render as the brush, i.e. the disc sweep with patterns.
- Keys still reach the radix through the canvas (non-Esc keys return
  FALSUM), so switching tool with p/a/e/l while a line is pending
  goes through instrumentum.eligere, which now drops the pending line
  (else the brush would inherit a captured 2-point preview).

## 2026-10-13 - pending paste (glutinatio pendens), UI half (Fran)

Cmd+V no longer creates a layer: imaginem_glutinare prepares the
document's pending paste (both ditherings, outside the acta) and sets
the transient field 'glutinatio' = "B". One action, glutinatio.agere,
owns the field and dispatches on the button id (glutinatio.A / .B /
.confirmare / .abicere), the strata.agere pattern - so one owner ever
writes the field. Key paths (Esc, Return, Cmd+V-again) run inside
instrumentum.eligere, so glutinationem_scribere sets repo->scriptor to
"glutinatio.agere" for the write and restores it (memory: domini
refuse silently otherwise). confirmare writes stratum_activum under
"strata.agere" exactly like the old paste did.

While pending, instrumentum.eligere handles ONLY Esc / Return / Cmd+V
and returns FALSUM for every other key (tool letters, undo) - Fran:
the canvas ignores drawing while a paste is pending; this is where
resize / positioning will live later.

Tests: probatio_pictor_imago II rewritten for the pending flow (+ II b:
Esc, X, Return) - written after the code, so planted twice to see red
(canvas action kept while pending -> red; Esc not discarding -> red),
restored with plain cp. Owner counts in probatio_pictor_canones and
probatio_organum went XVII -> XVIII (glutinatio is a new owned field;
probatio_organum exempts it from the frozen pre-S2 owner table the
same way it exempts S5c palette).

## 2026-10-13 — pending paste: canvas drag erases (glutinatio.agere)

While pending, the tabula's action is `glutinatio.agere` (was ""), and
`pictor_glutinatio_agere` routes nodus "tabula" to `glutinationem_radere`
for down/move/up (captura held like ictum_tractare; last point in
`PictorActiones.rasura`). No new action, so organum.stml and the owner
counts did not change. Option (MOD_ALT) = restore. The mask is
document memory outside insula, so per-event writes are fine (the insula
is never touched mid-drag).

Size = magnitudo_penicilli whatever the current tool: the pending bar
gained `quadratum.magnitudo` after X (opens the penicillus size palette
- componentia forces instrumentum "penicillus" while pending, palette
allows only "magnitudo"), and `pictor_magnitudinem_ponere` treats a
pending paste as penicillus (the spongia has no size).

Test gotcha: after II's undo/redo the layer ORDER is ids 1,3,4,2, not
by id - II c finds the new layer by stratum_activum's id. First version
indexed by count and saw 51 "wrong" pixels that were the A layer.
Plants: Option ignored, size override removed, tabula action "" - all
red in probatio_pictor_imago.

Fran's look (2026-10-13, ./apps/pictor/pictor.sh): Cmd+V, size square,
drag reveals Atkinson under B, Option-drag restores, A swaps the reveal,
OK commits as previewed - "everything seems to be working".

## 2026-10-26 - elenchus.agere (pinacotheca T4, quarta)

Menu handler. `palette` is owned by `palette.aperire`; a write from
`elenchus.agere` was refused SILENTLY (the domini trap, memory
domini-scriptor-silent-refusal) - "Open picture" left palette unchanged
while New picture only looked fine because dimissio closed the menu.
`palettam_scribere` sets the writer to palette.aperire for the call
(same pattern as glutinationem_scribere). Picture switching goes
through `PictorActiones.picturam_condere/ponere` + `montatio` (filled
by pictor_montare) because pictor_applicatio.h includes this header.
New picture name: "pictura-<n>", n = 1 + highest existing n.

## 2026-10-26 - pinacotheca.agere (T5, quarta)

"Open picture…" prepares the gallery state (pictor_pinacothecam_parare:
scratch emptied, newest-first list re-read) BEFORE switching palette,
and resets the page. `pinacotheca.agere` owns `pagina_pinacothecae`;
`paginam_scribere` still swaps the writer because elenchus.agere also
calls it. Thumbnail tap: spatium parsed from the node title, palette
closed, then picturam_ponere through the montatio. Found: with a line
half-drawn the line tool CAPTURES the next tap anywhere and abandons
the line (existing linea rule) - so the first tap on ⋯ only cancels the
line; a picture switch can never carry a pending line (pinned in test).

## 2026-10-28 - menu, Shared pictures, viewer (communio-plan-5 T6, quarta)

The ⋯ menu is built from ONE table (`pictor_elenchum_parare` over
`PictorElenchiCondiciones {importare, communio, inscripta}`) that the
composition, the menu figure and `elenchus.agere` all rebuild from the
composition's state (`actiones_ctx.compositio`, `figurae_ctx.compositio`).
The figure used to guess "Import image" from the number of children -
with five conditional items that guess no longer works.

Trap (again): a menu item that OPENS another palette must not be
`claudit`, or dimissio removes the palette the action just wrote. It was
done for item 0 only ("Open picture"); now every item whose titulus is
`pinacotheca.aperire`, `communia.aperire` or `inscriptio.aperire`.

Shared pictures reuse the gallery grid (`rete_componere`, generic over
dialog name, node prefix and a title callback; empty list -> one
"<name>.vacua" text node). The viewer is a dialog over the canvas:
image node `spectator.imago` (title `alienus:<w>x<h>:<hora>:<replica>:
<spatium>`, drawn like a thumbnail), credit line via the composition's
`auctor` callback (pictor_componentia never links pictor_communio), and
‹ "Edit a copy" › X. `spectata` stores the picture's IDENTITY
(`replica:spatium`), so arrivals never shift it; a missing identity
composes the grid instead. Swipe = `EVENTUS_TRACTUS_INCIPIT` (origin
kept in `PictorActiones.origo_tractus`) + `_FINIT`, |dx| >= a quarter of
the image width and |dx| > |dy|; ← → in the root key handler only while
the palette is "spectator". The image node consumes plain presses so a
tap on the picture does not close it; the dialog itself (tap outside the
picture) closes back to the grid.

Generated file: new organum fields/actions need `bin/fabrica sanare
lib/organa_infixa.c` (the manifest is compiled in). A `"` inside an
organum `nota="..."` breaks the whole manifest silently (app fails to
build its montatio) - use single quotes inside notes.
