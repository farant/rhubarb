/* saltuarius_res.c - Implementatio status app-latus */

#include "saltuarius_res.h"
#include "via.h"

SaltuariusRes*
saltuarius_res_creare (
               Piscina* piscina,
    constans character* radix)
{
    SaltuariusRes* res;

    res = (SaltuariusRes*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(SaltuariusRes), IV);
    si (res == NIHIL)
    {
        redde NIHIL;
    }
    res->piscina  = piscina;
    res->modus    = SALT_MODUS_COLUMNAE;
    res->radix = via_normalizare(
        chorda_ex_literis(radix, piscina), piscina);
    res->via_currens        = res->radix;
    res->currens            = VERUM;
    res->nuntius.mensura    = ZEPHYRUM;
    res->nuntius.datum      = NIHIL;
    res->nuntius_aetas      = ZEPHYRUM;
    res->fructus_visibilis  = FALSUM;
    redde res;
}

vacuum
saltuarius_res_nuntiare (
         SaltuariusRes* res,
    constans character* textus,
                   s32  quadra)
{
    res->nuntius        = chorda_ex_literis(textus, res->piscina);
    res->nuntius_aetas  = quadra;
}

b32
saltuarius_res_nuntius_senescere (
    SaltuariusRes* res)
{
    si (res->nuntius_aetas > ZEPHYRUM)
    {
        res->nuntius_aetas--;
    }
    redde (res->nuntius_aetas > ZEPHYRUM) ? VERUM : FALSUM;
}

vacuum
saltuarius_res_intrare (
    SaltuariusRes* res,
           chorda  titulus)
{
    chorda partes[II];

    partes[ZEPHYRUM]  = res->via_currens;
    partes[I]         = titulus;
    res->via_currens = via_normalizare(
        via_iungere(partes, II, res->piscina), res->piscina);
}

vacuum
saltuarius_res_ascendere (
    SaltuariusRes* res)
{
    res->via_currens = via_directorium(res->via_currens,
        res->piscina);
}
