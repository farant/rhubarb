# pensa — a daily checklist on natural-language schedules

Status: designed with Fran 2026-10-23 (brainstorm, three sections
approved in chat); implementation in phases K1–K5 (§VII), each phase's
header text approved before code. Latin names are proposals until
then. Builds on the calendar (`kalendarium`, agenda-plan), scriba
books (`scriba_liber`) and the volume's act log.

A **daily checklist**: a template of items that appear every day as
checkable rows on the calendar's day view. Each item belongs to a
**schedule group** — "every day", "weekends", "every other monday from
oct 26 2026" — so the checklist is really the first client of a
**natural-language schedule library**, the one decided on 2026-10-14
(nomenclator-plan decision XV: "a recurring scheduled date data type …
from which clients extract the concrete dates in a range"). The
checklist is the first section of a future **Reminders** area
(particular days, times); it is the only section for now.

## I. Doctrine

1. **Schedules are deterministic C.** No AI, no guessing. A phrase the
   parser does not know is an error with a helpful message, never a
   best effort. The parser is expected to accrue special cases over
   the years ("oh, can it parse this too?"); every accepted phrase is
   a recorded example (§III.4), so growth never silently changes what
   an older phrase means.
2. **The text is the source of truth.** The checklist template is an
   outline the user writes; schedule phrases are stored exactly as
   typed and parsed on every load. Nothing about a schedule lives
   outside its phrase — hence anchors carry their year (§III.3).
3. **Parser and rule are separate layers.** phrase → parser → rule →
   days. The rule API (§III.1) is the part to get right first; the
   parser behind it may sprawl.
4. **Ticks are history.** A tick is an act in the volume, keyed by day
   and item text. The log is append-only, so past days show what was
   and was not done, for free.

## II. Pieces

| unit | what | depends on |
|---|---|---|
| `recursus` (lib, new) | phrase -> opaque rule; days in a range; contains a day; next occurrence; canonical text | `fasti` only (pure: no volume, no clock) |
| `pensa` (lib, new) | outline text -> groups (schedule + items, parse errors with line numbers); items for a day; tick / untick acts; ticked? | `recursus`, `scriba_liber` (reading the book), `volumen` |
| `kalendarium` (+ manifest) | day view Checklist block; Reminders tab (`visus = monita`) with read and edit views | `pensa` |

`recursus` is reusable: birthdays (`fasti_fontes` annual expansion,
"in a shape the recurrence type can later replace") and recurring
agenda items (agenda-plan decision 9) are its next clients — parked
(§VIII).

## III. The schedule — `recursus`

### III.1 API (opaque rule)

```c
nomen structura Recursus Recursus;   /* opacus: interior crescere potest */

/* phrasis -> regula (in piscina); NIHIL et *vitium (Anglice, cum
 * exemplo correcto ubi potest) si non intellegitur */
Recursus* recursus_legere   (chorda phrasis, Piscina* piscina,
                             chorda* vitium);
b32       recursus_continet (constans Recursus* r, Dies d);
/* dies [prima, ultima] (inclusive) ordine; numerus additorum */
i32       recursus_dies     (constans Recursus* r, Dies prima,
                             Dies ultima, Xar* exitus);
/* dies primus >= ab; FALSUM si nullus (semel praeteritum) */
b32       recursus_proximus (constans Recursus* r, Dies ab,
                             Dies* exitus);
/* forma canonica Anglica ("Every other Monday from Mon 26 Oct 2026") */
chorda    recursus_scribere (constans Recursus* r, Piscina* piscina);
```

Opaque on purpose: unions ("mondays and the 1st"), exceptions and end
dates can arrive later without changing callers. No clock inside —
the library never reads "today"; callers pass days.

### III.2 Seed grammar

Case-insensitive; singular or plural weekday names; common
abbreviations (`mon`, `tue`/`tues`, …, `oct`, `october`); `,` `and`
`&` as separators.

| you write | canonical |
|---|---|
| `every day`, `daily`, `each day` | Every day |
| `weekdays` | Weekdays |
| `weekends` | Weekends |
| `mondays`, `every tuesday`, `mon, wed, fri`, `tuesdays and thursdays` | Mondays, Wednesdays and Fridays |
| `every other monday from oct 26 2026` | Every other Monday from Mon 26 Oct 2026 |
| `every 2 weeks on monday from 2026-10-26` | Every other Monday from Mon 26 Oct 2026 |
| `every 3 days from 2026-10-23` | Every 3 days from Fri 23 Oct 2026 |
| `the 1st of the month`, `monthly on the 15th` | The 1st of each month |
| `last day of the month` | The last day of each month |
| `first monday of the month`, `last friday of the month` | The first Monday of each month |
| `every year on oct 14`, `yearly on october 14` | Every year on 14 October |
| `2026-10-30`, `oct 30 2026` | Once, Fri 30 Oct 2026 |

Rules: the 29th–31st of a month skip months that lack them (no
spill-over); "every year on feb 29" falls only in leap years; weeks
for "every N weeks" are counted from the anchor's week (weeks start on
Sunday, as the calendar's).

### III.3 Anchors carry their year

"Every other Monday" needs to know which Mondays. Because the text is
the truth (I.2), the anchor must be **in the phrase**, **with its
year**: `every other monday from oct 26 2026`. Without an anchor, or
without a year, the error suggests the fix:
`every other monday: from which day? add "from" and a date with its
year`. Rejected: a hidden fixed epoch (silently picks your
weeks; invisible and unchangeable) and "the next oct 27" (the meaning
would drift as time passes). An anchor that is not itself a member
(`every other monday from oct 27 2026` - a Tuesday: 2026-10-19 is a
Monday) is an error: `... : Tue 27 Oct 2026 is not a Monday`. (The
first draft of this spec used oct 27 as its Monday example - caught
while computing the corpus with an independent calendar.)

### III.4 The corpus is the test suite

`lib/recursus.corpus.stml`: one element per phrase —
the phrase, its canonical form, a window and the exact days it must
produce; rejected phrases with their expected error. Teaching the
parser a new format = add a line, see it fail, make it pass. The
corpus also documents what the parser understands. Every canonical
form must parse back to the same canonical form and the same days
(round trip), so what the read view shows can always be typed back.

```xml
<exemplum phrasis="every other monday from oct 26 2026"
          canon="Every other Monday from Mon 26 Oct 2026"
          ab="2026-10-20" ad="2026-11-30"
          dies="2026-10-27 2026-11-10 2026-11-24"/>
<vitium phrasis="every other monday" incipit="every other monday: from which Monday?"/>
```

## IV. The checklist — `pensa`

### IV.1 Outline format (revised with Fran 2026-10-23, K2)

```
vitamins                 <- every day (no children)
morning offering         <- every day

weekends                 <- schedule (it has children)
    mow the lawn
    change the sheets

    every other monday from oct 26 2026   <- the section as a whole is
        call mom                             indented: dedent -> heading
```

- **Sections are separated by blank lines** (a page break of the book
  counts as one).
- **Each section is dedented on its own**: its common leading
  indentation is removed; only RELATIVE indentation matters.
- **Inside a section**, a top-level line WITH more-indented lines under
  it is a **schedule heading** (parsed by `recursus`) and those lines
  are its items, at any depth (two logical levels); a top-level line
  WITHOUT children is an **every-day item** and is never parsed as a
  schedule. Consecutive childless lines form one implicit "every day"
  group, so the groups keep the text's order.
- A heading that does not parse keeps its items but contributes none to
  any day; the read view shows the error under it (nothing silently
  disappears; a misspelled heading still has children, so it is still
  a heading).
- **Item identity is its text** (trimmed). Renaming an item starts a
  fresh history; past days keep their ticks under the old text. The
  same text in two groups that apply on the same day is shown once.

### IV.2 Storage

- **Template**: an ordinary scriba book in the volume, prefix `pensa`
  (pages `pensa/paginae/...`). Editing, undo, history and pagination
  are scriba's. The outline is the book's pages read in order WITH their
  blank lines (they separate sections): `scriba_liber_textum_legere`,
  added in K2 - the older readers drop blank lines (written for search).
- **Ticks**: acts in the volume, genus `pensa`:
  `<pensum actio="factum" dies="2026-10-23">vitamins</pensum>` and
  `actio="infectum"` to untick (last act per day + item wins). The item
  is text CONTENT, not an attribute (attribute values are raw in STML;
  `R&D "notes"` must round-trip).
  Rejected: `[x]` marks inside the template text (a daily list would
  rewrite the template every day and mix template with history).

### IV.3 API sketch (names proposals)

```c
/* forma: greges (schedula + res), vitia cum lineis */
PensaForma* pensa_formam_legere (Volumen*, InternamentumChorda*,
                                 Piscina*);
/* res diei d (ordine formae, sine iteratis) */
Xar*        pensa_res_diei      (constans PensaForma*, Dies d,
                                 Piscina*);
b32         pensa_factum        (PensaDocumentum*, Dies d, chorda res);
b32         pensa_ponere        (PensaDocumentum*, Dies d, chorda res,
                                 b32 factum);   /* actum, si mutatur */
```

## V. Calendar UI

### V.1 Day view: Checklist block

- After the day's agenda items: a **Checklist** heading and rows
  `☐ vitamins` / `☑ vitamins` — only groups whose schedule includes
  the day.
- `j`/`k` move through agenda items and checklist rows alike; `x` on a
  checklist row ticks / unticks (as it completes an agenda item);
  clicking the box does the same.
- **Today and past days are tickable**; **future days** show their
  items dimmed and not tickable (planning view). Past days show what
  was and was not ticked (history).
- Pagination: the day view's existing `[` / `]` pages cover the block.

### V.2 Reminders tab (`visus = monita`, after Agenda)

One section for now: **Daily checklist**.

- **Read view** (paginated): each heading as typed; a dim note — "today"
  when the group applies today, else its next date ("next: Sat 25
  Oct") via `recursus_proximus`; items beneath; an error line under a
  heading that does not parse.
- **Edit view**: `e` mounts a scriba on the `pensa` book (paginated as
  any scriba book). Leaving it works like leaving the day-notes editor
  (exact key confirmed against that code in K5); back in the read
  view the outline is re-parsed.
- Route: `visus` gains option `monita`; the read/edit mode and page are
  status fields (names in K4's header text).

## VI. Decisions (Fran 2026-10-23)

1. Deterministic C parser, grows by special cases; corpus records each.
2. Each item has a schedule; default every day; groups = schedule ->
   items (nested list); the group's title is its phrase as typed.
3. Template edited as an outline text in an embedded scriba (edit
   view); a polished read-only rendered view; both paginated.
4. Ticking: today and past days; future days visible, not tickable.
5. Anchors in the phrase, with year (§III.3).
6. Reminders tab after Agenda; the checklist is its only section now.

## VII. Phases

| | |
|---|---|
| **K1** | `recursus` + corpus: seed grammar (§III.2), canonical form, errors, anchors. DONE 2026-10-23: 62-entry corpus with round trips, API edge tests; see lib/recursus.worklog.md |
| **K2** | `pensa`: outline parsing (§IV.1), items for a day, tick/untick acts. DONE 2026-10-23: `scriba_liber_textum_legere` (book text with blank lines), outline parser + items for a day, ticks as acts read before every query; see lib/pensa.worklog.md |
| **K3** | Day view Checklist block (show, tick, history, future dimmed). DONE 2026-10-23: "k"/"k<n>" rows between to-dos and Completed, one day selection sequence (`electiones_diei`), tick via the complete command (today/past only), drawing + vicus end-to-end; pensa scratch pool for per-frame queries; see lib/kalendarium.worklog.md |
| **K4** | Reminders tab read view |
| **K5** | Edit view (embedded scriba on the `pensa` book) |

Each phase: header text approved first, tests red, implementation,
plants, worklog, commit on approval.

## VIII. Parked

- Reminders for particular times; one-off reminders on a day outside
  the checklist.
- Birthdays (`fasti_fontes`) and recurring agenda items onto
  `recursus` (decision XV's other clients).
- Checklist items in search (`indagatio` kind).
- Unions/exceptions/end dates in `recursus` ("mondays except holidays",
  "until dec 2026").
- Phrases in Latin ("dominica", "feria secunda").

## AUDIENDA

- The day-notes editor's exit key was not checked yet (K5).
- (verified 2026-10-23) `scriba_liber_lineas_legere` keeps leading
  whitespace (`lineam_legere`, lib/scriba_liber.c: cells 0..end of
  content, tabs/continuations as spaces) and drops blank lines — fits
  the outline (blank lines are ignored anyway).
