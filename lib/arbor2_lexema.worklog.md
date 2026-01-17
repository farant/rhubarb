# arbor2_lexema.c Worklog

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
