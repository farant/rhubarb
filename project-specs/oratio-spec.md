# oratio — natural-language parser on materia, with cross-language word classification (spec v1, 2026-09-04)

> From the interview of 2026-09-04 (`oratio-interview.md`, 24
> questions). Stance, ladder and names were agreed in conversation
> before the interview; the interview fixed the choices below. A v2
> with codebase-specific planning follows the inline codebase pass.

## 0. Identity

`oratio` parses PROSE — any natural language, plain text bytes — into
a materia tree under the byte law (every byte owned by one token in
one slot, parse→emit byte-exact), projects it to STML like css and md,
and carries on top of the tree a CLASSIFICATION layer: for every word,
an ordered list of candidate analyses (lemma, language, native tag from
the source dictionary, accidents), projected into one universal set of
classes named in Latin, so that a selector such as
`vocabulum[classes~=verbum]` finds a possible verb in any language.

Consumers, in the order they arrive: the Latin vocabulary lint over
the codebase's identifiers and comments; a measured successor to
`sententia_fissio`; annotated words for scripts (`silva.Oratio`);
resolution of ambiguity by context as an STML program; lemmas for the
tabularium's full-text search. Semantics (meaning, ontology) is
explicitly a later, language-independent layer — likely an extension
of natura — and is out of this spec.

Languages: Latin and English now; any natural language is fair game
later (lapide.org already carries 31); scripts without word spaces
(Chinese, Japanese, Thai) and right-to-left scripts wait for the
rhubarb unicode story. Sabaw is not a target (it is a tag library, not
a language to tokenize).

## 1. Decisions (the interview, verbatim in substance)

| # | Decision |
|---|---|
| 1 | Input unit = plain text bytes, any file; the corpus gate feeds the TEXT of every markdown paragraph (extracted through the md parser); composition with md is a later stage. |
| 2 | Word joins: hyphenated compounds, contractions, enclitics are ONE `verbum` owning all bytes, with a list of parts; the tree never decides the linguistics. |
| 3 | Orthography: bytes untouched; LOOKUP folds to a key (u/v and i/j merged, macrons and ligatures stripped, case folded); LEMMAS are classical i/v as Whitaker spells them. |
| 4 | Unicode floor: ASCII letters/digits by table; any multi-byte UTF-8 run is a letter run; no folding or classes for them yet. |
| 5 | Analyses: one genus per universal class, each with exactly its accidents as INDEX slots; a word's analyses list is mixed-genus. |
| 6 | Universal set = UD's 17 UPOS classes, named in Latin; classical categories (participle, gerund, supine) are accidents, not classes. |
| 7 | Candidates ordered by source preference; primary = first; nothing dropped; resolution may reorder, never delete. |
| 8 | Dictionary truth = vendored upstream files (FONTES.md, pinned bytes); compiled binary table is GENERATED and sealed; a gate proves compile(source) == committed binary. |
| 9 | Unknown word = `ignotum` finding; no morphological hypotheses in stages 1–4 (a later gated stage with an explicit `coniectura` flag). |
| 10 | One hand-written house-wide glossary `oratio/glossarium.stml` (canon-judged) is the highest-priority source; natura's genus glossae stay documentation. |
| 11 | Licenses: WORDS and Moby (public domain) vendored; treebanks: BY-SA/PD vendored verbatim as fixtures, NC ones fetched into build/ and reported, never pinned; nothing derived from a treebank is ever shipped. |
| 12 | Identifier lint = EXISTENCE (every underscore part known in dictionary, glossary, or the allowed technical terms list) + a report of classes and forms; no form rules yet. |
| 13 | `sententia_fissio`: measured against oratio over the corpus, then reimplemented over oratio behind the same API. |
| 14 | Language: every word looked up in every dictionary; `lingua` is a bit on each analysis; document language is a later derived summary. |
| 15 | Home: `oratio/` top-level in the md/css layout, registered in pythonica's gate tables in the birth commit. |
| 16 | Python face: `silva.Oratio(via | text)`, query-only; Prosa gains `.sententia(n)` by delegation. |
| 17 | First search consumer: candidate lemmas as an extra indexed column of the tabularium/forum FTS. |
| 18 | Envelope: whole markdown corpus classified under ~10 s, single file instant; computus twin pins per-file cost. |
| 19 | Oracle metric: both reported; 'gold class among candidates' PINNED until stage 5; primary agreement printed from birth. |
| 20 | Resolution (stage 5) lives in STML pattern arms over the projection; C only for the substrate; measured per rule. |
| 21 | WORDS meanings compiled and exposed as a derived `sensus` string per analysis (the source's gloss verbatim, not a semantic layer). |
| 22 | The C89 comments corpus (lib/, silva/) is a second Latin corpus: read through silva's tree, unknown words reported there too. |

## 2. Doctrine

**Moderate realism, applied.** Classes are real because words signify
differently (substance, action, quality); the registry abstracts them
from instances and does not invent them. Three rules follow:

1. A class is EARNED by function: it enters the universal registry only
   when the corpus and the oracle show words doing something no
   existing class captures. Natura's discipline ("is there a genus for
   X?") applies to classes.
2. Edge cases resolve by ANALOGY, not by new boxes: a participle is a
   verb form functioning adjectivally and carries both readings with
   the primary marked; the candidate list expresses it.
3. The scheme is CHOSEN, not surveyed: UD's classes are the values
   because the oracles speak them; the Latin names and the accidents
   are the house's own vocabulary; no other theory of language owes or
   is owed a seat.

**Ambiguity law.** Every layer stores what it knows as a LIST: parts
of a word, analyses of a word, classes of an analysis, languages of a
word. Nothing below resolution deletes a candidate. Ambiguity that
survives every layer is a fact about the text, reported as such.

**Finding, not failure.** An unknown word, an unmatched ending, a
form the dictionary lacks — each is a row in a report (`citata`'s
shape), never a refused parse.

## 3. Stage 1 — the tree (`oratio_arbor`)

**Registry (`oratio_registrum`, hand-written like md's).** Genera:
`documentum` (paragraphi LISTA_NODUS, finis TOKEN), `paragraphus`
(sententiae LISTA_NODUS, finis), `sententia` (vocabula LISTA_NODUS,
finis), `vocabulum` (partes LISTA_TOKEN — the surface pieces: letters,
hyphens, apostrophes, digits; `analyses` LISTA_NODUS — empty in stage
1; summary slots filled by stage 3), `interpunctio` (signum TOKEN),
`numerus` (crudum LISTA_TOKEN), `spatium` (crudum LISTA_TOKEN), and
the analysis genera of §5 appended later. Blank lines separate
paragraphs; whitespace and newlines are TOKENS (`or-spatium`,
`or-linea` terminator, like md), never trivia.

**Lexicon (`oratio_lexicon`, prefix `or-`).** `LITTERAE` (a run of
ASCII letters and/or multi-byte UTF-8 sequences), `DIGITI`, `HYPHEN`,
`APOSTROPHUS`, `INTERPUNCTIO` (one punctuation byte or a run of the
same), `SPATIUM`, `LINEA` (terminator, `\n` | `\r\n`, munus LINEA so
the FIDELIS comparator runs), `SIGNUM` (any other byte), `DERIVATUM`.

**Tokenizer law.** Scanners return extents; the parser decides
ownership (md's law). A word = a maximal run of letters joined by
single hyphens or apostrophes between letters (`well-known`, `don't`,
`Fran's`) — the join is a PART; a run of letters ending in an enclitic
is still one word (the dictionary splits `-que` later). A period
between letters (`e.g.`) joins as a part; a trailing period is
punctuation unless the word is in the abbreviation table (stage 1
carries a small built-in table; the dictionary supersedes it in
stage 2). Digits with separators (`1,024`, `3.14`, `XIV`) are
`numerus` (Roman numerals become `numerus` only through the
dictionary; in stage 1 they are words).

**Sentence boundary (v1).** A sentence ends at `.`, `!`, `?` (with any
closing quotes/brackets) followed by whitespace and then a capital
letter, a digit, an opening quote, or end of paragraph — unless the
preceding word is an abbreviation or a single capital letter
(initial). A paragraph end always ends a sentence. Everything not
covered stays in the current sentence: the splitter never invents a
boundary it cannot justify.

**Gates.** `probatio_oratio_registrum`, `_lexema` (scanners +
positions), `_arbor` (structure fixtures: joins, abbreviations,
quotes, paragraphs, CRLF) + CORPUS: every paragraph text of every
tracked `*.md` (extracted through md's tree) parse→emit byte-exact,
and every plain text fixture; `_stml` (STRUCTURALIS + FIDELIS round
trip, canon `oratio/grammatica/oratio.canon` hand-written, seal
pinned); `_totalitas`; `_computus` (golden over snapshots);
`_fissio` (sententia_fissio vs oratio boundaries over the corpus:
disagreements REPORTED with file:line, count published). Every gate
born red by a planted fault.

**Instruments.** `oratio/arbor.sh <x.txt>` (projection),
`oratio/sententiae.sh <x.txt>` (one sentence per line with byte
extents), `oratio/computus.sh`.

**As built (T1, 2026-09-04).** No `spatium` genus: whitespace rides
as `cauda` LISTA_TOKEN slots on the preceding `vocabulum`,
`interpunctio` or `numerus` under STML's possessor law (up to the first
newline inclusive), the remainder of a blank-line run in
`paragraphus.cauda`, leading whitespace in `documentum.praefixa` and
`paragraphus.praefixa`; `sententia` has no whitespace slots. Lexicon
`or-` (12 genera): FINIS, LINEA, LINEA_CR, SPATIUM, LITTERAE, DIGITI,
HYPHEN, APOSTROPHUS, PUNCTUM (its own genus: abbreviations and
sentence ends hang on it), INTERPUNCTIO, SIGNUM, DERIVATUM. Registry 6
genera / 16 slots; `vocabulum` carries `analyses`, `classes`, `linguae`
from birth (absent until stage 3). Gate `probatio_oratio_registrum`
(130 assertions, round trip of "a\n").

**As built (T2, 2026-09-04).** `oratio_lexare` is a whole-stream lexer
(prose needs no container context). Unicode floor as built: bytes at or
above 0x80 join letter runs unvalidated, with two exceptions found on
first contact with real text — NBSP is SPATIUM, and the General
Punctuation block U+2000–U+206F is INTERPUNCTIO (thin spaces SPATIUM,
U+2019 between letters APOSTROPHUS). Joiners by neighbouring bytes;
PUNCTUM only for a lone period; other punctuation one token per byte
except same-byte runs. Corpus: every tracked markdown file byte-exact
(1,134 files, 6.7M tokens, 0.7 s).

**As built (T3, 2026-09-04).** Two passes per paragraph: elements with
tails, then sentences with one element of lookahead. Numbers are
"digits first" and may take a letter suffix; words are "letters first"
and may take digits; a period between letters without space is a word
part (e.g, i.e); the trailing period is its own element and the
abbreviation table is consulted for the word before it. A byte at or
above 0x80 counts as a capital for the boundary decision (splitting
preferred until real case exists). Control bytes are whitespace-class.
Corpus gate as built: every markdown paragraph's extent from md's tree
directly (no STML in the loop): 81,844 paragraphs byte-exact, 155,328
sentences, 1,918,204 words, 1.45 s.

**As built (T4, 2026-09-04).** Projection consilium `oratio_stml` (md's
origin hook, grammar `oratio`); canon `oratio/grammatica/oratio.canon`
with seal `b27fe13a`, 6 genera / 16 slots / 12 tokens, `signum`
admitting `or-hyphen` and `or-apostrophus` (a joiner after a number's
letter suffix has no word to join — found by the corpus judgment).
Parent pointers fixed after parsing. Both corpus gates SAMPLE every
fifth file because the word-per-node tree makes the STML round trip
of the whole corpus cost 40 s (byte gate 1.5 s); `ORATIO_CORPUS_TOTUS=1`
runs all of it. The derived-token channel is exercised from stage 1.

## 4. Stage 2 — the Latin dictionary (`vocabularium`)

**Sources vendored under `oratio/vocabularium/`** with `FONTES.md`
(URL, version, license, byte counts pinned by a gate): Whitaker's
WORDS — `DICTLINE.GEN` (stems, part of speech, declension/
conjugation, gender/kind, age/area/geography/frequency codes,
meaning), `INFLECTS.LAT` (endings by part, declension, variant,
case/number/gender or person/number/tense/voice/mood), `ADDONS.LAT`
(prefixes, suffixes, tackons/enclitics), `UNIQUES.LAT`.

**Compiled table (`oratio/build/vocabularium_la.bin`, committed as
`oratio/vocabularium/la.bin`).** A build step (`oratio/vocabularium.sh
-coquere`) parses the sources and emits one binary table: stems keyed
by the folded orthography (decision 3), inflection rows keyed by
ending, addons, and the meaning strings; a seal (hash of the table)
pinned in the gate; the gate proves compile(sources) == committed
table byte-for-byte (`probatio_oratio_vocabularium`). The house
serialization (`nuntium`) is the candidate carrier; the v2 pass
decides.

**Lookup (`vocabularium_la_quaerere(forma) → analyses`).** Fold the
form; for every split into stem + ending, an ending row must exist
whose part/declension/variant match a stem row; enclitics (`-que`,
`-ne`, `-ve`) are tried as tackons first, yielding TWO analyses
(host and enclitic) attached to the word's parts; prefixes/suffixes
per ADDONS. The result is an ordered list: WORDS' own order (age and
frequency codes) first, then stem order. Each analysis carries the
native record verbatim (the DICTLINE codes), the lemma (WORDS'
dictionary form), the accidents, and `sensus` (the meaning string).

**Glossary (`oratio/glossarium.stml`).** Hand-written, canon-judged
(`oratio/grammatica/glossarium.canon`): entries with lemma, lingua,
universal class, accidents, note; plus the ALLOWED TECHNICAL TERMS
list (`offset`, `index`, `token`, `byte`, …) as entries of class
`ignotum-permissum`. Highest-priority source; a glossary hit is an
analysis like any other, marked `fons="glossarium"`.

**Consumer: the vocabulary lint.** `oratio/vocabula.sh [-symbola |
-commenta | -omnia]`: symbols from `build/nexus.tsv` definitions split
at `_` (9,867 distinct words today), and comments from every `lib/`
and `silva/` source through silva's tree (decision 22), each word
looked up; report per word: known (with primary class and lemma),
ambiguous (candidate count), UNKNOWN (file:line of first use); summary
per directory. Python: `silva.vocabula()` on the shape of `citata`.
Report first; a gate that pins the unknown count "only falling"
comes when the report is read.

## 5. Stage 3 — annotated words (`partes`)

**Universal classes** (registry `partes_registrum`, one genus per
class, appended never reordered; UD UPOS in parentheses):
`substantivum` (NOUN), `nomen-proprium` (PROPN), `verbum` (VERB), `auxiliare` (AUX), `adiectivum` (ADJ),
`adverbium` (ADV), `pronomen` (PRON), `determinans` (DET),
`adpositio` (ADP), `numerale` (NUM), `coniunctio-coordinans`
(CCONJ), `coniunctio-subordinans` (SCONJ), `particula` (PART),
`interiectio` (INTJ), `symbolum` (SYM), `interpunctio` (PUNCT),
`ignotum` (X). The tree's WORD genus is `vocabulum` (§3), so `verbum`
is free to name the verb class; analysis genera are `analysis-verbi`,
`analysis-substantivi`, and so on.

**Analysis genera** `analysis-<classis>` with exactly their
accidents as INDEX slots, values from small enumerations shared where
UD shares them: `casus` (nominativus … ablativus, locativus,
vocativus), `numerus` (singularis, pluralis, dualis), `genus`
(masculinum, femininum, neutrum, commune), `persona` (I II III),
`tempus` (praesens, imperfectum, futurum, perfectum, plusquamperfectum,
futurum-exactum, praeteritum), `modus` (indicativus, subiunctivus,
imperativus, infinitivus), `vox` (activa, passiva, deponens), `forma-verbi`
(finitum, infinitivum, participium, gerundium, gerundivum, supinum),
`gradus` (positivus, comparativus, superlativus), `declinatio`,
`coniugatio`. Common slots on every analysis: `lemma` (TOKEN, derived),
`lingua` (INDEX: latina, anglica, …), `fons` (INDEX: vocabularium-la,
vocabularium-en, glossarium), `nativum` (TOKEN, derived: the source's
own code verbatim), `sensus` (TOKEN, derived, optional).

**Mapping tables per source** (`partes_la.c`, `partes_en.c`): WORDS
part codes → class + accidents (N→substantivum with declension/gender;
V→verbum with conjugation; VPAR→verbum forma-verbi=participium;
ADJ, ADV, PREP→adpositio with the governed case as an accident, CONJ
→ coordinans/subordinans by a small list, PRON, NUM, INTERJ, PACK/
TACKON/PREFIX/SUFFIX → parts of the host analysis). One native code may
map to more than one class (a list), which produces more than one
analysis.

**The annotated word.** On `vocabulum`: `analyses` LISTA_NODUS in source
order; summary derived tokens `classes` (space-separated class names
in order, first = primary) and `linguae`. In the STML projection these
are attributes-shaped wrappers, so `vocabulum[classes~=adiectivum]` and
`vocabulum[linguae~=latina]` work in `selectio.h` (word match `~=`).

**Oracle (`probatio_oratio_oraculum`).** A CoNLL-U reader (one form:
`ID FORM LEMMA UPOS XPOS FEATS HEAD DEPREL DEPS MISC`, comments,
blank-line sentence separators; multiword tokens ranges skipped by
rule). For each token: does any candidate's class equal the gold
UPOS (COVERAGE, pinned); does the primary equal it (PRIMARY,
reported); lemma agreement reported. Per-class table published.
Vendored: English EWT (BY-SA) and one BY-SA Latin treebank (to be
chosen in the v2 pass after license verification); NC Latin
treebanks fetched by `oratio/oraculum.sh -petere` into build/ and
reported only.

**Instruments.** `oratio/verba.sh <x.txt> [-machina]` (annotated words
TSV: via, byte extent, form, classes, primary lemma, lingua, count of
analyses), `oratio/arbor.sh` shows the analyses inline.

**Python.** `silva.Oratio(via_or_text)`: `.sententiae()` (extents),
`.vocabula(classis=None, lingua=None, ignota=False)`, `.analyses(vocabulum)`,
`.ignota()`; `Prosa.sententia(n, intra=)` delegates.

## 6. Stage 4 — English (`vocabularium_en`)

Moby Part-of-Speech (`mobypos.txt`, public domain): one line per
entry, form × codes (N noun, p plural, h noun phrase, V verb
participle, t transitive, i intransitive, A adjective, v adverb, C
conjunction, P preposition, ! interjection, r pronoun, D definite
article, I indefinite article, o nominative) in preference order.
Compiled like Latin (folded key = lower case). A light MORPHOLOGY in
C: regular plurals, `-ed`, `-ing`, `-s` third person, `-er`/`-est`,
an irregular-forms table (vendored from a PD list or hand-written);
each rule yields analyses marked with the rule as `nativum`. Mapping
Moby codes → classes. Oracle: UD English EWT (COVERAGE pinned).

## 7. Stage 5 — resolution by context

An STML program `oratio/partes/resolutio.stml` run through the
command layer over the projection: pattern arms that REORDER a
word's analyses (never delete) by agreement (adjective ↔ noun in case,
number, gender; determiner ↔ noun), adjacency (adposition governs the
following case), and sentence position. Each rule is measured by the
oracle's PRIMARY count; a rule that cannot be expressed in the
vocabulary is a finding for the STML exploratory design. The primary
pin starts here.

## 8. Stage 6 — search

Candidate lemmas of every word become an extra indexed column of the
tabularium/forum FTS (every candidate indexed: recall over precision),
so a Latin query matches inflected forms without prefix wildcards.
Recall change measured on a fixed query set.

## 9. Gates (every one red at birth by a planted fault)

| gate | proves |
|---|---|
| registrum | titles, contiguity, materia round trip |
| lexema | scanners, positions, UTF-8 runs, CRLF |
| arbor | structure fixtures + CORPUS byte-exact (md paragraph texts + plain fixtures) |
| stml | STRUCTURALIS + FIDELIS, canon seal, corpus judged |
| totalitas | random / mutated / nested input never crashes |
| computus | golden costs over snapshots |
| fissio | sententia_fissio vs oratio: disagreements reported, count published |
| vocabularium | compile(sources) == committed table; form fixtures → expected analyses; folding |
| vocabula | the lint report over identifiers and comments runs; counts published |
| partes | mapping tables total (every native code maps), analysis genera shapes judged |
| oraculum | CoNLL-U reader; COVERAGE pinned, PRIMARY reported, per class |
| pythonica | Oratio queries, Prosa.sententia |

## 10. Measurements published, not asserted

Sentences, words, unknown words per language over the markdown
corpus; identifier words known/unknown/ambiguous; comment words the
same; analyses per word (mean, max); dictionary table sizes and load
time; per-file parse and classify cost (computus); fissio
disagreements by kind.

## 11. Order and size

Stage 1 (registry, lexicon, tokenizer, sentences, corpus gate,
projection, canon, totality, computus, fissio comparison) ≈ 6 tasks.
Stage 2 (vendor + compile + lookup + glossary + lint report) ≈ 4.
Stage 3 (classes, analysis genera, mapping, annotation, oracle,
instruments, Python) ≈ 5. Stage 4 ≈ 3. Stage 5 ≈ 3 (program, per-rule
measurement, primary pin). Stage 6 ≈ 2. Each stage ships value alone;
the ladder can stop after any rung.

## 12. Out of scope (named)

Semantics and ontology (a later natura-adjacent layer); dependency
syntax; unicode tables (rhubarb's own story); scripts without word
spaces and right-to-left scripts; sabaw; committed NC treebank data;
morphological hypotheses for unknown words (a later flagged stage);
form rules in the lint (report first).

## 13. Questions for the v2 codebase pass

Exact licenses of the Latin treebanks (Perseus, PROIEL, ITTB, LLCT,
UDante) and which BY-SA one to vendor; WORDS file formats (fixed
columns of DICTLINE.GEN / INFLECTS.LAT) and their size after
compilation; whether `nuntium` is the right carrier for a
tens-of-MB table or a flat sealed record file is; `sententia_fissio`'s
API and consumers; the tabularium FTS schema for the lemma column;
how silva exposes comments for the comments corpus; the md paragraph
text extraction for the corpus gate (Prosa/extenta or a C helper).

---

## v2 — codebase pass (2026-09-04, inline, no subagents)

### A. Sources and licenses, verified

| source | what | license | size | vendoring |
|---|---|---|---|---|
| Whitaker's WORDS (`github.com/mk270/whitakers-words`) | `DICTLINE.GEN`, `INFLECTS.LAT`, `ADDONS.LAT`, `UNIQUES.LAT` | "Permission is hereby freely given for any and all use of program and data" (author) | DICTLINE ~39k records | vendor verbatim, `oratio/vocabularium/la/` |
| Moby Part-of-Speech (Gutenberg #3203, `files/3203/files/mobypos.txt`) | 233,356 entries, form × codes in preference order | public domain (US) | 3.1 MB | vendor verbatim, `oratio/vocabularium/en/` |
| UD_Latin-CIRCSE | Seneca (tragedies), Tacitus (Germania); classical | CC BY-SA 4.0 | 1,972 sent / 28,287 tok | VENDOR (oracle, classical) |
| UD_Latin-LLCT | early-medieval Tuscan charters; non-standard Latin | CC BY-SA 4.0 | 9,023 / 242,391 | VENDOR (oracle, volume) |
| UD_Latin-ITTB | Index Thomisticus: Aquinas and 61 related authors | CC BY-NC-SA 3.0 | 26,977 / 450,480 | FETCH into build/, report only |
| UD_Latin-PROIEL | Vulgate NT, Caesar, Cicero, Palladius | CC BY-NC-SA 3.0 | 18,689 / 205,566 | fetch, report only |
| UD_Latin-Perseus | Cicero, Vergil, Ovid, Sallust, Jerome | CC BY-NC-SA 2.5 | 2,273 / 28,868 | fetch, report only |
| UD_Latin-UDante | Dante's Latin works | CC BY-NC-SA 3.0 | 1,723 / 55,286 | fetch, report only |
| UD_English-EWT | weblogs, newsgroups, emails, reviews, answers | CC BY-SA 4.0 | 16,622 / 251,491 | VENDOR (oracle) |

Decision from the pass: the committed Latin oracle is CIRCSE + LLCT
(both BY-SA); the pin is over both files summed, and the per-file
counts are published. ITTB is the one to WATCH (Aquinas, 450k tokens,
scholastic Latin like the house's own), fetched by `oratio/oraculum.sh
-petere` and reported, never pinned, never committed.

**WORDS formats.** `INFLECTS.LAT` is whitespace-separated, one record
per line, comments prefixed `--`: part, declension, variant, then the
part's accidents (N: case number gender; V: tense voice mood person
number; ADJ: case number gender degree; ADV: degree; PREP: governed
case), stem key, ending length, ending, age, frequency — e.g.
`N 1 1 NOM S C 1 1 a X A`, `V 1 1 PRES ACTIVE IND 1 S 1 1 o X A`,
`ADJ 1 1 NOM S M POS 1 2 us X A`. `DICTLINE.GEN` is FIXED-WIDTH: four
stem fields of 19 columns (76), then the part-of-speech record (part,
declension, variant, gender/kind as the part requires), then the five
single-letter codes age/area/geography/frequency/source, then the
meaning to end of line (e.g. `a … N 9 8 M T … X X X D G`, `ab … PREP
ABL … X X X A O`). Exact column offsets are transcribed from the Ada
declarations (`dictionary_package.ads`) at vendoring time, and the
compile step ASSERTS them on every record (a record that does not fit
is a finding that stops the compile — the pinned byte count guards
the source, the assertion guards the reading).

**Moby codes** (`mobypos.txt`, one entry per line, form `×` codes):
N noun, p plural, h noun phrase, V verb (participle), t transitive
verb, i intransitive verb, A adjective, v adverb, C conjunction, P
preposition, ! interjection, r pronoun, D definite article, I
indefinite article, o nominative. Mapping to classes: N/p/h/o →
substantivum, V/t/i → verbum, A → adiectivum, v → adverbium, C →
coniunctio (coordinans by default; the subordinating list decides),
P → adpositio, ! → interiectio, r → pronomen, D/I → determinans.

### B. Seams found

- **Materia client pattern** (css, md): `X_registrum.{c,h}` hand-written
  tables with named slot enums, `X_lexicon.{c,h}` with a prefix
  (`or-`), `X_stml.c` consilium with the origin hook for derived
  tokens (copy `md_stml.c` — the derived-token position law is
  exactly what analyses need), `X/compile_probationes.sh` compiling
  materia's six sources and root deps (copy `md/compile_probationes.sh`;
  selectio.o and stml.o are already in its RADIX_FONTES), runner
  registered in pythonica's `PORTAE`, `SUITAE`, `FORMAE` and the
  mensor prefix table (four edits in `silva.py`, as md did in A2).
  The extenta/census instruments are copied with their `.sh`.
- **The corpus gate's text.** Every markdown paragraph's bytes come from
  `md_extenta_quaerere(…, "paragraphus")` — no new extraction code;
  the gate links md's objects (one more `for` in the runner). The
  paragraph bytes include inline markup (`*em*`, backticks), which
  oratio tokenizes as punctuation; that is correct for the byte law
  and harmless for sentences.
- **`sententia_fissio` API to preserve** (`include/sententia_fissio.h`):
  `sententia_fissio(chorda, Piscina*) → SententiaeFissioFructus`
  (copies), `sententia_numerare(chorda)`, `sententia_fissio_indices(…)
  → SententiaIndicesFructus` (byte offsets, exclusive end), and the
  lazy `SententiaIterator` (`sententia_iterator_creare` …). Consumer:
  `lib/sententia_paginarium.c` (uses `_indices`). Its comment lists
  the cases it handles: abbreviations (Mr., Dr., etc.), straight and
  curly quotes, decimals ($3.50), ellipsis, dialogue attribution
  ("Hello!" she said.), Roman numerals (George II.) — these are the
  stage-1 fixture list, and the fissio gate compares indices.
- **The comments corpus.** Silva's token genera
  `SILVA_LEX_COMMENTUM_CLAUSUM` / `_LINEA` (`silva/fontes/silva_token.h`).
  The lint reads each `lib/*.c` and `silva/fontes/*.c` through
  `silva_parsare` and walks comment tokens (or, cheaper for v1, over
  the STML of `./silva/arbor.sh -nudum` with selectio on the comment
  tag) — 33,810 block-comment openers in lib/ + silva/fontes today.
  Comment bodies pass through oratio with the `*` margins tokenized as
  punctuation.
- **Identifiers.** `build/nexus.tsv` `sedes` rows (the whole symbol
  table, read once as `citata` does), split at `_`, lower-cased:
  9,867 distinct words of three or more letters today.
- **Search.** `gesta/fontes/gesta.c`: `res_fts` is SQLite FTS5 over
  `titulus, corpus, notae` with `tokenize='unicode61 remove_diacritics
  2'`; `gesta_quaerere(mundus, textus, genus, status, pn)`. Stage 6 =
  a fourth indexed column `lemmata` filled at write time from oratio's
  candidate lemmas of titulus + corpus + notae, and `quaerere`
  matching it too. The tabularium resident links oratio's objects.
- **Dictionary carrier.** `nuntium.h` is a varint/length-delimited
  stream writer/reader (protobuf-shaped): fine as the SEALED FILE
  format for records (stems, inflections, addons, meanings). Lookup
  needs a hash by folded key: build it at load with `tabula_dispersa.h`
  from the stream (tens of thousands of records; load time is a
  computus column and must stay well under the single-file budget).
  No mmap in v1.
- **Selectors.** `selectio.h` supports `[attr~=word]`; the projection
  writes `classes` and `linguae` as attributes on `vocabulum`'s
  wrapper — confirm in stage 3 that the materia writer can emit a
  derived token as an ATTRIBUTE rather than a child (today derived
  tokens are child elements with `f="1"`); if not, `classes` is a
  child element and the selector becomes `vocabulum > classes` text
  matching, which selectio cannot do — then a small `partes` attribute
  emission hook is the stage-3 substrate change (the second ever after
  md's fourteen lines).

### C. Codebase-specific plan (stages → tasks)

**Stage 1 (tree).** T1 `oratio/fontes/oratio_registrum.{c,h}` +
`oratio_lexicon.{c,h}` + runner + pythonica registration + gate
`probatio_oratio_registrum` (planted: a slot title). T2
`oratio_lexema.{c,h}` scanners (letters incl. UTF-8 runs, digits,
hyphen/apostrophe joins, punctuation runs, spatium, linea) + gate
(positions, CRLF, UTF-8). T3 `oratio_arbor.{c,h}` paragraphs,
sentences (boundary v1 + abbreviation table), vocabula with partes,
numerus; fixtures from sententia_fissio's comment list; CORPUS gate
over md paragraph extents + plain fixtures. T4 `oratio_stml.c`,
`oratio/grammatica/oratio.canon`, `probatio_oratio_stml` (STRUCTURALIS
+ FIDELIS), `oratio/arbor.sh`. T5 totalitas + computus twin
(`oratio/computus.sh`, golden). T6 `probatio_oratio_fissio`: indices
from both, disagreements reported by kind, count published;
`oratio/sententiae.sh`.

**Stage 2 (Latin).** T7 vendor WORDS + `FONTES.md` + byte pins;
`oratio/vocabularium.sh -coquere` → `oratio/vocabularium/la.bin`
(nuntium records), seal, gate compile==committed. T8
`vocabularium_la.{c,h}`: load (hash by folded key), fold, lookup
(stem+ending, tackons, addons), ordering; fixture forms → analyses.
T9 `oratio/glossarium.stml` + canon + loader; allowed technical terms.
T10 `oratio/vocabula.sh` + `silva.vocabula()`: identifiers (nexus.tsv)
and comments (silva tokens) reports; counts published.

**Stage 3 (partes).** T11 `partes_registrum` (17 classes, analysis
genera with accidents), appended to the oratio registry; canon rules.
T12 `partes_la.c` mapping + annotation pass (`analyses`, `classes`,
`linguae`) + projection attributes (substrate check above). T13
CoNLL-U reader + `probatio_oratio_oraculum` over CIRCSE + LLCT
(COVERAGE pinned, PRIMARY reported, per class); `oratio/oraculum.sh
-petere` for NC treebanks. T14 `oratio/verba.sh`, `silva.Oratio`,
`Prosa.sententia`.

**Stage 4 (English).** T15 vendor Moby + compile + `vocabularium_en`
+ morphology rules + irregulars. T16 `partes_en.c` + EWT oracle.

**Stage 5.** T17 `oratio/partes/resolutio.stml` + runner through the
command layer + per-rule oracle deltas. T18 primary pin.

**Stage 6.** T19 `lemmata` column in gesta FTS + tabularium write path.
T20 recall measurement on a fixed query set.

About 20 tasks; T3 (sentences) and T8 (Latin lookup) are the hard
ones. Re-entry: this file, `oratio-interview.md`, the ledger parcum
(created with this commit).
