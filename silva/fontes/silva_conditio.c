/* silva_conditio.c - Evaluator expressionum praeprocessoris */

#include "silva_conditio.h"
#include <string.h>

/* ==================================================
 * Contextus evaluationis
 * ================================================== */

nomen structura {
    Xar* lexemata;   /* SilvaToken* - post substitutionem et expansionem */
    i32  positus;
    i32  numerus;
    b32  error;
} SilvaConditioEval;

/* ==================================================
 * Praedeclarationes (descensus recursivus)
 * ================================================== */

interior s64 _evaluare_expressio (SilvaConditioEval* eval);
interior s64 _evaluare_ternarius (SilvaConditioEval* eval);
interior s64 _evaluare_disiunctio (SilvaConditioEval* eval);
interior s64 _evaluare_coniunctio (SilvaConditioEval* eval);
interior s64 _evaluare_vel_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_xor_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_et_bitalis (SilvaConditioEval* eval);
interior s64 _evaluare_aequalitas (SilvaConditioEval* eval);
interior s64 _evaluare_comparatio (SilvaConditioEval* eval);
interior s64 _evaluare_translatio (SilvaConditioEval* eval);
interior s64 _evaluare_additio (SilvaConditioEval* eval);
interior s64 _evaluare_multiplicatio (SilvaConditioEval* eval);
interior s64 _evaluare_unarium (SilvaConditioEval* eval);
interior s64 _evaluare_primarium (SilvaConditioEval* eval);

/* ==================================================
 * Auxiliares
 * ================================================== */

interior SilvaToken*
_lexema_currens (SilvaConditioEval* eval)
{
    si (eval->positus >= eval->numerus)
    {
        redde NIHIL;
    }
    redde *(SilvaToken**)xar_obtinere(eval->lexemata, eval->positus);
}

interior SilvaLexemaGenus
_genus_currens (SilvaConditioEval* eval)
{
    SilvaToken* tok;

    tok = _lexema_currens(eval);
    si (tok == NIHIL)
    {
        redde SILVA_LEX_EOF;
    }
    redde tok->genus;
}

interior vacuum
_progredi (SilvaConditioEval* eval)
{
    si (eval->positus < eval->numerus)
    {
        eval->positus++;
    }
}

/* Potestne lexema nomen macro esse? (praeprocessor verba clausa non novit) */
interior b32
_est_nomen_potentiale (SilvaToken* token)
{
    si (token->genus == SILVA_LEX_IDENTIFICATOR)
    {
        redde VERUM;
    }
    si (token->genus >= SILVA_LEX_AUTO && token->genus <= SILVA_LEX_WHILE)
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior b32
_est_defined (SilvaToken* tok)
{
    si (tok == NIHIL || tok->genus != SILVA_LEX_IDENTIFICATOR)
    {
        redde FALSUM;
    }
    si (tok->valor.mensura != VII)
    {
        redde FALSUM;
    }
    redde (memcmp(tok->valor.datum, "defined", VII) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Parsere litteram integram (dec/hex/oct, suffixa L/U praetermissa) */
interior s64
_parsere_integer (chorda valor)
{
    s64 fructus;
    s64 basis;
    i32 i;
    constans i8* p;

    fructus = ZEPHYRUM;
    basis = X;
    i = ZEPHYRUM;
    p = valor.datum;

    si (valor.mensura == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    si (i + I < valor.mensura && p[i] == '0')
    {
        si (p[i + I] == 'x' || p[i + I] == 'X')
        {
            basis = XVI;
            i += II;
        }
        alioquin si (p[i + I] >= '0' && p[i + I] <= '7')
        {
            basis = VIII;
            i++;
        }
    }

    dum (i < valor.mensura)
    {
        character c;
        s64 cifra;

        c = (character)p[i];

        si (c == 'L' || c == 'l' || c == 'U' || c == 'u')
        {
            frange;  /* suffixum */
        }

        si (c >= '0' && c <= '9')
        {
            cifra = (s64)(c - '0');
        }
        alioquin si (basis == XVI && c >= 'a' && c <= 'f')
        {
            cifra = X + (s64)(c - 'a');
        }
        alioquin si (basis == XVI && c >= 'A' && c <= 'F')
        {
            cifra = X + (s64)(c - 'A');
        }
        alioquin
        {
            frange;
        }

        si (cifra >= basis)
        {
            frange;
        }

        fructus = fructus * basis + cifra;
        i++;
    }

    redde fructus;
}

/* Littera characteris: 'a' vel '\n' etc. */
interior s64
_parsere_characterem (chorda valor)
{
    si (valor.mensura >= II && valor.datum[ZEPHYRUM] == '\'')
    {
        si (valor.mensura >= III && valor.datum[I] == '\\')
        {
            commutatio ((character)valor.datum[II])
            {
                casus 'n':  redde '\n';
                casus 't':  redde '\t';
                casus 'r':  redde '\r';
                casus '0':  redde ZEPHYRUM;
                casus '\\': redde '\\';
                casus '\'': redde '\'';
                ordinarius: redde (s64)valor.datum[II];
            }
        }
        redde (s64)valor.datum[I];
    }
    redde ZEPHYRUM;
}

/* ==================================================
 * Descensus recursivus
 *
 * Praecedentia (infima ad summam):
 *   ternarius ?:, || , &&, |, ^, &, == !=, < > <= >=,
 *   << >>, + -, * / %, unaria ! ~ - +, primaria
 * ================================================== */

interior s64
_evaluare_expressio (SilvaConditioEval* eval)
{
    redde _evaluare_ternarius(eval);
}

interior s64
_evaluare_ternarius (SilvaConditioEval* eval)
{
    s64 conditio;
    s64 valor_verus;
    s64 valor_falsus;

    conditio = _evaluare_disiunctio(eval);

    si (_genus_currens(eval) == SILVA_LEX_QUAESTIO)
    {
        _progredi(eval);
        valor_verus = _evaluare_expressio(eval);
        si (_genus_currens(eval) != SILVA_LEX_COLON)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _progredi(eval);
        valor_falsus = _evaluare_ternarius(eval);
        redde conditio ? valor_verus : valor_falsus;
    }

    redde conditio;
}

interior s64
_evaluare_disiunctio (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_coniunctio(eval);
    dum (_genus_currens(eval) == SILVA_LEX_VEL_VEL)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_coniunctio(eval);
        sinister = (sinister || dexter) ? I : ZEPHYRUM;
    }
    redde sinister;
}

interior s64
_evaluare_coniunctio (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_vel_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_ET_ET)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_vel_bitalis(eval);
        sinister = (sinister && dexter) ? I : ZEPHYRUM;
    }
    redde sinister;
}

interior s64
_evaluare_vel_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_xor_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_BARRA)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_xor_bitalis(eval);
        sinister = sinister | dexter;
    }
    redde sinister;
}

interior s64
_evaluare_xor_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_et_bitalis(eval);
    dum (_genus_currens(eval) == SILVA_LEX_CARET)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_et_bitalis(eval);
        sinister = sinister ^ dexter;
    }
    redde sinister;
}

interior s64
_evaluare_et_bitalis (SilvaConditioEval* eval)
{
    s64 sinister;

    sinister = _evaluare_aequalitas(eval);
    dum (_genus_currens(eval) == SILVA_LEX_AMPERSAND)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_aequalitas(eval);
        sinister = sinister & dexter;
    }
    redde sinister;
}

interior s64
_evaluare_aequalitas (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_comparatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_AEQUALIS_AEQUALIS
        || op == SILVA_LEX_NON_AEQUALIS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_comparatio(eval);
        si (op == SILVA_LEX_AEQUALIS_AEQUALIS)
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

interior s64
_evaluare_comparatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_translatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_MINOR
        || op == SILVA_LEX_MAIOR
        || op == SILVA_LEX_MINOR_AEQUALIS
        || op == SILVA_LEX_MAIOR_AEQUALIS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_translatio(eval);
        commutatio (op)
        {
            casus SILVA_LEX_MINOR:
                sinister = (sinister < dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MAIOR:
                sinister = (sinister > dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MINOR_AEQUALIS:
                sinister = (sinister <= dexter) ? I : ZEPHYRUM;
                frange;
            casus SILVA_LEX_MAIOR_AEQUALIS:
                sinister = (sinister >= dexter) ? I : ZEPHYRUM;
                frange;
            ordinarius:
                frange;
        }
    }
    redde sinister;
}

interior s64
_evaluare_translatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_additio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_SINISTRORSUM
        || op == SILVA_LEX_DEXTRORSUM)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_additio(eval);
        si (dexter < ZEPHYRUM || dexter >= LXIV)
        {
            eval->error = VERUM;  /* translatio extra fines: UB vitatum */
            redde ZEPHYRUM;
        }
        si (op == SILVA_LEX_SINISTRORSUM)
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

interior s64
_evaluare_additio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_multiplicatio(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_PLUS
        || op == SILVA_LEX_MINUS)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_multiplicatio(eval);
        si (op == SILVA_LEX_PLUS)
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

interior s64
_evaluare_multiplicatio (SilvaConditioEval* eval)
{
    s64 sinister;
    SilvaLexemaGenus op;

    sinister = _evaluare_unarium(eval);
    dum ((op = _genus_currens(eval)) == SILVA_LEX_STAR
        || op == SILVA_LEX_SOLIDUS
        || op == SILVA_LEX_PERCENTUM)
    {
        s64 dexter;

        _progredi(eval);
        dexter = _evaluare_unarium(eval);
        si (op == SILVA_LEX_STAR)
        {
            sinister = sinister * dexter;
        }
        alioquin
        {
            si (dexter == ZEPHYRUM)
            {
                eval->error = VERUM;  /* divisio per zephyrum */
                redde ZEPHYRUM;
            }
            si (op == SILVA_LEX_SOLIDUS)
            {
                sinister = sinister / dexter;
            }
            alioquin
            {
                sinister = sinister % dexter;
            }
        }
    }
    redde sinister;
}

interior s64
_evaluare_unarium (SilvaConditioEval* eval)
{
    SilvaLexemaGenus op;

    op = _genus_currens(eval);

    si (op == SILVA_LEX_EXCLAMATIO)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde operandum ? ZEPHYRUM : I;
    }
    si (op == SILVA_LEX_TILDE)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde ~operandum;
    }
    si (op == SILVA_LEX_MINUS)
    {
        s64 operandum;

        _progredi(eval);
        operandum = _evaluare_unarium(eval);
        redde -operandum;
    }
    si (op == SILVA_LEX_PLUS)
    {
        _progredi(eval);
        redde _evaluare_unarium(eval);
    }

    redde _evaluare_primarium(eval);
}

interior s64
_evaluare_primarium (SilvaConditioEval* eval)
{
    SilvaToken* tok;

    tok = _lexema_currens(eval);
    si (tok == NIHIL)
    {
        eval->error = VERUM;
        redde ZEPHYRUM;
    }

    si (tok->genus == SILVA_LEX_INTEGER)
    {
        s64 valor;

        valor = _parsere_integer(tok->valor);
        _progredi(eval);
        redde valor;
    }

    si (tok->genus == SILVA_LEX_PAREN_APERTA)
    {
        s64 valor;

        _progredi(eval);
        valor = _evaluare_expressio(eval);
        si (_genus_currens(eval) != SILVA_LEX_PAREN_CLAUSA)
        {
            eval->error = VERUM;
            redde ZEPHYRUM;
        }
        _progredi(eval);
        redde valor;
    }

    si (tok->genus == SILVA_LEX_CHARACTER_LIT)
    {
        s64 valor;

        valor = _parsere_characterem(tok->valor);
        _progredi(eval);
        redde valor;
    }

    /* Identificator reliquus post expansionem = 0 (C89) */
    si (_est_nomen_potentiale(tok))
    {
        _progredi(eval);
        redde ZEPHYRUM;
    }

    eval->error = VERUM;
    redde ZEPHYRUM;
}

/* ==================================================
 * Substitutio 'defined' (protecta - ANTE expansionem)
 *
 * defined X / defined(X) -> lexema INTEGER syntheticum "1"/"0".
 * Operanda numquam expanduntur (C89 6.8.1). Reddit NIHIL in
 * forma malformata (evaluatio deficit).
 * ================================================== */

interior SilvaToken*
_integer_syntheticum (SilvaExpansio* exp, b32 valor, chorda* nomen_macro)
{
    chorda textus;
    unio { constans character* c; i8* m; } u;

    u.c = valor ? "1" : "0";
    textus.datum = u.m;
    textus.mensura = I;
    redde silva_token_ex_api(exp->piscina, SILVA_LEX_INTEGER, textus,
        nomen_macro, -I);
}

interior chorda*
_chorda_figere (Piscina* piscina, chorda c)
{
    chorda* fixa;

    fixa = (chorda*)piscina_allocare(piscina, (memoriae_index)magnitudo(chorda));
    si (fixa != NIHIL)
    {
        *fixa = c;
    }
    redde fixa;
}

interior Xar*
_defined_substituere (SilvaExpansio* exp, Xar* lexemata)
{
    Xar* exitus;
    SilvaToken* tok;
    i32 i;
    i32 n;

    exitus = xar_creare(exp->piscina, magnitudo(SilvaToken*));
    n = xar_numerus(lexemata);
    i = ZEPHYRUM;

    dum (i < n)
    {
        tok = *(SilvaToken**)xar_obtinere(lexemata, i);

        si (_est_defined(tok))
        {
            SilvaToken* operandum;
            SilvaToken** locus;
            b32 habet_paren;
            b32 est_def;

            i++;
            habet_paren = FALSUM;
            si (i < n && (*(SilvaToken**)xar_obtinere(lexemata, i))->genus
                == SILVA_LEX_PAREN_APERTA)
            {
                habet_paren = VERUM;
                i++;
            }
            si (i >= n)
            {
                redde NIHIL;  /* malformata */
            }
            operandum = *(SilvaToken**)xar_obtinere(lexemata, i);
            si (!_est_nomen_potentiale(operandum))
            {
                redde NIHIL;
            }
            i++;
            si (habet_paren)
            {
                si (i >= n || (*(SilvaToken**)xar_obtinere(lexemata, i))->genus
                    != SILVA_LEX_PAREN_CLAUSA)
                {
                    redde NIHIL;
                }
                i++;
            }

            est_def = silva_conditio_est_definitum(exp, operandum->valor);
            locus = (SilvaToken**)xar_addere(exitus);
            si (locus != NIHIL)
            {
                *locus = _integer_syntheticum(exp, est_def,
                    _chorda_figere(exp->piscina, operandum->valor));
            }
            perge;
        }

        {
            SilvaToken** locus;

            locus = (SilvaToken**)xar_addere(exitus);
            si (locus != NIHIL)
            {
                *locus = tok;
            }
        }
        i++;
    }

    redde exitus;
}

/* ==================================================
 * API publica
 * ================================================== */

b32
silva_conditio_est_definitum (
    SilvaExpansio* exp,
    chorda         titulus)
{
    si (exp == NIHIL)
    {
        redde FALSUM;
    }
    redde (silva_expansio_quaerere(exp, titulus) != NIHIL) ? VERUM : FALSUM;
}

s64
silva_conditio_evaluare (
    SilvaExpansio* exp,
    Xar*           lexemata,
    b32*           successus)
{
    SilvaConditioEval eval;
    Xar* substituta;
    Xar* expansa;
    s64 fructus;

    si (successus != NIHIL)
    {
        *successus = FALSUM;
    }
    si (exp == NIHIL || lexemata == NIHIL || xar_numerus(lexemata) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    /* 1. defined-operanda protecte substituere */
    substituta = _defined_substituere(exp, lexemata);
    si (substituta == NIHIL)
    {
        redde ZEPHYRUM;
    }

    /* 2. expansio macro plena (expansor verus, caecationes) */
    expansa = silva_expansio_expandere(exp, substituta, NIHIL);

    /* 3. evaluatio arithmetica */
    eval.lexemata = expansa;
    eval.positus = ZEPHYRUM;
    eval.numerus = xar_numerus(expansa);
    eval.error = FALSUM;

    si (eval.numerus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }

    fructus = _evaluare_expressio(&eval);

    si (eval.error || eval.positus < eval.numerus)
    {
        redde ZEPHYRUM;  /* error vel lexemata non consumpta */
    }

    si (successus != NIHIL)
    {
        *successus = VERUM;
    }
    redde fructus;
}
