# lapifex_c89_grammatica.c Worklog

## 2026-02-09 — Initial Implementation (M2.3)

### Overview
C89 expression grammar for the Lapifex GLR parser. Contains:
- STML grammar string with 94 productions covering all C89 expression precedence levels
- Reduction callback (switch on production index) that builds Arbor2Nodus* AST nodes
- `lapifex_c89_expressionem_parsare()` pipeline: lex -> adapt -> GLR parse -> AST
- Static table caching for performance

### Grammar Structure
Productions P0-P93 (+ augmented P94 added automatically):
- P0-P1: comma (virga)
- P2-P13: assignment operators (=, +=, -=, *=, /=, %=, &=, |=, ^=, <<=, >>=)
- P14-P15: ternary (?:)
- P16-P43: binary operators by precedence (||, &&, |, ^, &, ==, !=, <, >, <=, >=, <<, >>, +, -, *, /, %)
- P44-P55: unary/cast/sizeof
- P56-P63: postfix (call, subscript, member, increment)
- P64-P69: primary (identifiers, literals, parenthesized)
- P70-P71: function call arguments
- P72-P93: type names for casts and sizeof (keyword types only, no typedef ambiguity)

### Key Design Decisions
1. **Minimal type_name**: Only keyword types (void, char, int, etc.) and struct/union/enum tags. No IDENTIFICATOR in type_name to avoid cast ambiguity with parenthesized expressions. Typedef resolution deferred to M2.6.

2. **Table caching**: Grammar/LALR table built once into static variables on first call. Subsequent parses skip the expensive ~2s table construction.

3. **Uses LALR(1)** instead of canonical LR(1) due to state explosion with the full grammar. No LALR conflicts found — the expression grammar is unambiguous with the precedence hierarchy encoded directly in the grammar.

### Terminal Names
All terminal names in the STML must match NOMINA_GENERUM[] in arbor2_lexema.c exactly. Unused C89 keywords (AUTO, BREAK, CASE, etc.) are omitted from the grammar since they don't appear in expressions.

### Production Numbering
Productions are numbered 0-based in the order they appear in the STML `<regulae>`. The augmented production is added as P94 by `lapifex_grammaticam_legere`. The reduction callback uses a switch on production_index with roman numeral case labels (ZEPHYRUM through XCIII).

## 2026-02-09 — Declaration Grammar (M2.4)

### Overview
Extended C89 grammar from 94 to 162 productions to parse declarations. Start symbol changed from `virga` to `summum` which accepts both expressions and declarations. Augmented production is now P162.

### New Productions (P94–P161)
- P94–P95: `summum` (start) → declaratio | virga
- P96–P97: `declaratio` → decl_specifiers + init_declarator_lista + SEMICOLON (or just specifiers + SEMICOLON for forward decl)
- P98–P117: `decl_specifiers` chain — storage classes (AUTO/REGISTER/STATIC/EXTERN/TYPEDEF), qualifiers (CONST/VOLATILE), type specifiers (9 keyword types), struct_specifier, enum_specifier
- P118–P121: `init_declarator_lista` — comma-separated declarators with optional `= initializer`
- P122–P130: `declarator` / `directus_declarator` — identifiers, grouped `(*fp)`, arrays `[]`/`[N]`, function params `(void)`/`(int a, char b)`
- P131–P144: `struct_specifier` / struct members — named/anonymous/forward, bit fields
- P145–P151: `enum_specifier` / enumerators — named/anonymous/forward, optional `= value`
- P152–P156: `parameter_lista` / `parameter_decl` — named/abstract params, variadic `...`
- P157–P161: `initializer` / `init_items` — expression init, brace-enclosed `{1, 2, 3}`, trailing comma

### New Terminals Added to STML
SEMICOLON, BRACE_APERTA, BRACE_CLAUSA, AUTO, REGISTER, STATIC, EXTERN, TYPEDEF, ELLIPSIS

### Helper Functions (17 new)
`_pointer_chain_creare`, `_nodus_declarator`, `_nodus_declaratio`, `_nodus_declaratio_vacua`, `_nodus_init_declarator`, `_nodus_specifier_chain`, `_nodus_array_dim`, `_nodus_declarator_functi`, `_nodus_struct_specifier`, `_nodus_enum_specifier`, `_nodus_enumerator`, `_nodus_parameter_decl`, `_nodus_initializor_lista`, `_nodus_struct_member_append`, `_nodus_struct_member`, `_nodus_bit_field`, `_nodus_anon_bit_field`

### Pointer Reduction Enhancement
P90–P93 changed from simple passthrough to building Xar pointer chains via `_pointer_chain_creare()`. Backward-compatible with expression grammar (cast/sizeof path ignores pointer values).

### Key Design Decisions
1. **Combined grammar**: Expressions and declarations share one STML grammar/LALR table. `summum → declaratio | virga` dispatches at top level.
2. **`initializer → assignatio`** (not `virga`): Prevents comma ambiguity in `int a = 1, b = 2;` — comma separates declarators, not expression-level comma operator.
3. **No LALR conflicts**: 162 productions, LALR(1), zero conflicts. Declaration paths are always disambiguated by keyword type specifiers at entry.
4. **Arena doubled**: `MMMMXCVI * XXXII` (128KB) to accommodate larger grammar table.
5. **Typedef deferred to M2.6**: Only keyword types can start declarations.

### Test Results
- 30 declaration tests, 112 assertions — all pass
- 25 expression tests, 120 assertions — all pass (no regression)
- Table build time: ~3s (up from ~2s with expression-only grammar)
