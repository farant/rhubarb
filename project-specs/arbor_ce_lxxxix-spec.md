# arbor_ce_lxxxix - C89 Parser Library Specification

## Overview

A hand-written recursive descent C89 parser library with full preprocessor support, macro expansion tracking, CSS-like AST querying, and roundtrip-preserving formatting. Designed for syntax highlighting, code intelligence, static analysis, and code generation within the rhubarb ecosystem.

## Goals

1. Parse C89 with selective C99 support (flaggable for linting)
2. Track macro expansion provenance for source mapping
3. Enable CSS-selector-style AST queries with captures
4. Support roundtrip fidelity: `code -> parse -> emit == code`
5. Provide semantic tokens for syntax highlighting
6. Build incrementally as testable, composable modules

## Non-Goals (Initial Version)

- Cross-platform portability (macOS only for now)
- Incremental/fine-grained re-parsing
- Disk-based index caching
- Multi-threaded parsing

---

## Architecture

### Module Breakdown

```
arbor_ce_lxxxix/
├── lexema/           # Lexer - tokenization
├── praeparator/      # Preprocessor - macro expansion
├── syntaxis/         # Parser - AST construction
├── typus/            # Type resolver
├── index/            # Project-wide symbol index
├── quaestio/         # Query engine (CSS selectors)
├── formator/         # Code formatter/pretty-printer
└── stml/             # STML serialization
```

### Dependencies

- **piscina** - Arena memory allocation
- **internamentum** - String interning
- **chorda** - String type
- **xar** - Dynamic arrays

---

## Module 1: Lexema (Lexer)

### Purpose
Tokenize C source into a stream of tokens with full location and trivia tracking.

### Token Structure

```c
nomen enumeratio {
    /* Keywords */
    LEXEMA_AUTO, LEXEMA_BREAK, LEXEMA_CASE, LEXEMA_CHAR,
    LEXEMA_CONST, LEXEMA_CONTINUE, LEXEMA_DEFAULT, LEXEMA_DO,
    LEXEMA_DOUBLE, LEXEMA_ELSE, LEXEMA_ENUM, LEXEMA_EXTERN,
    LEXEMA_FLOAT, LEXEMA_FOR, LEXEMA_GOTO, LEXEMA_IF,
    LEXEMA_INT, LEXEMA_LONG, LEXEMA_REGISTER, LEXEMA_RETURN,
    LEXEMA_SHORT, LEXEMA_SIGNED, LEXEMA_SIZEOF, LEXEMA_STATIC,
    LEXEMA_STRUCT, LEXEMA_SWITCH, LEXEMA_TYPEDEF, LEXEMA_UNION,
    LEXEMA_UNSIGNED, LEXEMA_VOID, LEXEMA_VOLATILE, LEXEMA_WHILE,

    /* Literals */
    LEXEMA_IDENTIFICATOR,
    LEXEMA_INTEGER_LITERAL,
    LEXEMA_FLOAT_LITERAL,
    LEXEMA_CHAR_LITERAL,
    LEXEMA_STRING_LITERAL,

    /* Operators & punctuation */
    LEXEMA_PLUS, LEXEMA_MINUS, LEXEMA_ASTERISK, /* ... */

    /* Preprocessor */
    LEXEMA_HASH,
    LEXEMA_HASH_HASH,

    /* Special */
    LEXEMA_EOF,
    LEXEMA_ERROR,

    /* C99 (flagged) */
    LEXEMA_C99_COMMENT  /* // style */
} LexemaGenus;

nomen structura {
    LexemaGenus    genus;
    chorda         valor;           /* Interned identifier/literal text */

    /* Location */
    i32            byte_offset;
    i32            linea;
    i32            columna;
    i32            longitudo;       /* In bytes */

    /* Trivia (whitespace, comments) */
    Xar*           trivia_ante;     /* Leading trivia */
    Xar*           trivia_post;     /* Trailing trivia */

    /* Standard tracking */
    i8             standard;        /* 0=C89, 1=C99, 2=extension */
} Lexema;

nomen structura {
    character*     datum;
    i32            mensura;
    b32            est_commentum;   /* vs whitespace */
    b32            est_c99;         /* // comment */
} Trivia;
```

### API

```c
/* Create lexer from source */
Lexator* lexema_creare(Piscina* piscina, constans character* fons, i32 mensura);

/* Get next token */
Lexema* lexema_proximum(Lexator* lexator);

/* Peek without consuming */
Lexema* lexema_prospicere(Lexator* lexator, i32 ahead);

/* Get all tokens (for preprocessor) */
Xar* lexema_omnia(Lexator* lexator);
```

---

## Module 2: Praeparator (Preprocessor)

### Purpose
Handle #directives, expand macros, track expansion provenance.

### Expansion Tracking

```c
nomen structura {
    chorda         nomen_macro;     /* Name of macro that was expanded */
    i32            expansio_index;  /* Which expansion level (0 = original) */
    i32            linea_originalis;
    i32            columna_originalis;
    OrigoPater*    pater;           /* Parent expansion (for nesting) */
} Origo;

nomen structura {
    Lexema*        lexema;
    Origo*         origo;           /* NULL if not from macro */
} LexemaOrigo;
```

### Directive Handling

```c
nomen enumeratio {
    DIRECTIVA_INCLUDE,
    DIRECTIVA_DEFINE,
    DIRECTIVA_UNDEF,
    DIRECTIVA_IF,
    DIRECTIVA_IFDEF,
    DIRECTIVA_IFNDEF,
    DIRECTIVA_ELIF,
    DIRECTIVA_ELSE,
    DIRECTIVA_ENDIF,
    DIRECTIVA_LINE,
    DIRECTIVA_ERROR,
    DIRECTIVA_PRAGMA,
    DIRECTIVA_UNKNOWN
} DirectivaGenus;
```

### API

```c
/* Create preprocessor */
Praeparator* praeparator_creare(Piscina* piscina, InternamentumChorda* intern);

/* Add include path */
vacuum praeparator_addere_via(Praeparator* pp, constans character* via);

/* Define macro */
vacuum praeparator_definire(Praeparator* pp, constans character* nomen, constans character* valor);

/* Process file, return expanded token stream with origins */
Xar* praeparator_processare(Praeparator* pp, constans character* via_file);

/* Get macro definition (for hover info) */
MacroDefinitio* praeparator_obtinere_macro(Praeparator* pp, chorda* nomen);
```

---

## Module 3: Syntaxis (Parser)

### Purpose
Build AST from token stream using recursive descent.

### Node Types (lowercase-kebab-case for CSS selectors)

```c
nomen enumeratio {
    /* Declarations */
    NODUS_TRANSLATION_UNIT,
    NODUS_FUNCTION_DEFINITION,
    NODUS_DECLARATION,
    NODUS_PARAMETER_DECLARATION,
    NODUS_STRUCT_DECLARATION,
    NODUS_UNION_DECLARATION,
    NODUS_ENUM_DECLARATION,
    NODUS_TYPEDEF_DECLARATION,

    /* Statements */
    NODUS_COMPOUND_STATEMENT,
    NODUS_IF_STATEMENT,
    NODUS_SWITCH_STATEMENT,
    NODUS_CASE_STATEMENT,
    NODUS_DEFAULT_STATEMENT,
    NODUS_WHILE_STATEMENT,
    NODUS_DO_STATEMENT,
    NODUS_FOR_STATEMENT,
    NODUS_GOTO_STATEMENT,
    NODUS_CONTINUE_STATEMENT,
    NODUS_BREAK_STATEMENT,
    NODUS_RETURN_STATEMENT,
    NODUS_EXPRESSION_STATEMENT,

    /* Expressions */
    NODUS_BINARY_EXPRESSION,
    NODUS_UNARY_EXPRESSION,
    NODUS_CALL_EXPRESSION,
    NODUS_MEMBER_EXPRESSION,
    NODUS_SUBSCRIPT_EXPRESSION,
    NODUS_CAST_EXPRESSION,
    NODUS_SIZEOF_EXPRESSION,
    NODUS_CONDITIONAL_EXPRESSION,
    NODUS_ASSIGNMENT_EXPRESSION,
    NODUS_COMMA_EXPRESSION,

    /* Primary */
    NODUS_IDENTIFIER,
    NODUS_INTEGER_LITERAL,
    NODUS_FLOAT_LITERAL,
    NODUS_CHAR_LITERAL,
    NODUS_STRING_LITERAL,

    /* Types */
    NODUS_TYPE_SPECIFIER,
    NODUS_POINTER,
    NODUS_ARRAY_DECLARATOR,
    NODUS_FUNCTION_DECLARATOR,

    /* Special */
    NODUS_ERROR,              /* For error recovery */
    NODUS_EXTENSION           /* Opaque platform extension */
} NodusGenus;
```

### Node Structure

```c
nomen structura Nodus {
    NodusGenus         genus;

    /* Location span */
    i32                byte_initium;
    i32                byte_finis;
    i32                linea_initium;
    i32                linea_finis;

    /* Trivia for roundtrip */
    Xar*               trivia_ante;
    Xar*               trivia_post;

    /* Standard tracking */
    i8                 standard;      /* C89/C99/extension */

    /* Expansion provenance (optional) */
    Origo*             origo;

    /* Parent (for tree navigation) */
    structura Nodus*   pater;

    /* Children (type-specific) */
    unio {
        structura { /* function-definition */
            Nodus* return_type;
            Nodus* declarator;
            Xar*   parameters;
            Nodus* body;
        } functio;

        structura { /* if-statement */
            Nodus* conditio;
            Nodus* consequens;
            Nodus* alternans;    /* else branch, may be NULL */
        } si;

        structura { /* binary-expression */
            Nodus*     sinister;
            LexemaGenus operator;
            Nodus*     dexter;
        } binarium;

        /* ... other node types ... */

        structura { /* identifier, literals */
            chorda  valor;
        } folium;

        structura { /* error node */
            chorda  nuntius;
            Xar*    lexemata_praetermissa;  /* Skipped tokens */
        } error;
    } datum;
} Nodus;
```

### Error Recovery Strategy

Following tree-sitter approach:
1. On parse error, create ERROR node
2. Capture skipped tokens in the error node
3. Use synchronization points (`;`, `}`, keywords) to resume
4. Continue parsing rest of file

### API

```c
/* Create parser */
Syntaxis* syntaxis_creare(Piscina* piscina, InternamentumChorda* intern);

/* Parse token stream to AST */
Nodus* syntaxis_parsere(Syntaxis* syn, Xar* lexemata);

/* Parse with visitor callbacks */
Nodus* syntaxis_parsere_cum_visitore(Syntaxis* syn, Xar* lexemata, VisitorFunctiones* vis);

/* Get parse errors */
Xar* syntaxis_errores(Syntaxis* syn);
```

---

## Module 4: Typus (Type Resolver)

### Purpose
Resolve type references, build type graph, track declarations.

### Type Representation

```c
nomen enumeratio {
    TYPUS_VOID,
    TYPUS_CHAR,
    TYPUS_SHORT,
    TYPUS_INT,
    TYPUS_LONG,
    TYPUS_FLOAT,
    TYPUS_DOUBLE,
    TYPUS_SIGNED,
    TYPUS_UNSIGNED,
    TYPUS_STRUCT,
    TYPUS_UNION,
    TYPUS_ENUM,
    TYPUS_POINTER,
    TYPUS_ARRAY,
    TYPUS_FUNCTION,
    TYPUS_TYPEDEF
} TypusGenus;

nomen structura {
    TypusGenus      genus;
    chorda*         nomen;          /* For struct/union/enum/typedef */
    Typus*          basis;          /* For pointer/array/typedef */
    i32             array_mensura;  /* -1 for unsized */
    Xar*            parametra;      /* For function types */
    Typus*          reditum;        /* Function return type */
    b32             est_const;
    b32             est_volatile;
} Typus;
```

### API

```c
/* Create type resolver */
TypusResolver* typus_creare(Piscina* piscina);

/* Resolve types in AST (modifies nodes with type info) */
vacuum typus_resolvere(TypusResolver* res, Nodus* arbor);

/* Get type of expression */
Typus* typus_expressionis(TypusResolver* res, Nodus* expr);

/* Get declaration of identifier */
Nodus* typus_declaratio(TypusResolver* res, chorda* nomen);
```

---

## Module 5: Index (Project Index)

### Purpose
Cross-file symbol resolution, go-to-definition, find-references.

### Project Structure

```c
nomen structura {
    chorda          via;            /* Relative path */
    chorda*         fons;           /* Source content */
    Nodus*          arbor;          /* Parsed AST */
    i64             tempus_mutatum; /* For cache invalidation */
} Filum;

nomen structura {
    Piscina*        piscina;
    Xar*            fila;           /* Array of Filum */
    TabulaDispersa* symbola;        /* Global symbol table */
    Xar*            viae_include;   /* Include paths */
} Proiectum;
```

### Symbol Entry

```c
nomen enumeratio {
    SYMBOLUM_FUNCTIO,
    SYMBOLUM_VARIABILIS,
    SYMBOLUM_TYPUS,
    SYMBOLUM_MACRO,
    SYMBOLUM_ENUM_VALOR
} SymbolumGenus;

nomen enumeratio {
    NEXUS_EXTERNUS,     /* extern */
    NEXUS_INTERNUS,     /* static */
    NEXUS_NULLUS        /* local/parameter */
} Nexus;

nomen structura {
    chorda          nomen;
    SymbolumGenus   genus;
    Nexus           nexus;
    Filum*          filum;
    Nodus*          declaratio;
    Nodus*          definitio;      /* May differ from declaration */
    Xar*            referentiae;    /* All usage sites */
} Symbolum;
```

### API

```c
/* Create project */
Proiectum* index_creare(Piscina* piscina);

/* Add files to project */
vacuum index_addere_filum(Proiectum* proj, constans character* via);
vacuum index_addere_folder(Proiectum* proj, constans character* via, constans character* pattern);

/* Build/rebuild index */
vacuum index_aedificare(Proiectum* proj);

/* Query */
Symbolum* index_invenire(Proiectum* proj, chorda* nomen);
Xar* index_referentiae(Proiectum* proj, chorda* nomen);
Nodus* index_definitio_ad_punctum(Proiectum* proj, constans character* via, i32 linea, i32 columna);
```

---

## Module 6: Quaestio (Query Engine)

### Purpose
CSS-selector-style AST queries with captures.

### Query Syntax

```
# Basic selectors
function-definition                    # All function definitions
identifier                             # All identifiers

# Attribute selectors
function-definition[name="main"]       # Function named main
identifier[value^="str_"]              # Identifiers starting with str_

# Descendant/child
function-definition > compound-statement > if-statement
function-definition if-statement       # Any descendant

# Pseudo-selectors (structural)
:first-child
:last-child
:nth-child(2)
:not(expression-statement)

# Semantic pseudo-selectors
:calls(printf)                         # Calls to printf
:defines(variable)                     # Variable definitions
:uses(identifier)                      # Uses of identifier
:returns                               # Has return statement
:c99                                   # C99-only constructs

# Captures
function-definition > identifier$name  # Capture function name as $name
if-statement > expression$cond         # Capture condition as $cond

# Combinators
selector1, selector2                   # Either matches
selector1 + selector2                  # Adjacent sibling
selector1 ~ selector2                  # General sibling
```

### Query Result

```c
nomen structura {
    Nodus*          nodus;          /* Matched node */
    TabulaDispersa* capturae;       /* name -> Nodus* */
} QuaestioResultatum;
```

### API

```c
/* Compile query */
Quaestio* quaestio_compilare(Piscina* piscina, constans character* selector);

/* Execute query on AST */
Xar* quaestio_exsequi(Quaestio* q, Nodus* arbor);

/* Check if node matches */
b32 quaestio_congruit(Quaestio* q, Nodus* nodus);

/* Register custom pseudo-selector */
vacuum quaestio_registrare_pseudo(
    constans character* nomen,
    b32 (*functio)(Nodus*, vacuum*),
    vacuum* datum);
```

---

## Module 7: Formator (Formatter)

### Purpose
Pretty-print AST back to source code with consistent style.

### Formatting Rules (Fran's Style)

- Allman brace style (braces on own line)
- 4-space indentation (tabs stored, spaces for display)
- Space after keywords (`if (`, not `if(`)
- No space inside parens (`(x + y)`, not `( x + y )`)
- Operators with spaces (`a + b`, not `a+b`)
- Single blank line between functions
- Comments preserved in original positions

### API

```c
/* Format AST to string */
chorda formator_emittere(Piscina* piscina, Nodus* arbor);

/* Format with roundtrip preservation (keep original trivia) */
chorda formator_emittere_fidelis(Piscina* piscina, Nodus* arbor);

/* Format selection only */
chorda formator_emittere_selectionem(Piscina* piscina, Nodus* nodus);
```

---

## Module 8: STML Serializer

### Purpose
Serialize AST to STML for caching, interchange, and transformation DSL.

### STML Representation

```xml
<translation-unit>
  <function-definition name="main" linea="42" standard="c89">
    <trivia-ante>/* Main entry point */\n</trivia-ante>
    <return-type>
      <type-specifier>int</type-specifier>
    </return-type>
    <declarator>
      <identifier>main</identifier>
      <parameter-list>
        <parameter-declaration>
          <type-specifier>int</type-specifier>
          <identifier>argc</identifier>
        </parameter-declaration>
      </parameter-list>
    </declarator>
    <compound-statement>
      <!-- body -->
    </compound-statement>
  </function-definition>
</translation-unit>
```

### Transformation DSL

```xml
<!-- Match pattern -->
<transform selector="if-statement[condition/identifier='VERUM']">
  <!-- Replace with simplified version -->
  <%replace>
    <compound-statement>
      <%children selector="> compound-statement > *" />
    </compound-statement>
  </%replace>
</transform>

<!-- Append to matched nodes -->
<transform selector="function-definition">
  <%prepend selector="> compound-statement">
    <expression-statement>
      <call-expression>
        <identifier>TRACE_ENTER</identifier>
      </call-expression>
    </expression-statement>
  </%prepend>
</transform>
```

### API

```c
/* Serialize AST to STML */
chorda stml_serialisare(Piscina* piscina, Nodus* arbor, b32 includere_trivia);

/* Parse STML back to AST */
Nodus* stml_deserialisare(Piscina* piscina, chorda stml);

/* Apply transformation */
Nodus* stml_transformare(Piscina* piscina, Nodus* arbor, chorda transformatio);
```

---

## Integration: Semantic Tokens for Highlighting

```c
nomen enumeratio {
    TOKEN_KEYWORD,
    TOKEN_TYPE,
    TOKEN_FUNCTION,
    TOKEN_VARIABLE,
    TOKEN_PARAMETER,
    TOKEN_MACRO,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_COMMENT,
    TOKEN_PREPROCESSOR
} TokenSemanticum;

nomen structura {
    i32             linea;
    i32             columna;
    i32             longitudo;
    TokenSemanticum genus;
} TokenColoratio;

/* Get semantic tokens for highlighting */
Xar* arbor_semantic_tokens(Nodus* arbor, TypusResolver* types);
```

---

## Error Handling

```c
nomen enumeratio {
    ERROR_LEXEMA,       /* Tokenization error */
    ERROR_PRAEPARATOR,  /* Preprocessor error */
    ERROR_SYNTAXIS,     /* Parse error */
    ERROR_TYPUS,        /* Type error */
    ERROR_LINT          /* Lint warning */
} ErrorGenus;

nomen enumeratio {
    SEVERITAS_ERROR,
    SEVERITAS_WARNING,
    SEVERITAS_INFO
} Severitas;

nomen structura {
    ErrorGenus      genus;
    Severitas       severitas;
    chorda          nuntius;
    chorda          via_file;
    i32             linea;
    i32             columna;
    i32             longitudo;
    chorda          fons_linea;     /* Source line for context */
} Error;

/* Format error for display */
chorda error_formare(Piscina* piscina, Error* err);
```

---

## Implementation Phases

### Phase 1: Lexer
- Token types for C89 + C99 comments
- Location tracking (byte, line, column)
- Trivia capture (leading + trailing)
- Standard tagging (C89/C99)
- Unit tests for all token types

### Phase 2: Preprocessor
- #define (object-like and function-like)
- #include (with path resolution)
- #if/#ifdef/#ifndef/#elif/#else/#endif
- Macro expansion with origin tracking
- Nested expansion chains
- Unit tests for macro scenarios

### Phase 3: Parser (Core)
- Expressions (precedence climbing)
- Statements (if, while, for, switch, etc.)
- Function definitions
- Struct/union/enum declarations
- Error recovery with ERROR nodes
- Unit tests per grammar rule

### Phase 4: Parser (Complete)
- Complex declarators (pointers, arrays, functions)
- Typedef handling
- Extension nodes for __attribute__ etc.
- Full C89 grammar coverage
- Roundtrip tests (parse -> format == original)

### Phase 5: Type Resolver
- Primitive types
- Struct/union member resolution
- Typedef resolution
- Function signature matching
- Scope tracking

### Phase 6: Index
- Single-file symbol table
- Cross-file resolution
- Go-to-definition
- Find-references
- Project management

### Phase 7: Query Engine
- Selector parser
- Structural pseudo-selectors
- Semantic pseudo-selectors
- Capture groups
- Custom pseudo-selector registration

### Phase 8: Formatter
- AST to source emission
- Style rules
- Trivia preservation for roundtrip

### Phase 9: STML
- Serialization
- Deserialization
- Transformation DSL

---

## Testing Strategy

Each module gets:
1. Unit tests for individual functions
2. Integration tests with other modules
3. Roundtrip tests where applicable
4. Error case coverage

Test file naming: `probatio_arbor_lexema.c`, `probatio_arbor_syntaxis.c`, etc.

---

## Files

```
include/
  arbor_ce_lxxxix.h      # Main header (includes all)
  arbor_lexema.h
  arbor_praeparator.h
  arbor_syntaxis.h
  arbor_typus.h
  arbor_index.h
  arbor_quaestio.h
  arbor_formator.h
  arbor_stml.h

lib/
  arbor_lexema.c
  arbor_praeparator.c
  arbor_syntaxis.c
  arbor_typus.c
  arbor_index.c
  arbor_quaestio.c
  arbor_formator.c
  arbor_stml.c

probationes/
  probatio_arbor_lexema.c
  probatio_arbor_praeparator.c
  probatio_arbor_syntaxis.c
  probatio_arbor_typus.c
  probatio_arbor_index.c
  probatio_arbor_quaestio.c
  probatio_arbor_formator.c
  probatio_arbor_stml.c
```
