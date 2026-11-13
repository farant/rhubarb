/*
 * mundus_mappa.c - mappa tabulae gressus (mundus_mappa.h)
 *
 * Ordo picturae: cellae, reticulum, numeri axium, notae. Litterae
 * ex fonte VI x VIII (fons_6x8.h) in pixela directe.
 */

#include "mundus_mappa.h"
#include "fons_6x8.h"
#include <string.h>

#define PIXELA_CELLAE     MUNDUS_MAPPA_PIXELA_PER_CELLAM
#define MARGO   MUNDUS_MAPPA_MARGO
#define CELLAE  MUNDUS_CELLAE_PER_UNITATEM
/* reticulum et numeri (unitates) */
#define RETICULI_GRADUS   IV
#define RETICULI_MAGNUS   XVI
#define NUMERORUM_GRADUS  VIII

interior i8
_octetus (
    f32 x)
{
    si (x < 0.0f)
    {
        redde ZEPHYRUM;
    }
    redde x > 255.0f ? (i8)CCLV : (i8)x;
}

interior i8*
_pixelum (
    constans Imago* imago,
               s32  x,
               s32  y)
{
    si (   x < ZEPHYRUM || y < ZEPHYRUM || x >= (s32)imago->latitudo
        || y >= (s32)imago->altitudo)
    {
        redde NIHIL;
    }
    redde imago->pixela
        + ((memoriae_index)y * imago->latitudo + (memoriae_index)x)
        * IV;
}

interior vacuum
_ponere (
    constans Imago* imago,
               s32  x,
               s32  y,
                i8  r,
                i8  g,
                i8  b)
{
    i8* p = _pixelum(imago, x, y);

    si (p)
    {
        p[ZEPHYRUM]  = r;
        p[I]         = g;
        p[II]        = b;
        p[III]       = CCLV;
    }
}

/* pixelum obscurare: 'pars' ex CCLVI manet */
interior vacuum
_obscurare (
    constans Imago* imago,
               s32  x,
               s32  y,
               i32  pars)
{
     i8* p = _pixelum(imago, x, y);
    i32  k;

    per (k = ZEPHYRUM; p && k < III; k++)
    {
        p[k] = (i8)(p[k] * pars / CCLVI);
    }
}

interior vacuum
_rectangulum (
    constans Imago* imago,
               s32  x,
               s32  y,
               s32  latitudo,
               s32  altitudo,
                i8  r,
                i8  g,
                i8  b)
{
    s32 dx;
    s32 dy;

    per (dy = ZEPHYRUM; dy < altitudo; dy++)
    {
        per (dx = ZEPHYRUM; dx < latitudo; dx++)
        {
            _ponere(imago, x + dx, y + dy, r, g, b);
        }
    }
}

/* textus niger ex fonte VI x VIII; angulus summus sinister (x, y) */
interior vacuum
_textus (
        constans Imago* imago,
                   s32  x,
                   s32  y,
    constans character* textus)
{
    s32 ordo;
    s32 columna;

    per (; *textus; textus++, x += FONS_LATITUDO)
    {
        constans i8* glypha =
            fons_6x8[(insignatus character)*textus];

        per (ordo = ZEPHYRUM; ordo < FONS_ALTITUDO; ordo++)
        {
            per (columna = ZEPHYRUM; columna < FONS_LATITUDO;
                columna++)
            {
                si (glypha[ordo] & (0x80 >> columna))
                {
                    _ponere(imago, x + columna, y + ordo, ZEPHYRUM,
                        ZEPHYRUM, ZEPHYRUM);
                }
            }
        }
    }
}

/* numerus integer (negativus quoque) in litteras; longitudo eius */
interior s32
_numerum_scribere (
    character* alveus,
          s32  numerus)
{
    character inversus[XVI];
          s32 k          = ZEPHYRUM;
          s32 longitudo  = ZEPHYRUM;
          b32 negativus  = numerus < ZEPHYRUM;

    si (negativus)
    {
        numerus = -numerus;
    }
    fac
    {
        inversus[k++]  = (character)('0' + numerus % X);
        numerus        /= X;
    } dum (numerus > ZEPHYRUM && k < XII);
    si (negativus)
    {
        alveus[longitudo++] = '-';
    }
    dum (k > ZEPHYRUM)
    {
        alveus[longitudo++] = inversus[--k];
    }
    alveus[longitudo] = '\0';
    redde longitudo;
}

interior vacuum
_cellas_pingere (
    constans MundusSolum* s,
             constans i8* attingibiles,
          constans Imago* imago)
{
    i32 cella_x;
    i32 cella_z;

    per (cella_z = ZEPHYRUM; cella_z < s->altitudo; cella_z++)
    {
        per (cella_x = ZEPHYRUM; cella_x < s->latitudo; cella_x++)
        {
            memoriae_index locus = (memoriae_index)cella_z
                * s->latitudo + cella_x;
                       f32 altitudo  = s->altitudines[locus];
                        i8 r         = CCLV;
                        i8 g         = CCLV;
                        i8 b         = CCLV;

            commutatio (s->genera[locus])
            {
                casus MUNDUS_SOLUM_AQUA:
                    r = 60;
                    g = 110;
                    b = 220;
                    frange;
                casus MUNDUS_SOLUM_SOLIDUM:
                    r = 50;
                    g = 50;
                    b = 50;
                    frange;
                casus MUNDUS_SOLUM_GRESSIBILE:
                    si (attingibiles && !attingibiles[locus])
                    {
                        r = 230;
                        g = _octetus(90.0f + altitudo * 12.0f);
                        b = 150;
                    }
                    alioquin
                    {
                        r = _octetus(40.0f + altitudo * 22.0f);
                        g = _octetus(120.0f + altitudo * 12.0f);
                        b = 40;
                    }
                    frange;
                ordinarius:
                    frange;
            }
            _rectangulum(imago, MARGO + (s32)cella_x * PIXELA_CELLAE,
                MARGO + (s32)cella_z * PIXELA_CELLAE, PIXELA_CELLAE,
                PIXELA_CELLAE, r, g, b);
        }
    }
}

/* lineae reticuli et numeri axium. Coordinata mundi lineae k =
 * origo + k unitates (origo tabulae unitas integra est) */
interior vacuum
_reticulum_pingere (
    constans MundusSolum* s,
          constans Imago* imago)
{
    character numerus[XVI];
          s32 unitates_x  = (s32)s->latitudo / CELLAE;
          s32 unitates_z  = (s32)s->altitudo / CELLAE;
          s32 passus      = PIXELA_CELLAE * CELLAE;
          s32 k;
          s32 j;
          s32 longitudo;

    per (k = ZEPHYRUM; k <= unitates_x; k++)
    {
        s32 mundi  = (s32)s->origo_x + k;
        s32 x      = MARGO + k * passus;

        si (mundi % RETICULI_GRADUS)
        {
            perge;
        }
        per (j = MARGO; j < (s32)imago->altitudo; j++)
        {
            _obscurare(imago, x, j,
                mundi % RETICULI_MAGNUS ? CXCII : CXXVIII);
        }
        si (mundi % NUMERORUM_GRADUS == ZEPHYRUM)
        {
            longitudo = _numerum_scribere(numerus, mundi);
            /* numerus ad marginem dextrum intra imaginem manet */
            j = x - longitudo * FONS_LATITUDO / II;
            si (j + longitudo * FONS_LATITUDO > (s32)imago->latitudo)
            {
                j = (s32)imago->latitudo - longitudo * FONS_LATITUDO;
            }
            _textus(imago, j, IV, numerus);
        }
    }
    per (k = ZEPHYRUM; k <= unitates_z; k++)
    {
        s32 mundi  = (s32)s->origo_z + k;
        s32 y      = MARGO + k * passus;

        si (mundi % RETICULI_GRADUS)
        {
            perge;
        }
        per (j = MARGO; j < (s32)imago->latitudo; j++)
        {
            _obscurare(imago, j, y,
                mundi % RETICULI_MAGNUS ? CXCII : CXXVIII);
        }
        si (mundi % NUMERORUM_GRADUS == ZEPHYRUM)
        {
            longitudo = _numerum_scribere(numerus, mundi);
            _textus(imago, MARGO - I - longitudo * FONS_LATITUDO
                > ZEPHYRUM
                ? MARGO - I - longitudo * FONS_LATITUDO : ZEPHYRUM,
                y - FONS_ALTITUDO / II + FONS_ALTITUDO
                    > (s32)imago->altitudo
                    ? (s32)imago->altitudo - FONS_ALTITUDO
                    : y - FONS_ALTITUDO / II, numerus);
        }
    }
}

interior vacuum
_notas_pingere (
        constans MundusSolum* s,
    constans MundusMappaNota* notae,
                         i32  notarum,
              constans Imago* imago)
{
    character numerus[XVI];
          i32 n;
          s32 longitudo;

    per (n = ZEPHYRUM; n < notarum; n++)
    {
        f32 cellae_x = (notae[n].x - s->origo_x) * (f32)CELLAE;
        f32 cellae_z = (notae[n].z - s->origo_z) * (f32)CELLAE;
        s32 x;
        s32 y;

        si (   !(cellae_x >= 0.0f) || !(cellae_z >= 0.0f)
            || cellae_x >= (f32)s->latitudo
            || cellae_z >= (f32)s->altitudo)
        {
            perge;
        }
        x = MARGO + (s32)(cellae_x * (f32)PIXELA_CELLAE);
        y = MARGO + (s32)(cellae_z * (f32)PIXELA_CELLAE);
        /* punctum: V x V nigrum, medium album */
        _rectangulum(imago, x - II, y - II, V, V, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM);
        _ponere(imago, x, y, CCLV, CCLV, CCLV);
        longitudo = _numerum_scribere(numerus, (s32)notae[n].numerus);
        _rectangulum(imago, x + IV, y - V,
            longitudo * FONS_LATITUDO + II, FONS_ALTITUDO + II, CCLV,
            CCLV, CCLV);
        _textus(imago, x + V, y - IV, numerus);
    }
}

b32
mundus_mappa_pingere (
        constans MundusSolum* s,
                 constans i8* attingibiles,
    constans MundusMappaNota* notae,
                         i32  notarum,
                       Imago* exitus,
                     Piscina* piscina)
{
             Imago imago;
    memoriae_index octeti;

    si (   !s || !exitus || !piscina || !s->genera || !s->altitudines
        || s->latitudo == ZEPHYRUM || s->altitudo == ZEPHYRUM
        || (notarum && !notae))
    {
        redde FALSUM;
    }
    imago.latitudo = MARGO + s->latitudo * PIXELA_CELLAE;
    imago.altitudo = MARGO + s->altitudo * PIXELA_CELLAE;
    octeti          = (memoriae_index)imago.latitudo * imago.altitudo
        * IV;
    imago.pixela    = (i8*)piscina_allocare(piscina, octeti);
    si (!imago.pixela)
    {
        redde FALSUM;
    }
    memset(imago.pixela, 0xFF, octeti);
    _cellas_pingere(s, attingibiles, &imago);
    _reticulum_pingere(s, &imago);
    _notas_pingere(s, notae, notarum, &imago);
    *exitus = imago;
    redde VERUM;
}
