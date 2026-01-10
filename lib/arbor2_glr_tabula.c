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
    { ARBOR2_NT_SI, 7, ARBOR2_NODUS_SI }
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
    /* State 0: Initial - expect factor-starting tokens, ';', '{', or 'if' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },   /* empty statement */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },   /* compound statement */
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },   /* if statement */

    /* State 1: After expression - expect '+', ';', or end */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 22 },   /* expression statement */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 0 },  /* for nested */

    /* State 2: After term - expect '*', '+', ';', '}', or end */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 2 },  /* expression -> term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 2 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },

    /* State 3: After factor - reduce to term */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 4 },  /* term -> factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 4 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },

    /* State 4: After IDENTIFIER - AMBIGUOUS for '*' (expr vs decl) */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 5 },  /* expr: factor -> ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },  /* decl: start declarator */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 5 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },

    /* State 5: After INTEGER - reduce to factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 6 },  /* factor -> INT */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 6 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },

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

    /* State 13: After expression '+' term - reduce or continue */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },  /* * binds tighter */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 1 },   /* expr -> expr + term */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 1 },   /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },   /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },

    /* State 14: After term '*' factor - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 3 },  /* term -> term * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 3 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },

    /* State 15: After '*' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 8 },  /* factor -> * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 8 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },

    /* State 16: After '&' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 9 },  /* factor -> & factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 9 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },

    /* State 17: After '*' in declarator - expect ID or more '*' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* declarator name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* more pointers */

    /* State 18: After IDENTIFIER in declarator - reduce P12 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12 },  /* declarator -> ID */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },

    /* State 19: After '* declarator' - reduce P11 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 11 },  /* declarator -> * declarator */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11 },

    /* State 20: After 'type_specifier declarator' - reduce P10 */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 10 },  /* declaration -> type declarator */
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

    /* State 35: After 'if ( expr ) stmt else' - expect statement */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 23 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT, 30 },

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

    /* State 38: Nested compound_statement - always reduce P15 (never accept) */
    /* Used for compound statements inside if/else/while bodies */
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
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 15 }
};

hic_manens i32 NUM_ACTIONES = (i32)(magnitudo(ACTIONES) / magnitudo(ACTIONES[0]));

/* State -> first action index mapping */
hic_manens i32 ACTIO_INDICES[] = {
    0,      /* State 0: 8 actions (ID, INT, (, *, &, ;, {, if) */
    8,      /* State 1: 4 actions */
    12,     /* State 2: 6 actions */
    18,     /* State 3: 6 actions */
    24,     /* State 4: 7 actions */
    31,     /* State 5: 6 actions */
    37,     /* State 6: 5 actions */
    42,     /* State 7: 5 actions */
    47,     /* State 8: 5 actions */
    52,     /* State 9: 5 actions */
    57,     /* State 10: 5 actions */
    62,     /* State 11: 2 actions */
    64,     /* State 12: 6 actions */
    70,     /* State 13: 6 actions */
    76,     /* State 14: 6 actions */
    82,     /* State 15: 6 actions */
    88,     /* State 16: 6 actions */
    94,     /* State 17: 2 actions (declarator path) */
    96,     /* State 18: 4 actions (reduce P12) */
    100,    /* State 19: 4 actions (reduce P11) */
    104,    /* State 20: 4 actions (reduce P10) */
    108,    /* State 21: 1 action (accept declaration) */
    109,    /* State 22: 11 actions (reduce P13 + if/else) */
    120,    /* State 23: 11 actions (reduce P14 + if/else) */
    131,    /* State 24: 1 action (accept statement) */
    132,    /* State 25: 9 actions (epsilon reduce P18 + if) */
    141,    /* State 26: 9 actions (inside compound + if) */
    150,    /* State 27: 11 actions (reduce P16 + if/else) */
    161,    /* State 28: 9 actions (reduce P17 + if) */
    170,    /* State 29: 11 actions (accept/reduce P15 + if/else) */
    181,    /* State 30: 1 action (after 'if') */
    182,    /* State 31: 5 actions (after 'if (') */
    187,    /* State 32: 2 actions (after 'if ( expr') */
    189,    /* State 33: 8 actions (after 'if ( expr )') */
    197,    /* State 34: 11 actions (after 'if ( expr ) stmt') */
    208,    /* State 35: 8 actions (after 'if ( expr ) stmt else') */
    216,    /* State 36: 11 actions (reduce P21) */
    227,    /* State 37: 11 actions (reduce P19) */
    238,    /* State 38: 11 actions (reduce P15 for nested compound) */
    249     /* End marker */
};

#define NUM_STATES 39

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
    { 35, INT_NT_SI,        37 }    /* nested if statement in else-branch */
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
        casus ARBOR2_NODUS_SENTENTIA:     redde "SENTENTIA";
        casus ARBOR2_NODUS_SENTENTIA_VACUA: redde "SENTENTIA_VACUA";
        casus ARBOR2_NODUS_CORPUS:        redde "CORPUS";
        casus ARBOR2_NODUS_SI:            redde "SI";
        casus ARBOR2_NODUS_AMBIGUUS:      redde "AMBIGUUS";
        casus ARBOR2_NODUS_ERROR:         redde "ERROR";
        ordinarius:                       redde "IGNOTUM";
    }
}
