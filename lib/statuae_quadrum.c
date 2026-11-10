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

b32
statuae_fines_tecti (
    constans i8* tectum,
            i32  latitudo,
            i32  altitudo,
            i32* x,
            i32* y,
            i32* latitudo_finium,
            i32* altitudo_finium)
{
    s32 sinistrum  = (s32)latitudo;
    s32 summum     = (s32)altitudo;
    s32 dextrum    = -I;
    s32 infimum    = -I;
    s32 px;
    s32 py;

    si (!tectum || !x || !y || !latitudo_finium || !altitudo_finium)
    {
        redde FALSUM;
    }
    per (py = ZEPHYRUM; py < (s32)altitudo; py++)
    {
        constans i8* linea = tectum + (i32)py * latitudo;

        per (px = ZEPHYRUM; px < (s32)latitudo; px++)
        {
            si (!linea[px])
            {
                perge;
            }
            sinistrum  = px < sinistrum ? px : sinistrum;
            dextrum    = px > dextrum ? px : dextrum;
            summum     = py < summum ? py : summum;
            infimum    = py;
        }
    }
    si (dextrum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* margo; anguli extra ad reticulum; intra picturam */
    sinistrum  -= STATUAE_FINIUM_MARGO;
    summum     -= STATUAE_FINIUM_MARGO;
    dextrum    += STATUAE_FINIUM_MARGO + I;
    infimum    += STATUAE_FINIUM_MARGO + I;
    sinistrum  = sinistrum < ZEPHYRUM ? ZEPHYRUM
        : sinistrum / STATUAE_FINIUM_GRADUS * STATUAE_FINIUM_GRADUS;
    summum     = summum < ZEPHYRUM ? ZEPHYRUM
        : summum / STATUAE_FINIUM_GRADUS * STATUAE_FINIUM_GRADUS;
    dextrum    = (dextrum + STATUAE_FINIUM_GRADUS - I)
        / STATUAE_FINIUM_GRADUS * STATUAE_FINIUM_GRADUS;
    infimum    = (infimum + STATUAE_FINIUM_GRADUS - I)
        / STATUAE_FINIUM_GRADUS * STATUAE_FINIUM_GRADUS;
    dextrum = dextrum > (s32)latitudo ? (s32)latitudo : dextrum;
    infimum = infimum > (s32)altitudo ? (s32)altitudo : infimum;
    *x = (i32)sinistrum;
    *y = (i32)summum;
    *latitudo_finium = (i32)(dextrum - sinistrum);
    *altitudo_finium = (i32)(infimum - summum);
    redde VERUM;
}

/* exemplaria intra fines tecti solum: pictura praecisa dithering
 * accipit et in exitum reponitur. Extra fines exitus non scribitur
 * (vocans chartam ibi ponit). FALSUM si dithering deficit */
interior b32
_exemplaria_intra_fines (
    constans StatuaPictura* pictura,
                        i8* exitus,
                   Piscina* piscina)
{
    DitheringFructus  f;
                 b32  activi[XVI];
                 i32  x;
                 i32  y;
                 i32  lat;
                 i32  alt;
                 i32  linea;
                  i8* praecisa;
                  i8* picta;

    si (!statuae_fines_tecti(pictura->tectum, pictura->latitudo,
        pictura->altitudo, &x, &y, &lat, &alt))
    {
        redde VERUM;
    }
    praecisa = (i8*)piscina_allocare(piscina,
        (memoriae_index)lat * alt * IV);
    picta    = (i8*)piscina_allocare(piscina,
        (memoriae_index)lat * alt * IV);
    per (linea = ZEPHYRUM; linea < alt; linea++)
    {
        memcpy(praecisa + (memoriae_index)linea * lat * IV,
            pictura->rgba
                + ((memoriae_index)(y + linea) * pictura->latitudo + x)
                    * IV,
            (memoriae_index)lat * IV);
    }
    dithering_praeparare_omnes(activi);
    f = dithering_exemplaria_velox_colorum(praecisa, lat, alt, activi,
        NIHIL, NIHIL, piscina);
    si (!f.successus || !f.indices)
    {
        redde FALSUM;
    }
    dithering_indices_ad_rgba(f.indices, picta, lat, alt);
    per (linea = ZEPHYRUM; linea < alt; linea++)
    {
        memcpy(exitus
                + ((memoriae_index)(y + linea) * pictura->latitudo + x)
                    * IV,
            picta + (memoriae_index)linea * lat * IV,
            (memoriae_index)lat * IV);
    }
    redde VERUM;
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
            redde _exemplaria_intra_fines(pictura, exitus, piscina);
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
