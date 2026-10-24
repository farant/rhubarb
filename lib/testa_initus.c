/* testa_initus.c - partes purae testae Android (testa-spec
 * II.5-II.6; testa_initus.h) */

#include "testa_initus.h"

#include <string.h>

#define MORA_PULSUS  XVI   /* ut lib/testa.c (R4) */

/* pavimentum sine libm: (s32)v truncat versus ZEPHYRUM */
interior s32
_pavimentum (
    f32 v)
{
    s32 t;

    t = (s32)v;
    si (v < (f32)t)
    {
        t -= I;
    }
    redde t;
}

/* divisio pavimento (a negativum: -1/3 = -1, non 0) */
interior s32
_divisio_pavimenti (
    s32 a,
    s32 b)
{
    s32 q;

    q = a / b;
    si ((a % b) != ZEPHYRUM && a < ZEPHYRUM)
    {
        q -= I;
    }
    redde q;
}

b32
testa_geometriam_computare (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               i32  scala,
    TestaGeometria* g)
{
    redde testa_geometriam_in_area(fenestra_latitudo,
        fenestra_altitudo, ZEPHYRUM, ZEPHYRUM, (s32)fenestra_latitudo,
        (s32)fenestra_altitudo, scala, g);
}

b32
testa_geometriam_in_area (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               s32  x0,
               s32  y0,
               s32  x1,
               s32  y1,
               i32  scala,
    TestaGeometria* g)
{
    i32 latitudo_rect;
    i32 altitudo_rect;

    si (!g || scala < I)
    {
        redde FALSUM;
    }
    /* ad fenestram secare */
    si (x0 < ZEPHYRUM)
    {
        x0 = ZEPHYRUM;
    }
    si (y0 < ZEPHYRUM)
    {
        y0 = ZEPHYRUM;
    }
    si (x1 > (s32)fenestra_latitudo)
    {
        x1 = (s32)fenestra_latitudo;
    }
    si (y1 > (s32)fenestra_altitudo)
    {
        y1 = (s32)fenestra_altitudo;
    }
    si (x1 - x0 < (s32)scala || y1 - y0 < (s32)scala)
    {
        redde FALSUM;
    }
    latitudo_rect = (i32)(x1 - x0);
    altitudo_rect = (i32)(y1 - y0);
    g->fenestra_latitudo = fenestra_latitudo;
    g->fenestra_altitudo = fenestra_altitudo;
    g->scala = scala;
    g->latitudo = latitudo_rect / scala;
    g->altitudo = altitudo_rect / scala;
    g->margo_x = (i32)x0 + (latitudo_rect - g->latitudo * scala) / II;
    g->margo_y = (i32)y0 + (altitudo_rect - g->altitudo * scala) / II;
    redde VERUM;
}

vacuum
testa_punctum_mappare (
    constans TestaGeometria* g,
                        f32  x,
                        f32  y,
                        s32* nx,
                        s32* ny)
{
    *nx = _divisio_pavimenti(_pavimentum(x) - (s32)g->margo_x,
        (s32)g->scala);
    *ny = _divisio_pavimenti(_pavimentum(y) - (s32)g->margo_y,
        (s32)g->scala);
}

interior s32
_indicem_invenire (
    constans TestaTactusCrudus* c,
                           s32  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < c->numerus && i < TESTA_INDICATORES_MAXIMI;
         i++)
    {
        si (c->ids[i] == id)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior EventusIndicatorGenus
_genus_instrumenti (
    s32 instrumentum)
{
    si (   instrumentum == TESTA_INSTRUMENTUM_STILUS
        || instrumentum == TESTA_INSTRUMENTUM_GUMMI)
    {
        redde EVENTUS_INDICATOR_STILUS;
    }
    si (instrumentum == TESTA_INSTRUMENTUM_MUS)
    {
        redde EVENTUS_INDICATOR_MUS;
    }
    redde EVENTUS_INDICATOR_TACTUS;
}

/* eventum muris ex indicatore i crudi implere (botton sinister:
 * tactus = sinistra pressa, etiam in motu - spec D1) */
interior vacuum
_murem_implere (
                     Eventus* ev,
              eventus_genus_t  genus,
    constans TestaTactusCrudus* c,
                          s32  i,
                          s32  indicator,
       constans TestaGeometria* g)
{
    s32 x;
    s32 y;

    memset(ev, ZEPHYRUM, magnitudo(Eventus));
    testa_punctum_mappare(g, c->x[i], c->y[i], &x, &y);
    ev->genus                = genus;
    ev->tempus               = c->tempus;
    ev->datum.mus.x          = x;
    ev->datum.mus.y          = y;
    ev->datum.mus.botton     = MUS_SINISTER;
    ev->datum.mus.indicator  = indicator;
    ev->datum.mus.indicator_genus =
        _genus_instrumenti(c->instrumenta[i]);
    ev->datum.mus.pressio             = EVENTUS_PRESSIO_IGNOTA;
    ev->datum.mus.exempla             = NIHIL;
    ev->datum.mus.numerus_exemplorum  = ZEPHYRUM;
}

b32
testa_tactum_vertere (
                   TestaTactus* s,
    constans TestaTactusCrudus* c,
       constans TestaGeometria* g,
                       Eventus* ev)
{
    s32 i;
    i32 h;
    i32 n;
    b32 erat;

    si (c->numerus < I)
    {
        redde FALSUM;
    }
    commutatio (c->actio)
    {
        casus TESTA_TACTUS_DEPRESSIO:
            si ((i32)c->index_actionis >= c->numerus)
            {
                redde FALSUM;
            }
            i             = (s32)c->index_actionis;
            s->activus    = VERUM;
            s->exspectat  = FALSUM;
            s->indicator  = c->ids[i];
            _murem_implere(ev, EVENTUS_MUS_DEPRESSUS, c, i,
                s->indicator,
                g);
            redde VERUM;

        casus TESTA_TACTUS_MOTUS:
            si (!s->activus)
            {
                redde FALSUM;
            }
            i = _indicem_invenire(c, s->indicator);
            si (i < ZEPHYRUM)
            {
                redde FALSUM;
            }
            _murem_implere(ev, EVENTUS_MUS_MOTUS, c, i, s->indicator,
                g);
            n = c->numerus_historiae;
            si (n > EVENTUS_EXEMPLA_MAXIMA)
            {
                n = EVENTUS_EXEMPLA_MAXIMA;
            }
            per (h = ZEPHYRUM; h < n; h++)
            {
                testa_punctum_mappare(g, c->historia_x[i][h],
                    c->historia_y[i][h], &s->exempla[h].x,
                    &s->exempla[h].y);
                s->exempla[h].tempus = c->historia_tempus[h];
            }
            si (n > ZEPHYRUM)
            {
                ev->datum.mus.exempla             = s->exempla;
                ev->datum.mus.numerus_exemplorum  = n;
            }
            redde VERUM;

        casus TESTA_TACTUS_INDICATOR_LIBERATIO:
            si (   !s->activus || (i32)c->index_actionis >= c->numerus
                || c->ids[c->index_actionis] != s->indicator)
            {
                redde FALSUM;
            }
            s->activus    = FALSUM;
            s->exspectat  = VERUM;
            _murem_implere(ev, EVENTUS_MUS_LIBERATUS, c,
                (s32)c->index_actionis, s->indicator, g);
            redde VERUM;

        casus TESTA_TACTUS_LIBERATIO:
        casus TESTA_TACTUS_ABRUPTIO:
            erat          = s->activus;
            s->activus    = FALSUM;
            s->exspectat  = FALSUM;
            si (!erat)
            {
                redde FALSUM;
            }
            i = _indicem_invenire(c, s->indicator);
            _murem_implere(ev, EVENTUS_MUS_LIBERATUS, c,
                i >= ZEPHYRUM ? i : ZEPHYRUM, s->indicator, g);
            redde VERUM;

        ordinarius:
            /* POINTER_DOWN (digitus II), HOVER, OUTSIDE: nihil */
            redde FALSUM;
    }
}

s32
testa_actio_ex_phase (
    i32 phasis,
    i32 activi_ante,
    i32 activi_post)
{
    commutatio (phasis)
    {
        casus TESTA_PHASIS_INCIPIT:
            redde activi_ante == ZEPHYRUM
                ? TESTA_TACTUS_DEPRESSIO
                : TESTA_TACTUS_INDICATOR_DEPRESSIO;
        casus TESTA_PHASIS_MOVETUR:
            redde TESTA_TACTUS_MOTUS;
        casus TESTA_PHASIS_FINIT:
            redde activi_post == ZEPHYRUM
                ? TESTA_TACTUS_LIBERATIO
                : TESTA_TACTUS_INDICATOR_LIBERATIO;
        casus TESTA_PHASIS_ABRUPTA:
            redde TESTA_TACTUS_ABRUPTIO;
        ordinarius:
            redde -I;
    }
}

b32
testa_mensuras_aptare (
    i32  latitudo,
    i32  altitudo,
    i32  maxima,
    i32* lat,
    i32* alt)
{
    i32 longius;
    i32 brevius;
    i32 aptatum;

    si (   latitudo == ZEPHYRUM || altitudo == ZEPHYRUM
        || maxima   == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (latitudo <= maxima && altitudo <= maxima)
    {
        *lat = latitudo;
        *alt = altitudo;
        redde VERUM;
    }
    longius = latitudo >= altitudo ? latitudo : altitudo;
    brevius = latitudo >= altitudo ? altitudo : latitudo;
    /* brevius * maxima / longius, dimidium sursum; i64 ne superfluat */
    aptatum  = (i32)(((i64)brevius * (i64)maxima + (i64)(longius / II))
             / (i64)longius);
    si (aptatum < I)
    {
        aptatum = I;
    }
    *lat = latitudo >= altitudo ? maxima : aptatum;
    *alt = latitudo >= altitudo ? aptatum : maxima;
    redde VERUM;
}

vacuum
testa_facultates_tactus (
    Eventus* ev,
        s64  tempus)
{
    memset(ev, ZEPHYRUM, magnitudo(Eventus));
    ev->genus                          = EVENTUS_FACULTATES;
    ev->tempus                         = tempus;
    ev->datum.facultates.liberationes  = VERUM;
    ev->datum.facultates.super         = FALSUM;
    ev->datum.facultates.pressio       = FALSUM;
}

TestaQuadriPetitio
testa_quadrum_petere (
    constans TestaQuadriStatus* s)
{
    TestaQuadriPetitio p;
                   b32 statim;

    p.genus    = TESTA_QUADRUM_NULLUM;
    p.post_ms  = ZEPHYRUM;
    si (!s->fenestra || !s->vivus)
    {
        redde p;
    }
    statim = s->fumus_reliqua > ZEPHYRUM || s->debitum
          || (!s->pulsus && s->mora == ZEPHYRUM);
    si (statim)
    {
        si (!s->pendens_statim)
        {
            p.genus = TESTA_QUADRUM_STATIM;
        }
        redde p;
    }
    si (s->pendens_statim || s->pendens_post)
    {
        redde p;
    }
    p.genus    = TESTA_QUADRUM_POST;
    p.post_ms  = s->mora;
    si (s->pulsus && (s->mora == ZEPHYRUM || s->mora > MORA_PULSUS))
    {
        p.post_ms = MORA_PULSUS;
    }
    redde p;
}

vacuum
testa_pixela_transferre (
    constans TabulaPixelorum* t,
     constans TestaGeometria* g,
                         i32* destinatio,
                         i32  gradus,
                         i32  color_marginis)
{
             i32  latitudo;
             i32  altitudo;
             i32  y;
             i32  x;
             i32  k;
             i32  r;
             s32  ry;
             i32* linea;
    constans i32* fons;

    latitudo = t->latitudo < g->latitudo ? t->latitudo : g->latitudo;
    altitudo = t->altitudo < g->altitudo ? t->altitudo : g->altitudo;
    per (y = ZEPHYRUM; y < g->fenestra_altitudo; y++)
    {
        linea  = destinatio + y * gradus;
        ry     = (s32)y - (s32)g->margo_y;
        si (ry < ZEPHYRUM || ry >= (s32)(altitudo * g->scala))
        {
            per (x = ZEPHYRUM; x < g->fenestra_latitudo; x++)
            {
                linea[x] = color_marginis;
            }
            perge;
        }
        fons = t->pixela + ((i32)ry / g->scala) * t->latitudo;
        per (k = ZEPHYRUM; k < g->margo_x; k++)
        {
            linea[k] = color_marginis;
        }
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            per (r = ZEPHYRUM; r < g->scala; r++)
            {
                linea[k] = fons[x];
                k++;
            }
        }
        per (; k < g->fenestra_latitudo; k++)
        {
            linea[k] = color_marginis;
        }
    }
}
