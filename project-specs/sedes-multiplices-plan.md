# sedes multiplices — implementation plan

> Execute INLINE on main, one task per turn, Fran approves each. No
> subagents. Steps use `- [ ]` for tracking.

**Goal:** a `MateriaDiagnosticum` can point at two places, and the
printer draws both in one block.

**Spec:** `project-specs/sedes-multiplices-spec.md` (`69f06895`). The
plan argues from the spec; both travel together. Where they differ the
spec governs.

**Architecture.** Four tasks. Tasks 1 and 2 are **independent** — the
record and walker live in materia, the printer in `lib/excerptum`,
which knows nothing about trees. Task 3 (the instrument) depends on
both; Task 4 (pythonica) depends on 3.

```
1 record + walker  ─┐
                    ├─→ 3 instrument (human + TSV) ─→ 4 pythonica
2 printer          ─┘
```

---

## Global Constraints

Every task's requirements implicitly include these.

- **C89 in Latin via `latina.h`**, house flags from `tools/vexilla.sh`.
  Latina macros are forbidden identifiers (`nomen`, `registrum`,
  `casus`, `magnitudo`, `duplex`…); single capitals `I V X L C D M` are
  Roman numerals.
- **`i32`/`i64` are UNSIGNED** (`s32`/`s64` signed). `chorda` is not
  NUL-terminated.
- **New C files via `./silva/scribe.sh`**; never `silva.Editio` on a
  `.sh`.
- **Check new words with `./oratio/quaere.sh w1 w2…` BEFORE staging** —
  untracked files are invisible to `-nova`. Already cleared for this
  plan: `relata relatorum nota coepit exspectatur multa sedes
  excerptum`.
- **`./silva/formator.sh <viae> -scribere` then `-vitia`** before every
  commit.
- **Every gate born red by a plant** (`silva.planta`), and the spec
  names the exact line each plant removes. **A plant that stays green
  is a dead gate** — the parent arc hit that three times, always a
  missing corpus case.
- **Docs first, gates last, then commit.** A gate already green on an
  unchanged tree is not rerun by `silva.commissio`.
- **Every materia writer/reader commit runs `./materia/shim_probare.sh`
  plus all client suites** (css, md, html, oratio, crusta). A
  substrate edit invalidates every client's build.
- Commit via `silva.commissio` with explicit paths, trailer
  `Co-Authored-By: Claude Opus 5 (1M context) <noreply@anthropic.com>`.
- **Never commit** `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`.

---

## Task 1: the record and the walker

**Files:**
- Modify: `materia/fontes/materia_diagnostica.h` (the two structs)
- Modify: `materia/fontes/materia_diagnostica.c` (`_addere`, the
  `ABSENTIA` arm at `:300`, the `emissa` loop at `:534`)
- Test: `materia/probationes/probatio_materia_diagnostica.c`
- Test: `crusta/probationes/probatio_crusta_diagnostica.c`

**Produces:** `MateriaSedesRelata`; `MateriaDiagnosticum.nota`,
`.relata`, `.numerus_relatorum`; constants `MATERIA_NOTA_COEPIT`,
`MATERIA_NOTA_EXSPECTATUR`.

- [ ] **Step 1: find a fixture that PROVABLY yields an `absentia`.**

Do not assume a shape produces one. Run the instrument and read the
codices:

```bash
printf 'if true; then\n  echo x\n' > /tmp/ap.sh
./tools/diagnostica.sh /tmp/ap.sh -machina
```

Pick a case whose codex maps to an `absentia` declaration in
`crusta/grammatica/*.stml`. Record the file, the codex and the node's
start offset in the worklog — Step 6 pins that offset.

*This step exists because the three green plants in the parent arc were
all the same mistake: a gate over a corpus that lacked the case.*

- [ ] **Step 2: the header.**

```c
/* Sedes cognata: locus alter quem diagnosticum nominat, cum nota sua.
 * 'nota' verbum FINITUM est, non participium: subiectum per genera
 * variat (grex masculinum, redirectio femininum), verbum cum nullo
 * convenit. */
nomen structura {
        MateriaTractus  tractus;
    constans character* nota;      /* NIHIL licet */
} MateriaSedesRelata;
```

and in `MateriaDiagnosticum`, after `tractus`:

```c
            constans character* nota;      /* nota sedis primariae */
    constans MateriaSedesRelata* relata;   /* NIHIL = ut ante */
                           i32  numerus_relatorum;
```

plus the two label constants beside the existing `MATERIA_CODEX_*`:

```c
#define MATERIA_NOTA_COEPIT       "hic coepit"
#define MATERIA_NOTA_EXSPECTATUR  "hic exspectatur"
```

- [ ] **Step 3: `_addere` zeroes the new fields.**

`_addere` (`:44`) writes every field today; add the three so a
diagnostic never carries stack garbage:

```c
    r->tractus            = *tractus;
    r->nota               = NIHIL;
    r->relata             = NIHIL;
    r->numerus_relatorum  = ZEPHYRUM;
```

- [ ] **Step 4: the `ABSENTIA` arm fills them.**

Replace the `_addere` call in the `ABSENTIA` case (`:307`) with a
helper that also allocates one `MateriaSedesRelata` from `d->piscina`:

```c
casus MATERIA_DIAGNOSTICUM_ABSENTIA:
    valor = &nodus->loci[r->locus];
    si (   valor->genus == MATERIA_VALOR_NIHIL
        || (   valor->genus == MATERIA_VALOR_REFERENTIA
            && valor->datum.nodus == NIHIL))
    {
        finis = habet ? _punctum_finis(&t) : punctum;
        _absentiam_addere(d, r, nodus, &finis,
            habet ? &t : NIHIL);
    }
    frange;
```

`_absentiam_addere` calls `_addere`, then sets
`nota = MATERIA_NOTA_EXSPECTATUR` and, when the opening range is not
NIHIL, allocates one `MateriaSedesRelata` holding it with
`MATERIA_NOTA_COEPIT`. **`GENUS` and `VACUA` are untouched** — neither
has a second position in hand.

- [ ] **Step 5: `emissa` passes related spans through unchanged.**

In the `emissa` loop (`:534`), copy `nota`, `relata` and
`numerus_relatorum` from the client's record. **The `initium == -I`
auto-compute stays primary-only** — it derives from `nodus`, and a
related span points elsewhere by definition.

- [ ] **Step 6: the gate, then its plant.**

In `probatio_materia_diagnostica.c`, over the Step 1 fixture, assert
**both**:

```c
CREDO_AEQUALIS_I32 (d->numerus_relatorum, I);
CREDO_AEQUALIS_S32 (d->relata[ZEPHYRUM].tractus.initium, <nodi initium>);
CREDO_VERUM (strcmp(d->nota, MATERIA_NOTA_EXSPECTATUR) == ZEPHYRUM);
```

The offset assertion is not optional: **the count alone passes if the
span is filled with the wrong range.**

Also assert the negative: a `GENUS` or `VACUA` diagnostic still has
`numerus_relatorum == 0` and `nota == NIHIL`, so §1's byte-identity
claim is measured, not asserted in prose.

- [ ] **Step 7: run the plant.** Delete the `relata` assignment inside
`_absentiam_addere`. The gate must go **red on the count**. Then break
only the offset (point the span at `finis` instead of `t`) and confirm
it goes red on the offset too. Restore.

*Two plants, because one assertion can hide behind the other.*

- [ ] **Step 8: format, audit, commit.**

```bash
./silva/formator.sh materia/fontes/materia_diagnostica.h \
    materia/fontes/materia_diagnostica.c -scribere && \
./silva/formator.sh materia/fontes/materia_diagnostica.h \
    materia/fontes/materia_diagnostica.c -vitia
./materia/shim_probare.sh
```

Then all six client suites, then commit.

---

## Task 2: the printer

**Files:**
- Modify: `include/excerptum.h`
- Modify: `lib/excerptum.c`
- Test: `probationes/probatio_excerptum.c`

**Produces:** `ExcerptumSedes`, `excerptum_scribere_multa`.

**Consumes:** nothing from Task 1 — this file knows nothing about
trees and can be done first or in parallel.

- [ ] **Step 1: extract the width, which is currently hidden.**

`_marginem` (`lib/excerptum.c:7`) computes its own width from `linea`
via `latitudo = n < IV ? (i32)IV : n`. Multi-span needs one width for
the whole block, so split it:

```c
interior i32
_figurae (i32 linea, character* figurae);   /* reddit numerum */

interior i32
_latitudo (i32 linea);                      /* max(IV, _figurae) */

interior vacuum
_marginem (ChordaAedificator* exitus, i32 linea, b32 numerus,
           i32 latitudo);                   /* latitudo PARAMETER */
```

`excerptum_scribere` passes `_latitudo(linea)` and is otherwise
unchanged.

- [ ] **Step 2: split the two rows out of `excerptum_scribere`.**

Extract `_lineam_scribere` (the source line, `:78-89`) and
`_signum_scribere` (the caret row, `:90-120`), both taking
`principium`, `terminus` and `latitudo`. `_signum_scribere` gains a
`constans character* nota` and appends `' '` then the label before the
newline when it is not NIHIL.

`excerptum_scribere` becomes: compute bounds, call both with
`nota = NIHIL`. **No behaviour change** — Step 3 measures that.

- [ ] **Step 3: pin the identity BEFORE adding the new entry point.**

In `probatio_excerptum.c`, assert the existing outputs are unchanged
by the refactor. Run the suite. If anything moved, the split is wrong
and the rest of this task is built on sand.

- [ ] **Step 4: the new entry point.**

```c
nomen structura {
                    s32  initium;
                    s32  finis;
                    i32  linea;
    constans character* nota;      /* NIHIL licet */
} ExcerptumSedes;

b32
excerptum_scribere_multa (
         ChordaAedificator* exitus,
        constans character* fons,
                       i32  mensura,
    constans ExcerptumSedes* sedes,
                       i32  numerus);
```

Body: refuse (`numerus == ZEPHYRUM`, any `initium` outside
`[0, mensura]`, any `linea == ZEPHYRUM`, or `initium` not
non-decreasing). Compute `latitudo` as the max of `_latitudo(linea)`
over the array. Walk the array; for each run of equal `linea`, call
`_lineam_scribere` once, then `_signum_scribere` per span.

**Sortedness is required, not repaired** — no allocation, no mutating
a const array, and a loud refusal instead of silently reordered
output.

- [ ] **Step 5: four gates.**

| gate | asserts |
|---|---|
| identity | `numerus == I` and `nota == NIHIL` ⇒ byte-identical to `excerptum_scribere` on the same span |
| same line | two spans on one line ⇒ **one** source line, two caret rows |
| gutter | spans on lines 999 and 1000 ⇒ both rows width V |
| refusal | a descending array ⇒ FALSUM |

- [ ] **Step 6: plants.** Remove same-line grouping → the same-line
gate goes red with two source lines. Revert `latitudo` to per-line →
the 999/1000 gate goes ragged. Drop the sortedness check → the refusal
gate goes red. Restore each.

- [ ] **Step 7: format, run the root suite, commit.**

---

## Task 3: the instrument

**Files:**
- Modify: `tools/diagnostica.c`
- Modify: `tools/diagnostica_fumus.sh`

**Consumes:** `MateriaDiagnosticum.relata` (Task 1),
`excerptum_scribere_multa` (Task 2).

- [ ] **Step 1: the human format routes through the multi-span
printer.** Build an `ExcerptumSedes` array — primary first, then
`relata` — sort by `initium`, call `excerptum_scribere_multa`.

Sorting is the instrument's job, not the printer's: the record stores
the primary separately, so the array is only sorted by construction
when the related span happens to follow.

- [ ] **Step 2: TSV fields 11 and 12.**

| # | field |
|---|---|
| 11 | related spans, `L:C-L:C\|nota` entries joined by `;`; empty when none |
| 12 | the primary's `nota`; empty when NIHIL |

A vertical bar, semicolon, TAB or NEWLINE in a label becomes a single
space before writing. Lossy, never corrupting.

**Appended, never extra rows.** One row stays one diagnostic — extra
rows would inflate every count that greps the file, including
`diagnostica_fumus.sh` gate I.

- [ ] **Step 3: extend the smoke test.** Add a gate asserting a label
containing `;` produces **one** parseable span in field 11, not two.
Verify gates I–IV still pass unchanged — they grep mid-line, so
appended columns must not disturb them.

- [ ] **Step 4: plant.** Drop the escape. The new gate goes red.
Restore.

- [ ] **Step 5: run `./tools/diagnostica_fumus.sh`, format, commit.**

---

## Task 4: pythonica

**Files:**
- Modify: `pythonica/silva.py` (`Diagnosticum` at `:3435`,
  `diagnostica_materiae` at `:3439`)
- Test: `pythonica/probatio_silva.py` (`:1589`)

**Consumes:** the twelve-field TSV (Task 3).

- [ ] **Step 1: the shape.**

```python
SedesRelata = namedtuple('SedesRelata',
                         'linea columna linea_finis columna_finis nota')

Diagnosticum = namedtuple('Diagnosticum', 'via linea columna linea_finis '
                          'columna_finis gravitas codex causa textus '
                          'nota relata')
```

`relata` is a tuple, empty when field 11 is empty. `nota` is `None`
when field 12 is empty.

- [ ] **Step 2: parse fields 11 and 12.** Split on `;`, then on the
first `|`. An entry without a `|` has no label.

- [ ] **Step 3: the gate.** Extend the block at `:1589` over a fixture
with an `absentia`: assert one related span, its line and column, and
that `nota` is the expected string.

- [ ] **Step 4: plant.** `silva.planta` cannot plant in `.py` (its
anchors are C tokens) — **plant by hand**, as in the parent arc: drop
the field-11 parse, confirm red, restore.

- [ ] **Step 5: run `probatio_silva.py`, commit.**

---

## Closing

- [ ] `materia/phase-log.md`: RELATIO + DEBRIEF INSTRUMENTORUM.
- [ ] Spec §16-style "As built" section if anything diverged.
- [ ] `01M2RKYA4Z` → `impletum`; note the commits.
- [ ] Unblock `01M2RNJ9XN` (the `Congruentia` adapter) — the shape it
  was waiting on now exists.
