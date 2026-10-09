/* probatio_vicus_acervus.c - acervus lateris dextri per iussa
 * (vicus-latera S3c)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba).
 * Iussa in pagina sinistra: I: '$terminale' - petitio (nihil ante
 * pulsum), post pulsum terminale in fronte, focus dexter, dispositio
 * servata (id). II: '$scriba' scribam ordinariam in frontem reddit
 * (acervus non crescit). III: '$terminale' iterum = idem latus
 * (montatio eadem). IV: '$scriba(notae)' - latus novum
 * '3_dextrum_scriba_2', pagina 'notae' condita et visa. V:
 * '$pictor(x)'. VI: errores ('$scriba(Mala)', '$terminale(a, b)')
 * nihil petunt. VII: reapertura - ordo, ids, argumenta, pagina visus
 * servantur. */
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
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

/* ictus UNUS in cellulam paginae sinistrae: latus non focatum focat
 * et iussum sub ictu statim currit (Franus) */
interior vacuum
ictus (
         Vicus* v,
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)v;
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

interior i32
numerus_acervi (
    Vicus* v)
{
    redde xar_numerus(vicus_activa(v)->acervus);
}

interior VicusLatus*
acervi (
    Vicus* v,
      i32  k)
{
    redde (VicusLatus*)xar_obtinere(vicus_activa(v)->acervus, k);
}

interior b32
frons_est (
                  Vicus* v,
     constans character* id)
{
    VicusLatus* d;

    d = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    redde d && d->montata && chorda_aequalis_literis(d->id, id);
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

s32 principale (vacuum)
{
           Volumen* vol;
   VicusApplicatio  app;
   VicusApplicatio  app_altera;
             Vicus* v;
        ManusLudus* m;
            vacuum* terminale;
    ScribaMontatio* sm;

    piscina = piscina_generare_dynamicum("probatio_vicus_acervus",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_acervus");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    ictus(v, m, II, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m,
        "i$terminale $scriba $pictor(x)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(v, m, III, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m,
        "i$scriba(notae) $scriba(Mala)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(v, m, IV, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$terminale(a, b)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "3_dextrum_scriba"));

    imprimere("\n--- I: '$terminale' - in fronte post pulsum ---\n");
    ictus(v, m, II, III);
    /* petitio: nihil intra tractationem */
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(vicus_pulsare(v));
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "3_dextrum_terminale"));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_DEXTRUM));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_motus(app.d)->spatium,
        "3_dextrum_terminale");
    CREDO_VERUM(index_continet(vol,
        "<latus genus=\"terminale\" id=\"3_dextrum_terminale\"/>"));
    terminale = vicus_latus(vicus_activa(v), VICUS_DEXTRUM)->montatio;

    imprimere("\n--- II: '$scriba' - ordinaria in frontem ---\n");
    ictus(v, m, II, XIII);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "3_dextrum_scriba"));
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, ZEPHYRUM)->id,
        "3_dextrum_terminale");

    imprimere("\n--- III: '$terminale' iterum - idem latus ---\n");
    ictus(v, m, II, III);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "3_dextrum_terminale"));
    CREDO_VERUM(vicus_latus(vicus_activa(v), VICUS_DEXTRUM)->montatio
        == terminale);

    imprimere("\n--- IV: '$scriba(notae)' - latus novum ---\n");
    ictus(v, m, III, III);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), III);
    CREDO_VERUM(frons_est(v, "3_dextrum_scriba_2"));
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->argumentum, "notae");
    sm = (ScribaMontatio*)vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->montatio;
    CREDO_AEQUALIS_S32(scriba_liber_index(sm->liber,
        chorda_ex_literis("notae", piscina)), I);
    CREDO_VERUM(sm->doc == scriba_liber_pagina(sm->liber,
        chorda_ex_literis("notae", piscina)));
    /* sinister in pagina sua manet */
    CREDO_VERUM(((ScribaMontatio*)vicus_latus(vicus_activa(v),
        VICUS_SINISTRUM)->montatio)->doc != sm->doc);
    CREDO_VERUM(index_continet(vol, "id=\"3_dextrum_scriba_2\" "
        "argumentum=\"notae\""));

    imprimere("\n--- V: '$pictor(x)' ---\n");
    ictus(v, m, II, XX);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));

    imprimere("\n--- VI: errores nihil petunt ---\n");
    ictus(v, m, III, XX);
    CREDO_VERUM(nuntius_est(v,
        "scriba: nomen paginae invalidum: Mala"));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    ictus(v, m, IV, III);
    CREDO_VERUM(nuntius_est(v, "terminale: unum argumentum"));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    /* genus ignotum recusatur */
    CREDO_FALSUM(vicus_acervo_aperire(v, "ignotum", NIHIL));

    imprimere("\n--- VII: reapertura ---\n");
    CREDO_VERUM(vicus_applicatio_aedificare(&app_altera, piscina,
        intern,
        vol, NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app_altera.vicus;
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, ZEPHYRUM)->id,
        "3_dextrum_scriba");
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, I)->id,
        "3_dextrum_terminale");
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, II)->id,
        "3_dextrum_scriba_2");
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, II)->argumentum, "notae");
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, III)->argumentum, "x");
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    sm = (ScribaMontatio*)acervi(v, II)->montatio;
    CREDO_VERUM(sm->doc == scriba_liber_pagina(sm->liber,
        chorda_ex_literis("notae", piscina)));
    /* errores (VI) in sinistro: ictus focum ibi reliquit */
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
