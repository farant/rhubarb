# glr_quaestio worklog

## 2026-01-12: Dynamic Enum Extraction via Arbor1

Replaced the hardcoded `LEXEMA_NOMINA[]` lookup table with dynamic extraction using arbor1 to parse `include/arbor2_lexema.h` at startup.

**Problem**: The hardcoded table could get out of sync when new tokens were added to the `Arbor2LexemaGenus` enum.

**Solution**: Use arbor1's C parser to parse the header file and extract enum values automatically.

**Key learnings/gotchas**:

1. **Typedef enum AST structure**: When arbor1 parses `typedef enum { ... } TypeName;`, the enum specifier has no `titulus` (tag name), and the declarators array is empty. The typedef name isn't easily accessible. We verify the correct enum by checking that the first enumerator starts with the expected prefix (`ARBOR2_LEXEMA_`).

2. **Union member access**: The `ArborNodus.datum` field is a union. The ENUMERATOR nodes store their name in `datum.folium.valor`. Initially tried to also access `datum.genericum.liberi` to check for explicit enum values, but this caused a segfault because the same memory was being interpreted as two different types. Since `arbor2_lexema.h` enums are sequential (no explicit `= N` assignments), we just use sequential values starting from 0.

3. **i32 vs integer**: In this codebase, `i32` is unsigned int and `integer` is signed int. Using -1 with i32 in ternary expressions causes sign conversion warnings.

**Files changed**:
- `tools/glr_quaestio.c`: Added `extrahere_enum_ex_filo()` function, global piscina/intern state, removed hardcoded LEXEMA_NOMINA array

## 2026-01-12: Dynamic NT Extraction (Phase 2)

Added dynamic extraction of `#define INT_NT_*` macros from `lib/arbor2_glr_tabula.c`.

**Initial approach (failed)**: Tried using arbor1's PRESERVARE mode to parse the file and extract directive nodes. This caused the parser to hang/infinite loop on the 4500-line file - likely a bug in arbor1's PRESERVARE mode.

**Final approach**: Simple text-based extraction - read file line by line, match lines starting with `#define INT_NT_`, parse name and value with string operations. Much faster and more reliable for this regular pattern.

**Key points**:

1. **Pattern matching**: Lines match `#define INT_NT_<NAME> <VALUE>` format
2. **Two-pass algorithm**: First pass counts matches, allocate table, second pass extracts
3. **Fallback preserved**: The hardcoded `NT_NOMINA[]` array is kept as fallback - it provides aliases like "EXPRESSIO" -> INT_NT_EXPR that the dynamic extraction doesn't capture

**Files changed**:
- `tools/glr_quaestio.c`: Added `extrahere_defines_ex_filo()` using simple file I/O, added `g_nt_tabula`/`g_nt_numerus` globals, updated `initializare_tabulas()` and `parsere_nt_titulus()`

**Result**: Tool now dynamically extracts both token types (89 values from enum) and NT types (26 values from #defines) at startup. No hardcoded values need manual updates when grammar changes.
