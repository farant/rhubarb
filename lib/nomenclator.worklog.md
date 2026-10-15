# nomenclator worklog

## 2026-10-14 - P2a: the contacts list in vicus

nomenclator-plan P2a; manifest (apps/nomenclator/organum.stml) and
header approved by Fran (one Latin fix in the manifest note: "indici"
-> "indicis"). Shape copied from the calendar on purpose:
- dispositio tree in 6x8 cells (root column with a one-cell margin:
  "titulus" 3 lines, caput row 2 lines = "caput" + "pagina-prior" +
  "index_paginae" + "pagina-proxima", then the list region);
- ONE root figure (PARTES_TABULA) draws everything in root space
  (title, count, arrows, letter bands, rows) - the same convention the
  calendar's tests rely on (text found by the child's root-space box);
- rows from `album`: letter headings "l<L>" ("l_" = '#') are
  ALBUM_TITULUS, so a page starting inside a letter repeats it
  ("lR-c"); contacts "p<id>" are ALBUM_RES; 2 lines each;
- commands from `membrum__praecepta` (j k [ ] - one view for now);
  selection writes persona_electa AND the page of that contact (album_
  pagina_rei), so j across a page boundary turns the page; [ ] select
  the first contact of the new page.
The layout glue (LocusComponentis/Disponens/locum_addere/
componentem_locare, ~60 lines) is a COPY of the calendar's - second
consumer, so a candidate for a membrum__ library (noted in the plan).

Mistake on the way: I used `nomen` as a variable (twice: lib and
test) - it is the latina.h macro for typedef (CLAUDE.md says so).
Renamed appellatio / cognomen.

vicus: ContextusNomenclatoris opens ONE personae document per volume
(spatium "personae") lazily, like the agenda; genus "nomenclator";
`$contacts` via aperiens_fixum_registrare (no fixed argument).
Opening the same genus twice in one tab returns the existing pane - the
shared-document test opens the second pane in tab 2.

Tests: probatio_nomenclator (6 sections; plants: no letter headings,
k moving forward, page not following the selection, page turn without
selecting, row click ignored, tags not drawn, selection not drawn,
heading not repeated - all red) and probatio_vicus_nomenclator (2
sections; plants: document per mount, $contacts unregistered - red).
Screenshot (headless probe, deleted; copy in $TMPDIR/imago): list with
letter bands B C E G R S, tags right, selection. The business marker
(5px diamond) is barely visible and lost on the selected row - revisit
in P5.

## 2026-10-14 - P2b-1: create, delete, undo by key

Manifest (approved) gained the three fields membrum__prompta expects
(prompta = nulla | homo | societas, prompta_textus, prompta_cursor).
n / N open "name:" / "business:"; Enter commits through
promptam_committere: person = last word -> cognomen, the rest ->
praenomen ("Cher" = praenomen only); empty text is refused by personae
(prompt stays); on success the prompt is CLOSED FIRST and only then the
new contact is selected - the page of the selection is computed with
the album, whose capacity loses a row while the prompt is open, so the
other order could pick the wrong page. X deletes the selection
(REQUIRIT_PERSONAM via requisita_nomenclatoris) and moves the
selection to the next contact in index order, else the previous one;
u / C-r undo / redo. While the prompt is open it takes every key
(PROMPTA_EDITA consumed); non-key events fall through to the list
(clicks still select).
The prompt row is laid out at the bottom of the list region (spacer +
"prompta" PARTES_CAMPUS) and drawn by the root figure with
prompta_pingere in root space.
Tests: probatio_nomenclator +4 sections (novus_homo, nova_societas incl.
the new contact on page 2, deletio incl. undo/redo, prompta drawing);
probatio_vicus_nomenclator +1 (real vicus keys: n, "Anna Rossi",
Enter). Plants: name not split; prompt not closed; successor not the
next one; undo a no-op; new contact not selected; wrong prompt label -
all red. Untested: the one-row capacity reduction while the prompt is
open (no fixture fills a page exactly).

## 2026-10-14 - P2b-2: "+ New" button and menu

Fran chose option 1 (one button opening a Person / Business menu).
Manifest (approved) gained `elenchus` (popover, context mask + anchor)
and `elenchus_electa`. "nova" (PARTES_BOTTONE, 7 cells) sits first in
the header row, drawn with figura_globulum_pingere + FIGURA_UMBRA like
the agenda's; a left click opens context ELENCHUS_NOVUS anchored at the
button's bottom-left (the button is a direct child of the root, so its
fines are already root space - no destinatio conversion needed).
TABULA_ELENCHORUM has the two "new" commands, so the menu shows their
keys (n, N) from the praecepta table; mount checks it with
elenchi_verificare. While the menu is open it takes every event first
(before the prompt and the list), via hospitem_parare +
elenchi_legere / elenchi_agere; the composer adds the menu as the last
root child; figura_elenchi (PARTES_DIALOGUS) draws it.
Tests: menu_novum (opens below the button, two items with titles, a
click on "New business" opens the business prompt and closes the menu,
Esc closes). Plants: button not opening; menu not added; anchor at the
button's top - red. Screenshot (probe deleted; copy in $TMPDIR/imago).

## 2026-10-14 - P3: by tag, tag filter, view switch

Manifest (approved): `visus` (index | rubricae, persistent),
`rubrica_filtrum` (persistent), prompta option `filtrum`.
Tag view: one group per tag (personae_rubricae, canonical order), then
"(none)" for untagged contacts (only without a filter); heading
"g<k>" = ALBUM_TITULUS (k = index in greges(), so a filtered list
restarts at g0); rows "p<id>-<k>" - a contact with several tags is
listed under each (selection highlights every copy; album_pagina_rei
prefers the current page). The filter applies in BOTH views (index
view skips contacts without the tag). Row ids are parsed up to '-'
(id_ordinis) everywhere - clicks and drawing.
Filter: heading click sets it (again on the active one clears; "(none)"
inert), `f` opens "filter:" prefilled with the current tag (empty
clears, invalid refused - prompt stays), Esc clears an active filter
(command with REQUIRIT_FILTRUM; when no filter is set Esc is not
consumed and passes up). Setting a filter selects the first visible
contact; switching views keeps the selection and its page follows.
Fallback selection is now the first contact of the CURRENT index
(filter + view), not of all contacts.
View switch: `g`, and an "All | Tags" segmented control at the right of
the title row (figura_segmentum_pingere); "visus_indicis" /
"visus_rubricarum" are keyless commands for the M3 palette.
Count: "k of N contacts" under a filter; the title gets a dim "#tag".
Tests: greges, filtrum, segmenta (+3 sections). Plants: (none) group
dropped; filter ignored in the index view; heading click not
toggling; invalid filter accepted; count without "of"; segment click
ignored - all red. Screenshot of the tag view (probe deleted).
Process slip: I started P3 edits while the P2b-2 commit's gates were
still running; commissio refused ("plagulae commissionis mutatae DUM
porta currebat"), I reverted and re-ran - the freeze rule exists for
exactly this.

## 2026-10-15 - P5a: contact detail view

Approved header change: NomenclatorHodie port (+ hodie / hodie_ctx in
the mount) for the age; vicus passes hodie_vici + the vicus (the same
clock as the calendar); the test fixture a fixed 14 X 2026. Manifest
(approved): persona_aperta, campus_electus, prompta options nomen /
telephonum / epistula / rubricae / natalis.
Opening: Enter (VISUS_INDICIS, REQUIRIT_PERSONAM) or a double click on
a row (DUPLEX on "p<id>"); back: Esc (VISUS_PERSONAE - Esc in the list
stays "clear filter") or the "< Contacts" button. The views are a
command-table mask: visus_currens = persona_aperta ? VISUS_PERSONAE :
VISUS_INDICIS; delete / undo / redo are in both (VISUS_OMNES); delete
from the detail view closes it and selects the successor in the list.
Rows (campos_struere): name, each phone, "+ phone", each email,
"+ email", tags, birthday - ids "f<k>", selection campus_electus
(clamped). Enter opens the prompt for the row prefilled with its value
(birthday as "YYYY-MM-DD" or "MM-DD"); campum_committere: name split
like creation (a business edits its title), phone / email via
indicem_mutare (row index replaced, EMPTY removes it, the "+" rows
append), tags via personae (invalid refused), birthday accepts
"YYYY-MM-DD" / "MM-DD" / "--MM-DD" (empty clears; validity is personae's).
Kind: `t` or the "Person | Business" segments.
Layout: toolbar (back, segments), top half = fields column + a photo
box of a third of the width x the half height (~1/6, reserved for P7,
drawn as a dim frame "photo"), bottom half = "notae" (empty until
P5b). The list layout moved into indicem_disponere so the composer only
chooses; componentem_locare takes lineae < 0 for a growing height.
Test arithmetic slips on the way (row indexes shift when a phone is
added or removed - the selection stays on the row number); caught by
the tests themselves. Display bug found in the screenshot: tags drawn
"#familia roma" - now "#familia #roma", asserted.
Tests: aperire, campi, natalis, persona_alia (+4 sections). Plants:
Enter not opening; empty not removing; MM-DD rejected; age not drawn;
kind toggle a no-op; delete not closing the view; double click not
opening - all red.
