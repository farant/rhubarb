# graphis.c worklog

## 2026-11-13 - T1, flat shapes

- `stml_attributum_capere` gives NIHIL for `titulus=""` (an empty
  value reads as absent). To refuse an empty title the code scans the
  tag's own attribute list (`_attributum_scriptum`).
- Whitespace-tolerant plants (`silva.planta`, tolerans 'spatia') see
  `p->domini[locus] = (i8)q->dominus;` twice (fill and outline differ
  only in alignment). Use a multi-line anchor that ends in the next
  distinct line.
- Division of negatives is implementation-defined in C89:
  `_pavimentum` / `_tectum` do floor and ceiling by hand; the polygon
  scan compares crossings as fractions in s64 (no floats anywhere).
- An untitled overlay (`colores="a -"`) does not change the owner of
  the pixels it paints; a titled one owns its set bits only.
- "Paints no pixel inside the picture" doubles as the test for a shape
  wholly outside the frame and for a polygon too thin to cover any
  pixel centre.
