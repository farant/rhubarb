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
