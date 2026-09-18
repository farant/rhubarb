# adaptator lintris — implementation plan

> Execute INLINE on main, one task per turn, Fran approves each. No
> subagents. Steps use `- [ ]` for tracking.

**Goal:** a tier-2 lint finding becomes a located diagnostic, printed
by the same C printer as a tier-1 one.

**Ticket:** `01M2RNJ9XN`, ordered second by decree `01M2RNK5MG`, now
unblocked (`01M2RKYA4Z` impletum). Related finding: `01M2RYR3JJ`.

**Spec:** none. The record shape is already specified — 
`project-specs/sedes-multiplices-spec.md` §9 "As built" governs, and
this plan consumes it without changing it.

---

## What is already true (measured 2026-09-17, do not re-derive)

**1. The rule vocabulary already works.** `_relata` pops `lint` and
keeps every other attribute, so a rule written

```xml
<relatum lint="nt" gravitas="monitum" causa="'-nt' pro aequalitate"
         nota="hic comparatur">
```

arrives as `Congruentia.attributa == {'gravitas': 'monitum',
'causa': "'-nt' pro aequalitate", 'nota': 'hic comparatur'}`. Verified
by running it. **No C, no STML, no engine change** — the adapter reads
what is already there.

**2. The positions already exist and are discarded** (`silva.py:3607`).
`sedes` is the full `"L:C-L:C"` and `octeti` the full `"B-B"`; the code
keeps the start line/column and uses the byte pair only to slice
`textus_fontis`, then drops it. Six numbers in hand, two kept.

**3. The seam is the TSV.** Python produces the 12-field format; C
renders it. The C side never learns what a `Congruentia` is, so moving
row extraction into C later (`01M2RYR3JJ`) replaces the Python half
and emits the same TSV — the renderer, record shape and rule
vocabulary all survive.

---

## Global Constraints

- **C89 in Latin via `latina.h`**, house flags from `tools/vexilla.sh`;
  latina macros are forbidden identifiers; `i32`/`i64` UNSIGNED.
- **Never `silva.Editio` on a `.sh`**; edit runners with perl/Edit.
- **`silva.planta` cannot anchor in `.py`** — plant Python by hand.
- **Check new words with `./oratio/quaere.sh` BEFORE staging.**
- **`./silva/formator.sh <viae> -scribere` then `-vitia`** before every
  commit (C files only).
- **Every gate born red by a plant**, and **a fixture where two fields
  are EQUAL cannot discriminate them** — the dominant failure mode of
  the last two arcs. Pick fixtures where line ≠ column, start ≠ end.
- **When you change a format, run its CONSUMERS** — `tools/diagnostica`
  and `pythonica/probatio_silva.py` both read the TSV.
- Commit via `silva.commissio` with explicit paths, trailer
  `Co-Authored-By: Claude Opus 5 (1M context) <noreply@anthropic.com>`.
- **Never commit** `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`.

---

## Task 1: `tools/diagnostica` reads its own TSV

**Files:** Modify `tools/diagnostica.c`, `tools/diagnostica_fumus.sh`.

**Produces:** mode `-lege` — read 12-field TSV on stdin, render the
human format (header, causa, multi-span excerpt) by re-reading each
`via`. This is what lets a Python-produced finding reach `lib/excerptum`
without a second implementation of the caret.

- [x] **Step 1: parse a TSV row back into a `MateriaDiagnosticum`.**
Split on TAB, refuse a row without exactly 12 fields naming the row.
Field 11 splits on `;` then on the first `|`; related spans allocate a
`MateriaSedesRelata` array from the piscina. `nodus`/`lexema` stay
NIHIL — the renderer never needs them.

- [x] **Step 2: route.** `-lege` reads stdin instead of iterating
files; everything downstream is the existing
`_diagnosticum_imprimere`, unchanged. The grammar prefix comes from
the codex itself (a codex containing `:` is already printed bare).

- [x] **Step 3: the gate — ROUND TRIP, byte for byte.**

```bash
./tools/diagnostica.sh "$T/plantata.sh" > direct.out
./tools/diagnostica.sh -machina "$T/plantata.sh" | ./tools/diagnostica.sh -lege > per_tsv.out
diff direct.out per_tsv.out
```

They must be identical. This single assertion pins the whole
convergence: if the two paths ever diverge, one of them has grown its
own printer. Use a fixture with **two spans on different lines** so
the multi-span path is exercised, not just the single-span one.

- [x] **Step 4: plants.** (a) Drop field 11 on read → the excerpt loses
its second caret row → round trip differs. (b) Accept a row with the
wrong field count instead of refusing → feed a 10-field row and
confirm it is *not* silently accepted.

- [x] **Step 5:** formator, examen, `./tools/diagnostica_fumus.sh`,
`probatio_silva.py` (it reads this TSV), commit.

---

## Task 2: `Congruentia` stops discarding positions

**Files:** Modify `pythonica/silva.py` (`Congruentia` at `:3318`,
`_relata` at `:3351`, the row loop at `:3607`), `pythonica/probatio_silva.py`.

**Produces:** `Congruentia` carrying `linea_finis columna_finis
initium finis`, and **every** position in a row rather than the first.

- [x] **Step 1: keep the end.** Match the full
`(\d+):(\d+)-(\d+):(\d+)` from `sedes` and keep both byte offsets from
`octeti` instead of discarding them after the slice.

- [x] **Step 2: MEASURE how a rule expresses two positions, before
specifying anything.** Write a rule that captures two nodes in one
finding, run it, and read the expanded output. The open question is
where a second position can even live: `<PER>` emits one row per
match, `_relata` treats each child of `<relatum>` as one row, and
`_sedem_capture` takes the first `sedes`-bearing descendant and
ignores the rest. So a second span is either a second `sedes`-bearing
descendant inside one row, or it does not exist yet and the rule
language needs something.

Do not guess this from reading — the last arc's Task 1 found that the
fixture it needed already existed and the shape was not what the
ticket assumed. Record what the output actually looks like, then
write Step 3 against it.

- [x] **Step 3: collect every position in the row**, per Step 2's
measurement; the first is the primary. Labels for the related spans
come from whatever Step 2 shows is available to a rule author — do not
invent an attribute convention before seeing the output.

- [x] **Step 4: the gate.** Assert the end position and byte offsets
on a fixture where **line ≠ column and start ≠ end** — the
symmetric-fixture trap that cost two green plants last arc. Plus the
two-position case from Step 2, if Step 2 shows one exists.

- [x] **Step 5: plant by hand** (`silva.planta` cannot anchor `.py`):
keep only the first position → the two-position assertion goes red;
swap `linea_finis`/`columna_finis` → the end assertion goes red on the
asymmetric fixture.

- [x] **Step 6:** `probatio_silva.py`, commit.

**Executed 2026-09-17.**

**Step 2's measurement settled the convention by running it, and the
answer was not what reading suggested.** Three shapes, three results:
two insertions side by side in one element are **refused**
(`ARGUMENTUM_ARBOREUM` — a captured tree must be the sole content of
its element); two `<situs>` children of `<relatum>` give **two rows**,
i.e. two findings; one `<situs>` holding two wrapper elements gives
**one row with two positions**, and the wrappers can carry `nota=`.
So the convention already existed and needed no engine change:

```xml
<situs><hic nota="hic adhibetur">&@t;</hic>
       <ibi nota="in hoc imperio">&@i;</ibi></situs>
```

One child of `<relatum>` = one finding; each wrapper = one position;
`nota=` labels it; first is primary. The two shapes give the rule
author a real choice — two findings, or one finding pointing twice —
that falls out of the existing structure. `nota=` appears 0× in client
trees, so no collision.

**A bug I introduced and caught inside the task.** The first version
collected *every* `sedes`-bearing descendant, and an inserted node
carries its whole subtree — one command yielded **29** positions, and
the "second" span was trivia nested inside the first. Fixed by
suppressing descent once a position is captured, so each wrapper
contributes exactly one. Plant 3 now pins it.

**`Congruentia` mirrors `Diagnosticum`** (SM1 one layer up): flat
primary fields plus a `relata` tuple of `SedesRelata`. 7 fields → 13.
No consumer outside `silva.py` and its own gate.

Three plants: keep only the first position → 1 red; swap
`linea_finis`/`columna_finis` → 1 red, caught because the fixtures are
asymmetric (line 1, column 8/10; start ≠ end); remove the descent
suppression → **3** red including `(1 ordines, 29 relatae)`.

Audited: `probatio_silva` exit 0; `diagnostica_fumus` sanum; vocabula
NOVA 0.

---

## Task 3: the adapter

**Files:** Modify `pythonica/silva.py`, `pythonica/probatio_silva.py`.

**Produces:** `silva.diagnostica_lintris(viae, regula)` →
`[Diagnosticum]`, and TSV emission so the result can be piped to
`./tools/diagnostica.sh -lege`.

- [x] **Step 1: the mapping.**

| `Diagnosticum` | from |
|---|---|
| `via`, `linea`, `columna`, `linea_finis`, `columna_finis` | the row's primary position |
| `gravitas` | `attributa['gravitas']`, default `'erratum'` |
| `codex` | **`'lint:' + lint`** — self-prefixed like `materia:`, so the renderer prints it bare and the tier is visible in every line |
| `causa` | `attributa['causa']`, default the lint name |
| `textus` | `textus_fontis` |
| `nota` | `attributa['nota']` or None |
| `relata` | the row's remaining positions (Task 2 Step 2 decides how a rule labels them; if Task 2 finds no way to express a second position, `relata` is empty and that becomes a named gap, not an invented attribute) |

- [x] **Step 2: TSV emission** — the same 12 fields, same escaping
rule (`|`, `;`, TAB, NEWLINE → space). This is the seam; keep it
byte-compatible with what `tools/diagnostica -machina` writes.

- [x] **Step 3: the gate.** A rule over a crusta fixture produces a
`Diagnosticum` with the declared gravitas, `lint:` codex, cause and
label; a rule declaring none of them gets the defaults.

- [x] **Step 4: plant by hand** — read `attributa` but ignore
`gravitas` → the declared-severity assertion goes red.

- [x] **Step 5:** `probatio_silva.py`, commit.

**Executed 2026-09-17. A lint finding reached the printer for the
first time:**

```
lint.sh:1:8: [monitum] lint:nt-aequalitas
  '-nt' pro aequalitate adhibitum
   1 | test a -nt b
     | ^~~~~~~~~~~~ in hoc imperio
     |        ^~~ hic adhibetur
```

**`Diagnosticum` needed byte offsets.** It carried line/column only, so
the adapter could not emit a renderable TSV row — the printer works in
bytes. 11 fields → 13, mirroring the C `tractus`, which carries both.
`diagnostica_materiae` fills them from fields 6–7 it was already
parsing and discarding.

**Unlocatable findings are REFUSED, not dropped.** A row with no
captured node cannot become a located diagnostic, and letting it
vanish silently is the exact failure this whole arc exists to prevent.
`diagnostica_lintris` raises, naming the count and the lint. A rule
fixes it by capturing a node.

**Caught in review before running:** I had written
`'lint' in d.codex and d.codex or d.codex` in the TSV emitter — an
expression that always evaluates to `d.codex`. Removed.

Three plants: declared `gravitas` ignored → 1 red; `lint:` prefix
dropped → 3 red; the sine-sede refusal disabled → 1 red.

---

## Task 4: end to end, and the escape gate comes due

**Files:** Modify `pythonica/probatio_silva.py`,
`tools/diagnostica_fumus.sh`.

- [ ] **Step 1: the whole chain, once.** A rule over a `.sh` fixture →
`diagnostica_lintris` → TSV → `./tools/diagnostica.sh -lege` → the
compiler format with `^~~~`. Assert the rendered text. This is the
first time a lint finding has ever reached the printer.

- [ ] **Step 2: THE LABEL ESCAPE GATE, now buildable.** Spec §9 B and
AUDIENDA VII deferred it because no label contained an escaped byte.
A rule can now write `nota="a;b"`, so write the gate the spec owes:
field 11 must parse as **one** span, not two.

- [ ] **Step 3: plant** — drop the escape. The gate must go red. This
is the plant that stayed green in the last arc; it must not now.

- [ ] **Step 4:** remove the deferral from the spec (§9 B and AUDIENDA
VII become "built, and here"), run everything, commit.

---

## Closing

- [ ] Worklog or phase-log note — this is not a materia change, so a
  `pythonica` note plus the ticket record may be enough; decide by
  what a cold session would need.
- [ ] `01M2RNJ9XN` → `impletum`, commits noted.
- [ ] Update `01M2RYR3JJ` with what the TSV seam actually cost, so the
  C-extractor decision is informed when it arrives.
- [ ] Then, and only then: **write lint rules** (decree step 3).
  crusta has none.
