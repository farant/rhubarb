# tessellatio — plan (the terminal as a render target for Mandata)

*2026-10-01. Covers terminal-planning modules/012 (Mandata → cells) and
Fran's drawing-layer metric (module 012's units question,
2026-09-30). This is the layer the design-vocabulary research
(features/021) will build on.*

*Worktree `../rhubarb-secunda`. Executed INLINE, one task per turn, with
Fran approving each. Tasks with a terminal step end with Fran's own look
before the commit. Names marked (unsealed) are working names, and Fran
names them. Decisions D1–D8 below are PROPOSED; Fran decides before T1.*

## 1. Goal

The same `Mandata` display list that `delineare_mandata` turns into
pixels can also be drawn into terminal cells. The drawing layer gains an
explicit **metric**: what a cell is in "our pixels", and how big text is.

| today | after this plan |
|---|---|
| `Mandata` can only become pixels (`delineare_mandata` → fenestra) | it can also become terminal cells (`tessellatio` → tessera) |
| the cell size is implicit: `FONS_LATITUDO` 6 / `FONS_ALTITUDO` 8 hardcoded in `fenestra_textus.c`, `layout.c` (×2), `importatio_visus.c` | a named metric, `Modulus` (unsealed), that each target declares |
| nothing can measure text; pictor puts its title at `(2, 2)` and hopes | `modulus_textum_metiri` (unsealed): text → its size in our pixels, measured BY THE TARGET |
| `textus` ignores its `fons` index and the coetus scale | the same in both adapters, written down (D5) |

Out of scope:
- layout (`componere` taking the metric: the next plan);
- events (module 013, `ludus_tessera`);
- the design vocabulary (features/021);
- the retina device ratio in fenestra;
- `pictor` running live in the terminal.

## 2. What exists (read 2026-10-01, main aebac61f)

- **`include/mandatum.h`.** Six primitives in a flat xar:
  - `coetus` (fines, sectio, translatio, integer `scala`,
    `magnitudo_arboris`), `rectangulum` (impletum), `linea` (two points,
    crassitudo), `polygonum`, `imago` (by name, through an `ImagoFons`)
    and `textus` (x, y, chorda, `fons` index).
  - Colour can be a THEMA role, a palette INDEX, or RGBA.
  - Coordinates are `s32` "our pixels".
  - STML serialization exists (`mandata_scribere_stml` /
    `mandata_legere_stml`).
- **`lib/delineare_mandata.c` (393 lines), the native adapter.**
  - A stack of scaenae (origo, scala, clip ∩) driven by
    `magnitudo_arboris`.
  - Primitives are scaled by `scala`. Text goes through
    `tabula_pixelorum_pingere_chordam`, which is UNscaled, unclipped, and
    ignores the `fons` index.
  - Text advance is `FONS_LATITUDO` (6) and the line step is
    `FONS_ALTITUDO` (8). (`FONS_LATITUDO_REDDENDI` 8 = columns drawn per
    glyph.) This settles module 012's snag: **the fenestra cell is 6×8**.
- **`fons_6x8` coverage.** It maps only ASCII and Latin-1. Every other
  codepoint is one TOFU glyph, one cell wide, so **native text width ≠
  terminal text width** (CJK is 2 under runae). Measurement must be
  per target (D4).
- **Tests.**
  - `probatio_delineare_mandata.c` has pixel asserts plus the image
    golden `mandata_prima` (`probationes/pictor/specimina`). That golden
    is the native regression gate.
  - `probationes/pictor/pictor.arbor.stml` is a real component tree,
    which `pingere` turns into `Mandata`.
- **Producers.**
  - `lib/figura.c`: `pingere` and `figura_finium`.
  - `lib/pictor_figurae.c`: the tabula is an `imago` + `linea` + a 3×3
    `rectangulum` cursor; the titulus is a filled `rectangulum` +
    `textus` at (2, 2).
  - `apps/pictor` is 640×480, which is about 107×60 cells at 6×8.
- **Terminal side.**
  - tessera already draws boxes and lines (`tessera_quadrum_pingere`,
    `tessera_lineam_pingere`, `TesseraLineaGenus`) and places one
    drawable unit (`tessera_graphema_ponere`, wide-aware).
  - `quadrans` turns pixels into cells.
  - `runae_unitas_proxima` / `runae_latitudo_textus` measure text.
  - effigies and saltuarius show the "pure cells, thin placement"
    pattern (`effigies_pingere`, `saltuarius_pen_cellulas`).
- **Dependency rule.** No `lib/*.c` includes tessera, and tessera's pin
  is "grid + input, forever". So the adapter cannot live in tessera,
  and its pure core should not depend on tessera (D2).

## 3. Decisions (proposed; Fran decides)

**D1 — the metric is declared by the target and chosen by the host.**
A small struct, `Modulus` (unsealed; Vitruvius's *modulus*, the one unit
every dimension derives from):
- `cellula_latitudo` / `cellula_altitudo`: our pixels per cell;
- `extensio_latitudo` / `extensio_altitudo`: the surface in our pixels;
- a device ratio as a rational pair per axis (num/den, 2-D). It is
  carried but unused in this plan, which reserves the field.

What fills it, per target:
- **Native:** the metric is an OUTPUT, the font's facts (6×8 × scala).
  `delineare_mandata` gains a function that reports it.
- **Terminal:** the metric is a CHOICE. It defaults to 6×8, so that a
  layout made for fenestra maps one glyph to one cell. Any metric works,
  including 1×1 (cell-native apps) or 8×16.

**D2 — the pure core returns cells; placement is thin and separate**
(the quadrans/effigies pattern):
- `lib/tessellatio.c` (unsealed) turns `Mandata` + `Modulus` into a
  caller-sized grid of `TessellatioCellula` (unsealed).
- A cell is a text *unit* (a view into the interned chorda: offset +
  length, so grapheme clusters survive), plus its width (1/2), fg, bg,
  and a box-junction mask (D6).
- No tessera include. The thin placement into `TesseraOpus` lives
  beside saltuarius's pencil (or in a tessera instrumentum) and calls
  `tessera_graphema_ponere`.

**D3 — `Mandata` coordinates stay in our pixels.** Cells are DERIVED
through the metric. `mandatum.h` is unchanged: the interface already
works, and changing it would ripple through every producer.

**D4 — text is measured by the target.**
`modulus_textum_metiri(modulus, mensor, textus)` (unsealed) returns
columns × cell width and lines × cell height. `mensor` is a per-target
width rule:
- native: one cell per codepoint, as `fons_6x8` draws it;
- terminal: runae units under the width policy.

The test pins the divergence: "中" measures 6 px wide natively and
12 px in the terminal.

**D5 — the rounding and scaling rules** (written in the header, tested by
property):
- **Positions.** A primitive at our pixel (x, y) maps to cell
  (⌊x/cw⌋, ⌊y/ch⌋) after the coetus transform. That transform is the
  native one exactly: origo + local × scala.
- **Filled rectangles** cover cells by NEAREST EDGE: [round(x0/cw),
  round(x1/cw)). A rectangle smaller than half a cell vanishes on the
  cell path. That is honest, and the pixel path is the alternative.
- **Text** starts at its cell and advances by unit width.
- **Scale and text.** `scala` scales positions, never glyph size: a
  terminal character is one cell, as native text is unscaled today
  (V1). Big text is a later experiment.
- **Clip** (sectio) clips to whole cells by the same nearest-edge rule.

**D6 — which primitives take which path.**
- **Cell-native:**
  - `textus`;
  - axis-aligned `rectangulum` (fill → background; outline → box
    drawing);
  - axis-aligned `linea` (→ `─ │`). Crossings merge into junctions
    (`┼ ├ ┬ …`) through a per-cell 4-bit mask (up/right/down/left).
- **Pixel path:** `imago`, `polygonum`, and any non-axis `linea`.

**D7 — compositing the two paths.**
- The pixel path rasterizes ONLY its primitives, with the existing
  `delineare_mandata` and the same Mandata filtered, into an offscreen
  `TabulaPixelorum` at extent size.
- `imago_scalare` AREA brings it to cells × 2, and quadrans gives the
  cells.
- Cell-native primitives are then applied in painter's order on top.

v1 rule: a cell-native primitive overwrites the pixel layer *for the
cells it covers*. A text cell over an image keeps the image cell's
colour as its background when the text has no background of its own
(labels over images). Module 012's third open question is answered by
painter's order.

**D8 — the test subject and the look.**
- Goldens are text grids (as in folium):
  - `mandata_prima` (the native gate's own scene) through the terminal
    adapter;
  - a hand-made scene (panel, frame, crossing lines, wide text, clip);
  - `pictor.arbor.stml` through `pingere` with `figura_finium`.
- The look: a viewer instrumentum, `tessellatio` (unsealed), reads a
  `Mandata` STML file (or the built-in scenes) and shows it in the
  terminal. Keys switch the metric (6×8 / 8×16 / 1×1) and the scene.
  Fran checks it in Ghostty and Terminal.app.

## 4. Tasks (inline, one per turn; red first; plants caught by name)

**T1 — the metric and text measurement (pure).**
- `Modulus` (D1) and the conversions: our pixels → cell (floor),
  rectangle edges (nearest), and cells → our pixels.
- `modulus_textum_metiri` with the two width rules (D4).
- Red first: hand cases (6×8: x = 11 → column 1; 中 = 6 vs 12;
  multi-line; empty).
- Plants: floor ↔ round swapped; the native rule using runae widths; the
  line step using cell width.

**T2 — the native adapter declares its metric** (no behaviour change).
- `delineare_mandata` reports 6×8 × scala.
- `fenestra_textus.c`, `layout.c` and `importatio_visus.c` read the
  named metric, so the magic 6 and 8 go away (where safe; `layout.c`'s
  two copies at least).
- **Gate:** `mandata_prima` byte-identical, pictor suites unchanged.
- Plant: the metric off by one → the golden goes red.

**T3 — terminal adapter, cell-native** (D2, D3, D5, D6).
- Text (units, wide, clusters), filled and outlined rectangles, axis
  lines with junction merging, coetus translation + scala + clip.
- Colour resolution through `color_ex_mandato` (THEMA/INDEX/RGBA → RGB).
- Goldens: the hand scene + `mandata_prima` (cell-native parts).
- Plants: junction mask bit swapped, clip ignored, wide text advancing
  by 1, scala applied to glyph size.

**T4 — terminal adapter, pixel path + compositing** (D7).
- **Prerequisite (found in T3, 2026-10-01):** the pure pixel-table
  functions (`tabula_pixelorum_vacare`, `_obtinere_pixelum`, …) live in
  `lib/fenestra_macos.m`. Linking the rasterizer therefore pulls Cocoa +
  `-framework` into any program, saltuarius included. Move them into a
  plain library first. That is a no-behaviour-change step for fenestra,
  and fenestra's suites are the gate.
- `imago`, `polygonum` and diagonal `linea` via offscreen →
  AREA → quadrans.
- Painter's-order overwrite; background kept under text.
- Goldens: `mandata_prima` complete; a label over an image.
- Plants: the pixel layer drawn over text; the scale factor transposed.

**T5 — placement, viewer, look** (D8).
- Thin placement into `TesseraOpus`.
- The viewer (STML scenes, metric switching).
- `pictor.arbor.stml` scene golden.
- Headless goldens through the memory pons, then Fran's look in Ghostty
  + Terminal.app (the 256-colour path).

**T6 — RELATIO.**
- Phase log.
- terminal-planning module 012 (→ `completed_` if it holds) and the
  README.
- Merge main in, fast-forward main, and the briar MUTATIONES line for
  the new corpus library.

## 5. Names to seal (Fran)

- The library: `tessellatio` (the laying of tesserae, which is what
  this does, and tessera's family).
- The metric: `Modulus` (Vitruvius, *De architectura*: *symmetria* in
  I.2, the Doric *modulus* in IV.3, the unit from which the temple's
  measures derive).
- The cell type, the measuring function, and the viewer.

Every name gets checked against latina.h's reserved words and the
glossary before use. `modulus` already occurs in `moneta.h`, `qr.h` and
`sputnik_interpres.h` (other senses), so it needs no new gloss.

## 6. AUDIENDA (not verified)

- Whether `pictor.arbor.stml` + `figura_finium` alone (without the
  pictor document) gives a scene worth looking at. `figura_tabulae`
  needs `PictorFigurae.doc`.
- Whether filtering one `Mandata` into "pixel-path only" for
  `delineare_mandata` is cheap (a copy per frame) or needs a predicate
  hook in the rasterizer (T4 measures it).
- Whether `delineare_mandata`'s `scala`-unaware text is a bug or a choice
  (V1 says "scala integra; textus non praecisus"). D5 follows it either
  way.
- Whether box-drawing glyphs show seams in Terminal.app, which draws them
  from the font (Ghostty draws them itself). The Q5 look saw this question
  for block elements.
- Whether `layout.c`'s 6/8 copies are safe to replace in T2. Its widgets
  are the older generation, so check for callers first.
