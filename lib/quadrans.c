/* quadrans.c - Vide quadrans.h */

#include "quadrans.h"

/* quadrantChars OpenTUI: index = bita (TL 8, TR 4, BL 2, BR 1) */
interior constans s32 FIGURAE[XVI] = {
    0x20,   0x2597, 0x2596, 0x2584, 0x259D, 0x2590, 0x259E, 0x259F,
    0x2598, 0x259A, 0x258C, 0x2599, 0x2580, 0x259C, 0x259B, 0x2588
};

/* luminositas Rec. 709 in decem-millesimis per canalem (R, G, B;
 * integra: exitus idem in omni machina) */
interior constans i32 LUMEN[III] = { 2126, 7152, 722 };

/* Pixelum (rx, ry) regionis contra fundum compositum (opacum,
 * 0x00RRGGBB); extra regionem aut imaginem = fundus */
interior i32
_pixelum (
    constans Imago* imago,
               s32  x,
               s32  y,
               i32  latitudo,
               i32  altitudo,
               s32  rx,
               s32  ry,
               i32  fundus)
{
            s32  px = x + rx;
            s32  py = y + ry;
    constans i8* p;
            i32  a;
            i32  c = ZEPHYRUM;
            i32  k;

    si (   imago == NIHIL || rx >= (s32)latitudo || ry >= (s32)altitudo
        || px < ZEPHYRUM || py < ZEPHYRUM
        || px    >= (s32)imago->latitudo || py >= (s32)imago->altitudo)
    {
        redde fundus;
    }
    p = imago->pixela + ((i32)py * imago->latitudo + (i32)px) * IV;
    a = (i32)p[III];
    per (k = ZEPHYRUM; k < III; k++)
    {
        i32 f = (fundus >> (XVI - VIII * k)) & 0xFF;
        i32 v = ((i32)p[k] * a + f * (0xFF - a) + 0x7F) / 0xFF;

        c |= v << (XVI - VIII * k);
    }
    redde c;
}

/* Canalis k (0 R, 1 G, 2 B) coloris 0x00RRGGBB */
interior i32
_canalis (
    i32 c,
    i32 k)
{
    redde (c >> (XVI - VIII * k)) & 0xFF;
}

interior i32
_distantia (
    i32 a,
    i32 b)
{
    i32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        s32 d = (s32)_canalis(a, k) - (s32)_canalis(b, k);

        summa += (i32)(d * d);
    }
    redde summa;
}

interior i32
_lumen (
    i32 c)
{
    i32 summa = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        summa += LUMEN[k] * _canalis(c, k);
    }
    redde summa;
}

/* Media pixelorum quorum bitum (in 'bita') == 'pars' (rotundatio:
 * dimidium sursum) */
interior i32
_media (
    constans i32* px,
             i32  bita,
             b32  pars)
{
    i32 c = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        i32 summa  = ZEPHYRUM;
        i32 n      = ZEPHYRUM;
        i32 q;

        per (q = ZEPHYRUM; q < IV; q++)
        {
            si ((b32)((bita & (VIII >> q)) != ZEPHYRUM) == pars)
            {
                summa += _canalis(px[q], k);
                n++;
            }
        }
        c |= ((summa + n / II) / n) << (XVI - VIII * k);
    }
    redde c;
}

/* Bloccus 2x2 (TL, TR, BL, BR; opaci) -> cellula (quadrans.h) */
interior QuadransCellula
_bloccus (
       constans i32* px,
    QuadransColores  colores)
{
    QuadransCellula c;
                i32 maxima  = _distantia(px[0], px[1]);
                i32 a       = ZEPHYRUM;
                i32 b       = I;
                i32 obscurus;
                i32 clarus;
                i32 bita = ZEPHYRUM;
                i32 i;
                i32 j;

    si (px[0] == px[1] && px[0] == px[2] && px[0] == px[3])
    {
        c.runa            = 0x20;   /* uniformis: spatium */
        c.color_litterae  = px[0];
        c.color_fundi     = px[0];
        redde c;
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        per (j = i + I; j < IV; j++)
        {
            si (_distantia(px[i], px[j]) > maxima)
            {
                maxima  = _distantia(px[i], px[j]);
                a       = i;
                b       = j;
            }
        }
    }
    si (_lumen(px[a]) <= _lumen(px[b]))
    {
        obscurus  = px[a];
        clarus    = px[b];
    }
    alioquin
    {
        obscurus  = px[b];
        clarus    = px[a];
    }
    per (i = ZEPHYRUM; i < IV; i++)
    {
        si (_distantia(px[i], obscurus) <= _distantia(px[i], clarus))
        {
            bita |= VIII >> i;
        }
    }
    c.runa = FIGURAE[bita];
    si (colores == QUADRANS_MEDIA)
    {
        c.color_litterae  = _media(px, bita, VERUM);
        c.color_fundi     = _media(px, bita, FALSUM);
    }
    alioquin
    {
        c.color_litterae  = obscurus;
        c.color_fundi     = clarus;
    }
    redde c;
}

QuadransOptiones
quadrans_optiones_ordinariae (vacuum)
{
    QuadransOptiones o;

    o.modus    = QUADRANS_QUADRANTES;
    o.colores  = QUADRANS_EXTREMA;
    o.fundus   = ZEPHYRUM;
    redde o;
}

vacuum
quadrans_mensurare (
    constans QuadransOptiones* optiones,
                          i32  latitudo,
                          i32  altitudo,
                          i32* cellulae_lat,
                          i32* cellulae_alt)
{
    (vacuum)optiones;   /* QUADRANTES solus modus (planum D3) */
    *cellulae_lat = (latitudo + I) / II;
    *cellulae_alt = (altitudo + I) / II;
}

vacuum
quadrans_computare (
               constans Imago* imago,
                          s32  x,
                          s32  y,
                          i32  latitudo,
                          i32  altitudo,
    constans QuadransOptiones* optiones,
              QuadransCellula* exitus)
{
    i32 lat;
    i32 alt;
    i32 i;
    i32 j;

    quadrans_mensurare(optiones, latitudo, altitudo, &lat, &alt);
    per (j = ZEPHYRUM; j < alt; j++)
    {
        per (i = ZEPHYRUM; i < lat; i++)
        {
            i32 px[IV];
            s32 rx = (s32)(i * II);
            s32 ry = (s32)(j * II);

            px[0] = _pixelum(imago, x, y, latitudo, altitudo, rx, ry,
                optiones->fundus);
            px[1] = _pixelum(imago, x, y, latitudo, altitudo, rx + I,
                ry,
                optiones->fundus);
            px[2] = _pixelum(imago, x, y, latitudo, altitudo, rx, ry
                + I,
                optiones->fundus);
            px[3] = _pixelum(imago, x, y, latitudo, altitudo, rx + I,
                ry + I, optiones->fundus);
            exitus[j * lat + i] = _bloccus(px, optiones->colores);
        }
    }
}
