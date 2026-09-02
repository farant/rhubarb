/* probatio_silva_quaestio_messis.c - barra v1 arbor_quaestio
 * translata (QD, 2026-07-22): ~100 asserta Anglica contra genera
 * DECISUS Latina re-asserta. Sectiones speculant probatio_arbor_
 * quaestio.c; deltae ad messem RECORDATAE:
 *   1. tag ignotum: v1 tacite vacuum -> fractura clara (DECISUS)
 *   2. translation-unit nodum non habet (radix = LISTA nuda) -
 *      congruit in nodis veris probatur
 *   3. utilitates introspectionis (pseudo_nomen/combinator_nomen)
 *      non portatae - nomina in superficie selectoris ipsa vivunt
 *   4. "identifier" v1 = declarator-titulus (nomina declarata) AUT
 *      folium-identificator (usus) - silva ea distinguit
 *   5. asserta mollia v1 (">= I"; ":calls sine assertione") EXACTA
 *      ubi arbor silvae numerabilis */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "silva_token.h"
#include "silva_nodus.h"
#include "silva_parsare.h"
#include "silva_quaestio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior s32
_numerus (
                  Piscina* piscina,
    constans SilvaParsura* parsura,
       constans character* selector)
{
    constans character* causa = NIHIL;
         SilvaQuaestio* q = silva_quaestio_compilare(piscina,
             &SILVA_C89_REGISTRUM, selector, &causa);
    Xar* resultata;

    si (q == NIHIL) redde -I;
    resultata = silva_quaestio_exsequi(q,
        parsura->commissio->radix, piscina);
    si (resultata == NIHIL) redde -I;
    redde (s32)xar_numerus(resultata);
}

interior constans SilvaNodus*
_captura (
    constans SilvaQuaestioResultatum* r,
                  constans character* titulus)
{
    i32 i;

    si (r == NIHIL || r->capturae == NIHIL) redde NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(r->capturae); i++)
    {
        SilvaQuaestioCaptura* cap = (SilvaQuaestioCaptura*)
            xar_obtinere(r->capturae, i);

        si (   cap                  != NIHIL
            && cap->titulus.mensura == (i32)strlen(titulus)
            && memcmp(cap->titulus.datum, titulus,
                   strlen(titulus)) == ZEPHYRUM)
        {
            redde cap->nodus;
        }
    }
    redde NIHIL;
}

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_messis",
        8388608);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * Selectores typorum (v1: Type Selectors)
     * ======================================================== */

    {
        constans character* fons = "int main(void) { return 0; }";
              SilvaParsura* parsura;

        imprimere("\n--- Messis: selectores typorum ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), I);
        /* v1 "identifier" imprecise - silva distinguit: main =
         * nomen declaratum, usus nulli */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-identificator"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "redde"), I);
    }


    /* ========================================================
     * Selectores typorum ampliati (v1: Extended)
     * ======================================================== */

    {
        constans character* fons =
            "struct Point { int x; int y; };\n"
            "enum Color { RED, GREEN, BLUE };\n"
            "typedef int Integer;\n"
            "int global = 42;\n"
            "int add(int a, int b) {\n"
            "    int sum = a + b;\n"
            "    if (sum > 0) { return sum; }\n"
            "    else { return -sum; }\n"
            "}\n"
            "void loop(void) {\n"
            "    int i;\n"
            "    for (i = 0; i < 10; i++) { }\n"
            "    while (i > 0) { i--; }\n"
            "}\n";
        SilvaParsura* parsura;

        imprimere("\n--- Messis: typi ampliati ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "structura"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "enumeratio"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "enumerator"), III);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declaratio"), VI);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "si"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "per"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "dum"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "binarium"), IV);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "unarium"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "folium-integer"), V);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus"), VI);
        /* v1 ">= I" molle servatum: numerus specificatorum
         * primitivorum arbori sensibilis */
        CREDO_VERUM (_numerus(piscina, parsura,
            "typus-primitivus") >= (s32)VIII);
    }


    /* ========================================================
     * Combinatores (v1: Descendant + Child)
     * ======================================================== */

    {
        constans character* fons =
            "int foo(void) { int x; return x; }";
        SilvaParsura* parsura;

        imprimere("\n--- Messis: combinatores ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        /* v1 "function-definition identifier" (foo et x) */
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis declarator-titulus"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis folium-identificator"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus redde"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis > corpus"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "corpus > redde"), I);
    }


    /* ========================================================
     * Attributa (v1: Attribute Selectors + Operators)
     * ======================================================== */

    {
        constans character* fons = "int main(void) { return 0; }";
              SilvaParsura* parsura;

        imprimere("\n--- Messis: attributa ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus=\"main\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus=\"nonexistent\"]"),
            ZEPHYRUM);
    }
    {
        constans character* fons =
            "int prefix_one(void) { return 0; }\n"
            "int prefix_two(void) { return 0; }\n"
            "int other_suffix(void) { return 0; }\n"
            "int contains_mid_value(void) { return 0; }\n";
        SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus^=\"prefix\"]"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus$=\"suffix\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus*=\"mid\"]"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "declarator-titulus[tok_titulus^=\"xyz\"]"),
            ZEPHYRUM);
    }


    /* ========================================================
     * Capturae (v1: Captures + Multiple Captures; delta: tabula
     * dispersa v1 -> Xar gradus omnes)
     * ======================================================== */

    {
        constans character* fons = "int foo(void) { return 0; }";
              SilvaParsura* parsura;
        constans character* causa = NIHIL;
             SilvaQuaestio* q;
                       Xar* resultata;

        imprimere("\n--- Messis: capturae ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        q = silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM,
            "definitio-functionis declarator-titulus$fn", &causa);
        CREDO_NON_NIHIL (q);
        resultata = silva_quaestio_exsequi(q,
            parsura->commissio->radix, piscina);
        CREDO_NON_NIHIL (resultata);
        CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);
        {
            SilvaQuaestioResultatum* r =
                (SilvaQuaestioResultatum*)xar_obtinere(resultata,
                    ZEPHYRUM);

            CREDO_NON_NIHIL (r);
            CREDO_VERUM (_captura(r, "fn") == r->nodus);
        }
    }
    {
        constans character* fons =
            "int foo(int x) { return x + 1; }\n"
            "int bar(int y) { return y * 2; }\n";
              SilvaParsura* parsura;
        constans character* causa = NIHIL;
             SilvaQuaestio* q;
             SilvaQuaestio* q_def;
             SilvaQuaestio* q_redde;
                       Xar* resultata;
                       i32  i;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        q = silva_quaestio_compilare(piscina, &SILVA_C89_REGISTRUM,
            "definitio-functionis$func > corpus redde$ret",
            &causa);
        q_def = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "definitio-functionis", &causa);
        q_redde = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "redde", &causa);
        CREDO_NON_NIHIL (q);
        CREDO_NON_NIHIL (q_def);
        CREDO_NON_NIHIL (q_redde);

        resultata = silva_quaestio_exsequi(q,
            parsura->commissio->radix, piscina);
        CREDO_NON_NIHIL (resultata);
        CREDO_AEQUALIS_I32 (xar_numerus(resultata), II);
        per (i = ZEPHYRUM; i < xar_numerus(resultata); i++)
        {
            SilvaQuaestioResultatum* r =
                (SilvaQuaestioResultatum*)xar_obtinere(resultata,
                    i);
            constans SilvaNodus* func;
            constans SilvaNodus* ret;

            CREDO_NON_NIHIL (r);
            func  = _captura(r, "func");
            ret   = _captura(r, "ret");
            CREDO_NON_NIHIL (func);
            CREDO_VERUM (ret == r->nodus);
            CREDO_VERUM (silva_quaestio_congruit(q_def, func));
            CREDO_VERUM (silva_quaestio_congruit(q_redde, ret));
        }
    }


    /* ========================================================
     * Pseudo (v1: Pseudo-Selectors + Extended; delta: asserta
     * mollia v1 EXACTA - :vocat(compute) v1 sine assertione)
     * ======================================================== */

    {
        constans character* fons =
            "void foo(void) { printf(\"hello\"); }\n"
            "int bar(void) { return 42; }";
        SilvaParsura* parsura;

        imprimere("\n--- Messis: pseudo ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:reddit"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:vocat(printf)"), I);
    }
    {
        constans character* fons =
            "int global_var = 100;\n"
            "int compute(int x) {\n"
            "    int local = x * 2;\n"
            "    return local + global_var;\n"
            "}\n"
            "void caller(void) {\n"
            "    int result = compute(5);\n"
            "}\n";
        SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:definit(compute)"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:vocat(compute)"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:utitur(global_var)"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:utitur(nonexistent)"),
            ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis:definit(caller)"), I);
    }


    /* ========================================================
     * Quaestiones complexae + nidificatio (v1: Complex + Deep
     * Nesting + Child vs Descendant)
     * ======================================================== */

    {
        constans character* fons =
            "int add(int a, int b) { return a + b; }\n"
            "int main(void) { return add(1, 2); }";
        SilvaParsura* parsura;

        imprimere("\n--- Messis: complexae/nidificatio ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis > corpus redde"), II);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "vocatio"), I);
    }
    {
        constans character* fons =
            "int deep(int n) {\n"
            "    if (n > 0) {\n"
            "        if (n > 10) {\n"
            "            if (n > 100) {\n"
            "                return n;\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    return 0;\n"
            "}\n";
        SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis si si si redde"), I);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis > redde"), ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis redde"), II);
    }
    {
        constans character* fons =
            "int func(void) {\n"
            "    if (1) {\n"
            "        return 1;\n"
            "    }\n"
            "    return 0;\n"
            "}\n";
        SilvaParsura* parsura;
                 s32  filii;
                 s32  descendentes;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        filii         = _numerus(piscina, parsura, "corpus > redde");
        descendentes  = _numerus(piscina, parsura, "corpus redde");
        CREDO_AEQUALIS_S32 (filii, II);
        CREDO_AEQUALIS_S32 (descendentes, II);
        CREDO_VERUM (descendentes >= filii);
    }


    /* ========================================================
     * Casus marginis (v1: Edge Cases; delta: tag ignotum v1
     * tacite vacuum -> fractura clara)
     * ======================================================== */

    {
        SilvaParsura* parsura;

        imprimere("\n--- Messis: casus marginis ---\n");

        /* fons vacuus: parsura viva, nihil congruit */
        parsura = silva_c89_parsare(piscina, "probatio.c", "",
            ZEPHYRUM, NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), ZEPHYRUM);
    }
    {
        constans character* fons = "int x;";
              SilvaParsura* parsura;
        constans character* causa = NIHIL;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), ZEPHYRUM);

        /* DELTA pinnata: v1 "nonexistent-node-type" tacite
         * vacuum reddebat - silva fracturam claram exigit */
        CREDO_NIHIL (silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "nonexistent-node-type",
            &causa));
        CREDO_VERUM (causa != NIHIL
            && strcmp(causa, "tag registro ignotum") == ZEPHYRUM);
    }
    {
        constans character* fons =
            "int alpha(void) { return 1; }\n"
            "int beta(void) { return 2; }\n"
            "int gamma(void) { return 3; }\n";
        SilvaParsura* parsura;

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);
        CREDO_VERUM (parsura->successus);

        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"), III);
        CREDO_AEQUALIS_S32 (_numerus(piscina, parsura,
            "definitio-functionis"
            " declarator-titulus[tok_titulus=\"beta\"]"), I);
    }


    /* ========================================================
     * Congruit (v1: Simple Match; delta: translation-unit nodum
     * non habet - radix LISTA nuda, nodi veri probantur)
     * ======================================================== */

    {
        constans character* fons = "int x;";
              SilvaParsura* parsura;
        constans character* causa = NIHIL;
             SilvaQuaestio* q_decl;
             SilvaQuaestio* q_def;
                       Xar* resultata;

        imprimere("\n--- Messis: congruit ---\n");

        parsura = silva_c89_parsare(piscina, "probatio.c", fons,
            (i32)strlen(fons), NIHIL);
        CREDO_NON_NIHIL (parsura);

        q_decl = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "declaratio", &causa);
        q_def = silva_quaestio_compilare(piscina,
            &SILVA_C89_REGISTRUM, "definitio-functionis", &causa);
        CREDO_NON_NIHIL (q_decl);
        CREDO_NON_NIHIL (q_def);

        resultata = silva_quaestio_exsequi(q_decl,
            parsura->commissio->radix, piscina);
        CREDO_NON_NIHIL (resultata);
        CREDO_AEQUALIS_I32 (xar_numerus(resultata), I);
        {
            SilvaQuaestioResultatum* r =
                (SilvaQuaestioResultatum*)xar_obtinere(resultata,
                    ZEPHYRUM);

            CREDO_NON_NIHIL (r);
            CREDO_VERUM (silva_quaestio_congruit(q_decl,
                r->nodus));
            CREDO_FALSUM (silva_quaestio_congruit(q_def,
                r->nodus));
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
