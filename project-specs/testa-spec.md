# Testa - the app contract and two shells: pictor on the Fire (spec B1)

**Status: PROPOSITUM 2026-10-23** - designed in conversation with
Fran section by section (2026-10-23), awaiting review of this text.
Worktree quarta. Sub-project B1 of the mobile line; A (the Android
build tier) is `project-specs/android-gradus-spec.md`, IMPLETUM.
Ledger: vision desideratum ...577GAS (`01M5KAM5ATMEGGA7ZTFE577GAS`),
spike note ...RMQ67J, composition/sync note ...TEP7VXW
(`01M5MRFVRNST7KJH3HDTEP7VXW`).

*Fran 2026-10-23: "b1 is a good first slice. pictor drawing in android
would be great"; on the shape, the Handmade Hero question - "rather
than rhubarb stuff accessing platform through platform layer the
platform layer being able to use the rhubarb libraries inside their
own shell"; "lets define the contract and have both shells use it";
all three desktop apps move ("(a) is probably the right way to go");
"the mobile apps will be their own things and it will be more like
using shared material to make good experiences that have continuity
/ same aesthetics etc on mobile and desktop ... vs. making an app and
being able to drop it as is"; for B1 "an as is translation ... we can
just letterbox if need to (although adapting size isn't a problem)";
"earth.golden. is a good domain to use for this kind of thing".*

Goal: **pictor - the same pictor as in vicus, spraycan and layers and
all - draws under a finger on the Fire 7, launched from its home-screen
icon, built and installed by one command.** On the way, the house
gets an app contract: the platform calls the app (Handmade Hero
shape), never the app the platform. macOS pictor, scriba and vicus
move onto it with byte-identical output; Android is the second shell.

Decisions (Fran, 2026-10-23, in conversation):

- **D1 Inversion.** One contract (`include/testa.h`) between a
  per-OS *shell* and a *host app*; every windowed app uses it, on
  every OS. No second thread, ever (spike: main thread suffices).
- **D2 All three desktop apps move in B1** (pictor, scriba, vicus) -
  vicus is the hardest consumer (pulse, fullscreen, ports), so the
  contract meets it now rather than being designed around pictor.
- **D3 As-is translation.** The *desktop composition* of pictor runs
  on the Fire; a tablet composition is a later sub-project of its
  own. Letterbox is acceptable; adapting size is fine too.
- **D4 Shared material vs compositions** (II.9): continuity across
  surfaces comes from shared documents, actions, figures, theme - not
  from one layout everywhere. A document belongs to the aspect ratio
  it was made in.
- **D5 Package names `earth.golden.<app>`** (`earth.golden.pictor`).
- **D6 Back = quit, landscape locked, clipboard stubbed** on Android
  in B1; scale = the app's own (II) with a measured fallback (II.5).


## I. What exists (quarta 1b523d1b, 2026-10-23)

| Fact | Where |
|---|---|
| Every windowed app's `principale` interleaves shell work (window, tabula, fullscreen, loop, `-imago`, teardown) with app work | `apps/pictor/pictor.c:39-125`, `apps/scriba/scriba.c:28-108`, `apps/vicus/vicus.c:90-257` |
| The loop is one function over separable pieces: `ludus_fenestra_tractare` (one event), `ludus_fenestra_pingendum` (decide), `ludus_quadrum` (one frame), `ludus_fenestra_mora` (wait), pulse at <= `MORA_PULSUS` XVI ms | `lib/ludus_fenestra.c:214` (`ludus_fenestra_currere`), `:12`, `include/ludus_fenestra.h` |
| Platform services already reach apps as function pointers: clipboard image (`actiones_ctx.imago_capere`), battery (`vicus_pilam_ponere`), sound, `$question` ports | `apps/pictor/pictor.c:28-36,89-90`, `apps/vicus/vicus.c:60-87,179-208` |
| Sub-app libraries are already platform-free: the only platform include in `lib/pictor_*`, `lib/scriba_*`, `lib/vicus_*`, `lib/insula*`, `lib/dispensator*`, `lib/ludus_*` is `lib/vicus_sonus_porta.c` (a port, the allowed pattern) | grep of `#include "(fenestra|clipboard_platform|energia|fenestra_tempus|sonus*).h"`, 2026-10-23 |
| pictor's Android closure (`bin/aedilis apps/pictor/pictor.c --varians android,posix --partes`) is 67 objects, all building except two platform pieces: `fenestra.h` resolves only to the portable `lib/fenestra_textus.c`, `clipboard_platform.h` has only `lib/clipboard_platform_macos.m` | measured 2026-10-23 |
| pictor's core is split by role: `pictor_documentum` (the stroke log in a volumen - truth; bitmap derived), `pictor_actiones` (handlers, no I/O), `pictor_figurae` (pure), `pictor_componentia` (the composition - per surface); `pictor_terminalis.c` is already a second surface over one core | `include/pictor_*.h` headers; `apps/pictor/pictor_terminalis.c` |
| The event vocabulary already covers touch: pointer kind `EVENTUS_INDICATOR_MUS/STILUS/TACTUS`, pointer id, pressure `0..M` or `EVENTUS_PRESSIO_IGNOTA`, coalesced `exempla`, `EVENTUS_FACULTATES` (hover, real pressure), `EVENTUS_SUSPENSIO/RESUMPTIO` | `include/eventus.h:58-59,231-237,284,320-332` |
| Android tier: `bin/aedilis --varians android,posix`, NDK r27d pin, `tools/android/probatio.sh` (device preflight, host lock, purge), machine identity under `$HOME/.rhubarb/machina` | `project-specs/android-gradus-spec.md` |
| Spike (throwaway, 2026-10-22): NativeActivity callbacks + `AInputQueue_attachLooper(ALooper_forThread())` + `AChoreographer_postFrameCallback64` + `ANativeWindow_lock` at ~59 fps 600x1024 RGBA on the main thread; aapt2 + zip + zipalign + apksigner -> 25 KB APK; NDK headers need `#define inline static __inline__` | ledger ...RMQ67J; memory android-toolchain |
| `vicus.c:117` `b32 plena` is assigned only on the fullscreen path (`:171`) and read at `:231` - uninitialized under `-fumus` or when `fenestra_spatium_schirmi` fails | read 2026-10-23 |
| `include/concha.h` is an older widget-era "testa" with its own loop; only `probationes/probatio_combinado.c` uses it | grep 2026-10-23 |


## II. Design

### II.1 The contract: `include/testa.h`

```c
/* clipboard: idem genus ac actiones_ctx.imago_capere pictoris */
nomen ImagoFructus (*TestaImagoCapere)(vacuum* ctx, Piscina* piscina);

/* quod testa applicationi praebet */
nomen structura {
               Piscina* piscina;      /* testa possidet et destruit */
                   s32  argc;         /* argv integer (optiones testae
              character** argv;        * quoque - app suas legit) */
                   b32  fumus;        /* -fumus (Android: proprietas) */
                   i32  schirmi_latitudo;   /* puncta (macOS) / pixela */
                   i32  schirmi_altitudo;   /* (Android): schirmus totus */
                   i32  utilis_latitudo;    /* area utilis (sine menu, */
                   i32  utilis_altitudo;    /* navali); = schirmus si nulla */
    constans character* sedes_datorum;      /* macOS: cwd; Android:
                                             * internalDataPath (et cwd) */
    /* servitia quae objecta VIVA testae egent */
     TestaImagoCapere  imago_glutinata;     /* clipboard; Android B1:
               vacuum* imago_ctx;            * semper vacuum */
} TestaAmbitus;

/* quod applicatio testae reddit */
nomen structura {
    constans character* titulus;
                   i32  latitudo;     /* pixela NOSTRA: dispositio */
                   i32  altitudo;
                   i32  scala;        /* puncta per pixelum nostrum */
                   b32  plena_visio;
          Dispensator* d;            /* -> ludus_fenestra_creare */
      FiguraRegistrum* figurae;
                   i32  thema;
             ImagoFons  fons;
                vacuum* fons_ctx;
         LudusPulsator  pulsator;     /* NIHIL = nullus */
                vacuum* pulsator_ctx;
                vacuum (*finis)(vacuum* ctx);   /* NIHIL licet */
                vacuum* finis_ctx;
} TestaForma;

/* omnis applicatio fenestrae hoc UNUM definit; FALSUM = exitus I */
b32 applicatio_initium (constans TestaAmbitus* a, TestaForma* forma);
```

Rules of the contract:

- The **shell owns** `principale` (or the Android entry point), the
  piscina, the window, the tabula, the `LudusFenestra`, the loop,
  `-imago`/screenshot, and teardown order. The **app owns** its
  state (built in `a->piscina`), its volumen, its ports.
- `applicatio_initium` runs **once**, with the real screen known.
  `finis` runs once after the last frame (close the volumen etc.);
  the shell destroys the piscina after it.
- The app never calls `fenestra_*` or `ludus_fenestra_currere`.
  `ludus_fenestra_currere` leaves the public header and becomes the
  macOS shell's loop.
- `argv` is passed whole: apps keep parsing their own options
  (`-volumen`, `-quaestio`, `-fumus` inside `*_volumen_aperire`)
  unchanged. On Android the shell synthesizes `argv` = `{titulus}` or
  `{titulus, "-fumus"}`, so the app takes the same code path.
- The shell picks the shell's options from `argv` (`-imago <via>`,
  `-fumus` -> XXX frames then exit), as each `principale` does today.

### II.2 Services: through the shell, or a variant library?

- **Through `TestaAmbitus`**: anything needing the shell's *live
  objects* - clipboard (on Android a JNI call on the Activity), screen
  geometry, the data directory; later IME, immersive mode. In B1 the
  only service field is the clipboard image.
- **A variant library, called directly**: plain OS calls that need no
  session - reactor, volumen, sqlite, sound, battery (`energia`). They
  get Android implementations by the aedilis variant rule when an app
  that needs them is ported (not in B1: vicus and scriba stay
  desktop-only).

### II.3 The macOS shell: `lib/testa_fenestra.c`

Plain C over `fenestra.h` (so a future Linux `fenestra` gets it free).
`principale`:

1. piscina (the piscina exists before `applicatio_initium`, so its
   size cannot come from the forma: the shell takes the largest of
   today's, VIII MiB dynamic - vicus; pictor and scriba ask IV); read `-imago`, `-fumus` from argv;
   `fenestra_spatium_schirmi` / `fenestra_spatium_utile` -> ambitus;
   `imago_glutinata` = `clipboard_capere_imaginem`.
2. `applicatio_initium(&a, &forma)`; FALSUM -> exit I.
3. Window: `forma.plena_visio` (vicus S2c) -> window at the usable
   rectangle (`fenestra_spatium_utile` origin), height in our pixels
   kept integral (`utilis / scala * scala`, the 1147/573 rule of
   `vicus.c:143-149`), tabula at `utilis_altitudo / scala`, THEN
   `fenestra_commutare_plenam_visionem`; otherwise window at (C, C),
   `forma.latitudo * scala` x `forma.altitudo * scala`,
   `FENESTRA_ORDINARIA`, tabula at `forma.altitudo`.
4. `ludus_fenestra_creare(forma.d, forma.figurae, forma.thema,
   forma.fons, forma.fons_ctx, tabula)`; pulse if set.
5. The loop (`ludus_fenestra_currere` moved verbatim, private).
6. `-imago` -> PNG (`ludus_fenestra_imaginem_scribere`);
   `forma.finis`; `fenestra_destruere`; `piscina_destruere`.

Per app, `principale` becomes `applicatio_initium` (vicus ~170 lines
-> ~100); every `lib/<app>_*` file is untouched. vicus's `plena` bug
(I) disappears: `forma->plena_visio = !a->fumus`.

### II.4 The Android shell: `lib/testa_android.c` (main thread only)

`include/postulata_android.h` (guarded `#define inline static
__inline__`) is the first include of every Android-only `.c`.

| Callback | Shell does |
|---|---|
| `ANativeActivity_onCreate` | piscina; `chdir` + `HOME` + `TMPDIR` (`<data>/tmp`, created) = `internalDataPath`; fds 1 and 2 -> a pipe read on the main looper (`ALooper_addFd`) -> `__android_log_write`, tag = the lib name, so every `imprimere` / `fprintf(stderr)` reaches logcat; read `debug.rhubarb.fumus` (`__system_property_get`, N frames, absent/0 = off); attach the input queue when it arrives |
| first `onNativeWindowCreated` | window size -> ambitus (schirmus = utilis = window); **`applicatio_initium` runs here** (apps need the real size); tabula + `LudusFenestra`; first frame requested |
| later `onNativeWindowCreated` | reuse state; `MUTARE_MAGNITUDINEM` only if the size changed |
| `onNativeWindowDestroyed` | drop the window; no drawing until a new one |
| `onPause` / `onResume` | `EVENTUS_SUSPENSIO` / `EVENTUS_RESUMPTIO` to the dispensator; frames stop / restart |
| `onInputQueueCreated/Destroyed` | `AInputQueue_attachLooper` / detach on the main looper |
| BACK key | `ANativeActivity_finish` |
| `onDestroy` | `forma.finis`; piscina destroyed |
| `onSaveInstanceState`, `onConfigurationChanged`, `onLowMemory` | nothing in B1 (manifest `configChanges` keeps the activity alive across orientation/keyboard/screenSize, II.7) |

**Frames.** The Choreographer callback: drain the input queue ->
`ludus_fenestra_tractare` -> `ludus_fenestra_pingendum` ->
`ludus_quadrum` -> present (II.5). The next frame is requested **only
when needed**, by a pure decision function (II.6): input arrived, a
frame is still owed, a dispensator timer is due
(`AChoreographer_postFrameCallbackDelayed64` with
`ludus_fenestra_mora`), or a pulse is set (every vsync). Idle pictor
= no callbacks = ~0% CPU, like `fenestra_expectare_eventus` on macOS.

**fumus on the device.** With `debug.rhubarb.fumus=N`: N frames are
drawn regardless of need, the last is written to
`<data>/fumus.png`, logcat gets `EXITUS=<n>`, then
`ANativeActivity_finish`.

### II.5 Size, scale, presenting

The window is 1024x600 (landscape, II.7). The shell honours
`forma.scala`: our size = `floor(window / scala)` each way, sent as
`MUTARE_MAGNITUDINEM` (`ludus_fenestra` already resizes the tabula on
it, scale kept, 013 B3b). The remainder is a letterbox margin,
centred, filled with the theme background. The shell blits
**itself**, nearest-neighbour integer scale, into the window buffer
(`ANativeWindow_setBuffersGeometry` = window size, `RGBA_8888`):
compositor scaling may filter bilinearly, which would blur pixel art
and make the device screenshot differ from what we drew.

pictor's scale is II -> 512x300 of our pixels, smaller than its
640x480 design. **Measured in the plan, not assumed:** if pictor's
composition lays out cleanly at 512x300 (its canvas, status line,
palette), II stays; if not, the shell falls back to scale I for that
app with a letterbox (1024x600 holds 640x480) and the spec records
which. Touch coordinates are mapped by the same geometry: minus the
margin, divided by scale; a touch in the margin maps outside
`[0, latitudo)` like a pointer outside a window (A3c).

### II.6 Pure pieces (host-testable)

Two functions carry the shell's logic with plain values in and out,
in `lib/testa_android_initus.c` (compiles on any OS; Android constants
mirrored as house constants with their NDK values cited):

- **Touch -> `Eventus`**: (action, pointer index/id, tool type, x, y,
  time, history samples, geometry) -> zero or one `Eventus`.
  `DOWN` -> `EVENTUS_MUS_DEPRESSUS` (left button); `MOVE` ->
  `EVENTUS_MUS_MOTUS` with the button held and `exempla` from the
  history; `UP`/`CANCEL` -> `EVENTUS_MUS_LIBERATUS`; FINGER ->
  `EVENTUS_INDICATOR_TACTUS`, STYLUS -> `EVENTUS_INDICATOR_STILUS`;
  pressure `EVENTUS_PRESSIO_IGNOTA` (the Fire's finger pressure is a
  synthetic 0.10 - spike); **only the first pointer drives** (a
  second `POINTER_DOWN` and its moves are ignored until all pointers
  are up). The first frame queues `EVENTUS_FACULTATES` with hover
  FALSUM, pressure FALSUM.
- **Frame decision**: (window present, resumed, events pending, frame
  owed, `mora`, pulse set, fumus frames left) -> none / now / after
  N ms.

### II.7 Build and packaging: `tools/android/apk.sh`

`tools/android/apk.sh <apps/X/X.c> [-instituere] [-currere] [-fumus]
[-servare]`:

1. `bin/aedilis <fons> --varians android,posix --partes` -> closure;
   the shell arrives through `testa.h`'s variants (`testa_android.c`
   under android, `testa_fenestra.c` otherwise) - no special case in
   the script.
2. NDK clang (pin `tools/android/ndk_fixum.sh`), `--target=
   aarch64-linux-android30`, `VEXILLA_C89` + `-fPIC` (vendored
   sources: the runner's VFLAGS) -> objects in
   `build/android/arm64-v8a/apk/<X>/obiecta/`; link `-shared
   -landroid -llog -Wl,--no-undefined` -> `lib<X>.so` (a missing
   platform piece FAILS here, naming the symbol); `llvm-strip`.
3. Generated `AndroidManifest.xml`: package `earth.golden.<X>`, label
   = the app's name, `android:hasCode="false"`, NativeActivity with
   `android.app.lib_name` = `<X>`, `minSdkVersion 30`,
   `targetSdkVersion 34`, `screenOrientation="sensorLandscape"`,
   `configChanges="orientation|screenSize|screenLayout|keyboard|keyboardHidden"`,
   `debuggable="true"` (for `run-as`), launcher intent filter. No icon
   in B1 (the launcher's default).
4. `aapt2 link` against the SDK's `platforms/android-34/android.jar`
   -> zip `lib/arm64-v8a/lib<X>.so` -> `zipalign -p 4` -> `apksigner`
   -> `build/android/arm64-v8a/apk/<X>/<X>.apk`. Build-tools and
   platform versions are pinned beside the NDK pin
   (`tools/android/ndk_fixum.sh`: `INSTRUMENTA_VERSIO=34.0.0`,
   `PLATFORMA=android-34`); absent -> exit 2 naming the path.
5. `-instituere` = `adb install -r`; `-currere` = `am start -n
   earth.golden.<X>/android.app.NativeActivity`. Device preflight
   (one authorized device or `ANDROID_SERIAL`) and the host lock are
   `probatio.sh`'s, shared, not copied.

Signing: a house debug key `~/.rhubarb/android/debug.jks`, created
once by the script with `keytool` (JKS - the system Java 8 cannot
read the modern PKCS12 default, spike), mode 0600, never in the repo.
Release key, our own v2 signer, store upload = B6.

Only arm64-v8a in B1 (the Fire 7's userspace is 64-bit).

### II.8 The smoke: `apk.sh apps/pictor/pictor.c -fumus`

build -> install -> `adb shell setprop debug.rhubarb.fumus 30` ->
launch -> `adb shell input swipe` across the canvas (a stroke) ->
wait for `EXITUS=` in logcat (timeout -> apparatus, exit 2) -> pull
`fumus.png` via `adb exec-out run-as earth.golden.pictor cat
files/fumus.png` into `build/android/arm64-v8a/apk/pictor/` ->
`setprop debug.rhubarb.fumus 0` -> **uninstall** (Fran's cleanup
rule, android-gradus II.5; `-servare` keeps it). Exit 0 only with
`EXITUS=0` and a PNG. The PNG is then looked at (Read); a stroke in it
proves touch -> dispensator -> pictor -> volumen -> frame -> screen.

### II.9 Shared material and compositions (authoring rules)

For every **shared** library (document formats, actions, figures,
insula, volumen, theme) - hard rules, checkable:

1. No platform call: services arrive as ports from the host (the
   `imago_capere` pattern). Checked by the gate (II.10): pictor's
   Android closure must link.
2. Time comes from `nunc` (the dispensator's), never "N ms since I
   last ran"; nothing blocks or runs its own loop - on Android a
   paused app gets no frames.
3. State that matters lives in insula / volumen, never only in C
   memory - Android kills background processes (B2 tests this).

For a **composition** (`pictor_componentia`-like, per surface): it
may assume its surface - hover, right-click and keys on desktop;
touch and portrait on a tablet. Reachability by a primary press and
any-size layout are the *tablet composition's* design concerns, not
every author's. Paths come from the host (`a->sedes_datorum`), never
from the binary's cwd by assumption. A document belongs to the
aspect ratio it was made in; other clients may show it read-only.

These go into `include/testa.h`'s header comment (the contract's
home) and a pointer from the insula/vicus docs.

### II.10 The gate `android-struere`

A new merge-tier gate (`gradus="fusio"`): builds the pictor APK, no
device. Triggered by changes in pictor's Android closure (portae
declaratae). NDK or SDK absent -> **refuses** (exit 2, "NDK absens:
<via>"), never a silent skip. Registered in `pythonica/silva.py`
PORTAE (a new gate script unregistered = commits without it - lesson
0d069357).


## III. Tests

1. **macOS parity, byte-exact.** Before the move, PNGs from today's
   binaries: `pictor -fumus -imago`, `scriba -fumus -imago`, `vicus
   -fumus -imago`, saved under the build tree. After: the same runs
   produce identical files (`cmp`). The existing `apps/pictor/
   fumus.sh` stays green. vicus's fullscreen path (not under fumus)
   is checked by Fran opening vicus.
2. **Contract suite** (`probationes/probatio_testa.c`, credo v2):
   a no-window test shell (`tabula_pixelorum_creare_nuda`) drives a
   tiny test app through `applicatio_initium` -> events -> frames ->
   `finis`: initium once, finis once and last, `MUTARE_MAGNITUDINEM`
   before the first frame, `SUSPENSIO`/`RESUMPTIO` delivered in
   order, a FALSUM initium makes no window and no finis.
3. **Pure pieces** (`probationes/probatio_testa_android_initus.c`):
   every row of II.6 - down/move-with-history/up/cancel, tool types,
   `IGNOTA` pressure, margin mapping (inside, margin, exact edges),
   second pointer ignored then accepted after all-up, the frame
   decision's table. Runs on the host and on the Fire through
   `tools/android/probatio.sh`.
4. **Device smoke** (II.8): green = `EXITUS=0` + PNG with a visible
   stroke + the app uninstalled afterwards (`pm list packages` lacks
   `earth.golden.pictor`).
5. **Gate born red**: `android-struere` with a planted platform call
   in a shared pictor lib (a `clipboard_capere_imaginem` call in
   `lib/pictor_actiones.c`) -> link fails, naming the symbol; with
   `NDK` path planted wrong -> exit 2 naming it.
6. **Fran, by hand**: home-screen icon -> pictor -> draw with a
   finger, change tool on screen, spraycan, lock/unlock the tablet
   (pause/resume), back = quit, reopen (the volumen kept the
   drawing).


## IV. Not in this spec

- **B2** state across process death (`onSaveInstanceState`, killed
  in background); **B3** immersive mode / nav bar (first JNI),
  rotation, density; **B4** soft keyboard / IME; **B5** multi-touch
  (pinch-zoom), stylus pressure; **B6** own APK signer (v2 over
  BearSSL), release key, icons, armeabi-v7a, Amazon Appstore
  submission API.
- The **tablet composition** of pictor (its own sub-project, D3).
- Porting vicus or scriba to Android (reactor on `ALooper_addFd`,
  energia, sound - II.2).
- The terminal surfaces (`*_terminalis.c` on `ludus_tessera`) - they
  could become a third shell on this contract later; untouched here.
- `concha` (the older widget shell) - untouched.
- iOS - the third shell later (UIKit + CADisplayLink, an `.m`); the
  contract is meant to fit it, not tested against it.
- Hot reload (Handmade Hero style) - possible on macOS later; app
  state holds function pointers (dispensator, figure registries,
  `ImagoFons`), so a reload must re-register them. B1 only avoids
  making it harder.


## V. Order of work (the plan will follow approval)

1. **T1** `testa.h` + `lib/testa_fenestra.c`; parity PNGs captured
   first; pictor moved; scriba and vicus moved; `ludus_fenestra_
   currere` private; probatio_testa.
2. **T2** `postulata_android.h` + `lib/testa_android_initus.c` (pure
   pieces) + their suite, green on host and on the Fire.
3. **T3** `lib/testa_android.c` + `tools/android/apk.sh` (build,
   key, install, launch) -> pictor on the Fire by hand; the scale
   measurement of II.5 decided and recorded here.
4. **T4** the smoke (II.8), the gate (II.10), the authoring rules
   (II.9) in `testa.h`.


## AUDIENDA (not verified when written)

- Whether pictor's desktop composition lays out cleanly at 512x300
  (II.5) - measured in T3.
- `setprop debug.*` from `adb shell` on Fire OS 8 (stock Android
  allows the shell user to set `debug.*`); `run-as` on a debuggable
  APK under Fire OS; `input swipe` coordinates equal window
  coordinates with the nav bar showing.
- That a 1024x600 nearest-neighbour blit per frame keeps ~60 fps on
  the mt8168 with pictor's own drawing on top (the spike blitted a
  gradient only).
- `ALooper_addFd` on a pipe for stdout -> logcat: no lost output at
  `finis` (the pipe must be drained before `onDestroy` returns).
- `AChoreographer_postFrameCallbackDelayed64` (API 29) behaves on
  Fire OS 8 as on stock Android.
- Build-tools 34.0.0 / android-34 integrity is not pinned by hash
  (installed from Android Studio, not by our installer).
- That a VIII MiB first piscina block (pictor and scriba ask IV
  today) changes nothing observable - PNG parity (III.1) would show a
  layout change, not a memory one.
- vicus fullscreen parity under the new shell (III.1 covers only the
  fumus path).
