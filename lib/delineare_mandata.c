/* delineare_mandata.c - rasterizator mandatorum */

#include "delineare_mandata.h"
#include "delineare.h"
#include "thema.h"
#include "xar.h"

#include <string.h>

#define SCAENAE_MAXIMAE LXIV


/* ==================================================
 * Tabula nuda et imago
 * ================================================== */

TabulaPixelorum*
tabula_pixelorum_creare_nuda (
     Piscina* piscina,
         i32  latitudo,
         i32  altitudo)
{
    TabulaPixelorum* t;

    si (!piscina || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    t = (TabulaPixelorum*)piscina_allocare(piscina,
                                           magnitudo(TabulaPixelorum));
    si (!t)
    {
        redde NIHIL;
    }
    t->latitudo           = latitudo;
    t->altitudo           = altitudo;
    t->fenestra_latitudo  = latitudo;
    t->fenestra_altitudo  = altitudo;
    t->scala              = 1.0f;
    t->capacitas          = latitudo * altitudo;
    t->pixela = (i32*)piscina_allocare(piscina,
        (memoriae_index)latitudo * (memoriae_index)altitudo
            * magnitudo(i32));
    si (!t->pixela)
    {
        redde NIHIL;
    }
    memset(t->pixela, ZEPHYRUM,
           (memoriae_index)latitudo * (memoriae_index)altitudo
               * magnitudo(i32));
    redde t;
}

Imago
imago_ex_tabula (
    constans TabulaPixelorum* tabula)
{
    Imago imago;

    imago.pixela    = (i8*)tabula->pixela;
    imago.latitudo  = tabula->latitudo;
    imago.altitudo  = tabula->altitudo;
    redde imago;
}

Color
color_ex_mandato (
    ColorMandati color)
{
    commutatio (color.genus)
    {
        casus COLOR_MANDATI_THEMA:
            redde thema_color((ColorThema)color.valor);
        casus COLOR_MANDATI_INDEX:
            redde thema_color_ex_indice_colorationis((i8)color.valor);
        ordinarius:
            redde color_ex_pixelum(color.valor);
    }
}


/* ==================================================
 * Scaenae
 * ================================================== */

nomen structura {
    s32 origo_x;
    s32 origo_y;
    s32 scala;
    b32 praecisa;
    s32 sectio_x;
    s32 sectio_y;
    s32 sectio_latitudo;
    s32 sectio_altitudo;
    s32 reliqua;          /* elementa reliqua sub hoc coetu */
} Scaena;

interior vacuum
sectionem_applicare (
    ContextusDelineandi* ctx,
        constans Scaena* s)
{
    si (s->praecisa)
    {
        delineare_ponere_praecisionem(ctx, s->sectio_x, s->sectio_y,
            (i32)(s->sectio_latitudo < ZEPHYRUM ? ZEPHYRUM
                                                 : s->sectio_latitudo),
            (i32)(s->sectio_altitudo < ZEPHYRUM ? ZEPHYRUM
                                                 : s->sectio_altitudo));
    }
    alioquin
    {
        delineare_tollere_praecisionem(ctx);
    }
}

/* scaena nova ex coetu: origo, scala, sectio ∩ */
interior Scaena
scaenam_impellere (
      constans Scaena* parens,
    constans Mandatum* coetus)
{
    Scaena s;
       s32 x0;
       s32 y0;
       s32 x1;
       s32 y1;

    s = *parens;
    s.origo_x = parens->origo_x + coetus->fines.x * parens->scala
              + coetus->translatio.x * parens->scala;
    s.origo_y = parens->origo_y + coetus->fines.y * parens->scala
              + coetus->translatio.y * parens->scala;
    s.scala   = parens->scala * (coetus->scala > ZEPHYRUM
                                 ? (s32)coetus->scala : I);
    s.reliqua = (s32)coetus->magnitudo_arboris - I;
    si (coetus->sectio)
    {
        x0 = parens->origo_x + coetus->fines.x * parens->scala;
        y0 = parens->origo_y + coetus->fines.y * parens->scala;
        x1 = x0 + coetus->fines.latitudo * parens->scala;
        y1 = y0 + coetus->fines.altitudo * parens->scala;
        si (s.praecisa)
        {
            si (x0 < s.sectio_x)
            { x0 = s.sectio_x;
            }
            si (y0 < s.sectio_y)
            { y0 = s.sectio_y;
            }
            si (x1 > s.sectio_x + s.sectio_latitudo)
            { x1 = s.sectio_x + s.sectio_latitudo;
            }
            si (y1 > s.sectio_y + s.sectio_altitudo)
            { y1 = s.sectio_y + s.sectio_altitudo;
            }
        }
        s.praecisa         = VERUM;
        s.sectio_x         = x0;
        s.sectio_y         = y0;
        s.sectio_latitudo  = x1 - x0;
        s.sectio_altitudo  = y1 - y0;
    }
    redde s;
}


/* ==================================================
 * Primitiva
 * ================================================== */

interior vacuum
imaginem_pingere (
     TabulaPixelorum* t,
     constans Scaena* s,
   constans Mandatum* x,
      constans Imago* imago)
{
            s32  sx;
            s32  sy;
            s32  dx;
            s32  dy;
            s32  px;
            s32  py;
            s32  lat;
            s32  alt;
            s32  k;
    constans i8* p;

    lat = (s32)imago->latitudo < x->fines.latitudo
        ? (s32)imago->latitudo : x->fines.latitudo;
    alt = (s32)imago->altitudo < x->fines.altitudo
        ? (s32)imago->altitudo : x->fines.altitudo;
    per (sy = ZEPHYRUM; sy < alt; sy++)
    {
        per (sx = ZEPHYRUM; sx < lat; sx++)
        {
            p = imago->pixela + (sy * (s32)imago->latitudo + sx) * IV;
            si (p[III] == ZEPHYRUM)
            {
                perge;   /* transparens */
            }
            per (dy = ZEPHYRUM; dy < s->scala; dy++)
            {
                per (dx = ZEPHYRUM; dx < s->scala; dx++)
                {
                    px = s->origo_x + (x->fines.x + sx) * s->scala + dx;
                    py = s->origo_y + (x->fines.y + sy) * s->scala + dy;
                    si (   px < ZEPHYRUM || py < ZEPHYRUM
                        || px >= (s32)t->latitudo
                        || py >= (s32)t->altitudo)
                    {
                        perge;
                    }
                    si (   s->praecisa
                        && (px < s->sectio_x || py < s->sectio_y
                            || px >= s->sectio_x + s->sectio_latitudo
                            || py >= s->sectio_y + s->sectio_altitudo))
                    {
                        perge;
                    }
                    k = py * (s32)t->latitudo + px;
                    t->pixela[k] = color_ad_pixelum(
                        color_ex_rgba(p[ZEPHYRUM], p[I], p[II],
                        p[III]));
                }
            }
        }
    }
}

/* ORIGO NEGATIVA (013, 2026-10-05): API delineare coordinatas i32
 * (INSIGNATAS) accipit; coordinata negativa ut numerus ingens legitur -
 * rectangulum totum evanescebat (x + latitudo involvitur), linea et
 * polygonum directionem comparatione insignata errabant et IN
 * AETERNUM ibant. Primitiva tota in spatio positivo vias veteres
 * sequuntur (octeti eadem); cetera hic in spatio SIGNATO praeciduntur
 * ante delineare. Textus iam rectus est (pixela per additionem cum
 * involutione et finium probationem).
 *
 * Rectangulum plenum praecisum ad tabulam (s32). */
interior vacuum
_plenum_secare (
    ContextusDelineandi* ctx,
        TabulaPixelorum* t,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo,
                  Color  color)
{
    s32 x0 = x;
    s32 y0 = y;
    s32 x1 = x + latitudo;
    s32 y1 = y + altitudo;

    si (x0 < ZEPHYRUM)
    {
        x0 = ZEPHYRUM;
    }
    si (y0 < ZEPHYRUM)
    {
        y0 = ZEPHYRUM;
    }
    si (x1 > (s32)t->latitudo)
    {
        x1 = (s32)t->latitudo;
    }
    si (y1 > (s32)t->altitudo)
    {
        y1 = (s32)t->altitudo;
    }
    si (x1 <= x0 || y1 <= y0)
    {
        redde;
    }
    delineare_rectangulum_plenum(ctx, (i32)x0, (i32)y0, (i32)(x1 - x0),
                                 (i32)(y1 - y0), color);
}

/* Rectangulum exemplari impletum (vicus-latera, indicium foci):
 * praecisum ut _plenum_secare; bitus positi solum colore, exemplar ad
 * coordinatas TABULAE alligatum. Id invalidum: nihil. */
interior vacuum
_exemplar_secare (
    ContextusDelineandi* ctx,
        TabulaPixelorum* t,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo,
                  Color  color,
                    i32  exemplar)
{
    s32 x0 = x;
    s32 y0 = y;
    s32 x1 = x + latitudo;
    s32 y1 = y + altitudo;
    s32 a;
    s32 b;

    si (x0 < ZEPHYRUM)
    {
        x0 = ZEPHYRUM;
    }
    si (y0 < ZEPHYRUM)
    {
        y0 = ZEPHYRUM;
    }
    si (x1 > (s32)t->latitudo)
    {
        x1 = (s32)t->latitudo;
    }
    si (y1 > (s32)t->altitudo)
    {
        y1 = (s32)t->altitudo;
    }
    per (b = y0; b < y1; b++)
    {
        per (a = x0; a < x1; a++)
        {
            si (exemplar_punctum(exemplar, a, b))
            {
                delineare_pixelum(ctx, (i32)a, (i32)b, color);
            }
        }
    }
}

/* Linea in spatio signato: Bresenham IDEM ac delineare_lineam,
 * pixela solum in quadrante positivo (fines superiores et praecisio
 * per delineare_pixelum) */
interior vacuum
_lineam_signatam (
    ContextusDelineandi* ctx,
                    s32  x0,
                    s32  y0,
                    s32  x1,
                    s32  y1,
                  Color  color)
{
    s32 dx     = (x1 > x0) ? x1 - x0 : x0 - x1;
    s32 dy     = (y1 > y0) ? y1 - y0 : y0 - y1;
    s32 sx     = (x0 < x1) ? I : -I;
    s32 sy     = (y0 < y1) ? I : -I;
    s32 error  = dx - dy;
    s32 e2;

    dum (VERUM)
    {
        si (x0 >= ZEPHYRUM && y0 >= ZEPHYRUM)
        {
            delineare_pixelum(ctx, (i32)x0, (i32)y0, color);
        }
        si (x0 == x1 && y0 == y1)
        {
            frange;
        }
        e2 = II * error;
        si (e2 > -dy)
        {
            error  -= dy;
            x0     += sx;
        }
        si (e2 < dx)
        {
            error  += dx;
            y0     += sy;
        }
    }
}

/* Polygonum (n puncta s32, x y alternata) ad semiplanum axis >= 0
 * praecidere (Sutherland-Hodgman); exitus capacitatis n + I. Redde
 * numerum punctorum exitus. */
interior i32
_polygonum_secare (
    constans s32* in,
             i32  n,
             i32  axis,
             s32* ex)
{
    i32 i;
    i32 m = ZEPHYRUM;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans s32* a = in + i * II;
        constans s32* b = in + ((i + I) % n) * II;
                 b32  a_intus  = (a[axis]
                     >= ZEPHYRUM) ? VERUM : FALSUM;
                 b32 b_intus  = (b[axis]
                     >= ZEPHYRUM) ? VERUM : FALSUM;

        si (a_intus)
        {
            ex[m * II]      = a[0];
            ex[m * II + I]  = a[1];
            m++;
        }
        si (a_intus != b_intus)
        {
            /* sectio cum axe = 0 */
            i32 alter  = I - axis;
            s32 d      = b[axis] - a[axis];

            ex[m * II + axis]   = ZEPHYRUM;
            ex[m * II + alter]  = a[alter]
                + (b[alter] - a[alter]) * (ZEPHYRUM - a[axis]) / d;
            m++;
        }
    }
    redde m;
}

interior vacuum
primitivum_pingere (
    ContextusDelineandi* ctx,
        TabulaPixelorum* t,
        constans Scaena* s,
      constans Mandatum* x,
              ImagoFons  fons,
                 vacuum* fons_ctx)
{
             Color  color;
               s32  sx;
               s32  sy;
               s32  lat;
               s32  alt;
               i32  i;
               i32* puncta;
    constans Imago* imago;

    color  = color_ex_mandato(x->color);
    sx     = s->origo_x + x->fines.x * s->scala;
    sy     = s->origo_y + x->fines.y * s->scala;
    lat    = x->fines.latitudo * s->scala;
    alt    = x->fines.altitudo * s->scala;
    commutatio (x->genus)
    {
        casus MANDATUM_RECTANGULUM:
            si (x->impletum && x->exemplar != ZEPHYRUM)
            {
                _exemplar_secare(ctx, t, sx, sy, lat, alt, color,
                    x->exemplar);
            }
            alioquin si (x->impletum)
            {
                _plenum_secare(ctx, t, sx, sy, lat, alt, color);
            }
            alioquin si (sx >= ZEPHYRUM && sy >= ZEPHYRUM)
            {
                delineare_rectangulum(ctx, (i32)sx, (i32)sy,
                                      (i32)lat, (i32)alt, color);
            }
            alioquin si (lat > ZEPHYRUM && alt > ZEPHYRUM)
            {
                /* geometria delineare_rectangulum: margines quattuor */
                _plenum_secare(ctx, t, sx, sy, lat, I, color);
                _plenum_secare(ctx, t, sx, sy + alt - I, lat, I, color);
                _plenum_secare(ctx, t, sx, sy + I, I, alt - II, color);
                _plenum_secare(ctx, t, sx + lat - I, sy + I, I, alt
                    - II,
                               color);
            }
            frange;
        casus MANDATUM_LINEA:
            si (x->numerus_punctorum >= II)
            {
                s32 lx0 = s->origo_x + x->puncta[ZEPHYRUM].x * s->scala;
                s32 ly0 = s->origo_y + x->puncta[ZEPHYRUM].y * s->scala;
                s32 lx1 = s->origo_x + x->puncta[I].x * s->scala;
                s32 ly1 = s->origo_y + x->puncta[I].y * s->scala;

                si (   lx0 >= ZEPHYRUM && ly0 >= ZEPHYRUM
                    && lx1 >= ZEPHYRUM
                    && ly1 >= ZEPHYRUM)
                {
                    delineare_lineam(ctx, (i32)lx0, (i32)ly0, (i32)lx1,
                                     (i32)ly1, color);
                }
                alioquin
                {
                    _lineam_signatam(ctx, lx0, ly0, lx1, ly1, color);
                }
            }
            frange;
        casus MANDATUM_POLYGONUM:
            si (x->numerus_punctorum >= III)
            {
                 s32* signata;
                 s32* medium;
                 b32  negativum  = FALSUM;
                 i32  n          = x->numerus_punctorum;
                 i32  m;

                /* capacitas: quaeque praecisio puncta n + I addere
                 * potest - II * (n + II) satis */
                signata = (s32*)piscina_allocare(ctx->piscina,
                    (memoriae_index)(II * (n + II)) * magnitudo(s32));
                medium = (s32*)piscina_allocare(ctx->piscina,
                    (memoriae_index)(II * (n + II)) * magnitudo(s32));
                puncta = (i32*)piscina_allocare(ctx->piscina,
                    (memoriae_index)(II * (n + II)) * magnitudo(i32));
                per (i = ZEPHYRUM; i < n; i++)
                {
                    signata[i * II]      = s->origo_x
                        + x->puncta[i].x * s->scala;
                    signata[i * II + I]  = s->origo_y
                        + x->puncta[i].y * s->scala;
                    si (   signata[i * II] < ZEPHYRUM
                        || signata[i * II + I] < ZEPHYRUM)
                    {
                        negativum = VERUM;
                    }
                }
                si (negativum && !x->impletum)
                {
                    /* margo: lineae signatae, polygonum clausum */
                    per (i = ZEPHYRUM; i < n; i++)
                    {
                        i32 k = (i + I) % n;

                        _lineam_signatam(ctx, signata[i * II],
                            signata[i * II + I], signata[k * II],
                            signata[k * II + I], color);
                    }
                    frange;
                }
                m = n;
                si (negativum)
                {
                    /* plenum: ad x >= 0 deinde y >= 0 praecidere */
                    m = _polygonum_secare(signata, n, ZEPHYRUM, medium);
                    m = (m >= III) ? _polygonum_secare(medium, m, I,
                                                       signata)
                                   : ZEPHYRUM;
                }
                si (m < III)
                {
                    frange;
                }
                per (i = ZEPHYRUM; i < m; i++)
                {
                    puncta[i * II]      = (i32)signata[i * II];
                    puncta[i * II + I]  = (i32)signata[i * II + I];
                }
                si (x->impletum)
                {
                    delineare_polygonum_plenum(ctx, puncta, m, color);
                }
                alioquin
                {
                    delineare_polygonum(ctx, puncta, m, color);
                }
            }
            frange;
        casus MANDATUM_TEXTUS:
            tabula_pixelorum_pingere_chordam(t, (i32)sx, (i32)sy,
                x->textus, color_ad_pixelum(color));
            frange;
        casus MANDATUM_IMAGO:
            imago = fons ? fons(x->textus, fons_ctx) : NIHIL;
            si (imago)
            {
                imaginem_pingere(t, s, x, imago);
            }
            frange;
        ordinarius:
            frange;
    }
}


/* ==================================================
 * Cursus
 * ================================================== */

vacuum
delineare_mandata (
     constans Mandata* m,
      TabulaPixelorum* tabula,
            ImagoFons  fons,
               vacuum* ctx)
{
    delineare_mandata_selecta(m, tabula, fons, ctx, NIHIL, NIHIL);
}

vacuum
delineare_mandata_selecta (
      constans Mandata* m,
       TabulaPixelorum* tabula,
             ImagoFons  fons,
                vacuum* ctx,
      DelineareFiltrum  filtrum,
                vacuum* filtrum_ctx)
{
    ContextusDelineandi* dctx;
                 Scaena  scaenae[SCAENAE_MAXIMAE];
                    i32  altitudo;
                    i32  i;
                    i32  k;
                    i32  n;
      constans Mandatum* x;

    si (!m || !tabula)
    {
        redde;
    }
    dctx = delineare_creare_contextum(m->piscina, tabula);
    si (!dctx)
    {
        redde;
    }
    memset(&scaenae[ZEPHYRUM], ZEPHYRUM, magnitudo(Scaena));
    scaenae[ZEPHYRUM].scala    = I;
    scaenae[ZEPHYRUM].reliqua  = -I;   /* radix numquam cadit */
    altitudo                   = ZEPHYRUM;
    n                          = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(m, i);
        /* elementum quodque unum consumit ex OMNIBUS scaenis apertis
         * (magnitudo_arboris subarborem totam numerat) */
        per (k = I; k <= altitudo; k++)
        {
            scaenae[k].reliqua--;
        }
        si (x->genus == MANDATUM_COETUS)
        {
            si (altitudo + I < SCAENAE_MAXIMAE)
            {
                scaenae[altitudo + I] =
                    scaenam_impellere(&scaenae[altitudo],
                                                          x);
                altitudo++;
                sectionem_applicare(dctx, &scaenae[altitudo]);
            }
        }
        alioquin si (filtrum == NIHIL || filtrum(x, filtrum_ctx))
        {
            primitivum_pingere(dctx, tabula, &scaenae[altitudo], x,
                               fons, ctx);
        }
        /* scaenae exhaustae cadunt (coetus vacuus statim) */
        dum (   altitudo > ZEPHYRUM
             && scaenae[altitudo].reliqua <= ZEPHYRUM)
        {
            altitudo--;
            sectionem_applicare(dctx, &scaenae[altitudo]);
        }
    }
    delineare_restituere_contextum(dctx);
}


/* ==================================================
 * Modulus scopi nativi
 * ================================================== */

Modulus
delineare_mandata_modulus (
    constans TabulaPixelorum* tabula)
{
    i32 lat;
    i32 alt;

    tabula_pixelorum_cellula_textus(&lat, &alt);
    redde modulus_creare((s32)lat, (s32)alt,
        tabula ? (s32)tabula->latitudo : ZEPHYRUM,
        tabula ? (s32)tabula->altitudo : ZEPHYRUM);
}

ModulusMensor
delineare_mandata_mensor (
    vacuum)
{
    ModulusMensor r;

    r.genus     = MODULUS_MENSOR_FONTIS;
    r.politica  = RUNAE_POLITICA_GRAPHEMATUM;
    redde r;
}
