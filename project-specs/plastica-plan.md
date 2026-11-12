# plastica — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.

**Goal:** Claude writes a static 3D asset as a short text file, sees
it on a sheet of views, and builds it into Fran's volume, where he
sees it in `$models` and, placed, in the clearing of `$world`.

**Architecture:** a compiler from an STML description to the
existing `Statua` value; a sheet of views with numbers; seeded
texture generators; a GLB writer; a third kind ("prop") in the
store and in `$models`; a joiner that appends placed models to a
level; on the mundus side a layout file and a build that makes the
furnished place and checks that props block.

**Tech stack:** C89 + latina.h; house libraries only (`stml`,
`json`, `imago_png`, `sors`, `stereometria`, `statua*`, `dithering*`,
`mundus_solum`).

**Spec:** `project-specs/plastica-spec.md` (approved by Fran
2026-11-11). Read it first; this plan argues from it.

## Global constraints

- Latin identifiers; `VOCABULA_VIAE_ADDITAE=... ./oratio/vocabula.sh
  -nova` shows NOVA 0 (add glossary lemmata where a word is new).
  latina.h macros and Roman numerals are never identifiers.
- New C files through `./silva/scribe.sh`; every file
  `./silva/examen.sh` ACCIPE and `./silva/formator.sh -scribere`
  CONFORMIS; 72 columns. A new lib file needs
  `./tools/compile_tests_fontes_generare.sh`.
- `i32` is unsigned, `s32` signed; `chorda` is not NUL-terminated;
  `-Wconversion -Wsign-conversion -Werror`. All floating point is
  `f32`. Arenas only.
- TDD. Each gate is proved red by a compiling plant (`silva.planta`,
  porta `radix`, the suite as `filtrum`) before it counts.
- Plastica (`plastica*`) never includes a `mundus*` header (spec 9a).
  The mundus side may include plastica.
- Same source, same bytes: no clock, no address and no unseeded
  random number reaches a model, a texture or a written file.
- Pinned digests that must not move: `statua_pingere`,
  `statua_scaenam_pingere`, `statuae_quadrum_pingere`, the dithers,
  and the walk-map pins of the bare level (60626 walkable, 42881
  reachable).
- New code goes in new modules, not as new includes in old lib
  files (tessera and ludus_tessera keep hand source lists); run both
  `compile_probationes.sh` before any commit.
- Tests write only under `credo_area(c)`, close temporary volumes,
  and never touch Fran's live volume. Only the two build commands,
  run by Claude after T7 and T9, write there (standing OK,
  2026-11-11), and not before the vicus Fran runs knows the prop
  kind (spec 14d).
- Units: one unit is the mundus world unit; y is up; the ground is
  y = 0. Angles in the description are degrees.

## Review focus

Inputs the spec implies and Claude or Fran will meet; each has its
test in the task named.

1. A broken description: unknown element or attribute, a missing or
   non-numeric size, a zero or negative size, text that is not STML.
   Refused with a plain reason and a line number; never a crash, and
   never a half-made model. (T1)
2. Degenerate solids: a cylinder or lathe with fewer than 3 sides, a
   lathe profile with one point, an outline with fewer than 3 points
   or wound the wrong way, a repeat or scatter count of 0. Refused,
   or defined and tested (wrong winding is turned round). (T2)
3. An asset with no solids, or one that floats above or sinks below
   the ground: the first is refused; the second is allowed but the
   sheet's numbers show the lowest point. (T1, T3)
4. A prop whose title is already a character or a place in the
   volume: the build names it and writes nothing for it, and goes on
   with the rest. (T7)
5. A placement that names an unknown asset, lies outside the level,
   on water or on a spot that is not walkable, or a prop marked
   solid that blocks no cell: the place build fails and names the
   placement; nothing is written. (T9)

## Files

| file | holds |
|---|---|
| `include/plastica.h`, `lib/plastica.c` | description -> `Statua` |
| `include/plastica_textura.h`, `lib/plastica_textura.c` | texture generators |
| `include/plastica_specimen.h`, `lib/plastica_specimen.c` | numbers and the sheet |
| `include/statua_glb_scriptor.h`, `lib/statua_glb_scriptor.c` | GLB writer |
| `include/statua_iunctio.h`, `lib/statua_iunctio.c` | joiner |
| `include/statuae_repositorium.h`, `lib/statuae_repositorium.c` | prop kind (added) |
| `lib/statuae.c`, `apps/statuae/organum.stml` | group switch in `$models` |
| `include/mundus_locus.h`, `lib/mundus_locus.c` | layout, furnished place |
| `tools/plastica.c`, `tools/plastica_struere.sh`, `tools/plastica_fumus.sh` | `bin/plastica` |
| `tools/mundus.c`, `tools/mundus_struere.sh`, `tools/mundus_fumus.sh` | `bin/mundus` |
| `tools/statuae.c` | `-res` |
| `plastica/res/*.stml` | the assets |
| `apps/mundus/loci/clearing.stml` | the layout |
| `probationes/probatio_plastica*.c`, `probatio_statua_glb_scriptor.c`, `probatio_statua_iunctio.c`, `probatio_mundus_locus.c` | tests |

Each new `.c` gets a worklog beside it when there is something to
record.

## The description, fixed here so every task agrees

```
<res titulus="well">
  <grex locus="0 0 0" gyrus="0">
    <capsa titulus="murus" magnitudo="2 1 2" locus="0 0 0"
           color="#8a8f94" signum="solidum"/>
    <cylindrus radius="0.1" radius_summus="0.1" altitudo="2"
               latera="6" locus="0.9 1 0" color="#6b4a2b"/>
  </grex>
</res>
```

- `locus="x y z"` is the centre of the solid's base; default
  `0 0 0`. `gyrus` turns about the upright axis; `rotatio="x y z"`
  is the rare full turn, applied after `gyrus`.
- Solids: `capsa` (`magnitudo`), `cylindrus` (`radius`,
  `radius_summus` default = `radius`, 0 gives a cone; `altitudo`;
  `latera` default 8), `sphaera` (`radius`; `segmenta` default 6;
  `asperitas` 0..1 and `semen` for rocks), `cuneus` (`magnitudo`;
  the top slopes down along +x to nothing), `extrusio`
  (`puncta="x,z x,z ..."`, `altitudo`), `tornus`
  (`puncta="r,y r,y ..."`, `latera`).
- Every solid takes `titulus`, `color="#rrggbb"`, `textura` (T4),
  `signum` (`solidum`, `gressibile`, or absent) and
  `invisibilis="verum"` (the unseen blocker of spec 14a: material
  alpha 0, cut-out limit 0.5; the renderer already cuts it,
  `lib/statua_pictura.c:141`).
- Groups: `grex` (transform only), `speculum axis="x|z"` (children
  and their mirror image), `series numerus gressus="x y z"
  gyrus_gressus` (children repeated), `sparsio numerus semen
  area="x z" magnitudo_varia gyrus_varius` (children scattered).
- One solid is one node with one mesh; normals are per face (flat
  shading).

---

### Task 1: the compiler, boxes and groups

**Files:** create `include/plastica.h`, `lib/plastica.c`,
`probationes/probatio_plastica.c`.

**Interfaces:**
- Produces:
  ```c
  nomen structura {
         b32  successus;
      Statua* statua;   /* finita; NIHIL si recusata */
      chorda* partes;   /* statua->nodorum tituli (vacuus si nullus) */
      chorda  error;    /* Anglice, sine linea nova */
         i32  linea;    /* I-basata; 0 si ignota */
  } PlasticaFructus;

  PlasticaFructus
  plastica_legere (chorda textus, Piscina* piscina);
  ```

- [x] **Step 1: failing tests.** A single `capsa` of `2 1 2` at the
  origin: 1 node (plus group nodes), 12 triangles, bounds x and z in
  [-1, 1], y in [0, 1], every normal a unit axis vector, `signa`
  = `STATUA_SIGNUM_SOLIDUM` when marked, material colour from
  `color`. A `grex` with `locus` and `gyrus="90"` moves and turns
  its child (bounds checked through `statua_positura`). `partes[n]`
  equals the `titulus`. `invisibilis` gives alpha 0 and limit 0.5.
  Review focus 1 and 3: each broken input is refused with the line
  number; an empty `<res>` is refused; `statua` is NIHIL on refusal.
- [x] **Step 2:** run `./compile_tests.sh plastica`; red.
- [x] **Step 3:** implement `plastica_legere` for `res`, `grex`,
  `capsa`.
- [x] **Step 4:** green; plants (a face's winding flipped; the base
  centred instead of on the ground; a refusal let through).
- [x] **Step 5:** examen, formator, lint, worklog; commit on Fran's
  word.

### Task 2: the other solids and the repeaters

**Files:** modify `lib/plastica.c`, `probationes/probatio_plastica.c`.

**Interfaces:** consumes T1; no new functions.

- [x] **Step 1: failing tests.** `cylindrus` with `latera="6"`: 24
  triangles (6 x 2 sides, 6 + 6 caps), bounds = radius and height; a
  cone (`radius_summus="0"`) has 12. `sphaera`: every vertex at the
  radius when `asperitas` is 0; with `asperitas` and a `semen` the
  same seed gives the same vertices and another seed different
  ones. `cuneus`: 8 triangles, top edge at x = -size/2 only.
  `extrusio` of a square equals the `capsa` bounds; a clockwise
  outline gives the same outward normals. `tornus` of a two-point
  profile equals the `cylindrus` bounds. `speculum`: bounds
  symmetric, and mirrored faces still face outward. `series
  numerus="4"`: 4 copies at the step. `sparsio`: same seed same
  places; all inside `area`. Review focus 2.
- [x] **Step 2-4:** red, implement, green; plants (cap winding;
  mirror without the winding fix; scatter ignoring the seed).
- [x] **Step 5:** as T1.

### Task 3: numbers, the sheet, `bin/plastica`, and the well

**Files:** create `include/plastica_specimen.h`,
`lib/plastica_specimen.c`, `probationes/probatio_plastica_specimen.c`,
`tools/plastica.c`, `tools/plastica_struere.sh`,
`tools/plastica_fumus.sh`, `plastica/res/puteus.stml`.

**Interfaces:**
- Produces:
  ```c
  nomen structura {
      StereometriaVector minimum;
      StereometriaVector maximum;
                     i32 triangulorum;
                     i32 partium;
                     i32 cellae_solidae;    /* vestigium, cellae 0.25 */
                     i32 cellae_gressibiles;
  } PlasticaMensurae;

  PlasticaMensurae
  plastica_metiri (constans Statua* statua, Piscina* piscina);

  /* IV aspectus x IV modi (crudum, exemplaria, atkinson, partes) */
  b32
  plastica_specimen_pingere (
      constans Statua* statua,
               Imago* exitus,
             Piscina* piscina);
  ```
  (As built: no part-name table - the picture needs only the part
  order, the tool prints the names. `PlasticaMensurae` also carries
  `invisae`. No pinned digest: vertex bytes differ between
  compilers; determinism, layout, scale and colours are pinned
  instead. See `lib/plastica_specimen.worklog.md`.)
  `bin/plastica specimen <res.stml> -o <png>` prints the numbers and
  the part legend (colour name per part) and writes the sheet;
  refusals print `via:linea: causa` and exit 1.
- The footprint counts a cell when its centre lies under a marked
  solid's top face, the rule of `mundus_solum`, restated here
  without including it.

- [x] **Step 1: failing tests.** Numbers of a known box and of a
  thin post (0 solid cells). Sheet: size fixed, four columns by four
  rows, the raw cell is not blank, the part cell uses a different
  flat colour for each of two parts, digest pinned. Smoke script:
  the well compiles, a broken file exits 1 with a line number.
- [x] **Step 2-4:** red, implement, green; plants.
- [x] **Step 5:** write the well, in flat colours; look at the sheet;
  revise until it reads as a well both raw and dithered. Show Fran
  the sheet. This answers spec AUDIENDA 1 as far as flat colours
  can.
- [x] **Step 6:** as T1 step 5.

### Task 4: textures

**Files:** create `include/plastica_textura.h`,
`lib/plastica_textura.c`, `probationes/probatio_plastica_textura.c`;
modify `lib/plastica.c` (attribute `textura`, UVs).

**Interfaces:**
- Produces:
  ```c
  #define PLASTICA_TEXELLA_PER_UNITATEM  XVI   /* see step 1 */

  /* genus: "cortex" "tabulae" "saxum" "lateres" "stramen" "frondes"
   * "muscus" "humus" "granum"; colores III rgb; FALSUM si ignotum */
  b32
  plastica_textura_generare (
      chorda  genus,
         i32  latitudo,
         i32  altitudo,
         i32  semen,
      constans i8* colores,
           Imago* exitus,
         Piscina* piscina);
  ```
  In the description: `textura="saxum" semen="3"
  colores="#777 #999 #555"`; UVs per face so one unit covers the
  texel constant; textures repeat. `<textura titulus="x"
  latitudo="8">` with rows of legend characters defines a
  hand-written texture, at most 16 x 16.

  (As built: the constant is 16, measured - every material of
  `planum.glb` sits at 16 texels per unit. Every pixel is exactly
  one of the three colours, in the order dark, mid, light.
  `colores` absent derives them from `color`. Generated tiles are
  always 32 x 32 in a description. Added
  `plastica_textura_genus(index)` and `bin/plastica texturae -o
  <png>`, a swatch sheet of all nine. Legend letters are `a`..`p`
  indexing `colores`. See `lib/plastica_textura.worklog.md`.)

- [x] **Step 1:** measure the kit's texel density from
  `planum.glb` (texture width over the world width of one textured
  face, a throwaway program); set the constant to match and record
  the number in the worklog.
- [x] **Step 2: failing tests.** Each generator: same seed same
  digest, other seed other digest, only colours between the three
  given. A 2-unit box face carries UVs 0..2 x constant / width. A
  17 x 17 hand texture is refused; an unknown `genus` is refused
  with its line.
- [x] **Step 3-5:** red, implement, green, plants; texture the well
  and look at the sheet; as T1 step 5.

### Task 5: the GLB writer

**Files:** create `include/statua_glb_scriptor.h`,
`lib/statua_glb_scriptor.c`,
`probationes/probatio_statua_glb_scriptor.c`.

**Interfaces:**
- Produces:
  ```c
  /* chorda vacua si statua non scribi potest */
  chorda
  statua_glb_scribere (constans Statua* statua, Piscina* piscina);
  ```
  Writes nodes (TRS, children), meshes, materials (colour, texture,
  cut-out, emissive, two-sided, wrap modes), textures as PNG, marks
  as `extras.tags` (`walkable`, `collidable`, `water`), clips.

  (As built: the signature has a third argument, an optional table
  of node names - `statua_glb_scribere(statua, tituli, piscina)` -
  so a prop's part names reach the file. The reader gained one
  rule: a rotation that is already unit length is not normalised
  again, or read-write-read drifts in the last digit. Sizes:
  `planum.glb` 3399516 -> 3318480 bytes. See
  `lib/statua_glb_scriptor.worklog.md`.)

- [x] **Step 1: failing tests.** Read, write, read again and compare
  with the first read, field by field, for a made asset with a
  texture and marks, for `fungus.glb` (clips) and for `planum.glb`.
  Writing the same `Statua` twice gives the same bytes.
- [x] **Step 2-4:** red, implement, green; plants (marks dropped;
  texture wrap dropped; a clip's last key dropped).
- [ ] **Step 5:** as T1. Record the size of the rewritten level
  beside the original (spec AUDIENDA 5).

### Task 6: the prop kind and the group switch

**Files:** modify `include/statuae_repositorium.h`,
`lib/statuae_repositorium.c`, `tools/statuae.c`,
`tools/statuae_fumus.sh`, `lib/statuae.c`,
`apps/statuae/organum.stml` (+ `./tools/organa_infigere.sh`), their
suites.

**Interfaces:**
- Produces: `STATUAE_GENUS_RES` (in the act `"genus":"res"`);
  `bin/statuae addere ... -res`, `enumerare ... -res`. In `$models`
  a header button and one key (named in this task's header after
  reading the pane's key table) switch between characters and
  props; the choice is a persistent field; the title shows which
  group is open; an empty group says so.

- [ ] **Step 1: failing tests.** A prop is listed by
  `statuae_enumerare_generis(RES)` and not by `statuae_enumerare`;
  a prop title that is already a character is refused. Pane: with
  one character and one prop, the switch shows the prop and back;
  the field survives reopening; no props shows the message.
- [ ] **Step 2-4:** red, implement, green; plants.
- [ ] **Step 5:** as T1, and rebuild `bin/statuae`. After the
  commit Fran restarts vicus from secunda (spec 14d).

### Task 7: building props into a volume

**Files:** modify `tools/plastica.c`, `tools/plastica_fumus.sh`.

**Interfaces:**
- Produces: `bin/plastica condere -volumen V [-fontes DIR]`
  (default `plastica/res`): compiles each `*.stml` in name order,
  writes it as a prop titled by its `titulus`; prints one line per
  asset (`addita`, `eadem`, `recusata: causa`) and the props in the
  volume with no source; exit 1 if any was refused.

- [ ] **Step 1: failing smoke tests** on a temporary volume: two
  assets appear as props; a second run writes nothing (`eadem`); a
  title held by a character is refused and the other asset still
  lands (review focus 4); a prop with no source is listed; a broken
  file is named with its line.
- [ ] **Step 2-4:** red, implement, green.
- [ ] **Step 5:** as T1. Then, once Fran's vicus has T6, run it on
  the live volume: the well is in `$models`.

### Task 8: the joiner

**Files:** create `include/statua_iunctio.h`, `lib/statua_iunctio.c`,
`probationes/probatio_statua_iunctio.c`.

**Interfaces:**
- Produces:
  ```c
  nomen structura {
      constans Statua* statua;
       StereometriaMatrix locatio;
  } StatuaIungenda;

  /* statua nova finita: basis et addendae; NIHIL si deficit */
  Statua*
  statua_coniungere (
              constans Statua* basis,
      constans StatuaIungenda* addendae,
                         i32  numerus,
                      Piscina* piscina);
  ```
  Each added model hangs under one new root node carrying its
  placement; indices of meshes, materials and textures are shifted;
  marks kept; clips of added models are dropped, the base's kept.

- [ ] **Step 1: failing tests.** Counts add up; a box added at
  (5, 0, 7) turned 90 degrees has those world bounds; the base's
  nodes and its picture digest are unchanged with zero added;
  marks survive; the result passes `statua_finire`.
- [ ] **Step 2-5:** red, implement, green, plants, as T1.

### Task 9: the layout and the furnished place

**Files:** create `include/mundus_locus.h`, `lib/mundus_locus.c`,
`probationes/probatio_mundus_locus.c`, `tools/mundus.c`,
`tools/mundus_struere.sh`, `tools/mundus_fumus.sh`,
`apps/mundus/loci/clearing.stml`.

**Interfaces:**
- Layout:
  ```
  <locus titulus="clearing"
         basis="probationes/fixa/statuae/planum.glb">
    <res fons="plastica/res/puteus.stml" locus="30 34" gyrus="15"/>
  </locus>
  ```
- Produces:
  ```c
  nomen structura {
      chorda fons;
         f32 x;
         f32 z;
         f32 gyrus;
         i32 linea;
  } MundusLocusRes;

  nomen structura {
                 b32  successus;
              chorda  titulus;
              chorda  basis;
      MundusLocusRes* res;
                 i32  numerus;
              chorda  error;
                 i32  linea;
  } MundusLocusFructus;

  MundusLocusFructus
  mundus_locus_legere (chorda textus, Piscina* piscina);

  /* locus instructus: quaeque res in altitudine soli sub ea; error
   * rem nominat (linea) si extra locum, in aqua, non gressibilis,
   * aut solida nullam cellam claudit */
  Statua*
  mundus_locus_struere (
             constans Statua* basis,
      constans MundusLocusRes* res,
            constans Statua** statuae,
                         i32  numerus,
                      chorda* error,
                         i32* linea,
                     Piscina* piscina);
  ```
  `bin/mundus condere -volumen V [-loci DIR]` builds each layout and
  writes it as a place; `bin/mundus specimen <res.stml> -o <png>`
  draws the asset in the clearing beside the mushroom man with the
  scene's view and dithers.

- [ ] **Step 1: failing tests** on the hand-made level
  (`probationes/mundus_locus_fictus.h`): a box placed on flat
  ground stands at the ground's height and its cells turn solid;
  cells elsewhere are unchanged; a marked step makes a higher
  platform reachable; a character behind a tall box is hidden in
  the frame and in front of it is not (spec AUDIENDA 4). Review
  focus 5: each bad placement fails naming its line; a thin post
  with no blocker fails, with one passes.
- [ ] **Step 2-4:** red, implement, green; plants.
- [ ] **Step 5:** smoke script on a temporary volume; trace whether
  an open place reloads when replaced (spec AUDIENDA 6) and fix it
  in `mundus_latus` if it does not, with a test.
- [ ] **Step 6:** as T1.

### Task 10: the first assets and the furnished clearing

**Files:** create `plastica/res/*.stml` (spec decision 15), fill
`apps/mundus/loci/clearing.stml`; add one test to
`probationes/probatio_mundus_locus.c`.

- [ ] **Step 1: failing test.** The furnished real level: walkable
  and reachable counts differ from the bare pins only by the cells
  under props, except that with the stair the reachable count
  rises by the top plateau (numbers pinned once seen and checked
  by eye on a map picture).
- [ ] **Step 2:** model each asset; for each, the sheet and the
  in-clearing picture until it reads well dithered.
- [ ] **Step 3:** lay out the clearing; build; walk it headless
  (`mundus_imago` scratch program) and look at the pictures.
- [ ] **Step 4:** commit on Fran's word; run both builds on the
  live volume; tell Fran what is there to look at.

## Self-review (2026-11-11)

- Spec coverage: I.1 T1-T2; I.2 T3; I.3 T6-T7; I.4 T8-T10; I.5
  constraint + file split; IV.1 T1-T2; IV.2 T4; IV.3 T3 (+ T9 for
  the in-clearing picture); IV.4 T5; IV.5 T8; IV.6 T6; IV.7-8 T7,
  T9; decisions 14a T3/T9, 14b T1, 14c T7, 14d T6/T7; V.1-7 in the
  tasks named.
- Not covered by any task, on purpose: everything in spec VII.
