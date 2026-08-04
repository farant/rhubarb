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

## 2026-08-04 — the ui verb (front door opens)

`silex` bare / `silex ui` opens a vitrea window: capsula-embedded
index.html (tools/silex_assets/, generated pair COMMITTED per the
vitrea_assets pattern), one `status` bridge call on load (versio +
fabrica + validity) so the C↔JS loop is proven from the first screen.
Bare-with-no-args = ui; fabrica optional for ui, still required for
novum. Build moved to ./tools/silex_struere.sh — compile_tools.sh
can't know about the capsula source (regenerates it when index.html
is newer). isatty() detection considered and deferred: no-args is
already an explicit signal; the tty check earns its place when bare
silex-in-a-pipe should print help instead (that file will then owe
postulata_posix.h).

## 2026-08-04 — VCS verbs I: status / condere / historia

The doctrine landed as designed: A COMMIT IS A NAMED POINT IN THE
EVENT LOG (a `conditio` actum), not an object. Trees are folds;
the manifest is the current fold; massae dedup means nothing is
stored twice. Working tree = projection, volumen = residence.

- status: pure reader. directorium_ambulare (includere_occultos
  FALSUM — solves .DS_Store, but also means DOTFILES ARE INVISIBLE
  to the VCS v1; named limitation). Praetermissa: bin/, build/,
  *.volumen(-wal/-shm). Compare sigilla vs manifest →
  MUTATA/NOVA/ABSENS; clean files just count.
- condere: runs status, refuses an empty diff (exit 1), absorbs in
  ONE transaction. Volumen grew caller-owned transactions
  (in_transactione flag — plagulam_condere/removere skip their own
  BEGIN when the caller holds one; sqlite forbids nesting) +
  plagulam_removere (actum plagula-remota + DELETE; massa stays —
  history still references it). Commit origo = "condita" (the
  ORIGINAL vendicata provenance survives in acta; renovare will
  derive three-way facts from history, not the manifest).
- historia: walks acta; entries = ortus + conditiones; tactae =
  plagula events since previous entry. novum now writes a birth
  conditio ("proiectum excusum (novum)") so new projects' history
  owns its birth files; PRE-EXISTING projects (001/002, scratch)
  show birth files rolled into their first user commit — honest
  artifact of retroactivity, not a bug.
- Retroactivity proven: silicetum/001 (born before the verbs
  existed) answers status (omnia munda, 15 plagulae) and historia
  (ortus) with zero migration.
- THE HOUSE TRAP FIRED AND WAS CAUGHT: countdown loop `i32 index;
  index >= 0` — always true (i32 unsigned), infinite loop. Examen
  flagged "comparatio vana" AT EDIT TIME, pre-run. s32 for
  descending counters, always. The lint paid for itself again.

Next: proicere (plan-default covenant, -scribere applies; later
-ad seq = time travel via fold), then renovare on top.
