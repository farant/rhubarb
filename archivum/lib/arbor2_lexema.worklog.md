# arbor2_lexema.c Worklog

## 2026-01-16 - Phase 2.7: NOVA_LINEA Roundtrip Preservation

Moved NOVA_LINEA from main token stream into spatia. This enables proper roundtrip serialization while maintaining preprocessor directive detection.

### Problem

NOVA_LINEA tokens were emitted in the main token stream, but:
1. Test helpers skipped them (couldn't serialize)
2. Even if not skipped, serializer walks AST nodes, so unattached tokens were lost
3. Result: newlines lost during roundtrip

### Solution: NOVA_LINEA in spatia (Option D from plan)

**Before:** `_colligere_spatia()` broke on newlines, then main loop emitted NOVA_LINEA tokens.

**After:** Newlines collected as NOVA_LINEA tokens in spatia like any other whitespace:
- First newline goes in `spatia_post` of preceding token
- Subsequent newlines collected in `spatia_ante` of following token

### Preprocessor Adaptation

The preprocessor relied on NOVA_LINEA in stream for directive boundary detection. Updated to check spatia instead:
- `_habet_nova_linea_post(tok)` - checks if token has NOVA_LINEA in spatia_post
- `_habet_nova_linea_ante(tok)` - checks if token starts on new line

Updated `_processare_define()` and `_processare_undef()` to use these helpers.

### GLR Blank Line Detection

Updated `_habet_lineam_vacuam_ante/post()` and `_est_commentum_finis_lineae()` to check for both CONTINUATIO and NOVA_LINEA tokens in spatia.

### Testing

All tests pass:
- `probatio_arbor2_lexema` - Rewritten for Phase 2.7 (41/41)
- `probatio_arbor2_expandere` - PASSES (30/30)
- `probatio_arbor2_glr` - PASSES (1847/1847)
- `probatio_arbor2_scribere` - PASSES (81/81) - newline preservation tests now work

---

## 2026-01-16 - Phase 2.6: Tokenize Whitespace

Replaced `Arbor2Trivia` with explicit whitespace/comment tokens. This is a significant architectural change.

### What Changed

**Before:** Whitespace/comments stored as opaque `Arbor2Trivia` structs with flags:
- `est_commentum` - true if comment
- `est_c99` - true if // line comment
- `est_continuatio` - true if contains \\\n

**After:** Whitespace/comments are now typed `Arbor2Lexema*` tokens with specific genus:
- `SPATIA` - contiguous spaces (stores actual " " chars in valor)
- `TABULAE` - contiguous tabs (stores actual "\t" chars in valor)
- `CONTINUATIO` - line continuation \\\n
- `COMMENTUM_CLAUSUM` - block comment /* */
- `COMMENTUM_LINEA` - line comment //

### Key Design Decisions

1. **Separate SPATIA vs TABULAE** - Keeps them distinct for potential formatting tools. Each token stores the actual characters in `valor`.

2. **No mixed whitespace tokens** - A run of spaces becomes one SPATIA, then tabs become TABULAE. Simplifies querying.

3. **CONTINUATIO is atomic** - Contains exactly one newline. Two consecutive CONTINUATIO tokens = blank line (for floating comment detection).

4. **Comment tokens store full text** - Including delimiters (`/* */` or `//`). Extraction of content happens at higher level (GLR comment promotion).

### Files Affected

- `include/arbor2_lexema.h` - New enum values, removed Arbor2Trivia, renamed fields
- `lib/arbor2_lexema.c` - New _creare_spatium, rewrote _colligere_spatia
- `lib/arbor2_scribere.c` - Updated to iterate Arbor2Lexema*
- `lib/arbor2_glr.c` - Updated comment promotion to check genus
- `lib/arbor2_expandere.c` - Field renames

### Testing

- `probatio_arbor2_lexema` - PASSES (38/38)
- `probatio_arbor2_expandere` - PASSES (30/30)
- GLR/scribere tests have pre-existing EOF handling issues unrelated to this change
