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
822/848/881 (pins), Seneca count agreement 52 → 65 %. **T20a ter
(same day): the CLOSURE variants**, each measured alone by PAIRWISE
CONCORDANCE (Rand over word pairs; purity alone rewards
over-segmentation, count alone sees no order — concordance is now the
pinned decision floor, count agreement reported): `qui supra` (the
charters' "aforesaid") no relative seed, a COPULA (lemma `sum`) does
not close a clause without its verb, a non-seeding coordinator right
before a seed joins the seeded clause, CERTAINTY counts LATIN readings
only (`tenet` is a Moby noun); initial `unde/ubi` no-seed measured and
dropped (−0.2 alone, nothing combined); the stamper now takes the
DOCUMENT LANGUAGE from the resolver and an English document is not
seeded by Latin readings at all (`it sit do` are finite Latin verbs).
Concordance 799/769/781 → 819/811/798 (pins), purity 850/870/893
(pins), EWT purity 724/737 (down 2 with the cause named). NEXT = T20b
(catena along bindings, discordia count), then T20c (clause carrier
in the executor, cum conjunction, `cursus="clausulae"`, subject
umbra). **T20a quater (same day)**: the SENTENCE VIEW is in the
instrument (`oraculum.sh -clausulae -sententiae N [-causa T]`: text +
per word our clause, layer, gold root, `!` on the wrong ones); read on
the charters' `verbum` layer it gave four shapes, two kept by
concordance — the fixed phrase `id est` (its `est` neither splits nor
closes) and a proper NOUN after the closing verb stays in the clause
(`ubi vocitatur Iuveiano`) — and two measured and dropped (a clause
without its verb not closing at a comma: +0.8 test / −0.6 dev in
combination; the coordinator scan through commas: −0.9 test).
Concordance pins 820/816/802. **T20a quinquies (same day)**: the
`extentum` view on the charters gave three more: `una cum` never seeds
(112/112 gold prepositions), `cum` is corroborated by a certain or
SUBJUNCTIVE verb when the next word can be ablative (else any capable
verb, as before — the certain-only variant cost Seneca 39 pairs and
was dropped), forms of `quantus` seed a relative clause (`quantum ...
est pertenentes`); concordance pins 820/826/809; PURITY now reported
not pinned (fell 3 while concordance rose 7: one-sided like count).
Clauses per sentence 2.51 / 2.38 vs 2.52 / 2.25. **T20a sexies
(Fran)**: concordance pins are EXACT concordant-pair counts per
treebank (102551 / 438519 / 444139 / 104973 / 107344), not truncated
permille — no boundary luck; the measurement loop reads the machine
rows (`CLAUSULAE` columns: paria, concordia) and the human line prints
the gate's permille with the counts. **T20b DONE (same day): the
CHAIN measured and left as a COUNTER** — `oratio_clausulas_propagare`
(resolver calls it after all stages): a binding whose ends sit in
different clauses is a DISCORDIA (547 / 1016 / 956 on the Latin files);
following it was measured four ways (adjacent / any distance × weak
layers / any) — adjacent moves ~nothing and loses on Seneca, distant
loses a full percent everywhere: the span is right more often than
the distant binding, so the discordia is evidence AGAINST the binding
(= T20c law 3, loose heads inside the box). `catena` places nothing;
override switch at 0 with the numbers. **T20c I (same day): LOOSE
HEADS INSIDE THE BOX, measured seven ways and REFUSED** — executor
predicate `_trans_clausulas` (an impletio row whose ends sit in
different clauses is refused; switch `RECUSATIO_TRANS_CLAUSULAS`,
variants I–VI with their numbers in the code) and the pattern form
`clausula="$c"` on both words of the 16 loose head rules (hard, and
soft = in-clause copies at stage 3 with the unrestricted rules as a
stage-4 fallback). Primary/forced base 799/765, 838/710, 831/711;
pattern hard 797/782, 841/723, 834/729 (forced +17/+13/+18, Seneca
−26 words); soft LOSES everywhere (refusal is the only lever, the
nearest candidate is already in the box); the CONTROL without the box
(refuse across a certain seed word, stamps ignored) 797/772, 841/726,
834/731 = the box. Every variant costs Seneca primary → none kept
(rule: no treebank falls). Lesson: for boundary laws the box IS the
seeds (closure is the weakest layer); any clause law must beat the
seed-between control; the box has to prove itself on MEMBERSHIP laws.
Gate: `recusatae_clausulis == 0`. NEXT = T20c law 1 (verb umbra
hidden single: clauses as carriers), then `cum` conjunction, subject
umbra. Head-rule template still open. **T21 (same day): the ENCLITIC
SPLIT** — `armaque` is two elements now: `oratio_partes_encliticum_scindere`
(inside `oratio_partes_annotare`, before the host is annotated) splits
a one-part word whose FIRST dictionary analysis is a TACKON and which
Moby does not know whole; `oratio_lexema_pars` slices the token with
exact positions (emission == source), the tail spaces migrate to the
enclitic, the enclitic gets its one reading (que/ve coordinator, ne
particle), the element list is rebuilt via `materia_nodus_reponere`.
Whole-word entries (`quoque neque itaque`) never split. Oracle: a
range component whose form equals one of our elements is judged
against it. Seneca primary 799 → 834 (ranges 302 → 31), charters
840/833, EWT unchanged; Seneca forced 765 → 764 with the cause (the
enclitic element breaks strict adjacency with the NEXT word: five
words) → T21b = strict cursor transparent to enclitics (pattern
machine). `Lucane` (= Lucanae) was a DICTIONARY hole → `lucanus` in
the glossary; a proper-noun guard did nothing (luca = elephant).
Census field `enclitica`; Hilarius 1678 → 1682 words (pythonica). **T20c law 1 REFUTED BY CENSUS (same day, scratch
`verbum_unicum.py`)**: reach 68/25/24 finite verbs; sentence single
+3/−8, +1/−2, 0/−6; clause single −49/−53/−74 net — the clause holds
no finite verb or a certain one already. NOT BUILT. Verb errors are
PARTICIPLES read as adjective/noun (Seneca 152/253; participle-first
prior censused and REFUSED: +154/−269, charters +161/−984 — `sancti
casa censum suprascripta`) and, on the charters, UNKNOWN MEDIEVAL
FORMS (409/597): medieval orthography correspondences recover
1869/3145 unknown occurrences (1697 with the gold class) → next
dictionary tranche (`orthographia.py`). **T22 DONE (same day): MEDIEVAL
ORTHOGRAPHY = contract IV of the Latin vocabularium**
(`oratio_vocabularium_la_quaerere_variantes`, data table
`ORATIO_ORTHOGRAPHIA` with `activa` switches and measured numbers;
source value `orthographia`; annotator asks only for forms unknown to
both dictionaries, capitals excluded). Kept after the loop: h-prefix,
e→ae, inp→imp, b→p, e→i (last: alone it breaks the charter forced
floors, behind the others it holds them). Refused: doubling, d→t
(EWT forced), o→u, i→e, p→b (charter forced), inb→imb (nothing).
Charters 840/833 → 863/856 primary, unknowns 1622 → 1009; Seneca 835
after `mecum`/`temet` glossary entries (lemma = form: a `lemma="ego"`
entry shadows `ego` itself); EWT flat. Env `ORATIO_ORTHOGRAPHIA_SOLA`
/ `_SINE` for the loop. NEXT candidates: T8b pronoun tackons (mecum =
me + cum in gold), a charter-vocabulary glossary for the 1009
residual unknowns, T21b strict cursor, clause laws 2/4 (census
first). **T23 (same day): the CASE MEASURE** — oracle `casus`:
gold words with `Case=`, class right, first reading Latin with a case
slot; right = same case (conditional on class: case disambiguation
alone). Global, per class, per author; human tables + machine columns
appended to `SUMMA`/class/`AUCTOR` rows; pins 609/565/567 (Latin
files), EWT reported. FINDING: Seneca class 83.5 % but CASE 60.9 %
(nouns 58.6, adjectives 57.2); the nominative head rules pick the
right case 52 % / 45 % of the time. THE LATIN PROBLEM IS CASE. Law 4
is measured here. **Law 2 (`cum`) REFUTED BY CENSUS (same day,
`cum_census.py`)**: seed decision vs gold — Seneca 17 right / 7
wrong seeds (law +17/−7), charters 12 and 29 false seeds among 230
prepositions (law −11/−27); control (verb follows) −103/−116 on the
charters. Seed refinement `CUM_ABLATIVO_SOLO` (ablative-only next
word → preposition) cuts false seeds to 3/15 but loses 312 dev
concordance pairs (+157/+1021 elsewhere) → refused, switch at off
with numbers. **Law 4 (subject) REFUTED BY CENSUS (same day, `subiectum_census.py`)**:
reach 165/44/54; sentence single +24/−11, −1, −3; clause single
+65/−64, −55, −56 (the agreeing candidate is the object as often).
ALL FOUR CLAUSE LAWS CENSUSED AND REFUSED — the clause stays an
instrument. CASE CONFUSION MATRIX (`casus_confusio.py`): acc→nom 574,
abl→dat 367, abl→loc 134, gen→nom 160 on Seneca; charters gen→voc
197; 799 Seneca errors are open-tier DICTIONARY ORDER; 1297 charter
errors have no reading of the gold case. CASE PRIOR CENSUS
(`casus_prior.py`): per-set majority 73.4/73.8/76.4 %, global order
abl>acc>gen>nom>dat>voc>loc 65.3/71.7/72.9 % vs today 61.1/56.7/56.9;
sets agree except abl/acc/nom/voc, abl/nom/voc, abl/acc. NEXT = T24
CASE PRIOR (orders readings within a class at resolution start; global
order + per-set entries measured alone on all five; pins casus AND
primary/forced must hold since bindings take the first agreeing pair). **T24 DONE (same day): CASE PRIOR** — `_prior_casuum` runs
LAST in `_sententiam_resolvere`, on words nothing touched (not
decided, no filled umbra, not a binding target), reordering the FIRST
class's readings in their own positions by `ORDO_CASUUM`
(abl>acc>gen>nom>dat>voc>loc) + `PRIOR_COPIAE` entries (kept:
abl/acc/nom/voc → nom; refused: abl/nom/voc → nom, abl/acc → acc).
Casus 609/565/567 → 645/622/637; primary/forced IDENTICAL. BEFORE the
rules it wrecked primary (835 → 808, 863/856 → 822/811: spurious
ablative agreement) — a prior may not change what evidence rules see.
Gotchas: unsigned index initialised to −1 (examen "comparatio vana"),
class consolidation (`Cum`). Older resolutio hand cases now expect
the prior's ablative where no rule fired. NEXT candidates: head
rules' case choice (52/45 %), T8b pronoun tackons, charter glossary,
T21b. **TREEBANK SHELF (2026-09-08)**: `./oratio/oraculum.sh -petere`
fetches the four non-commercial Latin treebanks (ITTB, PROIEL,
Perseus, UDante; CC BY-NC-SA) into git-ignored `oratio/build/ud/`,
REPORTED never pinned or committed. Unseen by any rule they score
primary 862/824/790/780, case 722/707/723/676, coverage 949/914/934/859
— same band as the tuned files: no overfitting; expected other-Latin
class 78–86, case ~70. **T25 PAIR POLICY (same day): NULL RESULT on
readings** — `_ordines_ordinare` reorders a rule's rows per (carrier,
umbra): nearest partner, then case rank (`POLITICA_PARIUM`, on at II);
works on hand cases (`magna` → `puella` not `Terra`), but primary /
forced / case IDENTICAL on nine treebanks: both candidates agree with
the same reading, so only ATTACHMENT changes, which the oracle does
not measure. Kept on as the attachment default. NEXT = attachment
column in the oracle (gold heads), then the remote head rules judged
on it. **T26 ATTACHMENT MEASURE DONE (same day)**: oracle
`LIGATIO` — every aligned element remembers its gold token;
`_ligationes_iudicare` judges the first reading's filled umbrae
against gold heads (head umbra: partner == carrier's head; object
umbra of adposition/particle/auxiliary: partner == carrier's head, UD
case/mark/aux direction; verb object: carrier == partner's head).
PRECISION pinned 431/364/377 (Latin), 815/826 (EWT); RECALL reported
95/95/98 permille of gold arcs (EWT 19/22). Adjacent 586/730, head
umbrae 408/363. FINDING: we draw ~1/5 of the gold arcs and get < 1/2
right — the measured distance to the dependency design. Glossary
`ligatio`. Named: attachment per rule (umbra carries no author).
**T27 RETRY DOOR MEASURED (same day)**: the STML matcher's joint
backtracking now exists as `quaesitio="completa"` per EXEMPLAR
(exemplaria decree 21); run as the DEFAULT under the literal rules it
kept primary/forced but cost case 645/622/637 → 641/619/635 and
charter attachment 364/377 → 359/374 — the nominative rules claimed
rarer nominative readings before the accusative rules saw their first
reading; FIRST READING PER CASE IS A PRIOR. Default stays greedy, no
rule changed, pins identical. Number/gender were captures already;
case is the only literal dimension (28 head rules → 4 possible).
**T27 b (same day, Fran's decision): STRICT HEAD TIER UNIFIED** — 14
literal strict rules → 2 (`$cas` on both nodes, `quaesitio="omnes"` =
every agreeing pair is a row, T25 pair policy picks the nearest; 54 →
42 rules); pins case 662/644/665, attachment 437/368/378, forced
765/711/712 (charters −1 NAMED: fallback tier grows, refusing those
rows is worse); shelf case +21/+3/+9/+10. LOOSE tier stays literal
(unified it costs charter attachment −17, Aquinas case −19). WITHDRAWN:
"first reading per case is a prior" — nominative-first rule order plus
retry was the harm; dictionary order with every completion wins. Pair
policy case rank (level II) is REAL under these rules (level I loses
7/15/10 case); known miss `Puella bona ambulat` → ablative (hand case
moved to `Puerum bonum videt`). Refused: object unification, strict
`completa`, own-reading refusal. 2026-09-09: `_ligationes_remittere`
(a permuting stage remaps every umbra pointing at the word; stale
indices were two discordant bindings on Hilarius; no number moves);
`EMENDATIO_SEMEL_PER_SENTENTIAM` ON (Fran 2026-09-09: attachment
441/386/394, case 660/654/671, Seneca case pin 662 → 660 NAMED); pair order
`ORDO_CASUUM_PARIUM` (nominative-first loses 6–19 case: ablative
first stands; `Hoc templum est` reads accusative, recorded). **T28 (2026-09-09,
Fran's decision): LEX I CLAUSULAE ADOPTED AT LEVEL II** — re-judged on
the attachment column T20c lacked: attachment 462/398/412 (EWT dev
817, shelf +16..+44), forced 769/713/717, case 666/653/671 (charters
dev −1 NAMED); primary and recall unchanged. Gold census: nearest
agreeing nominal IS the head 96–99 % given true agreement — the
readings are the problem, not the law; a verb intervenes in 30 % of
Seneca's adjective arcs (no clause bound in the MATCH). Unifying the
loose tier loses attachment under every ordering (four tried, policy
attribute reverted): loose tier stays literal → §8.5 template. **T29 (2026-09-09): FEATURE
COLUMNS** — oracle `NOTAE_ORACULI` table (Number, Gender, Person,
Mood, Voice, VerbForm, Tense = Tense+Aspect) → `NOTA` lines/rows;
pins number 914/928/933, gender 837/813/810; person/mood/tense > 93 %
everywhere, GENDER 81–84 % is the next Latin problem, charter verb
form and voice 83–87 %. **T30 a (2026-09-09): THE GENDER CENSUS** —
`oraculum.sh -errata -nota <accidens|omnes>` lists feature errata
with partner, decision and ATTINGIBILE (a reading of the class carries
the gold value) | absens; rows `ERRATUM-NOTA`; gate law: an accident's
errata sum to verba − recti. Two ORACLE CONVENTIONS, not laws: gold
multi-values (`Gender=Fem,Masc`) any accepted (Seneca 837 → 847);
`commune` accepted for Masc/Fem like deponent for voice (Fran's
decision) with `conventione N` printed on every feature line so it
cannot hide (gender 897/890/885 pinned, 248/584/588 by convention;
shelf 856/901/923/866). CENSUS: commune 32/41 % of wrong (PROIEL
60 %), other unreachable 16/24 % (dictionary holes; known medieval
forms `sancte/bone` block the `-ae` variant — T22 asks only for
unknown forms), homonym lemma order 20/5 % (`armus/arma`,
`caelus/caelum`: WORDS frequency codes are in the dictionary, not on
readings), gerundive-before-gerund 7/12 %, determiner prior with the
partner's rarer reading 8/11 %, head rules 13/6 % (smallest — the pair
policy is not where gender points are). **T30 b (2026-09-09): THE
FREQUENCY ORDER** — `OratioDescriptio.frequentia` = WORDS stem
frequency code; the annotator stable-sorts each (class, language)
group of readings among its own positions by rank BEFORE nodes and
rules (class order untouched → primary cannot move; equal ranks keep
dictionary order; `ORATIO_ORDO_FREQUENTIAE=0` disables). Before the
rules unlike T24 because it changes no evidence, only which reading
of a case comes first; bound words repaired too. Nine treebanks:
primary identical, gender +5..+10 everywhere (pins 907/895/892),
Seneca number 917 and attachment 463; the `conventione` count
exposed `testes` moving to WORDS's common-gender entry on the
charters (same lemma). **T30 c / T22 b (2026-09-09): MEDIEVAL
VARIANTS ON KNOWN FORMS, GATED BY DIALECT** — a document has a
DIALECT (`OratioDialectus` classicus | medius) decided like its
language: `oratio_partes_praescandere` counts words T22 recovers
(charters 29 ‰, Dante 12, classical ≤ 1), `dialectus_censu` says
medius at 5 ‰, `annotare_dialecto` takes or decides it; the oracle
decides per FILE in the language pass. In a medius document a KNOWN
form also varies, only through `e-ae` (table flag `notis`;
`h-praefixa`/`e-i` on known forms REFUSED, lose everywhere) and only
into readings whose WORDS part is already native (`que` never becomes
`quae`); variants after native Latin readings, source `orthographia`.
Charters: primary +2/+1, forced +6/+11, case +9/+14, gender +18/+20,
attachment +15/+16; number −3/−1 NAMED (loose tier nominative-first
binds `sancte … ecclesie` plural) — Fran's decision; classical files
byte-identical. Unfiltered known-form variants lost primary on every
file: over-generated readings win spurious agreement (T28 again).
**T30 d (2026-09-09): RARITY OF INFLECTIONS** — gold: only CIRCSE
and PROIEL tag Ger/Gdv, the rest tag `-nd-` forms as Part with gender
(neuter 97 %); 332 of the charters' 467 verb-form errors were
infinitives read as WORDS's first-listed second-person passive.
Oracle conventions: gold Part/Ger accept `gerundivum`. Annotator
`RARITATES` table = second sort key after stem frequency: verb
readings with a rare inflection go after their stem's others —
`gerundivum-masculinum` (feminine kept, `-nda`), `passivum-secundae`
(deponents exempt); `participium-vocativus` measured null. Verb form
930/883 → 978/986, voice 930/871 → 982/999, gender charters 913/912 →
929/930, nothing else moves; verb form and voice PINNED at birth.
LESSON: a dictionary's inflection order is a prior nobody chose.
**T30 e (2026-09-09): LOOSE-TIER ORDER MEASURED** — the fourteen
literal head rules of stage 3 now run genitive, nominative, dative,
accusative, ablative, locative, vocative (nominative-first was the
enumeration order, never measured). Four orders on nine treebanks:
adopted genitive-first (case Seneca 666 → 670, charters +3/+7; number
917 → 920, charters 925/932 → 928/939; charters primary/forced/
attachment/gender −1..−2 NAMED: the genitive rules take 227 distant
bindings, readings better, arcs slightly worse — Fran's decision);
REFUSED the pair policy's ablative-first (gender and attachment fall
everywhere, Aquinas case −26: at distance the ablative finds spurious
pairs, T24's lesson) and accusative-before-nominative; dative vs
accusative order is INERT at distance. **T31 a (2026-09-09): THE
VERB STAR BEGINS — SUBJECT SLOT + LEX CAPITIS.** Census: explicit
subjects 40–69 % of finite verbs (empty slot = FINDING), nominative
77–89 % (charters a third accusative), within three words 65–74 %
(charters 30 %), nearest agreeing nominative = subject 69–81 %
(charters 45–52 %). Annotator writes `subiectum` umbrae on finite
3rd-person verb readings (impersonal and rare readings excluded); four
all-completion rules (adjacent in the strict tier, distant at the end
of the loose tier). Oracle counts arcs PER RELATION (`LIGATIO-RELATIO`),
subject precision pinned 520/244/394, recall 48/7/11 %. LEX CAPITIS =
executor's last pass: a subject/object binding to an ADJECTIVE with an
ADJACENT filled head binding follows it to the noun (determiners and
distant heads measured and excluded) — the first law that uses one
binding to correct another. Rarity rows `flexio-rarior` (WORDS
inflection code, syncopated perfect `-ere`) and `imperativum-futurum`.
Seneca case 670 → 690, forced 770 → 801, attachment 463 → 485, primary
841; shelf case +16..+40; charters case −2/−8, attachment dev −9 NAMED
(far, accusative subjects) — Fran's decision. **T31 b (2026-09-09):
OBJECT SLOT + LEX CARRIER.** Relation `obiectum-verbi` (5th) on finite
active/deponent readings of TRANS/DEP/SEMIDEP/X verbs (never passive
or rare); adjacent rules only — the loose tier LOSES everywhere
(charters −49/−50). LEX CARRIER: a verb-object order whose verb
reading is not the word's FIRST reading is refused — a bare case match
may fill a slot but never decide a class (without it homograph nouns
`amor`/`regis` became verbs, Seneca primary −26). Partner refusals
(class change, adposition membership, own subject) measured and
INACTIVE. Seneca attachment 485 → 503, case 690 → 704, object precision
55 % at recall 47 %, shelf attachment +10..+26; named falls 1–5 on
Seneca primary/forced/gender and charters gender/attachment — Fran's
decision. **T31 c (2026-09-09): OBJECTS ON INFINITIVES.** The object
slot goes on infinitive readings too (`_obiectum_forma_fert`;
`ORATIO_OBIECTUM_INFINITA=0|infinitivum|participium`); the subject
slot stays finite. Attachment up on every classical file (Dante +10),
case and forced up everywhere, object recall Seneca 47 → 52 %; charters
attachment −1/−4 NAMED (an accusative before a charter infinitive is
its object 16–20 % only). PARTICIPLES MEASURED AND REFUSED: case
+4..+13 but attachment −5/−3/−2 — the accusative is promoted rightly
and the arc goes to the participle instead of the main verb it stands
between (a right case with the wrong owner); kept inactive. NEXT =
dialect-gated loose subject tier, acc+inf subjects (a slot of their
own), the main verb claiming across a participle, §8.5 template
family, capitalised known forms, derived gender for commune.
**T31 d (2026-09-09): MULTIPASS PROBE + CENSUS HOME + REFUSAL
REGISTRY.** A naive second resolution pass (`ORATIO_RESOLUTIO_CURSUS=2`)
is inert on classical text and −5/−6 attachment on the charters: the
greedy rules re-fire on readings reordered by untrusted authors (new
arcs 2–19 % right); the omnes tiers see nothing new. Prior off
(`ORATIO_RESOLUTIO_PRIOR_ULTIMO=numquam`): case −41/−53/−63 but gender
+9 (the prior picks number/gender with the case). DECIDED ORDER: trust
table from the auctor tables → one head per word, best-wins → a
strict-decided tier. The gold census scripts and the measurement
harness live in `oratio/census/` (README there; `conllu.py` shared
reader). Refusals carry their COMPONENT in `oratio/recusata.md`
(annotatio | exsecutor | regulae | substratum | oraculum) and a retry
condition — a refusal is a fact about the configuration it was
measured against. **T32 a (2026-09-09): AUTHOR ON THE ARC + ONE HEAD
PER WORD + TRUST ON CONTESTS.** Umbrae carry their filling rule
(`auctor`); the oracle prints arc precision PER AUTHOR (a different
animal from reading accuracy: loose nominative head rules 11–18 %,
strict subject 70 %) and CONTESTED dependents. LEX CAPITIS UNIUS: a
claim on an already-claimed dependent is refused; FIDUCIA
(`fixa/auctores.tsv`, `census/fiducia.py`, regenerated only with a named
cause): the higher-trust claim wins the contest (`ORATIO_FIDUCIA=
contentio`; `ordo` = rules reordered by trust, MEASURED AND REFUSED:
case −10, rule order decides readings). LEX CAPITIS lives in the
contest (follow to the adjacent agreeing noun, else refuse, never
unseat the head). Attachment Seneca 505 → 526, charters 396/422 →
459/480, shelf +39..+67; RIGHT ARCS pinned as a FLOOR (Seneca 2057 →
1676 named: first-wins kept the right claim 46 %, the table is
charter-dominated). NEXT = losing claims kept as ALTERNATIVES with arc
coverage, per-dialect trust table, adjacency tie-break in contests,
strict-decided tier. **T32 b (2026-09-09): ALTERNATIVES ON THE
UMBRA + ARC COVERAGE.** Losing claims are kept as `alterna` nodes on
the umbra (partner, rule, cause recusata|revocata, trust, victor) —
nothing deleted, remapped like fillings; the oracle prints ARC COVERAGE
(any claim right) beside precision and the CONTEST table (winner rule,
loser rule, loser right %). Recoverable by ranking alone: Seneca 420,
Aquinas 447, charters ~100 — Seneca's residual is RANKING, the
charters' is PROPOSAL. Contest census: adjacent subject beats adjacent
object on a nom/acc word 1764× and the object was right 85 % → the
tie-break of the next tranche. `census/contentiones.py`,
`grep TECTUM|CONTENTIO`. **T32 c (2026-09-09): CONTEST JUDGE FROM
THE PAIR TABLE — REFUSED (R19).** The arc census lied once: adjacent
subject vs adjacent object on one word is the SAME arc (same verb), a
READING contest the arc oracle cannot judge; flipping it cost case −19.
Census corrected (loser judged against the winner's actual head, same
head = neither); four discriminating pairs remain; the judge
(`fixa/contentiones.tsv`, `ORATIO_IUDEX=1`, inactive) gains right arcs
but costs pinned case/forced/charter attachment. NEXT instrument = a
CASE-AWARE contest census. **T32 d (2026-09-10): CASE-AWARE CONTEST
CENSUS.** The oracle keeps the reading each side proposed for the
dependent and judges both against the gold case (`CONTENTIO` columns
IX–XI; `census/contentiones.py … -casus`). Adjacent subject vs adjacent
object of one verb: coin flip on Seneca's VERSE (121 : 123) and Aquinas
before the verb, accusative 2 : 1 to 20 : 1 on every prose file — a
GENRE split, not the dialect split. Predicted rule-order flip: Seneca
−7 cases, charters +76, Perseus +102, PROIEL +127 (measure, never
assume — rule order decides readings). Seneca's tie needs a
perpendicular feature: another nominative candidate for the verb.
**T32 e (2026-09-10): FEATURE CENSUS OVER CONTESTS (LITES).** One row
per contest with what the rule could see (`oraculum -machina -lites`,
`census/lites.py [-meet]`). The other-nominative hypothesis is WEAK and
inconsistent; the VERB'S NUMBER splits the coin flip on every file:
plural verb → subject (Seneca 53 : 20), singular verb → object-leaning
(126 : 152) — PRO-DROP. Predicted conditional rule order (subject
first when plural, object first when singular): Seneca +26 cases,
Aquinas +94, Perseus +128, PROIEL +129; residual coin flip = singular
verb with no agreeing nominative (Seneca 97 : 85) → semantics. A
quarter of the answers are neither `obj` nor `nsubj`. NEXT = T32 f,
the conditional order as a grammar edit, measured against the
prediction. **T32 f (2026-09-10): CONDITIONAL ORDER MEASURED AND
REFUSED AS A GLOBAL RULE (R20).** Plain (subject first when the verb
is plural, object first when singular): case and right arcs up on
every file, prediction held in direction not size (Seneca +14 words
of +26 predicted, Perseus +27 of +128), but Seneca primary −1, forced
−2, number −2, gender −4 ‰ — the '-a' words (bona, illa, haec: fem
nom sg vs neut acc pl) before a singular verb are SUBJECTS in verse
(82 : 41) and OBJECTS in prose (85 : 40). Number-refined variant
restores Seneca and breaks every prose file. GENRE-dependent; the pins
straddle both genres → NEXT = a GENRE GATE (verse / prose per document
from the T6b forma layer), then order per genre. KEPT: the executor
READING-CONTEST law in the verb star (same head as the standing claim
→ refused under order, never revoked by arc trust;
`ORATIO_CONTENTIO_LECTIONIS=0`; byte-identical today; a prerequisite
of any conditional order), the reading-number columns of the rows.

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
