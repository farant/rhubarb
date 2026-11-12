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

## 2026-11-10 - T9: in vicus (`$models`)

- **Glue** in `lib/vicus_applicatio.c`: genus `statuae`, verb
  `$models`, clock `fenestra_tempus_ms`, face = actions, figures,
  composer, image source, tick, tick interval, first-click drag,
  place title, Esc back. `probationes/probatio_vicus_statuae.c`
  drives it through the real application (typed and clicked
  `$models`, real clock).
- **The frame was being built two or three times per tick.** Found
  by measuring, not by a test: tick and composer both read the live
  clock, each saw a new millisecond, each rebuilt. The pane now reads
  the clock ONCE per tick (`hora_visa`) and everything else uses that
  value; composing without a tick cannot see new time (test "tempus",
  and a plant). Before: exemplaria 143 ms per tick plus 72 ms in the
  repaint. After: 69 ms and 1 ms.
- **Measured** (headless vicus, -O2 test archive, 20 frames, golem
  walking, lit):

  | window | pane picture | Atkinson | Bayer | exemplaria | None | whole-window repaint |
  |---|---|---|---|---|---|---|
  | 720x458 | 360x438 | 4.3 ms | 1.3 ms | 69 ms | 3.1 ms | 1.0 ms |
  | 960x561 | 480x541 | 6.3 ms | 2.0 ms | 110 ms | 4.9 ms | 1.4 ms |

  So Atkinson, Bayer and None hold 30 ticks a second at either size;
  exemplaria gives about 14 frames a second at 360 wide and 9 at 480.
  Repainting the whole window is about a millisecond in the headless
  rasterizer (the real app also copies to the screen; not measured).
- **Bare paper.** The pane's background when focused is the house
  focus pattern (dots), which showed around the model. The picture
  area now gets a plain background rectangle and the frame carries
  opaque paper (palette 5); the focus pattern stays in the header.
  This replaces "transparent uncovered pixels" from T8.
- **j / k in the dropdown with real key events.** Real events (and
  `manus_ludus`) carry the logical key as an uppercase letter and the
  typed letter in `runa`; `elenchus_clavem_agere` compares `clavis`
  with lowercase 'j' / 'k', so it never matched and the highlight did
  not move in vicus. T7's test had built its events by hand with a
  lowercase `clavis`. `delectus_eventum_agere` now maps j / k to the
  arrow keys itself (test with a real-shaped event, and a plant). The
  same mismatch presumably affects every other user of
  `elenchus_clavem_agere`; filed, not fixed here.
- **No close hook.** `vicus.c` allocates a montation in the vicus
  arena and never calls anything when a pane leaves the stack
  (`vicus_acervo_redire` truncates). `statuae_dimittere` is therefore
  never called by vicus: a pane that is closed and opened again
  leaves its three arenas behind (a few MB; 30-50 MB if exemplaria
  ran). A pane that stays in the stack is reused, not remounted.
  Filed as a design question.
- The golem covers about a third of a 360x438 picture: the framing
  sphere includes its wide arms. `STATUA_MARGO` and
  `STATUA_CAMPUS_GRADUS` in `statua_pictura.h` are the knobs.
- Screenshots are made headless (`tabula_pixelorum_creare_nuda` +
  `ludus_fenestra_creare` + `ludus_quadrum`), by a scratch program,
  not by a tool in the tree.
- **Second link failure of the project, same shape as T5's.** The
  first T9 commit attempt failed the `ludus_tessera` gate 0/8:
  `ludus_tessera/compile_probationes.sh` links `vicus_applicatio.o`
  from a hand-kept library list, and the list did not have the new
  pane and what it pulls in. Added there: statuae, statuae_quadrum,
  statuae_repositorium, statua, statua_glb, statua_pictura,
  stereometria, membrum__delectus, dithering_ordinatum,
  dithering_exemplaria_velox, dithering_exemplaria_tabulae. A new
  pane kind in `vicus_applicatio.c` always owes that list an entry;
  run `./ludus_tessera/compile_probationes.sh` before committing.

## 2026-11-10 - background = the vicus page colour

Fran: the paper around the models should be the warm colour of the
rest of vicus, not white. The frame's uncovered pixels are
transparent again (`charta_perlucida` VERUM) and the plain rectangle
under the picture, in the theme's `COLOR_BACKGROUND`, shows through -
the same colour scriba pages have (b6957d on screen). Not done by
making the frame's paper palette index 15: the dither palette
(`AQUINAS_PALETTE`, b4947c) and the theme palette differ by a couple
of levels there, which would leave a faint seam; and through the
theme the pane follows a theme change. `bin/statuae pingere` still
writes opaque palette-5 paper. Each dither still runs on a white
background, so the model's edge pixels are unchanged.

## 2026-11-12 - plastica T6: the prop group

- `$models` now has two groups, characters (`STATUAE_GENUS_STATUA`)
  and props (`STATUAE_GENUS_RES`). The field `grex` ("res" = props,
  absent = characters) says which is open; key `g` and the header
  button `chars` / `props` switch.
- **Each group remembers its own model.** The viewed title lives in
  `statua` for characters and in the new field `res` for props
  (`campus_visae`). With one field, switching groups would have
  forgotten where you were in the other.
- **The index is the open group's.** `indicem_relegere` reads
  `statuae_enumerare_generis` for the open group and records which
  group it read (`grex_visus`). `gregem_curare` re-reads whenever
  the field and `grex_visus` disagree, and runs on every pulse and
  every compose.
- **Found by a plant.** The first version re-read in the key handler
  and, for changes from outside the pane (undo), inside the
  once-a-second volume check. A plant removing the outside path
  stayed green; the test written to catch it failed for a real
  reason: for up to a second the title of one group was looked up
  in the other group's list. Hence one mechanism, ungated. The test
  pauses the clip and views the first prop so that a stale list
  would show the same model and the pulse would report no change.
- **Label.** The header approved with Fran said `characters`; the
  button is `chars`. At the ordinary 360 width the long word took
  the title's room. The command palette entry reads "Characters /
  props".
- An empty prop group says "No props yet" and names
  `plastica condere` (T7).
- Everything else is untouched: a prop turns, tilts, dithers like a
  character; with no clips the clip chooser is inert as before.
