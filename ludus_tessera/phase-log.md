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
