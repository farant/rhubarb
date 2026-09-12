# icones — spec v1 (`.icns` and `.iconset` from one image)

*Born 2026-09-12, the third and last of the libraries that came out of a
question about packaging a binary as a double-clickable `.app`
(**plist** → **fasciculum** → **icones**, decretum 01M29AKBXD). plist
and fasciculum both shipped before this was written
(`plist-spec.md` sealed `21de2be1`; `fasciculum-spec.md` sealed
`94098956`, and its bundle demonstrably launches), which is what makes
this library's job narrow: `fasciculum` PLACES an icon file and names it
in `CFBundleIconFile`, and has no way to make one. Fran's decisions are
marked DECISUS; everything else is PROPOSITUM and was agreed in
conversation unless marked OPEN. Every "exists" claim cites the header
or file:line it rests on, and every claim about Apple's format cites a
measurement taken with `iconutil`, not documentation. English prose,
Latin identifiers.*

*Discipline note: an icon pipeline is where image-quality work hides
behind a container format. The container is a weekend; the resampler is
the part that decides whether a 16px icon is legible. This spec keeps
them in separate phases so the easy half cannot be mistaken for the
whole job.*

Status vocabulary: **DECISUS** (Fran decided) · **PROPOSITUM**
(proposed, agreed in conversation) · **OPEN** (undecided) ·
**PARCATUM** (deliberately deferred, §10) · **AUDIENDUM** (not
verified; §11).

## 1. Purpose and scope

From one source image, produce the icon artifacts a macOS bundle needs:
an `.iconset` directory of PNGs and an `.icns` container, with
downscaling that does not destroy the image.

In scope: a centred-square crop of a non-square source; area/box
downscaling; PNG encoding per size; the `.iconset` layout with Apple's
filenames; the `.icns` container and its type codes; and a result that
reports which sizes were emitted and which were skipped.

Out of scope, each a §10 row with the pull that would bring it in:
letterboxing, sharpening, reading `.icns` PIXELS, a metadata-only chunk
reader, and non-Apple icon formats.

Consumers: `fasciculum` (which today places an icon it cannot make),
`briar -app` later, and any tool wanting a thumbnail set. No consumer is
built by this spec — the layer stands alone first, as in fasciculum
(D4 there, and the same call here).

## 2. Decisions

| # | Decision | Source |
|---|---|---|
| D1 | Non-square source → CROP TO CENTRE; letterbox deferred | DECISUS |
| D2 | Source smaller than a size → emit only the sizes it covers, and REPORT the omissions | DECISUS |
| D3 | Write-only v1; a metadata-only chunk reader is the likely first deferral to graduate | DECISUS |
| D4 | The area resampler goes into `imago_opus`, not into icones | PROPOSITUM |
| D5 | We write `.icns` ourselves; `iconutil` is an ORACLE, never a dependency | PROPOSITUM |
| D6 | The pure core encodes each size ONCE; `.iconset` and `.icns` are projections of the SAME bytes | PROPOSITUM |
| D7 | icones takes an `Imago`, not a path — the caller decodes | PROPOSITUM |
| D8 | Area averaging is ALPHA-WEIGHTED; this is correctness, not polish | PROPOSITUM |
| D9 | PNG under `ic04`/`ic05` in v1; the table is DATA and `icp4`/`icp5` is a one-row fallback | PROPOSITUM |
| D10 | A source covering NOTHING (side < 16) is a REFUSAL, not an empty success | PROPOSITUM |

## 3. The value and the result

**D7, and why it is the load-bearing decision.** icones takes a decoded
`Imago` (`include/imago_typus.h`), never a file path. Its closure is then
`imago_typus.h` + `imago_png` + `flatura` — all ours. Decoding in this
house is vendored `stb_image`, and `imago_typus.h` exists precisely so
that wanting the TYPE does not import the DECODER: its own header records
that when `Imago` lived in `imago.h`, including `imago_png.h` dragged
`lib/imago.c` + `vendor/stb_image.h` into every vitrea app's closure
(measured with `bin/aedilis --partes`; forum went `vendores 1` →
`vendores 0` after the split). So the CLI tool decodes and icones stays
free of vendored code. It also makes the whole core testable from a
synthetic 4×4 image with no PNG fixture on disk.

```c
nomen enumeratio { ICONES_RECIDERE_CENTRUM } IconesModusRecidendi;

/* Latera petita ut VEXILLA: universum magnitudinum fixum et parvum est,
 * ergo tabula bitorum exacta est et piscina non eget; tabula a vocante
 * data latera admitteret quibus nullus codex .icns respondet. */
#define ICONES_LATERA_XVI       I      /*   16 px */
#define ICONES_LATERA_XXXII     II     /*   32 px */
#define ICONES_LATERA_LXIV      IV     /*   64 px */
#define ICONES_LATERA_CXXVIII   VIII   /*  128 px */
#define ICONES_LATERA_CCLVI     XVI    /*  256 px */
#define ICONES_LATERA_DXII      XXXII  /*  512 px */
#define ICONES_LATERA_MXXIV     LXIV   /* 1024 px */
#define ICONES_SETUM_APPLE      (I|II|IV|VIII|XVI|XXXII|LXIV)

nomen structura {
      constans Imago*  fons;
               chorda  titulus;   /* "AppIcon" -> AppIcon.iconset */
 IconesModusRecidendi  recidere;
                  i32  latera_petita;  /* ZEPHYRUM = ICONES_SETUM_APPLE */
} Icones;

nomen structura {
       i32  latera;   /* 16, 32, 64 ... pixela */
    chorda  semita;   /* "icon_16x16.png" */
    chorda  octeti;   /* PNG - IIDEM octeti quos .icns fert */
} IconesPars;

nomen structura {
    Xar*  partes;         /* IconesPars, una per PLAGULAM */
     i32  omissa;         /* vexilla laterum quae fons non tulit */
     i32  latera_fontis;  /* quadratum post recisionem */
} IconesFructus;
```

- **The crop mode is a one-value enum on purpose.** Letterbox is a named
  deferral and house enums are appended-never-reordered, so the deferral
  lands without changing a signature. A `b32 recidere` would have to be
  replaced.
- **The result cannot be a bare `b32`** (D2): a caller writing a bundle
  needs to know 1024 is absent so it can decide whether that matters.

## 4. The size table (measured, not documented)

Ten `.iconset` files, **seven distinct renderings** — three pixel sizes
appear twice under different logical names:

| px | `.iconset` files | `.icns` codes |
|---|---|---|
| 16 | `icon_16x16.png` | `ic04` |
| 32 | `icon_16x16@2x.png`, `icon_32x32.png` | `ic11`, `ic05` |
| 64 | `icon_32x32@2x.png` | `ic12` |
| 128 | `icon_128x128.png` | `ic07` |
| 256 | `icon_128x128@2x.png`, `icon_256x256.png` | `ic13`, `ic08` |
| 512 | `icon_256x256@2x.png`, `icon_512x512.png` | `ic14`, `ic09` |
| 1024 | `icon_512x512@2x.png` | `ic10` |

Measured 2026-09-12 by feeding `iconutil --convert icns` a full
ten-file iconset of PNGs: it emitted `ic04 ic05 ic07 ic08 ic09 ic10 ic11
ic12 ic13 ic14` plus a 310-byte `info` chunk, and the duplicate pairs
came out byte-identical (`ic13`/`ic08` both 34,524; `ic14`/`ic09` both
102,923) — which is the evidence behind D6.

Two further measurements worth keeping:

- **Apple ships FOUR entries, not ten.** `Calculator.app`'s `AppIcon.icns`
  holds `ic13 ic11 ic04 ic07` — 16, 32, 128, 256 — and **no 512 or
  1024 at all**. The ten-entry set is a documentation ideal. So D2's
  "emit what the source covers" is the normal case, not a degraded one.
- **Partial sets are legal.** A two-file iconset converted at `rc=0`.
  So skipping sizes carries no format penalty.

**D9, the one uncertain row, handled as data.** `ic04`/`ic05` are the only
codes for 16 and 32 px, and Apple fills them with ARGB, not PNG. A
hand-built `.icns` carrying PNG in `ic04` was read back by `iconutil` at
`rc=0` and by `sips` — and came back RE-ENCODED (1,290 bytes in, 763
out), so the tool genuinely decoded the payload. That is good evidence
and not proof of rendering (§11). The table is DATA; if the AUDIENDUM
fails, `icp4`/`icp5` — documented as PNG-capable — is a one-row edit.
Dropping 16px instead is not an option: it is the Finder list-view size.

## 5. The area resampler (in `imago_opus`)

`include/imago_opus.h` offers exactly two modes today,
`IMAGO_SCALA_PROXIMUS` and `IMAGO_SCALA_BILINEARIS`. Bilinear samples a
2×2 neighbourhood regardless of scale: downscaling 1024→16 is 64×, so
each output pixel covers 4,096 source pixels and bilinear reads FOUR of
them, discarding 99.9% of the image. At icon ratios that is not a
quality preference but the difference between a picture and noise.

`IMAGO_SCALA_AREA` is APPENDED after the existing two (house law: enums
append, never reorder), so no existing caller changes.

**D8 — alpha-weighted accumulation.** `imago_typus.h` states the buffer
is RGBA8 **not premultiplied**. Averaging RGB while ignoring alpha lets
invisible pixels contribute colour — a transparent green pixel
`(0,255,0,0)` drags green into its neighbours — and icons are mostly
transparent edges, so this is the dominant visual defect of a naive
implementation:

```
sum_w   = Σ w
sum_a   = Σ (a · w)
sum_rgb = Σ (rgb · a · w)
out_a   = sum_a / sum_w
out_rgb = sum_a > ZEPHYRUM ? sum_rgb / sum_a : ZEPHYRUM
```

**Fractional coverage is required, because our happy path hides its
absence.** From a 1024px source every icon size is an exact power-of-two
ratio, so integer binning would pass every test we would naturally
write; from a 1000px source, 16px is 62.5× and every footprint edge is
partial. Edge pixels are weighted by their fractional overlap.

**Accumulate in `s64`.** The existing bilinear uses `s32` with a 12-bit
fixed-point fraction, ample for 2×2. An area footprint of 64×64 sums to
~266M (inside `s32`), but a 256× reduction passes 4 billion and wraps.
`-Wno-long-long` is already in the house flags (`tools/vexilla.sh`).

**At 1:1, copy — do not resample.** The reason is not that bilinear
misbehaves at unit scale; measured, it does not (`lib/imago_opus.c:61-65`
region: `scale = ((fons-1)·FP_ONE)/(dest-1)` is exactly `FP_ONE` when the
dimensions match, so the fraction is zero and each pixel passes through).
The reason is that **the fit-within API cannot promise unit scale**:
`imago_extrahere_et_scalare` and `imago_scalare_ad_limites` derive the
output from MAX bounds preserving aspect
(`lib/imago_opus.c:263-271`) — `scale_fp = min(max·FP/fons, …)` then
`nova = (fons · scale_fp) >> SCALE_FP_SHIFT`, where `SCALE_FP_SHIFT` is
16 — so the ratio is truncated and then the product truncated again,
and asking 512 from a 512-wide crop can yield 511. Every pixel is then
interpolated. Therefore **icones computes exact target dimensions
itself** and does its own row-wise copy when the crop already equals the
target. Neither path has a unit-scale early-out; the only guards are
zero-dimension checks (`:173` in `imago_scalare`, `:246` and `:320` in
the aspect-preserving pair).

Noted, not fixed here: the existing bilinear maps `[0, fons−1]` onto
`[0, dest−1]` — corner-aligned, not centre-aligned — so it carries a
half-pixel bias on top of under-sampling.

## 6. Writing

```c
b32     icones_reddere          (constans Icones*, IconesFructus*,
                                 IconesStatus*, chorda* sedes_vitii,
                                 Piscina*);                    /* PURUM */
b32     icones_iconset_scribere (constans IconesFructus*,
                                 constans character* via_radicis,
                                 IconesStatus*, chorda*, Piscina*);
chorda  icones_icns_codificare  (constans IconesFructus*,
                                 IconesStatus*, Piscina*);     /* PURUM */
b32     icones_icns_scribere    (constans IconesFructus*,
                                 constans character* via,
                                 IconesStatus*, chorda*, Piscina*);
```

The pure/thin-twin split is plist's shape exactly (`plist_scribere` pure,
`plist_scribere_plagulam` its I/O twin), so the whole `.icns` format is
unit-testable with no filesystem.

**Container.** Magic `icns`, big-endian total length, then chunks of
4-char code + big-endian length (INCLUDING the 8-byte header) + payload.
A single-chunk `.icns` is legal — measured. The `info` chunk `iconutil`
emits is optional for reading — measured by a control that omitted it.

**Byte sharing (D6).** Parts are per FILE, so a writer iterates; but
`icon_16x16@2x.png` and `icon_32x32.png` carry the same `chorda` pointing
at the same `datum`, because they are the same 32px image. The
`.iconset` writer creates a possibly-nested directory with
`filum_directorium_creare_cum_parentibus` (`include/filum.h`, added for
fasciculum 2026-09-12 — this is its second consumer, which is the
argument for having put it in `filum`).

## 7. Refusals

```c
nomen enumeratio {
    ICONES_SUCCESSUS = ZEPHYRUM,
    ICONES_ERROR_DESUNT,        /* fons aut titulus */
    ICONES_ERROR_FONS,          /* pixela NIHIL aut dimensio <= 0 */
    ICONES_ERROR_MINIMUS,       /* latus < XVI: nihil tegitur */
    ICONES_ERROR_LATERA,        /* vexillum sine codice .icns */
    ICONES_ERROR_PNG,           /* imago_png recusavit */
    ICONES_ERROR_DIRECTORIUM,
    ICONES_ERROR_SCRIPTIO,
    ICONES_ERROR_MEMORIA
} IconesStatus;
```

Every refusal sets `sedes_vitii` — the missing field's name, the
offending size, or the relative path. Two are decisions, not
bookkeeping:

- **`_MINIMUS` (D10)** is where "report the omissions" stops being
  enough: a 12px source covers nothing, and an empty set with a full
  omissions list would be a success that produced no icon.
- **`_LATERA`** refuses an unknown requested size rather than ignoring
  it. Silently dropping a bit a caller deliberately set is the failure
  shape this arc keeps finding.

## 8. Probationes and gates

`probationes/probatio_icones.c`, wired with
`./tools/compile_tests_fontes_generare.sh` — run AFTER `lib/icones.c`
exists (fasciculum's plan had this backwards and it cost a confusing
red).

| # | Gate |
|---|---|
| I1 | The pure core from a synthetic 4×4 `Imago`: part count, Apple's filenames, and `pars_16at2x->octeti.datum == pars_32->octeti.datum` — proving SEVEN encodes, not ten; nothing written to disk |
| I2 | The resampler: half-opaque-red / half-TRANSPARENT-green 2:1 (fails loudly on naive averaging), plus **Python-pinned constants** for a fractional 3→2 ratio |
| I3 | Container structure, pure: magic, declared length == actual, chunk table walks, every payload carries a PNG signature |
| I4 | **`iconutil` BOTH WAYS**: our `.icns` → `--convert iconset` → extracted PNGs match our sizes; our `.iconset` → `--convert icns` → its chunk table matches ours (same codes, same sizes) |
| I5 | Every refusal in §7 by its own status AND its `sedes_vitii`, **with a plant per branch** |
| I6 | Writers: `.iconset` into a nested virgin directory, ten files, duplicates byte-identical ON DISK; `.icns` file byte-equal to `icones_icns_codificare`'s pure output |
| I7 | Quality: `imago_collatio` diffs our 16px against `sips`' 16px — a GENEROUS ceiling on max delta, measured first, calibrated to catch "we shipped nearest by mistake" or a systematic half-pixel bias, NOT to match Apple's filter |

Every gate is born red by a planted fault. Three laws this arc paid for:
a plant that stays GREEN names a coverage hole as readily as a dead gate
(two failure branches that refuse with the same shape look like one path
from outside); a test that varies ONE field proves ONE field is compared;
and ask what an oracle FORGIVES before treating its silence as proof —
`imago_png` learned that when stb_image validated neither Adler-32 nor
chunk CRCs and 54 assertions stayed green on a corrupted file.

**The oracle, and its honest limit.** `iconutil` is stronger than plist's
`plutil`: it converts BOTH directions, so Apple's own tool can be made to
agree with our container for identical input. It still cannot answer
whether Finder DRAWS our icon — see §11.

## 9. Phase plan (test-first; each phase ends green)

| Phase | Work |
|---|---|
| P1 | `imago_opus`: `IMAGO_SCALA_AREA`, alpha weighting, fractional coverage, `s64` accumulators, 1:1 copy + I2 — **its own commit**, as `filum`'s rows were before fasciculum |
| P2 | Value, pure core, `.iconset` projection + I1 |
| P3 | `.icns` pure encode + I3, I4 |
| P4 | Writers, refusals + I5, I6 |
| P5 | I7, by-hand fumus, worklog, spec as-built, whole-suite seal |

## 10. Named deferrals (PARCATUM)

| Row | Pull |
|---|---|
| Letterbox | A caller wanting padding rather than a crop. |
| Sharpening after downscale | If ours look soft beside Apple's at 16px. Real pipelines hand-tune small sizes. |
| Reading `.icns` PIXELS | A decompose tool. Needs `stb_image` in the closure — the cost D7 exists to avoid. |
| Metadata-only chunk reader | `fasciculum` validating an icon rather than placing bytes (its §11 AUDIENDUM). Needs nothing vendored — likely the first to graduate. |
| `icp4`/`icp5` instead of `ic04`/`ic05` | The §11 AUDIENDUM failing. One row of the §4 table. |
| The `info` chunk | A tool that demands it; measured optional for reading. |
| `.ico`, Linux hicolor | A consumer on another platform. |

## 11. Risks, notes, AUDIENDA

- **AUDIENDUM:** whether Finder DRAWS a PNG-in-`ic04` icon at 16px.
  `iconutil` and `sips` both accept it, and `iconutil` re-encoded the
  payload (so it decoded it) — but a converter's acceptance is not a
  renderer's. This is P5's by-hand fumus: build a bundle with
  `bin/fasciculum_instrumentum`, drop our `.icns` in, open it, look.
  Exactly fasciculum's pattern, and the machinery already exists.
- **AUDIENDUM:** whether our area downscale is visually competitive with
  Apple's at 16px. I7 pins a ceiling, not equality.
- **Risk, named:** `imago_opus` computes `dest_lat · dest_alt · IV` and
  `src_y · fons_lat · IV` in **`s32`**. A 4096² source is inside range; an
  8192² source passes 2.1 billion and WRAPS — the `i64`-wrap family, a
  silently wrong offset rather than a crash. Not icones' bug, but our
  resampler will sit beside it; a row for `imago_opus`.
- **Risk, low:** D9 could be wrong in the other direction — Finder might
  accept PNG in `ic04` while some other consumer (an older OS, a
  third-party tool) does not. We test what we ship on.
- **Note:** `.icns` payloads may be JPEG 2000 in Apple's own files
  (`sips` reports some as such). We emit PNG only, which is legal
  everywhere we measured.

## Cross-references

- `project-specs/plist-spec.md`, `project-specs/fasciculum-spec.md` —
  the two libraries below, and fasciculum's §12 as-built (seven
  corrections worth reading before writing this plan).
- `include/imago_typus.h` (the type without the decoder),
  `include/imago_opus.h`, `include/imago_png.h`, `include/flatura.h`,
  `include/imago_collatio.h`, `include/filum.h`.
- `lib/imago_png.worklog.md` — the lenient-oracle lesson in full.
- `tools/fasciculum_fumus.sh`, `tools/briar_fumus.sh` — the by-hand
  smoke pattern §8 borrows.
