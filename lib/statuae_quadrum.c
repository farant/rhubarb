/* statuae_quadrum.c - quadrum statuae (vide statuae_quadrum.h) */

#include "statuae_quadrum.h"
#include "dithering.h"
#include "dithering_ordinatum.h"
#include "dithering_exemplaria_velox.h"
#include <string.h>

hic_manens constans character* constans MODORUM_TITULI[STATUAE_MODORUM]
    = { "atkinson", "bayer", "exemplaria", "nullum", "crudum" };

constans character*
statuae_modi_titulus (
    StatuaeModus modus)
{
    si ((s32)modus < ZEPHYRUM || modus >= STATUAE_MODORUM)
    {
        redde NIHIL;
    }
    redde MODORUM_TITULI[modus];
}

b32
statuae_modum_legere (
          chorda  titulus,
    StatuaeModus* modus)
{
    s32 k;

    si (!modus)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < (s32)STATUAE_MODORUM; k++)
    {
        si (chorda_aequalis_literis(titulus, MODORUM_TITULI[k]))
        {
            *modus = (StatuaeModus)k;
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* pictura cruda in palettam secundum modum; FALSUM si deficit */
interior b32
_dithering (
              StatuaeModus  modus,
    constans StatuaPictura* pictura,
                        i8* exitus,
                   Piscina* piscina)
{
    DitheringFructus f;
                 b32 activi[XVI];

    dithering_praeparare_omnes(activi);
    commutatio (modus)
    {
        casus STATUAE_MODUS_ATKINSON:
            f = dithering_atkinson_colorum(pictura->rgba,
                pictura->latitudo, pictura->altitudo, activi, piscina);
            frange;
        casus STATUAE_MODUS_BAYER:
            f = dithering_bayer_colorum(pictura->rgba,
                pictura->latitudo, pictura->altitudo, activi, piscina);
            frange;
        casus STATUAE_MODUS_EXEMPLARIA:
            f = dithering_exemplaria_velox_colorum(pictura->rgba,
                pictura->latitudo, pictura->altitudo, activi, NIHIL,
                NIHIL, piscina);
            frange;
        ordinarius:
            f = dithering_proximus_colorum(pictura->rgba,
                pictura->latitudo, pictura->altitudo, activi, piscina);
            frange;
    }
    si (!f.successus || !f.indices)
    {
        redde FALSUM;
    }
    dithering_indices_ad_rgba(f.indices, exitus, pictura->latitudo,
        pictura->altitudo);
    redde VERUM;
}

b32
statuae_quadrum_pingere (
                   constans Statua* statua,
    constans StatuaeQuadrumPetitio* petitio,
                             Imago* exitus,
                           Piscina* piscina)
{
    StereometriaMatrix* matrices;
         StatuaPictura  pictura;
        memoriae_index  pixela;
        memoriae_index  k;
                    i8* p;

    si (   !statua || !petitio || !exitus || !piscina
        || (s32)petitio->modus < ZEPHYRUM
        || petitio->modus >= STATUAE_MODORUM)
    {
        redde FALSUM;
    }
    exitus->pixela    = NIHIL;
    exitus->latitudo  = petitio->latitudo;
    exitus->altitudo  = petitio->altitudo;
    si (petitio->latitudo == ZEPHYRUM || petitio->altitudo == ZEPHYRUM)
    {
        redde VERUM;
    }
    matrices = (StereometriaMatrix*)piscina_allocare(piscina,
        (statua->nodorum + I) * magnitudo(StereometriaMatrix));
    statua_positura(statua, petitio->cyclus, petitio->tempus, matrices);
    si (!statua_pingere(statua, matrices, &petitio->visus,
        petitio->latitudo, petitio->altitudo, &pictura, piscina))
    {
        redde FALSUM;
    }
    pixela = (memoriae_index)petitio->latitudo * petitio->altitudo;
    si (petitio->modus == STATUAE_MODUS_CRUDUS)
    {
        exitus->pixela = pictura.rgba;
    }
    alioquin
    {
        exitus->pixela = (i8*)piscina_allocare(piscina, pixela * IV);
        si (!_dithering(petitio->modus, &pictura, exitus->pixela,
            piscina))
        {
            exitus->pixela = NIHIL;
            redde FALSUM;
        }
    }
    /* charta munda ubi statua non tegit; tecta alpha plena */
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        p = exitus->pixela + k * IV;
        si (pictura.tectum[k])
        {
            p[III] = 255;
            perge;
        }
        p[ZEPHYRUM]  = AQUINAS_PALETTE[STATUAE_CHARTA_INDEX][ZEPHYRUM];
        p[I]         = AQUINAS_PALETTE[STATUAE_CHARTA_INDEX][I];
        p[II]        = AQUINAS_PALETTE[STATUAE_CHARTA_INDEX][II];
        p[III]       = petitio->charta_perlucida ? ZEPHYRUM : 255;
    }
    redde VERUM;
}
