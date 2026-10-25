# lector worklog

## 2026-10-24 - L3: the reader widget (library, contents, page view)

Spec project-specs/lector-spec.md, plan project-specs/lector-l3-plan.md.
Three units: `lector_paginae` (pure layout), `lector_bibliotheca`
(shared model), `lector` (the vicus widget, genus "lector").

### lector_paginae - a chapter into pages

Pure function of (CodexCaput, area px). Headings FONS_TITULI (8x16),
everything else FONS_PARVUS (6x8); both fonts are monospaced bitmaps that
draw one cell per CODEPOINT (curly quotes, em dashes = one cell), so
widths count codepoints via utf8_decodere, never bytes. Paragraphs flow
whole (spec VI.10): greedy wrap at the last ASCII space, a word longer
than the line is cut at a codepoint boundary, trailing spaces trimmed.
Verse: one source line per line, continuation indented 2 cells. 8 px gap
between blocks, never at the top of a page. A page always takes at least
one line (a pane smaller than a line still terminates). Empty chapter =
one page with no lines. Cranford: 226 pages at a 240-wide pane, 125 at
480; the whole book paginated twice in 0.04 s.

### lector_bibliotheca - the shared model

One per vicus (lazy, from RHUBARB_CODICES else $HOME/.rhubarb/codices),
shared by all panes, positions in vicus's volume.
- One book per dc:identifier; duplicates: first file by name wins (a
  re-downloaded copy keeps its state). No identifier -> file name.
  Unreadable `.epub` stays as a row with its error; other files ignored.
  Order: readable by title (ASCII case-insensitive), unreadable by file.
- Position plagula `lector/positio/<identity %XX>` (key pinned by test).
  STML `<positio caput initium>vestigium</positio>`: the fingerprint is
  element TEXT because STML attribute values are stored RAW - a `"` in a
  sentence broke the record (same lesson as effectus). Fingerprint <= 32
  bytes, cut on a UTF-8 boundary.
- Every write uses a throwaway piscina + interner: page turns write a
  position each time and must not grow the long-lived pool or interner.
- Progress = bytes before the position / all chapter bytes; chapter
  lengths measured once per book (Cranford 25 ms) then cached.

### lector - the widget

Mirrors sancti_visus: insula fields written in mutare_ramum with the
scriptor set, dispositio in 6x8 cells, membrum__album rows, membrum__
praecepta key table, one root figure. Views: bibliotheca, index, pagina.
- Per-pane cache (piscina `arca`): (book, chapter, area) -> CodexCaput,
  sentences, pages; rebuilt only when one changes. `_area()` is shared
  by layout and pagination so pages and painting agree. Largest Cranford
  chapter: parse + split + paginate 3 ms.
- Moves (page turns, contents Enter, a route naming a chapter) write the
  book position = first sentence that STARTS on the shown page, else the
  sentence containing the page start. Opening a book = stored position
  -> codex_reperire -> its page; not found -> chapter start with
  "position not found" (not rewritten); none -> chapter 0, written.
- Routes: visus + liber are persistent (ligatio); caput + pagina are
  TRANSIENT status fields, so a restarted pane re-reads the BOOK's
  position (last change wins even across a restart). An explicit route
  naming caput is a move.
- Trap (caught by the two-pane test): a bare book id in a pane already on
  that book does not change the binding, so ambitus="ligatio" never
  reset caput/pagina - semitam_secuta saw the pane's stale chapter, took
  it for a move and REWROTE the book position. argumentum_ponere now
  looks at the ROUTE (does it name caput?) and clears caput/pagina when
  it does not.
- locus = canonical route incl. caput/pagina (per-page companions, L5);
  locus_titulus "Book - Chapter - k/n", contents = book title, library
  "Library".

### Testing notes

- Fixtures: probationes/fixa/lector/bibliotheca (alpha, beta titled
  "aeneis" so case-sensitive and case-insensitive order differ, gamma =
  beta's id, fractum.epub, nota.txt) and probationes/fixa/lector/longus
  (one long chapter, own folder): the small books never start a page
  mid-sentence, so "position = page's first byte" was indistinguishable
  from the real rule until Longus.
- `$via` from a scriba page: a `$verb(...)` must close on the SAME line;
  the scriba page in tab 3 is half width (~38 columns), long routes wrap
  and are silently not commands. (First misread as a `%` problem.)
- Plants: 24 over the five tasks, all red (list in the plan's execution
  notes).

## 2026-10-24 - book margins on the page view (Fran)

Page view now has margins like a printed book: 10 characters left and
right, 6 lines top and bottom (inside the pane's one-cell frame).
`_margines()` computes them once for both `_area()` (pagination) and the
painter, so pages and painting cannot disagree. They shrink only when
the pane is too small: the side margin is half of whatever exceeds a
40-character column (clamped 1..10), top/bottom half of whatever exceeds
20 lines (clamped 1..6). The top margin carries the running head
(chapter title, dim, centred), the bottom margin the folio "k/n"
(centred) or "position not found" in red. Positions are sentences, so
the extra pages this creates move nobody's place. Tests pin x/y of the
first line in a 480 px pane (6 + 10 cells, 8 + 6 lines), the narrow-pane
clamp, and that no line of Longus runs into the right margin (that last
one catches an `_area` that ignores the margins).

## 2026-10-24 - double-spaced text lines (Fran)

Tried as an experiment, kept: every 6x8 text line is followed by an 8 px
blank line (`INTERVALLUM_LINEARUM` in lector_paginae.c); headings keep their 16 px
pitch, the 8 px gap between blocks stays (paragraphs read as 16 px of
white). The blank AFTER the last line of a page is not required to fit
(fit test uses the visible height, the advance uses height + blank), so
a 72 px page holds five lines, not four. Side effect found by the round
trip: with only a few lines per page, a chapter's LAST page can hold just
the tail of its final sentence - no sentence starts there, so (Decision 1,
accepted in the plan) reopening shows the page before. The round-trip
test now runs in a pane with five lines per page and allows exactly that
case on the last page only; the "first byte" plant still fails it.

## 2026-10-24 - L4: sentence view, fixes, favourites (plan project-specs/lector-l4-plan.md)

**Sentence view.** `lector_sententiam_disponere` picks 8x16 if the whole
sentence fits the page view's text area, else 6x8 double-spaced, else the
lines that fit + "..." in the footer. Multi-line sentences are a block
centred on its longest line with lines left-aligned inside it (Fran:
"text-align: left"). Census over 12 Gutenberg books (~105k sentences, scratch
probe `build/sonda/sonda_codices.c`): at a 1440x860 pane every sentence is
8x16; at 720x430 about 0.1% fall to 6x8 and 4 Hugo sentences (Les Mis,
2.4 KB max) are cut. So 6x8 is a rare fallback, "..." rarer.

**Fixes are minimal and undoable by the opposite key.** `J` at a boundary
made by a stored split removes that split; otherwise the IUNGERE goes on
the start of the split-only part ending at the boundary (so chains
extend right). Split mode's Esc mirrors it: a pending split at a
split-only part start whose previous part carries a stored IUNGERE
removes that IUNGERE; at a part start without one, nothing; else
FINDERE with the part's text as fingerprint. `_partes_fissae` (stored
splits re-anchored, joins ignored) is the shared ground truth for both.
Split mode has its own praecepta mask (`VISUS_FINDENDI`) while
`cursor_findendi` is present, so j/k/J/* cannot fire mid-split.

**Two-pane freshness** = `lector_bibliotheca_versio` (in-process counter
bumped on fix/favourite writes) in every cache key.

**Favourites list.** Built from per-sentence storage at list time;
consecutive favourite sentences (across paragraphs, never chapters) are
one entry (Fran), so `*` mid-run splits it. Own cache (`arca_favorum`,
keyed by book/versio/row width); reads only chapters with stored
favourites, fixes applied. The re-anchoring helpers now take caput +
piscina + lost counter instead of the page cache. Browsing writes only
`electus_favor`; Enter is the move. Esc with no `caput` (pane restored
straight into the list) opens the book at its position.

**API.** `lector_sententiam_involvere` (font given) is public now (Fran
approved); it replaced a 1-px-short-height trick that coaxed disponere
into 6x8 for split-mode parts.

**Plants that proved nothing at first:** "Enter jumps to chapter start"
stayed green because the opened entry started at offset 0 - test now
opens "Tertium!"; a "runs not merged" plant crashed instead of failing -
replaced with one that splits every run. A trailing-space trim stayed
green under a plant because the wrapper already ends lines at the last
word: trim removed as dead code.

Selected library rows paint their dim text (author, progress,
unreadable) in COLOR_BACKGROUND, like nomenclator (Fran).
