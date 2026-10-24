# LECTOR — reading EPUB books in vicus

*Designed with Fran 2026-10-23 (brainstorm in chat). A reader widget for
EPUB books: a library, a paginated page view with a table of contents,
and a calm one-sentence-at-a-time "zen" view, with companion notebooks
per page and per sentence. Latin names below are proposals until each
phase's header text is approved.*

## I. Intent

Fran wants to read real books inside vicus, the way he reads everything
else there: keyboard-first, paged (never scrolled), with every place in
a book addressable and a companion notebook (Cmd+T) beside it.

- **Sentence view is a reading mode, not a study tool**: "almost like a
  zen mode". One sentence on the screen, nothing else.
- **The companion notebook is the margin**: write beside a page or a
  sentence, come back later and find what you wrote.
- **Favourites** mark sentences to return to (a simple highlight).
- Not for translating, not for analysis.

A legacy reader exists for plain `.txt` in the old Concha shell
(`probationes/probatio_combinado.c` -> `lib/concha.c:248`
`librarium_visus_init`; `include/librarium_lector.h`,
`include/sententia_paginarium.h`). This is its successor for vicus and
for EPUB.

## II. Goals (v1)

1. Read EPUB books (Gutenberg EPUBs first: Cranford #394, Frankenstein
   #84).
2. A **library** of books to pick from.
3. **Page view**: the book paginated to the pane, chapter headings in
   the title font, **each chapter starting on a fresh page**, a **table
   of contents** to jump to chapters.
4. **Sentence view**: the current sentence alone, vertically centred,
   in a column with about ten characters of margin left and right.
5. **Fixing sentence splits** in sentence view: `J` joins with the next
   sentence; `i` opens the sentence in a scriba-style editor where a
   blank line splits it.
6. **Favourites**: whole sentences, set in sentence view only; a list of
   a book's favourites to jump back to.
7. **Addresses**: every place (library, contents, page, sentence) has a
   route, like the calendar's.
8. **Reading position per book**: opening a book always goes to the last
   position in THAT book, from any pane or tab.
9. **Companion notebooks** per page (page view) and per sentence
   (sentence view), **created only when written to**, with a small mark
   on pages and sentences that have notes.
10. **`$epubs`** opens the reader in a vicus pane; a pane resumes where it
    was (library or book); a fresh one shows the library.

## III. Non-goals (v1) and later

- Illustrations: left out. Later: each on its own page, Atkinson
  dithering, caption when the EPUB gives one.
- Italics, bold and other styling: plain paragraphs only; headings are
  the one exception.
- Arbitrary-text highlights ("persistent highlights"): later. v1
  favourites are whole sentences.
- An import button or file picker: Fran asks Claude to add books.
- Embedding the books into the vicus binary (capsula): later.
- Footnote and internal links, EPUB writing, DRM, fixed-layout EPUBs.
- **Verse** (Fran 2026-10-23: needed eventually, maybe not critical for
  v1). The legacy text reader detects verse heuristically -
  `_est_versus` in `lib/sententia_paginarium.c:100` (line-length test
  over the first V lines of a paragraph, threshold LXX%) - and then
  lays it out line by line (enjambment). EPUBs usually mark verse
  structurally (poem/stanza containers, `<br/>`). v1: `codex` keeps
  `<br/>` line breaks inside a block and records a block as verse when
  its markup says so; page view keeps those lines; sentence view treats
  a verse block's lines as its units. The text heuristic as a fallback
  for unmarked verse comes later.

## IV. Anti-goals (would make it a failure even if it works)

- **Changing the original EPUB file.** Fixes, favourites, positions and
  notes live in the volume, layered on top.
- **Notebooks created by browsing.** Following along through a book
  with the companion open must not leave hundreds of empty notebooks.
- **Fixes moving things.** Joining or splitting a sentence must not
  move other sentences' favourites, notes or the reading position.
- **Losing your place.**

## V. Decided (Fran)

| | |
|---|---|
| Entry | `$epubs`; fresh pane = library; pane resumes its last place; opening a book = that book's last reading position |
| Sentence view | the sentence alone, vertically centred, ~10 columns of margin each side |
| Fixes | `J` join with next; `i` edit, blank line = split |
| Favourites | whole sentence, sentence view only |
| Page view | headings in the title font (FONS_TITULI, as the calendar); chapters start on a fresh page |
| Pane size | fixed in vicus desktop -> pages are stable places |
| Notebooks | the left pane via Cmd+T (comites), one per page and one per sentence; created on first write; note marks |
| Storage of books | a folder, not the volume, not embedded (yet) |
| Adding books | Fran asks Claude; no import UI |
| Reading position | ONE per book, the LAST CHANGED one wins: any move in any pane (reading, page turns, contents jumps) sets it; a pane opening a book shows the book's position, not its own older one. "Simple for v1; more robust later" (Fran) |
| Favourites list | its own pages, grouped per chapter; reviewing favourites happens there and does NOT move the reading position |

## VI. Calls made by Claude (check these)

1. **Sentence identity = chapter + byte offset where the sentence starts
   in that chapter's plain text** (the text as extracted, before any
   fix). Ordinals would shift when a fix joins or splits an earlier
   sentence; offsets do not. A join keeps the first sentence's offset;
   the second's notes and favourite attach to the joined sentence. A
   split creates new offsets at the split points.
2. **Page identity = chapter + page number within the chapter.** With
   global page numbers, a fix in chapter 3 would renumber every later
   page. Chapters start on a fresh page, so a fix can only move pages
   after it in the same chapter (accepted by Fran).
3. **Reading position = a sentence identity**, saved in the volume per
   book (last change wins, Fran), updated as you move. Page view shows
   the page holding it.
4. **Fixes are a per-book list of operations in the volume** (join at
   offset X; split at offsets Y, Z), applied over the extracted text;
   the EPUB is never touched. The `i` editor accepts only inserted blank
   lines (splits); other text changes are not saved in v1.
5. **Books folder: `~/.rhubarb/codices/`** (beside the other house data
   in `~/.rhubarb`). Tests use tiny hand-made EPUBs committed under
   `probationes/fixa/`; the Gutenberg files are downloaded, never
   committed.
6. **Library rows**: title and author from the EPUB metadata, plus a
   reading-progress percentage.
7. **Keys (defaults)**: page view `[` `]` pages, `t` contents; sentence
   view `j` `k` sentences, `J` join, `i` edit, `*` favourite; a key to
   switch between page and sentence view at the same position (`s`);
   Esc goes up (sentence -> page -> contents/library). Vim-flavoured, in
   the calendar's command table style.
8. **Lazy companion creation is a change to comites for every view**,
   not a reader special case (Fran agreed in chat): an empty companion is
   shown but NOTHING is written until the first edit - not the book, not
   the `comes/<locus>/comes` search record, not the per-book viewed page.
   Empty companions created before this change are not cleaned up in v1.
9. **Join with notes on both sentences: nothing is merged or deleted.**
   Notes and favourites stay attached to each sentence's start offset.
   The joined sentence shows the first offset's notebook and counts as a
   favourite if either start offset is one; the second notebook stays
   reachable through companion search, and splitting at the same point
   again brings its offset - and its notes - back. No alias layer in
   comites, no destructive merge.
10. **Page view flows whole paragraphs** (word wrap over the paragraph's
    text, not sentence-by-sentence units), so fixes change only sentence
    boundaries and never move page breaks. Pages move only with the pane
    size (fixed in vicus desktop). `sententia_paginarium`'s per-sentence
    layout is therefore NOT the page paginator; it may serve sentence
    view's wrapping.
11. **Anchors survive drift**: every stored anchor (position, favourite,
    fix, companion locus) keeps its offset AND a short fingerprint of the
    text at that offset; on open, a mismatch triggers a re-search nearby
    and re-anchoring (or a visible "lost" mark, never a silent wrong
    attachment). The extractor's plain text is a stable contract: later
    styling (italics) adds markup beside the text, never changes it.
12. **Book identity = the EPUB's `dc:identifier`** (Gutenberg:
    `http://www.gutenberg.org/394`), not the file name; all volume state
    is keyed by it, so a re-downloaded file keeps its state.
14. **Gutenberg's extra pages are shown**: everything in the EPUB's
    reading order (front matter, licence header/footer) is part of the
    book and counts toward the progress percentage. Fran left this open;
    easy to change once he sees it.
13. **A chapter = a contents entry** (EPUB 3 nav / NCX), splitting spine
    documents at the entry's fragment anchor when several chapters share
    one file; text before the first entry is a "front matter" chapter.

## VII. Pieces

| unit | what | builds on |
|---|---|---|
| `sarcina` (lib, new) | read-only ZIP: central directory, entries by name, stored and DEFLATE entries, CRC check | `flatura_inflare`, `flatura_crc32` (include/flatura.h) |
| `codex` (lib, new) | an EPUB book: `META-INF/container.xml` -> OPF (title, author, language, manifest, spine) -> contents (EPUB 3 nav document, else EPUB 2 NCX) -> chapters as blocks (heading / paragraph) of plain text, entities decoded; sentences per chapter with offsets; fixes applied | `sarcina`, `xml.h`, `html_lexema.h`, `entitates_html.h`, `sententia_fissio.h` |
| `lector` (lib, new) | the widget: library, contents, page view, sentence view, favourites list, fixes, routes, locus; reading state in the volume | `codex`, `membrum__album` (pages), `dispositio`, `figura`, `insula`, `semitae`, `scriba_liber` (the `i` editor) |
| comites | lazy creation of companion books; note marks need "does this place have notes?" | `vicus_applicatio.c` comites code |
| vicus | `$epubs` opener (like `$agenda`, `lib/vicus_applicatio.c:3991`), genus manifest `apps/lector/organum.stml` | — |

## VIII. Phases

Each phase: header text approved first, tests red, implementation,
plants, worklog, commit on Fran's word (the pensa rhythm).

| | |
|---|---|
| **L1** | `sarcina`: ZIP reader, tested on hand-made archives (stored, deflated, bad CRC, missing entry) and a real Gutenberg EPUB. DONE 2026-10-23: in-memory archives (comment, data descriptors, ZIP64/encryption/unknown-method refusals, lying offsets), an independent macOS `zip` fixture (local extra 28 vs central 24), Cranford EPUB 134/134 entries CRC OK; see lib/sarcina.worklog.md |
| **L2** | `codex`: container, OPF, spine, contents, chapter blocks, sentences with offsets, fixes as data. DONE 2026-10-24: nav else NCX (preorder), front matter only with visible text, chapters read with `html_lexema` (xml.c loses mixed-content spaces and rejects `alt=""`), blocks heading/paragraph/verse (stanzas separate), splits-then-joins (order-free, joins stay in a block), `codex_reperire`; Cranford 21 chapters, 802 blocks, 2834 sentences, every byte covered, 0.35 s; see lib/codex.worklog.md |
| **L3** | `lector` page view: library, contents, pages (chapters on fresh pages, title-font headings), routes, reading position |
| **L4** | sentence view: centred column, `j`/`k`, switch views, `J` / `i` fixes, favourites + list |
| **L5** | companions: lazy creation (comites-wide), loci per page and sentence, note marks |
| **L6** | `$epubs` in vicus, resume rules end-to-end, the two Gutenberg books installed, screenshots |

## IX. Left to the builder

- Exact Latin names, structures and function signatures (header text
  per phase).
- How `codex` caches parsed chapters (the book is read-only; parse once
  per open).
- Pagination algorithm details (reuse or replace
  `sententia_paginarium`'s wrapping), as long as chapters start fresh
  and pages are deterministic for a pane size.
- Route spelling (`lector/<book>/<chapter>/p<n>` etc.), within the
  semitae rules.
- The favourites list's look; the note mark's look.

## AUDIENDA

- ~~Gutenberg EPUB 3 carries nav and NCX~~ - checked (Cranford: both;
  nav wins).
- ~~`xml.h` keeps `dc:title` as a plain name~~ - yes (L2 tests).
- ~~`xml.h` or `html_lexema.h` for chapters~~ - `html_lexema` (L2, Fran
  agreed): xml.c drops mixed-content whitespace and fails on `alt=""`.
- ~~`sententia_fissio` quality on Cranford~~ - measured in L2: 2834
  sentences, dialogue and quotes split well, nothing lost; 27 sentences
  over 600 B (sentence view must wrap long periods).
- That opening a companion today CREATES its book is inferred from the
  `pensa` book in K5 (`scriba_liber_aperire_in_spatio` at mount), not
  traced in the comites code.
- "Pane size is fixed" holds for vicus desktop (Fran); an Android build
  exists (memory: android toolchain) and could have other sizes.
