# comites — a location for every view, and its companion notebook

Status: decided with Fran 2026-10-22 (§VIII); implementation after
merging main. Latin names are proposals until the header text of
each phase is approved. Builds on routes (`semitae-spec.md`) and
the search arc (`indagatio.md`, D5 S3).

A vicus PLATFORM layer, not an app: every right-hand view gets a
companion notebook. Cmd+T swaps the left pane between the common book
and the companion of whatever is on the right; while it is shown it
follows the right-hand view, and each companion remembers its own
page. The key idea underneath is the view's LOCATION - what it is
showing, as opposed to how it is showing it - which a second layer
(drawing mode, §VII) will reuse.

## I. Doctrine

1. **Location is not route.** A route is everything a view can be put
   back into (page, selection, filter, notes page). A location is
   only WHAT the view shows: the contact, the day, the saint. Two
   views that show the same thing have the same location; a notes
   page turned inside a contact does not move its location.
2. **Normalized.** A location never contains a symbolic value
   ("today" files under the actual date) and is rounded to the
   view's own unit (the month view of October is one location
   whatever day is selected).
3. **Every app gets it for free.** A view that declares nothing has a
   default location (its binding); a view declares only when the
   default is wrong (rounding, terminals). Like search: opt-in by
   declaration, the platform does the rest.
4. **Separate form factors stay separate** (Fran 2026-10-22). Without
   scrolling, every notebook form is its own paginated thing; the
   companion of a calendar day is NOT the day notes embedded in the
   day view. Unifying them is not attempted.
5. **A companion keeps its own state.** Its page survives leaving
   and returning, like any other view.

## II. What exists today (cited)

- **Binding vs status** is already in the route model: fields are
  `semita="ligatio"` (which entity) or `semita="status"` (how)
  (`semitae-spec.md` §III). The URI LIST form "one URI per node with
  its own binding ... as projection keys later" exists:
  `semita_ad_uri_lista` (`include/semita.h:137`, `lib/semita.c:1023`).
- **Symbolic values** ("hodie") are resolved at the boundary by a
  per-view resolver: `semita_resolvere` (`include/semita.h:161`), the
  calendar's `diem_symbolicum_resolvere` (`lib/kalendarium.c`).
- **Notebooks are volume spaces**: `scriba_liber_aperire_in_spatio`
  (`include/scriba_liber.h:41`) - day notes `dies/<date>`, contact
  notes `persona/<id>`. The left pane is a scriba view of the common
  book (`lib/vicus_applicatio.c:3269`).
- **Switching a mounted scriba view to another book**:
  `scriba_librum_ponere` (`include/scriba_applicatio.h:106`); to a
  named page: `scriba_paginam_ostendere` (`:114`).
- **The viewed page is remembered per scriba view id**, plagula
  `scriba/visus/<id>` (`lib/scriba_applicatio.c:158`) - per VIEW, not
  per book; companions need it per book (§IV).
- **The left pane already has route state and history**: scriba's
  page is `scriba/<pagina>`, left-side Ctrl+O (7c51967b).
- **Host shortcuts**: `vicus_brevitatem_addere` / `vicus_advocare`
  (`include/vicus.h`, e818acdf). Cmd+T is unused (grep of `lib/`).
- **tmux windows**: the multiplexor already reads `#{window_index}`
  per tmux pane (`lib/multiplexor.c:15`); colloquia maps a terminal
  pane to its tmux client by tty (`lib/vicus_applicatio.c`,
  `latus_adhaerens`).
- **Not existing**: view relations (`semitae-spec.md` §VI-b) have no
  grammar, loader or host binding yet.

## III. The model

```
right-hand view (pane genus + route)
  | locus: binding fields only, symbols resolved, rounded per
  |        declaration; children dropped              (declared/default)
"nomenclator/1", "calendarium/mensis/2026-10", "indagatio"
  | companion book space: "comes/" + escaped locus
scriba book in the volume (pages, own viewed page)
  | Cmd+T: left pane shows it (bar: "Notes · Anna Rossi"), follows
```

- **Locus (location)**: a canonical string per view, derived from its
  current route. Default: the root's `ligatio` values (the first URI
  of `semita_ad_uri_lista`, symbols resolved). Nested children never
  count (a contact's notes page is not part of the contact's
  location).
- **Declared rounding** (calendar): a view may declare how its
  location is formed per view mode - day view by day, week view by
  week start, month view by month, agenda as one. Fran 2026-10-22.
- **Terminals** have no routes: one companion per terminal pane, and
  per tmux WINDOW when the pane is attached to tmux (Fran
  2026-10-22).
- **Search**: one companion for the search pane as a whole (its `q`
  is status, not binding). Saved searches may get their own later.
- **Title**: the bar shows a human label of the location
  ("Notes · Anna Rossi", "Notes · Mon 19 Oct 2026"); a view supplies
  it (most already compute such titles for search), fallback the
  locus string.

## IV. Behaviour

- **Cmd+T** (host shortcut): left pane common book -> companion of
  the right-hand front view; again -> back to the common book on the
  page it was on.
- **Follow mode**: while the companion is shown, a change of the
  right-hand view's location (another contact, another day, another
  pane in front) switches the left pane to that location's companion.
- **Per-book page**: each companion opens on the page it was last on
  (the viewed page is stored per BOOK for companions, not per view
  id).
- **Bar**: a strip at the top of the left pane while a companion is
  shown, with the title; visually distinct from the common book.
- **History** (Fran 2026-10-22, after P3): every book has its OWN
  left-side history - Ctrl+O in a companion moves only through that
  companion's pages, the common book keeps its own, and switching books
  (Cmd+T, follow) is not a history step. Links (#tags, page numbers)
  are local to a book in the same way.
- **Search**: companions become a searchable kind later (not in the
  first phases).

## V. Where it lives

| Piece | Place |
|---|---|
| Locus derivation (binding, resolve, round) | `lib/semita.c` / `include/semita.h` (route library) |
| Declarations (rounding, label) | manifests (`apps/*/organum.stml`), via the first slice of view relations (§VI-b) or a sibling element - decided with the P1 header text |
| Companion book space, per-book page | `lib/scriba_liber.c` / `lib/scriba_applicatio.c` |
| Cmd+T, follow, bar | `lib/vicus.c` (host), `lib/vicus_applicatio.c` (wiring), scriba status line |
| Terminal / tmux location | `lib/vicus_applicatio.c` (terminal genus) + multiplexor |

## VI. Phases

| Phase | Content |
|---|---|
| **P1** | Locus: derivation from a route (default + declared rounding + symbol resolution), view label; calendar declares its rounding. Tests on the route library and the calendar. DONE 2026-10-22: default `semita_locus` (binding fields of the current state); views override in CODE via `facies.locus` / `facies.locus_titulus` (the calendar's absent day means today and weeks start on Sunday - not expressible as manifest rounding). |
| **P2** | Companion books: space per locus, per-book viewed page, book in scriba's route (left history). DONE 2026-10-22: scriba field `liber` (status, impelle; `scriba/2?liber=...`), `scriba_librum_praefixi_ponere`, host book lookup, per-book page `<prefix>/visus`. |
| **P3** | Cmd+T, follow mode, bar - Fran's look. Implemented 2026-10-22: Cmd+T host key, facies.comitem_ponere / comes_currens, follow without extra state (left book prefix "comes/..."), bar = scriba `vectis` from transient `liber_titulus`. |
| **P4** | Terminals: per pane, per tmux window when attached. |
| **P5** | View relations slice: `quaestio` (Cmd+F) - the S4a work, if still wanted after P1 settles where declarations live. |
| later | Companions searchable; drawing mode (§VII); saints as a search kind (D5 S4b). |

## VII. Future consumer: drawing mode (Fran 2026-10-22)

Any right-hand pane can switch into DRAWING mode: the UI stops being
interactive and becomes a layer; pictor-like layers below and above
the UI layer (stickers, doodles) are persisted and come back when the
view returns to the same place. "The same place" is the locus of §III
- so P1 must not be companion-specific: the locus is a vicus
primitive with two consumers.

## VIII. Decisions (Fran, 2026-10-22)

1. Notes per view LOCATION, normalized ("today" -> the date).
2. Companions are separate from embedded notes (form factors stay
   separate).
3. Follow mode; each companion keeps its own page.
4. Cmd+T toggles; back returns to the common book's previous page.
5. Bar at the top distinguishes a companion; title = human label of
   the location.
6. Search pane: one companion; saved searches later.
7. Calendar: rounded per view unit (day / week / month / agenda).
8. Terminal: per pane, per tmux window when attached.
9. Locus is a platform layer reused by a future drawing mode.
10. Notifications and sessions: one companion each (the pane as a
    whole).
11. Picture editor: one companion per picture.
12. Names: `comes` (companion notebook), `locus` (location); vicus's
    internal `LocusTabulae` gets renamed.

## IX. Open questions for Fran

None at the spec level; header text and names come with each phase.

## X. AUDIENDA

- Not verified: that the first URI of `semita_ad_uri_lista` is exactly
  the binding-only form for every current view (calendar, contacts,
  saints, picture editor, search); P1 tests it per view.
- Not verified: how a terminal pane maps to its tmux WINDOW (colloquia
  maps pane -> client by tty; window id per client not yet read).
- Not measured: cost of deriving a locus on every route change
  (expected trivial: one route read).
- Not designed: what happens to companions when an entity is deleted
  (a deleted contact's companion book stays; orphan listing later).
