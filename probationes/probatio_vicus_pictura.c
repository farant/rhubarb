/* probatio_vicus_pictura.c - $pictor-next / $pictor-prev (S3e-3)
 *
 * Compositio VERA (vicus_applicatio). Bibliotheca: "2_dextrum_pictor"
 * (tabula 2, montata), "x1" (C x LX), "x2" (LX x XL). In tabula 3
 * (scriba | scriba) pagina: "$pictor-next $pictor-prev". I: next sine
 * pictore in acervo: latus novum '3_dextrum_pictor' picturam primam
 * liberam ostendit (x1; "2_..." alibi aperta praeteritur). II: next -
 * x2, latus idem. III: next - circulo, "2_..." praeteritur, pictura
 * lateris ipsius. IV: prev - x2. V: dispositio argumentum servat;
 * reapertura x2 ostendit. VI: argumenta recusantur. VII: memoria
 * communis - tabula 2 x1 ostendit documento EODEM. VIII: nulla alia
 * pictura - nuntius. */
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

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

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
     constans character* textus)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, INSULA_EPHEMERA, "nuntius");
    redde a && chorda_aequalis_literis(*a, textus);
}

interior b32
index_continet (
                Volumen* vol,
     constans character* textus)
{
    chorda c;
       b32 inventum;

    c = volumen_plagulam_promere(vol, chorda_ex_literis("vicus/latera",
        piscina), piscina, &inventum);
    redde inventum && chorda_continet(c, chorda_ex_literis(textus,
        piscina));
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

s32 principale (vacuum)
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

    piscina = piscina_generare_dynamicum("probatio_vicus_pictura",
        CXXVIII * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_pictura");
    /* old-style suita: NECESSE extra sectionem non abrumpit */
    si (   !pictor_documentum_creare(piscina, intern, vol, "x1", C, LX,
               LXIV)
        || !pictor_documentum_creare(piscina, intern, vol, "x2", LX, XL,
               LXIV))
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    scribere(m, II, "$pictor-next $pictor-prev");
    scribere(m, III, "$pictor-next(a)");
    CREDO_VERUM(chorda_aequalis_literis(frons(v)->genus, "scriba"));

    imprimere("\n--- I: next sine pictore - latus novum, x1 ---\n");
    iussum(&app, m, II, III);
    CREDO_VERUM(chorda_aequalis_literis(frons(v)->id,
        "3_dextrum_pictor"));
    CREDO_VERUM(argumentum_est(v, "x1"));
    x1 = pictura_frontis(v);
    CREDO_NON_NIHIL(x1);
    si (!x1)
    {
        credo_imprimere_compendium();
        redde I;
    }
    CREDO_VERUM(x1->latitudo == C && x1->altitudo == LX);
    latus_pictoris = frons(v);

    imprimere("\n--- II: next - x2, latus idem ---\n");
    iussum(&app, m, II, III);
    CREDO_VERUM(frons(v) == latus_pictoris);
    CREDO_VERUM(argumentum_est(v, "x2"));
    CREDO_VERUM(pictura_frontis(v) && pictura_frontis(v)->latitudo == LX
        && pictura_frontis(v)->altitudo == XL);

    imprimere("\n--- III: next - circulo, '2_...' praeteritur ---\n");
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "3_dextrum_pictor"));

    imprimere("\n--- IV: prev - x2 ---\n");
    iussum(&app, m, II, XV);
    CREDO_VERUM(argumentum_est(v, "x2"));
    CREDO_VERUM(pictura_frontis(v) && pictura_frontis(v)->latitudo
        == LX);

    imprimere("\n--- V: dispositio et reapertura ---\n");
    CREDO_VERUM(index_continet(vol,
        "id=\"3_dextrum_pictor\" argumentum=\"x2\""));
    CREDO_VERUM(vicus_applicatio_aedificare(&app_altera, piscina,
        intern, vol, NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    CREDO_VERUM(vicus_activam_ponere(app_altera.vicus, "3"));
    CREDO_VERUM(argumentum_est(app_altera.vicus, "x2"));
    CREDO_VERUM(pictura_frontis(app_altera.vicus)
        && pictura_frontis(app_altera.vicus)->latitudo == LX);

    imprimere("\n--- VI: argumenta recusantur ---\n");
    ictus(m, III, III);
    CREDO_VERUM(nuntius_est(v, "pictor-next: nulla argumenta"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(argumentum_est(v, "x2"));

    imprimere("\n--- VII: memoria communis - tabula 2 ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "2"));
    dispensator_recomponere(app.d);
    scribere(m, II, "$pictor-next $pictor-prev");
    /* 2_... -> 3_dextrum_pictor (liber: latus 3 x2 ostendit) -> x1 */
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "3_dextrum_pictor"));
    iussum(&app, m, II, III);
    CREDO_VERUM(argumentum_est(v, "x1"));
    CREDO_VERUM(pictura_frontis(v) == x1);

    imprimere("\n--- VIII: nulla alia pictura ---\n");
    vol_solus = volumen_temporarium(piscina,
        "probatio_vicus_pictura_s");
    CREDO_VERUM(vicus_applicatio_aedificare(&app_sola, piscina, intern,
        vol_solus, NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    m = manus_ludus_creare(piscina, app_sola.d);
    CREDO_VERUM(vicus_activam_ponere(app_sola.vicus, "2"));
    dispensator_recomponere(app_sola.d);
    scribere(m, II, "$pictor-next");
    iussum(&app_sola, m, II, III);
    CREDO_VERUM(nuntius_est(app_sola.vicus,
        "pictor-next: nulla alia pictura"));
    CREDO_VERUM(argumentum_est(app_sola.vicus, ""));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    volumen_claudere(vol_solus);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
