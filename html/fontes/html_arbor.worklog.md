# html_arbor — worklog

## 2026-09-15 — H6: the substrate limit the plan predicted, found in its true shape

The plan named one place this client could pay a substrate change: a
text token containing the literal closing tag of its own lexeme
element, which the STML raw form cannot carry. It fired, but not where
I expected.

- `<p>textus cum </lex-textus> intra se</p>` passes. The HTML lexer
  reads `</lex-textus>` as an END TAG, so the string never sits inside
  one `textus` token; it becomes an `elementum-malum` and round-trips.
  Bytes and tree both identical.
- `<script>var t = '</lex-textus-crudus>';</script>` is REFUSED by
  `materia_arbor_scribere_nodum`: the raw-text token is one token by
  the lexer's contract, its value contains `</lex-textus-crudus>`, and
  `_valorem_crudum_notare` refuses ("valor sequentiam claudentem
  fert"). Same for `<!-- </lex-commentarium> -->` inside a comment
  token.

Why materia insists on raw: values are "octeti codicis, non prosa" —
a pretty-printer re-fills plain text, so code bytes must be raw. When
raw is impossible the writer refuses the whole document rather than
falling back to escaped text. md met the same wall and split its token
values at `</md-` by hand; html cannot split a token (one owner per
byte, and the lexer's raw-text token is one token).

Pinned in `probatio_html_stml` as the CURRENT behaviour (the refusal
and its exact cause asserted), so that a materia fallback flips the
pin red and the two cases move into the round-trip list. Filed as a
materia question in the ledger; Fran's decision. Reachable only by
adversarial input (a script or comment containing the projection's own
closing tag), never by an ordinary page — the fourteen-file corpus
passed both cycles and the comparator.

**Measured on the side, worth keeping:** the STML projection of the
corpus is 15× the source (162,775 → 2,384,563 bytes). The 200-deep
nesting fixture is 287× (3,475 → 996,225): pretty indentation grows
with depth, so a deep tree's projection is quadratic in depth. H9's
computus will pin the counts; this is the number oratio's "projection
cost" conversation was about, seen from a different client.

## 2026-09-15 — H8: the totality gate found a byte-order bug at birth

The css precedent held: a gate over random, mutated and truncated
input found what fourteen clean pages could not. One mutation of the
reference page replaced the `>` after `</h3` with a tab. The lexer
stays in tag mode after an end tag, so it emitted attribute tokens
(`�`, `<div`, `class`, `=`, `"sl>der-group"`) and, much later, a real
`>`. The builder had recorded the pending close of `h3`; when that `>`
arrived it became `h3`'s `tok_clausura_finis`, which emits BEFORE the
garbage tokens that sit as siblings after `h3`. Bytes reordered:
`</h3>` followed by the tab where the source has `</h3` then the tab.

Fix: attribute-class tokens with no open start tag CLOSE the pending
clausura (its finis stays absent, truncation expressed) and go into
one pending `elementum-malum` that keeps collecting until the `>`,
which then belongs to the malum. `<h3>a</h3\t x=y>z` → h3 closed
without finis, one malum of four tokens, then text. Regression case in
`probatio_html_arbor`; the mutated page passes; 919 assertions green.

Lesson, same as css's: a pending state that survives a token it does
not own is a byte-order bug waiting for a mutation to find it. Every
pending state must be closed by the first token that is not its own.

**Depth, measured (01M1FAD8), each run its own process:**

| what | depth | result |
|---|---|---|
| parse alone (iterative builder) | 100,000 open and closed | survives |
| parse + `materia_scribere_nodum` | 40,000 | survives, bytes identical, 0.01 s |
| parse + `materia_scribere_nodum` | 50,000 | SIGSEGV — the substrate's recursion |
| parse + STML write | 2,000 | 56 MB of STML in 5 s |
| parse + STML write | 5,000 | 351 MB in 85 s, 859 MB resident |

So the client's half of the depth question is answered: the builder
never touches the C stack. The emitter dies between 40k and 50k, and
the projection is not a crash but a quadratic cost (pretty indentation
grows with depth). Pinned in `probatio_html_totalitas`: parse 100k
NON_RUIT, emit 40k NON_RUIT, emit 50k RUIT_CUM SIGSEGV (flips red when
the substrate is fixed). The STML cost is recorded, not pinned.

Two probe lessons: `$?` after a pipe is the pipe's; and zsh does not
word-split `$spec` in `set -- $spec` — every run got one mangled
argument and reported the same crash. Measure, then read the measure.

## 2026-09-15 — H10: md's HTML through the html parser, one finding

Seven markdown inputs rendered by `md_html_reddere`, then parsed,
emitted byte-identical, projected, re-read, compared and judged by
`html.canon`: every step held for all seven. The bad-element count was
zero for six and ONE for `sabaw.md`.

The one is sabaw's notation, passed through by md as an HTML block:
`<if⟨condition⟩>` … `</if⟨⟩>`. The condition lives inside the tag
name, so the closing name differs from the opening one and no open
element matches — exactly what `elementum-malum` records. Pinned at
one for that input. Two things it says about the neighbours:

- md accepts non-ASCII bytes in an HTML-block tag name (the html lexer
  does too: bytes ≥ 0x80 are name characters). CommonMark's HTML-block
  rule wants an ASCII tag name, so per spec that block is a paragraph
  and the angle brackets should be escaped. A polish item for md's
  oracle list, not a defect this client can judge.
- the rendered page nests 363 levels deep in the projection: sabaw's
  `<wright>`, `<slot>`, `<random>` examples open elements that the
  prose never closes, and the simple builder does not invent closes
  for unknown names. Correct, and a reminder that a projection's depth
  is the source's, not the parser's.

## 2026-09-15 — H11: the five questions of spec §11.10, answered

1. **Absent slots as meaning at scale.** Every void, implied close,
   unclosed-at-EOF and truncated tag is an absent typed slot; the
   emitter skips it and the reader accepts it. Fourteen pages, 919
   totality cases, 39 STML round trips: zero cost, zero surprise. H4's
   law holds without a line of substrate support.
2. **A content-dominant tree with almost no trivia.** The ligator runs
   only inside tags; between tags every byte is a text node. The
   whitespace-only VERBATIM values that T9 made projectable appear
   thousands of times per page and round-trip. The one place trivia
   mattered was the H8 bug, and that was the builder's pending state,
   not the substrate.
3. **Raw text through the STML raw form.** Held on every real page. The
   named limit is real: a value containing its own lexeme element's
   closing tag is refused rather than escaped (01M2KJ3PK2). Reachable
   only adversarially; pinned; the substrate's decision.
4. **An iterative builder under recursive walkers.** Parse survives
   100,000 levels; the byte emitter dies at 50,000; the STML projection
   is quadratic in depth (2,000 deep = 56 MB). The depth question is
   now the substrate's alone, with numbers (01M1FAD8).
5. **A consumer of a consumer.** md's rendering parsed by html, whole
   chain, seven inputs; one finding that belongs to neither parser.

**Substrate changes this client forced: none.** css forced four,
oratio five. html found one limit and measured one, and both are
recorded where the substrate can act on them. The client shape held
exactly: declaration → generated registry, hand enums, lexicon,
adaptare + ligator, builder, canon, computus, pythonica face.

**What its own gates found in it:** the pending-close byte-order bug
(totality, H8) and nothing else in eleven commits. Two of the ten gates
were born red for real (totality, md); the other eight by plants.

## 2026-09-15 — O1: the html5lib corpus vendored, and a grep that lied

The tree-construction suite was already on this machine: Go's html
package vendors it under `golang.org/x/net/html/testdata/webkit/`
(WebKit BSD, x/net v0.30.0, October 2024). Fran chose the local copy
over an upstream fetch: 54 `.dat` files, 428,448 bytes, now under
`probationes/fixa/html/html5lib/` with the README as the licence.

The first tally said 1,625 cases. The true count is 1,708. Three
files (`domjs-unsafe`, `plain-text-unsafe`,
`pending-spec-changes-plain-text-unsafe`) carry NUL bytes; `grep -c`
without `-a` treats them as binary and prints NOTHING for them, and a
shell loop summing the counts read nothing as zero. Eighty-three cases
vanished without an error. Same family as "N/N clean may mean never
appeared": a tool that skips silently lies by omission. The gate pins
those three files by name.

The reader (`html_exempla.c`) mirrors html5lib's own `support.py`
TestData rather than a guess at the format: a heading is a line whose
STRIPPED form starts with `#` (so an indented `#script-on` is a
heading), each section loses one trailing newline, and the last
section before the next `#data` loses one more (the blank separator).
Two consequences O2 must know: an input that IS a blank line is empty
(tests1 #62), and an expected-tree text node containing a newline is
printed across two lines (`|       "` then `foo"`, tests3 #7), so the
serializer must not assume one node per line.

My expectations were wrong three times before the reader was: a bare
suffix match on `plain-text-unsafe.dat` also caught the
`pending-spec-changes-` file (fixed with a leading `/`), and twice I
recorded the line of the content rather than the line of the `#data`
heading above it. The reader passed first time.

## 2026-09-15 — O2: the oracle's first number, 887 of 1,700

`probatio_html_oraculum` parses every html5lib case, writes our tree
through the cooked view (`html_coctum`) into html5lib's `| ` format,
unwraps the synthesized html/head/body from the expected tree when
the input never names them as start tags, and compares bytes. First
honest number: 887/1,700 (52 %); fragments 77/184. Eight `#script-on`
cases are skipped (the builder is scripting-off). Pinned rising at
887, in 0.19 s.

Two of my own bugs came out before pinning, both oracle-side: the
`svg`/`math` element itself prints in its own namespace (`<svg svg>`),
not only its descendants; and NUL is cooked (U+FFFD in raw text,
foreign content, attributes and comments; dropped in ordinary body
text, which is what "in body" does with it). 778 → 887.

The unwrap rule held on inspection: `<b><table><td><i></table>`
expects `b > table > tbody > tr > td > i` after unwrapping; ours is
`b > table > td > i`. What is missing is exactly the `tbody`/`tr` that
HTML5 invents, never the wrapper.

The 813 failures by the shape of the first differing line:

| class | cases | what it is |
|---|---|---|
| element differs | 242 | HTML5 synthesis the simple builder never does: `tbody`/`tr` in tables, adoption agency reopening formatting elements, `</p>` creating an empty `p` |
| text differs | 127 | script-data escaping states (`<!--<script>…</script>` inside a script), legacy entities without `;` (`&gt`, `&notit;`), NUL in modes other than body |
| same node, other depth | 118 | implied closes the builder's tables lack: `<head>` closed by `<body>` (tests1 #13), p-closers `dir listing summary center dialog search xmp plaintext` (blocks.dat #15/#33/#45) |
| foreign | 81 | SVG/MathML attribute adjustments (`viewBox`, `definitionURL`), HTML elements breaking out of foreign content, CDATA at integration points |
| other | 68 | mixed |
| entities | 28 | md's 57-entry named table against HTML's 2,231 |
| comments | 17 | `--!>` closing, bogus comments cut at EOF |
| attributes | 13 | legacy entities in values |
| (unclassified rest) | 119 | printed only with `ORACULUM_OMNIA=1` |

What the table says: the cheapest rises are BUILDER TABLE entries
(`head` closed by `body`; the full p-closing list) and LEXER states
(`--!>`, script escaping), each a named change that moves the pin.
The expensive ones are the mechanisms spec §6.1 deferred by H1
(table synthesis, adoption agency, foreign breakout): those are the
distance the oracle was built to measure, and 887 is that distance's
first mark.

## 2026-09-15 — O2b-1: the tables refilled from the oracle, 887 → 930

Ranking the depth-class failures by the node at the wrong depth gave
the list directly: `p` 15, `div` 11, `rt` 10, `plaintext` 6, `nobr`
6, `button` 5, `rp` 5, `li` 5, `rtc`/`rb` 4, `dd` 3, `listing`,
`summary`, `h2`. Every one is a missing row in `CLAUSURAE_IMPLICITAE`,
not a missing mechanism. The table gained one flag (`nisi`: the list
names the exceptions, everything else closes — for `head` and
`colgroup`, whose HTML5 rule is "any start tag not allowed here") and
one convention (`claudentia` NIHIL = closed by its own name — `a`,
`nobr`, `button`, `select`). Still a table, no program (M7).

Forty-three cases rose. The remaining depth failures are the real
mechanism: `<a>1<p>2</a>3</p>` wants the adoption agency to reopen `a`
inside `p`, and `<p><b>x<div>` wants button scope, which O2b-5 will
ask about before changing the top-of-stack law.

## 2026-09-15 — O2b-2/3: `--!>` was mine, script escaping the lexer's; 930 → 985

The comment failures were not the lexer's: it already closes a
comment on `--!>` (the "comment end bang" state) and my cooked view
only stripped `-->`. One line.

Script-data escaping was real. HTML5 keeps three states inside a
`<script>`: after `<!--` the content is "escaped"; there a `<script`
start opens "double escaped", inside which `</script>` is TEXT and
only `-->` returns; outside the double state `</script>` closes. Real
pages carry `<!--` in scripts (the old browser-hiding idiom), so
without this the lexer cut such scripts short — and the byte gate
could not see it, since both halves still concatenate to the source.
Implemented in `lib/html_lexema.c` as `_crudum_consumere`, a
three-valued flag over the existing raw-text scan; style, title and
textarea untouched. Seven new lexer cases in the root suite. The
raw-text token stays ONE token, so nothing downstream changed shape:
the byte gate, the STML gate and the totality gate never noticed.

Fifty-five cases rose, all from `scriptdata01`, `domjs-unsafe`,
`comments01`, `tests16`.

## 2026-09-15 — O2b-4: the entity table, shared with md; 985 → 1,047

HTML's 2,231 named references live in Python's standard library
(`html.entities.html5`, WHATWG's own list), so the table is
GENERATED offline by `tools/entitates_html_generare.sh` into
`lib/entitates_html_tabula.c`, byte-sorted for a binary search, with
`-probare` as the freshness check. The hand-written half
(`lib/entitates_html.c`) carries the HTML5 decoder: numeric
references with or without `;`, the C1 remap to Windows-1252, the
longest-match rule that turns `&notit;` into `¬it;`, and the
attribute exception that leaves `&ampx` alone in a value. md keeps
CommonMark's stricter rule (a name is only an entity with its `;`)
but looks it up in the same table; its 57-entry table is gone.

Two things bit on the way. A generated `.c` with no header of its own
is invisible to aedilis' include closure, so the root suite linked
without it: the table got `entitates_html_tabula.h`. And
`silva.Editio` on a `.sh` file applied the C formatter's width rule
and broke the md runner's `if … || …; then` lines — filed. Also a
latent md overflow: `md_decoquere` allocated input + 4 bytes on the
theory that a decoded entity never grows, but `&nGt;` is 5 bytes in
and 6 out (two code points); the buffer is now twice the input.

Sixty-two html cases rose (entities01/02, tests with `&nbsp` and
friends in text and attributes). md rose by two: the polish list had
called the table "cheapest", and it was, but its gain was small.

## 2026-09-15 — O2b-5: scope, and why H1 chose the top of the stack

HTML5 closes a `p` that is anywhere in "button scope" when a block
start tag arrives, closes the nearest `li` (or `dd`/`dt`) unless a
"special" element other than `address`, `div` or `p` lies between,
and closes a `button` in scope. H1 chose top-of-stack only, and the
first cut of this change showed exactly why: I wrote the scope query
as a stack walk per start tag, and the totality gate's 100,000-deep
`<div>` chain — every `<div>` is a p-closer — became quadratic. The
gate ran 489 s and reported its depth pins as crashes (the forked
probes were killed). The byte oracle and the html5lib oracle were
both green the whole time; only the depth pin saw it. That is the
totality gate paying for itself a second time.

Second cut: every stack frame records at push time the index of the
nearest VISIBLE `p`, `li`, `dd|dt` and `button` below it (a scope
boundary or a special element writes −1), computed from the frame
below in O(1). A query reads the top frame. Totality is back to
1.3 s, the tables are still tables (two more: `LIMITES_SCOPI`,
`SPECIALIA`), and the arbor gate's "vertex solus" case is inverted
with its cause. Seven cases rose. The depth class is now mostly the
adoption agency and table synthesis, which are not closes at all.

## 2026-09-15 — O2b-6: foreign content in two halves; 1,054 → 1,086

Cooked half first: SVG's attribute case table (58 names), MathML's
`definitionURL`, and the rule that `mglyph`/`malignmark` stay MathML
inside a text integration point — which needs the PARENT's own
namespace, not only the children's context, so the walk now carries
both. 1,054 → 1,073. The namespace logic left the cooked view for
`html_alienum`, a module shared with the builder, with its own gate.

Builder half: each stack frame now knows its element's own namespace
and its children's (computed at push from the frame below, like the
scope indices), and two HTML5 rules follow: a self-closing tag inside
SVG or MathML closes its element (`<path/><circle/>` are siblings; in
HTML `/>` still never closes, the H4 law), and an HTML start tag from
the breakout list pops foreign elements up to the nearest element
whose children are HTML. 1,073 → 1,086; fragments 77 → 82.

One bug of mine cost a segfault: the module compares names
case-insensitively by lowercasing the INPUT, so a literal with a
capital (`"foreignObject"`) can never match. Children of
`foreignObject` became SVG, `<div/>` self-closed, and my own test
walked into a missing child. Found through the crash report in
`~/Library/Logs/DiagnosticReports` (lldb in batch mode hangs on a
forking probatio); the gate now guards its walk. Lesson: a helper
that lowercases one side needs lowercase literals — the table's
values (`foreignObject`) are output, its keys are input.

What remains foreign, named not built: the lexer treats `<title>`
and `<textarea>` as raw text everywhere, but inside SVG `title` is an
ordinary element; `<font>` breaks out only with color/face/size,
which arrive after the tag; `annotation-xml`'s encoding is likewise
unseen at push.

## 2026-09-15 — the shape of what remains, and O4; 1,086 → 1,112

Fran asked whether the rest is chunks or a tail. Measured over the
551 of 614 failures the first-diff line could be attributed: tables
97 (`tbody`/`tr` synthesis, foster parenting), foreign rest 90,
adoption agency 75, frameset modes 58, lexer raw-text set 49,
template 39, select 33, doctype and html/head/body out of place 61,
a tail of 48. Chunks, then: seven mechanisms are four fifths. Three
are cheap and self-contained (raw text, frameset, select), the
"ignored tokens" family is one mechanism across four of the chunks,
and only tables, the adoption agency and one foreign rule reach the
registry, the byte law or the lexer boundary. Fran chose to keep
pushing, cheap chunks first, with bash scripts as a candidate next
parser after html.

O4 was the lexer's: four RAWTEXT names and a `plaintext` mode (see
`lib/html_lexema.worklog.md`). Twenty-six cases rose; nothing in the
builder or the cooked view moved.

## 2026-09-15 — O5: tokens the DOM ignores; 1,112 → 1,189

HTML5 says "parse error, ignore the token" in a dozen places: a
second `<html>` or `<body>`, a `<head>` after the body began, a
doctype after any content, a `<frameset>` once the frameset-ok flag
is off, everything but frame/frameset/noframes inside a frameset,
everything but options and a few others inside a `select`, and
`</body>`/`</html>`, which never pop anything and only change mode.
The byte law has one answer for all of them: the token still owns
its bytes, so it goes into an `elementum-malum`, the genus that
already meant "bytes the DOM keeps no node for" once `</x>` without
a match lived there. The cooked view drops mala, so the html5lib
trees line up. The pending-tag machinery from H8 did the rest for
start tags: mark the tag ignored, and its attributes and `>` follow
into the same malum.

Two consequences worth knowing. `</body>` no longer closes the body
element: it stays open to EOF with an absent `tok_clausura`, its
`</body>` bytes sit in a malum child, and content after it lands
inside the body, which is what browsers do. The two real pages in
the computus golden therefore gained two nodes each. And a
`<frameset>` after an empty `<body>` still mismatches: HTML5 removes
the body element from the DOM, and a node whose bytes must stay
cannot be removed — those few cases stay red by design.

Seventy-seven cases rose: frameset, select, domjs-unsafe, tests1.

## 2026-09-15 — O6: end tags in three classes; 1,189 → 1,195

`<div><template></div>Hello` expects Hello INSIDE the template: the
`</div>` is ignored because HTML5's "any other end tag" walks the
stack and gives up at the first "special" element, and `template` is
one. So the template chunk was really an end-tag rule. First cut: I
sent every end tag without a named rule to that walk, and `</ul>`
died at `li`, because the block end tags (address … ul) have their
own rule — pop to the element if it is IN SCOPE — and I had left
them out of the named list. −23 before the split. Three classes now:
tags with their own rule (nearest open anywhere, the H3
approximation), block tags (nearest open within scope boundaries),
the rest (stop at special). Six cases rose, and the remaining
template failures are the table's: `<table><div><template>` wants
the div foster-parented before the table. That is `reinserendum`.

With this the cheap chunks are spent. Every remaining class of size
reaches something the v1 registry deliberately left out: a node
without bytes (`clonatum`, the adoption agency) or a node whose bytes
lie elsewhere than its tree position (`reinserendum`, tables). The
worklog stops here and the design conversation starts.

## 2026-09-15 — O7a: synthesis as an annotation; 1,195 → 1,299

The design turn ended with three annotations appended to the html
registry and materia untouched: `synthesis:index`, `sedes:referentia`,
`exemplar:referentia`. O7a is the first. An element HTML5 invents —
`html`, `head`, `body`, `tbody`, `tr`, `colgroup` — is an `elementum`
with every token slot absent and the `synthesis` index set. The
emitter walks through it and writes nothing (H4 keeps its word: no
synthetic tokens), the cooked view prints it, the STML projection
writes `<synthesis(> n` and reads it back, and the comparator sees
the index. The oracle's unwrap rule, which had hidden the wrappers
since O2, is gone; whole trees are compared now.

The builder gained the WHATWG wrapper modes ("before html" through
"after head") as `HtmlModus`, one function that invents what is
missing before a token that needs it, and one for table parts that
looks only at the current node. Fragments got their own entry point:
the context element is the frame UNDER the stack (`basis`), which is
why select/frameset/table contexts started passing for free. The
arbor gate's structure cases now run as a `body` fragment — they test
in-body mechanics, not wrappers — and the md gate parses md's output
as a `body` fragment too, which is what it is.

What the first run taught, in order:

- The diff of failing sets lied by one case per file: credo prints a
  dot per assertion and the oracle asserts once per file, so the
  first failure line of every file began with `..` and my `^  `
  anchor missed it. Strip the dots before matching; count the lines
  against the pin before trusting a diff.
- Content inside a `<template>` in the head was being pushed into an
  invented body (47 template cases). Inside an open template the
  wrapper modes must be silent — a per-frame `templi` index, O(1),
  never a stack walk (the O2b-5 lesson holds).
- The new "table parts outside a table are ignored" rule fired inside
  SVG (`<svg><tr>` is a foreign element) and refused `<col>` in a
  `colgroup` context; a `<td>` inside a template must be accepted
  (the spec's "in template" mode hands it to "in row"). `in_tabula` is
  set by `table` AND `template`.
- Breakout from foreign content never happens in a fragment. The spec
  calls it the fragment case; x/net's `parse.go` has `if !p.fragment`
  around the whole breakout branch. Before O7a we passed
  `foreign-fragment.dat` #48 by accident: the document parse broke
  out, and the cooked view painted the SVG namespace from the context
  over an HTML tree.
- `</body><frameset>`: HTML5 invents a body at `</body>`, then removes
  it when the frameset arrives. A node with bytes cannot be removed
  (O5); a node WITHOUT bytes can — the invented body's only children
  are mala, they move up, the frameset takes its place. The
  asymmetry is the whole point of the annotation.
- Two cases that used to pass now fail, and they should: a comment
  after `</body>` belongs to `html`, and `<title>` after `</head>`
  belongs to `head`. Their bytes lie inside the body / after the head,
  so the byte tree cannot put them where the DOM does. That is exactly
  `sedes` (O7b): the node stays at its bytes, the annotation names its
  DOM parent.

Numbers: 1,195 → 1,299 of 1,700 (76 %), fragments 85 → 131; 106 rises,
2 named regressions. Zero substrate changes, again. Next: O7b — table
modes and `sedes`.

## 2026-09-15 — the whole corpus through the round trip

Fran asked whether the html5lib corpus was going through the byte
round trip. It was not: the oracle compared cooked views only, and the
byte law and the STML cycle ran on fourteen pages and thirty inline
cases. `probatio_html_circuitus` now pushes all 1,708 cases (fragments
through the fragment entry with their context, script-on included)
through direct emission and then the full STML cycle. First run: 1,708
of 1,708 byte-identical; 1,672 STML-identical; the 36 others are the
NUL-bearing cases the STML writer refuses (`valor lexematis NUL fert`),
pinned by cause against the raw-form limit 01M2KPJ0HW — when materia
grows its escaped fallback, that pin goes red and the cases promote.
No closing-sequence refusal in the corpus, no other cause. 0.3 s. The
O7b and O7c annotations will be proven here rather than on cases I
chose.

## 2026-09-16 — O7b: table modes and `sedes`; 1,299 → 1,398

The second annotation. `sedes:referentia` on `elementum`, `textus`,
`referentia` and `commentarium` names the DOM parent of a node whose
bytes lie somewhere else. The node stays where its bytes are; materia
writes `<sedes(> #nodN` on it and `id="nodN"` on the target — html is
the first client to declare a reference locus after oratio, and the
canon now declares `id` on `elementum` and `documentum`. The cooked
view merges each parent's byte-children with the nodes whose sedes
point at it: before the child that is a `table` and contains them
(foster parenting), otherwise after everything (content after
`</body>`, head content after `</head>`, fragments in a table
context, where the spec appends). One rule, three sources.

The builder learned WHATWG's table modes as per-frame fields, O(1)
as always: the mode from the frame's own HTML title (a MathML `tr`
never sets one), the nearest open table, the nearest REAL table
part. A table-part start tag clears the stack back to that real
part; `<table>` inside a table closes it; when the current node is
`table tbody tfoot thead tr` and the token is not one the mode
handles, the node is created where it is and fostered by annotation.

The O5 law reversed: `</body>` and `</html>` now close their
element. The spec leaves the stack untouched and only switches mode,
so later content goes into whatever element was open then — that
element is remembered (`sedes_posterior`) and everything inserted at
html or document level afterwards carries `sedes` to it. Comments
follow only once content has re-entered the body, which is why
`commentarium` got the locus too.

What the runs taught:

- Fragment contexts whose table mode comes from the context alone
  fail the spec's "in table scope" checks, because the context
  element is not on the stack: a table body accepts only tr/td/th, a
  row td/th, a caption nothing, a column group col, a table
  everything but table. Twelve innerHTML cases hung on that.
- `<input type=hidden>` stays in the table, and attributes are not
  seen at push time — the decision is deferred to the tag's `>`
  (`fovens`), the first time a pending state carries a placement.
- The first cut of "`<frameset>` after `</body>` removes the
  invented body" dropped the body's closing tokens. The oracle
  rewarded it (the tree matched) and the corpus round-trip gate
  refused it (bytes lost). The tokens now move into a malum. That is
  the gate Fran asked for doing exactly its job, one commit after it
  was born.
- Accepting `<frameset>` had reset the mode to "in body", silently
  discarding the post-body sedes. A mode may only move forward.

Numbers: 1,299 → 1,398 of 1,700 (82 %), fragments 131 → 152, 99
rises, zero regressions. Next: O7c, the adoption agency and
`exemplar`.

## 2026-09-16 — O7c: the adoption agency; 1,398 → 1,504

The third annotation, and a fourth the decree had not named. An
`exemplar` is a formatting element reopened by the adoption agency
or by the reconstruction of the active formatting list: no tokens,
a reference to the original (always the real one), name and
attributes read through it. That much was the plan. What the plan
could not know: where a moved block sits among its new siblings.
`<b><a><div>x</b>y</a>` ends with body{ b{a}, a', div{...} } and the
clone `a'` was born AFTER `div`'s bytes, so no rule over byte order
puts `div` after it. The spec appends the block right after the
formatting element being closed, and the builder knows that element
exactly — so `praecedens:referentia`, the DOM previous sibling, one
locus beyond the decree, and the cooked view inserts a sedes-node
after its praecedens before it falls back to the O7b rules.

The stack changed nature. Until now it was the byte stack, and O7b
had bolted a single remembered node (`sedes_posterior`) on the side
for content after `</body>`. The adoption agency needs the spec's
DOM stack: after `</a>` closes `a`, the `div` above it is still open
in the DOM but its bytes are done. So every frame now carries
`octeti` (byte-open) and `receptor` (the nearest byte-open frame at
or below); a node goes to the receptor and, when the DOM current
node is a different frame, gets `sedes` = that node. `</body>` no
longer pops anything, it marks frames byte-closed, and O7b's special
case disappeared into the general rule — with it the "`<b>x</body>y`
closes b" deviation. The closing tag itself lands in the formatting
element's closure when that element is byte-open (the frames above
are cut), in a malum otherwise.

The agency runs WHATWG's steps as written, on that stack: furthest
block, the inner loop over the elements between, a clone per
formatting element in the chain (the chain top is a byte-child of
the receptor; each lower clone a byte-child of the one above; the
block hangs off the innermost by `sedes`), then the wrap: the clone
of the formatting element takes the block's DOM children. The wrap
is byte-tree surgery and it is byte-safe, because the clone emits
nothing and the children keep their order — fostered byte-children
stay outside it, and the nodes elsewhere whose `sedes` was the block
are re-pointed at the clone (one scan of the parse's sedes-nodes).
The stack segment from the formatting element to the block is
replaced in one pass, scope indices and list indices recomputed;
eight outer iterations at most. Markers, Noah's Ark and
reconstruction as in the spec; markers are purged lazily, when the
list is next used and the marker's element is no longer on the
stack — no pop site had to learn about them.

What the runs taught:

- The first cut lost 21 cases against O7b and 13 of them were not
  regressions at all: my diff kept the fragment-context suffix
  (`svg.dat #1 [td]`) on one side and stripped it on the other.
  Strip both before trusting a diff of failing sets.
- `<nobr><nobr><nobr>` produced FIVE nobr elements: `a` and `nobr`
  were still in the implied-close table ("closed by itself"), so the
  second `<nobr>` popped the first, reconstruction cloned it back,
  and only then did the agency close the clone. The agency owns
  those two tags now; the table lost them.
- `</p>` with no `p` in scope makes a synthesized `p` — but only in
  the body modes and inside a template. Before the head is closed,
  and in a frameset, the spec IGNORES it (noscript01 #15, tests19 #2,
  #3, #44 said so).
- The foster parent is the table's DOM parent, not the frame below
  the table: `<a><table><a>` runs the agency for the first `a`,
  finds it out of scope, and removes it from the list AND THE STACK
  even though it is still byte-open; the second `a` is then fostered
  into the first, which is no longer on the stack at all.
- A 21-byte string declared as 22 bytes: the parser read the
  terminating NUL, the byte gate happily round-tripped it, and the
  STML writer refused it ("valor lexematis NUL fert"). Count.
- `renominare.sh` refuses a dirty tree; the two-letter locals the
  lint forbids were renamed by hand within their functions.
- The registrum probatio's hand-built element (`(i32)VIII` loci)
  went red the moment the registry grew: the writer walks the
  registry's window. It now sizes itself from the registry.

Numbers: 1,398 → 1,504 of 1,700 (88 %), fragments 152 → 155, 106
rises, zero regressions; adoption01 18/18, adoption02 2/2, tests1
108/114, tricky01 8/9, tests26 9/16. Zero substrate changes. The
corpus round trip: 1,708 of 1,708 byte-identical, 1,672 STML, the 36
NUL refusals unchanged. Left on the table: template interplay (22),
frameset after content (plain-text-unsafe, tests19), the foreign tail
(svg.dat 0/8 — SVG `title` is lexed raw; `font` breakout wants
attributes), raw-text fragment contexts (tests4: the lexer would need
the context's tokenizer state), `<image>` → `img`.

## 2026-09-19 — `spatium-omissum`: the split tier 1 needed

Task 1 of the html diagnostics arc (declaration only; the builder still
creates nothing of this genus — that is Task 2).

**Why a genus at all.** The plan started as "declare a `diagnosticum` on
`elementum-malum` and be done" — declaration-only, no C. That was
wrong twice over, and measuring said so before a line was written:

1. **materia can only key a diagnostic on a GENUS.** `diagnosticum` is a
   genus attribute; `absentia` and `vacua` are locus attributes
   (`materia/grammatica/registrum.canon`). There is no condition on what
   lies *inside* a node. So a `diagnosticum` on `elementum-malum` fires
   on every instance without exception.
2. **And the population is overwhelmingly innocent.** Over the 260 house
   pages: 531 `elementum-malum` nodes, **every file carrying at least
   one**, of which 518 are a newline between `<!DOCTYPE html>` and
   `<html>` or between `<html>` and `<head>` — whitespace the DOM
   drops, correctly recorded, nobody's mistake. Only 13 are real
   (unmatched
   `</em>`, all in generated `knotapel/atlas/web/` pages). A bare
   declaration would have shipped at a **97.6 % false-positive rate**.
   The sibling corpus `../lapide` agrees: 651 files sampled, 686 benign
   against 2 real.

**The discriminator is not the token genus.** The obvious cheap fix —
"a `lex-textus` malum is benign, the rest are errors" — is false. Two
probes settle it: a newline before `<head>` is a `lex-textus` malum and
innocent; the `x` in `<frameset>x</frameset>` is a `lex-textus` malum
and a genuine error. Same genus, same token kind, opposite verdict. The
predicate is *whitespace-only text dropped while in a wrapper mode*,
which only the builder knows at the moment it drops it. Hence a genus:
the tree has to carry the distinction, because nothing downstream can
recover it.

This is worth doing on its own terms. O5 widened `elementum-malum` into
the catch-all for everything HTML5 ignores; the genus answers "does the
DOM keep a node for this?", while a diagnostic must answer "did the
author make a mistake?" Those two predicates disagree 518 times out of
531. The split walks O5 back one step and narrows the genus toward what
its own nota always said it was for.

**Classes 2 and 3 stay together, deliberately.** There are arguably
three populations: whitespace the DOM drops (split off), tokens HTML5
explicitly ignores (repeated `<html>`, doctype after content, table
parts outside a table, tags in select/frameset), and unmatched end tags.
The middle class has **zero instances in either corpus** — 260 house
pages and 651 lapide files. Splitting it would be designing against a
population of nothing. If it ever proves noisy, another declaration
append plus a seal move is the documented ritual and it is cheap twice.

**Seal `b307882e` → `1ebf2676`**, 10 genera → 11, 25 loci → 26.

Both drift guards did their job unprompted: the canon gate went red on
the seal, and the registrum gate went red on `GENERA_EXSPECTATA` (10 vs
11) and then *segfaulted* — the loop walks the registry's window, so a
hand table one row short reads past its end. That is the third time this
probatio's hand-written tables have rotted the moment the registry grew,
and the third time they caught it.

Oracle pin verified UNMOVED at 1,504/1,700 — the genus is inert until
Task 2, so every tree is byte-for-byte what it was. That was checked
first rather than last, because the cooked view drops mala and a second
genus it does not drop would regress the pin silently.

## 2026-09-19 — Task 2: the split becomes real in the tree

`_spatium_omissum_addere` beside `_malum_addere`, one call site
rerouted (`html_arbor.c`, the text handler's `neglectum` block), and
`html_coctum.c` drops the new genus alongside the old.

**Result over the 260 house pages: `elementum-malum` 531 -> 13, and
the files carrying any 260 -> 10.** The 13 are the real ones — stray
`</em>` in generated `knotapel/atlas/web/` pages. Oracle pin
**1,504/1,700 UNMOVED**, which was the risk worth naming: the cooked
view drops mala, so a second genus it did not drop would have
regressed the pin silently.

**The nota I wrote in Task 1 was wrong, and the code said so.** It
claimed the benign modes were "before html, before head, post head".
The builder only ever ignored the first two, and a four-newline probe
confirmed it: two mala, not four. After `</head>` the spec INSERTS the
character, so it becomes a real text node. Both notas corrected. A
comment recording intent the code never implemented survives every test
that does not exist — this one lived about forty minutes.

**Two gates went red for the right reason and one taught something.**

`probatio_html_arbor` failed on the one inline case that asserts this
exact behaviour (`"\n<p>x"`), reporting genus 9 against 10. That is the
inverse of a plant: the change made a true assertion false and the gate
named the genus numerically. Updated, and a companion case added — a
NON-white text before html stays content, so the split cannot quietly
widen to swallow it.

`probatio_html_computus` diverged by **4 bytes** on one fixture
(`adversarius_2`, `allocationes_arboris` 46840 -> 46844) with `nodi`
unchanged at 370. Goldens are not regenerated without an audit, so:

- A controlled experiment (same new code path, old genus constant)
  returned 46840 exactly. So the delta is GENUS-dependent, not
  code-path dependent.
- Both genera carry `(i32)1` loci in the generated table — identical
  node size and arity.
- Three of the four fixtures contain the new genus (3, 2, 2 nodes) and
  match the golden to the byte. Only `adversarius_2` moves, and it has
  the same count (2) as two files that do not.

Same count, same size, three files unmoved: an arena alignment
artifact, where three fixtures absorb it in existing padding and one
crosses a boundary. Every semantic gate is green — byte corpus
identical, STML two-cycle with comparator, totalitas, oracle. Golden
moved by exactly that one number with the cause recorded here.

What the experiment was worth: it turned "probably alignment" into a
measurement, and it took one perl substitution and one rebuild. A
golden moved on a hunch is a golden that stops meaning anything.

## 2026-09-19 — Task 3: the diagnostic declared

`diagnosticum` on `elementum-malum`, `.html`/`.htm` in
`tools/diagnostica`, and `./tools/diagnostica.sh page.html` now prints
the compiler form with a caret. No C was written for the diagnostic
itself: the declaration is the whole of it, materia's one walker does
the rest, and the seal did not move (diagnostics never move it — css
and crusta both proved that before us).

The cause is capped at 60 bytes by the coctor, which refused the first
draft at 61. A good guard: these strings are printed beside a caret,
not read as prose.

**What it found, immediately:** 13 errata over the 260 house pages, in
10 generated `knotapel/atlas/web/` files, every one a stray `</em>`.
The cause is visible in the excerpt — the atlas generator ran a
markdown emphasis pass over text using `*` as multiplication, so
`2*6+1=13 ... 2*6+1=13` became `2<em>6+1=13 ... 2</em>6+1=13`, pairing
asterisks across half a paragraph and leaving the tail unmatched. One
generator, one pass, ten files. Filed for Fran; not this arc's to fix.

**A two-client ternary was about to swallow the third.** The `-lege`
path derived the grammar name as `_suffixum(via, ".css") ? "css" :
"crusta"` — correct for exactly two clients, and it would have
labelled
every html finding `crusta:` without failing anything. Replaced with
`_grammatica_ex_suffixo`, one function both paths call, which is what
the file's own comment already said the design was ("dispositio
cliens-eligendi semel scripta est, ne itinera divergant"). The comment
was right and the code had drifted from it by one client.

**The corpus golden moved, and the README predicted the shape.** Editing
`tools/diagnostica.sh` (html's sources into the runner) shifted four
rows in `domus.cruda.tsv` — the frozen Python-provenance golden. Its
README names the audit: a legitimate move keeps the arms equal and
`plena` fixed. Measured: one hunk, four rows, one file — the file I
edited — with `cruda` 570, `plena` 14, census 292/278 all unchanged,
and the line shift 45 to 47 matching the net +2 lines exactly. Same
constructs, new offsets. Regenerated with that cause.

That README earned its keep today: it turned "the gate is red, is that
bad?" into a three-line check with a yes/no answer.

## 2026-09-19 — the NUL tail: one label, three bugs

Fran asked whether the `nul` attribute had helped the html5lib oracle.
Measured: **1504/1700, unmoved** — exactly as predicted, because the
oracle compares cooked trees and STML is not in that path. NUL
*representation* and NUL *handling* are different problems that happen
to share a byte.

Then the handling. The orientation doc called `plain-text-unsafe`'s 14
failures "frameset-after-content", and the arithmetic matched exactly
(33 − 19 = 14), so I had repeated it twice. **The match was a
coincidence of totals.** Categorising all 14 gave THREE bugs:

| n | bug |
|---|---|
| 8 | NUL treated as content in wrapper modes |
| 4 | `frameset-ok` cleared by tags that must not clear it |
| ~2 | `<frameset>` inside foreign content vanishes |

### Fixed: NUL as content (1504 → 1509)

HTML5 IGNORES a NUL character token in the wrapper modes (parse
error). We made it content, which forged a body, which made the
following `<frameset>` a malum — `<body>` where html5lib expects
`<frameset>`. `_textus_albus_cum_nullo` routes a whitespace-and-NUL run
to `elementum-malum`: a MALUM, not the `spatium-omissum` added two
commits ago, because dropped whitespace is faultless and NUL is a
parse error. The two genera stay honest.

The lexer makes one token per byte run, so ` NUL ` is ONE token and is
ignored whole. Splitting it would need a synthetic token, which H4
forbids. Every failing corpus case is whitespace-and-NUL only, so the
whole-run rule covers them.

Gate case in `probatio_html_arbor`, planted: neutering the condition
fails two assertions AND drops the oracle straight back to 1504, which
is the evidence that the +5 is this change and nothing else.

### Not fixed, and the measurement is the finding

**`frameset-ok` polarity is inverted.** `COMPAGIS_INNOCUA` lists 16
head-ish tags and clears the flag for everything else; the spec's "not
ok" list is short and specific. So `<svg>`, `<p>`, `<div>`, `<path>`
clear it when they must not — and two of the four cases
(`<svg><path></path></svg><frameset>`, `<svg><p><frameset>`) contain no
NUL at all, which is how you know this bug is separate.

I replaced the table with the spec's list and measured: **1509 →
1503.** A net regression of six. The reason is that `_vexilla_renovare`
fires for every accepted start tag in EVERY mode, while the spec's list
governs "in body" only; the hook is mode-blind, so the old over-clearing
happens to match more cases than correct under-clearing does. Reverted.
A real fix has to make the flag update mode-aware. **Being right about
the spec is not the same as being right about this code.**

**`<frameset>` in foreign content** should be an ordinary foreign
element — `frameset` is correctly absent from `RUMPENTIA` — but the
node vanishes. I guarded the body-removal branch on
`proprium == HTML_ALIENUM_NULLUM`, which is spec-correct reasoning, and
it changed the number by exactly zero. Reverted rather than shipped: a
change with no measured effect is a future reader's false lead.

### Two traps worth keeping

**A build break is not a measurement.** Reverting the polarity
experiment, I left both tables in the file; `-Werror
-Wunused-const-variable` refused, and I captured a "before" failure set
from a stale binary and diffed it against a real one. The diff looked
like a catastrophic regression. Same family as "a plant that breaks the
BUILD proves nothing" — check the build succeeded before believing the
number.

**The oracle diff prints the first differing LINE, so a shorter tree
flags its last common line.** `<svg>NUL<frameset>` reported line 5
the replacement character against the replacement
character — visually identical, and I nearly filed it as a
cooking bug. The real difference was a missing line 6 (`<svg
frameset>`). When both sides of a diff look the same, compare lengths.
