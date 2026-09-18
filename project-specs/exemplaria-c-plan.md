# exemplaria in C — implementation plan

> Execute INLINE on main, one task per turn, Fran approves each. No
> subagents. Steps use `- [ ]` for tracking.

**Goal:** tier-2 lint findings are extracted in C, so a client's
diagnostics — both tiers — are one call behind that client's own
facade, reachable from the commit path without Python.

**Architecture:** materia gains a client-blind `<relatum>` extractor
and a shared orchestration sequence parameterised by a config the
CLIENT constructs. crusta gains a three-level facade. `tools/diagnostica`
becomes a thin suffix dispatcher. Python's extraction chain is deleted
only after the C reproduces it exactly over the whole corpus.

**Spec:** `project-specs/exemplaria-c-spec.md` (`7cde4d8a`, revised
`88b5a06d`). Read it first — decisions E1–E8 govern, and E1 (ownership
of the config) and E3 (facade) are the ones easiest to get subtly
wrong.

**Ticket:** `01M2RYR3JJ`; decree `01M2S9GR5W` is overtaken by three
triggers, the third being that the commit path must stay Python-free.

---

## What is already true (measured 2026-09-18, do not re-derive)

1. **Every stage but extraction is a library call, tree to tree.**
   `crusta_arbor_parsare` → `materia_arbor_proicere_nodum`
   (`consilium.sedes_scribere` = VERUM, handing back
   `MateriaArborScriptura.arbor`) → `stml_expandere(radix, piscina,
   intern)`, which takes a tree and returns a tree. No text hop.
2. **`stml vertere` has exactly one caller**, `silva._relata` — it
   exists solely so Python's `html.parser` can read our own STML.
3. **`TRANSPARENTIA` is per-substrate**: `('ante post', 'sedes octeti')`
   for every materia client.
4. **The printer is generic but `interior`** in `tools/diagnostica.c`;
   the excerpt half already builds a `ChordaAedificator` and only then
   `fwrite`s. Only the header line goes straight to stdout.
5. **`crusta_diagnostica()` has four callers** — `tools/diagnostica.c`,
   `crusta_diagnostica.c`, `probatio_crusta_differentia.c`,
   `probatio_crusta_diagnostica.c`. The facade must not break them.
6. **Corpus for the differential gate: 232 `.sh`** (`git ls-files
   '*.sh'` minus `probationes/fixa/`).
7. **`xar_extendere` DOES NOT EXIST.** The spec's pseudocode invented
   it. Appending one Xar to another is a loop over `xar_obtinere` +
   `xar_addere`.

---

## Global Constraints

- C89 in Latin via `latina.h`; flags from `tools/vexilla.sh`.
- **`i32`/`i64` are UNSIGNED**; `s32`/`s64` for anything signed.
  `MateriaTractus.initium` is `s32` because `-I` is a value.
- `chorda` is NOT NUL-terminated.
- Latina macros are forbidden identifiers (`nomen`, `casus`,
  `magnitudo`, `ratio` is NOT a macro and is fine). Single capitals are
  Roman numerals.
- New C files via `./silva/scribe.sh`, never `Write`.
- `./silva/formator.sh <viae> -scribere` then `-vitia` before commit;
  banners are 50 `=`, lines ≤ 72.
- `./oratio/quaere.sh w1 w2…` on every new word BEFORE staging.
- **Every gate born red by a plant** (`silva.planta`); a gate that
  cannot fail is DEFERRED WITH A NAMED TRIGGER.
- **A fixture where two fields are EQUAL cannot discriminate them.**
- **Assert the replacement count on every scripted edit** — the
  formator reindents, and an exact-match replace that matches nothing
  must raise, not shrug.
- Commits via `silva.commissio(nuntius, viae, portae)`. **Never**
  commit `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`. Never touch
  `project-specs/exempla/kalendarium.html` or `salutatio2.html`.
- Every materia commit: `./materia/shim_probare.sh` + all five client
  suites.
- Trailer: `Co-Authored-By: Claude Opus 5 (1M context) <noreply@anthropic.com>`

---

## File structure

| file | responsibility |
|---|---|
| `materia/fontes/materia_exemplaria.{h,c}` | **new** — expanded tree + rule → `MateriaDiagnosticum` rows. Client-blind (E2). |
| `materia/fontes/materia_pictor.{h,c}` | **new** — `MateriaDiagnosticum` → `chorda`. Composes `lib/excerptum`; never writes to stdout (E4). |
| `materia/fontes/materia_diagnostica.{h,c}` | **modify** — add `MateriaDiagnosticaRatio` + `materia_diagnostica_plena` (E1). |
| `crusta/fontes/crusta_facies.{h,c}` | **new** — the three-level facade (E3). |
| `tools/diagnostica.c` | **modify** — printer moves out; `main` becomes suffix dispatch (E7). |
| `pythonica/silva.py` | **modify** — extraction chain deleted, face becomes a shell (E6). |
| `tools/unci-git/pre-commit` | **modify** — tier 2 joins tier 1. |

**Placement decision, which E4 left to this plan:** the composer goes
in **materia**, not the instrument layer. Reason: `lib/excerptum`'s own
header says form "can be called from any source" and is tree-free, so
the rule forbids reimplementing form in materia, not composing it; and
the composer needs `MateriaDiagnosticum`, so `lib/` would have to
depend on materia. It returns a `chorda` and never performs I/O — the
instrument owns stdout.

---

## Task 1: `materia_exemplaria` — the extractor

**Files:**
- Create: `materia/fontes/materia_exemplaria.h`, `.c` (via `./silva/scribe.sh`)
- Create: `materia/probationes/probatio_materia_exemplaria.c`
- Create: `materia/probationes/fixa/exemplaria/` (golden rows, see Step 5)

**Produces:** `materia_exemplaria_applicare`, consumed by Task 3.

- [ ] **Step 1: the header.**

```c
/* Documentum EXPANSUM -> ordines inventorum. CLIENTE CAECUM: legit
 * 'sedes'/'octeti' (quae scriptor materiae uniformiter emittit) et
 * 'lint'/'gravitas'/'causa'/'nota' (quae auctor regulae scribit).
 * Plagulam, suffixum, grammaticam NUMQUAM videt. */
Xar*                                  /* de MateriaDiagnosticum */
materia_exemplaria_extrahere (
              Piscina* piscina,
            StmlNodus* expansum,      /* post stml_expandere */
   constans character* fons,          /* pro textu inventi */
                  i32  mensura);

/* Documentum componere: proiectio + TRANSPARENTIA (si regula nullam
 * declarat) + elementa regulae. Arborem NOVAM reddit; neutrum
 * argumentum mutatur. */
StmlNodus*
materia_exemplaria_componere (
              Piscina* piscina,
            StmlNodus* proiectio,
            StmlNodus* regula,
  InternamentumChorda* intern);
```

- [ ] **Step 2: the probatio, hand-built trees, and make it fail.**

Build an expanded document by hand with `stml_legere_ex_literis` —
this isolates extraction from parsing and projection:

```c
constans character* DOC =
    "<relatum lint=\"x\" gravitas=\"monitum\" causa=\"c\">"
      "<situs>"
        "<a nota=\"hic\" sedes=\"1:8-1:11\" octeti=\"7-10\">-nt</a>"
        "<b nota=\"ibi\" sedes=\"1:1-1:13\" octeti=\"0-12\">"
          "<c sedes=\"1:2-1:3\" octeti=\"1-2\">x</c>"
        "</b>"
      "</situs>"
    "</relatum>";
```

Assertions: one row; `codex == "lint:x"`; `gravitas` monitum; primary
span `7-10` with `nota == "hic"`; exactly ONE related span (`0-12`,
`nota == "ibi"`) — **`<c>` must NOT appear**, because descent stops at
a captured node. That last assertion is the descent-suppression law
and it is the one that matters.

Run: `./materia/compile_probationes.sh exemplaria` → FAIL (link).

- [ ] **Step 3: implement the walk.**

```
per <relatum> summum R:
    lint/gravitas/causa/nota <- attributa R
    per liberum elementum C ipsius R:        /* ordo unus per liberum */
        _sedes_colligere(C, nota_ambiens, series)
        prima = primaria; ceterae = relatae
```

`_sedes_colligere(nodus, nota_ambiens, series)`: if the node carries
`sedes`, append `(tractus, nota_ambiens)` and **RETURN without
descending**; else recurse into children with
`nodus.nota ? nodus.nota : nota_ambiens`.

Recursion carries what Python's `notae` stack and `gradus_capti`
carried by hand — do not port those.

- [ ] **Step 4: run, expect PASS.**

- [ ] **Step 5: golden fixtures from the Python extractor.**

Pick three real files of different shapes and freeze the Python rows:

```bash
python3 -c "
import sys; sys.path.insert(0,'pythonica')
import silva
x = silva.exemplaria(['tools/vexilla.sh','gesta/fori.sh','crusta/coctum.sh'],
                     'crusta/lintrum/nt-aequalitas.stml')
print(silva.diagnostica_tsv(silva.diagnostica_ex_congruentiis(x.congruentiae)))
" > materia/probationes/fixa/exemplaria/aurum.tsv
```

These are the reference until Task 4's full sweep. **They are a
SNAPSHOT of the Python extractor, not a specification** — record that
in the fixture's header comment, so a future reader does not mistake
them for the contract.

- [ ] **Step 6: plant, twice.**

```python
silva.planta('materia/fontes/materia_exemplaria.c',
  '        redde;',                      # the descent-suppression return
  '        /* planta */',                # descend anyway
  'materia', filtrum='exemplaria')
```
Expect the one-related-span assertion RED (it becomes two). Second
plant: drop `nota_ambiens` from the recursion — the `nota == "ibi"`
assertion must go red.

- [ ] **Step 7: format, words, commit.**

Words: `extrahere componere ambiens proiectio series`.
Gates: `materia`, `materia-shim`, `crusta`, `css`, `md`, `html`,
`oratio`.

---

## Task 2: `materia_pictor` — the composer

**Files:**
- Create: `materia/fontes/materia_pictor.h`, `.c`
- Create: `materia/probationes/probatio_materia_pictor.c`
- Modify: `tools/diagnostica.c` (printer body moves out; `main` keeps stdout)

**Produces:** `materia_pictor_scribere`, consumed by Task 4.

- [ ] **Step 1: the header.**

```c
/* Diagnosticum -> textus humanus (caput, causa, excerptum '^~~~').
 * CHORDAM REDDIT, numquam scribit: I/O instrumenti est, non
 * substrati. Formam ipsam lib/excerptum dat - hoc eam COMPONIT, non
 * refingit (excerptum.h: 'a quolibet fonte vocari potest'). */
chorda
materia_pictor_scribere (
                     Piscina* piscina,
constans MateriaDiagnosticum* d,
          constans character* via,
          constans character* grammatica,
          constans character* fons,
                         i32  mensura,
                         b32  excerptum);
```

- [ ] **Step 2: probatio asserting BYTE-IDENTICAL output to today.**

Capture today's rendering first, then assert the composer reproduces
it exactly:

```bash
printf '#!/bin/bash\n{ echo a\n' > /tmp/p.sh
./tools/diagnostica.sh /tmp/p.sh > materia/probationes/fixa/pictor/aurum.txt
```

The probatio builds the same `MateriaDiagnosticum` by hand and asserts
`materia_pictor_scribere` returns those bytes. Run → FAIL (link).

- [ ] **Step 3: move the body.** Lift lines 379–431 of
`tools/diagnostica.c` (header line + excerpt assembly) into the
composer, appending the header line into the `ChordaAedificator`
instead of `imprimere`. The counters (`summa->errata++`) and the TSV
branch STAY in the tool — they are policy, not form.

- [ ] **Step 4: run both.** `./materia/compile_probationes.sh pictor`
and `./tools/diagnostica_fumus.sh` — the fumus must stay green, which
is the real proof the move changed no bytes.

- [ ] **Step 5: plant.** Drop the `excerptum` guard so the caret block
is always emitted; the golden comparison must go red.

- [ ] **Step 6: format, words, commit.** Words: `pictor scribere`.
Gates as Task 1 plus `diagnostica`.

---

## Task 3: `MateriaDiagnosticaRatio` + the shared sequence

**Files:**
- Modify: `materia/fontes/materia_diagnostica.h`, `.c`
- Modify: `materia/probationes/probatio_materia_diagnostica.c`

**Consumes:** `materia_exemplaria_extrahere`, `materia_exemplaria_componere`.
**Produces:** `materia_diagnostica_plena`, consumed by Task 4.

- [ ] **Step 1: declare.** Exactly the spec's §3 struct, plus:

```c
/* Sequentia quam cliens quisque aliter exscriberet, SEMEL scripta:
 * gradus I derivatus, gradus II per regulam quamque extractus,
 * annotationes collectae, excusationes applicatae, ordo.
 *
 * 'ratio' A CLIENTE DATUR (E1): materia eam NON TENET, non enumerat,
 * non disponit. Clientem addere materiam NON emendat - haec est
 * probatio quae sola refert. */
Xar*
materia_diagnostica_plena (
                          Piscina* piscina,
            constans MateriaNodus* radix,
constans MateriaDiagnosticaRatio* ratio);
```

**`materia_diagnostica.h` must NOT gain an `#include "stml.h"`** — it
would drag the STML parser into the diagnostics contract, which is why
`materia_excusatio` is a separate header at all. Put `plena` in
`materia_excusatio.h` if the include cannot be avoided, and say so in
the commit.

- [ ] **Step 2: probatio with a two-tier fixture, and make it fail.**
Hand-built tree with one registry-declared error AND one rule finding,
plus a `<tolera>` covering only the second. Assert: two findings before
exemption, one after, and **ONE annotation collection** — plant Step 5
proves the last.

- [ ] **Step 3: implement** as the documented sequence. Appending
tier-2 rows to tier-1 is a LOOP (`xar_extendere` does not exist).

- [ ] **Step 4: run, expect PASS.**

- [ ] **Step 5: plant.** Collect annotations twice (once per tier); the
"one collection" assertion goes red. Second plant: skip the sort —
the ordering assertion goes red.

- [ ] **Step 6: format, words, commit.** Words: `plena sequentia`.

---

## Task 4: the crusta facade, and the differential gate

**Files:**
- Create: `crusta/fontes/crusta_facies.h`, `.c`
- Create: `crusta/probationes/probatio_crusta_facies.c`
- Create: `crusta/instrumenta/differentia_exemplariorum.sh`

**Consumes:** `materia_diagnostica_plena`, `materia_pictor_scribere`.
**Produces:** `crusta_diagnostica_textus`, `crusta_diagnostica_omnia`.

- [ ] **Step 1: the three levels**, exactly as spec §4. `optiones ==
NIHIL` loads every `*.stml` in `crusta/lintrum/`. The existing
`crusta_diagnostica()` is UNTOUCHED — its four callers keep working.

- [ ] **Step 2: probatio.** One `.sh` fixture with a tier-1 error and
an unannotated `-nt`; assert `crusta_diagnostica_omnia` returns both,
and that `crusta_diagnostica_textus` renders them. Then add a
`<tolera>` and assert the `-nt` drops while the tier-1 error stays —
**the asymmetry is the point; a fixture where both are exempted cannot
discriminate.**

- [ ] **Step 3: implement**, ~20 lines per spec §4.

- [ ] **Step 4: THE GOVERNING GATE — the differential.**

`crusta/instrumenta/differentia_exemplariorum.sh`: for all 232 `.sh`
outside `probationes/fixa/`, run BOTH extractors to the 12-field TSV
and diff.

```bash
python3 -c "…diagnostica_tsv(diagnostica_ex_congruentiis(
    congruentiae_minus(exemplaria(viae, REGULA).congruentiae,
                       'nt-omnes','nt-negata','nt-aequalitas')))" > py.tsv
./crusta/facies.sh -machina -sine-excusatione <viae>              > c.tsv
diff py.tsv c.tsv
```

**Exit 2 if either side is empty** — two empty files diff clean, and
that is how this house last shipped a gate that measured nothing.
Pin the ROW COUNT as well as the diff.

- [ ] **Step 5: run it. Zero divergences over 232 files, or the
extractor is wrong.** This is the same method the sedes arc used over
5,501,210 elements: two implementations, one corpus.

- [ ] **Step 6: plant the differential.** Reintroduce descent into
captured subtrees in the C extractor; the diff must go red with a
named file. A differential that cannot go red is worse than none.

- [ ] **Step 7: format, words, commit.** Words: `facies differentia`.
Gates: all of Task 1 plus `diagnostica`, `pythonica`.

---

## Task 5: dispatcher, and tier 2 at the commit

**Files:**
- Modify: `tools/diagnostica.c` (`main` → suffix dispatch), `tools/diagnostica.sh`
- Modify: `tools/diagnostica_fumus.sh` (gates XV–XVII)
- Modify: `tools/unci-git/pre-commit`, `tools/unci-git/fumus.sh`

- [ ] **Step 1: dispatch.** `main` routes `.sh` → `crusta_diagnostica_*`,
`.css` → css's derive path (css has no facade yet — E7's fallback).
`-machina`, `-lege`, `-excusa`, `-sine-excerpto` all keep working.

- [ ] **Step 2: fumus XV–XVII**, each born red:

| # | asserts | plant |
|---|---|---|
| XV | `.sh` with an unannotated `-nt` → exit 1 | drop tier 2 from the dispatch |
| XVI | same file annotated → exit 0 | ignore exemptions in the facade |
| XVII | round trip still byte-identical | — regression guard |

- [ ] **Step 3: pre-commit.** The existing `diagnostica` section now
covers both tiers automatically — **verify this rather than assume it**,
by staging an unannotated `-nt` and confirming OBSTAT.

- [ ] **Step 4: `tools/unci-git/fumus.sh` gate XV**: staged `.sh` with
a bare `-nt` blocks. Plant: point the dispatch at tier 1 only.

- [ ] **Step 5: commit.** Gates: `unci`, `diagnostica`, `crusta`,
`radix`.

---

## Task 6: Python deletions, and closing

**Files:**
- Modify: `pythonica/silva.py`, `pythonica/probatio_silva.py`

- [ ] **Step 1: delete**, only now that Task 4's differential is green:
`_relata` (109), `_vertere` (18), `_sedem_ordinis` (14),
`_sedem_relatam` (22), `_notam_mundare` (9), `diagnostica_tsv` (23),
`diagnostica_ex_congruentiis` (20), `congruentiae_minus` (34),
`diagnostica_lintris` (31), `diagnostica_pingere` (24).

- [ ] **Step 2: `exemplaria` becomes a shell** over
`./crusta/facies.sh -machina`, keeping the glob and the parallel map.
Its return shape is UNCHANGED — `probatio_silva.py` pins it, and that
pin is the gate on this step.

- [ ] **Step 3: run `python3 pythonica/probatio_silva.py`.** The
rule-1 pins (291/277/14) must still hold — they now measure the C
path, which is the strongest possible check that the migration
preserved behaviour.

- [ ] **Step 4: `stml vertere` is now unexercised in-house.** Do NOT
delete it: `01M2RYR3JJ` says the seam REVERSES rather than vanishing,
and `vertere` is the CLI verb for foreign consumers. Record it as
deferred with a named trigger.

- [ ] **Step 5: commit**, gates `pythonica`, `radix`, `diagnostica`.

- [ ] **Step 6: closing.** Spec §"As built" with every divergence;
`01M2RYR3JJ` → impletum with the final line counts; `01M2S9GR5W`
noted as overtaken; memory `diagnostica-next-arc.md`; and
**AUDIENDUM 4 — dead `lint:` exemptions are now judgeable**, because
`ratio->regulae` IS the full rule set. Either build that check here or
file it with this arc named as its trigger.

---

## Self-review

**Spec coverage.** E1 → Task 3 Step 1. E2 → Task 1. E3 → Task 4.
E4 → Task 2 (+ the placement decision at File structure). E5 → Task 3
Step 2. E6 → Task 6 Step 4. E7 → Task 5. E8 → Task 1 Step 2 (the
probatio is written against the vocabulary, with rule 1 appearing only
as golden data in Step 5).

**Known gap, named rather than hidden:** css gets no facade (Task 5
Step 1 keeps its derive path). Spec §7 already says css writes its own
when it has rules; the dispatcher's fallback is the seam that makes
that possible later.

**Type consistency.** `materia_exemplaria_extrahere` /
`_componere` (Task 1) are the names Task 3 consumes;
`materia_diagnostica_plena` (Task 3) is what Task 4 consumes;
`materia_pictor_scribere` (Task 2) is what Task 4 renders with. The
spec's §2 called the entry point `materia_exemplaria_applicare` — this
plan splits it into `_componere` + `_extrahere` because assembly and
extraction have separate gates, and a single entry point would hide
which half a divergence came from.
