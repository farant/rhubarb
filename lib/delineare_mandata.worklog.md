# delineare_mandata.worklog.md

## 2026-09-05 — natus (ludus P3 T3)

The CPU rasterizer: `Mandata` in, pixels out, one pass over the
elements with a stack of frames. A coetus pushes a frame (origin
moves by fines and translatio, scale multiplies, the clip
intersects the fines when sectio is set) and every element consumes
one count from EVERY open frame, since `magnitudo_arboris` counts
the whole subtree including the coetus itself; a frame pops when
its count reaches zero, an empty coetus pops at once. The plan's
draft decremented only the frame just pushed — caught by tracing
T2's six-element tree by hand before the first build, not by a
test, which is the wrong order; the probatio's nested coetus now
pins it.

Pixel formats, so nobody rediscovers them: the pixel table's packed
`i32` is ABGR in the word, which is R,G,B,A in memory on this
machine — exactly `Imago`'s layout. `imago_ex_tabula` is therefore
a reinterpretation, no copy. `color_ad_pixelum`/`color_ex_pixelum`
are the two conversions; `thema_color` and
`thema_color_ex_indice_colorationis` resolve the two thematic
colour genera; RGBA is the packed pixel itself.

An image mandatum carries no pixels: its `textus` field (what
`mandata_imago` stores) names the picture, and the caller's
`ImagoFons` resolves the name. Pictor will name pictures by the
projection's sigillum. Alpha 0 is skipped, which is the overlay
convention. v1 limits: line thickness I, text unclipped, integer
scale, no blending. The window-free `tabula_pixelorum_creare_nuda`
exists because `fenestra_creare_tabulam_pixelorum` needs a window;
it sets scale 1 and the window dims equal to the table.

Specimen discipline (tier 7): `specimen_iudicare` on a missing
exemplar writes `<titulus>.novum.png` beside it and fails; the
candidate is promoted by `mv` after being LOOKED AT; `.novum.png`
and `.differentia.png` are gitignored.

Gate: `probatio_delineare_mandata` (13: twelve pixel assertions that
hold without any golden, then the specimen). Planted faults: the
coetus ORIGIN dropped from the frame — red at the scaled-image pixel
and the specimen; `sectio` never applied — red at the
yellow-must-be-absent pixels. Both green on revert. A plant on the
coetus TRANSLATION was mute: the probatio's coetus translates by
(0,0). The third mute plant of this plan, same cause each time —
a plant must touch a value the test varies. The rasterizer's
translation path is exercised by T9's prospectus pan (-5,3).
Examen ACCIPE; formator 0 after five long lines (three comments).

## 2026-10-05 — negative origins vanished, and lines/polygons HUNG

Found via pictor's desk fill (module 013): a filled rectangle inside the
viewport, panned left (pan x = −5), drew NOTHING. Cause: the delineare
API takes coordinates as `i32`, which in this house is UNSIGNED;
`primitivum_pingere` computed origins as `s32` and cast them. A −5
became ~4·10⁹: `delineare_rectangulum_plenum`'s `x + latitudo` wrapped
and its loop never ran; `delineare_lineam` works in s32 internally (the
cast round-trips) but picks its step with an UNSIGNED compare
(`x0 < x1`), so a line from −5 to 34 stepped AWAY from its end and never
stopped - an infinite loop, as did a filled polygon with a negative
vertex. Text was already right (glyph pixels by wrapping addition, each
bounds-checked).

Fix here, not in the API (Fran: clip every primitive; the legacy
delineare API keeps its unsigned coordinates and ~14 consumers):
primitives wholly in positive space take their old calls (bytes
identical - the mandata_prima specimen is unchanged); otherwise they
are clipped in SIGNED space first - `_plenum_secare` (filled rectangle
to the canvas; outlines as their four edges, delineare_rectangulum's
exact geometry), `_lineam_signatam` (the same Bresenham, plotting only
non-negative pixels; bounds and group clip still by the pixel setter),
`_polygonum_secare` (Sutherland–Hodgman against x ≥ 0 then y ≥ 0 before
the old fill; outlines as signed lines).

Tests (a 20×20 buffer, each primitive starting negative): fill, outline
(top, bottom, right edges; inside empty), line (CREDO_NON_PENDET first,
the real call only if it did not hang), filled polygon (same guard),
text "M" at x = −3 (pinned, already correct). Red: rectangles absent,
line and polygon HUNG (non_pendet failed). Plants caught, clean
compiles: fill unclipped, outline via the old call, line via the old
path (hang), polygon fill unclipped (hang).

Still open, older and separate: text ignores the group clip (sectio) -
`tabula_pixelorum_pingere_chordam` takes no clip.
