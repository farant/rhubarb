# silex worklog

## 2026-08-04 — v0: the novum verb (first strike)

Design banked on ledger res 01KZ4Y9A8T (single distribution binary,
sources within, VCS seed). v0 ships only `novum`: closure from the
FABRICA (the rhubarb checkout — the "where libraries come from" is a
parameter; capsula embedding is a distribution-day decision, not a
prerequisite), scaffold, record everything in a volumen, project to
disk.

Mechanics + traps:
- **Closure = BFS over quoted includes** with the aedilis convention
  (include/X.h → lib/X.c automatic). v0 scan is TEXTUAL (lineatim) —
  a `#include "x.h"` mention inside a comment block would be picked
  up (gesta.h and silva_lexicon.h both contain such comment mentions).
  Mitigation: unresolvable cited headers are a WARNING + skip, never
  fatal — real deps always exist in the forge, phantom mentions don't.
  Promotion path when it bites: silva parsura → directivae.
- **Projection reads FROM the volumen** (plagulam_promere), not from
  the in-memory buffers that were just condere'd — the promere path is
  exercised on every novum from birth, and probatio_silex pins
  disk == volumen for a vendored file.
- chmod +x via processus_exsequi (/bin/chmod) — non-fatal on failure
  (`sh aedificare.sh` always works). No POSIX in silex itself.
- First real strike: silicetum/001 (outside git, per the dogfood
  protocol) — 10 vendored + 5 generated; scaffold compiles under the
  FULL flag battery and its credo probatio passes. The closure
  correctly dragged postulata_posix.h (foundation quotes it since the
  Linux arc) — 001 is Linux-portable from birth without silex knowing
  anything about POSIX.
- Scaffolded seeds: latina/piscina/chorda/credo → closure pulled in
  chorda_aedificator too (transitive). 15 plagulae total; volumen
  ~450KB. Fine.

Next verbs (designed, not built): status (three-way sigillum compare),
condere (absorb working tree), proicere (re-project), historia (walk
acta). Then the MCP posture.
