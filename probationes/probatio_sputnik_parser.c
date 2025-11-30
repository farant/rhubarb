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
_obtinere_infantem(SputnikAstNodus* nodus, i32 index)
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
_numerus_infantium(SputnikAstNodus* nodus)
{
    si (nodus == NIHIL || nodus->liberi == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(nodus->liberi);
}

interior SputnikAstNodus*
_prima_sententia(SputnikAstNodus* programma)
{
    redde _obtinere_infantem(programma, ZEPHYRUM);
}

interior SputnikAstNodus*
_expressio_ex_sententia(SputnikAstNodus* sententia)
{
    /* Pro SENTENTIA_EXPRESSIO, expressio est in liberi[0] */
    redde _obtinere_infantem(sententia, ZEPHYRUM);
}


/* ==================================================
 * Probationes: Literalia
 * ================================================== */

interior vacuum
_probare_literalia(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;

    imprimere("\n--- Probatio Literalia ---\n");

    /* Numerus integer */
    {
        r = sputnik_parsere_ex_literis("42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO(sent != NIHIL);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_EXPRESSIO);
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
        r = sputnik_parsere_ex_literis("true;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VERUM_LITERALIS);
    }

    /* Falsum */
    {
        r = sputnik_parsere_ex_literis("false;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_FALSUM_LITERALIS);
    }

    /* Nihil */
    {
        r = sputnik_parsere_ex_literis("null;", piscina, intern);
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
_probare_identificatores(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;

    imprimere("\n--- Probatio Identificatores ---\n");

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
_probare_operationes_binariae(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;
    SputnikAstNodus* sin;
    SputnikAstNodus* dex;

    imprimere("\n--- Probatio Operationes Binariae ---\n");

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
_probare_praecedentia(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;
    SputnikAstNodus* dex;

    imprimere("\n--- Probatio Praecedentia ---\n");

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
        CREDO_AEQUALIS_I32(sin_tmp->operator, SPUTNIK_LEXEMA_ASTERISCUS);
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
_probare_operationes_unariae(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;
    SputnikAstNodus* operandus;

    imprimere("\n--- Probatio Operationes Unariae ---\n");

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
        r = sputnik_parsere_ex_literis("!true;", piscina, intern);
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
_probare_ternaria(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;
    SputnikAstNodus* conditio;
    SputnikAstNodus* verum_ramus;
    SputnikAstNodus* falsum_ramus;

    imprimere("\n--- Probatio Ternaria ---\n");

    /* a ? b : c */
    {
        r = sputnik_parsere_ex_literis("a ? b : c;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_TERNARIA);

        conditio = _obtinere_infantem(expr, ZEPHYRUM);
        verum_ramus = _obtinere_infantem(expr, I);
        falsum_ramus = _obtinere_infantem(expr, II);

        CREDO(conditio != NIHIL);
        CREDO(verum_ramus != NIHIL);
        CREDO(falsum_ramus != NIHIL);

        CREDO_AEQUALIS_I32(conditio->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(verum_ramus->genus, SPUTNIK_AST_IDENTIFICATOR);
        CREDO_AEQUALIS_I32(falsum_ramus->genus, SPUTNIK_AST_IDENTIFICATOR);
    }

    /* Ternaria in conditione: x > 0 ? 1 : -1 */
    {
        r = sputnik_parsere_ex_literis("x > 0 ? 1 : 0 - 1;", piscina, intern);
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
_probare_accessus(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;

    imprimere("\n--- Probatio Accessus ---\n");

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
_probare_vocatio(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;

    imprimere("\n--- Probatio Vocatio ---\n");

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
        r = sputnik_parsere_ex_literis("foo(1, 2, 3);", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        expr = _expressio_ex_sententia(sent);
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_VOCATIO);
        CREDO_AEQUALIS_I32(_numerus_infantium(expr), IV);  /* callee + 3 args */
    }

    /* Vocatio methodi */
    {
        SputnikAstNodus* callee;
        r = sputnik_parsere_ex_literis("obj.method();", piscina, intern);
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
_probare_literalia_composita(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* expr;

    imprimere("\n--- Probatio Literalia Composita ---\n");

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
        r = sputnik_parsere_ex_literis("let x = {};", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        expr = _obtinere_infantem(sent, ZEPHYRUM);  /* valor */
        CREDO_AEQUALIS_I32(expr->genus, SPUTNIK_AST_OBJECTUM_LITERALIS);
    }

    /* Objectum cum proprietatibus */
    {
        r = sputnik_parsere_ex_literis("let x = {a: 1, b: 2};", piscina, intern);
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
_probare_declarationes(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;
    SputnikAstNodus* valor;

    imprimere("\n--- Probatio Declarationes ---\n");

    /* Let sine valore */
    {
        r = sputnik_parsere_ex_literis("let x;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        CREDO_AEQUALIS_I32(sent->valor.mensura, I);  /* "x" */
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), ZEPHYRUM);
    }

    /* Let cum valore */
    {
        r = sputnik_parsere_ex_literis("let x = 42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_LET);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
        valor = _obtinere_infantem(sent, ZEPHYRUM);
        CREDO_AEQUALIS_F64(valor->numerus, 42.0);
    }

    /* Const (requirit valorem) */
    {
        r = sputnik_parsere_ex_literis("const PI = 3.14;", piscina, intern);
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
_probare_control_flow(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;

    imprimere("\n--- Probatio Control Flow ---\n");

    /* If simplex */
    {
        r = sputnik_parsere_ex_literis("if (x) { y; }", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_SI);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), II);  /* conditio + tunc */
    }

    /* If/else */
    {
        r = sputnik_parsere_ex_literis("if (x) { y; } else { z; }", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_SI);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), III);  /* conditio + tunc + alioquin */
    }

    /* While */
    {
        r = sputnik_parsere_ex_literis("while (x) { y; }", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_DUM);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), II);  /* conditio + corpus */
    }

    /* For */
    {
        r = sputnik_parsere_ex_literis("for (let i = 0; i < 10; i = i + 1) { x; }", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_PER);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), IV);  /* init + cond + incr + corpus */
    }

    /* Return */
    {
        r = sputnik_parsere_ex_literis("return 42;", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_SENTENTIA_REDDE);
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
    }

    /* Return sine valore */
    {
        r = sputnik_parsere_ex_literis("return;", piscina, intern);
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
_probare_functiones(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    SputnikAstNodus* sent;

    imprimere("\n--- Probatio Functiones ---\n");

    /* Functio sine parametris */
    {
        r = sputnik_parsere_ex_literis("function foo() { return 42; }", piscina, intern);
        CREDO(r.successus);
        sent = _prima_sententia(r.radix);
        CREDO_AEQUALIS_I32(sent->genus, SPUTNIK_AST_DECLARATIO_FUNCTIO);
        CREDO_AEQUALIS_I32(sent->valor.mensura, III);  /* "foo" */
        /* Solum corpus (grex) */
        CREDO_AEQUALIS_I32(_numerus_infantium(sent), I);
    }

    /* Functio cum parametris */
    {
        r = sputnik_parsere_ex_literis("function add(a, b) { return a + b; }", piscina, intern);
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
_probare_programma_complexum(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;
    constans character* programma =
        "function factorial(n) {\n"
        "    if (n <= 1) {\n"
        "        return 1;\n"
        "    }\n"
        "    return n * factorial(n - 1);\n"
        "}\n"
        "\n"
        "let result = factorial(5);\n";

    imprimere("\n--- Probatio Programma Complexum ---\n");

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
_probare_errores(Piscina* piscina, InternamentumChorda* intern)
{
    SputnikParserResultus r;

    imprimere("\n--- Probatio Errores ---\n");

    /* Semicolon deest */
    {
        r = sputnik_parsere_ex_literis("let x = 1", piscina, intern);
        CREDO(!r.successus);
    }

    /* Parenthesis deest */
    {
        r = sputnik_parsere_ex_literis("if x { y; }", piscina, intern);
        CREDO(!r.successus);
    }

    /* Bracchium deest */
    {
        r = sputnik_parsere_ex_literis("function foo() return 1; }", piscina, intern);
        CREDO(!r.successus);
    }

    imprimere("  Errores: VERUM\n");
}


/* ==================================================
 * Principale
 * ================================================== */

s32 principale(vacuum)
{
    Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_parser", MMMMXCVI * IV);
    intern = internamentum_creare(piscina);

    credo_aperire(piscina);

    imprimere("========================================\n");
    imprimere("PROBATIO SPUTNIK PARSER\n");
    imprimere("========================================\n");

    _probare_literalia(piscina, intern);
    _probare_identificatores(piscina, intern);
    _probare_operationes_binariae(piscina, intern);
    _probare_praecedentia(piscina, intern);
    _probare_operationes_unariae(piscina, intern);
    _probare_ternaria(piscina, intern);
    _probare_accessus(piscina, intern);
    _probare_vocatio(piscina, intern);
    _probare_literalia_composita(piscina, intern);
    _probare_declarationes(piscina, intern);
    _probare_control_flow(piscina, intern);
    _probare_functiones(piscina, intern);
    _probare_programma_complexum(piscina, intern);
    _probare_errores(piscina, intern);

    imprimere("\n========================================\n");
    imprimere("OMNES PROBATIONES: SUCCESSUS\n");
    imprimere("========================================\n");

    piscina_destruere(piscina);

    redde ZEPHYRUM;
}
