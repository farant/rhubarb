# auditus worklog

## 2026-10-21 — S3b: live output through CoreAudio (sexta)

Spec `project-specs/sonus-spec.md` §IV.1, §VI-a; header approved by Fran
with atomus/circulus (10-21). Pattern = energia: `include/auditus.h`,
`lib/auditus_macos.m` (plain C API, but Apple headers are not C89 ->
.m; system headers BEFORE latina.h), `lib/auditus_posix.c` stub
(aperire FALSUM - silent, never broken), aedilis variants (macos posix),
`aedilis.stml` nexus rule `-framework AudioToolbox -framework CoreAudio`.

**Unit**: `kAudioUnitSubType_DefaultOutput` (follows the system default
device), our input format 48 kHz f32 stereo INTERLEAVED (`IsFloat |
IsPacked`, no NonInterleaved flag) - the unit converts to the device.
`mixtura_reddere` writes straight into `mBuffers[0].mData`.

**Callback** (CoreAudio's real-time thread; we create no thread): drain
command ring -> `mixtura_iubere`; `mixtura_reddere`; reports ->
report ring (ring full: STOP moving and leave the rest in the mixer's
own queue for the next block - never dropped here); publish the clock
(`atomus_publicare`, 32 bits). No allocation, lock, file or stderr.

**Clock** = frames rendered (published AFTER the render = start of the
next block). 32 bits because atomus is i32-only; the main side extends
wraps into s64 (2^32 frames = 24.8 h; call at least every ~12 h).

**Latency** (`auditus_mora`) = device latency + safety offset + buffer
frame size (output scope, device frames) scaled by 48000 / nominal
device rate, + the unit's own `kAudioUnitProperty_Latency`. Measured on
Fran's machine: 731 frames = 15.2 ms.

**Close**: `AudioOutputUnitStop` is synchronous - the running callback
finishes before it returns, so the mixer is the caller's again after
`auditus_claudere`.

**Smoke tool** `tools/auditus_fumus.c` (aedilis-built, nothing
installed): schedules punctum, radius, fragor (left), passus x4
(left/right), crepitus at `momentum + mora + 50 ms` offsets, polls the
clock with nanosleep, feeds reports to the rector, stops crepitus with
a 100 ms fade, asks for a census. First run 2026-10-21: clock 3901 ->
138206 (2.80 s), 8/8 commands, 8/8 voices finished, census tarda 0 /
irrita 0 / amissa 0 / nuntii amissi 0, apex 0.836; 0.04 s user CPU for
2.8 s of audio. No unit test: CoreAudio needs the hardware; the pure
parts (mixtura, circulus, atomus) carry the tests.

**Fran listened 2026-10-21** (auditus_fumus, live): "it works!" - the
first sound the house made LIVE through its own mixer (S1c's audition
went through afplay).
