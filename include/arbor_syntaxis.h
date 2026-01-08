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
 *
 * EXEMPLUM:
 *   ArborSyntaxis* syn = arbor_syntaxis_creare(piscina, intern);
 *   ArborSyntaxisResultus res = arbor_syntaxis_parsere(syn, lexemata);
 *   si (res.successus) {
 *       arbor_nodus_imprimere(res.radix, ZEPHYRUM);
 *   }
 * ================================================== */

/* ==================================================
 * Node Types (genus)
 * ================================================== */

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

    /* Special */
    ARBOR_NODUS_ERROR,
    ARBOR_NODUS_EXTENSION
} ArborNodusGenus;

/* ==================================================
 * Error Types
 * ================================================== */

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

/* ==================================================
 * AST Node
 *
 * Uniform structure for all node types.
 * Type-specific data stored in union.
 * ================================================== */

nomen structura ArborNodus ArborNodus;

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

    /* Macro provenance (NULL if from original source) */
    ArborOrigo*        origo;

    /* Parent for navigation */
    ArborNodus*        pater;

    /* Resolved type (populated by arbor_typus, NULL until then) */
    structura ArborTypus*  typus_resolutum;

    /* Type-specific data */
    unio {
        /* function-definition */
        structura {
            Xar*          specifiers;    /* Xar of ArborNodus* (type/storage) */
            ArborNodus*   declarator;
            Xar*          old_params;    /* K&R style param decls */
            ArborNodus*   corpus;        /* compound-statement */
        } functio;

        /* declaration */
        structura {
            Xar*          specifiers;    /* Xar of ArborNodus* */
            Xar*          declaratores;  /* Xar of ArborNodus* (init-declarators) */
        } declaratio;

        /* struct/union-specifier */
        structura {
            chorda*       titulus;       /* Tag name (may be NULL) */
            Xar*          membra;        /* Member declarations */
        } aggregatum;

        /* enum-specifier */
        structura {
            chorda*       titulus;
            Xar*          enumeratores;
        } enum_spec;

        /* if-statement */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   consequens;
            ArborNodus*   alternans;     /* else branch, may be NULL */
        } conditionale;

        /* switch-statement */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   corpus;
        } selectio;

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
        } circuitus;

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
            b32               est_postfix;  /* For ++ -- */
        } unarium;

        /* call-expression */
        structura {
            ArborNodus*   callee;
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

        /* sizeof-expression */
        structura {
            ArborNodus*   operandum;     /* Expression or type */
            b32           est_typus;     /* sizeof(type) vs sizeof expr */
        } sizeof_expr;

        /* conditional-expression (ternary) */
        structura {
            ArborNodus*   conditio;
            ArborNodus*   verum;
            ArborNodus*   falsum;
        } ternarium;

        /* assignment-expression */
        structura {
            ArborNodus*       target;
            ArborLexemaGenus  operator;  /* = += -= etc */
            ArborNodus*       valor;
        } assignatio;

        /* compound-statement */
        structura {
            Xar*          sententiae;    /* statements + declarations */
            Xar*          trivia_vacuum; /* trivia for empty body (between { and }) */
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
        } eventus;

        /* identifier, type specifier, storage class, type qualifier */
        structura {
            chorda*       valor;         /* Interned for identifiers */
            ArborLexemaGenus keyword;    /* For keywords like int, const, static */
        } folium;

        /* integer literal */
        structura {
            chorda*       textus;
            i64           valor;
            b32           est_unsigned;
            b32           est_long;
        } numerus;

        /* float literal */
        structura {
            chorda*       textus;
            f64           valor;
        } floating;

        /* char literal */
        structura {
            chorda*       textus;
            i32           valor;
        } character_lit;

        /* string literal */
        structura {
            chorda*       textus;
        } string_lit;

        /* pointer declarator */
        structura {
            Xar*          qualifiers;    /* Xar of ArborNodus* (const, volatile) */
            ArborNodus*   inner;         /* Inner declarator */
        } pointer;

        /* array declarator */
        structura {
            ArborNodus*   inner;
            ArborNodus*   size;          /* May be NULL for [] */
        } array;

        /* init-declarator */
        structura {
            ArborNodus*   declarator;
            ArborNodus*   initializer;   /* May be NULL */
        } init_decl;

        /* parameter declaration */
        structura {
            Xar*          specifiers;
            ArborNodus*   declarator;    /* May be NULL for abstract */
        } parametrum;

        /* initializer list { a, b, c } */
        structura {
            Xar*          elementa;
        } init_list;

        /* error node */
        structura {
            chorda        nuntius;
            Xar*          lexemata_praetermissa;  /* Skipped tokens */
        } error;

        /* generic children (for nodes with uniform child lists) */
        structura {
            Xar*          liberi;        /* Xar of ArborNodus* */
        } genericum;

        /* comma expression */
        structura {
            Xar*          expressiones;  /* Xar of ArborNodus* */
        } comma;

    } datum;
};

/* ==================================================
 * Parse Result
 * ================================================== */

nomen structura {
    b32                 successus;
    ArborNodus*         radix;         /* translation-unit */
    Xar*                errores;       /* Xar of ArborError */
} ArborSyntaxisResultus;

/* ==================================================
 * Parser State (opaque)
 * ================================================== */

nomen structura ArborSyntaxis ArborSyntaxis;

/* ==================================================
 * API - Creation
 * ================================================== */

/* Creare parser */
ArborSyntaxis* arbor_syntaxis_creare(
    Piscina*              piscina,
    InternamentumChorda*  intern);

/* ==================================================
 * API - Parsing
 * ================================================== */

/* Parsere lexemata ad AST
 *
 * lexemata: Xar of ArborLexemaOrigo (from preprocessor)
 * Returns: ArborSyntaxisResultus with AST or errors
 */
ArborSyntaxisResultus arbor_syntaxis_parsere(
    ArborSyntaxis*        syn,
    Xar*                  lexemata);

/* Parsere ex fonte directo (lexer + preprocessor + parser)
 *
 * Convenience function that runs full pipeline.
 */
ArborSyntaxisResultus arbor_syntaxis_parsere_fontem(
    ArborSyntaxis*        syn,
    constans character*   fons,
    i32                   mensura,
    constans character*   via_file);

/* ==================================================
 * API - Utilities
 * ================================================== */

/* Obtinere nomen genus (for CSS selectors and debugging) */
constans character* arbor_nodus_genus_nomen(ArborNodusGenus genus);

/* Imprimere AST pro depuratione */
vacuum arbor_nodus_imprimere(ArborNodus* nodus, i32 indentatio);

/* Format error for display */
chorda arbor_error_formare(Piscina* piscina, ArborError* err);

#endif /* ARBOR_SYNTAXIS_H */
