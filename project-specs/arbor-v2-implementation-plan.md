# Arbor v2 Implementation Plan

Date: 2026-01-15
Status: Updated after Phase 1.1e completion (sizeof array variants)

---

## Current State (Updated 2026-01-15)

### arbor2_lexema.c - COMPLETE
- 93 token types (all C89 keywords, operators, literals)
- NOVA_LINEA as explicit token (key difference from v1)
- Trivia handling (whitespace, comments attached to tokens)
- Line continuation tracking (est_continuatio flag)
- Standard tracking (ARBOR2_STANDARD_C89/C99/EXTENSION)

### arbor2_token.c - COMPLETE
- Full provenance chain implementation
- Token origins: FONS (source), EXPANSIO (macro), PASTA (##), CHORDA (#), API
- Chain walking: arbor2_token_radix(), arbor2_token_profunditas()
- Merged provenance for token paste

### arbor2_expandere.c (~1576 lines) - MOSTLY COMPLETE
- Object-like and function-like macro expansion
- Parameter substitution
- Stringification (#) - C89 compliant
- Token pasting (##) with re-lexing
- Macro recursion prevention
- Segment-based #define/#undef tracking
- Typedef detection (keyword + "nomen" heuristic)
- Layered expansion (expand to fixpoint, store all layers)
- Layer and segment query APIs

**Missing in expandere:**
- #include processing (files not actually read)
- Conditional compilation (#ifdef, #if, #else, #endif)
- #error, #line, #pragma handling

### arbor2_glr.c + arbor2_glr_tabula.c - SUBSTANTIAL
- GLR parser core with GSS
- GSS path enumeration for multi-path reductions
- Piscina checkpointing for rollback
- **252 grammar productions** (P0-P251)
- **611 LR states** (0-610)
- **1491 tests passing**
- Table validation

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
| sizeof | COMPLETE | sizeof expr, sizeof(type), sizeof(type*), sizeof(type[N]), sizeof(type[N][M]), sizeof(type*[N]), sizeof(struct X) |
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
| Parameters | COMPLETE | |
| Storage classes | PARTIAL | Individual only (no combinations like `static const`) |
| Type qualifiers | PARTIAL | Individual only (no combinations) |
| struct/union | COMPLETE | With members, bit fields |
| enum | COMPLETE | With enumerators and values |
| Arrays | COMPLETE | name[size] |

**Grammar NOT covered (remaining gaps):**

| Missing | Impact | Phase |
|---------|--------|-------|
| Specifier combinations | `static const int x` not parsed | 1.4 |
| Type modifiers | `unsigned int`, `long int` not parsed | 1.4 |
| Multiple declarators | `int x, y` not parsed | 1.3 |
| Pointer qualifiers | `int * const p` not parsed | 1.4 |
| K&R function defs | Old-style parameters | Low priority |
| Translation unit | Top-level external-declaration list | 1.5 |

### Arbor2Nodus Structure - BASIC
Current structure has:
- genus (node type)
- lexema (associated token)
- datum union (38 node type variants including DESIGNATOR_ITEM)

**Missing for tooling:**
- Location spans (byte_initium/finis, linea/columna)
- Trivia (trivia_ante/post)
- Parent pointer (pater)
- Layer index
- Type resolution field

### Tests - COMPREHENSIVE
- 1491 tests passing
- Covers expressions, statements, declarations
- Full initializer coverage (simple, brace, designated)
- sizeof variants (type, pointer, array, multi-dim, pointer array)
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

### 1.3 Multiple Declarators - NOT STARTED

**Current state:** Parser only handles single declarator per type.

**Failing cases:**
- `int x, y;` - FAILS (no comma support in declarations)
- `int x = 1, y = 2;` - FAILS
- `int *x, y;` - FAILS

**Required:**
```
declaration -> specifiers init_declarator_list ';'
init_declarator_list -> init_declarator
init_declarator_list -> init_declarator_list ',' init_declarator
init_declarator -> declarator
init_declarator -> declarator '=' initializer
```

Estimated: ~5 productions, ~10 states

### 1.4 Specifier Combinations - NOT STARTED

**Current state:** Parser only handles single storage class OR single qualifier.

**Working:**
- `static int x;` ✓
- `const int x;` ✓
- `volatile int y;` ✓

**Failing:**
- `static const int x;` - FAILS
- `extern volatile int y;` - FAILS
- `unsigned int z;` - FAILS
- `long int w;` - FAILS
- `unsigned long int v;` - FAILS

**Required:**
```
decl_specifiers -> decl_specifier
decl_specifiers -> decl_specifiers decl_specifier

decl_specifier -> storage_class_specifier
decl_specifier -> type_qualifier
decl_specifier -> type_specifier

type_specifier -> 'int' | 'char' | 'void' | 'short' | 'long'
                | 'signed' | 'unsigned' | 'float' | 'double'
```

This is a substantial refactor - the type specifier lexemes exist but aren't wired into grammar.

Estimated: ~15 productions, ~25 states

### 1.5 Translation Unit - NOT STARTED

**Current state:** Parser handles single declaration/function, cannot sequence multiple.

**Working:**
- `int foo() { return 1; }` ✓ (single function)
- `int x;` ✓ (single declaration)

**Failing:**
- `int a; int b;` - FAILS (multiple declarations)
- `int foo() {} int x;` - FAILS (function + declaration)

**Required:**
```
translation_unit -> external_declaration
translation_unit -> translation_unit external_declaration

external_declaration -> function_definition
external_declaration -> declaration ';'
```

Would also need new entry point: `arbor2_glr_parsere_translation_unit()`

Estimated: ~4 productions, ~5 states

**Deliverable:** Can parse complete C89 files

---

## Recommended Next Steps

Given the current state, the recommended priority is:

1. **Phase 1.4 (Specifier Combinations)** - Enables real C code with `static const`, `unsigned int`, etc. This is the most impactful remaining grammar gap.

2. **Phase 1.5 (Translation Unit)** - Enables parsing complete files. Relatively small change but high impact.

3. **Phase 1.3 (Multiple Declarators)** - Convenience feature, less critical than above.

Alternatively, **Phase 2 (Rich AST)** could come first if the goal is to enable formatter development sooner, since the grammar is already quite functional for single-declaration parsing.

---

## Phase 2: Rich AST Structure

Extend Arbor2Nodus for tooling support:

```c
structura Arbor2Nodus {
    Arbor2NodusGenus            genus;

    /* Location span - NEW */
    i32     byte_initium;
    i32     byte_finis;
    i32     linea_initium;
    i32     columna_initium;
    i32     linea_finis;
    i32     columna_finis;

    /* Trivia for roundtrip - NEW */
    Xar*    trivia_ante;
    Xar*    trivia_post;

    /* Provenance - EXISTING but enhance */
    Arbor2Token*    lexema;
    i32             layer_index;    /* NEW: which expansion layer */

    /* Navigation - NEW */
    Arbor2Nodus*    pater;

    /* Type (populated later) - NEW */
    structura Arbor2Typus*  typus_resolutum;

    /* Type-specific data - EXISTING */
    unio { ... } datum;
};
```

#### 2.1 Location Propagation
- Modify AST construction in arbor2_glr.c
- Compute spans from first/last tokens in production
- ~50 locations in _exequi_reduce need updating

#### 2.2 Trivia Attachment
- Tokens already have trivia_ante/post from lexer
- Copy to AST nodes during construction
- Decision: Leading trivia attaches to following node

#### 2.3 Parent Pointer
- Set during AST construction
- ~50 locations to update

#### 2.4 Comment Nodes
Add explicit comment handling:
```c
nomen structura {
    chorda              textus;
    Arbor2CommentLocus  locus;      /* OWN_LINE, END_OF_LINE, INLINE */
    b32                 est_multiline;
    Arbor2Nodus*        attached_to;
    b32                 ante;
} Arbor2Commentum;
```

**Deliverable:** AST supports roundtrip formatting

---

## Phase 3: GLR-Expansion Integration

The infrastructure exists but isn't connected:

#### 3.1 Typedef Lookahead
Current: arbor2_glr_est_probabiliter_typus() exists but limited
Need: Query expansion context during parsing

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

#### 3.2 Macro Lookahead
Add to expansion context:
```c
/* Query what first token of macro expansion would be */
Arbor2LexemaGenus arbor2_expansion_lookahead_genus(
    Arbor2Expansion*    exp,
    chorda              identifier);
```

Use during parsing to guide decisions (e.g., is this macro function-like?).

#### 3.3 Fork on Unknown
When typedef/macro status unknown:
1. Fork both interpretations
2. Let one die naturally
3. If both survive, create ambiguity node

Current: ARBOR2_NODUS_AMBIGUUS exists but unused
Need: Actually create these during parsing

**Deliverable:** Parser handles latina.h macros correctly

---

## Phase 4: Error Recovery

Current: Parser stops on first error
Need: Continue and produce partial AST

#### 4.1 Panic Mode Recovery
On error, skip to synchronizing tokens:
- `;` (statement boundary)
- `}` (block boundary)
- `)` (expression boundary)

```c
interior vacuum
_recuperare_panic(Arbor2GLR* glr)
{
    dum (_currens_lexema(glr)->lexema->genus != ARBOR2_LEXEMA_EOF)
    {
        Arbor2LexemaGenus g;
        g = _currens_lexema(glr)->lexema->genus;

        si (g == ARBOR2_LEXEMA_SEMICOLON ||
            g == ARBOR2_LEXEMA_BRACE_CLAUSA)
        {
            _progredi(glr);
            redde;
        }
        _progredi(glr);
    }
}
```

#### 4.2 Error Node Creation
```c
Arbor2Nodus* error_nodus;
error_nodus = piscina_allocare(glr->piscina, magnitudo(Arbor2Nodus));
error_nodus->genus = ARBOR2_NODUS_ERROR;
error_nodus->datum.error.nuntius = /* error message */;
error_nodus->datum.error.lexemata_saltata = /* skipped tokens */;
```

#### 4.3 Error Productions (optional)
Add grammar rules that match common errors:
```
stmt -> error ';'  /* recover at semicolon */
stmt -> error '}'  /* recover at brace */
```

**Deliverable:** Parser continues past errors, produces partial AST

---

## Phase 5: Conditional Compilation

Current: Not implemented at all
Need: Parse ALL branches

#### 5.1 Directive Recognition in Parser
Extend parser to recognize #ifdef, #if, #else, #elif, #endif

#### 5.2 Branch Structure
```c
nomen structura {
    Arbor2DirectivaGenus    genus;          /* IFDEF, IF, ELSE, ELIF, ENDIF */
    chorda*                 condition_text; /* "DEBUG", "x > 0", etc. */
    Xar*                    lexemata;       /* Tokens in directive line */
} Arbor2Directiva;

nomen structura {
    Xar*                    branches;       /* Xar of Arbor2CondBranch* */
    i32                     linea_if;
    i32                     linea_endif;
} Arbor2Conditional;

nomen structura {
    Arbor2Directiva*        directiva;      /* The #if/#elif/#else */
    Arbor2Nodus*            parsed;         /* AST for this branch */
} Arbor2CondBranch;
```

#### 5.3 Node-Based Forking
Don't duplicate entire file per #ifdef - create local fork:
```
CONDITIONAL_NODE
  ├─ branch[0]: #ifdef DEBUG → { parsed_stmts_A }
  └─ branch[1]: #else → { parsed_stmts_B }
```

**Deliverable:** Can search across all conditional branches

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

#### 7.1 Compiled-in Latina.h
```c
hic_manens constans structura {
    constans character* nomen;
    constans character* valor;
} LATINA_MACROS[] = {
    { "si", "if" },
    { "alioquin", "else" },
    { "per", "for" },
    { "dum", "while" },
    { "fac", "do" },
    { "commutatio", "switch" },
    { "casus", "case" },
    { "ordinarius", "default" },
    { "frange", "break" },
    { "perge", "continue" },
    { "redde", "return" },
    { "salta", "goto" },
    { "structura", "struct" },
    { "unio", "union" },
    { "enumeratio", "enum" },
    { "nomen", "typedef" },
    { "constans", "const" },
    { "volatilis", "volatile" },
    { "staticus", "static" },
    { "externus", "extern" },
    { "registrum", "register" },
    { "sponte", "auto" },
    { "vacuum", "void" },
    { "character", "char" },
    { "integer", "int" },
    { "brevis", "short" },
    { "longus", "long" },
    { "signatus", "signed" },
    { "insignatus", "unsigned" },
    { "fluitans", "float" },
    { "duplex", "double" },
    { "magnitudo", "sizeof" },
    { "NIHIL", "NULL" },
    { "VERUM", "1" },
    { "FALSUM", "0" },
    { "hic_manens", "static" },
    { "interior", "static" },
    { "universalis", "static" },
    { NIHIL, NIHIL }
};

vacuum arbor2_includere_latina(Arbor2Expansion* exp)
{
    i32 i;
    per (i = ZEPHYRUM; LATINA_MACROS[i].nomen != NIHIL; i++)
    {
        arbor2_expansion_addere_macro(exp,
            LATINA_MACROS[i].nomen,
            LATINA_MACROS[i].valor,
            "latina.h");
    }
}
```

#### 7.2 Standard Library Type Hints
```c
/* Common typedefs from system headers */
hic_manens constans character* STDLIB_TYPEDEFS[] = {
    "FILE", "size_t", "ptrdiff_t", "wchar_t",
    "int8_t", "int16_t", "int32_t", "int64_t",
    "uint8_t", "uint16_t", "uint32_t", "uint64_t",
    NIHIL
};
```

**Deliverable:** Out-of-box Latin C89 parsing

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
                     [Grammar Complete]
                                │
Phase 2 (Rich AST) ─────────────┤
                                │
Phase 3 (GLR-Expansion) ────────┤
                                │
Phase 4 (Error Recovery) ───────┤
                                │
Phase 5 (Conditionals) ─────────┤
                                │
Phase 6 (#include) ─────────────┤
                                │
Phase 7 (Built-ins) ────────────┤
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

## Files Reference (Updated)

| File | Lines | Status |
|------|-------|--------|
| lib/arbor2_lexema.c | ~500 | Complete |
| lib/arbor2_token.c | 264 | Complete |
| lib/arbor2_expandere.c | 1576 | Mostly complete |
| lib/arbor2_glr.c | ~2700 | Functional |
| lib/arbor2_glr_tabula.c | ~13000 | 252 productions, 611 states |
| include/arbor2_glr.h | ~650 | Good structure |
| include/arbor2_lexema.h | 220 | Complete |
| include/arbor2_token.h | 147 | Complete |
| include/arbor2_expandere.h | 197 | Complete |
| probationes/probatio_arbor2_glr.c | ~9500 | 1491 tests |
| tools/glr_debug.c | 311 | Working |

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
| **Current** | | **252** | **611** | **1491** | |
