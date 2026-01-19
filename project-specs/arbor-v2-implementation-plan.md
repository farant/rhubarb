# Arbor v2 Implementation Plan

Date: 2026-01-19
Status: Updated after Phase 5.4 completion - Conditional Compilation COMPLETE

---

## Current State (Updated 2026-01-17)

### arbor2_lexema.c - COMPLETE
- 98 token types (all C89 keywords, operators, literals, whitespace)
- NOVA_LINEA as explicit token (key difference from v1)
- Explicit whitespace tokens (Phase 2.6):
  - SPATIA (contiguous spaces), TABULAE (contiguous tabs)
  - CONTINUATIO (line continuation `\\\n`)
  - COMMENTUM_CLAUSUM (block `/* */`), COMMENTUM_LINEA (line `//`)
- Whitespace/comments attached via `spatia_ante`/`spatia_post` (Xar of Arbor2Lexema*)
- Standard tracking (ARBOR2_STANDARD_C89/C99/EXTENSION)

### arbor2_token.c - COMPLETE
- Full provenance chain implementation
- Token origins: FONS (source), EXPANSIO (macro), PASTA (##), CHORDA (#), API
- Chain walking: arbor2_token_radix(), arbor2_token_profunditas()
- Merged provenance for token paste

### arbor2_expandere.c (~1892 lines) - MOSTLY COMPLETE
- Object-like and function-like macro expansion
- Parameter substitution
- Stringification (#) - C89 compliant
- Token pasting (##) with re-lexing
- Macro recursion prevention
- Segment-based #define/#undef tracking
- Typedef detection (keyword + "nomen" heuristic)
- Layered expansion (expand to fixpoint, store all layers)
- Layer and segment query APIs
- **Lookahead API** for macro type disambiguation (Phase 3.2)
- **Built-in latina.h macros** via `arbor2_includere_latina()` (Phase 7.1)

**Missing in expandere:**
- #include processing (files not actually read)
- #error, #line, #pragma handling

### arbor2_conditio_evaluare.c (~800 lines) - COMPLETE
- Full C89 preprocessor expression evaluator for #if/#elif
- Recursive descent parser with proper operator precedence
- Supports: integers (decimal/octal/hex), arithmetic, comparison, logical, bitwise, ternary
- `defined(MACRO)` and `defined MACRO` operators
- Macro expansion within expressions
- Character literal support
- API: `arbor2_conditio_evaluare()`, `arbor2_conditio_est_definitum()`

### arbor2_glr.c + arbor2_glr_tabula.c - SUBSTANTIAL
- GLR parser core with GSS
- GSS path enumeration for multi-path reductions
- Piscina checkpointing for rollback
- **346 grammar productions** (P0-P345)
- **952 LR states** (0-951)
- **~1900 tests total** (all passing)
- Table validation
- **Location propagation** via LOCUS_EX_LEXEMATIS macro (Phase 2.1)
- **Macro lookahead integration** via public API (Phase 3.2)
- **AMBIGUUS identifier tracking** (Phase 3.3)
- **Error recovery** with ERROR nodes and skipped token tracking (Phase 4)
- **Conditional compilation** detection and collection (Phase 5/5b):
  - `_est_conditionale_directivum()` - detects #ifdef/#ifndef/#if/#elif/#else/#endif
  - `_colligere_conditionale()` - collects entire conditional block into AST
  - `ARBOR2_NODUS_CONDITIONALIS` with `Arbor2CondRamus` branches
  - Nested conditional support with depth tracking
  - Expression evaluation for #if/#elif via arbor2_conditio_evaluare()

**Grammar currently covered:**

| Category | Status | Details |
|----------|--------|---------|
| Binary ops | COMPLETE | +, -, *, /, %, &, \|, ^, <<, >>, &&, \|\|, ==, !=, <, >, <=, >= |
| Unary ops | COMPLETE | *, &, -, ~, !, ++, -- (pre and post) |
| Postfix | COMPLETE | [], (), ., ->, ++, -- |
| Ternary | COMPLETE | ? : |
| Assignment | COMPLETE | =, +=, -=, *=, /=, %=, &=, \|=, ^=, <<=, >>= |
| Comma | COMPLETE | , (expression comma operator) |
| Literals | COMPLETE | int, float, char, string |
| sizeof | COMPLETE | sizeof expr, sizeof(type), sizeof(type*), sizeof(type[N]), sizeof(type[N][M]), sizeof(type*[N]), sizeof(struct X), sizeof(unsigned int), sizeof(long int) |
| Casts | COMPLETE | All types including (int*), (char**), (struct X*), (TypeName*) |
| if/else | COMPLETE | |
| while | COMPLETE | |
| do-while | COMPLETE | |
| for | COMPLETE | |
| break/continue | COMPLETE | |
| return | COMPLETE | |
| goto/labels | COMPLETE | |
| switch/case/default | COMPLETE | |
| Compound stmt | COMPLETE | { stmts } |
| Declarations | COMPLETE | type declarator, with initializers |
| Initializers | COMPLETE | Simple (=expr), brace ({1,2,3}), designated ({.x=1, [5]=100}) |
| Pointer declarators | COMPLETE | * name |
| Function declarators | COMPLETE | name(), name(void), name(params) |
| Function definitions | COMPLETE | type declarator { body } |
| Parameters | COMPLETE | With const/volatile qualifiers |
| Variadic functions | COMPLETE | `int printf(char *fmt, ...)` (P341, state 906) |
| Pointer qualifiers | COMPLETE | `int * const p`, `int * volatile p` (P252-P255, states 657-661) |
| Storage classes | COMPLETE | Individual and combinations (`static const int x`) |
| Type qualifiers | COMPLETE | Individual and combinations |
| Multiple declarators | COMPLETE | `int x, y;`, `int *x, y;` (AMBIGUUS for pointer case) |
| Type modifiers | COMPLETE | `unsigned`, `long`, `short`, `signed` + combinations (states 617-624) |
| Qualifier + type modifier | COMPLETE | `const unsigned int`, `volatile long int`, compound combos (states 920-951) |
| struct/union | COMPLETE | With members, bit fields |
| enum | COMPLETE | With enumerators and values |
| Arrays | COMPLETE | name[size] |
| Function pointers | COMPLETE | `int (*fp)(void)` (P339, states 901-904) |
| Translation unit | COMPLETE | Multiple external declarations via wrapper API |

**Grammar NOT covered (remaining gaps):**

| Missing | Impact | Phase |
|---------|--------|-------|
| K&R function defs | Old-style parameters | Low priority |

**Note:** All specifier combinations now work:
- `static unsigned int x` ✓
- `const unsigned int x` ✓
- `volatile long int y` ✓
- `static const unsigned int z` ✓
- `extern volatile long int e` ✓

### Arbor2Nodus Structure - ENHANCED (Phase 2.1, 2.3, 2.4)
Current structure has:
- genus (node type)
- lexema (associated token)
- **linea_initium, columna_initium** (start position) - Phase 2.1
- **linea_finis, columna_finis** (end position) - Phase 2.1
- **layer_index** (0 = source, >0 = macro-expanded) - Phase 2.1
- **pater** (parent pointer, NIHIL for root) - Phase 2.3
- **commenta_ante, commenta_post** (promoted comment nodes) - Phase 2.4
- datum union (38+ node type variants including DESIGNATOR_ITEM, COMMENTUM)

**Whitespace/Trivia Model (Phase 2.6):**
- Whitespace lives on TOKENS via `spatia_ante`/`spatia_post` (Xar of Arbor2Lexema*)
- Each spatium is a typed token: SPATIA, TABULAE, CONTINUATIO, COMMENTUM_CLAUSUM, COMMENTUM_LINEA
- Comments also promoted to AST nodes for querying (dual storage)

**Missing for tooling:**
- Byte offsets (byte_initium/finis) - line/column added, bytes not yet
- Type resolution field

### Tests - COMPREHENSIVE
- **~2069 GLR tests** (all passing) - updated after Phase 5.4
- **124 expandere tests** (all passing) - includes lookahead API
- Covers expressions, statements, declarations
- Full initializer coverage (simple, brace, designated)
- sizeof variants (type, pointer, array, multi-dim, pointer array)
- Translation unit tests (multiple decls, functions, mixed)
- Variadic function tests
- Qualified parameter tests (const/volatile in function params)
- Function pointer tests
- Qualifier + type modifier tests (Phase 1.4b)
- **Location propagation tests** (identifier, binary expr, literals, function call) - Phase 2.1
- **Parent pointer tests** (binary expr, nested expr, ternary, function call, subscript) - Phase 2.3
- **Latin macro disambiguation tests** (integer x, i32 x, constans integer x) - Phase 3.2
- **AMBIGUUS tracking tests** (foo * bar) - Phase 3.3
- **Error recovery tests** (invalid decl + valid, multiple errors, error + function) - Phase 4
- **Conditional compilation tests** (Phase 5/5b/5.4):
  - Basic #ifdef/#endif, #ifdef/#else/#endif, #ifndef
  - Multiple independent conditionals, mixed with declarations
  - Nested conditionals
  - #if with expressions: literals, arithmetic, comparison, logical, defined()
  - #elif branches
  - Branch parsing: basic, multiple decls, nested, empty, ifdef/else
  - 19+ test cases for conditional directives
- Table validation
- Parser statistics

---

## Phase 1: Complete Grammar - Status

### 1.1 Abstract Declarators - COMPLETE ✓

Implemented 2026-01-14 through 2026-01-15 in multiple sub-phases.

**1.1a - Basic sizeof(type) and Pointer Casts (2026-01-14):**
- `sizeof(int)`, `sizeof(char)`, `sizeof(void)` - P162-P165
- `sizeof(int*)`, `sizeof(char*)` - P166-P169
- `sizeof(int**)`, `sizeof(char**)` - P170-P173
- `sizeof(struct X)`, `sizeof(union Y)`, `sizeof(enum Z)` - P183-P191
- Pointer casts: `(int*)x`, `(int**)x`, `(struct X*)p` - P154-P182

**1.1b - sizeof(type[N]) Single Dimension Arrays:**
- `sizeof(int[10])`, `sizeof(char[256])` - P240-P243
- States 563-578 (16 new states)

**1.1c - sizeof(type[N][M]) Multi-Dimensional Arrays:**
- `sizeof(int[10][20])`, `sizeof(char[256][4])` - P244-P247
- States 579-594 (16 new states)

**1.1d - sizeof(type*[N]) Pointer Arrays (2026-01-15):**
- `sizeof(int*[10])`, `sizeof(char*[256])` - P248-P251
- States 595-610 (16 new states)
- Note: ID variant (`sizeof(MyType*[N])`) not supported (ID path disabled for ambiguity)

160+ tests covering all sizeof variants.

### 1.2 Declaration Initializers - COMPLETE ✓

Implemented 2026-01-14 in three sub-phases:

**1.2a - Simple Initializers:**
- `int x = 5` - P192
- `static int x = 5` - P193
- All storage class variants - P193-P198

**1.2b - Brace Initializers:**
- `int a[] = {1, 2, 3}` - P199-P206
- Empty braces `{}` - P200
- Trailing comma `{1, 2,}` - P202
- Nested braces `{{1,2}, {3,4}}` - P206
- Storage class variants - P207-P212

**1.2c - Designated Initializers:**
- Field designators `.x = 1` - P214
- Array designators `[5] = 100` - P213
- Chained designators `[0].x`, `.a.b`, `[0][1]` - P215-P216
- Mixed positional + designated `{1, .b = 2, 3}` - P217-P219
- Nested brace values `{.a = {1, 2}}` - P220

35+ new tests for initializers.

### 1.3 Multiple Declarators - COMPLETE ✓

**Working:**
- `int x, y` ✓ (multiple simple declarators)
- `int x = 1, y = 2` ✓ (multiple declarators with initializers)
- `int *x, y` ✓ (pointer declarator with multiple) - returns AMBIGUUS

**Fix (2026-01-15):** Added COMMA handling to States 18 and 19 so the declaration path survives. `int *x, y` is genuinely ambiguous (could be declaration or multiplication+comma expression), so AMBIGUUS is the correct result.

### 1.4 Type Modifiers - COMPLETE ✓

Implemented 2026-01-15.

**Working:**
- `unsigned int x` ✓
- `long int y` ✓
- `short int s` ✓
- `signed int x` ✓
- `unsigned long z` ✓ (implicit int)
- `unsigned short u` ✓ (implicit int)
- `signed long w` ✓
- `signed short ss` ✓
- `signed char c` ✓
- `unsigned char uc` ✓

**Implementation:**
- Added UNSIGNED, LONG, SHORT, SIGNED to State 0 actions
- Created states 617-624 for type modifier chains:
  - 617: after 'unsigned' → accepts INT, CHAR, LONG, SHORT, ID
  - 618: after 'unsigned long' → accepts INT, ID
  - 619: after 'unsigned short' → accepts INT, ID
  - 620: after 'long' → accepts INT, ID
  - 621: after 'short' → accepts INT, ID
  - 622: after 'signed' → accepts INT, CHAR, LONG, SHORT, ID
  - 623: after 'signed long' → accepts INT, ID
  - 624: after 'signed short' → accepts INT, ID
- Added GOTO tables with DECLARATIO, DECLARATOR, INIT_DECLARATOR, INIT_DECLARATOR_LIST
- 16 new tests

**Storage class + qualifier combinations (from before):**
- `static int x` ✓
- `const int x` ✓
- `volatile int y` ✓
- `static const int x` ✓
- `extern volatile int y` ✓
- `static const volatile int x` ✓

**Note:** Storage class + type modifier completed in earlier work.

### 1.4b Qualifier + Type Modifier Combinations - COMPLETE ✓

Implemented 2026-01-15.

**Problem:** States 350 (after `const`) and 351 (after `volatile`) did not accept type modifiers (UNSIGNED, LONG, SHORT, SIGNED). `const unsigned int x` failed.

**Solution:**
- Added type modifier actions to State 350 (const): UNSIGNED→920, LONG→923, SHORT→924, SIGNED→925
- Added type modifier actions to State 351 (volatile): UNSIGNED→928, LONG→931, SHORT→932, SIGNED→933
- Created States 920-935 for qualifier + type modifier chains (16 states)
- Added GOTO entries for States 350/351 (needed for DECLARATIO after reduction)
- Added type modifier actions to compound states 530 (static const) and 535 (extern volatile)
- Created States 936-951 for compound + type modifier chains (16 states)

**Working:**
- `const unsigned int x` ✓
- `const long int y` ✓
- `const short int z` ✓
- `volatile unsigned int a` ✓
- `volatile long int b` ✓
- `const signed char c` ✓
- `static const unsigned int d` ✓
- `extern volatile long int e` ✓

**Stats:** +33 states (919→952), 8 tests now passing

### 1.5 Translation Unit - COMPLETE ✓

Implemented 2026-01-15 using wrapper approach (no grammar changes).

**Design Decision:** Chose wrapper approach over grammar-based approach:
- Grammar approach would require ~4 productions, ~20 states, complex accept-state modifications
- Wrapper approach: scan for item boundaries, parse each chunk with synthetic EOF
- Lower risk, no changes to existing grammar/states

**Key Implementation Detail:** Declaration grammar does NOT include semicolon (handled at statement level). Boundary finder returns separate `parse_finis` (excludes semicolon) and `proximus` (skips semicolon) values.

**New API:**
```c
Arbor2GLRResultus arbor2_glr_parsere_translation_unit(
    Arbor2GLR*  glr,
    Xar*        lexemata);
```

**New Node Type:** `ARBOR2_NODUS_TRANSLATION_UNIT` with `datum.translation_unit.declarationes` (Xar of Arbor2Nodus*)

**New Result Field:** `tokens_consumed` in `Arbor2GLRResultus`

**Now Working:**
- `int a; int b;` ✓ (multiple declarations)
- `int x = 5; int y = 10;` ✓ (with initializers)
- `int foo() { return 1; }` ✓ (single function)
- `int x; int foo() { return x; }` ✓ (declaration + function)
- `int foo() {} int bar() {}` ✓ (multiple functions)
- Empty input ✓
- Functions with parameters ✓

32 new tests for translation unit support.

**Deliverable:** Can parse complete C89 files ✓

### 1.6 Function Pointers, Variadic Functions, Qualified Parameters - COMPLETE ✓

Implemented 2026-01-15.

**1.6a - Function Pointer Declarators:**
- `int (*fp)(void)` - grouped pointer declarator
- P339: grouped_declarator -> '(' '*' declarator ')'
- States 901-904 (4 new states)
- 16 new tests

**1.6b - Variadic Function Parameters:**
- `int printf(char *fmt, ...)` - variadic parameter lists
- P341: param_list -> param_list ',' '...'
- State 906 (1 new state)
- Added `est_variadicus` field to ARBOR2_NODUS_PARAMETER_LIST
- 2 new tests

**1.6c - Qualified Parameters (const/volatile in function params):**
- `int fn(const int x)` - const value parameter
- `int fn(const char *p)` - const type with pointer
- `int fn(volatile int v)` - volatile value parameter
- `int fn(int * const p)` - const pointer parameter
- P342-P345: qualified parameter declarations
- States 907-919 (13 new states)
- 4 new tests

**Implementation Details:**
- States 907/908 handle `( const` / `( volatile` after function name
- States 910/911 handle `, const` / `, volatile` for subsequent params
- States 912-919 route qualified types through DECLARATOR GOTO to reduce states
- P342/P343 pop 3 symbols (qualifier + type + declarator)
- P344/P345 pop 2 symbols (qualifier + type, abstract params)
- Reuse existing pointer qualifier states 657-661 for `int * const p`

---

## Recommended Next Steps

Given the current state, the recommended priority is:

1. **Phase 2 (Rich AST)** - **COMPLETE** ✓
   - 2.1 Location Propagation: **COMPLETE** ✓
   - 2.2 Trivia Attachment: **COMPLETE** ✓ (superseded by 2.6)
   - 2.3 Parent Pointer: **COMPLETE** ✓
   - 2.4 Comment Nodes: **COMPLETE** ✓ (promoted from trivia to first-class AST nodes)
   - 2.6 Tokenize Whitespace: **COMPLETE** ✓ (explicit SPATIA/TABULAE/CONTINUATIO/COMMENTUM tokens)

2. **Phase 3 (GLR-Expansion Integration)** - **MOSTLY COMPLETE** ✓
   - 3.1 Typedef Lookahead: **COMPLETE** ✓
   - 3.2 Macro Lookahead: **COMPLETE** ✓ (public API + parser integration)
   - 3.3 AMBIGUUS Tracking: **IMPROVED** ✓ (identificator field set)
   - 3.4 Fork on Unknown: **WORKING** ✓

3. **Phase 7.1 (Built-in Latina)** - **COMPLETE** ✓
   - `arbor2_includere_latina()` with 35 macros

4. **Phase 4 (Error Recovery)** - **COMPLETE** ✓
   - Statement-level recovery in translation unit parser
   - ERROR nodes with skipped token tracking
   - Error limit (10 max) to prevent infinite loops
   - 21 new test assertions

5. **Phase 5 (Conditional Compilation)** - **COMPLETE** ✓
   - 5.1 Directive Recognition: **COMPLETE** ✓
   - 5.2 Branch Structure: **COMPLETE** ✓ (Arbor2CondRamus, CONDITIONALIS node)
   - 5.3 Expression Evaluation: **COMPLETE** ✓ (arbor2_conditio_evaluare.c)
   - 5.4 Branch Parsing: **COMPLETE** ✓ (recursive AST parsing of branch contents)
   - 24+ test cases

6. **Next priorities:**
   - Phase 6: #include Processing (actually read files)
   - Phase 7.2: Standard Library Type Hints (FILE, size_t, etc.)
   - #error, #warning, #pragma, #line directives

**Current capabilities:**
- Complete C89 files with multiple declarations/functions ✓
- Storage class combinations (`static const int`) ✓
- Type modifiers (`unsigned int`, `long int`, implicit int) ✓
- Qualifier + type modifier (`const unsigned int`, `volatile long int`) ✓
- Compound combinations (`static const unsigned int`, `extern volatile long int`) ✓
- Multiple declarators (`int x, y`, `int *x, y` as AMBIGUUS) ✓
- All expression/statement/control flow constructs ✓
- Variadic functions (`int printf(char *fmt, ...)`) ✓
- Pointer qualifiers (`int * const p`, `int * volatile p`) ✓
- Qualified parameters (`int fn(const int x)`, `int fn(volatile int v)`) ✓
- Function pointers (`int (*fp)(void)`) ✓
- **Latin C89 macros** (`integer x` parses correctly with latina.h) ✓
- **Macro type lookahead** (recursive resolution: i32→integer→int) ✓
- **Conditional compilation** (#ifdef, #ifndef, #if, #elif, #else, #endif) ✓
- **#if expression evaluation** (arithmetic, comparison, logical, bitwise, ternary, defined()) ✓

**Remaining gaps (low priority):**
- K&R function definitions (old-style parameters)

---

## Phase 2: Rich AST Structure

Extend Arbor2Nodus for tooling support:

```c
structura Arbor2Nodus {
    Arbor2NodusGenus            genus;

    /* Location span */
    i32     linea_initium;
    i32     columna_initium;
    i32     linea_finis;
    i32     columna_finis;

    /* Provenance */
    Arbor2Token*    lexema;
    i32             layer_index;    /* which expansion layer */

    /* Navigation */
    Arbor2Nodus*    pater;

    /* Comments (promoted from token spatia) */
    Xar*    commenta_ante;  /* Leading comment nodes */
    Xar*    commenta_post;  /* Trailing comment nodes */

    /* Type (populated later) - FUTURE */
    structura Arbor2Typus*  typus_resolutum;

    /* Type-specific data */
    unio { ... } datum;
};

/* Note: Whitespace/trivia lives on TOKENS, not nodes:
 *   Arbor2Lexema.spatia_ante  - Xar of Arbor2Lexema* (SPATIA, TABULAE, CONTINUATIO, COMMENTUM_*)
 *   Arbor2Lexema.spatia_post  - Xar of Arbor2Lexema* (SPATIA, TABULAE, CONTINUATIO, COMMENTUM_*)
 * Roundtrip serialization reads these from tokens.
 */
```

#### 2.1 Location Propagation — **COMPLETE** ✓

Implemented 2026-01-16.

**Added to Arbor2Nodus:**
- `linea_initium`, `columna_initium` - Start position (line/column)
- `linea_finis`, `columna_finis` - End position (column is after last char)
- `layer_index` - Macro expansion layer (0 = source, >0 = expanded)

**Implementation:**
- Added `LOCUS_EX_LEXEMATIS(nodus, primus_idx, ultimus_idx)` helper macro in arbor2_glr.c
- Added `LOCUS_EX_LEXEMA(nodus, idx)` single-token helper
- Updated `arbor2_nodus_creare_folium()` for leaf nodes
- Updated ~35 node type cases in reduce logic
- Uses `arbor2_token_radix()` to trace back to invocation site through macro expansions

**Design decisions:**
- **Tight spans** - excludes trivia, just meaningful tokens
- **Invocation site** - macro `X` expanding to `42` reports location of `X`, not definition
- **No byte offsets** - line/column sufficient for current needs

**Coverage:** Expression nodes, statement nodes, declaration nodes, leaf nodes (identifiers, literals). Internal structural nodes (LISTA_SEPARATA) left without location.

**Tests:** 26 new location verification tests (identifier, binary expr, integer literal, function call)

#### 2.2 Trivia Attachment — **COMPLETE** ✓ (superseded by 2.6)
- Originally: Tokens had opaque `Arbor2Trivia` structs with flags
- Now: Replaced by explicit whitespace/comment tokens (see Phase 2.6)
- Roundtrip serialization works via `spatia_ante`/`spatia_post` on tokens
- Arbor2Nodus does NOT have its own trivia fields — trivia flows through tokens
- Comments also accessible via promoted `commenta_ante`/`commenta_post` AST nodes

#### 2.3 Parent Pointer — **COMPLETE** ✓

Implemented 2026-01-16.

**Added to Arbor2Nodus:**
- `pater` - Parent node pointer, NIHIL for root nodes

**Implementation:**
- Added `pater` field to Arbor2Nodus struct after `layer_index`
- Updated node creation helpers to initialize `pater = NIHIL`
- Updated ~40 allocation sites in reduce cases to set `pater = NIHIL`
- Added ~55 child->pater assignments after child assignments

**Coverage:**
- Expression nodes: BINARIUM, UNARIUM, SIZEOF, CONVERSIO, TERNARIUS, SUBSCRIPTIO, VOCATIO, MEMBRUM, POST_UNARIUM
- Statement nodes: SENTENTIA, CORPUS, SI, DUM, FAC, PER, FRANGE, PERGE, REDDE, SALTA, TITULATUM, COMMUTATIO, CASUS, ORDINARIUS
- Declaration nodes: DECLARATIO variants, PARAMETER_DECL, DEFINITIO_FUNCTI

**Tests:** 19 new parent pointer tests covering:
- Binary expression parent pointers
- Nested expression parent chains
- Ternary operator children
- Function call (basis + argumenta)
- Subscript (basis + index)

**Enables:** Navigation up the tree - "what function am I in?", "what's the enclosing statement?"

#### 2.4 Comment Nodes — **COMPLETE**
Comments promoted from trivia to first-class AST nodes:
```c
/* COMMENTUM genus added to Arbor2NodusGenus enum */
ARBOR2_NODUS_COMMENTUM

/* Comment subtype enum */
nomen enumeratio {
    ARBOR2_COMMENTUM_CLAUSUM,   /* Block comment */
    ARBOR2_COMMENTUM_LINEA,     /* Line comment (C99) */
    ARBOR2_COMMENTUM_DOC        /* Doc comment (future) */
} Arbor2CommentumGenus;

/* Comment datum entry */
structura {
    Arbor2CommentumGenus    subgenus;
    chorda                  textus;         /* Without delimiters */
    chorda                  textus_crudus;  /* With delimiters */
    b32                     est_fluitans;   /* Floating comment */
    Xar*                    fragmenta;      /* Future: parsed structure */
} commentum;

/* Attached comments on all AST nodes */
Xar*    commenta_ante;  /* Leading comments */
Xar*    commenta_post;  /* Trailing comments */
```

**Implemented:**
- ARBOR2_NODUS_COMMENTUM genus and Arbor2CommentumGenus subtype enum
- Comment datum entry with text extraction (raw and stripped)
- commenta_ante/commenta_post attachment fields on all nodes
- Helper functions for comment creation and attachment detection
- Comment promotion in TRANSLATION_UNIT parsing
- Serialization of COMMENTUM nodes and attached comments
- Blank line detection for floating comment identification
- End-of-line detection for trailing comment attachment
- 64 tests passing (including comment roundtrip and structure tests)
- Design spec: project-specs/arbor2-comment-spec.md

**Enables:** Querying comments like any other node type, future parsing of comment content

#### 2.6 Tokenize Whitespace — **COMPLETE** ✓

Implemented 2026-01-16.

**Replaced `Arbor2Trivia` with explicit whitespace/comment tokens:**

Before: Whitespace/comments stored as opaque `Arbor2Trivia` structs with flags:
- `est_commentum` - true if comment
- `est_c99` - true if // line comment
- `est_continuatio` - true if contains `\\\n`

After: Whitespace/comments are now typed `Arbor2Lexema*` tokens with specific genus:
- `SPATIA` - contiguous spaces (stores actual " " chars in valor)
- `TABULAE` - contiguous tabs (stores actual "\t" chars in valor)
- `CONTINUATIO` - line continuation `\\\n`
- `COMMENTUM_CLAUSUM` - block comment `/* */`
- `COMMENTUM_LINEA` - line comment `//`

**Key Design Decisions:**
1. Separate SPATIA vs TABULAE - keeps them distinct for formatting tools
2. No mixed whitespace tokens - a run of spaces becomes one SPATIA, then tabs become TABULAE
3. CONTINUATIO is atomic - contains exactly one newline
4. Comment tokens store full text including delimiters

**Files Modified:**
- `include/arbor2_lexema.h` - New enum values, removed Arbor2Trivia, renamed fields
- `lib/arbor2_lexema.c` - New _creare_spatium, rewrote _colligere_spatia
- `lib/arbor2_scribere.c` - Updated to iterate Arbor2Lexema* in spatia
- `lib/arbor2_glr.c` - Updated comment promotion to check genus
- `lib/arbor2_expandere.c` - Field renames

**Tests:** probatio_arbor2_lexema (38/38), probatio_arbor2_expandere (30/30)

**Enables:** Clean typed whitespace queries, simpler serialization, structured trivia

**Deliverable:** AST supports roundtrip formatting
**Current Status:** Roundtrip formatting WORKS via explicit whitespace tokens AND promoted comment nodes. Phase 2 COMPLETE with location spans, parent pointers, comment nodes, and typed whitespace tokens for advanced tooling.

---

## Phase 3: GLR-Expansion Integration

Status: **MOSTLY COMPLETE** (Updated 2026-01-17)

#### 3.1 Typedef Lookahead — **COMPLETE** ✓
`arbor2_glr_est_probabiliter_typus()` queries expansion context during parsing:
- Checks typedef table via `_potest_esse_typus()`
- Checks macro type hints via `_macro_suggerit_typum()`

```c
/* In parser, when seeing identifier in ambiguous position: */
si (tok->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
{
    si (arbor2_expansion_est_typedef(glr->expansion, tok->lexema->valor))
    {
        /* Treat as type */
    }
}
```

#### 3.2 Macro Lookahead — **COMPLETE** ✓

Implemented 2026-01-17.

**API added to arbor2_expandere.h:**
```c
/* Simple genus query */
Arbor2LexemaGenus arbor2_expansion_lookahead_genus(
    Arbor2Expansion*    exp,
    chorda              nomen_macro);

/* Extended lookahead with metadata */
Arbor2ExpansionLookahead arbor2_expansion_lookahead(
    Arbor2Expansion*    exp,
    chorda              nomen_macro);
```

**Parser integration:**
- `_macro_suggerit_typum()` refactored to use public `arbor2_expansion_lookahead()` API
- Follows one level of macro recursion (e.g., `i32` → `integer` → `int`)
- Recognizes type-starting keywords: INT, CHAR, VOID, FLOAT, DOUBLE, LONG, SHORT, SIGNED, UNSIGNED, STRUCT, UNION, ENUM, TYPEDEF, CONST, VOLATILE

**Location:** lib/arbor2_glr.c lines 999-1059

#### 3.3 AMBIGUUS Tracking — **IMPROVED** ✓

Implemented 2026-01-17.

**Before:** `_creare_nodum_ambiguum()` set `identificator = NIHIL` always.

**After:** When lexema is an identifier, stores the identifier causing ambiguity:
```c
si (lexema != NIHIL && lexema->lexema != NIHIL &&
    lexema->lexema->genus == ARBOR2_LEXEMA_IDENTIFICATOR)
{
    nodus->datum.ambiguus.identificator = piscina_allocare(glr->piscina, magnitudo(chorda));
    *(nodus->datum.ambiguus.identificator) = lexema->lexema->valor;
}
```

**Enables:** Future resolution by looking up the identifier that caused ambiguity.

**Location:** lib/arbor2_glr.c lines 1105-1115

#### 3.4 Fork on Unknown — **WORKING**
GLR parser already forks on ambiguous positions:
1. Fork both interpretations ✓
2. Let one die naturally ✓
3. If both survive, create AMBIGUUS node ✓

**Tests added:**
- Latin type macro disambiguation: `integer x` → DECLARATIO (with latina.h)
- Nested macro resolution: `i32 x` → DECLARATIO (i32→integer→int)
- AMBIGUUS tracking: `foo * bar` creates AMBIGUUS with 2+ interpretations

**Tests:** 1856 (up from 1847, +9 new assertions)

**Deliverable:** Parser handles latina.h macros correctly ✓

---

## Phase 4: Error Recovery — **COMPLETE** ✓

Implemented 2026-01-17.

**Before:** Parser stopped on first error, returned FALSUM immediately.

**After:** Parser continues past errors, produces partial AST with ERROR nodes.

#### 4.1 Design Decisions

1. **Recovery granularity: Statement-level only**
   - Uses existing `_invenire_finem_declarationis()` to find item boundaries
   - Synchronizes at `;` and `}` tokens
   - Recovery happens in `arbor2_glr_parsere_translation_unit()`

2. **Error limit: 10 errors max**
   - `ARBOR2_GLR_MAX_ERRORES X` (10 in Roman numerals)
   - Prevents infinite loops on pathological input
   - After limit, returns partial result with `successus = FALSUM`

3. **Error classification: None**
   - Just stores error message and skipped tokens
   - No "probably a declaration" guessing - keeps it simple

#### 4.2 Implementation

**New struct member (arbor2_glr.h):**
```c
i32  num_errores;    /* Error count for recovery limit */
```

**Error node creation (`_creare_nodum_error`):**
```c
Arbor2Nodus* error_nodus;
error_nodus->genus = ARBOR2_NODUS_ERROR;
error_nodus->datum.error.nuntius = /* error message from sub-parse */;
error_nodus->datum.error.lexemata_saltata = /* Xar of skipped tokens */;
```

**Translation unit parser modification:**
- On sub-parse failure: increment error count, check limit
- Collect tokens from failed parse range
- Create ERROR node with skipped tokens
- Add ERROR node to translation unit declarations
- Advance position and continue to next item

#### 4.3 Tests

4 new test sections (21 assertions):
1. Invalid decl + valid decl: `int @ x; int y;` → ERROR + DECLARATIO
2. Multiple consecutive errors: `@ x; @ y; @ z;` → 3 ERROR nodes
3. Error + valid function: `@ badstuff; int foo() {}` → ERROR + DEFINITIO_FUNCTI
4. Error collection: Verifies `res.errores` contains messages

**Deliverable:** Parser continues past errors, produces partial AST ✓

---

## Phase 5: Conditional Compilation — **COMPLETE** ✓

Implemented 2026-01-17 through 2026-01-19 in three sub-phases.

**Phase 5 (commit 5740c60):** Basic directive structure
**Phase 5b (commit 6013f71):** Expression evaluation for #if/#elif
**Phase 5.4 (2026-01-19):** Branch content parsing into AST

#### 5.1 Directive Recognition — **COMPLETE** ✓

**Detection functions in arbor2_glr.c:**
- `_est_conditionale_directivum()` - detects # followed by conditional keyword
- `_est_initium_conditionale()` - checks if position starts a conditional block
- Returns `Arbor2DirectivumGenus` enum value

**Recognized directives:**
- `ARBOR2_DIRECTIVUM_IFDEF` - #ifdef MACRO
- `ARBOR2_DIRECTIVUM_IFNDEF` - #ifndef MACRO
- `ARBOR2_DIRECTIVUM_IF` - #if EXPR
- `ARBOR2_DIRECTIVUM_ELIF` - #elif EXPR
- `ARBOR2_DIRECTIVUM_ELSE` - #else
- `ARBOR2_DIRECTIVUM_ENDIF` - #endif

#### 5.2 Branch Structure — **COMPLETE** ✓

**Arbor2CondRamus (branch structure):**
```c
structura Arbor2CondRamus {
    Arbor2DirectivumGenus   genus;              /* IFDEF, IFNDEF, IF, ELIF, ELSE */
    chorda*                 conditio;           /* Macro name (ifdef/ifndef) */
    Xar*                    expressio_lexemata; /* Expression tokens (#if/#elif) */
    i64                     valor_evaluatus;    /* Evaluated value (0=false, nonzero=true) */
    b32                     est_evaluatum;      /* VERUM if evaluation succeeded */
    Xar*                    lexemata;           /* Tokens in this branch */
    Arbor2Nodus*            parsed;             /* Parsed AST (NIHIL for now) */
    i32                     linea;              /* Line of directive */
};
```

**CONDITIONALIS node datum:**
```c
structura {
    Xar*    rami;           /* Xar of Arbor2CondRamus* */
    i32     linea_if;       /* Line of opening #if/#ifdef */
    i32     linea_endif;    /* Line of closing #endif */
} conditionalis;
```

**Core collection function:**
- `_colligere_conditionale()` - collects entire conditional block
- Handles nested conditionals with depth tracking
- Creates Arbor2CondRamus for each branch
- Evaluates expressions for #if/#elif directives
- Returns single ARBOR2_NODUS_CONDITIONALIS node

#### 5.3 Expression Evaluation — **COMPLETE** ✓

**New file: lib/arbor2_conditio_evaluare.c (~800 lines)**

Recursive descent evaluator with proper C89 precedence:
```
Precedence (low to high):
  ternarius     ? :
  disiunctio    ||
  coniunctio    &&
  or_bitwise    |
  xor_bitwise   ^
  and_bitwise   &
  aequalitas    == !=
  comparatio    < > <= >=
  translatio    << >>
  additio       + -
  multiplicatio * / %
  unarium       ! ~ - +
  primarium     integer, defined, (expr), ident
```

**Supported:**
- Integer constants (decimal, octal 0x, hexadecimal 0x)
- All arithmetic operators: + - * / %
- All comparison operators: < > <= >= == !=
- Logical operators: && || !
- Bitwise operators: & | ^ ~ << >>
- Ternary operator: ? :
- Grouping: ( )
- `defined(MACRO)` and `defined MACRO`
- Macro expansion (single-token object macros)
- Character literals with escape sequences

**API:**
```c
i64 arbor2_conditio_evaluare(
    Arbor2Expansion*    exp,
    Xar*                lexemata,
    b32*                successus);

b32 arbor2_conditio_est_definitum(
    Arbor2Expansion*    exp,
    chorda              titulus);
```

#### 5.4 Branch Parsing — **COMPLETE** ✓

Each branch's `parsed` field is now populated with a TRANSLATION_UNIT AST node containing the parsed declarations from that branch.

**Implementation:**
- `_parsere_ramus()` helper: Creates token copy with synthetic EOF, calls `arbor2_glr_parsere_translation_unit()` recursively
- Branch parsing loop in `_colligere_conditionale()`: Iterates branches after collection, populates `ramus->parsed`
- Empty branches correctly produce `parsed = NIHIL`
- Nested conditionals create nested CONDITIONALIS nodes in the branch AST

#### 5.5 Tests

24+ test cases in probatio_arbor2_glr.c (lines 13100+):

**Phase 5 tests:**
1. Simple #ifdef/#endif
2. #ifdef/#else/#endif
3. #ifndef
4. Multiple independent conditionals
5. Conditionals mixed with regular declarations
6. Nested conditionals

**Phase 5b tests:**
7. #if 1, #if 0
8. #if 1 + 1
9. #if 5 > 3
10. #if 1 && 1
11. #if !0
12. #if defined(UNDEFINED)
13. #if 1 ? 5 : 10
14. #if 0 / #elif 1
15. #ifdef with defined macro
16. #ifndef with undefined macro
17. #if (1 + 2) * 3 == 9
18. #if 0xFF & 0x0F

**Phase 5.4 tests:**
19. Branch parsing: basic (1 declaration)
20. Branch parsing: multiple declarations
21. Branch parsing: nested conditional
22. Branch parsing: empty branch
23. Branch parsing: ifdef with else

**Deliverable:** Full conditional compilation with parsed branch AST ✓

---

## Phase 6: #include Processing

Current: Not implemented
Need: Actually read and process included files

#### 6.1 Include Directive Handling
```c
interior b32
_processare_include(Arbor2Expansion* exp, Xar* tokens, i32* positus)
{
    /* Parse filename from tokens */
    /* Resolve path (search include_viae) */
    /* Check if already included */
    /* Read file */
    /* Recursively process */
}
```

#### 6.2 Include Guard Tracking
```c
TabulaDispersa* included_viae;  /* Already exists in Arbor2Expansion */
```

#### 6.3 System vs User Includes
- `<stdio.h>` - search system paths
- `"myheader.h"` - search current directory first

**Deliverable:** Can parse files with #include

---

## Phase 7: Built-in Definitions

#### 7.1 Compiled-in Latina.h — **COMPLETE** ✓

Implemented 2026-01-17.

**API:** `arbor2_includere_latina(Arbor2Expansion* exp)`

**Location:** lib/arbor2_expandere.c lines 1821-1892

**Implementation note:** Struct field uses `titulus` instead of `nomen` because `nomen` is itself a macro that expands to `typedef`.

**Macros registered (35 total):**

| Category | Latin | C |
|----------|-------|---|
| Keywords | si, alioquin, per, dum, fac, commutatio, casus, ordinarius, frange, perge, redde, salta | if, else, for, while, do, switch, case, default, break, continue, return, goto |
| Type keywords | structura, unio, enumeratio, nomen | struct, union, enum, typedef |
| Qualifiers | constans, volatilis, staticus, externus, registrum, sponte | const, volatile, static, extern, register, auto |
| Types | vacuum, character, integer, brevis, longus, signatus, insignatus, fluitans, duplex | void, char, int, short, long, signed, unsigned, float, double |
| Operators | magnitudo | sizeof |
| Constants | NIHIL, VERUM, FALSUM | NULL, 1, 0 |
| Storage hints | hic_manens, interior, universalis | static |

**Usage:**
```c
Arbor2Expansion* exp = arbor2_expansion_creare(piscina, intern);
arbor2_includere_latina(exp);
/* Now "integer x" parses as declaration because integer→int */
```

#### 7.2 Standard Library Type Hints
```c
/* Common typedefs from system headers - NOT YET IMPLEMENTED */
hic_manens constans character* STDLIB_TYPEDEFS[] = {
    "FILE", "size_t", "ptrdiff_t", "wchar_t",
    "int8_t", "int16_t", "int32_t", "int64_t",
    "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    NIHIL
};
```

**Deliverable:** Out-of-box Latin C89 parsing ✓ (Phase 7.1 complete)

---

## Phase 8: Query Engine

Like arbor_quaestio - CSS-style AST queries.

#### 8.1 Selector Syntax
```
function-definition                    # All functions
identifier[value="main"]               # Identifier named main
function-definition > identifier$fn    # Capture function name
binary-expression[operator="+"]        # Addition expressions
:calls(printf)                         # Calls to printf
:c89 / :c99 / :extension               # Standard filtering
```

#### 8.2 Query API
```c
Arbor2Quaestio* arbor2_quaestio_compilare(
    Piscina* p,
    InternamentumChorda* intern,
    constans character* selector);

Xar* arbor2_quaestio_exsequi(
    Arbor2Quaestio* q,
    Arbor2Nodus* radix);
```

**Deliverable:** Code search and analysis

---

## Phase 9: Type Resolution

Like arbor_typus.

#### 9.1 Type Structure
```c
nomen enumeratio {
    ARBOR2_TYPUS_VOID,
    ARBOR2_TYPUS_CHAR,
    ARBOR2_TYPUS_SHORT,
    ARBOR2_TYPUS_INT,
    ARBOR2_TYPUS_LONG,
    ARBOR2_TYPUS_FLOAT,
    ARBOR2_TYPUS_DOUBLE,
    ARBOR2_TYPUS_STRUCT,
    ARBOR2_TYPUS_UNION,
    ARBOR2_TYPUS_ENUM,
    ARBOR2_TYPUS_POINTER,
    ARBOR2_TYPUS_ARRAY,
    ARBOR2_TYPUS_FUNCTION,
    ARBOR2_TYPUS_TYPEDEF
} Arbor2TypusGenus;
```

#### 9.2 Resolution Pass
Walk AST, resolve types, fill typus_resolutum field.

**Deliverable:** Type information on expressions

---

## Phase 10: Symbol Index

Like arbor_index - cross-file symbols.

**Deliverable:** Go-to-definition, find-references

---

## Phase 11: Formatter

Like arbor_formator.

#### 11.1 Fidelis Mode
Exact roundtrip using trivia.

#### 11.2 Formatus Mode
Apply formatting rules.

**Deliverable:** Code formatting

---

## Phase 12: Public API

Clean builder-pattern interface.

---

## Dependency Graph

```
Phase 1.3 (Multiple Decls) ─────┐
                                │
Phase 1.4 (Specifier Combos) ───┤
                                │
Phase 1.5 (Translation Unit) ───┤
                                ▼
                     [Grammar Complete] ✓
                                │
Phase 2 (Rich AST) ─────────────┤ ✓
                                │
Phase 3 (GLR-Expansion) ────────┤ ✓
                                │
Phase 4 (Error Recovery) ───────┤ ✓
                                │
Phase 5 (Conditionals) ─────────┤ ✓ (complete with branch parsing)
                                │
Phase 6 (#include) ─────────────┤
                                │
Phase 7 (Built-ins) ────────────┤ 7.1 ✓
                                ▼
                       [Core Complete]
                                │
         ┌──────────────────────┼──────────────────────┐
         │                      │                      │
         ▼                      ▼                      ▼
Phase 8 (Queries)      Phase 9 (Types)      Phase 10 (Index)
         │                      │                      │
         └──────────────────────┼──────────────────────┘
                                │
                                ▼
                      Phase 11 (Formatter)
                                │
                                ▼
                       Phase 12 (API)
```

---

## Files Reference (Updated 2026-01-17)

| File | Lines | Status |
|------|-------|--------|
| lib/arbor2_lexema.c | ~500 | Complete (Phase 2.6: explicit whitespace tokens) |
| lib/arbor2_token.c | 264 | Complete |
| lib/arbor2_expandere.c | ~1892 | Mostly complete (lookahead API, built-in latina.h) |
| lib/arbor2_conditio_evaluare.c | ~800 | Complete (Phase 5b: #if/#elif expression evaluator) |
| lib/arbor2_glr.c | ~5800 | Functional + Phase 5 conditional directives |
| lib/arbor2_glr_tabula.c | ~17500 | 346 productions, 952 states |
| lib/arbor2_scribere.c | ~200 | Complete (Phase 2.6: emits spatia tokens) |
| include/arbor2_glr.h | ~820 | Good structure + Arbor2CondRamus + CONDITIONALIS |
| include/arbor2_lexema.h | 220 | Complete (98 token types incl. whitespace) |
| include/arbor2_token.h | 147 | Complete |
| include/arbor2_expandere.h | ~230 | Complete (lookahead API, latina.h function) |
| include/arbor2_conditio_evaluare.h | ~70 | Complete (expression evaluator API) |
| probationes/probatio_arbor2_glr.c | ~13900 | ~1900 tests (Phase 5b conditionals) |
| probationes/probatio_arbor2_lexema.c | ~340 | 38 tests (whitespace tokens) |
| probationes/probatio_arbor2_expandere.c | ~600 | 124 tests (includes lookahead) |
| tools/glr_debug.c | ~340 | Working |
| lib/arbor2_lexema.worklog.md | - | Phase 2.6 design notes |
| lib/arbor2_expandere.worklog.md | - | Phase 3.2, 7.1 design notes |
| lib/arbor2_glr.worklog.md | - | Phase 3.2, 3.3, 4, 5 design notes |

---

## Implementation History

| Date | Phase | Productions | States | Tests | Notes |
|------|-------|-------------|--------|-------|-------|
| Pre-2026-01-14 | Baseline | 154 | 364 | ~1200 | Expressions, statements, basic decls |
| 2026-01-14 | 1.1a Abstract Decl | +37 | +98 | +46 | sizeof(type), pointer casts |
| 2026-01-14 | 1.2a Simple Init | +7 | +14 | +20 | int x = 5 |
| 2026-01-14 | 1.2b Brace Init | +14 | +13 | +50 | {1, 2, 3} |
| 2026-01-14 | 1.2c Designated | +8 | +13 | +35 | {.x = 1, [5] = 100} |
| 2026-01-15 | 1.1b Array sizeof | +4 | +16 | +30 | sizeof(int[10]) |
| 2026-01-15 | 1.1c Multi-dim | +4 | +16 | +30 | sizeof(int[10][20]) |
| 2026-01-15 | 1.1d Ptr array | +4 | +16 | +5 | sizeof(int*[10]) |
| 2026-01-15 | Various fixes | +19 | +61 | +65 | Misc improvements |
| 2026-01-15 | 1.5 Translation Unit | +0 | +0 | +32 | Wrapper approach, no grammar changes |
| 2026-01-15 | 1.6a Function Pointers | +1 | +4 | +16 | int (*fp)(void) - P339, states 901-904 |
| 2026-01-15 | 1.6b Variadic Params | +1 | +1 | +2 | printf(fmt, ...) - P341, state 906 |
| 2026-01-15 | 1.6c Qualified Params | +4 | +13 | +4 | const/volatile in params - P342-P345, states 907-919 |
| 2026-01-15 | 1.4b Qual+TypeMod | +0 | +33 | +0 | const unsigned int, volatile long int, compound combos - states 920-951 |
| 2026-01-16 | 2.1 Location Prop | +0 | +0 | +26 | linea/columna_initium/finis, layer_index on AST nodes |
| 2026-01-16 | 2.3 Parent Pointer | +0 | +0 | +19 | pater field, ~40 alloc sites, ~55 child->pater assignments |
| 2026-01-16 | 2.6 Tokenize Whitespace | +5 | +0 | +0 | SPATIA, TABULAE, CONTINUATIO, COMMENTUM_* tokens; removed Arbor2Trivia |
| 2026-01-17 | 3.2 Macro Lookahead | +0 | +0 | +6 | arbor2_expansion_lookahead() API, parser integration |
| 2026-01-17 | 3.3 AMBIGUUS Tracking | +0 | +0 | +3 | identificator field set in _creare_nodum_ambiguum() |
| 2026-01-17 | 7.1 Built-in Latina | +0 | +0 | +0 | arbor2_includere_latina() with 35 macros |
| 2026-01-17 | 4 Error Recovery | +0 | +0 | +21 | _creare_nodum_error(), translation unit recovery, error limit |
| 2026-01-17 | 5 Conditional Directives | +0 | +0 | +6 | #ifdef/#ifndef/#else/#endif detection, Arbor2CondRamus, CONDITIONALIS node |
| 2026-01-17 | 5b Expression Evaluation | +0 | +0 | +13 | arbor2_conditio_evaluare.c, #if/#elif support, defined() operator |
| 2026-01-19 | 5.4 Branch Parsing | +0 | +0 | +24 | _parsere_ramus(), recursive branch AST parsing, nested conditionals |
| **Current** | | **351** | **952** | **~2069** | |
