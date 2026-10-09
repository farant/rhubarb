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
