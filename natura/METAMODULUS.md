# METAMODULUS — the natura format specification

**Status:** descriptive, v1, 2026-08-04. Describes the format as
the 25 models actually use it (vocabulary below was extracted
mechanically from the corpus, not from memory).

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

Counts are live usage in the 25-model corpus at time of writing.

### Structural

| Element | Where | Attributes | Content |
|---|---|---|---|
| `natura` (25) | root | `modulus` `versio` `lingua` | genera, optional `fontes` |
| `genus` (106) | in `natura`, or nested in a `genus`/`species` for taxonomic depth | `nomen`, opt `sub` `modulus` `gradus` | everything below |
| `species` (111) | in `genus` | `nomen`, opt `gradus` `etiam` | definitio, differentia, valor, relatum, historia, nota, dubium, nested `genus` |
| `individuum` (83) | in `genus` | `nomen` | same as species |
| `cultivar` (1) | in `species` | `nomen` | same — a rank below species (`planta` only) |

`species` vs `individuum` is the format's central distinction: a
species is a KIND (C, pneumonia, the rose), an individuum is a
THING (Ritchie, c89, the kilogram).

### Descriptive

| Element | Count | Purpose |
|---|---|---|
| `definitio` | 169 | dictionary prose: what this is |
| `differentia` | 48 | what distinguishes it from its genus |
| `nota` | 85 | commentary that is not definitional |
| `dubium` | 8 | an unresolved tension, carried deliberately |

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
was case III. That is why 25 models survived nearly lie-free.

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
