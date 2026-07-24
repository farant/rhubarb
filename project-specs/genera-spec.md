# Genera per app definita — spec v1 (2026-07-23)

FileMaker-similis stratum entium in foro: user-defined entity types created
through the forum app at runtime, entries managed through derived views,
everything event-sourced through the existing tabularium machine.

Board park: 01KY656BG1. Raw interview: genera-interview.md. This is the
intent document; spec v2 (post-exploration) will carry exact file:line seams.

## I. Vision

The forum is the dogfooding surface. This feature makes it the house
FileMaker: define a genus (fields, types, relationships) in the app, get
working browse/detail/entry views derived from the definition — no layout
editor, no recompile, no deployment step. Every wanted-list item that is
secretly a database (books, timeline entries, later manutheca/finance)
becomes a genus + derived views instead of a separate app.

Strategic seed: the entity database brighton needs and the finite taxonomy
the forked internet needs get discovered empirically from genera Fran
actually defines for his real life. FileMaker proved ordinary people work
happily with semantic data when schema + entry + presentation live in one
direct-manipulation tool; the web killed that category by splitting data
from presentation; this restores it on owned infrastructure.

First real genera (the dogfood bar): **libri** (books — title, author
relationship, bare year, tags) and **linea temporis** (timeline entries —
bare years including BC).

## II. Data model

### Genus definitions are entities

A user genus definition is itself a res of a new seeded meta-genus
(working name: **`definitio`**). Its datum carries the schema:

```json
{
  "clavis": "liber",
  "titulus_monstrans": "Liber",
  "campi": [
    {"clavis": "titulus_operis", "monstrans": "Titulus", "typus": "textus"},
    {"clavis": "auctor", "monstrans": "Auctor", "typus": "relatio",
     "ad": "auctor", "cardinalitas": "unum"},
    {"clavis": "annus", "monstrans": "Annus", "typus": "annus"},
    {"clavis": "notae", "monstrans": "Notae", "typus": "area"}
  ],
  "campus_tituli": "titulus_operis",
  "ordo_ordinarius": {"campus": "annus", "directio": "ascendens"}
}
```

Why an entity and not a bare event: legere/quaerere/res read it for free,
gerere mutatio gives additive edits with judged writes, revision history of
the SCHEMA comes free in committed annales (better than FileMaker, where
schema changes vanished without trace). The machine's registry folds from
seeds + definitio entities at open/replay. (v2 must reconcile this with the
existing seed-time `definitio-generis` events — one mechanism, two sources.)

### Field types (the whole v1 palette)

| typus    | storage in datum        | notes                                   |
|----------|-------------------------|-----------------------------------------|
| textus   | JSON string             | single-line input                       |
| area     | JSON string             | textarea; same storage, UI hint only    |
| dies     | JSON string ISO date    | judged by shape                         |
| annus    | JSON number (integer)   | SIGNED: -753 = 753 BC; display adds era |
| numerus  | JSON number (integer)   | integers only in v1; no floats anywhere |
| relatio  | none in datum           | lives as nexus events, see below        |

No booleans (a tag IS a boolean). No images/containers (manutheca not in
v1 scope). No decimals (money later = integer cents). No calculations
(later; house version would be Latin formulas via the interpreter).

### Naming

Display name free-form (Latin or English, per-genus choice, unenforced);
machine key derived once at creation (slug: lowercase, spaces→underscore),
immutable thereafter — additive-only applies to keys. Genus clavis becomes
the genus string in events; field clavis becomes the datum key.

### Relationships

- Declared on the POINTING side: field = {clavis (the nexus verbum), ad
  (target genus), cardinalitas: unum|multa}.
- Data = existing nexus/denexus events with verbum = field key (the
  respondet-ad pattern generalized).
- Cardinality unum: a second nexus with the same verbum from the same res
  draws a custody note (judged, not blocked); the app does denexus+nexus on
  reassignment so clean data stays clean.
- Reverse direction: never declared; membra b→a query renders a free
  reverse portal on the target's detail view ("XII libri huc monstrant").
- Dangling: deletion tombstones the target; links survive; UI renders
  '[deletum]' (the forum-thread pattern). Nothing blocks, nothing cascades.

### Universal machinery already present

Tags (FTS-indexed, census-counted), titulus (fed from the designated title
field), actor + signatura provenance, mutatio = shallow merge (field edits),
remotio (field clears), tombstones, full event history.

## III. Machine changes (tabularium.c)

1. **Meta-genus `definitio` seeded** (SEMINA_GENERUM + SEMINA_NUMERUS bump;
   inline attributa per post-index-6 gotcha).
2. **Dynamic registry**: fold definitio entities (seeds + runtime events)
   into the genus registry the judgment path consults. Rebuilt on open and
   maintained per-event on fold. THE substantive machine change — v2 prices
   the exact seam.
3. **addere grows optional `datum`** (JSON object of field values), judged
   against the registry: unknown keys, type mismatches (string where number,
   non-integer annus/numerus, malformed dies) → custody notes appended,
   write proceeds. Judged-not-blocked throughout.
4. **Cardinality judgment**: nexus with a verbum that is a declared
   unum-field, when one already stands → custody note.
5. **Definition edits judged additive**: mutatio on a definitio that
   removes/retypes an existing campi entry → custody note naming the
   violation (write proceeds; the app never offers it).
6. **ab_lecto exclusion**: entry events of user genera skipped by the
   unread cursor (registry knows which genera are user-defined); definitio
   creations still surface as mail (a new genus is news). All events remain
   in plain acta.

## IV. App changes (apps/forum)

1. **Third top-level tab `Res`**: genus list with entry counts + "genus
   novum" creator entry point.
2. **Genus creator**: name → derived key (shown once); field rows (name,
   type, for relatio: target genus + cardinality); title-field designation
   (default first text field); optional default sort field + direction.
   Emits one definitio addere.
3. **Auto table view** per genus: columns = title field, scalar fields in
   definition order, relatio columns showing target titles, tags last.
   Column-header click sorts (annus sorts signed). Default order from the
   definition, else creation order. Row click → detail.
4. **Detail view**: all fields + reverse portals (who points here) + edit.
5. **Entry form** (create/edit): widgets by typus; annus input accepts
   negative + renders era label; relatio = type-ahead over target titles
   with inline-create row ("creare novum …" — target born with title only,
   linked immediately, details later).
6. **FTS search box** above the table (quaerere + genus filter).
7. **Tombstone rendering** for deleted relation targets: '[deletum]'.
8. Existing plumbing reused: per-request daemon client, diff-polling with
   focus guard, 22px warm-dark theme, speculum wearer.

## V. MCP surface

Full parity: Claude defines genera and creates/edits entries through the
same tools (addere with datum, gerere nexus/mutatio, legere, quaerere).
Machine judgment covers all writers identically; annales record actor +
signatura regardless. legere must return definitio entities like any res so
both the app and Claude read schemas through one door.

## VI. Culture

- Judged-not-blocked is the philosophy of the whole layer: your data is
  never refused at the door, it is annotated (moderate realism applied to
  one's own records).
- Schema history is correspondence-grade: every definitio and every edit
  is a signed event in committed annales.
- Entry churn is data, not mail: Claude's reading contract stays about
  discourse; a new genus is news, fifty book entries are not.

## VII. The bar

1. **Machine gates** (probatio_tabularium extension): definitio create →
   registry knows it; clean typed write → no note; type violation → custody
   note + write proceeds; unum double-link → custody note; additive
   definition edit clean / destructive edit noted; ab_lecto skips entry
   events but surfaces definitio creations.
2. **App gate** (fumus extension): JS defines a genus → creates entries
   (incl. negative annus) → table renders sorted → inline-create links an
   author → reverse portal shows it → FUMUS.
3. **Lived bar**: libri + linea temporis defined through the UI, loaded
   with real entries (a Rome entry at -753), used across at least two
   sessions, at least one entry added by Claude via MCP in normal
   conversation.

## VIII. Out of scope (named, not vague)

Statuses/lifecycles for user genera (v1.1, additive), images/containers
(manutheca trigger), decimals (finance trigger), layout editor, reports/
printing, CSV export, bulk import, revision-history UI, calculation fields,
world split for private data. Each arrives with evidence from daily use.

## IX. Open questions for v2 exploration

1. Exact registry seam: where SEMINA_GENERUM is consulted at judgment time
   and how a folded registry joins it; cost of rebuild-on-open vs
   maintain-on-fold.
2. Reconciling seed-time definitio-generis events with definitio entities
   (one registry, two sources — or migrate seeds' representation?).
3. addere datum param: envelope through fori proxy + MCP schema staleness
   (reconnect needed after daemon rebuild — known).
4. legere shape for relatio fields: does the app resolve targets per-row
   (N+1) or does legere embed target titles (the respondet_ad precedent
   suggests embedding)?
5. Slug derivation: where it lives (app-side at creation vs machine-side)
   — leaning app-side, machine stores what it is given.
6. Table-view rendering cost at hundreds of rows in the webview (books
   scale) — likely fine, verify the polling diff path.
