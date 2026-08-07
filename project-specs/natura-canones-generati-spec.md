# Natura → Canon: generated tag libraries

**Status:** design, approved in outline 2026-08-06
**Pilot:** `planta`
**Depends on:** natura loader (shipped), canon (shipped), canon_coquere (shipped)

---

## 1. The idea

Every genus in natura is projected into an **element** in a generated
canon, so that describing a kind once yields the tag library, the
schema, and — by feeding the result to `canon_coquere` — the typed C
reader, with no hand-written schema at any step.

The chain closes:

```
natura/planta.genera          (hand-written: what a plant IS)
    ↓  natura_canones
natura/cocta/planta.canon     (generated: what a plant RECORD may say)
    ↓  canon_coquere
include/planta_lectio.h       (generated: PlantaRosaCanina struct)
lib/planta_lectio.c
```

This is not a new charter. RATIO §3 lists four modes of use, and mode 2
is *"Generate from it — tables, structs, forms. **Needs the loader**."*
The loader shipped this week; this collects on a promise that has been
sitting in the document since it was written.

## 2. Why it is small

The expectation going in was a large canon expansion. Working the
mapping through, canon needs **almost nothing new**, because it grew the
required mechanisms during the canon wave of 2026-08-06:

| needed | mechanism | shipped |
|---|---|---|
| closed value sets | `genus="electio"` + `<optio>` | yes |
| stated defaults | `ordinarius=` | yes, this week |
| context-scoped element definitions | `elementum intra=` | yes (forced by aedilis) |
| in-document references | `citatio` + `intra=` | yes, this week |
| uniqueness of names | `unicitas` | yes |

The only genuinely new thing is a **monitum** (advisory) severity tier,
and it is deferred — see §7.

## 3. Doctrine

These are the load-bearing decisions. Everything else follows.

### 3.1 Residence: kinds here, individuals elsewhere

RATIO §3: *"the library describes kinds; your individuals live in your
volumina and cite it."* A generated canon governs documents that live
**outside** natura. Natura's own dictionary (`:clang`, `:linux`,
`:carl_linnaeus`) stays where it is; it is reference data, not app data.

### 3.2 Rank is relative; addressability decides element-hood

Genus, species, individuum and cultivar differ in name only — a species
of one genus is the genus of what sits below it, and natura's own
vocabulary confirms this by letting `<genus>` nest inside `<species>`
and `<individuum>`. So all four kind-bearing elements project alike.

The criterion for becoming an element is **addressability**: anything
you would ever want to give an identity to. That includes parts.

A document may instantiate at **any level of precision it actually
has** — `<planta>` when all you know is that it is a plant,
`<granny_smith>` when you know exactly.

### 3.3 Pure projection

Generation is deterministic and total. Generated canons are never
hand-edited; they carry the `GENERATUM` header with `NOLI MANU MUTARE`
so that `generata-custos` denies edits to them.

**Trap, measured:** `generata-custos` greps only `head -3`, and matches
exactly two phrases (`NE MANU EDITES|NOLI MANU MUTARE`) — a synonym is
silently unprotected, which has already bitten us once. A generated
canon opens with an XML declaration and a comment banner, so the signum
must be placed deliberately within the first three lines or the file
looks protected and is not. The birth test is to attempt an edit and
confirm it is denied.

Rejected alternatives:

- **Per-module override fragments.** Two sources of truth, and the
  staleness problem we spent a day building gates against.
- **Serialization hints inside `.genera` files.** Violates the Pieper
  guard (RATIO §2): describe things as they are, not as applications
  want them.

**The resolving rule:** if the generator needs a distinction natura does
not carry, that is evidence natura may be missing a *real* distinction.
It is added to natura only if it is ontologically real (as `multiplex`
and `necessaria` already are). Otherwise the generator applies a fixed
rule and exposes no knob. §8 records two distinctions this rule surfaced
on day one.

### 3.4 The canon LIMITS, it does not DEMAND

Everything generated is permissive. The canon says what you may not
claim **falsely**; it never says what you must claim.

This is the load-bearing consequence: **ontological necessity is not
documentary obligation.** natura saying a plant necessarily has a root
(`pars necessaria="verum"`) does *not* mean a record must enumerate one.
Mapping necessity to `minimum="1"` would make partial knowledge illegal,
and "I saw a plant, I do not know the kind" is a thing people need to
write down.

Precedent: forum's runtime genera already decided that a definition
**judges rather than constrains**.

So: everything is `minimum="0"`. Start lenient; tighten only on
evidence.

### 3.5 Relations are the loader's business, not the schema's

`<relatio nomen="auctor_nominis" ad="persona" modulus="persona"/>`
crosses files. That is layer 3, which `natura.canon` explicitly
disclaims (*"resolutio citationum trans exemplaria (regulae II-V) ...
Ea onerator tenet et tenebit"*).

A generated canon therefore checks only that the reference **is a
well-formed name**, and a natura-aware pass confirms the target exists.
The monolith (§5.2) is the exception: within one document, references
can become genuine `citatio` and get checked properly.

## 4. The mapping

| natura declares | instance dialect gets | canon mechanism |
|---|---|---|
| `genus` / `species` / `individuum` / `cultivar` `nomen="X"` | element `<X>` | `elementum` |
| `sub=` chain (crosses modules) | apparatus flattened down the chain | flatten at generation |
| — | optional `nomen=` identity on every kind | `attributum genus="nomen"` |
| `proprietas nomen="P" genus="G"` | attribute `P` of type `G` | `attributum` |
| `proprietas genus="electio"` + `<optio>` | attribute with closed value set | `genus="electio"` |
| `proprietas multiplex="verum"` | repeatable child `<P>`, scoped | `liberum` + `elementum intra="X"` |
| `pars nomen="P"` | optional child `<P>`, scoped | `liberum maximum="1"` + `intra="X"` |
| `machina_statuum nomen="M"` (see §4.1) | attribute `status_M` | `genus="electio"` over its `<status>` names |
| `actiones` | constrains `<eventum actio="…">` | `genus="electio"` |
| `relatio nomen="R" ad="T"` | reference attribute `R` | `genus="nomen"` (§3.5) |
| `relatio multiplex="verum"` | repeatable child `<R ad="…"/>` | `liberum` |
| `valor nomen="P"` at any level | stated default (see §8.2) | `ordinarius=` |
| `historia` / `eventum` | reused verbatim | same vocabulary as natura |

Canon has no inheritance and does not need any: flattening at generation
is correct, because a generated file is a projection and projections are
allowed to be redundant.

### 4.1 Only `gerens="individuum"` machines reach instances

A `machina_statuum` with `gerens="species"` describes states of the
**kind**, not of the individual, and must not become an instance
attribute. Only `gerens="individuum"` machines project. See §8.1 — this
rule cannot be applied reliably today.

### 4.2 Scoping, and the collisions it solves

Parts and repeatable properties are declared `intra="<kind>"`. This is
necessary, not decorative — measured against the corpus:

- **part name = genus name:** `directorium`, `eventum`, `proiectio`
- **property name = genus name:** `ambitus`, `codex`, `consilium`,
  `fundamentum`, `index`, `materia`, `titulus`

`canon_coquere` already emits qualified C type names for `intra=`
definitions, so this costs nothing downstream.

### 4.3 Names are kebab-case, and are produced mechanically

natura's `nomen` datatype is `[a-zA-Z0-9_*]` — snake_case, no hyphens.
Generated element and attribute names are **kebab-case**:
`rosa_canina` → `<rosa-canina>`, `altitudo_matura` → `altitudo-matura`.

Because natura names can never contain a hyphen, the transform is
**bijective** and loses nothing.

Verified to cost nothing at either hop:

- **canon** already types `elementum/@nomen`, `attributum/@nomen` and
  `liberum/@nomen` as `compositum` — *nomen + lineola*, the kebab
  datatype grammatica forced into existence. No canon change.
- **canon_coquere** already treats `-` as a segment separator in all
  three of its identifier transforms, so `rosa-canina` yields type
  `PlantaRosaCanina`, field `rosa_canina`, enum `PLANTA_ROSA_CANINA`.
  No generator change.

Two mechanical rules follow, both instances of *mechanical beats elegant
for generated names*:

- **No declension.** A machine named `vita` yields `status-vita=`, not
  `status-vitae`. The generator cannot decline Latin, and a table of
  irregulars is exactly the kind of thing that rots.
- **No pluralisation.** Repeatable children take the singular name from
  natura unchanged.

### 4.4 Names are kebab; values referencing natura stay verbatim

Kebab-casing applies to **names the generator invents** — element names
and attribute names. It does **not** apply to attribute *values* that
reference natura, which are foreign keys: relation targets
(`auctor-nominis="carl_linnaeus"`), `actio=` names, and `electio`
options keep natura's own spelling.

The mixed look is deliberate. The name is ours and the value is
natura's, so the natura-aware pass of §3.5 resolves references by plain
string comparison with no inverse transform in the path — and a
reference in a document still greps against the `.genera` files that
define it.

## 5. Output

### 5.1 Per-module canons

`natura/cocta/<modulus>.canon` — one per module. Small, focused, and
each yields its own C reader. Relations are name-checked only (§3.5).

Note the generator must load **all** modules regardless of output scope,
because `sub=` crosses module boundaries (`planta sub="vivens"
modulus="corporalia"`). This mirrors `natura_examen`, which already
loads every model even when judging one file.

### 5.2 The monolith

`natura/cocta/individua.canon` — every kind in the library, ~560
elements. A document may mix freely (a plant, the person who named it,
the work the name was published in), and cross-module relations can
become real in-document `citatio` rather than bare names.

Both modes come from one mapping; they differ only in the element set.

### 5.3 Document shape

Root is `<individua>`, holding any number of kind elements.

```xml
<individua>
  <rosa-canina nomen="rosa ad murum"
               habitus="frutex"
               status-vita="florens"
               auctor-nominis="carl_linnaeus">
    <radix nota="muro proxima"/>
    <historia>
      <eventum quando="2024-04" actio="plantari"/>
    </historia>
  </rosa-canina>
</individua>
```

Note the two casings doing different jobs (§4.4): `rosa-canina`,
`status-vita` and `auctor-nominis` are generated names; `carl_linnaeus`
is a natura key, spelled as natura spells it.

A generated `unicitas` over `nomen=` across all kind elements makes the
identity in §3.2 real.

### 5.4 Registry dispatch is by extension, and MUST NOT use a root key

`canones.registrum` keys by extension **and** root element, and **root
wins** — its own header says so: *"RADIX VINCIT — '.stml' quattuor
dialectos fert, ergo extensio illis nihil dicit."*

Both modes root at `<individua>`. Registering a root key for
`<individua>` would therefore bind **every** instance file to whichever
canon claimed it, silently. The generator must never emit one.

Dispatch:

```
.individua	natura/cocta/individua.canon
.planta	natura/cocta/planta.canon
```

Whether the remaining 32 per-module extensions are worth registering is
an open question (§10).

### 5.5 Freshness

`tools/natura_canones.sh -probare` mirrors `canon_coquere.sh -probare`:
regenerate to a temp location, diff the **output** (never mtimes — a
comment-only edit to a `.genera` file must not register as staleness),
report `RANCIDUS` on divergence.

The existing `.genera` PostToolUse hook gains this check, so editing a
model tells you immediately that its canon is stale.

## 6. Verification

- **Pilot on `planta`** — richest apparatus in the corpus: essential and
  optional parts, a *cyclic* state machine, fixed values at two
  taxonomic levels, cross-module relations to `persona`,
  `opus_scriptum` and `animal`, and dictionary depth down to a cultivar.
  If the mapping survives planta it survives most of the corpus.
- **Feed the result to `canon_coquere`** and read a real document with
  the generated C reader. This proves the whole chain, not just the
  first hop.
- **Planted fault, semantic not cosmetic.** The staleness gate's birth
  test must be a structural change to a `.genera` file (a new property),
  not a comment edit — a comment edit correctly does *not* trip a
  semantic gate, and mistaking that for a working gate has burned us.
- **Differential against the corpus.** Generate all 33, judge them with
  `bin/canon_examen` (each generated canon is itself judged against
  `canon.canon`), and confirm zero vitia before trusting any of them.

## 7. Deferred, with trigger

**The `monitum` tier.** Essential parts (`necessaria="verum"`) are
unchecked in the first pass. When we want "this record is missing a
root" surfaced *without* it being an error, canon gains an advisory
severity beside `vitium`.

The shape is already known: natura's regula XVIII is explicitly
`ADVISORIA, portam non frangit` and surfaces as the `monita` section of
INDEX.md. The trigger is the first time absence needs to be visible
without being fatal — which is RATIO §3 mode 4, *"its sharpest
affordance is detecting what is MISSING."*

## 8. What the generator found in natura

The §3.3 rule paid on day one. Both of these are natura's to fix, not
the generator's to work around.

### 8.1 `gerens=` has no stated default

`natura.canon` declares `gerens` as an optional `electio` of
`species | individuum` with **no `ordinarius=`**. Since §4.1 makes the
instance projection depend entirely on this attribute, the generator
must guess for every machine that omits it.

**Fix:** state the default in `natura.canon` (one line). Which default
is correct is a natura question, not a generator question.

### 8.2 `valor` means two different things

- `rosa` carrying `<valor nomen="habitus">frutex</valor>` **constrains**
  — a rose that is a tree is not a rose.
- `rosa_canina` carrying `<valor nomen="altitudo_matura">3</valor>`
  merely **typifies** — a particular bush may well be two metres.

Same element, two readings. Mapping both to `ordinarius=` is safe under
either, because a stated default never rejects, so the first pass does
not have to resolve it. But natura should carry a `dubium` recording
that the distinction exists.

## 9. The `::` escape hatch — measured, not built

Should two modules ever want the same genus name, tags can be qualified
as `<tempus::momentum>`. This is an **escape hatch**, not a general
provenance scheme: genus names are currently unique across all 33
modules (177 declarations, 177 distinct names), so qualifying today
would qualify nothing.

Measured 2026-08-06 with a live inline-canon test:

- **STML already parses it.** `_est_nomen_initium` accepts `:`, and
  `_est_nomen_character` accepts `:`, `.` and `-` — almost certainly
  inherited from XML namespace syntax. `<tempus::momentum/>` and
  `<planta::rosa_canina/>` both parsed as elements with no complaint.
- **Canon already matches it.** An inline canon declaring
  `<elementum nomen="tempus::momentum">` judged the corresponding
  element as licit.
- **Exactly one thing blocks it.** `canon.canon` types `elementum/@nomen`
  as `compositum` (nomen + hyphen), which has no colon, so the *canon
  declaring* the qualified name fails self-judgment. Widening that type —
  or adding a `qualificatum` genus used only in element-name positions —
  unlocks the whole feature.

**Trigger:** the first genus-name collision across modules. When it
comes, this is a generator change plus one datatype, not a language
change.

## 10. Open questions

1. **Per-module extensions.** Registering all 33 in `canones.registrum`
   means a generated block with markers (the METAMODULUS §3 pattern).
   Registering only the monolith plus the pilot is simpler. Deciding
   this needs to know whether per-module instance files get written in
   practice at all — the monolith may simply be the useful artifact.
2. **`canon_examen` has no explicit `-canon` flag.** Without one, a
   canon that is not in the registry cannot be used to judge a file.
   Either add the flag or accept registry-only dispatch.
3. **Whether kinds may nest inside kinds.** The first pass allows kinds
   as children of `<individua>` only; a kind's children are its parts,
   `historia`, and multiplex properties. Whether "the tree in this
   garden" should nest structurally, or be expressed by relation, is a
   real modelling question and is not answered here.
4. **`canon_coquere` on ~560 elements.** The monolith's generated C
   reader will be large. Whether that is acceptable or wants splitting
   is unknown until measured — the pilot answers it for one module and
   the number can be extrapolated.

## 11. Incidental finding

During the `::` test, an unknown element with no close match rendered
its suggestion as a bare `-`:

```
<planta::rosa_canina> elementum extra canonem: - [infixus]
```

Cosmetic, in the "an X?" suggestion path added 2026-08-06. Worth a
glance when next in `lib/canon.c`.
