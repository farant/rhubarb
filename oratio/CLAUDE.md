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
+ read ≈ 10× parse. **T6 DONE — STAGE 1 COMPLETE**: `oratio_sententiae`
(sentence extents, tails excluded), `./oratio/sententiae.sh <x.txt>
[-machina]`, gate `probatio_oratio_fissio` (measure, not pin: fissio
38,899 vs oratio 36,762 sentences, 35,922 common = 92.3% / 97.7%; three
disagreement classes in the worklog: line ends as boundaries in fissio,
`?`/`!` before lowercase, paragraph ends without punctuation in
oratio). Replacement of `sententia_fissio` behind its API = Fran's call.
**T6b DONE — STAGE 1b**: the text-FORM layer (decisions 23–25).
`ORATIO_PARAGRAPHUS_FORMA` (INDEX, appended; registry now 17 slots,
seal `93c1c9cf`, canon pinned with the cause); `oratio_forma` decides
prosa | versus | titulus | tabula | index per paragraph from indicia
(lines, widths, the VOLUNTARY BREAK, punctuated/capitalised line ends
and starts, list marks, inner gaps) through a RULE TABLE
(`ORATIO_REGULAE_FORMAE`, ten rows, first match wins, no match = prosa
= the merge bias); the reader consults the slot (versus/tabula/index:
line = unit; titulus: one unit). Instruments `./oratio/formae.sh
<x.txt> [-machina]` (forma, rule, every indicium per paragraph) and a
forma column on `sententiae.sh`. Gate `probatio_oratio_forma` (171:
rule table, 22 inline cases, fixture census — Propertius 21 versus /
0 prosa, 731 units where T6 had 52; Lincoln 41 prosa / 0 versus —, the
HAND-JUDGED set `fixa/iudicia/` (`[[` before each unit start, pins only
rising: 8/8, 8/8, 25/25, Cicero 10/14), md corpus census sampled
(14,862 paragraphs: prosa 11,879, titulus 2,869, versus 76, index 21,
tabula 17; `ORATIO_FORMA_SPECIMINA=1` prints the non-prose ones).
Fissio re-run: 93.0% / 95.7%, Propertius now covers 97% of fissio's
starts. Open DATA: two spaces after `?`/`.` before a lowercase word is
an editorial boundary in Gutenberg (Cicero, 4 of 14) — candidate rule,
measure first. **T7 DONE — stage 2 opened**: Whitaker's WORDS vendored
verbatim under `oratio/vocabularium/la/` (`FONTES.md`, `LICENTIA.txt`;
commit 1f2f0fb0), `oratio_vocabularium` compiles the four files into
ONE nuntium stream (header + 39,335 stems, 1,797 inflections, 343
addons, 79 uniques; column law from the Ada asserted on every record)
committed as `oratio/vocabularium/la.bin` (3,816,483 bytes, seal
`f598155c06f52682`, coction 24 ms, read-back 3 ms);
`./oratio/vocabularium.sh [-coquere] [-scribere]`; gate
`probatio_oratio_vocabularium` (77: pins, coction == committed,
decoded samples, mutations stop with file and line). **T8 DONE**:
`oratio_vocabularium_la` — load once (22–25 ms) into hashes by FOLDED
key (v→u, j→i, ligatures, macrons), `quaerere(forma)` → analyses in
WORDS' order with the matching law transcribed from
`words_engine-word_package.adb` (uniques → stem+ending → -que/-ne/-ve
only when nothing found), lemma v1 from the inflection table;
`./oratio/quaere.sh forma…`; gate `probatio_oratio_vocabularium_la`
(94: WORDS-behaviour forms, unknowns as findings, corpus coverage of the
Latin fixtures 95.1 %, ~8 µs a word). FINDING pinned: `sum` is absent
from the vendored DICTLINE.GEN (compounds only) — supplement in T9.
T8b (data-counted): tackons with a base part, prefixes, suffixes, full
dictionary form. NEXT: **T9** `oratio/glossarium.stml` + canon + loader
(house entries incl. `sum`; allowed technical terms), then T10
`vocabula.sh` + `silva.vocabula()`.

## Laws to keep (spec §2–3)

- Every byte is a token in ONE slot; spaces and newlines are CONTENT in
  `cauda`/`praefixa` slots, never trivia; LINEA has munus LINEA so the
  FIDELIS comparator runs.
- A word is ONE `vocabulum` with parts; the tokenizer never decides the
  linguistics (enclitics, contractions are the dictionary's job).
- Genera and token genera are APPENDED, never reordered; analysis genera
  (stage 3) append after `numerus`.
- Unknown = finding, never a refused parse.
- The FORM layer runs before the sentence reader and is decoupled from
  it; its rules are DATA rows with a named cause and a measured count,
  never code paths; no rule holding = prosa (ambiguity → merging).
- A hand-built node takes its slot count from the registry
  (`loci_numerus`), never a literal.
- Vendored sources are VERBATIM (CRLF kept); the compiled table is
  committed and proven equal to the coction; its seal moves only with
  a named cause (`-scribere` prints the warning).
