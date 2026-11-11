# mundus — a vicus pane where a character walks around a place

Status: BUILT 2026-11-10 in secunda (T1-T10 of
`project-specs/mundus-plan.md`), not in main; `$world` opens the pane.
Not yet judged by Fran in the running vicus. Approved by Fran
2026-11-10. Brainstormed
2026-11-10 with Fran (secunda session 134c8417). Nothing is built.
Builds on statuae (`project-specs/statuae-spec.md`), which is in
secunda and not yet in main.

*The question (Fran, 2026-11-10): a `$world` command, "being able to
move a character around and then maybe having wandering mobs that you
attack at some point will be plenty to shoot for".*

Names: the family is `mundus` (Claude's proposal; the word is free in
include/ and lib/). All file and function names here are proposals.

## I. Intent

1. **Walk a character around a place.** A pane opened with `$world`
   shows a small part of a level from above at an angle; one of
   Fran's characters stands in it and walks where he steers.
2. **Dithered, and calm.** Everything is drawn in the 16-colour
   palette. The ground does not shimmer while it scrolls.
3. **A base for the second stage.** Wandering mobs and attacking come
   later with their own short design. This stage is built so that
   stage adds to it without rework.

Non-goals (this stage): mobs, attacking, health; building our own
levels from the tile kit; trees, boulders and other objects (the kit
came without them; see III.5); turning or zooming the view; jumping,
swimming, falling; going inside the hut; the lights stored in the
level file; sound; remembering where the character stood after the
pane closes; more than one place.

Anti-goals: ground that flickers when the view moves; a character
that walks through cliffs, water or the hut, or off the level; a
character whose speed depends on the frame rate; a pane that keeps
the machine busy when hidden or when nothing moves; a GPU path;
outside libraries; any change to what `$models` shows or to the
pinned digests of the existing renderer and dithers.

## II. Decisions

Fran's, as he made them (2026-11-10):

1. **Scope:** walking only to start. Mobs and attacking are a later
   stage.
2. **Size:** the character is about a half to a third of its size in
   the model viewer; "might need to be a little smaller".
3. **View:** one fixed angle.
4. **Steering:** W, A, S, D.
5. **Character:** the mushroom man to start; he will try others.
6. **Place:** one bounded space for now. Later, ideally, a 3D
   tileset used to make a variety of landscapes.
7. **Storage:** the clearing may be added to his live volume
   (`~/.rhubarb/paginae-proba.volumen`) the way the models are.

Fran accepted these when they were played back to him as Claude's
guesses:

8. The view follows the character, which stays at the centre of the
   pane.
9. The view is the three-quarter view from above, drawn without
   perspective (parallel projection).
10. Steering is relative to the screen: W walks up the picture. Two
    keys together walk diagonally.
11. The character turns to face where it is going, plays its walk
    clip while moving and its idle clip when the keys are released.
    CORRECTION (Claude, after the playback): none of Fran's
    characters has an idle clip (III.6). Standing still is the rest
    pose, held; a model that does have a clip named `idle` plays it.
12. It cannot pass cliffs, the pond or the hut; it uses the stairs to
    change height; it cannot leave the level.
13. The ground is dithered with Atkinson, characters with exemplaria.
14. The pane has a character chooser like the one in `$models`.
15. The place is stored marked as a place, so it does not appear in
    `$models` or in the character chooser.
16. The character starts on a walkable spot near the middle.
17. A place is "a ground model whose parts are marked walkable or
    solid", wherever the model came from. This is what keeps the
    tileset step open.

Claude's calls (not discussed; change freely):

18. View angles: turn 45 degrees, tilt 35 degrees (the angles of the
    test pictures in III.2).
19. Scale: 40 pixels per unit to start, one named constant. A
    character is 2 units tall (kit rule), so about 80 pixels.
20. Every character is scaled so its rest pose is 2 units tall.
21. Walking speed: about 4 units a second, one named constant, tuned
    by eye against the walk clip.
22. The ground picture is made once per region and reused (see IV.5),
    for calm, not for speed.
23. One dither each (decision 13); no dither chooser in this pane.

## III. What was measured and found (2026-11-10)

All with throwaway programs outside the tree (scratch folder
`mundus/`: `tempus.c`, `lapsus.c`, `tabula_gressus.py`, and a patched
copy of the renderer made by `gen_pictura_x.py`).

1. **The level.** `planum.glb` from the Retro Diffusion scene kit
   (asset 33d50990): 64 x 64 units, which is 8 x 8 tiles of 8 units;
   height 0 to 9 units; 914 nodes in a flat list (no children, no
   node matrices), 912 with a mesh; 32,614 triangles; 53 materials,
   12 textures; no clips. Our reader loads it unchanged.
2. **Render time**, whole frame, parallel view at 45 / 35 degrees,
   every triangle of the level submitted each frame, no culling:

   | pixels per unit | 360x438 | 480x541 |
   |---|---|---|
   | 16 | 5.5 ms | 7.7 ms |
   | 24 | 4.9 ms | 7.6 ms |
   | 40 | 4.1 ms | 5.9 ms |

   Atkinson over the full frame adds 3.5 ms (360x438) or 5.5 ms
   (480x541). Pictures: `build/statuae/mundus_24.png`,
   `build/statuae/mundus_40.png`.
3. **Sliding.** With the parallel view, moving the view by a whole
   number of pixels gives exactly the same raw picture, shifted
   (0.00 % of pixels differ at 1, 2 and 8 pixels). Re-running
   Atkinson on the shifted picture changes 20-22 % of the pixels.
   So dithering the visible ground afresh each frame would flicker
   over a fifth of the picture at every step; a ground picture
   dithered once and slid does not.
4. **Walkability from the tags.** Every mesh node carries
   `extras.tags`. Counts: `walkable` on 231 nodes (ground, stairs,
   floor), `water` on 16, `collidable` without `walkable` on about
   610, `decor` only on about 50. A top-down map at 2 cells per unit,
   where the topmost surface in each cell decides, gives a sensible
   result: 15,121 walkable cells, 848 water, 415 solid, 257 empty.
   Walkable heights cluster at three levels (about 0.5, 4.5 and 8.5
   units) joined by stairs that rise about 4 units over about 4
   units of run; cliffs are a 4-unit jump between neighbouring
   cells; the hut is one solid block of about 8 x 6 units; ponds
   read as water. The reader does not keep the tags today.
5. **What the kit lacks.** No objects (trees, boulders, fence, well):
   populating costs $2.50, the balance is $1.95. Single tiles are
   refused by our reader because they share one texture file; this
   blocks own layouts, not this stage.
6. **Characters.** Clips in the files on disk: golem `walk`,
   `boulder_throw`; mushroom man `walk_bouncy`; dragon giraffe
   `punch`, `walk`; giant spider `spooky_walk`, `run`; troll
   `walk_bouncy`, `club_attack`. No idle clip anywhere; no skins.
   Exemplaria on a full-size
   character costs 19-30 ms a frame at 360 wide (statuae spec VII);
   at a half to a third of the size the character has a quarter to a
   ninth of the pixels. Not measured at that size.

## IV. Design

**The parts, and what each one knows.**

1. *Part marks in the model* (`statua`, `statua_glb`). The reader
   keeps, per node, three flags read from `extras.tags`: walkable,
   solid (`collidable`), water. Unknown tags are ignored; a model
   without tags has no flags. Nothing else about the model changes.
2. *The scene renderer* (new function beside `statua_pingere`, which
   stays byte-for-byte as it is). Input: a list of placed models
   (model, node matrices, one placement matrix each) and a view: turn
   and tilt angles, pixels per unit, the world point at the picture's
   centre, lit or flat. Parallel projection. One depth buffer for
   all. Output: RGBA, depth, and per pixel which placed model covers
   it (0 = none). The view's centre is snapped so that world
   positions land on whole pixels; this is what makes III.3 hold.
   Back faces are judged by the view direction, not by an eye point.
3. *The walk map.* Built once from a place model: a grid over the
   level's footprint (2 cells per unit worked in III.4; the builder
   may go finer). Per cell: the kind of the topmost surface
   (walkable, water, solid, nothing) and its height. Nodes with no
   walkable, solid or water flag are skipped. Answers two questions:
   "what height is the ground at (x, z)" and "may a body of radius r
   move from here to there". A move is allowed when every cell under
   the body at the target is walkable and the ground does not rise
   or fall faster than a slope limit that lies between the stairs
   (about 1 in 1) and a cliff (4 units between neighbours). Knows
   nothing about pixels or keys.
4. *The world* (`mundus`). The state: one place (model + walk map)
   and a list of bodies. A body has a position on the ground, a
   facing angle, a model, a current clip and clip time, and an
   intent: a direction to walk in, or none. One function advances
   the world by a time step: each body with an intent turns toward
   it and moves at walking speed as far as the walk map allows
   (blocked in one axis, it slides along the other); its height
   follows the ground; its clip is walk while it moved, otherwise
   idle if the model has one, else the rest pose. The world does not know who sets an intent. In this
   stage the pane sets the player's from the keys; in the next stage
   a wander routine sets the mobs'. No pixels, no keys, no clock of
   its own: fully testable with numbers.
5. *The ground picture.* The place alone, rendered with the scene
   renderer and dithered with Atkinson, in regions anchored to the
   world (not to the pane), each made the first time the view
   reaches it and then kept: palette indices and depth. A frame's
   ground is copied out of the regions. Seams between regions
   (Atkinson's error does not cross them) are to be looked at; if
   they show, regions are rendered with an overlap margin.
6. *A frame.* Advance the world by the clock; snap the view to the
   player; copy the ground; for each body in view, pose it, render
   it alone into its own small box with the scene renderer, drop the
   pixels that lie behind the ground's depth, dither what is left
   with exemplaria under that mask (the crop, mask and budgets of
   `statuae_quadrum`, unchanged), and lay it over the ground. Pixels
   outside the level are the theme's background colour. Bodies are
   drawn far to near, so a nearer body covers a farther one.
7. *The store.* A place is a model blob in the volume like any
   other, with a kind mark. Listing models for `$models` and for the
   character chooser leaves places out; listing places leaves
   characters out. `bin/statuae addere` gains a way to say "this is
   a place" and still refuses to guess the volume.
8. *The pane* (`$world`). The picture fills the pane; one header row
   with the character chooser (previous / next arrows and a count,
   as in `$models`). Keys: W A S D held to walk; `[` and `]` change
   character. Mouse: the header arrows. With no place in the volume,
   or no character, the picture area says so and names the command.

**Time.** The world advances by clock time, capped per step so a
stall does not teleport a body. While a key is held or a body is
moving the pane asks for a tick every 33 ms. With nothing moving and
no idle clip playing it asks for none. A hidden pane is not ticked
(vicus does this).

**Keys held and focus.** The pane keeps a set of held keys, filled by
presses and emptied by releases. When the pane loses focus or is
hidden, the set is emptied, so the character stops. The first task
of the plan traces how releases and focus loss reach a pane in
vicus; if a hook is missing, adding it to vicus is its own task
(Claude's call that this is acceptable).

**How many bodies.** The frame pipeline is sized for up to about
eight bodies in view (one now, mobs later). The cost of exemplaria
on an 80-pixel body is measured in an early task, before the pane
is built; if eight do not fit in a frame, the remedy (reuse of
dithered poses, or a cheaper dither for bodies) is decided then,
with Fran.

**Reusing the body dither.** `statuae_quadrum_pingere` renders the
model itself. Bodies need the same crop, mask and budgets applied to
a picture and mask already rendered, so that part gets its own entry
point beside the existing function, whose digests stay pinned.

**Budget** (targets, not gates). At 360x438: ground copy under 1 ms;
one character, render + exemplaria, under 10 ms; a frame under 16 ms.
Making one ground region may take longer than a frame; it happens
once per region.

## V. How it is tested

- Part marks: a small fixture GLB with tagged nodes; flags read back.
- Scene renderer: pinned digests for a fixed scene; a test that the
  picture shifted by whole pixels is identical (III.3 as a test); a
  test that a model behind another is hidden and the cover map says
  who is in front. The existing `statua_pingere` digests do not move.
- Walk map: a hand-made level with a floor, a cliff, a stair, water
  and a block; each rule (blocked, allowed, height) asserted.
- World: stepping with numbers only. Walks at the set speed
  regardless of step size; stops at a cliff; slides along a wall;
  climbs the stair; cannot leave; clip switches between walk and
  idle; facing follows the intent.
- Pane: headless picture tests as in statuae (`-imago`), with keys
  injected.
- Every gate is proved by a compiling plant, as in statuae.

## VI. Left to the builder

File split and names; the grid resolution and slope limit (from the
map, III.4); region size and whether the whole ground is made at
open; which clip counts as "walk" when names vary (a name that
contains `walk`, else the first clip, else none); which way a
model's front points; how diagonal steering maps onto the 45-degree
view; whether the character chooser is a dropdown or arrows only;
the exact command form for adding a place; turn speed.

## VII. The second stage, for orientation only

Mobs are more bodies in the same world whose intent comes from a
wander routine. Attacking is a third kind of clip state on a body
plus a reach test between bodies. Neither is designed here.

## AUDIENDA (not verified)

- Whether vicus passes key releases to a pane. fenestra has the
  event (`EVENTUS_CLAVIS_LIBERATUS`, lib/fenestra_macos.m:853); the
  path from there to a pane was not read. Held-key walking depends
  on it.
- Exemplaria's cost and look on an 80-pixel character.
- Whether Atkinson seams between ground regions are visible.
- How the walk clips look at 4 units a second (feet may slide).
- The white squares seen along the top edge of the earlier
  perspective picture of the level (`build/statuae/scaena_planum.png`);
  cause unknown, not seen in the parallel pictures.
- Asked of Fran 2026-11-10 and not answered separately ("that sounds
  good, go ahead with the plan"), so Claude's defaults stand until
  he says otherwise: the real level is NOT committed (licence
  unknown, 3.4 MB; tests use hand-made levels); a character behind
  a cliff or the hut is fully hidden. A copy of the level, the kit and the scratch programs is in
  `build/statuae/scaena/` (ignored by git).
- Whether the hut's doorway should be passable; the top-down rule
  makes the whole hut solid.
- Memory for ground regions: the whole level at 40 pixels per unit
  is roughly 3,600 x 2,400 pixels (an estimate, not measured).
