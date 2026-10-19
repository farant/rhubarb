/* probatio_pictor_imago.c - imago glutinata in stratum novum (L5)
 *
 * I: pictor_imaginem_aptare - "contain": lata (C x XX in CDLXVIII x
 * CDXL), alta, parva (sursum); centrata; indices palettae (0-15) aut
 * 0xFF; alpha < CXXVIII perspicuum. II: Cmd+V cum porta ficta -
 * glutinatio PENDENS (Franus 2026-10-13): campus "B", linea status A /
 * B / OK / X, tabula sine actione (ictus nihil scribit); A eligitur, OK
 * stratum novum supra currens, currens fit, pixela palettae in strato,
 * margo perspicuus; revocatio una stratum et imaginem tollit; reficere
 * reddit. II b: Esc abicit (acta intacta), Reditus confirmat. II c
 * (Franus 2026-10-13): tractus in tabula pendente RADIT - larva modum
 * alterum revelat, Optio-tractus reponit, acta intacta; magnitudo
 * penicilli ex quadrato lineae pendentis (etiam sub spongia); A electo
 * larva manet, OK per pixelum miscet. III:
 * reapertio - massa ex volumine, pixela eadem. IV: sine porta aut
 * imagine vacua nihil. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "xar.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include "imago.h"
#include "imago_typus.h"
#include "pictor_actiones.h"
#include "delineare.h"
#include "tabula_pixelorum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* tabula in applicatione sola ad cellulam (VI, VIII) */
#define ORIGO_X VI
#define ORIGO_Y VIII

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;


interior i32
pixelum (
    PictorDocumentum* doc,
                 s32  x,
                 s32  y)
{
    redde (i32)tabula_pixelorum_obtinere_pixelum(doc->tabula, (i32)x,
        (i32)y);
}

interior i32
palettae (
    s32 index)
{
    redde (i32)color_ad_pixelum(color_ex_palette((i32)index));
}

interior chorda
ephemera_legere (
    PictorApplicatio* app,
  constans character* titulus)
{
    chorda* a;

    a = insula_attributum(app->repo, titulus);
    redde a ? *a : chorda_ex_literis("", piscina);
}

/* datum acti 'ictus' ultimi in volumine */
interior chorda
actum_ultimum (
    Volumen* vol)
{
             Xar* acta;
    VolumenActum* a;
          chorda  ultimum;
             i32  i;

    ultimum  = chorda_ex_literis("", piscina);
    acta     = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        a = (VolumenActum*)xar_obtinere(acta, i);
        si (chorda_aequalis_literis(a->genus, "ictus"))
        {
            ultimum = a->datum;
        }
    }
    redde ultimum;
}

/* porta ficta: imago C x L, dimidium sinistrum rubrum, dextrum
 * caeruleum, quadrans infimus dexter perspicuus */
interior ImagoFructus
porta_ficta (
    vacuum* ctx,
   Piscina* p)
{
    ImagoFructus  f;
             i32  x;
             i32  y;
              i8* q;

    (vacuum)ctx;
    memset(&f, ZEPHYRUM, magnitudo(f));
    f.imago.latitudo = C;
    f.imago.altitudo = L;
    f.imago.pixela = (i8*)piscina_allocare(p,
        (memoriae_index)(C * L * IV));
    per (y = ZEPHYRUM; y < L; y++)
    {
        per (x = ZEPHYRUM; x < C; x++)
        {
            q            = f.imago.pixela + (y * C + x) * IV;
            q[ZEPHYRUM]  = x < L ? (i8)CCLV : ZEPHYRUM;
            q[I]         = x < L ? ZEPHYRUM : (i8)C;
            q[II]        = x < L ? ZEPHYRUM : (i8)CCLV;
            q[III]       = (x >= L && y >= XXV) ? ZEPHYRUM : (i8)CCLV;
        }
    }
    f.successus = VERUM;
    redde f;
}

interior ImagoFructus
porta_vacua (
    vacuum* ctx,
   Piscina* p)
{
    ImagoFructus f;

    (vacuum)ctx;
    (vacuum)p;
    memset(&f, ZEPHYRUM, magnitudo(f));
    redde f;
}

/* imago plena colore uno (RGBA) */
interior Imago
imago_plena (
    i32 lat,
    i32 alt,
     i8 alpha)
{
    Imago im;
      i32 i;

    im.latitudo = lat;
    im.altitudo = alt;
    im.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)(lat * alt * IV));
    per (i = ZEPHYRUM; i < lat * alt; i++)
    {
        im.pixela[i * IV]        = (i8)CC;
        im.pixela[i * IV + I]    = (i8)L;
        im.pixela[i * IV + II]   = (i8)L;
        im.pixela[i * IV + III]  = alpha;
    }
    redde im;
}

/* clavis simplex (Esc, Reditus) */
interior vacuum
clavem_premere (
    PictorApplicatio* app,
            clavis_t  clavis)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus               = M;
    e.datum.clavis.clavis  = clavis;
    e.datum.clavis.actio   = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

/* clavis litterae ('e' spongia) */
interior vacuum
clavem_runam_premere (
    PictorApplicatio* app,
           character  runa)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus               = M;
    e.datum.clavis.clavis  = (clavis_t)runa;
    e.datum.clavis.runa    = (s32)runa;
    e.datum.clavis.actio   = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

/* ictus in nodum lineae status (filius 'status', fines relativi) */
interior b32
nodum_premere (
    PictorApplicatio* app,
  constans character* id)
{
     Componens* s;
     Componens* q;
       Eventus  e;

    s = componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis("status", piscina));
    q = componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
    si (!s || !q)
    {
        imprimere("  nodus abest: %s\n", id);
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_MUS_DEPRESSUS;
    e.tempus  = M;
    e.datum.mus.x = s->fines.x + q->fines.x + q->fines.latitudo
        / II;
    e.datum.mus.y = s->fines.y + q->fines.y + q->fines.altitudo
        / II;
    e.datum.mus.botton = MUS_SINISTER;
    dispensator_tractare(app->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
    redde VERUM;
}

/* ictus (pressio + solutio) in tabula ad (x, y) tabulae */
interior vacuum
tabulam_premere (
    PictorApplicatio* app,
                 s32  x,
                 s32  y)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.tempus            = M;
    e.datum.mus.x       = ORIGO_X + x;
    e.datum.mus.y       = ORIGO_Y + y;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(app->d, &e);
    e.genus   = EVENTUS_MUS_LIBERATUS;
    e.tempus  = M + X;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

/* ictus in optionem palettae apertae (filius 'palette', fines
 * relativi) */
interior b32
optionem_premere (
    PictorApplicatio* app,
  constans character* id)
{
     Componens* p;
     Componens* q;
       Eventus  e;

    p = componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis("palette", piscina));
    q = componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
    si (!p || !q)
    {
        imprimere("  optio abest: %s\n", id);
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus   = EVENTUS_MUS_DEPRESSUS;
    e.tempus  = M;
    e.datum.mus.x = p->fines.x + q->fines.x + q->fines.latitudo
        / II;
    e.datum.mus.y = p->fines.y + q->fines.y + q->fines.altitudo
        / II;
    e.datum.mus.botton = MUS_SINISTER;
    dispensator_tractare(app->d, &e);
    e.genus = EVENTUS_MUS_LIBERATUS;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
    redde VERUM;
}

/* tractus in tabula: pressio (x0, y0), motus et solutio (x1, y1)
 * coordinatis tabulae; modificantes in eventu quoque */
interior vacuum
tabulam_trahere (
    PictorApplicatio* app,
                 s32  x0,
                 s32  y0,
                 s32  x1,
                 s32  y1,
                 i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = EVENTUS_MUS_DEPRESSUS;
    e.tempus                  = M;
    e.datum.mus.x             = ORIGO_X + x0;
    e.datum.mus.y             = ORIGO_Y + y0;
    e.datum.mus.botton        = MUS_SINISTER;
    e.datum.mus.modificantes  = modificantes;
    dispensator_tractare(app->d, &e);
    e.genus        = EVENTUS_MUS_MOTUS;
    e.tempus       = M + V;
    e.datum.mus.x  = ORIGO_X + x1;
    e.datum.mus.y  = ORIGO_Y + y1;
    dispensator_tractare(app->d, &e);
    e.genus   = EVENTUS_MUS_LIBERATUS;
    e.tempus  = M + X;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
}

/* pixela larvae signata */
interior i32
larva_numerus (
    constans PictorDocumentum* doc)
{
    i32 n;
    i32 k;

    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < doc->glutinatio.latitudo
        * doc->glutinatio.altitudo; k++)
    {
        si (doc->glutinatio.larva[k])
        {
            n++;
        }
    }
    redde n;
}

interior Componens*
nodus_arboris (
    PictorApplicatio* app,
  constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(app->d),
        chorda_ex_literis(id, piscina));
}

interior b32
cmd_v (
    PictorApplicatio* app)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = M;
    e.datum.clavis.clavis        = (clavis_t)'v';
    e.datum.clavis.runa          = 'v';
    e.datum.clavis.modificantes  = MOD_SUPER;
    e.datum.clavis.actio         = EVENTUS_ACTIO_PRESSA;
    dispensator_tractare(app->d, &e);
    dispensator_recomponere(app->d);
    redde VERUM;
}

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    (vacuum)c;
    {
        Imago  im;
           i8* ix;
          s32  x;
          s32  y;
          i32  w;
          i32  h;
          i32  i;
          i32  mala;

        /* lata C x XX in CDLXVIII x CDXL: latitudo plena, altitudo
         * XX x CDLXVIII / C = XCIII, centrata */
        im = imago_plena(C, XX, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_NON_NIHIL(ix);
        CREDO_AEQUALIS_I32(w, CDLXVIII);
        CREDO_AEQUALIS_I32(h, XCIII);
        CREDO_AEQUALIS_S32(x, ZEPHYRUM);
        CREDO_AEQUALIS_S32(y, (CDXL - XCIII) / II);
        mala = ZEPHYRUM;
        per (i = ZEPHYRUM; ix && i < w * h; i++)
        {
            si ((i32)ix[i] >= XVI)
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        /* alta XX x C: altitudo plena */
        im = imago_plena(XX, C, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_AEQUALIS_I32(h, CDXL);
        CREDO_AEQUALIS_I32(w, LXXXVIII);
        CREDO_AEQUALIS_S32(y, ZEPHYRUM);
        CREDO_AEQUALIS_S32(x, (CDLXVIII - LXXXVIII) / II);
        /* parva X x X: sursum ad CDXL */
        im = imago_plena(X, X, (i8)CCLV);
        ix = pictor_imaginem_aptare(&im, CDLXVIII, CDXL, piscina, &x,
            &y,
            &w, &h);
        CREDO_AEQUALIS_I32(w, CDXL);
        CREDO_AEQUALIS_I32(h, CDXL);
        /* alpha nullum: omnia perspicua */
        im = imago_plena(X, X, ZEPHYRUM);
        ix = pictor_imaginem_aptare(&im, X, X, piscina, &x, &y, &w, &h);
        mala = ZEPHYRUM;
        per (i = ZEPHYRUM; ix && i < w * h; i++)
        {
            si (ix[i] != (i8)CCLV)
            {
                mala++;
            }
        }
        CREDO_AEQUALIS_I32(mala, ZEPHYRUM);
        /* fons vacua */
        im.latitudo = ZEPHYRUM;
        CREDO_VERUM(pictor_imaginem_aptare(&im, X, X, piscina, &x, &y,
            &w, &h) == NIHIL);
    }
}

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
              Volumen* vol;
     PictorApplicatio  app;
     PictorDocumentum* doc;
                  i32  fundus;

    (vacuum)c;
    fundus = (i32)color_ad_pixelum(thema_color(COLOR_BACKGROUND));

    vol = volumen_temporarium(piscina, "probatio_pictor_imago");
    CREDO_VERUM(pictor_applicatio_aedificare(&app, piscina, intern, vol,
        CDLXXX, CDLXXX));
    doc = app.doc;

    CREDO_VERUM(cmd_v(&app));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
    app.montatio.actiones_ctx.imago_capere = porta_vacua;
    CREDO_VERUM(cmd_v(&app));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);

    CREDO_GRADUS("II: Cmd+V - stratum novum");
    {
        s32 lat;
        s32 alt;
        s32 h;
        s32 y0;
        i32 rubri;
        i32 perspicui;
        i32 x;
        i32 y;
        i32 p;
        i32 r;

        s64 finis;

        app.montatio.actiones_ctx.imago_capere = porta_ficta;
        finis = pictor_documentum_finis(doc);
        CREDO_VERUM(cmd_v(&app));
        /* pendens: nullum stratum, nihil in actis, campus B */
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
        CREDO_VERUM(pictor_documentum_finis(doc) == finis);
        CREDO_VERUM(doc->glutinatio.pendens);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "glutinatio"), "B");
        /* linea status: A / B / OK / X, quadrata absunt; tabula radit
         * (glutinatio.agere), praevisio in titulo */
        CREDO_NON_NIHIL(nodus_arboris(&app, "glutinatio.A"));
        CREDO_NON_NIHIL(nodus_arboris(&app, "glutinatio.B"));
        CREDO_NON_NIHIL(nodus_arboris(&app, "glutinatio.confirmare"));
        CREDO_NON_NIHIL(nodus_arboris(&app, "glutinatio.abicere"));
        CREDO_VERUM(nodus_arboris(&app, "quadratum.instrumentum")
            == NIHIL);
        CREDO_VERUM(nodus_arboris(&app, "quadratum.strata") == NIHIL);
        CREDO_VERUM(nodus_arboris(&app, "tabula")
            && chorda_aequalis_literis(nodus_arboris(&app,
            "tabula")->actio, "glutinatio.agere"));
        CREDO_VERUM(nodus_arboris(&app, "tabula")
            && chorda_aequalis_literis(nodus_arboris(&app,
            "tabula")->titulus,
                "glutinatio:B"));
        /* ictus in tabula dum pendet: acta intacta (radit solum),
         * Optio-ictus reponit */
        tabulam_premere(&app, C, C);
        CREDO_VERUM(pictor_documentum_finis(doc) == finis);
        tabulam_trahere(&app, C, C, C, C, MOD_ALT);
        CREDO_AEQUALIS_I32(larva_numerus(doc), ZEPHYRUM);
        /* A eligitur */
        CREDO_VERUM(nodum_premere(&app, "glutinatio.A"));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "glutinatio"), "A");
        CREDO_VERUM(nodus_arboris(&app, "tabula")
            && chorda_aequalis_literis(nodus_arboris(&app,
            "tabula")->titulus,
                "glutinatio:A"));
        /* OK: stratum novum */
        CREDO_VERUM(nodum_premere(&app, "glutinatio.confirmare"));
        CREDO_FALSUM(doc->glutinatio.pendens);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "glutinatio"), "");
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.instrumentum"));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "stratum_activum"), "2");
        CREDO_VERUM(chorda_continet(actum_ultimum(vol),
            chorda_ex_literis(
            "<imago stratum=\"2\"", piscina)));
        /* C x L in tabula: latitudo plena, centrata verticaliter */
        lat        = (s32)doc->latitudo;
        alt        = (s32)doc->altitudo;
        h          = L * lat / C;
        y0         = (alt - h) / II;
        r          = palettae(VI) ;
        rubri      = ZEPHYRUM;
        perspicui  = ZEPHYRUM;
        per (y = ZEPHYRUM; y < (i32)alt; y++)
        {
            per (x = ZEPHYRUM; x < (i32)lat; x++)
            {
                p = (i32)tabula_pixelorum_obtinere_pixelum(
                    pictor_documentum_stratum(doc, I)->pixela, x, y);
                si ((p >> XXIV) == ZEPHYRUM)
                {
                    perspicui++;
                }
                alioquin si (x < (i32)(lat / II))
                {
                    rubri++;
                }
            }
        }
        (vacuum)r;
        /* margines supra et infra + quadrans perspicuus */
        CREDO_VERUM(perspicui > (i32)(lat * y0 * II));
        CREDO_VERUM(rubri > (i32)(lat / II * h / II));
        /* margo summus: fundus in composito */
        CREDO_AEQUALIS_I32(pixelum(doc, X, I), fundus);
        CREDO_VERUM(pixelum(doc, X, y0 + h / II) != fundus);
        /* revocatio una: stratum et imago */
        CREDO_VERUM(pictor_documentum_revocare(doc));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc), I);
        CREDO_AEQUALIS_I32(pixelum(doc, X, y0 + h / II), fundus);
        CREDO_VERUM(pictor_documentum_reficere(doc));
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        CREDO_VERUM(pixelum(doc, X, y0 + h / II) != fundus);
    }

    CREDO_GRADUS("II b: Esc abicit, Reditus confirmat");
    {
        s64 finis;

        finis = pictor_documentum_finis(doc);
        CREDO_VERUM(cmd_v(&app));
        CREDO_VERUM(doc->glutinatio.pendens);
        clavem_premere(&app, CLAVIS_EFFUGIUM);
        CREDO_FALSUM(doc->glutinatio.pendens);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "glutinatio"), "");
        CREDO_VERUM(pictor_documentum_finis(doc) == finis);
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        /* X quoque abicit */
        CREDO_VERUM(cmd_v(&app));
        CREDO_VERUM(nodum_premere(&app, "glutinatio.abicere"));
        CREDO_FALSUM(doc->glutinatio.pendens);
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            II);
        /* Reditus confirmat modum B (ordinarium) */
        CREDO_VERUM(cmd_v(&app));
        clavem_premere(&app, CLAVIS_REDITUS);
        CREDO_FALSUM(doc->glutinatio.pendens);
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            III);
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "stratum_activum"), "3");
    }

    CREDO_GRADUS("II c: radere - alterum subter, Optio reponit");
    {
          PictorGlutinatio  g;
                       s64  finis;
                       s32  y;
                       s32  x0;
                       s32  x1;
                       i32  k;
                       i32  j;
                       i32  diversum;
                       i32  revelata;
                       i32  strata;
                       s32  activum;
                       i32  index_novi;
    constans PictorStratum* novum;

        finis   = pictor_documentum_finis(doc);
        strata  = pictor_documentum_numerus_stratorum(doc);
        /* instrumentum spongia: radere tamen magnitudine penicilli */
        clavem_runam_premere(&app, 'e');
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "instrumentum"), "spongia");
        CREDO_VERUM(cmd_v(&app));
        CREDO_VERUM(doc->glutinatio.pendens);
        /* linea in dimidio rubro (opaco), magnitudo penicilli I */
        y   = doc->glutinatio.y + (s32)doc->glutinatio.altitudo / IV;
        x0  = doc->glutinatio.x + XX;
        x1  = x0 + CC;
        tabulam_trahere(&app, x0, y, x1, y, ZEPHYRUM);
        CREDO_VERUM(pictor_documentum_finis(doc) == finis);
        revelata = larva_numerus(doc);
        CREDO_AEQUALIS_I32(revelata, CCI);
        k = (i32)(y - doc->glutinatio.y) * doc->glutinatio.latitudo
            + (i32)(x0 - doc->glutinatio.x);
        CREDO_AEQUALIS_I32((i32)doc->glutinatio.larva[k], I);
        /* Optio-tractus dimidium reponit */
        tabulam_trahere(&app, x0, y, x0 + C - I, y, MOD_ALT);
        CREDO_AEQUALIS_I32(larva_numerus(doc), CI);
        CREDO_AEQUALIS_I32((i32)doc->glutinatio.larva[k], ZEPHYRUM);
        /* magnitudo in linea pendente: quadratum penicilli (etiam si
         * instrumentum spongia), palette, optio VIII */
        CREDO_NON_NIHIL(nodus_arboris(&app, "quadratum.magnitudo"));
        CREDO_VERUM(nodus_arboris(&app, "quadratum.magnitudo")
            && chorda_aequalis_literis(nodus_arboris(&app,
            "quadratum.magnitudo")->titulus, "magnitudo:1"));
        CREDO_VERUM(nodum_premere(&app, "quadratum.magnitudo"));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "magnitudo");
        CREDO_VERUM(optionem_premere(&app, "optio.magnitudo.8"));
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "magnitudo_penicilli"), "8");
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app, "palette"),
            "");
        CREDO_CHORDA_AEQUALIS_LITERIS(ephemera_legere(&app,
            "instrumentum"), "spongia");
        CREDO_VERUM(doc->glutinatio.pendens);
        /* tractus VIII latus: fascia, non linea */
        tabulam_trahere(&app, x0, y + XX, x1, y + XX, ZEPHYRUM);
        revelata = larva_numerus(doc) - CI;
        CREDO_VERUM(revelata > CC * VII && revelata < CCXX * VIII);
        revelata = larva_numerus(doc);
        /* A: larva manet */
        CREDO_VERUM(nodum_premere(&app, "glutinatio.A"));
        CREDO_AEQUALIS_I32(larva_numerus(doc), revelata);
        CREDO_VERUM(pictor_documentum_finis(doc) == finis);
        /* OK: per pixelum - larva ? B : A */
        g = doc->glutinatio;
        CREDO_VERUM(nodum_premere(&app, "glutinatio.confirmare"));
        CREDO_FALSUM(doc->glutinatio.pendens);
        CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(doc),
            strata + I);
        /* stratum novum per id (stratum_activum) - ordo post revocationem
         * et refectionem in II non est ordo id */
        activum = ZEPHYRUM;
        CREDO_VERUM(chorda_ut_s32(ephemera_legere(&app,
            "stratum_activum"), &activum));
        novum = NIHIL;
        per (index_novi = ZEPHYRUM; index_novi < strata
            + I; index_novi++)
        {
            si (pictor_documentum_stratum(doc, index_novi)->id
                == activum)
            {
                novum = pictor_documentum_stratum(doc, index_novi);
            }
        }
        CREDO_NON_NIHIL(novum);
        diversum = ZEPHYRUM;
        per (j = ZEPHYRUM; novum && j < g.latitudo * g.altitudo; j++)
        {
            i8 index = g.larva[j]
                ? g.indices[PICTOR_GLUTINATIO_EXEMPLARIA][j]
                : g.indices[PICTOR_GLUTINATIO_ATKINSON][j];
            i32 p =
                (i32)tabula_pixelorum_obtinere_pixelum(novum->pixela,
                (i32)(g.x + (s32)(j % g.latitudo)),
                (i32)(g.y + (s32)(j / g.latitudo)));
            si (p != (index >= XVI ? ZEPHYRUM : palettae((s32)index)))
            {
                diversum++;
            }
        }
        CREDO_AEQUALIS_I32(diversum, ZEPHYRUM);
    }

    CREDO_GRADUS("III: reapertio");
    {
        PictorDocumentum* alter;

        alter = pictor_documentum_aperire(piscina, intern, vol, "");
        CREDO_NON_NIHIL(alter);
        CREDO_VERUM(alter && memcmp(alter->tabula->pixela,
            doc->tabula->pixela, (size_t)(doc->latitudo * doc->altitudo)
            * magnitudo(i32)) == ZEPHYRUM);
        CREDO_VERUM(alter && pictor_documentum_verificare(alter));
    }
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I: aptare (contain)", _sectio_prima, _parare, NIHIL, NIHIL,
        NIHIL },
    { "IV: sine porta, porta vacua - nihil", _sectio_secunda, _parare,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("pictor_imago", SECTIONES);
}
