/* musivum_pictura.c - Vide musivum_pictura.h */

#include "musivum_pictura.h"
#include "utf8.h"

vacuum
musivum_pingere (
                    TesseraOpus* opus,
                            s32  x,
                            s32  y,
    constans TessellatioCellula* cellulae,
                            s32  columnae,
                            s32  lineae)
{
    s32 i;
    s32 j;

    per (j = ZEPHYRUM; j < lineae; j++)
    {
        per (i = ZEPHYRUM; i < columnae; i++)
        {
            constans TessellatioCellula* c = &cellulae[j * columnae
                + i];
                          TesseraStilus stilus;
                                    i32 lat;
                                    s32 runa;
                                     i8 octeti[IV];
                                    s32 n;

            si (c->latitudo == ZEPHYRUM)
            {
                perge;   /* continuatio: unitas lata eam posuit */
            }
            stilus = tessera_stilus(c->color_litterae, c->color_fundi,
                ZEPHYRUM);
            si (c->unitas != NIHIL)
            {
                (vacuum)tessera_graphema_ponere(opus, x + i, y + j,
                    c->unitas, c->unitas + c->mensura, stilus, &lat);
                perge;
            }
            runa = tessellatio_runa_juncturae(c->juncturae);
            si (runa > ZEPHYRUM)
            {
                n = utf8_codere(runa, octeti);
                tessera_cellulam_ponere(opus, x + i, y + j,
                    tessera_signum_ex_octetis(octeti,
                        n > ZEPHYRUM ? (i32)n : ZEPHYRUM), stilus);
                perge;
            }
            tessera_cellulam_ponere(opus, x + i, y + j, (i32)' ',
                stilus);
        }
    }
}
