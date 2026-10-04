# ludus_tessera — phase log

Module 013 (ludus apps in the terminal). Plan:
`project-specs/ludus-tessera-plan.md`. Sketch:
`../terminal-planning/modules/013-ludus-tessera.md`.

## A0 — the directory (2026-10-03)

`ludus_tessera/` beside tessera and saltuarius (decision 4): runner
`compile_probationes.sh` modelled on saltuarius's (effusio log, sera,
vexilla, incremental objects, exit contract 0/1/2, excubitor channel),
gate `ludus_tessera` registered in `pythonica/silva.py` (PORTAE,
FORMAE 'suita', SUITAE); `tools/frigida_probare.sh` finds the suite by
glob.

**The amalgam boundary is the API, as in saltuarius.** tessera enters
ONLY as `tessera/amalgama/tessera.c` + its include dir;
`tessera/fontes` is absent from the include flags. Checked first that
this is livable: the amalgam's `tessera.h` exposes only renamed types
(`TesseraPons`, `TesseraPiscina`, opaque `TesseraRivusTerminalis` …),
so it sits beside `include/eventus.h` & co. without clashes, and
`TesseraPons` is public - A1's new pons can be built from outside.

Red: the empty suite → `NULLA PROBATIO CURSA`, exit 2. Then
`probatio_ludus_tessera_fumus` (5 asserts): tessera opened through the
amalgam on its memoria pons, next to rhubarb's own piscina + credo in
one TU and one link; `Z` at (2,1) → the frame contains `ESC[2;3H` and
`Z`. Plants: the cell moved to (1,1) (CUP assert fails by name);
`#include "tessera_opus.h"` (a fontes header) → compile error, the
boundary pin working.

Deferred, on purpose: the aedilis root `ludus_tessera/fontes` waits for
A1's first fontes file (an empty dir is not tracked - a fresh clone
would have a root pointing at nothing); the tabularium inventory row
for gate coverage (portae debitae lens 'suitae probationum') must be
filed in MAIN - the ledger is disabled in secunda.

Commit note: the `pythonica` gate (owed by the silva.py edit) is RED in
secunda independent of this change - the identical four failures with
HEAD's silva.py restored (all portae-debitae / live-inventory checks:
`tabularium.c -> radix (per tabulariumd)`, README coverage, crusta
closure, CLI exit 2): the inventory is unreadable here. Committed with
`ludus_tessera` only; the registration itself was exercised by
`silva.porta('ludus_tessera')` (sana). Owed again in main after merge.

## A1 — the pons over `terminalis` (2026-10-03)

**Audit (the plan's "name it, pin it").** Three writers could touch the
terminal's mode bytes on this path:

| bytes | tessera posix pons | rivus | tessera per frame |
|---|---|---|---|
| alternate screen ?1049h/l | INTRANDI/EXEUNDI | no | - |
| mouse ?1000/1002/1006, paste ?2004 | yes | declared | - |
| ?1003 ?1004 kitty >31u | - | declared | - |
| ?2026l on exit | first in EXEUNDI | - | ?2026h…l per frame |
| ?25l / 0m ?25h | 0m ?25h on exit | - | ?25l 2J first frame; 0m ?25h in claudere |

Resolution: input modes come ONLY from rivus. The screen bytes cannot
live in the pons: terminalis's signal handlers (crash, TSTP) write
only the exit bytes handed to `terminalis_intrare`, so a pons-entered
alternate screen would survive a crash. Hence
`ludus_tessera_modos_componere` (pure): enter = `?1049h` + rivus's
declaration; exit = `?2026l` FIRST, rivus's exits (reverse), `?1049l`,
`0m`, `?25h` - tessera's own order (tessera_modi.h, internal - the
three constants are restated and pinned by the test). The pons is
thin: scribere/amplitudo → terminalis; intrare/egredi write NOTHING;
legere returns -1 (tessera's reader is unused on this path: misuse
fails loudly, never hangs); resumptum NIHIL (the glue handles
resumption, A3).

**Testing the fd-bound pons:** fd 1 is dup2'd onto a mkstemp file
around the pons calls. First run exposed my own test bug: credo's
progress dots landed in the capture (assertions inside the redirect
window) - results now go to locals, asserted after restoring.

Tests: 29 asserts (exact bytes for MUS+GLUTINUM; pair law over all
modes incl. kitty push/pop; ?2026l at 0; ?1049l after every rivus
exit; FALSUM on a second declaration / NIHIL rivus; pons writes only
what scribere is given; amplitudo == terminalis_amplitudo). Red: a
stub (21 named failures). Plants caught: ?1049l before the rivus exits
(4), pons intrare writing modes - the posix habit (capture shows
`?1049h?1000h` instead of `abc`), ?2026l dropped (2).

aedilis root `ludus_tessera/fontes` added (deferred from A0) and
`-Iludus_tessera/fontes` in compile_flags.txt.

## A2 — cells into tessera; the boundary reversed (2026-10-03)

**The bridge already existed.** `musivum_pingere`
(tessera/instrumenta/musivum/musivum_pictura.c, module 012's T5) is
exactly the plan's A2: continuation cells skipped, text units through
`tessera_graphema_ponere`, junctions through
`tessellatio_runa_juncturae`, colours into `tessera_stilus`; tested by
probatio_tessera_musivum. My plan had missed it.

**A0's amalgam pin blocked reusing it** (its header includes the fontes
`tessera_opus.h`; the two tessera forms cannot share a TU). Fran chose
to switch: ludus_tessera builds against tessera FONTES, like musivum,
folium and effigies. Reason recorded: the amalgam pin is saltuarius's
job (first host, proving the vendorable form); ludus_tessera is a
monorepo framework client. Evidence I already had in A1: the amalgam
path linked TWO rivus copies (lib's, driven by the glue, and the
amalgam's renamed one). Now: one copy of everything, from lib/. A0's
"fontes include = compile error" plant is retired; public-API-only is
a convention here, as for the other instruments.

**What A2 adds: one politica.** tessellatio measures units under a
`RunaePolitica`, `tessera_graphema_ponere` under the opus's
`TesseraPolitica`; tessera maps between them only inline
(tessera_opus.c:653). Disagreement is not cosmetic: a ZWJ family is ONE
unit of 2 cells under GRAPHEMATUM and THREE emoji of 6 cells under
SIMPLEX (Terminal.app) - glyphs vanish or columns slide.
`ludus_tessera_demittere(opus, mandata, modulus, fundus, fons, ctx,
piscina)` = tessellatio with the politica DERIVED from the opus +
musivum_pingere; the caller cannot pass a mismatched one.
`ludus_tessera_politica_runarum(opus)` restates tessera's rule.

Tests: 19 asserts (mapping both ways; the ZWJ family + "x" through
mandata → lowering → tessera cells under each politica: GRAPHEMA+LATUM,
continuation, x at col 2 vs three wide emoji, x at col 6; fundus and
littera colours; NIHIL arguments). Red: a stub. Plants caught: the
lowering with a fixed GRAPHEMATUM (SIMPLEX columns fail), musivum
writing the continuation as a space, musivum swapping colours.

## A3 — the glue (2026-10-03)

`LudusTessera` (fontes/ludus_tessera.{h,c}), the twin of
`LudusFenestra`: `_creare(piscina, d, figurae, thema, fons, ctx, opus,
cell_w, cell_h)`, `_tractare(ev, nunc)`, `_quadrum(nunc)` (pulse →
pingere → `ludus_tessera_demittere`; not presenting), `_currere(n)`.
The Modulus is cell × the opus size and is renewed with it. The clock
(`fenestra_tempus_ms`, plain C) is read only here.

**Quit (Fran): Ctrl-C ends the loop** like the window's close - the
app never sees it (fenestra handles EVENTUS_CLAUDERE before dispatch;
auscultator_terminalis already quit on Ctrl-C; raw mode keeps ISIG
only for Ctrl-Z). The chord is two fields (`claudendi_runa`,
`claudendi_modificantes`) so an app that needs Ctrl-C picks another.

**Resize and resume ride the existing tessera API:**
`tessera_magnitudinem_renovare` (re-reads the pons, forces a full
repaint) and `tessera_resumere`; both events are ALSO dispatched (track
b will read the size).

**The loop (`_currere`):** refuses with 1 and touches nothing when
`terminalis_adest` is false; rivus with the Modulus cell; modes MUS +
GLUTINUM composed (A1) into ONE `terminalis_intrare`; per turn: drain
`rivus_eventum_coalitum` (Ctrl-C stops), resumption and SIGWINCH as
events, frame + `tessera_praesentare`, then wait = rivus's pending mora
or else the dispensator's `quies_ms`. v1 honesty: an idle app wakes
every quies_ms (300) and re-renders; tessera's diff emits nothing when
unchanged, so it is quiet but not free - frame-on-demand is a later
refinement. Exit: `tessera_claudere` then `terminalis_exire`; measures
to stdout after the alternate screen is left.

Tests (headless, ludus_toy - the same target-free app the fenestra
test uses; pictor is A4): 28 asserts. Red: a stub (named failures; the
run also died indexing the empty notarius Xar - the stub never
dispatched). Plants caught: tempus stamped after dispatch (notarius
sees 0), resize ignored (2), resume ignored (no 2J), the frame never
lowered (no box-drawing cells), the chord ignoring modifiers. One of
my plants first failed to COMPILE (a malformed ternary) and printed
nothing - redone as `si (FALSUM)`; a silent plant is not a caught one.

The test closure was built from aedilis's enumeration
(`bin/aedilis <test> --enumerare`), not by chasing link errors -
`stml_macros` and `similitudo` came from there.

`_currere` itself is covered only by its no-terminal guard; Fran's look
in A4 is its real test.

## A4 — pictor in a terminal (2026-10-03; headless part)

**One assembly, two mains (Fran).** pictor's assembly left its window
`principale` for `lib/pictor_applicatio.c` (beside the other pictor
libs; apps/pictor is not an aedilis root, so a test elsewhere could
not resolve it). `apps/pictor/pictor.c` = assembly + ludus_fenestra;
`apps/pictor/pictor_terminalis.c` = assembly + ludus_tessera (opus on
the A1 pons; colour depth and width politica from the environment;
surface = terminal size at start × 6×8 - responsive is track b). Find
on the way: `pictor_documentum.h` pulled `fenestra.h` (Cocoa) for one
type; narrowed to `tabula_pixelorum.h` - the terminal binary links no
Cocoa.

**The proof (probatio_ludus_tessera_pictor, 9 asserts):** the REAL
assembly twice, temporary volumes, canons from RHUBARB_RADIX. A window
session: a pen stroke (press, three drags, release) at cell centres,
recorded by the notarius. The same recording replayed through the
terminal path: codificator (6×8) → bytes → rivus (modes as `_currere`
declares) → `ludus_tessera_tractare`. Equal: act cursor, both insulae,
and the document's sigillum. A frame then paints "penicillus" (status
title) into cells. Plants caught: the glue dropping MUS_MOTUS (sigillum
differs - the act COUNT did not: one stroke either way; that is why
the raster hash is compared); the stroke recorded off cell centres
(x = 6k+5 - the terminal reports centres only: B6b's finding, live).

The runner grew pictor's closure (from `aedilis --enumerare`) and the
vendored sqlite (volumen → scrinium), compiled relaxed with the root's
flags.

**Fran's look (2026-10-03): "everything seems to be working perfectly"**
- canvas and status row, strokes under the pointer, Ctrl-Z / fg with a
full repaint, resize repaint (layout fixed until track b), Ctrl-C
leaving the terminal clean. Session numbers (no -fumus, scratch
volume): 779 frames, compositio 4 ms, pingere 12 ms, demittere 1879 ms
(~2.4 ms/frame - the canvas through the quadrant path dominates),
slowest frame 3 ms, 79,860 bytes emitted. The `-fumus` run (30 frames
then exit) was a poor look: frames are per EVENT, so a drag spends the
budget in a second - a frame count is a smoke bound, not a viewing
window.

## A5 — RELATIO: track (a) done (2026-10-03)

**Track (a), ludus apps rendering in a terminal, is done.** A ludus
app's `componere`, figurae and actiones run unchanged in a tty; only
`principale` picks the glue. pictor is the proof: one assembly
(`lib/pictor_applicatio`), two mains, Fran's look "everything seems to
be working perfectly".

**What exists** (`ludus_tessera/`, gate `ludus_tessera`, 5 suites, 89
asserts at A4):
- `ludus_tessera_pons` (A1): the bytes that enter and leave the
  terminal composed in ONE place (`?1049h` + rivus's declared modes;
  `?2026l` first … `?1049l 0m ?25h`) and handed to the ONE
  `terminalis_intrare` - crash-safe; a thin TesseraPons over
  terminalis.
- `ludus_tessera_demissio` (A2): mandata → tessellatio → musivum_pingere
  with the width politica derived from the opus.
- `ludus_tessera` (A3): the glue - clock here only, Ctrl-C ends like a
  window close, resize/resume through tessera's own API, the real loop.
- `apps/pictor/pictor_terminalis` (A4).

**What the work found, by weight:**
1. **Width politica is a correctness rule, not a preference** (A2): a
   ZWJ family is one 2-cell unit under GRAPHEMATUM and three 2-cell
   emoji under SIMPLEX (Terminal.app). Layout and writing must share
   one politica; the lowering derives it, callers cannot pass it.
2. **Crash safety decides where bytes live** (A1): terminalis's signal
   handlers only know the exit bytes given at entry, so the alternate
   screen cannot be the pons's business.
3. **The bridge already existed** (A2: `musivum_pingere`) and my A0
   boundary (tessera only through its amalgam, copied from saltuarius)
   blocked it. Reversed with Fran: ludus_tessera is a monorepo client
   like musivum, not the amalgam's proof host; the amalgam path had
   linked rivus twice.
4. **A header leaked Cocoa into every pictor build** (A4:
   `pictor_documentum.h` → `fenestra.h` for one type).
5. **The raster hash, not the act count, catches a lost drag** (A4).
6. Process: a plant that does not compile prints nothing and proves
   nothing (A3); the test closure comes from `aedilis --enumerare`,
   not from chasing link errors; and a corpus library added in A4
   owed a briar MUTATIONES line in THAT commit - added here, late.

**v1 limits, named:** an idle app wakes every `quies_ms` (300 ms) and
re-renders (tessera emits nothing when unchanged: quiet, not free);
the layout is fixed at the start size (track b); `-fumus`'s frame
count is a smoke bound, not a viewing window; `_currere` is covered
by its no-terminal guard and Fran's look, not by a headless test.

**Next: track (b), responsive** - surface size as state (B1), cell-
aligned layout (B2), pictor responsive (B3); tasks written now.

## B1 — the surface size is state (2026-10-03)

**The dispensator writes it** (Fran): on a RAW `EVENTUS_MUTARE_MAGNITUDINEM`,
before destination and recomposition, one ephemeral mutation sets
`superficies_latitudo` / `superficies_altitudo` (our pixels) with the
writer "dispensator" - exactly how it already owns `focus` /
`focus_acervus`. **Both glues announce the size once**, lazily, at the
first `tractare` or first frame (whichever comes first; the flag is set
before dispatch because `tractare` re-enters): ludus_tessera = cells ×
Modulus, ludus_fenestra = the TabulaPixelorum's drawing size. The
window already emitted live resizes (fenestra_macos.m:945).

**Canons refuse silently.** An undeclared attribute makes `mutare`
return FALSUM, and `attr_scribere` ignores that (for focus too) - so an
app canon that forgets the declaration loses the size without a word.
pictor's ephemera.canon and domini.stml declare both (owner
"dispensator"); the pictor replay test asserts the canon accepted it,
and a plant removing the declaration is caught there.

Tests: dispensator (two resizes → attributes; honest restore after),
ludus_fenestra (initial 200×100), ludus_tessera glue (initial 240×104),
pictor replay (480; its window session now sends the initial size as a
window glue would, so both insulae carry it). The full root suite
found one more consumer: `probatio_pictor_canones` pins the ephemeral
owner count (XI → XIII, the reason in a comment).

Plants caught: the dispensator ignoring the event (4); each glue
without its initial event; the canon without the declaration. **My
first dispensator plant did not compile** (removing the only call made
the helper an unused static → -Werror → exit 2, nothing ran) and my
grep for failures printed "0" - the A3 lesson again, worse: read the
EXIT CODE of every plant run, not only its FRACTA lines. Redone as a
never-true condition.

Not proven here: that the composition after a resize SEES the new size
(the order is by construction - written before recompose - but no app
reads it yet). B3 (pictor responsive) is that proof.
