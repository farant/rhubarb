# dithering_exemplaria.worklog.md

## 2026-10-13 - origin: from scratchpad experiments to library

Written fresh from the experiments of the same day (scratchpad
exemplaria1..9.c, ten generations, each logged in the ledger as a note
under desideratum ...GKGNJT8QM; Fran's choice of generation VIII is
decretum ...DVAB). The ledger notes carry the reasoning for every
constant at the top of the .c - read them before retuning. Short form:

- Eye = Gaussian in LINEAR light, compared in OKLab. A 3x3 box eye
  favoured diagonal patterns (its variance artefact, not perception).
- Base = Bayer chosen per 2x2 area, patterns anchored to the global
  grid. Per 8x8 block was a mosaic at ~512 px.
- Shaped patterns must NOT be chosen by an error optimizer: it puts
  bricks in skies. Pictograms carry meaning, so they come from the
  `materia` table (palette colour -> family), one per REGION (paint
  bucket), calmest regions first, under a budget (Fran: 30%).
- Regions = nearest colour + lightness band 0.06, so a big shadow splits
  into medium regions instead of one flat blob. The pair must contain the
  region's own colour; in lights the second colour pays for hue drift
  (white robe went gold without it), in shadows (L < 0.35) low-contrast
  pairs with a chroma penalty.
- Curved hatching = oriented Gabor filter iterated over noise (the
  fingerprint-enhancement trick) along the sigma-4 structure-tensor
  field, mid tones only, largest regions first under a budget (15%), so
  small bits (hands) stay clean.
- Glitter (isoluminant third-colour specks) was tried and dropped: Fran,
  "the flat patterns read better without it".

## 2026-10-13 - spray: subtract the region mean (behaviour change vs v8)

The port's flat-red test failed: with spray on, half the brick mortar
vanished. Cause: a pictogram pair needs visible contrast, so the
patterned region's mean tone differs from the flat source; the spray
read that whole-region offset as "lost shading" and erased the pattern
to compensate - the same dissolving Fran saw at pressure 3 in the
experiments. The spray exists to restore shading VARIATION; the
region's flat tone is deliberate. Now the per-region mean of the
difference is subtracted before the wide blur. Effect on the demo
photos: spray share of region pixels 9-18% (v8) -> 2.5-5%, patterns
cleaner, band edges still softened (the within-band variation remains).

## 2026-10-13 - budget semantics: a region larger than the budget is skipped

Both budgets take whole regions in order and skip any region that would
overshoot; they never fill a region partially. So a single region bigger
than the budget (a flat image; a sky that is one band) gets no
treatment at all. The tests set the budget to 1.0 (pictograms) or 0.5
(hatching) where they need one big region treated. If this bites on real
images, the options are: split large regions (more bands, or a size
cap) or allow a partial last region - decide with Fran, do not change
silently.

## 2026-10-13 - memory

Roughly a dozen n*3 f64 arrays live in the caller's piscina (the
structure tensor allocates its own scratch, twice). At 512x512 that is
~80 MB; at 1536x2048 it would be ~900 MB. The header says callers
downscale to ~300-700 px; if a caller ever needs full-size images,
reuse the scratch arrays instead of allocating per stage.
