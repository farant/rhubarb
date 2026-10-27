# vicipaedia worklog

## 2026-10-27 — N1: Parsoid HTML -> Articulus

Source: Wikipedia REST page HTML (`/api/rest_v1/page/html/<Title>`),
Parsoid. Walks the html/ BYTE tree (html_arbor_parsare) - Parsoid
output is generated and clean, so the byte tree equals the DOM in
practice; messy HTML (adoption agency, foster parenting) would need
the cooked view. Rules as built:
- top-level `<section>` = ArticulusSectio, its first h2 = titulus +
  `id` as ancora; nested sections flow into the same top section,
  h3-h6 become TITULUS clausulae with gradus III-VI;
- block elements -> clausulae (p/dt/dd/pre/address/figcaption =
  PARAGRAPHUS, li = INDEX with gradus = nesting depth, blockquote =
  CITATIO, figure = FIGURA with clavis = image URL, caption = textus);
  inline runs between blocks (e.g. the hatnote div's text) become an
  anonymous PARAGRAPHUS;
- text: entities decoded, ASCII whitespace contracted (NBSP U+00A0
  and thin space U+2009 are kept - Wikipedia uses them in "100 g",
  "88 kJ", "c. 1225"), <br> = '\n', nested blocks inside a collected
  element = line breaks; notes (NEXUS/EMPHASIS/FORTIS) are byte spans
  recorded at close (a pending space is written BEFORE a note opens so
  the span starts on the first byte of the linked text);
- table.infobox -> ArticulusAdiunctum of the top section it sits in:
  caption or a first lone th row = titulus; two-cell rows = PAR
  (clavis: trailing ':' and spaces stripped - the taxobox writes
  "Kingdom:" in plain td cells while other boxes use
  th.infobox-label); lone th = TITULUS gradus III; lone td =
  PARAGRAPHUS; a row containing a nested table is replaced by that
  table's rows; other tables skipped (v1);
- dropped: style/link/meta/script/noscript/img (inline), classes
  mw-ref, navbox(-styles), noprint, metadata, shortdescription,
  mw-empty-elt, reflist, mw-references-wrap, gallery (v1); sections
  References / Further reading / External links; empty sections.

Sizes: Rhubarb (296 KB) parses + extracts in ~10 ms (scratch dump in
build/specimen_vicipaediae, not committed). Test fixtures: real REST
HTML in probationes/fixa/vicipaedia (FONS.md: CC BY-SA attribution,
Aquinas trimmed at top-level section boundaries). Four plants red
(reference markers kept, omitted-section list emptied, heading level
shifted, infobox not routed). Note: emptying the omitted-section list
still hides References, because its contents (reflist) are dropped and
the section becomes empty - Further reading is what catches it.

Gotcha: a new lib/*.c is only linked into the test runner after
`./tools/compile_tests_fontes_generare.sh` regenerates
compile_tests_fontes_generata.sh (here it also pulled in the five
html/fontes objects).

## 2026-10-27 - N2.2 sedes (URL <-> article identity)

`VicipaediaSedes` = (hospes, titulus decoded with '_', ancora). Identity
of an article is (hospes, titulus); the canonical URL is rebuilt from it
with `url_codificare`, so `(` comes out as `%28` - noisier than
Wikipedia's own links but one spelling per article, which is what a
bookmark key needs.

Trap found: `url_decodificare` maps '+' to a space (form encoding), so
the article "C++" decoded to "C  ". Added `url_viam_decodificare`
(RFC 3986 path: '+' stays); both share one static decoder in url.c.

REST form `/api/rest_v1/page/html/<T>` is cut at the first RAW '/'
(a revision id can follow); a '/' inside a title arrives as %2F there,
so "AC/DC" round-trips. In the `/wiki/` form a raw '/' belongs to the
title. Host rule: `<lingua>[.m].wikipedia.org`, lingua [a-z0-9-]+,
case-insensitive; userinfo refused. Not accepted in v1:
`/w/index.php?title=`.
