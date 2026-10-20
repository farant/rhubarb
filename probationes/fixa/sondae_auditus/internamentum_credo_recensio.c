#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
    piscina = piscina_generare_dynamicum("probatio_internamentum", CDLVI);
    credo_aperire(piscina);

    {
        InternamentumChorda* intern;
                     chorda  mala;

        imprimere("\n--- Probans chorda_est_internata (datum NIHIL, mensura > 0) ---\n");

        intern        = internamentum_creare(piscina);
        mala.datum    = NIHIL;
        mala.mensura  = V;

        CREDO_NIHIL(chorda_internare(intern, mala));
        CREDO_NON_RUIT(chorda_est_internata(intern, mala));
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);
    }

    {
        InternamentumChorda* intern;
                     chorda  vacua;
             memoriae_index  usus_ante;
             memoriae_index  usus_post;
                        s32  k;

        imprimere("\n--- Probans recusationem vacuam sine allocatione ---\n");

        intern = internamentum_creare(piscina);
        vacua  = chorda_ex_literis("x", piscina);
        vacua.mensura = ZEPHYRUM;

        usus_ante = piscina_summa_usus(piscina);
        per (k = ZEPHYRUM; k < M; k++)
        {
            CREDO_NIHIL(chorda_internare_ex_literis(intern, ""));
            CREDO_NIHIL(chorda_internare(intern, vacua));
        }
        usus_post = piscina_summa_usus(piscina);

        CREDO_VERUM(usus_post == usus_ante);
        CREDO_AEQUALIS_I32(internamentum_numerus(intern), ZEPHYRUM);
    }

    credo_imprimere_compendium();
    {
        b32 ok = credo_omnia_praeterierunt();
        credo_claudere();
        piscina_destruere(piscina);
        redde ok ? ZEPHYRUM : I;
    }
}
