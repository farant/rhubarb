# md_arbor worklog

## 2026-09-03 — A2: registry, lexicon, line lexer, runner

Born green on the second compile; the reds were the compiler's, not the
tests': `MdLineae L` collides with latina.h's Roman `L` (50) and reads
as "expected identifier" at the declaration; an s32-minus-i32 comparison
trips `-Wsign-compare`. Two planted faults confirmed both gates bite:
`md_scan_atx` accepting seven `#` fails the CommonMark `####### foo`
case; dropping the INDENTATIO token from the crude stream fails the
corpus gate with every indented file named (positions drift from the
first skipped byte, so the first divergent token is reported, not the
whole file).

**Decisions made here, not in the spec:**
- The lexer is NOT a whole-stream lexer (css's shape). Markdown cannot be
  lexed without container context, so `md_lexema` is cmark's split:
  a line table, scanners that return EXTENTS over one line's content,
  and a factory that mints tokens with line/column from the table. The
  crude stream (`md_lexare_crude`) exists only for the A2 byte gate.
- Scanners decide nothing about ownership: `md_scan_marca_listae`
  returns the marker extent and the COUNT of following space columns
  (1–4 belong to the marker; 5+ means one space + indented code —
  CommonMark §5.2) and the parser splits. Same for tabs: a tab is one
  byte with one owner; partially consumed tab columns become content
  columns in the derived value only (corpus has zero tab-indented
  lines; a few CommonMark examples have them).
- A lone `\r` line ending is `LINEA_CR` (VERBATIM, munus LINEA) because
  materia's TERMINATOR species must end in `\n`. Corpus has zero CR
  files; the case is tested anyway.
- The lexicon has NO trivia genera except the terminators (munus LINEA
  is needed so the reader's cursor counts lines): INDENTATIO and
  SPATIA_FINALIA are SUBSTANTIVUM, `genus_spatii = -1`. The registry
  test asserts exactly two trivium genera.
- The corpus is git's list, produced by the runner into
  `md/build/corpus_md.txt` and read by the test; the test refuses
  loudly (CREDO_CULPA) when the list is missing. A directory walk was
  measured first: 3,110 files versus 1,122 tracked.
- `md_scan_tabula_separator` accepts `---` alone (one cell): whether it
  is a delimiter row or a setext underline is the parser's call (a table
  needs a header row above).

**Numbers:** registry 28 genera / 87 slots; lexicon 26 genera; corpus
1,122 files, 17,209,349 bytes, 600,524 crude tokens, 0.43 s.

## 2026-09-03 — A3: leaf blocks

Green on the first compile — the file was born through `scribe` (ACCIPE)
and every edit to the registry and lexer went through `Editio`, so the
only surprises were two `Editio` anchor refusals, both mine: tolerant
mode matches anchors as a C TOKEN SERIES, and an anchor that cuts through
a comment (`… },\n\n    /* capitulum`) can never match — use
`tolerans=False` for comment-bearing anchors, or `inserere_post(name)`
by function name.

**Shape decisions made here:**
- `paragraphus` gained a `finis` slot (its last line's terminator; the
  content never owns it). Registry windows after it shifted by one — the
  contiguity gate recomputed, nothing pinned yet.
- A paragraph is appended to `bloci` when it CLOSES, not when it opens,
  so a setext underline turns it into a `capitulum` without touching the
  list. The capitulum takes the paragraph's `praefixa` and `inlinea`
  values whole.
- Blank lines inside an INDENTED code block are pending until the next
  line decides: another code line pulls them into the block, anything
  else releases them to the document. Leading and trailing blanks are
  never part of the block (CommonMark §4.4).
- HTML block lines are ONE `HTML` token each, indentation included (raw
  by definition). Conditions 1–5 close on the line that carries the end
  string, 6–7 on a blank line that stays outside the block. Condition 7
  cannot interrupt a paragraph; the scanner takes that flag.
- Front matter is recognized only at line 1 with an exact `---` opener
  and an exact `---` closer somewhere below; without a closer the line is
  a thematic break. Its lines are raw `linea`/`linea-vacua` nodes.
- A closing fence's trailing whitespace goes into the limes `info` slot
  as an INFO token — byte-honest, semantically empty; `lingua` (derived,
  A7) reads only the opening limes.

**Corpus (1,124 files):** parse → `materia_scribere_nodum` → memcmp
byte-exact on all; capitula 18,520 · saepta 3,893 (65 indented) · html
310 · divisiones 3,039 · praefationes 358 (= the census). The ~1,082
"indented code" heuristic from the census was list continuations: the
ten printed sites are all genuine (an STML block in SABAW2, `gcc` lines
in knotapel READMEs, equations). A4 will re-measure: some of the 65 sit
inside list items and become continuation paragraphs.

**Planted faults:** setext ignored → four structure reds; paragraph
terminator dropped → byte-count mismatch on every fixture.
