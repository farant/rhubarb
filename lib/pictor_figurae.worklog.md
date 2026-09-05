# pictor_figurae.worklog.md

## 2026-09-05 — natus (ludus P3 T9)

Pictor's first two figurae, registered by role. `figura_tabulae`
emits ONE image whose identity is the projection's sigillum (the
mandata carry a name, never pixels; the same acta always name the
same picture), the pending stroke as lines between the tabula's
`puncta` (copied there by `componere`, T7), and a 3×3 cursor at the
last point. `figura_tituli` paints the status strip and its title.
`pictor_imago_fons` is the rasterizer's `ImagoFons`: a provenientia
equal to the document's current sigillum resolves to the projection,
anything else to NIHIL.

The preview colour is a compromise: a figura reads the tree only,
so it cannot read `ephemera.color_primus`; the pending stroke draws
in `COLOR_ACCENT_PRIMARY`. The P4 fix is a `color` field on the
tabula componens filled by `componere`, the same move as `puncta`.
The marquee overlay is P5. The sigillum hex is formatted on every
call; a cached hex on the document is the obvious saving if it ever
shows in T10's numbers.

Tier 6 asserts the mandata (one image naming the sigillum, one
line for two points, one text); tier 7 rasterizes the real tree
through the real document: a stroke at y=2 lands at screen y=5
under pan (-5,3), which is the translation path the T3 plant could
not reach. `PARTES_TITULUS` is the status line's role, not a type.

Gate: `probatio_pictor_figurae` (13); specimen
`probationes/pictor/specimina/pictor_prima.png` born on the first
run, looked at, promoted. Planted faults: the image never emitted —
red at the image count and the black pixel; the fons comparing
against a foreign name — red at the fons assertion. Both green on
revert. Examen ACCIPE; formator 0 after three long lines.
