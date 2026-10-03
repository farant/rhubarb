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
