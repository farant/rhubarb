# compile_tests.sh worklog

## 2026-10-23 — radix gate overhead outside fabrica: generare in batches, QR oracle as its own gate

**Measured.** The radix gate is `--residua-prae` → `bin/fabrica probare probationes_radicis` → `--residua-post`. Traced with Homebrew bash 5 (`COMPILE_TESTS_EFFUSIO=1` skips the tee re-exec, otherwise the child runs untraced under /bin/bash 3.2; `PS4='+ $EPOCHREALTIME ${FUNCNAME[0]} '`). The warm prae step took 9.7 s:

| step | time |
|---|---|
| `probatio_qr_gyrus.sh`, inside `run_speculum` | 4.83 s |
| library compiles (352 `bin/compilator` cache hits via xargs; ~22 ms per call) | 1.92 s |
| `bin/generare`, once per test file (377 processes) | 1.67 s |
| `amalgamata_probare.sh` | 0.88 s |

`--residua-post` (plutil/iconutil oracles) was 1.9 s.

**generare.** `run_generare` passes the list in batches of 100 via `xargs -n 100`: 1.68 → 0.05 s. generare already took many paths and stops at the first failure, naming it ("terminatur propter fracta in <via>"). A single call failed: argumenta.h caps positionals at 128 ("Nimis multa positionalia"), hence the batches.

**QR oracle.** It is now the gate `qr-gyrus`:
- annotation in probationes/probatio_qr_gyrus.sh: `tegit` = the script and qr_assets, `currit` = the five C files it compiles, so header changes in that closure owe it;
- `PORTAE` entry in silva.py; verdict line `QR GYRUS: n/n legibiles | k ex n FRACTAE`.

`run_speculum` skips it when `RESIDUA_MODE` is set; the plain full run (frigida) still runs it. Checks:
- `portae_debitae`: lib/qr.c, include/qr.h, include/piscina.h and qr_assets owe qr-gyrus; lib/stml.c does not;
- portae-declaratae fumus: sanum.

**Result.** Warm prae step 9.7 → 4.2 s on every radix gate run.

**Plants.**
- A version-information placement bug (`(i + I) % III` at qr.c:456) stayed GREEN. The version information has its own BCH protection, and decoders infer the version from the symbol size, so this oracle cannot see that class of bug. That is worth knowing; the structural tests in probatio_qr.c are what guard it.
- Inverting mask 0 (`(x + y) % II == I`) is RED: 5/40 symbols unreadable (ECC-M V2 decoded 14 of 26 octets). Restored, green.

**Still in the prae step:** library compiles (1.9 s wall, about 7.7 s CPU; a `bin/compilator` batch mode would amortize the per-call startup and compiler identification) and `amalgamata_probare` (0.9 s; could also become its own gate, as could the post-step oracles).

## 2026-11-02 — budget B2 G6/J5b: tests link an archive, one full link per run

Every radix test linked all 419 library objects (+4 frameworks): an
18.4 MB binary each, ~8 GB written per frigida. Fran's ruling (J5b,
tabularium decision 01M6FQ3GYV): link each test against
`build/probationes/libradix.a` (`ar rcs`, once per run, ~0.3 s) with
`-ObjC`, and keep the whole-library guarantees — no duplicate globals,
no unresolved externs, which the per-test link proved as a side effect
— with ONE link per run: an empty `main` with `-Wl,-all_load` against
the same archive (`nexus_radicis_parare`, called once before the tests
on both the serial and the FILA path; failure = run fails, counted as
"nexus_plenus").

Trap found while prototyping: an EMPTY archive (zsh didn't word-split
the object list, so `ar` got one giant name) made the `-all_load` link
pass. The runner now checks the member count (minus __.SYMDEF) against
the object list.

`-ObjC` is insurance: no .m defines a category or looks a class up by
name today, so nothing exercises it (binary 332 KB with it vs ~130 KB
without). `build/test_logs/obiecta_probationum.txt` is no longer
written (only a frozen computus fixture names it).

Plants (lib/cursor.c, which probatio_xar never pulls in): a second
definition of COLOR_NIGER -> "duplicate symbol '_COLOR_NIGER'", NEXUS
PLENUS FRACTUS; a call to a function defined nowhere -> "Undefined
symbols", NEXUS PLENUS FRACTUS. In both, probatio_xar itself still
passed — under the old scheme every test would have failed instead.
Restored green.

Full run: 434/434 passed, 223 s (wall unchanged — the suite is mostly
waiting); build/probationes 5.9 GB -> 752 MB.
