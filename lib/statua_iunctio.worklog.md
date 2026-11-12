# statua_iunctio worklog

## 2026-11-12 - plastica T8: the joiner

- **Placement is a position and a turn**, not the plan's matrix. A
  node stores translation, rotation and scale; a matrix would have
  to be decomposed again, with rounding. The layout (T9) only has a
  position and a turn about the upright axis anyway. The turn uses
  the same sense as `gyrus` in a plastica description: the test
  joins a box and compares its world bounds with a plastica `grex`
  given the same `locus` and `gyrus`.
- **The base is copied first, indices untouched**, so its clips
  still address its nodes. Each added model gets one new root node
  (parent -1) and its own nodes after it; a node's parent is shifted
  by the root's index + 1, its mesh by where that model's meshes
  start.
- **Stored once.** The same `Statua*` added several times brings
  its meshes, parts, vertices, materials and textures once; only the
  nodes repeat. Identity is the pointer, which is what the layout
  build has (one compiled prop, many placements).
- **Identical textures are kept once** (same size, same pixels),
  across base and added models. The place is written as one GLB
  with a PNG per texture, so this is what keeps a furnished place
  from carrying the stone tile once per prop. The base's own
  textures are not compared with each other: its indices must not
  move.
- **Borrowed memory.** The result owns its tables but points at the
  inputs' texture pixels and clip keys. Fine for the build tool
  (everything lives in one arena until the GLB is written).
- Clips of added models are dropped: a place has no use for a
  prop's animation yet, and merging clips would need a naming rule.

Plants: see the T8 commit.
