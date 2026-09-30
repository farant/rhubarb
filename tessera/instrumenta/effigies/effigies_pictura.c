/* effigies_pictura.c - Vide effigies_pictura.h */

#include "effigies_pictura.h"
#include "utf8.h"
#include "dithering.h"

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
