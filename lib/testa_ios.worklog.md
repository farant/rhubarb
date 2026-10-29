# testa_ios worklog

## 2026-10-23 - the shell, first light (testa-ios T3)

- **UIKit under `-std=c89`:** UIKit headers use `static inline`
  (UIKIT_STATIC_INLINE), an unknown word in C89 -> errors inside system
  headers. `include/postulata_ios.h` (`#define inline __inline__`) is
  included FIRST in every iOS .m, like postulata_android.h before NDK
  headers. Unlike Android, no `static` is added: UIKit already writes it.
- **aedilis and .m files:** the `-MM` oracle needs the iOS SDK for
  `_ios.m` files - `<oracula>` in aedilis.stml (see tools/aedilis.worklog.md).
- **Units:** pictor's surface = the safe area in POINTS (430 x 839 on the
  15 Pro Max -> 215 x 419 our pixels at scale II). Geometry, the pixel
  buffer and touches are in DEVICE pixels: scale = forma.scala x
  displayScale (2 x 3 = 6). `tabula->scala` / `fenestra_*` follow Android
  (window pixels), so MUTARE_MAGNITUDINEM stays consistent.
- **Touch time:** UITouch.timestamp is systemUptime (mach base); our clock
  is CLOCK_MONOTONIC (fenestra_tempus_posix). Converted as "now in our
  clock minus age in the touch clock" (CACurrentMediaTime shares the touch
  base) - no assumption that the two bases agree.
- **Frames:** CADisplayLink paused except for STATIM (one frame, then
  paused again); POST = one-shot NSTimer. Each shown frame copies the
  buffer into CFData before CGImageCreate: Core Animation may still read
  the previous image while the next frame is blitted. ~14 MB per frame on
  a 3x phone; only drawn frames pay it (pictor idles at 0 frames).
- **Smoke:** `-fumus` in argv (simctl/devicectl pass it). The stroke is
  fed through testa_actio_ex_phase + testa_tactum_vertere from frame 5 (R4);
  120 frames, PNG to Documents, EXITUS=, exit(). Simulator: 430x932 pt,
  scale 3, safe area [0,59]-[430,898]; EXITUS=0 first run.
- Machine id lands in `$HOME/Library/.rhubarb/machina` (0600), the
  drawing in `Documents/pictor.volumen` (seen in the simulator container).

## 2026-10-24 - on the iPhone (testa-ios T4)

- Xcode 16.1 / devicectl 397.24 installs, launches and copies files on
  iOS 26.4 over Wi-Fi (the phone's own hotspot): "Enabling developer disk
  image services" happens by itself. The D8 upgrade is only for C2.
- Smoke PNG from the phone is byte-identical to the simulator's: the
  composition is deterministic across simulator and device.
- `plutil -convert json` fails on a whole provisioning profile (data
  fields), so `ProvisionedDevices` is walked with `plutil -extract
  ProvisionedDevices.N raw` (tools/ios/communia.sh).
- Signing identity = the profile certificate whose SHA-1 is in the
  keychain (`security find-identity`), passed to codesign BY HASH.
- Installing over an existing app keeps its data container (drawings
  survived the pop-up fix reinstall).

## 2026-10-24 - photo picker (C1b T3)

- `PHPickerViewController` runs out of process: no photo-library
  permission, no Info.plist usage string. `[PHPickerFilter imagesFilter]`
  needs no UniformTypeIdentifiers link.
- The provider calls back on Apple's queue; the shell retains the
  `UIImage` and `dispatch_async`s to the main queue, so all our code
  stays on the main thread. Blocks compile under the house ObjC flags
  (manual retain/release, no ARC).
- `UIGraphicsImageRenderer` (format scale 1.0) + `[UIImage drawInRect:]`
  = the EXIF orientation applied, so a portrait photo arrives upright;
  alpha is kept (default format is not opaque).
- The picked photo is written to `$TMPDIR/imago_electa.png` and delivered
  as `EVENTUS_DEPOSITIO` - pictor's drop handler (T1) does the rest.
- One flag `_eligens` covers "picker open" and "photo still loading";
  an empty result (Cancel or swipe-down) clears it.

## 2026-10-24 - Xcode 27.1 (macOS 27) after merging main 822639ae

- iOS 27.1 SDK + clang 21: `lib/testa_ios.m` compiles clean (postulata_ios.h
  already does what main's postulata_macos.h does for macOS .m files).
  Old simulator runtimes (17.0, 18.1) remain; the default "iPhone 15 Pro
  Max" simulator still exists.
- `devicectl list devices` now lists SIMULATORS too (`reality =
  simulated`), and for the phone omits `developerModeStatus`,
  `marketingName` and `reality` until a tunnel is open; the identifier
  changed (16FF1CC7-... instead of C935DC82-...). `communia.sh` skips
  simulated entries, accepts an absent Developer Mode field (install
  names the cause if it is off), falls back to `productType`.
- TestFlight's "iOS 26 SDK" requirement is now met by the installed Xcode;
  C2 step 0 (the upgrade) is done.

## 2026-10-28 - URLSession veredarius, reactor timers (communio-plan-5 T8, quarta)

The iOS shell hands the app `a->veredarius` backed by URLSession
(ephemeral session, no cache, delegate queue = main queue so completions
run on the main thread like everything else here), `a->reactor` (timers
only - no sockets on iOS; networking is URLSession) and `a->machina =
"iPhone"`. The pictor binary links no BearSSL, no house HTTP/TLS, no
server (checked: 0 `_br_` symbols, no such objects).

Request timeout = the request's tempus + 15 s (URLSession counts idle
time itself). Responses are copied into the caller's pool; the finish
callback runs, then the shell asks for a frame at once (`debitum`) so the
reactor handles the deferred response without waiting for the 300 ms
idle tick. Abort cancels the task and the cancellation completion is
ignored (generation + `abrupta`), matching the veredarius contract.

Each frame polls the app's reactor (timeout 0) before drawing; the shell
hands the reactor to the window state with `ludus_fenestra_reactorem_ponere`
so `ludus_fenestra_mora` (read for frame scheduling) is bounded by the
reactor's next timer. `testa_quadrum_petere` now returns NO frame for
mora -1 (nothing due) instead of a POST with a negative delay.

`-probare-rete <url>`: one GET through the veredarius, prints `testa:
rete successus=.. status=..` - for Simulator checks only.

EVENTUS_RESUMPTIO (scene active again) reaches pictor's root handler,
which calls `communio_excitare` (wired by pictor_communionem_ligare):
lanes resume at once and the device list is refetched.

## 2026-10-28 — communio plan-5 T9: system keyboard

TestaVisus itself adopts UIKeyInput + UITextInputTraits (no hidden
UITextField): pictor draws its own field, the shell only shows the
keyboard and turns keystrokes into events. canBecomeFirstResponder is
TRUE only while the app asked for a keyboard (`_testa.claviatura` !=
NULLA), so touches never pop it up by accident.

- insertText -> NSData (UTF-8) -> mutableCopy -> `testa_textum_eventus`
  (pure, in testa_initus; tested). The mutable copy matters: the TEXTUS
  chorda is a VIEW and chorda.datum is `i8*`, so a `const char*` from
  UTF8String would need a cast that -Wcast-qual rejects. Events are
  dispatched synchronously (ludus_fenestra_tractare), so the autoreleased
  buffer outlives them.
- '\n' (Return/Send/Go) -> CLAVIS_DEPRESSUS REDITUS, deleteBackward ->
  RETRORSUM (`testa_clavem_eventus`). hasText is always YES so Backspace
  arrives even when pictor's field is empty (pictor decides).
- Traits are getters reading `_testa.claviatura`; switching EPISTULA ->
  CODEX while the keyboard is up calls reloadInputViews so the new
  keyboard type takes effect without hiding it.
- No pressesBegan handler exists, so a hardware keyboard (Simulator,
  iPad) also arrives through insertText — no double delivery.
