# nomenclator — contacts app plan (personae)

*Written 2026-10-14 in secunda from Fran's answers. The question: after
the calendar/agenda, Fran wants a contacts app - people and businesses
with names, tags (groups), phones, emails, notes, birthdays (shown on
the calendar, with age) and later a profile picture; browsable
alphabetically, grouped by tag, and by fuzzy search. A* nomenclator
*was the Roman slave who walked beside his master and whispered the
names of the people they met.*

*Discipline: phases land one at a time, each red-first with plants,
each committed with its gates. PULL rule - nothing built ahead of the
phase that needs it. Header text and manifest text shown to Fran
before coding.*

## I. Decisions (Fran, 2026-10-14)

1. **Two kinds of contact, one toggle**: person (*homo*) and business
   (*societas*). No extra business fields for now - notes and tag
   groups cover it.
2. **Names**: a person has first and last name, SHOWN "Anna Rossi",
   SORTED by last name (then first). A business has one name and sorts
   by it. Both kinds live in one list.
3. **Tags** put contacts in groups - same idea as agenda rubrics
   (hashtag-style kebab names; a contact may carry several).
4. **Browsing**: alphabetical; alphabetical grouped by tag; fuzzy
   search.
5. **Phones and emails**: several allowed, the default is one.
6. **Notes**: an embedded scriba page per contact, like the calendar's
   day view.
7. **Birthday with or without year**. With a year the profile shows
   the age; birthdays show up on the calendar.
8. **Profile picture later**: an embedded pictor, roughly 1/6 of the
   screen area. The detail layout reserves the space from the start.
9. **Reuse**: pull what the calendar already has into a widget /
   component UI library - "a good investment" (Fran). Contacts is its
   second consumer.

Answers to the first open questions (Fran, 2026-10-14, same day):

10. **No labels** on phones or emails for now.
11. **Fuzzy search covers names only.**
12. **Accented letters sort as their unaccented letter** - not needed
    yet (no such names expected); do it when a consumer appears.
13. **Command word: `$contacts`.**
14. **Dates reach the calendar through a generic date-source port**,
    not a direct dependency on contacts - and the dates are TYPED, so
    the calendar knows a date is a birthday and can display it its own
    way.
15. **Recurring schedules get their own library** (Fran, 2026-10-14):
    a "recurring scheduled date" data type ("2nd Monday of the month",
    "every other Tuesday", ...) that the calendar knows, and from which
    clients extract the concrete dates in a range. Birthdays are its
    simplest case (yearly), but they do not wait for it - P6 handles
    them simply, in a shape the recurrence type can later replace.
    Survey 2026-10-14: no cron or recurrence code in rhubarb nor in the
    predecessor trees (nog*, solarium - only vendored sqlite3.c hits);
    building blocks in `include/fasti.h` (weekday, first weekday of a
    month, add days/months/years, day/week of year) and Easter in
    `calendarium_liturgicum`. Agenda decision 9 (recurring items, data
    model must not preclude them) is the same need.

## II. Where things stand (2026-10-14)

- **Fuzzy matching exists**: `include/similitudo.h` - fzf-style
  subsequence scoring (word boundaries, runs, exact case rewarded;
  gaps punished), pure functions, no allocation;
  `similitudo_optima` (`include/similitudo.h:41`) ranks candidates.
- **Document pattern**: `include/agenda.h` - one historia log in the
  volume (genus `agenda`), acts like `<res actio="nova" .../>`, the
  state is a projection; undo/redo, seal and checkpoint come from
  historia L1. Rubric rules (kebab, canonical order, no duplicates)
  live there.
- **Calendar list machinery**, all `interior` in `lib/kalendarium.c`
  (4608 lines):
  - rows and pages: `ordines_indicis` (:1318), `capacitas_indicis`
    (:1403), `titulum_gregis` (:1426, repeated group heading),
    `altitudo_tituli` (:1459, heading chains keep with the first item),
    `paginas_computare` (:1485), `pagina_rei` (:418),
    `paginam_vertere` (:1628);
  - layout and drawing: `indicem_disponere` (:4302), `greges_pingere`
    (:3079);
  - commands: `TABULA_MANDATORUM`, `clavis_congruit` (:2082),
    `mandata_verificare` (:2112), `contextum_parare` (:2141),
    `requisita_impleta` (:2163), `mandatum_invocare` (:2176);
  - toolbar buttons: `GLOBULI`, `globulos_disponere` (:4388),
    `globulum_premere` (:2574);
  - prompt line: `prompta_aperta` (:724), `agere_promptam` (:1167);
  - context menu wiring: `TABULA_ELENCHORUM`, `elenchum_legere`
    (:2360), `elenchum_aperire` (:2438), `elenchum_agere` (:2496);
  - embedded scriba book: `notas_montare` (:3896), `librum_aptare`
    (:2747), `notarum_area` (:77).
- **Already libraries**: `elenchus` (the menu itself), `dimissio`
  (popover closing), `dispositio` (Clay-style layout), `figura` (the V1
  drawing helpers), `scriba_liber` + `scriba_montare_in_ramo` (nested
  book), `derivare` (double-click, now per button).

## III. Model

ONE contacts document (historia log, genus `personae`). Acts:

```
<persona actio="nova" id="7" genus="homo" praenomen="Anna" cognomen="Rossi"/>
<persona actio="nova" id="8" genus="societas" titulus="Rossi e Figli"/>
<persona actio="nomen" id="7" praenomen="Anna" cognomen="Rossi-Bianchi"/>
<persona actio="titulus" id="8" titulus="Rossi e Figli s.r.l."/>
<persona actio="genus" id="7" genus="societas"/>
<persona actio="rubricae" id="7" rubricae="familia roma"/>     whole set
<persona actio="telephona" id="7">                             whole list
  <telephonum valor="+39 06 1234567"/>
</persona>
<persona actio="epistulae" id="7">                             whole list
  <epistula valor="anna@example.org"/>
</persona>
<persona actio="natalis" id="7" dies="1986-03-02"/>   "--03-02" = no year;
                                                      dies="" = none
<persona actio="deleta" id="7"/>
```

- **Both name forms are kept**; the kind decides which one is shown.
  Toggling to a kind whose name is empty fills it from the other
  ("Anna Rossi" <-> praenomen "Anna", cognomen "Rossi") - decided in
  P1 with Fran.
- **Sort key**: person = cognomen, then praenomen (empty cognomen ->
  praenomen); business = titulus. Case-insensitive. Letter heading =
  first letter of the key, uppercased; anything else under "#".
- **Lists are written whole** (phones, emails, rubrics), like agenda
  rubrics: an edit is one act, and undo is one step.
- **Birthday without year** uses the ISO 8601 no-year form `--MM-DD`.
  Age is NEVER stored: it is computed from today (clock port) at
  display time.
- **Notes**: scriba book `persona/<id>` (like `dies/<ISO>`).
- **Picture (later)**: a pictor document keyed by the contact id.
- **State tiers** (agenda-plan III): content in the volume; view state
  on the island (view: list / by tag / detail; selected contact; search
  text; tag filter; page); menus as popover fields.

### Calendar integration: a typed date-source port (decision 14)

The calendar does not know contacts. It reads dated entries from any
number of registered date sources; contacts is the first. Shape to be
shown to Fran as header text in P6 - the idea:

- an entry = date (full, or month-day for yearly ones), **genus**
  (`natalis` first; the calendar has one display rule per genus),
  title ("Anna Rossi"), optional origin year (birth year -> age), and a
  reference the calendar can hand back to open the source ("$contacts"
  + contact id);
- a source answers "entries of month M of year Y" (yearly entries are
  repeated into every year by the source or by the port - decided in
  P6);
- display rules: `natalis` = a birthday mark on the month cell, and in
  the day view "Anna Rossi turns 40" (year known) / "Anna Rossi's
  birthday"; an unknown genus falls back to a plain titled row;
- the calendar's own agenda items stay as they are (the agenda is the
  calendar's own document, not a source).

## IV. Component library (phase U): what moves out of the calendar

Order follows need: contacts lists need U1 first. Each step moves the
calendar onto the new library with its tests unchanged - the calendar's
suite (30 sections) is the oracle for every extraction.

- **U1 paginated index** - row model (id, kind, height, owner id),
  page computation (capacity, keep-with-next chains, repeated group
  headings), current page, page turning, the page of an item,
  selection following the page; layout of one page into a dispositio
  region; heading/row drawing hooks supplied by the host.
- **U2 command table** - rows (name, description, key, view mask,
  requirement mask, handler), key matching, verification at mount,
  invoke by name, availability query (the M3 palette will read this).
- **U3 prompt line** - the one-line vim input field (label, text,
  cursor; the fields live in the host's island).
- **U4 context menu wiring** - menu table (context, command, divider)
  -> `Elenchus`, open / read / act; built on `elenchus` + U2.
- **U5 embedded book ("post-it")** - mount a scriba book in a region,
  cell-exact area, shadow, book follows the selection.
- **U6 toolbar** - button table (id, text, commands per view),
  segmented controls, arrow buttons.

- **U1 DONE 2026-10-14: `album`** (include/album.h). The approved name
  `catalogus` collided at link time with `cellae.h`'s public
  `catalogus_*` functions; Fran chose `album`. Rows declare their kind
  (ALBUM_RES / ALBUM_TITULUS / ALBUM_FASCIA); the calendar's 30
  sections passed unchanged after the move.
- **DECIDED (Fran, 2026-10-14): UI component libraries carry the kind
  prefix `membrum__`** on their FILES (`include/membrum__album.h`,
  `lib/membrum__album.c`, `probationes/probatio_membrum__album.c`);
  functions and types keep the short library name (`album_*`,
  `Album`); header guard `MEMBRUM__ALBUM_H`. `ls include/membrum__*`
  lists them. Order (Fran asked, Claude recommended): P1 next (pure
  document, settles the data model), U2 right before P2 (its second
  consumer shapes it). Background - the ask was The ledger already
  has the house convention (…PTMJ2, 2026-10-08): double-underscore
  KIND prefixes on file names (`forma__`, `stilus__`, `nuntius__`,
  `probatio__`, `generata__`) with a kind registry in STML; libraries
  keep their name as the function prefix. Proposal: a UI-component kind
  (e.g. `membrum__album.h`, functions stay `album_*`), decided before
  U2 so the renames happen once.

Names, and each header, go to Fran before code. The library is
probably several small ones (one per item above), following the
house rule of independent composable libraries.

## V. Phases

- **U1–U6** as above (U1 before P2; the others as P-phases pull them).
- **U2 DONE 2026-10-14: `membrum__praecepta`** (include/
  membrum__praecepta.h) - command table (key forms, check, lookup,
  absent/grey/available, invoke); requirements and the Esc policy stay
  with the host. Calendar moved onto it, 30 sections unchanged.
- **P1a DONE 2026-10-14: `rubrica`** (include/rubrica.h) - the tag
  rules shared by agenda, calendar and contacts (were two private
  copies).
- **P1b DONE 2026-10-14: `personae`** (include/personae.h) - with one
  forced deviation: phone/email values use the element-attribute form
  `<telephonum><@valor=>..</></telephonum>` (a '"' in a plain attribute
  is not representable).
- **P1 `personae` document** - acts, projection, sort key, letter
  heading, birthday parse (`YYYY-MM-DD` / `--MM-DD`), age function,
  natales projection; undo/redo, seal; tests.
- **P2a DONE 2026-10-14: the list** - vicus genus `nomenclator`,
  `$contacts`, alphabetical list with letter bands (album), j k [ ],
  click to select, page arrows; one shared personae document per
  volume. Next: U3 (prompt line), then P2b (new person / business,
  delete, undo). Layout glue copied from the calendar = membrum__
  candidate (second consumer).
- **U3 DONE 2026-10-14: `membrum__prompta`** (include/membrum__prompta.h)
  - prompt line on the host's island branch; Home/End fixed on the way.
  Calendar moved onto it, 30 sections unchanged. Next P2b: new person /
  business (keys n / N + a "+ New" button - Fran to choose between a
  menu, two buttons, or Tab in the prompt), delete, undo.
- **P2b-1 DONE 2026-10-14**: n / N (prompt "name:" / "business:"), X
  delete (selection moves to the next contact), u / C-r. Next P2b-2:
  U4 (menu wiring out of the calendar) + "+ New" button opening a
  Person / Business menu (Fran chose option 1).
- **P2 nomenclator list** - vicus genus `nomenclator`, command word
  `$contacts`, alphabetical list with letter headings (U1), selection, new
  contact (kind chosen at creation), open detail.
- **P3 by tag** - grouped by tag like the agenda (a contact appears
  under each of its tags), tag filter, heading click filters.
- **P4 fuzzy search** - prompt (U3) + `similitudo` over display names
  only; results ranked, paginated.
- **P5 detail view** - name(s), kind toggle, phones / emails (one by
  default, add more), tags, birthday + age, notes book (U5) in the
  bottom half; space reserved for the picture.
- **P6 birthdays on the calendar** - the typed date-source port (III),
  contacts as its first source, `natalis` display rule (month mark, day
  rows), click opens the contact.
- **P7 profile picture** - embedded pictor in the reserved area.

## VI. AUDIENDA (open, not decided or not verified)

- **29 February** birthdays in non-leap years: show on 28 Feb or
  1 Mar?
- Name handling on kind toggle (III) - proposal only.
- Yearly entries in the date-source port: repeated by the source or by
  the port (P6).
- Unverified: `similitudo` on non-ASCII names (decision 12 defers it).
- Delete confirmation, merging duplicates, vCard import/export: later.
