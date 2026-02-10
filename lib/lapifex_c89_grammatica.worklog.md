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

## 2026-02-09 — Statement Grammar (M2.5)

### Overview
Extended C89 grammar from 162 to 203 productions to parse statements. Start symbol `summum` now accepts expressions, declarations, and statements. Augmented production is now P203.

### New Productions (P96, P163–P202)
- P96: `summum → sententia` (3rd alternative, shifted all M2.4 declarations by +1)
- P163–P164: `sententia → sententia_compar | sententia_incompar`
- P165–P171: `sententia_compar` (matched statements: non_si, if-else balanced, while, for, label, case, default)
- P172–P178: `sententia_incompar` (unmatched: dangling-else variants)
- P179–P188: `non_si_sententia` (corpus, expr;, ;, do-while, switch, goto, continue, break, return;, return expr;)
- P189–P190: `corpus` (empty block, block with body)
- P191–P192: `elenchus_corporis` (single element, append)
- P193–P194: `elementum_corporis` (declaration or statement)
- P195–P202: `per_clausula` (8 for-loop variants: all combinations of optional init/cond/incr)

### New Terminals Added to STML
IF, ELSE, WHILE, DO, FOR, SWITCH, CASE, DEFAULT, BREAK, CONTINUE, RETURN, GOTO

### Helper Functions (16+ new)
`_nodus_si`, `_nodus_dum`, `_nodus_fac_dum`, `_nodus_per`, `_nodus_commutatio`, `_nodus_redde`, `_nodus_salta`, `_nodus_frange`, `_nodus_perge`, `_nodus_sententia_expr`, `_nodus_sententia_vacua`, `_nodus_corpus`, `_nodus_corpus_append`, `_nodus_titulatum`, `_nodus_casus`, `_nodus_ordinarius`, `_per_clausula_creare`

### Dangling-Else Resolution
Used classical matched/unmatched (compar/incompar) grammar transformation. Every statement position that could create dangling-else ambiguity is split into two non-terminals: `sententia_compar` (all if-branches balanced with else) and `sententia_incompar` (has a dangling else somewhere). Grammar is LR(1) unambiguous; LALR(1) generates no conflicts.

### Key Design Decisions
1. **Production renumbering**: Adding P96 `summum → sententia` shifted all existing M2.4 declaration productions by +1 (old P96→P97 through P161→P162). All case labels in the reduction callback were updated.
2. **elenchus_corporis accumulation**: Uses a wrapper Arbor2Nodus with genus marker `0xDEAD` to distinguish between a single AST node and a Xar* collection. `_nodus_corpus` extracts the Xar from the wrapper or wraps a single node into a new Xar. Fragile but works within the s64-value-only GLR reduction system.
3. **PerClausula struct**: Intermediate struct to pack for-loop clause components (init/cond/incr + semicolon tokens). Allocated from arena, cast to Arbor2Nodus* to pass through the s64 value system, then unpacked in `_nodus_per`.
4. **C99-style interleaving**: `elementum_corporis → declaratio | sententia` allows declarations and statements to interleave freely. Semantic C89 ordering check can be added later.
5. **IDENTIFICATOR COLON conflict**: GLR engine forks on shift-reduce conflict (label vs expression start); only the correct parse survives.

### Bug Found & Fixed
`_nodus_corpus` and `_nodus_corpus_append` had a bug where the 0xDEAD wrapper's Xar* was extracted by blind pointer cast instead of accessing `wrapper->datum.corpus.sententiae`. Fixed to correctly handle both single-node (wrap in new Xar) and wrapper-node (extract existing Xar) cases.

### New API
`lapifex_c89_sententiam_parsare()` — parses a C89 statement from source text, returns AST root.

### Test Results
- 30 statement tests, 148 assertions — all pass
- 30 declaration tests, 112 assertions — all pass (no regression)
- 25 expression tests, 120 assertions — all pass (no regression)
- All 10 lapifex tests pass
- Table build time: ~7s (up from ~3s with expression+declaration grammar)

## 2026-02-09 — Function Definitions, Translation Unit, Naive Typedef (M2.6)

### Overview
Extended C89 grammar from 203 to 207 productions. Added function definitions, translation unit support (loop-based, not grammar-driven), and naive typedef pre-scan mechanism. Enables parsing complete C89 source files.

### New Token
`ARBOR2_LEXEMA_NOMEN_TYPUS` — typedef name token (IDENTIFICATOR remapped to this by pre-scan when it matches a known typedef). Added to `arbor2_lexema.h` enum, `arbor2_lexema.c` NOMINA_GENERUM[], and `ARBOR2_LAPIFEX_MAX_GENERA` bumped from 94 to 95.

### New Productions (P203–P206)
- P203: `declaratio → decl_specifiers declarator corpus` (function definition)
- P204: `decl_specifier → NOMEN_TYPUS` (typedef name as type specifier)
- P205: `specifier_singulum → NOMEN_TYPUS` (for casts: `(my_type)x`)
- P206: `directus_declarator → NOMEN_TYPUS` (for definition sites in typedef decls)

### Translation Unit Architecture
**Originally attempted grammar-driven** (summum → translatio → declaratio_externa*) but hit two fatal issues:
1. **GLR phantom forks**: `summum → translatio` caused invalid reductions from other forks accessing wrong union members → segfault
2. **LALR ambiguity**: After `summum → declaratio` matched `int x;`, parser expected EOF but multi-item input continued

**Solution: loop-based segment parsing** at the token level in `lapifex_c89_translationem_parsare()`:
1. Lex entire input → single token stream
2. Pre-scan for typedefs (remaps IDENTIFICATOR → NOMEN_TYPUS in-place)
3. Scan token stream for segment boundaries
4. For each segment, create sub-Xar + EOF, run adaptor + GLR
5. Collect results into TRANSLATION_UNIT node (or return single item directly)

### Token-Level Segment Finder
`_segmentum_finem_in_lexematibus()` — scans non-trivia tokens tracking brace depth:
- `;` at depth 0 → end of declaration
- `}` at depth 0 → end of segment ONLY if `{` was preceded by `)` (function definition). If preceded by anything else (struct tag, keyword), it's a struct/enum body and scanning continues to find the `;`.

This heuristic correctly handles:
- `void f(void) { ... }` — `)` before `{` → function def, split at `}`
- `typedef struct Foo { ... } Foo_t;` — `Foo` before `{` → struct body, scan to `;`
- `int arr[] = {1, 2, 3};` — `=` before `{` → initializer, scan to `;`

### Naive Typedef Pre-Scan
`lapifex_c89_typedef_praescandere()` — forward scan through token stream:
1. Accept optional external typedef names (e.g., `size_t` from `<stddef.h>`)
2. When encountering `TYPEDEF` keyword, scan forward to `;` **at brace depth 0** (critical for struct bodies), extract last IDENTIFICATOR at depth 0 before `;` as new typedef name
3. Remap any known-typedef IDENTIFICATORs within the typedef decl (but not the name being defined)
4. For all other positions, remap IDENTIFICATOR → NOMEN_TYPUS if the name is known

**Bug fixed**: Initial implementation didn't track brace depth while scanning for `;`, causing `typedef struct Foo { int x; } Foo_t;` to find the `;` inside the struct body and extract `x` as the typedef name instead of `Foo_t`.

### New API
- `lapifex_c89_translationem_parsare()` — parses multiple declarations/definitions from source
- `lapifex_c89_typedef_praescandere()` — pre-scans tokens for typedef resolution

### Helper Function
`_nodus_definitio_functi()` — creates `ARBOR2_NODUS_DEFINITIO_FUNCTI` AST node with specifier, declarator, corpus.

Removed `_nodus_translatio` and `_nodus_translatio_append` (no longer needed with loop-based approach).

### Test Results
- 20 function/translation/typedef tests — all pass
- 30 statement tests, 148 assertions — all pass (no regression)
- 30 declaration tests, 112 assertions — all pass (no regression)
- 25 expression tests, 120 assertions — all pass (no regression)
- All 11 lapifex tests pass
