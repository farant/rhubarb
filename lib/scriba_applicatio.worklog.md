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
