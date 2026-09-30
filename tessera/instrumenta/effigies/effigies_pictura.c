/* effigies_pictura.c - Vide effigies_pictura.h */

#include "effigies_pictura.h"
#include "utf8.h"
#include "dithering.h"

vacuum
effigies_mensurare (
              i32  img_lat,
              i32  img_alt,
              i32  max_lat,
              i32  max_alt,
    QuadransModus  modus,
              i32  aspectus,
              i32* sub_lat,
              i32* sub_alt)
{
    /* sub-pixela per cellulam: QUADRANTES 2x2, DIMIDIUM 1x2 */
    i64 sx     = (modus == QUADRANS_DIMIDIUM) ? I : II;
    i64 sy     = II;
    i64 max_w  = (i64)max_lat * sx;
    i64 max_h  = (i64)max_alt * sy;
    i64 w;
    i64 h;
    i64 dividendus;
    i64 divisor;

    *sub_lat = ZEPHYRUM;
    *sub_alt = ZEPHYRUM;
    si (   img_lat == ZEPHYRUM || img_alt == ZEPHYRUM
        || max_w   == ZEPHYRUM
        || max_h   == ZEPHYRUM || aspectus == ZEPHYRUM)
    {
        redde;
    }
    /* latitudo apparens sub-pixeli / altitudo = aspectus * sy /
     * (C * sx); ergo w / h = (img_lat / img_alt) / illa */
    h           = max_h;
    dividendus  = h * (i64)img_lat * C * sx;
    divisor     = (i64)img_alt * (i64)aspectus * sy;
    w           = (dividendus + divisor / II) / divisor;
    si (w > max_w)
    {
        w           = max_w;
        dividendus  = w * (i64)img_alt * (i64)aspectus * sy;
        divisor     = (i64)img_lat * C * sx;
        h           = (dividendus + divisor / II) / divisor;
    }
    *sub_lat = (i32)(w < I ? I : w);
    *sub_alt = (i32)(h < I ? I : h);
}

vacuum
effigies_pingere (
                 TesseraOpus* opus,
                         s32  x,
                         s32  y,
    constans QuadransCellula* cellulae,
                         i32  lat,
                         i32  alt)
{
    i32 i;
    i32 j;

    per (j = ZEPHYRUM; j < alt; j++)
    {
        per (i = ZEPHYRUM; i < lat; i++)
        {
             constans QuadransCellula* c = &cellulae[j * lat + i];
                                   i8  octeti[IV];
                                  s32  n = utf8_codere(c->runa, octeti);

            tessera_cellulam_ponere(opus, x + (s32)i, y + (s32)j,
                tessera_signum_ex_octetis(octeti,
                    n > ZEPHYRUM ? (i32)n : ZEPHYRUM),
                tessera_stilus(c->color_litterae, c->color_fundi,
                    ZEPHYRUM));
        }
    }
}

Imago
effigies_palettam_applicare (
    constans Imago* imago,
           Piscina* piscina)
{
               Imago nova;
                 b32 colores[AQUINAS_COLORUM_NUMERUS];
    DitheringFructus fructus;

    nova.latitudo  = imago->latitudo;
    nova.altitudo  = imago->altitudo;
    nova.pixela    = NIHIL;
    dithering_praeparare_omnes(colores);
    fructus = dithering_atkinson_colorum(imago->pixela, imago->latitudo,
        imago->altitudo, colores, piscina);
    si (!fructus.successus || fructus.indices == NIHIL)
    {
        redde nova;
    }
    nova.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)imago->latitudo
            * (memoriae_index)imago->altitudo * IV);
    si (nova.pixela != NIHIL)
    {
        dithering_indices_ad_rgba(fructus.indices, nova.pixela,
            imago->latitudo, imago->altitudo);
    }
    redde nova;
}
