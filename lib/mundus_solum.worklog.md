# mundus_solum worklog

## 2026-11-10 - the walk map (mundus-plan T4)

**The rule that had to change.** The plan fixed one slope limit
(`MUNDUS_CLIVUS_MAXIMUS`, rise per unit of travel). That cannot work
on the real level: its stairs are STEPS, eight flat treads of half a
unit, not a ramp. At four cells per unit the height jumps 0.5 between
two neighbouring cells, a slope of 2 or more over any short move,
while the average over the whole stair is 1. So the test is now
"step plus slope": a height difference is passable when it is at most
`MUNDUS_ASCENSUS_MAXIMUS` (0.75, one stair step with room to spare)
plus `MUNDUS_CLIVUS_MAXIMUS` (1.5) times the distance. A cliff is 4
units and its rim pieces rise 1.0-1.5 per quarter-unit cell, so both
stay blocked. The same test serves three places: a move (`licet`), the
cells under a body (`stare`), and which neighbours a height query may
blend with (`altitudo`, step part only).

**Why `stare` compares heights and not just kinds.** The top of a
cliff and the ground below it are both walkable. With kinds alone a
body could stand with half its width hanging over the edge, or pushed
into the wall from below. Requiring every cell under the body to be
within a step of the centre keeps it a radius away from both.

**Why `altitudo` blends only joined cells.** Plain bilinear blending
made a body sink toward the lower ground as it neared a cliff edge.
Blending only with neighbours within a step keeps the plateau flat to
its edge and still turns the stair's treads into a ramp for the eye.

**Vertical triangles cover nothing** (zero area seen from above), so
cliff walls never appear in the map; a cliff is the height jump
between the cells on either side.

**The real level** (fixture `probationes/fixa/statuae/planum.glb`,
3.4 MB, committed with Fran's word 2026-11-10; 256 x 256 cells):
60,626 walkable, 3,424 water, 1,486 solid, none empty. A body of
radius 0.5 starting at the middle (31.88, 31.88) reaches 42,881 cells:
all of the low ground (y about 0.5) and the middle level (y about
4.5). The TOP level (y about 8.5, the north-west corner, about 28 x 24
units) is NOT reachable, and that is the level's layout, not a fault:
its three staircases (kit tiles 14, 20, 27) all join y 0.5 to 3.8.
What looked like stairs up to the top in the first rough map were
sloped walkable-tagged cliff rims, too steep to pass.

The scratch program `solum.c` (not in the tree; copy in
`build/statuae/scaena/exploratio/` may be stale) prints the
reachability map and a window of cell heights.

**Names.** The word lint refuses two-letter coordinate names it has
not seen (cz, dz, gx, gz, pz, tz); hence `cella_x`, `delta_z`,
`reticuli_x`, `pars_z` and so on. `Punctum` is taken (mandatum.h,
situs.h); the local point type is `PunctumSoli`.

**Build.** A new lib file needs `./tools/compile_tests_fontes_generare.sh`
before `./compile_tests.sh` can link its suite.

Plants, four red: walkable wins a tie in height; radius ignored;
heights blended across a cliff; no step test in `licet`.

## 2026-11-10 - footprint from the cell centre (mundus-plan T5)

`mundus_solum_stare` now measures the body's footprint from the
centre of the cell the body is in, not from the body's exact
position. Reason and symptom in `lib/mundus.worklog.md` ("the wall
was bumpy"). Test: four more lines in section "radius". The real
level's pinned counts did not change (the reachability walk there
already stood on cell centres).
