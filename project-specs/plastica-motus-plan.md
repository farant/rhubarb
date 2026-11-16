# plastica motus — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.
> Signatures below are proposals; the header Fran approves at the
> start of each task is what binds.

**Goal:** Claude writes a creature as a sheet (parts on joints, poses,
cycles), builds it, reads a strip of its motion, and Fran sees it
walk in `$models` and `$world`.

**Architecture:** plastica's reader learns joints (a named group that
is a node). A new module reads poses, cycles and limbs from the same
sheet (and a shared skeleton sheet), bakes them to ordinary `Statua`
cycles, and returns a model everything downstream already plays. The
preview and the build tool grow a figure path. Joint names and marked
moments ride in the GLB beside `Statua`.

**Tech stack:** C89 + latina.h; house libraries only.

**Spec:** `project-specs/plastica-motus-spec.md` (approved by Fran
2026-11-14). Read it first; decision numbers refer to its section II,
design numbers to its section IV.

## Global constraints

- The global constraints of `project-specs/graphis-plan.md` hold
  unchanged (Latin identifiers at NOVA 0, `./silva/scribe.sh` for new
  C files, `examen` ACCIPE, `formator` CONFORMIS, TDD with compiling
  plants, new code in new modules, both subsystem runners before a
  commit, tests write only under `credo_area(c)` and close temporary
  volumes, no raw IO outside `filum_*` / `lectiones_*`).
- `plastica*` headers include no `mundus*` header (plastica spec 9a).
- `Statua` does not grow (plastica spec 14b). No change to the
  renderer or the dithers; their pinned digests stay.
- Every existing `<res>` sheet builds to the same bytes as before.
  The existing plastica suites pass without edits to their
  assertions.
- Same source, same bytes, on every machine: seeds and integer or
  house arithmetic only, as plastica does today.
- No model with a non-finite number ever leaves the reader: a sheet
  that would produce one is refused.
- Only `bin/plastica condere`, run by Claude, writes into Fran's live
  volume (standing OK, 2026-11-11).
- Names `figura`, `motus`, `articulus` are taken as library names;
  new files are `plastica_*`.

## Review focus

1. A malformed motion sheet: a pose naming an unknown joint, a key
   naming an unknown pose, key times not increasing or beyond the
   duration, duplicate joint / pose / cycle names, a cycle with no
   keys. Refused with reason, sheet and line; no model. (T2)
2. A turn of 180 degrees or more between two neighbouring keys: the
   in-between goes the short way round, which is not what was
   written. Refused with "add a key between". (T2)
3. A limb target out of reach, at exactly full stretch, or on the
   hip itself: refused or solved, never a non-finite number. (T6)
4. Joints in the wrong place: inside a mirror, series or scatter;
   in a `<res>`; a figure naming a joint its skeleton lacks; a
   missing skeleton; a skeleton that names a skeleton. (T1, T5)
5. Scale keys of zero or below; a cycle of one pose (plays as a
   still); a cycle that sinks the figure below the ground (reported
   in the strip's numbers, not refused). (T2, T3)

## Files

| File | Task | Is |
|---|---|---|
| `include/plastica.h`, `lib/plastica.c` | 1 | `<figura>` root, `<articulus>` |
| `probationes/probatio_plastica.c` | 1 | new sections only |
| `include/plastica_motus.h`, `lib/plastica_motus.c` (+ worklog) | 2, 5, 6, 7 | poses, cycles, baking; skeleton sheets; targets; dials |
| `probationes/probatio_plastica_motus.c` | 2, 5, 6, 7 | new suite |
| `include/plastica_specimen.h`, `lib/plastica_specimen.c` | 3, 6 | the strip and its numbers |
| `probationes/probatio_plastica_specimen.c` | 3, 6 | new sections only |
| `tools/plastica.c`, `tools/plastica_fumus.sh` | 3, 4, 5 | `specimen` and `condere` for figures |
| `plastica/figurae/*.stml`, `plastica/ossa/*.stml` | 4, 5, 6, 8 | slime; biped skeleton and goblin; boar |
| `include/plastica_membrum.h`, `lib/plastica_membrum.c` | 6 | the two-joint solve, a pure function |
| `probationes/probatio_plastica_membrum.c` | 6 | new suite |
| `include/statua_glb.h`, `lib/statua_glb.c`, `include/statua_glb_scriptor.h`, `lib/statua_glb_scriptor.c` | 8 | names and moments through the GLB |

---

### Task 1: joints in the sheet

**Produces:**

```c
/* plastica.h - FORMA additur:
 *   <figura titulus="slime">            radix altera; ut <res>
 *     <articulus titulus="corpus" locus="0 0.4 0">
 *       <sphaera .../>
 *       <articulus titulus="oculus_sin" locus="..."> ... */
/* PlasticaFructus += */
    i32* articuli;      /* indices nodorum qui articuli sunt, ordine
                         * plagulae; NIHIL si nulli */
    i32  articulorum;
```

A joint takes `titulus` (required, unique in the sheet), `locus`,
`gyrus`, `rotatio`, and makes exactly one node. Under `<figura>` the
elements `<positura>`, `<cyclus>`, `<membrum>` are passed over here
(task 2 reads them). At most 64 joints.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: node tree and parents of a
  three-joint fixture; solids and plain groups inside a joint are
  its children; joint table in sheet order; each refusal of review
  focus 4 that needs no skeleton (joint in a repeater, joint in a
  `<res>`, missing or duplicate title, 65 joints) with its line;
  `<res>` sheets unchanged (digest of two existing props).
- [ ] Implement; `./compile_tests.sh plastica`.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-14).** As the header Fran approved. Suite
`probatio_plastica` gained sections `articuli` and
`recusationes_articulorum` (22 sections); 27 plants, all red. Beyond
the plan, both in the approved header: a joint may not sit inside a
plain `<grex>` either (the skeleton is exactly the nesting of
joints), and `<figura>` takes no `obex`. Found by a plant that stayed
green: STML gives an empty attribute as absent, so the joint's
empty-title test was redundant and was removed. Existing props
checked unchanged by comparing four preview sheets before and after
(byte-identical), not by a new pinned digest.

### Task 2: poses and plain cycles

**Consumes:** the joint table of task 1.

**Produces:**

```c
/* plastica_motus.h */
nomen structura {
    i32    cyclus;      /* index in statua->cycli */
    chorda titulus;
    f32    tempus;
} PlasticaMomentum;

nomen structura {
               b32  successus;
            Statua* statua;     /* finita, cum cyclis */
            chorda* partes;
               i32* articuli;
               i32  articulorum;
  PlasticaMomentum* momenta;
               i32  momentorum;
            chorda  error;
            chorda  plagula;    /* quae plagula erravit; vacua = ipsa */
               i32  linea;
} PlasticaFiguraFructus;

PlasticaFiguraFructus plastica_figura_legere (
    chorda textus, i32 semen, Piscina* piscina);
```

Sheet form (design 2, 3): `<positura titulus ex>` holding
`<art titulus rotatio scala motus>` lines (`motus` on the root joint
only); `<cyclus titulus duratio circulus>` holding
`<clavis tempus positura>` and `<momentum titulus tempus>`. Linear
keys only in this task. A looping cycle gets its first pose again at
`duratio`. Only joints that a cycle's poses touch get channels.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: a turn in the sheet is the channel
  value in the model (checked through `statua_positura` at key times
  and halfway); `ex` changes only the joints it lists; loop closes;
  a one-pose cycle; moments returned; untouched joints have no
  channel; review focus 1, 2 and the scale part of 5, each with its
  line; digest of the fixture figure.
- [ ] Implement; `./compile_tests.sh plastica_motus`.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-14).** As the header Fran approved, plus one
sentence: the closing key of a looping cycle is compared modulo 360,
so keys at 0, 120, 240 close into a full turn. New module
`plastica_motus` and suite `probatio_plastica_motus` (9 sections);
81 plants, 80 red, one green by nature (a bounds guard whose removal
overflows an array without changing the answer). Differences from
this plan: the result has no `plagula` field yet (it arrives with
skeleton sheets in task 5); `quies` is a built-in pose; `circulus`
defaults to off; `<membrum>` is refused with "limbs are not built
yet" until task 6; determinism is pinned by reading the same sheet
twice and comparing every key, not by a digest. Limits: 64 poses,
16 cycles, 64 keys and 8 moments per cycle, 60 seconds.

### Task 3: the strip

**Produces:**

```c
/* plastica_specimen.h */
nomen structura {
    f32 infimum_minimum;   /* punctum infimum per omnia quadra */
    f32 infimum_maximum;
    f32 hiatus_suturae;    /* maxima distantia articuli inter quadrum
                            * primum et ultimum cycli circularis */
} PlasticaMensuraeMotus;

b32 plastica_metiri_motum (constans Statua* statua, i32 cyclus,
    i32 quadra, PlasticaMensuraeMotus* mensurae, Piscina* piscina);
Imago* plastica_specimen_motus_pingere (constans Statua* statua,
    constans chorda* partes, constans i32* articuli,
    i32 articulorum, Piscina* piscina);
```

One row per cycle: eight frames from the side and eight from
three-quarter, dithered at world size, joint pivots marked, the
cycle's title and numbers printed beside it. `bin/plastica specimen`
picks this path when the root is `<figura>` and prints the numbers
as text too.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: numbers of a fixture that rises 0.5
  and of one that sinks; seam gap 0 for a closed loop; picture size
  follows the number of cycles; a figure with no cycles gives the
  rest sheet only; smoke case in `tools/plastica_fumus.sh`.
- [ ] Implement; `./compile_tests.sh plastica_specimen`; rebuild the
  tool (`./tools/plastica_struere.sh`).
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-14).** As the header Fran approved. Differences
from this plan: two rows per cycle (side, then three-quarter), eight
cells wide, 960 pixels, not one row of sixteen; the strip is a
second file, `plastica specimen <sheet> -o <png> -motus <png>`; no
lettering in the picture (the tool prints one line per cycle and its
moments); the seam number measures every vertex; the painter writes
into the caller's `Imago` and returns `b32`, like the rest sheet.
`PlasticaFructus` gained `figura` (the root was `<figura>`), which
the tool uses to decide whether to read motion. Suite
`probatio_plastica_specimen` has 4 new sections (17); 40 library
plants and 12 hand plants on the tool (`tools/plastica_fumus.sh`),
all red after three weak tests were strengthened and one redundant
check removed. Claude read a strip of a test puppet (walk and wave)
by eye. An arm raised sideways does not show in the side row.

### Task 4: the build, and the slime

- [ ] Header text to Fran (the `condere` output lines and the usage
  text); wait for his word.
- [ ] `tools/plastica.c`: `condere` also compiles every sheet in
  `plastica/figurae/` and writes it as a character. A figure that
  is refused stops the build with sheet and line; nothing written.
  Smoke cases: figure added, unchanged figure writes nothing,
  refused figure.
- [ ] `plastica/figurae/limax.stml`: the slime, self-contained (its
  own joints, poses, cycles `idle`, `walk`, `attack`, `hurt`,
  `die`), squash by scale keys.
- [ ] Claude reads the strip and revises until it is right; then a
  headless picture of it walking in `$world` on the test volume
  (answers spec AUDIENDA 4, scale keys in the world).
- [ ] Build into Fran's live volume; tell Fran where to look.
- [ ] Plants for the tool, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-16).** As the header Fran approved, including
the change from this plan that he accepted: a refused figure does
not stop the others (exit 1, as props). Added beyond it: a `<figura>`
sheet in the props directory is refused; duplicate titles are caught
across props and figures of one run. Smoke script gained the figure
cases; 18 hand plants on the tool, all red. The slime
(`plastica/figurae/limax.stml`, title `slime`): one joint for the
body, one for the top drop, one per eye; cycles idle, walk (a hop),
attack with moment `ictus`, hurt, die. Revised by strip: base disc
removed, tilted poses lifted off the ground. Seen headless in the
world: stands, hops, scale keys show (spec AUDIENDA 4 answered);
recoloured from green to blue because green vanished on the grass.
Built into Fran's live volume 2026-11-16 (`figura addita slime`).
Found, not fixed: the strip's 120 pixel cells cut off wide poses and
forward lunges of a wide figure.

Known limit, not fixed here: the world sizes every character to the
same height (`lib/mundus_pictura.c:536`), so the slime stands as
tall as the mushroom man when chosen as the character. Creature
size in the world belongs to the combat spec.

### Task 5: mirrored poses and shared skeletons

**Produces:**

```c
/* plastica_motus.h */
nomen chorda (*PlasticaFons) (chorda titulus, vacuum* contextus);

PlasticaFiguraFructus plastica_figura_legere_cum_fonte (
    chorda textus, i32 semen, PlasticaFons fons, vacuum* contextus,
    Piscina* piscina);
```

`<positura titulus speculum="other">`: joints ending `_sin` and
`_dex` swap, turns reflect. Skeleton sheet `<ossa titulus>` holds
the joint tree, poses, cycles (and limbs, task 6). A figure with
`ossa="bipes"` lists flat `<articulus titulus="...">` elements that
hang solids on that joint and may give a new `locus`. The root
joint's `motus` is scaled by figure root height over skeleton root
height (decision 6). A figure's own pose or cycle replaces the
skeleton's of the same title.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: mirror of mirror is the original;
  mirror swaps a pair and leaves an unpaired joint reflected in
  place; one cycle on two figures with different root heights gives
  equal turns and scaled rise; pivot override moves one node only;
  figure cycle replaces skeleton cycle; the skeleton cases of
  review focus 4, the error naming the right sheet.
- [ ] Implement; tool passes a source that reads
  `plastica/ossa/<name>.stml`.
- [ ] `plastica/ossa/bipes.stml` and `plastica/figurae/` goblin with
  `idle` and a first `walk` from turns alone.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-16).** As the header Fran approved. Added to it:
the symmetry check on a mirrored pair's rest pose; `plagula` names
the skeleton and `linea` is that sheet's line; `ex` and `speculum`
copy when read (noted in the header); after a replaced pose every
cycle's 180-degree check runs again. `plastica_legere_arboris` is
new in `plastica.h` (plastica reads a tree, so the figure's solids
and the skeleton's joints are merged as STML trees and line numbers
survive). The tool looks for `ossa/<name>.stml` beside the figure's
directory, no flag. Strip cells widened to 160 (Fran's yes,
2026-11-16): `PLASTICA_MOTUS_CELLA_LATA`, strip 1280 wide, cell
height and ground line unchanged. `plastica/ossa/bipes.stml` (13
joints; idle and walk; moments `gressus_sin`, `gressus_dex`) and
`plastica/figurae/goblin.stml`, built into Fran's live volume
(`figura addita goblin`). Plants: 119 on the library, 9 by hand on
the tool, all red. Left: the slime's attack still touches the cell
edge in one frame; the goblin's feet dip 0.03 at contact (foot
targets, task 6).

### Task 6: limbs and targets

**Produces:**

```c
/* plastica_membrum.h - pura */
nomen structura {
    b32 successus;        /* FALSUM: extra ambitum aut in radice */
    f32 angulus_radicis;  /* gradus, in plano membri */
    f32 angulus_medii;
} PlasticaMembrumSolutio;

PlasticaMembrumSolutio plastica_membrum_solvere (
    f32 longitudo_prima, f32 longitudo_secunda,
    f32 meta_ante, f32 meta_infra, b32 flexus_ante);
```

Skeleton: `<membrum titulus radix medium finis flexus>`. Pose:
`<meta membrum="crus_sin" locus="x y z"/>` in figure space, in place
of turns for those two joints (both given = refused). The strip
gains the foot-slide number for cycles titled `walk`: how far a limb
end travels over the ground while it is the lowest point, against
the world's walking speed.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: the solve on known triangles; limb
  end lands on the target within 0.001; knee bends the declared
  way; review focus 3; a target and a turn on the same joint
  refused; slide 0 for a fixture whose planted foot moves back at
  exactly walking speed.
- [ ] Implement; `./compile_tests.sh plastica_membrum`,
  `plastica_motus`, `plastica_specimen`.
- [ ] Goblin: `walk` redone with foot targets; `attack`, `hurt`,
  `die`. Claude reads strips; build into the live volume.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

**As built (2026-11-16).** As the header Fran approved, with one
change and two additions. Change: skeleton targets are scaled per
limb (offset from the limb's rest end x figure limb length /
skeleton limb length), not by root height - the goblin's feet were
out of reach under the header's rule. Additions: `inclinatio` on a
target (the foot keeps its rest orientation, pitched); the goblin
sheet carries its own `walk` of 0.29 s. New module
`plastica_membrum` (suite 3/3); motus suite 17 sections, specimen
18. bipes rewritten on targets: idle, walk, attack (moment `ictus`),
hurt, die. Goblin: walk lowest point -0.02 .. 0.00, slide 0.17.
135 library plants and 5 tool plants, all red. Details in
`lib/plastica_motus.worklog.md`.

### Task 7: the dials

`lenitas="linearis | lenis | tenet"` on a key; `<mora
titulus="joint" tempus="0.08"/>` in a cycle. Curved stretches are
sampled into linear keys at a fixed rate (builder's choice, a
constant); a cycle with neither dial bakes exactly as in task 2.

- [ ] Header text to Fran; wait for his word.
- [ ] Tests red against a stub: eased values at three times; held
  key is a step channel; a delayed joint equals the undelayed one
  shifted, and wraps in a looping cycle; a plain cycle's bytes are
  unchanged from task 2 (digest).
- [ ] Implement; polish the goblin and the slime with them.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.

### Task 8: names and moments through the GLB; the boar; close

- [ ] Header text to Fran (the reader's and writer's changed
  comments and signatures); wait for his word.
- [ ] Writer: moments as `extras` on the animation. Reader: joint
  names and moments returned beside the `Statua`. Tests red first:
  figure to GLB and back gives equal cycles, names and moments; a
  bought model (`fungus.glb`) reads as before.
- [ ] `plastica/ossa/quadrupes.stml` and the boar, five cycles.
- [ ] Spec status BUILT with an "As built" section; this plan's
  as-built notes; worklogs; memory.
- [ ] Plants, each red.
- [ ] Commit on Fran's word.
