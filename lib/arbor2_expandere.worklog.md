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
