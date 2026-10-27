# lector L3 — the reader widget: library, contents, page view Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** a new vicus widget kind `lector`: a library of the EPUBs in
`~/.rhubarb/codices/`, a contents page per book, and a page view that
flows each chapter into stable pages (chapters on fresh pages, headings
in the title font), with routes for every place and ONE reading position
per book that the last move from any pane sets.

**Architecture:** three units. `lector_paginae` (pure): a `CodexCaput`
laid out into pages for a pane size. `lector_bibliotheca` (shared by all
panes, opened once by vicus like the saints corpus): the folder scan,
one record per `dc:identifier`, opened `Codex` objects, reading
positions as volume plagulae, progress. `lector` (the widget, modelled
on `sancti_visus`): montatio + compose + one action function + key
table, state on the insula branch, manifest `apps/lector/organum.stml`,
registered as genus "lector" in `vicus_applicatio`. No `$epubs` yet
(L6); in L3 the widget opens with `$via lector/bibliotheca`.

**Tech Stack:** C89 in Latin; `codex` (L2 ec9652b0), `sarcina`, insula +
organum manifest + `semita`, `membrum__album` (paged lists),
`membrum__praecepta` (key table), `figura`/`mandata`, `fons`
(FONS_TITULI 8x16, FONS_PARVUS 6x8), `volumen` plagulae,
`iter_directoria`, `filum`, credo v2.

**Spec:** `project-specs/lector-spec.md` §II goals 2-3, 7-8, §V
(decided), §VI calls 2, 3, 5, 6, 10-12, §VIII L3. Precedents read for
this plan (2026-10-24): `sancti_visus` (widget shape, album pages,
key table, routes, `_paginae_sancti`), `kalendarium` (FONS_TITULI
headings, `semitam_secuta`, `locus`), `scriba_actiones` (per-book
"last viewed page" plagula = last change wins), semitae spec §III-VI
(persistent route = restart; transient fields are not restored). Plan and header text approved by Fran
2026-10-24 ("yes, that looks great").

## Global Constraints

- C89 + latina.h: no Roman-numeral identifiers, `i32` unsigned, `s32`
  signed, `chorda` not NUL-terminated; new lint words real Latin.
- Files only through `filum_*` / `directorium_iterator_*`, environment
  only through `lectiones_ambitus` (examen code 96 / lectiones lint).
- Fonts are monospaced bitmaps: width = codepoints × `fons_latitudo`
  (6 for FONS_PARVUS, 8 for FONS_TITULI), height 8 / 16; count
  codepoints with `utf8_decodere`, never bytes (curly quotes, em
  dashes are multibyte and draw in one cell).
- Pane state lives on the insula branch, written only inside
  `mutare_ramum` with `repo->scriptor = "lector.agere"` set for the call
  (else owners refuse silently).
- Tests close every temporary volume (`volumen_claudere`) - credo names
  leaks.
- The EPUB is never written; positions live in the volume.
- Commit only through `silva.commissio`, only on Fran's word.

## Decisions taken in this plan (check these)

1. **Position on a move = the first sentence that STARTS on the shown
   page**; a page with no sentence start (one huge sentence) stores the
   sentence that contains the page's first byte (reopening then shows
   the page where it starts - the one before; accepted).
2. **Pane route = view + book (persistent); chapter and page are
   transient route fields** (`?caput=4&pagina=2`). Restart restores only
   persistent fields, so a resumed pane re-reads the BOOK's position
   (last change wins even across a restart); an explicit route that
   names chapter/page is a move and writes the position.
3. **Same `dc:identifier` in two files = one book** (spec: a
   re-downloaded file keeps its state): one library row, the first file
   by file name. Unreadable `.epub` files get a dim "unreadable" row with
   their error; other files are ignored.
4. **Position storage:** plagula `lector/positio/<identity %XX>`, STML
   `<positio caput="4" initium="1234">In the first place,</positio>`
   written with the stml writer; fingerprint = the sentence's first ≤32
   bytes, cut on a UTF-8 boundary. (RULING in Task 2: the fingerprint is
   element TEXT, not an attribute - STML stores attribute values raw, so
   a `"` in the sentence broke the plagula; the writer escapes text.)
5. **Progress** = bytes of all chapter texts before the position / all
   chapter text bytes, as a whole percent; chapter lengths are measured
   once per book per process (parse all chapters, ~0.2-0.35 s for
   Cranford) and cached in the bibliotheca. "new" when there is no
   position.
6. **Page layout** (sizes in logical px of the text area): heading
   lines in FONS_TITULI (16 px), other lines FONS_PARVUS (8 px); one
   empty 8 px line between blocks (none at the top of a page); verse
   lines wrap with a 2-cell hanging indent; words break at ASCII spaces,
   a word longer than the line is cut at a codepoint boundary; a page
   always holds at least one line.
7. **No live following**: a pane already showing a book does not jump
   when another pane moves; it follows on its next open (spec §V: a
   pane OPENING a book shows the book's position). A pulse can add
   live following later.

## Header text (approved with the plan)

`include/lector_paginae.h`:

```c
/* lector_paginae.h - caput codicis (codex.h) in paginas (lector L3;
 * spec project-specs/lector-spec.md VI.2, VI.10). Paragraphi toti
 * fluunt: verba ad spatia ASCII franguntur, verbum linea longius ad
 * runam secatur. Titulus in FONS_TITULI (8x16), cetera FONS_PARVUS
 * (6x8); versus linea per lineam, continuatio II cellulis indentata;
 * inter clausulas linea vacua VIII px (non in summa pagina). Functio
 * pura magnitudinis: eadem area -> eaedem paginae. Pagina saltem
 * lineam unam habet; caput sine textu = pagina una vacua. */

nomen structura {
    i32 initium;   /* octeti in textu capitis */
    i32 finis;     /* exclusivus, spatia finalia dempta */
    i32 fons;      /* FONS_TITULI aut FONS_PARVUS */
    s32 x;         /* px in area textus */
    s32 y;
} LectorLinea;

nomen structura {
    Xar* lineae;    /* LectorLinea */
     i32 initium;   /* octetus primus (lineae primae) */
} LectorPagina;

/* caput in paginas areae latitudo x altitudo (px); Xar de
 * LectorPagina, saltem una */
Xar*
lector_paginare (
    constans CodexCaput* caput,
                    i32  latitudo,
                    i32  altitudo,
               Piscina* piscina);

/* index paginae ubi octetus positio iacet (initium paginae <= positio
 * < initium sequentis); ultra finem -> ultima */
i32
lector_pagina_positionis (
    Xar* paginae,
    i32  positio);
```

`include/lector_bibliotheca.h`:

```c
/* lector_bibliotheca.h - bibliotheca codicum EPUB (lector L3):
 * directorium ~/.rhubarb/codices/ (vicus: RHUBARB_CODICES vincit), liber
 * unus per dc:identifier (fila eiusdem identitatis: primum nomine fili),
 * fila .epub non legibilia nominata (vitium), cetera ignorata. Positio
 * legendi per librum in volumine (plagula "lector/positio/<identitas
 * %XX>"; ultima mutatio vincit, ex quovis latere). Communis omnibus
 * lateribus: vicus semel aperit. */

nomen structura LectorBibliotheca LectorBibliotheca;   /* opaca */

nomen structura {
    chorda via;         /* filum */
    chorda identitas;   /* vacua si non legitur */
    chorda titulus;     /* dc:title; nomen fili si vacuum */
    chorda auctor;
    chorda vitium;      /* non vacuum = non legitur */
} LectorLiber;

nomen structura {
       i32 caput;
       i32 initium;     /* sententiae initium, octeti in textu capitis */
    chorda vestigium;   /* octeti primi sententiae (XXXII summum) */
} LectorPositio;

/* directorium legere; volumen NIHIL licet (positiones non servantur) */
LectorBibliotheca*
lector_bibliotheca_aperire (
     constans character* via,
                Volumen* volumen,
                Piscina* piscina,
    InternamentumChorda* intern);

i32
lector_bibliotheca_numerus (
    constans LectorBibliotheca* b);

/* liber k: legibiles ordine tituli (sine casu litterarum), deinde non
 * legibiles ordine fili; NIHIL extra ambitum */
constans LectorLiber*
lector_bibliotheca_liber (
    constans LectorBibliotheca* b,
                           i32  k);

/* per identitatem; NIHIL si abest */
constans LectorLiber*
lector_bibliotheca_invenire (
    constans LectorBibliotheca* b,
                        chorda  identitas);

/* codex libri, semel apertus et servatus; NIHIL et *vitium si non */
Codex*
lector_bibliotheca_codex (
          LectorBibliotheca* b,
    constans LectorLiber* liber,
                    chorda* vitium);

/* positio servata libri; FALSUM si nulla (aut sine volumine) */
b32
lector_positionem_legere (
    LectorBibliotheca* b,
               chorda  identitas,
        LectorPositio* positio,
             Piscina* piscina);

/* positionem servare (plagula; ultima vincit) */
b32
lector_positionem_scribere (
         LectorBibliotheca* b,
                    chorda  identitas,
    constans LectorPositio* positio);

/* progressus 0..100 (octeti capitum ante positionem / omnes); -1 si
 * nulla positio aut liber non legitur. Longitudines capitum semel
 * mensae. */
s32
lector_progressus (
          LectorBibliotheca* b,
    constans LectorLiber* liber);
```

`include/lector.h`:

```c
/* lector.h - LECTOR in latere vici (genus "lector"; lector L3, spec
 * project-specs/lector-spec.md). Manifestum apps/lector/organum.stml;
 * bibliotheca (lector_bibliotheca.h) per ligamen a parente, communis
 * omnibus lateribus; status visus in ramo.
 *
 * BIBLIOTHECA: linea per librum - titulus, auctor, progressus ("37%",
 * "new"); non legibiles obscuri ("unreadable"). Claves: j k (electus),
 * [ ] (paginae), Enter (liber ad positionem suam), Esc (latus per
 * vicum).
 * INDEX: capita libri (prooemium "Beginning"), caput positionis
 * notatum. Claves: j k, [ ], Enter (caput a pagina prima), Esc (ad
 * bibliothecam).
 * PAGINA: caput paginatum (lector_paginae.h), titulus capitis et "3/12"
 * in linea ima. Claves: [ ] (pagina; trans capita), t (index), Esc (ad
 * indicem). Omnis motus positionem libri scribit (sententia prima quae
 * in pagina incipit; nulla -> sententia continens). Liber apertus =
 * positio eius (codex_reperire); amissa -> initium capitis cum nota
 * "position not found". */

nomen structura {
            InsulaRamus  ramus;
         ActioRegistrum* actiones;
        FiguraRegistrum* figurae;
      LectorBibliotheca* bibliotheca;   /* ligamen (a parente) */
                Piscina* piscina;
    InternamentumChorda* intern;
                 vacuum* figurae_ctx;   /* privatum: codex, caput,
                                         * paginae in cache */
} LectorMontatio;

/* montare in ramo <lector id="..."> (manifestum infixum); superficies
 * latitudo x altitudo (0 = ordinaria); systemata NIHIL licet */
b32
lector_montare (
         LectorMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
      LectorBibliotheca* bibliotheca,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata);

/* compositor (dispensator; facies vici) - ctx = LectorMontatio* */
Componens*
lector_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* argumentum lateris: URI semitae ("lector/bibliotheca",
 * "lector/pagina/<id>?caput=4&pagina=2"), identitas libri nuda (->
 * liber ad positionem suam), vacuum (bibliotheca). Semita mala aut
 * liber ignotus: FALSUM, nihil mutatum. */
b32
lector_argumentum_ponere (
         LectorMontatio* m,
     constans character* argumentum);

/* facies.semitam_secuta (historia, $via, restitutio): caput et pagina
 * nominata = motus (positio scribitur); non nominata = positio libri */
b32
lector_semitam_secuta (
    vacuum* ctx);

/* facies.locus: semita loci currentis cum capite et pagina
 * ("lector/pagina/<id>?caput=4&pagina=2"); index, bibliotheca: semita
 * sua */
chorda
lector_locus (
     vacuum* ctx,
    Piscina* piscina);

/* facies.locus_titulus: "Cranford - CHAPTER I. OUR SOCIETY - 3/12";
 * index: titulus libri; bibliotheca: "Library" */
chorda
lector_locus_titulus (
     vacuum* ctx,
    Piscina* piscina);
```

`apps/lector/organum.stml` (fields; comments in the file):

- `visus` electio {bibliotheca, index, pagina}, persistens, absens
  bibliotheca, `semita="ligatio" historia="impelle"`.
- `liber` textus (dc:identifier), persistens, `semita="ligatio"
  historia="impelle"`.
- `caput`, `pagina` numerus, transiens, `semita="status"
  ambitus="ligatio"` (Decision 2).
- `electus_liber` textus persistens; `electus_caput` numerus transiens;
  `pagina_ordinum` numerus transiens (album page, follows the selection);
  `nota` textus transiens ("position not found").
- `<ligamina><ligamen nomen="bibliotheca" genus="lector" a="parens"/>`,
  `<actiones><actio nomen="lector.agere"/>`, cognomina
  `<cognomen semita="lector/bibliotheca"/>` and
  `<cognomen typus="textus" semita="lector/pagina/{}"/>`,
  `<basis genus="organum_commune"/>`; `apps/lector/unitas.stml` like
  sancti's. Then `./tools/organa_infigere.sh`.

## Review Focus

1. **Two panes, one book**: pane A turns pages, pane B opens the same
   book from the library -> B shows A's page; B moves, A reopens -> B's
   page. Pinned in Task 5.
2. **Restart after another pane moved on**: a pane saved on page 3 of a
   book that another pane later moved to chapter 7 resumes in chapter 7,
   not on its stale page. Pinned in Task 5 (persistent route has no
   chapter/page).
3. **Book replaced by another edition** (text shifted, or the sentence
   gone): nearby sentence found by fingerprint; gone -> chapter start +
   "position not found", never a silently wrong page. Pinned in Task 4.
4. **Tiny pane, long words, empty chapter**: no endless loop; every
   non-space byte of a chapter appears on exactly one page, in order;
   empty chapter = one empty page. Pinned in Task 1.
5. **Messy books folder** (unreadable `.epub`, duplicate identity,
   `.txt`, missing folder): dim row / one row / ignored / empty library
   with a hint; nothing crashes. Pinned in Tasks 2 and 3.

---

### Task 1: `lector_paginae` - a chapter into pages

**Files:** create `include/lector_paginae.h` (header text above),
`lib/lector_paginae.c`, `probationes/probatio_lector_paginae.c`;
regenerate `compile_tests_fontes_generata.sh`.

**Interfaces:** Consumes `CodexCaput`, `CodexClausula` (codex.h),
`fons_latitudo`/`fons_altitudo`, `utf8_decodere`. Produces
`lector_paginare`, `lector_pagina_positionis`, `LectorLinea`,
`LectorPagina`.

- [ ] **Step 1: Failing tests** (section `paginae`, chapters built in the
  test as `CodexCaput` values, no EPUB needed):
  - heading `"Caput I\nDe Cranfordia"` + paragraph, area 240x200: lines
    0-1 FONS_TITULI at y 0 and 16; the paragraph's first line at y 40
    (32 + 8 gap), FONS_PARVUS; every line's text fits `latitudo / 6`
    (or `/ 8`) codepoints.
  - a paragraph of 400 words at 240x80: page count > 1; concatenating
    every line's text (joined by spaces) reproduces the paragraph with
    its spaces collapsed; pages in order (`initium` strictly growing).
  - curly quotes / em dash: a line of 40 three-byte codepoints fits a
    240 px line (40 cells), not 13.
  - verse block `"line one long enough to wrap\nline two"` at 60 px:
    continuation lines have x = 12, first lines x = 0.
  - a 100-letter word at 60 px (10 cells): cut into 10-codepoint lines,
    never mid-codepoint (`"é"` × 100 as the word).
  - area 60x4 (smaller than a line): every page has exactly one line,
    the call returns (no hang - `CREDO_NON_PENDET`).
  - empty chapter: one page, zero lines.
  - determinism: two calls, same pages; a different area, different
    pages.
  - `lector_pagina_positionis`: offset of the 2nd page's first byte ->
    1; offset 0 -> 0; offset past the end -> last.
- [ ] **Step 2: Run** `./compile_tests.sh probatio_lector_paginae` -
  Expected: FAIL (no library).
- [ ] **Step 3: Implement** in `lib/lector_paginae.c`: walk blocks in
  order; per block choose font (TITULUS -> FONS_TITULI) and cell count;
  split verse blocks on `'\n'`; greedy wrap by codepoints breaking at
  the last ASCII space (trailing spaces excluded from `finis`), hard
  cut at codepoint boundary when no space; place lines top-down with
  the 8 px gap between blocks (not at page top); start a new page when
  the next line does not fit (always at least one line per page).
- [ ] **Step 4: Run** - PASS.
- [ ] **Step 5: Plants** (silva.planta): gap at page top (red: first
  line y != 0 on page 2), byte-counted width (red: curly quotes), no
  minimum line per page (red via `CREDO_NON_PENDET`), verse indent 0
  (red).

---

### Task 2: `lector_bibliotheca` - folder, books, positions, progress

**Files:** create `include/lector_bibliotheca.h`,
`lib/lector_bibliotheca.c`, `probationes/probatio_lector_bibliotheca.c`;
fixtures `probationes/fixa/lector/bibliotheca/` (sources under
`probationes/fixa/lector/fontes/`, EPUBs made with `zip -X0` mimetype +
`zip -Xr`): `alpha.epub` (minimus content, id `urn:rhubarb:alpha`, title
"Alpha", author "Anna Scriptrix"), `beta.epub` (vetus content, id
`urn:rhubarb:beta`, title "beta" lower case), `gamma.epub` (copy of
beta: same id), `fractum.epub` (bytes "not a zip"), `nota.txt`.

**Interfaces:** Consumes `codex_aperire`, `codex_numerus_capitum`,
`codex_caput_legere`, `volumen_plagulam_condere/_promere`,
`directorium_iterator_*`, `filum_legere_totum`, stml writer/reader.
Produces the `lector_bibliotheca.h` API above.

- [ ] **Step 1: Failing tests** (sections `bibliotheca`, `positio`):
  - open the fixture folder (no volume): numerus III; liber 0 "Alpha",
    1 "beta" (case-insensitive order), 2 = `fractum.epub` with vitium
    non-empty and titulus "fractum.epub"; `invenire("urn:rhubarb:beta")`
    -> via ends in `beta.epub` (not gamma); `invenire("x")` NIHIL.
  - missing folder: numerus 0, no crash.
  - `lector_bibliotheca_codex` twice -> the same pointer; on the
    unreadable book -> NIHIL + vitium.
  - positio (temporary volume): legere on a fresh book -> FALSUM;
    scribere {caput II, initium XL, "Secundum"} then legere -> same three
    values; scribere again -> the second wins; an id with ':' and '/'
    (`http://www.gutenberg.org/394`) round-trips.
  - a second `LectorBibliotheca` over the SAME volume reads the position
    the first wrote (shared across panes = across opens).
  - progressus: no position -> -1; position at chapter 0 offset 0 -> 0;
    at the start of the last chapter -> (bytes before it * 100 / all);
    unreadable book -> -1.
  - fingerprint over 32 bytes is cut on a UTF-8 boundary (write a
    vestigium of 31 ASCII + "é": stored ≤ 32 bytes, valid UTF-8).
- [ ] **Step 2: Run** - FAIL (no library).
- [ ] **Step 3: Implement**: scan with `directorium_iterator_*`, names
  ending in `.epub` (case-insensitive); read with `filum_legere_totum`,
  `codex_aperire` for metadata (keep the Codex: it is opened anyway);
  sort; drop later duplicates by identity; plagula via = `lector/positio/`
  + identity with `%XX` outside `A-Za-z0-9-_.~`; content written and
  read with the stml library; chapter lengths measured lazily
  (`codex_caput_legere` per chapter into a scratch piscina) and kept.
- [ ] **Step 4: Run** - PASS.
- [ ] **Step 5: Plants**: duplicates kept (red: numerus IV), case-sensitive
  sort (red), raw identity as via (red: gutenberg id round-trip), no
  UTF-8 cut (red).

---

### Task 3: the widget and the library view, registered in vicus

**Files:** create `include/lector.h` (header text above), `lib/lector.c`,
`apps/lector/organum.stml`, `apps/lector/unitas.stml`,
`probationes/probatio_lector.c`; regenerate `lib/organa_infixa.c`
(`./tools/organa_infigere.sh`); modify `lib/vicus_applicatio.c`
(context `ContextusLectoris` with a lazily opened bibliotheca:
`lectiones_ambitus("RHUBARB_CODICES")` else `$HOME/.rhubarb/codices`;
montator wrapper; describer: componere, argumentum_ponere, ramus,
semitam_secuta, effugium_redit, locus, locus_titulus;
`vicus_genus_addere(app->vicus, "lector", ...)`).

**Interfaces:** Consumes Task 2's API. Produces `lector_montare`,
`lector_componere`, the action `lector.agere`, component ids used by
later tasks: rows `"r<k>"` (k from 1), `"pagina-prior"`,
`"pagina-proxima"`, `"index_paginae"`, `"titulus"`.

- [ ] **Step 1: Failing tests** (`probatio_lector.c`, Mundus like
  `probatio_sancti_visus.c`: repo, temporary volume, bibliotheca over
  the Task 2 fixtures, `lector_montare(..., "l1", ..., CCXL, CDLXXII)`,
  dispensator; helpers `clavis`, `clavis_nominata`, `campus_est`,
  `reddere`, `pictum`, `ictus_in`):
  - fresh pane: `visus` = bibliotheca; painted "Library", "Alpha", "Anna
    Scriptrix", "new", "beta", "unreadable".
  - `j` moves the selection to beta (`electus_liber` =
    `urn:rhubarb:beta`); `k` back; clicking row "r2" selects beta.
  - page arrows without 30 books: a second mount whose altitudo is the
    smallest that holds title + one row + page bar (computed in the test
    from the layout constants): `]` -> page 2 shows "beta", `[` back.
  - Enter on "fractum.epub": nothing opens; its error is painted.
  - empty folder bibliotheca: painted "No books in ~/.rhubarb/codices".
  - vicus-level (`probatio_vicus_lector.c`, like
    `probatio_vicus_nomenclator.c`, `RHUBARB_CODICES` pointed at the
    fixtures): `$via lector/bibliotheca` opens a lector pane showing
    "Alpha".
- [ ] **Step 2: Run** - FAIL.
- [ ] **Step 3: Implement**: manifest; mount as `sancti_visus_montare`;
  layout with `dispositio` (title row, album rows of 2 lines: title +
  author/progress, page bar); album over the books; key table
  (`membrum__praecepta`) for the library keys; Enter -> Task 4's open
  (in Task 3: sets `visus` = pagina and `liber` only).
- [ ] **Step 4: Run** - PASS. **Step 5: Plants**: Enter on unreadable
  opens it (red), selection not following page (red: `]` then Enter
  opens alpha).

---

### Task 4: contents and page view, positions written and read

**Files:** `lib/lector.c`, `probationes/probatio_lector.c` (sections
`index`, `pagina`, `positio`).

**Interfaces:** Consumes Tasks 1-3. Produces the open/move functions
used by Task 5: `interior b32 librum_aperire(LectorMontatio*, chorda
identitas)` (book -> its position or chapter 0) and `interior b32
paginam_ponere(LectorMontatio*, i32 caput, i32 pagina)` (a move: sets
fields, writes the position).

- [ ] **Step 1: Failing tests** (alpha = minimus: chapters "", Caput I,
  Caput II, Caput III; pane CCXL x CDLXXII):
  - Enter on Alpha (no position) -> `visus` pagina, `caput` 0, `pagina`
    0; painted "Prooemium."; position written {0, 0, "Prooemium."}.
  - `]` -> chapter 1 page 0 (chapter 0 has one page): painted "Caput I"
    in FONS_TITULI (a TEXTUS mandate with fons FONS_TITULI holds
    "Caput I"), footer "Caput I" and "1/1"; position = {1, offset of
    "Caput I\nDe Cranfordia" heading = 0}.
  - `[` from chapter 1 page 0 -> chapter 0's LAST page; `[` at book
    start -> nothing; `]` at the last page of the book -> nothing.
  - `t` -> `visus` index, current chapter selected; Enter on "Caput
    III" -> chapter 3 page 0; Esc -> index; Esc -> library; front matter
    painted as "Beginning".
  - reopen: Esc to library, Enter on Alpha -> chapter 3 (its position).
  - shifted text: write a position {3, initium + 3, "Primum hic."} (3
    bytes off) -> opening Alpha finds the true sentence, `nota` empty.
  - lost: write {3, 10, "Nusquam"} -> opens chapter 3 page 0 with
    `nota` "position not found" painted in the footer.
  - tiny page (pane CCXL x LXXX): a move to a page whose first sentence
    starts there stores that sentence; reopening shows the same page.
- [ ] **Step 2: Run** - FAIL. **Step 3: Implement**: per-pane cache in
  `figurae_ctx` (codex, caput number, `CodexCaput`, its sentences via
  `codex_sententiae(caput, NIHIL, 0, p)`, pages for the area; rebuilt
  when book/chapter/area change); page painting (lines at their x/y in
  their font, footer: chapter title cut to width + "k/n" or the nota);
  contents album; moves write `LectorPositio` (Decision 1) with the
  fingerprint; open = `lector_positionem_legere` -> `codex_reperire` ->
  `lector_pagina_positionis`.
- [ ] **Step 4: Run** - PASS. **Step 5: Plants**: position = page's
  first byte instead of first sentence start (red: tiny-page reopen),
  no reperire (red: shifted), lost silently ignored (red: nota),
  boundary `[` to page 0 of the previous chapter (red).

---

### Task 5: routes, locus, two panes, restart

**Files:** `lib/lector.c`, `lib/vicus_applicatio.c` (describer already
wired in Task 3), `probationes/probatio_lector.c` (section `semitae`),
`probationes/probatio_vicus_lector.c`.

**Interfaces:** Consumes Task 4's `librum_aperire`, `paginam_ponere`.
Produces `lector_argumentum_ponere`, `lector_semitam_secuta`,
`lector_locus`, `lector_locus_titulus`.

- [ ] **Step 1: Failing tests**:
  - `lector_argumentum_ponere(m, "lector/pagina/urn%3Arhubarb%3Aalpha?caput=2&pagina=0")`
    -> chapter 2, position written; bare `"urn:rhubarb:alpha"` -> the
    book at its position; `""` -> library; `"lector/pagina/nemo"` ->
    FALSUM, fields unchanged.
  - `lector_locus` on that page = `"lector/pagina/urn%3Arhubarb%3Aalpha?caput=2&pagina=0"`
    (whatever `semita_ad_uri` spells - assert it parses back with
    `semita_ex_uri` to the same fields); `lector_locus_titulus` =
    "Alpha - Caput II - 1/1"; library -> "Library".
  - persistent route (`semita_legere_persistentem`) of a page pane names
    `visus` and `liber` but not `caput`/`pagina`.
  - vicus-level, two panes (`vicus_acervo_aperire` twice with
    `lector`, ids differ): pane A to chapter 3; pane B opens Alpha from
    the library -> chapter 3; B `[` -> chapter 2's last page; A Esc,
    Esc, Enter on Alpha -> chapter 2's last page.
  - restart: a pane's saved route (`semita_servata`) applied to a fresh
    mount after another pane moved the book -> shows the newer position.
- [ ] **Step 2: Run** - FAIL. **Step 3: Implement**:
  `argumentum_ponere` (cognomen + `semita_ex_argumento`, then
  `semita_applicare`, then `lector_semitam_secuta`); `semitam_secuta`
  (page view: caput+pagina present -> `paginam_ponere`, else
  `librum_aperire`); `locus` = `semita_ad_uri(semita_legere_plenam)` of
  the current place; `locus_titulus`.
- [ ] **Step 4: Run** - PASS. **Step 5: Plants**: `caput`/`pagina`
  persistent (red: restart test), route move not writing the position
  (red: two-pane test), locus without page (red).

---

### Task 6: Cranford in vicus, docs, commit

- [ ] Manual (Fran): `apps/vicus/vicus.sh`, `$via lector/bibliotheca`
  with `~/.rhubarb/codices/cranford.epub` present; open Cranford, turn
  pages, `t`, Esc; a second pane opens Cranford at the same page.
  Claude first runs a throwaway probe: Cranford paginated at 240x464
  and 480x464 (page counts per chapter, time) - recorded in the worklog.
- [ ] Worklog `lib/lector.worklog.md` (+ `lector_paginae`,
  `lector_bibliotheca` notes inside it); spec §VIII L3 status; ledger
  notes for any rulings; lint (NOVA 0); formatter; examen.
- [ ] Commit on Fran's word via `silva.commissio` (headers, libs, tests,
  fixtures sources + EPUBs, manifests, regenerated organa and test
  lists, vicus_applicatio, worklog, spec, this plan,
  `briar/MUTATIONES.md` line for the new corpus libraries).

---

## Execution notes (2026-10-24)

Changes from the plan, each recorded where it happened:

- T1: two extra tests (a block starting a new page sits at y 0; trailing
  spaces trimmed). The "no minimum line" plant produced empty pages, not
  a hang.
- T2: beta's title is "aeneis" (so sort order shows case sensitivity);
  fingerprint stored as element text (Decision 4 ruling); `.epub` match
  is exact-case.
- T3: `pagina_ordinum` dropped (library page derives from the
  selection); the `$via` check moved to T5 (it needs argumentum_ponere);
  ten libraries added to ludus_tessera's hand-written link list.
- T4: fixture `probationes/fixa/lector/longus` (one long chapter, own
  folder) - needed for the round-trip, reperire and boundary tests.
- T5: bug fixed - a bare book id in a pane already on that book kept
  the pane's chapter and rewrote the book position (see worklog). The
  "caput persistent" plant is not plantable (generated manifest
  embedding); pinned by the persistent-route assertion. The `$via` test
  uses a route that fits one line of a half-width scriba page.
- Plants (all red): T1 5, T2 6, T3 4, T4 4, T5 5 (incl. the vicus
  describer hook).
- Cranford probe: library 3 ms, first progress 25 ms, largest chapter
  parse + split + paginate 3 ms, 226 / 125 pages at 240 / 480 px.

