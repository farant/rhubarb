# officina/compile_probationes.sh worklog

## 2026-11-10 — the suite was red ALONE in a fresh clone (symbol index)

Budget fusionis VII (11-09) noted that `./tools/frigida_probare.sh
-suitae officina` is red while the full cold run is green. Reproduced
on 08eb5c15 with `-servare`: probatio_officina_lanceatores and
probatio_officina_legatus.

Evidence from the kept clone (`build/` held aedilis, fabrica, test
logs - no nexus.tsv):
- lanceatores, "interpretare sine plagulis" (l.82): stderr was
  "interpretare: index symbolorum absens (./silva/nexus.sh -renovare)
  - mundus totus per bibliothecas (~1 min)... mundus: 319 moduli";
  the test expects "mundus: 1 moduli". Same cause for "interpretare
  fasti" (l.111, "mundus totus" must not appear).
- legatus: "legatus: nexus.tsv deest (.../build/nexus.tsv) - index
  superpositione sola crescet", then 10+ assertions of the MCP mode
  (symbols, callers, inclusions) fail.

Cause: both tests read the REAL tree's symbol index (build/nexus.tsv,
build/inclusiones.tsv). Nothing in this runner made it; it existed
only because the root suite (or oratio, whose runner renews it) had
run earlier in the same tree. The tools themselves behave correctly
without an index (they say so and fall back).

Fix: the runner renews the index before the tests, the same line
oratio's runner has had since 2026-09-07 (`./silva/nexus.sh
-renovare`, log in officina/build/nexus_renovatio.log; a failed
renewal is a CAUTIO, the tests then speak for themselves). Cost:
1.3 s when nothing changed; a full sweep (~35 CPU s) in a cold tree
that has no index yet - in the full cold run the root suite has
already paid it.

This was one of the two things in the way of running the cold suites
in parallel (B7); the suites still share what radix builds (compiled
objects, tools), so "independent" is not yet true for wall time.
