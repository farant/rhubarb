/* probatio_silva_compat_expansio.c - Vectis compatibilitatis: arbor2 expandere
 *
 * Portatum ex probationes/probatio_arbor2_expandere.c - "carry the tests".
 * Sectiones ordine originali; API adaptata (silva_macro_addere pro
 * addere_macro, prospectare pro lookahead, praebere pro viis plagularum).
 *
 * NON portata (consulto, vide phase-log Phase 2.5):
 *   - collectio typedef (est_typedef, quaerere_typedef): silva oraculum
 *     typedef ad parsatorem ponit (Phase 4) - probationes ibi portandae
 *   - administratio viarum plagularum (system_viae/local_viae): silva
 *     contentum per API accipit (praebere) - plagulas numquam ipsa aperit;
 *     semantica includendi HIC portatur, viae non
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "silva_token.h"
#include "silva_lexema.h"
#include "silva_expandere.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior SilvaExpansio*
_exp_novum (
    Piscina* piscina)
{
    SilvaExpansio* exp;

    exp = silva_expansio_creare(piscina);
    silva_fons_addere(exp, "test.c", FALSUM);
    redde exp;
}

/* Processare fontem plene: lexare + directivae + expansio positionalis
 * (semantica cpp: quodque lexema statum macro sui puncti videt) */
interior Xar*
_processare (
         SilvaExpansio* exp,
               Piscina* piscina,
    constans character* fons)
{
    Xar* lexemata;
    Xar* reliqua;

    lexemata = silva_lexare(piscina, fons, (i32)strlen(fons), ZEPHYRUM);
    reliqua = silva_expansio_directivas_processare(exp, lexemata,
        NIHIL);
    redde silva_expansio_expandere_reliqua(exp, reliqua, NIHIL);
}

interior SilvaToken*
_ad (
    Xar* lexemata,
    i32  i)
{
    redde *(SilvaToken**)xar_obtinere(lexemata, i);
}

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

s32 principale (vacuum)
{
        b32  praeteritus;
    Piscina* piscina;

    piscina =
        piscina_generare_dynamicum("probatio_silva_compat_expansio",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: expansio obiecti-similis per API
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans expansionem obiecti-similis per API ---\n");

        exp = _exp_novum(piscina);
        CREDO_VERUM (silva_macro_addere(exp, "FOO", "42"));

        fructus = _processare(exp, piscina, "int x = FOO;");

        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_INT);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, II)->genus,
            (i32)SILVA_LEX_ASSIGNATIO);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, III)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, III)->valor, "42");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, IV)->genus,
            (i32)SILVA_LEX_SEMICOLON);
    }


    /* ========================================================
     * PROBARE: #define acta creat (arbor2: segmenta)
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans #define acta creat ---\n");

        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define BAR 1\nint x = BAR;");
        (vacuum)fructus;

        CREDO_VERUM (xar_numerus(exp->acta) >= I);
    }


    /* ========================================================
     * PROBARE: catena provenientiae
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
           SilvaToken* tok;
           SilvaToken* radix;

        imprimere("\n--- Probans catenam provenientiae ---\n");

        exp = _exp_novum(piscina);
        silva_macro_addere(exp, "NUM", "123");

        fructus = _processare(exp, piscina, "NUM");

        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)tok->origo.genus,
            (i32)SILVA_ORIGO_EXPANSIO);

        radix = silva_token_radix(tok);
        CREDO_NON_NIHIL (radix);
        CREDO_VERUM (silva_token_est_fons(radix));
        CREDO_CHORDA_AEQUALIS_LITERIS (radix->valor, "NUM");
    }


    /* ========================================================
     * PROBARE: expansio nidificata per API
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans expansionem nidificatam ---\n");

        exp = _exp_novum(piscina);
        silva_macro_addere(exp, "A", "B");
        silva_macro_addere(exp, "B", "99");

        fructus = _processare(exp, piscina, "A");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "99");
    }


    /* ========================================================
     * PROBARE: macros ad lineam
     * ======================================================== */

    {
         SilvaExpansio* exp;
                   Xar* fructus;
        TabulaDispersa* tunc;
                vacuum* valor;

        imprimere("\n--- Probans macros ad lineam ---\n");

        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define X 1\nint a;\n#undef X\nint b;");
        (vacuum)fructus;

        /* ad lineam II: X definita */
        tunc = silva_expansio_macros_ad_lineam(exp, piscina, ZEPHYRUM,
            II);
        CREDO_NON_NIHIL (tunc);
        CREDO_VERUM (tabula_dispersa_invenire(tunc, _ch("X"), &valor));
    }


    /* (collectio typedef: V sectiones DELATAE ad Phase 4 - oraculum) */


    /* ========================================================
     * PROBARE: functio-similis substitutio simplex
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
        constans character* parametra_add[3];

        imprimere("\n--- Probans functionem-similem ADD ---\n");

        parametra_add[0]   = "a";
        parametra_add[I]   = "b";
        parametra_add[II]  = NIHIL;

        exp = _exp_novum(piscina);
        CREDO_VERUM (silva_macro_functio_addere(exp, "ADD",
            parametra_add,
            "((a)+(b))"));

        fructus = _processare(exp, piscina, "ADD(1,2)");

        /* ((1)+(2)) */
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, II)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, III)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, IV)->genus,
            (i32)SILVA_LEX_PLUS);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, V)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VI)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VII)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VIII)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
    }


    /* ========================================================
     * PROBARE: parentheses nidificatae in argumentis
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
                SilvaToken* tok;
        constans character* parametra_add[3];
                       i32  i;

        imprimere("\n--- Probans parentheses nidificatas ---\n");

        parametra_add[0]   = "a";
        parametra_add[I]   = "b";
        parametra_add[II]  = NIHIL;

        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "ADD", parametra_add,
            "((a)+(b))");

        /* ADD((1+2),3): (1+2) est argumentum primum */
        fructus = _processare(exp, piscina, "ADD((1+2),3)");

        tok = NIHIL;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            tok = _ad(fructus, i);
            si (tok->genus == SILVA_LEX_INTEGER)
            {
                frange;
            }
        }
        CREDO_NON_NIHIL (tok);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "1");
    }


    /* ========================================================
     * PROBARE: functio-similis sine parametris
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
        constans character* parametra_vacua[1];

        imprimere("\n--- Probans functionem sine parametris ---\n");

        parametra_vacua[0] = NIHIL;

        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "EMPTY", parametra_vacua, "x");

        fructus = _processare(exp, piscina, "EMPTY()");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "x");
    }


    /* ========================================================
     * PROBARE: MAX cum ternario
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
        constans character* parametra_max[3];
                       b32  quaestio_inventa;
                       i32  i;

        imprimere("\n--- Probans MAX ternarium ---\n");

        parametra_max[0]   = "a";
        parametra_max[I]   = "b";
        parametra_max[II]  = NIHIL;

        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "MAX", parametra_max,
            "((a)>(b)?(a):(b))");

        fructus = _processare(exp, piscina, "MAX(x,y)");

        quaestio_inventa = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            si (_ad(fructus, i)->genus == SILVA_LEX_QUAESTIO)
            {
                quaestio_inventa = VERUM;
                frange;
            }
        }
        CREDO_VERUM (quaestio_inventa);
    }


    /* ========================================================
     * PROBARE: functio-similis per #define
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans SQUARE per #define ---\n");

        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define SQUARE(x) ((x)*(x))\nSQUARE(5)");

        /* ((5)*(5)) */
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, II)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, III)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, IV)->genus,
            (i32)SILVA_LEX_STAR);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, V)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VI)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VII)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, VIII)->genus,
            (i32)SILVA_LEX_PAREN_CLAUSA);
    }


    /* ========================================================
     * PROBARE: stringificatio (basica, numerica, effugium, #define)
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
                SilvaToken* tok;
        constans character* parametra_str[2];
                       b32  habet_obliquam;
                       i32  j;

        imprimere("\n--- Probans stringificationem ---\n");

        parametra_str[0] = "x";
        parametra_str[I] = NIHIL;

        /* basica */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "STR", parametra_str, "#x");
        fructus  = _processare(exp, piscina, "STR(hello)");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "\"hello\"");

        /* numerica */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "STR", parametra_str, "#x");
        fructus  = _processare(exp, piscina, "STR(123)");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "\"123\"");

        /* effugium: STR("hi") - quotes effugiuntur */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "STR", parametra_str, "#x");
        fructus  = _processare(exp, piscina, "STR(\"hi\")");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        habet_obliquam = FALSUM;
        per (j = ZEPHYRUM; j < tok->valor.mensura; j++)
        {
            si (tok->valor.datum[j] == '\\')
            {
                habet_obliquam = VERUM;
                frange;
            }
        }
        CREDO_VERUM (habet_obliquam);

        /* per #define */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define STRINGIFY(x) #x\nSTRINGIFY(test)");
        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "\"test\"");
    }


    /* ========================================================
     * PROBARE: pasta (basica, praefixum, numerica, #define)
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
                SilvaToken* tok;
        constans character* parametra_paste[3];
        constans character* parametra_var[2];

        imprimere("\n--- Probans pastam ---\n");

        parametra_paste[0]   = "a";
        parametra_paste[I]   = "b";
        parametra_paste[II]  = NIHIL;
        parametra_var[0]     = "n";
        parametra_var[I]     = NIHIL;

        /* basica: foobar */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "PASTE", parametra_paste,
            "a##b");
        fructus  = _processare(exp, piscina, "PASTE(foo,bar)");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "foobar");

        /* praefixum: var_1 */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "VAR", parametra_var,
            "var_##n");
        fructus  = _processare(exp, piscina, "VAR(1)");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "var_1");

        /* numerica: 1234 */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "PASTE", parametra_paste,
            "a##b");
        fructus  = _processare(exp, piscina, "PASTE(12,34)");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "1234");

        /* per #define: helloworld */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define GLUE(a,b) a##b\nGLUE(hello,world)");
        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "helloworld");
    }


    /* ========================================================
     * PROBARE: macro variadica (__VA_ARGS__)
     * ======================================================== */

    {
             SilvaExpansio* exp;
                       Xar* fructus;
        constans character* parametra_va[2];
        constans character* parametra_fmt[3];

        imprimere("\n--- Probans variadicam ---\n");

        parametra_va[0]    = "...";
        parametra_va[I]    = NIHIL;
        parametra_fmt[0]   = "f";
        parametra_fmt[I]   = "...";
        parametra_fmt[II]  = NIHIL;

        /* basica: VARIADIC(a,b,c) -> a , b , c */
        exp = _exp_novum(piscina);
        CREDO_VERUM (silva_macro_functio_addere(exp, "VARIADIC",
            parametra_va, "__VA_ARGS__"));
        fructus = _processare(exp, piscina, "VARIADIC(a,b,c)");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "a");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_COMMA);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, II)->valor, "b");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, III)->genus,
            (i32)SILVA_LEX_COMMA);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, IV)->valor, "c");

        /* cum praefixo: FMT(foo,1,2) -> foo ( 1 , 2 ) */
        exp = _exp_novum(piscina);
        silva_macro_functio_addere(exp, "FMT", parametra_fmt,
            "f(__VA_ARGS__)");
        fructus = _processare(exp, piscina, "FMT(foo,1,2)");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "foo");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_PAREN_APERTA);

        /* per #define: ARGS(x,y,z) -> x , y , z */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define ARGS(...) __VA_ARGS__\nARGS(x,y,z)");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "x");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_COMMA);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, II)->valor, "y");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, III)->genus,
            (i32)SILVA_LEX_COMMA);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, IV)->valor, "z");
    }


    /* ========================================================
     * PROBARE: praeventio recursionis
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
           SilvaToken* tok;

        imprimere("\n--- Probans praeventionem recursionis ---\n");

        /* directa: SELF -> SELF */
        exp = _exp_novum(piscina);
        silva_macro_addere(exp, "SELF", "SELF");
        fructus  = _processare(exp, piscina, "SELF");
        tok      = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "SELF");

        /* mutua: MACRO_A <-> MACRO_B */
        exp = _exp_novum(piscina);
        silva_macro_addere(exp, "MACRO_A", "MACRO_B");
        silva_macro_addere(exp, "MACRO_B", "MACRO_A");
        fructus = _processare(exp, piscina, "MACRO_A");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);

        /* per #define */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define RECURSIVE RECURSIVE\nRECURSIVE");
        CREDO_VERUM (xar_numerus(fructus) > ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: #undef sistit expansionem; redefinitio; inexistens
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
                  b32  integer_inventum;
                  b32  identificator_inventum;
                  i32  i;

        imprimere("\n--- Probans #undef ---\n");

        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define TEMP 42\nTEMP\n#undef TEMP\nTEMP");

        integer_inventum        = FALSUM;
        identificator_inventum  = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            si (_ad(fructus, i)->genus == SILVA_LEX_INTEGER)
            {
                integer_inventum = VERUM;
            }
            si (_ad(fructus, i)->genus == SILVA_LEX_IDENTIFICATOR)
            {
                identificator_inventum = VERUM;
            }
        }
        CREDO_VERUM (integer_inventum);      /* prima TEMP expansa */
        CREDO_VERUM (identificator_inventum); /* secunda TEMP manet */

        /* redefinitio post #undef */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define X 1\nX\n#undef X\n#define X 2\nX");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "1");
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, I)->valor, "2");

        /* #undef inexistentis: nullus fragor */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#undef NONEXISTENT\nint x;");
        CREDO_VERUM (xar_numerus(fructus) > ZEPHYRUM);
    }


    /* ========================================================
     * PROBARE: variadica - zephyrum/unum argumentum, bis usa
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
                  i32  numerus_commatum;
                  i32  i;

        imprimere("\n--- Probans variadicam extremam ---\n");

        /* zephyrum argumenta: ARGS() -> solum EOF */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define ARGS(...) __VA_ARGS__\nARGS()");
        CREDO_AEQUALIS_I32 (xar_numerus(fructus), I);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_EOF);

        /* unum argumentum */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define ARGS(...) __VA_ARGS__\nARGS(x)");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "x");

        /* __VA_ARGS__ bis: duo commata */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define TWICE(...) __VA_ARGS__ __VA_ARGS__\nTWICE(a,b)");
        numerus_commatum = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            si (_ad(fructus, i)->genus == SILVA_LEX_COMMA)
            {
                numerus_commatum++;
            }
        }
        CREDO_AEQUALIS_I32 (numerus_commatum, II);
    }


    /* ========================================================
     * PROBARE: invocationes nidificatae
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
                  i32  numerus_plus;
                  b32  foo_inventum;
                  i32  i;

        imprimere("\n--- Probans invocationes nidificatas ---\n");

        /* argumentum est invocatio: ADD(MUL(2,3),4) */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define MUL(a,b) ((a)*(b))\n#define ADD(a,b) ((a)+(b))\n"
            "ADD(MUL(2,3),4)");
        numerus_plus = ZEPHYRUM;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            si (_ad(fructus, i)->genus == SILVA_LEX_PLUS)
            {
                numerus_plus++;
            }
        }
        CREDO_AEQUALIS_I32 (numerus_plus, I);

        /* catenatae: OUTER(INNER(foo)) */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define INNER(x) (x)\n#define OUTER(x) [x]\nOUTER(INNER(foo))");
        foo_inventum = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
        {
            SilvaToken* t;

            t = _ad(fructus, i);
            si (   t->genus         == SILVA_LEX_IDENTIFICATOR
                && t->valor.mensura == III)
            {
                foo_inventum = VERUM;
                frange;
            }
        }
        CREDO_VERUM (foo_inventum);
    }


    /* ========================================================
     * PROBARE: obiecti-similis per #define
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans obiecti-similem per #define ---\n");

        /* simplex */
        exp      = _exp_novum(piscina);
        fructus  = _processare(exp, piscina, "#define FOO 42\nFOO");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "42");

        /* multi-lexematum */
        exp      = _exp_novum(piscina);
        fructus  = _processare(exp, piscina, "#define EXPR 1+2\nEXPR");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, I)->genus,
            (i32)SILVA_LEX_PLUS);
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, II)->genus,
            (i32)SILVA_LEX_INTEGER);
    }


    /* ========================================================
     * PROBARE: stringificatio extrema
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
           SilvaToken* tok;

        imprimere("\n--- Probans stringificationem extremam ---\n");

        /* argumentum vacuum: STR() -> "" */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina, "#define STR(x) #x\nSTR()");
        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        CREDO_AEQUALIS_I32 (tok->valor.mensura, II);  /* "" */

        /* lexemata multa: STR(a b c) -> "a b c" */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define STR(x) #x\nSTR(a b c)");
        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus, (i32)SILVA_LEX_STRING_LIT);
        CREDO_CHORDA_AEQUALIS_LITERIS (tok->valor, "\"a b c\"");
    }


    /* ========================================================
     * PROBARE: pasta extrema
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
           SilvaToken* tok;

        imprimere("\n--- Probans pastam extremam ---\n");

        /* operator factus: OP(+,=) -> += */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define OP(a,b) a##b\nOP(+,=)");
        tok = _ad(fructus, 0);
        CREDO_AEQUALIS_I32 ((i32)tok->genus,
            (i32)SILVA_LEX_PLUS_ASSIGNATIO);

        /* latus sinistrum vacuum: PASTE(,bar) - nullus fragor */
        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define PASTE(a,b) a##b\nPASTE(,bar)");
        CREDO_VERUM (xar_numerus(fructus) > ZEPHYRUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "bar");
    }


    /* ========================================================
     * PROBARE: expansio ter nidificata
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;

        imprimere("\n--- Probans expansionem ter nidificatam ---\n");

        exp = _exp_novum(piscina);
        fructus = _processare(exp, piscina,
            "#define A B\n#define B C\n#define C 999\nA");
        CREDO_AEQUALIS_I32 ((i32)_ad(fructus, 0)->genus,
            (i32)SILVA_LEX_INTEGER);
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, 0)->valor, "999");
    }


    /* ========================================================
     * PROBARE: prospectus (arbor2: lookahead)
     * ======================================================== */

    {
          SilvaExpansio* exp;
        SilvaProspectus  prospectus;

        imprimere("\n--- Probans prospectum ---\n");

        exp = _exp_novum(piscina);
        silva_macro_addere(exp, "TYPUS", "int");
        silva_macro_addere(exp, "VACUUS", "");
        silva_macro_addere(exp, "INNER", "42");
        silva_macro_addere(exp, "OUTER", "INNER");
        silva_macro_addere(exp, "ALIAS", "foo");
        silva_macro_addere(exp, "NUMERUS", "42");

        /* macro simplex ad verbum clausum */
        CREDO_VERUM (silva_expansio_prospectare(exp, _ch("TYPUS"),
            &prospectus));
        CREDO_AEQUALIS_I32 ((i32)prospectus.genus, (i32)SILVA_LEX_INT);
        CREDO_FALSUM (prospectus.est_recursivum);

        /* macro ignotum: FALSUM redditur */
        CREDO_FALSUM (silva_expansio_prospectare(exp,
            _ch("NONEXISTENT"),
            &prospectus));

        /* macro vacuum */
        CREDO_VERUM (silva_expansio_prospectare(exp, _ch("VACUUS"),
            &prospectus));
        CREDO_AEQUALIS_I32 ((i32)prospectus.genus, (i32)SILVA_LEX_EOF);
        CREDO_VERUM (prospectus.est_vacuum);
        CREDO_FALSUM (prospectus.est_recursivum);

        /* macro ad macro: est_recursivum */
        CREDO_VERUM (silva_expansio_prospectare(exp, _ch("OUTER"),
            &prospectus));
        CREDO_AEQUALIS_I32 ((i32)prospectus.genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_VERUM (prospectus.est_recursivum);
        CREDO_FALSUM (prospectus.est_vacuum);

        /* macro ad identificatorem non-macro */
        CREDO_VERUM (silva_expansio_prospectare(exp, _ch("ALIAS"),
            &prospectus));
        CREDO_AEQUALIS_I32 ((i32)prospectus.genus,
            (i32)SILVA_LEX_IDENTIFICATOR);
        CREDO_FALSUM (prospectus.est_recursivum);

        /* macro ad litteram integram */
        CREDO_VERUM (silva_expansio_prospectare(exp, _ch("NUMERUS"),
            &prospectus));
        CREDO_AEQUALIS_I32 ((i32)prospectus.genus,
            (i32)SILVA_LEX_INTEGER);
    }


    /* ========================================================
     * PROBARE: includenda (semantica; viae plagularum = res hospitis)
     * ======================================================== */

    {
        SilvaExpansio* exp;
                  Xar* fructus;
                  Xar* lexemata;

        imprimere("\n--- Probans includenda ---\n");

        /* extractio macro ex includendo */
        exp = _exp_novum(piscina);
        silva_includendum_praebere(exp, "caput.h",
            "#define HEADER_MACRO 42\n",
            (i32)strlen("#define HEADER_MACRO 42\n"));
        lexemata = silva_lexare(piscina,
            "#include \"caput.h\"\nint x = HEADER_MACRO;",
            (i32)strlen("#include \"caput.h\"\nint x = HEADER_MACRO;"),
            ZEPHYRUM);
        fructus = silva_expansio_expandere(exp,
            silva_expansio_directivas_processare(exp, lexemata, NIHIL),
            NIHIL);
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp,
            _ch("HEADER_MACRO")));
        CREDO_CHORDA_AEQUALIS_LITERIS (_ad(fructus, III)->valor, "42");

        /* includendum nidificatum: a.h -> b.h */
        exp = _exp_novum(piscina);
        silva_includendum_praebere(exp, "b.h", "#define B_MACRO 2\n",
            (i32)strlen("#define B_MACRO 2\n"));
        silva_includendum_praebere(exp, "a.h",
            "#include \"b.h\"\n#define A_MACRO 1\n",
            (i32)strlen("#include \"b.h\"\n#define A_MACRO 1\n"));
        lexemata = silva_lexare(piscina, "#include \"a.h\"\n",
            (i32)strlen("#include \"a.h\"\n"), ZEPHYRUM);
        fructus = silva_expansio_directivas_processare(exp, lexemata,
            NIHIL);
        (vacuum)fructus;
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("A_MACRO")));
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("B_MACRO")));

        /* custos: bis includere sine processione duplici */
        exp = _exp_novum(piscina);
        silva_includendum_praebere(exp, "cust.h",
            "#ifndef CUST_H\n#define CUST_H\n#define C_MACRO 3\n#endif\n",
            (i32)strlen(
            "#ifndef CUST_H\n#define CUST_H\n#define C_MACRO 3\n#endif\n"));
        lexemata = silva_lexare(piscina,
            "#include \"cust.h\"\n#include \"cust.h\"\n",
            (i32)strlen("#include \"cust.h\"\n#include \"cust.h\"\n"),
            ZEPHYRUM);
        fructus = silva_expansio_directivas_processare(exp, lexemata,
            NIHIL);
        CREDO_NON_NIHIL (fructus);
        CREDO_NON_NIHIL (silva_expansio_quaerere(exp, _ch("C_MACRO")));

        /* includendum non resolutum: gratiose */
        exp = _exp_novum(piscina);
        lexemata = silva_lexare(piscina, "#include \"nonexistent.h\"",
            (i32)strlen("#include \"nonexistent.h\""), ZEPHYRUM);
        fructus = silva_expansio_directivas_processare(exp, lexemata,
            NIHIL);
        CREDO_NON_NIHIL (fructus);

        /* syntaxis systematis: <stdio.h> */
        exp = _exp_novum(piscina);
        lexemata = silva_lexare(piscina, "#include <stdio.h>",
            (i32)strlen("#include <stdio.h>"), ZEPHYRUM);
        fructus = silva_expansio_directivas_processare(exp, lexemata,
            NIHIL);
        CREDO_NON_NIHIL (fructus);
    }


    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
