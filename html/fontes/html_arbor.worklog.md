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
