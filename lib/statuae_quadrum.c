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

b32
statuae_picturam_dithering (
    constans i8* rgba,
    constans i8* larva,
            i32  latitudo,
            i32  altitudo,
   constans b32* activi,
             i8* indices,
        Piscina* piscina)
{
               DitheringFructus  f;
    DitheringExemplariaOptiones  optiones;
                            i32  x;
                            i32  y;
                            i32  lat;
                            i32  alt;
                            i32  linea;
                            i32  columna;
                             i8* praecisa;
                             i8* larva_praecisa;

    si (!rgba || !larva || !activi || !indices || !piscina)
    {
        redde FALSUM;
    }
    si (!statuae_fines_tecti(larva, latitudo, altitudo, &x, &y, &lat,
        &alt))
    {
        redde VERUM;
    }
    praecisa        = (i8*)piscina_allocare(piscina,
        (memoriae_index)lat * alt * IV);
    larva_praecisa  = (i8*)piscina_allocare(piscina,
        (memoriae_index)lat * alt);
    si (!praecisa || !larva_praecisa)
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < alt; linea++)
    {
        memcpy(praecisa + (memoriae_index)linea * lat * IV,
            rgba + ((memoriae_index)(y + linea) * latitudo + x) * IV,
            (memoriae_index)lat * IV);
        memcpy(larva_praecisa + (memoriae_index)linea * lat,
            larva + (memoriae_index)(y + linea) * latitudo + x,
            (memoriae_index)lat);
    }
    dithering_exemplaria_optiones_ordinariae(&optiones);
    optiones.praesupputatio_hachurarum
        = STATUAE_PRAESUPPUTATIO_HACHURARUM;
    optiones.praesupputatio_pictographica
        = STATUAE_PRAESUPPUTATIO_PICTOGRAMMATUM;
    f = dithering_exemplaria_velox_cum_larva(praecisa, lat, alt, activi,
        &optiones, NIHIL, larva_praecisa, piscina);
    si (!f.successus || !f.indices)
    {
        redde FALSUM;
    }
    per (linea = ZEPHYRUM; linea < alt; linea++)
    {
        per (columna = ZEPHYRUM; columna < lat; columna++)
        {
            si (larva_praecisa[(memoriae_index)linea * lat + columna])
            {
                indices[(memoriae_index)(y + linea) * latitudo + x
                    + columna]
                    = f.indices[(memoriae_index)linea * lat + columna];
            }
        }
    }
    redde VERUM;
}

/* exemplaria intra fines tecti solum (statuae_picturam_dithering);
 * pixela tecta in exitum ut colores palettae. Pixela non tecta exitus
 * non scribuntur (vocans chartam ibi ponit). FALSUM si dithering
 * deficit */
interior b32
_exemplaria_intra_fines (
    constans StatuaPictura* pictura,
                        i8* exitus,
                   Piscina* piscina)
{
    memoriae_index pixela = (memoriae_index)pictura->latitudo
        * pictura->altitudo;
    memoriae_index  k;
               b32  activi[XVI];
                i8* indices;

    indices = (i8*)piscina_allocare(piscina, pixela);
    si (!indices)
    {
        redde FALSUM;
    }
    memset(indices, ZEPHYRUM, pixela);
    dithering_praeparare_omnes(activi);
    si (!statuae_picturam_dithering(pictura->rgba, pictura->tectum,
        pictura->latitudo, pictura->altitudo, activi, indices, piscina))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        si (pictura->tectum[k])
        {
            dithering_indices_ad_rgba(indices + k, exitus + k * IV, I,
                I);
        }
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
