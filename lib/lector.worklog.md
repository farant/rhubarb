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
