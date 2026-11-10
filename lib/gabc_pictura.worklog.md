# gabc_pictura worklog

## 2026-11-04 — P4: first implementation (gabc part 2)

**Copy, not extract, of the mask -> rectangles helper.** terminale.c's
`larvam_pingere` blends partial pixels against the background, which
needs theme colours resolved. Moving it into mandatum would make the
command-list library depend on thema — the wrong direction. So
gabc_pictura carries its own copy (+ a small channel blend); the
duplication is noted in the tabularium. If a third user appears, the
right home is a small `mandata_larva` library above mandatum and thema.

**Rubric red is RGBA, not COLOR_MANDATI_INDEX.** The plan said palette
red via COLOR_MANDATI_INDEX; rasterizers read INDEX as a syntax-colour
index (pictor_figurae.c warns about this), so rubrics are
`color_ex_palette(PALETTE_DARK_RED)` as RGBA.

**Measured:** the Salve's page 1 is 1539 commands; delineare_mandata on
them takes ~0.2 ms. No per-piece cache needed (threshold was 4 ms).

**GabcCompositio now carries its form** (`comp->forma`): drawing needs
spatium for the piece masks, and the pane will need page size.

**Looked at (neumae -scala 2):** Salve, 03441 Dies irae (porrectus),
03561 Te Deum (quilismas), 01568 Ingrediente (F clef). Fixes made from
the look, each in compositio/glyphae with tests re-pinned:
- syllables of one word a few px apart read as separate words: hyphen
  now drawn when the gap is >= half a cell (was a whole cell);
- clivis/porrectus stems used the virga's stem rule and ran a whole
  space below the lower head: now end s/2 + s/4 below the lower note
  (past the head, never on a line);
- ℟ ℣ were not in the font (drawn as substitutes): 6x8 pictures in
  glyphae_ductae; a run of rubric signs plus `. , :` is red ("℣.").
Open for Fran's taste: the F clef shape, the porrectus at small
intervals (third head overlaps the band), initial size.
