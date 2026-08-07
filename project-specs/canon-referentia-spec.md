# Canon referentia — the `#` mark for individuum names

**Status:** approved design, 2026-08-07 (Fran + Claude, in-session brainstorm)
**Scope:** canon value-grammar + machina, natura_canones generator, canon_coquere,
document corpus (hortus.planta), tests. **Documents only** — the `.genera`
dictionary keeps bare references (ruling below).

## 0. Terminology guard

The `#` character is the **signum individui** (or just *signum*). It is NOT a
"sigillum" — in this house *sigillum* means a content seal/hash (sententiae
anchors, tabularium ancorae). Do not use *sigillum* for the `#` anywhere in
code, canons, or diagnostics.

## 1. Problem

A relation value in a document (`pollinatur-a="..."`, `auctor-nominis="..."`,
`<generat ad="..."/>`) can mean two different things:

- a **kind** — "pollinated by bees" (`apis`, a category), or
- an **individuum** — "pollinated by that bee" (`apis-prima`, a proper name).

Nothing in the current grammar distinguishes them. The de-facto discriminator
was orthography (snake = natura dictionary key, kebab = document identity) — a
style convention from hortus.planta's header that was never a mechanism: the
machina ignores spelling, and `compositum ⊇ nomen` means snake-shaped document
identities are legal, so the signal is unreliable. Measured consequence
(2026-08-07): hortus.planta is VALID under planta.canon and INVALID under the
monolith (one vitium: `auctor-nominis="carl_linnaeus"` unresolved) — the
restriction property broken by accident, not scope.

Additionally, kind-level references in documents ("bee-pollinated, no
particular bee") are currently inexpressible: citation would hunt for an
individuum named `apis` and reject a true statement.

## 2. The design in one paragraph

Attribute values partition into THREE marked spaces. Individuum proper names
carry a leading `#` **everywhere they appear** — declaration and reference
alike (`nomen="#rosa-ad-murum"`, `pollinatur-a="#apis-prima"`). Kind
references carry a leading `.` (`pollinatur-a=".apis"` — "bee-pollinated, no
particular bee"), in canon vocabulary (kebab, the same names used as tags).
**Bare values are always literals** — text, numbers, dates, electio tokens —
so `attr="foo"` is unambiguously data at a glance. The sigils dispatch two
dumb checks: `#`-values resolve against document keys (citatio, verbatim
match — keys are sigiled too, so no stripping on that branch); `.`-values
must name a kind in the citation's existing `ad=` closure list (vocabulary
check, one char stripped for title comparison, needs no keys, works
identically per-module and monolith). An UNSIGILED value in a reference
attribute is a TYPE error (`valor_congruit`), which means "references
announce themselves" is enforced on ALL relation sites — including the
open-target and multiplex ones citation cannot reach. Canon learns no
expression language.

Precedents: the CSS selector sigils EXACTLY — `#id` = identity, `.class` =
kind-membership, bare = literal/type — same marks, same semantics; HTML
`id`/`href="#x"`; house `##` nids ("hash = identity" is existing house
vocabulary). Philosophically: `#` marks rigid designation (a proper name),
`.` marks classification (a predicate) — markup's capital letter and its
common noun. Known overload, accepted with eyes open: `.planta` also reads
as a file extension (canones.registrum uses that very string as one), but
the two live in different value spaces, as CSS classes have cohabited with
extensions for thirty years.

## 3. Rulings (made in-session, 2026-08-07)

1. **Enrollment (census doctrine)** — resolves quaestio 01KZECZ2CZ.
   `#`-references resolve ONLY against in-document keys. A document that
   invokes a dictionary individuum enrolls it:
   `<carl-linnaeus nomen="#carl_linnaeus"/>` seats Linnaeus;
   `auctor-nominis="#carl_linnaeus"` finds him. One namespace; citation stays
   in-document and dumb. (An individuum is a singleton kind — instantiating
   its element presents the individual itself, it does not create "a
   carl-linnaeus".)
2. **Documents only.** The `.genera` dictionary keeps bare references: its
   names are corpus-unique and loader-checked, so it has no ambiguity to
   solve. **Named future:** Fran expects to extend the signum to `.genera`
   eventually — record intent, do not build.
3. **Signum in as many contexts as possible** (human readability affordance).
   Hence mandatory `#` on identity declarations, not declare-bare: every
   occurrence of a proper name is visually marked, and keys/references match
   verbatim. Any future format that writes an individuum identifier carries
   the signum with it (doctrine; out of scope here).
4. **Full trichotomy** (Fran, same session): kind references carry `.`,
   so bare values are ALWAYS literals. Rationale: `attr="foo"` should not
   require consulting the canon to know whether foo is data or a reference.
   Consequence: electio tokens, text, numbers, dates stay bare (they are
   literal-space); `.x` resolves in taxonomy kind-space only — things that
   exist as tags. If natura later promotes an electio token to a genus, the
   spelling change in documents is deliberate and visible.

## 4. Grammar — two new value-genera

Added to canon's genus electio (canon.canon + `genus_legere` +
`valor_congruit` in lib/canon.c):

- **`identitas`** — mandatory `#` followed by a non-empty `compositum` body.
  For identity attributes (`nomen=`). A bare identity is a VITIUM: the type
  system itself enforces that baptism is always marked.
- **`referentia`** — either `#` + non-empty `compositum` body (instance
  reference) or `.` + non-empty `compositum` body (kind reference). A BARE
  value is a VITIUM (`CANON_VALOR_MALUS`, existing machinery) — references
  always announce themselves. For all relation-origin attributes, including
  multiplex `ad=` (e.g. `generat`), in BOTH per-module canons and the
  monolith.

The sigil-bearing genera are EXACTLY these two; every other genus (textus,
numerus, dies, veritas, electio, nomen, compositum) is literal-space and
never sigiled. Element text content (`textus=` elements) is prose, not
value-space — `#` and `.` appear there freely with no meaning.

Notes:
- `referentia` everywhere kills the class-1 typing divergence
  (`nomen` per-module vs `compositum` monolith) as a side effect: both canons
  now agree.
- `certitudo=` and `fons=` stay `genus="nomen"` — they cite natura's keys
  (dictionary-side), never take the signum. (When ruling 2's named future
  lands, revisit.)
- Kind references (`.x`) are KEBAB (canon vocabulary = element titles), not
  snake. The duae-orthographiae convention (hortus header par.) is RETIRED:
  spelling is style, the sigils are the mechanism.
- A lone sigil (`#` or `.` with empty body) is a vitium under both genera.

## 5. Machina — citatio's signum dispatch (lib/canon.c)

Pass I (key collection) unchanged: collect `nomen=` values of elements whose
title is in `ad_elementa`. Keys now all begin with `#` (identitas), which
makes key-space and kind-vocabulary disjoint by construction.

Pass II, per citing value:
- value begins with `#` → look up in keys **verbatim** (no stripping). Miss =
  existing `CANON_CITATIO_IRRITA`.
- value begins with `.` → strip the one sigil character, compare against the
  `ad_elementa` titles (tags cannot carry sigils, so the list stays bare).
  Miss = NEW vitium **`CANON_VOCABULUM_IGNOTUM`** ("vocabulum extra petitum
  citationis"), whose detail uses the existing `_suggestio` machinery to
  offer the nearest kind title.
- bare value → LEGACY verbatim key lookup (today's behavior). Discovered
  during planning: `natura/natura.canon` itself carries citations over bare
  values, and by ruling 2 the `.genera` dialect KEEPS bare references — so
  the bare branch must stay alive for pre-signum canons. In
  referentia-typed canons a healthy document never produces a bare citing
  value (typing already flagged it); a faulty one may collect VALOR_MALUS
  plus a legacy key-miss — noisy but honest, and the typing vitium names
  the real fault first.

Uncited relations (open-target `ad='*'` 438 sites, multiplex 587,
cross-module targets in per-module canons) get typing only — but typing now
carries real weight: the value must be a well-formed `#`- or `.`-reference
even where nothing resolves it. Honest limit on resolution, stated in the
monolith praefatio.

## 6. Generator (tools/natura_canones.c + _emissio.c)

- Identity attribute emitted `genus="identitas"` (was `compositum`), all
  canons.
- Relation-origin members emitted `genus="referentia"` (was `nomen`
  per-module / `compositum` monolith), all canons. `certitudo=`/`fons=`
  excluded as today.
- **Per-module canons now emit citations** wherever the target closure lies
  entirely within the module (the document can hold those keys and enroll
  those figures). Monolith: all resolvable targets, as today. This is the
  class-1b convergence: everything a per-module document CAN express is
  checked identically under both canons; the only remaining divergence is
  cross-module instance references (inherently unresolvable per-module —
  class-D scope, librarium's future).
- Praefatio text updated: describes signum dispatch, per-module citation
  criterion, and keeps live numbers (never fixed counts in prose).

## 7. Downstream

- **canon_coquere**: `identitas`/`referentia` map to `chorda*`, value
  VERBATIM (signum included). Stripping is the consumer's affair in V1 — a
  shared helper can come on pull. Regenerate readers; `-probare` gates.
- **canon.canon**: two new optiones in the genus electio + notae carrying the
  grammar (incl. the sigillum/signum terminology guard).
- **Hooks**: no changes — canon-custos already judges registered document
  extensions; the new vitium flows through it.

## 8. Migration (corpus = one document + test fixtures)

- `natura/exempla/hortus.planta`: all `nomen=` values gain `#`
  (`#rosa-ad-murum`, `#malus-prima`); `auctor-nominis="#carl_linnaeus"`.
  Header's duae-orthographiae paragraph rewritten to describe the signum.
  Under planta.canon the auctor reference is typed-but-unchecked
  (cross-module target — honest class-D). NOTE: hortus cannot enroll
  Linnaeus (a `.planta` root admits only planta-module elements) — that is
  the class-D boundary made visible, not a defect.
- Test documents (probatio_natura_canones DOC_* pairs, probatio_canon
  fixtures): identities and instance references gain `#`, kind references
  gain `.`.

## 9. Testing (pre-fix-oracle discipline throughout)

1. **Typing pairs**: `identitas` rejects bare / accepts `#x`; `referentia`
   accepts `#x` and `.x`, rejects BARE, rejects lone `#` and lone `.`.
   Bare-`nomen=` and bare-referentia vitium tests fail on old code (both
   were legal) — true oracles.
2. **Vocabulary pair**: `.apis` (kind in closure) passes; `.piscis-volans`
   (unknown kind) gets `CANON_VOCABULUM_IGNOTUM` with suggestion.
   Fails-before/passes-after.
3. **Signum citation pair**: `#`-reference resolving (enrolled) passes;
   `#`-reference dangling gets `CITATIO_IRRITA`. (Extends today's closure
   pair.)
4. **Kind-level statement**: `pollinatur-a=".apis"` VALID — inexpressible
   before this spec; the capability test.
5. **Enrollment exemplar**: an `.individua` test document enrolling
   `#carl_linnaeus` and citing him — green; same document without enrollment
   — one vitium. This closes the measured hortus divergence at monolith level.
6. **Per-module citation pair**: intra-module target cited per-module;
   resolving/dangling pair under the MODULE canon (new capability).
7. **Restriction differential**: one crafted single-module document judged
   under module canon AND monolith; verdicts must agree for all in-scope
   checks (class-D subtracted). Red today, green after — then permanent.
8. Section VI audit (1021 recensitae) extended: emitted citations well-formed
   under the new grammar.

## 10. Out of scope / named futures

- `.genera` dictionary adoption of the signum (Fran: "at some point we might
  want to add this") — file as desideratum when this ships.
- Cross-document `#`-resolution — librarium (gradus III, per canon.canon).
- Scoped `super=` for the 587 multiplex sites — parked decision, untouched.
- Path-scoped `intra=` for per-genus action election — separate canon-language
  decision (class-3 divergence).
- Consumer-side signum-stripping helper in readers — on pull.
