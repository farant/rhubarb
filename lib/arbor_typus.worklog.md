# arbor_typus.c worklog

## 2026-01-07: Union Variant Access Bug - FIXED

### Problem

Segfault in `_resolvere_liberos` when processing function bodies containing return statements.

Crash address: `0x800000001` (corrupted pointer).

### Root Cause

`_resolvere_liberos` blindly reads `nodus->datum.genericum.liberi` for ALL node types, but `datum` is a **union** - different node types use different variants:
- TRANSLATION_UNIT uses `genericum.liberi`
- RETURN_STATEMENT uses `reditio.valor` (ArborNodus*, not Xar*)
- BINARY_EXPRESSION uses `binarium.sinister`, `binarium.dexter`
- etc.

When reading the wrong union variant, garbage memory is accessed.

### Fix Applied

1. Modified `_resolvere_liberos` to check node type before accessing `genericum.liberi`
2. Added explicit cases in `_resolvere_nodus` for all statement/expression types:
   - RETURN_STATEMENT → reditio.valor
   - IF_STATEMENT → conditionale.*
   - WHILE/DO_STATEMENT → iteratio.*
   - FOR_STATEMENT → circuitus.*
   - SWITCH_STATEMENT → selectio.*
   - BINARY_EXPRESSION → binarium.*
   - UNARY_EXPRESSION → unarium.operandum
   - CONDITIONAL_EXPRESSION → ternarium.*
   - CALL_EXPRESSION → vocatio.*
   - SUBSCRIPT_EXPRESSION → subscriptum.*
   - MEMBER_EXPRESSION → membrum.objectum
   - CAST/SIZEOF_EXPRESSION → their specific fields

3. Added terminal node cases (INTEGER_LITERAL, etc.) that do nothing

### Same Bug Found in arbor_index.c

The identical bug existed in `_extrahere_symbola` and `_colligere_referentias` in arbor_index.c - fixed with the same pattern.

### Key Rule

**Never access `datum.genericum.liberi` without first checking the node type.** Only these node types use `genericum.liberi`:
- TRANSLATION_UNIT
- DECLARATOR
- INIT_DECLARATOR
- PARAMETER_LIST
- EXPRESSION_STATEMENT
- FUNCTION_DECLARATOR

All other node types use type-specific union variants.
