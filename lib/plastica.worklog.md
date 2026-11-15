# plastica worklog

## 2026-11-11 - T1: the compiler, boxes and groups

Plan `project-specs/plastica-plan.md` T1, spec `plastica-spec.md` IV.1.

- **Two walks over the STML tree.** Walk 0 validates and counts
  (nodes, solids, vertices, indices); walk 1 fills the `Statua`
  arrays. Every refusal is born in walk 0, so walk 1 can only fail on
  an exhausted arena. A refused description never yields a half-made
  model: `statua` and `partes` are NIHIL.
- **Shape of the model.** `<res>` is node 0 (no mesh). Each `grex` is
  a node with no mesh; each solid is a node with one mesh, one part
  and one material of its own. Materials are not shared between
  solids (simple; revisit if a prop ever has hundreds of parts).
- **`locus` is the centre of the base.** The box's vertices are local
  with the base centre at the origin (y from 0 to height), and the
  node carries `locus` as its translation. So `gyrus` and `rotatio`
  turn a solid about its base centre.
- **Rotation order.** `gyrus` (about +y) is applied first, then
  `rotatio` about x, then y, then z. Conventions match
  `stereometria_rotatio_x/_y`: +y goes toward +z, +x goes toward -z.
  Quaternion helpers are local (stereometria has no axis-angle
  constructor).
- **Winding.** Each box face is given by a normal and two tangents
  with u x v = normal; corners (-u -v) (+u -v) (+u +v) (-u +v) are
  counter-clockwise seen from outside. The test checks the cross
  product of every triangle against its stored normal.
- **Marks.** `signum="gressibile"` sets only
  `STATUA_SIGNUM_GRESSIBILE` (the level's walkable parts also carry
  the solid mark, but `mundus_solum` lets walkable win, so one bit
  is enough).
- **Numbers.** Space-separated, finite, within +-1e6; `nan` and `inf`
  are refused (strtod accepts them).
- **Colours** are stored as byte / 255 with no gamma step; the
  renderer multiplies texel bytes by the factor directly.
- **Lint:** `sedecimalis` was flagged as a new word; the helper is
  `_digitus_hex`. `./silva/renominare.sh` refuses an untracked file,
  so the rename was by hand.

Plants (all red, file restored): face winding flipped (capsa); base
centred instead of on the ground (5 sections); unknown attribute let
through (recusationes); gyrus sign flipped (grex, rotatio).

## 2026-11-11 - T2: the other solids and the repeaters

Plan T2. Solids `cylindrus`, `sphaera`, `cuneus`, `extrusio`,
`tornus`; groups `speculum`, `series`, `sparsio`.

- **One emitter for every solid.** `_triangulum(p, a, b, c)` counts
  in walk 0 and writes in walk 1: three own vertices with the face
  normal (flat shading), reflected by the current mirror state, and
  skipped when it has no area. Both walks compute the whole
  geometry, so the counts always agree. The box moved onto it too
  (36 vertices instead of 24; nothing depended on 24).
- **FMA bit me.** A cone's top cap and the pole triangles of a
  sphere have two identical corners; their cross product is exactly
  0 on paper. clang on arm64 contracts `a*b - c*d` into an fma, and
  the result is the rounding residue of `c*d` (about 1e-8), so an
  absolute threshold of 1e-10 let them through with a garbage
  normal (cone: 16 triangles instead of 12). The test is now
  RELATIVE: area <= 1e-5 x the product of the two edge lengths.
  Consequence worth knowing: vertex bytes can differ between
  machines that contract differently. "Same source, same bytes"
  holds per build, and the random draws (integers only) are the
  same everywhere.
- **Mirror is a true reflection, not a negative scale.** The state
  (`speculum_x`, `speculum_z`, each 1 or -1) reflects every node
  transform below it as M T M (translation reflected, quaternion
  conjugated: x-mirror negates q.y and q.z; z-mirror negates q.x
  and q.y) and every vertex as M v, with the winding swapped when
  an odd number of mirrors is active. The chain
  (M T1 M)(M T2 M)(M v) = M T1 T2 v gives the image; at the
  `speculum` node the children run twice, once with the state
  toggled, which gives M_s T M_a Tc v. Nested mirrors compose.
- **Lathe facing.** The quad order is the cylinder side's order
  (lower_k, upper_k, upper_k+1, lower_k+1), which faces outward
  when the profile travels up; travelling down the same order faces
  the axis. So "the surface faces the right of travel" needs no
  case analysis. Ends are closed only when the profile touches
  r = 0 (those quads collapse to triangles through the area test).
- **Sphere.** Bands = max(2, segments / 2). Pole rows are forced to
  sin = 0 exactly and to segment index 0, so the roughness value at
  a pole is one value. Roughness is a hash of (seed, band, segment),
  so every triangle that touches a shared vertex gets the same
  radius: the test counts 14 distinct positions with and without
  roughness.
- **Extrusion.** The outline is put in counter-clockwise order seen
  from above (negative signed area in x,z). Caps by ear clipping;
  a collinear vertex is dropped without a triangle. Refused:
  repeated point, self-crossing outline (proper intersection of
  non-adjacent edges), no area.
- **Scatter draws** are four hashes per copy (x, z, turn, scale),
  always all four, so copy k stays where it is when the varying
  attributes are switched on or off.
- **Limits.** 4096 nodes, 200000 triangles, 64 points, 64 sides, 32
  segments, 256 copies.
- **Lint.** `ordinarius` is the `default` macro (compile error as a
  parameter name): `praesumptus`. Glossary entries added for
  `extrusio` and `sparsio` (element names). Renamed `titulatus`,
  `hx`, `hz`, `_introrsa`, `summa_xz`.

Plants (all red, file restored): top cap flipped; mirror without
the winding swap; hash ignoring the seed (sphaera, sparsio); wedge
side flipped; outline order not normalised; lathe quad broken;
series step not growing; node limit lifted.

## 2026-11-12 - T10: the first assets (`plastica/res`)

Ten assets: well, fence section, boulder, rock, pine, oak, bush,
crate, signpost, stair. What modelling them taught:

- **A thin thing needs an unseen blocker.** Fence rails, the pine
  and oak trunks and the signpost are thinner than two walk cells,
  so each carries an invisible 0.5-wide solid box. The crown of a
  tree stays unmarked: you walk under it.
- **UVs count from the solid's own centre.** A box 1 unit wide shows
  a 16-texel tile shifted by half (u runs -8 .. 8). The signpost's
  arrow is therefore written with its rows rotated by 8 texels; the
  comment in `index_viae.stml` says so. Vertically the tile is
  aligned (the base is at y = 0).
- **Frames as geometry, not texture.** The crate's dark frame is
  twelve thin boxes (two nested `speculum`s), not a texture: it
  catches light and needs no alignment.
- **A cap on a rock reads as a hole** when it is darker than the
  rock under dithering. The boulder's moss is lighter and flatter
  than first drawn.
- **Rocks sink a little** (asperity moves the lowest vertex below
  y = 0, about 0.08). On grass that reads as a rock sitting in the
  ground; left as is.
- **The stair is measured, not guessed.** The level has strata at
  y 0.5, 4.5 and 8.5; the cliff under the top plateau runs straight
  along z = 12 (slope cells at z 11.875 and 12.125). Eight steps of
  0.5 rise and 0.5 depth, 3 wide, all marked walkable; the top step
  is 1.0 deep so it covers the cliff's edge. Placed with its foot
  at z 15.75 and turned 90 degrees it joins 4.5 to 8.5 exactly.
- Dithered, dark foliage and bark lose most of their texture; the
  silhouette carries the pine and the oak. Stone, thatch and planks
  keep theirs.

## 2026-11-12 - a seed added from outside (plastica II, task 2)

`plastica_legere_seminis(textus, semen, piscina)` adds `semen` to
every seed the description reads (`_semen`: sphere roughness and
texture, which share one attribute, and `sparsio`). Seed 0 is
`plastica_legere`, so nothing that existed changes and the texture
digests stand. The test compares "description read with 3 added"
against "the same description written with every seed 3 higher".
A pine has no seeded geometry, so a seed only changes its texture
patches; rocks, bushes and oaks change shape.

## 2026-11-12 - description conveniences (plastica II, task 3)

Four additions that came out of modelling the first ten assets:

- **`motus_texturae="u v"`** on a solid adds that many texels to the
  texture coordinates of every face. UVs count from the solid's
  centre, so a 16-texel tile on a 1-unit box used to need its rows
  written rotated by half; the signpost now writes its arrow the
  natural way with `motus_texturae="8 0"`. The offset is not part of
  the texture's key: shifted and unshifted solids share one texture.
- **`series` steps.** `magnitudo_gressus="x y z"` adds k times that
  to the size of each copy's children (`magnitudo` of capsa, cuneus,
  sphaera; the y part to `altitudo` of cylindrus and extrusio). The
  stair's seven rising steps are one line. `semen_gressus="n"` adds
  k * n to the children's seeds. `motus_texturae_gressus="u v"`
  shifts each copy's texture by k * (u v) texels. All three are
  saved and restored around the loop, and a nested series adds to
  its outer copy's values.
- **`obex="x z"` on `<res>`** appends an unseen solid box at the
  origin (one number = square), 1 unit tall, part title "obex",
  always the last node. It replaces the hand-written invisible box
  in the fence, pine, oak and signpost. To emit it without a source
  element, `_solidum_aperire` was split (`_solidum_inire` takes the
  properties) and the box's six faces moved to `_capsam_emittere`.
- **Found while converting the assets: a seed per copy is the wrong
  tool for "each stone shows a different patch".** With
  `semen_gressus` each of the well's twenty stones got its own
  32 x 32 generated texture. Together with the stair and the varied
  props the furnished clearing passed the GLB reader's cap of 64
  images, and `bin/mundus condere` refused to store it ("the file
  has too many images") - its read-back check did its job; the map
  command and the pinned test, which never wrote a GLB, did not
  notice. Hence `motus_texturae_gressus` (not in the header first
  shown to Fran): another patch of the SAME tile, no new texture.
  The well and the stair use it. The reader's cap was raised from
  64 to 256, and test `pratum` now writes the place to GLB, reads it
  back and holds the texture count under 100 (it is 64 today).
- Every converted asset kept its numbers (parts, triangles, bounds,
  footprint) and the clearing's pins did not move.

## 2026-11-14 - joints (plastica motus T1)

`<figura>` is a second root and `<articulus>` a named group that makes
one node (project-specs/plastica-motus-spec.md IV.1). Notes:

- "Joint not allowed here" is one field, `claustrum`, set in the
  child dispatcher (`_liberi`) from a flag in the element table and
  restored after the handler returns. The four group handlers were
  not touched; series and scatter call `_liberi` several times and
  would have needed the save/restore on every path.
- The reader runs twice (count, then fill), so the joint table is
  rebuilt in each pass and `articulorum` must be zeroed in `_radix`.
  Without that the second pass sees every title as a duplicate.
- STML returns an empty attribute (`titulus=""`) as absent, so the
  joint's title check is `!titulus` alone. A plant that allowed an
  empty title stayed green until the redundant length test was
  removed.
- Motion elements (`<positura>`, `<cyclus>`, `<membrum>`) are passed
  over only as direct children of the `<figura>` root; anywhere else
  they are unknown elements. plastica_motus (T2) reads them.
- A comment beginning `/* <res>:` in a test is taken for a malformed
  annotation by examen (domesticum). Reworded.
