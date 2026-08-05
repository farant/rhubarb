# METAMODULUS — the natura format specification

**Status:** descriptive, v1. Describes the format as the models
actually use it — the vocabulary below was extracted mechanically
from the corpus, not from memory (27 elements, 26 attributes).

**Live counts live in `natura/INDEX.md`**, which is regenerated on
every validator run. This document deliberately carries NO tallies
of models or genera: a specification that quotes a moving number
is stale the next afternoon.

**Audience:** anyone writing a model by hand, and whoever
implements the loader. The loader is expected to enforce the
NORMATIVE RULES section.

**Language note (recorded decision, not drift):** this file is
English prose with Latin identifiers, following the precedent of
`natura.worklog.md` in the same directory. The *artifacts* are
Latin; meta-documentation about the artifacts is English. If that
flips, it is a mechanical translation.

**History lives elsewhere.** Every mechanism here was forced by
specific content, and `natura.worklog.md` records what forced it.
This file says what the format IS; the worklog says how we came
to say it. When they disagree, the models are the truth and both
documents are wrong.

---

## 1. Doctrine that governs the format

Five commitments, each with consequences in the grammar:

1. **Pieper** — describe things as they are, not as applications
   want them. Consequence: no field exists because a use case
   asked; every property must be true of the kind.
2. **Linnaeus** — taxonomy and dictionary in ONE artifact. A
   model carries both the form of the genus and described
   species/individuals beneath it.
3. **Alexander** — hand-crafted, plastic, format learned from
   content. Consequence: nothing here was designed in advance,
   and this document is descriptive rather than prescriptive.
4. **Residence rule** — the library holds what is timelessly
   describable (kinds at every rank, plus individuals of public
   record). YOUR individuals live in application volumina and
   CITE the library.
5. **Historia doctrine** — a thing's history is its event log;
   its current status is a FOLD of that log. (The storage-layer
   doctrine one level up.)

---

## 2. Document shape

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- header comment: what this model is, what forced it,
     transformations, open tensions -->
<natura modulus="NAME" versio="N" lingua="latina">
  <fontes>…</fontes>          <!-- optional, module-level -->
  <genus nomen="…">…</genus>  <!-- one or more -->
</natura>
```

One model = one file = `natura/<modulus>.stml`. The `modulus`
attribute MUST equal the filename stem.

**Models are files, not kingdoms.** The taxonomy is ONE tree;
splitting it across files is our convenience. Genera, properties
and relations may all cross model boundaries (§5).

---

## 3. Element reference

### Structural

| Element | Where | Attributes | Content |
|---|---|---|---|
| `natura` | root | `modulus` `versio` `lingua` | genera, optional `fontes` |
| `genus` | in `natura`, or nested in a `genus`/`species` for taxonomic depth | `nomen`, opt `sub` `modulus` `gradus` | everything below |
| `species` | in `genus` | `nomen`, opt `gradus` `etiam` | definitio, differentia, valor, relatum, historia, nota, dubium, nested `genus` |
| `individuum` | in `genus` | `nomen` | same as species |
| `cultivar` | in `species` | `nomen` | same — a rank below species (`planta` only) |

`species` vs `individuum` is the format's central distinction: a
species is a KIND (C, pneumonia, the rose), an individuum is a
THING (Ritchie, c89, the kilogram).

### Descriptive

| Element | Purpose |
|---|---|
| `definitio` | dictionary prose: what this is |
| `differentia` | what distinguishes it from its genus |
| `nota` | commentary that is not definitional |
| `dubium` | an unresolved tension, carried deliberately |

`dubium` is load-bearing: honest descriptions carry their open
questions. Resolved dubia become comments recording the
resolution, never silent deletions.

### Properties

```xml
<proprietates>
  <proprietas nomen="…" genus="…" [modulus=] [multiplex=]
              [ordinarius=] [nota=]>
    <optio>…</optio>          <!-- when genus="electio" -->
  </proprietas>
</proprietates>
```

`genus=` names the value kind. Palette primitives: `textus`,
`numerus`, `dies`, `veritas`, `electio`, plus `relatio` (a
reference). A property may also be typed by a LIBRARY GENUS with
`modulus=` (§5.2).

### Composition

```xml
<partes>
  <pars nomen="…" necessaria="verum|falsum" [nota=]/>
</partes>
```

Essential composition (a plant HAS a root). `necessaria="falsum"`
is honest, not a defect: ferns have no flower.

### State machines

```xml
<machina_statuum nomen="…" [gerens="species|individuum"] [nota=]>
  <status nomen="…" [nota=]/>
  <transitus a="…" ad="…" per="…" [nota=]/>
</machina_statuum>
```

`gerens` names the BEARER — who is in this state. The species
macOS never sleeps; an installation does. Machines may contain
cycles (a perennial reflowers). A genus may declare several
machines (`vita`, `cursus`, `vigor`…).

### Actions and relations

```xml
<actiones>
  <actio nomen="…" [nota=]/>
</actiones>

<relationes>
  <relatio nomen="…" ad="TARGET" [modulus=] [multiplex=]
           [externum=] [nota=]/>
  <relatio nomen="…" a="SOURCE" inversa="verum" [modulus=] [nota=]/>
</relationes>
```

- `ad="*"` = deliberately open (may point at anything).
- `externum="verum"` = the target genus is SURFACED but not yet
  described. These are the library's computed agenda (§7).
- `inversa="verum"` + `a=` declares the reverse of a relation
  owned elsewhere. Documentation, not a second edge.

### Dictionary content

```xml
<valor nomen="PROPERTY" [certitudo=] [fons=] [valens_a=]
       [valens_ad=] [nota=]>value</valor>

<relatum nomen="RELATION" ad="TARGET" [modulus=] [externum=]
         [certitudo=] [fons=] [valens_a=] [valens_ad=] [nota=]/>

<historia>
  <eventum quando="DATE" [actio=] [certitudo=] [fons=] [nota=]>
    description
  </eventum>
</historia>
```

A `valor` at GENUS level fixes an inherited property for
everything beneath (the proprium of a subordinate genus).

### Sources

```xml
<fontes>
  <fons clavis="KEY" genus="SPECIES-OF-FONS" modulus="iudicium"
        [verificatus="verum|falsum"]>prose</fons>
</fontes>
```

Module-level bibliography; `fons="KEY"` on a claim cites it (§6).

---

## 4. Attribute reference

| Attribute | On | Meaning |
|---|---|---|
| `nomen` | nearly everything | the identifier |
| `modulus` | natura; genus; proprietas; relatio; relatum; fons | which model the referenced thing lives in |
| `versio` `lingua` | natura | model version; always `latina` so far |
| `sub` | genus | parent genus (Porphyrian spine) |
| `gradus` | genus, species | rank name in domain terms (familia/genus/species/cultivar) |
| `etiam` | species | essential dual membership (rare by design) |
| `genus` | proprietas, fons | the value kind / the source kind |
| `ad` | relatio, relatum, transitus | target (or destination state) |
| `a` | relatio, transitus | source model of an inverse / origin state |
| `per` | transitus | the transition's name |
| `multiplex` | proprietas, relatio | may hold several values |
| `ordinarius` | proprietas | default value |
| `necessaria` | pars | is this part essential |
| `externum` | relatio, relatum | target surfaced, not yet described |
| `inversa` | relatio | this declares the reverse of a relation owned elsewhere |
| `gerens` | machina_statuum | bearer of the state |
| `quando` | eventum | when it happened |
| `actio` | eventum | which declared actio this event instantiates |
| `clavis` | fons | citation key |
| `verificatus` | fons | has anyone actually checked it (default falsum) |
| `certitudo` | valor, relatum, eventum | degree of assent (§6) |
| `fons` | valor, relatum, eventum | citation key (§6) |
| `valens_a` `valens_ad` | valor, relatum | valid-time bounds (§6) |
| `nota` | most elements | inline commentary |

**Two known warts, recorded rather than hidden:**

1. `genus` is both an ELEMENT (a kind) and an ATTRIBUTE (a value
   type). Context disambiguates, but a parser must not assume.
2. `a` / `ad` mean origin/destination on `transitus` and
   source/target on `relatio`. Analogous, not identical.

---

## 5. Cross-model mechanisms

The taxonomy is one tree; three kinds of reference cross files,
and all three use `modulus=` to name the other model.

**5.1 Relations** (day one) —
`<relatio ad="persona" modulus="persona"/>`

**5.2 Library-typed properties** (round 14) — a property whose
type is a library genus rather than a palette primitive:
`<proprietas nomen="massa" genus="quantitas" modulus="mensura"/>`

**5.3 Subordination** (round 17) — a genus whose parent lives in
another model:
`<genus nomen="planta" sub="vivens" modulus="corporalia">`

---

## 6. The three coordinates of a claim

A dictionary claim (`valor`, `relatum`, `eventum`) may carry
three independent qualifications. All are optional; their absence
has defined meaning.

**How sure — `certitudo`** (round 16). Value MUST be a species of
`iudicium.gradus_assensus`: `scientia` (certain, demonstrated or
stipulated), `opinio` (held with fear of the opposite), `fides`
(held on another's authority), `suspicio` (weak inclination),
`dubitatio` (assent suspended). Absent = asserted as the model's
own.

**From whom — `fons`** (round 16). Value MUST be a `clavis`
declared in a `<fontes>` block. The species
`corpus_institutionis` marks claims taken from the scribe's
training memory: unverifiable by nature, and saying so calls the
reader to verification rather than to belief.

**True when — `valens_a` / `valens_ad`** (round 18). Format:
`yyyy`, `yyyy-mm`, or `yyyy-mm-dd`. Absent `valens_ad` = STILL
HOLDS (following `organizatio.dissoluta`'s "vacuum = perdurans").
If genuinely unknown, say so with `certitudo` or a `nota`.

### When to mark valid-time — the triple rule

| Case | Example | Mark? |
|---|---|---|
| I. Essentials — definitions, differentiae, species properties | "C is imperative" | **Never.** What belongs to a thing as such has no date. |
| II. Statuses — machine values | `vita=vigens` | **No.** A status IS a fold of historia; it means "now" by definition. |
| III. Relational accidents — parents, prices, locations, employments | `bell_labs pars_de att` | **Yes, always.** These were true, ceased, and flat form lies. |

Measured: of 117 dictionary relations in the corpus, exactly ONE
was case III. That is why the corpus survived nearly lie-free for
its first twenty-five models: it was writing cases I and II.

**Reification is the alternative.** A time-bound fact may instead
become an entity with its own properties (`commercium.pretium`
carries `valens_a`/`valens_ad` as properties). Heavier but
richer — choose it when the fact has its own author or cause;
choose attributes when the fact is bare.

---

## 7. Umbrae — the computed agenda

`externum="verum"` marks a genus named but not yet described.
`tools/natura_visus.sh` collects these into a list. This is the
library's work queue, MEASURED rather than remembered — it once
recovered four surfaced genera that had been forgotten, and
`animal` waited ten rounds as an umbra before being described.

Removing `externum` when the target is described is REQUIRED;
a stale flag is caught by the validator only if the reference
also resolves, so treat it as a discipline.

---

## 8. Normative rules (the loader's contract)

Rules marked **[E]** are enforced today by
`tools/natura_visus.sh` (exit 1 on violation). Rules marked
**[S]** are specified but not yet enforced — they need
genus-inheritance resolution, which is precisely the loader's
job and the reason to build it.

| # | Rule | Status |
|---|---|---|
| 1 | Every file is well-formed XML | [E] (`xmllint`) |
| 2 | `relatio`/`relatum` `ad=` resolves to a genus or dictionary entry in the named model, unless `externum` or `ad="*"` | [E] |
| 3 | `proprietas` with `modulus=` resolves to a genus in that model | [E] |
| 4 | `genus` with `sub=` + `modulus=` resolves cross-model | [E] |
| 5 | `fons=` resolves to a declared `clavis` | [E] |
| 6 | `certitudo=` is a species of `gradus_assensus` | [E] |
| 7 | `valens_a`/`valens_ad` are well-formed dates and correctly ordered | [E] |
| 8 | Only known elements and attributes appear (this document's §3/§4) | [E] |
| 9 | `valor nomen=` names a property declared on the entry's genus or an ancestor (or a `machina_statuum`) | [S] |
| 10 | A `valor` for an `electio` property matches a declared `optio` | [S] |
| 11 | `relatum nomen=` names a relation declared on the genus or an ancestor | [S] |
| 12 | `transitus a=`/`ad=` name declared statuses of the same machine | [S] |
| 13 | `eventum actio=` names a declared `actio` | [S] |
| 14 | `modulus` attribute equals the filename stem | [S] |
| 15 | No two genera share a name within a model | [S] |

Rules 9–13 are the Cyc-consistency defense: they are what makes a
hand-written library stay coherent past the point where one mind
holds it.

---

## 9. Conventions

### When to make a new genus (Fran's ruling, 2026-08-05)

**The test is ONTOLOGICAL, not pragmatic.** Does this name
something real — a distinction that exists in the things
themselves? Moderate realism: universals are real, *in re*. If
the answer is yes, coin the genus, even with no application in
view.

Do **not** ask "does this buy us anything." That is a use-case
test wearing a taxonomy costume — the Pieper failure relocated
from the property layer to the genus layer, and harder to see
there.

**The risk is asymmetric.** Failing to name a real distinction is
nominalism creeping in: collapsing a real difference because it
is inconvenient. A redundant genus for something real costs
almost nothing and is locally fixable. So when uncertain,
DIVIDE.

Expect **hundreds** of genera at general scope. That is the
correct order of magnitude for describing things as they are,
and the work does not become obsolete.

### Genera vs. analogies (Fran, 2026-08-05)

Working across domains surfaces **cross-domain rhymes**: the
mailbox is to an actor what parameters are to a function; a
ledger's entries are to its balance what a volumen's acta are to
its manifest; a bearer bond is to its goods what a bearer token
is to its authority.

These are real, and they are **not genera**. They are ANALOGIES —
mostly *analogia proportionalitatis*, A:B :: C:D. A genus admits
univocal predication (a dog and a horse are each *animal* in the
same sense); an analogy does not. Forcing an analogy into a genus
is where the material stops supporting the abstraction, and it is
the classic failure of grand computer-science ontologies —
everything-is-an-object, everything-is-a-stream, deep inheritance
towers, universal upper ontologies. The pattern is real; the
shared essence was invented.

**Why this is a moderate-realist advantage.** Nominalism is good
at *finding* these patterns (it looks for regularity in usage,
not essence) but has no way to say "genuinely similar, not the
same kind" — so every strong similarity becomes a candidate
abstraction, and gears spin. Moderate realism can affirm the
resemblance, decline the genus, **articulate the individual case,
and move on.**

**Practice:**
- Default home for a cross-domain rhyme is a `nota` on both
  sides. Cheap, honest, loses nothing.
- Make it a **genus** only if it names a real kind — not merely
  a real resemblance.
- Use a **relation** when the tie is between two named things.
- Interfaces and duck typing are the engineering form of
  analogical grouping: "responds to read()" is a claim about
  behaviour, not essence. Useful, and on a different axis from
  the Porphyrian spine — which is why such groupings should not
  be forced onto it.

The deferral of a catch-all "design pattern" genus (§10) was this
same judgement made before it had a name.

### The Linnaean escape hatch (Fran, 2026-08-05)

**When an abstraction question stalls, stop arguing and go
collect specimens.** What is actually out there in the wild?

This is the signature affordance of the moderate-realist
position, and it is not available to the alternatives:

- If universals are real *in the things*, then the things are
  the evidence. Surveying specimens is going to where the
  universal actually lives, so it can settle the question.
- Under nominalism, more specimens are just more particulars —
  the grouping is a convention, so collection yields data to
  impose a scheme on, never a verdict. You are left arguing
  about which scheme is more useful, which has no ground.
- Under Platonic realism the form is elsewhere, so specimens are
  shadows and one reasons toward it a priori — which produces
  taxonomies that match nothing.

**Prior projects in this space, when stuck, went UP** — another
axiom, another abstraction layer, another negotiated upper
ontology. The escape hatch goes DOWN. Collecting specimens is
also *bounded and terminating* work, where arguing about levels
is neither.

**The specimens can reframe the question, not merely answer it.**
Worked example (round XXIII): "is `charta_negotiabilis` a genus
whose name is too narrow?" — an abstraction question. Collecting
bills of lading, warehouse receipts, banknotes, tickets,
cloakroom tokens and digital bearer tokens produced a different
answer than either option on the table: there were TWO kinds,
split by a doctrine (clean title to a good-faith purchaser) that
the abstraction framing had not noticed at all. Specimens can
push back only if there is something real in them.

This composes with the coining rule above: because a genus is
cheap and local, the specimens are free to speak. If coining
were expensive, every survey would end in a dilemma.

**Precondition: specimen access** (Fran's diagnosis). Prior
projects did not refuse the Linnaean move out of philosophical
error — they were structurally unable to make it. Taxonomies
were built by programmers, not domain experts: aware of some
details, ignorant of others, abstracting from partial
information. With no specimens, going UP is the only move
available. Knowledge engineering by interview was slow, lossy
and expensive, so economising on expert contact and spending
effort on formalism was the *rational* response to that cost
structure — and it produced exactly the failures we now name.
The cost structure has since inverted: specimen access is now
cheap. That is what makes the method practical rather than
merely correct. (Linnaeus had the same structure and knew it:
a taxonomist at the centre, correspondents shipping specimens
from places he would never visit.)

**Known limits of that access — declare them, do not assume them
away:**
- It is TEXTUAL and secondhand. What practitioners know but
  never write down is systematically missing.
- It is strongest where the literature is thick (Roman law,
  FRBR, biological nomenclature) and *equally fluent* where the
  literature is thin — so confidence does not track accuracy.
- It reproduces the CONSENSUS view, including a field's
  conventional simplifications.
- Therefore the discipline, not the expertise, is what catches
  errors: "compiled" as a language property is the popular view
  and would have been reproduced, had the format not forced the
  question "property of WHICH subject?". An expert without the
  discipline restates the field's received lies fluently.
- Mitigation is not verification but honest marking
  (`certitudo`, `fons`, `corpus_institutionis`) plus a real
  structural property: **the taxonomy is more robust than the
  dictionary.** A wrong date in an `individuum` is local; the
  genus structure rests on many specimens agreeing, so
  individual errors do not propagate upward.

### What is cheap and what is expensive

- **Division decisions are cheap and LOCAL.** A genus can be
  renamed, re-parented, split or merged at any time; dangling
  references are caught by the gate, so the corpus tells you
  what you broke.
- **Metamodel decisions are expensive and GLOBAL.** Every
  mechanism must be retrofitted across all models.

This asymmetry is why dimensions are taken BEFORE nouns
(iudicium and valid-time were built ahead of easier material,
while the corpus was small enough to retrofit). It also means
the current phase is FILLING OUT, not perfecting: revision
passes are available indefinitely, and no division decision
made now can paint the work into a corner.

- **Values match declared names exactly.** `vita`→`vigens`,
  `certitudo`→`fides` (not `fide`). Machine-checkability beats
  grammatical flourish; this was caught by the validator on the
  hour the mechanism was born.
- **Naming is Latin throughout** — identifiers, values, prose.
- **`latina.h` collisions**: some genus names (`constans`,
  `structura`…) are C macros in this house. Harmless in STML;
  a hazard only if a future generator emits C identifiers from
  genus names. Flagged where it occurs.
- **Dictionary entries teach.** Residents are chosen because they
  carry a truth of the genus (rand breaks both purity axes;
  the kilogram was a thing and became a law), not for coverage.

---

## 10. Deliberately NOT in the format

Recording non-decisions is part of the discipline; each of these
was considered and refused with a reason.

- **No cardinality beyond `multiplex`.** "Exactly 3" has never
  been forced.
- **No inverse-relation generation.** `inversa` documents; it
  does not create edges.
- **No probability on `certitudo`.** The scholastic degrees are
  categorical on purpose; numeric probability lives in
  `iudicium.periculum`, where it belongs to the subject matter.
- **No genus for "validity."** Valid-time is a qualification of a
  claim, not a kind of thing (compare: `gradus_assensus` IS a
  genus because degrees of assent are things philosophers name
  and define).
- **No catch-all "pattern" genus.** Deferred as a bag risk;
  GoF-membership, if wanted, is a PUBLICATION relation to the
  1994 work — the biological-nomenclature precedent.

---

## 11. Open questions

- **Ranges.** Living properties are intervals (height 2–5 m,
  flowering May–June). Partly answered by `mensura.quantitas`
  (min/max); TIME ranges remain unanswered and point at `tempus`.
- **Inherited-machine misfit.** A periodical never reaches
  "perfecta". May a sub-genus override a parent's machine?
- **Computed recurrence.** Movable feasts (Easter) need a
  calculated rule; `recurrentia.periodus` only expresses simple
  periodicity.
- **Presentation vs type.** Are `area` and `annus` real value
  kinds, or `textus`-with-presentation and `dies`-with-precision?
  (Presentation and precision may belong to the field.)
- **Bitemporality.** We now have valid-time, and `iudicium.latum`
  gives assertion-time for judgments. Whether ordinary claims
  need assertion-time too is open — deliberately, until content
  forces it.
