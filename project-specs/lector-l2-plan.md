# lector L2 — codex (an EPUB book as chapters, blocks, sentences) Implementation Plan

> **For agentic workers:** this repo's standing method (Fran): tasks are
> executed INLINE in the main session, one task per turn, Fran approves
> each before the next; no implementation subagents. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** open an EPUB (via `sarcina`) and expose it as metadata, an
ordered list of chapters (one per contents entry, plus front matter),
each chapter as plain text cut into blocks (heading / paragraph /
verse), and the chapter's sentences with stable byte offsets, with
split/join fixes applied as data.

**Architecture:** `codex_aperire` reads `META-INF/container.xml` -> the
OPF (metadata, manifest, spine) -> the contents (EPUB 3 nav document,
else EPUB 2 NCX) and builds the chapter table: each contents entry is
an anchor (spine document + fragment); a chapter runs from its anchor
to the next entry's anchor in spine order; text before the first entry
is a front-matter chapter. `codex_caput_legere` parses the chapter's
XHTML with `xml.h` and emits the chapter text + blocks.
`codex_sententiae` cuts a chapter into sentences (`sententia_fissio_
indices` for paragraphs, one per heading, one per verse line) and
applies fixes. No volume, no UI: pure data (state lives in L3+).

**Tech Stack:** C89 in Latin; `sarcina` (L1, 13d3a6b3), `xml.h`
(`xml_legere`, element names may contain ':' - lib/xml.c:26),
`sententia_fissio_indices` (include/sententia_fissio.h), credo v2.

**Spec:** `project-specs/lector-spec.md` §VI calls 1, 11-14 and §VIII
L2; recon of the real Cranford EPUB 2026-10-23 (below).

## Recon (Cranford, gutenberg.org #394 epub3.images)

- OPF `OEBPS/content.opf`, EPUB 3.0, `unique-identifier="id"` ->
  `<dc:identifier id="id">http://www.gutenberg.org/394</dc:identifier>`;
  `dc:title` Cranford, `dc:creator` Elizabeth Cleghorn Gaskell (a second
  `dc:contributor` is the illustrator), `dc:language` en.
- Spine: 47 itemrefs - `coverpage-wrapper`, `pg-header`, 20 chapter
  files `…_394-h-0 … -19.htm.xhtml`, and 24 illustration WRAPPER pages
  (`…wrap-0.html.xhtml`: one `<img>` + a "back" link, no other text)
  between them.
- Contents: `OEBPS/toc.xhtml` (nav, 20 entries: CRANFORD, CONTENTS,
  LIST OF ILLUSTRATIONS, CHAPTER I … XVI, THE FULL PROJECT GUTENBERG™
  LICENSE) - each `file#anchor`, anchor near the file's start; also
  `toc.ncx`.
- Chapter body: `div.chapter > h2 (with <br/>), p (with <i>)`; curly
  quotes as UTF-8; source lines hard-wrapped (whitespace to collapse);
  no DOCTYPE (lib/xml.c:1538 tolerates one anyway).

## Global Constraints

- C89 + latina.h rules (Roman numerals are macros; `i32` unsigned,
  `s32` signed; `chorda` not NUL-terminated); new lint words real Latin.
- **The chapter text is a stable contract** (spec call 11): blocks in
  document order joined by exactly `"\n\n"`; inside a heading or verse
  block `<br/>` = `'\n'`; whitespace runs collapse to one space, lines
  trimmed; `<i>`/`<b>` etc. contribute their text only. Later styling
  must never change this text.
- Offsets are BYTE offsets into the chapter text (spec call 1).
- Never crash on a bad EPUB: errors are English messages.
- Commit only through `silva.commissio`, only on Fran's word.

## Header text (approved with the plan)

`include/codex.h`:

```c
/* codex.h - liber EPUB (lector L2; spec project-specs/lector-spec.md):
 * metadata, capita (unum per introitum indicis, et prooemium ante
 * primum), caput = textus planus in clausulas sectus (titulus,
 * paragraphus, versus), sententiae cum offsetibus stabilibus,
 * emendationes (iungere, findere) ut data. Arca per sarcina, XHTML per
 * xml.h. Nulla UI, nullum volumen.
 *
 * TEXTUS CAPITIS (contractus stabilis): clausulae ordine documenti
 * "\n\n" iunctae; in titulo et versu <br/> = '\n'; spatia contracta,
 * lineae purgatae; <i> <b> etc. solum textum dant. Offsets = octeti in
 * hoc textu. Paginae involucrorum imaginum (nihil textus extra <a>)
 * omittuntur (imagines non in v1). */

nomen structura Codex Codex;   /* opacus */

nomen enumeratio {
    CODEX_TITULUS = ZEPHYRUM,  /* h1-h6 */
    CODEX_PARAGRAPHUS,         /* p, li, ceteri */
    CODEX_VERSUS               /* poem/stanza/verse in classe */
} CodexGenus;

nomen structura {
    CodexGenus  genus;
           i32  initium;   /* octeti in textu capitis */
           i32  finis;     /* exclusivus */
} CodexClausula;

nomen structura {
    chorda  titulus;     /* nomen in indice; vacuum = prooemium */
    chorda  textus;      /* textus capitis (contractus supra) */
       Xar* clausulae;   /* CodexClausula */
} CodexCaput;

nomen structura {
    i32 initium;      /* octeti in textu capitis = IDENTITAS */
    i32 finis;
    i32 clausula;     /* index clausulae */
} CodexSententia;

nomen enumeratio {
    CODEX_IUNGERE = ZEPHYRUM,   /* sententia ad positionem + sequens */
    CODEX_FINDERE               /* sententia nova incipit ad positionem */
} CodexEmendationisGenus;

nomen structura {
    CodexEmendationisGenus  genus;
                       i32  positio;
} CodexEmendatio;

/* datum = arca EPUB (filum lectum); NIHIL et *vitium si non est */
Codex*
codex_aperire (
                 chorda  datum,
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda* vitium);

chorda codex_identitas (constans Codex* c);  /* dc:identifier unicus */
chorda codex_titulus   (constans Codex* c);  /* dc:title */
chorda codex_auctor    (constans Codex* c);  /* dc:creator primus */
chorda codex_lingua    (constans Codex* c);  /* dc:language */

i32
codex_numerus_capitum (
    constans Codex* c);

/* caput k (textus, clausulae) in piscina; NIHIL et *vitium si XHTML
 * frangitur */
CodexCaput*
codex_caput_legere (
    constans Codex* c,
               i32  k,
          Piscina* piscina,
           chorda* vitium);

/* sententiae capitis ordine, emendationibus applicatis (ordo
 * emendationum non refert); Xar de CodexSententia */
Xar*
codex_sententiae (
    constans CodexCaput* caput,
    constans CodexEmendatio* emendationes,
                     i32  numerus,
                Piscina* piscina);

/* sententiam ancoratam reperire: positio + vestigium (primi octeti
 * eius textus) -> positio vera; congruit -> positio; aliter quaeritur
 * prope (+/- M octeti) - inventa -> ea, aliter -1 */
s32
codex_reperire (
    constans CodexCaput* caput,
                     i32  positio,
                  chorda  vestigium);
```

(The four metadata getters are written on one line each above only for
the plan's brevity; the header uses house formatting.)

Rulings at execution (recorded in the header as committed): Task 2 added
`chorda codex_caput_titulus(constans Codex* c, i32 k)` (contents view
needs titles without parsing chapters); Task 3 changed the
`codex_caput_legere` comment (html_lexema, tolerant; NIHIL only for k
out of range or a missing/unreadable member); Task 4 spelled out the fix
rules in the `codex_sententiae` comment (splits before joins, joins
within a block, split part starts at the first non-space).

## Review Focus

1. **Two chapters in one XHTML file** (anchor mid-file): the second
   chapter starts at its anchor, the first ends there. Pinned in Task 2
   (fixture `c1.xhtml` holds chapters I and II).
2. **EPUB 2 without a nav document**: contents from the NCX. Pinned in
   Task 2 (second fixture).
3. **Illustration wrapper pages** contribute nothing (no "back"
   paragraph). Pinned in Task 3.
4. **Whitespace and line breaks**: hard-wrapped source collapses; `<br/>`
   inside a heading stays a line break; entities decoded. Pinned in
   Task 3.
5. **Fixes that overlap or point nowhere** (join at the last sentence,
   split at a non-boundary inside a word, the same fix twice): no crash,
   documented result. Pinned in Task 4.

---

### Task 1: fixtures, opening, metadata, spine

**Files:** create `include/codex.h`, `lib/codex.c`,
`probationes/probatio_codex.c`, fixtures
`probationes/fixa/codex/minimus/` (sources) + `minimus.epub`, and
`probationes/fixa/codex/vetus/` + `vetus.epub` (EPUB 2, NCX only, used
in Task 2); regenerate `compile_tests_fontes_generata.sh`.

- [ ] **Step 1: Fixtures** (made with macOS `zip`, NOT our code, as in
  L1: `zip -X0 x.epub mimetype && zip -Xr x.epub META-INF OEBPS`;
  `-X` is fine here - these fixtures test EPUB structure, not ZIP
  fields):
  - `minimus`: OPF in `OEBPS/` with `dc:identifier`
    `urn:rhubarb:minimus`, title `Minimus`, creator `Anna Scriptrix`,
    language `la`; spine `c0 c1 wrap c2`; nav listing
    `c1.xhtml#cap1` "Caput I", `c1.xhtml#cap2` "Caput II",
    `c2.xhtml` "Caput III" (no fragment); `c0.xhtml` = front matter
    (one paragraph "Prooemium."); `c1.xhtml` = chapters I and II
    (anchors `cap1`, `cap2`, a heading with `<br/>`, hard-wrapped
    paragraphs, an `<i>`, an `&amp;`); `wrap.xhtml` = an illustration
    wrapper (img + "back" link); `c2.xhtml` = a `div class="poem"`
    with two lines `<br/>`-separated, then a paragraph of three
    sentences.
  - `vetus`: the same text as EPUB 2.0: no nav, `toc.ncx` with nested
    navPoints (Caput II nested under Caput I).
  - `sine_prooemio`: `minimus` without `c0.xhtml` in the spine (first
    entry at the start of the first text document).
  - `fractus`: `minimus` whose `c2.xhtml` has an unclosed `<p>`.
- [ ] **Step 2: Failing tests** (section `aperire`): `minimus` opens;
  identitas `urn:rhubarb:minimus`, titulus `Minimus`, auctor `Anna
  Scriptrix`, lingua `la`; errors: not a ZIP; a ZIP without
  `META-INF/container.xml`; container pointing to a missing OPF - each
  NIHIL + vitium.
- [ ] **Step 3: Implement** `codex_aperire` (container -> rootfile
  `full-path`; OPF via `xml_legere`; identifier = the `dc:identifier`
  whose `id` equals `package@unique-identifier`, else the first;
  manifest id -> href resolved against the OPF directory, `%20`
  decoded; spine idrefs -> paths) and the four getters.
- [ ] **Step 4: Run** `./compile_tests.sh probatio_codex` — PASS.
- [ ] **Step 5: Plants**: identifier = first `dc:identifier` regardless
  of `unique-identifier` (needs a fixture with two identifiers - add a
  second `dc:identifier` BEFORE the unique one in `minimus`'s OPF);
  hrefs not resolved against the OPF directory (red: OPF in `OEBPS/`).

---

### Task 2: contents -> chapter table

**Files:** `lib/codex.c`, tests section `capita`.

- [ ] **Step 1: Failing tests**: `minimus`: `codex_numerus_capitum ==
  IV`: 0 front matter (titulus empty), 1 "Caput I", 2 "Caput II", 3
  "Caput III"; `vetus` (NCX, nested): the same four in the same order.
- [ ] **Step 2: Implement**: nav = manifest item with `properties`
  containing `nav`, its `nav` whose `epub:type` is `toc`, every `a`
  under it in document order (nested lists flattened); else NCX = spine
  `toc` attribute -> manifest item -> `navMap` navPoints in document
  order (`navLabel/text`, `content@src`). Each entry -> (spine index,
  fragment or empty); entries not in the spine skipped; duplicates
  (same spine index + fragment) skipped; front matter chapter only if
  the first entry is not at the very start of the first spine document
  with text.
  Also `sine_prooemio`: `codex_numerus_capitum == III` (no empty front
  matter chapter).
- [ ] **Step 3: Run** — PASS. **Step 4: Plants**: nested NCX points
  dropped (red: `vetus` has fewer than IV); front matter always added
  (red: `sine_prooemio` has IV).

---

### Task 3: chapter text and blocks

**Files:** `lib/codex.c`, tests section `textus`.

- [ ] **Step 1: Failing tests** (`codex_caput_legere` on `minimus`):
  - chapter 1 text begins with its heading `"Caput I\nDe Cranfordia"`
    (the `<br/>` kept as `'\n'`), then `"\n\n"`, then the first
    paragraph with hard wraps collapsed to single spaces; `&amp;`
    decoded to `&`; `<i>` text present without markup; chapter 1 ends
    BEFORE `cap2`'s content; chapter 2 starts with its own heading.
  - blocks: genus TITULUS for the heading, PARAGRAPHUS for `p`, VERSUS
    for the poem block whose text is `"line one\nline two"`; block
    offsets slice exactly the block texts out of `textus`.
  - `wrap.xhtml` lies between `c1` and `c2` in the spine, so it is in
    chapter 2's span: chapter 2's text contains no `"back"`.
  - `fractus`: `codex_caput_legere` of chapter 3 -> NIHIL + vitium
    (the book itself still opens).
- [ ] **Step 2: Implement** (RULING at execution, Fran: chapters via
  `html_lexema`, not `xml_legere` - xml.c drops a text node's leading
  space and fails on `alt=""`; header comment and the `fractus` fixture
  changed accordingly: an unclosed `<p>` is tolerated, NIHIL only for a
  missing member): walk each spine document of the chapter's
  span with `xml_legere`; start emitting at the start anchor (element
  with `id` = fragment), stop at the next chapter's anchor; block
  elements `h1-h6` -> TITULUS, `p`/`li`/`dd`/`blockquote` leaf text ->
  PARAGRAPHUS, any element whose `class` contains `poem`, `stanza` or
  `verse` -> VERSUS (its `<br/>` and child `div`/`p` = lines); skip
  `head`, `script`, `style`, `img`; a document whose only text is inside
  `<a>` is skipped (wrappers).
- [ ] **Step 3: Run** — PASS. **Step 4: Plants**: `<br/>` -> space in
  headings (red); wrapper skip off (red: "back"); end anchor ignored
  (red: chapter 1 contains chapter 2).

---

### Task 4: sentences, fixes, re-anchoring

**Files:** `lib/codex.c`, tests section `sententiae`.

- [ ] **Step 1: Failing tests** (chapter 3 of `minimus`: verse block of
  two lines + a paragraph `"Primum hic. Secundum ibi. Tertium!"`):
  - without fixes (`c2.xhtml` has no heading): sentences = `line one`,
    `line two`, `Primum hic.`, `Secundum ibi.`, `Tertium!` - each `initium`
    and `finis` slicing exactly that text out of `textus`.
  - join at `Primum hic.`'s initium -> one sentence `"Primum hic.
    Secundum ibi."` with the SAME initium; `Tertium!` keeps its initium.
  - split at the offset of `"ibi"` -> `Secundum` and `ibi.` with
    `ibi.`'s initium = that offset.
  - join at the LAST sentence (no next): unchanged; the same join twice:
    same as once; a split exactly at an existing boundary: unchanged.
  - `codex_reperire(caput, initium, "Tertium")` -> initium; with the
    vestigium shifted 3 bytes later in a modified copy of the text ->
    the new offset; a vestigium not found -> -1.
- [ ] **Step 2: Implement** `codex_sententiae` (units per block as in
  the header; paragraphs via `sententia_fissio_indices` on the block
  slice, offsets shifted by the block's `initium`; then fixes: splits
  add boundaries, joins remove the boundary after the sentence starting
  at `positio`) and `codex_reperire`.
- [ ] **Step 3: Run** — PASS. **Step 4: Plants**: paragraph offsets not
  shifted by the block start (red); join removes the boundary BEFORE
  (red); reperire without the nearby search (red).

---

### Task 5: the real book, docs, commit

- [ ] Throwaway probe on `~/.rhubarb/codices/cranford.epub`: metadata,
  chapter count (expected 20 contents entries + front matter =
  `pg-header` text -> 21), each chapter's first 60 bytes, block and
  sentence counts per chapter, no chapter contains `"back"` from a
  wrapper, chapter "CHAPTER I. OUR SOCIETY" begins with
  `"CHAPTER I.\nOUR SOCIETY"`. Record in the worklog.
- [ ] Worklog `lib/codex.worklog.md`; spec §VIII L2 status; lint
  (NOVA 0); commit on Fran's word (paths: header, lib, test, fixtures
  sources + .epub, worklog, spec, this plan, link lists).
