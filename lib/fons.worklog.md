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
