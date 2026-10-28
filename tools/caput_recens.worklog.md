# tools/caput_recens.sh — worklog

## 2026-10-28 — born (budget B1 E2)

38 wrapper scripts (silva/*.sh, officina/*.sh, gesta, materia, saltuarius,
tessera/amalgamare.sh, tools/{aedilis,canon,natura}_struere.sh) each defined
`newest_header () { find <dirs> -name '*.h' -newer "$1" | head -1; }` and
called it per object as `[ -n "$(newest_header "$obj")" ]`: subshell + find
+ head = three processes and a directory walk per object, 44 per examen
call. The test runners (briar, oratio, ludus_tessera, ...) already did
"CAPUT RECENTISSIMUM SEMEL"; this helper gives the wrappers the same.

Decisions:
- **Lazy**: the newest header is found on the first `caput_recentius` call,
  not at `source` time, so a wrapper that generated a header before its
  compile loop still sees it.
- **Status, not output**: `caput_recentius OBJ` returns 0 (stale) / 1. A
  `$(...)` call would run the lazy init in a subshell and lose the cache —
  the walk would come back per object. All 38 scripts' call sites were
  converted mechanically (`[ -n "$(newest_header "X")" ]` → `caput_recentius
  "X"`); tools/instrumentum_nectere.sh prefers `caput_recentius` and falls
  back to `newest_header` for the runners that still define it.
- **Ties recompile**: bash 3.2 `-nt` compares whole seconds; `! [ obj -nt
  newest ]` treats a tie as stale. Old `find -newer` was nanosecond-strict.
  Consequence measured: touch a header, then two examen calls in the same
  second → the second call recompiles the objects built in that second once
  more (18 then 10 then 0). Cautious direction (never misses a change);
  only scripted touch-then-build flows hit it.

Numbers (load 4–6): `./silva/examen.sh lib/xar.c` 0.6 s → 0.27 s (binary
alone 0.08 s). ~330 wrapper calls per merge commit.

Test gap closed: nothing tested the header-dir scan (a plant making
`caput_recentius` always "fresh" left porta nexus-instrumentorum green).
New stage VI in tools/instrumenta_nexus_fumus.sh: touch a silva/fontes
header → examen recompiles ([dep] lines) and relinks; mtime restored with
`touch -r` so nothing cascades. The same plant now turns VI red.
