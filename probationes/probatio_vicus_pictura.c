/* probatio_vicus_pictura.c - $pictor-next / $pictor-prev (S3e-3)
 *
 * Compositio VERA (vicus_applicatio). Bibliotheca: "2_dextrum_pictor"
 * (tabula 2, montata), "x1" (C x LX), "x2" (LX x XL). In tabula 3
 * (scriba | scriba) pagina: "$pictor-next $pictor-prev". Franus: nulla
 * regula praeteritionis - documentum unum per picturam, laterum
 * plurium tutum. I: next sine pictore: latus novum '3_dextrum_pictor'
 * picturam primam ostendit ("2_..." - documento EODEM ac tabula 2);
 * ictus in eo in documento tabulae 2 apparet. II-IV: next - "3_...",
 * x1, x2, circulo "2_...". V: prev circulo - x2. VI: dispositio
 * argumentum servat; reapertura x2 ostendit. VII: argumenta
 * recusantur. VIII: memoria communis - tabula 2 x1 ostendit documento
 * eodem. IX: nulla alia pictura - nuntius. X (S3f): $pictor-new -
 * pictura_1, pictura_2 vacuae magnitudine tabulae novae, in
 * bibliotheca, next eas visitat; argumenta recusantur; tabula sine
 * pictore: latus novum picturam novam ostendit. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ictus UNUS in cellulam paginae sinistrae */
interior vacuum
ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* ictus, pulsus, recompositio */
interior vacuum
iussum (
    VicusApplicatio* app,
         ManusLudus* m,
                s32  linea,
                s32  columna)
{
    ictus(m, linea, columna);
    (vacuum)vicus_pulsare(app->vicus);
    dispensator_recomponere(app->d);
}

interior VicusLatus*
frons (
    Vicus* v)
{
    redde vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
}

interior PictorDocumentum*
pictura_frontis (
    Vicus* v)
{
    VicusLatus* d;

    d = frons(v);
    si (   !d || !d->montata || !chorda_aequalis_literis(d->genus,
        "pictor"))
    {
        redde NIHIL;
    }
    redde ((PictorMontatio*)d->montatio)->doc;
}

interior b32
argumentum_est (
                  Vicus* v,
     constans character* argumentum)
{
    redde frons(v) && chorda_aequalis_literis(frons(v)->argumentum,
        argumentum);
}

interior b32
nuntius_est (
                  Vicus* v,
                Piscina* piscina,
     constans character* textus)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, "nuntius");
    redde a && chorda_aequalis_literis(*a, textus);
}

interior b32
index_continet (
                Volumen* vol,
                Piscina* piscina,
     constans character* textus)
{
    chorda c;
       b32 inventum;

    c = volumen_plagulam_promere(vol, chorda_ex_literis("vicus/latera",
        piscina), piscina, &inventum);
    redde inventum && chorda_continet(c, chorda_ex_literis(textus,
        piscina));
}

/* pictura in bibliotheca voluminis? */
interior b32
in_bibliotheca (
                Volumen* vol,
                Piscina* piscina,
     constans character* pictura)
{
    Xar* l;
    i32  i;

    l = pictor_documenta_enumerare(vol, piscina);
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis_literis(*(chorda*)xar_obtinere(l, i),
            pictura))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* linea scribitur in pagina sinistra */
interior vacuum
scribere (
             ManusLudus* m,
                    s32  linea,
     constans character* textus)
{
    ictus(m, linea, ZEPHYRUM);
    (vacuum)manus_ludus_scribere(m, "i");
    (vacuum)manus_ludus_scribere(m, textus);
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
}

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
             Volumen* vol;
             Volumen* vol_solus;
     VicusApplicatio  app;
     VicusApplicatio  app_altera;
     VicusApplicatio  app_sola;
               Vicus* v;
          ManusLudus* m;
    PictorDocumentum* x1;
          VicusLatus* latus_pictoris;
             Piscina* piscina;
 InternamentumChorda* intern;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_pictura");
    /* nulla assertio ante: sectio VACUA, ergo non transit */
    si (   !pictor_documentum_creare(piscina, intern, vol, "x1", C, LX,
               LXIV)
        || !pictor_documentum_creare(piscina, intern, vol, "x2", LX, XL,
               LXIV))
    {
        redde;
    }
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    scribere(m, II, "$pictor-next $pictor-prev");
    scribere(m, III, "$pictor-next(a)");
    CREDO_VERUM(chorda_aequalis_literis(frons(v)->genus, "scriba"));

    CREDO_GRADUS("I: next sine pictore - '2_...' communis");
    {
        PictorDocumentum* documentum_tabulae;
                     i32  vivi;

        CREDO_VERUM(vicus_activam_ponere(v, "2"));
        documentum_tabulae = pictura_frontis(v);
        CREDO_VERUM(vicus_activam_ponere(v, "3"));
        dispensator_recomponere(app.d);
        iussum(&app, m, II, III);
        CREDO_VERUM(chorda_aequalis_literis(frons(v)->id,
            "3_dextrum_pictor"));
        CREDO_VERUM(argumentum_est(v, "2_dextrum_pictor"));
        CREDO_NON_NIHIL(documentum_tabulae);
        CREDO_VERUM(pictura_frontis(v) == documentum_tabulae);
        latus_pictoris = frons(v);
        /* ictus in latere dextro (primus focat, secundus pingit):
         * tabula ad (CCXL + VI, linea + VIII) */
        vivi = ZEPHYRUM;
        si (documentum_tabulae)
        {
            vivi =
                pictor_documentum_numerus_vivorum(documentum_tabulae);
        }
        (vacuum)manus_ludus_premere_ad(m, CCXL + VI + X,
            VICUS_ALTITUDO_TABULARUM + VIII + X);
        (vacuum)manus_ludus_premere_ad(m, CCXL + VI + XX,
            VICUS_ALTITUDO_TABULARUM + VIII + X);
        CREDO_VERUM(documentum_tabulae
            && pictor_documentum_numerus_vivorum(
            documentum_tabulae) > vivi);
    }

    CREDO_GRADUS("II-IV: next - '3_...', x1, x2, circulo");
    iussum(&app, m, II, III);
    CREDO_VERUM(frons(v) == latus_pictoris);
    CREDO_VERUM(argumentum_est(v, "3_dextrum_pictor"));
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "x1"));
    x1 = pictura_frontis(v);
    CREDO_NON_NIHIL(x1);
    si (!x1)
    {
        redde;
    }
    CREDO_VERUM(x1->latitudo == C && x1->altitudo == LX);
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "x2"));
    CREDO_VERUM(pictura_frontis(v) && pictura_frontis(v)->latitudo == LX
        && pictura_frontis(v)->altitudo == XL);
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "2_dextrum_pictor"));

    CREDO_GRADUS("V: prev circulo - x2");
    iussum(&app, m, II, XV);
    CREDO_VERUM(argumentum_est(v, "x2"));
    CREDO_VERUM(frons(v) == latus_pictoris);

    CREDO_GRADUS("VI: dispositio et reapertura");
    CREDO_VERUM(index_continet(vol, piscina,
        "id=\"3_dextrum_pictor\" argumentum=\"x2\""));
    CREDO_VERUM(vicus_applicatio_aedificare(&app_altera, piscina,
        intern, vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    CREDO_VERUM(vicus_activam_ponere(app_altera.vicus, "3"));
    CREDO_VERUM(argumentum_est(app_altera.vicus, "x2"));
    CREDO_VERUM(pictura_frontis(app_altera.vicus)
        && pictura_frontis(app_altera.vicus)->latitudo == LX);

    CREDO_GRADUS("VII: argumenta recusantur");
    ictus(m, III, III);
    CREDO_VERUM(nuntius_est(v, piscina,
        "pictor-next: nulla argumenta"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(argumentum_est(v, "x2"));

    CREDO_GRADUS("VIII: memoria communis - tabula 2");
    CREDO_VERUM(vicus_activam_ponere(v, "2"));
    dispensator_recomponere(app.d);
    scribere(m, II, "$pictor-next $pictor-prev");
    /* 2_... -> 3_dextrum_pictor -> x1 */
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "3_dextrum_pictor"));
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "x1"));
    CREDO_VERUM(pictura_frontis(v) == x1);

    CREDO_GRADUS("IX: nulla alia pictura");
    vol_solus = volumen_temporarium(piscina,
        "probatio_vicus_pictura_s");
    CREDO_VERUM(vicus_applicatio_aedificare(&app_sola, piscina, intern,
        vol_solus, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    m = manus_ludus_creare(piscina, app_sola.d);
    CREDO_VERUM(vicus_activam_ponere(app_sola.vicus, "2"));
    dispensator_recomponere(app_sola.d);
    scribere(m, II, "$pictor-next");
    iussum(&app_sola, m, II, III);
    CREDO_VERUM(nuntius_est(app_sola.vicus, piscina,
        "pictor-next: nulla alia pictura"));
    CREDO_VERUM(argumentum_est(app_sola.vicus, ""));

    CREDO_GRADUS("X: $pictor-new");
    {
          ManusLudus* mn;
    PictorDocumentum* propria;
    PictorDocumentum* nova;

        v   = app.vicus;
        mn  = manus_ludus_creare(piscina, app.d);
        CREDO_VERUM(vicus_activam_ponere(v, "3"));
        dispensator_recomponere(app.d);
        /* pagina communis tabularum: lineae novae (linea II in VIII
         * iterum scripta est) */
        scribere(mn, V, "$pictor-new $pictor-new(x)");
        scribere(mn, VI, "$pictor-next $pictor-prev");
        /* magnitudo tabulae novae = pictura propria lateris 3 */
        propria = pictor_documentum_aperire(piscina, intern, vol,
            "3_dextrum_pictor");
        CREDO_NON_NIHIL(propria);
        iussum(&app, mn, V, III);
        CREDO_VERUM(frons(v) == latus_pictoris);
        CREDO_VERUM(argumentum_est(v, "pictura_1"));
        nova = pictura_frontis(v);
        CREDO_VERUM(   nova && propria
                    && nova->latitudo == propria->latitudo
                    && nova->altitudo == propria->altitudo);
        CREDO_VERUM(nova && pictor_documentum_numerus_vivorum(nova)
            == ZEPHYRUM);
        CREDO_VERUM(in_bibliotheca(vol, piscina, "pictura_1"));
        iussum(&app, mn, V, III);
        CREDO_VERUM(argumentum_est(v, "pictura_2"));
        /* bibliotheca: 2_ 3_ pictura_1 pictura_2 x1 x2 */
        iussum(&app, mn, VI, III);
        CREDO_VERUM(argumentum_est(v, "x1"));
        iussum(&app, mn, VI, XV);
        CREDO_VERUM(argumentum_est(v, "pictura_2"));
        ictus(mn, V, XV);
        CREDO_VERUM(nuntius_est(v, piscina,
            "pictor-new: nulla argumenta"));
        (vacuum)vicus_pulsare(v);
        CREDO_VERUM(argumentum_est(v, "pictura_2"));
        /* tabula 4 sine pictore */
        CREDO_VERUM(vicus_activam_ponere(v, "4"));
        dispensator_recomponere(app.d);
        scribere(mn, VII, "$pictor-new");
        iussum(&app, mn, VII, III);
        CREDO_VERUM(chorda_aequalis_literis(frons(v)->id,
            "4_dextrum_pictor"));
        CREDO_VERUM(argumentum_est(v, "pictura_3"));
        /* pictura nova vere condita et ostensa (non pictura lateris
         * propria, aeque vacua) */
        CREDO_VERUM(in_bibliotheca(vol, piscina, "pictura_3"));
        CREDO_VERUM(pictura_frontis(v)
            && pictor_documentum_numerus_vivorum(pictura_frontis(v))
               == ZEPHYRUM);
    }

    volumen_claudere(vol);
    volumen_claudere(vol_solus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "$pictor-next / $pictor-prev", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_pictura", SECTIONES);
}
