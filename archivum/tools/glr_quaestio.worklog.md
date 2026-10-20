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

## 2026-01-12: Fix Dual NT Numbering Scheme Bug

**Problem**: The `goto` command was returning wrong results. For example, `goto FACTOR` showed `State 0 -> State 24` when the source code clearly had `{ 0, INT_NT_FACTOR, 3 }`.

**Root cause**: Two different NT numbering schemes exist:

1. **`Arbor2NonTerminalis` enum** (in arbor2_glr.h) - used by REGULAE array
   - DISIUNCTIO=0, CONIUNCTIO=1, AEQUALITAS=2, ..., FACTOR=6, ...

2. **`INT_NT_*` defines** (in arbor2_glr_tabula.c) - used by GOTO table
   - EXPR=0, TERM=1, FACTOR=2, ..., CONIUNCTIO=24, DISIUNCTIO=25

The previous "fix" switched to using `Arbor2NonTerminalis` values to fix the `rules` command, but that broke `goto` because the GOTO table uses `INT_NT_*` values internally.

**Solution**: Extract both numbering schemes and use the appropriate one for each command:
- `goto` command: uses `INT_NT_*` values (via `parsere_int_nt_titulus()`)
- `rules` command: uses `Arbor2NonTerminalis` values (via `parsere_nt_titulus()`)

**Implementation**:
1. Added `extrahere_int_nt_ex_filo()` - text-based extraction of `#define INT_NT_*` from arbor2_glr_tabula.c
2. Added `g_int_nt_tabula` / `g_int_nt_numerus` globals
3. Added `parsere_int_nt_titulus()` lookup function
4. Updated `cmd_goto()` to use `parsere_int_nt_titulus()`
5. Updated `initializare_tabulas()` to extract both tables

**Result**: Both `goto FACTOR` (returns 3) and `rules FACTOR` (finds 5 rules) now work correctly.

## 2026-01-12: NT Extraction Bug Fix + Phase 3 Commands

### Bug Fix: Wrong NT Source

The Phase 2 extraction of `#define INT_NT_*` macros from `arbor2_glr_tabula.c` was incorrect. The REGULAE array uses `Arbor2NonTerminalis` enum values from `arbor2_glr.h`, NOT the `INT_NT_*` defines. The INT_NT_ defines are only used for the GOTO table.

**Symptom**: `rules EXPR` returned rules with LHS=DISIUNCTIO because INT_NT_EXPR=0 maps to ARBOR2_NT_DISIUNCTIO=0, but they're different numbering schemes.

**Fix**: Changed NT extraction to use `extrahere_enum_ex_filo()` on `include/arbor2_glr.h` with prefix `ARBOR2_NT_` instead of text parsing #defines. Removed the now-unused `extrahere_defines_ex_filo()` function.

### Phase 3: New Query Commands

Added two new commands:

1. **`rules <NT>`** - Find all grammar rules where the left-hand side equals the given non-terminal
   - Linear scan through all rules checking `regula->sinister == nt`
   - Prints rule index, LHS name, RHS length, and node type

2. **`conflicts`** - Find shift/reduce and reduce/reduce conflicts in the parse tables
   - Iterates all states
   - For each state, tracks which tokens have actions
   - Reports when multiple actions exist for the same token
   - Found 2 conflicts in state 4: ASTERISCUS (shift/reduce) and COLON (shift/reduce)

**Files changed**:
- `tools/glr_quaestio.c`: Added `cmd_rules()`, `cmd_conflicts()`, `ConflictusContextus` struct, `cb_detectere_conflictum()` callback. Updated help text and principale() dispatch. Fixed NT extraction source.

## 2026-01-12: Full Source Introspection Refactor

Made the tool completely independent of the GLR library by parsing all tables directly from source.

**Problem**: The tool was querying the library via callback functions (e.g., `arbor2_tabula_iterare_actiones_status`, `arbor2_tabula_obtinere_regula`). This created tight coupling and meant any library changes could break the query tool.

**Solution**: Parse all three table arrays directly from `lib/arbor2_glr_tabula.c` source:

1. **GOTO_TABULA parsing** (`parsere_goto_tabula()`):
   - Two-pass algorithm: count entries, allocate, parse
   - Format: `{ <state>, INT_NT_<name>, <new_state> }`
   - Resolves INT_NT_ names to values using pre-extracted table

2. **ACTIONES parsing** (`parsere_actiones_tabula()`):
   - Tracks current state via `/* State N: */` comments
   - Format: `{ ARBOR2_LEXEMA_<name>, ARBOR2_ACTIO_<type>, <value> }`
   - Maps action names (SHIFT/REDUCE/ACCEPT/ERROR) to numeric values

3. **REGULAE parsing** (`parsere_regulae_tabula()`):
   - Format: `{ ARBOR2_NT_<name>, <length>, ARBOR2_NODUS_<type> }`
   - Only extracts sinister (LHS) and longitudo (RHS length)

**Commands updated**:
- `cmd_state()` - iterates g_actio_tabula
- `cmd_token()` - iterates g_actio_tabula
- `cmd_goto()` - iterates g_goto_tabula
- `cmd_rule()` - indexes g_regula_tabula
- `cmd_rules()` - iterates g_regula_tabula
- `cmd_conflicts()` - iterates g_actio_tabula (rewritten without callback)
- `cmd_stats()` - uses parsed counts

**Cleanup**:
- Removed unused callback functions (`cb_imprimere_actio`, `cb_imprimere_status_actio`, `cb_imprimere_goto`, `cb_detectere_conflictum`)
- Removed `ConflictusContextus` struct
- Removed `#include "arbor2_glr.h"` and `#include "arbor2_lexema.h"`

**Result**: Tool now only needs arbor1 (for enum extraction) plus basic utilities. No GLR library functions are called at runtime. All queries are against parsed data structures.
