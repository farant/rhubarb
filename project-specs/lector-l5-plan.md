# lector L5 — companions: lazy creation, loci, note marks Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** companion notebooks (Cmd+T) beside a page or a sentence that
cost nothing until you write in them: reading through a book with the
companion open and in follow mode leaves no trail of empty notebooks;
the first keystroke that changes a page brings the notebook into being.
Pages and sentences that have notes show a small mark.

**Architecture:** companions already work for lector (L3 gave it
`locus`/`locus_titulus`; vicus follows the right-hand view and shows
`comes/<locus>` on the left). What is missing is laziness, and it is a
comites change for EVERY view (spec VI.8), not a reader special case.
Traced 2026-10-24 - showing a companion today writes, before any typing:
(1) the book index with page "1" (`librum_aperire`, `lib/scriba_liber.c:
160`), (2) the page's manifest (`scriba_documentum_creare`,
`lib/scriba_documentum.c:597`), (3) the search record `comes/<locus>/
comes` (`comitem_scribae_ponere`, `lib/vicus_applicatio.c:1127`), and on
page changes (4) the per-book viewed page `<praefixum>/visus`
(`lib/scriba_actiones.c:654`). Follow mode calls this on every pulse
(`comitem_sequi`, `lib/vicus.c:3851`), so paging through a book writes a
notebook per page. L5 makes the document and the book able to exist
UNBORN (in memory only) and be born at their first real write; the
companion gate opens companions that way. Then lector normalises its
loci and asks the host whether a place has notes.

**Tech Stack:** C89 in Latin; `scriba_documentum`, `scriba_liber`,
`scriba_applicatio`/`scriba_actiones`, `vicus`/`vicus_applicatio`
(comites P1-P5b), `lector` (L3/L4), credo v2.

**Spec:** `project-specs/lector-spec.md` §II goal 9, §IV non-goal
"Notebooks created by browsing", §VI calls 8, 9, 11; §VIII L5;
`project-specs/comites.md` §III-§V. Plan and header text approved by
Fran 2026-10-24 ("the mark look sounds good too, so i think that's all
good"); both open questions answered below.

## Global Constraints

- Same C89/Latin/lint/examen rules as L3/L4; insula writes with the
  scriptor set; tests close temporary volumes.
- Only COMPANION books (`comes/...`) become lazy. The common book, day
  notes (`dies/<date>`), contact notes and every other scriba book keep
  today's behaviour - they are opened on purpose.
- Nothing about an unborn notebook reaches the volume: not the index,
  not a page manifest or history, not the `comes` record, not the
  viewed-page memory. View state (scriba's ramus field `liber`) is
  pane state, not a notebook, and is unchanged.
- Commit only through `silva.commissio`, only on Fran's word.

## Decisions taken in this plan (check these)

1. **What gives birth:** the first NON-EMPTY commit on any page
   (`committere` already drops empty mutations), creating a new page,
   or creating a named page. Viewing, following, turning to a page,
   title changes and route changes never do. Undo on an unborn page
   does nothing (there is nothing to undo).
2. **Birth order:** page manifest + history first (the commit itself),
   then the book index, then the `comes` record with the companion's
   LATEST title and route (the gate remembers them while unborn). After
   birth everything behaves exactly as today (title/route updates
   rewrite the record only when changed).
3. **Viewed-page memory** (`<praefixum>/visus`) is not written while the
   book is unborn (an unborn book has only page "1" anyway).
4. **Loci normalised per view** (lector): page view = book + chapter +
   page; sentence view = book + chapter + sentence (no stale `pagina`);
   library, contents and favourites = book only (no stale chapter or
   page). Today `lector_locus` is the whole route, so a sentence's
   companion would differ depending on the page you came from - Task 4
   pins this with a test first (if the leak is not real, the task
   shrinks to the test).
5. **"Has notes" = the companion book exists AND has text** (Fran,
   2026-10-24, after the look - first version was "exists"). Read-only:
   index lookup first (no notebook -> no further work), then the first
   non-blank line (`scriba_liber_lineas_legere`, maximum 1). Covers the
   empty notebooks the eager era left behind and notes written then
   deleted.
6. **The mark lives in the footer** of the view it belongs to: page
   view "12/40 ¶", sentence view "sentence 4/12 ¶" (dim, Latin-1
   pilcrow, which the 6x8 font has). Current page / sentence only - not
   in the contents or favourites lists (v1). See Open Question 1.
7. **The host answers, lector asks:** `LectorMontatio` gains a callback
   `notas_habet(ctx, locus)`; vicus sets it to "does `comes/<locus>`
   exist". Lector never learns the `comes/` key format.
8. **Not in v1:** cleaning up empty companions created before this
   change (spec VI.8); re-anchoring companion loci when a re-downloaded
   book shifts offsets (spec VI.11 - they stay reachable through
   companion search; filed as a park).

## Header text (for approval)

`include/scriba_documentum.h` - additions:

```c
/* documentum NASCITURUM (comites pigri, lector L5): in memoria solum,
 * NIHIL scribitur. Actum primum non vacuum (committere) manifestum et
 * historiam condit, deinde natale(ctx) vocat; revocare et reficere ante
 * natalem nihil agunt. Ceterum ut scriba_documentum_creare. */
ScribaDocumentum*
scriba_documentum_nasciturum (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum,
                    b32  (*natale)(vacuum* ctx),
                 vacuum* natale_ctx);

/* documentum in volumine exstat (creatum, apertum aut natum)? */
b32
scriba_documentum_natum (
    constans ScribaDocumentum* doc);
```

`include/scriba_liber.h` - additions:

```c
/* comites pigri (lector L5): liber in spatio sine scriptura. Index
 * absens: pagina "1" in memoria, paginae documenta nascitura; nihil
 * scribitur donec liber nascitur - actum primum non vacuum, pagina nova
 * aut nominata. Natalis: index scribitur, deinde natale(ctx, liber).
 * Index praesens: ut scriba_liber_aperire_in_spatio (natus). */
ScribaLiber*
scriba_liber_aperire_pigre (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    i32  latitudo,
                    i32  altitudo,
                    b32  (*natale)(vacuum* ctx, ScribaLiber* liber),
                 vacuum* natale_ctx);

/* liber in volumine exstat? (aperire_in_spatio: semper; pigre: post
 * natalem) */
b32
scriba_liber_natus (
    constans ScribaLiber* l);

/* LECTIO SOLA: liber in spatio exstat (plagula '<praefixum>/paginae')?
 * Nihil aperitur, nihil scribitur */
b32
scriba_liber_exstat (
                Volumen* volumen,
     constans character* praefixum,
                Piscina* piscina);
```

`include/lector.h` - `LectorMontatio` gains, and the header comment
gains a NOTAE paragraph:

```c
    /* L5: VERUM si locus (lector_locus) notas habet - hospes (vicus:
     * comes/<locus> exstat); NIHIL = nullae notae */
                    b32  (*notas_habet)(vacuum* ctx, chorda locus);
                 vacuum* notas_habet_ctx;
```

```c
 * NOTAE (L5): locus per visum - pagina = liber, caput, pagina;
 * sententia = liber, caput, sententia; ceteri = liber. Comes (Cmd+T)
 * nascitur scriptura prima; locus cum notis '¶' obscurum in pede
 * habet (notas_habet hospitis).
```

No header change in vicus: the gate (`librum_scribae`) and the deferred
record live in `lib/vicus_applicatio.c` (interior).

## Review Focus

1. **Follow mode leaves no trail:** with the companion shown and
   following, paging through five lector pages and three sentences
   writes no plagula under `comes/` (Task 3 counts them).
2. **Birth is complete:** the first keystroke in a companion produces
   index + page + `comes` record, and companion search finds it with
   the right title and route (Task 3; the existing P5b search steps
   must stay green with their record checks moved after the write).
3. **Born books unchanged:** the common book, day notes, contact notes
   and an already-existing companion behave as before (existing scriba
   and comites suites green, no assertion weakened except the two
   record checks that now follow the first write).
4. **Joined sentences** keep the first offset's notebook (spec VI.9):
   sentence locus = start offset of the (joined) sentence.

---

### Task 1: unborn documents (`scriba_documentum`)

**Files:** `include/scriba_documentum.h`, `lib/scriba_documentum.c`,
`probationes/probatio_scriba_documentum.c`.

- [x] Tests: `nasciturum` writes no plagula (volume census before/after);
  empty commit stays unborn; first non-empty commit writes manifest +
  history, calls `natale` once, `natum` VERUM; reopened with
  `scriba_documentum_aperire` it holds the text; undo before birth
  does nothing; second commit does not call `natale` again.
- [x] Implement (store `spatium`, `natale`; birth inside `committere`).
- [x] Plants: manifest written at creation, `natale` called twice.

  Done 2026-10-24. Found: `historia_creare` writes nothing (only acts
  are written), so the ONLY eager write was the manifest - an unborn
  document is a normal one whose manifest is not yet written; undo
  before birth is already FALSUM (no acts). The public struct gained
  four fields (`natum`, `spatium`, `natale`, `natale_ctx`) beyond the
  approved function text. `creare`/`aperire` set `natum`. Census uses
  both `volumen_summa_actorum` and `volumen_summa_plagularum` (a fresh
  temporary volume's act count is not 0). Plants red (3): manifest at
  creation, natale on every act, birth without manifest. scriba 6/6,
  vicus 28/28, ludus_tessera 8/8.

### Task 2: unborn books (`scriba_liber`)

**Files:** `include/scriba_liber.h`, `lib/scriba_liber.c`,
`probationes/probatio_scriba_liber.c`.

- [x] Tests: `aperire_pigre` on an empty space writes nothing, shows page
  "1", `natus` FALSUM, `exstat` FALSUM; first non-empty commit on its
  page writes the index, `natale` called once with the book,
  `exstat` VERUM; `pagina_nova` / `paginam_condere` also give birth;
  `aperire_pigre` on an existing book = born, `natale` never called;
  `exstat` writes nothing.
- [x] Implement.
- [x] Plants: index written at open, page doc created eagerly.

  Done 2026-10-24. `librum_aperire` takes `piger`; an unborn book's
  pages are `scriba_documentum_nasciturum` whose `natale` gives birth to
  the book (`librum_nasci`: index, then the book's `natale`, once).
  `pagina_nova` saves through `indicem_servare` (birth if unborn);
  `paginam_condere` always gives birth first, even for the existing
  "1" (naming a page is intent). Existing books opened lazily are born
  and never call `natale`. Plants red (3): index at open, page created
  eagerly, page birth not giving birth to the book. scriba 6/6, vicus
  28/28, pensa 2/2, kalendarium 2/2, ludus_tessera 8/8.

### Task 3: lazy companions (comites gate)

**Files:** `lib/vicus_applicatio.c`, `lib/scriba_actiones.c` (or
`scriba_applicatio.c`), `probationes/probatio_vicus_comites.c`.

- [x] Tests: Cmd+T on a day, following through two days and a contact:
  no `comes/` plagula; write in the third -> its index, page and record
  (latest title + route) appear, the others still absent; search finds
  it; viewed page not written while unborn. Existing P5b steps: record
  checks move after `comitem_scribere`.
- [x] Implement: gate opens `comes/...` with `aperire_pigre`; pending
  title/route per prefix in `ContextusScribae`, written at birth;
  `visus` write skipped while unborn.
- [x] Plants: record written at show, gate opens eagerly.

  Done 2026-10-24. `ComesPendens` (prefix, latest title + route) per
  lazily opened companion in `cs->pendentes`; `comitem_scribae_ponere`
  updates it and writes the record only when the book is already born
  (or was not opened lazily). The book's `natale` writes the record.
  The test checks per companion (book, record, `/visus`) rather than
  whole-volume counts, because vicus also saves pane state. The two P5b
  record checks now assert ABSENT before the first write and present
  after. Plants red (4): record at show, gate eager, `/visus` guard
  off (showing a companion DOES write `/visus` - the guard is needed),
  record at birth without route. vicus 28/28, scriba 6/6, indagatio
  4/4, lector 6/6, ludus_tessera 8/8.

### Task 4: lector loci per view

**Files:** `lib/lector.c`, `probationes/probatio_lector.c`.

- [x] Tests: page -> `s` -> locus has `sententia`, no `pagina`;
  favourites / contents / library loci = book only; two panes on the
  same sentence (one came from page 2, one from page 5) -> same locus.
- [x] Implement (locus from a normalised route copy).
- [x] Plant: stale `pagina` kept in the sentence locus.

  Done 2026-10-24. The leak WAS real: the sentence locus carried
  `pagina=`, and favourites, contents and even the library carried
  leftover `caput`/`pagina`/`sententia` (fac_ad_bibliothecam does not
  clear `sententia`). `lector_locus` now drops fields per view from the
  route node it just read (`insula_attributum_tollere`); the stored
  route is untouched, and the companion record's route stays the FULL
  route. Companions made earlier on leaky loci (only during testing)
  are not migrated. Plants red (2): pagina kept in sentence locus,
  caput kept outside page/sentence. lector 6/6, vicus_comites 2/2,
  ludus_tessera 8/8.

### Task 5: note marks

**Files:** `include/lector.h`, `lib/lector.c`, `lib/vicus_applicatio.c`,
`probationes/probatio_lector.c`, `probationes/probatio_vicus_lector.c`.

- [x] Tests (lector, fake host): `notas_habet` VERUM for the current
  locus -> footer ends with "¶" in page and sentence views; FALSUM or
  NIHIL callback -> no mark. Vicus: write in a sentence's companion,
  the sentence view shows "¶", the next sentence does not; a join
  keeps the first sentence's mark.
- [x] Implement.
- [x] Plants: mark from the wrong locus (stale page), callback ignored.

  Done 2026-10-24. `_notae_loci` asks the host with `lector_locus`;
  page and sentence footers append " \302\266" (not when a red note
  replaces the footer). Vicus: `lectoris_notas_habere` = 
  `scriba_liber_exstat("comes/" + locus)` with its own scratch piscina
  (`scheda_notarum`, emptied per call - it runs on every paint), set at
  mount. Extra end-to-end step: companion opened through
  `vicus_comitem_commutare` (the Cmd+T path) has no mark until the first
  write, then has one - proves the key matches comitem_sequi's. Plants
  red (4): foreign locus, host ignored, wrong `comes/` key, no mark in
  the sentence footer. lector 6/6, vicus 28/28, scriba 6/6,
  ludus_tessera 8/8.

### Task 6: look, docs, commit

- [x] Fran: read with the companion open and following, write a note on
  a page and on a sentence, check marks; Claude first: volume census
  after a scripted 50-page follow (zero `comes/` plagulae).

  Census 2026-10-24 (scratch probe build/sonda/sonda_sequela.c, real
  vicus over the real library, Cranford at 1440 px): 54 distinct
  companions followed (50 pages, view switch, 3 sentences) -> 0
  `comes/` plagulae; one write -> exactly 3 (record, index, page).
  Fran: "i think that looks good"; asked for "has notes = has text"
  (Decision 5 revised; vicus test pins empty book and deleted note;
  plant "exists = notes" red).
- [x] Worklogs (`lib/lector.worklog.md`, scriba/comites notes), spec
  §VIII L5 DONE, comites.md lazy note, lint, commit on Fran's word
  ("yes, lets go ahead with the commit", 2026-10-24).

## Open Questions (answered 2026-10-24)

Fran: "i don't think we have half width panes anywhere so shouldn't
have to worry. the mark look sounds good too". So: footer "¶" as in
Decision 6; page companions stay chapter + page number.

1. **The mark's look** (spec leaves it open): dim "¶" at the end of the
   footer, as above? Alternatives: a dim "¶" in the right margin beside
   the first text line, or a small dot.
2. **Page companions and pane width:** a page is "chapter + page within
   chapter", and pages depend on the pane size - a half-width pane
   numbers its pages differently, so the same page number there is a
   different notebook. The spec accepts this (pane size fixed in vicus
   desktop). Keep for v1, or key page companions by the page's first
   sentence offset instead (stable across widths, but a mark would then
   show on whichever page holds that offset)?
