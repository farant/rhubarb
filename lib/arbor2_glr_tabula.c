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
    { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM }
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
    /* State 0: Initial - expect factor-starting tokens */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 1: After expression - expect '+' or end */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 0 },  /* for nested */

    /* State 2: After term - expect '*', '+', or end */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 2 },  /* expression -> term */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },

    /* State 3: After factor - reduce to term */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 4 },  /* term -> factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },

    /* State 4: After IDENTIFIER - reduce to factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 5 },  /* factor -> ID */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },

    /* State 5: After INTEGER - reduce to factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 6 },  /* factor -> INT */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 6 },
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
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 7 },

    /* State 13: After expression '+' term - reduce or continue */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },  /* * binds tighter */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 1 },   /* expr -> expr + term */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },

    /* State 14: After term '*' factor - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 3 },  /* term -> term * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },

    /* State 15: After '*' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 8 },  /* factor -> * factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },

    /* State 16: After '&' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 9 },  /* factor -> & factor */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 }
};

hic_manens i32 NUM_ACTIONES = (i32)(magnitudo(ACTIONES) / magnitudo(ACTIONES[0]));

/* State -> first action index mapping */
hic_manens i32 ACTIO_INDICES[] = {
    0,      /* State 0: 5 actions */
    5,      /* State 1: 3 actions */
    8,      /* State 2: 4 actions */
    12,     /* State 3: 4 actions */
    16,     /* State 4: 4 actions */
    20,     /* State 5: 4 actions */
    24,     /* State 6: 5 actions */
    29,     /* State 7: 5 actions */
    34,     /* State 8: 5 actions */
    39,     /* State 9: 5 actions */
    44,     /* State 10: 5 actions */
    49,     /* State 11: 2 actions */
    51,     /* State 12: 4 actions */
    55,     /* State 13: 4 actions */
    59,     /* State 14: 4 actions */
    63,     /* State 15: 4 actions */
    67,     /* State 16: 4 actions */
    71      /* End marker */
};

#define NUM_STATES 17

/* ==================================================
 * GOTO Table
 *
 * After reduce, look up next state based on
 * current state and non-terminal produced.
 * ================================================== */

/* Internal non-terminals for goto */
#define INT_NT_EXPR     0
#define INT_NT_TERM     1
#define INT_NT_FACTOR   2

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
    { 10, INT_NT_FACTOR, 14 }
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
        casus ARBOR2_NT_CONVERSIO:      redde "CONVERSIO";
        casus ARBOR2_NT_SIZEOF:         redde "SIZEOF";
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
        casus ARBOR2_NODUS_AMBIGUUS:      redde "AMBIGUUS";
        casus ARBOR2_NODUS_ERROR:         redde "ERROR";
        ordinarius:                       redde "IGNOTUM";
    }
}
