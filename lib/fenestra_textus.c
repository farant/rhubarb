/* Tabula pixelorum PURA (tabula_pixelorum.h): pixela et textus - sine
 * fenestra, sine Cocoa */
#include "tabula_pixelorum.h"
#include "fons_6x8.h"
#include "utf8.h"
#include "fons.h"
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
             i8 bits;

    ch      = (i8)c;
    glypha  = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        pixelum_y = y + ordo;
        /* Praetermittere si ordo extra aream praecisionis */
        si (   pixelum_y < praecisio_y
            || pixelum_y >= praecisio_y + praecisio_altitudo) perge;

        bits = glypha[ordo];
        per (columna = ZEPHYRUM; columna
            < FONS_LATITUDO_REDDENDI; columna++)
        {
            pixelum_x = x + columna;
            /* Praetermittere si columna extra aream praecisionis */
            si (   pixelum_x < praecisio_x
                || pixelum_x >= praecisio_x + praecisio_latitudo) perge;

            si (bits & (0x80 >> columna))
            {
                tabula_pixelorum_ponere_pixelum(tabula, pixelum_x,
                    pixelum_y, color);
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
             i8  ch;
    constans i8* glypha;
    i32 ordo, columna;
    i8 bits;

    ch      = (i8)c;
    glypha  = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        bits = glypha[ordo];
        /* Reddere usque ad VIII pixela sed solum procedere cursor per VI */
        per (columna = ZEPHYRUM; columna
            < FONS_LATITUDO_REDDENDI; columna++)
        {
            si (bits & (0x80 >> columna))
            {
                tabula_pixelorum_ponere_pixelum(tabula, x + columna, y
                    + ordo, color);
            }
        }
    }
}

vacuum
tabula_pixelorum_cellula_textus (
    i32* latitudo,
    i32* altitudo)
{
    *latitudo = FONS_LATITUDO;
    *altitudo = FONS_ALTITUDO;
}

vacuum
tabula_pixelorum_pingere_chordam (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color)
{
            i32  initium_x;
    constans i8* ptr;
    constans i8* finis;

    initium_x  = x;
    ptr        = textus.datum;
    finis      = textus.datum + textus.mensura;

    dum (ptr < finis)
    {
        s32 runa;
         i8 glypha;

        runa = utf8_decodere(&ptr, finis);

        /* Nova linea */
        si (runa == '\n')
        {
            y += FONS_ALTITUDO;
            x = initium_x;
        }
        alioquin
        {
            /* Mappare codepoint ad glypham */
            si (runa < ZEPHYRUM)
            {
                glypha = FONS_TOFU;
            }
            alioquin
            {
                glypha = fons_codepoint_ad_glypham(runa);
            }

            tabula_pixelorum_pingere_characterem(tabula, x, y,
                (character)glypha, color);
            x += FONS_LATITUDO;
        }
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
             i8  ch;
    constans i8* glypha;
    i32 ordo, columna, sy, sx;
    i8 bits;

    ch      = (i8)c;
    glypha  = fons_6x8[ch];

    per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
    {
        bits = glypha[ordo];
        /* Reddere usque ad VIII pixela sed solum procedere cursor per VI */
        per (columna = ZEPHYRUM; columna
            < FONS_LATITUDO_REDDENDI; columna++)
        {
            si (bits & (0x80 >> columna))
            {
                /* Pingere pixelum scalatum */
                per (sy = ZEPHYRUM; sy < scala; sy++)
                {
                    per (sx = ZEPHYRUM; sx < scala; sx++)
                    {
                        tabula_pixelorum_ponere_pixelum(tabula,
                                                         x
                                                             + columna * scala
                                                             + sx,
                                                         y
                                                             + ordo * scala
                                                             + sy,
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
             chorda  textus,
                i32  color,
                i32  scala)
{
            i32  initium_x;
    constans i8* ptr;
    constans i8* finis;

    initium_x  = x;
    ptr        = textus.datum;
    finis      = textus.datum + textus.mensura;

    dum (ptr < finis)
    {
        s32 runa;
         i8 glypha;

        runa = utf8_decodere(&ptr, finis);

        si (runa == '\n')
        {
            y += FONS_ALTITUDO * scala;
            x = initium_x;
        }
        alioquin
        {
            si (runa < ZEPHYRUM)
            {
                glypha = FONS_TOFU;
            }
            alioquin
            {
                glypha = fons_codepoint_ad_glypham(runa);
            }

            tabula_pixelorum_pingere_characterem_scalatum(tabula, x, y,
                (character)glypha, color, scala);
            x += FONS_LATITUDO * scala;
        }
    }
}

vacuum
tabula_pixelorum_pingere_chordam_praecisum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  praecisio_x,
                i32  praecisio_y,
                i32  praecisio_latitudo,
                i32  praecisio_altitudo)
{
            i32  initium_x;
    constans i8* ptr;
    constans i8* finis;

    initium_x  = x;
    ptr        = textus.datum;
    finis      = textus.datum + textus.mensura;

    dum (ptr < finis)
    {
        s32 runa;
         i8 glypha;

        runa = utf8_decodere(&ptr, finis);

        si (runa == '\n')
        {
            y += FONS_ALTITUDO;
            x = initium_x;
        }
        alioquin
        {
            /* Cessare pingendo si ultra aream praecisionis */
            si (x >= praecisio_x + praecisio_latitudo) frange;
            si (y >= praecisio_y + praecisio_altitudo) frange;

            /* Mappare codepoint ad glypham */
            si (runa < ZEPHYRUM)
            {
                glypha = FONS_TOFU;
            }
            alioquin
            {
                glypha = fons_codepoint_ad_glypham(runa);
            }

            /* Solum pingere si character saltem partim visibilis */
            si (   x + FONS_LATITUDO >= praecisio_x
                && y + FONS_ALTITUDO >= praecisio_y)
            {
                pingere_characterem_praecisum(tabula, x, y,
                    (character)glypha, color,
                                               praecisio_x, praecisio_y,
                                               praecisio_latitudo,
                                               praecisio_altitudo);
            }
            x += FONS_LATITUDO;
        }
    }
}

i32
fons_latitudo_chordae (
             chorda textus,
                i32 scala)
{
            i32  latitudo;
            i32  latitudo_maxima;
    constans i8* ptr;
    constans i8* finis;

    latitudo         = ZEPHYRUM;
    latitudo_maxima  = ZEPHYRUM;
    ptr              = textus.datum;
    finis            = textus.datum + textus.mensura;

    dum (ptr < finis)
    {
        s32 runa;

        runa = utf8_decodere(&ptr, finis);

        si (runa == '\n')
        {
            si (latitudo > latitudo_maxima) latitudo_maxima = latitudo;
            latitudo = ZEPHYRUM;
        }
        alioquin
        {
            /* Omnis runa (etiam TOFU) habet eandem latitudinem */
            latitudo += FONS_LATITUDO * scala;
        }
    }

    redde latitudo > latitudo_maxima ? latitudo : latitudo_maxima;
}

i32
fons_altitudo_chordae (
    i32 scala)
{
    redde FONS_ALTITUDO * scala;
}


/* ==================================================
 * Pixela (ex fenestra_macos.m motae, tessellatio T4)
 * ================================================== */

vacuum
tabula_pixelorum_vacare (
    TabulaPixelorum* tabula,
                i32  color)
{
    i32 pixela_totalia;
    i32 i;

    si (!tabula || !tabula->pixela) redde;

    pixela_totalia = tabula->latitudo * tabula->altitudo;
    per (i = ZEPHYRUM; i < pixela_totalia; i++)
    {
        tabula->pixela[i] = color;
    }
}

vacuum
tabula_pixelorum_ponere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
                i32  color)
{
    si (!tabula || !tabula->pixela) redde;
    si (   x < ZEPHYRUM || x >= tabula->latitudo || y < ZEPHYRUM
        || y
                                                                                                                          >= tabula->altitudo) redde;

    tabula->pixela[y * tabula->latitudo + x] = color;
}

i32
tabula_pixelorum_obtinere_pixelum (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y)
{
    si (!tabula || !tabula->pixela) redde ZEPHYRUM;
    si (   x < ZEPHYRUM || x >= tabula->latitudo || y < ZEPHYRUM
        || y
                                                                                                                          >= tabula->altitudo) redde ZEPHYRUM;

    redde tabula->pixela[y * tabula->latitudo + x];
}

b32
tabula_pixelorum_ad_fenestram (
    TabulaPixelorum* tabula,
            Piscina* piscina,
                i32  fenestra_latitudo,
                i32  fenestra_altitudo)
{
    i32  latitudo;
    i32  altitudo;
    i32  opus;
    i32* nova;

    si (!tabula || !piscina || tabula->scala <= 0.0f)
    {
        redde FALSUM;
    }
    latitudo = (i32)((f32)fenestra_latitudo / tabula->scala);
    altitudo = (i32)((f32)fenestra_altitudo / tabula->scala);
    si (latitudo < I)
    {
        latitudo = I;
    }
    si (altitudo < I)
    {
        altitudo = I;
    }
    opus = latitudo * altitudo;
    si (opus > tabula->capacitas)
    {
        nova = (i32*)piscina_allocare(piscina,
            (memoriae_index)opus * magnitudo(i32));
        si (!nova)
        {
            redde FALSUM;
        }
        tabula->pixela     = nova;
        tabula->capacitas  = opus;
    }
    tabula->latitudo           = latitudo;
    tabula->altitudo           = altitudo;
    tabula->fenestra_latitudo  = fenestra_latitudo;
    tabula->fenestra_altitudo  = fenestra_altitudo;
    redde VERUM;
}

b32
tabula_pixelorum_in_imaginem (
    constans TabulaPixelorum* tabula,
                     Piscina* piscina,
                       Imago* imago)
{
     i8* octeti;
    i32  n;
    i32  i;
    i32  p;

    si (   !tabula || !piscina || !imago || !tabula->pixela
        || tabula->latitudo <= ZEPHYRUM || tabula->altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    n       = tabula->latitudo * tabula->altitudo;
    octeti  = (i8*)piscina_conari_allocare(piscina,
        (memoriae_index)n * IV);
    si (!octeti)
    {
        redde FALSUM;
    }
    /* RGBA(r, g, b, a) = a<<24 | b<<16 | g<<8 | r */
    per (i = ZEPHYRUM; i < n; i++)
    {
        p                     = tabula->pixela[i];
        octeti[i * IV]        = (i8)(p & 0xFF);
        octeti[i * IV + I]    = (i8)((p >> VIII) & 0xFF);
        octeti[i * IV + II]   = (i8)((p >> XVI) & 0xFF);
        octeti[i * IV + III]  = (i8)((p >> XXIV) & 0xFF);
    }
    imago->pixela    = octeti;
    imago->latitudo  = tabula->latitudo;
    imago->altitudo  = tabula->altitudo;
    redde VERUM;
}
