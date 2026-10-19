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

---

## 2026-01-07: Function Parameter Type Resolution - IMPLEMENTED

### Problem

Function types were created with empty `parametra` arrays. Parameter types weren't resolved, and parameters weren't accessible by name in function bodies.

### Implementation

Two parts to the fix:

**1. Parameter Type Resolution in `_applicare_declarator`**

When processing a FUNCTION_DECLARATOR, iterate through its `datum.genericum.liberi` children. For each PARAMETER_DECLARATION:
- Resolve type from `datum.parametrum.specifiers` via `_resolvere_specifiers`
- Apply declarator modifiers via `_applicare_declarator` (for pointers, arrays)
- Add resolved type to function's `parametra` Xar
- Set `typus_resolutum` on the parameter node

**2. Add Parameters to Scope in `_resolvere_functio`**

After opening function scope, navigate to the FUNCTION_DECLARATOR within the declarator, then for each PARAMETER_DECLARATION:
- Extract parameter name via `_extrahere_declarator_nomen`
- Register in function scope via `_registrare_symbolum`

### Test Coverage

Added two new tests (29 new assertions):
- `probatio_typus_functio_parametra`: Verifies 3 parameter types resolve correctly (int, char, long*)
- `probatio_typus_param_in_scope`: Verifies parameter `x` is accessible in function body with correct type

### Key Pattern

For FUNCTION_DECLARATOR children, check `genus == ARBOR_NODUS_PARAMETER_DECLARATION` before accessing `datum.parametrum`. The children may include other node types.

### Remaining TODOs

- Typedef name usage in type lookups

---

## 2026-01-07: Struct Member Type Resolution - IMPLEMENTED

### Problem

Struct types were created without member information. The `membra` field was always NULL, preventing queries like "what type is field x of struct S".

### Implementation

In `_resolvere_specifiers` for STRUCT_SPECIFIER case:

1. Create `membra` Xar to hold ArborMembrum*
2. Iterate through `spec->datum.aggregatum.membra` (Xar of DECLARATION nodes)
3. For each member declaration:
   - Resolve base type from specifiers via `_resolvere_specifiers`
   - For each init_declarator in declaratores:
     - Apply declarator modifiers via `_applicare_declarator`
     - Extract member name via `_extrahere_declarator_nomen`
     - Create ArborMembrum with name, type, and bitfield width
     - Add to `typus->membra`

### Bitfield Support

Struct member declarations repurpose `init_decl.initializer` for bitfield width. When present and is INTEGER_LITERAL, extract the value for `membrum->bitfield_width`.

### Header Fix

Changed `ArborMembrum.bitfield_width` from `i32` (unsigned) to `s32` (signed) to properly support -1 sentinel value for non-bitfields (matching `array_mensura` pattern).

### Test Coverage

Added `probatio_typus_struct_membra` test (23 new assertions):
- Verifies struct with 3 members: `int x`, `char *name`, `long arr[10]`
- Checks member count, names, and resolved types including pointer and array

### Forward Declaration

Added `_extrahere_declarator_nomen` to internal prototypes since it's now called from `_resolvere_specifiers` (before its definition).

### Remaining TODOs

- Typedef name lookups for user-defined types in member declarations

---

## 2026-01-07: Union Member Resolution - IMPLEMENTED

Added same member resolution logic for UNION_SPECIFIER case. Identical pattern to struct - iterate through `datum.aggregatum.membra`, resolve types, create ArborMembrum entries.

Test: `probatio_typus_union_membra` with `union Value { int i; float f; char *s; }` - 16 new assertions.

---

## 2026-01-07: Typedef Name Lookup in Type Resolution - IMPLEMENTED

### Problem

When a typedef name was used in a declaration (e.g., `typedef int MyInt; MyInt x;`), the type resolver couldn't look up the typedef to resolve `x`'s type.

### Root Cause

The TYPEDEF_NAME case in `_resolvere_specifiers` was using `_quaerere_symbolum`, which searches `scopes` and `globalia`. But typedefs are stored in `typedef_nomina` - a separate hash table specifically for typedef names.

### Fix

Changed TYPEDEF_NAME case to use `tabula_dispersa_invenire(res->typedef_nomina, ...)`:

```c
casus ARBOR_NODUS_TYPEDEF_NAME:
    {
        vacuum* valor_ptr;
        chorda* typedef_nomen;

        typedef_nomen = spec->datum.folium.valor;
        si (typedef_nomen != NIHIL &&
            tabula_dispersa_invenire(res->typedef_nomina, *typedef_nomen, &valor_ptr))
        {
            ArborTypus* typedef_basis;
            typedef_basis = (ArborTypus*)valor_ptr;
            typus = _creare_typus(res, ARBOR_TYPUS_TYPEDEF);
            si (typus != NIHIL)
            {
                typus->titulus = typedef_nomen;
                typus->basis = typedef_basis;
                typus->est_const = est_const;
                typus->est_volatile = est_volatile;
            }
            redde typus;
        }
    }
    frange;
```

### Test Coverage

Added `probatio_typus_typedef_usage` test (5 assertions):
- Parses `typedef int MyInt; MyInt x;`
- Verifies `x` has type ARBOR_TYPUS_TYPEDEF
- Verifies typedef's basis is ARBOR_TYPUS_INT

### Key Insight

The type resolver maintains three separate namespaces:
- `typedef_nomina`: typedef names → ArborTypus*
- `tag_nomina`: struct/union/enum tags → ArborTypus*
- `scopes`/`globalia`: variable/function names → ArborSymbolum*

TYPEDEF_NAME nodes must look up in `typedef_nomina`, not in the symbol tables.
