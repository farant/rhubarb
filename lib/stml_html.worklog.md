# lib/stml_html.worklog.md

## 2026-09-01 — natus (spec instrumenti §5, verbum `vertere`)

Born as a LIBRARY, not tool-internal code (Fran's decree at the
vertere design round): the §8 battery lives in
`probationes/probatio_stml_html.c` as credo tests, and apps (forum,
villa) can emit HTML from STML later without shelling out.

First run: 50/50. The two design decisions that shaped the code:

**Singletons emit as written** (Fran, 2026-09-01, AGAINST the
house-instinct recommendation — recorded loudly for that reason):
the emitter judges HTML validity NOWHERE, so there is no singleton
table and none should be re-added. `<body>x</body><body>y</body>`
out of a distributed wrapper is the pinned expected value.

**The whitespace law (§5.9) is the architecture**: whitespace with
a newline lives in trivia (spatia_ante/post/clausurae), not TEXT
nodes. Every child emission is `ante + node + post`; element
interiors end with `clausurae`; the document's clausurae is the
file tail. An emitter walking children alone renders "x y" as
"xy" on essentially every pretty-printed source line.

Two escape regimes (§5.4), never one: text is DECODED in the tree
(escape `&` `<` `>`); inline attribute values are RAW (emit bytes
untouched — escaping them yields `&amp;amp;`); attribute-element
values are DECODED (escape `&` `"`). `stml_attributum_capere` is
banned here (01M16YRSZ5 — it lies about decoding).

Entity refusal (§5.10) is a FIGURE match: `&word;` / `&#...;` in
text refuses with the entity named, because `&nbsp;` and
`&amp;nbsp;` produce identical trees — no emitter can be correct,
so the ambiguity is refused. "Tom & Jerry" passes.

Relectio oracle is html_lexema — a second oracle of a DIFFERENT
KIND (the recorded lesson): emitted HTML must lex with zero
PRAVUM/IMPERFECTUM lexemes and survive the lexer's own fidelity
round-trip.

ATTRIBUTUM_SOLUTUM is only reachable on post-expansion/manual
trees (the parser itself judges attr-element position, §6.3), so
its pin builds the tree by hand with stml_elementum_creare.
