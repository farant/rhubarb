# oratio — orientation

*You are reading this because you touched a file in `oratio/`.*

Natural-language (prose) parser on **materia** (fourth client after the
C89 shim, css and md) with cross-language word classification. Design:
`project-specs/oratio-spec.md` (v1 + v2 codebase pass, as-built notes
per stage); interview `oratio-interview.md`; ledger parcum 01M1NH3F46.
Stance: moderate realism — classes earned by function, edge cases by
analogy, ambiguity as LISTS at every layer, unknown word = FINDING.

## Status (2026-09-08)

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
contextus latinus). Identifier unknowns were PINNED as a count only
falling until 2026-09-07; the pin is now a committed SET (see the
2026-09-07 note below). T8b stays parked. **T15a DONE (warmup, decision 27)**: the
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
858 ms — pronoun packings dominate (`quis` 215). **T13 DONE**: the
oracle. `oratio/probationes/fixa/ud/` = CIRCSE test + LLCT dev/test
(CC BY-SA, licences + READMEs + FONTES.md; NC treebanks via
`./oratio/oraculum.sh -petere`, reported only, never vendored).
`oratio_conllu` (reader: ranges, empty nodes skipped, `# text` or
reconstruction) and `oratio_oraculum` (sentence parsed + annotated,
elements by byte extent, gold forms located in the text, ranges judged
per word; TECTUM = coverage PINNED only rising in permille per file,
PRIMARIUM + LEMMA reported, `-exempla` prints the uncovered). Mapping
pass II from the per-class table — SECONDARY readings appended after
the primary, data lists in `oratio_partes_la.h` with the counts as
cause: `sum` + auxiliare, `ORATIO_DETERMINANTIA` + determinans,
`ORATIO_PARTICULAE` + particula, subordinating adverbs/prepositions +
coniunctio-subordinans, ordinals + adiectivum, WORDS kind N + substantivum,
capitalized unknown → nomen-proprium (source `regula`). Coverage
CIRCSE 84 → 93.7 %, LLCT 72 → 88.9 / 88.2 %; primary ≈ 68 % (stage 5's
job). Gate `probatio_oratio_oraculum` (104). **T14 DONE**: the
instruments and the Python face of stage 3. `./oratio/verba.sh
<x.txt>... [-machina] [-analyses]` (one row per word: via, index, byte
extent, line, paragraph/sentence ordinals, form, classes, linguae,
first lemma, count; `-analyses` = one row per analysis with the
accidents as option TITLES read from the tree by slot title);
`silva.Oratio(via | text)` (`sententiae()`, `vocabula(classis=,
lingua=, ignota=, sententia=)` — a class filter matches ANY candidate
—, `ignota()`, `analyses(vocabulum)` with `accidentia` dict) and
`Prosa.sententia(n, intra=)` (markdown paragraphs → one parse →
extents mapped back to file bytes; an edit anchor). Gate = the
pythonica suite (20 assertions; planted fault red). **T16 DONE —
STAGE 4 COMPLETE**: `OratioVocabularia {la, en}` + one loader
(`oratio_vocabularia_onerare`: la.bin, glossary, Moby from the root;
instruments and gates share it); the annotator consults BOTH
dictionaries, English readings after Latin (`oratio_partes_en`: exact
record → one description per code-letter class in Moby's order,
rules → accidents from the rule title, possessive/contraction parts,
`nativum` = codes + rule); pass II data lists (auxiliaria,
subordinantes, particulae, numeralia, interiectiones) once per word,
source `regula`; the capital rule widened to words no LATIN source
knows (Moby knows Hercules); a sign element carries interpunctio AND
symbolum in the oracle. EWT dev/test vendored (CC BY-SA): 77 → 91.3 /
91.8 %, primary 57 %; Latin pins rose to 940 / 895 / 887. Gates partes
275, oraculum 115. **T17 DONE — STAGE 5 OPENED** (decisions 28–35):
the projection stamps ORDINALS (`n=` on sententia/elements/analyses)
and `lingua=` (census) through the node hook; `oratio/partes/
resolutio.stml` = `<regula titulus=>` elements, each an EXEMPLAR over
the sentence projection + a `<consilium>` of PER-made `<praelatio
vocabulum= analysis= regula=/>` rows (canon declares them: the plan is
judged); `oratio_resolutio_applicare` composes sentence + program, one
legere + expandere, reads the rows in program order, FIRST WINS per
word, permutes the analyses list (materia verbs `lista_permutare` +
`reponere`, substrate change #3, contracts checked) and rebuilds the
classes/linguae summaries. Two literal rules (adposition governs
accusative / ablative; a captured case never retries, a sentence root
matches one greedy pair — findings for the pattern spec's doors).
Primary +0.5 CIRCSE; pins 678/679/679/566/569 permille only rising;
`oraculum.sh -regulae` = cumulative table; `-crudus` everywhere;
`silva.Oratio(crudus=)`. Gates resolutio 87, oraculum 127, materia
nodus 92. **T18 DONE — the language rule** (2026-09-07): rule
`lingua-documenti-anglica` FIRST in `resolutio.stml` — a two-stage
chain (floating `<sententia lingua="anglica"/>` narrows, a floating
`<vocabulum>` stage under `de=` enumerates its words, `<* n="$a">`
with a `<lingua>1</lingua>` child = the first English reading);
no executor change. Exposed and fixed on the way: Moby's case
variants precede the lowercase record (`In\N` before `in\PvAN`) —
LEX CASUS in the English analyser (lowercase token → lowercase
record; Capitalized → lowercase then exact; else exact first);
the census counted analyses, now WORD VOTES
(`OratioPartesCensus.vocabula_linguarum`); the oracle censuses the
whole treebank as one document (instruments already did) and
prints `sententiae censae` + `lingua documenti`. EWT primary
56.6/56.9 → 67.8/68.0 (pins 677/679), Latin unchanged. **T19a
DONE** (2026-09-07): `./oratio/oraculum.sh -discrepantiae` lists per
gold class the forms COVERED but not FIRST with our first class and
a count (`-machina` rows `DISCREPANTIA`); law gated: per class the
tally sums to tecta − primaria. Rules are written FROM this listing.
**T19b DONE** (same day): eleven CLASS-PRIOR rules in
`resolutio.stml` placed BEFORE the language rule (first wins per
word), each language-gated by the T18 two-stage chain (`<sententia
lingua=>` then the reading's `<lingua>` index): auxiliare, particula,
adpositio, subordinans (both languages), interiectio and numerale
(English); pass II readings selected by their list TITLE in
`<nativum>` (`subordinantes`, `interiectiones`), the ambiguous
members moved to `*-ambigua` lists whose reading stays (coverage) but
carries another title (so/when/where, no; Latin `ORATIO_SUBORDINANTES_
CERTAE` gives the title only to sicut/qualiter/quam/nisi/licet…);
two lemma EXCEPTION rules placed before their class rule (`supra`
adverb in the charters, 209 words; article `a`, which Moby also codes
as a preposition, 492 words). Glossary pass III = lemma priors (iam,
etiam, vel, nec, neque, tantum, quantum, deus, casa, pars, sanctus
paradigm, four saints; English I, us, just, even, only, still,
however, go, said, no). Removed from the Latin particle list: nec,
neque, vero (conjunction/adverb in gold). Primary: CIRCSE 67.9 →
72.8, LLCT 68.0/67.9 → 77.0/77.1, EWT 67.8/68.0 → 75.3/75.5 (pins
728/770/771/753/755); coverage unchanged. LAWS LEARNED: forma
children match as an ORDERED SUBSEQUENCE of the analysis children
(lemma, lingua, fons, nativum, sensus, accidents) — `<lemma>` before
`<lingua>` or the pattern silently never matches; derived-token text
IS compared; never prefer a Moby class blindly (`!` and `P` sit on
good/thanks/there/a). Ranges (`-que`, `'s`) can never be primary:
≈ 2 % ceiling on Seneca. NEXT: **T19c** the sibling-run door
(`cursus="fratrum"` on EXEMPLAR — one match per start position among
a parent's children) in `lib/stml_macros.c`, gated in the exemplaria
suite; then **T19d** agreement rules (adjective/determiner ↔ noun by
case, number, gender — Latin determinans is 0 % primary on 2,096 LLCT
words), `to` + verb, `have` + participle, non-initial capitalized
word → nomen-proprium. **T19c DONE** (2026-09-07): the sibling-run
door is built (`cursus="fratrum"` on EXEMPLAR, increment 20 of the
pattern spec, commit b29ce72f: one row per start position among a
parent's children, first written child fixed to the start, gaps
after) and the two adposition rules are re-rooted on it at
`<elementa>`: every preposition now governs its own noun (`Cum puella
et cum femina` → both ablative first; before, only the first pair).
MEASURED: the treebank primaries did not move (728/770/771/753/755),
so the one-pair ceiling was almost never binding for these two rules;
the door's value is the rules that need a neighbour (T19d). Fran's
direction (2026-09-07, desideratum 01M1YYN00E): agreement goes in as
UMBRA nodes, not as hand rules — a reading carries the dependent it
expects (relation, conditions from the dictionary's own codes, an
empty binding); a generic rule per relation fills it from siblings;
a bound umbra is a dependency edge, an unfilled one a finding. First
umbra = the adposition's object (the existing rule as the comparison).
**T19d SKELETON DONE** (2026-09-07): registry 24 genera / 170 slots
(seal `54a5e44f`): every analysis genus gained a last slot `umbrae`
(LISTA_NODUS) and a genus `umbra` (relatio, classis, casus, numerus,
genus, impletio-vocabulum, impletio-analysis — all INDEX; unwritten =
unconstrained / unfilled) is appended AFTER the analyses, so
`ORATIO_GENUS_ANALYSIS_ULTIMUM` bounds the class mapping. The
annotator (`_umbras_ponere`) gives every adposition reading with a
case an object umbra in that case, from the dictionary's own code.
The program's two adposition rules became two generic umbra rules
(`umbra-obiectum-accusativi` / `-ablativi`: any reading carrying an
object umbra in that case, filled by a following noun in that case —
the constraint sits in the tree, the rule names only relation and
case) whose rows are `<impletio vocabulum= analysis= umbra=
ad-vocabulum= ad-analysis=/>`; the executor validates, prefers both
readings under first-wins, permutes, then BINDS the umbra (`ponere`
on the impletio slots, the filler's index remapped through the
preferred-first law; a second fill is a repetita). Canon declares
umbrae/umbra/impletio; drift guard knows impletio. Gates: registrum
(998), canon, partes I b (umbrae born on `in`'s readings only, empty
impletio, projection `<umbra n="0">`), resolutio (`Cum puella`: the
adposition reading's umbra bound to word 1 reading 0; composed plan
carries `<impletio>` and no `<praelatio>`), oraculum pins 729/771/772
(+1 permille on each Latin treebank over the T17 rules; English
unchanged). **T19d HEAD UMBRA + STAGES DONE** (same day): every
adjective and determiner reading with a case carries a CAPUT umbra
(relatio 1) with the reading's own case, number, gender; fourteen
generated rules `umbra-caput-<casus>-{sequente,praecedente}`
(`gradus="2"`, `cursus="fratrum"` at `<elementa>`, case literal,
number and gender captured at the umbra, filler = any reading of
the neighbour with those accidents) emit `<impletio>` rows. The
executor gained STAGES: `<regula gradus=>` (default 1, canon), the
program runs stage by stage, each stage against a FRESH projection
(stage 2 sees stage 1's reorderings), the vindicated set persists,
and a filler row on a vindicated word whose filler reading is not
that word's first is repetita — both forced by `Cum puella bona
ambulat` (one stage: the nominative head rule fired before the
ablative and bound `bona` to `puella`'s nominative). Probes: `Puella
bona ambulat` (bona adjective nominative, crude first a noun), `Cum
puella bona ambulat` (both ablative), `Hoc templum est` (determiner +
nominative, crude first genitive plural), `Hoc est` (pronoun).
CIRCSE primary 72.9 → 76.5 % (adjective 78.6 %, determiner 19.9 %), LLCT 77.1 / 77.2 → 78.8 / 78.6 % (determiner 0 → 18.7 / 18.1 % on 2,096 / 1,914 words), EWT unchanged 75.3 / 75.5 (English readings carry no case, so no head umbra is born); coverage unchanged; the vocative pair adds nothing on Seneca and one permille on the charters; pins 765 / 787 / 785 / 753 / 754. **T19d GAMMA — THE LAW OF UMBRAE** (same day): after the last stage
the executor puts a word's readings whose umbrae are all filled by
a NEIGHBOUR first (rest in order), leaves words decided by a
`<praelatio>` row alone, and remaps bindings into permuted words
(`_umbris_ordinare`; census `umbris_ordinata`). Measured before
kept: three-rank with demotion and without regard for explicit
decisions lowered every treebank (chance bindings overrode the
language rule and `supra`; unfilled prepositions sank when the
object is a proper name without case); promotion at any distance
cost Seneca 0.5 (the `praecedente` rule binds the FIRST agreeing
earlier word, not the nearest). Neighbour-only promotion depresses
nothing: Seneca 76.5, charters 79.2 / 78.9, English untouched; pins
765 / 791 / 789 / 753 / 754; Hilarius 532 bound umbrae, 0 discordant.
Gate resolutio VII (structural promotion `in bona terra`, explicit
decisions kept for `supra` and English `a` + `die`, distant filler
ignored, accusative remap on `hoc templum`, the corpus invariant).
Instruments: `verba.sh -analyses` column `umbrae`
(`relatio:casus.numerus.genus=word.reading`, `=?` unfilled;
`silva.Oratio.analyses()[i].umbrae`), `oraculum.sh -regulae -ab N`
(table from rule N). FINDINGS: "no object" evidence needs
unknown-case fillers first; "nearest" needs the `strictus` door;
two filled readings still tie by rule order. **T19e — ENGLISH BY EVIDENCE** (same evening): the pattern
engine's `cursus="strictus"` built (commit 0c7f133f; adjacency),
then: `nomen-proprium-capitale-anglicum` (a capitalized word after
any element, by the capital rule's reading; contractions excluded
from the capital rule via their pronoun reading; EWT proper nouns
19 → 64 %); class umbrae on English readings — `to`'s particle and
have/do's auxiliary readings (moved to `*-ambigua` lists with their
own title) carry an object umbra `classis` verbum filled strictly by
the next word's verb (auxiliary filler first: `have been`), no fill
→ the language rule's Moby-first (`to` adposition 89 → 99 %, have
verb); a contraction prior by `fons` for `'s 'm 'll`; all evidence
rules BEFORE the class priors (first wins: a prior had decided `to`
before its umbra could fill). Executor: within a stage an
EXPLICITLY decided filler word is not re-read (`have a car`: Moby
codes `a` as a verb); structural same-stage decisions do not block
(measured). Dropped after measuring: determiner head umbra (Moby's
noun readings of `is`/`it's`), distant-verb variant for have/do.
EWT 75.3 / 75.5 → 78.6 / 79.0 (pins 786 / 789), Latin and coverage
unchanged; 33 rules. **T19f — FILLERS OF UNKNOWN CASE** (same
night): three rules after the exact ones fill a Latin reading's
object or head umbra of any case with the neighbouring capital-rule
proper noun (no case): Seneca 76.8, charters 79.6 / 79.1 (pins 768 /
795 / 790), English untouched; 36 rules; the corpus invariant accepts
an unwritten filler accident. Demotion RE-MEASURED three ways on top
and rejected again (charters −0.3 to −0.7): unknown words have no
reading node to bind, so "no object" still mostly means "object
unreadable". NEXT: an `analysis-ignoti` reading for unknown words
(then demotion once more), a charter glossary / spelling
normalisation, Latin determiners from the listing, sentence-initial
proper nouns, `there is`, verb slots. **T19g — THE DECISION IN THE TREE** (2026-09-08, decretum SUDOKU
decision 40): `vocabulum` gained `decisio` (praelatio | impletio |
umbra; unwritten = nobody decided) and `auctor` (the deciding rule's
title), written by the executor after the last stage (the law only
when it changed the first reading), mirrored as `decisio=` on the
projection; `verba.sh` columns, `Oratio.partitio()`, the oracle's
PARTITIO and AUCTOR tables with FORCED accuracy pinned (721 / 692 /
688 / 913 / 918). FIRST FINDING: forced decisions are the least
accurate Latin tier (69–72 %) against priors 85–98 % and untouched
order 80–92 %; the law itself 51–57 %; the Latin adposition prior
99 % on the charters and it speaks BEFORE the object rule; the
dative head rule 37–41 %; the English contraction prior 0 % (ranges).
**T19g bis** (same day): `oraculum.sh -errata [-auctor R]` lists
the wrong decisions per rule with the binding partner and its
distance; the rule table splits accuracy adjacent | distant.
FINDINGS: head rules are reliable adjacent and unreliable distant
(charters genitive 99 vs 33, accusative 84 vs 52; Seneca nominative
82 vs 70 with three quarters distant) — the row's preference was
never restricted to neighbours, only the law's promotion; the
charters' nominative and dative head errors are UD's DET
convention (possessives, alius, omnis, ipse, suprascriptus, qui)
losing the tie to the adjective reading. NEXT: strict head tier
(measure), determinans-over-adjective law for the UD-DET lemmas
(data), the Latin rule order measured swapped, the ignoti reading. **T19h** (same day): the strict head
TIER — sixteen `-proximo` copies of the head rules at stage 2
(`cursus="strictus"`), the loose originals at stage 3; measured:
strict alone loses everywhere (undecided words fall to a worse
default), the tier gains a little everywhere (76.9 / 79.7 / 79.3;
forced 72.3 / 69.4 / 69.4; pins 769 / 796 / 792, 723 / 694 / 694); 52
rules. **T19i** (same day): DETERMINERS BY DATA — the list split into
certain lemmas (DET without exception in all three treebanks; title
`determinantia`, form exclusions for the adverbial `tantum`/`hic`
and the name `toto`) and ambiguous ones (is, qui, quis, aliquis;
`unus` splits by treebank), prior `determinans-primum-latinum`
before the object rules; and the LAW OF THE EXPLICIT CLASS in the
executor: a prior fixes the class, agreement may still choose the
reading (or its twin) of that class — carriers via head umbrae
only, fillers via head umbrae or a settled-carrier object umbra,
same lemma and language, once per stage. Seneca 80.0, charters
83.9 / 83.2, English untouched; pins 799 / 838 / 831, forced 761 /
762 / 758; 53 rules. **T19j** (same day) measured the tail four
ways and kept none: strong-class heads (Seneca +0.2, charters −0.1 /
−0.3), weak heads binding without preferring (same), strong-twin
substitution (−0.2 / −0.1), loose-stage noun-first carriers not
preferred (Seneca −0.4): a distant adjective binding at 70 % still
beats the crude order for those words, and the charters' heads are
often unknown nouns. **T19k** (same day): THE IGNOTI READING — an
unknown word carries one `analysis-ignoti` reading (lemma = folded
form, nativum `ignotum`, fons regula, NO language: `lingua_ignota`),
`classes` still `ignotum`; rule 21 `umbra-obiectum-ignoti` binds a
Latin object umbra to it (unknown HEAD rules measured 33–57 % and
dropped); a decision on a single-reading word is vacuous and no
longer recorded (forced tier honestly 71 % on the charters, pins
moved with the cause); primary unchanged; 54 rules. **T19l** (same
day): DEMOTION MEASURED A FOURTH TIME on the complete grid and
rejected: prior off −0.1 / −0.3 / −0.2; any-class object fillers
+0.1 charters / −0.1 Seneca; objectless explicit adposition
demoted = no change (the alternative readings are junk: `A` as a
name, `de` as a noun, `super` as a verb; `cum` the conjunction is
two words). NEXT (Fran, 2026-09-08): THE CLAUSE — a `clausula`
INDEX slot stamped on elements (the sudoku "box"),
seeded from certain subordinators, relatives, punctuation and
coordinators between finite verbs, one clause where nothing decides;
laws over it: one finite verb per clause, subject inside, `cum` +
finite verb = conjunction, loose head rules inside the clause only;
oracle = the gold verb subtrees already vendored. **DESIGNED
2026-09-08** (spec §2 decisions 43–47, §7 "Design — T20", decretum
01M20W7HH3): the clause is a NODE on the sentence (`clausulae` list;
genus `clausula` after `umbra`: semen, species, pater, umbrae
verbum-finitum + subiectum born from the schema), elements stay flat
with `clausula` + `clausula-causa` INDEX (semen | extentum | clausura |
catena | unica; unwritten = OPEN); stamp module `oratio_clausula`
(`seminare` before resolution, `propagare` between stages), seeds are
DATA measured through `-semina` first; oracle = purity per layer +
coverage + count against gold finite-root subtrees, pinned only
rising; NO phrase structure (decision 46: a phrase is the component
of bindings). **T20a DONE (2026-09-08)**: genus `clausula` (semen,
species, pater, umbrae verbum-finitum by `forma-verbi`=finitum +
subiectum nominativus) in the sentence's `clausulae` list, elements
carry `clausula` + `clausula-causa` (registry 184 slots, seal
62a7afd8); module `oratio_clausula` (`oratio_clausulas_seminare`,
called by the resolver per sentence): seeds are DATA measured first
(`oraculum.sh -semina`; C census == Python census) — certain si quia
qualiter dum quod sicut donec quamquam quando..., corroborated ut cum
nisi ubi unde (a finite-verb-capable word must follow), relatives by
lemma qui except the forms quis/quid, coordinators et atque ac sed aut
nec neque at only between finite-verb-capable words, `-que` never;
comma/semicolon/colon CLOSE a subordinate (never seed), parentheses
open a parenthetical; closure also at the first certain finite verb;
main clause born lazily; sentence without seeds = `unica`. Oracle:
gold clause = finite-root subtree; PURITY per layer, coverage, count
agreement (`-clausulae`, `-errata` listing with layer/species/gold
root); pins puritas 738/829/864/726/739, pares 518/740/771/638/659
(birth: semen 89/96/95, extentum 76/86/89, clausura 49/65/66 = the
weak layer as predicted, 29 % of charter words; unica 74/98/99);
RESIDUAL = a second finite verb with no seed (asyndeton, Seneca verse,
charter formulae) → T20c law 1. Python `Oratio.clausulae()`,
`vocabulum.clausula/clausula_causa`; `verba.sh -clausulae`; gate
`probatio_oratio_clausula` (161, planted fault si). Primary/forced
unchanged. **T20a bis (same day): the VERB SPLIT (layer V,
reason `verbum`)** — a certain finite verb in a non-closable clause
that already holds one opens a coordinate clause; the words between
the two verbs go to the NEW clause (variant A, verb-final; B measured
and worse on every Latin file); overall purity 738/829/864 →
822/848/881 (pins), Seneca count agreement 52 → 65 %. NEXT = T20b
(catena along bindings, discordia count, override decision from the
numbers), then T20c (clause carrier in the executor, cum conjunction,
`cursus="clausulae"`, subject umbra). Closure layer (`est` early in
charter subordinates) is the next measured target. Head-rule template
still open.

**Index defect and lint hardening (2026-09-07).** The pythonica
gate's red `usus: sedes definitionis` was a briar fixture's truncated
`latina.h` shadowing `include/latina.h` in the nexus sweep (first-wins
by basename in strcmp order, 'b' < 'i'; question 01M1TD1FMFT3, fixed in
`nexus_ordines_capitis_gradus`, gate `probatio_officina_capita`). The
identifier pin had been measured on the degraded table. Since then:
the pin is a committed SET (`fixa/vocabula/ignota_symbolorum.txt`,
2,949 words, only shrinking, a new word red with its first site,
regeneration `ORATIO_VOCABULA_SCRIBERE=1` with a cause); the corpus is
git-tracked `.c/.h` only (`oratio/build/corpus_tractatae.txt`, another
session's uncommitted work never counts); the total word count (7,703)
is pinned only rising so a shrinking index goes red where its number
is read; the runner renovates the nexus index before the gates, so new
words are seen in the same run; glossary section TERMINI GRAMMATICI
(eight accident titles WORDS lacks). The failure CARRIES ITS REMEDY:
`oratio_vocabula_nova` (shared by the gate's block IV b and
`./oratio/vocabula.sh -nova`, exit 1 when new words exist) prints each
new word with its first whole identifier, site count and first site,
the three exits with the decision rule, and ready-to-paste glossary
skeletons; the assertion message itself lists the words, so the commit
helper's summary shows them. The pre-commit AND pre-merge-commit hooks run
this lint and BLOCK (Fran, 2026-09-07; `tools/unci-git/lint_latinus.sh`,
birth gate stages IX–XI; bypass `--no-verify` with the cause).

## Laws to keep (spec §2–3)

- **The SUDOKU model (Fran 2026-09-08, spec §2 decisions 36–42).** A
  constraint solver over candidate lists, never a guess: eliminate
  only over a COMPLETE grid (an unknown word is a cell with all
  candidates, not a missing cell); hard constraints eliminate, soft
  ones (priors) only order and never block; at a plateau add a
  PERPENDICULAR dimension (clause counting, document consistency,
  natura relations as umbrae, zoom level); report a forced | ordered |
  open PROFILE; residual = unsolved | unsolvable; a surviving soft
  violation is a FIGURE, not an error. Decretum 01M1ZNHQ3F.
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
  is the Latin context's and is a committed SET of tolerated unknown
  words (`fixa/vocabula/ignota_symbolorum.txt`) that only shrinks: a
  new house coinage in an API name is red with its first site until
  it has a glossary entry, an `ignotum-permissum` line, or a rename;
  regeneration (`ORATIO_VOCABULA_SCRIBERE=1`) needs a named cause;
  the total identifier word count is pinned only rising as the
  instrument's own health check; only git-tracked `.c/.h` count.
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
- The oracle never feeds the code: coverage pins move only with a
  named mapping change; secondary readings are APPENDED (primary
  untouched, ordering is stage 5); nothing derived from a treebank is
  shipped, NC treebanks live in build/ only.
- Enumeration titles = the glossary canon's options, in order (gate);
  the parser never annotates — `oratio_partes_annotare` is a separate
  pass with the dictionary supplied by the caller; accidents are placed
  by slot TITLE, never by a hand index table; the `classes`/`linguae`
  ATTRIBUTES are mirrors of the slots (reader ignores them); `casus`
  and `nomen` are latina macros — fields are `casus_grammaticus`, a
  parameter is never `nomen`.
