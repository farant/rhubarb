# Corpus Lapidis — provenientia

Source: Fran's own translations of Cornelius a Lapide, `../lapide`
(git 7b00f3b0, 2026-09-28; the `01_Preliminares*.html` files clean). Checked
in by Fran's decision (runae plan D8, 2026-09-28).

One file per language, `01_Preliminares{,_<lingua>}.html`: the
unsuffixed page is ENGLISH (`en.txt`), Lapide's `_lt` is LATIN
(`la.txt`), the rest by their code. 35 languages.

Made by `./tools/runae_corpus.sh` (the house HTML parser, materia):
every `<p>` in document order, entities decoded, `<br>` → newline,
ASCII whitespace runs → one space, paragraphs separated by a blank line,
WHOLE paragraphs until the sample reaches 4 KiB (4,108–6,169 bytes).

Cross-checked 2026-09-28: an independent extraction with Python's stdlib
`html.parser` under the same normalization gave byte-identical output
for all 35 files.

Regenerate: `./html/compile_probationes.sh registrum` (objects), then
`./tools/runae_corpus.sh [radix_lapidis]`. The gate is
`probationes/probatio_runae_corpus.c`.
