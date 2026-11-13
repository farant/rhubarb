# negotia paginae Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. House rule: executed INLINE, one task per turn; Fran approves each task's header text before code.

**Goal:** Pages of a project in the Cmd+H/L ring: mock-ups Claude writes (drawn by vicus's own drawing code) and pictor mood boards Fran makes, each with an id, notes, revisions, newest-active first.

**Architecture:** Page records join the negotia acts (`lib/negotia.c`). A new library `lib/adumbratio.c` turns a mock-up document (STML + macros) into a display list (`Mandata`) with the house layout and figura pieces, plus a raw `<mandata>` escape; the same list is drawn in the negotia pane and written to PNG by `bin/negotia`. Pictor pages are ordinary pictor documents named `negotia/<id>`, hidden from pictor's shared lists. The host builds the ring as built-ins, then pages by last activity (page act, notes, pictor strokes).

**Tech Stack:** C89 house libraries: volumen, stml + stml_macros, dispositio, figura, mandatum, delineare_mandata, imago_png, pictor_documentum, scriba_liber, the vicus ring (vicus.h).

**Spec:** `project-specs/negotia-paginae.md` (decisions II.1-10, builder's calls II-a.1-3). Builds on `project-specs/negotia.md` / `negotia-plan.md` (T1-T6 done).

## Global Constraints

- C89, house flags (-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings); 72 columns; latina.h macros never identifiers; chorda not NUL-terminated; i32/i64 UNSIGNED.
- Latin identifiers, `VOCABULA_VIAE_ADDITAE="..." ./oratio/vocabula.sh -nova` NOVA 0; UI strings English.
- Per file: `./silva/examen.sh` ACCIPE; `./silva/formator.sh -scribere` on the in-tree path.
- Gates born red by a COMPILING plant (`silva.planta`, porta `radix` + filtrum), backups in `build/plantae_*`, `cmp` after.
- Records in the volume vicus is given (live `~/.rhubarb/paginae-proba.volumen`); full-snapshot acts; an identical edit writes nothing.
- Page id shown and spoken = last 6 characters of the ULID; full id or unique suffix accepted everywhere (spec IV).
- Mock-up repetition = STML macros (`stml_expandere`), no new template language (decision 10).
- Pictor pages: vicus only, spatium `negotia/<page id>`, never in pictor's shared lists (decisions 6, 9).
- Ring order: built-ins (features, conversations), then pages by last activity = max(page act, notes book act, pictor act) (decision 4, II-a.1).
- Name proposal: the mock-up kind is `adumbratio` ("sketch, outline"; the spec's working name was "designum") - Fran confirms at T2's header.

## Review Focus

1. **A malformed mock-up** (bad STML, unknown fragment, missing argument, unknown element) draws an error page naming the problem and line, never a blank pane or a crash -> T2 "vitia", T3 "adumbratio mala".
2. **A mock-up bigger than the pane** is clipped to the pane (drawn at its declared size from the top-left), never drawn outside it -> T3 "maior latere".
3. **A page added or revised while the project is open** reaches the ring within about a second, without reopening the project -> T6 "renovatio anuli".
4. **A pictor page whose document is missing** (record exists, document not yet created) opens as a new empty picture, not a refused or blank pane -> T4 "documentum absens".
5. **An unknown or ambiguous page id in the CLI** is refused, ambiguous suffix lists candidates (as features) -> T1 smoke.

---

### Task 1: page records + CLI

**Files:**
- Modify: `include/negotia.h`, `lib/negotia.c`, `tools/negotia.c`, `lib/negotia.worklog.md`
- Test: `probationes/probatio_negotia.c`, scratchpad smoke `negotia_paginae_fumus.sh`

**Interfaces:**
- Produces (`include/negotia.h`):

```c
nomen enumeratio {
    NEGOTIA_PAGINA_PICTOR = 0,   /* "pictor" */
    NEGOTIA_PAGINA_ADUMBRATIO    /* "adumbratio" (mock-up) */
} NegotiaPaginaGenus;

nomen structura {
    chorda id;  chorda negotium;  s32 genus;
    chorda titulus;  chorda corpus;   /* adumbratio: documentum */
    s64 seq;  chorda momentum;
} NegotiiPagina;

chorda negotia_paginam_creare (Negotia*, chorda negotium, s32 genus,
    chorda titulus, chorda corpus);              /* vacua: refutatum */
b32 negotia_paginam_mutare (Negotia*, chorda id, chorda titulus,
    chorda corpus);                               /* genus fixum */
constans NegotiiPagina* negotia_paginam_invenire (constans Negotia*,
    chorda id, b32* ambiguum_out);
Xar* negotia_paginae (constans Negotia*, chorda negotium, Piscina*);
                                    /* copiae, ordine creationis */
Xar* negotia_paginae_revisiones (constans Negotia*, chorda id,
    Piscina*);                      /* vetustissima prima */
constans character* negotia_pagina_genus_titulus (s32 genus);
b32 negotia_pagina_genus_legere (chorda verbum, s32* genus_out);
```

Act genus `"negotii-pagina"` `{id, negotium, genus, titulus, corpus}`
(genus token `pictor` | `adumbratio`). A page act counts as project
activity (`seq_activitatis`). CLI (same house flags/exit codes as T2 of
negotia-plan):

```
negotia pagina <negotium> <titulus> -genus pictor|adumbratio
               [-corpus-filum F]                    -> id
negotia pagina-mutare <pagina> [-titulus T] [-corpus-filum F]
negotia paginae <negotium>     ("<suffix>  <genus>  <titulus>")
```

- [ ] **Step 1 (red):** sections "paginae" (create both kinds, read back,
  revise -> 2 revisions, identical revise writes nothing, genus unchanged
  by mutare, refusals: empty title, unknown project, bad genus),
  "pagina alius scriptor" (second handle sees a new page after
  `negotia_renovare`); smoke: create, list, revise, ambiguous suffix
  (exit 1 + candidates), bad genus (exit 2).
- [ ] **Step 2:** implement (pattern of `negotii-facultas`); CLI verbs.
- [ ] **Step 3:** green; plants: identical revise appends (-> "paginae");
  genus changed by mutare (-> "paginae").
- [ ] **Step 4:** gates, commit on Fran's word.

### Task 2: the mock-up format (adumbratio) and PNG

**Files:**
- Create: `include/adumbratio.h`, `lib/adumbratio.c`, `lib/adumbratio.worklog.md`
- Modify: `tools/negotia.c` (verb `png`)
- Test: `probationes/probatio_adumbratio.c`
- Regenerate: `./tools/compile_tests_fontes_generare.sh`

**Interfaces:**
- Produces:

```c
nomen structura {
    Mandata* mandata;     /* semper (pagina vitii si vitium) */
         s32 latitudo;    /* superficies declarata (pixela) */
         s32 altitudo;
      chorda vitium;      /* vacua = bene; aliter "linea N: ..." */
} AdumbratioResultus;

AdumbratioResultus adumbrationem_reddere (chorda documentum,
    s32 latitudo_ordinaria, s32 altitudo_ordinaria, Piscina*,
    InternamentumChorda*);
b32 adumbrationem_png_scribere (constans AdumbratioResultus*,
    constans character* via, Piscina*);
```

Pipeline: `stml_legere` -> `stml_expandere` (error -> its line and
fragment/argument names) -> walk the tree -> dispositio in 6x8 cells ->
figura calls into `Mandata`. Elements (exact names in the header): root
`<adumbratio latitudo altitudo>` (pixels; absent = defaults), boxes
`<columna>` / `<linea>` (attrs `latitudo`/`altitudo` = cells or
`crescens`, `spatium` padding), leaves `<textus fons="titulus|ordinarius"
color="text|dim|accentus|...">`, `<globulum status="shipped|partial|
planned">` (or `textus=`), `<ordo electus="verum" super="verum">`
(container drawn as selected / hover row), `<sagitta directio>`,
`<fascia>`, `<campus>`, `<mensa>`, `<spatium/>`, and `<mandata>` whose
children are the six primitives in `mandata_legere_stml`'s form,
translated into the box. Colours by theme slot name. Error page: the
pane background, "Mock-up error" title, the message. CLI:
`negotia png <pagina> <via.png>` (adumbratio pages here; pictor in T4).

- [ ] **Step 1 (red):** "textus" (a text leaf drawn at its cell),
  "fragmenta" (a row fragment called 3 times -> 3 rows' texts in order),
  "globulus" (status -> chip text), "mandata" (raw `<rectangulum>` drawn
  translated into its box), "vitia" (unknown fragment, missing argument,
  unknown element, malformed STML -> `vitium` names it and its line;
  `mandata` holds the error page), "png" (file written, decoded size =
  declared size).
- [ ] **Step 2:** implement; `png` verb (exit 1 on error with the message).
- [ ] **Step 3:** green; plants: macros not expanded (-> "fragmenta");
  `<mandata>` not translated (-> "mandata"); error swallowed to an empty
  list (-> "vitia").
- [ ] **Step 4:** gates; look check (render the T3-style negotia list as a
  mock-up, Read the PNG); commit on Fran's word.

### Task 3: mock-up view in the negotia pane

**Files:**
- Modify: `apps/negotia/organum.stml` (`visus` option `adumbratio`,
  field `pagina` textus, semita status), `lib/negotia_visus.c`,
  `include/negotia_visus.h` (comment)
- Test: `probationes/probatio_negotia_visus.c`

**Interfaces:**
- Consumes: T1 `negotia_paginam_invenire`, T2 `adumbrationem_reddere`.
- Produces: pane argument `adumbratio/<id>` (as `colloquium/<sessio>`);
  locus `negotia/pagina/<full id>`; locus title "<project> · <page
  title>". Title bar shows the title and the 6-char id; the mock-up is
  drawn below at its declared size from the top-left, clipped to the
  pane (coetus with sectio); re-rendered when `negotia_versio` moves.

- [ ] **Step 1 (red):** "adumbratio" (argument -> visus adumbratio, text
  of the mock-up drawn, id drawn, locus/locus title), "adumbratio mala"
  (broken document -> "Mock-up error" + message drawn), "maior latere"
  (1200x1200 mock-up -> no drawn element outside the pane bounds),
  "adumbratio revisa" (second handle revises -> pulse -> new text).
- [ ] **Step 2:** implement. **Step 3:** green; plants: no clipping
  (-> "maior latere"); revision not picked up (-> "adumbratio revisa").
- [ ] **Step 4:** gates; screenshot; commit on Fran's word.

### Task 4: pictor pages

**Files:**
- Modify: `include/pictor_documentum.h`, `lib/pictor_documentum.c`
  (`spatium_documenti` skips `negotia/` beside `aliena/` :1166; new
  exists check + last act), `lib/pictor_applicatio.c`
  (`pictor_picturam_ponere` :333 uses the exists check, not
  `in_bibliotheca`, for `negotia/` spatia; missing -> create empty),
  `tools/negotia.c` (`pagina -genus pictor` creates the document; `png`
  for pictor pages), `lib/vicus_applicatio.c` (`$add-pictor` iussum;
  ring gains pictor pages)
- Test: `probationes/probatio_pictor_documentum.c` (or the pictor suite
  holding library tests), `probationes/probatio_negotia.c` smoke via CLI,
  `probationes/probatio_vicus_acervus.c` (`$add-pictor`, ring)

**Interfaces:**
- Produces:

```c
/* documentum spatii in volumine exstat (plagula 'documentum'), in
 * bibliotheca aut non */
b32 pictor_documentum_exstat (Volumen*, constans character* spatium,
    Piscina*);
/* seq actus ultimi picturae (ictus, ramus, cursor, coniunctio;
 * historia_genera); 0 si nulla */
s64 pictor_documentum_ultimum (Volumen*, constans character* spatium,
    Piscina*);
```

Pictor page = ring entry ("pictor", "negotia/<full id>"). Default size
for a new page document: the pictor pane's default canvas. For a
`negotia/` spatium the pictor pane (a) shows the page's 6-char id in its
title and (b) reports companion locus `negotia/pagina/<full id>`
(facies.locus), so Cmd+T opens the page's notebook, not pictor's.

**`$add-pictor` (Fran 2026-11-06):** how Fran makes a mood board in vicus.
Typed on any scriba page; `$add-pictor` or `$add-pictor(title)` (default
title "mood board"). The project = the ACTIVE TAB's ring when its first
entry is ("negotia", id) - the tab's project, not strictly the right
pane's. Creates the pictor page + empty document, rebuilds the ring, and
opens the page in front. Consumed on run (like `$dies`), so a second
click cannot make a second page. No project ring -> nothing created,
error "add-pictor: no project in this tab".

- [ ] **Step 1 (red):** "negotia occulta" (a `negotia/x` document is not
  in `pictor_documenta_enumerare` / `_recentia`, but `_exstat` is VERUM),
  "ultimum" (strokes raise `_ultimum`), "documentum absens" (pane opened
  on `negotia/<id>` with no document -> an empty picture, pane mounted),
  "aperire occultam" (pane opens an existing hidden document), "locus
  paginae" (pane on `negotia/<id>`: locus = `negotia/pagina/<id>`, id
  drawn in the title); CLI
  smoke: `pagina -genus pictor` then `png` writes the bitmap;
  "add-pictor" (project tab: page created, in the ring, in front;
  title argument used), "add-pictor sine negotio" (no project ring:
  error, nothing created).
- [ ] **Step 2:** implement. **Step 3:** green; plants: prefix not
  skipped (-> "negotia occulta"); exists check reverted to library
  (-> "aperire occultam").
- [ ] **Step 4:** gates (whole pictor suites); commit on Fran's word.

### Task 5: notes by id + activity

**Files:**
- Modify: `include/scriba_liber.h`, `lib/scriba_liber.c` (last act of a
  book), `tools/negotia.c` (`notae`). Page activity combines pictor and
  scriba sources, so it is computed in the host (T6), not in
  `lib/negotia.c`.
- Test: `probationes/probatio_scriba_liber.c` (or its suite), smoke

**Interfaces:**
- Produces:

```c
/* seq actus ultimi libri in spatio (paginae omnes; historia_genera
 * generis documenti scribae); 0 si liber nullus */
s64 scriba_liber_ultimum (Volumen*, constans character* praefixum,
    Piscina*);
```

and `negotia notae <pagina>`: the companion book
`comes/negotia/pagina/<full id>` read with `scriba_liber_lineas_legere`,
printed with the page's revision times interleaved as
`-- revision 2026-11-05 10:12 --` lines (by act order of notes pages vs
revisions; v1 granularity: per notes page, II-a.2). No book: "no notes".

- [ ] **Step 1 (red):** "ultimum" (book written twice -> seq grows;
  absent -> 0); smoke: notes written through `scriba_liber` then
  `negotia notae` prints them and the revision markers; "no notes".
- [ ] **Step 2:** implement. **Step 3:** green; plant: `_ultimum` reads
  only the first page (-> "ultimum").
- [ ] **Step 4:** gates; commit on Fran's word.

### Task 6: pages in the ring

**Files:**
- Modify: `lib/vicus_applicatio.c` (`negotium_aperire_in_vico` ring:
  built-ins + pages; page activity; refresh),
  `lib/negotia_visus.worklog.md`
- Test: `probationes/probatio_vicus_acervus.c`

**Interfaces:**
- Consumes: T1 `negotia_paginae`, T4 `pictor_documentum_ultimum`, T5
  `scriba_liber_ultimum`, negotia T5 `vicus_anulum_ponere`.
- Produces: ring [("negotia", id), ("colloquium", s)..., then pages
  sorted by activity desc: adumbratio -> ("negotia", "adumbratio/<id>"),
  pictor -> ("pictor", "negotia/<id>")]. The host keeps which project
  each tab's ring belongs to; when `negotia_versio` moves (pulse) or a
  page's notes/pictor activity changes order, the ring of that tab is
  re-set, keeping the current front (position = its new index).

- [ ] **Step 1 (red):** "anulus paginarum" (project with two mock-ups and
  a pictor page: ring order = features, conversation, newest-active
  page first), "notae movent" (notes on the older page -> it moves
  first), "renovatio anuli" (page added by a second handle -> after a
  pulse the ring has it, front pane unchanged).
- [ ] **Step 2:** implement. **Step 3:** green; plants: pages in creation
  order (-> "anulus paginarum"); notes ignored (-> "notae movent").
- [ ] **Step 4:** all vicus suites; live check with Fran (a mock-up and a
  pictor page in negotia, Cmd+L reaches the newest); commit on Fran's
  word; `negotia mutare`/new features; phase-log entry.

## Notes

- T2 is the format (Steenberg: the API is the expensive part): its header
  approval fixes element and attribute names.
- The pictor pane change (T4) touches pictor itself: run the pictor and
  pinacotheca suites, not only negotia's.
- One ledger opus per task under a new park.
