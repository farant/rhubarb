
## 2026-10-31 — scans narrowed to the filter directory (budget B2 H2)

Every gate caller (oratio, gesta, officina, tessera) passes a trailing-/
filter such as `gesta/build/`, yet each call walked the whole tree three
times with /usr/bin/find, with ~415k entries under build/ (fabrica
areas, the aedilis thesaurus): 51-56 s per call, real I/O-bound.
1. The graph-freshness check filtered build/ with `-not -path`, which
   still stats everything; `-prune` gives the same set in the same order
   (no source is named `build`).
2. The object stream: with a trailing-/ filter that names an existing
   directory, only that directory is scanned. The awk judge inspects only
   objects that `congruit()` (directly in the filter dir). Objects
   elsewhere entered `mtempus` but never decided another verdict:
   fabrica res read their sources through the M stream.
Old vs new, byte-for-byte, on all five caller filters with a touched
gesta header (non-empty STALA lists, 17-76 entries): IDENTICAL,
51-56 s → 0-1 s.

New gate `excubitor` (tools/excubitor_fumus.sh, synthetic tree in a temp
dir): no gate asserted excubitor verdicts before (diagnostica only greps
the signature). Covers the narrow path, the full path and the pruned
freshness check. Plant: narrow path `*.c` instead of `*.o` (always PURUS)
→ FRACTUM. Note: a new gate script is invisible to the gate registry
until git tracks it (portae CLI reads `git ls-files`); `git add -N`
before running portae-declaratae.
