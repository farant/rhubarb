# Testa iOS - pictor on the iPhone, the pilot of house iOS apps (spec C1)

**Status: PROPOSITUM 2026-10-23** - designed in conversation with Fran
(2026-10-23), awaiting review of this text. Worktree quarta (main =
quarta = ede64ea5 when written). Third shell of the app contract
`testa` (B1: `project-specs/testa-spec.md`, IMPLETUM). Ledger: mobile
vision desideratum ...577GAS.

*Fran 2026-10-23: "just an iphone ... i do have an apple developer
account. i think simulator -> device -> test flight would be good. then
maybe we can use my phone as part of how we iterate on additional
pictor features like gallery"; "this is not the only ios app that we
will make, it's just our pilot ... during development if there's a way
to get builds directly to my device via usb or wifi that's great ...
but will also want test flight so my wife can have apps on her phone
later down the line. so maybe a little bit of a commit vs merge
cadence"; "ideally no xcode projects, no xcodegen, just our own modules
for whatever reusable capabilities we need to build out"; "getting what
we already have for fire tablet on the phone will be success. only
possible modification might be a portrait mode orientation"; on the
rendered comparison: "option c, and maybe the tweak could be just to
have a layer icon instead of showing the layer name and then you see
what layer you have picked by doing the pop-up".*

Goal: **pictor - the same pictor as on the Fire - draws under a finger
on Fran's iPhone, in portrait, at scale II, installed straight from the
Mac over USB or Wi-Fi, and first in the iOS Simulator.** On the way the
house gets its third `testa` shell (UIKit) and an iOS build script with
no Xcode project. This is sub-project C1. **C2** - a house App Store
Connect API client that makes certificates and profiles headlessly and
uploads TestFlight builds - is its own spec, after C1.

Decisions (Fran, 2026-10-23, in conversation):

- **D1 Stages:** simulator -> Fran's iPhone (C1) -> TestFlight (C2).
- **D2 Cadence:** simulator for every edit; direct-to-phone (USB or
  Wi-Fi) to feel it in the hand ("commit"); TestFlight when a feature is
  ready, for family ("merge") - C2.
- **D3 No Xcode project, no XcodeGen:** our own modules and scripts
  (house rule: own every layer). prunifex (`../prunifex`, XcodeGen +
  Xcode, TestFlight via a GitHub-connected App Store Connect workflow)
  is not the model.
- **D4 Pilot of a family:** everything iOS-specific is reusable per app
  (shell, script, signing); vicus mobile comes later.
- **D5 Success = the Fire's pictor on the phone,** portrait, scale II.
- **D6 One composition change, everywhere:** the "stratum N" text box
  in pictor's tool row becomes a 20 x 20 **layers icon square** like
  the tool squares; the current layer is seen in the existing layer
  pop-up. Applies on every surface (desktop, Fire, phone) - one
  composition (Fran: "yes" to the playback naming it everywhere).
  **And the tool name text is dropped from the status row, everywhere**
  (Fran, after the builder check: "yes, i think we can drop the tool
  name") - the tool square's icon names the tool; without the name and
  with the icon, the row fits 215 px.
- **D7 Device signing for C1 without portal work (my call, after the
  builder check):** the Xcode-made wildcard development profile already
  on this Mac ("iOS Team Provisioning Profile: *", App ID
  `Q5WC9FQ3J2.*`, expires 2027-01-27, contains the keychain's
  "Apple Development: Francis Arant" and Fran's iPhone) signs
  `earth.golden.pictor` and every later house app. Explicit per-app
  App IDs/profiles are C2's job (house API client). Cost if wrong: one
  manual profile on the developer website.
- **D8 Toolchain:** C1 is built on today's Xcode 16.1 / macOS 14.8;
  moving to macOS Sequoia 15.6+ and Xcode 26 (needed for TestFlight
  uploads: the iOS 26 SDK has been required since April 2026) is step
  0 of C2 (Fran: "loathe to update ... but i guess i'll have to do
  that eventually"). Sequoia suffices; Tahoe is not needed.
- **D9 UIScene from day one (my call):** the shell uses the scene
  lifecycle (a minimal scene delegate) - Apple has said apps built with
  SDKs after iOS 26 must, and this shell is the template for every
  later house iOS app. Cost if wrong: none (works on iOS 13+).


## I. What exists (2026-10-23)

| Fact | Where |
|---|---|
| The contract: an app includes `testa.h`, defines `applicatio_initium(a, forma)` and a one-line `principale` returning `testa_principale(argc, argv)`; shells are chosen by the aedilis variant rule (`lib/testa.c` base, `lib/testa_android.c` variant `android`) | `include/testa.h`, `include/testa_cursus.h` |
| `TestaAmbitus` carries screen/usable area, `superficies_*` (surface the shell imposes - new documents are sized to it), `densitas`, `sedes_datorum`, clipboard service | `include/testa_cursus.h` |
| Pure, host-tested shell pieces (built for Android, platform-free): geometry inside an area (`testa_geometriam_in_area`), touch -> `Eventus` state machine (`testa_tactum_vertere`: first pointer drives, cancel releases once, history -> `exempla`), frame decision (`testa_quadrum_petere`), nearest-neighbour blit (`testa_pixela_transferre`) | `include/testa_android_initus.h`, `lib/testa_android_initus.c`, `probationes/probatio_testa_android_initus.c` (9 sections) |
| The Android shell, the model for this one: deferred initium until window AND content area known, cadence mirroring the macOS loop, stdout -> system log, smoke via a property | `lib/testa_android.c`, `tools/android/apk.sh` |
| Objective-C is already house code: `lib/fenestra_macos.m`; aedilis links frameworks per object via `<nexus><regula obiectum="fenestra_macos"><vexillum! (>-framework Cocoa` | `aedilis.stml:63-90` |
| Fran's phone: iPhone 15 Pro Max (iPhone16,2), iOS 26.4, Developer Mode on, paired over Wi-Fi ("Fran's Phone", `xcrun devicectl list devices`); usable portrait area at scale II ~215 x 419 of our pixels | measured 2026-10-23 (builder check) |
| Mac: macOS 14.8.9; Xcode 16.1 (16B40), iPhoneOS SDK 18.1, simulator runtimes iOS 17.0 and 18.1 (iPhone SE 3, 15, 15 Plus/Pro/Pro Max), `xcrun devicectl` 397.24; keychain: one identity "Apple Development: Francis Arant (44TZ4R8793)"; no "Apple Distribution" identity; no App Store Connect API key (.p8) in the usual places | measured 2026-10-23 |
| Rendered: pictor at iPhone 15 portrait usable area (393 x 759 pt) - scale II = 196 x 379 our pixels: the tool row overflows (stratum box overlaps the size square, tool name hidden); scale I = 393 x 759 fits but text ~1.3 mm, squares ~20 pt | spike `build/spica/reddere_telephonum.c` (throwaway, not committed), companion page `pictor-iphone-scala.html` |
| The layer pop-up exists: a palette (PARTES_DIALOGUS) above the layers square - per layer an eye toggle (20 x 20) + name (100 x 20, current one `:electum`), then + and -; 120 px wide | `lib/pictor_componentia.c:379-` (`strata_palettam_componere`) |


## II. Design

### II.1 The iOS shell: `lib/testa_ios.m`

Objective-C (UIKit), variant `ios` of `testa.h` (aedilis rule:
`lib/testa_ios.m` found before `lib/testa.c` under `--varians
ios,posix`). Main thread only, like the other shells.

- **Entry:** `testa_principale(argc, argv)` calls `UIApplicationMain`
  with the shell's app delegate (it never returns). Apps keep their
  one-line `principale` unchanged.
- **Window:** scene lifecycle (D9): `UIApplicationSceneManifest` in the
  Info.plist names the shell's scene delegate, which creates one
  `UIWindow` + root view controller whose view is the shell's view;
  scene foreground/background map to the lifecycle below.
- **Initium:** deferred, as on Android, until the view has its bounds
  AND its safe-area insets (`viewSafeAreaInsetsDidChange` /
  `viewDidLayoutSubviews`): then the ambitus is filled -
  `schirmi_*` = view bounds in points, `utilis_*` and `superficies_*`
  = the safe area in points, `densitas` from `UIScreen.scale` x 163-ish
  dpi equivalent left to the builder (only pictor's scale choice reads
  it, and pictor keeps scale II), `sedes_datorum` = the app's Documents
  directory (also `chdir` there, `HOME` unchanged, `TMPDIR` =
  `NSTemporaryDirectory()`), clipboard service = a stub returning
  nothing (as on Android; paste is not in C1).
- **Pixels:** our tabula at scale II in points (`floor(safe area /
  II)`); each frame is blitted with `testa_pixela_transferre` into a
  buffer of the view's size in **device pixels** (points x
  `UIScreen.scale`, i.e. integer scale II x 3 = 6 on a 3x phone) and
  shown as the layer's `contents` (CGImage over the buffer, or an
  equivalent zero-copy path - builder's choice) with
  `magnificationFilter` nearest. Margins = theme background, as on
  Android.
- **Frames:** a `CADisplayLink`, paused when `testa_quadrum_petere`
  says NULLUM, unpaused for STATIM, and a one-shot timer for POST
  (the dispensator's idle wait) - same cadence rule (R4 of B1).
- **Input:** `touchesBegan/Moved/Ended/Cancelled` (multipleTouch on)
  mapped onto the existing touch state machine: the shell fills the
  same plain "raw touch" struct (action, pointer ids, positions in
  points, tool type, history from `coalescedTouchesForTouch:`) and
  calls `testa_tactum_vertere`. Behaviour identical to the Fire: first
  finger draws, second ignored, cancel releases once, pressure
  IGNOTA (iPhone fingers report no force; Pencil is not supported on
  the phone).
- **Lifecycle:** `applicationWillResignActive` -> SUSPENSIO, frames
  stop; `applicationDidBecomeActive` -> RESUMPTIO; background entry
  needs nothing in C1 (state lives in the volumen; state restore =
  B2). `applicationWillTerminate` -> `testa_cursum_finire` +
  `testa_cursum_dimittere` (best effort; iOS rarely calls it).
- **Output:** stdout/stderr stay as they are; `simctl launch
  --console-pty` and `devicectl ... launch --console` show them. The
  timing line ("ludus: quadra=...") and the testa line ("testa:
  fenestra WxH, area [..]") are printed as on Android.
- **Machine identity:** `gethostuuid` is unavailable on iOS - the
  `__APPLE__` branch of `lib/machina_posix.c` gets an iOS path using the
  per-install file (`machina_identitas_ex_plagula`, as on Android).
- **The Mac-only loop** `ludus_fenestra_currere` is kept out of the iOS
  build (its guard becomes `#if` not-Android and not-iOS).
- **Smoke mode:** launched with argument `-fumus` (both `simctl
  launch` and `devicectl ... launch` pass arguments, so no property
  trick is needed): N frames, last frame to `<Documents>/fumus.png`,
  `EXITUS=<n>` printed, then the app exits (`exit()` is acceptable in
  smoke mode only).
- **Portrait only** (Info.plist), status bar visible, the home
  indicator area left to the system (safe area keeps us clear of it).

### II.2 The pure pieces become platform-neutral

`include/testa_android_initus.h` / `lib/testa_android_initus.c` /
`probationes/probatio_testa_android_initus.c` are renamed to
`testa_initus` (`./silva/renominare.sh` for identifiers if any change;
file renames by git), since two shells use them. Their constants
`TESTA_TACTUS_*` mirror Android's numbering; the iOS shell maps UITouch
phases onto them (DOWN for the first touch, POINTER_DOWN for later
ones, MOVE, POINTER_UP / UP, CANCEL) - the mapping is a pure function
too, host-tested. Android keeps its compile-time congruence check.

### II.3 The composition change: a layers icon (D6)

In `lib/pictor_componentia.c` the stratum box ("stratum N", text) is
replaced by a 20 x 20 square like the tool squares, same action (opens
the existing layer palette), drawn by `pictor_figurae`'s square figure
with a new icon title (e.g. "strata": a few stacked rectangles, theme
colours). The palette is unchanged - it already marks the current
layer. On every surface. Consequences the builder must handle: desktop
pictor's `-fumus -imago` PNG changes on purpose (new parity baseline
recorded in the plan); pictor suites that assert the stratum text
(if any) are updated; `apps/pictor` docs/interrogationes mentioning
"stratum" text updated.

### II.4 Build and run: `tools/ios/app.sh`

`tools/ios/app.sh <apps/X/X.c> [-simulator | -tabula] [-currere]
[-fumus] [-servare]` - sibling of `tools/android/apk.sh`:

1. `bin/aedilis <fons> --varians ios,posix --partes` -> closure (the
   shell arrives through `testa.h`); compile with `xcrun --sdk
   iphonesimulator|iphoneos clang`, `-target arm64-apple-ios17.0[-simulator]`
   (deployment target iOS 17.0 - builder may lower it if Fran's phone
   needs it), house flags (`VEXILLA_C89` for `.c`; ObjC flags as
   `fenestra_macos.m` gets them), frameworks via an aedilis `<nexus>`
   rule for `testa_ios` (UIKit, QuartzCore, CoreGraphics, Foundation).
2. Bundle `build/ios/<sdk>/<X>/<X>.app`: the binary + a generated
   `Info.plist` (`CFBundleIdentifier` `earth.golden.<X>`,
   `CFBundleExecutable`, versions, `UIRequiredDeviceCapabilities`
   arm64, `UISupportedInterfaceOrientations` portrait,
   `UILaunchScreen` empty dict, `MinimumOSVersion`), from a template
   under `tools/ios/`.
3. **Simulator:** unsigned (ad-hoc `codesign -s -` if the simulator
   requires a signature); `xcrun simctl boot` a chosen device if none
   is booted (default iPhone 15 on the newest runtime), `simctl install`,
   `simctl launch [--console-pty] earth.golden.<X> [-fumus]`.
4. **Device (`-tabula`):** `embedded.mobileprovision` = the wildcard
   development profile (D7), found by its App ID `<team>.*` among
   `~/Library/MobileDevice/Provisioning Profiles/` (`IOS_PROFILUM`
   overrides; expired or not containing the device -> exit 2 naming
   it), entitlements derived from it (`application-identifier` =
   `<team>.earth.golden.<X>`, `get-task-allow`), `codesign --force --sign "Apple Development: ..."
   --entitlements`, `xcrun devicectl device install app --device <id>`,
   `devicectl device process launch [--console]`. Device chosen like
   `communia.sh` does for Android: exactly one connected/paired device
   or `IOS_DEVICE` given; USB or Wi-Fi (devicectl handles both once
   paired).
5. **Smoke (`-fumus`):** launch with `-fumus`, wait for `EXITUS=`, pull
   `fumus.png` (simulator: from the app's data container via `simctl
   get_app_container`; device: `devicectl device copy from`), check PNG
   magic. Same safety rule as Android: never uninstall an app that was
   installed before the smoke (the phone will hold drawings).

### II.5 Device prerequisites (D7)

Already in place (2026-10-23): Developer Mode on Fran's iPhone, Wi-Fi
pairing, the wildcard development profile containing the phone and the
Development certificate. `app.sh -tabula` checks each (device paired
and available, profile valid and containing the device UDID, identity
in the keychain) and names the missing one (exit 2), never guesses. If
the profile lapses before C2 exists, Xcode or the developer website
renews it.

## III. Tests

1. **Pure pieces** (renamed suite): all existing sections stay green on
   host and on the Fire; new section for the UITouch-phase -> raw
   action mapping (first touch DOWN, later touches POINTER_DOWN, the
   driving finger lifting while another stays, cancel).
2. **Desktop parity:** scriba unchanged (byte-exact); vicus by eye
   (clock - and its embedded pictor pane shows the new status row);
   pictor changes on purpose (D6: layers icon, no tool name) - new
   baseline captured, inspected by Fran once.
3. **Simulator smoke** (`app.sh apps/pictor/pictor.c -simulator
   -fumus`): `EXITUS=0` + PNG showing a synthetic stroke (`simctl` has
   no touch injection - the builder chooses: an XCTest-free option such
   as a smoke-mode synthetic stroke fed through the same touch path by
   the shell itself, or `simctl io` alternatives; the stroke must go
   through `testa_tactum_vertere`). Screenshot opened and checked.
4. **Gate:** extend `android-struere`'s idea with `ios-struere`
   (merge tier): builds the simulator `.app` for pictor with no device
   and no signing; Xcode absent -> refuses (exit 2), never a silent
   skip. Registered in `pythonica/silva.py` PORTAE, no `instrumentum=`
   it does not own.
5. **Fran, by hand, on the iPhone:** draw, tools, spraycan, layers via
   the new icon, lock mid-stroke, quit and reopen (drawing kept).


## IV. Not in this spec

- **C2:** house App Store Connect API client (ES256 JWT over our TLS):
  certificates, devices, profiles, app records, build upload,
  TestFlight testers (Fran's wife as internal tester). Its own spec.
- The **pinacotheca** (gallery: open / new picture) - next, iterated on
  the phone.
- iPad layouts, multi-touch gestures, Apple Pencil, paste/clipboard,
  state restore after the system kills the app (B2), app icon and
  launch screen artwork, landscape on the phone.


## V. Order of work (the plan will follow approval)

1. Rename the pure pieces (II.2) + the UITouch mapping test.
2. The layers icon (II.3) + pictor parity baseline.
3. `lib/testa_ios.m` + `tools/ios/app.sh -simulator` -> pictor in the
   simulator; simulator smoke.
4. Device: `app.sh -tabula` (wildcard profile, D7) -> pictor on
   Fran's iPhone over Wi-Fi; Fran by hand; gate `ios-struere`.


## AUDIENDA (not verified when written)

- Whether UIKit on the iOS 18.1 SDK still runs a scene-less single
  window app without warnings that matter.
- Simulator code-signing requirement (ad-hoc signature or none) for
  an app installed with `simctl`.
- `devicectl` argument passing and console streaming on Xcode 16.1
  (`--console`), and pulling a file from the app's container.
- Xcode 16.1 deploying to iOS 26.4: devicectl reports the device's
  DDI services available, but install/launch from an older Xcode to a
  newer iOS can still fail - if it does, D8's upgrade moves into C1
  (Fran asked).
- How a stroke can be injected in the simulator without XCTest/XCUITest.
- `exit()` from smoke mode is accepted by iOS for a development build.
