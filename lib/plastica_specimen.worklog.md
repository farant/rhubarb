# plastica_specimen worklog

## 2026-11-11 - T3: numbers and the view sheet

- The sheet uses the parallel renderer (`statua_scaenam_pingere`), not
  `statua_pingere`: the perspective renderer clamps tilt to 85 degrees,
  so a true top view is not possible there, and the world itself is
  drawn without perspective. One pixels-per-unit for all sixteen cells,
  fitted to the rest-pose bounding sphere with an 8 % margin each side;
  each view's origin puts the sphere centre in the middle of its cell.
- View directions, pinned by `pagina_aspectus`: front = the eye stands
  on +z; side (gyrus 90) shows the -x face with the model's front (+z)
  to the right; top is a map with +x right and the front (+z) at the
  bottom.
- Part colours: a shallow copy of the `Statua` with new node, mesh,
  range and material tables (24 flat materials appended: 12 one-sided,
  12 two-sided, so a two-sided part covers the same pixels as in the
  raw row). Each mesh node gets its own mesh in the copy, so a mesh
  shared between nodes (GLB models) still gets one colour per node.
  Invisible ranges keep their own material and stay unpainted; an
  invisible part still uses up its number in the colour cycle.
- "Invisible" = every range of the part has a material whose base alpha
  is below its cut limit. That is exactly what plastica writes for
  `invisibilis="verum"` (alpha 0, limit 0.5).
- Atkinson row: the cell is dithered whole, with uncovered pixels first
  set to the palette's paper colour so no error flows from the paper
  into the model; only covered pixels are copied to the sheet. The
  exemplaria row goes through `statuae_picturam_dithering` (mask =
  cover), the same path a character in the world takes.
- Footprint: the rule of `mundus_solum` restated (cell centre, topmost
  wins, equal height: non-walkable wins, vertical triangles skipped).
  One difference on purpose: a centre lying exactly on a triangle edge
  counts as inside (tolerance 1e-4 on the weights). A box's top face is
  split along a diagonal that runs through cell centres, and with
  strict `< 0` rounding could drop those cells from both triangles.
  `mundus_solum` still has the strict test; if a place build ever shows
  a missing diagonal of cells under a prop, that is where to look.
- The plan asked for a pinned digest of the sheet. Not done: vertex
  bytes differ between differently compiled machines (see
  plastica.worklog, fma contraction), so a digest would be red on
  another compiler for no fault. The tests pin determinism (two runs,
  same bytes), layout, scale and colours instead; the smoke script
  compares two runs of the tool with `cmp`.
- `plastica_specimen_pingere` does not take the part-name table (plan
  said it would): the picture needs only the part order; `bin/plastica`
  prints names beside colour names.
