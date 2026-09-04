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

## 2026-09-03 — A6: the example corpus

Vendored `spec.txt` for CommonMark 0.31.2 (652 examples) and GFM 0.29
(672) under `md/probationes/fixa/`, sources and pins in `FONTES.md`.
One reader, `md_exempla`, for both: fence of 32 backticks + ` example`,
`.` separator, closing fence, `→` → tab, section = nearest `#`/`##`
heading above. cmark-gfm tags extension examples ON the fence line
(`example table`, `example strikethrough`…): the exact-match reader
found 648 of 672 and zero examples in the extension sections; matching
the fence by prefix and keeping the tag as `extensio` gave 672 with the
24 tagged ones (8 tables, 2 task lists, 2 strikethrough, 11 autolinks,
1 disallowed raw html). Pinned: byte counts, example counts, the tag
count, example 1's tab conversion.

Every example INPUT of both files goes through parse → emit → memcmp:
1,324 of 1,324 exact on the first run after the reader fix. These are
CommonMark's own adversarial cases (tabs inside markers, deep nesting,
unclosed fences, lazy continuation across quotes and lists), so this is
the first evidence that the line model's byte law survives inputs the
house corpus never produces. The corpus half of A6 already lived in
`probatio_md_arbor`'s corpus section (all 1,124 files, structure
counts interrogated, indented-code sites printed); it stays there — the
plan's `probatio_md_corpus` name is split across `probatio_md_arbor`
(house corpus) and `probatio_md_exempla` (spec examples).

Note for later: the GFM spec.txt closes its YAML front matter with
`...`, which our `praefatio` does not recognize (it wants `---`); the
file is a fixture, not corpus, so nothing depends on it — recorded as a
known limit of the front-matter scanner.

Fault: tab conversion replaced by a space → example 1 red.

Addendum (post-commit 6c2d50e0): the A6 planted fault was confirmed red
only AFTER the commit — the plant script's literal anchor missed twice
because the formatter aligns `d[n]  = '\t';` with two spaces, and the
commit went ahead on the green suite. Four reds once planted with a
regex anchor. Rule from this: a plant step must PRINT that it planted
and the run must PRINT the reds before the commit step is allowed to
run; the scratchpad script now chains them with `&&`.

## 2026-09-03 — A7: the inline tree

`md_inlinea`: after block parsing, every `inlinea` node's children (one
raw text per line, soft breaks between) are re-lexed and rebuilt in
place. The content lines are copied contiguously with `\n` between and
a segment table maps copy offsets back to source, so every token minted
lies within one line. A doubly-linked list of ITEMS (lexeme leaf,
finished node, line break) is what the delimiter-stack and bracket-
stack algorithms restructure; only at the end do consecutive leaves
merge into `textus` nodes (derived `valor` only when an escape, entity
or trailing-space item made decoding differ) and the list becomes
materia nodes. Breaks inside a LISTA_TOKEN slot (code span content,
link tail, inline html) are the break's raw tokens; inside LISTA_NODUS
(emphasis, link text) they stay break nodes — the old `fractura-mollis`
node is reused, or replaced by a `fractura-dura` that OWNS the trailing
spaces or backslash as its `signum`.

**Birth reds, in order:** examen refused the first draft for exactly the
things I already knew were wrong (an undefined helper macro, a stray
field) plus two sign conversions in the rule-of-three arithmetic —
`scribe` earned its keep. Then a SIGKILL after 18 s with no output: an
infinite loop in the emphasis matcher — when both delimiter runs were
consumed whole the items left the list but their extents never shrank,
so the same pair matched again forever. Extents now shrink in every
branch, decided before the list surgery. After that: 1,125 corpus files
and all 1,324 spec example inputs byte-exact with inlines on, and the
structure gate green except for my own fixture (a failed `[nope]`
merges with its neighbours into one text node — correct, and the
expectation was wrong).

**What passed first time and surprised me:** all the §6.2 emphasis cases
I pinned (flanking, intraword `_`, `***foo***`, `*foo**bar*` by the rule
of three, `**foo*bar*baz**`), links in emphasis and emphasis in links,
"links cannot contain links", reference/collapsed/shortcut forms with
case-insensitive labels, angle autolinks, GFM bare URLs with trailing
punctuation and unbalanced `)` excluded, code spans across lines with
the LINEA token inside `crudum` and the normalized `valor`.

**Named gaps kept:** Unicode punctuation/whitespace classes in flanking
(ASCII), inline link titles that span lines, labels that span lines,
~150 named entities. Fault: rule of three disabled → `*foo**bar*` red.

## 2026-09-03 — A8a: the STML projection

`md_stml_consilium`: registry, lexicon, grammar "md", and an ORIGIN
HOOK for derived tokens. Writer side, `sedes_quaerere` declares source-1
tokens non-source, so their position is carried (`linea`/`columna`; no
`b` because `byte_offset` is -1) and `f="1"` is written. Reader side,
`radix_quaerere` returns an empty, always-positioned sentinel as their
root, so the reader neither touches their carried position nor advances
the cursor — materia's own "a derived token holds no bytes" path, built
for C89 expansions, reused unchanged. No frons: markdown tokens have no
tail. Gate `probatio_md_stml`: write→read→write twice, STRUCTURALIS AND
FIDELIS (css could not run fidelity; markdown's newline has munus LINEA
so the reader's cursor reconstructs positions), emission from the read
tree equals the file, 17 inline cases + the whole corpus (1,125 files
through the projection with fidelity) + the derived-token spelling
pinned (`f="1"`, `<md-derivatum`, no `<ante>` anywhere).

**Two substrate findings, both fixed in `materia_arbor.c`:**
1. A TERMINATOR genus in a content SLOT lost its CRLF: the `crlf` flag
   was written and read only on the trivia paths; C89's newline is
   trivia so it never showed. Slot writer and slot reader now mirror the
   trivia branch. materia 6/6, css 9/9, shim 348/348 unchanged.
2. The raw text form refuses a value containing its own closing tag,
   and `project-specs/md-arbor-spec.md` contains `</md-textus>` inside
   a fence. Since every md token tag starts with `md-`, a token value
   must never contain `</md-`: `md_scissio_proxima` names the split
   point and `linea.contentum` became a LISTA_TOKEN split there; the
   inline module splits code-span, link-tail and inline-html runs the
   same way. Byte-honest, and the corpus file that documents the
   projection now projects.

`md/arbor.sh <x.md>` prints the projection (instrument
`md/instrumenta/arbor.c`, links `md/build/*.o`).
Fault: origin hook removed → fidelity red on every derived token.
Addendum: the first two plants of the A8a fault did not run — removing
the hook made the hook table an unused constant and clang refused the
file, which my grep read as silence. A plant that keeps the reference
(`origo = (datum == NIHIL) ? NIHIL : &ORIGO_MD`) went red on six
assertions, including the FOLLOWING source token's drifted position.

## 2026-09-03 — A8b: the canon

`md/grammatica/md.canon`, hand-written from the registry, the lexicon
and the two parsers (what each slot actually receives): 143 rules —
28 genera, 87 slot rules `intra=` their genus, 26 `md-` lexeme rules,
the `<arbor>` involucrum with the seal pinned (`de590d67`). No `ante`/
`post` rules on purpose: the line model has no trivia, so a trivia
wrapper appearing anywhere is an unknown element, which is the right
verdict. INDEX slots are wrappers with text only (`textus="verum"`),
node slots are wrappers holding the genus element, derived tokens are
`md-derivatum` with `f`/`linea`/`columna`, `crlf` on `md-linea` is a
one-option choice as c89 does it. Gate `probatio_md_canon` (the css B7
gate adapted): drift guard both ways over the three tables and the
involucrum, seal pin vs `materia_arbor_sigillum` live, then every
projection judged — 19 inline cases (CRLF, `</md-textus>` inside a
code span and in text, every block and inline genus) and all 1,125
corpus documents: 0 faults, green on the first run. Faults: seal
altered → pin red; the `nudus` slot rule removed → drift guard names
`locus 'nudus' intra 'paragraphus': regulae 0`.

A note on tooling: I fed a generated file to `scribe` as its own
stdin — `cat > X < X` truncates X before reading, so the file became
empty without a word. scribe now refuses that (same-inode test on fd 0)
and the smoke test covers it.

## 2026-09-03 — A8c: totality and the computus twin

**Totality found a real defect on its first run**, as it did for css at
birth: quotes or lists nested past the container cap (64) made
`md_arbor_parsare` return NIHIL — `_aperire` refused and the refusal
propagated — although the spec and orientation both promised "beyond
the cap a marker is text". The two container-open sites now fall
through at the cap (a list needs two stack slots, list + item), so the
extra markers become paragraph text and the byte law holds. Generators
as css: random bytes (32 seeds), mutated fixtures (1 in 40 bytes), 23
truncation steps, six markdown nesting forms (quotes, lists, brackets,
asterisks, backticks, mixed) at depths 1/10/100/1000 closed and open,
then 50,000 quotes and 50,000 brackets asserted NON RUIT — md has no
known crash to pin, unlike css's 01M1FAD8. Coverage floor 150 for four
fixtures (css's 200 assumed six). A failing source lands in
`md/build/totalitas_fractum.md`.

**Computus twin**: `md_computus_metiri` with css's columns exactly, so
phase-5 comparisons read across clients; `./md/computus.sh <x.md>
[-machina] [-iter N]`; golden `md/probationes/fixa/computus/basis.tsv`
over four SNAPSHOT fixtures (`fixa/md/`, so a living document's edit
never moves a pin), regenerated only with `COMPUTUS_SCRIBERE=1` and a
named cause (this one: birth). First readings, sabaw.md (47.6 KB):
parse 0.63 ms, emit 0.13, STML write 2.7 + read 4.0 — the same 10×
shape css showed: the projection, not the parser, is where time goes.

## 2026-09-03 — B1 begins: quid STML voluit (1) — token slots

Before a line of engine code, the first usability finding came from
printing a real projection (`./md/arbor.sh`) and reading it as the
html program would. Three slot kinds project three ways: INDEX slots
hold bare text (`<gradus>2</gradus>`), NODUS slots hold the genus
element inside the wrapper (`<inlinea><inlinea>…`), TOKEN slots hold
token ELEMENTS in raw form (`<crudum><md-textus!>Hello </md-textus>`).
The spec's §6.1 rule ("text-only wrapper = scalar") fits the first,
makes the second a one-element forest (which PER handles uniformly),
and leaves every token slot — including the derived ones, `valor`,
`url` — unreadable as a string: the text arm would splice a
`<md-textus>` element into html. No path or parser trick escapes it,
because a token is always an element in the projection.

Ruling with Fran (three rounds; the wrong first proposal recorded
honestly): I first proposed an implicit rule "raw leaves count as
text". Fran asked when this should be implicit vs explicit and whether
a form like `&@n.b.c#text` or a `<TEXT>` command fits the landscape.
Against the sigil registry: `#` is document-space (retired once
already from a fourth role), `#text` is a DOM embedding not a meaning;
`!` is the house raw sigil in the same glued suffix position, so
`&@n.crudum!;` = "bytes of" rhymes exactly. Decision: paths yield
CONTENT (never the wrapper; same-tag selection is a pattern's job);
text is a scalar implicitly (nothing converts); raw elements become
bytes only under `!`; `!` allowed anywhere as an assertion; structure
under `!` = vitium VII; `<TEXTUS>` (textContent) reserved, not built.
The html program will read `&@n.crudum!;` — the intent is on the page.
Known edge: terminators are written as empty elements, so `!` on a
`finis` slot yields ""; the program never projects one (breaks have
arms). Recorded in spec §6.1 + §13 i, visio §2 row + §6 reservation.

## 2026-09-03 — B1.1 built: slot projections — two more findings

**(2) Paths are literal child steps.** My first implementation put a
hidden hop between segments (segment = child of the unique element
root of the previous wrapper's content), which made the spec's
`&@n.inlinea.liberi;` read nicely on materia's double-wrapped NODUS
slots but broke the plain case I had promised Fran: in
`<a><b><c>text</c></b></a>`, `&@n.b.c;` came back PROIECTIO_ABSENS.
The fixture caught it at birth. Rule now: every segment names a child
element of the element the previous segment reached; the argument's
forest must have one element root for the first step. Materia's
honest spelling is `&@n.inlinea.inlinea.liberi;` — the wrapper is the
slot, the inner element is the node — and the html program never
needs it, because PER over `@n.inlinea` hands the genus element to
the next fill where `&@n.liberi;` is one step. Spec §6.1 corrected.

**(3) `est=""` cannot exist.** I wanted `<CASUS est="">` to tell an
empty wrapper from a filled one. The base grammar reads an empty
attribute value as a TOMBSTONE (attribute present, value NIHIL — the
same explicit-absence reading `<@attr=>` elements have) and writes it
back bare, so COMMUTATIO's collection judged the arm malformed. Not
a defect, a decree I had forgotten. Consequence for the projection
rule: an empty wrapper is a PRESENT empty scalar (`non-nihil`
matches it); absence in materia is a MISSING wrapper (loud XXVII),
and the arm that discriminates it is the pattern arm of B1.4 —
exactly the spec's html-textus shape. Also learned the hard way:
`structura` is `struct` (latina.h) — a variable named that fails to
parse, and Editio then reports every later function as "not found in
this file" because the whole in-memory parse died. `-fsyntax-only`
on the dumped text named it in one line.

Vitia XXVI–XXVIII; fixtures in `probatio_stml_exemplaria.c` (274
assertions); planted fault = the implicit raw rule (raw leaves read as
bytes without `!`): the `<q>` expectation goes red.

## 2026-09-03 — B1.3 built: PER over a projected forest

`<PER de="@n.elementa" voca="#@item"/>` and the `ut="e"` body form
both work inside template bodies; the nested case (a PER inside the
delegated template's body over the row's own forest) produces
`<ul><li><i>a</i><i>b</i></li></ul>` in one pass, which is the shape
the md dispatcher needs minus the self-call. Decisions taken while
building, all recorded in spec §6.3 "as built":

**(4) An empty wrapper is a source with zero rows.** Piece one made an
empty wrapper an empty scalar; the first PER fixture over
`<elementa/>` therefore died as "source is a scalar" (XIII). The two
rules meet honestly: an empty scalar as a PER source = no rows;
a scalar WITH text is still refused. Every empty list slot in the
projection (`<praefixa/>`, empty `elementa`) needs this.

**(5) The row variable is local and cannot shadow.** Collection scans
the PER body with a temporary declared slot for `ut="e"`, so `&@e;`
outside the body is LOCULUS_IGNOTUS as any undeclared slot, and
`ut="n"` over a declared `n` is refused instead of silently winning
or losing against the outer argument (argument lookup is first-match).

**(6) Delegation is a real call.** The delegated template gets a fresh
table holding only the row in its sole required slot — the caller's
other arguments do not leak in, the same as `<<#@f>>` — and optional
slots may exist unfilled. A template with zero or several required
slots refuses as XXIX PER_DELEGATIO_AMBIGUA, naming the template.
Text and comment nodes in the forest are not rows (the md projection
never has them in list slots; hand-written STML may).

Gate: five fixtures + nine vitia (305 assertions); planted fault
(break after the first row) red on the delegation fixture, green on
revert. Still refused inside bodies: EXEMPLAR/CATENA/DIRIBITIO with
`de="@n"` — that is B1.2; the dispatcher's self-call is B1.5.

## 2026-09-03 — B1.4 built: pattern and tag arms in COMMUTATIO

Green on the first run, which after B1.1 and B1.3 says the seams were
already cut: `_exemplar_petere` anchored at the scrutinee's root was
the whole matcher, `_ligamina_ad_argumenta` the whole bridge, and
`_bracchii_est_forma` (shared by collection and fill) the only new
reading. Findings, spec §6.4 "as built":

**(7) Patterns are parameterized for free.** Filling the pattern with
the current arguments before matching (`_expandere_nodum` on the
EXEMPLAR body) makes `<p id="&@k;"/>` a pattern that depends on a
slot — the relational plane's bridge idiom, in the fill plane, with
no new syntax. The dispatcher will not need it, but a template that
dispatches on "the node whose id is my argument" reads naturally.

**(8) Captures sit beside the outer scope.** The arm's table is the
outer arguments plus the captures, so `<h g="&@g;">&@q.t;</h>` reads
a capture and projects into a captured node in one line, and the
outer `&@n;` stays visible. Collision is loud twice: a capture naming
a declared slot at collection, a repeated name at fill (XXII).

**(9) Scalar in tree position = XXX, the mirror of VII.** A tag or
pattern arm against a scalar with text refuses; the PER over a scalar
source of B1.3 moved to the same vitium. The empty scalar (empty
wrapper) has no root and matches nothing — the two rules of B1.1 and
B1.3 hold here without a third.

Gate: three fixtures (tag dispatch; five pattern arms with value,
node, text captures and `<**>` descent; parameterized pattern and
absent optional) + nine vitia, 331 assertions; planted fault (tag arm
always true) red on the dispatch fixture, green on revert.
