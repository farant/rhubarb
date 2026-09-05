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
dictionary form. **T9 DONE**: `oratio/glossarium.stml` (hand-owned;
`sum` 77 forms, `Vergilius`, sixteen `ignotum-permissum` technical
terms) judged by `oratio/grammatica/glossarium.canon` (registered
`<glossarium>`; the stage-three class and accident vocabulary lives
there now); `oratio_glossarium` reads it and hashes forms by the same
fold as the table; `oratio_vocabularium_la_glossarium_ponere` makes it
the FIRST source of `quaerere` (`ORATIO_ANALYSIS_GLOSSARIUM`);
`quaere.sh` attaches it; gate `probatio_oratio_glossarium` (84; corpus
95.1 → 96.3 %). **T10 DONE — STAGE 2 COMPLETE**: `oratio_vocabula`
(identifiers from `build/nexus.tsv` split at `_` and case boundaries;
comments of `lib/*.c` + `silva/fontes/*.c` through silva's total lexer
and oratio's tree; one lookup per word, glossary first; status notum /
ambiguum / permissum / ignotum with sites and first site);
`./oratio/vocabula.sh [-symbola | -commenta | -omnia] [-machina]
[-omnes]`, `silva.vocabula(fons)`; gate `probatio_oratio_vocabula` (69,
corpus floors only). DAY ONE: identifiers 10,060 words, 5,515 unknown;
comments 14,205 / 5,736; unknowns = abbreviations, English, quoted C
keywords in generated grammar comments, vendor, house coinages WORDS
lacks (transponere, lexema, lista, xar). The runner compiles
`silva_token`/`silva_lexema` and writes `oratio/build/corpus_c.txt`.
Glossary pass I done from the report (77 entries: house coinages and
late Latin, house proper names, C/API/format names; common English and
abbreviations left unknown on purpose). The lint's corpus is the
HOUSE: `knotapel/` (English by decision), `vendor/`, `archivum/` are
excluded in one place (`ORATIO_VOCABULA_EXCLUSA`; `-omnes-viae` scans
all). Glossary entries carry `contextus="latinus | anglicus | ambo"`
(decision 26): the identifier/comment lint is the Latin context and
ignores English-only entries; abbreviations are KEPT (36 permitted,
contextus latinus). Identifier unknowns 2,958 of 7,553 words, PINNED
only falling in `probatio_oratio_vocabula` (a red = a new unknown
identifier word: glossary entry or rename, pin moved for a named
cause). T8b stays parked. **T15a DONE (warmup, decision 27)**: the
ENGLISH context. Moby Part-of-Speech vendored VERBATIM under
`oratio/vocabularium/en/` (no coction — the source is one table; seal
pinned), `oratio_vocabularium_en` (record law asserted per line,
unknown code letters counted: `cowardic\Ne`), gate
`probatio_oratio_vocabularium_en` (81). `oratio_vocabula_prosa` = md's
TEXTUS nodes only (code spans, fences, link targets, html, front matter
never reach the lookup); `oratio_vocabula_creare_anglice` judges
glossary-English → Moby → Latin table = status **LATINUM** (known
Latin in prose is never a finding); corpus = house + knotapel minus
vendor/ archivum/ generated (`ORATIO_PROSA_EXCLUSA`);
`./oratio/vocabula.sh -prosa`, `silva.vocabula('prosa')`. Day one:
62,524 words, 58 % unknown = regular inflections (-s -ed -ing 's) +
house English terms. **T15b DONE (same day)**: `ORATIO_REGULAE_EN` —
25 DATA rows (suffix, base substitution, gemination, minimum base,
required Moby codes, class, cause with the measured tally) +
`ORATIO_PRAEFIXA_EN` for hyphen compounds; `oratio_vocabularium_en_analysare`
= exact first, then rules in order, each analysis with rule, base,
class; `OratioVerbum.regula`; the fold maps U+2019 to `'`. Glossary
pass II (117 entries, ANGLICA PROSA section; Roman numerals ii–xxx
`numerale` both contexts; house Latin internamentum, diagnostica,
friatio, amalgama). Prose unknowns 58 % → 26.2 %; PROSE PIN = ceiling
on the unknown share (29 %) + floor on the known share (70 %), not a
count; identifier pin 2,950. Instrument prints a per-rule tally.
**T11 DONE — STAGE 3 OPENED**: seventeen `analysis-<classis>` genera
appended after `numerus` (registry 23 genera / 146 slots, seal
`87b35173`): common five slots first (lemma, lingua, fons, nativum =
ONE derived token, sensus), then the class's accidents as INDEX slots
into appended-only enumerations with title arrays (`OratioCasus` …,
`ORATIO_TITULI_CASUUM` …); `OratioClassis` in UD order with
`oratio_classis_genus`/`oratio_genus_classis`/`_titulus`/`_ex_titulo`;
class titles = the glossary canon's `classis` options (gate-guarded).
Existing STML bytes unchanged (the word's slots were reserved since
T1; absent lists are not written). Canon: 17 genus rules + 129 slot
rules intra their genus (generated once, kept by hand; ceiling 512).
Gates registrum (860) and canon (219, a hand-built analysis judged and
two mutations refused). **T12 DONE**: `oratio_partes_la` (one source
analysis → universal description: class, accidents as enum indices with
−1 = not given, lemma, `nativum` verbatim, sense, source, language; noun
kind N/L = proper, stem gender wins, DEP = deponens, FUT PASSIVE PPL =
gerundivum, subordinating conjunctions by data list, enclitics, glossary
by class name) and `oratio_partes` (`oratio_partes_annotare` after the
tree: one lookup per word, one `analysis-<classis>` node per
description with accidents placed by SLOT TITLE, `classes`/`linguae`
derived tokens, `ignotum` alone when nothing, write-once,
`patres_figere` last). Enum titles reconciled with the glossary canon
(genitivus, locativus before vocativus, I II III) and gate-guarded in
order. SUBSTRATE: `MateriaArborFrons.nodum_ornare` (node attributes on
write, ignored on read) mirrors `classes`/`linguae` onto `<vocabulum>`
so `vocabulum[classes~=verbum]` selects; selectio's `~=` underflow
fixed. `./oratio/arbor.sh <x.txt> -partes`. Gate `probatio_oratio_partes`
(171). Day one: Cicero 13,214 words, 114,876 analyses, 2.9 % unknown,
858 ms — pronoun packings dominate (`quis` 215). NEXT: **T13** CoNLL-U
reader + `probatio_oratio_oraculum` over CIRCSE + LLCT (COVERAGE
pinned, PRIMARY reported, per class); then T14 `verba.sh`,
`silva.Oratio`.

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
  a named cause (`-scribere` prints the warning). A one-table source
  (Moby) is read directly and its own seal is pinned.
- The prose lint is the ENGLISH context: TEXTUS nodes only; known Latin
  = `latinum`, never a finding; `ambiguum` never; the identifier pin
  is the Latin context's and a new house coinage in an API name moves
  it (glossary entry, pin moved for the cause).
- English morphology is DATA (`ORATIO_REGULAE_EN`): a row has a cause
  and a measured count, exact forms always precede rule analyses, rules
  reduce to MOBY bases only (glossary entries list their own plurals),
  and the prose pin is a SHARE ceiling, never a word count.
- Analysis genera: genus = PRIMUM + classis, never reordered; the five
  common slots come first on every genus; accidents are INDEX into
  enumerations that are APPENDED only (a value in the projection is a
  number); a slot-count enumerator is `_NUMERUS_LOCORUM` (the `numerus`
  accident owns `_NUMERUS`); every slot has ONE canon rule intra its
  genus; the registry seal moves only with the task as its cause.
- Enumeration titles = the glossary canon's options, in order (gate);
  the parser never annotates — `oratio_partes_annotare` is a separate
  pass with the dictionary supplied by the caller; accidents are placed
  by slot TITLE, never by a hand index table; the `classes`/`linguae`
  ATTRIBUTES are mirrors of the slots (reader ignores them); `casus`
  and `nomen` are latina macros — fields are `casus_grammaticus`, a
  parameter is never `nomen`.
