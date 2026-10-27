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

## 2026-10-27 - N3.3 opening by URL inside vicus

'o' / "Open URL..." button -> prompt (membrum__prompta, fields like
sancti's); Enter or $nav(URL) -> saved article opens with no network,
otherwise navigator_petere on the HOST reactor (m->reactor hook; vicus
gives v->reactor). Messages (Fetching / Not found / Unsupported site /
Network: ...) live in `nota`; the index gives them a full-width row of
their own (in the header row they were cut off at 360 px), dim while a
fetch is pending, red otherwise.

Pulse/timer: vicus pulses a pane with `pulsare` every 16 ms unless the
pane also has `mora` - so navigator_mora returns -1 (idle) except right
after a fetch finished; the fetch's sockets in vicus's reactor are what
wake the loop. navigator_pulsare reports "changed" once and frees
finished fetches (never inside their own callback).

Superseding: a new fetch cancels the pending one AT ONCE. dimittere on
the next pulse would also cancel it, so a plant that removed the
explicit cancel first stayed green; the race is the window before that
pulse, where a late answer would be taken as the current fetch's. The
test now sends the old fetch to a slow (300 ms) server that would
succeed and polls the reactor without pulsing - red without the cancel.

Found on the way: probatio_vicus_quies is flaky under load (~1 in 3 at
load 7.7), also with the committed vicus_applicatio - ledger …N8S6.
