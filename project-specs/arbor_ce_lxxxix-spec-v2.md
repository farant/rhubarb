# arbor_ce_lxxxix - C89 Parser Library Specification v2

## Overview

A hand-written recursive descent C89 parser library with full preprocessor support, macro expansion tracking, CSS-like AST querying, and roundtrip-preserving formatting. Designed to integrate seamlessly with the rhubarb ecosystem.

**Name**: arbor_ce_lxxxix (Latin: "tree of C89")

---

## Codebase Integration

### Dependencies (Existing Libraries)

| Library | Purpose | Usage in arbor |
|---------|---------|----------------|
| `piscina` | Arena memory allocation | All allocations |
| `internamentum` | String interning | Identifiers, keywords, type names |
| `chorda` | Non-null-terminated strings | All string storage |
| `xar` | Dynamic arrays | AST children, token lists, errors |
| `tabula_dispersa` | Hash tables | Symbol tables, macro defs, captures |
| `credo` | Test assertions | Unit tests |

### File Structure

```
include/
  arbor_ce_lxxxix.h        # Main header (includes all)
  arbor_lexema.h           # Lexer
  arbor_praeparator.h      # Preprocessor
  arbor_syntaxis.h         # Parser
  arbor_typus.h            # Type resolver
  arbor_index.h            # Project index
  arbor_quaestio.h         # Query engine
  arbor_formator.h         # Formatter
  arbor_stml.h             # STML serialization

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
  fixa/
    exemplo_simplex.c
    exemplo_macro.c
    exemplo_functio.c
```

---

## Module 1: arbor_lexema (Lexer)

### Header: arbor_lexema.h

```c
#ifndef ARBOR_LEXEMA_H
#define ARBOR_LEXEMA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"

/* ==================================================
 * ARBOR LEXEMA - Lexer pro C89
 *
 * Tokenizat fontem C in fluxum lexematum.
 * Conservat trivia (spatia, commenta) pro roundtrip.
 *
 * PHILOSOPHIA:
 * - Omnia ex piscina allocata
 * - Trivia ante et post conservata
 * - Standard tracking (C89/C99/extension)
 * - 1-based linea/columna (ut editores)
 *
 * EXEMPLUM:
 *   ArborLexator* lex = arbor_lexator_creare(piscina, intern, fons, mensura);
 *   ArborLexema* tok;
 *   dum ((tok = arbor_lexema_proximum(lex))->genus != ARBOR_LEXEMA_EOF) {
 *       /* processare tok */
 *   }
 * ================================================== */

/* Standard values */
#define ARBOR_STANDARD_C89       0
#define ARBOR_STANDARD_C99       1
#define ARBOR_STANDARD_EXTENSION 2

nomen enumeratio {
    /* Keywords (C89) */
    ARBOR_LEXEMA_AUTO,
    ARBOR_LEXEMA_BREAK,
    ARBOR_LEXEMA_CASE,
    ARBOR_LEXEMA_CHAR,
    ARBOR_LEXEMA_CONST,
    ARBOR_LEXEMA_CONTINUE,
    ARBOR_LEXEMA_DEFAULT,
    ARBOR_LEXEMA_DO,
    ARBOR_LEXEMA_DOUBLE,
    ARBOR_LEXEMA_ELSE,
    ARBOR_LEXEMA_ENUM,
    ARBOR_LEXEMA_EXTERN,
    ARBOR_LEXEMA_FLOAT,
    ARBOR_LEXEMA_FOR,
    ARBOR_LEXEMA_GOTO,
    ARBOR_LEXEMA_IF,
    ARBOR_LEXEMA_INT,
    ARBOR_LEXEMA_LONG,
    ARBOR_LEXEMA_REGISTER,
    ARBOR_LEXEMA_RETURN,
    ARBOR_LEXEMA_SHORT,
    ARBOR_LEXEMA_SIGNED,
    ARBOR_LEXEMA_SIZEOF,
    ARBOR_LEXEMA_STATIC,
    ARBOR_LEXEMA_STRUCT,
    ARBOR_LEXEMA_SWITCH,
    ARBOR_LEXEMA_TYPEDEF,
    ARBOR_LEXEMA_UNION,
    ARBOR_LEXEMA_UNSIGNED,
    ARBOR_LEXEMA_VOID,
    ARBOR_LEXEMA_VOLATILE,
    ARBOR_LEXEMA_WHILE,

    /* Identifiers and literals */
    ARBOR_LEXEMA_IDENTIFICATOR,
    ARBOR_LEXEMA_INTEGER,
    ARBOR_LEXEMA_FLOAT_LIT,
    ARBOR_LEXEMA_CHAR_LIT,
    ARBOR_LEXEMA_STRING_LIT,

    /* Operators */
    ARBOR_LEXEMA_PLUS,           /* + */
    ARBOR_LEXEMA_MINUS,          /* - */
    ARBOR_LEXEMA_ASTERISCUS,     /* * */
    ARBOR_LEXEMA_SOLIDUS,        /* / */
    ARBOR_LEXEMA_PERCENTUM,      /* % */
    ARBOR_LEXEMA_AMPERSAND,      /* & */
    ARBOR_LEXEMA_PIPA,           /* | */
    ARBOR_LEXEMA_CARET,          /* ^ */
    ARBOR_LEXEMA_TILDE,          /* ~ */
    ARBOR_LEXEMA_EXCLAMATIO,     /* ! */
    ARBOR_LEXEMA_QUAESTIO,       /* ? */
    ARBOR_LEXEMA_DUPLUS,         /* ++ */
    ARBOR_LEXEMA_DUMINUS,        /* -- */
    ARBOR_LEXEMA_DUAMPERSAND,    /* && */
    ARBOR_LEXEMA_DUPIPA,         /* || */
    ARBOR_LEXEMA_SINISTRUM,      /* << */
    ARBOR_LEXEMA_DEXTRUM,        /* >> */
    ARBOR_LEXEMA_AEQUALIS,       /* == */
    ARBOR_LEXEMA_NON_AEQUALIS,   /* != */
    ARBOR_LEXEMA_MINOR,          /* < */
    ARBOR_LEXEMA_MAIOR,          /* > */
    ARBOR_LEXEMA_MINOR_AEQ,      /* <= */
    ARBOR_LEXEMA_MAIOR_AEQ,      /* >= */
    ARBOR_LEXEMA_ASSIGNATIO,     /* = */
    ARBOR_LEXEMA_PLUS_ASSIGN,    /* += */
    ARBOR_LEXEMA_MINUS_ASSIGN,   /* -= */
    ARBOR_LEXEMA_MULT_ASSIGN,    /* *= */
    ARBOR_LEXEMA_DIV_ASSIGN,     /* /= */
    ARBOR_LEXEMA_MOD_ASSIGN,     /* %= */
    ARBOR_LEXEMA_AND_ASSIGN,     /* &= */
    ARBOR_LEXEMA_OR_ASSIGN,      /* |= */
    ARBOR_LEXEMA_XOR_ASSIGN,     /* ^= */
    ARBOR_LEXEMA_SHL_ASSIGN,     /* <<= */
    ARBOR_LEXEMA_SHR_ASSIGN,     /* >>= */
    ARBOR_LEXEMA_PUNCTUM,        /* . */
    ARBOR_LEXEMA_SAGITTA,        /* -> */
    ARBOR_LEXEMA_COMMA,          /* , */
    ARBOR_LEXEMA_COLON,          /* : */
    ARBOR_LEXEMA_SEMICOLON,      /* ; */

    /* Delimiters */
    ARBOR_LEXEMA_PAREN_APERTA,   /* ( */
    ARBOR_LEXEMA_PAREN_CLAUSA,   /* ) */
    ARBOR_LEXEMA_BRACKET_APERTA, /* [ */
    ARBOR_LEXEMA_BRACKET_CLAUSA, /* ] */
    ARBOR_LEXEMA_BRACE_APERTA,   /* { */
    ARBOR_LEXEMA_BRACE_CLAUSA,   /* } */

    /* Preprocessor */
    ARBOR_LEXEMA_HASH,           /* # */
    ARBOR_LEXEMA_HASH_HASH,      /* ## */

    /* Special */
    ARBOR_LEXEMA_EOF,
    ARBOR_LEXEMA_ERROR,

    /* C99 (flagged) */
    ARBOR_LEXEMA_C99_COMMENT     /* // style */
} ArborLexemaGenus;

/* Trivia (whitespace or comment) */
nomen structura {
    chorda      valor;           /* Actual text */
    b32         est_commentum;   /* true if comment, false if whitespace */
    b32         est_c99;         /* true if // comment */
    i32         linea;
    i32         columna;
} ArborTrivia;

/* Token */
nomen structura {
    ArborLexemaGenus  genus;
    chorda            valor;       /* Text of identifier/literal */
    i32               byte_offset; /* 0-based offset in source */
    i32               longitudo;   /* Length in bytes */
    i32               linea;       /* 1-based */
    i32               columna;     /* 1-based */
    Xar*              trivia_ante; /* Xar of ArborTrivia */
    Xar*              trivia_post; /* Xar of ArborTrivia */
    i8                standard;    /* ARBOR_STANDARD_* */
} ArborLexema;

/* Lexer state (opaque) */
nomen structura ArborLexator ArborLexator;

/* ==================================================
 * API
 * ================================================== */

/* Creare lexator */
ArborLexator* arbor_lexator_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   fons,
    i32                   mensura);

/* Obtinere proximum lexema */
ArborLexema* arbor_lexema_proximum(ArborLexator* lex);

/* Prospicere sine consumendo */
ArborLexema* arbor_lexema_prospicere(ArborLexator* lex, i32 ahead);

/* Obtinere omnia lexemata (pro preprocessor) */
Xar* arbor_lexema_omnia(ArborLexator* lex);

/* Obtinere nomen genus (pro debugging) */
constans character* arbor_lexema_genus_nomen(ArborLexemaGenus genus);

#endif /* ARBOR_LEXEMA_H */
```

---

## Module 2: arbor_praeparator (Preprocessor)

### Header: arbor_praeparator.h

```c
#ifndef ARBOR_PRAEPARATOR_H
#define ARBOR_PRAEPARATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_lexema.h"

/* ==================================================
 * ARBOR PRAEPARATOR - C Preprocessor
 *
 * Tractat directivas (#include, #define, #if, etc.)
 * Expandit macros cum tracking originis.
 *
 * PHILOSOPHIA:
 * - Origo tracking pro source mapping
 * - Nested expansion chains conservatae
 * - Separatus a parser (two-pass)
 * ================================================== */

/* Macro expansion origin */
nomen structura ArborOrigo {
    chorda*                  nomen_macro;   /* Which macro expanded */
    i32                      expansio_index; /* Nesting level (0=original) */
    i32                      linea_originalis;
    i32                      columna_originalis;
    structura ArborOrigo*    pater;         /* Parent expansion */
} ArborOrigo;

/* Token with origin tracking */
nomen structura {
    ArborLexema*    lexema;
    ArborOrigo*     origo;    /* NULL if not from macro */
} ArborLexemaOrigo;

/* Directive types */
nomen enumeratio {
    ARBOR_DIRECTIVA_INCLUDE,
    ARBOR_DIRECTIVA_DEFINE,
    ARBOR_DIRECTIVA_UNDEF,
    ARBOR_DIRECTIVA_IF,
    ARBOR_DIRECTIVA_IFDEF,
    ARBOR_DIRECTIVA_IFNDEF,
    ARBOR_DIRECTIVA_ELIF,
    ARBOR_DIRECTIVA_ELSE,
    ARBOR_DIRECTIVA_ENDIF,
    ARBOR_DIRECTIVA_LINE,
    ARBOR_DIRECTIVA_ERROR,
    ARBOR_DIRECTIVA_PRAGMA,
    ARBOR_DIRECTIVA_UNKNOWN
} ArborDirectivaGenus;

/* Macro definition */
nomen structura {
    chorda*         nomen;
    b32             est_functio;   /* Function-like macro */
    Xar*            parametra;     /* Xar of chorda* (param names) */
    Xar*            corpus;        /* Xar of ArborLexema* (body tokens) */
    chorda          via_file;      /* Where defined */
    i32             linea_def;
} ArborMacroDefinitio;

/* Preprocessor state (opaque) */
nomen structura ArborPraeparator ArborPraeparator;

/* ==================================================
 * API
 * ================================================== */

/* Creare praeparator */
ArborPraeparator* arbor_praeparator_creare(
    Piscina*             piscina,
    InternamentumChorda* intern);

/* Addere via include */
vacuum arbor_praeparator_addere_via(
    ArborPraeparator*    pp,
    constans character*  via);

/* Definire macro (e.g., from command line -D) */
vacuum arbor_praeparator_definire(
    ArborPraeparator*    pp,
    constans character*  nomen,
    constans character*  valor);

/* Processare file, return expanded tokens with origins */
Xar* arbor_praeparator_processare(
    ArborPraeparator*    pp,
    constans character*  via_file);

/* Processare lexemata jam habita */
Xar* arbor_praeparator_processare_lexemata(
    ArborPraeparator*    pp,
    Xar*                 lexemata,
    constans character*  via_file);

/* Obtinere macro definition (for hover info) */
ArborMacroDefinitio* arbor_praeparator_obtinere_macro(
    ArborPraeparator*    pp,
    chorda*              nomen);

#endif /* ARBOR_PRAEPARATOR_H */
```

---

## Module 3: arbor_syntaxis (Parser)

### Header: arbor_syntaxis.h

```c
#ifndef ARBOR_SYNTAXIS_H
#define ARBOR_SYNTAXIS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "arbor_lexema.h"
#include "arbor_praeparator.h"

/* ==================================================
 * ARBOR SYNTAXIS - C89 Parser
 *
 * Aedificat AST ex fluxu lexematum.
 * Pratt parser pro expressionibus.
 * Recursive descent pro sententiis.
 *
 * PHILOSOPHIA:
 * - Nodus uniformis cum unio pro variantes
 * - Xar* liberi pro uniformitate
 * - Trivia conservata pro roundtrip
 * - ERROR nodes pro error recovery
 * - Origo tracking ex praeparator
 * ================================================== */

/* Node types (lowercase-kebab-case for CSS selectors) */
nomen enumeratio {
    /* Translation unit */
    ARBOR_NODUS_TRANSLATION_UNIT,

    /* Declarations */
    ARBOR_NODUS_FUNCTION_DEFINITION,
    ARBOR_NODUS_DECLARATION,
    ARBOR_NODUS_PARAMETER_DECLARATION,
    ARBOR_NODUS_STRUCT_SPECIFIER,
    ARBOR_NODUS_UNION_SPECIFIER,
    ARBOR_NODUS_ENUM_SPECIFIER,
    ARBOR_NODUS_ENUMERATOR,
    ARBOR_NODUS_TYPEDEF_NAME,

    /* Declarators */
    ARBOR_NODUS_DECLARATOR,
    ARBOR_NODUS_POINTER,
    ARBOR_NODUS_ARRAY_DECLARATOR,
    ARBOR_NODUS_FUNCTION_DECLARATOR,
    ARBOR_NODUS_PARAMETER_LIST,
    ARBOR_NODUS_INIT_DECLARATOR,

    /* Type specifiers */
    ARBOR_NODUS_TYPE_SPECIFIER,
    ARBOR_NODUS_TYPE_QUALIFIER,
    ARBOR_NODUS_STORAGE_CLASS,

    /* Statements */
    ARBOR_NODUS_COMPOUND_STATEMENT,
    ARBOR_NODUS_IF_STATEMENT,
    ARBOR_NODUS_SWITCH_STATEMENT,
    ARBOR_NODUS_CASE_LABEL,
    ARBOR_NODUS_DEFAULT_LABEL,
    ARBOR_NODUS_WHILE_STATEMENT,
    ARBOR_NODUS_DO_STATEMENT,
    ARBOR_NODUS_FOR_STATEMENT,
    ARBOR_NODUS_GOTO_STATEMENT,
    ARBOR_NODUS_CONTINUE_STATEMENT,
    ARBOR_NODUS_BREAK_STATEMENT,
    ARBOR_NODUS_RETURN_STATEMENT,
    ARBOR_NODUS_LABELED_STATEMENT,
    ARBOR_NODUS_EXPRESSION_STATEMENT,

    /* Expressions */
    ARBOR_NODUS_BINARY_EXPRESSION,
    ARBOR_NODUS_UNARY_EXPRESSION,
    ARBOR_NODUS_POSTFIX_EXPRESSION,
    ARBOR_NODUS_CALL_EXPRESSION,
    ARBOR_NODUS_MEMBER_EXPRESSION,
    ARBOR_NODUS_SUBSCRIPT_EXPRESSION,
    ARBOR_NODUS_CAST_EXPRESSION,
    ARBOR_NODUS_SIZEOF_EXPRESSION,
    ARBOR_NODUS_CONDITIONAL_EXPRESSION,
    ARBOR_NODUS_ASSIGNMENT_EXPRESSION,
    ARBOR_NODUS_COMMA_EXPRESSION,

    /* Primary */
    ARBOR_NODUS_IDENTIFIER,
    ARBOR_NODUS_INTEGER_LITERAL,
    ARBOR_NODUS_FLOAT_LITERAL,
    ARBOR_NODUS_CHAR_LITERAL,
    ARBOR_NODUS_STRING_LITERAL,

    /* Initializers */
    ARBOR_NODUS_INITIALIZER,
    ARBOR_NODUS_INITIALIZER_LIST,
    ARBOR_NODUS_DESIGNATION,      /* C99 designated initializers */

    /* Special */
    ARBOR_NODUS_ERROR,            /* Error recovery node */
    ARBOR_NODUS_EXTENSION         /* Opaque __attribute__ etc */
} ArborNodusGenus;

/* Forward declaration */
nomen structura ArborNodus ArborNodus;

/* AST Node (following sputnik pattern with union) */
structura ArborNodus {
    ArborNodusGenus    genus;

    /* Location span */
    i32                byte_initium;
    i32                byte_finis;
    i32                linea_initium;
    i32                columna_initium;
    i32                linea_finis;
    i32                columna_finis;

    /* Trivia for roundtrip */
    Xar*               trivia_ante;   /* Xar of ArborTrivia */
    Xar*               trivia_post;

    /* Standard tracking */
    i8                 standard;      /* ARBOR_STANDARD_* */

    /* Macro provenance (NULL if from original source, not macro-expanded) */
    ArborOrigo*        origo;

    /* Resolved type (populated by arbor_typus, NULL until then) */
    structura ArborTypus*  typus_resolutum;

    /* Parent for navigation */
    ArborNodus*        pater;

    /* Type-specific data */
    unio {
        /* function-definition */
        structura {
            ArborNodus*   specifiers;    /* Type specifiers */
            ArborNodus*   declarator;
            Xar*          old_params;    /* K&R style param decls */
            ArborNodus*   corpus;        /* compound-statement */
        } functio;

        /* declaration */
        structura {
            Xar*          specifiers;    /* Type/storage specifiers */
            Xar*          declaratores;  /* init-declarator list */
        } declaratio;

        /* struct/union-specifier */
        structura {
            chorda*       nomen;         /* Tag name (may be NULL) */
            Xar*          membra;        /* Member declarations */
        } aggregatum;

        /* enum-specifier */
        structura {
            chorda*       nomen;
            Xar*          enumeratores;
        } enum_spec;

        /* if-statement */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   consequens;
            ArborNodus*   alternans;     /* else branch, may be NULL */
        } si;

        /* switch-statement */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   corpus;
        } commutatio;

        /* while/do-statement */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   corpus;
        } iteratio;

        /* for-statement */
        structura {
            ArborNodus*   init;          /* May be NULL */
            ArborNodus*   conditio;      /* May be NULL */
            ArborNodus*   post;          /* May be NULL */
            ArborNodus*   corpus;
        } per;

        /* binary-expression */
        structura {
            ArborNodus*       sinister;
            ArborLexemaGenus  operator;
            ArborNodus*       dexter;
        } binarium;

        /* unary-expression */
        structura {
            ArborLexemaGenus  operator;
            ArborNodus*       operandum;
        } unarium;

        /* call-expression */
        structura {
            ArborNodus*   functio;
            Xar*          argumenta;
        } vocatio;

        /* member-expression */
        structura {
            ArborNodus*       objectum;
            ArborLexemaGenus  operator;  /* . or -> */
            chorda*           membrum;
        } membrum;

        /* subscript-expression */
        structura {
            ArborNodus*   array;
            ArborNodus*   index;
        } subscriptum;

        /* cast-expression */
        structura {
            ArborNodus*   typus;
            ArborNodus*   expressio;
        } conversio;

        /* conditional-expression */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   verum;
            ArborNodus*   falsum;
        } ternarium;

        /* compound-statement */
        structura {
            Xar*          sententiae;    /* statements + declarations */
        } compositum;

        /* return-statement */
        structura {
            ArborNodus*   valor;         /* May be NULL */
        } reditio;

        /* goto-statement */
        structura {
            chorda*       label;
        } saltus;

        /* labeled-statement */
        structura {
            chorda*       label;
            ArborNodus*   sententia;
        } titulatum;

        /* case-label */
        structura {
            ArborNodus*   valor;
            ArborNodus*   sententia;
        } casus;

        /* identifier, literals */
        structura {
            chorda*       valor;         /* Interned for identifiers */
        } folium;

        /* integer literal with value */
        structura {
            chorda*       textus;
            i64           valor;
            b32           est_unsigned;
            b32           est_long;
        } integer;

        /* float literal with value */
        structura {
            chorda*       textus;
            f64           valor;
        } floating;

        /* error node */
        structura {
            chorda        nuntius;
            Xar*          lexemata_praetermissa;  /* Skipped tokens */
        } error;

        /* generic children (for nodes with uniform child lists) */
        structura {
            Xar*          liberi;        /* Xar of ArborNodus* */
        } genericum;

    } datum;
};

/* Error info */
nomen enumeratio {
    ARBOR_ERROR_LEXEMA,
    ARBOR_ERROR_PRAEPARATOR,
    ARBOR_ERROR_SYNTAXIS,
    ARBOR_ERROR_TYPUS,
    ARBOR_ERROR_LINT
} ArborErrorGenus;

nomen enumeratio {
    ARBOR_SEVERITAS_ERROR,
    ARBOR_SEVERITAS_WARNING,
    ARBOR_SEVERITAS_INFO
} ArborSeveritas;

nomen structura {
    ArborErrorGenus     genus;
    ArborSeveritas      severitas;
    chorda              nuntius;
    chorda              via_file;
    i32                 linea;
    i32                 columna;
    i32                 longitudo;
} ArborError;

/* Parse result (following sputnik pattern) */
nomen structura {
    b32                 successus;
    ArborNodus*         radix;
    Xar*                errores;       /* Xar of ArborError */
    ArborErrorGenus     primus_status;
} ArborSyntaxisResultus;

/* Parser state (opaque) */
nomen structura ArborSyntaxis ArborSyntaxis;

/* Visitor callbacks (optional) */
nomen structura {
    vacuum (*ante_nodus)(ArborNodus* n, vacuum* ctx);
    vacuum (*post_nodus)(ArborNodus* n, vacuum* ctx);
    vacuum* datum;
} ArborVisitor;

/* ==================================================
 * API
 * ================================================== */

/* Creare parser */
ArborSyntaxis* arbor_syntaxis_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Parsere lexemata ad AST */
ArborSyntaxisResultus arbor_syntaxis_parsere(
    ArborSyntaxis*        syn,
    Xar*                  lexemata);

/* Parsere cum visitor callbacks */
ArborSyntaxisResultus arbor_syntaxis_parsere_cum_visitore(
    ArborSyntaxis*        syn,
    Xar*                  lexemata,
    ArborVisitor*         visitor);

/* Obtinere nomen genus (for CSS selectors) */
constans character* arbor_nodus_genus_nomen(ArborNodusGenus genus);

/* Format error for display */
chorda arbor_error_formare(Piscina* piscina, ArborError* err);

#endif /* ARBOR_SYNTAXIS_H */
```

---

## Module 4: arbor_typus (Type Resolver)

### Header: arbor_typus.h

```c
#ifndef ARBOR_TYPUS_H
#define ARBOR_TYPUS_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_syntaxis.h"

/* ==================================================
 * ARBOR TYPUS - Type Resolution
 *
 * Resolvit references typorum, aedificat graphum typorum.
 * Separatus passus post parsing.
 * ================================================== */

nomen enumeratio {
    ARBOR_TYPUS_VOID,
    ARBOR_TYPUS_CHAR,
    ARBOR_TYPUS_SHORT,
    ARBOR_TYPUS_INT,
    ARBOR_TYPUS_LONG,
    ARBOR_TYPUS_FLOAT,
    ARBOR_TYPUS_DOUBLE,
    ARBOR_TYPUS_STRUCT,
    ARBOR_TYPUS_UNION,
    ARBOR_TYPUS_ENUM,
    ARBOR_TYPUS_POINTER,
    ARBOR_TYPUS_ARRAY,
    ARBOR_TYPUS_FUNCTION,
    ARBOR_TYPUS_TYPEDEF
} ArborTypusGenus;

nomen structura ArborTypus ArborTypus;

structura ArborTypus {
    ArborTypusGenus    genus;
    chorda*            nomen;           /* For struct/union/enum/typedef */
    ArborTypus*        basis;           /* For pointer/array/typedef */
    i32                array_mensura;   /* -1 for unsized */
    Xar*               parametra;       /* Function param types */
    ArborTypus*        reditum;         /* Function return type */
    b32                est_const;
    b32                est_volatile;
    b32                est_signed;
    b32                est_unsigned;
};

/* Type resolver state (opaque) */
nomen structura ArborTypusResolver ArborTypusResolver;

/* ==================================================
 * API
 * ================================================== */

/* Creare type resolver */
ArborTypusResolver* arbor_typus_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Resolvere typos in AST (modifies nodes) */
vacuum arbor_typus_resolvere(
    ArborTypusResolver*   res,
    ArborNodus*           radix);

/* Obtinere typum expressionis */
ArborTypus* arbor_typus_expressionis(
    ArborTypusResolver*   res,
    ArborNodus*           expr);

/* Obtinere declarationem identificatoris */
ArborNodus* arbor_typus_declaratio(
    ArborTypusResolver*   res,
    chorda*               nomen);

#endif /* ARBOR_TYPUS_H */
```

---

## Module 5: arbor_index (Project Index)

### Header: arbor_index.h

```c
#ifndef ARBOR_INDEX_H
#define ARBOR_INDEX_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_syntaxis.h"
#include "arbor_typus.h"

/* ==================================================
 * ARBOR INDEX - Project-Wide Symbol Index
 *
 * Cross-file symbol resolution.
 * Go-to-definition, find-references.
 * ================================================== */

/* File in project */
nomen structura {
    chorda          via;            /* Relative path */
    chorda          fons;           /* Source content */
    ArborNodus*     radix;          /* Parsed AST */
    i64             tempus_mutatum;
} ArborFilum;

/* Symbol types */
nomen enumeratio {
    ARBOR_SYMBOLUM_FUNCTIO,
    ARBOR_SYMBOLUM_VARIABILIS,
    ARBOR_SYMBOLUM_TYPUS,
    ARBOR_SYMBOLUM_MACRO,
    ARBOR_SYMBOLUM_ENUM_VALOR
} ArborSymbolumGenus;

/* Linkage */
nomen enumeratio {
    ARBOR_NEXUS_EXTERNUS,     /* extern */
    ARBOR_NEXUS_INTERNUS,     /* static */
    ARBOR_NEXUS_NULLUS        /* local/parameter */
} ArborNexus;

/* Symbol entry */
nomen structura {
    chorda*             nomen;
    ArborSymbolumGenus  genus;
    ArborNexus          nexus;
    ArborFilum*         filum;
    ArborNodus*         declaratio;
    ArborNodus*         definitio;     /* May differ from declaration */
    Xar*                referentiae;   /* Usage sites */
} ArborSymbolum;

/* Reference site */
nomen structura {
    ArborFilum*         filum;
    i32                 linea;
    i32                 columna;
    ArborNodus*         nodus;
} ArborReferentia;

/* Project state (opaque) */
nomen structura ArborProiectum ArborProiectum;

/* ==================================================
 * API
 * ================================================== */

/* Creare project */
ArborProiectum* arbor_index_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* Addere file to project */
vacuum arbor_index_addere_filum(
    ArborProiectum*       proj,
    constans character*   via);

/* Addere folder (with glob pattern) */
vacuum arbor_index_addere_folder(
    ArborProiectum*       proj,
    constans character*   via,
    constans character*   pattern);  /* e.g., "*.c" */

/* Addere include path */
vacuum arbor_index_addere_via_include(
    ArborProiectum*       proj,
    constans character*   via);

/* Build/rebuild index */
vacuum arbor_index_aedificare(ArborProiectum* proj);

/* Query: find symbol by name */
ArborSymbolum* arbor_index_invenire(
    ArborProiectum*       proj,
    chorda*               nomen);

/* Query: find all references */
Xar* arbor_index_referentiae(
    ArborProiectum*       proj,
    chorda*               nomen);

/* Query: go-to-definition at location */
ArborNodus* arbor_index_definitio_ad_punctum(
    ArborProiectum*       proj,
    constans character*   via,
    i32                   linea,
    i32                   columna);

/* Query: find symbol at location */
ArborSymbolum* arbor_index_symbolum_ad_punctum(
    ArborProiectum*       proj,
    constans character*   via,
    i32                   linea,
    i32                   columna);

#endif /* ARBOR_INDEX_H */
```

---

## Module 6: arbor_quaestio (Query Engine)

### Header: arbor_quaestio.h

```c
#ifndef ARBOR_QUAESTIO_H
#define ARBOR_QUAESTIO_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "arbor_syntaxis.h"

/* ==================================================
 * ARBOR QUAESTIO - CSS-Style AST Query Engine
 *
 * Quaerit nodos AST cum selectoribus CSS-similis.
 * Supportat captures ($name), pseudo-selectors.
 *
 * SYNTAXIS:
 *   function-definition                  # All functions
 *   identifier[value="main"]             # Identifier named main
 *   function-definition > compound-statement > if-statement
 *   :calls(printf)                       # Calls to printf
 *   function-definition > identifier$fn  # Capture fn name
 * ================================================== */

/* Query result */
nomen structura {
    ArborNodus*         nodus;
    TabulaDispersa*     capturae;     /* name -> ArborNodus* */
} ArborQuaestioResultatum;

/* Compiled query (opaque) */
nomen structura ArborQuaestio ArborQuaestio;

/* Custom pseudo-selector function */
nomen b32 (*ArborPseudoFunctio)(ArborNodus* n, vacuum* datum);

/* ==================================================
 * API
 * ================================================== */

/* Compilare selector string to query */
ArborQuaestio* arbor_quaestio_compilare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    constans character*   selector);

/* Exsequi query on AST */
Xar* arbor_quaestio_exsequi(
    ArborQuaestio*        q,
    ArborNodus*           radix);

/* Check if single node matches */
b32 arbor_quaestio_congruit(
    ArborQuaestio*        q,
    ArborNodus*           nodus);

/* Registrare custom pseudo-selector */
vacuum arbor_quaestio_registrare_pseudo(
    ArborQuaestio*        q,
    constans character*   nomen,
    ArborPseudoFunctio    functio,
    vacuum*               datum);

/* Built-in semantic pseudo-selectors:
 *   :calls(name)      - Function calls
 *   :defines(name)    - Defines symbol
 *   :uses(name)       - Uses symbol
 *   :returns          - Has return statement
 *   :c89              - C89-only construct
 *   :c99              - C99-only construct
 *   :extension        - Compiler extension
 */

#endif /* ARBOR_QUAESTIO_H */
```

---

## Module 7: arbor_formator (Formatter)

### Header: arbor_formator.h

```c
#ifndef ARBOR_FORMATOR_H
#define ARBOR_FORMATOR_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "arbor_syntaxis.h"

/* ==================================================
 * ARBOR FORMATOR - Code Formatter
 *
 * Pretty-prints AST back to source code.
 * Fran's style: Allman braces, 4-space indent.
 *
 * MODI:
 * - fidelis: Preserve original trivia for roundtrip
 * - formatus: Apply consistent formatting rules
 * ================================================== */

/* ==================================================
 * API
 * ================================================== */

/* Format AST to string (with formatting rules) */
chorda arbor_formator_emittere(
    Piscina*              piscina,
    ArborNodus*           radix);

/* Format with roundtrip preservation (keep original trivia) */
chorda arbor_formator_emittere_fidelis(
    Piscina*              piscina,
    ArborNodus*           radix);

/* Format single node/subtree */
chorda arbor_formator_emittere_nodum(
    Piscina*              piscina,
    ArborNodus*           nodus,
    b32                   fidelis);

#endif /* ARBOR_FORMATOR_H */
```

---

## Module 8: arbor_stml (STML Serialization)

### Header: arbor_stml.h

```c
#ifndef ARBOR_STML_H
#define ARBOR_STML_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "arbor_syntaxis.h"
#include "stml.h"

/* ==================================================
 * ARBOR STML - STML Serialization
 *
 * Serializes AST to/from STML format.
 * Enables caching, interchange, transformations.
 *
 * EXEMPLUM OUTPUT:
 *   <function-definition linea="42" standard="c89">
 *     <type-specifier>int</type-specifier>
 *     <declarator>
 *       <identifier>main</identifier>
 *       ...
 *     </declarator>
 *     ...
 *   </function-definition>
 * ================================================== */

/* ==================================================
 * API
 * ================================================== */

/* Serialize AST to STML */
StmlNodus* arbor_stml_serialisare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborNodus*           radix,
    b32                   includere_trivia);

/* Serialize to STML string */
chorda arbor_stml_ad_chordam(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborNodus*           radix,
    b32                   includere_trivia);

/* Parse STML back to AST */
ArborNodus* arbor_stml_deserialisare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    StmlNodus*            stml);

/* Apply transformation DSL */
ArborNodus* arbor_stml_transformare(
    Piscina*              piscina,
    InternamentumChorda*  intern,
    ArborNodus*           radix,
    chorda                transformatio);

/* ==================================================
 * Transformation DSL Operations (Minimal Spec)
 *
 * Transform rules use CSS selector to match, then apply operations:
 *
 *   <%replace>...</%replace>
 *       Replace matched node with new content
 *
 *   <%append>...</%append>
 *       Append content after matched node's children
 *
 *   <%prepend>...</%prepend>
 *       Prepend content before matched node's children
 *
 *   <%remove />
 *       Delete matched node entirely
 *
 *   <%wrap>...</%wrap>
 *       Wrap matched node in new parent element
 *
 *   <%unwrap />
 *       Replace node with its children (remove wrapper)
 *
 * Special references within operations:
 *   <%this />        - The matched node itself
 *   <%children />    - Children of matched node
 *   <%capture name="$foo" />  - Previously captured node
 *
 * Example:
 *   <transform selector="if-statement[condition/identifier='VERUM']">
 *     <%replace>
 *       <compound-statement><%children /></compound-statement>
 *     </%replace>
 *   </transform>
 *
 * Full grammar deferred to implementation phase.
 * ================================================== */

#endif /* ARBOR_STML_H */
```

---

## Semantic Tokens for Highlighting

```c
/* In arbor_syntaxis.h or separate header */

nomen enumeratio {
    ARBOR_TOKEN_KEYWORD,
    ARBOR_TOKEN_TYPE,
    ARBOR_TOKEN_FUNCTION,
    ARBOR_TOKEN_VARIABLE,
    ARBOR_TOKEN_PARAMETER,
    ARBOR_TOKEN_MACRO,
    ARBOR_TOKEN_STRING,
    ARBOR_TOKEN_NUMBER,
    ARBOR_TOKEN_OPERATOR,
    ARBOR_TOKEN_COMMENT,
    ARBOR_TOKEN_PREPROCESSOR
} ArborTokenSemanticum;

nomen structura {
    i32                    linea;
    i32                    columna;
    i32                    longitudo;
    ArborTokenSemanticum   genus;
} ArborTokenColoratio;

/* Generate semantic tokens for highlighting */
Xar* arbor_semantic_tokens(
    Piscina*              piscina,
    ArborNodus*           radix,
    ArborTypusResolver*   types);  /* Optional, for semantic info */
```

---

## Implementation Phases

### Phase 1: arbor_lexema
- All C89 tokens + C99 // comments
- Location tracking (byte offset, line, column)
- Trivia capture (leading + trailing)
- Standard tagging
- Unit tests: `probatio_arbor_lexema.c`

### Phase 2: arbor_praeparator
- #define (object-like and function-like macros)
- #include with path resolution
- #if/#ifdef/#ifndef/#elif/#else/#endif
- Macro expansion with Origo tracking
- Unit tests: `probatio_arbor_praeparator.c`

### Phase 3: arbor_syntaxis (Core)
- Pratt parser for expressions
- Statements (if, while, for, switch, etc.)
- Function definitions
- Error recovery with ERROR nodes
- Unit tests: `probatio_arbor_syntaxis.c`

### Phase 4: arbor_syntaxis (Complete)
- Complex declarators
- Struct/union/enum
- Typedef handling
- Extension nodes
- Roundtrip tests

### Phase 5: arbor_typus
- Primitive types
- Struct/union members
- Typedef resolution
- Scope tracking

### Phase 6: arbor_index
- Single-file symbol table
- Cross-file resolution
- Go-to-definition
- Find-references

### Phase 7: arbor_quaestio
- Selector parser
- Structural pseudo-selectors
- Semantic pseudo-selectors
- Capture groups

### Phase 8: arbor_formator
- AST to source emission
- Style rules
- Roundtrip fidelity mode

### Phase 9: arbor_stml
- Serialization
- Deserialization
- Transformation DSL

---

## Testing Pattern (following credo.h)

```c
/* probatio_arbor_lexema.c */
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "arbor_lexema.h"

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("test_lexema", M);
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* Test: keywords */
    imprimere("\n--- Keywords ---\n");
    {
        ArborLexator* lex;
        ArborLexema* tok;

        lex = arbor_lexator_creare(piscina, intern, "int x;", VI);
        tok = arbor_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
        CREDO_AEQUALIS_I32(tok->linea, I);
        CREDO_AEQUALIS_I32(tok->columna, I);
    }

    /* Test: trivia preservation */
    imprimere("\n--- Trivia ---\n");
    {
        ArborLexator* lex;
        ArborLexema* tok;

        lex = arbor_lexator_creare(piscina, intern, "/* comment */ int", XVII);
        tok = arbor_lexema_proximum(lex);
        CREDO_AEQUALIS_I32(tok->genus, ARBOR_LEXEMA_INT);
        CREDO_NON_NIHIL(tok->trivia_ante);
        CREDO_AEQUALIS_I32(xar_numerus(tok->trivia_ante), I);
    }

    credo_imprimere_compendium();
    piscina_destruere(piscina);

    redde credo_falsa() > ZEPHYRUM ? I : ZEPHYRUM;
}
```

### Roundtrip Fidelity Test Pattern

Critical test: `parse → format_fidelis → byte compare == original`

```c
/* probatio_arbor_roundtrip.c */
hic_manens vacuum
_test_roundtrip(
    Piscina* piscina,
    InternamentumChorda* intern,
    constans character* fons,
    i32 mensura)
{
    ArborLexator* lex;
    ArborPraeparator* pp;
    ArborSyntaxis* syn;
    Xar* lexemata;
    Xar* expandita;
    ArborSyntaxisResultus res;
    chorda emissa;

    /* Parse */
    lex = arbor_lexator_creare(piscina, intern, fons, mensura);
    lexemata = arbor_lexema_omnia(lex);

    pp = arbor_praeparator_creare(piscina, intern);
    expandita = arbor_praeparator_processare_lexemata(pp, lexemata, "test.c");

    syn = arbor_syntaxis_creare(piscina, intern);
    res = arbor_syntaxis_parsere(syn, expandita);

    CREDO_VERUM(res.successus);

    /* Format with fidelity (preserve trivia) */
    emissa = arbor_formator_emittere_fidelis(piscina, res.radix);

    /* Byte-exact comparison */
    CREDO_AEQUALIS_I32((i32)emissa.mensura, mensura);
    CREDO_VERUM(memcmp(emissa.datum, fons, (size_t)mensura) == ZEPHYRUM);
}

/* Test cases */
imprimere("\n--- Roundtrip: simple function ---\n");
{
    constans character* fons = "int main(void)\n{\n    return 0;\n}\n";
    _test_roundtrip(piscina, intern, fons, (i32)strlen(fons));
}

imprimere("\n--- Roundtrip: comments preserved ---\n");
{
    constans character* fons = "/* header */\nint x; /* trailing */\n";
    _test_roundtrip(piscina, intern, fons, (i32)strlen(fons));
}
```

---

## Summary of Changes from v1

1. **Codebase-aligned data structures**: Using patterns from sputnik_parser, stml
2. **Explicit dependencies**: piscina, internamentum, chorda, xar, tabula_dispersa
3. **Header format**: Following existing PHILOSOPHIA/EXEMPLUM comment style
4. **Node structure**: Tagged union following sputnik pattern but with richer variants
5. **Error handling**: ArborError struct matching SputnikParserResultus pattern
6. **Testing**: Following credo.h assertion patterns
7. **File organization**: Matching existing include/lib/probationes structure
8. **API naming**: Consistent Latin verb-based function names

## Clarifications (v2.1)

1. **Hybrid declarator handling**: AST mirrors raw C declarator syntax (inside-out). The `typus_resolutum` field on each node is populated by `arbor_typus` pass with normalized `ArborTypus*`.
2. **Source text**: Byte spans (`byte_initium`/`byte_finis`) are sufficient. Slice from original source buffer when needed - no duplication.
3. **Origo tracking**: `ArborOrigo*` on nodes. NULL means original source (not macro-expanded). Non-NULL points to expansion chain.
4. **Transformation DSL**: Minimal operations spec added (<%replace>, <%append>, <%prepend>, <%remove>, <%wrap>, <%unwrap>). Full grammar deferred.
5. **Roundtrip fidelity test**: Explicit test pattern added showing `parse → format_fidelis → byte compare`.
