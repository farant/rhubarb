/* probatio_briar_bibliotheca.c - descriptio ex linea prima capitis
 * (briar -bibliothecae)
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "briar_bibliotheca.h"
#include <stdio.h>

/* descriptio textus pro titulo ut chorda litterarum comparanda */
interior chorda
_descriptio (
    constans character* textus,
    constans character* titulus,
               Piscina* pn)
{
    redde briar_bibliotheca_descriptio(chorda_ex_literis(textus, pn),
        chorda_ex_literis(titulus, pn));
}

s32
principale (vacuum)
{
     Piscina* pn;
         b32  praeteritus;

    pn = piscina_generare_dynamicum("probatio_briar_bibliotheca",
        65536);
    si (!pn)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(pn);

    imprimere("\n--- Probans descriptionem capitis ---\n");
    /* conventio domus: '/' '*' titulus ' - ' descriptio */
    CREDO_CHORDA_AEQUALIS_LITERIS (_descriptio(
        "/* sors.h - Numeri pseudo-fortuiti SEMINABILES\n"
        " * plus\n */\n",
        "sors.h", pn), "Numeri pseudo-fortuiti SEMINABILES");
    /* clausura in eadem linea, spatia finalia */
    CREDO_CHORDA_AEQUALIS_LITERIS (_descriptio(
        "/* xar.h - \"Codex Tabulae Exponentialis\"   */\n", "xar.h",
        pn),
        "\"Codex Tabulae Exponentialis\"");
    /* separatores ':' et '\xe2\x80\x94' */
    CREDO_CHORDA_AEQUALIS_LITERIS (_descriptio(
        "/* a.h: brevis */", "a.h", pn), "brevis");
    CREDO_CHORDA_AEQUALIS_LITERIS (_descriptio(
        "/* a.h \xe2\x80\x94 longa linea\n", "a.h", pn), "longa linea");
    /* conventionem non sequentia: vacua */
    CREDO_AEQUALIS_I32 (_descriptio("#ifndef TEMPUS_H\n/* x */\n",
        "tempus.h", pn).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_descriptio("/* alius.h - x */\n", "a.h", pn)
        .mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_descriptio("/* a.h - */\n", "a.h", pn).mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_descriptio("/* a.hx - y\n", "a.h", pn)
        .mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (_descriptio("", "a.h", pn).mensura, ZEPHYRUM);

    imprimere("\n--- Probans signaturas alineatas ---\n");
    {
        chorda lineae[V];
        chorda tituli[V];

        lineae[ZEPHYRUM] = chorda_ex_literis("JsonResultus json_legere"
            " (chorda input, Piscina* piscina);", pn);
        tituli[ZEPHYRUM] = chorda_ex_literis("json_legere", pn);
        lineae[I] = chorda_ex_literis("chorda json_scribere_pulchrum"
            " (JsonValor* valor, Piscina* piscina);", pn);
        tituli[I] = chorda_ex_literis("json_scribere_pulchrum", pn);
        /* typus reditus monstrator; nomen verbum TOTUM ('sors' in
         * 'Sors' aut 'sors_x' non congruit) */
        lineae[II] = chorda_ex_literis("JsonValor* json_ad ("
            "JsonValor* valor);", pn);
        tituli[II] = chorda_ex_literis("json_ad", pn);
        lineae[III] = chorda_ex_literis("Sors sors (constans Sors* s,"
            " i64 sors_clavis);", pn);
        tituli[III] = chorda_ex_literis("sors", pn);
        /* nomen non inventum: linea tota */
        lineae[IV] = chorda_ex_literis("int fn(void);", pn);
        tituli[IV] = chorda_ex_literis("alia", pn);
        CREDO_CHORDA_AEQUALIS_LITERIS (briar_bibliotheca_functiones(
            lineae, tituli, V, pn),
            "JsonResultus json_legere (chorda input, Piscina*"
            " piscina);\n"
            "chorda       json_scribere_pulchrum (JsonValor* valor,"
            " Piscina* piscina);\n"
            "JsonValor*   json_ad (JsonValor* valor);\n"
            "Sors         sors (constans Sors* s, i64 sors_clavis);\n"
            "int fn(void);\n");
        /* typus ultra tectum XXIV: spatium unum, ceteri alineati */
        lineae[ZEPHYRUM] = chorda_ex_literis("constans"
            " TabulariumConfiguratio* cfg_x (i32 n);", pn);
        tituli[ZEPHYRUM] = chorda_ex_literis("cfg_x", pn);
        lineae[I] = chorda_ex_literis("b32 cfg_y (vacuum);", pn);
        tituli[I] = chorda_ex_literis("cfg_y", pn);
        CREDO_CHORDA_AEQUALIS_LITERIS (briar_bibliotheca_functiones(
            lineae, tituli, II, pn),
            "constans TabulariumConfiguratio* cfg_x (i32 n);\n"
            "b32 cfg_y (vacuum);\n");
        CREDO_AEQUALIS_I32 (briar_bibliotheca_functiones(lineae, tituli,
            ZEPHYRUM, pn).mensura, ZEPHYRUM);
        /* nomen ut cauda verbi alieni ('xar_numerus') non congruit:
         * linea tota, sine scissione */
        lineae[ZEPHYRUM] = chorda_ex_literis("i32 xar_numerus (Xar*"
            " numerus_x);", pn);
        tituli[ZEPHYRUM] = chorda_ex_literis("numerus", pn);
        CREDO_CHORDA_AEQUALIS_LITERIS (briar_bibliotheca_functiones(
            lineae, tituli, I, pn),
            "i32 xar_numerus (Xar* numerus_x);\n");
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(pn);
    redde praeteritus ? ZEPHYRUM : I;
}
