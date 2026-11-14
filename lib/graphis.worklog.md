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

## 2026-11-13 - T4, sheet and bin/graphis

- Hand plants on a tool: sleep BEFORE writing the mutated source as
  well as after. The second of three plants ran against the first
  plant's binary (the smoke's first failure line was the first
  plant's); rerun alone with a sleep on both sides it was caught
  properly.
- `argv` is compacted in place: flags are consumed, file arguments
  move down to index 2, so `iudicare` can take any number of files
  without a second array.

## 2026-11-13 - T5, the hut and titled groups

- A region of one titled shape is not enough for real pictures: any
  untitled shape painted over it takes its pixels (fire, pot, window
  bars, blanket). `<grex titulus>` gives the whole group one owner
  (`dominus_gregis`); `_titulum_legere` hands that owner to every
  untitled shape inside, including shapes of pieces.
- The room shell is a piece (`casa_camera`) so both views agree by
  construction; the test compares one ceiling row of both.
- View two has the fire behind the viewer: a wide point light placed
  below the frame (`locus="180 270" radii="210 150"`) lights the
  floor and fades up the back wall.

## 2026-11-14 — wider picture: 480 x 320, older sheets scaled 4/3

`GRAPHIS_LATITUDO/ALTITUDO` are now 480 x 320. A root with
`latitudo="360" altitudo="240"` marks an older sheet; `_augere`
scales its coordinates by 480/360 (integer, round half up, floor
fixed for negatives). No attributes = plain pixels.

- Scale the ABSOLUTE coordinate, after `motus_x/y`: scaling a group
  offset and a child separately rounds twice and shapes in different
  groups drift a pixel apart (test: group (1,1) + shape (1,1) lands
  on pixel 3, not 2).
- `fines` are scaled as two edges, not as x and width: 100 + 100
  must end at 267, not 266, or neighbours show a seam.
- Stroke 1 stays 1 (1.33), 3 becomes 4, 16 becomes 21. No minimum
  clamp is needed: the smallest written stroke is 1.
- Light softness is scaled and capped at 16 because
  `graphis_illustrare` refuses more.
- Patterns are not scaled, so on a scaled sheet they are finer
  relative to the shapes. Checked by eye on both hut pictures
  (`bin/graphis specimen`): fine.
- Tests that had the canvas size baked in were moved to LAT/ALT or
  to the new numbers; the three "fixed" digest sheets were marked as
  older sheets so the scaled path has digests too.
