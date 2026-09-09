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

**Decided after stage 1 (Fran, 2026-09-04).** (23) A TEXT-FORM layer
sits before the sentence reader, decoupled from it: a pass over each
paragraph decides `prosa` | `versus` | `titulus` | `tabula` | `index`
from the tree's own evidence (lines against sentences, line lengths
and their variance, capitalized line starts, unpunctuated line ends)
and stores it as an INDEX slot `forma` on `paragraphus` (the push-down
law); the reader consults the slot (a line is the unit in verse and
tables, a heading is one unit, prose keeps the current rule). Special
cases accumulate as DATA with a measured count, never as code paths.
(24) The ergonomic criterion for ambiguous cases is "reading a text one
sentence at a time": a false merge is a mild annoyance, a false split
breaks a thought, so ambiguity resolves toward MERGING — oratio's
"never invent a boundary it cannot justify" is the same bias. (25)
Oracles for segmentation: the UD treebanks (sentence-segmented; CIRCSE
= Seneca's tragedies = gold verse), plus a small hand-judged set from
the fixtures encoding the reader criterion, pinned only rising; the
fissio comparison stays a report.

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

**The sudoku model (Fran, 2026-09-08; decisions 36–42).** Text
interpretation has gone two ways that each paint into a corner: the
STATISTICAL model (guess by a prior; no reasons, uniform noise,
retrained forever) and the TOTAL model (every sentence fully
decipherable in isolation; treebank conventions become law and
category errors leak down into the classification layer). Oratio is a
third thing: a constraint solver over candidate lists, the way a
sudoku is solved — never a guess, only a narrowing, and when one
direction stops moving, a perpendicular one.

- **36 Solid layers.** A question at a layer is finite and yes/no
  (has this form a verb reading?), never a fudge. The aim at every
  layer is 100 % COVERAGE (the answer is among the candidates) with
  ambiguity accepted; 100 % primary at one layer is overfitting and
  corrupts the layer with category error.
- **37 Elimination only over a complete grid.** A candidate is
  demoted only when it contradicts a constraint, and a contradiction
  is sound only when every cell it reads is on the board with all its
  candidates. This is why demotion failed three times on 2026-09-07:
  unknown words were not on the board. An unknown word is a cell with
  ALL candidates (`analysis-ignoti`), never a missing cell.
- **38 Hard and soft.** A hard constraint (case mismatch, genus
  incompatibility) may eliminate; a soft one (class prior,
  plausibility, a dictionary's code order) only ORDERS, in the last
  tier, and may never block a constraint. Every forced decision
  carries its reason.
- **39 Plateau → perpendicular dimension.** When a direction stops
  moving the number, add a dimension that narrows the same cells from
  another side. Dimensions: morphology, language, agreement and
  adjacency, clause counting (one finite verb per clause: a hidden
  single), the text form, document consistency (one class per form
  per document), the SEMANTIC layer (natura genera and their
  relations as umbrae of their own), and the ZOOM level — a sentence
  may need its paragraph, a paragraph its work: the contextualisation
  a reader does. A sound constraint stays sound when a dimension is
  added.
- **40 A spectrum, reported as a profile.** Every cell at every scale
  ends FORCED | ORDERED | OPEN; the profile across the tower is the
  text's decipherability and names the next dimension. Reading 80 %
  of a sentence well is the value: never throw it away, never guess
  the rest. 100 % is not honestly possible (malformed text, poetry).
- **41 The residual is a finding of two kinds.** UNSOLVED (a
  constraint is missing) or UNSOLVABLE (genuine ambiguity — a poem is
  a coordinate, given here as a discrete, inspectable lattice of
  readings rather than a vector). A soft-constraint violation that
  survives every narrowing is a FIGURE (metaphor), not an error.
  Malformed text degrades by open cells; the totality gate guarantees
  the tree beneath.
- **42 Umbra is the recurring pattern.** At every layer a node
  declares what it expects from the schema of its genus (a class, a
  case; a genus relation), a neighbour fills it, a filled slot is
  evidence, an unfilled one a finding — ONE executor law shared by
  word class, syntax and meaning. A bound umbra is a dependency edge
  (dependency syntax stays out of scope as a target and is welcome as
  a by-product). Outer-scale cells are high-leverage: a work-level
  class is forced only on lopsided evidence (the language census),
  otherwise it stays a list at its own scale; every scale gets its
  own oracle (hand-judged fixtures pinned only rising, as the form
  layer's).

Implications for the build, noted 2026-09-08 and not yet done: split
the primary measure by decision kind (forced / prior / crude) and pin
FORCED accuracy as a floor; make evidence and priors separate STAGES
enforced by the gate rather than an order remembered in the file;
coverage work (the ignoti reading, charter spelling) outranks
ordering work.

**The clause (Fran, 2026-09-08, design conversation after T19l;
decisions 43–47).** The tail of Seneca and the demotion both asked for
a dimension beyond adjacency, and the sudoku has one left between the
row and the whole grid: the box.

- **43 The clause is the box.** Between adjacency and the whole
  sentence lies the clause, the structure a finite verb governs.
  Membership is a per-element stamp with a REASON, written only when
  a layer decided it; unwritten = OPEN. A law over the clause reads
  only decided members, so elimination stays sound over a partial box
  (decision 37). A candidate SET of clauses per word is the named
  fallback if the single stamp proves too coarse.
- **44 The clause is an umbra-bearing node** (decision 42 at sentence
  scale). A `clausula` node on the sentence carries its seed, its
  species, its parent and two umbrae from the schema of its genus,
  the finite verb and the subject, filled by members through the ONE
  executor law. Elements stay flat and point at it.
- **45 Layers, each with its reason.** Seeds (data rows with a cause
  and a count: certain subordinators, relatives, punctuation,
  coordinators between finite-verb-capable words), the span, the
  closure (the verb-final prior) and the chain (along bound umbrae,
  the relative's own binding excepted). Each layer writes its reason
  beside the placement so the oracle scores it alone; a word two
  layers disagree on is a DISCORDIA, counted before either is trusted.
- **46 No phrase structure.** A phrase is the connected component of
  bound umbrae and is already there; a phrase SPAN would lie in Latin
  (hyperbaton), and the one phrase-level notion the laws need, the
  ROOT of a component, is a derived view when a law first asks for
  it. Phrase-level evidence (a coordinator, a determiner) belongs to
  a word's umbra, not to a structure.
- **47 The clause has its own oracle.** Gold clause = the subtree of
  a finite root (finite mood and not aux/cop, or the predicate of a
  finite aux/cop) minus the finite subtrees below it. Purity per
  layer, coverage and count agreement are pinned only rising per
  treebank; the verb slot is scored against the gold root.

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

**As built (T5, 2026-09-04).** Totality gate on md's shape with prose
nesting forms; green first run (the tree is total by construction, so
the planted fault lives in the lexer's token lengths). Computus twin
over five vendored Gutenberg texts (three Latin, two English; bodies
only, FONTES.md). First published costs: parse 1–4 ms per file; STML
projection ≈ 34× the source bytes and write+read ≈ 10× the parse — the
price of a node per word, to be paid by a leaner `vocabulum` projection
if it ever matters, not by a different tree.

**As built (T6, 2026-09-04) — stage 1 complete.** The fissio gate
measures, it does not pin: over the fixtures and a fifth of the
markdown corpus, `sententia_fissio` 38,899 sentences, oratio 36,762,
common starts 35,922 (92.3% of the former, 97.7% of the latter);
English prose agrees exactly. Three disagreement classes: fissio takes
a line end as a boundary (verse: 247 vs 52 on Propertius); fissio ends
at `?`/`!` before a lowercase word; oratio ends at a paragraph end
without punctuation. Replacement behind the same API awaits the call,
with a verse mode as the likely precondition.

**As built (T6b, 2026-09-04).** `ORATIO_PARAGRAPHUS_FORMA` (INDEX,
appended after `cauda`; seal b27fe13a → 93c1c9cf, canon pinned with the
cause). `oratio_forma.c`: indicia per paragraph (lines, widths, the
VOLUNTARY BREAK — a line ended though the next word would have fit the
paragraph's width, tokens of 24+ bytes excluded from the width —
punctuated and capitalised line ends and starts, list marks, inner
gaps) and a rule table `ORATIO_REGULAE_FORMAE` of ten rows, first match
wins, no match = prosa. Verse needs width ≤ 64 (Gutenberg prose is
typist-wrapped with a ragged margin at 70+, verse never reaches it) and
one more signal (punctuated ends ≥ 40 %, capitalised starts ≥ 60 %, or
≥ 70 % punctuated ends on short equal lines). The reader takes the
forma: versus/tabula/index end a unit at every line end, titulus is one
unit. Gate `probatio_oratio_forma` (171): rule table, 22 inline cases,
fixture census (Propertius 21 versus / 0 prosa, 731 units where T6 had
52; Lincoln 41 prosa / 0 versus), the hand-judged set `fixa/iudicia`
(`[[` before each unit start; pins 8/8, 8/8, 25/25 and Cicero 10/14 —
the four misses are one class: two spaces after `?`/`.` before a
lowercase word as editorial boundary, recorded as data), and the md
corpus census (14,862 paragraphs: prosa 11,879, titulus 2,869, versus
76, index 21, tabula 17). Instruments `./oratio/formae.sh` (forma, rule,
indicia per paragraph) and a forma column on `sententiae.sh`. CIRCSE
joins as the verse oracle when vendored (T13).

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

**As built (T7, 2026-09-04).** Sources vendored verbatim (CRLF) under
`oratio/vocabularium/la/` from `mk270/whitakers-words@1f2f0fb0` with
`LICENTIA.txt` and `FONTES.md`; column law transcribed from the Ada
(`Stem_Type` 18 + space × 4, part record 24 wide from column 77, five
codes from 101, meaning from 111) and ASSERTED on every record.
`oratio_vocabularium.{h,c}`: `coquere` → one nuntium stream (header
with version, source and counts; nested records tagged 2 stems, 3
inflections, 4 addons, 5 uniques; empty strings absent; codes as
varints), `recensere` (full read-back against the header),
per-record decoders, `sigillum` (SHA-256, 16 hex). Table 3,816,483
bytes (0.62 × sources), seal `f598155c06f52682`, coction 24 ms,
recensio 3 ms, committed as `la.bin`; `./oratio/vocabularium.sh
[-coquere] [-scribere]`. Gate `probatio_oratio_vocabularium` (77):
source bytes, record counts (39,335 / 1,797 / 343 / 79), table bytes
and seal, coction == committed, decoded samples, truncation refused,
mutation moves the seal, four mutated sources stop coction with file
and line. Findings: ADDONS is three lines by POSITION (twelve meaning
lines begin "TACKON …", eleven "PACKON …"); INFLECTS records carry
trailing `--` comments; the first INFLECTS record is an ADV.

**As built (T8, 2026-09-04).** `oratio_vocabularium_la.{h,c}`: `onerare`
(record arrays + hashes by folded key: stems with stem key, endings,
uniques; lists of blank endings, INTERNAL blank stems — WORDS' Bdl — and
base-X tackons; 22–25 ms), `plicare` (decision 3, one function for
keys and queries), `quaerere` → ordered `OratioAnalysis` list (genus
STIRPS/UNICUM/TACKON, stem and inflection indices, matched key, split
point, carried tackon); the matching law transcribed from WORDS'
Reduce_Stem_List (part with VPAR/SUPINE→V and PACK≤PRON; key; declension
wildcards 0 0 / d 0; noun gender X and C; ADJ/ADV degree with X entries
deriving the degree from the stem key; NUM exact key; PREP case);
search order uniques → stem+ending → tackons only when nothing was
found (deviation from "tackons first": WORDS' order and the merge bias);
ordering by dictionary line, longer ending, inflection line. `lemma`
v1 = first form from the inflection table. `./oratio/quaere.sh`. Gate
94: folding, WORDS-behaviour forms, ordering, unknowns as findings,
lemmas, corpus coverage of the Latin fixtures 95.1 % (Cicero 95.8 %).
FINDING: `sum` is absent from this commit's DICTLINE.GEN (compounds
only; 19 esse forms in INFLECTS) — pinned; supplement in T9. Deferred to
T8b with counts: tackons with a base part, prefixes, suffixes, the full
dictionary form.

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

**As built (T9, 2026-09-04).** `oratio/glossarium.stml` +
`oratio/grammatica/glossarium.canon` (registered `<glossarium>`;
`bin/canon_examen` judges it): `<vocabulum lemma lingua classis nota>`,
`<sensus>`, `<forma textus + universal accidents>` — the stage-three
class and accident vocabulary is enumerated in the canon now. Entries:
`sum` (77 forms, the T8 finding), `Vergilius` (folding proof), sixteen
`ignotum-permissum` terms with their nexus.tsv counts.
`oratio_glossarium.{h,c}`: STML reader, forms hashed by the SAME fold as
the WORDS table, lemma is a form unless listed. `oratio_vocabularium_la`
takes the glossary as optional first source (`glossarium_ponere`);
hits are `ORATIO_ANALYSIS_GLOSSARIUM` and count as found. Gate 84
(canon real by three mutations, loader refusals by line, folding both
ways, integration order, corpus 95.1 → 96.3 %). The technical-terms
list grows from the T10 report.

**Consumer: the vocabulary lint.** `oratio/vocabula.sh [-symbola |
-commenta | -omnia]`: symbols from `build/nexus.tsv` definitions split
at `_` (9,867 distinct words today), and comments from every `lib/`
and `silva/` source through silva's tree (decision 22), each word
looked up; report per word: known (with primary class and lemma),
ambiguous (candidate count), UNKNOWN (file:line of first use); summary
per directory. Python: `silva.vocabula()` on the shape of `citata`.
Report first; a gate that pins the unknown count "only falling"
comes when the report is read.

**As built (T10, 2026-09-04).** `oratio_vocabula.{h,c}`: identifiers
from the definition rows of `build/nexus.tsv` split at `_` and at the
lower→upper boundary, parts with fewer than two letters dropped;
comments from tracked `lib/*.c` + `silva/fontes/*.c` through
`silva_lexare_cruda` (comment tokens) and oratio's tree; one lookup per
distinct word, glossary first; status notum / ambiguum / permissum /
ignotum; sites per source, first site kept. `./oratio/vocabula.sh
[-symbola | -commenta | -omnia] [-machina] [-omnes]`,
`silva.vocabula(fons)`. Gate 69 (rules on inline fixtures, corpus
floors, no pin yet). Day one: identifiers 10,060 words / 5,515 unknown
(55 %); comments 14,205 / 5,736; together 19,371 words, 346,418 sites,
49 % unknown, 559 ms. Unknowns are abbreviations, English, C keywords
quoted in generated grammar comments, vendor names, and house coinages
WORDS lacks (transponere, lexema, lista, xar). Consequences: the
glossary grows from this list; T8b stays parked (no addon cases at the
top); the "only falling" pin goes on the identifier corpus after the
first glossary pass. **Glossary pass I (same day):** 77 entries from
the report — house coinages and late Latin (lexema, lista, octetus,
parso, transpono …), house proper names, C/API/format names as
permitted; common English words and abbreviations left unknown on
purpose. Lint corpus = the house: `knotapel/` (English by decision,
Fran 2026-09-04), `vendor/`, `archivum/` excluded via
`ORATIO_VOCABULA_EXCLUSA` (`-omnes-viae` scans all). Identifier
unknowns 3,022 of 7,469, PINNED only falling in
`probatio_oratio_vocabula`; comments not pinned. **As built 2026-09-07 (after the index defect, question
01M1TD1FMFT3):** the count pin became a committed SET of tolerated
unknown words (`fixa/vocabula/ignota_symbolorum.txt`, only shrinking;
a new unknown word is red with its first site; regeneration
`ORATIO_VOCABULA_SCRIBERE=1` with a named cause), the corpus is
git-tracked `.c/.h` only, the total identifier word count is pinned
only rising as the instrument's health check, and the runner
renovates the nexus index before the gates. **Decided (Fran,
2026-09-04, decision 26):** abbreviations are kept (36 permitted), and
every glossary entry carries a CONTEXT — `contextus="latinus |
anglicus | ambo"` — where the word is allowed: identifiers and
comments are the Latin context, English prose (worklogs, markdown) the
English one; the Latin lint ignores English-only entries. Unknowns
2,987 after; pin moved.

**As built — T15a, the English context (warmup, 2026-09-04; decision
27, four calls by Fran):** an English lint needs a word list, so stage
4's dictionary came forward. Moby Part-of-Speech II vendored VERBATIM
under `oratio/vocabularium/en/` (§6 as-built); `oratio_vocabula_prosa`
walks md's tree and harvests TEXTUS nodes only — code spans, fences,
link destinations, html blocks and front matter never reach the lookup;
`oratio_vocabula_creare_anglice` judges in the ENGLISH context:
glossary entries allowed in English first (a Latin entry → LATINUM,
`ignotum-permissum` → PERMISSUM, else NOTUM), Moby second, the Latin
table third → the new status **LATINUM** (known Latin in English prose
is counted, never a finding — decision 27.1); `ambiguum` never. Corpus
= the house INCLUDING `knotapel/` (English), minus `vendor/`,
`archivum/` and the generated files (`gesta/annales/tabula.md`,
`md/CENSUS.md`) — `ORATIO_PROSA_EXCLUSA` (27.2). `./oratio/vocabula.sh
-prosa`, `silva.vocabula('prosa')` (a `prosa` column, a `latinum`
count). Gate sections V–VII (inline md with every exclusion proved;
English judgement; corpus floors, no pin). Day one: 1,139 files, 62,524
distinct words, 1.89M sites, 58 % unknown — the top of the list is
regular inflection Moby does not list (-s, -ed, -ing, 's), then house
English terms; morphology lands next as DATA rows with those counts
(27.4). Two corpus lessons: a lone `→` was a "word" (bytes ≥ 0x80 were
letters; now UTF-8 lead bytes count), and the site-count ordering was
quadratic (now a counting sort).

**As built — T15b (same day):** the morphology is DATA —
`ORATIO_REGULAE_EN` (25 rows: plural ×3, past ×4, participle ×3,
possessive, contractions ×6, adverb ×3, comparative ×2, superlative ×2,
`compositum`; each row = suffix, base substitution, gemination, minimum
base, required Moby codes of the base, class, cause with the measured
tally) plus `ORATIO_PRAEFIXA_EN` for hyphen compounds (non, multi, semi
…); `oratio_vocabularium_en_analysare` = exact records first, then
rules in table order, each analysis carrying rule, base and class; the
judge takes the first analysis (lemma = base, `OratioVerbum.regula`);
the English fold maps U+2019 to `'`. Glossary pass II (117 entries,
ANGLICA PROSA section: proper names, computing English with explicit
plurals — rules reduce to Moby bases only —, abbreviations, `struct`
and `sizeof`, Roman numerals ii–xxx as `numerale` for both contexts,
house Latin internamentum/diagnostica/friatio/amalgama). Prose
unknowns 58 % → 30 % (rules) → 26.2 % (pass II); known 73.8 %. PROSE
PIN = a ceiling on the unknown SHARE (29 %) plus a floor on the known
share (70 %), not a count only falling — prose gains words daily; a red
means a rule or a glossary section broke. Identifier pin 2,986 → 2,950
on the `ambo` entries.

**As built — T12 (2026-09-04), the annotated word.** `oratio_partes_la`
describes one source analysis universally (class, twelve accidents as
enumeration indices, −1 = not given; lemma; `nativum` = the source's
code verbatim; sense; source; language) by the mapping above, with
these corpus-decided details: noun kind N/L → nomen-proprium; noun
gender from the STEM when M/F/N, else from the ending; stem kind
DEP/SEMIDEP → vox deponens; INF ending → modus infinitivus + forma
infinitivum; PPL → participium, FUT PASSIVE PPL → gerundivum; SUPINE →
supinum; PREP case from the ending else the stem; CONJ subordinating
by the data list `ORATIO_CONIUNCTIONES_SUBORDINANTES`; enclitic
`-que`/`-ve` → coniunctio-coordinans, `-ne` → particula; glossary
class by name, accidents through the title arrays, `ignotum-permissum`
→ ignotum. Enumeration titles were RECONCILED with the glossary canon
(genitivus; locative before vocative; persons I II III) and the
registry gate now proves the canon's option lists equal the title
arrays in order. `oratio_partes_annotare` (the pass after the tree):
one lookup per word, one node per description with accidents placed
by SLOT TITLE through the registry, `classes`/`linguae` derived tokens
(distinct, in order; `ignotum` alone for a word without analyses),
write-once, `patres_figere` at the end. SUBSTRATE CHANGE (the second):
`MateriaArborFrons.nodum_ornare`, a node-level attribute hook the
writer calls after creating a node's element and the reader ignores;
oratio's frons mirrors `classes`/`linguae` onto `<vocabulum>` so
`vocabulum[classes~=verbum]` works in selectio — which exposed an
unsigned underflow in selectio's `~=` when the value is longer than
the attribute (fixed in `lib/selectio.c`). `./oratio/arbor.sh -partes`
annotates before projecting. Gate `probatio_oratio_partes` (171;
planted fault = stem gender dropped). Day one: Hilarius 1,678 words /
10,352 analyses / 5.2 % unknown; Propertius 4,423 / 27,819 / 4.6 %;
Cicero 13,214 / 114,876 / 2.9 %, 858 ms — six to nine analyses a word,
dominated by WORDS' pronoun packings (`quis` = 215): faithful to the
source, to be ordered in stage 5 or deduplicated by (class, accidents,
lemma) before the oracle.

**As built — T13 (2026-09-04), the oracle.** Vendored under
`oratio/probationes/fixa/ud/` (CC BY-SA 4.0, licence + README
verbatim, FONTES.md with commits/seals): CIRCSE test (Seneca, 893
sentences, 11,503 tokens) and LLCT dev + test (850 + 884 sentences,
24k tokens each; train NOT vendored — Fran's call, UD scores on
dev/test); NC treebanks fetched by `./oratio/oraculum.sh -petere`
into `oratio/build/ud/`, reported only. `oratio_conllu` (ten fields
or stop with the line; ranges `a-b`; empty nodes skipped; `# text`
or reconstruction from forms and SpaceAfter). `oratio_oraculum`:
sentence text parsed and annotated, elements located by byte
extents, gold forms located in the same text in order, a range
judged per word against the one element's class set; per gold word
TECTUM (coverage, PINNED only rising, permille per file), PRIMARIUM
and LEMMA reported, ignotum and unaligned counted, five uncovered
examples per class (`-exempla`). Alignment: 0 unaligned of 60k,
0 broken sentences. Day one CIRCSE 84.2 % / LLCT 71.9 % / 72.5 %
coverage; zero on auxiliare, determinans, particula. MAPPING PASS II
from the table (data lists with the counts as cause, secondary
descriptions APPENDED after the primary): `sum` + auxiliare;
`ORATIO_DETERMINANTIA` + determinans; `ORATIO_PARTICULAE` +
particula; adverb/preposition in the subordinating list +
coniunctio-subordinans (list grown: qualiter unde quam quatenus
quomodo …); ordinal + adiectivum; WORDS kind N (deus) + substantivum
(kind L stays proper); capitalized unknown → nomen-proprium with
source `regula` (fourth value appended to `OratioFonsAnalysis`),
nativum `capitalis`. After: CIRCSE 93.7 %, LLCT 88.9 % / 88.2 %
(pins 937/889/882); primary ≈ 68 % everywhere (the unordered list,
stage 5); lemma 89 / 65 / 65 %. Gate `probatio_oratio_oraculum`
(104); partes gate 203.

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

**As built — T11 (2026-09-04):** seventeen genera `analysis-<classis
genitive>` appended after `numerus` (registry 23 genera, 146 slots;
genus = `ORATIO_GENUS_ANALYSIS_PRIMUM` + `OratioClassis`; class titles
in `ORATIO_TITULI_CLASSIUM` = the glossary canon's `classis` options,
gate-guarded). Common five first on every genus, then accidents as
INDEX (decision, Fran approving the lists): substantivum and nomen
proprium casus/numerus/genus/declinatio; verbum and auxiliare
persona/numerus/tempus/modus/vox/forma-verbi/coniugatio/casus/genus;
adiectivum casus/numerus/genus/gradus/declinatio; adverbium gradus;
pronomen casus/numerus/genus/persona; determinans casus/numerus/genus;
adpositio casus; numerale casus/numerus/genus/species (cardinale,
ordinale, distributivum, adverbiale); the rest common only. `nativum`
is ONE derived token (Fran). Enumerations appended-only with title
arrays (`ORATIO_TITULI_CASUUM` …; casus has locativus and vocativus,
numerus dualis, vox deponens, tempus the English praeteritum). The
word's three slots were reserved since T1, so existing STML bytes are
unchanged; seal 93c1c9cf → 87b35173; the canon carries 17 genus rules
+ 129 slot rules intra their genus (generated once, hand-kept; ceiling
512, 182 rules). Gates: registrum (860; helpers, layouts, glossary
drift, materia round trip with an analysis), canon (219; a hand-built
analysis judged, two mutations refused).

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

*As-built (T14, 2026-09-05).* `verba.sh` takes several files (the via
column is meaningful), prints paragraph and sentence ordinals beside
the byte extent (sentence ordinals agree with `sententiae.sh`), and
has an `-analyses` mode: one row per analysis node with class, lemma,
lingua, fons, nativum, sensus and the twelve accidents as the
registry's option titles, read from the tree by slot title (never
re-derived). `silva.Oratio` accepts a path, a text or bytes (a text is
written to build/pythonica for one call), caches each query, and
`vocabula(classis=)` matches ANY candidate class (the primary is stage
5's); `vocabula(sententia=)` added; `analyses()` takes a word or its
index and returns `accidentia` as a dict with absences omitted.
`Prosa.sententia` concatenates the markdown paragraphs inside the
extent (headings, fences, html never), parses once, maps the extents
back with line and column; usable as an edit anchor. Gate = the
pythonica suite, 20 assertions, planted fault (word end shortened)
red.

## 6. Stage 4 — English (`vocabularium_en`)

Moby Part-of-Speech (`mobypos.txt`, public domain): one line per
entry, form `\` codes (the Gutenberg copy uses a backslash; `×` is the
CD-ROM edition) (N noun, p plural, h noun phrase, V verb
participle, t transitive, i intransitive, A adjective, v adverb, C
conjunction, P preposition, ! interjection, r pronoun, D definite
article, I indefinite article, o nominative) in preference order.
Compiled like Latin (folded key = lower case). A light MORPHOLOGY in
C: regular plurals, `-ed`, `-ing`, `-s` third person, `-er`/`-est`,
an irregular-forms table (vendored from a PD list or hand-written);
each rule yields analyses marked with the rule as `nativum`. Mapping
Moby codes → classes. Oracle: UD English EWT (COVERAGE pinned).

**As built — T15a (2026-09-04):** `oratio/vocabularium/en/mobypos.txt`
verbatim from the Gutenberg mirror (3,237,558 bytes, 233,356 records,
CRLF; `LICENTIA.txt` = the ebook's documentation note, grant + legend;
`FONTES.md`). NO coction (decision 27.3): the source is one table, the
loader `oratio_vocabularium_en_onerare` reads it directly, asserting
the record law on every line (CRLF, one backslash, non-empty form and
codes — stops with the line) and COUNTING code letters outside the
legend (one: `cowardic\Ne`); words hashed by lower-cased form (197,387),
phrases counted not indexed (35,969); `quaerere` → records in file
order; `oratio_vocabularium_en_classis` maps a code letter to the
universal class. Findings pinned in `probatio_oratio_vocabularium_en`
(seal `cc81458b820a3625`, 27 ms): the legend's `I` and `o` never
occur; 1,231 CP437 records (fold = ASCII lower case + U+2019 → `'`;
accents stay as they are); 4,437 case-duplicate forms. **T15b (same
day):** the light morphology is the DATA table `ORATIO_REGULAE_EN`
(§4 as-built) with `oratio_vocabularium_en_analysare`; Moby lists more
inflected forms than its legend admits (`entries` p, `tried` V,
`making` N, `simply` v, `I'm`), so exact forms always precede rule
analyses and two contraction rows explain nothing in this corpus. The
irregular-forms table is not needed yet (no irregular at the top of
the unknown list); gate section VI (160 assertions).

**As built — T16 (2026-09-05, stage 4 complete):** `oratio_vocabularia`
(`OratioVocabularia {la, en}` + `oratio_vocabularia_onerare` from the
repo root — the one loader every instrument and gate uses); the
annotator queries BOTH dictionaries for every word and appends the
English readings after the Latin ones (ambiguity as a list; `in` is
an ablative preposition and an English one). `oratio_partes_en`: an
exact Moby record → one description per class among its code letters
in Moby's order (`p` = plural), a rule analysis → accidents from the
rule's title (pluralis per BASE class: `cats` noun plural and verb
third person; praeteritum finite + participle; possessivum genitive of
the base's class + particula + auxiliare; contractions base + particula
or auxiliare; comparativus/superlativus), `nativum` = codes + rule,
no sense. Pass II from the EWT day-one table as DATA lists
(`ORATIO_AUXILIARIA_EN` … `ORATIO_INTERIECTIONES_EN`) applied once
per word with source `regula`; the capital rule now fires for a
capitalised word no Latin source knows that is a noun or nothing
(Moby knows Hercules as a noun — CIRCSE had dropped); sign elements
carry interpunctio AND symbolum in the oracle. Oracle: EWT dev + test
vendored (commit 4a4d77f5, CC BY-SA; contractions are ranges); day one
77.1 / 77.0 %, after pass II 91.3 / 91.8 % coverage (pins 913 / 918),
primary 57 %, lemma 75 %; Latin pins rose to 940 / 895 / 887. The
irregular-forms table is still not needed (lost/grown are adjectives
in Moby, a class question, not a form one). Gates: partes 275 (planted
fault: plural made singular), oraculum 115.

## 7. Stage 5 — resolution by context

An STML program `oratio/partes/resolutio.stml` run through the
command layer over the projection: pattern arms that REORDER a
word's analyses (never delete) by agreement (adjective ↔ noun in case,
number, gender; determiner ↔ noun), adjacency (adposition governs the
following case), and sentence position. Each rule is measured by the
oracle's PRIMARY count; a rule that cannot be expressed in the
vocabulary is a finding for the STML exploratory design. The primary
pin starts here.

**Decided 2026-09-05 (interview round 5, decisions 28–35):**

- **28 Addressing.** Plan rows point at words and analyses by
  ORDINALS the projection stamps (`n=` on `sententia`, `vocabulum`,
  every `analysis-*`, through the `nodum_ornare` hook; attributes are
  mirrors, the reader ignores them). No byte extents: the pattern
  engine's extent-provenance obligation stays unpulled.
- **29 Plan surface.** The arms emit `<consilium><praelatio
  sententia= vocabulum= analysis= regula=/>…</consilium>` rows through
  PER into the expanded document; the oratio canon declares them, so
  the plan is judged before execution; the C executor walks the rows.
- **30 Conflicts.** Rule order is priority, FIRST WINS per word; rules
  are appended, never reordered without a named cause. No weights (an
  expression language in the data plane is refused).
- **31 Adjacency.** Loose matching's ordered-subsequence-with-gaps is
  accepted in v1 and measured; a measured loss pulls the strict-run
  door of the pattern spec with data behind it.
- **32 First rule.** Walking skeleton = *adpositio casum regit*: the
  preposition's `casus` captured, the next word's analysis with the
  equal `casus` preferred. One pure pattern exercising captures,
  iterated-capture equality, sibling order, executor, oracle delta.
- **33 Language.** The annotator's per-language census is stamped as
  `lingua=` on the projection root (a decreed shallow projection); the
  language rule is then an ordinary pattern that prefers each word's
  first analysis of the document's language.
- **34 Measurement.** The oracle applies the program's rules
  CUMULATIVELY in order and prints PRIMARY per treebank after each
  rule; the gate pins PRIMARY per treebank only rising from the first
  rule on.
- **35 Default.** Resolution is ON wherever words are annotated when
  the program file exists (`verba.sh`, `arbor.sh -partes`,
  `silva.Oratio`); `-crudus` shows the source order; the oracle prints
  before and after.

**Execution model (v2 pass, same day).** Per SENTENCE: the annotated
sentence subtree is written to STML with ordinals and the document's
`lingua`, concatenated with the program (the lint harness's
composition: one `stml_legere`, one `stml_expandere`), the
`<consilium>` rows are read, and a materia verb permutes the word's
analyses list: `materia_nodus_lista_permutare` (substrate change #3),
contract-checked — the new order is a permutation of the old, nothing
dropped or doubled, refused loudly otherwise. Sentence-local rules
need nothing more; the language rule reads the stamped attribute.
Per-sentence keeps memory bounded (Cicero's whole projection is 107
MB) and matches the oracle's unit.

**As built — T17 (2026-09-05, stage 5 opened):** exactly the execution
model above. Ordinals and `lingua` through `OratioStmlOrnatus` /
`oratio_stml_consilium_ornatum` (plain `oratio_stml_consilium` still
stamps ordinals, never the language); canon: `n` on sententia,
vocabulum, interpunctio, numerus and all seventeen analysis genera,
`lingua` on documentum and sententia, elements `regula` (with
`titulus`), `consilium`, `praelatio` — a canon declares ONE root, so
`regula` rides as a declared second root, and the drift guard names
the three. Materia verbs `materia_nodus_lista_permutare` (permutation
contract) and `materia_nodus_reponere` (written-slot contract).
`oratio_resolutio`: `programma_legere/onerare` (rules = `<regula>`
roots, text by byte extent), `applicare(piscina, intern, ratum,
programma, regulae_numerus, lingua, radix, census)` per sentence with
a scratch piscina, census per rule, first wins, `compendia_reponere`
after each permutation. Two probes decided the rule form: a captured
case binds to the preposition's first analysis and never retries
(`in urbem` empty), and a sentence-rooted pattern matches one greedy
pair per sentence — two LITERAL rules (accusative, ablative), one
pair each, measured; the sibling-run door is the finding. Cumulative
primary: CIRCSE 67.4 → 67.9, LLCT 67.8 → 68.0 / 67.7 → 67.9, EWT 56.6
→ 56.7 / 56.9 → 57.0; pins 678 / 679 / 679 / 566 / 569 permille.
Projection +4.2 % (computus golden regenerated). Gates resolutio 87,
oraculum 127, materia nodus 92; `-crudus` on the instruments,
`oraculum.sh -regulae`, `silva.Oratio(crudus=)`.

**As built — T19a + T19b (2026-09-07, stage 5 continued).** The
oracle names its primary misses first (`oraculum.sh -discrepantiae`:
per gold class the covered-but-not-first forms with our first class
and a count; gated sum law tecta − primaria), and the class-prior
rules are written from that listing. Eleven rules precede the
language rule, because first wins per word and the language rule
would otherwise claim every English word with its dictionary-order
reading; each is language-gated by the T18 chain and selects a
reading by class, by pass II list TITLE in `<nativum>` where the
class also comes from a dictionary code (Moby marks good, thanks,
there as interjections and `a` as a preposition), or by lemma text
for the two exceptions (`supra`, `a`) placed before their class rule.
Ambiguous list members (so, when, where, no; Latin ubi, unde, quod)
keep their secondary reading under an `*-ambigua` title the rules do
not match — coverage untouched, primary untouched. Lemma priors that
are not context (iam, vel, nec, the sanctus paradigm, the charters'
saints, English I, just, no) went into the glossary, which is the
first source. Engine law found: pattern children are an ORDERED
SUBSEQUENCE of the candidate's children, so a rule must write
`<lemma>` before `<lingua>`. Cumulative primary: CIRCSE 67.9 → 72.8,
LLCT 68.0/67.9 → 77.0/77.1, EWT 67.8/68.0 → 75.3/75.5; pins moved.
Remaining for T19c/d: the sibling-run door and agreement.

**As built — T19c (2026-09-07).** The door: `cursus="fratrum"`
(exemplaria increment 20). The adposition rules are rooted at
`<elementa>` under it and fire once per preposition. Measured on the
five treebanks the primaries did not change: the greedy one-pair
match of T17 was almost never the binding limit for these two rules.
Direction decided with Fran the same day (desideratum 01M1YYN00E):
agreement and government are to be UMBRA NODES in the tree — a
reading carries the dependent it expects (relation, conditions drawn
from the dictionary's own codes: WORDS TRANS = accusative object,
PREP ACC = object in that case, ADJ = agreeing head; candidates as a
list; binding by ordinal, empty = finding), generic patterns per
relation fill them from siblings (the constraint lives in the tree,
the rule stays one per relation), `reponere` binds. A bound umbra is
a dependency edge; a reading whose umbrae cannot be filled yields to
one whose can. First umbra: the adposition's object, against the
existing rule as the comparison. Long term the same node carries
frame expectations at the sense level (natura genera as the types).

**As built — T19d skeleton (2026-09-07).** Registry: slot `umbrae`
(LISTA_NODUS) appended last on all seventeen analysis genera; genus
`umbra` appended after them with seven INDEX slots (relatio, classis,
casus, numerus, genus, impletio-vocabulum, impletio-analysis); 170
slots, seal `54a5e44f`; `ORATIO_GENUS_ANALYSIS_ULTIMUM` closes the
class range. Annotation: adposition readings with a case get one
object umbra in that case. Program: the T17 adposition rules replaced
by `umbra-obiectum-<casus>` rules (any carrier, filler noun in the
case; cases literal because a capture on the umbra binds to the first
reading). Plan rows `<impletio>`; executor prefers carrier and filler
under first-wins, permutes, then binds with the filler's index remapped
through the preferred-first law; repeated fills counted. Canon declares
the three elements. Numbers: identical to the T17 rules plus one
permille on each Latin treebank (729/771/772); the mechanism is proven
on the same measure. Open: head umbrae (agreement), the outranking law,
English object umbrae.

**As built — T19d head umbra and stages (2026-09-07).** Annotation:
every adjective and determiner reading with a case carries one CAPUT
umbra (relatio 1) whose case, number and gender are the reading's
own — the dictionary's codes are the constraint, no rule repeats
them. Program: fourteen generated rules `umbra-caput-<casus>-
{sequente,praecedente}` (the head after or before the carrier),
each rooted at `<elementa>` under `cursus="fratrum"`: a carrier
reading whose umbra wants the literal case, number `$num` and gender
`$gen` captured AT THE UMBRA, then any reading `<* n="$b">` of the
neighbouring word with that case and the captured number and gender.
The case is literal for the known reason (a capture binds to the
first reading it meets and never retries); number and gender may be
captured because the umbra is the first place they are read. Rows
are the same `<impletio>` as the object umbra. Executor: STAGES.
`<regula gradus="2">` (attribute, default 1, declared in the canon);
`oratio_resolutio_applicare` runs stage 1 to the highest stage
present, each stage composing the program's rules of that stage
against a FRESH projection of the sentence, so a stage-2 rule sees
stage 1's reorderings; the vindicated-word set persists across
stages and gains one law: an impletio row whose FILLER word is
already vindicated and whose filler reading is not that word's first
is repetita. Both were forced by `Cum puella bona ambulat`: in one
stage the nominative head rule fired before the ablative one (rules
are ordered by case, not by evidence) and bound `bona` to `puella`'s
nominative; with stages the object umbra decides `puella` first and
the law refuses the nominative filler because `puella` is spoken for
and that reading is no longer its first. Probes: `Puella bona
ambulat` → bona adiectivum nominativus (crude first: substantivum);
`Cum puella bona ambulat` → puella ablativus, bona adiectivum
ablativus; `Hoc templum est` → hoc determinans nominativus, templum
nominativus (crude first: genitivus pluralis); `Hoc est` → hoc
pronomen (no filler, the umbra stays a finding). Numbers:
CIRCSE primary 72.9 → 76.5 % (adjective 78.6 %, determiner 19.9 %), LLCT 77.1 / 77.2 → 78.8 / 78.6 % (determiner 0 → 18.7 / 18.1 % on 2,096 / 1,914 words), EWT unchanged 75.3 / 75.5 (English readings carry no case, so no head umbra is born); coverage unchanged; the vocative pair adds nothing on Seneca and one permille on the charters; pins 765 / 787 / 785 / 753 / 754. Gates: resolutio (stage table, the four probes with
their census, stage 1 alone leaves `bona` a noun), partes I b
(caput umbrae on every cased reading of `bona`, six or more, each
mirroring its reading's accidents), oraculum pins, canon (`gradus`).
Open: the outranking law ("a reading with filled umbrae outranks one
without", executor-side), English object umbrae (`to` + verb, `have`
+ participle), nomen-proprium and pronoun fillers if the numbers ask.

**As built — T19d gamma, the law of umbrae (2026-09-07).** After the
last stage the executor orders each word's readings: readings whose
umbrae are ALL filled by a NEIGHBOURING word (distance one) come
first, the rest keep their order; words decided by a `<praelatio>`
row (a class prior, an exception, the language rule) are left alone;
bindings that point into a permuted word are remapped (the umbra's
`impletio-analysis` rewritten through `reponere`). The law is C, in
`_umbris_ordinare`, never arithmetic in the program. Three shapes
were MEASURED before this one was kept, each on the five treebanks:
(1) three ranks — filled, no umbrae, unfilled — with no regard for
explicit decisions: every treebank fell (Seneca −0.5, charters
−1.5/−0.9, EWT −0.2/−0.4) because a chance binding on a Latin reading
overrode the language rule (`a` + a stray ablative `die`) or an
exception rule (`supra` + any accusative), and the demotion of
unfilled readings sank prepositions whose object is a proper name
without case (adpositions −11 on the charters); (2) the same with
explicit decisions respected: EWT back to par, charters +0.3/+0.5,
Seneca still −0.5 with adjectives paying for the demotion (−4); (3)
promotion only, any distance: charters +0.5/+0.5, Seneca −0.5 —
nouns lost to adjective homographs bound to a distant word, because
the `praecedente` rule binds a carrier to the FIRST agreeing earlier
word, not the nearest. Neighbour-only promotion is the first shape
that depresses no treebank: Seneca 76.5 (unchanged), charters
79.2 / 78.9 (+0.4 / +0.3), English untouched (no case, no umbra);
pins 765 / 791 / 789 / 753 / 754. On Hilarius: 532 bound umbrae, 0
discordant (every binding points at a reading of the umbra's case,
number and gender), 44 words reordered by the law. What the law buys
is the word decided STRUCTURALLY in stage 1 by a weak choice — `in
bona terra`: the object rule takes `bona`'s first ablative reading
(the noun, by source order), stage 2 binds the adjective's head to
`terra`, the law puts the adjective first. Findings recorded for the
next steps: (a) "no object found" is real evidence against a
preposition (`cum venit`, `ante venit`) but cannot be used until
fillers of UNKNOWN case (the capital rule's proper names, unknown
words) can fill a case umbra — the demotion must wait for them; (b)
"nearest agreeing word" needs the pattern engine's reserved
`strictus` door or a nearest-first sibling policy — adjacency is the
stand-in; (c) rule order still decides between two filled readings
(`in bona terra` ends nominative, the head rules run by case) — the
decided case of a structurally chosen word could break that tie.
Instruments: `verba.sh -analyses` gained an `umbrae` column
(`relatio:casus.numerus.genus=word.reading`, `=?` unfilled),
`silva.Oratio.analyses()` exposes it as `umbrae`; `oraculum.sh
-regulae -ab N` prints the cumulative table from rule N only (the
full table re-judges five treebanks per rule, nine minutes at 29
rules). The wrapper's empty-argument expansion under `set -u` (bash
3.2) was fixed on the way.

**As built — T19e, English by evidence (2026-09-07, evening).** The
pattern engine's reserved `cursus="strictus"` was built first
(exemplaria increment 20: the sibling run without gaps, every later
child must match the next candidate child) because English function
words are adjacency facts. Then three kinds of evidence, all placed
BEFORE the class priors in the program, because a prior at index 7
had decided `to` before the umbra rule at index 13 could fill it
(first wins; particles fell from 70 to 15 % before the move): (1)
`nomen-proprium-capitale-anglicum` — any element, then a word with the
capital rule's reading (`nativum` capitalis), strictly adjacent, so
the sentence-initial word is excluded (Thanks, Store) — EWT proper
nouns 19 → 64 %; the capital rule itself no longer fires on a word
with a pronoun reading (I'm, You'll, It's: Moby lists them as nouns,
the contraction gives the pronoun) but still on possessive names
(Debra's, coverage pinned) and on Will/May; (2) class umbrae — an
English reading has no case, so its umbra names a class: `to`'s
particle reading and have/do's auxiliary reading (moved from the
prior lists to `*-ambigua` lists with their own `nativum` title, so
the class priors are blind to them) carry an object umbra `classis`
verbum, filled strictly by the next word's verb reading, with an
auxiliary-filler rule first so `have been` keeps `been` an
auxiliary; no fill → the language rule takes Moby's first code (`to`
adposition, `have` verb) — adpositions 89 → 99 %, verbs 67 → 73 %;
(3) the contraction auxiliaries (`'s 'm 'll`) lost the auxiliary
prior when it switched to the list title, so a second prior selects
them by source (`fons` vocabularium-en). One executor law came out of
`have a car`: Moby codes `a` as a verb too, and the umbra bound it;
the filler law now also refuses, within a stage, a filler word an
EXPLICIT rule decided as another reading (the `a` exception), while
a structural decision of the same stage does not block (measured:
the charters lost 0.2 when it did, the accusative-before-ablative
order of the object rules being arbitrary evidence). Measured and
dropped: a determiner head umbra (`the` + noun) — it preferred
Moby's noun readings of `is` and `it's` after that/this/it
(auxiliaries −9) and never reached store/care behind an adjective;
a distant-verb variant for have/do (`do not like`, `have you seen`)
that lost more on `have to go` than it gained. Numbers: EWT 75.3 /
75.5 → 78.6 / 79.0 (pins 786 / 789), coverage unchanged, Latin
exactly unchanged. Gates: resolutio I (33 rules, stage 1 = 19) and
VIII (the four English probes), partes I b (class umbra on `to`'s
particle reading, none on the adposition, no capital reading on
It's, one on Bush), oraculum pins. Still open on English: proper
nouns at sentence start, `saw`-type noun-verb homographs after
pronouns, `there is`, possessive determiners as PRON (EWT's
convention), `like` as a verb, ranges.

**As built — T19f, fillers of unknown case (2026-09-07, late).**
Three rules after their exact counterparts: a Latin reading's object
umbra of any case, or head umbra of any case, number and gender, is
filled by the neighbouring capital-rule proper noun (`nativum`
capitalis, which carries no case) — `in Lucca`, `sancti Petri`. The
corpus invariant now treats an unwritten accident on the filler as
compatible. Every Latin treebank rose: Seneca 76.5 → 76.8, charters
79.2 / 78.9 → 79.6 / 79.1 (pins 768 / 795 / 790), English untouched;
Hilarius 560 bound umbrae. Then the demotion half of the umbra law
was re-measured on top, three ways, and every way lost on the
charters: dropping the Latin adposition prior so objects alone decide
adpositions cost 0.4 / 0.3 there; adding demotion of unfilled object
umbrae cost 0.7 / 0.6; demoting any unfilled umbra 0.6 / 0.5. The
cause is unchanged: 18 % of charter nouns are unknown to WORDS and an
unknown word has NO reading at all (the `ignotum` class is a summary
token, not a node), so nothing can be bound to it. "No object found"
becomes usable evidence only when unknown words get an
`analysis-ignoti` reading — a registry-visible change (projection
size, the computus golden, `Oratio.ignota`) that is the next lever
for Latin, together with a charter glossary or spelling
normalisation. Until then the adposition prior stays and the law of
umbrae stays promotion-only.

**As built — T19g, the decision in the tree (2026-09-08; decretum
SUDOKU decision 40, first build implication).** Registry: two slots
appended to `vocabulum` (172 slots, seal `6c5a6ab9`): `decisio`, an
INDEX into `OratioDecisio` — praelatio = a `<praelatio>` row decided
the first reading (class prior, exception, language rule: a SOFT
decision), impletio = an `<impletio>` row preferred it (carrier or
filler: evidence), umbra = the law of umbrae put a neighbour-filled
reading first (evidence) — and `auctor`, a derived token carrying
the deciding rule's title (`lex-umbrarum` for the law). Unwritten =
nobody decided: an OPEN cell when several readings remain, a SOLVED
one when one. The executor records the decision at the row that
first preferred the word (first-wins across stages), the law
overwrites only when it changed the FIRST reading (a lower reading
moving up leaves the earlier decision: `bona` after `Puella` keeps
its impletio when the vocative rule lifts a second reading), and
writes both slots after the last stage (`_decisiones_scribere`,
reponere on a second run; census `decisae[]`). The projection mirrors
`decisio="…"` as an attribute through the node hook, so
`vocabulum[decisio=impletio]` selects and a later rule can read the
kind; the canon declares both slots. Annotation alone writes nothing
(crude bytes unchanged; partes gate). Instruments: `verba.sh` columns
`decisio` and `auctor`; `silva.Oratio.vocabula()[i].decisio/.auctor`
and `Oratio.partitio()` (praelatio | impletio | umbra | una | aperta
| nulla, plus `coactae` = impletio + umbra); the oracle prints the
PARTITIO per treebank — every aligned gold word by the decision kind
of its first element, `ranga` = a range word after the first, never
primary by construction, the 2 % ceiling made visible — and an
AUCTOR table (words and primary per deciding rule); machine rows
`PARTITIO` and `AUCTOR`; the gate pins the accuracy of FORCED
decisions (impletio + umbra) per treebank only rising and asserts
two sum laws (partition = verba − inalignata; auctores = decided
words). Computus golden regenerated: the projection carries the two
slots on decided words.

FIRST MEASUREMENT — the finding the doctrine asked for. Forced
decisions are the LEAST accurate tier on Latin: Seneca 72.1 % over
4,462 words (39 % of the treebank), charters 69.2 / 68.8 % over
8,166 / 8,123 (34 %) — against priors at 85 / 98 / 98 %, the
untouched dictionary order at 80 / 92 / 92 % and single readings at
95 / 96 %. English forced decisions are 91.3 / 91.8 % but only 4 % of
words; the language rule alone decides 79 % of English words at
78 %. Per rule: the Latin adposition prior is right 85 / 99 / 99 %
and speaks BEFORE the object umbra rules (the Latin program is
priors-then-evidence, the reverse of the English order — the swap is
to be measured); the object umbra rules 84–86 % (ablative) and
77–84 % (accusative); the unknown-case object filler 99 % on the
charters and 57 % on Seneca; the head rules 59–76 % (nominative
following, the largest at 1,452 / 1,454 words, 73 / 59 %), the
dative following rule 37 / 41 % on the charters, the vocative rules
25–55 %; the law of umbrae itself 54 / 57 / 51 %; the English
contraction prior 0 %, because a contraction is a range the oracle
judges by its first word (a measurement artifact, now visible). The
primary pins did not move (768 / 795 / 790 / 786 / 789); forced pins
721 / 692 / 688 / 913 / 918. NEXT: a wrong-forced listing per auctor
(which forms a rule decides wrongly, as `-discrepantiae` does per
class), then make the head rules and the law trustworthy before
adding any rule; the Latin priors-then-evidence order measured
swapped; the ignoti reading; the strict head tier.

**As built — T19g bis, the wrong-forced listing (2026-09-08, same
session).** The oracle tabulates every DECIDED gold word that is not
primary by (deciding rule, folded form, gold class, our first class,
binding partner, distance): the partner is the word the first
reading's bound umbra points at (a carrier) or, failing that, the
NEAREST word whose reading has an umbra pointing here (a filler),
with the distance in elements. `oraculum.sh -errata [-auctor <rule>]`
prints per rule the most frequent rows (machine rows `ERRATUM`), and
the per-rule table splits each rule's words and primary by partner
distance (`vicina` = 1, `remota` > 1; the machine `AUCTOR` rows
gained the four columns); the gate asserts per rule that the errata
sum to verba − primaria and prints the top three of the two largest
rules. FINDINGS on the first reading: (1) distance is the
discriminator for the HEAD rules — charters genitive following 99 %
adjacent against 33 % distant (118 against 6 words), accusative
following 84 against 52 (136 against 389), Seneca nominative
following 82 against 70 with three quarters of its 1,452 decisions
distant: `cursus="fratrum"` binds the first agreeing word at any gap,
and while the law of umbrae promotes only neighbours the ROW's
preference was never restricted; the object rules are the reverse
(accusative object 64 adjacent against 84 distant on the charters,
where the object legitimately sits past an intervening modifier);
(2) the charters' nominative (61 % even adjacent) and dative (27 %
adjacent) head rules fail on a CONVENTION: UD tags possessives and
quantifiers (meus, noster, vester, suus, alius, omnis, ipse, iste,
suprascriptus, qui) as DET, and our adjective reading wins the tie
with the determinans reading by list order; (3) Seneca's remainder is
a long tail of noun, participle and verb forms with an adjective
homograph bound at distance 2–13 (`abstrusa`→`torta` @10,
`arua`→`indica` @13). NEXT: the strict head tier — rewrite the
fourteen head rules with `cursus="strictus"` and measure; keep loose
rules only if they can bind without preferring, which needs a rule
attribute; then a determinans-over-adjective law for the UD-DET
lemmas (data); then the ignoti reading.

**As built — T19h, the strict head tier (2026-09-08, same session).**
Measured first: all sixteen head rules switched to `cursus="strictus"`
LOSE primary everywhere (Seneca 76.8 → 75.2, charters 79.6 / 79.1 →
78.5 / 78.5) although forced accuracy rises to 76 %: the forced count
halves and the words left undecided fall to a worse default, so a
distant binding at 70 % still beats no decision for those words. Kept
instead: a TIER — the sixteen head rules duplicated with
`cursus="strictus"` at stage 2 (titles `-proximo`), the loose
originals moved to stage 3 — so an adjacent agreeing word of any case
wins over a distant one of an earlier case (the loose nominative rule
used to bind a carrier's nominative reading to a far nominative before
the accusative rule could see the adjacent accusative). Program 52
rules (stage 1 = 20, stage 2 = 16 strict, stage 3 = 16 loose). Seneca
76.9, charters 79.7 / 79.3, English unchanged; forced accuracy 72.3 /
69.4 / 69.4; pins 769 / 796 / 792 and 723 / 694 / 694. Small, positive
everywhere, no treebank depressed. The gate's rule table asserts the
three stages and both titles at XX and XXXVI. NEXT: the
determinans-over-adjective law for the UD-DET lemmas (data; the
charters' remaining adjacent errors), loose rules that bind without
preferring (edges only; a rule attribute), the Latin order measured
swapped, the ignoti reading.

**As built — T19i, determiners by data and the law of the explicit
class (2026-09-08, same session).** From the listing: the determiner
data list is split. `ORATIO_DETERMINANTIA_CERTA` (meus tuus suus
noster vester hic ille iste ipse idem quicumque quidam omnis nullus
ullus totus uterque alter alius tantus talis quantus qualis quot tot
cunctus ambo suprascriptus praedictus ceterus reliquus: DET without
exception across the three Latin treebanks, some 4,000 words; `is`,
`qui`, `quis`, `aliquis`, `quisquam` are pronouns, and `unus` splits
by treebank — numeral in Seneca, determiner in the charters — so they
keep their reading without the title) gives its determiner reading the
nativum title `determinantia`, except forms the treebanks tag
otherwise by shape: `tantum`/`quantum`/`ceterum` singular neuter and
masculine accusative are adverbs (113 to 2), `hic` nominative singular
masculine is the adverb (76 to 14) while `hoc`/`hac` are determiners,
`toto` is a charter name (16 to 2), and `solus` is dropped for the sun
and the soil. Rule 17 `determinans-primum-latinum` prefers that
reading, placed before the object umbra rules so the head rules bind
the determiner reading. Measured: Seneca 76.9 → 80.0, charters 79.7 /
79.3 → 83.9 / 83.2, English unchanged; the prior itself is right
95 % (Seneca 25 wrong of 520, charters 71 of 1,552); forced accuracy
76 %, since the adjective-versus-determiner confusion left the forced
tier.

EXECUTOR — THE LAW OF THE EXPLICIT CLASS. A `<praelatio>` decision
fixes the CLASS, not the case: the prior's first determiner reading of
`hoc` is the ablative, and first-wins would have frozen it there. An
`<impletio>` row on an explicit word is now accepted when it names a
reading of the decided class with the same lemma and language, or
its TWIN (same lemma, case, number, gender — the determiner copy of
the bound adjective reading), which then becomes first. Carriers
refine only through HEAD umbrae (agreement chooses the carrier's
reading: `hoc templum` nominative); fillers through head umbrae, or
through an object umbra whose carrier reading is the carrier's own
decided one (the object's case follows the preposition's decided
reading, not the order of the case rules: `in bona terra` stays
ablative, `in urbem` untouched); once per stage; a carrier with no
reading of its class is still bound and not preferred (as before:
`supra` + accusative), a filler is repetita. The first draft without
the lemma-and-language condition cost English 0.3 (a Latin reading of
`die` refined the English noun) and turned `in bona terra`
accusative; both measured, both fixed. Probes: `Hoc est` is a
determiner (the treebanks never tag `hic` PRON), `Hoc templum`
nominative with the accusative twin bound. 53 rules (prior at 17,
object rules 18–20, strict heads 21–36, loose 37–52); pins 799 / 838 /
831, forced 761 / 762 / 758. NEXT: loose rules that bind without
preferring (Seneca's distant tail), the head-rule template, the
ignoti reading.

**As built — T19k, the ignoti reading (2026-09-08, same session;
doctrine decision 37: an unknown word is a cell with all candidates,
never a missing cell).** A word no source knows now carries ONE
`analysis-ignoti` reading: lemma = the folded form, nativum
`ignotum`, source `regula`, no accidents, and NO language (a
`lingua_ignota` flag on the description keeps the slot unwritten, so
the language census, the `linguae` summary and the language rule
never see it); the `classes` summary stays `ignotum`, so every
selector and `Oratio.ignota()` keep working; `census.ignota` counts
the readings added. The reading fills any-case umbrae: rule 21
`umbra-obiectum-ignoti` (after the exact and the proper-noun fillers)
binds a Latin object umbra to the unknown neighbour. Two strict HEAD
rules for unknown heads were measured and dropped: 53–57 % on the
charters, 33–56 % on Seneca, and a tenth of primary lost everywhere
(an adjective homograph beside an unknown word is not an adjective
half the time). Executor: a decision on a word with a SINGLE reading
is vacuous and is no longer recorded (the cell was solved, not
decided), which is a definition change to the partition: the forced
tier on the charters had been inflated by single-reading fillers that
were trivially right, and stands honestly at 71 % (pins 765 / 710 /
711 / 913 / 913, cause named); the oracle counts an unknown word's
one reading as `nulla`, never `una`; the Python face agrees
(`ignota()[i].analyses == 1`, `analyses()` returns the reading,
`partitio()` puts it under `nulla`). Primary unchanged (80.0 / 83.9 /
83.2 / 78.6 / 79.0), lemma coverage up a little (the folded form of
an unknown word is sometimes the gold lemma). 54 rules; computus
golden regenerated (one node per unknown word). NEXT: the demotion
measured a fourth time (adposition prior removed, objectless
adposition readings demoted, now that unknown objects bind); the
head-rule template; verb slots.

**Measured and rejected — T19j and T19l (2026-09-08, same session).**
Head-class laws for Seneca's tail, each on all five treebanks: a head
must be a strong class (noun, proper noun, pronoun; Seneca +0.2,
charters −0.1 / −0.3); a weak head binds without preferring and the
law of umbrae ignores it (identical); only the strong twin with the
same accidents substituted (charters −0.2 / −0.1: `sancta` the noun
over the adjective); a loose-stage carrier whose crude first reading
is a noun or verb bound without preferring (Seneca −0.4, charters
−0.2 / −0.4); and earlier the noun-only head in the pattern (charters
−0.6 / −1.2). Lesson: a distant adjective binding at seventy percent
beats the crude order for those words even on Seneca, and the
charters' heads are proper nouns, pronouns and, for the eighteen
percent of unknown nouns, the agreeing adjective beside them. The
demotion a fourth time, on the complete grid: the adposition prior
removed (−0.1 / −0.3 / −0.2: the object rules take noun fillers only);
any case-bearing reading as object (+0.1 charters, −0.1 Seneca); an
explicit adposition with no object anywhere demoted (no change: the
alternative readings of an objectless preposition are junk — a
capital-rule name for a sentence-initial `A`, `de` as a noun, `super`
as a verb — and `cum` the conjunction is two words on Seneca). The
contradiction is sound now and worth nothing, because the reading that
should replace the preposition is not on the board. Decision (Fran):
the next structure is the CLAUSE — a `clausula` INDEX slot stamped on
elements, the sudoku box between adjacency and the whole sentence,
with the finite verb's slots as its laws and the gold dependency
subtrees as its oracle (ledger 01M1ZYZB67).

**Design — T20, the clause (2026-09-08, the conversation after
compaction; decisions 43–47).**

STRUCTURE. Genus `clausula` appended after `umbra`. Slots: `semen`
INDEX (ordinal of the seed element in the sentence; unwritten for the
main clause), `species` INDEX (appended enumeration principalis |
subordinata | relativa | coordinata | parenthetica — not `genus`,
which the umbra uses for gender), `pater` INDEX (position of the
enclosing clause in the sentence's list; unwritten at top level;
coordinate clauses are siblings, never nested), `umbrae` LISTA_NODUS
of umbra nodes written at BIRTH from the schema of the genus, not by
a rule: relation `verbum-finitum` (class verbum | auxiliare, mood
finite — the umbra genus gains an appended `modus` condition and the
relation enumeration the two relations) and relation `subiectum`
(nominative; number and person copied from the verb once it is
filled). The sentence gains `clausulae` LISTA_NODUS after `elementa`.
Every element genus (vocabulum, interpunctio, numerus) gains
`clausula` INDEX into that list and `clausula-causa` INDEX (semen |
extentum | clausura | catena | unica); unwritten = open. Punctuation
is a member, assigned to the clause it closes, and excluded from the
scores like range words. The registry seal moves with T20 as its
cause; the canon grows the genus and the two element attributes; the
projection shows `clausula="1" clausula-causa="extentum"` on elements
and the clause nodes after the elements; the computus golden is
regenerated; the Python face gets `Oratio.clausulae()` per sentence
and `clausula` / `clausula_causa` on the word object, as `decisio`.

STAMP. Module `oratio_clausula`, two entry points: `seminare`, called
by the resolver at its start (readings exist, bindings do not), and
`propagare`, called between resolution stages. Layer 1, SEEDS, data
rows with a cause and a measured count: `ORATIO_SUBORDINANTES_CERTAE`
(lemmas SCONJ without exception across the three Latin treebanks —
`cum` and `quod` are not on it and seed only with a finite-verb-
capable word between them and the next seed); relative forms (lemma
`qui`, `quicumque`; a sentence-initial relative is the connecting
relative and opens the MAIN clause); punctuation (comma, semicolon,
colon, parentheses, quotes — measured per treebank through `-semina`
before the list is fixed; the charters have almost none); coordinators
only when a finite-verb-capable word stands on each side before the
next seed (enclitic `-que` seeds at its host if the dictionary reports
it). Layer 2, EXTENTUM: seeds walked with a stack — a subordinator or
relative pushes (parent = top), a coordinator replaces the top with a
sibling, opening punctuation pushes a parenthetical until its closer;
every word between seeds joins the top; words before the first seed
are clause 0, principalis; a sentence with no seed places every word
with reason `unica`. Layer 3, CLAUSURA: a subordinate or relative
clause pops at its closing punctuation or, failing that, at its first
CERTAIN finite verb (all readings finite); the words after it and
before the next seed return to the parent with reason `clausura` —
the verb-final prior, expected to score lowest, carrying its own
reason for that purpose. Layer 4, CATENA (after each resolution
stage): an OPEN word bound by a head or object umbra to a placed word
joins that word's clause; the relative pronoun's own binding to its
antecedent is excepted by rule; bound to placed words of two clauses
= stays open; a span-placed word whose binding points elsewhere is NOT
moved in T20 but counted as a `discordia`. Finite-verb-capable = a
Latin verbum or auxiliare reading in the indicative, subjunctive or
imperative; English has no mood in Moby, so English sentences are
`unica` throughout in T20 and English seeds are a later tranche with
their own data.

ORACLE. Gold clause root = a token in a finite mood whose relation is
not aux/cop, or a predicate with a finite aux/cop child (UD hangs
`amatus est`, `bonus est` under the participle and the adjective);
coordinated finite verbs are each a root; no finite root = one clause
under the sentence root; a word's gold clause = its nearest root
ancestor, itself included (participial and infinitive constructions
belong to the finite clause above them). Scores per treebank in
permille, punctuation excluded: PURITY per layer (the majority gold
clause of each stamped clause; a placed word is right when its gold
clause is that majority; split by reason), COVERAGE (share of words
placed), COUNT (share of sentences whose clause count equals the
gold count), later VERB SLOT (the filled verb equals the gold root).
Purity and coverage pinned only rising from the gate's printed
number, down only with a named definition change; discordiae
reported, not pinned. Instrument: `oraculum.sh -clausulae` (the
table), `-errata -clausula` (misplaced word, its seed, its stamped
clause, its gold root), `-semina` (per mark and per candidate lemma:
how often on a gold boundary). Gate `probatio_oratio_clausula`:
planted fault = a seed removed from the list turns the pin red; hand
cases — nested subordinate closed by comma, closed by verb, coordinate
siblings, connecting relative, parenthetical, seedless sentence.

LAWS (T20c on; each measured on all five treebanks, kept only if no
treebank drops, in this order). (1) The VERB HIDDEN SINGLE: the
executor learns clauses as carriers with the clause's members as its
rows; the verb umbra is filled when exactly one member is finite-verb-
capable, that reading goes first, labelled `umbra` with the clause law
as author; two certain finite verbs in one clause is a missed seed
(asyndeton), a FINDING and a candidate fifth layer, never an
elimination. (2) `cum` THE CONJUNCTION: a `cum` whose span holds a
finite verb and which binds no ablative object gets its conjunction
reading first — the reading the demotion could not reach four times.
(3) LOOSE HEADS INSIDE THE BOX: a row cursor `cursus="clausulae"`
(rows = the members of the anchor's clause) on the sixteen loose head
rules; if it depresses a treebank the rules stay and the number is
recorded. (4) The SUBJECT umbra: a nominative member agreeing in
number with the filled verb, person from a pronoun or none; first use
= the nominative head rule prefers the subject's component.

TRANCHES. T20a: the genus and slots, canon and seal, layers 1–3 with
their lists measured first through `-semina`, the oracle table and
listing, the Python face, the gates with the planted fault, docs —
numbers and no laws. T20b: the chain layer, the discordia count, and
the override decision from the numbers. T20c: the clause carrier and
law 1; then laws 2–4 one at a time, each its own commit with its
measurement. LEFT OUT, named: candidate clause sets per word (returns
if the stamp proves too coarse), English seeds (own data), participial
and infinitive clauses as clauses of their own (return when a law
needs them), any phrase structure (decision 46).

**As built — T20a (2026-09-08, same session).** STRUCTURE as designed:
genus `clausula` after `umbra` (semen, species, pater, umbrae), the
sentence's `clausulae` list, `clausula` + `clausula-causa` on the
three element genera, one refinement — the umbra's new condition
slot is `forma-verbi` (finitum) rather than `modus`, since one value
expresses finiteness where mood would need three; registry 184 slots,
25 genera, seal 62a7afd8; canon, projection attributes, computus
golden (two umbrae and a clause node per sentence), Python face
(`Oratio.clausulae()`, `clausula` / `clausula_causa` on the word),
`verba.sh -clausulae`. STAMP `oratio_clausula` (`seminare`, called by
the resolver per sentence before its stages; idempotent). Seeds fixed
from the gold census (`oraculum.sh -semina`, agreeing line for line
with the exploratory Python census): CERTAE si quia qualiter dum quod
sicut sicuti donec quamquam quando quoniam antequam priusquam postquam
etsi quamvis quatenus quomodo quotiens quin; CORROBORANDAE ut cum nisi
ubi unde; RELATIVA qui quicumque quisquis on pronoun/determiner
readings, the forms quis/quid excluded (WORDS lemma `qui` for the
relative, the interrogative and the indefinite alike; `si quis`);
COORDINANTIA et atque ac sed aut nec neque at, between finite-verb-
capable words only; `-que` never (0–3 %). Punctuation never seeds
(comma 23–37 % on the charters; Seneca's treebank has none) — comma,
semicolon and colon CLOSE a closable clause, parentheses open and
close a parenthetical. Layer 2 with a stack and a per-clause
closability flag (subordinate, relative, and a coordinate sibling of
one); the main clause is born lazily at the first top-level word and
adopts earlier orphans (`Si quis ... , poenam dabit`); a sentence-
initial relative opens the main clause. Layer 3 at the first certain
finite verb (all readings finite) or a closer. ORACLE as designed:
gold root = finite mood not aux/cop, or the predicate of a finite
aux/cop, or the sentence root; purity per layer, coverage, count
agreement, `-errata` listing (layer, form, our species, gold root).
BIRTH NUMBERS (Seneca / charters dev / test): semen 890 / 957 / 953,
extentum 759 / 857 / 887, clausura 491 / 650 / 657, unica 740 / 984 /
986; overall purity PINNED 738 / 829 / 864 (English 726 / 739, all
`unica` by design); coverage 1000; sentences with the gold count 518 /
740 / 771; clauses per sentence 1.68 vs 2.27, 2.48 vs 2.52, 2.37 vs
2.25. Primary and forced accuracy unchanged (no rule reads the clause
yet). FINDINGS: the closure layer is the weakest as predicted and
carries 29 % of the charters' words; the residual on every Latin file
is a SECOND FINITE VERB WITH NO SEED before it (Seneca's imperatives
and verse, the charters' formulae "alio lato tenet in terra", "ego
... manifestu sum") — law 1 of T20c as a fifth layer. Gate
`probatio_oratio_clausula` (161: data lists, ten hand cases including
nested-closed-by-verb, coordinate siblings, `si quis`, seedless,
connecting relative, negated corroboration, parenthetical, English,
idempotence, umbrae, projection round trip, resolver census; planted
fault = si removed, red on `si quis`). NEXT: T20b.

**As built — T20a bis, the verb split (2026-09-08, same session; law 1
of the design brought forward as a fifth layer, Fran's choice on the
birth listing).** A certain finite verb (all readings finite) in a
clause that is not closable and already holds its verb opens a new
COORDINATA clause with the verb as seed; the verb and the words after
it up to the next seed carry the reason `verbum` (appended to the
enumeration). The cut position was measured, not chosen: A = the
words between the two verbs join the new clause (verb-final order),
B = the cut falls right before the second verb. Same clause count in
both; A better on every Latin file — overall purity Seneca 738 → 822
(B 805), charters 829 → 848 (845) and 864 → 881 (875); count agreement
Seneca 518 → 65x, clauses per sentence 1.68 → 2.07 against 2.27 gold;
the charters now over-segment slightly (2.46 vs 2.25). The layer
scores 69 / 69 / 76 % on its own words and lifts `extentum` and
`unica` by taking their worst words. Gate: three hand cases (asyndeton
without seeds, `Puer currit, puella cantat` with a comma, no split
inside a subordinate; `Veni, vidi, vici` does not split because `veni`
carries a non-verb reading). NEXT: T20b; the closure layer's early
`est` on the charters is the next measured target.

**As built — T20a ter, the closure layer and the decision measure
(2026-09-08, same session).** A scratch view of charter sentences
with our clause beside the gold root showed four shapes behind the
"early `est`": the formula `qui supra` (a verbless relative that UD
keeps in the main clause), the copula closing before its predicate,
sentence-initial `unde` as a connective, and a coordinator before a
seed; a fifth came from `tenet`, uncertain only because Moby lists an
English noun. MEASURE FIRST: purity rewards over-segmentation (a
one-word clause is pure) and the sentence count sees no order, so the
oracle gained PAIRWISE CONCORDANCE (Rand over word pairs within a
sentence: do we and the gold agree that two words share a clause),
which is now the pinned decision floor; count agreement stays
reported. Each variant alone, from 799 / 769 / 781 (Seneca / charters
dev / test): `qui supra` no seed = / +5 / +2 permille; a COPULA (lemma
`sum`) does not close a closable clause that has no verb yet +2 / +10
/ +2; a non-seeding coordinator directly before a seed joins the
seeded clause = / +1 / +3; certainty over LATIN readings only +19 /
+10 / +5; all four +21 / +43 / +18 (superadditive on the charters);
initial `unde/ubi` no seed −2 on dev alone and nothing in combination:
removed. Latin-only certainty exposed that the stamper never knew the
document language: `it`, `sit`, `do` are finite Latin verbs and split
English sentences; the resolver now passes the language and an
English document is not seeded by Latin readings at all (parentheses
only), which moved the English purity pins down two permille with
that cause named. Final pins: purity 850 / 870 / 893 / 724 / 737,
concordance 819 / 811 / 798 / 539 / 547; clauses per sentence Seneca
2.22 vs 2.27, charters 2.65 vs 2.52 and 2.51 vs 2.25 (over-segmented:
the verb split inside formulae is the next listing to read); the
closure layer itself 659 / 738 / 692, still the weakest. Gate: four
hand cases (`qui supra`, copula not closing, coordinator before `si`,
`tenet` certain) and the English document unseeded. NEXT: T20b.

**As built — T20a quater, the sentence view and the split's extra
clauses (2026-09-08, same session).** The instrument gained
`-clausulae -sententiae N [-causa T]`: N sentences holding a misplaced
word of layer T, with the text and, per gold word, our clause, the
layer and the gold root (records collected by the oracle,
`census->ostensae`). On the charters' `verbum` layer it showed four
shapes; measured alone by concordance from 820 / 812 / 799: the fixed
phrase `id est` (its `est` neither certain nor capable: = / +4 / +2
permille) and a proper noun after the closing verb kept inside the
clause (`ubi vocitatur Iuveiano`: = / +1 / +1), together = / +5 / +4,
KEPT; a clause without its verb not closing at a comma (appositive
commas: +8 on test, −1 on dev alone and −6 on dev beside the two)
and the coordinator corroboration scanning through commas (−9 on
test), REMOVED with the numbers recorded. Seneca unmoved by all four.
Concordance pins 820 / 816 / 802. Gate: two hand cases. NEXT: T20b;
the charters' remaining over-segmentation (2.58 / 2.44 vs 2.52 /
2.25) through the view on `extentum`.

**As built — T20a quinquies, the extentum view (2026-09-08, same
session).** Three shapes on the charters' test file, each measured
alone by concordance from 820 / 817 / 803: `una cum` never seeds
(gold: 112 of 112 prepositions, never a boundary); forms of `quantus`
seed a relative clause (`quantum ... est pertenentes`: = / +2 / +3);
`cum` corroborated by a CERTAIN or SUBJUNCTIVE finite verb when the
next word carries an ablative reading (the preposition's shape), by
any capable verb otherwise — the certain-only variant (−1 Seneca /
+10 / +9) and the subjunctive variant without the ablative test (−1
Seneca at the gate's resolution, 39 pairs) were both measured and
refused by the rule that no treebank may fall. Final 820 / 826 / 809.
PURITY is now reported, not pinned: it fell 3 on the test charters
while concordance rose 7 and the count moved toward gold, the same
one-sidedness that demoted count agreement; concordance is the only
pinned decision measure. Gate: `una cum`, `quantum` hand cases.
T20a sexies (Fran): the concordance pins are the EXACT concordant
pair counts, not truncated permille (a 39-pair loss was visible or
not depending on where the truncation boundary fell); the loop reads
the machine rows.

**As built — T20b, the chain layer (2026-09-08, same session).** The
span layers leave no open word, so the chain could only count and
override. `oratio_clausulas_propagare`, called by the resolver after
all stages, reads the bound umbrae of every word's first reading; an
edge whose ends are placed in different clauses is a DISCORDIA (the
relative's binding to its antecedent excepted); the override moves
the end whose layer is weaker by measured purity (semen > unica >
extentum > verbum > clausura) into the other's clause with reason
`catena`. Measured in exact pairs against the count-only base
(102551 / 438519 / 444139): adjacent bindings, weak layers only, −33
/ +17 / −6 moving 5 / 1 / 2 words; adjacent any layer −39 / +17 / −6;
any distance −1142 / −7363 / −4926 moving 265 / 617 / 495; any
distance any layer −1203 / −7419 / −4980. Adjacent bindings do not
cross clauses (spans are contiguous); distant ones cross 547 / 1016 /
956 times and are wrong more often than the span. DECISION: the chain
stays a counter (override at zero, numbers recorded), `catena` places
nothing, and a crossing binding is read as evidence against the
BINDING — law 3 of T20c. The oracle reports discordiae and catenatae;
the gate asserts catenatae == 0. NEXT: T20c.

**As built — T20c I, the box against the loose heads (2026-09-08,
same session).** Law 3 went first because T20b pointed at it. Two
mechanisms: the executor predicate `_trans_clausulas` refuses an
impletio row whose ends are stamped into different clauses (the
relative's binding to its antecedent excepted; switch
`RECUSATIO_TRANS_CLAUSULAS`, variants I–VI), and the pattern itself,
`clausula="$c"` on both words of the sixteen loose head rules (a
repeated capture is equality), in a hard form and a soft one (in-
clause copies at stage 3, the unrestricted rules as a stage-4
fallback). Primary / forced permille against the base 799/765,
838/710, 831/711 (EWT never moves): executor, pure layers 798/769,
838/711, 832/716; any layer 795/775, 839/718, 833/727; no coordinate
clause 798/765, 838/711, 832/714; pattern hard 797/782, 841/723,
834/729; pattern soft 799/764, 837/707, 830/707; and the CONTROL
without the box — refuse whenever a certain seed word lies between
the two words, stamps ignored — 797/772, 841/726, 834/731.
FINDINGS: the soft form loses, so the loose rule's nearest candidate
is already inside the box and refusal is the only lever; the refused
bindings choose the right reading 71 % of the time, better than
Seneca's default and worse than the charters' — an uncertain cell,
decision 37 measured rather than argued; and the control equals the
box, because the box's only information beyond the seeds is the
closure layer at 66–74 %. DECISION: every variant depresses Seneca
primary at the gate's resolution, so none is kept — the predicate
and the control stay in the executor at zero with the numbers, the
pattern file is unchanged, and the forced gain (up to +18 permille)
is the price the Seneca pin holds. Law 3 as designed is not built;
any later clause law must beat the seed-between control. Laws 1, 2
and 4 use membership, which the control cannot express. NEXT: law 1,
the verb hidden single.

**As built — T21, the enclitic split (2026-09-08, same session).**
The residual census showed 302 Seneca words in multiword ranges,
wrong by construction: the enclitics `-que` (265), `-ne`, `-ve`, which
the dictionary already reads (contract III: tackon only when the
whole form is unknown and the form minus the tackon is known, the
TACKON analysis first) but the parser kept as one element. A pass in
`oratio_partes_annotare` (`oratio_partes_encliticum_scindere`) splits
such a word — one part, first dictionary analysis TACKON, whole form
unknown to Moby (`unique`, `mosque`) — into a host and an enclitic
element: `oratio_lexema_pars` slices the token with exact byte offset
and column (emission == source), the host's trailing spaces move to
the enclitic, the enclitic gets its one TACKON reading (coordinator
for `que`/`ve`, particle for `ne`) with summaries, the host is
annotated afterwards by its own form, the sentence's element list is
rebuilt with the enclitics inserted (`materia_nodus_reponere`).
Whole-word entries (`quoque`, `neque`, `itaque`) never split. The
oracle judges a range component whose form equals exactly one of
our elements in the range's span against that element as a first
word; otherwise the old path (EWT's contractions). Registry and seal
unchanged. NUMBERS: Seneca primary 799 → 834 permille, ranges
302 → 31, concordance 102551/125027 → 109193/132303 pairs; charters
838/831 → 840/833 (`Lucane` = medieval `Lucanae` split 67 times into
`Luca` + `ne` until `lucanus` entered the glossary — a dictionary
hole, not a rule); EWT unchanged. Seneca FORCED 765 → 764 with the
cause named: the enclitic element stands between its host and the
next word, so the strict adjacent head rules see five fewer pairs
and the loose rules take them less accurately (`senatus`, `que`,
`populus`). Named, not built: the strict cursor transparent to an
enclitic coordinator (pattern-machine question, T21b); `utraque`
splits into `utra` + `que` until PACK tackons (T8b); `pleve`. Gate:
partes hand cases (split, unsplit, `-ne`, `-ve`, migrated tail,
emission == source), oracle range case, pythonica Hilarius 1682.

**As measured — T20c law 1, refuted by census (2026-09-08, same
session).** The verb hidden single was censused before being built
(our readings, finiteness and clause stamp aligned to gold). Reach:
68 / 25 / 24 gold finite verbs carry a verb reading that is not
first. Sentence-level single (one finite-capable word in the
sentence): +3 / −8, +1 / −2, +0 / −6. Clause-level single: net −49 /
−53 / −74, the losses nouns, determiners, pronouns and non-finite
verbs — the clause holds no finite verb, or holds a certain one
already (791 Seneca clauses). NOT BUILT. The verb errors are
participles read as adjectives or nouns (Seneca 152 of 253) and, on
the charters, unknown medieval forms (409 of 597). The participle-
first twin prior was censused too and refused (+154 / −269 Seneca,
+161 / −984 charters; `sancti`, `casa`, `censum`, `suprascripta`).
The charters' unknown forms yield to medieval orthographic
correspondences (1869 of 3145 recovered, 1697 with the gold class),
the next dictionary-level tranche. Laws 2 and 4 remain unmeasured.

**As built — T22, medieval orthography as contract IV (2026-09-08,
same session).** `oratio_vocabularium_la_quaerere_variantes`: variant
forms of the folded form under the data table `ORATIO_ORTHOGRAPHIA`
(one correspondence at a time in table order, one occurrence at a
time left to right; modes substitution, prefix, insertion,
doubling), each run through the full lookup; the first variant with
readings wins and its analyses carry `varians`; the describer marks
them with the new source `orthographia` (registry value 4, canon
note; seal unchanged). The dictionary never varies on its own: the
annotator asks only when the form is unknown and Moby does not know
it whole; capitalised forms are excluded (names stay names by the
capital rule); the identifier lint is untouched. MEASURED alone on
all five treebanks and kept only where no floor falls: h-prefix,
e→ae, inp→imp, b→p, and e→i behind them (alone it dropped the charter
forced floors by seven; after the h-prefix and e→ae take its words
it holds them). Refused: doubled consonant and d→t (EWT forced
falls), o→u, i→e, p→b (charter forced falls), inb→imb (no effect),
h inserted after c/p/t and t→d, h removed, u→o (census). Order by a
lemma census (h-prefix first: `abere` → `habere`, not `abire`).
NUMBERS: charters primary 840/833 → 863/856, unknown words 1622 →
1009 and 1664 → 1061, forced 710/711 → 712/713; Seneca 834 → 835
after `mecum`, `temet` and kin became glossary entries (their
variants were `mica` and `timet`; the gold splits `mecum` as
`me` + `cum`, T8b pronoun tackons are the real fix); EWT unchanged.
Environment switches `ORATIO_ORTHOGRAPHIA_SOLA` / `_SINE` isolate one
correspondence for the loop. Named: `petia`, truncations, two-change
forms (`presbitero`, `pegiorata`), a charter vocabulary glossary.

**As built — T23, the case measure (2026-09-08, same session).**
Primary accuracy judges the first reading's CLASS only; a noun in
the wrong case counts as right. The oracle now carries CASUS:
population = gold words bearing `Case=` whose class is right and
whose first reading is Latin with a case slot; right = the first
reading's case equals the gold's. Conditional on class by design, so
it measures case disambiguation alone. Counted globally, per class
and per deciding rule; printed in the human tables (`CASUS`, the
class table's `casus (n)` column, `casus N %` on the author lines);
appended as two columns to the `SUMMA`, class and `AUCTOR` machine
rows; pinned per Latin treebank, only rising: 609 / 565 / 567
permille (Seneca 3312 of 5433, charters 5176 of 9156, 5324 of
9387); EWT reported (0 of 0). FINDING: class 83.5 % but case 60.9 %
on Seneca, 56.5 / 56.7 % on the charters; the nominative head rules
choose the right case 52 % and 45 % of the time. The Latin problem
is case. Law 4 (subject) is measured against this number.

**As measured — T20c law 2, refuted by census (2026-09-08, same
session).** Our seed decision for every `cum` against the gold class:
Seneca 26 (seeded conjunction 17, seeded preposition 7, unseeded
preposition 2); charters 106 and 124, all but four prepositions, 12
and 29 of them seeded (`cum uno lato tenet`). The law (seeded →
conjunction first) would gain 17 / lose 7 on Seneca and lose 11 and
27 on the charters. The box-free control (a finite-capable word
follows) is equal on Seneca and loses 103 / 116 on the charters. A
seed refinement, `cum` before an ablative-only non-verb never seeds,
cuts the false seeds to 3 / 15 but loses 312 concordance pairs on the
dev charters (+157 Seneca, +1021 test): refused, switch kept at off
with the numbers. NOT BUILT. Law 4 remains, measured against the
case column.

**As measured — T20c law 4, refuted by census; the case prior found
(2026-09-08, same session).** Subject as a hidden single (one certain
finite verb, one nominative-capable member agreeing in number):
reach 165 / 44 / 54 gold subjects with a nominative reading not
first (882 / 699 / 710 subjects, 626 / 378 / 372 already right; the
charters' subjects mostly lack the nominative reading); sentence
level +24 / −11, −1, −3; clause level +65 / −64, −55, −56 — the one
agreeing candidate is the object as often as the subject. NOT BUILT.
All four clause laws are now censused: 1 and 4 fail on membership, 2
and 3 on boundaries; the clause remains an instrument. The case
CONFUSION MATRIX (Seneca 1945 wrong of 5002): accusative read as
nominative 574, ablative as dative 367 and locative 134, genitive as
nominative 160; charters add genitive as vocative 197; the gold has
one locative and few vocatives. 799 of Seneca's are open-tier
dictionary order; 1297 charter errors have no reading of the gold
case at all. CASE PRIOR CENSUS: majority gold case per candidate set
would give 73.4 / 73.8 / 76.4 % (today 61.1 / 56.7 / 56.9); one
global order abl > acc > gen > nom > dat > voc > loc gives 65.3 /
71.7 / 72.9; frequent sets agree across files except abl/acc/nom/voc,
abl/nom/voc, abl/acc (Seneca nominative or accusative, charters
ablative). NEXT: T24, a case prior that ORDERS readings within a
class (never eliminates), global order plus per-set entries each
measured alone on all five treebanks.

**As built — T24, the case prior (2026-09-08, same session).** An
executor law in the resolver, `_prior_casuum`, run LAST — after all
stages, the law of umbrae, the written decisions and the chain — on
words nothing touched: not decided, not carrying a filled umbra, not
the target of one (binding indices point into the target's list).
It reorders the readings of the word's FIRST class, each into the
positions that class already held, by a measured case rank
(`ORDO_CASUUM`: ablative, accusative, genitive, nominative, dative,
vocative, locative) with per-candidate-set entries (`PRIOR_COPIAE`,
switches, env `ORATIO_PRIOR_CASUUM_SOLA`); other readings stay put,
no class moves, no decision is written (`census.prior_casuum` counts).
MEASURED before the rules first and refused: primary Seneca 835 → 808,
charters 863 / 856 → 822 / 811 — the loose head rules bind spurious
ablative agreement and promote the wrong twin; a prior may not change
what the evidence rules see. After the rules, primary and forced are
identical to the base on every file and only case moves: base 609 /
565 / 567; global order 636 / 617 / 630 (kept); entry abl/acc/nom/voc
→ nominative 645 / 622 / 637 (kept); abl/nom/voc → nominative 640 /
617 / 629 (test falls, refused); abl/acc → accusative 648 / 596 / 606
(charters fall, refused). Pins 645 / 622 / 637. Two defects caught by
house tools: an unsigned group index initialised to −1 (the examen's
"comparatio vana") and class consolidation (`Cum`, caught by the
gate's permutation count). The remaining distance to the census's
73 % is the bound tier: the head rules' own case choice (52 / 45 %),
a rule question, not a prior.

**As built — T25, the pair policy (2026-09-08, same session).**
`_ordines_ordinare` reorders each rule's impletio rows within groups
of the same carrier and umbra: nearest partner first, then the partner
reading's case rank (`ORDO_CASUUM`), then original order; a stable
sort before the first-wins loop (switch `POLITICA_PARIUM`: 0, I
distance, II distance and rank). NULL RESULT on readings: primary,
forced and case identical on all nine treebanks (five vendored, four
on the shelf); the policy moves one to four remote partners per rule
and both candidates always agree with the same reading. Kept on as
the principled attachment default. NEXT measure: ATTACHMENT in the
oracle (head umbra: partner == gold head of the carrier; object
umbra: carrier == gold head of the partner), which the policy, the
remote head rules and the dependency design all need.

**As built — T26, the attachment measure (2026-09-08, same
session).** Each element remembers its aligned gold token; after
alignment `_ligationes_iudicare` walks every aligned element's first
reading and its filled umbrae with an aligned partner. Head umbra:
right when the carrier's gold head is the partner. Object umbra:
direction by the carrier's class — a preposition, particle or
auxiliary hangs from its object in UD (case, mark, aux), so right when
the carrier's gold head is the partner; a verb's object hangs from the
verb, right when the partner's gold head is the carrier. PRECISION =
right / asserted, pinned per file only rising; RECALL = right / gold
arcs among aligned non-punctuation words, reported; split by head
umbrae and adjacent partners. Human `LIGATIO` line, machine `LIGATIO`
row. NUMBERS: precision 431 / 364 / 377 permille on Seneca and the
charters, 815 / 826 on EWT; recall 95 / 95 / 98 and 19 / 22; adjacent
586 / 730, head umbrae 408 / 363. FINDING: we assert about a fifth of
the gold arcs and get under half of those right; adjacent bindings
are fair, remote ones poor; subjects, verb objects, adverbials and
coordination are never drawn. This is the measured distance to the
dependency design. Named: attachment per deciding rule (the umbra
would carry its author).

**As built — T27, the retry door measured (2026-09-08, same
session).** The STML matcher's joint backtracking (exemplaria spec
decree 21, `quaesitio="completa"`) was built and run against every
rule as written on the five treebanks. Primary and forced identical;
case 645/622/637 → 641/619/635; attachment precision 431/364/377 →
438/359/374; the law-of-umbrae tier grew on every Latin file. Cause
from the author table: the nominative head rules gained (798 → 823,
strict 319 → 361) what the accusative ones lost (125 → 87), because
retry lets a literal-case rule claim a carrier's rarer readings of
its case before the next case's first reading, and FIRST READING PER
CASE is an informative prior (two to four permille). DECISION: the
search is opt-in per EXEMPLAR, the resolver default stays greedy, no
rule changed, pins identical. FINDING on rule growth: number and
gender were already captures; case is the only literal dimension
(28 head rules where 4 suffice, 2 object rules where 1), so the
tracked features grow rules linearly already and the collapse is
worth 28 → 4. Next: unified rules with the attribute, measured
beside the literal set; if the loss returns, an enumeration order
(each case's first reading before any case's second) is the named
policy to measure.

**As built — T27 b, the strict head tier unified (2026-09-08, same
session; decision Fran's).** The one-hour measurement was run and it
overturned T27's reading. The engine gained `quaesitio="omnes"`
(every completion per start is a row; exemplaria decree 21). Nine
treebanks, seven variants: all thirty rules unified (object rule
loses 50–150 bindings per file: refused); head only (case up, charter
attachment −8/−11); head only with every completion and nearest-first
(Seneca case 698 but charters −17 attachment, Aquinas −19 case); the
LOOSE tier alone carries all of that loss; the STRICT tier alone is
up or equal on every column of every file except the charters' forced
ratio, −1 each (the law-of-umbrae fallback grows: a row for a losing
reading still binds and decides its partner; refusing such rows was
measured and is far worse); strict with first completion only is
worse than every completion on every charter column; distance-only
pair ordering is worse than distance-then-case-rank on every file, so
the T25 policy's case rank is real work. ADOPTED: 14 strict rules →
2 (`$cas` on both nodes, `quaesitio="omnes"`, stage 2), 54 → 42
rules; loose tier literal. Pins: case 662/644/665, attachment
437/368/378, forced 765/711/712 (charters −1 NAMED CAUSE); shelf case
+21/+3/+9/+10 and attachment +5..+8. WITHDRAWN: "first reading per
case is a prior" — dictionary order with every completion beats rule
order; what T27 a measured was nominative-first order plus retry.
Known miss: `Puella bona ambulat` now reads ablative (equal-distance
agreeing pairs tie-break by ORDO_CASUUM, ablative first — better on
nine files, wrong here); the pythonica hand case moved to `Puerum
bonum videt`. Refused for the record: object unification, own-reading
refusal, policy I, strict `completa`.
ADDENDA (2026-09-09): (i) `_ligationes_remittere` — a stage that
permutes a word's readings remaps every umbra pointing at it (stale
indices found by the resolutio probatio's discordance check: two on
Hilarius, both class-explicit partners re-emended at stage 3); no
number moves, discordes 0. (ii) `EMENDATIO_SEMEL_PER_SENTENTIAM`
(off, measured): emending a class-explicit word once per sentence
rather than once per stage gives attachment 441/386/394 and shelf
+10..+12, case up on eight files, Seneca case −2 — ADOPTED
2026-09-09 (Fran), Seneca case pin 662 → 660 named; pins now case
660/654/671, attachment 441/386/394. (iii) `ORDO_CASUUM_PARIUM`: the pair policy's case rank as
its own table; nominative-first orders lose 6–19 permille of case,
distance-only loses 7–15, so ablative-first stands and the hand
sentences `Puella bona ambulat` / `Hoc templum est` read ablative and
accusative, recorded as the price.

**As built — T28, the loose tier censused; LEX I CLAUSULAE adopted at
level II (2026-09-09, decision Fran's).** Gold census: the nearest
agreeing nominal is the head 96–99 % of adjective/determiner arcs on
all seven Latin files given true agreement — the law is right, the
readings are not; a verb intervenes in 30 % of Seneca's arcs, so the
clause may not be a MATCH condition for verse. Unifying the loose tier
loses attachment under every ordering tried (four, incl. a per-rule
`politica` attribute, reverted): the literal tier stays, destined for
the §8.5 template. T20c's clause refusal, re-judged on the attachment
column it lacked: level II gives attachment 462/398/412, EWT dev 817,
shelf +16..+44, forced 769/713/717, case 666/653/671 (charters dev
654 → 653 NAMED), primary and recall unchanged. Pins moved
accordingly.

**As built — T29, the feature columns (2026-09-09).** Oracle table
`NOTAE_ORACULI` (Number → numerus, Gender → genus, Person → persona,
Mood → modus, Voice → vox, VerbForm → forma-verbi, Tense → tempus via
Tense+Aspect); per-key census `notae_verba/recti`, instrument `NOTA`
lines and rows, gate prints seven and pins number and gender
(914/837, 928/813, 933/810). Voice: deponent accepted for Act and
Pass (convention, named). First reading of the numbers: gender is the
next Latin problem (81–84 %), then charter verb form and voice
(83–87 %); person, mood and tense are above 93 % everywhere.

**As built — T30 a, the gender census (2026-09-09).** Feature errata
in the oracle: `oraculum.sh -errata -nota <accidens|omnes>` lists
every class-right word whose first reading carries the wrong value —
form, class, gold and our value, decision and rule, binding partner
and distance, and ATTINGIBILE (a Latin reading of the class carries
the gold value) or absens; machine rows `ERRATUM-NOTA`; the element
keeps a bitmask of reachable values; gate law: an accident's errata
sum to verba − recti (planted fault red). Two oracle CONVENTIONS,
Fran's decision, both named in the code: gold multi-values
(`Gender=Fem,Masc`) any accepted, replacing a substring reader that
saw only the first (Seneca 837 → 847); `commune` accepted for gold
Masc and Fem as deponent voice is for Act and Pass, with
`conventione N` (right by the alternate only) printed on every
feature line in the gate, the instrument and the `NOTA` row so the
convention cannot hide. Gender pins 897 / 890 / 885 (by convention
248 / 584 / 588); shelf 856 / 901 / 923 / 866; voice's deponent
count visible (163 / 48 / 45). The census that decided it (Seneca /
charters dev share of wrong genders): commune 32 / 41 % (PROIEL
60 %, personal pronouns marked masculine); other unreachable
16 / 24 % (dictionary holes; known medieval forms `sancte`, `bone`,
`suprascripte` whose vocative reading blocks the `-ae` variant, since
T22 asks only for unknown forms); homonym lemma order 20 / 5 %
(`armus/arma`, `caelus/caelum`, `tela/telum`, `census/censum` — WORDS
frequency codes are in the dictionary, not on readings); gerundive
listed before the neuter reading 7 / 12 %; determiner prior agreeing
with the partner's rarer reading 8 / 11 %; head rules 13 / 6 %, the
smallest bucket. Next by that ranking: a frequency prior for homonym
lemmas, the medieval variant on known forms (T22 b), the gerund
reading; a derived-gender count through the agreement partner if the
convention is to be earned rather than granted.

**As built — T30 b, the frequency order (2026-09-09).** Every reading
description carries the WORDS frequency code of its stem
(`OratioDescriptio.frequentia`; zero for glossary, unique, rule and
English readings). Before nodes are built, the annotator stable-sorts
the readings of each (class, language) group among their own
positions by frequency rank (A … F, rarer codes last, X in the
middle, no code first); class and language order are untouched, so
the first class of a word cannot move and nothing is eliminated;
equal ranks keep dictionary order. It runs BEFORE the rules, unlike
the case prior (T24), because it changes no case, number or gender
evidence — only which reading of a case is met first — so the rules
bind the same evidence with the frequent lemma, and bound words are
repaired too. `ORATIO_ORDO_FREQUENTIAE=0` disables it for measurement.
Nine treebanks, off → on: primary identical everywhere; gender Seneca
897 → 907, charters 890 → 895 and 885 → 892, shelf +6/+3/+8/+3;
Seneca number 914 → 917 and attachment 462 → 463; case and forced
level. Pins gender 907 / 895 / 892, Seneca number 917, Seneca
attachment 463. The `conventione` count showed that on the charters
about 120 / 150 words moved from strictly right to right by
convention: `testes`, the witnesses, now read WORDS's common-gender
`testis` entry before its masculine one — the same lemma. Not done:
inflection frequency as a second key, the age code, derived gender.

**As built — T30 c (T22 b), medieval variants on known forms
(2026-09-09).** A document has a DIALECT (`OratioDialectus`:
ignotus, classicus, medius), decided like its language (T18) from
evidence the pipeline already produces: `oratio_partes_praescandere`
counts the words T22 recovers as unknown medieval spellings (one
cached lookup per folded form); `oratio_partes_dialectus_censu` says
medius at five per thousand (charters 29, Dante 12, classical ≤ 1);
`oratio_partes_annotare_dialecto` takes the dialect or decides it for
the subtree; the oracle decides per treebank file in the language
census pass and hands it to every sentence (`DIALECTUS` line and row,
`census.dialectus`). In a medieval document a KNOWN form also asks
for variants, through the correspondences flagged `notis` in the
table — only `e-ae`; `h-praefixa` and `e-i` measured on known forms
lose everywhere — and keeps a variant reading only if its WORDS part
of speech is among the form's native readings (`que` stays a
conjunction). Variant readings come after the native Latin ones
(their own group in the frequency order), source `orthographia`,
census `orthographia_notae`. Measured against the committed state:
charters dev / test primary 863 → 865 / 857 → 858, forced 713 → 719 /
717 → 728, case 653 → 662 / 671 → 685, gender 895 → 913 / 892 → 912,
attachment 398 → 413 / 412 → 428; number 928 → 925 / 933 → 932, the
one fall, cause named (the loose tier's nominative-first literal
order binds `sancte … ecclesie` as a plural pair), Fran's decision;
Dante gains case, number, gender and attachment; every classical file
and EWT byte-identical. Not done: capitalised known forms, the loose
tier's order, a sentence-level dialect.

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

**Stage 3 (partes).** T11 DONE 2026-09-04 (`partes_registrum`: 17
analysis genera with accidents appended, canon rules, seal moved).
T12 DONE 2026-09-04 (`oratio_partes_la` mapping, `oratio_partes`
annotation pass, node-attribute hook, `-partes`). T13 DONE 2026-09-04
(`oratio_conllu`, `oratio_oraculum`, CIRCSE + LLCT dev/test vendored,
coverage pinned 937/889/882 permille, mapping pass II from the table).
Was: `partes_la.c` mapping + annotation pass (`analyses`, `classes`,
`linguae`) + projection attributes (substrate check above). T13
CoNLL-U reader + `probatio_oratio_oraculum` over CIRCSE + LLCT
(COVERAGE pinned, PRIMARY reported, per class); `oratio/oraculum.sh
-petere` for NC treebanks. T14 DONE 2026-09-05 (`oratio/verba.sh`
with `-analyses`, `silva.Oratio`, `Prosa.sententia` by delegation; gate
= pythonica).

**Stage 4 (English).** T15a DONE 2026-09-04 (vendor Moby verbatim,
loader without coction, the prose lint `-prosa`). T15b DONE 2026-09-04
(morphology as DATA rows with measured counts, prefixes, compounds,
glossary pass II, the prose pin as a share ceiling). T16 DONE
2026-09-05 (`oratio_vocabularia`, `oratio_partes_en`, both dictionaries
in the annotator, EWT oracle 91.3 / 91.8 %, English pass II; no
irregulars table needed).

**Stage 5.** T17 DONE 2026-09-05 (the walking skeleton: two adposition
rules, executor, verbs, canon, pins). T18 DONE 2026-09-07 (the
language rule as a two-stage chain, first in the program; lex casus of
Moby records; word-vote census; oracle censuses the treebank as one
document; EWT primary 56.6/56.9 → 67.8/68.0). T19 class-prior rules
(pass II readings) and agreement rules; the sibling-run door pulled
when the oracle shows the loss.

**Stage 6.** T20 `lemmata` column in gesta FTS + tabularium write path.
T21 recall measurement on a fixed query set. (Renumbered 2026-09-07:
the stage-6 rungs were labelled T19/T20 before T6b, T15a/b and T18
were inserted; T19 is the stage-5 class-prior and agreement task.)

About 20 tasks; T3 (sentences) and T8 (Latin lookup) are the hard
ones. Re-entry: this file, `oratio-interview.md`, the ledger parcum
(created with this commit).

**Stage 1b (added 2026-09-04, after T6).** T6b `forma` classifier:
`ORATIO_PARAGRAPHUS_FORMA` INDEX slot (append; seal + canon pin
updated with cause), `oratio_forma.c` (evidence from the paragraph's
lines: count, lengths, variance, capital starts, unpunctuated ends;
rules as a table), reader consults it (`versus`/`tabula`: line = unit;
`titulus`: one unit), gate `probatio_oratio_forma` (fixtures: verse,
tables, headings, wrapped prose; CIRCSE boundaries once vendored in T13
— until then the five fixtures), fissio report re-run. Precedes T7.
