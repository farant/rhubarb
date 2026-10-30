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

## 2026-10-27 - tofu on Thomas Aquinas: ⓘ, thin/hair spaces, LRM

Fran's "blank" tofu were U+2009 (thin, 8x on the page) and U+200A
(hair, 2x); the Claude Code paste flattened them to ASCII, so the
real page was fetched to check. Also U+200E (LRM, 2x) and ⓘ U+24D8
(the audio link after the IPA). Fix: lib/fons.c maps
U+2000-200A, 202F, 205F, 3000 to space (like NBSP); zero-width marks
(U+200B-200F, 2060, FEFF) can't be drawn in a fixed-cell font, so the
vicipaedia collector drops them, raw or via entities (&lrm;). ⓘ =
new 6x8 glyph (circle as ◯, i inside). Saved articles benefit without
a refetch: extraction runs on open from the stored HTML.

## 2026-10-27 - N3.4c remember the page

Reading position = {versio, offset of the page's first line, in the
link index?} per article, stored as volume PRESENCE
"navigator/positio/<ULID>" (JSON), not a plagula: lector's position
plagula writes an act + massa on every page turn, presence is the
last-writer-wins store made for this (outside the act log). Offset,
not page number, so a resized pane re-cuts the pages and still lands
on the page containing the offset (`_pagina_servata` searches only
the right half: article pages [0, paginae_articuli) or link pages).
Different version than the saved one -> page 0.

Saved on: every j/k/arrow (`_paginam_movere`), every open without a
named page (`_paginam_restituere` after `_articulum_aperire`; bare
argument via semitam_secuta). NOT saved when a locus names its page
(history, companion, notes) - those are "look at", not "read".

Gotcha: a bare `$nav(<id>)` applied through semita_applicare left the
previous `pagina` field in place (status fields absent from the semita
are not cleared), so "no page named" was undetectable. Fix:
navigator_argumentum_ponere clears pagina when the semita has none.

Test gotcha: at 360 px the side margin is already capped, so 240 px
narrows the text column only 240 -> 216 px and the same page index
held the offset; 150 px (126 px column) actually re-cuts. Resize in
tests = write superficies_latitudo with scriptor "dispensator".

## 2026-10-27 - N3.4b recent list

Presence "navigator/recentes" = article ULIDs, one per line, newest
first, unique, cap 20 (NAVIGATOR_RECENTES_MAXIMI); unknown ids are
skipped on read. Noted on every OPEN (the same two places that restore
the reading position: `_paginam_restituere` and the bare-argument
branch of semitam_secuta), not on page turns.

Root = one list of entries: bookmark domains [0, nd) then recents
[nd, nd+nr) - `RadixIndicis` holds both, the album gives domains
"d<k>" (res k) and recents "a<k>" (res nd+k), selection field
`electus` = "d:<domain>" / "a:<ULID>". The N3.4a domain-only helpers
were generalised and renamed (_radicem_eligere, fac_radicis_*,
_radicis_album, _electum_radicis). Enter on a recent row opens it; b
toggles its bookmark (b on a domain row does nothing). With recents
but no bookmarks the "Bookmarks" group carries a dim "No bookmarks
yet" title row ("g2") so the hint survives.

Bug on the way: the first `lineam_proximam` reassigned `valor` before
testing for the '\n', never advanced past it and hung the suite -
stopped the run, killed the binary, fixed (keep the end index b).

## 2026-10-28 - N4.1 links drawn in the link color

Link color = the editor's hashtag color: COLORATIO_LINK through
thema_color_ex_indice_colorationis -> color_ad_pixelum as an RGBA
ColorMandati (same path as pagina.c), so links follow the theme like
#tags do. `_lineam_textus_pingere` cuts each laid-out line at the
NEXUS notes (notes are in text order; article pages use
textus->notae, link-index pages textus->notae_nexuum) and advances x
by runes * fons_latitudo(l->fons) per piece. A link that wraps is just
two overlaps, one per line - no special case.

Side effect: a line with a link is now 2-3 text commands, so any test
that matched a WHOLE line containing a link must match the pieces
(probatio_navigator_latus 'argumentum' updated). New test helper
x_picti checks the pieces are contiguous.

## 2026-10-28 - N4.2 click a link

`_nexus_ad_punctum` maps a click on the page's "textus" component to
a link with the SAME geometry the painter uses: line by y (l->y,
fons_altitudo), column by x from lateris*NAV_LITTERA + l->x divided by
the line's font width, then walk that many runes from l->initium
(skipping UTF-8 continuation bytes) to an offset, then the NEXUS note
whose [initium, finis) holds it (article notes or link-index notes
by page). The URL goes through _url_aperire, so saved = offline,
Wikipedia = fetch, else the footer says "Unsupported site:
<host/path>" (message changed from "(Wikipedia only for now)").

Back: no new code - opening an article writes visus/articulus, which
are historia="impelle" fields, so vicus history records it;
probatio_vicus_navigator 'retro' pins that (URL open, Ctrl+O via
manus_ludus -> previous article).

Commit gate note: the N4.1 commit was refused once by the known
vicus_admonitiones l.206 flake under load (ledger …PXFR, ictus
logged); the retry passed.

Follow-up same day (Fran: "clicking the link isn't doing anything"):
the first version hit-tested with the RAW event point (ev->datum.mus)
against child fines. Child fines are relative to the pane root, and in
vicus the pane sits at an offset on screen, so nothing ever matched.
The pane-only suite drives the pane at (0,0), where raw == local, so
it passed. Fix: use `des->punctum_locale` (destinatio.c: point minus
the hit component's own origin, after parent transforms); fallback
ev - nodus->fines when des is absent. Pinned by
probatio_vicus_navigator 'ictus nexus': clicks through manus_ludus at
a real screen point (manus_ludus_ad_schirmum), red with screen coords.
Test-harness trap there: drawing a sub-tree with the pane's own figure
registry draws nothing inside vicus (registry keyed by spatium), so
the test computes the point from the layout instead (title 16 px +
gap 8 -> first paragraph line at y 24).

## 2026-10-28 - N4.3 keyboard links (Tab / Shift+Tab / Enter)

membrum__praecepta learned "Tab" and "S-Tab" (CLAVIS_TABULA, Shift
decides, never with Cmd); no other praecepta table used those names,
so nothing else changed. Selection = transient field `nexus` = start
offset of the selected NEXUS note in the current page's text. Only
links that START on a page are Tab stops there (a link continuing
from the previous page is selectable on that page), so crossing pages
never re-selects the same link. Tab past the last link moves to the
next page that has a link (pagina + nexus in one write, position
saved); none ahead -> nothing. Esc clears the selection before
leaving the page; j/k/arrows and opening an article clear it.
Painter draws the selected piece inverted (filled rect in the link
color, text COLOR_BACKGROUND); footer shows "→ host/path".

vicus finding (vicus-level test 'tabula nexus'): Tab only reaches the
navigator when the navigator has FOCUS; unfocused, the dispatcher's
built-in Tab moves focus (within the pane's own space). In the test,
vicus_focum_ponere needs a vicus_pulsare + dispensator_recomponere
before the dispatcher focus follows - without the recompose the key
went to the vicus root only (traced with temporary prints in
dispensator mittere, removed; dispensator.c unchanged).

## 2026-10-28 - N4.5 hide body links (~)

Fran: a clean-reading toggle. Key `~` (not `l`: spec item 13 keeps
h/l for the page grid / infobox side column). Persistent per-pane
field `nexus_occulti`. One gate, `_notae_activae(m, f, k)`, returns
no notes for ARTICLE pages while hidden (link-index pages always keep
theirs) and every consumer goes through it - painter, click hit test,
Tab stops, selected-link lookup - so the three behaviours can't drift.
Toggling clears the selection. Persistence pinned by
probatio_vicus_navigator 'nexus celati' (vicus rebuilt over the same
volume); planted by flipping the manifest field to transiens +
re-embedding, red, restored.

## 2026-10-28 - N6.1 sentence view

Reuses lector's shared pieces unchanged: codex_sententiae over the
article's CodexCaput (computed once in _paginas_parare, next to the
pages, in the same arca - link-index text is not read) and
lector_sententiam_disponere for the centered single-sentence layout
(FONS_TITULI if it fits). Layout = the page layout (title in the top
margin component, sentence in "textus", footer + arrows + star).
Field `sententia` = start offset of the current sentence (like
lector); `visus` gained "sententia" -> VISUS_SENTENTIAE (XVI). Every
sentence move saves the N3.4c position (offset = sentence start), so
leaving to the page view lands on the containing page. Restoration
into sentence view via semita returns early for now (N6.2 polishes:
location form, restart, companions).

## 2026-10-29 - N6.2 sentence view location / restore / companions

Most of it was free: `sententia` is a status field with ambitus
"ligatio", `visus` is the path segment, so the generic semita code
already produced and applied "navigator/sententia/<art>?versio=&
sententia=" (cognomen added for the bare form). Added in the pane:
_sententiam_restituere in semitam_secuta (coerce a mid-sentence offset
to the sentence start, pin versio, bare form -> saved N3.4c position
if same version and not in the link index), bare-argument clearing of
a stale `sententia` (like `pagina` in N3.4c), pilcrow in the footer.

Two behaviours worth knowing: (1) ambitus-ligatio fields are already
reset when visus or articulus change, so the stale-sentence clear only
matters for a bare argument while ALREADY in sentence view of the same
article - the test sets the saved position directly to reach it. (2)
Leaving sentence view for the page used to save the PAGE start as the
position, throwing away the finer sentence offset; removed - the
sentence offset finds the same page anyway.

## 2026-10-29 - N6.3 links in sentence view

Refactor: the link machinery now works on a "unit" = the page (page
view) or the sentence (sentence view): _unitas_currens /
_unitates_numerus / _notae_unitatis / _nexum_in_unitate on top of a
range-only core `_nexum_in_ambitu(notae, a, b, ab, gradus)`.
_nexus_electus and _nexum_eligere (Tab crossing units; sentence
crossing writes sententia + nexus and saves the position) are shared,
so page and sentence behaviour can't drift. The click hit test takes
its lines from lector_sententiam_disponere in sentence view (same
call the painter makes, same area) and from the page otherwise. `~`
applies: _notae_articuli_activae.

Process slip: this time the code was written before its tests; the
tests came right after and 7 plants (colors, Tab crossing, ~, Esc,
footer, page-lines-in-sentence hit test, raw screen coords in vicus)
showed each piece is caught. The vicus test recomputes the centered
sentence layout (area from the pane size like _margines/_area +
lector_sententiam_disponere) to click a link at a real screen point.

## 2026-10-29 - N3.5 infobox side column (grid tool, first use)

Library (navigator_paginae): NavigatorTextus.columnae - one
NavigatorColumna {ancora, caput, notae} per section that has adjuncta
(Rhubarb: taxobox in the lead, nutrition box under "Uses"). The
Scriptor now targets any (caput, notae) pair, so the same writer
builds the column: adjunct caption as title, inner titles, each PAR
as a paragraph "Key: value" (value's link notes shifted past the
prefix), paragraphs/lists; figures skipped. ancora = initium of the
section's first clausula (its title if it has one).

Pane: each column paginated with the same area; owner = the article
page whose start <= ancora. Field `columna` (status, ambitus
ligatio). l / h / Esc / top-margin markers (columna-intrare right,
columna-redire left, in a new caput row line so the text area is
unchanged); j/k and footer arrows move inside the column via a hook
at the top of _paginam_movere; main-column moves clear `columna` (a
location like pagina=3&columna=0 where page 3 has no column would
otherwise drop you into page 1's column later). Tab/Enter/s are no-ops
in the column for now (no selection on the hidden page). Footer and
pane title "Rhubarb - side 1/2".

Test notes: at the default size the taxobox fits one page - the j/k
test shrinks the HEIGHT (new altitudinem_ponere); at 150 px the footer
is too narrow for "side 1/", so that step checks the field.

## 2026-10-29 - N3.5b Tab in the side column

Third kind of link "unit": a page of the side column. _unitas_currens
/ _unitates_numerus / _notae_unitatis / _nexum_in_unitate check
_columna_currens first; _nexum_eligere writes `columna` + `nexus` in
the column (no position save) and never leaves it (units = the
column's pages only). The temporary "Tab does nothing in the column"
guards are gone; painter passes the selection; footer target comes
from the shared code.

Two test lessons: (1) pictum_colore("") never matched - chorda_continet
with an empty needle is false - so the helper now treats "" as "any
text" (used to assert "something is drawn inverted" when the first
infobox link's text isn't known). (2) The plant "column unit ignored"
stayed green while column page == article page == 0; the case that
exposes it is Tab on the column's SECOND page.
