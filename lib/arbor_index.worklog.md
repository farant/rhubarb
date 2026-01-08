# arbor_index.c worklog

## 2026-01-07: Union Variant Access Bug - FIXED

### Problem

Segfault in `_extrahere_symbola` and `_colligere_referentias` when processing AST nodes.

Crash address: `0x800000001` (corrupted pointer).

### Root Cause

Same issue as arbor_typus.c - functions blindly read `nodus->datum.genericum.liberi` for ALL node types, but `datum` is a **union** - different node types use different variants:

- TRANSLATION_UNIT uses `genericum.liberi`
- RETURN_STATEMENT uses `reditio.valor` (ArborNodus*, not Xar*)
- BINARY_EXPRESSION uses `binarium.sinister`, `binarium.dexter`
- etc.

When reading the wrong union variant, garbage memory is accessed.

### Fix Applied

1. Modified `_extrahere_symbola` to check node type before accessing `genericum.liberi`
2. Modified `_colligere_referentias` - changed `ordinarius` case to not blindly access `genericum.liberi`
3. Modified `_invenire_nodum_ad_punctum` - same pattern, set `liberi = NIHIL` for unknown types

### Key Rule

**Never access `datum.genericum.liberi` without first checking the node type.** Only these node types use `genericum.liberi`:

- TRANSLATION_UNIT
- DECLARATOR
- INIT_DECLARATOR
- PARAMETER_LIST
- EXPRESSION_STATEMENT
- FUNCTION_DECLARATOR

All other node types use type-specific union variants.

### See Also

Same bug and fix documented in `arbor_typus.worklog.md`.
