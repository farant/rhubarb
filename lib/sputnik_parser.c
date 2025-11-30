/* sputnik_parser.c - Parser pro Sputnik
 *
 * Pratt parser pro expressionibus, recursive descent pro sententiis.
 */

#include "sputnik_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Praecedentiae Operatorum
 *
 * A minimo ad maximum:
 * 1  = (assignment)
 * 2  ?: (ternaria)
 * 3  || (vel)
 * 4  && (et)
 * 5  == === != !== (aequalitas)
 * 6  < > <= >= (comparatio)
 * 7  + - (additio)
 * 8  * / % (multiplicatio)
 * 9  ! - (unaria prefix)
 * 10 . [] () (postfix)
 * ================================================== */

#define PRAEC_ASSIGNATIO    I
#define PRAEC_TERNARIA      II
#define PRAEC_VEL           III
#define PRAEC_ET            IV
#define PRAEC_AEQUALITAS    V
#define PRAEC_COMPARATIO    VI
#define PRAEC_ADDITIO       VII
#define PRAEC_MULTIPLICATIO VIII
#define PRAEC_UNARIA        IX
#define PRAEC_POSTFIX       X


/* ==================================================
 * Prototypi Interni
 * ================================================== */

interior SputnikAstNodus* _parsere_expressionem(SputnikParser* parser, i32 praecedentia);
interior SputnikAstNodus* _parsere_sententiam(SputnikParser* parser);
interior SputnikAstNodus* _parsere_declarationem_let(SputnikParser* parser);
interior SputnikAstNodus* _parsere_declarationem_const(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_si(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_dum(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_per(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_redde(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_grex(SputnikParser* parser);
interior SputnikAstNodus* _parsere_declarationem_functio(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_frange(SputnikParser* parser);
interior SputnikAstNodus* _parsere_sententiam_perge(SputnikParser* parser);
interior SputnikAstNodus* _parsere_functionem_sagittam(SputnikParser* parser);
interior b32 _est_functio_sagitta_parenthesis(SputnikParser* parser);


/* ==================================================
 * Adiutores Interni - Navigation
 * ================================================== */

interior SputnikLexema*
_currens(SputnikParser* parser)
{
    si (parser->positus >= xar_numerus(parser->lexemata))
    {
        redde NIHIL;
    }
    redde xar_obtinere(parser->lexemata, parser->positus);
}

interior SputnikLexema*
_aspicere(SputnikParser* parser, i32 offset)
{
    i32 pos;
    pos = parser->positus + offset;
    si (pos < ZEPHYRUM || pos >= xar_numerus(parser->lexemata))
    {
        redde NIHIL;
    }
    redde xar_obtinere(parser->lexemata, pos);
}

interior b32
_finis(SputnikParser* parser)
{
    SputnikLexema* lex;
    lex = _currens(parser);
    redde lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS;
}

interior vacuum
_progredi(SputnikParser* parser)
{
    si (!_finis(parser))
    {
        parser->positus++;
    }
}

interior b32
_verificare(SputnikParser* parser, SputnikLexemaGenus genus)
{
    SputnikLexema* lex;
    lex = _currens(parser);
    redde lex != NIHIL && lex->genus == genus;
}

interior b32
_consumere(SputnikParser* parser, SputnikLexemaGenus genus)
{
    si (_verificare(parser, genus))
    {
        _progredi(parser);
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
_error(SputnikParser* parser, constans character* nuntius)
{
    SputnikLexema* lex;

    si (parser->error_accidit)
    {
        redde;  /* Solum primus error */
    }

    parser->error_accidit = VERUM;
    parser->error_nuntius = chorda_ex_literis(nuntius, parser->piscina);

    lex = _currens(parser);
    si (lex != NIHIL)
    {
        parser->error_linea = lex->linea;
        parser->error_columna = lex->columna;
    }
    alioquin
    {
        parser->error_linea = ZEPHYRUM;
        parser->error_columna = ZEPHYRUM;
    }
}

interior vacuum
_expectare(SputnikParser* parser, SputnikLexemaGenus genus, constans character* nuntius)
{
    si (!_consumere(parser, genus))
    {
        _error(parser, nuntius);
    }
}


/* ==================================================
 * Adiutores Interni - Node Creation
 * ================================================== */

interior SputnikAstNodus*
_creare_nodum(SputnikParser* parser, SputnikAstGenus genus)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;

    nodus = piscina_allocare(parser->piscina, magnitudo(SputnikAstNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus = genus;
    nodus->valor.datum = NIHIL;
    nodus->valor.mensura = ZEPHYRUM;
    nodus->operator = ZEPHYRUM;
    nodus->numerus = 0.0;
    nodus->liberi = xar_creare(parser->piscina, magnitudo(SputnikAstNodus*));

    lex = _currens(parser);
    si (lex != NIHIL)
    {
        nodus->linea = lex->linea;
        nodus->columna = lex->columna;
    }
    alioquin
    {
        nodus->linea = ZEPHYRUM;
        nodus->columna = ZEPHYRUM;
    }

    redde nodus;
}

interior vacuum
_addere_infantem(SputnikAstNodus* parens, SputnikAstNodus* infans)
{
    SputnikAstNodus** ptr;

    si (parens == NIHIL || parens->liberi == NIHIL)
    {
        redde;
    }

    ptr = xar_addere(parens->liberi);
    si (ptr != NIHIL)
    {
        *ptr = infans;
    }
}


/* ==================================================
 * Praecedentia Lookup
 * ================================================== */

interior i32
_praecedentia_infixum(SputnikLexemaGenus genus)
{
    commutatio (genus)
    {
        /* Assignatio (includere assignatio complexa) */
        casus SPUTNIK_LEXEMA_AEQUALIS:
        casus SPUTNIK_LEXEMA_PLUS_AEQ:
        casus SPUTNIK_LEXEMA_MINUS_AEQ:
        casus SPUTNIK_LEXEMA_ASTERISCUS_AEQ:
        casus SPUTNIK_LEXEMA_DIVISIO_AEQ:
        casus SPUTNIK_LEXEMA_MODULUS_AEQ:
            redde PRAEC_ASSIGNATIO;

        /* Ternaria */
        casus SPUTNIK_LEXEMA_INTERROGATIO:
            redde PRAEC_TERNARIA;

        /* Logici */
        casus SPUTNIK_LEXEMA_VEL:
            redde PRAEC_VEL;
        casus SPUTNIK_LEXEMA_ET:
            redde PRAEC_ET;

        /* Aequalitas */
        casus SPUTNIK_LEXEMA_AEQUALIS_DUO:
        casus SPUTNIK_LEXEMA_AEQUALIS_TRIA:
        casus SPUTNIK_LEXEMA_NON_AEQUALIS:
        casus SPUTNIK_LEXEMA_STRICTE_NON_AEQ:
            redde PRAEC_AEQUALITAS;

        /* Comparatio */
        casus SPUTNIK_LEXEMA_MINOR:
        casus SPUTNIK_LEXEMA_MAIOR:
        casus SPUTNIK_LEXEMA_MINOR_AUT:
        casus SPUTNIK_LEXEMA_MAIOR_AUT:
            redde PRAEC_COMPARATIO;

        /* Additio */
        casus SPUTNIK_LEXEMA_PLUS:
        casus SPUTNIK_LEXEMA_MINUS:
            redde PRAEC_ADDITIO;

        /* Multiplicatio */
        casus SPUTNIK_LEXEMA_ASTERISCUS:
        casus SPUTNIK_LEXEMA_DIVISIO:
        casus SPUTNIK_LEXEMA_MODULUS:
            redde PRAEC_MULTIPLICATIO;

        /* Postfix */
        casus SPUTNIK_LEXEMA_PUNCTUM:
        casus SPUTNIK_LEXEMA_QUADRATUM_A:
        casus SPUTNIK_LEXEMA_PARENTHESIS_A:
            redde PRAEC_POSTFIX;

        ordinarius:
            redde ZEPHYRUM;
    }
}

interior b32
_est_dexter_associativum(SputnikLexemaGenus genus)
{
    commutatio (genus)
    {
        casus SPUTNIK_LEXEMA_AEQUALIS:
        casus SPUTNIK_LEXEMA_PLUS_AEQ:
        casus SPUTNIK_LEXEMA_MINUS_AEQ:
        casus SPUTNIK_LEXEMA_ASTERISCUS_AEQ:
        casus SPUTNIK_LEXEMA_DIVISIO_AEQ:
        casus SPUTNIK_LEXEMA_MODULUS_AEQ:
        casus SPUTNIK_LEXEMA_INTERROGATIO:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}


/* ==================================================
 * Prefix Parselets
 * ================================================== */

interior SputnikAstNodus*
_parsere_numerum(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;
    character buffer[LXIV];
    i32 len;
    i32 i;
    i32 j;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        _error(parser, "Expectabatur numerus");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_NUMERUS_LITERALIS);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->valor = lex->valor;

    /* Convertere ad f64, ignorando underscores */
    len = lex->valor.mensura;
    si (len >= LXIV)
    {
        len = LXIII;
    }

    j = ZEPHYRUM;
    per (i = ZEPHYRUM; i < len && j < LXIII; i++)
    {
        si (lex->valor.datum[i] != '_')
        {
            buffer[j++] = (character)lex->valor.datum[i];
        }
    }
    buffer[j] = '\0';

    nodus->numerus = atof(buffer);

    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_chordam(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        _error(parser, "Expectabatur chorda");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_CHORDA_LITERALIS);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Valor includit quotas, ergo sectio interna */
    si (lex->valor.mensura >= II)
    {
        nodus->valor.datum = lex->valor.datum + I;
        nodus->valor.mensura = lex->valor.mensura - II;
    }
    alioquin
    {
        nodus->valor = lex->valor;
    }

    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_verum(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_VERUM_LITERALIS);
    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_falsum(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_FALSUM_LITERALIS);
    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_nihil(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_NIHIL_LITERALIS);
    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_identificatorem(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        _error(parser, "Expectabatur identificator");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_IDENTIFICATOR);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->valor = lex->valor;

    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_signum(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        _error(parser, "Expectabatur signum");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_SIGNUM);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->valor = lex->valor;

    _progredi(parser);
    redde nodus;
}

interior SputnikAstNodus*
_parsere_parenthesin(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    /* Skip ( */
    _progredi(parser);

    nodus = _parsere_expressionem(parser, ZEPHYRUM);

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur )");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_xar_literalem(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* elementum;

    nodus = _creare_nodum(parser, SPUTNIK_AST_XAR_LITERALIS);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip [ */
    _progredi(parser);

    /* Parsere elementa */
    dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_QUADRATUM_C))
    {
        elementum = _parsere_expressionem(parser, ZEPHYRUM);
        si (elementum == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, elementum);

        si (!_consumere(parser, SPUTNIK_LEXEMA_COMMA))
        {
            frange;
        }
    }

    _expectare(parser, SPUTNIK_LEXEMA_QUADRATUM_C, "Expectabatur ]");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_objectum_literalem(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* clavis;
    SputnikAstNodus* valor;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_OBJECTUM_LITERALIS);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip { */
    _progredi(parser);

    /* Parsere paria clavis:valor */
    dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_BRACCHIUM_C))
    {
        /* Clavis - identificator vel chorda */
        lex = _currens(parser);
        si (lex == NIHIL)
        {
            _error(parser, "Expectabatur clavis objecti");
            redde NIHIL;
        }

        si (lex->genus == SPUTNIK_LEXEMA_IDENTIFICATOR)
        {
            clavis = _parsere_identificatorem(parser);
        }
        alioquin si (lex->genus == SPUTNIK_LEXEMA_CHORDA)
        {
            clavis = _parsere_chordam(parser);
        }
        alioquin
        {
            _error(parser, "Clavis objecti debet esse identificator vel chorda");
            redde NIHIL;
        }

        _addere_infantem(nodus, clavis);

        /* Colon */
        _expectare(parser, SPUTNIK_LEXEMA_COLON, "Expectabatur : post clavem");

        /* Valor */
        valor = _parsere_expressionem(parser, ZEPHYRUM);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, valor);

        si (!_consumere(parser, SPUTNIK_LEXEMA_COMMA))
        {
            frange;
        }
    }

    _expectare(parser, SPUTNIK_LEXEMA_BRACCHIUM_C, "Expectabatur }");

    redde nodus;
}

/* ==================================================
 * Arrow Function Parsing
 *
 * Formats:
 *   x => expr
 *   x => { ... }
 *   (x, y) => expr
 *   (x, y) => { ... }
 * ================================================== */

/* Verificare si parenthesis initiat functionem sagittam
 * Scan forward usque ad ) et verificare si => sequitur
 */
interior b32
_est_functio_sagitta_parenthesis(SputnikParser* parser)
{
    i32 offset;
    i32 depth;
    SputnikLexema* lex;

    /* Currens est (, incipiamus ab offset 1 */
    offset = I;
    depth = I;

    dum (depth > ZEPHYRUM)
    {
        lex = _aspicere(parser, offset);
        si (lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS)
        {
            redde FALSUM;
        }

        si (lex->genus == SPUTNIK_LEXEMA_PARENTHESIS_A)
        {
            depth++;
        }
        alioquin si (lex->genus == SPUTNIK_LEXEMA_PARENTHESIS_C)
        {
            depth--;
        }

        offset++;
    }

    /* Nunc offset punctat ad lexema post ) */
    lex = _aspicere(parser, offset);
    redde lex != NIHIL && lex->genus == SPUTNIK_LEXEMA_SAGITTA;
}

/* Parsere functionem sagittam
 * Assumit currentem positum est aut:
 *   - Identificator (x => ...)
 *   - Parenthesis apertam ((x, y) => ...)
 */
interior SputnikAstNodus*
_parsere_functionem_sagittam(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* parametrum;
    SputnikAstNodus* corpus;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_FUNCTIO_SAGITTA);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    lex = _currens(parser);

    /* Forma 1: x => ... (singulum parametrum sine parenthesi) */
    si (lex->genus == SPUTNIK_LEXEMA_IDENTIFICATOR)
    {
        parametrum = _creare_nodum(parser, SPUTNIK_AST_IDENTIFICATOR);
        si (parametrum == NIHIL)
        {
            redde NIHIL;
        }
        parametrum->valor = lex->valor;
        _addere_infantem(nodus, parametrum);
        _progredi(parser);
    }
    /* Forma 2: (x, y) => ... (parametra in parenthesi) */
    alioquin si (lex->genus == SPUTNIK_LEXEMA_PARENTHESIS_A)
    {
        /* Skip ( */
        _progredi(parser);

        /* Parsere parametra */
        dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C))
        {
            lex = _currens(parser);
            si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
            {
                _error(parser, "Expectabatur nomen parametri");
                redde NIHIL;
            }

            parametrum = _creare_nodum(parser, SPUTNIK_AST_IDENTIFICATOR);
            si (parametrum == NIHIL)
            {
                redde NIHIL;
            }
            parametrum->valor = lex->valor;
            _addere_infantem(nodus, parametrum);
            _progredi(parser);

            si (!_consumere(parser, SPUTNIK_LEXEMA_COMMA))
            {
                frange;
            }
        }

        _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur )");
    }
    alioquin
    {
        _error(parser, "Functio sagitta invalida");
        redde NIHIL;
    }

    /* Expectare => */
    _expectare(parser, SPUTNIK_LEXEMA_SAGITTA, "Expectabatur =>");

    /* Corpus - aut block { ... } aut expressio */
    si (_verificare(parser, SPUTNIK_LEXEMA_BRACCHIUM_A))
    {
        /* Block corpus */
        corpus = _parsere_sententiam_grex(parser);
    }
    alioquin
    {
        /* Expressio corpus (implicita redde) */
        corpus = _parsere_expressionem(parser, ZEPHYRUM);
    }

    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, corpus);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_praefixum_unarium(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* operandus;
    SputnikLexema* lex;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        _error(parser, "Expectabatur operator unarius");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_OPERATIO_UNARIA);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->operator = lex->genus;

    /* Skip operator */
    _progredi(parser);

    /* Parsere operandum cum praecedentia unaria */
    operandus = _parsere_expressionem(parser, PRAEC_UNARIA);
    si (operandus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, operandus);

    redde nodus;
}


/* ==================================================
 * Infix Parselets
 * ================================================== */

interior SputnikAstNodus*
_parsere_binarium(SputnikParser* parser, SputnikAstNodus* sinister)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* dexter;
    SputnikLexema* lex;
    i32 praecedentia;
    b32 dexter_assoc;

    lex = _currens(parser);
    si (lex == NIHIL)
    {
        redde sinister;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_OPERATIO_BINARIA);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->operator = lex->genus;
    _addere_infantem(nodus, sinister);

    praecedentia = _praecedentia_infixum(lex->genus);
    dexter_assoc = _est_dexter_associativum(lex->genus);

    /* Skip operator */
    _progredi(parser);

    /* Parsere dextrum cum praecedentia (minore si dexter associativum) */
    dexter = _parsere_expressionem(parser, dexter_assoc ? praecedentia - I : praecedentia);
    si (dexter == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, dexter);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_ternarium(SputnikParser* parser, SputnikAstNodus* conditio)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* verum_ramus;
    SputnikAstNodus* falsum_ramus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_TERNARIA);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, conditio);

    /* Skip ? */
    _progredi(parser);

    /* Parsere verum ramus */
    verum_ramus = _parsere_expressionem(parser, ZEPHYRUM);
    si (verum_ramus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, verum_ramus);

    /* Expectare : */
    _expectare(parser, SPUTNIK_LEXEMA_COLON, "Expectabatur : in ternaria");

    /* Parsere falsum ramus */
    falsum_ramus = _parsere_expressionem(parser, PRAEC_TERNARIA - I);
    si (falsum_ramus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, falsum_ramus);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_assignationem(SputnikParser* parser, SputnikAstNodus* target)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* valor;

    /* Verificare target validum */
    si (target->genus != SPUTNIK_AST_IDENTIFICATOR &&
        target->genus != SPUTNIK_AST_ACCESSUS_MEMBRI &&
        target->genus != SPUTNIK_AST_ACCESSUS_INDICE)
    {
        _error(parser, "Target assignationis invalidum");
        redde NIHIL;
    }

    nodus = _creare_nodum(parser, SPUTNIK_AST_ASSIGNATIO);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, target);

    /* Skip = */
    _progredi(parser);

    /* Parsere valorem (dexter associativum) */
    valor = _parsere_expressionem(parser, PRAEC_ASSIGNATIO - I);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, valor);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_assignationem_complexam(SputnikParser* parser, SputnikAstNodus* target)
{
    SputnikAstNodus* assignatio;
    SputnikAstNodus* binaria;
    SputnikAstNodus* target_copia;
    SputnikAstNodus* valor;
    SputnikLexema* lex;
    SputnikLexemaGenus op_binarius;

    /* Verificare target validum */
    si (target->genus != SPUTNIK_AST_IDENTIFICATOR &&
        target->genus != SPUTNIK_AST_ACCESSUS_MEMBRI &&
        target->genus != SPUTNIK_AST_ACCESSUS_INDICE)
    {
        _error(parser, "Target assignationis invalidum");
        redde NIHIL;
    }

    lex = _currens(parser);

    /* Convertere operator complexus ad operator binarius */
    commutatio (lex->genus)
    {
        casus SPUTNIK_LEXEMA_PLUS_AEQ:
            op_binarius = SPUTNIK_LEXEMA_PLUS;
            frange;
        casus SPUTNIK_LEXEMA_MINUS_AEQ:
            op_binarius = SPUTNIK_LEXEMA_MINUS;
            frange;
        casus SPUTNIK_LEXEMA_ASTERISCUS_AEQ:
            op_binarius = SPUTNIK_LEXEMA_ASTERISCUS;
            frange;
        casus SPUTNIK_LEXEMA_DIVISIO_AEQ:
            op_binarius = SPUTNIK_LEXEMA_DIVISIO;
            frange;
        casus SPUTNIK_LEXEMA_MODULUS_AEQ:
            op_binarius = SPUTNIK_LEXEMA_MODULUS;
            frange;
        ordinarius:
            _error(parser, "Operator assignationis ignotus");
            redde NIHIL;
    }

    /* Skip operator */
    _progredi(parser);

    /* Parsere valorem dextrum */
    valor = _parsere_expressionem(parser, PRAEC_ASSIGNATIO - I);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }

    /* Creare copiam target pro dextro latere
     * Pro identificatore simplex, creare nodum novum cum eodem valore */
    target_copia = _creare_nodum(parser, target->genus);
    si (target_copia == NIHIL)
    {
        redde NIHIL;
    }
    target_copia->valor = target->valor;
    target_copia->operator = target->operator;
    target_copia->numerus = target->numerus;

    /* Creare operationem binariam: target OP valor */
    binaria = _creare_nodum(parser, SPUTNIK_AST_OPERATIO_BINARIA);
    si (binaria == NIHIL)
    {
        redde NIHIL;
    }
    binaria->operator = op_binarius;
    _addere_infantem(binaria, target_copia);
    _addere_infantem(binaria, valor);

    /* Creare assignationem: target = binaria */
    assignatio = _creare_nodum(parser, SPUTNIK_AST_ASSIGNATIO);
    si (assignatio == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(assignatio, target);
    _addere_infantem(assignatio, binaria);

    redde assignatio;
}

interior SputnikAstNodus*
_parsere_vocationem(SputnikParser* parser, SputnikAstNodus* callee)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* argumentum;

    nodus = _creare_nodum(parser, SPUTNIK_AST_VOCATIO);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, callee);

    /* Skip ( */
    _progredi(parser);

    /* Parsere argumenta */
    dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C))
    {
        argumentum = _parsere_expressionem(parser, ZEPHYRUM);
        si (argumentum == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, argumentum);

        si (!_consumere(parser, SPUTNIK_LEXEMA_COMMA))
        {
            frange;
        }
    }

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur )");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_accessum_membri(SputnikParser* parser, SputnikAstNodus* objectum)
{
    SputnikAstNodus* nodus;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_ACCESSUS_MEMBRI);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, objectum);

    /* Skip . */
    _progredi(parser);

    /* Expectare identificatorem */
    lex = _currens(parser);
    si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
    {
        _error(parser, "Expectabatur nomen membri post .");
        redde NIHIL;
    }

    nodus->valor = lex->valor;
    _progredi(parser);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_accessum_indice(SputnikParser* parser, SputnikAstNodus* objectum)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* index;

    nodus = _creare_nodum(parser, SPUTNIK_AST_ACCESSUS_INDICE);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    _addere_infantem(nodus, objectum);

    /* Skip [ */
    _progredi(parser);

    /* Parsere indicem */
    index = _parsere_expressionem(parser, ZEPHYRUM);
    si (index == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, index);

    _expectare(parser, SPUTNIK_LEXEMA_QUADRATUM_C, "Expectabatur ]");

    redde nodus;
}


/* ==================================================
 * Pratt Parser Core
 * ================================================== */

interior SputnikAstNodus*
_parsere_expressionem(SputnikParser* parser, i32 praecedentia)
{
    SputnikAstNodus* sinister;
    SputnikLexema* lex;
    i32 praec_currens;

    si (parser->error_accidit)
    {
        redde NIHIL;
    }

    lex = _currens(parser);
    si (lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS)
    {
        _error(parser, "Expectabatur expressio");
        redde NIHIL;
    }

    /* Skip commenta */
    dum (lex != NIHIL && lex->genus == SPUTNIK_LEXEMA_COMMENTUM)
    {
        _progredi(parser);
        lex = _currens(parser);
    }

    si (lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS)
    {
        _error(parser, "Expectabatur expressio");
        redde NIHIL;
    }

    /* Prefix parselets */
    commutatio (lex->genus)
    {
        casus SPUTNIK_LEXEMA_NUMERUS:
            sinister = _parsere_numerum(parser);
            frange;
        casus SPUTNIK_LEXEMA_CHORDA:
            sinister = _parsere_chordam(parser);
            frange;
        casus SPUTNIK_LEXEMA_VERUM:
            sinister = _parsere_verum(parser);
            frange;
        casus SPUTNIK_LEXEMA_FALSUM:
            sinister = _parsere_falsum(parser);
            frange;
        casus SPUTNIK_LEXEMA_NIHIL:
            sinister = _parsere_nihil(parser);
            frange;
        casus SPUTNIK_LEXEMA_IDENTIFICATOR:
            /* Verificare si arrow function: x => ... */
            {
                SputnikLexema* next;
                next = _aspicere(parser, I);
                si (next != NIHIL && next->genus == SPUTNIK_LEXEMA_SAGITTA)
                {
                    sinister = _parsere_functionem_sagittam(parser);
                }
                alioquin
                {
                    sinister = _parsere_identificatorem(parser);
                }
            }
            frange;
        casus SPUTNIK_LEXEMA_SIGNUM:
            sinister = _parsere_signum(parser);
            frange;
        casus SPUTNIK_LEXEMA_PARENTHESIS_A:
            /* Verificare si arrow function: (x, y) => ... */
            si (_est_functio_sagitta_parenthesis(parser))
            {
                sinister = _parsere_functionem_sagittam(parser);
            }
            alioquin
            {
                sinister = _parsere_parenthesin(parser);
            }
            frange;
        casus SPUTNIK_LEXEMA_QUADRATUM_A:
            sinister = _parsere_xar_literalem(parser);
            frange;
        casus SPUTNIK_LEXEMA_BRACCHIUM_A:
            sinister = _parsere_objectum_literalem(parser);
            frange;
        casus SPUTNIK_LEXEMA_MINUS:
        casus SPUTNIK_LEXEMA_NON:
            sinister = _parsere_praefixum_unarium(parser);
            frange;
        ordinarius:
            _error(parser, "Lexema inexpectatum in expressione");
            redde NIHIL;
    }

    si (sinister == NIHIL || parser->error_accidit)
    {
        redde sinister;
    }

    /* Infix parselets (Pratt loop) */
    dum (!_finis(parser))
    {
        lex = _currens(parser);
        si (lex == NIHIL)
        {
            frange;
        }

        /* Skip commenta */
        si (lex->genus == SPUTNIK_LEXEMA_COMMENTUM)
        {
            _progredi(parser);
            perge;
        }

        praec_currens = _praecedentia_infixum(lex->genus);
        si (praec_currens <= praecedentia)
        {
            frange;
        }

        commutatio (lex->genus)
        {
            /* Operatores binarii */
            casus SPUTNIK_LEXEMA_PLUS:
            casus SPUTNIK_LEXEMA_MINUS:
            casus SPUTNIK_LEXEMA_ASTERISCUS:
            casus SPUTNIK_LEXEMA_DIVISIO:
            casus SPUTNIK_LEXEMA_MODULUS:
            casus SPUTNIK_LEXEMA_AEQUALIS_DUO:
            casus SPUTNIK_LEXEMA_AEQUALIS_TRIA:
            casus SPUTNIK_LEXEMA_NON_AEQUALIS:
            casus SPUTNIK_LEXEMA_STRICTE_NON_AEQ:
            casus SPUTNIK_LEXEMA_MINOR:
            casus SPUTNIK_LEXEMA_MAIOR:
            casus SPUTNIK_LEXEMA_MINOR_AUT:
            casus SPUTNIK_LEXEMA_MAIOR_AUT:
            casus SPUTNIK_LEXEMA_ET:
            casus SPUTNIK_LEXEMA_VEL:
                sinister = _parsere_binarium(parser, sinister);
                frange;

            /* Ternaria */
            casus SPUTNIK_LEXEMA_INTERROGATIO:
                sinister = _parsere_ternarium(parser, sinister);
                frange;

            /* Assignatio */
            casus SPUTNIK_LEXEMA_AEQUALIS:
                sinister = _parsere_assignationem(parser, sinister);
                frange;

            /* Assignatio Complexa */
            casus SPUTNIK_LEXEMA_PLUS_AEQ:
            casus SPUTNIK_LEXEMA_MINUS_AEQ:
            casus SPUTNIK_LEXEMA_ASTERISCUS_AEQ:
            casus SPUTNIK_LEXEMA_DIVISIO_AEQ:
            casus SPUTNIK_LEXEMA_MODULUS_AEQ:
                sinister = _parsere_assignationem_complexam(parser, sinister);
                frange;

            /* Vocatio */
            casus SPUTNIK_LEXEMA_PARENTHESIS_A:
                sinister = _parsere_vocationem(parser, sinister);
                frange;

            /* Accessus membri */
            casus SPUTNIK_LEXEMA_PUNCTUM:
                sinister = _parsere_accessum_membri(parser, sinister);
                frange;

            /* Accessus indice */
            casus SPUTNIK_LEXEMA_QUADRATUM_A:
                sinister = _parsere_accessum_indice(parser, sinister);
                frange;

            ordinarius:
                /* Finis expressionis */
                redde sinister;
        }

        si (sinister == NIHIL || parser->error_accidit)
        {
            redde sinister;
        }
    }

    redde sinister;
}


/* ==================================================
 * Statement Parsing
 * ================================================== */

interior SputnikAstNodus*
_parsere_declarationem_let(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* valor;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_DECLARATIO_LET);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'let' */
    _progredi(parser);

    /* Expectare identificatorem */
    lex = _currens(parser);
    si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
    {
        _error(parser, "Expectabatur nomen variabilis post 'let'");
        redde NIHIL;
    }

    nodus->valor = lex->valor;
    _progredi(parser);

    /* Optionalis initializer */
    si (_consumere(parser, SPUTNIK_LEXEMA_AEQUALIS))
    {
        valor = _parsere_expressionem(parser, ZEPHYRUM);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, valor);
    }

    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post declaratione");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_declarationem_const(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* valor;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_DECLARATIO_CONST);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'const' */
    _progredi(parser);

    /* Expectare identificatorem */
    lex = _currens(parser);
    si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
    {
        _error(parser, "Expectabatur nomen variabilis post 'const'");
        redde NIHIL;
    }

    nodus->valor = lex->valor;
    _progredi(parser);

    /* Const requirit initializer */
    _expectare(parser, SPUTNIK_LEXEMA_AEQUALIS, "Expectabatur = post nomen const");

    valor = _parsere_expressionem(parser, ZEPHYRUM);
    si (valor == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, valor);

    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post declaratione");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_si(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* conditio;
    SputnikAstNodus* tunc;
    SputnikAstNodus* alioquin_ramus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_SI);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'if' */
    _progredi(parser);

    /* Conditio in parenthesi */
    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_A, "Expectabatur ( post 'if'");

    conditio = _parsere_expressionem(parser, ZEPHYRUM);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, conditio);

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur ) post conditione");

    /* Corpus tunc */
    tunc = _parsere_sententiam(parser);
    si (tunc == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, tunc);

    /* Optionalis else */
    si (_consumere(parser, SPUTNIK_LEXEMA_ALIOQUIN))
    {
        alioquin_ramus = _parsere_sententiam(parser);
        si (alioquin_ramus == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, alioquin_ramus);
    }

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_dum(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* conditio;
    SputnikAstNodus* corpus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_DUM);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'while' */
    _progredi(parser);

    /* Conditio in parenthesi */
    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_A, "Expectabatur ( post 'while'");

    conditio = _parsere_expressionem(parser, ZEPHYRUM);
    si (conditio == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, conditio);

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur ) post conditione");

    /* Corpus */
    corpus = _parsere_sententiam(parser);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, corpus);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_per(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* init;
    SputnikAstNodus* conditio;
    SputnikAstNodus* incrementum;
    SputnikAstNodus* corpus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_PER);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'for' */
    _progredi(parser);

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_A, "Expectabatur ( post 'for'");

    /* Initializatio (optionalis) */
    si (_verificare(parser, SPUTNIK_LEXEMA_LET))
    {
        init = _parsere_declarationem_let(parser);
    }
    alioquin si (_verificare(parser, SPUTNIK_LEXEMA_CONST))
    {
        init = _parsere_declarationem_const(parser);
    }
    alioquin si (!_verificare(parser, SPUTNIK_LEXEMA_SEMICOLON))
    {
        init = _parsere_expressionem(parser, ZEPHYRUM);
        _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ;");
    }
    alioquin
    {
        init = NIHIL;
        _progredi(parser);  /* Skip ; */
    }
    _addere_infantem(nodus, init);

    /* Conditio (optionalis) */
    si (!_verificare(parser, SPUTNIK_LEXEMA_SEMICOLON))
    {
        conditio = _parsere_expressionem(parser, ZEPHYRUM);
    }
    alioquin
    {
        conditio = NIHIL;
    }
    _addere_infantem(nodus, conditio);
    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ;");

    /* Incrementum (optionalis) - supportare i++ / i-- */
    si (!_verificare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C))
    {
        incrementum = _parsere_expressionem(parser, ZEPHYRUM);

        /* Verificare si postfix ++ vel -- */
        {
            SputnikLexema* lex_inc;
            lex_inc = _currens(parser);
            si (lex_inc != NIHIL && lex_inc->genus == SPUTNIK_LEXEMA_INCREMENT)
            {
                SputnikAstNodus* inc_nodus;
                inc_nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_INCREMENT);
                _addere_infantem(inc_nodus, incrementum);
                incrementum = inc_nodus;
                _progredi(parser);
            }
            alioquin si (lex_inc != NIHIL && lex_inc->genus == SPUTNIK_LEXEMA_DECREMENT)
            {
                SputnikAstNodus* dec_nodus;
                dec_nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_DECREMENT);
                _addere_infantem(dec_nodus, incrementum);
                incrementum = dec_nodus;
                _progredi(parser);
            }
        }
    }
    alioquin
    {
        incrementum = NIHIL;
    }
    _addere_infantem(nodus, incrementum);

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur )");

    /* Corpus */
    corpus = _parsere_sententiam(parser);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, corpus);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_redde(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* valor;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_REDDE);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'return' */
    _progredi(parser);

    /* Optionalis valor */
    si (!_verificare(parser, SPUTNIK_LEXEMA_SEMICOLON))
    {
        valor = _parsere_expressionem(parser, ZEPHYRUM);
        si (valor == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, valor);
    }

    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post 'return'");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_grex(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* sententia;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_GREX);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip { */
    _progredi(parser);

    /* Parsere sententias usque ad } */
    dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_BRACCHIUM_C))
    {
        sententia = _parsere_sententiam(parser);
        si (sententia == NIHIL)
        {
            redde NIHIL;
        }
        _addere_infantem(nodus, sententia);
    }

    _expectare(parser, SPUTNIK_LEXEMA_BRACCHIUM_C, "Expectabatur }");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_declarationem_functio(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* parametrum;
    SputnikAstNodus* corpus;
    SputnikLexema* lex;

    nodus = _creare_nodum(parser, SPUTNIK_AST_DECLARATIO_FUNCTIO);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'function' */
    _progredi(parser);

    /* Nomen functionis */
    lex = _currens(parser);
    si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
    {
        _error(parser, "Expectabatur nomen functionis");
        redde NIHIL;
    }

    nodus->valor = lex->valor;
    _progredi(parser);

    /* Parametra */
    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_A, "Expectabatur ( post nomen functionis");

    dum (!_finis(parser) && !_verificare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C))
    {
        lex = _currens(parser);
        si (lex == NIHIL || lex->genus != SPUTNIK_LEXEMA_IDENTIFICATOR)
        {
            _error(parser, "Expectabatur nomen parametri");
            redde NIHIL;
        }

        parametrum = _creare_nodum(parser, SPUTNIK_AST_IDENTIFICATOR);
        si (parametrum == NIHIL)
        {
            redde NIHIL;
        }
        parametrum->valor = lex->valor;
        _progredi(parser);

        _addere_infantem(nodus, parametrum);

        si (!_consumere(parser, SPUTNIK_LEXEMA_COMMA))
        {
            frange;
        }
    }

    _expectare(parser, SPUTNIK_LEXEMA_PARENTHESIS_C, "Expectabatur )");

    /* Corpus */
    si (!_verificare(parser, SPUTNIK_LEXEMA_BRACCHIUM_A))
    {
        _error(parser, "Expectabatur { post parametris functionis");
        redde NIHIL;
    }

    corpus = _parsere_sententiam_grex(parser);
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    _addere_infantem(nodus, corpus);

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_frange(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_FRANGE);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'frange' */
    _progredi(parser);

    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post 'frange'");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam_perge(SputnikParser* parser)
{
    SputnikAstNodus* nodus;

    nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_PERGE);
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    /* Skip 'perge' */
    _progredi(parser);

    _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post 'perge'");

    redde nodus;
}

interior SputnikAstNodus*
_parsere_sententiam(SputnikParser* parser)
{
    SputnikAstNodus* nodus;
    SputnikAstNodus* expressio;
    SputnikLexema* lex;

    si (parser->error_accidit)
    {
        redde NIHIL;
    }

    lex = _currens(parser);
    si (lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS)
    {
        redde NIHIL;
    }

    /* Skip commenta */
    dum (lex != NIHIL && lex->genus == SPUTNIK_LEXEMA_COMMENTUM)
    {
        _progredi(parser);
        lex = _currens(parser);
    }

    si (lex == NIHIL || lex->genus == SPUTNIK_LEXEMA_FINIS)
    {
        redde NIHIL;
    }

    commutatio (lex->genus)
    {
        casus SPUTNIK_LEXEMA_LET:
            redde _parsere_declarationem_let(parser);

        casus SPUTNIK_LEXEMA_CONST:
            redde _parsere_declarationem_const(parser);

        casus SPUTNIK_LEXEMA_SI:
            redde _parsere_sententiam_si(parser);

        casus SPUTNIK_LEXEMA_DUM:
            redde _parsere_sententiam_dum(parser);

        casus SPUTNIK_LEXEMA_PER:
            redde _parsere_sententiam_per(parser);

        casus SPUTNIK_LEXEMA_REDDE:
            redde _parsere_sententiam_redde(parser);

        casus SPUTNIK_LEXEMA_BRACCHIUM_A:
            redde _parsere_sententiam_grex(parser);

        casus SPUTNIK_LEXEMA_FUNCTIO:
            redde _parsere_declarationem_functio(parser);

        casus SPUTNIK_LEXEMA_FRANGE:
            redde _parsere_sententiam_frange(parser);

        casus SPUTNIK_LEXEMA_PERGE:
            redde _parsere_sententiam_perge(parser);

        ordinarius:
            /* Expression statement */
            expressio = _parsere_expressionem(parser, ZEPHYRUM);
            si (expressio == NIHIL)
            {
                redde NIHIL;
            }

            /* Verificare si postfix increment/decrement */
            lex = _currens(parser);
            si (lex != NIHIL && lex->genus == SPUTNIK_LEXEMA_INCREMENT)
            {
                si (expressio->genus != SPUTNIK_AST_IDENTIFICATOR &&
                    expressio->genus != SPUTNIK_AST_ACCESSUS_MEMBRI &&
                    expressio->genus != SPUTNIK_AST_ACCESSUS_INDICE)
                {
                    _error(parser, "++ requirit variabilem");
                    redde NIHIL;
                }
                nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_INCREMENT);
                si (nodus == NIHIL)
                {
                    redde NIHIL;
                }
                _addere_infantem(nodus, expressio);
                _progredi(parser);  /* Skip ++ */
                _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post ++");
                redde nodus;
            }
            alioquin si (lex != NIHIL && lex->genus == SPUTNIK_LEXEMA_DECREMENT)
            {
                si (expressio->genus != SPUTNIK_AST_IDENTIFICATOR &&
                    expressio->genus != SPUTNIK_AST_ACCESSUS_MEMBRI &&
                    expressio->genus != SPUTNIK_AST_ACCESSUS_INDICE)
                {
                    _error(parser, "-- requirit variabilem");
                    redde NIHIL;
                }
                nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_DECREMENT);
                si (nodus == NIHIL)
                {
                    redde NIHIL;
                }
                _addere_infantem(nodus, expressio);
                _progredi(parser);  /* Skip -- */
                _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post --");
                redde nodus;
            }

            nodus = _creare_nodum(parser, SPUTNIK_AST_SENTENTIA_EXPRESSIO);
            si (nodus == NIHIL)
            {
                redde NIHIL;
            }
            _addere_infantem(nodus, expressio);

            _expectare(parser, SPUTNIK_LEXEMA_SEMICOLON, "Expectabatur ; post expressione");

            redde nodus;
    }
}


/* ==================================================
 * Program Parsing
 * ================================================== */

interior SputnikAstNodus*
_parsere_programma(SputnikParser* parser)
{
    SputnikAstNodus* programma;
    SputnikAstNodus* sententia;

    programma = _creare_nodum(parser, SPUTNIK_AST_PROGRAMMA);
    si (programma == NIHIL)
    {
        redde NIHIL;
    }

    dum (!_finis(parser) && !parser->error_accidit)
    {
        sententia = _parsere_sententiam(parser);
        si (sententia == NIHIL)
        {
            si (parser->error_accidit)
            {
                redde NIHIL;
            }
            frange;
        }
        _addere_infantem(programma, sententia);
    }

    redde programma;
}


/* ==================================================
 * Functiones Principales
 * ================================================== */

SputnikParserResultus
sputnik_parsere(
    Xar*                 lexemata,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikParserResultus resultus;
    SputnikParser parser;

    resultus.successus = FALSUM;
    resultus.radix = NIHIL;
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    si (lexemata == NIHIL || piscina == NIHIL)
    {
        redde resultus;
    }

    /* Initializare parser */
    parser.lexemata = lexemata;
    parser.positus = ZEPHYRUM;
    parser.piscina = piscina;
    parser.intern = intern;
    parser.error_accidit = FALSUM;
    parser.error_nuntius.datum = NIHIL;
    parser.error_nuntius.mensura = ZEPHYRUM;
    parser.error_linea = ZEPHYRUM;
    parser.error_columna = ZEPHYRUM;

    /* Parsere */
    resultus.radix = _parsere_programma(&parser);

    si (parser.error_accidit)
    {
        resultus.error_nuntius = parser.error_nuntius;
        resultus.error_linea = parser.error_linea;
        resultus.error_columna = parser.error_columna;
        redde resultus;
    }

    resultus.successus = VERUM;
    redde resultus;
}

SputnikParserResultus
sputnik_parsere_ex_fonte(
    chorda               fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SputnikLexatorResultus lex_res;
    SputnikParserResultus resultus;

    resultus.successus = FALSUM;
    resultus.radix = NIHIL;
    resultus.error_nuntius.datum = NIHIL;
    resultus.error_nuntius.mensura = ZEPHYRUM;
    resultus.error_linea = ZEPHYRUM;
    resultus.error_columna = ZEPHYRUM;

    /* Lexicare */
    lex_res = sputnik_lexicare(fons, piscina, intern);
    si (!lex_res.successus)
    {
        resultus.error_nuntius = chorda_ex_literis("Lexer error", piscina);
        resultus.error_linea = lex_res.error_linea;
        resultus.error_columna = lex_res.error_columna;
        redde resultus;
    }

    /* Parsere */
    redde sputnik_parsere(lex_res.lexemata, piscina, intern);
}

SputnikParserResultus
sputnik_parsere_ex_literis(
    constans character*  fons,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda ch;
    ch = chorda_ex_literis(fons, piscina);
    redde sputnik_parsere_ex_fonte(ch, piscina, intern);
}


/* ==================================================
 * Utilitas
 * ================================================== */

constans character*
sputnik_ast_genus_nomen(SputnikAstGenus genus)
{
    commutatio (genus)
    {
        casus SPUTNIK_AST_NUMERUS_LITERALIS:    redde "NUMERUS_LITERALIS";
        casus SPUTNIK_AST_CHORDA_LITERALIS:     redde "CHORDA_LITERALIS";
        casus SPUTNIK_AST_VERUM_LITERALIS:      redde "VERUM_LITERALIS";
        casus SPUTNIK_AST_FALSUM_LITERALIS:     redde "FALSUM_LITERALIS";
        casus SPUTNIK_AST_NIHIL_LITERALIS:      redde "NIHIL_LITERALIS";
        casus SPUTNIK_AST_IDENTIFICATOR:        redde "IDENTIFICATOR";
        casus SPUTNIK_AST_SIGNUM:               redde "SIGNUM";
        casus SPUTNIK_AST_OPERATIO_BINARIA:     redde "OPERATIO_BINARIA";
        casus SPUTNIK_AST_OPERATIO_UNARIA:      redde "OPERATIO_UNARIA";
        casus SPUTNIK_AST_TERNARIA:             redde "TERNARIA";
        casus SPUTNIK_AST_VOCATIO:              redde "VOCATIO";
        casus SPUTNIK_AST_ACCESSUS_MEMBRI:      redde "ACCESSUS_MEMBRI";
        casus SPUTNIK_AST_ACCESSUS_INDICE:      redde "ACCESSUS_INDICE";
        casus SPUTNIK_AST_ASSIGNATIO:           redde "ASSIGNATIO";
        casus SPUTNIK_AST_XAR_LITERALIS:        redde "XAR_LITERALIS";
        casus SPUTNIK_AST_OBJECTUM_LITERALIS:   redde "OBJECTUM_LITERALIS";
        casus SPUTNIK_AST_DECLARATIO_LET:       redde "DECLARATIO_LET";
        casus SPUTNIK_AST_DECLARATIO_CONST:     redde "DECLARATIO_CONST";
        casus SPUTNIK_AST_SENTENTIA_SI:         redde "SENTENTIA_SI";
        casus SPUTNIK_AST_SENTENTIA_DUM:        redde "SENTENTIA_DUM";
        casus SPUTNIK_AST_SENTENTIA_PER:        redde "SENTENTIA_PER";
        casus SPUTNIK_AST_SENTENTIA_REDDE:      redde "SENTENTIA_REDDE";
        casus SPUTNIK_AST_SENTENTIA_EXPRESSIO:  redde "SENTENTIA_EXPRESSIO";
        casus SPUTNIK_AST_SENTENTIA_GREX:       redde "SENTENTIA_GREX";
        casus SPUTNIK_AST_DECLARATIO_FUNCTIO:   redde "DECLARATIO_FUNCTIO";
        casus SPUTNIK_AST_PROGRAMMA:            redde "PROGRAMMA";
        casus SPUTNIK_AST_SENTENTIA_FRANGE:     redde "SENTENTIA_FRANGE";
        casus SPUTNIK_AST_SENTENTIA_PERGE:      redde "SENTENTIA_PERGE";
        casus SPUTNIK_AST_SENTENTIA_INCREMENT:  redde "SENTENTIA_INCREMENT";
        casus SPUTNIK_AST_SENTENTIA_DECREMENT:  redde "SENTENTIA_DECREMENT";
        casus SPUTNIK_AST_FUNCTIO_SAGITTA:      redde "FUNCTIO_SAGITTA";
        ordinarius:                             redde "IGNOTUM";
    }
}

vacuum
sputnik_ast_imprimere(SputnikAstNodus* nodus, i32 indentatio)
{
    i32 i;
    i32 num_liberi;
    SputnikAstNodus* infans;

    si (nodus == NIHIL)
    {
        per (i = ZEPHYRUM; i < indentatio; i++)
        {
            imprimere("  ");
        }
        imprimere("(nihil)\n");
        redde;
    }

    /* Indentatio */
    per (i = ZEPHYRUM; i < indentatio; i++)
    {
        imprimere("  ");
    }

    /* Genus */
    imprimere("%s", sputnik_ast_genus_nomen(nodus->genus));

    /* Valor si praesens */
    si (nodus->valor.mensura > ZEPHYRUM)
    {
        imprimere(" \"%.*s\"", nodus->valor.mensura, nodus->valor.datum);
    }

    /* Numerus si NUMERUS_LITERALIS */
    si (nodus->genus == SPUTNIK_AST_NUMERUS_LITERALIS)
    {
        imprimere(" = %g", nodus->numerus);
    }

    /* Operator si OPERATIO */
    si (nodus->genus == SPUTNIK_AST_OPERATIO_BINARIA ||
        nodus->genus == SPUTNIK_AST_OPERATIO_UNARIA)
    {
        imprimere(" [%s]", sputnik_lexema_genus_nomen(nodus->operator));
    }

    imprimere("\n");

    /* Liberi */
    si (nodus->liberi != NIHIL)
    {
        num_liberi = xar_numerus(nodus->liberi);
        per (i = ZEPHYRUM; i < num_liberi; i++)
        {
            infans = *(SputnikAstNodus**)xar_obtinere(nodus->liberi, i);
            sputnik_ast_imprimere(infans, indentatio + I);
        }
    }
}
