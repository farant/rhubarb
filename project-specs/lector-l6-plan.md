# lector L6 — `$epubs`, resume end-to-end, the real library Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** the reader as an everyday app: `$epubs` opens it in a vicus
pane; a fresh pane shows the library; a pane reopened in its tab, or
restored after vicus restarts, comes back where it was - the library, or
the book at THAT book's last reading position (whichever pane moved it
last); and it stays quick with the real 37-book library.

**Architecture:** almost everything exists. Traced 2026-10-24:
- `$` commands that open a pane are one line each in
  `vicus_applicatio_aedificare` (`aperiens_fixum_registrare(cs->iussa,
  app->vicus, "contacts", "nomenclator", NIHIL, ...)`); with no fixed
  argument, `$epubs(<book id or URI>)` also works for free
  (`aperire_iussum` passes the argument to `vicus_acervo_aperire` ->
  `lector_argumentum_ponere`).
- Reopening a pane in a tab inherits its regular state (semitae-spec
  VI-f; the `$saints` lesson in `lib/sancti_visus.worklog.md`).
- Restart: vicus applies each pane's saved PERSISTENT route on the first
  pulse, then calls `semitam_secuta` (`semitam_restituere`,
  `lib/vicus.c:1295`). Lector's persistent fields are only `visus` and
  `liber` (L3), so a restored book pane names the book but no chapter,
  and `lector_semitam_secuta` opens it at the book's position.
- Never tested end-to-end in vicus: the L3 restart tests run on the
  widget with a hand-built route. L6 proves it across a real vicus
  rebuild over the same volume (the pattern of
  `probationes/probatio_vicus_semitae.c` "reapertura") and fixes
  whatever does not hold.

**Tech Stack:** C89 in Latin; `vicus`/`vicus_applicatio`, `lector`
(L3-L5), credo v2, scratch probes in `build/sonda/`.

**Spec:** `project-specs/lector-spec.md` §II goals 8 and 10, §V "Entry"
row, §VIII L6. Plan approved by Fran 2026-10-24 ("that looks like a
good plan. i think we can go ahead with just $epubs for now" - no
alias).

## Global Constraints

- Same C89/Latin/lint/examen rules as L3-L5; tests close temporary
  volumes.
- No header changes expected (no new API); if Task 2 finds a gap that
  needs one, its text comes to Fran first.
- Commit only through `silva.commissio`, only on Fran's word.

## Decisions taken in this plan (check these)

1. **`$epubs` opens a NEW reader pane in front** (like `$contacts`,
   `$saints`): fresh = library; in a tab where a reader pane was closed,
   it comes back with that pane's state (the platform rule). No
   "focus the existing reader instead" special case.
2. **`$epubs(<arg>)`** = the same argument as `$via(lector/...)` takes:
   a bare book id (`$epubs(http://www.gutenberg.org/394)`) opens that
   book at its position; a URI (`lector/sententia/...`) opens that view.
   Free from the existing machinery; Task 1 pins it with a test.
3. **Restart = persistent route only** (unchanged from L3): view +
   book. A restored page or sentence view reopens at the BOOK's
   position, never at a stale page of that pane - "last change wins"
   across panes and tabs (spec goal 8). Favourites and contents views
   restore as such; Esc from them still goes to the book's position.
4. **Speed bar for the real library:** library first paint, opening a
   book, a page turn and a sentence step each under ~100 ms on this
   machine with 37 books (Monte Cristo is an 85 MB EPUB, War and Peace
   has 386 chapters). Measured by a scratch probe; anything slower is
   fixed in Task 3, not filed.
5. **The library stays at 37 books** in `~/.rhubarb/codices` (outside
   the repo; Gutenberg + CCEL). Nothing is vendored into the repo.

## Review Focus

1. **Restart lands on the book's position**, not the pane's old page:
   pane A reads chapter 3, pane B then moves the same book to chapter
   1; after a restart both show chapter 1.
2. **A fresh pane is the library** even when the library has positions
   for books (no auto-open of the last book).
3. **Nothing slows down** with 37 books: progress percentages are only
   computed for books with a position (others say "new").

---

### Task 1: `$epubs`

**Files:** `lib/vicus_applicatio.c`, `apps/lector/unitas.stml`
(description), `probationes/probatio_vicus_lector.c`.

- [x] Tests: `$epubs` typed in a scriba page and clicked (the `$via`
  test's recipe) -> a lector pane in front showing the library;
  `$epubs(urn:rhubarb:alpha)` -> page view at Alpha's position;
  `$epubs(lector/sententia/...)` -> that sentence.
- [x] Implement (one registration line).
- [x] Plant: command not registered (red).

  Done 2026-10-25. Tested by typing in the scriba page and clicking, in
  a 1440 px vicus: `$epubs` -> library pane in front; `$epubs(urn:...)`
  in another tab -> page view at the book's stored position. Test
  traps (the recipe, not the feature): a click ANYWHERE on a command's
  line runs it, so each command gets its own blank line (0, 2); a
  command longer than one line of the half-width page (58 columns
  here) wraps and is no command. The URI form shares
  `lector_argumentum_ponere` with `$via` (covered by the lector suites),
  so it has no typed step. Plant (registration renamed) red.

### Task 2: resume end-to-end (vicus restart)

**Files:** `probationes/probatio_vicus_lector.c` (+ `lib/lector.c` /
`lib/vicus*.c` only if a case fails).

- [x] Tests, each: set up in vicus, rebuild vicus over the same volume,
  pulse, check the restored pane: library -> library (selected book
  kept); page view -> book at its position (chapter + page of that
  position); sentence view -> sentence view on the position's sentence;
  favourites -> favourites (Esc -> book position); contents ->
  contents; two panes on one book, B moves last -> both restore to B's
  position (Review Focus 1); reopening the reader in its tab after
  closing it -> same state; fresh tab -> library (Review Focus 2).
- [x] Fix what fails (report each to Fran with its cause).
- [x] Plants: restored page view keeps a stale page; restored sentence
  view falls back to the page view.

  Done 2026-10-26. NOTHING failed: six new vicus sections (library with
  selection; page; sentence; favourites -> Esc to the book's position,
  and contents; two panes, last mover wins, plus a fresh tab = library;
  close + reopen in its tab) passed on the first run - restart already
  worked through `semitam_restituere` -> `lector_semitam_secuta`. Plants
  red (2). Correction to Decision 1: `vicus_acervo_aperire` brings a
  pane already in the tab's stack to the front instead of mounting a
  second one (vicus.h); `$epubs` in a tab that has a reader shows it.

### Task 3: the real library (speed)

**Files:** scratch probe `build/sonda/`; fixes where the probe points.

- [x] Probe over `~/.rhubarb/codices` (37 books, 1440 px pane): library
  open + first paint, each library page, opening Monte Cristo / War
  and Peace / Les Misérables at a stored position, page turns, sentence
  steps, favourites list with a few favourites; report the table.
- [x] Fix anything over the bar (Decision 4), with a test where the
  cause is logic (e.g. a cache missed), not just speed.

  Done 2026-10-26 (probes build/sonda/sonda_celeritas.c,
  sonda_progressus.c). Reading is fast everywhere: opening Monte Cristo
  (85 MB EPUB) at chapter 45 ~1.2 ms, War and Peace ch. 247 ~0.4 ms,
  page turns avg 0.2-0.3 ms (max ~1 ms, across chapters), sentence
  steps ~0.1 ms, favourites list ~0.9 ms - only the chapter in hand is
  unzipped. ONE slow spot: library pages showing books with a position
  computed exact progress by reading EVERY chapter, once per book per
  SESSION (Anna Karenina 559 ms; a library page up to ~1.1 s). Fix:
  chapter lengths saved in the volume (plagula
  `lector/longitudines/<id>` = "<chapters> <EPUB bytes> L0 L1 ...";
  signature mismatch -> remeasure). Second session: all 37 books'
  progress 1.8 ms (was 2.3 s). Left: the FIRST time a book shows with a
  position it is still measured once (up to ~0.6 s for Anna Karenina);
  opening the library scans 37 OPFs once per vicus run (~160 ms).
  Plants red (2): saved lengths ignored, signature unchecked.

### Task 4: screenshots, look, docs, commit

- [x] Screenshots (vicus `-imago`, headless recipe in
  `probationes/probatio_ludus_imago.c`): library, contents, page view,
  sentence view, split mode, favourites list, companion with a note
  mark - opened and checked by Claude, shown to Fran.

  Done 2026-10-26: scratch probe build/sonda/sonda_imagines.c (headless
  vicus 1440 px over the real library, temporary volume) ->
  build/sonda/imagines/01..08 (library, contents, page, sentence, split
  mode, sentence with star, favourites, companion + pilcrow). All
  render as designed; first run landed in Cranford's CONTENTS chapter,
  rerun on Chapter I for prose.
- [x] Fran: `$epubs` in vicus, quit vicus, start it again: the reader
  comes back where it was. ("that looks like everything is working
  well", 2026-10-26)
- [x] Worklog, spec §VIII L6 DONE (lector arc closed), memory, ledger;
  lint; commit on Fran's word.

## Open Questions

1. **Another name too?** `$epubs` is the spec's name. An alias such as
   `$books` or `$read` costs one more line; say if you want one.
