# nexus worklog

## 2026-10-03 - build through bin/compilator (fabrica plan 2 follow-up)

Found by timing the pythonica gate (492 s): one test, `silva.usus()`,
took 189 s. `usus` calls `silva/nexus.sh`, which self-heals the index
before answering. The cost was never the query: nexus.sh rebuilt its
own tool under mtime rules ("any include/*.h newer than an object
recompiles ALL of them", for both the -O0 set and the -O2 -flto sweep
set), and a relinked sweep tool forces "instrumentum novum -> plenus" =
a FULL sweep (now ~190 s, 1685 files - the "~55 s" in the script was
from a smaller tree). So any commit touching include/ or lib/, even a
header nexus never includes (include/fabrica.h), made the next nexus
caller pay ~3 minutes: the pythonica gate, the Latin lint (probably the
two 188 s lint outliers, ledger …HBJD4), lectiones_lint, legati.

Change: every object goes through bin/compilator (plain set, celer set,
amalgam, ordines, and now the CLI and sweep mains as objects too).
compilator rewrites an object only when its BYTES change, so "binary
strictly newer than every object" means "nothing changed": the CLI
(relinked on EVERY query before) and the sweep tool relink only then,
and "instrumentum novum -> plenus" now fires only when the sweep tool
really changed. The principle stays (a changed tool may judge
differently, so the table is rebuilt whole); only its trigger moved
from mtime to content. Local `recentius` check, not nexus_recens.sh's
binarium_recens: that one always says stale under FABRICA_AGIT (an
installer rule) and nexus runs under fabrica actions. Without
bin/compilator (fresh clone, build fails): plain clang with the old
rules - slow, correct, never missing.

Evidence: full index from the new script byte-identical to the old
script's (only the GENERATUM timestamp line differs), also after two
edit+restore incremental runs. Comment added to include/fabrica.h: 2.2 s,
no full sweep (was ~190 s). Comment appended to lib/filum.c (in nexus's
closure): 2.2 s - compilator missed on the source but the object bytes
did not change, so no relink. Query after restore 1.8 s, then 1.48 s.
Fallback path (compilator forced off): exit 0, old behaviour (213 s
with a full sweep), and the next normal run 1.48 s.
A real code change in a file nexus links still triggers a full sweep,
by design.
