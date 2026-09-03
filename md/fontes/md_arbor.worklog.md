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
