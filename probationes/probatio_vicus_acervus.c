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
 * -> indagatio (D5 S3b) -> domus, retrorsum, focus domus restitutus,
 * cyclus ab insigni coeptus, gradus duo in pulsu uno, API recusat,
 * latus cycli imum, calendarium sine argumento domus manet. */
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
#include "negotia.h"
#include "negotia_visus.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    c = volumen_praesens_legere(vol, chorda_ex_literis("vicus/latera",
        piscina), piscina, &inventum);
    redde inventum && chorda_continet(c, chorda_ex_literis(textus,
        piscina));
}

/* T4f: cyclus Cmd+] et Cmd+[ (constantes - status nullus inter
 * sectiones) */
hic_manens constans character* constans cyclus[] = {
    "admonitiones", "colloquia", "calendarium" };
hic_manens constans character* constans argumenta_cycli[] = {
    NIHIL, NIHIL, "agenda" };
hic_manens constans character* constans cyclus_malus[] = {
    "admonitiones", "ignotum" };

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

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
                          Volumen* vol;
                  VicusApplicatio  app;
                  VicusApplicatio  app_altera;
                            Vicus* v;
                       ManusLudus* m;
                           vacuum* terminale;
                   ScribaMontatio* sm;
                          Piscina* piscina;
              InternamentumChorda* intern;
                          Volumen* vol_imum;
                  VicusApplicatio  app_imum;

    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
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

    CREDO_GRADUS("I: '$terminale' - in fronte post pulsum");
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
    CREDO_VERUM(index_continet(vol, piscina,
        "<latus genus=\"terminale\" id=\"3_dextrum_terminale\"/>"));
    terminale = vicus_latus(vicus_activa(v), VICUS_DEXTRUM)->montatio;

    CREDO_GRADUS("II: '$scriba' - ordinaria in frontem");
    ictus(v, m, II, XIII);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "3_dextrum_scriba"));
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, ZEPHYRUM)->id,
        "3_dextrum_terminale");

    CREDO_GRADUS("III: '$terminale' iterum - idem latus");
    ictus(v, m, II, III);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), II);
    CREDO_VERUM(frons_est(v, "3_dextrum_terminale"));
    CREDO_VERUM(vicus_latus(vicus_activa(v), VICUS_DEXTRUM)->montatio
        == terminale);

    CREDO_GRADUS("IV: '$scriba(notae)' - latus novum");
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
    CREDO_VERUM(index_continet(vol, piscina,
        "id=\"3_dextrum_scriba_2\" "
        "argumentum=\"notae\""));

    CREDO_GRADUS("V: '$pictor(x)'");
    ictus(v, m, II, XX);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app.d);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));

    CREDO_GRADUS("VI: errores nihil petunt");
    ictus(v, m, III, XX);
    CREDO_VERUM(nuntius_est(v, piscina,
        "scriba: nomen paginae invalidum: Mala"));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    ictus(v, m, IV, III);
    CREDO_VERUM(nuntius_est(v, piscina, "terminale: unum argumentum"));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    /* genus ignotum recusatur */
    CREDO_FALSUM(vicus_acervo_aperire(v, "ignotum", NIHIL));

    CREDO_GRADUS("VII: reapertura");
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

    CREDO_GRADUS("VIII: Cmd+] - domus et cyclus");
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
    CREDO_AEQUALIS_I32(numerus_acervi(v), VIII);
    CREDO_VERUM(frons_est(v, "3_dextrum_indagatio"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    CREDO_GRADUS("IX: Cmd+[ - retrorsum");
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), V);
    CREDO_VERUM(frons_est(v, "3_dextrum_indagatio"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VI);
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VII);
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), VIII);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    CREDO_CHORDA_AEQUALIS_LITERIS(acervi(v, IV)->id,
        "3_dextrum_indagatio");
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

    CREDO_GRADUS("X: cyclus ab insigni coeptus");
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_SINISTRUM));
    CREDO_VERUM(vicus_acervo_aperire(v, "admonitiones", NIHIL));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(frons_est(v, "3_dextrum_admonitiones"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_indagatio"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(frons_est(v, "3_dextrum_pictor"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    CREDO_GRADUS("XI: gradus duo in pulsu uno");
    CREDO_VERUM(manus_ludus_clavem(m, ']', MOD_SUPER));
    CREDO_VERUM(manus_ludus_clavem(m, ']', MOD_SUPER));
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_I32(numerus_acervi(v), VI);
    CREDO_VERUM(frons_est(v, "3_dextrum_colloquia"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_calendarium"));
    gradus(v, m, ']');
    CREDO_VERUM(frons_est(v, "3_dextrum_indagatio"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), IV);
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));

    CREDO_GRADUS("XII: API recusat");
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

    CREDO_GRADUS("XIII: calendarium sine argumento = domus");
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

    CREDO_GRADUS("XIV: latus cycli imum acervi");
    vol_imum = volumen_temporarium(piscina, "probatio_vicus_acervus");
    CREDO_VERUM(volumen_praesens_ponere(vol_imum,
        chorda_ex_literis("vicus/latera", piscina),
        chorda_ex_literis("<tabulae activa=\"1\"><tabula id=\"1\" "
            "focus=\"sinistrum\"><latus genus=\"scriba\" "
            "id=\"1_sinistrum_scriba\"/><acervus><latus "
            "genus=\"admonitiones\" id=\"1_dextrum_admonitiones\"/>"
            "</acervus></tabula></tabulae>", piscina)));
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
    CREDO_VERUM(frons_est(v, "1_dextrum_indagatio"));
    gradus(v, m, ']');
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "1_dextrum_admonitiones"));
    CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));
    gradus(v, m, '[');
    CREDO_VERUM(frons_est(v, "1_dextrum_indagatio"));
    gradus(v, m, '[');
    CREDO_VERUM(frons_est(v, "1_dextrum_calendarium"));
    gradus(v, m, '[');
    CREDO_VERUM(frons_est(v, "1_dextrum_colloquia"));
    gradus(v, m, '[');
    CREDO_AEQUALIS_I32(numerus_acervi(v), I);
    CREDO_VERUM(frons_est(v, "1_dextrum_admonitiones"));
    CREDO_GRADUS("XV: sancti (bibliotheca, $saints)");
    /* corpus fixturae; argumentum sanctum aperit */
    CREDO_VERUM(vicus_acervo_aperire(v, "sancti", "thomas-aquinas"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(frons_est(v, "1_dextrum_sancti"));
    {
        InsulaRamus  r;
             chorda* a;

        r = insula_ramus(v->repo, "sancti", "1_dextrum_sancti");
        a = insula_ramus_attributum(&r, "sanctus_apertus");
        CREDO_VERUM(a && chorda_aequalis_literis(*a, "thomas-aquinas"));
    }
    CREDO_GRADUS("XVI: negotia ($projects; porta: anulus et latus)");
    {
           Negotia* ng;
            chorda  id;
       InsulaRamus  r;
            chorda* a;

        /* scriptor alius in volumine vici (ut bin/negotia) */
        ng = negotia_aperire(vol_imum, NIHIL, NIHIL, piscina);
        CREDO_NECESSE_NON_NIHIL(ng);
        id = negotia_negotium_creare(ng, chorda_ex_literis("negotia",
            piscina), chorda_ex_literis("", piscina));
        CREDO_CHORDA_NON_VACUA(id);
        CREDO_VERUM(vicus_acervo_aperire(v, "negotia", NIHIL));
        (vacuum)vicus_pulsare(v);
        CREDO_VERUM(frons_est(v, "1_dextrum_negotia"));
        /* porta vera lateris (ictus in indice): anulus tabulae
         * [negotium], deinde latus novum (T5) */
        {
            NegotiaMontatio* nm;

            nm = (NegotiaMontatio*)vicus_latus(vicus_activa(v),
                VICUS_DEXTRUM)->montatio;
            CREDO_NECESSE(nm && nm->portae
                && nm->portae->negotium_aperire);
            CREDO_VERUM(nm->portae->negotium_aperire(nm->portae->ctx,
                id));
            CREDO_VERUM(   vicus_activa(v)->anulus
                        && xar_numerus(vicus_activa(v)->anulus) == I);
        }
        (vacuum)vicus_pulsare(v);
        CREDO_VERUM(frons_est(v, "1_dextrum_negotia_2"));
        r = insula_ramus(v->repo, "negotia", "1_dextrum_negotia_2");
        a = insula_ramus_attributum(&r, "negotium");
        CREDO_VERUM(a && chorda_aequalis(*a, id));
        CREDO_GRADUS("XVII: $projects(adumbratio/<pagina>)");
        {
               chorda pid;
            character arg[LXIV];
                  i32 k;

            pid = negotia_paginam_creare(ng, id,
                NEGOTIA_PAGINA_ADUMBRATIO, chorda_ex_literis("index",
                piscina), chorda_ex_literis("<adumbratio/>", piscina));
            CREDO_CHORDA_NON_VACUA(pid);
            sprintf(arg, "adumbratio/%.*s", (integer)VI,
                (constans character*)pid.datum + pid.mensura - VI);
            CREDO_VERUM(vicus_acervo_aperire(v, "negotia", arg));
            per (k = ZEPHYRUM; k < III; k++)
            {
                (vacuum)vicus_pulsare(v);
            }
            CREDO_VERUM(frons_est(v, "1_dextrum_negotia_3"));
            r = insula_ramus(v->repo, "negotia", "1_dextrum_negotia_3");
            a = insula_ramus_attributum(&r, "visus");
            CREDO_VERUM(a && chorda_aequalis_literis(*a, "adumbratio"));
            a = insula_ramus_attributum(&r, "adumbratio");
            CREDO_VERUM(a && chorda_aequalis(*a, pid));
            CREDO_GRADUS("XVIII: Cmd+J, foco sinistro (T3b)");
            CREDO_VERUM(negotia_paginam_mutare(ng, pid,
                chorda_ex_literis("index", piscina), chorda_ex_literis(
                "<adumbratio><columna><textus>b</textus></columna>"
                "</adumbratio>", piscina)));
            (vacuum)negotia_visus_reficere((NegotiaMontatio*)
                vicus_latus(vicus_activa(v), VICUS_DEXTRUM)->montatio);
            CREDO_VERUM(vicus_focum_ponere(v, VICUS_SINISTRUM));
            (vacuum)manus_ludus_clavem(m, 'j', MOD_SUPER);
            (vacuum)vicus_pulsare(v);
            CREDO_VERUM(focus_est(v, VICUS_SINISTRUM));
            CREDO_VERUM(frons_est(v, "1_dextrum_negotia_3"));
            r = insula_ramus(v->repo, "negotia", "1_dextrum_negotia_3");
            a = insula_ramus_attributum(&r, "revisio");
            CREDO_VERUM(a && a->mensura > ZEPHYRUM);
        }
    }
    volumen_claudere(vol_imum);
    volumen_claudere(vol);
}


/* ==================================================
 * negotia T5: anulus (Cmd+H / Cmd+L), per tabulam
 * ================================================== */

nomen structura {
            Volumen* vol;
    VicusApplicatio  app;
              Vicus* v;
         ManusLudus* m;
                i32  aperta;
          character  acta[LXIV];
} MundusAnuli;

/* anulus trium generum planorum, extra cyclum */
hic_manens constans character* constans anulus_tres[] = {
    "sancti", "negotia", "pictor" };
hic_manens constans character* constans argumenta_trium[] = {
    "thomas-aquinas", NIHIL, "anulus" };

interior MundusAnuli*
mundum_anuli (
    CredoContextus* c)
{
    MundusAnuli* w;

    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    w = (MundusAnuli*)piscina_allocare(c->piscina,
        magnitudo(MundusAnuli));
    memset(w, ZEPHYRUM, magnitudo(MundusAnuli));
    w->vol = volumen_temporarium(c->piscina, "probatio_vicus_anulus");
    si (   !w->vol
        || !vicus_applicatio_aedificare(&w->app, c->piscina,
               internamentum_creare(c->piscina), w->vol, CDLXXX,
               CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde NIHIL;
    }
    w->v = w->app.vicus;
    w->m = manus_ludus_creare(c->piscina, w->app.d);
    (vacuum)vicus_activam_ponere(w->v, "1");
    dispensator_recomponere(w->app.d);
    redde w;
}

/* frons dextra: genus et argumentum (NIHIL = sine) */
interior b32
frons_generis (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum)
{
    VicusLatus* d;

    d = vicus_latus(vicus_activa(v), VICUS_DEXTRUM);
    redde d && d->montata && chorda_aequalis_literis(d->genus, genus)
        && (argumentum ? chorda_aequalis_literis(d->argumentum,
               argumentum) : d->argumentum.mensura == ZEPHYRUM);
}

interior b32
campus_est_vici (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, titulus);
    redde a && chorda_aequalis_literis(*a, valor);
}

/* Cmd + runa (petitio), sine pulsu */
interior vacuum
anuli_clavis (
    MundusAnuli* w,
      character  runa)
{
    (vacuum)manus_ludus_clavem(w->m, runa, MOD_SUPER);
}

interior b32
aperitor_fictus (
                 vacuum* ctx,
     constans character* argumentum)
{
    MundusAnuli* w = (MundusAnuli*)ctx;

    w->aperta++;
    strncpy(w->acta, argumentum ? argumentum : "", LX);
    redde VERUM;
}

/* Cmd+L bis: tertium in fronte; nihil ante pulsum */
interior vacuum
_anulus_gradus (
    CredoContextus* c)
{
     MundusAnuli* w;
          chorda  frons;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, ZEPHYRUM));
    frons = vicus_latus(vicus_activa(w->v), VICUS_DEXTRUM)->id;
    anuli_clavis(w, 'l');
    anuli_clavis(w, 'l');
    CREDO_VERUM(chorda_aequalis(vicus_latus(vicus_activa(w->v),
        VICUS_DEXTRUM)->id, frons));
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "pictor", "anulus"));
    CREDO_VERUM(focus_est(w->v, VICUS_DEXTRUM));
    volumen_claudere(w->vol);
}

/* circulariter: L ab ultimo ad primum, H a primo ad ultimum */
interior vacuum
_anulus_circulus (
    CredoContextus* c)
{
    MundusAnuli* w;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, II));
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "sancti", "thomas-aquinas"));
    anuli_clavis(w, 'h');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "pictor", "anulus"));
    anuli_clavis(w, 'h');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "negotia", NIHIL));
    volumen_claudere(w->vol);
}

/* anulus unius aut nullus: acervus, frons, focus intacti */
interior vacuum
_anulus_unius (
    CredoContextus* c)
{
     MundusAnuli* w;
          chorda  frons;
             i32  n;
             i32  focus;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        I, ZEPHYRUM));
    CREDO_FALSUM(vicus_anulo_gradi(w->v, I));
    n      = numerus_acervi(w->v);
    frons  = vicus_latus(vicus_activa(w->v), VICUS_DEXTRUM)->id;
    focus  = vicus_activa(w->v)->focus;
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    CREDO_AEQUALIS_I32(numerus_acervi(w->v), n);
    CREDO_VERUM(chorda_aequalis(vicus_latus(vicus_activa(w->v),
        VICUS_DEXTRUM)->id, frons));
    CREDO_AEQUALIS_I32(vicus_activa(w->v)->focus, focus);
    CREDO_GRADUS("anulus nullus");
    CREDO_VERUM(vicus_anulum_ponere(w->v, NIHIL, NIHIL, ZEPHYRUM,
        ZEPHYRUM));
    CREDO_FALSUM(vicus_anulo_gradi(w->v, -I));
    anuli_clavis(w, 'h');
    (vacuum)vicus_pulsare(w->v);
    CREDO_AEQUALIS_I32(numerus_acervi(w->v), n);
    CREDO_AEQUALIS_I32(vicus_activa(w->v)->focus, focus);
    CREDO_GRADUS("refutationes");
    {
        hic_manens constans character* constans ignota[] =
            { "ignotum" };

        CREDO_FALSUM(vicus_anulum_ponere(w->v, ignota, NIHIL, I,
            ZEPHYRUM));
        CREDO_FALSUM(vicus_anulum_ponere(w->v, anulus_tres,
            argumenta_trium, III, III));
    }
    volumen_claudere(w->vol);
}

/* genus virtuale: aperitor argumentum accipit, acervus intactus */
interior vacuum
_anulus_aperitor (
    CredoContextus* c)
{
     MundusAnuli* w;
             i32  n;
    hic_manens constans character* constans genera[] = {
        "virtuale", "sancti" };
    hic_manens constans character* constans argumenta[] = {
        "a1", "thomas-aquinas" };

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_aperitorem_addere(w->v, "virtuale",
        aperitor_fictus, w));
    CREDO_FALSUM(vicus_aperitorem_addere(w->v, "virtuale",
        aperitor_fictus, w));
    CREDO_VERUM(vicus_anulum_ponere(w->v, genera, argumenta, II, I));
    n = numerus_acervi(w->v);
    anuli_clavis(w, 'l');
    CREDO_AEQUALIS_I32(w->aperta, ZEPHYRUM);
    (vacuum)vicus_pulsare(w->v);
    CREDO_AEQUALIS_I32(w->aperta, I);
    CREDO_LITERAE_AEQUALES(w->acta, "a1");
    CREDO_AEQUALIS_I32(numerus_acervi(w->v), n);
    volumen_claudere(w->vol);
}

/* aperitio post gradum anuli: petitio nova vexillum anuli prioris
 * (locus xar petitionum iterum adhibitus) non hereditat */
interior vacuum
_anulus_deinde_aperitio (
    CredoContextus* c)
{
    MundusAnuli* w;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, ZEPHYRUM));
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "negotia", NIHIL));
    CREDO_VERUM(vicus_acervo_aperire(w->v, "calendarium", "agenda"));
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "calendarium", "agenda"));
    volumen_claudere(w->vol);
}

/* Cmd+] cyclum gradit; positio anuli servata */
interior vacuum
_anulus_et_cyclus (
    CredoContextus* c)
{
    MundusAnuli* w;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, ZEPHYRUM));
    gradus(w->v, w->m, ']');
    CREDO_VERUM(frons_generis(w->v, "admonitiones", NIHIL));
    /* frons extra anulum: positio servata (0) -> negotia */
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "negotia", NIHIL));
    CREDO_AEQUALIS_I32(vicus_activa(w->v)->anulus_positio, I);
    volumen_claudere(w->vol);
}

/* dispositio: anulus et positio scripta et restituta */
interior vacuum
_anulus_restitutus (
    CredoContextus* c)
{
        MundusAnuli* w;
    VicusApplicatio  app;
        VicusTabula* t;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, ZEPHYRUM));
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(index_continet(w->vol, c->piscina,
        "<anulus positio=\"1\">"));
    CREDO_VERUM(vicus_applicatio_aedificare(&app, c->piscina,
        internamentum_creare(c->piscina), w->vol, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    CREDO_VERUM(vicus_activam_ponere(app.vicus, "1"));
    t = vicus_activa(app.vicus);
    CREDO_NECESSE(t != NIHIL);
    CREDO_VERUM(t->anulus != NIHIL && xar_numerus(t->anulus) == III);
    CREDO_AEQUALIS_I32(t->anulus_positio, I);
    volumen_claudere(w->vol);
}

/* tabula altera anulum non habet; prima servat */
interior vacuum
_anulus_per_tabulam (
    CredoContextus* c)
{
    MundusAnuli* w;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    CREDO_VERUM(vicus_anulum_ponere(w->v, anulus_tres, argumenta_trium,
        III, ZEPHYRUM));
    CREDO_VERUM(vicus_activam_ponere(w->v, "2"));
    CREDO_FALSUM(vicus_anulo_gradi(w->v, I));
    CREDO_VERUM(vicus_activam_ponere(w->v, "1"));
    CREDO_VERUM(vicus_anulo_gradi(w->v, I));
    volumen_claudere(w->vol);
}

/* negotia T6: negotium cum colloquio adiuncto - anulus [negotium,
 * colloquium]; Cmd+L: sessio ignota (fons non apertus) -> charta;
 * Resume cum sessione mala refutatur (terminale verum numquam: tmux et
 * claude verae currerent) */
interior vacuum
_colloquium_in_anulo (
    CredoContextus* c)
{
        MundusAnuli* w;
            Negotia* ng;
             chorda  id;
    NegotiaMontatio* nm;
        InsulaRamus  r;
                i32  n;

    w = mundum_anuli(c);
    CREDO_NECESSE(w != NIHIL);
    ng = negotia_aperire(w->vol, NIHIL, NIHIL, c->piscina);
    CREDO_NECESSE_NON_NIHIL(ng);
    id = negotia_negotium_creare(ng, chorda_ex_literis("negotia",
        c->piscina), chorda_ex_literis("", c->piscina));
    CREDO_VERUM(negotia_colloquium_adiungere(ng, id, chorda_ex_literis(
        "s9", c->piscina), chorda_ex_literis("/tmp/x", c->piscina)));
    CREDO_VERUM(vicus_acervo_aperire(w->v, "negotia", NIHIL));
    (vacuum)vicus_pulsare(w->v);
    nm = (NegotiaMontatio*)vicus_latus(vicus_activa(w->v),
        VICUS_DEXTRUM)->montatio;
    CREDO_NECESSE(nm && nm->portae && nm->portae->negotium_aperire);
    CREDO_VERUM(nm->portae->negotium_aperire(nm->portae->ctx, id));
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(   vicus_activa(w->v)->anulus
                && xar_numerus(vicus_activa(w->v)->anulus) == II);
    CREDO_VERUM(frons_generis(w->v, "negotia", chorda_ut_cstr(id,
        c->piscina)));

    CREDO_GRADUS("Cmd+L: charta colloquii");
    anuli_clavis(w, 'l');
    (vacuum)vicus_pulsare(w->v);
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "negotia", "colloquium/s9"));
    r = insula_ramus(w->v->repo, "negotia", chorda_ut_cstr(
        vicus_latus(vicus_activa(w->v), VICUS_DEXTRUM)->id,
        c->piscina));
    CREDO_VERUM(campus_est_vici(&r, "visus", "colloquium"));
    CREDO_VERUM(campus_est_vici(&r, "colloquium", "s9"));

    CREDO_GRADUS("Resume: sessio mala refutatur");
    nm = (NegotiaMontatio*)vicus_latus(vicus_activa(w->v),
        VICUS_DEXTRUM)->montatio;
    n  = numerus_acervi(w->v);
    CREDO_FALSUM(nm->portae->colloquium_resumere(nm->portae->ctx,
        chorda_ex_literis(";rm x", c->piscina),
        chorda_ex_literis("/tmp",
        c->piscina)));
    CREDO_FALSUM(nm->portae->colloquium_resumere(nm->portae->ctx,
        chorda_ex_literis("s9", c->piscina), chorda_ex_literis("",
        c->piscina)));
    (vacuum)vicus_pulsare(w->v);
    CREDO_AEQUALIS_I32(numerus_acervi(w->v), n);
    CREDO_GRADUS("Cmd+H: ad negotium");
    anuli_clavis(w, 'h');
    (vacuum)vicus_pulsare(w->v);
    CREDO_VERUM(frons_generis(w->v, "negotia", chorda_ut_cstr(id,
        c->piscina)));
    volumen_claudere(w->vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "acervus lateris dextri per iussa", _sectio_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "anulus gradus", _anulus_gradus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anulus circulus", _anulus_circulus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anulus unius", _anulus_unius, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anulus aperitor", _anulus_aperitor, NIHIL, NIHIL, NIHIL, NIHIL },
    { "anulus et cyclus", _anulus_et_cyclus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "anulus deinde aperitio", _anulus_deinde_aperitio, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "anulus restitutus", _anulus_restitutus, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "anulus per tabulam", _anulus_per_tabulam, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "colloquium in anulo", _colloquium_in_anulo, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_acervus", SECTIONES);
}
