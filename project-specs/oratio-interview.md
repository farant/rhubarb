# oratio — interview record (2026-09-04)

Raw questions and answers, in order, before the spec (`oratio-spec.md`).
Context carried into the interview from the conversation: a
natural-language (prose) parser as a materia client with
cross-language word classification; staged ladder (tree → Latin
dictionary → annotated words → English → resolution → search);
moderate realism as the stance on categories; ambiguity kept as
candidate lists at every layer; unknown word = finding, not failure;
names `oratio` / `vocabularium` / `partes` (`lexicon` is materia's,
`nomen` is a latina macro).

## Round 1 — the text

**Q1. Input unit under the byte law?** (a) plain text bytes, any
file — corpus gate fed the TEXT of every markdown paragraph via the md
parser; (b) markdown paragraphs from the start; (c) whole files with
non-prose passthrough.
**A.** (a) Plain text bytes, any file.

**Q2. Word joins — hyphenated compounds, contractions, Latin
enclitics (virumque): what does the tree hold?** (a) one word node with
parts; (b) split at the surface; (c) language-dependent tokenizer.
**A.** (a) One verbum owning all bytes with a list of parts; the
classification layer may analyze parts; the tree never decides the
linguistics.

**Q3. Latin orthography for lookup and lemmas?** (a) normalize at
lookup (u/v, i/j merged, macrons and ligatures stripped, case folded),
lemmas classical i/v as WORDS spells them; (b) ecclesiastical j/v;
(c) exact surface only.
**A.** (a).

**Q4. Sabaw as a declared target?** (a) Latin + English now, sabaw
reserved; (b) sabaw from the start; (c) out of scope.
**A (Fran).** Sabaw is not a normal conlang — a big library of xml tags
mostly borrowed from other languages — so not a concern now. Be
prepared for MORE languages: any natural language is fair game; see
lapide.org's language switcher for ones already translated into
(31 languages: Latin, Spanish, French, Portuguese, Italian, Romanian,
German, Dutch, Swedish, Russian, Polish, Japanese, Korean, Chinese,
Thai, Hindi, Bengali, Tamil, Malayalam, Gujarati, Indonesian,
Filipino, Cebuano, Vietnamese, Arabic, Persian, Hebrew, Swahili,
Kinyarwanda, Yoruba, Igbo) plus obvious ones like Greek. Some will
wait for a better unicode story in rhubarb, which will come.

## Round 2 — the classification layer

**Q5. Unicode floor of the word tokenizer now?** (a) UTF-8 multi-byte
sequences count as letters, no classes/folding for them yet; (b) ASCII
letters only, other bytes are signa; (c) minimal Unicode tables now.
**A.** (a).

**Q6. Representation of candidate analyses?** (a) one genus per
class with exactly its accidents as INDEX slots, mixed-genus list;
(b) one sparse analysis genus; (c) UD-style feature string.
**A.** (a).

**Q7. Universal class set as house ontology?** (a) UD's 17 UPOS
classes named in Latin, classical categories as accidents; (b) the
classical eight with UD mapped on; (c) both registries.
**A.** (a).

**Q8. Candidate ordering?** (a) ordered by source preference, primary
= first, nothing dropped, resolution may reorder never delete;
(b) unordered, no primary; (c) numeric rank from frequency codes.
**A.** (a).

## Round 3 — the dictionary as data

**Q9. Truth form of the compiled dictionary?** (a) vendored upstream
files are truth (FONTES.md, pinned bytes), compiled binary table is
generated and sealed, gate proves compile(source) == committed
binary; STML projection of an entry for inspection only; (b) STML
document is truth; (c) STML truth + binary projection.
**A.** (a).

**Q10. Unknown words — morphological hypotheses?** (a) finding only,
no hypotheses in stages 1–4, a later gated stage with an explicit
coniectura flag; (b) hypotheses flagged from the start; (c) never.
**A.** (a).

**Q11. Glossary of house neologisms?** (a) one hand-written
house-wide STML glossary (lemma, lingua, class, accidents, note),
canon-judged, highest-priority source; natura's per-genus glossae stay
documentation; (b) reuse natura glossae; (c) per-directory glossaries.
**A.** (a).

**Q12. Licenses for vendoring: WORDS (PD), Moby (PD), UD treebanks
(CC BY-SA 4.0 data as fixtures)?**
**A (Fran).** Definitely Whitaker and Moby; treebanks as test fixtures
sound okay, but explain the consequences of CC BY-SA 4.0 in our case.
**Explanation given.** Attribution is a FONTES file. ShareAlike applies
only to ADAPTED material; vendoring verbatim beside differently
licensed code is a collection, not an adaptation; the parser and its
output are not derivatives; the rule is: vendor verbatim, read only at
test time, never ship anything computed from a treebank (derived
tables live in build/). Caution: several Latin treebanks are
CC BY-NC-SA (non-commercial: Perseus, Index Thomisticus) — a gray
area for a repository whose products may be commercial; English EWT
is plain BY-SA; each Latin treebank's license to be verified in the
codebase pass, preferring BY-SA for the committed oracle.

## Round 4 — consumers

**Q13. Oracle vendoring rule after the explanation?** (a) vendor
BY-SA/PD verbatim, NC fetched into build/ for measurement only and
reported, not pinned; nothing derived ever shipped; (b) fetch all,
commit none; (c) vendor everything including NC.
**A.** (a).

**Q14. Identifier lint standard (multi-select)?** existence (every
underscore part known: dictionary, glossary, or allowed technical
terms list) / verbs infinitive for functions / nominative nouns for
types and variables / report only, no form rules yet.
**A.** Existence + report only, no form rules yet.

**Q15. Relationship to sententia_fissio?** (a) measure it, then
replace it behind the same API; (b) keep both; (c) replace
immediately.
**A.** (a).

**Q16. Language assignment for mixed documents?** (a) every word
looked up in every dictionary, lingua is a bit on each analysis,
document language a later derived summary; (b) declared per document
with overrides; (c) detect per paragraph.
**A.** (a).

## Round 5 — home and faces

**Q17. Where it lives?** (a) oratio/ top-level in the md/css layout;
(b) lingua/ umbrella; (c) under md/.
**A.** (a).

**Q18. Python face?** (a) silva.Oratio(via or text), query-only at
first (.sententiae, .verba(classis=, lingua=), .ignota, .analyses),
Prosa gains .sententia(n) by delegation; (b) extend Prosa; (c) none
until stage 3.
**A.** (a).

**Q19. First search consumer?** (a) lemmatize the tabularium/forum
FTS index (extra indexed column of candidate lemmas); (b) standalone
lemma index; (c) out of scope.
**A.** (a).

**Q20. Performance envelope for classifying the 1.2M-token corpus?**
(a) seconds: whole corpus under ~10 s, single file instant, computus
twin pins per-file cost; (b) batch minutes; (c) no target until
measured.
**A.** (a).

## Round 6 — measurement and edges

**Q21. Oracle metric pinned?** (a) both reported, 'any candidate
matches gold' pinned until stage 5, primary agreement printed from
birth and pinned when resolution moves it; (b) primary from birth;
(c) per class.
**A.** (a).

**Q22. Home of stage-5 resolution logic?** (a) STML pattern arms over
the projection (CASUS/PER), C only for the substrate, measured per
rule by the oracle — the 'logic as tag manipulation' experiment;
(b) C first; (c) both compared.
**A.** (a).

**Q23. WORDS meanings compiled now?** (a) compile, expose as a
derived 'sensus' string per analysis, not a semantic layer; (b) later.
**A (Fran).** (a) for now. In the future there will be a
language-independent SEMANTIC layer, maybe an extension of natura to
build out thorough ontology/semantic taxonomies — a ways down the line.

**Q24. Anything not asked (multi-select)?** Greek next / the C89
comments corpus as a second Latin corpus / sentence-level anchors for
sententiae / nothing more.
**A.** The C89 comments corpus is a second Latin corpus: every comment
in lib/ and silva/ is Latin prose; oratio should read them through
silva's tree and report unknown words there too.

## Round 5 — stage 5, resolution (2026-09-05, after T16)

**Q24. How does a plan row address the word and analysis it reorders,
given the engine zeroes byte positions on expanded clones?** (a)
ordinals stamped by the projection (`n=` on sententia, vocabulum,
analysis-*) and a C executor walking the materia tree by them; (b)
build the extent-provenance rule in the engine first; (c) mutate the
STML tree and re-read it. **A.** (a).

**Q25. How does the executor receive the plan?** (a) PER emits
`<consilium><praelatio .../></consilium>` rows into the expanded
document, canon-judged before execution; (b) C reads the in-memory
relation through a new engine entry point. **A.** (a).

**Q26. Several rules prefer different analyses for one word?** (a)
rule order = priority, first wins; (b) last wins; (c) weighted votes
(refused: an expression language). **A.** (a).

**Q27. Adjacency, which loose matching lacks?** (a) accept gaps in v1,
measure, pull the strict-run door with data; (b) pull the door now;
(c) make adjacency structural in the projection. **A.** (a).

**Q28. The walking skeleton's rule?** (a) adposition governs the
following case; (b) adjective–noun agreement; (c) document language.
**A.** (a).

**Q29. The language lever, an aggregate the vocabulary refuses?** (a)
C stamps the census as `lingua=` on the projection, the rule matches
it; (b) C pre-orders by language before patterns; (c) wait for the
aggregation door. **A.** (a).

**Q30. Measurement and pin?** (a) cumulative per-rule PRIMARY table,
pin per treebank only rising from the first rule; (b) pin the final
primary only; (c) report only. **A.** (a).

**Q31. Resolution by default?** (a) on wherever words are annotated
when the program exists, `-crudus` for the source order, oracle prints
both; (b) opt-in flag. **A.** (a).

