# tools/venenum_probare.sh worklog

## 2026-10-22 — suites run as the fabrica composite probationes_veneni

Section I (the specimen: poisoned read 165, pure read 42) is unchanged: it proves poison mode is live AND normally off. Section II used to be a hand loop: one clang invocation per suite with a hand-maintained source list plus BASIS. Each suite's whole chain was recompiled under ASan, ~487 s every run. It is now `bin/fabrica probare probationes_veneni` (aedificatio.stml):
- members are named with `membra=`, and their sources come from the aedilis closure;
- `vexilla=` carries the old SANITAS + `-Wno-overlength-strings -DPISCINA_VENENUM=1`, and `vexilla_nexus=` the sanitizer link flags;
- the flags are part of each member's key.

The final line keeps its contract (`venenum: sana` / `venenum: FRACTA (n)`, the regex in silva.py). fabrica exit 2 (nothing ran) becomes `venenum: NULLA PROBATIO CURSA`, exit 2. SANITAS still exists for the specimen, so changing the flags means editing both the script and the actio (the comment says so).

Cold 432 s (probatio_cellae alone is 306 s under ASan; it is also radix's 92 s critical path, so the next target), warm 1.2 s. Plant in lib/fabrica.worklog.md (same date).
