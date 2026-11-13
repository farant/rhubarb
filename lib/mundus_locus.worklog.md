# mundus_locus worklog

## 2026-11-12 - plastica T9: the layout and the furnished place

- **What it is.** A place in `$world` is now built from a layout
  (`apps/mundus/loci/*.stml`): the bare level's GLB and a list of
  plastica assets with a spot (x z) and a turn. `mundus_locus_legere`
  reads it; `mundus_locus_struere` puts each asset at the ground's
  height and joins everything (`statua_iunctio`) into one `Statua`,
  which is an ordinary place. The library does not include plastica:
  the caller compiles the assets.
- **Height** comes from the walk map of the BARE level under the
  asset's origin. A prop on a slope stands at the height of its
  origin and is not tilted. A prop cannot stand on another prop.
- **Three refusals for a bad spot** (outside, water, not walkable),
  each naming the placement's line. First bad placement wins and
  nothing is built.
- **The solid rule (spec 14a).** A prop with a solid mark must close
  at least one walkable cell. Checked on the prop's OWN walk map:
  the prop alone, placed, joined onto an empty `Statua`
  (`statua_finire` accepts one with no nodes), so the check costs a
  map of the prop, not of the level. A solid cell counts if the bare
  level is walkable at that cell's centre and the prop's top is at
  or above the ground there. This is exact because every walk map's
  grid starts at a whole unit (`floor` of the bounds), so the cells
  of the prop's map are the cells of the level's map.
- **Answers to the spec's open questions.**
  - AUDIENDA 3 (an unmarked crown over grass): test `coma` - a tree
    with a 3-unit unmarked crown and a 0.5 unseen blocker closes
    exactly 4 cells; the grass under the crown stays walkable.
  - AUDIENDA 4 (a character behind a tall prop): test `occultatio`
    - 2583 body pixels in front of a 3 x 5 x 3 box, 0 behind it.
    The view looks along the x - z diagonal, not x + z: the first
    version of the test put both points at the same depth.
  - AUDIENDA 4a (the unseen blocker is cut out): seen in the well's
    pictures; the blocker never shows.
  - AUDIENDA 6 (does an open place reload when replaced): yes.
    `locum_curare` in `lib/mundus_latus.c` compares the place's seal
    and reloads when it differs; test `substitutus` in
    `probatio_mundus_latus`. The character is put back at the start
    spot on reload (the body is cleared with the place).
- **Constants tied.** `plastica_specimen.h` now names its
  three-quarter view (`PLASTICA_SPECIMEN_GYRUS`, `_INCLINATIO`); the
  test `constantes` holds them and `PLASTICA_CELLA_VESTIGII` equal
  to mundus's view and cell. plastica still does not include mundus.

### The tool (`tools/mundus.c`)

- `mundus condere -volumen V [-loci DIR]`: one line per layout with
  the walkable cell count before and after, so a prop that closes
  more than meant shows at once (the well: 60626 -> 60574).
- `mundus specimen RES -o PNG [-locus "x z"] [-gyrus G]`: the asset
  in the bare clearing with the mushroom man beside it, two panels
  (Atkinson ground, exemplaria ground). The character goes to the
  standable spot, among 16 directions at the asset's radius + 1,
  that lies furthest right on screen, so it never covers the asset.
  About half a second.
- A layout's paths are relative to the working directory (the repo
  root).

Plants: see the T9 commit.

## 2026-11-12 - plastica T10: the furnished clearing

`apps/mundus/loci/clearing.stml` places 42 props on the bare level.
Numbers (pinned in test `pratum` of `probatio_mundus_locus`):
walkable cells 60626 -> 60043; reachable from the start 42881 ->
46683, of which 5405 are on the top plateau, which no stair of the
kit reaches - ours does. Without the top plateau fewer cells are
reachable than on the bare level (props only close cells), which the
test also holds. Removing the stair from the layout turns the test
red (top plateau 0).

How the spots were chosen: a throwaway test program (kept out of the
tree) drew the walk map at 2 pixels per cell with heights and
reachability and rendered frames with the mushroom man at chosen
spots. `bin/mundus condere` names a bad spot with its line (two
props were first put in ponds).

Still unreachable, as on the bare level: the north-east corner and
the plateau south of the hut.

## 2026-11-12 - variation per placement (plastica II, task 2)

After T10 every pine was the same pine. Three additions to the
layout:

- **`magnitudo` on a `<res>`** (0.25 .. 4): a uniform scale. It goes
  to the joiner as `StatuaIungenda.scala`, which sets the scale of
  the prop's new root node, so the prop grows about its own origin
  and still stands on the ground. The solid rule is checked on the
  scaled prop.
- **`semen` on a `<res>`**: added to every seed inside the asset
  (`plastica_legere_seminis`), so one file gives another rock shape
  or texture patch. The caller compiles once per (file, seed); each
  distinct seed is its own model in the place.
- **`<sparsio>`**: scatter N copies in a rectangle.
  `mundus_locus_explicare` turns scatters into plain placements
  before the build, using the bare level's walk map: a candidate is
  dropped if it is not walkable ground or closer than `distantia` to
  any placement so far. Explicit `<res>` come first in the list
  wherever they are written, so a scatter avoids all of them. Up to
  40 candidates per copy; a scatter that cannot place all its copies
  is REFUSED with its line and the number that fit, not silently
  shortened. Candidates come from an integer hash of (seed, attempt,
  channel), so the same layout gives the same place on any machine.
- **`varietates`** on a scatter cycles the asset seed 0 .. n-1 over
  the copies, which bounds how many distinct models a scatter adds.
- **Zero is not "default" in the structs.** `MundusLocusRes.scala`
  and `StatuaIungenda.scala` must be positive; the reader fills 1.
  A struct made with memset is refused with the placement's line.
  This cost two test helpers a line each and keeps a forgotten
  field from quietly meaning "normal size".
- `mundus_solum_altitudo` already fails on anything that is not
  walkable, so the separate genus test before it in `explicare` is
  redundant; kept because it reads as the rule.

The clearing now uses all three: 71 props (42 before), walkable
59666, reachable 45525, top plateau 5332. The pins in test `pratum`
were copied from `bin/mundus mappa` and matched on the first run.
One scattered pine (number 52 on the map) stands in the gap between
the two south-west ponds and closes it; the land beyond is still
reached from the south.
