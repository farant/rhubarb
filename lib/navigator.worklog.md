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

## 2026-10-27 - N3.3b Wikipedia search

Plain words in the url: prompt search Wikipedia (REST
/w/rest.php/v1/search/page, limit 20) instead of "Not a URL". "la: Roma"
picks the language; without a prefix the current article's host (page
view) else en. navigator_quaerere shares the fetch machinery (handle,
User-Agent, cancel, release) - navigator_petere was split into
_petitionem_creare + _vocare. Results live in the FETCH's arena, so
the pane copies them into its own `arca_eventuum` before the fetch is
released on the next pulse. Results view `eventus` = album like the
index; Enter goes through _url_aperire (saved opens offline). Esc goes
back to the page if an article is open, else the index. A restart in
the results view re-runs the stored search (results are not
persisted).

## 2026-10-27 - N3.4a bookmarks, Yahoo-style index

The index now lists BOOKMARKS only, in two levels. Root ("Saved"): a
"Bookmarks" group, one row per root domain with a count; Enter or
double click goes down. Domain level: the old grouped-by-host list,
filtered to bookmarked articles of that domain (`_ordo` filters on
`a->favor` and `_radix_dominii(host) == dominium`). Root domain = last
two host labels (co.uk etc. need the Public Suffix List later).

The level is not a new `visus` value: `visus="index"` with an empty
`dominium` field is mapped to an internal VISUS_RADICIS (VIII) by
`_visus()`. So the manifest kept three visus options, the praecepta
table can bind j k [ ] Enter separately per level, and Esc at the
domain level (VISUS_INDICIS) goes to the root while Esc at the root is
unbound (falls through to vicus). Root selection is its own field
`electus` = "d:<domain>" (N3.4b will add "a:<ULID>" rows);
domain-level selection still uses `electus_articulus`.

`_album_indicis` hides the two-level difference from the layout code:
returns the album, the selected item's page and the row count for
whichever level is showing. Drawing is split (`_radicem_pingere` vs
`_indicem_pingere`); the row painter was generalised into
`_lineam_ordinis_pingere(left, right, selected)`.

`b` toggles (page: the open article; domain level: the selected row),
and the page footer has a star button (bright COLOR_WARNING if
bookmarked, dim otherwise; ☆ has no glyph so the dim ★ stands in).
Toggling changes the library, not the branch, so `_favorem_vertere`
sets `mutatum` for a repaint. The id is copied before
navigator_favorem_ponere because the acts are re-read.

Esc from a page goes to the article's domain IF that domain still has
bookmarks, else the root (`_dominium_reditus`), so you land where the
article is listed.

Migration note: articles saved before N3.4 have no favor act, so they
don't appear in the index until bookmarked (they still open offline by
URL/search). N3.4b (recent list) surfaces them.
