/* tessera_pons_memoriae.c - Implementatio pontis memoriae (Phase A) */

#include "tessera_pons_memoriae.h"
#include <string.h>

interior s32
_legere (vacuum* datum, i8* buffer, i32 capacitas, s32 mora_ms)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;
    i32 reliqui;
    i32 n;

    (vacuum)mora_ms;  /* scriptum: mora numquam expectatur */
    si (pm == NIHIL || buffer == NIHIL || capacitas == ZEPHYRUM)
    {
        redde -I;
    }
    reliqui = pm->initus_mensura - pm->initus_cursor;
    si (reliqui == ZEPHYRUM)
    {
        redde ZEPHYRUM;  /* exhaustum = mora exacta */
    }
    n = (reliqui < capacitas) ? reliqui : capacitas;
    memcpy(buffer, pm->initus + pm->initus_cursor, (memoriae_index)n);
    pm->initus_cursor += n;
    redde (s32)n;
}

interior b32
_scribere (vacuum* datum, constans i8* octeti, i32 numerus)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;
    i32 k;

    si (pm == NIHIL || octeti == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        chorda_aedificator_appendere_character(pm->captum,
            (character)octeti[k]);
    }
    redde VERUM;
}

interior b32
_amplitudo (vacuum* datum, i32* latitudo_out, i32* altitudo_out)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL || latitudo_out == NIHIL || altitudo_out == NIHIL)
    {
        redde FALSUM;
    }
    *latitudo_out = pm->latitudo;
    *altitudo_out = pm->altitudo;
    redde VERUM;
}

interior b32
_intrare (vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL)
    {
        redde FALSUM;
    }
    pm->intratum = VERUM;
    pm->numerus_intratum++;
    redde VERUM;
}

interior b32
_resumptum (vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL || !pm->resumendum)
    {
        redde FALSUM;
    }
    pm->resumendum = FALSUM;
    redde VERUM;
}

interior b32
_egredi (vacuum* datum)
{
    TesseraPonsMemoriae* pm = (TesseraPonsMemoriae*)datum;

    si (pm == NIHIL)
    {
        redde FALSUM;
    }
    pm->intratum = FALSUM;
    pm->numerus_exitum++;
    redde VERUM;
}

TesseraPonsMemoriae*
tessera_pons_memoriae_creare (Piscina* piscina, i32 latitudo,
    i32 altitudo)
{
    TesseraPonsMemoriae* pm;

    si (piscina == NIHIL || latitudo == ZEPHYRUM
        || altitudo == ZEPHYRUM)
    {
        redde NIHIL;
    }
    pm = (TesseraPonsMemoriae*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(TesseraPonsMemoriae), IV);
    si (pm == NIHIL)
    {
        redde NIHIL;
    }
    pm->piscina = piscina;
    pm->initus = NIHIL;
    pm->initus_mensura = ZEPHYRUM;
    pm->initus_cursor = ZEPHYRUM;
    pm->captum = chorda_aedificator_creare(piscina, 16384);
    pm->latitudo = latitudo;
    pm->altitudo = altitudo;
    pm->intratum = FALSUM;
    pm->resumendum = FALSUM;
    pm->numerus_intratum = ZEPHYRUM;
    pm->numerus_exitum = ZEPHYRUM;
    si (pm->captum == NIHIL)
    {
        redde NIHIL;
    }
    pm->pons.datum = pm;
    pm->pons.legere = _legere;
    pm->pons.scribere = _scribere;
    pm->pons.amplitudo = _amplitudo;
    pm->pons.intrare = _intrare;
    pm->pons.egredi = _egredi;
    pm->pons.resumptum = _resumptum;
    redde pm;
}

b32
tessera_pons_memoriae_initum (TesseraPonsMemoriae* pm,
    constans i8* octeti, i32 mensura)
{
    i8* copia;

    si (pm == NIHIL || (octeti == NIHIL && mensura > ZEPHYRUM))
    {
        redde FALSUM;
    }
    copia = (i8*)piscina_allocare(pm->piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (copia == NIHIL)
    {
        redde FALSUM;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(copia, octeti, (memoriae_index)mensura);
    }
    pm->initus = copia;
    pm->initus_mensura = mensura;
    pm->initus_cursor = ZEPHYRUM;
    redde VERUM;
}

chorda
tessera_pons_memoriae_captum (TesseraPonsMemoriae* pm)
{
    redde chorda_aedificator_spectare(pm->captum);
}

vacuum
tessera_pons_memoriae_purgare (TesseraPonsMemoriae* pm)
{
    chorda_aedificator_reset(pm->captum);
}

vacuum
tessera_pons_memoriae_amplitudo (TesseraPonsMemoriae* pm,
    i32 latitudo, i32 altitudo)
{
    si (pm == NIHIL)
    {
        redde;
    }
    pm->latitudo = latitudo;
    pm->altitudo = altitudo;
}
