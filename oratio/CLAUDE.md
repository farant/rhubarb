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
planted fault red). NEXT: T2 `oratio_lexema` (scanners, positions,
UTF-8 runs, CRLF).

## Laws to keep (spec §2–3)

- Every byte is a token in ONE slot; spaces and newlines are CONTENT in
  `cauda`/`praefixa` slots, never trivia; LINEA has munus LINEA so the
  FIDELIS comparator runs.
- A word is ONE `vocabulum` with parts; the tokenizer never decides the
  linguistics (enclitics, contractions are the dictionary's job).
- Genera and token genera are APPENDED, never reordered; analysis genera
  (stage 3) append after `numerus`.
- Unknown = finding, never a refused parse.
