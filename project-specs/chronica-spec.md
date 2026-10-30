# CHRONICA — a master timeline gathered from companion notes

*Designed with Fran 2026-10-30 (brainstorm in chat, after navigator
N3.5b and the main merge ea7ad46c). Any companion note may carry
`<event year="1234">description</event>`; every such event shows up on
one master timeline pane, a year at a time. DRAFT - `chronica` is a
proposed pane name (`fasti` is taken: it is the calendar library,
include/fasti.h); other Latin names are proposals until each phase's
header text is approved.*

## I. Intent

Fran reads history inside vicus (navigator on Wikipedia, the saints,
books) and writes companion notes beside what he reads. Dates turn up
in those notes all the time. He wants them to add up to one timeline
without keeping a separate list: write the event where you met it,
see it in its place among everything else.

- **The note is the source.** The timeline is never edited directly;
  it is gathered from notes and follows them when they change.
- **Every event leads home.** From the timeline you can jump back to
  the note the event was written in, beside the view that note
  belongs to (the Wikipedia article, the calendar day, ...).
- **People and places are things, not strings.** "Moses" is one
  person across every note that names him; opening Moses shows his
  events.

Prior art Fran pointed at:

- franarant.com/events.html - his own timeline page: 472 events,
  1920 BC to the 1960s, data in a script tag. Fields: `year`
  (negative = BC, shown "1446 B.C." / "1274 A.D."), `description`
  (usually a line, sometimes paragraphs with links), `people` (list),
  `place` + map coordinates + zoom, `type` (birth, death, coronation,
  publication, war, marriage, stigmata...), `image`, `month`. One long
  scroll: big grey year on the left, a line down the page,
  description / map / picture on the right.
- homeschooltimeline.com - a catalog of ~6000 events: a page per year,
  events with year, title - summary, linked person and place pages,
  tags, a random button.

## II. Goals (v1)

1. **The event tag.** In any companion scriba page or the common
   book (Fran, XIII), Fran types
   `<event year="-1446" people="Moses" place="Egypt" type="exodus">Moses
   leads the Exodus out of Egypt</event>` (or closes with `</>`). The
   text inside is the description. Attributes follow his events.html
   fields (Fran: "attributes make sense"). `year` is required.
2. **The master timeline pane**, a year at a time (Fran):
   - **h / l** = previous / next year that has events;
   - **j / k** = next / previous page within a year (pagination, never
     scrolling - house rule);
   - the year is shown large ("1446 BC", "AD 1274"), then that year's
     events.
3. **Jump home** (Fran): selecting an event and pressing Enter opens
   its companion page in the left pane, at the event, with the
   companion's paired view on the right. An event from the common
   book opens there and the timeline stays on the right (XIII).
4. **Person and place entities** (Fran): every name in `people` and
   every `place` is an entity. Opening a person shows that person's
   events (Fran: "events for Moses for now"); a place the same.
5. **Mouse** in every view (house rule): click an event to select,
   click again / double-click to open; click a person or place to
   open it.

## III. Non-goals (v1)

- **No seed data.** The 472 events.html events are NOT imported
  (Fran).
- No maps or images (events.html has them; later, if wanted).
- No editing on the timeline itself - all writing happens in notes.
- No person/place descriptions, lifespans, aliases ("King David" vs
  "David" are two people until aliases exist).
- No search kind for events yet (indagatio could gain one later).
- No date ranges / eras (`year` is a single year).

## IV. Anti-goals

- A second place to keep dates. If Fran ever has to copy an event
  into the timeline by hand, it failed.
- Losing the way back: an event shown without being able to reach
  its note.
- A scrolling infinite list. Pages, like every other view.

## V. What exists (read 2026-10-30)

- **Companion notebooks** (`project-specs/comites.md`): every
  right-hand view has a locus; its companion is a scriba book in space
  `comes/<locus>/paginae`, record `comes/<locus>/comes` = label +
  paired route.
- **Opening a companion page with its paired view** already exists:
  comites search result -> `vicus_comitem_ostendere` (comites P5b,
  3da533c6). Jump home should reuse it.
- **Projections** (`project-specs/proiectiones.md`, lib `conspectus`):
  derived view models, pure f(sources, key) with invalidation;
  declared in `lib/conspectus_<n>.stml`. `conspectus_comites.stml`
  already sources from `comes/{clavis}/paginae`. The gathered events
  are a projection.
- **Scriba** pages are a character grid (`lib/scriba_documentum.c`);
  `#tag` links are found by scanning line text and coloured in
  `lib/scriba_figurae.c:241` - the same place could colour `<event>`.
- **STML** parses `</>` (anonymous close) and attributes.
- **Dates**: `include/fasti.h` - `Dies {annus, mensis, dies}`, annus
  negative = BC.
- **personae** (contacts) exist with ULID ids - but historical people
  are a different thing (see VII).
- **Navigator** (`project-specs/navigator-spec.md`) is the nearest
  pane to copy: paginated, h/j/k/l, Tab/Enter selection, mouse via
  `des->punctum_locale`, presence-stored position.

## VI. The event tag

```
<event year="-1446" month="5" day="1" people="Moses; Aaron"
       place="Egypt" type="exodus">Moses leads the Exodus</event>
```

| Attribute | Meaning | Required |
|---|---|---|
| `year` | integer, negative = BC; 0 invalid (no year zero) | yes |
| `month` | 1-12 | no |
| `day` | 1-31, only with month | no |
| `people` | names separated by `;` | no |
| `place` | one place name | no |
| `type` | free word (birth, death, coronation, ...) | no |

- The description is the element's text, whitespace collapsed; it may
  span lines on the page.
- An event that does not parse (no year, bad year, never closed) is
  left out of the timeline. *Claude's call:* it is coloured as an
  error in scriba so Fran sees it on the page.
- *Claude's call:* well-formed events are coloured in scriba (accent
  colour on the tag) the way `#tags` are.
- *Claude's call:* names are matched exactly after trimming spaces
  (case-sensitive), so "Moses" in two notes = one person.

## VII. Entities

- A **person** or **place** exists because an event names it; it has
  no record of its own in v1. Its key is the trimmed name.
- *Claude's call:* separate from personae (contacts). Moses is not a
  contact; mixing them would put historical people in the address
  book. Merging later stays possible (same name -> link).
- A person/place page is a right-hand view with a locus, so it gets a
  companion notebook for free (comites) - Fran can write about Moses
  there.

## VIII. Views

**Year view (the master timeline).**
- Heading: the year, large ("1446 BC" / "AD 1274", house glyphs).
- Each event: the description, then a small line with month/day if
  given, type, people and place (people/place selectable, like
  links).
- Order within a year: events with month/day first in date order,
  then the rest in note order. *Claude's call.*
- Footer/title in the navigator style: "Timeline - 1446 BC - 1/2".
- Opening position: *Claude's call:* the last viewed year (stored in
  presence like navigator's position); first time, the event year
  nearest to today.

**Selection.** Tab / S-Tab move through the selectable things on the
page (events and their people/places), Enter opens, Esc clears - the
navigator N4.3 model. *Claude's call.*

**Person / place view.** Heading: the name; then all its events in
date order, each with its year; paginated j/k. Enter on an event =
jump home; Esc / Ctrl+O back (house history rules).

**Empty.** No events anywhere: a page saying how to write one.

## IX. Where it lives (proposal)

| Piece | Place |
|---|---|
| Event scanner: page text -> events (pure) | new lib (`chronica_eventus` or similar) |
| Gathering across companions + common book | a conspectus over `comes/*/paginae` and the common book |
| Pane: year / person / place views | `lib/chronica.c`, manifest `apps/chronica/organum.stml` |
| Scriba colouring | `lib/scriba_figurae.c` beside `nexus_colorare` |
| Jump home | reuse `vicus_comitem_ostendere` |

## X. Phases (proposal)

- **C1** event scanner + tests (parse, bad events, multi-line, `</>`).
  **DONE 2026-10-30** (`lib/chronica_eventus.c`, 11 sections, 5
  plants; extra cause "dies invalidus").
- **C2** gathering (two per-book declarations, XII): all companion books + common book -> events with origin (book,
  page, line); updates when a page changes.
- **C3** pane: year view, h/l years, j/k pages, footer, position.
- **C4** selection (reuse navigator unit code) + jump home (line plumbing through vicus + scriba, XII).
- **C5** person / place views.
- **C6** scriba colouring of good / bad event tags.
- Mouse in each phase as it lands.

## XI. Left to the builder

Library and file names (Latin, lint-clean), the conspectus
declaration details, page layout metrics, how the scanner reuses the
STML parser.

## XII. Builder check (2026-10-30) - what the code says

- **Gathering fits conspectus as it is (planning, 2026-10-30).** A
  `fons spatium` holds `{clavis}` once (`lib/conspectus.c:760-778`)
  and a change maps to ONE key (`lib/conspectus.c:465-503`) - so
  there is no single "all books" key, but there need not be: one
  declaration PER BOOK (`comes/{clavis}/paginae`, key = locus; page
  plagulae are `comes/<locus>/paginae/<n>`, `lib/scriba_liber.c:269`)
  and one per common-book PAGE (`paginae/{clavis}`, exactly like
  `lib/conspectus_liber.stml`). The master timeline is the union,
  rebuilt when an auditor reports a stale key. No conspectus change.
- **Jump home opens a page, not a line.** `vicus_comitem_ostendere`
  (`lib/vicus.c:4581`) passes prefix, title, route;
  `scriba_comitem_ponere` (`lib/scriba_applicatio.c:611`) sets book +
  title. Landing AT the event needs a line carried through vicus and
  scriba: C4 grows a cross-pane step. Pairing pattern:
  `lib/vicus_applicatio.c:4935-4967`.
- **Selection is ready to reuse**: navigator N4.3 / N6.3 unit code
  (Tab/S-Tab across pages, Enter, Esc, footer target).
- *Claude's call:* person/place keys are raw names; renaming "Moses"
  in notes orphans Moses's companion notebook. Accepted for v1
  (aliases later). Names are escaped in the locus (`/`, `?` split
  loci, `lib/conspectus.c:485`).

## XIII. Decisions (Fran, 2026-10-30)

1. **Tags stay English** as typed: `<event year= month= day= people=
   place= type=>`. (Code identifiers stay Latin; the lint does not
   read note text.)
2. **The common book counts too.** Jump home from a common-book
   event opens the note in the left pane and leaves the timeline in
   the right pane (no paired view to open).

## XIV. AUDIENDA

- Layout metrics and glyph sizes not yet measured in a real pane.
- Escaping rules for names in loci not yet read in `lib/semita.c`.
