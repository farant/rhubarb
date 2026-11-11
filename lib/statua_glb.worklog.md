# statua_glb worklog

## 2026-11-09 - T3 of project-specs/statuae-plan.md

- **What PixMesh writes** (both fixtures, generator "PixMesh", glTF
  2.0): two chunks (JSON, BIN), one buffer, one embedded PNG atlas,
  one sampler (nearest, clamp), float positions / normals / texture
  coordinates, 16-bit indices, no byteStride, no extensions, every
  mesh used by exactly one node, nodes as translation / rotation /
  scale (never `matrix`), parents through `children` lists, one time
  accessor shared by all channels of a clip.
  - golem: 864 vertices, 1002 indices (334 triangles), 9 materials
    (3 cutout, 1 emissive), clips `walk` 1.4 s and `boulder_throw`
    1.583 s, 70 channels each.
  - mushroom: 1306 vertices, 1974 indices (658 triangles), 16
    materials, clip `walk_bouncy` 1.0 s, 54 channels.
  - `walk` and `walk_bouncy` close (last key == first on every
    channel). `boulder_throw` does not on 17 of 70 channels: a
    one-shot action that jumps when looped.
- **Numbers.** Every JSON number goes through `_numerus`, which takes
  integers and floats alike (`json_ad_fluitans` returns 0 for `2`).
  Section "numeri_integri" pins it.
- **The real buffer length is the BIN chunk's**, not
  `buffers[0].byteLength`. Each bufferView is checked against the
  chunk and each accessor against its view, in 64-bit arithmetic.
  This also lets the truncation test reach past the header: it patches
  the header and chunk lengths to the cut size so the reader walks
  into the damaged part.
- **Hostile sizes.** `piscina_allocare` is fatal on failure, so every
  count that sizes an allocation is capped first: 65 536 nodes /
  meshes / materials / parts / channels, 1 M vertices, 3 M indices,
  1 M keys, 64 images, 4096 px a side (read from the image header
  before decoding).
- **Non-finite floats are refused** in vertex positions, node
  transforms, material factors and animation keys (a NaN position
  would poison the bounding sphere and the renderer's clamps).
  Normals and texture coordinates that are not finite become 0.
- **glTF defaults that are easy to get wrong:** a texture without a
  sampler repeats (the fixtures say clamp explicitly); `emissive` is
  decided by a non-zero `emissiveFactor`, not by the presence of an
  emissive texture (the factor defaults to 0).
- **Scene.** Nodes outside the default scene keep their place in the
  tree but lose their mesh. A file with no `scenes` shows every node.
  A file with nothing to draw is refused ("nothing to show").
- **Tests assert the reason.** Each of the 25 refusal cases checks a
  fragment of the message, so a hand-built file that is broken in
  some other way cannot pass for the wrong reason. Plants: accessor
  bound loosened and vertex-index bound loosened both turn
  "recusationes" red through that check (the file is then refused
  later for a different reason, never read out of bounds).
- "corrupta": 300 single-byte changes in the BIN chunk per fixture
  (fixed linear generator), each parsed in its own arena under
  CREDO_NON_PENDET. Whole suite 1.2 s.
- Reads little-endian bytes by `memcpy` into `f32` (host is
  little-endian on every house target). A big-endian port needs a
  byte swap in `_partem_legere`.
- Glossary: `glb`, `accessor`, `normalis`, `emissivus`. The lint then
  reports `evanida 5` (words in the tolerated list that are now
  defined); the list wants regenerating at the next oratio pass.

## 2026-11-10 - part marks from extras.tags (mundus-plan T2)

`StatuaNodus.signa` (STATUA_SIGNUM_GRESSIBILE / SOLIDUM / AQUA) is
read from the node's `extras.tags`, an array of strings ("walkable",
"collidable", "water"), which is how the Retro Diffusion scene kit
marks its parts. `extras` is free-form in glTF, so nothing about it
can refuse a file: a non-object `extras`, a non-array `tags`, non-string
entries and unknown words all give 0. The kit also writes
`extras.collidable: true` beside the tags; it says nothing the tags do
not, and is not read. Test section "signa"; three plants red.
