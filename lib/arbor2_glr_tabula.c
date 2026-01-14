/* arbor2_glr_tabula.c - Hand-written LR tables for expression grammar */

/* Temporarily suppress warnings during refactor.
 * TODO: Remove these pragmas once Phase 3 (array-of-arrays) is complete. */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"
#pragma clang diagnostic ignored "-Wunused-const-variable"
#endif

#include "latina.h"
#include "arbor2_glr.h"
#include "xar.h"
#include <string.h>
#include <stdio.h>

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
    /* P95 */ { ARBOR2_NT_CONIUNCTIO, 3, ARBOR2_NODUS_BINARIUM, "and -> and '&&' pipa_bitwise" },
    /* P96 */ { ARBOR2_NT_CONIUNCTIO, 1, ARBOR2_NODUS_ERROR, "and -> pipa_bitwise" },
    /* P97 */ { ARBOR2_NT_TRANSLATIO, 3, ARBOR2_NODUS_BINARIUM, "shift -> shift '<<' expr" },
    /* P98 */ { ARBOR2_NT_TRANSLATIO, 3, ARBOR2_NODUS_BINARIUM, "shift -> shift '>>' expr" },
    /* P99 */ { ARBOR2_NT_TRANSLATIO, 1, ARBOR2_NODUS_ERROR, "shift -> expr" },

    /* Bitwise OR (|) - between && and ^ */
    /* P100 */ { ARBOR2_NT_PIPA_BITWISE, 3, ARBOR2_NODUS_BINARIUM, "pipa_bitwise -> pipa_bitwise '|' caret_bitwise" },
    /* P101 */ { ARBOR2_NT_PIPA_BITWISE, 1, ARBOR2_NODUS_ERROR, "pipa_bitwise -> caret_bitwise" },

    /* Bitwise XOR (^) - between | and & */
    /* P102 */ { ARBOR2_NT_CARET_BITWISE, 3, ARBOR2_NODUS_BINARIUM, "caret_bitwise -> caret_bitwise '^' ampersand_bitwise" },
    /* P103 */ { ARBOR2_NT_CARET_BITWISE, 1, ARBOR2_NODUS_ERROR, "caret_bitwise -> ampersand_bitwise" },

    /* Bitwise AND (&) - between ^ and == */
    /* P104 */ { ARBOR2_NT_AMPERSAND_BITWISE, 3, ARBOR2_NODUS_BINARIUM, "ampersand_bitwise -> ampersand_bitwise '&' aequalitas" },
    /* P105 */ { ARBOR2_NT_AMPERSAND_BITWISE, 1, ARBOR2_NODUS_ERROR, "ampersand_bitwise -> aequalitas" },

    /* Unary operators (bitwise NOT and logical NOT) */
    /* P106 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '~' factor" },
    /* P107 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '!' factor" },

    /* Comma operator (lowest precedence, left-associative) */
    /* P108 */ { ARBOR2_NT_VIRGA, 3, ARBOR2_NODUS_BINARIUM, "virga -> virga ',' assignatio" },
    /* P109 */ { ARBOR2_NT_VIRGA, 1, ARBOR2_NODUS_ERROR, "virga -> assignatio" },

    /* Assignment operators (right-associative) - now from ternarius, not disiunctio */
    /* P110 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '=' assignatio" },
    /* P111 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '+=' assignatio" },
    /* P112 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '-=' assignatio" },
    /* P113 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '*=' assignatio" },
    /* P114 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '/=' assignatio" },
    /* P115 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '%=' assignatio" },
    /* P116 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '&=' assignatio" },
    /* P117 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '|=' assignatio" },
    /* P118 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '^=' assignatio" },
    /* P119 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '<<=' assignatio" },
    /* P120 */ { ARBOR2_NT_ASSIGNATIO, 3, ARBOR2_NODUS_BINARIUM, "assignatio -> ternarius '>>=' assignatio" },
    /* P121 */ { ARBOR2_NT_ASSIGNATIO, 1, ARBOR2_NODUS_ERROR, "assignatio -> ternarius" },
    /* P122 */ { ARBOR2_NT_EXPRESSIO_OPTATIVA, 1, ARBOR2_NODUS_ERROR, "expr_opt -> virga" },

    /* Ternary conditional operator (right-associative) */
    /* P123 */ { ARBOR2_NT_TERNARIUS, 5, ARBOR2_NODUS_TERNARIUS, "ternarius -> disiunctio '?' ternarius ':' ternarius" },
    /* P124 */ { ARBOR2_NT_TERNARIUS, 1, ARBOR2_NODUS_ERROR, "ternarius -> disiunctio" },

    /* ========================================================
     * Postfix expressions (highest precedence, left-to-right)
     * ======================================================== */

    /* Postfixum base cases */
    /* P125 */ { ARBOR2_NT_POSTFIXUM, 1, ARBOR2_NODUS_IDENTIFICATOR, "postfixum -> ID" },
    /* P126 */ { ARBOR2_NT_POSTFIXUM, 1, ARBOR2_NODUS_INTEGER, "postfixum -> INT" },
    /* P127 */ { ARBOR2_NT_POSTFIXUM, 3, ARBOR2_NODUS_ERROR, "postfixum -> '(' expressio ')'" },

    /* Postfix operators (left-recursive for chaining) */
    /* P128 */ { ARBOR2_NT_POSTFIXUM, 4, ARBOR2_NODUS_SUBSCRIPTIO, "postfixum -> postfixum '[' expressio ']'" },

    /* Factor from postfixum (bridge production) */
    /* P129 */ { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_ERROR, "factor -> postfixum" },

    /* Function call (no args) */
    /* P130 */ { ARBOR2_NT_POSTFIXUM, 3, ARBOR2_NODUS_VOCATIO, "postfixum -> postfixum '(' ')'" },

    /* Function call (with args) */
    /* P131 */ { ARBOR2_NT_POSTFIXUM, 4, ARBOR2_NODUS_VOCATIO, "postfixum -> postfixum '(' argumenta ')'" },

    /* Argument list productions */
    /* P132 */ { ARBOR2_NT_ARGUMENTA, 1, ARBOR2_NODUS_ERROR, "argumenta -> assignatio" },
    /* P133 */ { ARBOR2_NT_ARGUMENTA, 3, ARBOR2_NODUS_ERROR, "argumenta -> argumenta ',' assignatio" },

    /* Member access */
    /* P134 */ { ARBOR2_NT_POSTFIXUM, 3, ARBOR2_NODUS_MEMBRUM, "postfixum -> postfixum '.' ID" },
    /* P135 */ { ARBOR2_NT_POSTFIXUM, 3, ARBOR2_NODUS_MEMBRUM, "postfixum -> postfixum '->' ID" },

    /* Post-increment/decrement */
    /* P136 */ { ARBOR2_NT_POSTFIXUM, 2, ARBOR2_NODUS_POST_UNARIUM, "postfixum -> postfixum '++'" },
    /* P137 */ { ARBOR2_NT_POSTFIXUM, 2, ARBOR2_NODUS_POST_UNARIUM, "postfixum -> postfixum '--'" },

    /* Pre-increment/decrement */
    /* P138 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '++' factor" },
    /* P139 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_UNARIUM, "factor -> '--' factor" },

    /* Additional literals (like P6 for INTEGER - reduce directly to FACTOR) */
    /* P140 */ { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_FLOAT,  "factor -> FLOAT_LIT" },
    /* P141 */ { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_CHAR,   "factor -> CHAR_LIT" },
    /* P142 */ { ARBOR2_NT_FACTOR, 1, ARBOR2_NODUS_STRING, "factor -> STRING_LIT" },

    /* sizeof operator */
    /* P143 */ { ARBOR2_NT_FACTOR, 2, ARBOR2_NODUS_SIZEOF, "factor -> 'sizeof' factor" }
};

hic_manens i32 NUM_REGULAE = (i32)(magnitudo(REGULAE) / magnitudo(REGULAE[0]));

/* ==================================================
 * PER-STATE ACTION ARRAYS (new array-of-arrays structure)
 *
 * Each state has its own array with sizeof-computed count.
 * Conflicts are marked with VERUM in the 4th field.
 * ================================================== */

/* State 0: initial - expects expression start */
hic_manens constans Arbor2TabulaActio STATUS_0_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },  /* pre-increment */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },  /* pre-decrement */
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM },  /* sizeof */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_SHIFT, 198, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT,   4, FALSUM }
};

/* State 1: after expression - expects operator or end */
hic_manens constans Arbor2TabulaActio STATUS_1_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 2: after term - expects * / % or reduce */
hic_manens constans Arbor2TabulaActio STATUS_2_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE,  2, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  2, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  2, FALSUM }
};

/* State 3: after factor - reduce to term */
hic_manens constans Arbor2TabulaActio STATUS_3_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE,  4, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  4, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  4, FALSUM }
};

/* State 4: after ID - reduce to factor OR shift for declarator (CONFLICT)
 * Also reduce to postfixum P125 for subscript/call/member handling */
hic_manens constans Arbor2TabulaActio STATUS_4_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  17, VERUM },  /* intentional conflict */
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 116, FALSUM }, /* decl: direct declarator name */
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* bitwise OR */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* bitwise XOR */
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* bitwise AND */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* = */
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* += */
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* -= */
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* *= */
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* /= */
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* %= */
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* &= */
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* |= */
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* ^= */
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* <<= */
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* >>= */
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE,  5, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT,  77, FALSUM },  /* label: */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  5, VERUM },   /* intentional conflict */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  5, FALSUM },
    /* Function call: fork between postfixum and declarator paths */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 125, VERUM },  /* intentional fork for call: ID -> postfixum */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  20, VERUM },   /* declarator path */
    /* Postfix subscript: reduce to postfixum first, then handle '[' */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 125, VERUM },  /* intentional fork for subscript */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE,   5, VERUM },  /* regular factor path */
    /* Member access: reduce to postfixum first, then handle '.' and '->' */
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 125, FALSUM }, /* member access: ID -> postfixum */
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 125, FALSUM }, /* ptr member: ID -> postfixum */
    /* Post-increment/decrement: reduce to postfixum first */
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 125, FALSUM }, /* post-inc: x++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 125, FALSUM }  /* post-dec: x-- */
};

/* State 5: after INTEGER - reduce to factor
 * Also reduce to postfixum P126 for subscript handling */
hic_manens constans Arbor2TabulaActio STATUS_5_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE,  6, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  6, FALSUM },
    /* Postfix subscript: reduce to postfixum first, then handle '[' */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 126, VERUM },  /* intentional fork for subscript */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE,   6, VERUM }   /* regular factor path */
};

/* State 6: after '(' - expects expression */
hic_manens constans Arbor2TabulaActio STATUS_6_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 7: after '*' (unary) - expects factor */
hic_manens constans Arbor2TabulaActio STATUS_7_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 8: after '&' (unary) - expects factor */
hic_manens constans Arbor2TabulaActio STATUS_8_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 9: after '+' or '-' - expects term */
hic_manens constans Arbor2TabulaActio STATUS_9_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 10: after '*', '/', '%' - expects factor */
hic_manens constans Arbor2TabulaActio STATUS_10_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 11: after '(' expression - expects ')' or operators */
hic_manens constans Arbor2TabulaActio STATUS_11_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 12: after '(' expression ')' - reduce to factor */
hic_manens constans Arbor2TabulaActio STATUS_12_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  7, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  7, FALSUM }
};

/* State 13: after expr '+' term - reduce or shift */
hic_manens constans Arbor2TabulaActio STATUS_13_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  1, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  1, FALSUM }
};

/* State 14: after term '*' factor - reduce */
hic_manens constans Arbor2TabulaActio STATUS_14_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  3, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  3, FALSUM }
};

/* State 15: after '*' factor (unary) - reduce */
hic_manens constans Arbor2TabulaActio STATUS_15_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  8, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  8, FALSUM }
};

/* State 16: after '&' factor (unary) - reduce */
hic_manens constans Arbor2TabulaActio STATUS_16_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE,  9, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE,  9, FALSUM }
};

/* State 17: after '*' in declarator - expects ID or more '*' */
hic_manens constans Arbor2TabulaActio STATUS_17_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  18, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  17, FALSUM }
};

/* State 18: after ID in declarator - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_18_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 19: after '* declarator' - reduce P11 or shift ( for fn */
hic_manens constans Arbor2TabulaActio STATUS_19_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  91, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 11, FALSUM }
};

/* State 20: after 'type declarator' - reduce P10 or continue fn/array */
hic_manens constans Arbor2TabulaActio STATUS_20_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 10, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  91, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_ERROR,   0, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_ERROR,   0, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_ERROR,   0, FALSUM }
};

/* State 21: after declaration - accept */
hic_manens constans Arbor2TabulaActio STATUS_21_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT,  0, FALSUM }
};

/* State 22: after 'expr ;' - reduce P13 stmt */
hic_manens constans Arbor2TabulaActio STATUS_22_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 13, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 13, FALSUM }
};

/* State 23: after lone ';' - reduce P14 empty stmt */
hic_manens constans Arbor2TabulaActio STATUS_23_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 14, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 14, FALSUM }
};

/* State 24: after statement - accept */
hic_manens constans Arbor2TabulaActio STATUS_24_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT,  0, FALSUM }
};

/* State 25: after '{' - epsilon reduce P18 empty list */
hic_manens constans Arbor2TabulaActio STATUS_25_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 18, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 18, FALSUM }
};

/* State 26: after '{ stmt_list' - expect stmts or '}' */
hic_manens constans Arbor2TabulaActio STATUS_26_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT,  27, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 27: after '{ stmt_list }' - reduce P16 compound */
hic_manens constans Arbor2TabulaActio STATUS_27_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 16, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 16, FALSUM }
};

/* State 28: after stmt in stmt_list - reduce P17 */
hic_manens constans Arbor2TabulaActio STATUS_28_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 17, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 17, FALSUM }
};

/* State 29: after compound_statement - accept or reduce P15 */
hic_manens constans Arbor2TabulaActio STATUS_29_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT,  0, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 15, FALSUM }
};

/* State 30: after 'if' - expect '(' */
hic_manens constans Arbor2TabulaActio STATUS_30_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  31, FALSUM }
};

/* State 31: after 'if (' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_31_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 32: after 'if ( expr' - expect ')' or continue */
hic_manens constans Arbor2TabulaActio STATUS_32_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 33: after 'if ( expr )' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_33_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 34: after 'if ( expr ) stmt' - dangling else resolution */
hic_manens constans Arbor2TabulaActio STATUS_34_ACTIONES[] = {
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_SHIFT,  35, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 20, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 20, FALSUM }
};

/* State 35: after 'if ( expr ) stmt else' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_35_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 36: after 'if ( expr ) stmt else stmt' - reduce P21 */
hic_manens constans Arbor2TabulaActio STATUS_36_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 21, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 21, FALSUM }
};

/* State 37: after if_statement - reduce P19 */
hic_manens constans Arbor2TabulaActio STATUS_37_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 19, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 19, FALSUM }
};

/* State 38: nested compound_statement - always reduce P15 */
hic_manens constans Arbor2TabulaActio STATUS_38_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 15, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 15, FALSUM }
};

/* State 39: after 'while' - expect '(' */
hic_manens constans Arbor2TabulaActio STATUS_39_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  40, FALSUM }
};

/* State 40: after 'while (' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_40_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 41: after 'while ( expr' - expect ')' or continue */
hic_manens constans Arbor2TabulaActio STATUS_41_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 42: after 'while ( expr )' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_42_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 43: after 'while ( expr ) stmt' - reduce P23 */
hic_manens constans Arbor2TabulaActio STATUS_43_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 23, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 23, FALSUM }
};

/* State 44: after while_statement - reduce P22 */
hic_manens constans Arbor2TabulaActio STATUS_44_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 22, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 22, FALSUM }
};

/* State 45: after 'do' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_45_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 46: after 'do stmt' - expect 'while' */
hic_manens constans Arbor2TabulaActio STATUS_46_ACTIONES[] = {
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  47, FALSUM }
};

/* State 47: after 'do stmt while' - expect '(' */
hic_manens constans Arbor2TabulaActio STATUS_47_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  48, FALSUM }
};

/* State 48: after 'do stmt while (' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_48_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 49: after 'do stmt while ( expr' - expect ')' or continue */
hic_manens constans Arbor2TabulaActio STATUS_49_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 50: after 'do stmt while ( expr )' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_50_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  51, FALSUM }
};

/* State 51: after 'do stmt while ( expr ) ;' - reduce P25 */
hic_manens constans Arbor2TabulaActio STATUS_51_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 25, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 25, FALSUM }
};

/* State 52: after do_statement - reduce P24 */
hic_manens constans Arbor2TabulaActio STATUS_52_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 24, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 24, FALSUM }
};

/* State 53: after 'for' - expect '(' */
hic_manens constans Arbor2TabulaActio STATUS_53_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  54, FALSUM }
};

/* State 54: after 'for (' - expect expression or ';' for empty init */
hic_manens constans Arbor2TabulaActio STATUS_54_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29, FALSUM }
};

/* State 55: after 'for ( expression' - reduce to expr_opt or continue */
hic_manens constans Arbor2TabulaActio STATUS_55_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 56: after 'for ( expr_opt' - expect first ';' */
hic_manens constans Arbor2TabulaActio STATUS_56_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  57, FALSUM }
};

/* State 57: after 'for ( expr_opt ;' - expect condition or ';' */
hic_manens constans Arbor2TabulaActio STATUS_57_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29, FALSUM }
};

/* State 58: after 'for ( expr_opt ; expression' - continue or reduce */
hic_manens constans Arbor2TabulaActio STATUS_58_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 59: after 'for ( expr_opt ; expr_opt' - expect second ';' */
hic_manens constans Arbor2TabulaActio STATUS_59_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  60, FALSUM }
};

/* State 60: after 'for ( ... ; expr_opt ;' - expect increment or ')' */
hic_manens constans Arbor2TabulaActio STATUS_60_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 29, FALSUM }
};

/* State 61: after 'for ( ... ; expression' (increment) - reduce or continue */
hic_manens constans Arbor2TabulaActio STATUS_61_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 62: after 'for ( expr_opt ; expr_opt ; expr_opt' - expect ')' */
hic_manens constans Arbor2TabulaActio STATUS_62_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT,  63, FALSUM }
};

/* State 63: after 'for ( ... )' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_63_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 64: after 'for ( ... ) stmt' - reduce P27 */
hic_manens constans Arbor2TabulaActio STATUS_64_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 27, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 27, FALSUM }
};

/* State 65: after for_statement - reduce P26 */
hic_manens constans Arbor2TabulaActio STATUS_65_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 26, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 26, FALSUM }
};

/* State 66: after 'break' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_66_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  67, FALSUM }
};

/* State 67: after 'break ;' - reduce P30 */
hic_manens constans Arbor2TabulaActio STATUS_67_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 30, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 30, FALSUM }
};

/* State 68: after 'continue' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_68_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  69, FALSUM }
};

/* State 69: after 'continue ;' - reduce P31 */
hic_manens constans Arbor2TabulaActio STATUS_69_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 31, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 31, FALSUM }
};

/* State 70: after 'return' - expect expression or ';' */
hic_manens constans Arbor2TabulaActio STATUS_70_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 29, FALSUM }
};

/* State 71: after 'return expression' - can continue or reduce */
hic_manens constans Arbor2TabulaActio STATUS_71_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 28, FALSUM }
};

/* State 72: after 'return expr_opt' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_72_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  73, FALSUM }
};

/* State 73: after 'return expr_opt ;' - reduce P32 */
hic_manens constans Arbor2TabulaActio STATUS_73_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 32, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 32, FALSUM }
};

/* State 74: after 'goto' - expect IDENTIFIER */
hic_manens constans Arbor2TabulaActio STATUS_74_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  75, FALSUM }
};

/* State 75: after 'goto IDENTIFIER' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_75_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  76, FALSUM }
};

/* State 76: after 'goto IDENTIFIER ;' - reduce P33 */
hic_manens constans Arbor2TabulaActio STATUS_76_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 33, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 33, FALSUM }
};

/* State 77: after 'IDENTIFIER :' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_77_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 78: after 'IDENTIFIER : statement' - reduce P34 */
hic_manens constans Arbor2TabulaActio STATUS_78_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 34, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 34, FALSUM }
};

/* State 79: after 'switch' - expect '(' */
hic_manens constans Arbor2TabulaActio STATUS_79_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  80, FALSUM }
};

/* State 80: after 'switch (' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_80_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 81: after 'switch ( expr' - expect ')' or continue */
hic_manens constans Arbor2TabulaActio STATUS_81_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    /* Assignment operators - reduce to TRANSLATIO, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 82: after 'switch ( expr )' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_82_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 83: after 'switch ( expr ) stmt' - reduce P35 */
hic_manens constans Arbor2TabulaActio STATUS_83_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 35, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 35, FALSUM }
};

/* State 84: after 'case' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_84_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 85: after 'case expr' - expect ':' */
hic_manens constans Arbor2TabulaActio STATUS_85_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT,  86, FALSUM }
};

/* State 86: after 'case expr :' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_86_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM }
};

/* State 87: after 'case expr : stmt' - reduce P36 */
hic_manens constans Arbor2TabulaActio STATUS_87_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 36, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 36, FALSUM }
};

/* State 88: after 'default' - expect ':' */
hic_manens constans Arbor2TabulaActio STATUS_88_ACTIONES[] = {
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT,  89, FALSUM }
};

/* State 89: after 'default :' - expect statement */
hic_manens constans Arbor2TabulaActio STATUS_89_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT,  23, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT,  25, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_SHIFT,  30, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_SHIFT,  39, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_SHIFT,  45, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_SHIFT,  53, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_SHIFT,  66, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_SHIFT,  68, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_SHIFT,  70, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_SHIFT,  74, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_SHIFT,  79, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_SHIFT,  84, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_SHIFT,  88, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 90: after 'default : stmt' - reduce P37 */
hic_manens constans Arbor2TabulaActio STATUS_90_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_IF,             ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_ELSE,           ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_WHILE,          ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_DO,             ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_FOR,            ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_BREAK,          ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_CONTINUE,       ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_RETURN,         ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_GOTO,           ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_SWITCH,         ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_CASE,           ARBOR2_ACTIO_REDUCE, 37, FALSUM },
    { ARBOR2_LEXEMA_DEFAULT,        ARBOR2_ACTIO_REDUCE, 37, FALSUM }
};

/* State 91: after 'declarator (' - expect ')' or 'void' or param type */
hic_manens constans Arbor2TabulaActio STATUS_91_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT,  92, FALSUM },
    { ARBOR2_LEXEMA_VOID,           ARBOR2_ACTIO_SHIFT,  93, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  95, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT,  95, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT,  95, FALSUM }
};

/* State 92: after 'declarator ( )' - reduce P38 */
hic_manens constans Arbor2TabulaActio STATUS_92_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 38, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 38, FALSUM }
};

/* State 93: after 'declarator ( void' - expect ')' */
hic_manens constans Arbor2TabulaActio STATUS_93_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT,  94, FALSUM }
};

/* State 94: after 'declarator ( void )' - reduce P39 */
hic_manens constans Arbor2TabulaActio STATUS_94_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 39, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 39, FALSUM }
};

/* State 95: after '( type_specifier' - expect '*' or param name */
hic_manens constans Arbor2TabulaActio STATUS_95_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  96, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  97, FALSUM }
};

/* State 96: after '( type_spec *' - expect '*' or param name */
hic_manens constans Arbor2TabulaActio STATUS_96_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  96, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  98, FALSUM }
};

/* State 97: after '( type_spec name' - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_97_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 98: after '( type_spec *...* name' - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_98_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 99: after '( type_spec declarator' - reduce P43 */
hic_manens constans Arbor2TabulaActio STATUS_99_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 43, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 43, FALSUM }
};

/* State 100: after '( type_spec * declarator' - reduce P11 */
hic_manens constans Arbor2TabulaActio STATUS_100_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 11, FALSUM }
};

/* State 101: after '( param_decl' - reduce P41 */
hic_manens constans Arbor2TabulaActio STATUS_101_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 41, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 41, FALSUM }
};

/* State 102: after '( param_list' - expect ')' or ',' */
hic_manens constans Arbor2TabulaActio STATUS_102_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 103, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 104, FALSUM }
};

/* State 103: after '( param_list )' - reduce P40 */
hic_manens constans Arbor2TabulaActio STATUS_103_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 40, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 40, FALSUM }
};

/* State 104: after 'param_list ,' - expect next type_specifier */
hic_manens constans Arbor2TabulaActio STATUS_104_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 105, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 105, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 105, FALSUM }
};

/* State 105: after 'param_list , type_spec' - expect '*' or param name */
hic_manens constans Arbor2TabulaActio STATUS_105_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 106, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 107, FALSUM }
};

/* State 106: after 'param_list , type_spec *' - expect '*' or name */
hic_manens constans Arbor2TabulaActio STATUS_106_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 106, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 108, FALSUM }
};

/* State 107: after 'param_list , type_spec name' - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_107_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 108: after 'param_list , type_spec *...* name' - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_108_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 109: after 'param_list , type_spec declarator' - reduce P43 */
hic_manens constans Arbor2TabulaActio STATUS_109_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 43, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 43, FALSUM }
};

/* State 110: after 'param_list , type_spec * declarator' - reduce P11 */
hic_manens constans Arbor2TabulaActio STATUS_110_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 11, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 11, FALSUM }
};

/* State 111: after 'param_list , param_decl' - reduce P42 */
hic_manens constans Arbor2TabulaActio STATUS_111_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 42, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 42, FALSUM }
};

/* State 112: (reserved/unused) */
hic_manens constans Arbor2TabulaActio STATUS_112_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF, ARBOR2_ACTIO_ERROR, 0, FALSUM }
};

/* State 113: after 'type_spec declarator compound' - reduce P44 */
hic_manens constans Arbor2TabulaActio STATUS_113_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 44, FALSUM }
};

/* State 114: after function_definition - accept */
hic_manens constans Arbor2TabulaActio STATUS_114_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT,  0, FALSUM }
};

/* State 115: (reserved/unused) */
hic_manens constans Arbor2TabulaActio STATUS_115_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF, ARBOR2_ACTIO_ERROR, 0, FALSUM }
};

/* State 116: after 'type_spec name' (direct declarator) - reduce P12 */
hic_manens constans Arbor2TabulaActio STATUS_116_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 12, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 12, FALSUM }
};

/* State 117: after 'struct' - expect ID or '{' */
hic_manens constans Arbor2TabulaActio STATUS_117_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 118, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 119, FALSUM }
};

/* State 118: after 'struct ID' - expect '{' or reduce P47 */
hic_manens constans Arbor2TabulaActio STATUS_118_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 120, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 47, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 47, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 47, FALSUM }
};

/* State 119: after 'struct {' (anonymous) - start member list */
hic_manens constans Arbor2TabulaActio STATUS_119_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 120: after 'struct ID {' (named) - start member list */
hic_manens constans Arbor2TabulaActio STATUS_120_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 121: after member type_specifier - expect '*' or name or ':' */
hic_manens constans Arbor2TabulaActio STATUS_121_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  18, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 168, FALSUM }
};

/* State 122: after member type_spec '*' - expect more '*' or name */
hic_manens constans Arbor2TabulaActio STATUS_122_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 122, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 124, FALSUM }
};

/* State 123: after member type_spec name - expect ';' or ':' */
hic_manens constans Arbor2TabulaActio STATUS_123_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 125, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 162, FALSUM }
};

/* State 124: after member type_spec * name - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_124_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 126, FALSUM }
};

/* State 125: after member decl ';' (no pointer) - reduce P48 */
hic_manens constans Arbor2TabulaActio STATUS_125_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 126: after first pointer member decl ';' - reduce P50 */
hic_manens constans Arbor2TabulaActio STATUS_126_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 50, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 50, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 50, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 50, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 50, FALSUM }
};

/* State 127: after struct_member_list in anonymous struct - expect more or '}' */
hic_manens constans Arbor2TabulaActio STATUS_127_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 128, FALSUM }
};

/* State 128: after 'struct { members }' - reduce P46 (anonymous) */
hic_manens constans Arbor2TabulaActio STATUS_128_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 46, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 46, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 46, FALSUM }
};

/* State 129: after struct_member_list in named struct - expect more or '}' */
hic_manens constans Arbor2TabulaActio STATUS_129_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 130, FALSUM }
};

/* State 130: after 'struct ID { members }' - reduce P45 (named) */
hic_manens constans Arbor2TabulaActio STATUS_130_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 45, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 45, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 45, FALSUM }
};

/* State 131: after type_specifier in subsequent member - expect '*' or name or ':' */
hic_manens constans Arbor2TabulaActio STATUS_131_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  18, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 171, FALSUM }
};

/* State 132: after type_spec '*' in subsequent member - expect more '*' or name */
hic_manens constans Arbor2TabulaActio STATUS_132_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 132, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 134, FALSUM }
};

/* State 133: after subsequent non-pointer member type + name - expect ';' or ':' */
hic_manens constans Arbor2TabulaActio STATUS_133_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 135, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 165, FALSUM }
};

/* State 134: after subsequent pointer member type + * + name - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_134_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 136, FALSUM }
};

/* State 135: after subsequent non-pointer member ';' - reduce P49 */
hic_manens constans Arbor2TabulaActio STATUS_135_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 49, FALSUM }
};

/* State 136: after subsequent pointer member ';' - reduce P51 */
hic_manens constans Arbor2TabulaActio STATUS_136_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 51, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 51, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 51, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 51, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 51, FALSUM }
};

/* State 137: after 'union' - expect ID or '{' */
hic_manens constans Arbor2TabulaActio STATUS_137_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 138, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 139, FALSUM }
};

/* State 138: after 'union ID' - expect '{' or reduce P54 */
hic_manens constans Arbor2TabulaActio STATUS_138_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 140, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 54, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 54, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 54, FALSUM }
};

/* State 139: after 'union {' (anonymous) - start member list */
hic_manens constans Arbor2TabulaActio STATUS_139_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 140: after 'union ID {' (named) - start member list */
hic_manens constans Arbor2TabulaActio STATUS_140_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 121, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 141: after struct_member_list in anonymous union - expect more or '}' */
hic_manens constans Arbor2TabulaActio STATUS_141_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 142, FALSUM }
};

/* State 142: after 'union { members }' - reduce P53 (anonymous) */
hic_manens constans Arbor2TabulaActio STATUS_142_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 53, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 53, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 53, FALSUM }
};

/* State 143: after struct_member_list in named union - expect more or '}' */
hic_manens constans Arbor2TabulaActio STATUS_143_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 131, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 144, FALSUM }
};

/* State 144: after 'union ID { members }' - reduce P52 (named) */
hic_manens constans Arbor2TabulaActio STATUS_144_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 52, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 52, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 52, FALSUM }
};

/* State 145: after 'enum' - expect ID or '{' */
hic_manens constans Arbor2TabulaActio STATUS_145_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 146, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 147, FALSUM }
};

/* State 146: after 'enum ID' - expect '{' or reduce P57 */
hic_manens constans Arbor2TabulaActio STATUS_146_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_SHIFT, 148, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 57, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 57, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 57, FALSUM }
};

/* State 147: after 'enum {' (anonymous) - expect first enumerator */
hic_manens constans Arbor2TabulaActio STATUS_147_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 149, FALSUM }
};

/* State 148: after 'enum ID {' (named) - expect first enumerator */
hic_manens constans Arbor2TabulaActio STATUS_148_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 149, FALSUM }
};

/* State 149: after enumerator ID - expect '=' or ',' or '}' */
hic_manens constans Arbor2TabulaActio STATUS_149_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_SHIFT, 150, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 60, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 60, FALSUM }
};

/* State 150: after 'ID =' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_150_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 151: after 'ID = expr' - continue expr or reduce P61 */
hic_manens constans Arbor2TabulaActio STATUS_151_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 61, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 61, FALSUM }
};

/* State 152: after first enumerator - reduce P58 to create list */
hic_manens constans Arbor2TabulaActio STATUS_152_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 58, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 58, FALSUM }
};

/* State 153: after enumerator_list in anonymous enum - expect ',' or '}' */
hic_manens constans Arbor2TabulaActio STATUS_153_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 156, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 154, FALSUM }
};

/* State 154: after 'enum { list }' - reduce P56 (anonymous) */
hic_manens constans Arbor2TabulaActio STATUS_154_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 56, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 56, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 56, FALSUM }
};

/* State 155: after enumerator_list in named enum - expect ',' or '}' */
hic_manens constans Arbor2TabulaActio STATUS_155_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 156, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_SHIFT, 157, FALSUM }
};

/* State 156: after ',' - expect next enumerator */
hic_manens constans Arbor2TabulaActio STATUS_156_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 158, FALSUM }
};

/* State 157: after 'enum ID { list }' - reduce P55 (named) */
hic_manens constans Arbor2TabulaActio STATUS_157_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 55, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 55, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 55, FALSUM }
};

/* State 158: after ',' ID (subsequent enumerator) - expect '=' or ',' or '}' */
hic_manens constans Arbor2TabulaActio STATUS_158_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_SHIFT, 159, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 60, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 60, FALSUM }
};

/* State 159: after subsequent 'ID =' - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_159_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 160: after subsequent 'ID = expr' - continue expr or reduce P61 */
hic_manens constans Arbor2TabulaActio STATUS_160_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 61, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 61, FALSUM }
};

/* State 161: after subsequent enumerator - reduce P59 (append) */
hic_manens constans Arbor2TabulaActio STATUS_161_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 59, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 59, FALSUM }
};

/* ========== BIT FIELD STATES (162-173) ========== */

/* State 162: after type_spec ID ':' (first named bit field) - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_162_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 163: after type_spec ID ':' expr (first) - continue expr or ';' */
hic_manens constans Arbor2TabulaActio STATUS_163_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 164, FALSUM }
};

/* State 164: after type_spec ID ':' expr ';' (first) - reduce P62 */
hic_manens constans Arbor2TabulaActio STATUS_164_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 62, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 62, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 62, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 62, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 62, FALSUM }
};

/* State 165: after member_list type_spec ID ':' (subsequent) - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_165_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 166: after member_list type_spec ID ':' expr (subsequent) - continue or ';' */
hic_manens constans Arbor2TabulaActio STATUS_166_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 167, FALSUM }
};

/* State 167: after member_list type_spec ID ':' expr ';' - reduce P63 */
hic_manens constans Arbor2TabulaActio STATUS_167_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 63, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 63, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 63, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 63, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 63, FALSUM }
};

/* State 168: after type_spec ':' (first anonymous bit field) - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_168_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 169: after type_spec ':' expr (first anon) - continue expr or ';' */
hic_manens constans Arbor2TabulaActio STATUS_169_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 170, FALSUM }
};

/* State 170: after type_spec ':' expr ';' (first anon) - reduce P64 */
hic_manens constans Arbor2TabulaActio STATUS_170_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 64, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 64, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 64, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 64, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 64, FALSUM }
};

/* State 171: after member_list type_spec ':' (subsequent anon) - expect expression */
hic_manens constans Arbor2TabulaActio STATUS_171_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 172: after member_list type_spec ':' expr (subsequent anon) - continue or ';' */
hic_manens constans Arbor2TabulaActio STATUS_172_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 173, FALSUM }
};

/* State 173: after member_list type_spec ':' expr ';' (subsequent anon) - reduce P65 */
hic_manens constans Arbor2TabulaActio STATUS_173_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 65, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 65, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 65, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 65, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 65, FALSUM }
};

/* ========== NESTED STRUCT/UNION AS MEMBER TYPE (174-185) ========== */

/* State 174: after struct_specifier (first member) - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_174_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 175: after struct_specifier '*' (first member) - expect ID */
hic_manens constans Arbor2TabulaActio STATUS_175_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 175, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 177, FALSUM }
};

/* State 176: after struct_specifier ID (first member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_176_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 178, FALSUM }
};

/* State 177: after struct_specifier '*' ID (first member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_177_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 179, FALSUM }
};

/* State 178: after struct_specifier ID ';' (first member) - reduce P66 */
hic_manens constans Arbor2TabulaActio STATUS_178_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 66, FALSUM }
};

/* State 179: after struct_specifier '*' ID ';' (first member) - reduce P67 */
hic_manens constans Arbor2TabulaActio STATUS_179_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 67, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 67, FALSUM }
};

/* State 180: after struct_specifier (subsequent member) - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_180_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 181: after struct_specifier '*' (subsequent member) - expect ID */
hic_manens constans Arbor2TabulaActio STATUS_181_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 181, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 183, FALSUM }
};

/* State 182: after struct_specifier ID (subsequent member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_182_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 184, FALSUM }
};

/* State 183: after struct_specifier '*' ID (subsequent member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_183_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 185, FALSUM }
};

/* State 184: after struct_specifier ID ';' (subsequent member) - reduce P68 */
hic_manens constans Arbor2TabulaActio STATUS_184_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 68, FALSUM }
};

/* State 185: after struct_specifier '*' ID ';' (subsequent member) - reduce P69 */
hic_manens constans Arbor2TabulaActio STATUS_185_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 69, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 69, FALSUM }
};

/* ========== NESTED ENUM AS MEMBER TYPE (186-197) ========== */

/* State 186: after enum_specifier (first member) - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_186_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 187: after enum_specifier '*' (first member) - expect ID */
hic_manens constans Arbor2TabulaActio STATUS_187_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 187, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 189, FALSUM }
};

/* State 188: after enum_specifier ID (first member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_188_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 190, FALSUM }
};

/* State 189: after enum_specifier '*' ID (first member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_189_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 191, FALSUM }
};

/* State 190: after enum_specifier ID ';' (first member) - reduce P70 */
hic_manens constans Arbor2TabulaActio STATUS_190_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 70, FALSUM }
};

/* State 191: after enum_specifier '*' ID ';' (first member) - reduce P71 */
hic_manens constans Arbor2TabulaActio STATUS_191_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 71, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 71, FALSUM }
};

/* State 192: after enum_specifier (subsequent member) - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_192_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 193: after enum_specifier '*' (subsequent member) - expect ID */
hic_manens constans Arbor2TabulaActio STATUS_193_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 193, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 195, FALSUM }
};

/* State 194: after enum_specifier ID (subsequent member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_194_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 196, FALSUM }
};

/* State 195: after enum_specifier '*' ID (subsequent member) - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_195_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 197, FALSUM }
};

/* State 196: after enum_specifier ID ';' (subsequent member) - reduce P72 */
hic_manens constans Arbor2TabulaActio STATUS_196_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 72, FALSUM }
};

/* State 197: after enum_specifier '*' ID ';' (subsequent member) - reduce P73 */
hic_manens constans Arbor2TabulaActio STATUS_197_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 73, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 73, FALSUM }
};

/* ========== TYPEDEF DECLARATION STATES (198-216) ========== */

/* State 198: after 'typedef' - expect type_specifier */
hic_manens constans Arbor2TabulaActio STATUS_198_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 199, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_SHIFT, 199, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_SHIFT, 199, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_SHIFT, 117, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_SHIFT, 137, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_SHIFT, 145, FALSUM }
};

/* State 199: after 'typedef type_spec' - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_199_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 200: after 'typedef type_spec *' - expect more '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_200_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 200, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 202, FALSUM }
};

/* State 201: after 'typedef type_spec ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_201_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 203, FALSUM }
};

/* State 202: after 'typedef type_spec *... ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_202_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 204, FALSUM }
};

/* State 203: reduce P74 (typedef type_spec ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_203_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 74, FALSUM }
};

/* State 204: reduce P75 (typedef type_spec *... ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_204_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 75, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 75, FALSUM }
};

/* State 205: after 'typedef struct_spec' - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_205_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 206: after 'typedef struct_spec *' - expect more '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_206_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 206, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 208, FALSUM }
};

/* State 207: after 'typedef struct_spec ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_207_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 209, FALSUM }
};

/* State 208: after 'typedef struct_spec *... ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_208_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 210, FALSUM }
};

/* State 209: reduce P76 (typedef struct_spec ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_209_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 76, FALSUM }
};

/* State 210: reduce P77 (typedef struct_spec *... ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_210_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 77, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 77, FALSUM }
};

/* State 211: after 'typedef enum_spec' - expect '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_211_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 17, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 18, FALSUM }
};

/* State 212: after 'typedef enum_spec *' - expect more '*' or ID */
hic_manens constans Arbor2TabulaActio STATUS_212_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT, 212, FALSUM },
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 214, FALSUM }
};

/* State 213: after 'typedef enum_spec ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_213_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 215, FALSUM }
};

/* State 214: after 'typedef enum_spec *... ID' - expect ';' */
hic_manens constans Arbor2TabulaActio STATUS_214_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 216, FALSUM }
};

/* State 215: reduce P78 (typedef enum_spec ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_215_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 78, FALSUM }
};

/* State 216: reduce P79 (typedef enum_spec *... ID ;) */
hic_manens constans Arbor2TabulaActio STATUS_216_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 79, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 79, FALSUM }
};

/* ========== ARRAY DECLARATOR STATES (217-220) ========== */

/* State 217: after 'declarator [' - expect expression or ']' */
hic_manens constans Arbor2TabulaActio STATUS_217_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_SHIFT, 218, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM }
};

/* State 218: after 'declarator [ ]' - reduce P81 (unsized array) */
hic_manens constans Arbor2TabulaActio STATUS_218_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 81, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 81, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 81, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 81, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 81, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 81, FALSUM }
};

/* State 219: after 'declarator [ expression' - expect ']' or continue expr */
hic_manens constans Arbor2TabulaActio STATUS_219_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_SHIFT, 220, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM }
};

/* State 220: after 'declarator [ expression ]' - reduce P80 (sized array) */
hic_manens constans Arbor2TabulaActio STATUS_220_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 80, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 80, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 80, FALSUM },
    { ARBOR2_LEXEMA_BRACE_APERTA,   ARBOR2_ACTIO_REDUCE, 80, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 80, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 80, FALSUM }
};

/* ========== STRUCT MEMBER DECLARATOR COMPLETION (221-224) ========== */

/* State 221: after 'type_spec declarator' in struct (first) - ';' or '[' or ':' */
hic_manens constans Arbor2TabulaActio STATUS_221_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 222, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 162, FALSUM }
};

/* State 222: after 'type_spec declarator ;' (first member) - reduce P48 */
hic_manens constans Arbor2TabulaActio STATUS_222_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 48, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 48, FALSUM }
};

/* State 223: after 'list type_spec declarator' (subsequent) - ';' or '[' or ':' */
hic_manens constans Arbor2TabulaActio STATUS_223_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 224, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 165, FALSUM }
};

/* State 224: after 'list type_spec declarator ;' (subsequent) - reduce P49 */
hic_manens constans Arbor2TabulaActio STATUS_224_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 49, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 49, FALSUM }
};

/* ========== TYPEDEF DECLARATOR COMPLETION (225-230) ========== */

/* State 225: after 'typedef type_spec declarator' - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_225_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 226, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 226: after 'typedef type_spec declarator ;' - reduce P74 */
hic_manens constans Arbor2TabulaActio STATUS_226_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 74, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 74, FALSUM }
};

/* State 227: after 'typedef struct_spec declarator' - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_227_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 228, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 228: after 'typedef struct_spec declarator ;' - reduce P76 */
hic_manens constans Arbor2TabulaActio STATUS_228_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 76, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 76, FALSUM }
};

/* State 229: after 'typedef enum_spec declarator' - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_229_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 230, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 230: after 'typedef enum_spec declarator ;' - reduce P78 */
hic_manens constans Arbor2TabulaActio STATUS_230_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_TYPEDEF,        ARBOR2_ACTIO_REDUCE, 78, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 78, FALSUM }
};

/* ========== NESTED TYPE MEMBER DECLARATOR COMPLETION (231-238) ========== */

/* State 231: after 'struct_spec declarator' (first member) - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_231_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 232, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 232: after 'struct_spec declarator ;' (first member) - reduce P66 */
hic_manens constans Arbor2TabulaActio STATUS_232_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 66, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 66, FALSUM }
};

/* State 233: after 'list struct_spec declarator' (subsequent) - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_233_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 234, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 234: after 'list struct_spec declarator ;' (subsequent) - reduce P68 */
hic_manens constans Arbor2TabulaActio STATUS_234_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 68, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 68, FALSUM }
};

/* State 235: after 'enum_spec declarator' (first member) - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_235_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 236, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 236: after 'enum_spec declarator ;' (first member) - reduce P70 */
hic_manens constans Arbor2TabulaActio STATUS_236_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 70, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 70, FALSUM }
};

/* State 237: after 'list enum_spec declarator' (subsequent) - expect ';' or '[' */
hic_manens constans Arbor2TabulaActio STATUS_237_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 238, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 217, FALSUM }
};

/* State 238: after 'list enum_spec declarator ;' (subsequent) - reduce P72 */
hic_manens constans Arbor2TabulaActio STATUS_238_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_INT,            ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_CHAR,           ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_STRUCT,         ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_UNION,          ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_ENUM,           ARBOR2_ACTIO_REDUCE, 72, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 72, FALSUM }
};

/* ========== COMPARISON/EQUALITY STATES (239-263) ========== */

/* State 239: after comparatio - shift to continue or reduce to aequalitas */
hic_manens constans Arbor2TabulaActio STATUS_239_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 87, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 87, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 87, FALSUM }
};

/* State 240: after aequalitas at top-level - reduce P105 to ampersand_bitwise or continue */
hic_manens constans Arbor2TabulaActio STATUS_240_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* bitwise AND */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* bitwise XOR */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* bitwise OR */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 241: after 'comparatio <|>|<=|>=' - expect expression starters */
hic_manens constans Arbor2TabulaActio STATUS_241_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 242: after 'aequalitas ==|!=' - expect comparatio (expression starters) */
hic_manens constans Arbor2TabulaActio STATUS_242_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 243: after 'comparatio <|>|<=|>= expression' - continue or reduce P99 */
hic_manens constans Arbor2TabulaActio STATUS_243_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99, FALSUM }
};

/* State 244: after 'aequalitas ==|!= comparatio' - reduce P85 */
hic_manens constans Arbor2TabulaActio STATUS_244_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 85, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 85, FALSUM }
};

/* State 245: after aequalitas inside parens - shift ) or continue */
hic_manens constans Arbor2TabulaActio STATUS_245_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 12, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM }
};

/* State 246: after comparatio in if condition - reduce to aequalitas or continue */
hic_manens constans Arbor2TabulaActio STATUS_246_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    /* Assignment operators - reduce to AEQUALITAS, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM }
};

/* State 247: after aequalitas in if condition - reduce P105 to continue chain */
hic_manens constans Arbor2TabulaActio STATUS_247_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    /* Assignment operators - reduce to AMPERSAND_BITWISE, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 248: after aequalitas in while condition - reduce to continue chain */
hic_manens constans Arbor2TabulaActio STATUS_248_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    /* Assignment operators - reduce to AMPERSAND_BITWISE, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 249: after comparatio in for condition - reduce to aequalitas or continue */
hic_manens constans Arbor2TabulaActio STATUS_249_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT, 241, FALSUM },
    /* Assignment operators - reduce to AEQUALITAS, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 87, FALSUM }
};

/* State 250: after aequalitas in for condition - reduce to AMPERSAND_BITWISE or continue */
hic_manens constans Arbor2TabulaActio STATUS_250_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    /* Assignment operators - reduce to AMPERSAND_BITWISE, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 251: after aequalitas in do-while condition - reduce to continue chain */
hic_manens constans Arbor2TabulaActio STATUS_251_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    /* Assignment operators - reduce to AMPERSAND_BITWISE, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 252: after aequalitas in switch condition - reduce to continue chain */
hic_manens constans Arbor2TabulaActio STATUS_252_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    /* Assignment operators - reduce to AMPERSAND_BITWISE, chain continues */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* ==== E10: Logical Operators (&&, ||) ==== */

/* State 253: after coniunctio at top-level - reduce to disiunctio or continue && */
hic_manens constans Arbor2TabulaActio STATUS_253_ACTIONES[] = {
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_SHIFT, 254, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 94, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 94, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 94, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 94, FALSUM }
};

/* State 254: after 'coniunctio &&' - expect aequalitas (expression starters) */
hic_manens constans Arbor2TabulaActio STATUS_254_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 255: after disiunctio at top-level - ternary or reduce to ternarius */
hic_manens constans Arbor2TabulaActio STATUS_255_ACTIONES[] = {
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_SHIFT, 256, FALSUM },
    /* Ternary operator - SHIFT to 306 */
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_SHIFT, 306, FALSUM },
    /* No ternary - reduce to TERNARIUS (P124: ternarius -> disiunctio) */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 124, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 124, FALSUM }
};

/* State 256: after 'disiunctio ||' - expect coniunctio (expression starters) */
hic_manens constans Arbor2TabulaActio STATUS_256_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 257: after aequalitas in && context (top-level) - reduce P96 or continue */
hic_manens constans Arbor2TabulaActio STATUS_257_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 96, FALSUM }
};

/* State 258: after 'coniunctio && aequalitas' - reduce P95 */
hic_manens constans Arbor2TabulaActio STATUS_258_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 95, FALSUM }
};

/* State 259: after aequalitas in || context - reduce P96 to coniunctio */
hic_manens constans Arbor2TabulaActio STATUS_259_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 96, FALSUM }
};

/* State 260: after coniunctio in || context - reduce P93 or continue && */
hic_manens constans Arbor2TabulaActio STATUS_260_ACTIONES[] = {
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_SHIFT, 261, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 93, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 93, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 93, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 93, FALSUM }
};

/* State 261: after 'coniunctio &&' in || context - expect aequalitas */
hic_manens constans Arbor2TabulaActio STATUS_261_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 262: after aequalitas in && within || context - reduce P96 */
hic_manens constans Arbor2TabulaActio STATUS_262_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96, FALSUM }
};

/* State 263: after 'coniunctio && aequalitas' in || context - reduce P95 */
hic_manens constans Arbor2TabulaActio STATUS_263_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 95, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 95, FALSUM }
};

/* ========== SHIFT OPERATOR STATES (264-267) ========== */

/* State 264: after translatio (general context) - reduce P92 or continue shift */
hic_manens constans Arbor2TabulaActio STATUS_264_ACTIONES[] = {
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_SHIFT, 265, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_SHIFT, 265, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 92, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 92, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 92, FALSUM }
};

/* State 265: after translatio << or >> - expect expression starters */
hic_manens constans Arbor2TabulaActio STATUS_265_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 266: after translatio << expression - reduce P97 or continue expr */
hic_manens constans Arbor2TabulaActio STATUS_266_ACTIONES[] = {
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 97, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 97, FALSUM }
};

/* State 267: after translatio in 'comparatio <' context - reduce P88 */
hic_manens constans Arbor2TabulaActio STATUS_267_ACTIONES[] = {
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_SHIFT, 265, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_SHIFT, 265, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 88, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 88, FALSUM }
};

/* ==================================================
 * States 268-273: Bitwise operator states (top-level context)
 * ================================================== */

/* State 268: after AMPERSAND_BITWISE at top-level - shift & or reduce P103 */
hic_manens constans Arbor2TabulaActio STATUS_268_ACTIONES[] = {
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT, 269, FALSUM },  /* bitwise AND */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 103, FALSUM }, /* reduce to caret_bitwise */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 103, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 103, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 103, FALSUM }
};

/* State 269: after '&' bitwise - expect expression starters */
hic_manens constans Arbor2TabulaActio STATUS_269_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 270: after CARET_BITWISE at top-level - shift ^ or reduce P101 */
hic_manens constans Arbor2TabulaActio STATUS_270_ACTIONES[] = {
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_SHIFT, 271, FALSUM },  /* bitwise XOR */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 101, FALSUM }, /* reduce to pipa_bitwise */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 101, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 101, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 101, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 101, FALSUM }
};

/* State 271: after '^' bitwise - expect expression starters */
hic_manens constans Arbor2TabulaActio STATUS_271_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 272: after PIPA_BITWISE at top-level - shift | or reduce P96 */
hic_manens constans Arbor2TabulaActio STATUS_272_ACTIONES[] = {
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_SHIFT, 273, FALSUM },  /* bitwise OR */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },  /* reduce to coniunctio */
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 96, FALSUM },  /* ? ternary */
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 96, FALSUM },  /* : ternary */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 96, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 96, FALSUM }
};

/* State 273: after '|' bitwise - expect expression starters */
hic_manens constans Arbor2TabulaActio STATUS_273_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 274: after aequalitas in '&' context - reduce P104 or continue == */
hic_manens constans Arbor2TabulaActio STATUS_274_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 104, FALSUM },  /* complete P104 */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 104, FALSUM }
};

/* State 275: after aequalitas in '^' context - reduce P105 to ampersand_bitwise */
hic_manens constans Arbor2TabulaActio STATUS_275_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },  /* reduce to ampersand_bitwise */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 276: after ampersand_bitwise in '^' context - shift & or reduce P102 */
hic_manens constans Arbor2TabulaActio STATUS_276_ACTIONES[] = {
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT, 280, FALSUM },  /* nested & */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 102, FALSUM }, /* complete P102 */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 102, FALSUM }
};

/* State 277: after aequalitas in '|' context - reduce P105 to ampersand_bitwise */
hic_manens constans Arbor2TabulaActio STATUS_277_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 278: after ampersand_bitwise in '|' context - shift & or reduce P103 */
hic_manens constans Arbor2TabulaActio STATUS_278_ACTIONES[] = {
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT, 281, FALSUM },  /* nested & */
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 103, FALSUM }, /* reduce to caret_bitwise */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 103, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 103, FALSUM }
};

/* State 279: after caret_bitwise in '|' context - shift ^ or reduce P100 */
hic_manens constans Arbor2TabulaActio STATUS_279_ACTIONES[] = {
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_SHIFT, 282, FALSUM },  /* nested ^ */
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 100, FALSUM }, /* complete P100 */
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 100, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 100, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 100, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 100, FALSUM }
};

/* States 280-282: Expression starters for nested bitwise within bitwise */

/* State 280: after '&' within '^' context */
hic_manens constans Arbor2TabulaActio STATUS_280_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 281: after '&' within '|' context */
hic_manens constans Arbor2TabulaActio STATUS_281_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 282: after '^' within '|' context */
hic_manens constans Arbor2TabulaActio STATUS_282_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 283: after aequalitas within '& in ^' context - reduce P104 */
hic_manens constans Arbor2TabulaActio STATUS_283_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 104, FALSUM }
};

/* State 284: after aequalitas within '& in |' context - reduce P104 */
hic_manens constans Arbor2TabulaActio STATUS_284_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 104, FALSUM }
};

/* State 285: after aequalitas within '^ in |' context - reduce P105 */
hic_manens constans Arbor2TabulaActio STATUS_285_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 105, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 105, FALSUM }
};

/* State 286: after ampersand_bitwise within '^ in |' context - shift & or reduce P102 */
hic_manens constans Arbor2TabulaActio STATUS_286_ACTIONES[] = {
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT, 287, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 102, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 102, FALSUM }
};

/* State 287: after '&' within '^ in |' context - expression starters */
hic_manens constans Arbor2TabulaActio STATUS_287_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,  4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,  5, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,  6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,  8, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,  9, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM }
};

/* State 288: after aequalitas within '& in ^ in |' context - reduce P104 */
hic_manens constans Arbor2TabulaActio STATUS_288_ACTIONES[] = {
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT, 242, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 104, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 104, FALSUM }
};

/* States 289-292: Unary operators ~ and ! */

/* State 289: after '~' - expects factor starters */
hic_manens constans Arbor2TabulaActio STATUS_289_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 290: after '~' factor - reduce P106 */
hic_manens constans Arbor2TabulaActio STATUS_290_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 106, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 106, FALSUM }
};

/* State 291: after '!' - expects factor starters */
hic_manens constans Arbor2TabulaActio STATUS_291_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 292: after '!' factor - reduce P107 */
hic_manens constans Arbor2TabulaActio STATUS_292_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 107, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 107, FALSUM }
};

/* ==================================================
 * States 293-298: Assignment and Comma Operators
 *
 * New precedence levels:
 *   VIRGA (,) - lowest, left-associative
 *   ASSIGNATIO (= += etc) - right-associative
 *   DISIUNCTIO (||) - was lowest, now 3rd
 * ================================================== */

/* State 293: after assignment op '=' '+=' etc - expects expression for RHS */
hic_manens constans Arbor2TabulaActio STATUS_293_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 294: after 'disiunctio = assignatio' - reduce P110 (or P111-P120 based on op) */
/* For simplicity, we reduce P110 here - semantic action handles actual op */
hic_manens constans Arbor2TabulaActio STATUS_294_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 110, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 110, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 110, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 110, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 110, FALSUM }
};

/* State 295: after ASSIGNATIO at top-level - comma or reduce to VIRGA */
hic_manens constans Arbor2TabulaActio STATUS_295_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 109, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 109, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 109, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 109, FALSUM }
};

/* State 296: after VIRGA at top-level - ACCEPT or continue comma */
hic_manens constans Arbor2TabulaActio STATUS_296_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_ACCEPT, 0, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_SHIFT, 22, FALSUM }
};

/* State 297: after 'virga ,' or 'assignatio ,' - expects assignatio (expr starters) */
hic_manens constans Arbor2TabulaActio STATUS_297_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 298: after 'virga , assignatio' - reduce P108 (left-associative) */
hic_manens constans Arbor2TabulaActio STATUS_298_ACTIONES[] = {
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 108, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 108, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 108, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 108, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 108, FALSUM }
};

/* State 299: after VIRGA in if condition - shift ) to then-branch */
hic_manens constans Arbor2TabulaActio STATUS_299_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 33, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 300: after VIRGA in while condition - shift ) to loop body */
hic_manens constans Arbor2TabulaActio STATUS_300_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 42, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 301: after VIRGA in for-init - reduce P122 to expr_opt */
hic_manens constans Arbor2TabulaActio STATUS_301_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 122, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 302: after VIRGA in for-condition - reduce P122 to expr_opt */
hic_manens constans Arbor2TabulaActio STATUS_302_ACTIONES[] = {
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 122, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 303: after VIRGA in for-increment - reduce P122 to expr_opt */
hic_manens constans Arbor2TabulaActio STATUS_303_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 122, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 304: after VIRGA in do-while condition - shift ) to semicolon state */
hic_manens constans Arbor2TabulaActio STATUS_304_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 50, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* State 305: after VIRGA in switch condition - shift ) to switch body */
hic_manens constans Arbor2TabulaActio STATUS_305_ACTIONES[] = {
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 82, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 297, FALSUM }
};

/* ==================================================
 * Ternary Operator States (306-310)
 * ================================================== */

/* State 306: after 'disiunctio ?' - expect expression (true branch) */
hic_manens constans Arbor2TabulaActio STATUS_306_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 307: after 'disiunctio ? ternarius' - expect ':' */
hic_manens constans Arbor2TabulaActio STATUS_307_ACTIONES[] = {
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_SHIFT, 308, FALSUM }
};

/* State 308: after 'disiunctio ? ternarius :' - expect expression (false branch) */
hic_manens constans Arbor2TabulaActio STATUS_308_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 309: after 'disiunctio ? ternarius : ternarius' - reduce P123 */
hic_manens constans Arbor2TabulaActio STATUS_309_ACTIONES[] = {
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 123, FALSUM }
};

/* State 310: after TERNARIUS at top-level - assignment ops or reduce to assignatio */
hic_manens constans Arbor2TabulaActio STATUS_310_ACTIONES[] = {
    /* Ternary chains - shift ? for right-associativity */
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_SHIFT, 306, FALSUM },
    /* Assignment operators - SHIFT to 293 (right-associative) */
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_SHIFT, 293, FALSUM },
    /* No assignment - reduce to ASSIGNATIO (P121: assignatio -> ternarius) */
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 121, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 121, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 121, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 121, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 121, FALSUM }
};

/* ========================================================
 * POSTFIX EXPRESSION STATES (311+)
 * ======================================================== */

/* State 311: after POSTFIXUM - check for postfix operators or reduce to factor
 * This state handles: subscript '[', call '(', member '.', ptr-member '->'
 * and post-increment '++', post-decrement '--' */
hic_manens constans Arbor2TabulaActio STATUS_311_ACTIONES[] = {
    /* Subscript: shift '[' to enter subscript expression */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_SHIFT, 312, FALSUM },
    /* Function call: shift '(' to enter call expression */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT, 315, FALSUM },
    /* Member access: shift '.' and '->' */
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_SHIFT, 322, FALSUM },
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_SHIFT, 324, FALSUM },
    /* Post-increment/decrement: shift '++' and '--' */
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 326, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 327, FALSUM },
    /* No postfix operator - reduce to factor P129 */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 129, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 129, FALSUM }
};

/* State 312: after 'postfixum [' - expect expression for subscript */
hic_manens constans Arbor2TabulaActio STATUS_312_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 313: after 'postfixum [ expression' - continue expr or expect ']' */
hic_manens constans Arbor2TabulaActio STATUS_313_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_SHIFT, 314, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_SHIFT,   9, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_SHIFT,  10, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_SHIFT,  11, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_SHIFT,  11, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_SHIFT,  11, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_SHIFT,  11, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_SHIFT, 256, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_SHIFT, 256, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_SHIFT,  12, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_SHIFT,  12, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 99, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT, 274, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_SHIFT, 276, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_SHIFT, 278, FALSUM }
};

/* State 314: after 'postfixum [ expression ]' - reduce P128 (subscript)
 * This creates a SUBSCRIPTIO node. After reduction, GOTO[parent, POSTFIXUM] -> 311
 * which allows chaining (arr[i][j]) or eventual reduction to factor */
hic_manens constans Arbor2TabulaActio STATUS_314_ACTIONES[] = {
    /* Always reduce P128 (subscript) - the resulting postfixum will be handled by state 311 */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i][j] */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i]() */
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i].x */
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i]->x */
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i]++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 128, FALSUM },  /* chain: arr[i]-- */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 128, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 128, FALSUM }
};

/* State 315: after 'postfixum (' - expects ')' for no-arg call or expression for args
 * On ')': shift to 318 and reduce P130 (call no args)
 * On expr start: shift to parse first argument, eventually reduce P132 -> ARGUMENTA */
hic_manens constans Arbor2TabulaActio STATUS_315_ACTIONES[] = {
    /* Empty call: foo() */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 318, FALSUM },
    /* Call with args: foo(expr) - shift to parse expression */
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 316: after 'postfixum ( assignatio' - have one argument
 * On ')': shift to 319 and reduce P131 (call with args)
 * On ',': shift to 317 for more args */
hic_manens constans Arbor2TabulaActio STATUS_316_ACTIONES[] = {
    /* End of args: foo(a) */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_SHIFT, 319, FALSUM },
    /* More args: foo(a, ... */
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_SHIFT, 317, FALSUM }
};

/* State 317: after 'postfixum ( argumenta ,' - expects next argument */
hic_manens constans Arbor2TabulaActio STATUS_317_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 318: after 'postfixum ( )' - reduce P130 (empty call) */
hic_manens constans Arbor2TabulaActio STATUS_318_ACTIONES[] = {
    /* Reduce P130: postfixum -> postfixum '(' ')' */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo()[i] */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo()() */
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo().x */
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo()->x */
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo()++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 130, FALSUM },  /* chain: foo()-- */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 130, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 130, FALSUM }
};

/* State 319: after 'postfixum ( argumenta )' - reduce P131 (call with args) */
hic_manens constans Arbor2TabulaActio STATUS_319_ACTIONES[] = {
    /* Reduce P131: postfixum -> postfixum '(' argumenta ')' */
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a)[i] */
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a)() */
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a).x */
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a)->x */
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a)++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 131, FALSUM },  /* chain: foo(a)-- */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 131, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 131, FALSUM }
};

/* State 320: after 'postfixum ( argumenta , assignatio' - have another arg
 * On ')': reduce P133 first (argumenta -> argumenta ',' assignatio), then eventually P131
 * On ',': reduce P133 first, then shift comma for more args */
hic_manens constans Arbor2TabulaActio STATUS_320_ACTIONES[] = {
    /* Reduce P133: argumenta -> argumenta ',' assignatio */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 133, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 133, FALSUM }
};

/* State 321: after 'postfixum ( assignatio' - first argument parsed
 * Reduce P132: argumenta -> assignatio
 * Then GOTO[315, ARGUMENTA] -> 316 */
hic_manens constans Arbor2TabulaActio STATUS_321_ACTIONES[] = {
    /* Reduce P132: argumenta -> assignatio */
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 132, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 132, FALSUM }
};

/* State 322: after 'postfixum .' - expects member identifier */
hic_manens constans Arbor2TabulaActio STATUS_322_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 323, FALSUM }
};

/* State 323: after 'postfixum . ID' - reduce P134: postfixum -> postfixum '.' ID
 * Then GOTO returns to 311 to check for more postfix operators */
hic_manens constans Arbor2TabulaActio STATUS_323_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 134, FALSUM },  /* chain: s.x++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 134, FALSUM },  /* chain: s.x-- */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 134, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 134, FALSUM }
};

/* State 324: after 'postfixum ->' - expects member identifier */
hic_manens constans Arbor2TabulaActio STATUS_324_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT, 325, FALSUM }
};

/* State 325: after 'postfixum -> ID' - reduce P135: postfixum -> postfixum '->' ID
 * Then GOTO returns to 311 to check for more postfix operators */
hic_manens constans Arbor2TabulaActio STATUS_325_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 135, FALSUM },  /* chain: p->x++ */
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 135, FALSUM },  /* chain: p->x-- */
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 135, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 135, FALSUM }
};

/* State 326: after 'postfixum ++' - reduce P136 (post-increment)
 * Immediately reduce - no lookahead needed */
hic_manens constans Arbor2TabulaActio STATUS_326_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 136, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 136, FALSUM }
};

/* State 327: after 'postfixum --' - reduce P137 (post-decrement)
 * Immediately reduce - no lookahead needed */
hic_manens constans Arbor2TabulaActio STATUS_327_ACTIONES[] = {
    { ARBOR2_LEXEMA_BRACKET_APERTA, ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PUNCTUM,        ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SAGITTA,        ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 137, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 137, FALSUM }
};

/* State 328: after '++' prefix - expects factor expression */
hic_manens constans Arbor2TabulaActio STATUS_328_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 329: after '--' prefix - expects factor expression */
hic_manens constans Arbor2TabulaActio STATUS_329_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 330: after '++ factor' - reduce P138 (pre-increment) */
hic_manens constans Arbor2TabulaActio STATUS_330_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 138, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 138, FALSUM }
};

/* State 331: after '-- factor' - reduce P139 (pre-decrement) */
hic_manens constans Arbor2TabulaActio STATUS_331_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 139, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 139, FALSUM }
};

/* State 332: after FLOAT_LIT - reduce P140 (postfixum -> FLOAT_LIT) */
hic_manens constans Arbor2TabulaActio STATUS_332_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 140, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 140, FALSUM }
};

/* State 333: after CHAR_LIT - reduce P141 (postfixum -> CHAR_LIT) */
hic_manens constans Arbor2TabulaActio STATUS_333_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 141, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 141, FALSUM }
};

/* State 334: after STRING_LIT - reduce P142 (postfixum -> STRING_LIT) */
hic_manens constans Arbor2TabulaActio STATUS_334_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 142, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 142, FALSUM }
};

/* State 335: after 'sizeof' - expects factor */
hic_manens constans Arbor2TabulaActio STATUS_335_ACTIONES[] = {
    { ARBOR2_LEXEMA_IDENTIFICATOR,  ARBOR2_ACTIO_SHIFT,   4, FALSUM },
    { ARBOR2_LEXEMA_INTEGER,        ARBOR2_ACTIO_SHIFT,   5, FALSUM },
    { ARBOR2_LEXEMA_FLOAT_LIT,      ARBOR2_ACTIO_SHIFT, 332, FALSUM },
    { ARBOR2_LEXEMA_CHAR_LIT,       ARBOR2_ACTIO_SHIFT, 333, FALSUM },
    { ARBOR2_LEXEMA_STRING_LIT,     ARBOR2_ACTIO_SHIFT, 334, FALSUM },
    { ARBOR2_LEXEMA_PAREN_APERTA,   ARBOR2_ACTIO_SHIFT,   6, FALSUM },
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_SHIFT,   7, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_SHIFT,   8, FALSUM },
    { ARBOR2_LEXEMA_TILDE,          ARBOR2_ACTIO_SHIFT, 289, FALSUM },
    { ARBOR2_LEXEMA_EXCLAMATIO,     ARBOR2_ACTIO_SHIFT, 291, FALSUM },
    { ARBOR2_LEXEMA_DUPLUS,         ARBOR2_ACTIO_SHIFT, 328, FALSUM },
    { ARBOR2_LEXEMA_DUMINUS,        ARBOR2_ACTIO_SHIFT, 329, FALSUM },
    { ARBOR2_LEXEMA_SIZEOF,         ARBOR2_ACTIO_SHIFT, 335, FALSUM }
};

/* State 336: after 'sizeof' factor - reduce P143 */
hic_manens constans Arbor2TabulaActio STATUS_336_ACTIONES[] = {
    { ARBOR2_LEXEMA_ASTERISCUS,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_SOLIDUS,        ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_PERCENTUM,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_PLUS,           ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MINUS,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MINOR,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MAIOR,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MINOR_AEQ,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MAIOR_AEQ,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_SINISTRUM,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_DEXTRUM,        ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_AEQUALIS,       ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_NON_AEQUALIS,   ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_DUAMPERSAND,    ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_DUPIPA,         ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_PIPA,           ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_CARET,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_AMPERSAND,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_ASSIGNATIO,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_PLUS_ASSIGN,    ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MINUS_ASSIGN,   ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MULT_ASSIGN,    ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_DIV_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_MOD_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_AND_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_OR_ASSIGN,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_XOR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_SHL_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_SHR_ASSIGN,     ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_QUAESTIO,       ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_SEMICOLON,      ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_BRACE_CLAUSA,   ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_EOF,            ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_PAREN_CLAUSA,   ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_COLON,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_COMMA,          ARBOR2_ACTIO_REDUCE, 143, FALSUM },
    { ARBOR2_LEXEMA_BRACKET_CLAUSA, ARBOR2_ACTIO_REDUCE, 143, FALSUM }
};

/* ==================================================
 * STATUS_TABULA - Master state table (UNDER CONSTRUCTION)
 *
 * Will be populated as states are converted.
 * For now, lookup still uses old ACTIONES array.
 * ================================================== */

/* STATUS_INFO macro computes count from sizeof */
#define STATUS_INFO(n, desc) { \
    STATUS_##n##_ACTIONES, \
    (s32)(magnitudo(STATUS_##n##_ACTIONES) / magnitudo(STATUS_##n##_ACTIONES[0])), \
    desc \
}

/* Partial STATUS_TABULA - first 53 states converted */
hic_manens constans Arbor2StatusInfo STATUS_TABULA_PARTIAL[] = {
    STATUS_INFO(0,  "initial - expects expression start"),
    STATUS_INFO(1,  "after expression - expects operator or end"),
    STATUS_INFO(2,  "after term - expects * / % or reduce"),
    STATUS_INFO(3,  "after factor - reduce to term"),
    STATUS_INFO(4,  "after ID - reduce to factor or declarator"),
    STATUS_INFO(5,  "after INTEGER - reduce to factor"),
    STATUS_INFO(6,  "after '(' - expects expression"),
    STATUS_INFO(7,  "after '*' unary - expects factor"),
    STATUS_INFO(8,  "after '&' unary - expects factor"),
    STATUS_INFO(9,  "after '+' or '-' - expects term"),
    STATUS_INFO(10, "after '*' '/' '%' - expects factor"),
    STATUS_INFO(11, "after '(' expression - expects ')' or operators"),
    STATUS_INFO(12, "after '(' expression ')' - reduce to factor"),
    STATUS_INFO(13, "after expr '+' term - shift *// or reduce P1"),
    STATUS_INFO(14, "after term '*' factor - reduce P3"),
    STATUS_INFO(15, "after '*' factor unary - reduce P8"),
    STATUS_INFO(16, "after '&' factor unary - reduce P9"),
    STATUS_INFO(17, "after '*' in declarator - expects ID or '*'"),
    STATUS_INFO(18, "after ID in declarator - reduce P12"),
    STATUS_INFO(19, "after '* declarator' - reduce P11 or fn"),
    STATUS_INFO(20, "after 'type declarator' - reduce P10 or fn/array"),
    STATUS_INFO(21, "after declaration - accept"),
    STATUS_INFO(22, "after 'expr ;' - reduce P13 stmt"),
    STATUS_INFO(23, "after ';' - reduce P14 empty stmt"),
    STATUS_INFO(24, "after statement - accept"),
    STATUS_INFO(25, "after '{' - epsilon reduce P18"),
    STATUS_INFO(26, "after '{ stmt_list' - expect stmts or '}'"),
    STATUS_INFO(27, "after '{ stmt_list }' - reduce P16"),
    STATUS_INFO(28, "after stmt in list - reduce P17"),
    STATUS_INFO(29, "after compound - accept or reduce P15"),
    STATUS_INFO(30, "after 'if' - expect '('"),
    STATUS_INFO(31, "after 'if (' - expect expression"),
    STATUS_INFO(32, "after 'if ( expr' - expect ')' or continue"),
    STATUS_INFO(33, "after 'if ( expr )' - expect statement"),
    STATUS_INFO(34, "after 'if ( expr ) stmt' - dangling else"),
    STATUS_INFO(35, "after 'if (...) stmt else' - expect statement"),
    STATUS_INFO(36, "after 'if (...) stmt else stmt' - reduce P21"),
    STATUS_INFO(37, "after if_statement - reduce P19"),
    STATUS_INFO(38, "nested compound_statement - reduce P15"),
    STATUS_INFO(39, "after 'while' - expect '('"),
    STATUS_INFO(40, "after 'while (' - expect expression"),
    STATUS_INFO(41, "after 'while ( expr' - expect ')' or continue"),
    STATUS_INFO(42, "after 'while ( expr )' - expect statement"),
    STATUS_INFO(43, "after 'while ( expr ) stmt' - reduce P23"),
    STATUS_INFO(44, "after while_statement - reduce P22"),
    STATUS_INFO(45, "after 'do' - expect statement"),
    STATUS_INFO(46, "after 'do stmt' - expect 'while'"),
    STATUS_INFO(47, "after 'do stmt while' - expect '('"),
    STATUS_INFO(48, "after 'do stmt while (' - expect expression"),
    STATUS_INFO(49, "after 'do stmt while ( expr' - expect ')' or cont"),
    STATUS_INFO(50, "after 'do stmt while ( expr )' - expect ';'"),
    STATUS_INFO(51, "after 'do stmt while ( expr ) ;' - reduce P25"),
    STATUS_INFO(52, "after do_statement - reduce P24"),
    STATUS_INFO(53, "after 'for' - expect '('"),
    STATUS_INFO(54, "after 'for (' - expect expression or ';'"),
    STATUS_INFO(55, "after 'for ( expression' - reduce to expr_opt"),
    STATUS_INFO(56, "after 'for ( expr_opt' - expect first ';'"),
    STATUS_INFO(57, "after 'for ( expr_opt ;' - expect condition or ';'"),
    STATUS_INFO(58, "after 'for ( ... ; expression' - continue or reduce"),
    STATUS_INFO(59, "after 'for ( ... ; expr_opt' - expect second ';'"),
    STATUS_INFO(60, "after 'for ( ... ; expr_opt ;' - expect incr or ')'"),
    STATUS_INFO(61, "after 'for ( ... ; expression' incr - reduce"),
    STATUS_INFO(62, "after 'for ( ... ; ... ; expr_opt' - expect ')'"),
    STATUS_INFO(63, "after 'for ( ... )' - expect statement"),
    STATUS_INFO(64, "after 'for ( ... ) stmt' - reduce P27"),
    STATUS_INFO(65, "after for_statement - reduce P26"),
    STATUS_INFO(66, "after 'break' - expect ';'"),
    STATUS_INFO(67, "after 'break ;' - reduce P30"),
    STATUS_INFO(68, "after 'continue' - expect ';'"),
    STATUS_INFO(69, "after 'continue ;' - reduce P31"),
    STATUS_INFO(70, "after 'return' - expect expression or ';'"),
    STATUS_INFO(71, "after 'return expression' - continue or reduce"),
    STATUS_INFO(72, "after 'return expr_opt' - expect ';'"),
    STATUS_INFO(73, "after 'return expr_opt ;' - reduce P32"),
    STATUS_INFO(74, "after 'goto' - expect IDENTIFIER"),
    STATUS_INFO(75, "after 'goto IDENTIFIER' - expect ';'"),
    STATUS_INFO(76, "after 'goto IDENTIFIER ;' - reduce P33"),
    STATUS_INFO(77, "after 'IDENTIFIER :' - expect statement"),
    STATUS_INFO(78, "after 'IDENTIFIER : statement' - reduce P34"),
    STATUS_INFO(79, "after 'switch' - expect '('"),
    STATUS_INFO(80, "after 'switch (' - expect expression"),
    STATUS_INFO(81, "after 'switch ( expr' - expect ')' or continue"),
    STATUS_INFO(82, "after 'switch ( expr )' - expect statement"),
    STATUS_INFO(83, "after 'switch ( expr ) stmt' - reduce P35"),
    STATUS_INFO(84, "after 'case' - expect expression"),
    STATUS_INFO(85, "after 'case expr' - expect ':'"),
    STATUS_INFO(86, "after 'case expr :' - expect statement"),
    STATUS_INFO(87, "after 'case expr : stmt' - reduce P36"),
    STATUS_INFO(88, "after 'default' - expect ':'"),
    STATUS_INFO(89, "after 'default :' - expect statement"),
    STATUS_INFO(90, "after 'default : stmt' - reduce P37"),
    STATUS_INFO(91, "after 'declarator (' - expect ')' or 'void' or param"),
    STATUS_INFO(92, "after 'declarator ( )' - reduce P38"),
    STATUS_INFO(93, "after 'declarator ( void' - expect ')'"),
    STATUS_INFO(94, "after 'declarator ( void )' - reduce P39"),
    STATUS_INFO(95, "after '( type_spec' - expect '*' or param name"),
    STATUS_INFO(96, "after '( type_spec *' - expect '*' or param name"),
    STATUS_INFO(97, "after '( type_spec name' - reduce P12"),
    STATUS_INFO(98, "after '( type_spec *...* name' - reduce P12"),
    STATUS_INFO(99, "after '( type_spec declarator' - reduce P43"),
    STATUS_INFO(100, "after '( type_spec * declarator' - reduce P11"),
    STATUS_INFO(101, "after '( param_decl' - reduce P41"),
    STATUS_INFO(102, "after '( param_list' - expect ')' or ','"),
    STATUS_INFO(103, "after '( param_list )' - reduce P40"),
    STATUS_INFO(104, "after 'param_list ,' - expect next type_spec"),
    STATUS_INFO(105, "after 'param_list , type_spec' - expect '*' or name"),
    STATUS_INFO(106, "after 'param_list , type_spec *' - expect '*' or name"),
    STATUS_INFO(107, "after 'param_list , type_spec name' - reduce P12"),
    STATUS_INFO(108, "after 'param_list , type_spec *...* name' - reduce P12"),
    STATUS_INFO(109, "after 'param_list , type_spec declarator' - reduce P43"),
    STATUS_INFO(110, "after 'param_list , type_spec * decl' - reduce P11"),
    STATUS_INFO(111, "after 'param_list , param_decl' - reduce P42"),
    STATUS_INFO(112, "(reserved/unused)"),
    STATUS_INFO(113, "after 'type_spec declarator compound' - reduce P44"),
    STATUS_INFO(114, "after function_definition - accept"),
    STATUS_INFO(115, "(reserved/unused)"),
    STATUS_INFO(116, "after 'type_spec name' direct decl - reduce P12"),
    STATUS_INFO(117, "after 'struct' - expect ID or '{'"),
    STATUS_INFO(118, "after 'struct ID' - expect '{' or reduce P47"),
    STATUS_INFO(119, "after 'struct {' (anon) - start member list"),
    STATUS_INFO(120, "after 'struct ID {' (named) - start member list"),
    STATUS_INFO(121, "after member type_spec - expect '*' or name or ':'"),
    STATUS_INFO(122, "after member type_spec '*' - expect more '*' or name"),
    STATUS_INFO(123, "after member type_spec name - expect ';' or ':'"),
    STATUS_INFO(124, "after member type_spec * name - expect ';'"),
    STATUS_INFO(125, "after member decl ';' (no ptr) - reduce P48"),
    STATUS_INFO(126, "after first ptr member decl ';' - reduce P50"),
    STATUS_INFO(127, "after member_list in anon struct - expect more or '}'"),
    STATUS_INFO(128, "after 'struct { members }' - reduce P46 (anon)"),
    STATUS_INFO(129, "after member_list in named struct - expect more or '}'"),
    STATUS_INFO(130, "after 'struct ID { members }' - reduce P45 (named)"),
    STATUS_INFO(131, "after type_spec in subseq member - expect '*' or name"),
    STATUS_INFO(132, "after type_spec '*' in subseq member - expect '*'/name"),
    STATUS_INFO(133, "after subseq non-ptr member - expect ';' or ':'"),
    STATUS_INFO(134, "after subseq ptr member + * + name - expect ';'"),
    STATUS_INFO(135, "after subseq non-ptr member ';' - reduce P49"),
    STATUS_INFO(136, "after subseq ptr member ';' - reduce P51"),
    STATUS_INFO(137, "after 'union' - expect ID or '{'"),
    STATUS_INFO(138, "after 'union ID' - expect '{' or reduce P54"),
    STATUS_INFO(139, "after 'union {' (anon) - start member list"),
    STATUS_INFO(140, "after 'union ID {' (named) - start member list"),
    STATUS_INFO(141, "after member_list in anon union - expect more or '}'"),
    STATUS_INFO(142, "after 'union { members }' - reduce P53 (anon)"),
    STATUS_INFO(143, "after member_list in named union - expect more or '}'"),
    STATUS_INFO(144, "after 'union ID { members }' - reduce P52 (named)"),
    STATUS_INFO(145, "after 'enum' - expect ID or '{'"),
    STATUS_INFO(146, "after 'enum ID' - expect '{' or reduce P57"),
    STATUS_INFO(147, "after 'enum {' (anon) - expect first enumerator"),
    STATUS_INFO(148, "after 'enum ID {' (named) - expect first enumerator"),
    STATUS_INFO(149, "after enumerator ID - expect '=' or ',' or '}'"),
    STATUS_INFO(150, "after 'ID =' - expect expression"),
    STATUS_INFO(151, "after 'ID = expr' - continue expr or reduce P61"),
    STATUS_INFO(152, "after first enumerator - reduce P58 to list"),
    STATUS_INFO(153, "after enumerator_list in anon enum - expect ',' or '}'"),
    STATUS_INFO(154, "after 'enum { list }' - reduce P56 (anon)"),
    STATUS_INFO(155, "after enumerator_list in named enum - expect ',' or '}'"),
    STATUS_INFO(156, "after ',' - expect next enumerator"),
    STATUS_INFO(157, "after 'enum ID { list }' - reduce P55 (named)"),
    STATUS_INFO(158, "after ',' ID (subsequent) - expect '=' or ',' or '}'"),
    STATUS_INFO(159, "after subsequent 'ID =' - expect expression"),
    STATUS_INFO(160, "after subsequent 'ID = expr' - continue or reduce P61"),
    STATUS_INFO(161, "after subsequent enumerator - reduce P59 (append)"),
    STATUS_INFO(162, "after type_spec ID ':' (first bit field) - expect expr"),
    STATUS_INFO(163, "after type_spec ID ':' expr (first) - continue or ';'"),
    STATUS_INFO(164, "after type_spec ID ':' expr ';' (first) - reduce P62"),
    STATUS_INFO(165, "after member_list type_spec ID ':' (subsequent) - expr"),
    STATUS_INFO(166, "after member_list type_spec ID ':' expr - continue/';'"),
    STATUS_INFO(167, "after member_list type_spec ID ':' expr ';' - P63"),
    STATUS_INFO(168, "after type_spec ':' (first anon bit field) - expect expr"),
    STATUS_INFO(169, "after type_spec ':' expr (first anon) - continue or ';'"),
    STATUS_INFO(170, "after type_spec ':' expr ';' (first anon) - reduce P64"),
    STATUS_INFO(171, "after member_list type_spec ':' (subsequent anon) - expr"),
    STATUS_INFO(172, "after member_list type_spec ':' expr (anon) - continue"),
    STATUS_INFO(173, "after member_list type_spec ':' expr ';' (anon) - P65"),
    STATUS_INFO(174, "after struct_spec (first member) - expect '*' or ID"),
    STATUS_INFO(175, "after struct_spec '*' (first member) - expect ID"),
    STATUS_INFO(176, "after struct_spec ID (first member) - expect ';'"),
    STATUS_INFO(177, "after struct_spec '*' ID (first member) - expect ';'"),
    STATUS_INFO(178, "after struct_spec ID ';' (first) - reduce P66"),
    STATUS_INFO(179, "after struct_spec '*' ID ';' (first) - reduce P67"),
    STATUS_INFO(180, "after struct_spec (subsequent member) - expect '*' or ID"),
    STATUS_INFO(181, "after struct_spec '*' (subsequent) - expect ID"),
    STATUS_INFO(182, "after struct_spec ID (subsequent) - expect ';'"),
    STATUS_INFO(183, "after struct_spec '*' ID (subsequent) - expect ';'"),
    STATUS_INFO(184, "after struct_spec ID ';' (subsequent) - reduce P68"),
    STATUS_INFO(185, "after struct_spec '*' ID ';' (subsequent) - reduce P69"),
    STATUS_INFO(186, "after enum_spec (first member) - expect '*' or ID"),
    STATUS_INFO(187, "after enum_spec '*' (first member) - expect ID"),
    STATUS_INFO(188, "after enum_spec ID (first member) - expect ';'"),
    STATUS_INFO(189, "after enum_spec '*' ID (first member) - expect ';'"),
    STATUS_INFO(190, "after enum_spec ID ';' (first) - reduce P70"),
    STATUS_INFO(191, "after enum_spec '*' ID ';' (first) - reduce P71"),
    STATUS_INFO(192, "after enum_spec (subsequent member) - expect '*' or ID"),
    STATUS_INFO(193, "after enum_spec '*' (subsequent) - expect ID"),
    STATUS_INFO(194, "after enum_spec ID (subsequent) - expect ';'"),
    STATUS_INFO(195, "after enum_spec '*' ID (subsequent) - expect ';'"),
    STATUS_INFO(196, "after enum_spec ID ';' (subsequent) - reduce P72"),
    STATUS_INFO(197, "after enum_spec '*' ID ';' (subsequent) - reduce P73"),
    STATUS_INFO(198, "after 'typedef' - expect type_specifier"),
    STATUS_INFO(199, "after 'typedef type_spec' - expect '*' or ID"),
    STATUS_INFO(200, "after 'typedef type_spec *' - expect more '*' or ID"),
    STATUS_INFO(201, "after 'typedef type_spec ID' - expect ';'"),
    STATUS_INFO(202, "after 'typedef type_spec *... ID' - expect ';'"),
    STATUS_INFO(203, "reduce P74 (typedef type_spec ID ;)"),
    STATUS_INFO(204, "reduce P75 (typedef type_spec *... ID ;)"),
    STATUS_INFO(205, "after 'typedef struct_spec' - expect '*' or ID"),
    STATUS_INFO(206, "after 'typedef struct_spec *' - expect more '*' or ID"),
    STATUS_INFO(207, "after 'typedef struct_spec ID' - expect ';'"),
    STATUS_INFO(208, "after 'typedef struct_spec *... ID' - expect ';'"),
    STATUS_INFO(209, "reduce P76 (typedef struct_spec ID ;)"),
    STATUS_INFO(210, "reduce P77 (typedef struct_spec *... ID ;)"),
    STATUS_INFO(211, "after 'typedef enum_spec' - expect '*' or ID"),
    STATUS_INFO(212, "after 'typedef enum_spec *' - expect more '*' or ID"),
    STATUS_INFO(213, "after 'typedef enum_spec ID' - expect ';'"),
    STATUS_INFO(214, "after 'typedef enum_spec *... ID' - expect ';'"),
    STATUS_INFO(215, "reduce P78 (typedef enum_spec ID ;)"),
    STATUS_INFO(216, "reduce P79 (typedef enum_spec *... ID ;)"),
    STATUS_INFO(217, "after 'declarator [' - expect expression or ']'"),
    STATUS_INFO(218, "after 'declarator [ ]' - reduce P81 (unsized array)"),
    STATUS_INFO(219, "after 'declarator [ expression' - expect ']' or cont"),
    STATUS_INFO(220, "after 'declarator [ expression ]' - reduce P80"),
    STATUS_INFO(221, "after 'type_spec declarator' (first) - ';' or '[' or ':'"),
    STATUS_INFO(222, "after 'type_spec declarator ;' (first) - reduce P48"),
    STATUS_INFO(223, "after 'list type_spec declarator' (subseq) - ';'/['/:"),
    STATUS_INFO(224, "after 'list type_spec declarator ;' (subseq) - P49"),
    STATUS_INFO(225, "after 'typedef type_spec declarator' - expect ';' or '['"),
    STATUS_INFO(226, "after 'typedef type_spec declarator ;' - reduce P74"),
    STATUS_INFO(227, "after 'typedef struct_spec declarator' - ';' or '['"),
    STATUS_INFO(228, "after 'typedef struct_spec declarator ;' - reduce P76"),
    STATUS_INFO(229, "after 'typedef enum_spec declarator' - ';' or '['"),
    STATUS_INFO(230, "after 'typedef enum_spec declarator ;' - reduce P78"),
    STATUS_INFO(231, "after 'struct_spec declarator' (first) - ';' or '['"),
    STATUS_INFO(232, "after 'struct_spec declarator ;' (first) - reduce P66"),
    STATUS_INFO(233, "after 'list struct_spec declarator' (subseq) - ';'/'['"),
    STATUS_INFO(234, "after 'list struct_spec declarator ;' (subseq) - P68"),
    STATUS_INFO(235, "after 'enum_spec declarator' (first) - ';' or '['"),
    STATUS_INFO(236, "after 'enum_spec declarator ;' (first) - reduce P70"),
    STATUS_INFO(237, "after 'list enum_spec declarator' (subseq) - ';' or '['"),
    STATUS_INFO(238, "after 'list enum_spec declarator ;' (subseq) - P72"),
    STATUS_INFO(239, "after comparatio - shift to continue or reduce P87"),
    STATUS_INFO(240, "after aequalitas at top-level - reduce P96 or continue"),
    STATUS_INFO(241, "after 'comparatio <|>|<=|>=' - expect expression"),
    STATUS_INFO(242, "after 'aequalitas ==|!=' - expect comparatio"),
    STATUS_INFO(243, "after 'comparatio op expression' - continue or P99"),
    STATUS_INFO(244, "after 'aequalitas op comparatio' - reduce P85"),
    STATUS_INFO(245, "after aequalitas inside parens - shift ) or continue"),
    STATUS_INFO(246, "after comparatio in if condition - reduce P87 or cont"),
    STATUS_INFO(247, "after aequalitas in if condition - shift ) or cont"),
    STATUS_INFO(248, "after aequalitas in while condition - shift ) or cont"),
    STATUS_INFO(249, "after comparatio in for condition - reduce P87 or cont"),
    STATUS_INFO(250, "after aequalitas in for condition - P28 or continue"),
    STATUS_INFO(251, "after aequalitas in do-while - shift ) or continue"),
    STATUS_INFO(252, "after aequalitas in switch - shift ) or continue"),
    STATUS_INFO(253, "after coniunctio at top-level - P94 or continue &&"),
    STATUS_INFO(254, "after 'coniunctio &&' - expect aequalitas"),
    STATUS_INFO(255, "after disiunctio at top-level - ACCEPT or continue ||"),
    STATUS_INFO(256, "after 'disiunctio ||' - expect coniunctio"),
    STATUS_INFO(257, "after aequalitas in && context - reduce P96 or cont"),
    STATUS_INFO(258, "after 'coniunctio && aequalitas' - reduce P95"),
    STATUS_INFO(259, "after aequalitas in || context - reduce P96"),
    STATUS_INFO(260, "after coniunctio in || context - P93 or continue &&"),
    STATUS_INFO(261, "after 'coniunctio &&' in || context - expect aeq"),
    STATUS_INFO(262, "after aequalitas in && within || - reduce P96"),
    STATUS_INFO(263, "after 'coniunctio && aequalitas' in || - reduce P95"),
    STATUS_INFO(264, "after translatio - reduce P92 or continue shift"),
    STATUS_INFO(265, "after translatio << or >> - expect expression"),
    STATUS_INFO(266, "after translatio << expression - reduce P97 or cont"),
    STATUS_INFO(267, "after translatio in 'comparatio <' - reduce P88"),
    /* States 268-288: Bitwise operator states */
    STATUS_INFO(268, "after AMPERSAND_BITWISE top-level - shift & or P103"),
    STATUS_INFO(269, "after '&' bitwise top-level - expr starters"),
    STATUS_INFO(270, "after CARET_BITWISE top-level - shift ^ or P101"),
    STATUS_INFO(271, "after '^' bitwise top-level - expr starters"),
    STATUS_INFO(272, "after PIPA_BITWISE top-level - shift | or P96"),
    STATUS_INFO(273, "after '|' bitwise top-level - expr starters"),
    STATUS_INFO(274, "after aequalitas in & context - P104 or =="),
    STATUS_INFO(275, "after aequalitas in ^ context - P105 or =="),
    STATUS_INFO(276, "after ampersand_bitwise in ^ context - & or P102"),
    STATUS_INFO(277, "after aequalitas in | context - P105 or =="),
    STATUS_INFO(278, "after ampersand_bitwise in | context - & or P103"),
    STATUS_INFO(279, "after caret_bitwise in | context - ^ or P100"),
    STATUS_INFO(280, "after & within ^ context - expr starters"),
    STATUS_INFO(281, "after & within | context - expr starters"),
    STATUS_INFO(282, "after ^ within | context - expr starters"),
    STATUS_INFO(283, "after aequalitas in & in ^ - P104 or =="),
    STATUS_INFO(284, "after aequalitas in & in | - P104 or =="),
    STATUS_INFO(285, "after aequalitas in ^ in | - P105 or =="),
    STATUS_INFO(286, "after ampersand_bitwise in ^ in | - & or P102"),
    STATUS_INFO(287, "after & in ^ in | context - expr starters"),
    STATUS_INFO(288, "after aequalitas in & in ^ in | - P104 or =="),
    /* States 289-292: Unary ~ and ! operators */
    STATUS_INFO(289, "after ~ - expects factor starters"),
    STATUS_INFO(290, "after ~ factor - reduce P106"),
    STATUS_INFO(291, "after ! - expects factor starters"),
    STATUS_INFO(292, "after ! factor - reduce P107"),
    /* States 293-298: Assignment and Comma operators */
    STATUS_INFO(293, "after assignment op - expects expression"),
    STATUS_INFO(294, "after disiunctio = assignatio - reduce P110"),
    STATUS_INFO(295, "after ASSIGNATIO - comma or reduce P109"),
    STATUS_INFO(296, "after VIRGA - ACCEPT or continue comma"),
    STATUS_INFO(297, "after comma - expects assignatio"),
    STATUS_INFO(298, "after virga , assignatio - reduce P108"),
    STATUS_INFO(299, "after VIRGA in if condition - shift ) to 33"),
    STATUS_INFO(300, "after VIRGA in while condition - shift ) to 42"),
    STATUS_INFO(301, "after VIRGA in for-init - reduce P122"),
    STATUS_INFO(302, "after VIRGA in for-condition - reduce P122"),
    STATUS_INFO(303, "after VIRGA in for-increment - reduce P122"),
    STATUS_INFO(304, "after VIRGA in do-while condition - shift ) to 50"),
    STATUS_INFO(305, "after VIRGA in switch condition - shift ) to 82"),
    /* States 306-310: Ternary operator */
    STATUS_INFO(306, "after ? - expects expression (true branch)"),
    STATUS_INFO(307, "after disiunctio ? ternarius - expects :"),
    STATUS_INFO(308, "after : - expects expression (false branch)"),
    STATUS_INFO(309, "after disiunctio ? ternarius : ternarius - reduce P123"),
    STATUS_INFO(310, "after TERNARIUS - assignment or reduce P121"),

    /* States 311-314: Postfix operators (subscript) */
    STATUS_INFO(311, "after POSTFIXUM - check for postfix ops or reduce to factor"),
    STATUS_INFO(312, "after postfixum '[' - expects subscript expression"),
    STATUS_INFO(313, "after postfixum '[' expr - expects ']' or continue expr"),
    STATUS_INFO(314, "after postfixum '[' expr ']' - reduce P128 subscript"),

    /* States 315-321: Function call */
    STATUS_INFO(315, "after postfixum '(' - expects ')' or arguments"),
    STATUS_INFO(316, "after postfixum '(' argumenta - expects ')' or ','"),
    STATUS_INFO(317, "after postfixum '(' argumenta ',' - expects next argument"),
    STATUS_INFO(318, "after postfixum '(' ')' - reduce P130 empty call"),
    STATUS_INFO(319, "after postfixum '(' argumenta ')' - reduce P131 call with args"),
    STATUS_INFO(320, "after postfixum '(' argumenta ',' assignatio - reduce P133 args"),
    STATUS_INFO(321, "after postfixum '(' assignatio - reduce P132 first arg"),
    STATUS_INFO(322, "after postfixum '.' - expects member ID"),
    STATUS_INFO(323, "after postfixum '.' ID - reduce P134 member access"),
    STATUS_INFO(324, "after postfixum '->' - expects member ID"),
    STATUS_INFO(325, "after postfixum '->' ID - reduce P135 ptr member"),
    STATUS_INFO(326, "after postfixum '++' - reduce P136 post-increment"),
    STATUS_INFO(327, "after postfixum '--' - reduce P137 post-decrement"),

    /* Pre-increment/decrement states */
    STATUS_INFO(328, "after '++' prefix - expects factor"),
    STATUS_INFO(329, "after '--' prefix - expects factor"),
    STATUS_INFO(330, "after '++ factor' - reduce P138 pre-increment"),
    STATUS_INFO(331, "after '-- factor' - reduce P139 pre-decrement"),

    /* Additional literal states */
    STATUS_INFO(332, "after FLOAT_LIT - reduce P140"),
    STATUS_INFO(333, "after CHAR_LIT - reduce P141"),
    STATUS_INFO(334, "after STRING_LIT - reduce P142"),

    /* sizeof states */
    STATUS_INFO(335, "after 'sizeof' - expects factor"),
    STATUS_INFO(336, "after 'sizeof' factor - reduce P143")
};

/* ==================================================
 * LR(1) Action Table (OLD - will be removed after migration)
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
#define INT_NT_PIPA_BITWISE  27
#define INT_NT_CARET_BITWISE 28
#define INT_NT_AMPERSAND_BITWISE 29
#define INT_NT_VIRGA             30
#define INT_NT_ASSIGNATIO        31
#define INT_NT_TERNARIUS         32
#define INT_NT_POSTFIXUM         33
#define INT_NT_ARGUMENTA         34

/* ==================================================
 * Per-State GOTO Arrays (Phase 4 refactor)
 *
 * New structure: Arbor2StatusGotoEntry { non_terminalis, status_novus }
 * sizeof-computed counts eliminate manual maintenance
 * ================================================== */

/* State 0: initial - expects expression/declaration/statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_0_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_POSTFIXUM,        311 },  /* postfix expression for subscript/call/member */
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_DECLARATIO,  21 },
    { INT_NT_SENTENTIA,   24 },
    { INT_NT_CORPUS,      29 },
    { INT_NT_SI,          37 },
    { INT_NT_DUM,         44 },
    { INT_NT_FAC,         52 },
    { INT_NT_PER,         65 },
    { INT_NT_DEFINITIO,   114 },
    { INT_NT_STRUCT_SPEC, 4 },
    { INT_NT_ENUM_SPEC,   4 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 4: after identifier as type_specifier */
hic_manens constans Arbor2StatusGotoEntry STATUS_4_GOTO[] = {
    { INT_NT_DECLARATOR, 20 }
};

/* State 6: after '(' - full expression chain inside parens */
hic_manens constans Arbor2StatusGotoEntry STATUS_6_GOTO[] = {
    { INT_NT_EXPR,        11 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_POSTFIXUM,   311 },  /* postfix for subscript */
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  245 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE, 270 },
    { INT_NT_PIPA_BITWISE, 272 },
    { INT_NT_CONIUNCTIO,  253 },
    { INT_NT_DISIUNCTIO,  255 },
    { INT_NT_TERNARIUS,   310 },
    { INT_NT_ASSIGNATIO,  295 },
    { INT_NT_VIRGA,       296 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 7: after unary '*' */
hic_manens constans Arbor2StatusGotoEntry STATUS_7_GOTO[] = {
    { INT_NT_FACTOR, 15 },
    { INT_NT_POSTFIXUM, 311 }  /* postfix for subscript e.g. *arr[i] */
};

/* State 8: after unary '&' */
hic_manens constans Arbor2StatusGotoEntry STATUS_8_GOTO[] = {
    { INT_NT_FACTOR, 16 },
    { INT_NT_POSTFIXUM, 311 }  /* postfix for subscript e.g. &arr[i] */
};

/* State 9: after expr '+' or '-' */
hic_manens constans Arbor2StatusGotoEntry STATUS_9_GOTO[] = {
    { INT_NT_TERM,       13 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_POSTFIXUM,  311 },  /* postfix for subscript */
    { INT_NT_TRANSLATIO, 264 }
};

/* State 10: after term '*' or '/' */
hic_manens constans Arbor2StatusGotoEntry STATUS_10_GOTO[] = {
    { INT_NT_FACTOR, 14 }
};

/* State 17: after '*' in declarator */
hic_manens constans Arbor2StatusGotoEntry STATUS_17_GOTO[] = {
    { INT_NT_DECLARATOR, 19 }
};

/* State 20: after declarator */
hic_manens constans Arbor2StatusGotoEntry STATUS_20_GOTO[] = {
    { INT_NT_CORPUS, 113 }
};

/* State 25: after epsilon reduce to empty list */
hic_manens constans Arbor2StatusGotoEntry STATUS_25_GOTO[] = {
    { INT_NT_ELENCHUS, 26 }
};

/* State 26: inside compound, after expression components */
hic_manens constans Arbor2StatusGotoEntry STATUS_26_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        28 },
    { INT_NT_CORPUS,           29 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 31: after 'if (' - expression components */
hic_manens constans Arbor2StatusGotoEntry STATUS_31_GOTO[] = {
    { INT_NT_EXPR,        32 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  246 },
    { INT_NT_AEQUALITAS,  247 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,  253 },
    { INT_NT_DISIUNCTIO,  255 },
    { INT_NT_TERNARIUS,   310 },
    { INT_NT_ASSIGNATIO,  295 },
    { INT_NT_VIRGA,       299 },  /* if-context: shift ) to state 33 */
    { INT_NT_TRANSLATIO,  264 }
};

/* State 33: after 'if ( expr )' - then-branch statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_33_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        34 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 35: after 'if ( expr ) stmt else' - else-branch statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_35_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        36 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 40: after 'while (' - condition expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_40_GOTO[] = {
    { INT_NT_EXPR,              41 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        246 },
    { INT_NT_AEQUALITAS,        248 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             300 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 42: after 'while ( expr )' - loop body */
hic_manens constans Arbor2StatusGotoEntry STATUS_42_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        43 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 45: after 'do' - loop body */
hic_manens constans Arbor2StatusGotoEntry STATUS_45_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        46 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 48: after 'do stmt while (' - condition expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_48_GOTO[] = {
    { INT_NT_EXPR,              49 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        246 },
    { INT_NT_AEQUALITAS,        251 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             304 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 54: after 'for (' - init expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_54_GOTO[] = {
    { INT_NT_EXPR,              55 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        239 },
    { INT_NT_AEQUALITAS,        240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             301 },
    { INT_NT_EXPRESSIO_OPT,     56 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 57: after 'for ( expr_opt ;' - condition expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_57_GOTO[] = {
    { INT_NT_EXPR,              58 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        249 },
    { INT_NT_AEQUALITAS,        250 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             302 },
    { INT_NT_EXPRESSIO_OPT,     59 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 60: after 'for ( ... ;' - increment expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_60_GOTO[] = {
    { INT_NT_EXPR,              61 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        239 },
    { INT_NT_AEQUALITAS,        240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             303 },
    { INT_NT_EXPRESSIO_OPT,     62 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 63: after 'for ( ... )' - loop body */
hic_manens constans Arbor2StatusGotoEntry STATUS_63_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        64 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 70: after 'return' - expression for return value */
hic_manens constans Arbor2StatusGotoEntry STATUS_70_GOTO[] = {
    { INT_NT_EXPR,          71 },
    { INT_NT_TERM,          2 },
    { INT_NT_FACTOR,        3 },
    { INT_NT_EXPRESSIO_OPT, 72 },
    { INT_NT_TRANSLATIO,    264 }
};

/* State 77: after 'IDENTIFIER :' - labeled statement body */
hic_manens constans Arbor2StatusGotoEntry STATUS_77_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        78 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 80: after 'switch (' - expression components */
hic_manens constans Arbor2StatusGotoEntry STATUS_80_GOTO[] = {
    { INT_NT_EXPR,              81 },
    { INT_NT_TERM,              2 },
    { INT_NT_FACTOR,            3 },
    { INT_NT_COMPARATIO,        246 },
    { INT_NT_AEQUALITAS,        252 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        295 },
    { INT_NT_VIRGA,             305 },
    { INT_NT_TRANSLATIO,        264 }
};

/* State 82: after 'switch ( expr )' - body statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_82_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       295 },
    { INT_NT_VIRGA,            296 },
    { INT_NT_SENTENTIA,        83 },
    { INT_NT_CORPUS,           38 },
    { INT_NT_SI,               37 },
    { INT_NT_DUM,              44 },
    { INT_NT_FAC,              52 },
    { INT_NT_PER,              65 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 84: after 'case' - expression components */
hic_manens constans Arbor2StatusGotoEntry STATUS_84_GOTO[] = {
    { INT_NT_EXPR,       85 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 86: after 'case expr :' - body statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_86_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,  253 },
    { INT_NT_DISIUNCTIO,  255 },
    { INT_NT_TERNARIUS,   310 },
    { INT_NT_ASSIGNATIO,  295 },
    { INT_NT_VIRGA,       296 },
    { INT_NT_SENTENTIA,   87 },
    { INT_NT_CORPUS,      38 },
    { INT_NT_SI,          37 },
    { INT_NT_DUM,         44 },
    { INT_NT_FAC,         52 },
    { INT_NT_PER,         65 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 89: after 'default :' - body statement */
hic_manens constans Arbor2StatusGotoEntry STATUS_89_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,  253 },
    { INT_NT_DISIUNCTIO,  255 },
    { INT_NT_TERNARIUS,   310 },
    { INT_NT_ASSIGNATIO,  295 },
    { INT_NT_VIRGA,       296 },
    { INT_NT_SENTENTIA,   90 },
    { INT_NT_CORPUS,      38 },
    { INT_NT_SI,          37 },
    { INT_NT_DUM,         44 },
    { INT_NT_FAC,         52 },
    { INT_NT_PER,         65 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 91: parameter list start */
hic_manens constans Arbor2StatusGotoEntry STATUS_91_GOTO[] = {
    { INT_NT_PARAM_DECL, 101 },
    { INT_NT_PARAM_LIST, 102 }
};

/* State 95: after type_spec in parameter */
hic_manens constans Arbor2StatusGotoEntry STATUS_95_GOTO[] = {
    { INT_NT_DECLARATOR, 99 }
};

/* State 96: after type_spec * in parameter */
hic_manens constans Arbor2StatusGotoEntry STATUS_96_GOTO[] = {
    { INT_NT_DECLARATOR, 100 }
};

/* State 104: after ',' in parameter list */
hic_manens constans Arbor2StatusGotoEntry STATUS_104_GOTO[] = {
    { INT_NT_PARAM_DECL, 111 }
};

/* State 105: after ', type_spec' in parameter list */
hic_manens constans Arbor2StatusGotoEntry STATUS_105_GOTO[] = {
    { INT_NT_DECLARATOR, 109 }
};

/* State 106: after ', type_spec *' in parameter list */
hic_manens constans Arbor2StatusGotoEntry STATUS_106_GOTO[] = {
    { INT_NT_DECLARATOR, 110 }
};

/* State 119: anonymous struct - expects member list */
hic_manens constans Arbor2StatusGotoEntry STATUS_119_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 127 },
    { INT_NT_STRUCT_SPEC,    174 },
    { INT_NT_ENUM_SPEC,      186 }
};

/* State 120: named struct - expects member list */
hic_manens constans Arbor2StatusGotoEntry STATUS_120_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 129 },
    { INT_NT_STRUCT_SPEC,    174 },
    { INT_NT_ENUM_SPEC,      186 }
};

/* State 121: type_spec in first struct member */
hic_manens constans Arbor2StatusGotoEntry STATUS_121_GOTO[] = {
    { INT_NT_DECLARATOR, 221 }
};

/* State 127: more members in anonymous struct */
hic_manens constans Arbor2StatusGotoEntry STATUS_127_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 127 },
    { INT_NT_STRUCT_SPEC,    180 },
    { INT_NT_ENUM_SPEC,      192 }
};

/* State 129: more members in named struct */
hic_manens constans Arbor2StatusGotoEntry STATUS_129_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 129 },
    { INT_NT_STRUCT_SPEC,    180 },
    { INT_NT_ENUM_SPEC,      192 }
};

/* State 131: type_spec in subsequent struct member */
hic_manens constans Arbor2StatusGotoEntry STATUS_131_GOTO[] = {
    { INT_NT_DECLARATOR, 223 }
};

/* State 139: anonymous union - expects member list */
hic_manens constans Arbor2StatusGotoEntry STATUS_139_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 141 },
    { INT_NT_STRUCT_SPEC,    174 },
    { INT_NT_ENUM_SPEC,      186 }
};

/* State 140: named union - expects member list */
hic_manens constans Arbor2StatusGotoEntry STATUS_140_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 143 },
    { INT_NT_STRUCT_SPEC,    174 },
    { INT_NT_ENUM_SPEC,      186 }
};

/* State 141: more members in anonymous union */
hic_manens constans Arbor2StatusGotoEntry STATUS_141_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 141 },
    { INT_NT_STRUCT_SPEC,    180 },
    { INT_NT_ENUM_SPEC,      192 }
};

/* State 143: more members in named union */
hic_manens constans Arbor2StatusGotoEntry STATUS_143_GOTO[] = {
    { INT_NT_STRUCT_MEMBERS, 143 },
    { INT_NT_STRUCT_SPEC,    180 },
    { INT_NT_ENUM_SPEC,      192 }
};

/* State 147: anonymous enum - expects enumerator list */
hic_manens constans Arbor2StatusGotoEntry STATUS_147_GOTO[] = {
    { INT_NT_ENUMERATOR, 152 },
    { INT_NT_ENUM_LIST,  153 }
};

/* State 148: named enum - expects enumerator list */
hic_manens constans Arbor2StatusGotoEntry STATUS_148_GOTO[] = {
    { INT_NT_ENUMERATOR, 152 },
    { INT_NT_ENUM_LIST,  155 }
};

/* State 150: expression for enumerator value */
hic_manens constans Arbor2StatusGotoEntry STATUS_150_GOTO[] = {
    { INT_NT_EXPR,       151 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 156: subsequent enumerator after comma */
hic_manens constans Arbor2StatusGotoEntry STATUS_156_GOTO[] = {
    { INT_NT_ENUMERATOR, 161 }
};

/* State 159: expression for subsequent enumerator value */
hic_manens constans Arbor2StatusGotoEntry STATUS_159_GOTO[] = {
    { INT_NT_EXPR,       160 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 162: expression for first named bit field */
hic_manens constans Arbor2StatusGotoEntry STATUS_162_GOTO[] = {
    { INT_NT_EXPR,       163 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 165: expression for subsequent named bit field */
hic_manens constans Arbor2StatusGotoEntry STATUS_165_GOTO[] = {
    { INT_NT_EXPR,       166 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 168: expression for first anonymous bit field */
hic_manens constans Arbor2StatusGotoEntry STATUS_168_GOTO[] = {
    { INT_NT_EXPR,       169 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 171: expression for subsequent anonymous bit field */
hic_manens constans Arbor2StatusGotoEntry STATUS_171_GOTO[] = {
    { INT_NT_EXPR,       172 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 174: nested struct_spec in first member context */
hic_manens constans Arbor2StatusGotoEntry STATUS_174_GOTO[] = {
    { INT_NT_DECLARATOR, 231 }
};

/* State 180: nested struct_spec in subsequent member context */
hic_manens constans Arbor2StatusGotoEntry STATUS_180_GOTO[] = {
    { INT_NT_DECLARATOR, 233 }
};

/* State 186: nested enum_spec in first member context */
hic_manens constans Arbor2StatusGotoEntry STATUS_186_GOTO[] = {
    { INT_NT_DECLARATOR, 235 }
};

/* State 192: nested enum_spec in subsequent member context */
hic_manens constans Arbor2StatusGotoEntry STATUS_192_GOTO[] = {
    { INT_NT_DECLARATOR, 237 }
};

/* State 198: after 'typedef' */
hic_manens constans Arbor2StatusGotoEntry STATUS_198_GOTO[] = {
    { INT_NT_STRUCT_SPEC, 205 },
    { INT_NT_ENUM_SPEC,   211 }
};

/* State 199: typedef type_spec - expects declarator */
hic_manens constans Arbor2StatusGotoEntry STATUS_199_GOTO[] = {
    { INT_NT_DECLARATOR, 225 }
};

/* State 205: typedef struct_spec - expects declarator */
hic_manens constans Arbor2StatusGotoEntry STATUS_205_GOTO[] = {
    { INT_NT_DECLARATOR, 227 }
};

/* State 211: typedef enum_spec - expects declarator */
hic_manens constans Arbor2StatusGotoEntry STATUS_211_GOTO[] = {
    { INT_NT_DECLARATOR, 229 }
};

/* State 217: after 'declarator [' - array size expression */
hic_manens constans Arbor2StatusGotoEntry STATUS_217_GOTO[] = {
    { INT_NT_EXPR,       219 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 264 }
};

/* State 241: after 'comparatio <|>|<=|>=' */
hic_manens constans Arbor2StatusGotoEntry STATUS_241_GOTO[] = {
    { INT_NT_EXPR,       243 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 267 }
};

/* State 242: after 'aequalitas ==|!=' */
hic_manens constans Arbor2StatusGotoEntry STATUS_242_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  244 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 254: after 'coniunctio &&' - parse RHS aequalitas */
hic_manens constans Arbor2StatusGotoEntry STATUS_254_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  257 },
    { INT_NT_CONIUNCTIO,  258 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 256: after 'disiunctio ||' - parse RHS coniunctio */
hic_manens constans Arbor2StatusGotoEntry STATUS_256_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  259 },
    { INT_NT_CONIUNCTIO,  260 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 261: after 'coniunctio &&' within || context */
hic_manens constans Arbor2StatusGotoEntry STATUS_261_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  262 },
    { INT_NT_CONIUNCTIO,  263 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 265: after translatio << or >> */
hic_manens constans Arbor2StatusGotoEntry STATUS_265_GOTO[] = {
    { INT_NT_EXPR,       266 },
    { INT_NT_TERM,       2 },
    { INT_NT_FACTOR,     3 },
    { INT_NT_TRANSLATIO, 266 }
};

/* State 269: after '&' bitwise - parse RHS aequalitas */
hic_manens constans Arbor2StatusGotoEntry STATUS_269_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  274 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 271: after '^' bitwise - parse RHS aequalitas->ampersand_bitwise chain */
hic_manens constans Arbor2StatusGotoEntry STATUS_271_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       275 },
    { INT_NT_AMPERSAND_BITWISE, 276 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 273: after '|' bitwise - parse RHS caret_bitwise chain */
hic_manens constans Arbor2StatusGotoEntry STATUS_273_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       277 },
    { INT_NT_AMPERSAND_BITWISE, 278 },
    { INT_NT_CARET_BITWISE,    279 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 280: after '&' within '^' context - parse aequalitas for nested & */
hic_manens constans Arbor2StatusGotoEntry STATUS_280_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  283 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 281: after '&' within '|' context - parse aequalitas for nested & */
hic_manens constans Arbor2StatusGotoEntry STATUS_281_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  284 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 282: after '^' within '|' context - parse ampersand_bitwise for nested ^ */
hic_manens constans Arbor2StatusGotoEntry STATUS_282_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       285 },
    { INT_NT_AMPERSAND_BITWISE, 286 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 287: after '&' within '^ in |' context - parse aequalitas */
hic_manens constans Arbor2StatusGotoEntry STATUS_287_GOTO[] = {
    { INT_NT_EXPR,        1 },
    { INT_NT_TERM,        2 },
    { INT_NT_FACTOR,      3 },
    { INT_NT_COMPARATIO,  239 },
    { INT_NT_AEQUALITAS,  288 },
    { INT_NT_TRANSLATIO,  264 }
};

/* State 289: after ~ - GOTO for FACTOR to state 290 */
hic_manens constans Arbor2StatusGotoEntry STATUS_289_GOTO[] = {
    { INT_NT_FACTOR, 290 }
};

/* State 291: after ! - GOTO for FACTOR to state 292 */
hic_manens constans Arbor2StatusGotoEntry STATUS_291_GOTO[] = {
    { INT_NT_FACTOR, 292 }
};

/* State 293: after assignment op - need ASSIGNATIO for RHS */
hic_manens constans Arbor2StatusGotoEntry STATUS_293_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       294 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 297: after comma - need ASSIGNATIO for RHS */
hic_manens constans Arbor2StatusGotoEntry STATUS_297_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        310 },
    { INT_NT_ASSIGNATIO,       298 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 306: after '?' - expects expression (true branch), needs TERNARIUS -> 307 */
hic_manens constans Arbor2StatusGotoEntry STATUS_306_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        307 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 308: after ':' - expects expression (false branch), needs TERNARIUS -> 309 */
hic_manens constans Arbor2StatusGotoEntry STATUS_308_GOTO[] = {
    { INT_NT_EXPR,             1 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TERNARIUS,        309 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 312: after 'postfixum [' - expects subscript expression, needs EXPR -> 313 */
hic_manens constans Arbor2StatusGotoEntry STATUS_312_GOTO[] = {
    { INT_NT_EXPR,             313 },
    { INT_NT_TERM,             2 },
    { INT_NT_FACTOR,           3 },
    { INT_NT_POSTFIXUM,        311 },  /* subscript expressions can themselves have subscripts */
    { INT_NT_COMPARATIO,       239 },
    { INT_NT_AEQUALITAS,       240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,    270 },
    { INT_NT_PIPA_BITWISE,     272 },
    { INT_NT_CONIUNCTIO,       253 },
    { INT_NT_DISIUNCTIO,       255 },
    { INT_NT_TRANSLATIO,       264 }
};

/* State 315: after 'postfixum (' - expects ')' or argument expression
 * Arguments reduce to ASSIGNATIO (not VIRGA, since comma separates args)
 * INT_NT_ASSIGNATIO -> 321 (reduce P132), then ARGUMENTA -> 316 */
hic_manens constans Arbor2StatusGotoEntry STATUS_315_GOTO[] = {
    { INT_NT_EXPR,                1 },   /* expr -> state 1 to handle +/- or reduce P99 */
    { INT_NT_TERM,                2 },
    { INT_NT_FACTOR,              3 },
    { INT_NT_POSTFIXUM,         311 },
    { INT_NT_COMPARATIO,        239 },
    { INT_NT_AEQUALITAS,        240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TRANSLATIO,        264 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        321 },  /* assignatio -> state 321 which reduces P132 */
    { INT_NT_ARGUMENTA,         316 }   /* argumenta -> state 316 to check ')' or ',' */
};

/* State 317: after 'postfixum ( argumenta ,' - expects next argument
 * INT_NT_ASSIGNATIO -> 320 (reduce P133), then ARGUMENTA -> 316 */
hic_manens constans Arbor2StatusGotoEntry STATUS_317_GOTO[] = {
    { INT_NT_EXPR,                1 },   /* expr -> state 1 to handle +/- or reduce P99 */
    { INT_NT_TERM,                2 },
    { INT_NT_FACTOR,              3 },
    { INT_NT_POSTFIXUM,         311 },
    { INT_NT_COMPARATIO,        239 },
    { INT_NT_AEQUALITAS,        240 },
    { INT_NT_AMPERSAND_BITWISE, 268 },
    { INT_NT_CARET_BITWISE,     270 },
    { INT_NT_PIPA_BITWISE,      272 },
    { INT_NT_CONIUNCTIO,        253 },
    { INT_NT_DISIUNCTIO,        255 },
    { INT_NT_TRANSLATIO,        264 },
    { INT_NT_TERNARIUS,         310 },
    { INT_NT_ASSIGNATIO,        320 }   /* assignatio -> state 320 which reduces P133 */
};

/* State 328: after '++' prefix - expects factor */
hic_manens constans Arbor2StatusGotoEntry STATUS_328_GOTO[] = {
    { INT_NT_FACTOR, 330 },
    { INT_NT_POSTFIXUM, 311 }  /* postfix for chaining e.g. ++arr[i] */
};

/* State 329: after '--' prefix - expects factor */
hic_manens constans Arbor2StatusGotoEntry STATUS_329_GOTO[] = {
    { INT_NT_FACTOR, 331 },
    { INT_NT_POSTFIXUM, 311 }  /* postfix for chaining e.g. --arr[i] */
};

/* State 335: after 'sizeof' - expects factor */
hic_manens constans Arbor2StatusGotoEntry STATUS_335_GOTO[] = {
    { INT_NT_FACTOR, 336 },
    { INT_NT_POSTFIXUM, 311 }  /* postfix for chaining e.g. sizeof arr[i] */
};

/* ==================================================
 * STATUS_GOTO Macro and Master Table
 * ================================================== */

#define STATUS_GOTO(n) { \
    STATUS_##n##_GOTO, \
    (s32)(magnitudo(STATUS_##n##_GOTO) / magnitudo(STATUS_##n##_GOTO[0])) \
}

#define STATUS_GOTO_NIL { NIHIL, 0 }

/* Master GOTO table - indexed by state number */
hic_manens constans Arbor2StatusGoto GOTO_TABULA_NOVA[] = {
    STATUS_GOTO(0),    /* 0: initial */
    STATUS_GOTO_NIL,   /* 1: after expression */
    STATUS_GOTO_NIL,   /* 2: after term */
    STATUS_GOTO_NIL,   /* 3: after factor */
    STATUS_GOTO(4),    /* 4: after ID as type_spec */
    STATUS_GOTO_NIL,   /* 5: after INTEGER */
    STATUS_GOTO(6),    /* 6: after '(' */
    STATUS_GOTO(7),    /* 7: after unary '*' */
    STATUS_GOTO(8),    /* 8: after unary '&' */
    STATUS_GOTO(9),    /* 9: after expr '+' */
    STATUS_GOTO(10),   /* 10: after term '*' */
    STATUS_GOTO_NIL,   /* 11: after '(' expression */
    STATUS_GOTO_NIL,   /* 12: after '(' expression ')' */
    STATUS_GOTO_NIL,   /* 13: after expression '+' term */
    STATUS_GOTO_NIL,   /* 14: after term '*' factor */
    STATUS_GOTO_NIL,   /* 15: after '*' factor (unary) */
    STATUS_GOTO_NIL,   /* 16: after '&' factor (unary) */
    STATUS_GOTO(17),   /* 17: after '*' in declarator */
    STATUS_GOTO_NIL,   /* 18: after ID in declarator */
    STATUS_GOTO_NIL,   /* 19: after pointer declarator */
    STATUS_GOTO(20),   /* 20: after declarator */
    STATUS_GOTO_NIL,   /* 21: after declaration */
    STATUS_GOTO_NIL,   /* 22: reserved */
    STATUS_GOTO_NIL,   /* 23: reserved */
    STATUS_GOTO_NIL,   /* 24: after expression statement */
    STATUS_GOTO(25),   /* 25: after '{' - epsilon reduce */
    STATUS_GOTO(26),   /* 26: inside compound */
    STATUS_GOTO_NIL,   /* 27: reserved */
    STATUS_GOTO_NIL,   /* 28: statement in list */
    STATUS_GOTO_NIL,   /* 29: after compound */
    STATUS_GOTO_NIL,   /* 30: after 'if' */
    STATUS_GOTO(31),   /* 31: after 'if (' */
    STATUS_GOTO_NIL,   /* 32: after 'if (' expression */
    STATUS_GOTO(33),   /* 33: after 'if ( expr )' */
    STATUS_GOTO_NIL,   /* 34: then-branch statement */
    STATUS_GOTO(35),   /* 35: after 'if ( expr ) stmt else' */
    STATUS_GOTO_NIL,   /* 36: else-branch statement */
    STATUS_GOTO_NIL,   /* 37: after if_statement */
    STATUS_GOTO_NIL,   /* 38: nested compound */
    STATUS_GOTO_NIL,   /* 39: after 'while' */
    STATUS_GOTO(40),   /* 40: after 'while (' */
    STATUS_GOTO_NIL,   /* 41: after 'while (' expression */
    STATUS_GOTO(42),   /* 42: after 'while ( expr )' */
    STATUS_GOTO_NIL,   /* 43: while body statement */
    STATUS_GOTO_NIL,   /* 44: after while_statement */
    STATUS_GOTO(45),   /* 45: after 'do' */
    STATUS_GOTO_NIL,   /* 46: do body statement */
    STATUS_GOTO_NIL,   /* 47: after 'do stmt while' */
    STATUS_GOTO(48),   /* 48: after 'do stmt while (' */
    STATUS_GOTO_NIL,   /* 49: after 'do stmt while (' expression */
    STATUS_GOTO_NIL,   /* 50: after 'do stmt while ( expr )' */
    STATUS_GOTO_NIL,   /* 51: reserved */
    STATUS_GOTO_NIL,   /* 52: after do_statement */
    STATUS_GOTO_NIL,   /* 53: after 'for' */
    STATUS_GOTO(54),   /* 54: after 'for (' */
    STATUS_GOTO_NIL,   /* 55: for init expression */
    STATUS_GOTO_NIL,   /* 56: after for expr_opt (init) */
    STATUS_GOTO(57),   /* 57: after 'for ( expr_opt ;' */
    STATUS_GOTO_NIL,   /* 58: for condition expression */
    STATUS_GOTO_NIL,   /* 59: after for expr_opt (condition) */
    STATUS_GOTO(60),   /* 60: after 'for ( ... ;' */
    STATUS_GOTO_NIL,   /* 61: for increment expression */
    STATUS_GOTO_NIL,   /* 62: after for expr_opt (increment) */
    STATUS_GOTO(63),   /* 63: after 'for ( ... )' */
    STATUS_GOTO_NIL,   /* 64: for body statement */
    STATUS_GOTO_NIL,   /* 65: after for_statement */
    STATUS_GOTO_NIL,   /* 66: reserved */
    STATUS_GOTO_NIL,   /* 67: reserved */
    STATUS_GOTO_NIL,   /* 68: after 'break' */
    STATUS_GOTO_NIL,   /* 69: after 'continue' */
    STATUS_GOTO(70),   /* 70: after 'return' */
    STATUS_GOTO_NIL,   /* 71: return expression */
    STATUS_GOTO_NIL,   /* 72: after return expr_opt */
    STATUS_GOTO_NIL,   /* 73: reserved */
    STATUS_GOTO_NIL,   /* 74: reserved */
    STATUS_GOTO_NIL,   /* 75: after 'goto' */
    STATUS_GOTO_NIL,   /* 76: after 'goto ID' */
    STATUS_GOTO(77),   /* 77: after 'ID :' */
    STATUS_GOTO_NIL,   /* 78: labeled statement */
    STATUS_GOTO_NIL,   /* 79: after 'switch' */
    STATUS_GOTO(80),   /* 80: after 'switch (' */
    STATUS_GOTO_NIL,   /* 81: switch condition expression */
    STATUS_GOTO(82),   /* 82: after 'switch ( expr )' */
    STATUS_GOTO_NIL,   /* 83: switch body statement */
    STATUS_GOTO(84),   /* 84: after 'case' */
    STATUS_GOTO_NIL,   /* 85: case expression */
    STATUS_GOTO(86),   /* 86: after 'case expr :' */
    STATUS_GOTO_NIL,   /* 87: case body statement */
    STATUS_GOTO_NIL,   /* 88: after 'default' */
    STATUS_GOTO(89),   /* 89: after 'default :' */
    STATUS_GOTO_NIL,   /* 90: default body statement */
    STATUS_GOTO(91),   /* 91: param list start */
    STATUS_GOTO_NIL,   /* 92: reserved */
    STATUS_GOTO_NIL,   /* 93: reserved */
    STATUS_GOTO_NIL,   /* 94: reserved */
    STATUS_GOTO(95),   /* 95: after type_spec in param */
    STATUS_GOTO(96),   /* 96: after type_spec * in param */
    STATUS_GOTO_NIL,   /* 97: reserved */
    STATUS_GOTO_NIL,   /* 98: reserved */
    STATUS_GOTO_NIL,   /* 99: param declarator */
    STATUS_GOTO_NIL,   /* 100: pointer param declarator */
    STATUS_GOTO_NIL,   /* 101: after param_decl */
    STATUS_GOTO_NIL,   /* 102: after param_list */
    STATUS_GOTO_NIL,   /* 103: reserved */
    STATUS_GOTO(104),  /* 104: after ',' in param list */
    STATUS_GOTO(105),  /* 105: after ', type_spec' */
    STATUS_GOTO(106),  /* 106: after ', type_spec *' */
    STATUS_GOTO_NIL,   /* 107: reserved */
    STATUS_GOTO_NIL,   /* 108: reserved */
    STATUS_GOTO_NIL,   /* 109: subsequent param declarator */
    STATUS_GOTO_NIL,   /* 110: subsequent pointer param */
    STATUS_GOTO_NIL,   /* 111: after , param_decl */
    STATUS_GOTO_NIL,   /* 112: reserved */
    STATUS_GOTO_NIL,   /* 113: after declarator + compound */
    STATUS_GOTO_NIL,   /* 114: after function_definition */
    STATUS_GOTO_NIL,   /* 115: reserved */
    STATUS_GOTO_NIL,   /* 116: after 'struct' */
    STATUS_GOTO_NIL,   /* 117: after 'struct ID' */
    STATUS_GOTO_NIL,   /* 118: reserved */
    STATUS_GOTO(119),  /* 119: anonymous struct '{' */
    STATUS_GOTO(120),  /* 120: named struct '{' */
    STATUS_GOTO(121),  /* 121: type_spec in first member */
    STATUS_GOTO_NIL,   /* 122: reserved */
    STATUS_GOTO_NIL,   /* 123: reserved */
    STATUS_GOTO_NIL,   /* 124: reserved */
    STATUS_GOTO_NIL,   /* 125: reserved */
    STATUS_GOTO_NIL,   /* 126: reserved */
    STATUS_GOTO(127),  /* 127: more anonymous struct members */
    STATUS_GOTO_NIL,   /* 128: reserved */
    STATUS_GOTO(129),  /* 129: more named struct members */
    STATUS_GOTO_NIL,   /* 130: reserved */
    STATUS_GOTO(131),  /* 131: type_spec in subseq member */
    STATUS_GOTO_NIL,   /* 132: reserved */
    STATUS_GOTO_NIL,   /* 133: reserved */
    STATUS_GOTO_NIL,   /* 134: reserved */
    STATUS_GOTO_NIL,   /* 135: after 'union' */
    STATUS_GOTO_NIL,   /* 136: after 'union ID' */
    STATUS_GOTO_NIL,   /* 137: reserved */
    STATUS_GOTO_NIL,   /* 138: reserved */
    STATUS_GOTO(139),  /* 139: anonymous union '{' */
    STATUS_GOTO(140),  /* 140: named union '{' */
    STATUS_GOTO(141),  /* 141: more anonymous union members */
    STATUS_GOTO_NIL,   /* 142: reserved */
    STATUS_GOTO(143),  /* 143: more named union members */
    STATUS_GOTO_NIL,   /* 144: reserved */
    STATUS_GOTO_NIL,   /* 145: after 'enum' */
    STATUS_GOTO_NIL,   /* 146: after 'enum ID' */
    STATUS_GOTO(147),  /* 147: anonymous enum '{' */
    STATUS_GOTO(148),  /* 148: named enum '{' */
    STATUS_GOTO_NIL,   /* 149: enumerator ID */
    STATUS_GOTO(150),  /* 150: after enumerator '=' */
    STATUS_GOTO_NIL,   /* 151: enumerator value expression */
    STATUS_GOTO_NIL,   /* 152: after enumerator */
    STATUS_GOTO_NIL,   /* 153: anon enum list */
    STATUS_GOTO_NIL,   /* 154: reserved */
    STATUS_GOTO_NIL,   /* 155: named enum list */
    STATUS_GOTO(156),  /* 156: after enum list ',' */
    STATUS_GOTO_NIL,   /* 157: reserved */
    STATUS_GOTO_NIL,   /* 158: subsequent enumerator ID */
    STATUS_GOTO(159),  /* 159: after subseq enumerator '=' */
    STATUS_GOTO_NIL,   /* 160: subseq enumerator value */
    STATUS_GOTO_NIL,   /* 161: after subsequent enumerator */
    STATUS_GOTO(162),  /* 162: first named bit field ':' */
    STATUS_GOTO_NIL,   /* 163: first named bit field width */
    STATUS_GOTO_NIL,   /* 164: reserved */
    STATUS_GOTO(165),  /* 165: subseq named bit field ':' */
    STATUS_GOTO_NIL,   /* 166: subseq named bit field width */
    STATUS_GOTO_NIL,   /* 167: reserved */
    STATUS_GOTO(168),  /* 168: first anon bit field ':' */
    STATUS_GOTO_NIL,   /* 169: first anon bit field width */
    STATUS_GOTO_NIL,   /* 170: reserved */
    STATUS_GOTO(171),  /* 171: subseq anon bit field ':' */
    STATUS_GOTO_NIL,   /* 172: subseq anon bit field width */
    STATUS_GOTO_NIL,   /* 173: reserved */
    STATUS_GOTO(174),  /* 174: nested struct_spec (first) */
    STATUS_GOTO_NIL,   /* 175: reserved */
    STATUS_GOTO_NIL,   /* 176: reserved */
    STATUS_GOTO_NIL,   /* 177: reserved */
    STATUS_GOTO_NIL,   /* 178: reserved */
    STATUS_GOTO_NIL,   /* 179: reserved */
    STATUS_GOTO(180),  /* 180: nested struct_spec (subseq) */
    STATUS_GOTO_NIL,   /* 181: reserved */
    STATUS_GOTO_NIL,   /* 182: reserved */
    STATUS_GOTO_NIL,   /* 183: reserved */
    STATUS_GOTO_NIL,   /* 184: reserved */
    STATUS_GOTO_NIL,   /* 185: reserved */
    STATUS_GOTO(186),  /* 186: nested enum_spec (first) */
    STATUS_GOTO_NIL,   /* 187: reserved */
    STATUS_GOTO_NIL,   /* 188: reserved */
    STATUS_GOTO_NIL,   /* 189: reserved */
    STATUS_GOTO_NIL,   /* 190: reserved */
    STATUS_GOTO_NIL,   /* 191: reserved */
    STATUS_GOTO(192),  /* 192: nested enum_spec (subseq) */
    STATUS_GOTO_NIL,   /* 193: reserved */
    STATUS_GOTO_NIL,   /* 194: reserved */
    STATUS_GOTO_NIL,   /* 195: reserved */
    STATUS_GOTO_NIL,   /* 196: reserved */
    STATUS_GOTO_NIL,   /* 197: reserved */
    STATUS_GOTO(198),  /* 198: after 'typedef' */
    STATUS_GOTO(199),  /* 199: typedef type_spec */
    STATUS_GOTO_NIL,   /* 200: reserved */
    STATUS_GOTO_NIL,   /* 201: reserved */
    STATUS_GOTO_NIL,   /* 202: reserved */
    STATUS_GOTO_NIL,   /* 203: reserved */
    STATUS_GOTO_NIL,   /* 204: reserved */
    STATUS_GOTO(205),  /* 205: typedef struct_spec */
    STATUS_GOTO_NIL,   /* 206: reserved */
    STATUS_GOTO_NIL,   /* 207: reserved */
    STATUS_GOTO_NIL,   /* 208: reserved */
    STATUS_GOTO_NIL,   /* 209: reserved */
    STATUS_GOTO_NIL,   /* 210: reserved */
    STATUS_GOTO(211),  /* 211: typedef enum_spec */
    STATUS_GOTO_NIL,   /* 212: reserved */
    STATUS_GOTO_NIL,   /* 213: reserved */
    STATUS_GOTO_NIL,   /* 214: reserved */
    STATUS_GOTO_NIL,   /* 215: reserved */
    STATUS_GOTO_NIL,   /* 216: reserved */
    STATUS_GOTO(217),  /* 217: declarator '[' */
    STATUS_GOTO_NIL,   /* 218: reserved */
    STATUS_GOTO_NIL,   /* 219: array size expression */
    STATUS_GOTO_NIL,   /* 220: reserved */
    STATUS_GOTO_NIL,   /* 221: first struct member declarator */
    STATUS_GOTO_NIL,   /* 222: reserved */
    STATUS_GOTO_NIL,   /* 223: subseq struct member declarator */
    STATUS_GOTO_NIL,   /* 224: reserved */
    STATUS_GOTO_NIL,   /* 225: typedef type_spec declarator */
    STATUS_GOTO_NIL,   /* 226: reserved */
    STATUS_GOTO_NIL,   /* 227: typedef struct_spec declarator */
    STATUS_GOTO_NIL,   /* 228: reserved */
    STATUS_GOTO_NIL,   /* 229: typedef enum_spec declarator */
    STATUS_GOTO_NIL,   /* 230: reserved */
    STATUS_GOTO_NIL,   /* 231: nested struct_spec declarator (first) */
    STATUS_GOTO_NIL,   /* 232: reserved */
    STATUS_GOTO_NIL,   /* 233: nested struct_spec declarator (subseq) */
    STATUS_GOTO_NIL,   /* 234: reserved */
    STATUS_GOTO_NIL,   /* 235: nested enum_spec declarator (first) */
    STATUS_GOTO_NIL,   /* 236: reserved */
    STATUS_GOTO_NIL,   /* 237: nested enum_spec declarator (subseq) */
    STATUS_GOTO_NIL,   /* 238: reserved */
    STATUS_GOTO_NIL,   /* 239: after comparatio */
    STATUS_GOTO_NIL,   /* 240: after aequalitas (accept) */
    STATUS_GOTO(241),  /* 241: after comparison op */
    STATUS_GOTO(242),  /* 242: after equality op */
    STATUS_GOTO_NIL,   /* 243: after comp op expr */
    STATUS_GOTO_NIL,   /* 244: after eq op comparatio */
    STATUS_GOTO_NIL,   /* 245: after aequalitas in parens */
    STATUS_GOTO_NIL,   /* 246: after comparatio in condition */
    STATUS_GOTO_NIL,   /* 247: after aequalitas in if cond */
    STATUS_GOTO_NIL,   /* 248: after aequalitas in while cond */
    STATUS_GOTO_NIL,   /* 249: after comparatio in for cond */
    STATUS_GOTO_NIL,   /* 250: after aequalitas in for cond */
    STATUS_GOTO_NIL,   /* 251: after aequalitas in do-while cond */
    STATUS_GOTO_NIL,   /* 252: after aequalitas in switch cond */
    STATUS_GOTO_NIL,   /* 253: after coniunctio */
    STATUS_GOTO(254),  /* 254: after 'coniunctio &&' */
    STATUS_GOTO_NIL,   /* 255: after disiunctio (accept) */
    STATUS_GOTO(256),  /* 256: after 'disiunctio ||' */
    STATUS_GOTO_NIL,   /* 257: after aequalitas in && */
    STATUS_GOTO_NIL,   /* 258: after coniunctio && aequalitas */
    STATUS_GOTO_NIL,   /* 259: after aequalitas in || */
    STATUS_GOTO_NIL,   /* 260: after coniunctio in || */
    STATUS_GOTO(261),  /* 261: after && in || */
    STATUS_GOTO_NIL,   /* 262: after aequalitas in && within || */
    STATUS_GOTO_NIL,   /* 263: after coniunctio && aequalitas in || */
    STATUS_GOTO_NIL,   /* 264: after translatio */
    STATUS_GOTO(265),  /* 265: after translatio << or >> */
    STATUS_GOTO_NIL,   /* 266: after translatio << expr */
    STATUS_GOTO_NIL,   /* 267: after translatio in comp context */
    /* States 268-288: Bitwise operator states */
    STATUS_GOTO_NIL,   /* 268: after AMPERSAND_BITWISE top-level */
    STATUS_GOTO(269),  /* 269: after & bitwise top-level */
    STATUS_GOTO_NIL,   /* 270: after CARET_BITWISE top-level */
    STATUS_GOTO(271),  /* 271: after ^ bitwise top-level */
    STATUS_GOTO_NIL,   /* 272: after PIPA_BITWISE top-level */
    STATUS_GOTO(273),  /* 273: after | bitwise top-level */
    STATUS_GOTO_NIL,   /* 274: after aequalitas in & context */
    STATUS_GOTO_NIL,   /* 275: after aequalitas in ^ context */
    STATUS_GOTO_NIL,   /* 276: after ampersand_bitwise in ^ context */
    STATUS_GOTO_NIL,   /* 277: after aequalitas in | context */
    STATUS_GOTO_NIL,   /* 278: after ampersand_bitwise in | context */
    STATUS_GOTO_NIL,   /* 279: after caret_bitwise in | context */
    STATUS_GOTO(280),  /* 280: after & within ^ context */
    STATUS_GOTO(281),  /* 281: after & within | context */
    STATUS_GOTO(282),  /* 282: after ^ within | context */
    STATUS_GOTO_NIL,   /* 283: after aequalitas in & in ^ context */
    STATUS_GOTO_NIL,   /* 284: after aequalitas in & in | context */
    STATUS_GOTO_NIL,   /* 285: after aequalitas in ^ in | context */
    STATUS_GOTO_NIL,   /* 286: after ampersand_bitwise in ^ in | context */
    STATUS_GOTO(287),  /* 287: after & in ^ in | context */
    STATUS_GOTO_NIL,   /* 288: after aequalitas in & in ^ in | context */
    /* States 289-292: Unary ~ and ! operators */
    STATUS_GOTO(289),  /* 289: after ~ - FACTOR -> 290 */
    STATUS_GOTO_NIL,   /* 290: after ~ factor - reduce P106 */
    STATUS_GOTO(291),  /* 291: after ! - FACTOR -> 292 */
    STATUS_GOTO_NIL,   /* 292: after ! factor - reduce P107 */
    /* States 293-298: Assignment and Comma operators */
    STATUS_GOTO(293),  /* 293: after assignment op - expr -> assignatio */
    STATUS_GOTO_NIL,   /* 294: after disiunctio = assignatio - reduce P110 */
    STATUS_GOTO_NIL,   /* 295: after ASSIGNATIO - comma or reduce P109 */
    STATUS_GOTO_NIL,   /* 296: after VIRGA - ACCEPT */
    STATUS_GOTO(297),  /* 297: after comma - expr -> assignatio */
    STATUS_GOTO_NIL,   /* 298: after virga , assignatio - reduce P108 */
    STATUS_GOTO_NIL,   /* 299: after VIRGA in if condition */
    STATUS_GOTO_NIL,   /* 300: after VIRGA in while condition */
    STATUS_GOTO_NIL,   /* 301: after VIRGA in for-init */
    STATUS_GOTO_NIL,   /* 302: after VIRGA in for-condition */
    STATUS_GOTO_NIL,   /* 303: after VIRGA in for-increment */
    STATUS_GOTO_NIL,   /* 304: after VIRGA in do-while condition */
    STATUS_GOTO_NIL,   /* 305: after VIRGA in switch condition */
    /* Ternary operator states */
    STATUS_GOTO(306),  /* 306: after ? - expects true branch */
    STATUS_GOTO_NIL,   /* 307: after disiunctio ? ternarius - expects : */
    STATUS_GOTO(308),  /* 308: after : - expects false branch */
    STATUS_GOTO_NIL,   /* 309: after disiunctio ? ternarius : ternarius - reduce P123 */
    STATUS_GOTO_NIL,   /* 310: after TERNARIUS - assignment or reduce P121 */
    /* Postfix operator states */
    STATUS_GOTO_NIL,   /* 311: after POSTFIXUM - check postfix ops or reduce P129 */
    STATUS_GOTO(312),  /* 312: after postfixum '[' - subscript expression */
    STATUS_GOTO_NIL,   /* 313: after postfixum '[' expr - expects ']' */
    STATUS_GOTO_NIL,   /* 314: after postfixum '[' expr ']' - reduce P128 */
    /* Function call states */
    STATUS_GOTO(315),  /* 315: after postfixum '(' - expects args or ')' */
    STATUS_GOTO_NIL,   /* 316: after postfixum '(' argumenta - expects ')' or ',' */
    STATUS_GOTO(317),  /* 317: after postfixum '(' argumenta ',' - expects arg */
    STATUS_GOTO_NIL,   /* 318: after postfixum '(' ')' - reduce P130 */
    STATUS_GOTO_NIL,   /* 319: after postfixum '(' argumenta ')' - reduce P131 */
    STATUS_GOTO_NIL,   /* 320: after postfixum '(' argumenta ',' assignatio - reduce P133 */
    STATUS_GOTO_NIL,   /* 321: after postfixum '(' assignatio - reduce P132 */
    STATUS_GOTO_NIL,   /* 322: after postfixum '.' - expects member ID */
    STATUS_GOTO_NIL,   /* 323: after postfixum '.' ID - reduce P134 */
    STATUS_GOTO_NIL,   /* 324: after postfixum '->' - expects member ID */
    STATUS_GOTO_NIL,   /* 325: after postfixum '->' ID - reduce P135 */
    STATUS_GOTO_NIL,   /* 326: after postfixum '++' - reduce P136 */
    STATUS_GOTO_NIL,   /* 327: after postfixum '--' - reduce P137 */

    /* Pre-increment/decrement states */
    STATUS_GOTO(328),  /* 328: after '++' prefix - expects factor */
    STATUS_GOTO(329),  /* 329: after '--' prefix - expects factor */
    STATUS_GOTO_NIL,   /* 330: after '++ factor' - reduce P138 */
    STATUS_GOTO_NIL,   /* 331: after '-- factor' - reduce P139 */

    /* Additional literal states */
    STATUS_GOTO_NIL,   /* 332: after FLOAT_LIT - reduce P140 */
    STATUS_GOTO_NIL,   /* 333: after CHAR_LIT - reduce P141 */
    STATUS_GOTO_NIL,   /* 334: after STRING_LIT - reduce P142 */

    /* sizeof states */
    STATUS_GOTO(335),  /* 335: after 'sizeof' - expects factor */
    STATUS_GOTO_NIL    /* 336: after 'sizeof' factor - reduce P143 */
};


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
    constans Arbor2StatusInfo* info;
    i32 num_states;
    i32 i;

    resultus = xar_creare(glr->piscina, magnitudo(Arbor2TabulaActio*));

    num_states = (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));
    si (status < ZEPHYRUM || status >= num_states)
    {
        redde resultus;  /* Empty = error */
    }

    info = &STATUS_TABULA_PARTIAL[status];
    si (info->actiones == NIHIL)
    {
        redde resultus;
    }

    per (i = ZEPHYRUM; i < (i32)info->numerus; i++)
    {
        si (info->actiones[i].lexema == lexema)
        {
            /* Union cast to preserve const without compiler warning */
            unio { constans Arbor2TabulaActio* c; Arbor2TabulaActio* m; } u;
            Arbor2TabulaActio** slot;

            u.c = &info->actiones[i];
            slot = xar_addere(resultus);
            *slot = u.m;
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
        casus ARBOR2_NT_PIPA_BITWISE:
            nt_int = INT_NT_PIPA_BITWISE;
            frange;
        casus ARBOR2_NT_CARET_BITWISE:
            nt_int = INT_NT_CARET_BITWISE;
            frange;
        casus ARBOR2_NT_AMPERSAND_BITWISE:
            nt_int = INT_NT_AMPERSAND_BITWISE;
            frange;
        casus ARBOR2_NT_VIRGA:
            nt_int = INT_NT_VIRGA;
            frange;
        casus ARBOR2_NT_ASSIGNATIO:
            nt_int = INT_NT_ASSIGNATIO;
            frange;
        casus ARBOR2_NT_TERNARIUS:
            nt_int = INT_NT_TERNARIUS;
            frange;
        casus ARBOR2_NT_POSTFIXUM:
            nt_int = INT_NT_POSTFIXUM;
            frange;
        casus ARBOR2_NT_ARGUMENTA:
            nt_int = INT_NT_ARGUMENTA;
            frange;
        ordinarius:
            nt_int = -I;
            frange;
    }

    si (nt_int < ZEPHYRUM)
    {
        redde -I;  /* Error */
    }

    /* Use new per-state GOTO table */
    si (status >= ZEPHYRUM && status < (s32)(magnitudo(GOTO_TABULA_NOVA) / magnitudo(GOTO_TABULA_NOVA[0])))
    {
        constans Arbor2StatusGoto* sg = &GOTO_TABULA_NOVA[status];
        si (sg->transitus != NIHIL)
        {
            per (i = ZEPHYRUM; (s32)i < sg->numerus; i++)
            {
                si (sg->transitus[i].non_terminalis == nt_int)
                {
                    redde sg->transitus[i].status_novus;
                }
            }
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
    /* Old flat arrays removed - use STATUS_TABULA_PARTIAL and GOTO_TABULA_NOVA */
    glr->actiones = NIHIL;
    glr->num_actiones = ZEPHYRUM;
    glr->goto_tabula = NIHIL;
    glr->num_goto = ZEPHYRUM;
    glr->regulae = REGULAE;
    glr->num_regulae = NUM_REGULAE;
}

/* ==================================================
 * Table Validation
 *
 * Verifies table integrity:
 * - All SHIFT targets point to valid states
 * - All REDUCE targets point to valid rules
 * - All GOTO targets point to valid states
 * ================================================== */

b32
arbor2_glr_validare_tabulas(vacuum)
{
    i32 status;
    i32 j;
    b32 valida = VERUM;
    i32 num_action_states = (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));
    i32 num_goto_states = (i32)(magnitudo(GOTO_TABULA_NOVA) / magnitudo(GOTO_TABULA_NOVA[0]));

    /* Validate action table entries */
    per (status = ZEPHYRUM; status < num_action_states; status++)
    {
        constans Arbor2StatusInfo* info = &STATUS_TABULA_PARTIAL[status];

        si (info->actiones == NIHIL)
        {
            /* State has no actions - this is a bug */
            imprimere("VALIDATIO: status %d habet actiones NIHIL\n", status);
            valida = FALSUM;
            perge;
        }

        per (j = ZEPHYRUM; (s32)j < info->numerus; j++)
        {
            constans Arbor2TabulaActio* actio = &info->actiones[j];

            si (actio->actio == ARBOR2_ACTIO_SHIFT)
            {
                /* SHIFT target must be valid state */
                si (actio->valor < ZEPHYRUM || actio->valor >= num_goto_states)
                {
                    imprimere("VALIDATIO: status %d SHIFT ad status invalidum %d\n",
                              status, actio->valor);
                    valida = FALSUM;
                }
            }
            alioquin si (actio->actio == ARBOR2_ACTIO_REDUCE)
            {
                /* REDUCE target must be valid rule */
                si (actio->valor < ZEPHYRUM || actio->valor >= NUM_REGULAE)
                {
                    imprimere("VALIDATIO: status %d REDUCE ad regulam invalidam %d\n",
                              status, actio->valor);
                    valida = FALSUM;
                }
            }
        }
    }

    /* Validate GOTO table entries */
    per (status = ZEPHYRUM; status < num_goto_states; status++)
    {
        constans Arbor2StatusGoto* sg = &GOTO_TABULA_NOVA[status];

        si (sg->transitus == NIHIL)
        {
            /* State has no GOTO entries - this is normal */
            perge;
        }

        per (j = ZEPHYRUM; (s32)j < sg->numerus; j++)
        {
            s32 target = sg->transitus[j].status_novus;

            si (target < ZEPHYRUM || target >= (s32)num_goto_states)
            {
                imprimere("VALIDATIO: status %d GOTO ad status invalidum %d\n",
                          status, target);
                valida = FALSUM;
            }
        }
    }

    si (valida)
    {
        imprimere("VALIDATIO: Omnes tabulae validae sunt (%d actio, %d goto).\n",
                  num_action_states, num_goto_states);
    }

    redde valida;
}

/* ==================================================
 * Table Query API (no GLR instance required)
 * ================================================== */

i32
arbor2_tabula_numerus_statuum(vacuum)
{
    redde (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));
}

i32
arbor2_tabula_numerus_regularum(vacuum)
{
    redde NUM_REGULAE;
}

/* Old iterator functions removed - use STATUS_TABULA_PARTIAL directly */

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
        casus ARBOR2_NT_POSTFIXUM:      redde "POSTFIXUM";
        casus ARBOR2_NT_ARGUMENTA:      redde "ARGUMENTA";
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
        casus ARBOR2_NT_PIPA_BITWISE:   redde "PIPA_BITWISE";
        casus ARBOR2_NT_CARET_BITWISE:  redde "CARET_BITWISE";
        casus ARBOR2_NT_AMPERSAND_BITWISE: redde "AMPERSAND_BITWISE";
        casus ARBOR2_NT_TRANSLATIO:     redde "TRANSLATIO";
        casus ARBOR2_NT_VIRGA:          redde "VIRGA";
        casus ARBOR2_NT_ASSIGNATIO:     redde "ASSIGNATIO";
        casus ARBOR2_NT_TERNARIUS:      redde "TERNARIUS";
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
        casus ARBOR2_NODUS_FLOAT:         redde "FLOAT";
        casus ARBOR2_NODUS_CHAR:          redde "CHAR";
        casus ARBOR2_NODUS_STRING:        redde "STRING";
        casus ARBOR2_NODUS_BINARIUM:      redde "BINARIUM";
        casus ARBOR2_NODUS_UNARIUM:       redde "UNARIUM";
        casus ARBOR2_NODUS_CONVERSIO:     redde "CONVERSIO";
        casus ARBOR2_NODUS_SIZEOF:        redde "SIZEOF";
        casus ARBOR2_NODUS_VOCATIO:       redde "VOCATIO";
        casus ARBOR2_NODUS_SUBSCRIPTIO:   redde "SUBSCRIPTIO";
        casus ARBOR2_NODUS_MEMBRUM:       redde "MEMBRUM";
        casus ARBOR2_NODUS_POST_UNARIUM:  redde "POST_UNARIUM";
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
        casus ARBOR2_NODUS_TERNARIUS:     redde "TERNARIUS";
        casus ARBOR2_NODUS_AMBIGUUS:      redde "AMBIGUUS";
        casus ARBOR2_NODUS_STRUCT_SPECIFIER: redde "STRUCT_SPECIFIER";
        casus ARBOR2_NODUS_ERROR:         redde "ERROR";
        ordinarius:                       redde "IGNOTUM";
    }
}

/*
 * Obtinere list of expected tokens for a state.
 * Returns comma-separated string of token names, or NIHIL if invalid state.
 */
chorda*
arbor2_glr_exspectata_pro_statu(Piscina* piscina, s32 status)
{
    constans Arbor2StatusInfo* info;
    character* buffer;
    i32 buffer_size;
    i32 offset;
    i32 i;
    b32 first;
    chorda* result;
    i32 num_states;

    num_states = (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));

    si (status < ZEPHYRUM || status >= (s32)num_states)
    {
        redde NIHIL;
    }

    info = &STATUS_TABULA_PARTIAL[status];
    si (info->actiones == NIHIL || info->numerus == ZEPHYRUM)
    {
        redde NIHIL;
    }

    /* Allocate buffer for result (generous size) */
    buffer_size = 1024;
    buffer = piscina_allocare(piscina, (memoriae_index)buffer_size);
    si (buffer == NIHIL)
    {
        redde NIHIL;
    }

    offset = ZEPHYRUM;
    first = VERUM;

    per (i = ZEPHYRUM; (s32)i < info->numerus && offset < buffer_size - 32; i++)
    {
        constans Arbor2TabulaActio* actio = &info->actiones[i];

        /* Skip ERROR actions */
        si (actio->actio == ARBOR2_ACTIO_ERROR)
        {
            perge;
        }

        /* Get token name */
        {
            constans character* token_nomen = arbor2_lexema_genus_nomen(actio->lexema);

            /* Add separator */
            si (!first)
            {
                buffer[offset++] = ',';
                buffer[offset++] = ' ';
            }
            first = FALSUM;

            /* Copy token name */
            dum (*token_nomen != '\0' && offset < buffer_size - I)
            {
                buffer[offset++] = *token_nomen++;
            }
        }
    }

    buffer[offset] = '\0';

    /* Create chorda result */
    result = piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
    si (result != NIHIL)
    {
        result->datum = (i8*)buffer;
        result->mensura = offset;
    }

    redde result;
}

/* ==================================================
 * State Tag Validation System
 *
 * Defines tags for state categories and validates that
 * tagged states have the required tokens.
 * ================================================== */

/* Required tokens for EXPR_INITIUM states (can start expressions) */
hic_manens constans Arbor2LexemaGenus EXPR_INITIUM_LEXEMATA[] = {
    ARBOR2_LEXEMA_IDENTIFICATOR,
    ARBOR2_LEXEMA_INTEGER,
    ARBOR2_LEXEMA_PAREN_APERTA,
    ARBOR2_LEXEMA_ASTERISCUS,
    ARBOR2_LEXEMA_AMPERSAND,
    ARBOR2_LEXEMA_TILDE,
    ARBOR2_LEXEMA_EXCLAMATIO
};

/* Required tokens for CONDITIO_CONTEXTUS states (condition expressions) */
hic_manens constans Arbor2LexemaGenus CONDITIO_LEXEMATA[] = {
    ARBOR2_LEXEMA_DUAMPERSAND,
    ARBOR2_LEXEMA_DUPIPA,
    ARBOR2_LEXEMA_QUAESTIO,
    ARBOR2_LEXEMA_AMPERSAND,
    ARBOR2_LEXEMA_PIPA,
    ARBOR2_LEXEMA_CARET
};

/* Required tokens for SENTENTIA_INITIUM states (statement starters) */
hic_manens constans Arbor2LexemaGenus SENTENTIA_INITIUM_LEXEMATA[] = {
    /* Expression starters */
    ARBOR2_LEXEMA_IDENTIFICATOR,
    ARBOR2_LEXEMA_INTEGER,
    ARBOR2_LEXEMA_PAREN_APERTA,
    ARBOR2_LEXEMA_ASTERISCUS,
    ARBOR2_LEXEMA_AMPERSAND,
    ARBOR2_LEXEMA_TILDE,
    ARBOR2_LEXEMA_EXCLAMATIO,
    /* Statement keywords */
    ARBOR2_LEXEMA_IF,
    ARBOR2_LEXEMA_WHILE,
    ARBOR2_LEXEMA_DO,
    ARBOR2_LEXEMA_FOR,
    ARBOR2_LEXEMA_SWITCH,
    ARBOR2_LEXEMA_RETURN,
    ARBOR2_LEXEMA_BREAK,
    ARBOR2_LEXEMA_CONTINUE,
    ARBOR2_LEXEMA_GOTO,
    ARBOR2_LEXEMA_CASE,
    ARBOR2_LEXEMA_DEFAULT,
    /* Empty and compound */
    ARBOR2_LEXEMA_SEMICOLON,
    ARBOR2_LEXEMA_BRACE_APERTA
};

/* Tag rule structure */
nomen structura {
    i32                         signum;        /* Tag bit flag */
    constans character*         descriptio;    /* Human-readable name */
    constans Arbor2LexemaGenus* lexemata;      /* Required tokens */
    i32                         num_lexemata;  /* Number of required tokens */
} Arbor2TagRegula;

hic_manens constans Arbor2TagRegula TAG_REGULAE[] = {
    {
        ARBOR2_TAG_EXPR_INITIUM,
        "EXPR_INITIUM",
        EXPR_INITIUM_LEXEMATA,
        (i32)(magnitudo(EXPR_INITIUM_LEXEMATA) / magnitudo(EXPR_INITIUM_LEXEMATA[0]))
    },
    {
        ARBOR2_TAG_CONDITIO_CONTEXTUS,
        "CONDITIO_CONTEXTUS",
        CONDITIO_LEXEMATA,
        (i32)(magnitudo(CONDITIO_LEXEMATA) / magnitudo(CONDITIO_LEXEMATA[0]))
    },
    {
        ARBOR2_TAG_SENTENTIA_INITIUM,
        "SENTENTIA_INITIUM",
        SENTENTIA_INITIUM_LEXEMATA,
        (i32)(magnitudo(SENTENTIA_INITIUM_LEXEMATA) / magnitudo(SENTENTIA_INITIUM_LEXEMATA[0]))
    }
};

#define NUM_TAG_REGULAE (i32)(magnitudo(TAG_REGULAE) / magnitudo(TAG_REGULAE[0]))

/* State tags array - index corresponds to state number.
 * Zero means no tags. Multiple tags can be combined with |. */
hic_manens constans i32 STATUS_TAGS[] = {
    /* 0 */   ARBOR2_TAG_EXPR_INITIUM,  /* initial - expression start */
    /* 1 */   0,
    /* 2 */   0,
    /* 3 */   0,
    /* 4 */   0,
    /* 5 */   0,
    /* 6 */   ARBOR2_TAG_EXPR_INITIUM,  /* after '(' */
    /* 7 */   ARBOR2_TAG_EXPR_INITIUM,  /* after unary '*' */
    /* 8 */   ARBOR2_TAG_EXPR_INITIUM,  /* after unary '&' */
    /* 9 */   ARBOR2_TAG_EXPR_INITIUM,  /* after '+' or '-' */
    /* 10 */  ARBOR2_TAG_EXPR_INITIUM,  /* after '*' or '/' */
    /* 11 */  0, /* 12 */ 0, /* 13 */ 0, /* 14 */ 0, /* 15 */ 0,
    /* 16 */  0, /* 17 */ 0, /* 18 */ 0, /* 19 */ 0, /* 20 */ 0,
    /* 21 */  0, /* 22 */ 0, /* 23 */ 0, /* 24 */ 0, /* 25 */ 0,
    /* 26 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* inside compound */
    /* 27 */  0, /* 28 */ 0, /* 29 */ 0, /* 30 */ 0,
    /* 31 */  ARBOR2_TAG_EXPR_INITIUM,  /* if condition - entry state */
    /* 32 */  0,
    /* 33 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* after 'if (expr)' - if body */
    /* 34 */  0,
    /* 35 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* else branch */
    /* 36 */  0, /* 37 */ 0, /* 38 */ 0, /* 39 */ 0,
    /* 40 */  ARBOR2_TAG_EXPR_INITIUM,  /* while condition - entry state */
    /* 41 */  0,
    /* 42 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* while body */
    /* 43 */  0, /* 44 */ 0,
    /* 45 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* do body */
    /* 46 */  0, /* 47 */ 0,
    /* 48 */  ARBOR2_TAG_EXPR_INITIUM,  /* do-while condition - entry state */
    /* 49 */  ARBOR2_TAG_CONDITIO_CONTEXTUS,  /* after do-while condition expression */
    /* 50 */  0, /* 51 */ 0, /* 52 */ 0, /* 53 */ 0,
    /* 54 */  ARBOR2_TAG_EXPR_INITIUM,  /* for init */
    /* 55 */  0, /* 56 */ 0,
    /* 57 */  ARBOR2_TAG_EXPR_INITIUM,  /* for condition - entry state */
    /* 58 */  0, /* 59 */ 0,
    /* 60 */  ARBOR2_TAG_EXPR_INITIUM,  /* for increment */
    /* 61 */  0, /* 62 */ 0,
    /* 63 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* for body */
    /* 64 */  0, /* 65 */ 0, /* 66 */ 0, /* 67 */ 0, /* 68 */ 0, /* 69 */ 0,
    /* 70 */  ARBOR2_TAG_EXPR_INITIUM,  /* return expr */
    /* 71 */  0, /* 72 */ 0, /* 73 */ 0, /* 74 */ 0, /* 75 */ 0, /* 76 */ 0,
    /* 77 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* labeled statement */
    /* 78 */  0, /* 79 */ 0,
    /* 80 */  ARBOR2_TAG_EXPR_INITIUM,  /* switch condition - entry state */
    /* 81 */  0,
    /* 82 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* switch body */
    /* 83 */  0,
    /* 84 */  ARBOR2_TAG_EXPR_INITIUM,  /* case expression */
    /* 85 */  0,
    /* 86 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* case body */
    /* 87 */  0, /* 88 */ 0,
    /* 89 */  ARBOR2_TAG_SENTENTIA_INITIUM,  /* default body */
    /* Rest are untagged for now - add as needed */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 90-99 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 100-109 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 110-119 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 120-129 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 130-139 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 140-149 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 150-159 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 160-169 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 170-179 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 180-189 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 190-199 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 200-209 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 210-219 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 220-229 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  /* 230-239 */
    0,  /* 240: after aequalitas - no tags yet */
    ARBOR2_TAG_EXPR_INITIUM,  /* 241: after comparison op */
    ARBOR2_TAG_EXPR_INITIUM,  /* 242: after equality op */
    0, 0, 0,
    ARBOR2_TAG_CONDITIO_CONTEXTUS,  /* 246: after comparatio in condition */
    0, 0, 0, 0, 0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 254: after && */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 256: after || */
    0,  /* 257: after && RHS aequalitas - reduce state */
    0,  /* 258: after && RHS coniunctio - reduce state */
    0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 261: after && in || */
    0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 265: after << or >> */
    0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 269: after & bitwise */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 271: after ^ bitwise */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 273: after | bitwise */
    0, 0, 0, 0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 280: after & in ^ */
    ARBOR2_TAG_EXPR_INITIUM,  /* 281: after & in | */
    ARBOR2_TAG_EXPR_INITIUM,  /* 282: after ^ in | */
    0, 0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 287: after & in ^ in | */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 289: after ~ */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 291: after ! */
    0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 293: after assignment op */
    0, 0, 0,
    ARBOR2_TAG_EXPR_INITIUM,  /* 297: after comma */
    0, 0, 0, 0, 0, 0, 0, 0,  /* 298-305 */
    ARBOR2_TAG_EXPR_INITIUM,  /* 306: after ? */
    0,  /* 307: after ? ternarius - expects : */
    ARBOR2_TAG_EXPR_INITIUM,  /* 308: after : */
    0,  /* 309: reduce state */
    0   /* 310: after TERNARIUS */
};

#define NUM_STATUS_TAGS (i32)(magnitudo(STATUS_TAGS) / magnitudo(STATUS_TAGS[0]))

/* Helper: check if a state has an action for a token */
hic_manens b32
status_habet_lexema(i32 status, Arbor2LexemaGenus lexema)
{
    constans Arbor2StatusInfo* info;
    i32 i;
    i32 num_states;

    num_states = (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));

    si (status < ZEPHYRUM || status >= num_states)
    {
        redde FALSUM;
    }

    info = &STATUS_TABULA_PARTIAL[status];
    si (info->actiones == NIHIL)
    {
        redde FALSUM;
    }

    per (i = ZEPHYRUM; i < (i32)info->numerus; i++)
    {
        si (info->actiones[i].lexema == lexema)
        {
            redde VERUM;
        }
    }

    redde FALSUM;
}

b32
arbor2_glr_validare_tags(vacuum)
{
    i32 status;
    i32 r;
    i32 t;
    b32 valida = VERUM;
    i32 num_states;
    i32 num_tagged = ZEPHYRUM;
    i32 num_missing = ZEPHYRUM;

    num_states = (i32)(magnitudo(STATUS_TABULA_PARTIAL) / magnitudo(STATUS_TABULA_PARTIAL[0]));

    /* Check each state */
    per (status = ZEPHYRUM; status < num_states && status < NUM_STATUS_TAGS; status++)
    {
        i32 tags = STATUS_TAGS[status];

        si (tags == ZEPHYRUM)
        {
            perge;  /* No tags for this state */
        }

        num_tagged++;

        /* Check each tag rule */
        per (r = ZEPHYRUM; r < NUM_TAG_REGULAE; r++)
        {
            constans Arbor2TagRegula* regula = &TAG_REGULAE[r];

            /* Does this state have this tag? */
            si ((tags & regula->signum) == ZEPHYRUM)
            {
                perge;
            }

            /* Check all required tokens */
            per (t = ZEPHYRUM; t < regula->num_lexemata; t++)
            {
                Arbor2LexemaGenus lexema = regula->lexemata[t];

                si (!status_habet_lexema(status, lexema))
                {
                    imprimere("TAG VALIDATIO: Status %d (%s) caret %s\n",
                              status,
                              regula->descriptio,
                              arbor2_lexema_genus_nomen(lexema));
                    valida = FALSUM;
                    num_missing++;
                }
            }
        }
    }

    si (valida)
    {
        imprimere("TAG VALIDATIO: Omnes %d status cum signis habent omnia lexemata necessaria.\n",
                  num_tagged);
    }
    alioquin
    {
        imprimere("TAG VALIDATIO: %d lexemata desunt in %d statusibus cum signis.\n",
                  num_missing, num_tagged);
    }

    redde valida;
}
