/* probatio_tessera_pons.c - Pons memoriae: sutura probationum
 * (Phase A)
 *
 * Machina ficta: initus scriptus legitur portionibus et exhauritur
 * (0 = mora), exitus capitur et purgatur (buffer manet), amplitudo
 * mutabilis, intrare/egredi numerata.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tessera_pons.h"
#include "tessera_pons_memoriae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
    b32      praeteritus;
    Piscina* piscina;
    TesseraPonsMemoriae* pm;

    piscina = piscina_generare_dynamicum("probatio_tessera_pons",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    pm = tessera_pons_memoriae_creare(piscina, LXXX, XXIV);
    CREDO_NON_NIHIL (pm);

    /* ========================================================
     * PROBARE: amplitudo + mutatio
     * ======================================================== */
    {
        i32 lat = ZEPHYRUM;
        i32 alt = ZEPHYRUM;

        imprimere("\n--- Probans amplitudinem ---\n");

        CREDO_VERUM (pm->pons.amplitudo(pm->pons.datum, &lat, &alt));
        CREDO_AEQUALIS_I32 (lat, LXXX);
        CREDO_AEQUALIS_I32 (alt, XXIV);

        tessera_pons_memoriae_amplitudo(pm, XL, XII);
        CREDO_VERUM (pm->pons.amplitudo(pm->pons.datum, &lat, &alt));
        CREDO_AEQUALIS_I32 (lat, XL);
        CREDO_AEQUALIS_I32 (alt, XII);
    }

    /* ========================================================
     * PROBARE: legere - scriptum portionibus, exhaustio = 0
     * ======================================================== */
    {
        i8 buffer[IV];
        s32 n;

        imprimere("\n--- Probans lectionem scriptam ---\n");

        CREDO_VERUM (tessera_pons_memoriae_initum(pm,
            (constans i8*)"abcdef", VI));
        n = pm->pons.legere(pm->pons.datum, buffer, IV, C);
        CREDO_AEQUALIS_S32 (n, IV);
        CREDO_VERUM (memcmp(buffer, "abcd", IV) == ZEPHYRUM);
        n = pm->pons.legere(pm->pons.datum, buffer, IV, C);
        CREDO_AEQUALIS_S32 (n, II);
        CREDO_VERUM (memcmp(buffer, "ef", II) == ZEPHYRUM);
        n = pm->pons.legere(pm->pons.datum, buffer, IV, C);
        CREDO_AEQUALIS_S32 (n, ZEPHYRUM);  /* exhaustum = mora */

        /* scriptum novum: cursor redit */
        CREDO_VERUM (tessera_pons_memoriae_initum(pm,
            (constans i8*)"z", I));
        n = pm->pons.legere(pm->pons.datum, buffer, IV, C);
        CREDO_AEQUALIS_S32 (n, I);
        CREDO_AEQUALIS_I32 ((i32)buffer[ZEPHYRUM], (i32)'z');
    }

    /* ========================================================
     * PROBARE: scribere captum + purgare (buffer manet)
     * ======================================================== */
    {
        chorda captum;

        imprimere("\n--- Probans capturam ---\n");

        CREDO_VERUM (pm->pons.scribere(pm->pons.datum,
            (constans i8*)"\033[2J", IV));
        CREDO_VERUM (pm->pons.scribere(pm->pons.datum,
            (constans i8*)"xy", II));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "\033[2Jxy");

        tessera_pons_memoriae_purgare(pm);
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_AEQUALIS_I32 (captum.mensura, ZEPHYRUM);
        CREDO_VERUM (pm->pons.scribere(pm->pons.datum,
            (constans i8*)"q", I));
        captum = tessera_pons_memoriae_captum(pm);
        CREDO_CHORDA_AEQUALIS_LITERIS (captum, "q");
    }

    /* ========================================================
     * PROBARE: intrare/egredi numerata
     * ======================================================== */
    {
        imprimere("\n--- Probans intrare/egredi ---\n");

        CREDO_FALSUM (pm->intratum);
        CREDO_VERUM (pm->pons.intrare(pm->pons.datum));
        CREDO_VERUM (pm->intratum);
        CREDO_VERUM (pm->pons.egredi(pm->pons.datum));
        CREDO_FALSUM (pm->intratum);
        CREDO_AEQUALIS_I32 (pm->numerus_intratum, I);
        CREDO_AEQUALIS_I32 (pm->numerus_exitum, I);
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
