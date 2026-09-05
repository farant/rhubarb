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
