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

## 2026-09-03 — A4: containers

CommonMark's appendix-A strategy in the line model: a stack of open
containers (document, citatio, lista, elementum); per line, match the
open containers in order, then look for new container starts on the
remainder, then hand the rest to the leaf. Every marker consumed on the
way becomes a PREFIX TOKEN of the leaf that owns the line
(`MdPraefixa`, a per-line list of (genus, ab, ad)), so containers own no
bytes and the byte gate never noticed the difference: all 1,124 files
stayed exact through the rewrite.

**Three bugs at birth, all in the algorithm's seams, all found by the
fixtures before the corpus was consulted:**
1. A LISTA container was allowed to receive a non-item block. After an
   unmatched item was popped the list stayed on top and the next
   paragraph landed in `elementa`; the pending-blank flush then treated
   that paragraph as "the last item" and appended into its `nudus`
   INDEX slot → refusal → `parsura NIHIL` on every file with a list
   followed by text. Fix: `_claudere_pro_bloco` closes unmatched
   containers AND any list left on top before a non-item block.
2. The paragraph-interruption rule for list markers (non-empty; ordered
   must start at 1) applies only when a NEW list would start, never to a
   sibling item — `4) b` after `3) a` had become lazy continuation.
3. An item whose marker line is blank (`-` + EOL) must own that line: its
   prefix carries the marker. Pending it and flushing to the parent later
   put the marker's line outside the item.

**Design points to keep:**
- Blank lines are PENDING until the next non-blank line names the
  container: deepest matched container, with `lista → last item` (a blank
  between items belongs to the earlier item and makes the list loose),
  indented code if it continues, the document at EOF. This is what makes
  trailing blanks after a list land in the parent, not the last item.
- `laxa` is computed at list close (item other than the last ending in a
  blank; or two blocks of one item separated by a blank); `nudus` is set
  then for direct-child paragraphs of tight items and to 0 for every
  remaining paragraph at the end — INDEX slots are write-once, so
  paragraphs no longer set `nudus` at creation.
- The item content offset is stored RELATIVE to the parent's consumption
  point (cmark's `marker_offset + padding`), matched by consuming that
  many columns of whitespace; a tab is one byte with one owner, so a
  partially consumed tab's overshoot is lost to the remainder's column
  count (documented; corpus has zero tab-indented lines).
- Setext applies only when every container matched; a `---` under a
  quoted paragraph without `>` is a thematic break outside the quote.
- Task boxes: `md_scan_officium` on the first paragraph of an item opened
  on the same line; the box plus one space is a MARCA_OFFICII prefix;
  `officium` is written once at end of line (0 when absent).
- Depth cap 64 containers per line; beyond it a marker is text.

**Numbers:** listae 10,435 · elementa 46,707 · citationes 160 · indented
code 65 → 60 (five sites were list continuations). Faults: laxitas never
set → 3 reds; lazy continuation removed → reds. The full CommonMark
container example inputs run at A6 with the spec.txt reader; A4's
coverage is the fixture set (interruption, empty items, nesting, quotes
with lists, fences closing with their quote).

## 2026-09-03 — A5: tables (A5a) and link reference definitions (A5b)

Both are "a paragraph turns out to be something else at a later line".
Materia's list value is a VIEW `{xar, mensura}` over an append-only
store, so a paragraph's inline list can be shortened by assigning its
`mensura` — that is how the last line of a paragraph becomes a table
header (16 files in the corpus have a header directly under paragraph
text). Dropping leading lines is not a view operation, so definitions
rebuild the remainder as a NEW paragraph node whose children are the
old nodes (moved, not cloned; the orphaned node gets `nudus` 0
harmlessly at the end).

**Tables.** `md_scan_ordo` splits a row at unescaped pipes; the first
and last segments are cells only if non-blank, interior ones always;
every byte belongs to a cell's PIPA token (trailing whitespace of the
previous content + `|` + leading whitespace) or to the row's `clausum`.
Rows shorter than the header are padded with byte-less cells so the
html transform never has to count. A delimiter row under a paragraph
makes a table only if a pipe appears somewhere (delimiter or header);
otherwise setext and thematic break keep precedence. Tables close on a
blank line, a block start, or an unmatched container. 1,367 tables in
the corpus.

**Definitions.** Extracted at paragraph CLOSE from the leading lines:
forms `[t]: d "s"`, `[t]:` + destination on the next line, destination
+ title on the next line; a title must close on its own line and a
label may not span lines (named gaps; rare). The label is normalized
(trim, whitespace collapsed, ASCII lowercase — Unicode case folding is
a named gap) into the parser's definition table, first wins. Each
definition carries THREE derived tokens (source 1, `byte_offset` -1,
position portable from the origin token): `titulus`, `url`,
`descriptio`, decoded by `md_decoquere` (backslash escapes + numeric
entities + ~150 named entities; the full HTML5 table of 2,231 is a
named gap for B3's entity section). The emitter with
`consilium.fons_index = 0` omits them — the corpus stays byte-exact —
and the gate also proves the mechanism the other way: emitting ALL
sources yields more bytes than the file. Corpus: 0 definitions, as the
census said.

**Birth reds:** `nomen` as a struct member in the entity table (latina.h
`typedef`) — examen REICE through `scribe`, fixed before a single
compile; an `Editio` anchor on the state struct failed literal matching
after the formatter realigned it (tolerant mode fixed it). Planted:
storing the raw destination instead of the decoded one → red; minting
derived tokens as source 0 → the omission proof red.
