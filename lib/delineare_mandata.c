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
            si (x->impletum)
            {
                delineare_rectangulum_plenum(ctx, (i32)sx, (i32)sy,
                                             (i32)lat, (i32)alt, color);
            }
            alioquin
            {
                delineare_rectangulum(ctx, (i32)sx, (i32)sy,
                                      (i32)lat, (i32)alt, color);
            }
            frange;
        casus MANDATUM_LINEA:
            si (x->numerus_punctorum >= II)
            {
                delineare_lineam(ctx,
                    (i32)(s->origo_x
                        + x->puncta[ZEPHYRUM].x * s->scala),
                    (i32)(s->origo_y
                        + x->puncta[ZEPHYRUM].y * s->scala),
                    (i32)(s->origo_x + x->puncta[I].x * s->scala),
                    (i32)(s->origo_y + x->puncta[I].y * s->scala),
                    color);
            }
            frange;
        casus MANDATUM_POLYGONUM:
            si (x->numerus_punctorum >= III)
            {
                puncta = (i32*)piscina_allocare(ctx->piscina,
                    (memoriae_index)x->numerus_punctorum * II
                        * magnitudo(i32));
                per (i = ZEPHYRUM; i < x->numerus_punctorum; i++)
                {
                                        sx = s->origo_x
                                            + x->puncta[i].x * s->scala;
                    sy = s->origo_y + x->puncta[i].y * s->scala;
                    puncta[i * II] = (i32)sx;
                    puncta[i * II + I] = (i32)sy;
                }
                si (x->impletum)
                {
                    delineare_polygonum_plenum(ctx, puncta,
                                               x->numerus_punctorum,
                                               color);
                }
                alioquin
                {
                    delineare_polygonum(ctx, puncta,
                        x->numerus_punctorum,
                                        color);
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
        alioquin
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
