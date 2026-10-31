# chronica (pane) worklog

## 2026-10-30 - C3 year view (Opus 5.5)

**Modelled on aetherophonum, not navigator.** navigator.c is ~4800
lines; aetherophonum (750) has the same mount / praecepta / agere /
figure / compose skeleton. The pane is one PARTES_TABULA figure that
draws everything, plus four figure-less children that only take
clicks: `supra` / `infra` (halves of the event area) and
`sagitta_prior` / `sagitta_proxima` (footer). Clicks route by
`nodus->id`, so no coordinate maths (and no punctum_locale bug class).
The vicus test still presses the right arrow at a real screen point
(manus_ludus_ad_schirmum) to prove routing inside vicus.

**The year is always RESOLVED, never trusted.** annus_quaesitus = field
-> presence ("chronica/annus") -> hodie -> none; index_anni snaps to
the nearest year with events (tie: earlier). Every reader (figure,
title, locus, keys) resolves through index_currens. So an edit that
empties the shown year moves the view on the next paint with no state
write, and h/l step from where the user SEES they are.

**Page cache keyed by table pointer.** paginas_parare caches pages per
(tabula, annus, area). Sound only because chronica_fontes builds a new
table in a NEW piscina before destroying the old one: same address =
same table. If that ever changes, the cache would show stale pages
after an edit.

**Year heading outside lector.** lector_paginare would draw a title
only on the first page; we want the year on every page, so the heading
is drawn by the figure and lector lays out only the event area (a
CodexCaput built by hand: description paragraph + dim meta paragraph
per event; `obscurae` flags per clause pick the colour).

**Arrows.** praecepta knows "Up"/"Down" but not Left/Right, so agere
maps CLAVIS_SINISTER / CLAVIS_DEXTER itself before the table.

**Locus built directly**, not via semita_legere: on first open the
`annus` field is absent (resolved from hodie), and the locus must
still name the year ("chronica/annus/1274") for companions.
semita_ex_argumento still parses arguments (cognomina "chronica/annus"
and "chronica/annus/{}"); year 0 is refused by hand.

Plants (8, all red, files IDEM): h/l by one calendar year; BC sign
shown ("-1446 BC" - caught only by the exact title check, since
pictum() is substring); position not saved; page count +1; hodie
ignored; tie prefers later; "supra" pages forward; genus missing in
vicus.

## 2026-10-30 - C3b continuous view, now the default (Opus 5.5)

Fran asked for an events.html-like view: all years flow, pages fill,
year in a left fifth beside the first line of its first event, and
repeated at the top of a page it continues onto. `v` toggles; `$timeline`
and bare arguments ("1274", "") open it (cognomina moved to
chronica/flumen; year view needs "chronica/annus/...").

**One lector caput for everything.** paginas_parare(flumen=VERUM)
puts every event of the table into one caput, laid out at the
four-fifths column width. Per clause, NotaClausulae carries the year,
the dim flag, and "prima" (first description clause of its year).
Drawing a page: a year label goes beside the line where a `prima`
clause starts, plus one on the page's first line (that is what repeats
a continuing year). The label uses FONS_TITULI if it fits the column,
else FONS_PARVUS.

**Position = (year at top of page, offset).** The page index alone
would drift with every edit, so the anchor is annus_paginae(p) (year
of the page's first line) plus `pagina` = p - pagina_initii(that
year). Restoring: base = page where the year starts; then base +
offset, clamped to the run of pages whose top year is still that
year. The offset also goes in presence ("chronica/pagina"), and it is
read only when the `annus` field is absent (so it came from presence
too). Any year-view write resets it to 0, so a stale offset never
lands on another year. The first version forgot the presence half; the
'flumen positio' test caught it (remount came back on page 1).

**h / l in flow.** l = first year whose start page is AFTER the
current page (a year starting on the same page is skipped, otherwise
l would do nothing on dense pages). h = the start page of the
top-of-page year if that is earlier, else the latest year starting
before this page.

Plants C3b: 7 + 1 manual, all red in the end. Three were green on
the first run:
- "year label not repeated at top": the footer ("Timeline · AD 1250 ·
  2/3") satisfied the substring pictum("AD 1250"). Fixed the TEST with
  pictum_exactum (whole-text match). The lesson again: pictum()
  substring checks also match the footer and title.
- "default view = annus" in visus_fluminis: no effect, because the
  manifest's absens="flumen" writes the field at mount. The default
  lives in the MANIFEST, so the plant was done by hand (absens="annus",
  organa_infigere, run -> red, restore, re-embed, cmp IDEM).
- "v does not pass the year": aimed too narrowly. v also writes the
  presence year, which the year view resolves from. Re-aimed to drop
  both writes -> red.

## 2026-10-30 - C4a selection (Opus 5.5)

**Units, built with the pages.** paginas_parare now also fills
`unitates`: one per event (the description clause), one per person
and one for the place (byte spans inside the dim meta clause), each
with the page it starts on. lineam_parvam reports the person and place
spans (ParsParva) while it builds the line; " · " is 4 bytes (· is
U+00B7, 2 bytes in UTF-8). One table drives everything: Tab order,
inverted drawing (span intersected with each line, so a wrapped name
inverts on both lines), the footer target and click hit-testing.

**Key = origin, not position.** `electus` =
"<liber>#<pagina>#<linea>.<columna>" + "#p<k>" | "#l". The column was
added to the approved form (which had only the line) because two
events can share a line of prose. electa() rebuilds unit keys to find
the match; a key that matches nothing (event deleted by an edit) is
simply "no selection", with no write needed.

**Motion clears** by writing electus="" in annum_scribere,
paginam_scribere, paginam_fluminis_scribere and the v toggle. So Tab
crossing a page writes the page first (clearing) and then the new
key.

**Clicks:** supra/infra now first try ictum_eligere. The point is
nodus->fines + des->punctum_locale (radix space), then line by y,
rune index by x (fons width), byte offset, and the unit containing
it. Only if no text is hit does the half flip the page as before. The
pane harness helper ictus_in now clicks the bottom-right corner (blank
margin) to keep meaning "flip". The vicus test clicks the first line
at a layout-derived screen point (constants X/XII mirror
CHRONICA_SINISTRUM and LIMBUS_Y + MARGO).
Plants C4a: 7, all red in the end. "Person span off by one" was
green at first: in the test event the people were the FIRST part of
the meta line, so the " · " offset was never exercised. The test now
has a type before the people and checks the place is inverted too.

## 2026-10-30 - C4b jump home (Opus 5.5)

Enter (or a click on the already-selected event) builds
`scriba/<pagina>?linea=<n>` and appends `&liber=<liber>` for a
companion, then calls m->aperire. vicus wires that to
inventum_aperire, the same opener search results use, so a companion
event opens its paired view on the right and the companion page on
the left, with the focus on the left (consistent with search; Fran
agreed). Ctrl+O on the right pane returns to the timeline at the same
year (vicus test 'domum comitis'); the selection is transient and
does not come back.

**`linea` before `liber`:** a book prefix can contain '?' and '&'
(navigator's comes/navigator/pagina/<a>?versio=..&pagina=0), so
liber stays LAST and everything after "liber=" is the prefix, as
before. inventum_aperire peels an optional leading "linea=<digits>&".

**Line, not column:** the scanner's columna is a byte offset and
scriba's cursor is a grid cell, so the cursor goes to column 0 of the
event's line. scriba already had paginam_ponere(..., linea, columna);
the new public steps (scriba_actiones_paginam_ostendere_ad_lineam,
scriba_paginam_ostendere_ad_lineam) just pass the line through, and
the old functions call them with 0.

**Context:** no new vicus struct after all. ContextusIndagationis
already holds `kalendarii`, so the chronica genus registers with ci,
reaching the sources via ci->kalendarii and the opener via ci.
The chronica block moved below the indagatio section so the type is
known.

**Test pitfall:** keys sent through manus in a vicus test reached
nothing until the test called vicus_focum_ponere(VICUS_DEXTRUM) +
pulse + recompose, as navigator's vicus test does. A freshly opened
pane does not own the keyboard until then.
