# plastica — a library of 3D assets that Claude models from text

Status: APPROVED by Fran 2026-11-11 ("yes, that looks good"). Nothing
is built. Plan: `project-specs/plastica-plan.md`.
Brainstormed 2026-11-11 with Fran (secunda session 134c8417). Builds
on statuae (`project-specs/statuae-spec.md`) and mundus
(`project-specs/mundus-spec.md`), both in secunda and not in main.

*The question (Fran, 2026-11-11): "what kind of tooling might we be
able to make that would allow you to potentially do modeling yourself
of at least static assets? like assume some kinds of conversion
stuff, ability to see previews, maybe procedural texture generation
tools".*

Names: the family is `plastica` (Latin for the art of modelling;
Claude's proposal; the word is free in include/, lib/, apps/ and
tools/). All file and function names here are proposals.

## I. Intent

1. **Claude makes static 3D assets from text.** A prop is a short
   description file listing simple solids; a tool turns it into the
   same kind of model the rest of the stack already draws.
2. **Claude can see what it made.** One command gives a sheet of
   views to look at, so a prop is revised by looking, not by hoping.
3. **Fran sees every asset in the model viewer**, each on its own in
   the focused, turning view, "not just as background on the scene".
4. **The clearing gets furnished.** The props stand in the place
   Fran walks; he bumps into them and walks behind them.
5. **A library, not a mundus feature.** Fran: "we might use them in
   many contexts so i think we could almost treat that as a module".
   The assets and their tools know nothing about mundus.

Non-goals (this stage): characters and anything skinned; animation
of any kind; reading Blockbench (`.bbmodel`) or OBJ files; an editor
in a pane; cutting one solid out of another; building whole levels
from tiles; buying models (the $10 of Retro Diffusion credit is on
hold).

Anti-goals: a prop that reads well alone but turns to mush once
dithered in the scene; assets or tools that depend on mundus; a
build that gives different bytes from the same source; outside
libraries; any change to the pinned digests of the renderer and the
dithers; a review queue in which Fran must approve each prop.

## II. Decisions

Fran's, as he made them (2026-11-11):

1. **Order:** the in-house stack first; bought models later, for
   what Claude turns out to be bad at.
2. **Autonomy:** Claude decides which props and environment pieces
   to make and where they stand, checks its own previews, and
   revises. "most of the time whatever you make will be good for us
   to use as is".
3. **Review:** Fran checks in "to see the inventory of things you've
   made, see the integrated new environment"; feedback is by
   exception.
4. **Viewer:** each asset must be visible in the model viewer in a
   focused view.
5. **Module:** the asset library is its own module, usable in many
   contexts.

Fran accepted these when they were played back as Claude's guesses:

6. Description files and the layout live in the repo and are
   committed like code.
7. A tool builds them into his live volume
   (`~/.rhubarb/paginae-proba.volumen`) whenever something new is
   made, without asking each time.
8. Props are a third kind in the volume, beside character and place,
   so `$models` can show them as a group and `$world` does not offer
   a well as a character.

Claude's calls (not discussed; change freely):

9. **Where things live.** `plastica/` at the top of the repo holds
   the assets (`plastica/res/<name>.stml`). The C libraries go in
   `lib/` and `include/` like statua and mundus, so vicus and the
   test runners find them with no hand-kept lists.
9a. **The line between plastica and mundus** (settled after the
   builder check). Plastica is: the description, the textures, the
   compiler, the sheet of an asset on its own, and building assets
   into a volume as props. It links statua and the dithers, never
   mundus. Everything that knows about a place is on the mundus
   side: the layout files (`apps/mundus/loci/<place>.stml`), joining
   props into a place, the picture of an asset standing in the
   clearing, and the walk-map tests. The writer and the joiner are
   statua-level and neutral.
10. **The volume stores built GLB, not the description.** Everything
    downstream (viewer, world) already reads GLB from the volume;
    the description stays in the repo. This needs a GLB writer.
11. **A furnished place is one model.** The build tool merges the
    level and its placed props into a single GLB whose prop parts
    carry the same `walkable` / `collidable` marks the level uses,
    and stores that as the place. Mundus then needs no new concept:
    its walk map and its ground squares read the marks as they do
    today (mundus spec, decision 17).
12. **The furnished clearing replaces `clearing`** in the live
    volume under the same title. The bare level stays in the repo as
    the fixture `probationes/fixa/statuae/planum.glb`.
13. **Fixed texel density:** one constant, textures drawn at so many
    texels per unit on every face, so nothing is stretched. The
    value is chosen to match the kit (see AUDIENDA 2).
14. **Textures are generated, with seeds.** Small hand-written
    textures (character grids with a colour legend) are allowed up
    to 16 x 16.
14a. **How a prop blocks.** The walk map samples cell centres, 0.25
    units apart, and upright triangles cover nothing, so a thin post
    or rail can be walked through. A prop therefore blocks through a
    marked solid whose top covers at least two cells each way (0.5
    units). Where the visible shape is thinner than that, the
    description may carry an unseen blocker: a solid whose material
    is fully cut out (alpha below the cut-out limit), which `Statua`
    can already express. The place build fails if a placed prop that
    has a solid mark blocks no cell.
14b. **Part names stay out of `Statua`.** The compiler returns the
    model and, beside it, a table of part names per node; the sheet's
    legend reads the table. `Statua` does not grow.
14c. **The build never removes.** It cannot tell its own props from
    ones added by hand, so it only adds and replaces, and prints the
    props in the volume that have no source. Claude removes stale
    ones with `bin/statuae removere`.
14d. **Order of arrival.** A kind the running vicus does not know
    reads as a character (`lib/statuae_repositorium.c:133`). The
    third kind must be in the vicus Fran runs before the first build
    into his live volume.
15. **First assets:** well, fence section, two boulders, pine, oak,
    bush, signpost, crate, and a stair that reaches the top plateau.

## III. What exists already (checked 2026-11-11)

1. `Statua` (`include/statua.h`) is a plain value: nodes, meshes,
   materials, textures, clips, flat arrays. A compiler can fill one
   directly and call `statua_finire`.
2. Node marks exist: `STATUA_SIGNUM_GRESSIBILE`, `_SOLIDUM`, `_AQUA`
   (`include/statua.h`), read from `extras.tags` by the GLB reader.
3. `bin/statuae pingere` renders any GLB to a PNG at a given turn,
   tilt and dither; Claude can read the PNG.
4. The volume stores a model as its original bytes with a title, a
   kind (`STATUAE_GENUS_STATUA`, `_LOCUS`) and `"forma": "glb"`
   (`include/statuae_repositorium.h`).
5. There is a GLB reader and no writer. `json.h` and `imago_png.h`
   (with a PNG encoder) exist.
6. Retro Diffusion describes its "detailed" style as "boxes,
   cylinders, spheres, extrusions" with pixel textures: the same
   vocabulary as section IV.1.

## IV. Design

Each part is a library with one job; later parts use earlier ones.

1. **The description (`plastica`).** An STML file: a tree of groups
   and solids. Solids: box, cylinder (with a top radius, so cones
   and tapers), sphere (with a jitter seed, for rocks), wedge,
   extruded outline, lathe (a profile spun round the upright axis).
   Each has position, turn, size, a texture or flat colour, and an
   optional mark (walkable, solid). Groups carry a transform and
   three repeaters: mirror, repeat along a line, scatter with a
   seed. `plastica_legere(textus, piscina)` gives a finished
   `Statua` or a refusal with a plain reason and a line number.
   Unit: one unit is the world unit of mundus; a character is 2
   units tall. The ground is y = 0 and every asset sits on it.
2. **Textures (`plastica_textura`).** Named generators (bark,
   planks, stone, brick, thatch, leaves, moss, dirt, plain speckle),
   each a pure function of size, seed and two or three colours. UVs
   are laid per face from the fixed texel density (decision 13).
3. **The preview sheet.** `bin/plastica specimen <res.stml> -o
   <png>`: front, side, top and three-quarter views on one sheet,
   in three renderings: raw colour, dithered with exemplaria and
   with Atkinson, and parts in flat colours with a legend. The
   sheet also prints numbers: bounds, triangles, lowest point,
   footprint in walk cells, marks. The picture of the asset
   standing in the clearing beside the mushroom man is a mundus-side
   command (`bin/mundus specimen`, decision 9a).
4. **The GLB writer (`statua_glb_scribere`).** `Statua` to bytes
   that `statua_glb_legere` reads back to an equal `Statua` (equal
   means: read, write, read again, compared with the first read -
   the reader drops meshes outside the default scene and decodes
   textures to RGBA):
   geometry, materials, textures as PNG, node marks as
   `extras.tags`. Clips are written too, so the level round-trips.
5. **Joining (`statua_coniungere`).** Append one `Statua` to
   another at a placement (position, turn about the upright axis).
6. **The third kind.** `STATUAE_GENUS_RES` in the repository;
   `bin/statuae addere -res`, `enumerare -res`. `$models` gains a
   group switch between characters and props; props show in the
   same focused, turning view. The character chooser of `$world`
   stays characters only.
7. **The layout (mundus side).** `apps/mundus/loci/clearing.stml`:
   the level's fixture path and a list of placements (asset, x, z,
   turn). The height is taken from the level's walk map under the
   spot.
8. **The builds.** `bin/plastica condere -volumen V` compiles every
   asset and writes each as a prop into the volume.
   `bin/mundus condere -volumen V` joins each layout into its place,
   checks decision 14a, and writes the place. Same sources give the
   same bytes; nothing is written when nothing changed (the
   repository already skips identical bytes). Neither removes
   anything (decision 14c).

## V. How it is tested

1. Compiler: each solid's triangle count, bounds and marks pinned;
   each refusal has a test; repeaters with a seed are deterministic.
2. Textures: digest of each generator at a fixed size and seed.
3. Writer: write then read gives an equal `Statua` for a made
   asset, for `fungus.glb` and for `planum.glb`; hostile input is
   the reader's business and stays so.
4. Joined place: the walk map of the furnished clearing differs
   from the bare one only under the placed props, and the stair
   makes the top plateau reachable (today's pins: 60626 walkable,
   42881 reachable).
5. Tools: a smoke script each for `bin/plastica` and `bin/mundus`,
   like `tools/statuae_fumus.sh`. A thin post with no blocker makes
   the place build fail; with one it passes and the cells block.
6. Every gate is proved red once by a compiling plant.
7. By eye: Claude looks at every sheet before an asset is built
   into the volume; Fran looks when he likes.

## VI. Left to the builder

The exact STML element and attribute names; whether a wedge is its
own solid or an extrusion; the generators' parameters; the sheet's
layout; how `$models` shows the group switch; the task order (the
suggested order is IV.1 with flat colours, IV.3, IV.2, IV.4, IV.6,
IV.5 + IV.7 + IV.8, then the first assets). A builder check by a
fresh reader (2026-11-11) raised decisions 9a and 14a-14d; all four
were settled by Claude, none by Fran.

## VII. Later, for orientation only

A Blockbench reader, to study and edit the bought models; rigid
motion (a door, a windmill, a bobbing pickup) as keyframes on
groups; own tiles and whole levels; projectiles and pickups for the
mobs stage.

## AUDIENDA (not verified)

1. Whether Claude's props look good enough is the open question of
   the whole design. The first asset and its sheet answer it.
2. The kit's texel density has not been measured.
3. Read in the code, not run: the walk map skips every node without
   a mark (`_percurrere`, `lib/mundus_solum.c`), so a tree's
   unmarked crown over grass should leave the grass walkable and
   only the marked trunk should block. No test shows it yet.
4. Depth is kept per pixel and a body pixel is drawn only where it
   is nearer (`lib/mundus_pictura.c`), so a character behind a pine
   should be hidden. Read in the code; never exercised with a tall
   thin object.
4a. That the renderer cuts out a material whose base colour alpha is
   below the limit with no texture (the unseen blocker of 14a) has
   not been tried.
5. `planum.glb` has 12 textures; whether any is JPEG, and what
   re-encoding as PNG does to the file size, is not measured.
6. The world pane re-reads its lists when the volume changes
   (`lib/mundus_latus.c`, `volumen_seq_ultimum`); whether the place
   it has open reloads when replaced under the same title has not
   been traced.
7. Titles are unique across kinds, so a prop may not share a title
   with a character or place already in the volume.
