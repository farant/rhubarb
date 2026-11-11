# mundus — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.

**Goal:** a vicus pane (`$world`) where one of Fran's characters
walks around the forest clearing under W A S D, drawn in the
16-colour palette with ground that stays calm while it scrolls.

**Architecture:** part marks in the model and reader; a parallel
"scene" entry in the existing renderer (several placed models, one
depth buffer, a cover map); a walk map built from the place's marks;
a world of bodies advanced by numbers only; a ground picture dithered
once per world-anchored region; a frame that lays exemplaria-dithered
bodies over the ground; a kind mark in the store; the pane; the glue
in vicus.

**Tech stack:** C89 + latina.h; house libraries only. Builds on
statuae (`statua`, `statua_glb`, `statua_pictura`, `statuae_quadrum`,
`statuae_repositorium`, `statuae`), all in secunda.

**Spec:** `project-specs/mundus-spec.md` (approved by Fran
2026-11-10). Read it first; this plan argues from it.

## Global constraints

- Latin identifiers; `VOCABULA_VIAE_ADDITAE=... ./oratio/vocabula.sh
  -nova` shows NOVA 0 (add glossary lemmata where a word is new).
  latina.h macros and Roman numerals are never identifiers.
- New C files through `./silva/scribe.sh`; every file
  `./silva/examen.sh` ACCIPE and `./silva/formator.sh -scribere`
  CONFORMIS; 72 columns.
- `i32` is unsigned, `s32` signed; `chorda` is not NUL-terminated;
  `-Wconversion -Wsign-conversion -Werror`. All floating point is
  `f32`.
- Arenas only. No allocation per frame outside a frame arena the
  pane empties each frame. Ground regions live in their own arena.
- TDD. Each gate is proved red by a compiling plant (`silva.planta`,
  porta `radix`, the suite as `filtrum`) before it counts.
- Pinned digests that must not move: `statua_pingere` ("fixa" in
  `probatio_statua_pictura.c`), `statuae_quadrum_pingere`,
  `dithering_exemplaria_velox*`.
- Tests write only under `credo_area(c)`, close temporary volumes,
  and never touch Fran's live volume. Rule tests use hand-made
  levels. Fran allowed committing the real level as a fixture
  (2026-11-10, "if you want"); it goes in as
  `probationes/fixa/statuae/planum.glb` in T4, for one test that
  pins the real map's cell counts.
- A new include in an old lib file breaks subsystem runners with
  hand source lists (tessera, ludus_tessera): prefer new modules;
  run both `compile_probationes.sh` before any commit.
- Headers carry `<aedilis corpus="lib/....c"/>`.

## Review focus

Inputs the spec implies and a user will meet; each has its test in
the task named.

1. A key is still held when the pane loses focus or is hidden: the
   character stops and the pane stops ticking. (T1, T9)
2. A character with no clip whose name contains `walk`, or with no
   clips at all: it still moves, in its first clip or its rest pose.
   (T5, T9)
3. The view reaches the edge of the level, or the pane is larger
   than the level: outside is background colour, nothing is read out
   of bounds; a 0- or 1-pixel pane draws nothing. (T6, T7)
4. No place in the volume, a place the reader refuses, or a place
   with no walkable cell: the pane says so and does not crash. (T4,
   T9)
5. A long stall between ticks (seconds): the step is capped and the
   body neither teleports nor passes through a cliff; diagonal
   walking is not faster than straight. (T5)

## Files

| file | holds |
|---|---|
| `include/statua.h`, `lib/statua_glb.c` | part marks per node (changed) |
| `include/statua_pictura.h`, `lib/statua_pictura.c` | scene entry (added) |
| `include/mundus_solum.h`, `lib/mundus_solum.c` | the walk map |
| `include/mundus.h`, `lib/mundus.c` | bodies and the step |
| `include/mundus_pictura.h`, `lib/mundus_pictura.c` | ground regions, the frame |
| `include/statuae_quadrum.h`, `lib/statuae_quadrum.c` | body dither entry (added) |
| `include/statuae_repositorium.h`, `lib/statuae_repositorium.c` | kind mark (added) |
| `include/mundus_latus.h`, `lib/mundus_latus.c` | the pane |
| `lib/vicus_applicatio.c`, `tools/statuae.c` | glue, `-locus` |
| `probationes/probatio_mundus*.c` and the existing statua suites | tests |

Each new `.c` gets a worklog beside it when there is something to
record.

---

### Task 1: key releases and focus loss reach a pane

**Why first:** held-key walking depends on it and it is the one
unknown outside mundus (spec AUDIENDA). Known so far: fenestra emits
`EVENTUS_CLAVIS_LIBERATUS`; `lib/vicus.c` filters mention it;
every existing pane acts on `DEPRESSUS` only; `VicusFacies`
(`include/vicus.h:93-205`) has no focus-lost callback.

**Files:** `include/vicus.h`, `lib/vicus.c`, the vicus test suite
that already mounts a fake pane (find with `./silva/nexus.sh
vicus_latus_focatum`).

**Produces:**

```c
/* in VicusFacies: latus focum amisit aut absconditum est (tabula
 * mutata, latus aliud in fronte, fenestra focum amisit). NIHIL =
 * nihil. */
vacuum  (*focus_amissus)(vacuum* ctx);
vacuum* focus_amissus_ctx;
```

DONE 2026-11-10. Releases already reach a focused pane (traced by
reading; `lib/vicus.worklog.md`), so the "liberatio" test moved to
T9's "ambulat", which needs a pane that listens. The callback is
called from one place (`motum_aptare`) plus window blur.

- [x] Trace whether a focused pane's event handler receives
  `LIBERATUS` for a plain letter key. Record the path in
  `lib/vicus.worklog.md`.
- [ ] Tests (fake pane that records what it gets):
  - "liberatio": press and release `w` on the focused pane; the pane
    saw both, in order.
  - "focus_amissus": the callback fires once on each of: focus moved
    to the other side, tab changed, another pane put in front; it
    does not fire while the pane keeps focus.
  - If fenestra reports window blur, the same on blur; if it does
    not, say so in the worklog and leave it (the OS sends no
    release then; the pane's own guard in T9 covers it).
- [ ] Red, implement the smallest change, green.
- [ ] Plants: (a) drop the release delivery -> "liberatio" red;
  (b) drop one call site of the callback -> "focus_amissus" red.
- [ ] examen, formator, lint; both tessera runners.

### Task 2: part marks in the model and the reader

DONE 2026-11-10 (glossary: gressibilis).

**Files:** `include/statua.h`, `lib/statua_glb.c`,
`probationes/probatio_statua_glb.c`.

**Produces:**

```c
#define STATUA_SIGNUM_GRESSIBILE  I     /* "walkable" */
#define STATUA_SIGNUM_SOLIDUM     II    /* "collidable" */
#define STATUA_SIGNUM_AQUA        IV    /* "water" */

/* in StatuaNodus */
i32 signa;      /* STATUA_SIGNUM_*; 0 = nullum */
```

Fixed by this plan: marks come from the node's `extras.tags`, an
array of strings; any other shape of `extras`, unknown strings and a
missing `extras` give 0 and never a refusal.

- [ ] Tests (GLB bytes built in the test, as the suite already does):
  "signa": three nodes tagged `["ground","walkable","collidable"]`,
  `["water"]`, `["decor"]` read back as GRESSIBILE|SOLIDUM, AQUA, 0;
  "signa_mala": `extras` a number, `tags` a string, a tag that is a
  number: all 0, file accepted. Existing digests unmoved.
- [ ] Red, implement, green.
- [ ] Plant: read `tags` but never set the bit -> "signa" red.
- [ ] examen, formator, lint.

### Task 3: the scene entry in the renderer

DONE 2026-11-10. Real level, every triangle, no culling: 3.7 ms at
360x438 and 5.6 ms at 480x541 (40 ppu), 5.0 ms at 16 ppu. The
"fixa" digest was pinned after Claude looked at the PNG
(`build/statuae/scaena_lapis.png`); Fran has not seen it yet.

**Files:** `include/statua_pictura.h`, `lib/statua_pictura.c`,
`probationes/probatio_statua_pictura.c`.

**Consumes:** T2 is not needed. **Produces:**

```c
nomen structura {
                constans Statua* statua;
    constans StereometriaMatrix* matrices;  /* statua_positura */
             StereometriaMatrix  locatio;   /* statua -> mundus */
} StatuaLocata;

nomen structura {
    f32 gyrus;                /* gradus */
    f32 inclinatio;           /* gradus */
    f32 pixela_per_unitatem;  /* > 0 */
    s32 origo_x;              /* pixelum mundi in angulo summo
                               * sinistro picturae */
    s32 origo_y;
    b32 illuminatum;
} StatuaScaenaVisus;

nomen structura {
     i8* rgba;         /* IV per pixelum; vacuum: album opacum */
     i8* tectum;       /* 0 = nihil; k = locata k-1 */
    f32* profunditas;  /* unitates mundi secundum axem visus; maius
                        * = remotius; vacuum: valor maximus */
    i32  latitudo;
    i32  altitudo;
} StatuaScaenaPictura;

#define STATUA_LOCATAE_MAXIMAE CCLV

/* punctum mundi -> pixelum mundi (fractum) et profunditas */
vacuum statua_scaena_proicere (constans StatuaScaenaVisus* visus,
    StereometriaVector punctum, f32* x, f32* y, f32* profunditas);

/* FALSUM: argumenta NIHIL, numerus > MAXIMAE, pixela_per_unitatem
 * <= 0. Determinatum. */
b32 statua_scaenam_pingere (constans StatuaLocata* locatae,
    i32 numerus, constans StatuaScaenaVisus* visus, i32 latitudo,
    i32 altitudo, StatuaScaenaPictura* exitus, Piscina* piscina);
```

Fixed by this plan:
- "World pixel" of a point p: E = rotatio_x(inclinatio) *
  rotatio_y(gyrus); x = E(p).x * ppu, y = -E(p).y * ppu, depth =
  -E(p).z. No translation by any centre. Picture pixel = world pixel
  minus `origo`. Two pictures whose origins differ by whole pixels
  therefore agree on every shared pixel, and depths from different
  pictures are comparable.
- Same rasterizer, coverage rule, texture sampling, alpha cutout and
  brightness as `statua_pingere`; texture coordinates interpolate
  linearly (no perspective); a back face is one whose view-space
  normal points away along the view axis; no near plane.
- `statua_pingere` and its digests do not change.

- [ ] Tests:
  - "lapsus": a hand-built textured model, origins (0,0) and (3,-5):
    every shared pixel has identical rgba, tectum and depth.
  - "tectum": two placed models, the nearer covers the farther;
    `tectum` names the nearer where they overlap and each alone
    elsewhere; drawing order reversed gives the same bytes.
  - "locatio": the same model placed with a translation of one unit
    along x appears shifted by exactly the pixels
    `statua_scaena_proicere` predicts.
  - "proicere": at gyrus 0, inclinatio 0, ppu 10 the point (1,2,3)
    is (10,-20) with depth -3.
  - "vacua": 0x0 and 1x1; numerus 0 gives an all-white picture.
  - "fixa": digest of the golem, rest pose, 45 / 35, 40 ppu, 128x128
    centred on it; pinned after Fran has seen the PNG.
  - "determinatum": two calls, identical bytes.
- [ ] Red, implement, green.
- [ ] Plants: (a) add 0.5 to the x offset -> "lapsus" stays green
  but "proicere"/"locatio" red; (b) make the origin a float offset
  of 0.3 -> "lapsus" red; (c) invert depth -> "tectum" red.
- [ ] Measure the real level at 360x438 and 480x541, 40 ppu, against
  spec III.2 (4-8 ms); worklog.
- [ ] examen, formator, lint.

### Task 4: the walk map

DONE 2026-11-10. Two changes from the text below, both in
`lib/mundus_solum.worklog.md`: (1) the passability test is "step plus
slope" (`MUNDUS_ASCENSUS_MAXIMUS` 0.75 added to the header; the real
stairs are half-unit steps, not a ramp); (2) the real level is a
committed fixture with pinned cell counts, and its top level is
unreachable by design (no staircase leads there).

**Files:** create `include/mundus_solum.h`, `lib/mundus_solum.c`,
`probationes/probatio_mundus_solum.c`.

**Consumes:** T2. **Produces:**

```c
#define MUNDUS_CELLAE_PER_UNITATEM  IV
#define MUNDUS_CLIVUS_MAXIMUS       1.5f  /* ascensus per unitatem
                                           * itineris */

nomen enumeratio {
    MUNDUS_SOLUM_NIHIL = ZEPHYRUM,
    MUNDUS_SOLUM_GRESSIBILE,
    MUNDUS_SOLUM_AQUA,
    MUNDUS_SOLUM_SOLIDUM
} MundusSoliGenus;

nomen structura {
    f32  origo_x;       /* angulus cellae (0,0) in mundo */
    f32  origo_z;
    i32  latitudo;      /* cellae */
    i32  altitudo;
     i8* genera;        /* MundusSoliGenus per cellam */
    f32* altitudines;   /* y summi per cellam */
} MundusSolum;

/* NIHIL: argumenta NIHIL aut piscina deficit. Statua sine nodis
 * signatis: tabula tota NIHIL. */
MundusSolum* mundus_solum_struere (constans Statua* locus,
    Piscina* piscina);

MundusSoliGenus mundus_solum_genus (constans MundusSolum* s,
    f32 x, f32 z);

/* FALSUM si (x,z) non gressibile */
b32 mundus_solum_altitudo (constans MundusSolum* s, f32 x, f32 z,
    f32* y);

/* corpus radii dati stare potest in (x,z)? */
b32 mundus_solum_stare (constans MundusSolum* s, f32 x, f32 z,
    f32 radius);

/* a (x0,z0) ad (x1,z1): stare in fine licet et clivus non excedit */
b32 mundus_solum_licet (constans MundusSolum* s, f32 x0, f32 z0,
    f32 x1, f32 z1, f32 radius);

/* cella gressibilis medio tabulae proxima ubi corpus stare potest */
b32 mundus_solum_initium (constans MundusSolum* s, f32 radius,
    f32* x, f32* z);
```

Fixed by this plan: nodes in their rest pose; a node with none of
the three marks is skipped; per cell the topmost triangle at the
cell's centre decides, and at equal height a non-walkable surface
wins; a node marked walkable is GRESSIBILE even when also solid;
water without walkable is AQUA; height queries interpolate between
cell centres so a stair is a ramp, not steps.

- [ ] Tests (one hand-built level: a floor at y 0, a raised floor at
  y 4 with a vertical face, a ramp joining them over 4 units, a
  water patch, a solid block, an unmarked decor quad above the
  floor):
  - "genera": one probe point per kind, plus outside the level
    (NIHIL) and under the decor quad (GRESSIBILE).
  - "altitudo": floor 0, raised 4, mid-ramp 2 (within 0.3).
  - "licet": along the floor yes; floor to raised across the face
    no; up and down the ramp in steps of 0.25 yes; into water, into
    the block, off the edge no.
  - "radius": a point next to the block is allowed with radius 0 and
    refused with radius 0.5.
  - "initium": lands on a walkable cell; a level with no walkable
    cell returns FALSUM.
  - "vacua": a model with no marks: all NIHIL, no crash.
- [ ] Red, implement, green.
- [ ] Plants: (a) let solid lose the tie at equal height; (b) drop
  the slope test -> "licet" red; (c) ignore the radius -> "radius"
  red.
- [ ] Scratch check on the real level: print the map, compare with
  spec III.4 (three levels, stairs passable in both directions,
  ponds and hut blocked); worklog, with the slope limit and grid
  finally chosen.
- [ ] examen, formator, lint.

### Task 5: the world

DONE 2026-11-10. One fix reached back into T4: the body's footprint
is measured from its cell's centre (a straight wall was "bumpy" and
sliding stuck). New shared test level
`probationes/mundus_locus_fictus.h`.

**Files:** create `include/mundus.h`, `lib/mundus.c`,
`probationes/probatio_mundus.c`.

**Consumes:** T4. **Produces:**

```c
#define MUNDUS_CELERITAS        4.0f    /* unitates per secundam */
#define MUNDUS_VERSURA          720.0f  /* gradus per secundam */
#define MUNDUS_GRADUS_MAXIMUS   0.1f    /* secundae per gradum */
#define MUNDUS_PASSUS           0.125f  /* unitates: motus in
                                         * particulas dividitur */
#define MUNDUS_CORPORIS_RADIUS  0.5f
#define MUNDUS_CORPORIS_ALTITUDO 2.0f

nomen structura {
    f32 x;              /* locus in solo */
    f32 z;
    f32 y;              /* altitudo soli sub corpore */
    f32 facies;         /* gradus; 0 = +z, crescit versus +x */
    f32 radius;
    b32 intendit;       /* VERUM: ambulare vult */
    f32 intentum_x;     /* directio in mundo; longitudo neglegitur */
    f32 intentum_z;
    b32 movetur;        /* gradu ultimo locum mutavit */
    f32 tempus_motus;   /* secundae motus continui (tempus cycli) */
    s32 forma;          /* index vocantis (quae statua); mundus eum
                         * non legit */
} MundusCorpus;

nomen structura {
    constans MundusSolum* solum;
            MundusCorpus* corpora;
                     i32  corporum;
} Mundus;

/* corpus in loco gressibili ponere; FALSUM si ibi stare non potest */
b32 mundus_corpus_ponere (constans Mundus* m, MundusCorpus* c,
    f32 x, f32 z);

/* mundum dt secundis promovere (dt cogitur ad [0, GRADUS_MAXIMUS]) */
vacuum mundus_gradi (Mundus* m, f32 dt);
```

Fixed by this plan: intent is normalized, so diagonal speed equals
straight speed; a step moves in pieces of at most `MUNDUS_PASSUS`,
each tried whole, then x only, then z only (sliding); `facies` turns
toward the intent at `MUNDUS_VERSURA` by the shorter way, also while
blocked; `tempus_motus` grows while `movetur` and returns to 0 when
it stops; bodies do not block each other in this stage.

- [ ] Tests (numbers only, on T4's hand-built level):
  - "celeritas": 1 s of intent +x in steps of 1/30 s and in steps of
    1/7 s both travel 4 units (within 0.05) - the 1/7 s steps are
    capped at 0.1 s each, so that run is compared over the same
    capped time.
  - "diagonalis": intent (1,1) for 1 s travels 4 units.
  - "rupes": walking at the raised face stops before it; `movetur`
    goes FALSUM.
  - "lapsus_parietis": intent diagonal into the face moves along it.
  - "clivus": walks up the ramp; `y` goes 0 -> 4.
  - "margo": cannot leave the level; cannot enter water or the block.
  - "mora_longa": one call with dt 5 s moves at most 0.4 units and
    does not cross the face.
  - "facies": after 1 s of intent +x the body faces 90; after
    release it keeps its facing; `tempus_motus` 0 when stopped.
  - "ponere": refused in water.
- [ ] Red, implement, green.
- [ ] Plants: (a) no normalization -> "diagonalis" red; (b) no dt
  cap -> "mora_longa" red; (c) no single-axis retry ->
  "lapsus_parietis" red.
- [ ] examen, formator, lint.

### Task 6: the ground picture

DONE 2026-11-10. Regions are rendered with a 16-pixel margin from the
start (no seam visible to Claude in `build/statuae/humus_suturae.png`;
Fran to judge). Added `mundus_visus(origo_x, origo_y)` to the header:
the one place the fixed view is assembled, which T7 also needs.

**Files:** create `include/mundus_pictura.h`, `lib/mundus_pictura.c`,
`probationes/probatio_mundus_pictura.c`.

**Consumes:** T3. **Produces:**

```c
#define MUNDUS_GYRUS          45.0f
#define MUNDUS_INCLINATIO     35.0f
#define MUNDUS_PIXELA_PER_UNITATEM 40.0f
#define MUNDUS_REGIO          CCLVI   /* latus regionis, pixela */

nomen structura MundusHumus MundusHumus;

/* humus loci: regiones pigre factae, in piscina data servatae */
MundusHumus* mundus_humus_creare (constans Statua* locus,
    constans b32* activi, Piscina* piscina);

/* rectangulum pixelorum mundi in indices palettae et profunditatem
 * copiare; extra locum: index = fundus dati, profunditas maxima.
 * FALSUM si piscina deficit. */
b32 mundus_humus_copiare (MundusHumus* h, s32 origo_x, s32 origo_y,
    i32 latitudo, i32 altitudo, i8 fundus, i8* indices,
    f32* profunditas);

/* regiones iam factae (probationibus et mensurae) */
i32 mundus_humus_regiones (constans MundusHumus* h);
```

Fixed by this plan: regions are squares of `MUNDUS_REGIO` on the
world-pixel grid (region (i,j) covers [i*R, (i+1)*R) x [j*R,
(j+1)*R), negative indices included); a region is rendered with the
place alone, lit, dithered with `dithering_atkinson_colorum`, pixels
with `tectum` 0 set to "outside"; regions wholly outside the place's
projected bounds are never made.

- [ ] Tests (a hand-built place: one textured floor):
  - "stabilis": copy at origin (10,20), then at (11,20): every
    shared pixel has the same index.
  - "pigra": a 100x100 copy inside one region makes one region; a
    copy across a corner makes four; repeating makes no more.
  - "extra": a copy wholly outside the place returns all `fundus`,
    maximum depth, and makes no region.
  - "profunditas": depth at the floor's centre equals what
    `statua_scaena_proicere` gives.
  - "sutura": print (not assert) the count of differing indices in
    a 16-pixel band across a region border versus one large render;
    Fran looks at the PNG.
- [ ] Red, implement, green.
- [ ] Plants: (a) re-dither on every copy at the copy's origin ->
  "stabilis" red; (b) forget made regions -> "pigra" red.
- [ ] Scratch: real level, time one region and a 360x438 copy;
  write a PNG of a border for Fran. If seams show, add an overlap
  margin to the region render (worklog, spec IV.5).
- [ ] examen, formator, lint.

### Task 7: bodies and the frame

DONE 2026-11-10. Measured: one body 3-5 ms; whole frame with one
character 3.5-3.9 ms; eight in view 26.7 ms average at 480x541.
Changes from the text below (reasons in
`lib/mundus_pictura.worklog.md`): two passes with a per-pixel owner
instead of far-to-near drawing; `mundus_forma` takes an arena and
`MundusForma` gained `imum`, `medium_x`, `medium_z`; scale is capped
by width (3 units); `mundus_quadri_origo` added to the header.

**Files:** `include/statuae_quadrum.h`, `lib/statuae_quadrum.c`,
`probationes/probatio_statuae_quadrum.c`; `include/mundus_pictura.h`,
`lib/mundus_pictura.c`, `probationes/probatio_mundus_pictura.c`.

**Consumes:** T3, T5, T6. **Produces:**

```c
/* statuae_quadrum: pictura iam facta + larva -> indices (exemplaria
 * cum finibus, larva et praesupputationibus statuae_quadrum).
 * Pixela extra larvam non scribuntur. */
b32 statuae_picturam_dithering (constans i8* rgba,
    constans i8* larva, i32 latitudo, i32 altitudo,
    constans b32* activi, i8* indices, Piscina* piscina);

/* mundus_pictura */
nomen structura {
    constans Statua* statua;
                s32  cyclus_ambulandi;  /* -1 = nullus */
                s32  cyclus_quietis;    /* -1 = quies */
                f32  scala;             /* ad altitudinem II */
} MundusForma;

/* forma ex statua: cyclus cuius titulus "walk" continet (aliter
 * primus, aliter -1); cyclus "idle" (aliter -1); scala ex capsa
 * quietis */
MundusForma mundus_forma (constans Statua* statua);

/* quadrum: humus + corpora, corpus 'sequens' in medio. Exitus
 * indices palettae latitudo x altitudo. */
b32 mundus_quadrum_pingere (MundusHumus* humus,
    constans Mundus* mundus, constans MundusForma* formae,
    i32 formarum, i32 sequens, constans b32* activi, i8 fundus,
    i32 latitudo, i32 altitudo, i8* indices, Piscina* piscina);
```

Fixed by this plan: `statuae_quadrum_pingere` calls the new entry
internally and its digests do not move; a body's picture is its
projected bounding box grown by `STATUAE_FINIUM_MARGO` and snapped
to the 8-pixel grid in world pixels (so its dither does not swim as
the view slides); a body pixel is kept when its depth is nearer than
the ground's at that pixel; bodies are laid far to near; a body
wholly outside the picture is skipped before rendering; the body's
pose is clip `cyclus_ambulandi` at `tempus_motus` while `movetur`,
else `cyclus_quietis`.

- [ ] First, measure (scratch): exemplaria on the mushroom man and
  the golem at 40 ppu, walking, per body, and eight bodies in one
  frame. Report to Fran against the 10 ms / 16 ms targets before
  going on (spec IV, "How many bodies").
- [ ] Tests:
  - "larva" (statuae_quadrum): pixels outside the mask keep a
    sentinel; the existing "fixa" digests unmoved.
  - "forma": golem -> walk clip 0, no idle; mushroom man ->
    `walk_bouncy`; a model with no clips -> -1, -1; scale makes the
    rest pose 2 units tall (within 1 %).
  - "medium": the followed body's feet project to the picture's
    centre column and a fixed row (within 1 pixel).
  - "occultum": a body behind the raised floor of T4's level
    contributes no pixel; the same body in front does.
  - "ordo": two bodies overlapping on screen: the nearer one's
    pixels win.
  - "extra": the followed body at the level's corner: outside is
    `fundus`; 0x0 and 1x1 frames return VERUM.
  - "quies": a frame with a still body twice gives identical bytes.
- [ ] Red, implement, green.
- [ ] Plants: (a) skip the depth test -> "occultum" red; (b) draw
  near to far -> "ordo" red; (c) write outside the mask -> "larva"
  red.
- [ ] examen, formator, lint.

### Task 8: places in the store

DONE 2026-11-10. The mark is a `"genus":"locus"` field in the act; a
character's act is unchanged.

**Files:** `include/statuae_repositorium.h`,
`lib/statuae_repositorium.c`,
`probationes/probatio_statuae_repositorium.c`, `tools/statuae.c`.

**Produces:**

```c
nomen enumeratio {
    STATUAE_GENUS_STATUA = ZEPHYRUM,
    STATUAE_GENUS_LOCUS
} StatuaeGenus;

b32 statuae_addere_generis (Volumen* volumen, chorda titulus,
    chorda octeti, StatuaeGenus genus);
Xar* statuae_enumerare_generis (Volumen* volumen, StatuaeGenus genus,
    Piscina* piscina);
```

and `statuae addere <file> -volumen V -locus`, `statuae enumerare
-volumen V -loci`.

Fixed by this plan: `statuae_addere` and `statuae_enumerare` keep
their meaning (characters only); an entry written before this task
has no mark and is a character; titles are unique across both kinds;
`removere` works on either. How the mark is stored is the builder's
choice after reading the existing act format.

- [ ] Tests: "genera" (one of each; each listing shows only its
  kind; the old listing shows only the character); "vetus" (an entry
  written through the old call lists as a character); "titulus"
  (same title in the other kind refused); tool round trip with
  `-locus` on a temporary volume.
- [ ] Red, implement, green.
- [ ] Plant: list without filtering -> "genera" red.
- [ ] Rebuild `./tools/statuae_struere.sh`; examen, formator, lint.

### Task 9: the pane library

DONE 2026-11-10. Pane definition `apps/mundus/organum.stml` (one
persistent field, `persona`). The one-second "no key event" guard was
dropped (reason in `lib/mundus_latus.worklog.md`); the pinned start
picture is left to T10's screenshot.

**Files:** create `include/mundus_latus.h`, `lib/mundus_latus.c`,
`probationes/probatio_mundus_latus.c`.

**Consumes:** T1, T4-T8; modelled on `lib/statuae.c` (mount, tick,
`mora`, image source, header row, volume poll). **Produces:**
`MundusLatusMontatio`, `mundus_latus_montare`, `_dimittere`,
`_componere`, `_pulsare`, `_mora`, `_imago_fons`, `_titulus`,
`_focus_amissus`, with the same parameter shapes as their `statuae_`
counterparts.

Fixed by this plan:
- Keys: W A S D set and clear four held flags on press and release;
  intent = the screen direction of the held flags (W up, S down, A
  left, D right; opposite keys cancel) turned into world x/z by the
  view's yaw. `[` and `]` change character; the new one stands where
  the old one stood.
- The held flags are cleared by `_focus_amissus`, and as a guard
  when no key event has arrived for 1 s while the OS should be
  repeating (decided in T1's worklog; drop the guard if releases
  proved reliable).
- `mora`: 33 ms while any flag is held or the body moved in the last
  step or an idle clip is playing; otherwise the volume poll only.
- Place: the first entry of kind LOCUS; character list: kind STATUA;
  both re-read when the volume's sequence changes.
- Start: `mundus_solum_initium`. Messages for: no place (names
  `statuae addere ... -locus`), place refused (title and reason), no
  walkable cell, no characters.
- The frame is recomputed only when the world, the character or the
  size changed.

- [ ] Tests (headless, temporary volume holding T4's level as a
  place and the two fixture characters, injected clock):
  - "ambulat": hold `d` for 1 s of ticks: the body's world position
    changed by about 4 units in the screen-right direction; release:
    it stops and `mora` returns the poll interval.
  - "focus": hold `w`, call `_focus_amissus`: stopped, no ticks.
  - "oppositae": `a` and `d` together: no movement.
  - "persona": `]` switches model; position kept; wraps around.
  - "vacuum": no place; refused place; no characters: each message,
    arrows harmless, no crash.
  - "magnitudo": 0x0, 1x1 and odd sizes.
  - "imago": a pinned picture of the start view, after Fran has
    looked at it.
- [ ] Red, implement, green.
- [ ] Plants: (a) ignore releases -> "ambulat" red; (b) do not clear
  on focus loss -> "focus" red.
- [ ] examen, formator, lint.

### Task 10: into vicus

**Files:** `lib/vicus_applicatio.c` (command `world` -> pane
`mundus`, mount and describe functions beside the statuae ones,
`focus_amissus` wired), the ludus_tessera and tessera library lists,
`tools/statuae_struere.sh` if its libs changed.

- [ ] Test in the vicus application suite: `$world` opens the pane;
  a headless `-imago` screenshot shows the clearing and a character.
- [ ] Plant: unregister the command -> red.
- [ ] `./tessera/compile_probationes.sh`,
  `./ludus_tessera/compile_probationes.sh`.
- [ ] With Fran's word (given 2026-11-10, confirm at the moment):
  add the clearing to his live volume with `-locus`.
- [ ] Fran runs `./apps/vicus/vicus.sh` from secunda and walks.
  Record what he sees in spec section VIII (results): speed, size,
  seams, hidden-behind-cliff, exemplaria on the small character.
- [ ] Update `statuae-project.md` memory pointer and the spec status.
