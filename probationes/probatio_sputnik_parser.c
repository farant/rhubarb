/* probatio_sputnik_parser.c - Probationes pro Sputnik Parser */

#include "../include/sputnik_parser.h"
#include "../include/credo.h"
#include <stdio.h>
#include <string.h>

/* Commoditas macros */
#define CREDO(x) CREDO_VERUM(x)
#define CREDO_AEQUALIS_F64(a, b) CREDO_F64_PROXIMUS(a, b, 0.0001)


/* ==================================================
 * Adiutores
 * ================================================== */

interior SputnikAstNodus*
_obtinere_infantem (
    SputnikAstNodus* nodus,
                i32  index)
{
    si (nodus == NIHIL || nodus->liberi == NIHIL)
    {
        redde NIHIL;
    }
    si (index >= xar_numerus(nodus->liberi))
    {
        redde NIHIL;
    }
    redde *(SputnikAstNodus**)xar_obtinere(nodus->liberi, index);
}

interior i32
_numerus_infantium (
    SputnikAstNodus* nodus)
{
    si (nodus == NIHIL || nodus->liberi == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(nodus->liberi);
}

interior SputnikAstNodus*
_prima_sententia (
    SputnikAstNodus* programma)
{
    redde _obtinere_infantem(programma, ZEPHYRUM);
}

interior SputnikAstNodus*
_expressio_ex_sententia (
    SputnikAstNodus* sententia)
{
    /* Pro SENTENTIA_EXPRESSIO, expressio est in liberi[0] */
    redde _obtinere_infantem(sententia, ZEPHYRUM);
}


/* ==================================================
 * Probationes: Literalia
 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Numerus integer */
    {
        r = sputnik_parsere_ex_literis("42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO(sent != NIHIL);
        CREDO_AEQUALIS_I32(sent->genus,
            SPUTNIK_AST_SENTENTIA_EXPRESSIO);
        expr = _expressio_ex_sententia(sent);
        CREDO(expr != NIHIL);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_NUMERUS_LITERALIS);
        CREDO_AEQUALIS_F64(expr->numerus, 42.0);
    }

    /* Numerus decimalis */
    {
        r = sputnik_parsere_ex_literis("3.14;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_NUMERUS_LITERALIS);
        CREDO_AEQUALIS_F64(expr->numerus, 3.14);
    }

    /* Numerus cum underscore */
    {
        r = sputnik_parsere_ex_literis("1_000_000;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_NUMERUS_LITERALIS);
        CREDO_AEQUALIS_F64(expr->numerus, 1000000.0);
    }

    /* Chorda */
    {
        r = sputnik_parsere_ex_literis("\"hello\";", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_CHORDA_LITERALIS);
        CREDO_AEQUALIS_I32(expr->valor.mensura, V);
    }

    /* Verum */
    {
        r = sputnik_parsere_ex_literis("verum;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VERUM_LITERALIS);
    }

    /* Falsum */
    {
        r = sputnik_parsere_ex_literis("falsum;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_FALSUM_LITERALIS);
    }

    /* Nihil */
    {
        r = sputnik_parsere_ex_literis("nihil;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_NIHIL_LITERALIS);
    }

    imprimere("  Literalia: VERUM\n");
}


/* ==================================================
 * Probationes: Identificatores
 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Identificator simplex */
    {
        r = sputnik_parsere_ex_literis("foo;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(expr->valor.mensura, III);
    }

    /* Identificator kebab-case */
    {
        r = sputnik_parsere_ex_literis("foo-bar;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(expr->valor.mensura, VII);  /* "foo-bar" */
    }

    /* Identificator x-1 */
    {
        r = sputnik_parsere_ex_literis("x-1;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(expr->valor.mensura, III);  /* "x-1" */
    }

    /* Signum */
    {
        r = sputnik_parsere_ex_literis("#tag;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_SIGNUM);
    }

    imprimere("  Identificatores: VERUM\n");
}


/* ==================================================
 * Probationes: Operationes Binariae
 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;
          SputnikAstNodus* sin;
          SputnikAstNodus* dex;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Additio simplex */
    {
        r = sputnik_parsere_ex_literis("1 + 2;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_PLUS);
        sin = _obtinere_infantem(expr, ZEPHYRUM);
        dex = _obtinere_infantem(expr, I);
        CREDO_AEQUALIS_F64(sin->numerus, 1.0);
        CREDO_AEQUALIS_F64(dex->numerus, 2.0);
    }

    /* Subtractio (cum spatiis) */
    {
        r = sputnik_parsere_ex_literis("x - y;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_MINUS);
    }

    /* Multiplicatio */
    {
        r = sputnik_parsere_ex_literis("3 * 4;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_ASTERISCUS);
    }

    /* Comparatio */
    {
        r = sputnik_parsere_ex_literis("a == b;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_AEQUALIS_DUO);
    }

    /* Logicus */
    {
        r = sputnik_parsere_ex_literis("a && b;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_ET);
    }

    imprimere("  Operationes Binariae: VERUM\n");
}


/* ==================================================
 * Probationes: Praecedentia
 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;
          SputnikAstNodus* dex;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* 1 + 2 * 3 => 1 + (2 * 3) */
    {
        r = sputnik_parsere_ex_literis("1 + 2 * 3;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);

        /* Radix debet esse + */
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_PLUS);

        /* Dexter debet esse * */
        dex = _obtinere_infantem(expr, I);
        CREDO(dex != NIHIL);
        CREDO_AEQUALIS_I32(dex->genus, SPUTNIK_AST_OPERATIO_BINARIA);
        CREDO_AEQUALIS_I32(dex->operator, SPUTNIK_LEXEMA_ASTERISCUS);
    }

    /* 1 * 2 + 3 => (1 * 2) + 3 */
    {
        SputnikAstNodus* sin_tmp;
        r = sputnik_parsere_ex_literis("1 * 2 + 3;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);

        /* Radix debet esse + */
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_PLUS);

        /* Sinister debet esse * */
        sin_tmp = _obtinere_infantem(expr, ZEPHYRUM);
        CREDO_AEQUALIS_I32(sin_tmp->operator,
            SPUTNIK_LEXEMA_ASTERISCUS);
    }

    /* a == b && c => (a == b) && c */
    {
        r = sputnik_parsere_ex_literis("a == b && c;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);

        /* Radix debet esse && */
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_ET);
    }

    imprimere("  Praecedentia: VERUM\n");
}


/* ==================================================
 * Probationes: Operationes Unariae
 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;
          SputnikAstNodus* operandus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Negatio */
    {
        r = sputnik_parsere_ex_literis("-42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_UNARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_MINUS);
        operandus = _obtinere_infantem(expr, ZEPHYRUM);
        CREDO_AEQUALIS_F64(operandus->numerus, 42.0);
    }

    /* Logica negatio */
    {
        r = sputnik_parsere_ex_literis("!verum;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OPERATIO_UNARIA);
        CREDO_AEQUALIS_I32(expr->operator, SPUTNIK_LEXEMA_NON);
    }

    imprimere("  Operationes Unariae: VERUM\n");
}


/* ==================================================
 * Probationes: Ternaria
 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;
          SputnikAstNodus* conditio;
          SputnikAstNodus* verum_ramus;
          SputnikAstNodus* falsum_ramus;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* a ? b : c */
    {
        r = sputnik_parsere_ex_literis("a ? b : c;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_TERNARIA);

        conditio      = _obtinere_infantem(expr, ZEPHYRUM);
        verum_ramus   = _obtinere_infantem(expr, I);
        falsum_ramus  = _obtinere_infantem(expr, II);

        CREDO(conditio != NIHIL);
        CREDO(verum_ramus != NIHIL);
        CREDO(falsum_ramus != NIHIL);

        CREDO_AEQUALIS_I32(conditio->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(verum_ramus->genus,
            SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(falsum_ramus->genus,
            SPUTNIK_AST_IDENTIFICATOR);
    }

    /* Ternaria in conditione: x > 0 ? 1 : -1 */
    {
        r = sputnik_parsere_ex_literis("x > 0 ? 1 : 0 - 1;", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_TERNARIA);
    }

    imprimere("  Ternaria: VERUM\n");
}


/* ==================================================
 * Probationes: Accessus
 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Accessus membri */
    {
        r = sputnik_parsere_ex_literis("obj.prop;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_ACCESSUS_MEMBRI);
        CREDO_AEQUALIS_I32(expr->valor.mensura, IV);  /* "prop" */
    }

    /* Accessus indice */
    {
        r = sputnik_parsere_ex_literis("arr[0];", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_ACCESSUS_INDICE);
    }

    /* Accessus concatenatus */
    {
        SputnikAstNodus* ab;
        r = sputnik_parsere_ex_literis("a.b.c;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_ACCESSUS_MEMBRI);
        /* a.b debet esse in infante */
        ab = _obtinere_infantem(expr, ZEPHYRUM);
        CREDO_AEQUALIS_I32(ab->genus, SPUTNIK_AST_ACCESSUS_MEMBRI);
    }

    imprimere("  Accessus: VERUM\n");
}


/* ==================================================
 * Probationes: Vocatio
 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Vocatio sine argumentis */
    {
        r = sputnik_parsere_ex_literis("foo();", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VOCATIO);
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), I);  /* solum callee */
    }

    /* Vocatio cum argumentis */
    {
        r = sputnik_parsere_ex_literis("foo(1, 2, 3);", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VOCATIO);
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), IV);  /* callee + 3 args */
    }

    /* Vocatio methodi */
    {
        SputnikAstNodus* callee;
        r = sputnik_parsere_ex_literis("obj.method();", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VOCATIO);
        callee = _obtinere_infantem(expr, ZEPHYRUM);
        CREDO_AEQUALIS_I32(callee->genus, SPUTNIK_AST_ACCESSUS_MEMBRI);
    }

    imprimere("  Vocatio: VERUM\n");
}


/* ==================================================
 * Probationes: Literalia Composita
 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* expr;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Xar vacuum */
    {
        r = sputnik_parsere_ex_literis("[];", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_XAR_LITERALIS);
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), ZEPHYRUM);
    }

    /* Xar cum elementis */
    {
        r = sputnik_parsere_ex_literis("[1, 2, 3];", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_XAR_LITERALIS);
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), III);
    }

    /* Objectum vacuum - in assignatione */
    {
        r = sputnik_parsere_ex_literis("sit x = {};", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        expr = _obtinere_infantem(sent, ZEPHYRUM);  /* valor */
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OBJECTUM_LITERALIS);
    }

    /* Objectum cum proprietatibus */
    {
        r = sputnik_parsere_ex_literis("sit x = {a: 1, b: 2};", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _obtinere_infantem(sent, ZEPHYRUM);  /* valor */
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OBJECTUM_LITERALIS);
        /* 2 paria = 4 infantes (key, val, key, val) */
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), IV);
    }

    imprimere("  Literalia Composita: VERUM\n");
}


/* ==================================================
 * Probationes: Declarationes
 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;
          SputnikAstNodus* valor;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Sit sine valore */
    {
        r = sputnik_parsere_ex_literis("sit x;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        CREDO_AEQUALIS_I32(sent->valor.mensura, I);  /* "x" */
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), ZEPHYRUM);
    }

    /* Sit cum valore */
    {
        r = sputnik_parsere_ex_literis("sit x = 42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
        valor = _obtinere_infantem(sent, ZEPHYRUM);
        CREDO_AEQUALIS_F64(valor->numerus, 42.0);
    }

    /* Constans (requirit valorem) */
    {
        r = sputnik_parsere_ex_literis("constans PI = 3.14;", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_CONST);
        CREDO_AEQUALIS_I32(sent->valor.mensura, II);  /* "PI" */
    }

    imprimere("  Declarationes: VERUM\n");
}


/* ==================================================
 * Probationes: Control Flow
 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Si simplex */
    {
        r = sputnik_parsere_ex_literis("si (x) { y; }", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_SI);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), II);  /* conditio + tunc */
    }

    /* Si/alioquin */
    {
        r = sputnik_parsere_ex_literis("si (x) { y; } alioquin { z; }",
            piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_SI);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), III);  /* conditio + tunc + alioquin */
    }

    /* Dum */
    {
        r = sputnik_parsere_ex_literis("dum (x) { y; }", piscina,
            intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_DUM);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), II);  /* conditio + corpus */
    }

    /* Per */
    {
        r =
            sputnik_parsere_ex_literis("per (sit i = 0; i < 10; i = i + 1) { x; }",
            piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_PER);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), IV);  /* init + cond + incr + corpus */
    }

    /* Redde */
    {
        r = sputnik_parsere_ex_literis("redde 42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_REDDE);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
    }

    /* Redde sine valore */
    {
        r = sputnik_parsere_ex_literis("redde;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_REDDE);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), ZEPHYRUM);
    }

    imprimere("  Control Flow: VERUM\n");
}


/* ==================================================
 * Probationes: Functiones
 * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
          SputnikAstNodus* sent;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Functio sine parametris */
    {
        r = sputnik_parsere_ex_literis("functio foo() { redde 42; }",
            piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_FUNCTIO);
        CREDO_AEQUALIS_I32(sent->valor.mensura, III);  /* "foo" */
        /* Solum corpus (grex) */
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
    }

    /* Functio cum parametris */
    {
        r =
            sputnik_parsere_ex_literis("functio add(a, b) { redde a + b; }",
            piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_FUNCTIO);
        /* 2 parametra + corpus = 3 infantes */
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), III);
    }

    imprimere("  Functiones: VERUM\n");
}


/* ==================================================
 * Probationes: Programma Complexum
 * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;
       constans character* programma =
           "functio factorial(n) {\n"
           "    si (n <= 1) {\n"
           "        redde 1;\n"
           "    }\n"
           "    redde n * factorial(n - 1);\n"
           "}\n"
           "\n"
           "sit result = factorial(5);\n";

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    r = sputnik_parsere_ex_literis(programma, piscina, intern);
    CREDO(r.successus);
    CREDO(r.radix != NIHIL);
    CREDO_AEQUALIS_I32(r.radix->genus, SPUTNIK_AST_PROGRAMMA);
    CREDO_AEQUALIS_I32(_numerus_infantium(r.radix), II);  /* functio + let */

    imprimere("  Programma Complexum: VERUM\n");
}


/* ==================================================
 * Probationes: Errores
 * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
                  Piscina* piscina;
      InternamentumChorda* intern;
    SputnikParserResultus  r;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);

    /* Semicolon deest */
    {
        r = sputnik_parsere_ex_literis("sit x = 1", piscina, intern);
        CREDO(!r.successus);
    }

    /* Parenthesis deest */
    {
        r = sputnik_parsere_ex_literis("si x { y; }", piscina, intern);
        CREDO(!r.successus);
    }

    /* Bracchium deest */
    {
        r = sputnik_parsere_ex_literis("functio foo() redde 1; }",
            piscina, intern);
        CREDO(!r.successus);
    }

    imprimere("  Errores: VERUM\n");
}


/* ==================================================
 * Principale
 * ================================================== */

hic_manens constans CredoSectio SECTIONES[] = {
    { "Literalia", _sectio_prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Identificatores", _sectio_secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Operationes Binariae", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Praecedentia", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Operationes Unariae", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Ternaria", _sectio_sexta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Accessus", _sectio_septima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Vocatio", _sectio_octava, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Literalia Composita", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Declarationes", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "Control Flow", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Functiones", _sectio_decima_secunda, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Programma Complexum", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "Errores", _sectio_decima_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sputnik_parser", SECTIONES);
}
