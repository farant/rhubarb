/* arbor2_glr_tabula.c - Hand-written LR tables for expression grammar */
#include "latina.h"
#include "arbor2_glr.h"
#include "xar.h"
#include <string.h>

/* ==================================================
 * Expression Grammar (simplified for initial testing)
 *
 * P0: S' -> expression $
 * P1: expression -> expression '+' term
 * P2: expression -> term
 * P3: term -> term '*' factor
 * P4: term -> factor
 * P5: factor -> IDENTIFIER
 * P6: factor -> INTEGER
 * P7: factor -> '(' expression ')'
 * P8: factor -> '*' factor          (dereference)
 * P9: factor -> '&' factor          (address-of)
 *
 * This handles precedence correctly:
 * - '*' binds tighter than '+'
 * - Unary '*' and '&' bind tightest
 * ================================================== */

/* Non-terminal indices (internal) */
#define NT_EXPRESSIO    0
#define NT_TERMINUS     1   /* term */
#define NT_FACTOR       2

/* ==================================================
 * Grammar Rules
 * ================================================== */

hic_manens Arbor2Regula REGULAE[] = {
    /* P0: S' -> expression (accept rule, not used in reduce) */
    { ARBOR2_NT_EXPRESSIO, 1, ARBOR2_NODUS_ERROR },

    /* P1: expression -> expression '+' term */
    { ARBOR2_NT_EXPRESSIO, 3, ARBOR2_NODUS_BINARIUM },

    /* P2: expression -> term */
    { ARBOR2_NT_EXPRESSIO, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P3: term -> term '*' factor */
    { ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM },

    /* P4: term -> factor */
    { ARBOR2_NT_TERMINUS, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P5: factor -> IDENTIFIER */
    { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_IDENTIFICATOR },

    /* P6: factor -> INTEGER */
    { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_INTEGER },

    /* P7: factor -> '(' expression ')' */
    { ARBOR2_NT_FACTOR, 3, ARBOR2_NODUS_ERROR },  /* unwrap parens */

    /* P8: factor -> '*' factor (dereference) */
    { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM },

    /* P9: factor -> '&' factor (address-of) */
    { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM },

    /* P10: declaration -> type_specifier declarator */
    { ARBOR2_NT_DECLARATIO, 2, ARBOR2_NODUS_DECLARATIO },

    /* P11: declarator -> '*' declarator */
    { ARBOR2_NT_DECLARATOR, 2, ARBOR2_NODUS_DECLARATOR },

    /* P12: declarator -> IDENTIFIER */
    { ARBOR2_NT_DECLARATOR, 1, ARBOR2_NODUS_DECLARATOR },

    /* P13: statement -> expression ';' */
    { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_SENTENTIA },

    /* P14: statement -> ';' (empty statement) */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_SENTENTIA_VACUA },

    /* P15: statement -> compound_statement */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P16: compound_statement -> '{' statement_list '}' */
    { ARBOR2_NT_CORPUS, 3, ARBOR2_NODUS_CORPUS },

    /* P17: statement_list -> statement_list statement */
    { ARBOR2_NT_ELENCHUS_SENTENTIARUM, 2, ARBOR2_NODUS_CORPUS },  /* accumulate */

    /* P18: statement_list -> ε (empty) */
    { ARBOR2_NT_ELENCHUS_SENTENTIARUM, 0, ARBOR2_NODUS_CORPUS },  /* empty list */

    /* P19: statement -> if_statement */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P20: if_statement -> 'if' '(' expression ')' statement */
    { ARBOR2_NT_SI, 5, ARBOR2_NODUS_SI },

    /* P21: if_statement -> 'if' '(' expression ')' statement 'else' statement */
    { ARBOR2_NT_SI, 7, ARBOR2_NODUS_SI },

    /* P22: statement -> while_statement */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P23: while_statement -> 'while' '(' expression ')' statement */
    { ARBOR2_NT_DUM, 5, ARBOR2_NODUS_DUM },

    /* P24: statement -> do_statement */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P25: do_statement -> 'do' statement 'while' '(' expression ')' ';' */
    { ARBOR2_NT_FAC, 7, ARBOR2_NODUS_FAC },

    /* P26: statement -> for_statement */
    { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P27: for_statement -> 'for' '(' expr_opt ';' expr_opt ';' expr_opt ')' statement */
    { ARBOR2_NT_PER, 9, ARBOR2_NODUS_PER },

    /* P28: expression_opt -> expression */
    { ARBOR2_NT_EXPRESSIO_OPTATIVA, 1, ARBOR2_NODUS_ERROR },  /* pass through */

    /* P29: expression_opt -> epsilon */
    { ARBOR2_NT_EXPRESSIO_OPTATIVA, 0, ARBOR2_NODUS_ERROR },  /* epsilon, NULL value */

    /* P30: statement -> 'break' ';' */
    { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_FRANGE },

    /* P31: statement -> 'continue' ';' */
    { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_PERGE },

    /* P32: statement -> 'return' expression_opt ';' */
    { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_REDDE },

    /* P33: statement -> 'goto' IDENTIFIER ';' */
    { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_SALTA },

    /* P34: statement -> IDENTIFIER ':' statement */
    { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_TITULATUM },

    /* P35: statement -> 'switch' '(' expression ')' statement */
    { ARBOR2_NT_SENTENTIA, 5, ARBOR2_NODUS_COMMUTATIO },

    /* P36: statement -> 'case' expression ':' statement */
    { ARBOR2_NT_SENTENTIA, 4, ARBOR2_NODUS_CASUS },

    /* P37: statement -> 'default' ':' statement */
    { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_ORDINARIUS },

    /* P38: declarator -> declarator '(' ')' (function declarator) */
    { ARBOR2_NT_DECLARATOR, 3, ARBOR2_NODUS_DECLARATOR_FUNCTI },

    /* P39: declarator -> declarator '(' VOID ')' (function declarator with void) */
    { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR_FUNCTI },

    /* P40: declarator -> declarator '(' parameter_list ')' */
    { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR_FUNCTI },

    /* P41: parameter_list -> parameter_declaration */
    { ARBOR2_NT_PARAMETER_LIST, 1, ARBOR2_NODUS_ERROR },  /* builds Xar */

    /* P42: parameter_list -> parameter_list ',' parameter_declaration */
    { ARBOR2_NT_PARAMETER_LIST, 3, ARBOR2_NODUS_ERROR },  /* extends Xar */

    /* P43: parameter_declaration -> type_specifier declarator */
    { ARBOR2_NT_PARAMETER_DECL, 2, ARBOR2_NODUS_PARAMETER_DECL },

    /* P44: function_definition -> type_specifier declarator compound_statement */
    { ARBOR2_NT_DEFINITIO_FUNCTI, 3, ARBOR2_NODUS_DEFINITIO_FUNCTI },

    /* Phase E1: Struct grammar rules */
    /* P45: struct_specifier -> 'struct' IDENTIFIER '{' struct_member_list '}' */
    { ARBOR2_NT_STRUCT_SPECIFIER, 5, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* P46: struct_specifier -> 'struct' '{' struct_member_list '}' (anonymous) */
    { ARBOR2_NT_STRUCT_SPECIFIER, 4, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* P47: struct_specifier -> 'struct' IDENTIFIER (forward reference) */
    { ARBOR2_NT_STRUCT_SPECIFIER, 2, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* P48: struct_member_list -> type_specifier IDENTIFIER ';' (non-pointer first member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO },

    /* P49: struct_member_list -> struct_member_list type_specifier IDENTIFIER ';' (append non-pointer) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P50: struct_member_list -> type_specifier '*' IDENTIFIER ';' (pointer first member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P51: struct_member_list -> struct_member_list type_specifier '*' IDENTIFIER ';' (append pointer) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO },

    /* P52: struct_specifier -> 'union' IDENTIFIER '{' struct_member_list '}' (named union) */
    { ARBOR2_NT_STRUCT_SPECIFIER, 5, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* P53: struct_specifier -> 'union' '{' struct_member_list '}' (anonymous union) */
    { ARBOR2_NT_STRUCT_SPECIFIER, 4, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* P54: struct_specifier -> 'union' IDENTIFIER (union forward reference) */
    { ARBOR2_NT_STRUCT_SPECIFIER, 2, ARBOR2_NODUS_STRUCT_SPECIFIER },

    /* ==================================================
     * Enum Specifier Rules (P55-P61)
     * ================================================== */

    /* P55: enum_specifier -> 'enum' IDENTIFIER '{' enumerator_list '}' (named enum) */
    { ARBOR2_NT_ENUM_SPECIFIER, 5, ARBOR2_NODUS_ENUM_SPECIFIER },

    /* P56: enum_specifier -> 'enum' '{' enumerator_list '}' (anonymous enum) */
    { ARBOR2_NT_ENUM_SPECIFIER, 4, ARBOR2_NODUS_ENUM_SPECIFIER },

    /* P57: enum_specifier -> 'enum' IDENTIFIER (forward reference) */
    { ARBOR2_NT_ENUM_SPECIFIER, 2, ARBOR2_NODUS_ENUM_SPECIFIER },

    /* P58: enumerator_list -> enumerator (first enumerator, creates Xar) */
    { ARBOR2_NT_ENUMERATOR_LIST, 1, ARBOR2_NODUS_ERROR },

    /* P59: enumerator_list -> enumerator_list ',' enumerator (append to list) */
    { ARBOR2_NT_ENUMERATOR_LIST, 3, ARBOR2_NODUS_ERROR },

    /* P60: enumerator -> IDENTIFIER (plain enumerator) */
    { ARBOR2_NT_ENUMERATOR, 1, ARBOR2_NODUS_ENUMERATOR },

    /* P61: enumerator -> IDENTIFIER '=' expression (enumerator with value) */
    { ARBOR2_NT_ENUMERATOR, 3, ARBOR2_NODUS_ENUMERATOR },

    /* ========== BIT FIELD RULES (P62-P65) ========== */

    /* P62: struct_member_list -> type_spec ID ':' expr ';' (first named bit field) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO },

    /* P63: struct_member_list -> member_list type_spec ID ':' expr ';' (append named bit field) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 6, ARBOR2_NODUS_DECLARATIO },

    /* P64: struct_member_list -> type_spec ':' expr ';' (first anonymous bit field) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P65: struct_member_list -> member_list type_spec ':' expr ';' (append anonymous bit field) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO },

    /* ========== NESTED TYPE MEMBERS (P66-P73) ========== */

    /* P66: struct_member_list -> struct_specifier ID ';' (first nested struct/union member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO },

    /* P67: struct_member_list -> struct_specifier '*' ID ';' (first nested struct/union ptr member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P68: struct_member_list -> member_list struct_specifier ID ';' (append nested struct/union) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P69: struct_member_list -> member_list struct_specifier '*' ID ';' (append nested struct/union ptr) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO },

    /* P70: struct_member_list -> enum_specifier ID ';' (first nested enum member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO },

    /* P71: struct_member_list -> enum_specifier '*' ID ';' (first nested enum ptr member) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P72: struct_member_list -> member_list enum_specifier ID ';' (append nested enum) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO },

    /* P73: struct_member_list -> member_list enum_specifier '*' ID ';' (append nested enum ptr) */
    { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO }
};

hic_manens i32 NUM_REGULAE = (i32)(magnitudo(REGULAE) / magnitudo(REGULAE[0]));

/* ==================================================
 * LR(1) Action Table
 *
 * States:
 * 0: Initial state
 * 1: After expression
 * 2: After term
 * 3: After factor
 * 4: After IDENTIFIER
 * 5: After INTEGER
 * 6: After '('
 * 7: After '*' (unary)
 * 8: After '&' (unary)
 * 9: After expression '+'
 * 10: After term '*'
 * 11: After '(' expression
 * 12: After '(' expression ')'
 * 13: After expression '+' term
 * 14: After term '*' factor
 * 15: After '*' factor (unary result)
 * 16: After '&' factor (unary result)
 * ================================================== */

/* Action encoding: positive = shift to state, negative = reduce by rule */
#define SHIFT(s)    (s)
#define REDUCE(r)   (-(r) - 1)
#define ACCEPT      (-1000)
#define ERROR_ACT   (-2000)

hic_manens Arbor2TabulaActio ACTIONES[] = {
    /* State 0: Initial - expect factor-starting tokens, ';', '{', 'if', 'while', 'do', 'for', jump, struct */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },   /* empty statement */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },   /* compound statement */
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },   /* if statement */
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },   /* while statement */
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },   /* do-while statement */
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },   /* for statement */
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },   /* break statement */
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },   /* continue statement */
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },   /* return statement */
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },   /* goto statement */
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },   /* switch statement */
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },   /* case label */
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },   /* default label */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* struct type specifier */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* union type specifier */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* enum type specifier */

    /* State 1: After expression - expect '+', ';', or end */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 22 },   /* expression statement */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 0 },  /* for nested */

    /* State 2: After term - expect '*', '+', ';', '}', ':', ',', or end */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 2 },  /* expression -> term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 2 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 2 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 2 },  /* for enum values */

    /* State 3: After factor - reduce to term */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 4 },  /* term -> factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 4 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 4 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 4 },  /* for enum values */

    /* State 4: After IDENTIFIER - AMBIGUOUS for '*' (expr vs decl), or label with ':' */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 5 },  /* expr: factor -> ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },  /* decl: start declarator */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 116 }, /* decl: direct declarator name */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 5 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 77 },  /* label: shift to labeled stmt */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 5 },  /* case expr: reduce ID to factor */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 5 },  /* for enum values */

    /* State 5: After INTEGER - reduce to factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 6 },  /* factor -> INT */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 6 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 6 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 6 },  /* for enum values */

    /* State 6: After '(' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 7: After unary '*' - expect factor */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 8: After unary '&' - expect factor */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 9: After expression '+' - expect term-starting tokens */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 10: After term '*' - expect factor-starting tokens */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 11: After '(' expression - expect ')' or '+' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT,  12 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },

    /* State 12: After '(' expression ')' - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 7 },  /* factor -> (expr) */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 7 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 7 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 7 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 7 },  /* for enum values */

    /* State 13: After expression '+' term - reduce or continue */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },  /* * binds tighter */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 1 },   /* expr -> expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 1 },   /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },   /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 1 },   /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 1 },   /* for enum values */

    /* State 14: After term '*' factor - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 3 },  /* term -> term * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 3 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 3 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 3 },  /* for enum values */

    /* State 15: After '*' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 8 },  /* factor -> * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 8 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 8 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 8 },  /* for enum values */

    /* State 16: After '&' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 9 },  /* factor -> & factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 9 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },  /* for compound */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 9 },  /* for enum values */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 9 },  /* for case expression */

    /* State 17: After '*' in declarator - expect ID or more '*' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* declarator name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* more pointers */

    /* State 18: After IDENTIFIER in declarator - reduce P12 */
    /* When we see '(' we reduce first, then state 19 handles the shift */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12 },  /* declarator -> ID */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 12 },  /* reduce first, then fn decl */

    /* State 19: After '* declarator' - reduce P11 or shift ( for fn */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 11 },  /* declarator -> * declarator */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 91 },   /* function declarator */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 11 },  /* for function definition body */

    /* State 20: After 'type_specifier declarator' - reduce P10 or continue to function */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 10 },  /* declaration -> type declarator */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 91 },   /* function declarator params */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },   /* function definition body */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_ERROR,  0 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_ERROR,  0 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_ERROR,  0 },

    /* State 21: After declaration - accept */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },

    /* State 22: After 'expression ;' - reduce to statement */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 13 },  /* P13: statement -> expr ; */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 13 },  /* for compound stmts */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 13 },  /* next stmt */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 13 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 13 },

    /* State 23: After lone ';' - reduce to empty statement */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 14 },  /* P14: statement -> ; */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 14 },  /* for compound stmts */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 14 },  /* next stmt */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 14 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 14 },

    /* State 24: After statement - accept */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },

    /* State 25: After '{' - epsilon reduce to empty list (P18) */
    /* All lookaheads trigger epsilon reduction for empty stmt_list */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 18 },  /* } -> empty list */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 18 },  /* if stmt */
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 18 },  /* while stmt */
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 18 },  /* do-while stmt */
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 18 },  /* for stmt */
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 18 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 18 },

    /* State 26: After '{ stmt_list' - expect more statements or '}' */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 27 },   /* end compound */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* next stmt */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },   /* empty stmt */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },   /* nested compound */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },   /* if statement */
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },   /* while statement */
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },   /* do-while statement */
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },   /* for statement */
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },   /* break statement */
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },   /* continue statement */
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },   /* return statement */
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },   /* goto statement */
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },   /* switch statement */
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },   /* case label */
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },   /* default label */

    /* State 27: After '{ stmt_list }' - reduce to compound (P16) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 16 },  /* nested */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 16 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 16 },

    /* State 28: After stmt in stmt_list - reduce P17 (list -> list stmt) */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 17 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 17 },

    /* State 29: After compound_statement - accept or reduce P15 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },   /* top level */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 15 },  /* stmt -> compound */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 15 },

    /* ==================================================
     * IF/ELSE States (30-36)
     * ================================================== */

    /* State 30: After 'if' - expect '(' */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 31 },

    /* State 31: After 'if (' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 32: After 'if ( expr' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 33 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },

    /* State 33: After 'if ( expr )' - expect statement */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 34: After 'if ( expr ) stmt' - dangling else resolution */
    /* SHIFT else wins over REDUCE P20 (prefer else binding to nearest if) */
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_SHIFT, 35 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 20 },  /* if without else */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 20 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 20 },

    /* State 35: After 'if ( expr ) stmt else' - expect statement */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 36: After 'if ( expr ) stmt else stmt' - reduce P21 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 21 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 21 },

    /* State 37: After if_statement - reduce P19 (stmt -> if_stmt) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 19 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 19 },

    /* State 38: Nested compound_statement - always reduce P15 (never accept) */
    /* Used for compound statements inside if/else/while/for bodies */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 15 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 15 },

    /* ==================================================
     * WHILE States (39-44)
     * ================================================== */

    /* State 39: After 'while' - expect '(' */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 40 },

    /* State 40: After 'while (' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 41: After 'while ( expr' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 42 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },

    /* State 42: After 'while ( expr )' - expect statement (loop body) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 43: After 'while ( expr ) stmt' - reduce P23 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 23 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 23 },

    /* State 44: After while_statement - reduce P22 (stmt -> while_stmt) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 22 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 22 },

    /* ==================================================
     * DO-WHILE States (45-52)
     * ================================================== */

    /* State 45: After 'do' - expect statement (loop body) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 46: After 'do stmt' - expect 'while' */
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 47 },

    /* State 47: After 'do stmt while' - expect '(' */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 48 },

    /* State 48: After 'do stmt while (' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 49: After 'do stmt while ( expr' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 50 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },

    /* State 50: After 'do stmt while ( expr )' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 51 },

    /* State 51: After 'do stmt while ( expr ) ;' - reduce P25 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 25 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 25 },

    /* State 52: After do_statement - reduce P24 (stmt -> do_stmt) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 24 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 24 },

    /* ==================================================
     * FOR States (53-65)
     * ================================================== */

    /* State 53: After 'for' - expect '(' */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 54 },

    /* State 54: After 'for (' - expect expression or ';' for empty init */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29 },  /* epsilon: expr_opt -> ε */

    /* State 55: After 'for ( expression' - reduce to expr_opt */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 28 },  /* expr_opt -> expression */

    /* State 56: After 'for ( expr_opt' - expect first ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 57 },

    /* State 57: After 'for ( expr_opt ;' - expect condition or ';' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29 },  /* epsilon: expr_opt -> ε */

    /* State 58: After 'for ( expr_opt ; expression' - reduce to expr_opt */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 28 },  /* expr_opt -> expression */

    /* State 59: After 'for ( expr_opt ; expr_opt' - expect second ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 60 },

    /* State 60: After 'for ( expr_opt ; expr_opt ;' - expect increment or ')' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 29 },  /* epsilon: expr_opt -> ε */

    /* State 61: After 'for ( ... ; expression' (increment) - reduce to expr_opt */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 28 },  /* expr_opt -> expression */

    /* State 62: After 'for ( expr_opt ; expr_opt ; expr_opt' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 63 },

    /* State 63: After 'for ( ... )' - expect statement (loop body) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 64: After 'for ( ... ) stmt' - reduce P27 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 27 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 27 },

    /* State 65: After for_statement - reduce P26 (stmt -> for_stmt) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 26 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 26 },

    /* ==================================================
     * JUMP STATEMENT States (66-76)
     * ================================================== */

    /* State 66: After 'break' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 67 },

    /* State 67: After 'break ;' - reduce P30 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 30 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 30 },

    /* State 68: After 'continue' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 69 },

    /* State 69: After 'continue ;' - reduce P31 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 31 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 31 },

    /* State 70: After 'return' - expect expression or ';' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29 },  /* epsilon: expr_opt -> ε */

    /* State 71: After 'return expression' - can continue or reduce */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* continue expr with + */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 28 },  /* expr_opt -> expression */

    /* State 72: After 'return expr_opt' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 73 },

    /* State 73: After 'return expr_opt ;' - reduce P32 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 32 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 32 },

    /* State 74: After 'goto' - expect IDENTIFIER */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 75 },

    /* State 75: After 'goto IDENTIFIER' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 76 },

    /* State 76: After 'goto IDENTIFIER ;' - reduce P33 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 33 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 33 },

    /* State 77: After 'IDENTIFIER :' - expect statement (like states 33, 35, 42, 45, 63) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 78: After 'IDENTIFIER : statement' - reduce P34 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 34 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 34 },

    /* ==================================================
     * Switch/Case/Default States (79-90)
     * ================================================== */

    /* State 79: After 'switch' - expect '(' */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 80 },

    /* State 80: After 'switch (' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 81: After 'switch ( expr' - expect ')' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 82 },

    /* State 82: After 'switch ( expr )' - expect statement (body) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 83: After 'switch ( expr ) stmt' - reduce P35 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 35 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 35 },

    /* State 84: After 'case' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 85: After 'case expr' - expect ':' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 86 },

    /* State 86: After 'case expr :' - expect statement */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 87: After 'case expr : stmt' - reduce P36 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 36 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 36 },

    /* State 88: After 'default' - expect ':' */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 89 },

    /* State 89: After 'default :' - expect statement */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT, 39 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT, 45 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT, 53 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT, 66 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT, 68 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT, 70 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT, 74 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT, 79 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT, 84 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT, 88 },

    /* State 90: After 'default : stmt' - reduce P37 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 37 },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 37 },

    /* ==================================================
     * FUNCTION DECLARATOR STATES (Phase D1)
     * ================================================== */

    /* State 91: After 'declarator (' - expect ')' or 'void' or parameter type */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 92 },   /* () */
    { ARBOR2_LEXEMA_VOID,           ARBOR2_ACTIO_SHIFT, 93 },   /* (void */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 95 },   /* param type (typedef) */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 95 },   /* param type (int) */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 95 },   /* param type (char) */

    /* State 92: After 'declarator ( )' - reduce P38 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 38 },  /* declarator -> declarator () */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 38 },  /* for chained fn decl */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 38 },  /* for function definition body */

    /* State 93: After 'declarator ( void' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 94 },   /* (void) */

    /* State 94: After 'declarator ( void )' - reduce P39 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 39 },  /* declarator -> declarator (void) */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 39 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 39 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 39 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 39 },  /* for chained fn decl */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 39 },  /* for function definition body */

    /* ==================================================
     * Phase D2: Parameter List States (95-111)
     * ================================================== */

    /* State 95: After '( type_specifier' - expect '*' or param name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 96 },   /* pointer param */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 97 },   /* param name */

    /* State 96: After '( type_spec *' - expect '*' or param name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 96 },   /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 98 },   /* param name after * */

    /* State 97: After '( type_spec name' - reduce P12 (declarator -> IDENTIFIER) */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12 },

    /* State 98: After '( type_spec *...* name' - reduce P12 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12 },

    /* State 99: After '( type_spec declarator' - reduce P43 (param_decl) */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 43 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 43 },

    /* State 100: After '( type_spec * declarator' - reduce P11 (declarator -> * declarator) */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 11 },

    /* State 101: After '( param_decl' - reduce P41 (param_list -> param_decl) */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 41 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 41 },

    /* State 102: After '( param_list' - expect ')' or ',' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 103 },  /* close params */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 104 },  /* more params */

    /* State 103: After '( param_list )' - reduce P40 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 40 },  /* for function definition body */

    /* State 104: After 'param_list ,' - expect next type_specifier */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 105 },  /* next param type (typedef) */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 105 },  /* next param type (int) */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 105 },  /* next param type (char) */

    /* State 105: After 'param_list , type_spec' - expect '*' or param name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 106 },  /* pointer param */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 107 },  /* param name */

    /* State 106: After 'param_list , type_spec *' - expect '*' or param name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 106 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 108 },  /* param name */

    /* State 107: After 'param_list , type_spec name' - reduce P12 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12 },

    /* State 108: After 'param_list , type_spec *...* name' - reduce P12 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12 },

    /* State 109: After 'param_list , type_spec declarator' - reduce P43 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 43 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 43 },

    /* State 110: After 'param_list , type_spec * declarator' - reduce P11 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 11 },

    /* State 111: After 'param_list , param_decl' - reduce P42 */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 42 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 42 },

    /* ==================================================
     * Phase D3: Function Definition States (113-116)
     * ================================================== */

    /* State 113: After 'type_spec declarator compound' - reduce P44 (function_definition) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 44 },

    /* State 114: After function_definition - accept */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },

    /* State 115: (reserved) */

    /* State 116: After 'type_spec name' (direct declarator) - reduce P12 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12 },

    /* ==================================================
     * Phase E1: Struct States (117-130)
     * ================================================== */

    /* State 117: After 'struct' - expect ID or '{' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 118 },  /* struct tag */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 119 },  /* anonymous struct body */

    /* State 118: After 'struct ID' - expect '{' or reduce P47 (forward ref) */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 120 },  /* start struct body */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 47 },  /* forward ref: struct foo x */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 47 },  /* forward ref: struct foo * */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 47 },  /* forward ref */

    /* State 119: After 'struct {' (anonymous) - start member list */
    /* Members start with type specifier (ID, int, char, struct, union, enum) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121 },  /* member type spec */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121 },  /* int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121 },  /* char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* nested enum */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* empty struct (edge case) */

    /* State 120: After 'struct ID {' (named) - start member list */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121 },  /* member type spec */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121 },  /* int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121 },  /* char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* nested enum */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* empty struct (edge case) */

    /* State 121: After member type_specifier (ID) - expect '*' or member name or ':' (anon bit field) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 122 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 123 },  /* member name */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 168 },  /* anonymous bit field */

    /* State 122: After member type_spec '*' - expect more '*' or member name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 122 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 124 },  /* member name */

    /* State 123: After member type_spec name - expect ';' or ':' (bit field) */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 125 },  /* end member decl */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 162 },  /* named bit field */

    /* State 124: After member type_spec * name - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 126 },  /* end member decl */

    /* State 125: After member decl ';' (no pointer) - reduce member, more or end */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, end struct */

    /* State 126: After first pointer member decl ';' - reduce P50 (4 symbols) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 50 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 50 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 50 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 50 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 50 },  /* reduce, end struct */

    /* State 127: After struct_member_list in anonymous struct - expect more or '}' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent member type */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* nested enum */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 128 },  /* end anonymous struct */

    /* State 128: After 'struct { members }' - reduce P46 (anonymous) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 46 },  /* anon struct var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 46 },  /* anon struct pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 46 },

    /* State 129: After struct_member_list in named struct - expect more or '}' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent member type */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* nested enum */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 130 },  /* end named struct */

    /* State 130: After 'struct ID { members }' - reduce P45 (named) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 45 },  /* struct var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 45 },  /* struct pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 45 },

    /* ==================================================
     * States 131-136: Subsequent member path
     * (same structure as 121-126, but reduces P49/P51 instead of P48/P50)
     * ================================================== */

    /* State 131: After type_specifier in subsequent member - expect '*' or name or ':' */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 132 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 133 },  /* member name */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 171 },  /* anonymous bit field */

    /* State 132: After type_spec '*' in subsequent member - expect more '*' or name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 132 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 134 },  /* member name */

    /* State 133: After subsequent non-pointer member type + name - expect ';' or ':' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 135 },  /* end member decl */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 165 },  /* named bit field */

    /* State 134: After subsequent pointer member type + * + name - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 136 },  /* end member decl */

    /* State 135: After subsequent non-pointer member ';' - reduce P49 (4 symbols) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, end struct */

    /* State 136: After subsequent pointer member ';' - reduce P51 (5 symbols) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 51 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 51 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 51 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 51 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 51 },  /* reduce, end struct */

    /* ========== UNION STATES (137-144) ========== */

    /* State 137: After 'union' - expect ID or '{' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 138 },  /* union tag */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 139 },  /* anonymous union body */

    /* State 138: After 'union ID' - expect '{' or reduce P54 (forward ref) */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 140 },  /* start union body */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 54 },  /* forward ref: union foo x */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 54 },  /* forward ref: union foo * */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 54 },  /* forward ref */

    /* State 139: After 'union {' (anonymous) - start member list */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121 },  /* member type spec (reuse) */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121 },  /* int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121 },  /* char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* empty union (edge case) */

    /* State 140: After 'union ID {' (named) - start member list */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121 },  /* member type spec (reuse) */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121 },  /* int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121 },  /* char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* empty union (edge case) */

    /* State 141: After struct_member_list in anonymous union - expect more or '}' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent member type */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 142 },  /* end anonymous union */

    /* State 142: After 'union { members }' - reduce P53 (anonymous) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 53 },  /* anon union var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 53 },  /* anon union pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 53 },

    /* State 143: After struct_member_list in named union - expect more or '}' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent member type */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131 },  /* subsequent char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* nested struct */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* nested union */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 144 },  /* end named union */

    /* State 144: After 'union ID { members }' - reduce P52 (named) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 52 },  /* union var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 52 },  /* union pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 52 },

    /* ==================================================
     * Enum States (145-162)
     * ================================================== */

    /* State 145: After 'enum' - expect ID or '{' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 146 },  /* enum tag */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 147 },  /* anonymous enum body */

    /* State 146: After 'enum ID' - expect '{' or reduce P57 (forward ref) */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 148 },  /* start enum body */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 57 },  /* forward ref: enum foo x */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 57 },  /* forward ref: enum foo * */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 57 },

    /* State 147: After 'enum {' (anonymous) - expect first enumerator */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 149 },  /* enumerator name */

    /* State 148: After 'enum ID {' (named) - expect first enumerator */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 149 },  /* enumerator name */

    /* State 149: After enumerator IDENTIFIER - expect '=' or ',' or '}' */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_SHIFT, 150 },  /* ID = ... */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 60 },  /* reduce ID to enumerator */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 60 },  /* reduce ID to enumerator */

    /* State 150: After 'ID =' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 151: After 'ID = expr' - continue expr or reduce P61 */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 61 },  /* ID = expr -> enumerator */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 61 },  /* ID = expr -> enumerator */

    /* State 152: After first enumerator - reduce P58 to create list */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 58 },  /* enumerator -> enumerator_list */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 58 },  /* enumerator -> enumerator_list */

    /* State 153: After enumerator_list in anonymous enum - expect ',' or '}' */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 156 },  /* more enumerators */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 154 },  /* end anonymous enum */

    /* State 154: After 'enum { list }' - reduce P56 (anonymous) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 56 },  /* anon enum var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 56 },  /* anon enum pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 56 },

    /* State 155: After enumerator_list in named enum - expect ',' or '}' */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 156 },  /* more enumerators */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 157 },  /* end named enum */

    /* State 156: After ',' - expect next enumerator */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 158 },  /* next enumerator name */

    /* State 157: After 'enum ID { list }' - reduce P55 (named) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 55 },  /* enum var */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 55 },  /* enum pointer */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 55 },

    /* State 158: After ',' IDENTIFIER (subsequent enumerator) - expect '=' or ',' or '}' */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_SHIFT, 159 },  /* ID = ... */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 60 },  /* reduce to enumerator */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 60 },  /* reduce to enumerator */

    /* State 159: After subsequent 'ID =' - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 160: After subsequent 'ID = expr' - continue expr or reduce P61 */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 61 },  /* ID = expr -> enumerator */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 61 },  /* ID = expr -> enumerator */

    /* State 161: After subsequent enumerator - reduce P59 (append) */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 59 },  /* list , enumerator -> list */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 59 },

    /* ========== BIT FIELD STATES (162-173) ========== */

    /* State 162: After type_spec ID ':' (first named bit field) - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 163: After type_spec ID ':' expr (first) - continue expr or ';' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 164 },  /* end bit field */

    /* State 164: After type_spec ID ':' expr ';' (first) - reduce P62 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 62 },  /* more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 62 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 62 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 62 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 62 },  /* end struct */

    /* State 165: After member_list type_spec ID ':' (subsequent named) - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 166: After member_list type_spec ID ':' expr (subsequent) - continue or ';' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 167 },  /* end bit field */

    /* State 167: After member_list type_spec ID ':' expr ';' - reduce P63 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 63 },  /* more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 63 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 63 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 63 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 63 },  /* end struct */

    /* State 168: After type_spec ':' (first anonymous bit field) - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 169: After type_spec ':' expr (first anon) - continue expr or ';' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 170 },  /* end bit field */

    /* State 170: After type_spec ':' expr ';' (first anon) - reduce P64 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 64 },  /* more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 64 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 64 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 64 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 64 },  /* end struct */

    /* State 171: After member_list type_spec ':' (subsequent anon) - expect expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression ID */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression INT */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* (expr) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 172: After member_list type_spec ':' expr (subsequent anon) - continue or ';' */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 173 },  /* end bit field */

    /* State 173: After member_list type_spec ':' expr ';' (subsequent anon) - reduce P65 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 65 },  /* more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 65 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 65 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 65 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 65 },  /* end struct */

    /* ==================================================
     * States 174-197: Nested struct/union/enum as member type (E5)
     * ================================================== */

    /* ========== FIRST MEMBER WITH NESTED STRUCT/UNION (P66/P67) ========== */

    /* State 174: After struct_specifier (first member) - expect '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 175 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 176 },  /* member name */

    /* State 175: After struct_specifier '*' (first member) - expect ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 175 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 177 },  /* member name */

    /* State 176: After struct_specifier ID (first member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 178 },

    /* State 177: After struct_specifier '*' ID (first member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 179 },

    /* State 178: After struct_specifier ID ';' (first member) - reduce P66 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 66 },

    /* State 179: After struct_specifier '*' ID ';' (first member) - reduce P67 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 67 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 67 },

    /* ========== SUBSEQUENT MEMBER WITH NESTED STRUCT/UNION (P68/P69) ========== */

    /* State 180: After struct_specifier (subsequent member) - expect '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 181 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 182 },  /* member name */

    /* State 181: After struct_specifier '*' (subsequent member) - expect ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 181 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 183 },  /* member name */

    /* State 182: After struct_specifier ID (subsequent member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 184 },

    /* State 183: After struct_specifier '*' ID (subsequent member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 185 },

    /* State 184: After struct_specifier ID ';' (subsequent member) - reduce P68 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 68 },

    /* State 185: After struct_specifier '*' ID ';' (subsequent member) - reduce P69 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 69 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 69 },

    /* ========== FIRST MEMBER WITH NESTED ENUM (P70/P71) ========== */

    /* State 186: After enum_specifier (first member) - expect '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 187 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 188 },  /* member name */

    /* State 187: After enum_specifier '*' (first member) - expect ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 187 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 189 },  /* member name */

    /* State 188: After enum_specifier ID (first member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 190 },

    /* State 189: After enum_specifier '*' ID (first member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 191 },

    /* State 190: After enum_specifier ID ';' (first member) - reduce P70 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 70 },

    /* State 191: After enum_specifier '*' ID ';' (first member) - reduce P71 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 71 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 71 },

    /* ========== SUBSEQUENT MEMBER WITH NESTED ENUM (P72/P73) ========== */

    /* State 192: After enum_specifier (subsequent member) - expect '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 193 },  /* pointer member */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 194 },  /* member name */

    /* State 193: After enum_specifier '*' (subsequent member) - expect ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 193 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 195 },  /* member name */

    /* State 194: After enum_specifier ID (subsequent member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 196 },

    /* State 195: After enum_specifier '*' ID (subsequent member) - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 197 },

    /* State 196: After enum_specifier ID ';' (subsequent member) - reduce P72 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 72 },

    /* State 197: After enum_specifier '*' ID ';' (subsequent member) - reduce P73 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 73 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 73 }
};

hic_manens i32 NUM_ACTIONES = (i32)(magnitudo(ACTIONES) / magnitudo(ACTIONES[0]));

/* State -> first action index mapping */
hic_manens i32 ACTIO_INDICES[] = {
    0,      /* State 0: 21 actions (+struct, +union, +enum) */
    21,     /* State 1: 4 actions */
    25,     /* State 2: 8 actions (added COLON, COMMA) */
    33,     /* State 3: 8 actions (added COLON, COMMA) */
    41,     /* State 4: 11 actions (2x COLON + IDENTIFIER + COMMA) */
    52,     /* State 5: 8 actions (added COLON, COMMA) */
    60,     /* State 6: 5 actions */
    65,     /* State 7: 5 actions */
    70,     /* State 8: 5 actions */
    75,     /* State 9: 5 actions */
    80,     /* State 10: 5 actions */
    85,     /* State 11: 2 actions */
    87,     /* State 12: 8 actions (added COLON, COMMA) */
    95,     /* State 13: 8 actions (added COLON, COMMA) */
    103,    /* State 14: 8 actions (added COLON, COMMA) */
    111,    /* State 15: 8 actions (added COLON, COMMA) */
    119,    /* State 16: 8 actions (added COLON, COMMA) */
    127,    /* State 17: 2 actions (declarator path) */
    129,    /* State 18: 5 actions (reduce P12 + fn decl) */
    134,    /* State 19: 6 actions (reduce P11 + fn decl + BRACE_APERTA) */
    140,    /* State 20: 6 actions (reduce P10 + fn decl (/) + fn def ({)) */
    146,    /* State 21: 1 action (accept declaration) */
    147,    /* State 22: 21 actions (reduce P13 + switch/case/default) */
    168,    /* State 23: 21 actions (reduce P14 + switch/case/default) */
    189,    /* State 24: 1 action (accept statement) */
    190,    /* State 25: 19 actions (epsilon reduce P18 + switch/case/default) */
    209,    /* State 26: 19 actions (+switch, case, default) */
    228,    /* State 27: 21 actions (reduce P16 + switch/case/default) */
    249,    /* State 28: 19 actions (reduce P17 + switch/case/default) */
    268,    /* State 29: 21 actions (accept/reduce P15 + switch/case/default) */
    289,    /* State 30: 1 action (after 'if') */
    290,    /* State 31: 5 actions (after 'if (') */
    295,    /* State 32: 2 actions (after 'if ( expr') */
    297,    /* State 33: 18 actions (+switch, case, default) */
    315,    /* State 34: 21 actions (after 'if ( expr ) stmt' + switch/case/default) */
    336,    /* State 35: 18 actions (+switch, case, default) */
    354,    /* State 36: 21 actions (reduce P21 + switch/case/default) */
    375,    /* State 37: 21 actions (reduce P19 + switch/case/default) */
    396,    /* State 38: 21 actions (reduce P15 for nested compound + switch/case/default) */
    417,    /* State 39: 1 action (after 'while') */
    418,    /* State 40: 5 actions (after 'while (') */
    423,    /* State 41: 2 actions (after 'while ( expr') */
    425,    /* State 42: 18 actions (+switch, case, default) */
    443,    /* State 43: 21 actions (reduce P23 + switch/case/default) */
    464,    /* State 44: 21 actions (reduce P22 + switch/case/default) */
    485,    /* State 45: 18 actions (+switch, case, default) */
    503,    /* State 46: 1 action (after 'do stmt') */
    504,    /* State 47: 1 action (after 'do stmt while') */
    505,    /* State 48: 5 actions (after 'do stmt while (') */
    510,    /* State 49: 2 actions (after 'do stmt while ( expr') */
    512,    /* State 50: 1 action (after 'do stmt while ( expr )') */
    513,    /* State 51: 21 actions (reduce P25 + switch/case/default) */
    534,    /* State 52: 21 actions (reduce P24 + switch/case/default) */
    555,    /* State 53: 1 action (after 'for') */
    556,    /* State 54: 6 actions (after 'for (') */
    562,    /* State 55: 1 action (after 'for ( expr') */
    563,    /* State 56: 1 action (after 'for ( expr_opt') */
    564,    /* State 57: 6 actions (after 'for ( expr_opt ;') */
    570,    /* State 58: 1 action (after 'for ( ... ; expr') */
    571,    /* State 59: 1 action (after 'for ( ... ; expr_opt') */
    572,    /* State 60: 6 actions (after 'for ( ... ; expr_opt ;') */
    578,    /* State 61: 1 action (after 'for ( ... ; expr') */
    579,    /* State 62: 1 action (after 'for ( ... ; expr_opt') */
    580,    /* State 63: 18 actions (+switch, case, default) */
    598,    /* State 64: 21 actions (reduce P27 + switch/case/default) */
    619,    /* State 65: 21 actions (reduce P26 + switch/case/default) */
    640,    /* State 66: 1 action (after 'break') */
    641,    /* State 67: 21 actions (reduce P30 + switch/case/default) */
    662,    /* State 68: 1 action (after 'continue') */
    663,    /* State 69: 21 actions (reduce P31 + switch/case/default) */
    684,    /* State 70: 6 actions (after 'return') */
    690,    /* State 71: 2 actions (after 'return expr' + continue) */
    692,    /* State 72: 1 action (after 'return expr_opt') */
    693,    /* State 73: 21 actions (reduce P32 + switch/case/default) */
    714,    /* State 74: 1 action (after 'goto') */
    715,    /* State 75: 1 action (after 'goto ID') */
    716,    /* State 76: 21 actions (reduce P33 + switch/case/default) */
    737,    /* State 77: 18 actions (+switch, case, default) */
    755,    /* State 78: 21 actions (reduce P34 +switch, case, default) */
    776,    /* State 79: 1 action (after 'switch') */
    777,    /* State 80: 5 actions (after 'switch (') */
    782,    /* State 81: 2 actions (after 'switch ( expr') */
    784,    /* State 82: 18 actions (after 'switch ( expr )') */
    802,    /* State 83: 21 actions (reduce P35) */
    823,    /* State 84: 5 actions (after 'case') */
    828,    /* State 85: 2 actions (after 'case expr') */
    830,    /* State 86: 18 actions (after 'case expr :') */
    848,    /* State 87: 21 actions (reduce P36) */
    869,    /* State 88: 1 action (after 'default') */
    870,    /* State 89: 18 actions (after 'default :') */
    888,    /* State 90: 21 actions (reduce P37) */
    909,    /* State 91: 5 actions (after 'declarator (', +IDENTIFIER/INT/CHAR) */
    914,    /* State 92: 6 actions (reduce P38 + BRACE_APERTA) */
    920,    /* State 93: 1 action (after 'declarator ( void') */
    921,    /* State 94: 6 actions (reduce P39 + BRACE_APERTA) */
    927,    /* State 95: 2 actions (after '( type_spec') */
    929,    /* State 96: 2 actions (after '( type_spec *') */
    931,    /* State 97: 2 actions (reduce P12) */
    933,    /* State 98: 2 actions (reduce P12 after *) */
    935,    /* State 99: 2 actions (reduce P43) */
    937,    /* State 100: 2 actions (reduce P11) */
    939,    /* State 101: 2 actions (reduce P41) */
    941,    /* State 102: 2 actions (after param_list) */
    943,    /* State 103: 6 actions (reduce P40 + BRACE_APERTA) */
    949,    /* State 104: 3 actions (after param_list ,, +INT/CHAR) */
    952,    /* State 105: 2 actions (after , type_spec) */
    954,    /* State 106: 2 actions (after , type_spec *) */
    956,    /* State 107: 2 actions (reduce P12) */
    958,    /* State 108: 2 actions (reduce P12 after *) */
    960,    /* State 109: 2 actions (reduce P43) */
    962,    /* State 110: 2 actions (reduce P11) */
    964,    /* State 111: 2 actions (reduce P42) */
    966,    /* State 112: (unused) */
    966,    /* State 113: 1 action (reduce P44 function_definition) */
    967,    /* State 114: 1 action (accept function_definition) */
    968,    /* State 115: (unused) */
    968,    /* State 116: 6 actions (reduce P12 for direct declarator) */
    974,    /* State 117: 2 actions (after 'struct') */
    976,    /* State 118: 4 actions (after 'struct ID') */
    980,    /* State 119: 7 actions (after 'struct {', +UNION, +ENUM) */
    987,    /* State 120: 7 actions (after 'struct ID {', +UNION, +ENUM) */
    994,    /* State 121: 3 actions (after member type_spec, +COLON) */
    997,    /* State 122: 2 actions (after member type_spec *) */
    999,    /* State 123: 2 actions (after member type name, +COLON) */
    1001,   /* State 124: 1 action (after member type * name) */
    1002,   /* State 125: 5 actions (after member ;) */
    1007,   /* State 126: 5 actions (after pointer member ;) */
    1012,   /* State 127: 7 actions (after member_list in anon, +UNION, +ENUM) */
    1019,   /* State 128: 3 actions (after anon struct }) */
    1022,   /* State 129: 7 actions (after member_list in named, +UNION, +ENUM) */
    1029,   /* State 130: 3 actions (after named struct }) */
    1032,   /* State 131: 3 actions (subsequent member type_spec, +COLON) */
    1035,   /* State 132: 2 actions (subsequent member type_spec *) */
    1037,   /* State 133: 2 actions (subsequent member type name, +COLON) */
    1039,   /* State 134: 1 action (subsequent member type * name) */
    1040,   /* State 135: 5 actions (subsequent non-pointer ;) */
    1045,   /* State 136: 5 actions (subsequent pointer ;) */
    1050,   /* State 137: 2 actions (after 'union') */
    1052,   /* State 138: 4 actions (after 'union ID') */
    1056,   /* State 139: 6 actions (after 'union {') */
    1062,   /* State 140: 6 actions (after 'union ID {') */
    1068,   /* State 141: 6 actions (after member_list in anon union) */
    1074,   /* State 142: 3 actions (after anon union }) */
    1077,   /* State 143: 6 actions (after member_list in named union) */
    1083,   /* State 144: 3 actions (after named union }) */
    1086,   /* State 145: 2 actions (after 'enum') */
    1088,   /* State 146: 4 actions (after 'enum ID') */
    1092,   /* State 147: 1 action (after 'enum {') */
    1093,   /* State 148: 1 action (after 'enum ID {') */
    1094,   /* State 149: 3 actions (after enumerator ID) */
    1097,   /* State 150: 5 actions (after 'ID =') */
    1102,   /* State 151: 3 actions (after 'ID = expr', +PLUS) */
    1105,   /* State 152: 2 actions (after first enumerator) */
    1107,   /* State 153: 2 actions (after enum_list in anon) */
    1109,   /* State 154: 3 actions (after 'enum { list }') */
    1112,   /* State 155: 2 actions (after enum_list in named) */
    1114,   /* State 156: 1 action (after ',') */
    1115,   /* State 157: 3 actions (after 'enum ID { list }') */
    1118,   /* State 158: 3 actions (after subsequent ID) */
    1121,   /* State 159: 5 actions (after subsequent 'ID =') */
    1126,   /* State 160: 3 actions (after subsequent 'ID = expr', +PLUS) */
    1129,   /* State 161: 2 actions (after subsequent enumerator) */
    1131,   /* State 162: 5 actions (first named bit field ':' expr) */
    1136,   /* State 163: 2 actions (first named bit field expr done) */
    1138,   /* State 164: 5 actions (reduce P62) */
    1143,   /* State 165: 5 actions (subsequent named bit field ':' expr) */
    1148,   /* State 166: 2 actions (subsequent named bit field expr done) */
    1150,   /* State 167: 5 actions (reduce P63) */
    1155,   /* State 168: 5 actions (first anon bit field ':' expr) */
    1160,   /* State 169: 2 actions (first anon bit field expr done) */
    1162,   /* State 170: 5 actions (reduce P64) */
    1167,   /* State 171: 5 actions (subsequent anon bit field ':' expr) */
    1172,   /* State 172: 2 actions (subsequent anon bit field expr done) */
    1174,   /* State 173: 5 actions (reduce P65) */
    /* ========== NESTED TYPE MEMBER STATES (E5) ========== */
    1179,   /* State 174: 2 actions (after struct_spec first) */
    1181,   /* State 175: 2 actions (after struct_spec * first) */
    1183,   /* State 176: 1 action (after struct_spec ID first) */
    1184,   /* State 177: 1 action (after struct_spec * ID first) */
    1185,   /* State 178: 7 actions (reduce P66) */
    1192,   /* State 179: 7 actions (reduce P67) */
    1199,   /* State 180: 2 actions (after struct_spec subsequent) */
    1201,   /* State 181: 2 actions (after struct_spec * subsequent) */
    1203,   /* State 182: 1 action (after struct_spec ID subsequent) */
    1204,   /* State 183: 1 action (after struct_spec * ID subsequent) */
    1205,   /* State 184: 7 actions (reduce P68) */
    1212,   /* State 185: 7 actions (reduce P69) */
    1219,   /* State 186: 2 actions (after enum_spec first) */
    1221,   /* State 187: 2 actions (after enum_spec * first) */
    1223,   /* State 188: 1 action (after enum_spec ID first) */
    1224,   /* State 189: 1 action (after enum_spec * ID first) */
    1225,   /* State 190: 7 actions (reduce P70) */
    1232,   /* State 191: 7 actions (reduce P71) */
    1239,   /* State 192: 2 actions (after enum_spec subsequent) */
    1241,   /* State 193: 2 actions (after enum_spec * subsequent) */
    1243,   /* State 194: 1 action (after enum_spec ID subsequent) */
    1244,   /* State 195: 1 action (after enum_spec * ID subsequent) */
    1245,   /* State 196: 7 actions (reduce P72) */
    1252,   /* State 197: 7 actions (reduce P73) */
    1259    /* End marker */
};

#define NUM_STATES 198

/* ==================================================
 * GOTO Table
 *
 * After reduce, look up next state based on
 * current state and non-terminal produced.
 * ================================================== */

/* Internal non-terminals for goto */
#define INT_NT_EXPR         0
#define INT_NT_TERM         1
#define INT_NT_FACTOR       2
#define INT_NT_DECLARATIO   3
#define INT_NT_DECLARATOR   4
#define INT_NT_DECLARATION  5
#define INT_NT_SENTENTIA    6
#define INT_NT_CORPUS       7
#define INT_NT_ELENCHUS     8
#define INT_NT_SI           9
#define INT_NT_DUM          10
#define INT_NT_FAC          11
#define INT_NT_PER          12
#define INT_NT_EXPRESSIO_OPT 13
#define INT_NT_PARAM_LIST    14
#define INT_NT_PARAM_DECL    15
#define INT_NT_DEFINITIO     16
#define INT_NT_STRUCT_SPEC   17
#define INT_NT_STRUCT_MEMBERS 18
#define INT_NT_ENUM_SPEC     19
#define INT_NT_ENUM_LIST     20
#define INT_NT_ENUMERATOR    21

hic_manens Arbor2TabulaGoto GOTO_TABULA[] = {
    /* From state 0 */
    { 0, INT_NT_EXPR,   1 },
    { 0, INT_NT_TERM,   2 },
    { 0, INT_NT_FACTOR, 3 },

    /* From state 6: after '(' */
    { 6, INT_NT_EXPR,   11 },
    { 6, INT_NT_TERM,   2 },
    { 6, INT_NT_FACTOR, 3 },

    /* From state 7: after unary '*' */
    { 7, INT_NT_FACTOR, 15 },

    /* From state 8: after unary '&' */
    { 8, INT_NT_FACTOR, 16 },

    /* From state 9: after expr '+' */
    { 9, INT_NT_TERM,   13 },
    { 9, INT_NT_FACTOR, 3 },

    /* From state 10: after term '*' */
    { 10, INT_NT_FACTOR, 14 },

    /* From state 4: after identifier as type_specifier */
    { 4, INT_NT_DECLARATOR, 20 },

    /* From state 17: after '*' in declarator */
    { 17, INT_NT_DECLARATOR, 19 },

    /* From state 0: after declaration */
    { 0, INT_NT_DECLARATIO, 21 },

    /* From state 0: after statement */
    { 0, INT_NT_SENTENTIA, 24 },

    /* From state 0: after compound_statement */
    { 0, INT_NT_CORPUS, 29 },

    /* From state 25: after epsilon reduce to empty list */
    { 25, INT_NT_ELENCHUS, 26 },

    /* From state 26: inside compound, after expression components */
    { 26, INT_NT_EXPR,      1 },    /* reuse expression state */
    { 26, INT_NT_TERM,      2 },    /* reuse term state */
    { 26, INT_NT_FACTOR,    3 },    /* reuse factor state */
    { 26, INT_NT_SENTENTIA, 28 },   /* statement in list */
    { 26, INT_NT_CORPUS,    29 },   /* nested compound */
    { 26, INT_NT_SI,        37 },   /* if statement in list */

    /* From state 0: after if_statement */
    { 0, INT_NT_SI,         37 },

    /* From state 31: after 'if (' - expression components */
    { 31, INT_NT_EXPR,      32 },   /* condition expression */
    { 31, INT_NT_TERM,      2 },    /* reuse term state */
    { 31, INT_NT_FACTOR,    3 },    /* reuse factor state */

    /* From state 33: after 'if ( expr )' - then-branch statement */
    { 33, INT_NT_EXPR,      1 },    /* expression in then-branch */
    { 33, INT_NT_TERM,      2 },    /* term in expression */
    { 33, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 33, INT_NT_SENTENTIA, 34 },   /* then-branch statement */
    { 33, INT_NT_CORPUS,    38 },   /* nested compound - reduce P15 first */
    { 33, INT_NT_SI,        37 },   /* nested if statement */

    /* From state 35: after 'if ( expr ) stmt else' - else-branch statement */
    { 35, INT_NT_EXPR,      1 },    /* expression in else-branch */
    { 35, INT_NT_TERM,      2 },    /* term in expression */
    { 35, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 35, INT_NT_SENTENTIA, 36 },   /* else-branch statement */
    { 35, INT_NT_CORPUS,    38 },   /* nested compound - reduce P15 first */
    { 35, INT_NT_SI,        37 },   /* nested if statement in else-branch */
    { 35, INT_NT_DUM,       44 },   /* while in else-branch */
    { 35, INT_NT_FAC,       52 },   /* do-while in else-branch */

    /* ==================================================
     * WHILE/DO-WHILE GOTO Entries
     * ================================================== */

    /* From state 0: after while_statement */
    { 0, INT_NT_DUM,        44 },   /* while_statement reduces to P22 */
    { 0, INT_NT_FAC,        52 },   /* do_statement reduces to P24 */

    /* From state 26: while/do in compound statement list */
    { 26, INT_NT_DUM,       44 },   /* while_statement in list */
    { 26, INT_NT_FAC,       52 },   /* do_statement in list */

    /* From state 33: while/do as if then-branch */
    { 33, INT_NT_DUM,       44 },   /* while as then-branch */
    { 33, INT_NT_FAC,       52 },   /* do-while as then-branch */

    /* From state 40: after 'while (' - condition expression */
    { 40, INT_NT_EXPR,      41 },   /* condition expression */
    { 40, INT_NT_TERM,      2 },    /* term in expression */
    { 40, INT_NT_FACTOR,    3 },    /* factor in expression */

    /* From state 42: after 'while ( expr )' - loop body */
    { 42, INT_NT_EXPR,      1 },    /* expression in body */
    { 42, INT_NT_TERM,      2 },    /* term in expression */
    { 42, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 42, INT_NT_SENTENTIA, 43 },   /* loop body statement */
    { 42, INT_NT_CORPUS,    38 },   /* compound body */
    { 42, INT_NT_SI,        37 },   /* if in body */
    { 42, INT_NT_DUM,       44 },   /* nested while */
    { 42, INT_NT_FAC,       52 },   /* nested do-while */

    /* From state 45: after 'do' - loop body */
    { 45, INT_NT_EXPR,      1 },    /* expression in body */
    { 45, INT_NT_TERM,      2 },    /* term in expression */
    { 45, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 45, INT_NT_SENTENTIA, 46 },   /* loop body statement */
    { 45, INT_NT_CORPUS,    38 },   /* compound body */
    { 45, INT_NT_SI,        37 },   /* if in body */
    { 45, INT_NT_DUM,       44 },   /* nested while */
    { 45, INT_NT_FAC,       52 },   /* nested do-while */

    /* From state 48: after 'do stmt while (' - condition expression */
    { 48, INT_NT_EXPR,      49 },   /* condition expression */
    { 48, INT_NT_TERM,      2 },    /* term in expression */
    { 48, INT_NT_FACTOR,    3 },    /* factor in expression */

    /* ==================================================
     * FOR GOTO Entries
     * ================================================== */

    /* From state 0: after for_statement */
    { 0, INT_NT_PER,        65 },   /* for_statement reduces to P26 */

    /* From state 26: for in compound statement list */
    { 26, INT_NT_PER,       65 },   /* for_statement in list */

    /* From state 33: for as if then-branch */
    { 33, INT_NT_PER,       65 },   /* for as then-branch */

    /* From state 35: for as else-branch */
    { 35, INT_NT_PER,       65 },   /* for in else-branch */

    /* From state 42: for as while body */
    { 42, INT_NT_PER,       65 },   /* for as while body */

    /* From state 45: for as do body */
    { 45, INT_NT_PER,       65 },   /* for as do body */

    /* From state 54: after 'for (' - expression components */
    { 54, INT_NT_EXPR,              55 },   /* init expression */
    { 54, INT_NT_TERM,              2 },    /* term in expression */
    { 54, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 54, INT_NT_EXPRESSIO_OPT,     56 },   /* expr_opt (init) */

    /* From state 57: after 'for ( expr_opt ;' - condition expression */
    { 57, INT_NT_EXPR,              58 },   /* condition expression */
    { 57, INT_NT_TERM,              2 },    /* term in expression */
    { 57, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 57, INT_NT_EXPRESSIO_OPT,     59 },   /* expr_opt (condition) */

    /* From state 60: after 'for ( ... ;' - increment expression */
    { 60, INT_NT_EXPR,              61 },   /* increment expression */
    { 60, INT_NT_TERM,              2 },    /* term in expression */
    { 60, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 60, INT_NT_EXPRESSIO_OPT,     62 },   /* expr_opt (increment) */

    /* From state 63: after 'for ( ... )' - loop body */
    { 63, INT_NT_EXPR,              1 },    /* expression in body */
    { 63, INT_NT_TERM,              2 },    /* term in expression */
    { 63, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 63, INT_NT_SENTENTIA,         64 },   /* loop body statement */
    { 63, INT_NT_CORPUS,            38 },   /* compound body */
    { 63, INT_NT_SI,                37 },   /* if in body */
    { 63, INT_NT_DUM,               44 },   /* while in body */
    { 63, INT_NT_FAC,               52 },   /* do-while in body */
    { 63, INT_NT_PER,               65 },   /* nested for */

    /* ==================================================
     * JUMP STATEMENT GOTO Entries
     * ================================================== */

    /* From state 70: after 'return' - expression for return value */
    { 70, INT_NT_EXPR,              71 },   /* return expression */
    { 70, INT_NT_TERM,              2 },    /* term in expression */
    { 70, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 70, INT_NT_EXPRESSIO_OPT,     72 },   /* expr_opt (return value) */

    /* From state 77: after 'IDENTIFIER :' - labeled statement body */
    { 77, INT_NT_EXPR,              1 },    /* expression in label body */
    { 77, INT_NT_TERM,              2 },    /* term in expression */
    { 77, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 77, INT_NT_SENTENTIA,         78 },   /* labeled statement */
    { 77, INT_NT_CORPUS,            38 },   /* compound body */
    { 77, INT_NT_SI,                37 },   /* if in body */
    { 77, INT_NT_DUM,               44 },   /* while in body */
    { 77, INT_NT_FAC,               52 },   /* do-while in body */
    { 77, INT_NT_PER,               65 },   /* for in body */

    /* ==================================================
     * SWITCH/CASE/DEFAULT GOTO Entries
     * ================================================== */

    /* From state 80: after 'switch (' - expression components */
    { 80, INT_NT_EXPR,              81 },   /* switch condition expression */
    { 80, INT_NT_TERM,              2 },    /* term in expression */
    { 80, INT_NT_FACTOR,            3 },    /* factor in expression */

    /* From state 82: after 'switch ( expr )' - body statement */
    { 82, INT_NT_EXPR,              1 },    /* expression in body */
    { 82, INT_NT_TERM,              2 },    /* term in expression */
    { 82, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 82, INT_NT_SENTENTIA,         83 },   /* switch body statement */
    { 82, INT_NT_CORPUS,            38 },   /* compound body */
    { 82, INT_NT_SI,                37 },   /* if in body */
    { 82, INT_NT_DUM,               44 },   /* while in body */
    { 82, INT_NT_FAC,               52 },   /* do-while in body */
    { 82, INT_NT_PER,               65 },   /* for in body */

    /* From state 84: after 'case' - expression components */
    { 84, INT_NT_EXPR,              85 },   /* case expression */
    { 84, INT_NT_TERM,              2 },    /* term in expression */
    { 84, INT_NT_FACTOR,            3 },    /* factor in expression */

    /* From state 86: after 'case expr :' - body statement */
    { 86, INT_NT_EXPR,              1 },    /* expression in body */
    { 86, INT_NT_TERM,              2 },    /* term in expression */
    { 86, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 86, INT_NT_SENTENTIA,         87 },   /* case body statement */
    { 86, INT_NT_CORPUS,            38 },   /* compound body */
    { 86, INT_NT_SI,                37 },   /* if in body */
    { 86, INT_NT_DUM,               44 },   /* while in body */
    { 86, INT_NT_FAC,               52 },   /* do-while in body */
    { 86, INT_NT_PER,               65 },   /* for in body */

    /* From state 89: after 'default :' - body statement */
    { 89, INT_NT_EXPR,              1 },    /* expression in body */
    { 89, INT_NT_TERM,              2 },    /* term in expression */
    { 89, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 89, INT_NT_SENTENTIA,         90 },   /* default body statement */
    { 89, INT_NT_CORPUS,            38 },   /* compound body */
    { 89, INT_NT_SI,                37 },   /* if in body */
    { 89, INT_NT_DUM,               44 },   /* while in body */
    { 89, INT_NT_FAC,               52 },   /* do-while in body */
    { 89, INT_NT_PER,               65 },   /* for in body */

    /* Phase D2: Parameter list GOTO entries */
    { 95, INT_NT_DECLARATOR,        99 },   /* After type_spec + declarator */
    { 96, INT_NT_DECLARATOR,        100 },  /* After type_spec * + declarator */
    { 91, INT_NT_PARAM_DECL,        101 },  /* After param_decl */
    { 91, INT_NT_PARAM_LIST,        102 },  /* After param_list */
    { 105, INT_NT_DECLARATOR,       109 },  /* After , type_spec + declarator */
    { 106, INT_NT_DECLARATOR,       110 },  /* After , type_spec * + declarator */
    { 104, INT_NT_PARAM_DECL,       111 },  /* After , + param_decl */

    /* Phase D3: Function definition GOTO entries */
    { 20, INT_NT_CORPUS,            113 },  /* After declarator + compound body */
    { 0, INT_NT_DEFINITIO,          114 },  /* After function_definition */

    /* ==================================================
     * STRUCT GOTO Entries
     * ================================================== */

    /* From state 0: struct_specifier acts as type_specifier */
    { 0, INT_NT_STRUCT_SPEC,        4 },    /* struct_specifier → declarator path */

    /* From state 119: anonymous struct member list */
    { 119, INT_NT_STRUCT_MEMBERS,   127 },  /* member_list accumulation */

    /* From state 120: named struct member list */
    { 120, INT_NT_STRUCT_MEMBERS,   129 },  /* member_list accumulation */

    /* From state 127: more members in anonymous struct */
    { 127, INT_NT_STRUCT_MEMBERS,   127 },  /* append member to list (P49) */

    /* From state 129: more members in named struct */
    { 129, INT_NT_STRUCT_MEMBERS,   129 },  /* append member to list (P49) */

    /* Nested type specifiers as struct member types (Phase E5) */
    /* First member context (from states 119/120) */
    { 119, INT_NT_STRUCT_SPEC,      174 },  /* nested struct/union → first member path */
    { 119, INT_NT_ENUM_SPEC,        186 },  /* nested enum → first member path */
    { 120, INT_NT_STRUCT_SPEC,      174 },  /* nested struct/union → first member path */
    { 120, INT_NT_ENUM_SPEC,        186 },  /* nested enum → first member path */

    /* Subsequent member context (from states 127/129) */
    { 127, INT_NT_STRUCT_SPEC,      180 },  /* nested struct/union → subsequent member path */
    { 127, INT_NT_ENUM_SPEC,        192 },  /* nested enum → subsequent member path */
    { 129, INT_NT_STRUCT_SPEC,      180 },  /* nested struct/union → subsequent member path */
    { 129, INT_NT_ENUM_SPEC,        192 },  /* nested enum → subsequent member path */

    /* ==================================================
     * UNION GOTO Entries
     * ================================================== */

    /* From state 139: anonymous union member list */
    { 139, INT_NT_STRUCT_MEMBERS,   141 },  /* member_list accumulation */

    /* From state 140: named union member list */
    { 140, INT_NT_STRUCT_MEMBERS,   143 },  /* member_list accumulation */

    /* From state 141: more members in anonymous union */
    { 141, INT_NT_STRUCT_MEMBERS,   141 },  /* append member to list */

    /* From state 143: more members in named union */
    { 143, INT_NT_STRUCT_MEMBERS,   143 },  /* append member to list */

    /* Nested type specifiers as union member types (Phase E5) */
    /* First member context (from states 139/140) */
    { 139, INT_NT_STRUCT_SPEC,      174 },  /* nested struct/union → first member path */
    { 139, INT_NT_ENUM_SPEC,        186 },  /* nested enum → first member path */
    { 140, INT_NT_STRUCT_SPEC,      174 },  /* nested struct/union → first member path */
    { 140, INT_NT_ENUM_SPEC,        186 },  /* nested enum → first member path */

    /* Subsequent member context (from states 141/143) */
    { 141, INT_NT_STRUCT_SPEC,      180 },  /* nested struct/union → subsequent member path */
    { 141, INT_NT_ENUM_SPEC,        192 },  /* nested enum → subsequent member path */
    { 143, INT_NT_STRUCT_SPEC,      180 },  /* nested struct/union → subsequent member path */
    { 143, INT_NT_ENUM_SPEC,        192 },  /* nested enum → subsequent member path */

    /* ==================================================
     * ENUM GOTO Entries
     * ================================================== */

    /* From state 0: enum_specifier acts as type_specifier */
    { 0, INT_NT_ENUM_SPEC,          4 },    /* enum_specifier → declarator path */

    /* From state 147: anonymous enum - enumerator and list */
    { 147, INT_NT_ENUMERATOR,       152 },  /* first enumerator */
    { 147, INT_NT_ENUM_LIST,        153 },  /* enumerator_list */

    /* From state 148: named enum - enumerator and list */
    { 148, INT_NT_ENUMERATOR,       152 },  /* first enumerator */
    { 148, INT_NT_ENUM_LIST,        155 },  /* enumerator_list */

    /* From state 150: expression for enumerator value */
    { 150, INT_NT_EXPR,             151 },  /* value expression done */
    { 150, INT_NT_TERM,             2 },
    { 150, INT_NT_FACTOR,           3 },

    /* From state 156: subsequent enumerator after comma */
    { 156, INT_NT_ENUMERATOR,       161 },  /* subsequent enumerator */

    /* From state 159: expression for subsequent enumerator value */
    { 159, INT_NT_EXPR,             160 },  /* value expression done */
    { 159, INT_NT_TERM,             2 },
    { 159, INT_NT_FACTOR,           3 },

    /* ========== BIT FIELD EXPRESSION GOTO ========== */

    /* From state 162: expression for first named bit field */
    { 162, INT_NT_EXPR,             163 },  /* bit field width done */
    { 162, INT_NT_TERM,             2 },
    { 162, INT_NT_FACTOR,           3 },

    /* From state 165: expression for subsequent named bit field */
    { 165, INT_NT_EXPR,             166 },  /* bit field width done */
    { 165, INT_NT_TERM,             2 },
    { 165, INT_NT_FACTOR,           3 },

    /* From state 168: expression for first anonymous bit field */
    { 168, INT_NT_EXPR,             169 },  /* bit field width done */
    { 168, INT_NT_TERM,             2 },
    { 168, INT_NT_FACTOR,           3 },

    /* From state 171: expression for subsequent anonymous bit field */
    { 171, INT_NT_EXPR,             172 },  /* bit field width done */
    { 171, INT_NT_TERM,             2 },
    { 171, INT_NT_FACTOR,           3 }
};

hic_manens i32 NUM_GOTO = (i32)(magnitudo(GOTO_TABULA) / magnitudo(GOTO_TABULA[0]));

/* ==================================================
 * Table Lookup Functions
 * ================================================== */

Xar*
arbor2_glr_quaerere_actiones(
    Arbor2GLR*          glr,
    i32                 status,
    Arbor2LexemaGenus   lexema)
{
    Xar* resultus;
    i32 initium;
    i32 finis;
    i32 i;

    resultus = xar_creare(glr->piscina, magnitudo(Arbor2TabulaActio*));

    si (status < ZEPHYRUM || status >= NUM_STATES)
    {
        redde resultus;  /* Empty = error */
    }

    initium = ACTIO_INDICES[status];
    finis = ACTIO_INDICES[status + I];

    per (i = initium; i < finis; i++)
    {
        si (ACTIONES[i].lexema == lexema)
        {
            Arbor2TabulaActio** slot = xar_addere(resultus);
            *slot = &ACTIONES[i];
        }
    }

    redde resultus;
}

s32
arbor2_glr_quaerere_goto(
    Arbor2GLR*          glr,
    s32                 status,
    Arbor2NonTerminalis nt)
{
    i32 i;
    s32 nt_int;

    /* Map external NT enum to internal index */
    commutatio (nt)
    {
        casus ARBOR2_NT_EXPRESSIO:
            nt_int = INT_NT_EXPR;
            frange;
        casus ARBOR2_NT_TERMINUS:
            nt_int = INT_NT_TERM;
            frange;
        casus ARBOR2_NT_FACTOR:
            nt_int = INT_NT_FACTOR;
            frange;
        casus ARBOR2_NT_DECLARATIO:
            nt_int = INT_NT_DECLARATIO;
            frange;
        casus ARBOR2_NT_DECLARATOR:
            nt_int = INT_NT_DECLARATOR;
            frange;
        casus ARBOR2_NT_SENTENTIA:
            nt_int = INT_NT_SENTENTIA;
            frange;
        casus ARBOR2_NT_CORPUS:
            nt_int = INT_NT_CORPUS;
            frange;
        casus ARBOR2_NT_ELENCHUS_SENTENTIARUM:
            nt_int = INT_NT_ELENCHUS;
            frange;
        casus ARBOR2_NT_SI:
            nt_int = INT_NT_SI;
            frange;
        casus ARBOR2_NT_DUM:
            nt_int = INT_NT_DUM;
            frange;
        casus ARBOR2_NT_FAC:
            nt_int = INT_NT_FAC;
            frange;
        casus ARBOR2_NT_PER:
            nt_int = INT_NT_PER;
            frange;
        casus ARBOR2_NT_EXPRESSIO_OPTATIVA:
            nt_int = INT_NT_EXPRESSIO_OPT;
            frange;
        casus ARBOR2_NT_PARAMETER_LIST:
            nt_int = INT_NT_PARAM_LIST;
            frange;
        casus ARBOR2_NT_PARAMETER_DECL:
            nt_int = INT_NT_PARAM_DECL;
            frange;
        casus ARBOR2_NT_DEFINITIO_FUNCTI:
            nt_int = INT_NT_DEFINITIO;
            frange;
        casus ARBOR2_NT_STRUCT_SPECIFIER:
            nt_int = INT_NT_STRUCT_SPEC;
            frange;
        casus ARBOR2_NT_STRUCT_MEMBER_LIST:
            nt_int = INT_NT_STRUCT_MEMBERS;
            frange;
        casus ARBOR2_NT_ENUM_SPECIFIER:
            nt_int = INT_NT_ENUM_SPEC;
            frange;
        casus ARBOR2_NT_ENUMERATOR_LIST:
            nt_int = INT_NT_ENUM_LIST;
            frange;
        casus ARBOR2_NT_ENUMERATOR:
            nt_int = INT_NT_ENUMERATOR;
            frange;
        ordinarius:
            nt_int = -I;
            frange;
    }

    si (nt_int < ZEPHYRUM)
    {
        redde -I;  /* Error */
    }

    per (i = ZEPHYRUM; i < NUM_GOTO; i++)
    {
        si (GOTO_TABULA[i].status == status &&
            GOTO_TABULA[i].non_terminalis == nt_int)
        {
            redde GOTO_TABULA[i].status_novus;
        }
    }

    redde -I;  /* Not found = error */

    (vacuum)glr;  /* Unused for now */
}

Arbor2Regula*
arbor2_glr_obtinere_regula(
    Arbor2GLR*          glr,
    i32                 index)
{
    si (index < ZEPHYRUM || index >= NUM_REGULAE)
    {
        redde NIHIL;
    }

    (vacuum)glr;  /* Unused */
    redde &REGULAE[index];
}

vacuum
arbor2_glr_initializare_tabulas(Arbor2GLR* glr)
{
    glr->actiones = ACTIONES;
    glr->num_actiones = NUM_ACTIONES;
    glr->goto_tabula = GOTO_TABULA;
    glr->num_goto = NUM_GOTO;
    glr->regulae = REGULAE;
    glr->num_regulae = NUM_REGULAE;
}

/* ==================================================
 * Name Functions
 * ================================================== */

constans character*
arbor2_actio_genus_nomen(Arbor2ActioGenus genus)
{
    commutatio (genus)
    {
        casus ARBOR2_ACTIO_SHIFT:  redde "SHIFT";
        casus ARBOR2_ACTIO_REDUCE: redde "REDUCE";
        casus ARBOR2_ACTIO_ACCEPT: redde "ACCEPT";
        casus ARBOR2_ACTIO_ERROR:  redde "ERROR";
        ordinarius:                redde "IGNOTUM";
    }
}

constans character*
arbor2_nt_nomen(Arbor2NonTerminalis nt)
{
    commutatio (nt)
    {
        casus ARBOR2_NT_EXPRESSIO:      redde "EXPRESSIO";
        casus ARBOR2_NT_TERMINUS:       redde "TERMINUS";
        casus ARBOR2_NT_FACTOR:         redde "FACTOR";
        casus ARBOR2_NT_PRIMARIUM:      redde "PRIMARIUM";
        casus ARBOR2_NT_UNARIUM:        redde "UNARIUM";
        casus ARBOR2_NT_BINARIUM:       redde "BINARIUM";
        casus ARBOR2_NT_NOMEN_TYPI:     redde "NOMEN_TYPI";
        casus ARBOR2_NT_SPECIFIER_TYPI: redde "SPECIFIER_TYPI";
        casus ARBOR2_NT_DECLARATIO:     redde "DECLARATIO";
        casus ARBOR2_NT_DECLARATOR:     redde "DECLARATOR";
        casus ARBOR2_NT_CONVERSIO:      redde "CONVERSIO";
        casus ARBOR2_NT_SIZEOF:         redde "SIZEOF";
        casus ARBOR2_NT_SENTENTIA:      redde "SENTENTIA";
        casus ARBOR2_NT_CORPUS:         redde "CORPUS";
        casus ARBOR2_NT_ELENCHUS_SENTENTIARUM: redde "ELENCHUS_SENTENTIARUM";
        casus ARBOR2_NT_SI:             redde "SI";
        casus ARBOR2_NT_DUM:            redde "DUM";
        casus ARBOR2_NT_FAC:            redde "FAC";
        casus ARBOR2_NT_PER:            redde "PER";
        casus ARBOR2_NT_EXPRESSIO_OPTATIVA: redde "EXPRESSIO_OPTATIVA";
        casus ARBOR2_NT_STRUCT_SPECIFIER:   redde "STRUCT_SPECIFIER";
        casus ARBOR2_NT_STRUCT_MEMBER_LIST: redde "STRUCT_MEMBER_LIST";
        ordinarius:                     redde "IGNOTUM";
    }
}

constans character*
arbor2_nodus_genus_nomen(Arbor2NodusGenus genus)
{
    commutatio (genus)
    {
        casus ARBOR2_NODUS_IDENTIFICATOR: redde "IDENTIFICATOR";
        casus ARBOR2_NODUS_INTEGER:       redde "INTEGER";
        casus ARBOR2_NODUS_BINARIUM:      redde "BINARIUM";
        casus ARBOR2_NODUS_UNARIUM:       redde "UNARIUM";
        casus ARBOR2_NODUS_CONVERSIO:     redde "CONVERSIO";
        casus ARBOR2_NODUS_SIZEOF:        redde "SIZEOF";
        casus ARBOR2_NODUS_DECLARATIO:    redde "DECLARATIO";
        casus ARBOR2_NODUS_DECLARATOR:    redde "DECLARATOR";
        casus ARBOR2_NODUS_DECLARATOR_FUNCTI: redde "DECLARATOR_FUNCTI";
        casus ARBOR2_NODUS_PARAMETER_DECL:    redde "PARAMETER_DECL";
        casus ARBOR2_NODUS_SENTENTIA:     redde "SENTENTIA";
        casus ARBOR2_NODUS_SENTENTIA_VACUA: redde "SENTENTIA_VACUA";
        casus ARBOR2_NODUS_CORPUS:        redde "CORPUS";
        casus ARBOR2_NODUS_SI:            redde "SI";
        casus ARBOR2_NODUS_DUM:           redde "DUM";
        casus ARBOR2_NODUS_FAC:           redde "FAC";
        casus ARBOR2_NODUS_PER:           redde "PER";
        casus ARBOR2_NODUS_FRANGE:        redde "FRANGE";
        casus ARBOR2_NODUS_PERGE:         redde "PERGE";
        casus ARBOR2_NODUS_REDDE:         redde "REDDE";
        casus ARBOR2_NODUS_SALTA:         redde "SALTA";
        casus ARBOR2_NODUS_TITULATUM:     redde "TITULATUM";
        casus ARBOR2_NODUS_COMMUTATIO:    redde "COMMUTATIO";
        casus ARBOR2_NODUS_CASUS:         redde "CASUS";
        casus ARBOR2_NODUS_ORDINARIUS:    redde "ORDINARIUS";
        casus ARBOR2_NODUS_AMBIGUUS:      redde "AMBIGUUS";
        casus ARBOR2_NODUS_STRUCT_SPECIFIER: redde "STRUCT_SPECIFIER";
        casus ARBOR2_NODUS_ERROR:         redde "ERROR";
        ordinarius:                       redde "IGNOTUM";
    }
}
