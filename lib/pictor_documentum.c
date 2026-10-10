/* pictor_documentum.c - pars pictoris documenti (pixela, ictus);
 * cauda per historia */

#include "pictor_documentum.h"
#include "sors.h"
#include "exemplaria.h"
#include "delineare_mandata.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"
#include "stml.h"
#include "xar.h"
#include "flatura.h"
#include "dithering.h"
#include "imago_opus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior memoriae_index
mensura_pixelorum (
    constans PictorDocumentum* doc)
{
    redde (memoriae_index)doc->latitudo * (memoriae_index)doc->altitudo
         * magnitudo(i32);
}

interior s32
attributum_s32 (
             StmlNodus* n,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}


/* ==================================================
 * Applicatio actorum
 * ================================================== */

/* segmentum ad tabulam [0, latitudo) x [0, altitudo) praecidere
 * (Cohen-Sutherland, s64 - nihil revolvitur); FALSUM si totum extra.
 * Coordinatae negativae ut i32 insignatum ingentes fiebant: tractus
 * extra marginem ad oppositum saliebat (Franus 2026-10-08). */
interior b32
segmentum_praecidere (
     s64* x0,
     s64* y0,
     s64* x1,
     s64* y1,
     s64  latitudo,
     s64  altitudo)
{
    s64 x_ultimum;
    s64 y_ultimum;
    s64 x;
    s64 y;
    i32 c0;
    i32 c1;
    i32 c;

    x_ultimum = latitudo - I;
    y_ultimum = altitudo - I;
    si (x_ultimum < ZEPHYRUM || y_ultimum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (VERUM)
    {
        c0 = (*x0 < ZEPHYRUM ? I : ZEPHYRUM) | (*x0
            > x_ultimum ? II : ZEPHYRUM)
            | (*y0 < ZEPHYRUM ? IV : ZEPHYRUM) | (*y0
               > y_ultimum ? VIII : ZEPHYRUM);
        c1 = (*x1 < ZEPHYRUM ? I : ZEPHYRUM) | (*x1
            > x_ultimum ? II : ZEPHYRUM)
            | (*y1 < ZEPHYRUM ? IV : ZEPHYRUM) | (*y1
               > y_ultimum ? VIII : ZEPHYRUM);
        si (!(c0 | c1))
        {
            redde VERUM;
        }
        si (c0 & c1)
        {
            redde FALSUM;
        }
        c = c0 ? c0 : c1;
        si (c & IV)
        {
            x = *x0 + (*x1 - *x0) * (ZEPHYRUM - *y0) / (*y1 - *y0);
            y = ZEPHYRUM;
        }
        alioquin si (c & VIII)
        {
            x = *x0 + (*x1 - *x0) * (y_ultimum - *y0) / (*y1 - *y0);
            y = y_ultimum;
        }
        alioquin si (c & II)
        {
            y = *y0 + (*y1 - *y0) * (x_ultimum - *x0) / (*x1 - *x0);
            x = x_ultimum;
        }
        alioquin
        {
            y = *y0 + (*y1 - *y0) * (ZEPHYRUM - *x0) / (*x1 - *x0);
            x = ZEPHYRUM;
        }
        si (c == c0)
        {
            *x0 = x;
            *y0 = y;
        }
        alioquin
        {
            *x1 = x;
            *y1 = y;
        }
    }
}

/* gutta: sors ex (semen, puncto, gutta) - rivus derivatus, nulla
 * dependentia ordinis; rejectio in disco (XVI tentamina; deinde
 * centrum) */
vacuum
pictor_gutta (
    s64  semen,
    i32  i,
    i32  k,
    s32  radius,
    s32* dx,
    s32* dy)
{
    Sors s;
     s32 x;
     s32 y;
     i32 n;

    *dx = ZEPHYRUM;
    *dy = ZEPHYRUM;
    si (radius < I)
    {
        redde;
    }
    sors_seminare(&s, (i64)semen, (i64)i * (i64)M + (i64)k);
    per (n = ZEPHYRUM; n < XVI; n++)
    {
        x = sors_inter(&s, -radius, radius);
        y = sors_inter(&s, -radius, radius);
        si (x * x + y * y <= radius * radius)
        {
            *dx = x;
            *dy = y;
            redde;
        }
    }
}

/* P3: atramentum ictus - color primus (bitus positi), secundus
 * (ceteri), exemplar; color extra [0, XVI) = nullus (intactum) */
nomen structura {
    s32 primus;
    s32 secundus;
    i32 exemplar;
} Atramentum;

interior b32
color_validus (
    s32 color)
{
    redde color >= ZEPHYRUM && color < XVI;
}

interior Atramentum
atramentum_legere (
    StmlNodus* ictus)
{
    Atramentum a;
           s32 e;

    a.primus    = attributum_s32(ictus, "color", (s32)PALETTE_BLACK);
    a.secundus  = attributum_s32(ictus, "color_secundus", -I);
    e           = attributum_s32(ictus, "exemplar", ZEPHYRUM);
    a.exemplar  = ZEPHYRUM;
    si (e > ZEPHYRUM && e < (s32)EXEMPLAR_NUMERUS)
    {
        a.exemplar = (i32)e;
    }
    redde a;
}

/* pixelum atramenti ad (x, y) TABULAE; FALSUM = intactum */
interior b32
atramenti_pixelum (
    constans Atramentum* a,
                    s32  x,
                    s32  y,
                    i32* pixelum)
{
    s32 color;

    color = exemplar_punctum(a->exemplar, x, y) ? a->primus
                                                : a->secundus;
    si (!color_validus(color))
    {
        redde FALSUM;
    }
    *pixelum = color_ad_pixelum(color_ex_palette((i32)color));
    redde VERUM;
}

/* aspergillum: guttae circa quodque punctum (vide caput); gutta
 * quaeque colorem atramenti ad locum suum */
interior vacuum
guttas_applicare (
       PictorDocumentum* doc,
              StmlNodus* ictus,
    constans Atramentum* atramentum,
                    s32  magnitudo_penicilli)
{
          i32  pixelum;
    StmlNodus* punctum;
          s64  semen;
          s32  radius;
          s32  x;
          s32  y;
          s32  t;
          s32  t_ante;
          s32  dx;
          s32  dy;
          s32  numerus;
          i32  i;
          i32  k;
          i32  n;
          i32  ordo;

    semen   = (s64)attributum_s32(ictus, "semen", ZEPHYRUM);
    radius  = PICTOR_ASPERGILLI_RADIUS * magnitudo_penicilli;
    n       = stml_numerus_liberorum(ictus);
    t_ante  = ZEPHYRUM;
    ordo    = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        punctum = stml_liberum_ad_indicem(ictus, i);
        si (punctum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        x = attributum_s32(punctum, "x", ZEPHYRUM);
        y = attributum_s32(punctum, "y", ZEPHYRUM);
        t = attributum_s32(punctum, "t", t_ante);
        /* P4b: densitas linearis - magnitudo m: VI x m per punctum, m
         * per GUTTA_MS morae (m I: ut ante) */
        numerus  = PICTOR_GUTTAE_PUNCTO * magnitudo_penicilli;
        si (ordo > ZEPHYRUM && t > t_ante)
        {
            numerus += (t - t_ante) / PICTOR_GUTTA_MS
                     * magnitudo_penicilli;
        }
        per (k = ZEPHYRUM; k < (i32)numerus; k++)
        {
            pictor_gutta(semen, ordo, k, radius, &dx, &dy);
            si (   x + dx >= ZEPHYRUM && y + dy >= ZEPHYRUM
                && x + dx < (s32)doc->tabula->latitudo
                && y + dy < (s32)doc->tabula->altitudo
                && atramenti_pixelum(atramentum, x + dx, y + dy,
                       &pixelum))
            {
                tabula_pixelorum_ponere_pixelum(doc->tabula,
                    (i32)(x + dx), (i32)(y + dy), pixelum);
            }
        }
        t_ante = t;
        ordo++;
    }
}


/* ==================================================
 * Verrere: penicillus (discus) et spongia (quadratum)
 * ================================================== */

vacuum
pictor_lineam_ambulare (
                s32  x0,
                s32  y0,
                s32  x1,
                s32  y1,
    PictorVestigium  vestigium,
             vacuum* ctx)
{
    s32 dx;
    s32 dy;
    s32 sx;
    s32 sy;
    s32 error;
    s32 e2;

    /* idem Bresenham ac delineare_lineam (comparationes strictae):
     * penicillus I eadem pixela ac ante P4 */
    dx     = x1 > x0 ? x1 - x0 : x0 - x1;
    dy     = y1 > y0 ? y1 - y0 : y0 - y1;
    sx     = x0 < x1 ? I : -I;
    sy     = y0 < y1 ? I : -I;
    error  = dx - dy;
    dum (x0 != x1 || y0 != y1)
    {
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
        vestigium(x0, y0, ctx);
    }
}

b32
pictor_disci_pixelum (
    s32 n,
    s32 i,
    s32 j)
{
    s32 a;
    s32 b;

    si (i < ZEPHYRUM || j < ZEPHYRUM || i >= n || j >= n)
    {
        redde FALSUM;
    }
    si (n <= III)
    {
        redde VERUM;
    }
    /* coordinatae duplicatae a centro: (2i - (n - 1))^2 + ... <= n^2 */
    a = II * i - (n - I);
    b = II * j - (n - I);
    redde a * a + b * b <= n * n;
}

/* sigillum verrendi: discus (penicillus, colore per modum contextus)
 * aut quadratum (spongia, pixelum fundi) */
nomen structura {
       PictorDocumentum* doc;
    ContextusDelineandi* ctx;
                  Color  color;
                    i32  pixelum;
                    s32  n;
                    b32  quadratum;
} Impressio;

interior vacuum
sigillum_imprimere (
       s32  cx,
       s32  cy,
    vacuum* ctx)
{
    Impressio* g;
          s32  i;
          s32  j;
          s32  x;
          s32  y;

    g = (Impressio*)ctx;
    per (j = ZEPHYRUM; j < g->n; j++)
    {
        y = cy - g->n / II + j;
        si (y < ZEPHYRUM || y >= (s32)g->doc->tabula->altitudo)
        {
            perge;
        }
        per (i = ZEPHYRUM; i < g->n; i++)
        {
            x = cx - g->n / II + i;
            si (   x < ZEPHYRUM || x >= (s32)g->doc->tabula->latitudo
                || (!g->quadratum && !pictor_disci_pixelum(g->n, i, j)))
            {
                perge;
            }
            si (g->quadratum)
            {
                tabula_pixelorum_ponere_pixelum(g->doc->tabula, (i32)x,
                    (i32)y, g->pixelum);
            }
            alioquin
            {
                delineare_pixelum(g->ctx, (i32)x, (i32)y, g->color);
            }
        }
    }
}

/* verrere (vide caput): segmentum quodque ad tabulam cum margine n-1
 * praecisum (sigillum ultra eam tabulam non tangit), sigillum in
 * initio praeciso et per lineam; deinde in puncto ipso. Penicillus I:
 * eadem pixela ac olim delineare_lineam + rectangulum puncti */
interior vacuum
verrere (
     PictorDocumentum* doc,
            StmlNodus* ictus,
            Impressio* g)
{
    StmlNodus* punctum;
          s32  x;
          s32  y;
          s32  x_ante;
          s32  y_ante;
          s64  x0;
          s64  y0;
          s64  x1;
          s64  y1;
          s64  margo;
          b32  primum;
          i32  i;
          i32  n;

    margo   = (s64)(g->n - I);
    n       = stml_numerus_liberorum(ictus);
    primum  = VERUM;
    x_ante  = ZEPHYRUM;
    y_ante  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        punctum = stml_liberum_ad_indicem(ictus, i);
        si (punctum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        x = attributum_s32(punctum, "x", ZEPHYRUM);
        y = attributum_s32(punctum, "y", ZEPHYRUM);
        si (!primum)
        {
            x0 = (s64)x_ante + margo;
            y0 = (s64)y_ante + margo;
            x1 = (s64)x + margo;
            y1 = (s64)y + margo;
            si (segmentum_praecidere(&x0, &y0, &x1, &y1,
                    (s64)doc->tabula->latitudo + II * margo,
                    (s64)doc->tabula->altitudo + II * margo))
            {
                sigillum_imprimere((s32)(x0 - margo), (s32)(y0 - margo),
                    g);
                pictor_lineam_ambulare((s32)(x0 - margo),
                    (s32)(y0 - margo), (s32)(x1 - margo),
                    (s32)(y1 - margo), sigillum_imprimere, g);
            }
        }
        sigillum_imprimere(x, y, g);
        primum = FALSUM;
        x_ante = x;
        y_ante = y;
    }
}

/* spongia (vide caput): quadratum colore fundi */
interior vacuum
spongiam_applicare (
    PictorDocumentum* doc,
           StmlNodus* ictus,
                 s32  magnitudo_penicilli)
{
    Impressio g;

    g.doc    = doc;
    g.ctx    = NIHIL;
    g.color  = thema_color(COLOR_BACKGROUND);
    /* L2: stratum suum ad perspicuum (alpha 0) - olim color fundi in
     * tabula unica */
    g.pixelum    = ZEPHYRUM;
    g.n          = PICTOR_SPONGIAE_LATUS * magnitudo_penicilli;
    g.quadratum  = VERUM;
    verrere(doc, ictus, &g);
}

/* penicillus: discus magnitudinis per verrere, colore 'color' per
 * modum contextus (solidus aut exemplar) */
interior vacuum
penicillum_pingere (
       PictorDocumentum* doc,
    ContextusDelineandi* ctx,
              StmlNodus* ictus,
                  Color  color,
                    s32  magnitudo_penicilli)
{
    Impressio g;

    g.doc        = doc;
    g.ctx        = ctx;
    g.color      = color;
    g.pixelum    = ZEPHYRUM;
    g.n          = magnitudo_penicilli;
    g.quadratum  = FALSUM;
    verrere(doc, ictus, &g);
}

/* <ictus instrumentum color magnitudo [color_secundus] [exemplar]>
 * <punctum x y/>...</ictus> - in tabulam doc->tabula (stratum, per
 * ictum_applicare) */
interior vacuum
ictum_pingere (
    PictorDocumentum* doc,
           StmlNodus* ictus)
{
    ContextusDelineandi* ctx;
             Atramentum  atramentum;
                    s32  magnitudo_penicilli;
                     i8  inversum[VIII];
            constans i8* octeti;
                    i32  k;

    ctx = delineare_creare_contextum(doc->piscina, doc->tabula);
    si (!ctx)
    {
        redde;
    }
    /* palette Aquinas ipsa (XVI colores distincti; Franus 2026-10-09)
     * - olim thema_color_ex_indice_colorationis, quae munera
     * syntaxeos (0-12) colorat: XIII-XV et duplicata colorem textus
     * dabant, V ('album') aurum */
    atramentum           = atramentum_legere(ictus);
    magnitudo_penicilli  = attributum_s32(ictus, "magnitudo", I);
    si (magnitudo_penicilli < I)
    {
        magnitudo_penicilli = I;
    }
    si (   stml_attributum_capere(ictus, "instrumentum")
        && chorda_aequalis_literis(*stml_attributum_capere(ictus,
               "instrumentum"), "aspergillum"))
    {
        guttas_applicare(doc, ictus, &atramentum, magnitudo_penicilli);
        delineare_restituere_contextum(ctx);
        redde;
    }
    si (   stml_attributum_capere(ictus, "instrumentum")
        && chorda_aequalis_literis(*stml_attributum_capere(ictus,
               "instrumentum"), "spongia"))
    {
        spongiam_applicare(doc, ictus, magnitudo_penicilli);
        delineare_restituere_contextum(ctx);
        redde;
    }
    /* P3: bis pingitur - primus per exemplar, secundus per exemplar
     * inversum; solidus (0) = modus solidus, pixela eadem ac ante P3.
     * Color nullus: transitus omittitur (olim color_ex_palette(-1)
     * octetos ante palettam legebat) */
    si (color_validus(atramentum.primus))
    {
        si (atramentum.exemplar != ZEPHYRUM)
        {
            delineare_ponere_exemplar_internum(ctx,
                atramentum.exemplar);
            delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
        }
        penicillum_pingere(doc, ctx, ictus,
            color_ex_palette((i32)atramentum.primus),
            magnitudo_penicilli);
    }
    octeti = exemplar_obtinere(atramentum.exemplar);
    si (   atramentum.exemplar != ZEPHYRUM && octeti
        && color_validus(atramentum.secundus))
    {
        per (k = ZEPHYRUM; k < VIII; k++)
        {
            inversum[k] = (i8)~octeti[k];
        }
        delineare_ponere_exemplar(ctx, inversum);
        delineare_ponere_modum(ctx, MODUS_EXEMPLAR);
        penicillum_pingere(doc, ctx, ictus,
            color_ex_palette((i32)atramentum.secundus),
            magnitudo_penicilli);
    }
    delineare_restituere_contextum(ctx);
}


/* ==================================================
 * Strata (pictor-strata L2)
 * ================================================== */

/* index strati per id; -1 si abest */
interior s32
strati_index (
    constans PictorDocumentum* doc,
                          s32  id)
{
    i32 k;

    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        si (doc->strata[k].id == id)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* tabula loci k: reservata (stratum deletum) aut nova; perspicua */
interior TabulaPixelorum*
strati_tabula (
    PictorDocumentum* doc,
                 i32  k)
{
    si (!doc->strata[k].pixela)
    {
        doc->strata[k].pixela =
            tabula_pixelorum_creare_nuda(doc->piscina,
            doc->latitudo, doc->altitudo);
    }
    si (doc->strata[k].pixela)
    {
        tabula_pixelorum_vacare(doc->strata[k].pixela, ZEPHYRUM);
    }
    redde doc->strata[k].pixela;
}

/* status ante actum primum: stratum unum (id I) perspicuum; tabulae
 * locorum ceterorum reservantur */
interior vacuum
strata_vacare (
    PictorDocumentum* doc)
{
    doc->numerus_stratorum          = I;
    doc->strata[ZEPHYRUM].id        = I;
    doc->strata[ZEPHYRUM].visibile  = VERUM;
    (vacuum)strati_tabula(doc, ZEPHYRUM);
}

/* compositum: color fundi, deinde strata visibilia ab imo (alpha 0 =
 * perspicuum). Fundus = color SEMANTICUS thematis (thema_color), non
 * index colorationis (Franus 2026-10-09: ut scriba et terminale; olim
 * PALETTE_WHITE, flavum clarum) - in actis non est: picturae omnes
 * fundo hodierno redduntur */
interior vacuum
componere (
    PictorDocumentum* doc)
{
    i32* c;
    i32* s;
    i32  n;
    i32  i;
    i32  k;

    tabula_pixelorum_vacare(doc->tabula, color_ad_pixelum(
        thema_color(COLOR_BACKGROUND)));
    c = doc->tabula->pixela;
    n = doc->latitudo * doc->altitudo;
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        si (!doc->strata[k].visibile || !doc->strata[k].pixela)
        {
            perge;
        }
        s = doc->strata[k].pixela->pixela;
        per (i = ZEPHYRUM; i < n; i++)
        {
            si ((s[i] >> XXIV) != ZEPHYRUM)
            {
                c[i] = s[i];
            }
        }
    }
}

/* ictus in strato suo ("stratum", absens = I; stratum ignotum: nihil):
 * tabula doc->tabula ad stratum dum pingitur (rasteres omnes eam
 * legunt), deinde compositum restituitur */
interior vacuum
ictum_applicare (
    PictorDocumentum* doc,
           StmlNodus* ictus)
{
     TabulaPixelorum* compositum;
                 s32  index;

    index = strati_index(doc, attributum_s32(ictus, "stratum", I));
    si (index < ZEPHYRUM || !doc->strata[index].pixela)
    {
        redde;
    }
    compositum   = doc->tabula;
    doc->tabula  = doc->strata[index].pixela;
    ictum_pingere(doc, ictus);
    doc->tabula  = compositum;
}

/* <stratum actio=.../> (vide caput) */
interior vacuum
stratum_applicare (
    PictorDocumentum* doc,
           StmlNodus* n)
{
             chorda* actio;
             chorda* ids;
                s32  id;
                s32  index;
                s32  supra;
                i32  k;
      PictorStratum  stratum;
    TabulaPixelorum* reservata;

    actio  = stml_attributum_capere(n, "actio");
    id     = attributum_s32(n, "id", ZEPHYRUM);
    si (!actio)
    {
        redde;
    }
    index = strati_index(doc, id);
    si (chorda_aequalis_literis(*actio, "novum"))
    {
        si (   id < I || index >= ZEPHYRUM
            || doc->numerus_stratorum >= (i32)PICTOR_STRATA_MAXIMA)
        {
            redde;
        }
        supra = strati_index(doc, attributum_s32(n, "supra", ZEPHYRUM));
        supra = supra < ZEPHYRUM ? (s32)doc->numerus_stratorum - I
                                 : supra;
        /* tabula reservata loci numerus (si qua) stratum novum fit */
        reservata = doc->strata[doc->numerus_stratorum].pixela;
        per (k = doc->numerus_stratorum; k > (i32)(supra + I); k--)
        {
            doc->strata[k] = doc->strata[k - I];
        }
        doc->strata[supra + I].id        = id;
        doc->strata[supra + I].visibile  = VERUM;
        doc->strata[supra + I].pixela    = reservata;
        doc->numerus_stratorum++;
        (vacuum)strati_tabula(doc, (i32)(supra + I));
        redde;
    }
    si (chorda_aequalis_literis(*actio, "deletum"))
    {
        si (index < ZEPHYRUM || doc->numerus_stratorum <= I)
        {
            redde;
        }
        reservata = doc->strata[index].pixela;
        per (k = (i32)index; k + I < doc->numerus_stratorum; k++)
        {
            doc->strata[k] = doc->strata[k + I];
        }
        doc->numerus_stratorum--;
        doc->strata[doc->numerus_stratorum].id        = ZEPHYRUM;
        doc->strata[doc->numerus_stratorum].visibile  = FALSUM;
        doc->strata[doc->numerus_stratorum].pixela    = reservata;
        redde;
    }
    si (chorda_aequalis_literis(*actio, "visibile"))
    {
        si (index >= ZEPHYRUM)
        {
            doc->strata[index].visibile =
                attributum_s32(n, "valor", I) != ZEPHYRUM;
        }
        redde;
    }
    si (chorda_aequalis_literis(*actio, "ordo"))
    {
        PictorStratum  ordo[PICTOR_STRATA_MAXIMA];
                  b32  usus[PICTOR_STRATA_MAXIMA];
            character* s;
            character* finis;
                 long  v;
                  i32  numerus;

        ids = stml_attributum_capere(n, "ids");
        si (!ids)
        {
            redde;
        }
        memset(usus, ZEPHYRUM, magnitudo(usus));
        s        = chorda_ut_cstr(*ids, doc->piscina);
        numerus  = ZEPHYRUM;
        dum (*s)
        {
            v = strtol(s, &finis, X);
            si (finis == s)
            {
                frange;
            }
            s      = finis;
            index  = strati_index(doc, (s32)v);
            si (   index < ZEPHYRUM || usus[index]
                || numerus >= doc->numerus_stratorum)
            {
                redde;
            }
            usus[index]    = VERUM;
            ordo[numerus]  = doc->strata[index];
            numerus++;
        }
        si (numerus != doc->numerus_stratorum)
        {
            redde;
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            stratum         = ordo[k];
            doc->strata[k]  = stratum;
        }
    }
}

/* L5: <imago stratum x y latitudo altitudo massa/> - indices ex massa
 * (deflati) in stratum: palette Aquinas, 0xFF perspicuum (intactum) */
interior vacuum
imaginem_applicare (
    PictorDocumentum* doc,
           StmlNodus* n)
{
            chorda* massa_hex;
            chorda  massa;
               b32  inventum;
    FlaturaFructus  fructus;
               s32  index;
               s32  x0;
               s32  y0;
               s32  lat;
               s32  alt;
               s32  x;
               s32  y;
               i32  k;
               i32* pixela;

    index      = strati_index(doc, attributum_s32(n, "stratum", I));
    massa_hex  = stml_attributum_capere(n, "massa");
    x0         = attributum_s32(n, "x", ZEPHYRUM);
    y0         = attributum_s32(n, "y", ZEPHYRUM);
    lat        = attributum_s32(n, "latitudo", ZEPHYRUM);
    alt        = attributum_s32(n, "altitudo", ZEPHYRUM);
    si (   index < ZEPHYRUM || !massa_hex || lat <= ZEPHYRUM
        || alt <= ZEPHYRUM || !doc->strata[index].pixela)
    {
        redde;
    }
    massa = volumen_massam_promere(doc->volumen, *massa_hex,
        doc->piscina, &inventum);
    si (!inventum)
    {
        redde;
    }
    fructus = flatura_inflare(massa.datum, massa.mensura, doc->piscina);
    si (   fructus.status       != FLATURA_STATUS_OK
        || (s32)fructus.mensura != lat * alt)
    {
        redde;
    }
    pixela = doc->strata[index].pixela->pixela;
    per (y = ZEPHYRUM; y < alt; y++)
    {
        si (y0 + y < ZEPHYRUM || y0 + y >= (s32)doc->altitudo)
        {
            perge;
        }
        per (x = ZEPHYRUM; x < lat; x++)
        {
            k = (i32)(y * lat + x);
            si (   x0 + x < ZEPHYRUM || x0 + x >= (s32)doc->latitudo
                || fructus.datum[k] >= XVI)
            {
                perge;
            }
            pixela[(i32)(y0 + y) * doc->latitudo + (i32)(x0 + x)] =
                color_ad_pixelum(color_ex_palette(
                    (i32)fructus.datum[k]));
        }
    }
}

interior vacuum
actum_applicare (
    PictorDocumentum* doc,
              chorda  datum)
{
    StmlResultus res;

    res = stml_legere_ex_literis(chorda_ut_cstr(datum, doc->piscina),
                                 doc->piscina, doc->intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde;
    }
    si (chorda_aequalis_literis(*res.elementum_radix->titulus, "ictus"))
    {
        ictum_applicare(doc, res.elementum_radix);
    }
    alioquin si (chorda_aequalis_literis(*res.elementum_radix->titulus,
                 "stratum"))
    {
        stratum_applicare(doc, res.elementum_radix);
    }
    alioquin si (chorda_aequalis_literis(*res.elementum_radix->titulus,
                 "imago"))
    {
        imaginem_applicare(doc, res.elementum_radix);
    }
    /* ramus: nihil pingit; cetera v1 ignorata (worklog) */
}

/* proiectio pro historia (L2): strata; compositum in sigillare
 * (historia id vocat cum status constat - post actum, reproiectionem,
 * vacationem) */
interior vacuum
proiectio_vacare (
    vacuum* ctx)
{
    strata_vacare((PictorDocumentum*)ctx);
}

/* status crudus canonicus: numerus, (id, visibile) per stratum, pixela
 * stratorum; compositum renovatur */
interior vacuum
proiectio_sigillare (
      vacuum* ctx,
    Sigillum* exitus)
{
     PictorDocumentum* doc;
    SigillumContextus  sc;
                  i32  k;
                  s32  caput[II];

    doc = (PictorDocumentum*)ctx;
    componere(doc);
    sigillum_incipere(&sc);
    sigillum_addere(&sc, &doc->numerus_stratorum,
        magnitudo(doc->numerus_stratorum));
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        caput[ZEPHYRUM]  = doc->strata[k].id;
        caput[I]         = doc->strata[k].visibile ? I : ZEPHYRUM;
        sigillum_addere(&sc, caput, magnitudo(caput));
        si (doc->strata[k].pixela)
        {
            sigillum_addere(&sc, doc->strata[k].pixela->pixela,
                mensura_pixelorum(doc));
        }
    }
    *exitus = sigillum_finire(&sc);
}

/* checkpoint: "STRATA1 <n> <id>:<visibile>...\n" deinde per stratum
 * longitudo (IV octeti, maior primus) et pixela deflata */
interior chorda
proiectio_codificare (
     vacuum* ctx,
    Piscina* piscina)
{
      PictorDocumentum* doc;
        FlaturaFructus  fructus[PICTOR_STRATA_MAXIMA];
                chorda  caput;
                chorda  codex;
             character  par[XXIV];
                   i32  k;
                   i32  mensura;
                   i32  locus;

    doc            = (PictorDocumentum*)ctx;
    codex.datum    = NIHIL;
    codex.mensura  = ZEPHYRUM;
    sprintf(par, "STRATA1 %d", (integer)doc->numerus_stratorum);
    caput = chorda_ex_literis(par, piscina);
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        sprintf(par, " %d:%d", (integer)doc->strata[k].id,
            doc->strata[k].visibile ? I : ZEPHYRUM);
        caput = chorda_concatenare(caput, chorda_ex_literis(par,
            piscina),
            piscina);
    }
    caput = chorda_concatenare(caput, chorda_ex_literis("\n", piscina),
        piscina);
    mensura = caput.mensura;
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        si (!doc->strata[k].pixela)
        {
            redde codex;
        }
        fructus[k] = flatura_deflare((constans i8*)doc->strata[k].pixela
            ->pixela, (i32)mensura_pixelorum(doc),
            FLATURA_COMPRESSIO_RAPIDA, piscina);
        si (fructus[k].status != FLATURA_STATUS_OK)
        {
            redde codex;
        }
        mensura += IV + fructus[k].mensura;
    }
    codex.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    si (!codex.datum)
    {
        redde codex;
    }
    memcpy(codex.datum, caput.datum, (size_t)caput.mensura);
    locus = caput.mensura;
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        codex.datum[locus]        = (i8)(fructus[k].mensura >> XXIV);
        codex.datum[locus + I]    = (i8)(fructus[k].mensura >> XVI);
        codex.datum[locus + II]   = (i8)(fructus[k].mensura >> VIII);
        codex.datum[locus + III]  = (i8)fructus[k].mensura;
        locus                     += IV;
        memcpy(codex.datum + locus, fructus[k].datum,
            (size_t)fructus[k].mensura);
        locus += fructus[k].mensura;
    }
    codex.mensura = mensura;
    redde codex;
}

/* restitutio; FALSUM (forma vetus - massa cruda - aut corrupta):
 * historia reproicit ex nihilo */
interior b32
proiectio_decodificare (
    vacuum* ctx,
    chorda  codex)
{
     PictorDocumentum* doc;
       FlaturaFructus  fructus;
                  i32  locus;
                  i32  finis_capitis;
                  i32  numerus;
                  i32  k;
                  i32  longitudo;
            character  caput[CCLVI];
            character* s;
            character* post;
                 long  v;
                 long  vis;

    doc = (PictorDocumentum*)ctx;
    si (   codex.mensura < VIII
        || memcmp(codex.datum, "STRATA1 ", VIII) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    finis_capitis = ZEPHYRUM;
    dum (   finis_capitis < codex.mensura && finis_capitis < CCLV
         && codex.datum[finis_capitis] != '\n')
    {
        finis_capitis++;
    }
    si (finis_capitis >= codex.mensura || finis_capitis >= CCLV)
    {
        redde FALSUM;
    }
    memcpy(caput, codex.datum, (size_t)finis_capitis);
    caput[finis_capitis]  = '\0';
    s                     = caput + VIII;
    v                     = strtol(s, &post, X);
    si (post == s || v < I || v > (long)PICTOR_STRATA_MAXIMA)
    {
        redde FALSUM;
    }
    numerus  = (i32)v;
    s        = post;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        v = strtol(s, &post, X);
        si (post == s || *post != ':')
        {
            redde FALSUM;
        }
        s    = post + I;
        vis  = strtol(s, &post, X);
        si (post == s)
        {
            redde FALSUM;
        }
        s                        = post;
        doc->strata[k].id        = (s32)v;
        doc->strata[k].visibile  = vis != ZEPHYRUM;
    }
    locus = finis_capitis + I;
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (locus + IV > codex.mensura)
        {
            redde FALSUM;
        }
        longitudo = ((i32)(i8)codex.datum[locus] << XXIV)
                  | ((i32)(i8)codex.datum[locus + I] << XVI)
                  | ((i32)(i8)codex.datum[locus + II] << VIII)
                  | (i32)(i8)codex.datum[locus + III];
        locus += IV;
        si (longitudo > codex.mensura - locus)
        {
            redde FALSUM;
        }
        fructus = flatura_inflare(codex.datum + locus, longitudo,
            doc->piscina);
        si (   fructus.status                  != FLATURA_STATUS_OK
            || (memoriae_index)fructus.mensura != mensura_pixelorum(doc)
            || !strati_tabula(doc, k))
        {
            redde FALSUM;
        }
        memcpy(doc->strata[k].pixela->pixela, fructus.datum,
            mensura_pixelorum(doc));
        locus += longitudo;
    }
    doc->numerus_stratorum = numerus;
    redde VERUM;
}

interior vacuum
proiectio_applicare (
    vacuum* ctx,
    chorda  actum)
{
    actum_applicare((PictorDocumentum*)ctx, actum);
}

interior HistoriaProiectio
proiectio_facere (
    PictorDocumentum* doc)
{
    HistoriaProiectio p;

    /* L2: cliens codex (historia L1) - strata, non memoria fixa */
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.memoria       = NIHIL;
    p.mensura       = ZEPHYRUM;
    p.vacare        = proiectio_vacare;
    p.applicare     = proiectio_applicare;
    p.ctx           = doc;
    p.sigillare     = proiectio_sigillare;
    p.codificare    = proiectio_codificare;
    p.decodificare  = proiectio_decodificare;
    redde p;
}


/* ==================================================
 * Vita
 * ================================================== */

Xar*
pictor_documenta_enumerare (
     Volumen* volumen,
     Piscina* piscina)
{
               Xar* plagulae;
               Xar* l;
    VolumenPlagula* pl;
            chorda  spatium;
               i32  i;

    si (!volumen || !piscina)
    {
        redde NIHIL;
    }
    l         = xar_creare(piscina, (i32)magnitudo(chorda));
    plagulae  = volumen_plagulas_enumerare(volumen, piscina);
    per (i = ZEPHYRUM; l && plagulae && i < xar_numerus(plagulae); i++)
    {
        pl = (VolumenPlagula*)xar_obtinere(plagulae, i);
        si (!chorda_aequalis_literis(pl->origo, "pictor:documentum"))
        {
            perge;
        }
        /* "documentum" (nuda) aut "<spatium>/documentum" */
        si (chorda_aequalis_literis(pl->via, "documentum"))
        {
            spatium = chorda_ex_literis("", piscina);
        }
        alioquin si (   pl->via.mensura > XI
                     && memcmp(pl->via.datum + pl->via.mensura - XI,
                            "/documentum", XI) == ZEPHYRUM)
        {
            spatium.datum    = pl->via.datum;
            spatium.mensura  = pl->via.mensura - XI;
        }
        alioquin
        {
            perge;
        }
        *(chorda*)xar_addere(l) = spatium;
    }
    redde l;
}

/* clavis manifesti: "spatium/documentum" (spatium vacuum: nuda) */
interior chorda
clavis_manifesti (
               Piscina* piscina,
    constans character* spatium)
{
    chorda c;

    si (!spatium || !spatium[ZEPHYRUM])
    {
        redde chorda_ex_literis("documentum", piscina);
    }
    c = chorda_concatenare(chorda_ex_literis(spatium, piscina),
                           chorda_ex_literis("/documentum", piscina),
                           piscina);
    redde c;
}

interior PictorDocumentum*
documentum_struere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;

    doc = (PictorDocumentum*)piscina_allocare(piscina,
                                              magnitudo(*doc));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PictorDocumentum));
    doc->volumen      = volumen;
    doc->piscina      = piscina;
    doc->intern       = intern;
    doc->latitudo     = latitudo;
    doc->altitudo     = altitudo;
    doc->intervallum  = intervallum > ZEPHYRUM ? intervallum : LXIV;
    doc->tabula = tabula_pixelorum_creare_nuda(piscina, latitudo,
        altitudo);
    si (!doc->tabula)
    {
        redde NIHIL;
    }
    doc->proiectio = imago_ex_tabula(doc->tabula);
    redde doc;
}

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;
              chorda  manifestum;

    si (   !piscina || !intern || !volumen || latitudo <= ZEPHYRUM
        || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    doc = documentum_struere(piscina, intern, volumen, latitudo,
        altitudo,
                             intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    manifestum = chorda_ex_literis("<documentum latitudo=\"", piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)latitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)altitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" intervallum=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)doc->intervallum, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\"/>", piscina), piscina);
    volumen_plagulam_condere(volumen, clavis_manifesti(piscina,
        spatium),
                             manifestum, "pictor:documentum");
    doc->historia = historia_creare(piscina, intern, volumen, spatium,
        "ictus",
                                    "pictor:checkpoint",
                                    doc->intervallum,
                                    proiectio_facere(doc));
    si (!doc->historia)
    {
        redde NIHIL;
    }
    redde doc;
}

PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* spatium)
{
    PictorDocumentum* doc;
              chorda  manifestum;
                 b32  inventum;
        StmlResultus  res;
                 s32  latitudo;
                 s32  altitudo;
                 s32  intervallum;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    manifestum = volumen_plagulam_promere(volumen,
        clavis_manifesti(piscina, spatium), piscina, &inventum);
    si (!inventum)
    {
        redde NIHIL;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(manifestum, piscina),
                                 piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }
    latitudo    = attributum_s32(res.elementum_radix, "latitudo",
        ZEPHYRUM);
    altitudo    = attributum_s32(res.elementum_radix, "altitudo",
        ZEPHYRUM);
    intervallum = attributum_s32(res.elementum_radix, "intervallum",
        LXIV);
    doc = documentum_struere(piscina, intern, volumen, (i32)latitudo,
                             (i32)altitudo, (i32)intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    doc->historia = historia_aperire(piscina, intern, volumen, spatium,
        "ictus",
                                     "pictor:checkpoint",
                                     doc->intervallum,
                                     proiectio_facere(doc));
    si (!doc->historia)
    {
        redde NIHIL;
    }
    redde doc;
}


/* ==================================================
 * Acta, revocare, reficere (historia)
 * ================================================== */

s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml)
{
    redde doc ? historia_actum(doc->historia, actum_stml) : ZEPHYRUM;
}

b32
pictor_documentum_revocare (
    PictorDocumentum* doc)
{
    redde doc ? historia_revocare(doc->historia) : FALSUM;
}

b32
pictor_documentum_reficere (
    PictorDocumentum* doc)
{
    redde doc ? historia_reficere(doc->historia) : FALSUM;
}


/* ==================================================
 * Lectio et verificatio
 * ================================================== */

constans Imago*
pictor_documentum_proiectio (
    constans PictorDocumentum* doc)
{
    redde doc ? &doc->proiectio : NIHIL;
}

chorda
pictor_documentum_sigillum_hex (
    constans PictorDocumentum* doc,
                      Piscina* piscina)
{
    chorda vacua;

    si (!doc)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde historia_sigillum_hex(doc->historia, piscina);
}

b32
pictor_documentum_verificare (
    PictorDocumentum* doc)
{
    redde doc ? historia_verificare(doc->historia) : FALSUM;
}

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_cursor(doc->historia) : ZEPHYRUM;
}

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_finis(doc->historia) : ZEPHYRUM;
}

i8*
pictor_imaginem_aptare (
    constans Imago* fons,
               i32  latitudo,
               i32  altitudo,
           Piscina* piscina,
               s32* x,
               s32* y,
               i32* lat,
               i32* alt)
{
               Imago scalata;
    DitheringFructus df;
                 b32 activi[XVI];
                 i32 nl;
                 i32 na;
                 i32 i;

    si (   !fons || !fons->pixela || fons->latitudo == ZEPHYRUM
        || fons->altitudo == ZEPHYRUM || latitudo == ZEPHYRUM
        || altitudo       == ZEPHYRUM || !piscina)
    {
        redde NIHIL;
    }
    /* contain: latus quod prius marginem tangit (aspectus servatur) */
    si (fons->latitudo * altitudo >= fons->altitudo * latitudo)
    {
        nl = latitudo;
        na = fons->altitudo * latitudo / fons->latitudo;
    }
    alioquin
    {
        na = altitudo;
        nl = fons->latitudo * altitudo / fons->altitudo;
    }
    nl = nl < I ? I : nl;
    na = na < I ? I : na;
    scalata = imago_scalare(fons, nl, na, nl < fons->latitudo
        ? IMAGO_SCALA_AREA : IMAGO_SCALA_BILINEARIS, piscina);
    si (!scalata.pixela)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        activi[i] = VERUM;
    }
    df = dithering_atkinson_colorum(scalata.pixela, nl, na, activi,
        piscina);
    si (!df.successus || !df.indices)
    {
        redde NIHIL;
    }
    /* alpha < CXXVIII: perspicuum */
    per (i = ZEPHYRUM; i < nl * na; i++)
    {
        si (scalata.pixela[i * IV + III] < CXXVIII)
        {
            df.indices[i] = (i8)CCLV;
        }
    }
    *x    = (s32)(latitudo - nl) / II;
    *y    = (s32)(altitudo - na) / II;
    *lat  = nl;
    *alt  = na;
    redde df.indices;
}

s32
pictor_documentum_imaginem_inserere (
     PictorDocumentum* doc,
          constans i8* indices,
                  i32  lat,
                  i32  alt,
                  s32  x,
                  s32  y,
                  s32  supra)
{
    FlaturaFructus fructus;
            chorda contentum;
         character hex[SIGILLUM_HEX_MENSURA];
         character actum[CCLVI];
               s32 id;
               i32 k;

    si (   !doc || !indices || lat == ZEPHYRUM || alt == ZEPHYRUM
        || doc->numerus_stratorum >= (i32)PICTOR_STRATA_MAXIMA)
    {
        redde ZEPHYRUM;
    }
    id = ZEPHYRUM;
    per (k = ZEPHYRUM; k < doc->numerus_stratorum; k++)
    {
        id = doc->strata[k].id > id ? doc->strata[k].id : id;
    }
    id++;
    fructus = flatura_deflare(indices, (i32)(lat * alt),
        FLATURA_COMPRESSIO_ORDINARIA, doc->piscina);
    si (fructus.status != FLATURA_STATUS_OK)
    {
        redde ZEPHYRUM;
    }
    contentum.datum    = fructus.datum;
    contentum.mensura  = fructus.mensura;
    si (!volumen_massam_condere(doc->volumen, contentum, hex))
    {
        redde ZEPHYRUM;
    }
    sprintf(actum, "<stratum actio=\"novum\" id=\"%d\" supra=\"%d\"/>",
        (integer)id, (integer)supra);
    si (historia_actum(doc->historia, chorda_ex_literis(actum,
            doc->piscina)) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    sprintf(actum, "<imago stratum=\"%d\" x=\"%d\" y=\"%d\""
        " latitudo=\"%d\" altitudo=\"%d\" massa=\"%s\"/>", (integer)id,
        (integer)x, (integer)y, (integer)lat, (integer)alt, hex);
    (vacuum)historia_actum_coniunctum(doc->historia, chorda_ex_literis(
        actum, doc->piscina));
    redde id;
}

constans PictorStratum*
pictor_documentum_stratum (
    constans PictorDocumentum* doc,
                          i32  index)
{
    si (!doc || index >= doc->numerus_stratorum)
    {
        redde NIHIL;
    }
    redde &doc->strata[index];
}

i32
pictor_documentum_numerus_stratorum (
    constans PictorDocumentum* doc)
{
    redde doc ? doc->numerus_stratorum : ZEPHYRUM;
}

chorda
pictor_documentum_sigillum_compositi_hex (
    constans PictorDocumentum* doc,
                      Piscina* piscina)
{
     Sigillum s;
    character hex[SIGILLUM_HEX_MENSURA];

    si (!doc || !piscina)
    {
        redde chorda_ex_literis("", piscina);
    }
    s = sigillum_computare(doc->tabula->pixela, mensura_pixelorum(doc));
    sigillum_hex(&s, hex);
    redde chorda_ex_literis(hex, piscina);
}

i32
pictor_documentum_numerus_vivorum (
    constans PictorDocumentum* doc)
{
    redde doc ? historia_numerus_vivorum(doc->historia) : ZEPHYRUM;
}
