/*
 * graphis_specimen.c - scheda ad inspiciendum (graphis_specimen.h)
 */

#include "graphis_specimen.h"
#include "dithering.h"
#include <string.h>

#define LAT              GRAPHIS_LATITUDO
#define ALT              GRAPHIS_ALTITUDO
#define STRIAE_COLOR     XC
#define PALLOR_DIVISOR   IV
#define PALLOR_ADDITUS   CXC

nomen structura {
    constans character* titulus;
                    i8  rgb[III];
} ColorRegionis;

hic_manens constans ColorRegionis COLORES[GRAPHIS_SPECIMEN_COLORES] =
{
    { "red",    { 230, 60, 60 } },
    { "blue",   { 60, 160, 230 } },
    { "yellow", { 240, 200, 40 } },
    { "green",  { 80, 200, 90 } },
    { "purple", { 200, 90, 220 } },
    { "orange", { 250, 140, 40 } },
    { "cyan",   { 60, 210, 200 } },
    { "brown",  { 150, 110, 60 } },
    { "indigo", { 120, 120, 240 } },
    { "pink",   { 240, 120, 170 } },
    { "olive",  { 130, 180, 60 } },
    { "grey",   { 90, 90, 90 } }
};

/* lucens in tabula larvae */
hic_manens constans i8 COLOR_LUCENTIS[III] = { 64, 128, 255 };

interior Imago
_imago_vacua (vacuum)
{
    Imago imago;

    memset(&imago, ZEPHYRUM, magnitudo(imago));
    redde imago;
}

interior b32
_fructus_bonus (
    constans GraphisFructus* fructus)
{
    redde fructus && fructus->successus && fructus->tabula
        && fructus->indices && fructus->illustratus;
}

interior vacuum
_pone (
    constans Imago* imago,
               i32  x,
               i32  y,
       constans i8* rgb)
{
    i8* p = imago->pixela + (y * imago->latitudo + x) * IV;

    memcpy(p, rgb, III);
    p[III] = 255;
}

constans i8*
graphis_specimen_color (
    i32 dominus)
{
    redde dominus < I ? NIHIL
        : COLORES[(dominus - I) % GRAPHIS_SPECIMEN_COLORES].rgb;
}

constans character*
graphis_specimen_color_titulus (
    i32 dominus)
{
    redde dominus < I ? NIHIL
        : COLORES[(dominus - I) % GRAPHIS_SPECIMEN_COLORES].titulus;
}

Imago
graphis_specimen_pingere (
    constans GraphisFructus* fructus,
                    Piscina* piscina)
{
    Imago imago   = _imago_vacua();
      i32 dexter  = II * LAT + GRAPHIS_SPECIMEN_STRIA;
      i32 x;
      i32 y;
       i8 color[III];

    si (!_fructus_bonus(fructus) || !piscina)
    {
        redde imago;
    }
    imago.latitudo = GRAPHIS_SPECIMEN_LATITUDO;
    imago.altitudo = GRAPHIS_SPECIMEN_ALTITUDO;
    imago.pixela   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(imago.latitudo * imago.altitudo * IV));
    si (!imago.pixela)
    {
        redde _imago_vacua();
    }
    memset(color, STRIAE_COLOR, III);
    per (y = ZEPHYRUM; y < imago.altitudo; y++)
    {
        per (x = II * LAT; x < dexter; x++)
        {
            _pone(&imago, x, y, color);
        }
    }
    per (y = ZEPHYRUM; y < ALT; y++)
    {
        per (x = ZEPHYRUM; x < LAT; x++)
        {
                    i32  locus   = y * LAT + x;
            constans i8* palette =
                AQUINAS_PALETTE[fructus->indices[locus]];
                    i32 dominus = fructus->tabula->domini[locus];

            /* pictura finita, bis aucta */
            _pone(&imago, II * x, II * y, palette);
            _pone(&imago, II * x + I, II * y, palette);
            _pone(&imago, II * x, II * y + I, palette);
            _pone(&imago, II * x + I, II * y + I, palette);
            /* fons */
            _pone(&imago, dexter + x, y,
                fructus->tabula->fons + locus * IV);
            /* larva; lucentia caerulea */
            si (fructus->tabula->lucentes[locus])
            {
                _pone(&imago, dexter + LAT + x, y, COLOR_LUCENTIS);
            }
            alioquin
            {
                memset(color, fructus->tabula->larva[locus], III);
                _pone(&imago, dexter + LAT + x, y, color);
            }
            /* illustratus */
            _pone(&imago, dexter + x, ALT + y,
                fructus->illustratus + locus * IV);
            /* regiones super picturam pallidam */
            si (dominus)
            {
                _pone(&imago, dexter + LAT + x, ALT + y,
                    graphis_specimen_color(dominus));
            }
            alioquin
            {
                color[ZEPHYRUM]  = (i8)(palette[ZEPHYRUM]
                    / PALLOR_DIVISOR + PALLOR_ADDITUS);
                color[I]         = (i8)(palette[I] / PALLOR_DIVISOR
                    + PALLOR_ADDITUS);
                color[II]        = (i8)(palette[II] / PALLOR_DIVISOR
                    + PALLOR_ADDITUS);
                _pone(&imago, dexter + LAT + x, ALT + y, color);
            }
        }
    }
    redde imago;
}

Imago
graphis_quadrum_pingere (
    constans GraphisFructus* fructus,
                        s32  x,
                        s32  y,
                        s32  latitudo,
                        s32  altitudo,
                    Piscina* piscina)
{
    Imago imago = _imago_vacua();
      i32 columna;
      i32 ordo;
      i32 u;
      i32 v;

    si (   !_fructus_bonus(fructus) || !piscina || x < ZEPHYRUM
        || y < ZEPHYRUM || latitudo < I || altitudo < I
        || latitudo > (s32)LAT || altitudo > (s32)ALT
        || x + latitudo > (s32)LAT || y + altitudo > (s32)ALT)
    {
        redde imago;
    }
    imago.latitudo = (i32)latitudo * GRAPHIS_QUADRUM_AUCTIO;
    imago.altitudo = (i32)altitudo * GRAPHIS_QUADRUM_AUCTIO;
    imago.pixela   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(imago.latitudo * imago.altitudo * IV));
    si (!imago.pixela)
    {
        redde _imago_vacua();
    }
    per (ordo = ZEPHYRUM; ordo < (i32)altitudo; ordo++)
    {
        per (columna = ZEPHYRUM; columna < (i32)latitudo; columna++)
        {
            constans i8* palette = AQUINAS_PALETTE[fructus->indices[
                ((i32)y + ordo) * LAT + (i32)x + columna]];

            per (v = ZEPHYRUM; v < GRAPHIS_QUADRUM_AUCTIO; v++)
            {
                per (u = ZEPHYRUM; u < GRAPHIS_QUADRUM_AUCTIO; u++)
                {
                    _pone(&imago,
                        columna * GRAPHIS_QUADRUM_AUCTIO + u,
                        ordo * GRAPHIS_QUADRUM_AUCTIO + v, palette);
                }
            }
        }
    }
    redde imago;
}

i32
graphis_regiones (
    constans GraphisFructus* fructus,
               GraphisRegio* exitus)
{
    i32 numerus;
    i32 k;
    i32 x;
    i32 y;

    si (!_fructus_bonus(fructus) || !exitus)
    {
        redde ZEPHYRUM;
    }
    numerus = fructus->tabula->titulorum;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        exitus[k].titulus    = fructus->tabula->tituli[k];
        exitus[k].pixela     = ZEPHYRUM;
        exitus[k].x_primus   = (s32)LAT;
        exitus[k].y_primus   = (s32)ALT;
        exitus[k].x_ultimus  = -I;
        exitus[k].y_ultimus  = -I;
    }
    per (y = ZEPHYRUM; y < ALT; y++)
    {
        per (x = ZEPHYRUM; x < LAT; x++)
        {
                     i32 dominus =
                         fructus->tabula->domini[y * LAT + x];
            GraphisRegio* r;

            si (dominus < I || dominus > numerus)
            {
                perge;
            }
            r = &exitus[dominus - I];
            r->pixela++;
            si ((s32)x < r->x_primus)
            {
                r->x_primus = (s32)x;
            }
            si ((s32)x > r->x_ultimus)
            {
                r->x_ultimus = (s32)x;
            }
            si ((s32)y < r->y_primus)
            {
                r->y_primus = (s32)y;
            }
            si ((s32)y > r->y_ultimus)
            {
                r->y_ultimus = (s32)y;
            }
        }
    }
    redde numerus;
}
