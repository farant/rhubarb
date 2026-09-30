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

## Q5 — the viewer, `effigies` (2026-09-30)

**INTENTIO (plan D5, D8; name `effigies` sealed by Fran — `pictura`
was already a common word in tessera).** Shaped like folium:
- `tessera/instrumenta/effigies/effigies_pictura.{h,c}` (pure, linked by
  tessera's test runner; not in `fontes/` or the amalgam):
  `effigies_mensurare` fits an image to a cell area WITHOUT distortion —
  the cell aspect (w/h) is a parameter in hundredths (default 50 = 1:2,
  never queried); a quadrant sub-pixel is half a cell each way (tall),
  a half-block pixel a whole cell wide and half tall (square at 1:2);
  scales up or down, integer maths. `effigies_pingere` places cells
  (codepoint → UTF-8 → packed signum; colours → stilus).
- `tessera/instrumenta/principalia/effigies.c` + `tessera/effigies.sh`
  (generated source list: imago, imago_opus, quadrans resolved by
  aedilis; `aedilis.stml` root added): loads the images given (default:
  Fran's two photographs), fits the screen minus the status line with
  IMAGO_SCALA_AREA, centres; keys m (mode), c (colouring — added so D2's
  perceptual half could be judged), ] / [ (image), q; resize re-fits.
  Status: name, source size → cells, mode + colouring, colour depth,
  error.

**Red first** (stubs; the fitting cases and the placement red by name).
**Green:** the fitting cases (square 100×100 in 40×10 cells → 40×20
sub-pixels = 20×10 cells, square on screen; half-block → 20×20, the same
footprint; wide 400×100 → 80×10; tall → 10×20; a 1×1 image scaled UP to
fill; square cells, aspect 100 → 20×20; empty → 0×0), placement (▀ →
0x8096E2 = E2 96 80, colours; a space keeps its background), and an end-
to-end 4×2 image (▘ + a red space) read back. **Plants (compiling), all
caught by name:** half-block treated as quadrants (first did not apply —
the formatter had realigned the line — redone), aspect ignored, width
limit ignored, fg/bg swapped.

**Fran's look:**
- proportions right (the 1:2 cell assumption holds on his fonts);
- **Terminal.app: QUADRANTES + MEDIA definitely the best**;
- **Ghostty: EXTREMA "might actually be better — perceptually it feels
  like a sharpening filter"**. That is close to literally what it is:
  taking the most-different pair as the cell's colours pushes each cell
  toward local contrast (unsharp-mask-like). The error metric penalizes
  it; the eye, at full colour depth, may like it. Under 256 colours the
  extra contrast plausibly turns harsh — consistent with MEDIA winning in
  Terminal.app. One look, so recorded, not acted on: the library default
  stays MEDIA (the measured one). A candidate later: the viewer's default
  colouring follows the colour depth (PLENI → EXTREMA, CCLVI → MEDIA),
  or a MEDIA-plus-mild-sharpening colouring measured with a perceptual
  metric.
- chafa in Ghostty drew real pixels (it detected the kitty graphics
  protocol; `--symbols` does not apply) — `chafa -f symbols --symbols
  quad …` forces the character comparison.

## Q6 — the Aquinas palette mode (2026-09-30)

**INTENTIO (plan D4, the optional palette look).**
`effigies_palettam_applicare` (effigies module, not `quadrans` — which
stays allocation-free and knows nothing of dithering): the scaled image
→ Atkinson error diffusion into the 16 Aquinas colours
(`dithering_atkinson_colorum` + `dithering_indices_ad_rgba`) → a new
opaque image in the piscina. The viewer's `p` key toggles it; its error
figure is measured against the UNdithered image, so the pixel-level cost
of the look is shown honestly.

**Design finding:** the palette pairs with EXTREMA — after dithering
every pixel is a palette colour, EXTREMA takes two actual pixels as a
cell's colours, so cells stay exactly inside the palette; MEDIA averages
and drifts out of it. The test pins this as a property.

**Why no metric judges this mode:** dithering is deliberately pixel-
INACCURATE (it scatters colours for the eye to average). Against the
undithered reference at 80×48 cells:

| photograph | plain MEDIA | palette + EXTREMA | palette + MEDIA |
|---|---|---|---|
| assumptio (137×96) | 3360 | 112260 | 93568 |
| christus_sculptus (160×73) | 1990 | 74237 | 64930 |

~30× worse per pixel, as expected — the look decides.

**Red first** (a stub returning the image unchanged). **Green:** tessera
14/14 — a dithered gradient is all palette colours, fully opaque, same
size, and uses ≥ 3 CHROMATIC palette colours (7 in practice); EXTREMA
cells on it stay in the palette; the photo measurement asserts only that
the palette costs accuracy. The runner's library list gained dithering,
imago and imago_opus (the photos load in this suite).

**Plants (compiling):** no dithering (caught), indices not converted
(caught — first version did not compile, redone), a GREYS-ONLY palette —
**mute at first**: the test only checked membership, and greys are
members. Strengthened with the chromatic-colours assertion, then caught
(0 chromatic colours).

**Fran's look (2026-09-30): "that looks good!"**

---

## Q7 — saltuarius image preview (2026-09-30)

**INTENTIO (plan Q7).** saltuarius's preview pane shows a picture when the
selected file is one, instead of "(filum binarium)". Recognition by
SIGNATURE, not suffix: PNG `89 50 4E 47 0D 0A 1A 0A` or JPEG `FF D8 FF`,
checked BEFORE the binary guard (images are full of NULs). The limes
(disk edge) decodes, fits, scales (AREA) and computes the cells into its
preview arena, so `columnae_pingere` still allocates nothing per frame;
the columns module only centres and blits the stored cells
(`saltuarius_pen_cellulas`). The limes needs the pane size to fit, so it
gained `saltuarius_limes_amplitudo` (called at startup and on
AMPLITUDO/RESUMPTUM, followed by a fresh `praevisum`). Notes: size
unknown → "(imago)"; decode failure → "(imago non legibilis)"; the
existing 1 MB preview cap still applies ("(filum magnum)").

**API move:** the fitting maths left effigies for `lib/quadrans` as
`quadrans_aptare(img_lat, img_alt, max_lat, max_alt, modus, aspectus,
&sub_lat, &sub_alt)` + `QUADRANS_ASPECTUS_ORDINARIUS` (L = cell 1:2), so
saltuarius does not reach into tessera's instrumenta tree. effigies calls
it too; its fitting tests moved to probatio_quadrans section IX
(quadrans 200/200, tessera 14/14).

**Tests:** probatio_saltuarius_limes "imagines" (a copied
christus_sculptus.jpg and a corrupt fracta.jpg: unknown size → NUNTIUS;
80×24 → IMAGO inside the pane touching one limit; corrupt → the note);
probatio_saltuarius_columnae IMAGO case (exact cells + colours at the
centred position, separator intact). One fix on the way: an earlier
scene resized the pons to 40×8, so the section restores 60×12 first.

**Plants (compiling):** JPEG not recognized, size not stored (first
version didn't compile — redone), pane fitting neglected, not centred —
all four caught. saltuarius 13/13.

**Fran's look (2026-09-30): "that looks great".**

---

## Q8 — RELATIO (2026-09-30)

**What was built (Q1–Q7, rhubarb-secunda → main).** `quadrans`, a pure
library (no allocation, integer maths, no tessera dependency) that turns
an `Imago` region into terminal cells — block glyph + foreground +
background:
- QUADRANTES (2×2 sub-pixels, 16 glyphs; OpenTUI's most-different-pair
  split) and DIMIDIUM (▀, 1×2, exact colours);
- colourings MEDIA (group means, the default) and EXTREMA (OpenTUI's
  two extreme pixels);
- `quadrans_error` (exact integer MSE, hundredths) + an aurum RATCHET
  file of errors that may only fall;
- `quadrans_aptare` (fit without distortion; cell aspect a parameter,
  default 1:2, never queried).
Around it: tessera's colour policy (PLENI 24-bit / CCLVI 256-colour,
from the environment; quantized only at emission); `effigies`, the
viewer (m / c / p / ] / [; status line shows the error); the Aquinas
palette via Atkinson (`effigies_palettam_applicare`); saltuarius's image
preview (recognized by signature, computed at the disk edge, blitted
allocation-free).

**How it was checked.** Hand-computed synthetic blocks for all 16 masks
and every tie; the ratchet on two of Fran's photographs plus gradient,
checker and noise; byte goldens for 256-colour emission; headless cell
goldens through the memory pons (effigies, saltuarius columns); Fran's
looks in Ghostty and Terminal.app at Q4, Q5, Q6, Q7. Every rule shipped
with compiling plants caught by name; the mute ones (greys-only palette)
were recorded and the test strengthened.

**What the measurements and looks decided.**
- MEDIA ≈ 46% of EXTREMA's error on photographs (it can never be worse
  — same partition, least-squares colours). Default = MEDIA. Fran's
  eye: MEDIA best in Terminal.app; in Ghostty EXTREMA reads like a
  sharpening filter and may look better — recorded, NOT acted on.
- Same terminal grid: quadrants beat half-blocks 3.8× (painting) to 7.3×
  (bust). Horizontal detail matters more than exact colour.
- Terminal.app misreads 24-bit SGR; with the 256 path images show
  correctly (Q4 look). The colour policy is the width policy's twin.
- The palette mode is ~30× worse per pixel by design (dithering
  scatters colour for the eye); it pairs with EXTREMA, which keeps
  cells inside the palette.
- chafa needs `-f symbols` to compare like for like (it used Ghostty's
  graphics protocol by default).

**Answered from module 011's open questions:** extremes vs group means
(measured: means); aspect handling (the caller's pre-scale, now
`quadrans_aptare` in the library, parameterized). Still open: sextants
and braille modes; defaulting the colouring from the colour depth
(PLENI → EXTREMA?) — a possible follow-up from the Q5 look.

**Deferred, on purpose:** kitty graphics / sixel (feature 017 — this is
their query-free fallback), sextants, braille, animation/bandwidth
(~40 bytes of SGR per cell is fine for previews, poor for video), the
fenestra closed loop (module 010 does not exist yet).
