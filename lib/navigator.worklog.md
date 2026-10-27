# navigator worklog

## 2026-10-27 - N3.2 pane skeleton (secunda)

Mirrors lib/lector.c (same cell layout, praecepta table, album, figura
radicis, semitae). Index = album grouped by host (ALBUM_TITULUS "g<pos>",
rows "r<library index + 1>"); order = host, then title (ASCII
case-folded), built per call by insertion sort - cheap at our sizes.
Page view = navigator_textum_facere + lector_paginare cached per
(article, version, area) in `arca`. Mouse: row click selects, double
click opens, footer/list arrows turn pages (Fran: mouse "decently" too).

VERSION PINNING (found while writing the vicus test): opening an article
"at its latest version" used to leave `versio` empty, so the locus - and
the Cmd+T companion keyed on it - named no version; a later version
would have inherited the notes, against spec V.11/V.16. Now
_articulum_aperire writes the latest version id, and semitam_secuta pins
it when a path arrives without one. Two plants red.

Plant that stayed green: removing "articulus" from the locus in index
view - every writer that leads to the index already clears it, so the
block was dead; deleted rather than kept untested.

Traps: `duplex` is a latina.h macro (double) - not usable as a name.
`probationes/probatio_navigator.c` ALREADY EXISTS - the GUI demo of
navigator_entitatum (compile_tests.sh builds it as a GUI app, never runs
it). I overwrote it by accident with the first draft of the pane suite
(the Write tool said "updated", not "created") and restored it from git
the same hour; the pane suite is probatio_navigator_latus.c.
vicus: lector's "comes/<locus> has text" check is now shared
(comitis_notas_habere) by lector and navigator.
