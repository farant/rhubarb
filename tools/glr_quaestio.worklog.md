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

**Future work (Phase 2)**: Parse `#define INT_NT_*` macros from `lib/arbor2_glr_tabula.c` using arbor1's PRESERVARE preprocessor mode to also dynamically extract non-terminal names. Currently NT_NOMINA array remains hardcoded.
