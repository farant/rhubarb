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
