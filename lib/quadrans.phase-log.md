# quadrans phase-log

Plan: `project-specs/quadrans-plan.md` (D1–D8 approved by Fran
2026-09-30, "all the recommendations make sense"; D6 test photographs
supplied by Fran, `probationes/fixa/quadrans/`).

## Q1 — the pure core (2026-09-30)

**INTENTIO.** `include/quadrans.h` + `lib/quadrans.c`: an image region →
the caller's array of `QuadransCellula {runa, color_litterae,
color_fundi}`; no allocation, no state, depends on latina.h +
imago_typus.h only. `quadrans_optiones_ordinariae`, `quadrans_mensurare`
(cells for a region: half, rounded up), `quadrans_computare`.

**The algorithm, read from OpenTUI's code** (`packages/three/src/shaders/
supersampling.wgsl`, `renderQuadrantBlock`, commit 7581976f) — and, as
with Ghostty's width rule at runae U2, the planning note's paraphrase
was wrong in three places:
- OpenTUI's `averageColorsWithAlpha` chains `blendColors` ("over"
  compositing), not an average: for opaque pixels it returns TL alone;
- mask 0 is UNREACHABLE and 15 happens only for a uniform block: the
  dark candidate is one of the four pixels, so its bit is always set,
  and the light candidate always stays clear unless dark == light;
- pixels outside the image read as opaque black.

Kept from OpenTUI: the most-different pair (squared RGB distance; FIRST
pair in (0,1)(0,2)(0,3)(1,2)(1,3)(2,3) order on ties), the luminance
split (2126/7152/722; first of the pair is dark on a tie), dark-or-equal
sets the bit (TL 8, TR 4, BL 2, BR 1), the 16-glyph table. Deliberate
differences, documented in the header: alpha composited against
`fundus` FIRST (tessera cells have no alpha; the algorithm then sees
opaque RGB only); a uniform block → a SPACE with that colour as its
background (never a glyph seam in a font-drawn terminal); pixels outside
image or region → `fundus`; integer arithmetic (identical on every
machine). D2's two colourings are both in: EXTREMA (OpenTUI: the
extreme pair) and MEDIA (each group's mean, rounded half up) — Q2
measures which wins.

**Red first:** stubs; every case red by name. **Green:** 135/135 —
all 14 mixed glyphs from black/white blocks under both colourings (the
glyph table in the test is retyped from the WGSL, not copied from our
code), uniform → space, EXTREMA vs MEDIA on a three-grey block
(▀, 0/255 vs 5/228), distance ties, luminance ties with two colours of
EQUAL integer luminance ((0,38,239) and (209,0,0) = 444,334, found by a
search), alpha 0 and 128, an odd 3×3 region (edge cells ▐ ▄ ▟ against
the background), a region partly outside the image, a NULL image, and
the sizes.

**Plants (compiling), all caught by name:** last pair on ties (caught
ONLY by a diagonal A B / B A block, added for it — the earlier tie cases
had both candidate pairs in the same colour order, so the plant would
have stayed green), luminance inverted, bit order reversed, alpha
ignored, MEDIA's rounding changed, uniform block not a space.

Formatter note: `(s32)RUBER(a) - (s32)RUBER(b)` with function-like
colour macros drew "one space before a binary operator (4 for 1)" from
the formator; the code is cleaner with an interior `_canalis(c, k)`
anyway, and the macros are gone.

## Q2 — the error metric, and D2 decided (2026-09-30)

**INTENTIO.** `quadrans_error`: rebuild each cell's 2-colour pattern
(bit set = foreground, else background; a space = background), compare
with the source composited over `fundus`, return the MEAN squared error
per channel in HUNDREDTHS (integer, i64 sum — exact, so it can be
committed). Then measure D2 — EXTREMA (OpenTUI) vs MEDIA (group means).

**Theory before the numbers.** For a FIXED partition of the four pixels
the group mean minimizes the squared error of each group; MEDIA uses the
same partition as EXTREMA, so MEDIA ≤ EXTREMA always. The measurement
says by HOW MUCH; the test asserts the inequality on every image as an
independent check (a violation would be a bug, not a result). Whether
the extremes LOOK sharper is a question for Fran's look (Q5), not for
this metric.

**Hand cases (red first against a stub):** the three-grey block —
EXTREMA 9375/12 → 78125, MEDIA 4689/12 → 39075; a two-colour block → 0;
an empty region → 0.

**The measurement** (photographs scaled with `IMAGO_SCALA_AREA` to fit
160×96 sub-pixels ≈ an 80×48-cell terminal):

| image | sub-pixels | EXTREMA | MEDIA | MEDIA/EXTREMA |
|---|---|---|---|---|
| assumptio (Fran) | 68×95 | 10924 | 5113 | 46% |
| christus_sculptus (Fran) | 105×95 | 4536 | 2087 | 46% |
| gradiens | 160×96 | 47 | 30 | 63% |
| tabula (1-px checker) | 160×96 | 0 | 0 | exact |
| strepitus (LCG noise) | 160×96 | 292813 | 147210 | 50% |

On the photographs MEDIA halves the error (RMSE per channel ~10.5 → 7.2
levels on the painting, 6.7 → 4.6 on the bust). **D2: MEDIA is the
default** (`quadrans_optiones_ordinariae`), pinned by a test. The numbers
are `probationes/fixa/quadrans/aurum_errores.txt` — a RATCHET: every
error may only fall (`QUADRANS_AURUM_SCRIBERE=1` rewrites it after a
deliberate improvement).

**Green** 162/162. **Plants (compiling), all caught by name:**
reconstruction always using the foreground (hand cases + ratchet on all
5), quadrant index transposed (hand cases + ratchet), default back to
EXTREMA (the default assertion — the measurement sets its colouring
explicitly, so only that case sees it), MEDIA quietly degraded (a Q1 case
+ the MEDIA ratchet on 4 images).

## Q3 — half-block mode (2026-09-30)

**INTENTIO.** `QUADRANS_DIMIDIUM` (planum D3): a cell is 1×2 pixels; the
top pixel is the foreground under U+2580 (▀), the bottom the background;
equal pixels give a space (same reason as Q1's uniform block). Colours
are EXACT per pixel; horizontal resolution is halved.
`quadrans_mensurare` gives cells = width × half the height;
`quadrans_error` rebuilds the mode too.

**Measuring it honestly.** At its OWN resolution the half-block is exact,
so its error is 0 — true, and useless as a comparison. The fair question
is "for the same terminal": an 80×48-cell grid is 160×96 sub-pixels for
quadrants but 80×96 for half-blocks. The test scales each photograph to
both grids (IMAGO_SCALA_AREA), rebuilds the half-block result at 160×96
(each pixel covers two columns) and compares both with the SAME 160×96
reference:

| photograph | quadrants (MEDIA) | half-blocks | ratio |
|---|---|---|---|
| assumptio | 5113 | 19412 | 3.8× |
| christus_sculptus | 2087 | 15146 | 7.3× |

Doubling horizontal detail matters far more than exact per-cell colour —
most on the bust, whose detail is fine texture. **The viewer's default
should be quadrants** (Q5), half-blocks an option. Both comparison
numbers are in the aurum ratchet (`<name>_modi` lines).

**Red first** (cases red by name; the comparison read nonsense before
the mode existed). **Green** 186/186 — the ▀ split, equal → space, an odd
height (missing bottom pixel = `fundus`), sizes, exactness (error 0) on
both photographs, the ratchet. **Plants (compiling), all caught by name:**
fg/bg swapped, width halved as in quadrant mode, the error rebuild
flipping top and bottom (only the exactness assertion sees this one),
equal pixels not a space.

## Q4 — tessera's colour policy (2026-09-30)

**INTENTIO (plan D4, the real decision).** Terminal.app has no 24-bit
colour and MISREADS `48;2;R;G;B` (U5 look: the trailing parameter turned
text blue). A quadrant image puts a 24-bit background on nearly every
cell, so images there would be garbage. tessera gains a colour policy
shaped exactly like the width policy (runae U5c):
- `TesseraColores`: `TESSERA_COLORES_PLENI` (38;2/48;2, default) and
  `TESSERA_COLORES_CCLVI` (38;5/48;5 into the xterm 6×6×6 cube 16–231
  and the 24 greys 232–255; never the 16 theme colours, which users
  remap); field `colores` appended to `TesseraOpus`;
- `tessera_colores_ponere` (forces a full repaint — cells unchanged,
  encoding changed), `tessera_colores_ambitus`: COLORTERM
  truecolor/24bit → PLENI; else TERM_PROGRAM Apple_Terminal → CCLVI;
  else PLENI. Environment, never a query (the thesis);
- quantization at EMISSION only (`_cclvi` + one `_colorem_emittere`):
  cells keep true RGB, goldens stay exact. Nearest cube colour per
  channel (tie → lower level) vs nearest grey (the grey nearest the RGB
  mean — distance to a grey is convex in its level); cube wins ties;
- spectaculum, saltuarius and folium opt in from the environment.

**Red first:** stubs; the 256 goldens and the environment rule red by
name. **Green:** tessera 13/13 — byte goldens on hand-computed points
(red → 196, 808080 → 244 the exact grey, white → 231, black → 16,
336699 → 60 where the cube beats the grey), native colour emits no
38/48, cells keep RGB, and the environment for BOTH policies (the width
policy's environment rule had never been tested). **Plants (compiling),
all caught by name:** grey never chosen, cube level stuck, CCLVI
ignored at emission, COLORTERM ignored.

Amalgam: public header mirrors the enum, the field (exact order) and
the two functions; `TesseraColores` added to the amalgamator's
CADENDA_TYPEDEF (the header owns the type — same as TesseraPolitica at
U5c; without it: "typedef redefinition"). VERIFICATUM + idempotent;
saltuarius 13/13.

**Fran's look (Terminal.app, spectaculum):** the colour bar and the red
background behind 中 now show; no blue text.
