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
