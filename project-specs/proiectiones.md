# proiectiones — view models derived from the journal (lib `conspectus`)

*Born 2026-10-20 in secunda, after genera-datorum D1-D3 (semitae R5;
`project-specs/semitae-spec.md` §VIII names this spec). Fran: "part of
how i see projections is basically as very performant de-normalized
view models." Designed in conversation the same day: purpose,
storage, maintenance, ergonomics, scale and composition were decided
one question at a time (§X); the sections below are the agreed
design, not proposals.*

*Discipline note: the PULL rule holds. The spec designs the composed
system (§VI) so the first slice does not paint it into a corner, but
code is built one consumer at a time: D4 builds ONE projection for
ONE widget. Persistence, composition, "as of" routes and full-text
search each wait for the consumer or the measurement that asks.*

## I. Doctrine

1. **A projection is a read model shaped for a view** (CQRS): the
   journal is the write side, normalized and append-only; a
   projection is denormalized ON PURPOSE, holding everything one view
   draws, so drawing does no assembly. Writes fan out (one act may
   stale several view models); reads are one lookup.
2. **A view model is a PURE FUNCTION of its sources.** One function
   `f(sources, key)` assembles it; the cache, a rebuild and
   verification all call that same function. There is no second path
   (event-by-event handlers) that could drift from it.
3. **Projections are `derived`** (genera-datorum §VI): rebuildable,
   never journaled, never backed up or synced, droppable at any time.
   Each device rebuilds its own.
4. **Stale is cheap, wrong is not.** Invalidation that is too coarse
   costs time; a MISSED invalidation is the one real failure, so the
   library carries a verification mode that finds it (§VII).
5. **Addresses are data, not panes.** A view model is addressed
   `projection/key` (`dies/2026-10-19`), independent of any widget;
   a pane's route DEMANDS addresses (§VI).
6. **No clock, no selection, no layout in a view model.** Today's
   highlight, the selected day and text cut to a column width stay in
   paint. A view model holding any of them would go stale at midnight
   or on a resize with no act to invalidate it. (The current week
   column mixes all three into its assembly - `hodierna`, `electa`,
   `praecidere(..., latitudo)` in `lib/kalendarium.c:3293-3425`.)

## II. What exists today (cited)

- **Classic event sourcing, one layer down.** `historia`
  (`include/historia.h:1-35`) folds a document's acts into its state
  in memory, with checkpoints and undo/redo/branches; agenda,
  personae, scriba pages and pictures are such documents. In ES
  terms each is an aggregate.
- **Observers.** `historia_observatorem_addere`
  (`include/historia.h:175-181`, S5b) calls back after every act,
  undo and redo; `lib/relatio.c:406` is the one consumer today.
- **Views query folded state by linear scans** (`include/agenda.h:185-
  243`: `agenda_res_diei`, `agenda_perfectae_die`,
  `agenda_signa_mensis`), and the accessors return pointers valid
  "only until the next mutation" (`include/agenda.h:185-187`).
- **Cross-source views assemble while painting.** The week view's
  `diem_hebdomadis_pingere` (`lib/kalendarium.c:3293`) collects, per
  day and per paint: birthdays (`fasti_fontes`), open agenda items,
  and the day's notebook lines through `scriba_liber_lineas_legere`
  (`lib/kalendarium.c:3422`), which OPENS that day's scriba documents
  through historia (`lib/scriba_liber.c:430`) - seven times per week
  paint.
- **Pure sources.** Liturgy and saints are functions of the date
  (reference data); they need no catch-up.
- **The only index on the journal** is `acta(genus, seq)`
  (`include/volumen.h`, `volumen_acta_generum_legere`).
- **Ordering** (D2): `seq` = arrival order in this volume, the HLC =
  event order; documents already apply acts in seq order.

## III. Layers and vocabulary

```
journal (acta)      truth, append-only, ordered by seq
  | fold - historia (exists): classic event sourcing
documents           agenda, personae, scriba pages - folded state
  | f - pure, cached by key (NEW)
view models         derived: one per address (projection, key)
  | read by
widgets             calendar first; later every pane declares demand
```

- **Projection:** a declared name, version, key type and set of
  SOURCES, plus its function `f` and a canonical encoding of its view
  model. The library is `conspectus` ("view, overview"); a view model
  is a conspectus (`DiesConspectus`), read with `conspectus_legere`.
- **Address:** `projection/key`, e.g. `dies/2026-10-19`.
- **Source:** something a projection reads, with a rule for which
  keys an act on it makes stale (§IV).
- **Demand:** the addresses a widget's current state needs (§VI).

Classic CQRS would fold the journal straight into the view models;
here the middle layer exists and is tested, so a view model only
answers "given these documents, what does key X look like?". The
price: a projection's documents must be open and current. When a
projection must cover data whose document is too large to keep open,
that ONE projection may be built the classic way (handlers + its own
cursor by seq); the declaration leaves room for it, nothing builds it
until a consumer needs it.

## IV. Declaration

**Data in STML, logic in C** (the canon has no expression language,
ever). Sketch - element and attribute names are placeholders until
the header text is approved:

```xml
<!-- lib/conspectus_dies.stml - beside lib/conspectus_dies.c -->
<conspectus nomen="dies" versio="1" genus="derivata">
  <clavis typus="dies"/>
  <fons documentum="agenda"   irritat="per-rem"/>
  <fons documentum="personae" irritat="omnia"/>
  <fons spatium="dies/{clavis}" irritat="spatium"/>
  <fons pura="liturgia"/>
</conspectus>
```

Source kinds (how an act on the source invalidates):

| kind | stale after an act | code needed |
|---|---|---|
| `documentum` + `omnia` | every cached key of the projection | none |
| `documentum` + `per-rem` | old ∪ new keys of the items (§V) | one helper: item -> keys it touches |
| `spatium` | the key named by the space (`dies/2026-10-19` -> key `2026-10-19`) | none |
| `pura` | never (a function of the key alone) | none |

The C side registers, by projection name: `f`, the canonical encoding
(view model -> STML text) and any `per-rem` helpers. A declaration
without registered C, or C without a declaration, is refused when the
registry loads. Sketch:

```c
/* view model of one day: everything a month cell or week column draws */
nomen structura {
      Dies  dies;
    chorda  celebratio;
       i32  color;
       Xar* natales;        /* chorda */
       Xar* apertae;        /* id + titulus, COPIED */
       Xar* perfectae;
       Xar* lineae_libri;   /* chorda, common indentation removed */
} DiesConspectus;

/* f: the only place a day is assembled */
interior vacuum*
dies_computare (ConspectusFontes* f, chorda clavis, Piscina* p);

/* per-rem: keys one agenda item contributes to */
interior vacuum
dies_claves_rei (vacuum* ctx, constans AgendaRes* r, Xar* claves)
{
    si (r->dies_posita) clavem_diei_addere(claves, r->dies);
    si (r->perfecta)    clavem_diei_addere(claves, r->dies_perfectionis);
}
```

**Where declarations live: beside the C that implements `f`**
(`lib/conspectus_<name>.stml` + `.c`), embedded like the manifests
(`tools/organa_infigere.sh`). A projection belongs to its DATA, not to
a widget: `dies` reads agenda, personae and the notebooks, and more
than the calendar will read it. Declaration and `f` must agree (the
loader refuses a mismatch), so they stay one edit apart. Widget
manifests only NAME the projections they read (demand, §VI).

**Versions.** Bumping `versio` drops the projection's cache (and,
once persistence exists, its stored table): derived tables never need
migrations.

**Copying is mandatory.** A view model outlives mutations, so it
copies what it shows (titles, ids) into its own memory; it never
keeps a pointer into a document (`include/agenda.h:185-187`). Copying
is also what makes it denormalized.

## IV-a. Before and after

**Before** (`diem_hebdomadis_pingere`, `lib/kalendarium.c:3293`,
~130 lines, per day, per paint): the liturgy rule ("the principal
celebration if memoria ad libitum or higher, else the first optional
memorial"), the birthday filter over the date signs, a scan of every
agenda item for the open ones of the day, OPENING the day's notebook
through historia and stripping its common indentation - then drawing.
The month cell and the day view each repeat their own version (the
liturgy rule again, `agenda_signa_mensis` / `agenda_res_diei`).

**After:**

```c
constans DiesConspectus* c = conspectus_legere(reg, "dies", clavis);
/* paint only: c->celebratio, c->color, c->lineae - assembled and
 * copied once, in dies_computare */
```

What becomes easier:

1. **A new source for every view.** Saints of the day (`sancti`) in
   the week, month and day views: before, three paint functions, each
   with its own copy of the logic; after, one line in `f` and
   `<fons pura="sancti"/>`.
2. **Testing at the data level.** "Anna's birthday shows on the 19th"
   is tested today by rendering the pane and searching the drawn text
   (`probationes/probatio_kalendarium.c:2603`,
   `textus_ullus_continet`); after, it asserts on the encoding
   (`<dies clavis="2026-10-19"><natalis>Anna Rossi turns 40</natalis>
   ...`) - no drawing, nothing to break when a pixel moves.
3. **A second reader** (a "today" summary in vicus, a clock tooltip, a
   Claude-facing "what is on my plate" command) reads `dies/<today>`
   instead of copying the assembly.
4. **Cross-pane updates by design.** A notebook edited in a scriba
   pane while the calendar shows that day: today the calendar is right
   only because it reassembles everything every paint; after, the
   `spatium` source stales `dies/2026-10-19` and only that day is
   recomputed.

## V. Cache and invalidation

- **Cache per address.** An entry holds the view model, its own small
  arena, and the `seq` it reflects. Reading a stale or absent address
  runs `f` once; invalidating releases the arena. A least-recently-
  used cap IN BYTES per registry bounds memory: view models vary
  widely (an empty day vs a day with 40 notebook lines), and the arena
  per entry makes the accounting exact (`piscina_summa_usus`,
  `include/piscina.h:97`). Only addresses someone read are cached (a
  month on screen is ~42 days).
- **Lazy recompute = debouncing.** An act only marks addresses stale;
  recomputation happens on the next read. Typing in a notebook (many
  acts per second) recomputes the day once per paint.
- **Notification** through the historia observers (§II). Undo, redo
  and branches need no special case: the document's state changes and
  the observer fires.
- **`per-rem` by diffing.** After each act on the source document,
  the library recomputes every item's keys with the helper and
  compares them with the keys it remembered; changed items stale old
  ∪ new. O(items) per act, no API change in the documents, correct
  across undo/redo/branches because it compares RESULTS instead of
  interpreting events. If a measurement shows it is slow, documents
  may report the ids an act touched; neither `f` nor the declaration
  changes.
- **Projections reading projections.** Reads made while `f` runs are
  RECORDED as dependencies (Salsa-style dynamic tracking); staleness
  propagates up the chain on its own. STML declares only the base
  sources. Cycles are refused when detected.
- **One registry per volume**, shared by every pane that reads it.
- **No threads** (house rule): `f` runs synchronously on read, in the
  paint path. Its cost is the read cost of a stale address - D4b
  measures it.

## VI. Composition (specified now, built at the second consumer)

1. **Demand next to the widget.** A widget's manifest declares which
   projections it reads; a C hook per widget maps its current state
   to addresses (calendar week of 2026-10-18 -> `dies/2026-10-18` ...
   `dies/2026-10-24`; month -> its 42 days). A hook, because the canon
   has no expressions; the route URIs (semitae §IV) are its input.
2. **Shared cache.** Two panes on the same day share one view model.
3. **Selective repaint.** After acts, stale addresses are intersected
   with each pane's demand; only panes with a hit are marked dirty,
   and vicus repaints only those.
4. **Recorded dependencies** (§V) make chains compose without
   declarations.

Out of scope (overkill here, network-era tools): normalized entity
caches (Relay/Apollo), query batching, prefetching neighbouring
weeks, persisting before a measurement asks.

## VII. Testing

- **Canonical encoding** per projection (STML text): verification
  compares encodings now; persistence stores them later - written
  once.
- **Verification mode** (the projection twin of
  `historia_verificare`): after every act, every cached address is
  recomputed from scratch and its encoding compared. Run over scripted
  sequences: create, move a day, complete, reopen, delete; undo, redo,
  a branch after undo; rename a person, change a birthday; edit a
  notebook line.
- **Precision:** moving an item 14 -> 16 stales exactly {14, 16}
  under `per-rem`; `omnia` stales all cached keys; `pura` never.
- **Plants** (through `silva.planta`, red first): drop the "old" half
  of old ∪ new (a missed invalidation - verification must go red); an
  `f` that keeps a pointer into document memory instead of copying;
  a version bump that keeps the cache; a projection-to-projection read
  not recorded.
- **Memory:** 10,000 acts with the cache open stay under a fixed bound
  (credo's per-section memory accounting, in main since 10-20).
- **Calendar (D4b):** existing kalendarium and vicus tests stay green
  UNCHANGED (a refactor of what is drawn, not of the drawing), plus
  Fran's look in vicus.

## VIII. Amendments to genera-datorum

- **§VII.2 and §VIII-a, narrowed:** "derived may not be the source of
  another derived" becomes: derived data never feeds TRUTH and is
  never stored or backed up as truth; a projection may read other
  projections when the dependency is recorded (§V) so the chain
  rebuilds and invalidates as one.
- **Slices:** R5 is this spec; D4 splits into D4a / D4b / D4c (§IX).

## IX. Phases

| phase | content |
|---|---|
| **R5** | this spec; the genera-datorum amendments (§VIII) |
| **D4a** | the library `conspectus`: registry, STML declaration + canon, cache with an arena per address, LRU cap in bytes, `omnia` / `pura` / `spatium` / `per-rem` sources through historia observers, diff-based `per-rem`, recorded dependencies, verification mode. Tested against a FIXTURE projection only |
| **D4b** | the `dies` projection; week columns and month cells read it; the assembly inside `diem_hebdomadis_pingere` (`lib/kalendarium.c:3293`) goes. Week paint measured BEFORE (baseline) and after. Fran's look in vicus |
| **D4c** | composition (§VI), when a second consumer arrives |
| **later, on PULL** | persistence in a sibling derived file (only when startup cost is measured); `@seq` "as of" routes (semitae §VI-d); D5 full-text on the same registry |

## X. Decisions (Fran, 2026-10-20)

1. **Purpose:** cross-source view models first (option 1 of four:
   cross-source views / persistence / loaders + invalidation / time
   travel), built so loaders and composition fit later.
2. **Projections are "very performant de-normalized view models"**
   (Fran's framing; CQRS read models).
3. **Storage B:** a sibling derived file plus memory, built in two
   steps - declaration, catch-up and in-memory read path first; the
   file when a measurement asks. Never tables inside the volume
   (truth and derived would share a file).
4. **Maintenance B:** pure function + invalidation, not classic
   reducers; the classic form stays available per projection for
   documents too large to keep open.
5. **Ergonomics accepted:** STML declares sources, C computes; the
   `per-rem` "which keys does this item touch" helper.
6. **Scope:** specify the composed design (§VI), build the single
   projection first (D4a/b); narrow the derived-from-derived rule
   (§VIII).
7. **Name:** lib `conspectus` (a view model = a conspectus). Checked
   free: the word appeared only in two prose comments. "proiectio" was
   refused - taken in spirit by materia's tree projections
   (`project-specs/proiectio-macrior-plan.md`) and `HistoriaProiectio`
   (`include/historia.h:60-79`), and five libs use private
   `proiectio_*` names.
8. **Declarations beside the C** that implements `f` (§IV), not in
   app manifests.
9. **Cap in bytes** per registry (§V), not a count of addresses.
10. **No clock, selection or layout in a view model** (§I.6).

## XI. Open questions for Fran

1. **The cap's default** (bytes): set in D4a from a measurement of a
   month's worth of day view models, not guessed here.
2. **Header text** of `conspectus` (D4a): names of the STML elements
   and attributes sketched in §IV are placeholders until then.

## XII. AUDIENDA

- **Not measured:** the week paint's cost today (seven notebook
  opens per paint) - D4b takes the baseline FIRST; if it is already
  negligible, D4b's case rests on one-place assembly and testability,
  not speed.
- **Not verified:** that vicus can repaint ONE pane alone today
  (§VI.3 assumes it can be made to); not needed before D4c.
- **Not verified:** that every agenda mutation reaches observers
  through historia (the diff-based `per-rem` relies on it); D4a tests
  it against agenda directly.
- **Not designed:** keys that are ranges (a projection keyed by week
  or month rather than by day); the calendar needs only days.
- **Not checked:** how `fasti_fontes` sources (birthdays) map onto
  `documentum` sources when a source is registered at run time rather
  than declared; D4b decides (likely: personae is the declared source,
  `fasti_fontes` stays the read path inside `f`).
- The `per-rem` diff costs O(items) per act; at today's ~155 agenda
  acts this is nothing, at 10^5 items it is unmeasured.
