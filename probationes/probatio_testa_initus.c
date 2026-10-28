/* probatio_testa_initus.c - partes purae testae Android
 * (testa-spec II.5-II.6, III.3): geometria, margo, tactus, digiti
 * duo, abruptio, facultates, decisio quadri, transfusio. In hospite et
 * in tabula (tools/android/probatio.sh testa_initus). */
#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "delineare_mandata.h"
#include "testa_initus.h"
#include "credo.h"
#include <string.h>

interior vacuum
_crudum (
    TestaTactusCrudus* c,
                  s32  actio,
                  i32  index_actionis,
                  s64  tempus)
{
    memset(c, ZEPHYRUM, magnitudo(TestaTactusCrudus));
    c->actio           = actio;
    c->index_actionis  = index_actionis;
    c->tempus          = tempus;
}

/* indicatorem i in crudo ponere */
interior vacuum
_indicator (
    TestaTactusCrudus* c,
                  i32  i,
                  s32  id,
                  s32  instrumentum,
                  f32  x,
                  f32  y)
{
    c->ids[i]          = id;
    c->instrumenta[i]  = instrumentum;
    c->x[i]            = x;
    c->y[i]            = y;
    si (c->numerus < i + I)
    {
        c->numerus = i + I;
    }
}

interior vacuum
_sectio_geometria (
    CredoContextus* c)
{
     TestaGeometria  g;
    TabulaPixelorum* t;

    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    CREDO_AEQUALIS_I32(g.latitudo, DXII);
    CREDO_AEQUALIS_I32(g.altitudo, CCC);
    CREDO_AEQUALIS_I32(g.margo_x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(g.margo_y, ZEPHYRUM);

    CREDO_GRADUS("congruit cum tabula_pixelorum_ad_fenestram");
    t         = tabula_pixelorum_creare_nuda(c->piscina, X, X);
    t->scala  = 2.0f;
    CREDO_VERUM(tabula_pixelorum_ad_fenestram(t, c->piscina, MXXIV,
        DC));
    CREDO_AEQUALIS_I32(t->latitudo, g.latitudo);
    CREDO_AEQUALIS_I32(t->altitudo, g.altitudo);

    CREDO_GRADUS("residuum II sub scala III: margo I centratus");
    CREDO_VERUM(testa_geometriam_computare(MXXV, DCII, III, &g));
    CREDO_AEQUALIS_I32(g.latitudo, CCCXLI);
    CREDO_AEQUALIS_I32(g.altitudo, CC);
    CREDO_AEQUALIS_I32(g.margo_x, I);
    CREDO_AEQUALIS_I32(g.margo_y, I);

    CREDO_GRADUS("pravae: scala ZEPHYRUM, fenestra minor scala");
    CREDO_FALSUM(testa_geometriam_computare(MXXIV, DC, ZEPHYRUM, &g));
    CREDO_FALSUM(testa_geometriam_computare(I, I, II, &g));
}

/* Review Focus 1: tactus in margine extra [0, latitudo) */
interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    TestaGeometria g;
               s32 x;
               s32 y;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXV, DCII, III, &g));
    testa_punctum_mappare(&g, 0.0f, 0.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, -I);
    CREDO_AEQUALIS_S32(y, -I);
    testa_punctum_mappare(&g, 1.0f, 1.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(y, ZEPHYRUM);
    testa_punctum_mappare(&g, 1023.9f, 600.5f, &x, &y);
    CREDO_AEQUALIS_S32(x, CCCXL);
    CREDO_AEQUALIS_S32(y, CXCIX);
    CREDO_GRADUS("margo dexter: x == latitudo (extra)");
    testa_punctum_mappare(&g, 1024.0f, 601.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, CCCXLI);
    CREDO_AEQUALIS_S32(y, CC);
    CREDO_GRADUS("negativum pavimento, non truncatione");
    testa_punctum_mappare(&g, -2.5f, -0.5f, &x, &y);
    CREDO_AEQUALIS_S32(x, -II);
    CREDO_AEQUALIS_S32(y, -I);
}

interior vacuum
_sectio_tactus (
    CredoContextus* c)
{
       TestaGeometria g;
          TestaTactus s;
    TestaTactusCrudus r;
              Eventus e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));

    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 100.0f,
        50.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_DEPRESSUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, L);
    CREDO_AEQUALIS_S32(e.datum.mus.y, XXV);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.botton, (i32)MUS_SINISTER);
    CREDO_AEQUALIS_S32(e.datum.mus.indicator, VII);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_TACTUS);
    CREDO_AEQUALIS_S32(e.datum.mus.pressio, EVENTUS_PRESSIO_IGNOTA);
    CREDO_AEQUALIS_S64(e.tempus, (s64)M);

    CREDO_GRADUS("motus cum historia: exempla II, botton tentus");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)(M + XX));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 120.0f,
        60.0f);
    r.numerus_historiae               = II;
    r.historia_x[ZEPHYRUM][ZEPHYRUM]  = 104.0f;
    r.historia_y[ZEPHYRUM][ZEPHYRUM]  = 52.0f;
    r.historia_tempus[ZEPHYRUM]       = (s64)(M + V);
    r.historia_x[ZEPHYRUM][I]         = 110.0f;
    r.historia_y[ZEPHYRUM][I]         = 56.0f;
    r.historia_tempus[I]              = (s64)(M + XII);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_MOTUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, LX);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.botton, (i32)MUS_SINISTER);
    CREDO_AEQUALIS_I32(e.datum.mus.numerus_exemplorum, II);
    CREDO_NON_NIHIL(e.datum.mus.exempla);
    CREDO_AEQUALIS_S32(e.datum.mus.exempla[ZEPHYRUM].x, LII);
    CREDO_AEQUALIS_S32(e.datum.mus.exempla[I].y, XXVIII);
    CREDO_AEQUALIS_S64(e.datum.mus.exempla[I].tempus, (s64)(M + XII));

    CREDO_GRADUS("liberatio; motus post eam nihil");
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)(M + XL));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 120.0f,
        60.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)(M + L));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 130.0f,
        60.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("instrumenta: stilus -> STILUS, mus -> MUS");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_STILUS, 2.0f, 2.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_STILUS);
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_STILUS, 2.0f, 2.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, II, TESTA_INSTRUMENTUM_MUS, 2.0f, 2.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_MUS);
}

/* Review Focus 3: digitus secundus praetermittitur */
interior vacuum
_sectio_digiti_duo (
    CredoContextus* c)
{
       TestaGeometria g;
          TestaTactus s;
    TestaTactusCrudus r;
              Eventus e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("indicator secundus deprimitur: nihil");
    _crudum(&r, TESTA_TACTUS_INDICATOR_DEPRESSIO, I, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    _indicator(&r, I, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f, 300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("motus amborum: positio PRIMI (ordine permutato)");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f,
        300.0f);
    _indicator(&r, I, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS, 20.0f,
        30.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_S32(e.datum.mus.x, X);
    CREDO_AEQUALIS_S32(e.datum.mus.y, XV);

    CREDO_GRADUS("secundus liberatur: nihil");
    _crudum(&r, TESTA_TACTUS_INDICATOR_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f,
        300.0f);
    _indicator(&r, I, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS, 20.0f,
        30.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("primus liberatur dum alius manet: LIBERATUS");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_INDICATOR_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        12.0f, 10.0f);
    _indicator(&r, I, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f, 300.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, VI);

    CREDO_GRADUS("reliquus movetur, deinde liberatur: nihil");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 510.0f,
        300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 510.0f,
        300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("omnibus sublatis tactus novus ducit");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, II, TESTA_INSTRUMENTUM_DIGITUS, 40.0f,
        40.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_S32(e.datum.mus.indicator, II);
}

/* Review Focus 2: abruptio (tabula clausa media linea) */
interior vacuum
_sectio_abruptio (
    CredoContextus* c)
{
       TestaGeometria g;
          TestaTactus s;
    TestaTactusCrudus r;
              Eventus e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, III, TESTA_INSTRUMENTUM_DIGITUS, 10.0f,
        10.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_ABRUPTIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, III, TESTA_INSTRUMENTUM_DIGITUS, 14.0f,
        10.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    CREDO_GRADUS("abruptio iterata: nihil (botton non bis liberatur)");
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_FALSUM(s.activus);
}

interior vacuum
_sectio_facultates (
    CredoContextus* c)
{
    Eventus e;

    (vacuum)c;
    testa_facultates_tactus(&e, (s64)M);
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_FACULTATES);
    CREDO_FALSUM(e.datum.facultates.super);
    CREDO_FALSUM(e.datum.facultates.pressio);
    CREDO_AEQUALIS_S64(e.tempus, (s64)M);
}

interior TestaQuadriPetitio
_petere (
     b32 fenestra,
     b32 vivus,
     b32 pendens_statim,
     b32 pendens_post,
     b32 debitum,
    Mora mora,
     b32 pulsus,
     i32 fumus)
{
    TestaQuadriStatus s;

    s.fenestra        = fenestra;
    s.vivus           = vivus;
    s.pendens_statim  = pendens_statim;
    s.pendens_post    = pendens_post;
    s.debitum         = debitum;
    s.mora            = mora;
    s.pulsus          = pulsus;
    s.fumus_reliqua   = fumus;
    redde testa_quadrum_petere(&s);
}

interior vacuum
_sectio_quadrum (
    CredoContextus* c)
{
    TestaQuadriPetitio p;

    (vacuum)c;
    CREDO_GRADUS("sine fenestra aut in pausa: nullum");
    p = _petere(FALSUM, VERUM, FALSUM, FALSUM, VERUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    p = _petere(VERUM, FALSUM, FALSUM, FALSUM, VERUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("otiosa: post moram (quies 300)");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_POST);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)CCC);
    CREDO_GRADUS("ante quadrum primum (mora 0): statim");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, ZEPHYRUM, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
    CREDO_GRADUS("pendens post, debitum -> statim (Review Focus 4)");
    p = _petere(VERUM, VERUM, FALSUM, VERUM, VERUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
    CREDO_GRADUS("pendens statim: nihil iterum");
    p = _petere(VERUM, VERUM, VERUM, FALSUM, VERUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("pendens post, otiosa: nihil iterum");
    p = _petere(VERUM, VERUM, FALSUM, VERUM, FALSUM, CCC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("pulsus: XVI ad summum (mora 300 et 0)");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, VERUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_POST);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)XVI);
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, ZEPHYRUM, VERUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)XVI);
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, X, VERUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)X);
    CREDO_GRADUS("fumus: statim donec quadra restant");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, FALSUM, III);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
    CREDO_GRADUS("plan-5 T8: hora reactoris (CC) -> post CC");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CC, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_POST);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)CC);
    CREDO_GRADUS("plan-5 T8: nihil debitum (mora -1): nullum quadrum");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, -I, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, VERUM, -I, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
}

interior vacuum
_sectio_transferre (
    CredoContextus* c)
{
     TestaGeometria  g;
    TabulaPixelorum* t;
                i32  alveus[IX * VIII];
                i32  speratum[VIII];
                i32  y;
                i32  x;

    t = tabula_pixelorum_creare_nuda(c->piscina, II, II);
    t->pixela[ZEPHYRUM] = I;
    t->pixela[I] = II;
    t->pixela[II] = III;
    t->pixela[III] = IV;
    CREDO_VERUM(testa_geometriam_computare(VIII, VIII, III, &g));
    per (x = ZEPHYRUM; x < IX * VIII; x++)
    {
        alveus[x] = 0xABu;
    }
    testa_pixela_transferre(t, &g, alveus, IX, 0x77u);
    CREDO_GRADUS("lineae 0 et VII margo; columna VIII intacta");
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        CREDO_AEQUALIS_I32(alveus[y * IX + VIII], 0xABu);
        si (y == ZEPHYRUM || y == VII)
        {
            per (x = ZEPHYRUM; x < VIII; x++)
            {
                CREDO_AEQUALIS_I32(alveus[y * IX + x], 0x77u);
            }
            perge;
        }
        speratum[ZEPHYRUM]  = 0x77u;
        speratum[VII]       = 0x77u;
        per (x = I; x < VII; x++)
        {
            speratum[x] = (y < IV ? I : III) + (x < IV ? ZEPHYRUM : I);
        }
        per (x = ZEPHYRUM; x < VIII; x++)
        {
            CREDO_AEQUALIS_I32(alveus[y * IX + x], speratum[x]);
        }
    }
}

/* area contenti (T3: vectes systematis Fire - status [0,24], navalis
 * [552,600] - tactus suos devorant): geometria intra rectangulum */
interior vacuum
_sectio_rectangulum (
    CredoContextus* c)
{
     TestaGeometria  g;
    TabulaPixelorum* t;
                s32  x;
                s32  y;

    CREDO_GRADUS("Fire 7: contentum [0,24]-[1024,552], scala II");
    CREDO_VERUM(testa_geometriam_in_area(MXXIV, DC, ZEPHYRUM,
        XXIV, MXXIV, DLII, II, &g));
    CREDO_AEQUALIS_I32(g.fenestra_latitudo, MXXIV);
    CREDO_AEQUALIS_I32(g.fenestra_altitudo, DC);
    CREDO_AEQUALIS_I32(g.latitudo, DXII);
    CREDO_AEQUALIS_I32(g.altitudo, CCLXIV);
    CREDO_AEQUALIS_I32(g.margo_x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(g.margo_y, XXIV);

    CREDO_GRADUS("congruit cum ad_fenestram super contentum");
    t         = tabula_pixelorum_creare_nuda(c->piscina, X, X);
    t->scala  = 2.0f;
    CREDO_VERUM(tabula_pixelorum_ad_fenestram(t, c->piscina, MXXIV,
        DXXVIII));
    CREDO_AEQUALIS_I32(t->latitudo, g.latitudo);
    CREDO_AEQUALIS_I32(t->altitudo, g.altitudo);

    CREDO_GRADUS("tactus sub vecte status et in navali: extra");
    testa_punctum_mappare(&g, 10.0f, 30.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, V);
    CREDO_AEQUALIS_S32(y, III);
    testa_punctum_mappare(&g, 10.0f, 10.0f, &x, &y);
    CREDO_AEQUALIS_S32(y, -VII);
    testa_punctum_mappare(&g, 100.0f, 560.0f, &x, &y);
    CREDO_AEQUALIS_S32(y, CCLXVIII);

    CREDO_GRADUS("residuum impar: margo intra rectangulum");
    CREDO_VERUM(testa_geometriam_in_area(MXXIV, DC, V, XXIV,
        MXXIV, DLIII, II, &g));
    CREDO_AEQUALIS_I32(g.latitudo, DIX);
    CREDO_AEQUALIS_I32(g.altitudo, CCLXIV);
    CREDO_AEQUALIS_I32(g.margo_x, V);
    CREDO_AEQUALIS_I32(g.margo_y, XXIV);

    CREDO_GRADUS("rectangulum extra fenestram secatur; vacuum: FALSUM");
    CREDO_VERUM(testa_geometriam_in_area(MXXIV, DC, -X, -X,
        MM, MM, II, &g));
    CREDO_AEQUALIS_I32(g.latitudo, DXII);
    CREDO_AEQUALIS_I32(g.altitudo, CCC);
    CREDO_AEQUALIS_I32(g.margo_x, ZEPHYRUM);
    CREDO_FALSUM(testa_geometriam_in_area(MXXIV, DC, C, C, C, CC,
        II, &g));
    CREDO_FALSUM(testa_geometriam_in_area(MXXIV, DC, ZEPHYRUM,
        ZEPHYRUM, I, I, II, &g));
}

/* testa-ios T1: phases UITouch -> actiones machinae tactus (digitus
 * primus DEPRESSIO, ceteri INDICATOR_*; ultimus sublatus LIBERATIO) */
interior vacuum
_sectio_gradus_tactus (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_GRADUS("incipit: primus DEPRESSIO, secundus INDICATOR");
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_INCIPIT,
        ZEPHYRUM, I), (s32)TESTA_TACTUS_DEPRESSIO);
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_INCIPIT, I,
        II), (s32)TESTA_TACTUS_INDICATOR_DEPRESSIO);
    CREDO_GRADUS("movetur: MOTUS; quiescit: nihil");
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_MOVETUR, I, I),
        (s32)TESTA_TACTUS_MOTUS);
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_QUIESCIT, I,
        I), -I);
    CREDO_GRADUS("finit: ultimus LIBERATIO, alius restans INDICATOR");
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_FINIT, I,
        ZEPHYRUM), (s32)TESTA_TACTUS_LIBERATIO);
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_FINIT, II, I),
        (s32)TESTA_TACTUS_INDICATOR_LIBERATIO);
    CREDO_GRADUS("abrupta: ABRUPTIO semper (Review Focus 2)");
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_ABRUPTA, I,
        ZEPHYRUM), (s32)TESTA_TACTUS_ABRUPTIO);
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(TESTA_PHASIS_ABRUPTA, II,
        ZEPHYRUM), (s32)TESTA_TACTUS_ABRUPTIO);
    CREDO_GRADUS("phasis ignota: nihil");
    CREDO_AEQUALIS_S32(testa_actio_ex_phase(IX, I, I), -I);
}

/* C1b R4: photographia electa intra MXXIV (Review Focus 2: panorama
 * longissima, numquam ZEPHYRUM) */
interior vacuum
_sectio_mensurae (
    CredoContextus* c)
{
    i32 l;
    i32 a;

    (vacuum)c;
    CREDO_GRADUS("parva: immutata");
    CREDO_VERUM(testa_mensuras_aptare(DCCC, DC, M + XXIV, &l, &a));
    CREDO_AEQUALIS_I32(l, DCCC);
    CREDO_AEQUALIS_I32(a, DC);
    CREDO_GRADUS("photographia jacens et erecta");
    CREDO_VERUM(testa_mensuras_aptare(IV * M + XXXII, III * M + XXIV,
        M + XXIV, &l, &a));
    CREDO_AEQUALIS_I32(l, M + XXIV);
    CREDO_AEQUALIS_I32(a, DCCLXVIII);
    CREDO_VERUM(testa_mensuras_aptare(III * M + XXIV, IV * M + XXXII,
        M + XXIV, &l, &a));
    CREDO_AEQUALIS_I32(l, DCCLXVIII);
    CREDO_AEQUALIS_I32(a, M + XXIV);
    CREDO_GRADUS("panorama: proportio, minimum I");
    CREDO_VERUM(testa_mensuras_aptare(XVI * M, III * M, M + XXIV, &l,
        &a));
    CREDO_AEQUALIS_I32(l, M + XXIV);
    CREDO_AEQUALIS_I32(a, CXCII);
    CREDO_VERUM(testa_mensuras_aptare(XX * M, V, M + XXIV, &l, &a));
    CREDO_AEQUALIS_I32(l, M + XXIV);
    CREDO_AEQUALIS_I32(a, I);
    CREDO_GRADUS("rotundatio: dimidium sursum");
    CREDO_VERUM(testa_mensuras_aptare(M, CCCXXXIII, D, &l, &a));
    CREDO_AEQUALIS_I32(l, D);
    CREDO_AEQUALIS_I32(a, CLXVII);
    CREDO_GRADUS("ZEPHYRUM: FALSUM");
    CREDO_FALSUM(testa_mensuras_aptare(ZEPHYRUM, X, M + XXIV, &l, &a));
    CREDO_FALSUM(testa_mensuras_aptare(X, X, ZEPHYRUM, &l, &a));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "geometria", _sectio_geometria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tactus", _sectio_tactus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "digiti duo", _sectio_digiti_duo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abruptio", _sectio_abruptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "facultates", _sectio_facultates, NIHIL, NIHIL, NIHIL, NIHIL },
    { "decisio quadri", _sectio_quadrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transfusio", _sectio_transferre, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rectangulum contenti", _sectio_rectangulum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "phases UITouch", _sectio_gradus_tactus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "mensurae imaginis", _sectio_mensurae, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("testa_initus", SECTIONES);
}
