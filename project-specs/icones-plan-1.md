# icones plan 1 — the resampler and the library (P1–P5 of icones-spec.md)

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:subagent-driven-development (recommended) or
> superpowers:executing-plans to implement this plan task-by-task.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** an area/box resampler in `imago_opus`, then
`include/icones.h` + `lib/icones.c` — describe an icon set, render it
purely into PNG bytes once, project those same bytes into an `.iconset`
directory and an `.icns` container, refuse with a named cause, and prove
the container against `iconutil` in both directions.

**Architecture:** `IMAGO_SCALA_AREA` (alpha-weighted, fractional
coverage, `s64` accumulators) lands in `imago_opus` first and commits on
its own. Then `icones_reddere` (pure: `Imago` in, `Xar` of
`IconesPars` out, each carrying PNG bytes) → `icones_iconset_scribere`
and `icones_icns_codificare` / `_scribere`, both projections of the SAME
byte arrays. The library never decodes: the caller supplies a decoded
`Imago`, so `stb_image` stays in the CLI tool.

**Tech Stack:** C89 under the house flags; `imago_typus`, `imago_png`,
`flatura` (transitively), `imago_collatio`, `filum`, `via`, `xar`,
`chorda`, `piscina`, `credo`; `iconutil` and `sips` as foreign oracles
inside gates, never inside the library; pythonica (`silva.commissio`,
`silva.planta`, `silva.Editio`) for gates and commits.

**Spec:** `project-specs/icones-spec.md` v1 — this plan implements §3
(value), §4 (size table), §5 (resampler), §6 (writing), §7 (refusals),
§8 (gates I1–I7) and §9 (phases).

## Global Constraints

- **Compile flags, verbatim** (`tools/vexilla.sh`, never a local copy):
  `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion
  -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings -Wno-long-long
  -Wno-overlength-strings -fbracket-depth=512 -O2 -g`
- **Latin identifiers, comments and messages.** English only in `.md`.
- **`latina.h` macros are FORBIDDEN identifiers.** `magnitudo` is
  `sizeof` — which is why the size-set field is `latera_petita` and not
  `magnitudines`. Also taken: `nomen`, `casus`, `structura`, `brevis`,
  `registrum`, `exire`; single capitals `I V X L C D M` are numerals.
- **`i8` is `insignatus character`** (`latina.h:404`), so `p[III] == 255`
  reads correctly and `(i8)` casts from `int` are clean.
- **Numerals live in `latina.h`:** `XVI` 16, `XXXII` 32, `LXIV` 64,
  `CXXVIII` 128, `CCLVI` 256, `DXII` 512, `MMMMXCVI` 4096. **`MXXIV`
  (1024) is ABSENT** and Task 1 adds it — see that task's step 3.
- **C89 shape:** declarations at the head of each block; no `//`; no
  `_Bool`; no variadic macros; no flexible arrays.
- **`i32`/`i64` UNSIGNED, `s32`/`s64` signed.** Area accumulators are
  `s64`: a 64×64 footprint sums to ~266M (inside `s32`) but a 256×
  reduction passes 4 billion. **Compute pixel offsets in `s64` too** —
  `imago_opus` computes `src_y * fons_lat * IV` in `s32`, which wraps
  near 23170² sources (NOT 8192² as the spec's §11 says; that number is
  wrong and Task 5 corrects it in the as-built).
- **New C file → `./silva/scribe.sh <via> <<'EOF'`.** **Existing C file
  → `silva.Editio` / `silva.Refactio`.**
- **Probationes are discovered by `find probationes -name
  "probatio_*.c"`** (`compile_tests.sh:725`), so a new probatio needs NO
  list entry — but `./tools/compile_tests_fontes_generare.sh` must run
  AFTER `lib/icones.c` exists, or the link fails on undefined symbols.
- **A `.sh` gate registers AFTER the test loop**, near
  `probationes/probatio_plist_plutil.sh` (~`compile_tests.sh:828`), NEVER
  in `run_speculum()` — that ran before the loop and its exit 2 aborted
  the whole suite in plist's arc.
- **Arena: 16 MB** (`piscina_generare_dynamicum(…, 16777216)`) — house
  size for image work. A 1024² RGBA source is 4 MB before seven
  downscales and PNG buffers; the 1 MB used elsewhere is far too small.
- **Every gate is born red by a planted fault** (`silva.planta`). A plant
  must keep the code compiling AND every variable used.
- **Calibrations run SEQUENTIALLY, never batched**, and a plant that is
  reverted must leave no built binary behind.
- **A refusal test asserts the SPECIFIC status**, plus the subject it
  names. "It refused" is not the assertion; "it refused with THIS status
  naming THIS thing" is.
- **Three laws this arc paid for, all of which bit in fasciculum:**
  a plant that stays GREEN names a COVERAGE HOLE as readily as a dead
  gate (two failure branches refusing with the same shape look like one
  path from outside); **a test that varies ONE field proves ONE field is
  compared**; and ask what an oracle FORGIVES before treating its
  silence as proof.
- **When a probe reports ABSENCE, re-probe with the dumbest possible
  method before believing it** — no loops, no `printf`, print the lines.
  Five probes lied by omission while writing this plan; every one was the
  shell pattern, not the world.
- **Format before commit:** `./silva/formator.sh <viae> -scribere` then
  `-vitia`; lines ≤ 72 columns. Pick plant anchors AFTER formatting.
- **New identifier words:** `git add -N` the new files FIRST, then
  `./silva/nexus.sh -renovare`, then `./oratio/vocabula.sh -nova` —
  otherwise `NOVA 0` means "nothing examined", not "nothing new". Words
  to expect: `recidere`, `omissa`, `latera`, `iconset`, `icns`.
- **Commit from Python:** `silva.commissio(msg, viae, portae=[...])`.
- **Exit 2 from any runner means NOTHING RAN.**

## Task sequence differs from the spec's phases, deliberately

Spec §9 puts the `.icns` gates in P3 and the writers in P4. But **I4
(`iconutil` both ways) needs real files on disk**, so the writers must
precede it. Order here: resampler → value + pure core → writers +
container → refusals → oracles + tool + smoke + seal.

## File structure

| File | Responsibility |
|---|---|
| `include/latina.h` (modify) | add `MXXIV` 1024 — the one numeral the size set needs |
| `include/imago_opus.h` (modify) | append `IMAGO_SCALA_AREA` |
| `lib/imago_opus.c` (modify) | the alpha-weighted area resampler |
| `probationes/probatio_imago_opus.c` (create) | **the first automated gate this library has ever had** |
| `lib/imago_opus.worklog.md` (create) | resampler findings |
| `include/icones.h` (create) | value, result, statuses, four functions |
| `lib/icones.c` (create) | render, iconset write, icns encode, icns write |
| `probationes/probatio_icones.c` (create) | I1, I3, I5, I6, I7 |
| `probationes/probatio_icones_iconutil.sh` (create) | I4 — foreign oracle, both directions |
| `tools/icones_instrumentum.c` (create) | CLI; `stb_image` lives HERE |
| `tools/icones_fumus.sh` (create) | by-hand: real bundle, Finder look |
| `lib/icones.worklog.md` (create) | findings at find-time |

---

### Task 1: the area resampler (its own commit)

Implements spec §5; gate I2. `imago_opus` has **no automated test at
all** today — `probationes/probatio_imago.c` is a windowed demo with zero
`CREDO_` assertions and a `fenestra_destruere` at the end. So this task
also pins the two EXISTING modes, which have never been asserted.

**Files:**
- Create: `probationes/probatio_imago_opus.c`, `lib/imago_opus.worklog.md`
- Modify: `include/latina.h`, `include/imago_opus.h`, `lib/imago_opus.c`

**Interfaces:**
- Consumes: `imago_typus.h`, `piscina`, `credo`.
- Produces: `IMAGO_SCALA_AREA`; `MXXIV`.

- [ ] **Step 1: Write the failing tests**

```bash
./silva/scribe.sh probationes/probatio_imago_opus.c <<'EOF'
/* probatio_imago_opus.c - scalatio: modi tres, media areae alpha-ponderata
 *
 * PRIMA porta automata huius bibliothecae: probatio_imago.c fenestram
 * aperit et nihil asserit, ergo PROXIMUS et BILINEARIS numquam pinnati
 * sunt. Hic ambo pinnantur ANTE modum novum, ne porta modi novi sola
 * stet.
 */

#include "latina.h"
#include "imago_opus.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>

interior Imago
_fingere (
        s32  latitudo,
        s32  altitudo,
    Piscina* piscina)
{
    Imago im;

    im.latitudo  = (i32)latitudo;
    im.altitudo  = (i32)altitudo;
    im.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(latitudo * altitudo * IV));
    redde im;
}

interior vacuum
_ponere (
     Imago* im,
       s32  x,
       s32  y,
       s32  r,
       s32  g,
       s32  b,
       s32  a)
{
    i8* p = im->pixela + (((y * (s32)im->latitudo) + x) * IV);

    p[ZEPHYRUM]  = (i8)r;
    p[I]         = (i8)g;
    p[II]        = (i8)b;
    p[III]       = (i8)a;
}

interior s32
_legere (
    constans Imago* im,
                s32  x,
                s32  y,
                s32  canalis)
{
    redde (s32)im->pixela[(((y * (s32)im->latitudo) + x) * IV) + canalis];
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_imago_opus",
                                         16777216);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- modi EXSISTENTES pinnati (numquam antea asserti) ---- */
    {
        Imago fons;
        Imago par;
        Imago dimidia;

        imprimere("\n--- modi exsistentes: PROXIMUS, BILINEARIS ---\n");

        fons = _fingere(II, II, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, ZEPHYRUM, 255);
        _ponere(&fons, I, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, 255);
        _ponere(&fons, ZEPHYRUM, I, ZEPHYRUM, ZEPHYRUM, 255, 255);
        _ponere(&fons, I, I, 255, 255, 255, 255);

        /* 1:1 per PROXIMUM = transitus exactus */
        par = imago_scalare(&fons, II, II, IMAGO_SCALA_PROXIMUS,
                            piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, I, I), 255);

        /* 1:1 per BILINEAREM: fractio ZEPHYRUM est, ergo item exactus.
         * Hoc MENSURATUM est, non creditum: scala = ((fons-1)*FP)/(dest-1)
         * exacte FP_ONE fit cum dimensiones congruunt. */
        par = imago_scalare(&fons, II, II, IMAGO_SCALA_BILINEARIS,
                            piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, I), 255);

        /* PROXIMUS 2:1 unum pixelum ELIGIT, non miscet */
        dimidia = imago_scalare(&fons, I, I, IMAGO_SCALA_PROXIMUS,
                                piscina);
        CREDO_NON_NIHIL(dimidia.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&dimidia, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&dimidia, ZEPHYRUM, ZEPHYRUM, I),
                           ZEPHYRUM);
    }

    /* ---- I2 a: AREA alpha-PONDERATA ---- */
    {
        Imago fons;
        Imago parva;

        imprimere("\n--- I2 a: area alpha-ponderata ---\n");

        /* rubrum OPACUM iuxta viride PELLUCIDUM */
        fons = _fingere(II, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, ZEPHYRUM, 255);
        _ponere(&fons, I, ZEPHYRUM, ZEPHYRUM, 255, ZEPHYRUM, ZEPHYRUM);

        parva = imago_scalare(&fons, I, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(parva.pixela);

        /* RECTUM: rubrum plenum, alpha dimidium. Media ingenua
         * (127,127,127,127) lutulentum daret - haec assertio de eo
         * CLAMAT. */
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 255);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM, I),
                           ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM, II),
                           ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)_legere(&parva, ZEPHYRUM, ZEPHYRUM, III),
                           128);
    }

    /* ---- I2 b: COPERTURA FRACTIONARIA, constantes pinnatae ---- */
    {
        Imago fons;
        Imago duo;

        imprimere("\n--- I2 b: copertura fractionaria III -> II ---\n");

        /* Ex fonte MXXIV omnis magnitudo ratio potestatis binariae
         * exacta est, ergo tabula integra omnes probationes nostras
         * naturales transiret. Haec ratio III -> II fractionaria est. */
        fons = _fingere(III, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM, 255);
        _ponere(&fons, I, ZEPHYRUM, 120, 120, 120, 255);
        _ponere(&fons, II, ZEPHYRUM, 240, 240, 240, 255);

        duo = imago_scalare(&fons, II, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(duo.pixela);

        /* Manu computatum, exactum: pixelum exitus 0 extentum fontis
         * [0, 1.5) tegit -> (0*1.0 + 120*0.5)/1.5 = 40; pixelum 1
         * [1.5, 3.0) -> (120*0.5 + 240*1.0)/1.5 = 200. */
        CREDO_AEQUALIS_I32((i32)_legere(&duo, ZEPHYRUM, ZEPHYRUM,
                                       ZEPHYRUM), 40);
        CREDO_AEQUALIS_I32((i32)_legere(&duo, I, ZEPHYRUM, ZEPHYRUM),
                           200);
        CREDO_AEQUALIS_I32((i32)_legere(&duo, ZEPHYRUM, ZEPHYRUM, III),
                           255);
    }

    /* ---- I2 c: AREA ad 1:1 TRANSIT, non resamplat ---- */
    {
        Imago fons;
        Imago par;

        imprimere("\n--- I2 c: area 1:1 exacta ---\n");

        fons = _fingere(III, I, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        _ponere(&fons, ZEPHYRUM, ZEPHYRUM, 13, 21, 34, 255);
        _ponere(&fons, I, ZEPHYRUM, 55, 89, 144, 200);
        _ponere(&fons, II, ZEPHYRUM, 233, 255, 1, 100);

        par = imago_scalare(&fons, III, I, IMAGO_SCALA_AREA, piscina);
        CREDO_NON_NIHIL(par.pixela);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, ZEPHYRUM), 55);
        CREDO_AEQUALIS_I32((i32)_legere(&par, I, ZEPHYRUM, I), 89);
        CREDO_AEQUALIS_I32((i32)_legere(&par, II, ZEPHYRUM, III), 100);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
EOF
```

- [ ] **Step 2: Run and watch it fail**

```bash
./compile_tests.sh imago_opus > /tmp/i1.log 2>&1; echo "rc=$?"
tail -20 /tmp/i1.log
```

Expected: `use of undeclared identifier 'IMAGO_SCALA_AREA'` under
`-Werror`. **Tail the log unconditionally** — do not grep for an
expected pattern.

- [ ] **Step 3: Add the numeral and the enum value**

`include/latina.h` — beside the other thousands (`M` is at `:373`,
`MMMMXCVI` at `:389`):

```c
#define MXXIV				1024
```

`include/imago_opus.h` — APPENDED, never reordered:

```c
nomen enumeratio {
    IMAGO_SCALA_PROXIMUS,   /* Nearest neighbor - velox, pixelatum */
    IMAGO_SCALA_BILINEARIS, /* Bilinear interpolatio - lenis */
    IMAGO_SCALA_AREA        /* Media areae - alpha-ponderata, pro
                             * DEMINUTIONE. Bilinearis II x II legit
                             * quantavis scala sit: 1024 -> 16 pixela IV
                             * ex 4096 legit. Ad AUCTIONEM bilinearem
                             * adhibe. */
} ImagoScalaModus;
```

- [ ] **Step 4: Implement**

Insert before `imago_scalare` in `lib/imago_opus.c`:

```c
#define AREA_FIX_SHIFT XVI
#define AREA_FIX_ONE   (((s64)I) << AREA_FIX_SHIFT)

/* Media areae ALPHA-PONDERATA. Pixela NON praemultiplicata sunt
 * (imago_typus.h), ergo RGB per alpha ponderandum et per summam alpha
 * dividendum est: aliter pixelum pellucidum colorem suum in vicinos
 * trahit (halo classicus), et icones marginibus pellucidis constant.
 *
 * Accumulatio in s64: area LXIV x LXIV ad ~266M summat (intra s32), sed
 * reductio CCLVI x duo miliarda transit. Offset quoque in s64
 * computatur - semita vetus s32 utitur et circa 23170^2 circumvolvitur.
 */
interior vacuum
_scalare_area (
    constans i8* fons_pixela,
            s32  fons_lat,
            s32  fons_alt,
             i8* dest_pixela,
            s32  dest_lat,
            s32  dest_alt)
{
    s32 dx, dy;

    per (dy = ZEPHYRUM; dy < dest_alt; dy++)
    {
        s64 sy0 = ((s64)dy * fons_alt * AREA_FIX_ONE) / dest_alt;
        s64 sy1 = ((s64)(dy + I) * fons_alt * AREA_FIX_ONE) / dest_alt;
        s32 y_primus  = (s32)(sy0 >> AREA_FIX_SHIFT);
        s32 y_ultimus = (s32)((sy1 - I) >> AREA_FIX_SHIFT);

        si (y_ultimus >= fons_alt) y_ultimus = fons_alt - I;

        per (dx = ZEPHYRUM; dx < dest_lat; dx++)
        {
            s64 sx0 = ((s64)dx * fons_lat * AREA_FIX_ONE) / dest_lat;
            s64 sx1 = ((s64)(dx + I) * fons_lat * AREA_FIX_ONE)
                / dest_lat;
            s32 x_primus  = (s32)(sx0 >> AREA_FIX_SHIFT);
            s32 x_ultimus = (s32)((sx1 - I) >> AREA_FIX_SHIFT);
            s64 pondus_totum  = ZEPHYRUM;
            s64 summa_a       = ZEPHYRUM;
            s64 summa_r       = ZEPHYRUM;
            s64 summa_g       = ZEPHYRUM;
            s64 summa_b       = ZEPHYRUM;
            s32 sy, sx;
            i8* dest_ptr;

            si (x_ultimus >= fons_lat) x_ultimus = fons_lat - I;

            per (sy = y_primus; sy <= y_ultimus; sy++)
            {
                s64 y_ab = (sy == y_primus)
                    ? sy0 : ((s64)sy * AREA_FIX_ONE);
                s64 y_ad = (sy == y_ultimus)
                    ? sy1 : ((s64)(sy + I) * AREA_FIX_ONE);
                s64 pondus_y = y_ad - y_ab;

                per (sx = x_primus; sx <= x_ultimus; sx++)
                {
                    s64 x_ab = (sx == x_primus)
                        ? sx0 : ((s64)sx * AREA_FIX_ONE);
                    s64 x_ad = (sx == x_ultimus)
                        ? sx1 : ((s64)(sx + I) * AREA_FIX_ONE);
                    s64 pondus = (pondus_y * (x_ad - x_ab))
                        >> AREA_FIX_SHIFT;
                    s64 offset = (((s64)sy * fons_lat) + sx) * IV;
                    s64 a = (s64)fons_pixela[offset + III];

                    pondus_totum  += pondus;
                    summa_a       += a * pondus;
                    summa_r       += (s64)fons_pixela[offset] * a
                        * pondus;
                    summa_g       += (s64)fons_pixela[offset + I] * a
                        * pondus;
                    summa_b       += (s64)fons_pixela[offset + II] * a
                        * pondus;
                }
            }

            dest_ptr = dest_pixela
                + ((((s64)dy * dest_lat) + dx) * IV);

            si (pondus_totum <= ZEPHYRUM)
            {
                dest_ptr[ZEPHYRUM]  = ZEPHYRUM;
                dest_ptr[I]         = ZEPHYRUM;
                dest_ptr[II]        = ZEPHYRUM;
                dest_ptr[III]       = ZEPHYRUM;
                perge;
            }

            /* alpha = media simplex; RGB = media per alpha ponderata.
             * Rotundatio dimidium sursum. */
            dest_ptr[III] = (i8)((summa_a + (pondus_totum / II))
                / pondus_totum);
            si (summa_a > ZEPHYRUM)
            {
                dest_ptr[ZEPHYRUM] = (i8)((summa_r + (summa_a / II))
                    / summa_a);
                dest_ptr[I] = (i8)((summa_g + (summa_a / II))
                    / summa_a);
                dest_ptr[II] = (i8)((summa_b + (summa_a / II))
                    / summa_a);
            }
            alioquin
            {
                dest_ptr[ZEPHYRUM]  = ZEPHYRUM;
                dest_ptr[I]         = ZEPHYRUM;
                dest_ptr[II]        = ZEPHYRUM;
            }
        }
    }
}
```

And a third arm in `imago_scalare`'s `commutatio`:

```c
        casus IMAGO_SCALA_AREA:
            _scalare_area(fons->pixela, (s32)fons->latitudo,
                          (s32)fons->altitudo, dest.pixela,
                          nova_latitudo_s, nova_altitudo_s);
            frange;
```

- [ ] **Step 5: Run and watch it pass**

```bash
./compile_tests.sh imago_opus > /tmp/i1.log 2>&1; echo "rc=$?"
tail -20 /tmp/i1.log
```

If I2 b disagrees, read the two pinned numbers before touching the
code: 40 and 200 are exact by hand, so a mismatch names either a
coverage-weight bug or an off-by-one in the span.

- [ ] **Step 6: Prove the gate can go red**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
# alpha-pondus tollere: media INGENUA, quam I2 a capere DEBET
print(silva.planta('lib/imago_opus.c',
                   'summa_r       += (s64)fons_pixela[offset] * a\n                        * pondus;',
                   'summa_r       += (s64)fons_pixela[offset] * 255\n                        * pondus;',
                   'radix', 'imago_opus'))
PY
```

Expected RED on I2 a's green channel or red channel assertion. A SECOND
plant, run separately: change `pondus` to ignore fractional edges
(`pondus = AREA_FIX_ONE`), which must redden I2 b's 40/200 pins.

- [ ] **Step 7: Whole suite, format, worklog, commit**

```bash
./compile_tests.sh > /tmp/omnia.log 2>&1; echo "rc=$?"
grep -E 'Tests (Total|Passed|Failed)' /tmp/omnia.log
./silva/formator.sh include/latina.h include/imago_opus.h \
    lib/imago_opus.c probationes/probatio_imago_opus.c -scribere
./silva/formator.sh include/latina.h include/imago_opus.h \
    lib/imago_opus.c probationes/probatio_imago_opus.c -vitia
```

`lib/imago_opus.worklog.md` records: this library had NO automated gate
before today; bilinear is exact at 1:1 by arithmetic (measured, not
assumed) but the fit-within API cannot promise unit scale because it
truncates twice; the `s32` offset wrap point is ~23170², not 8192²; and
alpha weighting is the difference between an icon and a halo. Then
commit with `portae=[('radix', 'imago_opus')]`.

---

### Task 2: the value and the pure core

Implements spec §3; gate I1. No filesystem is touched.

**Files:**
- Create: `include/icones.h`, `lib/icones.c`,
  `probationes/probatio_icones.c`
- Run AFTER `lib/icones.c` exists:
  `./tools/compile_tests_fontes_generare.sh`

**Interfaces:**
- Consumes: Task 1's `IMAGO_SCALA_AREA`; `imago_png_codificare`
  (`PngFructus {b32 successus; i8* datum; i32 mensura; chorda error;}`).
- Produces: `Icones`, `IconesPars`, `IconesFructus`, `IconesStatus`,
  `icones_reddere`.

- [ ] **Step 1: Write `include/icones.h`**

The header is spec §3 verbatim plus the four signatures of §6 and the
statuses of §7. The size table is DATA in the `.c`, not the header.

- [ ] **Step 2: Write the failing tests (I1)**

Assert: seven distinct renderings from a 256×256 source (16, 32, 64,
128, **256**, and 512/1024 OMITTED); ten parts with Apple's filenames;
`omissa` carrying exactly `ICONES_LATERA_DXII | ICONES_LATERA_MXXIV`;
and the byte-sharing identity that proves seven encodes rather than ten:

```c
        pars_a = _partem_invenire(fructus.partes, "icon_16x16@2x.png");
        pars_b = _partem_invenire(fructus.partes, "icon_32x32.png");
        CREDO_NON_NIHIL(pars_a);
        CREDO_NON_NIHIL(pars_b);
        CREDO_VERUM(pars_a->octeti.datum == pars_b->octeti.datum);
        CREDO_AEQUALIS_I32(pars_a->latera, pars_b->latera);
```

Plus purity: `CREDO_FALSUM(filum_directorium_existit("AppIcon.iconset"))`.

- [ ] **Step 3: Run; expect undefined `_icones_reddere` at LINK**

- [ ] **Step 4: Implement `icones_reddere`**

The size table as data, one row per `.iconset` file:

```c
nomen structura {
                i32  latera;
constans character*  semita;
constans character*  codex;   /* codex .icns */
                i32  vexillum;
} IconesOrdo;

/* Decem plagulae, SEPTEM redditiones: tria latera bis sub nominibus
 * logicis diversis apparent (spec par. IV, iconutil mensuratum).
 * NOMEN SINE '_' PRAEPOSITO: '_' + capitalis implementationi
 * reservatum est (C89 7.1.3) et examen id recusat - clang TACET. */
hic_manens constans IconesOrdo ORDINES[X] = {
    {   XVI, "icon_16x16.png",       "ic04", ICONES_LATERA_XVI     },
    { XXXII, "icon_16x16@2x.png",    "ic11", ICONES_LATERA_XXXII   },
    { XXXII, "icon_32x32.png",       "ic05", ICONES_LATERA_XXXII   },
    {  LXIV, "icon_32x32@2x.png",    "ic12", ICONES_LATERA_LXIV    },
    { CXXVIII, "icon_128x128.png",   "ic07", ICONES_LATERA_CXXVIII },
    { CCLVI, "icon_128x128@2x.png",  "ic13", ICONES_LATERA_CCLVI   },
    { CCLVI, "icon_256x256.png",     "ic08", ICONES_LATERA_CCLVI   },
    {  DXII, "icon_256x256@2x.png",  "ic14", ICONES_LATERA_DXII    },
    {  DXII, "icon_512x512.png",     "ic09", ICONES_LATERA_DXII    },
    { MXXIV, "icon_512x512@2x.png",  "ic10", ICONES_LATERA_MXXIV   }
};
```

`reddere` then: validate; compute the centred square
(`latus = min(lat, alt)`, `x = (lat - latus)/2`, `y = (alt - latus)/2`);
for each DISTINCT `latera` ≤ `latus`, crop-and-scale ONCE (own row copy
when `latera == latus`, else `imago_extrahere_et_scalare` with
`IMAGO_SCALA_AREA` and EXACT dimensions) and encode ONCE with
`imago_png_codificare`, caching `chorda` per size; then walk `ORDINES`
emitting one part per row, reusing the cached `chorda` so duplicates
share `datum`. On `!fructus_png.successus`, refuse `ICONES_ERROR_PNG`
carrying `fructus_png.error` as `sedes_vitii` — the encoder's own
message, richer than spec §7's "the size being encoded" (an as-built
note for Task 5).

- [ ] **Step 5: Wire the manifest, run, watch it pass**

```bash
./tools/compile_tests_fontes_generare.sh
./compile_tests.sh icones > /tmp/i2.log 2>&1; echo "rc=$?"
tail -20 /tmp/i2.log
```

- [ ] **Step 6: Plant** — make the per-size cache return a fresh
  `chorda` each time (encode ten times). I1's pointer-identity assertion
  must go red and nothing else. If other assertions redden too, the
  cache is doing more than caching.

- [ ] **Step 7: `git add -N`, vocabula, format, commit**
  (`portae=[('radix', 'icones')]`).

---

### Task 3: the writers and the container

Implements spec §6; gates I3, I6.

**Files:** modify `lib/icones.c`, `probationes/probatio_icones.c`

- [ ] **Step 1: Write the failing tests (I3 + I6)**

I3 is pure: `icones_icns_codificare` output starts `icns`, its declared
big-endian length equals the buffer length, the chunk table walks to
exactly the end, every payload begins with the PNG signature
`\x89PNG\r\n\x1a\n`, and the codes match `ORDINES` in order.

I6 writes into a nested virgin directory and asserts the duplicates are
byte-identical ON DISK, then that the `.icns` FILE is byte-equal to the
pure encoder's output — the pure/twin consistency check that plist
established.

- [ ] **Step 2: Run; expect undefined `_icones_icns_codificare`**

- [ ] **Step 3: Implement**

Container: `icns` + big-endian total length, then per chunk a 4-char
code + big-endian length **including the 8-byte header** + payload. Build
with `chorda_aedificator`. `_ordinem_scribere` writes the four length
bytes explicitly, MSB first — never `memcpy` of a host `i32`.

`icones_iconset_scribere` joins `via_radicis` with `titulus` +
`".iconset"`, creates it with `filum_directorium_creare_cum_parentibus`
(its second consumer), then writes each part with `filum_scribere`.

- [ ] **Step 4: Run green, then two plants, SEQUENTIALLY**

Plant one: emit the chunk length WITHOUT the 8-byte header — I3's
"walks to exactly the end" must go red. Plant two: have
`icones_icns_scribere` write the bytes but skip the final length
patch — I6's byte-equality must go red.

- [ ] **Step 5: Whole suite, format, commit.**

**As executed (2026-09-12) — four deviations, each with its reason:**

- **No `chorda_aedificator`.** Its own header says it serves an
  UNKNOWN final size and TEXT; it has no raw-byte append. The `.icns`
  size is known before writing, so the container is computed,
  allocated once and filled — `lib/imago_png.c`'s shape. Its closing
  size self-check was NOT carried over: a mismatch has no honest
  `IconesStatus`, and I3's independent walk catches the short case.
- **`IconesFructus` gained `chorda titulus`** (Fran chose this over
  making `via_radicis` the full directory path). The iconset writer
  receives only the fructus, so without it `<titulus>.iconset` could
  not be derived; the suffix is a format requirement `iconutil`
  enforces, so the library owns it.
- **I6 uses a 512 source and asserts NINE files, not ten.** Ten needs
  a source ≥ 1024, about 13 MB of the 16 MB arena; 512 also reaches
  TWO duplicate pairs on disk where 256 reaches one.
- **Plant two translated, plant three added.** The thin twin has no
  length patch to skip, so plant two writes one byte short: 1 red, and
  the writer still returns `VERUM`/`SUCCESSUS` — only the byte
  comparison sees it. Plant three writes part 0's bytes into every
  file: 8 reds on the per-file loop while BOTH duplicate-pair checks
  stay green. Plant one: 13 reds, all in I3.

---

### Task 4: refusals

Implements spec §7; gate I5.

- [ ] **Step 1** — one case per status, each asserting the SPECIFIC
  status AND its `sedes_vitii`: no source; `pixela` NIHIL; a 12×12
  source (`_MINIMUS`, naming the side); an unknown flag (`_LATERA`,
  naming the size); a closed directory (`_DIRECTORIUM`); an unwritable
  file (`_SCRIPTIO`).
- [ ] **Step 2** — run; expect the new assertions to fail.
- [ ] **Step 3** — implement the guards, validating `status` and
  `sedes_vitii` BEFORE writing through them.
- [ ] **Step 4** — **a plant per refusal branch.** Task 3 of fasciculum
  proved that two branches refusing with the same status shape look like
  one path from outside, and one plant stayed green because no test
  reached the other branch.
- [ ] **Step 5** — whole suite, format, commit.

**Found during Task 3 — cases Step 1 must add** (see
`lib/icones.worklog.md`):

- **Every requested size exceeds the source** (`latera_petita =
  ICONES_LATERA_MXXIV` from a 256 px source): `reddere` today returns
  `VERUM` with ZERO parts, because `_MINIMUS` only checks side < 16.
  That is D10's forbidden shape. Which status refuses it is a decision
  to make here; `_MINIMUS` naming the requested set is the obvious
  candidate.
- **The writers have no argument guards** — a NIHIL `fructus`, `via`
  or `via_radicis` dereferences. Left out deliberately so these tests
  are born red; expect a crash rather than a legible red until the
  guards land, so run this step's red with that in mind.
- **`si (!pars)` → `_MEMORIA` in the writers is unreachable by test**
  (`xar_obtinere` within range). "A plant per branch" cannot cover it;
  keep or remove it on purpose.
- **An empty `semita`** collapses under `via_iungere` (it skips empty
  parts) to the directory path itself, and `filum_scribere` then fails
  `_SCRIPTIO`. Legible, but worth a case.

**As executed (2026-09-12):**

- **28 reachable refusal branches**, each tested by status AND
  `sedes_vitii`, each with its own plant: 28 matched, 35 reds predicted
  and counted. Plant form: swap the branch's status for a sibling
  (equal tokens, no crash), so it reddens only if a test reaches it.
- **Nothing requested covered → `_MINIMUS`** naming the requested sizes
  (`"128 1024"`), Fran's call; one covered size still succeeds
  (positive twin).
- **`fons` NIHIL is now `_DESUNT`** — the enum's comment said so; the
  code said `_FONS`.
- **One shared `_fructum_iudicare` for all three writers**; the writers'
  `si (!pars)` checks are deleted. Unreachable `_MEMORIA`/`_PNG`
  branches carry no plant and are named in the worklog.
- **`silva.planta` cannot select an occurrence**; anchors are token
  sequences with enough context to be unique.

---

### Task 5: the oracles, the tool, the smoke, the seal

Gates I4, I7 plus the by-hand AUDIENDUM.

- [ ] **Step 1: `tools/icones_instrumentum.c`** — `-fons x.png -titulus
  AppIcon -radix <dir> [-latera …]`, and `-legere` printing an `.icns`
  chunk table. **`stb_image` lives here**: this is the only place
  `imago.h` is included. Exit 0 / 1 / 2 with 2 = nothing done. Build
  with `./compile_tools.sh icones_instrumentum` (fixed this session to
  exclude probatio objects).
- [ ] **Step 2: `probationes/probatio_icones_iconutil.sh`** (I4) — our
  `.icns` → `iconutil --convert iconset` → extracted PNGs match our
  sizes; our `.iconset` → `iconutil --convert icns` → its chunk table
  matches ours. **Register AFTER the test loop** (~`compile_tests.sh:828`),
  never in `run_speculum()`. Exit 2 when `iconutil` is absent, reported
  as a named skip.
- [ ] **Step 3: I7** — `sips -z 16 16` the source, `imago_conferre`
  against our 16px with `regula.tolerantia` 0 and
  `sperne_lenimentum` FALSUM, and pin a ceiling on
  `fructus.delta_maximum` **measured first**. Report `pixela_diversa`.
  The struct's own comment warns that `delta_maximum` is a bare
  measurement, not a judgement — so the ceiling catches "we shipped
  nearest by mistake", not "we differ from Apple's filter".
- [ ] **Step 4: `tools/icones_fumus.sh`** — build a real `.icns`, put it
  in a bundle with `bin/fasciculum_instrumentum`, print the tree, STOP
  before `open`. `-agere` opens it. This answers spec §11's AUDIENDUM:
  **does Finder DRAW a PNG-in-`ic04` icon at 16px?** Record the answer
  verbatim. If it does not, the `icp4`/`icp5` row of §4 is the fix and
  it lands before the seal.
- [ ] **Step 5: worklog, spec as-built, whole-suite seal**
  (`portae=[('radix', None)]`). The as-built must carry at least: the
  `s32` wrap point is ~23170², not 8192² (§11 is wrong); `_ERROR_PNG`
  carries the encoder's own message; and the task order deviates from
  §9 because I4 needs files.

**As executed (2026-09-12) — Task 5 became four commits, because its
oracles found two defects in committed code:**

- **`4fe5ab6f` (imago_opus):** `imago_extrahere_et_scalare` dispatched
  `si (PROXIMUS) … alioquin bilinear`, so `AREA` silently ran bilinear —
  Task 1 had added `AREA` only to `imago_scalare`. Every icon icones had
  produced was bilinear. New gate I2 d (equivalence + hand pins 98/88).
- **`34d78ac3` (icones):** icones passed exact sizes to that function as
  MAXIMUM bounds; its fit-within arithmetic truncates, so any
  non-power-of-two source gave icons one pixel short (1000 px → 15, 31,
  …). Fix: crop the centred square once, `imago_scalare` per size (exact).
  **I7 as built:** a FROZEN `sips` oracle (committed fixture + generator),
  measured on the INTERIOR — the plan's `delta_maximum` could not separate
  area (224) from bilinear (255). **I8, new:** exact sizes from a 100 px
  source and centred cropping (D1), which had never been tested.
- **`4e32d9c8`:** `tools/icones_instrumentum.c` (with `-legere`) and I4.
  `iconutil` measured LENIENT (`rc=0` on broken containers), so I4 asserts
  file count and sizes. Registered after `plutil`, filtered runs only when
  the filter names it. Its plant showed the code → size map above 64 px is
  guarded by I4 alone, deliberately.
- **The fumus** builds FOUR bundles, not one: Calculator's icon as a
  control, all sizes, `ic04` only, `ic11`+`ic05` only — on Retina Finder
  draws the 32 px image at 16 pt, so a full set never exercises `ic04`.
  **AUDIENDUM — answered NO, and D9 overturned.** PNG in `ic04`/`ic05`
  is drawn by Finder as noise (Fran's screenshot of single-chunk
  bundles), `iconutil` misreads it the same way (10/256 and 20/1024
  pixels identical), and the spec's own fallback `icp4`/`icp5` fails too
  — while `sips`/ImageIO decoded all of it correctly, which had produced
  an "answered yes" here that was retracted within the hour. Fix
  (decretum `01M2BZDWEB`): `ic04`/`ic05` carry Apple's `ARGB` — RLE
  planes, colour PREMULTIPLIED, because `iconutil` divides by alpha. A
  first straight-value version passed I3 and FAILED I4, whose pixel
  comparison was added for exactly this. `IconesPars` gained `onus_icns`;
  I3 b pins three encodings; I4 compares composited pixels. **Finder
  confirmed it** (Fran's second screenshot): `SolumXVI`, whose only image
  is our 16 px `ARGB`, draws a clean disc. Sealed with spec §12.

---

## Self-review

**Spec coverage.** §3 → Task 2. §4 → Task 2's `ORDINES` table. §5 →
Task 1. §6 → Task 3. §7 → Task 4. §8: I1 → Task 2, I2 → Task 1, I3/I6 →
Task 3, I5 → Task 4, I4/I7 → Task 5. §9 P1–P5 → Tasks 1–5, reordered
with the reason stated. §10 deferrals: nothing here implements them.
§11's AUDIENDUM → Task 5 step 4, the only step whose outcome this plan
cannot predict.

**Placeholders.** Tasks 1–3 carry full code. Task 4 is described per
status and Task 5 per artifact — deliberate thinness, and fasciculum's
plan made the same call and its self-review was right that this is where
an executor must think. If it proves too thin, write Task 4's cases out
in full on the next pass.

**Type consistency.** `IconesStatus` is the status type in all four
functions; `sedes_vitii` is `chorda*` everywhere; `latera` is `i32`
pixels throughout and never `magnitudo`; `PngFructus.successus` is
checked before `.datum`/`.mensura` are read; `IMAGO_SCALA_AREA` is the
mode name in both Task 1 and Task 2.

**The one number this plan corrects in its own spec:** §11 says the
`s32` offset wraps above 8192² sources. 8192² × 4 = 268 M, inside
`s32`; the real wrap is near 23170². Found by writing the arithmetic
out, which is the argument for plans carrying real code.
