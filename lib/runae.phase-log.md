# runae — phase log

*The text stack's Unicode core, first brick: width + graphemes. Plan:
`project-specs/unicode-width-graphemes-plan.md` (D1–D8 approved by Fran
2026-09-28). Narrative of building here (INTENTIO before a task,
FACTUM/RELATIO after); find-time notes in `lib/runae.worklog.md`.*

## U1 — CORPUS (2026-09-28)

### INTENTIO

Test DATA before any table. `tools/runae_corpus.c` parses Lapide's
`01_Preliminares{,_<lingua>}.html` with the HOUSE html parser
(`html_arbor_parsare`, materia) and walks the tree: every `<p>` in
document order; its text and entity nodes decoded exactly as
`html_coctum` does (`entitates_html_decoquere`), descendants included
(`<b>`, `<a>`); `<br>` → newline; runs of ASCII whitespace collapsed to
one space (U+3000 and other non-ASCII spaces kept); paragraphs trimmed
and separated by a blank line. Whole paragraphs until the sample
reaches 4 KiB. One file per language:
`probationes/fixa/runae/corpus/<lingua>.txt`, 35 languages (Lapide's
unsuffixed files are English, `_lt` is Latin → `la.txt`). The tool
builds against `html/build/` objects like `html/arbor.sh`.

Gate: `probationes/probatio_runae_corpus.c` (red first: files absent):
for each of the 35 languages the file exists, is ≥ 4 KiB, is valid
UTF-8 end to end (`utf8_decodere` never −1), and is in the right
SCRIPT: at least 100 codepoints in that language's main block
(Devanagari for hi, Thai for th, Hangul for ko, Hebrew for he, …;
Latin letters for the Latin-script languages). The script check is
what catches an extractor that mixes files up or loses the text. A
plant: a sample file swapped between two languages → red by name.

**U1 FACTUM (corpus).**
- `tools/runae_corpus.c` + `.sh` extract 35 samples (4,108–6,169 bytes)
  through the house HTML parser.
- An independent Python `html.parser` extraction matched all 35 byte
  for byte.
- `probatio_runae_corpus.c` was red first (35 files absent by name),
  then green.
- Plant: hi and th samples swapped → both named (0 runes in their
  script), restored.
- Provenance in `probationes/fixa/runae/corpus/PROVENIENTIA.md`.
