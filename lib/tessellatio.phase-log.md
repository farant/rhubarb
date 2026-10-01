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
