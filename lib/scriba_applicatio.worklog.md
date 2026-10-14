# scriba_applicatio worklog

## 2026-10-05 — S3: scriba as an app (headless part)

One assembly for both targets, after `pictor_applicatio`: volume
(`-fumus` = temporary, `-volumen <path>`, default `scriba.volumen`),
document (opened, or a new 68×56 sheet, checkpoint interval 64),
insulae with scriba's canons and owners (durable sheet size; ephemeral
starts `focus="pagina" modus="normalis"`), actions, figurae (no image
source - the page is text), composition (6×8 cells, one status row),
dispatcher, and the gesture slot (`scriba_gestum_ponere` - without it
nothing is ever saved: plant). Mains: `apps/scriba/scriba.c` (window,
480×480 our-pixels = 80×60 cells: the sheet with its frame and the
status row fit) and `scriba_terminalis.c`; both `.sh` have a REAL
`AEDIFICARE_SOLUM=1` build-only switch (pictor.sh lacks one - it
launched a window during H2).

**Replay proof** (`ludus_tessera/probationes/probatio_ludus_tessera_scriba.c`):
a window session typed with `manus_ludus_scribere` (insert, a pause
that saves a chunk, more text, Esc, `u`, Ctrl-R) is recorded and
replayed through encoder -> rivus -> glue; same sigillum, cursor, acts
(2 `mutatio`, 1 `coniunctio`), both insulae; the frame shows the text
and NORMALIS on the last row. The transport had to learn two things:
the encoder pairs a key-down with its committed text only when given
BOTH in one call (`codificator_eventa(…, II)`) - one at a time, every
letter arrives twice (plant: the insulae differ); a lone ESC waits on
rivus's escape timeout (`rivus_moram` after `rivus_mora_ms`). Pulses
(EVENTUS_NIHIL - the notarius records them) carry no bytes and go to
the glue directly, so the debounce replays.

Not headless: the glues' `dispensator_finire` at exit (needs a real
window / tty) - Fran's look.

## 2026-10-05 — mounting split from the standalone app (insula-rami-plan T1a)

`scriba_montare(m, …, repo, id, …)` (and `pictor_montare`): document
opened/created in namespace `id` ("" for the root), canons on the
branch, initial element (`insula_ramum_initiare`), owners, contexts
with the branch inside, the app's OWN action and figura registries. The
`…Montatio` struct owns the contexts (registries point into them -
never copy it). Standalone = a repo whose roots are bare `<scriba/>` /
`<scriba focus="pagina"/>` + a root-branch mount + dispatcher + gesture
slot; `ScribaApplicatio` keeps `doc`/`actiones`/`figurae` as aliases,
pictor's mains take `&app.montatio.figurae_ctx`. No behaviour change
(root 203/203 before the new test, ludus_tessera 6/6, all four apps
build). Proof (probatio_montatio): pictor + two scribae in one store and
one volume - elements, namespaced documents, branch canons and owners,
componere from a branch, a remount on the same volume reopens the tab's
own document. Glossary: montare, montatio, initiatio.

## 2026-10-08 - page indicator: data in the branch, refreshed on change only

The status line is composed purely from the branch, so the page
indicator is two ephemeral attributes (`pagina_positio`,
`paginae_numerus`) rather than something the composer asks the book.
`paginam_indicare` writes them; it reads the view's page from the
volume plagula, so it only runs when needed: forced at mount and when
`scriba_reficere` sees the document change (page navigation), otherwise
only when the book's count differs from the stored one (another view
created a page). Page navigation itself (in scriba_actiones) does not
write them - the next pulse/focus refresh does, one frame later.
Proof: probatio_vicus_paginae checks the composed tree's "paginae"
node on both views across new-page/back, and PIXELS in the left status
line (the tree checks alone cannot see an unregistered figura);
a headless PNG lands in build/probatio_vicus_paginae.png. Five plants
caught.

## 2026-10-13 — A6c: scriba_montare_in_ramo (nested mount)

`scriba_montare` hard-coded its branch (`<scriba id>` under the ROOT or
the root itself). Its body is now `ad_ramum_montare(..., InsulaRamus
ramus, ...)`; `scriba_montare` computes the root-level branch as before
and `scriba_montare_in_ramo(parens, id)` uses
`insula_ramus_liberi(parens, "scriba", id)` - the calendar day view
mounts the day's notes under its own branch (A6d). The id also keys the
page cursor ('scriba/visus/<id>') and the saved state, so it must be
unique in the volume, AND it is a canon `nomen`: '-' is refused
("canon rami recusat scripturam: valor generi attributi non congruit") -
use "c1_notae", not "c1-notae" (the first test tripped on exactly that).
Plant: nested mount placed at the root -> red.

## 2026-10-13 — A6d: scriba_librum_ponere

Switches a mounted view to another book (the calendar's day books)
through scriba's own page-switch path (`paginam_ponere`, exposed as
`scriba_actiones_librum_ponere`): page from the view file if it exists
in the new book, else the first; working sheet, cursor, mode and branch
dimensions follow. Writes go to the scriba branch, whose owners admit
only `pagina.clavis` - the caller is calendarium.agere, so the writer
is set to pagina.clavis for the call and restored (memory: domini refuse
silently). Registries and contexts stay; nothing is remounted.
