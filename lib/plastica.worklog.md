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
