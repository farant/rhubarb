# codex worklog

## 2026-10-24 - L2: an EPUB book as chapters, blocks, sentences

Spec project-specs/lector-spec.md (§VI calls 1, 9-14), plan
project-specs/lector-l2-plan.md. Container layer = sarcina (L1).

**Opening.** `META-INF/container.xml` -> rootfile full-path -> OPF.
Manifest hrefs are resolved against the OPF's directory (`%XX` decoded,
`#fragment` stripped); nav/NCX hrefs against THEIR document's directory.
Identity = the `dc:identifier` whose `id` equals the package's
`unique-identifier` (minimus has a decoy first identifier), else the
first one. Gutenberg: `http://www.gutenberg.org/394`.

**Chapter table.** One chapter per contents entry, mapped to (spine
index, fragment). EPUB 3 nav first (the `<nav>` whose epub:type contains
"toc", all its `<a>` in document order), else the EPUB 2 NCX (navPoints
in PREORDER, so nested points are chapters too). Entries outside the
spine and duplicates (same spine index + fragment) are dropped. No
contents at all -> one chapter titled with the book title. Front matter
(chapter 0, empty title) only when there is visible text before the
first entry; text that sits only inside `<a>` does not count
(illustration wrapper pages: an `<img>` + a "back" link).
Trap: `xml_textus_internus` returns DIRECT child text only - NCX labels
(`<navLabel><text>`) came back empty; `titulum_legere` collects all
descendant text.

**Why chapters are NOT read with xml.h** (Fran agreed 2026-10-23). xml.c
is data-oriented: `_legere_contentum` skips whitespace before every
child, so a text node loses its leading space (`<i>tenetur</i> &amp;
regitur` -> "tenetur& regitur") and whitespace-only nodes vanish
(`<i>a</i> <i>b</i>` -> "ab"); an empty attribute (`alt=""`) fails the
parse; `&#8212;` stays raw. Ledger quaestio 01M5P06KJ8VZ, wish
01M299JPS0X (xml as a materia client). Chapters go through
`html_lexema` (total, byte-faithful); references decode through
`entitates_html_decoquere`. xml.h stays for container/OPF/nav/NCX.

**Chapter walker** = a token stream with a stack of open elements
(`Gradus`: inline / block / omitted). Text after the start anchor goes
into the current line; `<br/>` ends a line in headings and verse, is a
space in paragraphs; block elements end the block (inside a heading or
verse they only end a line); an element whose class contains
poem/stanza/verse always makes its own block (stanzas stay separate);
head/script/style/svg/title are skipped. An end tag closes back to its
matching open element, an unmatched end tag is ignored, so an unclosed
`<p>` is harmless. The end anchor (next chapter's fragment) stops the
walk. A start anchor that does not exist -> the whole document.
Wrapper documents (no visible text outside `<a>`) are skipped; the same
token-based check decides front matter.

**Text contract** (header): blocks joined by exactly "\n\n", lines by
"\n", whitespace runs collapsed, lines trimmed, inline markup = its text
only. Offsets are bytes into this text; it must never change when
styling arrives later.

**Sentences.** Heading = one sentence, verse = one per line, paragraph =
`sententia_fissio_indices` shifted to the block start, edges trimmed.
Fixes are data: ALL splits first (only strictly inside a sentence; the
new part starts at the first non-space from the split point), then ALL
joins, marked against the post-split list and then merged - so the
order of the fix list does not matter and chained joins work. A join
never crosses blocks (last sentence of a paragraph or stanza: no-op).
The "splitter returned nothing" fallback was dropped: a probe shows
sententia_fissio returns >= 1 sentence for any non-empty text ("* * *",
"...", "—", ".").
`codex_reperire`: exact fingerprint at the stored offset wins, else the
nearest match within M bytes (forward first on ties), else -1.

**Fixtures** (probationes/fixa/codex/, sources + .epub made with
`zip -X0 x.epub mimetype && zip -Xr x.epub META-INF OEBPS`): minimus
(EPUB 3: two chapters in one file, wrapper page, poem, decoy
identifier), vetus (EPUB 2, nested NCX, two stanzas, `&#8212;`,
`<i>primus</i> <i>ab</i>`), sine_prooemio, fractus (unclosed `<p>`,
anchor `#nusquam`, a chapter in a missing member), sine_opf.

**Cranford (Gutenberg #394 epub3.images), throwaway probe 2026-10-24:**
21 chapters (front matter = Gutenberg header 840 B, CRANFORD, CONTENTS,
LIST OF ILLUSTRATIONS, CHAPTER I-XVI, licence); 802 blocks (22 headings,
777 paragraphs, 3 verse = the 3 `p.poem`); 2834 sentences, every
non-space byte covered; no "back" block; CHAPTER I text starts
"CHAPTER I.\nOUR SOCIETY"; dialogue splits well; 27 sentences > 600 B
(Gaskell's long periods - sentence view layout must wrap them); whole
book parsed + split in 0.35 s.

Plants (all red): nested NCX dropped; front matter always; OPF dir
ignored; `<br/>` = space in headings; wrappers kept; end anchor
ignored; stanzas merged; references raw; text trimmed; missing start
anchor -> nothing; paragraph offsets unshifted; join marks the boundary
before; no nearby search; joins across blocks; split part untrimmed;
joins before splits; verse as prose.
