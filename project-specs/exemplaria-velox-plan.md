# exemplaria velox Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. House rule: executed INLINE, one task per turn; Fran approves each task's header text before code.

**Goal:** A second exemplaria dither, `dithering_exemplaria_velox_colorum`, with the same look as classic and at least 12 fps (<= 83 ms) at 600x751 on the golem, so the 3D pane can try it per frame.

**Architecture:** Start from an exact copy of classic in its own file, behind a harness that pins classic's output and times both. Then rebuild the copy step by step on data-oriented lines: shared pattern sources and build-time tables, f32 planes per channel, bitsets, cheaper maths, then the hot layers one at a time. Classic's output never changes; after each task Fran gets the timing table and side-by-side PNGs.

**Tech Stack:** C89 house libraries: dithering, dithering_exemplaria, color, imago + imago_png + imago_opus, blake2b, credo II (suite fixture), aedilis (measuring tool), fabrica generator action (GENERATUM tables).

**Spec:** `project-specs/exemplaria-velox.md` (decisions II.1-7; approved by Fran 2026-11-07).

## Global Constraints

- C89, house flags (`tools/vexilla.sh`: -std=c89 -pedantic ... -O2 -g); 72 columns; latina.h macros never identifiers; i32 UNSIGNED (s32 for signed maths); -Wcast-qual.
- f32 storage, but C89 has no `sqrtf`/`expf`/`powf`: call the f64 functions with casts, or hand-written steps (cube root, task 3).
- Latin identifiers, `VOCABULA_VIAE_ADDITAE="${F[*]}" ./oratio/vocabula.sh -nova` NOVA 0. Names below are proposals until each task's header is approved.
- Per file: `./silva/examen.sh` ACCIPE; `./silva/formator.sh -scribere` on the in-tree path.
- Gates born red by a COMPILING plant (`silva.planta(via, vetus, novus, 'radix', filtrum=..., tolerans='spatia')`), backups in `build/plantae_*`, `cmp` after.
- Classic's OUTPUT never changes (decision 2): pinned by blake2b of its indices on both fixtures from task 1 on.
- Same signature and types as classic (spec IV): `DitheringExemplariaOptiones`, `DitheringExemplariaCensus`, `DitheringFructus`; `optiones == NIHIL` means `dithering_exemplaria_optiones_ordinariae`.
- No caches: no `hic_manens` mutable state, nothing kept between calls (anti-goal). Constant tables only from the generator.
- Tests write only in `credo_area(c)`. Timings and comparison PNGs come from the measuring tool (below), never from the suite - the suite's area is deleted after the run.
- Target: golem fixture, 600x751, default options, all 16 colours: <= 83 ms (stretch <= 42 ms). Photos must speed up at least as much (decision 5).
- Commit only on Fran's word, via the commissio script; after each commit `bin/fabrica sanare build/speculum/hospes/capsula_speculi_hospes.c` and `./tools/generata_iudicare.sh`.

**Builder's calls in this plan (Claude, 2026-11-07), for Fran to check:**
- a. Spec V put timings and PNGs in the credo suite; the house rule says tests write only in their deleted area. So timings and PNGs come from a measuring tool, `tools/exemplaria_mensura.c` (built with aedilis like `tools/xar_mensura.c`); the suite keeps the correctness checks.
- b. Task order swaps spec VII.2 and VII.3: the build-time tables come first, because with f64 tables the copy stays byte-identical to classic, so the move is proved exactly before any drift begins.
- c. Classic's ten synthetic test sections move into a shared test header and run against both functions, so the fast version passes everything classic passes (including "lateres >= 95 %"), without copying 300 lines.
- d. The "velox == classic" check holds through task 2 and is retired in task 3, when f32 starts the allowed drift (decision 1); from then on the PNGs judge.

## Review Focus

1. **Hidden state between calls** (a cache slipping in): dithering image A, then B, then A again gives A's first result exactly -> T1 "sine memoria inter vocationes".
2. **Alpha** is ignored by classic (it reads only RGB): the same RGB with alpha 0 and 255 gives identical indices -> T1 "alpha neglecta".
3. **Sizes at the blur's edge** (smaller than the kernel, thin strips, odd widths): no crash, only active colours -> T4 "magnitudines oris".
4. **Few active colours** (fewer than the candidate count; 1, 2, 3 colours): every pixel uses an active colour and pure palette colours stay pure -> T5 "pauci colores".
5. **Memory** at pane size: f32 planes and bitsets must use less arena than classic, not more -> T3 "memoria minor".

---

### Task 1: harness - fixtures, shared sections, fast = copy, measuring tool

**Files:**
- Create: `probationes/fixa/exemplaria/lapis_600.png` (golem, 600x751), `probationes/fixa/exemplaria/pratum_600.png` (meadow, 600x800), `probationes/fixa/exemplaria/generare.sh` (the two `sips` commands from `~/Downloads/stone-golem.png` and `~/Downloads/test-image.png`; frozen copy, header says so)
- Create: `probationes/dithering_exemplaria_commune.h` (synthetic images, `_omnes_activi`, the ten sections, moved from the classic suite)
- Modify: `probationes/probatio_dithering_exemplaria.c` (includes the header; suite fixture selects classic)
- Create: `include/dithering_exemplaria_velox.h`, `lib/dithering_exemplaria_velox.c` (classic's body under the new name; reuses classic's `dithering_exemplaria_optiones_ordinariae`, does not copy it)
- Create: `probationes/probatio_dithering_exemplaria_velox.c`
- Create: `tools/exemplaria_mensura.c`, `lib/dithering_exemplaria_velox.worklog.md`

**Interfaces:**
- Produces (`include/dithering_exemplaria_velox.h`), exactly classic's shape:

```c
DitheringFructus
dithering_exemplaria_velox_colorum (
                             constans i8* rgba,
                                     i32  latitudo,
                                     i32  altitudo,
                            constans b32  colores_activi[XVI],
    constans DitheringExemplariaOptiones* optiones,
               DitheringExemplariaCensus* census,
                                 Piscina* piscina);
```

- Produces (`probationes/dithering_exemplaria_commune.h`): the function under test arrives through the suite fixture (`c->suita`), wrapped because C89 cannot cast a function pointer to `vacuum*`:

```c
nomen DitheringFructus (*DitheringExemplariaFunctio)(constans i8*,
    i32, i32, constans b32[XVI],
    constans DitheringExemplariaOptiones*,
    DitheringExemplariaCensus*, Piscina*);
nomen structura { DitheringExemplariaFunctio functio; }
    DitheringExemplariaProbandum;
```

  Sections keep their titles and assertions; each calls `((constans DitheringExemplariaProbandum*)c->suita)->functio(...)`. Both suites run them through `credo_suitam_currere_cum` with a `parare_suitae` that sets the wrapper.
- Produces (tool): `./bin/aedilis tools/exemplaria_mensura.c && bash build/aedilis/exemplaria_mensura/struere.sh && ./build/aedilis/exemplaria_mensura/exemplaria_mensura [-n N]` (N default X, average of N calls, `clock()` like xar_mensura). Rows: golem at 360, 600, 720 wide (scaled from the fixture with `imago_scalare`), meadow at 600:

```
imago    magnitudo  atkinson  classica  velox  velocitas  differentia
lapis    600x751      9.4 ms   606 ms   598 ms   1.0x       0.0 %
```

  At 600 wide it writes `build/exemplaria_velox/<imago>_classica.png`, `<imago>_velox.png` and `<imago>_par.png` (classic left, fast right, side by side).

- [ ] **Step 1: Fixtures.** Run `generare.sh`; check both PNGs load (`imago_caricare_ex_file`) at 600x751 and 600x800.
- [ ] **Step 2: Move the sections.** Classic suite reads them from the header. Run `./compile_tests.sh probatio_dithering_exemplaria` -> all ten sections TRANSIIT (pure refactor, same count as before).
- [ ] **Step 3: Pin classic.** Run classic once on each fixture (default options, all 16 colours) and record the blake2b-256 hex of its indices. These two strings go into the velox suite verbatim.
- [ ] **Step 4: Write the velox suite.** The ten shared sections (fixture = velox) plus:

```c
/* "classica fixa": blake2b(classic indices) on each fixture
 *   == the hex recorded in step 3 */
/* "par classicae": memcmp(velox, classic) == 0 on each fixture */
/* "alpha neglecta": scaena 96x72, alpha 255 vs alpha 0 ->
 *   memcmp(indices) == 0 */
/* "sine memoria inter vocationes": a = velox(scaena 96x72),
 *   velox(gradiens rubra 96x64), b = velox(scaena 96x72) ->
 *   memcmp(a, b) == 0 */
```

- [ ] **Step 5: Header + copy.** `dithering_exemplaria_velox.c` = classic's file with the public name changed and `optiones_ordinariae` removed. Run `./compile_tests.sh probatio_dithering_exemplaria_velox` -> all sections TRANSIIT.
- [ ] **Step 6: Plants, one at a time (each must turn its section red, then restore + `cmp`):**
  - velox `CANDIDATI_MAX XII` -> `XI` -> "par classicae" FRACTA
  - classic `POENA_CONTRASTUS 0.004` -> `0.0041` -> "classica fixa" FRACTA
  - velox reads alpha for blue (`rgba[p * IV + k]` -> `rgba[p * IV + k + (k == II)]`) -> "alpha neglecta" FRACTA
  - velox `hic_manens i32` call counter added to `semen` -> "sine memoria inter vocationes" FRACTA
- [ ] **Step 7: Tool.** Build and run; the velox column equals classic within noise, differentia 0.0 %. Show Fran the table and the `_par.png` files.
- [ ] **Step 8: Checks + commit** on Fran's word (examen, formator, vocabula NOVA 0).

---

### Task 2: shared pattern sources + build-time tables

**Files:**
- Create: `include/dithering_exemplaria_fontes.h`, `lib/dithering_exemplaria_fontes.c` (BAYER, PICTOGRAMMATA and the constants the generator needs, moved out of classic)
- Modify: `lib/dithering_exemplaria.c` (reads the shared sources; computes its weights as today), `lib/dithering_exemplaria_velox.c` (reads the generated tables; `_exemplar_finire`, `_bayer_parare`, `_pictogrammata_parare` deleted; sRGB by table)
- Create: `tools/dithering_tabulae_generare.sh`, `tools/dithering_tabulae_generare.c` (pattern after `tools/runae_generare.sh`: writes, or `-probare` compares with `cmp`), `include/dithering_exemplaria_tabulae.h` (hand-written), generated `lib/dithering_exemplaria_tabulae.c` (`GENERATUM` on line 1)
- Modify: `aedificatio.stml` (actio `dithering_exemplaria_tabulae`, genus generator, after `runae_tabulae`; ingressus = the generator .sh/.c, fontes .h/.c, `include/dithering_exemplaria.h`; exitus the .c, provenientia regeneratio)

**Interfaces:**
- Produces (`include/dithering_exemplaria_fontes.h`):

```c
#define DITHERING_TEGULA            VIII
#define DITHERING_GRADUS_MIXTI      XXXII
#define DITHERING_BAYER_NUMERUS     XVI
#define DITHERING_SIGMA_OCULI       1.0
#define DITHERING_SIGMA_OCULI_LATI  2.0
#define DITHERING_PICTOGRAMMATA_FONTIUM  XI
nomen structura {
    DitheringPictogramma  familia;
      constans character* lineae[DITHERING_TEGULA];
} DitheringPictogrammaFons;
extern constans s32 DITHERING_BAYER[DITHERING_TEGULA][DITHERING_TEGULA];
extern constans DitheringPictogrammaFons
    DITHERING_PICTOGRAMMATA[DITHERING_PICTOGRAMMATA_FONTIUM];
```

- Produces (`include/dithering_exemplaria_tabulae.h`), index `[exemplar][y * VIII + x]`, values exactly what classic's `_exemplar_finire` computes today:

```c
extern constans i8  DITHERING_BAYER_ORDINES[XVI][VIII];
extern constans s32 DITHERING_BAYER_DENSITATES[XVI];
extern constans i8  DITHERING_BAYER_PONDERA[XVI][LXIV];
extern constans i8  DITHERING_BAYER_PONDERA_LATA[XVI][LXIV];
extern constans i8  DITHERING_PICTOGRAMMATUM_ORDINES[XI][VIII];
extern constans s32 DITHERING_PICTOGRAMMATUM_DENSITATES[XI];
extern constans i8  DITHERING_PICTOGRAMMATUM_PONDERA[XI][LXIV];
extern constans i8  DITHERING_PICTOGRAMMATUM_PONDERA_LATA[XI][LXIV];
extern constans f64 DITHERING_SRGB_LINEARIS[CCLVI];  /* %.17g; f32 in T3 */
```

- [ ] **Step 1: Move the sources** into fontes; classic includes them. "classica fixa" stays TRANSIIT (the proof that the move changed nothing).
- [ ] **Step 2: Generator + action.** Run it; `./tools/dithering_tabulae_generare.sh -probare` exit 0; `./tools/generata_iudicare.sh` names the new artifact recent.
- [ ] **Step 3: velox reads the tables.** Run the velox suite: "par classicae" stays TRANSIIT (f64 table entries round-trip `color_octetus_ad_linearem` exactly; weights are the same integers).
- [ ] **Step 4: Plants:**
  - shared `DITHERING_BAYER[0][1] 32` -> `33` -> "classica fixa" FRACTA, and `generare -probare` exit 1 (table stale)
  - generator `DITHERING_SIGMA_OCULI` read as `1.1` -> `-probare` exit 1, generata gate FRACTA
- [ ] **Step 5: Tool row** (expect a few ms saved: no `exp`, no `pow` for sRGB). Checks + commit on Fran's word.

---

### Task 3: data layout - f32 planes, bitsets, cube root

**Files:** Modify `lib/dithering_exemplaria_velox.c`, `tools/dithering_tabulae_generare.c` (sRGB table as f32), `include/dithering_exemplaria_tabulae.h`, the generated .c; test `probationes/probatio_dithering_exemplaria_velox.c`.

**Interfaces:** Public API unchanged. Inside velox:
- every per-pixel buffer = one `f32*` plane per channel (lin r/g/b; L/a/b of source, eye, wide eye; structure energy/coherence/angle; error planes);
- masks (`hachuratus`, `activus`, `eligibilis`, `larva`, `servatus`) = bitsets, `i32` words, bit `p & XXXI` of word `p >> V`;
- active colours = one 16-bit mask; `Opus.mixtio` = f32, built only for active pairs;
- OKLab without `pow`: cube root by `frexp` guess + two Newton steps `y -= (y*y*y - a) / (3*y*y)`, `0 -> 0`; relative error < 1e-5 on [1e-6, 1] (OKLab's LMS inputs are never negative for RGB in [0, 1]).

- [ ] **Step 1: Retire "par classicae"** (decision 1: drift allowed from here) and add:

```c
/* "colores puri": per index i in 0..XV, a flat 16x16 image of
 *   AQUINAS_PALETTE[i], all colours active, default options ->
 *   every index == i (16 sub-checks, the failing index named) */
/* "memoria minor": on lapis_600, piscina_summa_usus after velox
 *   < piscina_summa_usus after classic (fresh arenas each) */
```

  On the current copy "colores puri" passes (a guard for the rebuild) and "memoria minor" is FRACTA (memory is equal) - that is its red.
- [ ] **Step 2: Rebuild the layout**; run the velox suite -> all TRANSIIT, "classica fixa" untouched.
- [ ] **Step 3: Plants:**
  - OKLab writes a into the b plane -> "colores puri" FRACTA (chromatic colours)
  - bitset `p >> V` -> `p >> VI` in the `activus` test -> shared sections FRACTA
- [ ] **Step 4: Tool table + PNGs to Fran** (first drift). Checks + commit on Fran's word.

---

### Task 4: blur

**Files:** Modify `lib/dithering_exemplaria_velox.c`; test the velox suite.

**Interfaces:** `interior vacuum _gauss_planum (constans f32* fons, f32* meta, f32* tmp, s32 lat, s32 alt, f64 sigma)`: separable, one plane, f32; kernel weights computed once per call into a stack array (radius as classic); rows and columns split into interior (no clamp) and edge strips (clamp, as classic). Callers blur only the planes they read.

- [ ] **Step 1: Add "magnitudines oris"**: sizes {XII,XII}, {XIII,XIII}, {XXV,III}, {III,XXV}, {DCI,II} with the scaena image -> success, only active colours. Passes on today's blur (guard for the split).
- [ ] **Step 2: Implement**; velox suite all TRANSIIT.
- [ ] **Step 3: Plants:** kernel not normalised (skip the divide) -> "colores puri" FRACTA; interior loop bounds one too wide on the right -> "magnitudines oris" FRACTA or ABORTA (an out-of-row read on {XIII,XIII}).
- [ ] **Step 4: Tool table + PNGs.** Checks + commit on Fran's word.

---

### Task 5: base layer - k-nearest, packed pairs, pruning

**Files:** Modify `lib/dithering_exemplaria_velox.c`; test the velox suite.

**Interfaces:**
- `interior s32 _proximi_k (constans f32 lab[III], constans Opus* o, s32 k, s32* indices)`: partial selection over active colours, ties by lower index (the insertion sort's order); returns min(k, active count).
- `interior s32 _proximus (...)`: plain minimum for the single nearest.
- Pair key `i32 clavis = a | (b << IV) | (gradus << VIII)` (12 bits, 16 Bayer levels); a pair stops as soon as its contrast penalty (`POENA_CONTRASTUS * distantia`) alone exceeds the best error so far; weights read as `PONDERA[gradus][(y & VII) * VIII + (x & VII)]`.

- [ ] **Step 1: Add "pauci colores"**: scaena 80x60 with active sets {V}, {0, V}, {0, V, VI} -> only active colours; and "colores puri" restricted to each set's own colours stays exact.
- [ ] **Step 2: Implement**; velox suite all TRANSIIT.
- [ ] **Step 3: Plants:** pruning test inverted (`>` -> `<`) -> "colores puri" or "lateres in rubro plano" FRACTA; key unpack `>> IV` -> `>> V` for b -> "pauci colores" FRACTA.
- [ ] **Step 4: Tool table + PNGs.** Checks + commit on Fran's word.

---

### Task 6: hatching field, pictogram titles, residual

**Files:** Modify `lib/dithering_exemplaria_velox.c`; test the velox suite.

**Interfaces:** No new names. `_campum_striarum` on f32 planes with the bitset of hatched pixels; pictogram titles and `_residuum` use `_proximus` (plain minimum) instead of a full sort; region labels stay flat `s32` arrays with offsets.

- [ ] **Step 1: Add "census in fixis"**: velox on both fixtures, default options -> `tegmen_hachurarum <= 0.15`, `tegmen_pictographicum <= 0.30`, `regiones_pictographicae > 0` on pratum, `tegmen_aspersionis` in [0, 1].
- [ ] **Step 2: Implement**; velox suite all TRANSIIT.
- [ ] **Step 3: Plants:** hatching iterations `VIII` -> `0` -> "hachurae" FRACTA; residual `_proximus` returns the first active colour -> "colores puri" or "lateres in rubro plano" FRACTA.
- [ ] **Step 4: Tool table + PNGs.** Checks + commit on Fran's word.

---

### Task 7: docs and verdict

**Files:** Modify `include/dithering_exemplaria_velox.h` (top comment: what differs from classic and why, the measured table), `lib/dithering_exemplaria_velox.worklog.md`, `project-specs/exemplaria-velox.md` (status line + result), memory `dithering-exemplaria.md`; tabularium note for the pictor swap (spec VI) as a wish.

- [ ] **Step 1: Final run** of the tool with `-n XXX`: golem 600x751 <= 83 ms? meadow speed-up >= golem's? If either misses, the spec gets a dated paragraph saying where the time goes (a fresh `sample` profile).
- [ ] **Step 2: PNGs to Fran** (`_par.png` for both fixtures) for the look verdict.
- [ ] **Step 3: Checks + commit** on Fran's word.
