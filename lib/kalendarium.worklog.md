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

## 2026-10-13 — A4a: the agenda (todo) view

Fran chose (b): no separate genus - `visus="agenda"` on the calendarium
pane, opened by argument "agenda"; vicus registers `$agenda` as a
fixed-argument alias (`aperiens_fixum_registrare`, ContextusAperiendi
.argumentum_fixum). Enter -> day view deferred.

agenda.h gained `agenda_apertae` (live incomplete items, any day,
unscheduled included) and `agenda_rubricae` (distinct rubrics of live
items, canonical order). Groups (`greges_agendae`): rubrics in order
(filter: that one only), then items without rubrics (not under a
filter); within a group by date, unscheduled last, then id; empty groups
skipped. An item with two rubrics appears in both, so row ids carry the
group: "r<id>-<k>" (no ':' - component ids may not contain it, S8c);
group headings "g<k>" (PARTES_INDEX). j/k skip a repeated occurrence of
the same item. The day-view code was generalized to "the current list"
(`ordines_currentes`): h/l and </> only in the day view; Esc in the
agenda view clears the filter (A4b) or does nothing; `n` creates an
UNSCHEDULED item carrying the filter rubric; `x` refreshes the selection
in the agenda view (a completed item leaves the list).

Manifest correction after approval: `rubrica_filtrum` had to be
`typus="textus"`, not `nomen` - canon `nomen` refuses '-', and rubrics
are kebab-case. Also rewrote the j/k step with s32: it used i32 (which
is UNSIGNED here) and only worked through casts.

Tests: probatio_agenda `apertae` (queries), probatio_kalendarium
`visus_agendae` (grouping, skip, complete, create), probatio_vicus_
kalendarium `agenda` (typed and clicked '$agenda' in a scriba page).
Plant: no "(none)" group -> red.

## 2026-10-13 — A4b: rubric prompt and filter

`r` (day and agenda views - same prompt, so the day view got it too)
opens the prompt pre-filled with the item's rubrics ("domus opus");
Enter goes through `agenda_rubricas_ponere`, which validates (" Bad"
refused, prompt stays open). `f` (agenda view) opens a prompt with the
current filter; empty removes it, otherwise the name must be kebab
([a-z0-9-]+, checked locally - agenda's own validator is private). The
filter commit is handled before the "needs a selected item" check, so it
works on an empty list. Under a filter `n` gives the new item that
rubric; Esc on the list clears the filter; the filter is persistent
(survives a remount through conservatio - tested). Test bug fixed: after
`r` the order is r1 r5 r6 r5, so one `k` from item 6 reaches item 5.
Plant: filter rubric not applied to `n` -> red.

## 2026-10-13 — A4c: drawing the agenda view

`ordines_pingere` now serves both list views instead of a copy: ids are
parsed by `numerus_identitatis` (one-letter prefix, digits up to '-'),
so "r5-0" and "g2" both work - the old parse took the whole tail and
"r5-0" failed silently (no row drawn). In the agenda view nothing is
"elsewhere" (no dimming, no "(Oct 20)" suffix); the date goes at the
right in its own column ("-" when unscheduled) and the title is cut a
full letter short of it; notes only under the selected item (the
composer already gave only that row the extra height). Group headings
("#rubric" / "(none)", dim) are drawn by `greges_pingere` from the same
`greges_agendae` the composer used, indexed by the "g<k>" suffix.

Header: "Agenda: n open" counts DISTINCT items in the current list (an
item in two rubrics is one), so under a filter it is the filter's count;
"  #rubric" appended when filtered.

Observed in the screenshot, left as is: an item in two groups is
selected (fill + notes) in BOTH places, since the selection is the item
id, not the row. It reads as "this is the same thing" and costs nothing;
revisit if it feels noisy in use. Right margin for the date is 8px (4px
looked cramped against the pane edge).

Plant: agenda branch of `figura_tituli` disabled -> red at the header
assertion.

## 2026-10-13 — mensa: focus border like scriba and pictor

Fran asked for the lighter inset border that changes texture on focus.
Scriba and pictor do it with a "mensa": a component filling the pane in
COLOR_SUPERFICIES, titled "focatum" when `motus->spatium == ramus.via`,
whose figure overlays EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA; the page sits
one cell in. The calendarium had no wrapper component, so the root
plays the mensa: it keeps the full pane size, the views are composed
for the inner area (pane minus 6px/8px each side - one scriba cell) and
`liberos_movere` shifts every child by the margin afterwards. The root
figure paints surface, focus pattern, then COLOR_BACKGROUND inset. No
tree change: ids, actions and hit-testing are as before, all old tests
green unchanged. Month composition moved into `mensem_componere` so all
three views share the one exit.

`focatum_est` and the EXEMPLAR_FOCI macro are now copied THREE times
(scriba_figurae/componentia, pictor_figurae/componentia, here). Rule of
three: a candidate for a shared home (exemplaria.h or a small "mensa"
helper in the widget layer) - asked Fran, not done.

Plant: focatum never set -> red.

## 2026-10-13 — mensa helper shared (figura.h, motus.h)

The three copies (scriba, pictor, here) of the focus check and the mensa
painting are gone: `motus_ramus_focatus(motus, ramus)` in motus.h and
`figura_mensam_pingere(md, fines, focatum)` + `FIGURA_EXEMPLAR_FOCI` in
figura.h (Fran approved option (a): mensa only). `color_thematis` is
still copied in five files (scriba, pictor, kalendarium, terminale,
vicus) - its own follow-up commit, since terminale.c brings the
aemulator gates and an install refresh.

## 2026-10-13 — command table (TABULA_MANDATORUM)

The key switches in agere_mensem / agere_diem became a table of 24 named
commands: titulus ("calendarium.perficere"), descriptio (English UI
label), clavis ("x", "Enter", "Esc", "C-r"), visus mask (MENSIS / DIEI /
AGENDAE), requisita mask (REQUIRIT_REM selected item, REQUIRIT_FILTRUM
active filter) and a `fac_*` handler taking a ContextusMandati computed
once per event (day, current list, selected id/index/item, scratch
pool). Availability is DATA (two masks), not a per-command function -
every current command fits, which is what a palette wants ("what is
available now" = filter over rows). Same key in different views = row
per view (j = next week in the month, next item in lists); h/l are one
command valid in month AND day.

Consumption rule made explicit: a key the view owns but whose command
is not available is swallowed, EXCEPT Esc, which passes to the host
(go-up). That reproduces the old behaviour exactly; the one intentional
change: letter keys no longer fire with Cmd or Ctrl held - Cmd chords
also arrive as keys (memory fenestra-cmd-keys), so Cmd+X completed the
selected item. Test `mandata` (red first). Side effects of the masks:
'<'/'>' in the agenda view are no longer swallowed (they did nothing).

`mandata_verificare` runs at mount: every row complete, no key twice in
a shared view; a broken table refuses the mount so no test can pass
over it. Plant: "Previous item" on 'j' -> mount refused, red in
probatio_kalendarium and probatio_vicus_kalendarium.

Mouse stays outside (ictus_mensis / ictus_ordinis) until mouse batch 1
maps gestures to command titles; the prompt stays a text editor.

## 2026-10-13 — M1a: invoke by name, double-click, checkbox click

`mandatum_invocare(m, titulus, p)` runs a table command by name (view
mask + requirements checked); the key path and it share
`contextum_parare` + `requisita_impleta`. Rows may now have clavis NIHIL
(mouse/palette only - none yet, M1b adds the view commands), and
`mandata_verificare` also requires unique titles since names are now
addresses.

Gestures: EVENTUS_MUS_DUPLEX arrives AFTER an ordinary press at the same
spot (derivare), so a double-click on a month cell = the press selects,
the duplex runs calendarium.diem_aperire. A press in a list row first
selects (ictus_ordinis), then, if `des->punctum_locale` is on the
checkbox (first line, x < KALENDARIUM_CAPSA = 24px: 4px pad + "[ ]" +
slack), runs calendarium.perficere - the fresh context reads the
selection just written. Wheel dropped from M1 (Fran: a trackpad swipe on
the MacBook; later, with pagination).
Plants: checkbox branch off -> red; duplex branch off -> red.

## 2026-10-13 — M1b: header buttons (toolbar row)

GLOBULI table: id, label, views where shown, own view (highlight),
group (left/right), command in the month vs in the other views. `<`/`>`
= mensis_prior/proximus in the month, dies_prior/proximus in the day
view; Today = calendarium.hodie (now valid in the day view too, so `t`
works there); Month/Day/Agenda = new KEYLESS commands
calendarium.visus_{mensis,diei,agendae}. A press on a PARTES_BOTTONE
child runs `globulum_premere` -> mandatum_invocare. Figure: label
centred, own-view button on COLOR_SELECTION, others COLOR_ACCENT_PRIMARY
(low contrast on the tan theme - tune after use).

DEVIATION from the approved design (buttons at the right end of the
title's first line): the test pane (280px default) left the title 7
letters, and any narrow vicus pane would too. Buttons now have their own
row (KALENDARIUM_BARRA = 14px) above every view: left group `< > Today`,
right group `Month Day Agenda`; views are composed 14px shorter and
moved down. Titles keep their full width at any pane size.
Gotcha: a button figure draws in the BUTTON's own space, so
`textus_in_continet(md, fines_of_button, ...)` (root-space check) does
not see its label - use textus_ullus_continet.
Plant: button press inert -> red.

## 2026-10-13 — V0b: layout on dispositio

The composer builds a dispositio tree in cells (6x8): root column with a
one-cell padding (the mensa margin) > barra row (2: left group, grow
spacer, right group) > titulus (2; day view 4) > corpus (grow: month =
6 grow rows x 7 grow cells, remainders spread evenly; lists = rows of 2
cells + 1 per visible note; headings 2) > prompta (2, only while open).
Each component is created at its leaf (`componentem_locare`) and gets its
bounds after `dispositio_computare` via `dispositio_ad_pixela`; children
are still added to the root in the old order (toolbar buttons last), ids
and actions unchanged. Gone: liberos_movere, KALENDARIUM_BARRA, the
pixel title heights, globuli_latitudo, all `y +=` arithmetic.

Visible: rows 10 -> 16px (text now at +4, centred), toolbar 14 -> 16,
day title 28 -> 32. No test needed changing - they assert relations
(text inside a component's bounds, fills at a row's y), not pixel
constants. The pixel remainder of a pane that is not a cell multiple now
lands in the right/bottom margin (content never touches the edge).
Plant: root left padding 0 -> red (mensa section).

## 2026-10-13 — index_rei signed

`index_rei` returned `(i32)-I` (i32 is UNSIGNED) as "absent"; examen
flagged it (domesticum: sentinella negativa in functione insignata) once
contextum_parare made the flow visible. Now s32 with -1; ContextusMandati
.index and electionem_reficere take s32, and electionem_reficere keeps
its old meaning explicitly: index -1 or past the end = last row.

## 2026-10-13 — V2a: toolbar and prompt on the V1 helpers

Toolbar: `<` `>` = figura_sagittam_pingere, Today = globulus, Month /
Day / Agenda = figura_segmentum_pingere (PRIMUM / MEDIUM / ULTIMUM by
the button's own view), all with FIGURA_UMBRA; the current view's
segment ELECTUM (purple). Left group has a one-cell gap (its own
DispositioForma with intervallum); the right group stays contiguous so
the segments join. The toolbar row grew to 3 cells (buttons 2, top
aligned) - at 2 the button shadows touched the title text.
Prompt: kind label in dim text, then an input field (figura_campum_
pingere) with the text and a 2px COLOR_CURSOR bar. Fixed on the way: the
label only knew new/note/title and fell through to "date:" for the
rubric and filter prompts - now "rubrics:" / "filter:" (test).
Test change: "note: abc" as one string became "note:" + "abc" (label and
field are drawn separately now).
Plant: left arrow drawn as a plain button -> red (sagittas_numerare).

## 2026-10-13 — V2b: lists on the V1 helpers

Rows: drawn checkbox (figura_capsam_pingere, 9x9 at x+8, y+3) instead
of "[ ]"/"[x]" text; title at x+22; completed items dim with a 1px
strikethrough over the title part only (not the "(Oct 20)" suffix),
clipped to the visible width; selected row = figura_ordinem_electum_
pingere (purple + 3px green accent bar), the others a 1px COLOR_
SUPERFICIES separator at the bottom; notes aligned with the title
(x+22). Agenda headings: figura_fasciam_pingere band + bold text.
The checkbox click area (x < 24) still covers the drawn box (8..17).
Tests: "[ ]"/"[x]" text checks became `capsa_in` (outlined 9x9 box,
tick = >= 2 lines of thickness 2 inside the row); new `linea_transversa_
in`, `accentus_in`, `fascia_in`. Plant: tick never drawn -> red.

## 2026-10-13 — V2c: month grid on the V1 helpers

Weekday letters left the title: a 2-cell band row at the top of the
month body (layout node without a component), painted by the TABLE
figure as figura_fasciam_pingere with each letter centred over its REAL
first-week cell. Since V0b the columns are grow cells with the remainder
spread (6 or 7 cells wide in the 44-cell test pane), so the old
title-width / 7 spacing drifted off the columns; the plant "uniform
spacing" turns `litterae_dierum_sunt` red, so the test sees exactly that
bug. Cells: 1px grid line = outline 1px PAST the bounds (shared with the
neighbour, never doubled); outside-month days PUNCTA_DISPERSA; today =
dark badge (COLOR_TEXT, specimen); selected = DARK double border, not
purple - COLOR_SELECTION and the grid's COLOR_TEXT_DIM are the same
purple in the theme, so a purple selection differed from the grid only
by thickness (seen in the first screenshot).
Tests: today ACCENT_PRIMARY -> TEXT, selection BORDER_ACTIVE (= cell
bounds) -> TEXT (bounds + 1); new: band above the first week, outside
pattern, grid outline.

## 2026-10-13 — V3c: titles in the 8x16 header font

Month: "October 2026" in FONS_TITULI (8x16) with the liturgical season
small and dim beside it (6x8, lower half); title row 2 -> 3 cells. Day:
the full date in 8x16, colour square + celebration and the season below
in 6x8; row 4 -> 5 cells. Agenda: "Agenda: n open  #filter" in 8x16;
row 2 -> 3. Truncation per font: `praecidere_fonte` divides by
fons_latitudo(fons) (8 for the header font). Test helper
`textus_fontis_est` checks the text command's `fons`.
Plant: month title drawn with FONS_PARVUS -> red.

## 2026-10-13 — A6a: Completed section in the agenda view

Fran: the agenda is a paginated view of the WHOLE todo history, so all
completed items show, below the open groups: "p" heading (band, bold
"Completed"), one "p<k>" sub-heading per completion day (dim
"Wednesday, Oct 14"), rows "q<id>" (ticked, strikethrough, assigned date
on the right). `perfectae_agendae` applies the rubric filter;
`ordines_currentes` = open groups (`apertae_agendae`) + completed, so
j/k and selection cross into the history; `x` on a q-row reopens it and
it moves back into its group. The header counts open items only (it
counted ordines_currentes before - would have included the history).
Click handling and row drawing accept 'q' ids. Pagination (A6b) is what
keeps a long history on screen.

## 2026-10-13 — A6b: paginated lists, day list regrouped, + New

Row model `OrdoIndicis` (id, partes, height in cells, item id) built by
`ordines_indicis` for BOTH list views (day: open "r<id>", "c", completed
"r<id>"; agenda: g/r, p/p<k>/q). `paginas_computare(rows, capacity)`:
a row that does not fit starts a page; a heading never ends a page (if
it and the next row do not both fit, the heading moves); an oversize row
sits alone. `capacitas_indicis` repeats the composer's arithmetic in
cells (pane / 8 - padding 2 - toolbar 3 - title - prompt 2 if open; day:
half the body, dispositio PARS floors; minus the 2-cell header) - pure,
so keys, figures and the composer agree on page breaks without layout.
`indicem_disponere` replaced diem_/agendam_disponere: header row "caput"
("To do" in the day view), [+ New], gap, [<] "paginae" "k/n" [>], then
only the current page's rows. Day view: the body splits PARS 50 / grow;
the lower half is empty until A6d.
State: manifest field `pagina` (transient). Writes that change the list
reset it in the SAME act (campos_duos_scribere: day, visus via
`visum_ponere` - all 7 writers - filter). `electionem_scribere` moves to
the item's page (current page preferred for agenda duplicates), so j/k
cross pages; page turns ([ ] / buttons, `paginam_vertere`) select the
first item of the new page. Arrows dim (FIGURA_OBSCURUM) at the ends.
Day order is now open first, then completed today (anywhere), then
assigned here but done elsewhere ("done Oct 16", right column). The
`prompta` test's position expectation changed with the order (1 5 2 3).
Bugs caught in review, not by the first tests: the counter text was a
stack buffer passed by reference (A3 class) - now transcribed; the
prior/next arrow test `id[VIII] == 'r'` was true for BOTH ids - new test
counts dimmed arrows (1 on the first page, 0 in the middle).
Plant: capacity huge (no paging) -> red.

## 2026-10-13 — A6d-1: the day's notes (nested scriba) in the lower half

With a volume (new `kalendarium_montare` parameter; vicus passes its
own), the calendar mounts ONE nested scriba "<id>_notae" under its
branch (scriba_montare_in_ramo) on the selected day's book
("dies/<ISO>") and merges its action and figure registries into its own
under the scriba's path (vicus then merges the calendar's; nested
entries keep their absolute path - S8c-2). It is kept for the pane's
lifetime; a day change swaps the BOOK (`librum_aptare` after every
action via an `agere` wrapper, and in kalendarium_argumentum_ponere):
pending typing is flushed first, then scriba_librum_ponere. Registries
never change, so nothing has to be re-merged.
Layout: the day view's lower half is a PARTES_NULLUM slot "notae"; after
dispositio the composer composes scriba with a COPY of its compositio
sized to the slot minus an 8px margin and 2px for the shadow (pure - the
mounted config is not touched), offsets it there and stamps the subtree
with the scriba's path. The root figure draws the flat shadow under it
(figura_umbram_pingere) - the "post-it".
Day pages have a FIXED size (72 x 30 cells): the first version sized
them from superficies_* at mount time, but vicus writes those after
mounting, so pages came out 40 x 6 (defaults) - seen in the screenshot.
A scriba folium keeps the size it was created with; a smaller area
scrolls. My calendar page counter id "paginae" collided with scriba's
own "paginae" component - renamed "index_paginae".
Also new: kalendarium_gestum_ponere (vicus facies.gestum_ponere) hands
the pane's gesture slot to the nested scriba - exercised in A6d-2.
Plant: book never follows the day -> red.
