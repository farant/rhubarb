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

Individuum proper names carry a leading `#` **everywhere they appear** —
declaration and reference alike (`nomen="#rosa-ad-murum"`,
`pollinatur-a="#apis-prima"`). Bare values in reference position name
**kinds**, in canon vocabulary (kebab, the same names used as tags:
`pollinatur-a="apis"`). The `#` is a one-character dispatcher between two dumb
checks: `#`-values resolve against document keys (citatio, verbatim match —
keys are sigiled too, so there is NO stripping anywhere); bare values must be
one of the kinds in the citation's existing `ad=` closure list (vocabulary
check, needs no keys, works identically per-module and monolith). Canon learns
no expression language.

Precedents: HTML `id`/`href="#x"`, CSS `#id` vs type selectors, house `##`
nids ("hash = identity" is existing house vocabulary). Philosophically: the
`#` marks rigid designation (a proper name) vs classification (a description)
— markup's capital letter.

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

## 4. Grammar — two new value-genera

Added to canon's genus electio (canon.canon + `genus_legere` +
`valor_congruit` in lib/canon.c):

- **`identitas`** — mandatory `#` followed by a non-empty `compositum` body.
  For identity attributes (`nomen=`). A bare identity is a VITIUM: the type
  system itself enforces that baptism is always marked.
- **`referentia`** — either `#` + non-empty `compositum` body (instance
  reference) or a bare `compositum` (kind reference). For all relation-origin
  attributes, including multiplex `ad=` (e.g. `generat`), in BOTH per-module
  canons and the monolith.

Notes:
- `referentia` everywhere kills the class-1 typing divergence
  (`nomen` per-module vs `compositum` monolith) as a side effect: both canons
  now agree.
- `certitudo=` and `fons=` stay `genus="nomen"` — they cite natura's keys
  (dictionary-side), never take the signum. (When ruling 2's named future
  lands, revisit.)
- Bare kind references are KEBAB (canon vocabulary = element titles), not
  snake. The duae-orthographiae convention (hortus header par.) is RETIRED:
  spelling is style, the signum is the mechanism.
- `#` alone (empty body) is a vitium under both genera.

## 5. Machina — citatio's signum dispatch (lib/canon.c)

Pass I (key collection) unchanged: collect `nomen=` values of elements whose
title is in `ad_elementa`. Keys now all begin with `#` (identitas), which
makes key-space and kind-vocabulary disjoint by construction.

Pass II, per citing value:
- value begins with `#` → look up in keys **verbatim** (no stripping). Miss =
  existing `CANON_CITATIO_IRRITA`.
- bare value → must equal one of the `ad_elementa` titles. Miss = NEW vitium
  **`CANON_VOCABULUM_IGNOTUM`** ("vocabulum extra petitum citationis"), whose
  detail uses the existing `_suggestio` machinery to offer the nearest kind
  title AND the hint that an instance reference wants `#` (the commonest
  authoring mistake will be an unsigiled instance name).

Uncited relations (open-target `ad='*'` 438 sites, multiplex 587,
cross-module targets in per-module canons) get typing (`referentia`) only —
same as today, now with intent visible. Honest limit, stated in the monolith
praefatio.

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
  fixtures): identities and instance references gain `#`.

## 9. Testing (pre-fix-oracle discipline throughout)

1. **Typing pairs**: `identitas` rejects bare / accepts `#x`; `referentia`
   accepts both forms, rejects lone `#`. Bare-`nomen=` vitium test fails on
   old code (was legal compositum) — true oracle.
2. **Vocabulary pair**: bare kind in closure passes; bare non-kind gets
   `CANON_VOCABULUM_IGNOTUM` with suggestion. Fails-before/passes-after.
3. **Signum citation pair**: `#`-reference resolving (enrolled) passes;
   `#`-reference dangling gets `CITATIO_IRRITA`. (Extends today's closure
   pair.)
4. **Kind-level statement**: `pollinatur-a="apis"` VALID — inexpressible
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
