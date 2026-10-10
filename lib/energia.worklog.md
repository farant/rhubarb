# energia - worklog

## 2026-10-10 - born (vicus S5, Fran: battery % in the tab bar)

- include/energia.h has NO aedilis corpus line on purpose: aedilis
  resolves header -> lib/energia_macos.(m|c) then energia_posix then
  energia.c (variantes praelatio="macos posix"). macOS reads IOKit
  power sources (kIOPSInternalBatteryType, current/max capacity,
  IsCharging); posix reports no battery.
- .m, not .c: CoreFoundation headers are not C89, and the house
  latina.h macros ('character' -> char, Roman numerals I/C/...) corrupt
  them - system headers go BEFORE energia.h (and latina.h).
- Linking: aedilis.stml nexus rule energia_macos -> IOKit +
  CoreFoundation; compile_tests.sh's FIXED framework list needed
  -framework IOKit (4 link lines); compile_tests_fontes_generata.sh
  regenerated (the new principal pulls lib/energia_macos.m).
- Oracle = the machine: first read gave "7% (oneratur)", identical to
  `pmset -g batt` ("7%; charging"). The smoke test only checks sanity
  (0-100, stable across two reads, NIHIL refused).
- vicus reads it through a port (VicusPila) at each minute tick with
  the clock; the apps install an energia_status adapter; tests a fake.
- Gate surprise (radix composite, RESIDUA icones_iconutil): FOUR more
  scripts link EVERY object in build/ with a hard-coded framework list
  - compile_tools.sh, run_lint.sh, tools/silex_struere.sh (and
  compile_tests.sh). Once energia_macos.o sits in build/, each needs
  -framework IOKit or fails with _IOPSCopyPowerSourcesInfo undefined.
  House debt: these lists should come from aedilis.stml's nexus rules
  (one flag source), not be repeated per script.
