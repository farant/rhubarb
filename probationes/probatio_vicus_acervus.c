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
 * servantur. VIII-XIV (colloquia-plan T4f): cyclus Cmd+] / Cmd+[ -
 * domus -> admonitiones -> colloquia -> agenda (calendarium "agenda")
 * -> domus, retrorsum, focus domus restitutus, cyclus ab insigni
 * coeptus, gradus duo in pulsu uno, API recusat, latus cycli imum,
 * calendarium sine argumento domus manet. */
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
    a = insula_ramus_attributum(&r, "nuntius");
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

interior b32
focus_est (
    Vicus* v,
      i32  latus)
{
    redde vicus_latus_focatum(v) == vicus_latus(vicus_activa(v), latus);
}

/* T4f: Cmd+] aut Cmd+[ premitur (petitio), deinde pulsus applicat */
interior vacuum
gradus (
          Vicus* v,
     ManusLudus* m,
      character  clavis)
{
    (vacuum)manus_ludus_clavem(m, clavis, MOD_SUPER);
    (vacuum)vicus_pulsare(v);
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
                          Volumen* vol_imum;
                  VicusApplicatio  app_imum;
    hic_manens constans character* cyclus[] = {
        "admonitiones", "colloquia", "calendarium" };
    hic_manens constans character* argumenta_cycli[] = {
        NIHIL, NIHIL, "agenda" };
    hic_manens constans character* cyclus_malus[] = {
        "admonitiones", "ignotum" };

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
        CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
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
        "terminale:3_dextrum_terminale");
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
        vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
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

    imprimere("\n--- VIII: Cmd+] - domus et cyclus ---\n");
    m = manus_ludus_creare(piscina, app_altera.d);
    dispensator_recomponere(app_altera.d);
    /* domus = pictor, focus sinister (VII) */
    CREDO_VERUM(manus_ludus_clavem(m, ']', MOD_SUPER));
    /* petitio: nihil intra tractationem */
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), V);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    CREDO_VERUM(focus_est(v, VICUS_DEXTRUM));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VI);
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    CREDO_VERUM(focus_est(v, VICUS_DEXTRUM));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VII);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->argumentum, "agenda");
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    imprimere("\n--- IX: Cmd+[ - retrorsum ---\n");
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), V);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VI);
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VII);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, IV)->id,
        "3_dextrum_calendarium");
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));
    /* focus dexter in domo: dexter redit */
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_DEXTRUM));

    imprimere("\n--- X: cyclus ab insigni coeptus ---\n");
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_SINISTRUM));
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    imprimere("\n--- XI: gradus duo in pulsu uno ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, ']', MOD_SUPER));
    CREDO_VERUM(manus_ludus_clavem(m, ']', MOD_SUPER));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), VI);
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    imprimere("\n--- XII: API recusat ---\n");
    CREDO_FALSUM(vicus_cyclo_gradi(v, II));
    CREDO_FALSUM(vicus_cyclum_ponere(v, cyclus_malus, NIHIL, II));
    /* cyclus prior manet */
    CREDO_VERUM(vicus_cyclo_gradi(v, I));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    CREDO_VERUM(vicus_cyclum_ponere(v, NIHIL, NIHIL, ZEPHYRUM));
    CREDO_FALSUM(vicus_cyclo_gradi(v, -I));
    CREDO_VERUM(vicus_cyclum_ponere(v, cyclus, argumenta_cycli, III));
    CREDO_VERUM(vicus_cyclo_gradi(v, -I));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));

    imprimere("\n--- XIII: calendarium sine argumento = domus ---\n");
    CREDO_VERUM(vicus_acervo_aperire(v, "calendarium", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), V);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, ']');
    gradus(v, m, ']');
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VIII);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium_2"));
    CREDO_CHORDA_AEQUALIS_LITERIS(vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->argumentum, "agenda");
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), V);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    CREDO_VERUM(chorda_vacua(vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->argumentum));

    imprimere("\n--- XIV: latus cycli imum acervi ---\n");
    vol_imum = volumen_temporarium(piscina, "probatio_vicus_acervus");
    CREDO_VERUM(volumen_plagulam_condere(vol_imum,
        chorda_ex_literis("vicus/latera", piscina),
        chorda_ex_literis("<tabulae activa=\"1\"><tabula id=\"1\" "
            "focus=\"sinistrum\"><latus genus=\"scriba\" "
            "id=\"1_sinistrum_scriba\"/><acervus><latus "
            "genus=\"admonitiones\" id=\"1_dextrum_admonitiones\"/>"
            "</acervus></tabula></tabulae>", piscina), "vicus:latera"));
    CREDO_VERUM(vicus_applicatio_aedificare(&app_imum, piscina, intern,
        vol_imum, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app_imum.vicus;
    m = manus_ludus_creare(piscina, app_imum.d);
    dispensator_recomponere(app_imum.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "1_dextrum_admonitiones"));
    /* domus = admonitiones: gradus ad latus domus praeteritur */
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "1_dextrum_colloquia"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "1_dextrum_calendarium"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "1_dextrum_admonitiones"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));
    gradus(v, m, '[');
    CREDO_VERUM(frons_est(v, "1_dextrum_calendarium"));
    gradus(v, m, '[');
    CREDO_VERUM(frons_est(v, "1_dextrum_colloquia"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "1_dextrum_admonitiones"));
    volumen_claudere(vol_imum);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
