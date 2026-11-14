# graphis — 2D pictures that Claude draws from text, and a first still-picture adventure

Status: BUILT 2026-11-13 in secunda, tasks 1-9 of
`project-specs/graphis-plan.md` (section VII says what was built and
where it differs from the decisions below). Not in main. Slice,
names and pane layout agreed by Fran 2026-11-13 ("yes, that sounds
good"); builder check done 2026-11-13 (section VI).
Brainstormed 2026-11-13 with Fran (secunda session 134c8417). Sibling
of plastica (`project-specs/plastica-spec.md`); meets mundus
(`project-specs/mundus-spec.md`) only at a doorway.

*The question (Fran, 2026-11-13): mundus should grow "mini-engines
for different kinds of games"; for adventure "i would probably start
with myst style / world builder style", later a mode "more like ff7
style ... static backgrounds but potentially in perspective somehow
... with a low poly 3d character moving through".*

Names: `graphis` (Latin for the art of drawing) is the picture
library; `scaena` is the still-picture adventure mode. Both are
Claude's proposals and free in include/, lib/, apps/ and tools/.
All file, element and function names here are proposals.

## I. Intent

1. **Claude draws still pictures from text.** A picture is a short
   description file: a tree of flat shapes with colours, patterns
   and lights. A tool turns it into a 16-colour picture.
2. **Claude can see what it drew.** One command gives a sheet to
   look at, so a picture is revised by looking.
3. **A picture knows its parts.** Shapes with a title are click
   regions; the tool gives the region map with the picture, so the
   two can never disagree.
4. **Fran can walk into one.** A first adventure: the hut in the
   clearing, as still pictures with things to click and a way out.
5. **A library, not an adventure feature.** graphis knows nothing
   of scaena, mundus or vicus.

Non-goals (this stage): a character in the picture; a depth or walk
layer; animation (fire flicker, palette cycling); inventory, state,
puzzles, saved games; an editor in a pane; a general framework for
game modes; a black-and-white variant; sound.

Anti-goals: copying any game's pictures, rooms or text; a picture
language as large as a programming language; a build that gives
different bytes from the same source; outside libraries; a format
that could not later take a depth value per shape; a review queue in
which Fran must approve each picture.

## II. Decisions

Fran's, as he made them (2026-11-13):

1. **Order of modes:** still pictures with click regions first
   (Myst / World Builder kind); the walked, perspective kind later.
2. **Colour is the primary style.**
3. **The look** is the right panel of
   `build/spike2d/sheet_d.png`: smooth lit colour and hand-chosen
   patterns mixed per shape by a mask, then one Atkinson pass. "the
   color image of sheet_d looks best for sure".
4. **The mask idea is his:** a value that says how much of each
   treatment a region gets, with greys as blends.
4a. **First slice:** the hut, two views, in a pane of its own; the
   doorway from `$world` last. Names `graphis`, `scaena`, `$scene`
   accepted.
4b. **The hut picture** from the spike may stay or change, as Claude
   judges.
4c. **Pane layout** (his picture of it): "the top half of the pane
   be the image and the bottom half be a text description".

Found by the spike (throwaway; `build/spike2d/`, not in git):

5. Hand patterns alone read as materials but are flat and heavy in
   gold and brown (`sheet_a.png`, left).
6. The exemplaria dither is the wrong tool for drawn pictures: it
   re-reads hand patterns as texture (`sheet_a.png`, right) and
   turns smooth gradients into rings (`sheet_b.png`, right).
7. Free colour plus lighting through Atkinson gives light and mood
   but loses materials (`sheet_e.png`, left).
8. Pattern at full strength everywhere is too busy (`sheet_e.png`,
   right). Strength must be chosen per shape.
9. Below about 0.4 on a dark surface a pattern dissolves into
   speckle.

Claude's calls (not discussed; change freely):

10. **One pass, one dither.** The mask blends the *source* colour of
    each pixel between the shape's flat colour and its pattern
    pixel; lighting multiplies the result; Atkinson
    (`dithering_atkinson_colorum`, `include/dithering.h`) runs once
    over the whole picture. No second dither, so no seams.
11. **Description is STML**, like plastica. Elements: `rectangulum`,
    `ovum`, `polygonum`, `linea`, `grex` (group with offset),
    `lumen` (light), `usus` (place another description here).
    Later shapes paint over earlier ones.
12. **A shape carries:** a colour or a colour pair (palette index or
    free `#rrggbb`); a pattern by its exemplaria name
    (`include/exemplaria.h`); a strength 0 to 1; an outline (width,
    colour); `lucens` for things that give light and take none
    (window, flame); an optional title.
13. **Lights are in the description**, not in code: ambient level
    and tint on the root; point lights (place, radius, tint,
    strength); area lights (a polygon, soft edge). Falloff is by
    distance in the picture. A depth value per shape can refine
    this later without changing the format.
14. **Same source, same bytes.** Lighting and blending in integer
    fixed point; no floating arithmetic in the path to the picture
    (as `plastica_textura`).
15. **One fixed size: 360 x 240, shown at 1x.** The pane surface is
    360 x 450 by default (`lib/mundus_latus.c:27-28`), so the
    picture fills the width and takes the top 240 rows under the
    12-row header; about 198 rows (some 24 lines of 60 characters)
    remain for text. That is Fran's top half / bottom half. A
    second size waits for a second need.
16. **Outputs of the compiler:** the palette picture (one index per
    pixel), the owner map (which titled shape owns each pixel, 0 =
    none), and the table of titled shapes. Titles only: text, exits
    and behaviour belong to scaena.
17. **The linter refuses**, with the line: unknown element,
    attribute, pattern or colour; a shape wholly outside the frame;
    a titled shape that owns fewer than 64 pixels after painting; a
    duplicate title; a strength between 0 and 0.4 exclusive (finding
    9; 0 means smooth).
17a. **A group with a title is one region** (added in plan T5,
    2026-11-13, when the hut's regions came out full of holes):
    everything painted inside `<grex titulus="...">` belongs to
    it. Shapes inside carry no title of their own.
18. **Reuse is by file.** `graphis/res/<name>.stml` holds pieces (a
    chair, a jar) that a picture places with `usus`. No variables,
    loops or conditions in the format.
19. **Where things live.** Library in `lib/` and `include/`
    (`graphis.h`), tool `bin/graphis`, pieces in `graphis/res/`.
    scaena: `lib/scaena.c`, layouts in `apps/scaena/loci/`, the pane
    with the other vicus panes.
20. **A scene file is separate from its picture.** It names the
    picture and gives the view's description and, per title, the
    text shown on click and
    optionally an exit (another scene, or back to the world). A
    picture can then be reused and checked alone.
21. **The volume stores the built scene** (picture, owner map,
    texts, exits), as plastica stores built GLB. Under the standing
    OK of 2026-11-11 the build tool writes into Fran's live volume
    without asking each time.
22. **First slice:** the hut, two views (hearth wall; door wall),
    each view an exit to the other, the door an exit out. A pane
    opened with `$scene` shows the picture in its top half and text
    in its bottom half (decision 4c).
22a. **What the text half holds.** Each view has a description, a
    short paragraph, always shown first. Under it, the text of the
    thing last clicked; a new click replaces it; a new view clears
    it. The text wraps by word in the pane's small type. In a pane
    wider or taller than the default the picture stays 360 x 240,
    centred in its band; the text takes the rest.
22b. **Finding the regions.** Moving the pointer over a region
    names it in the header row (its title); no pointer change is
    needed for the slice.
23. **The doorway from `$world` is a follow-up, not in this
    slice** (changed after the builder check). The furnished place
    is one merged GLB and the layout stays in the repo, so a door
    position has no carrier to the running pane, mundus has no
    trigger concept, and no pane opens another today. It needs its
    own small design. In the slice the hut door is a region with
    text and an exit named `mundus` that the pane shows as text
    only ("the clearing is outside").
23a. **Scenes are their own store in the volume:** act kinds
    `scaena-addita` / `scaena-remota`, one blob per scene, apart
    from statuae (whose index reads only its two act kinds,
    `lib/statuae_repositorium.c:60-62`, and treats an unknown kind
    as a character, `:134`). No order-of-arrival hazard, no title
    clash.
23b. **Pattern names** are a new small table in graphis (the
    lowercase enum names of `include/exemplaria.h:21-61`, e.g.
    `latericium`, `textum`); exemplaria has ids only.
23c. **Strength word:** `vis` (plastica uses `textura` for a
    texture kind). Flat colour of a pair = mean weighted by the
    pattern's density, as in the spike.
23d. **Light shape.** A point light has two radii (the spike
    squashed the fire's falloff by 1.3 vertically); the root may
    darken toward the corners (`umbra`). These reproduce what the
    spike hard-coded.
24. **No mode framework.** scaena is a library with enter, step,
    paint and leave. What a second mode shares with it is decided
    when the second mode exists.

## III. Tools Claude needs (from the spike and from `../agi-stylist`, read only)

1. `bin/graphis specimen x.stml -o x.png`: one sheet with the flat
   source, the mask, the lit source, the final picture and the
   region map, at 2x.
2. `bin/graphis quadrum x.stml -o x.png -fines "x y w h"`: a crop at
   4x, for looking at one corner.
3. `bin/graphis iudicare x.stml`: the linter alone.
4. `bin/scaena condere -volumen V`: build every scene into a volume;
   adds and replaces, never removes; names a bad exit with its line.

## IV. Tests

- Pure-function pins: a small fixture picture, digest of the palette
  picture and of the owner map.
- Each refusal in decision 17, by a fixture that must fail with its
  line.
- Mask ends: a picture with full ambient light, no other lights,
  palette colours only and strength 1 comes out as the patterns'
  own pixels exactly (the dither has no error to spread); strength
  0 on a palette colour gives that colour everywhere.
- scaena: every exit in the committed scenes resolves; every title
  in a scene file exists in its picture, and the reverse.
- Pane: headless picture test (`-imago`) with a click on a region
  and on an exit.
- Gates are proved red first by compiling plants, as in plastica.

## VI. Builder check (2026-11-13, a fresh reader against the code)

1. A pane does get pointer movement with no button held
   (`lib/fenestra_macos.m:969-973` -> `EVENTUS_MUS_MOTUS`,
   `lib/destinatio.c:194` fills `des->punctum_locale`; used by
   `lib/negotia_visus.c:1322-1326`, `lib/kalendarium.c:2814-2820`).
   Decision 22b stands.
2. `mandata_imago` crops, it does not scale
   (`lib/delineare_mandata.c:190-193`): hence 1x (decision 15).
3. Atkinson is plain RGB, integer only, error `>> 3`
   (`lib/dithering.c:186-221`); the routine the spike called.
4. Refusals with lines: `PlasticaFructus {successus, error, linea}`
   (`include/plastica.h:149-159`), helpers in `lib/plastica.c:153-215`
   and `:454-481`, all static; `lib/mundus_locus.c:342` has its own
   copy. graphis copies the pattern.
5. Pane drawing: indices -> RGBA through `AQUINAS_PALETTE`
   (`lib/mundus_latus.c:589-601`), served by a `fons` callback
   (`:683-696`, wired at `lib/vicus_applicatio.c:3233`), drawn with
   `mandata_imago` (`:1038`); wrapped text is a static
   `textum_fractum_pingere` (`:918-962`, 6 x 8 cells) to copy.
6. `Scaena` and `ConformitasScaena` already exist as type names
   (`lib/delineare_mandata.c:175`, `include/eventus_conformitas.h:115`):
   scaena's own types need other names.
7. Left to the builder: which file and line a refusal inside a
   `usus` piece reports, and `usus` cycles (refuse); the layout of
   the scene blob; `$scene` opens on the first scene by title.

## VII. As built (2026-11-13)

Each task's own note is in `project-specs/graphis-plan.md`; worklogs
sit beside the lib files. In short:

1. **Compiler** (`lib/graphis.c`): `graphis_legere` turns a
   `<pictura>` description into palette indices 360 x 240 plus the
   owner table of titled regions. Shapes `rectangulum`, `ovum`,
   `polygonum`, `linea`, `grex`; attributes `colores`, `exemplar`,
   `vis`, `stilus`, `titulus`. Integer only.
2. **Lights** (`lib/graphis_lumen.c`): root `lux`, `tinctura`,
   `umbra`; `<lumen>` point or area, at most 16; `lucens` shapes.
3. **Pieces**: `<usus fons="name" locus="x y"/>`, piece files in
   `graphis/res/` with root `<pars>`; depth 8, cycles refused; a
   refusal names the piece and the line in it.
4. **Eyes** (`lib/graphis_specimen.c`, `bin/graphis`): `specimen`
   (sheet 1448 x 480: finished picture, source, mask, lit, regions),
   `quadrum` (a part at 4x), `iudicare`.
5. **The hut**: `apps/scaena/loci/casa_focus.stml`,
   `casa_ostium.stml`. Added to the description: a titled `<grex>`
   is one region for everything painted inside it (decision 17a).
6. **Scenes** (`lib/scaena.c`): a `*.scaena.stml` sheet holds
   `<prospectus titulus pictura>` with one `<descriptio>` and a
   `<res titulus [exitus]>` per region; every region needs a thing
   and every thing a region. A view becomes one blob (`SCAENA01`).
   Types are `ScaenaProspectus`, `ScaenaRes` (builder check 6).
7. **Store and tool** (`lib/scaena_repositorium.c`, `bin/scaena`):
   own act kinds, apart from models; `condere`, `removere`, `index`;
   a refused run writes nothing; `-volumen` has no default.
8. **Pane `$scene`** (`lib/scaena_latus.c`): header, picture,
   description, the last touched thing's text.

Where the build differs from the decisions above:

- An exit's line stays on screen in the view it leads to; stepping
  with `[` `]` or the arrows clears it.
- The pane asks for one tick a second to notice a rebuilt scene.
- `j` / `k` choose a thing and Enter touches it (no mouse needed).
- Text that does not fit is cut at the last whole line; no
  scrolling.
- Layout after Fran's first look: flat background inside a one-cell
  margin, one blank line under the picture, two cells of padding at
  the sides of the text, both texts in the normal text colour.
- Exits lead only to views in the same sheet, or to `mundus`
  (text only; the doorway is a follow-up).

Follow-ups filed in the ledger (2026-11-13): the doorway between
`$world` and `$scene` (design question), the walked perspective
mode, animation of a picture, a "room" helper.

## VIII. Wider picture (2026-11-14)

Fran, after walking into the hut from `$world`: "expanding the width
of the illustration so it matches the size of live fullscreen vicus
panes"; the target size is roughly constant, so no per-pane
rendering. His desktop is 1920 x 1200 points; vicus is that divided
by 2, in two panes of 480.

- **The picture is 480 x 320** (`GRAPHIS_LATITUDO`,
  `GRAPHIS_ALTITUDO`), wherever sections above say 360 x 240. One
  size; nothing is rendered per pane.
- **A sheet says which measure its coordinates are in.** No
  `latitudo`/`altitudo` on the root, or 480 and 320: plain pixels.
  360 and 240: an older sheet, drawn 4/3 larger. Anything else, or
  one without the other, is refused. (The header Fran approved had
  the default the other way round, "absent = older sheet"; built
  this way because some 75 test sheets and every future sheet are
  then plain pixels with no attribute, and only the two hut pictures
  and one fixture needed marking. One line to flip.)
- **How an older sheet is scaled.** The ABSOLUTE position (after all
  group and piece offsets) is multiplied and rounded to nearest; a
  rectangle's right and bottom are scaled as edges, so neighbours
  still meet; stroke thickness and lights (place, radii, area,
  softness, capped at 16) likewise. Patterns are never scaled.
  Pieces (`<usus>`) are read in the measure of the picture using
  them.
- **Scenes.** A view blob holds 480 x 320; one built before this is
  refused by its size and the pane says to rebuild it. Both hut
  views were rebuilt in Fran's volume.
- **Narrow panes.** A pane narrower than 480 shows the MIDDLE of the
  picture (cut equally on both sides; it used to show the left).
  Things in the cut part cannot be clicked but `j` `k` Enter reach
  them. On the laptop panel at its default (panes of 360) the hut's
  "turn" arrow is cut.

## AUDIENDA (not verified)

1. Whether the look holds once lights come from the description
   (the spike's fire, window patch and corner darkening were
   constants in code).
2. Whether the proposed element and attribute words pass the Latin
   lint and the glossary.
3. Integer lighting was not tried; the spike used floats. The look
   must be re-checked after the change.
4. The spike drew perspective by hand as polygons. Whether a "room"
   helper is worth having was not tested.
5. `../agi-stylist` was read, never run.
6. No search was made for every way one pane might open another.
