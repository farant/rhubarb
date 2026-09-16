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
