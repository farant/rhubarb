# oratio — orientation

*You are reading this because you touched a file in `oratio/`.*

Natural-language (prose) parser on **materia** (fourth client after the
C89 shim, css and md) with cross-language word classification. Design:
`project-specs/oratio-spec.md` (v1 + v2 codebase pass, as-built notes
per stage); interview `oratio-interview.md`; ledger parcum 01M1NH3F46.
Stance: moderate realism — classes earned by function, edge cases by
analogy, ambiguity as LISTS at every layer, unknown word = FINDING.

## Status (2026-09-04)

**T1 DONE**: `oratio_registrum` (6 genera / 16 slots: documentum,
paragraphus, sententia, vocabulum, interpunctio, numerus; whitespace as
`cauda` tokens under the possessor law, no spatium genus),
`oratio_lexicon` (12 token genera, prefix `or-`, LINEA is the
terminator), runner `./oratio/compile_probationes.sh [filter]` (exit 2 =
NOTHING RAN), pythonica gate tables (`PORTAE['oratio']`), gate
`probatio_oratio_registrum` (titles, contiguity, materia round trip;
planted fault red). **T2 DONE**: `oratio_lexema` — whole-stream lexer
(`oratio_lexare`: letters incl. bytes >= 0x80 except NBSP and the
General Punctuation block, digits, hyphen/apostrophe between letters,
PUNCTUM vs dot runs, one punctuation token per byte except same-byte
runs, CRLF/CR, positions), derived-token factory; gate
`probatio_oratio_lexema` (75 assertions + the md corpus byte-exact:
1,134 files, 6.7M tokens, 0.7 s). **T3 DONE**: `oratio_arbor` —
`oratio_arbor_parsare` (paragraphs by blank-line runs with the tail
split; elements vocabulum/numerus/interpunctio with parts; sentences
v1: candidate punctuation + closers, lookahead for a capital/number,
abbreviation table incl. initials, internal-period words) and
`oratio_abbreviatio_est`; gate `probatio_oratio_arbor` (68 assertions
+ CORPUS: every md paragraph via md's tree, 81,844 paragraphs
byte-exact, 155k sentences, 1.9M words, 1.5 s). **T4 DONE**: `oratio_stml`
(consilium with the origin hook for derived tokens),
`oratio/grammatica/oratio.canon` (hand-written, seal `b27fe13a` pinned;
`signum` admits leftover joiners after a number suffix),
`./oratio/arbor.sh <x.txt> [-tacitus]`; gates `probatio_oratio_stml`
(STRUCTURALIS + FIDELIS round trip, derived channel exercised, corpus
SAMPLED every fifth file, 8.5 s) and `probatio_oratio_canon` (drift
guard both ways + seal + corpus judged, sampled, 4 s);
`ORATIO_CORPUS_TOTUS=1` runs the whole corpus (40 s + 20 s). Parent
pointers are fixed after parsing (`materia_arbor_patres_figere`).
**T5 DONE**: `probatio_oratio_totalitas` (random / mutated / truncated /
nested / 50k-deep: never crashes, always byte-exact; 386 assertions,
0.8 s) and the computus twin `./oratio/computus.sh <x.txt> [-machina]
[-iter N]` with gate `probatio_oratio_computus` over five vendored
Gutenberg texts (`fixa/txt/`, FONTES.md; golden
`fixa/computus/basis.tsv`, `COMPUTUS_SCRIBERE=1` + a named cause;
`silva.metiri('x.txt')`). Measured: STML projection ≈ 34× source, write
+ read ≈ 10× parse. NEXT: T6 fissio comparison (`sententia_fissio` vs
oratio over the corpus), then stage 2 (Latin dictionary).

## Laws to keep (spec §2–3)

- Every byte is a token in ONE slot; spaces and newlines are CONTENT in
  `cauda`/`praefixa` slots, never trivia; LINEA has munus LINEA so the
  FIDELIS comparator runs.
- A word is ONE `vocabulum` with parts; the tokenizer never decides the
  linguistics (enclitics, contractions are the dictionary's job).
- Genera and token genera are APPENDED, never reordered; analysis genera
  (stage 3) append after `numerus`.
- Unknown = finding, never a refused parse.
