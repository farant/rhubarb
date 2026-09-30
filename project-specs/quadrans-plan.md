# quadrans — plan (images into terminal cells: quadrant and half-block blitters)

*2026-09-30. terminal-planning features/006 (quadrant image blitting) and
modules/011 (quadrant blitter); the first brick of the "terminal as a
drawing target" arc (features/019 ludus/pictor in the terminal, 020 3D).
Worktree `../rhubarb-secunda`. Executed INLINE, one task per turn, Fran
approving each; tasks with a terminal step end with Fran's own look
before the commit. Names marked (unsealed) are working names; Fran
names. Decisions D1–D8 below are PROPOSED — Fran decides before Q1.*

## 1. Goal

Draw an RGBA image into terminal cells with no graphics protocol and no
terminal query: each cell is a block glyph plus one foreground and one
background colour. Quadrant mode (16 glyphs, 2×2 sub-pixels per cell)
doubles the resolution on both axes; half-block mode (▀ ▄, 1×2) trades
resolution for truer colour.

| today | after this plan |
|---|---|
| images cannot be shown in a tessera app | `quadrans` (pure) turns an image region into cells; a viewer and saltuarius show PNGs |
| tessera emits colour as 24-bit SGR only | tessera emits 24-bit OR 256-colour, chosen from the environment (no query) |
| Terminal.app garbles every 24-bit background (U5 look) | Terminal.app gets the 256-colour path and shows images correctly |
| "which fg/bg per cell" is an open opinion (module 011) | settled by a MEASURED error metric on a fixed image set |

## 2. What exists (read 2026-09-30, commit 0f16c9c9)

- **Images:** `include/imago_typus.h` — `Imago { i8* pixela; i32
  latitudo; i32 altitudo; }`, RGBA, 4 bytes per pixel, non-premultiplied,
  in a piscina. `include/imago.h` — `imago_caricare_ex_file`,
  `imago_mensuras_ex_file`. `include/imago_png.h` — decode/encode.
  `include/imago_opus.h` — `imago_scalare`, `imago_scalare_ad_limites`
  (keeps aspect), `imago_extrahere_et_scalare`, `imago_excidere`, with
  `ImagoScalaModus` PROXIMUS / BILINEARIS / AREA (AREA: alpha-weighted,
  for shrinking — what a blitter wants).
- **Dithering:** `include/dithering.h` — `dithering_atkinson_colorum`
  (RGBA → indices into the 16-colour Aquinas palette),
  `dithering_color_ex_indice`, `dithering_praeparare_omnes`.
- **tessera colour:** cells hold `0x00RRGGBB` or `TESSERA_COLOR_NATIVUS`;
  emission writes `;38;2;R;G;B` / `;48;2;R;G;B` only
  (`tessera/fontes/tessera_opus.c:94–108`). spec-v2 §6 defers "256-
  quantizing emit".
- **The Terminal.app finding** (`tessera/phase-log.md`, U5 look,
  2026-09-28): no 24-bit colour, and it MISREADS `48;2;R;G;B` (the
  trailing parameter turns text blue). A quadrant image has a 24-bit
  background on nearly every cell, so today it would be garbage there.
- **Width:** the block glyphs U+2580–U+259F have East_Asian_Width
  Ambiguous; `runae_latitudo` gives 1 (Ghostty's rule).
- **Reference algorithm:** OpenTUI
  `packages/three/src/shaders/supersampling.wgsl` (`renderQuadrantBlock`,
  `quadrantChars`, `sampleAlgo` 0/1) and
  `packages/native/src/buffer.zig` (`drawSuperSampleBuffer`,
  `materializeImageFallback`) — summarized in modules/011.
- **Reference look:** `chafa` 1.x at `/opt/homebrew/bin/chafa`.
- **Images in the repo:** 5 PNGs (`git ls-files '*.png'`): two icons
  (`probationes/fixa/icones/`), two pictor specimens, briar's app icon.
  No photograph.

## 3. Decisions (proposed; Fran decides)

**D1 — name and home.** A pure library `quadrans` (unsealed) in `lib/`:
depends on latina.h + `imago_typus.h` only, no allocation (the caller
provides the output array), returns codepoints + RGB, not tessera's
packed signum — so the same core can feed tessera, an emulator test,
or fenestra. The tessera adapter (codepoint → packed signum →
`tessera_cellulam_ponere`) lives with its consumers (the viewer, then
saltuarius), not in tessera: tessera stays "grid + input".

**D2 — the colours of a mixed cell.** OpenTUI takes fg/bg = the two
EXTREME pixels of the most-different pair; the alternative is the MEAN
of each group. Proposal: implement both behind a flag, measure the
reconstruction error (D7) on the image set (D6), make the winner the
default, and record the numbers. Uniform cells (mask 0 / 15) use the
block's mean either way.

**D3 — modes in v1.** QUADRANTES (2×2, 16 glyphs) and DIMIDIUM (▀/▄,
1×2 — two colours per cell, exact). Sextants (2×3, U+1FB00, uneven font
coverage) and braille (2×4, one colour) are later modes when something
pulls them.

**D4 — colour depth: THE decision.** Proposal: tessera gains a colour
policy exactly like the width policy (`TesseraPolitica`, runae U5c):
`TESSERA_COLORES_PLENI` (24-bit, default) and `TESSERA_COLORES_CCLVI`
(the xterm 6×6×6 cube + 24 greys, nearest by squared RGB), chosen from
the ENVIRONMENT, never by query: `COLORTERM=truecolor|24bit` → full;
`TERM_PROGRAM=Apple_Terminal` → 256; otherwise full (the thesis: wrong
colours, never broken layout). Quantization happens at emission, so
cells keep true RGB and goldens stay exact. This also fixes
spectaculum's colours in Terminal.app, and it undefers a named spec-v2
§6 item — it belongs to tessera, not to `quadrans`. Separate, optional:
a PALETTE mode in the viewer (Atkinson dithering to the 16 Aquinas
colours before blitting) for a deliberate rhubarb look.

**D5 — aspect.** No query for cell size (the thesis). Assume a cell is
1:2 (w:h), so a quadrant sub-pixel is 1:2 too (tall). The viewer
pre-scales the source with `IMAGO_SCALA_AREA` so the image keeps its
proportions in sub-pixel space; the ratio is a parameter, not a
constant, in case a font disagrees. `quadrans` itself only samples.

**D6 — the image set.** Synthetic images built in the test (every mask,
gradients, checkerboards, alpha ramps) + the 5 committed PNGs + ONE or
TWO photographs, checked in with provenance like the UCD files (D3 of
the runae plan). Needs Fran: either his own photos, or a public-domain
image fetched once (a network fetch — ask first).

**D7 — the oracle.** (a) The 16 masks and their tie-breaking, derived
from OpenTUI's code, as exact cases. (b) An OBJECTIVE error metric:
reconstruct each cell as its 2-colour sub-pixel pattern and take the
mean squared RGB error against the (pre-scaled) source — per image,
committed as aurum numbers so a change that makes pictures worse goes
red. (c) chafa as a VISUAL reference in the terminal looks only (a
different algorithm; not a byte oracle).

**D8 — consumers.** A viewer instrumentum next to folium (unsealed
name, e.g. `pictura`): loads PNGs, fits them to the terminal, switches
mode (quadrants / half / palette) and image, shows the error figure.
Then saltuarius: a preview of `.png` files in the miller-column
preview pane.

## 4. Tasks (inline, one per turn; red first; plants caught by name)

**Q1 — the pure core.** `quadrans_computare` (unsealed) over a region →
caller's cell array. All 16 masks from synthetic 2×2 images (exact
glyph, fg, bg); ties (equal distances, equal luminance, all four
identical) deterministic and documented; odd widths/heights padded, never
read out of bounds; alpha composited against a given background.
Plants: pair search wrong, luminance swapped, mask bit order.

**Q2 — the error metric and D2.** `quadrans_error` (unsealed) + the
extreme-vs-mean measurement on the image set; winner becomes default;
aurum error numbers committed.

**Q3 — half-block mode (D3).**

**Q4 — tessera colour policy (D4).** Red first: goldens for the 256
path (bytes, in tessera's own suite: `38;5;n`); the quantizer against
hand-checked cube/grey points; `tessera_colores_ambitus` from the
environment; amalgam + saltuarius. Terminal look: spectaculum in
Terminal.app (the colour bar should finally show).

**Q5 — the viewer (D5, D8).** Headless goldens through the memory pons
(cells of a small image); then Fran's look in Ghostty and Terminal.app,
next to `chafa` on the same image.

**Q6 — palette mode (D4, optional).** Atkinson → Aquinas colours →
blit; look.

**Q7 — saltuarius preview (D8).** Goldens + look.

**Q8 — RELATIO** (phase log, terminal-planning features/006 +
modules/011 status, spec-v2 §6), merge main in, fast-forward.

## 5. Names to seal (Fran)

The library (`quadrans`) and its functions; the modes (QUADRANTES /
DIMIDIUM); tessera's colour policy type and values; the viewer's name.
Every name checked against latina.h's reserved words before use.

## 6. AUDIENDA (not verified)

- That Terminal.app renders `38;5;n` / `48;5;n` correctly (widely
  reported; the Q4 look measures it).
- Block-glyph rendering: Ghostty draws box and block elements itself
  (exact cells); Terminal.app uses the font, so seams between cells or
  uneven quadrants are possible. The Q5 look says which.
- Cell aspect 1:2 is an assumption; fonts vary (hence a parameter).
- Emission bandwidth (~40 bytes of SGR per cell per the module note) is
  not measured here; fine for previews, poor for video — measure in Q5.
- chafa's symbol-selection flags for a like-for-like comparison were not
  checked.
- Ambiguous-width block glyphs in a CJK-locale terminal (drawn 2 wide)
  are out of scope; recorded, not handled.
