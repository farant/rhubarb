/* arbor2_glr_tabula.c - Hand-written LR tables for expression grammar */

/* Temporarily suppress missing-field-initializers during refactor.
 * TODO: Remove this pragma once Phase 3 (array-of-arrays) is complete. */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#endif

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
    /* P0 */  { ARBOR2_NT_EXPRESSIO, 1, ARBOR2_NODUS_ERROR, "S' -> expr" },
    /* P1 */  { ARBOR2_NT_EXPRESSIO, 3, ARBOR2_NODUS_BINARIUM, "expr -> expr '+' term" },
    /* P2 */  { ARBOR2_NT_EXPRESSIO, 1, ARBOR2_NODUS_ERROR, "expr -> term" },
    /* P3 */  { ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM, "term -> term '*' factor" },
    /* P4 */  { ARBOR2_NT_TERMINUS, 1, ARBOR2_NODUS_ERROR, "term -> factor" },
    /* P5 */  { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_IDENTIFICATOR, "factor -> ID" },
    /* P6 */  { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_INTEGER, "factor -> INT" },
    /* P7 */  { ARBOR2_NT_FACTOR, 3, ARBOR2_NODUS_ERROR, "factor -> '(' expr ')'" },
    /* P8 */  { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '*' factor" },
    /* P9 */  { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '&' factor" },
    /* P10 */ { ARBOR2_NT_DECLARATIO, 2, ARBOR2_NODUS_DECLARATIO, "decl -> type declarator" },
    /* P11 */ { ARBOR2_NT_DECLARATOR, 2, ARBOR2_NODUS_DECLARATOR, "declarator -> '*' declarator" },
    /* P12 */ { ARBOR2_NT_DECLARATOR, 1, ARBOR2_NODUS_DECLARATOR, "declarator -> ID" },
    /* P13 */ { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_SENTENTIA, "stmt -> expr ';'" },
    /* P14 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_SENTENTIA_VACUA, "stmt -> ';'" },
    /* P15 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR, "stmt -> compound" },
    /* P16 */ { ARBOR2_NT_CORPUS, 3, ARBOR2_NODUS_CORPUS, "compound -> '{' stmts '}'" },
    /* P17 */ { ARBOR2_NT_ELENCHUS_SENTENTIARUM, 2, ARBOR2_NODUS_CORPUS, "stmts -> stmts stmt" },
    /* P18 */ { ARBOR2_NT_ELENCHUS_SENTENTIARUM, 0, ARBOR2_NODUS_CORPUS, "stmts -> e" },
    /* P19 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR, "stmt -> if_stmt" },
    /* P20 */ { ARBOR2_NT_SI, 5, ARBOR2_NODUS_SI, "if -> 'if' '(' expr ')' stmt" },
    /* P21 */ { ARBOR2_NT_SI, 7, ARBOR2_NODUS_SI, "if -> 'if' '(' expr ')' stmt 'else' stmt" },
    /* P22 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR, "stmt -> while_stmt" },
    /* P23 */ { ARBOR2_NT_DUM, 5, ARBOR2_NODUS_DUM, "while -> 'while' '(' expr ')' stmt" },
    /* P24 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR, "stmt -> do_stmt" },
    /* P25 */ { ARBOR2_NT_FAC, 7, ARBOR2_NODUS_FAC, "do -> 'do' stmt 'while' '(' expr ')' ';'" },
    /* P26 */ { ARBOR2_NT_SENTENTIA, 1, ARBOR2_NODUS_ERROR, "stmt -> for_stmt" },
    /* P27 */ { ARBOR2_NT_PER, 9, ARBOR2_NODUS_PER, "for -> 'for' '(' e ';' e ';' e ')' stmt" },
    /* P28 */ { ARBOR2_NT_EXPRESSIO_OPTATIVA, 1, ARBOR2_NODUS_ERROR, "expr_opt -> expr" },
    /* P29 */ { ARBOR2_NT_EXPRESSIO_OPTATIVA, 0, ARBOR2_NODUS_ERROR, "expr_opt -> e" },
    /* P30 */ { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_FRANGE, "stmt -> 'break' ';'" },
    /* P31 */ { ARBOR2_NT_SENTENTIA, 2, ARBOR2_NODUS_PERGE, "stmt -> 'continue' ';'" },
    /* P32 */ { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_REDDE, "stmt -> 'return' expr_opt ';'" },
    /* P33 */ { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_SALTA, "stmt -> 'goto' ID ';'" },
    /* P34 */ { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_TITULATUM, "stmt -> ID ':' stmt" },
    /* P35 */ { ARBOR2_NT_SENTENTIA, 5, ARBOR2_NODUS_COMMUTATIO, "stmt -> 'switch' '(' expr ')' stmt" },
    /* P36 */ { ARBOR2_NT_SENTENTIA, 4, ARBOR2_NODUS_CASUS, "stmt -> 'case' expr ':' stmt" },
    /* P37 */ { ARBOR2_NT_SENTENTIA, 3, ARBOR2_NODUS_ORDINARIUS, "stmt -> 'default' ':' stmt" },
    /* P38 */ { ARBOR2_NT_DECLARATOR, 3, ARBOR2_NODUS_DECLARATOR_FUNCTI, "declarator -> declarator '(' ')'" },
    /* P39 */ { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR_FUNCTI, "declarator -> declarator '(' 'void' ')'" },
    /* P40 */ { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR_FUNCTI, "declarator -> declarator '(' params ')'" },
    /* P41 */ { ARBOR2_NT_PARAMETER_LIST, 1, ARBOR2_NODUS_ERROR, "params -> param" },
    /* P42 */ { ARBOR2_NT_PARAMETER_LIST, 3, ARBOR2_NODUS_ERROR, "params -> params ',' param" },
    /* P43 */ { ARBOR2_NT_PARAMETER_DECL, 2, ARBOR2_NODUS_PARAMETER_DECL, "param -> type declarator" },
    /* P44 */ { ARBOR2_NT_DEFINITIO_FUNCTI, 3, ARBOR2_NODUS_DEFINITIO_FUNCTI, "func_def -> type declarator compound" },
    /* P45 */ { ARBOR2_NT_STRUCT_SPECIFIER, 5, ARBOR2_NODUS_STRUCT_SPECIFIER, "struct -> 'struct' ID '{' members '}'" },
    /* P46 */ { ARBOR2_NT_STRUCT_SPECIFIER, 4, ARBOR2_NODUS_STRUCT_SPECIFIER, "struct -> 'struct' '{' members '}'" },
    /* P47 */ { ARBOR2_NT_STRUCT_SPECIFIER, 2, ARBOR2_NODUS_STRUCT_SPECIFIER, "struct -> 'struct' ID" },
    /* P48 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO, "members -> type declarator ';'" },
    /* P49 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> members type declarator ';'" },
    /* P50 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "UNUSED" },
    /* P51 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO, "UNUSED" },
    /* P52 */ { ARBOR2_NT_STRUCT_SPECIFIER, 5, ARBOR2_NODUS_STRUCT_SPECIFIER, "union -> 'union' ID '{' members '}'" },
    /* P53 */ { ARBOR2_NT_STRUCT_SPECIFIER, 4, ARBOR2_NODUS_STRUCT_SPECIFIER, "union -> 'union' '{' members '}'" },
    /* P54 */ { ARBOR2_NT_STRUCT_SPECIFIER, 2, ARBOR2_NODUS_STRUCT_SPECIFIER, "union -> 'union' ID" },
    /* P55 */ { ARBOR2_NT_ENUM_SPECIFIER, 5, ARBOR2_NODUS_ENUM_SPECIFIER, "enum -> 'enum' ID '{' enums '}'" },
    /* P56 */ { ARBOR2_NT_ENUM_SPECIFIER, 4, ARBOR2_NODUS_ENUM_SPECIFIER, "enum -> 'enum' '{' enums '}'" },
    /* P57 */ { ARBOR2_NT_ENUM_SPECIFIER, 2, ARBOR2_NODUS_ENUM_SPECIFIER, "enum -> 'enum' ID" },
    /* P58 */ { ARBOR2_NT_ENUMERATOR_LIST, 1, ARBOR2_NODUS_ERROR, "enums -> enumerator" },
    /* P59 */ { ARBOR2_NT_ENUMERATOR_LIST, 3, ARBOR2_NODUS_ERROR, "enums -> enums ',' enumerator" },
    /* P60 */ { ARBOR2_NT_ENUMERATOR, 1, ARBOR2_NODUS_ENUMERATOR, "enumerator -> ID" },
    /* P61 */ { ARBOR2_NT_ENUMERATOR, 3, ARBOR2_NODUS_ENUMERATOR, "enumerator -> ID '=' expr" },
    /* P62 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO, "members -> type ID ':' expr ';'" },
    /* P63 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 6, ARBOR2_NODUS_DECLARATIO, "members -> members type ID ':' expr ';'" },
    /* P64 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> type ':' expr ';'" },
    /* P65 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO, "members -> members type ':' expr ';'" },
    /* P66 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO, "members -> struct ID ';'" },
    /* P67 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> struct '*' ID ';'" },
    /* P68 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> members struct ID ';'" },
    /* P69 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO, "members -> members struct '*' ID ';'" },
    /* P70 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 3, ARBOR2_NODUS_DECLARATIO, "members -> enum ID ';'" },
    /* P71 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> enum '*' ID ';'" },
    /* P72 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 4, ARBOR2_NODUS_DECLARATIO, "members -> members enum ID ';'" },
    /* P73 */ { ARBOR2_NT_STRUCT_MEMBER_LIST, 5, ARBOR2_NODUS_DECLARATIO, "members -> members enum '*' ID ';'" },
    /* P74 */ { ARBOR2_NT_DECLARATIO, 4, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' type ID ';'" },
    /* P75 */ { ARBOR2_NT_DECLARATIO, 5, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' type '*' ID ';'" },
    /* P76 */ { ARBOR2_NT_DECLARATIO, 4, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' struct ID ';'" },
    /* P77 */ { ARBOR2_NT_DECLARATIO, 5, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' struct '*' ID ';'" },
    /* P78 */ { ARBOR2_NT_DECLARATIO, 4, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' enum ID ';'" },
    /* P79 */ { ARBOR2_NT_DECLARATIO, 5, ARBOR2_NODUS_DECLARATIO, "typedef -> 'typedef' enum '*' ID ';'" },
    /* P80 */ { ARBOR2_NT_DECLARATOR, 4, ARBOR2_NODUS_DECLARATOR, "declarator -> declarator '[' expr ']'" },
    /* P81 */ { ARBOR2_NT_DECLARATOR, 3, ARBOR2_NODUS_DECLARATOR, "declarator -> declarator '[' ']'" },
    /* P82 */ { ARBOR2_NT_EXPRESSIO, 3, ARBOR2_NODUS_BINARIUM, "expr -> expr '-' term" },
    /* P83 */ { ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM, "term -> term '/' factor" },
    /* P84 */ { ARBOR2_NT_TERMINUS, 3, ARBOR2_NODUS_BINARIUM, "term -> term '%' factor" },
    /* P85 */ { ARBOR2_NT_AEQUALITAS, 3, ARBOR2_NODUS_BINARIUM, "equality -> equality '==' comparison" },
    /* P86 */ { ARBOR2_NT_AEQUALITAS, 3, ARBOR2_NODUS_BINARIUM, "equality -> equality '!=' comparison" },
    /* P87 */ { ARBOR2_NT_AEQUALITAS, 1, ARBOR2_NODUS_ERROR, "equality -> comparison" },
    /* P88 */ { ARBOR2_NT_COMPARATIO, 3, ARBOR2_NODUS_BINARIUM, "comparison -> comparison '<' shift" },
    /* P89 */ { ARBOR2_NT_COMPARATIO, 3, ARBOR2_NODUS_BINARIUM, "comparison -> comparison '>' shift" },
    /* P90 */ { ARBOR2_NT_COMPARATIO, 3, ARBOR2_NODUS_BINARIUM, "comparison -> comparison '<=' shift" },
    /* P91 */ { ARBOR2_NT_COMPARATIO, 3, ARBOR2_NODUS_BINARIUM, "comparison -> comparison '>=' shift" },
    /* P92 */ { ARBOR2_NT_COMPARATIO, 1, ARBOR2_NODUS_ERROR, "comparison -> shift" },
    /* P93 */ { ARBOR2_NT_DISIUNCTIO, 3, ARBOR2_NODUS_BINARIUM, "or -> or '||' and" },
    /* P94 */ { ARBOR2_NT_DISIUNCTIO, 1, ARBOR2_NODUS_ERROR, "or -> and" },
    /* P95 */ { ARBOR2_NT_CONIUNCTIO, 3, ARBOR2_NODUS_BINARIUM, "and -> and '&&' equality" },
    /* P96 */ { ARBOR2_NT_CONIUNCTIO, 1, ARBOR2_NODUS_ERROR, "and -> equality" },
    /* P97 */ { ARBOR2_NT_TRANSLATIO, 3, ARBOR2_NODUS_BINARIUM, "shift -> shift '<<' expr" },
    /* P98 */ { ARBOR2_NT_TRANSLATIO, 3, ARBOR2_NODUS_BINARIUM, "shift -> shift '>>' expr" },
    /* P99 */ { ARBOR2_NT_TRANSLATIO, 1, ARBOR2_NODUS_ERROR, "shift -> expr" }
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
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_SHIFT, 198 },  /* typedef declaration */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT,  4 },   /* int type specifier */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT,  4 },   /* char type specifier */

    /* State 1: After expression - continue with +/-, start comparison, or reduce to comparatio */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* continue expression */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },   /* E9: subtraction */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99 },  /* E9.3: reduce for << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99 },  /* E9.3: reduce for >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99 },  /* E10: && reduce to comparatio */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99 },  /* E10: || reduce to comparatio */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* for compound */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 99 },  /* for case/label */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 99 },  /* for enums/params */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 99 },  /* for array size */

    /* State 2: After term - expect '*', '/', '%', '+', '-', comparison ops, or end */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_SHIFT,  10 },  /* E9: division */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_SHIFT,  10 },  /* E9: modulo */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 2 },  /* expression -> term */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 2 },  /* E9: reduce for - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 2 },  /* E9.3: reduce for << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 2 },  /* E9.3: reduce for >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 2 },  /* E9.2: reduce for != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 2 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 2 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 2 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 2 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 2 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 2 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 2 },  /* for array size */

    /* State 3: After factor - reduce to term */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 4 },  /* term -> factor */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 4 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 4 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 4 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 4 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 4 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 4 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 4 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 4 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 4 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 4 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 4 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 4 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 4 },  /* for array size */

    /* State 4: After IDENTIFIER - AMBIGUOUS for '*' (expr vs decl), or label with ':' */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 5 },  /* expr: factor -> ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },  /* decl: start declarator */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 5 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 5 },  /* E9: % */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 116 }, /* decl: direct declarator name */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 5 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 5 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 5 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 5 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 5 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 5 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 5 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 5 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 77 },  /* label: shift to labeled stmt */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 5 },  /* case expr: reduce ID to factor */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 5 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 5 },  /* for array size */

    /* State 5: After INTEGER - reduce to factor */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 6 },  /* factor -> INT */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 6 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 6 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 6 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 6 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 6 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 6 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 6 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 6 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 6 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 6 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 6 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 6 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 6 },  /* for array size */

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

    /* State 11: After '(' expression - expect ')' or operators */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* continue expression */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },   /* E9: subtraction */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99 },  /* E9.3: reduce for << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99 },  /* E9.3: reduce for >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99 },  /* E10: || */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio before ) */

    /* State 12: After '(' expression ')' - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 7 },  /* factor -> (expr) */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 7 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 7 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 7 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 7 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 7 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 7 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 7 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 7 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 7 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 7 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 7 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 7 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 7 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 7 },  /* for array size */

    /* State 13: After expression '+' term - reduce or continue */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10 },  /* * binds tighter */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_SHIFT,  10 },  /* E9: / binds tighter */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_SHIFT,  10 },  /* E9: % binds tighter */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 1 },   /* expr -> expr + term */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 1 },   /* E9: same precedence */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: < lower prec */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: > lower prec */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: <= lower prec */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: >= lower prec */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 1 },   /* E9.3: << lower prec */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 1 },   /* E9.3: >> lower prec */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: == lower prec */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 1 },   /* E9.2: != lower prec */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 1 },   /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 1 },   /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 1 },   /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },   /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 1 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 1 },   /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 1 },   /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 1 },   /* for array size */

    /* State 14: After term '*' factor - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 3 },  /* term -> term * factor */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 3 },  /* E9: same precedence */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 3 },  /* E9: same precedence */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 3 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 3 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 3 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 3 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 3 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 3 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 3 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 3 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 3 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 3 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 3 },  /* for array size */

    /* State 15: After '*' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 8 },  /* factor -> * factor */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 8 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 8 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 8 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 8 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 8 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 8 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 8 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 8 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 8 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },  /* for compound */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 8 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 8 },  /* for case expression */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 8 },  /* for enum values */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 8 },  /* for array size */

    /* State 16: After '&' factor (unary) - reduce */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 9 },  /* factor -> & factor */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 9 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 9 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 9 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 9 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 9 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 9 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 9 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 9 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 9 },  /* for statements */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },  /* for compound */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 9 },  /* for enum values */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 9 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 9 },  /* for case expression */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 9 },  /* for array size */

    /* State 17: After '*' in declarator - expect ID or more '*' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* declarator name */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* more pointers */

    /* State 18: After IDENTIFIER in declarator - reduce P12 */
    /* When we see '(' we reduce first, then state 19 handles the shift */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12 },  /* declarator -> ID */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 12 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 12 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 12 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 12 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 12 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 12 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 12 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 12 },  /* E10: || */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 12 },  /* reduce first, then fn decl */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 12 },  /* reduce first, then array decl */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 12 },  /* for struct member ending */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 12 },  /* for bit field (E4) */

    /* State 19: After '* declarator' - reduce P11 or shift ( for fn */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 11 },  /* declarator -> * declarator */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 11 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 11 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 11 },  /* E9: - */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: < */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: > */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: <= */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: >= */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 11 },  /* E9.3: << */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 11 },  /* E9.3: >> */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: == */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 11 },  /* E9.2: != */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 11 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 11 },  /* E10: || */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 91 },   /* function declarator */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 11 },  /* for function definition body */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 11 },  /* reduce first, then array decl */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 11 },  /* for struct member ending */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 11 },  /* for bit field (E4) */

    /* State 20: After 'type_specifier declarator' - reduce P10 or continue to function/array */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 10 },  /* declaration -> type declarator */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 91 },   /* function declarator params */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 25 },   /* function definition body */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array declarator */
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

    /* State 32: After 'if ( expr' - expect ')' or continue expression */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },

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

    /* State 41: After 'while ( expr' - expect ')' or continue expression */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },

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

    /* State 49: After 'do stmt while ( expr' - expect ')' or continue */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },

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

    /* State 58: After 'for ( expr_opt ; expression' - continue or reduce to comparatio */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },

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

    /* State 81: After 'switch ( expr' - expect ')' or continue */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* E9.2: start comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },  /* reduce to comparatio first */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },

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
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 38 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 38 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 38 },  /* E9: - */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 38 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 38 },  /* for chained fn decl */
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 38 },  /* for function definition body */

    /* State 93: After 'declarator ( void' - expect ')' */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 94 },   /* (void) */

    /* State 94: After 'declarator ( void )' - reduce P39 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 39 },  /* declarator -> declarator (void) */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 39 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 39 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 39 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 39 },  /* E9: - */
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
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 40 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 40 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 40 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 40 },  /* E9: - */
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
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 12 },  /* E9: / */
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 12 },  /* E9: % */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12 },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 12 },  /* E9: - */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 12 },  /* reduce first, then array decl */

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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */
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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */
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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */

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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */

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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */

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
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* member name - use declarator states */

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
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 73 },

    /* ========== TYPEDEF DECLARATION STATES (198-216) ========== */

    /* State 198: After 'typedef' - expect type_specifier */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 199 },  /* typedef SomeType */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 199 },  /* typedef int */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 199 },  /* typedef char */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117 },  /* typedef struct {...} */
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137 },  /* typedef union {...} */
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145 },  /* typedef enum {...} */

    /* State 199: After 'typedef type_spec' - expect '*' or ID */
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* typedef name - use declarator states */

    /* State 200: After 'typedef type_spec *' - expect more '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 200 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 202 },  /* typedef name */

    /* State 201: After 'typedef type_spec ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 203 },

    /* State 202: After 'typedef type_spec *... ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 204 },

    /* State 203: Reduce P74 (typedef type_spec ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 74 },

    /* State 204: Reduce P75 (typedef type_spec *... ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 75 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 75 },

    /* State 205: After 'typedef struct_spec' - expect '*' or ID */
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* typedef name - use declarator states */

    /* State 206: After 'typedef struct_spec *' - expect more '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 206 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 208 },  /* typedef name */

    /* State 207: After 'typedef struct_spec ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 209 },

    /* State 208: After 'typedef struct_spec *... ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 210 },

    /* State 209: Reduce P76 (typedef struct_spec ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 76 },

    /* State 210: Reduce P77 (typedef struct_spec *... ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 77 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 77 },

    /* State 211: After 'typedef enum_spec' - expect '*' or ID */
    /* E8: Route through declarator states for array support */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17 },   /* pointer - use declarator states */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18 },   /* typedef name - use declarator states */

    /* State 212: After 'typedef enum_spec *' - expect more '*' or ID */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 212 },  /* more pointers */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 214 },  /* typedef name */

    /* State 213: After 'typedef enum_spec ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 215 },

    /* State 214: After 'typedef enum_spec *... ID' - expect ';' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 216 },

    /* State 215: Reduce P78 (typedef enum_spec ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 78 },

    /* State 216: Reduce P79 (typedef enum_spec *... ID ;) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 79 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 79 },

    /* ==================================================
     * Array Declarator States (217-220)
     * ================================================== */

    /* State 217: After 'declarator [' - expect expression or ']' */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },   /* expression start */
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },   /* expression start */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_SHIFT, 218 },  /* unsized array [] */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },   /* parenthesized expr */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },   /* unary * */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },   /* unary & */

    /* State 218: After 'declarator [ ]' - reduce P81 (unsized array) */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 81 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 81 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 81 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 81 },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 81 },  /* multi-dim: [][] */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 81 },

    /* State 219: After 'declarator [ expression' - expect ']' or continue expr */
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_SHIFT, 220 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* continue expression */

    /* State 220: After 'declarator [ expression ]' - reduce P80 (sized array) */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 80 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 80 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 80 },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 80 },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 80 },  /* multi-dim: [n][] */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 80 },

    /* ==================================================
     * E8: Struct Member Declarator Completion States (221-224)
     * ================================================== */

    /* State 221: After 'type_spec declarator' in struct (first member) */
    /* Expects ';' to end member, '[' for array, or ':' for bit field */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 222 },  /* end member decl */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 162 },  /* bit field width */

    /* State 222: After 'type_spec declarator ;' (first member) - reduce P48 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48 },  /* reduce, end struct */

    /* State 223: After 'list type_spec declarator' (subsequent member) */
    /* Expects ';' to end member, '[' for array, or ':' for bit field */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 224 },  /* end member decl */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 165 },  /* bit field width */

    /* State 224: After 'list type_spec declarator ;' (subsequent member) - reduce P49 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, more members */
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, int member */
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, char member */
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, nested struct */
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 49 },  /* reduce, end struct */

    /* ==================================================
     * E8: Typedef Declarator Completion States (225-230)
     * ================================================== */

    /* State 225: After 'typedef type_spec declarator' - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 226 },  /* end typedef */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 226: After 'typedef type_spec declarator ;' - reduce P74 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 74 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 74 },

    /* State 227: After 'typedef struct_spec declarator' - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 228 },  /* end typedef */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 228: After 'typedef struct_spec declarator ;' - reduce P76 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 76 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 76 },

    /* State 229: After 'typedef enum_spec declarator' - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 230 },  /* end typedef */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 230: After 'typedef enum_spec declarator ;' - reduce P78 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 78 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 78 },

    /* ==================================================
     * E8: Nested Type Member Declarator Completion States (231-238)
     * ================================================== */

    /* State 231: After 'struct_spec declarator' (first member) - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 232 },  /* end member */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 232: After 'struct_spec declarator ;' (first member) - reduce P66 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 66 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 66 },

    /* State 233: After 'list struct_spec declarator' (subsequent member) - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 234 },  /* end member */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 234: After 'list struct_spec declarator ;' (subsequent member) - reduce P68 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 68 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 68 },

    /* State 235: After 'enum_spec declarator' (first member) - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 236 },  /* end member */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 236: After 'enum_spec declarator ;' (first member) - reduce P70 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 70 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 70 },

    /* State 237: After 'list enum_spec declarator' (subsequent member) - expect ';' or '[' */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 238 },  /* end member */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217 },  /* array continuation */

    /* State 238: After 'list enum_spec declarator ;' (subsequent member) - reduce P72 */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 72 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 72 },

    /* ========== COMPARISON/EQUALITY STATES (E9 Part 2) ========== */

    /* State 239: After comparatio - shift to continue chain or reduce to aequalitas */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* start equality */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },  /* start equality */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* chain: comp < ... */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* chain: comp > ... */
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* chain: comp <= ... */
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },  /* chain: comp >= ... */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 87 },  /* comparatio -> aequalitas */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 87 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 87 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 87 },  /* E10: || */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 87 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 87 },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 87 },

    /* State 240: After aequalitas at top-level - reduce to coniunctio or continue equality */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality: aeq == ... */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality: aeq != ... */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96 },  /* reduce P96: coniunctio -> aequalitas */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96 },  /* && triggers reduce to coniunctio */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96 },  /* || triggers reduce to coniunctio */

    /* State 241: After 'comparatio <|>|<=|>=' - expect expression starters */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 242: After 'aequalitas ==|!=' - expect comparatio (expression starters) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 243: After 'comparatio <|>|<=|>= expression' - continue expr or reduce P99
     * Stack has: comparatio, <, expression
     * Must reduce expr to translatio (P99) first, then state 264 will handle P88 */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9 },   /* +/- have higher prec: continue expr */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9 },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99 },  /* reduce expr->translatio, then shift */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 99 },  /* reduce expr->translatio first */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 99 },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 99 },

    /* State 244: After 'aequalitas ==|!= comparatio' - reduce P85 */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 85 },  /* left-associativity */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 85 },  /* E10: && */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 85 },  /* E10: || */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 85 },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 85 },

    /* State 245: After aequalitas inside parens - shift ) or continue equality chain */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 12 },   /* complete paren-expr */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue: aeq == ... */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },  /* continue: aeq != ... */

    /* State 246: After comparatio in if condition - reduce to aequalitas or continue chain */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87 },  /* comparatio -> aequalitas */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* start equality */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* chain comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },

    /* State 247: After aequalitas in if condition - shift ) or continue equality chain */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 33 },   /* continue to then-branch */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },

    /* State 248: After aequalitas in while condition - shift ) or continue equality chain */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 42 },   /* continue to while body */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },

    /* State 249: After comparatio in for condition - reduce to aequalitas or continue */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 87 },  /* reduce to aequalitas */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* start equality */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241 },  /* chain comparison */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241 },

    /* State 250: After aequalitas in for condition - reduce to expr_opt or continue equality */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 28 },  /* reduce to expr_opt */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },

    /* State 251: After aequalitas in do-while condition - shift ) or continue equality */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 50 },   /* continue to do-while completion */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },

    /* State 252: After aequalitas in switch condition - shift ) or continue equality */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 82 },   /* continue to switch body */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },

    /* ==== E10: Logical Operators (&&, ||) ==== */

    /* State 253: After coniunctio at top-level - reduce to disiunctio or continue && chain */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_SHIFT, 254 },  /* continue && chain */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 94 },  /* reduce P94: disiunctio -> coniunctio */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 94 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 94 },  /* || triggers reduce to disiunctio */

    /* State 254: After 'coniunctio &&' - expect aequalitas (expression starters) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 255: After disiunctio at top-level - ACCEPT or continue || chain */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_SHIFT, 256 },  /* continue || chain */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 22 },   /* expression statement */

    /* State 256: After 'disiunctio ||' - expect coniunctio (expression starters) */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 257: After aequalitas in && context (top-level) - reduce P96 or continue equality */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96 },

    /* State 258: After 'coniunctio && aequalitas' (via coniunctio) - reduce P95 */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 95 },  /* left-associativity */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 95 },

    /* State 259: After aequalitas in || context - reduce P96 to coniunctio */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },  /* continue equality chain */
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96 },

    /* State 260: After coniunctio in || context - reduce P93 or continue && */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_SHIFT, 261 },  /* continue && within || RHS */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 93 },  /* reduce P93: disiunctio -> disiunctio || coniunctio */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 93 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 93 },

    /* State 261: After 'coniunctio &&' in || context - expect aequalitas */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8 },

    /* State 262: After aequalitas in && within || context - reduce P96 */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96 },

    /* State 263: After 'coniunctio && aequalitas' in || context - reduce P95 */
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 95 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 95 },

    /* State 264: After translatio (general context) - reduce P92 only
     * Note: P88 reductions for 'comparatio < translatio' are handled via
     * state 243 which reduces expr->translatio then to P88 in one flow.
     */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_SHIFT, 265 },  /* << continue shift chain */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_SHIFT, 265 },  /* >> continue shift chain */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 92 },  /* reduce translatio -> comparatio */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 92 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 92 },

    /* State 265: After translatio << or >> - expect expression starters */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4 },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5 },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6 },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7 },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8 },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   7 },  /* unary + */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   7 },  /* unary - */

    /* State 266: After translatio << expression - reduce P97 or continue expr */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9 },  /* higher prec: continue expr */
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9 },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 97 },  /* left assoc: reduce then continue */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 97 },  /* lower prec: reduce */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 97 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 97 },

    /* State 267: After translatio in 'comparatio <' context - reduce P88
     * This state is ONLY reached via GOTO TRANSLATIO from state 241.
     * Reduces comparatio < translatio -> comparatio (P88)
     */
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_SHIFT, 265 },  /* << continue shift chain */
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_SHIFT, 265 },  /* >> continue shift chain */
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 88 },  /* reduce P88 */
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 88 },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 88 }
};

hic_manens i32 NUM_ACTIONES = (i32)(magnitudo(ACTIONES) / magnitudo(ACTIONES[0]));

/* ==================================================
 * STATE ACTION COUNTS
 *
 * Each macro defines how many actions a state has.
 * When adding/removing actions from a state, only
 * update that state's count - indices recalculate
 * automatically at compile time.
 * ================================================== */

#define STATE_0_COUNT   24
#define STATE_1_COUNT   19  /* E9.2: +10 for comparison/equality ops, E9.3: +2 shift, E10: +2 for &&,|| */
#define STATE_2_COUNT   22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_3_COUNT   22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_4_COUNT   25  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_5_COUNT   22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_6_COUNT   5
#define STATE_7_COUNT   5
#define STATE_8_COUNT   5
#define STATE_9_COUNT   5
#define STATE_10_COUNT  5
#define STATE_11_COUNT  13  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_12_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_13_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_14_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_15_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_16_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_17_COUNT  2
#define STATE_18_COUNT  21  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_19_COUNT  22  /* E9.2: +6 for comparison ops, E9.3: +2 shift, E10: +2 */
#define STATE_20_COUNT  7
#define STATE_21_COUNT  1
#define STATE_22_COUNT  21
#define STATE_23_COUNT  21
#define STATE_24_COUNT  1
#define STATE_25_COUNT  19
#define STATE_26_COUNT  19
#define STATE_27_COUNT  21
#define STATE_28_COUNT  19
#define STATE_29_COUNT  21
#define STATE_30_COUNT  1
#define STATE_31_COUNT  5
#define STATE_32_COUNT  9   /* E9.2: added comparison operators */
#define STATE_33_COUNT  18
#define STATE_34_COUNT  21
#define STATE_35_COUNT  18
#define STATE_36_COUNT  21
#define STATE_37_COUNT  21
#define STATE_38_COUNT  21
#define STATE_39_COUNT  1
#define STATE_40_COUNT  5
#define STATE_41_COUNT  9   /* E9.2: added comparison operators */
#define STATE_42_COUNT  18
#define STATE_43_COUNT  21
#define STATE_44_COUNT  21
#define STATE_45_COUNT  18
#define STATE_46_COUNT  1
#define STATE_47_COUNT  1
#define STATE_48_COUNT  5
#define STATE_49_COUNT  9   /* E9.2: added comparison operators */
#define STATE_50_COUNT  1
#define STATE_51_COUNT  21
#define STATE_52_COUNT  21
#define STATE_53_COUNT  1
#define STATE_54_COUNT  6
#define STATE_55_COUNT  1
#define STATE_56_COUNT  1
#define STATE_57_COUNT  6
#define STATE_58_COUNT  9   /* E9.2: added comparison operators */
#define STATE_59_COUNT  1
#define STATE_60_COUNT  6
#define STATE_61_COUNT  1
#define STATE_62_COUNT  1
#define STATE_63_COUNT  18
#define STATE_64_COUNT  21
#define STATE_65_COUNT  21
#define STATE_66_COUNT  1
#define STATE_67_COUNT  21
#define STATE_68_COUNT  1
#define STATE_69_COUNT  21
#define STATE_70_COUNT  6
#define STATE_71_COUNT  2
#define STATE_72_COUNT  1
#define STATE_73_COUNT  21
#define STATE_74_COUNT  1
#define STATE_75_COUNT  1
#define STATE_76_COUNT  21
#define STATE_77_COUNT  18
#define STATE_78_COUNT  21
#define STATE_79_COUNT  1
#define STATE_80_COUNT  5
#define STATE_81_COUNT  9   /* E9.2: added comparison operators */
#define STATE_82_COUNT  18
#define STATE_83_COUNT  21
#define STATE_84_COUNT  5
#define STATE_85_COUNT  2
#define STATE_86_COUNT  18
#define STATE_87_COUNT  21
#define STATE_88_COUNT  1
#define STATE_89_COUNT  18
#define STATE_90_COUNT  21
#define STATE_91_COUNT  5
#define STATE_92_COUNT  9   /* E9: +3 for SOLIDUS, PERCENTUM, MINUS */
#define STATE_93_COUNT  1
#define STATE_94_COUNT  9   /* E9: +3 for SOLIDUS, PERCENTUM, MINUS */
#define STATE_95_COUNT  2
#define STATE_96_COUNT  2
#define STATE_97_COUNT  2
#define STATE_98_COUNT  2
#define STATE_99_COUNT  2
#define STATE_100_COUNT 2
#define STATE_101_COUNT 2
#define STATE_102_COUNT 2
#define STATE_103_COUNT 9   /* E9: +3 for SOLIDUS, PERCENTUM, MINUS */
#define STATE_104_COUNT 3
#define STATE_105_COUNT 2
#define STATE_106_COUNT 2
#define STATE_107_COUNT 2
#define STATE_108_COUNT 2
#define STATE_109_COUNT 2
#define STATE_110_COUNT 2
#define STATE_111_COUNT 2
#define STATE_112_COUNT 0
#define STATE_113_COUNT 1
#define STATE_114_COUNT 1
#define STATE_115_COUNT 0
#define STATE_116_COUNT 10  /* E9: +3 for SOLIDUS, PERCENTUM, MINUS */
#define STATE_117_COUNT 2
#define STATE_118_COUNT 4
#define STATE_119_COUNT 7
#define STATE_120_COUNT 7
#define STATE_121_COUNT 3
#define STATE_122_COUNT 2
#define STATE_123_COUNT 2
#define STATE_124_COUNT 1
#define STATE_125_COUNT 5
#define STATE_126_COUNT 5
#define STATE_127_COUNT 7
#define STATE_128_COUNT 3
#define STATE_129_COUNT 7
#define STATE_130_COUNT 3
#define STATE_131_COUNT 3
#define STATE_132_COUNT 2
#define STATE_133_COUNT 2
#define STATE_134_COUNT 1
#define STATE_135_COUNT 5
#define STATE_136_COUNT 5
#define STATE_137_COUNT 2
#define STATE_138_COUNT 4
#define STATE_139_COUNT 6
#define STATE_140_COUNT 6
#define STATE_141_COUNT 6
#define STATE_142_COUNT 3
#define STATE_143_COUNT 6
#define STATE_144_COUNT 3
#define STATE_145_COUNT 2
#define STATE_146_COUNT 4
#define STATE_147_COUNT 1
#define STATE_148_COUNT 1
#define STATE_149_COUNT 3
#define STATE_150_COUNT 5
#define STATE_151_COUNT 3
#define STATE_152_COUNT 2
#define STATE_153_COUNT 2
#define STATE_154_COUNT 3
#define STATE_155_COUNT 2
#define STATE_156_COUNT 1
#define STATE_157_COUNT 3
#define STATE_158_COUNT 3
#define STATE_159_COUNT 5
#define STATE_160_COUNT 3
#define STATE_161_COUNT 2
#define STATE_162_COUNT 5
#define STATE_163_COUNT 2
#define STATE_164_COUNT 5
#define STATE_165_COUNT 5
#define STATE_166_COUNT 2
#define STATE_167_COUNT 5
#define STATE_168_COUNT 5
#define STATE_169_COUNT 2
#define STATE_170_COUNT 5
#define STATE_171_COUNT 5
#define STATE_172_COUNT 2
#define STATE_173_COUNT 5
#define STATE_174_COUNT 2
#define STATE_175_COUNT 2
#define STATE_176_COUNT 1
#define STATE_177_COUNT 1
#define STATE_178_COUNT 7
#define STATE_179_COUNT 7
#define STATE_180_COUNT 2
#define STATE_181_COUNT 2
#define STATE_182_COUNT 1
#define STATE_183_COUNT 1
#define STATE_184_COUNT 7
#define STATE_185_COUNT 7
#define STATE_186_COUNT 2
#define STATE_187_COUNT 2
#define STATE_188_COUNT 1
#define STATE_189_COUNT 1
#define STATE_190_COUNT 7
#define STATE_191_COUNT 7
#define STATE_192_COUNT 2
#define STATE_193_COUNT 2
#define STATE_194_COUNT 1
#define STATE_195_COUNT 1
#define STATE_196_COUNT 7
#define STATE_197_COUNT 7
#define STATE_198_COUNT 6
#define STATE_199_COUNT 2
#define STATE_200_COUNT 2
#define STATE_201_COUNT 1
#define STATE_202_COUNT 1
#define STATE_203_COUNT 8
#define STATE_204_COUNT 8
#define STATE_205_COUNT 2
#define STATE_206_COUNT 2
#define STATE_207_COUNT 1
#define STATE_208_COUNT 1
#define STATE_209_COUNT 8
#define STATE_210_COUNT 8
#define STATE_211_COUNT 2
#define STATE_212_COUNT 2
#define STATE_213_COUNT 1
#define STATE_214_COUNT 1
#define STATE_215_COUNT 8
#define STATE_216_COUNT 8
#define STATE_217_COUNT 6
#define STATE_218_COUNT 6
#define STATE_219_COUNT 2
#define STATE_220_COUNT 6
#define STATE_221_COUNT 3
#define STATE_222_COUNT 5
#define STATE_223_COUNT 3
#define STATE_224_COUNT 5
#define STATE_225_COUNT 2
#define STATE_226_COUNT 8
#define STATE_227_COUNT 2
#define STATE_228_COUNT 8
#define STATE_229_COUNT 2
#define STATE_230_COUNT 8
#define STATE_231_COUNT 2
#define STATE_232_COUNT 7
#define STATE_233_COUNT 2
#define STATE_234_COUNT 7
#define STATE_235_COUNT 2
#define STATE_236_COUNT 7
#define STATE_237_COUNT 2
#define STATE_238_COUNT 7

/* E9.2 Comparison/Equality States */
#define STATE_239_COUNT 15  /* after comparatio, E10: +2 */
#define STATE_240_COUNT 6   /* after aequalitas at top-level (reduce P96) E10: +2 for &&,|| */
#define STATE_241_COUNT 5   /* after comparison op */
#define STATE_242_COUNT 5   /* after equality op */
#define STATE_243_COUNT 17  /* after comp op expr (reduce P99->P88) */
#define STATE_244_COUNT 11  /* after eq op comparatio (reduce P85), E10: +2 */
#define STATE_245_COUNT 3   /* after aequalitas inside parens */
#define STATE_246_COUNT 7   /* after comparatio in if condition */
#define STATE_247_COUNT 3   /* after aequalitas in if condition */
#define STATE_248_COUNT 3   /* after aequalitas in while condition */
#define STATE_249_COUNT 7   /* after comparatio in for condition */
#define STATE_250_COUNT 3   /* after aequalitas in for condition */
#define STATE_251_COUNT 3   /* after aequalitas in do-while condition */
#define STATE_252_COUNT 3   /* after aequalitas in switch condition */

/* E10: Logical Operators States */
#define STATE_253_COUNT 4   /* after coniunctio at top-level */
#define STATE_254_COUNT 5   /* after 'coniunctio &&' */
#define STATE_255_COUNT 3   /* after disiunctio at top-level (accept) */
#define STATE_256_COUNT 5   /* after 'disiunctio ||' */
#define STATE_257_COUNT 6   /* after aequalitas in && context */
#define STATE_258_COUNT 6   /* after 'coniunctio && aequalitas' (reduce P95) */
#define STATE_259_COUNT 6   /* after aequalitas in || context */
#define STATE_260_COUNT 4   /* after coniunctio in || context */
#define STATE_261_COUNT 5   /* after 'coniunctio &&' in || context */
#define STATE_262_COUNT 6   /* after aequalitas in && within || */
#define STATE_263_COUNT 6   /* after 'coniunctio && aequalitas' in || (reduce P95) */
#define STATE_264_COUNT 15  /* after translatio - shift <</>>, reduce P92 */
#define STATE_265_COUNT 7   /* after translatio << or >> - expect expression */
#define STATE_266_COUNT 15  /* after translatio << expression - reduce P97 */
#define STATE_267_COUNT 15  /* after translatio in 'comp <' context - reduce P88 */

/* ==================================================
 * CHAINED INDEX MACROS
 *
 * Each IDX_STATE_N computes its index from the
 * previous state's index plus that state's count.
 * The compiler evaluates these at compile time.
 * ================================================== */

#define IDX_STATE_0     0
#define IDX_STATE_1     (IDX_STATE_0 + STATE_0_COUNT)
#define IDX_STATE_2     (IDX_STATE_1 + STATE_1_COUNT)
#define IDX_STATE_3     (IDX_STATE_2 + STATE_2_COUNT)
#define IDX_STATE_4     (IDX_STATE_3 + STATE_3_COUNT)
#define IDX_STATE_5     (IDX_STATE_4 + STATE_4_COUNT)
#define IDX_STATE_6     (IDX_STATE_5 + STATE_5_COUNT)
#define IDX_STATE_7     (IDX_STATE_6 + STATE_6_COUNT)
#define IDX_STATE_8     (IDX_STATE_7 + STATE_7_COUNT)
#define IDX_STATE_9     (IDX_STATE_8 + STATE_8_COUNT)
#define IDX_STATE_10    (IDX_STATE_9 + STATE_9_COUNT)
#define IDX_STATE_11    (IDX_STATE_10 + STATE_10_COUNT)
#define IDX_STATE_12    (IDX_STATE_11 + STATE_11_COUNT)
#define IDX_STATE_13    (IDX_STATE_12 + STATE_12_COUNT)
#define IDX_STATE_14    (IDX_STATE_13 + STATE_13_COUNT)
#define IDX_STATE_15    (IDX_STATE_14 + STATE_14_COUNT)
#define IDX_STATE_16    (IDX_STATE_15 + STATE_15_COUNT)
#define IDX_STATE_17    (IDX_STATE_16 + STATE_16_COUNT)
#define IDX_STATE_18    (IDX_STATE_17 + STATE_17_COUNT)
#define IDX_STATE_19    (IDX_STATE_18 + STATE_18_COUNT)
#define IDX_STATE_20    (IDX_STATE_19 + STATE_19_COUNT)
#define IDX_STATE_21    (IDX_STATE_20 + STATE_20_COUNT)
#define IDX_STATE_22    (IDX_STATE_21 + STATE_21_COUNT)
#define IDX_STATE_23    (IDX_STATE_22 + STATE_22_COUNT)
#define IDX_STATE_24    (IDX_STATE_23 + STATE_23_COUNT)
#define IDX_STATE_25    (IDX_STATE_24 + STATE_24_COUNT)
#define IDX_STATE_26    (IDX_STATE_25 + STATE_25_COUNT)
#define IDX_STATE_27    (IDX_STATE_26 + STATE_26_COUNT)
#define IDX_STATE_28    (IDX_STATE_27 + STATE_27_COUNT)
#define IDX_STATE_29    (IDX_STATE_28 + STATE_28_COUNT)
#define IDX_STATE_30    (IDX_STATE_29 + STATE_29_COUNT)
#define IDX_STATE_31    (IDX_STATE_30 + STATE_30_COUNT)
#define IDX_STATE_32    (IDX_STATE_31 + STATE_31_COUNT)
#define IDX_STATE_33    (IDX_STATE_32 + STATE_32_COUNT)
#define IDX_STATE_34    (IDX_STATE_33 + STATE_33_COUNT)
#define IDX_STATE_35    (IDX_STATE_34 + STATE_34_COUNT)
#define IDX_STATE_36    (IDX_STATE_35 + STATE_35_COUNT)
#define IDX_STATE_37    (IDX_STATE_36 + STATE_36_COUNT)
#define IDX_STATE_38    (IDX_STATE_37 + STATE_37_COUNT)
#define IDX_STATE_39    (IDX_STATE_38 + STATE_38_COUNT)
#define IDX_STATE_40    (IDX_STATE_39 + STATE_39_COUNT)
#define IDX_STATE_41    (IDX_STATE_40 + STATE_40_COUNT)
#define IDX_STATE_42    (IDX_STATE_41 + STATE_41_COUNT)
#define IDX_STATE_43    (IDX_STATE_42 + STATE_42_COUNT)
#define IDX_STATE_44    (IDX_STATE_43 + STATE_43_COUNT)
#define IDX_STATE_45    (IDX_STATE_44 + STATE_44_COUNT)
#define IDX_STATE_46    (IDX_STATE_45 + STATE_45_COUNT)
#define IDX_STATE_47    (IDX_STATE_46 + STATE_46_COUNT)
#define IDX_STATE_48    (IDX_STATE_47 + STATE_47_COUNT)
#define IDX_STATE_49    (IDX_STATE_48 + STATE_48_COUNT)
#define IDX_STATE_50    (IDX_STATE_49 + STATE_49_COUNT)
#define IDX_STATE_51    (IDX_STATE_50 + STATE_50_COUNT)
#define IDX_STATE_52    (IDX_STATE_51 + STATE_51_COUNT)
#define IDX_STATE_53    (IDX_STATE_52 + STATE_52_COUNT)
#define IDX_STATE_54    (IDX_STATE_53 + STATE_53_COUNT)
#define IDX_STATE_55    (IDX_STATE_54 + STATE_54_COUNT)
#define IDX_STATE_56    (IDX_STATE_55 + STATE_55_COUNT)
#define IDX_STATE_57    (IDX_STATE_56 + STATE_56_COUNT)
#define IDX_STATE_58    (IDX_STATE_57 + STATE_57_COUNT)
#define IDX_STATE_59    (IDX_STATE_58 + STATE_58_COUNT)
#define IDX_STATE_60    (IDX_STATE_59 + STATE_59_COUNT)
#define IDX_STATE_61    (IDX_STATE_60 + STATE_60_COUNT)
#define IDX_STATE_62    (IDX_STATE_61 + STATE_61_COUNT)
#define IDX_STATE_63    (IDX_STATE_62 + STATE_62_COUNT)
#define IDX_STATE_64    (IDX_STATE_63 + STATE_63_COUNT)
#define IDX_STATE_65    (IDX_STATE_64 + STATE_64_COUNT)
#define IDX_STATE_66    (IDX_STATE_65 + STATE_65_COUNT)
#define IDX_STATE_67    (IDX_STATE_66 + STATE_66_COUNT)
#define IDX_STATE_68    (IDX_STATE_67 + STATE_67_COUNT)
#define IDX_STATE_69    (IDX_STATE_68 + STATE_68_COUNT)
#define IDX_STATE_70    (IDX_STATE_69 + STATE_69_COUNT)
#define IDX_STATE_71    (IDX_STATE_70 + STATE_70_COUNT)
#define IDX_STATE_72    (IDX_STATE_71 + STATE_71_COUNT)
#define IDX_STATE_73    (IDX_STATE_72 + STATE_72_COUNT)
#define IDX_STATE_74    (IDX_STATE_73 + STATE_73_COUNT)
#define IDX_STATE_75    (IDX_STATE_74 + STATE_74_COUNT)
#define IDX_STATE_76    (IDX_STATE_75 + STATE_75_COUNT)
#define IDX_STATE_77    (IDX_STATE_76 + STATE_76_COUNT)
#define IDX_STATE_78    (IDX_STATE_77 + STATE_77_COUNT)
#define IDX_STATE_79    (IDX_STATE_78 + STATE_78_COUNT)
#define IDX_STATE_80    (IDX_STATE_79 + STATE_79_COUNT)
#define IDX_STATE_81    (IDX_STATE_80 + STATE_80_COUNT)
#define IDX_STATE_82    (IDX_STATE_81 + STATE_81_COUNT)
#define IDX_STATE_83    (IDX_STATE_82 + STATE_82_COUNT)
#define IDX_STATE_84    (IDX_STATE_83 + STATE_83_COUNT)
#define IDX_STATE_85    (IDX_STATE_84 + STATE_84_COUNT)
#define IDX_STATE_86    (IDX_STATE_85 + STATE_85_COUNT)
#define IDX_STATE_87    (IDX_STATE_86 + STATE_86_COUNT)
#define IDX_STATE_88    (IDX_STATE_87 + STATE_87_COUNT)
#define IDX_STATE_89    (IDX_STATE_88 + STATE_88_COUNT)
#define IDX_STATE_90    (IDX_STATE_89 + STATE_89_COUNT)
#define IDX_STATE_91    (IDX_STATE_90 + STATE_90_COUNT)
#define IDX_STATE_92    (IDX_STATE_91 + STATE_91_COUNT)
#define IDX_STATE_93    (IDX_STATE_92 + STATE_92_COUNT)
#define IDX_STATE_94    (IDX_STATE_93 + STATE_93_COUNT)
#define IDX_STATE_95    (IDX_STATE_94 + STATE_94_COUNT)
#define IDX_STATE_96    (IDX_STATE_95 + STATE_95_COUNT)
#define IDX_STATE_97    (IDX_STATE_96 + STATE_96_COUNT)
#define IDX_STATE_98    (IDX_STATE_97 + STATE_97_COUNT)
#define IDX_STATE_99    (IDX_STATE_98 + STATE_98_COUNT)
#define IDX_STATE_100   (IDX_STATE_99 + STATE_99_COUNT)
#define IDX_STATE_101   (IDX_STATE_100 + STATE_100_COUNT)
#define IDX_STATE_102   (IDX_STATE_101 + STATE_101_COUNT)
#define IDX_STATE_103   (IDX_STATE_102 + STATE_102_COUNT)
#define IDX_STATE_104   (IDX_STATE_103 + STATE_103_COUNT)
#define IDX_STATE_105   (IDX_STATE_104 + STATE_104_COUNT)
#define IDX_STATE_106   (IDX_STATE_105 + STATE_105_COUNT)
#define IDX_STATE_107   (IDX_STATE_106 + STATE_106_COUNT)
#define IDX_STATE_108   (IDX_STATE_107 + STATE_107_COUNT)
#define IDX_STATE_109   (IDX_STATE_108 + STATE_108_COUNT)
#define IDX_STATE_110   (IDX_STATE_109 + STATE_109_COUNT)
#define IDX_STATE_111   (IDX_STATE_110 + STATE_110_COUNT)
#define IDX_STATE_112   (IDX_STATE_111 + STATE_111_COUNT)
#define IDX_STATE_113   (IDX_STATE_112 + STATE_112_COUNT)
#define IDX_STATE_114   (IDX_STATE_113 + STATE_113_COUNT)
#define IDX_STATE_115   (IDX_STATE_114 + STATE_114_COUNT)
#define IDX_STATE_116   (IDX_STATE_115 + STATE_115_COUNT)
#define IDX_STATE_117   (IDX_STATE_116 + STATE_116_COUNT)
#define IDX_STATE_118   (IDX_STATE_117 + STATE_117_COUNT)
#define IDX_STATE_119   (IDX_STATE_118 + STATE_118_COUNT)
#define IDX_STATE_120   (IDX_STATE_119 + STATE_119_COUNT)
#define IDX_STATE_121   (IDX_STATE_120 + STATE_120_COUNT)
#define IDX_STATE_122   (IDX_STATE_121 + STATE_121_COUNT)
#define IDX_STATE_123   (IDX_STATE_122 + STATE_122_COUNT)
#define IDX_STATE_124   (IDX_STATE_123 + STATE_123_COUNT)
#define IDX_STATE_125   (IDX_STATE_124 + STATE_124_COUNT)
#define IDX_STATE_126   (IDX_STATE_125 + STATE_125_COUNT)
#define IDX_STATE_127   (IDX_STATE_126 + STATE_126_COUNT)
#define IDX_STATE_128   (IDX_STATE_127 + STATE_127_COUNT)
#define IDX_STATE_129   (IDX_STATE_128 + STATE_128_COUNT)
#define IDX_STATE_130   (IDX_STATE_129 + STATE_129_COUNT)
#define IDX_STATE_131   (IDX_STATE_130 + STATE_130_COUNT)
#define IDX_STATE_132   (IDX_STATE_131 + STATE_131_COUNT)
#define IDX_STATE_133   (IDX_STATE_132 + STATE_132_COUNT)
#define IDX_STATE_134   (IDX_STATE_133 + STATE_133_COUNT)
#define IDX_STATE_135   (IDX_STATE_134 + STATE_134_COUNT)
#define IDX_STATE_136   (IDX_STATE_135 + STATE_135_COUNT)
#define IDX_STATE_137   (IDX_STATE_136 + STATE_136_COUNT)
#define IDX_STATE_138   (IDX_STATE_137 + STATE_137_COUNT)
#define IDX_STATE_139   (IDX_STATE_138 + STATE_138_COUNT)
#define IDX_STATE_140   (IDX_STATE_139 + STATE_139_COUNT)
#define IDX_STATE_141   (IDX_STATE_140 + STATE_140_COUNT)
#define IDX_STATE_142   (IDX_STATE_141 + STATE_141_COUNT)
#define IDX_STATE_143   (IDX_STATE_142 + STATE_142_COUNT)
#define IDX_STATE_144   (IDX_STATE_143 + STATE_143_COUNT)
#define IDX_STATE_145   (IDX_STATE_144 + STATE_144_COUNT)
#define IDX_STATE_146   (IDX_STATE_145 + STATE_145_COUNT)
#define IDX_STATE_147   (IDX_STATE_146 + STATE_146_COUNT)
#define IDX_STATE_148   (IDX_STATE_147 + STATE_147_COUNT)
#define IDX_STATE_149   (IDX_STATE_148 + STATE_148_COUNT)
#define IDX_STATE_150   (IDX_STATE_149 + STATE_149_COUNT)
#define IDX_STATE_151   (IDX_STATE_150 + STATE_150_COUNT)
#define IDX_STATE_152   (IDX_STATE_151 + STATE_151_COUNT)
#define IDX_STATE_153   (IDX_STATE_152 + STATE_152_COUNT)
#define IDX_STATE_154   (IDX_STATE_153 + STATE_153_COUNT)
#define IDX_STATE_155   (IDX_STATE_154 + STATE_154_COUNT)
#define IDX_STATE_156   (IDX_STATE_155 + STATE_155_COUNT)
#define IDX_STATE_157   (IDX_STATE_156 + STATE_156_COUNT)
#define IDX_STATE_158   (IDX_STATE_157 + STATE_157_COUNT)
#define IDX_STATE_159   (IDX_STATE_158 + STATE_158_COUNT)
#define IDX_STATE_160   (IDX_STATE_159 + STATE_159_COUNT)
#define IDX_STATE_161   (IDX_STATE_160 + STATE_160_COUNT)
#define IDX_STATE_162   (IDX_STATE_161 + STATE_161_COUNT)
#define IDX_STATE_163   (IDX_STATE_162 + STATE_162_COUNT)
#define IDX_STATE_164   (IDX_STATE_163 + STATE_163_COUNT)
#define IDX_STATE_165   (IDX_STATE_164 + STATE_164_COUNT)
#define IDX_STATE_166   (IDX_STATE_165 + STATE_165_COUNT)
#define IDX_STATE_167   (IDX_STATE_166 + STATE_166_COUNT)
#define IDX_STATE_168   (IDX_STATE_167 + STATE_167_COUNT)
#define IDX_STATE_169   (IDX_STATE_168 + STATE_168_COUNT)
#define IDX_STATE_170   (IDX_STATE_169 + STATE_169_COUNT)
#define IDX_STATE_171   (IDX_STATE_170 + STATE_170_COUNT)
#define IDX_STATE_172   (IDX_STATE_171 + STATE_171_COUNT)
#define IDX_STATE_173   (IDX_STATE_172 + STATE_172_COUNT)
#define IDX_STATE_174   (IDX_STATE_173 + STATE_173_COUNT)
#define IDX_STATE_175   (IDX_STATE_174 + STATE_174_COUNT)
#define IDX_STATE_176   (IDX_STATE_175 + STATE_175_COUNT)
#define IDX_STATE_177   (IDX_STATE_176 + STATE_176_COUNT)
#define IDX_STATE_178   (IDX_STATE_177 + STATE_177_COUNT)
#define IDX_STATE_179   (IDX_STATE_178 + STATE_178_COUNT)
#define IDX_STATE_180   (IDX_STATE_179 + STATE_179_COUNT)
#define IDX_STATE_181   (IDX_STATE_180 + STATE_180_COUNT)
#define IDX_STATE_182   (IDX_STATE_181 + STATE_181_COUNT)
#define IDX_STATE_183   (IDX_STATE_182 + STATE_182_COUNT)
#define IDX_STATE_184   (IDX_STATE_183 + STATE_183_COUNT)
#define IDX_STATE_185   (IDX_STATE_184 + STATE_184_COUNT)
#define IDX_STATE_186   (IDX_STATE_185 + STATE_185_COUNT)
#define IDX_STATE_187   (IDX_STATE_186 + STATE_186_COUNT)
#define IDX_STATE_188   (IDX_STATE_187 + STATE_187_COUNT)
#define IDX_STATE_189   (IDX_STATE_188 + STATE_188_COUNT)
#define IDX_STATE_190   (IDX_STATE_189 + STATE_189_COUNT)
#define IDX_STATE_191   (IDX_STATE_190 + STATE_190_COUNT)
#define IDX_STATE_192   (IDX_STATE_191 + STATE_191_COUNT)
#define IDX_STATE_193   (IDX_STATE_192 + STATE_192_COUNT)
#define IDX_STATE_194   (IDX_STATE_193 + STATE_193_COUNT)
#define IDX_STATE_195   (IDX_STATE_194 + STATE_194_COUNT)
#define IDX_STATE_196   (IDX_STATE_195 + STATE_195_COUNT)
#define IDX_STATE_197   (IDX_STATE_196 + STATE_196_COUNT)
#define IDX_STATE_198   (IDX_STATE_197 + STATE_197_COUNT)
#define IDX_STATE_199   (IDX_STATE_198 + STATE_198_COUNT)
#define IDX_STATE_200   (IDX_STATE_199 + STATE_199_COUNT)
#define IDX_STATE_201   (IDX_STATE_200 + STATE_200_COUNT)
#define IDX_STATE_202   (IDX_STATE_201 + STATE_201_COUNT)
#define IDX_STATE_203   (IDX_STATE_202 + STATE_202_COUNT)
#define IDX_STATE_204   (IDX_STATE_203 + STATE_203_COUNT)
#define IDX_STATE_205   (IDX_STATE_204 + STATE_204_COUNT)
#define IDX_STATE_206   (IDX_STATE_205 + STATE_205_COUNT)
#define IDX_STATE_207   (IDX_STATE_206 + STATE_206_COUNT)
#define IDX_STATE_208   (IDX_STATE_207 + STATE_207_COUNT)
#define IDX_STATE_209   (IDX_STATE_208 + STATE_208_COUNT)
#define IDX_STATE_210   (IDX_STATE_209 + STATE_209_COUNT)
#define IDX_STATE_211   (IDX_STATE_210 + STATE_210_COUNT)
#define IDX_STATE_212   (IDX_STATE_211 + STATE_211_COUNT)
#define IDX_STATE_213   (IDX_STATE_212 + STATE_212_COUNT)
#define IDX_STATE_214   (IDX_STATE_213 + STATE_213_COUNT)
#define IDX_STATE_215   (IDX_STATE_214 + STATE_214_COUNT)
#define IDX_STATE_216   (IDX_STATE_215 + STATE_215_COUNT)
#define IDX_STATE_217   (IDX_STATE_216 + STATE_216_COUNT)
#define IDX_STATE_218   (IDX_STATE_217 + STATE_217_COUNT)
#define IDX_STATE_219   (IDX_STATE_218 + STATE_218_COUNT)
#define IDX_STATE_220   (IDX_STATE_219 + STATE_219_COUNT)
#define IDX_STATE_221   (IDX_STATE_220 + STATE_220_COUNT)
#define IDX_STATE_222   (IDX_STATE_221 + STATE_221_COUNT)
#define IDX_STATE_223   (IDX_STATE_222 + STATE_222_COUNT)
#define IDX_STATE_224   (IDX_STATE_223 + STATE_223_COUNT)
#define IDX_STATE_225   (IDX_STATE_224 + STATE_224_COUNT)
#define IDX_STATE_226   (IDX_STATE_225 + STATE_225_COUNT)
#define IDX_STATE_227   (IDX_STATE_226 + STATE_226_COUNT)
#define IDX_STATE_228   (IDX_STATE_227 + STATE_227_COUNT)
#define IDX_STATE_229   (IDX_STATE_228 + STATE_228_COUNT)
#define IDX_STATE_230   (IDX_STATE_229 + STATE_229_COUNT)
#define IDX_STATE_231   (IDX_STATE_230 + STATE_230_COUNT)
#define IDX_STATE_232   (IDX_STATE_231 + STATE_231_COUNT)
#define IDX_STATE_233   (IDX_STATE_232 + STATE_232_COUNT)
#define IDX_STATE_234   (IDX_STATE_233 + STATE_233_COUNT)
#define IDX_STATE_235   (IDX_STATE_234 + STATE_234_COUNT)
#define IDX_STATE_236   (IDX_STATE_235 + STATE_235_COUNT)
#define IDX_STATE_237   (IDX_STATE_236 + STATE_236_COUNT)
#define IDX_STATE_238   (IDX_STATE_237 + STATE_237_COUNT)
#define IDX_STATE_239   (IDX_STATE_238 + STATE_238_COUNT)
#define IDX_STATE_240   (IDX_STATE_239 + STATE_239_COUNT)
#define IDX_STATE_241   (IDX_STATE_240 + STATE_240_COUNT)
#define IDX_STATE_242   (IDX_STATE_241 + STATE_241_COUNT)
#define IDX_STATE_243   (IDX_STATE_242 + STATE_242_COUNT)
#define IDX_STATE_244   (IDX_STATE_243 + STATE_243_COUNT)
#define IDX_STATE_245   (IDX_STATE_244 + STATE_244_COUNT)
#define IDX_STATE_246   (IDX_STATE_245 + STATE_245_COUNT)
#define IDX_STATE_247   (IDX_STATE_246 + STATE_246_COUNT)
#define IDX_STATE_248   (IDX_STATE_247 + STATE_247_COUNT)
#define IDX_STATE_249   (IDX_STATE_248 + STATE_248_COUNT)
#define IDX_STATE_250   (IDX_STATE_249 + STATE_249_COUNT)
#define IDX_STATE_251   (IDX_STATE_250 + STATE_250_COUNT)
#define IDX_STATE_252   (IDX_STATE_251 + STATE_251_COUNT)
#define IDX_STATE_253   (IDX_STATE_252 + STATE_252_COUNT)
#define IDX_STATE_254   (IDX_STATE_253 + STATE_253_COUNT)
#define IDX_STATE_255   (IDX_STATE_254 + STATE_254_COUNT)
#define IDX_STATE_256   (IDX_STATE_255 + STATE_255_COUNT)
#define IDX_STATE_257   (IDX_STATE_256 + STATE_256_COUNT)
#define IDX_STATE_258   (IDX_STATE_257 + STATE_257_COUNT)
#define IDX_STATE_259   (IDX_STATE_258 + STATE_258_COUNT)
#define IDX_STATE_260   (IDX_STATE_259 + STATE_259_COUNT)
#define IDX_STATE_261   (IDX_STATE_260 + STATE_260_COUNT)
#define IDX_STATE_262   (IDX_STATE_261 + STATE_261_COUNT)
#define IDX_STATE_263   (IDX_STATE_262 + STATE_262_COUNT)
#define IDX_STATE_264   (IDX_STATE_263 + STATE_263_COUNT)
#define IDX_STATE_265   (IDX_STATE_264 + STATE_264_COUNT)
#define IDX_STATE_266   (IDX_STATE_265 + STATE_265_COUNT)
#define IDX_STATE_267   (IDX_STATE_266 + STATE_266_COUNT)

/* State -> first action index mapping (using chained macros)
 *
 * To modify a state's action count, update STATE_N_COUNT above.
 * All downstream indices recalculate automatically.
 */
hic_manens i32 ACTIO_INDICES[] = {
    IDX_STATE_0,    /* initial state */
    IDX_STATE_1,    /* after expression */
    IDX_STATE_2,    /* after term */
    IDX_STATE_3,    /* after factor */
    IDX_STATE_4,    /* after IDENTIFIER */
    IDX_STATE_5,    /* after INTEGER */
    IDX_STATE_6,    /* after '(' */
    IDX_STATE_7,    /* after unary '*' */
    IDX_STATE_8,    /* after unary '&' */
    IDX_STATE_9,    /* after expr '+' */
    IDX_STATE_10,   /* after term '*' */
    IDX_STATE_11,   /* after '( expr' */
    IDX_STATE_12,   /* after '( expr' (term reduce) */
    IDX_STATE_13,   /* after 'expr + term' */
    IDX_STATE_14,   /* after 'term * factor' */
    IDX_STATE_15,   /* after '* factor' (unary) */
    IDX_STATE_16,   /* after '& factor' (unary) */
    IDX_STATE_17,   /* after '*' in declarator path */
    IDX_STATE_18,   /* after declarator IDENTIFIER (reduce P12) */
    IDX_STATE_19,   /* after '* declarator' (reduce P11) */
    IDX_STATE_20,   /* after 'type_specifier declarator' (reduce P10) */
    IDX_STATE_21,   /* accept declaration */
    IDX_STATE_22,   /* reduce P13 (expression statement) */
    IDX_STATE_23,   /* reduce P14 (empty statement) */
    IDX_STATE_24,   /* accept statement */
    IDX_STATE_25,   /* after '{' (epsilon reduce P18) */
    IDX_STATE_26,   /* after '{ statement_list' */
    IDX_STATE_27,   /* reduce P16 (compound_statement) */
    IDX_STATE_28,   /* reduce P17 (statement_list) */
    IDX_STATE_29,   /* accept/reduce P15 (compound) */
    IDX_STATE_30,   /* after 'if' */
    IDX_STATE_31,   /* after 'if (' */
    IDX_STATE_32,   /* after 'if ( expr' */
    IDX_STATE_33,   /* after 'if ( expr )' */
    IDX_STATE_34,   /* after 'if ( expr ) stmt' (dangling else) */
    IDX_STATE_35,   /* after 'if ( expr ) stmt else' */
    IDX_STATE_36,   /* reduce P21 (if-else) */
    IDX_STATE_37,   /* reduce P19 (if statement) */
    IDX_STATE_38,   /* reduce P15 for nested compound */
    IDX_STATE_39,   /* after 'while' */
    IDX_STATE_40,   /* after 'while (' */
    IDX_STATE_41,   /* after 'while ( expr' */
    IDX_STATE_42,   /* after 'while ( expr )' */
    IDX_STATE_43,   /* reduce P23 (while body) */
    IDX_STATE_44,   /* reduce P22 (while statement) */
    IDX_STATE_45,   /* after 'do' body */
    IDX_STATE_46,   /* after 'do stmt' */
    IDX_STATE_47,   /* after 'do stmt while' */
    IDX_STATE_48,   /* after 'do stmt while (' */
    IDX_STATE_49,   /* after 'do stmt while ( expr' */
    IDX_STATE_50,   /* after 'do stmt while ( expr )' */
    IDX_STATE_51,   /* reduce P25 (do-while body) */
    IDX_STATE_52,   /* reduce P24 (do-while statement) */
    IDX_STATE_53,   /* after 'for' */
    IDX_STATE_54,   /* after 'for (' */
    IDX_STATE_55,   /* after 'for ( expr' */
    IDX_STATE_56,   /* after 'for ( expr_opt' */
    IDX_STATE_57,   /* after 'for ( expr_opt ;' */
    IDX_STATE_58,   /* after 'for ( ... ; expr' */
    IDX_STATE_59,   /* after 'for ( ... ; expr_opt' */
    IDX_STATE_60,   /* after 'for ( ... ; expr_opt ;' */
    IDX_STATE_61,   /* after 'for ( ... ; ... ; expr' */
    IDX_STATE_62,   /* after 'for ( ... ; ... ; expr_opt' */
    IDX_STATE_63,   /* after 'for ( ... )' */
    IDX_STATE_64,   /* reduce P27 (for body) */
    IDX_STATE_65,   /* reduce P26 (for statement) */
    IDX_STATE_66,   /* after 'break' */
    IDX_STATE_67,   /* reduce P30 (break) */
    IDX_STATE_68,   /* after 'continue' */
    IDX_STATE_69,   /* reduce P31 (continue) */
    IDX_STATE_70,   /* after 'return' */
    IDX_STATE_71,   /* after 'return expr' */
    IDX_STATE_72,   /* after 'return expr_opt' */
    IDX_STATE_73,   /* reduce P32 (return) */
    IDX_STATE_74,   /* after 'goto' */
    IDX_STATE_75,   /* after 'goto ID' */
    IDX_STATE_76,   /* reduce P33 (goto) */
    IDX_STATE_77,   /* after 'ID :' (labeled statement) */
    IDX_STATE_78,   /* reduce P34 (labeled statement) */
    IDX_STATE_79,   /* after 'switch' */
    IDX_STATE_80,   /* after 'switch (' */
    IDX_STATE_81,   /* after 'switch ( expr' */
    IDX_STATE_82,   /* after 'switch ( expr )' */
    IDX_STATE_83,   /* reduce P35 (switch) */
    IDX_STATE_84,   /* after 'case' */
    IDX_STATE_85,   /* after 'case expr' */
    IDX_STATE_86,   /* after 'case expr :' */
    IDX_STATE_87,   /* reduce P36 (case) */
    IDX_STATE_88,   /* after 'default' */
    IDX_STATE_89,   /* after 'default :' */
    IDX_STATE_90,   /* reduce P37 (default) */
    IDX_STATE_91,   /* after 'declarator (' */
    IDX_STATE_92,   /* reduce P38 (fn decl empty params) */
    IDX_STATE_93,   /* after 'declarator ( void' */
    IDX_STATE_94,   /* reduce P39 (fn decl void) */
    IDX_STATE_95,   /* after '( type_spec' (param) */
    IDX_STATE_96,   /* after '( type_spec *' (param) */
    IDX_STATE_97,   /* reduce P12 (param declarator) */
    IDX_STATE_98,   /* reduce P12 after * (param declarator) */
    IDX_STATE_99,   /* reduce P43 (parameter_declaration) */
    IDX_STATE_100,  /* reduce P11 (pointer param declarator) */
    IDX_STATE_101,  /* reduce P41 (parameter_list start) */
    IDX_STATE_102,  /* after param_list */
    IDX_STATE_103,  /* reduce P40 (fn decl with params) */
    IDX_STATE_104,  /* after 'param_list ,' */
    IDX_STATE_105,  /* after ', type_spec' */
    IDX_STATE_106,  /* after ', type_spec *' */
    IDX_STATE_107,  /* reduce P12 (subsequent param) */
    IDX_STATE_108,  /* reduce P12 after * (subsequent param) */
    IDX_STATE_109,  /* reduce P43 (subsequent param_decl) */
    IDX_STATE_110,  /* reduce P11 (subsequent pointer param) */
    IDX_STATE_111,  /* reduce P42 (parameter_list extend) */
    IDX_STATE_112,  /* (unused) */
    IDX_STATE_113,  /* reduce P44 (function_definition) */
    IDX_STATE_114,  /* accept function_definition */
    IDX_STATE_115,  /* (unused) */
    IDX_STATE_116,  /* reduce P12 (direct declarator) */
    IDX_STATE_117,  /* after 'struct' */
    IDX_STATE_118,  /* after 'struct ID' */
    IDX_STATE_119,  /* after 'struct {' */
    IDX_STATE_120,  /* after 'struct ID {' */
    IDX_STATE_121,  /* after member type_spec */
    IDX_STATE_122,  /* after member type_spec * */
    IDX_STATE_123,  /* after member type_spec name */
    IDX_STATE_124,  /* after member type_spec * name */
    IDX_STATE_125,  /* after member ; (first) */
    IDX_STATE_126,  /* after pointer member ; (first) */
    IDX_STATE_127,  /* after member_list (anon struct) */
    IDX_STATE_128,  /* after anon struct } */
    IDX_STATE_129,  /* after member_list (named struct) */
    IDX_STATE_130,  /* after named struct } */
    IDX_STATE_131,  /* subsequent member type_spec */
    IDX_STATE_132,  /* subsequent member type_spec * */
    IDX_STATE_133,  /* subsequent member type_spec name */
    IDX_STATE_134,  /* subsequent member type_spec * name */
    IDX_STATE_135,  /* subsequent member ; */
    IDX_STATE_136,  /* subsequent pointer member ; */
    IDX_STATE_137,  /* after 'union' */
    IDX_STATE_138,  /* after 'union ID' */
    IDX_STATE_139,  /* after 'union {' */
    IDX_STATE_140,  /* after 'union ID {' */
    IDX_STATE_141,  /* after member_list (anon union) */
    IDX_STATE_142,  /* after anon union } */
    IDX_STATE_143,  /* after member_list (named union) */
    IDX_STATE_144,  /* after named union } */
    IDX_STATE_145,  /* after 'enum' */
    IDX_STATE_146,  /* after 'enum ID' */
    IDX_STATE_147,  /* after 'enum {' */
    IDX_STATE_148,  /* after 'enum ID {' */
    IDX_STATE_149,  /* after enumerator ID */
    IDX_STATE_150,  /* after 'ID =' (enum value) */
    IDX_STATE_151,  /* after 'ID = expr' (enum value) */
    IDX_STATE_152,  /* after first enumerator */
    IDX_STATE_153,  /* after enum_list (anon) */
    IDX_STATE_154,  /* after 'enum { list }' */
    IDX_STATE_155,  /* after enum_list (named) */
    IDX_STATE_156,  /* after ',' (enum) */
    IDX_STATE_157,  /* after 'enum ID { list }' */
    IDX_STATE_158,  /* after subsequent enumerator ID */
    IDX_STATE_159,  /* after subsequent 'ID =' */
    IDX_STATE_160,  /* after subsequent 'ID = expr' */
    IDX_STATE_161,  /* after subsequent enumerator */
    IDX_STATE_162,  /* first named bit field ':' */
    IDX_STATE_163,  /* first named bit field expr done */
    IDX_STATE_164,  /* reduce P62 (first named bit field) */
    IDX_STATE_165,  /* subsequent named bit field ':' */
    IDX_STATE_166,  /* subsequent named bit field expr done */
    IDX_STATE_167,  /* reduce P63 (subsequent named bit field) */
    IDX_STATE_168,  /* first anon bit field ':' */
    IDX_STATE_169,  /* first anon bit field expr done */
    IDX_STATE_170,  /* reduce P64 (first anon bit field) */
    IDX_STATE_171,  /* subsequent anon bit field ':' */
    IDX_STATE_172,  /* subsequent anon bit field expr done */
    IDX_STATE_173,  /* reduce P65 (subsequent anon bit field) */
    /* ========== NESTED TYPE MEMBER STATES (E5) ========== */
    IDX_STATE_174,  /* after struct_spec (first member) */
    IDX_STATE_175,  /* after struct_spec * (first member) */
    IDX_STATE_176,  /* after struct_spec ID (first member) */
    IDX_STATE_177,  /* after struct_spec * ID (first member) */
    IDX_STATE_178,  /* reduce P66 (nested struct member) */
    IDX_STATE_179,  /* reduce P67 (nested struct ptr member) */
    IDX_STATE_180,  /* after struct_spec (subsequent member) */
    IDX_STATE_181,  /* after struct_spec * (subsequent member) */
    IDX_STATE_182,  /* after struct_spec ID (subsequent member) */
    IDX_STATE_183,  /* after struct_spec * ID (subsequent member) */
    IDX_STATE_184,  /* reduce P68 (subsequent nested struct) */
    IDX_STATE_185,  /* reduce P69 (subsequent nested struct ptr) */
    IDX_STATE_186,  /* after enum_spec (first member) */
    IDX_STATE_187,  /* after enum_spec * (first member) */
    IDX_STATE_188,  /* after enum_spec ID (first member) */
    IDX_STATE_189,  /* after enum_spec * ID (first member) */
    IDX_STATE_190,  /* reduce P70 (nested enum member) */
    IDX_STATE_191,  /* reduce P71 (nested enum ptr member) */
    IDX_STATE_192,  /* after enum_spec (subsequent member) */
    IDX_STATE_193,  /* after enum_spec * (subsequent member) */
    IDX_STATE_194,  /* after enum_spec ID (subsequent member) */
    IDX_STATE_195,  /* after enum_spec * ID (subsequent member) */
    IDX_STATE_196,  /* reduce P72 (subsequent nested enum) */
    IDX_STATE_197,  /* reduce P73 (subsequent nested enum ptr) */
    /* ========== TYPEDEF DECLARATION STATES (E6) ========== */
    IDX_STATE_198,  /* after 'typedef' */
    IDX_STATE_199,  /* after 'typedef type_spec' */
    IDX_STATE_200,  /* after 'typedef type_spec *' */
    IDX_STATE_201,  /* after 'typedef type_spec ID' */
    IDX_STATE_202,  /* after 'typedef type_spec *... ID' */
    IDX_STATE_203,  /* reduce P74 (typedef simple) */
    IDX_STATE_204,  /* reduce P75 (typedef pointer) */
    IDX_STATE_205,  /* after 'typedef struct_spec' */
    IDX_STATE_206,  /* after 'typedef struct_spec *' */
    IDX_STATE_207,  /* after 'typedef struct_spec ID' */
    IDX_STATE_208,  /* after 'typedef struct_spec *... ID' */
    IDX_STATE_209,  /* reduce P76 (typedef struct) */
    IDX_STATE_210,  /* reduce P77 (typedef struct ptr) */
    IDX_STATE_211,  /* after 'typedef enum_spec' */
    IDX_STATE_212,  /* after 'typedef enum_spec *' */
    IDX_STATE_213,  /* after 'typedef enum_spec ID' */
    IDX_STATE_214,  /* after 'typedef enum_spec *... ID' */
    IDX_STATE_215,  /* reduce P78 (typedef enum) */
    IDX_STATE_216,  /* reduce P79 (typedef enum ptr) */
    IDX_STATE_217,  /* after 'declarator [' - array dimension start */
    IDX_STATE_218,  /* after 'declarator []' - unsized array reduce */
    IDX_STATE_219,  /* after 'declarator [ expr' - expect ] */
    IDX_STATE_220,  /* after 'declarator [ expr ]' - sized array reduce */
    IDX_STATE_221,  /* after type_spec declarator in struct (first member) */
    IDX_STATE_222,  /* reduce P48 (first member with declarator) */
    IDX_STATE_223,  /* after list type_spec declarator (subsequent member) */
    IDX_STATE_224,  /* reduce P49 (subsequent member with declarator) */
    IDX_STATE_225,  /* after typedef type_spec declarator */
    IDX_STATE_226,  /* reduce P74 (typedef with declarator) */
    IDX_STATE_227,  /* after typedef struct_spec declarator */
    IDX_STATE_228,  /* reduce P76 (typedef struct with declarator) */
    IDX_STATE_229,  /* after typedef enum_spec declarator */
    IDX_STATE_230,  /* reduce P78 (typedef enum with declarator) */
    IDX_STATE_231,  /* after struct_spec declarator (first nested) */
    IDX_STATE_232,  /* reduce P66 (first nested struct member) */
    IDX_STATE_233,  /* after list struct_spec declarator (subsequent nested) */
    IDX_STATE_234,  /* reduce P68 (subsequent nested struct member) */
    IDX_STATE_235,  /* after enum_spec declarator (first nested) */
    IDX_STATE_236,  /* reduce P70 (first nested enum member) */
    IDX_STATE_237,  /* after list enum_spec declarator (subsequent nested) */
    IDX_STATE_238,  /* reduce P72 (subsequent nested enum member) */
    IDX_STATE_239,  /* E9.2: after comparatio */
    IDX_STATE_240,  /* E9.2: after aequalitas (accept) */
    IDX_STATE_241,  /* E9.2: after comparison op */
    IDX_STATE_242,  /* E9.2: after equality op */
    IDX_STATE_243,  /* E9.2: after comp op expr (reduce P88) */
    IDX_STATE_244,  /* E9.2: after eq op comparatio (reduce P85) */
    IDX_STATE_245,  /* E9.2: after aequalitas inside parens */
    IDX_STATE_246,  /* E9.2: after comparatio in if condition */
    IDX_STATE_247,  /* E9.2: after aequalitas in if condition */
    IDX_STATE_248,  /* E9.2: after aequalitas in while condition */
    IDX_STATE_249,  /* E9.2: after comparatio in for condition */
    IDX_STATE_250,  /* E9.2: after aequalitas in for condition */
    IDX_STATE_251,  /* E9.2: after aequalitas in do-while condition */
    IDX_STATE_252,  /* E9.2: after aequalitas in switch condition */
    IDX_STATE_253,  /* E10: after coniunctio at top-level */
    IDX_STATE_254,  /* E10: after 'coniunctio &&' */
    IDX_STATE_255,  /* E10: after disiunctio at top-level (accept) */
    IDX_STATE_256,  /* E10: after 'disiunctio ||' */
    IDX_STATE_257,  /* E10: after aequalitas in && context */
    IDX_STATE_258,  /* E10: after 'coniunctio && aequalitas' */
    IDX_STATE_259,  /* E10: after aequalitas in || context */
    IDX_STATE_260,  /* E10: after coniunctio in || context */
    IDX_STATE_261,  /* E10: after 'coniunctio &&' in || context */
    IDX_STATE_262,  /* E10: after aequalitas in && within || */
    IDX_STATE_263,  /* E10: after 'coniunctio && aequalitas' in || */
    IDX_STATE_264,  /* after translatio */
    IDX_STATE_265,  /* after translatio << or >> */
    IDX_STATE_266,  /* after translatio << expression */
    IDX_STATE_267,  /* after translatio in 'comp <' context */
    (IDX_STATE_267 + STATE_267_COUNT) /* End marker */
};

/* NUM_STATES derived from array size (array has NUM_STATES + 1 entries for end marker) */
#define NUM_STATES ((i32)(magnitudo(ACTIO_INDICES) / magnitudo(ACTIO_INDICES[0])) - 1)

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
#define INT_NT_AEQUALITAS    22
#define INT_NT_COMPARATIO    23
#define INT_NT_CONIUNCTIO    24
#define INT_NT_DISIUNCTIO    25
#define INT_NT_TRANSLATIO    26

hic_manens Arbor2TabulaGoto GOTO_TABULA[] = {
    /* From state 0 */
    { 0, INT_NT_EXPR,   1 },
    { 0, INT_NT_TERM,   2 },
    { 0, INT_NT_FACTOR, 3 },
    { 0, INT_NT_COMPARATIO, 239 },      /* E9.2: after comparatio */
    { 0, INT_NT_AEQUALITAS, 240 },      /* E9.2: after aequalitas */
    { 0, INT_NT_CONIUNCTIO, 253 },      /* E10: after coniunctio */
    { 0, INT_NT_DISIUNCTIO, 255 },      /* E10: after disiunctio (accept) */

    /* From state 6: after '(' - full expression chain inside parens */
    { 6, INT_NT_EXPR,   11 },
    { 6, INT_NT_TERM,   2 },
    { 6, INT_NT_FACTOR, 3 },
    { 6, INT_NT_COMPARATIO, 239 },      /* E9.2 */
    { 6, INT_NT_AEQUALITAS, 245 },      /* E9.2: inside-parens aequalitas */
    { 6, INT_NT_CONIUNCTIO, 253 },      /* E10: coniunctio inside parens */
    { 6, INT_NT_DISIUNCTIO, 255 },      /* E10: disiunctio inside parens */

    /* From state 7: after unary '*' */
    { 7, INT_NT_FACTOR, 15 },

    /* From state 8: after unary '&' */
    { 8, INT_NT_FACTOR, 16 },

    /* From state 9: after expr '+' */
    { 9, INT_NT_TERM,   13 },
    { 9, INT_NT_FACTOR, 3 },

    /* From state 10: after term '*' */
    { 10, INT_NT_FACTOR, 14 },

    /* From state 241: after 'comparatio <|>|<=|>=' - E9.2 */
    { 241, INT_NT_EXPR,   243 },        /* RHS expression → state 247 (reduce P88) */
    { 241, INT_NT_TERM,   2 },
    { 241, INT_NT_FACTOR, 3 },

    /* From state 242: after 'aequalitas ==|!=' - E9.2 */
    { 242, INT_NT_EXPR,   1 },          /* parse expression, then chain to comparatio */
    { 242, INT_NT_TERM,   2 },
    { 242, INT_NT_FACTOR, 3 },
    { 242, INT_NT_COMPARATIO, 244 },    /* RHS comparatio → state 244 (reduce P85) */

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
    { 26, INT_NT_COMPARATIO, 239 }, /* E9.2: comparatio in compound */
    { 26, INT_NT_AEQUALITAS, 240 }, /* E9.2: aequalitas in compound */
    { 26, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in compound */
    { 26, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in compound -> shift ';' for stmt */
    { 26, INT_NT_SENTENTIA, 28 },   /* statement in list */
    { 26, INT_NT_CORPUS,    29 },   /* nested compound */
    { 26, INT_NT_SI,        37 },   /* if statement in list */

    /* From state 0: after if_statement */
    { 0, INT_NT_SI,         37 },

    /* From state 31: after 'if (' - expression components */
    { 31, INT_NT_EXPR,      32 },   /* condition expression */
    { 31, INT_NT_TERM,      2 },    /* reuse term state */
    { 31, INT_NT_FACTOR,    3 },    /* reuse factor state */
    { 31, INT_NT_COMPARATIO, 246 }, /* E9.2: after comparatio in if condition */
    { 31, INT_NT_AEQUALITAS, 247 }, /* E9.2: after aequalitas in if condition */
    { 31, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in if condition */
    { 31, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in if condition */

    /* From state 33: after 'if ( expr )' - then-branch statement */
    { 33, INT_NT_EXPR,      1 },    /* expression in then-branch */
    { 33, INT_NT_TERM,      2 },    /* term in expression */
    { 33, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 33, INT_NT_COMPARATIO, 239 }, /* E9.2: comparatio in then-branch */
    { 33, INT_NT_AEQUALITAS, 240 }, /* E9.2: aequalitas in then-branch */
    { 33, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in then-branch */
    { 33, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in then-branch */
    { 33, INT_NT_SENTENTIA, 34 },   /* then-branch statement */
    { 33, INT_NT_CORPUS,    38 },   /* nested compound - reduce P15 first */
    { 33, INT_NT_SI,        37 },   /* nested if statement */

    /* From state 35: after 'if ( expr ) stmt else' - else-branch statement */
    { 35, INT_NT_EXPR,      1 },    /* expression in else-branch */
    { 35, INT_NT_TERM,      2 },    /* term in expression */
    { 35, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 35, INT_NT_COMPARATIO, 239 }, /* E9.2: comparatio in else-branch */
    { 35, INT_NT_AEQUALITAS, 240 }, /* E9.2: aequalitas in else-branch */
    { 35, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in else-branch */
    { 35, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in else-branch */
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
    { 40, INT_NT_COMPARATIO, 246 }, /* E9.2: after comparatio in while condition */
    { 40, INT_NT_AEQUALITAS, 248 }, /* E9.2: after aequalitas in while condition */
    { 40, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in while condition */
    { 40, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in while condition */

    /* From state 42: after 'while ( expr )' - loop body */
    { 42, INT_NT_EXPR,      1 },    /* expression in body */
    { 42, INT_NT_TERM,      2 },    /* term in expression */
    { 42, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 42, INT_NT_COMPARATIO, 239 }, /* E9.2: comparatio in while body */
    { 42, INT_NT_AEQUALITAS, 240 }, /* E9.2: aequalitas in while body */
    { 42, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in while body */
    { 42, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in while body */
    { 42, INT_NT_SENTENTIA, 43 },   /* loop body statement */
    { 42, INT_NT_CORPUS,    38 },   /* compound body */
    { 42, INT_NT_SI,        37 },   /* if in body */
    { 42, INT_NT_DUM,       44 },   /* nested while */
    { 42, INT_NT_FAC,       52 },   /* nested do-while */

    /* From state 45: after 'do' - loop body */
    { 45, INT_NT_EXPR,      1 },    /* expression in body */
    { 45, INT_NT_TERM,      2 },    /* term in expression */
    { 45, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 45, INT_NT_COMPARATIO, 239 }, /* E9.2: comparatio in do body */
    { 45, INT_NT_AEQUALITAS, 240 }, /* E9.2: aequalitas in do body */
    { 45, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in do body */
    { 45, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in do body */
    { 45, INT_NT_SENTENTIA, 46 },   /* loop body statement */
    { 45, INT_NT_CORPUS,    38 },   /* compound body */
    { 45, INT_NT_SI,        37 },   /* if in body */
    { 45, INT_NT_DUM,       44 },   /* nested while */
    { 45, INT_NT_FAC,       52 },   /* nested do-while */

    /* From state 48: after 'do stmt while (' - condition expression */
    { 48, INT_NT_EXPR,      49 },   /* condition expression */
    { 48, INT_NT_TERM,      2 },    /* term in expression */
    { 48, INT_NT_FACTOR,    3 },    /* factor in expression */
    { 48, INT_NT_COMPARATIO, 246 }, /* E9.2: after comparatio in do-while condition */
    { 48, INT_NT_AEQUALITAS, 251 }, /* E9.2: after aequalitas in do-while condition */
    { 48, INT_NT_CONIUNCTIO, 253 }, /* E10: coniunctio in do-while condition */
    { 48, INT_NT_DISIUNCTIO, 255 }, /* E10: disiunctio in do-while condition */

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
    { 57, INT_NT_COMPARATIO,        249 },  /* E9.2: comparatio in for condition */
    { 57, INT_NT_AEQUALITAS,        250 },  /* E9.2: aequalitas in for condition */
    { 57, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in for condition */
    { 57, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in for condition */
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
    { 63, INT_NT_COMPARATIO,        239 },  /* E9.2: comparatio in for body */
    { 63, INT_NT_AEQUALITAS,        240 },  /* E9.2: aequalitas in for body */
    { 63, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in for body */
    { 63, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in for body */
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
    { 77, INT_NT_COMPARATIO,        239 },  /* E9.2: comparatio in label body */
    { 77, INT_NT_AEQUALITAS,        240 },  /* E9.2: aequalitas in label body */
    { 77, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in label body */
    { 77, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in label body */
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
    { 80, INT_NT_COMPARATIO,        246 },  /* E9.2: after comparatio in switch condition */
    { 80, INT_NT_AEQUALITAS,        252 },  /* E9.2: after aequalitas in switch condition */
    { 80, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in switch condition */
    { 80, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in switch condition */

    /* From state 82: after 'switch ( expr )' - body statement */
    { 82, INT_NT_EXPR,              1 },    /* expression in body */
    { 82, INT_NT_TERM,              2 },    /* term in expression */
    { 82, INT_NT_FACTOR,            3 },    /* factor in expression */
    { 82, INT_NT_COMPARATIO,        239 },  /* E9.2: comparatio in switch body */
    { 82, INT_NT_AEQUALITAS,        240 },  /* E9.2: aequalitas in switch body */
    { 82, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in switch body */
    { 82, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in switch body */
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
    { 86, INT_NT_COMPARATIO,        239 },  /* E9.2: comparatio in case body */
    { 86, INT_NT_AEQUALITAS,        240 },  /* E9.2: aequalitas in case body */
    { 86, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in case body */
    { 86, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in case body */
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
    { 89, INT_NT_COMPARATIO,        239 },  /* E9.2: comparatio in default body */
    { 89, INT_NT_AEQUALITAS,        240 },  /* E9.2: aequalitas in default body */
    { 89, INT_NT_CONIUNCTIO,        253 },  /* E10: coniunctio in default body */
    { 89, INT_NT_DISIUNCTIO,        255 },  /* E10: disiunctio in default body */
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

    /* E8: Declarator in struct member context (first member) */
    { 121, INT_NT_DECLARATOR,       221 },  /* type_spec declarator → completion state */

    /* E8: Declarator in struct member context (subsequent member) */
    { 131, INT_NT_DECLARATOR,       223 },  /* list type_spec declarator → completion state */

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
    { 171, INT_NT_FACTOR,           3 },

    /* ========== TYPEDEF GOTO ENTRIES (E6) ========== */

    /* From state 198: after 'typedef' - struct/enum specifier reduces */
    { 198, INT_NT_STRUCT_SPEC,      205 },  /* typedef struct { } → ID path */
    { 198, INT_NT_ENUM_SPEC,        211 },  /* typedef enum { } → ID path */

    /* E8: Declarator in typedef context */
    { 199, INT_NT_DECLARATOR,       225 },  /* typedef type_spec declarator → completion */
    { 205, INT_NT_DECLARATOR,       227 },  /* typedef struct_spec declarator → completion */
    { 211, INT_NT_DECLARATOR,       229 },  /* typedef enum_spec declarator → completion */

    /* E8: Declarator in nested type member context */
    { 174, INT_NT_DECLARATOR,       231 },  /* struct_spec declarator (first) → completion */
    { 180, INT_NT_DECLARATOR,       233 },  /* struct_spec declarator (subseq) → completion */
    { 186, INT_NT_DECLARATOR,       235 },  /* enum_spec declarator (first) → completion */
    { 192, INT_NT_DECLARATOR,       237 },  /* enum_spec declarator (subseq) → completion */

    /* ==================================================
     * Array Declarator GOTO Entries (State 217)
     * ================================================== */

    /* From state 217: after 'declarator [' - expression for array size */
    { 217, INT_NT_EXPR,   219 },  /* array size expression -> state 219 */
    { 217, INT_NT_TERM,   2 },    /* reuse term state */
    { 217, INT_NT_FACTOR, 3 },    /* reuse factor state */

    /* ==================================================
     * E10: Logical Operators GOTO Entries
     * ================================================== */

    /* From state 254: after 'coniunctio &&' - parse RHS aequalitas */
    { 254, INT_NT_EXPR,       1 },
    { 254, INT_NT_TERM,       2 },
    { 254, INT_NT_FACTOR,     3 },
    { 254, INT_NT_COMPARATIO, 239 },     /* reuse comparatio state */
    { 254, INT_NT_AEQUALITAS, 257 },     /* RHS aequalitas in && context */
    { 254, INT_NT_CONIUNCTIO, 258 },     /* after building coniunctio -> reduce P95 */

    /* From state 256: after 'disiunctio ||' - parse RHS coniunctio */
    { 256, INT_NT_EXPR,       1 },
    { 256, INT_NT_TERM,       2 },
    { 256, INT_NT_FACTOR,     3 },
    { 256, INT_NT_COMPARATIO, 239 },
    { 256, INT_NT_AEQUALITAS, 259 },     /* aequalitas in || context -> reduce P96 */
    { 256, INT_NT_CONIUNCTIO, 260 },     /* coniunctio in || context -> reduce P93 or && */

    /* From state 261: after 'coniunctio &&' within || context */
    { 261, INT_NT_EXPR,       1 },
    { 261, INT_NT_TERM,       2 },
    { 261, INT_NT_FACTOR,     3 },
    { 261, INT_NT_COMPARATIO, 239 },
    { 261, INT_NT_AEQUALITAS, 262 },     /* aequalitas in && within || */
    { 261, INT_NT_CONIUNCTIO, 263 },     /* coniunctio from && within || -> reduce P95 */

    /* INT_NT_TRANSLATIO entries - after expression reduces to translatio */
    { 0, INT_NT_TRANSLATIO, 264 },
    { 6, INT_NT_TRANSLATIO, 264 },
    { 9, INT_NT_TRANSLATIO, 264 },       /* after + or - in expression */
    { 26, INT_NT_TRANSLATIO, 264 },
    { 31, INT_NT_TRANSLATIO, 264 },
    { 33, INT_NT_TRANSLATIO, 264 },
    { 35, INT_NT_TRANSLATIO, 264 },
    { 40, INT_NT_TRANSLATIO, 264 },
    { 42, INT_NT_TRANSLATIO, 264 },
    { 45, INT_NT_TRANSLATIO, 264 },
    { 48, INT_NT_TRANSLATIO, 264 },
    { 54, INT_NT_TRANSLATIO, 264 },      /* for init */
    { 57, INT_NT_TRANSLATIO, 264 },      /* for condition */
    { 60, INT_NT_TRANSLATIO, 264 },      /* for increment */
    { 63, INT_NT_TRANSLATIO, 264 },
    { 70, INT_NT_TRANSLATIO, 264 },      /* return */
    { 77, INT_NT_TRANSLATIO, 264 },
    { 80, INT_NT_TRANSLATIO, 264 },
    { 82, INT_NT_TRANSLATIO, 264 },
    { 84, INT_NT_TRANSLATIO, 264 },      /* case expression */
    { 86, INT_NT_TRANSLATIO, 264 },
    { 89, INT_NT_TRANSLATIO, 264 },
    { 150, INT_NT_TRANSLATIO, 264 },
    { 159, INT_NT_TRANSLATIO, 264 },
    { 162, INT_NT_TRANSLATIO, 264 },
    { 165, INT_NT_TRANSLATIO, 264 },
    { 168, INT_NT_TRANSLATIO, 264 },
    { 171, INT_NT_TRANSLATIO, 264 },
    { 217, INT_NT_TRANSLATIO, 264 },
    { 241, INT_NT_TRANSLATIO, 267 },     /* after comparison op RHS - goes to P88 state */
    { 242, INT_NT_TRANSLATIO, 264 },     /* after equality op RHS */
    { 254, INT_NT_TRANSLATIO, 264 },
    { 256, INT_NT_TRANSLATIO, 264 },
    { 261, INT_NT_TRANSLATIO, 264 },
    { 265, INT_NT_TRANSLATIO, 266 },     /* after translatio << expression -> reduce P97 */

    /* GOTO entries for state 265 (after translatio << or >>) */
    { 265, INT_NT_EXPR,   266 },         /* expression parsed after shift op -> state 266 (reduce P97) */
    { 265, INT_NT_TERM,   2 },
    { 265, INT_NT_FACTOR, 3 }
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
        casus ARBOR2_NT_AEQUALITAS:
            nt_int = INT_NT_AEQUALITAS;
            frange;
        casus ARBOR2_NT_COMPARATIO:
            nt_int = INT_NT_COMPARATIO;
            frange;
        casus ARBOR2_NT_TRANSLATIO:
            nt_int = INT_NT_TRANSLATIO;
            frange;
        casus ARBOR2_NT_CONIUNCTIO:
            nt_int = INT_NT_CONIUNCTIO;
            frange;
        casus ARBOR2_NT_DISIUNCTIO:
            nt_int = INT_NT_DISIUNCTIO;
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
 * Table Query API (no GLR instance required)
 * ================================================== */

i32
arbor2_tabula_numerus_statuum(vacuum)
{
    redde NUM_STATES;
}

i32
arbor2_tabula_numerus_regularum(vacuum)
{
    redde NUM_REGULAE;
}

i32
arbor2_tabula_numerus_actionum(vacuum)
{
    redde NUM_ACTIONES;
}

i32
arbor2_tabula_numerus_goto(vacuum)
{
    redde NUM_GOTO;
}

vacuum
arbor2_tabula_iterare_actiones_status(
    i32 status,
    vacuum (*callback)(constans Arbor2TabulaActio* actio, vacuum* ctx),
    vacuum* ctx)
{
    i32 initium;
    i32 finis;
    i32 i;

    si (status < ZEPHYRUM || status >= NUM_STATES)
    {
        redde;
    }

    initium = ACTIO_INDICES[status];
    finis = ACTIO_INDICES[status + I];

    per (i = initium; i < finis; i++)
    {
        callback(&ACTIONES[i], ctx);
    }
}

vacuum
arbor2_tabula_iterare_actiones_lexema(
    Arbor2LexemaGenus lexema,
    vacuum (*callback)(i32 status, constans Arbor2TabulaActio* actio, vacuum* ctx),
    vacuum* ctx)
{
    i32 status;
    i32 initium;
    i32 finis;
    i32 i;

    per (status = ZEPHYRUM; status < NUM_STATES; status++)
    {
        initium = ACTIO_INDICES[status];
        finis = ACTIO_INDICES[status + I];

        per (i = initium; i < finis; i++)
        {
            si (ACTIONES[i].lexema == lexema)
            {
                callback(status, &ACTIONES[i], ctx);
            }
        }
    }
}

vacuum
arbor2_tabula_iterare_goto_nt(
    s32 nt,
    vacuum (*callback)(constans Arbor2TabulaGoto* entry, vacuum* ctx),
    vacuum* ctx)
{
    i32 i;

    per (i = ZEPHYRUM; i < NUM_GOTO; i++)
    {
        si (GOTO_TABULA[i].non_terminalis == nt)
        {
            callback(&GOTO_TABULA[i], ctx);
        }
    }
}

Arbor2Regula*
arbor2_tabula_obtinere_regula(i32 index)
{
    si (index < ZEPHYRUM || index >= NUM_REGULAE)
    {
        redde NIHIL;
    }

    redde &REGULAE[index];
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
        casus ARBOR2_NT_COMPARATIO:     redde "COMPARATIO";
        casus ARBOR2_NT_AEQUALITAS:     redde "AEQUALITAS";
        casus ARBOR2_NT_CONIUNCTIO:     redde "CONIUNCTIO";
        casus ARBOR2_NT_DISIUNCTIO:     redde "DISIUNCTIO";
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
