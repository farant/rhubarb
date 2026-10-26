/* Tabula pixelorum PURA (tabula_pixelorum.h): pixela et textus - sine
 * fenestra, sine Cocoa */
#include "tabula_pixelorum.h"
#include "fons_6x8.h"
#include "fons_8x16.h"
#include "utf8.h"
#include "fons.h"
#include "glyphae_ductae.h"
#include <string.h>

/* pixelum RGBA8888 (a<<24 | b<<16 | g<<8 | r): fundus in litteram per
 * opacitatem (0..CCLV) mixtus, canalis quisque */
interior i32
pixela_miscere (
    i32 fundus,
    i32 littera,
    i32 opacitas)
{
    i32 fructus;
    i32 k;
    i32 a;
    i32 b;

    fructus = littera & 0xFF000000u;
    per (k = ZEPHYRUM; k < XXIV; k += VIII)
    {
        a = (fundus >> k) & 0xFFu;
        b = (littera >> k) & 0xFFu;
        fructus |= ((a * (CCLV - opacitas) + b * opacitas) / CCLV) << k;
    }
    redde fructus;
}

/* runa ducta (glyphae_ductae, ut terminale: capsae, quadra, braille,
 * symbola) in cellula cw x ch, scala: opacitas plena = color, partialis
 * cum pixelo subiecto mixta. FALSUM si runa non ducta (vocans fontem
 * adhibet) */
interior b32
glypham_ductam_pingere (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
                s32  runa,
                i32  cw,
                i32  ch,
                i32  scala,
                i32  color)
{
     i8 larva[FONS_8X16_LATITUDO * FONS_8X16_ALTITUDO];
    i32 px;
    i32 py;
    i32 sx;
    i32 sy;
    i32 v;
    i32 qx;
    i32 qy;

    si (   runa < ZEPHYRUM || cw * ch > (i32)magnitudo(larva)
        || !glyphae_ductae_est(runa)
        || !glyphae_ductae_pingere(runa, cw, ch, larva))
    {
        redde FALSUM;
    }
    per (py = ZEPHYRUM; py < ch; py++)
    {
        per (px = ZEPHYRUM; px < cw; px++)
        {
            v = (i32)larva[py * cw + px];
            si (v == ZEPHYRUM)
            {
                perge;
            }
            per (sy = ZEPHYRUM; sy < scala; sy++)
            {
                per (sx = ZEPHYRUM; sx < scala; sx++)
                {
                    qx = x + px * scala + sx;
                    qy = y + py * scala + sy;
                    tabula_pixelorum_ponere_pixelum(tabula, qx, qy,
                        v == CCLV ? color : pixela_miscere(
                        tabula_pixelorum_obtinere_pixelum(tabula, qx,
                        qy),
                        color, v));
                }
            }
        }
    }
    redde VERUM;
}

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

    /* VIA CELERIS (terminale park 011): glypha tota intra tabulam -
     * ordines directe, sine probatione finium per pixelum. i32 sine
     * signo: x negativum ingens advenit, ergo 'x + VIII <= latitudo'
     * circumvolveret (ruina in probatio_delineare_mandata) - comparatio
     * per subtractionem a latitudine */
    si (   tabula && tabula->pixela
        && tabula->latitudo >= FONS_LATITUDO_REDDENDI
        && tabula->altitudo >= FONS_ALTITUDO
        && x                <= tabula->latitudo - FONS_LATITUDO_REDDENDI
        && y                <= tabula->altitudo - FONS_ALTITUDO)
    {
        i32* linea;

        per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
        {
            bits  = glypha[ordo];
            si (bits == ZEPHYRUM)
            {
                perge;
            }
            linea = tabula->pixela + (y + ordo) * tabula->latitudo + x;
            per (columna = ZEPHYRUM; columna
                < FONS_LATITUDO_REDDENDI; columna++)
            {
                si (bits & (0x80 >> columna))
                {
                    linea[columna] = color;
                }
            }
        }
        redde;
    }

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
        /* glyphae ductae (ut terminale) ante fontem */
        alioquin si (glypham_ductam_pingere(tabula, x, y, runa,
                     FONS_LATITUDO, FONS_ALTITUDO, I, color))
        {
            x += FONS_LATITUDO;
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

/* glypha 8x16 (fons_8x16, V3b): pixelum per pixelum, fines probantur */
interior vacuum
characterem_8x16_pingere (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
                 i8  glypha,
                i32  color)
{
    i32 ordo;
    i32 columna;
     i8 bits;

    per (ordo = ZEPHYRUM; ordo < FONS_8X16_ALTITUDO; ordo++)
    {
        bits = fons_8x16[glypha][ordo];
        per (columna = ZEPHYRUM; bits && columna < FONS_8X16_LATITUDO;
            columna++)
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
tabula_pixelorum_pingere_chordam_fonte (
    TabulaPixelorum* tabula,
                i32  x,
                i32  y,
             chorda  textus,
                i32  color,
                i32  fons)
{
            i32  initium_x;
    constans i8* ptr;
    constans i8* finis;
            s32  runa;

    si (fons != FONS_TITULI)
    {
        tabula_pixelorum_pingere_chordam(tabula, x, y, textus, color);
        redde;
    }
    initium_x  = x;
    ptr        = textus.datum;
    finis      = textus.datum + textus.mensura;
    dum (ptr < finis)
    {
        runa = utf8_decodere(&ptr, finis);
        si (runa == '\n')
        {
            y += FONS_8X16_ALTITUDO;
            x = initium_x;
            perge;
        }
        si (!glypham_ductam_pingere(tabula, x, y, runa,
                FONS_8X16_LATITUDO, FONS_8X16_ALTITUDO, I, color))
        {
            characterem_8x16_pingere(tabula, x, y,
                fons_8x16_codepoint_ad_glypham(runa), color);
        }
        x += FONS_8X16_LATITUDO;
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
        alioquin si (glypham_ductam_pingere(tabula, x, y, runa,
                     FONS_LATITUDO, FONS_ALTITUDO, scala, color))
        {
            x += FONS_LATITUDO * scala;
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
