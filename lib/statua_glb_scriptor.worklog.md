# statua_glb_scriptor worklog

## 2026-11-12 - plastica T5: the GLB writer

- **What "equal" had to mean.** The reader drops meshes outside the
  default scene, decodes every image to RGBA and numbers textures in
  order of first use by a material. So the test is read, write, read
  again, and the second read is compared with the first, field by
  field (`_statuae_pares` in the test). It also checks that writing
  the second read gives the same bytes as writing the first.
- **Vertex ranges.** `Statua` keeps one vertex table with absolute
  indices; glTF wants each primitive's own accessors. A part's range
  cannot simply be min..max of its indices: a file may carry vertices
  at the edge of an accessor that no triangle uses, and dropping them
  would shift every later index. When parts are in order (each starts
  above the previous one's highest index, which the reader and
  plastica always give) part k runs from the end of part k-1 to its
  own highest index and the last runs to the end of the table - the
  whole table and every absolute index come back untouched. Parts
  that share vertices fall back to min..max each (duplicated
  vertices, correct picture; test `communes`).
- **The reader had to change by one rule.** `statua_glb_legere`
  normalised every node rotation. Normalising an already-unit
  quaternion can move the last digit, so read-write-read drifted and
  the second write differed in bytes (fungus, lapis). The reader now
  leaves a rotation alone when its squared length is within 1e-6 of
  one (`_longitudo_una`). All statua and mundus suites still pass.
- **Clip times are shared.** Channels of one clip with identical key
  times use one time accessor. Without it the mushroom man grew from
  106864 to 119496 bytes; with it he is 106776.
- **Sizes (spec AUDIENDA 5).** Rewritten beside original:
  `planum.glb` 3399516 -> 3318480 bytes; `fungus.glb` 106864 ->
  106776; `lapis.glb` 196476 -> 187984. The level's twelve atlases
  stay PNG and dominate; nothing was JPEG.
- **Floats.** JSON numbers go through `json_fluitans_creare`, which
  prints the shortest text that reads back to the same f64, so an
  f32 survives exactly. Binary tables are written byte by byte,
  little-endian.
- **Not kept** because `Statua` has no place for it: material names,
  the emissive colour (white is written), JPEG as JPEG. Node names
  are written when the caller passes a table (plastica's `partes`);
  our reader ignores them.
- **Refuses** (empty chorda): NIHIL arguments, no nodes, a number
  that is not finite, a part with no triangles, a mesh with no
  parts. glTF forbids empty accessors and empty primitive lists.
- `MMMM` is not a numeral in latina.h: `(IV * M)`.

Plants: see the T5 commit.
