# fons worklog

## 2026-10-09 - 40 invisible Latin-1 glyphs (Fran: '×' blank, not tofu)

`fons_codepoint_ad_glypham` sent U+00A1-00FF straight to the same slot
of fons_6x8; 41 slots were all zeros, so those characters drew as
NOTHING (no tofu, so nobody noticed). The substitutions further down
(`× -> x`, `¹ -> 1`, ...) were dead code for the same reason. Now:
40 glyphs drawn (accented capitals use a TWO-row accent over a
five-row body, matching the lowercase accents - the older Ä/É/Ö
squeeze a one-row accent over six rows, where circumflex and umlaut
would look alike), and an empty Latin-1 slot falls through to the
substitutions, then tofu; U+00AD (soft hyphen) stays deliberately
blank. Test: every Latin-1 code point maps to its own slot, nothing
mapped is blank. Drafted with a contact-sheet script (glyphs as
'.#' strings -> PNG at 4x) - the way to design any future glyphs.
Noticed, untouched: Ñ (0xD1) has no visible tilde.

## 2026-10-13 — fons_8x16 (Nix8810 M15) imported; attribution for fons_6x8

Fran wants an oversized header face (agenda-plan V3). Source: The
Ultimate Oldschool PC Font Pack v2.2 (int10h.org, VileR), CC BY-SA 4.0 -
LICENSE.TXT in ../oldschool_pc_font_pack_v2.2_FULL (outside the repo,
like the reference clones). `tools/fon_ad_fons.c` reads the Windows
bitmap font (`Bm437_Nix8810_M15.FON`: MZ -> NE -> RT_FONT 0x8008 -> FNT
v2/v3 glyph table; 8 px wide = one byte per row) and writes
include/fons_8x16.h with the attribution and the source's SHA-256
(house friatio == shasum -a 256, checked). The formatter leaves the tool
output unchanged, so the committed file IS the tool output (regenerable
byte for byte; not marked GENERATUM because its input lives outside the
repo - no gate could rerun it).

Design change vs the first plan: glyphs stay in NATIVE CP437 order. The
6x8's high slots are a hybrid (Latin-1 at their codepoints, but the
substitutions for degree, superscript 2, 1/4 1/2 3/4 point at leftover
HP 100LX/CP437 positions 0xF8, 0xFD, 0xAB-0xAD), so one shared mapping
cannot serve both. The 8x16 gets its own Unicode -> CP437 lookup in V3b.

fons_6x8.h now carries its origin: HP 100LX 6x8 from the same pack (CC
BY-SA 4.0), Alcuin by Aliivibrio (itch.io, "free to use for any
purpose"), 40 Latin-1 glyphs drawn in-house (a11400ef) - so the file as
a whole is CC BY-SA 4.0.
Plant: one row of 'A' changed -> red (probatio_fons).

## 2026-10-13 — V3b: Mandatum.fons is read; 8x16 lookup and metrics

`Mandatum.fons` ("index fontis") existed but the rasterizer always drew
6x8. Now FONS_PARVUS (0, every existing caller) / FONS_TITULI (1, 8x16):
delineare_mandata -> tabula_pixelorum_pingere_chordam_fonte, which
forwards FONS_PARVUS (and any unknown id) to the old 6x8 path untouched
and draws FONS_TITULI glyph by glyph (bounds-checked per pixel - header
text is short; no fast path yet). fons_8x16.h is included only by
fenestra_textus.c, so the 4 KB table exists once.
`fons_8x16_codepoint_ad_glypham`: ASCII direct (0x01-0x1F stay the CP437
dingbats, like the 6x8), the full CP437 0x80-0xFF and 0x01-0x1F tables
inverted, U+2302 -> 0x7F; then base letters for Latin-1 CP437 lacks
(A grave -> A, o slash -> o, ...), the 6x8's substitutions (quotes,
dashes, ellipsis, minus) and Greek capitals that look Latin; else 0x7F
(house = tofu, as in the 6x8). `fons_latitudo/altitudo(fons)` give glyph
metrics (unknown id = 6x8) for layouts that centre or measure text.
Plant: rasterizer passes 0 instead of x->fons -> red (pixel compare of
"AB" against fons_8x16 bits, which also proves the 8 px advance).
