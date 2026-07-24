# Genera per app definita — raw interview (2026-07-23)

FileMaker-like user-defined entity system in the forum app. Interview conducted
the morning after Forum V1 shipped. Board park: 01KY656BG1 (parcum, "Genera per
app definita: FileMaker-similis in foro").

## Context that led here

- Fran's pipatum (01KY5HZ0ZE): "at some point make it more pkm/erp-y with
  different types of entities that can be referenced."
- Fran's pipatum (01KY5KEHPX): internet as a platform for sharing brighton
  documents — get people used to working with semantic data first, then the
  internet is the networked+ version.
- Conversation: FileMaker as the historical proof that ordinary people work
  happily with semantic data when schema + entry + presentation live in one
  direct-manipulation tool with no deployment step. The category (end-user
  databases) was killed by the web/SaaS split; the hunger persists (Airtable,
  Notion — rented and fragile). Brighton ≈ FileMaker where the records are
  documents; the fork-the-internet app ≈ FileMaker Server writ large.
- Fran: "forum is where i can dogfood a lot of stuff for myself first so i am
  thinking of basically building filemaker like capabilities into that."

## Settled in conversation (pre-interview)

- **Field palette v1**: text input, textarea (two UI flavors of text), date,
  bare year, number.
- **Relationships**: named field + target genus + cardinality (1-1 / 1-many).
- **Universal tags** on every entity (machine already has tags natively).
- **Title field**: one designated text field per genus (default: the first).
- **Cardinality convention**: the field lives on the record that POINTS;
  cardinality = how many targets one record may point at. The reverse
  direction needs no definition — it is a query on membra (free reverse
  portal).
- **Additive-only schema in v1**: add fields yes; rename/retype = migration,
  not offered.
- **Derived layouts**: forms/views generated from the definition; no layout
  editor in v1 (FileMaker's own adoption trick — a working default form for
  free the moment fields are defined).
- **Judged-not-blocked**: violations get custody notes, never rejections
  (moderate realism applied to one's own data).
- **The appeal (Fran)**: "a ui where i can create new genus types through the
  app and then manage entries of the genus types that i created."

## Round 1 — architecture forks

**Q: Where do user-defined genera and their entries live?** (forum world's
annales are committed to git; privacy consideration for finance-grade data;
tabulariumd serves exactly one world)
**A: Same forum world.** Genera live beside articulus/pipatum in forum.db +
committed forum.jsonl. Zero infrastructure work; matches "forum = dogfooding
surface." Finance-grade private data waits for a later world split.

**Q: Where does type judgment live in v1?** (machine currently judges against
compile-time SEMINA_GENERUM; runtime genera need a dynamic registry)
**A: Machine judges.** Machine builds a runtime genus registry from events and
judges every write — custody notes on violations, house-style. MCP writes
(Claude's included) get judged identically, not just app forms.

**Q: Status machines for user genera in v1?**
**A: Not in v1 — tags cover it.** A 'legendus'/'possessus' tag does the job
until the schema layer is proven. Definable lifecycles = natural v1.1,
additive like the rest.

**Q: First real genus (dogfood target)?**
**A: Books (catholicbookwishlist) + Timeline entries.** Books: title, author
relationship, bare year, tags — fits palette perfectly. Timeline: bare years
front and center, forces BC. NOT chosen: manutheca pages (would force an
image/container type into scope), finance recurrences (decimals + privacy
pressure). So v1 needs no image type and no decimals.

## Round 2 — data semantics

**Q: How do bare years handle BC?**
**A: Signed integer.** One number: negative = BC (store -753 for 753 BC),
display handles the era label. Sorting free, format trivial. (Astronomical
convention minus the year-0 headache.)

**Q: Dangling relationships when a pointed-at entry is deleted?**
**A: Tombstone rendering.** Nothing blocks; links survive pointing at the
tombstone; UI renders '[deletum]' where the entry was — same pattern the forum
threads already use for deleted comments. History stays honest.

**Q: Numbers — integers only or decimals?**
**A: Integers only in v1.** Covers books/timeline completely, keeps C89
parsing exact (no floating point in the machine); money later arrives as
integer cents anyway.

**Q: Does entity churn land in Claude's unread cursor (ab_lecto)?**
**A: Skip entries in ab_lecto.** Entry CRUD is data, not correspondence —
Claude's mail stays discourse (pipata, articles, comments). Everything still
lands in acta for review. Genus DEFINITIONS still count as mail (a new genus
is news).

## Round 3 — UI surface

**Q: Naming rules — genus/field names become machine identifiers?**
**A: Display name + derived key.** Type "Publication Year," machine derives a
stable slug key (publication_year) shown once at creation and never changed
(additive-only applies to keys). Latin or English display names — Fran's call
per genus, no enforcement.

**Q: Where do user genera live in the app's navigation?**
**A: One 'Res' tab.** Third top-level tab opens a genus list (with entry
counts); clicking a genus opens its browse view. Genus creator lives here too.
Scales to twenty genera without nav bloat.

**Q: Browse view shape?**
**A: Auto table view.** Columns derived from the definition: title field
first, then scalar fields in order, relationships shown as their target's
title, tags last. Click a column header to sort (year-sorted timeline free).
Click a row for detail.

**Q: Relationship picker interaction?**
**A: Type-ahead + inline create.** Search-as-you-type over target genus
titles; if no match, a 'create new …' row creates the target entry with just
its title, links it, details later. The daily flow (new book, new author)
never leaves the form.

## Round 4 — MCP + scope seal

**Q: Claude's MCP write access?**
**A: Full parity.** Claude can define genera AND create/edit entries via the
same tools the app uses — "add this book we just discussed" works; machine
judgment covers Claude identically; annales record who wrote what regardless.
Consistent with invited-spontaneity culture.

**Q: Default ordering of a genus's browse view?**
**A: Optional sort field in the definition** (+ direction; timeline: annus
ascending); absent that, creation order.

**Q: Search within a genus in v1?**
**A: FTS box on the browse view.** One input above the table, backed by
quaerere with the genus filter — machine already does the work.

**Q: Scope seal — anything missing?**
**A: Nothing — seal it.** Statuses, images, reports, CSV export, bulk import,
revision-history view: all arrive additively with evidence from daily use.

## Process note

After this raw file: spec v1 (genera-spec.md), then explorer agents over the
codebase (machine seams / app seams / MCP+test conventions), then spec v2 with
exact file:line seams (the forum build's proven pipeline).
