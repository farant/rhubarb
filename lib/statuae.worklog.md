# statuae worklog

## 2026-11-10 - T8 of project-specs/statuae-plan.md

- **Structure.** One root component (PARTES_TABULA) whose figure
  draws everything; its children are hit areas only ("pictura",
  "sagitta_prior", "sagitta_proxima", "lux", "duplex", and the two
  dropdown buttons "cyclus" and "modus"). The open dropdown's menu is
  the last child (elenchus, PARTES_DIALOGUS, drawn by
  `elenchus_figura` with `&m->menu` as context). Same shape as the
  antiphonale and theremin panes.
- **Both the tick and the composer keep the frame current**
  (`statuam_curare` + `quadrum_curare`). An event changes state, the
  host recomposes and paints; if only the tick refreshed the frame,
  the picture would lag one tick behind a key press. The frame is
  rebuilt only when its key string changes (model seal, clip, clip
  time in ms, angles, mode, look, 2x, size), so the double call is
  free.
- **Clip time is integer milliseconds** from the port's clock, taken
  modulo the clip's duration before it becomes a float, so the walk
  does not lose precision after hours. A full cycle later the key is
  identical and nothing is recomputed.
- **Pause** stores the clip time in memory (`quietum_ms`); `quiescit`
  is the persisted flag. A pane restored paused starts at time 0.
- **Drag** holds the angles in the montation and writes `gyrus` and
  `inclinatio` once, on release. Deltas come from raw event
  coordinates (only differences matter), so capture outside the pane
  works without `destinatio_ad_locale`.
- **2x** renders at half size and doubles the pixels itself
  (`quadrum_duplicare`) instead of using a scaled drawing group; the
  image handed to the host is then always at pane resolution and
  centred in the picture area. An odd size loses its last row or
  column.
- **Dropdowns.** `agere` gives every event to
  `delectus_eventum_agere` first while one is open. Options live in
  the montation (`menu`, `menu_res`, `menu_textus`: at most 16, 63
  bytes each) because the menu's figure reads them at paint time.
  Choosing a clip writes its NAME to `cyclus` and restarts clip time;
  a model without that clip name falls back to its first clip.
- **Changing model** clears `gyrus` and `inclinatio` (back to the
  defaults 300 / 150 tenths of a degree) in the same write as the
  title.
- **Header at 360 px wide fits one row** with both fixtures (spec
  decision 10's fallback to two rows triggers at about 200 px; test
  "dispositio").
- **latina.h bit twice here:** `duplex` (double) and, in the test,
  earlier `longus`. The manifest field is still called `duplex` (a
  string); the C identifiers are `gemina` / `geminatum`.
- `statuae_dimittere` destroys the three montation arenas. Whether
  vicus has a hook to call it on close is T9's question.
- Not yet seen on screen: the header and the menu have only been
  checked through the drawing commands they emit. T9 takes the
  screenshots.
- Test runner note: `./compile_tests.sh <filter>` goes on to side
  checks (qr_gyrus, speculum) after the suites; under load that looked
  like a hang. The suite's own verdict is in
  `build/test_logs/singulae/<probatio>.log`.
- Plants: frame key ignoring the mode -> "delectus", "modi"; volume
  never re-polled -> "vacuum"; open dropdown not swallowing keys ->
  "delectus".
