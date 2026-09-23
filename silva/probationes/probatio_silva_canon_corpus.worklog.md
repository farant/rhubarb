# probatio_silva_canon_corpus.worklog.md

## 2026-09-23 — lib census: a floor and equalities, not a pinned count

Applies to the five silva probationes that walk `lib/*.c`:
arbor_plagula, canon_corpus, exemplaria_inutilia, exemplaria_lint,
exemplaria_obsoleta.

**Was.** Every "all files pass" counter was pinned to the number of lib
files: `plagulae == 181`, `latinizatae == 181`, `purae == 181`,
`purae_expansae == 181`, `directa_exacta == 181`,
`octetim_exactae == 181`. That mixed two claims — "every lib file
passes" (silva's property) and "there are exactly 181 lib files" (a
fact about the repo). The second made EVERY new library fail five silva
tests even when it passed everything; new libraries land through
commits that run the root suite only, so silva sat red for days until
an unrelated commit found it. Re-pinned 156 → 173 → 176 → 180 → 181,
each time by a commit that did not cause it.

**Now.** `plagulae >= 181` (a FLOOR: catches an empty or truncated
walk; lowered only by a deliberate library removal) and every "all"
counter `== plagulae`. Zero counters untouched. `census_planus`'s 73
over `probationes/fixa/roundtrip` stays pinned — a snapshot over a
FROZEN fixture directory is exactly right (doctrine, quaestio
01M365VXAF: a live corpus gets invariants; snapshots need a frozen
input). arbor_plagula and canon_corpus now also PRINT each lib file
whose closure lacks latina.h, so a red `latinizatae == plagulae` names
the file instead of reporting a number.

**Plants** (temporary files in lib/, removed in `finally`):
- a clean new Latin library (`lib/zz_planta_munda.c`): all five GREEN —
  the new property (before: all five red);
- a new library without latina.h: arbor_plagula and canon_corpus RED on
  `latinizatae == plagulae`, output `NON LATINIZATA (latina.h extra
  clausuram): …/lib/zz_planta_nuda.c`; exemplaria_* stay green (not
  their property);
- an empty walk (the lib loop skips everything, silva.planta): RED on
  the floor `plagulae >= 181` (plus the existing `vidit_* > 0`
  presence checks); the equalities read 0 == 0 and stay quiet — which
  is why the floor exists. Reverted → green.

Known, unchanged: the walk uses opendir, so an UNTRACKED work-in-
progress file in lib/ from another session is judged too.
