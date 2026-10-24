# Testa iOS (C1) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (Fran chose inline, one task per turn, Fran approves each). Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** the Fire's pictor runs on Fran's iPhone 15 Pro Max in portrait at scale II - first in the iOS Simulator - through a third `testa` shell (UIKit) and a no-Xcode-project build script.

**Architecture:** `lib/testa_ios.m` is the `ios` variant of `testa.h` (aedilis picks it under `--varians ios,posix`); it reuses the host-tested pure pieces (renamed `testa_initus`) for geometry, touch and frame cadence. `tools/ios/app.sh` builds a `.app` with `xcrun clang`, signs it (ad hoc for the simulator, the wildcard development profile for the device) and installs with `simctl` / `devicectl`.

**Tech Stack:** C89 in Latin (latina.h), Objective-C/UIKit for the shell only, credo v2, aedilis, Xcode 16.1 (iOS 18.1 SDK, simctl, devicectl 397.24), codesign.

**Spec:** `project-specs/testa-ios-spec.md` (quarta f2c8873e, approved 2026-10-23).

## Global Constraints

- C89 + house flags for every `.c`; Latin identifiers; latina.h macros (Roman numerals, `nomen`, `brevis`, `per`...) are never identifiers; Latin lint blocks commits.
- No Xcode project, no XcodeGen (D3). No second thread (B1 D1).
- Bundle id `earth.golden.<app>`; portrait only; deployment target iOS 17.0; scale II (D5).
- Device signing: the wildcard profile "iOS Team Provisioning Profile: *" (App ID `Q5WC9FQ3J2.*`) and identity "Apple Development: Francis Arant (44TZ4R8793)" (D7). No portal work, no App Store Connect key in C1.
- UIScene lifecycle (D9).
- A smoke run never uninstalls an app that was installed before it (B1 review I1 - the phone will hold drawings).
- GUI windows on the Mac (pictor/scriba/vicus `-fumus -imago`) only with Fran's OK; the simulator window too.
- Commit with `silva.commissio`; pre-heal `build/capsula_corpus_silicis.c` + dependents before it (park ...QX6SQ0); logs outside the tree.

## Review Focus

1. The phone has drawings and a smoke run (simulator or `-tabula`) is started - expected: the app and its Documents survive. Pinned in Task 3 (`app.sh` smoke on a preinstalled simulator app) and Task 4.
2. Home swipe or a call while a finger is down - expected: `touchesCancelled` -> exactly one `MUS_LIBERATUS`, no stuck stroke after return. Pinned: Task 1 mapping row "cancelled" + existing abruptio section; Fran by hand in Task 4.
3. A second finger lands while drawing - expected: ignored, the first keeps drawing; new touch works after all lift. Pinned: Task 1 mapping rows (began with one active -> POINTER_DOWN).
4. The safe area changes while running (status bar height, rotation attempt) - expected: layout follows or stays portrait, no crash, no lost drawing. Pinned: Task 3 shell handles `viewSafeAreaInsetsDidChange` through the same `_aream_aptare`-style path; checked by hand in the simulator (Task 3) and on the phone (Task 4).
5. Xcode 16.1 vs iOS 26.4 refuses install/launch - expected: `app.sh` names the cause (exit 2) and the D8 upgrade is raised with Fran, never retried blindly. Pinned in Task 4 step 2.

## Rulings (made while planning)

- **R1 iOS detection without headers:** clang predefines `__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__` for iOS device and simulator targets; shared C uses it (no `<TargetConditionals.h>` in C89 libs). Measured: pictor's closure compiled for `arm64-apple-ios17.0-simulator` breaks only in `lib/machina_posix.c` (`gethostuuid` unavailable); `ludus_fenestra_currere` would break at link (no `fenestra_*` on iOS).
- **R2 iOS machine identity** = the per-install file path of Android, directory `$HOME/Library/.rhubarb` (the app container's `Library/` is writable and backed up; the container root may not be).
- **R3 layers icon title** `"strata:quadratum"` (drawn as an icon by `pictor_figurae`; the old `"strata:stratum N"` text title is gone); tool name: the status row's title becomes empty in the normal (non-paste) branch. The paste-pending branch ("glutinatio: A Atkinson, B exemplaria") is untouched (no paste on iOS in C1).
- **R4 smoke stroke on iOS:** `simctl` cannot inject touches; in smoke mode the shell itself feeds a synthetic DOWN / MOVE x8 / UP through `testa_actio_ex_phase` + `testa_tactum_vertere` after frame 5, from (0.3 w, 0.3 h) to (0.7 w, 0.45 h) of the safe area - the real touch path minus UIKit.

## File map

| File | Task | Role |
|---|---|---|
| `include/testa_initus.h`, `lib/testa_initus.c`, `probationes/probatio_testa_initus.c` (renamed from `testa_android_initus`) | 1 | pure pieces + UITouch phase mapping |
| `lib/testa_android.c` | 1 | include renamed header |
| `lib/pictor_componentia.c`, `lib/pictor_figurae.c` | 2 | layers icon square, no tool name |
| `lib/machina_posix.c`, `include/machina.h`, `lib/ludus_fenestra.c`, `include/ludus_fenestra.h` | 3 | iOS branches (R1, R2) |
| `lib/testa_ios.m`, `aedilis.stml` (`<nexus>` rule) | 3 | the shell |
| `tools/ios/app.sh`, `tools/ios/Info.forma.plist`, `tools/ios/communia.sh` | 3-4 | build / sign / install / smoke |
| `tools/ios_struere_porta.sh`, `pythonica/silva.py` | 4 | gate `ios-struere` |
| `project-specs/testa-ios-spec.md` | 4 | status IMPLETUM + VI |

---

### Task 1: platform-neutral pure pieces + UITouch phase mapping

**Files:** rename `include/testa_android_initus.h` -> `include/testa_initus.h`, `lib/testa_android_initus.c` -> `lib/testa_initus.c`, `probationes/probatio_testa_android_initus.c` -> `probationes/probatio_testa_initus.c` (`git mv`); modify `lib/testa_android.c` (include), `compile_tests_fontes_generata.sh` (regenerated by `bin/fabrica sanare compile_tests_fontes_generata.sh`), `project-specs/testa-spec.md` mentions.

**Interfaces:**
- Produces (in `testa_initus.h`): everything the old header had, unchanged names; plus
  - `#define TESTA_PHASIS_INCIPIT ZEPHYRUM`, `TESTA_PHASIS_MOVETUR I`, `TESTA_PHASIS_QUIESCIT II`, `TESTA_PHASIS_FINIT III`, `TESTA_PHASIS_ABRUPTA IV` (UITouchPhase Began/Moved/Stationary/Ended/Cancelled values)
  - `s32 testa_actio_ex_phase(i32 phasis, i32 activi_ante, i32 activi_post)` -> a `TESTA_TACTUS_*` action, or `-I` = nothing to deliver

- [ ] **Step 1: Rename** with `git mv`; header guard `TESTA_INITUS_H`; header comment "partes PURAE testarum Android et iOS"; update the include in `lib/testa_android.c` and the suite; regenerate the source list. Run `./compile_tests.sh probatio_testa_initus` -> 9/9 sections; `./tools/android_struere_porta.sh` -> viridis.
- [ ] **Step 2: Failing test** `_sectio_phases` in the suite: rows (phasis, ante, post) -> expected: (INCIPIT,0,1) DEPRESSIO; (INCIPIT,1,2) INDICATOR_DEPRESSIO; (MOVETUR,1,1) MOTUS; (QUIESCIT,1,1) -I; (FINIT,1,0) LIBERATIO; (FINIT,2,1) INDICATOR_LIBERATIO; (ABRUPTA,1,0) ABRUPTIO; (ABRUPTA,2,0) ABRUPTIO; unknown phasis 9 -> -I. Run: link fails on `testa_actio_ex_phase`.
- [ ] **Step 3: Implement** `testa_actio_ex_phase` in `lib/testa_initus.c` (pure table; `ante`/`post` = active touches before/after this touch's change).
- [ ] **Step 4: Green** on host (10/10) and on the Fire if plugged in (`./tools/android/probatio.sh testa_initus`; else ledger "not device-run").
- [ ] **Step 5: Commit** (lint first: `TESTA_PHASIS_*`, `quiescit`, `abrupta` are Latin).

### Task 2: layers icon square, no tool name (D6)

**Files:** modify `lib/pictor_componentia.c:673-676` (status title) and `:778-791` (layers box), `lib/pictor_figurae.c` (icon for `"strata:quadratum"`); suites/fixtures that assert the old status text or `"strata:stratum N"` (find with `grep -rn 'strata:stratum\|"penicillus"' probationes/ lib/*.interrogationes.stml`).

**Interfaces:**
- Consumes: nothing new.
- Produces: the layers square = node `"quadratum.strata"`, 20 x 20 (`XX`), at `x = fs.x + fs.latitudo - cw - XX`, same `y` as the other squares, title `"strata:quadratum"`, action `"palette.aperire"` (unchanged), still a child of the ROOT (not of the status line). Status line title `""` in the normal branch.

- [ ] **Step 1: Baseline before** (Fran's OK for windows): `pictor -fumus -imago build/testa/paritas/pictor.ante_icon.png` and keep the T1-era `scriba`/`vicus` PNGs.
- [ ] **Step 2: Failing test** in the pictor composition suite (the one that already checks `quadratum.*` nodes - `grep -ln 'quadratum.instrumentum' probationes/`): the layers node has latitudo XX and title `"strata:quadratum"`; the status node's title is empty; at a 215 x 419 surface no two status-row squares overlap (rectangles of `quadratum.*` + `quadratum.strata` pairwise disjoint). Run: red.
- [ ] **Step 3: Implement** the composition change (R3) and the icon branch in `pictor_figurae.c` (three stacked offset rectangles in the theme's foreground colour, inside the square's 1 px border like the other icons; the tool-square drawing code is the model). Update any assertion found by the grep in Files (ledger each).
- [ ] **Step 4: Green:** pictor suites + `./compile_tests.sh probatio_pictor` (all pictor suites); headless render at 215 x 419 with the Task-0-era spike (`build/spica/reddere_telephonum` rebuilt) -> open the PNG: icon, no overlap, no tool name.
- [ ] **Step 5: New desktop baseline:** `pictor -fumus -imago build/testa/paritas/pictor.icon.png`; scriba byte-identical to its old PNG; vicus by eye (its pictor pane shows the new row). Show Fran the pictor PNG once.
- [ ] **Step 6: Commit** (pictor goldens, if any, regenerated with the cause named).

### Task 3: the iOS shell and the simulator

**Files:** create `lib/testa_ios.m`, `tools/ios/app.sh`, `tools/ios/Info.forma.plist`; modify `lib/machina_posix.c` (+ `include/machina.h` doc), `lib/ludus_fenestra.c` + `include/ludus_fenestra.h` (guard), `aedilis.stml` (`<nexus>` rule).

**Interfaces:**
- Consumes: `testa.h` contract (`testa_principale`, `applicatio_initium`, `TestaAmbitus` incl. `superficies_*`, `testa_cursum_incipere/ludum_struere/finire/dimittere`); Task 1 (`testa_initus.h`: `testa_geometriam_in_area`, `testa_punctum_mappare`, `testa_tactum_vertere`, `testa_facultates_tactus`, `testa_quadrum_petere`, `testa_pixela_transferre`, `testa_actio_ex_phase`).
- Produces: `tools/ios/app.sh <apps/X/X.c> [-simulator|-tabula] [-currere] [-fumus] [-servare]` (exit 0 / 1 build / 2 apparatus); `build/ios/<sdk>/<X>/<X>.app`; smoke contract: app launched with argument `-fumus` prints `EXITUS=<n>` and writes `<Documents>/fumus.png`.

- [ ] **Step 1: Shared C for iOS (R1, R2).** `ludus_fenestra_currere` (decl + def) guarded `#if !defined(__ANDROID__) && !defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)`; `machina_posix.c`: `__APPLE__` branch only when not iOS; iOS joins the per-install file branch with directory `$HOME/Library/.rhubarb`. Verify: macOS root suites `probatio_machina`, `probatio_ludus_fenestra` green; `./tools/android_struere_porta.sh` viridis; iOS probe (`xcrun --sdk iphonesimulator clang -target arm64-apple-ios17.0-simulator -fsyntax-only` over pictor's `--varians ios,posix` closure) -> 0 errors.
- [ ] **Step 2: `lib/testa_ios.m`** (spec II.1), ObjC compiled with the same flags `build/aedilis/pictor/struere.sh` gives `fenestra_macos.m` (plus `-fobjc-arc` only if that line already has it). Pieces, each a small function or class: app delegate + scene delegate + view controller + `TestaVisus` (UIView); deferred initium when bounds AND safe area known (`viewDidLayoutSubviews` / `viewSafeAreaInsetsDidChange` -> one `_aream_aptare` like Android's); `CADisplayLink` paused unless `testa_quadrum_petere` asks (`STATIM` = unpause one frame, `POST` = `NSTimer`); `touches*` -> per touch: id table (UITouch* -> small s32), `testa_actio_ex_phase`, fill `TestaTactusCrudus` (positions in points, history from `coalescedTouchesForTouch:`), `testa_tactum_vertere`; lifecycle via scene `willResignActive`/`didBecomeActive` -> SUSPENSIO/RESUMPTIO, `sceneDidDisconnect` -> `testa_cursum_finire` + `testa_cursum_dimittere`; `densitas` = `UIScreen.scale` x 163 (points are ~163 per inch; pictor keeps scale II regardless); prints the `testa: fenestra WxH, area [..]` line at initium and the timing line at finire, as on Android; pixels: buffer = view size x `UIScreen.scale`, `testa_pixela_transferre` with geometry scale = II x screen scale, `CGImage` as `layer.contents`, `magnificationFilter = kCAFilterNearest`; Documents dir -> `sedes_datorum` + `chdir`, `TMPDIR = NSTemporaryDirectory()`; clipboard stub; `-fumus` argument -> smoke (R4 stroke, N = 120 frames, PNG, `EXITUS=`, `exit(0|1)`); `testa_principale` -> `UIApplicationMain`. Compile-time congruence of `TESTA_PHASIS_*` with `UITouchPhase*` (negative-array trick as in `testa_android.c`).
- [ ] **Step 3: `aedilis.stml`** `<regula obiectum="testa_ios">` with `-framework UIKit`, `QuartzCore`, `CoreGraphics`, `Foundation`.
- [ ] **Step 4: `tools/ios/app.sh` simulator path** (spec II.4 steps 1-3, 5): closure via `bin/aedilis --varians ios,posix --partes`; `.c` with `xcrun --sdk iphonesimulator clang -target arm64-apple-ios17.0-simulator` + `VEXILLA_C89` + `RADICES_C89` (via `bash -c '. tools/vexilla.sh; vexilla_radices ""'`); vendored sources with the Android runner's VFLAGS; `.m` with the ObjC flags; link with the frameworks; bundle with `Info.forma.plist` (`@TITULUS@` substituted; portrait; `UIApplicationSceneManifest` naming the shell's scene delegate class; `MinimumOSVersion` 17.0; `UILaunchScreen` = empty dict); `codesign -s -` (ad hoc); simulator chosen = booted one, else boot "iPhone 15 Pro Max" newest runtime; `simctl install`, `simctl launch [--console-pty] earth.golden.<X> [-fumus]`; smoke: preinstalled check via `simctl listapps` (never uninstall a preinstalled app; uninstall only what the smoke installed), wait for `EXITUS=` (60 s), copy `fumus.png` from `simctl get_app_container <booted> <bundle> data`/Documents, PNG magic check.
- [ ] **Step 5: Simulator by eye** (Fran's OK - Simulator window): `./tools/ios/app.sh apps/pictor/pictor.c -simulator -currere`, `xcrun simctl io booted screenshot` -> open it: portrait, safe area respected, four borders, icon row. Draw with the mouse in the Simulator (mouse = touch there).
- [ ] **Step 6: Simulator smoke:** `-simulator -fumus` -> `EXITUS=0`, PNG with the R4 stroke (open it). Run it again with the app already installed -> app still installed afterwards (Review Focus 1).
- [ ] **Step 7: Commit** (examen judges `.c`; the `.m` goes through the censor like `fenestra_macos.m`).

### Task 4: the iPhone, the gate, closure

**Files:** extend `tools/ios/app.sh` (device path), create `tools/ios/communia.sh` (device selection), `tools/ios_struere_porta.sh`; modify `pythonica/silva.py` (PORTAE), `project-specs/testa-ios-spec.md`.

**Interfaces:**
- Consumes: Task 3's `app.sh` and smoke contract.
- Produces: `app.sh ... -tabula` (device); gate `ios-struere` (signal `PORTA IOS-STRUERE: (viridis|FRACTA)`), merge tier, no `instrumentum=`.

- [ ] **Step 1: Device selection** `tools/ios/communia.sh`: `ios_tabulam_eligere` = exactly one device `available (paired)` in `xcrun devicectl list devices` (or `IOS_DEVICE` = identifier), else exit 2 naming the state; prints model and the device UDID.
- [ ] **Step 2: Device path** (spec II.4 step 4): `iphoneos` SDK, `-target arm64-apple-ios17.0`; profile = the one whose `Entitlements.application-identifier` is `Q5WC9FQ3J2.*` among `~/Library/MobileDevice/Provisioning Profiles/` (`IOS_PROFILUM` overrides), checked: not expired, contains the device UDID (`ProvisionedDevices`), else exit 2 naming it; entitlements plist `application-identifier = Q5WC9FQ3J2.earth.golden.<X>`, `com.apple.developer.team-identifier = Q5WC9FQ3J2`, `get-task-allow = true`; `codesign --force --sign "Apple Development: Francis Arant (44TZ4R8793)" --entitlements`; `devicectl device install app`, `devicectl device process launch [--console] -- [-fumus]`. If install/launch fails with an OS-support error (Xcode 16.1 vs iOS 26.4), exit 2 with that named and STOP - raise D8 with Fran (Review Focus 5).
- [ ] **Step 3: On the phone:** `-tabula -currere` (Wi-Fi); then the smoke `-tabula -fumus` (PNG via `devicectl device copy from --domain-type appDataContainer`); app kept if preinstalled.
- [ ] **Step 4: Fran by hand:** draw, tools via the squares, spraycan, layers via the new icon, home swipe mid-stroke and back (Review Focus 2), lock/unlock, quit and reopen (drawing kept).
- [ ] **Step 5: Gate** `tools/ios_struere_porta.sh`: builds the simulator `.app` for pictor (no device, no signing beyond ad hoc); Xcode absent -> `RECUSATA` exit 2; registered in PORTAE; plants by hand: a macOS-only call in a shared pictor lib -> FRACTA naming it; `xcode-select` path planted wrong -> RECUSATA; restore green.
- [ ] **Step 6: Close:** spec status IMPLETUM + section VI (rulings, measurements, AUDIENDA answered); memory (android-toolchain topic gains an iOS line, or a new `ios-toolchain` topic); ledger note under ...577GAS; commit.
