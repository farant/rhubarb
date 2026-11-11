# mundus_pictura worklog

## 2026-11-10 - the ground picture (mundus-plan T6)

The place is rendered and Atkinson-dithered in 256-pixel squares on
the WORLD pixel grid, each made the first time a copy touches it and
kept (palette indices + depth). A frame copies its rectangle out.
This exists for calm, not speed: re-dithering the visible ground each
frame changes a fifth of the pixels at every scroll step (spec III.3).

**Margin.** Each region is rendered 16 pixels larger on every side
and the margin is thrown away. Atkinson pushes error right and down;
without the margin a region's left and top edges would start with no
error from their neighbour and the border could show as a line. With
it, the pictures of the real level across four borders
(`build/statuae/humus_suturae.png`, and `_lineae.png` with the
borders drawn in red) show no seam that Claude can see. The rough
count in test "sutura" agrees: colour changes between neighbouring
columns are 1187 at a border against 1194 in mid-region (of 3100).
Fran has not judged the pictures yet.

**Bounds.** The place's vertices are projected once at creation; the
table of regions covers exactly that box. A copy outside it returns
the caller's background index and never makes a region; coordinates
beyond +-2^30 are answered with background without arithmetic on
them.

**Memory.** A region keeps 65,536 index bytes and 262,144 depth
bytes (about 320 KB). The render and the dither run in the same
arena behind a mark (`piscina_notare` / `piscina_reficere`) taken
AFTER the region's own two buffers are allocated, so only the kept
part stays.

**Measured, real level** (scratch `humus.c`): making a region costs
2-5 ms; the first 360x438 copy made 4 regions in 19 ms; later copies
of 360x438 cost 0.1 ms, 480x541 about 0.7 ms.

**Test-level gotcha.** The hand-made level in
`probationes/mundus_locus_fictus.h` had its quads wound so the faces
pointed DOWN. The walk map does not care, the renderer culls them:
the first run of this suite saw an empty picture. The winding is now
upward.

Plants, four red. Two were green at first and each named a missing
test: a region rendered at the wrong offset (now: the copy must match
a direct render of the same rectangle in coverage and depth, pixel
for pixel) and a region made outside the place's box (now: a copy
three regions left of and above the place makes none).
