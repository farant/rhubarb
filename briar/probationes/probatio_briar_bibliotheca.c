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

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(pn);
    redde praeteritus ? ZEPHYRUM : I;
}
