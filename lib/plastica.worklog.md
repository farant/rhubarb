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
