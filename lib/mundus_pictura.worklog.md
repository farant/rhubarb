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

## 2026-11-10 - bodies and the frame (mundus-plan T7)

**Measured first** (scratch `corpus.c`, one body at world size, pose
+ render + exemplaria): mushroom man 2.9 ms, golem 4.6, dragon
giraffe 3.3, troll 3.6; render is 0.15-0.5 ms of that, the rest is
exemplaria. Whole frames on the real level (scratch `quadrum.c`):
one walking character 3.5 ms at 360x438 and 3.9 ms at 480x541 (17-21
ms on the frames that make new ground regions); EIGHT characters in
view 26.7 ms average, 38 worst, at 480x541. So one character is far
inside a 33 ms tick; eight are at its edge. Nothing is done about
eight in this stage (there is one body); the remedy belongs to the
mobs stage (reuse the dither of a body that has not changed pose,
or a cheaper dither for mobs).

**Two passes, because order must not matter.** First version: each
body rendered, masked against the frame's depth, dithered, written,
one after the other. Test "ordo" (the same two overlapping bodies
listed in either order must give the same bytes) failed: a far body
dithered before the near one was drawn used a mask that still
included the pixels the near body would cover, and exemplaria's
result depends on its mask. Now pass one renders every body into its
box and records, per frame pixel, depth and OWNER (0 = ground, k + 1
= body k); pass two dithers each body under the mask "pixels I own".

**The box** is the body's scaled bounding sphere around its centre,
grown by the margin and snapped to the 8-pixel grid in WORLD pixels,
then clipped to the frame. World snapping matters: the frame's origin
moves with the player, and a box snapped in frame coordinates would
shift the dither grid under a standing body every time the player
took a step.

**Scale.** `mundus_forma` scales a model so its rest pose is 2 units
tall, unless that would make it wider than 3 units, in which case
width decides. Height alone made the spider (low and wide) 2.3 times
its natural size, over 4 units across.

**Idle.** A body that is not moving is posed with `cyclus_quietis` at
time 0. No model has an idle clip today and the world keeps no clock
for one; when one appears, a body needs an idle time beside
`tempus_motus`.

**Scratch-folder trap.** A stale copy of `mundus_pictura.h` beside the
scratch program shadowed the real header (`#include "..."` looks in
the source file's own folder first). Drafts now live in `vetera/`.

Plants, six red (five here, one in statuae_quadrum): no depth test
against the ground; mask not limited to owned pixels; frame origin
off centre; no width cap on the scale; walk clip ignored; writing
outside the mask.

## 2026-11-10 - ground dither choice (Fran's question, after T10)

Fran: "since the background is static anyway could we technically
'bake' it with exemplaria anyway since it doesn't need to be
recomputed every frame?" Yes: the ground is dithered once per region,
so exemplaria's cost is paid once. `mundus_humus_creare_modi` takes
`MundusHumiModus` (ATKINSON, EXEMPLARIA); `mundus_humus_creare` is
Atkinson. Exemplaria runs with its default options (no mask, no
budgets) on the region plus its 16-pixel margin.

Measured (scratch `humus_exemplaria.c`): about 43 ms per region
against 2-5 for Atkinson; a 768x512 picture whole 188 ms. No seam is
visible between regions (the 4-pixel hatching period divides the
256-pixel region and the 16-pixel margin). Pictures
`build/statuae/humus_ex_{atkinson,exemplaria_totum,exemplaria_regiones}.png`.
Claude's read of the look: grass becomes a regular dotted pattern and
darker, cliffs lose their crack lines, characters stand out less.
Fran chose to have a toggle and judge by eye.

A humus cannot change mode: its regions sit in its arena. The caller
gives the humus its own arena, empties it and creates a new one.

Test section "modus": different picture, same coverage, and the
exemplaria ground is as stable under a shifted copy as Atkinson's.
