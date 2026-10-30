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
