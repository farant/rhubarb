# agenda + calendarium — plan (planner)

*Written 2026-10-10 in secunda from Fran's answers. The question: vicus
has scriba (text), pictor (drawings) and a clock with the date; Fran
wants a calendar/planner - year, month and day views, liturgical days,
and a built-in todo manager (Todoist-like) whose items live
independently of days.*

*Discipline: phases land one at a time, each red-first with plants,
each committed with its gates. PULL rule - nothing built ahead of the
phase that needs it. Header text shown to Fran before coding.*

## I. Decisions (Fran, 2026-10-10)

1. **Views**: year, month, day. More substantial than the legacy
   `calendario_visus` (old fenestra widget stack; reference only).
2. **Liturgical days shown**: feasts and season colours from
   `calendarium_liturgicum` + `sanctorale` (Ordinary Form, existing,
   tested in `probatio_calendarium.c`).
3. **Todo items exist independently of days.** An ITEM (not a list,
   not a category) is what gets assigned to a day, and it can be
   reassigned. Unscheduled items are allowed.
4. **Completion records the day it happened**, and the calendar shows
   it.
5. **Categories are tag-style**: an item carries any number of them;
   arbitrary names; items can be grouped by category. Working name:
   *rubrica* (the red heading of a missal).
6. **Items have a title plus a list of notes/comments.**
7. **Overdue is not a concept**: no rolling forward, no overdue state.
   But an incomplete item never disappears from the dedicated todo
   view/editor, whatever its day.
8. **Each day has its own paginated document**, reusing scriba, in a
   different form factor: embedded in the calendar's day view (not the
   left scriba pane).
9. **Recurring items will exist, not yet.** The data model must not
   preclude them.
10. **Notes carry a date; the day is enough** (time of day not
    important). Stored in the act from the clock port (local day).
    volumen already stamps every act with `momentum` (UTC, ms, SQLite
    `strftime` at append) - a free audit trail, but UTC (wrong local
    day in the evening) and not injectable in tests, so never
    load-bearing.
11. **Rubrics are hashtag-style kebab-case names** (`house-projects`)
    for now, but may become entities later (a description, ...):
    items reference them by name; a future `<rubrica .../>` entity act
    can attach data (or rename) keyed by name without changing item
    acts.

## II. Where things stand (2026-10-10)

- `fasti` (`include/fasti.h`): `Dies {annus, mensis, dies}`, weekday,
  days in month, add days, ISO parse/format (`fasti_parsare_iso`,
  `fasti_formare_diem`).
- `calendarium_liturgicum` (`include/calendarium_liturgicum.h`, corpus
  `lib/calendarium_liturgicum.c` + `lib/sanctorale.c`): day lookup,
  seasons, colours, cycles.
- `calendario_visus`: legacy month widget (fenestra `widget.h`), vim
  navigation; used only by `concha`/`schirmata`. Not reusable as a vicus
  pane; grid math and navigation are reference.
- `historia` L1 hooks (pictor-strata L1): a client whose state is NOT a
  fixed-size block seals it (`sigillare`), encodes its checkpoint
  (`codificare`) and restores it (`decodificare`). The agenda document
  is exactly such a client.
- vicus: right-stack panes by genus (`$pictor`, ...), facies hooks,
  `VicusHorologium` port gives today (tests inject a fixed date).
- scriba: `scriba_liber` (page book), `scriba_documentum`.

## III. Model

ONE agenda document (a historia log, like a pictor drawing); calendar,
day view and category lists are projections of it. Acts (genus
`agenda`), dates ISO `YYYY-MM-DD`:

```
<res actio="nova" id="17" titulus="..."/>
<res actio="titulus" id="17" titulus="..."/>
<res actio="dies" id="17" dies="2026-10-14"/>     dies="" = unscheduled
<res actio="rubricae" id="17" rubricae="domus opus"/>   whole set
<res actio="nota" id="17" dies="2026-10-12">text</res>
<res actio="perfecta" id="17" dies="2026-10-13"/>
<res actio="aperta" id="17"/>                     un-complete
<res actio="deleta" id="17"/>
```

- Dates that come from "now" (completion, note date) are IN the act,
  captured from the clock port by the caller: replay never reads a
  clock.
- Ids: max + 1, assigned by the document.
- Reassignment history is the log itself (available later, not
  projected now).
- Day documents: scriba, one book per day (namespace keyed by date) -
  phase A6 decides the exact addressing.

### State tiers (same convention as pictor/scriba)

- **Volume (historia)**: the agenda content - items, days, tags,
  notes, completion. Like scriba ("textus ipse in volumine, non hic")
  and pictor pixels, it is NOT on the island.
- **Durable island** (`calendarium`/`agenda` canon): document
  metadata only, e.g. `sigillum` owned by scriptor `documentum`.
- **Ephemeral island**: UI state that survives restarts - view mode
  (year/month/day), selected date, selected item, tag filter, text
  being edited, focus, surface size; each attribute with one owner
  action (domini.stml).
- **Motus**: hover, drag - memory only.
- `agenda.h` (A1) knows nothing of the island, like
  `pictor_documentum.h`; the actions layer (A2+) bridges. One agenda
  document shared by every `$calendarium` / `$agenda` pane (vicus
  context, like `ContextusPictoris.documenta`).

### Fran's three state categories (2026-10-10) vs today's tiers

1. **Shared data/entities** - shown by several widgets (documents) ->
   volume (historia). Agenda content.
2. **Widget state persisted across sessions** -> today the EPHEMERA
   island (the name says ephemeral; it persists). Agenda: view mode,
   selected date, selected item, tag filter.
3. **Truly transient widget state** (dropdown open, animation) -> NO
   home today. `motus` is a write buffer FLUSHED into the ephemera
   island at quiet, so such state ends up persisted: pictor `palette`
   (open palette), `semen`; scriba `clavis_praecedens`,
   `fd_exspectans`, `nuntius`.

Also ambiguous: the DURABLE island holds per-pane copies of document
facts (pictor `latitudo`/`altitudo`, `sigillum`; the canon's `stratum`
elements predate L2 and look unused). Is it category 1 or 2?

State-model question to settle before A2 (A1 is content only).

## IV. Phases

- **A1 — `agenda` library**: document (create/open, typed act helpers,
  undo/redo, verify), projection (items with title, day, tags, notes,
  completion), queries needed by A2/A3 only. No UI.
- **A2 — month view pane** `$calendarium` (opens on today; argument
  `YYYY-MM`): grid, liturgical colour + feast, per-day marks (items
  assigned, items completed).
- **A3 — day view**: feast, items of the day, tick/untick, reassign,
  notes.
- **A4 — todo view/editor** `$agenda`: all incomplete items (whatever
  their day) + unscheduled, grouped by category.
- **A5 — year view.**
- **A6 — day documents**: embedded scriba per day.
- Later: recurring items.

## V. AUDIENDA

- Seal cost: canonical text of the whole state per act - fine for
  hundreds/thousands of items; measure in A1.
- Exact vicus shape of a pane that switches year/month/day (one genus
  with a mode, or three genera).
