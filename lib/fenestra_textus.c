/* Redditionis textus pro fenestra */
#include "fenestra.h"
#include "fons_6x8.h"
#include <string.h>

/* Functio adiutorii pro pingendo characterem cum praecisione */
interior vacuum
pingere_characterem_praecisum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color,
                i32  praecisio_x,
                i32  praecisio_y,
                i32  praecisio_latitudo,
                i32  praecisio_altitudo)
{
             i8  ch;
    constans i8* glypha;
            i32  ordo, columna, pixelum_y, pixelum_x;
             i8  bits;

    ch = (i8)c;
    glypha = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        pixelum_y = y + ordo;
        /* Praetermittere si ordo extra aream praecisionis */
        si (pixelum_y < praecisio_y || pixelum_y >= praecisio_y + praecisio_altitudo) perge;

        bits = glypha[ordo];
        per (columna = ZEPHYRUM; columna < FONS_LATITUDO_REDDENDI; columna++)
        {
            pixelum_x = x + columna;
            /* Praetermittere si columna extra aream praecisionis */
            si (pixelum_x < praecisio_x || pixelum_x >= praecisio_x + praecisio_latitudo) perge;

            si (bits & (0x80 >> columna))
            {
                tabula_pixelorum_ponere_pixelum(tabula, pixelum_x, pixelum_y, color);
            }
        }
    }
}


vacuum
tabula_pixelorum_pingere_characterem (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color)
{
    i8 ch;
    constans i8* glypha;
    i32 ordo, columna;
    i8 bits;

    ch = (i8)c;
    glypha = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        bits = glypha[ordo];
        /* Reddere usque ad VIII pixela sed solum procedere cursor per VI */
        per (columna = ZEPHYRUM; columna < FONS_LATITUDO_REDDENDI; columna++)
        {
            si (bits & (0x80 >> columna))
            {
                tabula_pixelorum_ponere_pixelum(tabula, x + columna, y + ordo, color);
            }
        }
    }
}


vacuum
tabula_pixelorum_pingere_chordam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
    constans character* chorda,
                i32  color)
{
    i32 initium_x;

    initium_x = x;
    dum (*chorda)
    {
        si (*chorda == '\n')
        {
            y += FONS_ALTITUDO;
            x = initium_x;
        }
        alioquin
        {
            tabula_pixelorum_pingere_characterem(tabula, x, y, *chorda, color);
            x += FONS_LATITUDO;
        }
        chorda++;
    }
}


vacuum
tabula_pixelorum_pingere_characterem_scalatum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
          character  c,
                i32  color,
                i32  scala)
{
    i8 ch;
    constans i8* glypha;
    i32 ordo, columna, sy, sx;
    i8 bits;

    ch = (i8)c;
    glypha = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        bits = glypha[ordo];
        /* Reddere usque ad VIII pixela sed solum procedere cursor per VI */
        per (columna = ZEPHYRUM; columna < FONS_LATITUDO_REDDENDI; columna++)
        {
            si (bits & (0x80 >> columna))
            {
                /* Pingere pixelum scalatum */
                per (sy = ZEPHYRUM; sy < scala; sy++)
                {
                    per (sx = ZEPHYRUM; sx < scala; sx++)
                    {
                        tabula_pixelorum_ponere_pixelum(tabula,
                                                         x + columna * scala + sx,
                                                         y + ordo * scala + sy,
                                                         color);
                    }
                }
            }
        }
    }
}


vacuum
tabula_pixelorum_pingere_chordam_scalatam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
    constans character* chorda,
                i32  color,
                i32  scala)
{
    i32 initium_x;

    initium_x = x;
    dum (*chorda)
    {
        si (*chorda == '\n')
        {
            y += FONS_ALTITUDO * scala;
            x = initium_x;
        }
        alioquin
        {
            tabula_pixelorum_pingere_characterem_scalatum(tabula, x, y, *chorda, color, scala);
            x += FONS_LATITUDO * scala;
        }
        chorda++;
    }
}


vacuum
tabula_pixelorum_pingere_chordam_praecisum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
    constans character* chorda,
                i32  color,
                i32  praecisio_x,
                i32  praecisio_y,
                i32  praecisio_latitudo,
                i32  praecisio_altitudo)
{
    i32 initium_x;

    initium_x = x;
    dum (*chorda)
    {
        si (*chorda == '\n')
        {
            y += FONS_ALTITUDO;
            x = initium_x;
        }
        alioquin
        {
            /* Cessare pingendo si ultra aream praecisionis */
            si (x >= praecisio_x + praecisio_latitudo) frange;
            si (y >= praecisio_y + praecisio_altitudo) frange;

            /* Solum pingere si character saltem partim visibilis */
            si (x + FONS_LATITUDO >= praecisio_x && y + FONS_ALTITUDO >= praecisio_y)
            {
                pingere_characterem_praecisum(tabula, x, y, *chorda, color,
                                               praecisio_x, praecisio_y,
                                               praecisio_latitudo, praecisio_altitudo);
            }
            x += FONS_LATITUDO;
        }
        chorda++;
    }
}


i32
fons_latitudo_chordae (
    constans character* chorda,
                  i32  scala)
{
    i32 latitudo;
    i32 latitudo_maxima;

    latitudo = ZEPHYRUM;
    latitudo_maxima = ZEPHYRUM;

    dum (*chorda)
    {
        si (*chorda == '\n')
        {
            si (latitudo > latitudo_maxima) latitudo_maxima = latitudo;
            latitudo = ZEPHYRUM;
        }
        alioquin
        {
            latitudo += FONS_LATITUDO * scala;
        }
        chorda++;
    }

    redde latitudo > latitudo_maxima ? latitudo : latitudo_maxima;
}


i32
fons_altitudo_chordae (
    i32 scala)
{
    redde FONS_ALTITUDO * scala;
}
