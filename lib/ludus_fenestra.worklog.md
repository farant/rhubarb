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
