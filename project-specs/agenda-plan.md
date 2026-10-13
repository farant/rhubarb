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
  - **A1a** DONE 2026-10-13 (lib/agenda.worklog.md): include/agenda.h
    (Fran approved), historia L1 client, typed acts, projection,
    undo/redo, reopen, verify; titles in attribute-element form
    (`<@titulus=>`, quotes representable). probatio_agenda (8
    sections, plants II).
  - **A1b** DONE 2026-10-13 (lib/agenda.worklog.md): `agenda_res_diei`,
    `agenda_perfectae_die`, `agenda_signa_mensis` (assigned / open /
    completed); seal cost measured (tools/agenda_mensura.c).
- **A2 — month view pane** `$calendarium` (opens on today; argument
  `YYYY-MM`): grid, liturgical colour + feast, per-day marks (items
  assigned, items completed).
  - **A2a** DONE 2026-10-13 (lib/kalendarium.worklog.md): one genus
    `calendarium` + `visus` field, C prefix `kalendarium_`, selection
    transient, Sunday first (Fran); manifest, include/kalendarium.h,
    42-cell grid (id = ISO date), keys h/l/j/k/H/L/t, click, argument;
    vicus genus + `$calendarium`, one shared agenda document, clock
    port.
  - **A2b** DONE 2026-10-13 (lib/kalendarium.worklog.md): one pure
    figure on the root draws all cells (colour bar, feast, marks,
    today, selection); title with season and weekday letters; 51 us
    per month render at 1000 items, no cache. Found: liturgical
    library ranks OT Sundays above solemnities (parcum ...KC11SH).
- **A3 — day view**: feast, items of the day, tick/untick, reassign,
  notes.
  - Design (Fran 2026-10-13): same pane, `visus="dies"`; keys j/k h/l
    x </> X u/Ctrl-R, prompt keys n/a/c/D; one-line prompt on vim.h
    (insert mode; Enter commits, Esc cancels); `n` creates items here;
    view stays when an item moves; notes under every item.
  - **A3a** DONE 2026-10-13 (lib/kalendarium.worklog.md): view switch,
    list (assigned + completed here), selection, tick/untick, move,
    delete, undo/redo, click selects.
  - **A3b** DONE 2026-10-13: one-line prompt on vim.h (insert mode;
    text stored escaped via scriba's codec), n/a/c/D, Enter commits,
    Esc cancels, bad date refused.
  - **A3c** DONE 2026-10-13: header (date, celebration, season), rows
    (checkbox, title, notes, selection), prompt line; screenshot
    checked.
- **A4 — todo view/editor** `$agenda`: all incomplete items (whatever
  their day) + unscheduled, grouped by category.
  - Design (Fran 2026-10-13): a third view of the calendarium pane
    (`visus="agenda"`), `$agenda` = alias; grouped by rubric, then
    "(none)"; dates shown, no overdue; notes only for the selected item;
    `r` rubrics, `f` filter (persistent); Enter -> day view deferred.
  - **A4a** DONE 2026-10-13: queries `agenda_apertae`, `agenda_rubricae`;
    view, grouping, keys, `$agenda`.
  - **A4b** DONE 2026-10-13: `r` rubrics prompt (both views), `f`
    filter (persistent; Esc clears; `n` inherits the filter rubric).
  - **A4c** DONE 2026-10-13: header "Agenda: n open" (+ "#filter"),
    group headings, rows with the date at the right ("-" unscheduled),
    selected item's notes; screenshot checked. A4 complete.
- **M — mouse + commands** (Fran 2026-10-13; order agreed):
  - **M0a** DONE: shared mensa helper (`motus_ramus_focatus`,
    `figura_mensam_pingere`) and `figura_color_thematis` (5 copies -> 1).
  - **M0b** DONE 2026-10-13: kalendarium command table
    (TABULA_MANDATORUM: name, label, key, view mask, requirement mask,
    handler); letters with Cmd/Ctrl no longer fire.
  - **M1** mouse batch 1 via the table: double-click day -> day view;
    click `[ ]` toggles; header arrows + view switcher (Month · Day ·
    Agenda); wheel over the month = month -1/+1.
  - **M2** batch 2: double-click row -> title prompt; click group
    heading -> filter; right-click context menu (from the table).
  - **M3** generalize: commands declared in the manifest, availability
    query on the action registry, then the palette UI (after a 2nd
    widget has a table).
  - Long lists: PAGINATION, not scrolling (Fran 2026-10-13).
- **A5 — year view.**
- **A6 — day documents**: embedded scriba per day.
- **Next after the widget (Fran 2026-10-13):** fix the liturgical
  library - review in project-specs/liturgia-recensio/RECENSIO.md
  (US calendar; other calendars a later design question).
- Later: recurring items.

## V. AUDIENDA

- Seal cost: canonical text of the whole state per act - fine for
  hundreds/thousands of items; measure in A1. **Measured (A1b):** ~1.5 us per item
  per act (1000 items: 1.56 ms/act, reopen 4.7 ms); bulk creation is
  quadratic (2000 items: 9.3 s). Incremental seal if ever needed.
- Exact vicus shape of a pane that switches year/month/day (one genus
  with a mode, or three genera).
