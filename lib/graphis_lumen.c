/*
 * graphis_lumen.c - lux picturae, arithmetica integra
 * (graphis_lumen.h)
 *
 * Lux cuiusque pixeli per canalem in partibus GRAPHIS_LUX_UNA
 * computatur (s64: producta crescunt), deinde fons multiplicatur et
 * ad CCLV saturatur. Divisiones omnes inter numeros non negativos
 * fiunt (C89 divisionem negativorum non definit).
 */

#include "graphis_lumen.h"
#include <string.h>

#define UNA             ((s64)GRAPHIS_LUX_UNA)
#define RADIUS_MAXIMUS  (IV * MXXIV)
#define GRADUS_TABULAE  LXIV
#define PARTES_GRADUS   CCLVI

/* GRAPHIS_LUX_UNA x exp(-k / VIII), k = 0 .. LXIV: gradus tabulae
 * octava pars est, ergo exp(-t / II) ad k = IV x t legitur */
hic_manens constans s32 DECRESCENTIA[GRADUS_TABULAE + I] = {
    4096, 3615, 3190, 2815, 2484, 2192, 1935, 1707, 1507, 1330, 1174,
    1036, 914, 807, 712, 628, 554, 489, 432, 381, 336, 297, 262, 231,
    204, 180, 159, 140, 124, 109, 96, 85, 75, 66, 58, 52, 46, 40, 35,
    31, 28, 24, 21, 19, 17, 15, 13, 12, 10, 9, 8, 7, 6, 5, 5, 4, 4, 3,
    3, 3, 2, 2, 2, 2, 1
};

interior b32
_lux_valida (
    constans GraphisLux* lux)
{
    i32 k;

    si (   lux->lux > GRAPHIS_LUX_UNA || lux->umbra > GRAPHIS_LUX_UNA
        || lux->luminum > GRAPHIS_LUMINA_MAXIMA
        || (lux->luminum > ZEPHYRUM && !lux->lumina))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < lux->luminum; k++)
    {
        constans GraphisLumen* l = &lux->lumina[k];

        si (l->vis > II * GRAPHIS_LUX_UNA)
        {
            redde FALSUM;
        }
        si (l->punctum)
        {
            si (   l->radius_x < I || l->radius_y < I
                || l->radius_x > (s32)RADIUS_MAXIMUS
                || l->radius_y > (s32)RADIUS_MAXIMUS)
            {
                redde FALSUM;
            }
        }
        alioquin si (   !l->area_x || !l->area_y || l->punctorum < III
                     || l->punctorum > GRAPHIS_AREA_PUNCTA
                     || l->mollities > GRAPHIS_MOLLITIES_MAXIMA)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* centrum pixeli intra polygonum areae? (paritas sectionum) */
interior b32
_in_area (
    constans GraphisLumen* l,
                      s32  x,
                      s32  y)
{
    s32 medium  = II * y + I;
    b32 intus   = FALSUM;
    i32 k;

    per (k = ZEPHYRUM; k < l->punctorum; k++)
    {
        i32 sequens  = (k + I) % l->punctorum;
        s32 y_a      = l->area_y[k];
        s32 y_b      = l->area_y[sequens];
        s64 numerator;
        s64 denominator;

        si ((II * y_a < medium) == (II * y_b < medium))
        {
            perge;
        }
        denominator = (s64)(y_b - y_a);
        numerator   = (s64)II * l->area_x[k] * denominator
            + (s64)(medium - II * y_a)
                * (l->area_x[sequens] - l->area_x[k]);
        si (denominator < ZEPHYRUM)
        {
            denominator  = -denominator;
            numerator    = -numerator;
        }
        si ((s64)(II * x + I) * denominator < numerator)
        {
            intus = !intus;
        }
    }
    redde intus;
}

/* pars luminis in pixelo, 0 .. vis */
interior s64
_pars_luminis (
    constans GraphisLumen* l,
                      s32  x,
                      s32  y)
{
    si (l->punctum)
    {
        s64 motus_x = (s64)x - l->x;
        s64 motus_y = (s64)y - l->y;
        s64 gradus  = IV * PARTES_GRADUS * motus_x * motus_x
            / ((s64)l->radius_x * l->radius_x)
            + IV * PARTES_GRADUS * motus_y * motus_y
            / ((s64)l->radius_y * l->radius_y);
        s64 index     = gradus / PARTES_GRADUS;
        s64 reliquum  = gradus % PARTES_GRADUS;
        s64 valor;

        si (index >= GRADUS_TABULAE)
        {
            redde ZEPHYRUM;
        }
        valor = DECRESCENTIA[index]
            - (DECRESCENTIA[index] - DECRESCENTIA[index + I])
                * reliquum / PARTES_GRADUS;
        redde (s64)l->vis * valor / UNA;
    }
    {
        s32 mollities  = (s32)l->mollities;
        s64 tectum     = _in_area(l, x, y) ? II : ZEPHYRUM;

        si (mollities == ZEPHYRUM)
        {
            redde tectum ? (s64)l->vis : ZEPHYRUM;
        }
        tectum += _in_area(l, x - mollities, y) ? I : ZEPHYRUM;
        tectum += _in_area(l, x + mollities, y) ? I : ZEPHYRUM;
        tectum += _in_area(l, x, y - mollities) ? I : ZEPHYRUM;
        tectum += _in_area(l, x, y + mollities) ? I : ZEPHYRUM;
        redde (s64)l->vis * tectum / VI;
    }
}

i8*
graphis_illustrare (
            constans i8* fons,
            constans i8* lucentes,
                    i32  latitudo,
                    i32  altitudo,
    constans GraphisLux* lux,
                Piscina* piscina)
{
     i8* exitus;
    s64  lat;
    s64  alt;
    s32  x;
    s32  y;
    i32  k;
    i32  j;

    si (   !fons || !lux || !piscina || latitudo < I || altitudo < I
        || !_lux_valida(lux))
    {
        redde NIHIL;
    }
    exitus = (i8*)piscina_allocare(piscina,
        (memoriae_index)(latitudo * altitudo * IV));
    si (!exitus)
    {
        redde NIHIL;
    }
    lat = (s64)latitudo;
    alt = (s64)altitudo;
    per (y = ZEPHYRUM; y < (s32)altitudo; y++)
    {
        per (x = ZEPHYRUM; x < (s32)latitudo; x++)
        {
                    i32  locus    = (i32)y * latitudo + (i32)x;
            constans i8* pixelum  = fons + locus * IV;
                     i8* scopus   = exitus + locus * IV;
                    s64  a        = (s64)II * x + I - lat;
                    s64  b        = (s64)II * y + I - alt;
                    s64  distantia;
                    s64  ambiens;
                    s64  summa[III];

            scopus[III] = 255;
            si (lucentes && lucentes[locus])
            {
                memcpy(scopus, pixelum, III);
                perge;
            }
            /* quadratum distantiae a centro: 0 .. UNA in angulis */
            distantia = (a * a * UNA / (lat * lat)
                + b * b * UNA / (alt * alt)) / II;
            ambiens   = (s64)lux->lux
                * (UNA - (s64)lux->umbra * distantia / UNA) / UNA;
            per (k = ZEPHYRUM; k < III; k++)
            {
                summa[k] = ambiens * lux->tinctura[k] / 255;
            }
            per (j = ZEPHYRUM; j < lux->luminum; j++)
            {
                constans GraphisLumen* l     = &lux->lumina[j];
                                  s64  pars  = _pars_luminis(l, x, y);

                si (pars == ZEPHYRUM)
                {
                    perge;
                }
                per (k = ZEPHYRUM; k < III; k++)
                {
                    summa[k] += pars * l->color[k] / 255;
                }
            }
            per (k = ZEPHYRUM; k < III; k++)
            {
                s64 valor = ((s64)pixelum[k] * summa[k] + UNA / II)
                    / UNA;

                scopus[k] = (i8)(valor > 255 ? 255 : valor);
            }
        }
    }
    redde exitus;
}
