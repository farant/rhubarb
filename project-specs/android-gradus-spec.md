# Android build tier - house C on the Fire tablet (spec)

**Status: IMPLETUM 2026-10-23** - built in quarta per
`project-specs/android-gradus-plan.md` (four tasks, Fran approving
each): T1 eb06acad (aedilis `--varians` list), T2 7714bb0c (volumen
TMPDIR), T3 aa63a0dd (NDK pin + installer), T4 (runner; plus machine
identity on Android, VI below). Live on the Fire 7 (KFQUWI, API 30,
arm64): `piscina chorda xar json volumen` PRAETERITAE, 2655
assertions. Not yet in main. Approved 2026-10-23 (Fran: "i think that
is a good spec"); designed in conversation 2026-10-22 section by
section. Worktree quarta. Ledger: vision desideratum ...577GAS
(`01M5KAM5ATMEGGA7ZTFE577GAS`), spike note ...RMQ67J
(`01M5KD9MH0WD9HXX1SGTRMQ67J`).

*Fran 2026-10-22: Android first ("there definitely will be android
apps (probably a drawing app based on pictor first) and the first real
published app will be on the amazon fire app store"); "we do have fire
tablets we can test on"; on scope: "three sounds like a good start"
(the minimal tier); on the device: "if we are creating any
build/temporary files that might add up over time on the tablet we
might want to consider the cleanup lifecycle because tablet has less
disk space than we're used to".*

Goal: **house C libraries build for Android and their credo suites
run green on a real Fire tablet**, through one manual command. This is
sub-project A of the mobile line. Sub-project B - `fenestra_android`,
the platform layer (window, input, lifecycle, JNI touchpoints) - gets
its own spec once A exists.

Decisions (Fran, 2026-10-22, in conversation):

- **Minimal tier first** (option 3 of 3): the variant list, the NDK
  pin, the `TMPDIR` fix and the runner, proven on a handful of
  foundation suites. Rejected for now: a broad port of every
  Linux-proven suite (option 1) and a pictor-shaped slice (option 2).
  The runner takes ANY suite name, so going broad later is a matter of
  naming more suites - each one graduates when a consumer needs it.
- **A thin runner script modelled on `tools/linux/probatio.sh`**
  (approach 1): cross-compile on the Mac with the NDK clang, push,
  run, read credo's verdict. Rejected for now: an aedilis cross target
  with cached objects (approach 2) - right for building app `.so`
  files in spec B, too much surface for proving suites; a fabrica gate
  (approach 3) - a gate that needs a plugged-in tablet cannot be a
  merge requirement, the same reason the Linux runner is manual.
- **Cleanup is designed in, not bolted on** (II.5): one root on the
  device, wiped at start and at end (trap), footprint reported.
- **`postulata_android.h` moves to spec B**: in this slice no file
  includes an `<android/*.h>` header, so the header would have no
  consumer and no test. Its content is already known (II.6).

## I. What exists (quarta 5c65f186, 2026-10-22)

- **Toolchain on Fran's Mac** (memory `android-toolchain`): NDK r27d
  LTS `27.3.13750724` at `~/Library/Android/sdk/ndk/27.3.13750724`
  (clang 18, universal binary); SDK platform-tools (`adb`), build-tools
  34.0.0, platforms/android-34. No `sdkmanager`.
- **Test tablet:** Fire 7 12th gen, model KFQUWI, Fire OS 8.3.3.3 =
  Android 11 / **API 30**, `ro.product.cpu.abilist` =
  `arm64-v8a,armeabi-v7a,armeabi`, `/data` 10 GB with 6.0 GB free
  (`adb shell df -h /data`, 2026-10-22). Shell environment:
  `TMPDIR=/data/local/tmp`, `HOME=/`, **no `/tmp`**.
- **Proven by the spike** (ledger note ...RMQ67J): house flags compile
  for `aarch64-linux-android30` and `armv7a-linux-androideabi30`; a
  plain C89 executable pushed to `/data/local/tmp` runs from
  `adb shell` (i64 8 bytes, pointers 8 bytes); a NativeActivity APK
  runs single-threaded at ~59 fps.
- **aedilis variants:** the configured preference is a list
  (`aedilis.stml:62`, `<variantes praelatio="macos posix"/>`), but an
  explicit `--varians X` means **X alone**
  (`lib/aedilis.c:879-898`: "-varians dato = illa sola; aliter
  praelatio configurationis ordine"). Android needs `android` first
  and `posix` as fallback - not expressible today.
- **Linux runner** `tools/linux/probatio.sh` (86 lines): closure via
  `bin/aedilis PROBATIO --varians posix --partes` (`O` lines = objects,
  `V` lines = vendored sources), compile in a container, run, exit
  0/1/2. Its flags are written inline (line 45, vendor flags line 46), not taken from
  `tools/vexilla.sh`.
- **Flags:** canonical list `VEXILLA_C89` in `tools/vexilla.sh:24`;
  `aedilis.stml:40-60` carries the same plus `-O2 -g`.
- **Temporary directories:** `credo` honours `$TMPDIR`, falling back
  to `/tmp` (`lib/credo.c:1763-1767`). **`volumen` does not**: the
  temporary volume path is built from a literal `"/tmp/"`
  (`lib/volumen.c:390`), which does not exist on Android.
  Other literal `/tmp` uses, NOT changed by this spec (no consumer in
  the minimal set): `lib/hospitium.c:1139`, `lib/imperium.c:563`,
  `lib/specimen.c:454`, `lib/mensura_contextus.c:277,295,297`,
  `lib/silex.c:1949`, `lib/fabrica.c:641` (a path classifier, not a
  writer).

## II. Design

### II.1 aedilis: a variant preference list

`--varians` accepts a comma-separated list: `--varians android,posix`
resolves each unit as `X_android.c` if it exists, else `X_posix.c`,
in the given order - the same walk the configured `praelatio` already
does. A single name (`--varians posix`) behaves exactly as today. The
host default in `aedilis.stml` is unchanged; `android` appears only
when a caller names it. An empty element (`android,,posix`) or an
empty list is a named usage error (exit 2).

### II.2 volumen honours TMPDIR

`volumen_temporarium` builds its base from `$TMPDIR` when set and
non-empty (trailing `/` trimmed, as credo does), else `/tmp`. On macOS
nothing changes in practice (`TMPDIR` is set and valid); on Android it
lands under the runner's root (II.5).

### II.3 The NDK pin and its installer

- `tools/android/ndk.stml` - data, one record: version
  `27.3.13750724`, release name `r27d`, URL
  `https://dl.google.com/android/repository/android-ndk-r27d-darwin.zip`,
  size `839006233`, Google manifest SHA-1
  `2970926d705988f79baa9b04c51b4f7914dd8c56`, our SHA-256
  `e69092f9d2bfa5d1199039980a14eb91c03cc971ab5c6968fc08a8e6b84e7bb7`,
  minimum API `30`.
- `tools/android/ndk_instituere.sh` - downloads into a fresh temporary
  directory, refuses (exit 2, naming which) on a size, SHA-1 or SHA-256
  mismatch, unpacks into `~/Library/Android/sdk/ndk/<version>`
  (the standard side-by-side layout); an existing correct install is
  reported and left alone. Run by hand, never by a gate.

### II.4 The runner: `tools/android/probatio.sh`

Usage: `./tools/android/probatio.sh [-servare | -purgare] <suites...>`
(names as for the Linux runner: `piscina` = `probationes/probatio_piscina.c`).

1. **Preflight** (each failure exits 2, cause named): the pinned NDK
   present at the pinned version (else name `ndk_instituere.sh`);
   `adb` found; exactly one authorized device, or `ANDROID_SERIAL`
   chooses (none, several, or `unauthorized` each named); device ABI
   read - `arm64-v8a` -> `aarch64-linux-android<api>`, `armeabi-v7a`
   -> `armv7a-linux-androideabi<api>`; device API below the pin's
   minimum refused.
2. **Host build**, per suite: closure from
   `bin/aedilis PROBATIO --varians android,posix --partes`; an `.m`
   source in the closure is a named refusal (the suite drags in
   Mac-only code); `O` sources compiled with the NDK clang and
   `VEXILLA_C89` from `tools/vexilla.sh` plus `-O2 -g`; `V` sources
   with the Linux runner's vendor flags. Output in
   `build/android/<abi>/<suite>/`, cleared before each suite's build.
   The binary pushed is a stripped copy (`llvm-strip`); the unstripped
   one stays on the host for symbolising a crash.
3. **Run:** each suite runs from the device root with
   `TMPDIR=<root>/tmp`; output captured to the host.
4. **Verdict:** per suite the tail of its output and credo's `Fracti`
   count; whole log in `build/test_logs/android.log`. Exit 0 all
   passed, 1 any failed (a compile failure counts, named), 2 apparatus.

### II.5 Device lifecycle (cleanup)

- **One root:** `/data/local/tmp/rhubarb/`. The runner never writes or
  removes anything outside it.
- **Start of run:** remove the root entirely (also clears what a
  killed run left), recreate it with `tmp/` inside.
- **During:** everything the suites write - credo scratch, temporary
  volumes (II.2) - goes under `<root>/tmp` via `TMPDIR`.
- **End of run** (normal, failure, or Ctrl-C - shell `trap` on EXIT,
  INT, TERM): measure the root (`du -sk`), print
  `vestigium in tabula: <N> KiB, purgatum`, remove it. A footprint
  that grows between runs is visible in that line.
- `-servare`: keep the root after the run and print its path (for
  debugging on the device). `-purgare`: remove the root and exit 0,
  running nothing.
- Host side: `build/android/` is regenerable build output (cleared per
  suite, II.4).

### II.6 Carried to spec B (known, not built here)

`include/postulata_android.h` - includes `postulata_posix.h`, then
`#define inline static __inline__` before any `<android/*.h>`. The
NDK headers use C99 `inline` unguarded (`android/native_window.h:375`
in r27d); under `-std=c89` clang applies GNU89 inline rules, where a
bare `inline` header function is EMITTED as an exported definition -
in the spike `ANativeWindow_clearFrameRate` leaked into the `.so` and
pulled the API-31 symbol `ANativeWindow_setFrameRateWithChangeStrategy`,
so `dlopen` failed on API 30 (shown with `llvm-nm -D`). `static`
keeps unused header functions out. Spec B also teaches examen the
`<android/*.h>` names, and decides the aedilis cross target for app
builds (approach 2 above).

## III. Tests

- **aedilis** (its suite, fixture corpus): `--varians android,posix`
  picks `X_android.c` when both exist; falls back to `X_posix.c` when
  only it exists; a unit with neither is the existing named failure;
  `--varians posix` alone unchanged; empty element refused. Planted:
  reverting to first-element-only turns the fallback case red.
- **volumen** (`probatio_volumen.c`): with `TMPDIR` set to a fresh
  directory, `volumen_temporarium` creates the volume under it; with
  `TMPDIR` unset or empty, under `/tmp`; trailing `/` trimmed. Planted:
  the literal `/tmp/` back -> the TMPDIR case red.
- **Runner, on the Fire** (manual tool, evidence recorded in the
  plan's ledger):
  1. green: `piscina chorda xar json volumen` all pass on the device
     (`volumen` proves II.2 where `/tmp` does not exist);
  2. red: a planted assertion (`silva.planta`) -> exit 1, suite named;
  3. apparatus: tablet unplugged -> exit 2, cause named; wrong NDK
     path -> exit 2 naming `ndk_instituere.sh`;
  4. cleanup: after a normal run and after a Ctrl-C'd run,
     `adb shell ls /data/local/tmp` shows no `rhubarb/`; after
     `-servare` it does, and `-purgare` removes it.
- **Installer:** a corrupted download (one byte changed in a copy)
  is refused with the mismatch named; the correct pin is accepted.

## IV. Not in this spec

- `postulata_android.h`, `fenestra_android`, APK packaging, our own
  APK signer, JNI - spec B and later.
- An aedilis cross target / cached Android objects (approach 2).
- Porting every Linux-proven suite - each graduates when named.
- Running suites in the Android emulator (the real tablet is the
  oracle; the emulator image exists if ever wanted).
- The other literal `/tmp` uses listed in I.
- Store APIs (Amazon Appstore submission, App Store Connect) - their
  own specs.

## V. Order of work (the plan will follow approval)

1. aedilis variant list (II.1) + tests.
2. volumen TMPDIR (II.2) + tests.
3. NDK pin + installer (II.3).
4. Runner + device lifecycle (II.4, II.5), proven on the Fire (III).

## VI. Found while building (2026-10-23)

- **Machine identity on Android** (Fran, option 1 of 3, 2026-10-23:
  "i think option 1 makes sense"). `volumen` stamps every record with
  `machina_identitas`, whose sources are `gethostuuid()` (macOS) and
  `/etc/machine-id` (Linux); Android has neither (checked on the
  tablet), so the identity was empty and `volumen_creare` refused. New
  `machina_identitas_ex_plagula(via, piscina)` (`include/machina.h`,
  `lib/machina_posix.c`): a random identity written ONCE under a lock
  (32 hex from `moneta_octeti_fortuiti`, temp file mode 0600, atomic
  rename) and read thereafter - what `/etc/machine-id` itself is; a
  present but short file is refused and never rewritten. Android
  branch of `machina_identitas`: `$HOME/.rhubarb/machina` (meaning:
  this installation); the runner sets `HOME=<root>`, a future app
  layer its private files directory. Rejected: `ro.serialno` (hidden
  from apps since API 29), `ANDROID_ID` (Java only - spec B).
- **Test-side portability:** `probatio_volumen.c` hardcoded
  `/tmp/probatio_volumen_ordinarium.volumen` -> `build/...` like its
  neighbours. Still hardcoded, NOT ported: `probatio_volumen_rivi.c:47`
  (`/tmp/volumen_rivi_*`) - fix when that suite is named.
- **Runner:** text tools under `LC_ALL=C` (macOS `tr` refused the
  json suite's UTF-8 output and lost the EXITUS line); cleanup
  messages written to the saved stdout (fd 3) - a TERM arriving while
  a suite's output was redirected put the "purgatum" line into that
  suite's `effusio.txt` (the cleanup itself always ran).
- **aedilis exit code:** a malformed `--varians` list exits 1 (the
  tool's convention for every refusal), not the 2 written in II.1.

## AUDIENDA - answered 2026-10-23

- Bionic compatibility: all five suites compiled against bionic with
  the house flags UNCHANGED; the only failure was the missing machine
  identity (VI), a platform fact, not a compile problem.
- Repository files: none of the five READS a repository file;
  `probatio_volumen` WRITES `build/*.db` relative to cwd - the runner
  creates `<root>/build/` (plan R3).
- `llvm-strip` and `du -sk`: both behaved as assumed (footprints
  92 KiB for piscina alone, ~2 MiB for all five).
- Vendor flags: sqlite (pulled by volumen) compiled for Android with
  the Linux runner's flags unchanged.
- Still open: the vendor flags for BearSSL (no suite in this slice
  pulls it).
