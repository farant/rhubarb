# ludus_tessera — plan (module 013: ludus apps in the terminal)

Written 2026-10-03, after module 004 (stilus_terminalis). Sketch:
`../terminal-planning/modules/013-ludus-tessera.md` (+ its "Findings
from eventus phase B"). Prompted by Fran: "lets look at ludus_tessera
and decide what pieces we want to prioritize porting … all that stuff
kind of assumes fixed page sizes in some places".

## I. The survey (what exists, 2026-10-03)

**Two widget worlds.**

- **Legacy (concha), ~12k lines.** `concha` → `schirmata` (10 screens,
  tab bar; `SCHIRMATA_MAXIMUS` include/schirmata.h:48) → `layout.c`
  (STML, fixed character coordinates: `x=0 … latitudo=71 altitudo=60`,
  include/layout.h header example) → `ManagerWidget` (include/widget.h)
  → widgets drawing straight into a `TabulaPixelorum`: `pagina` (vim
  editor, lib/pagina.c 799), `navigator_entitatum` (lib 2861),
  `libro_paginarum` (100 pages + history, lib 1321). Fixed sizes: the
  page IS a 68×56 `TabulaCharacterum`
  (include/tabula_characterum.h:38-43; a tab is two cells); hit tests
  divide by a hard 6 px glyph (lib/schirmata.c:202); pane widths are
  literals (lib/schirmata.c:945-970).
- **New (ludus).** dispensator (include/dispensator.h) → `componere`
  (insulae + motus → componens tree) → figurae → mandata →
  `delineare_mandata` (pixels) or `tessellatio_computare` (cells,
  module 012). One real app: pictor (lib/pictor_*.c; figurae TABULA and
  TITULUS, lib/pictor_figurae.c:107-108). It is fixed-size TOO:
  `pictor_componere` reads `cfg->fenestra_latitudo/altitudo` from a
  creation-time config (lib/pictor_componentia.c), and nothing handles
  a size change.

**The models are pure** and survive as-is: `vim.c` (2101 lines,
includes only tabula_characterum + piscina), `tabula_characterum.c`
(1102), `coloratio.c` (1194).

**The terminal side from phase B:** `terminalis` (include/terminalis.h:
raw mode, timed read, write, size, SIGWINCH/SIGCONT flags; static, one
per process) under `rivus_terminalis` (pure; lossless Eventus);
`tools/auscultator_terminalis.c` (192 lines) already runs that loop.
Clock: `fenestra_tempus_ms` lives in plain C
(lib/fenestra_tempus_macos.c), no Cocoa.

## II. Decisions (Fran, 2026-10-03)

1. **Legacy + new, not a port.** concha and its widgets stay untouched
   and working. New widgets are ludus-native (componens + figura +
   actiones, state in insulae), reusing the pure models unchanged.
2. **Three tracks, in order (a) → (b) → (c)**: (a) rendering in the
   terminal, (b) responsive layout, (c) insula-native widgets.
3. **The terminal: `terminalis` owns it, tessera draws.** A new
   `TesseraPons` backed by `terminalis`; rivus hands the lossless
   Eventus straight to the dispensator. tessera keeps the grid,
   diffing and output; its reader is not used on this path.
4. **Location: a new top-level directory `ludus_tessera/`** beside
   tessera and saltuarius (fontes, probationes, runner, phase log).
   lib/ stays free of tessera; tessera stays free of ludus.
5. **Pages keep a size, chosen per document** (paper, in spirit). When
   the view is smaller than the page, an edge mark shows on the rows
   (and below the last visible row) where NON-BLANK content is cut
   off; trailing whitespace earns no mark.
6. **First widgets (track c): pagina (text page + vim) and schirmata as
   app state** ("which screen" + a tab-bar component, not a manager).

## III. Track (a) — rendering in the terminal

The proof app is pictor (same `componere`, figurae, actiones as the
window). Until track (b), pictor composes for the size the terminal
had at start (cells × Modulus VI×VIII); a resize repaints at the new
grid size but the layout stays put.

**A0 — the directory.** `ludus_tessera/` with `fontes/`,
`probationes/`, `compile_probationes.sh` (saltuarius as the model),
`phase-log.md`; aedilis root; a gate `ludus_tessera` registered.
Red: the empty suite reports exit 2 (nothing ran), then one trivial
test runs.

A0 as built: runner + gate `ludus_tessera`; tessera ONLY through its
amalgam (saltuarius's pin - checked livable beside eventus.h); smoke
test 5 asserts, two plants. Deferred: aedilis root (to A1, first
fontes file); gate-inventory row (in main - no ledger in secunda).

**A1 — the pons over `terminalis`.** `TesseraPons` whose `scribere` =
`terminalis_scribere`, `amplitudo` = `terminalis_amplitudo` (columns,
rows), `resumptum` = `terminalis_resumptum`; `intrare`/`egredi` do NOT
touch raw mode (terminalis did, at `terminalis_intrare`). AUDIT FIRST:
the posix pons writes `INTRANDI` itself (tessera/fontes/tessera_modi.h
:23: alternate screen, mouse 1000/1002/1006, paste 2004); on this path
the input modes come from rivus's declaration, so the alternate
screen (and the cursor) must come from exactly one place - name it,
pin it. Tests through a fake `terminalis`? terminalis is static and
fd-bound, so the pons is tested by WHAT IT WRITES via an injected
writer, or the glue is tested with the memoria pons and this pons only
by Fran's look (decide in A1; record why).

**A2 — cells into tessera.** Pure bridge: a `TessellatioCellula` grid
(tessellatio_computare's output) → `tessera_graphema_ponere` /
`tessera_cellulam_ponere`: text units (UTF-8, width 1/2, continuation
skipped), box-drawing junctions (`tessellatio_runa_juncturae`), colours
0x00RRGGBB → `TesseraStilus`. Red: a mandata scene (text, a wide rune,
a box, colours) → expected tessera cells through the memoria pons.
Plants: continuation cell written as a space over the wide rune's
right half; fundus and littera colours swapped.

**A3 — the glue.** `LudusTessera` mirroring `LudusFenestra`:
`ludus_tessera_creare`, `_tractare(ev, nunc)`, `ludus_tessera_quadrum
(nunc)` (pulse → pingere → tessellatio → A2 → `tessera_praesentare`),
mensurae (compositio, pingere, demittere, octeti from
`TesseraFructus`), and `_currere(quadra_maxima)`: wait = min(rivus
mora, quiet seat, next frame if moving); `terminalis_legere` →
`rivus_tradere` → `rivus_eventum_coalitum` → dispensator; SIGWINCH →
size → `EVENTUS_MUTARE_MAGNITUDINEM` + tessera resize; SIGCONT →
resumption → full repaint. The clock is read HERE only. Red: headless
through the memoria pons - one frame of pictor = the cells
tessellatio gives for the same mandata. Plant: stamping tempus after
dispatch (an event reaches the dispensator with tempus 0).
OPEN at A3: the quit chord (013 open question 3: a tty has no window
close).

**A4 — pictor in a terminal.** Runner (`ludus_tessera/pictor.sh`,
`AEDIFICARE_SOLUM=1` builds only). Headless proof: a recorded pictor
`.eventus.stml` (stroke: press, drag, release) replayed through
codificator → rivus → glue → insulae IDENTICAL to the fenestra run,
cells equal to tessellatio's lowering of the same mandata
(`manus_ludus_iterare_per`, as probatio_iteratio_transversa). Fran's
look: Ghostty and Terminal.app (Terminal.app reports no pixel size;
256 colours via TesseraColores).

**A5 — RELATIO.** Phase log; terminal-planning 013 status.

## IV. Track (b) — responsive (outline; tasks written when (a) closes)

- **B1 surface size as state.** Leaning: the resize event writes
  `superficies.latitudo/altitudo` into INSULA_EPHEMERA and componere
  reads it like zoom (replayable, target-free). Alternatives kept: the
  glue updates the app's ctx; a new `Componere` argument. Decide at B1
  with (c)'s insula shape in view.
- **B2 cell-aligned layout.** A helper so rectangles fall on Modulus
  boundaries (B6b: a terminal reports cell centres only).
- **B3 pictor responsive** as the proof (status bar on the last row at
  any size).
- The page cut-off mark (decision 5) is specified here, built with
  pagina in (c).

## V. Track (c) — insula-native widgets (outline; interview first)

- **pagina:** the page text = durable insula; cursor, mode, visual
  selection, pending key (`d…`) = ephemeral insula. Reuses `vim.c` and
  `TabulaCharacterum` unchanged; a new figura (grid → mandata, with the
  cut-off mark) and actiones (key → vim). OPEN: where undo (32 line
  operations, `VimUndoAcervus`) and the shared clipboard live; vim's
  `fd` escape keeps an `f64 tempus_f` - under ludus's one-clock rule
  it must come from `Eventus.tempus`; vim's fixed buffers
  (`VIM_CLIPBOARD_LINEA_MAXIMA`) against per-document widths.
- **schirmata:** "which screen" in the ephemeral insula + a tab-bar
  component; screens are subtrees `componere` chooses.

## VI. Gates

A0-A3: `ludus_tessera` (new), radix (if lib/ is touched), aedilis,
generata. A4: + tessera (if tessera changes - it should not).

## AUDIENDA

- Whether tessera's posix pons and a terminalis-backed pons can share
  `tessera_aperire`'s current call order (`amplitudo` before
  `intrare`, tessera_opus.c:140/221) - read at A1.
- Whether rivus's declared modes include the alternate screen and
  cursor visibility (if not, who writes them) - A1.
- pictor's figurae are drawn in pixels-our (6×8 cell); whether its
  status bar text survives tessellatio's text measurement on narrow
  terminals - seen at A4.
- `terminalis` is static (one terminal per process): tests of A3 must
  not need it - the memoria pons path covers them.
