# runae — worklog

## 2026-09-28: U1, the Lapide corpus

- Lapide's language suffixes: the unsuffixed page is ENGLISH and `_lt` is
  LATIN (by the pages' own `hreflang`). The plan's first draft had both
  wrong; fixed there.
- The house HTML parser (materia's html client) extracted all 35
  languages, and an independent Python stdlib extraction under the same
  normalization matched byte for byte. The walk is simple because
  Lapide's pages are clean: no content out of place (`sedes`), no
  adoption agency. A messier page would need html_coctum's ordering
  logic, so don't reuse the walk on arbitrary HTML.
- Useful for later tasks: `ar` is fully vowel-marked (dense combining
  marks, width 0), `yo` stacks combining tone marks on dotted letters,
  `ja` uses U+3000 ideographic spaces (kept, not collapsed), `th` has no
  spaces between words.
- The tool builds against `html/build/*.o` (like `html/arbor.sh`); those
  objects have no `filum`/`utf8`, so it uses stdio directly.
- The script gate proves the script, not the language: Latin-script
  languages (21 of them) and ar/fa can't be told apart by it.
- examen: `<br>` inside a C comment is read as an annotation tag; I wrote
  "(br)" instead.
