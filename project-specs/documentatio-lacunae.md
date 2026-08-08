# LACUNAE — what the 177 genera could not say about 2026-08-07/08

Phase 1 of the genera-generation loop. The scenario (the generated
`natura/cocta/glossae.html`, the six glossators, the peregrinus walk,
the `canon_examen` verdicts) was modelled with existing genera only.
The census is `documentatio_probatio.census`; it holds at **3 vitia,
all forced**, each of which is itself a miss (L1, L2, L3).

Discipline notes:

- Every entry names a **fact of the scenario**, not a wish for a tag.
- **Measured** = the gate refused it, or a probe proved it. **Argued**
  = no gate involvement; the corpus was searched (`natura_quaere.sh`
  over definitiones + species + umbrae, plus whole-file reads of
  `iudicium`, `fabricatio`, `opus_scriptum`, `schema`, `persona`,
  `communicatio`) and nothing carries it.
- Two candidate misses **died on contact** and are recorded at the end
  as non-misses, because a refusal with its reason is worth as much as
  a gap.

---

## A. The three forced vitia in the census

### L1 — A machine rendered a judgment, and judgments must have a person as author

**Fact.** `bin/canon_examen` judged the peregrinus' document: it named
the unknown root element, four unknown event verbs, two bad renderings
of `fons=`, a decimal, and a closure violation. Six verdicts, each with
a moment, a content, and a ground.

**Nearest.** `iudicium.auctor`, closure `ad="persona/nomen"`. Measured:
`<iudicium auctor="#canon_examen">` → *citatio non resoluta*. The same
law fires a second time at `pensum.commissum` (probe: an `actor` as
assignee is refused).

**Why it fails, precisely.** Not a slack closure. `persona`'s definitio
is *"Subiectum actionum liberarum: **auctor esse potest, non solum
causa**"* — authorship is definitionally personal, and the closure is
that doctrine enforced. So the corpus is not silent here; it is
**committed**, and the commitment leaves the fact homeless. `iudicium`'s
own header names three brothers of the proposition — `assertio` (proved
by the program, running), **`annotatio`** (proved by *tools*, outside
the program), `iudicium` (held by a person) — and **`annotatio` is not a
genus**. The corpus names the kind it lacks, in a comment, in the file
where it is needed.

Fallbacks tried and rejected: `assertio` (its `affirmat-de` closure
admits only `status`/`proiectio` kinds — an assertion cannot be about a
document); `observatio` (affirms nothing beyond appearance — a verdict
concludes); dropping `auctor` (then "the gate judged" is unsayable, and
`iudicium` says an author-less judgment is an orphan).

**Kind of gap.** NEW GENUS — the third brother, a proposition whose
holder is a mechanism: no assent (a machine does not adhere), no
revision by persuasion, but a stateable ground and a stateable moment.
Plus, downstream, a relation for a producer-that-is-not-an-author.

**Candidate name (suggestion only).** `annotatio` — the corpus's own
word for it, already used in `iudicium.genera` and in the TOLERA layer.
Collision check: `annotatio_machinae` already exists as a *species of
`commentarium`*, so the bare name is contested and the check matters.

---

### L2 — 352 glosses were made in one run and share a fate; only drugs may be batched

**Fact.** Six glossators produced 352 vernacular glossae in one
afternoon over a partitioned corpus. Common provenance, and therefore
common fate: if the glossing rule was wrong, all 352 are wrong at once.

**Nearest.** `pharmacon.coctura` — *"Omnia quae uno cursu fabricationis
facta sunt, provenientiam communem et ideo FATUM COMMUNE habentia"*.
The differentia fits without a word changed. Measured:
`<coctura coquitur="#glossae_opus">` → refused; the closure is
`medicamentum medicamentum-genericum medicamentum-nominatum
medicamentum-compositum/nomen`.

**Why it fails, precisely.** Wrong closure, not wrong differentia. The
genus generalises and its one load-bearing relation does not, so a
batch can exist in the census but cannot be a batch **of** anything
outside pharmacy. This is the exact failure mode RATIO.md §4 exists to
prevent ("the best-elaborated version available everywhere"), showing up
inside a genus that was already cross-domain-ready.

**Kind of gap.** NEW RELATION/CLOSURE on an existing genus — widen
`coquitur` to `*`, or give `coctura` a `res_coctae` relation with an
open target. Cheap; no ontology moves.

---

### L3 — A file was judged against a schema, and only norms may be conformed to

**Fact.** The peregrinus' document was judged against
`natura/cocta/individua.canon`.

**Nearest.** `plagula_computatralis.conformis_ad`, closure
`norma/nomen`. Measured: refused for a `schema` target; a probe with a
`norma` target passes, confirming the closure is the whole obstacle.

**Why it fails, precisely.** The corpus holds two genera that both fit a
canon and neither fits it wholly:

- `schema` — has `campus`, `genus_valoris`, `disciplina`
  (constringens/iudicans). Carries the *structure*. Cannot be conformed
  to.
- `norma` — *"Documentum publicum quod formam praescribit: quid
  conformitas significet definit"*. Carries the *conformity*. Has no
  fields.

Inverting does not rescue it: `schema.describit` is annotated *"genus
rerum quas format — schema de aliquo est"*, i.e. it points at a **kind**,
not at the instance judged. So "this file conforms to this schema" and
"this file was found non-conforming in these six places" are both
unsayable, and `schema.disciplina="iudicans"` — whose whole content is
*"accipitur et NOTATUR"* — has nowhere to put the notation.

**Kind of gap.** NEW PROPERTY/RELATION, and possibly a NEW GENUS for
the verdict-of-conformity itself (a schema that judges produces a
finding; nothing holds findings). Minimum viable: widen `conformis_ad`
to admit `schema`, and give `schema` an inverse for instance-level
judgment.

**Candidate name (suggestion only).** `conformitas` for the verdict, or
reuse of the L1 `annotatio` if that genus is built — a non-conformity
*is* a tool-held proposition about an artifact.

---

## B. Measured by probe (facts the gate would refuse)

### L4 — A probability cannot be written, including on the property built for it

**Fact.** The peregrinus attached a stated confidence to each of its 21
questions, from 0% to 70%.

**Nearest.** `iudicium.assensus → gradus_assensus` (scientia / opinio /
fides / suspicio / dubitatio) carries the *kind* of adherence and no
magnitude. `periculum.probabilitas` is the only numeric confidence in
the corpus, `genus="numerus"`, annotated *"inter 0 et 1"*.

**Why it fails, precisely.** Measured: `probabilitas="0.7"` → *valor
generi attributi non congruit*; `paginae="12.5"` likewise. `numerus`
admits integers. **So the one property in the corpus that documents its
own range as 0–1 cannot hold any value in that range except 0 and 1.**
The projection turned an unstated intention into an enforced lexical
rule and contradicted the property's own nota.

The root is one level below: `genus_valoris` declares itself *"INTENTIO
valoris, non repraesentatio machinae"* — it deliberately refuses to say
what a value looks like — and the generated canon must decide anyway.
RATIO.md §3 already names this law ("prose tolerates a distinction left
unmade; a generator must emit something either way"); this is a live
instance the corpus has not absorbed.

**Kind of gap.** NEW MECHANISM — `genus_valoris` needs to be able to
state lexical form/range without becoming a machine type, or the palette
needs a fractional member. Note this is a **format** question, and
RATIO.md's own rule says dimensions come before nouns.

**Candidate name (suggestion only).** `fractio` as a palette member; or
a `forma_lexica` property on `genus_valoris`.

---

### L5 — A requirement cannot state what it requires

**Fact.** The page owed the reader the ability to author a valid
document. The peregrinus produced three ranked improvement requirements.

**Nearest.** `fabricatio.postulatum` — *"Intentio declarata ANTE opus:
quod verum esse debet ut opus perfectum dicatur"*.

**Why it fails, precisely.** Measured: `<postulatum contentum="...">` →
*attributum extra canonem*. `postulatum` has a state machine and two
relations and **no properties at all**. The same probe kills
`probatio.contentum`, `facultas.contentum`, `vitium.contentum` and
`vitium.gravitas`. The whole *quadratum intentionis* is nameable and
its propositions are unstatable: you can say a requirement exists, was
received, and was fulfilled, but not what it said. (Contrast `iudicium`,
which has `contentum` — the epistemic module solved this and the
fabrication module did not.) `<nota>` as a child is also refused, so
there is no escape hatch either; every explanation in the census had to
go into an XML comment, which no reader can query.

**Kind of gap.** NEW PROPERTY on four existing genera (`contentum` /
`textus`), plus arguably a general free-text `nota` at the document
layer — a NEW MECHANISM, since the `.genera` layer has `<nota>` and the
census layer does not.

---

### L6 — Three improvements were ranked, and nothing carries rank

**Fact.** The peregrinus' improvement list was **ranked**, 1 to 3.

**Nearest.** `fabricatio.postulatum` (no ordering); `consilium` orders
`pensum`, i.e. *works to be done in sequence*, which is not the same
claim — a rank is expected value, not a dependency. The only ordinal in
the corpus is `gradus_praescripti.ordo`, local to recipe steps.

**Why it fails, precisely.** Measured: `<postulatum ordo="1">` →
*attributum extra canonem*. Priority is a judgment *about* a set of
items relative to one another, and the corpus has judgments about single
things only (`iudicium.de` is singular).

**Kind of gap.** NEW PROPERTY (`ordo` / `momentum_ponderis` on
`postulatum`), or — the more interesting reading — a NEW GENUS for a
*comparative* judgment, since the corpus's whole epistemic layer assumes
one proposition about one subject.

---

### L7 — A closed vocabulary's members cannot be recorded

**Fact.** The event vocabulary is closed; `plantare`, `putare`,
`irrigare`, `metere` were refused for not being in it. The peregrinus
could not have known this from the page.

**Nearest.** `schema.campus.genus_eius → genus_valoris`, whose species
include `electio`.

**Why it fails, precisely.** Measured: `<electio><optio>…</optio>
</electio>` → *liberum hic non licet*. `genus_valoris` and every one of
its species (`textus`, `numerus`, `dies`, `veritas`, `electio`,
`citatio`) project to elements with **`nomen` and nothing else**. So the
corpus can say a field is a choice and never which choices — the exact
fact whose absence sent the peregrinus guessing. `campus.ordinarius_valor`
exists, so the corpus can state a default it cannot state the domain of.

**Kind of gap.** NEW PROPERTY/RELATION on `electio` (its options), which
in the format probably means a NEW MECHANISM: a value genus that carries
instance data.

---

## C. Argued (searched, nothing carries them)

### L8 — Six agents authored 352 glosses, and they are not persons

**Fact.** Six glossators, each rendering Latin definitiones into English
and French, subordinate to the Latin, never renaming the tag.

**Nearest.** `communicatio.actor` — used in the census, and honestly the
least-bad fit: *"Ens logicum cum identitate, statu privato, moribus: cum
aliis SOLUM per nuntios communicat"* is literally true of these agents
(SendMessage is the only channel). But `actor` is Hewitt's actor,
"realized in this house in actor.c"; it describes their **communication
discipline** and says nothing about them doing intellectual work.
`processus` describes the OS instance. `officium` is a post held by a
person. `organizatio` is a *universitas personarum* — so the "house"
(Fran plus N agents) is not one either, which is why
`peritia_communis.residet_in` could not be filled in the census.

**Why it fails, precisely.** Missing kind entirely, and L1's doctrine is
the reason: `persona` reserves authorship, and the corpus has no name
for a thing that produces text and judgment without being a person.
Note the honest measurement here: the census's
`<opus-scriptum><auctor ad="#glossator_a"/></opus-scriptum>` **passed**
the gate, because `auctor` is multiplex on `opus_scriptum` and the
monolith's own preface admits that 402 multiplex relations are judged by
name only. So this fact is expressible today only through a hole in the
projection, not because the model permits it.

**Kind of gap.** NEW GENUS. This is the largest one and it should be
walked, not reasoned about — RATIO.md §5's rule that candidates
surviving only in the abstract have not been tested applies with full
force.

**Candidate name (suggestion only).** deliberately withheld. The
temptation is `agens`, and `agens` is already an *attribute name* on
`interventio`; a name chosen before the walk would be a name to argue
with afterwards.

---

### L9 — The page is a dictionary; a manual was needed

**Fact.** The peregrinus' own conclusion. The page teaches what each
kind **is**; authoring a document requires knowing what to **do**.

**Nearest.** `opus_scriptum` — and its `materia` property is annotated
*"de quo agit — subiectum, **non genus litterarium**"*, an explicit
refusal to carry literary genre. `praescriptum` / `procedura` carry
how-to but are procedures, not documents about a subject.
`index_verificationis` distinguishes *"CONFIRMAT quod factum sit; non
docet quomodo fiat"* — the corpus has the confirm/teach distinction in
one narrow place and nowhere general.

**Why it fails, precisely.** The refusal was made at the **property**
level (genre is not a property of a work) and is defensible there. The
gap it leaves is at the **kind** level: dictionary and manual differ not
in subject but in *what they enable a reader to do*, and that is a real
division with real consequences — this scenario measured them.

**Kind of gap.** NEW SPECIES of `opus_scriptum` divided by reader-task,
or a NEW GENUS if the division turns out to be about the text-reader
relation rather than the text.

---

### L10 — A worked example is a kind of artifact, and so is a legend

**Fact.** Two artifacts the peregrinus named as missing: a complete
specimen document it could copy, and a key to the page's notation.

**Nearest.** Searched: `exemplum`, `manuale`, `legenda`, `glossa` — no
genus, no species, no umbra. `natura/exempla/hortus.census` **is** a
worked example and the corpus has no word for what it is.
`schema_identificandi` has `praefixum` / `nota_custos` parts; nothing
analogous for notation.

**Why it fails, precisely.** Missing kinds entirely. Both are
document-parts whose value is *pedagogical*, and the corpus's document
layer (`opus_scriptum` → `expressio` → `editio` → `codex`) is FRBR,
which is about identity and has no part-of-a-document layer at all.

**Kind of gap.** NEW GENERA, plausibly parts of whatever L9 turns into.

---

### L11 — Three ways a document can fail: silence, ambiguity, active misleading

**Fact.** The peregrinus' classification. SILENCE: the page never names
the root element. AMBIGUITY: `...` means three different things. ACTIVE
MISLEADING: the truncation `a|b|c|d|...` hid the value `mortua`,
teaching that a plant's death is unsayable.

**Nearest.** `fabricatio.vitium` — used in the census for all three,
and it strains. Its definitio is *"Discrepantia inter intentionem et
mores"*, and a document has no *mores*; the discrepancy shows up in the
reader's construal, not in the artifact's behaviour. `iudicium.ignotum`
covers the **reader's** side of silence (a named gap) and not the
**page's** side. Searched `ambiguit`: one hit, a nota on
`procedura_automata` (*"AMBIGUITATIS TOLERANTIA NULLA"*) — the corpus
knows ambiguity matters and has no genus for it.

**Why it fails, precisely.** Three distinct problems under one strained
genus:

- **Silence** is a fourth kind of absence. `persona.mortuus` already
  records three (empty-means-living, `ignotum`, and *"IUDICIUM de eo
  quod dici non debet"*); this is a fifth column — absence in the
  **telling**, where the knowledge exists and the document withholds it.
- **Ambiguity** is missing entirely.
- **Active misleading** is the sharp one, and it has a near-miss:
  `depravatio` is *"latet natura sua… datum pravum tam bene sedet quam
  rectum"*, which is the right shape — but `depravatio` is about **data
  gone wrong**, not about a text that **teaches** a falsehood. The
  census proves the difference by writing
  `<planta status-vita="mortua"/>`, which passes: the corpus could
  always say it; only the page could not.

**Kind of gap.** NEW SPECIES of `vitium` if `vitium` is widened to
artifacts-read-by-people, otherwise NEW GENUS. Ranked by force: active
misleading first (it has a victim), then silence, then ambiguity.

---

### L12 — One manifestation, three expressions

**Fact.** `glossae.html` is one file carrying Latin, English and French.

**Nearest.** `opus_scriptum.editio.editio_de → expressio`, single-valued.

**Why it fails, precisely.** Wrong cardinality. FRBR itself allows a
manifestation to embody several expressions (a facing-page bilingual
edition is the standard case); the corpus modelled the common case and
the projection froze it as an attribute rather than a repeatable child.
The census expresses the trilinguality as three `expressio` individuals
and then cannot bind them to the one artifact.

**Kind of gap.** NEW CARDINALITY on an existing relation — `editio_de`
`multiplex="verum"`. Smallest fix in this ledger.

---

### L13 — A generated document cannot name the work it realizes

**Fact.** `glossae.html` is simultaneously a written work (an
encyclopedia), a build artifact (`natura_glossae` from the corpus), and
a file (HTML bytes). The census models all three and cannot connect them.

**Nearest.** `artefactum_aedificationis.veritas_sua`, closure
`status proiectio artefactum-aedificationis obiectum-compilatum
exsecutabile-ligatum promptuarium index …/nomen` — program state only.
`opus_scriptum` has no relation to any artifact.

**Why it fails, precisely.** Wrong closure, from a real assumption: the
projection doctrine (`veritas` / `proiectio`) was built for program
state, where the source of truth is always itself state. A generated
*document* has a source of truth that is a **work**, and the closure
cannot see it. The result is that the census's most load-bearing single
fact — *this page is a projection of the natura corpus, and the corpus
is the truth* — is expressible only through the deliberately vague
`ex_fontibus`.

**Kind of gap.** NEW CLOSURE (widen `veritas_sua` to `*`), or a
recognition that documentary projection is a distinct relation.

---

### L14 — Every gloss is subordinate to the Latin, and subordination is unsayable

**Fact.** The glossator rule: a gloss renders the Latin definitio
standalone, **subordinate to it**, never renaming the tag. Where gloss
and Latin disagree, the Latin governs.

**Nearest.** `expressio.translata_ex` — records that B derives from A.
`expressio.genus_expressionis` = originalis / translata / recensita /
abbreviata — records the *manner*.

**Why it fails, precisely.** Derivation is not authority. Two
translations may both derive from the same original and differ in
whether either is normative; a certified translation of a statute
governs and a courtesy translation does not. The corpus has this
distinction elsewhere in a different vocabulary (`norma.substituit`
chains, `versio` supersession) and not for expressions.

**Kind of gap.** NEW PROPERTY on `expressio` (`auctoritativa`, or a
`subordinatur_ad` relation).

---

### L15 — A stranger reading a page is a test, and tests only exercise functions

**Fact.** The peregrinus was a deliberate experimental control: an agent
restricted to one source so that the source could be measured.

**Nearest.** `fabricatio.probatio` — *"Interrogatio exsecutabilis
intentionis: scaena parata, actio exercita, effectus contra
exspectationem collatus"*. That fits exactly, which is why the census
uses it, and its own nota (*"apparatus fictus assumptiones auctoris
communicat"*) is precisely the failure the peregrinus method exists to
avoid.

**Why it fails, precisely.** `probatio.exercet → functio`,
`continet → assertio`, and `assertio.affirmat_de` admits only program
state. So a test whose subject is a **document** and whose instrument is
an **ignorant reader** cannot state what it exercised or what it
asserted. (The census gets `<exercet ad="#pl_x"/>` past the gate only
because multiplex relations are unchecked — the same hole as L8, and a
second reason not to trust a green gate here.)

**Kind of gap.** NEW SPECIES of `probatio` (a documentation test, whose
oracle is a reader's success, not an assertion) plus widened closures on
`exercet` and `affirmat_de`.

---

## D. Candidates that died on contact — recorded, not claimed

**"There is no way to count things."** False; my ignorance. `mensura`
has `numerositas` as a species of `magnitudo_mensurabilis`, so a
`unitas_mensurae` for counted things can be minted and `quantitas` can
cite it. Measured: `<quantitas valor="352" in-unitate="#unitas_rerum"/>`
passes. Now in the census (§XVI) rather than in this ledger.

**"There is no genus for a false thing that passes a validity check."**
False, and the corpus is better than I expected. `fabricatio.depravatio`
is exactly it: *"Status rei factus alius quam esse debuit, sive quis
animadvertat sive non… LATET NATURA SUA… sola triadis QUAERENDA est"*.
The current height recorded in `altitudo-matura` is a `depravatio` in
state `latens`, and the reason `canon_examen` cannot see it is the
reason the genus exists. **Validity is not truth** is already doctrine
here; only the *conformity verdict* side is missing, and that is L3.

---

## E. Where I think the seam actually is

The scenario put three modules under load and they did not fail equally.

**`iudicium` carried the most weight and carried it well.** The 21
questions are `ignotum` — and choosing among its species (`ignotum_notum`
vs `controversum` for the `fons=` question, which had *two* answers
rather than none) forced a distinction the peregrinus' own notes had
blurred. `fons`, `fundamentum` (`illatio` for what it inferred from the
syntax sketch, `praesumptio` for what it assumed), `gradus_assensus`,
`observatio` and `investigatio` all fit without strain;
`investigatio.parit → postulatum` (*"quod investigatio VALET non est
scire quid acciderit sed quid mutandum sit"*) received the improvement
list exactly. Its one gap is L1 — the author must be a person — and that
is a doctrine, not an oversight.

**`fabricatio` gave the right shapes and no room to write in them.** The
*quadratum intentionis* is the correct frame for this scenario and every
one of its four genera is contentless (L5). `depravatio` was the single
best fit in the whole exercise. `vitium` is being asked to cover
document defects it was not built for (L11).

**`opus_scriptum` was thinner than expected**, because FRBR is a theory
of *identity* and this scenario is about *use*. Work/expression/edition
gave a clean account of the trilinguality right up to the point where
one file had to hold three expressions (L12), and nothing in it connects
a work to the artifact that realizes it (L13).

**So: what would a `documentatio` model own?** Not the text's identity —
`opus_scriptum` owns that and owns it well. Not the reader's epistemic
state — `iudicium` owns that and is the strongest module in the corpus.
Not intent-versus-behaviour — `fabricatio` owns that.

What none of them owns is **the relation between a text and a reader's
task**. Everything that went wrong on 2026-08-08 lives in that relation:
a page can be complete as a description and useless as an instrument
(L9); its defects are defined by an attempt rather than a spec (L11,
L15); its parts have pedagogical rather than bibliographic roles (L10);
and its verdicts are about *whether someone succeeded*, not whether the
text is true. The corpus can already describe the text, the reader, and
the failure — separately. It cannot say **"this document failed this
reader at this task,"** which is the one sentence the whole scenario was
about.

Concretely, the seam I would propose:

- `documentatio` owns: the reader-task genus of a document (dictionary /
  manual / worked example / legend / reference), the pedagogical
  document-part, the reader-defect trichotomy, and the reader-walk as a
  test with a human-or-agent oracle.
- It **cites** `opus_scriptum` for identity, `iudicium` for the reader's
  gaps and guesses, `fabricatio` for the requirement/defect frame, and
  `schema` for the grammar the document is teaching.
- It does **not** own: authorship (L1/L8 belong to a separate and larger
  question about non-personal producers, which the coverage loop should
  walk on its own), conformity verdicts (L3, which belongs with
  `schema`/`norma`), or value-type lexical form (L4/L7, which is a
  metamodel question and by RATIO.md's own rule — dimensions before
  nouns — should be settled before any new genera are written).

**Priority, if only one thing is done:** L5. Four genera in the
intention quadrangle that cannot state their own propositions is a
defect in the corpus's most-used module, it costs one property each, and
until it is fixed no `documentatio` model can record what a document
owed anybody.
