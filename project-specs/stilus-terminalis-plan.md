# stilus_terminalis — plan (module 004: the style codec)

**DONE 2026-10-03** (T1 f0ea4826, T2 1bd75a15, T3 1d743562, T4 RELATIO in
`lib/stilus_terminalis.phase-log.md`). Written 2026-10-03, after eventus
phase B and D2 step 3. Sketch:
`../terminal-planning/modules/004-style-codec.md`. Oracle: Ghostty
(`../ghostty` @ 12752b2, `src/terminal/sgr.zig`: 31 tests;
`style.zig`).

## Decisions (Fran, 2026-10-03)

1. **Name: `stilus_terminalis`** (`include/stilus_terminalis.h`,
   `lib/stilus_terminalis.c`), like the other `*_terminalis` libraries.
2. **RGB → 256 quantizing moves INTO the codec** (`stilus_quantizare`,
   tessera's `_cclvi` algorithm verbatim) with an encode option (full
   colour / 256); tessera's copy is deleted when it switches.
3. **Full reset now, minimal diff later.** The `prior` parameter is in
   the API from day one: NIHIL = full reset; v1 with a `prior` emits
   NOTHING when the styles are equal and a full reset otherwise
   (correct, not yet minimal). The real minimal diff is measured
   against tessera's `octeti_emissi` before it is adopted.
4. **Codec + tessera migration** in this plan; tessera's curly/coloured
   underline stays deferred.

## The API (pure; no allocation besides the builder)

```c
nomen enumeratio {
    STILUS_COLOR_NATIVUS = ZEPHYRUM,   /* the terminal's default */
    STILUS_COLOR_TABULA,               /* palette index 0-255 */
    STILUS_COLOR_RGB                   /* 0x00RRGGBB */
} StilusColorGenus;

nomen structura { StilusColorGenus genus; i32 valor; } StilusColor;

/* ornamenta (SGR number in the comment) */
#define STILUS_CRASSUM     0x001   /* 1 */
#define STILUS_OBSCURUM    0x002   /* 2 faint */
#define STILUS_CURSIVUM    0x004   /* 3 */
#define STILUS_NICTANS     0x008   /* 5 blink */
#define STILUS_INVERSUM    0x010   /* 7 */
#define STILUS_INVISIBILE  0x020   /* 8 */
#define STILUS_TRANSFIXUM  0x040   /* 9 */
#define STILUS_SUPERLINEA  0x080   /* 53 overline */

nomen enumeratio {
    STILUS_SUBLINEA_NULLA = ZEPHYRUM, STILUS_SUBLINEA_SIMPLEX,
    STILUS_SUBLINEA_DUPLEX, STILUS_SUBLINEA_UNDULATA,
    STILUS_SUBLINEA_PUNCTATA, STILUS_SUBLINEA_LINEOLATA
} StilusSublinea;

nomen structura {
       StilusColor color_litterae;
       StilusColor color_fundi;
       StilusColor color_sublineae;
               i32 ornamenta;
    StilusSublinea sublinea;
} StilusTerminalis;

#define STILUS_CODIFICATIO_PLENA  ZEPHYRUM   /* 38;2;r;g;b */
#define STILUS_CODIFICATIO_CCLVI  I          /* RGB quantized: 38;5;n */

vacuum stilus_nativus (StilusTerminalis* st);
b32    stilus_aequalis (constans StilusTerminalis* a,
                        constans StilusTerminalis* b);
vacuum stilus_codificare (ChordaAedificator* exitus,
                          constans StilusTerminalis* prior, /* NIHIL */
                          constans StilusTerminalis* novus,
                          i32 codificatio);
i32    stilus_applicare (constans SeriesLexema* lexema,  /* CSI 'm' */
                         StilusTerminalis* st);   /* = unknowns skipped */
i32    stilus_tabulae_color (i32 index);   /* xterm default → 0xRRGGBB */
i32    stilus_quantizare (i32 rgb);        /* → index 16-255 */
```

**Canonical encoding (full reset):** `ESC [ 0`, then `;1 ;2 ;3`, the
underline (`;4` single, `;4:n` other kinds), `;5 ;7 ;8 ;9 ;53`, then the
foreground (`;38;2;r;g;b`, `;38;5;n`), background (`;48;…`), underline
colour (`;58;…`), then `m`. For tessera's subset (1 2 3 4 7 9, fg, bg)
this is byte-identical to `_stilum_emittere` today. Palette colours
always use `38;5;n` (no 30-37 / 90-97 forms on encode; decode accepts
them).

**Decode is permissive** (Ghostty's rules): `;` and `:` separators,
mixed; `38;5;n`, `38;2;r;g;b`, `38:2::r:g:b`, `38:2:r:g:b`; 30-37,
40-47, 90-97, 100-107; resets (22 bold+faint, 23, 24, 25, 27, 28, 29,
39, 49, 55, 59); `4:0`..`4:5`; truncated colour forms consumed without
effect; unknowns skipped and counted, never fatal.

## Tasks

**T1 — the core: types, encode, palette, quantizing.** Red first:
encode vectors (every attribute and colour form, the tessera subset
byte-identical to today's `_stilum_emittere` outputs), the 256 palette
spot-checked (16 ANSI, cube corners, grays), `stilus_quantizare`
pinned against tessera's `_cclvi` on a grid. Plants: bold emitted as
`2`; the cube level rounding the wrong way at a tie.

**T2 — decode.** `stilus_applicare` over a `series_terminalis` CSI
token. Ghostty's 31 `sgr.zig` tests ported; every separator style and
colour form; truncated forms; unknowns counted. The ROUND TRIP
`applicare(codificare(NIHIL, s)) == s` over a generated grid of
styles. Plants: `:` treated as `;` (breaks `38:2::r:g:b`); a reset
(22) clearing only bold, not faint.

T2 as built: 117 asserts; the round trip found that a full style
exceeds the 24-parameter CSI cap (Ghostty MAX_PARAMS): the encoder
splits into a second sequence by whole groups.

**T3 — tessera emits through the codec.** `_stilum_emittere` becomes a
lossless conversion `TesseraStilus` → `StilusTerminalis` +
`stilus_codificare(aed, NIHIL, …)`; `TesseraColores` maps to the
encode option; `_cclvi` deleted (now `stilus_quantizare`). The bar:
tessera's byte goldens and vectors UNCHANGED; the amalgam vendors
`stilus_terminalis`. Plant: the conversion dropping one ornament.

T3 as built: bytes identical; the ornament plant first passed (no
tessera test pinned ornament bytes) - pins added; vendoring found a
false-convergence bug in the excludenda harvester (fixed, Fran's call).

**T4 — RELATIO.** Phase log; terminal-planning: 004 → completed_, 005
→ completed_ (its last row, SGR emit, lands in T3); briar MUTATIONES.

## Gates

T1/T2: radix + generata; T3: tessera (THE bar), radix, briar,
aedilis, generata; T4: briar, generata.
