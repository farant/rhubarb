# statuae — a vicus pane for animated low-poly models

Status: APPROVED by Fran 2026-11-09; being built from
`project-specs/statuae-plan.md` (T1 stereometria committed
23c5ae1d7). Brainstormed 2026-11-06 .. 2026-11-09 with Fran (secunda
session 134c8417).

*The question (Fran, 2026-11-06): "a 3d animated panel as a view" in
vicus that shows his low-poly models, lets him click-drag to rotate
them and switch between a model's animation cycles, with the in-house
dithering applied to every frame the way pictor applies it to a pasted
picture. Which dither looks good in motion is not known: "a bit of an
experiment".*

Names: the model family is `statua` (Fran, 2026-11-09). The first
drafts said `simulacrum` and `spatium`; both words were already taken
in the house (`simulacrum` is a test double, probationes/
horreum_simulacrum.h; `spatium` is the component tree's namespace), so
the model is `statua` and the space math is `stereometria`. The other
file names in VI are Claude's proposals.

## I. Intent

1. **See the models move, dithered.** A pane shows one model at a time,
   playing one of its animation clips in a loop, drawn in the 16-colour
   palette.
2. **Turn it by hand.** Dragging rotates the model the way a person
   expects.
3. **Compare dithers live.** A dropdown switches the dither while the
   model keeps moving. The comparison is the point of the first
   version: Fran decides by eye which modes are worth keeping.
4. **New models without a restart.** Fran names a file; a command
   stores it; the running vicus shows it.

Non-goals (first version): skinned (bending-mesh) models; zoom and pan;
ground and shadow; dither patterns that stick to the surfaces; formats
other than GLB; editing or exporting anything; sound; morph targets,
cameras and lights from the file; a paged dropdown (see II.9);
computing frames off vicus's loop (see II.8a).

Anti-goals: a GPU path (the picture must exist as raw pixels for the
dither); outside libraries; a loader that only understands PixMesh's
exact output and crashes on any other GLB (it may refuse a file, with
a reason, never crash); an animation that slows down when the dither
is slow (it drops frames instead); a pane that keeps the machine awake
when it is not visible or not moving.

## II. Decisions

Fran's, as he made them:

1. **The picture fills the pane.** Controls sit in one header row at the
   top: model name with previous/next arrows and a count (◂ 1/2 ▸), a
   clip dropdown (`walk ▾`), a dither dropdown.
2. **Rotation** is "what you would intuitively expect" from a drag.
3. **Two looks:** flat (texture colours as painted) and a simple
   lighting setup.
4. **Background:** bare paper.
5. **Dither dropdown:** Atkinson, Bayer, Exemplaria, None. Bayer is
   the ordered dither 3D games use (Obra Dinn) and is new code.
6. **File formats** are handled by modules, one per format, producing
   one shared in-memory model. GLB is the first and only one now.
7. **Adding a model:** Fran gives a path; no recompiling or restarting
   vicus.
8. **Resolution:** draw at the pane's own resolution; Fran is open to a
   lower resolution shown enlarged if exemplaria needs it.

Confirmed by Fran after the builder check (2026-11-09): decisions 9
(reuse the popup menu), 20 (models in the volume) and 23 (the two
exports as fixtures), and:

8a. **A slow dither may stall vicus, for now.** The pane computes its
    frame inside its own tick on vicus's one loop. Under exemplaria
    that holds the loop about 76 ms per frame while the pane is
    visible and the clip plays, and the pane beside it lags by that
    much. Accepted for the first version. A child process that
    computes frames is the later answer if exemplaria proves worth
    keeping in motion; it is not in this design.

Claude's calls (2026-11-09), for Fran to check:

9. **The dropdown reuses the existing popup menu** (`include/elenchus.h`:
   click, j/k and arrows, Enter, Esc already work, and pictor's ⋯ menu
   already hangs one under a button). The new shared piece is only the
   dropdown button (label + ▾, opens the menu under itself, shows the
   current choice). Earlier in the brainstorm the dropdown was to be a
   new widget with pages; the lists here have at most four entries, so
   paging is left for the first list that needs it.
10. **The header is drawn by the pane.** vicus has no host title line
    (it draws only the tab row); panes such as admonitiones and
    antiphonale draw their own rows with clickable arrows. If the
    controls do not fit one row at the narrow pane width (360 px on
    Fran's display), the dropdowns move to a second row. (T8: at
    360 px both fixtures fit one row; two rows start near 200 px.)
11. **Turntable rotation.** Horizontal drag spins the model about its
    vertical axis without limit; vertical drag tilts the view between
    -85 and +85 degrees, so it never flips. Releasing leaves it where
    it is: no inertia, no auto-spin. 1 pane pixel = 0.5 degree.
12. **Rotation survives a clip change and resets on a model change** to
    a front three-quarter view (30 degrees to the side, 15 degrees
    above). `r` resets by hand.
13. **The clip keeps playing during a drag.** Space pauses and resumes.
14. **Lit look:** one directional light from the upper left of the
    view plus a constant fill, so the darkest face keeps about 35 % of
    its colour. One brightness per triangle (faceted). The light is
    fixed to the view, not to the model. A material with any emission
    (a non-zero emissive factor, with or without an emissive texture)
    ignores the light and is drawn in its base colour at brightness 1.
    The golem's eyes are this case: emissive factor 1,1,1 and an
    emissive texture that is the base texture itself. A separate
    emissive texture is not sampled; the model keeps one texture per
    material and a flag. Flat look: brightness 1 everywhere. The look is a header toggle (`lit` / `flat`) and a
    key.
15. **Framing:** the model is centred and scaled so that the bounding
    sphere of its **rest pose** fits the picture with a small margin,
    at any rotation. Mild perspective (30 degrees field of view).
    (Changed 2026-11-09 in T4, Claude's call: it first said "over all
    clips", but the golem's `boulder_throw` sends the boulder almost 8
    units from a 2-unit golem, which made the golem tiny. A part that
    a clip moves far away now simply leaves the picture.)
16. **Every dither runs at pane resolution** (see III: the pane is
    already about 360x450, where exemplaria measured 76 ms). A header
    toggle `2x` renders at half size and shows each pixel doubled, for
    any dither; it is there for the experiment and for exemplaria's
    frame rate.
17. **Paper stays clean.** Pixels the model does not cover are paper
    after dithering, whatever the dither did near the edge (error
    diffusion must not speckle the background).
18. **"None"** maps each pixel to its nearest palette colour.
19. **All 16 colours are active.** No palette presets in this version.
20. **Models live in the vicus volume:** the original file bytes as a
    content-addressed blob, plus one act recording title and seal (the
    way pictor records a pasted image). Removing a model is another
    act. The pane lists models in the order they were added.
21. **The pane notices new models by polling** the volume's sequence
    number once a second while visible, as the negotia pane does.
22. **Remembered per pane:** which model, which clip, which dither,
    look, `2x`, paused, and the two rotation angles.
23. **The two GLB files become test fixtures** under
    `probationes/fixa/statuae/` with short names (`lapis.glb`,
    `fungus.glb`; 196 KB and 107 KB). They are Fran's PixMesh exports.

## III. What was measured and found (2026-11-09)

The two files (`a-low-poly-stone-golem-composed-of-stack.glb`,
`a-friendly-mushroom-man.glb`, untracked in the secunda root):

| | golem | mushroom man |
|---|---|---|
| size | 196 476 bytes | 106 864 bytes |
| nodes / meshes / skins | 54 / 30 / 0 | 42 / 24 / 0 |
| primitives | 40, triangles, 16-bit indices | 29, same |
| attributes | POSITION, NORMAL, TEXCOORD_0 | same |
| clips | `walk` 1.4 s, `boulder_throw` 1.6 s | `walk_bouncy` 1 s |
| channels | translation, rotation, scale | same |
| interpolation | LINEAR 138, STEP 2 | LINEAR 54 |
| texture | one embedded PNG, 256x256 | one embedded PNG |
| sampler | nearest, clamp to edge | same |
| materials | matte, non-metal; 3 with alpha MASK 0.5; eyes emissive | matte |
| lights, cameras | none | none |

- The golem's texture is surface detail only (cracks, moss); no light
  direction is painted in. The mushroom man's has light-to-dark bands
  on the cap and darker cloth edges, but no lit side. Both expect the
  viewer to supply light.
- vicus pixels are screen points divided by two (`apps/vicus/vicus.c`
  `SCALA II`), so the pane is already chunky. On Fran's display
  (2880x1864, 1440x932 points) the window is about 720x466 and the
  left pane about 360 wide by 450 high (`lib/vicus.c:196-210`). The
  600x751 used in the exemplaria speed work was the test image's size,
  not a pane's.
- Dither cost per frame at 360x451 (exemplaria-velox.md, VIII):
  Atkinson about 4 ms, fast exemplaria 76 ms (13 fps). At half size
  exemplaria should be near 20 ms (estimate from the size scaling,
  not measured).
- Exemplaria in motion: moving a picture by one pixel changed 17-22 %
  of its output, so it may shimmer. Unknown until real frames exist.
- vicus has no fixed frame rate. A pane asks for its next tick
  (`VicusFacies.mora`) and reports whether anything changed
  (`pulsare`); the window is repainted on the CPU only then. Only
  visible panes are ticked. Nothing in vicus animates every frame yet;
  antiphonale ticks at 33 ms while sound plays.
- A pane draws pixels by naming an RGBA `Imago` (`mandata_imago`,
  resolved through `VicusFacies.fons`); the rasterizer already repeats
  pixels for an integer scale (`componens_ponere_transformatio`).
- Drags reach a pane as mouse down / move / up events with capture;
  `lib/aetherophonum.c:473-525` is the working pattern.
- The house has JSON (`include/json.h`), PNG decoding
  (`imago_caricare_ex_memoria`) and base64, but **no floating-point
  3D math** (`matrix.h` and `quaternio.h` are exact algebra).
- Prior art: `../solarium/glb.c` (917 lines) and `skel.c` (113 lines),
  C11 and English on its own JSON, stb_image and a GPU layer. It is a
  reference for the format's corners, not code to copy.

## IV. Design

**The parts, and what each one knows.**

1. *Space math.* Three- and four-component float vectors, 4x4
   matrices, quaternions with slerp, compose and invert for
   translation-rotation-scale. Pure functions, f32. Knows nothing
   about models.
2. *The model* (`statua`). One format-neutral value in an arena:
   a node tree (parent index, rest translation / rotation / scale),
   meshes as flat arrays (positions, normals, texture coordinates,
   triangle indices, one material per run of triangles), materials
   (base colour texture, alpha cutoff, emissive flag, two-sided flag),
   textures as RGBA, and clips (name, duration, channels of key times
   and values with linear or step interpolation). Plus the pose
   function: model, clip, time -> one world matrix per node (clip time
   wraps; rotations slerp; a node without a channel keeps its rest
   value). Knows nothing about files or pixels.
3. *The GLB reader.* Bytes -> model, or a refusal with a reason in
   plain words. It reads what the table in III lists, and the nearby
   cases a GLB may legally contain: 8- and 32-bit indices, missing
   normals (computed per face), a missing texture (white), several
   scenes (the default one), `KHR_materials_emissive_strength` and
   other unknown optional extensions (ignored). It refuses: skins,
   morph targets, required extensions it does not know, external
   buffers or images, non-triangle primitives, CUBICSPLINE channels,
   sparse accessors, and any offset or count that points outside the
   file.

   The rule for everything else a GLB may legally hold (Claude's
   call): **show it approximately rather than refuse**, as long as
   the shape and the painted colours are right. So: a base colour
   factor multiplies the texture; repeat and mirror wrap are
   honoured; integer texture coordinates are normalized; embedded
   JPEG is decoded like PNG; several textures are fine (one per
   material); alpha BLEND is treated as cutout at 0.5; linear
   filtering is drawn nearest; vertex colours, second coordinate
   sets, and metal / roughness / normal / occlusion maps are ignored.
   The model therefore carries a colour factor and a wrap mode per
   material.
4. *The renderer.* Model + per-node matrices + view (two angles, size,
   look) -> an RGBA picture and a coverage mask. Depth buffer,
   back-face culling unless the material is two-sided,
   perspective-correct nearest texture sampling, alpha cutout, one
   brightness per triangle, no anti-aliasing (hard edges suit both the
   pixel textures and the dithers). Deterministic: the same inputs
   give the same bytes.
5. *Bayer dither.* A new function beside Atkinson in the dithering
   library with the same shape of signature (RGBA, size, active
   colours, arena -> indices). 8x8 grid anchored to the picture's
   origin, from the same pattern source exemplaria uses. Per pixel it
   makes a plan of 16 palette colours whose average approximates the
   pixel and lets the grid choose one. (Built in T5; a two-colour
   version was tried first and looked garish on this palette.)
6. *The store.* Add, remove and list models in a volume (decision 20).
7. *The command* (`bin/statuae`). `addere <file> [-titulus <name>]
   [-volumen <path>]`, `removere <title>`, `enumerare`. `addere` runs
   the GLB reader first and refuses a file the pane could not show.
   The title defaults to the file name without its extension. Without
   `-volumen` it refuses to guess: Fran's live volume is written only
   when he names it.
8. *The pane.* Header row, picture, popup menus; state (decision 22);
   ticking; the frame pipeline below.

**A frame.** time -> pose -> render (at pane size, or half with `2x`)
-> dither to indices -> uncovered pixels to paper -> indices to RGBA
-> shown (doubled if `2x`). A frame is recomputed only when its inputs
changed: clip time, angles, model, clip, dither, look, size.

**Time.** Clip time comes from the clock, not from a frame counter:
with a 76 ms dither the walk still takes 1.4 s and simply shows fewer
poses. While playing or dragging the pane asks for a tick every 33 ms
(30 frames a second at most). Paused and not dragged, or showing a
model with no clips, it asks for none and only the one-second volume
poll remains. A hidden pane is not ticked at all (vicus does this).

**Controls.** Everything has a mouse path and a key.

| action | mouse | key (builder's proposal) |
|---|---|---|
| rotate | drag on the picture | h / l, j / k in steps |
| reset view | - | r |
| previous / next model | header arrows | [ / ] |
| choose clip | clip dropdown | c opens it |
| choose dither | dither dropdown | d opens it |
| lit / flat | header toggle | f |
| 2x | header toggle | 2 |
| pause / resume | - | Space |

Inside an open dropdown j / k, Enter and Esc belong to the menu.

**Empty and broken states.** No models: the picture area says so and
names the command. A stored blob the reader refuses (for example
written by an older version): the pane shows the model's title and
the reason, and the arrows still work. A model without clips shows
its rest pose and a disabled clip dropdown.

**Budget.** At 360x450 with the golem: pose + render at most 5 ms,
Bayer at most 5 ms, so Atkinson, Bayer and None hold 30 frames a
second and exemplaria is limited by its own 76 ms. The numbers are
targets to measure against, not gates.

## V. How it is tested

- Space math: known identities (compose then invert, slerp end points
  and half way, a rotation of 90 degrees about each axis).
- Reader: both fixtures load with the counts in III; every refusal in
  IV.3 has a small hand-made or truncated file that triggers it;
  truncating a fixture at every one of a spread of lengths never
  crashes.
- Pose: at time 0 and at the clip's end the matrices agree (loop);
  a STEP channel holds; an unanimated node keeps its rest matrix.
- Renderer: a hand-made one-triangle and two-triangle model with
  exact expected pixels (coverage, depth order, cutout, culling);
  digest pins of the golem at fixed time and angles, flat and lit.
- Bayer: a flat mid-grey between two palette colours gives the
  checker ratio; shifting the picture by 8 pixels shifts the output
  by 8 pixels; digest pin on a fixture.
- Store and command: add, list, remove against a temporary volume;
  a second handle sees the addition through the sequence number.
- Pane: mounts in a test vicus with a temporary volume holding the
  fixtures; drag changes the angles; dropdown choices change state;
  paused pane asks for no tick; a model added through the store
  appears after a poll. A screenshot (`-imago`) for Fran's eye.
- Every gate is proved red first by a compiling plant, as usual.

## VI. Left to the builder

- The names: proposed `include/stereometria.h` (space math),
  `statua.h`, `statua_glb.h`, `statua_pictura.h`
  (renderer), `statuae_repositorium.h` (store),
  `dithering_bayer_colorum` in `dithering.h`, `tools/statuae.c`,
  `apps/statuae/organum.stml`, pane genus `statuae`, opened as
  `$models`. All must pass the vocabulary lint.
- How Bayer picks its two colours and the mixing ratio.
- The exact act and record shape in the volume, after reading how
  pictor and negotia write theirs.
- Light direction and fill values, within decision 14.
- Whether the header fits one row (decision 10).
- The order of the work. Suggested: space math, model + pose, reader,
  renderer (with a command-line PNG dump so frames can be judged
  before any pane exists), Bayer, store + command, dropdown button,
  pane.

## AUDIENDA (not verified)

- The pane size in III is derived from the code and the display's
  resolution, not read from a running vicus.
- ~~Whether the volume's sequence number moves for the store's
  records.~~ Answered in T6: it does, and a second handle on the same
  file sees it (probatio_statuae_repositorium, section "seq").
- The cost of vicus repainting the whole window 30 times a second
  with a full-pane image. No pane has done this yet.
- That the renderer meets 5 ms; nothing has been rasterized.
- Exemplaria's look in motion, and Atkinson's on real rotation.
- Whether texture rows in these files run top-down as glTF says (a
  flipped texture shows at the first rendered frame).
