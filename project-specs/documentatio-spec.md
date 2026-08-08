# SPEC — documentatio.genera: the reader-task module

2026-08-08. Closes the genera-generation loop that began with the
glossae scenario. Sources: `project-specs/documentatio-lacunae.md`
(the 15-miss ledger), tabularium nota 01KZFMTPR2 (the five forks),
decreta 01KZFK6H3M. Design approved by Fran in session.

## The seam (settled by the descriptor, revised once here)

documentatio owns **the relation between a text and a reader's
task**: the reader-task genre of a work, the reader-defect
trichotomy, and the reader-walk as a test. It CITES:

- `opus_scriptum` for identity (work/expression/edition/codex)
- `iudicium` for the reader's epistemic state — `ignotum`, `fons`,
  `fundamentum`, and `annotatio` when the reader is an agent
- `fabricatio` for the intention frame (postulatum/probatio/vitium)
- `schema` for the grammar a document teaches

**Seam revision vs the nota:** part *structure* stays in
`opus_scriptum` — its `pars_de` relation ("opus intra opus maius —
compositio operum") already carries whole/part composition, so
fork 5 needs NO new mechanism. documentatio owns only what makes a
part pedagogical: its reader-task genre.

## The five fork rulings

1. **Genre lives on the WORK as intended reader-task; fit lives on
   the walk.** A manual is authored as a manual (design intent, like
   finis); whether it served THIS reader at THIS task is the walk's
   verdict. Same work can be intended-dictionary and
   measured-failed-manual without contradiction. Genre never claims
   success.
2. **The defect trichotomy is documentatio's own genus**, not a
   widened or subclassed `vitium`. Differentia deliberately
   PARALLEL to vitium without inheritance: vitium = intention vs
   BEHAVIOR under execution; this = what the text owed vs what a
   READING yields. fabricatio stays sharp.
3. **The bearer of "this document failed this reader at this task"
   is the walk event**, citing its findings: `annotatio` instances
   for an agent-reader's verdicts, `iudicium` for a person's,
   `invenit → defectus_documenti` for the defects, and
   `investigatio.parit → postulatum` for the improvement list. All
   existing machinery.
4. **The walk is a cross-model SPECIES of `probatio`**, after
   widening `probatio.exercet` to `ad="*"` at the parent — the
   exact L2/coquitur precedent (genus already general, only the
   closure wasn't). The species narrows exercet back to documents.
5. **Parts are works + inclusion.** No pars genus; `pars_de`
   already exists. A legend is a small work (genre `legenda`)
   inside a host work (genre `dictionarium`). `hortus.census` gets
   its name: `exemplum`.

## Collision-check record (run 2026-08-08, before any drafting)

FREE: `documentatio` `dictionarium` `manuale` `legenda` `exemplum`
`silentium` `ambiguitas` `deceptio` `defectus_documenti`
`ambulatio_lectoris`. TAKEN: `lectio`
(/elementa_programmatis/lectio, the state-read event) — never use
bare; compounds fine. RESERVED by format: `exemplar` (skeleton
element) — another reason the worked example is `exemplum`.

## New module: `natura/documentatio.genera` — nine genera

Cross-model sub= syntax per corpus precedent:
`<genus nomen="X" sub="parent" modulus="module">` (as
animal sub="vivens" modulus="corporalia").

### The four work genres — species of opus_scriptum

All four: `sub="opus_scriptum" modulus="opus_scriptum"`. Division
principle: **what the work enables the reader to do** (intended,
not achieved — each carries a nota stating fork ruling 1). Only
the four the scenario measured; the division stays open for future
measured additions. Hybrid works use `etiam=` secondary
membership.

- **`dictionarium`** — ad inspectionem: teaches what things ARE,
  ordered for seeking not reading; the reader looks something up,
  finds it, leaves. (glossae.html is one, and a good one.)
- **`manuale`** — ad actionem: teaches what to DO; the reader
  wants to complete a task and the work supplies the steps. (What
  the peregrinus needed and the page was not.)
- **`exemplum`** — ad imitandum: a complete specimen that shows
  rather than describes; the reader transcribes and adapts.
  (hortus.census IS one — the corpus finally gets a word for it.)
- **`legenda`** — clavis notationis: teaches what the signs in
  another work mean. (The missing key to `|...`, `#`, `AAAA-MM-DD`.)

Draft definitiones are the implementer's to write in Latin; the
parenthetical anchors above are the content each must carry.

### `defectus_documenti` + three species

- **`defectus_documenti`** — quod textus lectori debuit nec lectio
  reddit: the discrepancy between what the document owed and what a
  reading yields. Nota states the deliberate parallel to vitium
  and why there is no sub= (a document has no mores; it does not
  act, it is read). Properties: `contentum` (textus — what
  precisely). Relations: `in_opere ad="opus_scriptum"
  modulus="opus_scriptum"` (where it inheres).
- **`deceptio`** sub="defectus_documenti" — the text TEACHES a
  falsehood; a correct reading leads the reader into error. Has a
  victim; ranked first in force. (Measured case: the `|...`
  truncation hid `mortua`, teaching that a plant's death is
  unsayable — while `<planta status-vita="mortua"/>` passed.)
- **`silentium`** sub="defectus_documenti" — the text withholds
  what it knows: the knowledge exists and the telling omits it.
  Distinct from iudicium.ignotum (the READER's named gap); this is
  the PAGE's side. (Measured case: the root element never named.)
- **`ambiguitas`** sub="defectus_documenti" — the text admits
  several construals and gives the reader no ground to choose.
  (Measured case: `...` meaning three different things.)

### `ambulatio_lectoris` — species of probatio

`sub="probatio" modulus="fabricatio"`. Definitio: a test of a
document by a restricted reader — the reader is confined to the
source under test so the source itself is measured; the oracle is
not an assertion but the READER'S SUCCESS at a stated task.
Inherits `contentum` (the task attempted — "quid interrogetur"
already fits), `interrogat → postulatum`.

Own relations:
- `lector ad="*"` — persona aut programma; nota mirrors the
  scriba wording and points at parked minister-machinalis genus
  (parcum 01KZFM3GW0). The peregrinus was a programma-shaped
  reader; a human reader is persona.
- `invenit ad="defectus_documenti" multiplex="verum"` — the
  defects the walk surfaced.
- `exercet` NARROWED: `ad="opus_scriptum"
  modulus="opus_scriptum"` (narrowing an inherited relation is
  sanctioned — versio_de precedent). Nota records the subtlety
  that the reader physically reads a codex but the walk tests the
  WORK as expressed.

## Change to an existing module (exactly one)

**`fabricatio.genera`**: `probatio.exercet` widens from
`ad="functio" modulus="elementa_programmatis"` to `ad="*"` — the
sanctioned open form (regula II), same move as coquitur (L2).
Existing citations only loosen; species re-narrow where checking
matters. The relation keeps a nota naming L15 and the narrowing
convention. `continet → assertio` is NOT touched: the walk's
propositions ride annotatio/iudicium, not assertio, and widening
continet has no measured case.

`opus_scriptum.genera`: NO change. `pars_de` already exists and
is single-host, which matches every measured case; multiplex
waits for a measured multi-host part.

## Proof obligations (the build is not done until all three)

1. **The scenario judges the module.** Re-model the peregrinus
   scenario as a census using the new tags: the walk as
   `<ambulatio-lectoris>`, its three forced vitia now expressible
   as `deceptio`/`silentium`/`ambiguitas`, glossae.html enrolled
   as a `dictionarium`, hortus.census cited as `exemplum`, the
   improvement list via investigatio → postulatum (with contentum
   and ordo, both already shipped). Judged green by
   `bin/canon_examen` against the regenerated canons. Negative
   probes: at least one wrong-closure probe per new relation
   (e.g. a lector-less walk stays legal, an `invenit` at a
   non-defect is refused).
2. **Glossae discipline holds.** Every new genus (9) carries
   definitio (or differentia for sub-genera) + `<glossa
   lingua="en">` + `<glossa lingua="fr">` at birth —
   `./tools/natura_glossae.sh -porta` stays green; the count
   becomes 187/187 in all three languages.
3. **Full chain green.** `bin/natura_examen` (0 vulnera beyond
   the 2 standing inversa), `./tools/natura_canones.sh -probare`
   exit 0 (whole chain fresh), `./tools/canon_coquere.sh`,
   INDEX/METAMODULUS regenerated via their generators,
   `./compile_tests.sh natura`, `canon`, `planta_lectio`,
   `natura_glossae` all green.

## Deferred, explicitly

- Minister-machinalis genus stays PARKED (01KZFM3GW0) — `lector
  ad="*"` is the same deliberate opening as `scriba ad="*"`.
- Genre species beyond the measured four (index, praefatio,
  reference card…) wait for their own measured scenario.
- `pars_de` multiplex waits for a measured multi-host part.
- Comparative judgment (L6's "more interesting reading") stays
  unminted; `postulatum.ordo` carries the measured need.
- The inversa pair ruling (01KZC7EFMR) is untouched by this
  module.
