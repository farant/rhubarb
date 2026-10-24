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

/* Pons memoriae (LXXX x XXIV): sectio quaeque suum creat (initus,
 * captum, amplitudo, numeri mutantur - status nullus communis); creatio
 * semel ASSERITUR in parare_suitae (ut olim ante blocos). */
interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm;

    pm = tessera_pons_memoriae_creare(c->piscina, LXXX, XXIV);
    CREDO_NON_NIHIL (pm);
}


/* ==================================================
 * PROBARE: amplitudo + mutatio
 * ================================================== */

interior vacuum
_amplitudinem_probare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(c->piscina,
        LXXX, XXIV);

    si (pm == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        i32 lat = ZEPHYRUM;
        i32 alt = ZEPHYRUM;

        CREDO_VERUM (pm->pons.amplitudo(pm->pons.datum, &lat, &alt));
        CREDO_AEQUALIS_I32 (lat, LXXX);
        CREDO_AEQUALIS_I32 (alt, XXIV);

        tessera_pons_memoriae_amplitudo(pm, XL, XII);
        CREDO_VERUM (pm->pons.amplitudo(pm->pons.datum, &lat, &alt));
        CREDO_AEQUALIS_I32 (lat, XL);
        CREDO_AEQUALIS_I32 (alt, XII);
    }
}


/* ==================================================
 * PROBARE: legere - scriptum portionibus, exhaustio = 0
 * ================================================== */

interior vacuum
_lectionem_probare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(c->piscina,
        LXXX, XXIV);

    si (pm == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
         i8 buffer[IV];
        s32 n;

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
}


/* ==================================================
 * PROBARE: scribere captum + purgare (buffer manet)
 * ================================================== */

interior vacuum
_capturam_probare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(c->piscina,
        LXXX, XXIV);

    si (pm == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        chorda captum;

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
}


/* ==================================================
 * PROBARE: intrare/egredi numerata
 * ================================================== */

interior vacuum
_intrare_probare (
    CredoContextus* c)
{
    TesseraPonsMemoriae* pm = tessera_pons_memoriae_creare(c->piscina,
        LXXX, XXIV);

    si (pm == NIHIL)
    {
        redde;   /* sine assertione: sectio VACUA, suita FRACTA */
    }
    {
        CREDO_FALSUM (pm->intratum);
        CREDO_VERUM (pm->pons.intrare(pm->pons.datum));
        CREDO_VERUM (pm->intratum);
        CREDO_VERUM (pm->pons.egredi(pm->pons.datum));
        CREDO_FALSUM (pm->intratum);
        CREDO_AEQUALIS_I32 (pm->numerus_intratum, I);
        CREDO_AEQUALIS_I32 (pm->numerus_exitum, I);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "amplitudinem", _amplitudinem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "lectionem scriptam", _lectionem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "capturam", _capturam_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "intrare/egredi", _intrare_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tessera_pons", SECTIONES,
        _suitam_parare, NIHIL);
}
