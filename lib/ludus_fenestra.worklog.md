# ludus_fenestra.worklog.md

## 2026-09-05 — natus (ludus P3 T10)

The only place ludus touches the clock and the only place it sees a
window. A frame is: pulse the dispensator (the quiet seat and the
staleness recompose), `pingere` the last tree into fresh `Mandata`
in a per-frame arena that is vacated first, rasterize into the pixel
table. The loop polls events, stamps a zero `tempus` from
`fenestra_tempus_ms` as a guard (the platform stamps in production,
the replay file in tests), hands each event to the dispensator,
runs one frame, presents, and stops on `EVENTUS_CLAUDERE` or after
`quadra_maxima` frames (the `-fumus` path). At exit it prints one
line: frames, ms in composition, ms in `pingere`, ms in rasterizing,
the worst frame. Those numbers are what decides brainstorm §XVI §3
— if rasterizing dominates, pipeline frame N's rasterization against
frame N+1's dispatch with double-buffered state.

The pixel table is created with the window's content height as its
fixed height, so scale is 1 and mouse coordinates equal table
coordinates. A scaled table (retina, or a fixed low resolution)
would need the mouse divided by the table's scale before dispatch;
that question is named here and not answered.

Wheel events: `lib/fenestra_macos.m`'s `scrollWheel:` is EMPTY
(2026-09-05). No `EVENTUS_MUS_ROTULA` is ever emitted, and the
Eventus union carries no position for one. Zoom-at-cursor (P4)
pulls: `x`/`y` on `datum.rotula`, emission in the `.m` with the
location, and `destinatio` treating a positioned wheel
geometrically. Not started here.

Gate: `probatio_ludus_fenestra` (10, headless: a bare pixel table, the
toy, `figura_finium` on the button; the loop itself is exercised by
T11's smoke). Planted fault: the per-frame arena not vacated and
the mandata reused — MUTE against the draft's `< X` bound (eight
accumulated elements are still under ten); the assertion now pins
the second frame's count to the first's, and the plant went red
there. Green on revert. Examen ACCIPE; formator 0 after four long
lines.

## 2026-10-05 — the window waits when idle

Found during scriba H2 (a stray pictor window sat at 100% CPU for ten
minutes): `ludus_fenestra_currere` polled (`fenestra_perscrutari_eventus`),
composed, drew and presented in a loop with no wait, since ludus P3.
The terminal glue already slept on `quies_ms`.

Now the loop blocks in `fenestra_expectare_eventus` (the house's
`nextEventMatchingMask:untilDate:` wait - vitrea, villa, forum, atrium,
silex use it) for `ludus_fenestra_mora(lf)`: 0 before the first frame
(draw at once), the dispensator's `quies_ms` after. An arriving event
wakes it, so strokes and drags are not delayed; the quiet flush
(`motus_quies` in `dispensator_pulsare`) still runs every frame, so it
lands within ~2 × quies_ms as in the terminal.

Measured: pictor `-fumus` (30 frames) under `/usr/bin/time` - real
8.95 s, user 0.15 + sys 0.04 s (~2%, startup included; before: a core
pinned). The smoke run is now ~9 s (idle frames wait 300 ms) instead
of instant - it is a smoke bound, not a benchmark. Fran's look: idle
CPU fine, drawing feels the same.

Test: `probatio_ludus_fenestra` asserts the wait (0, then CCC). Plants:
always 0 (= the old spin) and always quies (first frame delayed), both
caught. The loop's USE of the wait needs a real window - covered by the
measurement and Fran's look, not headlessly.

## 2026-10-06 — screenshots of frames (Fran: "might be a bit of an oversight")

fenestra draws in SOFTWARE: every frame is a TabulaPixelorum (RGBA
packed `a<<24|b<<16|g<<8|r`) handed to fenestra_praesentare_pixela, so a
screenshot is that buffer written out - no Screen Recording permission,
no dependence on the window being visible. Added (Fran approved the
draft): `tabula_pixelorum_in_imaginem` (pure; channels by SHIFTS, not
memcpy - a byte-order plant is caught), `ludus_fenestra_imaginem_scribere`
(last frame -> PNG, from the frame piscina; FALSUM before any frame or
on a failed write), and `-imago <path>` in apps pictor/scriba/vicus
(written after the frame loop; with -fumus = frame 30).

Headless for free: `tabula_pixelorum_creare_nuda` + ludus_fenestra_creare
+ `ludus_quadrum` renders an app with NO window (probatio_ludus_imago
writes build/probatio_ludus_imago_pictor.png). Claude can open the PNG.
First look: pictor's canvas sits with its top-left off-screen (only the
bottom/right border visible) - either pictor's real initial pan or a
headless artifact; the real-window smoke run (-fumus -imago) decides.

Not covered: the macOS composite (title bar, Retina scaling) - if ever
needed, fenestra_numerus_nativus + `screencapture -l<id>` (permission
prompt). 6 plants caught.

**Real-window smoke (Fran approved, 2026-10-06):** `pictor.sh -fumus
-imago /tmp/pictor_fenestra.png` -> exit 0, 30 frames; the window's PNG
is BYTE-IDENTICAL (`cmp`) to the headless one from probatio_ludus_imago.
So headless frames are faithful screenshots, and the off-screen canvas
corner is pictor's REAL initial view (not a headless artifact) - open
question for Fran whether that initial pan is intended.


## 2026-10-08 — vicus-latera S1c-1: pulse hook and frame skip

A living app (a terminal) must be pulsed every iteration, and the
shared loop waited up to quies_ms (300 ms) and then ALWAYS repainted
the whole window. `ludus_fenestra_pulsum_ponere(lf, fn, ctx)`: with a
pulse the loop waits at most 16 ms; `ludus_fenestra_pingendum(lf,
eventa, nunc)` (public so it is testable without a window) decides
whether to draw: events, or the pulse saw something, or - otherwise -
the dispatcher alone is pulsed (its timers, e.g. scriba's debounced
save) and the STORE VERSION is compared with the version at the last
drawn frame (`versio_picta`, set by ludus_quadrum). Without a pulse:
always draw, exactly as before.

Found while building it: the dispatcher recomposes on EVERY event,
including the empty pulse ("regula staleness"), so
`compositiones` cannot signal change - the store version
(`insula_versio`, bumped per accepted write) can. Comparing against the
LAST DRAWN frame (not just around this pulse) matters: a click handled
in an earlier iteration may still be undrawn. The twin
(ludus_tessera) got the same pair under its own names (its header must
not include the window's). Eight plants caught (version ignored,
version not recorded, no-pulse path changed, events ignored - each in
both loops). The 16 ms cap itself lives in the loops, which need a real
window / terminal: verified by use in S1c-2.

## 2026-10-08 - smoke mode vs the frame skip (S1c-1 regression)

With a pulse installed (vicus), the loop draws only on events, a pulse
change or a store-version change. An idle `vicus -fumus` drew frame 1
and then never reached its 30 frames - smoke runs and `-fumus -imago`
screenshots hung. Fix in both loops: with `quadra_maxima` set, every
iteration draws. Neither loop runs headless in a test (window /
real terminal); the twin was proven with `script -q /dev/null
vicus_terminalis -fumus` (a pty): before = still alive after 10 s,
after = exits, `quadra=30`. The window loop carries the same one-line
rule.

Side find the same day (not a fenestra bug): a GUI app that gets
mouse but never keys, even after a click, was launched inside an
ORPHANED tmux server (its original terminal app had quit). From a
fresh Ghostty tab the same binary types fine.
