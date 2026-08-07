# Census — individua as seeded data, `.census` as the document form

**Status:** approved design, 2026-08-08 (Fran + Claude, in-session).
Stands on `project-specs/canon-referentia-spec.md` (the signa) — read
that first. **This change touches the GENERATOR and the REGISTRY only:
lib/canon.c is untouched.**

## 1. Rulings

1. **`<individuum>` entries stop being tags.** Kinds remain tags —
   genus, species, AND cultivar (`granny-smith` is a kind: millions of
   trees). The generator branches on the source ELEMENT TYPE
   (`<individuum>` vs `<species>`/`<cultivar>`), data natura already
   carries — no new ontological distinction needed, which by the
   projection law means the distinction is real. Grounds: (a) Thomist —
   a tag is a predicable, and individuals are not predicated
   (*individuum de nullo praedicatur*; no forma of Linnaeus); (b)
   engineering — tags-per-individual couples canon size and closure
   size to the census, which is unbounded, while kinds are
   Pareto-finite; (c) two measured incoherences die (par. 5).
2. **The projection gains a THIRD output: the seed census.**
   `natura/cocta/semina.census` — a GENERATED document in which every
   dictionary individuum appears as an instance of its kind's tag,
   carrying its dictionary knowledge as attribute values and children.
   "Seeded and equivalent to user-defined individua" literally: same
   grammar, same layer, machine-written. This is the encyclopedia's
   mechanically-generated page one.
3. **`.census` is the universal document extension** — the DEFAULT
   form for recording individuals; tags from ALL modules valid, judged
   by the monolith. The root element stays `<individua>` (extension
   names the genre, root names the content; zero canon churn).
4. **Per-module EXTENSIONS retire; per-module CANONS stay.** `.planta`
   and `.individua` leave `canones.registrum`; `.census` →
   `natura/cocta/individua.canon` is the sole natura document route.
   The 33 module canons keep being generated (free, same run) and stay
   reachable via `canon_examen -canon <via>` as opt-in PROFILES — a
   deliberate tightening for domain-closed interchange (a plants-only
   feed where a persona element is an ERROR), not a default with gaps.

## 2. Generator changes (tools/natura_canones*)

- Entity collection: entities whose source element is `<individuum>`
  are EXCLUDED from element emission (no `<elementum>`, no scoped
  children, no root `<liberum>` entry, no unicitas `super=` entry).
  Species/cultivars unchanged.
- Closures (`_clausuram_colligere`): individuum-typed res leave the
  `ad=` lists — closures become taxonomy-sized. (Res that are
  species/cultivar remain.) Keys for individuum references come from
  ENROLLED instances of kind tags, which the existing citatio
  machinery already collects — no lib/canon.c change.
- **Seed census emission** (`semina.census`): for each dictionary
  individuum, emit an instance of its NEAREST KIND's tag:
  `<persona nomen="#carl_linnaeus" natus="1707-05-23" ...>` — valores
  become attribute values; relatum targets get signa by entity type
  (`#x` when the target is a dictionary individuum — self-resolving
  within the seed census, since all dictionary individua co-reside;
  `.x` when the target is a kind). Free-text knowledge (nomina) becomes
  the kind's scoped children. GENERATUM signum in the head-3 window.
- The monolith praefatio documents the exclusion and points at
  semina.census.

## 3. What dies with individuum tags (both measured)

1. **The impostor hole**: `<carl-linnaeus nomen="#impostor"/>` is
   legal today — tag claims Linnaeus, identity disagrees, nothing
   clamors. No tag, no hole.
2. **The nonsense kind-reference**: `auctor-nominis=".carl-linnaeus"`
   type- and vocabulary-checks today ("authored by the KIND
   carl-linnaeus"). Individua leave the vocabulary lists; the `.x`
   test becomes exactly "is it a kind", as the trichotomy intended.

Enrollment simplifies to the kind form — `<persona
nomen="#carl_linnaeus"/>` — and deliberately does NOT carry the
dictionary defaults: a census references Linnaeus's facts (one home:
the dictionary, projected into semina.census), it does not restate
them per document.

## 4. Registry + hooks

`canones.registrum`: remove `.planta` and `.individua` keys; add
`.census	natura/cocta/individua.canon`. The canon-custos hook is
registry-driven (2026-08-07 fix) — `.census` gets judgment
automatically, zero hook edits. The retired-extensions comment stays
as history; note that per-module canons are profile-only, reachable
via `-canon`.

## 5. The self-judging gate

`semina.census` is a `.census` document like any other: **generated
data judged by generated grammar**. `canon_examen semina.census` must
be 0 vitia after every regeneration — the projection validating
itself end-to-end (a fresh differential: an emitter bug in EITHER
output shows up as the seed census failing its own canon). This gate
runs in probatio and fires via the hook on every regeneration write.

## 6. Migration (corpus = one exemplar + fixtures)

- `natura/exempla/hortus.planta` → `natura/exempla/hortus.census`,
  gaining the Linnaeus ENROLLMENT line — the class-D wart in its
  header DIES; hortus becomes fully checked (auctor citation
  resolves). Header rewritten accordingly.
- probatio fixtures: DOC_INSCRIPTUS pair moves to
  `<persona nomen="#carl_linnaeus"/>`; any fixture using an
  individuum tag migrates to kind-tag + enrollment.
- probatio_planta_lectio: reads hortus.census (path change);
  cocta.registrum planta reader entry unchanged (canon unchanged in
  role, only thinner).
- Restriction test (section X) stays — per-module canons as profiles
  still owe the restriction property.

## 7. Tests (pre-fix oracles)

1. Monolith contains NO `<elementum nomen="carl-linnaeus">` (fails
   today — true oracle) AND semina.census contains
   `<persona nomen="#carl_linnaeus"` (fails today: file absent).
2. Impostor: a document `<carl-linnaeus .../>` is CANON
   ELEMENTUM_IGNOTUM after (was legal — oracle).
3. `.carl-linnaeus` as a reference value is VOCABULUM_IGNOTUM after
   (was accepted — oracle).
4. semina.census: 0 vitia under the monolith (the self-judging gate).
5. Enrollment-by-kind pair: `<persona nomen="#carl_linnaeus"/>` +
   citation resolves / without enrollment IRRITA (adapts the existing
   DOC_INSCRIPTUS pair).
6. hortus.census: 0 vitia INCLUDING the auctor citation (the wart's
   death, asserted).

## 8. Out of scope / futures

- Cross-document reference to semina.census without enrollment —
  librarium (gradus III), as ever.
- Promotion tooling (census individuum graduating into the
  dictionary) — by hand until it hurts.
- Signa in `.genera` — desideratum 01KZEJABR0, unchanged.
- Retiring per-module CANON generation — not planned; they are free
  and serve as profiles.
