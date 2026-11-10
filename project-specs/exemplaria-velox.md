# exemplaria velox — pattern dithering fast enough for animation

Status: DONE 2026-11-07 (secunda, commits 1b6670c04 .. T8). Fast is
3.1-3.4x classic, output bit-identical since T3; the 12 fps target at
600x751 is missed (185 ms, 5.4 fps) and met at 360x451 (76 ms) - see
VIII. Brainstormed 2026-11-06 with Fran (secunda session 134c8417)
while designing the 3D model pane.

*The question (Fran, 2026-11-06): the 3D model pane should dither every
frame like pictor does. Atkinson keeps up easily; exemplaria (the
MacPaint-pattern mode, `lib/dithering_exemplaria.c`) took 0.6 s per
pane-sized frame. "I want to at least be able to try it out." Fran's
performance questions (data-oriented design, flat arrays and indices,
no caches, SoA, inline-capacity lists, bitsets) are the design rules.*

## I. Intent

1. **Exemplaria per frame, live.** At pane size (600x751) the fast
   version must reach at least 12 frames per second (<= 83 ms per frame
   for the dither alone); 24 fps (<= 42 ms) is the stretch goal. Whether
   it then *looks* good in motion is the 3D pane's experiment, not this
   project's promise.
2. **The same look, by eye.** Same five layers, same parameters, same
   material table, same palette. Output may differ from today's pixel
   for pixel; Fran judges by comparing PNGs side by side.
3. **Compared outside pictor first.** Classic and fast are compared in
   the harness PNGs; once Fran trusts the fast one it replaces
   exemplaria as pictor paste's B (a one-line swap, later, not in this
   project).

Non-goals: byte-identical output to classic; reusing work between frames;
threads or processes; SIMD intrinsics (plain C89 the compiler can
vectorize is fine); changing the look on purpose; touching classic.

Anti-goals: caches - no memoization across calls, no frame-to-frame
reuse ("caching is cope for a slow design"); a fast version that only
wins on the golem and loses on photos; a second API shape callers must
learn.

## II. Decisions (Fran, 2026-11-06)

1. Drift from classic is allowed; before/after PNGs on fixed images are
   the judge.
2. Classic stays intact (`dithering_exemplaria_colorum`): its OUTPUT
   does not change. The fast version is a separate function and file.
3. The design follows the data-oriented rules: structure of arrays,
   32-bit floats, bitsets for masks, flat arrays with indices, constant
   tables built at build time, nothing cached between calls.

Builder's calls (Claude, 2026-11-06, after the builder check):

4. Shared pattern data. The pattern sources (`BAYER`, `PICTOGRAMMATA`,
   the default material table) move out of classic's .c into one shared
   source that classic, the fast version and the table generator all
   read - so an edited pattern changes both. "Classic unchanged" means
   identical output, proved by the harness: classic's indices on every
   fixture are pinned (hash) before the move and must match after.
5. The speed target holds at 600x751 with default options (all 16
   colours, pictogram budget 0.30, hatching 0.15) on the golem fixture -
   the closest thing to a rendered low-poly frame. Every fixture's time
   is reported; photos must be at least as much faster as the golem is,
   or the plan says why not. No per-option timing columns.
6. Fixtures (Fran, 2026-11-06): the stone golem (`~/Downloads/
   stone-golem.png`) and the meadow photo (`~/Downloads/test-image.png`),
   each scaled to 600 wide and committed under `probationes/fixa/`.
7. pictor's paste stays two-way in this project (Fran, 2026-11-06): its
   scratch mask, confirm and status line all assume exactly A/B, so a
   third mode is a redesign of paste, not a line.

## III. Baseline (measured 2026-11-06)

Spike in the scratchpad (`mensura.c`, not in the tree), clang -O2,
stone-golem.png scaled with `sips`, average of 10 calls:

| size    | Atkinson | exemplaria |
|---------|----------|------------|
| 360x451 | 4.4 ms   | 231 ms     |
| 600x751 | 9.4 ms   | 606 ms     |
| 720x902 | 13.4 ms  | 862 ms     |
| meadow 600x800 | 10.9 ms | 762 ms |

Moving the whole image one pixel changed 17-22 % of exemplaria's indices
(the Bayer grid is screen-anchored; regions are recomputed); Atkinson 0 %
on the golem (a pure translation; real rotation will differ); on the
meadow photo 35 % (Atkinson) and 53 % (exemplaria).

Profile (`sample`, 600x751, -O2 -fno-inline, share of samples):

| share | where | why |
|-------|-------|-----|
| 22 % | `_basis_bayer` (:541) | per 2x2 area: up to 12 candidates, ~130 ordered pairs x 16 Bayer levels x 4 pixels of `_d2` against `_mixtio`; early exit per pair only |
| 18 % | `_gauss` (:280) | 5 calls per image, f64, edge clamp in the innermost loop, interleaved channels |
| 16 % | `_proximi` (:221) | full insertion sort of 16 colours; 3x per area in the base, once per pixel in `_residuum` (:691) and in the pictogram titles (:1052) |
| 10 % + 3 % | `pow`, OKLab | sRGB->linear and three cube roots per conversion, all via `pow` (lib/color.c:184, :222) |
| 9 % | `_d2` (:163) | not inlined |
| ~6 % | `_campum_striarum` (:826) | 8 iterations of a 13x13 orientated filter over hatched pixels |

Data layout today: every per-pixel buffer is interleaved f64 triples
(`lin`, `lab`, `lab_oculi`, `lab_oculi_lati`, `error`, `tmp`: 24 bytes x
450 k pixels = 11 MB each), `Structura` is an array of 3-f64 structs, the
masks (`hachuratus`, `activus`, `eligibilis`, `larva`, `servatus`) are
`b32` per pixel (1.8 MB each). Constant data is rebuilt on every call:
the Bayer and pictogram weight tables (`_exemplar_finire`, ~600 k `exp`)
and the 16x16x33 `mixtio` table (~25 k OKLab conversions, 200 KB f64).

## IV. Design

**API.** `dithering_exemplaria_velox_colorum` with exactly classic's
signature (rgba, latitudo, altitudo, colores_activi[XVI], optiones,
census, piscina) and the same `DitheringExemplariaOptiones` /
`DitheringExemplariaCensus` / `DitheringFructus`, in
`include/dithering_exemplaria_velox.h` + `lib/dithering_exemplaria_velox.c`.
Callers switch by name only.

**Data.**
- Per-pixel planes, f32, one plane per channel: lin r/g/b, L/a/b for the
  source and for each "eye", structure energy/coherence/angle. A loop
  touches only the planes it reads (the base reads L of the eye for its
  min/max - one plane, not three).
- Masks as bitsets (one bit per pixel, `i32` words); the active-colour
  set as one 16-bit mask inside.
- Palette indices stay `i8` planes. Region labels stay flat `s32` arrays
  with offsets (already index-based).
- Constant tables generated at build time into a GENERATUM header: the
  256-entry sRGB->linear table, the Bayer level patterns and both weight
  tables, the pictogram weight tables. Built by a small generator, judged
  by `tools/generata_iudicare.sh` like every generated file.
- The `mixtio` table depends on the active colours, so it is still built
  per call - in f32, only for active pairs, with the cheaper cube root.

**Algorithms (same maths, cheaper steps).**
- sRGB->linear by table; OKLab cube root by a float approximation
  refined with Newton steps (no `pow`).
- Gaussian blur per plane, f32, with the image split into interior (no
  clamping) and edge strips; kernel weights computed once per call.
- k-nearest colours by partial selection (ties broken by index, as the
  insertion sort does); the single nearest by a plain minimum.
- Base layer: candidate pairs as packed keys (a, b, level fit in 12
  bits); a pair is skipped as soon as its contrast penalty alone exceeds
  the best error; the weight lookup per pixel position (x&7, y&7) is a
  direct table index.
- Hatching field and residual layer: same plane/bitset treatment; the
  residual's nearest-colour search uses the plain minimum.

## V. Measuring and comparing

A credo suite, `probatio_dithering_exemplaria_velox`, on fixed images
committed under `probationes/fixa/`:
- correctness: output only uses active colours; deterministic (same
  input + options -> same indices, twice); handles 1x1 and odd sizes;
  census sane (coverages within their budgets).
- timing: prints ms per frame for classic and fast at each size (no
  timing assertions - machines differ).
- comparison: writes `build/exemplaria_velox/<imago>_classica.png` and
  `<imago>_velox.png` for each fixture, plus the share of pixels that
  differ, for Fran to look at.

## VI. pictor

Not in this project (decision 7). When Fran trusts the fast version,
`PICTOR_GLUTINATIO_EXEMPLARIA` in lib/pictor_documentum.c:2701 calls it
instead of classic.

## VII. Rough task order (the plan file will fix it)

1. Harness: the suite + fixtures, with the fast function as a straight
   copy of classic (identical output, timings equal). Baseline PNGs.
2. Data layout: SoA f32 planes, bitsets, sRGB table, cheap cube root.
3. Constant tables at build time (generator + GENERATUM header).
4. Blur.
5. Base layer: k-nearest, packed pairs, pruning.
6. Hatching field, pictogram titles, residual.
7. Docs; the timing table and PNGs for Fran's verdict.

Each task reports the timing table and new PNGs.

## AUDIENDA

- The 12 fps target is a guess at what reads as animation for low-poly
  pixel art; the 3D pane may need more.
- Speed-up estimates (7-15x) are not measured; the profile shares were
  taken with inlining off, which inflates small functions.
- Shimmer in motion is not addressed here and may make exemplaria
  unusable for animation regardless of speed - that is the pane's
  experiment.

## VIII. Result (2026-11-07)

Final run, `tools/exemplaria_mensura -n 30`, ms per frame (dither
only), default options, all 16 colours:

| image  | size    | Atkinson | classic | fast | speed-up | arena c/v MB |
|--------|---------|----------|---------|------|----------|--------------|
| golem  | 360x451 | 3.5      | 226     | 76   | 3.0x     | 89/29        |
| golem  | 600x751 | 9.3      | 581     | 185  | 3.1x     | 247/80       |
| golem  | 720x901 | 13.9     | 840     | 258  | 3.3x     | 356/115      |
| meadow | 600x800 | 10.1     | 663     | 193  | 3.4x     | 260/82       |

**Verdict.**
- **Target (I.1, <= 83 ms at 600x751) missed:** 185 ms = 5.4 fps. At
  360x451 the fast version runs at 76 ms (13 fps), over 12 fps. Fran
  (2026-11-07) is open to a lower render resolution for the 3D pane ("a
  short hike" style: render small, show at 2x as chunky pixels); about
  300x375 should land near 45-50 ms (~20 fps). That, not more speed at
  600x751, is the planned way to try exemplaria in motion.
- **Photos at least as much faster as the golem (II.5): met** (meadow
  3.4x, golem 3.1x).
- **Same look (I.2):** drift from classic is 2 px (golem) and 14 px
  (meadow) of ~450 000. It comes only from T3 (f32 planes,
  approximate cube root); every later task is exact and pinned by
  blake2b digests ("velox fixa", "magnitudines oris", "hachurae fixae",
  "tecta fixa"). Side by side: `build/exemplaria_velox/*_par.png`
  (classic left, fast right) - Fran's look verdict pending.
- Classic is unchanged ("classica fixa" pinned since T1).

**Where the time went, by task** (golem 600 ms): T1 copy 590 -> T3
f32/bitsets 517 -> T4 blur 488 -> T5 nearest colours + exact pruning
328 -> T6 hatching field 324 -> T7 review 179 (~185 at N 30). T7 came
from a fresh-eyes review (a read-only subagent): skip base areas that
hatching/pictograms paint over, a capsule lower bound per colour pair
(all mixes of a pair lie within rho of the segment a -> b; distance to a
capsule is convex, so the area mean bounds every level), a branch-free
first-term sweep, the horizontal blur row by row, the hatching field 4
pixels at once.

**Where the time goes now** (no-inline profile): base 37%, residual 17%,
hatching field 10%, pictograms 7%, structure tensors 7%, aspersio 6%.
The base is mostly fixed per-area work now (three nearest-colour
searches, 36 capsule distances, the first-term sweep per surviving
pair).

**Departures from IV.**
- Packed pair keys dropped: holding the best choice as `a | b << 4 |
  level << 8` made the base 60% slower (T5, bisected); three separate
  variables avoid it.
- Pruning went well beyond "penalty exceeds best" (which fired on <1%
  of pairs): the capsule bound skips 74-82% of pairs.

**Tried and dropped** (so nobody retries them blind; details in
`lib/dithering_exemplaria_velox.worklog.md`):
- In-frame dedupe of identical base blocks: 38-40% of golem blocks
  repeat bit-for-bit (0% on the photo), but the gain was 3-4% on the
  golem and a 7% loss on the meadow - the repeats are flat background,
  already the cheapest blocks. Also leans on the no-memoization rule.
- Seeding the best-so-far with a likely pair and level: exact, cut the
  levels walked by 16-34%, no time change - the remaining base cost is
  fixed per-area work the seed doesn't touch.
- A plain triangle ("ball") bound per pair: -9%, superseded by the
  capsule bound.

**Left** (each ~1-3%, all exact; tabularium wish): the hatching angle
only where used; share the two structure tensors' gradients; no
div/mod in the region code; the residual skips both square roots when
the nearest colour is the base colour; aspersio's OKLab only under the
mask; branch-free nearest-colour ranking. Bigger levers are outside the
spec: threads/processes (non-goal) or a GPU path.

**pictor (VI)** is unchanged; swapping paste's B to the fast version is
a tabularium wish, to do when Fran trusts the look.

