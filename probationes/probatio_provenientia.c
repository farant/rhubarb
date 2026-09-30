/* probatio_provenientia.c - relatio '-provenientia' binariorum
 * installatorum (plan fabrica 1a T7). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "provenientia.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
                     b32  praeteritus;
                 Piscina* piscina;
     ProvenientiaRelatio  relatio;

    piscina = piscina_generare_dynamicum("probatio_provenientia",
        16384);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    relatio.artificium = "bin/manus";
    relatio.ingressus = "0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdef";
    relatio.commissum = "abc12345 SORDIDUM";


    /* ==================================================
     * PROBARE: textus - quattuor lineae exactae
     * ================================================== */

    {
        chorda textus;

        imprimere("\n--- Probans textum ---\n");
        textus = provenientia_textus(&relatio, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(textus,
            "provenientia 1\n"
            "artificium bin/manus\n"
            "ingressus 0123456789abcdef0123456789abcdef"
            "0123456789abcdef0123456789abcdef\n"
            "commissum abc12345 SORDIDUM\n");
    }


    /* ==================================================
     * PROBARE: respondere - vexillum solum, argumentum unicum
     * ================================================== */

    {
        character* sine[II];
        character* cum[II];
        character* cum_alio[III];
        character  nomen_binarii[16];
        character  vexillum[16];
        character  aliud[16];

        imprimere("\n--- Probans respondere ---\n");
        strcpy(nomen_binarii, "bin/manus");
        strcpy(vexillum, "-provenientia");
        strcpy(aliud, "-versio");
        sine[0]      = nomen_binarii;
        sine[1]      = aliud;
        cum[0]       = nomen_binarii;
        cum[1]       = vexillum;
        cum_alio[0]  = nomen_binarii;
        cum_alio[1]  = vexillum;
        cum_alio[2]  = aliud;

        CREDO_FALSUM(provenientia_respondere(I, sine, &relatio));
        CREDO_FALSUM(provenientia_respondere(II, sine, &relatio));
        /* vexillum cum argumentis aliis = usus alius, non relatio */
        CREDO_FALSUM(provenientia_respondere(III, cum_alio, &relatio));
        CREDO_VERUM(provenientia_respondere(II, cum, &relatio));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
