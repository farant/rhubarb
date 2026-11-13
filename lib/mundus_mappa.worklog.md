# mundus_mappa worklog

## 2026-11-12 - the walk map as a picture (plastica II, task 1)

Why: furnishing the clearing (plastica T10) needed a throwaway
program to see where things could go. That program is now two
commands and a small library.

- `mundus_mappa_pingere` draws a `MundusSolum` at 4 pixels per cell
  with a 16-pixel margin: walkable ground green, lighter with
  height (the same formula the throwaway used, so the three strata
  of the clearing read dark green / light green / yellow), water
  blue, solid dark grey, nothing white. Cells the character cannot
  reach are pink when a reachability table is given.
- Grid lines every 4 units (heavier every 16) by darkening the
  pixel, so the cell colour still shows through. Coordinates every
  8 units in the margins, written straight into the pixels from the
  6 x 8 font table; a label at the far edge is pulled back inside
  the image.
- A note is a 5 x 5 black dot with a white centre and its number on
  a white tab, so it reads over any colour. Notes outside the table
  are skipped.
- `mundus_solum_attingere` (in `mundus_solum`) is the flood fill
  that two tests and the throwaway each carried: four neighbours,
  every step checked with `mundus_solum_licet` between cell centres.
  It takes an arena for its stack (the header shown to Fran had no
  arena; a fill over 65 thousand cells needs one). The two tests
  (`planum` in probatio_mundus_solum, `pratum` in
  probatio_mundus_locus) now call it and their pins did not move.

### Tool

- `bin/mundus mappa <layout> -o <png> [-nudus]` prints walkable
  cells, reachable cells per stratum and a legend (number, asset,
  spot, layout line). The printed numbers are the ones test
  `pratum` pins, so updating the pins after a layout change is
  copying a line.
- `bin/mundus quadrum <layout> -o <png> -locus "x z" ...` renders
  up to 12 frames, three across, with the mushroom man at each
  spot. A spot where he cannot stand is named on stderr and its
  tile stays blank; if he can stand nowhere, exit 1 and no file.
- Strata are split at heights 3 and 7 (constants in the tool); that
  fits the clearing only.

Plants: see the commit.
