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

## 2026-10-27 - root runner on Linux

`./compile_tests.sh --obiecta` now builds all 391 root objects on
Linux. What it took:
- the generated source list (compile_tests_fontes_generata.sh) is
  produced on macOS. On Linux the runner maps `_macos.c` -> `_posix.c`,
  and `_macos.m` -> its `_posix.c` sibling when one exists (energia,
  fiducia, nomina, tls_securetransport); `.m` without a sibling
  (fenestra, vitrea, clipboard_platform) is dropped. New
  lib/clonatio_posix.c (ENOTSUP, which the header already allows).
- bin/aedilis (builds bin/compilator) picks posix through
  `aedilis_variantes_hospiti_aptare` - tools only; the library still
  honours the configured order (probatio_aedilis fixtures).
- aedilis-emitted scripts add `-lm` off Apple.
- VENDOR_FLAGS + aedilis.stml: `-DBR_ENABLE_INTRINSICS=1` for
  bearssl (x86: pclmul path fails without it; the linux/android/ios
  scripts already passed it; arm64 macOS: no effect).
- compile_tools.sh on Linux links through an archive (.a) of the
  objects, not every .o: fenestra/vitrea have no Linux build, and
  linking all objects made every tool fail on objects that call them.
