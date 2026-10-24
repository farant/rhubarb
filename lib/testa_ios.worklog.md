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
