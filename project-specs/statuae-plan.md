# statuae — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.

**Goal:** a vicus pane (`$models`) that plays Fran's low-poly GLB
models, rotates them by drag, and dithers every frame with a
switchable dither.

**Architecture:** five pure libraries stacked bottom-up (space math ->
model + pose -> GLB reader -> renderer -> dithers), a store that keeps
the original bytes in the vicus volume, a command-line tool over the
store and the renderer, a small shared dropdown member over the
existing popup menu, and the pane that ties them together inside its
own tick.

**Tech stack:** C89 + latina.h; house libraries only (`piscina`,
`chorda`, `xar`, `json`, `imago`, `volumen`, `dithering*`, `elenchus`,
`componens`, `mandatum`, `organum`, `credo`).

**Spec:** `project-specs/statuae-spec.md` (approved by Fran
2026-11-09). Read it first; this plan argues from it.

## Global constraints

- Latin identifiers; `VOCABULA_VIAE_ADDITAE=... ./oratio/vocabula.sh
  -nova` shows NOVA 0 (add glossary lemmata where a word is new).
  latina.h macros and Roman numerals are never identifiers.
- New C files through `./silva/scribe.sh`; every file `./silva/examen.sh`
  ACCIPE and `./silva/formator.sh -scribere` CONFORMIS; 72 columns.
- `i32` is unsigned, `s32` signed; `chorda` is not NUL-terminated;
  `-Wconversion -Wsign-conversion -Werror`.
- All floating point in these libraries is `f32`; `<math.h>` only
  through `sqrt`, `sin`, `cos`, `tan`, `acos`, `floor` with explicit
  casts.
- JSON numbers: glTF writes `1` and `1.0` interchangeably and
  `json_ad_fluitans` returns 0 for an integer. Every numeric read in
  the reader goes through one helper that accepts both.
- Memory: arenas only. No allocation per frame outside a frame arena
  that the pane empties each frame.
- TDD. Each gate is proved red by a compiling plant (`silva.planta`,
  porta `radix`, the suite as `filtrum`) before it counts.
- Tests write only under `credo_area(c)` and close temporary volumes.
- Raw file IO only through `filum_*` (examen 96/97).
- Nothing writes Fran's live volume. Tests use `volumen_temporarium`.
- Headers carry `<aedilis corpus="lib/....c"/>` so the build finds
  the body.

## Review focus

Inputs the spec implies and a user will meet; each has its test in the
task named.

1. A GLB whose numbers are integers where floats are expected (scale
   `[1,1,1]`, key time `0`) loads with the right values. (T3)
2. A stored model the reader refuses does not take the pane down: the
   title and reason show, arrows still work. (T8)
3. A model entirely behind or crossing the near plane, or a
   zero-area triangle, renders without a crash or a division by
   zero. (T4)
4. The volume gains a model while the pane shows "no models": the
   pane leaves the empty state on the next poll. (T8)
5. A pane resized to zero or one pixel in either direction, or `2x`
   on an odd size, draws nothing rather than reading out of bounds.
   (T8)

## Files

| file | holds |
|---|---|
| `include/stereometria.h`, `lib/stereometria.c` | f32 vectors, matrices, quaternions |
| `include/statua.h`, `lib/statua.c` | the model value, pose, bounds |
| `include/statua_glb.h`, `lib/statua_glb.c` | GLB bytes -> model |
| `include/statua_pictura.h`, `lib/statua_pictura.c` | renderer |
| `include/dithering_ordinatum.h`, `lib/dithering_ordinatum.c` | Bayer, nearest |
| `include/statuae_repositorium.h`, `lib/statuae_repositorium.c` | store |
| `include/statuae_quadrum.h`, `lib/statuae_quadrum.c` | one frame: pose, render, dither, paper (added in T6; tool and pane share it) |
| `tools/statuae.c`, `tools/statuae_struere.sh`, `tools/statuae_fumus.sh`, `aedificatio.stml` | `bin/statuae` and its hand-run smoke script |
| `include/membrum__delectus.h`, `lib/membrum__delectus.c` | dropdown member |
| `include/statuae.h`, `lib/statuae.c`, `apps/statuae/organum.stml` | the pane |
| `lib/vicus_applicatio.c` | genus + `$models` |
| `probationes/fixa/statuae/lapis.glb`, `fungus.glb` | fixtures |
| `probationes/probatio_<lib>.c` per library; `probatio_vicus_statuae.c` | tests |
| `lib/<file>.worklog.md` | finds, as they happen |

---

### Task 1: stereometria — space math

**Files:** create `include/stereometria.h`, `lib/stereometria.c`,
`probationes/probatio_stereometria.c`.

**Produces:**

```c
nomen structura { f32 x; f32 y; f32 z; } StereometriaVector;
nomen structura { f32 x; f32 y; f32 z; f32 w; } StereometriaQuaternio;
/* columnae primum: e[col * IV + ordo] */
nomen structura { f32 e[XVI]; } StereometriaMatrix;

StereometriaVector stereometria_vector (f32 x, f32 y, f32 z);
StereometriaVector stereometria_addere (StereometriaVector a, StereometriaVector b);
StereometriaVector stereometria_subtrahere (StereometriaVector a, StereometriaVector b);
StereometriaVector stereometria_scalare (StereometriaVector a, f32 s);
f32           stereometria_productum (StereometriaVector a, StereometriaVector b);
StereometriaVector stereometria_crux (StereometriaVector a, StereometriaVector b);
f32           stereometria_longitudo (StereometriaVector a);
/* vector nullus manet nullus */
StereometriaVector stereometria_normalizare (StereometriaVector a);
StereometriaVector stereometria_miscere (StereometriaVector a, StereometriaVector b, f32 t);

StereometriaQuaternio stereometria_quaternio_normalizare (StereometriaQuaternio q);
/* via brevior (signum inversum si productum < 0); prope parallelos
 * miscet et normalizat */
StereometriaQuaternio stereometria_quaternio_slerp (StereometriaQuaternio a,
    StereometriaQuaternio b, f32 t);

StereometriaMatrix stereometria_identitas (vacuum);
/* a * b: b primum applicatur */
StereometriaMatrix stereometria_multiplicare (StereometriaMatrix a, StereometriaMatrix b);
StereometriaMatrix stereometria_ex_trs (StereometriaVector translatio,
    StereometriaQuaternio rotatio, StereometriaVector scala);
StereometriaMatrix stereometria_translatio (StereometriaVector t);
StereometriaMatrix stereometria_rotatio_x (f32 radiani);
StereometriaMatrix stereometria_rotatio_y (f32 radiani);
/* dextera manu, -z ante oculum, profunditas [-1, 1] */
StereometriaMatrix stereometria_perspectiva (f32 campus_y, f32 ratio, f32 prope,
    f32 procul);
/* punctum (w = 1); *w = w exitus */
StereometriaVector stereometria_punctum_transformare (StereometriaMatrix m,
    StereometriaVector p, f32* w);
/* directio (w = 0), sine translatione */
StereometriaVector stereometria_directionem_transformare (StereometriaMatrix m,
    StereometriaVector d);
```

- [ ] Write `probatio_stereometria.c` (tolerance 1e-5):
  - "crux": x cross y = z.
  - "rotatio y": `stereometria_rotatio_y(pi/2)` sends (1,0,0) to (0,0,-1);
    `rotatio_x(pi/2)` sends (0,1,0) to (0,0,1).
  - "trs": `ex_trs((1,2,3), identity, (2,2,2))` sends (1,1,1) to
    (3,4,5); equals translatio * scale by multiplication.
  - "multiplicare": translatio(1,0,0) * rotatio_y(pi/2) applied to
    (1,0,0) gives (1,0,-1) (rotation first).
  - "slerp": end points returned at t 0 and 1; halfway between
    identity and 90 degrees about y is 45 degrees about y; slerp(q, -q)
    stays q (shorter path).
  - "perspectiva": a point at z = -prope maps to depth -1 after
    dividing by w, at -procul to +1.
  - "nullus": `normalizare((0,0,0))` is (0,0,0).
- [ ] Run `./compile_tests.sh probatio_stereometria`: fails to link.
- [ ] Implement; rerun: all green.
- [ ] Plant: in `stereometria_multiplicare` swap the operand order; "trs"
  or "multiplicare" must go red. Restore, `cmp`.
- [ ] examen, formator, vocabula lint.

### Task 2: statua — the model and its pose

**Files:** create `include/statua.h`, `lib/statua.c`,
`probationes/probatio_statua.c`.

**Consumes:** Task 1. **Produces:**

```c
nomen enumeratio {
    STATUA_TRANSLATIO = ZEPHYRUM, STATUA_ROTATIO,
    STATUA_SCALA
} StatuaProprietas;

nomen structura {
                     s32  nodus;
    StatuaProprietas  proprietas;
                     b32  gradatim;     /* STEP; aliter linearis */
                     i32  clavium;
                     f32* tempora;      /* crescentia */
                     f32* valores;      /* III aut IV (rotatio) per
                                         * clavem */
} StatuaCanalis;

nomen structura {
               chorda  titulus;
                  f32  duratio;         /* tempus clavis maximum */
    StatuaCanalis* canales;
                  i32  canalium;
} StatuaCyclus;

nomen structura {
                 s32  parens;           /* -1 = radix */
                 s32  rete;             /* -1 = nullum */
       StereometriaVector  translatio;
    StereometriaQuaternio  rotatio;
       StereometriaVector  scala;
} StatuaNodus;

nomen structura {
    s32 textura;          /* -1 = alba */
    f32 color[IV];        /* factor coloris basis */
    f32 limen_alpha;      /* < 0 = opaca; aliter excisio */
    b32 emissiva;         /* lucem neglegit */
    b32 bilateralis;
    b32 repetit_s;        /* FALSUM = ad marginem */
    b32 repetit_t;
    b32 speculat_s;       /* repetitio speculata */
    b32 speculat_t;
} StatuaMateria;

/* tractus triangulorum unius materiae intra rete */
nomen structura {
    i32 index_primus;     /* in indices */
    i32 indicum;          /* multiplum III */
    s32 materia;          /* -1 = ordinaria (alba, opaca) */
} StatuaPars;

nomen structura { i32 pars_prima; i32 partium; } StatuaRete;

nomen structura {
      StatuaNodus* nodi;
                  i32  nodorum;
                  i32* ordo;            /* parentes ante liberos */
       StatuaRete* retia;
                  i32  retium;
       StatuaPars* partes;
                  i32  partium;
                  f32* positiones;      /* III per verticem */
                  f32* normales;        /* III per verticem */
                  f32* coordinatae;     /* II per verticem */
                  i32  verticum;
                  i32* indices;
                  i32  indicum;
    StatuaMateria* materiae;
                  i32  materiarum;
                Imago* texturae;
                  i32  texturarum;
     StatuaCyclus* cycli;
                  i32  cyclorum;
        StereometriaVector  centrum;         /* sphaera omnes cyclos tegens */
                  f32  radius;
} Statua;

/* post impletionem: ordo (FALSUM si cyclus in arbore aut parens extra
 * fines) et sphaera ambiens positurae quietis (T4: cycli eam non
 * augent) */
b32 statua_finire (Statua* s, Piscina* piscina);

/* matrices mundi, una per nodum (vocator nodorum locos praebet).
 * cyclus -1 = quies. tempus involvitur per durationem (duratio 0 =
 * clavis prima). Nodus sine canali valorem quietis tenet. */
vacuum statua_positura (constans Statua* s, s32 cyclus,
    f32 tempus, StereometriaMatrix* matrices);
```

- [ ] Tests build a model by hand in the arena (two nodes, child at
  translation (0,1,0) under a parent; one 1 s clip):
  - "quies": cyclus -1 gives parent identity, child translation
    (0,1,0).
  - "linearis": parent translation channel keys 0 s -> (0,0,0), 1 s ->
    (2,0,0); at 0.5 the child's world origin is (1,1,0).
  - "gradatim": three keys (0, 0.5, 1 s -> x 0, 1, 2) with
    `gradatim`: at 0.25 and 0.49 still (0,1,0), at 0.5 and 0.75
    (1,1,0). (t = 1.0 wraps to the start, so the last key cannot be
    the check.)
  - "involutio": time 1.25 equals time 0.25; time -0.25 equals 0.75.
  - "rotatio": parent rotation keys identity -> 90 degrees about y;
    at 0.5 child x axis points to (cos 45, 0, -sin 45).
  - "sine canali": a node no channel names keeps its rest matrix
    while a clip plays.
  - "finire ordo": nodes given child-before-parent come out
    parent-first in `ordo`; a parent cycle returns FALSUM; a parent
    index beyond `nodorum` returns FALSUM.
  - "sphaera": a unit cube mesh on the child, with the linearis clip,
    has every posed corner at every sampled time inside
    centrum/radius.
- [ ] Run: red. Implement. Green.
- [ ] Plant: in key lookup use the previous key's value for linear
  channels (no blend); "linearis" red. Restore, `cmp`.
- [ ] examen, formator, lint.

### Task 3: statua_glb — the reader

**Files:** create `include/statua_glb.h`, `lib/statua_glb.c`,
`probationes/probatio_statua_glb.c`; copy the two exports to
`probationes/fixa/statuae/lapis.glb` and `fungus.glb`. Reference for
the format's corners (do not copy): `../solarium/glb.c`, `skel.c`.

**Consumes:** Tasks 1-2, `json.h`, `imago.h`. **Produces:**

```c
nomen structura {
           b32  successus;
    Statua* statua;   /* NIHIL si recusatum */
        chorda  error;        /* causa verbis planis, Anglice */
} StatuaFructus;

/* numquam ruit: omnis offset et numerus contra fines probatur */
StatuaFructus statua_glb_legere (chorda octeti,
    Piscina* piscina);
```

Rules beyond the spec's lists (IV.3):
- Container: magic `glTF`, version 2, a JSON chunk then one BIN
  chunk; lengths checked against the byte count before any read.
- Accessors: FLOAT for positions / normals / times / values; indices
  UNSIGNED_BYTE / SHORT / INT; texture coordinates FLOAT or normalized
  UNSIGNED_BYTE / SHORT. `byteStride` honoured. An accessor without a
  bufferView is refused.
- Nodes: `matrix` is decomposed into translation / rotation / scale;
  a node naming a camera or light is kept as an empty node.
- One vertex pool: each primitive's vertices are appended, indices
  rebased. A primitive without indices gets 0..n-1.
- Scene: the default scene, else scene 0. Nodes outside it are kept
  (the tree is by parent index) but their meshes are dropped.
- Animation channels targeting `weights` are refused (morph targets).
- A texture image the decoder rejects refuses the file with the
  decoder's reason.
- `statua_finire` is called last; its FALSUM is a refusal.

- [ ] Tests:
  - "lapis": nodes 54, retia 30, partes 40, cycli 2 named `walk`
    (duratio 1.4 +- 0.01) and `boulder_throw` (1.6), texturae 1 of
    256x256, three materials with `limen_alpha` 0.5, exactly one
    `emissiva`, `radius` > 0.
  - "fungus": nodes 42, retia 24, partes 29, one clip `walk_bouncy`
    (1.0), no emissive material.
  - "numeri integri": a hand-written GLB (built in the test by a
    small helper that wraps JSON + BIN) with `"scale":[2,2,2]` and
    key times stored as written `0` and `1` in `min`/`max` loads with
    scala 2.0.
  - "recusationes", one hand-built file each, each asserting
    `successus == FALSUM` and a non-empty `error`: bad magic; version
    1; JSON chunk length past the end; a skin; morph targets;
    `extensionsRequired: ["X"]`; an image with `uri`; primitive mode
    1 (lines); CUBICSPLINE sampler; sparse accessor; an index
    accessor whose count runs past the buffer; a node parent cycle.
  - "truncata": for each fixture, every length from 0 to 4096 and
    then every 997th length to the end: the call returns (either
    result) under `CREDO_NON_RUIT`.
  - "approximata": a hand-built file with alpha mode BLEND, a
    `baseColorFactor` of [0.5,1,1,1], sampler wrap REPEAT and
    unsigned-short normalized coordinates loads: `limen_alpha` 0.5,
    `color[0]` 0.5, `repetit_s` VERUM, coordinates within [0,1].
- [ ] Run: red. Implement. Green.
- [ ] Plant: drop the bounds check on accessor count; "recusationes"
  red (not a crash: the plant replaces the refusal with a clamp).
  Restore, `cmp`.
- [ ] examen, formator, lint; worklog note on anything PixMesh does
  that the glTF text does not promise.

### Task 4: statua_pictura — the renderer

**Files:** create `include/statua_pictura.h`,
`lib/statua_pictura.c`, `probationes/probatio_statua_pictura.c`.

**Consumes:** Tasks 1-3. **Produces:**

```c
#define STATUA_GYRUS_ORDINARIUS        30.0f  /* gradus */
#define STATUA_INCLINATIO_ORDINARIA    15.0f
#define STATUA_INCLINATIO_MAXIMA       85.0f
#define STATUA_CAMPUS_GRADUS           30.0f
#define STATUA_MARGO                   0.08f  /* pars lateris */
#define STATUA_LUX_FUNDI               0.35f  /* facies obscura */

nomen structura {
    f32 gyrus;          /* gradus circa axem verticalem */
    f32 inclinatio;     /* gradus; cogitur ad +-MAXIMA */
    b32 illuminatum;    /* FALSUM = planum */
} StatuaVisus;

nomen structura {
     i8* rgba;          /* IV per pixelum; fundus albus opacus */
     i8* tectum;        /* I per pixelum: I = statua, 0 = charta */
    i32  latitudo;
    i32  altitudo;
} StatuaPictura;

/* FALSUM solum si piscina deficit; latitudo aut altitudo 0 = pictura
 * vacua, VERUM. Determinatum. */
b32 statua_pingere (constans Statua* s,
    constans StereometriaMatrix* matrices, constans StatuaVisus* visus,
    i32 latitudo, i32 altitudo, StatuaPictura* exitus,
    Piscina* piscina);
```

Fixed by this plan:
- Camera: looks at `centrum` from distance `radius / sin(half the
  smaller field of view) * (1 + 2 * MARGO)`; the smaller field of
  view is the vertical one, or the horizontal one when the picture is
  taller than wide. View = translate back * rotatio_x(inclinatio) *
  rotatio_y(gyrus) * translate(-centrum). Near = distance - radius *
  1.5 (at least distance * 0.01), far = distance + radius * 1.5.
- Triangles are clipped against the near plane (0, 1 or 2 triangles
  out); the rest is handled by the pixel bounding box clamp.
- Coverage: a pixel is covered when its centre is inside, top-left
  rule on shared edges. Depth: f32 buffer, nearer wins, equal keeps
  the first drawn.
- Texture: perspective-correct coordinates, nearest texel, v = 0 at
  the top row, wrap per material. Texel alpha * `color[3]` below
  `limen_alpha` discards the pixel (no depth write).
- Brightness, lit look: `FUNDI + (1 - FUNDI) * max(0, n . l)` with n
  the triangle's view-space face normal (from positions, flipped
  toward the eye for two-sided materials) and l the unit vector
  toward (-0.5, 0.7, 0.5) in view space. Emissive or flat: 1.
  Colour = texel * factor * brightness, rounded to nearest, clamped.
- Back faces are skipped unless `bilateralis`.

- [ ] Tests (hand-built models, 16x16 pictures unless noted):
  - "triangulum": one triangle facing the eye with a 1x1 red texture,
    flat look: covered pixel count equals the count of centres inside
    (computed in the test by the edge functions), every covered pixel
    (255,0,0,255) with `tectum` 1, every other pixel white, `tectum`
    0.
  - "profunditas": two overlapping triangles, far one drawn last:
    the overlap shows the near colour.
  - "tergum": a triangle facing away draws nothing; the same with
    `bilateralis` draws.
  - "excisio": a 2x1 texture with one transparent texel and
    `limen_alpha` 0.5 leaves the transparent half uncovered.
  - "lux": lit look, a face toward the light is brighter than one
    away; an emissive material equals its flat colour; the darkest
    possible face is 35 % (+-1 level) of the flat colour.
  - "margo": the golem in its rest pose at four yaw angles (0, 90,
    180, 270) and tilts -85, 15, 85 never covers a pixel on the
    picture's outer border, at 360x450 and at 450x360, and covers
    between 8 % and 60 % of it. (Rest pose: framing no longer covers
    the clips, see spec decision 15.)
  - "prope": a triangle crossing the near plane and one wholly behind
    the eye render under `CREDO_NON_RUIT`; a zero-area triangle
    draws nothing.
  - "vacua": 0x0 and 1x1 pictures return VERUM.
  - "fixa": SHA-256 of `rgba` for the golem, clip 0 at 0.35 s, yaw 30,
    tilt 15, 360x450, flat and lit - two pinned digests, pinned after
    Fran has looked at the two PNGs (Task 6's tool writes them; until
    then the test only prints the digests and is marked so).
  - "determinatum": two calls give identical bytes.
- [ ] Run: red. Implement. Green.
- [ ] Plants: (a) invert the depth comparison -> "profunditas" red;
  (b) drop the alpha test -> "excisio" red. Restore, `cmp` each.
- [ ] Measure pose + render for the golem at 360x450 (N = 30, scratch
  build with the flags from `build/aedilis/.../struere.sh`); record in
  the worklog against the 5 ms target.
- [ ] examen, formator, lint.

### Task 5: Bayer and nearest in the dithering library

**Files:** create `include/dithering_ordinatum.h`,
`lib/dithering_ordinatum.c`, `probationes/probatio_dithering_ordinatum.c`
(a separate module: `dithering.c` must stay free of the exemplaria
sources, which tessera does not link).

**Consumes:** `DITHERING_BAYER` (8x8 thresholds,
`include/dithering_exemplaria_fontes.h`). **Produces:**

```c
/* color activus proximus in spatio RGB (idem ac Atkinson sine
 * diffusione) */
DitheringFructus dithering_proximus_colorum (constans i8* rgba,
    i32 latitudo, i32 altitudo, constans b32 colores_activi[XVI],
    Piscina* piscina);

/* dithering ordinatum, craticula VIII x VIII ad originem imaginis
 * ancorata; nihil inter pixela fertur */
DitheringFructus dithering_bayer_colorum (constans i8* rgba,
    i32 latitudo, i32 altitudo, constans b32 colores_activi[XVI],
    Piscina* piscina);
```

Bayer (as built; the plan's two-colour rule was dropped after it was
seen on real frames - `lib/dithering_ordinatum.worklog.md`): for each pixel
colour a plan of 16 candidates is made. Candidate k is the nearest
active colour to the pixel plus the accumulated error of candidates
0..k-1; the candidates are sorted by luminance and the output is
`candidati[DITHERING_BAYER[y & 7][x & 7] / 4]`. Plans are kept per
call in a table keyed by colour. Ties in "nearest" go to the lower
index.

- [ ] Tests:
  - "proximus": each palette colour maps to itself; with only black
    and white active, (100,100,100) maps to black.
  - "bayer purus": an image of one palette colour comes out all that
    index.
  - "bayer medius": black and white active, flat (128,128,128) 16x16:
    between 126 and 130 white pixels of 256, and no 2x2 block is
    uniform.
  - "bayer gradus": black and white active, a horizontal ramp 0..255:
    white count per 8-column band never decreases left to right.
  - "bayer ancora": an image and the same image shifted 8 pixels
    right give outputs equal under the same shift; shifted 1 pixel,
    the flat-grey case differs.
  - "bayer fixa": SHA-256 of the indices for
    `probationes/fixa/exemplaria/` golem fixture (the one the velox
    suite uses), pinned.
  - "inactivi": with a single active colour both functions return
    only it.
- [ ] Run: red. Implement. Green.
- [ ] Plant: anchor the grid at `(x + 1) & 7`; "bayer fixa" red.
  Restore, `cmp`.
- [ ] Measure both at 360x450 (target 5 ms); worklog.
- [ ] examen, formator, lint; update `lib/dithering.interrogationes.stml`
  if it lists the dither modes.

### Task 6: the store and `bin/statuae`

**Files:** create `include/statuae_repositorium.h`,
`lib/statuae_repositorium.c`,
`probationes/probatio_statuae_repositorium.c`, `tools/statuae.c`,
`tools/statuae_struere.sh` (model on `tools/negotia_struere.sh`);
modify `aedificatio.stml` (an `actio titulus="statuae"` like the
negotia one at :83).

**Consumes:** Tasks 3-5, `volumen.h`, `imago_png.h`. **Produces:**

```c
#define STATUAE_ACTUM_ADDITUM "statua-addita"
#define STATUAE_ACTUM_REMOTUM "statua-remota"

nomen structura {
    chorda titulus;
    chorda sigillum_hex;
} StatuaeInscriptio;

/* massa + actum in transactione una (iungit transactionem vocatoris).
 * Titulus iam notus: octeti substituuntur, locus in ordine manet.
 * Titulus vacuus aut cum '/' aut longior LXIV octetis recusatur. */
b32 statuae_addere (Volumen* volumen, chorda titulus, chorda octeti);

/* VERUM etiam si titulus aberat */
b32 statuae_removere (Volumen* volumen, chorda titulus);

/* ordine additionis primae; Xar de StatuaeInscriptio */
Xar* statuae_enumerare (Volumen* volumen, Piscina* piscina);
```

Acts carry JSON: additum `{"titulus": "...", "massa": "<hex>",
"forma": "glb"}`, remotum `{"titulus": "..."}`. The list is derived by
replaying the two genera with `volumen_acta_generum_replicae_legere`
for this volume's own replica (the rule at `volumen.h:358-363`), or
`volumen_acta_generum_legere` when the replica is empty. Bytes come
back through `volumen_massam_promere`.

The tool:

```
statuae addere <file> -volumen <path> [-titulus <name>]
statuae removere <title> -volumen <path>
statuae enumerare -volumen <path>
statuae pingere <file> -o <out.png> [-cyclus <n>] [-tempus <s>]
    [-gyrus <deg>] [-inclinatio <deg>] [-planum]
    [-modus atkinson|bayer|exemplaria|nullum|crudum]
    [-latitudo <w>] [-altitudo <h>]
```

`addere` reads the file, runs `statua_glb_legere`, and on refusal
prints the reason and exits 1 without touching the volume. The title
defaults to the file's base name without extension. A missing
`-volumen` is a usage error (exit 2): no default path. `enumerare`
prints one `title<TAB>seal` per line. `pingere` needs no volume and
writes the frame the pane would show (`crudum` = undithered render);
defaults 360x450, clip 0, time 0, the default view, lit, `crudum`.

- [ ] Store tests (temporary volume):
  - "addere et enumerare": two adds list in order with seals equal to
    `sigillum` of the bytes; `volumen_massam_promere` returns the
    bytes.
  - "substitutio": re-adding the first title with new bytes keeps it
    first with the new seal.
  - "removere": removed title disappears; removing an unknown title
    is VERUM; re-adding a removed title appends it at the end.
  - "tituli mali": empty, containing `/`, 65 bytes long: FALSUM and
    `volumen_seq_ultimum` unchanged.
  - "seq": `volumen_seq_ultimum` rises on add and on remove, and a
    second `volumen_aperire` handle on the same file sees the rise
    (this closes the spec's AUDIENDA item).
- [ ] Run: red. Implement store. Green. Plant: replay ignores
  remotum; "removere" red. Restore, `cmp`.
- [ ] Build the tool (`./tools/statuae_struere.sh`); tool test in
  the same suite via `processus_*`: `addere` of a fixture into a
  temporary volume then `enumerare` prints its title; `addere` of a
  truncated fixture exits 1 and `enumerare` stays empty; no
  `-volumen` exits 2; `pingere ... -modus bayer` writes a PNG whose
  size is 360x450.
- [ ] Write the eight judging frames to `build/statuae/`
  (golem and mushroom, flat and lit, `crudum`; golem lit in the four
  modes) and show Fran. On his nod, pin Task 4's two digests.
- [ ] examen, formator, lint; `bin/fabrica sanare` for the new action.

### Task 7: membrum__delectus — the dropdown member

**Files:** create `include/membrum__delectus.h`,
`lib/membrum__delectus.c`, `probationes/probatio_membrum__delectus.c`.
Read `lib/membrum__elenchi.c` and `include/elenchus.h` whole first;
this member is the same pattern with a button as its anchor.

**Produces:** state lives in the host's branch, in two manifest fields
the host declares: `delectus` (`typus="nomen" vita="transiens"
aspectus="popover"`: the id of the open dropdown, absent = closed) and
`delectus_electa` (`typus="numerus" vita="transiens"`).

```c
nomen enumeratio {
    DELECTUS_NIHIL = ZEPHYRUM,  /* eventus non delectus */
    DELECTUS_DEGLUTITUM,        /* consumptum, nihil electum */
    DELECTUS_ELECTUM,           /* *electum = index optionis */
    DELECTUS_CLAUSUM            /* Esc aut ictus extra */
} DelectusExitus;

/* botton "textus ▾" (PARTES_BOTTONE, id datum, actio hospitis) */
Componens* delectus_bottonem_componere (constans character* id,
    Fines fines, constans character* actio, Piscina* piscina,
    InternamentumChorda* intern);

/* latitudo bottonis pro textu (FONS_PARVUS) */
i32 delectus_bottonis_latitudo (chorda textus);

vacuum delectus_bottonem_pingere (Mandata* md, Fines fines,
    chorda textus, b32 apertus, b32 habilis);

b32 delectus_aperire (InsulaRamus* ramus, constans character* scriptor,
    constans character* id, s32 electa);
vacuum delectus_claudere (InsulaRamus* ramus,
    constans character* scriptor);
/* VERUM si delectus 'id' apertus */
b32 delectus_apertus (constans InsulaRamus* ramus,
    constans character* id, Piscina* piscina);

/* menu sub bottone (reflexum sursum si area deest); NIHIL si
 * clausum. Filius ULTIMUS radicis hospitis addendus. */
Componens* delectus_componere (constans InsulaRamus* ramus,
    constans Elenchus* optiones, Fines botton, Fines area,
    constans character* actio, Piscina* piscina,
    InternamentumChorda* intern);

/* dum apertus: j / k / sagittae movent, Reditus eligit, Esc claudit,
 * ictus in optione eligit, ictus extra claudit, cetera deglutit */
DelectusExitus delectus_eventum_agere (InsulaRamus* ramus,
    constans character* scriptor, constans Elenchus* optiones,
    constans Componens* nodus, constans Eventus* ev, s32* electum);
```

- [ ] Tests (a minimal host branch with the two fields, as the
  membrum__elenchi suite builds one):
  - "clausus": `delectus_componere` returns NIHIL; a key event
    returns DELECTUS_NIHIL.
  - "aperire": after `delectus_aperire(.., "cyclus", 1)` the menu
    node exists, is anchored at the button's bottom-left, and option
    1 is highlighted.
  - "claves": `j` moves to 2 (DEGLUTITUM); `k` back; Enter returns
    ELECTUM with `*electum` 1 and the dropdown is closed; Esc returns
    CLAUSUM.
  - "ictus": a click on option node `elenchus.2` returns ELECTUM 2;
    a click elsewhere returns CLAUSUM; an unrelated key while open
    returns DEGLUTITUM.
  - "reflexio": a button on the bottom edge of `area` puts the menu
    above it.
  - "duo": with two dropdown ids, opening the second closes the
    first (`delectus_apertus` of the first FALSUM).
- [ ] Run: red. Implement. Green. Plant: Enter does not close;
  "claves" red. Restore, `cmp`.
- [ ] examen, formator, lint.

### Task 8: the pane library

**Files:** create `include/statuae.h`, `lib/statuae.c`,
`apps/statuae/organum.stml`, `probationes/probatio_statuae.c`.
Pattern: `lib/aetherophonum.c` (drag with capture, `ictus_primus`,
command table) and `lib/antiphonale.c` (`pulsare` / `mora`, header
arrows). Run `tools/organa_infigere.sh` after writing the manifest.

**Consumes:** Tasks 2-7. **Produces:**

```c
nomen structura StatuaeMontatio StatuaeMontatio;
/* horologium (ms monotonica) - probationes suum praebent */
nomen structura {
       s64  (*nunc)(vacuum* ctx);
    vacuum* ctx;
} StatuaePorta;

memoriae_index statuae_montationis_mensura (vacuum);
b32 statuae_montare (StatuaeMontatio* m, Piscina* piscina,
    InternamentumChorda* intern, Volumen* volumen,
    InsulaRepositorium* repo, constans character* id,
    constans StatuaePorta* porta, i32 latitudo, i32 altitudo,
    StatusSystemata* systemata);
Componens* statuae_componere (InsulaRepositorium* repo,
    constans Motus* motus, Piscina* piscina,
    InternamentumChorda* intern, vacuum* ctx);
b32 statuae_pulsare (vacuum* ctx);          /* VERUM = repinge */
Mora statuae_mora (vacuum* ctx);
constans Imago* statuae_imago_fons (chorda titulus, vacuum* ctx);
b32 statuae_ictus_primus (vacuum* ctx, InsulaRepositorium* repo,
    Motus* motus, Componens* arbor, constans Eventus* ev);
ActioRegistrum*  statuae_actiones (StatuaeMontatio* m);
FiguraRegistrum* statuae_figurae (StatuaeMontatio* m);
```

(The struct may be public like `AetherophoniMontatio` if the glue
needs its size at compile time; then drop `_mensura`.)

Manifest fields, all `scriptor="statuae.agere"`:

| field | type | life | absent means |
|---|---|---|---|
| `statua` | textus | persistens | first model |
| `cyclus` | textus (clip name) | persistens | first clip |
| `modus` | nomen: atkinson / bayer / exemplaria / nullum | persistens | atkinson |
| `planum` | veritas | persistens | lit |
| `duplex` | veritas | persistens | full size |
| `quiescit` | veritas | persistens | playing |
| `gyrus` | numerus (tenths of a degree, 0..3599) | persistens | 300 |
| `inclinatio` | numerus (tenths, -850..850) | persistens | 150 |
| `delectus`, `delectus_electa` | Task 7 | transiens | closed |

Fixed by this plan:
- Layout: header row of one text line height at the top; picture
  below it. Header left to right: `◂`, `n/m`, `▸`, title; right
  aligned: clip dropdown, dither dropdown, `lit`/`flat`, `2x`. If the
  sum of widths exceeds the pane width the two dropdowns and toggles
  go on a second row and the picture starts lower.
- Picture size = area below the header; with `duplex`, half of it
  rounded down, shown through `componens_ponere_transformatio` scale
  2. Either dimension 0: no frame is made and nothing is drawn.
- Frame key = (model seal, clip, clip time in ms, gyrus, inclinatio,
  modus, planum, picture width, height). `statuae_pulsare` computes
  the clip time from the clock, and makes a frame only if the key
  changed; it returns VERUM only then. While paused the clip time is
  frozen at the pause instant.
- Pipeline per frame, in a frame arena emptied first: one call of
  `statuae_quadrum_pingere` (T6) with `charta_perlucida` VERUM; the
  result is the `Imago` that `statuae_imago_fons` returns under the
  name `statuae:<branch id>`.
- `statuae_mora`: 33 while (a clip exists and not `quiescit`) or a
  drag is held; otherwise 1000 (the volume poll).
- Poll: when 1000 ms have passed since the last one, compare
  `volumen_seq_ultimum`; on change re-list the store. The shown model
  is kept by title; if it is gone, the first model shows.
- Loading: the shown model is parsed once when it becomes the shown
  one (own arena, emptied on change). A refusal is kept as the text
  to show.
- Drag: capture on left button down inside the picture; each move
  adds `dx * 5` to gyrus (wrapping) and `dy * 5` to inclinatio
  (clamped), in tenths of a degree per pane pixel; written to the
  fields on release, held in memory during the drag.
- Keys, through a `Praeceptum` table: `h` / `l` gyrus -+150, `j` /
  `k` inclinatio +-150, `r` reset view, `[` / `]` previous / next
  model (wrapping), `c` clip dropdown, `d` dither dropdown, `f`
  planum, `2` duplex, Space quiescit. While a dropdown is open every
  event goes to `delectus_eventum_agere` first.
- Changing model: clip falls back to that model's first clip if the
  remembered name is not among its clips; view resets to 300 / 150.
  Changing clip restarts clip time at 0 and keeps the view.
- Empty store: the picture area shows `No models yet` and, below it,
  `statuae addere <file.glb> -volumen <volume>`. Refused model:
  title, then the reason, wrapped to the pane.

- [ ] Tests (temporary volume with both fixtures added through the
  store; a test clock in `StatuaePorta`; events sent as the
  aetherophonum suite sends them):
  - "primum": after mount and one `pulsare`, `statuae_imago_fons`
    returns an image of the picture size with at least one alpha-0
    and one opaque pixel.
  - "tempus": advancing the clock 100 ms makes `pulsare` return VERUM
    and the image bytes change; not advancing returns FALSUM.
  - "quies": after Space, `pulsare` returns FALSUM as the clock
    advances and `statuae_mora` is 1000; Space again resumes from
    the frozen time (image equals the one at the pause instant, then
    changes).
  - "tractus": down at (100,100), move to (140,100), up: field
    `gyrus` is 500; a vertical drag of 1000 pixels leaves
    `inclinatio` at 850; during the drag `mora` is 33 even when
    paused.
  - "claves": `]` shows the second model and resets the view; `[`
    wraps back; `f` toggles `planum` and changes the image; `2`
    halves the image's size; `r` restores 300 / 150.
  - "delectus": `d` opens the dither dropdown, `j` then Enter sets
    `modus` to bayer and the image changes; `c` on the golem lists
    two clips and choosing the second sets `cyclus` to
    `boulder_throw`; on a model without clips `c` does nothing.
  - "modi": each of the four modes yields an image; all pixels are
    palette colours or alpha 0.
  - "vacuum": mounting on an empty volume gives no image and a
    component tree containing the empty-state text; after
    `statuae_addere` and a clock advance of 1000 ms, `pulsare`
    returns VERUM and an image exists. (Review focus 4)
  - "recusatum": a store entry whose blob is 10 bytes of garbage
    shows its title and a reason, no image, and `]` still moves to
    the next model. (Review focus 2)
  - "magnitudines": panes of 0x0, 1x1, 361x451 with `duplex`, and a
    pane shorter than the header: mount, `pulsare`, compose and the
    image source all return under `CREDO_NON_RUIT`. (Review focus 5)
  - "angustum": at 200 px wide the controls are on two rows (the
    picture node's y is two rows down); at 360 px report which
    layout results (worklog; spec decision 10).
  - "memoria": 200 frames with the clock advancing leave the
    montation arena's size unchanged after the first frame.
- [ ] Run: red. Implement. Green.
- [ ] Plants: (a) frame key ignores `modus` -> "delectus" red;
  (b) poll never re-lists -> "vacuum" red. Restore, `cmp` each.
- [ ] examen, formator, lint; worklog.

### Task 9: into vicus

**Files:** modify `lib/vicus_applicatio.c` (glue beside
`antiphonale_montare_in_vico` at :3164; `vicus_genus_addere` in the
chain at :6529-6598; `aperiens_fixum_registrare(cs->iussa, app->vicus,
"models", "statuae", NIHIL, piscina)` beside :6487); create
`probationes/probatio_vicus_statuae.c` (model:
`probatio_vicus_antiphonale.c`); modify
`project-specs/statuae-spec.md` (status, measured numbers, AUDIENDA
answered).

Glue: `statuae_describere` sets `actiones`, `figurae`, `componere`,
`fons` (+ ctx), `pulsare`, `mora`, `ictus_primus`, `effugium_redit =
VERUM`, `locus_titulus` (the model's title). The clock port is the
application's monotonic clock.

- [ ] Tests:
  - "aperitur": `$models` typed and clicked in a scriba page opens a
    `statuae` pane at the front of the right stack.
  - "pingitur": with a fixture in the test volume, one application
    tick then `vicus_imago_fons` for `statuae:<id>` returns an
    image.
  - "non visum": with the pane behind another in the stack, the
    application's next mora is not 33 (the hidden pane asks for
    nothing).
  - "quies": pane visible and paused: next mora >= 1000.
- [ ] Run: red. Implement. Green. Plant: describere omits `mora`;
  "quies" red. Restore, `cmp`.
- [ ] Screenshot with `-imago` of vicus showing the golem in each
  dither; Read the PNGs; show Fran.
- [ ] Measure in the running vicus with the golem playing: frame time
  per dither and the whole-window repaint cost at 30 ticks a second;
  the real pane size. Write them into the spec (III, AUDIENDA) and
  the worklog.
- [ ] With Fran's OK only: `bin/statuae addere` the two models into
  his live volume.
- [ ] Owed gates (`./tools/portae_debitae.sh`), heal
  `speculum_hospes`, `generata_iudicare.sh`.
