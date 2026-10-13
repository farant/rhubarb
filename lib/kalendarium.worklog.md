# kalendarium worklog

## 2026-10-13 — A2a: month pane structure (agenda-plan A2)

Decisions (Fran 2026-10-13): one genus `calendarium` with a `visus`
field (mensis/dies/annus - views are alternatives, not nested children;
nesting stays for A6's embedded scriba); C prefix `kalendarium_`
because the liturgical library owns `calendarium_`; selection
TRANSIENT (every session opens on today), view mode persistent; weeks
start on Sunday.

Manifest apps/calendarium/organum.stml (embedded by
tools/organa_infigere.sh): binding `documentum` (agenda, from parent)
and port `hodie` declared - both wired by hand (gap 2). One action
`calendarium.agere` for keys and clicks (one writer per value field).
Forced fix after approval: `<porta modus>` admits only `petitio` |
`eventus` (organum.h) - "lectio" made the organ refuse to load (stderr
names it: "valor generi attributi non congruit: modus"); today is
asked for on demand = `petitio`.

`kalendarium_componere`: root "kalendarium" (focusable, the action) >
"titulus" + 42 day cells whose id IS the ISO date (a click reads the
date from the component id - no lookup table). Cell size from the
branch's superficies_* (absent: 280 x 256). `diem_scribere` sets the
writer to `calendarium.agere` and restores the previous one: calls from
outside the dispatcher (argumentum_ponere, mount) would otherwise be
refused silently by the owners. Month steps use `fasti_addere_menses`,
which already clamps (Jan 31 + 1 month = Feb 28).

vicus: `ContextusKalendarii` opens ONE agenda document per vicus volume
(namespace "agenda") lazily and hands it to every calendar pane; the
`hodie` port calls the vicus clock function itself (the source of the
root's tempus_* copies), so tests inject a fixed clock through
`vicus_horologium_ponere`. `$calendarium [YYYY-MM]` registered next to
$pictor/$terminale.

Tests: probatio_kalendarium (mount, grid, keys incl. the clamp, click,
argument, persistence: visus returns, selection does not) and
probatio_vicus_kalendarium (two panes, argument, clock port, shared
document). Plants: Monday-first grid, unshared document, dead clock
port - all red. ludus_tessera's hand list needed "agenda" and
"kalendarium" (vicus_applicatio links them).

## 2026-10-13 — A2b: drawing the month

Parts: root `PARTES_TABULA`, title `PARTES_TITULUS`, cells
`PARTES_ITEM` - no new Partes value (figures are per pane namespace, so
existing values cannot collide). Fran asked whether a liturgical-day
cache belonged in the liturgical library: it already caches the costly
part (movable-feast anchors per year, in its own pool); a day lookup is
cheap. So NO cache. First design (root computes into a per-frame
scratch the cells read) would have broken figura.h's purity rule
(figures write nothing but commands, lint L2) - final design (c): ONE
figure on the root draws background and all 42 cells, walking its
children (positions + ISO ids); cells have no figure; liturgy and
agenda marks computed once per frame. Measured: full month render with
1000 agenda items = 51 us.

Cell: number (other month dimmed, today inverted on the accent colour),
2 px liturgical colour bar (RGB table by ColorLiturgicus; "white" drawn
gold so it shows on the light theme), feast short title for memorials
and above or Sundays, cut to the cell width, marks (3x3 filled = open,
hollow = completed, 4 then '+'), selected = active border. Title:
"October 2026 - <season>" (season names come Latin from the library)
plus S M T W T F S over the columns.

Test bug found a LIBRARY bug: Nov 1 2026 (Sunday) comes back as
"Dominica XXXI per Annum", green - All Saints outranks Ordinary Time
Sundays. GRADUS_DOMINICA (V) > GRADUS_SOLLEMNITAS (IV) for every
Sunday. Parked in the ledger (parcum ...KC11SH), not fixed here. Colour
checks use unambiguous days (Oct 7/15 white memorials, Oct 19 red
memorial, Oct 20/21 green ferias). Plant: constant colour -> red.

Commit side-trip: the first A2b commit failed ludus_tessera at LINK time
- kalendarium now calls calendarium_liturgicum, and ludus_tessera's
hand-written library list (vicus_applicatio links kalendarium) lacked
it and sanctorale. Rule: when a library vicus links gains a NEW library
dependency (not only when a new lib file appears), run
./ludus_tessera/compile_probationes.sh before committing.

## 2026-10-13 — A2b screenshot: weekday letters were garbage

First headless screenshot (throwaway probe: vicus at 960x600, fixed
clock, calendar pane, agenda items; ludus_fenestra on a bare pixel
table) showed S M T W T F S as random glyphs. Cause: `mandata_textus`
stores the chorda BY REFERENCE; figura_tituli pointed each letter at a
stack buffer (`litterae + j`) that is gone when the rasterizer reads
it. Day numbers and the title were safe only because
`chorda_ex_literis` copies into the pool. Fix: `chorda_transcribere`
into `md->piscina`. Rule: any text handed to mandata must live in the
mandata pool (or static storage). Test `litterae_dierum_sunt` (letters
on the title's second line, y 8, read AFTER other calls reused the
stack): red before, green after. The probe source is kept in the
session scratchpad, not committed.

## 2026-10-13 — A2b after the screenshot (Fran's calls)

- Plain Sundays show no title: the library's short title "Dominica XXIX
  per Annum" cut to the cell read "Dominica X". Rule now: memorial or
  higher, except GRADUS_DOMINICA. (Solemnities on Sundays will show once
  the liturgical library ranks them properly - see below.)
- The liturgical colour moved from a 2 px edge bar (read as a divider
  between days) to a 5x5 square right after the day number.
- Test: the colour checks look for the 5x5 RGBA square; the Oct 18
  (Sunday) cell carries only its number. Red before, green after.

Liturgical library review (background agent, probe-verified): 15
findings - Sundays beat solemnities (132 dates 1970-2100), All Souls
never shown, movable celebrations unused, no transfers, Holy Week
colours, optional memorials recolouring weekdays (visible here: Oct 14
St Callistus paints the day red). Kept in
project-specs/liturgia-recensio/. Fran: the calendar is US; other
calendars later; this is the next project after the calendar widget.

## 2026-10-13 — A3a: day view structure and acts (agenda-plan A3)

Design approved by Fran: same pane, `visus="dies"`; Enter on a month
day opens it, Esc returns. List = items assigned to the day (id order)
then items completed that day but assigned elsewhere ("completed
here", decision 4) - an item assigned and completed the same day
appears once. Keys: j/k select (clamped), h/l day -+1, x tick/untick
(completion date = clock port's today), </> move the selected item a
day (the view stays; selection moves to the next row), X delete (undo
covers it), u / Ctrl-R undo/redo through the agenda's historia. New
transient field `res_electa` (absent or not in the list = first row).
Prompt keys (n, a, c, D) are A3b; drawing is A3c.

Implementation notes: state writes go through one `campum_scribere`
(writer set to calendarium.agere and restored). Each day-view action
computes the list in a scratch pool kept in the private context
(`figurae_ctx`, reset per action) - the mount pool would otherwise grow
per keystroke. Rows are components "r<id>" (PARTES_ITEM, the action) of
height (1 + notes) lines, so a click selects through the ordinary
dispatcher path. Plant: "completed here" group dropped -> red.

## 2026-10-13 — A3b: the one-line prompt (vim.h)

Fran: implement as envisioned, refine after testing. `n` (new item on
the shown day, selected after), `a` (note, dated today from the clock
port), `c` (rename, pre-filled), `D` (date, pre-filled with the item's
date; empty = unscheduled; a bad date is refused and the prompt stays
open). Enter commits, Esc cancels; while a prompt is open every key
goes to it (clicks are ignored). New transient fields `prompta`
(nulla/nova/nota/titulus/dies), `prompta_textus`, `prompta_cursor`,
written in ONE insula write (`GrexCamporum` mutator; an empty value
removes the attribute - the interner refuses "").

Mechanics: each key rebuilds a one-line TabulaCharacterum (256 wide)
from the island, starts vim.h in insert mode at the stored cursor, feeds
the key, reads line 0 back. Since vim state is rebuilt per key, vim's
"fd" escape can never fire. Findings:
- vim fills empty cells with SPACES, so "last non-empty cell" was always
  256. Length = max(last non-space cell, cursor) - a just-typed space
  survives until the next letter; trailing spaces are trimmed at Enter.
- `prompta_textus` lives in an attribute, so it is stored through
  scriba's codec (`scriba_effugere`/`scriba_solvere`: no raw `"` or
  control bytes). That makes kalendarium depend on scriba_documentum -
  candidate to move the codec into a neutral library. Plant: read the
  text undecoded -> red (by crash: the escaped form overflows the buffer).
- Test bugs fixed on the way: a second `D` typed into the prompt
  instead of clearing the pre-fill; after D moves item 5 away the next
  selected row is item 3 (same index rule as </>), not 1.

The tree gets a "prompta" component (PARTES_CAMPUS) on the pane's last
line while a prompt is open - drawing is A3c.

## 2026-10-13 — A3c: drawing the day view

Header (title figure, day branch): "Wednesday, October 14, 2026"; the
principal celebration's full title after its colour square; the season
dimmed. Rows (board figure, day branch, walking the "r<id>" children):
selection background, "[ ]" / "[x]" + title, items assigned elsewhere
dimmed with their own date ("(Oct 20)", or "unscheduled"), notes below
as "Oct 13  text". Prompt (new PARTES_CAMPUS figure): kind label
("new:", "note:", "title:", "date:"), the decoded text copied into the
mandata pool, a 1 px cursor bar.

Screenshot finding: notes in the SELECTED row were invisible - drawn in
the dimmed text colour, which nearly matches the selection background.
Now the selected row uses the full text colour (`color_notae`); test
`textus_coloris_est` + plant (always dim -> red). Test note: a figure
draws in its own component's space, so the prompt text is at local
y 1, not at the component's pane position - the check looks for the
text anywhere. A leftover temp volume from the A3b crash plant (purgare
never ran on the crash) was found in /tmp and removed.
