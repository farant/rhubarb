# plastica_textura worklog

## 2026-11-11 - T4: generators, UVs, hand-written textures

- **Texel density measured, not guessed.** A throwaway script read
  `probationes/fixa/statuae/planum.glb`: for every triangle edge of
  every textured primitive, texture-space length (UV difference times
  the atlas size) over world length. All 47 materials sit at 16.0
  texels per unit (stragglers at 15.x are sheared atlas quads). The
  mushroom man is 32 in his own units, but characters are scaled on
  the way in, so the level is the reference.
  `PLASTICA_TEXELLA_PER_UNITATEM` = 16.
- **The kit clamps, we repeat.** The kit packs every face into an
  atlas with clamp-to-edge sampling. We keep one small tile per
  (generator, seed, colours) and repeat it (`repetit_s/t`, already
  in `StatuaMateria` and the renderer).
- **Three colours exactly.** Every pixel is one of dark, mid, light.
  The plan said "between the three"; exact is stricter, keeps the
  grain hard and gives the dither only three inputs.
- **Integer only.** No float anywhere in the generators, so the nine
  digests are pinned in the test (seed 3, 32 x 32) and hold on every
  machine - unlike the sheet, whose vertex bytes vary by compiler.
  Changing a generator changes every asset that uses it; the pinned
  digest makes that a deliberate act.
- **Tiling at any size.** Cells are laid by `x * n / width` with n a
  whole number of cells, and every neighbour lookup wraps, so edges
  meet at any size (not only multiples of the cell). `lateres` forces
  an even number of courses or the half-brick offset would seam.
  `saxum` is an integer Voronoi with wrapped distances; a crack is a
  pixel whose right or lower neighbour belongs to another stone.
- **`i32` is unsigned here.** All offsets are written as
  `(x + size - 1) % size`, distances as `a > b ? a - b : b - a`.

### UVs (in `lib/plastica.c`)

- One rule in `_triangulum`, computed BEFORE the mirror so a mirror
  image carries a mirrored texture: s runs along the face's
  horizontal (rightwards seen from outside, `up x normal`), t runs
  down the slope (`normal x right` is up; t = 0 is the top row).
  Flat-lying faces use s = x, t = z. Square texels on every triangle
  by construction; the test checks every edge of every triangle of
  every solid, mirrored and turned.
- **The anchor.** A solid may set a point whose (s, t) it dictates
  (`_ancoram_ponere`); the face's coordinates are counted from
  there. That is all a curved surface needs for a continuous run:
  cylinder facet k anchors its lower right corner at s = -k * chord;
  the lathe accumulates t along the profile and flips the sign of s
  for inward-facing walls (travelling down); the extrusion
  accumulates s along the outline. Flat-lying faces ignore the
  anchor.
- **One seam, except where the surface narrows.** A cone cannot be
  unrolled without stretching, so tapered facets keep square texels
  and seam against their neighbours on the narrow ring. The anchor
  sits on the wider ring (a pole has no width). Header says so.
- **Copies carry the same patch.** UVs are in the solid's own space,
  so the ten stones of the well's course all show the same piece of
  the tile. It does not read as repetition at this size; if it ever
  does, the remedy is a per-copy seed, not world-space UVs (which
  would swim when a prop is placed elsewhere).

### In the description

- `semen` is shared with the sphere's roughness seed: one solid, one
  seed.
- `colores` absent: mid = `color`, dark = 3/4 of it, light = a
  quarter of the way to white (integer). The material's colour
  factor becomes white so the texture is not tinted twice; alpha is
  kept, so `invisibilis` still works with a texture.
- Textures are shared by key (generator or hand index, seed, nine
  colour bytes). The table is filled in pass 0 and found again in
  pass 1; at most 64 per asset.
- Hand-written `<textura>`: a direct child of `<res>`, defined
  before use, letters a..p index `colores`, whitespace means
  nothing, rows are cut by `latitudo`. It makes no node. A comment
  that begins with `<textura ...>` is taken by examen for a
  malformed annotation - do not start a C comment with a tag.

### Looking at them

`bin/plastica texturae -o x.png`: all nine, tiled 2 x 2, raw /
exemplaria / Atkinson. Raw and Atkinson read well. Exemplaria on a
full flat field goes to strong palette colours (bark turns red and
green, brick purple); that is the method on large even areas, the
same thing seen on the well's sheet, not a texture fault. Props join
the place, which is dithered as ground, so Atkinson is the row that
matters for `$world`.

Plants: see the T4 commit.
