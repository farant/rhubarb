# statua_pictura worklog

## 2026-11-09 - T4 of project-specs/statuae-plan.md

- **Framing is on the rest pose, not on all clips** (changes spec
  decision 15 and `statua_finire`, committed in T2). The golem's
  `boulder_throw` moves node `fx_boulder` 7.85 units from a model
  about 2 units tall; a sphere over every pose was five times the
  golem and the golem covered under 2 % of the picture. Now
  `Statua.centrum / radius` cover the rest pose only (golem radius
  1.475, mushroom 0.990), and whatever a clip moves outside leaves
  the picture. `STATUA_SPHAERAE_EXEMPLA` is gone.
- Consequences in the renderer: geometry may be anywhere, so (a) the
  depth buffer starts at 1e30, not at the far plane (a far boulder is
  still drawn while it is in view), and (b) near-plane clipping is
  live code, not a formality (the boulder can fly past the eye).
  Section "prope" forces both with a deliberately wrong tiny radius.
- **Fill rule.** Screen positions are snapped to 1/16 pixel and the
  edge functions are 64-bit integers, so two triangles sharing an
  edge agree exactly; a pixel centre exactly on an edge belongs to
  the triangle for which that edge is top or left. Proved on a
  square: the two halves alone sum to the whole (38 x 38 at 64 x 64),
  no pixel in both. A float version of the edge functions is not
  guaranteed to give that.
- **Camera sign conventions** (pinned by tests): positive `gyrus`
  turns the model's front toward the viewer's right; positive
  `inclinatio` puts the eye above (the model's top tips toward the
  viewer). The light is fixed to the view at (-0.5, 0.7, 0.5): a face
  straight on gets 0.68, a face turned to the left or tipped up gets
  more, a face turned away gets the 0.35 fill.
- Mirrored nodes (negative determinant) flip the facing test, as glTF
  requires.
- Vertex normals in the model are not used: one brightness per
  triangle from its own face normal.
- **Seen by eye** (scratch dump, not in the tree): golem lit and
  flat, mushroom lit, golem mid-throw at 360x450. Textures the right
  way up, cutouts cut, eyes glow, facets read.
- **Measured** (scratch program against the -O2 test archive, 200
  frames, 360x450, pose + render, fresh arena each frame): golem
  0.55-0.62 ms, mushroom 0.82 ms. Target was 5 ms. Matrices by value
  and the per-node channel scan are not worth touching.
- Plants: depth test off -> "profunditas"; alpha test off ->
  "excisio"; every face a front -> "tergum" (and "profunditas");
  diagonal edge always included -> "quadratum" (double paint).
  The same edge always excluded stays green, correctly: the other
  triangle owns that edge.
- Order note: the implementation was written before the test file
  this time; the plants are what show the tests bite.

## 2026-11-10 - scene entry: several placed models, parallel view (mundus-plan T3)

`statua_scaenam_pingere` shares the rasterizer with `statua_pingere`
through three switches in `Tabula`: `parallela` (projection without
the divide, no near-plane clip, back face judged by `normalis.z > 0`
instead of the eye-point dot product), `signum` (the byte written to
the cover map: 1 for the old entry, index + 1 for a placed model) and
the fixed-point origin. `statua_pingere` sets `signum = I`,
`parallela = FALSUM`; its two pinned digests did not move.

Why an integer origin and not a centre point: the world pixel of a
vertex is rounded to 1/16 pixel BEFORE the origin is subtracted
(`_proicere_parallele`, in f64), and the origin is a whole number of
pixels, so the fractional part of every vertex is the same in every
picture. Two pictures whose origins differ by whole pixels agree byte
for byte on shared pixels, depth included (test "scaena_lapsus"). A
float centre would give sub-pixel drift and a different raster every
frame. Depth is `-E(p).z` in world units with no translation, so
depths from separately rendered pictures (ground region, a body's
box) can be compared directly.

Test gotcha: hand-built models need `statua_finire` before
`statua_positura` (it builds `ordo`); without it positura reads a
NULL order and the suite dies with exit 139.

Measured (scratch `scaena.c`, real level 32,614 triangles, all
submitted, 45/35 degrees): 3.7 ms at 360x438 and 5.6 ms at 480x541
with 40 pixels per unit; 5.0 ms at 360x438 with 16. The golem alone
at 40 ppu in 128x128: 0.1 ms.

Plants, four red: origin scaled wrongly; depth inverted; back-face
test inverted; cover byte always 1.
