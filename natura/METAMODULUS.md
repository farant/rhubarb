# METAMODULUS — the natura format specification

**Status:** descriptive, v1. Describes the format as the models
actually use it — the vocabulary below was extracted mechanically
from the corpus, not from memory (27 elements, 26 attributes).

**Live counts live in `natura/INDEX.md`**, which is regenerated on
every validator run. This document deliberately carries NO tallies
of models or genera: a specification that quotes a moving number
is stale the next afternoon.

**Audience:** anyone writing a model by hand, and whoever
implements the loader. For WHY the library exists, what it is
for, and how the work is done, see `natura/RATIO.md` — this file
is the specification only. The loader is expected to enforce the
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

**OPEN — necessity is BINARY here, and reality wants it
conditional** (aviation, 2026-08-05). An aircraft's Minimum
Equipment List is a regulated statement of *which parts may be
absent, under which conditions, for how long* — one weather radar
may be inoperative if the other works and the flight avoids
convective weather, for ten days. That is `necessaria` with a
predicate and a clock, and we can only say verum/falsum.
Recorded, not built: one forcing case is an observation. If a
second domain asks (medical device tolerances, N+1 redundancy),
it becomes a mechanism.

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

## 4b. Addressing — how an entity is named from outside

**An entity's address is `modulus` + `nomen`, flat.** That is
already true of every internal reference (`ad="X" modulus="Y"`),
and it is unambiguous: measured 2026-08-05, **381 names across
all modules, zero collisions**.

A path rendering follows directly and is the intended external
form:

```
/persona/dennis_ritchie
/lingua_programmandi/c
/identificatio/isbn
/commercium/tessera_negotiabilis
```

Three consequences worth stating, because none is obvious:

1. **Rank does not appear in the address.** Genera, species and
   individua share one namespace per module, so `/actus/ritus`
   (a genus) and `/actus/stipulatio` (a species) address alike.
   This is correct for a citation: a consumer refers to a THING,
   and whether it is a kind or an instance is a fact you learn
   on arrival, not something you must know to ask.
2. **Nesting is display, not address.** `rosa_canina` sits four
   levels deep under `planta`, but its address is
   `/planta/rosa_canina`, not the full lineage. The Porphyrian
   depth is taxonomy; the address is identity. (This also means
   re-parenting a genus — which the coining rules encourage —
   does NOT break addresses.)
3. **Presentation is not essence** (the same law suffixes and
   ISBN hyphens taught). `dennis_ritchie` vs
   `dennis-ritchie` is a rendering choice; the `nomen` is the
   identifier. A UI may hyphenate; a citation must not depend
   on it.

**Uniqueness is therefore load-bearing, not hygiene** — rule 15
is what makes addressing work, and it is now ENFORCED (§8).

### What to call a particular (decided 2026-08-05)

**In the artifacts: `individuum`. In English prose: "particular".**
Two registers, deliberately, because no single word serves both.

- **`individuum`** — Porphyry's own term, literally *undivided*:
  what cannot be divided into further instances of itself. It is
  what sits below the *species specialissima*. Precise,
  traditional, and Latin, so it stays as the element name. Its
  one flaw is in ENGLISH: the cognate "individual" connotes a
  person, which grates when you say "x86 is an individual".
- **"particular"** — the standard English metaphysical term
  (universals and particulars), with no person-connotation. Best
  word for prose and discussion. But it is a poor LATIN choice:
  in scholastic logic *particularis* means QUANTIFICATION (some
  vs all — a "particular proposition"), not individuality, so
  `particulare` would be ambiguous in Latin in a way "particular"
  is not in modern English.
- **`singulare`** — the other classic pair-member
  (universale/singulare; Aristotle's καθ' ἕκαστον). Defensible,
  and it would harmonise with Scotus's *haecceitas*, already
  cited in `elementa_programmatis.identificator`. Rejected only
  because it is a rename with no functional gain.
- **"entity"** — REJECTED. In ER modelling "entity" means entity-
  TYPE or entity-INSTANCE depending on the speaker: it imports
  precisely the confusion this section exists to remove.
- **"instance"** — REJECTED. Accurate in one sense (an individuum
  is an instance of its genus) but it imports OOP baggage, and
  this library's framing is expressly *the taxonomy layer, not
  the implementation layer*. Borrowing the implementation
  vocabulary would undercut the thesis.

The connotation gap is real and worth knowing: our individua fall
into person-like (Ritchie, ISO, Moby-Dick) and artefact-like
(c89, x86, euro, macOS). Both are particulars; only the first
sound like "individuals".

### The particular-marker (Fran, 2026-08-05)

A citation may mark that it points at a **particular thing**
rather than a kind:

```
/persona/:dennis_ritchie        an individuum — a THING
/actus/stipulatio               a species — a KIND
/actus/ritus                    a genus — also a KIND
```

**Only individua take the colon.** Genus and species are BOTH
kinds; the line between them is positional, not ontological
(Porphyry: everything mid-tree is a *subaltern genus* — genus
with respect to what is below, species with respect to what is
above; our own `rosa` is both). The line that does not move is
universal vs particular, and that is the one worth marking.

**The colon is MANDATORY in canonical citations and IGNORED in
resolution** (strengthened 2026-08-05). Three properties, chosen
together:

1. **Mandatory** → at-a-glance is GUARANTEED. An optional marker
   fails exactly here: encountering a bare name, you cannot tell
   whether it is a kind or a lazy author.
2. **Ignored in resolution** → a correction never yields a dead
   link. When `libc` becomes a genus, every `:libc` citation
   still resolves and is FLAGGED STALE — information a consumer
   wants, not breakage they must handle.
3. **Checked both ways by the gate** → a missing colon on a
   particular is an error, and a colon on a kind is an error.

REJECTED, and the reason is the project's own value: making the
marker LOAD-BEARING would turn every correction of a misfiling
into a breaking change — so the schema would quietly pressure us
to leave `c` filed as a species because fixing it is expensive.
A mechanism that penalises truth-correction fights the discipline
it exists to serve. (Thirteen such corrections happened on
2026-08-05 alone.)

Also rejected: using the marker to let a genus and a particular
SHARE a name (`/systema_operativum/unix` vs `:unix`). It would
have spared the `unix_bell_labs` rename — but it preserves an
ambiguity instead of resolving it. The rename made the
distinction visible in prose, in the index and in conversation;
a sigil makes it visible only where the sigil appears. Measured justification
from the day it was decided: re-filing thirteen entities from
species to individuum (see below) changed 343 arcs to 343 arcs
with zero breakage — had rank been load-bearing in addresses,
that correction would have broken every citation to C, Python,
macOS, x86 and nine others. As a hint, the same event merely
flags stale citations, which is information.

**COROLLARY — the marker creates NO namespace** (Fran,
2026-08-05). Because resolution ignores the colon, `/x/unix` and
`/x/:unix` are THE SAME NAME and therefore a collision, caught by
rule 15. You cannot use the marker to disambiguate a genus from a
particular; you must name them distinctly.

That is deliberate discipline, not a limitation. A namespace
split would let two things called "unix" coexist forever, visible
as distinct only where a sigil happens to appear — in prose, in
an index, in conversation they would still collide. Forcing the
rename (`systema_unixiforme` / `unix_bell_labs`) resolves the
ambiguity everywhere instead of hiding it in punctuation.
VERIFIED 2026-08-05: a genus named `macos` beside the individuum
`macos` is rejected as "bis definitum (allocutio ambigua)".

Marker not yet exercisable in citations: no external consumer
cites addresses. Enforceable at first citation; rendered in
`INDEX.md` today.

### Promotion — prefer a sibling genus (Fran, 2026-08-05)

When a thing acquires sub-things, do **not** promote the thing to
a genus. Coin a SIBLING genus for the sub-things and relate them.

The library already does this: C did not become a genus when
versions appeared — `versio_linguae` was coined, with
`c89 versio_de c`. Dialects would take the same shape
(`dialectus_linguae`, generic like `versio_linguae`, not a
per-language `dialectus_c`).

**The reason is that the relation is wrong for membership.**
"GNU C is a dialect of C" is DERIVATION; "Socrates is a man" is
PREDICATION. Promotion silently converts one into the other.

**Test before ever promoting: are the would-be members
PREDICATED of it, or merely RELATED to it?**
- related (version of, dialect of, implementation of, part of)
  → sibling genus + relation;
- genuinely predicated → it was a kind all along, and you are
  CORRECTING a misfiling, not upgrading anything.

Three cases converge on this shape: versions, implementations,
dialects. It also answers `elementa_programmatis.libc`'s open
dubium — glibc and musl IMPLEMENT libc, so the answer is
`implementatio_bibliothecae` (parallel to
`implementatio_linguae`), not promotion.

**Open question — rename stability.** The coining rules say
divisions are cheap and local, and this session renamed
`charta_negotiabilis` → `tessera` on exactly that licence. That
is only cheap INSIDE the library. Once app volumina cite
addresses, a rename breaks consumers — the ordinary API-stability
problem, and the reason Eskil's "get the interface right first"
applies here as much as to a C header. The library is currently
pre-publication, so renames remain free. When it is first cited
externally, one of two disciplines must start: names frozen once
published, or a forwarding mechanism (a tombstone entry naming
the successor, which `norma.substituit` and
`identificatio.successit` already suggest the shape of). NOT
built — no consumer exists yet, and inventing the mechanism
ahead of the content would break the Alexander rule.

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
| 15 | No two entities share a name within a model (addressing depends on it, §4b) | [E] |
| 16 | No `externum` flag on a target that is now described (a stale flag makes the agenda LIE) | [E] |
| 17 | The `versio` attribute matches the version stated in the file's header comment | [E] |
| 18 | A name bearing another name as its stem (`x_y` where `x` is a name) either descends from it or bears a declared relation to it | [M] |
| 19 | A bare name used in two models is a declared homonym: a `nota` on each side naming the other and saying why they are not the same kind, and every citation of it carries `modulus=` | [M] → [E] on first use |

Rules 9–13 are the Cyc-consistency defense: they are what makes a
hand-written library stay coherent past the point where one mind
holds it.

**Rule 18 is advisory [M]** — it reports MONITA, does not fail the
run, and does not change the exit code. It cannot be enforced,
because a shared stem is often legitimate: `fons_c` (a C source
file) and `fons` (the source of a proposition) are honest
homonyms, and `titulus_iuris` is a copyright notice while
`titulus` is a credential. What the rule buys is that every such
coincidence is SEEN ONCE. Its first run found that the three
`editio` individua declare no `editio_de`, so the FRBR chain —
this library's most-cited borrowing — was declared and unused at
the layer it was built for. A heuristic that knows nothing about
FRBR found that, which is the argument for keeping it noisy.

**Rule 19 is the domain escape hatch, and it is deliberately
DORMANT.** Rule 15 requires uniqueness *within* a model; across
models the same bare name is **already legal**, and
`/chemia/substantia` beside `/metaphysica/substantia` would
validate today. Nothing needs building for the capability — the
address `/modulus/nomen` is already `domain::name`.

What is written down here is that **global uniqueness is a
CONVENTION, not a law** (measured 2026-08-05: 493 names, zero
homonyms). A session that mistakes the convention for a rule
reaches for a suffix — `substantia_chemica`, `numerus_dea`,
`mandatum_iudicis`, `titulus_iuris` — which is what a flat
namespace makes people do, and which rule 18 then reports as a
smell. Roughly 5% of names are currently that workaround.

**When to open the door: homonymy only, never convenience.** The
same word for two genuinely different kinds — the live case in
this house is scholastic, not medical: `materia` is taken (bulk
stuff in `corporalia`, not *materia prima*), `actus` is taken (a
model of solemn acts, not act/potency), and `species`, `genus`,
`actio`, `fons`, `status` are reserved by §3/§4 besides. Model
hylomorphism properly and you hit this on the first line.

**Why the door stays stiff.** A domain qualifier lets you skip
the question *"is this already a kind we have?"* — and that
question is what the library is for. `interactio` exists only
because someone asked whether drug interaction and hazmat
segregation are the same kind; `classis_normativa` only because
drug schedules and the aviation gradation are; `periculum`,
written for a clinic, took freight insurance unchanged. Automatic
per-domain namespacing dissolves all of that. It is the Pieper
failure one level up again: not *"does this buy us anything"* but
*"I need not ask whether this is the same thing."*

So the declaration must carry the ARGUMENT — why this is not that
kind — and prose in a `nota` carries it until prose proves
insufficient. No new attribute is coined for a case that has
happened zero times.

**On first use, two gates get built** (recorded so they are not
rediscovered): undeclared homonymy becomes a VULNUS, and citing a
homonymous name *without* `modulus=` becomes a VULNUS. That
second one is the real safety property and does not exist today —
rule 2 catches a *dangling* reference but never a
**wrong-but-resolving** one, and 188 citations currently omit
`modulus=` and resolve to their local model. The gate binds only
homonymous names, so it costs nothing until the door is used.

*Rules 16 and 17 were enforced by the validator for a full round
before being written here.* The closed-vocabulary gate (rule 8)
keeps §3/§4 from rotting against the corpus; nothing was watching
§8 against the validator. Noted rather than fixed: the same
species of drift the mechanism was invented to stop, one level up.

---

## 9. Conventions

**Doctrine at a glance** — this section carries the rules for
WRITING the library, and it has grown. In one line each:

| rule | one-line form |
|---|---|
| When to make a genus | Ontological, not pragmatic: is it REAL? When uncertain, DIVIDE. |
| The addressability test | Anything with individua you'd address by ID deserves a genus. Relations don't get IDs — their relata do. |
| Prefer the specific | The generic parent carries no payload and is dangerous because it LOOKS like coverage. |
| Three modes of membership | per naturam (read off) / per iudicium (asserted) / per actum (conferred). |
| Naming as leverage | A genus imports an APPARATUS, so naming hands you the next questions. Vocabulary and jargon differ only in whether the thing is there. |
| Genera vs analogies | Cross-domain rhymes are ANALOGIES, not genera. Default home is a nota on both sides. |
| How far to enumerate | Coining is ontological (divide); enumeration is economic (DEFER). Incompleteness is honest; mis-structure is a lie. |
| The Linnaean escape hatch | When abstraction stalls, collect specimens. Precondition: specimen access. Aim it: who is punished for getting X wrong? |
| Promotion | Do not promote a thing to a kind — coin a SIBLING genus and relate. Test: predicated of, or merely related to? |
| Cheap vs expensive | Divisions are LOCAL; metamodel changes are GLOBAL. Hence dimensions before nouns. |
| Domains (rule 19) | Cross-model homonyms are ALREADY legal; global uniqueness is a convention. Open the door for genuine homonymy, never convenience — a domain qualifier lets you skip "is this a kind we already have?", which is the question the library exists to force. |

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

**Prefer the SPECIFIC genus** (Fran, 2026-08-05). Better a
correct genus for *ISBN* or *Brazilian telephone number* than a
pure generic one that will mislead you on the case actually in
front of you. The generic parent is true but carries no payload:
"identifier" does not tell you that an ISBN designates a
MANIFESTATION rather than a work, carries a check digit, and
migrated from ten digits to thirteen in 2007. "Telephone number"
does not tell you that Brazilian mobile numbers gained a ninth
digit.

And the generic genus is dangerous precisely because it **looks
like coverage**: you believe you have modelled the thing when you
have modelled only its shape. Under-division yields an organising
scheme with no payload — the failure that makes a taxonomy
academic.

**Institutional kinds are real kinds.** ISBN and a national
numbering plan are conventional, yet genuinely there, with real
membership conditions, real properties and real histories.
Moderate realism does not require NATURAL kinds — the library
already holds many instituted ones (norma, licentia, tributum,
moneta, portorium).

**THE ADDRESSABILITY TEST** (Fran, 2026-08-05) — the most
practical of the three, because it is empirical rather than a
judgement call:

> Anything that can have **individua you would want to address by
> ID unambiguously** is worth making a genus.

The load-bearing half is *can have individua* (ontological: does
this kind have particulars under it?). ID-assignment is the
**evidence**, not the justification — if a domain's practitioners
routinely give these things identifiers, that is strong evidence
the things are real enough to be picked out singly. Practice
tracks reality.

**It reproduces every refusal the library made on other grounds,
which is why it is trustworthy:**

| candidate | address an instance by ID? | verdict |
|---|---|---|
| incident | yes — INC-1234, "the March 3rd outage" | genus |
| merx (merchandise) | no — you address the LOAF; being-for-sale gets no ID | not a genus (round XIII) |
| triage | no | not a genus (round XVII) |
| backlog priority | no | not a genus (round XXXII) |

And it explains WHY those three failed: all are relations or
orderings, and **relations do not get IDs — their relata do.**

**But a relation CAN earn one.** A marriage has a certificate
number; an employment has a contract number. When a relation
starts being addressed, it has been reified into an entity — the
same move `commercium.pretium` made against valid-time
attributes. So the deep form of the test is: *has this been
reified in practice?*

**Refinement:** read "individua" as *members worth picking out*.
Some genera have addressable SPECIES and no individua at all —
`iudicium.gradus_assensus` (you address `/iudicium/scientia`, but
never "an individual instance of fides"), `formae_datorum`. They
pass the test at species level, and that is enough.

**Linnaean parallel:** the knowledge lives at the species level.
Higher ranks organise; lower ranks carry the payload. A field
guide that only said "plant" would be useless, and a taxonomy
that stops at "identifier" is the same guide.

### Three modes of membership (Fran, 2026-08-05)

How a thing comes to belong to a genus is not one story but
three, and the library runs all three without having named them
until now.

| mode | membership is… | examples |
|---|---|---|
| **per naturam** | READ OFF the thing | a rock is a rock; `planta`, `animal`, `corporalia` |
| **per iudicium** | ASSERTED by someone competent, revisably | `morbus` is diagnosed; `vitium` is judged; `incidens` is declared; `debitum_technicum` is labelled |
| **per actum** | CONFERRED by a solemn act | `titulus`, `officium`, `licentia`, `matrimonium`, `persona_iuridica` — the whole `actus` model |

**Per-iudicium genera are not weaker.** They are about a third of
the library, and `fabricatio` already recorded the consequence
before the mode was named: *"not a bug but a feature"* is a
dispute about GENUS ASSIGNMENT — identical behaviour, contested
kind, resolvable only by consulting intention.

**And they are where an expert vocabulary earns its keep**
(Fran's point, and it is the strongest argument for the library's
practical value). Much expertise IS a repertoire of applicable
concepts: a novice sees code that is annoying to change, someone
with the vocabulary sees *debt* — and the word immediately
licenses moves (estimate the principal, weigh the interest,
decide to service or repay). The library supplies the vocabulary
that makes the judgement possible.

This does not weaken the Pieper discipline. An applied concept
must still be REAL — the expert recognises a genuine pattern
rather than inventing one. The guard is unchanged: is the thing
there? What changes is only how membership is established.

**Why per iudicium is the PURE case of naming-as-leverage**
(Fran, 2026-08-05). It is the only mode where naming changes
nothing about the world and everything about your powers. Per
naturam the name adds description, but the rock was already a
rock. Per actum the naming is efficacious — but only because an
institution stands behind it, so you need AUTHORITY. Per iudicium
needs no authority, only COMPETENCE, and afterwards you can do
what you could not do before. The naming effect is isolated
there, with nothing else mixed in.

**The mechanism, in this format specifically:** a genus is not a
label — it carries `machina_statuum`, `proprietates`,
`relationes`, `actiones`. Applying one imports an APPARATUS. Judge
"this is debt" and you inherit a state machine (susceptum /
servitum / solutum?), properties (what is the `usura`?) and
relations (`emit` — what capability did it buy?). **Naming hands
you the next questions**, which is what expertise looks like from
outside: the expert does not know more facts, they know what to
ask next, because the concept came with its interrogatives
attached.

**Lineage, and why this differs from it:** Minsky's frames,
Schank's scripts, the expert systems of the 1980s. Those largely
failed to scale, and the diagnosis is available — THEIR FRAMES
WERE TASK-SHAPED, OURS ARE PIEPER-SHAPED. A frame for "restaurant
visit" does not compose with one for "hospital admission",
because each encodes a use case. Frames built for what things ARE
compose, because reality does. Measured: `periculum`, written for
a clinic, was exercised unchanged by freight insurance one round
later.

**The failure mode, stated because leverage has a direction:**
misapplied vocabulary imports the WRONG apparatus. Call a genuine
design error "technical debt" and you inherit moves that say
*manage* and *service* what you should simply FIX — and it
misleads competently, which is worse than confusion.

**Hence: vocabulary and jargon are both applied; the difference
is whether the thing is really there.** Jargon imports
affordances without warrant, and that is the actual subject of
Pieper's book. The realism constraint is therefore not
philosophical decoration beside the practical value — it is what
keeps the practical value from turning into its counterfeit.

**Practical consequence for modelling:** a per-iudicium genus
should carry `asseritur_per → iudicium`, so that membership has
an author, a moment and a degree of assent, and two experts may
disagree without either being in error — which also makes
RECLASSIFICATION a legitimate move rather than an admission of
failure.

**Two halves of the library's value, now both stated:** per
naturam material saves a consumer from getting STRUCTURE wrong
(the distilled expensive surprises — ISBN, recurrence, valid
time); per iudicium material gives them LEVERAGE THEY LACKED.
Different mechanisms, both real.

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

### How far to enumerate (Fran, 2026-08-05)

Coining and enumeration are **different questions**, and the
uncertainty rule REVERSES between them.

- **Coining is ontological**: is this a real kind? When
  uncertain, DIVIDE.
- **Enumeration is economic**: which of the real kinds do we
  write down first? When uncertain, DEFER.

Not a contradiction. A missing *distinction* corrupts what you
have already written — things get misfiled under a kind that is
not theirs. A missing *instance* leaves everything written still
correct, merely incomplete. **Incompleteness is honest;
mis-structure is a lie.**

So: **Pareto plus judgement.** Do not chase the long tail.
Programming languages and identification schemes could each be
subdivided into hundreds; a well-chosen few dozen will meet
80–90% of what any application encounters. Anchor measured
2026-08-05: Fran's own working list of programming languages
runs to **38** — roughly 5% of the population of named languages,
and it covers the paradigm space, the historical spine and the
practical working set.

**The inclusion test is EXPOSURE, not importance:** is it almost
inevitable that some application will have to deal with this?
ISO 8601, E.164, ISBN, IBAN, UUID pass. A VIN probably does not,
yet.

Conservatism is safe here *because the escape hatch exists*: when
someone meets an obscure case, they collect that specimen and add
the sub-genus, and the structure already has a place for it. The
fix is always cheap and local.

**Two reasons a dictionary entry earns its place — neither
subsumes the other:**
1. It TEACHES (rand breaks both purity axes; the kilogram was a
   thing and became a law). Chosen for what it reveals.
2. It COVERS (you will meet this). Chosen for frequency.
The best entries do both — ISBN teaches the work/manifestation
distinction *and* is unavoidable.

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
