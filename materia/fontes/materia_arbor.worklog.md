# materia_arbor worklog

## 2026-09-17 — sedes A1: the positioned view (materia-sedes-plan A1)

**What.** `MateriaArborConsilium.sedes_scribere` (last field) makes the
writer stamp `sedes="L:C-L:C"` and `octeti="B-B"` on every node element,
token element and trivium element; locus wrappers, `ante`/`post`, and
the envelope get nothing. The envelope says `visio="sedes"` (or
`"partialis sedes"` with `loci_admissi`), and the reader refuses it with
a cause naming the value (`"visio sedes: non arbor"`; the old
`"visio partialis: non arbor"` is byte-identical for the filter alone,
same attribute order `visio`, `omissi`).

**The range lives in `materia_nodus`, not here.** `materia_tractus_lexematis`
/ `_nodi` / `_conferre` sit beside `materia_sedes_tokeni`, because B's
derivation walker and the writer's refusal ranges need them without the
writer. Start = the effective position (through the origin hook); end =
start advanced over the value by the reader's own cursor rule (`\n` →
next line, column 1; any other byte → column + 1). A token whose hook
says `est_fons` FALSUM (md and oratio derive tokens today) is a
zero-width POINT: its value is not the bytes at that place.

**Node ranges are accumulated bottom-up in the one walk.** The scriptor
carries `tractus` + `tractus_inventus`; `_scribere_nodum_internum` saves
the parent's pair, resets, lets every token use (the transclusion path
too — the node owns the token even when its element is a reference)
merge into it, stamps its own element after the loci, then merges its
range into the saved parent pair and restores. O(n); calling
`materia_tractus_nodi` per node would be O(n·depth). Trivia never merge:
they hang off tokens, not loci, so a node's range never starts at the
comment above it. `materia_tractus_conferre` keeps the first fons it
sees (only C89 includes can mix fontes).

**Measured / noticed while building.**
- Every `MateriaArborConsilium` in the tree (sources, probationes,
  instruments, briar, the C89 shim) is filled by `materia_arbor_consilium_nudum`
  or a helper that calls it; the one struct copy (`cv = c`) copies such
  a consilium. The new field is therefore zero wherever nobody sets it.
- `sedes_colligere` / `paria` are collected but `sedes_valorum` is never
  filled (D2) — left alone; A2's gate pairs elements by document order.
- A test comment holding `/*` broke the build (`-Wcomment`) exactly as the
  memory warns; examen's "domesticum" for `strstr` on a chorda's data does
  not block, but the new gate uses `chorda_ut_cstr` and adds none.
- Gate `probatio_materia_sedes`: 63/63 at first compile; planted
  `finis + I` in `materia_tractus_lexematis` → red at `t.finis == 13`,
  reverted → green.

## 2026-09-17 — sedes A2: the shared verifier, five client gates, instruments

**`materia_sedes_verificare` (new `materia/fontes/materia_sedes.{h,c}`).**
One library call every client STML gate makes per corpus file, like
`materia_registrum_recens`. Its oracle is disjoint from the writer's
arithmetic: it builds a LINE TABLE from the source bytes (binary search
for line/column), computes expected ranges by its own walk (writer
order: node pre-order; a first-use token, then its `ante`, then its
`post` trivia; a repeated use adds no element but still counts toward
the node), pairs them with the STML elements carrying `octeti` in
document pre-order, checks `octeti`, `sedes`, and for source tokens the
source slice against the value. It also checks the view equals the
plain projection minus `sedes`/`octeti`/`visio` (structural walk over
both arbor-tradita trees, two interns so titles compare by content),
and that the reader refuses the view. It refuses by name what it cannot
pair: `loci_admissi`, `templa_activa`, a frons with `liberos_ornare`.

**Measured over the corpora (all green at first run):** crusta 339,503
elements; css 9,679; html 16,439; md 2,491,843; oratio 2,643,746 — zero
divergences. **Zero derived points anywhere:** md's and oratio's derived
tokens carry `byte_offset` −1 (synthetic), so they get no attributes at
all. D1's point rule is exercised only by materia's own gate today.

**Size.** Over the 237 house `.sh` files the view is 57,459,770 bytes
against 23,264,631 for the plain projection (×2.47; the source is
826,532 bytes). Most of the growth is the pretty writer breaking an
element with two long attributes onto aligned lines. Opt-in; not pinned.

**Instruments.** `-sedes` on `crusta/`, `html/`, `md/`, `oratio/arbor.sh`
(`consilium.sedes_scribere = sedes` right before the write).

**Plant.** Writer column `+ I` → `probatio_crusta_stml` red at
`sedes.sana`; reverted → green.

**First commit attempt refused (the gate working).** The oratio
identifier lint in the shadow clone saw the untracked file for the first
time and named `verificatio` (the struct `Verificatio`) as a new unknown
word — `quaere.sh` had been run on `verificare`, not on the noun. Renamed
`Collatio`; the two-letter locals of the structural compare (`ia`/`ib`,
`na`/`nb`, `sn`/`so`) became full words before the retry. Lesson: run
`quaere.sh` on EVERY identifier word of a new file (a regex split of the
file's identifiers), not on the words remembered as new.

**Second refusal: crusta's computus golden.** `crusta/probationes/fixa/computus/basis.tsv`
measures five FIXED house files, one of them `html/compile_probationes.sh`,
which A2 edits (its materia module list gained `materia_sedes`). The
row moved by exactly that edit (+25 bytes, +1 token, +2 nodes, +13
allocations); regenerated with `COMPUTUS_SCRIBERE=1`, the other four
rows unchanged. A runner edit is a corpus edit for crusta's computus
gate — run `crusta ... computus` before committing any change to those
five files.

## 2026-09-17 — A1's struct change left stale objects in five client builds

A1 appended `sedes_scribere` to `MateriaArborConsilium`. The client
runners (css, md, html, oratio, crusta) rebuild an object when its
`.c` is newer or when any header in a HAND-WRITTEN list is newer, and
that list was `include/` plus the client's own `fontes/` and
`probationes/` — never `materia/fontes`, the substrate every one of
them compiles. So nothing that merely included `materia_arbor.h` was
rebuilt. After today's runs `excubitor.sh` still counted oratio 24,
html 20, md 14, css 10, crusta 9 stale objects (23 of oratio's behind
`materia_nodus.h`/`materia_arbor.h`, with its runner freshly run).

It was more than cosmetic. `oratio_resolutio.c:2043` holds a
`MateriaArborConsilium` on its stack; the stale object reserved the
old size, then called down to the fresh `materia_arbor_consilium_nudum`,
which writes the new last field past the end — undefined behaviour
in every local oratio run since A1. The commits were never at risk:
shadow gates build from scratch. And measured afterwards, the oracle's
873 rows on fully rebuilt objects are byte-identical to the morning
baseline taken on the stale ones, so the overwrite happened to land on
something harmless.

How it surfaced: A3's re-measure. `oratio/oraculum.sh` links
`oratio/build/*.o` and relinked only when an `oratio_*.o` was newer,
so a `lib/stml_macros.c` change gave "rows identical" from the
previous day's binary. Sixteen instruments across crusta, html, md
and oratio had the same prefix-only glob; their siblings
(`briar/arbor.sh`, `oratio/reticulum.sh`, every `computus.sh`) already
watched `*.o`.

Fix, three layers:
- the five guards list `materia/fontes` (html and oratio also
  `md/fontes`, which they compile);
- after the object loops each runner calls `./excubitor.sh
  <cliens>/build/ -tacitus`, whose include graph is DERIVED
  (`build/inclusiones.tsv`), so the next forgotten directory is caught
  without anyone remembering to add it: stale → print the report and
  exit 2 before any test compiles; excubitor unable to judge (graph
  absent) → a loud CAUTIO;
- the 16 instruments relink on any newer `*.o`, and both `oraculum.sh`
  refuse stale objects with exit 2, naming the runner to run. The
  per-file `arbor.sh` instruments do not check: `silva.exemplaria`
  calls them hundreds of times and the check costs about half a second.

Born red: crusta's runner with its old guard planted back left 8
objects stale behind materia headers and stopped with exit 2 and
"NIHIL CURSUM"; restored, it rebuilt 15 objects and ran. Both oracles
refused with exit 2 on one backdated object. Touching a non-prefix
object (`stml_macros.o`) relinked the oracle binary. All five runners
then left excubitor PURUS. Crusta's computus golden moved on its
`html/compile_probationes.sh` row only (8233 → 9525 bytes, 1517 → 1620
tokens, 1583 → 1697 nodes, 8456 → 9052 allocations): the runner is one
of its measured house files.

Lesson: a guard built from a hand list of directories drifts the day
the dependency graph grows; a guard that reads the derived graph does
not. Check with the derived one, keep the hand list as the fast path.
