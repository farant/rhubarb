#include "arbor_formator.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "arbor_lexema.h"
#include <string.h>
#include <stdio.h>

/* ===========================================================
 * KEYWORD TABULA - Latinum <-> Anglicum
 * =========================================================== */

interior constans structura {
    constans character* latinum;
    constans character* anglicum;
} keyword_tabula[] = {
    { "si",         "if" },
    { "alioquin",   "else" },
    { "commutatio", "switch" },
    { "casus",      "case" },
    { "ordinarius", "default" },
    { "per",        "for" },
    { "dum",        "while" },
    { "fac",        "do" },
    { "frange",     "break" },
    { "perge",      "continue" },
    { "salta",      "goto" },
    { "redde",      "return" },
    { "structura",  "struct" },
    { "unio",       "union" },
    { "enumeratio", "enum" },
    { "nomen",      "typedef" },
    { "vacuum",     "void" },
    { "constans",   "const" },
    { "volatilis",  "volatile" },
    { "staticus",   "static" },
    { "externus",   "extern" },
    { "signatus",   "signed" },
    { "insignatus", "unsigned" },
    { "magnitudo",  "sizeof" },
    { "character",  "char" },
    { "brevis",     "short" },
    { "integer",    "int" },
    { "longus",     "long" },
    { "fluitans",   "float" },
    { "duplex",     "double" },
    { "sponte",     "auto" },
    { "registrum",  "register" },
    { NIHIL, NIHIL }
};

/* ===========================================================
 * FORMATOR STATUS
 * =========================================================== */

nomen structura {
                   Piscina* piscina;
        ChordaAedificator*  aedificator;
    ArborFormatorOptiones*  optiones;
                       b32  fidelis;
                       i32  indentatio;
} FormatorStatus;

/* ===========================================================
 * ADIUTORES INTERNI - TRIVIA
 * =========================================================== */

interior vacuum
_emittere_trivia (
    FormatorStatus* status,
              Xar*  trivia)
{
    i32 i;
    i32 num;

    si (trivia == NIHIL) redde;

    num = xar_numerus(trivia);

    per (i = ZEPHYRUM; i < num; i++)
    {
        ArborTrivia** tp = xar_obtinere(trivia, i);
        si (tp != NIHIL && *tp != NIHIL)
        {
            ArborTrivia* t = *tp;
            chorda_aedificator_appendere_chorda(status->aedificator, t->valor);
        }
    }
}

/* ===========================================================
 * ADIUTORES INTERNI - LEXEMA GENUS AD CHORDA
 * =========================================================== */

interior constans character*
_lexema_genus_ad_symbolum (
    ArborLexemaGenus genus)
{
    commutatio (genus)
    {
    /* Punctuation */
    casus ARBOR_LEXEMA_SEMICOLON:      redde ";";
    casus ARBOR_LEXEMA_COMMA:          redde ",";
    casus ARBOR_LEXEMA_COLON:          redde ":";
    casus ARBOR_LEXEMA_QUAESTIO:       redde "?";
    casus ARBOR_LEXEMA_PAREN_APERTA:   redde "(";
    casus ARBOR_LEXEMA_PAREN_CLAUSA:   redde ")";
    casus ARBOR_LEXEMA_BRACKET_APERTA: redde "[";
    casus ARBOR_LEXEMA_BRACKET_CLAUSA: redde "]";
    casus ARBOR_LEXEMA_BRACE_APERTA:   redde "{";
    casus ARBOR_LEXEMA_BRACE_CLAUSA:   redde "}";

    /* Operatores arithmetici */
    casus ARBOR_LEXEMA_PLUS:           redde "+";
    casus ARBOR_LEXEMA_MINUS:          redde "-";
    casus ARBOR_LEXEMA_ASTERISCUS:     redde "*";
    casus ARBOR_LEXEMA_SOLIDUS:        redde "/";
    casus ARBOR_LEXEMA_PERCENTUM:      redde "%";

    /* Operatores bitwise */
    casus ARBOR_LEXEMA_AMPERSAND:      redde "&";
    casus ARBOR_LEXEMA_PIPA:           redde "|";
    casus ARBOR_LEXEMA_CARET:          redde "^";
    casus ARBOR_LEXEMA_TILDE:          redde "~";

    /* Operatores logici */
    casus ARBOR_LEXEMA_EXCLAMATIO:     redde "!";
    casus ARBOR_LEXEMA_DUAMPERSAND:    redde "&&";
    casus ARBOR_LEXEMA_DUPIPA:         redde "||";

    /* Operatores comparationis */
    casus ARBOR_LEXEMA_AEQUALIS:       redde "==";
    casus ARBOR_LEXEMA_NON_AEQUALIS:   redde "!=";
    casus ARBOR_LEXEMA_MINOR:          redde "<";
    casus ARBOR_LEXEMA_MAIOR:          redde ">";
    casus ARBOR_LEXEMA_MINOR_AEQ:      redde "<=";
    casus ARBOR_LEXEMA_MAIOR_AEQ:      redde ">=";

    /* Operatores shift */
    casus ARBOR_LEXEMA_SINISTRUM:      redde "<<";
    casus ARBOR_LEXEMA_DEXTRUM:        redde ">>";

    /* Assignatio */
    casus ARBOR_LEXEMA_ASSIGNATIO:     redde "=";
    casus ARBOR_LEXEMA_PLUS_ASSIGN:    redde "+=";
    casus ARBOR_LEXEMA_MINUS_ASSIGN:   redde "-=";
    casus ARBOR_LEXEMA_MULT_ASSIGN:    redde "*=";
    casus ARBOR_LEXEMA_DIV_ASSIGN:     redde "/=";
    casus ARBOR_LEXEMA_MOD_ASSIGN:     redde "%=";
    casus ARBOR_LEXEMA_AND_ASSIGN:     redde "&=";
    casus ARBOR_LEXEMA_OR_ASSIGN:      redde "|=";
    casus ARBOR_LEXEMA_XOR_ASSIGN:     redde "^=";
    casus ARBOR_LEXEMA_SHL_ASSIGN:     redde "<<=";
    casus ARBOR_LEXEMA_SHR_ASSIGN:     redde ">>=";

    /* Incrementum/Decrementum */
    casus ARBOR_LEXEMA_DUPLUS:         redde "++";
    casus ARBOR_LEXEMA_DUMINUS:        redde "--";

    /* Membrum accessus */
    casus ARBOR_LEXEMA_PUNCTUM:        redde ".";
    casus ARBOR_LEXEMA_SAGITTA:        redde "->";

    /* Keywords */
    casus ARBOR_LEXEMA_IF:             redde "if";
    casus ARBOR_LEXEMA_ELSE:           redde "else";
    casus ARBOR_LEXEMA_SWITCH:         redde "switch";
    casus ARBOR_LEXEMA_CASE:           redde "case";
    casus ARBOR_LEXEMA_DEFAULT:        redde "default";
    casus ARBOR_LEXEMA_FOR:            redde "for";
    casus ARBOR_LEXEMA_WHILE:          redde "while";
    casus ARBOR_LEXEMA_DO:             redde "do";
    casus ARBOR_LEXEMA_BREAK:          redde "break";
    casus ARBOR_LEXEMA_CONTINUE:       redde "continue";
    casus ARBOR_LEXEMA_GOTO:           redde "goto";
    casus ARBOR_LEXEMA_RETURN:         redde "return";
    casus ARBOR_LEXEMA_STRUCT:         redde "struct";
    casus ARBOR_LEXEMA_UNION:          redde "union";
    casus ARBOR_LEXEMA_ENUM:           redde "enum";
    casus ARBOR_LEXEMA_TYPEDEF:        redde "typedef";
    casus ARBOR_LEXEMA_VOID:           redde "void";
    casus ARBOR_LEXEMA_CHAR:           redde "char";
    casus ARBOR_LEXEMA_SHORT:          redde "short";
    casus ARBOR_LEXEMA_INT:            redde "int";
    casus ARBOR_LEXEMA_LONG:           redde "long";
    casus ARBOR_LEXEMA_FLOAT:          redde "float";
    casus ARBOR_LEXEMA_DOUBLE:         redde "double";
    casus ARBOR_LEXEMA_SIGNED:         redde "signed";
    casus ARBOR_LEXEMA_UNSIGNED:       redde "unsigned";
    casus ARBOR_LEXEMA_CONST:          redde "const";
    casus ARBOR_LEXEMA_VOLATILE:       redde "volatile";
    casus ARBOR_LEXEMA_AUTO:           redde "auto";
    casus ARBOR_LEXEMA_REGISTER:       redde "register";
    casus ARBOR_LEXEMA_STATIC:         redde "static";
    casus ARBOR_LEXEMA_EXTERN:         redde "extern";
    casus ARBOR_LEXEMA_SIZEOF:         redde "sizeof";

    ordinarius:
        redde "";
    }
}

/* ===========================================================
 * PROTOTYPA - EMITTERE NODUM
 * =========================================================== */

interior vacuum _emittere_nodum_fidelis(FormatorStatus* status, ArborNodus* nodus);

/* ===========================================================
 * EMITTERE NODUM - FIDELIS MODE
 *
 * In fidelis mode, emittimus trivia exacte ut est.
 * Hoc est simplicior modus - solum iterare et emittere.
 * =========================================================== */

interior vacuum
_emittere_nodum_fidelis (
    FormatorStatus* status,
        ArborNodus* nodus)
{
    i32 i;
    i32 num;

    si (nodus == NIHIL) redde;

    /* Emittere trivia ante nodum */
    _emittere_trivia(status, nodus->trivia_ante);

    commutatio (nodus->genus)
    {
    /* ===== TRANSLATION UNIT ===== */
    casus ARBOR_NODUS_TRANSLATION_UNIT:
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== FUNCTION DEFINITION ===== */
    casus ARBOR_NODUS_FUNCTION_DEFINITION:
        /* Specifiers */
        si (nodus->datum.functio.specifiers)
        {
            num = xar_numerus(nodus->datum.functio.specifiers);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.functio.specifiers, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        /* Declarator */
        _emittere_nodum_fidelis(status, nodus->datum.functio.declarator);
        /* Old-style params */
        si (nodus->datum.functio.old_params)
        {
            num = xar_numerus(nodus->datum.functio.old_params);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.functio.old_params, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        /* Body */
        _emittere_nodum_fidelis(status, nodus->datum.functio.corpus);
        frange;

    /* ===== DECLARATION ===== */
    casus ARBOR_NODUS_DECLARATION:
        /* Specifiers */
        si (nodus->datum.declaratio.specifiers)
        {
            num = xar_numerus(nodus->datum.declaratio.specifiers);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.declaratio.specifiers, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        /* Declarators */
        si (nodus->datum.declaratio.declaratores)
        {
            num = xar_numerus(nodus->datum.declaratio.declaratores);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.declaratio.declaratores, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== COMPOUND STATEMENT ===== */
    casus ARBOR_NODUS_COMPOUND_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "{");
        si (nodus->datum.compositum.sententiae)
        {
            num = xar_numerus(nodus->datum.compositum.sententiae);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.compositum.sententiae, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        chorda_aedificator_appendere_literis(status->aedificator, "}");
        frange;

    /* ===== IF STATEMENT ===== */
    casus ARBOR_NODUS_IF_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "if");
        /* NON emittere () - trivia conditionis iam continet punctuationem */
        _emittere_nodum_fidelis(status, nodus->datum.conditionale.conditio);
        _emittere_nodum_fidelis(status, nodus->datum.conditionale.consequens);
        si (nodus->datum.conditionale.alternans)
        {
            chorda_aedificator_appendere_literis(status->aedificator, "else");
            _emittere_nodum_fidelis(status, nodus->datum.conditionale.alternans);
        }
        frange;

    /* ===== WHILE STATEMENT ===== */
    casus ARBOR_NODUS_WHILE_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "while");
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        _emittere_nodum_fidelis(status, nodus->datum.iteratio.conditio);
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        _emittere_nodum_fidelis(status, nodus->datum.iteratio.corpus);
        frange;

    /* ===== DO STATEMENT ===== */
    casus ARBOR_NODUS_DO_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "do");
        _emittere_nodum_fidelis(status, nodus->datum.iteratio.corpus);
        chorda_aedificator_appendere_literis(status->aedificator, "while");
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        _emittere_nodum_fidelis(status, nodus->datum.iteratio.conditio);
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    /* ===== FOR STATEMENT ===== */
    casus ARBOR_NODUS_FOR_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "for");
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        si (nodus->datum.circuitus.init)
            _emittere_nodum_fidelis(status, nodus->datum.circuitus.init);
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        si (nodus->datum.circuitus.conditio)
            _emittere_nodum_fidelis(status, nodus->datum.circuitus.conditio);
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        si (nodus->datum.circuitus.post)
            _emittere_nodum_fidelis(status, nodus->datum.circuitus.post);
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        _emittere_nodum_fidelis(status, nodus->datum.circuitus.corpus);
        frange;

    /* ===== SWITCH STATEMENT ===== */
    casus ARBOR_NODUS_SWITCH_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "switch");
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        _emittere_nodum_fidelis(status, nodus->datum.selectio.conditio);
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        _emittere_nodum_fidelis(status, nodus->datum.selectio.corpus);
        frange;

    /* ===== CASE LABEL ===== */
    casus ARBOR_NODUS_CASE_LABEL:
        chorda_aedificator_appendere_literis(status->aedificator, "case");
        _emittere_nodum_fidelis(status, nodus->datum.eventus.valor);
        chorda_aedificator_appendere_literis(status->aedificator, ":");
        si (nodus->datum.eventus.sententia)
            _emittere_nodum_fidelis(status, nodus->datum.eventus.sententia);
        frange;

    /* ===== DEFAULT LABEL ===== */
    casus ARBOR_NODUS_DEFAULT_LABEL:
        chorda_aedificator_appendere_literis(status->aedificator, "default");
        chorda_aedificator_appendere_literis(status->aedificator, ":");
        frange;

    /* ===== RETURN STATEMENT ===== */
    casus ARBOR_NODUS_RETURN_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "return");
        si (nodus->datum.reditio.valor)
        {
            chorda_aedificator_appendere_literis(status->aedificator, " ");
            _emittere_nodum_fidelis(status, nodus->datum.reditio.valor);
        }
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    /* ===== BREAK/CONTINUE/GOTO ===== */
    casus ARBOR_NODUS_BREAK_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "break");
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    casus ARBOR_NODUS_CONTINUE_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "continue");
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    casus ARBOR_NODUS_GOTO_STATEMENT:
        chorda_aedificator_appendere_literis(status->aedificator, "goto");
        chorda_aedificator_appendere_literis(status->aedificator, " ");
        si (nodus->datum.saltus.label)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.saltus.label);
        }
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    /* ===== LABELED STATEMENT ===== */
    casus ARBOR_NODUS_LABELED_STATEMENT:
        si (nodus->datum.titulatum.label)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.titulatum.label);
        }
        chorda_aedificator_appendere_literis(status->aedificator, ":");
        _emittere_nodum_fidelis(status, nodus->datum.titulatum.sententia);
        frange;

    /* ===== EXPRESSION STATEMENT ===== */
    casus ARBOR_NODUS_EXPRESSION_STATEMENT:
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        chorda_aedificator_appendere_literis(status->aedificator, ";");
        frange;

    /* ===== BINARY EXPRESSION ===== */
    casus ARBOR_NODUS_BINARY_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.binarium.sinister);
        chorda_aedificator_appendere_literis(status->aedificator,
            _lexema_genus_ad_symbolum(nodus->datum.binarium.operator));
        _emittere_nodum_fidelis(status, nodus->datum.binarium.dexter);
        frange;

    /* ===== UNARY EXPRESSION ===== */
    casus ARBOR_NODUS_UNARY_EXPRESSION:
        si (!nodus->datum.unarium.est_postfix)
        {
            chorda_aedificator_appendere_literis(status->aedificator,
                _lexema_genus_ad_symbolum(nodus->datum.unarium.operator));
        }
        _emittere_nodum_fidelis(status, nodus->datum.unarium.operandum);
        si (nodus->datum.unarium.est_postfix)
        {
            chorda_aedificator_appendere_literis(status->aedificator,
                _lexema_genus_ad_symbolum(nodus->datum.unarium.operator));
        }
        frange;

    /* ===== CALL EXPRESSION ===== */
    casus ARBOR_NODUS_CALL_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.vocatio.callee);
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        si (nodus->datum.vocatio.argumenta)
        {
            num = xar_numerus(nodus->datum.vocatio.argumenta);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np;
                si (i > ZEPHYRUM)
                    chorda_aedificator_appendere_literis(status->aedificator, ",");
                np = xar_obtinere(nodus->datum.vocatio.argumenta, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        frange;

    /* ===== MEMBER EXPRESSION ===== */
    casus ARBOR_NODUS_MEMBER_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.membrum.objectum);
        chorda_aedificator_appendere_literis(status->aedificator,
            _lexema_genus_ad_symbolum(nodus->datum.membrum.operator));
        si (nodus->datum.membrum.membrum)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.membrum.membrum);
        }
        frange;

    /* ===== SUBSCRIPT EXPRESSION ===== */
    casus ARBOR_NODUS_SUBSCRIPT_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.subscriptum.array);
        chorda_aedificator_appendere_literis(status->aedificator, "[");
        _emittere_nodum_fidelis(status, nodus->datum.subscriptum.index);
        chorda_aedificator_appendere_literis(status->aedificator, "]");
        frange;

    /* ===== CAST EXPRESSION ===== */
    casus ARBOR_NODUS_CAST_EXPRESSION:
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        _emittere_nodum_fidelis(status, nodus->datum.conversio.typus);
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        _emittere_nodum_fidelis(status, nodus->datum.conversio.expressio);
        frange;

    /* ===== SIZEOF EXPRESSION ===== */
    casus ARBOR_NODUS_SIZEOF_EXPRESSION:
        chorda_aedificator_appendere_literis(status->aedificator, "sizeof");
        si (nodus->datum.sizeof_expr.est_typus)
        {
            chorda_aedificator_appendere_literis(status->aedificator, "(");
            _emittere_nodum_fidelis(status, nodus->datum.sizeof_expr.operandum);
            chorda_aedificator_appendere_literis(status->aedificator, ")");
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(status->aedificator, " ");
            _emittere_nodum_fidelis(status, nodus->datum.sizeof_expr.operandum);
        }
        frange;

    /* ===== CONDITIONAL EXPRESSION ===== */
    casus ARBOR_NODUS_CONDITIONAL_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.ternarium.conditio);
        chorda_aedificator_appendere_literis(status->aedificator, "?");
        _emittere_nodum_fidelis(status, nodus->datum.ternarium.verum);
        chorda_aedificator_appendere_literis(status->aedificator, ":");
        _emittere_nodum_fidelis(status, nodus->datum.ternarium.falsum);
        frange;

    /* ===== ASSIGNMENT EXPRESSION ===== */
    casus ARBOR_NODUS_ASSIGNMENT_EXPRESSION:
        _emittere_nodum_fidelis(status, nodus->datum.assignatio.target);
        chorda_aedificator_appendere_literis(status->aedificator,
            _lexema_genus_ad_symbolum(nodus->datum.assignatio.operator));
        _emittere_nodum_fidelis(status, nodus->datum.assignatio.valor);
        frange;

    /* ===== COMMA EXPRESSION ===== */
    casus ARBOR_NODUS_COMMA_EXPRESSION:
        si (nodus->datum.comma.expressiones)
        {
            num = xar_numerus(nodus->datum.comma.expressiones);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np;
                si (i > ZEPHYRUM)
                    chorda_aedificator_appendere_literis(status->aedificator, ",");
                np = xar_obtinere(nodus->datum.comma.expressiones, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== IDENTIFIER ===== */
    casus ARBOR_NODUS_IDENTIFIER:
        si (nodus->datum.folium.valor)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.folium.valor);
        }
        frange;

    /* ===== LITERALS ===== */
    casus ARBOR_NODUS_INTEGER_LITERAL:
        si (nodus->datum.numerus.textus)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.numerus.textus);
        }
        frange;

    casus ARBOR_NODUS_FLOAT_LITERAL:
        si (nodus->datum.floating.textus)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.floating.textus);
        }
        frange;

    casus ARBOR_NODUS_CHAR_LITERAL:
        si (nodus->datum.character_lit.textus)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.character_lit.textus);
        }
        frange;

    casus ARBOR_NODUS_STRING_LITERAL:
        si (nodus->datum.string_lit.textus)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.string_lit.textus);
        }
        frange;

    /* ===== TYPE SPECIFIER ===== */
    casus ARBOR_NODUS_TYPE_SPECIFIER:
        si (nodus->datum.folium.valor)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.folium.valor);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(status->aedificator,
                _lexema_genus_ad_symbolum(nodus->datum.folium.keyword));
        }
        frange;

    /* ===== STORAGE CLASS / TYPE QUALIFIER ===== */
    casus ARBOR_NODUS_STORAGE_CLASS:
    casus ARBOR_NODUS_TYPE_QUALIFIER:
        chorda_aedificator_appendere_literis(status->aedificator,
            _lexema_genus_ad_symbolum(nodus->datum.folium.keyword));
        frange;

    /* ===== STRUCT/UNION SPECIFIER ===== */
    casus ARBOR_NODUS_STRUCT_SPECIFIER:
        chorda_aedificator_appendere_literis(status->aedificator, "struct");
        si (nodus->datum.aggregatum.titulus)
        {
            chorda_aedificator_appendere_literis(status->aedificator, " ");
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.aggregatum.titulus);
        }
        si (nodus->datum.aggregatum.membra)
        {
            /* NON emittere {} - trivia membrorum iam continet punctuationem */
            num = xar_numerus(nodus->datum.aggregatum.membra);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.aggregatum.membra, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    casus ARBOR_NODUS_UNION_SPECIFIER:
        chorda_aedificator_appendere_literis(status->aedificator, "union");
        si (nodus->datum.aggregatum.titulus)
        {
            chorda_aedificator_appendere_literis(status->aedificator, " ");
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.aggregatum.titulus);
        }
        si (nodus->datum.aggregatum.membra)
        {
            /* NON emittere {} - trivia membrorum iam continet punctuationem */
            num = xar_numerus(nodus->datum.aggregatum.membra);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.aggregatum.membra, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== ENUM SPECIFIER ===== */
    casus ARBOR_NODUS_ENUM_SPECIFIER:
        chorda_aedificator_appendere_literis(status->aedificator, "enum");
        si (nodus->datum.enum_spec.titulus)
        {
            chorda_aedificator_appendere_literis(status->aedificator, " ");
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.enum_spec.titulus);
        }
        si (nodus->datum.enum_spec.enumeratores)
        {
            chorda_aedificator_appendere_literis(status->aedificator, "{");
            num = xar_numerus(nodus->datum.enum_spec.enumeratores);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np;
                si (i > ZEPHYRUM)
                    chorda_aedificator_appendere_literis(status->aedificator, ",");
                np = xar_obtinere(nodus->datum.enum_spec.enumeratores, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
            chorda_aedificator_appendere_literis(status->aedificator, "}");
        }
        frange;

    /* ===== ENUMERATOR ===== */
    casus ARBOR_NODUS_ENUMERATOR:
        si (nodus->datum.folium.valor)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.folium.valor);
        }
        frange;

    /* ===== DECLARATOR ===== */
    casus ARBOR_NODUS_DECLARATOR:
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== POINTER ===== */
    casus ARBOR_NODUS_POINTER:
        chorda_aedificator_appendere_literis(status->aedificator, "*");
        si (nodus->datum.pointer.qualifiers)
        {
            num = xar_numerus(nodus->datum.pointer.qualifiers);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.pointer.qualifiers, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        _emittere_nodum_fidelis(status, nodus->datum.pointer.inner);
        frange;

    /* ===== ARRAY DECLARATOR ===== */
    casus ARBOR_NODUS_ARRAY_DECLARATOR:
        _emittere_nodum_fidelis(status, nodus->datum.array.inner);
        chorda_aedificator_appendere_literis(status->aedificator, "[");
        si (nodus->datum.array.size)
            _emittere_nodum_fidelis(status, nodus->datum.array.size);
        chorda_aedificator_appendere_literis(status->aedificator, "]");
        frange;

    /* ===== FUNCTION DECLARATOR ===== */
    casus ARBOR_NODUS_FUNCTION_DECLARATOR:
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== PARAMETER LIST ===== */
    casus ARBOR_NODUS_PARAMETER_LIST:
        chorda_aedificator_appendere_literis(status->aedificator, "(");
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np;
                si (i > ZEPHYRUM)
                    chorda_aedificator_appendere_literis(status->aedificator, ",");
                np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        chorda_aedificator_appendere_literis(status->aedificator, ")");
        frange;

    /* ===== PARAMETER DECLARATION ===== */
    casus ARBOR_NODUS_PARAMETER_DECLARATION:
        si (nodus->datum.parametrum.specifiers)
        {
            num = xar_numerus(nodus->datum.parametrum.specifiers);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.parametrum.specifiers, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        si (nodus->datum.parametrum.declarator)
            _emittere_nodum_fidelis(status, nodus->datum.parametrum.declarator);
        frange;

    /* ===== INIT DECLARATOR ===== */
    casus ARBOR_NODUS_INIT_DECLARATOR:
        _emittere_nodum_fidelis(status, nodus->datum.init_decl.declarator);
        si (nodus->datum.init_decl.initializer)
        {
            chorda_aedificator_appendere_literis(status->aedificator, "=");
            _emittere_nodum_fidelis(status, nodus->datum.init_decl.initializer);
        }
        frange;

    /* ===== INITIALIZER ===== */
    casus ARBOR_NODUS_INITIALIZER:
        si (nodus->datum.genericum.liberi)
        {
            num = xar_numerus(nodus->datum.genericum.liberi);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np = xar_obtinere(nodus->datum.genericum.liberi, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        frange;

    /* ===== INITIALIZER LIST ===== */
    casus ARBOR_NODUS_INITIALIZER_LIST:
        chorda_aedificator_appendere_literis(status->aedificator, "{");
        si (nodus->datum.init_list.elementa)
        {
            num = xar_numerus(nodus->datum.init_list.elementa);
            per (i = ZEPHYRUM; i < num; i++)
            {
                ArborNodus** np;
                si (i > ZEPHYRUM)
                    chorda_aedificator_appendere_literis(status->aedificator, ",");
                np = xar_obtinere(nodus->datum.init_list.elementa, i);
                si (np && *np) _emittere_nodum_fidelis(status, *np);
            }
        }
        chorda_aedificator_appendere_literis(status->aedificator, "}");
        frange;

    /* ===== TYPEDEF NAME ===== */
    casus ARBOR_NODUS_TYPEDEF_NAME:
        si (nodus->datum.folium.valor)
        {
            chorda_aedificator_appendere_chorda(status->aedificator,
                *nodus->datum.folium.valor);
        }
        frange;

    /* ===== ERROR NODE ===== */
    casus ARBOR_NODUS_ERROR:
        /* Emit error marker */
        chorda_aedificator_appendere_literis(status->aedificator, "/* ERROR */");
        frange;

    ordinarius:
        /* Unhandled node type - emit comment */
        chorda_aedificator_appendere_literis(status->aedificator, "/* ? */");
        frange;
    }

    /* Emittere trivia post nodum */
    _emittere_trivia(status, nodus->trivia_post);
}

/* ===========================================================
 * API IMPLEMENTATION
 * =========================================================== */

ArborFormatorOptiones
arbor_formator_optiones_default (
    vacuum)
{
    ArborFormatorOptiones opt;

    opt.latinizare_keywords = VERUM;
    opt.romanizare_numeros  = VERUM;
    opt.max_linea_longitudo = LXXX;
    opt.indentatio_spatia   = II;
    opt.latina_via          = NIHIL;

    redde opt;
}

chorda
arbor_formator_emittere_fidelis (
       Piscina* piscina,
    ArborNodus* radix)
{
    FormatorStatus status;
    chorda         fructus;

    si (piscina == NIHIL || radix == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    status.piscina     = piscina;
    status.aedificator = chorda_aedificator_creare(piscina, MMMMXCVI);
    status.optiones    = NIHIL;
    status.fidelis     = VERUM;
    status.indentatio  = ZEPHYRUM;

    _emittere_nodum_fidelis(&status, radix);

    fructus = chorda_aedificator_finire(status.aedificator);
    redde fructus;
}

chorda
arbor_formator_emittere (
               Piscina* piscina,
            ArborNodus* radix,
    ArborFormatorOptiones* optiones)
{
    /* TODO: Implementare formatus mode */
    /* Pro nunc, use fidelis mode */
    (vacuum)optiones;
    redde arbor_formator_emittere_fidelis(piscina, radix);
}

chorda
arbor_formator_emittere_nodum (
               Piscina* piscina,
            ArborNodus* nodus,
    ArborFormatorOptiones* optiones)
{
    /* TODO: Implementare */
    (vacuum)optiones;
    redde arbor_formator_emittere_fidelis(piscina, nodus);
}

chorda
arbor_formator_emittere_intervallum (
               Piscina* piscina,
            ArborNodus* radix,
                   i32  linea_initium,
                   i32  linea_finis,
    ArborFormatorOptiones* optiones)
{
    /* TODO: Implementare */
    (vacuum)linea_initium;
    (vacuum)linea_finis;
    (vacuum)optiones;
    redde arbor_formator_emittere_fidelis(piscina, radix);
}

/* ===========================================================
 * KEYWORD CONVERSION
 * =========================================================== */

constans character*
arbor_formator_keyword_ad_latinum (
    constans character* anglicum)
{
    i32 i;

    si (anglicum == NIHIL) redde NIHIL;

    per (i = ZEPHYRUM; keyword_tabula[i].latinum != NIHIL; i++)
    {
        si (strcmp(anglicum, keyword_tabula[i].anglicum) == ZEPHYRUM)
        {
            redde keyword_tabula[i].latinum;
        }
    }

    redde NIHIL;
}

constans character*
arbor_formator_keyword_ad_anglicum (
    constans character* latinum)
{
    i32 i;

    si (latinum == NIHIL) redde NIHIL;

    per (i = ZEPHYRUM; keyword_tabula[i].latinum != NIHIL; i++)
    {
        si (strcmp(latinum, keyword_tabula[i].latinum) == ZEPHYRUM)
        {
            redde keyword_tabula[i].anglicum;
        }
    }

    redde NIHIL;
}

/* ===========================================================
 * ROMAN NUMERAL CONVERSION
 * =========================================================== */

chorda
arbor_formator_numerus_ad_romanum (
    Piscina* piscina,
        s64  valor)
{
    ChordaAedificator* aed;
             character  buffer[XXXII];
                  s64   abs_valor;
                  i32   i;
               chorda   fructus;

    /* Structura pro conversione */
    constans structura {
        i32                 num;
        constans character* sym;
    } tabula[] = {
        { M,   "M" },
        { CM,  "CM" },
        { D,   "D" },
        { CD,  "CD" },
        { C,   "C" },
        { XC,  "XC" },
        { L,   "L" },
        { XL,  "XL" },
        { X,   "X" },
        { IX,  "IX" },
        { V,   "V" },
        { IV,  "IV" },
        { I,   "I" },
        { ZEPHYRUM, NIHIL }
    };

    si (piscina == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum   = NIHIL;
        redde fructus;
    }

    /* Zero specialis */
    si (valor == ZEPHYRUM)
    {
        redde chorda_ex_literis("ZEPHYRUM", piscina);
    }

    aed = chorda_aedificator_creare(piscina, LXIV);

    /* Negativus */
    si (valor < ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, "-");
        abs_valor = -valor;
    }
    alioquin
    {
        abs_valor = valor;
    }

    /* Si maior quam MMMMXCVI, redde decimalis */
    si (abs_valor > MMMMXCVI)
    {
        snprintf(buffer, XXXII, "%lld", (longus longus)abs_valor);
        chorda_aedificator_appendere_literis(aed, buffer);
        redde chorda_aedificator_finire(aed);
    }

    /* Convertere ad Romanum */
    per (i = ZEPHYRUM; tabula[i].sym != NIHIL; i++)
    {
        dum (abs_valor >= tabula[i].num)
        {
            chorda_aedificator_appendere_literis(aed, tabula[i].sym);
            abs_valor -= tabula[i].num;
        }
    }

    redde chorda_aedificator_finire(aed);
}
