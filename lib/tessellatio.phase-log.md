# tessellatio phase-log

Plan: `project-specs/tessellatio-plan.md`. D1–D8 approved by Fran on
2026-10-01 ("that looks good to me"), with the working names
`tessellatio` and `Modulus`. This is the terminal as a render target for
`Mandata` (terminal-planning modules/012), plus Fran's drawing-layer
metric.

## T1 — the metric and text measurement (2026-10-01)

**INTENTIO (D1, D4).** A small pure library, `include/modulus.h` +
`lib/modulus.c`. It is its own library, not part of tessellatio,
because both adapters need it: the native one will declare its metric
in T2.

- **`Modulus`** holds:
  - the cell size in our pixels;
  - the surface extent;
  - the screen ratio as a rational pair per axis (`ModulusProportio`),
    reserved and 1/1 for now.
- **Constructor.** `modulus_creare` clamps a cell ≤ 0 to 1 and an extent
  < 0 to 0 (degrade, never break).
- **Conversions:**
  - `modulus_columna` / `modulus_linea`: FLOOR (a primitive's
    position, D5);
  - `modulus_columna_proxima` / `modulus_linea_proxima`: the NEAREST
    cell edge, half rounding up (filled rectangles and clip, D5),
    computed as floor((2x + c) / 2c) so it's exact for odd cell sizes
    too;
  - `modulus_extensio_cellularum`.
- **`modulus_textum_metiri`** gives the size in our pixels: widest line
  × cell width, and lines × cell height. Lines = 1 + the number of
  `\n`; empty text is 0×0. The width rule is the TARGET's
  (`ModulusMensor`):
  - FONTIS counts one cell per `utf8_decodere` step. That is the same
    decoder `fenestra_textus.c` draws with, invalid bytes included
    (TOFU, one byte), so it measures what fenestra draws by
    construction.
  - RUNARUM uses `runae_latitudo_textus` under the width policy.

**C89 detail:** the direction of division of a negative number is
implementation-defined in C89 (C99 fixed it to truncation). Negative
coordinates come from translation and clip, so `_pavimentum` never
divides a negative number. Plant D below shows the difference on a real
compiler: clang truncates, so `-1 / 6 = 0` where the floor is −1.

**Red first:** the stub returned zeros, and the cases failed by name.
**Green:** `probatio_modulus` covers:
- clamping;
- floor at 0 / 5 / 6 / 11 / −1 / −6 / −7;
- nearest at the half (3 → 1, −3 → 0) and with an odd cell;
- 640×480 → 106×60 at 6×8, 80×30 at 8×16;
- text: empty; "Ok" (12×8; 16×16 at 8×16); multi-line; a trailing
  newline;
- **中 = 6 px natively, 12 px in the terminal** (D4's divergence, pinned);
- e + U+0301 = 2 cells natively, 1 unit;
- an invalid byte = 1 on both;
- 👨‍👩 = 2 cells (GRAPHEMATUM) vs 4 (SIMPLEX);
- the widest line wins under RUNARUM.

**Plants (all compiling, 0 errors, each caught by name):**
- A: floor ↔ nearest swapped (the floor cases);
- B: the native rule using runae widths (中 and the combining mark);
- C: the line step using the cell width (every height);
- D: naive `/` division (exactly the negative cases).

No new words for the Latin lint. The three files are CONFORMIS after
`formator -scribere`. `compile_tests_fontes_generata.sh` was regenerated
(+ lib/modulus.c).

**Side find (merge hygiene, not T1):** after main's a0535064 declared 42
generated artifacts, every commissio in this worktree failed fabrica's
pre-check with IGNOTUM (no manifest under build/aedilis). Running the
SANATIO lines (`natura_struere`, `canon_struere`, then the three
regenerators) fixed it, and git was unchanged. Recorded in the secunda
memory.

## T2 — the native adapter declares its metric (2026-10-01)

**INTENTIO (D1: "the target declares").** The font header `fons_6x8.h`
stays the ONE source of the numbers. It is regenerated wholesale by
`tools/fons_merge.c` (`./fons_merge > include/fons_6x8.h`, defines
included), so moving the constants elsewhere would make two sources of
truth after the next merge. It also carries a static glyph table, so it
can't be included cheaply. Instead:
- fenestra's text drawer declares its cell:
  `tabula_pixelorum_cellula_textus(&lat, &alt)`, in `fenestra.h` next
  to the text functions, implemented in `fenestra_textus.c`, which
  includes the font;
- `delineare_mandata_modulus(tabula)` returns that cell plus the table
  as the extent;
- `delineare_mandata_mensor()` returns FONTIS.

`delineare_mandata.h` now includes `modulus.h`, so every program that
links the rasterizer also links modulus + runae (aedilis resolves this;
pictor builds).

**Red first** (the stubs gave cell 0 → 1 and the RUNARUM rule).
**Green:**
- direct values: 6×8, extent 64×48, FONTIS.
- **A property tying measurement to drawing.** Each sample text is drawn
  through the real adapter, then measured with the native metric:
  - zero ink outside the measured box;
  - the box is TIGHT: there is ink in its last cell column and its last
    line.

  The samples: "Ok", "Hg|", 中 (drawn as TOFU), e + U+0301, a two-line
  text, an invalid byte, "été".
- **The same containment for every drawable glyph** (printable ASCII +
  Latin-1, 0x21–0xFF).
- The `mandata_prima` image golden, byte-identical.

**FINDING: two glyphs overflow their cell.** `>` (U+003E) and `}`
(U+007D) put their tip in column 6 (bit 0x02). That pixel falls in the
first column of the NEXT character's cell, so `>H` can touch. This is
original HP 100LX data (the same in `fons_6x8.h.backup`). Every other
glyph stays within 6 columns, which confirms the advance empirically.
The test NAMES the two exceptions. Any new overflow fails, and so does
an exception that stops overflowing (a font fix would update it).
Whether to redraw them is Fran's call (open question).

**Magic numbers replaced** (the plan's scope): `layout.c` (two copies of
the click → character conversion) and `importatio_visus.c` now call
`tabula_pixelorum_cellula_textus`.

**NOT swept**, recorded instead (2026-10-01 inventory): the same 6/8
cell constants remain in:
- `widget.c:246` (plus a bare `/ (VIII)` at 248);
- `pagina.c:176,325`;
- `libro_paginarum.c:522`;
- `navigator_entitatum.c:2405`;
- `pinacotheca_visus.c:156,284,438`;
- `calendario_visus.c:202` (plus `s32` copies at 399/1300);
- `schirmata.c:200,1000,1481`.

They mix `i32`/`s32`, inline initializers, and bare literals, so a regex
can't tell a cell-size VI from any other 6. A complete sweep means
reading each file. A partial one would look finished. These all belong
to the older widget generation; a lint ("no literal cell metric outside
the drawer") would be the durable fix (cf. features/021's lint idea).

**Plants (compiling, 0 errors):**
- A: the drawer declares 8 (REDDENDI). Caught by the direct value, the
  tightness check, and the named-exceptions check: with an 8-wide box
  the exceptions stop overflowing, which also fails.
- B: the drawer ADVANCES by 8. Caught by containment and the image
  golden.
- C: the native mensor = RUNARUM. Caught by the rule check, 中's
  tightness (TOFU fills only the first of the two measured cells), and
  e + U+0301's containment (fenestra draws 2 cells, runae measures 1).

Root suite 177/177. Formatting: the five touched files were written
with `formator -scribere` before the final test runs. `layout.c` and
`importatio_visus.c` reflowed wholesale (they were far from conformant;
the pre-commit hook formats whole files anyway). No new Latin words.
