# lector L4 — sentence view, fixes, favourites Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** the calm reading mode: one sentence alone, centred, `j`/`k`
through the book, `s` to switch with the page view at the same place;
`J` and `i` to fix sentence splits (stored per book, never touching the
EPUB); `*` to mark favourite sentences and a favourites list grouped by
chapter.

**Architecture:** everything sits on what L2/L3 built. `codex_sententiae`
already applies join/split fixes (L2); `codex_reperire` re-finds an
anchor by fingerprint. L4 adds (1) per-book fix and favourite lists in
the volume (`lector_bibliotheca`, same plagula pattern as positions),
(2) a pure layout for one sentence (`lector_paginae`), (3) two new views
in the widget (`sententia`, `favores`) and a split mode. Fixes never
change the chapter TEXT - only sentence boundaries - so every stored
offset (positions, favourites, fixes) stays valid; a position whose
sentence got joined simply lands inside the joined sentence.

**Tech Stack:** C89 in Latin; `codex` (L2), `lector_*` (L3), insula +
manifest + semita, `membrum__album` (favourites pages, ALBUM_TITULUS
chapter groups), `membrum__praecepta`, credo v2.

**Spec:** `project-specs/lector-spec.md` §II goals 4-6, §IV anti-goals
("fixes moving things", "losing your place"), §V (favourites, sentence
view), §VI calls 1, 4, 9, 11; §VIII L4. Plan and header text approved by Fran 2026-10-24 ("that
all sounds good to me, i think the split mode is a good approach").

## Global Constraints

- Same C89/Latin/lint/examen rules as L3; insula writes with the
  scriptor set; tests close temporary volumes.
- The EPUB is never written. Fixes and favourites live in the volume,
  per book (keyed by dc:identifier), last write wins.
- A fix or favourite is an ANCHOR: chapter + byte offset + fingerprint
  (first <= 32 bytes, UTF-8 cut), re-found with `codex_reperire` when a
  chapter is read; one that cannot be re-found is NOT applied and is
  counted, never silently attached elsewhere (spec VI.11).
- Commit only through `silva.commissio`, only on Fran's word.

## Decisions taken in this plan (check these)

1. **The `i` editor is a lector-native SPLIT MODE, not an embedded
   scriba.** `i` shows the sentence with a word cursor; `w`/`l` and
   `b`/`h` move it word by word, Enter toggles a split before the word
   under the cursor (drawn as a blank line), Esc saves and leaves. It
   reads like scriba (vim keys, blank line = split) but edits only
   splits, which is all v1 saves (spec VI.4). The alternative - the
   nomenclator's nested scriba post-it (`membrum__charta`) holding the
   sentence in a scratch notebook, then diffing the edited text back to
   split offsets - gives full vim but must reconcile hard-wrapped grid
   text and discard every non-split edit; much more machinery for the
   same result. **Fran chose split mode (2026-10-24).**
2. **Fixes cancel their inverse.** `J` on a sentence whose following
   boundary exists only because of a split fix REMOVES that split fix;
   a split at a point a join fix had removed REMOVES that join. So the
   stored list stays minimal and every fix is undoable with the opposite
   key (spec VI.9: splitting at the same point brings the offset back).
3. **Sentence view font:** the sentence in FONS_TITULI (8x16) when it
   fits the column, else FONS_PARVUS double-spaced; a sentence too tall
   even then shows what fits and a dim "…" (rare: Cranford's longest
   ~600 bytes fits a real pane at 6x8). Column = pane minus the page
   view's margins (same `_margines`), centred horizontally, the block
   centred vertically.
4. **Around the sentence:** chapter title dim in the top margin (as the
   page view), "sentence 12/153" dim in the bottom margin, a dim `*`
   beside the sentence when it is a favourite.
5. **Position in sentence view** = the current sentence (written on
   every `j`/`k`, like page turns). `s` from the page view opens the
   reading position's sentence if it is on the shown page, else the
   first sentence starting on that page; `s` back shows the page holding
   the sentence. A stored offset inside a joined sentence shows that
   joined sentence.
6. **Favourites list** (`visus=favores`, key `f` from page or sentence
   view): pages grouped by chapter (chapter title as album group header,
   repeated on continuation pages), each favourite as its wrapped
   sentence text. Browsing it never moves the position (spec V); Enter
   on one jumps there in sentence view, which IS a move. A joined
   sentence counts as a favourite if any of its parts was one (spec
   VI.9); `*` off clears all its parts.
7. **Lost anchors:** a fix or favourite that cannot be re-found is kept
   in storage but not applied; the sentence view's bottom margin says
   "2 marks could not be placed" for that chapter (dim).
8. **Routes:** new `visus` options `sententia` and `favores`; field
   `sententia` (byte offset, transient, `semita="status"`,
   `ambitus="ligatio"`) like `caput`/`pagina`, so a sentence has an
   address (`lector/sententia/<id>?caput=4&sententia=1234`) for L5's
   per-sentence companions, and restart still follows the book.

## Header text (approved with the plan)

`include/lector_bibliotheca.h` - additions:

```c
/* emendatio ancorata (lector L4): genus, caput, positio (octeti in
 * textu capitis), vestigium (octeti primi sententiae ibi incipientis,
 * XXXII summum) */
nomen structura {
    CodexEmendationisGenus  genus;
                       i32  caput;
                       i32  positio;
                    chorda  vestigium;
} LectorEmendatio;

/* emendationes libri (Xar de LectorEmendatio, ordine scripturae);
 * vacuum si nullae; NIHIL sine volumine. Plagula "lector/emendationes/
 * <identitas %XX>" */
Xar*
lector_emendationes_legere (
    LectorBibliotheca* b,
               chorda  identitas,
             Piscina* piscina);

/* omnes scribere (ultima vincit) */
b32
lector_emendationes_scribere (
    LectorBibliotheca* b,
               chorda  identitas,
                  Xar* emendationes);

/* favores libri (Xar de LectorPositio); plagula "lector/favores/
 * <identitas %XX>" */
Xar*
lector_favores_legere (
    LectorBibliotheca* b,
               chorda  identitas,
             Piscina* piscina);

b32
lector_favores_scribere (
    LectorBibliotheca* b,
               chorda  identitas,
                  Xar* favores);
```

`include/lector_paginae.h` - addition:

```c
/* sententia sola (visus sententiae) in area latitudo x altitudo:
 * FONS_TITULI si tota capit, aliter FONS_PARVUS spatio duplici; lineae
 * centratae, grex verticaliter centratus; '\n' (titulus, versus) linea
 * nova. Non capit: lineae quae capiunt et *praecisa VERUM. Xar de
 * LectorLinea (x, y in area) */
Xar*
lector_sententiam_disponere (
       constans CodexCaput* caput,
    constans CodexSententia* sententia,
                       i32  latitudo,
                       i32  altitudo,
                       b32* praecisa,
                  Piscina* piscina);
```

`include/lector.h` - header comment gains:

```c
 * SENTENTIA: sententia sola in medio (lector_sententiam_disponere),
 * titulus capitis supra, "sentence 12/153" infra, '*' si favor.
 * Claves: j k (sententia; trans capita), s (ad paginam), J (iungere
 * cum sequente; iunctura per fissuram facta tollitur), i (modus
 * findendi), * (favor), f (favores), t (index), Esc (ad paginam).
 * MODUS FINDENDI: cursor verborum (w l, b h), Enter fissuram ante
 * verbum vertit (linea vacua), Esc servat.
 * PAGINA: s (ad sententiam), f (favores).
 * FAVORES: per caput (titulus gregis), sententiae involutae; j k, [ ],
 * Enter (ad sententiam - motus), Esc (unde venit). Lustratio positionem
 * non movet.
```

`apps/lector/organum.stml`: `visus` + `sententia`, `favores`;
`sententia` numerus transiens `semita="status" ambitus="ligatio"`;
`electus_favor` numerus transiens; `cursor_findendi` numerus transiens;
`fissurae` textus transiens ("12,40" pending split offsets);
`visus_prior` electio transiens (where Esc from favourites returns).

## Review Focus

1. **Undo by the opposite key:** `J` then `i`+split at the same point
   restores the original two sentences with the original offsets, and
   the stored fix list is empty again. Pinned in Task 4/5.
2. **Fixes move nothing else:** a favourite and the reading position on
   a sentence that gets joined into its predecessor stay attached (the
   joined sentence shows `*`, opening the book shows it); split it
   again and they are back on their own sentence. Pinned in Task 4.
3. **Text shifted / anchor lost:** a book re-downloaded with a changed
   chapter - fixes re-found nearby by fingerprint, lost ones not applied
   and counted. Pinned in Task 1/3.
4. **Chapter boundaries in sentence view:** `j` on the last sentence of
   a chapter goes to the first of the next (heading sentence), `k`
   back; `J` on a chapter's last sentence does nothing (L2 rule: joins
   stay inside a block). Pinned in Task 3/4.
5. **Two panes:** pane A joins two sentences; pane B, opening the
   chapter after that, shows them joined (fixes are per book, read when
   the chapter is (re)built). Pinned in Task 4.

---

### Task 1: fixes and favourites in the volume (`lector_bibliotheca`)

**Files:** `include/lector_bibliotheca.h` (additions above),
`lib/lector_bibliotheca.c`, `probationes/probatio_lector_bibliotheca.c`
(section `emendationes`).

- [ ] Tests: empty -> vacuum Xar; write 3 fixes (both kinds, one with a
  `"`/`<` fingerprint) and read them back in order; second write wins;
  favourites round-trip; ids with ':' '/' use `%XX` keys (pin
  `lector/emendationes/http%3A%2F%2Fwww.gutenberg.org%2F394`); a second
  bibliotheca over the same volume sees them; no volume -> NIHIL /
  FALSUM.
- [ ] Implement with the position pattern: one plagula per book and
  kind, STML `<emendationes><emendatio genus="iungere" caput positio>
  vestigium</emendatio>...</emendationes>` (fingerprint as TEXT),
  throwaway piscina per write.
- [ ] Plants: list order lost, kind not round-tripped, raw key.

### Task 2: one sentence laid out (`lector_paginae`)

**Files:** `include/lector_paginae.h` (addition above),
`lib/lector_paginae.c`, `probationes/probatio_lector_paginae.c`
(section `sententia`).

- [ ] Tests: short sentence -> FONS_TITULI, one line, centred (x =
  (latitudo - runes*8)/2, y = (altitudo-16)/2); a sentence too long
  for 8x16 but fine at 6x8 -> FONS_PARVUS, lines 16 px apart, block
  centred; heading sentence with '\n' -> two lines; too tall even at
  6x8 -> `*praecisa`, lines within the area; curly quotes counted as
  one cell.
- [ ] Implement by reusing the paginator's wrap (same break rules) for
  a single span, then centring.
- [ ] Plants: no fallback to 6x8, centring off, '\n' ignored.

### Task 3: sentence view (`lector`)

**Files:** `lib/lector.c`, `include/lector.h` (comment), manifest +
`./tools/organa_infigere.sh`, `probationes/probatio_lector.c` (section
`sententia`).

- [ ] Tests (Alpha): from page view `s` -> `visus=sententia`, the
  position's sentence painted (chapter 3 "Primum hic." etc.); `j` ->
  next sentence, position written; `j` at chapter end -> next chapter's
  heading sentence; `k` back; `s` -> page view on the page holding it;
  bottom margin "sentence 2/5"; route `lector/sententia/<id>?caput=3&
  sententia=N` opens it; restart route has no `sententia`; lost-anchor
  count shown when a stored fix cannot be re-found (Review 3).
- [ ] Implement: cache gains the chapter's applied fixes (re-anchored
  per chapter with codex_reperire, lost counted) and favourites; the
  current sentence = the one containing the `sententia` offset.
- [ ] Plants: j not crossing chapters, offset-in-joined-sentence not
  resolved, `sententia` persistent.

### Task 4: `J` and `*` (and the inverse rule)

(RULING at execution: `i32 lector_bibliotheca_versio(b)` added to the
library header - an in-process counter bumped on every fix/favourite
write; a pane rebuilds its cached chapter when it moves. Position writes
do not bump it. The two-pane test is widget-level with two panes over
one library (same mechanism as vicus).)

**Files:** `lib/lector.c`, tests (section `emendare`), vicus test
(section `emendare` - two panes).

- [ ] Tests: `J` on "Primum hic." -> "Primum hic. Secundum ibi."
  painted, one IUNGERE stored; `*` -> favourite stored, `*` painted;
  favourite + position on "Secundum ibi." then `J` on the previous
  sentence -> joined sentence shows `*`, reopening the book shows it
  (Review 2); `J` on the chapter's last sentence -> nothing stored;
  after a split fix at P, `J` on the sentence before P removes that
  split fix (list empty) (Decision 2); two panes: A joins, B opens the
  chapter -> joined (Review 5).
- [ ] Implement `J`/`*` via the Task 1 lists, the inverse rule using
  `codex_sententiae` with splits only.
- [ ] Plants: inverse rule off (list grows), joined favourite not
  shown, fixes cached across panes (B stale).

### Task 5: split mode (`i`)

**Files:** `lib/lector.c`, tests (section `findere`).

- [x] Tests: `i` -> split mode, cursor on the sentence's 2nd word; `w`
  `w` Enter -> blank line painted before that word; Esc -> FINDERE
  stored at that word's offset, two sentences; split at the point a
  join removed -> the join fix is removed instead (Review 1); Enter
  twice on the same word -> no split; Esc with no splits -> nothing
  stored.
- [x] Implement with transient fields `cursor_findendi`, `fissurae`.
- [x] Plants: split stored at the cursor's previous word, inverse rule
  off.

  Done 2026-10-24 (uncommitted). Keys in split mode get their own
  praecepta mask `VISUS_FINDENDI` (sentence keys j k J * s t sleep
  while the cursor field is present); `_visus` still says SENTENTIAE.
  Stored splits re-anchored -> `_partes_fissae` (shared with `J`).
  Commit per pending offset: a split-only part start whose previous
  part carries a stored IUNGERE -> that IUNGERE is removed; a part
  start without one -> nothing; else FINDERE with the part's text as
  vestigium. Painting: each part laid out by
  `lector_sententiam_disponere`, stacked with one blank line, block
  left-aligned on the longest line; the whole sentence's font is kept
  (6x8 parts are laid out 1 px short of their 8x16 need, which makes
  disponere choose 6x8 uncut). Cursor word underlined; footer adds
  " - splitting". Extra test step: longus sentence 2 in a 112 px pane
  stays 6x8 after a split. Plants (7 red; "previous word" became
  "split stored at the part's end", same check): split at part end, inverse
  off, cursor before 2nd word, Enter twice keeps, split keys off,
  parts not painted, 6x8 parts drawn 8x16 -> all red; a trailing-space
  trim plant stayed green (the wrap already ends lines at the last
  word) so the trim was removed as dead code.

### Task 6: favourites list

**Files:** `lib/lector.c`, tests (section `favores`).

- [x] Tests: favourites in chapters 1 and 3 -> list shows two groups
  with chapter titles; small pane -> group title repeated on the
  continuation page; `j`/`k`/`[`/`]` move without writing the position
  (stored position unchanged); Enter -> sentence view on it (position
  written); Esc -> back where `f` was pressed; no favourites -> "No
  favourites yet".
- [x] Plants: browsing writes the position, group title not repeated.

  Done 2026-10-24 (uncommitted), tests ticked too. Fran's addition:
  consecutive favourite sentences are ONE entry (a run), derived when
  the list is built - storage stays per sentence, so `*` on a middle
  sentence splits the run. Runs cross paragraph breaks, never chapters
  (Fran chose). Fran approved a header addition: `lector_sententiam_
  involvere` (the existing wrapper, font given) in lector_paginae.h;
  split mode now uses it (the 1-px-short trick is gone) and list entries
  wrap in 6x8 with it. The list is cached per book/versio/width in its
  own piscina (`arca_favorum`); chapters with stored favourites are
  read there with fixes applied (`_emendationes_capitis` /
  `_favores_capitis` now take caput + piscina + amissae, not the page
  cache). `f` is one praeceptum for PAGINAE|SENTENTIAE (mask); `f`
  selects the first entry. Esc with no `caput` (pane restored straight
  into the list) opens the book at its position. Pane title "Book -
  Favourites". Plants red (6): browsing writes position, title not
  repeated, runs split per sentence, Esc always to page, Enter to chapter
  start (needed the test to open 'Tertium!', not an entry at offset 0),
  involvere ignores the font.

### Task 7: Cranford, docs, commit

- [x] Fran: read Cranford in sentence view, join/split a few, star a
  few, open the list. Claude first: probe the longest Cranford
  sentences in a real pane (does 8x16 / 6x8 fit?).
- [ ] Worklog, spec §VIII L4 DONE, lint, commit on Fran's word.
