# tessellatio worklog

## 2026-10-05 — painter's order across the two layers

Found by Fran in terminal pictor (013 B3c): strokes showed while the
mouse was down, vanished on mouse-up. Cause: tessellatio has two layers
- the pixel stratum (images, polygons -> quadrant cells) is resolved
FIRST and the cell pass (rects, text, axial lines) writes over it. So
every cell-path primitive beat every image regardless of order. Pictor's
new desk (a filled rect, drawn BEFORE the page image) therefore covered
the whole page; the live stroke survived only because it is drawn after
the page as a line (cell path).

Fix: a cover map `operta` (s32 per cell = index of the LAST image whose
cell box covers it, -1 if none; NIHIL when there is no stratum, i.e. no
piscina). `_cellula` refuses a cell when `operta[k] > ctx->index`, so a
cell-path primitive placed before an image stays under it, one placed
after it stays on top. `_operta_computare` walks the scenes exactly like
the cell pass (same clip c0..c1/l0..l1) and boxes each image with
`modulus_columna`/`modulus_linea` (end = column(px + w*scala - 1) + 1).

Images ONLY: polygons and oblique lines would only give a bounding box,
which would wrongly hide earlier primitives in the box's empty corners.
A cell-path primitive under a polygon still draws over it (old
behaviour, unchanged; nothing in the house hits it yet).

Tests: probatio_tessellatio V (rect, image, rect, text interleaved);
probatio_ludus_tessera_pictor asserts a page cell is not the desk.
Plants: no cover check (caught by both), index stuck at 0 (caught only
once the text-AFTER-image assert was added - the first vectors only
checked "under").
