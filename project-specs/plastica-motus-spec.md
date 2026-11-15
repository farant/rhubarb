# plastica motus — figures with joints and motion, modelled from text

Status: APPROVED by Fran 2026-11-14 ("the draft of the spec looks
great, i think all those decisions make sense"). Nothing is built.
Plan: `project-specs/plastica-motus-plan.md`.
Brainstormed 2026-11-14 with Fran (secunda session 134c8417). Builds
on plastica (`project-specs/plastica-spec.md`), statuae and mundus,
all in secunda and not in main. Direction it serves: ledger vision
`01M7E9AY5VE4FX6CB2TFPH7Z8A` (mundus game modes; first game a simple
roguelike / action RPG).

*The question (Fran, 2026-11-14): "i guess the concerns are 'how hard
is it to author models', 'how hard is it to author animation' and
then implementation. we are not necessarily aiming for a 'higher
poly' modeling / animation approach than what we have but i think
within that quality level we do want reasonable flexibility in
quality for animation and stuff."*

Names: this stays inside the `plastica` family. `figura`, `motus` and
`articulus` are already libraries in `include/` (checked 2026-11-14),
so new files are `plastica_*`. All element, attribute and file names
here are proposals.

## I. Intent

1. **Claude makes creatures in-house.** A figure is a plastica sheet
   whose parts hang on named joints. The tool turns it into the same
   kind of model the bought characters are.
2. **Motion is cheap to write.** A cycle is a short list of named
   poses at times. Claude writes few numbers per pose and none per
   frame.
3. **Motion is shared.** A walk written once for a skeleton serves
   every figure built on that skeleton, whatever its proportions.
4. **Quality is a dial, not a rewrite.** A plain cycle is three or
   four poses; easing, delays, foot targets and squash can be added
   to the same cycle later without changing the figure.
5. **Claude can see the motion.** One command gives a strip of frames
   per cycle to look at, with numbers that catch the common faults.
6. **Nothing downstream changes to show a figure.** It appears in
   `$models` and can be chosen as the `$world` character as soon as
   it is built.

Non-goals (this spec): bending meshes (see II.3); cross-fade between
cycles, one-shot cycles and held items at run time (the combat spec
uses what this one carries, see IV.7); creature behaviour, health or
attack rules; an editor in a pane; reading the bought models' source
files; motion on props in a place (door, windmill).

Anti-goals: a format where a decent walk needs per-frame numbers; a
cycle that only fits the one figure it was written on; a figure that
reads well in the strip but turns to mush when dithered at world
size; feet that slide visibly at the world's walking speed; a build
that gives different bytes from the same source; any change to the
pinned digests of the renderer and the dithers; outside libraries.

## II. Decisions

Fran's (2026-11-14):

1. **Quality level:** not higher-poly than what we have; flexibility
   in animation quality within that level.
2. **Approach:** start with rigid parts, with the smoother option
   designed in ("that sounds like a good approach").

Found while writing this (changes what Claude said in the chat):

3. **The bought characters are already rigid.** The golem and the
   mushroom man have 54 and 42 nodes and no skin
   (`project-specs/statuae-spec.md` III); `Statua` moves nodes only
   ("Pelles (skins) non sunt", `include/statua.h:13`) and the GLB
   reader refuses skins. So rigid parts on joints IS today's quality
   level, and everything in this spec plays on the existing renderer
   unchanged. Bending meshes would need a new field in `Statua`, a
   new path in the renderer, the reader and the writer. That is a
   separate, later spec; this one only keeps the door open (IV.1).

Claude's calls (change freely):

4. **A joint is a named group.** It becomes one node; its origin is
   the pivot. Joints nest, and that nesting is the skeleton. Solids
   and plain groups inside a joint move with it.
5. **Skeletons are their own sheets** (`plastica/ossa/<name>.stml`):
   the joint tree with default pivots, the limbs (IV.4) and the
   cycles. A figure (`plastica/figurae/<name>.stml`) names its
   skeleton, may move any pivot (proportions), and hangs solids on
   joints by name. A figure may add or replace cycles.
6. **Cycles store turns, not positions,** except for the root joint,
   whose rise and fall is scaled by the figure's leg length. This is
   what makes a cycle fit differently proportioned figures.
7. **Left and right are a naming rule:** joints ending `_sin` and
   `_dex` are a pair. A pose can be declared as the mirror of
   another.
8. **Everything is baked at build.** Easing, delays and foot targets
   become ordinary linear keys in the model. The run time learns
   nothing new.
9. **Figures are characters in the volume,** not props, so `$models`
   lists them with the golem and `$world` offers them.
10. **Standard cycle titles** in English, because the world finds
    cycles by title (`"walk"`, `"idle"`,
    `include/mundus_pictura.h:145`): `idle`, `walk`, `attack`,
    `hurt`, `die`.
11. **Joint names and marked moments travel in the GLB** now, though
    nothing reads them until the combat spec: joint names as node
    names (the writer already writes them,
    `include/statua_glb_scriptor.h:59`), moments as `extras` on the
    animation. They stay beside `Statua`, not in it (plastica
    decision 14b).
12. **First skeletons and figures:** blob (a slime), biped (a small
    goblin-like creature), quadruped (a boar). In that order: the
    slime proves the pipeline and squash, the biped proves poses,
    mirroring and foot targets.

## III. What exists already (checked 2026-11-14)

1. `Statua` carries nodes with a parent, and cycles made of channels
   (move, turn, scale per node; linear or step; turns interpolated on
   the sphere) (`include/statua.h:28-52`). `statua_positura` gives
   every node's matrix at a time.
2. plastica makes one node per solid and already sets parents
   (`lib/plastica.c:729`). It writes no cycles.
3. The GLB writer writes cycles and node names; the reader reads
   cycles back (`include/statua_glb_scriptor.h:15-30`).
4. The world plays the first cycle whose title contains `walk` while
   moving and `idle` when still (`include/mundus_pictura.h:132-149`),
   at 4 units per second (`include/mundus.h:33`).
5. `bin/plastica specimen` renders a sheet of views of a prop;
   `bin/statuae pingere` renders any model at a turn and tilt.
6. solarium (`../solarium/skel.c`, 113 lines) has the same playback
   for skinned models on the GPU and no authoring. Nothing to port.

## IV. Design

1. **Joints (`<articulus titulus="genu_sin" locus="x y z">`).** A
   group that makes a node. Rest pose = the sheet as written. A
   later attribute on a solid (naming a second joint and a blend
   length) is where bending would attach; it is refused today with
   "not built yet", not ignored.
2. **Poses (`<positura titulus="contactus_dex">`).** A list of
   joints with a turn in degrees (`x y z`), optionally a scale, and
   for the root a move. Joints not listed stay at rest. A pose may
   start from another (`ex="..."`) and change a few joints, or be
   the mirror of another (`speculum="..."`).
3. **Cycles (`<cyclus titulus="walk" duratio="1.0"
   circulus="verum">`).** Keys: a time and a pose. A looping cycle
   closes itself (the first pose is repeated at the end). Dials, all
   optional:
   - `lenitas` on a key: linear, eased, or held.
   - `mora` on a joint: that joint runs so many seconds behind the
     rest (follow-through for arms, ears, tails).
   - `<momentum titulus="ictus" tempus="0.4"/>`: a named moment,
     e.g. the frame an attack lands.
4. **Limbs and targets.** A skeleton declares two-joint limbs (hip,
   knee, ankle; which way the knee bends). A pose may then say
   where the end of a limb is ("left foot at x y z") in place of
   two turns; the tool solves the two turns. A target out of reach
   is refused with the sheet line, not stretched.
5. **Baking.** Each cycle is sampled into linear keys per joint:
   only at the written key times when everything is linear, at a
   fixed rate where easing, delay or targets make the path curved.
   Same source, same bytes.
6. **The strip.** `bin/plastica specimen <figura.stml> -o <png>`:
   the rest pose as today, then per cycle a row of frames from the
   side and from three-quarter, dithered at world size, with joint
   pivots marked. Numbers printed per cycle: lowest point per frame
   (floating or sinking), how far a planted foot slides at the
   world's walking speed, and the gap at the loop seam.
7. **Carried for the combat spec, unused here:** joint names (to
   hold a sword at `manus_dex`), moments (to time a hit). Cross-fade
   and one-shot playing are that spec's work.
8. **The build.** `bin/plastica condere -volumen V` also compiles
   every figure and writes it as a character. Covered by the
   standing asset permission.

## V. How it is tested

1. Joints: the node tree and each node's parent pinned for a small
   skeleton; a figure that moves a pivot moves only that node.
2. Poses: turn in the sheet = channel value in the model; `ex` and
   `speculum` pinned on a two-arm fixture (mirror of mirror = the
   original).
3. Cycles: key times and values; the loop closes; easing values at
   three sample times; a delayed joint equals the undelayed one
   shifted.
4. Targets: the limb end lands on the target within a small
   tolerance; the knee bends the declared way; out of reach is
   refused with the line.
5. Sharing: one cycle on two figures with different leg lengths:
   same turns, root rise scaled.
6. Round trip: figure to GLB and back gives equal cycles, joint
   names and moments.
7. Refusals: unknown joint, pose or skeleton; duplicate names; a
   cycle with no keys; times not increasing; each with a test.
8. Determinism: digest of one built figure.
9. Every gate proved red once by a compiling plant.
10. By eye: Claude reads every strip before a figure is built into
    the volume; Fran looks in `$models` and `$world` when he likes.

## VI. Left to the builder

Exact element and attribute names; the sampling rate of IV.5; the
strip's layout; whether skeleton sheets are included by name or by
path; the task order (suggested: IV.1, IV.2 + IV.3 linear only,
IV.6, IV.8 with the slime, then mirroring and sharing with the
biped, IV.4, the dials of IV.3, IV.7's carrying, the boar).

## VII. Later, for orientation only

Bending meshes (II.3); cross-fade, one-shot cycles and held items
(combat spec); look-at and other motion computed at run time; motion
on props in a place; figures as clip art in graphis pictures at a
chosen frame.

## AUDIENDA (not verified)

1. Whether Claude can write a convincing walk from numbers and
   strips is the open question of the whole design. The slime and
   the first biped walk answer it.
2. (Settled while planning: plastica's groups already make a node
   each, `lib/plastica.c` `_grex_aperire`.)
3. That the reader keeps node names and can keep `extras` on an
   animation has not been read (the writer side was).
4. That the world's body drawing honours scale channels (the slime's
   squash) is inferred from the golem having them, not tested.
5. The foot-slide number assumes the world's fixed 4 units per
   second; a slower creature needs its own speed, which is the
   combat spec's concern.
6. Rigid parts leave gaps or overlaps at a bent knee or elbow. How
   that reads under dither at world size is not known; overlapping
   the parts a little at each joint is the planned answer.
