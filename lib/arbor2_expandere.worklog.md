# arbor2_expandere.c Worklog

## 2026-01-09 - Typedef Collection (Phase 2)

Added automatic typedef detection during macro expansion.

### Key additions:

- `Arbor2TypedefInfo` struct stores metadata: `titulus` (name), `layer_discovered`, `origo` (source location), `est_certum` (whether detected via `typedef` keyword or `nomen` heuristic)

- `_detectare_typedef()` function (~100 lines) detects typedefs by:
  1. Looking for `ARBOR2_LEXEMA_TYPEDEF` keyword or `"nomen"` identifier
  2. Scanning to semicolon while tracking brace/paren depth
  3. For struct typedefs: extracting name after closing brace at depth 0
  4. For function pointers: detecting `(* IDENTIFIER )` pattern
  5. Otherwise: taking last identifier at depth 0

- Modified `arbor2_expansion_addere_typedef()` to store `Arbor2TypedefInfo*` instead of simple flag

- Added `arbor2_expansion_quaerere_typedef()` to retrieve typedef info

- Integrated detection into `_expand_layer()` for both `typedef` keyword and `nomen` identifier

### Design notes:

- `est_certum = VERUM` for actual `typedef` keyword (definite)
- `est_certum = FALSUM` for `nomen` identifier (heuristic - could theoretically be a variable named "nomen")
- `layer_discovered = -1` marks API-injected typedefs
- Detection runs on every expansion layer to catch macro-generated typedefs
- Reuses `Arbor2OrigoMeta` from arbor2_token.h for provenance consistency

## 2026-01-16 - Fixed Preprocessor Gaps for Phase 3

### Issue 1: Function-like macros from source #define not expanding

**Problem:** `#define SQUARE(x) ((x)*(x))` followed by `SQUARE(5)` - macro was correctly parsed as function-like (est_functio=1), but the invocation was not being expanded.

**Root Cause:** After `_processare_define()` finishes, position `i` points to the first token AFTER the #define. However, `_expand_layer()` then does `perge` (continue) which causes `i++`, effectively skipping that first token.

**Fix:** Added `i--` before `perge` after processing #define and #undef directives. This compensates for the loop increment.

**Location:** `_expand_layer()` lines ~1379-1383 and ~1429-1433

### Issue 2: `__VA_ARGS__` substitution not implemented

**Problem:** Variadic macros were recognized (est_variadic=1) but `__VA_ARGS__` in the macro body was never substituted.

**Fix:** Added special case in `_expandere_macro()` to detect `__VA_ARGS__` identifier and substitute with all arguments beyond named parameters, with commas inserted between them.

**Location:** `_expandere_macro()` lines ~1269-1350

### Test Coverage

Updated tests from 69 to 92 assertions with proper verification of:
- Function-like macro via #define (checks actual expansion tokens)
- Stringification via #define (checks STRING_LIT result)
- Token pasting via #define (checks pasted identifier)
- Variadic macros (checks each substituted arg and comma)

## 2026-01-17 - Built-in Latina.h Registration (Phase 3.4)

Added `arbor2_includere_latina()` function to register all latina.h macros for Latin C89 parsing.

### Implementation

- Static table `LATINA_MACROS[]` at lines 1827-1878 contains all latin→C mappings
- Note: Had to use `titulus` instead of `nomen` for struct field name (since `nomen` is itself a macro that expands to `typedef`)
- Categories: keywords, type keywords, qualifiers, types, operators, constants, storage hints

### Macros registered

| Latin | C | Category |
|-------|---|----------|
| si, alioquin, per, dum, fac, commutatio, casus, ordinarius, frange, perge, redde, salta | if, else, for, while, do, switch, case, default, break, continue, return, goto | Keywords |
| structura, unio, enumeratio, nomen | struct, union, enum, typedef | Type keywords |
| constans, volatilis, staticus, externus, registrum, sponte | const, volatile, static, extern, register, auto | Qualifiers |
| vacuum, character, integer, brevis, longus, signatus, insignatus, fluitans, duplex | void, char, int, short, long, signed, unsigned, float, double | Types |
| magnitudo | sizeof | Operators |
| NIHIL, VERUM, FALSUM | NULL, 1, 0 | Constants |
| hic_manens, interior, universalis | static | Storage hints |

### Usage

```c
Arbor2Expansion* exp = arbor2_expansion_creare(piscina, intern);
arbor2_includere_latina(exp);
/* Now "integer x" parses as declaration because integer→int */
```
