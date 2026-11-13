# graphis + scaena — implementation plan

> **For agentic workers:** house execution, not the skill default:
> inline in the secunda worktree, one task per turn, Fran approves each
> task's header text before code, commit only on Fran's word through
> the commissio script. Steps use checkbox (`- [ ]`) syntax.

**Goal:** Claude writes a still picture as a short text file, sees it
on a sheet, and builds a two-view hut into Fran's volume, where he
opens it with `$scene`: picture on top, text below, things to click.

**Architecture:** a compiler from an STML description of flat shapes
and lights to a 16-colour picture plus an owner map (graphis); a
sheet for looking; a scene file that gives a picture its texts and
exits, a built blob, and a store of its own in the volume (scaena);
a vicus pane that shows one scene and follows exits.

**Tech stack:** C89 + latina.h; house libraries only (`stml`,
`exemplaria`, `dithering`, `imago_png`, `volumen`, `mandatum`).

**Spec:** `project-specs/graphis-spec.md` (slice, names and layout
agreed by Fran 2026-11-13). Read it first; this plan argues from it.
The look to match is `build/spike2d/sheet_d.png`, right panel
(not in git; the spike source is `build/spike2d/pictura.py` and
`casa.xml`).

## Global constraints

- All of the "Global constraints" of `project-specs/plastica-plan.md`
  that are not about models hold here unchanged: Latin identifiers
  and the vocabulary lint at NOVA 0; new C files through
  `./silva/scribe.sh`; `examen` ACCIPE and `formator` CONFORMIS at
  72 columns; `i32` unsigned, `chorda` not NUL-terminated; arenas
  only; TDD with every gate proved red by a compiling plant; new
  code in new modules; both `tessera` and `ludus_tessera` runners
  before a commit; tests write only under `credo_area(c)` and close
  temporary volumes.
- graphis (`graphis*`) includes no `scaena*`, `mundus*`, `statua*`
  or vicus header. scaena may include graphis.
- **No floating point** anywhere between a description and a
  picture (spec 14). Light is 8.8 fixed point per channel.
- Same source, same bytes: no clock, address or random number
  reaches a picture, an owner map or a blob.
- Picture size is exactly 360 x 240 (`GRAPHIS_LATITUDO`,
  `GRAPHIS_ALTITUDO`); a description that says otherwise is refused.
- The dither is `dithering_atkinson_colorum` unchanged; its pinned
  digests must not move.
- Type names `Scaena` and `ConformitasScaena` are taken (spec VI.6).
- Only `bin/scaena condere`, run by Claude after T7, writes into
  Fran's live volume (standing OK, 2026-11-11).
- No raw IO outside `filum_*` / `lectiones_*` (examen 96/97).

## Review focus

1. A broken description: text that is not STML, an unknown element,
   attribute, pattern name or colour, a missing or non-numeric
   number, a polygon with fewer than 3 points, a zero-size shape.
   Refused with a reason and a line; never a crash, never half a
   picture. (T1)
2. Shapes partly or wholly off the frame, negative coordinates, a
   line thicker than the frame: clipped without writing outside the
   buffers; wholly outside is refused. (T1)
3. A titled shape painted over until it owns fewer than 64 pixels,
   or two shapes with one title: refused, naming the title. (T1)
4. `usus` of a missing piece, a piece that uses itself (directly or
   through another), a refusal inside a piece: refused, naming the
   piece file and its line. (T3)
5. A scene whose picture lacks a title the scene names, or has a
   title the scene does not mention; an exit to a scene that does
   not exist; a blob cut short or with a wrong first word: the
   build names it and writes nothing; the reader refuses, the pane
   shows a plain message. (T6, T7, T8)

## Files

| file | holds |
|---|---|
| `include/graphis.h`, `lib/graphis.c` | description -> picture, owner map, titles |
| `include/graphis_lumen.h`, `lib/graphis_lumen.c` | lights in fixed point |
| `include/graphis_specimen.h`, `lib/graphis_specimen.c` | the sheet and the crop |
| `tools/graphis.c`, `tools/graphis_struere.sh`, `tools/graphis_fumus.sh` | `bin/graphis` |
| `graphis/res/*.stml` | pieces placed with `usus` |
| `include/scaena.h`, `lib/scaena.c` | scene file, checks, blob |
| `include/scaena_repositorium.h`, `lib/scaena_repositorium.c` | store in the volume |
| `tools/scaena.c`, `tools/scaena_struere.sh`, `tools/scaena_fumus.sh` | `bin/scaena` |
| `lib/scaena_latus.c`, `include/scaena_latus.h`, `apps/scaena/organum.stml` | the pane |
| `apps/scaena/loci/*.stml` | pictures and scene files of the hut |
| `probationes/probatio_graphis*.c`, `probatio_scaena*.c` | tests |
| `aedificatio.stml` | an `<actio>` for each new tool |

Each new `.c` gets a worklog beside it when there is something to
record.

## The description, fixed here so every task agrees

```
<pictura titulus="hut, hearth wall" lux="0.5" umbra="0.35">
  <rectangulum fines="64 34 232 116" colores="9 15"
               exemplar="horizontalis_latus" vis="0.45"
               stilus="1 0"/>
  <polygonum puncta="0,0 64,34 64,150 0,204" colores="1 9"
             exemplar="horizontalis_latus" stilus="1 0"/>
  <ovum fines="96 186 150 40" colores="6 15"
        exemplar="rhombus_lineatus" vis="1" stilus="2 6"/>
  <linea puncta="64,34 296,34" stilus="3 1"/>
  <rectangulum titulus="window" fines="222 54 58 50"
               colores="5 7" exemplar="cinereum_leve" vis="1"
               lucens="verum"/>
  <grex locus="196 150"> ... </grex>
  <usus fons="sella" locus="52 168"/>
  <lumen locus="170 140" radii="75 58" color="#ffb873" vis="0.85"/>
  <lumen area="214,156 286,156 322,198 226,198" mollities="3"
         color="#fff0d8" vis="0.55"/>
</pictura>
```

- Coordinates are pixels, x right, y down, origin top left, and
  are integers (a decimal point is refused). Strengths (`vis`,
  `lux`, `umbra`) are decimals with at most three places, which
  the reader turns into fixed point (1 = 256) without floating
  arithmetic. `grex` and `usus` add
  their `locus` to everything inside.
- `colores`: one or two colours, each a palette index `0`..`15` or
  `#rrggbb`. One colour: flat. Two: first is the pattern's set
  bits, second its clear bits; `-` as the second means "leave what
  is underneath" (an overlay).
- `exemplar`: a name from the table in `graphis.h`, which is the
  enum of `include/exemplaria.h` in lower case without the prefix
  (`latericium`, `textum`, `tessellatum`, ...). Default `solidus`.
- `vis` on a shape: `0`, or `0.4` .. `1`; default `0.6` when two colours and a pattern are
  given, else 0. Between 0 and 0.4: refused (spec 17).
- Source colour of a pixel = flat x (1 - vis) + pattern pixel x vis,
  where flat = the two colours mixed by the pattern's density
  (set bits / 64).
- `stilus="width colour"`: outline (or the line itself), drawn after
  the fill, never patterned. `titulus`: the shape and its outline
  own their pixels in the owner map. `lucens="verum"`: the shape
  takes light factor 256 exactly.
- Root: `lux` = ambient, 0 .. 1 (1 = full, no change; default 1);
  `umbra` = how much of the ambient is lost at the corners, 0 .. 1
  (default 0), falling
  with the squared distance from the centre; `tinctura="#rrggbb"`
  tints the ambient (default white).
- `lumen locus radii`: adds `vis` (0 .. 2, 1 = one full light) x
  `color` x g, where g is a Gaussian of the distance scaled by the
  two radii, read from a 65-entry integer table (256 x exp(-t/8),
  t = 0..64), linear between entries, 0 beyond.
- `lumen area`: adds `vis` x `color` inside the polygon; `mollities`
  n softens the edge by averaging the inside test over the pixel
  and its four neighbours at distance n.
- Light per channel = ambient + sum of lights, in 8.8; the lit
  source = source x light >> 8, clamped to 255.
- Later shapes paint over earlier ones. Lights are not shapes: their
  order does not matter and they own no pixels.

---

### Task 1: the compiler, flat shapes

**Files:** create `include/graphis.h`, `lib/graphis.c`,
`probationes/probatio_graphis.c`.

**Interfaces:**
- Produces:
  ```c
  #define GRAPHIS_LATITUDO   CCCLX
  #define GRAPHIS_ALTITUDO   CCXL
  #define GRAPHIS_TITULI_MAXIMI  CCLV

  nomen structura {
      i8* fons;      /* rgba, IV octeti per pixelum: ante lucem */
      i8* larva;     /* vis per pixelum, 0..255 */
      i8* domini;    /* 0 = nullus; n = tituli[n - I] */
      chorda* tituli;
      i32 titulorum;
  } GraphisTabula;

  nomen structura {
      b32 successus;
      GraphisTabula* tabula;  /* NIHIL si recusata */
      i8* indices;            /* palettae, unus per pixelum */
      chorda error;
      i32 linea;
  } GraphisFructus;

  GraphisFructus graphis_legere (chorda textus, Piscina* piscina);
  constans character* graphis_exemplar_titulus (i32 id);
  ```
- [x] Tests first (suite `graphis`), red:
  - `quadratum`: one flat palette rectangle on the default ground;
    exact pixel counts of the colour and of the ground.
  - `exemplar_purum`: full `lux`, palette colours, `vis="1"`:
    every pixel equals the pattern's own pixel (`exemplar_punctum`).
  - `vis_nulla`: `vis="0"` on a palette colour: one index everywhere
    in the shape.
  - `ordo`: a later shape covers an earlier one; an overlay (`-`)
    leaves clear bits untouched.
  - `domini`: titled shape and its outline own their pixels;
    untitled own none; a covered part is not owned.
  - `ovum_et_linea`: oval pixel count and symmetry; a 3-wide line's
    count.
  - `praecisio`: a shape half off the frame paints only inside.
  - `recusationes`: one fixture per refusal of Review focus 1-3 and
    of spec 17 (incl. size not 360 x 240, `vis="0.2"`, more than
    255 titles), each with its line.
  - `determinata`: two runs, same bytes; digest pinned.
- [x] Implement: read STML; scanline polygon fill (pixel centres),
  oval by rows, line by stamped squares, as the spike; refusals in
  the manner of `lib/plastica.c:153-215` and `:454-481` (copied,
  first refusal wins); in this task light is full ambient only, then
  `dithering_atkinson_colorum`.
- [x] Plants: pattern bit inverted; owner written for untitled
  shapes; clip test dropped; overlay paints clear bits.
- [x] Lint, examen, formator, `compile_tests_fontes_generare.sh`.

**As built (2026-11-13):** 10 sections, 9 plants red (bit inverted,
outline owns nothing, outline clip loosened, overlay paints clear
bits, region minimum off, weak strength accepted, oval too wide,
group offset dropped, duplicate title accepted). The source table is
RGBA (what Atkinson takes). `plagula` joins `GraphisFructus` in T3,
not here. An untitled overlay leaves the owner map alone (a light
patch must not punch holes in a region). One pass, not two: a shape
is checked and painted at once and any refusal drops everything.
The oval's outline is the oval minus the oval shrunk by the width
(no floats). STML returns an attribute with an empty value as
absent; `_attributum_scriptum` tells the two apart for `titulus`.
A throwaway viewer is `build/spike2d/spica_graphis.c`.

### Task 2: lights

**Files:** create `include/graphis_lumen.h`, `lib/graphis_lumen.c`,
`probationes/probatio_graphis_lumen.c`; modify `lib/graphis.c` (the
call only).

**Interfaces:**
- Consumes: `GraphisTabula`.
- Produces:
  ```c
  nomen structura {
      b32 punctum;          /* VERUM: locus et radii; FALSUM: area */
      s32 x, y, radius_x, radius_y;
      s32* puncta; i32 punctorum; i32 mollities;
      i8 color[III];
      i32 vis;              /* 0..512; 256 = 1 */
  } GraphisLumen;

  nomen structura {
      i32 lux, umbra;       /* 0..256 */
      i8 tinctura[III];
      GraphisLumen* lumina; i32 luminum;
  } GraphisLux;

  /* fons illustratus: rgb, III octeti per pixelum */
  i8* graphis_illustrare (constans GraphisTabula* tabula,
      constans GraphisLux* lux, constans i8* lucentes,
      Piscina* piscina);
  ```
  `GraphisFructus` gains `i8* illustratus` (the lit source, kept for
  the sheet).
- [x] Tests first: `plena` (lux 1, no lights: lit = source, byte
  for byte); `umbra` (centre pixel unchanged, corner darker by the
  stated amount); `punctum` (value at the centre, at one radius, and
  0 beyond the table; wider along x when `radius_x` is larger);
  `area` (inside, outside, and three steps across a soft edge);
  `lucens` (a lit-off shape keeps its source under any light);
  `saturatio` (two strong lights clamp at 255, no wrap);
  `recusationes` (radius 0, `vis` out of range, area under 3
  points, `lux` over 1, four decimal places); `casa_lux` (digest of a small fixture
  with all four kinds).
- [x] Implement in integers only; the Gaussian table is a literal.
- [x] Plants: clamp removed; `lucens` ignored; radii swapped; `umbra`
  applied to lights too.
- [x] Check by eye: port `build/spike2d/casa.xml` by hand into a
  scratch description and compare with `sheet_d.png` (a quick PNG
  through a throwaway caller; the real sheet is T4). Record in the
  worklog what differs from the float spike.

**As built (2026-11-13):** suite `graphis_lumen`, 10 sections, 10
plants red. Light is counted in 4096ths (`GRAPHIS_LUX_UNA`), not
256ths: smoother gradients before the dither. `graphis_illustrare`
takes raw tables (`fons`, `lucentes`, width, height, `GraphisLux`),
so `graphis_lumen.h` does not depend on `graphis.h`; `graphis.h`
includes it, `GraphisTabula` gains `lucentes` and `lux`,
`GraphisFructus` gains `illustratus`, and Atkinson now runs on the
lit source. Strengths in the description are decimals (`lux`,
`umbra` 0..1; light `vis` 0..2). A light must be a direct child of
`<pictura>`; a later shape that is not `lucens` covers one that is.
The eye check: the spike's hut converted to the real format
(`build/spike2d/casa.stml`) matches `sheet_d.png` - the spike's
constants are reproduced by `lux="0.55" tinctura="#d1dcff"
umbra="0.35"`, a point light `radii="75 58"` and an area light.
With default light a picture is byte for byte what T1 made (T1
digests unmoved).

### Task 3: pieces (`usus`)

**Files:** modify `include/graphis.h`, `lib/graphis.c`; create
`probationes/probatio_graphis_usus.c`, `graphis/res/sella.stml`.

**Interfaces:**
- Produces:
  ```c
  /* redde textum partis; chorda vacua (datum NIHIL) = non inventa */
  nomen chorda (*GraphisFons) (chorda titulus, vacuum* contextus,
      Piscina* piscina);

  GraphisFructus graphis_legere_cum_fonte (chorda textus,
      GraphisFons fons, vacuum* contextus, Piscina* piscina);
  ```
  A piece file has the root `<pars titulus="...">` and holds shapes
  and groups only (no lights, no `titulus` on shapes, no size).
  `graphis_legere` = no source: any `usus` is refused.
- [ ] Tests first: `pars` (a piece placed twice at two offsets
  equals the same shapes written out); `pars_in_parte` (nesting to
  depth 2); `recusationes`: missing piece, a piece that uses
  itself, a cycle of two, depth over 8, a light or a titled shape
  in a piece, a refusal inside a piece (`plagula` = the piece's
  name, `linea` = its line).
- [ ] Implement; the library does no IO (the caller's callback
  does).
- [ ] Plants: cycle check removed (must end by the depth limit, not
  hang: run under CREDO_NON_PENDET); offset not added.

### Task 4: the sheet and `bin/graphis`

**Files:** create `include/graphis_specimen.h`,
`lib/graphis_specimen.c`, `probationes/probatio_graphis_specimen.c`,
`tools/graphis.c`, `tools/graphis_struere.sh`,
`tools/graphis_fumus.sh`; modify `aedificatio.stml`.

**Interfaces:**
- Produces:
  ```c
  /* scheda: fons, larva, illustratus, pictura, regiones; II x */
  Imago graphis_specimen_pingere (constans GraphisFructus* fructus,
      Piscina* piscina);
  /* pars picturae finitae, IV x; fines intra picturam */
  Imago graphis_quadrum_pingere (constans GraphisFructus* fructus,
      s32 x, s32 y, s32 latitudo, s32 altitudo, Piscina* piscina);
  ```
  Commands: `bin/graphis specimen F -o PNG`, `bin/graphis quadrum F
  -o PNG -fines "x y w h"`, `bin/graphis iudicare F...` (prints
  `ACCIPE` or `F:linea: reason`, exit 1 on any refusal). Pieces are
  read from `graphis/res/` or `-fontes DIR`. The sheet prints the
  titles with their pixel counts on stdout.
- [ ] Tests first: sheet size and that each of the five panels is
  where the header says (a known pixel in each); the regions panel
  gives each title its own colour; crop refuses bounds outside.
- [ ] Smoke script: sheet and crop of a fixture exist and are PNG;
  `iudicare` exits 1 on a broken fixture and names the line.
- [ ] Hand plants on the tool (mutate, rebuild, smoke, restore).

### Task 5: the hut, two views

**Files:** create `apps/scaena/loci/casa_focus.stml`,
`apps/scaena/loci/casa_ostium.stml`, pieces in `graphis/res/`
(`sella`, `mensa`, `vasa`), `probationes/probatio_graphis_casa.c`.

- [ ] View one (hearth wall): the spike's hut ported to the real
  format, lights from the description. View two (door wall): the
  room turned round - door, a bed, a coat on a peg, a small window
  on the side wall, the same table seen from the other side.
- [ ] Look at both sheets; revise until they match the chosen look.
  Fran's word on the hut: keep or change as Claude judges.
- [ ] Test `casa`: both compile; titles pinned (`hearth`, `window`,
  `jars`, `book`, `turn` in view one; `door`, `bed`, `coat`, `turn`
  in view two); digests pinned.

### Task 6: scenes

**Files:** create `include/scaena.h`, `lib/scaena.c`,
`probationes/probatio_scaena.c`,
`apps/scaena/loci/casa.scaena.stml`.

**Interfaces:**
- Scene file (one file may hold several views):
  ```
  <scaenae>
    <prospectus titulus="hut-hearth" pictura="casa_focus">
      <descriptio>One paragraph.</descriptio>
      <res titulus="hearth">Text on click.</res>
      <res titulus="turn" exitus="hut-door">Text.</res>
    </prospectus>
  </scaenae>
  ```
- Produces:
  ```c
  nomen structura {
      chorda titulus, textus, exitus;   /* exitus vacuus = nullus */
  } ScaenaRes;

  nomen structura {
      chorda titulus, descriptio;
      i8* indices; i8* domini;          /* 360 x 240 */
      ScaenaRes* res; i32 rerum;        /* ordine dominorum */
  } ScaenaProspectus;

  nomen structura {
      b32 successus; ScaenaProspectus* prospectus; i32 numerus;
      chorda error; chorda plagula; i32 linea;
  } ScaenaFructus;

  ScaenaFructus scaena_struere (chorda textus, GraphisFons picturae,
      vacuum* contextus, Piscina* piscina);
  chorda scaena_scribere (constans ScaenaProspectus* p,
      Piscina* piscina);
  b32 scaena_legere (chorda massa, ScaenaProspectus* exitus,
      Piscina* piscina);
  #define SCAENA_EXITUS_MUNDUS "mundus"
  ```
  Blob: the word `SCAENA01`, then counted fields in a fixed order,
  numbers big-endian; `res` order equals owner numbers.
- [ ] Tests first: build of a two-view fixture; write, read back,
  equal field for field; the refusals of Review focus 5 (title
  missing either way, exit to nowhere, `mundus` accepted, duplicate
  view title, empty description); reader refuses a short blob, a
  wrong first word, a count past the end.
- [ ] Plants: exit check removed; reader's bound check removed (the
  short-blob test must fail, not crash: CREDO_NON_RUIT).

### Task 7: the store and `bin/scaena`

**Files:** create `include/scaena_repositorium.h`,
`lib/scaena_repositorium.c`,
`probationes/probatio_scaena_repositorium.c`, `tools/scaena.c`,
`tools/scaena_struere.sh`, `tools/scaena_fumus.sh`; modify
`aedificatio.stml`.

**Interfaces:**
- Produces (acts `scaena-addita` / `scaena-remota`, modelled on
  `include/statuae_repositorium.h`):
  ```c
  b32 scaena_addere (Volumen* v, chorda titulus, chorda massa);
  b32 scaena_removere (Volumen* v, chorda titulus);
  Xar* scaena_enumerare (Volumen* v, Piscina* piscina);
  chorda scaena_promere (Volumen* v, chorda titulus,
      Piscina* piscina);
  ```
  Command: `bin/scaena condere -volumen V [-fontes DIR]` builds
  every `*.scaena.stml` under `apps/scaena/loci/`; prints `addita`,
  `mutata`, `eadem` per view; never removes; writes nothing if any
  view is refused. `bin/scaena removere -volumen V TITULUS`.
- [ ] Tests first: add, list, fetch, replace, remove in a temporary
  volume; a statuae listing of the same volume does not see scenes,
  and the reverse.
- [ ] Smoke: build the hut into a temporary volume twice (second
  run all `eadem`); a broken scene writes nothing.
- [ ] Run `bin/scaena condere` on Fran's live volume (standing OK).

### Task 8: the pane `$scene`

**Files:** create `include/scaena_latus.h`, `lib/scaena_latus.c`,
`apps/scaena/organum.stml`, `probationes/probatio_scaena_latus.c`;
then `./tools/organa_infigere.sh`.

- Read first: `lib/mundus_latus.c` (picture through the `fons`
  callback `:683-696`, wiring at `lib/vicus_applicatio.c:3233`,
  wrapped text `:918-962`), `lib/negotia_visus.c:1322-1326` (hover).
- Behaviour: header row (12 rows) shows the view's title, and the
  title of the region under the pointer; the picture below it, 360
  x 240, centred across; the text band under the picture: the
  description, a blank line, then the last clicked thing's text.
  Click on a region with an exit to another view: go there, clear
  the clicked text. Exit `mundus`: text only (spec 23). No scenes
  in the volume: one plain line saying how to build them. No tick
  is asked for; the pane redraws on events only.
- Opens on the first view by title; `[` and `]` step through views
  (for looking at everything without finding the exits).
- [ ] Tests first (headless, `-imago` as in statuae and mundus):
  the picture's known pixel is where expected; click on `hearth`
  puts its text in the band; click on `turn` changes the view;
  hover sets the header; empty volume shows the message; a pane
  narrower than 360 crops and does not write outside.
- [ ] Plants: click coordinates not made local
  (`des->punctum_locale`); exit followed without clearing the text.
- [ ] Both subsystem runners; screenshot read by Claude.

### Task 9: finish

- [ ] Rebuild the hut into the live volume; Fran restarts vicus from
  secunda and looks.
- [ ] Spec: status and an "as built" note per task; memory
  (`graphis-project.md`); worklogs.
- [ ] File in the ledger: the `$world` doorway (needs its own small
  design), the walked perspective mode, animation, a "room" helper.
