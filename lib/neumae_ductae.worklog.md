# neumae_ductae worklog

## 2026-11-04 — P2: first implementation (gabc part 2)

**Head height is spatium - 1, not spatium.** The plan's sketch said
"punctum at VIII is 8x8, origo (0,4)". With an even spatium an 8-row head
cannot be centred on a row, so a note on a line would sit one pixel low
and a note in a space would cover one staff line but not the other.
Height s-1 (odd) centres on the origin row: a note in a space exactly
fills the gap between the two lines, a note on a line is symmetric about
it. Width stays s. All head pieces (punctum, inclinatum, stropha,
oriscus, quilisma, cavum, linea_punctum) share this height and origin
row; the test's stacking law pins it.

**Two drawing modes, one legend.** Rectangles fill solid. Slanted or
curved outlines (inclinatum, porrectus band, stropha's cut corner,
oriscus wave, quilisma teeth, flat belly, virgula, r1-r5 signs) are a
polygon or a thick polyline sampled 4x4 per pixel, coverage rounded to
quarters: 0, 0x40, 0x80, 0xC0, 0xFF — the same five values as
glyphae_ductae's legend, so every mask is expressible as an ASCII
picture. Overlapping figures combine by max.

**Measure and paint share one function** (`partem_ducere`, painter
optional) so metrics can never disagree with the drawing.

**No stem piece.** The plan listed NEUMAE_CAULIS; the approved header
dropped it — ligature stems and connecting lines are rectangles placed
by gabc_compositio. Only the virga carries its own stem (parametrum =
length in steps below the centre, II..VI; inked rows below the origin
row == n * spatium/2, a law the test checks for every n and spatium).

**By-eye shapes.** Rectangle-built pieces were derived by hand from the
header's rules and passed first time. Curved ones were rendered, judged
at spatium VIII and XXIV, and pinned. The oriscus wave needed deeper
control points (first try read as a plain square at XXIV). The F clef
(head with a stem down on the left of a C clef) is provisional — P4
compares it with Gregorio's at real size.

**NEUMAE_LARVA_MAXIMA = 128*128.** Largest real mask: porrectus V at
spatium XLVIII, 96 x 167 = 16032. The leges section checks every piece
at every even spatium against the constant, with guard bytes.

**Plants** (silva.planta, gate radix filtrum neumae_ductae, all red then
green): porrectus band one step short (section II); virga stem on the
left (I); guard byte written at XLVIII (IV); virga stem one pixel short
(I). `tolerans='spatia'` needed: the formatter wraps long call lines,
so verbatim anchors miss.
