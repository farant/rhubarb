/* probatio_silva_salve.c - Probatio trivialis: harnesa aedificationis viva est */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_silva_salve", 4096);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: harnesa viva (arithmetica trivialis)
     * ======================================================== */

    {
        imprimere("\n--- Probans harnesam silvae ---\n");

        CREDO_AEQUALIS_I32 (I + I, II);
        CREDO_VERUM (VERUM);
        CREDO_FALSUM (FALSUM);
    }


    /* ========================================================
     * PROBARE: bibliothecae radicis attingibiles (chorda)
     * ======================================================== */

    {
        chorda salve;

        imprimere("\n--- Probans bibliothecas radicis ---\n");

        salve = chorda_ex_literis("silva crescat", piscina);
        CREDO_AEQUALIS_I32 (salve.mensura, XIII);
        CREDO_CHORDA_AEQUALIS_LITERIS (salve, "silva crescat");
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
