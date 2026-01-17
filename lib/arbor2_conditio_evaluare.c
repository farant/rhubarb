/* arbor2_conditio_evaluare.c - Preprocessor Expression Evaluator */
#include "latina.h"
#include "arbor2_conditio_evaluare.h"
#include "arbor2_expandere.h"
#include "arbor2_lexema.h"
#include "arbor2_token.h"
#include "xar.h"
#include "chorda.h"
#include <string.h>

/* ==================================================
 * Evaluator Context
 * ================================================== */

nomen structura {
    Arbor2Expansion*    exp;        /* For macro lookups */
    Xar*                lexemata;   /* Expression tokens */
    i32                 positus;    /* Current position */
    i32                 numerus;    /* Token count */
    b32                 error;      /* Error flag */
    Piscina*            piscina;    /* For allocations */
} Arbor2ConditioEval;

/* ==================================================
 * Forward Declarations
 * ================================================== */

interior i64 _evaluare_expressio(Arbor2ConditioEval* eval);
interior i64 _evaluare_ternarius(Arbor2ConditioEval* eval);
interior i64 _evaluare_disiunctio(Arbor2ConditioEval* eval);
interior i64 _evaluare_coniunctio(Arbor2ConditioEval* eval);
interior i64 _evaluare_or_bitwise(Arbor2ConditioEval* eval);
interior i64 _evaluare_xor_bitwise(Arbor2ConditioEval* eval);
interior i64 _evaluare_and_bitwise(Arbor2ConditioEval* eval);
interior i64 _evaluare_aequalitas(Arbor2ConditioEval* eval);
interior i64 _evaluare_comparatio(Arbor2ConditioEval* eval);
interior i64 _evaluare_translatio(Arbor2ConditioEval* eval);
interior i64 _evaluare_additio(Arbor2ConditioEval* eval);
interior i64 _evaluare_multiplicatio(Arbor2ConditioEval* eval);
interior i64 _evaluare_unarium(Arbor2ConditioEval* eval);
interior i64 _evaluare_primarium(Arbor2ConditioEval* eval);

/* ==================================================
 * Helper Functions
 * ================================================== */

interior Arbor2Token*
_token_currens(Arbor2ConditioEval* eval)
{
    si (eval->positus >= eval->numerus)
    {
        redde NIHIL;
    }
    redde *(Arbor2Token**)xar_obtinere(eval->lexemata, eval->positus);
}

interior Arbor2LexemaGenus
_genus_currens(Arbor2ConditioEval* eval)
{
    Arbor2Token* tok;
    tok = _token_currens(eval);
    si (tok == NIHIL)
    {
        redde ARBOR2_LEXEMA_EOF;
    }
    redde tok->lexema->genus;
}

interior vacuum
_avanzare(Arbor2ConditioEval* eval)
{
    si (eval->positus < eval->numerus)
    {
        eval->positus++;
    }
}

interior b32
_est_defined(Arbor2Token* tok)
{
    si (tok == NIHIL)
    {
        redde FALSUM;
    }
    si (tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    si (tok->lexema->valor.mensura != VII)
    {
        redde FALSUM;
    }
    redde (memcmp(tok->lexema->valor.datum, "defined", VII) == ZEPHYRUM);
}

/* Parse integer literal value */
interior i64
_parsere_integer(chorda valor)
{
    i64 result;
    i32 i;
    i32 base;
    b32 is_negative;
    constans i8* p;

    result = ZEPHYRUM;
    i = ZEPHYRUM;
    is_negative = FALSUM;
    p = valor.datum;

    si (valor.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    /* Handle optional sign */
    si (p[i] == '-')
    {
        is_negative = VERUM;
        i++;
    }
    alioquin si (p[i] == '+')
    {
        i++;
    }

    /* Determine base */
    base = X;
    si (i + I < valor.mensura && p[i] == '0')
    {
        si (p[i + I] == 'x' || p[i + I] == 'X')
        {
            base = XVI;
            i += II;
        }
        alioquin si (p[i + I] >= '0' && p[i + I] <= '7')
        {
            base = VIII;
            i++;
        }
    }

    /* Parse digits */
    dum (i < valor.mensura)
    {
        character c;
        i32 digit;

        c = (character)p[i];

        /* Skip suffix letters (L, U, l, u) */
        si (c == 'L' || c == 'l' || c == 'U' || c == 'u')
        {
            frange;
        }

        si (c >= '0' && c <= '9')
        {
            digit = (i32)(c - '0');
        }
        alioquin si (base == XVI && c >= 'a' && c <= 'f')
        {
            digit = X + (i32)(c - 'a');
        }
        alioquin si (base == XVI && c >= 'A' && c <= 'F')
        {
            digit = X + (i32)(c - 'A');
        }
        alioquin
        {
            frange;
        }

        si (digit >= base)
        {
            frange;
        }

        result = result * base + digit;
        i++;
    }

    si (is_negative)
    {
        result = -result;
    }

    redde result;
}

/* ==================================================
 * Expression Parsing (Recursive Descent)
 *
 * Precedence (low to high):
 *   ternarius     ? :
 *   disiunctio    ||
 *   coniunctio    &&
 *   or_bitwise    |
 *   xor_bitwise   ^
 *   and_bitwise   &
 *   aequalitas    == !=
 *   comparatio    < > <= >=
 *   translatio    << >>
 *   additio       + -
 *   multiplicatio * / %
 *   unarium       ! ~ - +
 *   primarium     integer, defined, (expr), ident
 * ================================================== */

/* expressio -> ternarius */
interior i64
_evaluare_expressio(Arbor2ConditioEval* eval)
{
    redde _evaluare_ternarius(eval);
}

/* ternarius -> disiunctio ('?' expressio ':' ternarius)? */
interior i64
_evaluare_ternarius(Arbor2ConditioEval* eval)
{
    i64 conditio;
    i64 verum_val;
    i64 falsum_val;

    conditio = _evaluare_disiunctio(eval);

    si (_genus_currens(eval) == ARBOR2_LEXEMA_QUAESTIO)
    {
        _avanzare(eval);  /* skip ? */

        verum_val = _evaluare_expressio(eval);

        si (_genus_currens(eval) != ARBOR2_LEXEMA_COLON)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _avanzare(eval);  /* skip : */

        falsum_val = _evaluare_ternarius(eval);

        redde conditio ? verum_val : falsum_val;
    }

    redde conditio;
}

/* disiunctio -> coniunctio ('||' coniunctio)* */
interior i64
_evaluare_disiunctio(Arbor2ConditioEval* eval)
{
    i64 sinister;

    sinister = _evaluare_coniunctio(eval);

    dum (_genus_currens(eval) == ARBOR2_LEXEMA_DUPIPA)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_coniunctio(eval);
        sinister = (sinister || dexter) ? I : ZEPHYRUM;
    }

    redde sinister;
}

/* coniunctio -> or_bitwise ('&&' or_bitwise)* */
interior i64
_evaluare_coniunctio(Arbor2ConditioEval* eval)
{
    i64 sinister;

    sinister = _evaluare_or_bitwise(eval);

    dum (_genus_currens(eval) == ARBOR2_LEXEMA_DUAMPERSAND)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_or_bitwise(eval);
        sinister = (sinister && dexter) ? I : ZEPHYRUM;
    }

    redde sinister;
}

/* or_bitwise -> xor_bitwise ('|' xor_bitwise)* */
interior i64
_evaluare_or_bitwise(Arbor2ConditioEval* eval)
{
    i64 sinister;

    sinister = _evaluare_xor_bitwise(eval);

    dum (_genus_currens(eval) == ARBOR2_LEXEMA_PIPA)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_xor_bitwise(eval);
        sinister = sinister | dexter;
    }

    redde sinister;
}

/* xor_bitwise -> and_bitwise ('^' and_bitwise)* */
interior i64
_evaluare_xor_bitwise(Arbor2ConditioEval* eval)
{
    i64 sinister;

    sinister = _evaluare_and_bitwise(eval);

    dum (_genus_currens(eval) == ARBOR2_LEXEMA_CARET)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_and_bitwise(eval);
        sinister = sinister ^ dexter;
    }

    redde sinister;
}

/* and_bitwise -> aequalitas ('&' aequalitas)* */
interior i64
_evaluare_and_bitwise(Arbor2ConditioEval* eval)
{
    i64 sinister;

    sinister = _evaluare_aequalitas(eval);

    dum (_genus_currens(eval) == ARBOR2_LEXEMA_AMPERSAND)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_aequalitas(eval);
        sinister = sinister & dexter;
    }

    redde sinister;
}

/* aequalitas -> comparatio (('=='|'!=') comparatio)* */
interior i64
_evaluare_aequalitas(Arbor2ConditioEval* eval)
{
    i64 sinister;
    Arbor2LexemaGenus op;

    sinister = _evaluare_comparatio(eval);

    dum ((op = _genus_currens(eval)) == ARBOR2_LEXEMA_AEQUALIS ||
         op == ARBOR2_LEXEMA_NON_AEQUALIS)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_comparatio(eval);

        si (op == ARBOR2_LEXEMA_AEQUALIS)
        {
            sinister = (sinister == dexter) ? I : ZEPHYRUM;
        }
        alioquin
        {
            sinister = (sinister != dexter) ? I : ZEPHYRUM;
        }
    }

    redde sinister;
}

/* comparatio -> translatio (('<'|'>'|'<='|'>=') translatio)* */
interior i64
_evaluare_comparatio(Arbor2ConditioEval* eval)
{
    i64 sinister;
    Arbor2LexemaGenus op;

    sinister = _evaluare_translatio(eval);

    dum ((op = _genus_currens(eval)) == ARBOR2_LEXEMA_MINOR ||
         op == ARBOR2_LEXEMA_MAIOR ||
         op == ARBOR2_LEXEMA_MINOR_AEQ ||
         op == ARBOR2_LEXEMA_MAIOR_AEQ)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_translatio(eval);

        commutatio (op)
        {
            casus ARBOR2_LEXEMA_MINOR:
                sinister = (sinister < dexter) ? I : ZEPHYRUM;
                frange;
            casus ARBOR2_LEXEMA_MAIOR:
                sinister = (sinister > dexter) ? I : ZEPHYRUM;
                frange;
            casus ARBOR2_LEXEMA_MINOR_AEQ:
                sinister = (sinister <= dexter) ? I : ZEPHYRUM;
                frange;
            casus ARBOR2_LEXEMA_MAIOR_AEQ:
                sinister = (sinister >= dexter) ? I : ZEPHYRUM;
                frange;
            ordinarius:
                frange;
        }
    }

    redde sinister;
}

/* translatio -> additio (('<<'|'>>') additio)* */
interior i64
_evaluare_translatio(Arbor2ConditioEval* eval)
{
    i64 sinister;
    Arbor2LexemaGenus op;

    sinister = _evaluare_additio(eval);

    dum ((op = _genus_currens(eval)) == ARBOR2_LEXEMA_SINISTRUM ||
         op == ARBOR2_LEXEMA_DEXTRUM)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_additio(eval);

        si (op == ARBOR2_LEXEMA_SINISTRUM)
        {
            sinister = sinister << dexter;
        }
        alioquin
        {
            sinister = sinister >> dexter;
        }
    }

    redde sinister;
}

/* additio -> multiplicatio (('+'|'-') multiplicatio)* */
interior i64
_evaluare_additio(Arbor2ConditioEval* eval)
{
    i64 sinister;
    Arbor2LexemaGenus op;

    sinister = _evaluare_multiplicatio(eval);

    dum ((op = _genus_currens(eval)) == ARBOR2_LEXEMA_PLUS ||
         op == ARBOR2_LEXEMA_MINUS)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_multiplicatio(eval);

        si (op == ARBOR2_LEXEMA_PLUS)
        {
            sinister = sinister + dexter;
        }
        alioquin
        {
            sinister = sinister - dexter;
        }
    }

    redde sinister;
}

/* multiplicatio -> unarium (('*'|'/'|'%') unarium)* */
interior i64
_evaluare_multiplicatio(Arbor2ConditioEval* eval)
{
    i64 sinister;
    Arbor2LexemaGenus op;

    sinister = _evaluare_unarium(eval);

    dum ((op = _genus_currens(eval)) == ARBOR2_LEXEMA_ASTERISCUS ||
         op == ARBOR2_LEXEMA_SOLIDUS ||
         op == ARBOR2_LEXEMA_PERCENTUM)
    {
        i64 dexter;
        _avanzare(eval);
        dexter = _evaluare_unarium(eval);

        si (op == ARBOR2_LEXEMA_ASTERISCUS)
        {
            sinister = sinister * dexter;
        }
        alioquin si (op == ARBOR2_LEXEMA_SOLIDUS)
        {
            si (dexter == ZEPHYRUM)
            {
                eval->error = VERUM;
                redde ZEPHYRUM;
            }
            sinister = sinister / dexter;
        }
        alioquin
        {
            si (dexter == ZEPHYRUM)
            {
                eval->error = VERUM;
                redde ZEPHYRUM;
            }
            sinister = sinister % dexter;
        }
    }

    redde sinister;
}

/* unarium -> ('!'|'~'|'-'|'+') unarium | primarium */
interior i64
_evaluare_unarium(Arbor2ConditioEval* eval)
{
    Arbor2LexemaGenus op;

    op = _genus_currens(eval);

    si (op == ARBOR2_LEXEMA_EXCLAMATIO)
    {
        i64 operandum;
        _avanzare(eval);
        operandum = _evaluare_unarium(eval);
        redde operandum ? ZEPHYRUM : I;
    }

    si (op == ARBOR2_LEXEMA_TILDE)
    {
        i64 operandum;
        _avanzare(eval);
        operandum = _evaluare_unarium(eval);
        redde ~operandum;
    }

    si (op == ARBOR2_LEXEMA_MINUS)
    {
        i64 operandum;
        _avanzare(eval);
        operandum = _evaluare_unarium(eval);
        redde -operandum;
    }

    si (op == ARBOR2_LEXEMA_PLUS)
    {
        _avanzare(eval);
        redde _evaluare_unarium(eval);
    }

    redde _evaluare_primarium(eval);
}

/* primarium -> INTEGER | '(' expressio ')' | 'defined' ... | IDENT */
interior i64
_evaluare_primarium(Arbor2ConditioEval* eval)
{
    Arbor2Token* tok;
    Arbor2LexemaGenus genus;

    tok = _token_currens(eval);
    si (tok == NIHIL)
    {
        eval->error = VERUM;
        redde ZEPHYRUM;
    }

    genus = tok->lexema->genus;

    /* Integer literal */
    si (genus == ARBOR2_LEXEMA_INTEGER)
    {
        i64 valor;
        valor = _parsere_integer(tok->lexema->valor);
        _avanzare(eval);
        redde valor;
    }

    /* Parenthesized expression */
    si (genus == ARBOR2_LEXEMA_PAREN_APERTA)
    {
        i64 valor;
        _avanzare(eval);  /* skip ( */
        valor = _evaluare_expressio(eval);

        si (_genus_currens(eval) != ARBOR2_LEXEMA_PAREN_CLAUSA)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _avanzare(eval);  /* skip ) */
        redde valor;
    }

    /* defined(MACRO) or defined MACRO */
    si (genus == ARBOR2_LEXEMA_IDENTIFICATOR && _est_defined(tok))
    {
        b32 habet_paren;
        Arbor2Token* ident_tok;
        b32 est_def;

        _avanzare(eval);  /* skip 'defined' */

        /* Check for optional parenthesis */
        habet_paren = (_genus_currens(eval) == ARBOR2_LEXEMA_PAREN_APERTA);
        si (habet_paren)
        {
            _avanzare(eval);  /* skip ( */
        }

        /* Get identifier */
        ident_tok = _token_currens(eval);
        si (ident_tok == NIHIL || ident_tok->lexema->genus != ARBOR2_LEXEMA_IDENTIFICATOR)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _avanzare(eval);

        /* Check if macro is defined */
        est_def = arbor2_conditio_est_definitum(eval->exp, ident_tok->lexema->valor);

        /* Close parenthesis if opened */
        si (habet_paren)
        {
            si (_genus_currens(eval) != ARBOR2_LEXEMA_PAREN_CLAUSA)
            {
                eval->error = VERUM;
                redde ZEPHYRUM;
            }
            _avanzare(eval);
        }

        redde est_def ? I : ZEPHYRUM;
    }

    /* Identifier - expand macro or treat as 0 */
    si (genus == ARBOR2_LEXEMA_IDENTIFICATOR)
    {
        Arbor2MacroDef* def;

        def = arbor2_expansion_quaerere_macro(eval->exp, tok->lexema->valor);
        _avanzare(eval);

        si (def == NIHIL)
        {
            /* Undefined macro = 0 (C standard) */
            redde ZEPHYRUM;
        }

        /* Simple object-like macro: evaluate its body */
        si (!def->est_functio && def->corpus != NIHIL)
        {
            i32 num_body;
            num_body = xar_numerus(def->corpus);

            si (num_body == I)
            {
                /* Single token - evaluate directly */
                Arbor2Lexema* body_lex;
                body_lex = *(Arbor2Lexema**)xar_obtinere(def->corpus, ZEPHYRUM);

                si (body_lex->genus == ARBOR2_LEXEMA_INTEGER)
                {
                    redde _parsere_integer(body_lex->valor);
                }
            }

            /* For more complex macros, would need recursive evaluation */
            /* For now, treat as 0 */
        }

        redde ZEPHYRUM;
    }

    /* Character literal */
    si (genus == ARBOR2_LEXEMA_CHAR_LIT)
    {
        /* Parse character constant */
        chorda val;
        i64 result;

        val = tok->lexema->valor;
        _avanzare(eval);

        si (val.mensura >= II && val.datum[0] == '\'')
        {
            si (val.datum[1] == '\\' && val.mensura >= III)
            {
                /* Escape sequence */
                commutatio (val.datum[2])
                {
                    casus 'n': result = '\n'; frange;
                    casus 't': result = '\t'; frange;
                    casus 'r': result = '\r'; frange;
                    casus '0': result = '\0'; frange;
                    casus '\\': result = '\\'; frange;
                    casus '\'': result = '\''; frange;
                    ordinarius: result = val.datum[2]; frange;
                }
            }
            alioquin
            {
                result = (i64)(insignatus character)val.datum[1];
            }
            redde result;
        }
        redde ZEPHYRUM;
    }

    /* Unknown token */
    eval->error = VERUM;
    redde ZEPHYRUM;
}

/* ==================================================
 * Public API
 * ================================================== */

b32 arbor2_conditio_est_definitum(
    Arbor2Expansion*    exp,
    chorda              titulus)
{
    Arbor2MacroDef* def;

    si (exp == NIHIL)
    {
        redde FALSUM;
    }

    def = arbor2_expansion_quaerere_macro(exp, titulus);
    redde (def != NIHIL);
}

i64 arbor2_conditio_evaluare(
    Arbor2Expansion*    exp,
    Xar*                lexemata,
    b32*                successus)
{
    Arbor2ConditioEval eval;
    i64 result;

    /* Initialize evaluator context */
    eval.exp = exp;
    eval.lexemata = lexemata;
    eval.positus = ZEPHYRUM;
    eval.numerus = (lexemata != NIHIL) ? xar_numerus(lexemata) : ZEPHYRUM;
    eval.error = FALSUM;
    eval.piscina = (exp != NIHIL) ? exp->piscina : NIHIL;

    /* Empty expression = error */
    si (eval.numerus == ZEPHYRUM)
    {
        si (successus != NIHIL)
        {
            *successus = FALSUM;
        }
        redde ZEPHYRUM;
    }

    /* Evaluate expression */
    result = _evaluare_expressio(&eval);

    /* Check for errors or unconsumed tokens */
    si (eval.error || eval.positus < eval.numerus)
    {
        /* Allow trailing EOF */
        si (eval.positus == eval.numerus - I &&
            _genus_currens(&eval) == ARBOR2_LEXEMA_EOF)
        {
            si (successus != NIHIL)
            {
                *successus = VERUM;
            }
            redde result;
        }

        si (successus != NIHIL)
        {
            *successus = FALSUM;
        }
        redde ZEPHYRUM;
    }

    si (successus != NIHIL)
    {
        *successus = VERUM;
    }

    redde result;
}
