# html — orientation

*You are reading this because you touched a file in `html/`.*

HTML parser on **materia** (fifth client, after the C89 shim, css, md
and oratio). Design: `project-specs/html-arbor-spec.md` — **§11, the
2026-09-15 addendum "As built on materia", governs the body** (the
body was written against silva). Plan: `project-specs/html-arbor-plan.md`
(H1–H11, one commit each, one task per turn with Fran's approval).
Lexer: `lib/html_lexema.c` (root tier, shipped 2026-08-18; total, byte-
faithful, owns raw text and RCDATA — the builder never tells it anything).

## Status

- **H1 (2026-09-15):** declaration `html/grammatica/html.registrum.stml`
  → generated `html_registrum_coctum.{h,c}`; hand slot enums
  `html_registrum.h`; lexicon `html_lexicon.{h,c}` (22 rows; trivia
  INSIDE TAGS ONLY; comments are content, not `munus COMMENTUM`; a
  stray `/` inside a tag is trivia); runner; gate
  `probatio_html_registrum`. Registered in pythonica (`PORTAE['html']`).
- **H2 (2026-09-15):** `html_adaptare` one for one (positions carried,
  `initium_lineae` never set) + `HtmlLigator` (css's division rule;
  trivia bind only inside tags; `FINIS` receives the trivia of a tag
  cut off at EOF); gate `probatio_html_adaptare` = BYTE COVERAGE
  (every offset exactly once) over 25 inline cases, a NUL case and the
  three fixtures, plus the segmented-Xar contract (allocate between
  accumulations). A stray `/` inside a tag binds as trivia beside the
  spaces (`<a /\n href>`: space + `/` go to `<a`, the newline run to
  `href`).
- **H3 (2026-09-15):** `html_arbor_parsare` — the builder's first
  half: `documentum`, elements, attributes, content nodes by lexer
  genus, over an EXPLICIT open-element stack (`Xar` + depth, never
  recursion). An end tag pops to the nearest open element of its name
  (ASCII case-insensitive), closing the ones above it with absent
  slots; no such element → `elementum-malum` carrying `</x` and its
  `>`. `/>` does NOT close a non-void element (HTML5 ignores the
  self-closing flag on HTML elements; void handled by the H4 table).
  Three PENDING states carry a tag across its lexemes (`tag_apertum`,
  `clausura`, `malum`); any lexeme not belonging to the tag closes
  them with absent slots (truncation expressed, never a synthetic
  token). Gate `probatio_html_arbor`: byte oracle through
  `materia_scribere_nodum` on 16 cases + §4.2's rows (`<div>` at EOF,
  `<img/>`, `<div cl`, `<a = x>`), parentage, case-insensitive close,
  implicit close by an outer end tag, malum, content genera, raw text.
- Next: H4 the builder, part two (void elements, implied closes as
  tables).

## The registry is generated

`HtmlGenus`, `HTML_GENUS_NUMERUS_GENERUM` and `HTML_REGISTRUM` come from
`html/grammatica/html.registrum.stml` via
`./materia/coquere.sh html/grammatica/html.registrum.stml -scribere`
into `html/fontes/html_registrum_coctum.{h,c}`. To add a genus or slot:
append to the declaration (genera append-only; locus order = emission
order), run `-scribere`, add the slot enum member by hand in
`html_registrum.h`, add the row to `LOCI_NOMINATI` in the registrum
probatio, then move the seal in `html.canon` by hand with a cause.
`probatio_html_registrum` regenerates in-process and compares bytes; a
hand edit of a `_coctum` file goes red with the first divergent line.

## Laws (spec §11)

- **Absent slots are meaning** (H4): void, implied close, unclosed at
  EOF, self-closing = NIHIL token slots, never synthetic tokens.
- **Text is a node** (H6): between tags every byte is `TEXTUS`; the
  ligator binds trivia only inside tags.
- **The builder is iterative** — an explicit open-element stack, never
  recursion; source depth must not touch the C stack.
- **Nothing reserved in v1** (§11.6): the reader refuses unknown
  elements with a named cause and IGNORES unknown attributes (measured);
  extension points are named in the declaration's header, appended
  with the seal when their mechanism arrives.

## Currere

```
./html/compile_probationes.sh            # omnes
./html/compile_probationes.sh registrum  # filtrum substringae
```

0 sanum / 1 fractae / **2 = NULLA CURSA**. Log: `build/test_logs/html.log`.
Compiles materia's sources into `html/build/` — html CONSUMES materia,
it does not contain it.
