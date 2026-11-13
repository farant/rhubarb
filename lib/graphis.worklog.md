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

## 2026-11-13 - T2, lights (lib/graphis_lumen.c)

- All divisions in the light path are between non-negative numbers:
  the falloff interpolates as `T[i] - (T[i] - T[i+1]) * rest / 256`
  (the difference written the positive way round).
- The falloff table is 4096 x exp(-k / 8), k = 0..64; the index is
  4 x ((dx/rx)^2 + (dy/ry)^2), so a radius is one sigma and the light
  ends at four radii. At three radii a 100-grey still gets 1.
- `aedilis` build scripts are per dependency set: after a lib gains
  a new source (graphis -> graphis_lumen) a throwaway tool needs
  `bin/aedilis x.c` again, not just its old `struere.sh`.
- examen reads a C comment that begins with `<tag>` as an annotation
  ("annotatio malformata"); write "elementum lumen: ..." instead.
- The plant log's RUBRA lines list sections by FRACTA; a section that
  aborts on CREDO_NECESSE shows only in the RESULTATUM line.

## 2026-11-13 - T3, pieces (usus)

- The state struct already had `fons` (the source colour table);
  the piece callback is `fons_partium`.
- Every refusal copies `pars_currens` into `plagula` at the moment it
  is born, so the name survives the unwinding out of nested pieces.
- A plant must not leave a variable set but unused
  (`-Wunused-but-set-variable`): `x = prior` -> `x = bene ? titulus
  : prior`, not `x = titulus`.
