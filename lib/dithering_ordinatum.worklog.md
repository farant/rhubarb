# dithering_ordinatum worklog

## 2026-11-09 - Bayer and nearest (statuae-plan T5)

Two functions without error diffusion, for the statuae pane's dither
dropdown: `dithering_proximus_colorum` ("None") and
`dithering_bayer_colorum`.

- **Bayer went through three rules.** Judged on rendered frames of
  the golem and the mushroom man, 360x450.
  1. *Nearest plus the colour nearest the mirrored point* (the plan's
     rule). Wrong by construction: with two active colours and a pixel
     a quarter of the way between them, the mirrored point is the
     midpoint, the tie goes back to the nearest colour, and nothing is
     mixed. The quarter-tone test caught it.
  2. *Nearest plus the partner whose two-colour mix is closest.*
     Correct portions, but garish: on a 16-colour palette the best
     RGB partner is often a far, saturated colour at a small ratio
     (pink speckle across the red cap and the stone). Restricting
     partners to the 2-3 nearest colours, or penalising mixes by
     t(1-t)D^2, helped a little; two colours are simply not enough.
  3. *Sixteen candidates per colour* (kept; Yliluoma's ordered
     dithering with an arbitrary palette). Candidate k is the nearest
     colour to the pixel plus the accumulated error of candidates
     0..k-1, so the candidates' average approximates the pixel the
     way error diffusion would, but inside one pixel. Sort by
     luminance, pick `candidati[bayer / 4]`. Colour is now close to
     Atkinson's with an ordered pattern.
- **The plan depends only on the colour**, so the call keeps a small
  open-addressing table (4096 slots, at most half full) from RGB to
  plan. Nothing survives the call. A rendered frame has few distinct
  colours: 0.34-0.40 ms at 360x450, against 23 ms for the same rule
  with 8 candidates and no table. A photo overflows the table and
  pays 16 nearest-colour searches per remaining pixel: the 600x800
  meadow takes 164 ms (Atkinson 10 ms). Fine for the pane; a photo
  caller would want the plan table keyed coarser or built once.
- Sixteen candidates give 17 mixing levels between two colours (the
  pair rule had 64). Halves and quarters are exact (tests).
- Ties in "nearest" go to the lower palette index (first strict
  minimum), as in Atkinson. With no active colour all three dithers
  return `successus` FALSUM.
- Measured, 360x450 rendered frame, -O2: Bayer 0.4 ms, nearest
  1.8 ms, Atkinson 3.2 ms.
- Digests in `probatio_dithering_ordinatum.c` are pinned from this
  implementation on `fixa/exemplaria/lapis_600.png`, whose colours
  overflow the table, so the no-table path is pinned too.
- **Own module, not `dithering.h`.** The first commit attempt put
  both functions in `lib/dithering.c` and included
  `dithering_exemplaria_fontes.h` there for the Bayer matrix. The
  `tessera` gate then failed 0/15 at link: tessera links `dithering.o`
  without the exemplaria sources (`_DITHERING_BAYER` undefined). The
  base dither file must not depend on the exemplaria module, so the
  functions moved to `dithering_ordinatum.{h,c}` and `dithering.{h,c}`
  are byte-identical to before. Lesson: a new `#include` in an old
  library file can break subsystem runners that list sources by hand;
  the radix suite does not show it.
